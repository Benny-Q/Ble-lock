#ifndef	 _USART1_H_
#define  _USART1_H_

#include "mains.h"



void Usart1_init(void);
void Usart1_Disable(void);

void Usart1_SendByte(uint8_t data);
void Usart1_SendString(uint8_t* Data,uint8_t len);
void Usart1_SendStringALL(char* Data);
uint8_t UART1_ReceiveByte(void);



#endif
