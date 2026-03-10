#include "stm32f10x.h"                  // Device header

//输入捕获

void IC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);    //PWM部分
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=65536-1;                    //ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;                  //PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;  
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;                    //这是通道一
	TIM_ICInitStructure.TIM_ICFilter=0xF;
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;   //直连输入
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	TIM_PWMIConfig(TIM3,&TIM_ICInitStructure);                    //初始化上述相反配置，直连换成交叉，通道一换通道二
	
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);
	
	TIM_Cmd(TIM3,ENABLE);
	
}


uint32_t IC_GetFreq(void)
{
	return 1000000/TIM_GetCapture1(TIM3);                           //频率
}

uint32_t IC_GetDuty(void)
{
	return TIM_GetCapture2(TIM3)*100/TIM_GetCapture1(TIM3);           //占空比（%）
}



//int main(void)  
//{
//	OLED_Init();
//	PWM_Init();
//	IC_Init();
//	
//	OLED_ShowString(1,1,"Freq:00000hz");
//	OLED_ShowString(2,1,"Duty:00%");
//	
//	PWM_PrescalerConfig(720-1);
//	PWM_SetCompare1(50);
//	
//	while(1)
//	{
//		OLED_ShowNum(1,6,IC_GetFreq(),5);
//		OLED_ShowNum(2,6,IC_GetDuty(),2);
//	}
//}主函数模板
