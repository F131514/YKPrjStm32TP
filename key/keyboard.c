//**************************************************************************************/
//  文件名   AD7843.C
//  功能     对ADS7843进行读写操作，提供的
//  输入接口（1）设置引脚连接
//          （2）设置滤波参数
//          （3）设置屏幕2个对角点
//          （4）设置延时
//  输出接口（1）输出X Y 坐标的AD采样数值
//          （2）输出相对屏幕的X Y数值
//  作者：   王建光
//  版本号： V1.0
//  日期：   2010.09.25
/***************************************************************************************/
#include  "global.h"
#include  "lpc2400.h"
#include  "keyboard.h"

#define SCAN_NO    FIO1SET=(1<<4)+(1<<5)+(1<<6)

#define SCAN_A     FIO1SET=(1<<5)+(1<<6);FIO1CLR=(1<<4)
#define SCAN_B     FIO1SET=(1<<4)+(1<<6);FIO1CLR=(1<<5)
#define SCAN_C     FIO1SET=(1<<4)+(1<<5);FIO1CLR=(1<<6)


static volatile uint16 key_press,key_press_2s,key_press_5s;    //每一位对应一个按键(为1表示对应事件发生)

void Key_Pin_Init(void) 
{
 //p1.456 扫描脚 即：输出脚
 //P1.0123 输入脚
 
 PINSEL2 &= 0xffffc000;
 
 FIO1DIR &= 0xfffffff0;
 FIO1DIR |= 0x00000070;
 
 SCAN_NO;
}

//**********************************************************************
//函数  名:Scan_Key
//函数功能:分时段扫描键盘,并记录扫描结果(放在中断中2ms扫描一次)
//入口参数:无
//出口参数:无
//说    明:每2ms扫描2个键,扫描周期为14ms
//**********************************************************************
void Scan_Key(void)
{
    static uint8   scan_addr=0;               //记录扫描第几次
    static uint16  key_press_time[12];    //记录每个按键按下的时间
    static uint32  scan_data=0xffffffff;
           uint8   i;
    
    if(0==scan_addr)    
       {
        SCAN_A;
       } 
    else if(1==scan_addr)  
       {
        SCAN_B;
       } 
    else if(2==scan_addr)
       { 
        SCAN_C;
       }
    else
    {
     scan_addr = 0;
     scan_data = 0xffffffff;
    }  
    /////////////////////////////////////////////// 
    scan_data = (scan_data<<4) | (FIO1PIN&0x0f);
    scan_addr++;
    if(3 == scan_addr)    //所有按键被扫描过一次(需要15ms)
    {
    
     ////////////////////得到扫描一次的键盘数据
        for (i=0; i<12; i++)
        {
            if(scan_data & (1 << i))
            {//某键未按下,累加时间清零
                key_press_time[i] = 0;
            }
            if(key_press_time[i] == KEY_PRESS_TIME)
            {//某键按了下去(保持50ms)
                key_press |= 1 << i;
            }
            if(key_press_time[i]==KEY_2S_TIME)
            {//某键一直按了2s
                key_press_2s |= 1 << i;
            }
            if(key_press_time[i]==KEY_5S_TIME)
            {//某键一直按了5s
                key_press_5s |= 1 << i;
            }
            key_press_time[i]++;    //累加每个按键按下的时间(每次扫描完成需要15ms)            
        }
        scan_data  = 0xffffffff;
        scan_addr = 0; 
   }
}


/****************************************************************************
* 名称：Get_KeyCode()
* 功能：读键码
* 入口参数：无
* 出口参数：低4位键码(0~15), 高4位动作(0 未按,1 按下,2 按下2s, 3 按下5s)
****************************************************************************/
 uint8 Get_Key_Code(void)
{
 uint8 i;
    for(i=0; i<12; i++)
    {
        if(key_press & (1 << i))
        {
            key_press = 0;        //清键值
            return(i | KEY_PRESSED);
        }
        if(key_press_2s & (1 << i))
        {
            key_press_2s = 0;    //清键值
            return(i | KEY_PRESSED_2S);
        }
        if(key_press_5s & (1 << i))
        {
            key_press_5s = 0;    //清键值
            return(i | KEY_PRESSED_5S);
        }
    }
    return(0);
}

///////////////////////////////////////END OF THE FILE
