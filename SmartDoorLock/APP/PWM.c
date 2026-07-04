#include "PWM.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);       //开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);     //开启时钟	
	
	GPIO_InitTypeDef GPIO_InitStructure;				//配置输出PWM的GPIO
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;   //选择复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);					//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;         //分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;     //选择向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=20000-1;            //ARR自动重装值
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;     //PSC预分频值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;       //设置比较的模式
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;     //设置输出计较的极性
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;     //设置输出使能
	TIM_OCInitStructure.TIM_Pulse=0;				//设置CCR的值
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);

	TIM_Cmd(TIM3,ENABLE);     //启动定时器


}

/*
	CK_PSC=72M=72000000
	PWM频率：Freq=CK_PSC/(PSC+1)/(ARR+1)
	PWM占空比：Duty=CCR/(ARR+1)
	PWM分辨率：Reso=1/(ARR+1)
*/

void PWM_SetCompare2(uint16_t Compare)  //改变CCR值的函数，从而不断的改变占空比
{
	TIM_SetCompare2(TIM3,Compare);

}










