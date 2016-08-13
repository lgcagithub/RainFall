#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"Rain.h"

extern HINSTANCE hInst;

Status Rain_InitSystem(Rain* pRain,HWND hwnd)
{
	if(!pRain) return RA_FALSE;
	
	/*��ʼ����ϵͳ��һЩ���ԡ�*/
	pRain->m_hwnd = hwnd;
	RECT rect;
	GetClientRect(hwnd,&rect);
	Rain_SetWidth(pRain,rect.right - rect.left);
	Rain_SetHeight(pRain,rect.bottom - rect.top);
	pRain->m_lightningDelta = -1;
	pRain->m_landLevelStart = pRain->m_height/2 + 40;
	pRain->m_landLevelEnd = pRain->m_height;
	pRain->m_sizeRipMinSys = RAIN_SIZERIPMINSYS;
	pRain->m_sizeRipMaxSys = RAIN_SIZERIPMAXSYS;

	Rain_SetNumMax(pRain,RAIN_NUMMAX_DEFAULT);
	Rain_SetSizeMax(pRain,RAIN_SIZEMAX_DEFAULT);
	Rain_SetVelMax(pRain,RAIN_VELMAX_DEFAULT);
	Rain_SetVelMin(pRain,RAIN_VELMIN_DEFAULT);
	Rain_SetWindForce(pRain,RAIN_WINDFORCE_DEFAULT);	
	Rain_SetColorMax(pRain,RAIN_COLORMAX_DEFAULT);
	Rain_SetColorMin(pRain,RAIN_COLORMIN_DEFAULT);
	
	/*��ʼ����ϵͳ���������*/
	InitList(&pRain->m_rainDrops);
	srand(time(0));
	while(pRain->m_rainDrops.m_length < pRain->m_numMax)
	{
		Raindrop rd;
		Rain_ResetDrop(pRain,&rd);
		/*�������н��һ��ʼ���ȫ�������ڶ��˵�����*/
		rd.m_pos.y = rand()%pRain->m_height;
		
		InsertLast(&(pRain->m_rainDrops),&rd);
	}
	
	/*��ʼ��ϵͳ״̬Ϊ����*/
	pRain->m_state = RSS_RAIN;
	/*��������Ч��*/ 
	Rain_LoadLightingFrame(pRain);
	/*����������Ч*/
	PlaySound("res\\RainAndThunder.wav",
				NULL,
				SND_NOSTOP|SND_LOOP|SND_ASYNC|SND_FILENAME);
	
	//��ʾϵͳ״̬�����Ϣ 
	pRain->m_isPrintInfo = 1;
	
	return RA_OK;
}

Status Rain_DestroySystem(Rain* pRain)
{
	/*����������ϵͳ��*/
	if(pRain)
	{
		if(!DestroyList(&(pRain->m_rainDrops))) 
			return RA_FALSE;
		pRain->m_hwnd = NULL;
		Rain_SetWidth(pRain,0);
		Rain_SetHeight(pRain,0);
		pRain->m_lightningDelta = 0;
		pRain->m_landLevelStart = 0;
		pRain->m_landLevelEnd = 0;
		pRain->m_sizeRipMinSys = 0;
		pRain->m_sizeRipMaxSys = 0;
		
		pRain->m_numMax = 0;
		pRain->m_sizeMax = 0;
		pRain->m_velocityMax = 0;
		pRain->m_velocityMin = 0;
		pRain->m_windForce = 0;
		memset(&(pRain->m_colorMax),0,sizeof(Color));
		memset(&(pRain->m_colorMin),0,sizeof(Color));
		
		Rain_UnLoadLightingFrame(pRain);
	}
	
	return RA_OK;
}

