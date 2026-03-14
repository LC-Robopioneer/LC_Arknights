/**
 * @file dvc_motor_rmd.h
 * @brief RMD电机配置与操作
 * @version 0.1
 * @date 2024-10-07
 *
 * @copyright
 *
 */

#ifndef DVC_MOTOR_RMD_H
#define DVC_MOTOR_RMD_H

/* Includes ------------------------------------------------------------------*/

#include "drv_can.h"
#include "alg_filter.h"
#include <stdint.h>

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief RMD电机状态
 *
 */
enum Enum_Motor_RMD_Status
{
    Motor_RMD_Status_DISABLE = 0,
    Motor_RMD_Status_ENABLE,
};

/**
 * @brief RMD电机的ID枚举类型
 *
 */
enum Enum_Motor_RMD_ID
{
    Motor_RMD_ID_0x141 = 1,
    Motor_RMD_ID_0x142,
    Motor_RMD_ID_0x143,
    Motor_RMD_ID_0x144,
    Motor_RMD_ID_0x145,
    Motor_RMD_ID_0x146,
    Motor_RMD_ID_0x147,
    Motor_RMD_ID_0x148,
    Motor_RMD_ID_0x149,
    Motor_RMD_ID_0x14A,
    Motor_RMD_ID_0x14B,
    Motor_RMD_ID_0x14C,
    Motor_RMD_ID_0x14D,
    Motor_RMD_ID_0x14E,
    Motor_RMD_ID_0x14F,
    Motor_RMD_ID_0x150,
	
    // 添加更多ID根据需要
};

/**
 * @brief RMD电机控制方式
 *
 */
enum Enum_Motor_RMD_Control_Method
{
    Motor_RMD_Control_Method_VOLTAGE = 0,
    Motor_RMD_Control_Method_CURRENT,
    Motor_RMD_Control_Method_TORQUE,
    Motor_RMD_Control_Method_OMEGA,
    Motor_RMD_Control_Method_ANGLE,
};



// 定义数据范围
#define P_DES_MIN -12.5f
#define P_DES_MAX 12.5f
#define V_DES_MIN -45.0f
#define V_DES_MAX 45.0f
#define T_FF_MIN -24.0f
#define T_FF_MAX 24.0f
#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 100.0f



/**
 * @brief RMD电机类
 *
 */
class Class_Motor_RMD
{
public:
 

    void Init(FDCAN_HandleTypeDef *hfdcan, Enum_Motor_RMD_ID __FDCAN_Rx_ID, Enum_Motor_RMD_Control_Method __Motor_RMD_Control_Method = Motor_RMD_Control_Method_ANGLE, int32_t __Encoder_Offset = 0);

    inline float Get_Current_Max();
    inline float Get_Theoretical_Output_Current_Max();
    inline float Get_Now_Omega();
    inline float Get_Now_Current();
    inline float Get_Now_Temperature();
    inline float Get_Now_Power();
    inline float Get_Target_Angle();
    inline float Get_Target_Omega();
    inline float Get_Target_Current();


    inline void Set_Control_Method(Enum_Motor_RMD_Control_Method __Motor_RMD_Control_Method);
    inline void Set_Target_angle(int32_t __angle);
    inline void Set_Target_speed(int32_t __speed);

    inline void position_control_command1(int32_t actual_angle, uint16_t maxSpeed);
    inline void Get_Now_Angle();
    
    void Set_Status(Enum_Motor_RMD_Status __status);
    void FDCAN_RxCpltCallback(uint8_t *Rx_Data);
    void TIM_100ms_Alive_PeriodElapsedCallback();
    void TIM_2ms_Enable_PeriodElapsedCallback();
	void	Disable_Motor();
protected:
    // 初始化相关变量

    Struct_FDCAN_Manage_Object *FDCAN_Manage_Object;

    Enum_Motor_RMD_ID FDCAN_ID;

    uint8_t *Tx_Data;

    int32_t Encoder_Offset;

    float Current_Max;

    Enum_Motor_RMD_Status Motor_RMD_Status = Motor_RMD_Status_ENABLE;

	Enum_Motor_RMD_Control_Method Motor_RMD_Control_Method;
    // 内部变量
    uint32_t Flag;
    uint32_t Pre_Flag;
    float Out;

    // 读变量
     int8_t  Now_Temperature  = 0;

        // 解析转矩电流
     float Now_Iq = 0.0f;

        // 解析电机速度
     float Now_Omega =0.0f;

        // 解析电机角度
     float Now_Angle = 0.0f; 
	//读写变量
	float Now_Encoder_angle_deg = 0.0f;
	
	int32_t Target_angle = 0;
     
	uint16_t Target_speed  = 500; //无误改成1000
    

    // 内部函数
    // void Data_Process();
    void PID_Calculate();
    void Output();
    void Data_Process();
    void Get_Status();

};
inline void Class_Motor_RMD::Set_Target_angle(int32_t __angle)
{
	Target_angle = __angle;
}

inline void Class_Motor_RMD::Set_Target_speed(int32_t __speed)
{
	Target_speed = __speed;
}

inline void Class_Motor_RMD::Set_Status(Enum_Motor_RMD_Status __status)
{
	Motor_RMD_Status = __status;
}
#endif

/************************ COPYRIGHT **************************/
