/**
 * @file drv_gpio_exti.h
 * @author Lucy (2478427315@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-15
 *
 * @copyright RoboPioneer (c) 2025
 *
 */

#ifndef DRV_GPIO_EXTI_H
#define DRV_GPIO_EXTI_H

/* Includes ------------------------------------------------------------------*/

#include "stm32h7xx_hal.h"
#include "string.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief   GPIO_EXTI中断回调函数定义
 *
 */
typedef void (*GPIO_EXTI_Call_Back)();

/**
 * @brief GPIO_EXTI中断管理对象定义
 */
struct Struct_EXTI_Manage_Object
{
    uint16_t GPIO_Pin;
    GPIO_EXTI_Call_Back Callback_Function;
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

void GPIO_EXTI_Init(uint16_t __GPIO_Pin, GPIO_EXTI_Call_Back Callback_Function);

#endif

/************************ COPYRIGHT(C) ROBOPIONEER **************************/
