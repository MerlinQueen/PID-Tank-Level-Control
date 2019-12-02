#include "TM1638.h"
#include "delay.h"
#include "public.h"
//unsigned char data DisBuffer[8]={0,0,0,0,0,0,0,0};    /*ÏÔÊ¾»º´æÇø*/	//¸÷¸öÊýÂë¹ÜÏÔÊ¾µÄÖµ
unsigned char code tab[] = {
	0x3F, 0x06, 0x5B, 0x4F,
	0x66, 0x6D, 0x7D, 0x07,
	0x7F, 0x6F, 0x77, 0x7C,
	0x39, 0x5E, 0x79, 0x71, 0x40, //16                                              //0x6e,0x54,0x73,0x30,0x5e,0x50, 0x3e,
	0x50, 0x74, 0x38, //r h L
	0x54, 0x6e, 0x50, 0x73, 0x30, 0x5e, 0x5c, 0x1c, //n y r p i d o u
	0x38, 0x76, 0x79 , 0x3e //L H E U 28 29 30 31
};


void TM1638_Write(unsigned char	DATA)			//Ð´Êý¾Ýº¯Êý
{
	unsigned char i;
	for (i = 0; i < 8; i++)
	{
		CLK = 0;
		DelayUS(1);
		if (DATA & 0X01)
			DIO = 1;
		else
			DIO = 0;
		DATA >>= 1;
		CLK = 1;
		DelayUS(1);
	}
}
unsigned char TM1638_Read(void)					//¶ÁÊý¾Ýº¯Êý
{
	unsigned char i;
	unsigned char temp = 0;
	DIO = 1;	//ÉèÖÃÎªÊäÈë
	for (i = 0; i < 8; i++)
	{

		temp >>= 1;
		CLK = 0;
		DelayUS(1);
		if (DIO)
			temp |= 0x80;
		CLK = 1;
		DelayUS(1);
	}
	return temp;
}
void Write_COM(unsigned char cmd)		//·¢ËÍÃüÁî×Ö
{
	STB = 0;
	DelayUS(1);
	TM1638_Write(cmd);
	DelayUS(1);
	STB = 1;
}

unsigned char KeyScan(void)
{
	u8 c[4], sum = 0, i = 0;
	u8 key_value = 0xff;
	STB = 0;
	DelayUS(1);
	TM1638_Write(0x42);
	DelayMS(100);
	for (i = 0; i < 4; i++)
		c[i] = TM1638_Read();
	STB = 1;					//4¸ö×Ö½ÚÊý¾ÝºÏ³ÉÒ»¸ö×Ö½Ú
	if (c[0] == 0x04) 	key_value = 0;
	else  if (c[0] == 0x40) key_value = 1;
	else  if (c[1] == 0x04) key_value = 2;
	else  if (c[1] == 0x40) key_value = 3;
	else  if (c[2] == 0x04) key_value = 4;
	else  if (c[2] == 0x40) key_value = 5;
	else  if (c[3] == 0x04) key_value = 6;
	else  if (c[3] == 0x40) key_value = 7;
	else  if (c[0] == 0x02) key_value = 8;
	else  if (c[0] == 0x20) key_value = 9;
	else  if (c[1] == 0x02) key_value = 10;
	else  if (c[1] == 0x20) key_value = 11;
	else  if (c[2] == 0x02) key_value = 12;
	else  if (c[2] == 0x20) key_value = 13;
	else  if (c[3] == 0x02) key_value = 14;
	else  if (c[3] == 0x20) key_value = 15;
	else key_value = 0xff;
	return (key_value);
}

void TM1638_Init(void)
{
	unsigned char i;
	Write_COM(0x8a);//ÁÁ¶È
	Write_COM(0x40); //Ð´ Êý¾ÝÃüÁî
	STB = 0;
	TM1638_Write(0xc0);		//Ð´µØÖ·ÃüÁî
	for (i = 0; i < 16; i++)
		TM1638_Write(0xff);
	STB = 1;
}

