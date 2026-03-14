/**
 * @file crt_chassis.cpp
 * @author hzy by Lucy (2478427315@qq.com)
 * @brief 舵轮底盘电控
 * @version 0.1
 * @date 2026-01-18 
 *
 * @copyright Robopioneer (c) 2025-2026
 *
 */

/**
 * @brief 轮组编号
 * 1[0] 4[3]
 * 2[1] 3[2]
 * 前x左y上z
 */


/* Includes ------------------------------------------------------------------*/

#include "crt_chassis.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 舵向光电门初始化
 *
 */
void Class_PMT45_Steer::Init_Steer(GPIO_TypeDef *__GPIOx, uint16_t __GPIO_Pin, uint8_t __Motor_ID)
{
    // 设置GPIO端口号
    GPIO_Pin = __GPIO_Pin;
    // 设置电机ID
    Motor_ID = __Motor_ID;
}

/**
 * @brief 舵向光电门回调函数定义
 *
 */
void Class_PMT45_Steer::PMT45_EXTI_Callback_Steer()
{
    Start_TimeMs = DWT_GetCurrentTimeMs();
    //防止上电瞬间引起的电平变化触发中断
    if (Start_TimeMs > 1000)
    {
        Now_Time = HAL_GetTick();
        // 计算时间间隔
        Duration = Now_Time - Last_Time;

        if (Duration < Duration_Limit)
        {
            Valid_Count++;

            if (Valid_Count >= Valid_Count_Limit)
            {
                // 光电门更新角度
                Chassis->Steer_Angle_Set(Motor_ID);
                // 触发后清零计数器
                Valid_Count = 0;
            }
        }
        else
        {
            // 不满足条件时清零计数器
            Valid_Count = 0;
        }

        // 每次中断都更新 Last_Time
        Last_Time = Now_Time;
    }
}

/**
 * @brief 底盘初始化
 *
 */
