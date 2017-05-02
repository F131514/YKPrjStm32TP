#ifndef  _EPM7064S_H
#define  _EPM7064S_H
#include "stm32f0xx.h"
//
// PIN ASSIGNMENT
//data group
#define EPM7064_DATA_PORT        GPIOA
#define EPM7064_DATA0            GPIO_Pin_0
#define EPM7064_DATA1            GPIO_Pin_1
#define EPM7064_DATA2        		 GPIO_Pin_2
#define EPM7064_DATA3       	   GPIO_Pin_3
#define EPM7064_DATA4       		 GPIO_Pin_4
#define EPM7064_DATA5       		 GPIO_Pin_5
#define EPM7064_DATA6       		 GPIO_Pin_6
#define EPM7064_DATA7        		 GPIO_Pin_7
//int group
#define EPM7064_INT_PORT         GPIOB
#define EPM7_INT_PIN             GPIO_Pin_12

//control group
#define EPM7064_CON_PORT         GPIOC
#define EPM7064_DATA_LOW_PIN     GPIO_Pin_2  //使能低位
#define EPM7064_DATA_HIG_PIN     GPIO_Pin_3  //使能高位
#define EPM7_DATA_HIG_ENABLE     GPIO_ResetBits(EPM7064_CON_PORT, EPM7064_DATA_HIG_PIN ) 
#define EPM7_DATA_LOW_ENABLE     GPIO_ResetBits(EPM7064_CON_PORT, EPM7064_DATA_LOW_PIN )
#define EPM7_DATA_HIG_DISABLE    GPIO_SetBits(  EPM7064_CON_PORT, EPM7064_DATA_HIG_PIN )
#define EPM7_DATA_LOW_DISABLE    GPIO_SetBits(  EPM7064_CON_PORT, EPM7064_DATA_LOW_PIN )

//////////////////////////////////MOTOR 
//马达相关管脚
#define MOTOR_PORT               GPIOB
#define MOTOR_DRIVE_PIN          GPIO_Pin_8
#define MOTOR_POSI_PIN           GPIO_Pin_0

//////////////////////////////////////////////////以下为宏定义

#define MOTOR_RUN                GPIO_SetBits(  GPIOB, GPIO_Pin_8 )          
#define MOTOR_STOP               GPIO_ResetBits(GPIOB, GPIO_Pin_8 )
           
#define  MOTOR_POSITION          GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)
//光耦信号，来自于加卸载状态
#define  LOAD_POSITION                                0 
#define  UNLOAD_POSITION                              1 

extern void   EPM7064_PIN_Init(void);

extern void   EXTI0_1_IRQHandler(void);
extern Uint32 Get_Raw(void);

extern void   EPM7064_Data_Pro(Uint32 raw_data);  

#endif