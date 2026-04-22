#include "stm32f10x.h"                  // Device header



void Hardware_SPI_W_CS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitValue);
}

void HardWare_SPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//通用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;//波特率预分频器
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;//(时钟相位)高低电平进行模式选择
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;//(时钟极性)高低电平进行模式选择
	SPI_InitStructure.SPI_CRCPolynomial=7;
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;//数据长度
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//数据传输方向和模式
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;//不是第一字节,而是配置高位先行/低位先行
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;//模式
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
	SPI_Init(SPI1,&SPI_InitStructure);
	
	SPI_Cmd(SPI1,ENABLE);
	
	Hardware_SPI_W_CS(1);//默认为高电平
}

void HardWare_SPI_START(void)
{
	Hardware_SPI_W_CS(0);
}

void HardWare_SPI_STOP(void)
{
	Hardware_SPI_W_CS(1);
}

uint8_t HardWare_SPI_SwapByte(uint8_t ByteSend)//Swap交换
{
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET);
	SPI_I2S_SendData(SPI1,ByteSend);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET);
	return SPI_I2S_ReceiveData(SPI1);
}

// SPI模式切换函数：传入mode 0/1/2/3
void Hardware_SPI_Switch_Mode(uint8_t spi_mode)
{
    SPI_InitTypeDef  SPI_InitStructure;
    
    //先关闭SPI外设，才能修改时序配置
    SPI_Cmd(SPI1, DISABLE);  

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // 全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;        // 主机模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;    // 8位数据
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;            // 软件片选
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; // 分频不变
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;   // 高位先行不变

    switch(spi_mode)
    {
        case 0: // Mode0
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
            break;
        case 1: // Mode1
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
            break;
        case 2: // Mode2
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
            break;
        case 3: // Mode3
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
            break;
    }
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
	Hardware_SPI_W_CS(1);//默认为高电平
}
