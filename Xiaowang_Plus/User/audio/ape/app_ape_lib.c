#include "includes.h"
#include "MainTask.h"	

/********************************************************************************
//˵��
//1,֧��16λ������/������ape�Ľ���
//2,���֧��44.1K��APE��ʽ(Fastѹ������)
//3,��ĳЩape�ļ�,���ܲ�֧��,����ǧǧ�����������ת��һ��,������������
//4,ape�ݲ�֧�ֿ�������ˡ�
********************************************************************************/ 	 
 
__apectrl * apectrl;	//APE���ſ��ƽṹ��
struct ape_ctx_t  *apex; 
extern filter_int *filterbuf64;		//��Ҫ2816�ֽ� 
int *decoded0;
int *decoded1;
int firstbyte;  	
int currentframe;
int nblocks;
int bytesconsumed;
int bytesinbuffer;
u8 *readptr;

/*
*********************************************************************************************************
*	�� �� ��: Ape_Memory
*	����˵��: ר�����ڻ�ȡFLAC��Ƶ����ʱ�Ķ�̬�ڴ�
*	��    ��: FM     0��ȡ��1�ͷš�
*	�� �� ֵ: 0 ��ʾ�ɹ��� 1��ʾʧ��
*********************************************************************************************************
*/
uint8_t Ape_Memory(int8_t FM)
{
	u8 res;
  if(FM==0)   //��ȡ�ڴ�
	{		
	 res=rt_init_mem (SDRAM, 4*1024*1024);
	 if(res!=0)return 1;
	 apex=rt_alloc_mem(SDRAM,sizeof(struct ape_ctx_t));
	 apectrl=rt_alloc_mem(SDRAM,sizeof(__apectrl));
	}
	else//�ͷ��ڴ�.
	{
	  rt_free_mem(SDRAM, apectrl);
		rt_free_mem(SDRAM, apex);
	}
	return 0;
}
/*
*********************************************************************************************************
*	�� �� ��: GetAPERuntime
*	����˵��: ר�����ڻ�ȡFLAC��Ƶ������ʱ�䣬����emWin��ʾ�����б�ʱʱ�����ʾ��
*	��    ��: pFileName     �ļ���� 
*             uiTotalTime   ����ִ��ʱ��
*	�� �� ֵ: TRUE(1) ��ʾ�ɹ��� FALSE(0)��ʾʧ��
*********************************************************************************************************
*/
uint32_t GetAPERuntime(uint8_t *pFileName, uint32_t *uiTotalTime)
{
	
	FIL fout_flac;
  u8 res;
  u32 totalsamples; 
	
	/* ��ȡFLAC�����Ϣ����Ҫ��ִ��ʱ�� */
	res=Ape_Memory(0);   //���붯̬�ڴ�
	if(res!=0)return FALSE;
	res=f_open(&fout_flac,(TCHAR*)pFileName,FA_READ); //��ȡ�ļ�����
  if(res!=0)return FALSE;	
	res=ape_parseheader(&fout_flac,apex);//����ape�ļ�ͷ 
	apectrl->bps=apex->bps;					//�õ���λ��(ape,���ǽ�֧��16λ)
	apectrl->samplerate=apex->samplerate;	//�õ�������,ape֧��48Khz���µĲ�����,�ڸ߾Ϳ��ܿ���...
	if(apex->totalframes>1)totalsamples=apex->finalframeblocks+apex->blocksperframe*(apex->totalframes-1);
	else totalsamples=apex->finalframeblocks;        //һ���������ܲ�����
	apectrl->totsec=totalsamples/apectrl->samplerate;//�õ��ļ���ʱ��
	apectrl->bitrate=(fout_flac.fsize-apex->firstframe)*8/apectrl->totsec;//�õ�λ��
	apectrl->outsamples=APE_BLOCKS_PER_LOOP*2;//PCM���������(16λΪ��λ) 
	apectrl->datastart=apex->firstframe;	//�õ���һ֡�ĵ�ַ
	apectrl->ch=apex->channels;           //����
	apectrl->totalframes=apex->totalframes;//�ܿ���
  f_close(&fout_flac);	
	*uiTotalTime = apectrl->totsec ;
	
	return TRUE;	
}
/*
*********************************************************************************************************
*	�� �� ��: GetAPEInfoo
*	����˵��: ר�����ڻ�ȡAPE��Ƶ�������Ϣ�����㲥��FLAC����ʱ�����Ϣ����ʾ��
*	��    ��: _pPath   �ļ���� 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GetAPEInfo(uint8_t *_pPath)
{
	uint32_t uiTotalTime;
	 
	GetAPERuntime((uint8_t *)_pPath, &uiTotalTime);
	g_tWav.uiTotalTime = apectrl->totsec ;
	sprintf((char *)g_tWav.usInfo, "%dHzs/%dbits/", apectrl->samplerate,apectrl->bps);
	switch(apectrl->ch)
	{
		case 2:
			strcat((char *)g_tWav.usInfo, "Stereo/");
			break;

		case 1:
			strcat((char *)g_tWav.usInfo, "Mono/");
			break;
		
		default:
			break;
	}
	switch(apex->compressiontype)           
	{
		case 1000:
			strcat((char *)g_tWav.usInfo, "Fast");
			break;
		
		case 2000:
			strcat((char *)g_tWav.usInfo, "Normal");
			break;
		
		case 3000:
			strcat((char *)g_tWav.usInfo, "Higt");
			break;
		
		case 4000:
			strcat((char *)g_tWav.usInfo, "Extra High");
			break;
		
		case 5000:
			strcat((char *)g_tWav.usInfo, "Insane");
			break;
		
		default:
			break;
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: APE_TimeShow
*	����˵��: ���ڻ�ȡ��Ƶ��ǰ�Ĳ���ʱ�� 
*	��    ��: _pFile      FIL�ļ�
*            flacctrl  __flacctrl ���ͽṹ��ָ�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/

static void APE_TimeShow(FIL *_pFile, __apectrl *apectrl)
{	
	uint64_t uiPos;
	uint32_t uiTempPos;
	
	/* ��ȡ��ǰ����λ�� */
	uiTempPos = _pFile->fptr;
	uiPos = uiTempPos - apectrl->datastart;
	
	/* ��ȡ��Ƶ��ǰ�Ĳ���ʱ�� */
	g_tWav.uiCurTime = (uint64_t)(uiPos * g_tWav.uiTotalTime) / (_pFile->fsize -  apectrl->datastart);
	
	/* �ָ���ǰ���ڲ��ŵ�λ�� */
	f_lseek (_pFile, uiTempPos);
}



