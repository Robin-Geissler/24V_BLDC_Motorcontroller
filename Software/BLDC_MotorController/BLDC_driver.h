/*
 * BLDC_driver.h
 *
 * Created: 26/11/2022 14:41:58
 *  
 */ 


#ifndef BLDC_DRIVER_H_
#define BLDC_DRIVER_H_

#include <atmel_start.h>
#include "BLDC_Control_Law.h"

#define PWM_25KHZ_PERIOD 0x7d2
//#define DUTYCYCLE_05_PERCENT (PWM_25KHZ_PERIOD - 0x64)
//#define DUTYCYCLE_50_PERCENT (PWM_25KHZ_PERIOD - 0x3e9)
//#define DUTYCYCLE_75_PERCENT (PWM_25KHZ_PERIOD - 0x5DD)

#define CONTROL_SET_CURRENT 0
#define CONTROL_SET_SPEED 1
#define CONTROL_SET_POSITION 2
#define CONTROL_SET_TORQUE 3

#define MOTOR_VOLTAGE 24
#define MAX_VOLTAGE 12
#define MIN_VOLTAGE -12
#define MAX_PWM 50		/* MAX PWM Duty Cycle */
#define MIN_PWM -50		/* MAX REVERSE PWM DUTY CYCLE */
#define MAX_POS 140		/* MAX Joint Position in Degrees*/
#define MIN_POS 0		/* MIN Joint Position in Degrees*/
#define MAX_TORQUE 4	/* MAX Torque Value in Nm*/
#define MIN_TORQUE -4	/* MIN Torque Value in Nm*/



/************************************************************************/
/* Global Variables                                                     */
/************************************************************************/

volatile static int current_button_controlled_dutycycle_state = 0;
volatile static uint32_t current_BLDC_commutation_state = 0;
volatile static int last_BLDC_commutation_state = -1;
volatile static int8_t current_BLDC_direction = 0;
volatile static uint8_t current_BLDC_dutycycle = 0;
volatile static int32_t BLDC_revolutions = 0;
volatile static float BLDC_relative_steps = 0;
//volatile static int32_t current_BLDC_motor_rpm=0;



/* Pattern Buffer Settings for the 6 commutations states*/
/* State0, State1, State2, State3, State4, State5*/	
static uint16_t BLDC_commutation_clockwise[] = {0x082D, 0x202D, 0x2036, 0x1036, 0x101B, 0x081B};
static uint16_t BLDC_commutation_counter_clockwise[] = {0x1036, 0x101B, 0x081B, 0x082D, 0x202D, 0x2036};

/*EXplained Pro Board Pattern Buffer*/
// static uint16_t BLDC_commutation_clockwise[] = {0x507C, 0x607C, 0x30F4, 0x50F4, 0x60F8, 0x30F8};
// static uint16_t BLDC_commutation_counter_clockwise[] = {0x50F4, 0x60F8, 0x30F8, 0x507C, 0x607C, 0x30F4};



/* Current Measurements */
extern volatile uint16_t ADC_IB_value;
extern volatile uint16_t ADC_IC_value;
extern volatile float currentC_value;
extern volatile float currentB_value;
extern volatile float current_reference_current_value;
extern volatile float last_reference_current_value;
extern volatile uint8_t new_current_value_available_flag;

/* Speed Measurements */
extern volatile float current_BLDC_motor_rpm;
extern volatile uint16_t CC0_value ;
extern volatile uint16_t CC1_value ;
extern volatile uint8_t new_speed_value_available_flag;

/* Position Measurements */
extern volatile float encoder_joint_pos;
extern unsigned int encoder_joint_warning_bit;
extern unsigned int encoder_joint_error_bit;
extern volatile char encoder_spi_reply[7];
extern volatile int new_encoder_reading_flag ;
extern volatile int read_encoder_flag;

extern volatile float joint_torque;

/*controller values*/
extern volatile float error_current;
extern volatile float error_speed;
extern volatile float error_position;
extern volatile float control_voltage_output;
extern volatile float current_control_output[4];		//Output - prev. Output - prev. error - not used
extern volatile float speed_control_output[4];		//Output - prev. Output - prev. error - not used
extern volatile float position_control_output[4];		//Output - prev. Output - prev. error - not used

//general
extern volatile uint16_t input_control_mode;		//turn on/off controller
extern volatile uint16_t input_control_set;		//select between inputs

//current control
extern volatile float input_desired_current;		//target current
extern volatile float input_max_current;			//max possible current
extern volatile float input_min_current;			//max negative current
extern volatile float input_kp_current;			//p gain current control
extern volatile float input_ki_current;			//i gain current control

//speed control
extern volatile float input_desired_speed;			//target speed
extern volatile float input_speed_max;		//max possible motor speed
extern volatile float input_speed_min;		//max negative motor speed
extern volatile float input_kp_speed_control;		//p gain position control
extern volatile float input_ki_speed_control;		//p gain position control

//position control
extern volatile float input_desired_position;		//target position
extern volatile float input_max_position;			// max position
extern volatile float input_min_position;			// max neg. position
extern volatile float input_kp_position_control;	//p gain position control
extern volatile float input_ki_position_control;

PIDvariable currentPIDVar;
PIDcontrol currentPIDControl;
PIDvariable speedPIDVar;
PIDcontrol speedPIDControl;
PIDvariable posPIDVar;
PIDcontrol posPIDControl;
PIDvariable torquePIDVar;
PIDcontrol torquePIDControl;

//feedforward voltage control
extern volatile float input_kt_feedforward;

/************************************************************************/
/* Functions                                                            */
/************************************************************************/

/* Init Functions */
void init_PWM();
void init_currentPID();
void init_speedPID();
void init_posPID();
void init_posPID();

/*Button controlled functions*/
void check_sw0_pressed();
void button_controlled_dutycycle_state_machine();

/*General BLDC functions*/
int set_PWM_dutycycle(float dutycycle);
void BLDC_find_step();
void BLDC_revolution_counter();
void BLDC_drive(float DirectionAndDutycycle);

void BLDC_control_current();
void BLDC_control_speed();
void BLDC_control_position();

/*Get functions*/
uint32_t get_BLDC_commutation_state();
float get_BLDC_relative_steps();
uint16_t get_BLDC_revolutions();
int8_t get_BLDC_direction();

#endif /* BLDC_DRIVER_H_ */