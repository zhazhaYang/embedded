#ifndef __BEEP_H
#define __BEEP_H	 

#include "clock.h"

#define BEEP *(unsigned long*)(0x40010C0C)
//#define BEEP PBout(8)	// BEEP,�������ӿ�//λ������		   

void initBEEP(void);	//��������ʼ������	 				    
void runBeep(int i);//�����
#endif

















