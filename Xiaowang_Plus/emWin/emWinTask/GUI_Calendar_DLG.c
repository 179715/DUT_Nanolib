/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.28                          *
*        Compiled Jan 30 2015, 16:41:06                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

#include "GUI_Main.h"
#include "GUI_Clock_DLG.h"
#include "GUI_Calendar_DLG.h"
#if GUI_OPERATING > 0u
	#include "includes.h"
#endif

/************************************************************************************************************************
*
*       ��������
*
*************************************************************************************************************************
*/
/* APP���ھ�� */
extern WM_HWIN hWinCalenderTask;       //������APP���ھ��
/* ϵͳʱ��ṹ�� */
#if GUI_OPERATING == 0u
	extern RTC_T g_tRTC;
#endif
/* ϵͳʱ����ʾ���� */
extern char System_Date_Display[15];         //xxxx��xx��xx��
extern char System_Time_Display[9];          //xx:xx:xx

#if GUI_LANGUAGE > 0u
	const char* apWeek[7] =
	{
		"����һ",
		"���ڶ�",
		"������",
		"������",
		"������",
		"������",
		"������",
	};
#else
	const char* apWeek[7] =
	{
		"Monday",
		"Tuesday",
		"Wednesday",
		"Thursday",
		"Friday",
		"Saturday",
		"Sunday",
	};
#endif
#if GUI_LANGUAGE > 0u
	const char* apDays[8] =
	{
		"��",
		"��",
		"һ",
		"��",
		"��",
		"��",
		"��",
		"��",
	};
	const char* apMonths[12] =
	{
		"1��",
		"2��",
		"3��",
		"4��",
		"5��",
		"6��",
		"7��",
		"8��",
		"9��",
		"10��",
		"11��",
		"12��",
	};
#endif

/************************************************************************************************************************
*
*       ��̬����
*
*************************************************************************************************************************
*/
/**
  * @brief ϵͳ���á���Ӧ�ô��ڻص�����
  * @note ��
  * @param WM_MESSAGE* pMsg  GUI��Ϣ
  * @retval ��
  */
