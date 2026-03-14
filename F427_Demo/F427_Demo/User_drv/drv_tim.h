#ifndef DRV_TIM_H
#define DRV_TIM_H

#include "stm32f4xx_hal.h"

/* TIM6中断回调函数 */

typedef void (* TIM_Call_Back)(void);

/* TIM定时器处理结构体*/

typedef struct
{
    TIM_HandleTypeDef *TIM_Handler; // TIM句柄
    TIM_Call_Back Callback_Function; // 回调函数指针
} Struct_TIM_Manage_Object;

extern bool g_is_init_finished;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim6;

extern Struct_TIM_Manage_Object TIM1_Manage_Object;
extern Struct_TIM_Manage_Object TIM6_Manage_Object;

void TIM_Init(TIM_HandleTypeDef *htim, TIM_Call_Back Callback_Function);

#endif
