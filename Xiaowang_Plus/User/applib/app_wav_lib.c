/*
*********************************************************************************************************
*
*	ģ������ : WAV����Ӧ��
*	�ļ����� : app_wav_lib.c
*	��    �� : V1.0
*	˵    �� : WAV����ʵ�֣�WAV��ԱȽϼ򵥣�����ͷ�ļ���ʣ�µľ�����Ƶ���ݡ�
*              ����˵����
*              1. ����LoadWavHead��DumpWavHead����WAV�ļ��Ľ�����
*              2. WavMusicPlay��ʵ���ǻ���uCOS-III����ϵͳ�ġ�
*              3. ֧����һ������һ��������Ϳ��ˣ����õĲ����ʶ�֧�֣���������������Ҳ��֧�֡�
*                 ��ǰ��֧��16λ��Ƶ�������24λ��WAV�ᱻת����16λ���в��š�
*              4. ��g_tWav��ȫ�ֱ��������඼��ʹ�þֲ������Ͷ�̬�ڴ档
*
*	�޸ļ�¼ :
*		�汾��    ����         ����         ˵��
*       V1.0    2016-02-16   Eric2013       �׷�
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"
#include "GUI_Main.h"
#include "GUI_Music_DLG.h"

extern WM_HWIN hWinMusicTask;          //���ֲ���APP���ھ��

/*
*********************************************************************************************************
*	                                  ���ڱ��ļ��ĵ���
*********************************************************************************************************
*/
#if 1
	#define printf_Wavdbg printf
#else
	#define printf_Wavdbg(...)
#endif


/*
*********************************************************************************************************
*	                                         �궨��
*********************************************************************************************************
*/
#define Wav_DMABufferSize     1024*4  /* �����DMA�����С����λ�ֽ� */


/*
*********************************************************************************************************
                                          �ļ��ڲ����õĺ���
*********************************************************************************************************
*/
static uint8_t LoadWavHead(FIL *_pFile, WavHead_T *_WavHead);
static void DumpWavHead(WavHead_T *_WavHead);
//static uint32_t Wav_FillAudio(FIL *_pFile, 
//	                          int16_t *_ptr, 
//                              int16_t *_ptrtemp, 
//                              uint16_t _usSize, 
//                              uint8_t usBitsPerSample);
static void Wav_TimeShow(FIL *_pFile, WavHead_T * _WavHead);
static void Wav_TimeSet(FIL *_pFile, WavHead_T * _WavHead);


