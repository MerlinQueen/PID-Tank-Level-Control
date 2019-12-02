#include <STC15.h>
#include "pwm.h"
#define CYCLE   0x7000L     //����PWM����(���ֵΪ32767)  28627����ֻӰ�쾫��

void PWM_Init(void)
{
	P_SW2 |= 0x80; 					//ʹ�ܷ���PWM����չRAM�������⹦�ܼĴ���XSFR
	PWMCFG = 0x02;                  //����PWM3�������ʼ��ƽΪ�ߵ�ƽ,����һ�η�ת��ƽΪ�͵�ƽ,PWM7��ʼ��ƽΪ�͵�ƽ
	PWMCKS = 0x03;                  //ѡ��PWM��ʱ��ΪFosc/(0+1)   ϵͳʱ��4��Ƶ
	PWMC = CYCLE;                   //����PWM����,����PWM����(���ֵΪ32767)

	PWM3CR = 0x08;                  //ѡ��PWM2�����P4.5,��ʹ���ж�   IO��ӳ����
	PWM7CR = 0x00;					//ѡ��PWM2�����P1.7,��ʹ���ж�

	PWM3T1 = 0x0001;				// ���õ�һ�η�תʱ��
	PWM3T2 = 0;						// ���õ�һ�η�תʱ��
	PWM7T1 = 0x0001;
	PWM7T2 = 0;

	PWMCR |= 0x80;                  //ʹ��PWMģ��
	P_SW2 &= ~0x80;
}

//void PWM3_LedOut(unsigned int Duty) // �����Χ0-1000
//{
//	if (Duty >1000)    // Duty=0 ʱ,IO������͵�ƽ
//	{
//		PWMCR &= ~(1 << 1); // ��������4.5״̬Ϊͨ��IO,IO�ڸ���
//		PWM_LED= 0;
//	}
//	else
//	{
//		P_SW2 |= 0x80;  	 // ����XSRF
//		PWM3T1 = 0x0001;
//		PWM3T2 = CYCLE * Duty / 1000;	// �������÷�תʱ��,���ߵ�ƽ����ʱ��
//		P_SW2 &= ~0x80;			// �ر�XSRF
//		PWMCR |= (1 << 1); // ����PWM3���
//	}
//}

void PWM7_LevelOut(unsigned int Duty) // �����Χ0-1000
{
	if (Duty >1000)    // Duty=0 ʱ,IO������͵�ƽ
	{
		PWMCR &= ~(1 << 5); // ��������P1.7״̬Ϊͨ��IO,IO�ڸ���
		PWM_Level = 0;
	}
	else
	{
		P_SW2 |= 0x80;
		PWM7T1 = 0x0001;
		PWM7T2 = CYCLE * Duty / 1000;	// �������÷�תʱ��,���ߵ�ƽ����ʱ��
		P_SW2 &= ~0x80;
		PWMCR |= (1 << 5); // ����PWM7���
	}
}


