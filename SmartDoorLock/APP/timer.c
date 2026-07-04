#include "timer.h"


void timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);       //开启时钟
	TIM_InternalClockConfig(TIM2);          //开启内部时钟---可以不写这一行，定时器上电默认使用的就是内部时钟
	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;         //分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;     //选择向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=100-1;            //自动重装值
	TIM_TimeBaseInitStructure.TIM_Prescaler=7200-1;     //预分频值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);    //避免一初始化完成就进入中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);     //开启更新中断到NVIC的通路
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //选择响应级
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;  //指定通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;  //设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;  //设置响应优先级
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM2,ENABLE);     //启动定时器

}









