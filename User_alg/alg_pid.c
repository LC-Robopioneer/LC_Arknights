#include "alg_pid.h"

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
float PID_Caculate(PID * pid,float current,float target)
{
	
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
//速度环  输入目标转速 当前转速  通过输出合适电流调控到目标转速
float PID_Caculate_Speed(PID * pid,float current_speed,float target_speed)
{
	float output_current;//输出为电流
	output_current=PID_Caculate (pid,current_speed,target_speed);
	return output_current;
}
//角度环		输入目标角度，当前角度，通过输出合适的电流调控到目标角度 
float PID_Caculate_Angle(PID * pid,float current_angle,float target_angle)
{
	float output_current;//输出为电流
	output_current=PID_Caculate (pid,current_angle,target_angle);//选择PID_Caculate or PID_Caculate_Speed
	return output_current;
	
}


