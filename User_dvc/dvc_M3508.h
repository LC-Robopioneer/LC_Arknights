#ifndef DVC_M3508_H
#define DVC_M3508_H

#include "drv_can.h"
#include "alg_pid.h"

class Class_Motor_M3508
{
public:
    // Class_Motor_M3508(uint16_t ID, CAN_HandleTypeDef *hcan);
    void Init(uint16_t ID, CAN_HandleTypeDef *hcan);
    Class_PID Speed_PID; // 速度环PID参数
    Class_PID Angle_PID; // 角度环PID参数

    void Set_Current(int16_t current);                                   // 设置电机电流
    void Set_Omega(float target_omega);                                  // 设置目标角速度，使用速度环PID计算电流
    void Set_Angle(float target_angle);                                  // 设置目标角度，使用角度环PID计算速度
    void Update_From_Feedback(Struct_CAN_Rx_Buffer *rx_buffer);          // 从反馈数据更新电机状态
    inline int16_t Get_Actual_Current() const { return actual_current; } // 获取实际电机电流
    inline float Get_Omega() const { return omega; }                  // 获取电机角速度
    inline float Get_Angle() const { return angle; }                  // 获取电机角度
    inline uint8_t Get_Temperature() const { return temperature; }       // 获取电机温度
    inline uint16_t Get_ID() const { return motor_ID; }                  // 获取电机ID

protected:
    uint16_t motor_ID;
    CAN_HandleTypeDef *can_handler;
    uint8_t *tx_buffer = nullptr;
    uint8_t data_index;

    int16_t actual_current = 0; // 实际电机电流
    float omega = 0;          // 电机角速度
    float angle = 0;         // 电机角度
    uint8_t temperature = 0;    // 电机温度

private:
    static constexpr float ENCODER_RESOLUTION = 8192.0f;
    static constexpr float REDUCTION_RATIO = 19.0f;

    int32_t rotation_count = 0;
    bool is_first_update = true;
    uint16_t last_raw_angle = 0;

    // 私有方法
    float calculate_actual_angle(uint16_t raw_angle); // 计算实际角度
    float normalize_angle(float angle);          // 归一化角度到[-180, 180]
};

#endif