/*
*********************************************************************************************************
*	�� �� ��: LoadWavHead
*	����˵��: ����Wav��Ƶͷ�ļ�
*	��    ��: _pFile     �ļ����
*             _WavHead   Wavͷ�ļ��ṹ�������ַ
*	�� �� ֵ: TRUE(1) ��ʾ�ɹ��� FALSE(0)��ʾʧ��
*********************************************************************************************************
*/
static uint8_t LoadWavHead(FIL *_pFile, WavHead_T *_WavHead)
{
	RiffHead_T   tRiffChunk;
	FactBlock_T  tFactChunk;
	UINT BytesRead;
	
	/* ��λ���ļ���ʼ��ַ���ж�ȡ */
	f_lseek (_pFile, 0);

	/* ��1�����ȶ�ȡRiffChunk����֤ID��Type�Ƿ���ȷ *******************************************************/
	f_read(_pFile, (BYTE *)&(tRiffChunk), sizeof(tRiffChunk), &BytesRead);
	if(strncmp((char *)tRiffChunk.ucRiffID, "RIFF", 4) || strncmp((char *)tRiffChunk.ucWaveID, "WAVE", 4))
	{
		return FALSE;	
	}
		
	/* ��2������ȡFmtChunk ******************************************************************************/
	f_read(_pFile, (BYTE *)(&_WavHead->tFmt), sizeof(_WavHead->tFmt)-2, &BytesRead);
	
	/*
	   һ�������uiSizeΪ16����ʱ��󸽼���Ϣû�У����Ϊ18��������2���ֽڵĸ�����Ϣ��
       ��Ҫ��һЩ����Ƴɵ�wav��ʽ�к��и�2���ֽڵģ�����и�����Ϣ����ж�ȡ��
	*/
	if(_WavHead->tFmt.uiSize == 18)
	{
		f_read(_pFile, _WavHead->tFmt.ucAddtional, 2, &BytesRead);
	}
		
	/* ��3������ȡFactChunk ****************************************************************************/
	f_read(_pFile, &tFactChunk, sizeof(tFactChunk), &BytesRead);
	
	/* ����Ƿ�ΪFactChunk���������ִ�����´��� */
	if(strncmp((char *)tFactChunk.ucFactID, "fact", 4))
	{
		/* ��һ������Ƿ�ΪDataChunk*/
		if(strncmp((char*)tFactChunk.ucFactID, "data", 4)==0)
		{
			memcpy(_WavHead->tData.ucDataID, tFactChunk.ucFactID, 4);
			_WavHead->tData.uiDataSize = tFactChunk.uiFactSize;
			/* tRiffChunk + FmtChunkǰ������Ա + DataBlock_T = 12 + 8 + 8 = 28 */
			_WavHead->uiDataOffset = 28 + _WavHead->tFmt.uiSize;
		
			printf_Wavdbg("FormatTag = %d\r\n", _WavHead->tFmt.usFormatTag);
			printf_Wavdbg("Channels = %d\r\n", _WavHead->tFmt.usChannels);
			printf_Wavdbg("uiSamplePerSec = %d\r\n", _WavHead->tFmt.uiSamplePerSec);
			printf_Wavdbg("uiAvgBytesPerSec = %d\r\n", _WavHead->tFmt.uiAvgBytesPerSec);
			printf_Wavdbg("BlockAlign = %d\r\n", _WavHead->tFmt.usBlockAlign);
			printf_Wavdbg("BitsPerSample = %d\r\n", _WavHead->tFmt.usBitsPerSample);
			
			return TRUE;
		}
		else 
		{
			return FALSE;
		}
	}
	/* ����Ƿ�ΪFactChunk�������ִ�����´��� */
	else 
	{
		f_lseek(_pFile, _pFile->fptr + tFactChunk.uiFactSize);
	}
	
	/* ��4������ȡDataChunk ***********************************************************************/
	f_read(_pFile, &_WavHead->tData, sizeof(_WavHead->tData), &BytesRead);
	if(strncmp((char*)_WavHead->tData.ucDataID,"data",4))
	{
		return FALSE;
	}
	
	/* tRiffChunk + FmtChunkǰ������Ա + FactChunkǰ������Ա + DataBlock_T = 12 + 8 + 8 + 8 = 36 */
	_WavHead->uiDataOffset = 36 + _WavHead->tFmt.uiSize +  tFactChunk.uiFactSize;
	
	return TRUE;
}

