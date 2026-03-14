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

#ifndef  ITA_ROBOT_H
#define  ITA_ROBOT_H
 
/* Includes -----------------------------------------------------------------*/
#include "crt_chassis.h"
#include "dvc_dr16.h"
#include "dvc_motor_dji.h"


/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/**
 * @brief DR16控制数据来源
 *
 */
enum Enum_DR16_Control_Type
{
    DR16_Control_Type_REMOTE = 0,
    DR16_Control_Type_KEYBOARD,

    DR16_Control_Type_NONE,
};

enum Enum_Control_Source{
    DR16_Control,
    Control_DISABLE,
};

// 添加活动控制器枚举类型
enum Enum_Active_Controller
{
    Controller_NONE = 0,
    Controller_DR16,
};

class Class_Chariot
{
public:
		void Init(float _Dead_Zone);
    //遥控器
    Class_DR16 DR16;
    //全向轮底盘
    Class_Chassis Chassis;
    
    inline float Get_Chassis_Target_Angle_Yaw();

    void TIM_1000ms_Alive_PeriodElapsedCallback();

    void TIM_100ms_Alive_PeriodElapsedCallback();

    void TIM_100ms_Calculate_Callback();

    void TIM_10ms_Calculate_PeriodElapsedCallback();

    void TIM_2ms_Calculate_PeriodElapsedCallback();

    void TIM_1ms_Calculate_Callback();

    
    void Judge_DR16_Control_Type();
    void Judge_Active_Controller();
    void TIM_Control_Callback();
		void TIM_Unline_Protect_PeriodElapsedCallback();

protected:
    //DR16控制数据来源
    Enum_DR16_Control_Type DR16_Control_Type = DR16_Control_Type_NONE;

    
    // 当前活动的控制器
    Enum_Active_Controller Active_Controller = Controller_NONE;

    Enum_Control_Source Control_Source = Control_DISABLE; 

    //遥控器拨动的死区, 0~1
    float Dead_Zone;


		void _Status_Control();

    void _Chassis_Control();

    void _Shot_Control();
    
    void _Dribble_Control();
    
};
#endif
