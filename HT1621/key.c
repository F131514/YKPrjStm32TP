/********************************************************************************
********************************************************************************/
#include "global.h"
#include "epm7064s.h"

#include "key.h"
#include "led.h"
#include "I2C.h"

//#define KEY_TYPE_MECHA    1
//////
Uint8 const	SAMPLE_COUNT[]={5,5,10,20,50,100,100};
Uint8 const ANIMAL_TIME[] ={10,10,15,20,30,30};
//0
//1 满量程       100,    120,     200,  220, 300, 500 
//2 分辨率     0.000, 0.0000, 0.00000
//3 LOGO           0,      1,
//4 CN/EN          0,      1,
//5 REPETION       0,      1,
//6 INT comp       0,      1,
//7 LINECAL EN     0,      1,
//8 LOCK    
//9 ASD
//10 REAL_G
const  Uint8  F_Mode_MAX[] = {12,3,4,2,2,2,2,2,3,2,2}; //每种配置下的最大选择项数目
static Uint8  key_code,key_flag;
static Uint8  real_g_cfg;       //用于输入真实重量
//-----------------------------------------------------------------------------   
//初始化   
void Key_Init(void)   
  {   
    Uint8 i;
    //管脚配置
	  GPIO_InitTypeDef GPIO_InitStruct;
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); //PB
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE); //PC
		
    GPIO_InitStruct.GPIO_Pin   = KEY_PORT1_PIN1|KEY_PORT1_PIN2|KEY_PORT1_PIN3|KEY_PORT1_PIN4;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
    GPIO_Init(KEY_PORT1, &GPIO_InitStruct);
	  
		GPIO_InitStruct.GPIO_Pin   = KEY_PORT2_PIN1|KEY_PORT2_PIN2|KEY_PORT2_PIN3|KEY_PORT2_PIN4;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
    GPIO_Init(KEY_PORT2, &GPIO_InitStruct);
		
  }

//----------------------------------------------------
//
//----------------------------------------------------

void Key_Sound_Pro(void)
{
 if(Speaker_Time == 0)
    SPEAKER_Close();
 else
   {
		SPEAKER_Open();
		Speaker_Time--; 
   }	 
}	
	
