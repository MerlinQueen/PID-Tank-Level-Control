#ifndef	_TM1638_H
#define	_TM1638_H
#include <STC15.h>

//引脚定义
sbit	DIO=P2^3;
sbit	CLK=P2^2;
sbit	STB=P2^1;


void TM1638_Init(void);   //TM1638初始化
unsigned char KeyScan(void);   // 按键扫描程序
void LedDisplay(unsigned char data0,unsigned char data1,unsigned char data2,unsigned char data3,
               unsigned char data4,unsigned char data5,unsigned char data6,unsigned char data7);

void LedDisplay1(unsigned char ddata0,unsigned char ddata1,unsigned char ddata2,unsigned char ddata3,
                 unsigned char ddata4,unsigned char ddata5,unsigned char ddata6,unsigned char ddata7);

#endif