/*********************************************************************************
*	�� �� ��: GetAPEDecoderData
*	����˵��: ����APE��Ƶ���ݡ�
*	��    ��: file �ļ�ϵͳ���
********************************************************************************/

void GetAPEDecoderData(FIL *file)
{
	 int blockstodecode;
   u8 res;

		if(currentframe<apex->totalframes&&nblocks==0)//����δ�����֡?
			 {
      //����һ֡�����ж��ٸ�blocks?
			  if(currentframe==(apex->totalframes-1))nblocks=apex->finalframeblocks;
			  else nblocks=apex->blocksperframe; 
			  apex->currentframeblocks=nblocks;
			//��ʼ��֡����
			  init_frame_decoder(apex,readptr,&firstbyte,&bytesconsumed);
			  readptr+=bytesconsumed;
				bytesinbuffer-=bytesconsumed;
       }
			//��ʼ֡���� 
				if(nblocks>0)
				  {					
				    blockstodecode=AUDIO_MIN(APE_BLOCKS_PER_LOOP,nblocks);//���һ��Ҫ�����blocks����  
				    res=decode_chunk(apex,readptr,&firstbyte,&bytesconsumed,decoded0,decoded1,blockstodecode);
				   if(res!=0)
				     {
						     res=1;
				     }

					 readptr+=bytesconsumed;			  //����ָ��ƫ�Ƶ�������λ��
				   bytesinbuffer-=bytesconsumed; 	//buffer���������������
           if(bytesconsumed>4*APE_BLOCKS_PER_LOOP)//���ִ�����
				      {
					      res=1;								
				      }
					 if(bytesinbuffer<4*APE_BLOCKS_PER_LOOP)//��Ҫ������������
				      { 
					     
								f_lseek(file,file->fptr-bytesinbuffer);
								res=f_read(file,apectrl->i2STempBuff,APE_FILE_BUF_SZ,(u32*)&bytesinbuffer);
					      if(res)//������.
					        { 
						        res=1;
					        }								
					      readptr=apectrl->i2STempBuff;
				      }
            nblocks-=blockstodecode;//block�����ݼ�   
            if(nblocks==0)
							currentframe++;								
			    }
    						
}	

