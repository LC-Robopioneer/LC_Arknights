#include "tsk_config_and_callback.h"

Class_Motor_M3508 Motor_CAN1_ID1(1, &hcan1);

// CAN1回调函数
void CAN1_Callback(Struct_CAN_Rx_Buffer *rx_buffer)
{
    // 更新CAN1电机的反馈数据
    if (rx_buffer == nullptr)
        return;
    uint32_t can_id = rx_buffer->Header.StdId;

    switch (can_id)
    {
    case 0x201:
        Motor_CAN1_ID1.Update_From_Feedback(rx_buffer);
        break;
    case 0x202:
        /* code */
        break;
    case 0x203:
        /* code */
        break;
    case 0x204:
        /* code */
        break;
    case 0x205:
        /* code */
        break;
    case 0x206:
        /* code */
        break;
    case 0x207:
        /* code */
        break;
    case 0x208:
        /* code */
        break;
    default:
        break;
    }
}

// CAN2回调函数
void CAN2_Callback(Struct_CAN_Rx_Buffer *rx_buffer)
{
}

void Task_Init()
{
    BSP_Init(BSP_DC24_LU_ON | BSP_DC24_LD_ON | BSP_DC24_RU_ON | BSP_DC24_RD_ON);

    CAN_Init(&hcan1, CAN1_Callback);
}

void Task_Loop()
{
    static uint32_t last_time_10ms = 0;
    static uint32_t last_time_100ms = 0;
    Motor_CAN1_ID1.Set_Current(3000);
    if (HAL_GetTick() - last_time_10ms >= 10)
    {
        last_time_10ms = HAL_GetTick();
        CAN_Send_Data(&hcan1, 0x200, CAN1_0x200_Tx_Data, 8);
    }
    if(HAL_GetTick() - last_time_100ms >= 100 && uart_tx_complete)
    {
        last_time_100ms = HAL_GetTick();
        char buffer[128];
        sprintf(buffer,"%d,%d,%d,%d\n",Motor_CAN1_ID1.Get_Angle(),Motor_CAN1_ID1.Get_Speed(),Motor_CAN1_ID1.Get_Actual_Current(),Motor_CAN1_ID1.Get_Temperature());
        HAL_UART_Transmit_DMA(&huart1, (uint8_t*)buffer, strlen(buffer));
        uart_tx_complete = 0;
    }
}