#ifndef __HARDWARE_SPI_H
#define __HARDWARE_SPI_H

void HardWare_SPI_Init(void);
void HardWare_SPI_START(void);
void HardWare_SPI_STOP(void);
uint8_t HardWare_SPI_SwapByte(uint8_t ByteSend);
void Hardware_SPI_Switch_Mode(uint8_t spi_mode);

#endif
