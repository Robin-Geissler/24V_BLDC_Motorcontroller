/*
 * BLDC_driver.c
 *
 * Created: 26/11/2022 14:42:10
 *  
 */ 

#include "BLDC_driver.h"
#include "BLDC_Control_Law.h"


/************************************************************************/
/* Init Functions													*/
/************************************************************************/
void init_PWM(){
	hri_tcc_write_CTRLA_ENABLE_bit(TCC1, 0 << TCC_CTRLA_ENABLE_Pos); /* Enable: disabled */
	hri_tcc_write_WAVE_reg(TCC1,
	1 << TCC_WAVE_SWAP3_Pos         /* Swap DTI Output Pair 3: disabled */
	| 0 << TCC_WAVE_SWAP2_Pos   /* Swap DTI Output Pair 2: disabled */
	| 0 << TCC_WAVE_SWAP1_Pos   /* Swap DTI Output Pair 1: disabled */
	| 0 << TCC_WAVE_SWAP0_Pos   /* Swap DTI Output Pair 0: disabled */
	| 0 << TCC_WAVE_CICCEN3_Pos /*  Circular Channel 3 Enable: disabled */
	| 0 << TCC_WAVE_CICCEN2_Pos /*  Circular Channel 2 Enable: disabled */
	| 0 << TCC_WAVE_CICCEN1_Pos /*  Circular Channel 1 Enable: disabled */
	| 0 << TCC_WAVE_CICCEN0_Pos /*  Circular Channel 0 Enable: disabled */
	| 0 << TCC_WAVE_POL5_Pos    /* Channel 5 Polarity: disabled */
	| 0 << TCC_WAVE_POL4_Pos    /* Channel 4 Polarity: disabled */
	| 0 << TCC_WAVE_POL3_Pos    /* Channel 3 Polarity: disabled */
	| 0 << TCC_WAVE_POL2_Pos    /* Channel 2 Polarity: disabled */
	| 0 << TCC_WAVE_POL1_Pos    /* Channel 1 Polarity: disabled */
	| 0 << TCC_WAVE_POL0_Pos    /* Channel 0 Polarity: disabled */
	| 0 << TCC_WAVE_CIPEREN_Pos /* Circular period Enable: disabled */
	| 0 << TCC_WAVE_RAMP_Pos    /* Ramp Mode: 0 */
	| 5);                       /* Waveform Generation: 2 */
	hri_tcc_write_WEXCTRL_OTMX_bf(TCC1, 2);
	hri_tcc_write_WEXCTRL_DTHS_bf(TCC1, 30);
	hri_tcc_write_WEXCTRL_DTLS_bf(TCC1, 30);
	hri_tcc_write_WEXCTRL_DTIEN0_bit(TCC1, 1);
	hri_tcc_write_WEXCTRL_DTIEN1_bit(TCC1, 1);
	hri_tcc_write_WEXCTRL_DTIEN2_bit(TCC1, 1);
	hri_tcc_write_WEXCTRL_DTIEN3_bit(TCC1, 1);
	hri_tcc_write_CTRLA_ENABLE_bit(TCC1, 1 << TCC_CTRLA_ENABLE_Pos); /* Enable: enabled */
}

/************************************************************************/
/* BLDC Subfunctions													*/
/************************************************************************/
/*set the PWM dutycycle to a value between 0 and 100. Return -1 if DC is invalid and 0 if ok*/
int set_PWM_dutycycle(float dutycycle)
{
	if(dutycycle>100)
	{
		dutycycle = 100;
	}
	
	if(dutycycle>=0 && dutycycle<=100)
	{	
		hri_tcc_write_CCBUF_reg(TCC1, 0, (uint16_t)((PWM_25KHZ_PERIOD - (dutycycle*PWM_25KHZ_PERIOD)/100)));
	}
	else
	{
		return -1;
	}
	
	return 0;
}

