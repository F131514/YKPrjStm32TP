#include "global.h" 

#include "epm7064s.h" 
#include "I2C.h" 
#include "filter.h" 
#include "timer.h"
#include "key.h"


 //显示相关变量  
 Uint8      display_buffer[14],full_flag;    //显存
 Uint8      flag_LOGO,flag_language,Speaker_Time; 
 Uint8      repetion_func,int_func,line_cal_en,lock_func_en,asd_para,disdiv_para;
 un_rec_data   inner_real_g;
 Uint8      sleepmode;                       //睡眠模式
 Uint16     prehot_time,no_key_time;  
 Uint8      machine_mode,current_mode,current_unit,del_last_one;
 Uint8      done_ok_flag,current_menu,current_mode_bak;    //增加配置菜单变量
 Uint8      stable_flag;                       //????
 
 Uint8      power_on_flag,power_on_clr_cnt; //开机上电需要做的一些事情
 
 Uint8      do_zer0_flag,do_tare_flag,do_sam100_flag;
 
 Uint32     clr_data,zer0_data,full_code,new_data,new_data_big,tare_data; //
 
 Uint8      positive_flag,flag_new_data;
 Uint8      asd_var,lnt_var,prt_mod;  //

 Uint8      point10_cal_start,point10_cal_step,point10_cal_ok; 
 Uint32     Point10_Cal_Buf[CAL_POINT_NUM+1];
 float      percent_10_data,point10_k[CAL_POINT_NUM];
 
 Uint32     offset_polar;
 Uint32     offset_50g,offset_100g,offset_150g;
 Uint32     expected_100g,expected_50g,expected_25g;
  
 Uint8      point2_cal_start,point2_cal_step,point2_cal_ok;

 Uint8      inner_code_step;
 Uint8      auto_zer0_valid;   
 
 float      weigh_coef1;
 float      tare_weight,gross_weight,net_weight;  //
 ///////////PCS
 float      weight_per_pcs;    //???
 Uint32     pcs_number;        //??   
 Uint8      sample_pcs_enter,sample_pcs_index,sample_pcs_ok;
 
 ///////////百分比模式
 Uint8       compare_100_flag;
 float       weight_compare_100,number_100;        //????????
 ///////////CHECKmode
 Uint8       check_valid,check_flag,sample_limit_step;
 float       limit1_weight,limit2_weight;          //checkmode
 //动物称重模式
 float       animal_data; 
 Uint8       animal_time_index,animal_run_time,animal_start; 
 //密度称重模式
 Uint8       density_mode_index,density_step,solid_sel_index,liquid_sel_index;
 float       density_data;
 
 ///////////
 Uint8       temp_cmp_flag,temp_error_flag,temp_unit;
 float       temp_float,temp_cmp_point;

///////////////增加记录功能
 Uint8       REC_flag,REC_current_mode,REC_current_unit;
 Uint8       rec_total_num,rec_index,disp_rec_index;      //rec_total_pages;
 Uint8       display_flag_mp;  //记录成功后用于显示符号M+

 /////////////机器型号选择产生的变量
 Uint8       dot_position;
 Uint16      adc_times;
 Uint32      model_id,system_division,inner_code_min,inner_code_max;
 float       additional_num,display_min_value,full_range,weight_alarm;
 Uint16      line_cal_para, user_cal_para;  //此两个参数 根据满量程来决定
 //内校准
 Uint32      motor_run_time,factory_inner_cal_full,user_inner_cal_full;
 Uint8       motor_pos_flag,motor_err_flag,point2_cal_flag,Factory_Inncal_step;
 //add printer function
 Uint8       print_busy,Uart_Printer_Flag;

//增加 打印格式选择
 Uint8      uart_format_type;
 
 //mode selection function 
 Uint8      selemode;
 ///////////////////////////////////////////////////
 //functions 
 //////////////////////////////////////////////////	
//???????????
float const machine_alarm[3] = {
 0.090,  0.0090,  0.00090};

/////////////////////////////////////////
Uint32 const machine_division[] = {
 1000,   10000,  100000};
 


//???????????
float const machine_4s5r[] = {
0.0004,   0.00004,    0.000004
};
//?????????????
float const machine_display_min[] = {
0.004,     0.0007,     0.00009
};

//////////////////////////////////////////////////////////////////////////
const float DENSITY_STAND_SOLID[] = 
{0, 0.90,   2.7,   8.9,    7.8597,  10.5,  11.3,  19.3,   22.50};

const float DENSITY_STAND_LIQUID[] = 
{0, 0.70,  0.79,  0.80,  0.998229,   1.0,   1.4,  3.12,   13.60};
///////////////////////////////////////////////////
Uint8  motor_run_proc(Uint8 target_pos)
{
 Uint8 tmp,tmp2;
 		
  if(target_pos == MOTOR_POSITION)
     return(0);
  else   
    {
     motor_run_time = 0;
     MOTOR_RUN;
     while(1)
        {  
         for(tmp2=0;tmp2<100;tmp2++)
         ;					
         tmp =  MOTOR_POSITION;
         if((motor_run_time==400)||(MOTOR_POSITION==target_pos))
            {
             MOTOR_STOP;
             break;
            } 
        }
     /////////////////////////
     if(motor_run_time==400)  	       
     {
      return(1);
     }
     else
     {
      return(0);
     }
    }  
}

///////////////////////////////////////
//根据满量程 决定用户校 砝码 线性校准砝码
void  get_cal_para(void)
{
 Uint16 i,tmp;
 i = (model_id>>8)&0xffff;
 if(i<150)
 {
  line_cal_para = 100;
  user_cal_para = 100;
 }
 else if(i<250)
 {
  line_cal_para = 200;
  user_cal_para = 200;
 }
 else if(i<350)
 {
  line_cal_para = 300;
  user_cal_para = 300;
 }
 else if(i<450)
 {
  line_cal_para = 400;
  user_cal_para = 400;
 }
 else 
 {
  line_cal_para = 500;
  user_cal_para = 500;
 }
 
}


//////////////////////////////////////////////////
//1
//????? ??????
///////////////////////////////////////////////////
void InitGlobalVarible(void)
	{
	   //Key_Status = KEY_RELEASE ;   //????????
	   ////////////////////////////added after
	   sleepmode         = FALSE;
	   stable_flag       = FALSE;
	   power_on_flag     = TRUE;
	   point2_cal_start  = FALSE;
	   point2_cal_step   = 0;
	   point10_cal_start = FALSE;
  	 power_on_clr_cnt  = POWER_ON_STABLE_TIMES; //????????5???????
     current_mode      = STAT_WEIGH;
		 current_unit      = UNIT_G;
		
     //////////////////////////////////
     do_zer0_flag      = FALSE;
     do_tare_flag      = FALSE;
     do_sam100_flag    = FALSE;
     tare_data         = 0;
     weight_per_pcs    = 0.0;
     weight_compare_100= 0.0;
     ////////////////////////////////
		 solid_sel_index   = 4;
		 liquid_sel_index  = 4;
     no_key_time       = 0;
		 //Speaker_Time      = 0;
     
     temp_cmp_flag     = FALSE; 
     temp_error_flag   = FALSE;		 
	}
 
//////////////////////////////////////////////////
 //3a
 //?????? ID
 ///////////////////////////////////////////////////	 
void  Init_SYSLOGO_Para(void)
  {
	 Uint8 i,buf[4];
	 
	 Read_EEPROM(EEP_SYS_LOGO_ADDR, buf, 4);
	 for(i=0;i<200;i++){;}   //delay
	 if((CHECK_DATA==buf[3])&&(buf[2]==buf[1]+buf[0]))
		 flag_LOGO = buf[0]&0x03;
	 else
		 flag_LOGO = DEFAULT_MACHINE_LOGO;
  
	 Read_EEPROM(EEP_MODESEL_ADDR, buf, 4);
	 for(i=0;i<200;i++){;}   //delay
	 if((CHECK_DATA==buf[3])&&(buf[2]==buf[1]+buf[0]))
		 selemode = buf[0];
	 else
		 selemode = 0x01;
	
	
	} 	

