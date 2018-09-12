#include "APT8L16.h"

#ifdef __TOUCH_KEY_SUPPORT__

uint8_t key_map_status[16][2]=
{

/*	
	{KEY_NULL,0},
	{KEY_NULL,0},
	{KEY_NULL,0},
	{KEY_NULL,0},

	{KEY_STAR,0},
	{KEY_7,0},
	{KEY_4,0},
	{KEY_1,0},

	{KEY_2,0},
	{KEY_5,0},
	{KEY_8,0},
	{KEY_0,0},

	{KEY_3,0},
	{KEY_6,0},
	{KEY_9,0},
	{KEY_SHARP,0},
*/
	{KEY_NULL,0},
	{KEY_NULL,0},
	{KEY_STAR,0},
	{KEY_7,0},

	{KEY_4,0},
	{KEY_1,0},
	{KEY_2,0},
	{KEY_5,0},

	{KEY_8,0},
	{KEY_0,0},
	{KEY_3,0},
	{KEY_6,0},

	{KEY_9,0},
	{KEY_SHARP,0},
	{KEY_NULL,0},
	{KEY_NULL,0},

	
};

#define 		THKey_Debounce	 	1

/*************************/
//uint8_t WIFI_FLAG = 0;
/*****************************/
unsigned char NewTHKey0 = 0;
unsigned char OldTHKey0 = 0;
unsigned char TmpTHKey0 = 0;
unsigned char RelTHKey0 = 0;
unsigned char DebounceCNT0 = 0;

unsigned char NewTHKey1 = 0;
unsigned char OldTHKey1 = 0;
unsigned char TmpTHKey1 = 0;
unsigned char RelTHKey1 = 0;
unsigned char DebounceCNT1 = 0;
extern uint16_t APTTouchKeyreg_value =0;
uint16_t APTTouchKeyLongPress_reg_value =0;
uint8_t  APT_TouchKey_pressed =0;
uint8_t  APT_Value = 0;
//=================================================================================
static void i2c_Delay(void);
static void DelayMs(unsigned long int t);
static void APT_IIC_Start(void);
static void APT_IIC_Stop(void);
static void APT_IIC_WaitAck(void);
static void APT_IIC_SendACK(unsigned char Ack);
static void APT_IIC_Write8bit(unsigned char WrData);
static unsigned char APT_IIC_Read8bit(void);
void APT_WriteByte(unsigned char APTWrAdd, unsigned char APTWrData);
void APT_WriteChipReg(unsigned char ChipRegAdd, unsigned char ChipRegValueA);
unsigned char APT_ReadChipReg(unsigned char APTRdAdd);
void APT_ChipRegSet(void);
/**********************************************************************************/
/*
 *********************************************************************************************************
 *	函 数 名: i2c_Delay
 *	功能说明: I2C总线位延迟，最快400KHz
 *	形    参：无
 *	返 回 值: 无
 *********************************************************************************************************
 */
static void i2c_Delay(void)
{
	uint16_t i;

	/*　
	 下面的时间是通过安富莱AX-Pro逻辑分析仪测试得到的。
	 CPU主频72MHz时，在内部Flash运行, MDK工程不优化
	 循环次数为10时，SCL频率 = 205KHz
	 循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us
	 循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us
	 IAR工程编译效率高，不能设置为7


	 lijiaqin20160323
	 AX-PRO逻辑分析仪测到
	 循环次数为18时，SCL频率=102khz SCL高电平时间3.917us，SCL低电平时间5.833us
	 循环次数为10时，SCL频率=160khz SCL高电平时间2.167us，SCL低电平时间4.063us
	 循环次数为5时，SCL频率=250khz SCL高电平时间1.083us，SCL低电平时间2.917us

	 */
	for(i = 0; i < 10; i++)
		;
}

