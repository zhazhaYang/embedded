#ifndef __CLOCK_H
#define __CLOCK_H
#include "setNVIC.h"

#define RTC_CF 0xA0A0

#define BKP_DR_ALARM_LC *(unsigned long*)(0x40006C08)
#define BKP_DR_ALARM_L1 *(unsigned long*)(0x40006C0C)
#define BKP_DR_ALARM_L2 *(unsigned long*)(0x40006C10)
#define BKP_DR_ALARM_HC *(unsigned long*)(0x40006C14)
#define BKP_DR_ALARM_H1 *(unsigned long*)(0x40006C18)
#define BKP_DR_ALARM_H2 *(unsigned long*)(0x40006C1C)

#define PWR_CR *(unsigned long*)(0x40007000)
#define BKP_DR_1 *(unsigned long*)(0x40006C04)

#define RCC_ADDRESS ((unsigned long)0x40021000)
#define RCC_CR *(unsigned long*)(RCC_ADDRESS+0x00)
#define RCC_CFGR *(unsigned long*)(RCC_ADDRESS+0x04)
#define RCC_CIR *(unsigned long*)(RCC_ADDRESS+0x08)
#define RCC_APB2RSTR *(unsigned long*)(RCC_ADDRESS+0x0C)
#define RCC_APB1RSTR *(unsigned long*)(RCC_ADDRESS+0x10)
#define RCC_AHBENR *(unsigned long*)(RCC_ADDRESS+0x14)
#define RCC_APB2ENR *(unsigned long*)(RCC_ADDRESS+0x18)
#define RCC_APB1ENR *(unsigned long*)(RCC_ADDRESS+0x1C)
#define RCC_BDCR *(unsigned long*)(RCC_ADDRESS+0x20)
#define RCC_CSR *(unsigned long*)(RCC_ADDRESS+0x24)

#define RTC_ADDRESS ((unsigned long)0x40002800)
#define RTC_CRH *(unsigned long*)(RTC_ADDRESS+0x00)
#define RTC_CRL *(unsigned long*)(RTC_ADDRESS+0x04)
#define RTC_PRLH *(unsigned long*)(RTC_ADDRESS+0x08)
#define RTC_PRLL *(unsigned long*)(RTC_ADDRESS+0x0C)
#define RTC_DIVH *(unsigned long*)(RTC_ADDRESS+0x10)
#define RTC_DIVL *(unsigned long*)(RTC_ADDRESS+0x14)
#define RTC_CNTH *(unsigned long*)(RTC_ADDRESS+0x18)
#define RTC_CNTL *(unsigned long*)(RTC_ADDRESS+0x1C)
#define RTC_ALRH *(unsigned long*)(RTC_ADDRESS+0x20)	
#define RTC_ALRL *(unsigned long*)(RTC_ADDRESS+0x24)
	
//led
#define GPIOC_ADDRESS       (0x40011000)

#define GPIOC_CRL 	 	 *(unsigned int*)(GPIOC_ADDRESS+0x00)
#define GPIOC_CRH 	 	 *(unsigned int*)(GPIOC_ADDRESS+0x04)
#define GPIOC_IDR 	 	 *(unsigned int*)(GPIOC_ADDRESS+0x08)
#define GPIOC_ODR 	 	 *(unsigned int*)(GPIOC_ADDRESS+0x0C)
#define GPIOC_BSRR 	 	 *(unsigned int*)(GPIOC_ADDRESS+0x10)
#define GPIOC_BRR 	 	 *(unsigned int*)(GPIOC_ADDRESS+0x14)
#define GPIOC_LCKR 	 	 *(unsigned int*)(GPIOC_ADDRESS+0x18)
	
#define EXTI_IMR *(u32*)(0x40010400)
#define EXTI_RTSR *(u32*)(0x40010408)
#define EXTI_FTSR *(u32*)(0x4001040C)

void delay(int i);
void enableClock(void);
void setAlarm(unsigned int time);
void setTime(unsigned int time);
unsigned int getCounter(void);
void setNextAlarm(void);

void RTC_IRQHandler(void);

void led(void);

#endif 

