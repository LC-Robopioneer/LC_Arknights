/**
 * @file drv_can.h
 * @author Lucy
 * @brief H723 FDCAN配置为经典CAN数据包
 * @version 0.1
 * @date 2024-10-02
 *
 * @copyright RoboPioneer (c) 2024
 *
 */

#ifndef DRV_CAN_H
#define DRV_CAN_H

/* Includes ------------------------------------------------------------------*/
#include "drv_config.h"
#ifdef STM32F407_H
#include "stm32f4xx_hal.h"

/**
 * @brief CAN接收的信息结构体
 *
 */
struct Struct_CAN_Rx_Buffer
{
    CAN_RxHeaderTypeDef Header;
    uint8_t Data[8];
};

/**
 * @brief CAN通信接收回调函数数据类型
 *
 */
typedef void (*CAN_Call_Back)(Struct_CAN_Rx_Buffer *);

/**
 * @brief CAN通信处理结构体
 *
 */
struct Struct_CAN_Manage_Object
{
    CAN_HandleTypeDef *CAN_Handler;
    Struct_CAN_Rx_Buffer Rx_Buffer;
    CAN_Call_Back Callback_Function;
};

/* Exported variables ---------------------------------------------------------*/


extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

extern Struct_CAN_Manage_Object CAN1_Manage_Object;
extern Struct_CAN_Manage_Object CAN2_Manage_Object;

extern uint8_t CAN1_0x1fe_Tx_Data[];
extern uint8_t CAN1_0x1ff_Tx_Data[];
extern uint8_t CAN1_0x200_Tx_Data[];
extern uint8_t CAN1_0x2fe_Tx_Data[];
extern uint8_t CAN1_0x2ff_Tx_Data[];
extern uint8_t CAN1_0x3fe_Tx_Data[];
extern uint8_t CAN1_0x4fe_Tx_Data[];

extern uint8_t CAN2_0x1fe_Tx_Data[];
extern uint8_t CAN2_0x1ff_Tx_Data[];
extern uint8_t CAN2_0x200_Tx_Data[];
extern uint8_t CAN2_0x2fe_Tx_Data[];
extern uint8_t CAN2_0x2ff_Tx_Data[];
extern uint8_t CAN2_0x3fe_Tx_Data[];
extern uint8_t CAN2_0x4fe_Tx_Data[];

extern uint8_t CAN1_0x72_Tx_Data[];

/* Exported function declarations ---------------------------------------------*/

void CAN_Init(CAN_HandleTypeDef *hcan, CAN_Call_Back Callback_Function);

uint8_t CAN_Send_Data(CAN_HandleTypeDef *hcan, uint16_t ID, uint8_t *Data, uint16_t Length);

uint32_t EXT_ID_Set(uint8_t equipment_id,uint16_t data2,uint8_t cmd_id);

uint8_t CAN_Send_EXT_Data(CAN_HandleTypeDef *hcan, uint32_t ID, uint8_t *Data, uint16_t Length);

void TIM_CAN_PeriodElapsedCallback();





#elif defined(STM32H723_H)
#include "stm32h7xx_hal.h"
/* Exported macros -----------------------------------------------------------*/

// 滤波器编号
#define FDCAN_FILTER(x) ((x) << 3)

// 接收队列
#define FDCAN_FIFO_0 (0 << 2)
#define FDCAN_FIFO_1 (1 << 2)

// 标准帧或扩展帧
#define FDCAN_STDID (0 << 1)
#define FDCAN_EXTID (1 << 1)

// 数据帧或遥控帧
#define FDCAN_DATA_TYPE (0 << 0)
#define FDCAN_REMOTE_TYPE (1 << 0)

/* Exported types ------------------------------------------------------------*/
/**
 * @brief ID类型
 *
 */
enum Enum_FDCAN_ID_Type
{
	FDCAN_ID_Standard = 0,
	FDCAN_ID_Extended,
};

/**
 * @brief CAN接收的信息结构体
 *
 */
struct Struct_FDCAN_Rx_Buffer
{
	FDCAN_RxHeaderTypeDef Header;
	uint8_t Data[8];
};

/**
 * @brief CAN通信接收回调函数数据类型
 *
 */
typedef void (*FDCAN_Call_Back)(Struct_FDCAN_Rx_Buffer *);

/**
 * @brief CAN通信处理结构体
 *
 */
struct Struct_FDCAN_Manage_Object
{
	FDCAN_HandleTypeDef *FDCAN_Handler;
	Struct_FDCAN_Rx_Buffer Rx_Buffer;
	FDCAN_Call_Back Callback_Function;
};

/* Exported variables ---------------------------------------------------------*/

extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;
extern FDCAN_HandleTypeDef hfdcan3;

extern Struct_FDCAN_Manage_Object FDCAN1_Manage_Object;
extern Struct_FDCAN_Manage_Object FDCAN2_Manage_Object;
extern Struct_FDCAN_Manage_Object FDCAN3_Manage_Object;

/* Exported function declarations ---------------------------------------------*/

void FDCAN_Init(FDCAN_HandleTypeDef *hfdcan, FDCAN_Call_Back Callback_Function);

void FDCAN_Filter_Mask_Config(FDCAN_HandleTypeDef *hfdcan, uint8_t Object_Para, uint32_t ID, uint32_t Mask_ID);

uint8_t FDCAN_Send_Data(FDCAN_HandleTypeDef *hfdcan, uint32_t ID, uint8_t *Data, Enum_FDCAN_ID_Type __FDCAN_ID_Type = FDCAN_ID_Standard, uint8_t __data_length = 8);

void TIM_1ms_CAN_PeriodElapsedCallback();

#endif
#endif

/************************ COPYRIGHT(C) ROBOPIONEER **************************/
