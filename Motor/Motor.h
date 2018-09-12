#ifndef _MOTOR_H__
#define _MOTOR_H__

#include "mains.h"

#define MOTOR_RUN_PERIOD	400		



#define MOTOR_OPEN				GPIO_SetBits(MOTOR_PWR_EN_PORT,MOTOR_PWR_EN_PIN);
#define MOTOR_CLOSE				GPIO_ResetBits(MOTOR_PWR_EN_PORT,MOTOR_PWR_EN_PIN);

#define MOTOR_IN1_HIGH		GPIO_SetBits(MOTO1_IN1_PORT, MOTO1_IN1_PIN)
#define MOTOR_IN1_LOW			GPIO_ResetBits(MOTO1_IN1_PORT, MOTO1_IN1_PIN)

#define MOTOR_IN2_HIGH		GPIO_SetBits(MOTO1_IN2_PORT, MOTO1_IN2_PIN)
#define MOTOR_IN2_LOW			GPIO_ResetBits(MOTO1_IN2_PORT, MOTO1_IN2_PIN)

#define OPENDOOR   1
#define CLOSEDOOR  0
#define STOPMOTOR  2

extern uint8_t motor_state;

void Motor_GPIO_Init(void);
void Motor_GPIO_Deinit(void);
void open_door(void);
void close_door(void);


#endif 
