#ifndef __MQ135_H
#define __MQ135_H

#include "stm32f10x.h"

// MQ-135连接GPIO引脚：PA0(ADC)
#define MQ135_GPIO_PORT GPIOA
#define MQ135_GPIO_PIN  GPIO_Pin_0
#define MQ135_ADC_CH    ADC_Channel_0

// 校准参数
#define CALIBRATION_SAMPLE_TIMES 50
#define CALIBRATION_SAMPLE_INTERVAL 50  // ms

// 函数声明
void MQ135_Init(void);
uint16_t MQ135_ReadADC(void);
float MQ135_GetRS(uint16_t adc_value);
uint16_t MQ135_ReadPPM(void);
float MQ135_GetPPM(float rs_ro_ratio);

#endif
