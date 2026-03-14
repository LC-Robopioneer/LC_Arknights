/**
 * @file dvc_ops9.cpp
 * @author 刘麦琪&孙怡敏
 * @brief 遥控器DR16
 * @version 0.1
 * @date 2025-02-24 0.1 23赛季定稿
 *
 * @copyright NEUQ-RoboPioneer (c) 2024-2025
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_OPS9.h"
#include "drv_uart.h"


/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

//CubeMx配置DMA，只是配置DMA模式为串口到内存（DMA初始化）

/**
 * @brief ops9定位器初始化，根据传入的 huart 参数，选择相应的 UART 管理对象（自动）
 *
 * @param huart 指定的UART
 */
void Class_OPS9::Init(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        UART_Manage_Object = &UART1_Manage_Object;
    }
    else if (huart->Instance == USART2)
    {
        UART_Manage_Object = &UART2_Manage_Object;
    }
    else if (huart->Instance == USART3)
    {
        UART_Manage_Object = &UART3_Manage_Object;
    }
    else if (huart->Instance == UART4)
    {
        UART_Manage_Object = &UART4_Manage_Object;
    }
    else if (huart->Instance == UART5)
    {
        UART_Manage_Object = &UART5_Manage_Object;
    }
    else if (huart->Instance == USART6)
    {
        UART_Manage_Object = &UART6_Manage_Object;
    }
    else if (huart->Instance == UART7)
    {
        UART_Manage_Object = &UART7_Manage_Object;
    }
    else if (huart->Instance == UART8)
    {
        UART_Manage_Object = &UART8_Manage_Object;
    }
		else if (huart->Instance == UART9)
    {
        UART_Manage_Object = &UART9_Manage_Object;
    }
		else if (huart->Instance == USART10)
    {
        UART_Manage_Object = &UART10_Manage_Object;
    }
}

/**
 * @brief UART通信接收回调函数，UART 接收到数据后马上调用此函数，tsk_config_and_callback里面会调用这个函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_OPS9::UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length)  
{
		Flag += 1;
	
		Data_Process(Length);
}

void Class_OPS9::TIM_100ms_Alive_PeriodElapsedCallback()
{
    // 判断该时间段内是否接收过遥控器DR16数据
    if (Flag == Pre_Flag)
    {
        // 遥控器DR16断开连接
        OPS9_Status = OPS9_Status_DISABLE;

        UART_Reinit(UART_Manage_Object->UART_Handler);
    }
    else
    {
        // 遥控器DR16保持连接
        OPS9_Status = OPS9_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief 数据处理过程,要用的数据分配
 */
void Class_OPS9::Data_Process(uint16_t Length)
{
		// 数据处理过程，通过 tmp_buffer指针可以访问从 UART 接收到的具体数据
    Struct_OPS9_UART_Data *tmp_buffer = (Struct_OPS9_UART_Data *) UART_Manage_Object->Rx_Buffer;
		//（UART_Manage_Object结构体里面有Rx_Buffer）将从 UART 接收的原始数据缓冲区 Rx_Buffer 转换为一个指向 Struct_OPS9_UART_Data的结构体类型的指针，并将其赋值给 tmp_buffer
		//tmp_buffer 可以作为指向 Struct_OPS9_UART_Data 结构体的指针来访问 Rx_Buffer 中的具体数据；tmp_buffer 指向的内存区域已经按 Struct_OPS9_UART_Data 结构体的格式来解析 UART 接收到的数据
	
		if(tmp_buffer->head1 == 0x0D && tmp_buffer->head2 == 0x0A && tmp_buffer->end1 == 0x0A && tmp_buffer->end2 == 0x0D)
		{
			Data.ops_positioning_x = tmp_buffer->pos_x;
			Data.ops_positioning_y = tmp_buffer->pos_y;
			Data.ops_positioning_angle = tmp_buffer->zangle * DEG_TO_RAD;
		}
}


/**
 * @brief 更新X坐标
 * 
 * 该函数根据OPS9设备的状态和X坐标的初始化状态，更新OPS9设备的X坐标。
 * 
 * @param New_x 新的X坐标值
 */
void Class_OPS9::Update_X(float New_x) 
{
    // 检查OPS9设备是否处于启用状态
    if(OPS9_Status == OPS9_Status_ENABLE)
    {
        // 根据X坐标的初始化状态进行不同处理
        switch(OPS9_Status_X)
        {
            // 当X坐标未初始化时
            case(OPS9_Status_X_UNINITIALIZED):
            {
                // 构造数据包的标识符 "ACTX"
                UART_Manage_Object->Tx_Buffer[0] = 'A'; 
                UART_Manage_Object->Tx_Buffer[1] = 'C';
                UART_Manage_Object->Tx_Buffer[2] = 'T';
                UART_Manage_Object->Tx_Buffer[3] = 'X';
                
                // 定义一个联合体，用于将浮点数转换为字符数组
                static union
                {
                    float X;
                    char data[4];
                }New_set_X;
                // 将新的X坐标值存储到联合体中
                New_set_X.X = New_x;
                // 将联合体中的字符数组复制到发送缓冲区
                for (int i = 0; i < 4; i++)
                {
                    // 拷贝到Tx_Buffer[4]~Tx_Buffer[7]
                    memcpy(UART_Manage_Object->Tx_Buffer + (i + 4) * sizeof(uint8_t), &New_set_X.data[i], sizeof(uint8_t)); 
                }
                
                // 通过UART发送数据
                UART_Send_Data(UART_Manage_Object->UART_Handler, UART_Manage_Object->Tx_Buffer, 8);
                
                // 将X坐标的状态标记为已初始化
                OPS9_Status_X = OPS9_Status_X_INITIALIZED;
                
                break;
            }
            // 当X坐标已初始化时，不做任何处理
            case(OPS9_Status_X_INITIALIZED):
            {
                break;
            }
        }
    }
}

