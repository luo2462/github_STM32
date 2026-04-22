#ifndef __MY_BKP_H
#define __MY_BKP_H

void MY_BKP_Init(void);
void BKP_Write(uint16_t BKP_DR, uint16_t Data);
uint16_t BKP_Read(uint16_t BKP_DR);


#endif
