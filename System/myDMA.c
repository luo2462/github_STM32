#include "stm32f10x.h"                  // Device header

uint16_t MYDMA_Size;

void MYDMA_Init(uint32_t ADDrA,uint32_t ADDrB,uint16_t Size)         //注意传入参数是32,32,16
{
	MYDMA_Size=Size;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize=Size;                                  //传输次数
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;                        //传输方向
	DMA_InitStructure.DMA_M2M=DMA_M2M_Enable;                               //从哪里到哪里,存储器到存储器就是自身用软件(相当于怎么触发，ENABLE软件出发，DISABLE硬件出发)
	DMA_InitStructure.DMA_MemoryBaseAddr=ADDrB;                             //接收地址(不用自己写绝对路径)
	DMA_InitStructure.DMA_MemoryDataSize=DMA_PeripheralDataSize_Byte;       //接收长度,常为字节,半字,整字
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;                   //接收地址是否自增
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;                             //是否自动重装(可以自己看定义，和ADC一起时连续模式和重装不能同时用)
	DMA_InitStructure.DMA_PeripheralBaseAddr=ADDrA;                         //传输地址(常用传入地址,不用自己写绝对路径)
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;   //传输长度,常为字节,半字,整字
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Enable;           //传输地址是否自增
	DMA_InitStructure.DMA_Priority=DMA_Priority_VeryHigh;                   //指定当前通道优先级(极高,高,中等,低)
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1,DISABLE);
}

void MYDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);          //先失能
	DMA_SetCurrDataCounter(DMA1_Channel1,MYDMA_Size);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}
//uint16_t num;
//uint8_t KeyNum;

//uint8_t DataA[]={0x01,0x02,0x03,0x04};
//uint8_t DataB[]={0,0,0,0};

//int main(void)  
//{
//	OLED_Init();
//	MYDMA_Init((uint32_t)DataA,(uint32_t)DataB,4);
//	
//	OLED_ShowString(1,1,"DataA");
//	OLED_ShowString(3,1,"DataB");
//	OLED_ShowHexNum(1,8,(uint32_t)&DataA,8);
//	OLED_ShowHexNum(3,8,(uint32_t)&DataB,8);
//	
//	OLED_ShowHexNum(2,1,DataA[0],2);
//	OLED_ShowHexNum(2,4,DataA[1],2);
//	OLED_ShowHexNum(2,7,DataA[2],2);
//	OLED_ShowHexNum(2,10,DataA[3],2);
//	OLED_ShowHexNum(4,1,DataB[0],2);
//	OLED_ShowHexNum(4,4,DataB[1],2);
//	OLED_ShowHexNum(4,7,DataB[2],2);
//	OLED_ShowHexNum(4,10,DataB[3],2);
//	
//	while(1)
//	{
//		DataA[0]++;    数据自增而已
//		DataA[1]++;    如果想要Flash数据，在数据定义前面加上const，不过Flash数据不能更改
//		DataA[2]++;
//		DataA[3]++;
//		
//		OLED_ShowHexNum(2,1,DataA[0],2);
//		OLED_ShowHexNum(2,4,DataA[1],2);
//		OLED_ShowHexNum(2,7,DataA[2],2);
//		OLED_ShowHexNum(2,10,DataA[3],2);
//		OLED_ShowHexNum(4,1,DataB[0],2);
//		OLED_ShowHexNum(4,4,DataB[1],2);
//		OLED_ShowHexNum(4,7,DataB[2],2);
//		OLED_ShowHexNum(4,10,DataB[3],2);
//		
//		Delay_ms(1000);
//		
//		MYDMA_Transfer();
//		
//		OLED_ShowHexNum(2,1,DataA[0],2);
//		OLED_ShowHexNum(2,4,DataA[1],2);
//		OLED_ShowHexNum(2,7,DataA[2],2);
//		OLED_ShowHexNum(2,10,DataA[3],2);
//		OLED_ShowHexNum(4,1,DataB[0],2);
//		OLED_ShowHexNum(4,4,DataB[1],2);
//		OLED_ShowHexNum(4,7,DataB[2],2);
//		OLED_ShowHexNum(4,10,DataB[3],2);
//		
//		Delay_ms(1000);
//	}
//}
