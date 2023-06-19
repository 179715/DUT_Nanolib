/*
*********************************************************************************************************
*	                                  
*	ģ������ : uCOS-III
*	�ļ����� : mian.c
*	��    �� : V1.0
*	˵    �� : ��ʵ����ҪѧϰemWin��BMPͼƬ��ʾ���ܣ�ʵ�������ͼƬ�ļ�1.bmp�Ѿ��洢�������̵�Doc�ļ���
*              �£�ʹ�ô�����ǰ������ؽ����ļ��洢��SD����Ŀ¼�У�����SD���嵽���������档
*              ʵ��Ŀ�ģ�
*                1. ѧϰemWin��BMPͼƬ��ʾ��
*                2. emWin���ܵ�ʵ����MainTask.c�ļ����档
*              ʵ�����ݣ�
*                1. �����������¼�������ͨ�����°���K1����ͨ������/RTT��ӡ�����ջʹ�����
*                    ===============================================================
*                    ���ȼ� ʹ��ջ ʣ��ջ �ٷֱ� ������   ������
*                      Prio   Used  Free   Per    CPU     Taskname
*                       28    303   1745   14%    0.28%   App Task GUI
*                        5     70    442   13%    0.00%   App Task COM
*                        4    156    356   30%    0.01%   App Task UserIF
*                        3     70   1978   03%    0.00%   App Msp Pro
*                        2    156    356   30%    0.50%   App Task Start
*                       29     69     59   53%    0.00%   uC/OS-III Timer Task
*                       30     75    181   29%    0.08%   uC/OS-III Stat Task
*                       31     61     67   47%   99.10%   uC/OS-III Idle Task                   
*                    ����ʹ��SecureCRT����H7-TOOL RTT�鿴��ӡ��Ϣ��
*                    App Task Start����  ������������������BSP����������
*                    App Task MspPro���� ����Ϣ������������LED��˸��
*                    App Task UserIF���� ��������Ϣ����
*                    App Task COM����    ����δʹ�á�
*                    App Task GUI����    ��emWin����
*                2. (1) �����õ�printf������ȫ��ͨ������App_Printfʵ�֡�
*                   (2) App_Printf���������ź����Ļ�������������Դ�������⡣
*                3. Ĭ���ϵ���ͨ�����ڴ�ӡ��Ϣ�����ʹ��RTT��ӡ��Ϣ
*                   (1) MDK AC5��MDK AC6��IARͨ��ʹ��bsp.h�ļ��еĺ궨��Ϊ1����
*                       #define Enable_RTTViewer  1
*              ע�����
*                1. Ϊ��֧��uC/Probe�������:
*                   (1).MDKҪ��ѡoption->output -> Debug Infomationѡ�ѡ���Ż��ȼ�0
*                   (2).IARҪѡ��ѡ���Ż��ȼ�low���������������ط��ȹ�ѡ
*                       a. option->C/C++ Compiler->Outputѡ���й�ѡGenerate debug information��
*                       b. option->linker->Outputѡ���й�ѡInclude Generate debug information in output��
*                   (3).ʹ��uCOS-III�궨��
*                       #define OS_CFG_DBG_EN             1u
*                       #define OS_CFG_STAT_TASK_EN       1u ͬʱ��ʼ��ͳ������
*                      	#define CPU_CFG_INT_DIS_MEAS_EN   1u 
*                   (4).Ϊ�˷�����ԣ�����OS_CFG_APP_HOOKS_EN��OS_CFG_ARG_CHK_EN��ʹ��
*                2. ��ʵ��Ĵ����������ʹ��SecureCRT����H7-TOOL RTT�鿴��ӡ��Ϣ��
*                3. ��ؽ��༭��������������TAB����Ϊ4���Ķ����ļ���Ҫ��������ʾ�����롣
*
*	�޸ļ�¼ :
*		�汾��   ����         ����            ˵��
*       V1.0    2021-02-06   Eric2013    1. ST�̼���1.9.0�汾
*                                        2. BSP������V1.2
*                                        3. uCOS-III�汾V3.08.00
*                                        4. uC/CPU�汾V1.32.00
*                                        5. uC/LIB�汾V1.39.00
*                                        6. emWin�汾V6.x
*
*       V1.1    2021-06-22   Eric2013    1. ����RTT��ӡ
*                                       
*	Copyright (C), 2021-2030, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"
#include "GUI_Main.h"



/*
*********************************************************************************************************
*                                       ��̬ȫ�ֱ���
*********************************************************************************************************
*/                                                          
static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

