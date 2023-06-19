#ifndef __APP_APE_LIB_H__ 
#define __APP_APE_LIB_H__ 
#include "apedecoder.h"
#include "parser.h"
#include "stm32h7xx_hal.h"
#include "includes.h"

//********************************************************************************
//˵��
//1,֧��16λ������/������ape�Ľ���
//2,���֧��44.1K��APE��ʽ(Fast����)
//3,��ĳЩape�ļ�,���ܲ�֧��,����ǧǧ�����������ת��һ��,������������
////////////////////////////////////////////////////////////////////////////////// 	 
 
#define APE_FILE_BUF_SZ    			40*1024		//APE����ʱ,�ļ�buf��С
#define APE_BLOCKS_PER_LOOP			4*1024		 //APE����ʱ,ÿ��ѭ������block�ĸ���

//APE���ƽṹ��
typedef __packed struct 
{ 
    u32 totsec ;				   //���׸�ʱ��,��λ:��
    u32 cursec ;				   //��ǰ����ʱ��
	
    u32 bitrate;	   			//������
	u32 samplerate;				  //������
	u16 outsamples;				  //PCM�����������С
	u16 bps;					      //λ��
	uint32_t ch;                   //����
	u32 datastart;				 //����֡��ʼ��λ��(���ļ������ƫ��)
	
	u8 *i2sbuf0;           // ��������0
	u8 *i2sbuf1;           // ��������1
	u8 *i2STempBuff;        // �������ݻ���
	u32 bytesinbuffer;     //���ݿ�ʼ��λ��
	u32 totalframes;        //�ܿ���
}__apectrl;

extern __apectrl * apectrl; 

//ȡ2��ֵ����Ľ�Сֵ.
#ifndef AUDIO_MIN			
#define AUDIO_MIN(x,y)	((x)<(y)? (x):(y))
#endif

uint32_t GetAPERuntime(uint8_t *pFileName, uint32_t *uiTotalTime);
void ape_fill_buffer(u16* buf,u16 size);
void ape_i2s_dma_tx_callback(void);
void ape_get_curtime(FIL*fx,__apectrl *apectrl);
void APEMusicPlay(const char *filename);
#endif