void Rain_ResetDrop(Rain* pRain,Raindrop* pRaindrop)
{
	/*����ĳ������������*/
	if(pRain && pRaindrop)
	{
		RD_SetRaindropPos(pRaindrop,rand()%pRain->m_width,0);
		RD_SetRaindropSize(pRaindrop,GetRandomNum(0,pRain->m_sizeMax));
		RD_SetRaindropVel(pRaindrop,
					GetRandomNum(pRain->m_velocityMin,
					pRain->m_velocityMax));
		RD_SetRaindropColor(pRaindrop,
						GetRandomColor(*((unsigned long*)&(pRain->m_colorMin)),
						*((unsigned long*)&(pRain->m_colorMax))));
		
		/*����½������߶Ȳ�*/
		int levelDelta = pRain->m_landLevelEnd - pRain->m_landLevelStart;
		/*����ϵͳ����½����ʲ�*/
		double velDelta = pRain->m_velocityMax - pRain->m_velocityMin;
		/*���������½�߶ȱ�������*/
		double factor = (pRaindrop->m_velocity - pRain->m_velocityMin)/velDelta;
		/*���㲢���������½�߶�*/ 
		RD_SetLandLevel(pRaindrop,levelDelta*factor + pRain->m_landLevelStart);
		
		/*��γ�ʼˮȦ��СΪ0*/
		pRaindrop->m_sizeRippleCur = 0;
		
		/*�������ϵͳˮȦ�뾶��*/
		int sizeDelte = pRain->m_sizeRipMaxSys - pRain->m_sizeRipMinSys;
		/*���㲢����ˮ����Ӧ�����ˮȦ�뾶*/
		RD_SetSizeRippleMax(pRaindrop,sizeDelte*factor + pRain->m_sizeRipMinSys);
		
		/*�������֮�󣬽���״̬��Ϊ����״̬*/
		RD_SetRaindropState(pRaindrop,RD_FALLING);
	}
}

void Rain_ResetAllDrop(Rain* pRain)
{
	/*����������Ρ�*/
	if(pRain)
	{
		Node* p = pRain->m_rainDrops.m_head;
		while(p = GetNext(p))
			Rain_ResetDrop(pRain,&(p->m_data));
	}
}

void Rain_Render(Rain* pRain,double timeDelta)
{
	if(pRain)
	{	
		/*�ȸ���������Σ�����Ⱦ*/
		Rain_Update(pRain,timeDelta);
		
		/*ʹ�������弼������������˸��*/
		HDC hdc = GetDC(pRain->m_hwnd);
		HDC memDCSrc = CreateCompatibleDC(hdc);
		HDC memDC = CreateCompatibleDC(hdc);
		HBITMAP memBmpSrc = Rain_GetActiveFrame(pRain,timeDelta);
		HBITMAP oldBmpSrc = (HBITMAP)SelectObject(memDCSrc,memBmpSrc);
		HBITMAP memBmp = CreateCompatibleBitmap(hdc,pRain->m_width,pRain->m_height);
		HBITMAP oldBmp = (HBITMAP)SelectObject(memDC,memBmp);
		BitBlt(memDC,0,0,pRain->m_width,pRain->m_height,memDCSrc,0,0,SRCCOPY);
		
		Node* p = pRain->m_rainDrops.m_head;
		while(p = GetNext(p))
		{
			/*��������*/
			HPEN pen = CreatePen(PS_SOLID,
								p->m_data.m_size,
								RAIN_RGB(p->m_data.m_color.r,
										p->m_data.m_color.g,
										p->m_data.m_color.b));
			HPEN oldPen = (HPEN)SelectObject(memDC,pen);
			
			switch(p->m_data.m_state)
			{
				case RD_FALLING:
				{
					/*��˿˿ϸ��*/
					MoveToEx(memDC,p->m_data.m_pos.x,p->m_data.m_pos.y,NULL);
					LineTo(memDC,
							p->m_data.m_pos.x - pRain->m_windForce*0.05f,
							p->m_data.m_pos.y - p->m_data.m_velocity*0.05f);
					break;
				}
				case RD_RESET:
				case RD_LANDED:
				{
					/*�����пյı�ˢ���ܻ���Ȧ�����򻭳������档*/
					HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
					/*ѡ���±�ˢʱ��Ҫ����ԭ���ľɱ�ˢ���Ա����ָ���*/
					HBRUSH hOldBrush = (HBRUSH)SelectObject(memDC,hBrush);
					
					/*��ȦȦ����*/
					Ellipse(memDC,
							p->m_data.m_pos.x - p->m_data.m_sizeRippleCur,
							p->m_data.m_pos.y - p->m_data.m_sizeRippleCur*0.5,
							p->m_data.m_pos.x + p->m_data.m_sizeRippleCur,
							p->m_data.m_pos.y + p->m_data.m_sizeRippleCur*0.5);
							
					/*�ָ��ɱ�ˢ��*/
					SelectObject(memDC,hOldBrush);
					/*ɾ�����õ��±�ˢ��*/
					DeleteObject(hBrush);
					break;
				}
				default:break;
			}//end for switch
			
			/*ɾ������*/
			SelectObject(memDC,oldPen);
			DeleteObject(pen);
		}
		
		//��ʾһЩ��Ϣ 
		Rain_PrintSysInfo(memDC,pRain,timeDelta);
		
		//�ᶯ�ڴ��е�λͼ��ǰ̨ 
		BitBlt(hdc,0,0,pRain->m_width,pRain->m_height,memDC,0,0,SRCCOPY);
		
		//�ͷ�֮ǰ�������ڴ�DC��λͼ 
		SelectObject(memDC,oldBmp);
		SelectObject(memDCSrc,oldBmpSrc);
		DeleteObject(memBmp);
		//DeleteObject(memBmpSrc);		//ע�Ⱑ�������õ�λͼ�����ﲻ��ɾ����~ 
		DeleteDC(memDC);
		DeleteDC(memDCSrc);		
		ReleaseDC(pRain->m_hwnd,hdc);
	}
}

