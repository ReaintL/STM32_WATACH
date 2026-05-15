#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyRTC.h"
#include "MyI2C.h"
#include "MPU6050.h"
#include "Key.h"
#include "LED.h"
#include "Timer.h"
#include "Menu_UI.h"
#include "dino.h"

extern uint8_t KeyNum;
extern uint16_t key_count;

int main(void)
{
	OLED_Init();
	OLED_Clear();
	MyClock_Init();
	
	
	
	uint8_t clkflag1;
	while (1)
	{
		
		clkflag1 = first_page_clock();
		if(clkflag1 == 1){SettingPage();}
		if(clkflag1 == 2){SlidePage();}
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_Judge();
		Key3_tick();
		counter();
		dino_tick();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
