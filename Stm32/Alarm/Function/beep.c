#include "beep.h"

void initBEEP(void)
{
	RCC->APB2ENR|=1<<3;    	//使能PBA2总线的IO端口B时钟	   	  
	
	GPIOB->CRL &= ~(0x0F<<20);
  	
	GPIOB->CRL |= (0X03<<20);	//PB5 输出模式    
	
	BEEP &= ~(1<<5);					//关闭蜂鸣器输出
}

void runBeep(int i)
{
	while(i>=0)
	{
			BEEP |= (1<<5);
			delay(10000);
			BEEP &= ~(1<<5);
			delay(5000);
			i--;
	}
}





