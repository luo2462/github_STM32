#ifndef __HARDWARE_I2C_MPU6050_H
#define __HARDWARE_I2C_MPU6050_H


typedef struct{
int16_t accel_x;
int16_t accel_y;
int16_t accel_z;

int16_t gyro_x;
int16_t gyro_y;
int16_t gyro_z;
}Hardware_I2C_MPU6050_Data;

uint8_t Hardware_MPU6050_ReadReg(uint8_t RegAddress);
void Hardware_MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data);
void Hardware_MPU6050_Init(void);
uint8_t Hardware_MPU6050_GetID(void);
void Hardware_MPU6050_GetData(Hardware_I2C_MPU6050_Data *M);

#endif
