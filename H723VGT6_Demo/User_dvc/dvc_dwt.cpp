#include "dvc_dwt.h"

DWT_Time_t SystemTime;

static uint64_t accumulated_cycles = 0;
static uint32_t last_cycle_count = 0;

void DWT_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; 
    DWT->CYCCNT = 0;                                
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            
    
    last_cycle_count = DWT->CYCCNT;
    accumulated_cycles = 0;
}

void DWT_Update(void)
{
    uint32_t cpu_freq = HAL_RCC_GetSysClockFreq();
    if (cpu_freq == 0) return;
    uint32_t current_cnt = DWT->CYCCNT;

    // 利用无符号减法自动处理 32 位溢出 
    uint32_t delta = current_cnt - last_cycle_count;
    accumulated_cycles += delta;
    last_cycle_count = current_cnt;

    // 正确的时间单位换算
    SystemTime.s  = (uint32_t)(accumulated_cycles / cpu_freq);
    SystemTime.ms = (uint32_t)(accumulated_cycles / (cpu_freq / 1000));
    SystemTime.us = (uint64_t)(accumulated_cycles / (cpu_freq / 1000000));
}

uint32_t DWT_GetCurrentTimeS(void)
{
    DWT_Update();
    return SystemTime.s;
}
uint32_t DWT_GetCurrentTimeMs(void)
{
    DWT_Update();
    return SystemTime.ms;
}
uint32_t DWT_GetCurrentTimeUs(void)
{
    DWT_Update();
    return SystemTime.us;
}



/**
 * @brief  非阻塞微秒级超时检查
 * @param  timer  : 定时器结构体指针，需提前设置 expire_time
 * @param  status : 模式选择
 * - Expire_Loop: 循环模式。到期后自动补偿时间基准，消除累积误差。
 * - Expire_Once: 单次模式。到期后不再更新时间基准。
 * @return bool   : true  - 已达到或超过设定的超时时间
 * false - 仍在等待中
 */
bool Is_Timer_ExpiredUs(SoftTimer_t* timer,Expire_Status status = Expire_Loop)
{
    // 获取当前微秒级时间戳
    uint32_t current_time = DWT_GetCurrentTimeUs(); 
    
    // 计算时间差（无符号减法自动处理 32 位翻转）
    uint32_t delta = current_time - timer->start_time;

    if (delta >= timer->expire_time)
    {
        // 更新起始时间点
        // 注意：这里使用 += 而不是 = current_time 可以补偿函数调用的微小抖动
        if(status == Expire_Loop)
        {
            timer->start_time += timer->expire_time; 
        }
        return true;
    }
    return false;
}