void LedDisplay1(unsigned char ddata0, unsigned char ddata1, unsigned char ddata2, unsigned char ddata3,
                 unsigned char ddata4, unsigned char ddata5, unsigned char ddata6, unsigned char ddata7)
{


	unsigned char  writedata0, writedata1, writedata2, writedata3, writedata4, writedata5, writedata6, writedata7;
	unsigned char  data0, data1, data2, data3, data4, data5, data6, data7;
	data0 = ddata7;
	data1 = ddata6;
	data2 = ddata5;
	data3 = ddata4;
	data4 = ddata3;
	data5 = ddata2;
	data6 = ddata1;
	data7 = ddata0;

	writedata0 = (tab[data0] & 0x01) + ((tab[data1] & 0x01) << 1) + ((tab[data2] & 0x01) << 2) + ((tab[data3] & 0x01) << 3) + ((tab[data4] & 0x01) << 4)
	             + ((tab[data5] & 0x01) << 5) + ((tab[data6] & 0x01) << 6) + ((tab[data7] & 0x01) << 7);

	writedata1 = ((tab[data0] & 0x02) >> 1) + ((tab[data1] & 0x02)) + ((tab[data2] & 0x02) << 1) + ((tab[data3] & 0x02) << 2) + ((tab[data4] & 0x02) << 3)
	             + ((tab[data5] & 0x02) << 4) + ((tab[data6] & 0x02) << 5) + ((tab[data7] & 0x02) << 6);

	writedata2 = ((tab[data0] & 0x04) >> 2) + ((tab[data1] & 0x04) >> 1) + ((tab[data2] & 0x04)) + ((tab[data3] & 0x04) << 1) + ((tab[data4] & 0x04) << 2)
	             + ((tab[data5] & 0x04) << 3) + ((tab[data6] & 0x04) << 4) + ((tab[data7] & 0x04) << 5);

	writedata3 = ((tab[data0] & 0x08) >> 3) + ((tab[data1] & 0x08) >> 2) + ((tab[data2] & 0x08) >> 1) + ((tab[data3] & 0x08)) + ((tab[data4] & 0x08) << 1)
	             + ((tab[data5] & 0x08) << 2) + ((tab[data6] & 0x08) << 3) + ((tab[data7] & 0x08) << 4);

	writedata4 = ((tab[data0] & 0x10) >> 4) + ((tab[data1] & 0x10) >> 3) + ((tab[data2] & 0x10) >> 2) + ((tab[data3] & 0x10) >> 1) + ((tab[data4] & 0x10))
	             + ((tab[data5] & 0x10) << 1) + ((tab[data6] & 0x10) << 2) + ((tab[data7] & 0x10) << 3);

	writedata5 = ((tab[data0] & 0x20) >> 5) + ((tab[data1] & 0x20) >> 4) + ((tab[data2] & 0x20) >> 3) + ((tab[data3] & 0x20) >> 2) + ((tab[data4] & 0x20) >> 1)
	             + ((tab[data5] & 0x20)) + ((tab[data6] & 0x20) << 1) + ((tab[data7] & 0x20) << 2);

	writedata6 = ((tab[data0] & 0x40) >> 6) + ((tab[data1] & 0x40) >> 5) + ((tab[data2] & 0x40) >> 4) + ((tab[data3] & 0x40) >> 3) + ((tab[data4] & 0x40) >> 2)
	             + ((tab[data5] & 0x40) >> 1) + ((tab[data6] & 0x40)) + ((tab[data7] & 0x40) << 1);

	writedata7 = ((tab[data0] & 0x80) >> 7) + ((tab[data1] & 0x80) >> 6) + ((tab[data2] & 0x80) >> 5) + ((tab[data3] & 0x80) >> 4) + ((tab[data4] & 0x80) >> 3)
	             + ((tab[data5] & 0x80) >> 2) + ((tab[data6] & 0x80) >> 1) + ((tab[data7] & 0x80));

	Write_COM(0x88);//ÁÁ¶È
	Write_COM(0x40); //Ð´ Êý¾ÝÃüÁî
	STB = 0;
	TM1638_Write(0xc0);		//Ð´µØÖ·ÃüÁî

	TM1638_Write(writedata0);
	TM1638_Write(0x80);
	TM1638_Write(writedata1);
	TM1638_Write(0x00);
	TM1638_Write(writedata2);
	TM1638_Write(0x00);
	TM1638_Write(writedata3);
	TM1638_Write(0x00);
	TM1638_Write(writedata4);
	TM1638_Write(0x00);
	TM1638_Write(writedata5);
	TM1638_Write(0x00);
	TM1638_Write(writedata6);
	TM1638_Write(0x00);
	TM1638_Write(writedata7);
	TM1638_Write(0x00);

	STB = 1;


}

void LedDisplay(unsigned char ddata0, unsigned char ddata1, unsigned char ddata2, unsigned char ddata3,
                unsigned char ddata4, unsigned char ddata5, unsigned char ddata6, unsigned char ddata7)

