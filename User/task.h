#ifndef __TASK_H
#define __TASK_H

#include "main.h"
#include "can.h"
#include "gpio.h"
#include "M3508_drv.h"
#include "C_Can.h"
#include "stm32f4xx_hal.h"
#include "Delay.h"


//宏定义
//电机
#define MOTOR_CURRENT_MAX 16384
#define MOTOR_CURRENT_MIN (-16384)

//CAN
#define CAN_ID 0X200U  //can send data
#define CAN_BACK_ID 0X201U
#define CAN_CONTROL_PERIOD_MS 10U

//电机ID
typedef enum
{
	MOTOR_ID_1=0x201,
	MOTOR_ID_2=0x202,
	MOTOR_ID_3=0x203,
	MOTOR_ID_4=0x204
}Motor_ID;
	
void Task_Loop(void);

static void delay(unsigned long uldata)
{
	unsigned int i  =  0;
	unsigned int j  =  0;
	unsigned int k  =  0;
	for (i=0;i<5;i++)
	{
		for (j=0;j<uldata;j++)
		{
			k = 200;
			while(k--);
		}
	}
}
	
#endif




