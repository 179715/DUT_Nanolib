#ifndef  GUI_Camera_DLG_H
#define  GUI_Camera_DLG_H

#include "GUI_Main.h"

/***********************************************************************
*                            �û��Զ�����Ϣ
************************************************************************
*/
#define MSG_CAMERA (GUI_ID_USER + APP_Camera_BASE_ID + 0x01)
/***********************************************************************
*       �ؼ�ID����  (�ؼ�ID = ҳ�����ID + �ؼ�ƫ��ID)
************************************************************************
*/
#define ID_APP_Camera_FRAMEWIN       (GUI_ID_USER + APP_Camera_BASE_ID + 0x00)

/***********************************************************************
*                             �Ի�����Դ��
************************************************************************
*/
/* �����APP�Ի��� */
static const GUI_WIDGET_CREATE_INFO Dialog_APP_Camera_Create[] =
{
  { FRAMEWIN_CreateIndirect, "Camera", ID_APP_Camera_FRAMEWIN, NEW_APP_COORDINATE_X, NEW_APP_COORDINATE_Y, NEW_APP_WIDTH, NEW_APP_HIGHT, 0, 0x64, 0 },
};

/***********************************************************************
*						       ��������
************************************************************************
*/
static void _cbAPP_Camera(WM_MESSAGE* pMsg);                         //�����APP���ڻص�����
WM_HWIN FUN_APP_Camera_Create(void);                                 //�����APP���ڴ�������


#endif
