#ifndef __C_CAN_H
#define __C_CAN_H

#include "main.h"
#include "task.h"
#include "can.h"
#include "gpio.h"

//滤波器配置相关宏定义
/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"

/* Exported macros -----------------------------------------------------------*/

// 滤波器编号
#define CAN_FILTER(x) ((x) << 3)

// 接收队列
#define CAN_FIFO_0 (0 << 2)
#define CAN_FIFO_1 (1 << 2)

//标准帧或扩展帧
#define CAN_STDID (0 << 1)
#define CAN_EXTID (1 << 1)

// 数据帧或遥控帧
#define CAN_DATA_TYPE (0 << 0)
#define CAN_REMOTE_TYPE (1 << 0)


//接收信息结构体
typedef struct
{
    CAN_RxHeaderTypeDef Header;
	uint8_t Data[8];	
}CAN_Rx_Buffer;

typedef void (*CALL_BACK) (CAN_Rx_Buffer * can_rx_buffer);

typedef struct
{
	CAN_HandleTypeDef *CAN_Handler;
	CALL_BACK Callback_Function;
	
}CAN_Object_Manage;


//函数声明
void CAN_Init(CAN_HandleTypeDef *hcan,uint8_t Object_Para,uint32_t ID,uint32_t Mask_ID,CALL_BACK Callback_Function);
//void CAN_Filter_Config(CAN_HandleTypeDef *hcan);
void CAN_Filter_Config(CAN_HandleTypeDef *hcan,uint8_t Object_Param,uint32_t ID,uint32_t Mask_ID);
HAL_StatusTypeDef CAN_Send_Data(CAN_HandleTypeDef *hcan,uint8_t motor_position,int16_t current);//返回值为发送状态结构体

#endif




