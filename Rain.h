#pragma once
#ifndef _RAIN_H_
#define _RAIN_H_

#include<windows.h>
#include <mmsystem.h>
#include"DS_LinkList.h"
#include"Raindrop.h"

////////////////״̬��//////////////////// 
#define RA_OK 1
#define RA_FALSE 0

///////////////Ĭ����ֵ��//////////////// 
#define RAIN_NUMMAX_DEFAULT 300
#define RAIN_SIZEMAX_DEFAULT 2
#define RAIN_VELMAX_DEFAULT 500
#define RAIN_VELMIN_DEFAULT 100
#define RAIN_WINDFORCE_DEFAULT 100
#define RAIN_COLORMAX_DEFAULT RAIN_RGB(255,255,255)
#define RAIN_COLORMIN_DEFAULT RAIN_RGB(0,0,0)
//�����ɫ����С��ɫ�ķ�����Ҫһ��������Ҫ��֤min < max
//���򽫳��� 
#define RAIN_SIZERIPMINSYS 15
#define RAIN_SIZERIPMAXSYS 50

////////////////RGB��ɫת��Ϊ16���ƺ�////////////////// 
#define RAIN_RGB(r,g,b) 0x00000000|(b<<16)|(g<<8)|r

/*��ϵͳ״̬ö��*/
typedef enum tagRainSysState
{
	RSS_RAIN,
	RSS_RAINANDLIGHTNING,
	RSS_THUNDER
} RainSysState;

/*��ɫ����������ϵͳ�ṹ*/
typedef struct tagRain
{
	HWND m_hwnd;			//����Ⱦ�Ĵ��ھ�� 
	int m_width;			//���ڵĿ�� 
	int m_height;			//���ڵĸ߶� 
	
	LinkList m_rainDrops;	//������� 
	int m_numMax;			//�������� 
	int m_sizeMax;			//�����γߴ� 
	int m_velocityMax;		//�����������ٶ� 
	int m_velocityMin;		//��С�����ٶ� 
	int m_windForce;		//���� 
	//WindDirection m_windDir; //���� 
	Color m_colorMax;	//�����ɫ���� 
	Color m_colorMin;	//�����ɫ���� 
	
	HBITMAP m_lightningFrame[5];	//ʵ������Ч����5��λͼ 
	int m_lightningDelta;	//�ٷ�����ʱ���� 
	double m_moment;		//ϵͳ��ǰ���ڵ�ʱ�� 
	
	int m_landLevelStart;	//½��������ʼ�߶�(�������Ļ��˵) 
	int m_landLevelEnd;		//½����������߶�(�������Ļ��˵) 
	
	int m_sizeRipMinSys;	//���ˮȦ��ϵͳ��С�뾶 
	int m_sizeRipMaxSys;	//���ˮȦ��ϵͳ���뾶 
	
	RainSysState m_state;	//��ϵͳ����״̬ 
	char m_isPrintInfo;		//�Ƿ���ʾϵͳ״̬�����Ϣ 
} Rain,*PRain;

///////////////////////��ϵͳά����غ���////////////////////////// 

Status Rain_InitSystem(Rain* pRain,HWND hwnd);			//��ʼ����ϵͳ 
Status Rain_DestroySystem(Rain* pRain);				//������ϵͳ 
void Rain_ResetDrop(Rain* pRain,Raindrop* pRaindrop);//����ĳ��� 
void Rain_ResetAllDrop(Rain* pRain);					//����������� 
void Rain_Render(Rain* pRain,double timeDelta);			//��Ⱦ�� 
void Rain_Update(Rain* pRain,double timeDelta);		//�������״̬ 

//��ϵͳ����Ϣ������ 
void Rain_MessageHandler(Rain* pRain, UINT message, WPARAM wParam, LPARAM lParam);
//�����ϵͳ��һЩ��Ϣ
void Rain_PrintSysInfo(HDC hdc,Rain* pRain,double timeDelta); 

///////////////////����Ч����غ���/////////////////////////// 

unsigned int Rain_LoadLightingFrame(Rain* pRain);
unsigned int Rain_UnLoadLightingFrame(Rain* pRain);
HBITMAP Rain_GetActiveFrame(Rain* pRain,double timeDelta);

/////////////һϵ����ϵͳ״̬���ú���������֪��////////////// 

void Rain_SetNumMax(Rain* pRain,int iValue);
void Rain_SetSizeMax(Rain* pRain,int iValue);
void Rain_SetVelMax(Rain* pRain,int iValue);
void Rain_SetVelMin(Rain* pRain,int iValue);
void Rain_SetWindForce(Rain* pRain,int iValue);
void Rain_SetColorMax(Rain* pRain,unsigned long iValue);
void Rain_SetColorMin(Rain* pRain,unsigned long iValue);
void Rain_SetWidth(Rain* pRain,int width);
void Rain_SetHeight(Rain* pRain,int height);
void Rain_SetSizeRippleMaxSys(Rain* pRain,int iRadius);
void Rain_SetSizeRippleMinSys(Rain* pRain,int iRadius);

////////////////��ȡ�����ֵϵ�к���////////////////////// 

int GetRandomNum(int minNum,int maxNum);
unsigned long GetRandomColor(unsigned long minColor,unsigned long maxColor);

#endif