void init_currentPID(){
	currentPIDControl.desired = 0;
	currentPIDControl.Kp = 0;
	currentPIDControl.Ki = 0;
	currentPIDControl.Kd = 0;
	currentPIDControl.N = 0;
	currentPIDControl.max = 0;
	currentPIDControl.min = 0;
	currentPIDVar.y_k[0] = 0.0;
	currentPIDVar.y_k[1] = 0.0;
	currentPIDVar.y_k[2] = 0.0;
	currentPIDVar.e_k[0] = 0.0;
	currentPIDVar.e_k[1] = 0.0;
	currentPIDVar.e_k[2] = 0.0;
	currentPIDVar.d_y[0] = 0.0;
	currentPIDVar.d_y[1] = 0.0;
	currentPIDVar.d_y[2] = 0.0;
	
}

void init_speedPID(){
	speedPIDControl.desired = 0;
	speedPIDControl.Kp = 0;
	speedPIDControl.Ki = 0;
	speedPIDControl.Kd = 0;
	speedPIDControl.N = 0;
	speedPIDControl.max = 0;
	speedPIDControl.min = 0;
	speedPIDVar.y_k[0] = 0.0;
	speedPIDVar.y_k[1] = 0.0;
	speedPIDVar.y_k[2] = 0.0;
	speedPIDVar.e_k[0] = 0.0;
	speedPIDVar.e_k[1] = 0.0;
	speedPIDVar.e_k[2] = 0.0;
	speedPIDVar.d_y[0] = 0.0;
	speedPIDVar.d_y[1] = 0.0;
	speedPIDVar.d_y[2] = 0.0;
}

void init_posPID(){
	posPIDControl.desired = 0;
	posPIDControl.Kp = 0;
	posPIDControl.Ki = 0;
	posPIDControl.Kd = 0;
	posPIDControl.N = 0;
	posPIDControl.max = 0;
	posPIDControl.min = 0;
	posPIDVar.y_k[0] = 0.0;
	posPIDVar.y_k[1] = 0.0;
	posPIDVar.y_k[2] = 0.0;
	posPIDVar.e_k[0] = 0.0;
	posPIDVar.e_k[1] = 0.0;
	posPIDVar.e_k[2] = 0.0;
	posPIDVar.d_y[0] = 0.0;
	posPIDVar.d_y[1] = 0.0;
	posPIDVar.d_y[2] = 0.0;
}

void init_torquePID(){
	torquePIDControl.desired = 0;
	torquePIDControl.Kp = 0;
	torquePIDControl.Ki = 0;
	torquePIDControl.Kd = 0;
	torquePIDControl.N = 0;
	torquePIDControl.max = 0;
	torquePIDControl.min = 0;
	torquePIDVar.y_k[0] = 0.0;
	torquePIDVar.y_k[1] = 0.0;
	torquePIDVar.y_k[2] = 0.0;
	torquePIDVar.e_k[0] = 0.0;
	torquePIDVar.e_k[1] = 0.0;
	torquePIDVar.e_k[2] = 0.0;
	torquePIDVar.d_y[0] = 0.0;
	torquePIDVar.d_y[1] = 0.0;
	torquePIDVar.d_y[2] = 0.0;
}


/*Read Hallsensor values and find the current commutation state*/
void BLDC_find_step()
{
	last_BLDC_commutation_state = current_BLDC_commutation_state;
	
	uint32_t portlevel = (0x0070 & gpio_get_port_level(0)) >> 4;		
	switch(portlevel)
	{
		/* 110 */ 
		case 6:
			current_BLDC_commutation_state = 0;
			break;
		/* 010 */ 	
		case 2:
			current_BLDC_commutation_state = 1;
			break;
		/* 011 */
		case 3:
			current_BLDC_commutation_state = 2;
			break;
		/* 001 */	
		case 1:
			current_BLDC_commutation_state = 3;
			break;
		/* 101 */	
		case 5:
			current_BLDC_commutation_state = 4;
			break;
		/* 100 */	
		case 4:
			current_BLDC_commutation_state = 5;
			break;
			
		default:
			current_BLDC_commutation_state = -1;
	}
}

void boundary_Check(float *input, float max, float min){
	if(*input > max)
 	{
 		*input = max;
 	} else if (*input < min){
		*input = min;
	}
}


/*Calculate BLDC revolutions*/
void BLDC_revolution_counter()
{
	if (current_BLDC_commutation_state==5 && last_BLDC_commutation_state==0)
	{
		BLDC_revolutions--;
	}
	else if(current_BLDC_commutation_state==0 && last_BLDC_commutation_state==5)
	{
		BLDC_revolutions++;
	}
	
	BLDC_relative_steps = BLDC_revolutions*6.0 + current_BLDC_commutation_state;
}


