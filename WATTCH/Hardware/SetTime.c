#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Key.h"
#include "MyRTC.h"

void show_Settime1STUI(){
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(0,16,OLED_8X16,"年:%4d",MyRTC_Time[0]);
	OLED_Printf(0,32,OLED_8X16,"月:%2d",MyRTC_Time[1]);
	OLED_Printf(0,48,OLED_8X16,"日:%2d",MyRTC_Time[2]);

}
void show_Settime2NDUI(){
	OLED_Printf(0,0,OLED_8X16,"时:%2d",MyRTC_Time[3]);
	OLED_Printf(0,16,OLED_8X16,"分:%2d",MyRTC_Time[4]);
	OLED_Printf(0,32,OLED_8X16,"秒:%2d",MyRTC_Time[5]);

}
void changeTime(uint8_t index,uint8_t flag){

	if(flag==1){MyRTC_Time[index]++;}
	else if(flag==2){MyRTC_Time[index]--;}
	Write_Time();
}


uint8_t keyNum1;
uint8_t setYear(){
	while(1){
	keyNum1=Key_GetNum();
	if(keyNum1==1){changeTime(0,1);}
	else if(keyNum1==2){changeTime(0,2);}
	else if(keyNum1==3){return 0;}
	OLED_Clear();
	show_Settime1STUI();
	OLED_ReverseArea(24,16,32,16);
	OLED_Update();
	}
	
}
uint8_t setMon(){
while(1){
	keyNum1=Key_GetNum();
	if(keyNum1==1){if(MyRTC_Time[1]>=12){MyRTC_Time[1]=1;}  else changeTime(1,1);}
	else if(keyNum1==2){if(MyRTC_Time[1]<=1){MyRTC_Time[1]=12;}  else changeTime(1,2);}
	else if(keyNum1==3){return 0;}
	OLED_Clear();
	show_Settime1STUI();
	OLED_ReverseArea(24,32,16,16);
	OLED_Update();
	}
}
uint8_t setDate(){
while(1){
	keyNum1=Key_GetNum();
	if(keyNum1==1){if(MyRTC_Time[2]>=31){MyRTC_Time[2]=1;}  else changeTime(2,1);}
	else if(keyNum1==2){if(MyRTC_Time[2]<=1){MyRTC_Time[2]=1;}  else changeTime(2,2);}
	else if(keyNum1==3){return 0;}
	OLED_Clear();
	show_Settime1STUI();
	OLED_ReverseArea(24,48,16,16);
	OLED_Update();
	}
}
uint8_t setHour(){
while(1){
	keyNum1=Key_GetNum();
	if(keyNum1==1){if(MyRTC_Time[3]>=23){MyRTC_Time[3]=0;}  else changeTime(3,1);}
	else if(keyNum1==2){if(MyRTC_Time[3]<=0){MyRTC_Time[3]=23;}  else changeTime(3,2);}
	else if(keyNum1==3){return 0;}
	OLED_Clear();
	show_Settime2NDUI();
	OLED_ReverseArea(24,0,16,16);
	OLED_Update();
	}
}
uint8_t setMin(){
while(1){
	keyNum1=Key_GetNum();
	if(keyNum1==1){if(MyRTC_Time[4]>=60){MyRTC_Time[4]=0;}  else changeTime(4,1);}
	else if(keyNum1==2){if(MyRTC_Time[4]<=0){MyRTC_Time[4]=60;}  else changeTime(4,2);}
	else if(keyNum1==3){return 0;}
	OLED_Clear();
	show_Settime2NDUI();
	OLED_ReverseArea(24,16,16,16);
	OLED_Update();
	}}
uint8_t setSecond(){
while(1){
	keyNum1=Key_GetNum();
	if(keyNum1==1){if(MyRTC_Time[5]>=60){MyRTC_Time[5]=0;}  else changeTime(5,1);}
	else if(keyNum1==2){if(MyRTC_Time[1]<=0){MyRTC_Time[1]=60;}  else changeTime(5,2);}
	else if(keyNum1==3){return 0;}
	OLED_Clear();
	show_Settime2NDUI();
	OLED_ReverseArea(24,32,16,16);
	OLED_Update();
	}
}
	
	
uint8_t settimeflag=1;

uint8_t SetTime(){
		
	uint8_t KeyNum;
	//uint8_t settimeflagTemp;
	while(1)
	{
	KeyNum=Key_GetNum();
	
	//上一项
		if(KeyNum==1){
			settimeflag--;
			if(settimeflag<=0)settimeflag=7;
		}
	//下一项
		if(KeyNum==2){
			settimeflag++;
			if(settimeflag>=8)settimeflag=1;
		}
	//确定
		if(KeyNum==3){
			OLED_Clear();
			OLED_Update();
			
			if(settimeflag==1){return 0;}
			else if(settimeflag==2){setYear();}
			else if(settimeflag==3){setMon();}	
			else if(settimeflag==4){setDate();}
			else if(settimeflag==5){setHour();}
			else if(settimeflag==6){setMin();}
			else if(settimeflag==7){setSecond();}
			
			
		}
		

		
	switch(settimeflag){
		case 1:
			OLED_Clear();
			show_Settime1STUI();
			OLED_ReverseArea(0,0,16,16);
			OLED_Update();
			break;
		
		
		case 2:
			OLED_Clear();
			show_Settime1STUI();
			OLED_ReverseArea(0,16,16,16);
			OLED_Update();
			break;
		
		case 3:
			OLED_Clear();
			show_Settime1STUI();
			OLED_ReverseArea(0,32,16,16);
			OLED_Update();
			break;
		
		case 4:
			OLED_Clear();
			show_Settime1STUI();
			OLED_ReverseArea(0,48,16,16);
			OLED_Update();
			break;
		
		case 5:
			OLED_Clear();
			show_Settime2NDUI();
			OLED_ReverseArea(0,0,16,16);
			OLED_Update();
			break;
		
		case 6:
			OLED_Clear();
			show_Settime2NDUI();
			OLED_ReverseArea(0,16,16,16);
			OLED_Update();
			break;
		
		case 7:
			OLED_Clear();
			show_Settime2NDUI();
			OLED_ReverseArea(0,32,16,16);
			OLED_Update();
			break;
	}
		
}
}
