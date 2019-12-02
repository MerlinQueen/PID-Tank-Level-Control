
#include "OLED_spi.h"
#include "ASCII_image.h"
#include "delay.h"
 void SPIdelay(u8 t)
{
	u8 i, j;
	for ( i = 0; i < t; i++)
		for (j = 0; j < 12; j++);
}

void OledWriteCom(u8 Oled_com)
{
	u8 i;   // CPOL= 0,CPHA = 0 奇次边沿采样 空闲时,SCLK 为低电平, 上升沿采样
	SCLK = 0;    // 初始时钟状态;
	OLED_DC = 0; // 0 ,write com
	OLED_CS = 0;   // 片选信号拉低,开始发送数据
	for (i = 0; i < 8; i++)
	{
		MOSI = (bit)(Oled_com & 0x80);
		SCLK = 1; 
		SPIdelay(1);
		SCLK = 0;   // 上升沿采样
		SPIdelay(1);
		Oled_com <<= 1;
	}
	OLED_CS = 1; //结束发送
}

void OledWriteDat(u8 Oled_dat)
{
	u8 i;   // CPOL= 0,CPHA = 0 奇次边沿采样 空闲时,SCLK 为低电平, 上升沿采样
	SCLK = 0;    // 初始时钟状态;
	OLED_DC = 1; // 1 ,write dat
	OLED_CS = 0;   // 片选信号拉低,开始发送数据
	for (i = 0; i < 8; i++)
	{
		MOSI = (bit)(Oled_dat & 0x80);
		SCLK = 1; 
		SPIdelay(1);
		SCLK = 0;   // 上升沿采样
		SPIdelay(1);
		Oled_dat <<= 1;
	}
	OLED_CS = 1; //结束发送
}

/*OLED坐标设置*/
void OLED_set_position(u8 y, u8 x)
{
	OledWriteCom(0xb0 + y);  // 设置页显示开始y方向 地址b0 - b7
	OledWriteCom(0x00 | (x & 0x0f));  //设置低4位点的列作为页的开始x方向地址
	OledWriteCom(0x10 | ((x & 0xf0 ) >> 4)); // 设置列的高地址作为页的开始x方向地址
}

/*OLED清屏,全灭*/
void OLED_cls(void)
{
	u8 x = 0, y = 0;
	for (y = 0; y < 8; y++)
	{
		OledWriteCom(0xb0 + y); // 逐行清零
		OledWriteCom(0x00);  //设置列低4位的作为页的开始地址
		OledWriteCom(0x10); // 设置列高4位作为页的开始地址
		for (x = 0; x < 128; x++)
			OledWriteDat(0x00);  // 0x00清屏,0xff全亮
	}
}


void OLED_Init(void)
{
	OLED_RES = 1;
	DelayMS(10);
	OLED_RES = 0;  // 低电平,复位信号输入;
	DelayMS(10);
	OLED_RES = 1;
	DelayMS(10);   //开机前延时
	OledWriteCom(0xae);  // 关闭显示

	OledWriteCom(0x00);  //设置低一点的列作为页的开始地址
	OledWriteCom(0x10); // 设置列的高地址作为页的开始地址

	OledWriteCom(0xd5);  // 设置时钟分频因子,震荡周期
	OledWriteCom(0x80);	// [3:0]分频因子,[7:4]震荡周期

	OledWriteCom(0xa8);	// 设置驱动路数
	OledWriteCom(0x3f);	// 默认0x3f

	OledWriteCom(0xd3); // 设置显示偏移
	OledWriteCom(0x00);	// 0.96存默认显示偏移为0

	OledWriteCom(0x40); // 设置显示开始行[5:0],行数

	OledWriteCom(0x8d); // 电荷泵设置
	OledWriteCom(0x14); // bit2 = 1开启0x14,bit2 = 0关闭 0x10;

	OledWriteCom(0x20); // 设置内存地址模式
	OledWriteCom(0x02); // [1:0],00 列地址模式,  02页地址模式

	OledWriteCom(0xa1); // 重定义设置
	OledWriteCom(0xc8); // 设置com扫描防线[3:0]0xa0左右反置 0xa1正常 0xc0上下反置 0xc8正常

	OledWriteCom(0xda); // 设置COm硬件引脚配置
	OledWriteCom(0x12); // [5:4]配置

	OledWriteCom(0x81); // 对比度设置
	OledWriteCom(0xcf);  // 1-255 数值越大亮度越高

	OledWriteCom(0xd9); // 设置预充电周期
	OledWriteCom(0xf1); // [3:0] 时期1, [7:4] 时期2

	OledWriteCom(0xdb); // 设置电压倍率
	OledWriteCom(0x40); // [6:4]  00,0.65*vcc,10  0.77*vcc, 30 0.83vcc, 40 vcc

	OledWriteCom(0xa4); // 全区显示开始起  0xa4开启,0xa5关闭
	OledWriteCom(0xa6); // 设置显示方式 0xa6 正常显示, 0xa7反向显示
	OLED_cls();	 	  // 初始清屏
	OledWriteCom(0xaf);  // 开启显示
}


///*   在(0-8)y行,(0-16)x列  显示字符串 *****每个字(6*8像素) */
void OLED_display_str(u8 y, u8 x, u8 *input_str)
{
	u8 i, ascii_num = 0;  // ascii_num表示字符串对应的ASCII编码数值
	while (*input_str != '\0')
	{
		if (y > 7) y %= 7;
		if (x > 120) {x = 0; y++;}
		OLED_set_position(y, x);      // 设置显示位置
		ascii_num = *input_str - 32;
		for (i = 0; i < 6; i++)
		{
			OledWriteDat(Str6x8_code[ascii_num][i]); // 送入字符串对应的数据码
		}
		x += 6;
		input_str++;
	}
}
/* 显示ACII_image.h中的汉字,16*16点阵*/
void OLED_Show_Chinese(u8 y, u8 x, u8 N)
{
	u8 i, row = 0;
	row = N * 2;  // 每个汉字又两组8*16点阵组成
	OLED_set_position(y, x);
	for (i = 0; i < 16; i++)
	{
		OledWriteDat(Chinese_code[row][i]);
	}

	OLED_set_position(y+1, x); // 分两个8*8显示
	for (i = 0; i < 16; i++)
	{
		OledWriteDat(Chinese_code[row + 1][i]);
	}
}

