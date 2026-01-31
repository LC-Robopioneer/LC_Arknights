#include "dvc_M3508.h"

void Class_Motor_M3508::Init(uint16_t ID, CAN_HandleTypeDef *hcan)
{
    motor_ID = ID;
    can_handler = hcan;

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
    if (current > 16384)
        current = 16384;
    if (current < -16384)
        current = -16384;

    if (tx_buffer != nullptr)
    {
        tx_buffer[data_index] = (current >> 8) & 0xFF;
        tx_buffer[data_index + 1] = current & 0xFF;
    }
}

void Class_Motor_M3508::Set_Omega(float target_omega)
{
    // 使用速度环PID计算所需电流
    float current_output = Speed_PID.Calculate(target_omega, omega);
    Set_Current(static_cast<int16_t>(current_output));
}

float speedoutput = 0.0f; //调试用
float diff = 0.0f;
void Class_Motor_M3508::Set_Angle(float target_angle)
{
    float angle_diff = static_cast<float>(target_angle - angle);
    angle_diff = normalize_angle(angle_diff);

    float speed_output = Angle_PID.Calculate(angle_diff, 0.0f); // 跟踪差值
    speedoutput = speed_output;
    diff = angle_diff;
    Set_Omega(speed_output);
}

float Class_Motor_M3508::calculate_actual_angle(uint16_t raw_angle)
{
    if (is_first_update)
    {
        last_raw_angle = raw_angle;
        rotation_count = 0;
        is_first_update = false;
    }
    else
    {
        int16_t angle_diff = static_cast<int16_t>(raw_angle) - static_cast<int16_t>(last_raw_angle);
        if (angle_diff < -ENCODER_RESOLUTION / 2)
        {
            rotation_count++; // 增加一圈
        }
        // 处理反向溢出（从0跳到8191）
        else if (angle_diff > ENCODER_RESOLUTION / 2)
        {
            rotation_count--; // 减少一圈
        }

        last_raw_angle = raw_angle;
    }

    float motor_angle = (rotation_count * 360.0f) + (raw_angle * 360.0f / 8192.0f);

    float output_angle = motor_angle / REDUCTION_RATIO; // 减速比

    output_angle = normalize_angle(output_angle);
    rotation_count = rotation_count % static_cast<int32_t>(REDUCTION_RATIO); // 防止rotation_count过大
    
    return output_angle;
}

float Class_Motor_M3508::normalize_angle(float angle) // 归一化角度到[-180, 180]
{
    while (angle > 180.0f)
    {
        angle -= 360.0f;
    }
    while (angle < -180.0f)
    {
        angle += 360.0f;
    }
    return angle;
}

void Class_Motor_M3508::Update_From_Feedback(Struct_CAN_Rx_Buffer *rx_buffer)
{
    if (rx_buffer == nullptr)
    {
        return; // 空指针，直接返回
    }

    // 解析反馈数据
    uint16_t raw_angle = static_cast<uint16_t>(((rx_buffer->Data[0] << 8) | rx_buffer->Data[1]));
    angle = calculate_actual_angle(raw_angle);

    int16_t raw_speed = static_cast<int16_t>(((rx_buffer->Data[2] << 8) | rx_buffer->Data[3]));
    omega = static_cast<float>(raw_speed * 3.1415926 / REDUCTION_RATIO / 30); // 转换为输出轴速度

    actual_current = static_cast<int16_t>(((rx_buffer->Data[4] << 8) | rx_buffer->Data[5]));
    temperature = rx_buffer->Data[6];
}
