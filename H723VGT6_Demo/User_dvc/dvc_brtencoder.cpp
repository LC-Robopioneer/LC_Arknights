/**
 * @file dvc_brtencoder.cpp
 * @author Xiangyu1234 & Lucy
 * @brief 布瑞特多圈编码器配置及操作（对编码器发送CAN指令进行设置仍然存在问题，待修改）
 * @version 0.1
 * @date 2024-12-21 0.1 24-25赛季
 * @copyright NEUQ-RoboPioneer (c) 2024-2025
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_brtencoder.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/**
 * @brief 编码器初始化
 *
 * @param hfdcan 绑定的CAN句柄
 * @param __FDCAN_BRTEncoder_ID 编码器绑定的CAN ID
 * @param __CAN_ID 编码器的CAN ID 这里只能从0x00~0xFF 这是编码器自身传输指令的限制
 * @param __Mode 编码器模式 默认角度自动回传
 * @param __BPS 波特率 默认1M波特率
 * @param __Direction 旋转方向 默认顺时针方向
 * @param __auto_send_time 自动回传时间 注意：设置太短的返回时间后，通过编码器上位机再设置其他参数很容易失败，谨慎使用！默认1000us
 * @param __sampling_time 采样时间 默认1000us
 * @param __Gear_Ratio 传动齿数 默认为1 用于计算单圈角度。
 */
void Class_BRTEncoder::Init(FDCAN_HandleTypeDef *hfdcan, uint32_t __FDCAN_BRTEncoder_ID, Enum_BRTEncoder_Mode __Mode, Enum_BRTEncoder_BPS __BPS, Enum_BRTEncoder_Direction __Direction, uint16_t __auto_send_time, uint16_t __sampling_time, uint8_t __Gear_Ratio)
{
    if (hfdcan->Instance == FDCAN1)
    {
        FDCAN_Manage_Object = &FDCAN1_Manage_Object;
    }
    else if (hfdcan->Instance == FDCAN2)
    {
        FDCAN_Manage_Object = &FDCAN2_Manage_Object;
    }
    else if (hfdcan->Instance == FDCAN3)
    {
        FDCAN_Manage_Object = &FDCAN3_Manage_Object;
    }

    FDCAN_BRTEncoder_ID = __FDCAN_BRTEncoder_ID;

//    Mode = __Mode;

//    BPS = __BPS;

//    Direction = __Direction;

//    auto_send_time = __auto_send_time;

//    sampling_time = __sampling_time;

    Gear_Ratio = __Gear_Ratio;

    //Setting_to_BRTEncoder();
}

///**
// * @brief 将初始化设置传入编码器，注意通过上位机设置编码器ID
// *				bug
// */
//void Class_BRTEncoder::Setting_to_BRTEncoder()
//{
//    //定义FDCAN发送缓存区
//	uint8_t FDCAN_Tx_Buffer[8] = {0};

//    //设置波特率
//    //设置CAN通信的波特率默认都是1Mhz
//    FDCAN_Tx_Buffer[0] = 0x04;
//    FDCAN_Tx_Buffer[1] = FDCAN_BRTEncoder_ID;
//    FDCAN_Tx_Buffer[2] = 0x03;
//    switch (BPS)
//    {
//        case (bps_500k):
//        {
//            FDCAN_Tx_Buffer[3] = 0x00;
//            break;
//        }
//        case (bps_1M):
//        {
//            FDCAN_Tx_Buffer[3] = 0x01;
//            break;
//        }
//        case (bps_250k):
//        {
//            FDCAN_Tx_Buffer[3] = 0x02;
//            break;
//        }
//        case (bps_125k):
//        {
//            FDCAN_Tx_Buffer[3] = 0x03;
//            break;
//        }
//        case (bps_100k):
//        {
//            FDCAN_Tx_Buffer[3] = 0x04;
//            break;
//        }
//        default:
//            break;
//    }
//    //发送数据
//    FDCAN_Send_Data(FDCAN_Manage_Object->FDCAN_Handler, FDCAN_BRTEncoder_ID, FDCAN_Tx_Buffer);
//    //清空缓存区
//    for(int i = 0; i < 8; i++)
//    {
//        FDCAN_Tx_Buffer[i] = 0;
//    }

//    //设置编码器模式
//    FDCAN_Tx_Buffer[0] = 0x04;
//    FDCAN_Tx_Buffer[1] = FDCAN_BRTEncoder_ID;
//    FDCAN_Tx_Buffer[2] = 0x04;
//    switch(Mode)
//    {
//        case (Query_Mode):
//        {
//            FDCAN_Tx_Buffer[3] = 0x00;
//            break;
//        }
//        case (Auto_Read_Angle_mode):
//        {
//            FDCAN_Tx_Buffer[3] = 0xAA;
//            break;
//        }
//        case (Auto_Read_Omega_mode):
//        {
//            FDCAN_Tx_Buffer[3] = 0x02;
//            break;
//        }
//        default:
//            break;
//    }
//    //发送数据
//    FDCAN_Send_Data(FDCAN_Manage_Object->FDCAN_Handler, FDCAN_BRTEncoder_ID, FDCAN_Tx_Buffer);
//    //清空缓存区
//    for(int i = 0; i < 8; i++)
//    {
//        FDCAN_Tx_Buffer[i] = 0;
//    }

//    //设置自动回传时间
//    FDCAN_Tx_Buffer[0] = 0x05; //?
//    FDCAN_Tx_Buffer[1] = FDCAN_BRTEncoder_ID;
//    FDCAN_Tx_Buffer[2] = 0x05;
//    FDCAN_Tx_Buffer[3] = auto_send_time;        //低位数据
//    FDCAN_Tx_Buffer[4] = auto_send_time >> 8;   //高位数据
//    //发送数据
//    FDCAN_Send_Data(FDCAN_Manage_Object->FDCAN_Handler, FDCAN_BRTEncoder_ID, FDCAN_Tx_Buffer);
//    //清空缓存区
//    for(int i = 0; i < 8; i++)
//    {
//        FDCAN_Tx_Buffer[i] = 0;
//    }

