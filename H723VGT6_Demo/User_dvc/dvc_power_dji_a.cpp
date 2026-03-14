/**
 * @file dvc_power_dji_a.cpp
 * @author Gcy
 * @brief 通过控制大疆A板24V电源实现气泵控制
 * @date 2026-3-14 
 *
 *
 */
#include "dvc_power_dji_a.h"

uint8_t command_see;
uint8_t channel_see;
bool CAN_PowerController::turn_on_power(uint8_t powerChannel)
{
    uint8_t command;
    channel_see=powerChannel;
    switch (powerChannel)
    {
        case 1:
            command = Power_CHANNEL_LU_ON;
            break;
        case 2:
            command = Power_CHANNEL_LD_ON;
            break;
        case 3:
            command = Power_CHANNEL_RU_ON;
            break;
        case 4:
            command = Power_CHANNEL_RD_ON;
            break;
        default:
            return 0; // 无效的通道号
    }
    command_see=command;
    FDCAN_Send_Data(FDCAN1_Manage_Object.FDCAN_Handler, Power_COMMAND_ID, &command, FDCAN_ID_Standard, 2);
    return 1;
}
bool CAN_PowerController::turn_off_power(uint8_t powerChannel)
{
    uint8_t command;
    switch (powerChannel)
    { 
        case 1:
            command = Power_CHANNEL_LU_OFF;
            break;
        case 2:
            command = Power_CHANNEL_LD_OFF;
            break;
        case 3:
            command = Power_CHANNEL_RU_OFF;
            break;
        case 4:
            command = Power_CHANNEL_RD_OFF;
            break;
        default:
            return 0; // 无效的通道号
    }
    FDCAN_Send_Data(FDCAN3_Manage_Object.FDCAN_Handler, Power_COMMAND_ID, &command, FDCAN_ID_Standard, 2);
    return 1;
}
