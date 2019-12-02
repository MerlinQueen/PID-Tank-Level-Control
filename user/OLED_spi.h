#ifndef __OLED_SPI_H__
#define __OLED_SPI_H__
#include "public.h"
#include <STC15.h>
sbit SCLK = P0^4;
sbit MOSI = P0^3;
sbit OLED_RES = P0^2;
sbit OLED_DC = P0^1;   //0写命令,1写数据;
sbit OLED_CS = P0^0;


void OLED_Init(void);  // 初始化
//void OLED_cls(void);	// 清屏
void OLED_display_str(u8 y, u8 x, u8 *input_str); // 在(0-7)y行,(0-127)x列  显示字符串内容
void OLED_Show_Chinese(u8 y, u8 x, u8 N); //  显示ASCII_image.H中存储的第N个汉字

#endif
