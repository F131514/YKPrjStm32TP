/*
**********************************************************************************************************
* File           : ad7843.h
* Programmer(s)  : Wangjg	
***********************************************************************************************************
*/
#ifndef  _KEYBOARD_H
#define  _KEYBOARD_H


#define KEY_NUM        12        //��������


#define KEY_PRESS_TIME          (100/15)
#define KEY_2S_TIME            (2000/15)
#define KEY_5S_TIME            (5000/15)

#define KEY_PRESSED       0x10
#define KEY_PRESSED_2S    0x20
#define KEY_PRESSED_5S    0x30
//4�� ���ϼ�
#define KEY_UP               2    //�������(��ű�ʾɨ��˳�򣬱�Ŵ����ɨ��)
#define KEY_SAVE             2
   
#define KEY_DOWN             5
#define KEY_READ             5    //�������(��ű�ʾɨ��˳�򣬱�Ŵ����ɨ��)

#define KEY_OK               7
#define KEY_CAL              7    //�������(��ű�ʾɨ��˳�򣬱�Ŵ����ɨ��)

#define KEY_ESC              1
#define KEY_TARE             1    //�������(��ű�ʾɨ��˳�򣬱�Ŵ����ɨ��)

#define KEY_ZER0             3    //�������(��ű�ʾɨ��˳�򣬱�Ŵ����ɨ��)
#define KEY_PCS              4
#define KEY_CFG              6
#define KEY_PRT              8

#define KEY_SLEEP            0

/*
**************************************************************************************************************
*                                           TYPE DEFINITIONS OF DATA TYPES
**************************************************************************************************************
*/

typedef  unsigned int    INT32U;
typedef  signed   int    INT32S;
typedef  unsigned short  INT16U;
typedef  signed   short  INT16S;
typedef  uint8   INT08U;
typedef  signed   char   INT08S;
typedef  unsigned  char  BOOLEAN; 
#endif

/*
**************************************************************************************************************
*                                           FUNCTION DECLARATION
**************************************************************************************************************
*/

void    Key_Pin_Init(void);

void    Scan_Key(void);
uint8   Get_Key_Code(void);

//////////////////////////////END OF THE FILE

