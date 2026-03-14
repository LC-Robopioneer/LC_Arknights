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

#ifndef DRV_UART_H
#define DRV_UART_H

/* Includes ------------------------------------------------------------------*/

#include "stm32h7xx_hal.h"
#include "string.h"

/* Exported macros -----------------------------------------------------------*/

// 缓冲区字节长度
#define UART_BUFFER_SIZE 256

/* Exported types ------------------------------------------------------------*/

/**
 * @brief UART通信接收回调函数数据类型
 *
 */
typedef void (*UART_Call_Back)(uint8_t *Buffer, uint16_t Length);

/**
 * @brief UART通信处理结构体
 */
struct Struct_UART_Manage_Object
{
    UART_HandleTypeDef *UART_Handler;
    uint8_t Tx_Buffer[UART_BUFFER_SIZE];
    uint8_t Rx_Buffer[UART_BUFFER_SIZE];
    uint16_t Rx_Buffer_Length;
    UART_Call_Back Callback_Function;
};

/* Exported variables --------------------------------------------------------*/

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart9;
extern UART_HandleTypeDef huart10;

extern Struct_UART_Manage_Object UART1_Manage_Object;
extern Struct_UART_Manage_Object UART2_Manage_Object;
extern Struct_UART_Manage_Object UART3_Manage_Object;
extern Struct_UART_Manage_Object UART4_Manage_Object;
extern Struct_UART_Manage_Object UART5_Manage_Object;
extern Struct_UART_Manage_Object UART6_Manage_Object;
extern Struct_UART_Manage_Object UART7_Manage_Object;
extern Struct_UART_Manage_Object UART8_Manage_Object;
extern Struct_UART_Manage_Object UART9_Manage_Object;
extern Struct_UART_Manage_Object UART10_Manage_Object;

/* Exported function declarations --------------------------------------------*/

void UART_Init(UART_HandleTypeDef *huart, UART_Call_Back Callback_Function, uint16_t Rx_Buffer_Length);

void UART_Reinit(UART_HandleTypeDef *huart);

uint8_t UART_Send_Data(UART_HandleTypeDef *huart, uint8_t *Data, uint16_t Length);
char UART_Send_Char_Data(UART_HandleTypeDef *huart, char *Data, uint16_t Length);

void TIM_1ms_UART_PeriodElapsedCallback();

#endif

/************************ COPYRIGHT(C) ROBOPIONEER **************************/
