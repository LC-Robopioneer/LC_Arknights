/**
 * @file dvc_air_pump.h
 * @author Gcy
 * @brief 通过控制24V电源实现气泵控制
 * @date 2026-3-14 
 *
 *
 */
#ifndef __DVC_AIR_PUMP_H
#define __DVC_AIR_PUMP_H

#include "main.h"

#include "drv_bsp.h"
#include "drv_can.h"

 extern uint8_t g_Power_Channel_LU_Status;
 extern uint8_t g_Power_Channel_LD_Status;
 extern uint8_t g_Power_Channel_RU_Status;
 extern uint8_t g_Power_Channel_RD_Status;

 #define Power_COMMAND_ID 0x100

 /* 0xab a:通道号 b:开关状态 */
#define Power_CHANNEL_LU_ON 0x11  
#define Power_CHANNEL_LD_ON 0x21  
#define Power_CHANNEL_RU_ON 0x31  
#define Power_CHANNEL_RD_ON 0x41  

#define Power_CHANNEL_LU_OFF 0x10
#define Power_CHANNEL_LD_OFF 0x20
#define Power_CHANNEL_RU_OFF 0x30
#define Power_CHANNEL_RD_OFF 0x40

 extern void Power_Control_Callback(struct Struct_CAN_Rx_Buffer *Rx_Buffer);

 #endif

