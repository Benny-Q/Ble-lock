#ifndef	 _USART2_H_
#define  _USART2_H_

#include "mains.h"


void Usart2_init(void);
void Usart2_SendByte(uint8_t data);
void Usart2_SendString(uint8_t* Data,uint8_t len);
void USART2_SendStringALL(char* Data);
uint8_t UART2_ReceiveByte(void);
void Usart2_Disable(void);



#endif

