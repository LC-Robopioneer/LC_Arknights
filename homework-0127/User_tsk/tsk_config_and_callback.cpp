#include "tsk_config_and_callback.h"
#include "stm32f4xx_hal.h"
#include "dvc_motor_dji.h"
#include "drv_tim.h"
#include "drv_can.h"

// M3508电机类, PID参数以及目标值用debug调
Class_Motor_DJI_C620 M3508;

/**
 * @brief Chassis_CAN1回调函数
 *
 * @param CAN_RxMessage CAN1收到的消息
 */
void Motor_CAN1_Callback(Struct_CAN_Rx_Buffer *CAN_RxMessage)
{
    switch (CAN_RxMessage->Header.StdId)
    {
        case (0x201):  
        {
            M3508.CAN_RxCpltCallback(CAN_RxMessage->Data);
        }
        break;
    }
}

void Task_1ms_Callback()
{
    M3508.TIM_Calculate_PeriodElapsedCallback();

    static uint16_t mod100 = 0;
    if(++mod100 >= 100)
    {
        mod100 = 0;
        M3508.TIM_100ms_Alive_PeriodElapsedCallback();
    }
}

void Task_Init(void)
{
    TIM_Init(&htim4, Task_1ms_Callback);
    CAN_Init(&hcan1, Motor_CAN1_Callback);
    M3508.Init(&hcan1, Motor_DJI_ID_0x201);
}

void Task_Loop(void)
{

}
