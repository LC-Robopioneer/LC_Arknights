#include "tsk_config_and_callback.h"
#include "stm32f4xx_hal.h"
#include "dvc_motor_dji.h"
#include "drv_tim.h"

// M3508电机类, PID参数以及目标值用debug调
Class_Motor_DJI_C620 M3508;

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
    M3508.Init(&hcan1, Motor_DJI_ID_0x201);
}

void Task_Loop(void)
{

}
