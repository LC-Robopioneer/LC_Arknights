#include "delay.h"

//使用这个延时函数会覆盖SysTick的配置，可能会影响HAL库的SysTick计时。
//如果使用HAL库的HAL_GetTick()来获取系统时间，会导致系统时间不准。
//如果不需要HAL_GetTick()，那么可以放心使用。

/**
  * @brief  微秒级延时
  * @param  xus 延时时长，范围：0~186413
  * @retval 无
  */
void Delay_us(uint32_t xus)
{
    SysTick->LOAD = 180 * xus;               // 180MHz系统时钟
    SysTick->VAL = 0x00;
    SysTick->CTRL = 0x00000007;              // 使用HCLK作为时钟源
    while(!(SysTick->CTRL & 0x00010000));
    SysTick->CTRL = 0x00000006;
}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
    while(xms--)
    {
        Delay_us(1000);
    }
}

/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_s(uint32_t xs)
{
    while(xs--)
    {
        Delay_ms(1000);
    }
}


