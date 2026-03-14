#ifndef __DWT_H__
#define __DWT_H__

#include "stm32h7xx_hal.h"
typedef struct
{
    uint32_t s;
    uint32_t ms;
    uint32_t us;
}DWT_Time_t;


typedef struct {
    uint32_t start_time;   // 上次触发的时间点 
    uint32_t expire_time;  // 设定的间隔时间 
} SoftTimer_t;

enum Expire_Status {
    //只使用一次
    Expire_Once = 0,
    //循环使用
    Expire_Loop = 1
};
//非阻塞式延时，用于判断是否超时
bool Is_Timer_ExpiredUs(SoftTimer_t *timer, Expire_Status status);
void DWT_Init();
uint32_t DWT_GetCurrentTimeS();
uint32_t DWT_GetCurrentTimeMs();
uint32_t DWT_GetCurrentTimeUs();
void DWT_Update();
#endif
