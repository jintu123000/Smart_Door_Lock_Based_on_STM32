#ifndef __MENU_H
#define __MENU_H
#include "main.h"
u8 bit_view(u8 view_data, u8 view_bit);
u8 depth_view(u8 view_data);
u8 menu_entermsp(u8 state, u8 depth);
u8 menu_entercallback(u8 state_in, u8 depth_in);
u8 menu_statemachine(u8 current_state, u8 input);
#endif
