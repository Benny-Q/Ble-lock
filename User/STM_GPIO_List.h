#ifndef _GPIO_List_H_
#define _GPIO_List_H_


/******************* GPIOA口********************
*													   *
*				配置器件GPIOA口 				   *
*													   *
*************************************************/

#define	BT_POWER_PORT			  GPIOA		//PIN_0
#define	BT_RSTN_PORT			  GPIOA		//PIN_1
#define	USART2_TX_PORT		  	  GPIOA		//PIN_2
#define	USART2_RX_PORT			  GPIOA		//PIN_3
#define	LED_9_PORT		  		  GPIOA		//PIN_4
#define	LED_10_PORT		  		  GPIOA		//PIN_5
#define	BT_CONTROL_PORT		  	  GPIOA		//PIN_6
//#define			  				  GPIOA		//PIN_7
#define	SPEAKER_BUSY_PORT		  GPIOA		//PIN_8
#define	MCU_UART1_TX_PORT		  GPIOA		//PIN_9
#define	MCU_UART1_RX_PORT		  GPIOA		//PIN_10
#define	PF_TOUCH_OUT_PORT		  GPIOA		//PIN_11
#define	PF_POWERON_PORT 		  GPIOA		//PIN_12
//#define							  GPIOA		//PIN_13
//#define							  GPIOA		//PIN_14
//#define							  GPIOA		//PIN_15

#define	BT_POWER_PIN			  GPIO_Pin_0
#define	BT_RSTN_PIN			  	  GPIO_Pin_1
#define	USART2_TX_PIN		  	  GPIO_Pin_2
#define	USART2_RX_PIN 		  	  GPIO_Pin_3
#define LED_9_PIN		      	  GPIO_Pin_4
#define	LED_10_PIN				  GPIO_Pin_5
#define	BT_CONTROL_PIN		  	  GPIO_Pin_6
//#define			  				  GPIO_Pin_7
#define	SPEAKER_BUSY_PIN		  GPIO_Pin_8
#define	MCU_UART1_TX_PIN		  GPIO_Pin_9
#define	MCU_UART1_RX_PIN		  GPIO_Pin_10
#define	PF_TOUCH_OUT_PIN		  GPIO_Pin_11
#define	PF_POWERON_PIN  		  GPIO_Pin_12
//#define							  GPIO_Pin_13
//#define							  GPIO_Pin_14
//#define							  GPIO_Pin_15

/******************* GPIOB口********************
*													   *
*				配置器件GPIOB口 				   *
*													   *
*************************************************/



#define	LED_12_PORT			  	  GPIOB		//PIN_0
#define	LED_11_PORT				  GPIOB		//PIN_1
#define	TOUCHKEY_INT_PORT	  	  GPIOB		//PIN_2
#define	RFID_EXTI_PORT	  		  GPIOB		//PIN_3
#define	RFID_RST_PORT 			  GPIOB		//PIN_4
#define	RFID_POWER_PORT			  GPIOB		//PIN_5
//#define			  	  GPIOB		//PIN_6
//#define				  GPIOB		//PIN_7
#define	LED_4_PORT		  		  GPIOB		//PIN_8
#define	LED_3_PORT			  	  GPIOB		//PIN_9
#define	TOUCHKEY_SCL_PORT		  GPIOB		//PIN_10
#define	TOUCHKEY_SDA_PORT		  GPIOB		//PIN_11
#define	RFID_SPI_CS_PORT		  GPIOB		//PIN_12
#define	RFID_SPI_CLK_PORT		  GPIOB		//PIN_13
#define	RFID_SPI_MISO_PORT		  GPIOB		//PIN_14
#define	RFID_SPI_MOSI_PORT		  GPIOB		//PIN_15


#define	LED_12_PIN				  GPIO_Pin_0
#define	LED_11_PIN				  GPIO_Pin_1
#define	TOUCHKEY_INT_PIN 		  GPIO_Pin_2
#define	RFID_EXTI_PIN	 		  GPIO_Pin_3
#define	RFID_RST_PIN			  GPIO_Pin_4
#define	RFID_POWER_PIN			  GPIO_Pin_5
//#define				  GPIO_Pin_6
//#define				  GPIO_Pin_7
#define	LED_4_PIN		  		  GPIO_Pin_8
#define	LED_3_PIN				  GPIO_Pin_9
#define	TOUCHKEY_SCL_PIN		  GPIO_Pin_10
#define	TOUCHKEY_SDA_PIN		  GPIO_Pin_11
#define	RFID_SPI_CS_PIN			  GPIO_Pin_12
#define	RFID_SPI_CLK_PIN		  GPIO_Pin_13
#define	RFID_SPI_MISO_PIN		  GPIO_Pin_14
#define	RFID_SPI_MOSI_PIN		  GPIO_Pin_15

/******************* GPIOC口********************
*													   *
*				配置器件GPIOC口 				   *
*													   *
*************************************************/



