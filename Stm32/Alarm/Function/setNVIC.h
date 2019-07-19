#ifndef __SETNVIC_H
#define __SETNVIC_H	  
#include <stm32f10x.h> 

void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);//设置NVIC分组
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);//设置中断

#endif