void Class_Chassis::Init(float __Velocity_X_Max, float __Velocity_Y_Max, float __Omega_Max)
{
	
    Velocity_X_Max = __Velocity_X_Max;
    Velocity_Y_Max = __Velocity_Y_Max;
    Omega_Max = __Omega_Max;
    // PID初始化

    // 底盘速度xPID, 输出摩擦力
    PID_Velocity_X.Init(10.0f, 0.0f, 0.0f, 0.0f, 0.18f, 30.0f, 0.002f);

    // 底盘速度yPID, 输出摩擦力
    PID_Velocity_Y.Init(10.0f, 0.0f, 0.0f, 0.0f, 0.18f, 30.0f, 0.002f);

    // 底盘角速度PID, 输出扭矩
    PID_Omega.Init(10.0f, 0.0f, 0.0f, 0.0f, 0.01f, 10.0f, 0.002f);

    // 光电门初始化 待初始化修改
    for (int i = 0; i < 4; i++)
    {
        Steer_PMT45[i].Chassis = this;
    }

    // GPIOpin号待修改
    Steer_PMT45[0].Init_Steer(GPIOC, GPIO_PIN_10, 0);
    Steer_PMT45[1].Init_Steer(GPIOC, GPIO_PIN_11, 1);
    Steer_PMT45[2].Init_Steer(GPIOC, GPIO_PIN_12, 2);
    Steer_PMT45[3].Init_Steer(GPIOE, GPIO_PIN_14, 3);


    // 定位器初始化
    //OPS9.Init(&huart7);



    // 舵向电机初始化   使用2006电机角度环 实际角度设为校准后的角度

    // 电机初始化
    Motor_Steer[0].Init(&hfdcan1, Motor_DJI_ID_0x201, Motor_DJI_Control_Method_OMEGA);
    Motor_Steer[0].PID_Angle.Init(23.0f, 0.0f, 0.0f, 0.0f, 0.0f, 60.0f, 0.002f, 0.0f, 0.0f, 0.0f, 0.0f, PID_D_First_DISABLE);
    Motor_Steer[0].PID_Omega.Init(1.3f, 0.0f, 0.0f, 0.0f, 3.0f, 6.0f, 0.002f);
    
    Motor_Steer[1].Init(&hfdcan1, Motor_DJI_ID_0x202, Motor_DJI_Control_Method_OMEGA);
    Motor_Steer[1].PID_Angle.Init(23.0f, 0.0f, 0.0f, 0.0f, 0.0f, 60.0f, 0.002f, 0.0f, 0.0f, 0.0f, 0.0f, PID_D_First_DISABLE);
    Motor_Steer[1].PID_Omega.Init(1.3f, 0.0f, 0.0f, 0.0f, 3.0f, 6.0f, 0.002f);
    
    Motor_Steer[2].Init(&hfdcan1, Motor_DJI_ID_0x203, Motor_DJI_Control_Method_OMEGA);
    Motor_Steer[2].PID_Angle.Init(23.0f, 0.0f, 0.0f, 0.0f, 0.0f, 60.0f, 0.002f, 0.0f, 0.0f, 0.0f, 0.0f, PID_D_First_DISABLE);
    Motor_Steer[2].PID_Omega.Init(1.3f, 0.0f, 0.0f, 0.0f, 3.0f, 6.0f, 0.002f);
    
    
    Motor_Steer[3].Init(&hfdcan1, Motor_DJI_ID_0x204, Motor_DJI_Control_Method_OMEGA);
    Motor_Steer[3].PID_Angle.Init(23.0f, 0.0f, 0.0f, 0.0f, 0.0f, 60.0f, 0.002f, 0.0f, 0.0f, 0.0f, 0.0f, PID_D_First_DISABLE);
    Motor_Steer[3].PID_Omega.Init(1.3f, 0.0f, 0.0f, 0.0f, 3.0f, 6.0f, 0.002f);
    


    // 轮向电机初始化

    // MKSESC电调为拓展CANID故只能单独开出一条CAN通道进行使用
    // 电机初始化
    Motor_Wheel[0].Init(&hfdcan2, 0x031, 7, 6.2831, 50, 50, 30, Motor_MKSESC_Control_Method_Current);
    Motor_Wheel[1].Init(&hfdcan2, 0x032, 7, 6.2831, 200, 50, 30, Motor_MKSESC_Control_Method_Current);
    Motor_Wheel[2].Init(&hfdcan2, 0x033, 7, 6.2831, 50, 50, 30, Motor_MKSESC_Control_Method_Current);
    Motor_Wheel[3].Init(&hfdcan2, 0x034, 7, 6.2831, 50, 50, 30, Motor_MKSESC_Control_Method_Current);

}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Chassis::TIM_100ms_Alive_PeriodElapsedCallback()
{
    for (int i = 0; i < 4; i++)
    {
        Motor_Steer[i].TIM_100ms_Alive_PeriodElapsedCallback();
        Motor_Wheel[i].TIM_100ms_Alive_PeriodElapsedCallback();
    }

}
/**
 * @brief TIM定时器中断控制回调函数
 *
 */
void Class_Chassis::TIM_2ms_Control_PeriodElapsedCallback()
{   
    //自身解算
    Self_Resolution();

    // 运动学逆解算，解算出转向电机的角速度和舵向电机的角度
    Kinematics_Inverse_Resolution();

    Output_To_Dynamics();

    Dynamics_Inverse_Resolution();

    Output_To_Motor();
}

/**
 * @brief 舵向电机角度设置函数
 * @param __Steer_Motor_ID 舵向电机ID(0-3)
 * @details 用于舵向电机的标定过程
 *          当电机未标定时,记录当前角度作为标定误差值
 *          标定完成后电机停止转动
 *          当所有电机都标定完成后,设置整体标定完成标志位
 *          标定方向：以底盘中心为起点，朝向各个舵向电机的径向方向
 */
