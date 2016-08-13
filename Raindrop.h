#pragma once
#ifndef _RAINDROP_H_
#define _RAINDROP_H_

typedef struct tagPosition
{
	double x,y;	
} Position,*PPosition;

typedef struct tagColor
{
	unsigned char r,g,b,a;	//red,green,blue,alpha
} Color,*PColor;

/*雨点的状态枚举*/
typedef enum tagRaindropState
{
	RD_RESET,		//重置 
	RD_FALLING,		//下降 
	RD_LANDED		//着陆 
} RaindropState;

typedef struct tagRaindrop
{
	Position m_pos;		//雨滴出现在屏幕的位置 
	int m_size;			//雨滴的大小 
	double m_velocity;		//雨滴的下落速度 
	Color m_color;		//雨滴的颜色 
	
	RaindropState m_state;		//雨滴的状态 
	int m_landLevel;			//雨滴着陆的高度(相对于二维屏幕来说) 
	double m_sizeRippleCur;		//当前水圈的大小
	int m_sizeRippleMax;		//最大水圈的大小 
} Raindrop,*PRaindrop; 

/////////////////雨点相关属性设置函数/////////// 
void RD_SetRaindropPos(Raindrop* rd,double x,double y);
void RD_SetRaindropSize(Raindrop* rd,int size);
void RD_SetRaindropVel(Raindrop* rd,double velocity);
void RD_SetRaindropColor(Raindrop* rd,unsigned long color);

void RD_SetRaindropState(Raindrop* rd,RaindropState state);
void RD_SetLandLevel(Raindrop* rd,int level);
void RD_SetSizeRippleMax(Raindrop* rd,int size);

#endif