void Rain_Update(Rain* pRain,double timeDelta)
{
	/*����������Ρ�*/
	if(pRain)
	{
		/*����ˮ�����򲹳䡣*/
		if(pRain->m_rainDrops.m_length < pRain->m_numMax)
		{
			Raindrop rd;
			Rain_ResetDrop(pRain,&rd);
			InsertLast(&(pRain->m_rainDrops),&rd);
		}
		/*����ˮ��������١�*/
		if(pRain->m_rainDrops.m_length > pRain->m_numMax)
			DeleteFirst(&(pRain->m_rainDrops),NULL);
		
		/*����������ε�״̬��*/
		Node* p = pRain->m_rainDrops.m_head;
		while(p = GetNext(p))
		{
			switch(p->m_data.m_state)
			{
				case RD_RESET:
				{
					/*�������*/
					Rain_ResetDrop(pRain,&(p->m_data));
					break;
				}
				case RD_FALLING:
				{
					/*������ε����ڸ߶�*/
					p->m_data.m_pos.y += p->m_data.m_velocity * timeDelta;
					/*����ε�����½�߶ȣ������״̬��Ϊ��½״̬��*/
					if(p->m_data.m_pos.y > p->m_data.m_landLevel)
						RD_SetRaindropState(&(p->m_data),RD_LANDED);
					
					/*��������ܷ�����Ӱ�졣*/
					p->m_data.m_pos.x = (int)((p->m_data.m_pos.x+
										pRain->m_windForce*timeDelta+
										pRain->m_width)*1000)%
										(pRain->m_width*1000)/1000.0f;
							
					break;
				}//end for case RD_FALLING
				case RD_LANDED:
				{
					/*��ˮȦ�Ѵ���������ת������״̬��*/
					if(p->m_data.m_sizeRippleCur > p->m_data.m_sizeRippleMax)
						RD_SetRaindropState(&(p->m_data),RD_RESET);
						
					p->m_data.m_sizeRippleCur += 25*timeDelta;
					break;
				}
				default:break;
			}//end for switch
		}//end for while
	}//end for if
}

