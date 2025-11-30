#ifndef DVC_M3508_H
#define DVC_M3508_H

#include "drv_can.h"
#include "alg_pid.h"

class Class_Motor_M3508
{
public:
    Class_Motor_M3508(uint16_t ID, CAN_HandleTypeDef *hcan);

    Class_PID Speed_PID{15.0f, 0.0f, 0.1f, 0.01f, 16384.0f, 16384.0f}; // 速度环PID参数
    Class_PID Angle_PID{5.0f, 0.0f, 0.5f, 0.01f, 500.0f, 300.0f}; // 角度环PID参数

    void Set_Current(int16_t current);                            // 设置电机电流
    void Set_Speed(float target_speed);                          // 设置目标速度，使用速度环PID计算电流
    void Set_Angle(float target_angle);                          // 设置目标角度，使用角度环PID计算速度
    void Update_From_Feedback(Struct_CAN_Rx_Buffer *rx_buffer);   // 从反馈数据更新电机状态
    int16_t Get_Actual_Current() const { return actual_current; } // 获取实际电机电流
    uint16_t Get_Speed() const { return speed; }                  // 获取电机速度
    uint16_t Get_Angle() const { return angle; }                  // 获取电机角度
    uint8_t Get_Temperature() const { return temperature; }       // 获取电机温度
    uint16_t Get_ID() const { return motor_ID; }                  // 获取电机ID
	inline void Set_temperature();
private:
    uint16_t motor_ID;
    CAN_HandleTypeDef *can_handler;
    uint8_t *tx_buffer = nullptr;
    uint8_t data_index;

    int16_t actual_current = 0; // 实际电机电流
    uint16_t speed = 0;         // 电机速度
    uint16_t angle = 0;         // 电机角度
    uint8_t temperature = 0;    // 电机温度
};
void Class_Motor_M3508::Set_temperature()
{
	temperature++;
}
#endif
