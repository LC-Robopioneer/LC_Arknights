#include "tsk_callback.h"
int16_t  Rx_Encoder;
int16_t  Rx_Omega;
int16_t  Rx_Torque;
int16_t  Rx_Temperature;

int16_t  Tx_Encoder;
int16_t  Tx_Omega;
int16_t  Tx_Torque;
int16_t  Tx_Temperature;
int8_t TX_Data[8];
int test_callback=0;
void Motor_Call_Back(CAN_Rx_Buffer * can_rx_buffer)
{
    uint8_t *Rx_Data = can_rx_buffer->Data;
    switch (can_rx_buffer->Header.StdId)
    {
        case (0x201): case(0x202):case(0x203):case(0x204):
        {
            Rx_Encoder = (Rx_Data[0] << 8) | Rx_Data[1];
            Rx_Omega = (Rx_Data[2] << 8) | Rx_Data[3];
            Rx_Torque = (Rx_Data[4] << 8) | Rx_Data[5];
            Rx_Temperature = Rx_Data[6];
        }
        break;
    }
	Tx_Encoder = Rx_Encoder;
    Tx_Omega = Rx_Omega;
    Tx_Torque = Rx_Torque;
    Tx_Temperature = Rx_Temperature;
	test_callback++;
}
void CAN_RX_Message_Get(void)
{
    
    
    TX_Data[0] = (Tx_Omega >> 8) & 0xFF;
    TX_Data[1] = Tx_Omega & 0xFF;
    
    TX_Data[2] = (Tx_Encoder >> 8) & 0xFF;
    TX_Data[3] = Tx_Encoder & 0xFF;
    
    TX_Data[4] = (Tx_Torque >> 8) & 0xFF;
    TX_Data[5] = Tx_Torque & 0xFF;
    
    TX_Data[6] = (Tx_Temperature >> 8) & 0xFF;
    TX_Data[7] = Tx_Temperature & 0xFF;
    
    char buffer[64];
    //int len = 
	  sprintf(buffer, "Encoder:%d, Omega:%d, Torque:%d, Temp:%d\n", Tx_Encoder, Tx_Omega, Tx_Torque, Tx_Temperature);
    //HAL_UART_Transmit_DMA(&huart3, (uint8_t*)buffer, len);
	//Delay_ms(10);
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)//uart3
{
    Serial_RxEventCallback(huart, Size);
}