///////////////////////////////////////////////////
//继电器操作 用于双分度切换，备用功能
//////////////////////////////	 
void Set_Relay_Oper(void)
     {
      
      if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
        {
         //FIO0SET = RELAY_OUT_PIN;
        }
      else
        {
         //FIO0CLR = RELAY_OUT_PIN;
        } 
      }	
///////////////////////////////////////////////
//根据机器型号 初始化系统变量包含了
// 分辨率
// 满量程重量
// 满量程内码最小数值
// 零点内码最小数值
// 
// 四舍五入数值
// 最小显示数值
/////////////////////////////////////////////// 
void  Init_MachineVarible(void)
{	 
  //dot_position = model_id&0xff;
  //Set_Relay_Oper();
  
  additional_num     = machine_4s5r[dot_position];
  display_min_value  = machine_display_min[dot_position];
	
	
	
  weight_alarm       = ((model_id>>8)&0xffff) + machine_alarm[dot_position];
  full_range         =  (model_id>>8)&0xffff;   
  system_division	   =  full_range * machine_division[dot_position];
	 
  inner_code_min    = system_division*INNER_CODE_STEP_DEF/2;
  inner_code_max    = system_division*INNER_CODE_STEP_DEF*3;  
} 
		 		 
//////////////////////////////////////////////////
//初始化 用户参数1
///////////////////////////////////////////////////	 
Uint8  Init_UARTCONFIG_Para(void) 
	///////////读出用户配置数据
 {  
	 Uint32 i,j;
	 Uint8  buf[16];
	 
   Read_EEPROM(EEP_USR_FUN5_ADDR, buf, 8);
   for(i=0;i<1000;i++)	{;}	             //delay
   //第一个参数为 通讯速率
	 if((0x00!=buf[0])&&(CHECK_DATA==buf[3])&&(buf[2]==buf[1]+buf[0]))
		   uart_format_type = buf[0];
	 else 
 		   uart_format_type = UART_TYPE_DEF;
 
	 if((0x00!=buf[4])&&(CHECK_DATA==buf[7])&&(buf[6]==buf[5]+buf[4]))
		   Uart_Printer_Flag = buf[4];
	 else 
 		   Uart_Printer_Flag = 1;  //to PC
  
	 if(2 == Uart_Printer_Flag)  //to printer ,force baud to 9600
  		   uart_format_type = UART_TYPE_AUTO_F1_9600;
 }

//////////////////////////////////////////////////
//初始化 用户参数2
///////////////////////////////////////////////////
Uint8  Init_USERCONFIG_Para(void)
{
	 Uint32 i,j;
	 Uint8  buf[16];
		
 	 Read_EEPROM(EEP_USR_FUN1_ADDR, buf, 16);
   for(i=0;i<1000;i++)	{;}	             //delay
   //第一个参数为 用户选择模式 不保存
	 //第二个参数为 开始校验 依次为 重量单位 温度单位 是否显示最后一位
 	 if((CHECK_DATA==buf[7])&&(buf[6]==buf[5]+buf[4]))
		   current_unit = buf[4];
	 else 
 		   current_unit = UNIT_G;
	 /////////////////////////////////////////////////////////////////
	 if((CHECK_DATA==buf[11])&&(buf[10]==buf[9]+buf[8]))
		   temp_unit = buf[8];
	 else 
 		   temp_unit = TEMP_UNIT_OC;
	 /////////////////////////////////////////////////////////////////
	 if(QIAN_DIVISION == dot_position)     //在千分度下不显示最后一位
		  del_last_one = 0x01;
	 else if((CHECK_DATA==buf[15])&&(buf[14]==buf[13]+buf[12]))
		   del_last_one = buf[12];
	 else 
 		   del_last_one = 0x00;  //保留最后一位显示
	 
}
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
Uint8  Init_SYSCONFIG_Para(void)
{
	 Uint32 i,j;
	 Uint8  Rcv_buf[16];
	 model_id = 0xffffff;
	
 	 Read_EEPROM(EEP_SYS_FULL_ADDR, Rcv_buf, 16);
   for(i=0;i<1000;i++)	{;}	             //delay
   //////////////////////////////////////////////////校验四个参数
 	 if((CHECK_DATA==Rcv_buf[3])&&(Rcv_buf[2]==Rcv_buf[1]+Rcv_buf[0]))
	 {
		model_id &= 0xff; 
		model_id |= ((Rcv_buf[1]<<16) + (Rcv_buf[0]<<8));
	 }
 	 if((CHECK_DATA==Rcv_buf[7])&&(Rcv_buf[6]==Rcv_buf[5]+Rcv_buf[4]))
	 {
		model_id &= 0xffff00; 
		model_id |= Rcv_buf[4];
	 }
	 
	 if(0xffffff == model_id)
    model_id = DEFAULT_MACHINE_ID;
   
	 //根据满量程 自动 计算校准砝码 以及线性校 数据
	 get_cal_para();
	 
	 ////////////////////////////////////////////////////////////////////////
	 //LOGO 使能 因为前面已经验证，在此不再验证
	 ////////////////////////////////////////////////////////////////////////
	 //验证语言 选项
	 if((CHECK_DATA==Rcv_buf[15])&&(Rcv_buf[14]==Rcv_buf[13]+Rcv_buf[12]))
	  	flag_language = Rcv_buf[12]&0x01; 
	 else
		 	flag_language = DEFAULT_MACHINE_LANG;
	 
	 for(i=0;i<123456;i++);   //延时
	 for(i=0;i<16;i++)
	 Rcv_buf[i] = 0x00;
	 
	 //////////////////////////////////////////////////再校验四个参数
	 Read_EEPROM(EEP_SYS_REP_ADDR, Rcv_buf, 16);
	 if((CHECK_DATA==Rcv_buf[3])&&(Rcv_buf[2]==Rcv_buf[1]+Rcv_buf[0])) //重复性 功能
		  repetion_func = Rcv_buf[0]; 
   else
		  repetion_func = DEFAULT_MACHINE_REP;
	 //////////////////////////////////////////////////////////////////////
	 if((CHECK_DATA==Rcv_buf[7])&&(Rcv_buf[6]==Rcv_buf[5]+Rcv_buf[4])) //临界取整数
	 	  int_func = Rcv_buf[4]; 
   else
		  int_func = DEFAULT_MACHINE_INT;		 
	 //////////////////////////////////////////////////////////////////////
	 if((CHECK_DATA==Rcv_buf[11])&&(Rcv_buf[10]==Rcv_buf[9]+Rcv_buf[8])) //线性校准 使能
	 	  line_cal_en = Rcv_buf[8]; 
   else
		  line_cal_en = TRUE;
   if((CHECK_DATA==Rcv_buf[15])&&(Rcv_buf[14]==Rcv_buf[13]+Rcv_buf[12])) //锁字功能 使能
	 	  lock_func_en = Rcv_buf[12]; 
   else
		  lock_func_en = FALSE;
   
	 //////////////////////////////////////////////////再校验四个参数	 
	 for(i=0;i<123456;i++);   //延时
	 for(i=0;i<16;i++)
	 Rcv_buf[i] = 0x00;
	 Read_EEPROM(EEP_SYS_REALKG_ADDR, Rcv_buf, 8);
	
	 //////////////////////////////////////////////////////////////////内校砝码真实数据
	 //此参数为内校参数 
	 inner_real_g.rec_uint32 = (Rcv_buf[3]<<24) + (Rcv_buf[2]<<16) + (Rcv_buf[1]<<8) + Rcv_buf[0]; 
   if((inner_real_g.rec_float<90)||(inner_real_g.rec_float>110))			
	     {
 				inner_real_g.rec_float = 108.000;
       } 
	
    //////////////////////////////////////////////////////////////////////
	 if((CHECK_DATA==Rcv_buf[7])&&(Rcv_buf[6]==Rcv_buf[5]+Rcv_buf[4])) //显示分辨率
	 	  disdiv_para = Rcv_buf[4]; 
   else
		  disdiv_para = DEFAULT_MACHINE_DISDIV;		 			 
		////////////////////////////////////////////////////////////////////////
	  Init_MachineVarible();
}	  

 //////////////////////////////////////////////////
 //5
 //????zhongliang????
  ///////////////////////////////////////////////////
