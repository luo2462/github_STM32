#include "stm32f10x.h"                  // Device header

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600;  //波特率设置
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;  //硬件流控制
	USART_InitStructure.USART_Mode=USART_Mode_Tx;  //串口模式，TX发送模式，RX接受模式，如果两种都有，可以用 | (或符号)像pin口一样
	USART_InitStructure.USART_Parity=USART_Parity_No;  //校验位，Odd奇校验，even偶校验，no无校验
	USART_InitStructure.USART_StopBits=USART_StopBits_1;  //停止位，设定停止位数目，一般为1
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;  //字长，即数据位，通常，有校验位9，无校验位8
	USART_Init(USART1,&USART_InitStructure);
}
