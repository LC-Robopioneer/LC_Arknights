/**
 * @file drv_can.c
 * @author Lucy
 * @brief H723 FDCAN配置为经典CAN数据包
 * @version 0.1
 * @date 2024-10-02
 *
 * @copyright RoboPioneer (c) 2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "drv_can.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

//定义FDCAN_Manage结构体
Struct_FDCAN_Manage_Object FDCAN1_Manage_Object = {0};
Struct_FDCAN_Manage_Object FDCAN2_Manage_Object = {0};
Struct_FDCAN_Manage_Object FDCAN3_Manage_Object = {0};

// CAN通信发送缓冲区
uint8_t FDCAN1_0x1ff_Tx_Data[8];
uint8_t FDCAN1_0x200_Tx_Data[8];
uint8_t FDCAN1_0x2ff_Tx_Data[8];
uint8_t FDCAN1_0x1fe_Tx_Data[8];
uint8_t FDCAN1_0x2fe_Tx_Data[8];

// 0x500 DJI从板发送数据缓冲区
uint8_t FDCAN1_0x500_Tx_Data[8];

uint8_t FDCAN2_0x1ff_Tx_Data[8];
uint8_t FDCAN2_0x200_Tx_Data[8];
uint8_t FDCAN2_0x2ff_Tx_Data[8];
uint8_t FDCAN2_0x1fe_Tx_Data[8];
uint8_t FDCAN2_0x2fe_Tx_Data[8];

uint8_t FDCAN3_0x1ff_Tx_Data[8];
uint8_t FDCAN3_0x200_Tx_Data[8];
uint8_t FDCAN3_0x2ff_Tx_Data[8];
uint8_t FDCAN3_0x1fe_Tx_Data[8];
uint8_t FDCAN3_0x2fe_Tx_Data[8];

/* Private function declarations ---------------------------------------------*/

/* function prototypes -------------------------------------------------------*/

/**
* @brief 初始化CAN总线,如需添加过滤器，参考备注
 *
 * @param hcan CAN编号
 * @param Callback_Function 处理回调函数
 */
void FDCAN_Init(FDCAN_HandleTypeDef *hfdcan, FDCAN_Call_Back Callback_Function)
{
    if (hfdcan->Instance == FDCAN1)
    {
        FDCAN1_Manage_Object.FDCAN_Handler = hfdcan;
        FDCAN1_Manage_Object.Callback_Function = Callback_Function;
        FDCAN_Filter_Mask_Config(hfdcan, FDCAN_FILTER(0) | FDCAN_FIFO_0 | FDCAN_STDID | FDCAN_DATA_TYPE, 0, 0);
        FDCAN_Filter_Mask_Config(hfdcan, FDCAN_FILTER(1) | FDCAN_FIFO_1 | FDCAN_EXTID | FDCAN_DATA_TYPE, 0, 0);
    }
    else if (hfdcan->Instance == FDCAN2)
    {
        FDCAN2_Manage_Object.FDCAN_Handler = hfdcan;
        FDCAN2_Manage_Object.Callback_Function = Callback_Function;
        FDCAN_Filter_Mask_Config(hfdcan, FDCAN_FILTER(0) | FDCAN_FIFO_0 | FDCAN_STDID | FDCAN_DATA_TYPE, 0, 0);
        FDCAN_Filter_Mask_Config(hfdcan, FDCAN_FILTER(1) | FDCAN_FIFO_1 | FDCAN_EXTID | FDCAN_DATA_TYPE, 0, 0);
        //FDCAN_Filter_Mask_Config(hfdcan, FDCAN_FILTER(15) | FDCAN_FIFO_1 | FDCAN_STDID | FDCAN_DATA_TYPE, 0, 0);
    }
		else if (hfdcan->Instance == FDCAN3)
    {
        FDCAN3_Manage_Object.FDCAN_Handler = hfdcan;
        FDCAN3_Manage_Object.Callback_Function = Callback_Function;
        FDCAN_Filter_Mask_Config(hfdcan, FDCAN_FILTER(2) | FDCAN_FIFO_0 | FDCAN_STDID | FDCAN_DATA_TYPE, 0, 0);
				FDCAN_Filter_Mask_Config(hfdcan, FDCAN_FILTER(3) | FDCAN_FIFO_0 | FDCAN_EXTID | FDCAN_DATA_TYPE, 0, 0);
			//FDCAN_Filter_Mask_Config(hfdcan, FDCAN_FILTER(15) | FDCAN_FIFO_1 | FDCAN_STDID | FDCAN_DATA_TYPE, 0, 0);
    }
		
		HAL_FDCAN_Start(hfdcan);
}

