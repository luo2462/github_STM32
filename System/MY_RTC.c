#include "stm32f10x.h"                  // Device header
#include <time.h>



uint16_t MyRTC_Time[] = {2026, 4, 19, 16, 32, 0};
void MY_RTC_SetTime(void);

void MY_RTC_Init(uint8_t RTCCLK_mode)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);//PWR备份访问控制
	
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)			//通过写入备份寄存器的标志位，判断RTC是否是第一次配置
															//if成立则执行第一次的RTC配置
	{
		switch(RTCCLK_mode)
		{
			case 1://HSE/128外部高速时钟
				RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);
				break;
			case 2://LSE外部低速时钟
				RCC_LSEConfig(RCC_LSE_ON);
				while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == SET);
				RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
				break; 
			case 3://LSI内部低速时钟
				RCC_LSICmd(ENABLE);
				while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == SET);
				RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
				break;
			default:
				return;
		}
		RCC_RTCCLKCmd(ENABLE);
		
		RTC_WaitForSynchro();//等待同步
		RTC_WaitForLastTask();//等待上一次操作完成
		//RTCCLK部分
		
		//配置预分频器
		switch(RTCCLK_mode)
		{
			case 1://HSE/128外部高速时钟
				RTC_SetPrescaler(62500-1);
				break;
			case 2://LSE外部低速时钟
				RTC_SetPrescaler(32768-1);
				break;
			case 3://LSI内部低速时钟
				RTC_SetPrescaler(40000-1);
				return;
			default:
				return;
		}
		RTC_WaitForLastTask();//等待上一次操作完成
		
		MY_RTC_SetTime();
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);			//在备份寄存器写入自己规定的标志位，用于判断RTC是不是第一次执行配置
	}
	else													//RTC不是第一次配置
	{
		RTC_WaitForSynchro();								//等待同步
		RTC_WaitForLastTask();								//等待上一次操作完成
	}
}

void MY_RTC_SetTime(void)
{
	time_t time_cnt;
	struct tm time_data;
	
	time_data.tm_year=MyRTC_Time[0]-1900;
	time_data.tm_mon=MyRTC_Time[1]-1;
	time_data.tm_mday=MyRTC_Time[2];
	time_data.tm_hour=MyRTC_Time[3];
	time_data.tm_min=MyRTC_Time[4];
	time_data.tm_sec=MyRTC_Time[5];
	
	time_cnt=mktime(&time_data);
	RTC_SetCounter(time_cnt);
	RTC_WaitForLastTask();//等待上一次操作完成
}

void MyRTC_ReadTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_cnt = RTC_GetCounter() + 8 * 60 * 60;		//读取RTC的CNT，获取当前的秒计数器

	time_date = *localtime(&time_cnt);				//使用localtime函数，将秒计数器转换为日期时间格式
	
	MyRTC_Time[0] = time_date.tm_year + 1900;		//将日期时间结构体赋值给数组的时间
	MyRTC_Time[1] = time_date.tm_mon + 1;
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;
	MyRTC_Time[5] = time_date.tm_sec;
}

