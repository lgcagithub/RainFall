#include"Raindrop.h"

void RD_SetRaindropPos(Raindrop* rd,double x,double y)
{
	if(rd)
	{
		rd->m_pos.x = x;
		rd->m_pos.y = y;
	}
}

void RD_SetRaindropSize(Raindrop* rd,int size)
{
	if(rd) rd->m_size = size;
}

void RD_SetRaindropVel(Raindrop* rd,double velocity)
{
	if(rd) rd->m_velocity = velocity;
}

void RD_SetRaindropColor(Raindrop* rd,unsigned long color)
{
	if(rd)
	{
		//rd->m_color.a = (unsigned char)0;
		rd->m_color.b = (unsigned char)((color&0x00FF0000)>>16);
		rd->m_color.g = (unsigned char)((color&0x0000FF00)>>8);
		rd->m_color.r = (unsigned char)(color&0x000000FF);
	}
}

void RD_SetRaindropState(Raindrop* rd,RaindropState state)
{
	if(rd) rd->m_state = state;
}

void RD_SetLandLevel(Raindrop* rd,int level)
{
	if(rd) rd->m_landLevel = level;
}

void RD_SetSizeRippleMax(Raindrop* rd,int size)
{
	if(rd) rd->m_sizeRippleMax = size;
}