static  OS_TCB   AppTaskMsgProTCB;
static  CPU_STK  AppTaskMsgProStk[APP_CFG_TASK_MsgPro_STK_SIZE];

static  OS_TCB   AppTaskCOMTCB;
static  CPU_STK  AppTaskCOMStk[APP_CFG_TASK_COM_STK_SIZE];

static  OS_TCB   AppTaskUserIFTCB;
static  CPU_STK  AppTaskUserIFStk[APP_CFG_TASK_USER_IF_STK_SIZE];

static  OS_TCB   AppTaskGUITCB;
static  CPU_STK  AppTaskGUIStk[APP_CFG_TASK_GUI_STK_SIZE];

OS_TCB   AppTaskMusicTCB;
static  CPU_STK  AppTaskMusicStk[APP_CFG_TASK_MUSIC_STK_SIZE];

/*
*********************************************************************************************************
*                                      ��������
*********************************************************************************************************
*/
static  void  AppTaskStart          (void     *p_arg);
static  void  AppTaskMsgPro         (void     *p_arg);
static  void  AppTaskUserIF         (void     *p_arg);
static  void  AppTaskCOM			(void 	  *p_arg);
static  void  AppTaskCreate         (void);
static  void  DispTaskInfo          (void);
static  void  AppObjCreate          (void);
static  void  AppTaskMusic          (void);
void  App_Printf (CPU_CHAR *format, ...);

/*
*******************************************************************************************************
*                               ����
*******************************************************************************************************
*/
static  OS_SEM   AppPrintfSemp;	/* ����printf���� */
//static  OS_SEM       SEM_SYNCH;	    /* ����ͬ�� */
OS_FLAG_GRP	         FLAG_Music;

