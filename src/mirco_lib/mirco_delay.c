#include "misc.h"
#include <mirco_lib/mirco_delay.h>

volatile uint32_t fac_us = 0;

void mirco_delay_init(void)
{
    // 设置SysTick时钟源为HCLK
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    // 计算每微秒的时钟周期数
    fac_us = SystemCoreClock / 8000000;
}

void mirco_delay_us(uint32_t us)
{
    uint32_t temp;
    SysTick->LOAD = us * fac_us - 1;  // 设定装载值，减去1是为了避免不必要的误差
    SysTick->VAL = 0;                 // 清空当前计数值
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  // 启动SysTick计时器

    do {
        temp = SysTick->CTRL;
    } while (!(temp & SysTick_CTRL_COUNTFLAG_Msk));  // 等待计数标志被置位

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  // 关闭SysTick计时器
    SysTick->VAL = 0;  // 清空当前计数值
}

void mirco_delay_ms(uint32_t ms)
{
    while (ms--)
    {
        mirco_delay_us(1000);  // 每次延时1毫秒
    }
}

void mirco_delay_s(uint32_t s)
{
    while (s--)
    {
        mirco_delay_ms(1000);  // 每次延时1秒
    }
}
