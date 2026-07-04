#ifndef _SMOKE_H_
#define _SMOKE_H_

#include "stm32f10x.h"

/************** 用户可修改引脚定义 ****************/
#define RCU_SMOKE    RCC_APB2Periph_GPIOA
#define PORT_SMOKE   GPIOA
#define GPIO_SMOKE   GPIO_Pin_0

// 读取烟雾传感器数字输出电平（0=安全，1=检测到烟雾）
#define SMOKE_DO_IN  GPIO_ReadInputDataBit(PORT_SMOKE, GPIO_SMOKE)

extern uint8_t smoke_detected;   // 1=检测到烟雾
extern uint8_t smoke_alarm;      // 1=报警状态（可手动解除）

void Smoke_GPIO_Init(void);
uint8_t Smoke_Read(void);

#endif
