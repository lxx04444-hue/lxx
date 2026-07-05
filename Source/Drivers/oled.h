#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"

// OLED I2C接口定义
#define OLED_SCL_PORT GPIOB
#define OLED_SCL_PIN  GPIO_Pin_8
#define OLED_SDA_PORT GPIOB
#define OLED_SDA_PIN  GPIO_Pin_9

// 函数声明
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t x, uint8_t y, char chr);
void OLED_ShowString(uint8_t x, uint8_t y, char *str);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len);
void OLED_ShowCN(uint8_t x, uint8_t y, uint8_t *cn);
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *BMP);

#endif
