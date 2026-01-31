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
#include "main.h"
#ifdef STM32F407_H

/* Private macros ------------------------------------------------------------*/

// 滤波器编号
#define CAN_FILTER(x) ((x) << 3)

// 接收队列
#define CAN_FIFO_0 (0 << 2)
#define CAN_FIFO_1 (1 << 2)

// 标准帧或扩展帧
#define CAN_STDID (0 << 1)
#define CAN_EXTID (1 << 1)

// 数据帧或遥控帧
#define CAN_DATA_TYPE (0 << 0)
#define CAN_REMOTE_TYPE (1 << 0)

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Struct_CAN_Manage_Object CAN1_Manage_Object = {0};
Struct_CAN_Manage_Object CAN2_Manage_Object = {0};

// CAN通信发送缓冲区

// 电机共享区
uint8_t CAN1_0x1fe_Tx_Data[8];
uint8_t CAN1_0x1ff_Tx_Data[8];
uint8_t CAN1_0x200_Tx_Data[8];
uint8_t CAN1_0x2fe_Tx_Data[8];
uint8_t CAN1_0x2ff_Tx_Data[8];
uint8_t CAN1_0x3fe_Tx_Data[8];
uint8_t CAN1_0x4fe_Tx_Data[8];

// 电机共享区
uint8_t CAN2_0x1fe_Tx_Data[8];
uint8_t CAN2_0x1ff_Tx_Data[8];
uint8_t CAN2_0x200_Tx_Data[8];
uint8_t CAN2_0x2fe_Tx_Data[8];
uint8_t CAN2_0x2ff_Tx_Data[8];
uint8_t CAN2_0x3fe_Tx_Data[8];
uint8_t CAN2_0x4fe_Tx_Data[8];

uint8_t CAN1_0x72_Tx_Data[8];

/* Private function declarations ---------------------------------------------*/

/* function prototypes -------------------------------------------------------*/

/**
 * @brief 配置CAN的过滤器
 *
 * @param hcan CAN编号
 * @param Object_Para 编号 | FIFOx | ID类型 | 帧类型
 * @param ID ID
 * @param Mask_ID 屏蔽位(0x3ff, 0x1fffffff)
 */
void can_filter_mask_config(CAN_HandleTypeDef *hcan, uint8_t Object_Para, uint32_t ID, uint32_t Mask_ID)
{
    //检测传参是否正确
    assert_param(hcan != NULL);

	   //CAN过滤器初始化结构体
    CAN_FilterTypeDef can_filter_init_structure;
    //滤波器序号, 0-27, 共28个滤波器
    can_filter_init_structure.FilterBank = Object_Para >> 3;
    //滤波器模式，设置ID掩码模式
    can_filter_init_structure.FilterMode = CAN_FILTERMODE_IDMASK;
    
	
    if ((Object_Para & 0x02))
    {   
        //29位 拓展帧
			  // 32位滤波
        can_filter_init_structure.FilterScale = CAN_FILTERSCALE_32BIT;
        //验证码 高16bit
        can_filter_init_structure.FilterIdHigh = (ID << 3) >> 16;
        //验证码 低16bit
        can_filter_init_structure.FilterIdLow = ID << 3 | (Object_Para & 0x03) << 1;
        //屏蔽码 高16bit
        can_filter_init_structure.FilterMaskIdHigh = (Mask_ID << 3) >> 16;
        //屏蔽码 低16bit
        can_filter_init_structure.FilterMaskIdLow = Mask_ID << 3 | (0x03) << 1 ;
    }
    else
    {
        //11位 标准帧
			  // 32位滤波
        can_filter_init_structure.FilterScale = CAN_FILTERSCALE_16BIT;
        //标准帧验证码 高16bit不启用
        can_filter_init_structure.FilterIdHigh = 0x0000 ; 
        //验证码 低16bit
			  can_filter_init_structure.FilterIdLow =ID << 5 | (Object_Para & 0x02) << 4;  
        //标准帧屏蔽码 高16bit不启用
        can_filter_init_structure.FilterMaskIdHigh =  0x0000 ;
        //屏蔽码 低16bit
        can_filter_init_structure.FilterMaskIdLow =(Mask_ID << 5) | 0x01 << 4 ; 
    }

    //滤波器绑定FIFO0或FIFO1
    can_filter_init_structure.FilterFIFOAssignment = (Object_Para >> 2) & 0x01;
    //从机模式选择开始单元 , 前14个在CAN1, 后14个在CAN2
    can_filter_init_structure.SlaveStartFilterBank = 14;
    //使能滤波器
    can_filter_init_structure.FilterActivation = ENABLE;

    // 过滤器配置
    if(HAL_CAN_ConfigFilter(hcan, &can_filter_init_structure)!=HAL_OK)
    {
        Error_Handler();
    }
	
}