//----------------------------------------------------
//10ms * 300 = 3s
//----------------------------------------------------
Uint8 Get_Key_Code(void)
{
 return(key_code);
}
///////////////////////////////////////////////
Uint8 Key_Scan(void)
  {
	 static Uint8 	key_long_flag = FALSE;
	 static Uint8  	key_tmp;
   static Uint32  time_cnt;     //按键时间
	        Uint8   tmp;
		
	///////////////////////////////////////////////////
  tmp = (GPIO_ReadInputData(KEY_PORT2)>>6) & 0x0f;           //low
	tmp =	((GPIO_ReadInputData(KEY_PORT1)>>8) & 0xf0) + tmp;   //high
	tmp = ~tmp;	
	if(0x00 == tmp)
	  {
			//松开按键，发送代码
			time_cnt = 0;
			if((KEY_NULL==key_code)||(TRUE==key_flag))
			{
				switch(key_tmp)
					 {
				
						 #ifdef  KEY_TYPE_MECHA
						 case 0x01: key_tmp = KEY_SAVE_UP;      break;
						 case 0x02:	key_tmp = KEY_ON_OFF;       break;
						 case 0x04: key_tmp = KEY_CONFIG;       break;
						 case 0x08:	key_tmp = KEY_PRINT;        break;
						 case 0x10: key_tmp = KEY_ZERO;         break;
						 case 0x20:	key_tmp = KEY_TARE_RET;     break;
						 case 0x40: key_tmp = KEY_CAL_CONFIRM;  break;
						 case 0x80:	key_tmp = KEY_REC_DOWN;     break;
						 case 0x60: key_tmp = KEY_CAL_CONFIRM+KEY_TARE_RET;    break;
             #else			
						 /*/KEY_TYPE_TOUCH
             case 0x01: key_tmp = KEY_REC_DOWN;      break;
						 case 0x02:	key_tmp = KEY_PRINT;         break;
						 case 0x04: key_tmp = KEY_ON_OFF;        break;
						 case 0x08:	key_tmp = KEY_SAVE_UP;       break;
						 case 0x10: key_tmp = KEY_CONFIG;        break;
						 case 0x20:	key_tmp = KEY_TARE_RET;      break;
						 case 0x40: key_tmp = KEY_CAL_CONFIRM;   break;
						 case 0x80:	key_tmp = KEY_ZERO;          break;
						 case 0x60: key_tmp = KEY_CAL_CONFIRM+KEY_TARE_RET;    break;	
						 */
						 case 0x10: key_tmp = KEY_SAVE_UP;      break;
						 case 0x04:	key_tmp = KEY_ON_OFF;       break;
						 case 0x02: key_tmp = KEY_CONFIG;       break;
						 case 0x08:	key_tmp = KEY_PRINT;        break;
						 case 0x01: key_tmp = KEY_ZERO;         break;
						 case 0x20:	key_tmp = KEY_TARE_RET;     break;
						 case 0x40: key_tmp = KEY_CAL_CONFIRM;  break;
						 case 0x80:	key_tmp = KEY_REC_DOWN;     break;
						 case 0x60: key_tmp = KEY_CAL_CONFIRM+KEY_TARE_RET;    break;
						 #endif
						 default:   key_tmp = KEY_NULL;                 break;
					 } 
					 key_flag = FALSE;
           key_code = key_tmp;
	   }
		 else
		 {
			 key_code = KEY_NULL;		 
	     key_tmp  = KEY_NULL;
		 }
	}		
	else
	  {
	 	 time_cnt++;
     if(5 == time_cnt) //50ms 读取键值
   			key_tmp = tmp;
		 
		 if(time_cnt > 10)   //100ms 认为产生按键  
		    key_flag = TRUE;
		 
     if(time_cnt > 300)
		    {
				 key_flag = FALSE;
					
				 #ifdef KEY_TYPE_MECHA	
				 if(0x40==key_tmp)
				  {
					 key_code = KEY_LONG+KEY_CAL_CONFIRM;	
					 Speaker_Time = KEY_TIME_LONG;
					}
					
				 if(0x02==key_tmp)
				  {
					 key_code = KEY_LONG+KEY_ON_OFF;	
					 Speaker_Time = KEY_TIME_LONG;
					}
				 if(0x01==key_tmp)
				  { 
					 key_code = KEY_LONG+KEY_SAVE_UP;
					 Speaker_Time = KEY_TIME_LONG;
				  }	
				 if(0x20==key_tmp)
				  { 
					 key_code = KEY_LONG+KEY_TARE_RET;
					 Speaker_Time = KEY_TIME_LONG;
				  }		
				 #else
					
				 if(0x40==key_tmp)
				  {
					 key_code = KEY_LONG+KEY_CAL_CONFIRM;	
					 Speaker_Time = KEY_TIME_LONG;
					}
				 if(0x04==key_tmp)
				  {
					 key_code = KEY_LONG+KEY_ON_OFF;	
					 Speaker_Time = KEY_TIME_LONG;
					}
				 if(0x10==key_tmp)
				  { 
					 //key_code = KEY_LONG+KEY_SAVE_UP+KEY_REC_DOWN;
					 key_code = KEY_LONG+KEY_SAVE_UP;
					 Speaker_Time = KEY_TIME_LONG;
				  }	
			   if(0x20==key_tmp)
				  { 
					 key_code = KEY_LONG+KEY_TARE_RET;
					 Speaker_Time = KEY_TIME_LONG;
				  }	
				 #endif
        }					
     //else 
     //   key_long_flag = FALSE;
		 
		  return(KEY_NULL);		  
	  }
  }	
////////////////////////////////////////////////////////////
//以下是按键功能函数，系统设置为8个按键
////////////////////////////////////////////////////////////
void 	Key_Null_Pro(void)
  {
    key_flag = FALSE;
		key_code = KEY_NULL;
  }
////////////////////////////////////////////////////
void Key_ON_OFF_Pro(void)
   {
   	//
		static Uint8 tmp=0;
		if(TRUE == sleepmode)
		{
		 BL_Open();	
		 sleepmode = FALSE;	
		 tmp = 0;
		 return;
		}
		///////////////////////////////////////
    tmp++;
    if(tmp&0x01)		   
		{
			Displaybuf_All_off();   //模拟进入SLEEPMODE
			Update_Display();   
			sleepmode = TRUE;
		  BL_Close();
		}	
		else
		 BL_Open();	
   }
///////////////////////////////////////////////////////	
//
//
///////////////////////////////////////////////////////	 
void Key_Zero_Pro(void)
   {
		static float tmp1; 
		switch(current_mode)
		 {
			case STAT_CONFIG:
				                  current_menu++;				
			                 	  if(MENU_CONFIG_END == current_menu)
                          current_menu = MENU_CONFIG_MODE;
													
													if((MENU_CONFIG_DEL==current_menu)||(MENU_CONFIG_DEF==current_menu))
													   done_ok_flag = 0;    //当切换到这个菜单时，消除OK标志
													break;
			case STAT_DENSITY:
				                 if(((1==density_mode_index)||(2==density_mode_index))&&((0==density_step)||(6==density_step))) //一次称量结束
		                         density_step = 1;	 //启动 显示1 自动回称重状态2
                         else if((2==density_step)&&(TRUE == stable_flag))
	                         {
			                       density_step++;   //启动 显示2 自动回称重状态3
			                       tmp1 = net_weight;
			                     }			
		                     else if((4==density_step)&&(TRUE == stable_flag))
		                       {
														 density_step++;
		                         Cali_density_pro(tmp1,net_weight); //计算密度
		                       }
                         else {;}
                         break;													 
      case STAT_ANIMAL:
				                 if((0!=animal_time_index)&&(FALSE==animal_start))
												   {
														 animal_run_time = ANIMAL_TIME[animal_time_index];
														 animal_start = TRUE;
                           }																 
                         break;
		  case STAT_WEIGH:			      
                         do_zer0_flag = TRUE;
			default:           break;
     }
   }
