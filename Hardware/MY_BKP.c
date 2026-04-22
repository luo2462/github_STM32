#include "stm32f10x.h"                  // Device header


void MY_BKP_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);//PWR备份访问控制
}

void BKP_Write(uint16_t BKP_DR, uint16_t Data)
{
	BKP_WriteBackupRegister(BKP_DR,Data);
}

uint16_t BKP_Read(uint16_t BKP_DR)
{
	return BKP_ReadBackupRegister(BKP_DR);
}
