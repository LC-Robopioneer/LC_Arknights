/**
 * @file dvc_motor_rmd.c
 * @brief RMD电机配置与操作
 * @version 0.1
 * @date 2024-10-07
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_motor_rmd.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/


// 定义数据范围
#define P_DES_MIN -12.5f
#define P_DES_MAX 12.5f
#define V_DES_MIN -45.0f
#define V_DES_MAX 45.0f
#define T_FF_MIN -24.0f
#define T_FF_MAX 24.0f
#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 100.0f

// 约束函数
float constrain(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}


/**
 * @brief 电机初始化
 *
 * @param hfdcan 绑定的CAN总线
 * @param __FDCAN_Rx_ID 绑定的CAN ID
 * @param __Motor_RMD_Control_Method 电机控制方式, 默认角度
 * @param __Encoder_Offset 编码器偏移, 默认0
 */
void Class_Motor_RMD::Init(FDCAN_HandleTypeDef *hfdcan, Enum_Motor_RMD_ID __FDCAN_ID, Enum_Motor_RMD_Control_Method __Motor_RMD_Control_Method, int32_t __Encoder_Offset)
{
    if (hfdcan->Instance == FDCAN1)
    {
        FDCAN_Manage_Object = &FDCAN1_Manage_Object;
    }
    else if (hfdcan->Instance == FDCAN2)
    {
        FDCAN_Manage_Object = &FDCAN2_Manage_Object;
    }
    else if (hfdcan->Instance == FDCAN3)
    {
        FDCAN_Manage_Object = &FDCAN3_Manage_Object;
    }
    FDCAN_ID = __FDCAN_ID;
    Motor_RMD_Control_Method = __Motor_RMD_Control_Method;
    Encoder_Offset = __Encoder_Offset;

}

/**
 * @brief FDCAN通信接收回调函数
 *
 * @param Rx_Data 接收的数据
 */
void Class_Motor_RMD::FDCAN_RxCpltCallback(uint8_t *Rx_Data)
{
    Flag += 1;
    Data_Process();
}

/**
 * @brief TIM定时器中断定期检测电机是否存活
 *
 */
void Class_Motor_RMD::TIM_100ms_Alive_PeriodElapsedCallback()
{	
	Get_Status();
	Pre_Flag = Flag;

//    if (Flag == Pre_Flag)
//    {
//        Motor_RMD_Status = Motor_RMD_Status_DISABLE;

//    }
//    else
//    {
//        Motor_RMD_Status = Motor_RMD_Status_ENABLE;
//    }

}
/**
 * @brief TIM定时器中断计算回调函数, 计算周期取决于电机反馈周期
 *
 */
void Class_Motor_RMD::TIM_2ms_Enable_PeriodElapsedCallback()
{

    Output();
	//Get_Now_Angle();

}

/**
 * @brief 计算PID
 *
 */
void Class_Motor_RMD::PID_Calculate()
{
 
}

/**
 * @brief 电机数据输出到CAN总线发送缓冲区
 *
 */
void Class_Motor_RMD::Output()
{  
	
	
	if(Motor_RMD_Status == Motor_RMD_Status_ENABLE){
		
	position_control_command1(Target_angle,Target_speed);
		
	}
	
	if(Motor_RMD_Status == Motor_RMD_Status_DISABLE){
	
		Disable_Motor();
	}
		

}

void Class_Motor_RMD::Get_Status()
{	

}
void Class_Motor_RMD::position_control_command1(int32_t actual_angle, uint16_t maxSpeed) {
	
	uint8_t FDCAN_Tx_Buffer[8] ={0}; //临时发送数组
	int32_t angleControl;
	uint32_t tmp_id ;

	
	tmp_id = FDCAN_ID+0x140;
	angleControl = actual_angle*900;
	
	FDCAN_Tx_Buffer[0] = 0xA4; // 命令字节
    FDCAN_Tx_Buffer[1] = 0x00; // NULL

    // 拷贝速度限制 maxSpeed（2字节，uint16_t）
    FDCAN_Tx_Buffer[2] = (uint8_t)(maxSpeed & 0xFF);         // 低字节
    FDCAN_Tx_Buffer[3] = (uint8_t)((maxSpeed >> 8) & 0xFF);  // 高字节

    // 拷贝位置控制 angleControl（4字节，int32_t，小端序）
    FDCAN_Tx_Buffer[4] = (uint8_t)(angleControl & 0xFF);
    FDCAN_Tx_Buffer[5] = (uint8_t)((angleControl >> 8) & 0xFF);
    FDCAN_Tx_Buffer[6] = (uint8_t)((angleControl >> 16) & 0xFF);
    FDCAN_Tx_Buffer[7] = (uint8_t)((angleControl >> 24) & 0xFF);
	     
	FDCAN_Send_Data(FDCAN_Manage_Object->FDCAN_Handler, tmp_id, FDCAN_Tx_Buffer, FDCAN_ID_Standard);

    
}

void	Class_Motor_RMD::Disable_Motor(){
uint8_t FDCAN_Tx_Buffer[8] ={0}; //临时发送数组
	uint32_t tmp_id ;

	
	tmp_id = FDCAN_ID+0x140;
	
	FDCAN_Tx_Buffer[0] = 0x80; // 命令字节     
	FDCAN_Send_Data(FDCAN_Manage_Object->FDCAN_Handler, tmp_id, FDCAN_Tx_Buffer, FDCAN_ID_Standard);




}
	
void Class_Motor_RMD::Get_Now_Angle() {
	

	uint8_t FDCAN_Tx_Buffer[8] ={0};

	uint32_t tmp_id;
	tmp_id = FDCAN_ID+0x140;
	FDCAN_Tx_Buffer[0]=0x92;
    //发送数据
     FDCAN_Send_Data(FDCAN_Manage_Object->FDCAN_Handler, tmp_id, FDCAN_Tx_Buffer, FDCAN_ID_Standard);		   

	//设置临时变量
}

void Class_Motor_RMD::Data_Process()
{	 
    // 定义一个临时缓冲区
    uint8_t tmp_buffer[8];

    // 将数据复制到临时缓冲区
    for (int i = 0; i < 8; i++) {
        tmp_buffer[i] = FDCAN_Manage_Object->Rx_Buffer.Data[i];
    }
    switch(tmp_buffer[0]){
	    case(0xA4):{
		if (tmp_buffer[0] == 0xA4){  // 命令字节校验，可选防错

    // 电机温度（int8_t）
		Now_Temperature = (int8_t)tmp_buffer[1];

    // 转矩电流 iq（int16_t，小端）
		Now_Iq = (int16_t)((tmp_buffer[3] << 8) | tmp_buffer[2]);

    // 电机转速 speed（int16_t，小端）
		Now_Omega = ((int16_t)(tmp_buffer[5] << 8) | tmp_buffer[4]);

    // 编码器位置 encoder（uint16_t，小端）仅在单圈，仅对应360/9=40°。
		uint16_t encoder_raw = ((uint16_t)tmp_buffer[7] << 8) | tmp_buffer[6];
			
		Now_Encoder_angle_deg = (encoder_raw / 16383.0f) * 360.0f;
		
		break;}
		}
		case(0x92):{
		int64_t motorAngle;
//		tmp_angle = ((uint16_t)tmp_buffer[7] << 8) | tmp_buffer[6];
//		Now_Angle = 0.01f * tmp_angle;
	     for (int i = 1; i <= 7; i++) {
		   motorAngle |= ((int64_t)tmp_buffer[i] << (8 * (i - 1)));
	     }
   
    // 转换为float，单位为度
		Now_Angle = motorAngle * 0.01f;
		break;}
    }
}
