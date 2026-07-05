#include "stm32f10x.h"

// 系统时钟设置为72MHz
// 使用HSE外部晶振（8MHz）
// HSE -> PLL (x9) -> 72MHz

void SystemInit(void)
{
    // 启用HSE
    RCC->CR |= RCC_CR_HSEON;
    while(!(RCC->CR & RCC_CR_HSERDY));
    
    // 配置PLL
    RCC->CFGR |= RCC_CFGR_PLLSRC;      // 使用HSE作为PLL输入
    RCC->CFGR |= RCC_CFGR_PLLMULL9;    // PLL倍频系数x9
    
    // 启用PLL
    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY));
    
    // 配置FLASH时序
    FLASH->ACR |= FLASH_ACR_LATENCY_2;
    
    // 切换系统时钟到PLL
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}
