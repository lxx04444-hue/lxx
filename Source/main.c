#include "stm32f10x.h"
#include "delay.h"
#include "dht11.h"
#include "mq135.h"
#include "oled.h"
#include "stdio.h"

// 报警阈值
#define TEMP_LIMIT    30      // 温度报警阈值(℃)
#define HUMID_LIMIT   70      // 湿度报警阈值(%)
#define GAS_LIMIT     400     // 气体报警阈值(ppm)

// 全局变量
uint8_t temperature = 0;
uint8_t humidity = 0;
uint16_t gas_ppm = 0;
uint8_t alarm_flag = 0;

// 函数声明
void SystemInit_Config(void);
void GPIO_Config(void);
void Beep_Init(void);
void Beep_On(void);
void Beep_Off(void);
void Display_Data(void);
void Check_Alarm(void);

int main(void)
{
    // 系统初始化
    SystemInit_Config();
    Delay_Init();
    GPIO_Config();
    Beep_Init();
    DHT11_Init();
    MQ135_Init();
    OLED_Init();
    
    // OLED显示初始化信息
    OLED_Clear();
    OLED_ShowString(0, 0, "System Init...");
    OLED_ShowString(0, 2, "Waiting...");
    Delay_ms(2000);
    
    // 主循环
    while(1)
    {
        // 读取DHT11温湿度
        if(DHT11_Read(&temperature, &humidity) == 0)
        {
            // 读取成功
        }
        else
        {
            // 读取失败，显示错误
            temperature = 0;
            humidity = 0;
        }
        
        // 读取MQ-135气体浓度
        gas_ppm = MQ135_ReadPPM();
        
        // 更新OLED显示
        Display_Data();
        
        // 检查是否需要报警
        Check_Alarm();
        
        // 采样延时（1秒）
        Delay_ms(1000);
    }
}

/**
 * @brief 系统初始化配置
 */
void SystemInit_Config(void)
{
    // RCC初始化，使用HSE外部晶振
    // 此处可根据实际使用的库文件调整
}

/**
 * @brief GPIO初始化配置
 */
void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 打开GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    
    // PA0 - ADC输入（MQ-135）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // PA1 - DHT11数据线
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // 开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // PA2 - 蜂鸣器控制
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // PB8 - I2C SCL
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // 开漏
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // PB9 - I2C SDA
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // 开漏
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
 * @brief 蜂鸣器初始化
 */
void Beep_Init(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

/**
 * @brief 蜂鸣器打开
 */
void Beep_On(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

/**
 * @brief 蜂鸣器关闭
 */
void Beep_Off(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

/**
 * @brief 更新OLED显示
 */
void Display_Data(void)
{
    char buf[20];
    
    OLED_Clear();
    
    // 第一行：温度
    OLED_ShowString(0, 0, "Temp:");
    sprintf(buf, "%d%cC", temperature, 0xB0);
    OLED_ShowString(48, 0, buf);
    
    // 第二行：湿度
    OLED_ShowString(0, 2, "Humid:");
    sprintf(buf, "%d%%", humidity);
    OLED_ShowString(54, 2, buf);
    
    // 第三行：气体浓度
    OLED_ShowString(0, 4, "Gas:");
    sprintf(buf, "%dppm", gas_ppm);
    OLED_ShowString(36, 4, buf);
    
    // 第四行：报警状态
    OLED_ShowString(0, 6, "Alarm:");
    if(alarm_flag)
    {
        OLED_ShowString(54, 6, "ON ");
    }
    else
    {
        OLED_ShowString(54, 6, "OFF");
    }
}

/**
 * @brief 检查报警条件
 */
void Check_Alarm(void)
{
    if(temperature > TEMP_LIMIT || humidity > HUMID_LIMIT || gas_ppm > GAS_LIMIT)
    {
        Beep_On();
        alarm_flag = 1;
    }
    else
    {
        Beep_Off();
        alarm_flag = 0;
    }
}
