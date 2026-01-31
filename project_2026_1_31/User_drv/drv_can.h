#ifndef __DRV_CAN_H
#define __DRV_CAN_H

#include "main.h"
#include "can.h"

#define CAN_ID  0x200
#define CAN_IDE CAN_ID_STD

extern uint8_t can_rx_packet[8];
extern uint8_t can_tx_packet[8];
extern uint8_t can_rx_flag;
extern uint32_t can_rx_id;

void CAN_Init(CAN_HandleTypeDef *handle);
HAL_StatusTypeDef CAN_SendMsg(CAN_HandleTypeDef *handle, uint32_t id, uint8_t *pData);
HAL_StatusTypeDef CAN_ReceiveMsg(CAN_HandleTypeDef *handle, uint32_t RxFifo, uint8_t *buffer, uint32_t *id);


#endif
