//**************************************************************************************/
//  �ļ���   AD7843.C
//  ����     ��ADS7843���ж�д�������ṩ��
//  ����ӿڣ�1��������������
//          ��2�������˲�����
//          ��3��������Ļ2���Խǵ�
//          ��4��������ʱ
//  ����ӿڣ�1�����X Y �����AD������ֵ
//          ��2����������Ļ��X Y��ֵ
//  ���ߣ�   ������
//  �汾�ţ� V1.0
//  ���ڣ�   2010.09.25
/***************************************************************************************/
#include  "global.h"
#include  "lpc2400.h"
#include  "keyboard.h"

#define SCAN_NO    FIO1SET=(1<<4)+(1<<5)+(1<<6)

#define SCAN_A     FIO1SET=(1<<5)+(1<<6);FIO1CLR=(1<<4)
#define SCAN_B     FIO1SET=(1<<4)+(1<<6);FIO1CLR=(1<<5)
#define SCAN_C     FIO1SET=(1<<4)+(1<<5);FIO1CLR=(1<<6)


static volatile uint16 key_press,key_press_2s,key_press_5s;    //ÿһλ��Ӧһ������(Ϊ1��ʾ��Ӧ�¼�����)

void Key_Pin_Init(void) 
{
 //p1.456 ɨ��� ���������
 //P1.0123 �����
 
 PINSEL2 &= 0xffffc000;
 
 FIO1DIR &= 0xfffffff0;
 FIO1DIR |= 0x00000070;
 
 SCAN_NO;
}

//**********************************************************************
//����  ��:Scan_Key
//��������:��ʱ��ɨ�����,����¼ɨ����(�����ж���2msɨ��һ��)
//��ڲ���:��
//���ڲ���:��
//˵    ��:ÿ2msɨ��2����,ɨ������Ϊ14ms
//**********************************************************************
void Scan_Key(void)
{
    static uint8   scan_addr=0;               //��¼ɨ��ڼ���
    static uint16  key_press_time[12];    //��¼ÿ���������µ�ʱ��
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
    if(3 == scan_addr)    //���а�����ɨ���һ��(��Ҫ15ms)
    {
    
     ////////////////////�õ�ɨ��һ�εļ�������
        for (i=0; i<12; i++)
        {
            if(scan_data & (1 << i))
            {//ĳ��δ����,�ۼ�ʱ������
                key_press_time[i] = 0;
            }
            if(key_press_time[i] == KEY_PRESS_TIME)
            {//ĳ��������ȥ(����50ms)
                key_press |= 1 << i;
            }
            if(key_press_time[i]==KEY_2S_TIME)
            {//ĳ��һֱ����2s
                key_press_2s |= 1 << i;
            }
            if(key_press_time[i]==KEY_5S_TIME)
            {//ĳ��һֱ����5s
                key_press_5s |= 1 << i;
            }
            key_press_time[i]++;    //�ۼ�ÿ���������µ�ʱ��(ÿ��ɨ�������Ҫ15ms)            
        }
        scan_data  = 0xffffffff;
        scan_addr = 0; 
   }
}


/****************************************************************************
* ���ƣ�Get_KeyCode()
* ���ܣ�������
* ��ڲ�������
* ���ڲ�������4λ����(0~15), ��4λ����(0 δ��,1 ����,2 ����2s, 3 ����5s)
****************************************************************************/
 uint8 Get_Key_Code(void)
{
 uint8 i;
    for(i=0; i<12; i++)
    {
        if(key_press & (1 << i))
        {
            key_press = 0;        //���ֵ
            return(i | KEY_PRESSED);
        }
        if(key_press_2s & (1 << i))
        {
            key_press_2s = 0;    //���ֵ
            return(i | KEY_PRESSED_2S);
        }
        if(key_press_5s & (1 << i))
        {
            key_press_5s = 0;    //���ֵ
            return(i | KEY_PRESSED_5S);
        }
    }
    return(0);
}

///////////////////////////////////////END OF THE FILE
