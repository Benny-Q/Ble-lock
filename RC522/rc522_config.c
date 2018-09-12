#include "rc522_config.h"
#include "stm32f0xx.h"


void RC522_Init ( void )
{
	

  	RC522_GPIO_Config ();
    macRC522_Reset_Disable();
	
	RC522_SPI_Config ();
	
	macRC522_POWER_Enable();
}
void RFIDSleep(void)
{
  	//RC522_GPIO_Config ();
    //macRC522_Reset_Enable();	
	macRC522_POWER_Disable();
}

void RC522_GPIO_Config ( void )
{
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

	
		/*开启GPIOG的外设时钟*/
		RCC_APB2PeriphClockCmd( RCC_AHBPeriph_GPIOB, ENABLE ); 

    /* 配置RC522模块的复位（RST）引脚*/
		/*（RST）引脚*/															   
		GPIO_InitStructure.GPIO_Pin = RFID_RST_PIN;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIOB*/
		GPIO_Init( RFID_RST_PORT, &GPIO_InitStructure );	 
	
		GPIO_InitStructure.GPIO_Pin = RFID_POWER_PIN;

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_Init(RFID_POWER_PORT, &GPIO_InitStructure );
		

}


void RC522_SPI_Config ( void )
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
  

	RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
   
  
    GPIO_PinAFConfig(RFID_SPI_CS_PORT, GPIO_PinSource12,GPIO_AF_0);
    GPIO_PinAFConfig(RFID_SPI_CLK_PORT, GPIO_PinSource13,GPIO_AF_0);
    GPIO_PinAFConfig(RFID_SPI_MISO_PORT, GPIO_PinSource14,GPIO_AF_0);
    GPIO_PinAFConfig(RFID_SPI_MOSI_PORT, GPIO_PinSource15,GPIO_AF_0);
  
    /*!< Configure SPI_RC522_SPI pins: SCK */
    GPIO_InitStructure.GPIO_Pin = RFID_SPI_CLK_PIN | RFID_SPI_MISO_PIN | RFID_SPI_MOSI_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#if 0
    /*!< Configure SPI_RC522_SPI pins: MISO */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  
    /*!< Configure SPI_RC522_SPI pins: MOSI */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
    /*!< Configure SPI_RC522_SPI_CS_PIN pin: SPI_RC522 Card CS pin */
    GPIO_InitStructure.GPIO_Pin = RFID_SPI_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(RFID_SPI_CS_PORT, &GPIO_InitStructure);
  
    /* Deselect the RC522: Chip Select high */
    macRC522_CS_Disable();
  
    /* SPI1 configuration */
    // data input on the DIO pin is sampled on the rising edge of the CLK. 
    // Data on the DO and DIO pins are clocked out on the falling edge of CLK.
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
  	SPI_RxFIFOThresholdConfig(SPI2,SPI_RxFIFOThreshold_QF);
    /* Enable SPI1  */
    SPI_Cmd(SPI2, ENABLE);
	
}



