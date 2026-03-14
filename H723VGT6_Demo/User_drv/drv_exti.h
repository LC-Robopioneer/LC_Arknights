/**
 * @file drv_uart.h
 * @author Lucy (2478427315@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-10-04
 *
 * @copyright RoboPioneer (c) 2024
 *
 */

#ifndef DRV_EXTI_H
#define DRV_EXTI_H

/* Includes ------------------------------------------------------------------*/

#include "stm32h7xx_hal.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief EXTI中断回调函数
 *
 */
typedef void (*EXTI_Call_Back)();

/**
 * @brief EXTIͨ�Ŵ����ṹ��
 */
struct Struct_EXTI_Manage_Object
{
    EXTI_HandleTypeDef *EXTI_Handler;
    EXTI_Call_Back Callback_Function;
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

void EXTI_Init(EXTI_HandleTypeDef *huart, EXTI_Call_Back Callback_Function, uint16_t Rx_Buffer_Length);

void EXTI_Reinit(EXTI_HandleTypeDef *huart);

uint8_t EXTI_Send_Data(EXTI_HandleTypeDef *huart, uint8_t *Data, uint16_t Length);
char EXTI_Send_Char_Data(EXTI_HandleTypeDef *huart, char *Data, uint16_t Length);

void TIM_1ms_EXTI_PeriodElapsedCallback();

#endif

/************************ COPYRIGHT(C) ROBOPIONEER **************************/
