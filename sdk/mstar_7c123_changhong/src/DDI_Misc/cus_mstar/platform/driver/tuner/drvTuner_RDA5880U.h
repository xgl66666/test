/*=============Version==================

Version 1.0    20160622
 * Draft version

Version 1.2    20161221 
 * version ID    12 

Version 1.3    20170223 
 *  version ID   13 

Version 1.4    20170302 
 * version ID    14¡ê? 

 Version 1.5    20170308 
 *  version ID   15¡ê? 
 
========================================*/

#ifndef _RDA5880U_H
#define _RDA5880U_H

#include "MsCommon.h"

#define TV_MN_LO    2200   //(MHZ)
#define TV_MN_IF     5100   //(MHZ)


#define TUNER_NAME            "RDA5880"
#define RDA5880U_PRODUCT_ID   0xFF02
#define RDA5880U_CHIP_ID_0    0x61
#define RDA_VCOBIT            (1380000)
#define RDA_SW_VERSION        0x15

typedef struct
{
    MS_U32 add;
    MS_U16 wdata;
    MS_U16 delay;
} RDA_TVM_REG;

typedef enum
{
    FREQ_LO_INDEX_2 = 0,
    FREQ_LO_INDEX_4,
    FREQ_LO_INDEX_8,
    FREQ_LO_INDEX_16,
    FREQ_LO_INDEX_32,
    FREQ_LO_INDEX_QTY
} RDA_TV_FREQ_LO_INDEX;

typedef enum
{
    RDA_DK = 0,
    RDA_I,    // 1
    RDA_GH,
    RDA_B,
    RDA_N,
    RDA_M,
    RDA_L,
    RDA_L1,
    RDA_DTV_6M,    //8
    RDA_DTV_7M,
    RDA_DTV_8M,
    RDA_TV_MODE_QTY = 11
} RDA_TV_MODE;

typedef enum
{
    mauto = 0,
    manual
} RDA_TV_IFOUT_MODE;

typedef enum
{
    ADC_152 = 1,
    ADC_162 = 0,
    ADC_172 = 2
} RDA_TV_ADC_INDEX;

/************************************************************
ATV/DTMB/ATSC
|   -
DVB_T/DVB_T2/ISBT

DVB_C
************************************************************/
typedef enum
{
    LNA_SEL_L0 = 0xff,
    LNA_SEL_L1,    //LNA_SEL_1000_580
    LNA_SEL_L2,    //LNA_SEL_580_290
    LNA_SEL_L3,    //LNA_SEL_290_133
    LNA_SEL_L4      //LNA_SEL_133_45
} RDA_TV_LNA_SELECT;

typedef enum
{
    X = 0,
    MST_QTY = 1
} RDA_TV_DEMOD_MACH;

typedef enum
{
    TUNER_OUT_MODE_IF,
    TUNER_OUT_MODE_BB
} RDA_OUT_OUT_MODE;

typedef enum
{
    LNA_ATSC_MODE,
    LNA_DTMB_MODE,
    LNA_ATV_MODE,
    LNA_DVBT_MODE,
    LNA_ISDBT_MODE,
    LNA_DVBC_MODE
} RDA_MODULUTION_MODE;

struct rda_tuner_cfg
{
    RDA_TV_FREQ_LO_INDEX freq_lo_index;
    RDA_TV_ADC_INDEX freq_adc_index;
    RDA_OUT_OUT_MODE out_mode;
    RDA_TV_MODE tv_mode_index;
    RDA_TV_LNA_SELECT lna_index;
    RDA_TV_DEMOD_MACH demod_mach;
    RDA_MODULUTION_MODE mdlt_mode;

    MS_U16 lna_cap;
    MS_U16 xtal;
    MS_U16 i2vth_dtvh;
    MS_U16 i2vth_dtvl;
    MS_U32 freq;
    MS_U32 freq_lo;
    MS_U32 freq_vco_core;
    MS_U32 freq_if_out;
    MS_U16 LNA_Sel_off;
};

typedef struct rda_tuner_cfg *RDA_TUNER_CFG;

typedef struct
{
    MS_U32 freq;
    MS_U16 delta;
    RDA_TV_ADC_INDEX adc;
} RDA_TV_ADC_BY_FRE;

void rdaTVSoftwareVersion(MS_U8 u8TunerIndex);
void rdaTVInitial(MS_U8 u8TunerIndex, RDA_TV_DEMOD_MACH mDemod, RDA_MODULUTION_MODE mmdlt_mode);
void rdaTVTunerOpen(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg);
void rdaTVAGC(MS_U8 u8TunerIndex);
void rdaTVIQSwap(MS_U8 u8TunerIndex, MS_U8 flag);
void rdaDTVBW_8M(MS_U8 u8TunerIndex);
void rdaDTVBW_7M(MS_U8 u8TunerIndex);
void rdaDTVBW_6M(MS_U8 u8TunerIndex);
void rdaTVAGCReset(MS_U8 u8TunerIndex);
void rdaTVSetFreq_tune(MS_U8 u8TunerIndex, MS_U32 freq, MS_U32 bw, RDA_TUNER_CFG tuner_cfg);
void rdaTVFreqloIndexSet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg);
void rdaTVModeIndexSet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg, RDA_TV_MODE mode);
MS_U32 rdaTVMulDivCompensate(MS_U8 u8TunerIndex, MS_U32 par1, MS_U32 par2, MS_U32 par3);
RDA_TV_ADC_INDEX rdaTVGetADCbyFreqMode(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg);
void rdaTVDtvFreqTuneSet(MS_U8 u8TunerIndex, MS_U32 freq, MS_U16 eBand);
MS_U32 rdaTVGetFreq_lo(MS_U8 u8TunerIndex);
MS_U16 rdaTVLNACapGet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg);
MS_U32 rdaTVDetected(MS_U8 u8TunerIndex);
void rdaTVSetAGCMode(MS_U8 u8TunerIndex);
void rdaTVSetLoopSta(MS_U8 u8TunerIndex);
void rdaTVLNAIndexGet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg);
void rdaTVPllCalSet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg);
void rdaTVRFPLLSDMSet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg);
void rdaTVTunerModeSet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg);
void rdaTVRfBpset(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg, MS_U8 s_value);
void rdaTVIfOutSet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg);
void rdaTVI2vTh(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg);
void rdaTVBBPLLSDMSet(MS_U8 u8TunerIndex, RDA_TUNER_CFG tuner_cfg);
void rdaDemodFirmwareDL(MS_U8 u8TunerIndex);
void rdaTVSleepnoloop(MS_U8 u8TunerIndex);
void rdaTVSleepwithloop(MS_U8 u8TunerIndex);
MS_U16 rdaTVSingalStrength(MS_U8 u8TunerIndex);
void rdaTV8051Init(MS_U8 u8TunerIndex);
void rdaTV8051Start(MS_U8 u8TunerIndex);
void rdaTV8051Stop(MS_U8 u8TunerIndex);
MS_U16 i2cGetData(MS_U8 u8TunerIndex, MS_U8 addr);
MS_BOOL i2cSendData(MS_U8 u8TunerIndex, MS_U8 addr, MS_U16 wdata);
void RDA_DELAY_MS(MS_U32 cnt);

#endif

