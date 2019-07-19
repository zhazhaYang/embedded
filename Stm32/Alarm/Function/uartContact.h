#ifndef __UARTCONTACT_H
#define __UARTCONTACT_H
#include "setNVIC.h"

void initUart(void);
void sendData(unsigned short data);//unsigned short ch);
void setCurrentAlarm(unsigned int buf);
void USART1_IRQHandler(void);

#endif