void Rain_MessageHandler(Rain* pRain, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(!pRain) return;
	
	if(message == WM_KEYDOWN)
	{
		switch(wParam)
		{
			case VK_LEFT://���ͷ 
			{
				Rain_SetWindForce(pRain,pRain->m_windForce - 10);
				break;
			}
			case VK_RIGHT://�Ҽ�ͷ 
			{
				Rain_SetWindForce(pRain,pRain->m_windForce + 10);
				break;
			}
			case VK_UP://�ϼ�ͷ 
			{
				Rain_SetNumMax(pRain,pRain->m_numMax + 100);
				break;
			}
			case VK_DOWN://�¼�ͷ 
			{
				Rain_SetNumMax(pRain,pRain->m_numMax - 100);
				break;
			}
			case 187://0�Աߵ�+= 
			{
				Rain_SetVelMax(pRain,pRain->m_velocityMax + 10);
				break;
			}
			case 189://0�Աߵ�-_
			{
				Rain_SetVelMax(pRain,pRain->m_velocityMax - 10);
				break;
			}
			case 221://P�Աߵ�]}
			{
				Rain_SetSizeRippleMaxSys(pRain,pRain->m_sizeRipMaxSys + 1);
				break;
			} 
			case 219://P�Աߵ�[{ 
			{
				Rain_SetSizeRippleMaxSys(pRain,pRain->m_sizeRipMaxSys - 1);
				break;
			}
			case 72://��ĸH
			{
				pRain->m_isPrintInfo = !pRain->m_isPrintInfo;
				break;
			} 
			default:break;
		}
	}
}

void Rain_PrintSysInfo(HDC hdc,Rain* pRain,double timeDelta)
{
	if(!pRain) return;
	
	SetBkMode(hdc,TRANSPARENT);
	SetTextColor(hdc,RGB(255,255,255));
	
	char info[255]; 
	int y = pRain->m_height;
	
////////////////////////����֡��fps///////////////////////////////// 
/*�������Լ򵥵���timeDeltaȡ����������֡�٣��������������Ƶ��
ˢ����ʾ���������֡�٣��������������´��룬����֡����ʾΪһ��
��ˢ��һ�Ρ�*/
	static int fps = 0;
	static int frame = 0;
	frame += 1;
	static double timeSegment = 0.0f;
	timeSegment += timeDelta;
	
	if(timeSegment >= 1.0f)
	{
		fps = frame;
		frame = 0;
		timeSegment = 0.0f;
	}
	
	sprintf(info,"FPS = %d",fps);
	TextOutA(hdc,0,y -= 16,info,lstrlenA(info));
////////////////////////////////////////////////////////////////////	
	
	if(pRain->m_isPrintInfo)
	{
		sprintf(info,"Num of raindrop = %d",pRain->m_rainDrops.m_length);
		TextOutA(hdc,0,y -= 16,info,lstrlenA(info));
		sprintf(info,"Wind force = %d",pRain->m_windForce);
		TextOutA(hdc,0,y -= 16,info,lstrlenA(info));
		sprintf(info,"Next lightning time delta = %d",pRain->m_lightningDelta);
		TextOutA(hdc,0,y -= 16,info,lstrlenA(info));
		sprintf(info,"Current moment = %.2lf",pRain->m_moment);
		TextOutA(hdc,0,y -= 16,info,lstrlenA(info));
		sprintf(info,"Max ripple size = %d",pRain->m_sizeRipMaxSys);
		TextOutA(hdc,0,y -= 16,info,lstrlenA(info));
		sprintf(info,"Max velocity = %d",pRain->m_velocityMax);
		TextOutA(hdc,0,y -= 16,info,lstrlenA(info));
		wsprintf(info,"��H�ɻ�ȡ����");
		TextOut(hdc,0,y -= 16,info,lstrlenA(info));
	}
	else
	{
		wsprintf(info,"���¼����Ըı�����ܶ�");
		TextOut(hdc,0,y -= 16,info,lstrlenA(info));
		wsprintf(info,"���Ҽ����Ըı����");
		TextOut(hdc,0,y -= 16,info,lstrlenA(info));
		wsprintf(info,"P�ұߵ����������Ըı����ˮȦ�뾶");
		TextOut(hdc,0,y -= 16,info,lstrlenA(info));
		wsprintf(info,"�˸���Աߵ�+ -���Ըı��������½��ٶ�");
		TextOut(hdc,0,y -= 16,info,lstrlenA(info));
		wsprintf(info,"��H�ɲ鿴״̬��Ϣ");
		TextOut(hdc,0,y -= 16,info,lstrlenA(info));
	}
}

