#ifndef __PWM_H__
#define __PWM_H__

/************�ܽŶ���***********************
 * PWM3------P4.5-----LED�Ƶ�PWM
 * PWM7------P1.7-----ˮ�������PWM

 *******************************************/
sbit PWM_LED = P4^ 5;
sbit PWM_Level= P1^ 7;
void PWM_Init(void);      // PWM ��ʼ������PWMģ��
//void PWM3_LedOut(unsigned int Duty);   // ռ�ձ������Χ0-1000
void PWM7_LevelOut(unsigned int Duty); // ռ�ձ������Χ0-1000
	

#endif
