#include "smoke.h"
#include "Delay.h"

uint8_t smoke_detected = 0;   // 1=检测到烟雾
uint8_t smoke_alarm    = 0;   // 1=报警状态（可手动解除）

/**
 * @brief 烟雾传感器 GPIO 初始化
 *
 * 配置 PA0 为上拉输入模式，MQ-2 数字输出在未检测到烟雾时为低电平，
 * 检测到烟雾时输出高电平。上拉电阻确保悬空时电平确定（低电平=安全）。
 */
void Smoke_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCU_SMOKE, ENABLE);

    GPIO_InitStructure.GPIO_Pin  = GPIO_SMOKE;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PORT_SMOKE, &GPIO_InitStructure);

    smoke_detected = 0;
    smoke_alarm    = 0;
}

/**
 * @brief 读取烟雾传感器状态（带软件消抖）
 *
 * 连续 3 次读到高电平才确认检测到烟雾，避免误报。
 *
 * @return uint8_t  1=检测到烟雾，0=安全
 */
uint8_t Smoke_Read(void)
{
    uint8_t confirm_cnt = 0;
    uint8_t i;

    for (i = 0; i < 3; i++)
    {
        if (SMOKE_DO_IN == Bit_SET)
        {
            confirm_cnt++;
        }
        Delay_ms(10);   // 每次读取间隔 10ms，总共消抖 30ms
    }

    if (confirm_cnt >= 3)
    {
        return 1;   // 确认检测到烟雾
    }
    return 0;
}
