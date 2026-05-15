#include "stm32f10x.h"                  // Device header
#include <time.h>

uint16_t MyRTC_Time[]={2026, 5, 6, 17, 28, 30};
void Write_Time(void);

void RTC_Init(){

			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
		  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
			
	
			PWR_BackupAccessCmd(ENABLE);
	
			if(BKP_ReadBackupRegister(BKP_DR1) != 0x1126){
			RCC_LSEConfig(RCC_LSE_ON);
			while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)!= SET);
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
			RCC_RTCCLKCmd(ENABLE);
	
			RTC_WaitForSynchro();
			RTC_WaitForLastTask();
			
			RTC_SetPrescaler(32768-1);
			RTC_WaitForLastTask();
			
			Write_Time();
			RTC_WaitForLastTask();
			BKP_WriteBackupRegister(BKP_DR1,0x1126);
			}
			else{
					RTC_WaitForSynchro();
					RTC_WaitForLastTask();
			}

}
void Write_Time(){
		time_t time;
		struct tm time_date;
		time_date.tm_year = MyRTC_Time[0] - 1900;		
		time_date.tm_mon = MyRTC_Time[1] - 1;
		time_date.tm_mday = MyRTC_Time[2];
		time_date.tm_hour = MyRTC_Time[3];
		time_date.tm_min = MyRTC_Time[4];
		time_date.tm_sec = MyRTC_Time[5];
		
		time=mktime(&time_date);
	
		RTC_SetCounter(time);
		RTC_WaitForLastTask();
		
}	
void Read_Time(){
		time_t time;
		struct tm time_date;
		time=RTC_GetCounter();
		time_date=*localtime(&time);
		MyRTC_Time[0] = time_date.tm_year  + 1900;		
		MyRTC_Time[1] = time_date.tm_mon  + 1;
		MyRTC_Time[2] = time_date.tm_mday ;
		MyRTC_Time[3] = time_date.tm_hour ;
		MyRTC_Time[4] = time_date.tm_min ;
		MyRTC_Time[5] = time_date.tm_sec ;

}