#ifndef  GUI_Clock_DLG_H
#define  GUI_Clock_DLG_H

#include "DIALOG.h"
#include "GUI_Main.h"

#if GUI_OPERATING == 0u
typedef struct
{
	uint16_t Year;
	uint8_t Mon;
	uint8_t Day;
	uint8_t Hour;
	uint8_t Min;
	uint8_t Sec;
	uint8_t Week;
}RTC_T;
#endif

/***********************************************************************
*                            �û��Զ�����Ϣ
************************************************************************
*/

/***********************************************************************
*       �ؼ�ID����  (�ؼ�ID = ҳ�����ID + �ؼ�ƫ��ID)
************************************************************************
*/
#define ID_APP_Clock_FRAMEWIN       	(GUI_ID_USER + APP_Clock_BASE_ID + 0x00)
#define ID_APP_Clock_WINDOW         	(GUI_ID_USER + APP_Clock_BASE_ID + 0x01)
#define ID_APP_Clock_StrDate_TEXT       (GUI_ID_USER + APP_Clock_BASE_ID + 0x02)
#define ID_APP_Clock_Date_TEXT          (GUI_ID_USER + APP_Clock_BASE_ID + 0x03)
#define ID_APP_Clock_Week_TEXT          (GUI_ID_USER + APP_Clock_BASE_ID + 0x04)
#define ID_APP_Clock_StrTime_TEXT       (GUI_ID_USER + APP_Clock_BASE_ID + 0x05)
#define ID_APP_Clock_Time_TEXT          (GUI_ID_USER + APP_Clock_BASE_ID + 0x06)
/***********************************************************************
*                             �Ի�����Դ��
************************************************************************
*/
/* ����ʱ��APP�Ի��� */
static const GUI_WIDGET_CREATE_INFO Dialog_APP_Clock_Create[] =
{
  { FRAMEWIN_CreateIndirect, "Clock", ID_APP_Clock_FRAMEWIN, NEW_APP_COORDINATE_X, NEW_APP_COORDINATE_Y, NEW_APP_WIDTH, NEW_APP_HIGHT, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Date:", ID_APP_Clock_StrDate_TEXT,  100,  380,  60,  25, TEXT_CF_LEFT },
  { TEXT_CreateIndirect, "Date", ID_APP_Clock_Date_TEXT, 160,  380,  190,  25, TEXT_CF_LEFT },
  { TEXT_CreateIndirect, "Week", ID_APP_Clock_Week_TEXT,  350,  380,  100,  25, TEXT_CF_LEFT },
  { TEXT_CreateIndirect, "Time:", ID_APP_Clock_StrTime_TEXT,  520,  380,  125,  25, TEXT_CF_LEFT },
  { TEXT_CreateIndirect, "Time", ID_APP_Clock_Time_TEXT,  580,  380,  100,  25, TEXT_CF_LEFT },
};
/*
************************************************************************
*						       ��������
************************************************************************
*/
static void _cbClock(WM_MESSAGE* pMsg);                             //����ʱ��ˢ�»ص�����
static void _cbAPP_Clock(WM_MESSAGE* pMsg);                         //����ʱ��APP���ڻص�����
WM_HWIN FUN_APP_Clock_Create(void);                                 //����ʱ��APP���ڴ�������


#endif
