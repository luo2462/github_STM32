#include "stm32f10x.h"                  // Device header
#include "Software I2C.h" 
#include "MPU6050_Reg.h"
#include "MPU6050.h"


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
}//写寄存器

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
}//读寄存器

void MPU6050_Init(void)
{
	SOFTWAER_I2C_READS_MPU6050_Init();
	
	//写入寄存器配置
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x01);//电源管理寄存器1
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0x00);//电源管理寄存器2
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0x09);//采样率分频(这里10分频)
	MPU6050_WriteReg(MPU6050_CONFIG,0x06);//配置寄存器
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0x18);//陀螺仪配置寄存器
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0x18);//加速度配置寄存器

}

//MPU6050_WriteReg(0x6B,0x00);(在电源管理寄存器1解除睡眠模式)

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);		//返回WHO_AM_I寄存器的值
}

void MPU6050_GetData(MPU6050_Data *M)
{
	uint8_t DataH,DataL;
	DataH=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	M->accel_x=(int16_t)(DataH<<8 | DataL);
	
	DataH=MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	M->accel_y=(int16_t)(DataH<<8 | DataL);
	
	DataH=MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	M->accel_z=(int16_t)(DataH<<8 | DataL);
	
	DataH=MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	M->gyro_x=(int16_t)(DataH<<8 | DataL);
	
	DataH=MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	M->gyro_y=(int16_t)(DataH<<8 | DataL);
	
	DataH=MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	M->gyro_z=(int16_t)(DataH<<8 | DataL);
}
