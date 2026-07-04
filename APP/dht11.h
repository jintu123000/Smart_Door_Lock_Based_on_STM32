/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：https://oshwhub.com/forum
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-29     LCKFB-LP    first version
 */
#ifndef _BSP_DHT11_H_
#define _BSP_DHT11_H_

#include "stm32f10x.h"

 /**************引脚修改此处****************/
#define RCU_DHT11   RCC_APB2Periph_GPIOC
#define PORT_DHT11  GPIOC
#define GPIO_DHT11  GPIO_Pin_13

//设置DHT11输出高或低电平
#define DATA_GPIO_OUT(x)    GPIO_WriteBit(PORT_DHT11, GPIO_DHT11, x ? Bit_SET : Bit_RESET)
//获取DHT11数据引脚高低电平状态
#define DATA_GPIO_IN        GPIO_ReadInputDataBit(PORT_DHT11, GPIO_DHT11)

extern float temperature;
extern float humidity;

extern uint8_t Temperature;
extern uint8_t Humidity;

void DHT11_GPIO_Init(void);//引脚初始化
unsigned int DHT11_Read_Data(void);//读取模块数据
float Get_temperature(void);//返回读取模块后的温度数据
float Get_humidity(void);//返回读取模块后的湿度数据
void read_data(void);

#endif


