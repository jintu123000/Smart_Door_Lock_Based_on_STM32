# 智能门锁系统（Smart Door Lock）

基于 **STM32F103C8** 的多功能智能门锁系统，支持刷卡、密码、远程开锁，集成环境监测与火灾报警。

## 硬件架构

| 模块 | 型号 | 功能 |
|---|---|---|
| 主控 | STM32F103C8 | Cortex-M3, 72MHz |
| 显示屏 | 0.96" OLED (I2C) | 128x64 人机交互界面 |
| 射频读卡 | RC522 (SPI) | 13.56MHz MIFARE 卡读写 |
| 温湿度 | DHT11 | 环境温湿度监测 |
| 烟雾传感器 | MQ-2 | 烟雾/可燃气体检测与蜂鸣器报警 |
| 输入 | 4x4 矩阵按键 | 密码输入 / 菜单操作 |
| 执行器 | SG90 舵机 | 门锁驱动 |
| 通信 | USART1 + USART3 | 调试输出 / WiFi/蓝牙扩展 |
| 提示 | 有源蜂鸣器 | 操作反馈与火灾报警 |

## 功能特性

- **刷卡开锁** — RC522 读取 MIFARE 卡 ID，密钥认证后开锁
- **密码开锁** — 6 位数字密码，支持退格和确认
- **烟雾报警** — MQ-2 实时监测，软件消抖防误报，蜂鸣器 + OLED 双重报警
- **环境监测** — DHT11 采集温湿度，OLED 状态页实时显示
- **双串口通信** — UART1(115200) 调试 + UART3(9600) 扩展通信
- **菜单交互** — 密码输入页 / 状态显示页可切换

## 项目结构

```
├── APP/                  # 应用层驱动
│   ├── BUZZER.c/h        # 蜂鸣器驱动
│   ├── Delay.c/h         # 微秒/毫秒延时
│   ├── dht11.c/h         # DHT11 温湿度传感器
│   ├── smoke.c/h         # MQ-2 烟雾传感器
│   ├── bsp_rc522.c/h     # RC522 RFID 读写器
│   ├── bsp_uart.c/h      # USART 串口驱动
│   ├── matrix_button.c/h # 矩阵按键扫描
│   ├── OLED.c/h          # OLED 显示驱动
│   ├── Servo.c/h         # 舵机控制
│   ├── PWM.c/h           # PWM 输出
│   ├── timer.c/h         # 系统定时器
│   └── ui.c/h            # 用户界面逻辑
├── Library/              # STM32F10x 标准外设库 (SPL 3.5)
├── Start/                # CMSIS Core + 启动文件
├── Users/
│   ├── main.c            # 主程序入口
│   ├── stm32f10x_conf.h  # 外设库配置
│   └── stm32f10x_it.c/h  # 中断服务函数
├── project.uvprojx       # Keil MDK 工程文件
└── README.md
```

## 报警逻辑

```
TIM2 ISR (每10ms)
  └→ smoke_read_cnt++ → 每1秒触发检测

main() 主循环
  └→ Smoke_Read()          # 连续3次高电平确认烟雾
     └→ smoke_alarm = 1
        └→ 蜂鸣器报警         # 所有页面生效
        └→ OLED 显示 "FIRE ALARM! SMOKE!"
        └→ 按键12手动解除报警
```

## 开发环境

- **IDE**: Keil MDK 5 (uVision)
- **编译器**: ARMCC V5
- **库**: STM32F10x Standard Peripheral Library V3.5
- **烧录**: J-Link / ST-Link (SWD)

## 引脚分配

| 外设 | GPIO |
|---|---|
| OLED I2C SCL/SDA | PB6 / PB7 |
| RC522 SPI | PB12(CS), PB13(SCK), PB14(MOSI), PB15(MISO) |
| DHT11 | PC13 |
| MQ-2 烟雾 | PA0 |
| 蜂鸣器 | PA5 |
| 舵机 PWM | PA6 (TIM3_CH1) |
| 矩阵按键 | ROW: PA8,PB7,PB6,PB5 — COL: PB4,PB3,PA12,PA11 |
| USART1 TX/RX | PA9 / PA10 |
| USART3 TX/RX | PB10 / PB11 |
