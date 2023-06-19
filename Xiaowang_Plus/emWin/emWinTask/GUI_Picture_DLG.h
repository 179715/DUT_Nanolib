#ifndef  GUI_Picture_DLG_H
#define  GUI_Picture_DLG_H

#include "DIALOG.h"
#include "GUI_Main.h"

/***********************************************************************
*       �ؼ�ID����  (�ؼ�ID = ҳ�����ID + �ؼ�ƫ��ID)
************************************************************************
*/
#define ID_APP_Picture_FRAMEWIN		   (GUI_ID_USER + APP_Picture_BASE_ID + 0x00)
/***********************************************************************
*                             �Ի�����Դ��
************************************************************************
*/
/* ͼƬ���APP�Ի��� */
static const GUI_WIDGET_CREATE_INFO Dialog_APP_Picture_Create[] =
{
  { FRAMEWIN_CreateIndirect, "Set Date", ID_APP_Picture_FRAMEWIN, NEW_APP_COORDINATE_X, NEW_APP_COORDINATE_Y, NEW_APP_WIDTH, NEW_APP_HIGHT, 0, 0x64, 0 },
};

/*
************************************************************************
*						       ��������
************************************************************************
*/
static void _cbAPP_Picture(WM_MESSAGE* pMsg);						  //ͼƬ���APP���ڻص�����
WM_HWIN FUN_APP_Picture_Create(void);                                 //ͼƬ���APP���ڴ�������

#endif
