#ifndef _MATRIX_BUTTON_H_
#define _MATRIX_BUTTON_H_

#include "stm32f10x.h"

#define RCC_IN_GPIO     RCC_APB2Periph_GPIOA
#define RCC_IN_GPIOB     RCC_APB2Periph_GPIOB

#define PORT_IN         GPIOA

#define PORTB_IN         GPIOB

#define GPIO_IN1        GPIO_Pin_11

#define GPIO_IN2        GPIO_Pin_12

#define GPIO_IN3        GPIO_Pin_5

#define GPIO_IN4        GPIO_Pin_6

#define GPIO_IN5        GPIO_Pin_7

#define GPIO_IN6        GPIO_Pin_1

#define GPIO_IN7        GPIO_Pin_0

#define GPIO_IN8        GPIO_Pin_7

void MatrixKey_GPIO_Init(void);
uint8_t key_scan(void);


#endif

