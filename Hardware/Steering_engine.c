#include "stm32f10x.h"                  // Device header


//这是Servo的PWM模式
//Steering engine是转向发动机（舵机）

void PWM2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);             //AFIO和重映射，如果重映射改下方端口和模式等等参数
//	
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                           
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=20000-1;                    //ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;                  //PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;                             //CCR
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	
	TIM_Cmd(TIM2,ENABLE);

}

void PWM2_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2,Compare);
}


//uint8_t KeyNum;
//float Angle;

//int main(void)  
//{
//	
//	OLED_Init();
//	Key_Init();
//	Servo_Init();
//	
//	OLED_ShowString(1,1,"Angle:");
//	
//	while(1)
//	{
//		KeyNum=Key_GetNum();
//		if(KeyNum==1)
//		{
//			Angle+=30;
//			if(Angle>180)
//			{
//				Angle=0;
//			}
//		}
//		Servo_SetAngle(Angle);
//		OLED_ShowNum(1,7,Angle,3);
//	}
//}这是主函数用法
//先获取键值判断，然后计算500(0)~2500(180)转换计算交给电脑实施
