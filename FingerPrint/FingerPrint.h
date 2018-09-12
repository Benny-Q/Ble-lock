#ifndef __FINGERPRINT_H__
#define __FINGERPRINT_H__


#include "mains.h"
#include "STM_GPIO_List.h"

#define ACK_SUCCESS        0x00 //ִ�гɹ�
#define ACK_FAIL           0x01 //ִ��ʧ��
#define ACK_FULL           0x04 //���ݿ���
#define ACK_NOUSER         0x05 //û������û�
#define ACK_USER_EXIST     0x07 //�û��Ѵ���
#define ACK_TIMEOUT        0x08 //ͼ��ɼ���ʱ
#define ACK_BREAK          0x18 //��ֹ��ǰָ��
#define ACK_IMAGEFAIL      0x84 //ͼ�񲻺ϸ�




#define MANGER_ROLL        1 //����Ա
#define COMMON_ROLL        2 //��ͨ�û�
#define CALLER_ROLL        3 //�ÿ�

#define   USED    1  //������δ����ʱ��1
#define   NOUSED  0

extern uint8_t ICcard_int_flag;//IC�����ѱ�ʶλ


typedef enum{
	CMD_TEST=0,            //��� Target �� Host ֮���Ƿ�ͨѶ������
	CMD_ENROLL,            //ָ��¼��
	CMD_IDENTIFY,          //1:N �ȶԣ�������ָ��������ע��� Template �ȶԡ�	
	CMD_CLEAR_TEMPLATE,    //ɾ��ָ���������ע��� Template��
	CMD_CLEAR_ALLTEMPLATE, //ɾ����ע������� Template��
	CMD_BREAK			//�������
}CMD_ENUM;

typedef enum{
	Result_Succes,
	Result_Failed,
	Result_Timeout,
	Result_Exist,
	Result_Full,
	Result_Again,
	Result_Pass,
	Result_Finger_Exist
}Result_Enum;


//����ָ��ͷ�ṹ����Ϣ
typedef struct tagFINGER
{
	uint8_t  usefalg;       //������δ����ʱ��1����������
	uint8_t  data[17];                               
}FINGER;

extern FINGER g_finger[6];    //����ָ�ƽ��ն������棬��������
extern FINGER Rev_finger;     //����ָ�ƽ���һ������
extern uint8_t figcntRxd;        //�����ֽڼ���
//extern u8 figcntRxdbak;     //�����ֽڼ�������

extern uint8_t FingPrint_data[17]; //���͸�ָ��ģ�������

uint8_t FingerPrint_api(CMD_ENUM cmd,uint16_t data,uint8_t roll);
void FPRT_GPIO(void);
void FPRT_EXTI_GPIO(void);
void Fingerprint_init(void);
void Fingerprint_Enable(void);
void Fingerprint_Disable(void);
int Fingerprint_Enroll(uint8_t permissions, uint16_t User_Number);
int Fingerprint_Compare(void);

void FingerPrint_Delete(uint16_t User_Number);
void FingerPrint_Support(void);
void FPRT_Touch_GPIO(void);
void FPRT_EXTI_GPIO(void);
extern void fingerPro(void);


//EventFlg            equ     37h;ע��40h�Ժ�û��λѰַ
#define  SYS_MOTO_P			0x01         //;BIT = 1,CHANGE
#define  SYS_MOTO_WAIT		0x02         //;BIT = 1,CHANGE FULL
#define  SYS_MOTO_N			0x04       // ;BIT = 1,LOW POWER
#define  SYS_POWER_OFF		0x08       // ;BIT = 1,LOW POWER
#define  SYS_FINISH_WAIT	0x10        // ;BIT = 1,LOW POWER
#define  SYS_TIME_FINISH       0x20

#define LOCK_TIME_150MS 5// 3
#define LOCK_TIME_200MS 6// 4
#define LOCK_TIME_300MS 7// 6
extern void bluetooth_init(void);
#endif 
