#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "mains.h"
#include "stm32f0xx.h"


#ifdef __VOICE_SUPPORT__
#define  checkbusy()       ABUSY

/*******************************************/
//extern u8 Play_num;  //����˳��
extern uint8_t Dikey_flag;  //��Ҫ���Ű���������־
extern uint8_t Audio_Stop_flag;

#ifdef __S500__
typedef enum
	{
		AUDIO_PROMPT_NONE,
		AUDIO_PROMPT_SETTING_START, 				//ע�����Ա��������ָ�ơ������ˢ��
		AUDIO_PROMPT_GET_SETTING_INFO,				//���������Ա��Ϣ
		AUDIO_PROMPT_VALIDATION_SUCCESS,			//��֤ͨ��
		AUDIO_PROMPT_KEYIN_FGPRT_AGAIN, 			//��������ͬһָ��
		AUDIO_PROMPT_KEYIN_FGPRT_AGAIN_2,			//��������ͬһָ��
		AUDIO_PROMPT_FGPRT_RECORD_SUCCESS,			//ָ�ƵǼǳɹ�
		AUDIO_PROMPT_RECORD_EXIST,					//�û��Ѵ���
		AUDIO_PROMPT_RECORD_FAIL,					//����ʧ��
		AUDIO_PROMPT_KEYIN_PSWD_AGAIN,				//��������ͬһ����
		AUDIO_PROMPT_PSWD_RECORD_SUCCESS,			//����Ǽǳɹ�
		AUDIO_PROMPT_RECORD_EXIST_2,				//�û��Ѵ���
		AUDIO_PROMPT_KEYIN_PSWD_AGAIN_2,			//��������ͬһ����
		AUDIO_PROMPT_PSWD_RECORD_FAIL,				//���벻һ�£��Ǽ�ʧ��
		AUDIO_PROMPT_RECORD_PSWD_MORE_6_TO_12,		//������6��12λ����
		AUDIO_PROMPT_NFC_RECORD_SUCCESS,			//���Ǽǳɹ�
		AUDIO_PROMPT_NFC_DELETED,					//���û�ɾ��
		AUDIO_PROMPT_SYS_EXIT,						//ϵͳ�˳�
		AUDIO_PROMPT_OPEN_DOOR, 					//�ѿ���
		AUDIO_PROMPT_CLOSE_DOOR,					//�ѹ���
		AUDIO_PROMPT_BATTERY_LOW,					//��ص����ͣ���������
		AUDIO_PROMPT_RECOVER_SETTING,				//�ָ���������
		AUDIO_PROMPT_RECOVER_SETTING_SUCCESS,		//�������ûָ��ɹ�
	    AUDIO_PROMPT_WELCOME,                       //��������
		AUDIO_PROMPT_BI,						    //��  ��ע�⣬��Ϊ��е����
		AUDIO_PROMPT_WARING,						//������
		AUDIO_PROMPT_WARING2,						//������2
		AUDIO_PROMPT_WELCOME3,						//��������
		AUDIO_PROMPT_INIT_SETTING,					//������״�����
	    AUDIO_PROMPT_SET_SUCCESS,					//���óɹ�
	    AUDIO_PROMPT_PSWD_LESS_THAN_6,				//������������6λ����������������
	    AUDIO_PROMPT_SET_PSWD,						//����������
		AUDIO_PROMPT_TRY_AGAIN,						//������һ��
		AUDIO_PROMPT_WELCOME_ENTRY_MENU,			//��ӭ������������ϵͳѡ��
		AUDIO_PROMPT_RECORD_SAVED,					//��¼��
		AUDIO_PROMPT_INPUT_FGPRT,					//�뽫��ָ����ָ������
		AUDIO_PROMPT_INPUT_NFC, 					//���������󽫴ſ�����ˢ����λ
		AUDIO_PROMPT_COME_IN,						//���
		AUDIO_PROMPT_VALIDATION_FAIL,				//��֤ʧ��
		AUDIO_PROMPT_VALIDATION_TIMEOUT,			//��֤��ʱ
		AUDIO_PROMPT_PSWD,							//����
		AUDIO_PROMPT_NFC,							//�ſ�
		AUDIO_PROMPT_FGPRT, 						//ָ��
	AUDIO_PROMPT_STORAGE_FULL,					//�ռ���
	AUDIO_PROMPT_DELETED,						//��ɾ��
	AUDIO_PROMPT_DATA_CLEARED,					//���������
	AUDIO_PROMPT_BELL,							//����
	AUDIO_PROMPT_VALIDATION_ADMIN,				//����֤����Ա��Ϣ
	AUDIO_PROMPT_SET_ADMIN,						//�����ù���Ա��֤��Ϣ
	AUDIO_PROMPT_PRESS, 							//�밴

    }AUDIO_PROMPT_VOICE_TYPE;

#endif

#endif

typedef enum
{
	AUDIO_STOP,
	AUDIO_PLAY,
	AUDIO_PAUSE,
	AUDIO_NEXT,
}AUDIO_STATUS_TYPE;


extern void AudioInitialize(void);
//extern void AudioPlay(u8 playnum);
extern void AudioPlay(AUDIO_PROMPT_VOICE_TYPE _voice_type);
extern void AudioPlay_All(AUDIO_PROMPT_VOICE_TYPE _voice_type_1, AUDIO_PROMPT_VOICE_TYPE _voice_type_2, AUDIO_PROMPT_VOICE_TYPE _voice_type_3);
extern AUDIO_STATUS_TYPE _get_Audio_Status(void);
extern void AudioStop(void);
extern void Audio_Pause(void);

#endif
