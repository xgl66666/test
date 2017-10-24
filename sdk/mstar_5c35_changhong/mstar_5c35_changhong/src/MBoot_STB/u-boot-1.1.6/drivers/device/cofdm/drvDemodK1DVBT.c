

#if (ENABLE_HUMMINGBIRD_CUSTOMER)
#include <frontend_config.h>

#if (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT51_DEMOD)
//#include "MsCommon.h"
//#include "HbCommon.h"
#include "sys/drv/drvDMD_INTERN_DVBT.h"
#include "device/drvDemod.h"
#include "sys/drv/drvDMD_VD_MBX.h"


#include "sys/drv/drvSAR.h"
#include "device/drvTuner_MXL5007T.h"
//#include "drvTuner.h"
//#include <math.h>
#define TPS_timeout   2000
#define FEC_timeout   5000


#define COFDMDMD_MUTEX_TIMEOUT       (2000)

#ifdef MS_DEBUG
#define DBG_MSB(x)  x
#else
#define DBG_MSB(x)
#endif

static MS_BOOL bInited = FALSE;
//static MS_S32 _s32MutexId = -1;

//extern MS_S32 _u32LockTimeMax;
#ifdef MS_DEBUG

#define DBG_MSB(x)  x
#else
#define DBG_MSB(x)
#endif

#define DEMOD_IQ_I_IN 0
#define DEMOD_IQ_Q_IN 1
//||MS_BOARD_TYPE_SEL =BD_MST124B_D01A_S
#if (MS_BOARD_TYPE_SEL == BD_MST124SZ||MS_BOARD_TYPE_SEL == BD_MST124B_D01A_S)
//#error here
#define FRONTEND_DEMOD_IQ_TYPE      DEMOD_IQ_Q_IN // C:Q T:I
#else
#define FRONTEND_DEMOD_IQ_TYPE      DEMOD_IQ_I_IN // C:Q T:I
#endif

static  double pow( double d, int n)
{
    int s;
    double r = 1;

    if( n == 0 )
     return 1;

    if( n < 0 )
     s = 1 , n = -n;
    else
     s = 0;

    while( n != 1 )
    {
     if( n&1 ) r*=d;
     n /= 2;
     d *= d;
    }

    r *= d;
    return s?1/r:r;
}



static DMD_RFAGC_SSI ALPS_TUNER_RfagcSsi[] =
{    //for mxl5007 tuner setting
   {0,    0xff},  //default setting bryan add for MXL5007
    {0,    0xff},  //default setting

    /*  bryan mark 110517
        {-15.00,    0x19},
        {-25.00,    0x31},
        {-26.00,    0x33},
        {-27.00,    0x35},
        {-28.00,    0x35},
        {-29.00,    0x36},
        {-30.00,    0x37},
        {-31.00,    0x38},
        {-32.00,    0x3A},
        {-33.00,    0x3E},
        {-34.00,    0x40},
        {-35.00,    0x41},
        {-36.00,    0x43},
        {-37.00,    0x44},
        {-38.00,    0x46},
        {-39.00,    0x47},
        {-40.00,    0x49},
        {-41.00,    0x4B},
        {-42.00,    0x4E},
        {-43.00,    0x50},
        {-44.00,    0x53},
        {-45.00,    0x56},
        {-46.00,    0x59},
        {-46.50,    0x5B},
        {-47.00,    0x5D},
        {-48.00,    0x62},
        {-49.00,    0x67},
        {-50.00,    0x6B},
        {-51.00,    0x73},
        {-52.00,    0x7A},
        {-53.00,    0x85},
        {-53.50,    0x8E},
        {-54.00,    0x98},
        {-54.10,    0x9F},
        {-54.20,    0xA4},
        {-54.30,    0xA7},
        {-54.40,    0xAC},
        {-55.00,    0xAC},
        {-55.00,    0xFF},
    */
};

static DMD_IFAGC_SSI ALPS_TUNER_IfagcSsi_LoRef[] =
{
    //for mxl5007 tuner setting
    //current agc ref setting:150
    {10.0,     0x00},  //default setting
    {10.0,     0x00},  //default setting
    {-00.0,    0x31},
    {-05.0,    0x42},
    {-10.0,    0x57},
    {-15.0,    0x6e},
    {-20.0,    0x80},
    {-25.0,    0x87},
    {-30.0,    0x8d},
    {-35.0,    0x93},
    {-40.0,    0x98},
    {-50.0,    0x9d},
    {-55.0,    0xa3},
    {-60.0,    0xa8},
    {-65.0,    0xad},
    {-70.0,    0xb2},
    {-75.0,    0xb7},
    {-80.0,    0xbc},
    {-85.0,    0xc1},
    {-90.0,    0xc6},
    {-100.0,    0xff}, //end setting
    /*  bryan mark 110517
        {-54.30,    0x5F},
        {-54.40,    0x60},
        {-55.00,    0x63},
        {-56.00,    0x68},
        {-57.50,    0x6C},
        {-58.00,    0x70},
        {-59.00,    0x75},
        {-60.00,    0x79},
        {-61.40,    0x7D},
        {-62.00,    0x80},
        {-63.00,    0x81},
        {-64.00,    0x83},
        {-65.00,    0x84},
        {-66.00,    0x85},
        {-67.00,    0x87},
        {-68.00,    0x88},
        {-69.00,    0x89},
        {-70.00,    0x8A},
        {-71.00,    0x8C},
        {-72.00,    0x8D},
        {-73.00,    0x8F},
        {-74.00,    0x90},
        {-75.00,    0x92},
        {-76.00,    0x93},
        {-77.00,    0x95},
        {-78.00,    0x96},
        {-79.00,    0x98},
        {-80.00,    0x99},
        {-81.00,    0x9B},
        {-82.00,    0x9D},
        {-83.00,    0x9E},
        {-84.00,    0xA0},
        {-85.00,    0xA2},
        {-86.00,    0xA4},
        {-87.00,    0xA6},
        {-88.00,    0xA8},
        {-89.00,    0xAA},
        {-90.00,    0xAC},
        {-91.00,    0xAD},
        {-92.00,    0xAF},
        {-93.00,    0xB0},
        {-93.00,    0xFF},
    */
};


