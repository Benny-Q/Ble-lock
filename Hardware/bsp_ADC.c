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
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��DMAʱ��
	RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);

	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin =  BAT_ADC_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BAT_ADC_PORT, &GPIO_InitStructure);	

	DMA_DeInit(DMA1_Channel1); //��λDMA1_channel1
	DMA_StructInit(&DMA_Init_structure); //��ʼ��DMA�ṹ��

	DMA_Init_structure.DMA_BufferSize = 1; //DMA���������С����
	DMA_Init_structure.DMA_DIR = DMA_DIR_PeripheralSRC; //DMA����������Ϊ����Դ
	DMA_Init_structure.DMA_M2M = DISABLE; //�ڴ浽�ڴ����
	DMA_Init_structure.DMA_MemoryBaseAddr =  (uint32_t)&ADC_Value_Buff;//��������������ʼ��ַ
	DMA_Init_structure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//���ݴ�С����ΪHalfword
	DMA_Init_structure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ����
	DMA_Init_structure.DMA_Mode = DMA_Mode_Circular; //DMAѭ��ģʽ������ɺ����¿�ʼ����
	DMA_Init_structure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//ȡֵ�������ַ
	DMA_Init_structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//����ȡֵ��С����ΪHalfword
	DMA_Init_structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ��������
	DMA_Init_structure.DMA_Priority = DMA_Priority_High; //DMA���ȼ�����Ϊ��
	DMA_Init(DMA1_Channel1, &DMA_Init_structure);
	
	DMA_Cmd(DMA1_Channel1, ENABLE); //ʹ��DMA1	

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

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��DMAʱ��

	DMA_DeInit(DMA1_Channel1); //��λDMA1_channel1
	DMA_StructInit(&DMA_Init_structure); //��ʼ��DMA�ṹ��

	DMA_Init_structure.DMA_BufferSize = 2; //DMA���������С����
	DMA_Init_structure.DMA_DIR = DMA_DIR_PeripheralSRC; //DMA����������Ϊ����Դ
	DMA_Init_structure.DMA_M2M = DISABLE; //�ڴ浽�ڴ����
	DMA_Init_structure.DMA_MemoryBaseAddr =  (uint32_t)&ADC_Value_Buff;//��������������ʼ��ַ
	DMA_Init_structure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//���ݴ�С����ΪHalfword
	DMA_Init_structure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ����
	DMA_Init_structure.DMA_Mode = DMA_Mode_Circular; //DMAѭ��ģʽ������ɺ����¿�ʼ����
	DMA_Init_structure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//ȡֵ�������ַ
	DMA_Init_structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//����ȡֵ��С����ΪHalfword
	DMA_Init_structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ��������
	DMA_Init_structure.DMA_Priority = DMA_Priority_High; //DMA���ȼ�����Ϊ��
	DMA_Init(DMA1_Channel1, &DMA_Init_structure);
	
	DMA_Cmd(DMA1_Channel1, ENABLE); //ʹ��DMA1	
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
	/**�������С����������Ȼ����ƽ��ֵ**/

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
	/**�������С����������Ȼ����ƽ��ֵ**/
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
	  | ADC������ѹֵ = Verf*ADC����ֵ/4096
	  | ��������ĵ�ѹֵ = Vbat*(R��/R��)
	  | ADC������ѹֵ = ��������ĵ�ѹֵ
	  | Vbat = (Verf*ADC����ֵ/4096)/Vbat*(R��/R��)
      |      =  9.35*ADC_PC2_ConvertedValue/4096
	  ---------------------------------------------*/
	
	//uint32_t ADC_PC2_ConvertedValue;
	float VolBattery; 
	BatyLow_flag = 0;
	//For the ADC register is 12 Bits; so the ADC_detect_volage/ADC_Vref = ADC_convert_Value/4096;
	//cause the ADC's Vref is connected to the VDD, so the ADC_Vref is 3.0V, and the ADC_detect_volage=V_BAT*R2/(R1+R2);
	//So the Voltage = (adc_value*ADC_Vref*(R1+R2))/(4096*R2) that we want detect: V_BAT;

	ADC_PC2_ConvertedValue = ADC_Check();//ͨ��10
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

