#ifndef __PWM_H__
#define __PWM_H__

/************管脚定义***********************
 * PWM3------P4.5-----LED灯的PWM
 * PWM7------P1.7-----水箱输出的PWM

 *******************************************/
sbit PWM_LED = P4^ 5;
sbit PWM_Level= P1^ 7;
void PWM_Init(void);      // PWM 初始化开启PWM模块
//void PWM3_LedOut(unsigned int Duty);   // 占空比输出范围0-1000
void PWM7_LevelOut(unsigned int Duty); // 占空比输出范围0-1000
	

#endif
