/************************************************************************/
/* Integrated Software!!!                                               */
/************************************************************************/

/************************************************************************/
/* Important!!!                                                         */
/************************************************************************/
//Timer for speed capture: Waveform is NFRQ!
//Remember to copy relevant ECAT include files!!!


/************************************************************************/
/* Headers                                                              */
/************************************************************************/
#include <atmel_start.h>
#include "BLDC_driver.h"
#include <math.h>
#include "encoder_driver.h"
#include "qspi_ethercat.h"


/************************************************************************/
/* Definitions                                                          */
/************************************************************************/


#define INPUTCTRL_REG_IC 0x1803		//positive mux current pin (PB09), negative mux internal gnd, non diff mode
#define INPUTCTRL_REG_TS 0x0180		//pos mux vout, neg mux vref, diff mode on
#define ADC_TS_OFFSET -7.0f			//minus 80 (-80) in idle position


/************************************************************************/
/* Global Variables                                                     */
/************************************************************************/


/* Current Measurements */
volatile uint16_t ADC_IB_value;
volatile uint16_t ADC_IC_value;
volatile float currentC_value;
volatile float currentB_value;
volatile float current_reference_current_value = 0;
volatile float last_reference_current_value = 0;
volatile uint8_t new_current_value_available_flag = 0;

/* Speed Measurements */
volatile  float current_BLDC_motor_rpm = 0;
volatile uint16_t CC0_value ;
volatile uint16_t CC1_value ;
volatile uint8_t new_speed_value_available_flag = 0;

/* Position Measurements */
volatile float encoder_joint_pos = 0;
unsigned int encoder_joint_warning_bit = 1;
unsigned int encoder_joint_error_bit = 1;
volatile  char encoder_spi_reply[7] = {};
volatile int new_encoder_reading_flag = 1;
volatile int read_encoder_flag = 0;

/*controller values*/
volatile float error_current = 0;
volatile float error_speed = 0;
volatile float error_position = 0;
volatile float control_voltage_output = 0;
volatile float current_control_output[4] = {0};		//Output - prev. Output - prev. error - not used
volatile float speed_control_output[4] = {0};		//Output - prev. Output - prev. error - not used
volatile float position_control_output[4] = {0};		//Output - prev. Output - prev. error - not used

//general
volatile uint16_t input_control_mode;		//turn on/off controller
volatile uint16_t input_control_set;		//select between inputs

//current control
volatile float input_desired_current;		//target current
volatile float input_max_current;			//max possible current
volatile float input_min_current;			//max negative current
volatile float input_kp_current;			//p gain current control
volatile float input_ki_current;			//i gain current control

//speed control
volatile float input_desired_speed;			//target speed
volatile float input_speed_max;		//max possible motor speed
volatile float input_speed_min;		//max negative motor speed
volatile float input_kp_speed_control;		//p gain position control
volatile float input_ki_speed_control;		//p gain position control

//position control
volatile float input_desired_position;		//target position
 volatile float input_max_position;			// max position
 volatile float input_min_position;			// max neg. position
 volatile float input_kp_position_control;	//p gain position control
 volatile float input_ki_position_control;

//feedforward voltage control
 volatile float input_kt_feedforward = 0;



// ETHERCAT Timer Task
static struct timer_task TIMER_2_task_1kHz;



//Torque sensor
volatile int16_t ADC_TS_value;
volatile float joint_torque = 0.0;
volatile int new_torque_value_available_flag = 0;





/*Controller variables*/
volatile uint8_t activate_controller_flag = 0;








