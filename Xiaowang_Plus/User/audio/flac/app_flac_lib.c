





#include "includes.h"
#include "MainTask.h"	 
#include "bitstreamf.h"
#include "flacdecoder.h"

/********************************************************************************
//˵��
//1,֧��16λ������/������flac�Ľ������֧��96K��flac��ʽ(����)
//2,֧��24λ������/������flac�Ľ������֧��48K��flac��ʽ(����)
//3,��ĳЩflac�ļ�,���ܲ�֧��,���ø�ʽ�����������ת��һ��,������������
********************************************************************************/ 	 

__flacctrl * flacctrl;	//flac������ƽṹ��
FLACContext *fc;



//����FLAC�ļ�
//fx:flac�ļ�ָ��
//fc:flac��������
//����ֵ:0,�����ɹ�
// ����,�������
u8 flac_init(FIL* fx)
{
	FLAC_Tag * flactag;
	MD_Block_Head *flacblkh;
	uint8_t *buf; 
	u8 endofmetadata=0;			//���һ��metadata���
	int blocklength; 
	u32 br;
	u8 res;

	buf=rt_alloc_mem(SDRAM,512);	//����256�ֽ��ڴ�
	f_lseek(fx,0);				//ƫ�Ƶ��ļ�ͷ
	f_read(fx,buf,4,&br);		//��ȡ4�ֽ� 
	flactag=(FLAC_Tag*)buf;		//ǿ��ת��Ϊflac tag��ǩ
	if(strncmp("fLaC",(char*)flactag->id,4)!=0) 
	{
		rt_free_mem(SDRAM, buf);		//�ͷ��ڴ�
		return 2;				//��flac�ļ�
    } 
    while(!endofmetadata) 
	 {
		  f_read(fx,buf,4,&br);
        if(br<4)break;
		  flacblkh=(MD_Block_Head*)buf;
		  endofmetadata=flacblkh->head&0X80;	//�ж��ǲ������һ��block?
		  blocklength=((u32)flacblkh->size[0]<<16)|((u16)flacblkh->size[1]<<8)|(flacblkh->size[2]);//�õ����С
        if((flacblkh->head&0x7f)==0) 		//head���7λΪ0,���ʾ��STREAMINFO��
         { 
			      res=f_read(fx,buf,blocklength,&br);
            if(res!=FR_OK)break;  
            fc->min_blocksize=((u16)buf[0]<<8) |buf[1];					//��С���С
            fc->max_blocksize=((u16)buf[2]<<8) |buf[3];					//�����С
            fc->min_framesize=((u32)buf[4]<<16)|((u16)buf[5]<<8)|buf[6];//��С֡��С
            fc->max_framesize=((u32)buf[7]<<16)|((u16)buf[8]<<8)|buf[9];//���֡��С
            fc->samplerate=((u32)buf[10]<<12)|((u16)buf[11]<<4)|((buf[12]&0xf0)>>4);//������
            fc->channels=((buf[12]&0x0e)>>1)+1;							//��Ƶͨ����
            fc->bps=((((u16)buf[12]&0x01)<<4)|((buf[13]&0xf0)>>4))+1;	//����λ��16?24?32? 
            fc->totalsamples=((u32)buf[14]<<24)|((u32)buf[15]<<16)|((u16)buf[16]<<8)|buf[17];//һ���������ܲ�����

			flacctrl->samplerate=fc->samplerate;       //������
				
      //fctrl->bps=fc->bps;					 
			flacctrl->totsec=(fc->totalsamples/fc->samplerate);//�õ���ʱ�� 
        }else 	//��������֡�Ĵ��� 
		      { 
            if(f_lseek(fx,fx->fptr+blocklength)!=FR_OK)
            { 
				       rt_free_mem(SDRAM, buf); //�ͷ��ڴ�.
				       return 3;
            }
		      }
    } 
	rt_free_mem(SDRAM, buf);//�ͷ��ڴ�.
	if(flacctrl->totsec)
	{
		flacctrl->outsamples=fc->max_blocksize*2;//PCM���������(*2,��ʾ2��������������)
		flacctrl->bps=fc->bps;			//����λ��(16/24/32)
		flacctrl->ch=fc->channels;     //����
		flacctrl->datastart=fx->fptr;	//FLAC����֡��ʼ�ĵ�ַ
		flacctrl->bitrate=((fx->fsize-flacctrl->datastart)*8)/flacctrl->totsec;//�õ�FLAC��λ��
	}else return 2;	//��ʱ��Ϊ0?�������flac�ļ�
	return 0;
} 

