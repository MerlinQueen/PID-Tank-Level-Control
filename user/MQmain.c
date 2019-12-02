/*
* @Author: Merlin
* @Date:   2019-05-18 15:09:04
* @Last Modified by:   Administrator
* @����Ƶ��:18.432MHz
* @Last Modified time: 2019-05-26 17:59:37
*/

#include <STC15.h>
#include "public.h"
#include "delay.h"
#include "TM1638.h"
#include "ADC.h"
#include "PWM.h"
//#include "OLED_spi.h"
#include "stdio.h"
#define Vref  5.0   // �ο���ѹ

#define kp  value_1[3]
#define ki  value_1[4]
#define kd  value_1[5]
#define CV  value_2[2]
#define SV  value_1[2]

//u8 OLED_CV[5];   // ���OLED----��ǰ��ѹ

u8 keyValue;  		// �洢������ֵ

u16  value_1[8] = {0, 0, 77, 100, 50, 20, 0, 0};     //3_p,4-i,5-d          // �������� P100,i50,d20
u16  value_2[3] = {0, 1, 2};

/**************GPIO�˿ڳ�ʼ��******************/
void GPIO_Init(void)
{
	P0M0 = 0x00;
	P0M1 = 0x00;
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M0 = 0x00;
	P2M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
}
/**************��ʾ����******************/
void display_Value(unsigned char symb1, unsigned char symb2, unsigned int value1, unsigned int value2)
{
	unsigned char dat1, dat2, dat3, dat4, dat5, dat6;
	dat1 = value1 / 100;
	dat2 = value1 / 10 % 10;
	dat3 = value1 % 10;
	dat4 = value2 / 100;
	dat5 = value2 / 10 % 10;
	dat6 = value2 % 10;
	LedDisplay(symb1 + 20, dat1, dat2, dat3, symb2 + 17, dat4, dat5, dat6);
}


/**************���ò�������******************/
bit Timerflag = 0; 
u8 Timercount = 0;
void Timer0_Init(void)		//1����@18.432MHz   ��ʱ��0 �������ò���Ƶ��
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0xB8;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;        // ������ʱ���ж�
}
void Timer_IRQ(void) interrupt 1
{
	Timercount++;
	if (Timercount == 100)      // ÿ�� 100MS����һ��ADC����
	{
		Timercount = 0;         // ���������
		Timerflag = 1;		    // �ñ�־λΪ1
	}
}

/**************��������******************/
u16  setValue = 0;  // �洢�趨ֵ
u8	 setVflag;  		// �趨ֵȷ����־
u8 	 symb1 = 0, symb2 = 0; // ��Ӧy,r,PID,o,u,n  �� L,h,r
void KeyProcess()
{
	if (keyValue <= 9)
	{
		setValue = setValue * 10 + keyValue;	 //  ��ֵ��ȡ
	}
	else
	{
		switch (keyValue)
		{
		case 10:
		{
			symb1 = (symb1 + 1) % 8;   // y,r,PID,o,u,nѡ��
			break;
		}
		case 11:
		{
			symb2 = (symb2 + 1) % 3;  // L,h,rѡ��    hҺλ,L����,r����
			break;
		}
		case 12:
		{
			value_1[6] =0;        // o -0
			break;
		}
		case 13:
		{
			value_1[6] =1;      // o-1
			break;
		}
		case 14:
		{
			value_1[6] =2;     // o-2
			break;
		}
		case 15:
		{
			setVflag = (setVflag + 1) % 2;
			if (setVflag)
			{
				value_1[symb1] = setValue;    // ���趨ֵ����Һλ �� ����
			}
		}
		}
	}
	if (setValue > 999) setValue = 0;         
}
/**************˫λ����******************/
void Limit_Contl(void)
{
	int dec;
	dec = value_1[7] - value_2[7];     // ���ݵ�ѹ�ж�
	if (dec > 5)  
		PWM7_LevelOut(500);		
		//PWM3_LedOut(1000);
	if (dec < -5)
		PWM7_LevelOut(0);		
		//PWM3_LedOut(0) ;
}
/**************PID����******************/
 // 0-200mm  Һλ,0-200
int ek = 0, ek_1 = 0, ek_2 = 2, pk, pk_1;
float kpf, kif, kdf;

u16 PID()

