#include "stm32f10x.h"                  // Device header


void AD2_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);       //RCC_CLK的配置(分频)
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	//ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);      (多菜单向下传，不同可以用不同采样时间（即参数四）)
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;                                //连续转换模式开关
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
}

uint16_t AD2_GetValue(uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);              //软件触发
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	return ADC_GetConversionValue(ADC1);
}

//uint16_t AD0,AD1,AD2,AD3;
//uint16_t num;

//int main(void)  
//{
//	OLED_Init();
//	AD2_Init();
//	
//	
//	OLED_ShowString(1,1,"AD0:");
//	OLED_ShowString(2,1,"AD1:");
//	OLED_ShowString(3,1,"AD2:");
//	OLED_ShowString(4,1,"AD3:");
//	
//	while(1)
//	{
//		AD0=AD2_GetValue(ADC_Channel_0);
//		AD1=AD2_GetValue(ADC_Channel_1);
//		AD2=AD2_GetValue(ADC_Channel_2);
//		AD3=AD2_GetValue(ADC_Channel_3);
//		
//		OLED_ShowNum(1,5,AD0,4);
//		OLED_ShowNum(2,5,AD1,4);
//		OLED_ShowNum(3,5,AD2,4);
//		OLED_ShowNum(4,5,AD3,4);
//		
//		Delay_ms(100);
//	}
//}