////////////////////////////////////////////////////	 
void Key_Print_Pro(void)
   {
   	Uint8 i,j;
	 if((STAT_CONFIG==current_mode)&&(MENU_CONFIG_DEL==current_menu))
		    {
				 //删除所有记录
				 Del_rec_all();
				 done_ok_flag = 1;	
				} 
	 else if((STAT_CONFIG==current_mode)&&(MENU_CONFIG_DEF==current_menu))		
        {
				 //恢复出厂设置
				 Res_factory_pro();
				 done_ok_flag = 1;	
				} 
	 else if((current_mode>STAT_START)&&(current_mode<STAT_END))
	      { //在选择辅助物的时候起作用
				  if(STAT_DENSITY==current_mode)
					     {
								if(3==density_mode_index)
								  {
                   solid_sel_index++;
									 if(9==solid_sel_index)
										 solid_sel_index = 1;
								  }									
								else if(4==density_mode_index)
 							    {
                   liquid_sel_index++;
									 if(9==liquid_sel_index)
										 liquid_sel_index = 1;
								  }									
                else
								{}									
						    
							 }
					else if((0==print_busy)&&(TRUE==stable_flag)&&(2==Uart_Printer_Flag))          
				       {
                 print_busy = 1;
								 UART_PrintData();
				        } 
				  else
					    {}						
				}	
	
   else
	      {
	      }		 
   }
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////	 
void Key_Config_Pro(void) 
   {
   	//只有在称重模式下才可以进入
    if((current_mode>STAT_START)&&(current_mode<STAT_END)) //在正常称重模式下
		    {
				 current_mode_bak = current_mode;  //备份当前模式
				 current_mode     = STAT_CONFIG;
	       current_menu     = MENU_CONFIG_MODE;
				}
	 }
////////////////////////////////////////////////////	 
void Key_Save_Up_Pro(void)
   {
   	//
    if((current_mode>STAT_START)&&(current_mode<STAT_END)) //在正常称重模式下
		    {
				 if(TRUE == stable_flag)
				 { //SAVE 
				  Save_Rec();
				 }					 
				}
	  else if(STAT_DISP_REC == current_mode)  //显示记录模式下 切换上一条记录
		    {
				 //获取上一条记录
				 //if(disp_rec_index < rec_total_num)
 				  disp_rec_index++;
					disp_rec_index = disp_rec_index % MAX_REC_NUM;
        }					
		else		
	   	  {
				} 			
   }
////////////////////////////////////////////////////	 
void Key_Rec_Down_Pro(void)
   {
   	//
    if(STAT_WEIGH == current_mode) //在正常称重模式下
		    {//如果无记录 ，则 显示无记录，否则进入记录查看模式
				 if(0 == rec_total_num)
				   {
						 
				   }
				 else
				   {
						current_mode_bak = current_mode; 
						current_mode = STAT_DISP_REC; 
 						disp_rec_index = rec_index;
				   }					 
				}
		else if(STAT_DISP_REC == current_mode)
		    {
				 //获取下一条记录并显示
				 if(disp_rec_index > 0)
 						 disp_rec_index--;
         else
					 	 disp_rec_index = MAX_REC_NUM-1;		
				}	
    else if((STAT_PCS==current_mode)&&(TRUE==sample_pcs_enter))	//采集单件重 功能
		   {
			   sample_pcs_index++;
				 if(0x06 == sample_pcs_index)
				 sample_pcs_index = 0x01;	 
			 }		
    else if(STAT_DENSITY == current_mode)	//密度功能且还未按 置0
		   {
			      density_mode_index++;  //设置 密度模式1 2 3 4
            if(5 == density_mode_index)
							density_mode_index = 0x01;
			 }			
		else if((FALSE==animal_start)&&(STAT_ANIMAL==current_mode))	//动物称重模式
		   {
			      animal_time_index++;  //设置动物称重 时间 
            if(animal_time_index > 0x04)
							 animal_time_index = 0x01;
			 }			 
		else if(STAT_CONFIG == current_mode)
		   { //菜单下移动
	      switch(current_menu)
				{
				   case MENU_CONFIG_MODE: //称重模式
						                     do {
						                         current_mode_bak++;
																	   if(STAT_END == current_mode_bak)
																			 current_mode_bak = STAT_WEIGH;
																			 
																	   if(selemode&(1<<(current_mode_bak-0x07)))
																			 break;
					                       } while(STAT_WEIGH != current_mode_bak);
					                     
																break;												
					 case MENU_CONFIG_UNIT: //称量单位
						           	       	current_unit++;
                                if(UNIT_END == current_unit)
       													current_unit = UNIT_G;
                                break;																
				   case MENU_CONFIG_TEMP: //配置温度单位
						                    temp_unit++; 
					                      break;		
					 case MENU_CONFIG_DIV: //配置最后一位是否显示
						                    del_last_one++; 
					                      break;
					 case MENU_CONFIG_BAUD: //波特率
						                     //
					                       uart_format_type++;
					                       if(uart_format_type > UART_TYPE_END-1)
																    uart_format_type = 0x01; 
					                      break;		 
					 case MENU_CONFIG_DEL: //删除全部数据
						                     //
					                      break;		
					 case MENU_CONFIG_DEF: //恢复出厂数据
						                     //
				
					 case MENU_CONFIG_COMM: //打印格式选择
						                     //
					                       Uart_Printer_Flag++;
					                       if(Uart_Printer_Flag > 2)
																    Uart_Printer_Flag = 0x01; 
					                      break;		 
					 default:break;		           	       	 
 				}
			 }	
    else
	   	  {
				} 
   }	