/**
 * @brief 初始化CAN总线
 *
 * @param hcan CAN编号
 * @param Callback_Function 处理回调函数
 */
void CAN_Init(CAN_HandleTypeDef *hcan, CAN_Call_Back Callback_Function)
{
     if (hcan->Instance == CAN1)
    {
        CAN1_Manage_Object.CAN_Handler = hcan;
        CAN1_Manage_Object.Callback_Function = Callback_Function;					
//         can_filter_mask_config(hcan, CAN_FILTER(0) | CAN_FIFO_0 | CAN_STDID | CAN_DATA_TYPE, 0x200 ,0x7F8);  //只接收0x200-0x207
//         can_filter_mask_config(hcan, CAN_FILTER(1) | CAN_FIFO_1 | CAN_STDID | CAN_DATA_TYPE, 0x200, 0x7F8);
			can_filter_mask_config(hcan, CAN_FILTER(0) | CAN_FIFO_0 | CAN_STDID | CAN_DATA_TYPE, 0 ,0);
			can_filter_mask_config(hcan, CAN_FILTER(1) | CAN_FIFO_1 | CAN_STDID | CAN_DATA_TYPE, 0 ,0);
    }
    else if (hcan->Instance == CAN2)
    {
        CAN2_Manage_Object.CAN_Handler = hcan;
        CAN2_Manage_Object.Callback_Function = Callback_Function;
		can_filter_mask_config(hcan, CAN_FILTER(14) | CAN_FIFO_0 | CAN_STDID | CAN_DATA_TYPE, 0 ,0);  //只接收
			can_filter_mask_config(hcan, CAN_FILTER(14) | CAN_FIFO_0 | CAN_STDID | CAN_DATA_TYPE, 0 ,0);
//	    can_filter_mask_config(hcan, CAN_FILTER(15) | CAN_FIFO_1 | CAN_EXTID | CAN_DATA_TYPE, 0x200, 0x7F8);
    }
    /*离开初始模式*/
    HAL_CAN_Start(hcan);				
    
    /*开中断*/
    HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);       //can 接收fifo 0不为空中断
	HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);       //can 接收fifo 1不为空中断
}

/**
 * @brief 发送数据帧
 *
 * @param hcan CAN编号
 * @param ID ID
 * @param Data 被发送的数据指针
 * @param Length 长度
 * @return uint8_t 执行状态
 */
uint8_t CAN_Send_Data(CAN_HandleTypeDef *hcan, uint16_t ID, uint8_t *Data, uint16_t Length)
{
    CAN_TxHeaderTypeDef tx_header;
    uint32_t used_mailbox;

    //检测传参是否正确
    assert_param(hcan != NULL);

    tx_header.StdId = ID;
    tx_header.ExtId = 0;
    tx_header.IDE = 0;
    tx_header.RTR = 0;
    tx_header.DLC = Length;

    return (HAL_CAN_AddTxMessage(hcan, &tx_header, Data, &used_mailbox));
}
/**
 * @brief 配置拓展帧
 *   
 * @param equipment_id 设备id
 * @param data2	数据区2内容
 * @param cmd_id	控制指令
 * @return 拓展帧id
 */
uint32_t EXT_ID_Set(uint8_t equipment_id,uint16_t data2,uint8_t cmd_id)
{
	uint32_t send_ext_id;
	send_ext_id	=	cmd_id<<24|data2<<8|equipment_id;
	return send_ext_id;
}

/**
 * @brief 发送拓展帧
 *
 * @param hcan CAN编号
 * @param ID ID
 * @param Data 被发送的数据指针
 * @param Length 长度
 * @return uint8_t 执行状态
 */
