#include "drv_serial.h"

uint8_t serial_tx_packet[SERIAL_PACKET_SIZE];
uint8_t serial_rx_packet[SERIAL_PACKET_SIZE];
uint8_t serial_rx_flag = 0;



/**
 * @brief 串口DMA接收初始化
 */
void Serial_RxDMA_Init(UART_HandleTypeDef *huart)
{
	HAL_UARTEx_ReceiveToIdle_DMA(huart, serial_rx_packet, SERIAL_PACKET_SIZE);
	__HAL_DMA_DISABLE_IT(huart->hdmarx, DMA_IT_HT);    
}

/**
 * @brief 串口发送字符串
 * @return 成功为HAL_OK=0
 */
HAL_StatusTypeDef Serial_SendString(UART_HandleTypeDef *huart, char *str)
{
	uint16_t size = 0;
	char *p = str;
	while(*p++ != '\0') size++;	// 计算字符串长度
	
    return HAL_UART_Transmit_DMA(huart, (uint8_t *)str, size);
}

/**
 * @brief 串口发送serial_tx_packet数据包
 */
HAL_StatusTypeDef Serial_SendPacket(void)
{
	return HAL_UART_Transmit_DMA(&huart2, serial_tx_packet, SERIAL_PACKET_SIZE);
}

/**
 * @brief 用于串口的printf函数
 * @param huart 串口句柄
 * @param format 同printf
 * @param ... 同printf
 * @warning 不得超过100字符
 */
HAL_StatusTypeDef Serial_Printf(UART_HandleTypeDef *huart, char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	return Serial_SendString(huart, String);		//串口发送字符数组（字符串）
}

/**
 * @brief 串口DMA printf函数
 * @param huart 串口句柄
 * @param format 同printf
 * @param ... 同printf
 * @warning 不得超过100字符
 */
HAL_StatusTypeDef Serial_Printf_DMA(UART_HandleTypeDef *huart, char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	return HAL_UART_Transmit_DMA(huart, (uint8_t *)String, strlen(String));		//串口发送字符数组（字符串）
}

/**
 * @brief UART2 printf函数
 * @param format 同printf
 * @param ... 同printf
 * @warning 不得超过100字符
 */
HAL_StatusTypeDef UART2_printf(char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	return Serial_Printf_DMA(&huart2, String, strlen(String));		//串口发送字符数组（字符串）
}


/**
 * @brief 串口接收中断回调函数
 * @param huart 串口句柄
 */
void Serial_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance == USART2)
	{
        serial_rx_flag = 1;
		serial_rx_packet[Size] = '\0';		// 添加结尾空字符表示字符串结束

		// 重新开启DMA接收
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, serial_rx_packet, SERIAL_PACKET_SIZE);
        __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);		// 关闭传输一半中断
	}
}