static DMD_IFAGC_SSI ALPS_TUNER_IfagcSsi_HiRef[] =
{
    {-54.30,    0x5F},
    {-54.40,    0x60},
    {-55.00,    0x63},
    {-56.00,    0x68},
    {-57.50,    0x6C},
    {-58.00,    0x70},
    {-59.00,    0x75},
    {-60.00,    0x79},
    {-61.40,    0x7D},
    {-62.00,    0x80},
    {-63.00,    0x81},
    {-64.00,    0x83},
    {-65.00,    0x84},
    {-66.00,    0x85},
    {-67.00,    0x87},
    {-68.00,    0x88},
    {-69.00,    0x89},
    {-70.00,    0x8A},
    {-71.00,    0x8C},
    {-72.00,    0x8D},
    {-73.00,    0x8F},
    {-74.00,    0x90},
    {-75.00,    0x92},
    {-76.00,    0x93},
    {-77.00,    0x95},
    {-78.00,    0x96},
    {-79.00,    0x98},
    {-80.00,    0x99},
    {-81.00,    0x9B},
    {-82.00,    0x9D},
    {-83.00,    0x9E},
    {-84.00,    0xA0},
    {-85.00,    0xA2},
    {-86.00,    0xA4},
    {-87.00,    0xA6},
    {-88.00,    0xA8},
    {-89.00,    0xAA},
    {-90.00,    0xAC},
    {-91.00,    0xAD},
    {-92.00,    0xAF},
    {-93.00,    0xB0},
    {-93.00,    0xFF},
};

static DMD_IFAGC_ERR ALPS_TUNER_IfagcErr_LoRef[] =
{
    {0.00,        0x0A},
    {0.00,        0xFF},

};

static DMD_IFAGC_ERR ALPS_TUNER_IfagcErr_HiRef[] =
{
    {0.00,        0x2D},
    {0.00,        0xFF},        // stopper
};

static DMD_SQI_CN_NORDIGP1 SqiCnNordigP1[] =
{
    {_QPSK,  _CR1Y2, 5.1 },
    {_QPSK,  _CR2Y3, 6.9 },
    {_QPSK,  _CR3Y4, 7.9 },
    {_QPSK,  _CR5Y6, 8.9 },
    {_QPSK,  _CR7Y8, 9.7 },
    {_16QAM, _CR1Y2, 10.8},
    {_16QAM, _CR2Y3, 13.1},
    {_16QAM, _CR3Y4, 12.2},
    {_16QAM, _CR5Y6, 15.6},
    {_16QAM, _CR7Y8, 16.0},
    {_64QAM, _CR1Y2, 16.5},
    {_64QAM, _CR2Y3, 16.3},
    {_64QAM, _CR3Y4, 17.8},
    {_64QAM, _CR3Y4, 21.2},
    {_64QAM, _CR5Y6, 21.6},
    {_64QAM, _CR7Y8, 22.5},
};
MS_BOOL MDrv_Demod_Init(void)
{
    DMD_DVBT_InitData sDMD_DVBT_InitData;
    MS_BOOL ret;
/*
    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"OfDmd_Mutex", MSOS_PROCESS_SHARED);

        if (_s32MutexId < 0)
        {
           // GEN_EXCEP;
            return FALSE;
        }

    }
*/
    MDrv_SYS_DMD_VD_MBX_Init();
    MDrv_SAR_Kpd_Init();
#if((FRONTEND_TUNER_TYPE == MXL_MXL5007_TUNER)||(FRONTEND_TUNER_TYPE == TUNER_RT810)||(FRONTEND_TUNER_TYPE == TUNER_RT808)|| \
    (FRONTEND_TUNER_TYPE == TUNER_MXD1516) ||(FRONTEND_TUNER_TYPE == TUNER_RT820T))
//#error here
static MS_U8 u8DMD_DVBT_DSPRegInitExt[]= {
            1, // version, should be matched with library
            0, // reserved

            E_DMD_DVBT_CFG_FC_L,    // Addr_L
            E_DMD_DVBT_CFG_FC_L>>8, // Addr_H
            0xFF, // Mask
            0xC7,
            E_DMD_DVBT_CFG_FC_H,    // Addr_L
            E_DMD_DVBT_CFG_FC_H>>8, // Addr_H
            0xFF, // Mask
            0x9F,
            E_DMD_DVBT_CFG_IQ_SWAP,    // Addr_L
            E_DMD_DVBT_CFG_IQ_SWAP>>8, // Addr_H

            0xFF, // Mask
            0x01,
            E_DMD_DVBT_CFG_ZIF,
            E_DMD_DVBT_CFG_ZIF>>8,
            0xff,
#if(FRONTEND_TUNER_TYPE == TUNER_MXD1516)
            0x01    //en zif
#else
            0x00                                   //0x01 zif en, 0x00 zif disable
#endif
           }; // Value
#elif(FRONTEND_TUNER_TYPE == TUNER_MXL603 || FRONTEND_TUNER_TYPE == TUNER_MXL608)     //IF:5M
    static MS_U8 u8DMD_DVBT_DSPRegInitExt[]= {
            1, // version, should be matched with library
            0, // reserved

            E_DMD_DVBT_CFG_FC_L,    // Addr_L
            E_DMD_DVBT_CFG_FC_L>>8, // Addr_H
            0xFF, // Mask
            0x1A,
            E_DMD_DVBT_CFG_FC_H,    // Addr_L
            E_DMD_DVBT_CFG_FC_H>>8, // Addr_H
            0xFF, // Mask
            0x9E,
            E_DMD_DVBT_CFG_IQ_SWAP,    // Addr_L
            E_DMD_DVBT_CFG_IQ_SWAP>>8, // Addr_H

            0xFF, // Mask
            0x01,
            E_DMD_DVBT_CFG_ZIF,
            E_DMD_DVBT_CFG_ZIF>>8,
            0xff,
            0x00                                   //0x01 zif en, 0x00 zif disable
           }; // Value
 #elif(FRONTEND_TUNER_TYPE==TUNER_APLS_TDAD3C02A)
    static MS_U8 u8DMD_DVBT_DSPRegInitExt[]= {
            1, // version, should be matched with library
            0, // reserved

            E_DMD_DVBT_CFG_FC_L,    // Addr_L
            E_DMD_DVBT_CFG_FC_L>>8, // Addr_H
            0xFF, // Mask
            0xb5,
            E_DMD_DVBT_CFG_FC_H,    // Addr_L
            E_DMD_DVBT_CFG_FC_H>>8, // Addr_H
            0xFF, // Mask
            0x24,
            E_DMD_DVBT_CFG_IQ_SWAP,    // Addr_L
            E_DMD_DVBT_CFG_IQ_SWAP>>8, // Addr_H

            0xFF, // Mask
            0x01,
            E_DMD_DVBT_CFG_ZIF,
            E_DMD_DVBT_CFG_ZIF>>8,
            0xff,
            0x00                                   //0x01 zif en, 0x00 zif disable
                                             }; // Value