////////////////////////////////////////////////////	 
 
void Key_LONG_Cal_Confirm_Pro(void)
   {
   	if((INNER_CAL_VERSION)&&(STAT_WEIGH == current_mode))
		    {
   			  current_mode_bak = current_mode; 
			  	current_mode     = STAT_CAL;
				
			   	point2_cal_start = TRUE;
       	  point2_cal_step  = USER_CAL_ZER0_FLASH;		
          stable_manual_break();				 
	      }
		}
	 
void Key_Cal_Confirm_Pro(void)
   {
   	Uint8  buf[4];
		
		if((STAT_CAL_LINE==current_mode)&&(FALSE==point10_cal_start)) //启动线性校准
	     { 
				point10_cal_start = TRUE;					
			 }
		else if(STAT_WEIGH == current_mode)
	   	 {//进入用户校准
				current_mode_bak = current_mode; 
				if(INNER_CAL_VERSION)
				{
				  motor_err_flag = motor_run_proc(UNLOAD_POSITION);
					if(1 == motor_err_flag)
					 End_Cal_Pro_Inn(); 
					else	
				  current_mode     = STAT_CAL_INNER;
				}
			  else
				current_mode     = STAT_CAL;
				
				point2_cal_start = TRUE;
       	point2_cal_step  = USER_CAL_ZER0_FLASH;		
        stable_manual_break();				 
			 }  
    else if(STAT_CONFIG == current_mode)
		   { //配置模式下切换配置项目
				 //需要保存几个参数 温度单位 最后一位是否显示
				 if(MENU_CONFIG_UNIT == current_menu)
				 {
					 buf[0] = current_unit;
					 buf[1] = CHECK_DATA;
					 buf[2] = buf[0] + buf[1];
					 buf[3] = CHECK_DATA;
					 Write_EEPROM(EEP_USR_FUN2_ADDR,buf, 4); 
				 } 
				 if(MENU_CONFIG_TEMP == current_menu)
				 {
					 buf[0] = temp_unit&0x01;
					 buf[1] = CHECK_DATA;
					 buf[2] = buf[0] + buf[1];
					 buf[3] = CHECK_DATA;
					 Write_EEPROM(EEP_USR_FUN3_ADDR,buf, 4); 
				 }
				 if(MENU_CONFIG_DIV == current_menu)
				 {
					 buf[0] = del_last_one&0x01;
					 buf[1] = CHECK_DATA;
					 buf[2] = buf[0] + buf[1];
					 buf[3] = CHECK_DATA;
					 Write_EEPROM(EEP_USR_FUN4_ADDR,buf, 4); 
				 }
				 if(MENU_CONFIG_BAUD == current_menu)
				 {
					 buf[0] = uart_format_type;
					 buf[1] = CHECK_DATA;
					 buf[2] = buf[0] + buf[1];
					 buf[3] = CHECK_DATA;
					 Write_EEPROM(EEP_USR_FUN5_ADDR,buf, 4); 
					 
					 //Init_UARTCONFIG_Para();
					 //USART_Configuration();
				 }
				 if(MENU_CONFIG_COMM == current_menu)
				 {
					 buf[0] = Uart_Printer_Flag;
					 buf[1] = CHECK_DATA;
					 buf[2] = buf[0] + buf[1];
					 buf[3] = CHECK_DATA;
					 Write_EEPROM(EEP_USR_FUN6_ADDR,buf, 4);
 
           //Init_UARTCONFIG_Para();
           //USART_Configuration();					 
				 }
				 
				 
				 
			 }
		else if(STAT_PCS==current_mode)
	     { //PCS模式下 采集单件重
				 if(FALSE==sample_pcs_enter) 
				  {
					 sample_pcs_enter = TRUE;
           sample_pcs_index = 0x01;
           //sample_pcs_ok  = FALSE;				
		      }
				 else
					{
					 weight_per_pcs = net_weight / SAMPLE_COUNT[sample_pcs_index];
					 sample_pcs_enter = FALSE;
					 sample_pcs_ok = TRUE;	
				  } 	
			 }
		else if(STAT_CHECK==current_mode)
	     { //按确认，下箭头亮待稳定后 
				 //按确认  上箭头亮待稳定后
				 //按确认  保存两者数据 并退出采集状态
				 if(0==sample_limit_step) 
				  {
					 sample_limit_step++;
          }
				 else if((1==sample_limit_step)&&(TRUE==stable_flag))
					{
					 sample_limit_step++;
					 limit1_weight = net_weight;
				  }
				 else if((2==sample_limit_step)&&(TRUE==stable_flag))
					{
					 sample_limit_step = 0;
					 if(net_weight>limit1_weight)
					  limit2_weight = net_weight;
					 else 
					 {
						limit2_weight = limit1_weight;
            limit1_weight = net_weight; 						 
					 } 
				  }
					else
					{}
			 }
		 else if(STAT_100==current_mode)
	     { //在百分比模式下 按确认 , 采集当前基准点
				 if(TRUE == stable_flag) 
				  {
					 weight_compare_100 = net_weight;
          }  
			 }
     else if(STAT_ANIMAL==current_mode)
	     { //在动物模式下 按确认 , 采集当前基准点
			 }
     else if(STAT_DENSITY==current_mode)
	     { //密度模式下
			 }
			 
     else
		   { 
			 }
   }
