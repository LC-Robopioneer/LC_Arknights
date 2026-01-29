#include "M3508_drv.h"


void Security_Check(HAL_StatusTypeDef  Staus)
{
	if(Staus!=HAL_OK)
	{
		Error_Handler();
	}
	
}
void M3508_Init(M3508_Motor *Motor)
{
	//参数有效性检查
	if(Motor==NULL)//传入空指针直接返回
	{
		return ;
	}
	
	//初始化控制参数
	Motor->current_max=16384;//电流限制值 ，可以完善为用户传入
	Motor->current_min=-16384;
	Motor->emergency_stop=0;//紧急停止标志位
	Motor->is_online=0;    //是否收到反馈标志位
	Motor->last_update_time=0;//最后更新时间清零
	Motor->Motor_State=MOTOR_STOP;
	Motor->target_current=0;
	
}
void M3508_Set_Current(CAN_HandleTypeDef *hcan,M3508_Motor *Motor,int16_t current,uint8_t motor_position)
{
	//参数有效性检查
	if(Motor==NULL)//传入空指针直接返回
	{
		return ;
	}
	//电流限幅
	if(current>=Motor->current_max)
	{
		current=Motor->current_max;//可加警告完善 ERROR_Handler里面可以设置状态判断是哪一种警告然后做出相应处理
	}
	if(current<=Motor->current_min)
	{
		current=Motor->current_min;
	}
	
	Motor->target_current=current;
	//Security_Check(CAN_Send_Data(hcan,motor_position,Motor->target_current));
	CAN_Send_Data(hcan,motor_position,Motor->target_current);//谁传，传给谁，传多少	
}
void M3508_Set_Current_PID(CAN_HandleTypeDef *hcan,M3508_Motor *Motor,PID * pid ,int16_t target_current,
int16_t now_current,uint8_t motor_position)
{
	//参数有效性检查
	if(Motor==NULL)//传入空指针直接返回
	{
		return ;
	}
	//电流限幅
	if(target_current>=Motor->current_max)
	{
		target_current=Motor->current_max;//可加警告完善 ERROR_Handler里面可以设置状态判断是哪一种警告然后做出相应处理
	}
	if(target_current<=Motor->current_min)
	{
		target_current=Motor->current_min;
	}
	Motor->target_current=PID_Caculate(pid,now_current ,target_current );
	Security_Check(CAN_Send_Data(hcan,motor_position,Motor->target_current));	
}
void M3508_Stop(CAN_HandleTypeDef *hcan,M3508_Motor *Motor,uint8_t motor_position)
{
	//参数有效性检查
	if(Motor==NULL)//传入空指针直接返回
	{
		return ;
	}
	Motor->target_current=0;
	Motor->Motor_State=MOTOR_STOP;
	Security_Check(CAN_Send_Data(hcan,motor_position,Motor->target_current));
	//CAN_Send_Data(hcan,motor_position,Motor->target_current);
	
}
void M3508_Emergency_Stop(CAN_HandleTypeDef *hcan,M3508_Motor *Motor,uint8_t motor_position)
{
	if(Motor==NULL)//传入空指针直接返回
	{
		return ;
	}
	Motor->target_current=0;
	Motor->emergency_stop=1;
	Motor->Motor_State=MOTOR_STOP;
	Security_Check(CAN_Send_Data(hcan,motor_position,Motor->target_current));
	//CAN_Send_Data(hcan,motor_position,Motor->target_current);
}
void M3508_Resume_From_Stop(CAN_HandleTypeDef *hcan,M3508_Motor *Motor,uint8_t motor_position)
{
	if(Motor==NULL)//传入空指针直接返回
	{
		return ;
	}
	Motor->emergency_stop=0;
}
void M3508_Update_State(CAN_HandleTypeDef *hcan,M3508_Motor *Motor,uint8_t motor_position)
{
}