/* 100KB���ڲ�SRAM��64KB��CCM RAM��ͨ������rt_init_mem��ʼ��Ϊ���鶯̬�ڴ�� */
uint64_t AppMalloc[100*1024/8];         
uint64_t AppMallocCCM[64*1024/8] __attribute__((at(0x10000000))); /* ��ʱδ�õ� */
uint64_t SDRAM[4*1024*1024/8] __attribute__((at(EXT_SDRAM_ADDR + 12 * 1024 * 1024)));
/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: ��׼c������ڡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int main(void)
{
    OS_ERR  err;  
	
	/* HAL�⣬MPU��Cache��ʱ�ӵ�ϵͳ��ʼ�� */
	System_Init();

	/* �ں˿���ǰ�ر�HAL��ʱ���׼ */
	HAL_SuspendTick();
	
	/* ��ʼ���δ�ʱ�ӣ��������������濪�� */	
	BSP_OS_TickInit();
	
	/* ��ʼ��uC/OS-III �ں� */
    OSInit(&err);  

	/* ����һ����������Ҳ���������񣩡���������ᴴ�����е�Ӧ�ó������� */
	OSTaskCreate((OS_TCB       *)&AppTaskStartTCB,  /* ������ƿ��ַ */           
                 (CPU_CHAR     *)"App Task Start",  /* ������ */
                 (OS_TASK_PTR   )AppTaskStart,      /* ������������ַ */
                 (void         *)0,                 /* ���ݸ�����Ĳ��� */
                 (OS_PRIO       )APP_CFG_TASK_START_PRIO, /* �������ȼ� */
                 (CPU_STK      *)&AppTaskStartStk[0],     /* ��ջ����ַ */
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE / 10, /* ��ջ������������ʾ��10%��Ϊ����� */
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,      /* ��ջ�ռ��С */
                 (OS_MSG_QTY    )0,  /* ������֧�ֽ��ܵ������Ϣ�� */
                 (OS_TICK       )0,  /* ����ʱ��Ƭ */
                 (void         *)0,  /* ��ջ�ռ��С */  
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
		
				 /*  �������£�
					OS_OPT_TASK_STK_CHK      ʹ�ܼ������ջ��ͳ������ջ���õĺ�δ�õ�
					OS_OPT_TASK_STK_CLR      �ڴ�������ʱ����������ջ
					OS_OPT_TASK_SAVE_FP      ���CPU�и���Ĵ��������������л�ʱ���渡��Ĵ���������
				 */  
                 (OS_ERR       *)&err);

	/* ����������ϵͳ������Ȩ����uC/OS-III */
    OSStart(&err);                                               
    
    (void)&err;
    
    return (0);
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskStart
*	����˵��: ����һ�����������ڶ�����ϵͳ�����󣬱����ʼ���δ����������������Ҫʵ�ְ�����⡣
*	��    ��: p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ��: 2
*********************************************************************************************************
*/
static  void  AppTaskStart (void *p_arg)
{
	OS_ERR      err;

   (void)p_arg;
	
	HAL_ResumeTick();
	
	CPU_Init(); /* �˺���Ҫ���ȵ��ã���Ϊ����������ʹ�õ�us��ms�ӳ��ǻ��ڴ˺����� */
 	bsp_Init();
	BSP_OS_TickEnable();
	
	#if OS_CFG_STAT_TASK_EN > 0u
		OSStatTaskCPUUsageInit(&err);   
	#endif

	#ifdef CPU_CFG_INT_DIS_MEAS_EN
		CPU_IntDisMeasMaxCurReset();
	#endif
    
	/* �������� */
    AppTaskCreate(); 

	/* ���������ͨ�Ż��� */
	AppObjCreate();	
	
    while (1)
	{  
		/* ��Ҫ�����Դ���ĳ��򣬶�Ӧ������̵��õ�SysTick_ISR */
		bsp_ProPer1ms();
		OSTimeDly(1, OS_OPT_TIME_PERIODIC, &err);
    }
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskMsgPro
*	����˵��: ��Ϣ������������LED��˸
*	��    ��: p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ��: 3
*********************************************************************************************************
*/
static void AppTaskMsgPro(void *p_arg)
{
	OS_ERR  err;  
	
	(void)p_arg;
		  
	while(1)
	{
		bsp_LedToggle(2);
	    OSTimeDly(1000, OS_OPT_TIME_DLY, &err);		
	}   
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskUserIF
*	����˵��: ������Ϣ����
*	��    ��: p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ��: 4
*********************************************************************************************************
*/
static void AppTaskUserIF(void *p_arg)
{
	OS_ERR  err;  
	uint8_t ucKeyCode;	/* �������� */
	uint32_t ulStart, ulEnd;
	uint8_t		Pic_Name = 0;
	char buf[20];	
	uint8_t i;
	
	(void)p_arg;
		  
	while(1)
	{        
		ucKeyCode = bsp_GetKey();
		
		if (ucKeyCode != KEY_NONE)
		{
			switch (ucKeyCode)
			{
				case KEY_DOWN_K1:			  /* K1������ ��ӡ����ִ����� */
					DispTaskInfo();	     
					break;
				
				case KEY_DOWN_K2:			  /* K2������ ʵ�ֽ�ͼ���� */
					/* ��¼��ͼǰ��ʼʱ�� */
					ulStart = OSTimeGet(&err);
					/* ���������� */
					OSSchedLock(&err);
					/* ���SD����û��PicSave�ļ�������д��� */
					result = f_mkdir("0:/PicSave");
					/* ��ʼ��ͼ */
					for(i = 0; i < 100; i++)
					{
						if(Pic_Name > 100)
						{
							printf("��ͼ����ռ�����\n\r");
							break;
						}
						sprintf(buf,"0:/PicSave/%d.bmp",Pic_Name);
						/* ������ͼ */
						result = f_open(&file,buf, FA_WRITE|FA_CREATE_NEW);	
						/* �½���ͼʧ�� */
						if(result != FR_OK)
						{
							if(result == FR_EXIST)
							{
								Pic_Name++; 
							}
							else
							{
								printf("��ͼ����ʧ��\n\r");
								break;
							}
						}
						else
						{
							/* ��SD������BMPͼƬ */
							GUI_BMP_Serialize(_WriteByte2File, &file);			
							/* ������ɺ�ر�file */
							result = f_close(&file);
							/* ��¼��ͼ��ʱ�䲢��ȡ��ͼ���̺�ʱ */
							ulEnd = OSTimeGet(&err);
							ulEnd -= ulStart;					
							printf("��ͼ������ɣ���ʱ = %dms\r\n", ulEnd);
							printf("��ͼ����:%s\n\r",buf);
							break;
						}
					}	
					/* ���������� */
					OSSchedUnlock(&err);
					break;
				
				default:                     /* �����ļ�ֵ������ */
					break;
			}
		}
		
		OSTimeDly(20, OS_OPT_TIME_DLY, &err);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskCom
*	����˵��: ��δʹ��
*	��    ��: p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ��: 5
*********************************************************************************************************
*/
static void AppTaskCOM(void *p_arg)
{
	OS_ERR  err;  	

	(void)p_arg;
	
	while(1)
	{
	    OSTimeDly(1000, OS_OPT_TIME_DLY, &err);
	} 						  	 	       											   
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskGUI
*	����˵��: GUI����������ȼ�		  			  
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ����OS_CFG_PRIO_MAX - 4u
*********************************************************************************************************
*/
static void AppTaskGUI(void *p_arg)
{	
    (void)p_arg;		/* ����������澯 */
		
	while (1) 
	{
		MainTask();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: ����Ӧ������
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static  void  AppTaskCreate (void)
{
	OS_ERR      err;
	
	/**************����MsgPro����*********************/
	OSTaskCreate((OS_TCB       *)&AppTaskMsgProTCB,             
                 (CPU_CHAR     *)"App Msp Pro",
                 (OS_TASK_PTR   )AppTaskMsgPro, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_MsgPro_PRIO,
                 (CPU_STK      *)&AppTaskMsgProStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_MsgPro_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_MsgPro_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
				 
	/**************����USER IF����*********************/
	OSTaskCreate((OS_TCB       *)&AppTaskUserIFTCB,             
                 (CPU_CHAR     *)"App Task UserIF",
                 (OS_TASK_PTR   )AppTaskUserIF, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_USER_IF_PRIO,
                 (CPU_STK      *)&AppTaskUserIFStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_USER_IF_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_USER_IF_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);

	/**************����COM����*********************/
	OSTaskCreate((OS_TCB       *)&AppTaskCOMTCB,            
                 (CPU_CHAR     *)"App Task COM",
                 (OS_TASK_PTR   )AppTaskCOM, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_COM_PRIO,
                 (CPU_STK      *)&AppTaskCOMStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_COM_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_COM_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);	 
				 
	/**************����GUI����*********************/			 
	OSTaskCreate((OS_TCB       *)&AppTaskGUITCB,              
                 (CPU_CHAR     *)"App Task GUI",
                 (OS_TASK_PTR   )AppTaskGUI, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_GUI_PRIO,
                 (CPU_STK      *)&AppTaskGUIStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);		
				 
	/**************����Mucis����*********************/			 
	OSTaskCreate((OS_TCB       *)&AppTaskMusicTCB,              
                 (CPU_CHAR     *)"App Task Music",
                 (OS_TASK_PTR   )AppTaskMusic, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_MUSIC_PRIO,
                 (CPU_STK      *)&AppTaskMusicStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_MUSIC_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_MUSIC_STK_SIZE,
                 (OS_MSG_QTY    )5,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&err);					 				 
}

/*
*********************************************************************************************************
*	�� �� ��: AppObjCreate
*	����˵��: ��������ͨѶ
*	��    ��: p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static  void  AppObjCreate (void)
{
	OS_ERR      err;
	
	/* 
	   �����ź�����ֵΪ1��ʱ�����ʵ�ֻ��⹦�ܣ�Ҳ����ֻ��һ����Դ����ʹ�� 
	   �������ǽ�����1�Ĵ�ӡ������Ϊ��������Դ����ֹ���ڴ�ӡ��ʱ������������ռ
	   ��ɴ��ڴ�ӡ���ҡ�
	*/
	OSSemCreate((OS_SEM    *)&AppPrintfSemp,
				(CPU_CHAR  *)"AppPrintfSemp",
				(OS_SEM_CTR )1,
				(OS_ERR    *)&err);
	
	/* �����¼���־�� */
	OSFlagCreate ((OS_FLAG_GRP  *)&FLAG_Music,
                  (CPU_CHAR     *)"FLAG Music",
                  (OS_FLAGS      )0,
                  (OS_ERR       *)&err);
	
	/* ���ڲ�SRAM����100KB����̬�ڴ�ʹ�� */
	rt_init_mem(AppMalloc, 1024*100);
	
	/* ���ڲ�CCM SRAM��64KBȫ������̬�ڴ�ʹ�� */
	rt_init_mem(AppMallocCCM, 1024*64);
}

/*
*********************************************************************************************************
*	�� �� ��: App_Printf
*	����˵��: �̰߳�ȫ��printf��ʽ		  			  
*	��    ��: ͬprintf�Ĳ�����
*             ��C�У����޷��г����ݺ���������ʵ�ε����ͺ���Ŀʱ,������ʡ�Ժ�ָ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void  App_Printf(CPU_CHAR *format, ...)
{
    CPU_CHAR  buf_str[128 + 1];
    va_list   v_args;
    OS_ERR    os_err;


    va_start(v_args, format);
   (void)vsnprintf((char       *)&buf_str[0],
                   (size_t      ) sizeof(buf_str),
                   (char const *) format,
                                  v_args);
    va_end(v_args);

	/* ������� */
    OSSemPend((OS_SEM  *)&AppPrintfSemp,
              (OS_TICK  )0u,
              (OS_OPT   )OS_OPT_PEND_BLOCKING,
              (CPU_TS  *)0,
              (OS_ERR  *)&os_err);

    printf("%s", buf_str);

   (void)OSSemPost((OS_SEM  *)&AppPrintfSemp,
                   (OS_OPT   )OS_OPT_POST_1,
                   (OS_ERR  *)&os_err);

}

/*
*********************************************************************************************************
*	�� �� ��: DispTaskInfo
*	����˵��: ��uCOS-III������Ϣͨ�����ڴ�ӡ����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispTaskInfo(void)
{
	OS_TCB      *p_tcb;	        /* ����һ��������ƿ�ָ��, TCB = TASK CONTROL BLOCK */
	float CPU = 0.0f;
	CPU_SR_ALLOC();

	CPU_CRITICAL_ENTER();
    p_tcb = OSTaskDbgListPtr;
    CPU_CRITICAL_EXIT();
	
	/* ��ӡ���� */
	App_Printf("===============================================================\r\n");
	App_Printf(" ���ȼ� ʹ��ջ ʣ��ջ �ٷֱ� ������   ������\r\n");
	App_Printf("  Prio   Used  Free   Per    CPU     Taskname\r\n");

	/* ����������ƿ��б�(TCB list)����ӡ���е���������ȼ������� */
	while (p_tcb != (OS_TCB *)0) 
	{
		CPU = (float)p_tcb->CPUUsage / 100;
		App_Printf("   %2d  %5d  %5d   %02d%%   %5.2f%%   %s\r\n", 
		p_tcb->Prio, 
		p_tcb->StkUsed, 
		p_tcb->StkFree, 
		(p_tcb->StkUsed * 100) / (p_tcb->StkUsed + p_tcb->StkFree),
		CPU,
		p_tcb->NamePtr);		
	 	
		CPU_CRITICAL_ENTER();
        p_tcb = p_tcb->DbgNextPtr;
        CPU_CRITICAL_EXIT();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskMusic
*	����˵��: ��������
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: 2  
*********************************************************************************************************
*/
static  void  AppTaskMusic(void)
{
	OS_MSG_SIZE	 msg_size;
	MusicMsg_T	 *pMsg;
	OS_ERR       err;
	CPU_TS	     ts;
	
	BSP_WM8978Init();
	
    while(1)
    {
		/* �������ݣ�����MP3�ļ�����WAV�ļ� */
		pMsg = OSTaskQPend(0,
						   OS_OPT_PEND_BLOCKING,
						   &msg_size,   /* �˲����ǽ��յ����ݸ��� */
						   &ts,
						   &err);
		
		if(err == OS_ERR_NONE)
		{
			/* DMA��ֹ��Ҫ�ȵ���ǰת����ϲſ���ֹͣ������������Ҫ�����Ӧ��־ */
			OSFlagPend ((OS_FLAG_GRP  *)&FLAG_Music,
						(OS_FLAGS      )MusicTaskWaitFlag,
						(OS_TICK       )0,
						(OS_OPT        )OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME + OS_OPT_PEND_NON_BLOCKING,
						(CPU_TS       *)&ts,
						(OS_ERR       *)&err);
			
			if(pMsg->ucType == MusicType_WAV)
			{
				printf("������Ϣ%s\n\r",pMsg->ucName);
				WavMusicPlay((const char *)pMsg->ucName);					
			}
//			else if(pMsg->ucType == MusicType_MP3)
//			{
//				MP3MusicPlay((const char *)pMsg->ucName);		
//			}
//			else if(pMsg->ucType == MusicType_FLAC)
//			{
//				FLACMusicPlay((const char *)pMsg->ucName);		
//			}
		}	
    }
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