{
	unsigned char  writedata0, writedata1, writedata2, writedata3, writedata4, writedata5, writedata6, writedata7;
	unsigned char  data0, data1, data2, data3, data4, data5, data6, data7;
	data0 = ddata7;
	data1 = ddata6;
	data2 = ddata5;
	data3 = ddata4;
	data4 = ddata3;
	data5 = ddata2;
	data6 = ddata1;
	data7 = ddata0;

	writedata0 = (tab[data0] & 0x01) + (((tab[data1] | 0x80) & 0x01) << 1) + ((tab[data2] & 0x01) << 2) + ((tab[data3] & 0x01) << 3) + ((tab[data4] & 0x01) << 4)
	             + (((tab[data5] | 0x80) & 0x01) << 5) + ((tab[data6] & 0x01) << 6) + ((tab[data7] & 0x01) << 7);

	writedata1 = ((tab[data0] & 0x02) >> 1) + (((tab[data1] | 0x80) & 0x02)) + ((tab[data2] & 0x02) << 1) + ((tab[data3] & 0x02) << 2) + ((tab[data4] & 0x02) << 3)
	             + (((tab[data5] | 0x80) & 0x02) << 4) + ((tab[data6] & 0x02) << 5) + ((tab[data7] & 0x02) << 6);

	writedata2 = ((tab[data0] & 0x04) >> 2) + (((tab[data1] | 0x80) & 0x04) >> 1) + ((tab[data2] & 0x04)) + ((tab[data3] & 0x04) << 1) + ((tab[data4] & 0x04) << 2)
	             + (((tab[data5] | 0x80) & 0x04) << 3) + ((tab[data6] & 0x04) << 4) + ((tab[data7] & 0x04) << 5);

	writedata3 = ((tab[data0] & 0x08) >> 3) + (((tab[data1] | 0x80) & 0x08) >> 2) + ((tab[data2] & 0x08) >> 1) + ((tab[data3] & 0x08)) + ((tab[data4] & 0x08) << 1)
	             + (((tab[data5] | 0x80) & 0x08) << 2) + ((tab[data6] & 0x08) << 3) + ((tab[data7] & 0x08) << 4);

	writedata4 = ((tab[data0] & 0x10) >> 4) + (((tab[data1] | 0x80) & 0x10) >> 3) + ((tab[data2] & 0x10) >> 2) + ((tab[data3] & 0x10) >> 1) + ((tab[data4] & 0x10))
	             + (((tab[data5] | 0x80) & 0x10) << 1) + ((tab[data6] & 0x10) << 2) + ((tab[data7] & 0x10) << 3);

	writedata5 = ((tab[data0] & 0x20) >> 5) + (((tab[data1] | 0x80) & 0x20) >> 4) + ((tab[data2] & 0x20) >> 3) + ((tab[data3] & 0x20) >> 2) + ((tab[data4] & 0x20) >> 1)
	             + (((tab[data5] | 0x80) & 0x20)) + ((tab[data6] & 0x20) << 1) + ((tab[data7] & 0x20) << 2);

	writedata6 = ((tab[data0] & 0x40) >> 6) + (((tab[data1] | 0x80) & 0x40) >> 5) + ((tab[data2] & 0x40) >> 4) + ((tab[data3] & 0x40) >> 3) + ((tab[data4] & 0x40) >> 2)
	             + (((tab[data5] | 0x80) & 0x40) >> 1) + ((tab[data6] & 0x40)) + ((tab[data7] & 0x40) << 1);

	writedata7 = ((tab[data0] & 0x80) >> 7) + (((tab[data1] | 0x80) & 0x80) >> 6) + ((tab[data2] & 0x80) >> 5) + ((tab[data3] & 0x80) >> 4) + ((tab[data4] & 0x80) >> 3)
	             + (((tab[data5] | 0x80) & 0x80) >> 2) + ((tab[data6] & 0x80) >> 1) + ((tab[data7] & 0x80));

	Write_COM(0x88);//ÁÁ¶È
	Write_COM(0x40); //Ð´ Êý¾ÝÃüÁî
	STB = 0;
	TM1638_Write(0xc0);		//Ð´µØÖ·ÃüÁî

	TM1638_Write(writedata0);
	TM1638_Write(0x80);
	TM1638_Write(writedata1);
	TM1638_Write(0x00);
	TM1638_Write(writedata2);
	TM1638_Write(0x00);
	TM1638_Write(writedata3);
	TM1638_Write(0x00);
	TM1638_Write(writedata4);
	TM1638_Write(0x00);
	TM1638_Write(writedata5);
	TM1638_Write(0x00);
	TM1638_Write(writedata6);
	TM1638_Write(0x00);
	TM1638_Write(writedata7);
	TM1638_Write(0x00);
	STB = 1;

}