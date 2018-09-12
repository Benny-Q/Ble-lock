#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "mains.h"
#include "stm32f0xx.h"


#ifdef __VOICE_SUPPORT__
#define  checkbusy()       ABUSY

/*******************************************/
//extern u8 Play_num;  //播音顺序
extern uint8_t Dikey_flag;  //需要播放按键滴声标志
extern uint8_t Audio_Stop_flag;

#ifdef __S500__
typedef enum
	{
		AUDIO_PROMPT_NONE,
		AUDIO_PROMPT_SETTING_START, 				//注册管理员，请输入指纹、密码或刷卡
		AUDIO_PROMPT_GET_SETTING_INFO,				//请输入管理员信息
		AUDIO_PROMPT_VALIDATION_SUCCESS,			//验证通过
		AUDIO_PROMPT_KEYIN_FGPRT_AGAIN, 			//请再输入同一指纹
		AUDIO_PROMPT_KEYIN_FGPRT_AGAIN_2,			//请再输入同一指纹
		AUDIO_PROMPT_FGPRT_RECORD_SUCCESS,			//指纹登记成功
		AUDIO_PROMPT_RECORD_EXIST,					//用户已存在
		AUDIO_PROMPT_RECORD_FAIL,					//操作失败
		AUDIO_PROMPT_KEYIN_PSWD_AGAIN,				//请再输入同一密码
		AUDIO_PROMPT_PSWD_RECORD_SUCCESS,			//密码登记成功
		AUDIO_PROMPT_RECORD_EXIST_2,				//用户已存在
		AUDIO_PROMPT_KEYIN_PSWD_AGAIN_2,			//请再输入同一密码
		AUDIO_PROMPT_PSWD_RECORD_FAIL,				//密码不一致，登记失败
		AUDIO_PROMPT_RECORD_PSWD_MORE_6_TO_12,		//请输入6到12位密码
		AUDIO_PROMPT_NFC_RECORD_SUCCESS,			//卡登记成功
		AUDIO_PROMPT_NFC_DELETED,					//卡用户删除
		AUDIO_PROMPT_SYS_EXIT,						//系统退出
		AUDIO_PROMPT_OPEN_DOOR, 					//已开锁
		AUDIO_PROMPT_CLOSE_DOOR,					//已关锁
		AUDIO_PROMPT_BATTERY_LOW,					//电池电量低，请更换电池
		AUDIO_PROMPT_RECOVER_SETTING,				//恢复出厂设置
		AUDIO_PROMPT_RECOVER_SETTING_SUCCESS,		//出厂设置恢复成功
	    AUDIO_PROMPT_WELCOME,                       //开机音乐
		AUDIO_PROMPT_BI,						    //哔  （注意，此为机械音）
		AUDIO_PROMPT_WARING,						//报警声
		AUDIO_PROMPT_WARING2,						//报警声2
		AUDIO_PROMPT_WELCOME3,						//开机音乐
		AUDIO_PROMPT_INIT_SETTING,					//请进行首次设置
	    AUDIO_PROMPT_SET_SUCCESS,					//设置成功
	    AUDIO_PROMPT_PSWD_LESS_THAN_6,				//密码输入少于6位，请重新输入密码
	    AUDIO_PROMPT_SET_PSWD,						//请设置密码
		AUDIO_PROMPT_TRY_AGAIN,						//请再试一次
		AUDIO_PROMPT_WELCOME_ENTRY_MENU,			//欢迎进入赛福智能系统选项
		AUDIO_PROMPT_RECORD_SAVED,					//已录入
		AUDIO_PROMPT_INPUT_FGPRT,					//请将手指放在指纹区域
		AUDIO_PROMPT_INPUT_NFC, 					//请在哔声后将磁卡靠近刷卡部位
		AUDIO_PROMPT_COME_IN,						//请进
		AUDIO_PROMPT_VALIDATION_FAIL,				//验证失败
		AUDIO_PROMPT_VALIDATION_TIMEOUT,			//验证超时
		AUDIO_PROMPT_PSWD,							//密码
		AUDIO_PROMPT_NFC,							//磁卡
		AUDIO_PROMPT_FGPRT, 						//指纹
	AUDIO_PROMPT_STORAGE_FULL,					//空间满
	AUDIO_PROMPT_DELETED,						//已删除
	AUDIO_PROMPT_DATA_CLEARED,					//数据已清除
	AUDIO_PROMPT_BELL,							//门铃
	AUDIO_PROMPT_VALIDATION_ADMIN,				//请验证管理员信息
	AUDIO_PROMPT_SET_ADMIN,						//请设置管理员验证信息
	AUDIO_PROMPT_PRESS, 							//请按

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