unsigned int Rain_LoadLightingFrame(Rain* pRain)
{
	if(!pRain) return 0;
	
	int num_OK = 0;
	TCHAR fileName[MAX_PATH] = "\0";
	Rain_UnLoadLightingFrame(pRain);
	
	int index;
	for(index = 0; index < 5; index++)
	{
		wsprintf(fileName,"res\\pool%d.bmp",index);
		pRain->m_lightningFrame[index] =
			(HBITMAP)LoadImage(hInst,fileName,IMAGE_BITMAP,800,600,LR_LOADFROMFILE);
		if(pRain->m_lightningFrame[index]) num_OK++;
	}
	
	return num_OK;
}

unsigned int Rain_UnLoadLightingFrame(Rain* pRain)
{
	if(!pRain) return 0;
	
	int num_OK = 0;
	
	int index;
	for(index = 0; index < 5; index++)
	{
		if(pRain->m_lightningFrame[index])
			if(DeleteObject(pRain->m_lightningFrame[index]))
			{
				num_OK++;
				pRain->m_lightningFrame[index] = NULL;
			}
	}
	
	return num_OK;
}

HBITMAP Rain_GetActiveFrame(Rain* pRain,double timeDelta)
{
	static double timeSegment = 0;
	static int index = 0;
	
	pRain->m_moment += timeDelta;
	
	switch(pRain->m_state)
	{
		case RSS_RAIN:
		{
			if(pRain->m_moment > pRain->m_lightningDelta)
			{
				pRain->m_state = RSS_RAINANDLIGHTNING;
				pRain->m_moment = 0.0f;
				pRain->m_lightningDelta = rand()%10;
			}
			/*��������*/
			//PlaySound("res\\RainSound.wav",NULL,SND_NOSTOP|SND_LOOP|SND_ASYNC|SND_FILENAME);
			
			return pRain->m_lightningFrame[4];	//��ĩһ����û������� 
		}
		case RSS_RAINANDLIGHTNING:
		{
			timeSegment += timeDelta;
			if(timeSegment > 0.1)	//0.2��ÿ֡��ʾ��ʱ�� 
			{
				index++;
				timeSegment = 0;
				if(index == 5)
				{
					pRain->m_state = RSS_THUNDER;
					index = 0;
				}
			}
			return pRain->m_lightningFrame[index];
		}
		case RSS_THUNDER:
		{
			/*��������*/
			//PlaySound("res\\Thunder.wav",NULL,SND_NOSTOP|SND_ASYNC|SND_FILENAME);	
			
			pRain->m_state = RSS_RAIN;
			return pRain->m_lightningFrame[4];
		}
		default:return pRain->m_lightningFrame[4];
	}
}

void Rain_SetNumMax(Rain* pRain,int iValue)
{
	if(pRain && iValue>=0)
		pRain->m_numMax = iValue;
}

void Rain_SetSizeMax(Rain* pRain,int iValue)
{
	if(pRain && iValue>0)
		pRain->m_sizeMax = iValue;
}

void Rain_SetVelMax(Rain* pRain,int iValue)
{
	if(pRain && iValue>=0)
		if(iValue > pRain->m_velocityMin)
			pRain->m_velocityMax = iValue;
		else
			pRain->m_velocityMax = pRain->m_velocityMin + 1;
}

