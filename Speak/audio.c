#include "audio.h"

#ifdef __VOICE_SUPPORT__

//u8 Play_num=0;  //2£§®∞??3D®∞
uint8_t Dikey_flag=0;  //D®®®∞a2£§°§?°„°‰?®π¶Ã?®¶®¥°¿®∫??
AUDIO_STATUS_TYPE AudioStatus;
uint8_t Audio_Stop_flag;


void AudioInitialize(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd( RCC_AHBENR_GPIOAEN, ENABLE);
	RCC_AHBPeriphClockCmd( RCC_AHBENR_GPIOCEN, ENABLE);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = SPEAKER_RST_PIN;
	GPIO_Init(SPEAKER_RST_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = SPEAKER_DATA_PIN;
	GPIO_Init(SPEAKER_DATA_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = SPEAKER_BUSY_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(SPEAKER_BUSY_PORT, &GPIO_InitStructure);	
	
	AudioStatus = AUDIO_STOP;
	
	GPIO_ResetBits(SPEAKER_DATA_PORT,SPEAKER_DATA_PIN);                   //…œµÁ∏¥Œª∑¿÷π∏…»≈∑¢…˘–æ∆¨∑≈“Ù
	GPIO_SetBits(SPEAKER_RST_PORT,SPEAKER_RST_PIN);
	delay_us(10); //10*10us=100us
	GPIO_ResetBits(SPEAKER_RST_PORT,SPEAKER_RST_PIN);
	delay_us(10); //10*10us=100us
	
	
}

void AudioPlay(AUDIO_PROMPT_VOICE_TYPE _voice_type)
{
	uint8_t i;
	uint8_t Index;
	printf("AudioPlay=%d\r\n",_voice_type);
	if (Flag_Inf[Audio_mode] == 1 )
	{
		return;
	}
	if (AudioStatus != AUDIO_STOP)
		return;
	
	//return;
	TIM2_Stop();
	AudioStatus =AUDIO_PLAY;
	Index=(uint8_t)_voice_type;

	GPIO_ResetBits(SPEAKER_RST_PORT,SPEAKER_RST_PIN);
	delay_us(200); //00us
	GPIO_SetBits(SPEAKER_RST_PORT,SPEAKER_RST_PIN);  //∏¥Œª¬ˆ≥Â
	delay_us(100); //00us
	GPIO_ResetBits(SPEAKER_RST_PORT,SPEAKER_RST_PIN);
	delay_us(100); //100us
	
	for(i=0;i<Index;i++)
	{
		GPIO_SetBits(SPEAKER_DATA_PORT,SPEAKER_DATA_PIN);
		delay_us(100);
		GPIO_ResetBits(SPEAKER_DATA_PORT,SPEAKER_DATA_PIN);
		delay_us(100);
	}
	delay_us(20);

	while(GPIO_ReadInputDataBit(SPEAKER_BUSY_PORT, SPEAKER_BUSY_PIN));	//∂¡SDA–≈∫≈
	//debonce once
	soft_delay_ms(1);
	while(GPIO_ReadInputDataBit(SPEAKER_BUSY_PORT, SPEAKER_BUSY_PIN));	//∂¡SDA–≈∫≈
	//soft_delay_ms(1500);
	AudioStatus =AUDIO_STOP;
	TIM2_Start();
}

void AudioPlay_All(AUDIO_PROMPT_VOICE_TYPE _voice_type_1, AUDIO_PROMPT_VOICE_TYPE _voice_type_2, AUDIO_PROMPT_VOICE_TYPE _voice_type_3)
{
	Audio_Stop_flag = 0;

	AudioPlay(_voice_type_1);

	if (Audio_Stop_flag == 0 && _voice_type_2 != AUDIO_PROMPT_NONE)
	{
		AudioPlay(_voice_type_2);
	}

	if (Audio_Stop_flag == 0 && _voice_type_3 != AUDIO_PROMPT_NONE)
	{
		AudioPlay(_voice_type_3);
	}
}

void Audio_Pause(void)
{
	AudioStatus =AUDIO_PAUSE;

	GPIO_SetBits(SPEAKER_RST_PORT,SPEAKER_RST_PIN);  //∏¥Œª¬ˆ≥Â
	delay_us(100); //00us
	GPIO_ResetBits(SPEAKER_RST_PORT,SPEAKER_RST_PIN);
}

void AudioStop(void)
{
	AudioStatus =AUDIO_STOP;

	GPIO_SetBits(SPEAKER_RST_PORT,SPEAKER_RST_PIN);  //∏¥Œª¬ˆ≥Â
	delay_us(100); //00us
	GPIO_ResetBits(SPEAKER_RST_PORT,SPEAKER_RST_PIN);
}

AUDIO_STATUS_TYPE _get_Audio_Status(void)
{
	return AudioStatus;

}

#endif
