#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "stm32f0xx.h"
#include   "define.h"

#define SOFT_VERSION               1.0 

//用于打印或 其它场合
#define      CHINESE                 0
#define      ENGLISH                 1
//是否内校准版本
#define INNER_CAL_VERSION            0  //???????

#define DOUBLE_DIVISION              0  //双分度版本

#define  DEFAULT_MACHINE_FULL      0xc8 //200.0000     
#define  DEFAULT_MACHINE_DIV       0x01
#define  DEFAULT_MACHINE_ID        ((DEFAULT_MACHINE_FULL<<8)+DEFAULT_MACHINE_DIV)
#define  DEFAULT_MACHINE_LOGO      0x00  //不显示LOGO 型号
#define  DEFAULT_MACHINE_LANG      0x00  //CHINESE
#define  DEFAULT_MACHINE_REP       0x01  //开启 重复性检查
#define  DEFAULT_MACHINE_INT       0x01  //开启 整数位跟踪
#define  DEFAULT_MACHINE_DISDIV    0x00  //关闭

////开机预热时间
#define    PREHOT_TIME_CNT         1800
//#define      PREHOT_TIME_CNT        20
//进入休眠时间
#define    SLEEP_TIME_CNT         1800
//#define    SLEEP_TIME_CNT         60

#define  QIAN_DIVISION             0
#define   WAN_DIVISION             1
#define WAN10_DIVISION             2

////////////////////////////////////////////
#define   INNER_CODE_STEP_MAX      50
#define   INNER_CODE_STEP_MIN       5
#define   INNER_CODE_STEP_DEF      18

#define   ZER0_CODE_MIN            3000000
#define   ZER0_CODE_MAX            12000000
#define   USERCAL_CODE_MIN         15000000
#define   USERCAL_CODE_MAX         60000000

/////增加内校砝码重量
#define   INNCAL_CODE_MIN          10000000
#define   INNCAL_CODE_MAX          60000000


#define   INNER_CODE_STEP10        (2000000*15)/13    //??10?????????
///////////////////////////////////////////////////////
//?????????
///////////////////////////////////////////////////////

#define MACHINE_NORMAL_MODE        0    //??????
#define MACHINE_FACTORY_MODE       1    //????

//
#define MODE_WEIGH              0x01
#define MODE_PCS                0x02  
#define MODE_DENSITY            0x04
#define MODE_CHECK              0x08
#define MODE_100                0x10
#define MODE_ANIMAL             0x20


///////////////////////////////////////////////////////
//?????????????
//////////////////////////////////////////////////////
#define STAT_CONFIG2            0x00

#define STAT_CONFIG             0x01
#define STAT_CAL_LINE           0x02
#define STAT_CAL                0x03
#define STAT_CAL_INNER          0x04
#define STAT_DISP_REC           0x05

#define STAT_START              0x06
#define STAT_WEIGH              0x07
#define STAT_PCS                0x08  
#define STAT_DENSITY            0x09
#define STAT_CHECK              0x0A
#define STAT_100                0x0B
#define STAT_ANIMAL             0x0C


#define STAT_END                0x0D
///////////////////////////////////////工厂模式下增加内码显示
#define STAT_INNER              0x10

///////////////////////////////////////////////////////
//用户模式下的几个配置项
#define MENU_CONFIG_MODE        0x01
#define MENU_CONFIG_UNIT        0x02
#define MENU_CONFIG_TEMP        0x03
#define MENU_CONFIG_DIV         0x04
//波特率选择
#define MENU_CONFIG_BAUD        0x05

#define MENU_CONFIG_DEL         0x06
#define MENU_CONFIG_DEF         0x07
#define MENU_CONFIG_COMM        0x08

#define MENU_CONFIG_END         0x09
//////////////////////////////////////////////////////
//工厂模式下的几个配置项
#define MENU_F_CONFIG_FULL      0x00 //满量程
#define MENU_F_CONFIG_DIV       0x01 //分辨率
#define MENU_F_CONFIG_LOGO      0x02 //LOGO
#define MENU_F_CONFIG_LANG      0x03 //语言

