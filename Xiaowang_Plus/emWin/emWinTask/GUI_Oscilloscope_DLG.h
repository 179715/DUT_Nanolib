#ifndef  GUI_Oscilloscope_DLG_H
#define  GUI_Oscilloscope_DLG_H

#include "DIALOG.h"
#include "GUI_Main.h"

/***********************************************************************
*       �궨��
************************************************************************
*/
/* ADCͨ��ѡͨ */
#define Channel_Enable           1
#define Channel_Disable          0
/* ��ʾ������ɼ� */
#define Grid_Visible			 1
#define Grid_Invisible			 0
/* ��ʾ���������� */
#define Grid_Dash				 0
#define Grid_DashDot			 1
#define Grid_DASHDOTDOT			 2
#define Grid_DOT				 3
#define Grid_Solid				 4
/* ʾ����״̬ */
#define Oscilloscope_Running		0
#define Oscilloscope_Keep			1
/* ʾ�������� */
#define Range_5V				 0
#define Range_10V				 1
/* ADC����ģʽ */
#define Normal_Mode				 0
#define FIFO_Mode				 1
/* ADC�������� */
#define _NonOversampled				 0
#define _2XOversampled				 1
#define _4XOversampled				 2
#define _8XOversampled				 3
#define _16XOversampled				 4
#define _32XOversampled				 5
#define _64XOversampled				 6
/***********************************************************************
*       ʾ����ͼ��ҳ��궨��
************************************************************************
*/
/* ʾ����ͼ��ҳ������ */
#define Oscilloscope_COORDINATE_X                   260                          //ʾ����ͼ���ڸ���������
#define Oscilloscope_COORDINATE_Y                   85                          
#define Oscilloscope_WIDTH                          520							 //ʾ����ͼ�γ���
#define Oscilloscope_HIGHT                          380   
/* ʾ����ͼ��ҳ��߿�ߴ� */
#define Oscilloscope_BorderL						30							//ʾ����ͼ����߿�ߴ�
#define Oscilloscope_BorderT						5							//ʾ����ͼ���ϱ߿�ߴ�
#define Oscilloscope_BorderR						5							//ʾ����ͼ���ұ߿�ߴ�
#define Oscilloscope_BorderB						5							//ʾ����ͼ���±߿�ߴ�
#define Oscilloscope_ScrollbarWdith					20							//ʾ����ͼ�ι��ֿ��
/* ʾ����ͼ��ҳ������ߴ� */
#define Oscilloscope_GridDistX						80							//ʾ����ͼ��������
#define Oscilloscope_GridDistY						40							//ʾ����ͼ������߶�
/* ʾ����ͼ��ҳ��������ߴ� */
#define Oscilloscope_ScaleSizeX						4096						//ʾ����ͼ��X����ߴ�
#define Oscilloscope_ScaleSizeY						500							//ʾ����ͼ��Y����ߴ�
/* ʾ����ͼ��ҳ������������ */
#define Oscilloscope_ScalePos						25							//ʾ����ͼ��Y��̶�λ��
#define Oscilloscope_ScaleTickDist					40							//ʾ����ͼ��Y��̶ȼ��
#define Oscilloscope_ScaleFactor					0.05					    //ʾ����ͼ��Y���������
/* ʾ���������������� */
#define Oscilloscope_MaxNumItems					Oscilloscope_ScaleSizeY		//ʾ����������������
/* ʾ��������ҳ�� */
#define Oscilloscope_Settings_COORDINATE_X                   100                        //ʾ�������ô�����ʼ����
#define Oscilloscope_Settings_COORDINATE_Y                   20
#define Oscilloscope_Settings_WIDTH                          600                        //ʾ�������ô��ڳ���
#define Oscilloscope_Settings_HIGHT                          400
#define Oscilloscope_Settings_BAR_HIGHT						 40                         //ʾ�������ô��ڱ������߶�
/***********************************************************************
*       �ؼ�ID����  (�ؼ�ID = ҳ�����ID + �ؼ�ƫ��ID)
************************************************************************
*/
/* ʾ����APP���ڿؼ� */
#define ID_APP_Oscilloscope_FRAMEWIN				(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x00)
#define ID_APP_Oscilloscope_GRAPH					(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x01)
#define ID_APP_Oscilloscope_Channel_1_CHECKBOX      (GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x02)
#define ID_APP_Oscilloscope_Channel_2_CHECKBOX      (GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x03)
#define ID_APP_Oscilloscope_Channel_3_CHECKBOX      (GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x04)
#define ID_APP_Oscilloscope_Channel_4_CHECKBOX      (GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x05)
#define ID_APP_Oscilloscope_Channel_5_CHECKBOX      (GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x06)
#define ID_APP_Oscilloscope_Channel_6_CHECKBOX      (GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x07)
#define ID_APP_Oscilloscope_Channel_7_CHECKBOX      (GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x08)
#define ID_APP_Oscilloscope_Channel_8_CHECKBOX      (GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x09)
#define ID_APP_Oscilloscope_Channel_1_TEXT			(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x0A)
#define ID_APP_Oscilloscope_Channel_2_TEXT			(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x0B)
#define ID_APP_Oscilloscope_Channel_3_TEXT			(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x0C)
#define ID_APP_Oscilloscope_Channel_4_TEXT			(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x0D)
#define ID_APP_Oscilloscope_Channel_5_TEXT			(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x0E)
#define ID_APP_Oscilloscope_Channel_6_TEXT			(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x0F)
#define ID_APP_Oscilloscope_Channel_7_TEXT			(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x10)
#define ID_APP_Oscilloscope_Channel_8_TEXT			(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x11)
#define ID_APP_Oscilloscope_Settings_BUTTON			(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x12)
#define ID_APP_Oscilloscope_Pause_BUTTON			(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x13)
#define ID_APP_Oscilloscope_Switch_BUTTON			(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x14)
#define ID_APP_Oscilloscope_Save_BUTTON				(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x15)
#define ID_APP_Oscilloscope_Clear_BUTTON			(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x16)
#define ID_APP_Oscilloscope_Title_TEXT				(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x17)
/* ʾ�������ô��ڿؼ� */
#define ID_Oscilloscope_Settings_FRAMEWIN						(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x18)
#define ID_Oscilloscope_Settings_Appearance_TEXT				(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x19)
#define ID_Oscilloscope_Settings_GridVis_CHECKBOX				(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x20)
#define ID_Oscilloscope_Settings_Grid_DROPDOWN					(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x21)
#define ID_Oscilloscope_Settings_Range_TEXT						(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x22)
#define ID_Oscilloscope_Settings_Range_DROPDOWN					(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x23)
#define ID_Oscilloscope_Settings_Mode_TEXT						(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x24)
#define ID_Oscilloscope_Settings_Mode_DROPDOWN					(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x25)
#define ID_Oscilloscope_Settings_Oversampling_TEXT				(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x26)
#define ID_Oscilloscope_Settings_Oversampling_SPINBOX			(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x27)
#define ID_Oscilloscope_Settings_OversamplingRate_TEXT			(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x28)
#define ID_Oscilloscope_Settings_SamplingRate_TEXT				(GUI_ID_USER + APP_Oscilloscope_BASE_ID + 0x29)
/***********************************************************************
*                             �Ի�����Դ��
************************************************************************
*/
/* ʾ����APP�Ի��� */
static const GUI_WIDGET_CREATE_INFO Dialog_APP_Oscilloscope_Create[] =
{
  { FRAMEWIN_CreateIndirect, "Oscilloscope", ID_APP_Oscilloscope_FRAMEWIN, NEW_APP_COORDINATE_X, NEW_APP_COORDINATE_Y, NEW_APP_WIDTH, NEW_APP_HIGHT, 0, 0x64, 0 },
  { CHECKBOX_CreateIndirect, "Channel_1",ID_APP_Oscilloscope_Channel_1_CHECKBOX,  10,  10,  130,  25, TEXT_CF_LEFT },
  { CHECKBOX_CreateIndirect, "Channel_2",ID_APP_Oscilloscope_Channel_2_CHECKBOX,  10,  45,  130,  25, TEXT_CF_LEFT },
  { CHECKBOX_CreateIndirect, "Channel_3",ID_APP_Oscilloscope_Channel_3_CHECKBOX,  10,  80,  130,  25, TEXT_CF_LEFT },
  { CHECKBOX_CreateIndirect, "Channel_4",ID_APP_Oscilloscope_Channel_4_CHECKBOX,  10,  115,  130,  25, TEXT_CF_LEFT },
  { CHECKBOX_CreateIndirect, "Channel_5",ID_APP_Oscilloscope_Channel_5_CHECKBOX,  10,  150,  130,  25, TEXT_CF_LEFT },
  { CHECKBOX_CreateIndirect, "Channel_6",ID_APP_Oscilloscope_Channel_6_CHECKBOX,  10,  185,  130,  25, TEXT_CF_LEFT },
  { CHECKBOX_CreateIndirect, "Channel_7",ID_APP_Oscilloscope_Channel_7_CHECKBOX,  10,  220,  130,  25, TEXT_CF_LEFT },
  { CHECKBOX_CreateIndirect, "Channel_8",ID_APP_Oscilloscope_Channel_8_CHECKBOX,  10,  255,  130,  25, TEXT_CF_LEFT },
  { TEXT_CreateIndirect, "Channel_1",ID_APP_Oscilloscope_Channel_1_TEXT,  140,  10,  130,  25, TEXT_CF_LEFT },
  { TEXT_CreateIndirect, "Channel_2",ID_APP_Oscilloscope_Channel_2_TEXT,  140,  45,  130,  25, TEXT_CF_LEFT },
  { TEXT_CreateIndirect, "Channel_3",ID_APP_Oscilloscope_Channel_3_TEXT,  140,  80,  130,  25, TEXT_CF_LEFT },
  { TEXT_CreateIndirect, "Channel_4",ID_APP_Oscilloscope_Channel_4_TEXT,  140,  115,  130,  25, TEXT_CF_LEFT },
  { TEXT_CreateIndirect, "Channel_5",ID_APP_Oscilloscope_Channel_5_TEXT,  140,  150,  130,  25, TEXT_CF_LEFT },
  { TEXT_CreateIndirect, "Channel_6",ID_APP_Oscilloscope_Channel_6_TEXT,  140,  185,  130,  25, TEXT_CF_LEFT },
  { TEXT_CreateIndirect, "Channel_7",ID_APP_Oscilloscope_Channel_7_TEXT,  140,  220,  130,  25, TEXT_CF_LEFT },
  { TEXT_CreateIndirect, "Channel_8",ID_APP_Oscilloscope_Channel_8_TEXT,  140,  255,  130,  25, TEXT_CF_LEFT },
  { BUTTON_CreateIndirect, "Settings",ID_APP_Oscilloscope_Settings_BUTTON,   10,  355,  70,  70, 0},
  { BUTTON_CreateIndirect, "Switch",ID_APP_Oscilloscope_Switch_BUTTON,   90,  355,  70,  70, 0},
  { BUTTON_CreateIndirect, "Pause",ID_APP_Oscilloscope_Pause_BUTTON,   170,  355,  70,  70, 0},
  { BUTTON_CreateIndirect, "Save",ID_APP_Oscilloscope_Save_BUTTON,   130,  300,  110,  40, 0},
  { BUTTON_CreateIndirect, "Clear",ID_APP_Oscilloscope_Clear_BUTTON,   10,  300,  110,  40, 0},
  { TEXT_CreateIndirect, "Title",ID_APP_Oscilloscope_Title_TEXT,  Oscilloscope_COORDINATE_X,  10,  Oscilloscope_WIDTH,  25, TEXT_CF_HCENTER },
};
/* ʾ�������ô��ڶԻ��� */
static const GUI_WIDGET_CREATE_INFO Dialog_Oscilloscope_Settings_Create[] =
{
  { FRAMEWIN_CreateIndirect, "Settings", ID_Oscilloscope_Settings_FRAMEWIN, Oscilloscope_Settings_COORDINATE_X, Oscilloscope_Settings_COORDINATE_Y, Oscilloscope_Settings_WIDTH, Oscilloscope_Settings_HIGHT, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Appearance",ID_Oscilloscope_Settings_Appearance_TEXT,  10,  20,  250,  25, TEXT_CF_LEFT },
  { DROPDOWN_CreateIndirect, "Select",ID_Oscilloscope_Settings_Grid_DROPDOWN,  20,  50,  300,  150, 0 },
  { CHECKBOX_CreateIndirect, "GridVis",ID_Oscilloscope_Settings_GridVis_CHECKBOX,  380,  50,  250,  25, TEXT_CF_LEFT },
  { TEXT_CreateIndirect, "Range",ID_Oscilloscope_Settings_Range_TEXT,  10,  95,  450,  25, TEXT_CF_LEFT },
  { DROPDOWN_CreateIndirect, "Select",ID_Oscilloscope_Settings_Range_DROPDOWN,  20,  125,  300,  60, 0 },
  { TEXT_CreateIndirect, "Mode",ID_Oscilloscope_Settings_Mode_TEXT,  10,  170,  450,  25, TEXT_CF_LEFT },
  { DROPDOWN_CreateIndirect, "Mode",ID_Oscilloscope_Settings_Mode_DROPDOWN,  20,  200,  300,  60, 0 },
  { TEXT_CreateIndirect, "Oversampling",ID_Oscilloscope_Settings_Oversampling_TEXT,  10,  245,  450,  25, TEXT_CF_LEFT },
  { SPINBOX_CreateIndirect, "Oversampling",ID_Oscilloscope_Settings_Oversampling_SPINBOX,  20,  275,  100,  50, 0 },
  { TEXT_CreateIndirect, "SamplingRate",ID_Oscilloscope_Settings_OversamplingRate_TEXT,  140,  290,  250,  25, TEXT_CF_LEFT },
  { TEXT_CreateIndirect, "SamplingRate",ID_Oscilloscope_Settings_SamplingRate_TEXT,  340,  290,  250,  25, TEXT_CF_LEFT },
};
/*
************************************************************************
*						       ��������
************************************************************************
*/
static void _cbAPP_Oscilloscope(WM_MESSAGE* pMsg);							//ʾ����APP���ڻص�����
WM_HWIN FUN_APP_Oscilloscope_Create(void);									//ʾ����APP���ڴ�������
WM_HWIN FUN_Oscilloscope_Settings_Create(WM_HWIN hparents);					//ʾ�������ô��ڴ�������

#endif
