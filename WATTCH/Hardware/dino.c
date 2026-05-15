#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Key.h"
#include <stdlib.h>
#include <math.h>
#include "Delay.h"

typedef struct {
	int16_t minx;
	int16_t miny;
	int16_t maxx;
	int16_t maxy;

}ob_position;

uint8_t score;
void show_score(){
	OLED_ShowNum(98,0,score,5,OLED_6X8);
}

uint16_t ground_pos;
void show_ground(){
	
	if(ground_pos<=127){
	for(uint8_t i = 0;i<=127;i++){
			OLED_DisplayBuf[7][i]=Ground[i + ground_pos];
		}
	}
	else {
			for(uint8_t i =0;i< 256 - ground_pos;i++){
				OLED_DisplayBuf[7][i]=Ground[i+ground_pos];
			}
			for(uint8_t i =256 - ground_pos;i<128;i++){
				OLED_DisplayBuf[7][i]=Ground[i-(256-ground_pos)];
			}
	
	}


}


uint8_t barrier_pos;
uint8_t num;
ob_position bar_position;
void show_barrier(){
	if(barrier_pos>=143){
	 num=rand() % 3;
	}
	OLED_ShowImage(128 - barrier_pos,44,16,18,Barrier[num]);
	bar_position.maxx=128 - barrier_pos + 5;
	bar_position.maxy=62 + 5;
	bar_position.minx=128 - barrier_pos + 5;
	bar_position.miny=44 + 5;
	
}


uint8_t cloud_pos;
void show_cloud(){
	OLED_ShowImage(127 - cloud_pos,9,16,8,Cloud);
}


uint8_t Leg_flag,Dino_flag;
uint8_t jump_flag;
uint16_t jumptime;
int16_t jump_pos;
extern double pi;
ob_position dino_position;


void show_Dino(){
	if(Key_GetNum() == 1 && jump_flag==0)jump_flag=1;//Ìø
	jump_pos=28*sin((float)(pi*jumptime/1000));
	if(jump_flag==0){
		if(Leg_flag==1){OLED_ShowImage(0,44,16,18,Dino[0]);}
		else{OLED_ShowImage(0,44,16,18,Dino[1]);}
	}
	else{
		OLED_ShowImage(0,44 - jump_pos,16,18,Dino[2]);
	}
	dino_position.maxx=16;
	dino_position.maxy=62-jump_pos;
	dino_position.minx=0;
	dino_position.miny=44-jump_pos;
}


//¼ì²âÅö×²
int iscoliding(){
	if(dino_position.maxx > bar_position.minx && bar_position.maxx > dino_position.minx && dino_position.maxy > bar_position.miny && bar_position.maxy > dino_position.miny ){
			OLED_Clear();
			OLED_ShowString(28,24,"Game Over",OLED_8X16);
			OLED_Update();
			Delay_s(1);
			OLED_Clear();
			OLED_Update();
			return 1 ;//½áÊø
	}
	return 0;//¼ÌÐø

}

int  Dino_Animation(){
	OLED_Clear();
	show_score();
	show_ground();
	show_barrier();
	show_cloud();
	show_Dino();
	if(iscoliding()==1)return 1;//½áÊø
	OLED_Update();
	return 0;
}
void dino_tick(){
	static uint8_t score_count,ground_count,barrier_count,cloud_count,dino_count;
	score_count++;
	ground_count++;
	barrier_count++;
	cloud_count++;
	dino_count++;
	if(score_count>=100){
		score++;
		score_count=0;
	}
	if(ground_count>=20){
		ground_count=0;
		ground_pos++;
		if(ground_pos>=256){
			ground_pos=0;
		}
	}
	if(barrier_count>=20){
		barrier_pos++;
		barrier_count=0;
		if(barrier_pos>=144)barrier_pos=0;
	}
	if(cloud_count>=20){
		cloud_count=0;
		cloud_pos++;
		if(cloud_pos>=143){
			cloud_pos=0;
		}
	}
	
	if(dino_count >=50){
		dino_count=0;
		Leg_flag++;
		if(Leg_flag>=2){Leg_flag=0;}
	}
	if(jump_flag==1){
		jumptime++;
		if(jumptime>=1000){jumptime=0;jump_flag=0;}
	}
}

