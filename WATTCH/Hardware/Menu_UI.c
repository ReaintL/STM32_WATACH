#include "stm32f10x.h"                  // Device header
#include "MyRTC.h"
#include "OLED.h"
#include "Key.h"
#include "SetTime.h"
#include "LED.h"
#include "Delay.h"
#include "Timer.h"
#include "MPU6050.h"
#include "dino.h"
#include "AD.h"
#include <math.h>


uint8_t TimeCounter(void);
uint8_t Light(void);
uint8_t show_MPU6050(void);
uint8_t show_Game(void);
uint8_t Emoji(void);
uint8_t Gradienter(void);

void MyClock_Init(){
	RTC_Init();
	Read_Time();
	Key_Init();
	Key_Init();
	Timer_Init();
	AD_Init();
	LED_Init();
	
	MPU6050_Init();
}

/***********************************首页****************************************/


//电池

uint16_t Bat_Value;
int16_t Battery_Capacity;
void show_Battery(){
	int sum=0;
	for(int i=1;i<=3000;i++){
		Bat_Value = AD_GetValue();
		sum +=Bat_Value;
	}
	sum /= 3000;
	Battery_Capacity=(sum-3276)*100/819;
		if(Battery_Capacity<0)Battery_Capacity=0;
		OLED_ShowNum(85,4,Battery_Capacity,3,OLED_6X8);
		OLED_ShowChar(103,4,'%',OLED_6X8);
		if(Battery_Capacity==100)OLED_ShowImage(110,0,16,16,Battery);
	else if(Battery_Capacity>=10&&Battery_Capacity<100)
	{
		OLED_ShowImage(110,0,16,16,Battery);
		OLED_ClearArea((112+Battery_Capacity/10),5,(10-Battery_Capacity/10),6);
		OLED_ClearArea(85,4,6,8);
	}
	
	else
	{
		OLED_ShowImage(110,0,16,16,Battery);
		OLED_ClearArea(112,5,10,6);
		OLED_ClearArea(85,4,12,8);
}
}



void show_UI(){
	Read_Time();
	OLED_Printf(0,0,OLED_6X8,"%d-%d-%d",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2]);
	OLED_Printf(18,12,OLED_12X24,"%02d:%02d:%02d",MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
	show_Battery();
	OLED_ShowString(0,48,"设置",OLED_8X16);
	OLED_ShowString(96,48,"菜单",OLED_8X16);
}

uint8_t clkflag=1;
uint8_t first_page_clock(){
		
	uint8_t KeyNum;
	while(1)
	{
	KeyNum=Key_GetNum();
	
	//上一项
		if(KeyNum==1){
			clkflag--;
			if(clkflag<=0)clkflag=2;
		}
	//下一项
		if(KeyNum==2){
			clkflag++;
			if(clkflag>=3)clkflag=1;
		}
	//确定
		if(KeyNum==3){
			OLED_Clear();
			OLED_Update();
			return clkflag;
		}
		

		
	switch(clkflag){
		case 1:
			show_UI();
			OLED_ReverseArea(0,48,32,16);
			OLED_Update();
			break;
		
		
		case 2:
			show_UI();
			OLED_ReverseArea(96,48,32,16);
			OLED_Update();
			break;
	}
		
}
}
/*****************************设置*********************************/
void ShowSettingPage(){
			OLED_ShowImage(0,0,16,16,Return);
			OLED_ShowString(0,16,"日期时间设置",OLED_8X16);

}

uint8_t setflag=1;
//控制光标在设置界面移动的函数
uint8_t SettingPage()
{
	while(1)
	{
		uint8_t KeyNum;
		KeyNum=Key_GetNum();
		uint8_t setflag_temp=0;
		if(KeyNum==1)//上一项
		{
			setflag--;
			if(setflag<=0)setflag=2;
		}
		else if(KeyNum==2)//下一项
		{
			setflag++;
			if(setflag>=3)setflag=1;
		}
		else if(KeyNum==3)//确认
		{
			OLED_Clear();
			OLED_Update();
			setflag_temp=setflag;
		}
		
		if(setflag_temp==1){return 0;}
		else if(setflag_temp==2){SetTime();}//跳转到日期时间设置界面
		
		switch(setflag)
		{
			case 1:
				ShowSettingPage();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			
			case 2:
				ShowSettingPage();
				OLED_ReverseArea(0,16,96,16);
				OLED_Update();
				break;
		}
	}
}