uint8_t CAN_Send_EXT_Data(CAN_HandleTypeDef *hcan, uint32_t ID, uint8_t *Data, uint16_t Length)
{
    CAN_TxHeaderTypeDef tx_header;
    uint32_t used_mailbox;

    //检测传参是否正确
    assert_param(hcan != NULL);

    tx_header.ExtId = ID;
    tx_header.StdId = 0;
    tx_header.IDE = 4;
    tx_header.RTR = 0;
    tx_header.DLC = Length;

    return (HAL_CAN_AddTxMessage(hcan, &tx_header, Data, &used_mailbox));
}

// /**
//  * @brief CAN的TIM定时器中断发送回调函数
//  *
//  */
// void TIM_1ms_CAN_PeriodElapsedCallback()
// {
//     // DJI电机专属

//     static int mod2 = 0;
//     mod2++;
//     if (mod2 == 2)
//     {
//         mod2 = 0;

//         // CAN2半频电机
//         // 舵轮底盘舵向电机
//         CAN_Send_Data(&hcan2, 0x1fe, CAN2_0x1fe_Tx_Data, 8);
//         // 舵轮底盘轮向电机
//         CAN_Send_Data(&hcan2, 0x200, CAN2_0x200_Tx_Data, 8);
//     }
//     // 摩擦轮和拨弹盘电机
//     CAN_Send_Data(&hcan1, 0x200, CAN1_0x200_Tx_Data, 8);
//     // 云台电机
//     CAN_Send_Data(&hcan1, 0x1fe, CAN1_0x1fe_Tx_Data, 8);
// }

/**
 * @brief HAL库CAN接收FIFO0中断
 *
 * @param hcan CAN编号
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    // 判断程序初始化完成
    // if (init_finished == false)
    // {
    //     return;
    // }

    // 选择回调函数
    if (hcan->Instance == CAN1)
    {
        HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO0, &CAN1_Manage_Object.Rx_Buffer.Header, CAN1_Manage_Object.Rx_Buffer.Data);
        if(CAN1_Manage_Object.Callback_Function != nullptr)
        {
            CAN1_Manage_Object.Callback_Function(&CAN1_Manage_Object.Rx_Buffer);
        }
    }
    else if (hcan->Instance == CAN2)
    {
        HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO0, &CAN2_Manage_Object.Rx_Buffer.Header, CAN2_Manage_Object.Rx_Buffer.Data);
        if(CAN2_Manage_Object.Callback_Function != nullptr)
        {
            CAN2_Manage_Object.Callback_Function(&CAN2_Manage_Object.Rx_Buffer);
        }
    }
}

/**
 * @brief HAL库CAN接收FIFO1中断
 *
 * @param hcan CAN编号
 */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    // // 判断程序初始化完成
    // if (init_finished == false)
    // {
    //     return;
    // }

    // 选择回调函数
    if (hcan->Instance == CAN1)
    {
        HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO1, &CAN1_Manage_Object.Rx_Buffer.Header, CAN1_Manage_Object.Rx_Buffer.Data);
        if(CAN1_Manage_Object.Callback_Function != nullptr)
        {
            CAN1_Manage_Object.Callback_Function(&CAN1_Manage_Object.Rx_Buffer);
        }
    }
    else if (hcan->Instance == CAN2)
    {
        HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO1, &CAN2_Manage_Object.Rx_Buffer.Header, CAN2_Manage_Object.Rx_Buffer.Data);
        if(CAN2_Manage_Object.Callback_Function != nullptr)
        {
            CAN2_Manage_Object.Callback_Function(&CAN2_Manage_Object.Rx_Buffer);
        }
    }
}

/**
 * @brief CAN的定时器中断发送回调函数
 * 
 */
void TIM_CAN_PeriodElapsedCallback()
{
  //CAN1总线 挂载4个底盘电机(M33508)
  CAN_Send_Data(&hcan1, 0x200, CAN1_0x200_Tx_Data, 8);
}

