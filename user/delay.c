/************************************************************************************
Copyright,  LCE STUDIO   
程序名称：延时函数(晶振频率18.432MHz)
功能说明: 延时毫秒与微秒
************************************************************************************/

#include "stc15f2k60s2.h"	    // 单片机STC15F2K60S2头文件,可以不再加入reg51.h
#include <intrins.h>			    // 加入此头文件后,可使用_nop_库函数
#include <stdio.h>



void Delay1us()		//@18.432MHz
{
	unsigned char i;

	i = 2;
	while (--i);
}

void Delay1ms()		//@18.432MHz
{
	unsigned char i, j;

	i = 18;
	j = 235;
	do
	{
		while (--j);
	} while (--i);
}

void DelayMS(int t)
{
	while(t--)
	{
		Delay1ms();
	}
}

void DelayUS(int t)
{
	while(t--)
	{
		Delay1us();
	}
}



