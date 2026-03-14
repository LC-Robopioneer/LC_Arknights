/**
 * @file dvc_brtencoder.h
 * @author Xiangyu1234 & Lucy
 * @brief 布瑞特多圈编码器
 * @version 0.1
 * @date 2024-12-21 0.1 24-25赛季
 *
 * @copyright NEUQ-RoboPioneer (c) 2024-2025
 *
 */

#ifndef DVC_BRTENCODER_CPP
#define DVC_BRTENCODER_CPP

/* Includes ------------------------------------------------------------------*/

#include "alg_pid.h"
#include "drv_can.h"
#include "drv_math.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 编码器状态
 *
 */
enum Enum_BRTEncoder_Status
{
    Motor_BRTEncoder_DISABLE = 0,
    Motor_BRTEncoder_ENABLE,
};

/**
 * @brief 波特率枚举
 *
 */
enum Enum_BRTEncoder_BPS
{
    bps_500k = 0,
    bps_1M,
    bps_250k,
    bps_125k,
    bps_100k
};

/**
 * @brief 旋转方向枚举
 *
 */
enum Enum_BRTEncoder_Direction
{
    Direction_Clockwise = 0,
    Direction_AntiClockwise
};

/**
 * @brief 回传方式枚举
 *
 */
enum Enum_BRTEncoder_Mode
{
    Query_Mode = 0,
    Auto_Read_Angle_mode,
    Auto_Read_Omega_mode
};

/**
 * @brief 接收初始数据结构体
 *
 */
struct Struct_BRTEncoder_Rx_Data_Origin
{
    //数据最高就为七位所以不需要第八位
    uint8_t Data_Length;
    uint8_t BRTEncoder_ID;
    uint8_t Func_ID; 
    uint32_t Data_Origin;
} __attribute__((packed));

/**
 * @brief MKSESC电调返回电机各项实际参数
 *
 */
struct Struct_BRTEncoder_Rx_Data
{
    float Now_Angle;
    float Now_Angle_Single;
    float Now_Omega;
};

/**
 * @brief 编码器类
 *
 */
class Class_BRTEncoder
{
public:
    void Init(FDCAN_HandleTypeDef *hfdcan,uint32_t __FDCAN_BRTEncoder_ID, Enum_BRTEncoder_Mode __Mode = Auto_Read_Angle_mode, Enum_BRTEncoder_BPS __BPS = bps_1M, Enum_BRTEncoder_Direction __Direction = Direction_Clockwise, uint16_t __auto_send_time = 1000, uint16_t __sampling_time = 1000, uint8_t __Gear_Ratio = 1);

    inline uint8_t Get_FDCAN_BRTEncoder_ID();

    inline Enum_BRTEncoder_Mode Get_Mode();

    inline uint8_t Get_BPS();

    inline Enum_BRTEncoder_Direction Get_Direction();

    inline uint16_t Get_Auto_Send_Time();

    inline uint16_t Get_Sampling_Time();

    inline float Get_Now_Angle();

		inline float Get_Now_Angle_Single();

		inline float Get_Now_Omega();

		void Setting_to_BRTEncoder();

    void Angle_Query();

    void Omega_Query();

    void FDCAN_RxCpltCallback(uint8_t *Rx_Data);

    void TIM_100ms_Alive_PeriodElapsedCallback();

    void TIM_Send_PeriodElapsedCallback();

protected:
    //内部变量

    //转动机构与编码器的齿数比
    uint8_t Gear_Ratio;

		//FDCAN接收结构体
		Struct_FDCAN_Manage_Object *FDCAN_Manage_Object;

		// 编码器FDCANID
    uint32_t FDCAN_BRTEncoder_ID;

		//编码器当前模式
    Enum_BRTEncoder_Mode Mode;

		//波特率
    Enum_BRTEncoder_BPS BPS;

		//编码器正方向
    Enum_BRTEncoder_Direction Direction;

