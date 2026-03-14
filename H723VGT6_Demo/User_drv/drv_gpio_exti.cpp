/**
 * @file drv_gpio_exti.c
 * @author Lucy (2478427315@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-15
 *
 * @copyright RoboPioneer (c) 2025
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "drv_gpio_exti.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Struct_EXTI_Manage_Object GPIO0_EXTI_Manage_Object = {0};
Struct_EXTI_Manage_Object GPIO1_EXTI_Manage_Object = {0};
Struct_EXTI_Manage_Object GPIO2_EXTI_Manage_Object = {0};
Struct_EXTI_Manage_Object GPIO3_EXTI_Manage_Object = {0};
Struct_EXTI_Manage_Object GPIO4_EXTI_Manage_Object = {0};
Struct_EXTI_Manage_Object GPIO5_EXTI_Manage_Object = {0};
Struct_EXTI_Manage_Object GPIO6_EXTI_Manage_Object = {0};
Struct_EXTI_Manage_Object GPIO7_EXTI_Manage_Object = {0};
Struct_EXTI_Manage_Object GPIO8_EXTI_Manage_Object = {0};
Struct_EXTI_Manage_Object GPIO9_EXTI_Manage_Object = {0};
Struct_EXTI_Manage_Object GPIO10_EXTI_Manage_Object = {0};
Struct_EXTI_Manage_Object GPIO11_EXTI_Manage_Object = {0};
Struct_EXTI_Manage_Object GPIO12_EXTI_Manage_Object = {0};
Struct_EXTI_Manage_Object GPIO13_EXTI_Manage_Object = {0};
Struct_EXTI_Manage_Object GPIO14_EXTI_Manage_Object = {0};
Struct_EXTI_Manage_Object GPIO15_EXTI_Manage_Object = {0};

/* Private function declarations ---------------------------------------------*/

/* function prototypes -------------------------------------------------------*/

/**
 * @brief GPIO_EXTI中断初始化
 *
 * @param GPIO_PIN
 * @param Callback_Function 中断回调函数
 */
void GPIO_EXTI_Init(uint16_t __GPIO_Pin, GPIO_EXTI_Call_Back __Callback_Function)
{
		if (__GPIO_Pin == GPIO_PIN_0)
    {
        GPIO0_EXTI_Manage_Object.GPIO_Pin = __GPIO_Pin;
        GPIO0_EXTI_Manage_Object.Callback_Function = __Callback_Function;
    }
    if (__GPIO_Pin == GPIO_PIN_1)
    {
        GPIO1_EXTI_Manage_Object.GPIO_Pin = __GPIO_Pin;
        GPIO1_EXTI_Manage_Object.Callback_Function = __Callback_Function;
    }
    else if (__GPIO_Pin == GPIO_PIN_2)
    {
        GPIO2_EXTI_Manage_Object.GPIO_Pin = __GPIO_Pin;
        GPIO2_EXTI_Manage_Object.Callback_Function = __Callback_Function;
    }
    else if (__GPIO_Pin == GPIO_PIN_3)
    {
        GPIO3_EXTI_Manage_Object.GPIO_Pin = __GPIO_Pin;
        GPIO3_EXTI_Manage_Object.Callback_Function = __Callback_Function;
    }
    else if (__GPIO_Pin == GPIO_PIN_4)
    {
        GPIO4_EXTI_Manage_Object.GPIO_Pin = __GPIO_Pin;
        GPIO4_EXTI_Manage_Object.Callback_Function = __Callback_Function;
    }
    else if (__GPIO_Pin == GPIO_PIN_5)
    {
        GPIO5_EXTI_Manage_Object.GPIO_Pin = __GPIO_Pin;
        GPIO5_EXTI_Manage_Object.Callback_Function = __Callback_Function;
    }
    else if (__GPIO_Pin == GPIO_PIN_6)
    {
        GPIO6_EXTI_Manage_Object.GPIO_Pin = __GPIO_Pin;
        GPIO6_EXTI_Manage_Object.Callback_Function = __Callback_Function;
    }
    else if (__GPIO_Pin == GPIO_PIN_7)
    {
        GPIO7_EXTI_Manage_Object.GPIO_Pin = __GPIO_Pin;
        GPIO7_EXTI_Manage_Object.Callback_Function = __Callback_Function;
    }
    else if (__GPIO_Pin == GPIO_PIN_8)
    {
        GPIO8_EXTI_Manage_Object.GPIO_Pin = __GPIO_Pin;
        GPIO8_EXTI_Manage_Object.Callback_Function = __Callback_Function;
    }
	else if (__GPIO_Pin == GPIO_PIN_9)
    {
        GPIO9_EXTI_Manage_Object.GPIO_Pin = __GPIO_Pin;
        GPIO9_EXTI_Manage_Object.Callback_Function = __Callback_Function;
    }
	else if (__GPIO_Pin == GPIO_PIN_10)
    {
        GPIO10_EXTI_Manage_Object.GPIO_Pin = __GPIO_Pin;
        GPIO10_EXTI_Manage_Object.Callback_Function = __Callback_Function;
    }
    else if (__GPIO_Pin == GPIO_PIN_11)
    {
        GPIO11_EXTI_Manage_Object.GPIO_Pin = __GPIO_Pin;
        GPIO11_EXTI_Manage_Object.Callback_Function = __Callback_Function;
    }
    else if (__GPIO_Pin == GPIO_PIN_12)
    {
        GPIO12_EXTI_Manage_Object.GPIO_Pin = __GPIO_Pin;
        GPIO12_EXTI_Manage_Object.Callback_Function = __Callback_Function;
    }
    else if (__GPIO_Pin == GPIO_PIN_13)
    {
        GPIO13_EXTI_Manage_Object.GPIO_Pin = __GPIO_Pin;
        GPIO13_EXTI_Manage_Object.Callback_Function = __Callback_Function;
    }
    else if (__GPIO_Pin == GPIO_PIN_14)
    {
        GPIO14_EXTI_Manage_Object.GPIO_Pin = __GPIO_Pin;
        GPIO14_EXTI_Manage_Object.Callback_Function = __Callback_Function;
    }
    else if (__GPIO_Pin == GPIO_PIN_15)
    {
        GPIO15_EXTI_Manage_Object.GPIO_Pin = __GPIO_Pin;
        GPIO15_EXTI_Manage_Object.Callback_Function = __Callback_Function;
    }
}