/*
*********************************************************************************************************
*	�� �� ��: Flac_Memory
*	����˵��: ר�����ڻ�ȡFLAC��Ƶ����ʱ�Ķ�̬�ڴ�
*	��    ��: FM     0��ȡ��1�ͷš�
*	�� �� ֵ: 0 ��ʾ�ɹ��� 1��ʾʧ��
*********************************************************************************************************
*/
uint8_t Flac_Memory(int8_t FM)
{
	u8 res;
  if(FM==0)   //��ȡ�ڴ�
	{		
	 res=rt_init_mem (SDRAM, 4*1024*1024);
	 if(res!=0)return 1;
	 fc=rt_alloc_mem(SDRAM,sizeof(FLACContext));
	 flacctrl=rt_alloc_mem(SDRAM,sizeof(__flacctrl));
	}
	else//�ͷ��ڴ�.
	{
	  rt_free_mem(SDRAM, flacctrl);
		rt_free_mem(SDRAM, fc);
	}
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: GetFLACRuntime
*	����˵��: ר�����ڻ�ȡFLAC��Ƶ������ʱ�䣬����emWin��ʾ�����б�ʱʱ�����ʾ��
*	��    ��: pFileName     �ļ���� 
*             uiTotalTime   ����ִ��ʱ��
*	�� �� ֵ: TRUE(1) ��ʾ�ɹ��� FALSE(0)��ʾʧ��
*********************************************************************************************************
*/
uint32_t GetFLACRuntime(uint8_t *pFileName, uint32_t *uiTotalTime)
{
	
	FIL fout_flac;
  u8 res;	
	
	/* ��ȡFLAC�����Ϣ����Ҫ��ִ��ʱ�� */
	res=Flac_Memory(0);
	if(res!=0)return FALSE;
	res=f_open(&fout_flac,(TCHAR*)pFileName,FA_READ); //��ȡ�ļ�����
  if(res!=0)return FALSE;	
	flac_init(&fout_flac);	//flac�����ʼ�� (pFileName, &MP3Control);
	f_close(&fout_flac);
	*uiTotalTime = flacctrl->totsec ;
	
	return TRUE;	
}
/*
*********************************************************************************************************
*	�� �� ��: GetFLACInfoo
*	����˵��: ר�����ڻ�ȡFLAC��Ƶ�������Ϣ�����㲥��FLAC����ʱ�����Ϣ����ʾ��
*	��    ��: _pPath   �ļ���� 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GetFLACInfo(uint8_t *_pPath)
{
	uint32_t uiTotalTime;
	 
	GetFLACRuntime((uint8_t *)_pPath, &uiTotalTime);
	g_tWav.uiTotalTime = flacctrl->totsec ;
	sprintf((char *)g_tWav.usInfo, "%dHzs/%dbits/", flacctrl->samplerate,flacctrl->bps);
	switch(flacctrl->ch)
	{
		case 2:
			strcat((char *)g_tWav.usInfo, "Stereo");
			break;

		case 1:
			strcat((char *)g_tWav.usInfo, "Mono");
			break;
		
		default:
			break;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: FLAC_TimeShow
*	����˵��: ���ڻ�ȡ��Ƶ��ǰ�Ĳ���ʱ�� 
*	��    ��: _pFile      FIL�ļ�
*             pMP3Control MP3Control_T���ͽṹ��ָ�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/

static void FLAC_TimeShow(FIL *_pFile, __flacctrl *flacctrl)
{	
	uint64_t uiPos;
	uint32_t uiTempPos;
	
	/* ��ȡ��ǰ����λ�� */
	uiTempPos = _pFile->fptr;
	uiPos = uiTempPos - flacctrl->datastart;
	
	/* ͨ�����ַ�������ļ��Ĵ�С */
	g_tWav.uiCurTime = (uint64_t)(uiPos * g_tWav.uiTotalTime) / (_pFile->fsize -  flacctrl->datastart);
	
	/* �ָ���ǰ���ڲ��ŵ�λ�� */
	f_lseek (_pFile, uiTempPos);
}


/*********************************************************************************
*	�� �� ��: GetFLACDecoderData
*	����˵��: ����һ֡FLAC��Ƶ���ݡ�
*	��    ��: pFile         �ļ�ϵͳ���
*             _pBuf         ��¼����һ֡FLAC�������
********************************************************************************/

