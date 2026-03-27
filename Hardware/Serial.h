#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>


extern uint8_t Serial_RxFlag;
extern char Serial_RxTextPacket[];
extern uint8_t Serial_TxPacket[];  //发送的数组
extern uint8_t Serial_RxPacket[];

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array,uint16_t Length);
void Serial_SendString(char *String);

uint32_t Serial_Pow(uint32_t X,uint32_t Y);//这是用于Serial_SendNumber函数幂运算的部分
void Serial_SendNumber(uint32_t Number,uint8_t Length);
void Serial_Printf(char *format,...);


uint8_t Serial_GetRxData(void);

void Serial_SendPacket(void);

void Serial_RxTextStep(uint8_t RxTextData);
void Serial_RxHexStep(uint8_t RxHexData);

#endif
