//**********************************************************************
//�ļ���:timer.c
//˵��:

//**********************************************************************

#include "global.h"
#include "timer.h"
 
volatile Uint32 one_minute_timer;    //���Ӷ�ʱ��(�ֱ���:1ms)
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
volatile Uint32 warning_alm_timer;   //��ʾ�౨����ʾ��ʱ��
volatile Uint32 tips_alm_timer;       //��ʾ�౨����ʾ��ʱ��

//**********************************************************************
//����  ��:Init_Timer0
//��������:��ʼ����ʱ��0,���ж�ʱ
//��ڲ���:us--��ʱʱ��(���1000000)
//���ڲ���:��
//**********************************************************************

void TIM_INT_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /*  TIM3 �ж�Ƕ�����*/
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
	
	/* ����Ԥ��Ƶֵ */
  PrescalerValue = (Uint16) (48000000  / 1000000) - 1;

  /* Time ��ʱ���������� */
  TIM_TimeBaseInitStruct.TIM_Period        =  us;              //����200������ÿ������ʱ��Ϊ1US
  TIM_TimeBaseInitStruct.TIM_Prescaler     = PrescalerValue;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
  TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

  /* TIM �ж�ʹ�� */
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	/* TIM1 ʹ�� */
  TIM_Cmd(TIM3, ENABLE);
	
}

//**********************************************************************
//����  ��:IRQ_Timer0
//��������:��ʱ��0���жϴ���(200us)
//��ڲ���:��
//���ڲ���:��
//**********************************************************************
void TIM3_IRQHandler(void)
{
    static Uint32 count_200us;
    
	  if(SET == TIM_GetITStatus(TIM3, TIM_IT_Update))
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    
		count_200us++;
    if (count_200us<5) return;
    count_200us = 0;    //1ms��
    flag_1ms_ok = TRUE;
    
	  one_minute_timer++;        //��ʱ1����
    if (one_minute_timer > 60000)
    {
        one_minute_timer = 0;
    }
    
    if (timer_5ms++ >= 5)//5ms��
    {
        timer_5ms = 0;
        flag_5ms_ok = TRUE;    
    } 
     
    timer_10ms++;
    if (timer_10ms < 10) return;
    timer_10ms = 0;        //10ms��
    flag_10ms_ok = TRUE;
    motor_run_time++;
    ////////////////////////10ms ��������
    Key_Sound_Pro();
		Key_Scan();
    //Alarm_Sound_Pro();
   
    timer_100ms++;
    if (timer_100ms < 10) return;
    timer_100ms = 0;    //100ms��
    flag_100ms_ok = TRUE;
    
    timer_300ms++;
    if (timer_300ms == 3)
    {
     timer_300ms = 0;
     flag_300ms_ok = TRUE;  //300ms��
   
    }
    
    timer_500ms++;
    if (timer_500ms<5) return;
    timer_500ms = 0;    //500ms��
    flag_500ms_ok = TRUE;
    
    timer_1s++;
    if (timer_1s<2) return;
    timer_1s = 0;    //1s��
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
				Update_Display();   //�����ڴ˴����� ��Ϊ����������������SLEEPΪTRUE ���ٸ����Դ�
				sleepmode = TRUE;
		    BL_Close();
      }
}

//**********************************************************************
//����  ��:Get_Cur_Time
//��������:��ȡ��ǰʱ��(���Ӷ�ʱ����ʱ��)
//��ڲ���:��
//���ڲ���:ʱ��(ms)
//**********************************************************************
Uint32 Get_Cur_Time(void)
{
    return (one_minute_timer);
}

//**********************************************************************
//����  ��:Delay_Nms
//��������:��ʱN ms
//��ڲ���:time--��ʱʱ��(��λ:ms)
//���ڲ���:��
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