void GetFLACDecoderData(FIL *file)
{
	int consumed;
	int i;
  int32_t uiPos;
	uint8_t *pbuf=0;
	uint8_t res=0;
	UINT br;

	if(fc->bps==24)
		res=flac_decode_frame24(fc,flacctrl->FileBuff,flacctrl->bytesleft,(s32*)flacctrl->i2STempBuff);
	else 
		res=flac_decode_frame16(fc,flacctrl->FileBuff,flacctrl->bytesleft,(s16*)flacctrl->i2STempBuff); 
	if(res == 0)
		{
	     if(fc->bps==24)	//24λ��ʱ��,������Ҫ���������16λ���� SAI��Ƶ�ӿڵ�24λ��Ƶ������ʱδ��� 
			 { 
				 pbuf=flacctrl->i2STempBuff;
				 for(i=0;i<fc->blocksize*4;)
//				 for(i=0;i<fc->blocksize*8;)
				  {
					  flacctrl->i2STempBuff[i++]=pbuf[1];
					  flacctrl->i2STempBuff[i]=pbuf[2];
//            i+=2;
//				    flacctrl->i2STempBuff[i++]=pbuf[0];  //24���ݴ����16λ���ݶ���һ�����ݣ���������Ӱ�졣
					  i++;
					  pbuf+=4;
				  }					
			 }
	     consumed = fc->gb.index / 8;
	     memcpy(flacctrl->FileBuff, &flacctrl->FileBuff[consumed], flacctrl->bytesleft - consumed);
	     flacctrl->bytesleft=flacctrl->bytesleft-consumed;
		   f_read(file, &flacctrl->FileBuff[flacctrl->bytesleft], consumed, &br);
			 if(br>0)
		   flacctrl->bytesleft+=br;
		}
		else
    {  
		 do
		   {
        f_read(file,flacctrl->FileBuff,fc->max_framesize,&br);//��ȡ���֡������
				if(br==0) break;                                     
	      uiPos=flac_seek_frame(flacctrl->FileBuff,br,fc);		//����֡
		   }while(uiPos==-1);                           //�ҵ���ȷ��֡???
			 f_lseek(file,file->fptr-fc->max_framesize+uiPos);
	    f_read(file,flacctrl->FileBuff,fc->max_framesize,&br);
      flacctrl->bytesleft=br;		
	  }		 
}