#elif(FRONTEND_TUNER_TYPE == TUNER_RDA5880)
    // default is 8M Bandwidth
    // 45473-36000 = 0x2501
    static MS_U8 u8DMD_DVBT_DSPRegInitExt[]= {
            1, // version, should be matched with library
            0, // reserved

            E_DMD_DVBT_CFG_FC_L,    // Addr_L
            E_DMD_DVBT_CFG_FC_L>>8, // Addr_H
            0xFF, // Mask
            0x01,//[FC LOW BYTE,]
            E_DMD_DVBT_CFG_FC_H,    // Addr_L
            E_DMD_DVBT_CFG_FC_H>>8, // Addr_H
            0xFF, // Mask
            0x25,//[FC HIGH BYTE,]
            E_DMD_DVBT_CFG_IQ_SWAP,    // Addr_L
            E_DMD_DVBT_CFG_IQ_SWAP>>8, // Addr_H

            0xFF, // Mask
            0x00,
                                             }; // Value
#elif((FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18219)||(FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18212))
    // default is 8M Bandwidth
    // 8MBW: 4M IF  45473 - 4000 = 0xA201
    // 7MBW: 3.5M IF  45473 - 3500 = 0xA3F5
    // 6MBW: 3.5M IF  45473 - 3000 = 0xA4EF
    static MS_U8 u8DMD_DVBT_DSPRegInitExt[]= {
            1, // version, should be matched with library
            0, // reserved

            E_DMD_DVBT_CFG_FC_L,    // Addr_L
            E_DMD_DVBT_CFG_FC_L>>8, // Addr_H
            0xFF, // Mask
            0x01,//0xC7,
            E_DMD_DVBT_CFG_FC_H,    // Addr_L
            E_DMD_DVBT_CFG_FC_H>>8, // Addr_H
            0xFF, // Mask
            0xA2,//0x9F,
            E_DMD_DVBT_CFG_IQ_SWAP,    // Addr_L
            E_DMD_DVBT_CFG_IQ_SWAP>>8, // Addr_H

            0xFF, // Mask
            0x00,
            E_DMD_DVBT_CFG_ZIF,
            E_DMD_DVBT_CFG_ZIF>>8,
            0xff,
            0x00                                //0x01 zif en, 0x00 zif disable
                                                  }; // Value
#endif
#if 0
    static MS_U8 u8DMD_DVBT_InitExt[]= {
                                            2, // version
                                            0, // reserved
                                            0x15, // TS_CLK
                                            1
                                       }; // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
#else
    static MS_U8 u8DMD_DVBT_InitExt[]= { 4, // version
                                         0, // reserved
                                         0x15, // TS_CLK
                                         1 , // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
#if(FRONTEND_TUNER_TYPE == TUNER_RDA5880)
                                         (MS_U8)(36000>>24), // IF Frequency
                                         (MS_U8)(36000>>16),
                                         (MS_U8)(36000>>8),
                                         (MS_U8)(36000>>0),
#elif(FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18219)
                                         (MS_U8)(3950>>24), // IF Frequency
                                         (MS_U8)(3950>>16),
                                         (MS_U8)(3950>>8),
                                         (MS_U8)(3950>>0),
#elif(FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18212)
                                         (MS_U8)(4000>>24), // IF Frequency
                                         (MS_U8)(4000>>16),
                                         (MS_U8)(4000>>8),
                                         (MS_U8)(4000>>0),
#elif(FRONTEND_TUNER_TYPE == TUNER_MXD1516)
                                         (MS_U8)(48000>>24), // IF Frequency
                                         (MS_U8)(48000>>16),
                                         (MS_U8)(48000>>8),
                                         (MS_U8)(48000>>0),
#elif(FRONTEND_TUNER_TYPE==TUNER_APLS_TDAD3C02A)
                                         (MS_U8)(36166>>24), // IF Frequency
                                         (MS_U8)(36166>>16),
                                         (MS_U8)(36166>>8),
                                         (MS_U8)(36166>>0),
#elif(FRONTEND_TUNER_TYPE == TUNER_MXL603 || FRONTEND_TUNER_TYPE == TUNER_MXL608)     //IF:5M
                                         (MS_U8)(5000>>24), // IF Frequency
                                         (MS_U8)(5000>>16),
                                         (MS_U8)(5000>>8),
                                         (MS_U8)(5000>>0),
#else
                                         (MS_U8)(4570>>24), // IF Frequency
                                         (MS_U8)(4570>>16),
                                         (MS_U8)(4570>>8),
                                         (MS_U8)(4570>>0),
#endif
#if(FRONTEND_TUNER_TYPE == TUNER_MXD1516)
                                         (MS_U8)(48000>>24), // FS Frequency
                                         (MS_U8)(48000>>16),
                                         (MS_U8)(48000>>8),
                                         (MS_U8)(48000>>0),
#else
                                         (MS_U8)(45474>>24), // FS Frequency
                                         (MS_U8)(45474>>16),
                                         (MS_U8)(45474>>8),
                                         (MS_U8)(45474>>0),
#endif
#if((FRONTEND_TUNER_TYPE == TUNER_RT810)||(FRONTEND_TUNER_TYPE == TUNER_MXD1516) \
        ||(FRONTEND_TUNER_TYPE==TUNER_APLS_TDAD3C02A)||(FRONTEND_TUNER_TYPE == TUNER_RT820T))
                                         0,
#else
                                         1, // IQ Swap
#endif
#if(FRONTEND_TUNER_TYPE == TUNER_MXD1516)
                                         2,// u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
#else
                                         FRONTEND_DEMOD_IQ_TYPE, // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