//=================================================================================
//毫秒延时,利用系统滴答，必须先初始化系统滴答才能用这个函数
//=================================================================================
static void DelayMs(unsigned long int t)
{
	t = 100 * t;
	delay_us(t);
}
//=================================================================================
//IIC起始信号
//=================================================================================
static void APT_IIC_Start(void)
{ //当SCL和SDA为高电平时，如果SDA由高变低，就产生起始条件，SCL为高时SDA的数据才有效
	APT_SDA_HIGH;
	i2c_Delay();
	APT_SCL_HIGH;
	i2c_Delay();
	APT_SDA_LOW;
	i2c_Delay();
	APT_SCL_LOW;
	i2c_Delay();
}
//==================================================================================
//IIC停止信号
//==================================================================================
static void APT_IIC_Stop(void)
{ //当SCL为高电平时，如果SDA由低变高，就产生停止条件
	APT_SCL_LOW;
	i2c_Delay();
	APT_SDA_LOW;
	i2c_Delay();
	APT_SCL_HIGH;
	i2c_Delay();
	APT_SDA_HIGH;
}
//==================================================================================
//IIC等待应答信号
//==================================================================================
static void APT_IIC_WaitAck(void)
{
	unsigned char WaitTimeCnt = 0;

	//APT_SCL_HIGH;
	//APT_SDA_PIN_INPUT;//释放SDA，设SDA为输入
	APT_SDA_HIGH; //CPU释放SDA总线
	i2c_Delay();
	APT_SCL_HIGH;

	do
	{ //如果收到低电平应答或无应答一段时间跳出死循环
		i2c_Delay();
		i2c_Delay();
		WaitTimeCnt++;
	} while((APT_SDA_STATUS) && (WaitTimeCnt < 5));

	APT_SCL_LOW;
	i2c_Delay();

	//APT_SDA_PIN_OUTPUT;//重新设SDA为输出
}
//==================================================================================
//IIC是否发送应答位 ,Ack=1发送应答，0不发送应答
//==================================================================================
static void APT_IIC_SendACK(unsigned char Ack)
{
	if(Ack == 1)
		APT_SDA_LOW; //发送应答位
	else
		APT_SDA_HIGH; //不发送应答位

	i2c_Delay();
	APT_SCL_HIGH;  //CPU产生1个时钟
	i2c_Delay();
	i2c_Delay();
	APT_SCL_LOW;
	i2c_Delay();
	APT_SDA_HIGH;  //释放SDA总线
	i2c_Delay();

}
//==================================================================================
//IIC写8位数据
//==================================================================================
static void APT_IIC_Write8bit(unsigned char WrData)
{
	unsigned char i = 0;

	for(i = 0; i < 8; i++)
	{
		if((WrData << i) & 0x80)
			APT_SDA_HIGH;
		else
			APT_SDA_LOW;

		i2c_Delay();
		APT_SCL_HIGH;
		i2c_Delay();
		i2c_Delay();
		APT_SCL_LOW;
		if(7 == i)
		{
			APT_SDA_HIGH; //释放总线
		}
		i2c_Delay();
	}
}
//==================================================================================
//IIC读8位数据
//==================================================================================
static unsigned char APT_IIC_Read8bit(void)
{
	unsigned char i = 0, RdData;

	//APT_SDA_PIN_INPUT;
	RdData = 0;
	for(i = 0; i < 8; i++)
	{
		RdData <<= 1;
		APT_SCL_HIGH;
		i2c_Delay();
		i2c_Delay();

		if(APT_SDA_STATUS)
			RdData |= 0x01;

		APT_SCL_LOW;
		i2c_Delay();
	}
	//APT_SDA_PIN_OUTPUT;

	return (RdData);
}
//==================================================================================
//MCU写数据到APT指定寄存器地址
//==================================================================================
void APT_WriteByte(unsigned char APTWrAdd, unsigned char APTWrData)
{
	APT_IIC_Start();
	#ifdef __APT_GT316L_DEVICE__
	APT_IIC_Write8bit(GT316L_DEVICE_ADDR); //1011 0000 APT固定地址，写
	#elif defined(__APT8L16__)
	APT_IIC_Write8bit(APT8L16_DEVICE_ADDR); //1010 1100 APT固定地址，写
	#endif
	APT_IIC_WaitAck();
	APT_IIC_Write8bit(APTWrAdd); //APT内部寄存器地址
	APT_IIC_WaitAck();
	APT_IIC_Write8bit(APTWrData); //写数值
	APT_IIC_WaitAck();
	APT_IIC_Stop();
}
//==================================================================================
//MCU写数据到APT指定寄存器地址，并检验，此函数可做IIC通信成功的检测
//==================================================================================
void APT_WriteChipReg(unsigned char ChipRegAdd, unsigned char ChipRegValueA)
{
	unsigned char ChipRegValueB = 0;
	unsigned char try_num = 3;   //尝试次数为3

	while(1)
	{
		APT_WriteByte(ChipRegAdd, ChipRegValueA);
		i2c_Delay();
		i2c_Delay();
		ChipRegValueB = APT_ReadChipReg(ChipRegAdd);

		if(ChipRegValueA == ChipRegValueB)   //相同则退出
		{
			break;
		}
		if(try_num)
			try_num--;

		if(!try_num)
		{
			break;
		}
		DelayMs(10);

	}

}
//==================================================================================
//MCU读APT指定寄存器地址的数据
//==================================================================================
unsigned char APT_ReadChipReg(unsigned char APTRdAdd)
{
	unsigned char APTRdData = 0;

	APT_IIC_Start();
	#ifdef __APT_GT316L_DEVICE__
	APT_IIC_Write8bit(GT316L_DEVICE_ADDR); //1011 0000 APT固定地址，写
	#elif defined(__APT8L16__)
	APT_IIC_Write8bit(APT8L16_DEVICE_ADDR); //1010 1100 APT固定地址，写
	#endif
	APT_IIC_WaitAck();
	APT_IIC_Write8bit(APTRdAdd);   //APT内部寄存器地址
	APT_IIC_WaitAck();

	APT_IIC_Start();
	#ifdef __APT_GT316L_DEVICE__
	APT_IIC_Write8bit(GT316L_DEVICE_ADDR | 1); //1011 0000 APT固定地址，读
	#elif defined(__APT8L16__)
	APT_IIC_Write8bit(APT8L16_DEVICE_ADDR | 1); //1010 1100 APT固定地址，读
	#endif
	APT_IIC_WaitAck();
	APTRdData = APT_IIC_Read8bit();   //读数值

	APT_IIC_SendACK(0);   //不应答
	APT_IIC_Stop();

	return APTRdData;
}