Uint8  Init_Userfull_Para(void)
	{
	 Uint32 i,full_bak;
	 Uint8  buf[8];
	 
	 if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position)) 
	    Read_EEPROM(EEP_WEIFULL2_ADDR, buf, 8);
	 else
	    Read_EEPROM(EEP_WEIFULL1_ADDR, buf, 8);
	
	 for(i=0;i<1000;i++){;}   //delay
	 full_code = (buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3];
	 full_bak  = (buf[4]<<24) + (buf[5]<<16) + (buf[6]<<8) + buf[7];
	 //校验     
	 if((full_code==full_bak)&&(full_code>inner_code_min)&&(full_code < 50000000))
	  {
   	  inner_code_step = full_code/system_division;
   	  weigh_coef1 = full_range / full_code;
   	  point2_cal_ok = TRUE;
	    if((inner_code_step > INNER_CODE_STEP_MAX)||(inner_code_step < INNER_CODE_STEP_MIN))      
   	    inner_code_step = INNER_CODE_STEP_DEF;
	    return(OK);
	  }
	  else if((full_bak>inner_code_min)&&(full_bak<inner_code_max)) 
	   {//???????2
	    full_code       = full_bak;
	    inner_code_step = full_bak/system_division;
   	  weigh_coef1     = full_range / full_bak;
   	  point2_cal_ok   = TRUE;
	    if((inner_code_step > INNER_CODE_STEP_MAX)||(inner_code_step < INNER_CODE_STEP_MIN))      
   	    inner_code_step = INNER_CODE_STEP_DEF;
	    return(OK);
	   }
	  else
	   {
	    full_code       = 2000000 * INNER_CODE_STEP_DEF;
	    inner_code_step = INNER_CODE_STEP_DEF;
	    weigh_coef1     = 200.0/(2000000*INNER_CODE_STEP_DEF); //?????
	    point2_cal_ok   = FALSE;
	    return(ERROR);                                         //error
   	 }
   }	
 //////////////////////////////////////////////////
 //6
 //线性校准 0点 
  ///////////////////////////////////////////////////
Uint8  Init_Line_Zero(void)
	{
	 Uint32 i,zer0_cal,zer0_cal_bak;
	 Uint8  buf[16];
	 
	 if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position)) //?????? ?????????
	    Read_EEPROM(EEP_WEIZERO_L2_ADDR, buf,16);
	 else
	    Read_EEPROM(EEP_WEIZERO_L1_ADDR, buf,16);
	 for(i=0;i<1000;i++){;}   //delay
	 zer0_cal     = (buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3];
	 zer0_cal_bak = (buf[4]<<24) + (buf[5]<<16) + (buf[6]<<8) + buf[7];
	 
	   //??????
	   if((zer0_cal==zer0_cal_bak)&&(zer0_cal>ZER0_CODE_MIN)&&(zer0_cal<ZER0_CODE_MAX))
	    {
   	   clr_data  = zer0_cal;       //??????
   	  }
   	 else if((zer0_cal_bak>ZER0_CODE_MIN)&&(zer0_cal_bak<ZER0_CODE_MAX))
   	  {
   	   clr_data = zer0_cal_bak;    //??????
   	  }
   	 else 
   	   clr_data = 0;
   	  /////////////////////////////////////////// 
   	   if(clr_data==0)  
   	   {
	      return(ERROR);                 //error
   	   }
   	 else   
   	   {
   	    return(OK);
	     }
	}   
   	  
   	  
  //////////////////////////////////////////////////
 //6
 //???? lingdian ????
  ///////////////////////////////////////////////////
Uint8  Init_User_Zero(void)
	{
	 Uint32 i,zer0_cal,zer0_cal_bak;
	 Uint8  buf[16];  
 	  
   	/////////////////////
   	 if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position)) //?????? ?????????
	    Read_EEPROM(EEP_WEIZERO_U2_ADDR, buf,16);
	   else
	    Read_EEPROM(EEP_WEIZERO_U1_ADDR, buf,16);
	   for(i=0;i<1000;i++){;}   //delay
   	 zer0_cal     = (buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3];
	   zer0_cal_bak = (buf[4]<<24) + (buf[5]<<16) + (buf[6]<<8) + buf[7];
	   //??????
	   if((zer0_cal==zer0_cal_bak)&&(zer0_cal>ZER0_CODE_MIN)&&(zer0_cal<ZER0_CODE_MAX))
	    {
   	   zer0_data  = zer0_cal;       //??????
   	  }
   	 else if((zer0_cal_bak>ZER0_CODE_MIN)&&(zer0_cal_bak<ZER0_CODE_MAX))
   	  {
   	   zer0_data = zer0_cal_bak;    //??????
   	  }
   	 else 
   	   zer0_data = 0;  
   	 ////////////////////////////////////////////////////////  
   	 if(zer0_data==0)  
   	   {
   	    point2_cal_ok = FALSE;
	      return(ERROR);                 //error
   	   }
   	 else   
   	   {
   	    point2_cal_ok = TRUE;
	      return(OK);
	     } 
    }	   
 //////////////////////////////////////////////////
 //7
 //????????
 ///////////////////////////////////////////////////
void   Get_Linecal10_para(Uint32 full_data)
    {
     Uint8 i;
     if(TRUE == point10_cal_ok)  //?????????
      {
       percent_10_data =  full_data / (CAL_POINT_NUM-1);  //??10??
       for(i=0;i<10;i++)
	     point10_k[i] = percent_10_data/(Point10_Cal_Buf[i+1]-Point10_Cal_Buf[i]); //???? ?????
	    }
	   else
	    {
	     percent_10_data = 0.0;
	     for(i=0;i<10;i++)
	     point10_k[i] = 0.0;
	    }   
	 }
	 /////////////////////////////////////////////////
//3 ??????????
//  ????????,??????????????,??
//  ????
/////////////////////////////////////////////////
Uint32  Get_Line10_data(Uint32 data_no_line)
{  
    Uint32 where_k,data_offset;
    Uint32 pure_wei_code;
    ///////////////第一种情况
    if(data_no_line < clr_data)  //标定0位以下 不处理
      return(data_no_line);
    /////////////////////纯重量内码  
    pure_wei_code = data_no_line - clr_data;
    
    if(pure_wei_code > Point10_Cal_Buf[10])   //
     {     //////////////////第二种情况，使用最后一段斜率
      data_offset = pure_wei_code - Point10_Cal_Buf[10]; 
      data_offset = data_offset * point10_k[9];
      return((Uint32)(clr_data + 10*percent_10_data + data_offset));
     }
    ////////////////////////////////////////正常区间 计算偏移量
    for(where_k=1;where_k<11;where_k++)    //1 查找位置
    {                                         
     if(pure_wei_code < Point10_Cal_Buf[where_k])
     	break;
     }
    /////////////////////////////////////////////2 超出某个校准点的数据
    data_offset = pure_wei_code - Point10_Cal_Buf[where_k-1]; //
    /////////////////////////////////////////////3 拉回到校准点
    data_offset = data_offset * point10_k[where_k-1]; 
    return((Uint32)(clr_data + (where_k-1)*percent_10_data + data_offset));
}	
 //////////////////////////////////////////////////
 //7
 //????????
 ///////////////////////////////////////////////////
