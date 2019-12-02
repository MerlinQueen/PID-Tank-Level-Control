#include <STC15.h>
#include "ADC.h"
#include "delay.h"

#define	ADC_POWER (1<<7)
#define ADC_SPEED 0x40 // SPEED1 SPEED0 10 180个周期转换一次高速模式
#define ADC_FLAG  (1<<4)// D%转换结束标志位 转换结束自动置1,需要手动清零
#define ADC_START (1<<3) // D4转换开始位

void ADC_Init(void)
{
	P1ASF = 0x7f;  // 8路为ADC模拟功能 0为普通io口功能
	ADC_RES = 0;
	ADC_RESL= 0;
	ADC_CONTR = ADC_POWER | ADC_SPEED ; // 0xc0 开启ADC电源,设置转换速度
	//CLK_DIV |= 0x20;  //设置RES与RESL数据的存储方式在PCON2寄存器中D5位
	DelayMS(1000); // 上电延时
}

u16 getADC10bitResult(char channel) //编写自定义ADC通道函数
{
	u16 ADC_Value = 0; // 存放10位ADC转换结果
	// ADC_CONTR = (ADC_CONTR & 0xe0) | ADC_START | channel; 设置任意io口为AD通道
	ADC_CONTR = ADC_POWER | ADC_SPEED | ADC_START| channel; // 开始转换 
	DelayMS(1);
	while (!(ADC_CONTR & ADC_FLAG)); //查询转换结果,当转换完成时,清除标志并读取结果
	ADC_CONTR &= ~ADC_FLAG; //清除转换标志位
	//10位AD的结果高2位放在RES中,低8位 放在RESL中
	if(CLK_DIV&(1<<5)) //CLK_DIV 第5位的ADRJ决定RES与RESL的存放顺序
	{
		ADC_Value = (unsigned int)(ADC_RES&3);//3等同于0000 0011  保留低两位其余清零
		ADC_Value =(ADC_Value <<8)|ADC_RESL;
	}
	else
	{
		ADC_Value = (unsigned int)ADC_RES;
		ADC_Value = (ADC_Value<<2)|(ADC_RESL&3);
	}
	
	return ADC_Value;
}

