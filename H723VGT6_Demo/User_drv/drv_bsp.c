/**
 * @file drv_bsp.c
 * @author Lucy (2478427315@qq.com)
 * @brief 达妙H723的板级支持包
 * @version 0.1
 * @date 2024-10-06	24-25赛季定稿
 *
 * @copyright RoboPioneer (c) 2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "drv_bsp.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 初始化全部板级支持包引脚
 *
 * @param Status 各个状态的按位或，具体如何使用参考宏定义
 */
void BSP_Init(uint32_t Status, float IMU_Heater_Rate, float Buzzer_Rate)
{
    BSP_Set_DC24_L(static_cast<Enum_BSP_DC24_Status>((Status & BSP_DC24_L_ON) == 0 ? BSP_DC24_Status_DISABLED : BSP_DC24_Status_ENABLED));
    BSP_Set_DC24_R(static_cast<Enum_BSP_DC24_Status>((Status & BSP_DC24_R_ON) == 0 ? BSP_DC24_Status_DISABLED : BSP_DC24_Status_ENABLED));
		BSP_Set_DC5(static_cast<Enum_BSP_DC5_Status>((Status & BSP_DC5_ON) == 0 ? BSP_DC5_Status_DISABLED : BSP_DC5_Status_ENABLED));
		
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2);

    BSP_Set_PWM_IMU_Heater(IMU_Heater_Rate);
    BSP_Set_PWM_Buzzer(Buzzer_Rate);
}

/**
 * @brief 获取左上角DC24
 *
 * @return Enum_BSP_DC24_Status 状态
 */
Enum_BSP_DC24_Status BSP_Get_DC24_L()
{
    return (static_cast<Enum_BSP_DC24_Status>(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)));
}

/**
 * @brief 获取DC5
 *
 * @return Enum_BSP_DC5_Status 状态
 */
Enum_BSP_DC5_Status BSP_Get_DC5()
{
    return (static_cast<Enum_BSP_DC5_Status>(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15)));
}

/**
 * @brief 获取右上角DC24
 *
 * @return Enum_BSP_DC24_Status 状态
 */
Enum_BSP_DC24_Status BSP_Get_DC24_R()
{
    return (static_cast<Enum_BSP_DC24_Status>(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14)));
}

/**
 * @brief 获取按键
 *
 * @return Enum_BSP_Key_Status 状态
 */
Enum_BSP_Key_Status BSP_Get_Key()
{
    static GPIO_PinState pre_key_status;
    GPIO_PinState key_status;
    Enum_BSP_Key_Status return_value;

    key_status = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);

    switch (pre_key_status)
    {
    case (GPIO_PIN_RESET):
    {
        switch (key_status)
        {
        case (GPIO_PIN_RESET):
        {
            pre_key_status = key_status;
            return_value = BSP_Key_Status_FREE;

            break;
        }
        case (GPIO_PIN_SET):
        {
            pre_key_status = key_status;
            return_value = BSP_Key_Status_TRIG_FREE_PRESSED;

            break;
        }
        }

        break;
    }
    case (GPIO_PIN_SET):
    {
        switch (key_status)
        {
        case (GPIO_PIN_RESET):
        {
            pre_key_status = key_status;
            return_value = BSP_Key_Status_TRIG_PRESSED_FREE;

            break;
        }
        case (GPIO_PIN_SET):
        {
            pre_key_status = key_status;
            return_value = BSP_Key_Status_PRESSED;

            break;
        }
        }

        break;
    }
    }

    return (return_value);
}

/**
 * @brief 设定左上角DC24
 *
 * @param Status 状态
 */
void BSP_Set_DC24_L(Enum_BSP_DC24_Status Status)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定右上角DC24
 *
 * @param Status 状态
 */
void BSP_Set_DC24_R(Enum_BSP_DC24_Status Status)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定DC5
 *
 * @param Status 状态
 */
void BSP_Set_DC5(Enum_BSP_DC5_Status Status)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, static_cast<GPIO_PinState>(Status));
}

/**
 * @brief 设定IMU加热电阻
 *
 * @param Rate IMU加热电阻温度占空比
 */
void BSP_Set_PWM_IMU_Heater(float Rate)
{
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, Rate * 65535);
}

/**
 * @brief 设定蜂鸣器
 *
 * @param Rate 蜂鸣器响度占空比
 */
void BSP_Set_PWM_Buzzer(float Rate)
{
    __HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_2, Rate * 1000);
}

/************************ COPYRIGHT(C) ROBOPIONEER **************************/
