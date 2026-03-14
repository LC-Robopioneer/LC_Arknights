/**
 * @file dvc_air_pump.cpp
 * @author Gcy
 * @brief 通过控制24V电源实现气泵控制
 * @date 2026-3-14 
 *
 *
 */
#include "dvc_air_pump.h"

uint8_t command_see=0;
uint32_t StdId_see=0;

uint8_t g_Power_Channel_LU_Status=0;
uint8_t g_Power_Channel_LD_Status=0;
uint8_t g_Power_Channel_RU_Status=0;
uint8_t g_Power_Channel_RD_Status=0;

/**
 * @brief CAN接收回调函数，用于处理电源控制命令
 * @param Rx_Buffer CAN接收的信息结构体
 * @note LU LD RU RD排序:靠芯片一侧的短边朝向使用者
 */

void Power_Control_Callback(struct Struct_CAN_Rx_Buffer *Rx_Buffer)
{
    StdId_see=Rx_Buffer->Header.StdId;
    
    /* �?查是否是电源控制消息 */
    if (Rx_Buffer->Header.StdId == Power_COMMAND_ID)
    {
        /* 获取命令 */ 
        uint8_t command = Rx_Buffer->Data[0];
        command_see=command;
        
        /* 根据命令控制相应通道 */
        switch (command)
        {
        case Power_CHANNEL_LU_ON:
            g_Power_Channel_LU_Status=1;
  
            break;
        case Power_CHANNEL_LU_OFF:
            g_Power_Channel_LU_Status=0;
            break;
        case Power_CHANNEL_LD_ON:
            g_Power_Channel_LD_Status=1;
            break;

        case Power_CHANNEL_LD_OFF:
             g_Power_Channel_LD_Status=0;
            break;

        case Power_CHANNEL_RU_ON:
            g_Power_Channel_RU_Status=1;
            break;

        case Power_CHANNEL_RU_OFF:
            g_Power_Channel_RU_Status=0;
            break;

        case Power_CHANNEL_RD_ON:
            g_Power_Channel_RD_Status=1;
            break;

        case Power_CHANNEL_RD_OFF:
            g_Power_Channel_RD_Status=0;
            break;
            
        default:
            break;
        }
    }
}

