#ifndef __C_CAN_H
#define __C_CAN_H

#include "main.h"
#include "main0.h"
#include "can.h"
#include "gpio.h"


//函数声明
void CAN_Init(CAN_HandleTypeDef *hcan);
void CAN_Filter_Config(CAN_HandleTypeDef *hcan);
HAL_StatusTypeDef CAN_Send_Data(CAN_HandleTypeDef *hcan,uint8_t motor_position,int16_t current);//返回值为发送状态结构体

#endif