void   Get_Linecal4_para(void)
    {
     Uint32 i,j;
     // [1] [2]  [3]  [4]
     // 50  100  150  200
		 
     expected_100g = Point10_Cal_Buf[4]/2;
     expected_50g  = expected_100g/2;
     expected_25g  = expected_50g/2;
     
     if(expected_100g > Point10_Cal_Buf[2]) //???100???
        offset_polar = 0;             //???
     else
        offset_polar = 1;             //???
     
     offset_100g = abs(expected_100g-Point10_Cal_Buf[2]); //??100???????
     ///////////////////////////////////////////////////////?? 50???????
     if(expected_50g > Point10_Cal_Buf[1])
       {
         if(0 == offset_polar)
           {
             i = expected_50g-Point10_Cal_Buf[1];
             if(i> offset_100g/2)
                offset_50g = i-offset_100g/2;
             else
                offset_50g = 0;  
           }
         else 
           offset_50g = 0;
         
       }
     else
       {
         if(1 == offset_polar)
           {
             i = Point10_Cal_Buf[1] - expected_50g;
             if(i> offset_100g/2)
                offset_50g = i-offset_100g/2;
             else
                offset_50g = 0;  
           }
         else 
           offset_50g = 0; 
       }  
     ///////////////////////////////////////////////////////??150???????
     j = expected_100g+expected_50g;
     
     if(j > Point10_Cal_Buf[3])
       {
         if(0 == offset_polar)
           {
             i = j - Point10_Cal_Buf[3];
             if(i> offset_100g/2)
                offset_150g = i-offset_100g/2;
             else
                offset_150g = 0;  
           }
         else 
           offset_150g = 0;
         
       }
     else
       {
         if(1 == offset_polar)
           {
             i = Point10_Cal_Buf[3] - j;
             if(i> offset_100g/2)
                offset_150g = i-offset_100g/2;
             else
                offset_150g = 0;  
           }
         else 
           offset_150g = 0; 
       }  
	}	
////////////////////////////////////////////////////////////
// ?? ???? ??
////////////////////////////////////////////////////////////	
Uint32  Get_Line4_data(Uint32 weight1)
{  
  Uint32 i,j,k,weight2;
  float  offset_tmp[3];  
  float  offset_temp;
  
  if(weight1 < clr_data)
    return(weight1);
    
  weight2 = weight1 - clr_data;
  
  if(weight2 > Point10_Cal_Buf[4])  //????
    {
      i = weight2 - Point10_Cal_Buf[4];
      offset_temp = i*(offset_100g/(expected_50g+1.01));
      if(0 == offset_polar)       
       return(weight1-(Uint32)offset_temp); 
      else
       return(weight1+(Uint32)offset_temp);
    }
  else
    { //????? ??
      offset_tmp[0] = 0.0;
      offset_tmp[1] = 0.0;
      offset_tmp[2] = 0.0;
      for(i=0;i<3;i++)
      {
        if(i==0)
         {
          j = abs(expected_100g-abs(expected_100g-weight2));
          offset_tmp[0] = j*(offset_100g/(expected_100g+1.01)); 
          k = abs(expected_100g-weight2);
         }
        if(i==1)
         {
          j = abs(expected_50g-abs(expected_50g-k));
          if(weight2 > expected_100g)
          offset_temp = j*(offset_150g/(expected_50g+1.01)); 
          else
          offset_temp = j*(offset_50g/(expected_50g+1.01));
          offset_tmp[1] = offset_temp;  
          k = abs(expected_50g-k);  
         }    
        if(i==2)
         {
          j = abs(expected_25g-abs(expected_25g-k));
          if(weight2 > expected_100g)
          offset_temp = j*(offset_150g/(expected_50g+1.01)); 
          else
          offset_temp = j*(offset_50g/(expected_50g+1.01));
          offset_tmp[2] = offset_temp/2;
          //offset_tmp[2] = 0;  
         }
      }  //???????  
   
      k = (Uint32)(offset_tmp[0]+offset_tmp[1]+offset_tmp[2]);
     
      if(0 == offset_polar)       
        return(weight1+k); 
      else
       {
        return(weight1-k);
       }
    } 
}	
//////////////////////////////////////////////////
//8
//??????????
///////////////////////////////////////////////////	    
Uint8  Init_Linecal4_Variable(void)
	{
	 Uint32 i,j;
	 Uint32 checksum;
	 Uint8  WeighCalData[16];
	 
	 for(i=0;i<2;i++)
	 {
	  if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
	     Read_EEPROM(EEP_LINCAL2_ADDR01+i*16, WeighCalData, 16);
	  else
	     Read_EEPROM(EEP_LINCAL1_ADDR01+i*16, WeighCalData, 16);
	  
	  for(j=0;j<1000;j++){;}   //delay
	  for(j=0;j<4;j++)
	  Point10_Cal_Buf[4*i+j] = (WeighCalData[4*j+0]<<24) + (WeighCalData[4*j+1]<<16) + (WeighCalData[4*j+2]<<8) + WeighCalData[4*j+3];
	 }
	  //////////////check2
	  checksum = 0;
	  for(j=0;j<5;j++)
	  checksum += Point10_Cal_Buf[j];
	   
	  ///////////////////////////////////////////////////
	  if(checksum == Point10_Cal_Buf[5])
	  {//???? ???????????  
	   point10_cal_ok = TRUE;
	   Get_Linecal4_para();
	   return(OK);
	  }
	  else
	  {//????
	   point10_cal_ok = FALSE;
	   return(ERROR);
	  }
   }	
///////////////////////////////////////////////////////
Uint8  Init_Linecal10_Para(void)
	{
	 Uint32 i,j;
	 Uint32 checksum;
	 Uint8  buf[16];
	 
	 for(i=0;i<3;i++)
	 {
	  if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
	     Read_EEPROM(EEP_LINCAL2_ADDR01+i*16, buf, 16);
	  else
	     Read_EEPROM(EEP_LINCAL1_ADDR01+i*16, buf, 16);
	  
	  for(j=0;j<1000;j++){;}   //delay
	  for(j=0;j<4;j++)
	  Point10_Cal_Buf[4*i+j] = (buf[4*j+0]<<24) + (buf[4*j+1]<<16) + (buf[4*j+2]<<8) + buf[4*j+3];
	 }
	  //////////////check2
	  checksum = 0;
	  for(j=0;j<11;j++)
	  checksum += Point10_Cal_Buf[j];
	   
	  ///////////////////////////////////////////////////
	  if(checksum == Point10_Cal_Buf[11])
	  {//
	   point10_cal_ok = TRUE;
	   Get_Linecal10_para(Point10_Cal_Buf[10]);
	   return(OK);
	  }
	  else
	  {//????
	   point10_cal_ok = FALSE;
	   return(ERROR);
	  }
   }	
//////////////////////////////////////////////////
 
//////////////////////////////////////////////////
void  Del_rec_all(void)
{
	Uint8 i,buf[8];
  for(i=0;i<8;i++)
	   buf[i] = 0;
	Write_EEPROM(EEP_REC_TOTAL_ADDR,buf, 8); 
	for(i=0;i<200;i++)	{;}	                 //delay
	rec_total_num = 0;  //??????,???????
	rec_index = 0;
}	
///////////////////////////////////////////////////	     
void  Search_Rec_Pos(void)
     {
       Uint32 i,j;
 	     Uint8  BufferData[8];
	   
     Read_EEPROM(EEP_REC_TOTAL_ADDR,BufferData,8);
	   for(j=0;j<1000;j++){;}   //delay
	   
	   i = (BufferData[0]<<24)+(BufferData[1]<<16)+(BufferData[2]<<8)+BufferData[3];
	   j = (BufferData[4]<<24)+(BufferData[5]<<16)+(BufferData[6]<<8)+BufferData[7];
	   
	   if((0xffffffff==i)&&(0xffffffff==j)||(i!=j))
	   {//???
	    Del_rec_all();
	   }
	   ////////////////////////////////////////////??
	   else if(i==j)
	   {                        //??,?????????
	    rec_total_num = i;      //??????
	    rec_index = i%MAX_REC_NUM;
	   }
     else
		 {
		  //读到的记录有错误
			 rec_total_num = 0;
			 rec_index = 0;
		 }			 
    }   