		//自动回传时间
    uint16_t auto_send_time;

		//采样时间
    uint16_t sampling_time;

		//编码器状态
    Enum_BRTEncoder_Status BRTEncoder_Status = Motor_BRTEncoder_DISABLE;
		
		//数据接收标志位
    uint32_t Flag = 0;
    uint32_t Pre_Flag = 0;
		
		//接收实际数据
    Struct_BRTEncoder_Rx_Data Rx_Data;

    void Data_Process();

    void Output();
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取编码器的FDCAN ID
 * 
 * 此函数用于获取布瑞特编码器对应的FDCAN ID，该ID用于在CAN总线通信中识别该编码器。
 * 
 * @return uint8_t 编码器的FDCAN ID
 */
inline uint8_t Class_BRTEncoder::Get_FDCAN_BRTEncoder_ID()
{
    return (FDCAN_BRTEncoder_ID);
}

/**
 * @brief 获取编码器的FDCAN ID
 * 
 * 此函数用于获取布瑞特编码器对应的FDCAN ID，该ID用于在CAN总线通信中识别该编码器。
 * 
 * @return uint8_t 编码器的FDCAN ID
 */
inline Enum_BRTEncoder_Mode Class_BRTEncoder::Get_Mode()
{
    return (Mode);
}

/**
 * @brief 获取编码器的波特率
 * 
 * 此函数用于获取布瑞特编码器当前配置的波特率，返回值为 `Enum_BRTEncoder_BPS` 枚举类型。
 * 
 * @return Enum_BRTEncoder_BPS 编码器当前的波特率
 */
inline uint8_t Class_BRTEncoder::Get_BPS()
{
    return (BPS);
}

/**
 * @brief 获取编码器的旋转方向
 * 
 * 此函数用于获取布瑞特编码器当前配置的旋转方向，返回值为 `Enum_BRTEncoder_Direction` 枚举类型。
 * 
 * @return Enum_BRTEncoder_Direction 编码器当前的旋转方向
 */
inline Enum_BRTEncoder_Direction Class_BRTEncoder::Get_Direction()
{
    return (Direction);
}

/**
 * @brief 获取编码器的自动回传时间
 * 
 * 此函数用于获取布瑞特编码器当前配置的自动回传时间，单位为毫秒。
 * 
 * @return uint16_t 编码器的自动回传时间
 */
inline uint16_t Class_BRTEncoder::Get_Auto_Send_Time()
{
    return (auto_send_time);
}

/**
 * @brief 获取编码器的采样时间
 * 
 * 此函数用于获取布瑞特编码器当前配置的采样时间，单位为毫秒。
 * 
 * @return uint16_t 编码器的采样时间
 */
inline uint16_t Class_BRTEncoder::Get_Sampling_Time()
{
    return (sampling_time);
}

/**
 * @brief 获取编码器的当前角度
 * 
 * 此函数用于获取布瑞特编码器的当前角度值，返回值为浮点数类型。
 * 
 * @return float 编码器的当前角度
 */
inline float Class_BRTEncoder::Get_Now_Angle()
{
    return (Rx_Data.Now_Angle);
}

/**
 * @brief 获取编码器的当前单圈角度
 * 
 * 此函数用于获取布瑞特编码器的当前单圈角度值，返回值为浮点数类型。
 * 
 * @return float 编码器的当前单圈角度
 */
inline float Class_BRTEncoder::Get_Now_Angle_Single()
{
    return (Rx_Data.Now_Angle_Single);
}

/**
 * @brief 获取编码器的当前角速度
 * 
 * 此函数用于获取布瑞特编码器的当前角速度值，返回值为浮点数类型。
 * 
 * @return float 编码器的当前角速度
 */
inline float Class_BRTEncoder::Get_Now_Omega()
{
    return (Rx_Data.Now_Omega);
}
#endif

/************************ COPYRIGHT(C) ROBOPIONEER **************************/
