#include "beep.h"

void initBEEP(void)
{
	RCC->APB2ENR|=1<<3;    	//ʹ��PBA2���ߵ�IO�˿�Bʱ��	   	  
	
	GPIOB->CRL &= ~(0x0F<<20);
  	
	GPIOB->CRL |= (0X03<<20);	//PB5 ���ģʽ    
	
	BEEP &= ~(1<<5);					//�رշ��������
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





