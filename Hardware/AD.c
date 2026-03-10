#include "stm32f10x.h"                  // Device header

//模数转换器（DAC是数模转换器）

void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);       //RCC_CLK的配置(分频)
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);      (多菜单向下传，不同可以用不同采样时间（即参数四）)
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;                                //连续转换模式开关
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;                          //数据对齐（左右）
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;              //触发模式选择（这里是不用外部触发，即内部软件触发）
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;            //ADC模式（当前为独立模式，只有这一个参数代表独立模式，剩下都是双ADC）
	ADC_InitStructure.ADC_NbrOfChannel=1;                        //通道数目（扫描模式下）即菜单
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;                       //扫描转换模式开关
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);
	
	//校准
	ADC_ResetCalibration(ADC1);            
	while (ADC_GetResetCalibrationStatus(ADC1)==SET);    //用循环等待校准完成
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1)==SET);         //用循环等待校准完成
	
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);              //软件触发
}

uint16_t AD_GetValue(void)
{
	//while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	return ADC_GetConversionValue(ADC1);
}

//float V;
//uint16_t ADvalue;
//uint16_t num;

//int main(void)  
//{
//	OLED_Init();
//	AD_Init();
//	
//	
//	OLED_ShowString(1,1,"Value:");
//	OLED_ShowString(2,1,"V:");
//	
//	while(1)
//	{
//		ADvalue=AD_GetValue();
//		V=(float)ADvalue / 4095*(3.3);
//		
//		OLED_ShowNum(1,7,ADvalue,4);
//		OLED_ShowNum(2,7,V,1);
//		OLED_ShowNum(2,9,(uint16_t)(V*100)%100,2);
//		Delay_ms(100);
//	}
//}
