#ifndef __BEEP_H
#define __BEEP_H	 

#include "clock.h"

#define BEEP *(unsigned long*)(0x40010C0C)
//#define BEEP PBout(8)	// BEEP,蜂鸣器接口//位带操作		   

void initBEEP(void);	//蜂鸣器初始化函数	 				    
void runBeep(int i);//响蜂鸣
#endif

















