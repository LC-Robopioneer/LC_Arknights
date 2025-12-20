#include "alg_serial.h"
uint8_t serial_speed_flag=0;
uint8_t serial_angle_flag=0;  
float target_speed = 0;
float target_angle = 0;
HAL_StatusTypeDef PID_ParseSerialPack(PID * pid_speed, PID * pid_angle,uint8_t *serial_pack)
{
	//格式：
	//eg:KP1.14%F\n   Kp0.1%f\n SE100%f\n AN100%f\n

    // 包头为'K
    if(serial_pack[0] != 'K' && serial_pack[0]!='S'&& serial_pack[0]!='A')
    {
        return HAL_ERROR;
    }

    
	
	uint8_t  * endptr0;
	float value_str= strtof((char*)&serial_pack[2], (char**)&endptr0);//字符转换为浮点数
	
 
    switch (serial_pack[1])
    {
	//速度环参数 大写
    case 'P':
        pid_speed->KP = value_str;
        break;
    
    case 'I':
        pid_speed->KI = value_str;
        break;
    
    case 'D':
        pid_speed->KD = value_str;
        break;
	//角度环参数 小写
	 case 'p':
        pid_angle->KP = value_str;
        break;
    
    case 'i':
        pid_angle->KI = value_str;
        break;
    
    case 'd':
        pid_angle->KD = value_str;
        break;
	
	//速度环与角度环目标值
	//速度环
    case 'E':
		target_speed = value_str;
		serial_speed_flag=1;
		//output_speed =PID_Caculate_Speed(pid_speed,Tx_Omega,target_speed);
	    //M3508_Set_Current(&hcan1,&M3508_Motor_1,output_speed,0);
		break;
	//角度环
	case 'N':
		target_angle  = value_str;
		serial_angle_flag=1;
		//output_angle=PID_Caculate_Angle(pid_angle,Tx_Encoder,target_angle);
		//M3508_Set_Current(&hcan1,&M3508_Motor_1,output_angle,0);
		break;
    default:
        return HAL_ERROR;
    }

    return HAL_OK;
}
