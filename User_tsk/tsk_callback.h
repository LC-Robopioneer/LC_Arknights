#ifndef __TSK_CALLBACK_H
#define __TSK_CALLBACK_H

#include "main.h"
#include "can.h"
#include "usart.h"
#include "dma.h"

#include "drv_can.h"
#include "drv_serial.h"

#include "dvc_m3508.h"


extern int8_t TX_Data[8];

void Motor_Call_Back(CAN_Rx_Buffer * can_rx_buffer);
void CAN_RX_Message_Get(void);

#endif


