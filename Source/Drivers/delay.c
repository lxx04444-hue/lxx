#include "delay.h"

static uint8_t fac_us = 0;   // us延时倍数
static uint16_t fac_ms = 0;  // ms延时倍数

/**
 * @brief 延时初始化
 * 假设系统时钟为72MHz
 */
void Delay_Init(void)
{
    // 配置SysTick定时器
    // SysTick频率 = 系统时钟 / 8
    // 对于72MHz系统时钟，SysTick频率 = 9MHz
    fac_us = 9;    // 每us需要计数9次
    fac_ms = 9000; // 每ms需要计数9000次
}

/**
 * @brief 微秒级延时
 * @param nus: 延时微秒数
 */
void Delay_us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD = nus * fac_us;
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    
    do
    {
        temp = SysTick->CTRL;
    }while((temp & 0x01) && !(temp & (1 << 16)));
    
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
}

/**
 * @brief 毫秒级延时
 * @param nms: 延时毫秒数
 */
void Delay_ms(uint16_t nms)
{
    uint32_t temp;
    SysTick->LOAD = (uint32_t)nms * fac_ms;
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    
    do
    {
        temp = SysTick->CTRL;
    }while((temp & 0x01) && !(temp & (1 << 16)));
    
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
}

/**
 * @brief 秒级延时
 * @param ns: 延时秒数
 */
void Delay_s(uint16_t ns)
{
    uint16_t i;
    for(i = 0; i < ns; i++)
        Delay_ms(1000);
}
