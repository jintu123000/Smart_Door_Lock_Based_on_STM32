#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f0xx.h"
#include "main.h"

u8 col_port_scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_PIN);
u8 row_port_scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_PIN);
u8 col_scan(void);
u8 row_scan(u8 col_return);
u8 key_scan(void);

#endif
