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
#if GUI_OPERATING > 0u
	#include "includes.h"
#endif

/************************************************************************************************************************
*
*       ��������
*
*************************************************************************************************************************
*/
/* ����ʱ��λͼ���� */
extern GUI_CONST_STORAGE GUI_BITMAP bmclock_classic_background;
extern GUI_CONST_STORAGE GUI_BITMAP bmclock_standard_hour_hand;
extern GUI_CONST_STORAGE GUI_BITMAP bmclock_standard_minute_hand;
extern GUI_CONST_STORAGE GUI_BITMAP bmclock_standard_second_hand;
/* ʱ����ʾ������� */
extern char System_Date_Display[15];         //xxxx��xx��xx��
extern char System_Time_Display[9];          //xx:xx:xx
extern const char* apWeek[7];
/* APP���ھ�� */
extern WM_HWIN hWinClockTask;          //����ʱ��APP���ھ��
/* ϵͳʱ��ṹ�� */
#if GUI_OPERATING == 0u
	RTC_T g_tRTC;
#endif
/* �洢�豸��ر��� */
GUI_HMEM hMemSec;
GUI_HMEM hMemMin;
GUI_HMEM hMemHour;
GUI_HMEM hMemDST;
GUI_HMEM hMemBK;
uint32_t xSize;
uint32_t ySize;
WM_HWIN hClock;


/************************************************************************************************************************
*
*       ��̬����
*
*************************************************************************************************************************
*/
/**
  * @brief ����ʱ��ˢ�»ص�����
  * @note ��
  * @param WM_MESSAGE* pMsg  GUI��Ϣ
  * @retval ��
  */
