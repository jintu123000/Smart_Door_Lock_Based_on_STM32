#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "BUZZER.h"
#include "bsp_uart.h"
#include <string.h>
#include <stdio.h>
#include "dht11.h"
#include "bsp_rc522.h"
#include "matrix_button.h"
#include "timer.h"
#include "ui.h"
#include "Servo.h"
#include "smoke.h"

uint16_t temp;
uint16_t dity;


uint16_t dht11_read_cnt=0;
uint16_t OLED_read_cnt=0;

uint8_t dht11_read_sign=0;
uint8_t OLED_new_sign=0;
uint8_t RC55_new_sign=0;

uint16_t key_num_read_cnt=0;
uint16_t smoke_read_cnt=0;
uint8_t  smoke_read_sign=0;
/* 卡片ID存储, 32位, 4字节 */
u8 ucArray_ID [ 4 ];
uint8_t ucStatusReturn;    // 返回状态





int main(void)
{

	int i = 0;
	uint8_t read_write_data[16]={0};// 读写数据缓存
    uint8_t card_KEY[6] ={0xff,0xff,0xff,0xff,0xff,0xff};// 默认密钥

	BUZZER_Init();
//	BUZZER_ON();

	uart1_init(115200);
	set_printf_usart(PRINTF_USART1);
	printf("USART1 Init OK\r\n");
	uart3_init(9600);
	printf("USART3 Init OK\r\n");

	printf("OLED Init...\r\n");
	OLED_Init();		// OLED初始化
	OLED_ShowString(0, 0, "welcome to use",OLED_8X16);
	OLED_ShowString(0, 16, "No write program",OLED_8X16);
	OLED_ShowString(0, 32, "...",OLED_8X16);
	OLED_Update();
	printf("OLED Init OK\r\n");

	RC522_Init( );// RC522 IO口初始化
    RC522_Rese( );// 复位RC522
	printf("RC522 Init OK\r\n");
	DHT11_GPIO_Init();
	Smoke_GPIO_Init();
	printf("Smoke Sensor Init OK\r\n");
	BUZZER_OFF();

	MatrixKey_GPIO_Init();
	printf("Keypad Init OK\r\n");

	timer_Init();
	Servo_Init();
	Servo_SetAngle(0);
	Delay_ms(1000);
	Servo_SetAngle(90);
	Delay_ms(1000);
	OLED_Clear();
	while(1)
	{
		// 显示任务
		ui();
		if(OLED_new_sign)
		{
			OLED_Update();
			OLED_new_sign=0;
		}

		// DHT11读取任务
		if(dht11_read_sign)
		{
			read_data();
			dht11_read_sign=0;
		}
		if(RC55_new_sign)
		{
			RC55_new_sign=0;
			if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )
			{
				ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );
			}

			if ( ucStatusReturn == MI_OK  )
			{
					/* 与选中的卡片通信, 存储ID到ucArray_ID */
				if ( PcdAnticoll ( ucArray_ID ) == MI_OK )
				{
					// 打印卡ID
					printf("ID: %X %X %X %X\r\n", ucArray_ID [ 0 ], ucArray_ID [ 1 ], ucArray_ID [ 2 ], ucArray_ID [ 3 ]);

					 // 选卡
					if( PcdSelect(ucArray_ID) != MI_OK )
					{    printf("PcdSelect failure\r\n");         }

					// 校验卡片密钥
					// 使用扇区6密钥B校验（默认密钥为16字节的0xff）
					if( PcdAuthState(PICC_AUTHENT1B, 6, card_KEY,  ucArray_ID) != MI_OK )
					{    printf("PcdAuthState failure\r\n");      }

					// 向数据块4写入read_write_data
					read_write_data[0] = 0xaa;// 将第一个字节改为0xaa
					if( PcdWrite(4,read_write_data) != MI_OK )
					{    printf("PcdWrite failure\r\n");          }
					else
					{
						input_key[0]=1;
						input_key[1]=2;
						input_key[2]=3;
						input_key[3]=4;
						input_key[4]=5;
						input_key[5]=6;
//						set_printf_usart(PRINTF_USART3);
//						printf("<G>Unlock OK");
//						set_printf_usart(PRINTF_USART1);
//						printf("Unlock OK");
					}

					// 清空read_write_data（16字节置0）
					memset(read_write_data,0,16);

					Delay_us(8);

					// 读取数据块4的数据
					if( PcdRead(4,read_write_data) != MI_OK )
					{    printf("PcdRead failure\r\n");           }

					// 串口打印读回的数据
					for( i = 0; i < 16; i++ )
					{
							printf("%x ",read_write_data[i]);
					}
					printf("\r\n");
				}
			}

		}

		// 烟雾传感器检测
		if(smoke_read_sign)
		{
			smoke_detected = Smoke_Read();
			if (smoke_detected)
			{
				smoke_alarm = 1;
			}
			// 按键12手动解除报警
			if (Key_num == 12 && smoke_alarm)
			{
				smoke_alarm = 0;
				smoke_detected = 0;
			}
			smoke_read_sign = 0;
		}

		// 烟雾报警蜂鸣器控制（所有页面生效）
		if (smoke_alarm)
		{
			BUZZER_ON();
		}
		else
		{
			BUZZER_OFF();
		}

	}

}


void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		key_num_read_cnt++;
		if(key_num_read_cnt>15)
		{

			Key_num=key_scan();
			key_num_read_cnt=0;
		}

		// DHT11读取定时
		dht11_read_cnt++;
		if(dht11_read_cnt>500)
		{
			dht11_read_sign=1;

			dht11_read_cnt=0;
		}
		OLED_read_cnt++;
		if(OLED_read_cnt>80)
		{
			RC55_new_sign=1;
			OLED_new_sign=1;
			OLED_read_cnt=0;
		}
		// 烟雾传感器检测定时
		smoke_read_cnt++;
		if(smoke_read_cnt>100)
		{
			smoke_read_sign=1;
			smoke_read_cnt=0;
		}
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}

}