#elif STM32H723_H
// 定义FDCAN_Manage结构体
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

    // FDCAN_Filter_Mask_Config(hfdcan, FDCAN_FILTER(1) | FDCAN_FIFO_1 | FDCAN_STDID | FDCAN_DATA_TYPE, 0, 0);
  }
  else if (hfdcan->Instance == FDCAN2)
  {
    FDCAN2_Manage_Object.FDCAN_Handler = hfdcan;
    FDCAN2_Manage_Object.Callback_Function = Callback_Function;
    // FDCAN_Filter_Mask_Config(hfdcan, FDCAN_FILTER(1) | FDCAN_FIFO_0 | FDCAN_STDID | FDCAN_DATA_TYPE, 0, 0);
    FDCAN_Filter_Mask_Config(hfdcan, FDCAN_FILTER(4) | FDCAN_FIFO_0 | FDCAN_EXTID | FDCAN_DATA_TYPE, 0, 0);
    // FDCAN_Filter_Mask_Config(hfdcan, FDCAN_FILTER(15) | FDCAN_FIFO_1 | FDCAN_STDID | FDCAN_DATA_TYPE, 0, 0);
  }
  else if (hfdcan->Instance == FDCAN3)
  {
    FDCAN3_Manage_Object.FDCAN_Handler = hfdcan;
    FDCAN3_Manage_Object.Callback_Function = Callback_Function;
    FDCAN_Filter_Mask_Config(hfdcan, FDCAN_FILTER(2) | FDCAN_FIFO_0 | FDCAN_STDID | FDCAN_DATA_TYPE, 0, 0);
    FDCAN_Filter_Mask_Config(hfdcan, FDCAN_FILTER(3) | FDCAN_FIFO_0 | FDCAN_EXTID | FDCAN_DATA_TYPE, 0, 0);
    // FDCAN_Filter_Mask_Config(hfdcan, FDCAN_FILTER(15) | FDCAN_FIFO_1 | FDCAN_STDID | FDCAN_DATA_TYPE, 0, 0);
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

  // 检测传参是否正确
  assert_param(hfdcan != NULL);

  // 标准帧或拓展帧判断
  if ((Object_Para & 0x02) == 0)
  {
    fdcan_filter_init_structure.IdType = FDCAN_STANDARD_ID;
  }
  else if ((Object_Para & 0x02) != 0)
  {
    fdcan_filter_init_structure.IdType = FDCAN_EXTENDED_ID;
  }
  // 设置滤波器编号
  fdcan_filter_init_structure.FilterIndex = Object_Para >> 3; // Object_Para >> 3
  // 设置过滤器MASK模式
  fdcan_filter_init_structure.FilterType = FDCAN_FILTER_MASK;
  // 设置FIFO
  if ((Object_Para & 0x04) == 0)
  {
    fdcan_filter_init_structure.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  }
  else if ((Object_Para & 0x04) != 0)
  {
    fdcan_filter_init_structure.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
  }
  // 设置过滤ID及掩码
  fdcan_filter_init_structure.FilterID1 = ID;
  fdcan_filter_init_structure.FilterID2 = Mask_ID;

  // 使用结构体进行初始化
  HAL_FDCAN_ConfigFilter(hfdcan, &fdcan_filter_init_structure);
  // 启用全局过滤
  if ((Object_Para & 0x01) == 0)
  {
    HAL_FDCAN_ConfigGlobalFilter(hfdcan, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE);
  }
  else if ((Object_Para & 0x01) != 0)
  {
    HAL_FDCAN_ConfigGlobalFilter(hfdcan, FDCAN_REJECT, FDCAN_REJECT, ENABLE, ENABLE);
  }
  // 打开FIFO区的新消息通知
  if ((Object_Para & 0x04) == 0)
  {
    HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
  }
  else if ((Object_Para & 0x04) != 0)
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

  // 检测传参是否正确
  assert_param(hfdcan != NULL);

  tx_header.Identifier = ID; // ID号
  // 标准拓展ID判断，默认为标准ID
  if (__FDCAN_ID_Type == FDCAN_ID_Standard)
  {
    tx_header.IdType = FDCAN_STANDARD_ID; // 标准ID
  }
  else if (__FDCAN_ID_Type == FDCAN_ID_Extended)
  {
    tx_header.IdType = FDCAN_EXTENDED_ID;
  }
  tx_header.TxFrameType = FDCAN_DATA_FRAME; // 数据帧
  switch (__data_length)
  {
  case (1):
  {
    tx_header.DataLength = FDCAN_DLC_BYTES_1; // 数据长度

    break;
  }
  case (2):
  {
    tx_header.DataLength = FDCAN_DLC_BYTES_2; // 数据长度

    break;
  }
  case (3):
  {
    tx_header.DataLength = FDCAN_DLC_BYTES_3; // 数据长度

    break;
  }
  case (4):
  {
    tx_header.DataLength = FDCAN_DLC_BYTES_4; // 数据长度

    break;
  }
  case (5):
  {
    tx_header.DataLength = FDCAN_DLC_BYTES_5; // 数据长度

    break;
  }
  case (6):
  {
    tx_header.DataLength = FDCAN_DLC_BYTES_6; // 数据长度

    break;
  }
  case (7):
  {
    tx_header.DataLength = FDCAN_DLC_BYTES_7; // 数据长度

    break;
  }
  case (8):
  {
    tx_header.DataLength = FDCAN_DLC_BYTES_8; // 数据长度

    break;
  }
  }

  // 以下是FDCAN相较于经典CAN配置有拓展的地方

  tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;  // CAN发送错误提示（？）
  tx_header.BitRateSwitch = FDCAN_BRS_OFF;           // 波特率切换关闭
  tx_header.FDFormat = FDCAN_CLASSIC_CAN;            // 经典CAN模式
  tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS; // 不储存发送事件（？）
  tx_header.MessageMarker = 0;                       // 消息标记0（？）

  return (HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &tx_header, Data));
}

