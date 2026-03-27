#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

//USART通信

char Serial_RxTextPacket[100];   //接收的文本数据包
uint8_t Serial_TxPacket[4];  //发送的Hex数组
uint8_t Serial_RxPacket[4];	 //定义接收Hex数据包数组
uint8_t Serial_RxData;
uint8_t Serial_RxFlag;
uint8_t RxModeState=0;//0无模式，1 Hex，2 Text

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600;                                        //波特率设置
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;   //硬件流控制
	USART_InitStructure.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;                   //串口模式，TX发送模式，RX接受模式，如果两种都有，可以用 | (或符号)像pin口一样
	USART_InitStructure.USART_Parity=USART_Parity_No;                               //校验位，Odd奇校验，even偶校验，no无校验
	USART_InitStructure.USART_StopBits=USART_StopBits_1;                            //停止位，设定停止位数目，一般为1
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;                       //字长，即数据位，通常，有校验位9，无校验位8
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //依旧优先级配置
	
	NVIC_InitTypeDef NVIC_InitStructure;  //NVIC配置
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}//发送字节

void Serial_SendArray(uint8_t *Array,uint16_t Length)
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(Array[i]);
	}
}//发送一个数组

void Serial_SendString(char *String)
{
		uint16_t i;
		for(i=0;String[i] != '\0';i++)    //String[i]!='\0'   和  String[i]!=0  效果一样
		{
			Serial_SendByte(String[i]);
		}
}//遍历发送字符串

uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result=1;
	while(Y --)
	{
		Result *=X;
	}
	return Result;
}//通过循环累乘实现幂运算

void Serial_SendNumber(uint32_t Number,uint8_t Length)
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(Number / Serial_Pow(10,Length-1-i)%10+'0');
	}
}//发送数字

int fputc(int ch,FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}//用printf函数重定向
//如果多个串口都想用printf函数时
//可以在main.c中用sprintf
//以下为示例
//char string[n];   n为长度，给够就行，100
//sprintf(string,"要发送的数据");
//Serial_SendString(string);

void Serial_Printf(char *format,...)
{
	char string[100];
	va_list arg;//定义一个参数列表变量
	va_start(arg,format);//从format位置开始接受参数表，放在arg中
	vsprintf(string,format,arg);//打印位置是string,格式化字符串是format,参数表是arg
	va_end(arg);//释放arg
	Serial_SendString(string);
}//发送中文有两种方式，编码UTF-8和GB2312，保存关闭再打开


uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}//返回最近一次收到的字节数据(单数据)


void Serial_RxHexStep(uint8_t RxHexData)
{
  static uint8_t RxHexState=0;//静态变量，函数进入只会初始化一次0;
  //函数退出后仍然有效果，不过只能在本函数中使用(os:感觉就是专门用于中断，进入时可以读取上一次残留数据)
  static uint8_t pRxPacket=0;
  if (RxHexState==0)
  {//等待包头
    if (RxHexData==0xFF&&Serial_RxFlag==0)
    {
      RxHexState=1;
      pRxPacket=0;
    }
  }
  else if(RxHexState==1)
  {//接收数据
    Serial_RxPacket[pRxPacket]=RxHexData;
    pRxPacket++;
    if (pRxPacket>=4)
    {
      pRxPacket=0;
      RxHexState=2;
    }
  }
  else if(RxHexState==2)
  {//等待包尾
    if (RxHexData==0xFE)
    {
      RxHexState=0;
      Serial_RxFlag=1;
      RxModeState=0;//模式回0
    }
  }
}
void Serial_RxTextStep(uint8_t RxTextData)
{
  static uint8_t RxTextState=0;//静态变量，函数进入只会初始化一次0;
  //函数退出后仍然有效果，不过只能在本函数中使用(os:感觉就是专门用于中断，进入时可以读取上一次残留数据)
  static uint8_t pRxTextPacket=0;
  if (RxTextState==0)
  {//等待包头
	  if (RxTextData=='@'&& Serial_RxFlag == 0)
		{
		  RxTextState=1;
		  pRxTextPacket=0;
		}
  }
  else if(RxTextState==1)
  {//接收数据
	if (RxTextData == '\r')
	{
		RxTextState = 2;
	}
	else
		{
			Serial_RxTextPacket[pRxTextPacket] = RxTextData;
			pRxTextPacket ++;
		}
  }
  else if(RxTextState==2)
  {//等待包尾
    if (RxTextData=='\n')
    {
      RxTextState=0;
      Serial_RxTextPacket[pRxTextPacket] = '\0';			//将收到的字符数据包添加一个字符串结束标志
      Serial_RxFlag=1;
      RxModeState=0;//模式回0
    }
  }
}


void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		uint8_t RxData=USART_ReceiveData(USART1);
		if (RxModeState==0)
		{
		  if(RxData==0xFF)
		  {
			RxModeState=1;
			Serial_RxHexStep(RxData);
		  } 
		  else if(RxData=='@') 
			  {
				  RxModeState=2;
				  Serial_RxTextStep(RxData);
			  }
		}
		else if (RxModeState == 1)
		{
			Serial_RxHexStep(RxData);
		}
		else if (RxModeState == 2)
			{
			  Serial_RxTextStep(RxData);
			}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}//USART的中断

void Serial_SendPacket(void)
{
	Serial_SendByte(0xFF);//发送包头，自己设得，Up选这样的也还不错
	Serial_SendArray(Serial_TxPacket,4);
	Serial_SendByte(0xFE);//发送包尾，自己设得，Up选这样的也还不错
}//发送数据包
