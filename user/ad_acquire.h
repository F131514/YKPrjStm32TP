//**********************************************************************
//ÎÄ¼þÃû:ad_acquire.h
//**********************************************************************
#ifndef _AD_ACQUIRE_H_
#define _AD_ACQUIRE_H_

#include "global.h"

#define ADC1_PORT           GPIOC
#define ADC1_TEMP_PIN       GPIO_Pin_4
#define ADC1_DR_Address     0x40012440

extern void  ADC1_Init(void);
#endif