////////////////////////////////////////////////////	 
void Key_Tare_Ret_Pro(void)
   {
		if((STAT_CAL_LINE==current_mode)||(STAT_CAL_INNER==current_mode)||(STAT_CAL==current_mode))
		  {
			 current_mode = STAT_WEIGH;      //返回到基本称重模式
			 point2_cal_start  = FALSE;
			 point10_cal_start = FALSE;	
			}					 
   	else if((STAT_CONFIG==current_mode)||(STAT_DISP_REC==current_mode))
		  { 	
		   current_mode = current_mode_bak; //配置返回 //查看记录返回
			 if(STAT_CHECK == current_mode)
			   {
					sample_limit_step = 0;
          limit1_weight = 0;
          limit2_weight = 0;					 
			   } 
		 	 else if(STAT_DENSITY == current_mode)
			     {
						 density_mode_index = 0;
						 density_step = 0;
			     }
			 else if(STAT_ANIMAL == current_mode)
			     {
						 animal_time_index = 0;
						 animal_run_time   = 0;
						 animal_start      = FALSE;
		       } 
		   else			
		       {
      
		       }			
       }
    else  //只有在称重模式下才可以 去皮
		   {
		    do_tare_flag = TRUE;
	  	 } 				
	}		
////////////////////////////////////////////////////	 
	 
void Key_Group1_Pro(void)
{
 if(TRUE == power_on_flag)
  {
   current_mode = STAT_CAL_LINE;  //开机按组合键 进入线性校准模式
 	 point10_cal_step = 0x0;
	 point10_cal_start = FALSE;	
	 stable_manual_break();
  }
}	
////////////////////////////////////////////////////	 
//
////////////////////////////////////////////////////
void Key_Pro(Uint8 key_code)
   {
   	//校准按键  自动校准  按此键 返回称重模式
		if(KEY_NULL == key_code) 
			return;
		///////////////////////////////
		no_key_time = 0;   //有按键按下
		
		if(TRUE == sleepmode)
		{
		 if(KEY_ON_OFF == key_code)
			  Speaker_Time = KEY_TIME_NORMAL; 
		 else
			  key_code = KEY_NULL;
		}
		else
		{
		 if(KEY_NULL != key_code)
			 Speaker_Time = KEY_TIME_NORMAL;
		}
		
    switch(key_code)
		{
			case KEY_ON_OFF:       Key_ON_OFF_Pro();      break;
			case KEY_ZERO:         Key_Zero_Pro();        break;
			case KEY_PRINT:        Key_Print_Pro();       break;
			case KEY_CONFIG:       Key_Config_Pro();      break;
			case KEY_SAVE_UP:      Key_Save_Up_Pro();     break;
			case KEY_REC_DOWN:     Key_Rec_Down_Pro();    break;
			case KEY_CAL_CONFIRM:  Key_Cal_Confirm_Pro(); break;
			case KEY_TARE_RET:     Key_Tare_Ret_Pro();    break;
			case KEY_LONG+KEY_CAL_CONFIRM: 
                      			 Key_LONG_Cal_Confirm_Pro(); 
			                                              break;
			//case KEY_LONG+KEY_SAVE_UP+KEY_REC_DOWN: 
			case KEY_LONG+KEY_SAVE_UP:	
				                     Key_Group1_Pro();      break;
			default:                                      break;
		}
		  key_code = KEY_NULL;
		  key_flag = FALSE;
   }