void Rain_SetVelMin(Rain* pRain,int iValue)
{
	if(pRain && iValue>=0)
	{
		pRain->m_velocityMin = iValue;
		if(pRain->m_velocityMin > pRain->m_velocityMax)
			pRain->m_velocityMax = pRain->m_velocityMin + 1;
	}
}

void Rain_SetWindForce(Rain* pRain,int iValue)
{
	if(pRain)
		pRain->m_windForce = iValue;
}

void Rain_SetColorMax(Rain* pRain,unsigned long iValue)
{
	if(pRain)
	{			
		pRain->m_colorMax.b = (unsigned char)((iValue & 0x00FF0000)>>16);
		if(pRain->m_colorMax.b < pRain->m_colorMin.b)
			pRain->m_colorMax.b = pRain->m_colorMin.b;
			
		pRain->m_colorMax.g = (unsigned char)((iValue & 0x0000FF00)>>8);
		if(pRain->m_colorMax.g < pRain->m_colorMin.g)
			pRain->m_colorMax.g = pRain->m_colorMin.g;
			
		pRain->m_colorMax.r = (unsigned char)(iValue & 0x000000FF);
		if(pRain->m_colorMax.r < pRain->m_colorMin.r)
			pRain->m_colorMax.r = pRain->m_colorMin.r;
	}
}

void Rain_SetColorMin(Rain* pRain,unsigned long iValue)
{
	if(pRain)
	{		
		pRain->m_colorMin.b = (unsigned char)((iValue & 0x00FF0000)>>16);
		if(pRain->m_colorMin.b > pRain->m_colorMax.b)
			pRain->m_colorMax.b = pRain->m_colorMin.b;
			
		pRain->m_colorMin.g = (unsigned char)((iValue & 0x0000FF00)>>8);
		if(pRain->m_colorMin.g > pRain->m_colorMax.g)
			pRain->m_colorMax.g = pRain->m_colorMin.g;
			
		pRain->m_colorMin.r = (unsigned char)(iValue & 0x000000FF);
		if(pRain->m_colorMin.r > pRain->m_colorMax.r)
			pRain->m_colorMax.r = pRain->m_colorMin.r;
	}
}

void Rain_SetWidth(Rain* pRain,int width)
{
	if(pRain) pRain->m_width = width;
}

void Rain_SetHeight(Rain* pRain,int height)
{
	if(pRain) pRain->m_height = height;
}

void Rain_SetSizeRippleMaxSys(Rain* pRain,int iRadius)
{
	if(pRain && iRadius>=0)
		if(iRadius > pRain->m_sizeRipMinSys)
			pRain->m_sizeRipMaxSys = iRadius;
		else
			pRain->m_sizeRipMaxSys = pRain->m_sizeRipMinSys + 1;
}

void Rain_SetSizeRippleMinSys(Rain* pRain,int iRadius)
{
	if(pRain && iRadius>=0)
	{
		pRain->m_sizeRipMinSys = iRadius;
		if(pRain->m_sizeRipMinSys > pRain->m_sizeRipMaxSys)
			pRain->m_sizeRipMaxSys = pRain->m_sizeRipMinSys + 1;
	}
}


int GetRandomNum(int minNum,int maxNum)
{
	return minNum+rand()%(maxNum-minNum);
}

unsigned long GetRandomColor(unsigned long minColor,unsigned long maxColor)
{
	unsigned long delta = 0,result = 0,minC = 0,maxC = 0;
	
	minC = (minColor&0x00FF0000)>>16;
	maxC = (maxColor&0x00FF0000)>>16;
	delta = maxC-minC;
	result |= (minC+rand()%delta)<<16;
	
	minC = (minColor&0x0000FF00)>>8;
	maxC = (maxColor&0x0000FF00)>>8;
	delta = maxC-minC;
	result |= (minC+rand()%delta)<<8;
	
	minC = minColor&0x000000FF;
	maxC = maxColor&0x000000FF;
	delta = maxC-minC;
	result |= minC+rand()%delta;
	
	return result;
}
