# STM32F103 室内环境监测系统

基于STM32F103C8T6单片机的室内环境监测系统，支持温湿度和有害气体检测。

## 项目特性

- ✅ **温湿度检测**：DHT11传感器
- ✅ **有害气体检测**：MQ-135传感器
- ✅ **实时显示**：OLED 128×64屏幕（I2C）
- ✅ **报警功能**：超限自动蜂鸣器报警
- ✅ **Proteus仿真**：完整的电路仿真文件
- ✅ **Keil开发**：标准库STM32工程

## 硬件配置

| 组件 | 型号 | 接口 | 连接 |
|------|------|------|------|
| 主控 | STM32F103C8T6 | - | - |
| 温湿度 | DHT11 | GPIO | PA1 |
| 气体传感器 | MQ-135 | ADC | PA0 |
| 显示屏 | OLED 128×64 | I2C | PB8(SCL), PB9(SDA) |
| 蜂鸣器 | 无源蜂鸣器 | GPIO | PA2 |
| 电源 | 5V/GND | - | - |

## 项目结构

```
lxx/
├── README.md                      # 项目说明
├── Keil_Project/                  # Keil MDK工程
│   ├── STM32F103_Monitor.uvproj   # Keil项目文件
│   ├── STM32F103_Monitor.uvopt
│   └── Listings/                  # 编译输出
├── Proteus_Simulation/            # Proteus仿真
│   ├── env_monitor.dsn           # Proteus电路图
│   └── env_monitor.pdsprj        # Proteus项目文件
├── Source/                        # 源代码
│   ├── main.c                     # 主程序
│   ├── system_stm32f10x.c         # 系统初始化
│   ├── Drivers/
│   │   ├── dht11.c/dht11.h       # DHT11驱动
│   │   ├── mq135.c/mq135.h       # MQ-135驱动
│   │   ├── oled.c/oled.h         # OLED驱动
│   │   └── delay.c/delay.h       # 延时函数
│   └── Hardware/
│       ├── stm32f10x_conf.h      # 库配置文件
│       └── stm32f10x_it.c/h      # 中断处理
├── Libraries/                     # STM32库文件
│   └── STM32F10x_StdPeriph_Driver/
└── Doc/                           # 文档
    ├── 电路原理图.png
    ├── 引脚分配表.txt
    └── 调试说明.txt
```

## 快速开始

### 1. 环境要求
- Keil MDK 5.x 或更高版本
- Proteus 8.x（仿真用）
- STM32F103库文件

### 2. 编译步骤
1. 用Keil打开 `Keil_Project/STM32F103_Monitor.uvproj`
2. 选择编译配置（Debug/Release）
3. 点击 "Build" 编译

### 3. 仿真步骤
1. 用Proteus打开 `Proteus_Simulation/env_monitor.dsn`
2. 点击运行（Play）开始仿真
3. 观察OLED显示和传感器数据变化

### 4. 烧录到硬件
1. 连接ST-Link下载器
2. 在Keil中选择 Debug → Download
3. 或使用STM32 Programmer工具烧录hex文件

## 核心功能说明

### 温湿度采集（DHT11）
```c
// 读取温湿度
DHT11_ReadData(&temperature, &humidity);
// 结果存储在temperature和humidity中
```

### 有害气体检测（MQ-135）
```c
// 读取ADC值并转换为ppm
uint16_t gas_ppm = MQ135_ReadPPM();
```

### OLED显示
```c
// 显示数据
OLED_ShowString(0, 0, "Temp:");
OLED_ShowNum(48, 0, temperature, 2);
```

### 报警控制
```c
// 超限报警
if (temperature > TEMP_LIMIT || gas_ppm > GAS_LIMIT)
{
    Beep_On();
}
```

## 主要参数配置

| 参数 | 值 | 说明 |
|------|-----|------|
| TEMP_LIMIT | 30 | 温度报警上限(℃) |
| HUMID_LIMIT | 70 | 湿度报警上限(%) |
| GAS_LIMIT | 400 | 气体报警上限(ppm) |
| SAMPLE_TIME | 1000ms | 采样间隔 |

## 调试技巧

1. **串口调试**：可选接PA9(TX)/PA10(RX)到USB转串口模块
2. **OLED不显示**：检查I2C地址是否为0x78
3. **DHT11读取异常**：检查GPIO上拉电阻(4.7kΩ)
4. **MQ-135数据漂移**：需要预热30分钟

## 常见问题

**Q: 如何修改采样速率？**
A: 在main.c中修改 `Delay_ms(1000)` 的参数

**Q: OLED显示不正常？**
A: 检查I2C接线，确保SCL和SDA正确连接

**Q: 如何添加蓝牙通讯？**
A: 参考 `Source/Hardware/uart.c` 配置串口，连接HC-05模块

## 扩展功能

- [ ] 蓝牙无线传输数据
- [ ] SD卡数据记录
- [ ] WiFi上传到云平台
- [ ] 多传感器数据融合
- [ ] 数据曲线显示

## 参考资源

- [STM32F103官方数据手册](https://www.st.com/en/microcontrollers-microprocessors/stm32f103.html)
- [DHT11传感器文档](https://github.com/adafruit/DHT-sensor-library)
- [OLED I2C驱动](https://github.com/adafruit/Adafruit_SSD1306)
- [STM32标准库](https://github.com/STMicroelectronics/STM32F1xx_HAL_Driver)

## 许可证

MIT License

## 作者

lxx04444-hue

---

**最后更新**: 2024年  
**版本**: v1.0
