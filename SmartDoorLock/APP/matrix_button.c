#include "matrix_button.h"

//行
GPIO_TypeDef* port_row[4] = {PORT_IN, PORTB_IN, PORTB_IN, PORTB_IN};
uint32_t gpio_row[4]      = {GPIO_IN8,GPIO_IN7,GPIO_IN6,GPIO_IN5};


//列
GPIO_TypeDef* port_col[4] = {PORTB_IN, PORTB_IN, PORT_IN, PORT_IN};
uint32_t gpio_col[4]      = {GPIO_IN4,GPIO_IN3,GPIO_IN2,GPIO_IN1};


/**
 * @brief 初始化矩阵键盘所需的 GPIO 引脚配置
 * 
 * 该函数配置了用于矩阵键盘扫描的行线和列线引脚。
 * 具体配置如下：
 * - 行线 (GPIO_IN5, GPIO_IN6, GPIO_IN7, GPIO_IN8): 配置为推挽输出模式，用于主动驱动电平。
 * - 列线 (GPIO_IN1, GPIO_IN2, GPIO_IN3, GPIO_IN4): 配置为上拉输入模式，用于检测按键闭合后的电平变化。
 * 
 * @param 无
 * @retval 无
 */
void MatrixKey_GPIO_Init(void)
{
        GPIO_InitTypeDef  GPIO_InitStructure;
        // 使能相关 GPIO 端口的时钟
        RCC_APB2PeriphClockCmd (RCC_IN_GPIO|RCC_IN_GPIOB, ENABLE);

		GPIO_StructInit(&GPIO_InitStructure);
        // 配置端口 B 上的行线引脚为推挽输出
        GPIO_InitStructure.GPIO_Pin = GPIO_IN5|GPIO_IN6|GPIO_IN7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(PORTB_IN, &GPIO_InitStructure);
		
		GPIO_StructInit(&GPIO_InitStructure);
        // 配置端口 A 上的行线引脚为推挽输出
        GPIO_InitStructure.GPIO_Pin = GPIO_IN8;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(PORT_IN, &GPIO_InitStructure);
	
		GPIO_StructInit(&GPIO_InitStructure);
        // 配置端口 A 上的部分列线引脚为上拉输入
        GPIO_InitStructure.GPIO_Pin = GPIO_IN1|GPIO_IN2;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(PORT_IN, &GPIO_InitStructure);
	
		GPIO_StructInit(&GPIO_InitStructure);
        // 配置端口 B 上的剩余列线引脚为上拉输入
        GPIO_InitStructure.GPIO_Pin = GPIO_IN3|GPIO_IN4;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(PORTB_IN, &GPIO_InitStructure);
}

uint8_t key_scan(void)
{
    uint8_t i, j, key_val = 0;

    for (i = 0; i < 4; i++)
    {
        // 选中第i行，输出低电平
        GPIO_ResetBits(port_row[i], gpio_row[i]);

        // 检测列的状态
        for (j = 0; j < 4; j++)
        {
            // 若列被拉低（按键按下）
            if (!GPIO_ReadInputDataBit(port_col[j], gpio_col[j]))
            {
                key_val = 16 - i - j * 4;  
				
                break;
            }
        }

        // 恢复第i行的高电平状态
        GPIO_SetBits(port_row[i], gpio_row[i]);

        if (key_val) break;
    }
    return key_val;
}
