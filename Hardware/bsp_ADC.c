#include "bsp_ADC.h"

//#define ADC1_DR_Address                0x40012440

u8 Battery_Percent;
volatile u8 Battery_state = 4;
u8 Battery_statebak = 4;
volatile u8 BatyLow_flag = 0;
uint32_t ADC_Value_Buff;
#if 0
void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOCEN, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC1EN, ENABLE);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin =  BAT_ADC_PIN ;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BAT_ADC_PORT, &GPIO_InitStructure);	
}

void ADC_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);

	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//ADC_OverrunModeCmd(ADC1, ENABLE);
	ADC_ChannelConfig(ADC1, ADC_Channel_12, ADC_SampleTime_71_5Cycles);
	
	//ADC_DMACmd(ADC1, ENABLE);
	//ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
	
	ADC_GetCalibrationFactor(ADC1);
	
	ADC_Cmd(ADC1,ENABLE);
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_ADRDY));

	ADC_StartOfConversion(ADC1);

}

void ADC2_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOCEN, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC1EN, ENABLE);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin =  VBUS_ADC_PIN ;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(VBUS_ADC_PORT, &GPIO_InitStructure);	
}


void ADC2_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);

	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_ChannelConfig(ADC1, ADC_Channel_13, ADC_SampleTime_71_5Cycles);
	
	ADC_GetCalibrationFactor(ADC1);
	
	ADC_Cmd(ADC1,ENABLE);
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_ADRDY));

	ADC_StartOfConversion(ADC1);

}

#endif

void ADC_DMA_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_Init_structure;
	ADC_InitTypeDef ADC_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOCEN, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC1EN, ENABLE);	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA时钟
	RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);

	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin =  BAT_ADC_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BAT_ADC_PORT, &GPIO_InitStructure);	

	DMA_DeInit(DMA1_Channel1); //复位DMA1_channel1
	DMA_StructInit(&DMA_Init_structure); //初始化DMA结构体

	DMA_Init_structure.DMA_BufferSize = 1; //DMA缓存数组大小设置
	DMA_Init_structure.DMA_DIR = DMA_DIR_PeripheralSRC; //DMA方向：外设作为数据源
	DMA_Init_structure.DMA_M2M = DISABLE; //内存到内存禁用
	DMA_Init_structure.DMA_MemoryBaseAddr =  (uint32_t)&ADC_Value_Buff;//缓存数据数组起始地址
	DMA_Init_structure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//数据大小设置为Halfword
	DMA_Init_structure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址递增
	DMA_Init_structure.DMA_Mode = DMA_Mode_Circular; //DMA循环模式，即完成后重新开始覆盖
	DMA_Init_structure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//取值的外设地址
	DMA_Init_structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//外设取值大小设置为Halfword
	DMA_Init_structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址递增禁用
	DMA_Init_structure.DMA_Priority = DMA_Priority_High; //DMA优先级设置为高
	DMA_Init(DMA1_Channel1, &DMA_Init_structure);
	
	DMA_Cmd(DMA1_Channel1, ENABLE); //使能DMA1	

	ADC_DeInit(ADC1);
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_OverrunModeCmd(ADC1, ENABLE);
	ADC_ChannelConfig(ADC1, ADC_Channel_12, ADC_SampleTime_239_5Cycles);

	ADC_DMACmd(ADC1, ENABLE);
	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);

	ADC_GetCalibrationFactor(ADC1);

	ADC_Cmd(ADC1,ENABLE);
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_ADRDY));

	ADC_StartOfConversion(ADC1);

}


void ADC1_Init(void)
{

	ADC_DMA_Init();

}



#if 0
void ADC_DMA_Init(void)
{
	DMA_InitTypeDef DMA_Init_structure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA时钟

	DMA_DeInit(DMA1_Channel1); //复位DMA1_channel1
	DMA_StructInit(&DMA_Init_structure); //初始化DMA结构体

	DMA_Init_structure.DMA_BufferSize = 2; //DMA缓存数组大小设置
	DMA_Init_structure.DMA_DIR = DMA_DIR_PeripheralSRC; //DMA方向：外设作为数据源
	DMA_Init_structure.DMA_M2M = DISABLE; //内存到内存禁用
	DMA_Init_structure.DMA_MemoryBaseAddr =  (uint32_t)&ADC_Value_Buff;//缓存数据数组起始地址
	DMA_Init_structure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//数据大小设置为Halfword
	DMA_Init_structure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址递增
	DMA_Init_structure.DMA_Mode = DMA_Mode_Circular; //DMA循环模式，即完成后重新开始覆盖
	DMA_Init_structure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//取值的外设地址
	DMA_Init_structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//外设取值大小设置为Halfword
	DMA_Init_structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址递增禁用
	DMA_Init_structure.DMA_Priority = DMA_Priority_High; //DMA优先级设置为高
	DMA_Init(DMA1_Channel1, &DMA_Init_structure);
	
	DMA_Cmd(DMA1_Channel1, ENABLE); //使能DMA1	
}
#endif
uint32_t ADC_Check()
{
	uint16_t ResultVolt = 0;
	uint8_t i,j;
	uint16_t Volttmep[10] = {0};
	uint16_t temp,temp_val;
	soft_delay_ms(30);
	for(i=0 ;i<10 ;i++)
	{
		Volttmep[i] = ADC_Value_Buff;
	}


	for(i = 0; i < (10 - 1); i++)
	{
		for(j = (i + 1); j < 10; j++)
		{
			if(Volttmep[i] > Volttmep[j])
			{
				temp = Volttmep[i];
				Volttmep[i] = Volttmep[j];
				Volttmep[j] = temp;
			}
		}
	}
	/**舍最大最小的两个数，然后求平均值**/

	temp_val = 0;
	for(i = 2; i < 8; i++)
	{
		temp_val += Volttmep[i];
	}
	ResultVolt = temp_val/6;
	
	return ResultVolt;
}


