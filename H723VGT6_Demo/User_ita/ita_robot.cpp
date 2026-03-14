#include "ita_robot.h"
/**
 * @file ita_robot.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 人机交互控制逻辑
 * @version 1.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2024-01-17 1.1 更名为ita_robot.h, 引入新功能
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "ita_robot.h"
#include "dvc_dr16.h"
#include "drv_math.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
void Class_Chariot::Init(float __Dead_Zone)
{
    DR16.Init(&huart5);

    Chassis.Init();
    
    Dead_Zone = __Dead_Zone;


}

void Class_Chariot::TIM_100ms_Alive_PeriodElapsedCallback()
{
    DR16.TIM1msMod50_Alive_PeriodElapsedCallback();
	
    Chassis.TIM_100ms_Alive_PeriodElapsedCallback();
}

void Class_Chariot::TIM_Unline_Protect_PeriodElapsedCallback()
{
    if(DR16.Get_DR16_Status() == DR16_Status_DISABLE)
    {
        Chassis.Set_Chassis_Control_Type(Chassis_Control_Type_DISABLE);
    }
}

/**
 * @brief 定时器计算函数
 *
 */
void Class_Chariot::TIM_2ms_Calculate_PeriodElapsedCallback()
{
    Chassis.TIM_2ms_Control_PeriodElapsedCallback();

}
void Class_Chariot::Judge_DR16_Control_Type()
{
    if (DR16.Get_Left_X() != 0 ||
        DR16.Get_Left_Y() != 0 ||
        DR16.Get_Right_X() != 0 ||
        DR16.Get_Right_Y() != 0 ||
        DR16.Get_Yaw() != 0)
    {
        DR16_Control_Type = DR16_Control_Type_REMOTE;
    }
    else if (DR16.Get_Mouse_X() != 0 ||
             DR16.Get_Mouse_Y() != 0 ||
             DR16.Get_Mouse_Z() != 0 ||
             DR16.Get_Keyboard_Key_A() != 0 ||
             DR16.Get_Keyboard_Key_D() != 0 ||
             DR16.Get_Keyboard_Key_W() != 0 ||
             DR16.Get_Keyboard_Key_S() != 0)
    {
        DR16_Control_Type = DR16_Control_Type_KEYBOARD;
    }
    else{
        if (DR16.Get_DR16_Status() == DR16_Status_DISABLE)
            DR16_Control_Type = DR16_Control_Type_NONE;
    }
    
}

void Class_Chariot::Judge_Active_Controller()
{
    // 检查DR16是否有输入
    Judge_DR16_Control_Type();

    // 判断当前活动的控制器
    if (DR16_Control_Type != DR16_Control_Type_NONE)
    {
        Active_Controller = Controller_DR16;
    }
    else
    {
        Active_Controller = Controller_NONE;
    }
}
void Class_Chariot::_Chassis_Control()
{
    //遥控器摇杆值
    Judge_Active_Controller();
    //云台坐标系速度目标值 float
    float chassis_velocity_x = 0, chassis_velocity_y = 0;
    static float chassis_omega = 0;   
    static float chassis_angle = 0;  

	/************************************遥控器控制逻辑*********************************************/
    
    if (Active_Controller == Controller_DR16 && DR16_Control_Type == DR16_Control_Type_REMOTE)
    {
        float dr16_l_x, dr16_l_y, dr16_yaw;    
        //排除遥控器死区
        dr16_l_x = (Math_Abs(DR16.Get_Left_X()) > Dead_Zone) ? DR16.Get_Left_X() : 0;
        dr16_l_y = (Math_Abs(DR16.Get_Left_Y()) > Dead_Zone) ? DR16.Get_Left_Y() : 0;
        //yaw和xy的死区是否相同存疑
        dr16_yaw = (Math_Abs(DR16.Get_Yaw()) > Dead_Zone) ? DR16.Get_Yaw() : 0;
        //设定矩形到圆形映射进行控制
        chassis_velocity_x = dr16_l_y * sqrt(1.0f - dr16_l_x * dr16_l_x / 2.0f) * Chassis.Get_Velocity_X_Max() ;
        chassis_velocity_y = -dr16_l_x * sqrt(1.0f - dr16_l_y * dr16_l_y / 2.0f) * Chassis.Get_Velocity_Y_Max() ;
        chassis_omega = dr16_yaw * Chassis.Get_Omega_Max();
        chassis_angle += chassis_omega;
        //键盘遥控器操作逻辑
        if (DR16.Get_Right_Switch()==DR16_Switch_Status_UP)  //右上 随动模式
        {
            Chassis.Set_Chassis_Control_Type(Chassis_Control_Type_DISABLE);
        }
        else if (DR16.Get_Right_Switch()==DR16_Switch_Status_DOWN)  //右下 小陀螺模式
        {
            Chassis.Set_Chassis_Control_Type(Chassis_Control_Type_NORMAL);
            Chassis.Set_Target_Velocity_X(chassis_velocity_x);
            Chassis.Set_Target_Velocity_Y(chassis_velocity_y);
            
            Chassis.Set_Target_Omega(chassis_angle);
        }
        else if (DR16.Get_Right_Switch()==DR16_Switch_Status_MIDDLE)
        {
            //底盘随动
            Chassis.Set_Chassis_Control_Type(Chassis_Control_Type_NORMAL);
            Chassis.Set_Target_Velocity_X(chassis_velocity_x);
            Chassis.Set_Target_Velocity_Y(chassis_velocity_y);
            Chassis.Set_Target_Omega(chassis_omega); 
        }
    }
    
    
}
void Class_Chariot::TIM_Control_Callback()
{
    Judge_Active_Controller();

    //底盘，云台，发射机构控制逻辑
    _Chassis_Control();

}