static void _cbAPP_Calender(WM_MESSAGE* pMsg)
{
	WM_HWIN hItem;
	WM_HWIN hWin;
	int NCode;
	int Id;
	
	(void) NCode;
	(void) Id;
	
	hWin = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		#if APP_Calender_WINDOW_DEFAULT > 0u
			/* app���ڳ�ʼ�� */
			FRAMEWIN_SetBarColor(hWin, 1, NEW_APP_BAR_COLOR_ENALBE);                //���ڱ�������ɫ
			FRAMEWIN_SetBarColor(hWin, 0, NEW_APP_BAR_COLOR_UNENALBE);
			#if GUI_LANGUAGE > 0u
				FRAMEWIN_SetFont(hWin, &GUI_FontHZ24);                                  //�����ı�����
				FRAMEWIN_SetText(hWin, "������");                                     //�����ı�
			#else
				FRAMEWIN_SetFont(hWin, NEW_APP_BAR_FONT);                               //�����ı�����
				FRAMEWIN_SetText(hWin, "Calendar");                                      //�����ı�
			#endif
			FRAMEWIN_SetTextAlign(hWin, NEW_APP_BAR_ALIGN);                         //�������ı����뷽ʽ
			FRAMEWIN_SetTitleHeight(hWin, NEW_APP_BAR_HIGHT);

			/* ��ʼ���������� */
			if (NEW_APP_CLOSEBUTTON > 0u)
			{
				FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);    //�رհ�ť
			}
			if (NEW_APP_ACTIVE > 0u)
			{
				FRAMEWIN_SetActive(hWin, 1);                                //���ڼ���
			}
		#else
			/* �û������� */
			/* �û����������� */
		#endif
		/* �����ؼ���ʼ�� */
		CALENDAR_SetDefaultSize(CALENDAR_SI_HEADER, 40);
		CALENDAR_SetDefaultSize(CALENDAR_SI_CELL_X, 100);
		CALENDAR_SetDefaultSize(CALENDAR_SI_CELL_Y, 40);
		#if GUI_LANGUAGE > 0u
			CALENDAR_SetDefaultDays(apDays);
			CALENDAR_SetDefaultMonths(apMonths);
			CALENDAR_SetDefaultFont(CALENDAR_FI_CONTENT, &GUI_FontHZ24);
			CALENDAR_SetDefaultFont(CALENDAR_FI_HEADER, &GUI_FontHZ24);
		#else
			CALENDAR_SetDefaultFont(CALENDAR_FI_CONTENT, GUI_FONT_24B_1);
			CALENDAR_SetDefaultFont(CALENDAR_FI_HEADER, GUI_FONT_24B_1);
		#endif
		CALENDAR_SetDefaultBkColor(CALENDAR_CI_WEEKDAY, GUI_WHITE);               //����������ɫ
		CALENDAR_SetDefaultBkColor(CALENDAR_CI_WEEKEND, GUI_WHITE);
		CALENDAR_SetDefaultBkColor(CALENDAR_CI_SEL, GUI_BLUE);
		CALENDAR_SetDefaultBkColor(CALENDAR_CI_HEADER, GUI_LIGHTBLUE);
		/* ��ȡϵͳʱ�� */
		#if GUI_OPERATING > 0u
			RTC_ReadClock();
		#endif
		/* ��ָ��λ�ô���CALENDAR�ؼ� */
		CALENDAR_Create(hWin,                     /* �����ھ�� */
			50,                                   /* ����ڸ������������������ */
			70,                                   /* ����ڸ������������������ */
			g_tRTC.Year,         /* ��ʼ��� */
			g_tRTC.Mon,          /* ��ʼ�·� */
			g_tRTC.Day,          /* ��ʼ���� */
			2,                                    /* ÿ����ʼ���� */
			ID_APP_Calendar_CALENDAR,             /* �ؼ�ID */
			WM_CF_SHOW);                          /* ���ڴ�����־*/
		/* ��ʼ���ı��ؼ� */
		hItem = WM_GetDialogItem(hWin, ID_APP_Calendar_StrDate_TEXT);
		#if GUI_LANGUAGE > 0u
			TEXT_SetText(hItem, "���ڣ�");
			TEXT_SetFont(hItem, &GUI_FontHZ24);
		#else
			TEXT_SetText(hItem, "Date:");
			TEXT_SetFont(hItem, GUI_FONT_24B_1);
		#endif
		hItem = WM_GetDialogItem(hWin, ID_APP_Calendar_StrTime_TEXT);
		#if GUI_LANGUAGE > 0u
			TEXT_SetText(hItem, "ʱ�䣺");
			TEXT_SetFont(hItem, &GUI_FontHZ24);
		#else
			TEXT_SetText(hItem, "Time:");
			TEXT_SetFont(hItem, GUI_FONT_24B_1);
		#endif
		hItem = WM_GetDialogItem(hWin, ID_APP_Calendar_Date_TEXT);
		#if GUI_LANGUAGE > 0u
			sprintf(System_Date_Display, "%0.4d/%0.2d/%0.2d",
					g_tRTC.Year,
					g_tRTC.Mon,
					g_tRTC.Day);
			TEXT_SetText(hItem, System_Date_Display);
			TEXT_SetFont(hItem, &GUI_FontHZ24);
		#else
			sprintf(System_Date_Display, "20%0.2d/%0.2d/%0.2d",
					g_tRTC.Year,
					g_tRTC.Mon,
					g_tRTC.Day);
			TEXT_SetText(hItem, System_Date_Display);
			TEXT_SetFont(hItem, GUI_FONT_24B_1);
		#endif
		hItem = WM_GetDialogItem(hWin, ID_APP_Calendar_Week_TEXT);
		#if GUI_LANGUAGE > 0u
			TEXT_SetText(hItem, apWeek[g_tRTC.Week - 1]);
			TEXT_SetFont(hItem, &GUI_FontHZ24);
		#else
			TEXT_SetText(hItem, apWeek[g_tRTC.Week - 1]);
			TEXT_SetFont(hItem, GUI_FONT_24B_1);
		#endif
		hItem = WM_GetDialogItem(hWin, ID_APP_Calendar_Time_TEXT);
		sprintf(System_Time_Display, "%0.2d:%0.2d:%0.2d",
				g_tRTC.Hour,
				g_tRTC.Min,
				g_tRTC.Sec);
		TEXT_SetText(hItem, System_Time_Display);
		#if GUI_LANGUAGE > 0u
			TEXT_SetFont(hItem, &GUI_FontHZ24);
		#else
			TEXT_SetFont(hItem, GUI_FONT_24B_1);
		#endif
		break;

	case WM_PAINT:	
		GUI_SetBkColor(GUI_WHITE);
		GUI_Clear();					//����
		GUI_SetColor(GUI_BLACK);
		GUI_DrawRect(44, 25, 746, 347);
		GUI_DrawRect(43, 24, 747, 348);
		/* ��ȡϵͳʱ�� */
		#if GUI_OPERATING > 0u
			RTC_ReadClock();
		#endif
		hItem = WM_GetDialogItem(hWin, ID_APP_Calendar_Date_TEXT);
		#if GUI_LANGUAGE > 0u
			sprintf(System_Date_Display, "%0.4d/%0.2d/%0.2d",
					g_tRTC.Year,
					g_tRTC.Mon,
					g_tRTC.Day);
			TEXT_SetText(hItem, System_Date_Display);
		#else
			sprintf(System_Date_Display, "20%0.2d/%0.2d/%0.2d",
					g_tRTC.Year,
					g_tRTC.Mon,
					g_tRTC.Day);
			TEXT_SetText(hItem, System_Date_Display);
		#endif
		hItem = WM_GetDialogItem(hWin, ID_APP_Calendar_Week_TEXT);
		TEXT_SetText(hItem, apWeek[g_tRTC.Week - 1]);
		hItem = WM_GetDialogItem(hWin, ID_APP_Calendar_Time_TEXT);
		sprintf(System_Time_Display, "%0.2d:%0.2d:%0.2d",
				g_tRTC.Hour,
				g_tRTC.Min,
				g_tRTC.Sec);
		TEXT_SetText(hItem, System_Time_Display);
		break;

	case WM_NOTIFY_PARENT:
		break;

	case WM_TIMER:
		WM_Paint(WM_GetClientWindow(hWinCalenderTask));
    	WM_RestartTimer(pMsg->Data.v, 1000);
		break;

	case WM_DELETE:
		break;

	default:
		WM_DefaultProc(pMsg);
		break;
	}
}
/* function end */


/************************************************************************************************************************
*
*       ��������
*
*************************************************************************************************************************
*/
/**
  * @brief Ӧ�ô��ڴ�������--�˺Ź���
  * @note ��
  * @param ��
  * @retval hWin����Դ���е�һ���ؼ��ľ��
  */
WM_HWIN FUN_APP_Calender_Create(void)
{
	WM_HWIN hWin;
	WM_HWIN hTimer;
	(void) hTimer;    //��ָ��
	hWin = GUI_CreateDialogBox(Dialog_APP_Calender_Create, GUI_COUNTOF(Dialog_APP_Calender_Create), _cbAPP_Calender, WM_HBKWIN, 0, 0);
	hTimer = WM_CreateTimer(WM_GetClientWindow(hWin), 0, 1000, 0);       //������ʱ��
	return hWin;
}
/*************************** End of file ****************************/