///**
// * @file alg_location.h
// * @author lucy
// * @brief 位置相关算法
// * @version 0.1
// * @date 2024-06-02 0.1 新建文件
// *
// * @copyright Robopioneer (c) 2025
// *
// */

//#ifndef ALG_LOCATION_H
//#define ALG_LOCATION_H

///* Includes ------------------------------------------------------------------*/

//#include "alg_pid.h"

///* Exported macros -----------------------------------------------------------*/

///* Exported types ------------------------------------------------------------*/

///**
// * @brief 状态
// *
// */
//enum Enum_Location_Status
//{
//    Location_Status_Arrive = 0, // 到达
//    Location_Status_Go,         // 前进
//};

///**
// * @brief Reusable, 定时器本体, 一般以1ms为界
// *
// */
//class Class_Location
//{
//public:
//    // X轴PID
//    Class_PID PID_X;
//    // Y轴PID
//    Class_PID PID_Y;
//    // Yaw轴PID
//    Class_PID PID_Yaw;

//    void Init(uint32_t __Delay);

//    // 获取导航状态
//    inline Enum_Location_Status Get_Now_Status();

//    // 设置导航坐标点
//    inline void Set_Target_Point(float __Target_X, float __Target_Y, float __Target_Yaw);

//    // 设置当前坐标点
//    inline void Set_Now_Point(float __Now_X, float __Now_Y, float __Now_Yaw);

//    // 获取底盘X轴输出
//    inline float Get_Chassis_Out_X();

//    // 获取底盘Y轴输出
//    inline float Get_Chassis_Out_Y();

//    // 获取底盘Yaw轴输出
//    inline float Get_Chassis_Out_Yaw();
//    
//    void TIM_1ms_Calculate_PeriodElapsedCallback();

//protected:
//    // 初始化相关常量

//    // 常量

//    // 内部变量
//    Enum_Location_Status Location_Status = Location_Status_Arrive;

//    // 读变量

//    // 时钟计数

//    // 写变量

//    // 目标底盘速度
//    float Chassis_Out_X = 0.0f;
//    float Chassis_Out_Y = 0.0f;
//    float Chassis_Out_Yaw = 0.0f;

//    // 延迟时间单元数

//    // 读写变量

//    // 目标坐标
//    float Target_X = 0.0f;
//    float Target_Y = 0.0f;
//    float Target_Yaw = 0.0f;

//    // 当前坐标
//    float Now_X = 0.0f;
//    float Now_Y = 0.0f;
//    float Now_Yaw = 0.0f;

//    // 当前状态

//    // 内部函数
//};

///* Exported variables --------------------------------------------------------*/

///* Exported function declarations --------------------------------------------*/

//inline Enum_Location_Status  Class_Location::Get_Now_Status()
//{
//    return (Location_Status);
//}

//inline void Class_Location::Set_Target_Point(float __Target_X, float __Target_Y, float __Target_Yaw)
//{
//    Target_X = __Target_X;
//    Target_Y = __Target_Y;
//    Target_Yaw = __Target_Yaw;
//}

//inline void Class_Location::Set_Now_Point(float __Now_X, float __Now_Y, float __Now_Yaw)
//{
//    Now_X = __Now_X;
//    Now_Y = __Now_Y;
//    Now_Yaw = __Now_Yaw;
//}

//inline float Class_Location::Get_Chassis_Out_X()
//{
//    return(Chassis_Out_X);
//}

//inline float Class_Location::Get_Chassis_Out_Y()
//{
//    return(Chassis_Out_Y);
//}

//inline float Class_Location::Get_Chassis_Out_Yaw()
//{
//    return(Chassis_Out_Yaw);
//}

//#endif

///************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