////////////////////////////////////////////////////////////////////////	 
//工厂模式下的按键处理函数	 
///////////////////////////////////////////////////////////////////////	 
static Uint16  F_cfg_tmp;
Uint16	Get_F_tmp(void)  {return(F_cfg_tmp);}	
//////////////////////////////////////////////////////////////////////
void Key_F_ON_OFF_Pro(void)
   {
   	//
	  if(STAT_CONFIG == current_mode) {
			if(MENU_F_CONFIG_TESTKEY == current_menu)
				F_cfg_tmp |= 0x20;
   }
 }
	 
void	Key_F_Config_Pro(void)
{
	
	
	switch(current_mode)
	{
		case STAT_WEIGH:  
			                current_mode = STAT_INNER;  
		                  break;
		case STAT_INNER:  current_mode = STAT_CONFIG; 
	                	  current_menu = MENU_F_CONFIG_FULL;
		                  F_cfg_tmp    = (model_id>>8)&0xffff;
	                   	break;
		case STAT_CONFIG: 
			                current_mode = STAT_CONFIG2; 
		                  F_cfg_tmp = 1;
		                  current_menu = 1;
		                  break;
	  case STAT_CONFIG2: 
			                current_mode = STAT_WEIGH; 
		                  break;
		default:          current_mode = STAT_INNER;  
		                  break;
	} 
}

void  Key_F_Tare_Ret_Pro(void)
{
  if(STAT_WEIGH == current_mode)
	 {  //在重量模式下增加置0	
	  do_tare_flag = TRUE;
	 } else if(STAT_CONFIG == current_mode) {
	     if(MENU_F_CONFIG_TESTKEY == current_menu)
				 F_cfg_tmp |= 0x01;  //keyvalue 
	 }
}
//////////////////////////////////////////////////////////////////////////
void	Key_F_Zero_Pro(void)
{
	if(STAT_WEIGH == current_mode)
	{  //在重量模式下增加置0	
	 do_zer0_flag = TRUE;
	}
	else if(STAT_INNER == current_mode)
	{
	}
  else if(STAT_CONFIG2 == current_mode) {
	    current_menu++;
		  if(current_menu > 5)
				  current_menu = 1;
			
	} else {		
	current_menu++;
	if(MENU_F_CONFIG_END == current_menu)
		 current_menu = MENU_F_CONFIG_FULL;
	switch(current_menu)
	 {
		case MENU_F_CONFIG_FULL:    F_cfg_tmp = (model_id>>8)&0xffff; break;
		case MENU_F_CONFIG_DIV:     F_cfg_tmp =  model_id&0xff;       break;
		case MENU_F_CONFIG_LOGO:    F_cfg_tmp =  flag_LOGO;           break;
		case MENU_F_CONFIG_LANG:    F_cfg_tmp =  flag_language;       break;
		case MENU_F_CONFIG_REP:     F_cfg_tmp =  repetion_func;       break;
		case MENU_F_CONFIG_INT:     F_cfg_tmp =  int_func;            break;
		case MENU_F_CONFIG_LINE:    F_cfg_tmp =  line_cal_en;         break; 
		case MENU_F_CONFIG_LOCK:    F_cfg_tmp =  lock_func_en;        break; 
	  case MENU_F_CONFIG_ASD:     F_cfg_tmp =  asd_para;            break; 
	  case MENU_F_CONFIG_REALKG:  real_g_cfg=  1;                   break; 		
	  case MENU_F_CONFIG_DISDIV:  F_cfg_tmp =  disdiv_para;         break; 	 
		case MENU_F_CONFIG_TESTKEY: F_cfg_tmp =  0;                   break;  
		//case MENU_F_CONFIG_MOTOR: F_cfg_tmp =  0x00;                break; //0 is LOAD_POSITION
		default:break;
   }
  }
}

void  Key_F_Print_Pro(void)
{
	if(STAT_CONFIG2 == current_mode) {
		
	} else if(STAT_CONFIG == current_mode) {
		 if(MENU_F_CONFIG_REALKG==current_menu) {
         real_g_cfg++;
         if(8==real_g_cfg)
 		         real_g_cfg = 1;
		 } else if(MENU_F_CONFIG_TESTKEY==current_menu) {
          F_cfg_tmp |= 0x02; 
		 }	
	}
}
//////////////////////////////////////////////////////////////////////////