//    //设置编码器值递增方向
//    FDCAN_Tx_Buffer[0] = 0x04;
//    FDCAN_Tx_Buffer[1] = FDCAN_BRTEncoder_ID;
//    FDCAN_Tx_Buffer[2] = 0x07;
//    switch(Direction)
//    {
//        case (Direction_Clockwise):
//        {
//            FDCAN_Tx_Buffer[3] = 0x00;
//            break;
//        }
//        case (Direction_AntiClockwise):
//        {
//            FDCAN_Tx_Buffer[3] = 0x01;
//            break;
//        }
//        default:
//            break;
//    }
//    //发送数据
//    FDCAN_Send_Data(FDCAN_Manage_Object->FDCAN_Handler, FDCAN_BRTEncoder_ID, FDCAN_Tx_Buffer);
//    //清空缓存区
//    for(int i = 0; i < 8; i++)
//    {
//        FDCAN_Tx_Buffer[i] = 0;
//    }

//     //设置角速度采样时间
//    FDCAN_Tx_Buffer[0] = 0x05;
//    FDCAN_Tx_Buffer[1] = FDCAN_BRTEncoder_ID;
//    FDCAN_Tx_Buffer[2] = 0x0B;
//    FDCAN_Tx_Buffer[3] = sampling_time;        //低位数据
//    FDCAN_Tx_Buffer[4] = sampling_time >> 8;   //高位数据
//    //发送数据
//    FDCAN_Send_Data(FDCAN_Manage_Object->FDCAN_Handler, FDCAN_BRTEncoder_ID, FDCAN_Tx_Buffer);
//}

///**
// * @brief 角度查询函数，仅仅在查询模式时使用
// *
// */
//void Class_BRTEncoder::Angle_Query()
//{
//    //定义FDCAN发送缓存区
//	uint8_t FDCAN_Tx_Buffer[8] = {0};

//    FDCAN_Tx_Buffer[0] = 0x04;
//    FDCAN_Tx_Buffer[1] = FDCAN_BRTEncoder_ID;
//    FDCAN_Tx_Buffer[2] = 0x01;
//    FDCAN_Tx_Buffer[3] = 0x00;

//    //发送数据
//    FDCAN_Send_Data(FDCAN_Manage_Object->FDCAN_Handler, FDCAN_BRTEncoder_ID, FDCAN_Tx_Buffer);
//}

/**
 * @brief 角速度查询函数，仅仅在查询模式时使用
 *
 */
void Class_BRTEncoder::Omega_Query()
{
    //定义FDCAN发送缓存区
	uint8_t FDCAN_Tx_Buffer[8] = {0};

    FDCAN_Tx_Buffer[0] = 0x04;
    FDCAN_Tx_Buffer[1] = FDCAN_BRTEncoder_ID;
    FDCAN_Tx_Buffer[2] = 0x0A;
    FDCAN_Tx_Buffer[3] = 0x00;

    //发送数据
    FDCAN_Send_Data(FDCAN_Manage_Object->FDCAN_Handler, FDCAN_BRTEncoder_ID, FDCAN_Tx_Buffer);
}

/**
 * @brief CAN通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_BRTEncoder::FDCAN_RxCpltCallback(uint8_t *Rx_Data)
{
    // 滑动标志位
    Flag += 1;

    Data_Process();
}

/**
 * @brief 100ms编码器存活检测
 *
 */
void Class_BRTEncoder::TIM_100ms_Alive_PeriodElapsedCallback()
{
    // 判断计时标志位是否发生变化
    if (Flag == Pre_Flag)
    {
        // 设备断开连接
        BRTEncoder_Status = Motor_BRTEncoder_DISABLE;
    }
    else
    {
        // 设备连接正常
        BRTEncoder_Status = Motor_BRTEncoder_ENABLE;
    }

    Pre_Flag = Flag;
}



/**
 * @brief 编码器数据处理
 *
 */
void Class_BRTEncoder::Data_Process()
{
	// 数据指针传递
    Struct_BRTEncoder_Rx_Data_Origin *tmp_buffer = (Struct_BRTEncoder_Rx_Data_Origin *)FDCAN_Manage_Object->Rx_Buffer.Data;

    // ID不对应则直接返回
    if ((FDCAN_Manage_Object->Rx_Buffer.Header.Identifier) != FDCAN_BRTEncoder_ID)
    {
        return;
    }

    switch(tmp_buffer->Func_ID)
    {
        // 角度处理
        case (0x01):
        {
            // 数据处理	十位分辨率
            Rx_Data.Now_Angle = (static_cast<float>(tmp_buffer->Data_Origin) * 360.0f / 1024.0f) * DEG_TO_RAD;
            // 多圈编码器单圈角度
            Rx_Data.Now_Angle_Single = fmod((static_cast<float>(tmp_buffer->Data_Origin) * 360.0f / 1024.0f), Gear_Ratio * 360) / static_cast<float>(Gear_Ratio) * DEG_TO_RAD;
            break;
        }
        // 角速度处理
        case (0x0A):
        {
            // 数据处理	十位分辨率  注意采样时间为微秒
            Rx_Data.Now_Omega = ((static_cast<float>(tmp_buffer->Data_Origin)) / (1024.0f * sampling_time / 1000000.0f / 60.0f)) * DEG_TO_RAD;
            break;
        }
        default:
            break;
    }
}

/************************ COPYRIGHT(C) ROBOPIONEER **************************/
