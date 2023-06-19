/*
*********************************************************************************************************
*	                                  
*	ģ������ : GUI����������
*	�ļ����� : MainTask.c
*	��    �� : V1.0
*	˵    �� : ��ʵ����ҪѧϰemWin��BMPͼƬ��ʾ������������ṩ��������ʾ�������û����Ժ궨������л�
*              1. ֱ�Ӽ��ص�SDRAM������ʾѡ��Method1
*              2. �߼��ر���ʾ�ķ�ʽѡ��      :Method2
*              ʵ����Ŀ��ǿ���Ƽ���ʽһ����Method1������V7����H7+32λSDRAM��LTDC��ɫ��ʽ����ΪRGB565.
*              ����800*480*2��ͼƬ���Դﵽ8ms��11msһ֡(��Cache��8ms)�������ܹ�ʤ�δ󲿷�Ƕ��ʽGUI��Ŀ��
*              
*	�޸ļ�¼ :
*		�汾��   ����         ����          ˵��
*		V1.0    2021-05-23   Eric2013  	    �װ�    
*                                     
*	Copyright (C), 2021-2030, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __GUI_BMPdisplay_H
#define __GUI_BMPdisplay_H

#include "includes.h"
#include "GUI_Main.h"
#include "ff.h"


void _ShowBMPEx(const char *sFilename);
GUI_HMEM _ShowBMPAt(const char *sFilename, int x, int y);
uint8_t _DrawBMPasBackground(const char *sFilename, int x, int y, GUI_MEMDEV_Handle hMemBMP);

#endif