/**************************菜单********************************/

uint8_t pre_selection;//上次选择的选项
uint8_t target_selection;//目标选项
uint8_t x_pre=48;//上次选项的x坐标
uint8_t Speed=4;//速度
uint8_t move_flag;//开始移动的标志位，1表示开始移动，0表示停止移动


//滑动菜单动画函数
void Menu_Animation(void)
{
	OLED_Clear();
	OLED_ShowImage(42,10,44,44,Frame);
	
	if(pre_selection<target_selection){
		x_pre-=Speed;
		if(x_pre==0){
			pre_selection++;
			move_flag=0;
			x_pre=48;
		}
		}
	
		
	if(pre_selection>target_selection){
		x_pre+=Speed;
		if(x_pre==96){
			pre_selection--;
			move_flag=0;
			x_pre=48;
		}
	
	}
	if(pre_selection>=1){
		OLED_ShowImage(x_pre-48,16,32,32,Menu_Graph[pre_selection-1]);
	}	
		
	if(pre_selection>=2)
	{
		OLED_ShowImage(x_pre-96,16,32,32,Menu_Graph[pre_selection-2]);
	}
	if(pre_selection <= 5)
	{
			OLED_ShowImage(x_pre + 48, 16, 32, 32, Menu_Graph[pre_selection + 1]);
	}

	if(pre_selection <= 4)
	{
			OLED_ShowImage(x_pre + 96, 16, 32, 32, Menu_Graph[pre_selection + 2]);
	}
	OLED_ShowImage(x_pre,16,32,32,Menu_Graph[pre_selection]);
	
	
	OLED_Update();
}
	


//设置移动方向
void setDirection(uint8_t MoveFlag,uint8_t Pre_Selection,uint8_t Target_Selection){
		if(MoveFlag==1){
			pre_selection = Pre_Selection;
			target_selection =Target_Selection;
		}
		Menu_Animation();
}

uint8_t slideFlag=1;
uint8_t dirFlag=2;//上移或下移标志位   1上移  2 下移
//控制光标在设置界面移动的函数
uint8_t SlidePage()
{
	while(1)
	{
		uint8_t KeyNum;
		KeyNum=Key_GetNum();
		if(KeyNum==1)//上一项
		{
			dirFlag=1;
			move_flag=1;
			slideFlag--;
			if(slideFlag<=0)slideFlag=7;
		}
		else if(KeyNum==2)//下一项
		{
			dirFlag=2;
			move_flag=1;
			slideFlag++;
			if(slideFlag>=8)slideFlag=1;
		}
		else if(KeyNum==3)//确认
		{
			OLED_Clear();
			OLED_Update();
			if(slideFlag==1){return 0;}
			if(slideFlag==2){TimeCounter();}
			if(slideFlag==3){Light();}
			if(slideFlag==4){show_MPU6050();}
			if(slideFlag==5){show_Game();}
			if(slideFlag==6){Emoji();}
			if(slideFlag==7){Gradienter();}
		}
		
		if(slideFlag==1){
			if(dirFlag==1){setDirection(move_flag,1,0);}
			else if(dirFlag==2){setDirection(move_flag,0,0);}
		
		}
		else {
			if(dirFlag==1){setDirection(move_flag,slideFlag,slideFlag-1);}
			else if(dirFlag==2){setDirection(move_flag,slideFlag-2,slideFlag-1);};
		
		}
		
		
		
	}
}

/*********************秒表界面**********************************/
uint8_t hour,min,sec;

void show_TimeCounter(){
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(32,20,OLED_8X16,"%02d:%02d:%02d",hour,min,sec);
	OLED_ShowString(8,44,"开始",OLED_8X16);
	OLED_ShowString(48,44,"停止",OLED_8X16);
	OLED_ShowString(88,44,"清除",OLED_8X16);
}

