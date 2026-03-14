/**
 * @file drv_tim.cpp
 * @author Lucy (2478427315@qq.com)
 * @brief 仿照SCUT-Robotlab改写的TIM定时器初始化与配置流程
 * @version 0.1
 * @date 2024-10-19 0.1 24-25赛季定稿
 *
 * @copyright RoboPionner
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "drv_tim.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Struct_TIM_Manage_Object TIM1_Manage_Object;
Struct_TIM_Manage_Object TIM2_Manage_Object;
Struct_TIM_Manage_Object TIM3_Manage_Object;
Struct_TIM_Manage_Object TIM4_Manage_Object;
Struct_TIM_Manage_Object TIM5_Manage_Object;
Struct_TIM_Manage_Object TIM6_Manage_Object;
Struct_TIM_Manage_Object TIM7_Manage_Object;
Struct_TIM_Manage_Object TIM8_Manage_Object;
Struct_TIM_Manage_Object TIM12_Manage_Object;
Struct_TIM_Manage_Object TIM13_Manage_Object;
Struct_TIM_Manage_Object TIM14_Manage_Object;
Struct_TIM_Manage_Object TIM15_Manage_Object;
Struct_TIM_Manage_Object TIM16_Manage_Object;
Struct_TIM_Manage_Object TIM17_Manage_Object;
Struct_TIM_Manage_Object TIM23_Manage_Object;
Struct_TIM_Manage_Object TIM24_Manage_Object;


/* Private function declarations ---------------------------------------------*/

/* function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化TIM定时器
 *
 * @param htim 定时器编号
 * @param Callback_Function 处理回调函数
 */
void TIM_Init(TIM_HandleTypeDef *htim, TIM_Call_Back Callback_Function)
{
    if (htim->Instance == TIM1)
    {
        TIM1_Manage_Object.TIM_Handler = htim;
        TIM1_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (htim->Instance == TIM2)
    {
        TIM2_Manage_Object.TIM_Handler = htim;
        TIM2_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (htim->Instance == TIM3)
    {
        TIM3_Manage_Object.TIM_Handler = htim;
        TIM3_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (htim->Instance == TIM4)
    {
        TIM4_Manage_Object.TIM_Handler = htim;
        TIM4_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (htim->Instance == TIM5)
    {
        TIM5_Manage_Object.TIM_Handler = htim;
        TIM5_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (htim->Instance == TIM6)
    {
        TIM6_Manage_Object.TIM_Handler = htim;
        TIM6_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (htim->Instance == TIM7)
    {
        TIM7_Manage_Object.TIM_Handler = htim;
        TIM7_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (htim->Instance == TIM8)
    {
        TIM8_Manage_Object.TIM_Handler = htim;
        TIM8_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (htim->Instance == TIM12)
    {
        TIM12_Manage_Object.TIM_Handler = htim;
        TIM12_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (htim->Instance == TIM13)
    {
        TIM13_Manage_Object.TIM_Handler = htim;
        TIM13_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (htim->Instance == TIM14)
    {
        TIM14_Manage_Object.TIM_Handler = htim;
        TIM14_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (htim->Instance == TIM15)
    {
        TIM15_Manage_Object.TIM_Handler = htim;
        TIM15_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (htim->Instance == TIM16)
    {
        TIM16_Manage_Object.TIM_Handler = htim;
        TIM16_Manage_Object.Callback_Function = Callback_Function;
    }
    else if (htim->Instance == TIM17)
    {
        TIM17_Manage_Object.TIM_Handler = htim;
        TIM17_Manage_Object.Callback_Function = Callback_Function;
    }
		else if (htim->Instance == TIM23)
    {
        TIM23_Manage_Object.TIM_Handler = htim;
        TIM23_Manage_Object.Callback_Function = Callback_Function;
    }
		else if (htim->Instance == TIM24)
    {
        TIM24_Manage_Object.TIM_Handler = htim;
        TIM24_Manage_Object.Callback_Function = Callback_Function;
    }
}

/**
 * @brief HAL库TIM定时器中断
 *
 * @param htim TIM编号
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // 判断程序初始化完成
    if (init_finished == false)
    {
        return;
    }

    // 选择回调函数
    if (htim->Instance == TIM1)
    {
        if(TIM1_Manage_Object.Callback_Function != nullptr)
        {
            TIM1_Manage_Object.Callback_Function();
        }
    }
    else if (htim->Instance == TIM2)
    {
        if(TIM2_Manage_Object.Callback_Function != nullptr)
        {
            TIM2_Manage_Object.Callback_Function();
        }
    }
    else if (htim->Instance == TIM3)
    {
        if(TIM3_Manage_Object.Callback_Function != nullptr)
        {
            TIM3_Manage_Object.Callback_Function();
        }
    }
    else if (htim->Instance == TIM4)
    {
        if(TIM4_Manage_Object.Callback_Function != nullptr)
        {
            TIM4_Manage_Object.Callback_Function();
        }
    }
    else if (htim->Instance == TIM5)
    {
        if(TIM5_Manage_Object.Callback_Function != nullptr)
        {
            TIM5_Manage_Object.Callback_Function();
        }
    }
    else if (htim->Instance == TIM6)
    {
        if(TIM6_Manage_Object.Callback_Function != nullptr)
        {
            TIM6_Manage_Object.Callback_Function();
        }
    }
    else if (htim->Instance == TIM7)
    {
        if(TIM7_Manage_Object.Callback_Function != nullptr)
        {
            TIM7_Manage_Object.Callback_Function();
        }
    }
    else if (htim->Instance == TIM8)
    {
        if(TIM8_Manage_Object.Callback_Function != nullptr)
        {
            TIM8_Manage_Object.Callback_Function();
        }
    }
    else if (htim->Instance == TIM12)
    {
        if(TIM12_Manage_Object.Callback_Function != nullptr)
        {
            TIM12_Manage_Object.Callback_Function();
        }
    }
    else if (htim->Instance == TIM13)
    {
        if(TIM13_Manage_Object.Callback_Function != nullptr)
        {
            TIM13_Manage_Object.Callback_Function();
        }
    }
    else if (htim->Instance == TIM14)
    {
        if(TIM14_Manage_Object.Callback_Function != nullptr)
        {
            TIM14_Manage_Object.Callback_Function();
        }
    }
		else if (htim->Instance == TIM15)
    {
        if(TIM15_Manage_Object.Callback_Function != nullptr)
        {
            TIM15_Manage_Object.Callback_Function();
        }
    }
    else if (htim->Instance == TIM16)
    {
        if(TIM16_Manage_Object.Callback_Function != nullptr)
        {
            TIM16_Manage_Object.Callback_Function();
        }
    }
    else if (htim->Instance == TIM17)
    {
        if(TIM17_Manage_Object.Callback_Function != nullptr)
        {
            TIM17_Manage_Object.Callback_Function();
        }
    }
		else if (htim->Instance == TIM23)
    {
        if(TIM23_Manage_Object.Callback_Function != nullptr)
        {
            TIM23_Manage_Object.Callback_Function();
        }
    }
    else if (htim->Instance == TIM24)
    {
        if(TIM24_Manage_Object.Callback_Function != nullptr)
        {
            TIM24_Manage_Object.Callback_Function();
        }
    }
}

/************************ COPYRIGHT(C) ROBOPIONNER **************************/
