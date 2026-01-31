#ifndef __ALG_PID_H
#define __ALG_PID_H

#include "main.h"

typedef enum PID_ModeTypeDef
{
    PID_MODE_POS_NORMAL,        // 位置式
    PID_MODE_POS_DIFF_FIRST,    // 位置式微分先行
    PID_MODE_INC_NORMAL,        // 增量式
    PID_MODE_INC_DIFF_FIRST,    // 增量式微分先行

} PID_ModeTypeDef;

typedef struct PID_HandleTypeDef
{
    PID_ModeTypeDef mode;       // 计算模式
    float target, actual, output;
    float act0, act1, act2;     // 微分先行专用
    float kp, ki, kd;
    float err0, err1, err2, err_int;
    float err_int_max, output_max, output_min; // 限幅专用
    float deadzone;     // 死区
    float compensation; // 补偿值

} PID_HandleTypeDef;


void PID_SetParam(PID_HandleTypeDef *pid, float kp, float ki, float kd);
void PID_Update(PID_HandleTypeDef *pid);
void PID_UpdateInc(PID_HandleTypeDef *pid);
HAL_StatusTypeDef PID_ParseSerialPack(PID_HandleTypeDef *pid, uint8_t *serial_pack);

#endif /*__PID_ALG_H*/
