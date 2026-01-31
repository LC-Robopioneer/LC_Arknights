#include "alg_pid.h"

void Class_PID::Init(float __kp, float __ki, float __kd, float __kf,float __dt, float __integral_limit, float __output_limit,float __deadzone)
{
	Kp = __kp;
	Ki = __ki;
	Kd = __kd;
	dt = __dt;
    Kf = __kf;
	Integral_Limit = __integral_limit;
	Output_Limit = __output_limit;
    Previous_Target = 0.0f;
    Previous_Error = 0.0f;
    Integral = 0.0f;
    deadzone = __deadzone;
    
}

float Class_PID::Calculate(float target, float feedback)
{
    float error = target - feedback;
    
	if (error <= deadzone && error >= -deadzone)
	{
		error = 0;
        Integral = 0;
		return 0;
	}
	
    // 积分项计算
    Integral += error * dt;
    // 积分限幅
    if (Integral > Integral_Limit)
        Integral = Integral_Limit;
    else if (Integral < -Integral_Limit)
        Integral = -Integral_Limit;

    // 微分项计算
    float derivative = (error - Previous_Error) / dt;

    float feedforward = Kf * (target - Previous_Target) / dt;
    // PID输出计算
    float output = Kp * error + Ki * Integral + Kd * derivative + feedforward;

    // 输出限幅
    if (output > Output_Limit)
        output = Output_Limit;
    else if (output < -Output_Limit)
        output = -Output_Limit;

    // 更新上次误差
    Previous_Error = error;
    Previous_Target = target;
    return output;
}

void Class_PID::Reset()
{
    Previous_Error = 0.0f;
    Integral = 0.0f;
}
