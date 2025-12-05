#include "vofa_usart.h"
#include "stdlib.h"
#include "stdio.h"
PID pid;
M3508_Motor M3508_Motor_1;
int16_t Rx_Encoder, Rx_Omega, Rx_Torque, Rx_Temperature;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static uint8_t RX_Data;//
	static uint8_t RX_PID_Index;
	static uint8_t RX_PID[128];
	
	static uint8_t RX_Curret_Index;//rx_data 8 位 current 16位
	static uint8_t RX_Current[128];
	static enum 
	{
		Wait_Head,//接收包头 K
		Wait_Flag_PID,//接收包头->接收标志 P I D
		Wait_Flag_Current,
		Wait_Data_PID,		//等待数据接收	数据包：KP%fM
		Wait_Data_Current   //数据包 ：CU%fN  
		
	}RX_State=Wait_Head;
	
	static enum
	{
		KP_Come,
		KI_Come,
		KD_Come,
		None
	}Current_State_PID=None;
	
//	static enum
//	{
//		New_Current,
//		Current_None
//	}RX_Current=Current_None;
	
	if(huart==&huart2)
	{
		switch(RX_State)
	 {
		case Wait_Head:
		{
			if(RX_Data=='K')
			{
				RX_State=Wait_Flag_PID;
			}
			if(RX_Data=='C')
			{
				RX_State=Wait_Flag_Current;
			}
			break;
		}
		case Wait_Flag_PID:
		{
			if(RX_Data=='P')
			{
				Current_State_PID=KP_Come;
				RX_PID_Index=0;
				RX_State=Wait_Data_PID;
				
			}
			if(RX_Data=='I')
			{
				Current_State_PID=KI_Come;
				RX_PID_Index=0;
				RX_State=Wait_Data_PID;
				
			}
			if(RX_Data=='D')
			{
				Current_State_PID=KD_Come;
				RX_PID_Index=0;
				RX_State=Wait_Data_PID;
				
			}
			break;
		}
		case Wait_Flag_Current :
		{
			if(RX_Data=='U')
			{
				RX_State=Wait_Data_Current;
				RX_Curret_Index =0;        
			}
			break;
		}
		case Wait_Data_PID:
		{
			if(RX_Data=='M')//接收到包尾
			{
				RX_PID[RX_PID_Index]='\0';
				uint8_t  * endptr0;
				float NewValue= strtof((char*)RX_PID, (char**)&endptr0);//字符转换为浮点数
				
				if(endptr0!=RX_PID && *endptr0 == '\0') 
			  {
					switch(Current_State_PID)
					{
						case KP_Come:
						{
							pid.KP=NewValue;
							printf("KP updated:%.2f\n",pid.KP);
							break;
						}
						case KI_Come:
						{
							pid.KI=NewValue;
							printf("KI updated:%.2f\n",pid.KI);
							break;
						}
						case KD_Come:
						{
							pid.KD=NewValue;
							printf("KD updated:%.2f\n",pid.KD);
							break;
						}
						case None:
							break;
					}
			  }
				RX_State=Wait_Head;
				Current_State_PID=None;//状态重置
			}
			else//接收到数值
			{
				if(RX_PID_Index<sizeof(RX_PID)-1)//判断数组是否越界
				{
					RX_PID[RX_PID_Index++]=RX_Data;//将接收到的数据存储至数组中
				}
				else
				{
					RX_State=Wait_Head;//溢出复位
				}
			}
			break;		
		}
		case Wait_Data_Current:
		{
			if(RX_Data=='N')//接收到包尾
			{
				RX_Current[RX_PID_Index]='\0';
				uint8_t * endptr1;
				float New_Target_Current;//输入：-20 - 20 要转换为+-16384
				int16_t New_Target_Current_16bit;
				New_Target_Current=strtof((char*)RX_Current ,(char**)&endptr1);
				if(endptr1!=RX_Current&&*endptr1!='\0')
				{
					New_Target_Current_16bit=(int16_t)(New_Target_Current*16384.0f/20000.0f);
					printf("New_Current:%d\n",New_Target_Current_16bit);
					//通过pid设置电流
					M3508_Set_Current_PID(&hcan1,&M3508_Motor_1,&pid ,New_Target_Current_16bit,
                   Rx_Omega,0);//固定can1 0位置电机的调节  可改进：发送电机ID current 指定调速
				}
				RX_State =Wait_Head ;
			}
			else//接收到电流数值
			{
				if(RX_Curret_Index <sizeof(RX_Current )-1)
				{
					RX_Current[RX_Curret_Index ++]=RX_Data;
				}
				else
				{
					RX_State=Wait_Head ;
				}
					
			}
			break;
		}
		default:
			RX_State=Wait_Head;
			break;	
	   }
	  HAL_UART_Receive_IT(&huart2,&RX_Data,1);//重启串口接收
	}
}
