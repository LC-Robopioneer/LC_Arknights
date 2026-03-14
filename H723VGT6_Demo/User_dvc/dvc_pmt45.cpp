/**
 * @file dvc_pmt45.cpp
 * @author Lucy
 * @brief PMT45光电门
 * @version 0.1
 * @date 2025-03-18
 *
 * @copyright ROBOPIONEER (c) 2025
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_pmt45.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief PMT45初始化
 *
 * @param __GPIO_Pin 光电门对应中断引脚 
 */
void Class_PMT45::Init(GPIO_TypeDef *__GPIOx, uint16_t __GPIO_Pin)
{
    GPIOx = __GPIOx;
    GPIO_Pin = __GPIO_Pin;
}

/**
 * @brief 中断回调处理函数
 * 这是一个模板, 使用时请根据不同处理情况在不同文件内重新定义
 *
 */
void Class_PMT45::PMT45_EXTI_Callback()
{
    // 编写自己需要的
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
