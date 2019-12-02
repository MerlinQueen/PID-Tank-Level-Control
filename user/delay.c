/************************************************************************************
Copyright,  LCE STUDIO   
�������ƣ���ʱ����(����Ƶ��18.432MHz)
����˵��: ��ʱ������΢��
************************************************************************************/

#include "stc15f2k60s2.h"	    // ��Ƭ��STC15F2K60S2ͷ�ļ�,���Բ��ټ���reg51.h
#include <intrins.h>			    // �����ͷ�ļ���,��ʹ��_nop_�⺯��
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



