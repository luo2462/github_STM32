#include "stm32f10x.h"                  // Device header
#include "Software I2C.h" 


#define MPU6050_Address 0xD0

void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)
{
	SOFTWAER_I2C_READS_MPU6050_Start();
	SOFTWAER_I2C_READS_MPU6050_SendByte(MPU6050_Address);//指定从机地址
	SOFTWAER_I2C_READS_MPU6050_ReceiveACK();
	SOFTWAER_I2C_READS_MPU6050_SendByte(RegAddress);//指定寄存器地址
	SOFTWAER_I2C_READS_MPU6050_ReceiveACK();
	SOFTWAER_I2C_READS_MPU6050_SendByte(Data);//指定发送数据（1）
	SOFTWAER_I2C_READS_MPU6050_ReceiveACK();//（2）
	//这里可以用for循环发送数据(1)(2)
	SOFTWAER_I2C_READS_MPU6050_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	SOFTWAER_I2C_READS_MPU6050_Start();
	SOFTWAER_I2C_READS_MPU6050_SendByte(MPU6050_Address);//指定从机地址
	SOFTWAER_I2C_READS_MPU6050_ReceiveACK();
	SOFTWAER_I2C_READS_MPU6050_SendByte(RegAddress);//指定寄存器地址
	SOFTWAER_I2C_READS_MPU6050_ReceiveACK();
	/*设定完成
	这一段是写的时序
	转入读的时序，需要重新制定读取位
	*/
	
	SOFTWAER_I2C_READS_MPU6050_Start();
	SOFTWAER_I2C_READS_MPU6050_SendByte(MPU6050_Address | 0x01);//"|"或:有1为1，同0为0
	SOFTWAER_I2C_READS_MPU6050_ReceiveACK();
	uint8_t Data=SOFTWAER_I2C_READS_MPU6050_ReceiveByte();
	SOFTWAER_I2C_READS_MPU6050_SendACK(1);//给1停止应答，给0继续应答，这里只读取地址，只读一次
	SOFTWAER_I2C_READS_MPU6050_Stop();
	
	return Data;
}

void MPU6050_Init(void)
{
	SOFTWAER_I2C_READS_MPU6050_Init();
	
}