//////////////////////////////////////////////////
//13 ???????????
//return 
///////////////////////////////////////////////////
Uint8  System_Init(void)
{   
    Uint16 i,j;
     
    InitGlobalVarible();
    Search_Rec_Pos();           //???????????
	  //用户配置数据 //系统配置数据
	 
    Init_SYSCONFIG_Para();      //系统配置数据
    for(i=0;i<1000;i++){;}      //delay	
	  Init_USERCONFIG_Para();
   	for(i=0;i<1000;i++){;}      //delay
		///////////////////////////////////////////	
		////////////////////线性校准 线性校准0点 用户校准0点 用户校准满量程
    Init_Linecal10_Para();
    for(i=0;i<1000;i++){;}      //delay
		Init_Line_Zero();
		for(i=0;i<1000;i++){;}      //delay		
    //////////////////////////////用户校准数据
 	  Init_Userfull_Para();
		for(i=0;i<1000;i++){;}      //delay
		Init_User_Zero();
		for(i=0;i<1000;i++){;}      //delay	 

//////////////////////////////////////////////////////////	
		 Key_Null_Pro();
		 i = 450;
		 do
		 {
			if(flag_10ms_ok)
			 {
				flag_10ms_ok = FALSE; 
				i--; 
				if((KEY_LONG + KEY_TARE_RET) == Get_Key_Code())
			
					break;			
				}
		 }
		 while(i!=0);
		 
	   if(0 == i)
		   return(MACHINE_NORMAL_MODE);
     else 
       return(MACHINE_FACTORY_MODE);	
/////////////////////////////////////////////////////////////////		 
}

	
///////////////////////////////////////////////////////////
//15 ??????
///////////////////////////////////////////////////////////	
void  Set_Zer0_Flag(void)
{
  do_zer0_flag = TRUE;
}
///////////////////////////////////////////////////////////
//16 ??????
///////////////////////////////////////////////////////////
void  Set_Tare_Flag(void)
{
  do_tare_flag = TRUE;
}
///////////////////////////////////////////////////////////
//17 ???????????
///////////////////////////////////////////////////////////
void  Set_Sam100_Flag(void)
{
  do_sam100_flag = TRUE;
}

////////////////////////////////////////////////////
///以下函数 来自于 主文件MAIN 在此放在GLOBAL 中
////////////////////////////////////////////////////
////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//强制结束校准
//////////////////////////////////////////////////////
void  End_Cal_Pro(void)
    {
     point2_cal_start = FALSE;
     current_mode = STAT_WEIGH;  //switch to weigh mode
    }
    
void  End_Cal_Pro_Inn(void)
    {
     point2_cal_start = FALSE;
     current_mode = STAT_WEIGH;  //switch to weigh mode
     //????
	   motor_err_flag = motor_run_proc(UNLOAD_POSITION); 
    }
/////////////////////////////////////////////////////////////
//线性校准函数
////////////////////////////////////////////////////////////
static Uint32 Cal_Buf_Tmp[CAL_POINT_NUM+1];     //???		
		
void  point10_cal_proc(Uint32 weight1)
  {
   Uint8  i,Line_CalData[16];
   static Uint32 last_cal_raw_data;
          Uint32 x,j,CheckSum;
   x = abs(weight1-zer0_data);
   ///////////////////////////
   switch(point10_cal_step)
       {
         case 0: // 0 点
                 Cal_Buf_Tmp[point10_cal_step] = 0;
                 last_cal_raw_data = 0;
                 clr_data = weight1;
                 zer0_data = clr_data;
                 Line_CalData[0] = (Uint8)(clr_data>>24);
  	   			     Line_CalData[1] = (Uint8)((clr_data>>16)&0xff);
  	   			     Line_CalData[2] = (Uint8)((clr_data>>8)&0xff);
  	   			     Line_CalData[3] = (Uint8)(clr_data&0xff); 
  	             Line_CalData[4] = Line_CalData[0];
  	  			     Line_CalData[5] = Line_CalData[1];
  	  			     Line_CalData[6] = Line_CalData[2];
  	   			     Line_CalData[7] = Line_CalData[3]; 
  	   			
  	   			     if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
                 Write_EEPROM(EEP_WEIZERO_L2_ADDR, Line_CalData, 8);	 //?????????
  	   			     else
  	   			     Write_EEPROM(EEP_WEIZERO_L1_ADDR, Line_CalData, 8);	 //?????????
       			 
								 point10_cal_step++;
								 auto_zer0_valid = FALSE;
      			     stable_manual_break();
      			     break;
         case  1://1/10
         case  2://2/10
         case  3://3/10
         case  4://...
				 case  5:	 
				 case  6:
         case  7:
         case  8:
       	 case  9:
         case 10://满量程					 
                 if((x>last_cal_raw_data)&&((x-last_cal_raw_data) > 1200000))  
                  {
                   last_cal_raw_data = x;
                   Cal_Buf_Tmp[point10_cal_step] = x;
                   
									 point10_cal_step++; //当此值为5时，由显示程序终止校准过程
									 auto_zer0_valid = FALSE;
                   stable_manual_break(); 
                  }
                  break;
				 case 11:  //自动保存
                for(i=0;i<11;i++)
       	        Point10_Cal_Buf[i] = Cal_Buf_Tmp[i];
       	        //////////////////////////////////save to EEPROM
       	        CheckSum = 0;
       	        for(i=0;i<11;i++)  //最后一位放置校验和
       	          CheckSum += Cal_Buf_Tmp[i];
       	        Cal_Buf_Tmp[11] = CheckSum;
       	       
       	       for(i=0;i<3;i++)  //总共写3次
       	       {
       	        for(j=0;j<4;j++) //每次4个点的数据
       	        {
       	         Line_CalData[4*j+0] = Cal_Buf_Tmp[i*4+j]>>24;
       	         Line_CalData[4*j+1] = Cal_Buf_Tmp[i*4+j]>>16;
       	         Line_CalData[4*j+2] = Cal_Buf_Tmp[i*4+j]>>8;
       	         Line_CalData[4*j+3] = Cal_Buf_Tmp[i*4+j]&0xff;
       	        }
       	        
       	        if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
       	        Write_EEPROM(EEP_LINCAL2_ADDR01+i*16, Line_CalData, 16); // ?????????16???
       	        else
			          Write_EEPROM(EEP_LINCAL1_ADDR01+i*16, Line_CalData, 16); // ?????????16???
			          for(j=0;j<400000;j++)	{;}	                 //delay 
       	        } 
       	        /////////////////////////////////////////CHECK
       	        //////////////////////////////////////////////use current data
       	        point10_cal_start = FALSE;
       	        point10_cal_ok = TRUE;
       	        Get_Linecal10_para(Point10_Cal_Buf[10]);
								//增加一步，用于显示
								point10_cal_step++;
				default:break;				
	    } 					 
  }				
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////	
void  point4_cal_proc(Uint32 weight1)
  {
   Uint8  i,j,Line_CalData[16];
   static Uint32 last_cal_raw_data;
          Uint32 x,CheckSum;
   x = abs(weight1-zer0_data);
   ///////////////////////////
   switch(point10_cal_step)
       {
         case 0: // 0 点
                 Cal_Buf_Tmp[point10_cal_step] = 0;
                 last_cal_raw_data = 0;
                 clr_data = weight1;
                 zer0_data = clr_data;
                 Line_CalData[0] = (Uint8)(clr_data>>24);
  	   			     Line_CalData[1] = (Uint8)((clr_data>>16)&0xff);
  	   			     Line_CalData[2] = (Uint8)((clr_data>>8)&0xff);
  	   			     Line_CalData[3] = (Uint8)(clr_data&0xff); 
  	             Line_CalData[4] = Line_CalData[0];
  	  			     Line_CalData[5] = Line_CalData[1];
  	  			     Line_CalData[6] = Line_CalData[2];
  	   			     Line_CalData[7] = Line_CalData[3]; 
  	   			
  	   			     if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
                 Write_EEPROM(EEP_WEIZERO_L2_ADDR, Line_CalData, 8);	 //?????????
  	   			     else
  	   			     Write_EEPROM(EEP_WEIZERO_L1_ADDR, Line_CalData, 8);	 //?????????
       			 
								 point10_cal_step++;
								 auto_zer0_valid = FALSE;
      			     stable_manual_break();
      			     break;
         case 1://50
         case 2://100
         case 3://150
         case 4://200
                 if((x>last_cal_raw_data)&&((x-last_cal_raw_data) > 2000000*15/8))  
                  {
                   last_cal_raw_data = x;
                   Cal_Buf_Tmp[point10_cal_step] = x;
                   
									 point10_cal_step++; //当此值为5时，由显示程序终止校准过程
									 auto_zer0_valid = FALSE;
                   stable_manual_break(); 
                  }
                  break;
				 case 5:  //自动保存
              for(i=0;i<5;i++)
       	        Point10_Cal_Buf[i] = Cal_Buf_Tmp[i];
       	        //////////////////////////////////save to EEPROM
       	        CheckSum = 0;
       	        for(i=0;i<5;i++)  //??5??????
       	          CheckSum += Cal_Buf_Tmp[i];
       	        Cal_Buf_Tmp[5] = CheckSum;
       	       
       	       for(i=0;i<2;i++)  //总共写2次
       	       {
       	        for(j=0;j<4;j++) //每次4个点的数据
       	        {
       	         Line_CalData[4*j+0] = Cal_Buf_Tmp[i*4+j]>>24;
       	         Line_CalData[4*j+1] = Cal_Buf_Tmp[i*4+j]>>16;
       	         Line_CalData[4*j+2] = Cal_Buf_Tmp[i*4+j]>>8;
       	         Line_CalData[4*j+3] = Cal_Buf_Tmp[i*4+j]&0xff;
       	        }
       	        
       	        if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
       	        Write_EEPROM(EEP_LINCAL2_ADDR01+i*16, Line_CalData, 16); // ?????????16???
       	        else
			          Write_EEPROM(EEP_LINCAL1_ADDR01+i*16, Line_CalData, 16); // ?????????16???
			          for(j=0;j<400000;j++)	{;}	                 //delay 
       	        } 
       	        /////////////////////////////////////////CHECK
       	        //////////////////////////////////////////////use current data
       	        point10_cal_start = FALSE;
       	        point10_cal_ok = TRUE;
       	        Get_Linecal4_para();
				default:break;				
	    } 					 
  }		
