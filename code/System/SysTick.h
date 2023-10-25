#ifndef _SysTick_H
#define _SysTick_H

#include "system.h"

void SysTick_Init(u8 SYSCLK);
void v_delay_ms(u32 nms);
void delay_us(u32 nus);
void delay_ms(u32 nms);



#endif
