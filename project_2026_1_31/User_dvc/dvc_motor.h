#ifndef __DVC_MOTOR_H
#define __DVC_MOTOR_H

#include "main.h"
#include "can.h"
#include "drv_can.h"
#include "alg_pid.h"

typedef struct Motor_HandleTypeDef
{
    CAN_HandleTypeDef *hcan;    // 使用的CAN外设句柄
    PID_HandleTypeDef *pid;     // PID控制器
    uint32_t id;        // ID号, 1~8
    float gear_ratio;   // 减速比
    float angle;     // 转子角度, 单位度
    int16_t rpm;      // 转子转速, 单位RPM
    float real_current;     // 实际电流
    float target_current;   // 目标电流
    int8_t temperature;  // 电机温度, 单位摄氏度

} Motor_HandleTypeDef;

extern Motor_HandleTypeDef hmotor1;
extern Motor_HandleTypeDef hmotor[9];


void Motor_Init(void);
HAL_StatusTypeDef Motor_SetCurrentDirect(Motor_HandleTypeDef *hmotor, float current);
HAL_StatusTypeDef Motor_SetCurrentMap(Motor_HandleTypeDef *hmotor, int32_t current_map);
HAL_StatusTypeDef Motor_GetParam(Motor_HandleTypeDef *hmotor);
void Motor_PIDUpdate(Motor_HandleTypeDef *hmotor);
HAL_StatusTypeDef Motor_SetTargetRPM(Motor_HandleTypeDef *hmotor, float target_rpm);

#endif