#define MENU_F_CONFIG_REP       0x04 //重复性检查
#define MENU_F_CONFIG_INT       0x05 //整数位取整
#define MENU_F_CONFIG_LINE      0x06 //线性校准作用
#define MENU_F_CONFIG_LOCK      0x07 //锁字功能 
#define MENU_F_CONFIG_ASD       0x08 //稳定参数设置
#define MENU_F_CONFIG_REALKG    0x09 //输入真实砝码重量
#define MENU_F_CONFIG_DISDIV    0x0a //显示分度数切换
#define MENU_F_CONFIG_TESTKEY   0x0b //测试按键
#define MENU_F_CONFIG_END       0x0c // END


//////////////////////////////////////////////////////

#define UNIT_NULL               0x00
#define UNIT_G                  0x01
#define UNIT_MG                 0x02
#define UNIT_CT                 0x03  
#define UNIT_OZ                 0x04
#define UNIT_OZT                0x05
#define UNIT_END                0x06

//////////////////////////////////////////////////////

#define UART_TYPE_DEF           0x01
#define UART_TYPE_AUTO_F1_1200  0x01
#define UART_TYPE_AUTO_F1_2400  0x02
#define UART_TYPE_AUTO_F1_4800  0x03
#define UART_TYPE_AUTO_F1_9600  0x04
#define UART_TYPE_AUTO_F2_1200  0x05
#define UART_TYPE_AUTO_F2_2400  0x06
#define UART_TYPE_AUTO_F2_4800  0x07
#define UART_TYPE_AUTO_F2_9600  0x08

#define UART_TYPE_MANU_F1_1200  0x09
#define UART_TYPE_MANU_F1_2400  0x0A
#define UART_TYPE_MANU_F1_4800  0x0B
#define UART_TYPE_MANU_F1_9600  0x0C
#define UART_TYPE_MANU_F2_1200  0x0D
#define UART_TYPE_MANU_F2_2400  0x0E
#define UART_TYPE_MANU_F2_4800  0x0F
#define UART_TYPE_MANU_F2_9600  0x10

//#define UART_TYPE_END           0x11
#define UART_TYPE_END            0x05   //4种波特率

///////////////////????? ??
#define   CAL_POINT_NUM            11

//????????
#define   USER_CAL_ZER0_FLASH      1
#define   USER_CAL_FULL_FLASH      2
#define   USER_CAL_FULL            3
#define   USER_CAL_OK_FLAG         4
#define   USER_CAL_OVER            5
#define   USER_CAL_ER              6    //此处状态无法到达
//

//#define   WEIGHT_TIP_VALUE        200.0090
//#define   WEIGHT_ALM_VALUE        205.0090
///开机置零范围 10% 
//手动置零范围   2%
#define   START_ZER0_MAX           10  //????????  
#define    USER_ZER0_MAX           2  //????????

//单位转换常量
#define    G_TO_MG                1000
#define    G_TO_CT                5           //  克拉
#define    G_TO_OZ                0.035273962 //  盎司
#define    G_TO_OZT               0.032150747 //金盎司

//上电稳定时间
#define   POWER_ON_STABLE_TIMES    5   //开机稳定后多长时间内都允许置0

//PCS 相关宏
#define    PCS_ADJUST_COEF        0.5  //???????
#define    WPP_MAX                100  //(????)
#define    WPP_MIN              0.001  //??????
#define    PCS_MAX              99999  //??????

//检重范围
#define    CHECK_LIMIT_MAX        200   //????
#define    CHECK_LIMIT_MIN       0.01
//百分比基准点的选择范围
#define    COMPARE_100_MAX        200
#define    COMPARE_100_MIN        0.1

//动物称重最大时间
#define    ANIMAL_TIME_MAX         20

//////////////////////////////////////
//????????39.5
//温度相关宏定义
#define   TEMP_SENSOR_NC_VALUE    99.9 
#define   TEMP_MIN_RANGE          0  //超温报警
#define   TEMP_MAX_RANGE          90 //超温报警
#define   TEMP_WAR_RANGE          5  //温度变化量报警

#define   TEMP_UNIT_OC            0x00
#define   TEMP_UNIT_OF            0x01

//????
#define   CHECK_DATA              0xAA