/////////////////校准函数		
void  point2_cal_proc(Uint32 weight_tmp)
   {
    static Uint32 tmp;
    Uint32 i;
    Uint8  eeprom_tx_buf[8];
    if(USER_CAL_ZER0_FLASH==point2_cal_step)
		{
		 if((TRUE == stable_flag)&&(weight_tmp>ZER0_CODE_MIN)&&(weight_tmp<ZER0_CODE_MAX))
		    {
					     zer0_data = weight_tmp;
               tare_data = 0;
               //??? ????????? ??  ?????????
               eeprom_tx_buf[0] = (Uint8)( zer0_data>>24);
  	           eeprom_tx_buf[1] = (Uint8)((zer0_data>>16)&0xff);
  	           eeprom_tx_buf[2] = (Uint8)((zer0_data>>8)&0xff);
  	           eeprom_tx_buf[3] = (Uint8)( zer0_data&0xff); 
  	 
  	           eeprom_tx_buf[4] = eeprom_tx_buf[0];
  	           eeprom_tx_buf[5] = eeprom_tx_buf[1];
  	           eeprom_tx_buf[6] = eeprom_tx_buf[2];
  	           eeprom_tx_buf[7] = eeprom_tx_buf[3]; 
  	          
  	           if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
  	           Write_EEPROM(EEP_WEIZERO_U2_ADDR, eeprom_tx_buf, 8);	 //?????????
               else
               Write_EEPROM(EEP_WEIZERO_U1_ADDR, eeprom_tx_buf, 8);	 //?????????
          
               stable_manual_break();
					     point2_cal_step++;
		     }
		}		
		else if(USER_CAL_FULL_FLASH == point2_cal_step) //
		{
		  if(weight_tmp>(zer0_data + 10000000))
			    point2_cal_step++;	
		}
		else if(USER_CAL_FULL == point2_cal_step)
       {     
				    if((TRUE==stable_flag)&&(weight_tmp>USERCAL_CODE_MIN)&&(weight_tmp<USERCAL_CODE_MAX))
							{    
               //根据用户校准砝码 更新系数 以及每个分度内码数值
               tmp = weight_tmp - zer0_data;
               //////////////////////////////////////////////////////////////////
               weigh_coef1     =  user_cal_para/(tmp+0.5);		
							 full_code       =  full_range / weigh_coef1;               
							 inner_code_step =  full_code/system_division;
 								
               ///////////////////////////////////////////////////////////////////   
              
    	         eeprom_tx_buf[0] = (Uint8)(full_code>>24);
  	           eeprom_tx_buf[1] = (Uint8)((full_code>>16)&0xff);
  	           eeprom_tx_buf[2] = (Uint8)((full_code>>8)&0xff);
  	           eeprom_tx_buf[3] = (Uint8)(full_code&0xff);
  	            
  	           eeprom_tx_buf[4] = eeprom_tx_buf[0];
  	           eeprom_tx_buf[5] = eeprom_tx_buf[1];
  	           eeprom_tx_buf[6] = eeprom_tx_buf[2];
  	           eeprom_tx_buf[7] = eeprom_tx_buf[3]; 
  	           
  	           if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
  	           Write_EEPROM(EEP_WEIFULL2_ADDR, eeprom_tx_buf, 8);
  	           else
  	           Write_EEPROM(EEP_WEIFULL1_ADDR, eeprom_tx_buf, 8);
  	           
               point2_cal_step++;
							 tmp = 0;
							 //point2_cal_ok    = TRUE;
        }
      }
		else if(USER_CAL_OK_FLAG == point2_cal_step)
       {     
			 
			 }
		else if(USER_CAL_OVER == point2_cal_step) 
		   {
		    point2_cal_start = FALSE;	
		    current_mode     = STAT_WEIGH;  //返回到称重模式
	      //此处重新设定温度比较点
		    temp_cmp_point   = temp_float;
		   }
		else
		{;}			
	   		 
 }

