#include "mq135.h"
#include "delay.h"

// MQ-135校准参数
static float MQ135_RO = 10.0f;  // 标定时的R0值（在清洁空气中）

/**
 * @brief MQ-135初始化
 */
void MQ135_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    
    // 打开GPIOA和ADC1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    
    // 配置GPIO为模拟输入
    GPIO_InitStructure.GPIO_Pin = MQ135_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(MQ135_GPIO_PORT, &GPIO_InitStructure);
    
    // 复位ADC1
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
    
    // 配置ADC1
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    // 使能ADC1
    ADC_Cmd(ADC1, ENABLE);
    
    // ADC校准
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

/**
 * @brief 读取ADC值
 * @return ADC转换结果（0-4095）
 */
uint16_t MQ135_ReadADC(void)
{
    // 配置转换通道、采样周期
    ADC_RegularChannelConfig(ADC1, MQ135_ADC_CH, 1, ADC_SampleTime_239Cycles5);
    
    // 开始转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    // 等待转换完成
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    
    // 返回转换结果
    return ADC_GetConversionValue(ADC1);
}

/**
 * @brief 计算传感器电阻值RS
 * @param adc_value: ADC采样值
 * @return RS值
 */
float MQ135_GetRS(uint16_t adc_value)
{
    // MQ-135接分压电路
    // VRL = ADC_Value / 4095 * 3.3V
    // RS = RL * (VCC - VRL) / VRL
    // 假设RL = 10K, VCC = 5V
    
    float vrl = (float)adc_value / 4095.0f * 3.3f;
    float rs = 10000.0f * (5.0f - vrl) / vrl;  // RL=10K
    
    return rs;
}

/**
 * @brief 计算PPM浓度
 * @param rs_ro_ratio: RS/RO比值
 * @return PPM值
 */
float MQ135_GetPPM(float rs_ro_ratio)
{
    // MQ-135的PPM计算公式（针对CO2）
    // PPM = 116.6021 * ((RS/RO)^-2.769) - 0.0151
    // 或其他拟合公式
    
    float ppm = 116.6021f * pow(rs_ro_ratio, -2.769f) - 0.0151f;
    
    if(ppm < 0) ppm = 0;
    
    return ppm;
}

/**
 * @brief 读取PPM值
 * @return 浓度值（ppm）
 */
uint16_t MQ135_ReadPPM(void)
{
    uint16_t adc_value = MQ135_ReadADC();
    float rs = MQ135_GetRS(adc_value);
    float rs_ro_ratio = rs / MQ135_RO;
    float ppm = MQ135_GetPPM(rs_ro_ratio);
    
    return (uint16_t)ppm;
}
