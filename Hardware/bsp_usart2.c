#include "bsp_usart2.h"
#include "stdio.h"

void Usart2_init()
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		/* config USART2 clock */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
		/* USART2 GPIO config */
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,GPIO_AF_1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,GPIO_AF_1);

		/* Configure USART2 Tx (PA.02) */
		GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 	 	GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);
		
		/* Configure USART2 Rx (PA.03) */
		GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(USART2_RX_PORT, &GPIO_InitStructure);
		
		/* USART2 mode config */
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART2, &USART_InitStructure);

		/* 使能串口2接收中断 */
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART2, ENABLE);
		//USART_ClearFlag(USART2,USART_FLAG_TC);
		
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	  //使能UART1中断通道
		NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}
void Usart2_Disable(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
	USART_Cmd(USART2, DISABLE);
	
	/* config USART1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);
	
	/* Configure USART2 Tx (PA.9) */
	GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//GPIO_Mode_OUT
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);
				
	/* Configure USART2 Rx (PA.10) */
	GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//GPIO_Mode_OUT
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(USART2_RX_PORT, &GPIO_InitStructure);

}


//字节传输
void Usart2_SendByte(uint8_t data)
{
		USART_SendData(USART2,data);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET)
			;
}

//字符串传输
void Usart2_SendString(uint8_t* Data,uint8_t len)
{
		uint8_t i = 0;
		for(;i<len;i++)
		{
				Usart2_SendByte(Data[i]);
		}
}

//全部字符传输
void USART2_SendStringALL(char* Data)
{
	while(*Data)
	{
		Usart2_SendByte(*Data++);
	}
}

//接受数据
uint8_t UART2_ReceiveByte(void)
{
	uint8_t USART2_RX_BUF;

	/* 等待接收完成 */
	while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
		;

	USART2_RX_BUF = (uint8_t) USART_ReceiveData(USART2 );

	return USART2_RX_BUF;
}

#if 0
int SendChar (int ch)  {
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{
		;
	}
	USART_SendData(USART2, (uint8_t) ch);
  return (ch);
}
int GetKey (void)  {
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{
		;
	}
  return (int)USART_ReceiveData(USART2);
}

#endif

