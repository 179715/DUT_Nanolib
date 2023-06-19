#ifndef  GUI_Main_H
#define  GUI_Main_H

#include "GUI.h"
#include "DIALOG.h"
#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"
#include "GRAPH.h"
#include "MENU.h"
#include "MULTIPAGE.h"
#include "ICONVIEW.h"
#include "TREEVIEW.h"

/***********************************************************************
*                        GUI������/Ӳ�����б�־λ
************************************************************************
*/
/* GUI���л��� */
#define GUI_OPERATING                          1                           //���л���0Ϊģ������1ΪӲ������

/* GUI���� */
#define GUI_LANGUAGE                           GUI_OPERATING                           //GUI����0ΪӢ�ģ�1Ϊ����

#if GUI_OPERATING > 0u
	/* ͷ�ļ����� */
	#include "JPEGConf.h"
	#include "decode_dma.h"
	#include "GUI_BMPdisplay.h"
	#include "includes.h"
#else
	/* �������������Ͷ��� */
	/* exact-width signed integer types */
	typedef   signed          char int8_t;
	typedef   signed short     int int16_t;
	typedef   signed           int int32_t;

	/* exact-width unsigned integer types */
	typedef unsigned          char uint8_t;
	typedef unsigned short     int uint16_t;
	typedef unsigned           int uint32_t;
#endif
/***********************************************************************
*                        �ⲿ����/��������
************************************************************************
*/
#if GUI_OPERATING > 0u
	/* ϵͳ����ⲿ���� */
	extern void TOUCH_Calibration(uint8_t _PointCount);
	extern void MountSD(void);
	extern void _WriteByte2File(U8 Data, void * p);
#endif

/* GUI����ⲿ���� */
extern void MainTask(void);                      //GUI������
extern void Fun_Delete_DesktopTimer(void);       //ɾ�����涨ʱ��ֹͣ�����ػ�
extern void Fun_Create_DesktopTimer(void);       //�������涨ʱ��,��ʼ���涨���ػ�
/* ����δ����APP ��ʹ��������� */
extern WM_HWIN FUN_APP_Reserved_Create(void);    //Ԥ��APP���ڴ�������

#if GUI_OPERATING > 0u
	/* �ļ�ϵͳ���ȫ�ֱ��� */
	extern FRESULT result;
	extern FIL file;
	extern FIL FileSave;   //ר������ͼƬ�ı���
	extern DIR DirInf;
	extern UINT bw;
	extern FATFS fs;
	extern char DiskPath[4]; /* SD���߼�����·�������̷�0������"0:/" */
	extern const char * FR_Table[];
#endif

/* ϵͳ�����ֿ� */
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ32;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ24;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ16;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ12;

/* ����ͼ���һҳλͼ���� */
extern GUI_CONST_STORAGE GUI_BITMAP bmmycomputer;
extern GUI_CONST_STORAGE GUI_BITMAP bmsetting;
extern GUI_CONST_STORAGE GUI_BITMAP bmfolder;
extern GUI_CONST_STORAGE GUI_BITMAP bmpicture;
extern GUI_CONST_STORAGE GUI_BITMAP bmvideo;
extern GUI_CONST_STORAGE GUI_BITMAP bmcamera;
extern GUI_CONST_STORAGE GUI_BITMAP bmnews;
extern GUI_CONST_STORAGE GUI_BITMAP bmclock;
extern GUI_CONST_STORAGE GUI_BITMAP bmcalendar;
extern GUI_CONST_STORAGE GUI_BITMAP bmmusic;
extern GUI_CONST_STORAGE GUI_BITMAP bmrecorder;
extern GUI_CONST_STORAGE GUI_BITMAP bmradio;
/* ����ͼ��ڶ�ҳλͼ���� */
extern GUI_CONST_STORAGE GUI_BITMAP bmcodedlock;
extern GUI_CONST_STORAGE GUI_BITMAP bmcontrol;
extern GUI_CONST_STORAGE GUI_BITMAP bmdrawingboard;
extern GUI_CONST_STORAGE GUI_BITMAP bmusb;
extern GUI_CONST_STORAGE GUI_BITMAP bmgame;
extern GUI_CONST_STORAGE GUI_BITMAP bmgps;
extern GUI_CONST_STORAGE GUI_BITMAP bmlamplight;
extern GUI_CONST_STORAGE GUI_BITMAP bmoscilloscope;
/* ����ͼ�����ҳλͼ���� */


