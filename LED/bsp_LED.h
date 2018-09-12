#ifndef _LED_H_
#define _LED_H_

#include "mains.h"

#define ON  0
#define OFF 1

void LED_Config(void);
void LED_Deinit(void);
void LED_drive(void);
void LED_Disable(void);

extern void LED_Control(uint8_t state,uint8_t NUM);

#define LED1_ON				GPIO_ResetBits(LED_1_PORT, LED_1_PIN)
#define LED2_ON				GPIO_ResetBits(LED_2_PORT, LED_2_PIN)
#define LED3_ON				GPIO_ResetBits(LED_3_PORT, LED_3_PIN)
#define LED4_ON				GPIO_ResetBits(LED_4_PORT, LED_4_PIN)
#define LED5_ON				GPIO_ResetBits(LED_5_PORT, LED_5_PIN)
#define LED6_ON				GPIO_ResetBits(LED_6_PORT, LED_6_PIN)
#define LED7_ON				GPIO_ResetBits(LED_7_PORT, LED_7_PIN)
#define LED8_ON				GPIO_ResetBits(LED_8_PORT, LED_8_PIN)
#define LED9_ON				GPIO_ResetBits(LED_9_PORT, LED_9_PIN)
#define LED10_ON			GPIO_ResetBits(LED_10_PORT, LED_10_PIN)
#define LED11_ON			GPIO_ResetBits(LED_11_PORT, LED_11_PIN)
#define LED12_ON			GPIO_ResetBits(LED_12_PORT, LED_12_PIN)

#define LED1_OFF				GPIO_SetBits(LED_1_PORT, LED_1_PIN)
#define LED2_OFF				GPIO_SetBits(LED_2_PORT, LED_2_PIN)
#define LED3_OFF				GPIO_SetBits(LED_3_PORT, LED_3_PIN)
#define LED4_OFF				GPIO_SetBits(LED_4_PORT, LED_4_PIN)
#define LED5_OFF				GPIO_SetBits(LED_5_PORT, LED_5_PIN)
#define LED6_OFF				GPIO_SetBits(LED_6_PORT, LED_6_PIN)
#define LED7_OFF				GPIO_SetBits(LED_7_PORT, LED_7_PIN)
#define LED8_OFF				GPIO_SetBits(LED_8_PORT, LED_8_PIN)
#define LED9_OFF				GPIO_SetBits(LED_9_PORT, LED_9_PIN)
#define LED10_OFF				GPIO_SetBits(LED_10_PORT, LED_10_PIN)
#define LED11_OFF				GPIO_SetBits(LED_11_PORT, LED_11_PIN)
#define LED12_OFF				GPIO_SetBits(LED_12_PORT, LED_12_PIN)





#endif