/************************************************************************/
/* Interrupt Service Routines                                           */
/************************************************************************/
/*ISR for speed measurement*/
void TC0_Handler() 
{
	
	if (TC0->COUNT16.INTFLAG.bit.MC0)
	{
		//read results of capture
		CC0_value = TC0->COUNT16.CC[0].reg;
		CC1_value = TC0->COUNT16.CC[1].reg;
		
		TC0->COUNT16.INTFLAG.bit.MC0 = 1;
		TC0->COUNT16.INTFLAG.bit.MC1 = 1;
	} else if (TC0->COUNT16.INTFLAG.bit.OVF){
		//If overflow, motor is not moving
		TC0->COUNT16.INTFLAG.bit.OVF = 1;
		CC0_value = TC0->COUNT16.CC[0].reg;
		CC1_value = TC0->COUNT16.CC[1].reg;
		CC0_value  = 0xFFFF;
	}
	
	//activate encoder
	read_encoder_flag = 1;
	
	//Flag for speed calculation
	new_speed_value_available_flag = 1;

	//clear error bit if any
	hri_tc_clear_INTFLAG_ERR_bit(TC0);
}

/*ISR for current measurement*/
void ADC0_1_Handler()
{
	//either only read TS, or both current sensors in parallel. Reading mode interchange every cycle
	if(ADC0->INPUTCTRL.reg == INPUTCTRL_REG_TS)
	{		
		ADC_TS_value = hri_adc_read_RESULT_reg(ADC0);
		
		//change ADC0 input control to current reading mode
		ADC0->INPUTCTRL.reg = INPUTCTRL_REG_IC;
		
		//Flush results reg 
		ADC0->SWTRIG.bit.FLUSH = 1;
		while (ADC0->SYNCBUSY.bit.SWTRIG == 1){}
			
		//set new torque flag
		new_torque_value_available_flag = 1;
	}
	else
	{
		ADC_IC_value = hri_adc_read_RESULT_reg(ADC0);
		ADC_IB_value = hri_adc_read_RESULT_reg(ADC1);
		
		//change ADC1 input control to torque sensor reading mode
		ADC0->INPUTCTRL.reg = INPUTCTRL_REG_TS;
		
		//Flush results reg 
		ADC0->SWTRIG.bit.FLUSH = 1;
		while (ADC1->SYNCBUSY.bit.SWTRIG == 1){}
			
		//Set new current flag
		new_current_value_available_flag = 1;
	}

	//clear interrupt flag
	hri_adc_clear_INTFLAG_reg(ADC0, 0xFF);
	hri_adc_clear_INTFLAG_reg(ADC1, 0xFF);
	
}


/*Dummy ADC1_1 Handler so that it shuts up ;)*/
void ADC1_1_Handler()
{
}

/*Callback function for Encoder SPI*/
static void complete_cb_SPI_ENC(const struct spi_m_async_descriptor *const io_descr)
{
	//New reading flag
	new_encoder_reading_flag = 1;
	
	//convert received value to angles between 0 and 110 degrees
	conversion_joint_encoder(encoder_spi_reply, &encoder_joint_pos, &encoder_joint_warning_bit, &encoder_joint_error_bit);
	
	//Pull NCS high to stop reading cycle
	delay_us(10); //can be removed
	gpio_set_pin_level(ENCODER_SPI_NCS, true);
}

/*Callback function for EtherCAT QSPI */
static void TIMER_2_task_1kHz_cb(const struct timer_task *const timer_task)
{
  	write_16_DWORDS_data();
  	config_fifo();
  	read_16_DWORDS_data();
	gpio_toggle_pin_level(STATUS_LED1);
	
	//clear_rdram();
}

/************************************************************************/
/* Subfunctions                                                        */
/************************************************************************/
float current_selector(uint32_t current_commutation_state)
{
	switch(current_commutation_state)
	{
		case 5:
			return (-currentC_value);
			break;
		
		case 3:
			return (currentC_value);
			break;
		
		case 4:
			return (-currentB_value);
			break;
		
		case 1:
			return (currentB_value);
			break;
		
		case 0:
			return (currentB_value);
			break;
		
		case 2:
			return (currentC_value);
			break;
			
		default:
			return 0;
	}
}


