#include "drv_can.h"
#include "tsk_callback.h"
#include "drv_serial.h"
#include "can.h"
#include "usart.h"
#include "dvc_motor.h"
#include "dma.h"

uint8_t pid_motor_flag = 0;

// CAN接收回调函数
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan->Instance == CAN1)
    {
        // 将CAN报文存入电机句柄
        Motor_GetParam(&hmotor1);

        // 串口回显
        UART2_printf("pidato:%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", 
            hmotor1.pid->kp, hmotor1.pid->ki, hmotor1.pid->kd, 
            hmotor1.pid->actual, hmotor1.pid->target, hmotor1.pid->output);
            
        can_rx_flag = 1;
    }
}

// UART接收完成回调函数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    // 解析串口PID信息, 并赋值给hmotor1
    Serial_RxEventCallback(huart, Size);
}

// 定时器周期中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // TIM6提供1ms中断
    if(htim->Instance == TIM6)
    {
        static uint8_t pid_motor_count = 0;

        if(++pid_motor_count >= PID_MOTOR_PERIOAD)
        {
            pid_motor_count = 0;
            
            pid_motor_flag = 1;
        }
    }
}
