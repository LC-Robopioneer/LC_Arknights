#ifndef __DVC_M3508_H
#define __DVC_M3508_H

#include "main.h"
#include "can.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"
#include "alg_pid.h"
#include "drv_can.h"



typedef enum
{
	MOTOR_ID_1=0x201,
	MOTOR_ID_2=0x202,
	MOTOR_ID_3=0x203,
	MOTOR_ID_4=0x204
}Motor_ID;

typedef enum
{
	MOTOR_STOP=0,
	MOTOR_FORWARD,
	MOTOR_BACK,
	MOTOR_ERROR
}M3508_Motor_State;

typedef struct
{
	int16_t target_current;
	M3508_Motor_State Motor_State;
	
	//通信
	uint8_t is_online;
	uint32_t last_update_time;
	
	//安全保护
	int16_t emergency_stop;
	int16_t current_max;
	int16_t current_min;	
}M3508_Motor;

extern M3508_Motor M3508_Motor_1;

typedef enum
{
	Motor1=0,
	Motor2,
	Motor3,
	Motor4
}MOTOR_POSITION_ID;
//函数声明
void M3508_Init(M3508_Motor *Motor);
void M3508_Set_Current(CAN_HandleTypeDef *hcan,M3508_Motor *Motor,int16_t current,uint8_t motor_position);
//void M3508_Set_Current_PID(CAN_HandleTypeDef *hcan,M3508_Motor *Motor,PID* pid ,int16_t target_current,
//int16_t now_current,uint8_t motor_position);
void M3508_Stop(CAN_HandleTypeDef *hcan,M3508_Motor *Motor,uint8_t motor_position);
void M3508_Emergency_Stop(CAN_HandleTypeDef *hcan,M3508_Motor *Motor,uint8_t motor_position);
void M3508_Resume_From_Stop(CAN_HandleTypeDef *hcan,M3508_Motor *Motor,uint8_t motor_position);
void M3508_Update_State(CAN_HandleTypeDef *hcan,M3508_Motor *Motor,uint8_t motor_position);


#endif