void  point2_cal_proc_inner(Uint32 weight_tmp)
   {
    Uint32 tmp;
    Uint32 i;
    Uint8  eeprom_tx_buf[8];
    
		if(USER_CAL_ZER0_FLASH==point2_cal_step)
		{
		 if((TRUE == stable_flag)&&(weight_tmp>ZER0_CODE_MIN)&&(weight_tmp<ZER0_CODE_MAX))
		    {
					     zer0_data = weight_tmp;
               tare_data = 0;
					     motor_err_flag = motor_run_proc(LOAD_POSITION);
               if(1 == motor_err_flag)
                {
                 End_Cal_Pro();
                }
               else
                {
                 point2_cal_step++;
                 stable_manual_break();
                }             
		     }
		}		
		else if(USER_CAL_FULL_FLASH == point2_cal_step) //
		{
		  if(weight_tmp>(zer0_data + 10000000))
			    point2_cal_step++;	
		}
		else if(USER_CAL_FULL == point2_cal_step)
       {     
				    if((TRUE==stable_flag)&&(weight_tmp>INNCAL_CODE_MIN)&&(weight_tmp<INNCAL_CODE_MAX))
							{    
               //
               tmp = weight_tmp - zer0_data;
							 /////////////////////////////////////////////////
								
               weigh_coef1     =  inner_real_g.rec_float /(tmp+0.5);		
							 full_code       =  full_range / weigh_coef1;               
							 inner_code_step =  full_code / system_division;
 								
               ///////////////////////////////////////////////////////////////////   
    	         eeprom_tx_buf[0] = (Uint8)(full_code>>24);
  	           eeprom_tx_buf[1] = (Uint8)((full_code>>16)&0xff);
  	           eeprom_tx_buf[2] = (Uint8)((full_code>>8)&0xff);
  	           eeprom_tx_buf[3] = (Uint8)(full_code&0xff);
  	            
  	           eeprom_tx_buf[4] = eeprom_tx_buf[0];
  	           eeprom_tx_buf[5] = eeprom_tx_buf[1];
  	           eeprom_tx_buf[6] = eeprom_tx_buf[2];
  	           eeprom_tx_buf[7] = eeprom_tx_buf[3]; 
						  
  	           if(DOUBLE_DIVISION&&(WAN10_DIVISION==dot_position))
  	           Write_EEPROM(EEP_WEIFULL2_ADDR, eeprom_tx_buf, 8);
  	           else
  	           Write_EEPROM(EEP_WEIFULL1_ADDR, eeprom_tx_buf, 8);
  	           
               point2_cal_step++;
        }
      }
		else if(USER_CAL_OK_FLAG == point2_cal_step)
       { //卸载砝码    
			   motor_err_flag = motor_run_proc(UNLOAD_POSITION);
         if(1 == motor_err_flag)
            {
             End_Cal_Pro();
            }
         else
            {
             point2_cal_step++;
            }             
			 }
		else if(USER_CAL_OVER == point2_cal_step) 
		   {
		    point2_cal_start = FALSE;	
		    current_mode     = STAT_WEIGH;  //返回到称重模式
	      //此处重新设定温度比较点
		    temp_cmp_point   = temp_float;
		   }
		else
		{;}			 
	}

////////////////////////////////////
//仅仅对显示值修正 修正原则 200.0000
//偏离 <2 回0
//偏离 <3 回2
//偏离 <5 回3
/////////////////////////////////
static  float display_to_int(float net_tmp)
 {
  Uint32 tmp,tmp2;
	if((WAN_DIVISION==dot_position)&&(1==int_func))
	{
    	tmp  = (Uint32)(net_tmp * 10000);
      tmp  = tmp % 10000;
      if((tmp<2)||(tmp>9998))
       {
        return(net_tmp);
       } 
      else if(tmp<4)
       {
        return(net_tmp-0.0002);
       }
      else if(tmp<6)
       {
        return(net_tmp-0.0003);
       }
      else if(tmp>9996)
       {
        return(net_tmp+0.0002);
       }
      else if(tmp>9994)
       {
        return(net_tmp+0.0003);
       }
      else 
        return(net_tmp);
     }
	/*
  else if(WAN10_DIVISION == dot_position) 
     {
      tmp  = (Uint32)(net_tmp * 100000);
      tmp  = tmp % 10000;
      if((tmp<2)||(tmp>9998))
       {
        return(net_tmp);
       } 
      else if(tmp<4)
       {
        return(net_tmp-0.00002);
       }
      else if(tmp<6)
       {
        return(net_tmp-0.00003);
       }
      else if(tmp>9996)
       {
        return(net_tmp+0.00002);
       }
      else if(tmp>9994)
       {
        return(net_tmp+0.00003);
       }
      else 
        return(net_tmp);
     }*/
  else
        return(net_tmp);   
 }
//////////////////////////////////////////////////////////
//2?? ??? ???
//(1) ??????0?? ?????
//(2) ?????? ???? ???? ??????????
//(3) ??????????
//(4) ??????????
//(5) ????
//(6) ???? ?? ??
//////////////////////////////////////////////////////////
static Uint32 manual_zer0_data;

void    update_new_data(void)
	{
	 float  net_tmp;
	 Uint32 weight1,weight2,weight3,weight_tmp;
	 	 //////////////////////////////////////////////////	  
     weight1 = Get_Filter_Data();                //
     // //线性校准 使用的是最原始的数据
     if((TRUE==point10_cal_start)&&(TRUE==stable_flag))  
         //point4_cal_proc(weight1);
  		     point10_cal_proc(weight1);
		 
		 //线性校准OK  立即生效 用户校准使用的是线性校准后 的数据
		 weight_tmp = weight1;
     if((TRUE==point10_cal_ok)&&(TRUE==line_cal_en))  //线性校准使能
       //weight_tmp = Get_Line4_data(weight1);  
         weight_tmp = Get_Line10_data(weight1);
		 //用户校准 使用线性校准后的数据     
     ///////////////////////////////////////////
     if(TRUE==point2_cal_start)             
         {
          if(STAT_CAL_INNER == current_mode) //内校准
            point2_cal_proc_inner(weight_tmp);
          else 
            point2_cal_proc(weight_tmp);     //外校准 
         }	
			
     new_data_big = weight_tmp;              //含零点的重量内码    				 
     /////////////////////////////////////////去皮状态下不允许自动跟踪 
     if((1==flag_auto_track_enable)&&(0==tare_data))
        auto_zer0(weight_tmp);              //修改zer0_data
     
     //////////////////////////////////////////开机置0功能
     if((TRUE == power_on_flag)&&(TRUE==stable_flag))
     {
        power_on_clr_cnt--;
        if(0 == power_on_clr_cnt)
				 {
					power_on_flag = FALSE;
		 	    if(abs(weight_tmp-zer0_data) < (full_code * START_ZER0_MAX /100)) //开机置0 范围
             {
              zer0_data = weight_tmp;          //??????
             }
           /////////////////////////////
           manual_zer0_data = zer0_data;      //??????????,???????????             
          } 
      }    
     //////////////////////////////////////////////////////////	
		 ///////////////////////////////////////手动置0
     //if((TRUE == do_zer0_flag)&&(TRUE==stable_flag))
		 if(TRUE == do_zer0_flag)	 
      {
       do_zer0_flag = FALSE;
       if(abs(weight_tmp-manual_zer0_data)< (full_code * USER_ZER0_MAX /100)) 
          {
           zer0_data = weight_tmp;
           tare_data = 0;
          }                   
      }       
     ////////////////////////////////正向去皮处理 去皮后自动跟踪失效
     //if((TRUE == do_tare_flag)&&(weight_tmp>zer0_data)&&(TRUE==stable_flag))
     if((TRUE == do_tare_flag)&&(weight_tmp>zer0_data))
  		{
       do_tare_flag = FALSE;
       tare_data  = new_data;
      }
  
     weight2 = abs(weight_tmp - zer0_data);    //得到毛重 内码
     //first load track
			
		 new_data = auto_loadtrack(weight2); 
		
			/*	
		 if(1 == flag_load_track_enable)
      new_data = auto_repetioncheck(weight2);    //稳定跟踪功能
     else
      new_data = weight2; 
		*/
     ///////////////////////////////
		 if(TRUE == point2_cal_ok)               //???????
      {
		    gross_weight  = weigh_coef1  * new_data;    //毛重
        tare_weight   = weigh_coef1  * tare_data;   //去皮量   
    
        if(weight_tmp < zer0_data) 
          {
           positive_flag  = FALSE;
           net_tmp     = gross_weight + tare_weight ;
          }
        else
          {
           if(gross_weight < tare_weight)
             {
              positive_flag = FALSE;
              net_tmp     = tare_weight - gross_weight ;
             }
           else        
             {
              positive_flag = TRUE;
              net_tmp = gross_weight - tare_weight  ;
             }
          }
         ////////////////////////////////////整数位 处理
				 if(QIAN_DIVISION == dot_position)	
				  net_tmp += 0.0005;
         else
					net_tmp += 0.00005;
			
				 net_weight = display_to_int(net_tmp);
					
				 ////////////////////////////////////重复性检查 处理 暂时由 稳定跟踪代替
				 //net_weight = repeat_funct_proc(net_weight);	
					
         if(net_weight < display_min_value)
            net_weight = 0.0;
				 /////////////////////////////////////////////根据净重 和毛重得到是否溢出
				 if((net_weight>weight_alarm)||(gross_weight>weight_alarm))
            full_flag = TRUE;
				 else
					  full_flag = FALSE;
       }
    else
      {
       gross_weight   = new_data;  
       net_weight     = new_data;
      }
    /////////////////////////////////////over
   }   