//#define				  			  GPIOC		//PIN_0
//#define			 				  GPIOC		//PIN_1
#define	BAT_ADC_PORT			  GPIOC		//PIN_2
#define	ZIGBEE_EXTI_PORT		  GPIOC		//PIN_3
#define	HALL_PORT		  	  	  GPIOC		//PIN_4
//#define		  		  			  GPIOC		//PIN_5
//#define				  			  GPIOC		//PIN_6
//#define				  			  GPIOC		//PIN_7
#define	SPEAKER_RST_PORT		  GPIOC		//PIN_8
#define	SPEAKER_DATA_PORT		  GPIOC		//PIN_9
#define	LED_8_PORT				  GPIOC		//PIN_10
#define	LED_7_PORT				  GPIOC		//PIN_11
#define	LED_6_PORT		  		  GPIOC		//PIN_12
#define	MOTOR_PWR_EN_PORT		  GPIOC		//PIN_13
//#define							  GPIOC		//PIN_14
//#define							  GPIOC		//PIN_15


//#define				  			  GPIO_Pin_0
//#define			  				  GPIO_Pin_1
#define	BAT_ADC_PIN				  GPIO_Pin_2
#define	ZIGBEE_EXTI_PIN			  GPIO_Pin_3
#define HALL_PIN			 	  GPIO_Pin_4
//#define					 		  GPIO_Pin_5
//#define				  			  GPIO_Pin_6
//#define				  			  GPIO_Pin_7
#define	SPEAKER_RST_PIN			  GPIO_Pin_8
#define	SPEAKER_DATA_PIN		  GPIO_Pin_9
#define	LED_8_PIN				  GPIO_Pin_10
#define	LED_7_PIN				  GPIO_Pin_11
#define	LED_6_PIN		 		  GPIO_Pin_12
#define	MOTOR_PWR_EN_PIN		  GPIO_Pin_13
//#define							  GPIO_Pin_14
//#define							  GPIO_Pin_15

#define GET_PORT_SETUP			GPIO_ReadInputDataBit(Config_PORT,Config_PIN)

/******************* GPIOD口********************
*													   *
*				配置器件GPIOD口 				   *
*													   *
*************************************************/



//#define							  GPIOD		//PIN_0
//#define							  GPIOD		//PIN_1
#define	LED_5_PORT			  		 GPIOD		//PIN_2
//#define							  GPIOD		//PIN_3
//#define							  GPIOD		//PIN_4
//#define							  GPIOD		//PIN_5
//#define							  GPIOD		//PIN_6
//#define							  GPIOD		//PIN_7
//#define							  GPIOD		//PIN_8
//#define							  GPIOD		//PIN_9
//#define							  GPIOD		//PIN_10
//#define							  GPIOD		//PIN_11
//#define							  GPIOD		//PIN_12
//#define							  GPIOD		//PIN_13
//#define							  GPIOD		//PIN_14
//#define							  GPIOD		//PIN_15


//#define							  GPIO_Pin_0
//#define							  GPIO_Pin_1
#define	LED_5_PIN			  		 GPIO_Pin_2
//#define							  GPIO_Pin_3
//#define							  GPIO_Pin_4
//#define							  GPIO_Pin_5
//#define							  GPIO_Pin_6
//#define							  GPIO_Pin_7
//#define							  GPIO_Pin_8
//#define							  GPIO_Pin_9
//#define							  GPIO_Pin_10
//#define							  GPIO_Pin_11
//#define							  GPIO_Pin_12
//#define							  GPIO_Pin_13
//#define							  GPIO_Pin_14
//#define							  GPIO_Pin_15


/******************* GPIOF口********************
*													   *
*				配置器件GPIOF口 				   *
*													   *
*************************************************/

#define	MOTO1_IN2_PORT			  GPIOF		//PIN_0
#define	MOTO1_IN1_PORT			  GPIOF		//PIN_1
//#define							  GPIOF		//PIN_2
//#define							  GPIOF		//PIN_3
#define	LED_1_PORT				  GPIOF		//PIN_4
#define	LED_2_PORT				  GPIOF		//PIN_5
#define	Config_PORT		  		  GPIOF		//PIN_6
//#define			  				  GPIOF		//PIN_7
//#define							  GPIOF		//PIN_8
//#define							  GPIOF		//PIN_9
//#define							  GPIOF		//PIN_10
//#define							  GPIOF		//PIN_11
//#define							  GPIOF		//PIN_12
//#define							  GPIOF		//PIN_13
//#define							  GPIOF		//PIN_14
//#define							  GPIOF		//PIN_15


#define	MOTO1_IN2_PIN			  GPIO_Pin_0
#define	MOTO1_IN1_PIN			  GPIO_Pin_1
//#define							  GPIO_Pin_2
//#define							  GPIO_Pin_3
#define	LED_1_PIN				  GPIO_Pin_4
#define	LED_2_PIN				  GPIO_Pin_5
#define	Config_PIN		  		  GPIO_Pin_6
//#define 		  					  GPIO_Pin_7
//#define							  GPIO_Pin_8
//#define							  GPIO_Pin_9
//#define							  GPIO_Pin_10
//#define							  GPIO_Pin_11
//#define							  GPIO_Pin_12
//#define							  GPIO_Pin_13
//#define							  GPIO_Pin_14
//#define							  GPIO_Pin_15

#define GET_PORT_HALL			!GPIO_ReadInputDataBit(HALL_PORT,HALL_PIN)


#define BT_UART_RX_LOW	GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define BT_UART_TX_LOW	GPIO_ResetBits(GPIOA,GPIO_Pin_3)

#endif


