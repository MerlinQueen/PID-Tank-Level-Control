#include <STC15.h>
#include "pwm.h"
#define CYCLE   0x7000L     //定义PWM周期(最大值为32767)  28627周期只影响精度

void PWM_Init(void)
{
	P_SW2 |= 0x80; 					//使能访问PWM在扩展RAM区的特殊功能寄存器XSFR
	PWMCFG = 0x02;                  //配置PWM3的输出初始电平为高电平,即第一次翻转电平为低电平,PWM7初始电平为低电平
	PWMCKS = 0x03;                  //选择PWM的时钟为Fosc/(0+1)   系统时钟4分频
	PWMC = CYCLE;                   //设置PWM周期,定义PWM周期(最大值为32767)

	PWM3CR = 0x08;                  //选择PWM2输出到P4.5,不使能中断   IO重映射了
	PWM7CR = 0x00;					//选择PWM2输出到P1.7,不使能中断

	PWM3T1 = 0x0001;				// 设置第一次翻转时间
	PWM3T2 = 0;						// 设置第一次翻转时间
	PWM7T1 = 0x0001;
	PWM7T2 = 0;

	PWMCR |= 0x80;                  //使能PWM模块
	P_SW2 &= ~0x80;
}

//void PWM3_LedOut(unsigned int Duty) // 输出范围0-1000
//{
//	if (Duty >1000)    // Duty=0 时,IO口输出低电平
//	{
//		PWMCR &= ~(1 << 1); // 设置引脚4.5状态为通用IO,IO口复用
//		PWM_LED= 0;
//	}
//	else
//	{
//		P_SW2 |= 0x80;  	 // 开启XSRF
//		PWM3T1 = 0x0001;
//		PWM3T2 = CYCLE * Duty / 1000;	// 重新设置翻转时间,即高电平持续时间
//		P_SW2 &= ~0x80;			// 关闭XSRF
//		PWMCR |= (1 << 1); // 开启PWM3输出
//	}
//}

void PWM7_LevelOut(unsigned int Duty) // 输出范围0-1000
{
	if (Duty >1000)    // Duty=0 时,IO口输出低电平
	{
		PWMCR &= ~(1 << 5); // 设置引脚P1.7状态为通用IO,IO口复用
		PWM_Level = 0;
	}
	else
	{
		P_SW2 |= 0x80;
		PWM7T1 = 0x0001;
		PWM7T2 = CYCLE * Duty / 1000;	// 重新设置翻转时间,即高电平持续时间
		P_SW2 &= ~0x80;
		PWMCR |= (1 << 5); // 开启PWM7输出
	}
}


