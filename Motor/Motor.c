#include "Motor.h"

uint8_t motor_state = 0;

void Motor_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	
	//MOTOR1 IN1
	GPIO_InitStructure.GPIO_Pin = MOTO1_IN1_PIN;
	GPIO_Init(MOTO1_IN1_PORT,&GPIO_InitStructure);
	
	//MOTOR2 IN2
	GPIO_InitStructure.GPIO_Pin = MOTO1_IN2_PIN;
	GPIO_Init(MOTO1_IN2_PORT,&GPIO_InitStructure);

	//MOTOR POWER
	GPIO_InitStructure.GPIO_Pin = MOTOR_PWR_EN_PIN;
	GPIO_Init(MOTOR_PWR_EN_PORT,&GPIO_InitStructure);
	
	MOTOR_OPEN;
}

void Motor_GPIO_Deinit(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	
	//MOTOR1 IN1
	GPIO_InitStructure.GPIO_Pin = MOTO1_IN1_PIN;
	GPIO_Init(MOTO1_IN1_PORT,&GPIO_InitStructure);
	
	//MOTOR2 IN2
	GPIO_InitStructure.GPIO_Pin = MOTO1_IN2_PIN;
	GPIO_Init(MOTO1_IN2_PORT,&GPIO_InitStructure);

	//MOTOR POWER
	GPIO_InitStructure.GPIO_Pin = MOTOR_PWR_EN_PIN;
	GPIO_Init(MOTOR_PWR_EN_PORT,&GPIO_InitStructure);
	
	MOTOR_CLOSE;
}

void motor_ctrl(uint8_t doorctrl)
{
	
	
	switch(doorctrl)
	{
		case OPENDOOR:
			Motor_GPIO_Init();
			MOTOR_IN1_LOW;
			MOTOR_IN2_HIGH;
			break;
			
		case CLOSEDOOR:
			Motor_GPIO_Init();
			MOTOR_IN2_LOW;
			MOTOR_IN1_HIGH;
			break;
			
		case STOPMOTOR:
		default:
			MOTOR_IN2_LOW;
			MOTOR_IN1_LOW;
			Motor_GPIO_Deinit();
			break;
	}

}
extern u8 errorTouchCnt;
extern u8 errorFingerCnt;
extern u8 errorRFIDCnt;

void open_door(void)
{
	EXTI->IMR&=~EXTI_Line2;
	motor_ctrl(CLOSEDOOR);
//	AudioPlay(AUDIO_PROMPT_BI);
	soft_delay_ms(400);
	motor_ctrl(STOPMOTOR);
	motor_state=1;
	AudioPlay(AUDIO_PROMPT_OPEN_DOOR); //已开锁

	soft_delay_ms(5000);
	__Config_flag = 0;
	_set_system_sleep_status();
	close_door();
	EXTI->IMR |= EXTI_Line2;
	errorTouchCnt = 0;
	errorFingerCnt = 0;
	errorRFIDCnt = 0;
	AudioPlay(AUDIO_PROMPT_CLOSE_DOOR); //已关锁
}

void close_door(void)
{
	
	#ifdef __FOR_DEMO__
	u8 demo_delay =0;
	#endif

	#ifdef __DEBUG__
	printf("# @@@ # close_door ! \r\n");
	#endif
	motor_ctrl(OPENDOOR);
	
	#ifdef __FOR_DEMO__
	for (demo_delay =0;demo_delay<1;demo_delay++)
	{
		soft_delay_ms(MOTOR_RUN_PERIOD);
	}
	#else
	soft_delay_ms(400);
	#endif
	motor_ctrl(STOPMOTOR);
	motor_state=2;
}




