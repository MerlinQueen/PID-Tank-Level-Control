/*
* @Author: Merlin
* @Date:   2019-05-18 15:09:04
* @Last Modified by:   Administrator
* @晶振频率:18.432MHz
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
#define Vref  5.0   // 参考电压

#define kp  value_1[3]
#define ki  value_1[4]
#define kd  value_1[5]
#define CV  value_2[2]
#define SV  value_1[2]

//u8 OLED_CV[5];   // 存放OLED----当前电压

u8 keyValue;  		// 存储按键键值

u16  value_1[8] = {0, 0, 77, 100, 50, 20, 0, 0};     //3_p,4-i,5-d          // 基础整定 P100,i50,d20
u16  value_2[3] = {0, 1, 2};

/**************GPIO端口初始化******************/
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
/**************显示设置******************/
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


/**************设置采样周期******************/
bit Timerflag = 0; 
u8 Timercount = 0;
void Timer0_Init(void)		//1毫秒@18.432MHz   定时器0 用于设置采样频率
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xB8;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;        // 开启定时器中断
}
void Timer_IRQ(void) interrupt 1
{
	Timercount++;
	if (Timercount == 100)      // 每隔 100MS进行一次ADC采样
	{
		Timercount = 0;         // 清零计数器
		Timerflag = 1;		    // 置标志位为1
	}
}

/**************按键处理******************/
u16  setValue = 0;  // 存储设定值
u8	 setVflag;  		// 设定值确定标志
u8 	 symb1 = 0, symb2 = 0; // 对应y,r,PID,o,u,n  和 L,h,r
void KeyProcess()
{
	if (keyValue <= 9)
	{
		setValue = setValue * 10 + keyValue;	 //  键值获取
	}
	else
	{
		switch (keyValue)
		{
		case 10:
		{
			symb1 = (symb1 + 1) % 8;   // y,r,PID,o,u,n选择
			break;
		}
		case 11:
		{
			symb2 = (symb2 + 1) % 3;  // L,h,r选择    h液位,L光照,r电阻
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
				value_1[symb1] = setValue;    // 把设定值传给液位 或 光照
			}
		}
		}
	}
	if (setValue > 999) setValue = 0;         
}
/**************双位控制******************/
void Limit_Contl(void)
{
	int dec;
	dec = value_1[7] - value_2[7];     // 根据电压判断
	if (dec > 5)  
		PWM7_LevelOut(500);		
		//PWM3_LedOut(1000);
	if (dec < -5)
		PWM7_LevelOut(0);		
		//PWM3_LedOut(0) ;
}
/**************PID控制******************/
 // 0-200mm  液位,0-200
int ek = 0, ek_1 = 0, ek_2 = 2, pk, pk_1;
float kpf, kif, kdf;

u16 PID()

{   u16 Pk;
//	u16 SV;
//	SV	= (u16)((value_1[2]*210.91)-1044.54)*100;		 // 设定值
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
/**************输出选择:手动,双位,PID******************/
void OutChose(void)
{
	u16 pid_out;
	//	pid_out =(PID()-1044.54)/210.91;
	pid_out = PID()*10+100;       // 0-200;    //需要调节部分
	switch (value_1[6])
	{
	case 0:   //手动设置
	{
//		PWM3_LedOut(value_1[1]);
//		break;
		PWM7_LevelOut(value_1[7]+10);   // 直接输出0-10v电压  有0.10v压降
		break;
	}
	case 1:  // 双位控制
	{
		Limit_Contl();
		break;
	}
	case 2: // PID控制
	{
		//PWM3_LedOut(pid_out);      // 反馈
		PWM7_LevelOut(pid_out);   break;
	}
	default : PWM7_LevelOut(0); break;
	}
}

/**************ADC采样******************/

//void ADC_Simple(void)
//{
//	float actualV,level; // 实际电压值 
//	u16 temp;
//	if (Timerflag)
//	{
//		Timerflag = 0;  // 清除标志位
//		//value_2[2] = getADC10bitResult(0); // Led采样结果
//		             value_2[2]=getADC10bitResult(1);   // 水箱液位采样结果
//		actualV = (value_2[2]*Vref)/1024;
//		level = actualV*13.33-13.05;
//		if (level<=0)	  level = 0;
//		else
//		{
//			value_2[0] = (u16)(level*100);
//			sprintf(OLED_CV,"%4.2f",level);     
//			OLED_display_str(3,42,OLED_CV);   // 显示实际电压值	
//		}
//	}

//}


void ADC_Simple(void)
{
	u16 temp1,temp2,temp3;
	float actualV ,Curlevel,oledl; // 实际电压值 
	u16 temp;
	if (Timerflag)
	{
		Timerflag = 0;  // 清除标志位
		//value_2[2] = getADC10bitResult(0); // Led采样结果
		           temp1=getADC10bitResult(6);   // 水箱液位采样结果
				   temp2=getADC10bitResult(6); 
				   temp3=getADC10bitResult(6); 
				   temp=(temp1+temp2+temp3)/3;
		actualV = (temp*Vref)/1024;
		//Curlevel = actualV*13.33-12.95;      // 传感器参数整定
		Curlevel = actualV*13.33-13.95;
		oledl= actualV*14.08-13.24; 
		if(oledl<=0)  oledl=0;
		 value_2[1] = (u16)(oledl*10); 			
		if (Curlevel<=0)	  Curlevel = 0;	
		value_2[2] = (u16)(Curlevel*10);         // 实际值
//		sprintf(OLED_CV,"%-2.2f",Curlevel);     
//		OLED_display_str(4,42,OLED_CV);   // 显示实际电压值	

	}

}



void main(void)
{
	GPIO_Init();   		// GPIO初始化
	TM1638_Init();  	// 键盘数码管模块初始化
	//OLED_Init();                    // OLED初始化
	LedDisplay1(16, 29, 30, 28, 28, 0, 16, 16); // 数码管显示欢迎界面
//	OLED_Show_Chinese(0,0,0);  
//	OLED_Show_Chinese(0,16,1); 
//	OLED_Show_Chinese(0,32,2);   
//	OLED_Show_Chinese(0,50,3);  
//	OLED_Show_Chinese(0,66,4);  
//	OLED_Show_Chinese(0,82,5); 
//	OLED_Show_Chinese(0,98,6);   //显示PID水箱控制 
//	
//	OLED_display_str(4,0,"CurV:");
//	OLED_display_str(4,90,"MM");
//		
	DelayUS(10);
	ADC_Init();   		// ADC初始化
	PWM_Init();  		// PWM 初始化
	Timer0_Init();   	// 定时器初始化
	// PID_Init();      // PID初始化
	EA = 1;       		// 开启总中断
	PWM_LED = 0;   		// 初始化完成,提示灯亮起
	PWM_Level = 0;      // 水箱PWM输出端口初始化
	DelayMS(1000);		// 上电延时等待系统稳定
	while (1)
	{
		ADC_Simple();       	 // ADC采样
		OutChose();    			 // 输出选择
		keyValue = KeyScan();    // 获取键值
		if (keyValue != 0xff)    // 如果有按键按下
		{
			KeyProcess();            // 按键处理
			while (keyValue == KeyScan());  // 等待按键释放
		}
		if (!setVflag) 	// 菜单显示
		{
			display_Value(symb1, -1, value_1[symb1],setValue);
			
		}		
		else 
		{					
			display_Value(symb1, symb2, value_1[symb1], value_2[symb2]);
		}
	}
}
