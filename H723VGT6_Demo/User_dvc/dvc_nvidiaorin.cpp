/**
 * @file dvc_nvidiaorin.cpp
 * @author Lucy (2478427315@qq.com)
 * @brief orin上位机通信
 * @version 0.1
 * @date 2025-03-01
 *
 * @copyright ROBOPIONEER (c) 2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_nvidiaorin.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
Struct_Orin_UART_Rx_Data test;


// CRC8校验码
static const uint8_t crc_8_table[256] = {0,
                                        94,
                                        188,
                                        226,
                                        97,
                                        63,
                                        221,
                                        131,
                                        194,
                                        156,
                                        126,
                                        32,
                                        163,
                                        253,
                                        31,
                                        65,
                                        157,
                                        195,
                                        33,
                                        127,
                                        252,
                                        162,
                                        64,
                                        30,
                                        95,
                                        1,
                                        227,
                                        189,
                                        62,
                                        96,
                                        130,
                                        220,
                                        35,
                                        125,
                                        159,
                                        193,
                                        66,
                                        28,
                                        254,
                                        160,
                                        225,
                                        191,
                                        93,
                                        3,
                                        128,
                                        222,
                                        60,
                                        98,
                                        190,
                                        224,
                                        2,
                                        92,
                                        223,
                                        129,
                                        99,
                                        61,
                                        124,
                                        34,
                                        192,
                                        158,
                                        29,
                                        67,
                                        161,
                                        255,
                                        70,
                                        24,
                                        250,
                                        164,
                                        39,
                                        121,
                                        155,
                                        197,
                                        132,
                                        218,
                                        56,
                                        102,
                                        229,
                                        187,
                                        89,
                                        7,
                                        219,
                                        133,
                                        103,
                                        57,
                                        186,
                                        228,
                                        6,
                                        88,
                                        25,
                                        71,
                                        165,
                                        251,
                                        120,
                                        38,
                                        196,
                                        154,
                                        101,
                                        59,
                                        217,
                                        135,
                                        4,
                                        90,
                                        184,
                                        230,
                                        167,
                                        249,
                                        27,
                                        69,
                                        198,
                                        152,
                                        122,
                                        36,
                                        248,
                                        166,
                                        68,
                                        26,
                                        153,
                                        199,
                                        37,
                                        123,
                                        58,
                                        100,
                                        134,
                                        216,
                                        91,
                                        5,
                                        231,
                                        185,
                                        140,
                                        210,
                                        48,
                                        110,
                                        237,
                                        179,
                                        81,
                                        15,
                                        78,
                                        16,
                                        242,
                                        172,
                                        47,
                                        113,
                                        147,
                                        205,
                                        17,
                                        79,
                                        173,
                                        243,
                                        112,
                                        46,
                                        204,
                                        146,
                                        211,
                                        141,
                                        111,
                                        49,
                                        178,
                                        236,
                                        14,
                                        80,
                                        175,
                                        241,
                                        19,
                                        77,
                                        206,
                                        144,
                                        114,
                                        44,
                                        109,
                                        51,
                                        209,
                                        143,
                                        12,
                                        82,
                                        176,
                                        238,
                                        50,
                                        108,
                                        142,
                                        208,
                                        83,
                                        13,
                                        239,
                                        177,
                                        240,
                                        174,
                                        76,
                                        18,
                                        145,
                                        207,
                                        45,
                                        115,
                                        202,
                                        148,
                                        118,
                                        40,
                                        171,
                                        245,
                                        23,
                                        73,
                                        8,
                                        86,
                                        180,
                                        234,
                                        105,
                                        55,
                                        213,
                                        139,
                                        87,
                                        9,
                                        235,
                                        181,
                                        54,
                                        104,
                                        138,
                                        212,
                                        149,
                                        203,
                                        41,
                                        119,
                                        244,
                                        170,
                                        72,
                                        22,
                                        233,
                                        183,
                                        85,
                                        11,
                                        136,
                                        214,
                                        52,
                                        106,
                                        43,
                                        117,
                                        151,
                                        201,
                                        74,
                                        20,
                                        246,
                                        168,
                                        116,
                                        42,
                                        200,
                                        150,
                                        21,
                                        75,
                                        169,
                                        247,
                                        182,
                                        232,
                                        10,
                                        84,
                                        215,
                                        137,
                                        107,
                                        53,};

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief Orin初始化
 *
 * @param huart 指定的UART
 * @param __Frame_Rear 数据包头标
 */
void Class_Orin::Init(UART_HandleTypeDef *huart, uint16_t __Frame_Header, uint16_t __Frame_Rear)
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
    Frame_Header = __Frame_Header;
    Frame_Rear = __Frame_Rear;
    Tx_Data.Frame_Header = __Frame_Header;
    Tx_Data.Frame_Rear = __Frame_Rear;
}

/**
 * @brief UART通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Orin::UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length)
{
    //滑动窗口, 判断Orin是否在线
    Flag += 1;

    Data_Process(Length);
}

/**
 * @brief TIM定时器中断定期检测Orin是否存活
 *
 */