//==================================================================================
//MCU配置APT触摸参数
//==================================================================================
#ifdef __APT_GT316L_DEVICE__

void APT_ChipRegSet(void)
{
	//u8 i = 0;
	APT_WriteChipReg(0x06,0x92);
	APT_WriteChipReg(0x06,0x02);
	soft_delay_ms(200);
	APT_WriteChipReg(0x04,0xFC);//按键控制寄存器0-7
	APT_WriteChipReg(0x05,0x3F);//按键控制寄存器8-15
	APT_WriteChipReg(0x07,0x02);
	APT_WriteChipReg(0x08,0x07);	
	APT_WriteChipReg(0x0A,0x05);//采样次数寄存器
	APT_WriteChipReg(0x0B,0x00);

	/***********************************/
	//键值灵敏度控制寄存器
	//APT_WriteChipReg(0x10,0x0F);
	//APT_WriteChipReg(0x11,0x0F);
	APT_WriteChipReg(0x12,0x0A);
	APT_WriteChipReg(0x13,0x0A);
	APT_WriteChipReg(0x14,0x0A);
	APT_WriteChipReg(0x15,0x0A);
	APT_WriteChipReg(0x16,0x0A);
	APT_WriteChipReg(0x17,0x0A);
	APT_WriteChipReg(0x18,0x0A);
	APT_WriteChipReg(0x19,0x0A);
	APT_WriteChipReg(0x1A,0x0A);
	APT_WriteChipReg(0x1B,0x0A);
	APT_WriteChipReg(0x1C,0x0A);
	APT_WriteChipReg(0x1D,0x0A);
	//APT_WriteChipReg(0x1E,0x0F);
	//APT_WriteChipReg(0x1F,0x0F);
	#if 0
	for(i = 0; i< 0x1E;i++)
	{
		printf("apt_reg[%02X] = 0x%02X\r\n",i,APT_ReadChipReg(i));
	}
	#endif
	/***********************************/
}
#elif defined(__APT8L16__)

