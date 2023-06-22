/*
 * BLDC_Control_Law.c
 *
 * Created: 12/2/2022 9:55:37 AM
 * 
 */ 

#include "BLDC_Control_Law.h"



/*PD controller*/
//float pd_controller(float error_speed, float speed_kp, float speed_kd)
//{
	//float output = speed_kp*error_speed + speed_kd*(error_speed - speed_previous_error);
	//speed_previous_error = error_speed;
	//
	//return output;
//}

/*PI current controller*/
void pi_controller(float *out, float error, float Kp, float Ki)
{	
	out[0] = out[1] + Kp*((error-out[2]) + (Ki*(error+out[2]))/Fs/2);
		
	out[1] = out[0];
	out[2] = error;
}

float feedforward(float *out, float kt, float motor_speed)
{
	return (out[0] + kt*motor_speed);
}

PIDvariable pid_controller(PIDvariable out, float x_d, float x, PIDcontrol K)
{
	//system_interrupt_enter_critical_section();
	out.e_k[0] = x_d-x;
	
	out.y_k[0]= out.y_k[1]+ K.Kp*((out.e_k[0]-out.e_k[1])+ (K.Ki*(out.e_k[0]+out.e_k[1]-out.d_y[0]-out.d_y[1]))/Fs/2);
	
	out.y_k[1] = out.y_k[0];
	out.e_k[1] =out.e_k[0];
	out.d_y[1] = out.d_y[0];
	
	if (out.y_k[0]>K.max){
		out.d_y[0] =  out.y_k[0]-K.max;
		out.y_k[0] = K.max;
	}
	else if (out.y_k[0]<K.min){
		out.d_y[0] =  out.y_k[0]-K.min;
		out.y_k[0] = K.min;
	}
	else{
		out.d_y[0] = 0;
		
	}
	
	
	
	return out;
	//system_interrupt_leave_critical_section();
}


