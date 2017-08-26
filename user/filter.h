#ifndef _FILTER_H
#define _FILTER_H

#include "global.h"
///////////////////////////////////////////////////
////////////////////////////////////////////////////
#define      AD_AVER_CNT                 16

#define      AD_AVER_CNT1                 16   //第一级平均数  8 个数取中间4个 
/////////////////////////////////////////////////////////////
#define      AD_AVER_CNT2                 16   //第二级平均数  然后取平均值

///设置零点自动跟踪范围 8个字内跟踪掉
#define      AUTO_0_SETUP                 10

extern Uint8 flag_auto_track_enable,auto_track_cnt;  //在这种情况下都不允许自动跟踪和载荷跟踪
extern Uint8 flag_load_track_enable,load_track_cnt; 


extern  Uint32 Get_Filter_Data(void);
extern  void   filter(void);
extern  void   filter_init(void);
extern  void   stable_manual_break(void);
extern  Uint32 stable_load_zer0(Uint32 weight2);
extern  void   auto_zer0(Uint32 weight1);

extern  float  filter_ad_NTC(Uint16 ntc_data);

#endif