static void _cbClock(WM_MESSAGE* pMsg) 
{
    GUI_MEMDEV_Handle ret;
    int      t0;   /* ��������ָ��ļ��� */
    int      t1;
    int      t2;
	
    switch (pMsg->MsgId) 
    {
    case WM_TIMER:
		/* ��ʱ1�����һ��ʱ�� */
        WM_RestartTimer(pMsg->Data.v, 1000);
        #if GUI_OPERATING > 0u
            RTC_ReadClock();
        #endif

        #if GUI_OPERATING > 0u
            /* ��һ��ָ�������������ת���� */
            t0 = 360 - g_tRTC.Sec * 6;
            /* �ڶ���ָ�������������ת���� */
            t1 = 360 - g_tRTC.Min * 6;
            /* ������ָ�������������תʱ�� */
            t2 = 360 - g_tRTC.Hour * 30;
        #else
            /* ��һ��ָ�������������ת���� */
            t0 = 360 - 24 * 6;
            /* �ڶ���ָ�������������ת���� */
            t1 = 360 - 35 * 6;
            /* ������ָ�������������תʱ�� */
            t2 = 360 - 6  * 30;
        #endif

        /* ��������ݻ��Ƶ��洢�豸hMemDST���� */
        ret = GUI_MEMDEV_Select(hMemDST);
        GUI_SetBkColor(GUI_WHITE);
        GUI_Clear();

        /* ����ʱ�ӱ��� */
        GUI_MEMDEV_WriteAt(hMemBK, 0, 0);
        /* ����ʱ�� */
        GUI_MEMDEV_RotateHQ(hMemHour, hMemDST, 0, 0, t2 * 1000, 1000);
        /* ���Ʒ��� */
        GUI_MEMDEV_RotateHQ(hMemMin, hMemDST, 0, 0, t1 * 1000, 1000);
        /* �������� */
        GUI_MEMDEV_RotateHQ(hMemSec, hMemDST, 0, 0, t0 * 1000, 1000);

        GUI_MEMDEV_Select(ret);

        /* ִ�д�����Ч�����ᴥ��ִ��WM_PAINT��Ϣ */
        WM_InvalidateWindow(pMsg->hWin);
		WM_Paint(WM_GetClientWindow(hWinClockTask));
        break;

    case WM_PAINT:
		/* ѡ��������� */
		WM_SelectWindow(WM_GetClientWindow(hClock));
		/* ���Ƶ��໺������ */
		GUI_MULTIBUF_Begin();
		GUI_MEMDEV_WriteAt(hMemDST, (xSize - bmclock_classic_background.XSize) / 2, (ySize - bmclock_classic_background.YSize) / 2);
		GUI_MULTIBUF_End();
		WM_SelectWindow(WM_GetClientWindow(hWinClockTask));
        break;

	case WM_DELETE:
		/* ɾ������ʱ�Ӵ����豸 */
		GUI_MEMDEV_Delete(hMemBK);
	    GUI_MEMDEV_Delete(hMemSec);
	    GUI_MEMDEV_Delete(hMemMin);
		GUI_MEMDEV_Delete(hMemHour);
		GUI_MEMDEV_Delete(hMemDST);
	    break;
	
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}
/* function end */

/**
  * @brief ����ʱ��APP���ڻص�����
  * @note ��
  * @param WM_MESSAGE* pMsg  GUI��Ϣ
  * @retval ��
  */
static void _cbAPP_Clock(WM_MESSAGE* pMsg)
{
	WM_HWIN hWin;
	WM_HWIN hItem;

	int NCode;
	int Id;

	(void)NCode;        //��ָ��
	(void)Id;           //��ָ��

	hWin = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		#if APP_Clock_WINDOW_DEFAULT > 0u
			/* app���ڳ�ʼ�� */
			FRAMEWIN_SetBarColor(hWin, 1, NEW_APP_BAR_COLOR_ENALBE);                //���ڱ�������ɫ
			FRAMEWIN_SetBarColor(hWin, 0, NEW_APP_BAR_COLOR_UNENALBE);
			#if GUI_LANGUAGE > 0u
				FRAMEWIN_SetFont(hWin, NEW_APP_BAR_FONT);                              //�����ı�����
				FRAMEWIN_SetText(hWin, "����ʱ��");                                     //�����ı�
			#else
				FRAMEWIN_SetFont(hWin, NEW_APP_BAR_FONT);                               //�����ı�����
				FRAMEWIN_SetText(hWin, "Clock");                                        //�����ı�
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
		/* ��ȡϵͳʱ�� */
		#if GUI_OPERATING > 0u
			RTC_ReadClock();
		#endif
		/* ��ʼ���ı��ؼ� */
		hItem = WM_GetDialogItem(hWin, ID_APP_Clock_StrDate_TEXT);
		#if GUI_LANGUAGE > 0u
			TEXT_SetText(hItem, "���ڣ�");
			TEXT_SetFont(hItem, &GUI_FontHZ24);
		#else
			TEXT_SetText(hItem, "Date:");
			TEXT_SetFont(hItem, GUI_FONT_24B_1);
		#endif
		hItem = WM_GetDialogItem(hWin, ID_APP_Clock_StrTime_TEXT);
		#if GUI_LANGUAGE > 0u
			TEXT_SetText(hItem, "ʱ�䣺");
			TEXT_SetFont(hItem, &GUI_FontHZ24);
		#else
			TEXT_SetText(hItem, "Time:");
			TEXT_SetFont(hItem, GUI_FONT_24B_1);
		#endif
		hItem = WM_GetDialogItem(hWin, ID_APP_Clock_Date_TEXT);
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
		hItem = WM_GetDialogItem(hWin, ID_APP_Clock_Week_TEXT);
		#if GUI_LANGUAGE > 0u
			TEXT_SetText(hItem, apWeek[g_tRTC.Week - 1]);
			TEXT_SetFont(hItem, &GUI_FontHZ24);
		#else
			TEXT_SetText(hItem, apWeek[g_tRTC.Week - 1]);
			TEXT_SetFont(hItem, GUI_FONT_24B_1);
		#endif
		hItem = WM_GetDialogItem(hWin, ID_APP_Clock_Time_TEXT);
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
        /* ��ȡ*/
        xSize = LCD_GetXSize();
        ySize = LCD_GetYSize();

        /* ʱ�ӱ�������洢�豸 */
        hMemBK = GUI_MEMDEV_CreateFixed(0, 0, bmclock_classic_background.XSize, bmclock_classic_background.YSize, GUI_MEMDEV_HASTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);
        /* ʱ����������洢�豸 */
        hMemSec = GUI_MEMDEV_CreateFixed(0, 0, bmclock_classic_background.XSize, bmclock_classic_background.YSize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);
        /* ʱ�ӷ�������洢�豸 */
        hMemMin = GUI_MEMDEV_CreateFixed(0, 0, bmclock_classic_background.XSize, bmclock_classic_background.YSize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);
        /* ʱ��Сʱ����洢�豸 */
        hMemHour = GUI_MEMDEV_CreateFixed(0, 0, bmclock_classic_background.XSize, bmclock_classic_background.YSize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);
        /* ʱ��Ŀ�Ĵ洢����洢�豸 */
        hMemDST = GUI_MEMDEV_CreateFixed(0, 0, bmclock_classic_background.XSize, bmclock_classic_background.YSize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);

        /* ��ʱ�ӱ��̻��Ƶ��洢�豸 */
        GUI_MEMDEV_Select(hMemBK);
        GUI_SetBkColor(GUI_TRANSPARENT);
        GUI_Clear();
        GUI_DrawBitmap(&bmclock_classic_background, 0, 0);
        GUI_MEMDEV_Select(0);

        /* ��������Ƶ��洢�豸 */
        GUI_MEMDEV_Select(hMemSec);
        GUI_SetBkColor(GUI_TRANSPARENT);
        GUI_Clear();
        GUI_DrawBitmap(&bmclock_standard_second_hand,
            0,
            0);
        GUI_MEMDEV_Select(0);

        /* ��������Ƶ��洢�豸 */
        GUI_MEMDEV_Select(hMemMin);
        GUI_SetBkColor(GUI_TRANSPARENT);
        GUI_Clear();
        GUI_DrawBitmap(&bmclock_standard_minute_hand, 0, 0);
        GUI_MEMDEV_Select(0);

        /* ��ʱ����Ƶ��洢�豸 */
        GUI_MEMDEV_Select(hMemHour);
        GUI_SetBkColor(GUI_TRANSPARENT);
        GUI_Clear();
        GUI_DrawBitmap(&bmclock_standard_hour_hand,
            0,
            0);

        GUI_MEMDEV_Select(0);

		/* ����һ�����ڣ����ڻ���ʱ�� */
	    hClock = WINDOW_CreateEx((xSize - bmclock_classic_background.XSize) / 2, 
						(ySize - bmclock_classic_background.YSize) / 2,
						bmclock_classic_background.XSize,
						bmclock_classic_background.YSize,
						hWin,
						WM_CF_SHOW,
						0,
						ID_APP_Clock_WINDOW,
						_cbClock);
						/* ������ʱ�� */
						
        WM_CreateTimer(WM_GetClientWindow(hClock), /* ������Ϣ�Ĵ��ڵľ�� */
					  0, 	       /* �û������Id���������ͬһ����ʹ�ö����ʱ������ֵ��������Ϊ�㡣 */
					  10,          /* ���ڣ������ڹ���ָ������Ӧ�յ���Ϣ*/
                      0);		   /* ��������ʹ�ã�ӦΪ0 */
		break;

    case WM_PAINT:
        /* ���� */
        GUI_SetBkColor(GUI_WHITE);
        GUI_Clear();
		/* ��ȡϵͳʱ�� */
		#if GUI_OPERATING > 0u
			RTC_ReadClock();
		#endif
		hItem = WM_GetDialogItem(hWin, ID_APP_Clock_Date_TEXT);
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
		hItem = WM_GetDialogItem(hWin, ID_APP_Clock_Week_TEXT);
		TEXT_SetText(hItem, apWeek[g_tRTC.Week - 1]);
		hItem = WM_GetDialogItem(hWin, ID_APP_Clock_Time_TEXT);
		sprintf(System_Time_Display, "%0.2d:%0.2d:%0.2d",
				g_tRTC.Hour,
				g_tRTC.Min,
				g_tRTC.Sec);
		TEXT_SetText(hItem, System_Time_Display);
        break;

	case WM_DELETE:
		/* �������涨ʱ�� */
		Fun_Create_DesktopTimer();
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
  * @brief ����ʱ��APP���ڴ�������
  * @note ��
  * @param ��
  * @retval hWin����Դ���е�һ���ؼ��ľ��
  */
WM_HWIN FUN_APP_Clock_Create(void)
{
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(Dialog_APP_Clock_Create, GUI_COUNTOF(Dialog_APP_Clock_Create), _cbAPP_Clock, WM_HBKWIN, 0, 0);
	return hWin;
}
/*************************** End of file ****************************/