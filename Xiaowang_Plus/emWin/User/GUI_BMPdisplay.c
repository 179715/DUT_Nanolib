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
#include "GUI_BMPdisplay.h"

/*
*********************************************************************************************************
*	                                	����
*********************************************************************************************************
*/
/* ���ú���GUI_BMP_DrawEx��ʾBMPͼƬʱ��������Ĵ�Сһ��Ҫ�ܹ��洢ͼƬ��һ���������ݣ��мǣ�����
   ���磺BMPͼƬ��32λ��ɫ��ȣ����֧�ֳ�����1024�����أ���ô�洢һ���������ݾ���Ҫ1024*4�ֽڡ�
*/
static char acBuffer[1024*4];

/*
*********************************************************************************************************
*	�� �� ��: _GetData
*	����˵��: ������GUI_BMP_DrawEx����
*	��    �Σ�p             FIL��������
*             NumBytesReq   �����ȡ���ֽ���
*             ppData        ����ָ��
*             Off           ���Off = 1����ô�����´���ʼλ�ö�ȡ                 
*	�� �� ֵ: ���ض�ȡ���ֽ���
*********************************************************************************************************
*/
int _GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off) 
{
	static int FileAddress = 0;
	UINT NumBytesRead;
	FIL *PicFile;

	PicFile = (FIL *)p;

	/*
	* ��⻺���С
	*/
	if (NumBytesReq > sizeof(acBuffer)) {
	NumBytesReq = sizeof(acBuffer);
	}

	/*
	* ���ö�ȡλ��
	*/
	if(Off == 1) FileAddress = 0;
	else FileAddress = Off;
	result =f_lseek(PicFile, FileAddress);

	/*
	* ��ȡ���ݵ�����
	*/
	result = f_read(PicFile, acBuffer, NumBytesReq, &NumBytesRead);

	/*
	* ��ָ��ppDataָ���ȡ������
	*/
	*ppData = (const U8 *)acBuffer;

	/*
	* ���ض�ȡ���ֽ���
	*/
	return NumBytesRead;
}

/*
*********************************************************************************************************
*	�� �� ��: _ShowBMPEx
*	����˵��: ��ʾBMPͼƬ
*	��    ��: sFilename  Ҫ��ȡ���ļ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void _ShowBMPEx(const char *sFilename) 
{
	/* ���ļ� */		
	result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return;
	}
	
	/* ����BMPͼƬ */
	GUI_BMP_DrawEx(_GetData, &file, 0, 0);
	
	/* �ر��ļ� */
	f_close(&file);
}

/*
*********************************************************************************************************
*	�� �� ��: _ShowBMPAt
*	����˵��: ��ʾBMPͼƬ
*	��    ��: sFilename  Ҫ��ȡ���ļ���
*                     x  Ҫ��ʾ��x������λ��
*                     y  Ҫ��ʾ��y������λ��
*	�� �� ֵ: ���ػ�����BMPͼƬ���ڴ��豸�����
*********************************************************************************************************
*/
GUI_HMEM _ShowBMPAt(const char *sFilename, int x, int y) 
{
	char *_acBuffer;
	int XSize, YSize;
	GUI_HMEM hMem;
	GUI_MEMDEV_Handle hMemBMP;

	/* ���ļ� */		
	result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return 0;
	}
	 
	/* ����һ���ڴ�ռ� ���ҽ������� */
	hMem = GUI_ALLOC_AllocZero(file.obj.objsize);
	
	/* �����뵽�ڴ�ľ��ת����ָ������ */
	_acBuffer = GUI_ALLOC_h2p(hMem);

	/* ��ȡ�ļ�����̬�ڴ� */
	result = f_read(&file, _acBuffer, file.obj.objsize, &bw);
	if (result != FR_OK)
	{
		return 0;
	}
	
	XSize = GUI_BMP_GetXSize(_acBuffer);
	YSize = GUI_BMP_GetYSize(_acBuffer);
	
	/* �����ڴ��豸������BMPͼƬ���Ƶ����ڴ��豸���棬���ڴ��豸Ҫ�����������õ�
	   �����˳��˺���ǰ����Ҫ�ͷš�
	*/
	hMemBMP = GUI_MEMDEV_CreateEx(0, 0, XSize, YSize, GUI_MEMDEV_HASTRANS);
	GUI_MEMDEV_Select(hMemBMP);
	GUI_BMP_Draw(_acBuffer, x, y);
	GUI_MEMDEV_Select(0);

	/* �ͷŶ�̬�ڴ�hMem */
	GUI_ALLOC_Free(hMem);
	
	/* �ر��ļ� */
	f_close(&file);
	
	return hMemBMP;
}

/*
*********************************************************************************************************
*	�� �� ��: _DrawBMPasBackground
*	����˵��: ��ʾBMPͼƬ��Ϊ����ͼƬ
*	��    ��: sFilename  Ҫ��ȡ���ļ���
*                     x  Ҫ��ʾ��x������λ��
*                     y  Ҫ��ʾ��y������λ��
*               hMemBMP  ����ͼƬ�洢�豸
*	�� �� ֵ: 0Ϊ����ʧ�� 1Ϊ���Ƴɹ�
*********************************************************************************************************
*/
uint8_t _DrawBMPasBackground(const char *sFilename, int x, int y, GUI_HMEM hMemBMP) 
{
	char *_acBuffer;
	GUI_HMEM hMem;

	/* ���ļ� */		
	result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return 0;
	}
	 
	/* ����һ���ڴ�ռ� ���ҽ������� */
	hMem = GUI_ALLOC_AllocZero(file.obj.objsize);
	
	/* �����뵽�ڴ�ľ��ת����ָ������ */
	_acBuffer = GUI_ALLOC_h2p(hMem);

	/* ��ȡ�ļ�����̬�ڴ� */
	result = f_read(&file, _acBuffer, file.obj.objsize, &bw);
	if (result != FR_OK)
	{
		return 0;
	}

	/* �����ڴ��豸������BMPͼƬ���Ƶ����ڴ��豸���棬���ڴ��豸Ҫ�����������õ�
	   �����˳��˺���ǰ����Ҫ�ͷš�
	*/
	GUI_MEMDEV_Select(hMemBMP);
	GUI_BMP_Draw(_acBuffer, x, y);
	GUI_MEMDEV_Select(0);

	/* �ͷŶ�̬�ڴ�hMem */
	GUI_ALLOC_Free(hMem);
	
	/* �ر��ļ� */
	f_close(&file);
	return 1;
}