/**
 * @brief 更新Y坐标
 * 
 * 该函数根据OPS9设备的状态和Y坐标的初始化状态，更新OPS9设备的Y坐标。
 * 
 * @param New_y 新的Y坐标值
 */
void Class_OPS9::Update_Y(float New_y) 
{
    // 检查OPS9设备是否处于启用状态
    if(OPS9_Status == OPS9_Status_ENABLE)
    {
        // 根据Y坐标的初始化状态进行不同处理
        switch(OPS9_Status_Y)
        {
            // 当Y坐标未初始化时
            case(OPS9_Status_Y_UNINITIALIZED):
            {
                // 构造数据包的标识符 "ACTY"
                UART_Manage_Object->Tx_Buffer[0] = 'A'; 
                UART_Manage_Object->Tx_Buffer[1] = 'C';
                UART_Manage_Object->Tx_Buffer[2] = 'T';
                UART_Manage_Object->Tx_Buffer[3] = 'Y';
                
                // 定义一个联合体，用于将浮点数转换为字符数组
                static union
                {
                    float Y;
                    char data[4];
                }New_set_Y;
                // 将新的Y坐标值存储到联合体中
                New_set_Y.Y = New_y;
                
                // 使用memcpy函数将联合体中的字符数组复制到发送缓冲区
                for (int i = 0; i < 4; i++)
                {
                    // 拷贝到Tx_Buffer[4]~Tx_Buffer[7]
                    memcpy(UART_Manage_Object->Tx_Buffer + (i + 4) * sizeof(uint8_t), &New_set_Y.data[i], sizeof(uint8_t)); 
                }
                
                // 通过UART发送数据
                UART_Send_Data(UART_Manage_Object->UART_Handler, UART_Manage_Object->Tx_Buffer, 8);
                
                // 将Y坐标的状态标记为已初始化
                OPS9_Status_Y = OPS9_Status_Y_INITIALIZED;
                
                break;
            }
            // 当Y坐标已初始化时，不做任何处理
            case(OPS9_Status_Y_INITIALIZED):
            {
                break;
            }
        }
    }
}

/**
 * @brief 更新OPS9设备的X和Y坐标
 * 
 * 该函数用于更新OPS9设备的X和Y坐标。
 * 
 * @param new_x 新的X坐标值
 * @param new_y 新的Y坐标值
 */
void Class_OPS9::OPS_Update_XY(float new_x,float new_y)
{
    // 调用 Update_Y 函数更新Y坐标
    Update_Y(new_y);
    // 延时10毫秒，确保Y坐标更新完成
    HAL_Delay(10);
    // 调用 Update_X 函数更新X坐标
    Update_X(new_x);
    // 延时10毫秒，确保X坐标更新完成
    HAL_Delay(10);
}


//// 字符串拼接功能
//void Class_OPS9::Stract(char strDestination[], char strSource[], int num)
//{
//    int i = 0, j = 0;
//    while (strDestination[i] != '\0') i++;
//    for (j = 0; j < num; j++) strDestination[i++] = strSource[j];
//}

//// 更新 X
////Update_x[8] = ACTX（4个字符即4个字节）+New_x（float类型4个字节32位）

//void Class_OPS9::Update_X(float New_x) 
//{
//    char Update_x[8] = "ACTX";
//    union {
//        float X;
//        char data[4];
//    } New_set;
//    New_set.X = New_x;  //将 New_x 以浮动类型赋值给 X
//		
//    // 使用 Stract 拼接数据
//    Stract(Update_x, New_set.data, 4);

//    // 模拟USART数据发送
//    for (int i = 0; i < 8; i++) {
//        while (!USART_GetFlagStatus(USART6, USART_FLAG_TXE)) {
//            // 等待数据发送完成
//        }
//        UART_Send_Data(USART10, Update_x[i],8);
//    }
//}

//// 更新 y
//void Class_OPS9::Update_Y(float New_y)
//{
//    int i = 0;
//    char Update_y[8] = "ACTY";
//    static union
//    {
//        float X;
//        char data[4];
//    } New_set;
//    New_set.X = New_y;
//    Stract(Update_y, New_set.data, 4);

//    for (i = 0; i < 8; i++)
//    {
//        while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);
//        UART_Send_Data(&UART10, UART10_Manage_Object.Tx_Buffer,8);  //Update_y[i],
//    }
//}

////更新x和y
//void Class_OPS9::ops_update_XY(float new_x, float new_y) {
//    Update_X(new_x);
//    HAL_Delay(10);  // 假设delay_ms是一个全局的延时函数
//    Update_Y(new_y);
//    HAL_Delay(10);
//}

/************************ COPYRIGHT(C) ROBOPIONEER **************************/