void	Key_F_Up_Pro(void)
{
	if(STAT_INNER==current_mode)
	{
    motor_pos_flag = 0; 
	  motor_run_proc(UNLOAD_POSITION);
		Factory_Inncal_step = 1;
	}	
		
	if(STAT_CONFIG==current_mode)
	   {
			 if(MENU_F_CONFIG_FULL==current_menu)
	       {
   	      F_cfg_tmp-=10;
		      if(F_cfg_tmp < 100)
			    F_cfg_tmp = 500;
	       }
				else if(MENU_F_CONFIG_REALKG==current_menu)
				{
					switch(real_g_cfg)
					 {
						case 1:  inner_real_g.rec_float += 0.0001;    break;
					  case 2:  inner_real_g.rec_float += 0.001;     break;
					  case 3:  inner_real_g.rec_float += 0.01;      break;
					  case 4:  inner_real_g.rec_float += 0.1;       break;
					  case 5:  inner_real_g.rec_float += 1.00;      break;
					  case 6:  inner_real_g.rec_float += 10.00;     break;
					  case 7:  inner_real_g.rec_float += 100.000;   break;
					  default:break;	
					 }		
				} else if(MENU_F_CONFIG_TESTKEY==current_menu) {
				   F_cfg_tmp |= 0x04;
				}					
			}
 }