/***********************************************************************
*                        ϵͳ��־λ����
************************************************************************
*/
/***********************************************************************
*            �ؼ�ID����  (�ؼ�ID = ҳ�����ID + �ؼ�ƫ��ID)
************************************************************************
*/
/*---------------------------------------------------------------------------*/
/*---------------------------  ҳ�����ID��ַ   -----------------------------*/      
/*---------------------------------------------------------------------------*/
#define DESKTOP_BASE_ID             0x0000                             //�������ID
#define KEYPAD_BASE_ID              0x0100                             //��������̻���ID
#define APP_Camera_BASE_ID          0x0200                             //�����APPҳ�����ID
#define APP_Clock_BASE_ID           0x0300                             //����ʱ��APPҳ�����ID
#define APP_Calendar_BASE_ID        0x0400                             //������APPҳ�����ID
#define APP_Music_BASE_ID           0x0500                             //���ֲ���APPҳ�����ID
#define APP_Settings_BASE_ID        0x0600                             //ϵͳ����APPҳ�����ID
#define APP_Picture_BASE_ID         0x0700                             //ͼƬ���APPҳ�����ID
#define APP_DrawingBoard_BASE_ID    0x0b00                             //�滭��APPҳ�����ID
#define APP_Oscilloscope_BASE_ID    0x0c00                             //ʾ����APPҳ�����ID
#define APP_Reserved_BASE_ID        0x0f00                             //Ԥ��APPҳ�����ID

/*---------------------------------------------------------------------------*/
/*-----------------------------  ����ؼ�   ---------------------------------*/
/*---------------------------------------------------------------------------*/
#define ID_DESKTOP_ICONVIEW      (GUI_ID_USER + DESKTOP_BASE_ID + 0x00)               //����ͼ��
#define ID_DESKTOP_System_TEXT   (GUI_ID_USER + DESKTOP_BASE_ID + 0x02)               //ϵͳ����
#define ID_DESKTOP_Date_TEXT     (GUI_ID_USER + DESKTOP_BASE_ID + 0x03)               //ϵͳ����
#define ID_DESKTOP_Time_TEXT     (GUI_ID_USER + DESKTOP_BASE_ID + 0x04)               //ϵͳʱ��

/*---------------------------------------------------------------------------*/
/*------------------------------  ���������  -------------------------------*/
/*---------------------------------------------------------------------------*/
/* ���ּ��̽��� */
#define ID_KEYPAD_FRAMEWIN       (GUI_ID_USER + KEYPAD_BASE_ID + 0x00)
#define ID_KEYPAD_BUTTON0        (GUI_ID_USER + KEYPAD_BASE_ID + 0x01)
#define ID_KEYPAD_BUTTON1        (GUI_ID_USER + KEYPAD_BASE_ID + 0x02)
#define ID_KEYPAD_BUTTON2        (GUI_ID_USER + KEYPAD_BASE_ID + 0x03)
#define ID_KEYPAD_BUTTON3        (GUI_ID_USER + KEYPAD_BASE_ID + 0x04)
#define ID_KEYPAD_BUTTON4        (GUI_ID_USER + KEYPAD_BASE_ID + 0x05)
#define ID_KEYPAD_BUTTON5        (GUI_ID_USER + KEYPAD_BASE_ID + 0x06)
#define ID_KEYPAD_BUTTON6        (GUI_ID_USER + KEYPAD_BASE_ID + 0x07)
#define ID_KEYPAD_BUTTON7        (GUI_ID_USER + KEYPAD_BASE_ID + 0x08)
#define ID_KEYPAD_BUTTON8        (GUI_ID_USER + KEYPAD_BASE_ID + 0x09)
#define ID_KEYPAD_BUTTON9        (GUI_ID_USER + KEYPAD_BASE_ID + 0x0A)
#define ID_KEYPAD_BUTTON10       (GUI_ID_USER + KEYPAD_BASE_ID + 0x0B)
#define ID_KEYPAD_BUTTON11       (GUI_ID_USER + KEYPAD_BASE_ID + 0x0C)
#define ID_KEYPAD_BUTTON12       (GUI_ID_USER + KEYPAD_BASE_ID + 0x0D)
#define ID_KEYPAD_BUTTON13       (GUI_ID_USER + KEYPAD_BASE_ID + 0x0E)

