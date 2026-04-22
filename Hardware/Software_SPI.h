#ifndef __SOTFWARE_SPI_H
#define __SOTFWARE_SPI_H

void SoftWare_SPI_Init(void);
void SoftWare_SPI_START(void);
void SoftWare_SPI_STOP(void);
uint8_t SoftWare_SPI_mode_0_SwapByte(uint8_t ByteSend);//Swap交换
uint8_t SoftWare_SPI_mode_0_SwapByte2(uint8_t ByteSend);//Swap交换
uint8_t SoftWare_SPI_mode_1_SwapByte(uint8_t ByteSend);//Swap交换
uint8_t SoftWare_SPI_mode_2_SwapByte(uint8_t ByteSend);//Swap交换
uint8_t SoftWare_SPI_mode_3_SwapByte(uint8_t ByteSend);//Swap交换

#endif
