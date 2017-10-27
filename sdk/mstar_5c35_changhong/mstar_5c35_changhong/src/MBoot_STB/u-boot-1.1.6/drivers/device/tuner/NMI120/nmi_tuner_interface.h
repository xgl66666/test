
#ifndef NMI_TUNER_INTERFACE_H
#define NMI_TUNER_INTERFACE_H
#include "./nmiioctl.h"
#include "./nmicmn.h"
typedef enum
{
    NMI_NTSC = 0,
    NMI_PAL_G,
    NMI_PAL_M,
    NMI_PAL_N,
    NMI_PAL_K,
    NMI_PAL_L,
    NMI_PAL_D,
    NMI_SECAM_L,
    NMI_SECAM_B,
    NMI_SECAM_D,
    NMI_ATSC_8VSB,
    NMI_ATSC_64QAM,
    NMI_ATSC_256QAM,
    NMI_DVBT,
    NMI_DVBT2,
    NMI_DTMB,
    NMI_ISDBT,
    NMI_DVBC,
    NMI_ATVSTDMax,
} tNMI_ALLTVSTD;

typedef struct
{
    tNMI_ALLTVSTD tvstd;   //��ʽ
    tNmiOutput output;       //������ͣ�CVBS������Ƶ���
    tNmiDacSel dacsel;       //DACģʽ
    uint32_t freq;             //RFƵ��
    uint32_t if_freq;          //��ƵƵ��
    uint8_t freq_invert;       //RF �Ƿ���
    uint8_t if_freq_invert;    //��Ƶ�Ƿ���
    uint32_t aif;               //ADUIO��ƵƵ��
    uint8_t is_stereo;         //�Ƿ�������
    uint8_t ucBw;             //����
    bool_t  bretune;           //�Ƿ���Ҫ��������
} tNMI_TUNE_PARAM;

//���ô˺�����ʵ��NMI TUNER�ĳ�ʼ��������ֻ�ڿ�ʼʱִ��
extern uint32_t Nmi_Tuner_Interface_init_chip(tTnrInit* pcfg);

//����Ƶ�㣬�Լ����е���ʽ
extern uint8_t Nmi_Tuner_Interface_Tuning(tNMI_TUNE_PARAM* param);

//��ȡTUNER��AGC �Ƿ�LOCK
extern int Nmi_Tuner_Interface_GetLockStatus(void);

//SLEEPģʽ
extern void Nmi_Tuner_Interface_Sleep_Lt(void);

//wake up оƬ
extern void Nmi_Tuner_Interface_Wake_Up_Lt(void);


//��ȡCHIPID
extern uint32_t Nmi_Tuner_Interface_Get_ChipID(void);

//���üĴ���
extern void  Nmi_Tuner_Interface_Wreg(uint32_t addr, uint32_t value);

extern uint32_t  Nmi_Tuner_Interface_Rreg(uint32_t addr);

//��ȡRSSI
extern  int16_t Nmi_Tuner_Interface_GetRSSI(void);

//DEMOD֪ͨ TUNER  Ƶ���Ƿ�������DEMOD����Ƶ��󣬵��ô˺���
extern void Nmi_Tuner_Interface_Demod_Lock(void);

#endif





