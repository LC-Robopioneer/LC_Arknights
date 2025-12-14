#include "tsk_config_and_callback.h"

Class_Motor_M3508 Motor_CAN1_ID1;

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

// int cnt = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // cnt++;
    if (htim->Instance == TIM1)
    {
        CAN_Send_Data(&hcan1, 0x200, CAN1_0x200_Tx_Data, 8);
    }
}

void Task_Init()
{
    BSP_Init(BSP_DC24_LU_ON | BSP_DC24_LD_ON | BSP_DC24_RU_ON | BSP_DC24_RD_ON);

    CAN_Init(&hcan1, CAN1_Callback);

    Motor_CAN1_ID1.Init(1, &hcan1);

    Motor_CAN1_ID1.Speed_PID.Init(2550.0f, 0.0f, 1.5f,0.0f, 0.01f, 16384.0f, 16384.0f, 0.0f);
    Motor_CAN1_ID1.Angle_PID.Init(0.45f, 0.0f, 0.0f,0.0f, 0.01f, 5000.0f, 450.0f, 0.0f);

    HAL_TIM_Base_Start_IT(&htim1);
}

float target_angle = 150.0f;
float target_omega = 20.0f;
uint8_t mode = 0; // 0:角度模式 1:速度模式

void Task_Loop()
{
    if (mode == 0)
    {
        Motor_CAN1_ID1.Set_Angle(target_angle);
    }
    else if (mode == 1)
    {
        Motor_CAN1_ID1.Set_Omega(target_omega);
    }
}