void Class_Orin::TIM_1000ms_Alive_PeriodElapsedCallback()
{
    //判断该时间段内是否接收过Orin数据
    if (Flag == Pre_Flag)
    {
        // Orin断开连接
        Orin_Status = Orin_Status_DISABLE;

        Rx_Data.Frame_Header = 0;
        Rx_Data.Data_Size = 0;
        Rx_Data.Target_Velocity_X = 0;
        Rx_Data.Target_Velocity_Y = 0;
        Rx_Data.Target_Omega = 0;
	   Rx_Data.Yaw_Target_Omega = 0;
	   Rx_Data.Pitch_Target_Omega = 0;
        Rx_Data.Flag_Todo1 = 0;
        Rx_Data.Flag_Todo2 = 0;
        Rx_Data.Flag_Todo3 = 0;
        Rx_Data.Flag_Todo4 = 0;
        Rx_Data.Flag_Todo5 = 0;
        Rx_Data.Flag_Todo6 = 0;
        Rx_Data.CRC_8 = 0;
        Rx_Data.Frame_Rear = 0;

        UART_Reinit(UART_Manage_Object->UART_Handler);
    }
    else
    {
        // Orin保持连接
        Orin_Status = Orin_Status_ENABLE;
    }
    Pre_Flag = Flag;
}

/**
 * @brief TIM定时器中断计算回调函数
 *
 */
void Class_Orin::TIM_10ms_Send_PeriodElapsedCallback()
{
    Output();
    // 此处数据包长度要改,在h文件中
    UART_Send_Data(UART_Manage_Object->UART_Handler, UART_Manage_Object->Tx_Buffer, Tx_Size);
}

/**
 * @brief 数据处理过程
 *
 * @param Rx_Data 接收的数据
 */
void Class_Orin::Data_Process(uint16_t Length)
{
    // 数据处理过程
    Struct_Orin_UART_Rx_Data *tmp_buffer = (Struct_Orin_UART_Rx_Data *) UART_Manage_Object->Rx_Buffer;
		test = *tmp_buffer;
	
    // 未通过头校验
    if (tmp_buffer->Frame_Header != Frame_Header)
    {
        return;
    }
    // 未通过尾校验
    if (tmp_buffer->Frame_Rear != Frame_Rear)
    {
        return;
    }
    if (Verify_CRC_8((uint8_t *) tmp_buffer, Rx_Data_Size + 3) != tmp_buffer->CRC_8)
    {
        return;
    }
    
    Rx_Data.Frame_Header = tmp_buffer->Frame_Header;
    Rx_Data.Data_Size = tmp_buffer->Data_Size;
    Rx_Data.Target_Velocity_X = tmp_buffer->Target_Velocity_X;
    Rx_Data.Target_Velocity_Y = tmp_buffer->Target_Velocity_Y;
    Rx_Data.Target_Omega = tmp_buffer->Target_Omega;
    Rx_Data.Yaw_Target_Omega = tmp_buffer->Yaw_Target_Omega;
    Rx_Data.Pitch_Target_Omega = tmp_buffer->Pitch_Target_Omega;
    // 一些逻辑判断而不只是接收
    Rx_Data.Flag_Todo1 = tmp_buffer->Flag_Todo1;
    Rx_Data.Flag_Todo2 = tmp_buffer->Flag_Todo2;
    Rx_Data.Flag_Todo3 = tmp_buffer->Flag_Todo3;
    Rx_Data.Flag_Todo4 = tmp_buffer->Flag_Todo4;
    Rx_Data.Flag_Todo5 = tmp_buffer->Flag_Todo5;
    Rx_Data.Flag_Todo6 = tmp_buffer->Flag_Todo6;
    Rx_Data.CRC_8 = tmp_buffer->CRC_8;

}
    

/**
 * @brief Orin数据输出到UART发送缓冲区
 *
 */
void Class_Orin::Output()
{
    Struct_Orin_UART_Tx_Data *tmp_buffer = (Struct_Orin_UART_Tx_Data *) UART_Manage_Object->Tx_Buffer;

    tmp_buffer->Frame_Header = Tx_Data.Frame_Header;
    tmp_buffer->Data_Size = Tx_Data_Size;
    tmp_buffer->Now_Velocity_X = Tx_Data.Now_Velocity_X;
    tmp_buffer->Now_Velocity_Y = Tx_Data.Now_Velocity_Y;
    tmp_buffer->Now_Omega = Tx_Data.Now_Omega;
    tmp_buffer->Flag_Todo1 = Tx_Data.Flag_Todo1;
    tmp_buffer->Flag_Todo2 = Tx_Data.Flag_Todo2;
    tmp_buffer->Flag_Todo3 = Tx_Data.Flag_Todo3;
    tmp_buffer->Flag_Todo4 = Tx_Data.Flag_Todo4;
    tmp_buffer->Flag_Todo5 = Tx_Data.Flag_Todo5;
    tmp_buffer->Flag_Todo6 = Tx_Data.Flag_Todo6;
    tmp_buffer->CRC_8 = Verify_CRC_8((uint8_t *) tmp_buffer, Tx_Data_Size + 3);
    tmp_buffer->Frame_Rear = Tx_Data.Frame_Rear;
}

/**
 * @brief Orin8bit循环冗余检验
 *
 * @param Message 消息
 * @param Length 字节数
 * @return uint8_t 校验码
 */
uint8_t Class_Orin::Verify_CRC_8(uint8_t *Message, uint32_t Length)
{
    uint8_t check = 0;

    if (Message == nullptr)
    {
        return (check);
    }

    for (int i = 0; i < Length; i++)
    {
        uint8_t value = Message[i];
        uint8_t new_index = check ^ value;
        check = crc_8_table[new_index];
    }
    return (check);
}
/************************ COPYRIGHT(C) ROBOPIONEER **************************/