void Class_Chassis::Steer_Angle_Set(uint8_t __Steer_Motor_ID)
{

    // 添加边界检查
    uint16_t id_cnt_max = sizeof(Steer_Calibration_Error)/sizeof(Steer_Calibration_Error[0]);
    if(__Steer_Motor_ID >= id_cnt_max) 
        return;
    
    if(!Steer_Calibration_Status[__Steer_Motor_ID])
	{	
        // 记录角度
		Steer_Calibration_Error[__Steer_Motor_ID] = Motor_Steer[__Steer_Motor_ID].Get_Now_Angle();
		//设置标定标志位
		Steer_Calibration_Status[__Steer_Motor_ID] = Chassis_Steer_Calibration_Type_CALIBRATED;  
    }     
    
    // 判断是否全部标定完成
    bool all_calibrated =   Steer_Calibration_Status[0] == Chassis_Steer_Calibration_Type_CALIBRATED &&
                            Steer_Calibration_Status[1] == Chassis_Steer_Calibration_Type_CALIBRATED &&
                            Steer_Calibration_Status[2] == Chassis_Steer_Calibration_Type_CALIBRATED &&
                            Steer_Calibration_Status[3] == Chassis_Steer_Calibration_Type_CALIBRATED;

    // 每进入一次中断便判断是否全部初始化
    if(Chassis_Control_Type == Chassis_Control_Type_UNCALIBRATED && all_calibrated)
    {
        Chassis_Control_Type = Chassis_Control_Type_NORMAL;
    }
    
}

/**
 * @brief 自身解算
 *
 */
void Class_Chassis::Self_Resolution()
{
    // 使用临时变量计算新速度
    float tmp_velocity_x = 0.0f;
    float tmp_velocity_y = 0.0f;
    float tmp_omega = 0.0f;

    for (int i = 0; i < 4; i++)
    {
        // 待验证是否正确
        tmp_velocity_x += (Motor_Wheel[i].Get_Now_Omega() / Wheel_Motor_Reduction * arm_cos_f32(Now_Steer_Angle[i]) * Wheel_Radius) / 4.0f;
        tmp_velocity_y += (Motor_Wheel[i].Get_Now_Omega() / Wheel_Motor_Reduction * arm_sin_f32(Now_Steer_Angle[i]) * Wheel_Radius) / 4.0f;
        tmp_omega += (Motor_Wheel[i].Get_Now_Omega() / Wheel_Motor_Reduction * arm_sin_f32(Now_Steer_Angle[i] - Steer_Azimuth[i]) * Wheel_Radius / Wheel_To_Core_Distance[i]) / 4.0f;
    }

    // 更新类成员变量（可在此处加入滤波）
    Now_Velocity_X = tmp_velocity_x;
    Now_Velocity_Y = tmp_velocity_y;
    Now_Omega = tmp_omega;
		
		Steer_Angle_Self_Resolution();

    // 解算自身Yaw轴角度
   // Angle_Yaw = OPS9.Get_Position_Angle();
}

/**
 * @brief 获取舵向电机角度
 *
 */
void Class_Chassis::Steer_Angle_Self_Resolution()
{
    for(int i = 0; i < 4; i++)
    {
        float tmp_angle;

        // 计算角度
        tmp_angle =  Motor_Steer[i].Get_Now_Angle() - Steer_Calibration_Error[i] + Steer_Azimuth[i]*Steer_Motor_Reduction;

        Now_Steer_Angle[i] = fmod(tmp_angle, (Steer_Motor_Reduction * 2.0f * PI)) / Steer_Motor_Reduction ;

        Now_Steer_Angle[i] = Math_Modulus_Normalization(Now_Steer_Angle[i], 2.0f * PI);
    }
}

/**
 * @brief 运动学逆解算
 *
 */
void Class_Chassis::Kinematics_Inverse_Resolution()
{
    for (int i = 0; i < 4; i++)
    {
        float tmp_velocity_x, tmp_velocity_y, tmp_velocity_modulus;

        // 解算到每个轮组的具体线速度
        tmp_velocity_x = Target_Velocity_X - Target_Omega * Wheel_To_Core_Distance[i] * arm_sin_f32(Steer_Azimuth[i]);
        tmp_velocity_y = Target_Velocity_Y + Target_Omega * Wheel_To_Core_Distance[i] * arm_cos_f32(Steer_Azimuth[i]);
        arm_sqrt_f32(tmp_velocity_x * tmp_velocity_x + tmp_velocity_y * tmp_velocity_y, &tmp_velocity_modulus);
    
        // 根据线速度决定轮向电机角速度
        Target_Wheel_Omega[i] = tmp_velocity_modulus / Wheel_Radius;

        // 根据速度的xy分量分别决定舵向电机角度
        if (tmp_velocity_modulus == 0.0f)
        {
            // 排除除零问题
            Target_Steer_Angle[i] = Now_Steer_Angle[i];
        }
        else
        {
            // 没有除零问题
            Target_Steer_Angle[i] = atan2f(tmp_velocity_y, tmp_velocity_x);
        }
    }

    _Steer_Motor_Kinematics_Nearest_Transposition();
}

