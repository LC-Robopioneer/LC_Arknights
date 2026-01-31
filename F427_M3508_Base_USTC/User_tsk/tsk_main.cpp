#include "tsk_main.h"

void tsk_loop(void)
{
    static int16_t torque = 0;
    while(torque < 1000)
    {
      torque += 50;
      CAN1_0x200_Tx_Data[0] = torque >> 8;
      CAN1_0x200_Tx_Data[1] = torque;
      CAN_Send_Data(&hcan1, 0x200, CAN1_0x200_Tx_Data, 8);
      HAL_Delay(50);
    }
    while(torque > -1000)
    {
      torque -= 50;
      CAN1_0x200_Tx_Data[0] = torque >> 8;
      CAN1_0x200_Tx_Data[1] = torque;
      CAN_Send_Data(&hcan1, 0x200, CAN1_0x200_Tx_Data, 8);
      HAL_Delay(50);
    }
}
