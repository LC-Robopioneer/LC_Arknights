#include "drv_can.h"


uint8_t can_tx_packet[8] = {0};
uint8_t can_rx_packet[8] = {0};
uint8_t can_rx_flag = 0;
uint32_t can_rx_id = 0;

/**
 * @brief CAN总线初始化, 暂使用FIFO0
 * @param handle CAN句柄
 */
void CAN_Init(CAN_HandleTypeDef *handle)
{   

}

/**
 * @brief 本设备CAN发送8字节报文
 * @param handle CAN句柄
 * @param id 电机ID, 默认0x201
 * @param pData 要发送的8字节数据
 * @note 默认为标准帧, 电机不支持扩展帧
 * @return 发送状态
 */
HAL_StatusTypeDef CAN_SendMsg(CAN_HandleTypeDef *handle, uint32_t id, uint8_t *pData)
{
    CAN_TxHeaderTypeDef CAN_TxHeader = {0};
    CAN_TxHeader.IDE = CAN_ID_STD;
    CAN_TxHeader.StdId = id;
    CAN_TxHeader.DLC = 8;       // 8-byte
    CAN_TxHeader.RTR = CAN_RTR_DATA;// 数据帧
    CAN_TxHeader.TransmitGlobalTime = DISABLE;

    uint32_t used_mainbox;
    return HAL_CAN_AddTxMessage(handle, &CAN_TxHeader, pData, &used_mainbox);
}

/**
 * @brief 接收8字节报文
 * @param handle CAN句柄
 * @param RxFifo CAN_RX_FIFO0 或 CAN_RX_FIFO1
 * @param buffer 报文存入的缓冲区
 * @param id ID
 * @note 建议在中断里回调
 * @return 接收状态
 */
HAL_StatusTypeDef CAN_ReceiveMsg(CAN_HandleTypeDef *handle, uint32_t RxFifo, uint8_t *buffer, uint32_t *id)
{
    CAN_RxHeaderTypeDef CAN_RxHeader;
    HAL_StatusTypeDef state = HAL_CAN_GetRxMessage(handle, RxFifo, &CAN_RxHeader, buffer);
    
    if(CAN_RxHeader.IDE == CAN_ID_STD) *id = CAN_RxHeader.StdId;
    else *id = CAN_RxHeader.ExtId;

    return state;
}

