#ifndef __MPU6050_H
#define __MPU6050_H


typedef struct{
int16_t accel_x;
int16_t accel_y;
int16_t accel_z;

int16_t gyro_x;
int16_t gyro_y;
int16_t gyro_z;
}MPU6050_Data;

void MPU6050_Init(void);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data);
void MPU6050_GetData(MPU6050_Data *M);
uint8_t MPU6050_GetID(void);

#endif