uint8_t startTimeFlag;
static uint16_t count;
void counter(){
	if(startTimeFlag==1){
		count++;
		if(count==1000){
			count=0;
			sec++;
			if(sec>=60)
			{
				sec=0;
				min++;
				if(min>=60)
				{
					min=0;
					hour++;
					if(hour>99)hour=0;
				}
		}
	}
	}

		
}


uint8_t counterFlag=2;
uint8_t TimeCounter(){
	while(1)
	{
		uint8_t KeyNum;
		KeyNum=Key_GetNum();
		if(KeyNum==1)//上一项
		{
			counterFlag--;
			if(counterFlag<=0)counterFlag=4;
		}
		else if(KeyNum==2)//下一项
		{
			
			counterFlag++;
			if(counterFlag>=5)counterFlag=1;
		}
		else if(KeyNum==3)//确认
		{
			//返回
			if(counterFlag==1){
					counterFlag=2;
					return 0;
			}
			//开始
			if(counterFlag==2){
					startTimeFlag=1;
			}
			//停止
			else if (counterFlag==3){
					startTimeFlag=0;
			}
			//清楚
			else {startTimeFlag=0;hour=0;sec=0;min=0;}
		}
		
		
		switch(counterFlag){
			case 1:
				show_TimeCounter();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			
			case 2:
				show_TimeCounter();
				OLED_ReverseArea(8,44,32,16);
				OLED_Update();
				break;
			
		  case 3:
				show_TimeCounter();
				OLED_ReverseArea(48,44,32,16);
				OLED_Update();
				break;
			
			case 4:
				show_TimeCounter();
				OLED_ReverseArea(88,44,32,16);
				OLED_Update();
				break;
		
		}
	}

}
/*********************手电筒*********************/
void showLight(){
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(30,20,"开",OLED_8X16);
	OLED_ShowString(82,20,"关",OLED_8X16);
}


uint8_t lightFlag=2;
uint8_t Light(){
	while(1){
		uint8_t KeyNum;
		KeyNum=Key_GetNum();
		if(KeyNum==1)//上一项
		{
			lightFlag--;
			if(lightFlag<=0)lightFlag=3;
		}
		else if(KeyNum==2)//下一项
		{
			
			lightFlag++;
			if(lightFlag>=4)lightFlag=1;
		}
		else if(KeyNum==3)//确认
		{
			//返回
			if(lightFlag==1){lightFlag=2;return 0;}
			
			//开
			else if(lightFlag==2){LED1_ON();}
			
			//关
			else if(lightFlag==3){LED1_OFF();}
		}
		
		
		switch(lightFlag){
			case 1:
				showLight();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			
			case 2:
				showLight();
				OLED_ReverseArea(30,20,16,16);
				OLED_Update();
				break;
			
		  case 3:
				showLight();
				OLED_ReverseArea(82,20,16,16);
				OLED_Update();
				break;
			
		}
	}
}



/***********************MPU6050解算*******************************/
int16_t ax,ay,az,gx,gy,gz;//MPU6050测得的三轴加速度和角速度
float roll_g,pitch_g,yaw_g;//陀螺仪解算的欧拉角
float roll_a,pitch_a;//加速度计解算的欧拉角
float Roll,Pitch,Yaw;//互补滤波后的欧拉角
float a=0.9;//互补滤波器系数
float Delta_t=0.005;//采样周期
double pi=3.1415927;

//通过MPU6050的数据进行姿态解算的函数
void MPU6050_Calculation(void)
{
	Delay_ms(5);
	MPU6050_GetData(&ax,&ay,&az,&gx,&gy,&gz);
	
	//通过陀螺仪解算欧拉角
	roll_g=Roll+(float)gx*Delta_t;
	pitch_g=Pitch+(float)gy*Delta_t;
	yaw_g=Yaw+(float)gz*Delta_t;
	
	//通过加速度计解算欧拉角
	pitch_a=atan2((-1)*ax,az)*180/pi;
	roll_a=atan2(ay,az)*180/pi;
	
	//通过互补滤波器进行数据融合
	Roll=a*roll_g+(1-a)*roll_a;
	Pitch=a*pitch_g+(1-a)*pitch_a;
	Yaw=a*yaw_g;
	
}