///////////////////////////////////////////////////////////////////
//
typedef union    
{
   float  rec_float;    //
   Uint32 rec_uint32;
}un_rec_data;

///////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////
 //显示相关变量  
 extern Uint8      display_buffer[14],full_flag;    //显存
 extern Uint8      flag_LOGO,flag_language,Speaker_Time;  
 extern Uint8      repetion_func,int_func,line_cal_en,lock_func_en,asd_para,disdiv_para;
 extern un_rec_data inner_real_g;

 extern Uint8      sleepmode;             //睡眠模式
 extern Uint16     prehot_time,no_key_time;            //进入睡眠计时器

 extern Uint8      machine_mode,current_mode,current_unit,del_last_one;
 extern Uint8      done_ok_flag,current_menu,current_mode_bak;    //增加配置菜单变量
 extern Uint8      stable_flag;     //????
 
 extern Uint8      power_on_flag,power_on_clr_cnt; //开机上电需要做的一些事情
 
 extern Uint8      do_zer0_flag,do_tare_flag,do_sam100_flag;
 
 extern Uint32     clr_data,zer0_data,full_code,new_data,new_data_big,tare_data; //
 
 extern Uint8      positive_flag,flag_new_data;
 extern Uint8      asd_var,lnt_var,prt_mod;  //

 extern Uint8      point10_cal_start,point10_cal_step,point10_cal_ok; 
 extern Uint32     Point10_Cal_Buf[CAL_POINT_NUM+1];
 extern float      percent_10_data,point10_k[CAL_POINT_NUM];
 
 extern Uint32     offset_polar;
 extern Uint32     offset_50g,offset_100g,offset_150g;
 extern Uint32     expected_100g,expected_50g,expected_25g;
  
 extern Uint8      point2_cal_start,point2_cal_step,point2_cal_ok;

 extern Uint8      inner_code_step;
 extern Uint8      auto_zer0_valid;   
 
 extern float      weigh_coef1;
 extern float      tare_weight,gross_weight,net_weight;  //
 
 ///////////PCS
 extern float      weight_per_pcs;    //???
 extern Uint32     pcs_number;        //?? 
 extern Uint8      sample_pcs_enter,sample_pcs_index,sample_pcs_ok; //用于指示是否采集了单件重 
 ///////////百分比模式
 extern Uint8      compare_100_flag;
 extern float      weight_compare_100,number_100;        //????????
 ///////////CHECKmode
 extern Uint8       check_valid,check_flag,sample_limit_step;
 extern float       limit1_weight,limit2_weight;          //checkmode
 //动物称重模式
 extern float       animal_data; 
 extern Uint8       animal_time_index,animal_run_time,animal_start; 
 //密度称重模式
 extern Uint8       density_mode_index,density_step,solid_sel_index,liquid_sel_index;
 extern float       density_data;
 
 //温度相关变量
 extern Uint8       temp_cmp_flag,temp_error_flag,temp_unit;
 extern float       temp_float,temp_cmp_point;
 ////////////////////////////??????????
 ////////////////////////////??????????
 extern Uint16      adc_times; 
 
 ///////////////增加记录功能
 extern Uint8       REC_flag,REC_current_mode,REC_current_unit;
 extern Uint8       rec_total_num,rec_index,disp_rec_index;
 extern Uint8       display_flag_mp;
 /////////////机器型号选择产生的变量
 extern Uint8       dot_position;
 extern Uint32      model_id,system_division,inner_code_min,inner_code_max;
 extern float       additional_num,display_min_value,full_range,weight_alarm;
 extern Uint16      line_cal_para, user_cal_para;  //此两个参数 根据满量程来决定
 //内校准
 extern Uint32      motor_run_time,factory_inner_cal_full,user_inner_cal_full;
 extern Uint8       motor_pos_flag,motor_err_flag,point2_cal_flag,Factory_Inncal_step;
 //add printer function
 extern Uint8       print_busy,Uart_Printer_Flag;
 extern Uint8       uart_format_type;
 
 extern Uint8       selemode;



//function declare
extern  Uint8       motor_run_proc(Uint8 target_pos);
extern  void        update_new_data(void);
extern  void        Cali_density_pro(float w1,float w2);

#endif
