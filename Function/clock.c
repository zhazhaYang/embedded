#include "clock.h"
#include "beep.h"
#include "uartContact.h"

void delay(int i)
{
	while(i--);
}
void led(void)
{
	RCC_APB2ENR |= 1<<4;
	GPIOC_CRL &= ~( 0x0F<< (4*0));
	GPIOC_CRL |= (3<<4*0);
	GPIOC_BSRR=(1<<(16+0));
	while(1)
	{
		GPIOC_BSRR=(1<<(16+0));
		delay(0xFFFFF);
		GPIOC_BSRR=(1<<0);
		delay(0xFFFFF);
	}
}
void enableClock()
{
	unsigned int temp=0;
	if(1)//BKP_DR_1 != RTC_CF)
	{
		RCC_APB1ENR |= (1<<28);
		RCC_APB1ENR |= (1<<27);
		PWR_CR |= (1<<8);
		RCC_BDCR |= (1<<16);
		RCC_BDCR &= ~(1<<16);
		RCC_BDCR |= (1<<0);
		while(((RCC_BDCR & (1<<1))==0) && temp<250)
		{
			temp++;
			delay(100000);
		}
		
		if(temp>=250)
		{
		}
		
		RCC_BDCR |= (1<<8);
		RCC_BDCR |= (1<<15);

		while((RTC_CRL & (1<<5))==0) {}
		//RTC_CRL &= (0<<3);
		while((RTC_CRL & (1<<3))==0) {}
		RTC_CRH |= (1<<0);	//¿ªÆôÃëÖÐ¶Ï
		RTC_CRH |= (1<<1);	//¿ªÆôÄÖÖÓÖÐ¶Ï
		while((RTC_CRL & (1<<5))==0) {}

		RTC_CRL |= (1<<4);
		RTC_PRLH = (0x0000);
		RTC_PRLL = 0x7FFF;
		while((RTC_CRL & (1<<5))==0) {}
		RTC_CNTH = 0x0000;
		RTC_CNTL |= 0x0001;
		RTC_CRL &= ~(1<<4);
		while((RTC_CRL & (1<<5))==0) {}
		
		BKP_DR_ALARM_HC=0;
		BKP_DR_ALARM_LC=0;
		BKP_DR_ALARM_H1=0;
		BKP_DR_ALARM_L1=0;
		BKP_DR_ALARM_H2=0;
		BKP_DR_ALARM_L2=0;
			
		BKP_DR_1 = RTC_CF;
	}
//	else
//	{
//		while(!(RTC_CRL & (1<<3))) {}
//		RTC_CRH |= (1<<0);
//		while(!(RTC_CRL & (1<<5))) {}
//	}

	MY_NVIC_Init(0,0,RTC_IRQn,2);
}

void setAlarm(unsigned int time)
{
		RCC_APB1ENR |= (1<<28);
		RCC_APB1ENR |= (1<<27);
		PWR_CR |= (1<<8);
		
		RTC_CRL |= (1<<4);
		while((RTC_CRL & (1<<5))==0) {}
		RTC_ALRH = ((time>>16) & 0xFFFF);
		RTC_ALRL = time & 0xFFFF;
		RTC_CRL &= ~(1<<4);
		while((RTC_CRL & (1<<5))==0) {}
}

void setTime(unsigned int time)
{
		RCC_APB1ENR |= (1<<28);
		RCC_APB1ENR |= (1<<27);
		PWR_CR |= (1<<8);
		
		RTC_CRL |= (1<<4);
		while((RTC_CRL & (1<<5))==0) {}
		RTC_PRLH = ((time>>16) & 0xFFFF);
		RTC_PRLL = time & 0xFFFF;
		RTC_CRL &= ~(1<<4);
		while((RTC_CRL & (1<<5))==0) {}
}


unsigned int getCounter()
{
	unsigned int count=0;
//	int c1=0;
	count = RTC_CNTL;
//	c1=((unsigned int)RTC_CNTH<<16) | count;
	return (((unsigned int)RTC_CNTH<<16) | count);
}

int sendTimeCounter=0;
void RTC_IRQHandler(void)
{
	int j=0;
	unsigned short d[3];
	unsigned int g;
	if((RTC_CRL & (1<<0))==1)
	{
		if(sendTimeCounter==10)
		{
				g=getCounter();
				d[0] = (g/3600)%24;
				d[1] = (g/60)%60;
				d[2] = g%60;
				while(j<3)
				{
					sendData(d[j]);
					j++;
				}
				sendTimeCounter=0;
		}
		sendTimeCounter++;
	}
	if(RTC_CRL & (1<<1))
	{
		sendData(0x64);
		runBeep(3500);
		setNextAlarm();
		RTC_CRL &= ~(0x0002);
	}
	RTC_CRL &= ~(0x0001);
	//while((RTC_CRL & (1<<5))==0) {}
}

void setNextAlarm(void)
{
	unsigned int alarm;
	alarm=(BKP_DR_ALARM_H1<<16)|BKP_DR_ALARM_L1;
	if(alarm>0)
	{

		setAlarm(alarm);
		BKP_DR_ALARM_HC=BKP_DR_ALARM_H1;
		BKP_DR_ALARM_LC=BKP_DR_ALARM_L1;
		BKP_DR_ALARM_H1=BKP_DR_ALARM_H2;
		BKP_DR_ALARM_L1=BKP_DR_ALARM_L2;
		BKP_DR_ALARM_H2=0;
		BKP_DR_ALARM_L2=0;
	}
	else
	{
		BKP_DR_ALARM_HC=0;
		BKP_DR_ALARM_LC=0;
	}
}