/*
*********************************************************************************************************
*	�� �� ��: flac_TimeSet
*	����˵��: ��������flac����λ�ã���������ˣ�
*	��    ��: _pFile      FIL�ļ�
*          *flacctrl  __flacctrl ���ͽṹ��ָ�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void flac_TimeSet(FIL *_pFile, __flacctrl *flacctrl)
{	
	int32_t uiPos;
	u32 br=0; 
	
	uiPos  = (uint64_t)(_pFile->fsize -  flacctrl->datastart) * g_tWav.uiCurTime / g_tWav.uiTotalTime ;
	uiPos = uiPos + flacctrl->datastart;
	f_lseek (_pFile, uiPos);      //ƫ�Ƶ����ݿ�ʼ�ĵط�
  do
		{
      f_read(_pFile,flacctrl->FileBuff,fc->max_framesize,&br);//��ȡ���֡������
			if(br==0) break;
	    uiPos=flac_seek_frame(flacctrl->FileBuff,br,fc);		//����֡
		}while(uiPos==-1);                           //�ҵ���ȷ��֡ͷ.
	f_lseek(_pFile,_pFile->fptr-fc->max_framesize+uiPos);
	f_read(_pFile,flacctrl->FileBuff,fc->max_framesize,&br);
  flacctrl->bytesleft=br;	
}


/*
*********************************************************************************************************
*	�� �� ��: FLACMusicPlay
*	����˵��: ����FLAC��Ƶ
*	��    ��: filename  ������·�� 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void FLACMusicPlay(const char *filename) 
	
{
	OS_ERR       err;
	CPU_TS	     ts;
	OS_FLAGS     xResult;
	FRESULT res = FR_NOT_READY; /* �Ƚ����ʼ��Ϊ�˲�������������в���Ҫ�ļ�� */
	FIL fout;
	uint8_t ucFileOpenFlag = 0;
  u32 br=0;
	
	
	
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
					  flac_TimeSet(&fout, flacctrl);
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
					/* ��ȡҪ����FLAC�ļ��������Ϣ */
				  GetFLACInfo((uint8_t *)filename);
					
					/* �ļ����򿪱�־���� */
					ucFileOpenFlag = 1;
					
					/* ��FLAC��Ƶ�ļ�*/
					res = f_open(&fout, (TCHAR*)filename, FA_READ);
					f_lseek(&fout, flacctrl->datastart);
					FLAC_TimeShow(&fout, flacctrl);

					/* ��ʼ�����ֱ���*/ 
          if(fc->min_blocksize==fc->max_blocksize)//����min_blocksize����max_blocksize  &&fc->max_blocksize!=0
			       {
				       flacctrl->i2sbuf0=rt_alloc_mem(SDRAM,fc->max_blocksize*4);
					     flacctrl->i2sbuf1=rt_alloc_mem(SDRAM,fc->max_blocksize*4);									
						   fc->decoded0=rt_alloc_mem(SDRAM,fc->max_blocksize*8); //FLAC���뻺������0
				       fc->decoded1=rt_alloc_mem(SDRAM,fc->max_blocksize*8); //FLAC���뻺������1
						   flacctrl->i2STempBuff=rt_alloc_mem(SDRAM,fc->max_blocksize*8);
               flacctrl->FileBuff=rt_alloc_mem(SDRAM,fc->max_framesize);
					/*��ʼ��FLAC������ */
					 
			        memset(flacctrl->i2sbuf0,0,fc->max_blocksize*4);
			        memset(flacctrl->i2sbuf1,0,fc->max_blocksize*4);
		        }  		 
            f_read(&fout,flacctrl->FileBuff,fc->max_framesize,&br);//��ȡ���֡������	 
						flacctrl->bytesleft=br;
					/* ��ȡ3֡����õ����� */
					GetFLACDecoderData(&fout);
					memcpy(flacctrl->i2sbuf0,flacctrl->i2STempBuff, flacctrl->outsamples*2);
					GetFLACDecoderData(&fout);
					memcpy(flacctrl->i2sbuf1,flacctrl->i2STempBuff, flacctrl->outsamples*2);
					GetFLACDecoderData(&fout);
					
					/* ��ʼ��SAI��Ƶ�ӿڽ��в��� */
					AUDIO_Init(1, I2S_Standard_Phillips, SAI_DataSize_16b, flacctrl->samplerate, 2);
					Play_DMA_Init(flacctrl->i2sbuf0, flacctrl->i2sbuf1, flacctrl->outsamples);
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
					rt_free_mem(SDRAM, flacctrl->i2sbuf0);
					rt_free_mem(SDRAM, flacctrl->i2sbuf1);
					rt_free_mem(SDRAM,fc->decoded0);
					rt_free_mem(SDRAM,fc->decoded1);
					rt_free_mem(SDRAM,flacctrl->i2STempBuff);
          rt_free_mem(SDRAM,flacctrl->FileBuff);
					Flac_Memory(1);
					return;

				/* ��ȡ��������ʱ�� */
				case MusicTaskAudioGetTime_7:
					if(ucFileOpenFlag == 1)
          FLAC_TimeShow(&fout, flacctrl);
					break;
					
				/* ��ʼ���ţ������л���һ��ʹ�� */
				case MusicTaskAudioStart_8:
					/* ��ȡҪ����FLAC�ļ��������Ϣ */
				 GetFLACInfo((uint8_t *)filename);
				 
					
					/* mp3�ļ����򿪱�־���� */
					ucFileOpenFlag = 1;
					
					/* ��FLAC��Ƶ�ļ�*/
					res = f_open(&fout, (TCHAR*)filename, FA_READ);
          f_lseek(&fout, flacctrl->datastart);
				  FLAC_TimeShow(&fout, flacctrl);	
			
					/* ��ʼ�����ֱ���*/
          if(fc->min_blocksize==fc->max_blocksize&&fc->max_blocksize!=0)//����min_blocksize����max_blocksize
			       {
					     flacctrl->i2sbuf0=rt_alloc_mem(SDRAM,fc->max_blocksize*4);
					     flacctrl->i2sbuf1=rt_alloc_mem(SDRAM,fc->max_blocksize*4); 
						   fc->decoded0=rt_alloc_mem(SDRAM,fc->max_blocksize*8); //FLAC���뻺������0
				       fc->decoded1=rt_alloc_mem(SDRAM,fc->max_blocksize*8); //FLAC���뻺������1
               flacctrl->i2STempBuff=rt_alloc_mem(SDRAM,fc->max_blocksize*8);
               flacctrl->FileBuff=rt_alloc_mem(SDRAM,fc->max_framesize);
					 /*��ʼ��FLAC������ */
			        memset(flacctrl->i2sbuf0,0,fc->max_blocksize*4);
			        memset(flacctrl->i2sbuf1,0,fc->max_blocksize*4);
		        }  		 
	           f_read(&fout,flacctrl->FileBuff,fc->max_framesize,&br);//��ȡ���֡������	 
						 flacctrl->bytesleft=br;

					/* ��ȡ3֡����õ����� */
					GetFLACDecoderData(&fout);
					memcpy(flacctrl->i2sbuf0,flacctrl->i2STempBuff, flacctrl->outsamples*2);
					GetFLACDecoderData(&fout);
					memcpy(flacctrl->i2sbuf1,flacctrl->i2STempBuff, flacctrl->outsamples*2);
					GetFLACDecoderData(&fout);
					
					/* ��ʼ��SAI��Ƶ�ӿڽ��в��� */
					AUDIO_Init(1, I2S_Standard_Phillips, SAI_DataSize_16b, flacctrl->samplerate, 2);
					Play_DMA_Init(flacctrl->i2sbuf0,flacctrl->i2sbuf1, flacctrl->outsamples);
					AUDIO_Play();
					AUDIO_Pause();
					break;
					
				/* ��ǰʹ�õ��ǻ���0����仺��1����ͨ��64��FFTʵ��Ƶ����ʾ */
				case MusicTaskAudioFillBuffer1_9:
					DSP_FFT64(flacctrl->i2sbuf0);
					GetFLACDecoderData(&fout);
					memcpy(flacctrl->i2sbuf1,flacctrl->i2STempBuff, flacctrl->outsamples*2);
					break;

				/* ��ǰʹ�õ��ǻ���1����仺��0����ͨ��64��FFTʵ��Ƶ����ʾ */
				case MusicTaskAudioFillBuffer0_10:
					DSP_FFT64(flacctrl->i2sbuf1);
				  GetFLACDecoderData(&fout);
					memcpy(flacctrl->i2sbuf0,flacctrl->i2STempBuff, flacctrl->outsamples*2);
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
					memset(flacctrl->i2sbuf1, 0, fc->max_blocksize*4);
				}
				else
				{
					memset(flacctrl->i2sbuf0, 0, fc->max_blocksize*4);
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
					memset(flacctrl->i2sbuf1, 0, fc->max_blocksize*4);
				}
				else
				{
					memset(flacctrl->i2sbuf0, 0, fc->max_blocksize*4);
				}				
				
				/* �ȴ�һ�β���ʱ���ȡ����֤���ֲ��ŵĽ�����������ʾ��� */
				OSFlagPend ((OS_FLAG_GRP  *)&FLAG_Music,
							(OS_FLAGS      )MusicTaskAudioGetTime_7,
							(OS_TICK       )0,
							(OS_OPT        )OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME,
							(CPU_TS       *)&ts,
							(OS_ERR       *)&err);	
				
				/* �ȴ�һ�β���ʱ���ȡ����֤���ֲ��ŵĽ�����������ʾ��� */
        FLAC_TimeShow(&fout, flacctrl);
				OSTimeDly(800, OS_OPT_TIME_DLY, &err);
				
				/* �ر�SAI��Ƶ�ӿں�MP3�ļ� */
				AUDIO_Stop();
				
				if(res == FR_OK)
				{
					f_close (&fout); 
				}

				/* �ͷ���ض�̬�ڴ� */
          rt_free_mem(SDRAM, flacctrl->i2sbuf0);
					rt_free_mem(SDRAM, flacctrl->i2sbuf1);
					rt_free_mem(SDRAM,fc->decoded0);
					rt_free_mem(SDRAM,fc->decoded1);
					rt_free_mem(SDRAM,flacctrl->i2STempBuff);
          rt_free_mem(SDRAM,flacctrl->FileBuff);
					Flac_Memory(1);
				
				/* ��emWin�����ֲ���������Ϣ���л�����һ�׸������� */
				WM_SendMessageNoPara(hWinMusic, MSG_NextMusic);	
				return;			
			} 	
		}
	}
	
}

