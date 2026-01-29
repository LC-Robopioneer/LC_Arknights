#include "task.h"

//HAL初始化在自动生成的main里面
//各种Init hcan1句柄

extern M3508_Motor M3508_Motor_1;


//应用层初始化

int16_t time=0;
int16_t current=1;//-16384 —  +16384  +-20A  
int temp=0,a=0;

//还有一个使能24V
void Task_Loop(void)
{
	  //+-5A
	  //写一个呼吸灯
	 temp=0;
	 if(M3508_Motor_1.emergency_stop!=1)
	{
		   for(int i=500;i<=5000;i+=50)
		{
			M3508_Set_Current(&hcan1,&M3508_Motor_1,i,0);
			Delay_ms(10);
		}
	  for(int i=5000;i>=500;i-=50)
		{
			M3508_Set_Current(&hcan1,&M3508_Motor_1,i,0);
			Delay_ms(10);
		}	
	 }
//	 else//5000ms 后解除
//	 {
//		//HAL_Delay(5000); 
//		M3508_Resume_From_Stop(&hcan1,&M3508_Motor_1,0);
//	 }
//	 if(temp>=5)//紧急停止模拟 5个周期后紧急停止  
//		{
//			M3508_Emergency_Stop(&hcan1,&M3508_Motor_1,0);
//			temp=0;
//		}
}


	