/**
 * @brief EXTI中断回调函数
 *
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		if (GPIO_Pin == GPIO_PIN_0)
    {
        GPIO0_EXTI_Manage_Object.Callback_Function();
    }
    else if (GPIO_Pin == GPIO_PIN_1)
    {
        GPIO1_EXTI_Manage_Object.Callback_Function();
    }
    else if (GPIO_Pin == GPIO_PIN_2)
    {
        GPIO2_EXTI_Manage_Object.Callback_Function();
    }
    else if (GPIO_Pin == GPIO_PIN_3)
    {
        GPIO3_EXTI_Manage_Object.Callback_Function();
    }
    else if (GPIO_Pin == GPIO_PIN_4)
    {
        GPIO4_EXTI_Manage_Object.Callback_Function();
    }
    else if (GPIO_Pin == GPIO_PIN_5)
    {
        GPIO5_EXTI_Manage_Object.Callback_Function();
    }
    else if (GPIO_Pin == GPIO_PIN_6)
    {
        GPIO6_EXTI_Manage_Object.Callback_Function();
    }
    else if (GPIO_Pin == GPIO_PIN_7)
    {
        GPIO7_EXTI_Manage_Object.Callback_Function();
    }
    else if (GPIO_Pin == GPIO_PIN_8)
    {
        GPIO8_EXTI_Manage_Object.Callback_Function();
    }
	else if (GPIO_Pin == GPIO_PIN_9)
    {
        GPIO9_EXTI_Manage_Object.Callback_Function();
    }
	else if (GPIO_Pin == GPIO_PIN_10)
    {
        GPIO10_EXTI_Manage_Object.Callback_Function();
    }
    else if (GPIO_Pin == GPIO_PIN_11)
    {
        GPIO11_EXTI_Manage_Object.Callback_Function();
    }
    else if (GPIO_Pin == GPIO_PIN_12)
    {
        GPIO12_EXTI_Manage_Object.Callback_Function();
    }
    else if (GPIO_Pin == GPIO_PIN_13)
    {
        GPIO13_EXTI_Manage_Object.Callback_Function();
    }
    else if (GPIO_Pin == GPIO_PIN_14)
    {
        GPIO14_EXTI_Manage_Object.Callback_Function();
    }
    else if (GPIO_Pin == GPIO_PIN_15)
    {
        GPIO15_EXTI_Manage_Object.Callback_Function();
    }
}

/************************ COPYRIGHT(C) ROBOPIONEER **************************/