void APT_ChipRegSet(void)
{

	APT_WriteChipReg(0x3A, 0x5a);   //配置前，必须先进入STOP模式，才能设置功能寄存器

	APT_WriteChipReg(0x21, 0x04);   //适用APT8L16,多键全速工作模式//04

	APT_WriteChipReg(0x22,0x10);
//要求自恢复时间短，可设APT_BUR_ADD的值为0x00

	APT_WriteChipReg(0x23, 0x0F);   //K0通道触摸按键禁止寄存器 禁止k00  k01  k02 k03
	APT_WriteChipReg(0x24, 0x00);   //K1通道触摸按键禁止寄存器 全部打开
//	APT_WriteChipReg(0x25, 0x43);
//	APT_WriteChipReg(0x26, 0x7f);
//	APT_WriteChipReg(0x27, 0x02);
//	APT_WriteChipReg(0x28, 0x02);
//没用到的触摸按键，建议禁止，可缩短扫描周期，加快按键的识别速度.0允许，1禁止

/*当触摸按键上有介质覆盖式要注释掉以下两行代码，并单独调整每个按键的灵敏度*/
	//APT_WriteChipReg(0x2A,0xff);
	//APT_WriteChipReg(0x2D,0x08);

/*当没有在开发者模式下进行调节时候，不能注释下面两行代码*/
	APT_WriteChipReg(0x2A,0x10);
	APT_WriteChipReg(0x2D,0x00);
	/*********************************************************************/

//如果项目处于开发阶段，覆盖介质不确定或需要直接接触按键,
//可进入开发模式，让按键自适应，不用调以下灵敏度寄存器
	APT_WriteChipReg(0x20, 0x02);   //全局灵敏度，最大值不能超过0x0F

	//APT_WriteChipReg(0x00, 0xff);    //对应K00通道的按键阀值 //不用的配成0xff
	//APT_WriteChipReg(0x01, 0xff);    //不用的配成0xff
	//APT_WriteChipReg(0x02, 0xff);    //不用的配成0xff
	//APT_WriteChipReg(0x03, 0xff);/*0*/
	#if 1
	APT_WriteChipReg(0x04, 0x04);/***/
	APT_WriteChipReg(0x05, 0x06);/*7*/
	APT_WriteChipReg(0x06, 0x06);/*4*/
	APT_WriteChipReg(0x07, 0x09);/*1*/
	APT_WriteChipReg(0x08, 0x08);/*2*/
	APT_WriteChipReg(0x09, 0x06);/*3*/
	APT_WriteChipReg(0x0A, 0x06);/*5*/
	APT_WriteChipReg(0x0B, 0x06);/*6*/   
	APT_WriteChipReg(0x0C, 0x06);/*9*/
	APT_WriteChipReg(0x0D, 0x06);/*#*/
	APT_WriteChipReg(0x0E, 0x06);/*8*/
	APT_WriteChipReg(0x0F, 0x06);/*0*/ 
	#endif
	APT_WriteChipReg(0x3A, 0x00);   //配置好寄存器后，恢复到工作状态
}

#endif

//==================================================================================
//MCU初始化APT芯片,此函数放在MCU上电初始化位置
//==================================================================================

void APT_EXTI_init(void)
{
	
	/**************设置IO口******************/
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOBEN ,ENABLE);
	GPIO_InitStructure.GPIO_Pin = TOUCHKEY_INT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_Init(TOUCHKEY_INT_PORT, &GPIO_InitStructure);

	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFGEN,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource2);

	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority= 0x00 ;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
void APT_I2C_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOBEN,ENABLE); //开启外设时钟
	//RCC_APB1PeriphClockCmd(RCC_APB1ENR_I2C2EN,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; /* 开漏输出 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	
	
	GPIO_InitStructure.GPIO_Pin = TOUCHKEY_SCL_PIN;
	GPIO_Init(TOUCHKEY_SCL_PORT, &GPIO_InitStructure);  //SCL PC3

	GPIO_InitStructure.GPIO_Pin = TOUCHKEY_SDA_PIN ;
	GPIO_Init(TOUCHKEY_SDA_PORT, &GPIO_InitStructure);  //SDA PC2
	
	APT_IIC_Stop();
	
}
void APT_Init(void)
{

	APT_I2C_init();

	APT_EXTI_init();

	soft_delay_ms(200);
	
	APT_ChipRegSet();
	
}
void APT_Close(void)
{
	APT_WriteChipReg(0x3A, 0x5a);

}

