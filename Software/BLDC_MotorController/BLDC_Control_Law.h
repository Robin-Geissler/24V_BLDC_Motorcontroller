/*
 * BLDC_Control_Law.h
 *
 * Created: 12/2/2022 9:55:22 AM
 *  
 */ 

#include "atmel_start.h"

#ifndef BLDC_CONTROL_LAW_H_
#define BLDC_CONTROL_LAW_H_


#define Fs (25000)	//Sampling Rate


/************************************************************************/
/* Structures                                                            */
/************************************************************************/

typedef struct {
	float        desired;
	float        Kp;
	float        Ki;
	float        Kd;
	uint32_t      N;
	float        max;
	float		min;
}PIDcontrol;

typedef struct {
	float y_k[3];
	float e_k[3];
	float d_y[3];
}PIDvariable;

/************************************************************************/
/* Variables                                                            */
/************************************************************************/




/************************************************************************/
/* Functions                                                            */
/************************************************************************/

void pi_controller(float *out, float error, float Kp, float Ki);
float feedforward(float *out, float kt, float motor_speed);
PIDvariable pid_controller(PIDvariable out, float x_d, float x, PIDcontrol K);



#endif /* BLDC_CONTROL_LAW_H_ */