//////////////////////////////////////////////////////////////////////////
void	Key_F_Down_Pro(void)
{//-----
	if(STAT_WEIGH==current_mode)
	   return;
	
	else if(STAT_INNER==current_mode)
	 {  
		motor_pos_flag = 1; 
	  motor_run_proc(LOAD_POSITION);
		Factory_Inncal_step = 2;
	 } else if(STAT_CONFIG2==current_mode) {
		 
	   F_cfg_tmp++;
	   F_cfg_tmp &= 0x01;
				switch(current_menu) {
				case 1:
					     if(0x01 == F_cfg_tmp)
								 selemode |= MODE_PCS;
							 else
								 selemode &= (~MODE_PCS);
							 break;							 
				case 2:
					     if(0x01 == F_cfg_tmp)
								 selemode |= MODE_DENSITY;
							 else
								 selemode &= (~MODE_DENSITY);
							 break;				
				case 3:
					     if(0x01 == F_cfg_tmp)
								 selemode |= MODE_CHECK;
							 else
								 selemode &= (~MODE_CHECK);
							 break;				
				case 4:
					     if(0x01 == F_cfg_tmp)
								 selemode |= MODE_100;
							 else
								 selemode &= (~MODE_100);
							 break;				
				case 5:
					     if(0x01 == F_cfg_tmp)
								 selemode |= MODE_ANIMAL;
							 else
								 selemode &= (~MODE_ANIMAL);
							 break;				
				default:break;					
     	}
	 } 
	else 
	{
	  if(MENU_F_CONFIG_FULL == current_menu)
	   {
	     F_cfg_tmp+=10;
	     if(F_cfg_tmp > 500)
		      F_cfg_tmp = 100; 
	   }
	  else if(MENU_F_CONFIG_REALKG == current_menu)		
		 {
		  switch(real_g_cfg)
					 {
						case 1:  inner_real_g.rec_float -= 0.0001;    break;
					  case 2:  inner_real_g.rec_float -= 0.001;     break;
					  case 3:  inner_real_g.rec_float -= 0.01;      break;
					  case 4:  inner_real_g.rec_float -= 0.1;       break;
					  case 5:  inner_real_g.rec_float -= 1.00;      break;
					  case 6:  inner_real_g.rec_float -= 10.00;     break;
					  case 7:  inner_real_g.rec_float -= 100.000;   break;
					  default:break;	
					 }		
		 } else if(MENU_F_CONFIG_TESTKEY == current_menu) {
		    F_cfg_tmp |= 0x08;
		 } else
		 {  
		   F_cfg_tmp++;
	     if(F_cfg_tmp > (F_Mode_MAX[current_menu]-1))
		   F_cfg_tmp = 0;
	   }	
  }
}	
//////////////////////////////////////////////////////////////////////////
void	Key_F_Confirm_Pro(void) //save 
{
	//Uint32  tmp;
	Uint8   buf[6];
	if(STAT_WEIGH==current_mode)
		return;
	if(STAT_CONFIG2==current_mode) {
		 buf[0] =  selemode;
     buf[1] = 0;
	   buf[2] = buf[0]+buf[1];
	   buf[3] = CHECK_DATA; 
 		 Write_EEPROM(EEP_MODESEL_ADDR, buf, 4); 
	}
	if(STAT_INNER==current_mode)
	 {
		if(1 == Factory_Inncal_step) 
			 zer0_data = new_data_big;
	  else if(2 == Factory_Inncal_step)
		   {
				factory_inner_cal_full = new_data_big - zer0_data;
				buf[0] =  factory_inner_cal_full     &0xff;
        buf[1] = (factory_inner_cal_full>>8) &0xff;
	      buf[2] = (factory_inner_cal_full>>16)&0xff;
	      buf[3] = (factory_inner_cal_full>>24)&0xff; 
				buf[4] =  CHECK_DATA;
        buf[5] =  CHECK_DATA; 
				Write_EEPROM(EEP_INNER_CAL1_ADDR,buf, 6); 
				Factory_Inncal_step = 0; 
       }
		return;
	 }	
  ///////////////////////////////
	 if(MENU_F_CONFIG_TESTKEY == current_menu)
  	{
			F_cfg_tmp |= 0x10;
		} else if(MENU_F_CONFIG_REALKG == current_menu)
  	{
		 buf[0] =  (inner_real_g.rec_uint32)     & 0xff;
     buf[1] = ((inner_real_g.rec_uint32)>> 8)& 0xff;
	   buf[2] = ((inner_real_g.rec_uint32)>>16)& 0xff;
	   buf[3] = ((inner_real_g.rec_uint32)>>24)& 0xff;
		}	else {
	   buf[0] =  F_cfg_tmp    &0xff;
     buf[1] = (F_cfg_tmp>>8)&0xff;
	   buf[2] = buf[0]+buf[1];
	   buf[3] = CHECK_DATA;
    }
 	switch(current_menu)
	{
	 case MENU_F_CONFIG_FULL:   model_id= (F_cfg_tmp<<8)+(model_id&0xff);    Write_EEPROM(EEP_SYS_FULL_ADDR,buf, 4);   break; //高16位
	 case MENU_F_CONFIG_DIV:    model_id=  F_cfg_tmp    +(model_id&0xffff00);Write_EEPROM(EEP_SYS_DIV_ADDR,buf,  4);   break; //低 8位
	 case MENU_F_CONFIG_LOGO:   flag_LOGO       = F_cfg_tmp;                 Write_EEPROM(EEP_SYS_LOGO_ADDR,buf, 4);   break;
	 case MENU_F_CONFIG_LANG:   flag_language   = F_cfg_tmp;                 Write_EEPROM(EEP_SYS_LANG_ADDR,buf, 4);   break;
	 case MENU_F_CONFIG_REP:    repetion_func   = F_cfg_tmp;                 Write_EEPROM(EEP_SYS_REP_ADDR,buf,  4);   break;
	 case MENU_F_CONFIG_INT:    int_func        = F_cfg_tmp;                 Write_EEPROM(EEP_SYS_INT_ADDR,buf,  4);   break;
	 case MENU_F_CONFIG_LINE:   line_cal_en     = F_cfg_tmp;                 Write_EEPROM(EEP_SYS_LINE_ADDR,buf, 4);   break;	
	 case MENU_F_CONFIG_LOCK:   lock_func_en    = F_cfg_tmp;                 Write_EEPROM(EEP_SYS_LOCK_ADDR,buf, 4);   break;	
	 case MENU_F_CONFIG_ASD:    asd_para        = F_cfg_tmp;                 Write_EEPROM(EEP_SYS_ASD_ADDR,buf,  4);   break;	
   case MENU_F_CONFIG_REALKG:	                             	               Write_EEPROM(EEP_SYS_REALKG_ADDR,buf, 4); break;	
   case MENU_F_CONFIG_DISDIV: disdiv_para     = F_cfg_tmp;                 Write_EEPROM(EEP_SYS_DISDIV_ADDR,buf, 4); break;		
	 default:break;
	} 
}

void Key_F_Pro(Uint8 key_code)
   {
   	//校准按键  自动校准  按此键 返回称重模式
		if(KEY_NULL == key_code) 
			return;
		
		Speaker_Time = KEY_TIME_NORMAL;
		
		///////////////////////////////
    switch(key_code)
		{
			case KEY_ON_OFF:       Key_F_ON_OFF_Pro();      break;
			case KEY_ZERO:         Key_F_Zero_Pro();        break; //用于切换配置项 switch 
			case KEY_PRINT:        Key_F_Print_Pro();       break;
			case KEY_CONFIG:       Key_F_Config_Pro();      break; //inner weigh config switch 
			case KEY_SAVE_UP:      Key_F_Up_Pro();          break; //+
			case KEY_REC_DOWN:     Key_F_Down_Pro();        break; //-
			case KEY_CAL_CONFIRM:  Key_F_Confirm_Pro();     break; //ok
			case KEY_TARE_RET:     Key_F_Tare_Ret_Pro();    break;
			default:                                        break;
		}
		  key_code = KEY_NULL;
		  key_flag = FALSE;
   }
