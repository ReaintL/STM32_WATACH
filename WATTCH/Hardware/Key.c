#include "stm32f10x.h"                  // Device header
#include "Delay.h"


uint8_t KeyNum;
static uint8_t count;
static uint8_t PreState,CurState;
/**
  * 函    数：按键初始化
  * 参    数：无
  * 返 回 值：无
  */
	
	
void Key_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//初始化A4  和A0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	//初始化B1
}

/**
  * 函    数：按键获取键码
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~2，返回0代表没有按键按下
  */
uint8_t Key_GetNum(void)
{
	uint8_t Temp;
	if(KeyNum)
	{
		Temp=KeyNum;
		KeyNum=0;
		return Temp;
	}
	else
	{
		return 0;
	}
}

uint16_t key_count;
void Key3_tick(){
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0){
		
		if(key_count <1001){
			key_count++;
			}
		else {
			key_count=0;
			GPIO_ResetBits(GPIOB,GPIO_Pin_14);
		}
	}
	else {
		key_count=0;
	}		
	
	
	}

uint8_t Key_GetState(void){

	uint8_t KeyState=0;
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)			//读PB1输入寄存器的状态，如果为0，则代表按键1按下
	{
		
		KeyState = 1;												//置键码为1
	}
	
	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)			//读PA4输入寄存器的状态，如果为0，则代表按键2按下
	{
	
		KeyState = 2;												//置键码为2
	}
	
	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)			//读PA0输入寄存器的状态，如果为0，则代表按键1按下
	{
		KeyState = 3;												//置键码为3
			if(key_count>=1000 && KeyState==3){
				return 4;
			}
	}
	
	return KeyState;			
}



void Key_Judge(){

	count++;
	if(count >=20){
		count=0;
		PreState=CurState;
		CurState=Key_GetState();
	if(PreState!=0&&CurState==0){
		KeyNum=PreState;
	}
}
}
