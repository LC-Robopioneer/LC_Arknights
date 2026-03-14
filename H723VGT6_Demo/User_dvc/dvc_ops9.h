/**
 * @file dvc_ops9.cpp
 * @author 刘麦琪&孙怡敏
 * @brief OPS9定位器配置与操作
 * @version 0.1
 * @date 2025-02-24 0.1 23赛季定稿
 *
 * @copyright NEUQ-RoboPioneer (c) 2024-2025
 *
 */
#ifndef DVC_OPS9_H
#define DVC_OPS9_H
/* Includes ------------------------------------------------------------------*/
#include "drv_uart.h"
#include "drv_math.h"
#include <string>

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 定位器OPS9状态
 *
 */
enum Enum_OPS9_Status
{
    OPS9_Status_DISABLE = 0,
    OPS9_Status_ENABLE,
};

/**
 * @brief 定位器OPS9坐标X初始化状态
 *
 */
enum Enum_OPS9_Status_X
{
    OPS9_Status_X_UNINITIALIZED = 0,
    OPS9_Status_X_INITIALIZED,
};

/**
 * @brief 定位器OPS9坐标Y初始化状态
 *
 */
enum Enum_OPS9_Status_Y
{
    OPS9_Status_Y_UNINITIALIZED = 0,
    OPS9_Status_Y_INITIALIZED,
};

/**
 * @brief OPS9源数据
 
 * 数据形式 0x0D 0x0A [24字节数据] 0x0A 0x0D
 * 帧头2字节   24字节转化为6个float类型数值   帧尾2字节
 
 */
struct Struct_OPS9_UART_Data
{
	uint8_t head1;  //0x0D  //1字节即8位
	uint8_t head2;  //0x0A
	float zangle; //转向    //float是32位，4字节 一字节有8位
	float xangle;
	float yangle;
	float pos_x;  //坐标x
	float pos_y;  //坐标y
	float w_z;
	uint8_t end1;  //0x0A
	uint8_t end2;  //0x0D
} __attribute__((packed));


/**
 * @brief 处理后的OPS9数据源
 *
 */
struct Struct_OPS9_Data
{
	float ops_positioning_x;
	float ops_positioning_y;
	float ops_positioning_angle;
};

/**
 * @brief Specialized, OPS9
 *
 */
class Class_OPS9
{
public:
    void Init(UART_HandleTypeDef *huart);

	inline Enum_OPS9_Status Get_Status();

	void UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length);

	void OPS_Update_XY(float new_x,float new_y);

	void TIM_100ms_Alive_PeriodElapsedCallback();

	void Update_X(float New_x);

	void Update_Y(float New_y);

	inline Enum_OPS9_Status_X Get_Status_X();
		
	inline Enum_OPS9_Status_Y Get_Status_Y();

	inline float Get_Position_X();

	inline float Get_Position_Y();

	inline float Get_Position_Angle();

protected:
    // 初始化相关常量

    // 绑定的UART
    Struct_UART_Manage_Object *UART_Manage_Object;

	// 常量


	// 内部变量
	uint32_t Flag = 0;

	uint32_t Pre_Flag = 0;
	// 读变量
	Enum_OPS9_Status OPS9_Status = OPS9_Status_DISABLE;

	Enum_OPS9_Status_X OPS9_Status_X = OPS9_Status_X_UNINITIALIZED;

	Enum_OPS9_Status_Y OPS9_Status_Y = OPS9_Status_Y_UNINITIALIZED;
	// OPS9对外接口信息	
	Struct_OPS9_Data Data;  //和.c开头的Struct_OPS9_UART_Data test有点像，这里放protected是很多地方也有Data防止混乱

	// 写变量

	// 读写变量
	
	// 内部函数
	void Data_Process(uint16_t Length);
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取定位器OPS9的状态
 * 
 * 该函数用于获取定位器OPS9的当前状态，返回值为 Enum_OPS9_Status 枚举类型，
 * 
 * @return Enum_OPS9_Status 定位器OPS9的当前状态
 */
inline Enum_OPS9_Status Class_OPS9::Get_Status()
{
    return (OPS9_Status);
}

/**
 * @brief 获取定位器OPS9的X坐标初始化状态
 * 
 * 该函数用于获取定位器OPS9的X坐标当前的初始化状态，返回值为 Enum_OPS9_Status_X 枚举类型，
 * 
 * @return Enum_OPS9_Status_X 定位器OPS9的X坐标初始化状态
 */
inline Enum_OPS9_Status_X Class_OPS9::Get_Status_X()
{
    return (OPS9_Status_X);
}

/**
 * @brief 获取定位器OPS9的Y坐标初始化状态
 * 
 * 该函数用于获取定位器OPS9的Y坐标当前的初始化状态，返回值为 Enum_OPS9_Status_Y 枚举类型，
 * 
 * @return Enum_OPS9_Status_Y 定位器OPS9的Y坐标初始化状态
 */
inline Enum_OPS9_Status_Y Class_OPS9::Get_Status_Y()
{
    return (OPS9_Status_Y);
}

/**
 * @brief 获取OPS9设备的X轴坐标位置
 * 
 * 该函数用于从OPS9设备数据中读取当前X轴方向的位置坐标值
 * 
 * @return float 返回X轴坐标位置浮点数值
 */
inline float Class_OPS9::Get_Position_X()
{
	return (Data.ops_positioning_x);
}

/**
 * @brief 获取OPS9设备的Y轴坐标位置
 * 
 * 该函数用于从OPS9设备数据中读取当前Y轴方向的位置坐标值
 * 
 * @return float 返回Y轴坐标位置浮点数值
 */
inline float Class_OPS9::Get_Position_Y()
{
	return (Data.ops_positioning_y);
}

/**
 * @brief 获取OPS9设备的航向角信息
 * 
 * 该函数用于从OPS9设备数据中读取当前姿态的Z轴方向角度值
 * 
 * @return float 返回设备航向角浮点数值（单位：度）
 */
inline float Class_OPS9::Get_Position_Angle()
{
	return (Data.ops_positioning_angle);
}

#endif

/************************ COPYRIGHT(C) ROBOPIONEER **************************/
