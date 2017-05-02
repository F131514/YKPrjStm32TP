//**********************************************************************
//文件名:timer.c
//说明:

//**********************************************************************

#include "global.h"
#include "timer.h"
 
volatile Uint32 one_minute_timer;    //分钟定时器(分辨率:1ms)
volatile Uint32 timer_5ms;
volatile Uint8  flag_5ms_ok;
volatile Uint32 timer_10ms;
volatile Uint8  flag_10ms_ok;
volatile Uint32 timer_100ms;
volatile Uint8  flag_100ms_ok;
volatile Uint32 timer_300ms;
volatile Uint8  flag_300ms_ok;
volatile Uint8  flag_1ms_ok;
volatile Uint8  flag_1ms_ok_for_delay;
volatile Uint32 timer_500ms;
volatile Uint8  flag_500ms_ok;
volatile Uint32 timer_1s;
volatile Uint8  flag_1s_ok;
volatile Uint32 warning_alm_timer;   //警示类报警显示定时器
volatile Uint32 tips_alm_timer;       //提示类报警显示定时器

//**********************************************************************
//函数  名:Init_Timer0
//函数功能:初始化定时器0,进行定时
//入口参数:us--定时时间(最大1000000)
//出口参数:无
//**********************************************************************

void TIM_INT_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /*  TIM3 中断嵌套设计*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void TIM3_Init(Uint32 us)
{ 
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	Uint16 PrescalerValue;
	
	TIM_INT_Config();
	
	/* 计算预分频值 */
  PrescalerValue = (Uint16) (48000000  / 1000000) - 1;

  /* Time 定时器基础设置 */
  TIM_TimeBaseInitStruct.TIM_Period        =  us;              //计满200个脉冲每个脉冲时间为1US
  TIM_TimeBaseInitStruct.TIM_Prescaler     = PrescalerValue;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
  TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

  /* TIM 中断使能 */
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	/* TIM1 使能 */
  TIM_Cmd(TIM3, ENABLE);
	
}

//**********************************************************************
//函数  名:IRQ_Timer0
//函数功能:定时器0的中断处理(200us)
//入口参数:无
//出口参数:无
//**********************************************************************
void TIM3_IRQHandler(void)
{
    static Uint32 count_200us;
    
	  if(SET == TIM_GetITStatus(TIM3, TIM_IT_Update))
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    
		count_200us++;
    if (count_200us<5) return;
    count_200us = 0;    //1ms到
    flag_1ms_ok = TRUE;
    
	  one_minute_timer++;        //定时1分钟
    if (one_minute_timer > 60000)
    {
        one_minute_timer = 0;
    }
    
    if (timer_5ms++ >= 5)//5ms到
    {
        timer_5ms = 0;
        flag_5ms_ok = TRUE;    
    } 
     
    timer_10ms++;
    if (timer_10ms < 10) return;
    timer_10ms = 0;        //10ms到
    flag_10ms_ok = TRUE;
    motor_run_time++;
    ////////////////////////10ms 处理声音
    Key_Sound_Pro();
		Key_Scan();
    //Alarm_Sound_Pro();
   
    timer_100ms++;
    if (timer_100ms < 10) return;
    timer_100ms = 0;    //100ms到
    flag_100ms_ok = TRUE;
    
    timer_300ms++;
    if (timer_300ms == 3)
    {
     timer_300ms = 0;
     flag_300ms_ok = TRUE;  //300ms到
   
    }
    
    timer_500ms++;
    if (timer_500ms<5) return;
    timer_500ms = 0;    //500ms到
    flag_500ms_ok = TRUE;
    
    timer_1s++;
    if (timer_1s<2) return;
    timer_1s = 0;    //1s到
    flag_1s_ok = TRUE; 
          
    if((STAT_ANIMAL == current_mode)&&(TRUE == animal_start)&&(animal_run_time>0))
    animal_run_time--;
    ///////////////////	
    //
		prehot_time--;		
		
		no_key_time++;
		if((SLEEP_TIME_CNT==no_key_time)&&(FALSE==sleepmode))
		  {
				Displaybuf_All_off();
				Update_Display();   //必须在此处更新 因为在主程序里面由于SLEEP为TRUE 不再更新显存
				sleepmode = TRUE;
		    BL_Close();
      }
}

//**********************************************************************
//函数  名:Get_Cur_Time
//函数功能:获取当前时间(分钟定时器的时间)
//入口参数:无
//出口参数:时间(ms)
//**********************************************************************
Uint32 Get_Cur_Time(void)
{
    return (one_minute_timer);
}

//**********************************************************************
//函数  名:Delay_Nms
//函数功能:延时N ms
//入口参数:time--延时时间(单位:ms)
//出口参数:无
//**********************************************************************
void Delay_Nms(Uint32 time)
{
    while (time)
    {
        if (flag_1ms_ok_for_delay)
        {
            flag_1ms_ok_for_delay = FALSE;
            time--;
        }
    }
}
