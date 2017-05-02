#ifndef __KEY_H
#define	__KEY_H

#include "stm32f0xx.h"



#define KEY_PORT1        GPIOB
#define KEY_PORT2        GPIOC

#define KEY_PORT1_MASK   0xF000
#define KEY_PORT1_PIN1   GPIO_Pin_12
#define KEY_PORT1_PIN2   GPIO_Pin_13
#define KEY_PORT1_PIN3   GPIO_Pin_14
#define KEY_PORT1_PIN4   GPIO_Pin_15

#define KEY_PORT2_MASK   0x03C0
#define KEY_PORT2_PIN1   GPIO_Pin_6
#define KEY_PORT2_PIN2   GPIO_Pin_7
#define KEY_PORT2_PIN3   GPIO_Pin_8
#define KEY_PORT2_PIN4   GPIO_Pin_9

//define key name
#define KEY_NULL         0
#define KEY_ON_OFF       1
#define KEY_ZERO         2
#define KEY_PRINT        3
#define KEY_CONFIG       4
//¸´ºÏ¼üÅÌ
#define KEY_SAVE_UP      5
#define KEY_REC_DOWN     6
#define KEY_CAL_CONFIRM  7
#define KEY_TARE_RET     8
//
#define KEY_LONG         0x80

#define KEY_REC_LONG     9

#define KEY_TIME_NORMAL  10 //
#define KEY_TIME_LONG    20 //
#define KEY_TIME_WARNING 30 //

extern void  Key_Init(void);
extern Uint8 Key_Scan(void);
extern Uint8 Get_Key_Code(void);
extern void  Key_Pro(Uint8 key_code);
extern void  Key_F_Pro(Uint8 key_code);
extern void  Key_Sound_Pro(void);
extern void  Key_Null_Pro(void);

#endif /* __LED_H */