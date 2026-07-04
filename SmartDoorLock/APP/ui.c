#include "ui.h"
#include "OLED.h"
#include "dht11.h"
#include "matrix_button.h"
#include "bsp_uart.h"
#include "smoke.h"
#include <string.h>
#include <stdio.h>

uint8_t Key_num=0;
uint8_t role=0;
uint32_t key_count=0;

uint8_t door_key[6]={1,2,3,4,5,6};
uint8_t input_key[6];

uint8_t hua_kuang;
uint8_t success_door_count;
uint8_t cai_dan;

void key_role(void);
static uint8_t arrays_equal(const uint8_t *arr_1, const uint8_t *arr_2, uint8_t len);

void ui(void)
{
	if(Key_num==13)
	{
		OLED_Clear();
		cai_dan=0;
	}
	if(Key_num==16)
	{
		OLED_Clear();
		cai_dan=1;
	}
	if(cai_dan==0)
	{
		main_ui();
		key_role();
	}
	if(cai_dan==1)
	{
		Host_state();
	}

}



void main_ui(void)
{

//	OLED_ShowString(0,0,"                ",OLED_8X16);
//	OLED_ShowString(0,16,"                ",OLED_8X16);
	OLED_ShowString(0,0,"Input Password:",OLED_8X16);
	// 菜单
	OLED_ShowString(0, 48, "Menu", OLED_8X16);

	// 状态
	OLED_ShowString(96, 48, "Status", OLED_8X16);
//	OLED_DrawRectangle(0, 48, 32, 16, OLED_UNFILLED);
//	if(Key_num==13)
//	{
//		hua_kuang++;
//		if(hua_kuang>2)
//		{
//			hua_kuang=0;
//		}
//	}
//	if(hua_kuang==1)
//	{
//		OLED_DrawRectangle(0, 48, 32, 16, OLED_UNFILLED);
//	}
//	if(hua_kuang==2)
//	{
//		OLED_DrawRectangle(96, 48, 32, 16, OLED_UNFILLED);
//	}
	// 确认
//	OLED_DrawRectangle(0, 48, 32, 16, OLED_UNFILLED);
//	OLED_DrawRectangle(96, 48, 32, 16, OLED_UNFILLED);

}

void Host_state(void)
{
	// 主机状态
	OLED_ShowString(0,0,"  Host Status   ",OLED_8X16);

	// 温度
	OLED_ShowString(0,16,"Temp:",OLED_8X16);
	OLED_ShowNum(40,16,(Temperature/100)%10,1,OLED_8X16);
	OLED_ShowNum(48,16,(Temperature/10)%10,1,OLED_8X16);
	OLED_ShowChar(56,16,'.',OLED_8X16);
	OLED_ShowNum(64,16,Temperature%10,1,OLED_8X16);
	OLED_ShowString(72,16,"       ",OLED_8X16);

	// 湿度
	OLED_ShowString(0,32,"Humidity:",OLED_8X16);
	OLED_ShowNum(40,32,Humidity,2,OLED_8X16);
	OLED_ShowChar(56,32,'%',OLED_8X16);
	OLED_ShowString(64,32,"       ",OLED_8X16);

	// 报警状态显示
	if (smoke_alarm) {
		OLED_ShowString(0,48,"FIRE ALARM! SMOKE!",OLED_8X16);
	} else {
		OLED_ShowString(0,48,"Alarm:Not Active   ",OLED_8X16);
	}

}

void key_role(void)
{
	key_count++;
	if (Key_num >= 1 && Key_num <= 9 && role < 6)
	{
		input_key[role] = Key_num;
		OLED_ShowNum(role * 8, 16, Key_num, 1, OLED_8X16);
		Key_num=0;
		role++;
	}

	if(Key_num==10 && role!=0)
	{
		Key_num=0;
		role--;
		OLED_ShowChar(role*8,16,' ',OLED_8X16);
	}
	if(Key_num==11 && role<6)
	{
		Key_num=0;
		OLED_ShowNum(role*8,16,0,1,OLED_8X16);
		role++;
	}

	if(key_count<2 && role!=0 && role<6)
	{
		OLED_ShowChar((role)*8,16,'_',OLED_8X16);
	}
	if(key_count>1 && key_count<3 && role!=0 && role<6)
	{
		OLED_ShowChar((role)*8,16,' ',OLED_8X16);
	}
	if(key_count>2)
	{
		key_count=0;
	}
//	OLED_ShowString(0,32,"                ",OLED_8X16);
	// 比较密码
	if(success_door_count>200)
	{
		input_key[0]=0;
		input_key[1]=0;
		input_key[2]=0;
		input_key[3]=0;
		input_key[4]=0;
		input_key[5]=0;
		set_printf_usart(PRINTF_USART3);
		printf("<G>Unlock OK");
		set_printf_usart(PRINTF_USART1);
		printf("Unlock OK");
		success_door_count=0;
	}
	if (arrays_equal(door_key, input_key, 6))
	{
		role=0;
		success_door_count++;
//		input_key[3]=0;
		OLED_ShowString(0,32,"   Unlock OK",OLED_8X16);
		OLED_ShowString(0,16,"                ",OLED_8X16);

	}
	else
	{
		OLED_ShowString(0,32,"                ",OLED_8X16);
	}
}

static uint8_t arrays_equal(const uint8_t *arr_1, const uint8_t *arr_2, uint8_t len)
{
	for (uint8_t i = 0; i < len; i++)
	{
		if (arr_1[i] != arr_2[i])
		{
			return 0;
		}
	}
	return 1;
}
