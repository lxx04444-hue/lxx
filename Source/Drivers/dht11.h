#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"

// DHT11连接GPIO引脚：PA1
#define DHT11_GPIO_PORT GPIOA
#define DHT11_GPIO_PIN  GPIO_Pin_1

// 函数声明
void DHT11_Init(void);
uint8_t DHT11_Read(uint8_t *temp, uint8_t *humid);
void DHT11_SetOutput(void);
void DHT11_SetInput(void);
uint8_t DHT11_ReadByte(void);

#endif
