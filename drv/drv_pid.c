#include "drv_pid.h"

void PID_Init(PID * pid,float KP,float KI,float KD,float out_max,float out_min)
{
	pid->KP=KP;
	pid->KI=KI;
	pid->KD=KD;
	pid->out_max=out_max;
	pid->out_min=out_min;
	pid->pre_error=0;
	pid->errorint=0;
}
float PID_Caculate(PID * pid,float current,float target)//传入的是电流单位是A  传出的电流单位是物理单位 函数里面统一转为物理单位计算
{
	target=(target/20.0f*16384.0f);//+-20转换为+-16384
	float output;
	float error=target-current;//计算每一次的误差
	pid->errorint+=error;//*time;//积分
	float der=(error-pid->pre_error);//time;//微分 斜率
	
	//输出限制 积分
	if (pid->errorint >= pid->out_max) {pid->errorint = pid->out_max;}
    if (pid->errorint <=pid->out_min) {pid->errorint = pid->out_min;}
	
	output=pid->KP*(error)+pid->KI*(pid->errorint)+pid->KD*(der);
	
	//输出限制 output值
	if (output >=pid->out_max) output = pid->out_max;
    if (output <=pid->out_min) output = pid->out_min;

	pid->pre_error=error;

	return output;
}

