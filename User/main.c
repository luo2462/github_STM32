#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "Countsensor.h"
#include "Encoder.h"
#include "Timer.h"
#include "PWM.h"
#include "Servo.h"
#include "Motor.h"
#include "IC.h"
#include "Encoder_Speed.h"
#include "AD.h"
#include "AD2.h"
#include "myDMA.h"
#include "Serial.h"
#include <string.h>//c语言一个标准库，可调用strcmp等函数,strcmp可用于对比字符串是否相同,即==0
#include "Software I2C.h"


uint16_t num;
uint8_t KeyNum;



int main(void)  
{
	OLED_Init();
	Key_Init();
	SOFTWAER_I2C_READS_MPU6050_Init();

	SOFTWAER_I2C_READS_MPU6050_Start();
	SOFTWAER_I2C_READS_MPU6050_SendByte(0xD0);
	uint8_t ACK=SOFTWAER_I2C_READS_MPU6050_ReceiveACK();
	SOFTWAER_I2C_READS_MPU6050_Stop();
	OLED_ShowNum(1,1,ACK,2);
	
	while (1)
	{
		
	}
}
