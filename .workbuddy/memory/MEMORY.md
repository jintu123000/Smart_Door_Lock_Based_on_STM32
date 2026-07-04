# 智能门锁项目记忆

## 项目基本信息

- **项目路径**: `c:\Users\24263\Desktop\大三下\01智能门锁`
- **项目性质**: 大三课程/毕设项目，用于写简历
- **首次分析日期**: 2026-03-22

---

## 硬件平台

### 主工程
- **MCU**: STM32F103C8T6（ARM Cortex-M3，64KB Flash，20KB RAM，72MHz）
- **库**: STM32标准外设库 V3.5.0
- **IDE**: Keil MDK 5，工程文件 `project.uvprojx`

### 子工程（key_lock/）
- **MCU**: STM32F0xx（HAL库，48MHz）
- 功能更完整，含状态机菜单、Flash密码存储、继电器开锁

---

## 引脚配置汇总

| 模块 | 引脚 | 说明 |
|------|------|------|
| 蜂鸣器 | PA5 | 推挽输出，高电平开启 |
| USART1 | PA9/PA10 | 115200bps，调试串口 |
| USART3 | PB10/PB11 | 9600bps，蓝牙通信 |
| DHT11 | PC13 | 单总线协议 |
| RC522 CS | PB12 | RFID片选 |
| RC522 SCK | PB13 | RFID时钟 |
| RC522 MOSI | PB14 | 数据输出 |
| RC522 MISO | PB15 | 数据输入 |
| RC522 RST | PA8 | RFID复位 |
| 舵机 PWM | PA6 | TIM3_CH2，50Hz |
| 键盘行 | PA7,PB0,PB1,PB7 | 推挽输出 |
| 键盘列 | PA11,PA12,PB5,PB6 | 上拉输入 |

---

## 功能模块简述

### TIM2 任务调度（核心架构）
- PSC=7199, ARR=99 → 10ms中断（100Hz）
- 键盘扫描：150ms（计数器>15）
- DHT11读取：5000ms（计数器>500）
- OLED刷新+RFID检测：800ms（计数器>80）
- ISR只设标志位，实际操作在主循环

### RC522 RFID
- 软件模拟SPI（PB12-15 + PA8）
- ISO14443A/Mifare S50协议
- 流程：寻卡→防冲突→选卡→验密→读写块
- 成功后将 input_key 设为 {1,2,3,4,5,6} 触发开锁

### 矩阵键盘（4×4）
- 逐行扫描，键值1-16
- 键13：切主界面；键16：切状态界面
- 键1-9：数字输入；键10：退格；键11：输入0

### UI逻辑
- 主界面（cai_dan=0）：密码输入，光标闪烁动画
- 状态界面（cai_dan=1）：温度/湿度/蓝牙状态显示
- 默认密码 `door_key[6]={1,2,3,4,5,6}`
- 匹配成功计数>200后串口3发送 `<G>开锁成功`

### 舵机
- TIM3, PSC=71, ARR=19999 → 50Hz
- `CCR = Angle/180*2000+500`

### OLED
- SSD1306，128×64，软件I2C
- 双缓冲：操作显存→OLED_Update()整帧刷新

---

## 面试准备要点（2026-03-22整理）

### 简历描述关键词
- TIM2定时器中断多任务分时调度
- 软件模拟SPI + ISO14443A/Mifare协议栈
- OLED双缓冲刷新机制
- DHT11单总线协议
- TIM3 PWM舵机角度控制
- 双串口（USART1调试 + USART3蓝牙）

### 面试常问Q&A（已整理详细答案）
1. 为什么用软件SPI → 引脚冲突（PA6被TIM3占用），800ms触发频率影响不大
2. TIM2多任务调度原理 → 计数器+标志位，ISR最小化
3. RFID流程 → 寻卡→防冲突→选卡→验密→读写
4. DHT11时序 → 40位数据，40μs采样，读取时关TIM2中断防被打断
5. 舵机PWM计算 → PSC=71,ARR=19999,CCR=角度/180×2000+500
6. 密码持久化 → 主工程写死，子工程用Flash Page15(0x08003C00)存储
7. 难点/踩坑 → RFID时序延时、DHT11被中断打断、printf串口切换忘记切回

### 主动加分扩展点
- 可以增加Flash密码存储（参考key_lock子工程方案）
- RFID白名单（UID校验而非任意卡）
- 错误尝试3次锁定
- 升级到BLE远程开锁
- 引入FreeRTOS改造为多任务架构

---

## 子工程 key_lock/ 特有功能

- 状态机菜单：解锁/上锁/重设密码
- 继电器（PB1）控制门锁开合
- Flash存储密码（0x08003C00，Page15）
- 3×4键盘，逐列扫描
- HAL库，48MHz

---

## FreeRTOS+HAL 重构版本（2026-03-25）

- **生成路径**：`C:\Users\24263\Desktop\SmartLock_FreeRTOS\`
- **架构**：FreeRTOS v10.x + STM32F1xx HAL Driver
- **目录分层**：`Inc/`（全局配置）、`Drivers/`（BSP驱动层）、`App/`（应用层）、`Tasks/`（任务层）
- 原TIM2中断调度 → 软件定时器（xTimerCreate，150ms/800ms/5s）
- 全局标志位 → 任务通知（xTaskNotifyGive/ulTaskNotifyTake）+ 消息队列（xKeyQueue）
- 全局变量 → 结构体+互斥量（gSensor/gDoor/gUI + xMutex）
- DHT11时序保护：`taskENTER_CRITICAL()`替代原"关TIM2中断"
- DWT计数器实现微秒延时（DHT11、RC522共用）
- OLED移植：I2C HAL + 内置ASCII/中文字库，保留双缓冲接口
- OLED增加常用中文字库（请输入密码/菜单/状态/运行/温度/湿度/蓝牙/开锁成功等）

---

## 关键文件路径

| 文件 | 路径 |
|------|------|
| 主程序 | `Users/main.c` |
| UI逻辑 | `APP/ui.c` |
| RC522 | `APP/bsp_rc522.c` |
| 串口 | `APP/bsp_uart.c` |
| DHT11 | `APP/dht11.c` |
| 舵机 | `APP/Servo.c` |
| PWM | `APP/PWM.c` |
| 定时器 | `APP/timer.c` |
| 键盘 | `APP/matrix_button.c` |
| 蜂鸣器 | `APP/BUZZER.c` |
| 子工程主程序 | `key_lock/Core/Src/main.c` |
| 子工程菜单 | `key_lock/MDK-ARM/MENU/menu.c` |
| 子工程Flash | `key_lock/MDK-ARM/STMFLASH/stmflash.c` |