#if 0
uint32_t ADC_Check(void)
{
	uint32_t ResultVolt=0;
	uint8_t i,j;
	uint16_t Volttemp[10] = {0};
	uint16_t temp,temp_val;
	for(i=0 ;i<10 ;i++)
	{
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET)
		{
		}
			Volttemp[i] = (uint32_t)ADC_GetConversionValue(ADC1);
			printf("Volttemp[%d] = %d\r\n",i,Volttemp[i]);
	}
	ADC_StopOfConversion(ADC1);
	ADC_Cmd(ADC1,DISABLE);

	
	for(i = 0; i < (10 - 1); i++)
	{
		for(j = (i + 1); j < 10; j++)
		{
			if(Volttemp[i] > Volttemp[j])
			{
				temp = Volttemp[i];
				Volttemp[i] = Volttemp[j];
				Volttemp[j] = temp;
			}
		}
	}
	/**舍最大最小的两个数，然后求平均值**/
	temp_val = 0;
	for(i = 2; i < 8; i++)
	{
		temp_val += Volttemp[i];
	}
	ResultVolt = temp_val/6;
	return ResultVolt;
}
#endif




void battery_check(uint32_t ADC_PC2_ConvertedValue)
{
	/*---------------------------------------------
	  | ADC采样电压值 = Verf*ADC采样值/4096
	  | 采样电阻的电压值 = Vbat*(R采/R总)
	  | ADC采样电压值 = 采样电阻的电压值
	  | Vbat = (Verf*ADC采样值/4096)/Vbat*(R采/R总)
      |      =  9.35*ADC_PC2_ConvertedValue/4096
	  ---------------------------------------------*/
	
	//uint32_t ADC_PC2_ConvertedValue;
	float VolBattery; 
	BatyLow_flag = 0;
	//For the ADC register is 12 Bits; so the ADC_detect_volage/ADC_Vref = ADC_convert_Value/4096;
	//cause the ADC's Vref is connected to the VDD, so the ADC_Vref is 3.0V, and the ADC_detect_volage=V_BAT*R2/(R1+R2);
	//So the Voltage = (adc_value*ADC_Vref*(R1+R2))/(4096*R2) that we want detect: V_BAT;

	ADC_PC2_ConvertedValue = ADC_Check();//通道10
	VolBattery = (float) (10*(ADC_PC2_ConvertedValue*ADC_Vref))/(4096)+0.4;                  

	if (VolBattery > Battery_Level_6)
	{
		Battery_Percent = 100;
		BatyLow_flag = 5;
	}
	else if (VolBattery > Battery_Level_5)
	{
		Battery_Percent = 93+(u8)(((VolBattery - Battery_Level_5)/0.02)*0.7);
		BatyLow_flag = 4;
	}
	else if (VolBattery > Battery_Level_4)
	{
		Battery_Percent = 80+(u8)(((VolBattery - Battery_Level_4)/0.02)*1.3); 
		BatyLow_flag = 4;
	}
	else if (VolBattery > Battery_Level_3)
	{
		Battery_Percent = 56+(u8)(((VolBattery - Battery_Level_3)/0.02)*2.4);
		BatyLow_flag = 3;
	}
	else if (VolBattery > Battery_Level_2)
	{
		Battery_Percent = 18+(u8)(((VolBattery - Battery_Level_2)/0.02)*3.8);
		BatyLow_flag = 3;
	}
	else if (VolBattery > Battery_Level_1)
	{
		Battery_Percent = 5+(u8)(((VolBattery - Battery_Level_1)/0.01)*1.3);
		BatyLow_flag = 2;
	}
	else if (VolBattery > Battery_Level_0)
	{
		Battery_Percent = 0+(u8)(((VolBattery - Battery_Level_0)/0.01)*0.5);
		BatyLow_flag = 1;
	}
	else if (VolBattery <= Battery_Level_0)
	{
		Battery_state = 0; 
		BatyLow_flag = 1;  //the battery is no energy
	}
	
	#ifdef __DEBUG2__
	printf("### battery Volt = %f V ###\r\n",VolBattery);    
	printf("### BatyLow_flag=%d || Battery_Percent=%d ###\r\n",BatyLow_flag,Battery_Percent); 
	#endif
	
	return;
}

