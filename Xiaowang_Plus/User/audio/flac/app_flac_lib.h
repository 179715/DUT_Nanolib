#ifndef __APP_FLAC_LIB_H__ 
#define __APP_FLAC_LIB_H__  

#include "stdint.h" 
//flaC ��ǩ 
typedef __packed struct 
{
   uint8_t id[3];		   	//ID,���ļ���ʼλ��,������flaC 4����ĸ 
}FLAC_Tag;

//metadata ���ݿ�ͷ��Ϣ�ṹ�� 
typedef __packed struct 
{
  uint8_t head;		   	//metadata blockͷ
	uint8_t size[3];			//metadata block���ݳ���	
}MD_Block_Head;


//FLAC���ƽṹ��
typedef __packed struct 
{ 
    uint32_t totsec ;				   //���׸�ʱ��,��λ:��
    uint32_t cursec ;				   //��ǰ����ʱ��
	
    uint32_t bitrate;	   			//������
	uint32_t samplerate;				//������
	uint16_t outsamples;				//PCM�����������С
	uint16_t bps;					      //λ��,����16bit,24bit,32bit
	uint32_t  ch;                //����
	uint32_t datastart;				 //����֡��ʼ��λ��(���ļ������ƫ��)
	int bytesleft;				     //�ļ�λ��
	
	uint8_t* i2sbuf0;          // ��������0
	uint8_t* i2sbuf1;          // ��������1
	uint8_t* i2STempBuff;      // �������ݻ���
	uint8_t* FileBuff;         // �ļ�����

}__flacctrl;

extern __flacctrl * flacctrl;



uint32_t GetFLACRuntime(uint8_t *pFileName, uint32_t *uiTotalTime);
void GetFLACInfo(uint8_t *_pPath);
void FLACMusicPlay(const char *filename);
#endif




