{   u16 Pk;
//	u16 SV;
//	SV	= (u16)((value_1[2]*210.91)-1044.54)*100;		 // �趨ֵ
//	if (SV<=0) SV =0;
	kpf = kp / 100;
	kif = ki / 100;
	kdf = kd / 100;
	ek_2 = ek_1;
	ek_1 = ek;
	ek = SV - CV;
	Pk = pk_1 + kpf * (ek - ek_1) + kif * (ek) + kdf * (ek - ek_1 * 2 + ek_2);
	pk_1 = Pk;

	return Pk;
}
/**************���ѡ��:�ֶ�,˫λ,PID******************/
void OutChose(void)
{
	u16 pid_out;
	//	pid_out =(PID()-1044.54)/210.91;
	pid_out = PID()*10+100;       // 0-200;    //��Ҫ���ڲ���
	switch (value_1[6])
	{
	case 0:   //�ֶ�����
	{
//		PWM3_LedOut(value_1[1]);
//		break;
		PWM7_LevelOut(value_1[7]+10);   // ֱ�����0-10v��ѹ  ��0.10vѹ��
		break;
	}
	case 1:  // ˫λ����
	{
		Limit_Contl();
		break;
	}
	case 2: // PID����
	{
		//PWM3_LedOut(pid_out);      // ����
		PWM7_LevelOut(pid_out);   break;
	}
	default : PWM7_LevelOut(0); break;
	}
}

/**************ADC����******************/

//void ADC_Simple(void)
//{
//	float actualV,level; // ʵ�ʵ�ѹֵ 
//	u16 temp;
//	if (Timerflag)
//	{
//		Timerflag = 0;  // �����־λ
//		//value_2[2] = getADC10bitResult(0); // Led�������
//		             value_2[2]=getADC10bitResult(1);   // ˮ��Һλ�������
//		actualV = (value_2[2]*Vref)/1024;
//		level = actualV*13.33-13.05;
//		if (level<=0)	  level = 0;
//		else
//		{
//			value_2[0] = (u16)(level*100);
//			sprintf(OLED_CV,"%4.2f",level);     
//			OLED_display_str(3,42,OLED_CV);   // ��ʾʵ�ʵ�ѹֵ	
//		}
//	}

//}


void ADC_Simple(void)
{
	u16 temp1,temp2,temp3;
	float actualV ,Curlevel,oledl; // ʵ�ʵ�ѹֵ 
	u16 temp;
	if (Timerflag)
	{
		Timerflag = 0;  // �����־λ
		//value_2[2] = getADC10bitResult(0); // Led�������
		           temp1=getADC10bitResult(6);   // ˮ��Һλ�������
				   temp2=getADC10bitResult(6); 
				   temp3=getADC10bitResult(6); 
				   temp=(temp1+temp2+temp3)/3;
		actualV = (temp*Vref)/1024;
		//Curlevel = actualV*13.33-12.95;      // ��������������
		Curlevel = actualV*13.33-13.95;
		oledl= actualV*14.08-13.24; 
		if(oledl<=0)  oledl=0;
		 value_2[1] = (u16)(oledl*10); 			
		if (Curlevel<=0)	  Curlevel = 0;	
		value_2[2] = (u16)(Curlevel*10);         // ʵ��ֵ
//		sprintf(OLED_CV,"%-2.2f",Curlevel);     
//		OLED_display_str(4,42,OLED_CV);   // ��ʾʵ�ʵ�ѹֵ	

	}

}



void main(void)
{
	GPIO_Init();   		// GPIO��ʼ��
	TM1638_Init();  	// ���������ģ���ʼ��
	//OLED_Init();                    // OLED��ʼ��
	LedDisplay1(16, 29, 30, 28, 28, 0, 16, 16); // �������ʾ��ӭ����
//	OLED_Show_Chinese(0,0,0);  
//	OLED_Show_Chinese(0,16,1); 
//	OLED_Show_Chinese(0,32,2);   
//	OLED_Show_Chinese(0,50,3);  
//	OLED_Show_Chinese(0,66,4);  
//	OLED_Show_Chinese(0,82,5); 
//	OLED_Show_Chinese(0,98,6);   //��ʾPIDˮ����� 
//	
//	OLED_display_str(4,0,"CurV:");
//	OLED_display_str(4,90,"MM");
//		
	DelayUS(10);
	ADC_Init();   		// ADC��ʼ��
	PWM_Init();  		// PWM ��ʼ��
	Timer0_Init();   	// ��ʱ����ʼ��
	// PID_Init();      // PID��ʼ��
	EA = 1;       		// �������ж�
	PWM_LED = 0;   		// ��ʼ�����,��ʾ������
	PWM_Level = 0;      // ˮ��PWM����˿ڳ�ʼ��
	DelayMS(1000);		// �ϵ���ʱ�ȴ�ϵͳ�ȶ�
	while (1)
	{
		ADC_Simple();       	 // ADC����
		OutChose();    			 // ���ѡ��
		keyValue = KeyScan();    // ��ȡ��ֵ
		if (keyValue != 0xff)    // ����а�������
		{
			KeyProcess();            // ��������
			while (keyValue == KeyScan());  // �ȴ������ͷ�
		}
		if (!setVflag) 	// �˵���ʾ
		{
			display_Value(symb1, -1, value_1[symb1],setValue);
			
		}		
		else 
		{					
			display_Value(symb1, symb2, value_1[symb1], value_2[symb2]);
		}
	}
}
