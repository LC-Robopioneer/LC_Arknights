#include "main0.h"

//HAL初始化在自动生成的main里面
//各种Init hcan1句柄

M3508_Motor M3508_Motor_1;

//应用层初始化

int16_t time=0;
int16_t current=1;//-16384 —  +16384  +-20A  
int temp=0;

void Task_Start(void)
{
	  //+-5A
	  //写一个呼吸灯
	 temp+=1;
	 if(M3508_Motor_1.emergency_stop!=1)
	 {
		   for(int i=0;i<=1000;i+=50)
		{
			M3508_Set_Current(&hcan1,&M3508_Motor_1,i,0);
			HAL_Delay(50);
		}
	  for(int i=1000;i>=0;i-=50)
		{
			M3508_Set_Current(&hcan1,&M3508_Motor_1,i,0);
			HAL_Delay(50);
		}	
	 }
	 else
	 {
		HAL_Delay(5000); 
		M3508_Resume_From_Stop(&hcan1,&M3508_Motor_1,0);
	 }
		
	 if(temp>=5)//紧急停止模拟 5个周期后紧急停止  
		{
			M3508_Emergency_Stop(&hcan1,&M3508_Motor_1,0);
			temp=0;
		}
}


	
