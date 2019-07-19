#include "uartContact.h"
#include "clock.h"

void initUart(void)
{
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(72*1000000)/(115200*16);
	mantissa=temp;				 
	fraction=(temp-mantissa)*16; 
  mantissa<<=4;
	mantissa+=fraction;
	
	RCC->APB2ENR|=1<<2; //ʹ��APB2����io�˿�Aʱ��   
	RCC->APB2ENR|=1<<14;
	GPIOA->CRH&=0XFFFFFF0F;
	GPIOA->CRH|=0X000000D0;//���ģʽ50MHz���������������ģ������ģʽ
	RCC->APB2RSTR|=1<<14;   
	RCC->APB2RSTR&=~(1<<14); 
	
 	USART1->BRR=0x0271;//0x1D4C;//0x1D4C������9600
	USART1->CR1 |= (1<<13);//ʹ��uart
	USART1->CR1 &= ~(1<<12);
	//USART1->CR1 |= (1<<10);
	//USART1->CR1 |= (0<<9);
	USART1->CR1 |= (1<<5);//�����ж�ʹ��
	USART1->CR1 |= (1<<3);//ʹ�ܷ���
	USART1->CR1 |= (1<<2);//ʹ�ܽ���
	USART1->CR2 = 0;//ֹͣλ1λ
	USART1->CR3 = 0;
	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//��ռ���ȼ�����Ӧ���ȼ�����ţ�����
}

void sendData(unsigned short data)//unsigned short ch)
{  
	while((USART1->SR&0X40) == 0);//�ȴ���һ�����ݷ������
	USART1->DR = (data & (unsigned short)0x01FF);      	//��DRд����
	while((USART1->SR&0X40) == 0);
}

unsigned short recivedTime[3];
unsigned short reciveTimeCounter=0;
void USART1_IRQHandler(void)
{
	//while((USART1->SR & 0x20) == 0);//�ȴ������ݼĴ����ǿ�
	unsigned int buf;
	if(USART1->SR & (1<<5))
	{
		recivedTime[reciveTimeCounter] = USART1->DR & (unsigned short)0x00FF;
		sendData(recivedTime[reciveTimeCounter]);
		reciveTimeCounter++;
		if(reciveTimeCounter==3)
		{
			reciveTimeCounter=0;
			//buf = (ch[0]<<0) | (ch[1]<<8) | (ch[2]<<16);
			buf = (recivedTime[0]*3600) + (recivedTime[1]*60) + (recivedTime[2]);
			setCurrentAlarm(buf);
		}
	}
	//USART1->CR1 &= ~(1<<6);
}

void setCurrentAlarm(unsigned int buf)
{
	unsigned int currentTime, currentAlarm, counter;
	counter=getCounter();
	currentTime=counter%86400;
	if(buf>=currentTime)
	{
		buf=buf-currentTime+counter;
	}
	else 
	{
		buf=86400-currentTime+buf+counter;
	}
	currentAlarm=(BKP_DR_ALARM_HC<<16) | BKP_DR_ALARM_LC;
	if(currentAlarm==0)
	{
		setAlarm(buf);
		BKP_DR_ALARM_HC=buf>>16;
		BKP_DR_ALARM_LC=buf & 0xFFFF;
	}
	else
	{
		if(buf<currentAlarm)
		{
			setAlarm(buf);
			BKP_DR_ALARM_H1=currentAlarm>>16;
			BKP_DR_ALARM_L1=currentAlarm & 0xFFFF;
			BKP_DR_ALARM_HC=buf>>16;
			BKP_DR_ALARM_LC=buf & 0xFFFF;
		}
		else if(buf>currentAlarm)
		{
			if(((BKP_DR_ALARM_H1<<16)|(BKP_DR_ALARM_L1))==0)
			{
				BKP_DR_ALARM_H1=buf>>16;
				BKP_DR_ALARM_L1=buf & 0xFFFF;
			}
			else
			{
				if(buf>((BKP_DR_ALARM_H1<<16)|BKP_DR_ALARM_L1))
				{
					if(((BKP_DR_ALARM_H2<<16)| BKP_DR_ALARM_L2)==0)
					{
						BKP_DR_ALARM_H2=buf>>16;
						BKP_DR_ALARM_L2=buf & 0xFFFF;
					}
					else
					{
						if(buf<((BKP_DR_ALARM_H2<<16)|BKP_DR_ALARM_L2))
						{
							BKP_DR_ALARM_H2=buf>>16;
							BKP_DR_ALARM_L2=buf & 0xFFFF;
						}
					}
				}
				else if(buf<((BKP_DR_ALARM_H1<<16)|BKP_DR_ALARM_L1))
				{
					BKP_DR_ALARM_H2=BKP_DR_ALARM_H1;
					BKP_DR_ALARM_L2=BKP_DR_ALARM_L1;
					BKP_DR_ALARM_H1=buf>>16;
					BKP_DR_ALARM_L1=buf & 0xFFFF;
				}
			}
		}
	}
}