#endif
                                         0, // u8PadSel : 0=Normal, 1=analog pad
                                         0, // bPGAEnable : 0=disable, 1=enable
                                         5, // u8PGAGain : default 5
                                         (MS_U8)(TPS_timeout>>8), // TPS timeout 700ms~
                                         (MS_U8)(TPS_timeout>>0),
                                         (MS_U8)(FEC_timeout>>8), // FEC timeout 6000ms~
                                         (MS_U8)(FEC_timeout>>0),
                                       };

#endif

    // tuner parameter
    sDMD_DVBT_InitData.u8SarChannel=1; // 0xFF means un-connected
    sDMD_DVBT_InitData.pTuner_RfagcSsi=ALPS_TUNER_RfagcSsi;
    sDMD_DVBT_InitData.u16Tuner_RfagcSsi_Size=sizeof(ALPS_TUNER_RfagcSsi)/sizeof(DMD_RFAGC_SSI);
    sDMD_DVBT_InitData.pTuner_IfagcSsi_LoRef=ALPS_TUNER_IfagcSsi_LoRef;
    sDMD_DVBT_InitData.u16Tuner_IfagcSsi_LoRef_Size=sizeof(ALPS_TUNER_IfagcSsi_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT_InitData.pTuner_IfagcSsi_HiRef=ALPS_TUNER_IfagcSsi_HiRef;
    sDMD_DVBT_InitData.u16Tuner_IfagcSsi_HiRef_Size=sizeof(ALPS_TUNER_IfagcSsi_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT_InitData.pTuner_IfagcErr_LoRef=ALPS_TUNER_IfagcErr_LoRef;
    sDMD_DVBT_InitData.u16Tuner_IfagcErr_LoRef_Size=sizeof(ALPS_TUNER_IfagcErr_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT_InitData.pTuner_IfagcErr_HiRef=ALPS_TUNER_IfagcErr_HiRef;
    sDMD_DVBT_InitData.u16Tuner_IfagcErr_HiRef_Size=sizeof(ALPS_TUNER_IfagcErr_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT_InitData.pSqiCnNordigP1=SqiCnNordigP1;
    sDMD_DVBT_InitData.u16SqiCnNordigP1_Size=sizeof(SqiCnNordigP1)/sizeof(DMD_SQI_CN_NORDIGP1);

    // register init
    sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt=u8DMD_DVBT_DSPRegInitExt; // TODO use system variable type
    sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitSize=(sizeof(u8DMD_DVBT_DSPRegInitExt)-2)/4;
    sDMD_DVBT_InitData.u8DMD_DVBT_InitExt=u8DMD_DVBT_InitExt; // TODO use system variable type
    ret = MDrv_DMD_DVBT_Init(&sDMD_DVBT_InitData, sizeof(sDMD_DVBT_InitData)); // _UTOPIA
    if(ret == TRUE)
    {
        bInited = TRUE;
    }

    return ret;

}

MS_BOOL MDrv_Demod_Open(void)
{
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
*/
    return TRUE;
}

MS_BOOL MDrv_Demod_Close(void)
{
    //MS_BOOL ret;
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    ret = MDrv_DMD_DVBT_Exit();
    if(ret == TRUE)
    {
        bInited = FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
*/
    return TRUE;
}

MS_BOOL MDrv_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
*/
    return TRUE;
}
MS_BOOL MDrv_Demod_GetRfOffset(float *poffset)
{
    MS_BOOL ret;
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
*/
    if(bInited == FALSE)
    {
        *poffset = 0;
       // HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }


    ret = MDrv_DMD_DVBT_GetFreqOffset(poffset);

    //HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_GetLock(MS_BOOL *pbLock)
{
    DMD_LOCK_STATUS LockStatus;
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
*/

    if(bInited == FALSE)
    {
        *pbLock = FALSE;

        //HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    MDrv_DMD_DVBT_GetLock(E_DMD_DMD_DVBT_GETLOCK, &LockStatus);
    switch (LockStatus)
    {
    case E_DMD_LOCK:
        *pbLock = TRUE;
        //_u32LockTimeMax = FEC_timeout;
        break;
    case E_DMD_UNLOCK:
        *pbLock = FALSE;
       // _u32LockTimeMax = TPS_timeout;
        break;
    default:
        *pbLock = FALSE;
        break;
    }

    //HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

MS_BOOL MDrv_Demod_GetSNR(MS_U32 *pu32SNR)
{
    MS_BOOL ret;
    MS_U16 u16Snr =0;
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
*/
    if(bInited == FALSE)
    {
        *pu32SNR = 0;
  //      HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBT_GetSignalQuality(&u16Snr);
    if(u16Snr > 100)
         u16Snr = 100;
    *pu32SNR = u16Snr;
    //HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_GetBER(float *pfBER)
{
    MS_BOOL ret;
    float ber;
    ber = 0;

   /* if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }*/

    if(bInited == FALSE)
    {
        *pfBER = 0;
       // HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBT_GetPostViterbiBer(&ber);
   *pfBER = ber;

   // HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_GetPWR(MS_S32 *ps32Signal)
{
    MS_BOOL ret;
    MS_U16 u16Signal=0;
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
*/
    if(bInited == FALSE)
    {
        *ps32Signal = 0;
       // HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBT_GetSignalStrength(&u16Signal);
    if(u16Signal > 100)
    {
       u16Signal = 100;
    }
    *ps32Signal = u16Signal;
   // HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_Config(MS_U8 *pRegParam)
{
   /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
   */
    return TRUE;
}

MS_BOOL MDrv_Demod_GetParam(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    */
    return TRUE;
}

MS_BOOL MDrv_Demod_Restart(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MS_BOOL bPalBG = FALSE; //unknown variable
    static DEMOD_EN_TER_BW_MODE cur_BW = 0xff;

    /*if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
     */
    if(bInited == FALSE)
    {
       // HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    //printf("------pParam->u32Frequency [%d] pParam->TerParam.eBandWidth[%d]------\n",pParam->u32Frequency,pParam->TerParam.eBandWidth);
    MDrv_Tuner_SetTuner( pParam->u32Frequency, pParam->TerParam.eBandWidth);

    //if(cur_BW != pParam->TerParam.eBandWidth)
    {
        switch (pParam->TerParam.eBandWidth)
        {
        case DEMOD_BW_MODE_6MHZ:
            MDrv_DMD_DVBT_SetConfig(E_DMD_RF_CH_BAND_6MHz, FALSE, bPalBG ); // _UTOPIA
#if(FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18219)
            MDrv_DMD_DVBT_SetConfigHPLPSetIF(E_DMD_RF_CH_BAND_6MHz,FALSE,bPalBG,0,3250,45474,1);
#elif(FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18212)
            MDrv_DMD_DVBT_SetConfigHPLPSetIF(E_DMD_RF_CH_BAND_6MHz,FALSE,bPalBG,0,3250,45474,1);
#endif
            break;
        case DEMOD_BW_MODE_7MHZ:
            MDrv_DMD_DVBT_SetConfig(E_DMD_RF_CH_BAND_7MHz, FALSE, bPalBG ); // _UTOPIA
#if(FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18219)
            MDrv_DMD_DVBT_SetConfigHPLPSetIF(E_DMD_RF_CH_BAND_7MHz,FALSE,bPalBG,0,3450,45474,1);
#elif(FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18212)
            MDrv_DMD_DVBT_SetConfigHPLPSetIF(E_DMD_RF_CH_BAND_7MHz,FALSE,bPalBG,0,3500,45474,1);
#endif
            break;
        case DEMOD_BW_MODE_8MHZ:
            MDrv_DMD_DVBT_SetConfig(E_DMD_RF_CH_BAND_8MHz, FALSE, bPalBG ); // _UTOPIA
#if(FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18219)
            MDrv_DMD_DVBT_SetConfigHPLPSetIF(E_DMD_RF_CH_BAND_8MHz,FALSE,bPalBG,0,3950,45474,1);
#elif(FRONTEND_TUNER_TYPE == TUNER_NXP_TDA18212)
            MDrv_DMD_DVBT_SetConfigHPLPSetIF(E_DMD_RF_CH_BAND_8MHz,FALSE,bPalBG,0,4000,45474,1);
#endif
            break;
        default:
            break;
        }

        if(FALSE == MDrv_DMD_DVBT_SetActive(TRUE)) // _UTOPIA
        {
          //  HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }

        cur_BW = pParam->TerParam.eBandWidth;

    }

    //HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_I2C_ByPass(MS_BOOL bOn)
{
    return TRUE;
}

MS_BOOL MDrv_Demod_SetMode(Demod_Mode* pMode)
{
#if 0
    DMD_Mode dmdMod;
    dmdMod.bX4CFE_en            = pMode->bX4CFE_en;
    dmdMod.bPPD_en              = pMode->bPPD_en;
    dmdMod.bIQAutoSwap_en       = pMode->bIQAutoSwap_en;
    dmdMod.bQAMScan_en          = pMode->bQAMScan_en;
    dmdMod.bFHO_en              = pMode->bFHO_en;
    dmdMod.fptTunerSet          = pMode->fptTunerSet;
    return MDrv_Dmd_SetMode((DMD_Mode*)pMode);
#else
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
*/
    return TRUE;
#endif
}

MS_BOOL MDrv_Demod_SetOutoutPath(DEMOD_INTERFACE_MODE path)
{
    //Dummy function
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
*/
    return TRUE;
}

DEMOD_INTERFACE_MODE MDrv_Demod_GetOutoutPath(void)
{
    //Dummy function
/*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
*/
    return DEMOD_INTERFACE_PARALLEL;
}

#define E 2.7
#define PRECISE 20
float LnPrecise(float x)
{

    float e_x=0.0;
    float temp =x;
    MS_S32 y=0,i,j;

    if((x>0) && (x<=2))
    {
        i=1;
        j=2;
        x=x-1;
        for(; (i<PRECISE) && (j<PRECISE); i+=2,j+=2)
        {
            e_x += pow(x,i)/i;
            e_x -= pow(x,j)/j;
        }
        return e_x;
    }

    while(temp>2)
    {
        temp /=E;
        y++;
    }
    return y+LnPrecise(x/pow(E,y));
}

float Log10Precise(float x)
{
    float a = LnPrecise(x);
    float b = LnPrecise(10);
    return  a/b;


}
MS_BOOL MApi_DMD_DVBT_GetSignalQuality(int *quality)
{
    float     fber;
    float     log_ber;
    DMD_LOCK_STATUS LockStatus;
    *quality = 0;
    if (MDrv_DMD_DVBT_GetPostViterbiBer(&fber) == FALSE)
    {
        DBG_MSB(printf("\nGetPostViterbiBer Fail!"));
        return FALSE;
    }
    else
    {

        MDrv_DMD_DVBT_GetLock(E_DMD_COFDM_PSYNC_LOCK, &LockStatus);
        switch (LockStatus)
        {
        case E_DMD_LOCK:
            LockStatus = TRUE;
            break;
        default:
            return FALSE;
            break;
        }
        if (LockStatus)
        {
            // log_ber = log10(fber)
            log_ber = Log10Precise(1 / fber); // Log10Approx() provide 1~2^32 input range only

            DBG_MSB(printf("\nLog(BER) = %f", log_ber));

            //printf("this is log_ber : %f fber:%.12f\n",log_ber,fber);
            if (log_ber > 6)                   // 10^(-6)
            {
                *quality = 100;
            }
            else if (log_ber > 4.30)             // 5*10^(-5)
            {
                *quality = 90 + (((-4.30) + log_ber) / ((-4.30) - (-6.0)) * (100 - 90));
            }
            else if (log_ber > 3.69)             // 2*10^(-4)
            {
                *quality = 75 + (((-3.69) + log_ber) / ((-3.69) - (-4.30)) * (90 - 75));
            }
            else if (log_ber > 2.52)            // 3*10^(-3)
            {
                *quality = 50 + (((-2.52) + log_ber) / ((-2.52) - (-3.69)) * (75 - 50));
            }
            else if (log_ber > 2.22)            // 6*10^(-3)//??
            {
                *quality = 30 + (((-2.22) + log_ber) / ((-2.22) - (-2.52)) * (50 - 30));
            }
            else if (log_ber > 2)           // 10^(-2)
            {
                *quality = 10 + (((-2) + log_ber) / ((-2) - (-2.22)) * (30 - 10));
            }
            else
            {
                *quality = 0;
            }

            //printf(" quality : %d\n",*quality);

        }
        else
        {
            *quality = 0;
        }
        //HB_printf("\nSignal Quality = %d", (int)*quality);

        return TRUE;
    }
}

MS_U16 MDrv_Demod_ReadReg(MS_U16 RegAddr)
{
    MS_U8 RegData = 0;
    MDrv_DMD_DVBT_GetReg(RegAddr, &RegData);
    printf("RegData: %x\n",RegData);
    return (MS_U16) RegData;
}

MS_BOOL MDrv_Demod_WriteReg(MS_U16 RegAddr, MS_U16 RegData)
{

    return MDrv_DMD_DVBT_SetReg(RegAddr, (MS_U8)RegData);
}

#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSDVBC)
////////////////////////////////////////////////////////////////////////////
#else // else ENABLE_HUMMINGBIRD_CUSTOMER

#if (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT51_DEMOD)
//#include "MsCommon.h"
//#include "HbCommon.h"
#include "sys/drv/drvDMD_INTERN_DVBT.h"
#include "device/drvDemod.h"
#include "sys/drv/drvDMD_VD_MBX.h"
#include "sys/drv/drvSAR.h"
#include "device/drvTuner_MXL5007T.h"
#define DEMOD_IQ_I_IN 0
#define DEMOD_IQ_Q_IN 1



#if (MS_BOARD_TYPE_SEL == BD_MST124SZ)
#define FRONTEND_DEMOD_IQ_TYPE      DEMOD_IQ_Q_IN // C:Q T:I
#else
#define FRONTEND_DEMOD_IQ_TYPE      DEMOD_IQ_I_IN // C:Q T:I
#endif


#define COFDMDMD_MUTEX_TIMEOUT       (2000)

#ifdef MS_DEBUG
#define DBG_MSB(x)  x
#else
#define DBG_MSB(x)
#endif

static MS_BOOL bInited = FALSE;
//static MS_S32 _s32MutexId = -1;

static DMD_RFAGC_SSI ALPS_TUNER_RfagcSsi[] =
{
        //for mxl5007 tuner setting
        {0,    0xff},  //default setting bryan add for MXL5007
    {0,    0xff},  //default setting
            /*  bryan mark 110517

    {-15.00,    0x19},
    {-25.00,    0x31},
    {-26.00,    0x33},
    {-27.00,    0x35},
    {-28.00,    0x35},
    {-29.00,    0x36},
    {-30.00,    0x37},
    {-31.00,    0x38},
    {-32.00,    0x3A},
    {-33.00,    0x3E},
    {-34.00,    0x40},
    {-35.00,    0x41},
    {-36.00,    0x43},
    {-37.00,    0x44},
    {-38.00,    0x46},
    {-39.00,    0x47},
    {-40.00,    0x49},
    {-41.00,    0x4B},
    {-42.00,    0x4E},
    {-43.00,    0x50},
    {-44.00,    0x53},
    {-45.00,    0x56},
    {-46.00,    0x59},
    {-46.50,    0x5B},
    {-47.00,    0x5D},
    {-48.00,    0x62},
    {-49.00,    0x67},
    {-50.00,    0x6B},
    {-51.00,    0x73},
    {-52.00,    0x7A},
    {-53.00,    0x85},
    {-53.50,    0x8E},
    {-54.00,    0x98},
    {-54.10,    0x9F},
    {-54.20,    0xA4},
    {-54.30,    0xA7},
    {-54.40,    0xAC},
    {-55.00,    0xAC},
    {-55.00,    0xFF},
    */
};

static DMD_IFAGC_SSI ALPS_TUNER_IfagcSsi_LoRef[] =
{
		//for mxl5007 tuner setting
	  //current agc ref setting:150
	  {10.0,     0x00},  //default setting
	  {10.0,     0x00},  //default setting
	  {-00.0,    0x31},
	  {-05.0,    0x42},
    {-10.0,    0x57},
	  {-15.0,    0x6e},
    {-20.0,    0x80},
	  {-25.0,    0x87},
    {-30.0,    0x8d},
    {-35.0,    0x93},
    {-40.0,    0x98},
	  {-45.0,    0x9d},
    {-50.0,    0xa3},
    {-55.0,    0xa8},
    {-60.0,    0xad},
    {-65.0,    0xb2},
    {-70.0,    0xb7},
    {-75.0,    0xbc},
    {-80.0,    0xc1},
    {-85.0,    0xc6},
    {-90.0,    0xff}, //end setting
            /*  bryan mark 110517

    {-54.30,    0x5F},
    {-54.40,    0x60},
    {-55.00,    0x63},
    {-56.00,    0x68},
    {-57.50,    0x6C},
	{-58.00,    0x70},
    {-59.00,    0x75},
    {-60.00,    0x79},
    {-61.40,    0x7D},
    {-62.00,    0x80},
    {-63.00,    0x81},
    {-64.00,    0x83},
    {-65.00,    0x84},
    {-66.00,    0x85},
    {-67.00,    0x87},
    {-68.00,    0x88},
    {-69.00,    0x89},
    {-70.00,    0x8A},
    {-71.00,    0x8C},
    {-72.00,    0x8D},
    {-73.00,    0x8F},
    {-74.00,    0x90},
    {-75.00,    0x92},
    {-76.00,    0x93},
    {-77.00,    0x95},
    {-78.00,    0x96},
    {-79.00,    0x98},
    {-80.00,    0x99},
    {-81.00,    0x9B},
    {-82.00,    0x9D},
    {-83.00,    0x9E},
    {-84.00,    0xA0},
    {-85.00,    0xA2},
    {-86.00,    0xA4},
    {-87.00,    0xA6},
    {-88.00,    0xA8},
    {-89.00,    0xAA},
    {-90.00,    0xAC},
    {-91.00,    0xAD},
    {-92.00,    0xAF},
    {-93.00,    0xB0},
    {-93.00,    0xFF},
    */
};

static DMD_IFAGC_SSI ALPS_TUNER_IfagcSsi_HiRef[] =
{
    {-54.30,    0x5F},
    {-54.40,    0x60},
    {-55.00,    0x63},
    {-56.00,    0x68},
    {-57.50,    0x6C},
	{-58.00,    0x70},
    {-59.00,    0x75},
    {-60.00,    0x79},
    {-61.40,    0x7D},
    {-62.00,    0x80},
    {-63.00,    0x81},
    {-64.00,    0x83},
    {-65.00,    0x84},
    {-66.00,    0x85},
    {-67.00,    0x87},
    {-68.00,    0x88},
    {-69.00,    0x89},
    {-70.00,    0x8A},
    {-71.00,    0x8C},
    {-72.00,    0x8D},
    {-73.00,    0x8F},
    {-74.00,    0x90},
    {-75.00,    0x92},
    {-76.00,    0x93},
    {-77.00,    0x95},
    {-78.00,    0x96},
    {-79.00,    0x98},
    {-80.00,    0x99},
    {-81.00,    0x9B},
    {-82.00,    0x9D},
    {-83.00,    0x9E},
    {-84.00,    0xA0},
    {-85.00,    0xA2},
    {-86.00,    0xA4},
    {-87.00,    0xA6},
    {-88.00,    0xA8},
    {-89.00,    0xAA},
    {-90.00,    0xAC},
    {-91.00,    0xAD},
    {-92.00,    0xAF},
    {-93.00,    0xB0},
    {-93.00,    0xFF},
};

static DMD_IFAGC_ERR ALPS_TUNER_IfagcErr_LoRef[] =
{
    {0.00,        0x0A},
    {0.00,        0xFF},

};

static DMD_IFAGC_ERR ALPS_TUNER_IfagcErr_HiRef[] =
{
    {0.00,        0x2D},
    {0.00,        0xFF},        // stopper
};

static DMD_SQI_CN_NORDIGP1 SqiCnNordigP1[] =
{
    {_QPSK,  _CR1Y2, 5.1 },
    {_QPSK,  _CR2Y3, 6.9 },
    {_QPSK,  _CR3Y4, 7.9 },
    {_QPSK,  _CR5Y6, 8.9 },
    {_QPSK,  _CR7Y8, 9.7 },
    {_16QAM, _CR1Y2, 10.8},
    {_16QAM, _CR2Y3, 13.1},
    {_16QAM, _CR3Y4, 12.2},
    {_16QAM, _CR5Y6, 15.6},
    {_16QAM, _CR7Y8, 16.0},
    {_64QAM, _CR1Y2, 16.5},
    {_64QAM, _CR2Y3, 16.3},
    {_64QAM, _CR3Y4, 17.8},
	{_64QAM, _CR3Y4, 21.2},
    {_64QAM, _CR5Y6, 21.6},
    {_64QAM, _CR7Y8, 22.5},
};

MS_BOOL MDrv_Demod_Init(void)
{
    DMD_DVBT_InitData sDMD_DVBT_InitData;
    MS_BOOL ret;

    MDrv_SYS_DMD_VD_MBX_Init();


    MDrv_SAR_Kpd_Init();


    printf("------Power_On_Initialization------\n");


    static MS_U8 u8DMD_DVBT_DSPRegInitExt[]={	1, // version, should be matched with library
												0, // reserved

												E_DMD_DVBT_CFG_FC_L,    // Addr_L
												E_DMD_DVBT_CFG_FC_L>>8, // Addr_H
												0xFF, // Mask
												0xC7,
                                                E_DMD_DVBT_CFG_FC_H,    // Addr_L
                                                E_DMD_DVBT_CFG_FC_H>>8, // Addr_H
                                                0xFF, // Mask
                                                0x9F,
                                                E_DMD_DVBT_CFG_IQ_SWAP,    // Addr_L
                                                E_DMD_DVBT_CFG_IQ_SWAP>>8, // Addr_H

                                                0xFF, // Mask
												0x01, // 0x00=I path, 0x01=Q path
											}; // Value

	static MS_U8 u8DMD_DVBT_InitExt[]={	4, // version
        								0, // reserved
        								0x15, // TS_CLK
        								1 , // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
                                        (MS_U8)(4570>>24), // IF Frequency
                                        (MS_U8)(4570>>16),
                                        (MS_U8)(4570>>8),
                                        (MS_U8)(4570>>0),
                                        (MS_U8)(45474>>24), // FS Frequency
                                        (MS_U8)(45474>>16),
                                        (MS_U8)(45474>>8),
                                        (MS_U8)(45474>>0),
                                        1, // IQ Swap
                                        FRONTEND_DEMOD_IQ_TYPE, // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
                                        0, // u8PadSel : 0=Normal, 1=analog pad
                                        0, // bPGAEnable : 0=disable, 1=enable
                                        5, // u8PGAGain : default 5
                                        };

    // tuner parameter
    sDMD_DVBT_InitData.u8SarChannel=1; // 0xFF means un-connected
    sDMD_DVBT_InitData.pTuner_RfagcSsi=ALPS_TUNER_RfagcSsi;
    sDMD_DVBT_InitData.u16Tuner_RfagcSsi_Size=sizeof(ALPS_TUNER_RfagcSsi)/sizeof(DMD_RFAGC_SSI);
    sDMD_DVBT_InitData.pTuner_IfagcSsi_LoRef=ALPS_TUNER_IfagcSsi_LoRef;
    sDMD_DVBT_InitData.u16Tuner_IfagcSsi_LoRef_Size=sizeof(ALPS_TUNER_IfagcSsi_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT_InitData.pTuner_IfagcSsi_HiRef=ALPS_TUNER_IfagcSsi_HiRef;
    sDMD_DVBT_InitData.u16Tuner_IfagcSsi_HiRef_Size=sizeof(ALPS_TUNER_IfagcSsi_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT_InitData.pTuner_IfagcErr_LoRef=ALPS_TUNER_IfagcErr_LoRef;
    sDMD_DVBT_InitData.u16Tuner_IfagcErr_LoRef_Size=sizeof(ALPS_TUNER_IfagcErr_LoRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT_InitData.pTuner_IfagcErr_HiRef=ALPS_TUNER_IfagcErr_HiRef;
    sDMD_DVBT_InitData.u16Tuner_IfagcErr_HiRef_Size=sizeof(ALPS_TUNER_IfagcErr_HiRef)/sizeof(DMD_IFAGC_SSI);
    sDMD_DVBT_InitData.pSqiCnNordigP1=SqiCnNordigP1;
    sDMD_DVBT_InitData.u16SqiCnNordigP1_Size=sizeof(SqiCnNordigP1)/sizeof(DMD_SQI_CN_NORDIGP1);

                // register init
	sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt=u8DMD_DVBT_DSPRegInitExt; // TODO use system variable type
	sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitSize=(sizeof(u8DMD_DVBT_DSPRegInitExt)-2)/4;
	sDMD_DVBT_InitData.u8DMD_DVBT_InitExt=u8DMD_DVBT_InitExt; // TODO use system variable type
    ret = MDrv_DMD_DVBT_Init(&sDMD_DVBT_InitData, sizeof(sDMD_DVBT_InitData)); // _UTOPIA
    if(ret == TRUE)
    {
        bInited = TRUE;
    }

    return ret;

}

MS_BOOL MDrv_Demod_Open(void)
{
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    HB_ReleaseMutex(_s32MutexId);
    */
    return TRUE;
}

MS_BOOL MDrv_Demod_Close(void)
{
    MS_BOOL ret = 0;
   /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    ret = MDrv_DMD_DVBT_Exit();
    if(ret == TRUE)
    {
        bInited = FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    */
    return ret;
}

MS_BOOL MDrv_Demod_PowerOnOff(MS_BOOL bPowerOn)
{
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    */
    return TRUE;
}

MS_BOOL MDrv_Demod_GetLock(MS_BOOL *pbLock)
{
    DMD_LOCK_STATUS LockStatus;
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    */

    if(bInited == FALSE)
    {
        *pbLock = FALSE;

     //   HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    MDrv_DMD_DVBT_GetLock(E_DMD_DMD_DVBT_GETLOCK, &LockStatus);
    switch (LockStatus)
    {
        case E_DMD_LOCK:
            *pbLock = TRUE;
            break;
        case E_DMD_UNLOCK:
            *pbLock = FALSE;
            break;
        default:
            *pbLock = FALSE;
            break;
    }

    //HB_ReleaseMutex(_s32MutexId);

    return TRUE;
}

MS_BOOL MDrv_Demod_GetSNR(MS_U32 *pu32SNR)
{
    MS_BOOL ret;
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
   */
    if(bInited == FALSE)
    {
        *pu32SNR = 0;
    //     HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBT_GetSignalQuality((MS_U16 *)pu32SNR);

    //HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_GetBER(float *pfBER)
{
    MS_BOOL ret;

    /*if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    */
    if(bInited == FALSE)
    {
        *pfBER = 0;
    //    HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBT_GetPostViterbiBer(pfBER);
    //HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_GetPWR(MS_S32 *ps32Signal)
{
    MS_BOOL ret;
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    */
    if(bInited == FALSE)
    {
        *ps32Signal = 0;
    //    HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    ret = MDrv_DMD_DVBT_GetSignalStrength((MS_U16 *)ps32Signal);
    //HB_ReleaseMutex(_s32MutexId);
    return ret;
}

MS_BOOL MDrv_Demod_Config(MS_U8 *pRegParam)
{
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    */
    return TRUE;
}

MS_BOOL MDrv_Demod_GetParam(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    */
    return TRUE;
}

MS_BOOL MDrv_Demod_Restart(DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    MS_BOOL bPalBG = FALSE; //unknown variable

    /*if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }
    */
    if(bInited == FALSE)
    {
    //    HB_ReleaseMutex(_s32MutexId);
        return FALSE;
    }

    //printf("------pParam->u32Frequency [%d] pParam->TerParam.eBandWidth[%d]------\n",pParam->u32Frequency,pParam->TerParam.eBandWidth);
    MDrv_Tuner_SetTuner( pParam->u32Frequency, pParam->TerParam.eBandWidth);


        switch (pParam->TerParam.eBandWidth)
        {
            case DEMOD_BW_MODE_6MHZ:
                MDrv_DMD_DVBT_SetConfig(E_DMD_RF_CH_BAND_6MHz, FALSE, bPalBG ); // _UTOPIA
                break;
            case DEMOD_BW_MODE_7MHZ:
                MDrv_DMD_DVBT_SetConfig(E_DMD_RF_CH_BAND_7MHz, FALSE, bPalBG ); // _UTOPIA
                break;
            case DEMOD_BW_MODE_8MHZ:
                MDrv_DMD_DVBT_SetConfig(E_DMD_RF_CH_BAND_8MHz, FALSE, bPalBG ); // _UTOPIA
                break;
            default:
                break;
        }

        if(FALSE == MDrv_DMD_DVBT_SetActive(TRUE)) // _UTOPIA
        {
            //HB_ReleaseMutex(_s32MutexId);
            return FALSE;
        }

    //HB_ReleaseMutex(_s32MutexId);
    return TRUE;
}

MS_BOOL MDrv_Demod_SetMode(Demod_Mode* pMode)
{
#if 0
    DMD_Mode dmdMod;
    dmdMod.bX4CFE_en            = pMode->bX4CFE_en;
    dmdMod.bPPD_en              = pMode->bPPD_en;
    dmdMod.bIQAutoSwap_en       = pMode->bIQAutoSwap_en;
    dmdMod.bQAMScan_en          = pMode->bQAMScan_en;
    dmdMod.bFHO_en              = pMode->bFHO_en;
    dmdMod.fptTunerSet          = pMode->fptTunerSet;
    return MDrv_Dmd_SetMode((DMD_Mode*)pMode);
#else
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    */
    return TRUE;
#endif
}

MS_BOOL MDrv_Demod_SetOutoutPath(DEMOD_INTERFACE_MODE path)
{
    //Dummy function
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    */
    return TRUE;
}

DEMOD_INTERFACE_MODE MDrv_Demod_GetOutoutPath(void)
{
    //Dummy function
    /*
    if (HB_ObtainMutex(_s32MutexId, COFDMDMD_MUTEX_TIMEOUT) == FALSE)
    {
        DBG_MSB(HB_printf("%s: Obtain mutex failed.\n", __FUNCTION__));
        return FALSE;
    }

    HB_ReleaseMutex(_s32MutexId);
    */
    return DEMOD_INTERFACE_PARALLEL;
}

#endif // (FRONTEND_DEMOD_TYPE == DEMOD_MSDVBC)

#endif // end ENABLE_HUMMINGBIRD_CUSTOMER