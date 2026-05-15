#ifndef __MYRTC_H
#define __MYRTC_H

extern uint16_t MyRTC_Time[6];
void RTC_Init(void);
void Write_Time(void);
void Read_Time(void);

#endif
