#include "drv_tim.h"

Struct_TIM_Manage_Object TIM1_Manage_Object;
Struct_TIM_Manage_Object TIM6_Manage_Object;

bool g_is_init_finished=false;

void TIM_Init(TIM_HandleTypeDef *htim, TIM_Call_Back Callback_Function)
{
    if (htim->Instance == TIM1)
    {
        TIM1_Manage_Object.TIM_Handler = htim;
        TIM1_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (htim->Instance == TIM6)
    {
        TIM6_Manage_Object.TIM_Handler = htim;
        TIM6_Manage_Object.Callback_Function = Callback_Function;
    }
}

/*  TIM定时器中断回调函数选择 */
void HAl_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        if (TIM1_Manage_Object.Callback_Function != NULL)
        {
            TIM1_Manage_Object.Callback_Function();
        }
    }
    else if (htim->Instance == TIM6)
    {
        if (TIM6_Manage_Object.Callback_Function != NULL)
        {
            TIM6_Manage_Object.Callback_Function();
        }
    }
}