/*
*********************************************************************************************************
*	�� �� ��: ape_TimeSet
*	����˵��: ��������flac����λ�ã���������ˣ�
*	��    ��: _pFile      FIL�ļ�
*	�� �� ֵ: ��
* ��ʱ����ʵ��
*********************************************************************************************************
*/
void ape_TimeSet(FIL *_pFile)
{	
	
}



/*
*********************************************************************************************************
*	�� �� ��: APEMusicPlay
*	����˵��: ����APE��Ƶ
*	��    ��: filename  ������·�� 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void APEMusicPlay(const char *filename) 
{
	OS_ERR       err;
	CPU_TS	     ts;
	OS_FLAGS     xResult;
	FRESULT res = FR_NOT_READY; /* �Ƚ����ʼ��Ϊ�˲�������������в���Ҫ�ļ�� */
	FIL fout;
	uint8_t ucFileOpenFlag = 0;



	
	
	while(1)
		{
		xResult = OSFlagPend ((OS_FLAG_GRP  *)&FLAG_Music,
							  (OS_FLAGS      )0xFFFE,
							  (OS_TICK       )0,
							  (OS_OPT        )OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME,
							  (CPU_TS       *)&ts,
							  (OS_ERR       *)&err);
		
		if(err == OS_ERR_NONE)
		{
			switch (xResult)
			{
				/* �������emWin������Ļ�����λ�Ǳ�emWin������ʹ�� */
				case MusicTaskReserved_0:
					break;
				
				/* ʵ�ֿ������ */
				case MusicTaskAudioAdjust_1:
					  ape_TimeSet(&fout);     //��ʱ����ʵ��
					break;

				/* ��ͣ */
				case MusicTaskAudioPause_2:
				    AUDIO_Pause();
					break;

				/* ���� */
				case MusicTaskAudioResume_3:
				    AUDIO_Resume();
					break;	

				/* ��λ��δʹ�� */
				case MusicTaskReserved_4:
					break;	
				
				/* ��ʼ���� */
				case MusicTaskAudioPlay_5:
					/* ��ȡҪ����APE�ļ��������Ϣ */
				  GetAPEInfo((uint8_t *)filename);
					
					/* �ļ����򿪱�־���� */
					ucFileOpenFlag = 1;
					
					/* ��APE��Ƶ�ļ�*/
					res = f_open(&fout, (TCHAR*)filename, FA_READ);
					f_lseek(&fout, apectrl->datastart);
					APE_TimeShow(&fout, apectrl);

					/* ��ʼ�����ֱ���*/ 
            filterbuf64=rt_alloc_mem(SDRAM,10240);  
	          decoded0=rt_alloc_mem(SDRAM,APE_BLOCKS_PER_LOOP*4);
	          decoded1=rt_alloc_mem(SDRAM,APE_BLOCKS_PER_LOOP*4); 
	          apectrl->i2sbuf0=rt_alloc_mem(SDRAM,APE_BLOCKS_PER_LOOP*4);
	          apectrl->i2sbuf1=rt_alloc_mem(SDRAM,APE_BLOCKS_PER_LOOP*4);
            apectrl->i2STempBuff=rt_alloc_mem(SDRAM,APE_FILE_BUF_SZ);  				
					/*��ʼ��APE������ */
			        memset(apectrl->i2sbuf0,0,APE_BLOCKS_PER_LOOP*4);
			        memset(apectrl->i2sbuf1,0,APE_BLOCKS_PER_LOOP*4);
							
		      f_read(&fout,apectrl->i2STempBuff,APE_FILE_BUF_SZ,(u32*)&bytesinbuffer);
          readptr=apectrl->i2STempBuff;
          currentframe=0;
          firstbyte=3;  		//Take account of the little-endian 32-bit byte ordering					
					/* ��ȡ3֡����õ����� */
					GetAPEDecoderData(&fout);
					memcpy(apectrl->i2sbuf0,decoded1,APE_BLOCKS_PER_LOOP*4);
					GetAPEDecoderData(&fout);
					memcpy(apectrl->i2sbuf1,decoded1,APE_BLOCKS_PER_LOOP*4);
					GetAPEDecoderData(&fout);
					
					/* ��ʼ��SAI��Ƶ�ӿڽ��в��� */
					AUDIO_Init(1, I2S_Standard_Phillips, SAI_DataSize_16b, apectrl->samplerate, 2);
					Play_DMA_Init(apectrl->i2sbuf0, apectrl->i2sbuf1, apectrl->outsamples);
					AUDIO_Play();
					break;	
					
				/* �˳���Ƶ���� */
				case MusicTaskAudioReturn_6:
					AUDIO_Stop();
					if(res == FR_OK)
					{
						f_close (&fout); 
					}
        /*  �ͷ��ڴ�  */
					rt_free_mem(SDRAM, apectrl->i2sbuf0);
					rt_free_mem(SDRAM, apectrl->i2sbuf1);
					rt_free_mem(SDRAM,decoded0);
					rt_free_mem(SDRAM,decoded1);
					rt_free_mem(SDRAM,filterbuf64);
					rt_free_mem(SDRAM,apectrl->i2STempBuff);
					Ape_Memory(1);
					nblocks=0;
					return;

				/* ��ȡ��������ʱ�� */
				case MusicTaskAudioGetTime_7:
					if(ucFileOpenFlag == 1)
          APE_TimeShow(&fout, apectrl);
					break;
					
				/* ��ʼ���ţ������л���һ��ʹ�� */
				case MusicTaskAudioStart_8:
					/* ��ȡҪ����FLAC�ļ��������Ϣ */
				  GetAPEInfo((uint8_t *)filename);
				 
					
					/* mp3�ļ����򿪱�־���� */
					ucFileOpenFlag = 1;
					
					/* ��FLAC��Ƶ�ļ�*/
					res = f_open(&fout, (TCHAR*)filename, FA_READ);
          f_lseek(&fout, apectrl->datastart);
				  APE_TimeShow(&fout, apectrl);	
			
					/* ��ʼ�����ֱ���*/
            filterbuf64=rt_alloc_mem(SDRAM,10240);  
	          decoded0=rt_alloc_mem(SDRAM,APE_BLOCKS_PER_LOOP*4);
	          decoded1=rt_alloc_mem(SDRAM,APE_BLOCKS_PER_LOOP*4); 
	          apectrl->i2sbuf0=rt_alloc_mem(SDRAM,APE_BLOCKS_PER_LOOP*4);
	          apectrl->i2sbuf1=rt_alloc_mem(SDRAM,APE_BLOCKS_PER_LOOP*4);
            apectrl->i2STempBuff=rt_alloc_mem(SDRAM,APE_FILE_BUF_SZ); 				
					/*��ʼ��APE������ */
			        memset(apectrl->i2sbuf0,0,APE_BLOCKS_PER_LOOP*4);
			        memset(apectrl->i2sbuf1,0,APE_BLOCKS_PER_LOOP*4);
							
					f_read(&fout,apectrl->i2STempBuff,APE_FILE_BUF_SZ,(u32*)&bytesinbuffer);
          readptr=apectrl->i2STempBuff;
					currentframe=0;
				  firstbyte=3;  		//Take account of the little-endian 32-bit byte ordering
					/* ��ȡ3֡����õ����� */
					GetAPEDecoderData(&fout);
					memcpy(apectrl->i2sbuf0,decoded1,APE_BLOCKS_PER_LOOP*4);
					GetAPEDecoderData(&fout);
					memcpy(apectrl->i2sbuf1,decoded1,APE_BLOCKS_PER_LOOP*4);
					GetAPEDecoderData(&fout);
					
					/* ��ʼ��SAI��Ƶ�ӿڽ��в��� */
					AUDIO_Init(1, I2S_Standard_Phillips, SAI_DataSize_16b, apectrl->samplerate, 2);
					Play_DMA_Init(apectrl->i2sbuf0,apectrl->i2sbuf1, apectrl->outsamples);
					AUDIO_Play();
					AUDIO_Pause();
					break;
					
				/* ��ǰʹ�õ��ǻ���0����仺��1����ͨ��64��FFTʵ��Ƶ����ʾ */
				case MusicTaskAudioFillBuffer1_9:
					DSP_FFT64(apectrl->i2sbuf0);
					GetAPEDecoderData(&fout);
					memcpy(apectrl->i2sbuf1,decoded1,APE_BLOCKS_PER_LOOP*4);
					break;

				/* ��ǰʹ�õ��ǻ���1����仺��0����ͨ��64��FFTʵ��Ƶ����ʾ */
				case MusicTaskAudioFillBuffer0_10:
					DSP_FFT64(apectrl->i2sbuf1);
					GetAPEDecoderData(&fout);
					memcpy(apectrl->i2sbuf0,decoded1,APE_BLOCKS_PER_LOOP*4);
					break;	
				
				/* ��5λ��δʹ�� */
				default: 
					break;
			}
		}
		
		if(res == FR_OK)
		{
			/* ��Ƶ�ļ���ȡ��ϣ���һ����Ҫ���ڸ������Զ��л� */
			if(f_eof(&fout) != 0)
			{	
				/* �ȴ�����һ��������� */
				xResult = OSFlagPend ((OS_FLAG_GRP  *)&FLAG_Music,
										(OS_FLAGS      )MusicTaskWaitBuffer,
										(OS_TICK       )0,
										(OS_OPT        )OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME,
										(CPU_TS       *)&ts,
										(OS_ERR       *)&err);	
				
				/* �������㣬��ֹ���ſ������ʱ���ظ����Ŵ洢����Ƶ���� */
				if(xResult & MusicTaskAudioFillBuffer1_9)
				{
					memset(apectrl->i2sbuf1, 0, APE_BLOCKS_PER_LOOP*4);
				}
				else
				{
					memset(apectrl->i2sbuf0, 0, APE_BLOCKS_PER_LOOP*4);
				}
				
				/* �ȴ���һ��������� */
				xResult = OSFlagPend ((OS_FLAG_GRP  *)&FLAG_Music,
									  (OS_FLAGS      )MusicTaskWaitBuffer,
									  (OS_TICK       )0,
									  (OS_OPT        )OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME,
									  (CPU_TS       *)&ts,
									  (OS_ERR       *)&err);
				
				/* �������㣬��ֹ���ſ������ʱ���ظ����Ŵ洢����Ƶ���� */
				if(xResult & MusicTaskAudioFillBuffer1_9)
				{
					memset(apectrl->i2sbuf1, 0, APE_BLOCKS_PER_LOOP*4);
				}
				else
				{
					memset(apectrl->i2sbuf0, 0, APE_BLOCKS_PER_LOOP*4);
				}				
				
				/* �ȴ�һ�β���ʱ���ȡ����֤���ֲ��ŵĽ�����������ʾ��� */
				OSFlagPend ((OS_FLAG_GRP  *)&FLAG_Music,
							(OS_FLAGS      )MusicTaskAudioGetTime_7,
							(OS_TICK       )0,
							(OS_OPT        )OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME,
							(CPU_TS       *)&ts,
							(OS_ERR       *)&err);	
				
				/* �ȴ�һ�β���ʱ���ȡ����֤���ֲ��ŵĽ�����������ʾ��� */
        APE_TimeShow(&fout, apectrl);
				OSTimeDly(800, OS_OPT_TIME_DLY, &err);
				
				/* �ر�SAI��Ƶ�ӿں�MP3�ļ� */
				AUDIO_Stop();
				
				if(res == FR_OK)
				{
					f_close (&fout); 
				}

				/* �ͷ���ض�̬�ڴ� */
          rt_free_mem(SDRAM, apectrl->i2sbuf0);
					rt_free_mem(SDRAM, apectrl->i2sbuf1);
					rt_free_mem(SDRAM,decoded0);
					rt_free_mem(SDRAM,decoded1);
					rt_free_mem(SDRAM,filterbuf64);
				  rt_free_mem(SDRAM,apectrl->i2STempBuff);
					Ape_Memory(1);
				  nblocks=0;
				
				/* ��emWin�����ֲ���������Ϣ���л�����һ�׸������� */
				WM_SendMessageNoPara(hWinMusic, MSG_NextMusic);	
				return;			
			} 	
		}
	}
	
}