void show_MPU6050_UI(){
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(0,16,OLED_8X16,"Roll: %.2f",Roll);
	OLED_Printf(0,32,OLED_8X16,"Pitch:%.2f",Pitch);
	OLED_Printf(0,48,OLED_8X16,"Yaw:  %.2f",Yaw);
}


uint8_t MPU6050Flag=1;
uint8_t show_MPU6050(){
	uint8_t KeyNum=0;
	while(1){
		KeyNum=Key_GetNum();
		if(KeyNum==3)
		{
			OLED_Clear();
			OLED_Update();
			return 0;
		}
		
		OLED_Clear();
		MPU6050_Calculation();
		show_MPU6050_UI();
		OLED_ReverseArea(0,0,16,16);
		OLED_Update();
			
		}
	}
	
/**********************游戏*****************************/
void Show_Game_UI(void)
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(0,16,"谷歌小恐龙",OLED_8X16);
}
uint8_t Game_Flag=2;
uint8_t show_Game(){
	uint8_t keynum=0;
	while(1){
		keynum=Key_GetNum();
		if(keynum==1)//上一项
		{
			Game_Flag--;
			if(Game_Flag<=0)Game_Flag=2;
		}
		else if(keynum==2)//下一项
		{
			
			Game_Flag++;
			if(Game_Flag>=3)Game_Flag=1;
		}
		else if(keynum==3)//确认
		{
			if(Game_Flag==1){Game_Flag=2;return 0;}
			if(Game_Flag==2){Game_Flag=2;while(1){if(Dino_Animation() == 1)break;}}
		}
		switch(Game_Flag){
			case 1:
				Show_Game_UI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			
			case 2:
				Show_Game_UI();
				OLED_ReverseArea(0,16,80,16);
				OLED_Update();
				break;
		}
		
	
}
}

/*----------------------------------动态表情包-------------------------------------*/

//显示动态表情包
void Show_Emoji_UI(void)
{
	/*闭眼*/
	for(uint8_t i=0;i<3;i++)
	{
		OLED_Clear();
		OLED_ShowImage(30,10+i,16,16,Eyebrow[0]);//左眉毛
		OLED_ShowImage(82,10+i,16,16,Eyebrow[1]);//右眉毛
		OLED_DrawEllipse(40,32,6,6-i,1);//左眼
		OLED_DrawEllipse(88,32,6,6-i,1);//右眼
		OLED_ShowImage(54,40,20,20,Mouth);
		OLED_Update();
		Delay_ms(100);
	}
	
	/*睁眼*/
	for(uint8_t i=0;i<3;i++)
	{
		OLED_Clear();
		OLED_ShowImage(30,12-i,16,16,Eyebrow[0]);//左眉毛
		OLED_ShowImage(82,12-i,16,16,Eyebrow[1]);//右眉毛
		OLED_DrawEllipse(40,32,6,4+i,1);//左眼
		OLED_DrawEllipse(88,32,6,4+i,1);//右眼
		OLED_ShowImage(54,40,20,20,Mouth);
		OLED_Update();
		Delay_ms(100);
	}
	
	Delay_ms(500);
	
}

//用按键控制退出动态表情包界面的函数
uint8_t Emoji(void)
{
	uint8_t KeyNum;
	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==3)
		{
			OLED_Clear();
			OLED_Update();
			return 0;
		}
		
		Show_Emoji_UI();
		
	}
}


/*----------------------------------水平仪-------------------------------------*/
void Show_Gradienter_UI(void)
{
	MPU6050_Calculation();
	OLED_DrawCircle(64,32,30,0);
	OLED_DrawCircle(64-Roll,32+Pitch,4,1);
}

//用按键控制退出水平仪界面的函数
uint8_t Gradienter(void)
{
	uint8_t KeyNum;
	while(1)
	{

		KeyNum=Key_GetNum();
		if(KeyNum==3)
		{
			OLED_Clear();
			OLED_Update();
			return 0;
		}
		OLED_Clear();
		Show_Gradienter_UI();
		OLED_Update();
	}
}