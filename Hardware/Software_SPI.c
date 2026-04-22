#include "stm32f10x.h"                  // Device header




void SoftWare_SPI_W_CS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);
}

void SoftWare_SPI_W_SCK(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)BitValue);
}

void SoftWare_SPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)BitValue);
}

uint8_t SoftWare_SPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
}


void SoftWare_SPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	SoftWare_SPI_W_CS(1);
	SoftWare_SPI_W_SCK(0);
}

void SoftWare_SPI_START(void)
{
	SoftWare_SPI_W_CS(0);
}

void SoftWare_SPI_STOP(void)
{
	SoftWare_SPI_W_CS(1);
}

uint8_t SoftWare_SPI_mode_0_SwapByte(uint8_t ByteSend)//Swap交换
{
	//模式0
	uint8_t i,ByteReceive=0x00;
	
	for(i=0;i<8;i++)
	{
		SoftWare_SPI_W_MOSI(ByteSend & (0x80>>i));
		SoftWare_SPI_W_SCK(1);
		if(SoftWare_SPI_R_MISO()==1){ByteReceive |= (0x80>>i);}
		SoftWare_SPI_W_SCK(0);
	}
	return ByteReceive;
}

uint8_t SoftWare_SPI_mode_0_SwapByte2(uint8_t ByteSend)//Swap交换
{
	//模式0的第二种方法
	uint8_t i;
	
	for(i=0;i<8;i++)
	{
		SoftWare_SPI_W_MOSI(ByteSend & 0x80);
		ByteSend<<=1;
		SoftWare_SPI_W_SCK(1);
		if(SoftWare_SPI_R_MISO()==1){ByteSend |= 0x01;}
		SoftWare_SPI_W_SCK(0);
	}
	return ByteSend;
}

uint8_t SoftWare_SPI_mode_1_SwapByte(uint8_t ByteSend)//Swap交换
{
	//模式1
	uint8_t i,ByteReceive=0x00;
	
	for(i=0;i<8;i++)
	{
		SoftWare_SPI_W_SCK(1);
		SoftWare_SPI_W_MOSI(ByteSend & (0x80>>i));
		SoftWare_SPI_W_SCK(0);
		if(SoftWare_SPI_R_MISO()==1){ByteReceive |= (0x80>>i);}
	}
	return ByteReceive;
}

uint8_t SoftWare_SPI_mode_2_SwapByte(uint8_t ByteSend)//Swap交换
{
	//模式2
	uint8_t i,ByteReceive=0x00;
	
	for(i=0;i<8;i++)
	{
		SoftWare_SPI_W_MOSI(ByteSend & (0x80>>i));
		SoftWare_SPI_W_SCK(0);
		if(SoftWare_SPI_R_MISO()==1){ByteReceive |= (0x80>>i);}
		SoftWare_SPI_W_SCK(1);
	}
	return ByteReceive;
}

uint8_t SoftWare_SPI_mode_3_SwapByte(uint8_t ByteSend)//Swap交换
{
	//模式3
	uint8_t i,ByteReceive=0x00;
	
	for(i=0;i<8;i++)
	{
		SoftWare_SPI_W_SCK(0);
		SoftWare_SPI_W_MOSI(ByteSend & (0x80>>i));
		SoftWare_SPI_W_SCK(1);
		if(SoftWare_SPI_R_MISO()==1){ByteReceive |= (0x80>>i);}
	}
	return ByteReceive;
}

