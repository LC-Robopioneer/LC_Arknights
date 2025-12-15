#include "tsk_main.h"

M3508_Motor M3508_Motor_1;
PID pid_speed;
PID pid_angle;
int16_t isInit=0;

void Init_All(void)
{
	Serial_RxDMA_Init(&huart3);
	CAN_Init(&hcan1, CAN_FILTER(13) | CAN_FIFO_1 | CAN_STDID | CAN_DATA_TYPE, 0x201, 0, Motor_Call_Back);
    M3508_Init(&M3508_Motor_1);
    BSP_Init(BSP_DC24_LU_ON | BSP_DC24_LD_ON | BSP_DC24_RU_ON | BSP_DC24_RD_ON);
    PID_Init(&pid_speed, 0.1, 0.1, 0.1, 16384, -16384);
	PID_Init(&pid_angle, 0.1, 0.1, 0.1, 180, -180);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart3, serial_rx_packet, SERIAL_PACKET_SIZE);
}
void Task_Loop(void)
{
	if(isInit==0)
	{
		Init_All();
		isInit=1;
	}
	CAN_RX_Message_Get();
	
	if(serial_rx_flag == 1)
    {
        serial_rx_flag = 0;
        PID_ParseSerialPack(&pid_speed,&pid_angle, serial_rx_packet);
    }
	
}
