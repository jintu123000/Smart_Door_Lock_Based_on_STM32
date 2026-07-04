#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f10x.h"


#define BUZZER_RCC        RCC_APB2Periph_GPIOA
#define BUZZER_GPIO_PIN1   GPIO_Pin_5 
#define BUZZER_GPIO       GPIOA

void BUZZER_Init(void);
void BUZZER_ON(void);
void BUZZER_OFF(void);
void BUZZER_turn(void);

#endif