///////////////////////////////////////////////////////////
//20 ???????????
//???????
//??0:  ok
//    1:  not stable
//    2:  ee2prom fault
///////////////////////////////////////////////////////////
void  Save_Rec(void)
    {
     un_rec_data rec_data;
     Uint16 i;
     Uint8  check_data;
     Uint8  Send_Data[16];
     if((FALSE == positive_flag)||(FALSE == stable_flag))
     {
      //未保存提示
     }
     else
     { 
			 Send_Data[0] = CHECK_DATA;
       Send_Data[1] = CHECK_DATA;
       Send_Data[2] = 0; //时间
       Send_Data[3] = 0;
       Send_Data[4] = 0;
       Send_Data[5] = 0;
       Send_Data[6] = 0;
       Send_Data[7] = current_mode;
			 Send_Data[8] = current_unit;
       switch(current_mode)
        {
         case STAT_PCS:
              Send_Data[9]  = (pcs_number>>24)&0xff;
              Send_Data[10] = (pcs_number>>16)&0xff;
              Send_Data[11] =  (pcs_number>>8)&0xff;
              Send_Data[12] =       pcs_number&0xff;
              break;
         case STAT_100:
              rec_data.rec_float = number_100;
              Send_Data[9]  = (rec_data.rec_uint32>>24)&0xff;
              Send_Data[10] = (rec_data.rec_uint32>>16)&0xff;
              Send_Data[11] =  (rec_data.rec_uint32>>8)&0xff;
              Send_Data[12] =       rec_data.rec_uint32&0xff;
              break;
         default:
              rec_data.rec_float = net_weight;
              Send_Data[9]  = (rec_data.rec_uint32>>24)&0xff;
              Send_Data[10] = (rec_data.rec_uint32>>16)&0xff;
              Send_Data[11] =  (rec_data.rec_uint32>>8)&0xff;
              Send_Data[12] =       rec_data.rec_uint32&0xff;
              break;
         }
       ////////////////////////USER ID 此项目为0
       Send_Data[13] = 0; // (user_id>>8)&0xff;
       Send_Data[14] = 0; //  user_id&0xff;
       
       ////////////////////////??13???? ???
       check_data = 0;
       for(i=0;i<15;i++)
       check_data += Send_Data[i];
       
       Send_Data[15] = check_data; 
       //save one rec
       Write_EEPROM(EEP_REC_START_ADDR+rec_index*EEP_BYTE_PER_REC,Send_Data, 16); 
	     for(i=0;i<20000;i++)	{;}	         //delay
	  
	     for(i=0;i<16;i++) Send_Data[i] = 0;	 //clear buf
       //readback
	     Read_EEPROM(EEP_REC_START_ADDR+rec_index*EEP_BYTE_PER_REC,Send_Data, 16);	 
	     for(i=0;i<1000;i++)	{;}	         //delay
       check_data = 0;
       for(i=0;i<15;i++)
       check_data += Send_Data[i];
       if(Send_Data[15] == check_data)
       {
        rec_total_num++;               //总记录数
      
        rec_index++;             //????
        if(MAX_REC_NUM == rec_index)
           rec_index = 0;
                                       
        Send_Data[0] = (rec_total_num>>24)&0xff;
        Send_Data[1] = (rec_total_num>>16)&0xff;
        Send_Data[2] =  (rec_total_num>>8)&0xff;
        Send_Data[3] =       rec_total_num&0xff;
        Send_Data[4] = Send_Data[0];
        Send_Data[5] = Send_Data[1];
        Send_Data[6] = Send_Data[2];
        Send_Data[7] = Send_Data[3];
        Write_EEPROM(EEP_REC_TOTAL_ADDR,Send_Data, 8); 
        for(i=0;i<20000;i++)	{;}	
        //写入 成功
			  display_flag_mp = TRUE;
       }
       else
       {//写入 失败
        //Log_Alm(&tips_EE_bad,ALM_LEV_TIPS);
       }
    }
  }  
//////////////////////////////////////////////////////////////////
//恢复出厂设置项目 ：
//称量单位 温度单位 分辨率选择 记录数据擦除
///////////////////////////////////////////////
void  Res_factory_pro(void)
{
     Uint8 buf[16];
     Uint32 i;
	   ////////////////////////////温度摄氏度
	
	   //称量模式
	   current_mode = STAT_WEIGH;
	
	   //称重单位
	   //温度单位
   	 //分辨率0.1显示
	   //删除称重记录数据
	   current_unit     = UNIT_G; 
	   temp_unit        = TEMP_UNIT_OC;
	   del_last_one     = 0;
	
	   buf[4]  = current_unit;
	   buf[5]  = 0;
	   buf[6]  = buf[5] + buf[4];
	   buf[7]  = CHECK_DATA;
	   buf[8]  = temp_unit;
	   buf[9]  = 0;
	   buf[10] = buf[9] + buf[8];
	   buf[11] = CHECK_DATA;
	   buf[12] = del_last_one;
	   buf[13] = 0;
	   buf[14] = buf[13] + buf[12];
	   buf[15] = CHECK_DATA;
	   Write_EEPROM(EEP_USR_FUN1_ADDR,buf, 16); 
	   for(i=0;i<1234567;i++)  //延时
		 ///////////////////////////////////////////////////////////
		 uart_format_type = UART_TYPE_DEF; 	
     buf[0] = uart_format_type;
	   buf[1] = CHECK_DATA;
		 buf[2] = buf[0] + buf[1];
		 buf[3] = CHECK_DATA;
		 
		 Uart_Printer_Flag = 1; 	
     buf[4] = Uart_Printer_Flag;
	   buf[5] = CHECK_DATA;
		 buf[6] = buf[4] + buf[5];
		 buf[7] = CHECK_DATA;
		 Write_EEPROM(EEP_USR_FUN5_ADDR,buf, 8); 
		 
		 USART_Configuration();
     //调用删除记录
     Del_rec_all();		 
}	

//////////////////////////////////////////////////////////////////
#define    P_air         0.0012     //定义空气密度
#define    C_f_comp      0.99985    //定义空气浮力校准因子
#define    V_stand       1        
//////////////////////////////////////////////////////////////////
static float SOLID_v;
void Cali_density_pro(float w1,float w2)
{   //根据第一次和第二次测量的重量 计算密度
 if(1==density_mode_index)
	 {//固体密度计算公式 (w1/(w1-w2))*(p0-Pk) + p1 //p0:辅助液体密度 p1空气密度 为常数
    density_data = (w1/(w1-w2))*(DENSITY_STAND_LIQUID[liquid_sel_index]-P_air) + P_air; //固体密度
		SOLID_v      = ((w1-w2)/(DENSITY_STAND_LIQUID[liquid_sel_index]-P_air)) * C_f_comp; //固体体积
   } 
 else if(2==density_mode_index)
   {//液体密度计算公式 ((w1-w2)/V)*a + p1 //a:空气浮力校准因子为常数. V:下垂体体积
		//已知 下垂体密度 重量 可以计算出该固体的体积
		density_data = ((w1-w2)/(w1/DENSITY_STAND_SOLID[solid_sel_index]))*C_f_comp + P_air; 
   }
 else 
    density_data = 88888.88;	 
}	
 