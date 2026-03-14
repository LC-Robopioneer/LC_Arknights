///**
// * @file alg_location.cpp
// * @author lucy
// * @brief 位置相关算法
// * @version 0.1
// * @date 2024-06-02 0.1 新建文件
// *
// * @copyright Robopioneer (c) 2025
// *
// */
///* Includes ------------------------------------------------------------------*/

//#include "alg_location.h"

///* Private macros ------------------------------------------------------------*/

///* Private types -------------------------------------------------------------*/

///* Private variables ---------------------------------------------------------*/

///* Private function declarations ---------------------------------------------*/

///* Function prototypes -------------------------------------------------------*/

///**
// * @brief 初始化
// *
// */
//void Class_Location::Init()
//{
//    // 初始化PID控制器
//    // 这里的PID参数需要根据实际情况调整
//    PID_X.Init(0.5f, 0.01f, 0.1f, 0.0f, 100.0f, 500.0f);
//    PID_Y.Init(0.5f, 0.01f, 0.1f, 0.0f, 100.0f, 500.0f);
//    PID_Yaw.Init(2.0f, 0.05f, 0.2f, 0.0f, 100.0f, 10.0f);
//    
//    // 初始化目标点和当前点
//    Target_X = 0.0f;
//    Target_Y = 0.0f;
//    Target_Yaw = 0.0f;
//    
//    Now_X = 0.0f;
//    Now_Y = 0.0f;
//    Now_Yaw = 0.0f;
//    
//    // 初始化输出
//    Chassis_Out_X = 0.0f;
//    Chassis_Out_Y = 0.0f;
//    Chassis_Out_Yaw = 0.0f;
//    
//    // 初始状态为到达
//    Location_Status = Location_Status_Arrive;
//}

///**
// * @brief 定时器调整状态, 计时周期1ms
// *
// */
//void Class_Location::TIM_1ms_Calculate_PeriodElapsedCallback()
//{
//    // 计算位置误差
//    float error_x = Target_X - Now_X;
//    float error_y = Target_Y - Now_Y;
//    float error_yaw = Target_Yaw - Now_Yaw;
//    
//    // 归一化Yaw角误差到[-PI, PI]
//    while (error_yaw > M_PI) error_yaw -= 2 * M_PI;
//    while (error_yaw < -M_PI) error_yaw += 2 * M_PI;
//    
//    // 设置PID目标和当前值
//    PID_X.Set_Target(Target_X);
//    PID_X.Set_Now(Now_X);
//    
//    PID_Y.Set_Target(Target_Y);
//    PID_Y.Set_Now(Now_Y);
//    
//    PID_Yaw.Set_Target(Target_Yaw);
//    PID_Yaw.Set_Now(Now_Yaw);
//    
//    // 计算PID输出
//    PID_X.TIM_Calculate_PeriodElapsedCallback();
//    PID_Y.TIM_Calculate_PeriodElapsedCallback();
//    PID_Yaw.TIM_Calculate_PeriodElapsedCallback();
//    
//    // 获取PID输出作为底盘控制量
//    Chassis_Out_X = PID_X.Get_Out();
//    Chassis_Out_Y = PID_Y.Get_Out();
//    Chassis_Out_Yaw = PID_Yaw.Get_Out();
//        // 判断是否到达目标点
//    float position_error = sqrtf(error_x * error_x + error_y * error_y);
//    
//    if (position_error < POSITION_TOLERANCE && fabsf(error_yaw) < ANGLE_TOLERANCE)
//    {
//        // 到达目标点，停止输出
//        Location_Status = Location_Status_Arrive;
//        Chassis_Out_X = 0.0f;
//        Chassis_Out_Y = 0.0f;
//        Chassis_Out_Yaw = 0.0f;
//    }
//    else
//    {
//        // 未到达目标点，继续导航
//        Location_Status = Location_Status_Go;
//    }
//}

///************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
