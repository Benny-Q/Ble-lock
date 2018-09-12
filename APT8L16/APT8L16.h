#ifndef _APT8L16_H_
#define _APT8L16_H_

#include "mains.h"
#ifdef __TOUCH_KEY_SUPPORT__

extern uint8_t WIFI_FLAG;
/************************************************************************
*MCU引脚定义                                               
*************************************************************************/


//For locker's safty latch launch or retract status
#define 		APT_INT_EXTI_LINE 	EXTI_Line2
#define 		APT_INT_EXTI_PORT 	EXTI_PortSourceGPIOB
#define 		APT_INT_EXTI_SOUR 	GPIO_PinSource2

#define			APT_INT_STATUS		GPIO_ReadInputDataBit(TOUCHKEY_INT_PORT, TOUCHKEY_INT_PIN)//读INT信号
#define			APT_SDA_STATUS		GPIO_ReadInputDataBit(TOUCHKEY_SDA_PORT, TOUCHKEY_SDA_PIN)//读SDA信号

#define			APT_SDA_HIGH		TOUCHKEY_SDA_PORT->BSRR = TOUCHKEY_SDA_PIN //SDA置1
#define			APT_SDA_LOW		 	TOUCHKEY_SDA_PORT->BRR  = TOUCHKEY_SDA_PIN //SDA置0

#define			APT_SCL_HIGH		TOUCHKEY_SCL_PORT->BSRR = TOUCHKEY_SCL_PIN //SCL置1
#define			APT_SCL_LOW		 	TOUCHKEY_SCL_PORT->BRR  = TOUCHKEY_SCL_PIN //SCL置0
/************************************************************************
* APT内部寄存器地址定义                                                
*************************************************************************/
//#define APT8L16_DEVICE_ADDR   0xae //ADS接地0xae
#define APT8L16_DEVICE_ADDR   0xac //ADS接高0xac
#define GT316L_DEVICE_ADDR 	  0xB0

#define			APT_K00_OR_ADD    0x0
#define 		APT_K01_OR_ADD    0x1
#define 		APT_K02_OR_ADD    0x2
#define 		APT_K03_OR_ADD    0x3
#define			APT_K04_OR_ADD    0x4
#define			APT_K05_OR_ADD    0x5
#define			APT_K06_OR_ADD    0x6
#define			APT_K07_OR_ADD    0x7

#define			APT_K10_OR_ADD    0x8
#define			APT_K11_OR_ADD    0x9
#define			APT_K12_OR_ADD    0xA
#define			APT_K13_OR_ADD    0xB
#define			APT_K14_OR_ADD    0xC
#define			APT_K15_OR_ADD    0xD
#define			APT_K16_OR_ADD    0xE
#define			APT_K17_OR_ADD    0xF

#define			APT_GSR_ADD       0x20
#define			APT_MCON_ADD      0x21
#define			APT_BUR_ADD       0x22
#define			APT_K0_DR_ADD  	  0x23
#define			APT_K1_DR_ADD     0x24

#define			APT_DMR1_ADD      0x2A
#define			APT_DMR2_ADD      0x2D

#define			APT_K0_CON_ADD    0x18
#define			APT_K1_CON_ADD    0x19
#define			APT_K0_OUT_ADD    0x1A
#define			APT_K1_OUT_ADD    0x1B
#define			APT_K0_IN_ADD     0x1C
#define			APT_K1_IN_ADD     0x1D

#define			APT_K0_VR_ADD     0x34
#define			APT_K1_VR_ADD     0x35
#define			APT_SYSCON_ADD    0x3A

#define 		APT_TOUCH_KEY_SUPPORT_QUAN	16
#define  KEY_1	1
#define  KEY_2	2
#define  KEY_3	3

#define  KEY_4	4
#define  KEY_5	5
#define  KEY_6	6

#define  KEY_7	7
#define  KEY_8	8
#define  KEY_9	9

#define  KEY_STAR	10
#define  KEY_0	0 
#define  KEY_SHARP	11
#define  KEY_BELL   12
#define	 KEY_ENTER	0xF0
#define  KEY_NULL	NULL



#define KEY_LONG_PRESS_CNT_THD	2
extern uint8_t key_map_status[16][2];
extern uint16_t APTTouchKeyreg_value;

/************************************************************************
* 应用函数                                                
*************************************************************************/


extern void	APT_ScanTouchKey(void);
extern void APT_Touchkey_EXTI_reStart(void);
extern uint8_t _get_APT_Touch_Key_Quantity(void);
extern uint8_t _get_key_quantity_in_once_press(void);
extern void APT_I2C_init(void);
extern void APT_EXTI_init(void);
extern void	APT_Init(void);
extern void APT_Close(void);
extern void APT_low_power(void);
extern uint16_t _clean_APT_TouchKey_reg(void);
extern uint16_t _get_APT_TouchKey_reg(void);
extern uint16_t _get_APT_TouchKey_longpress_reg(void);
extern uint8_t _get_APT_TouchKey_status(void);
extern void _clear_APT_Touchkey_status(void);
extern void _set_APT_Touchkey_status(uint8_t _status);
extern void key_value_remap(uint16_t _key_value);
extern void long_press_key_value_remap(uint16_t _long_press_key_value);
extern void APT_Key_Long_Press_Scan(void);
extern void key_value_reset(void);
extern void WriteByte(unsigned char APTWrAdd, unsigned char APTWrData);
extern void ReadChipReg(unsigned char APTWrAdd);
extern u16 APT_Get_Reg(void);

#endif




#endif

