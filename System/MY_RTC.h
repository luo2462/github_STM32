#ifndef __MY_RTC_H
#define __MY_RTC_H


extern uint16_t MyRTC_Time[];

void MY_RTC_Init(uint8_t RTCCLK_mode);
void MY_RTC_SetTime(void);
void MyRTC_ReadTime(void);

#endif