/************************************************************************************************************************
*
*       GUIҳ��Ĭ��ѡ��
*
*************************************************************************************************************************
*/
/*---------------------------------------------------------------------------*/
/*--------------------------  �½�APPĬ������   ----------------------------*/
/*---------------------------------------------------------------------------*/
#define NEW_APP_COORDINATE_X                   0                          //�½�������ʼ����
#define NEW_APP_COORDINATE_Y                   0
#define NEW_APP_WIDTH                          800                        //�½����ڳ���
#define NEW_APP_HIGHT                          480
#define NEW_APP_BAR_HIGHT                      40                         //�½����ڱ������߶�
#define NEW_APP_BAR_COLOR_ENALBE               GUI_BLUE                   //�½����ڱ�������ɫ
#define NEW_APP_BAR_COLOR_UNENALBE             GUI_LIGHTGRAY              //�½����ڱ�������ɫ��δ���
#define NEW_APP_BAR_ALIGN                      GUI_TA_HCENTER|GUI_TA_VCENTER             //�½����ڱ��������뷽ʽ
#define NEW_APP_CLOSEBUTTON                    1                          //���ڹرհ�ť
#define NEW_APP_ACTIVE                         1                          //����Ĭ�ϼ���

#if GUI_LANGUAGE > 0u
	#define NEW_APP_BAR_FONT                       &GUI_FontHZ24              //�½��������壨���ģ�
#else
	#define NEW_APP_BAR_FONT                       GUI_FONT_24B_1             //�½���������
#endif

/*---------------------------------------------------------------------------*/
/*------------------------  �½�APP�Ƿ�ִ��Ĭ��ѡ��  ------------------------*/
/*---------------------------------------------------------------------------*/
#define APP_Clock_WINDOW_DEFAULT                     1                           //����ʱ��app�����Ƿ�Ĭ�ϸ�ʽ�½�
#define APP_Camera_WINDOW_DEFAULT               	 1                           //�����app�����Ƿ�Ĭ�ϸ�ʽ�½�
#define APP_Calender_WINDOW_DEFAULT                  1                           //������app�����Ƿ�Ĭ�ϸ�ʽ�½�
#define APP_Music_WINDOW_DEFAULT                     1                           //���ֲ���app�����Ƿ�Ĭ�ϸ�ʽ�½�
#define APP_Settings_WINDOW_DEFAULT                  1                           //ϵͳ����app�����Ƿ�Ĭ�ϸ�ʽ�½�
#define APP_Picture_WINDOW_DEFAULT                   1                           //ͼƬ���app�����Ƿ�Ĭ�ϸ�ʽ�½�
#define APP_DrawingBoard_WINDOW_DEFAULT              1                           //�滭��app�����Ƿ�Ĭ�ϸ�ʽ�½�
#define APP_Oscilloscope_WINDOW_DEFAULT              1                           //ʾ����app�����Ƿ�Ĭ�ϸ�ʽ�½�
#define APP_Reserved_WINDOW_DEFAULT               	 1                           //Ԥ��app�����Ƿ�Ĭ�ϸ�ʽ�½�
/*---------------------------------------------------------------------------*/
/*--------------------------  �½�����Ĭ������   ----------------------------*/
/*---------------------------------------------------------------------------*/
#define NEW_WINDOW_COORDINATE_X                 200                        //�½�������ʼ����
#define NEW_WINDOW_COORDINATE_Y                 90
#define NEW_WINDOW_WIDTH                        500                        //�½����ڳ���
#define NEW_WINDOW_HIGHT                        300
#define NEW_WINDOW_BAR_HIGHT                    40                         //�½����ڱ������߶�
#define NEW_WINDOW_BAR_COLOR_ENALBE             GUI_BLUE                   //�½����ڱ�������ɫ
#define NEW_WINDOW_BAR_COLOR_UNENALBE           GUI_LIGHTGRAY              //�½����ڱ�������ɫ��δ���
#define NEW_WINDOW_BAR_ALIGN                    GUI_TA_HCENTER|GUI_TA_VCENTER             //�½����ڱ��������뷽ʽ
#define NEW_WINDOW_CLOSEBUTTON                  1                          //���ڹرհ�ť
#define NEW_WINDOW_RESIZEABLE                   1                          //���ڿ�����
#define NEW_WINDOW_REMOVABLE                    1                          //���ڿ��ƶ�
#define NEW_WINDOW_MAXMIZE                      1                          //����Ĭ�����
#define NEW_WINDOW_ACTIVE                       1                          //����Ĭ�ϼ���

#if GUI_LANGUAGE > 0u
	#define NEW_WINDOW_BAR_FONT                     &GUI_FontHZ24              //�½��������壨���ģ�
#else
	#define NEW_WINDOW_BAR_FONT                     GUI_FONT_24B_1             //�½���������
#endif


/***********************************************************************
*            ��������
************************************************************************
*/
static void _cbBkWindow(WM_MESSAGE* pMsg);                          //���洰�ڻص�����


#endif
