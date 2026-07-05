#include "dht11.h"
#include "delay.h"

/**
 * @brief DHT11初始化
 */
void DHT11_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // 开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_SetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN);  // 初始拉高
}

/**
 * @brief 设置DHT11为输出模式
 */
void DHT11_SetOutput(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief 设置DHT11为输入模式（浮空/上拉输入）
 */
void DHT11_SetInput(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // 浮空输入
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief 读取DHT11一个字节
 * @return 读取到的字节数据
 */
uint8_t DHT11_ReadByte(void)
{
    uint8_t i, byte = 0;
    
    for(i = 0; i < 8; i++)
    {
        // 等待低电平变高电平
        while(!GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN));
        Delay_us(30);
        
        // 判断高电平持续时间
        if(GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN))
        {
            byte |= (0x80 >> i);  // 是1
            while(GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN));
            Delay_us(30);
        }
        // 否则是0
    }
    
    return byte;
}

/**
 * @brief 读取DHT11数据（温度、湿度）
 * @param temp: 温度指针
 * @param humid: 湿度指针
 * @return 0:成功  1:失败
 */
uint8_t DHT11_Read(uint8_t *temp, uint8_t *humid)
{
    uint8_t buf[5] = {0};
    uint8_t i = 0;
    uint32_t timeout = 0;
    
    // 主机拉低40-50us，等待DHT11响应
    DHT11_SetOutput();
    GPIO_ResetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    Delay_ms(18);  // 拉低至少18ms
    GPIO_SetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    Delay_us(30);  // 等待30us
    
    // 设置为输入，等待DHT11响应信号
    DHT11_SetInput();
    
    // 等待DHT11拉低（80us）
    timeout = 0;
    while(GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) && timeout < 100)
    {
        timeout++;
        Delay_us(1);
    }
    if(timeout >= 100) return 1;  // 超时
    
    // 等待DHT11拉高（80us）
    timeout = 0;
    while(!GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) && timeout < 100)
    {
        timeout++;
        Delay_us(1);
    }
    if(timeout >= 100) return 1;  // 超时
    
    // 读取5个字节（40bit）
    for(i = 0; i < 5; i++)
    {
        buf[i] = DHT11_ReadByte();
    }
    
    // 校验和验证（前4字节之和应等于第5字节）
    if((buf[0] + buf[1] + buf[2] + buf[3]) != buf[4])
    {
        return 1;  // 校验失败
    }
    
    // buf[0]:湿度整数部分  buf[1]:湿度小数部分
    // buf[2]:温度整数部分  buf[3]:温度小数部分
    // buf[4]:校验和
    
    *humid = buf[0];
    *temp = buf[2];
    
    return 0;  // 读取成功
}