void APT_low_power(void)
{

	#ifdef __APT_GT316L_DEVICE__
		APT_WriteChipReg(0x0A, 0x03);
		APT_WriteChipReg(0x06, 0x1C);
	#elif defined(__APT8L16__)
		APT_WriteChipReg(0x3A, 0x5a);   //配置前，必须先进入STOP模式，才能设置功能寄存器
		
		APT_WriteChipReg(0x21, 0x06);   //适用APT8L16,多键全速工作模式//04

		APT_WriteChipReg(0x3A, 0x00);   //配置好寄存器后，恢复到工作状态
	#endif
}

void APT_Touchkey_EXTI_reStart(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	//EXTI_ClearITPendingBit(EXTI_Line2);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	return;
}


//==================================================================================
//MCU扫描触摸按键
//==================================================================================
uint8_t press_key_count =0;

uint8_t _get_APT_Touch_Key_Quantity(void)
{
	return APT_TOUCH_KEY_SUPPORT_QUAN;
}

uint8_t _get_key_quantity_in_once_press(void)
{
	return press_key_count;
}

uint16_t _clean_APT_TouchKey_reg(void)
{
	return APTTouchKeyreg_value=0;	
}

uint16_t _get_APT_TouchKey_reg(void)
{
	return APTTouchKeyreg_value;
}

uint8_t _get_APT_TouchKey_status(void)
{
	return APT_TouchKey_pressed;
}

void _clear_APT_Touchkey_status(void)
{
	APT_TouchKey_pressed =0;
	return;
}

/**************************************
* function: 	_set_APT_Touchkey_status
* creator:		Howei Zhang
* date:		2017-06-15
* time:		15:11
* parameter:	_status ->	0: none input
*							1: key short press input
*							2: key long press input
*							3: NFC detect,clear key input
**************************************/
void _set_APT_Touchkey_status(uint8_t _status)
{
	APT_TouchKey_pressed =_status;
	return;
}

void key_value_remap(uint16_t _key_value)
{
	uint8_t remap_loop;

	press_key_count =0;
	
	for (remap_loop=0; remap_loop <APT_TOUCH_KEY_SUPPORT_QUAN; remap_loop++)
	{
		if (_key_value&(0x01<<remap_loop))
		{
			key_map_status[remap_loop][1] =1;
			#ifdef __DEBUG2__
			printf("key_map key%d pressed! \r\n",key_map_status[remap_loop][0]);
			#endif
			press_key_count++;
		}
		else
		{
			key_map_status[remap_loop][1] =0;
		}
	}	
}


void long_press_key_value_remap(uint16_t _long_press_key_value)
{
	uint8_t remap_loop;

	press_key_count =0;
	for (remap_loop=0; remap_loop <APT_TOUCH_KEY_SUPPORT_QUAN; remap_loop++)
	{
		if ((key_map_status[remap_loop][1] == 1)&&(_long_press_key_value&(0x01<<remap_loop)))
		{
			key_map_status[remap_loop][1] =2;
			press_key_count++;
		}
		else
		{
			key_map_status[remap_loop][1] =0;
		}
	}	
}

void key_value_reset(void)
{
	uint8_t remap_loop;

	press_key_count =0;
	
	for (remap_loop=0; remap_loop <APT_TOUCH_KEY_SUPPORT_QUAN; remap_loop++)
	{
		key_map_status[remap_loop][1] =0;
		
	}	
}
void APT_ScanTouchKey(void)
{
	unsigned char APTTouchKeyValue0 = 0, APTTouchKeyValue1 = 0;
	APTTouchKeyValue0 = APT_ReadChipReg(0x02);
	APTTouchKeyValue1 = APT_ReadChipReg(0x03);

	APTTouchKeyreg_value = (APTTouchKeyValue1<<8|APTTouchKeyValue0);

	if ((_get_Audio_Status() == AUDIO_PLAY)&&(APTTouchKeyreg_value!=0))
	{
		AudioStop();
		Audio_Stop_flag = 1;
	}

	_set_APT_Touchkey_status(1);
	
} 

u16 APT_Get_Reg(void)
{
	unsigned char APTTouchKeyValue0 = 0, APTTouchKeyValue1 = 0;
	u16 APT_Reg = 0;
	APTTouchKeyValue0 = APT_ReadChipReg(0x02);
	APTTouchKeyValue1 = APT_ReadChipReg(0x03);

	APT_Reg = (APTTouchKeyValue1<<8|APTTouchKeyValue0);
	
	return APT_Reg;
}

#endif