/************************************************************************/
/* BLDC Drive Function                                                  */
/************************************************************************/
void BLDC_drive(float DirectionAndDutycycle)
{
	volatile int16_t cmd_direction = 0;
	volatile float dutycycle = 0;

	//interprete input
	if (DirectionAndDutycycle > 0)
	{
	    cmd_direction = 1;
		dutycycle = DirectionAndDutycycle;
	}
	else if (DirectionAndDutycycle < 0)
	{
		cmd_direction = -1;
		dutycycle = -DirectionAndDutycycle;
	}

	//activate driver chip
	gpio_set_pin_level(NSLEEP,true);
	
	//read hallsensor
	BLDC_find_step();
	
	// get revolutions
	BLDC_revolution_counter();
	
	/*set the commutation step for pattern*/
	//If motor commutation is in extreme, we look at last and current step to determine the motor direction
	if(current_BLDC_commutation_state != last_BLDC_commutation_state)
	{
		switch(current_BLDC_commutation_state)
		{
			//Normal cases
			case 1:
			case 2:
			case 3:
			case 4:
				current_BLDC_direction = current_BLDC_commutation_state - last_BLDC_commutation_state;
				break;
			
			//extreme cases
			case 0:
				if (last_BLDC_commutation_state == 5)
				{
					current_BLDC_direction = 1;
				}
				else
				{
					current_BLDC_direction = -1;
				}
				
				break;
				
			case 5:
				if(last_BLDC_commutation_state == 0)
				{
					current_BLDC_direction = -1;
				}
				else
				{
					current_BLDC_direction = 1;
				}
				
				break;
		}
	}
	
	
	
	/*Set PWM*/
	set_PWM_dutycycle(dutycycle);
	
	/*set pattern*/
	if(cmd_direction >= 0)
	{
		TCC1->PATTBUF.reg = BLDC_commutation_clockwise[current_BLDC_commutation_state];
	}
	else
	{
		TCC1->PATTBUF.reg = BLDC_commutation_counter_clockwise[current_BLDC_commutation_state];
	}
}

void BLDC_control_current(){
	/* 1. Current Control*/
	/* constraints check*/
	if(currentPIDControl.desired > speedPIDControl.max){
 		currentPIDControl.desired = speedPIDControl.max;
	} else if(currentPIDControl.desired  < speedPIDControl.min){
 			currentPIDControl.desired = speedPIDControl.min;
 	}
 			
 	/*Voltage calculation*/
	currentPIDVar = pid_controller(currentPIDVar, currentPIDControl.desired, current_reference_current_value,currentPIDControl );
	
	/* 2. Voltage Control*/
	control_voltage_output = currentPIDVar.y_k[0] / MOTOR_VOLTAGE * 100;
	if(control_voltage_output > MAX_PWM){
		control_voltage_output = MAX_PWM;
		} else if(control_voltage_output  < MIN_PWM){
		control_voltage_output = MIN_PWM;
	}	
}

void BLDC_control_speed(){	
	/* 1. Speed Control*/
	/* constraints check*/
	if(speedPIDControl.desired > posPIDControl.max){
		speedPIDControl.desired = posPIDControl.max;
	} else if(speedPIDControl.desired  < posPIDControl.min){
		speedPIDControl.desired = posPIDControl.min;
	}
	/*Current Calculation*/
	speedPIDVar = pid_controller(speedPIDVar, speedPIDControl.desired, current_BLDC_motor_rpm,speedPIDControl );
	
	/* 2. Current Control*/
	/*Voltage calculation*/
	currentPIDVar = pid_controller(currentPIDVar, speedPIDVar.y_k[0], current_reference_current_value,currentPIDControl );
	
	/* 3. Voltage Control*/
	control_voltage_output = currentPIDVar.y_k[0]/ MOTOR_VOLTAGE * 100;
	if(control_voltage_output > MAX_PWM){
		control_voltage_output = MAX_PWM;
		} else if(control_voltage_output  < MIN_PWM){
		control_voltage_output = MIN_PWM;
	}			
}