/************************************************************************/
/* Main Function                                                        */
/************************************************************************/
int main(void){
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();	

	/*enable CCL*/
	CCL->CTRL.bit.ENABLE = 1;
	
	/*set TCC1 CC0 to overwrite all WO/x*/
	TCC1->CTRLA.bit.ENABLE = 0;
	TCC1->WEXCTRL.bit.OTMX = 0x2;
	TCC1->CTRLA.bit.ENABLE = 1;
	
	
	/*ADC Configurations*/
	//Disable ADC Freerun
	ADC0->CTRLB.bit.FREERUN = 0;
	ADC1->CTRLB.bit.FREERUN = 0;
	
	//Initialize ADC0 for current A sensing
	ADC0->INPUTCTRL.reg = INPUTCTRL_REG_IC;
	
	//configure ADC interrupt
	ADC0->INTENSET.bit.RESRDY = 1;
	ADC0->EVCTRL.bit.RESRDYEO = 1;
	ADC0->CTRLA.bit.ENABLE = 1;
//	ADC1->INTENSET.bit.RESRDY = 1;
	ADC1->EVCTRL.bit.RESRDYEO = 1;
	ADC1->CTRLA.bit.ENABLE = 1;

	init_PWM();
	init_currentPID();
	init_speedPID();
	init_posPID();
	init_torquePID();
	
	/*set Motor driver NSLEEP pin*/
	gpio_set_pin_level(NSLEEP, true);
	gpio_set_pin_direction(NSLEEP, GPIO_DIRECTION_OUT);
	
	/*Nested Vector Interrupt settings*/
	__NVIC_EnableIRQ(TC0_IRQn);        
	__NVIC_SetPriority(ADC0_1_IRQn,3);
	__NVIC_EnableIRQ(ADC0_1_IRQn);
	__NVIC_SetPriority(ADC1_1_IRQn,3);
	__NVIC_EnableIRQ(ADC1_1_IRQn);
	
	__enable_irq();
	
	
	///*configure ADC interrupt*/
	//hri_adc_write_INTEN_RESRDY_bit(ADC0,1);
	//adc_async_enable_channel(&ADC_IB, 0);
	////hri_adc_write_INTEN_RESRDY_bit(ADC1,1);
	//adc_async_enable_channel(&ADC_IC, 0);

	////Trigger ADC first conversion
	//ADC0->SWTRIG.bit.START = 1;
	//ADC1->SWTRIG.bit.START = 1;
	
	/*Init UART for comp debug interface*/
// 	struct io_descriptor *io_comp;
// 	usart_sync_get_io_descriptor(&USART_COM, &io_comp);
// 	usart_sync_enable(&USART_COM);
	
	/*Init SPI for Joint encoder*/
	struct io_descriptor *io_enc_spi;
	spi_m_async_get_io_descriptor(&SPI_ENC, &io_enc_spi);
	spi_m_async_register_callback(&SPI_ENC, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)complete_cb_SPI_ENC);
	spi_m_async_enable(&SPI_ENC);
	gpio_set_pin_level(ENCODER_SPI_NCS, true);
	
	
	/*ETHERCAT NEW*/
	init_standard_messages();
	
	qspi_sync_enable(&QUAD_SPI_0);
	delay_ms(25);
	//initialize ethercat register
	abort_fifo();
	clear_rdram();
	
	/* Set up task with 1kHz repetition */
	TIMER_2_task_1kHz.interval = 1;	// every millisecond
	TIMER_2_task_1kHz.cb       = TIMER_2_task_1kHz_cb;
	TIMER_2_task_1kHz.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_2, &TIMER_2_task_1kHz);
	timer_start(&TIMER_2);
	
	
	/*ETHERCAT*/
