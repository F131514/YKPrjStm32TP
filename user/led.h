#ifndef __LED_H
#define	__LED_H

#include "stm32f0xx.h"
#define CPU_LED_PIN      GPIO_Pin_12
#define CPU_LED_PORT     GPIOA

#define BL_LED_PIN       GPIO_Pin_9
#define BL_LED_PORT      GPIOB

#define SPEAKER_PIN      GPIO_Pin_13
#define SPEAKER_PORT     GPIOC


extern void CPU_LED_Init(void);
extern void BL_LED_Init(void);
extern void SPEAKER_PIN_Init(void);


extern void LED_Open(void);
extern void LED_Close(void);
extern void BL_Open(void);
extern void BL_Close(void);
extern void SPEAKER_Open(void);
extern void SPEAKER_Close(void);


#endif /* __LED_H */

