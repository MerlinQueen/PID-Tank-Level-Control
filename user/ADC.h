#ifndef __ADC_H__
#define __ADC_H__
#include "public.h"

/************�ܽŶ���***********************
 * AD0-------P1.0-----LED�ƵĲ���
 * ADC1------P1.1-----ˮ����4-20MA�����źŲ���  ��Ӧ1-5v��ѹ�ź�

 *******************************************/
void ADC_Init(void);
u16 getADC10bitResult(char channel);  //��ȡADC���ص����� ,channel 0-7 ��ӦADCP1^0-P1^7�˿�
#endif                  