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

/*����״̬ö��*/
typedef enum tagRaindropState
{
	RD_RESET,		//���� 
	RD_FALLING,		//�½� 
	RD_LANDED		//��½ 
} RaindropState;

typedef struct tagRaindrop
{
	Position m_pos;		//��γ�������Ļ��λ�� 
	int m_size;			//��εĴ�С 
	double m_velocity;		//��ε������ٶ� 
	Color m_color;		//��ε���ɫ 
	
	RaindropState m_state;		//��ε�״̬ 
	int m_landLevel;			//�����½�ĸ߶�(����ڶ�ά��Ļ��˵) 
	double m_sizeRippleCur;		//��ǰˮȦ�Ĵ�С
	int m_sizeRippleMax;		//���ˮȦ�Ĵ�С 
} Raindrop,*PRaindrop; 

/////////////////�������������ú���/////////// 
void RD_SetRaindropPos(Raindrop* rd,double x,double y);
void RD_SetRaindropSize(Raindrop* rd,int size);
void RD_SetRaindropVel(Raindrop* rd,double velocity);
void RD_SetRaindropColor(Raindrop* rd,unsigned long color);

void RD_SetRaindropState(Raindrop* rd,RaindropState state);
void RD_SetLandLevel(Raindrop* rd,int level);
void RD_SetSizeRippleMax(Raindrop* rd,int size);

#endif