/**
 * @brief CAN的TIM定时器中断发送回调函数
 *
 */
void TIM_1ms_CAN_PeriodElapsedCallback()
{
  // 如需使用拓展ID，切记第四个参数设为FDCAN_ID_Extended
  // FDCAN_Send_Data(&hfdcan1, 0x1ff, FDCAN1_0x1ff_Tx_Data, FDCAN_ID_Extended);

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
  // 选择回调函数
  if (hfdcan->Instance == FDCAN1)
  {
    HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN1_Manage_Object.Rx_Buffer.Header, FDCAN1_Manage_Object.Rx_Buffer.Data);
    if (FDCAN1_Manage_Object.Callback_Function != nullptr)
    {
      FDCAN1_Manage_Object.Callback_Function(&FDCAN1_Manage_Object.Rx_Buffer);
    }
  }
  else if (hfdcan->Instance == FDCAN2)
  {
    HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN2_Manage_Object.Rx_Buffer.Header, FDCAN2_Manage_Object.Rx_Buffer.Data);
    if (FDCAN2_Manage_Object.Callback_Function != nullptr)
    {
      FDCAN2_Manage_Object.Callback_Function(&FDCAN2_Manage_Object.Rx_Buffer);
    }
  }
  else if (hfdcan->Instance == FDCAN3)
  {
    HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN3_Manage_Object.Rx_Buffer.Header, FDCAN3_Manage_Object.Rx_Buffer.Data);
    if (FDCAN3_Manage_Object.Callback_Function != nullptr)
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
  // 选择回调函数
  if (hfdcan->Instance == FDCAN1)
  {
    HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &FDCAN1_Manage_Object.Rx_Buffer.Header, FDCAN1_Manage_Object.Rx_Buffer.Data);
    if (FDCAN1_Manage_Object.Callback_Function != nullptr)
    {
      FDCAN1_Manage_Object.Callback_Function(&FDCAN1_Manage_Object.Rx_Buffer);
    }
  }
  else if (hfdcan->Instance == FDCAN2)
  {
    HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &FDCAN2_Manage_Object.Rx_Buffer.Header, FDCAN2_Manage_Object.Rx_Buffer.Data);
    if (FDCAN2_Manage_Object.Callback_Function != nullptr)
    {
      FDCAN2_Manage_Object.Callback_Function(&FDCAN2_Manage_Object.Rx_Buffer);
    }
  }
  else if (hfdcan->Instance == FDCAN3)
  {
    HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &FDCAN3_Manage_Object.Rx_Buffer.Header, FDCAN3_Manage_Object.Rx_Buffer.Data);
    if (FDCAN3_Manage_Object.Callback_Function != nullptr)
    {
      FDCAN3_Manage_Object.Callback_Function(&FDCAN3_Manage_Object.Rx_Buffer);
    }
  }
}

#endif
/************************ COPYRIGHT(C) ROBOPIONEER **************************/
