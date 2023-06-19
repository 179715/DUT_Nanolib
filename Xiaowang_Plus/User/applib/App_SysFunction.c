/*
*********************************************************************************************************
*	                                  
*	ģ������ : GUI�����ϵͳ����
*	�ļ����� : App_SysFunction.c
*	��    �� : V1.1
*	˵    �� : ͼƬ��ʾ�ͱ����API
*	�޸ļ�¼ :
*		�汾��    ����          ����         ˵��
*		V1.0    2013-03-26    Eric2013       �׷�
*       V1.1    2014-05-23    Eric2013       ɾ�����ļ��ж���ĺ�����ֻ������ͼ��BMPͼƬ����
*
*********************************************************************************************************
*/
#include "bsp.h"
#include "GUI_Main.h"
#include "ff_gen_drv.h"
#include "sd_diskio_dma.h"



/*
*********************************************************************************************************
*	                                  ���ڱ��ļ��ĵ���
*********************************************************************************************************
*/
#if 1
	#define printf_libdbg printf
#else
	#define printf_libdbg(...)
#endif


/*
*********************************************************************************************************
*	                                         ����SD��
*********************************************************************************************************
*/
FRESULT result;
FIL file;
FIL FileSave;   //ר������ͼƬ�ı���
DIR DirInf;
UINT bw;
FATFS fs;
char DiskPath[4]; /* SD���߼�����·�������̷�0������"0:/" */


/* FatFs API�ķ���ֵ */
const char * FR_Table[]= 
{
	"FR_OK���ɹ�",				                             /* (0) Succeeded */
	"FR_DISK_ERR���ײ�Ӳ������",			                 /* (1) A hard error occurred in the low level disk I/O layer */
	"FR_INT_ERR������ʧ��",				                     /* (2) Assertion failed */
	"FR_NOT_READY����������û�й���",			             /* (3) The physical drive cannot work */
	"FR_NO_FILE���ļ�������",				                 /* (4) Could not find the file */
	"FR_NO_PATH��·��������",				                 /* (5) Could not find the path */
	"FR_INVALID_NAME����Ч�ļ���",		                     /* (6) The path name format is invalid */
	"FR_DENIED�����ڽ�ֹ���ʻ���Ŀ¼�������ʱ��ܾ�",         /* (7) Access denied due to prohibited access or directory full */
	"FR_EXIST���ļ��Ѿ�����",			                     /* (8) Access denied due to prohibited access */
	"FR_INVALID_OBJECT���ļ�����Ŀ¼������Ч",		         /* (9) The file/directory object is invalid */
	"FR_WRITE_PROTECTED������������д����",		             /* (10) The physical drive is write protected */
	"FR_INVALID_DRIVE���߼���������Ч",		                 /* (11) The logical drive number is invalid */
	"FR_NOT_ENABLED�������޹�����",			                 /* (12) The volume has no work area */
	"FR_NO_FILESYSTEM��û����Ч��FAT��",		             /* (13) There is no valid FAT volume */
	"FR_MKFS_ABORTED�����ڲ�������f_mkfs()����ֹ",	         /* (14) The f_mkfs() aborted due to any parameter error */
	"FR_TIMEOUT���ڹ涨��ʱ�����޷���÷��ʾ�����",		 /* (15) Could not get a grant to access the volume within defined period */
	"FR_LOCKED�������ļ�������Բ������ܾ�",				 /* (16) The operation is rejected according to the file sharing policy */
	"FR_NOT_ENOUGH_CORE���޷����䳤�ļ���������",		     /* (17) LFN working buffer could not be allocated */
	"FR_TOO_MANY_OPEN_FILES����ǰ�򿪵��ļ�������_FS_SHARE", /* (18) Number of open files > _FS_SHARE */
	"FR_INVALID_PARAMETER��������Ч"	                     /* (19) Given parameter is invalid */
};

/*
*********************************************************************************************************
*	�� �� ��: MountSD
*	����˵��: SD���Ĺ��ؼ���������ȡ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MountSD(void)
{
	/* ע��SD������ */
	FATFS_LinkDriver(&SD_Driver, DiskPath);

	/* �����ļ�ϵͳ */
	result = f_mount(&fs, DiskPath, 0);	/* Mount a logical drive */
	if (result != FR_OK)
	{
		printf_libdbg("�����ļ�ϵͳʧ�� (%s)\r\n", FR_Table[result]);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: _WriteByte2File()
*	����˵��: д�ļ���SD�����������洢����
*	��    �Σ�Data Ҫд�����ݣ� p ָ��FIL���ͱ���      	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void _WriteByte2File(U8 Data, void * p) 
{
	result = f_write (p, &Data, 1, &bw);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