/**
 * @brief 配置CAN的过滤器
 *
 * @param hcan CAN编号
 * @param Object_Para 编号 | FIFOx | ID类型 | 帧类型
 * @param ID ID
 * @param Mask_ID 屏蔽位(0x3ff, 0x1fffffff)
 */
void FDCAN_Filter_Mask_Config(FDCAN_HandleTypeDef *hfdcan, uint8_t Object_Para, uint32_t ID, uint32_t Mask_ID)
{
    FDCAN_FilterTypeDef fdcan_filter_init_structure;
	
    //检测传参是否正确
    assert_param(hfdcan != NULL);

    //标准帧或拓展帧判断
    if((Object_Para & 0x02) == 0)
    {
        fdcan_filter_init_structure.IdType = FDCAN_STANDARD_ID;
    }
		else if((Object_Para & 0x02) != 0)
		{
				fdcan_filter_init_structure.IdType = FDCAN_EXTENDED_ID;
		}
    //设置滤波器编号
    fdcan_filter_init_structure.FilterIndex = Object_Para >> 3;//Object_Para >> 3
    //设置过滤器MASK模式
    fdcan_filter_init_structure.FilterType = FDCAN_FILTER_MASK;
    //设置FIFO
    if((Object_Para & 0x04) == 0)
    {
        fdcan_filter_init_structure.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    }
    else if((Object_Para & 0x04) != 0)
    {
        fdcan_filter_init_structure.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
    }
    //设置过滤ID及掩码
    fdcan_filter_init_structure.FilterID1 = ID;
    fdcan_filter_init_structure.FilterID2 = Mask_ID;
		
    //使用结构体进行初始化
    HAL_FDCAN_ConfigFilter(hfdcan, &fdcan_filter_init_structure);
    //启用全局过滤
    if((Object_Para & 0x01) == 0)
    {
        HAL_FDCAN_ConfigGlobalFilter(hfdcan, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE);
    }
    else if((Object_Para & 0x01) != 0)
    {
        HAL_FDCAN_ConfigGlobalFilter(hfdcan, FDCAN_REJECT, FDCAN_REJECT, ENABLE, ENABLE);
    }
    //打开FIFO区的新消息通知
    if((Object_Para & 0x04) == 0)
    {
        HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    }
    else if((Object_Para & 0x04) != 0)
    {
        HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);
    }
}

/**
 * @brief 发送数据帧
 *
 * @param hcan CAN编号
 * @param ID ID
 * @param Data 被发送的数据指针
 * @param standard_extended_select 拓展ID标准ID选择变量，默认为标准ID
 * @return uint8_t 执行状态
 */
uint8_t FDCAN_Send_Data(FDCAN_HandleTypeDef *hfdcan, uint32_t ID, uint8_t *Data, Enum_FDCAN_ID_Type __FDCAN_ID_Type, uint8_t __data_length)
{
    FDCAN_TxHeaderTypeDef tx_header;

    //检测传参是否正确
    assert_param(hfdcan != NULL);

    tx_header.Identifier = ID;                          //ID号
		//标准拓展ID判断，默认为标准ID
		if(__FDCAN_ID_Type == FDCAN_ID_Standard)
		{
				tx_header.IdType = FDCAN_STANDARD_ID;			      //标准ID
		}
    else if(__FDCAN_ID_Type == FDCAN_ID_Extended)
		{
				tx_header.IdType = FDCAN_EXTENDED_ID;
		}
    tx_header.TxFrameType = FDCAN_DATA_FRAME;		        //数据帧
    switch(__data_length)
    {
        case(1):
        {
          tx_header.DataLength = FDCAN_DLC_BYTES_1;						//数据长度

          break;
        }      
        case(2):
        {
          tx_header.DataLength = FDCAN_DLC_BYTES_2;						//数据长度
          
          break;
        }   
        case(3):
        {
          tx_header.DataLength = FDCAN_DLC_BYTES_3;						//数据长度
          
          break;
        }   
        case(4):
        {
          tx_header.DataLength = FDCAN_DLC_BYTES_4;						//数据长度
          
          break;
        }   
        case(5):
        {
          tx_header.DataLength = FDCAN_DLC_BYTES_5;						//数据长度
          
          break;
        }   
        case(6):
        {
          tx_header.DataLength = FDCAN_DLC_BYTES_6;						//数据长度
          
          break;
        }   
        case(7):
        {
          tx_header.DataLength = FDCAN_DLC_BYTES_7;						//数据长度
          
          break;
        }   
        case(8):
        {
          tx_header.DataLength = FDCAN_DLC_BYTES_8;						//数据长度
          
          break;
        }  
    }
		
                              
    //以下是FDCAN相较于经典CAN配置有拓展的地方

    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;     //CAN发送错误提示（？）
		tx_header.BitRateSwitch = FDCAN_BRS_OFF;              //波特率切换关闭
    tx_header.FDFormat = FDCAN_CLASSIC_CAN;               //经典CAN模式   
    tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;    //不储存发送事件（？）
    tx_header.MessageMarker = 0;	                        //消息标记0（？）

    return (HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &tx_header, Data));
}

