#include "BUZZER.h"

void BUZZER_Init(void)
{
	RCC_APB2PeriphClockCmd(BUZZER_RCC, ENABLE);        //开启时钟
	GPIO_InitTypeDef GPIO_InitStructure;      //建立结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //选择上拉输出
	GPIO_InitStructure.GPIO_Pin = BUZZER_GPIO_PIN1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BUZZER_GPIO, &GPIO_InitStructure);
	BUZZER_OFF();
}

/**
 * @brief 开启蜂鸣器
 * 
 * 该函数通过将指定的 GPIO 引脚设置为高电平来启动蜂鸣器。
 * 
 * @param 无
 * @return 无
 */
void BUZZER_ON(void)
{
	// 设置蜂鸣器对应的 GPIO 引脚为高电平
	GPIO_SetBits(BUZZER_GPIO,BUZZER_GPIO_PIN1);
}


void BUZZER_OFF(void)  //关
{
	GPIO_ResetBits(BUZZER_GPIO,BUZZER_GPIO_PIN1);
}

/**
 * @brief 切换蜂鸣器引脚的输出电平状态
 * 
 * 该函数读取蜂鸣器控制引脚当前的输出电平，并将其翻转。
 * 若当前为低电平则置为高电平，若当前为高电平则置为低电平，
 * 从而实现蜂鸣器状态的切换（如开启/关闭或音调变化）。
 * 
 * @param 无
 * @return 无
 */
void BUZZER_turn(void)    
{
	// 根据当前引脚电平状态执行翻转操作
	if(GPIO_ReadOutputDataBit(BUZZER_GPIO, BUZZER_GPIO_PIN1)==0)
	{
		GPIO_SetBits(BUZZER_GPIO,BUZZER_GPIO_PIN1);
	}
	else
	{
		GPIO_ResetBits(BUZZER_GPIO,BUZZER_GPIO_PIN1);
	}

}





