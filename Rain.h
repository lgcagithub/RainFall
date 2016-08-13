#pragma once
#ifndef _RAIN_H_
#define _RAIN_H_

#include<windows.h>
#include <mmsystem.h>
#include"DS_LinkList.h"
#include"Raindrop.h"

////////////////状态宏//////////////////// 
#define RA_OK 1
#define RA_FALSE 0

///////////////默认数值宏//////////////// 
#define RAIN_NUMMAX_DEFAULT 300
#define RAIN_SIZEMAX_DEFAULT 2
#define RAIN_VELMAX_DEFAULT 500
#define RAIN_VELMIN_DEFAULT 100
#define RAIN_WINDFORCE_DEFAULT 100
#define RAIN_COLORMAX_DEFAULT RAIN_RGB(255,255,255)
#define RAIN_COLORMIN_DEFAULT RAIN_RGB(0,0,0)
//最大颜色和最小颜色的分量不要一样，并且要保证min < max
//否则将出错。 
#define RAIN_SIZERIPMINSYS 15
#define RAIN_SIZERIPMAXSYS 50

////////////////RGB颜色转换为16进制宏////////////////// 
#define RAIN_RGB(r,g,b) 0x00000000|(b<<16)|(g<<8)|r

/*雨系统状态枚举*/
typedef enum tagRainSysState
{
	RSS_RAIN,
	RSS_RAINANDLIGHTNING,
	RSS_THUNDER
} RainSysState;

/*彩色粒子雨天气系统结构*/
typedef struct tagRain
{
	HWND m_hwnd;			//被渲染的窗口句柄 
	int m_width;			//窗口的宽度 
	int m_height;			//窗口的高度 
	
	LinkList m_rainDrops;	//雨滴链表 
	int m_numMax;			//最大雨滴数 
	int m_sizeMax;			//最大雨滴尺寸 
	int m_velocityMax;		//雨滴最大下落速度 
	int m_velocityMin;		//最小下落速度 
	int m_windForce;		//风力 
	//WindDirection m_windDir; //风向 
	Color m_colorMax;	//雨滴颜色上限 
	Color m_colorMin;	//雨滴颜色下限 
	
	HBITMAP m_lightningFrame[5];	//实现闪电效果的5幅位图 
	int m_lightningDelta;	//促发打雷时间间隔 
	double m_moment;		//系统当前存在的时刻 
	
	int m_landLevelStart;	//陆地区域起始高度(相对于屏幕来说) 
	int m_landLevelEnd;		//陆地区域结束高度(相对于屏幕来说) 
	
	int m_sizeRipMinSys;	//雨滴水圈的系统最小半径 
	int m_sizeRipMaxSys;	//雨滴水圈的系统最大半径 
	
	RainSysState m_state;	//雨系统整体状态 
	char m_isPrintInfo;		//是否显示系统状态相关信息 
} Rain,*PRain;

///////////////////////雨系统维护相关函数////////////////////////// 

Status Rain_InitSystem(Rain* pRain,HWND hwnd);			//初始化雨系统 
Status Rain_DestroySystem(Rain* pRain);				//销毁雨系统 
void Rain_ResetDrop(Rain* pRain,Raindrop* pRaindrop);//重置某雨滴 
void Rain_ResetAllDrop(Rain* pRain);					//重置所有雨滴 
void Rain_Render(Rain* pRain,double timeDelta);			//渲染雨 
void Rain_Update(Rain* pRain,double timeDelta);		//更新雨的状态 

//雨系统的消息处理函数 
void Rain_MessageHandler(Rain* pRain, UINT message, WPARAM wParam, LPARAM lParam);
//输出雨系统的一些信息
void Rain_PrintSysInfo(HDC hdc,Rain* pRain,double timeDelta); 

///////////////////闪电效果相关函数/////////////////////////// 

unsigned int Rain_LoadLightingFrame(Rain* pRain);
unsigned int Rain_UnLoadLightingFrame(Rain* pRain);
HBITMAP Rain_GetActiveFrame(Rain* pRain,double timeDelta);

/////////////一系列雨系统状态设置函数，见名知意////////////// 

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

////////////////获取随机数值系列函数////////////////////// 

int GetRandomNum(int minNum,int maxNum);
unsigned long GetRandomColor(unsigned long minColor,unsigned long maxColor);

#endif
