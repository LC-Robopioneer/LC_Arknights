#ifndef __DRV_PID_H
#define __DRV_PID_H

#include "stm32f4xx_hal.h"
#include "can.h"
#include "gpio.h"
#include "Delay.h"
#include "tim.h"
//遇到了头文件重复包含导致的变量无法识别问题：pid->can->motor->pid
										   
typedef struct 
{
	float KP;
	float KI;
	float KD;
	float dt;
	
	float pre_error;
	float errorint;
	float out_max;
	float out_min;
}PID;

extern PID pid;

void PID_Init(PID * pid,float KP,float KI,float KD,float out_max,float out_min);
float PID_Caculate(PID * pid,float current,float target);

#endif