/**
 * @brief CAN的TIM定时器中断发送回调函数
 *
 */
void TIM_1ms_CAN_PeriodElapsedCallback()
{
		//如需使用拓展ID，切记第四个参数设为FDCAN_ID_Extended
		//FDCAN_Send_Data(&hfdcan1, 0x1ff, FDCAN1_0x1ff_Tx_Data, FDCAN_ID_Extended);
	
    // CAN1电机
    // FDCAN_Send_Data(&hfdcan1, 0x1ff, FDCAN1_0x1ff_Tx_Data);
    // FDCAN_Send_Data(&hfdcan1, 0x200, FDCAN1_0x200_Tx_Data);
    // FDCAN_Send_Data(&hfdcan1, 0x2ff, FDCAN1_0x2ff_Tx_Data);

    // CAN2电机
    // FDCAN_Send_Data(&hfdcan2, 0x1ff, FDCAN2_0x1ff_Tx_Data);
    // FDCAN_Send_Data(&hfdcan2, 0x200, FDCAN2_0x200_Tx_Data);
    // FDCAN_Send_Data(&hfdcan2, 0x2ff, FDCAN2_0x2ff_Tx_Data);
	
		// CAN3电机
    // FDCAN_Send_Data(&hfdcan3, 0x1ff, FDCAN3_0x1ff_Tx_Data);
    // FDCAN_Send_Data(&hfdcan3, 0x200, FDCAN3_0x200_Tx_Data);
    // FDCAN_Send_Data(&hfdcan3, 0x2ff, FDCAN3_0x2ff_Tx_Data);
}

/**
 * @brief HAL库CAN接收FIFO0中断
 *
 * @param hcan CAN编号
 */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    //选择回调函数
    if (hfdcan->Instance == FDCAN1)
    {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN1_Manage_Object.Rx_Buffer.Header, FDCAN1_Manage_Object.Rx_Buffer.Data);
				if(FDCAN1_Manage_Object.Callback_Function != nullptr)
				{
						FDCAN1_Manage_Object.Callback_Function(&FDCAN1_Manage_Object.Rx_Buffer);
				}
    }
    else if (hfdcan->Instance == FDCAN2)
    {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN2_Manage_Object.Rx_Buffer.Header, FDCAN2_Manage_Object.Rx_Buffer.Data);
        if(FDCAN2_Manage_Object.Callback_Function != nullptr)
				{
						FDCAN2_Manage_Object.Callback_Function(&FDCAN2_Manage_Object.Rx_Buffer);
				}
    }
		else if (hfdcan->Instance == FDCAN3)
    {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN3_Manage_Object.Rx_Buffer.Header, FDCAN3_Manage_Object.Rx_Buffer.Data);
        if(FDCAN3_Manage_Object.Callback_Function != nullptr)
				{
						FDCAN3_Manage_Object.Callback_Function(&FDCAN3_Manage_Object.Rx_Buffer);
				}
    }
}

/**
 * @brief HAL库CAN接收FIFO1中断
 *
 * @param hcan CAN编号
 */
void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    //选择回调函数
    if (hfdcan->Instance == FDCAN1)
    {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &FDCAN1_Manage_Object.Rx_Buffer.Header, FDCAN1_Manage_Object.Rx_Buffer.Data);
				if(FDCAN1_Manage_Object.Callback_Function != nullptr)
				{
						FDCAN1_Manage_Object.Callback_Function(&FDCAN1_Manage_Object.Rx_Buffer);
				}
        
    }
    else if (hfdcan->Instance == FDCAN2)
    {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &FDCAN2_Manage_Object.Rx_Buffer.Header, FDCAN2_Manage_Object.Rx_Buffer.Data);
        if(FDCAN2_Manage_Object.Callback_Function != nullptr)
				{
						FDCAN2_Manage_Object.Callback_Function(&FDCAN2_Manage_Object.Rx_Buffer);
				}
    }
		else if (hfdcan->Instance == FDCAN3)
    {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &FDCAN3_Manage_Object.Rx_Buffer.Header, FDCAN3_Manage_Object.Rx_Buffer.Data);
        if(FDCAN3_Manage_Object.Callback_Function != nullptr)
				{
						FDCAN3_Manage_Object.Callback_Function(&FDCAN3_Manage_Object.Rx_Buffer);
				}
    }
}

/************************ COPYRIGHT(C) ROBOPIONEER **************************/
