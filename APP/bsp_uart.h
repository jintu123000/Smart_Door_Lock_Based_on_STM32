/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：https://oshwhub.com/forum
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-26     LCKFB-LP    first version
 */
 
 #ifndef __BSP_UART_H__
 #define __BSP_UART_H__
 
 #include "stm32f10x.h"
 
 
 /* 串口3缓冲区的数据长度 */
#define USART3_RECEIVE_LENGTH  1024

extern uint8_t  u3_recv_buff[USART3_RECEIVE_LENGTH]; // 接收缓冲区
extern uint16_t u3_recv_length;                     // 接收数据长度
extern uint8_t  u3_recv_flag;                       // 接收完成标志位


 /* 串口1缓冲区的数据长度 */
#define USART1_RECEIVE_LENGTH  1024

extern uint8_t  u1_recv_buff[USART1_RECEIVE_LENGTH]; // 接收缓冲区
extern uint16_t u1_recv_length;                     // 接收数据长度
extern uint8_t  u1_recv_flag;                       // 接收完成标志位

 
 //外部可调用函数的声明
void uart3_init(uint32_t __Baud);
void USART3_IRQHandler(void);
void uart3_receive_clear(void);
uint8_t *uart3_get_data(void);
 
void uart1_init(uint32_t __Baud);
void USART1_IRQHandler(void);
void uart1_receive_clear(void);
uint8_t *uart1_get_data(void);

// 添加选择printf输出串口的枚举和函数声明
extern uint8_t g_printf_usart;



typedef enum {
    PRINTF_USART1 = 1,
    PRINTF_USART3 = 3
} PRINTF_USART_t; 
 
void set_printf_usart(PRINTF_USART_t usart);


#endif
