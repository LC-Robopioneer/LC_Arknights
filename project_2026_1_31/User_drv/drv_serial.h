#ifndef __DRV_SERIAL_H
#define __DRV_SERIAL_H

#include "main.h"
#include "string.h"
#include "stdio.h"
#include "stdarg.h"
#include "usart.h"

#define SERIAL_PACKET_SIZE     100

extern uint8_t serial_tx_packet[SERIAL_PACKET_SIZE];
extern uint8_t serial_rx_packet[SERIAL_PACKET_SIZE];
extern uint8_t serial_rx_flag;


void Serial_RxDMA_Init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef Serial_SendPacket(void);
HAL_StatusTypeDef Serial_SendString(UART_HandleTypeDef *huart, char *str);
HAL_StatusTypeDef Serial_Printf(UART_HandleTypeDef *huart, char *format, ...);
HAL_StatusTypeDef Serial_Printf_DMA(UART_HandleTypeDef *huart, char *format, ...);
void Serial_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
HAL_StatusTypeDef UART2_printf(char *format, ...);

#endif