/**
 * @brief 舵向电机依照轮向电机目标角速度就近转位
 *
 */
void Class_Chassis::_Steer_Motor_Kinematics_Nearest_Transposition()
{
    for (int i = 0; i < 4; i++)
    {
        float tmp_delta_angle = Math_Modulus_Normalization(Target_Steer_Angle[i] - Now_Steer_Angle[i], 2.0f * PI);

        // 根据转动角度范围决定是否需要就近转位
        if (-PI / 2.0f <= tmp_delta_angle && tmp_delta_angle <= PI / 2.0f)
        {
            // ±PI / 2之间无需反向就近转位
            Target_Steer_Angle[i] = tmp_delta_angle + Now_Steer_Angle[i];
        }
        else
        {
            // 需要反转扣圈情况
            Target_Steer_Angle[i] = Math_Modulus_Normalization(tmp_delta_angle + PI, 2.0f * PI) + Now_Steer_Angle[i];
            Target_Wheel_Omega[i] *= -1.0f;
        }
    }
}	

/**
 * @brief 输出到动力学状态
 *
 */
void Class_Chassis::Output_To_Dynamics()
{
    switch (Chassis_Control_Type)
    {
    // 未标定状态和失能状态下不进行控制
    case(Chassis_Control_Type_UNCALIBRATED):
        case (Chassis_Control_Type_DISABLE):
    {
        // 底盘失能
        for (int i = 0; i < 4; i++)
        {
            PID_Velocity_X.Set_Integral_Error(0.0f);
            PID_Velocity_Y.Set_Integral_Error(0.0f);
            PID_Omega.Set_Integral_Error(0.0f);
        }

        break;
    }
    case (Chassis_Control_Type_NORMAL):
    {

        PID_Velocity_X.Set_Target(Target_Velocity_X);
        PID_Velocity_X.Set_Now(Now_Velocity_X);
        PID_Velocity_X.TIM_Calculate_PeriodElapsedCallback();

        PID_Velocity_Y.Set_Target(Target_Velocity_Y);
        PID_Velocity_Y.Set_Now(Now_Velocity_Y);
        PID_Velocity_Y.TIM_Calculate_PeriodElapsedCallback();

        PID_Omega.Set_Target(Target_Omega);
        PID_Omega.Set_Now(Now_Omega);
        PID_Omega.TIM_Calculate_PeriodElapsedCallback();

        break;
    }
    }
}

/**
 * @brief 动力学逆解算
 *
 */
void Class_Chassis::Dynamics_Inverse_Resolution()
{
    float force_x, force_y, torque_omega;

    force_x = PID_Velocity_X.Get_Out();
    force_y = PID_Velocity_Y.Get_Out();
    torque_omega = PID_Omega.Get_Out();

    // 每个轮的扭力
    float tmp_force[4];
    for (int i = 0; i < 4; i++)
    {
        // 解算到每个轮组的具体摩擦力
        tmp_force[i] = force_x * arm_cos_f32(Now_Steer_Angle[i]) + force_y * arm_sin_f32(Now_Steer_Angle[i]) - torque_omega / Wheel_To_Core_Distance[i] * arm_sin_f32(Steer_Azimuth[i] - Now_Steer_Angle[i]);
    }
    for (int i = 0; i < 4; i++)
    {
        // 摩擦力转换至扭矩
				Target_Wheel_Current[i] = tmp_force[i] * Wheel_Radius + Wheel_Speed_Limit_Factor * (Target_Wheel_Omega[i] - Motor_Wheel[i].Get_Now_Omega());            
        
//            // 普通控制模式，应用原有的静摩擦和动摩擦前馈
//            if (Target_Wheel_Omega[i] < 40.0f && Target_Wheel_Omega[i] > 3.0f)
//            {
//                Target_Wheel_Current[i] += Static_Resistance_Wheel_Current[i];
//            }
//            else if (Target_Wheel_Omega[i] > -40.0f && Target_Wheel_Omega[i] < -3.0f)
//            {
//                Target_Wheel_Current[i] -= Static_Resistance_Wheel_Current[i];
//            }
            
            // 动摩擦阻力前馈
            if (Target_Wheel_Omega[i] > Wheel_Resistance_Omega_Threshold)
            {
                Target_Wheel_Current[i] += Dynamic_Resistance_Wheel_Current[i];
            }
            else if (Target_Wheel_Omega[i] < -Wheel_Resistance_Omega_Threshold)
            {
                Target_Wheel_Current[i] -= Dynamic_Resistance_Wheel_Current[i];
            }
            else
            {
                Target_Wheel_Current[i] += Motor_Wheel[i].Get_Now_Omega() / Wheel_Resistance_Omega_Threshold * Dynamic_Resistance_Wheel_Current[i];
            }
						
						// 低电流前馈控制模式
            if (Math_Abs(Target_Wheel_Current[i]) < Low_Current_Deadzone) 
								Target_Wheel_Current[i] = 0.0f;
						
            else if (Math_Abs(Target_Wheel_Current[i]) < Low_Current_Threshold)
            {
                // 如果电流小于阈值，添加前馈
                if (Target_Wheel_Current[i] > 0)
                {
                    Target_Wheel_Current[i] += Low_Current_Feedforward[i];
                }
                else if (Target_Wheel_Current[i] < 0)
                {
                    Target_Wheel_Current[i] -= Low_Current_Feedforward[i];
                }
            }
        
    }

    // 根据斜坡与压力进行电流限幅防止贴地打滑
    // TODO
}

