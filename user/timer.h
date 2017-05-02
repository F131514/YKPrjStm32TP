//**********************************************************************
//文件名:timer.h
//**********************************************************************
#include   "define.h"

#ifndef _TIMER_H
#define _TIMER_H

extern volatile Uint32 one_minute_timer;    //分钟定时器(分辨率:1ms)
extern volatile Uint8  flag_5ms_ok;
extern volatile Uint8  flag_10ms_ok;
extern volatile Uint8  flag_100ms_ok;
extern volatile Uint8  flag_300ms_ok;
extern volatile Uint8  flag_1ms_ok;
extern volatile Uint8  flag_500ms_ok;
extern volatile Uint8  flag_1s_ok;
extern volatile Uint32 tips_alm_timer;
extern volatile Uint32 warning_alm_timer;

void TIM3_Init(Uint32 us);
void TIM3_IRQHandler(void);
void Delay_Nms(Uint32 time);

#endif
