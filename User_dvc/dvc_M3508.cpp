#include "dvc_M3508.h"

Class_Motor_M3508::Class_Motor_M3508(uint16_t ID, CAN_HandleTypeDef *hcan) : motor_ID(ID), can_handler(hcan)
{
    if (motor_ID <= 4) // 0x200
    {
        data_index = (motor_ID - 1) * 2;

        if (can_handler == &hcan1)
        {
            tx_buffer = CAN1_0x200_Tx_Data;
        }
        else if (can_handler == &hcan2)
        {
            tx_buffer = CAN2_0x200_Tx_Data;
        }
    }
    else if (motor_ID <= 8) // 0x1ff
    {
        data_index = (motor_ID - 5) * 2;

        if (can_handler == &hcan1)
        {
            tx_buffer = CAN1_0x1ff_Tx_Data;
        }
        else if (can_handler == &hcan2)
        {
            tx_buffer = CAN2_0x1ff_Tx_Data;
        }
    }
    else
    {
        return; // 无效ID，直接返回
    }
}

void Class_Motor_M3508::Set_Current(int16_t current)
{
    if (current > 16384) current = 16384;
    if (current < -16384) current = -16384;

    if (tx_buffer != nullptr)
    {
        tx_buffer[data_index] = (current >> 8) & 0xFF;
        tx_buffer[data_index + 1] = current & 0xFF;
    }
}

void Class_Motor_M3508::Set_Speed(float target_speed)
{
    // 使用速度环PID计算所需电流
    float current_output = Speed_PID.Calculate(target_speed, static_cast<float>(speed));
    Set_Current(static_cast<int16_t>(current_output));
}

void Class_Motor_M3508::Set_Angle(float target_angle)
{
    // 使用角度环PID计算所需速度
    float speed_output = Angle_PID.Calculate(target_angle, static_cast<float>(angle));
    Set_Speed(speed_output);
}

void Class_Motor_M3508::Update_From_Feedback(Struct_CAN_Rx_Buffer *rx_buffer)
{
    if (rx_buffer == nullptr)
    {
        return; // 空指针，直接返回
    }

    // 解析反馈数据
    angle = (int16_t)((rx_buffer->Data[0] << 8) | rx_buffer->Data[1]);
    speed = (int16_t)((rx_buffer->Data[2] << 8) | rx_buffer->Data[3]);
    actual_current = (int16_t)((rx_buffer->Data[4] << 8) | rx_buffer->Data[5]);
    temperature = rx_buffer->Data[6];
}