/**
 * @brief 输出到电机
 *
 */
float omega_calibrate = 0.0f;
void Class_Chassis::Output_To_Motor()
{
    switch (Chassis_Control_Type)
    {
    case (Chassis_Control_Type_UNCALIBRATED):
    {
        for(int i = 0; i < 4;i++)
        {
        //对舵向电机单独校准
        if(!Steer_Calibration_Status[i])
        {
            Motor_Steer[i].Set_Control_Method(Motor_DJI_Control_Method_OMEGA);
            Motor_Steer[i].Set_Target_Omega(7.5f);
        }
        else
        {
            Motor_Steer[i].Set_Target_Omega(0.0f);
        }  
        }
                     
        break;
    }
    case (Chassis_Control_Type_DISABLE):
    {
        // 底盘失能
        for (int i = 0; i < 4; i++)
        {
            Motor_Steer[i].Set_Control_Method(Motor_DJI_Control_Method_CURRENT);
            Motor_Wheel[i].Set_Control_Method(Motor_MKSESC_Control_Method_Current);

            Motor_Steer[i].PID_Angle.Set_Integral_Error(0.0f);
            Motor_Steer[i].PID_Omega.Set_Integral_Error(0.0f);

            Motor_Steer[i].Set_Target_Current(0.0f);
            Motor_Wheel[i].Set_Control_Current(0.0f);
        }

        break;
    }
    case (Chassis_Control_Type_NORMAL):
    {
        // 舵轮模型
        for (int i = 0; i < 4; i++)
        {
            Motor_Steer[i].Set_Control_Method(Motor_DJI_Control_Method_ANGLE);
            Motor_Wheel[i].Set_Control_Method(Motor_MKSESC_Control_Method_Current);
        }

        for (int i = 0; i < 4; i++)
        {
            Motor_Steer[i].Set_Target_Angle(Target_Steer_Angle[i]);
            Motor_Steer[i].PID_Angle.Set_Now(Now_Steer_Angle[i]);
            

            if(Math_Abs(Target_Wheel_Current[i]) >= Wheel_Current_Limit)
            {
                Motor_Wheel[i].Set_Control_Current(Target_Wheel_Current[i]);
            }
            else
            {
                Motor_Wheel[i].Set_Control_Current(0.0f);
            }
        }

        break;
    }
    }
		for(int i = 0; i < 4; i++)
		{
			Motor_Steer[i].TIM_Calculate_PeriodElapsedCallback();
		}

    // 舵向电机数据发送
    FDCAN_Send_Data(&hfdcan1, 0x200, FDCAN1_0x200_Tx_Data);

    // 轮向电机数据发送
    for (int i = 0; i < 4; i++)
    {
        Motor_Wheel[i].TIM_Send_PeriodElapsedCallback();
    }
}
/************************ COPYRIGHT(C) ROBOPIONEER **************************/