/*
*********************************************************************************************************
*	�� �� ��: DumpWavHead
*	����˵��: ����Wav��Ƶͷ�ļ��������Ƶִ��ʱ�����ص���Ƶ��Ϣ��¼������g_tWav.usInfo����
*	��    ��: _WavHead   Wavͷ�ļ��ṹ�������ַ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
typedef struct
{
	uint32_t Fs;
	uint8_t  pFs[8]; 
}DispFS_T;

const DispFS_T g_tDispFS[]=
{ 
	{8000 ,  "8KHz"}, 
	{16000,  "16KHz"},	
	{32000,  "32KHz"},	
	{48000,  "48KHz"},	
	{96000,  "96KHz"},  
	{192000, "192KHz"}, 
	
	{11020,  "11.02KHz"}, 
	{22050,  "22.05KHz"},  
	{44100,  "44.1KHz"},  
}; 

static void DumpWavHead(WavHead_T *_WavHead)
{
	uint32_t n;
	
	/* 1. ͨ�������ʻ�ò����ʵ��ַ��� */
	for(n = 0; n < (sizeof(g_tDispFS)/sizeof(g_tDispFS[0])); n++)
	{
		if(_WavHead->tFmt.uiSamplePerSec == g_tDispFS[n].Fs)
		{
			break;
		}	
	}
	
	/* 2. ��¼�����ʣ�����λ��������������ͨ����������g_tWav.usInfo���� */
	sprintf((char *)g_tWav.usInfo, "%s/%dbits/",  g_tDispFS[n].pFs, _WavHead->tFmt.usBitsPerSample);
	switch(_WavHead->tFmt.usChannels)
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
	
	printf_Wavdbg("%s\r\n", g_tWav.usInfo);
	printf_Wavdbg("DataSize : %d\r\n", _WavHead->tData.uiDataSize);
	
	/* 3.�����Ƶ��ִ��ʱ�� */
	if((_WavHead->tFmt.uiSamplePerSec != 0 && _WavHead->tFmt.usChannels != 0) && (_WavHead->tFmt.usBitsPerSample != 0))
	{
		g_tWav.uiTotalTime = _WavHead->tData.uiDataSize / _WavHead->tFmt.uiSamplePerSec / _WavHead->tFmt.usChannels;
		g_tWav.uiTotalTime = g_tWav.uiTotalTime * 8 / _WavHead->tFmt.usBitsPerSample;
		
		printf_Wavdbg("uiTotalTime : %d:%d\r\n", g_tWav.uiTotalTime/60, g_tWav.uiTotalTime%60);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Wav_FillAudio
*	����˵��: ��ȡ��Ƶ������䵽DMA��˫������
*	��    ��: _pFile            FIL�ļ�
*             _ptr              ��ȡ����Ƶ���ݷŵ��˻�����
*             _ptrtemp          �������ݵ���ʱ���
*             _usSize           Ҫ��ȡ�����ݴ�С
*             usBitsPerSample   ��Ƶ������С��16λ����24λ
*	�� �� ֵ: ����ʵ�ʶ�ȡ���ֽ���
*********************************************************************************************************
*/
//static uint32_t Wav_FillAudio(FIL *_pFile, int16_t *_ptr, int16_t *_ptrtemp, uint16_t _usSize, uint8_t usBitsPerSample)
//{
//	uint32_t NumBytesReq;
//	uint32_t NumBytesRead;
//	uint32_t i;
//	int16_t *p;
//	
//	/* 24λ��Ƶת��Ϊ16λ���в��ţ�SAI��Ƶ�ӿڵ�24λ��Ƶ������ʱδ��� */
//	if(usBitsPerSample == 24)
//	{
//		/* ��Ҫ��ȡ���ֽ��� */
//		NumBytesReq=(_usSize / 2) * 3;						

//		f_read(_pFile, _ptrtemp, NumBytesReq, (UINT*)&NumBytesRead);	
//		p = _ptrtemp;
//		for(i = 0; i < _usSize; i += 2, p += 3)
//		{
//			_ptr[i]   = p[1];
//			_ptr[i+1] = p[2]; 
//		} 
//		
//		/* ʵ�����ȡ���ֽ��� */
//		NumBytesRead = (NumBytesRead / 3) * 2;		
//	}
//	/* 16λ��Ƶ */
//	else 
//	{
//		f_read(_pFile, _ptr, _usSize, (UINT*)&NumBytesRead);	
//	}
//	
//	if(NumBytesRead < _usSize)
//	{
//		/* ����Ĳ�0 */
//		for(i = NumBytesRead; i < _usSize; i++)
//		{
//			_ptr[i] = 0; 				
//		}
//	}
//	
//	return NumBytesRead;
//} 

/*
*********************************************************************************************************
*	�� �� ��: Wav_TimeShow
*	����˵��: ���ڻ�ȡ��������Ƶ��ǰ�Ĳ���ʱ�� 
*	��    ��: _pFile   FIL�ļ�
*             _WavHead Wavͷ�ļ��ṹ�������ַ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Wav_TimeShow(FIL *_pFile, WavHead_T * _WavHead)
{	
	uint32_t uiPos;
	//char buf[20];
	
	uiPos = _pFile->fptr -  _WavHead->uiDataOffset;
	g_tWav.uiCurTime  = uiPos / _WavHead->tFmt.uiSamplePerSec / _WavHead->tFmt.usChannels;
	g_tWav.uiCurTime  = g_tWav.uiCurTime * 8 / _WavHead->tFmt.usBitsPerSample;
	
	//sprintf(buf, "%02d:%02d/%02d:%02d", g_tWav.uiCurTime / 60, g_tWav.uiCurTime % 60, g_tWav.uiTotalTime / 60, g_tWav.uiTotalTime % 60);
	//printf_Wavdbg("����ʱ��:%s\n\r",buf);
	
}

static void Wav_TimeSet(FIL *_pFile, WavHead_T * _WavHead)
{	
	uint32_t uiPos;
	
	uiPos  = g_tWav.uiCurTime * _WavHead->tFmt.usBitsPerSample / 8;
	uiPos = uiPos *  _WavHead->tFmt.usChannels *  _WavHead->tFmt.uiSamplePerSec;
	uiPos = _WavHead->uiDataOffset + uiPos;
	f_lseek(_pFile, uiPos);
}

/*
*********************************************************************************************************
*	�� �� ��: GetWavRuntime
*	����˵��: ר�����ڻ�ȡWav��Ƶ������ʱ�䣬����emWin��ʾ�����б�ʱʱ�����ʾ��
*	��    ��: _pPath       �ļ�����·��
*             uiTotalTime  ����ִ��ʱ��
*	�� �� ֵ: TRUE(1) ��ʾ�ɹ��� FALSE(0)��ʾʧ��
*********************************************************************************************************
*/
uint32_t GetWavRuntime(uint8_t *_pPath, uint32_t *uiTotalTime)
{
	WavHead_T tWavHead;
	FIL fout;
	FRESULT res;
	
	
	/* 1. ��wav��Ƶ�ļ�������ͷ�ļ���Ϣ */
	res = f_open(&fout, (TCHAR*)_pPath, FA_READ);
	
	printf_Wavdbg("%s\r\n", _pPath);
	
	if(res == FR_OK)
	{
		LoadWavHead(&fout, &tWavHead);
		f_close(&fout);
	}
	else
	{
		return FALSE;		
	}

	/* 2. ���WAV��Ƶ��ִ��ʱ�� */
	if((tWavHead.tFmt.uiSamplePerSec!=0 && tWavHead.tFmt.usChannels!=0) && (tWavHead.tFmt.usBitsPerSample!=0))
	{
		*uiTotalTime = tWavHead.tData.uiDataSize / tWavHead.tFmt.uiSamplePerSec / tWavHead.tFmt.usChannels;
		*uiTotalTime = *uiTotalTime * 8 / tWavHead.tFmt.usBitsPerSample;
		return TRUE;
	}
	
	return FALSE;
}

/*
*********************************************************************************************************
*	�� �� ��: GetWavInfo
*	����˵��: ר�����ڻ�ȡWav��Ƶ������ʱ�䣬����emWin��ʾ�����б�ʱʱ�����ʾ��
*	��    ��: _pPath       �ļ�����·��
*	�� �� ֵ: TRUE(1) ��ʾ�ɹ��� FALSE(0)��ʾʧ��
*********************************************************************************************************
*/
uint32_t GetWavInfo(uint8_t *_pPath)
{
	WavHead_T tWavHead;
	FIL fout;
	FRESULT res;

	/* 1. ��wav��Ƶ�ļ�������ͷ�ļ���Ϣ */	
	res = f_open(&fout, (TCHAR*)_pPath, FA_READ);
	
	if(res == FR_OK)
	{
		LoadWavHead(&fout, &tWavHead);
		DumpWavHead(&tWavHead);
		f_close(&fout);
		return TRUE;
	}
	
	return FALSE;
}

/*
*********************************************************************************************************
*	�� �� ��: WavMusicPlay
*	����˵��: ����WAV��Ƶ
*	��    ��: filename  ������·�� 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void WavMusicPlay(const char *filename)
{
	OS_ERR       err;
	CPU_TS	     ts;
	OS_FLAGS     xResult;
	FRESULT res;
	
	WavHead_T tWavHead;
	uint32_t uiNumBytesFill = Wav_DMABufferSize; 
	FIL fout;
	uint8_t ucFileOpenFlag = 0;
	int16_t *pI2SBuffer1;
	int16_t *pI2SBuffer2;
	int16_t *pI2STempBuff;   
	
	/* ��������Ķ�̬�ڴ� */
	pI2SBuffer1 = (int16_t *)rt_alloc_mem(AppMalloc, Wav_DMABufferSize); 
	pI2SBuffer2 = (int16_t *)rt_alloc_mem(AppMalloc, Wav_DMABufferSize); 
	pI2STempBuff = (int16_t *)rt_alloc_mem(AppMalloc, Wav_DMABufferSize/2*3);
	
	printf_Wavdbg("pI2SBuffer = %x, %x, %x \n\r", (int)pI2SBuffer1, (int)pI2SBuffer2, (int)pI2STempBuff);	
	
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
					Wav_TimeSet(&fout, &tWavHead);
					break;

				/* ��ͣ */
				case MusicTaskAudioPause_2:
				    AUDIO_Stop();
					break;

				/* ���� */
				case MusicTaskAudioResume_3:
				    AUDIO_Play(pI2SBuffer1, pI2SBuffer2, tWavHead.tData.uiDataSize);
					break;	

				/* ��λ��δʹ�� */
				case MusicTaskReserved_4:
					break;	
				
				/* ��ʼ���� */
				case MusicTaskAudioPlay_5:
					/* ��WAV��Ƶ�ļ�����������Ϣ */
					res = f_open(&fout, (TCHAR*)filename, FA_READ);
					//printf_Wavdbg("���ļ�:%s\n\r",filename);
					ucFileOpenFlag = 1;
					LoadWavHead(&fout, &tWavHead);
					DumpWavHead(&tWavHead);
					Wav_TimeShow(&fout, &tWavHead);

					/* ��˫������������� */
					//uiNumBytesFill = Wav_FillAudio(&fout, pI2SBuffer1, pI2STempBuff, Wav_DMABufferSize, tWavHead.tFmt.usBitsPerSample);
					//uiNumBytesFill = Wav_FillAudio(&fout, pI2SBuffer2, pI2STempBuff, Wav_DMABufferSize, tWavHead.tFmt.usBitsPerSample);
					uiNumBytesFill = AUDIO_FillBuff(&fout, pI2STempBuff, Wav_DMABufferSize,tWavHead.tFmt.usBitsPerSample, 0);
					uiNumBytesFill = AUDIO_FillBuff(&fout, pI2STempBuff, Wav_DMABufferSize,tWavHead.tFmt.usBitsPerSample, 1);

					/* ����STM32��SAI��Ƶ�ӿ� */	
					AUDIO_Init(1, I2S_STANDARD_PHILIPS, SAI_DATASIZE_16, tWavHead.tFmt.uiSamplePerSec);
					AUDIO_Play(pI2SBuffer1, pI2SBuffer2, tWavHead.tData.uiDataSize);
					break;	
					
				/* �˳���Ƶ���� */				
				case MusicTaskAudioReturn_6:
					AUDIO_Stop();
					if(res == FR_OK)
					{
						f_close (&fout); 
					}
					rt_free_mem(AppMalloc, pI2STempBuff);
					rt_free_mem(AppMalloc, pI2SBuffer1);
					rt_free_mem(AppMalloc, pI2SBuffer2);
					
					return;

				/* ��ȡ����������Ϣ */
				case MusicTaskAudioGetTime_7:
					if(ucFileOpenFlag == 1)
					{
						Wav_TimeShow(&fout, &tWavHead);							
					}
					break;
					
				/* ��ʼ���ţ������л���һ��ʹ�� */
				case MusicTaskAudioStart_8:
					/* ��WAV��Ƶ�ļ�����������Ϣ */
					res = f_open(&fout, (TCHAR*)filename, FA_READ);
					ucFileOpenFlag = 1;
					LoadWavHead(&fout, &tWavHead);
					DumpWavHead(&tWavHead);
					Wav_TimeShow(&fout, &tWavHead);

					/* ��������������� */
					//uiNumBytesFill = Wav_FillAudio(&fout, pI2SBuffer1, pI2STempBuff, Wav_DMABufferSize, tWavHead.tFmt.usBitsPerSample);
					//uiNumBytesFill = Wav_FillAudio(&fout, pI2SBuffer2, pI2STempBuff, Wav_DMABufferSize, tWavHead.tFmt.usBitsPerSample);
					uiNumBytesFill = AUDIO_FillBuff(&fout, pI2STempBuff, Wav_DMABufferSize,tWavHead.tFmt.usBitsPerSample, 0);
					uiNumBytesFill = AUDIO_FillBuff(&fout, pI2STempBuff, Wav_DMABufferSize,tWavHead.tFmt.usBitsPerSample, 1);

					/* ����STM32��SAI��Ƶ�ӿ� */	
					AUDIO_Init(1, I2S_STANDARD_PHILIPS, SAI_DATASIZE_16, tWavHead.tFmt.uiSamplePerSec);
					AUDIO_Play(pI2SBuffer1, pI2SBuffer2, tWavHead.tData.uiDataSize);
					AUDIO_Stop();
					break;
				
				/* �Զ���仺������һ�� */
				case MusicTaskAudioFillBuffer2_9:
				    //uiNumBytesFill = Wav_FillAudio(&fout, pI2SBuffer2, pI2STempBuff, Wav_DMABufferSize, tWavHead.tFmt.usBitsPerSample);
					uiNumBytesFill = AUDIO_FillBuff(&fout, pI2STempBuff, Wav_DMABufferSize,tWavHead.tFmt.usBitsPerSample, 1);
			
					break;

				/* �Զ���仺����ǰһ�� */
				case MusicTaskAudioFillBuffer1_10:
					//uiNumBytesFill = Wav_FillAudio(&fout, pI2SBuffer1, pI2STempBuff, Wav_DMABufferSize, tWavHead.tFmt.usBitsPerSample);
					uiNumBytesFill = AUDIO_FillBuff(&fout, pI2STempBuff, Wav_DMABufferSize,tWavHead.tFmt.usBitsPerSample, 0);
					break;
				
				/* ��5λ��δʹ�� */
				default: 
					break;
			}	
		}
		
		//AUDIO_Poll();
		
		/* ��һ����Ҫ���ڸ������Զ��л� */
		if(uiNumBytesFill < Wav_DMABufferSize)//���Ž���?
		{
			/* �ȴ�����һ��������� */
			xResult = OSFlagPend ((OS_FLAG_GRP  *)&FLAG_Music,
									(OS_FLAGS      )MusicTaskWaitBuffer,
									(OS_TICK       )0,
									(OS_OPT        )OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME,
									(CPU_TS       *)&ts,
									(OS_ERR       *)&err);	
			
			/* �������㣬��ֹ���ſ������ʱ���ظ����Ŵ洢����Ƶ���� */
			if(xResult & MusicTaskAudioFillBuffer2_9)
			{
				memset(pI2SBuffer2, 0, Wav_DMABufferSize);
			}
			else
			{
				memset(pI2SBuffer1, 0, Wav_DMABufferSize);
			}
			
			/* �ȴ���һ��������� */
			xResult = OSFlagPend ((OS_FLAG_GRP  *)&FLAG_Music,
								  (OS_FLAGS      )MusicTaskWaitBuffer,
								  (OS_TICK       )0,
								  (OS_OPT        )OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME,
								  (CPU_TS       *)&ts,
								  (OS_ERR       *)&err);
			
			/* �������㣬��ֹ���ſ������ʱ���ظ����Ŵ洢����Ƶ���� */
			if(xResult & MusicTaskAudioFillBuffer2_9)
			{
				memset(pI2SBuffer2, 0, Wav_DMABufferSize);
			}
			else
			{
				memset(pI2SBuffer1, 0, Wav_DMABufferSize);
			}					
			
			/* �ȴ�һ�β���ʱ���ȡ����֤���ֲ��ŵĽ�����������ʾ��� */
			OSFlagPend ((OS_FLAG_GRP  *)&FLAG_Music,
						(OS_FLAGS      )MusicTaskAudioGetTime_7,
						(OS_TICK       )0,
						(OS_OPT        )OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME,
						(CPU_TS       *)&ts,
						(OS_ERR       *)&err);	
						
			Wav_TimeShow(&fout, &tWavHead);	
			
			OSTimeDly(800, OS_OPT_TIME_DLY, &err);
				
			/* �ر�SAI��Ƶ�ӿں�MP3�ļ� */
			AUDIO_Stop();
			
			if(res == FR_OK)
			{
				f_close (&fout); 
			}

			/* �ͷ���ض�̬�ڴ� */
			rt_free_mem(AppMalloc, pI2STempBuff);
			rt_free_mem(AppMalloc, pI2SBuffer1);
			rt_free_mem(AppMalloc, pI2SBuffer2);
			
			/* ��emWin�����ֲ���������Ϣ���л�����һ�׸������� */
			WM_SendMessageNoPara(hWinMusicTask, MSG_NextMusic);
			
			return;			
		} 	
	}
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
