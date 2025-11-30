#include "alg_pid.h"

float Class_PID::Calculate(float target, float feedback)
{
    float error = target - feedback;

    // 积分项计算
    Integral += error * dt;
    // 积分限幅
    if (Integral > Integral_Limit)
        Integral = Integral_Limit;
    else if (Integral < -Integral_Limit)
        Integral = -Integral_Limit;

    // 微分项计算
    float derivative = (error - Previous_Error) / dt;

    // PID输出计算
    float output = Kp * error + Ki * Integral + Kd * derivative;

    // 输出限幅
    if (output > Output_Limit)
        output = Output_Limit;
    else if (output < -Output_Limit)
        output = -Output_Limit;

    // 更新上次误差
    Previous_Error = error;

    return output;
}

void Class_PID::Reset()
{
    Previous_Error = 0.0f;
    Integral = 0.0f;
}
