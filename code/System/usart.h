#ifndef _usart_H
#define _usart_H

#include "system.h" 
#include "stdio.h" 

#define USART3_REC_LEN		200  

extern u8  USART3_RX_BUF[USART3_REC_LEN]; 
extern u16 USART3_RX_STA;         		


void USART3_Init(u32 bound);


#endif


