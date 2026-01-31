#include "alg_pid.h"
#include "stdlib.h"

#define abs(x)  ((x)>=0 ? (x) : -(x))     // 取绝对值

/**
 * @brief PID参数赋值
 */
void PID_SetParam(PID_HandleTypeDef *pid, float kp, float ki, float kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

/**
 * @brief 位置式PID更新
 * @note 取用output作为输出
 */
void PID_Update(PID_HandleTypeDef *pid)
{
    // 微分先行 - 更新实际值
    if(pid->mode == PID_MODE_POS_DIFF_FIRST)
        pid->act1 = pid->act0;
    
    // 误差更新
    pid->err1 = pid->err0;
    
    // 误差 = 目标 - 实际
    pid->err0 = pid->target - pid->actual;

    // 死区处理
    if(abs(pid->err0) < pid->deadzone) pid->err0 = 0;

    // 积分计算
    pid->err_int += pid->err0;

    // 积分限幅
    if(pid->err_int > pid->err_int_max) pid->err_int = pid->err_int_max;
    if(pid->err_int < -pid->err_int_max) pid->err_int = -pid->err_int_max;

    // 计算输出
    if(pid->mode == PID_MODE_POS_NORMAL)
        pid->output = pid->kp * pid->err0 +
                      pid->ki * pid->err_int +
                      pid->kd * (pid->err1 - pid->err0) +
                      pid->compensation;
    else if(pid->mode == PID_MODE_POS_DIFF_FIRST)
        pid->output = pid->kp * pid->err0 +
                      pid->ki * pid->err_int +
                      pid->kd * (pid->act1 - pid->act0) +
                      pid->compensation;

    // 输出限幅
    if(pid->output > pid->output_max) pid->output = pid->output_max;
    if(pid->output < pid->output_min) pid->output = pid->output_min;
    
}

/**
 * @brief 增量式PID更新, 输出为增量
 */
void PID_UpdateInc(PID_HandleTypeDef *pid)
{
    // 微分先行 - 更新实际值
    if(pid->mode == PID_MODE_POS_DIFF_FIRST)
    {
        pid->act2 = pid->act1;
        pid->act1 = pid->act0;
    }

    // 误差更新
    pid->err2 = pid->err1;
    pid->err1 = pid->err0;

    // 误差 = 目标 - 实际
    pid->err0 = pid->target - pid->actual;

    // 死区处理
    if(abs(pid->err0) < pid->deadzone) pid->err0 = 0;

    // 积分计算
    pid->err_int += pid->err0;

    // 积分限幅
    if(pid->err_int > pid->err_int_max) pid->err_int = pid->err_int_max;
    if(pid->err_int < -pid->err_int_max) pid->err_int = -pid->err_int_max;

    // 计算输出
    if(pid->mode == PID_MODE_POS_NORMAL)
        pid->output = pid->kp * (pid->err0 - pid->err1) +
                    pid->ki * pid->err0 +
                    pid->kd * (pid->err0 - 2*pid->err1 + pid->err2);
    else if(pid->mode == PID_MODE_POS_DIFF_FIRST)
        pid->output = pid->kp * (pid->err0 - pid->err1) +
                    pid->ki * pid->err0 +
                    pid->kd * (pid->act0 - 2*pid->act1 + pid->act2);

    // 输出限幅
    if(pid->output > pid->output_max) pid->output = pid->output_max;
    if(pid->output < pid->output_min) pid->output = pid->output_min;
    
}

/**
 * @brief 为串口调试的解析函数
 * @param pid 待赋值的PID结构体
 * @param serial_pack 串口接收数据包
 * @note 数据包结构: "KxFF.F\n"
 * @note 包头'K', 模式'x'(p/i/d/t), 数值"FF.F", 包尾'\n'
 */
HAL_StatusTypeDef PID_ParseSerialPack(PID_HandleTypeDef *pid, uint8_t *serial_pack)
{
    // 包头为'K
    if(serial_pack[0] != 'K')
    {
        return HAL_ERROR;
    }

    // 取出数值部分
    char *pack_ptr = (char *)&serial_pack[2];
    char value_str[20] = {0};
    uint8_t idx = 0;
    do
    {
        value_str[idx] = *pack_ptr;
        pack_ptr++;
    }
    while (*pack_ptr != '\n');
    

    //判断模式
    switch (serial_pack[1])
    {
    case 'p':
        pid->kp = atof(value_str);
        break;
    
    case 'i':
        pid->ki = atof(value_str);
        break;
    
    case 'd':
        pid->kd = atof(value_str);
        break;
    
    case 't':
        pid->target = atof(value_str);
        break;
    
    default:
        return HAL_ERROR;
    }

    return HAL_OK;
}