void BLDC_control_position(){
	/* 1. Position Control*/
	/* constraints check*/
	if(posPIDControl.desired > MAX_POS){
		posPIDControl.desired = MAX_POS;
	} else if(posPIDControl.desired < MIN_POS){
		posPIDControl.desired = MIN_POS;
	}
	/*Speed Calculation*/
	posPIDVar = pid_controller(posPIDVar, posPIDControl.desired, encoder_joint_pos,posPIDControl );
	
	
	/* 3. Speed Control*/
	/*Current calculation*/
	speedPIDVar = pid_controller(speedPIDVar, -posPIDVar.y_k[0], current_BLDC_motor_rpm,speedPIDControl );
	
	/* 2. Current Control*/
	/*Voltage calculation*/
	currentPIDVar = pid_controller(currentPIDVar, speedPIDVar.y_k[0], current_reference_current_value,currentPIDControl );
	
	/* 3. Voltage Control*/
	control_voltage_output = currentPIDVar.y_k[0]/ MOTOR_VOLTAGE * 100;	
	
	if(control_voltage_output > MAX_PWM){
	control_voltage_output = MAX_PWM;
	} else if(control_voltage_output  < MIN_PWM){
	control_voltage_output = MIN_PWM;
	}	
}

void BLDC_control_torque(){	
	/* 1. Torque Control*/
	/* constraints check*/
	if(torquePIDControl.desired > MAX_TORQUE){
		torquePIDControl.desired = MAX_TORQUE;
		} else if(torquePIDControl.desired < MIN_TORQUE){
		torquePIDControl.desired = MIN_TORQUE;
	}
	
	/*Current Calculation*/
	torquePIDVar = pid_controller(torquePIDVar, torquePIDControl.desired, joint_torque ,torquePIDControl );
	
	/* 2. Current Control*/
	/*Voltage calculation*/
	currentPIDVar = pid_controller(currentPIDVar, torquePIDVar.y_k[0], current_reference_current_value,currentPIDControl );
	
	/* 3. Voltage Control*/
	control_voltage_output = currentPIDVar.y_k[0]/ MOTOR_VOLTAGE * 100;
	if(control_voltage_output > MAX_PWM){
		control_voltage_output = MAX_PWM;
		} else if(control_voltage_output  < MIN_PWM){
		control_voltage_output = MIN_PWM;
	}			
}


				
/************************************************************************/
/* Get functions                                                    */
/************************************************************************/
uint32_t get_BLDC_commutation_state()
{
	return current_BLDC_commutation_state;	
}

float get_BLDC_relative_steps()
{
	return BLDC_relative_steps;
}

uint16_t get_BLDC_revolutions()
{
	return BLDC_revolutions;
}

int8_t get_BLDC_direction()
{
	return current_BLDC_direction;
}


/************************************************************************/
/* Button controlled functions                                          */
/************************************************************************/
/*check SW0 with debouncing function*/
// void check_sw0_pressed()
// {
// 	/*Task 3.1. Change PWM Dutycycle with SW0*/
// 	sw0_button_pressed_flag = 0;
// 	for(int i=0;i<5;i++)
// 	{
// 		sw0_button_pressed_flag += gpio_get_pin_level(SW0);
// 	}
// 	//value will add up to zero if button stayed pressed for 10 readings. eliminate signal fluctuation observed
// 	if(sw0_button_pressed_flag!=0 && sw0_button_hold_flag==1)
// 	{
// 		//gpio_toggle_pin_level(TESTPIN);
// 		button_controlled_dutycycle_state_machine();
// 		sw0_button_hold_flag = 0;
// 	}
// 	else if (sw0_button_pressed_flag==0 && sw0_button_hold_flag==0)
// 	{
// 		sw0_button_hold_flag = 1;
// 	}
//}


/*State machine for changing DC with button press*/
void button_controlled_dutycycle_state_machine()
{
	switch(current_button_controlled_dutycycle_state)
	{
		case 0:
		set_PWM_dutycycle(5);
		current_button_controlled_dutycycle_state = 1;
		break;
		case 1:
		set_PWM_dutycycle(50);
		current_button_controlled_dutycycle_state = 2;
		break;
		case 2:
		set_PWM_dutycycle(75);
		current_button_controlled_dutycycle_state = 0;
		break;
		default:
		current_button_controlled_dutycycle_state = 0;
	}
}

