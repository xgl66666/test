
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
    tNMI_ALLTVSTD tvstd;   //制式
    tNmiOutput output;       //输出类型，CVBS或者中频输出
    tNmiDacSel dacsel;       //DAC模式
    uint32_t freq;             //RF频率
    uint32_t if_freq;          //中频频率
    uint8_t freq_invert;       //RF 是否反相
    uint8_t if_freq_invert;    //中频是否反相
    uint32_t aif;               //ADUIO中频频率
    uint8_t is_stereo;         //是否立体声
    uint8_t ucBw;             //带宽
    bool_t  bretune;           //是否需要重新锁定
} tNMI_TUNE_PARAM;

//调用此函数，实现NMI TUNER的初始化操作，只在开始时执行
extern uint32_t Nmi_Tuner_Interface_init_chip(tTnrInit* pcfg);

//设置频点，以及运行的制式
extern uint8_t Nmi_Tuner_Interface_Tuning(tNMI_TUNE_PARAM* param);

//获取TUNER的AGC 是否LOCK
extern int Nmi_Tuner_Interface_GetLockStatus(void);

//SLEEP模式
extern void Nmi_Tuner_Interface_Sleep_Lt(void);

//wake up 芯片
extern void Nmi_Tuner_Interface_Wake_Up_Lt(void);


//获取CHIPID
extern uint32_t Nmi_Tuner_Interface_Get_ChipID(void);

//设置寄存器
extern void  Nmi_Tuner_Interface_Wreg(uint32_t addr, uint32_t value);

extern uint32_t  Nmi_Tuner_Interface_Rreg(uint32_t addr);

//获取RSSI
extern  int16_t Nmi_Tuner_Interface_GetRSSI(void);

//DEMOD通知 TUNER  频点是否锁定，DEMOD锁定频点后，调用此函数
extern void Nmi_Tuner_Interface_Demod_Lock(void);

#endif





