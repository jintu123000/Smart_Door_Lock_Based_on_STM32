#ifndef _UI_H
#define _UI_H

#include "stm32f10x.h"      


extern uint8_t Key_num;

extern uint8_t input_key[6];

extern uint8_t smoke_alarm;


void main_ui(void);
void Host_state(void);
void ui(void);



#endif