// 	configure_ethercat_dma_descriptors();
// 	__NVIC_EnableIRQ(TC7_IRQn);
// 	__NVIC_EnableIRQ(DMAC_0_IRQn);

	volatile uint8_t  *status				=&send_data[0];
	volatile uint8_t  *run_mode			=(((uint8_t *)&send_data[0])+1);
	volatile uint16_t   *read_hall_sector	=(((uint16_t*)&send_data[0])+1);
	volatile float *read_position			=&send_data[1];
	volatile float  *read_speed		=&send_data[2];
	volatile float  *read_torque 	=&send_data[3]; //float
	volatile float *read_current_i_d		=&send_data[4];
	volatile float  *read_current_i_B		=&send_data[5];
	volatile float  *read_current_i_C			=&send_data[6];
	volatile float  *read_emergency			=&send_data[7];
	volatile float  *read_remain_1		=&send_data[8];
	volatile float  *read_duty_cycle		=&send_data[9];
	volatile float  *read_abs_duty_cycle		=&send_data[10];
	volatile float  *read_remain2		=&send_data[11];
	volatile float  *read_remain3				=&send_data[12];
	volatile float  *read_remain4				=&send_data[13];
	volatile float  *read_remain5				=&send_data[14];
	volatile float  *read_remain6				=&send_data[15];
	
	//read
	volatile	uint16_t *control_mode			=&read_data[0];
	volatile	uint16_t *control_set			=(((uint16_t *)&read_data[0])+1);
	volatile	float *desired_position			=&read_data[1];
	volatile	float *desired_torque			=&read_data[2];
	volatile	float *desired_speed			=&read_data[3];
	volatile	float *I_desired				=&read_data[4];
	volatile	float *speed_max				=&read_data[5];
	volatile	float *I_max					=&read_data[6];
	volatile	float *pos_kp					=&read_data[7];
	volatile	float *pos_ki					=&read_data[8];
	volatile    float *torque_kp			    =&read_data[9];
	volatile	float *torque_ki				=&read_data[10];
	volatile	float *speed_kp					=&read_data[11];
	volatile	float *speed_ki					=&read_data[12];
	volatile	float *i_kp						=&read_data[13];
	volatile	float *i_ki						=&read_data[14];
	volatile	float *remain					=&read_data[15];		
	

	
	/**** 
		Set PWM DUTY CYCLE to 0 for testing
	****/
	set_PWM_dutycycle(20);
	
	/*Main while loop*/
	while (1) {		
		
		
		/************************************************************************/
		/* Encoder Read                                                         */
		/************************************************************************/
		if(read_encoder_flag == 1 && new_encoder_reading_flag == 1)
		{
			//reset flags
			new_encoder_reading_flag = 0;
			read_encoder_flag = 0;
			
			//Print joint position on data visualization
			//print_encoder_joint_data(io_comp, &encoder_joint_pos,  &encoder_joint_warning_bit, &encoder_joint_error_bit);
			
			//pull chip select low for next read
			gpio_set_pin_level(ENCODER_SPI_NCS, false);	
			delay_us(10);
			
			//restart reading
			io_read(io_enc_spi,encoder_spi_reply,7);
		}


		/************************************************************************/
		/*Torque sensor value conversion                                        */
		/************************************************************************/
		if(new_torque_value_available_flag == 1)
		{
			//reset flag
			new_torque_value_available_flag = 0;
			
			joint_torque = (ADC_TS_value + ADC_TS_OFFSET)*1.7*3030.3/708/2048; //max at 12bit/2, 2164.5 from 5Nm/2.31mV for 5 Nm at 3.3V input, 708 Gain of Amplifier Circuit
			
		}
		
		
		/************************************************************************/
		/* Current calculation (RMBM TO CALIBRATE!)                             */
		/************************************************************************/
		if(new_current_value_available_flag == 1)
		{	
			currentC_value = ((ADC_IC_value-1870)*3.3/4096)*10/4;; //offset
			currentB_value = -((ADC_IB_value-1870)*3.3/4096)*10/4; //sensor is installed opposite!
			
			//reset flags
			new_current_value_available_flag = 0;
			activate_controller_flag = 1;
		}
		
		/************************************************************************/
		/* Speed calculation                                                    */
		/************************************************************************/
		/*calculate speed if flag is set*/
		if(new_speed_value_available_flag == 1)
		{
			if(CC0_value == 0xFFFF)
			{
				current_BLDC_motor_rpm  = 0; 
			}
			else 
			{
				current_BLDC_motor_rpm = 60*(12e6/(float)CC0_value)/42;
			}

			//direction opposite to current. Thats why minus
			current_BLDC_motor_rpm = -current_BLDC_motor_rpm * ((float)get_BLDC_direction());
			
			//reset flags
			new_speed_value_available_flag = 0;

		}
		
		
		/*Select reference current*/
		current_reference_current_value = current_selector(get_BLDC_commutation_state());


		/************************************************************************/
		/* Ethercat Pointers                                                    */
		/************************************************************************/
		//Output
		*read_hall_sector = get_BLDC_commutation_state();
		*read_position = encoder_joint_pos;
		*read_speed = current_BLDC_motor_rpm;
		*read_current_i_C = currentC_value;
		*read_current_i_B = currentB_value;
		*read_current_i_d = current_reference_current_value;
		*read_torque = joint_torque;
		*read_duty_cycle = control_voltage_output;
		*read_abs_duty_cycle = control_voltage_output;

		//Input
		input_control_mode = *control_mode;
		input_control_set = *control_set;
		currentPIDControl.desired = *I_desired;
		currentPIDControl.max = MAX_PWM;
		currentPIDControl.min = MIN_PWM;
		currentPIDControl.Kp = *i_kp;
		currentPIDControl.Ki = *i_ki;
		speedPIDControl.desired = *desired_speed;
		speedPIDControl.max = *I_max;
		speedPIDControl.min = -(*I_max);
		speedPIDControl.Kp = *speed_kp;
		speedPIDControl.Ki = *speed_ki;
		posPIDControl.desired = *desired_position;
		posPIDControl.max = *speed_max;
		posPIDControl.min = -(*speed_max);
		posPIDControl.Kp = *pos_kp;
		posPIDControl.Ki = *pos_ki;
		torquePIDControl.desired = *desired_torque;
		torquePIDControl.max = *I_max;
		torquePIDControl.min = -(*I_max);
		torquePIDControl.Kp = *torque_kp;
		torquePIDControl.Ki = *torque_ki;
		
		
		//Overwrites
// 		input_control_mode = 1;
// 		input_control_set = 2;
// 		input_kp_current = 1;
// 		input_ki_current = 1100;
// 		input_desired_current = 0.1;	
// 		input_max_current = 0.5;
// 		input_min_current = -input_max_current;
// 		input_kp_speed_control = 0.001;
// 		input_ki_speed_control = 0.03;
// 		input_kp_position_control = 200;
// 		input_ki_position_control = 0;
// 		input_kt_feedforward = 1/997; //inverse of speed constant from datasheet
// 		input_kt_feedforward = 0;
// 		input_desired_speed = 50;
// 		input_desired_position = 1000;
// 		input_speed_max = 1000;
		
/*		input_kp_position_control = *pos_kp;*/		
				
		
		
		/************************************************************************/
		/* Active Driver                                               */
		/************************************************************************/
 		if(activate_controller_flag == 1 && input_control_mode == 1)
 		{
 			/* reset control routine activation flag */
 			activate_controller_flag = 0;
 			
			 /* use control algorithm */
 			switch(input_control_set){
				case CONTROL_SET_CURRENT: BLDC_control_current();
				break;
				case CONTROL_SET_SPEED: BLDC_control_speed();
				break;
				case CONTROL_SET_POSITION: BLDC_control_position();
				break;
				case CONTROL_SET_TORQUE: BLDC_control_torque();
				break;
				default:
				control_voltage_output = 0;
			 }
			
			/*Drive the motor*/
			BLDC_drive(control_voltage_output);
		}
		/************************************************************************/
		/* Non-Active Driver										*/
		/************************************************************************/
		else
		{
			BLDC_drive(0);
		}

		/*Drive BLDC if to bypass controller*/
		//BLDC_drive(5);
 	}
}

