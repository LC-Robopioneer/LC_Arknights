#include "alg_serial.h"
HAL_StatusTypeDef PID_ParseSerialPack(PID * pid_speed, PID * pid_angle,uint8_t *serial_pack)
{
	//格式：
	//eg:KP1.14%F\n   Kp0.1%f\n SE100%f\n AN100%f\n

    // 包头为'K
    if(serial_pack[0] != 'K' && serial_pack[0]!='S'&& serial_pack[0]!='A')
    {
        return HAL_ERROR;
    }

    // 取出数值部分
    char *pack_ptr = (char *)&serial_pack[2];
    char value_str[20] = {0};
    uint8_t idx = 0;
    do
    {
        value_str[idx] = *pack_ptr;
        pack_ptr++;
    }
    while (*pack_ptr != '\n');
    

    float target_speed ;
	float target_angle ;
	float output_speed=0 ;
	float output_angle=0 ; 
    switch (serial_pack[1])
    {
	//速度环参数 大写
    case 'P':
        pid_speed->KP = atof(value_str);
        break;
    
    case 'I':
        pid_speed->KI = atof(value_str);
        break;
    
    case 'D':
        pid_speed->KD = atof(value_str);
        break;
	//角度环参数 小写
	 case 'p':
        pid_angle->KP = atof(value_str);
        break;
    
    case 'i':
        pid_angle->KI = atof(value_str);
        break;
    
    case 'd':
        pid_angle->KD = atof(value_str);
        break;
	
	//速度环与角度环目标值
	//速度环
    case 'E':
		target_speed = atof(value_str);
		output_speed =PID_Caculate_Speed(pid_speed,Tx_Omega,target_speed);
	    M3508_Set_Current(&hcan1,&M3508_Motor_1,output_speed,0);
		break;
	//角度环
	case 'N':
		target_angle  = atof(value_str);
		output_angle=PID_Caculate_Angle(pid_angle,Tx_Encoder,target_angle);
		M3508_Set_Current(&hcan1,&M3508_Motor_1,output_angle,0);
		break;
    default:
        return HAL_ERROR;
    }

    return HAL_OK;
}
