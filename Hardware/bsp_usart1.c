#include "bsp_usart1.h"
#include "stdio.h"
void Usart1_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
			
	/* config USART1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* USART1 GPIO config */	
  	GPIO_PinAFConfig(MCU_UART1_TX_PORT, GPIO_PinSource9,GPIO_AF_1);
	GPIO_PinAFConfig(MCU_UART1_RX_PORT, GPIO_PinSource10,GPIO_AF_1);

	/* Configure USART1 Tx (PA.09) */
	GPIO_InitStructure.GPIO_Pin = MCU_UART1_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(MCU_UART1_TX_PORT, &GPIO_InitStructure);

	/* Configure USART2 Rx (PA.10) */
	GPIO_InitStructure.GPIO_Pin = MCU_UART1_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(MCU_UART1_RX_PORT, &GPIO_InitStructure);

	/* USART1 mode config */
	#ifdef __FPRT_BYD__
	USART_InitStructure.USART_BaudRate = 115200;
	#else
	USART_InitStructure.USART_BaudRate = 115200;
	#endif
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	/* 使能串口1接收中断 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_Cmd(USART1, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	  //使能UART1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void Usart1_Disable(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	USART_Cmd(USART1, DISABLE);
	
	/* config USART1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);
	
	/* Configure USART2 Tx (PA.9) */
	GPIO_InitStructure.GPIO_Pin = MCU_UART1_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(MCU_UART1_RX_PORT, &GPIO_InitStructure);
				
	/* Configure USART2 Rx (PA.10) */
	GPIO_InitStructure.GPIO_Pin = MCU_UART1_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(MCU_UART1_RX_PORT, &GPIO_InitStructure);

}


//字节传输
void Usart1_SendByte(uint8_t data)
{
		USART_SendData(USART1,data);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET)
			;
}

//字符串传输
void Usart1_SendString(uint8_t* Data,uint8_t len)
{
		uint8_t i = 0;
		for(;i<len;i++)
		{
				#ifdef __DEBUG2__
				//printf("Data[%d] = 0x%02x\r\n",i,Data[i]);
				#endif
				Usart1_SendByte(Data[i]);
		}
}

//全部字符传输
void Usart1_SendStringALL(char* Data)
{
	while(*Data)
	{
		Usart1_SendByte(*Data++);
	}
}

//接受数据
uint8_t UART1_ReceiveByte(void)
{
	uint8_t USART1_RX_BUF;

	/* 等待接收完成 */
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		;

	USART1_RX_BUF = (uint8_t) USART_ReceiveData(USART1 );

	return USART1_RX_BUF;
}

#if 1
int SendChar (int ch)  {
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{
		;
	}
	USART_SendData(USART1, (uint8_t) ch);
  return (ch);
}
int GetKey (void)  {
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{
		;
	}
  return (int)USART_ReceiveData(USART1);
}

#endif


