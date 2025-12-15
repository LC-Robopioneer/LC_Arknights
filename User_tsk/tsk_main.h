#ifndef __TSK_MAIN_H
#define __TSK_MAIN_H

#include "stm32f4xx_hal.h"

#include "drv_can.h"
#include "drv_bsp.h"

#include "dvc_m3508.h"

#include "alg_pid.h"
#include "alg_serial.h"

#include "tsk_callback.h"
#include "tsk_delay.h"


////宏定义
////电机
//#define MOTOR_CURRENT_MAX 16384
//#define MOTOR_CURRENT_MIN (-16384)

////CAN
//#define CAN_ID 0X200U  //can send data
//#define CAN_BACK_ID 0X201U
//#define CAN_CONTROL_PERIOD_MS 10U

//电机ID
//typedef enum
//{
//	MOTOR_ID_1=0x201,
//	MOTOR_ID_2=0x202,
//	MOTOR_ID_3=0x203,
//	MOTOR_ID_4=0x204
//}Motor_ID;
	
//extern M3508_Motor M3508_Motor_1;
//MOTOR_POSITION_ID   Motor_Posion=Motor1;

void Task_Loop(void);

	
#endif




