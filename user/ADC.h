#ifndef __ADC_H__
#define __ADC_H__
#include "public.h"

/************管脚定义***********************
 * AD0-------P1.0-----LED灯的采样
 * ADC1------P1.1-----水箱输4-20MA电流信号采样  对应1-5v电压信号

 *******************************************/
void ADC_Init(void);
u16 getADC10bitResult(char channel);  //获取ADC返回的数据 ,channel 0-7 对应ADCP1^0-P1^7端口
#endif                  