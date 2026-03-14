/**
 * @file drv_tim.h
 * @author Lucy (2478427315@qq.com)
 * @brief 仿照SCUT-Robotlab改写的TIM定时器初始化与配置流程
 * @version 0.1
 * @date 2024-10-19 0.1 24-25赛季定稿
 *
 * @copyright RoboPionner
 *
 */

#ifndef DRV_TIM_H
#define DRV_TIM_H

/* Includes ------------------------------------------------------------------*/

#include "stm32h7xx_hal.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief TIM定时器回调函数数据类型
 *
 */
typedef void (*TIM_Call_Back)();

/**
 * @brief TIM定时器处理结构体
 *
 */
struct Struct_TIM_Manage_Object
{
    TIM_HandleTypeDef *TIM_Handler;
    TIM_Call_Back Callback_Function;
};

/* Exported variables --------------------------------------------------------*/

extern bool init_finished;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim12;

extern Struct_TIM_Manage_Object TIM1_Manage_Object;
extern Struct_TIM_Manage_Object TIM2_Manage_Object;
extern Struct_TIM_Manage_Object TIM3_Manage_Object;
extern Struct_TIM_Manage_Object TIM4_Manage_Object;
extern Struct_TIM_Manage_Object TIM5_Manage_Object;
extern Struct_TIM_Manage_Object TIM6_Manage_Object;
extern Struct_TIM_Manage_Object TIM7_Manage_Object;
extern Struct_TIM_Manage_Object TIM8_Manage_Object;
extern Struct_TIM_Manage_Object TIM12_Manage_Object;
extern Struct_TIM_Manage_Object TIM13_Manage_Object;
extern Struct_TIM_Manage_Object TIM14_Manage_Object;
extern Struct_TIM_Manage_Object TIM15_Manage_Object;
extern Struct_TIM_Manage_Object TIM16_Manage_Object;
extern Struct_TIM_Manage_Object TIM17_Manage_Object;
extern Struct_TIM_Manage_Object TIM23_Manage_Object;
extern Struct_TIM_Manage_Object TIM24_Manage_Object;

/* Exported function declarations --------------------------------------------*/

void TIM_Init(TIM_HandleTypeDef *htim, TIM_Call_Back Callback_Function);

#endif

/************************ COPYRIGHT(C) ROBOPIONNER **************************/
