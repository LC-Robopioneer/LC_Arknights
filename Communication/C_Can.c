#include "C_Can.h"

//封装CAN初始化函数
void CAN_Init(CAN_HandleTypeDef *hcan)
{
	HAL_StatusTypeDef Status;//储存库函数返回状态 可以检查操作是否成功，后续完善
	Status = HAL_CAN_Start(&hcan1);
	assert_param(hcan != NULL);//检测关键传参
	
	if(Status!=HAL_OK)
	{
		Error_Handler();
	}
		
	CAN_Filter_Config(hcan);//配置滤波器
	
	Status=HAL_CAN_ActivateNotification(hcan,CAN_IT_RX_FIFO0_MSG_PENDING);//启用接收中断
	if(Status!=HAL_OK)
	{
		Error_Handler();
	}
	
}
void CAN_Filter_Config(CAN_HandleTypeDef *hcan)
{
	 CAN_FilterTypeDef can_filter;  // 过滤器配置结构体
    
    // 初始化过滤器结构体为零值，避免未定义行为
   // memset(&can_filter, 0, sizeof(can_filter));
    
    // 配置过滤器参数
    can_filter.FilterBank = 0;                       // 使用过滤器组0
    can_filter.FilterMode = CAN_FILTERMODE_IDMASK;   // 标识符屏蔽位模式
    can_filter.FilterScale = CAN_FILTERSCALE_32BIT;  // 32位尺度
    can_filter.FilterIdHigh = 0x0000;                // 过滤器ID高16位
    can_filter.FilterIdLow = 0x0000;                 // 过滤器ID低16位  
    can_filter.FilterMaskIdHigh = 0x0000;           // 屏蔽位高16位(0=不屏蔽)
    can_filter.FilterMaskIdLow = 0x0000;            // 屏蔽位低16位(0=不屏蔽)
    can_filter.FilterFIFOAssignment = CAN_RX_FIFO0; // 使用接收FIFO0
    can_filter.FilterActivation = ENABLE;           // 启用此过滤器
    can_filter.SlaveStartFilterBank = 14;            // 从过滤器组起始编号
    
    // 应用过滤器配置
    HAL_CAN_ConfigFilter(hcan, &can_filter);
}



HAL_StatusTypeDef CAN_Send_Data(CAN_HandleTypeDef *hcan,uint8_t motor_position,int16_t current)//返回值为发送状态结构体
{
	CAN_TxHeaderTypeDef tx_header;
	uint8_t tx_data[8]={0};//数据载体
	uint32_t tx_mailbox;//HAL库自身填写
	HAL_StatusTypeDef Status;//状态检测
	
	//参数有效性检查，后续完善
	
	//电流限幅保护
	if(current>=MOTOR_CURRENT_MAX)
	{
		current=MOTOR_CURRENT_MAX;
	}
	else if(current<=MOTOR_CURRENT_MIN)
	{
		current=MOTOR_CURRENT_MIN;
	}
	
	//数据发送配置
	tx_header.StdId = CAN_ID;        // 设置标准ID为控制命令ID(0x200)
    tx_header.ExtId = 0x0000;                 // 扩展ID设为0(不使用扩展帧)
    tx_header.IDE = CAN_ID_STD;               // 使用标准帧格式(11位ID)
    tx_header.RTR = CAN_RTR_DATA;            // 数据帧(非远程请求帧)
    tx_header.DLC = 8;                        // 数据长度码(8字节)
    tx_header.TransmitGlobalTime = DISABLE;   // 禁用全局时间戳
	
	//数据组织
	//若只控制一个电机只用配置tx_data 0 1；
	
	int8_t data_index=motor_position*2;//可控制所有电机
	tx_data[data_index]=(current>>8)&0xFF;//&0xff  防止获取高八位后有其他异常数据
	tx_data[data_index+1]=(current)&0xFF;//获取低八位
	
	//发送组织好的信息
	Status=HAL_CAN_AddTxMessage(hcan,&tx_header,tx_data,&tx_mailbox);
	
	return Status;//返回发送状态
	
	
}

