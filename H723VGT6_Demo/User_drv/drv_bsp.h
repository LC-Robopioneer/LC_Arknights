/**
 * @file drv_bsp.h
 * @author Lucy (2478427315@qq.com)
 * @brief 达妙H723的板级支持包
 * @version 0.1
 * @date 2024-10-06	24-25赛季定稿
 *
 * @copyright RoboPioneer (c) 2024
 *
 */

#ifndef DRV_BSP_H
#define DRV_BSP_H

/* Includes ------------------------------------------------------------------*/

#include "stm32h7xx_hal.h"

/* Exported macros -----------------------------------------------------------*/

// 初始化DC24被按位或的参数
#define BSP_DC24_L_ON (1 << 0)
#define BSP_DC24_R_ON (1 << 1)
#define BSP_DC5_ON (1 << 2)
#define BSP_DC24_L_OFF (0 << 0)
#define BSP_DC24_R_OFF (0 << 1)
#define BSP_DC5_OFF (0 << 2)

/**
 * @brief 板上DC24工作状态
 *
 */
enum Enum_BSP_DC24_Status
{
    BSP_DC24_Status_DISABLED = 0,
    BSP_DC24_Status_ENABLED,
};

/**
 * @brief 板上DC5工作状态
 *
 */
enum Enum_BSP_DC5_Status
{
    BSP_DC5_Status_DISABLED = 0,
    BSP_DC5_Status_ENABLED,
};

/**
 * @brief 按键状态
 *
 */
enum Enum_BSP_Key_Status
{
    BSP_Key_Status_FREE = 0,
    BSP_Key_Status_TRIG_FREE_PRESSED,
    BSP_Key_Status_TRIG_PRESSED_FREE,
    BSP_Key_Status_PRESSED,
};

/* Exported variables --------------------------------------------------------*/

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim12;

/* Exported function declarations --------------------------------------------*/

void BSP_Init(uint32_t Status, float IMU_Heater_Rate = 0, float Buzzer_Rate = 0);

Enum_BSP_DC24_Status BSP_Get_DC24_L();

Enum_BSP_DC24_Status BSP_Get_DC24_R();

Enum_BSP_DC5_Status BSP_Get_DC5();

Enum_BSP_Key_Status BSP_Get_Key();

void BSP_Set_DC24_L(Enum_BSP_DC24_Status Status);

void BSP_Set_DC24_R(Enum_BSP_DC24_Status Status);

void BSP_Set_DC5(Enum_BSP_DC5_Status Status);

void BSP_Set_PWM_IMU_Heater(float Rate);

void BSP_Set_PWM_Buzzer(float Rate);

#endif

/************************ COPYRIGHT(C) ROBOPIONEER **************************/
