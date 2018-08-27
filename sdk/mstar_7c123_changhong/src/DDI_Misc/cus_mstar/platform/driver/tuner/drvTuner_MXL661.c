
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/err.h>
#include <asm/uaccess.h>
#else
#include <stdio.h>
#endif
#include "error.h"
#include "MsCommon.h"
#include "drvTuner.h"
#include "drvTunerNull.h"
#include "drvIIC.h"


#if IF_THIS_TUNER_INUSE(TUNER_MXL661)
#include "MxL661_TunerApi.h"
#include "MxL661_OEM_Drv.h"
#include "MxL661_TunerCfg.h"
#include "drvTuner_MXL661.h"
#include <string.h>

#define EXAMPLE_DEV_MAX   2
//#define MXL661_I2C_ADDR   0xC0
#define MXL661_CHIP_ID 0x03//0x02

#define DTV_TUNER_IF_FREQ_KHz                 5000L

#define TUNER_PLL_STABLE_TIME             30

#define TUNER_DTV_IF_FREQ_KHz      DTV_TUNER_IF_FREQ_KHz

#define IF_OUT_CFG_GAIN_LEVEL_DVB        6 //4
#define IF_OUT_CFG_GAIN_LEVEL_DVB_T2     6
#define IF_OUT_CFG_GAIN_LEVEL_DTMB       10 // 15 //for external ATBM886x DTMB demod must use 15
#define IF_OUT_CFG_GAIN_LEVEL_ISDB       11 //13 update by sj.rao for RF
#define IF_OUT_CFG_GAIN_LEVEL_ATV_NORMAL 9 //8 //7
#define IF_OUT_CFG_GAIN_LEVEL_ATV_SCAN   9
#define IF_OUT_CFG_GAIN_LEVEL_ATV_SECAM  8
#define GLOBAL_VAR(p,i) *(p+i)
// Golbal Parameter
MXL661_COMMAND_T apiCmd;

MXL661_AGC_ID_E AGC_Pin_Internal;
MXL661_AGC_ID_E AGC_Pin_External;

MXL661_IF_FREQ_E IfFreqConversion(MS_U16 u16IfFreqKHz);
MS_BOOL ConfigAGCMode(MS_U8 u8TunerIndex, MS_U32 eMode);

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static MS_BOOL bEverDetect = FALSE;
static MS_U8 u8max_dev_num = 0;
static TUNER_MS_INIT_PARAM* pInitParam = NULL;
static MS_U8 u8Possible_SLAVE_IDs[4] = {0xC0, 0xC2, 0xC4, 0xC6};
static MS_U32* pu32LastBroadcastType = NULL;
static MS_U8* pu8LastOtherMode = NULL;
static MS_BOOL* pbLastInATVScanMode = NULL;
static MS_BOOL* pbNeedResetAGCMode = NULL;
static MS_BOOL* pbInATVFinetuneMode = NULL;
static MS_U32* pu32chantuneFreq = NULL;
static MS_U32* pu32GlobalATVRfFreqKHz = NULL;
static MS_BOOL* pbInATVScanMode = NULL;
static MS_U8* pu8curSignalMode = NULL;
static MS_U32 u32FreqBound[7] =
{
    MXL661_VHF_LOWMIN_FREQ,
    MXL661_VHF_LOWMAX_FREQ,
    MXL661_VHF_HIGHMIN_FREQ,
    MXL661_VHF_HIGHMAX_FREQ,
    MXL661_UHF_MIN_FREQ_UK,
    MXL661_UHF_MIN_FREQ,
    MXL661_UHF_MAX_FREQ
 };

static MS_BOOL _variable_alloc(void** pp, MS_U32 size)
{
    if(NULL == *pp)
    {
        *pp = malloc(size*u8max_dev_num);
        if(NULL == *pp)
        {
            return FALSE;
        }
        else
        {
           memset(*pp, 0, size*u8max_dev_num);
        }
    }
    return TRUE;
}

static void _variable_free(void** pp)
{
    if(NULL != *pp)
    {
        free(*pp);
        *pp = NULL;
    }
}

static MS_BOOL _variables_alloc(void)
{
    MS_BOOL bRet = TRUE;

    bRet &= _variable_alloc((void*)&pbLastInATVScanMode, sizeof(MS_BOOL));
    bRet &= _variable_alloc((void*)&pbNeedResetAGCMode, sizeof(MS_BOOL));
    bRet &= _variable_alloc((void*)&pbInATVFinetuneMode, sizeof(MS_BOOL));
    bRet &= _variable_alloc((void*)&pbInATVScanMode, sizeof(MS_BOOL));
    bRet &= _variable_alloc((void*)&pu32LastBroadcastType, sizeof(MS_U32));
    bRet &= _variable_alloc((void*)&pu32chantuneFreq, sizeof(MS_U32));
    bRet &= _variable_alloc((void*)&pu32GlobalATVRfFreqKHz, sizeof(MS_U32));
    bRet &= _variable_alloc((void*)&pu8LastOtherMode, sizeof(MS_U8));
    bRet &= _variable_alloc((void*)&pu8curSignalMode, sizeof(MS_U8));
    bRet &= _variable_alloc((void*)&pInitParam, sizeof(TUNER_MS_INIT_PARAM));
    return bRet;
}

static MS_BOOL _variables_free(void)
{
    _variable_free((void*)&pbLastInATVScanMode);
    _variable_free((void*)&pbNeedResetAGCMode);
    _variable_free((void*)&pbInATVFinetuneMode);
    _variable_free((void*)&pbInATVScanMode);
    _variable_free((void*)&pu32LastBroadcastType);
    _variable_free((void*)&pu32chantuneFreq);
    _variable_free((void*)&pu32GlobalATVRfFreqKHz);
    _variable_free((void*)&pu8LastOtherMode);
    _variable_free((void*)&pu8curSignalMode);
    _variable_free((void*)&pInitParam);
    return TRUE;
}

static MS_BOOL _SetTunerInScanMode(MS_U8 u8TunerIndex, MS_BOOL bScan)
{
    if(GLOBAL_VAR(pbInATVScanMode,u8TunerIndex) != bScan)
    {
        GLOBAL_VAR(pbInATVScanMode,u8TunerIndex) = bScan;
        GLOBAL_VAR(pbNeedResetAGCMode,u8TunerIndex) = TRUE;
    }

    return TRUE;
}

static MS_BOOL _CheckFineTuneRange(MS_U8 u8TunerIndex, MS_U32 u32FreqKHz)
{
    if( (u32FreqKHz > (GLOBAL_VAR(pu32chantuneFreq,u8TunerIndex) - MAX_FINE_TUNE_STEP*FREQ_STEP)) &&\
        (u32FreqKHz < (GLOBAL_VAR(pu32chantuneFreq,u8TunerIndex) + MAX_FINE_TUNE_STEP*FREQ_STEP)) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _GetVIFNotchSOSFilter(MS_U32 u32SoundSystem, MS_U32 u32NotchDataLength, MS_U16* pu16GetTunerData)
{
    DEMOD_VIF_SOUND_SYSTEM eSoundSystem;
    MS_U16* u16NotchArray;
    MS_U16* u16SosArray;

    eSoundSystem = (DEMOD_VIF_SOUND_SYSTEM)u32SoundSystem;
    u16NotchArray = pu16GetTunerData;
    u16SosArray = pu16GetTunerData + u32NotchDataLength;

    switch(eSoundSystem)
    {
        case E_VIF_SOUND_B_STEREO_A2:
        {
            u16NotchArray[0] = N_A1_C0_B_A2;
            u16NotchArray[1] = N_A1_C1_B_A2;
            u16NotchArray[2] = N_A1_C2_B_A2;

            u16NotchArray[3] = N_A2_C0_B_A2;
            u16NotchArray[4] = N_A2_C1_B_A2;
            u16NotchArray[5] = N_A2_C2_B_A2;

            u16SosArray[0] = S_11_C0_B_A2;
            u16SosArray[1] = S_11_C1_B_A2;
            u16SosArray[2] = S_11_C2_B_A2;
            u16SosArray[3] = S_11_C3_B_A2;
            u16SosArray[4] = S_11_C4_B_A2;

            u16SosArray[5] = S_12_C0_B_A2;
            u16SosArray[6] = S_12_C1_B_A2;
            u16SosArray[7] = S_12_C2_B_A2;
            u16SosArray[8] = S_12_C3_B_A2;
            u16SosArray[9] = S_12_C4_B_A2;
        }
        break;

        case E_VIF_SOUND_B_MONO_NICAM:
        {
            u16NotchArray[0] = N_A1_C0_B_NICAM;
            u16NotchArray[1] = N_A1_C1_B_NICAM;
            u16NotchArray[2] = N_A1_C2_B_NICAM;

            u16NotchArray[3] = N_A2_C0_B_NICAM;
            u16NotchArray[4] = N_A2_C1_B_NICAM;
            u16NotchArray[5] = N_A2_C2_B_NICAM;

            u16SosArray[0] = S_11_C0_B_NICAM;
            u16SosArray[1] = S_11_C1_B_NICAM;
            u16SosArray[2] = S_11_C2_B_NICAM;
            u16SosArray[3] = S_11_C3_B_NICAM;
            u16SosArray[4] = S_11_C4_B_NICAM;

            u16SosArray[5] = S_12_C0_B_NICAM;
            u16SosArray[6] = S_12_C1_B_NICAM;
            u16SosArray[7] = S_12_C2_B_NICAM;
            u16SosArray[8] = S_12_C3_B_NICAM;
            u16SosArray[9] = S_12_C4_B_NICAM;
        }
        break;

        case E_VIF_SOUND_GH_STEREO_A2:
        {
            u16NotchArray[0] = N_A1_C0_GHA2;
            u16NotchArray[1] = N_A1_C1_GHA2;
            u16NotchArray[2] = N_A1_C2_GHA2;

            u16NotchArray[3] = N_A2_C0_GHA2;
            u16NotchArray[4] = N_A2_C1_GHA2;
            u16NotchArray[5] = N_A2_C2_GHA2;

            u16SosArray[0] = S_11_C0_GHA2;
            u16SosArray[1] = S_11_C1_GHA2;
            u16SosArray[2] = S_11_C2_GHA2;
            u16SosArray[3] = S_11_C3_GHA2;
            u16SosArray[4] = S_11_C4_GHA2;

            u16SosArray[5] = S_12_C0_GHA2;
            u16SosArray[6] = S_12_C1_GHA2;
            u16SosArray[7] = S_12_C2_GHA2;
            u16SosArray[8] = S_12_C3_GHA2;
            u16SosArray[9] = S_12_C4_GHA2;
        }
        break;

        case E_VIF_SOUND_GH_MONO_NICAM:
        {
            u16NotchArray[0] = N_A1_C0_GHMN;
            u16NotchArray[1] = N_A1_C1_GHMN;
            u16NotchArray[2] = N_A1_C2_GHMN;

            u16NotchArray[3] = N_A2_C0_GHMN;
            u16NotchArray[4] = N_A2_C1_GHMN;
            u16NotchArray[5] = N_A2_C2_GHMN;

            u16SosArray[0] = S_11_C0_GHMN;
            u16SosArray[1] = S_11_C1_GHMN;
            u16SosArray[2] = S_11_C2_GHMN;
            u16SosArray[3] = S_11_C3_GHMN;
            u16SosArray[4] = S_11_C4_GHMN;

            u16SosArray[5] = S_12_C0_GHMN;
            u16SosArray[6] = S_12_C1_GHMN;
            u16SosArray[7] = S_12_C2_GHMN;
            u16SosArray[8] = S_12_C3_GHMN;
            u16SosArray[9] = S_12_C4_GHMN;
        }
        break;

        case E_VIF_SOUND_I:
        {
            u16NotchArray[0] = N_A1_C0_I;
            u16NotchArray[1] = N_A1_C1_I;
            u16NotchArray[2] = N_A1_C2_I;

            u16NotchArray[3] = N_A2_C0_I;
            u16NotchArray[4] = N_A2_C1_I;
            u16NotchArray[5] = N_A2_C2_I;

            u16SosArray[0] = S_11_C0_I;
            u16SosArray[1] = S_11_C1_I;
            u16SosArray[2] = S_11_C2_I;
            u16SosArray[3] = S_11_C3_I;
            u16SosArray[4] = S_11_C4_I;

            u16SosArray[5] = S_12_C0_I;
            u16SosArray[6] = S_12_C1_I;
            u16SosArray[7] = S_12_C2_I;
            u16SosArray[8] = S_12_C3_I;
            u16SosArray[9] = S_12_C4_I;
        }
        break;

        case E_VIF_SOUND_DK1_STEREO_A2:
        {
            u16NotchArray[0] = N_A1_C0_DK1A2;
            u16NotchArray[1] = N_A1_C1_DK1A2;
            u16NotchArray[2] = N_A1_C2_DK1A2;

            u16NotchArray[3] = N_A2_C0_DK1A2;
            u16NotchArray[4] = N_A2_C1_DK1A2;
            u16NotchArray[5] = N_A2_C2_DK1A2;

            u16SosArray[0] = S_11_C0_DK1A2;
            u16SosArray[1] = S_11_C1_DK1A2;
            u16SosArray[2] = S_11_C2_DK1A2;
            u16SosArray[3] = S_11_C3_DK1A2;
            u16SosArray[4] = S_11_C4_DK1A2;

            u16SosArray[5] = S_12_C0_DK1A2;
            u16SosArray[6] = S_12_C1_DK1A2;
            u16SosArray[7] = S_12_C2_DK1A2;
            u16SosArray[8] = S_12_C3_DK1A2;
            u16SosArray[9] = S_12_C4_DK1A2;
        }
        break;

        case E_VIF_SOUND_DK2_STEREO_A2:
        {
            u16NotchArray[0] = N_A1_C0_DK2A2;
            u16NotchArray[1] = N_A1_C1_DK2A2;
            u16NotchArray[2] = N_A1_C2_DK2A2;

            u16NotchArray[3] = N_A2_C0_DK2A2;
            u16NotchArray[4] = N_A2_C1_DK2A2;
            u16NotchArray[5] = N_A2_C2_DK2A2;

            u16SosArray[0] = S_11_C0_DK2A2;
            u16SosArray[1] = S_11_C1_DK2A2;
            u16SosArray[2] = S_11_C2_DK2A2;
            u16SosArray[3] = S_11_C3_DK2A2;
            u16SosArray[4] = S_11_C4_DK2A2;

            u16SosArray[5] = S_12_C0_DK2A2;
            u16SosArray[6] = S_12_C1_DK2A2;
            u16SosArray[7] = S_12_C2_DK2A2;
            u16SosArray[8] = S_12_C3_DK2A2;
            u16SosArray[9] = S_12_C4_DK2A2;
        }
        break;

        case E_VIF_SOUND_DK3_STEREO_A2:
        {
            u16NotchArray[0] = N_A1_C0_DK3A2;
            u16NotchArray[1] = N_A1_C1_DK3A2;
            u16NotchArray[2] = N_A1_C2_DK3A2;

            u16NotchArray[3] = N_A2_C0_DK3A2;
            u16NotchArray[4] = N_A2_C1_DK3A2;
            u16NotchArray[5] = N_A2_C2_DK3A2;

            u16SosArray[0] = S_11_C0_DK3A2;
            u16SosArray[1] = S_11_C1_DK3A2;
            u16SosArray[2] = S_11_C2_DK3A2;
            u16SosArray[3] = S_11_C3_DK3A2;
            u16SosArray[4] = S_11_C4_DK3A2;

            u16SosArray[5] = S_12_C0_DK3A2;
            u16SosArray[6] = S_12_C1_DK3A2;
            u16SosArray[7] = S_12_C2_DK3A2;
            u16SosArray[8] = S_12_C3_DK3A2;
            u16SosArray[9] = S_12_C4_DK3A2;
        }
        break;

        case E_VIF_SOUND_DK_MONO_NICAM:
        {
            u16NotchArray[0] = N_A1_C0_DKMN;
            u16NotchArray[1] = N_A1_C1_DKMN;
            u16NotchArray[2] = N_A1_C2_DKMN;

            u16NotchArray[3] = N_A2_C0_DKMN;
            u16NotchArray[4] = N_A2_C1_DKMN;
            u16NotchArray[5] = N_A2_C2_DKMN;

            u16SosArray[0] = S_11_C0_DKMN;
            u16SosArray[1] = S_11_C1_DKMN;
            u16SosArray[2] = S_11_C2_DKMN;
            u16SosArray[3] = S_11_C3_DKMN;
            u16SosArray[4] = S_11_C4_DKMN;

            u16SosArray[5] = S_12_C0_DKMN;
            u16SosArray[6] = S_12_C1_DKMN;
            u16SosArray[7] = S_12_C2_DKMN;
            u16SosArray[8] = S_12_C3_DKMN;
            u16SosArray[9] = S_12_C4_DKMN;
        }
        break;

        case E_VIF_SOUND_L:
        case E_VIF_SOUND_LL:
        {
            u16NotchArray[0] = N_A1_C0_L;
            u16NotchArray[1] = N_A1_C1_L;
            u16NotchArray[2] = N_A1_C2_L;

            u16NotchArray[3] = N_A2_C0_L;
            u16NotchArray[4] = N_A2_C1_L;
            u16NotchArray[5] = N_A2_C2_L;

            u16SosArray[0] = S_11_C0_L;
            u16SosArray[1] = S_11_C1_L;
            u16SosArray[2] = S_11_C2_L;
            u16SosArray[3] = S_11_C3_L;
            u16SosArray[4] = S_11_C4_L;

            u16SosArray[5] = S_12_C0_L;
            u16SosArray[6] = S_12_C1_L;
            u16SosArray[7] = S_12_C2_L;
            u16SosArray[8] = S_12_C3_L;
            u16SosArray[9] = S_12_C4_L;
        }
        break;

        case E_VIF_SOUND_MN:
        {
            u16NotchArray[0] = N_A1_C0_NTSC;
            u16NotchArray[1] = N_A1_C1_NTSC;
            u16NotchArray[2] = N_A1_C2_NTSC;

            u16NotchArray[3] = N_A2_C0_NTSC;
            u16NotchArray[4] = N_A2_C1_NTSC;
            u16NotchArray[5] = N_A2_C2_NTSC;

            u16SosArray[0] = S_11_C0_NTSC;
            u16SosArray[1] = S_11_C1_NTSC;
            u16SosArray[2] = S_11_C2_NTSC;
            u16SosArray[3] = S_11_C3_NTSC;
            u16SosArray[4] = S_11_C4_NTSC;

            u16SosArray[5] = S_12_C0_NTSC;
            u16SosArray[6] = S_12_C1_NTSC;
            u16SosArray[7] = S_12_C2_NTSC;
            u16SosArray[8] = S_12_C3_NTSC;
            u16SosArray[9] = S_12_C4_NTSC;
        }
        break;

        default:
            break;
    }
    return TRUE;
}

MS_BOOL MDrv_MXL661Tuner_PowerOnOff(MS_U8 u8TunerIndex,MS_BOOL bPowerOn)
{
    return TRUE;
}

MXL661_IF_FREQ_E IfFreqConversion(MS_U16 u16IfFreqKHz)
{
    MXL661_IF_FREQ_E enIfFreq = MXL661_IF_5MHz;
    switch(u16IfFreqKHz)
    {
        case 3650:
            enIfFreq = MXL661_IF_3_65MHz;
            break;
        case 4000:
            enIfFreq = MXL661_IF_4MHz;
            break;
        case 4100:
            enIfFreq = MXL661_IF_4_1MHz;
            break;
        case 4150:
            enIfFreq = MXL661_IF_4_15MHz;
            break;
        case 4500:
            enIfFreq = MXL661_IF_4_5MHz;
            break;
        case 4570:
            enIfFreq = MXL661_IF_4_57MHz;
            break;
        case 5000:
            enIfFreq = MXL661_IF_5MHz;
            break;
        case 5380:
            enIfFreq = MXL661_IF_5_38MHz;
            break;
        case 6000:
            enIfFreq = MXL661_IF_6MHz;
            break;
        case 6280:
            enIfFreq = MXL661_IF_6_28MHz;
            break;
        case 7200:
            enIfFreq = MXL661_IF_7_2MHz;
            break;
        case 8250:
            enIfFreq = MXL661_IF_8_25MHz;
            break;
        case 35250:
            enIfFreq = MXL661_IF_35_25MHz;
            break;
        case 36000:
            enIfFreq = MXL661_IF_36MHz;
            break;
        case 36150:
            enIfFreq = MXL661_IF_36_15MHz;
            break;
        case 36650:
            enIfFreq = MXL661_IF_36_65MHz;
            break;
        case 44000:
            enIfFreq = MXL661_IF_44MHz;
            break;
        default:
            enIfFreq = MXL661_IF_5MHz;
            break;
        }
    return enIfFreq;
}

// Comments by MXL LD, 05/22:  Tuner_AFC_RangeSet function is suitable for v2.1.10.5 version driver
//      in v2.1.10.9 driver, shall use Tuner AFC set API instead of calling below function.
/* This function is used to define the Tuner AFC range (in kHz unit) */
MS_BOOL Tuner_AFC_RangeSet(MS_U8 u8TunerIndex, MXL661_AFC_RANGE_E u32FreqOffsetKHz, MS_U8 devID)
{
    MXL_STATUS status = MXL_FAILED;
    apiCmd.commandId = MXL661_TUNER_AFC_CFG;
    apiCmd.MxLIf.cmdAfcCfg.I2cSlaveAddr = devID;
    apiCmd.MxLIf.cmdAfcCfg.AfcRangeInKHz = u32FreqOffsetKHz;
    status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
    return status;
}

MS_BOOL ConfigAGCMode(MS_U8 u8TunerIndex, MS_U32 eMode)
{
    MXL_STATUS status = MXL_FAILED;
    TUNER_MS_INIT_PARAM* pstParam = NULL;

    TUNER_DBG(("\n===>ConfigAGCMode eMode %ld\n",(long int)eMode));

    pstParam = pInitParam + u8TunerIndex;
    switch(eMode)
    {
        case DVBC:
        case DVBT:
            apiCmd.commandId = MXL661_DEV_IF_OUT_CFG;
            apiCmd.MxLIf.cmdIfOutCfg.I2cSlaveAddr = pstParam->u8SlaveID;
            apiCmd.MxLIf.cmdIfOutCfg.IFOutFreq = IfFreqConversion(DTV_TUNER_IF_FREQ_KHz);
            apiCmd.MxLIf.cmdIfOutCfg.ManualFreqSet= MXL_DISABLE;
            apiCmd.MxLIf.cmdIfOutCfg.IFInversion = MXL_DISABLE;
            apiCmd.MxLIf.cmdIfOutCfg.GainLevel = IF_OUT_CFG_GAIN_LEVEL_DVB;
            apiCmd.MxLIf.cmdIfOutCfg.PathSel = MXL661_IF_PATH1;
            status = MxLWare661_API_ConfigDevice(u8TunerIndex, &apiCmd);
            if (MXL_FAILED == status)
            {
                TUNER_DBG(("DVBC Error! MXL_DEV_IF_OUT_CFG\n"));
                return FALSE;
            }

            //AGC Setting:AGC1 external
            apiCmd.commandId = MXL661_TUNER_AGC_CFG;
            apiCmd.MxLIf.cmdAgcSetCfg.I2cSlaveAddr = pstParam->u8SlaveID;
            apiCmd.MxLIf.cmdAgcSetCfg.AgcSel = AGC_Pin_Internal;//default: AGC1
            apiCmd.MxLIf.cmdAgcSetCfg.AgcType =MXL661_AGC_EXTERNAL;
            apiCmd.MxLIf.cmdAgcSetCfg.SetPoint = 66;
            apiCmd.MxLIf.cmdAgcSetCfg.AgcPolarityInverstion = MXL_DISABLE;
            status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
            if (MXL_FAILED == status)
            {
                TUNER_DBG(("\r\nDVBC Error! MXL_TUNER_AGC_CFG\n"));
                return FALSE;
            }
            break;

           case ATV_PAL:
           case ATV_SECAM_L_PRIME:
           case ATV_NTSC:
              if(GLOBAL_VAR(pbNeedResetAGCMode,u8TunerIndex) == TRUE)//complete gain level set  for manual scan finished ,normal mode and autoscan finished done in atv_settune()
              {
                  apiCmd.commandId = MXL661_DEV_IF_OUT_CFG;
                  apiCmd.MxLIf.cmdIfOutCfg.I2cSlaveAddr = pstParam->u8SlaveID;
                  apiCmd.MxLIf.cmdIfOutCfg.IFOutFreq = MXL661_IF_5MHz;
                  apiCmd.MxLIf.cmdIfOutCfg.ManualFreqSet= MXL_DISABLE;
                  apiCmd.MxLIf.cmdIfOutCfg.IFInversion = MXL_DISABLE;
                  apiCmd.MxLIf.cmdIfOutCfg.IFInversion = MXL_ENABLE;
                  apiCmd.MxLIf.cmdIfOutCfg.GainLevel = IF_OUT_CFG_GAIN_LEVEL_ATV_NORMAL;
                  apiCmd.MxLIf.cmdIfOutCfg.PathSel = MXL661_IF_PATH1;

                  status = MxLWare661_API_ConfigDevice(u8TunerIndex, &apiCmd);
                  if (MXL_FAILED == status)
                  {
                      TUNER_DBG(("ATV normal Error! MXL_DEV_IF_OUT_CFG\n"));
                      return FALSE;
                  }

                  //Application Mode setting
                  apiCmd.commandId = MXL661_TUNER_MODE_CFG;
                  apiCmd.MxLIf.cmdModeCfg.I2cSlaveAddr = pstParam->u8SlaveID;
                  apiCmd.MxLIf.cmdModeCfg.SignalMode = MXL661_ANA_PAL_D;
                  apiCmd.MxLIf.cmdModeCfg.IFOutGainLevel = IF_OUT_CFG_GAIN_LEVEL_ATV_NORMAL;
                  apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_ANA_TV_DIG_CABLE_BW_8MHz;
                  apiCmd.MxLIf.cmdModeCfg.IFOutFreqinKHz = 5000;
                  apiCmd.MxLIf.cmdModeCfg.XtalFreqSel = MXL661_XTAL_16MHz;

                  status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
                  if (MXL_FAILED == status)
                  {
                      TUNER_DBG(("ATV normal Error! MXL_TUNER_MODE_CFG\n"));
                      return FALSE;
                  }
              }

               //AGC Setting:AGC1 external
               apiCmd.commandId = MXL661_TUNER_AGC_CFG;
               apiCmd.MxLIf.cmdAgcSetCfg.I2cSlaveAddr = pstParam->u8SlaveID;
               apiCmd.MxLIf.cmdAgcSetCfg.AgcSel = AGC_Pin_Internal;//default: AGC1
               apiCmd.MxLIf.cmdAgcSetCfg.AgcType =MXL661_AGC_SELF;//to avoid ATV vertical line scrambled
               apiCmd.MxLIf.cmdAgcSetCfg.SetPoint = 66;
               apiCmd.MxLIf.cmdAgcSetCfg.AgcPolarityInverstion = MXL_ENABLE;
               status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
               if (MXL_FAILED == status)
               {
                   TUNER_DBG(("\r\nATV normal Error! MXL_TUNER_AGC_CFG\n"));
                   return FALSE;
               }
               break;

        default:
            apiCmd.commandId = MXL661_DEV_IF_OUT_CFG;
            apiCmd.MxLIf.cmdIfOutCfg.I2cSlaveAddr = pstParam->u8SlaveID;
            apiCmd.MxLIf.cmdIfOutCfg.IFOutFreq = IfFreqConversion(DTV_TUNER_IF_FREQ_KHz);
            apiCmd.MxLIf.cmdIfOutCfg.ManualFreqSet= MXL_DISABLE;
            apiCmd.MxLIf.cmdIfOutCfg.IFInversion = MXL_DISABLE;
            apiCmd.MxLIf.cmdIfOutCfg.GainLevel = IF_OUT_CFG_GAIN_LEVEL_DVB;
            apiCmd.MxLIf.cmdIfOutCfg.PathSel = MXL661_IF_PATH1;
            status = MxLWare661_API_ConfigDevice(u8TunerIndex, &apiCmd);
            if (MXL_FAILED == status)
            {
                TUNER_DBG(("DVBC Error! MXL_DEV_IF_OUT_CFG\n"));
                return FALSE;
            }

            //AGC Setting:AGC1 external
            apiCmd.commandId = MXL661_TUNER_AGC_CFG;
            apiCmd.MxLIf.cmdAgcSetCfg.I2cSlaveAddr = pstParam->u8SlaveID;
            apiCmd.MxLIf.cmdAgcSetCfg.AgcSel = AGC_Pin_Internal;//default: AGC1
            apiCmd.MxLIf.cmdAgcSetCfg.AgcType =MXL661_AGC_EXTERNAL;
            apiCmd.MxLIf.cmdAgcSetCfg.SetPoint = 66;
            apiCmd.MxLIf.cmdAgcSetCfg.AgcPolarityInverstion = MXL_DISABLE;
            status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
            if (MXL_FAILED == status)
            {
                TUNER_DBG(("\r\ndefault:Error! MXL_TUNER_AGC_CFG\n"));
                return FALSE;
           }
            break;
        }
    return TRUE;
}

int MxL661_init_main(MS_U8 u8TunerIndex)
{
    MXL_STATUS status;
    UINT8 devId;
    TUNER_MS_INIT_PARAM* pstParam = NULL;

    /* If OEM data is not required, customer should treat devId as
     I2C slave Address */
    pstParam = pInitParam + u8TunerIndex;
    devId = pstParam->u8SlaveID;

    //Step 1 : Soft Reset MxL661
    apiCmd.commandId = MXL661_DEV_SOFT_RESET_CFG;
    apiCmd.MxLIf.cmdResetCfg.I2cSlaveAddr = devId;
    status = MxLWare661_API_ConfigDevice(u8TunerIndex, &apiCmd);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxL661_ConfigDevReset\n"));
        return status;
    }

    //Step 2 : Overwrite Default
    apiCmd.commandId = MXL661_DEV_OVERWRITE_DEFAULT_CFG;
    apiCmd.MxLIf.cmdOverwriteDefault.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdOverwriteDefault.SingleSupply_3_3V = MXL_ENABLE;
    status = MxLWare661_API_ConfigDevice(u8TunerIndex, &apiCmd);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxL661_ConfigDevOverwriteDefault\n"));
        return status;
    }

    //Step 3 : XTAL Setting
    apiCmd.commandId = MXL661_DEV_XTAL_SET_CFG;
    apiCmd.MxLIf.cmdXtalCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdXtalCfg.XtalFreqSel = MXL661_XTAL_16MHz;

    // Force XtalCap 0. Improve accuracy of XTAL from HW side...
    // Dont modify this value...Keep it XtalCap 0.
    // XtalCap = 0, ST18CM-2-E isnt working. (scan no channel).
    // XtalCap = 31 is fine.
    apiCmd.MxLIf.cmdXtalCfg.XtalCap = 31;
    apiCmd.MxLIf.cmdXtalCfg.ClkOutEnable = MXL_DISABLE;
    apiCmd.MxLIf.cmdXtalCfg.ClkOutDiv = MXL_DISABLE;
    apiCmd.MxLIf.cmdXtalCfg.SingleSupply_3_3V = MXL_ENABLE;
    // Comments by MXL LD, 05/22:  if update to v2.1.10.9, need add this parameter
    apiCmd.MxLIf.cmdXtalCfg.XtalSharingMode = MXL_DISABLE;
    status = MxLWare661_API_ConfigDevice(u8TunerIndex, &apiCmd);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxL661_ConfigDevXtalSet\n"));
        return status;
    }

    //Step 4 : IF Out setting
    apiCmd.commandId = MXL661_DEV_IF_OUT_CFG;
    apiCmd.MxLIf.cmdIfOutCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdIfOutCfg.IFOutFreq = IfFreqConversion(DTV_TUNER_IF_FREQ_KHz);
    apiCmd.MxLIf.cmdIfOutCfg.ManualFreqSet= MXL_DISABLE;  //arvin;;m10
    //apiCmd.MxLIf.cmdIfOutCfg.ManualIFOutFreqInKHz = 5380;//4100;  //arvin;;
    apiCmd.MxLIf.cmdIfOutCfg.IFInversion = MXL_DISABLE;
    apiCmd.MxLIf.cmdIfOutCfg.GainLevel = IF_OUT_CFG_GAIN_LEVEL_DVB;
    apiCmd.MxLIf.cmdIfOutCfg.PathSel = MXL661_IF_PATH1;
    status = MxLWare661_API_ConfigDevice(u8TunerIndex, &apiCmd);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxL661_ConfigDevIfOutSet\n"));
        return status;
    }

    //Step 5 : AGC Setting
    apiCmd.commandId = MXL661_TUNER_AGC_CFG;
    apiCmd.MxLIf.cmdAgcSetCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdAgcSetCfg.AgcSel = MXL661_AGC1;
    apiCmd.MxLIf.cmdAgcSetCfg.AgcType = MXL661_AGC_EXTERNAL;
    apiCmd.MxLIf.cmdAgcSetCfg.SetPoint = 66;
    apiCmd.MxLIf.cmdAgcSetCfg.AgcPolarityInverstion = MXL_DISABLE;
    status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
    if (status != MXL_SUCCESS)
    {
        TUNER_ERR(("Error! MxL661_ConfigTunerAgcSet\n"));
        return status;
    }

    // Comments by MXL LD, 05/22: Move tuner power up setting API calling from Channel tune to init process
    //Step 9 : Power up setting
    apiCmd.commandId = MXL661_TUNER_POWER_UP_CFG;
    apiCmd.MxLIf.cmdTunerPoweUpCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdTunerPoweUpCfg.Enable = MXL_ENABLE;
    status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
    if (MXL_FAILED == status)
    {
        TUNER_ERR(("Error! MxL661_ConfigTunerPowerUp\n"));
    }

    //Step 6 :set AFC range
    if (Tuner_AFC_RangeSet(u8TunerIndex, MXL661_AFC_DISABLE, devId))
    {
        TUNER_ERR(("Error! MXL661_TUNER_AFC_CFG\n"));
    }

    AGC_Pin_Internal = MXL661_AGC1; //default : AGC1
    AGC_Pin_External = MXL661_AGC2;//default : AGC2

    return status;
}

MS_BOOL MDrv_Tuner_MXL661_Init(MS_U8 u8TunerIndex,TUNER_MS_INIT_PARAM* pParam)
{
    UINT8 devId;
    TUNER_MS_INIT_PARAM* pstParam = NULL;

    pstParam = pInitParam + u8TunerIndex;

    if((pParam->pCur_Broadcast_type == NULL) || (pParam->pstDemodtab == NULL))
        return FALSE;
    else
    {
        pstParam->pCur_Broadcast_type = pParam->pCur_Broadcast_type;
        pstParam->pstDemodtab = pParam->pstDemodtab;
        pstParam->stTUNCon.eI2C_PORT = pParam->stTUNCon.eI2C_PORT;
        pstParam->stTUNCon.u32HW_ResetPin= pParam->stTUNCon.u32HW_ResetPin;
        MxLWare661_OEM_SetConInfo(u8TunerIndex, &pstParam->stTUNCon, u8max_dev_num);
    }

    // Double Confirm Tuner I2C is normal
    if(MDrv_Tuner_MXL661_CheckExist(u8TunerIndex, NULL) != TRUE)
    {
       TUNER_ERR(("[mxl661] Chip ID is incorrect\n"));
       return FALSE;
    }

    // Check PLL lock status, if PLL have locked before tuner init, it might be in used
    apiCmd.commandId = MXL661_DEV_PLL_STATE_REQ;
    apiCmd.MxLIf.cmdPllReq.I2cSlaveAddr = pstParam->u8SlaveID;
    MxLWare661_API_GetDeviceStatus(u8TunerIndex, &apiCmd);

    if(MXL661_PLL_STATE_NORMAL == apiCmd.MxLIf.cmdPllReq.PllState)
    {
        TUNER_ERR(("[mxl661] Tuner might be in use, PLL is locked\n"));
        return FALSE;
    }

    //This function returns AGC Lock status of MxL661 tuner
    apiCmd.commandId = MXL661_TUNER_AGC_LOCK_REQ;
    apiCmd.MxLIf.cmdTunerAgcLockReq.I2cSlaveAddr = pstParam->u8SlaveID;
    MxLWare661_API_GetTunerStatus(u8TunerIndex, &apiCmd);

    if(MXL_LOCKED == apiCmd.MxLIf.cmdTunerAgcLockReq.AgcLock)
    {
        TUNER_ERR(("[mxl661] Tuner might be in use, AGC is locked\n"));
        return FALSE;
    }

    devId = pstParam->u8SlaveID;
    apiCmd.MxLIf.cmdResetCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdOverwriteDefault.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdXtalCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdIfOutCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdGpoCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdDevInfoReq.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdGpoReq.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdModeCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdTunerPoweUpCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdStartTuneCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdAgcSetCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdChanTuneCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdSuprShiftAdjCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdFineTuneCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdEnableFineTuneCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdHlfrfCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdTunerSignalTypeReq.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdTunerLockReq.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdTunerPwrReq.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdTunerAgcLockReq.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdTunerAfcCorectionReq.I2cSlaveAddr = devId;

    if(MxL661_init_main(u8TunerIndex) == MXL_SUCCESS)
    {
        TUNER_DBG((" MXL661 INIT OK\n"));
        return TRUE;
    }
    else
    {
        TUNER_ERR((" MXL661 INIT FAILED\n"));
        return FALSE;
    }
}

MS_U32 MDrv_Tuner_MXL661_Loop_Through_On(MS_BOOL bOn)
{
    return 0;
}

MS_U32 MDrv_Tuner_MXL661_Stand_By(void)
{
    return 0;
}

MS_U32 MDrv_Tuner_MXL661_Wake_Up(void)
{
    return 0;
}

MS_BOOL MDrv_Tuner_MXL661_SetTuner(MS_U8 u8TunerIndex,MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/)
{
    UINT8 devId;
    TUNER_MS_INIT_PARAM* pstParam = NULL;
    MXL_STATUS status = MXL_FALSE;
    TUNER_EN_TER_BW_MODE eBw = ucBw;

    pstParam = pInitParam + u8TunerIndex;
    devId = pstParam->u8SlaveID;

    ConfigAGCMode(u8TunerIndex, *pstParam->pCur_Broadcast_type);
    //Step 6 : Application Mode setting
    apiCmd.commandId = MXL661_TUNER_MODE_CFG;
    apiCmd.MxLIf.cmdModeCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdModeCfg.IFOutFreqinKHz = TUNER_DTV_IF_FREQ_KHz;
    if(*pstParam->pCur_Broadcast_type== DVBT)
    {
        apiCmd.MxLIf.cmdModeCfg.SignalMode = MXL661_DIG_DVB_T_DTMB;
        apiCmd.MxLIf.cmdModeCfg.IFOutGainLevel = IF_OUT_CFG_GAIN_LEVEL_DVB;
    }
    else if(*pstParam->pCur_Broadcast_type== DVBT2)
    {
        apiCmd.MxLIf.cmdModeCfg.SignalMode = MXL661_DIG_DVB_T_DTMB;
        apiCmd.MxLIf.cmdModeCfg.IFOutGainLevel = IF_OUT_CFG_GAIN_LEVEL_DVB_T2;
    }
    else if(*pstParam->pCur_Broadcast_type== ISDBT)
    {
        apiCmd.MxLIf.cmdModeCfg.SignalMode = MXL661_DIG_ISDBT_ATSC;
        apiCmd.MxLIf.cmdModeCfg.IFOutGainLevel = IF_OUT_CFG_GAIN_LEVEL_ISDB;
    }
    else
    {
        apiCmd.MxLIf.cmdModeCfg.SignalMode = MXL661_DIG_DVB_C;
        apiCmd.MxLIf.cmdModeCfg.IFOutGainLevel = IF_OUT_CFG_GAIN_LEVEL_DVB;
    }

    apiCmd.MxLIf.cmdModeCfg.XtalFreqSel = MXL661_XTAL_16MHz;
    status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
    if (MXL_FAILED == status)
    {
        TUNER_ERR(("Error! MxL661_ConfigTunerApplicationModeSet\n"));
    }

    //Step 7 : Channel mode & bandwidth & frequency setting
    apiCmd.commandId = MXL661_TUNER_CHAN_TUNE_CFG;
    apiCmd.MxLIf.cmdModeCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdChanTuneCfg.TuneType = MXL661_VIEW_MODE;


    if((*pstParam->pCur_Broadcast_type == DVBT) ||
       (*pstParam->pCur_Broadcast_type == DVBT2) ||
       (*pstParam->pCur_Broadcast_type == ISDBT))
    {
        if(eBw == TUNER_BW_MODE_6MHZ)
        {
            apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_DIG_TERR_BW_6MHz;
        }
        else if(eBw == TUNER_BW_MODE_7MHZ)
        {
            apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_DIG_TERR_BW_7MHz;
        }
        else
        {
            apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_DIG_TERR_BW_8MHz;
        }
    }
    else
    {
        if(eBw == TUNER_BW_MODE_6MHZ)
        {
            apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_ANA_TV_DIG_CABLE_BW_6MHz;
        }
        else if(eBw == TUNER_BW_MODE_7MHZ)
        {
            apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_ANA_TV_DIG_CABLE_BW_7MHz;
        }
        else
        {
            apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_ANA_TV_DIG_CABLE_BW_8MHz;
        }
    }

    if(*pstParam->pCur_Broadcast_type == ATSC)
        apiCmd.MxLIf.cmdChanTuneCfg.FreqInHz = dwFreq*1000L;
    else
        apiCmd.MxLIf.cmdChanTuneCfg.FreqInHz = dwFreq*1000L;

    apiCmd.MxLIf.cmdChanTuneCfg.IFOutFreqinKHz = TUNER_DTV_IF_FREQ_KHz;
    if(*pstParam->pCur_Broadcast_type == DVBT)
    {
        apiCmd.MxLIf.cmdChanTuneCfg.SignalMode =  MXL661_DIG_DVB_T_DTMB;
    }
    else if(*pstParam->pCur_Broadcast_type == DVBT2)
    {
        apiCmd.MxLIf.cmdChanTuneCfg.SignalMode =  MXL661_DIG_DVB_T_DTMB;
    }
    else if(*pstParam->pCur_Broadcast_type == ISDBT)
    {
        apiCmd.MxLIf.cmdChanTuneCfg.SignalMode =  MXL661_DIG_ISDBT_ATSC;
    }
    else
    {
        apiCmd.MxLIf.cmdChanTuneCfg.SignalMode =  MXL661_DIG_DVB_C;
    }

    apiCmd.MxLIf.cmdChanTuneCfg.XtalFreqSel = MXL661_XTAL_16MHz;

    status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
    if (MXL_FAILED == status)
    {
        TUNER_ERR(("Error! MxL661_ConfigTunerChanTune\n"));
    }

    // Comments by MXL LD, 05/22:  call MXL_TUNER_START_TUNE_CFG instead of
    //     call MXL_TUNER_POWER_UP_CFG
#if 0
    //Step 8 : Power up setting
    apiCmd.commandId = MXL_TUNER_POWER_UP_CFG;
    apiCmd.MxLIf.cmdTunerPoweUpCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdTunerPoweUpCfg.Enable = MXL_ENABLE;
    status = MxLWare661_API_ConfigTuner(&apiCmd);
    if (MXL_FAILED == status)
    {
        printf("Error! MXL_TUNER_POWER_UP_CFG\n");
    }
#else
    //Step 9 : Sequencer setting
    apiCmd.commandId = MXL661_TUNER_START_TUNE_CFG;
    apiCmd.MxLIf.cmdStartTuneCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdStartTuneCfg.StartTune = MXL_DISABLE;
    status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
    if (MXL_FAILED == status)
    {
        TUNER_ERR(("Error! MxL661_ConfigTunerSequenceSet\n"));
    }
#endif

    //Step 9 : Sequencer setting
    apiCmd.commandId = MXL661_TUNER_START_TUNE_CFG;
    apiCmd.MxLIf.cmdStartTuneCfg.I2cSlaveAddr = devId;
    apiCmd.MxLIf.cmdStartTuneCfg.StartTune = MXL_ENABLE;
    status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
    if (MXL_FAILED == status)
    {
        TUNER_ERR(("Error! MxL661_ConfigTunerSequenceSet\n"));
    }

    // Read back Tuner lock status
    apiCmd.commandId = MXL661_TUNER_LOCK_STATUS_REQ;
    apiCmd.MxLIf.cmdTunerLockReq.I2cSlaveAddr = devId;

    status = MxLWare661_API_GetTunerStatus(u8TunerIndex, &apiCmd);

    if (status == MXL_TRUE)
    {
        if ((MXL_LOCKED == apiCmd.MxLIf.cmdTunerLockReq.RfSynLock) && (MXL_LOCKED == apiCmd.MxLIf.cmdTunerLockReq.RefSynLock))
        {
            TUNER_DBG(("\nTuner locked\n"));
        }
        else
        {
            TUNER_DBG(("Tuner unlocked\n"));
        }
    }

    // Wait 30ms
    MxLWare661_OEM_Sleep(TUNER_PLL_STABLE_TIME);

    if(status == MXL_TRUE)
        return TRUE;
    else
        return FALSE;
}

MS_BOOL MDrv_Tuner_MXL661_SetATVTuner(MS_U8 u8TunerIndex, MS_U32 u32FreqKHz,TUNER_EN_RFBAND eBand, MS_U8 u8OtherMode )
{
    MXL_STATUS status = MXL_FAILED;
    MXL661_IF_FREQ_E mxl_IF_Out_Freq;
    MS_U32 u32IFOutFreqinKHz = 5000;
    MXL_BOOL mxl_IF_Inversion;
    MXL_BOOL fineTuneScaleUp; //Add to fix fineTune garbage(Mantis 0586666)
    TUNER_MS_INIT_PARAM* pstParam = NULL;

    pstParam = pInitParam + u8TunerIndex;

    if( (GLOBAL_VAR(pbInATVFinetuneMode,u8TunerIndex) == TRUE) &&\
        (_CheckFineTuneRange(u8TunerIndex, u32FreqKHz) == TRUE) )  // Do FineTune
    {

        // Determine the fine tune direction
        if (u32FreqKHz > GLOBAL_VAR(pu32GlobalATVRfFreqKHz,u8TunerIndex))
        {
            fineTuneScaleUp = MXL_FINE_TUNE_STEP_UP;
            TUNER_ERR(("\x1b[34m[Hao] %s MXL_FINE_TUNE_STEP_UP [%d] \x1b[0m\n",__FUNCTION__,__LINE__));
        }
        else
        {
            fineTuneScaleUp = MXL_FINE_TUNE_STEP_DOWN;
            TUNER_ERR(("\x1b[34m[Hao] %s MXL_FINE_TUNE_STEP_DOWN [%d] \x1b[0m\n",__FUNCTION__,__LINE__));
        }

        // call MXL_TUNER_ENABLE_FINE_TUNE_CFG and MXL_TUNER_FINE_TUNE_CFG API functions
        apiCmd.commandId = MXL661_TUNER_ENABLE_FINE_TUNE_CFG;
        apiCmd.MxLIf.cmdEnableFineTuneCfg.EnableFineTune = MXL_ENABLE;
        apiCmd.MxLIf.cmdEnableFineTuneCfg.I2cSlaveAddr= pstParam->u8SlaveID;
        status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
        if (MXL_FAILED == status)
        {
            TUNER_ERR(("Error! MXL661_TUNER_ENABLE_FINE_TUNE_CFG\n"));
        }

        apiCmd.commandId = MXL661_TUNER_FINE_TUNE_CFG;
        apiCmd.MxLIf.cmdFineTuneCfg.ScaleUp = fineTuneScaleUp;
        apiCmd.MxLIf.cmdFineTuneCfg.I2cSlaveAddr = pstParam->u8SlaveID;
        status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
        if (MXL_FAILED == status)
        {
            TUNER_ERR(("Error! MXL661_TUNER_FINE_TUNE_CFG\n"));
        }
    }
    else    // Do SetAppMode and ChannelTune
    {

        if( (GLOBAL_VAR(pbInATVFinetuneMode,u8TunerIndex) == TRUE) &&\
            (_CheckFineTuneRange(u8TunerIndex,u32FreqKHz) == FALSE))
        {
            // if the frequency is out of fine-tune range in fine-tune mode, do ChannelTune command instead of FineTune command.
            if( u32FreqKHz >= (GLOBAL_VAR(pu32chantuneFreq,u8TunerIndex) + MAX_FINE_TUNE_STEP*FREQ_STEP) )
            {
                u32FreqKHz = GLOBAL_VAR(pu32chantuneFreq,u8TunerIndex) + MAX_FINE_TUNE_STEP*FREQ_STEP;
            }
            else    // u32FreqKHz <= (u32chantuneFreq - MAX_FINE_TUNE_STEP*FREQ_STEP)
            {
                u32FreqKHz = GLOBAL_VAR(pu32chantuneFreq,u8TunerIndex) - MAX_FINE_TUNE_STEP*FREQ_STEP;
            }
        }

        if((GLOBAL_VAR(pu32LastBroadcastType,u8TunerIndex) != (*pstParam->pCur_Broadcast_type)) ||\
           (GLOBAL_VAR(pu8LastOtherMode,u8TunerIndex) != u8OtherMode) ||\
           (GLOBAL_VAR(pbLastInATVScanMode,u8TunerIndex) != GLOBAL_VAR(pbInATVScanMode,u8TunerIndex)))
        {
            GLOBAL_VAR(pu32LastBroadcastType,u8TunerIndex) = *pstParam->pCur_Broadcast_type;
            GLOBAL_VAR(pu8LastOtherMode,u8TunerIndex) = u8OtherMode;
            GLOBAL_VAR(pbLastInATVScanMode,u8TunerIndex) = GLOBAL_VAR(pbInATVScanMode,u8TunerIndex);

        ConfigAGCMode(u8TunerIndex, *pstParam->pCur_Broadcast_type);

        //Step 4 : IF Out setting
        switch((TUNER_SIF_StandardType)u8OtherMode)
        {
        case E_TUNER_SIF_STANDARD_TYPE_BG:
        case E_TUNER_SIF_STANDARD_TYPE_BG_A2:
        case E_TUNER_SIF_STANDARD_TYPE_BG_NICAM:
               mxl_IF_Out_Freq = MXL661_IF_5MHz;
               u32IFOutFreqinKHz = 5000;
               mxl_IF_Inversion = MXL_ENABLE;
               GLOBAL_VAR(pu8curSignalMode,u8TunerIndex) = MXL661_ANA_PAL_BG;
               break;
        case E_TUNER_SIF_STANDARD_TYPE_I:
               mxl_IF_Out_Freq = MXL661_IF_5MHz;
               u32IFOutFreqinKHz = 5000;
               mxl_IF_Inversion = MXL_ENABLE;
               GLOBAL_VAR(pu8curSignalMode,u8TunerIndex) = MXL661_ANA_PAL_I;
               break;
        case E_TUNER_SIF_STANDARD_TYPE_DK:
        case E_TUNER_SIF_STANDARD_TYPE_DK1_A2:
        case E_TUNER_SIF_STANDARD_TYPE_DK2_A2:
        case E_TUNER_SIF_STANDARD_TYPE_DK3_A2:
        case E_TUNER_SIF_STANDARD_TYPE_DK_NICAM:
               mxl_IF_Out_Freq = MXL661_IF_5MHz;
               u32IFOutFreqinKHz = 5000;
               mxl_IF_Inversion = MXL_ENABLE;
               GLOBAL_VAR(pu8curSignalMode,u8TunerIndex) = MXL661_ANA_PAL_D;
               break;
        case E_TUNER_SIF_STANDARD_TYPE_M:
        case E_TUNER_SIF_STANDARD_TYPE_M_BTSC:
        case E_TUNER_SIF_STANDARD_TYPE_M_A2:
        case E_TUNER_SIF_STANDARD_TYPE_M_EIA_J:
               mxl_IF_Out_Freq = MXL661_IF_5MHz;
               u32IFOutFreqinKHz = 5000;
               mxl_IF_Inversion = MXL_ENABLE;
               GLOBAL_VAR(pu8curSignalMode,u8TunerIndex) = MXL661_ANA_NTSC_MODE;
               break;
          default:
               mxl_IF_Out_Freq = MXL661_IF_5MHz;
               u32IFOutFreqinKHz = 5000;
               mxl_IF_Inversion = MXL_ENABLE;
               GLOBAL_VAR(pu8curSignalMode,u8TunerIndex) = MXL661_ANA_PAL_D;
              break;
        }

        apiCmd.commandId = MXL661_DEV_IF_OUT_CFG;
        apiCmd.MxLIf.cmdIfOutCfg.IFOutFreq = mxl_IF_Out_Freq;
        apiCmd.MxLIf.cmdIfOutCfg.ManualFreqSet= MXL_DISABLE;
        apiCmd.MxLIf.cmdIfOutCfg.IFInversion = mxl_IF_Inversion;
        if(GLOBAL_VAR(pbInATVScanMode,u8TunerIndex) == TRUE)
        {
            apiCmd.MxLIf.cmdIfOutCfg.GainLevel = IF_OUT_CFG_GAIN_LEVEL_ATV_SCAN;
        }
        else
        {
            apiCmd.MxLIf.cmdIfOutCfg.GainLevel = IF_OUT_CFG_GAIN_LEVEL_ATV_NORMAL;
        }
        apiCmd.MxLIf.cmdIfOutCfg.PathSel = MXL661_IF_PATH1;
        apiCmd.MxLIf.cmdIfOutCfg.I2cSlaveAddr = pstParam->u8SlaveID;
        status = MxLWare661_API_ConfigDevice(u8TunerIndex, &apiCmd);
        if (MXL_FAILED == status)
        {
            TUNER_ERR(("Error! MXL_DEV_IF_OUT_CFG\n"));
        }

        //Step 6 : Application Mode setting
        apiCmd.commandId = MXL661_TUNER_MODE_CFG;

        if((*pstParam->pCur_Broadcast_type) == ATV_SECAM_L_PRIME)
        {
            apiCmd.MxLIf.cmdModeCfg.SignalMode = MXL661_ANA_SECAM_L;
        }
        else if ((*pstParam->pCur_Broadcast_type) == ATV_NTSC)
        {
            apiCmd.MxLIf.cmdModeCfg.SignalMode = MXL661_ANA_NTSC_MODE;
            apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_ANA_TV_DIG_CABLE_BW_6MHz;
        }
        else
        {
            switch((TUNER_SIF_StandardType)u8OtherMode)
            {
                case E_TUNER_SIF_STANDARD_TYPE_BG:
                case E_TUNER_SIF_STANDARD_TYPE_BG_A2:
                case E_TUNER_SIF_STANDARD_TYPE_BG_NICAM:
                    apiCmd.MxLIf.cmdModeCfg.SignalMode = MXL661_ANA_PAL_BG;
                    apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_ANA_TV_DIG_CABLE_BW_7MHz;
                    break;
                case E_TUNER_SIF_STANDARD_TYPE_I:
                    apiCmd.MxLIf.cmdModeCfg.SignalMode = MXL661_ANA_PAL_I;
                    apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_ANA_TV_DIG_CABLE_BW_8MHz;
                    break;
                case E_TUNER_SIF_STANDARD_TYPE_DK:
                case E_TUNER_SIF_STANDARD_TYPE_DK1_A2:
                case E_TUNER_SIF_STANDARD_TYPE_DK2_A2:
                case E_TUNER_SIF_STANDARD_TYPE_DK3_A2:
                case E_TUNER_SIF_STANDARD_TYPE_DK_NICAM:
                    apiCmd.MxLIf.cmdModeCfg.SignalMode = MXL661_ANA_PAL_D;
                    apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_ANA_TV_DIG_CABLE_BW_8MHz;
                    break;
                 case E_TUNER_SIF_STANDARD_TYPE_M:
                 case E_TUNER_SIF_STANDARD_TYPE_M_BTSC:
                 case E_TUNER_SIF_STANDARD_TYPE_M_A2:
                 case E_TUNER_SIF_STANDARD_TYPE_M_EIA_J:
                    apiCmd.MxLIf.cmdModeCfg.SignalMode = MXL661_ANA_NTSC_MODE;
                    apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_ANA_TV_DIG_CABLE_BW_6MHz;
                    break;
                default:
                    apiCmd.MxLIf.cmdModeCfg.SignalMode = MXL661_ANA_PAL_D;
                    apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_ANA_TV_DIG_CABLE_BW_8MHz;
                    break;
            }
        }

        apiCmd.MxLIf.cmdModeCfg.IFOutFreqinKHz = u32IFOutFreqinKHz;
        apiCmd.MxLIf.cmdModeCfg.XtalFreqSel = MXL661_XTAL_16MHz;
        if(GLOBAL_VAR(pbInATVScanMode,u8TunerIndex) == TRUE)
        {
            apiCmd.MxLIf.cmdModeCfg.IFOutGainLevel = IF_OUT_CFG_GAIN_LEVEL_ATV_SCAN;
        }
        else
        {
           apiCmd.MxLIf.cmdModeCfg.IFOutGainLevel = IF_OUT_CFG_GAIN_LEVEL_ATV_NORMAL;
        }
        apiCmd.MxLIf.cmdModeCfg.I2cSlaveAddr = pstParam->u8SlaveID;
        status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
        if (MXL_FAILED == status)
        {
            TUNER_ERR(("Error! MXL_TUNER_MODE_CFG\n"));
        }
    }

    //Step 7 : Channel frequency & bandwidth setting
    apiCmd.commandId = MXL661_TUNER_CHAN_TUNE_CFG;
    apiCmd.MxLIf.cmdChanTuneCfg.TuneType = MXL661_VIEW_MODE;
    apiCmd.MxLIf.cmdChanTuneCfg.XtalFreqSel = MXL661_XTAL_16MHz;
    apiCmd.MxLIf.cmdChanTuneCfg.IFOutFreqinKHz = u32IFOutFreqinKHz;
    switch((TUNER_SIF_StandardType)u8OtherMode)
    {
        case E_TUNER_SIF_STANDARD_TYPE_BG:
        case E_TUNER_SIF_STANDARD_TYPE_BG_A2:
        case E_TUNER_SIF_STANDARD_TYPE_BG_NICAM:

             apiCmd.MxLIf.cmdChanTuneCfg.SignalMode = MXL661_ANA_PAL_BG;
             apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_ANA_TV_DIG_CABLE_BW_7MHz;
             apiCmd.MxLIf.cmdChanTuneCfg.FreqInHz =(u32FreqKHz+2250)*1000L;//Freq*100*1000;
             break;

                case E_TUNER_SIF_STANDARD_TYPE_I:
                    apiCmd.MxLIf.cmdChanTuneCfg.SignalMode = MXL661_ANA_PAL_I;
                    apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_ANA_TV_DIG_CABLE_BW_8MHz;
                    apiCmd.MxLIf.cmdChanTuneCfg.FreqInHz =(u32FreqKHz+2750)*1000L;//Freq*100*1000;
                    break;
                case E_TUNER_SIF_STANDARD_TYPE_DK:
                case E_TUNER_SIF_STANDARD_TYPE_DK1_A2:
                case E_TUNER_SIF_STANDARD_TYPE_DK2_A2:
                case E_TUNER_SIF_STANDARD_TYPE_DK3_A2:
                case E_TUNER_SIF_STANDARD_TYPE_DK_NICAM:

                   apiCmd.MxLIf.cmdChanTuneCfg.SignalMode = MXL661_ANA_PAL_D;
                   apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_ANA_TV_DIG_CABLE_BW_8MHz;
                   apiCmd.MxLIf.cmdChanTuneCfg.FreqInHz =(u32FreqKHz+2750)*1000L;//Freq*100*1000;
                   break;

                case E_TUNER_SIF_STANDARD_TYPE_M:
                case E_TUNER_SIF_STANDARD_TYPE_M_BTSC:
                case E_TUNER_SIF_STANDARD_TYPE_M_A2:
                case E_TUNER_SIF_STANDARD_TYPE_M_EIA_J:

                    apiCmd.MxLIf.cmdChanTuneCfg.SignalMode = MXL661_ANA_NTSC_MODE;
                    apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_ANA_TV_DIG_CABLE_BW_6MHz;
                    apiCmd.MxLIf.cmdChanTuneCfg.FreqInHz =(u32FreqKHz+1750)*1000L;//Freq*100*1000;
                    break;

                default:
                    apiCmd.MxLIf.cmdChanTuneCfg.SignalMode = MXL661_ANA_PAL_D;
                    apiCmd.MxLIf.cmdChanTuneCfg.BandWidth = MXL661_ANA_TV_DIG_CABLE_BW_8MHz;
                    apiCmd.MxLIf.cmdChanTuneCfg.FreqInHz =(u32FreqKHz+2750)*1000L;//Freq*100*1000;
                    break;
    }

    apiCmd.MxLIf.cmdChanTuneCfg.I2cSlaveAddr = pstParam->u8SlaveID;
    status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
    if (MXL_FAILED == status)
    {
        TUNER_ERR(("Error! MXL_TUNER_CHAN_TUNE_CFG\n"));
    }

    //Step 8 : Disable AFC in tuner if scan
    if(GLOBAL_VAR(pbInATVScanMode,u8TunerIndex) == TRUE)
    {
        Tuner_AFC_RangeSet(u8TunerIndex, MXL661_AFC_DISABLE, pstParam->u8SlaveID);
    }
    else
    {
        Tuner_AFC_RangeSet(u8TunerIndex, MXL661_AFC_DISABLE, pstParam->u8SlaveID);
    }

    // Comments by MXL LD, 05/22:  remove MXL_TUNER_POWER_UP_CFG API calling from tuner channel tune to init phase
#if 0
    //Step 9 : Power up setting
    apiCmd.commandId = MXL_TUNER_POWER_UP_CFG;
    apiCmd.MxLIf.cmdTunerPoweUpCfg.Enable = MXL_ENABLE;
    status = MxLWare661_API_ConfigTuner(&apiCmd);
    if (MXL_FAILED == status)
    {
        printf("Error! MXL_TUNER_POWER_UP_CFG\n");
    }
#else
    // Comments by MXL LD, 05/22: replace MXL_TUNER_POWER_UP_CFG calling with MXL_TUNER_START_TUNE_CFG API
    //   First do disable tune operation
    //Step 10 : Sequencer setting: disable tune
    apiCmd.commandId = MXL661_TUNER_START_TUNE_CFG;
    apiCmd.MxLIf.cmdStartTuneCfg.StartTune = MXL_DISABLE;
    apiCmd.MxLIf.cmdStartTuneCfg.I2cSlaveAddr = pstParam->u8SlaveID;
    status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
    if (MXL_FAILED == status)
    {
        TUNER_ERR(("Error! MXL_TUNER_START_TUNE_CFG\n"));
    }
#endif
        //Step 10 : Sequencer setting : enable tune
        apiCmd.commandId = MXL661_TUNER_START_TUNE_CFG;
        apiCmd.MxLIf.cmdStartTuneCfg.StartTune = MXL_ENABLE;
        apiCmd.MxLIf.cmdStartTuneCfg.I2cSlaveAddr = pstParam->u8SlaveID;
        status = MxLWare661_API_ConfigTuner(u8TunerIndex, &apiCmd);
        if (MXL_FAILED == status)
        {
            TUNER_ERR(("Error! MXL_TUNER_START_TUNE_CFG\n"));
        }

        // Save the frequency set by ChannelTune command
        GLOBAL_VAR(pu32chantuneFreq,u8TunerIndex) = u32FreqKHz;
    }
    // Save current RF frequency point
    GLOBAL_VAR(pu32GlobalATVRfFreqKHz,u8TunerIndex) = u32FreqKHz;

    apiCmd.commandId = MXL661_TUNER_LOCK_STATUS_REQ;
    apiCmd.MxLIf.cmdTunerLockReq.I2cSlaveAddr = pstParam->u8SlaveID;
    status = MxLWare661_API_GetTunerStatus(u8TunerIndex, &apiCmd);

    if (status == MXL_TRUE)
    {
        if ((MXL_LOCKED == apiCmd.MxLIf.cmdTunerLockReq.RfSynLock) && (MXL_LOCKED == apiCmd.MxLIf.cmdTunerLockReq.RefSynLock))
        {
            TUNER_DBG(("\nTuner locked\n"));
        }
        else
        {
            TUNER_DBG(("Tuner unlocked\n"));
        }
    }

    // Wait 30ms,wait for signal stable
    MxLWare661_OEM_Sleep(TUNER_PLL_STABLE_TIME);
    return TRUE;
}

MS_BOOL MDrv_Tuner_MXL661_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MS_U8 i, u8CurID;
    TUNER_MS_INIT_PARAM* pstParam = NULL;

    if(pInitParam == NULL)
    {
        if(!_variables_alloc())
        {
            _variables_free();
            return FALSE;
        }
    }

    pstParam = pInitParam + u8TunerIndex;

    for(i=0; i< sizeof(u8Possible_SLAVE_IDs); i++)
    {
        u8CurID = u8Possible_SLAVE_IDs[i];

        apiCmd.commandId = MXL661_DEV_ID_VERSION_REQ;
        apiCmd.MxLIf.cmdDevInfoReq.I2cSlaveAddr = u8CurID;
        if(MXL_SUCCESS != MxLWare661_API_GetDeviceStatus(u8TunerIndex, &apiCmd))
        {
            TUNER_ERR(("[mxl661] Read chip ID fail with slave ID 0x%x\n", u8CurID));
        }
        else
        {
            pstParam->u8SlaveID = u8CurID;
            break;
        }
    }

    TUNER_DBG(("[mxl661] read id =0x%x\n", apiCmd.MxLIf.cmdDevInfoReq.ChipId));

    if(apiCmd.MxLIf.cmdDevInfoReq.ChipId == MXL661_CHIP_ID)
    {
        bEverDetect = TRUE;
        if(NULL != pu32channel_cnt)
            *(pu32channel_cnt) += 1;

        return TRUE;
    }
    else
    {
       if(!bEverDetect)
           _variables_free();

        return FALSE;
    }
}

MS_BOOL MDrv_Tuner_MXL661_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    TUNER_MS_INIT_PARAM* pstParam = NULL;
    UINT8 devId;
    TUNER_EXT_FUNCTION_PARAM* pstData;
    DEMOD_VIF_UserFilter* pstVIFUserFilter;
    DEMOD_VIF_CARRIER_RECOVERY *pstCR;
    MS_U16* pu16GetTunerData;
    MS_BOOL bRet = TRUE;
    TUNER_CON_CONFIG* pstCon;
    switch(fuction_type)
    {
        case TUNER_EXT_FUNC_GET_POWER_LEVEL:
            if(pInitParam == NULL)
                return FALSE;
            else
            {
                pstParam = pInitParam + u8TunerIndex;
                devId = pstParam->u8SlaveID;
            }
            apiCmd.commandId = MXL661_TUNER_RX_PWR_REQ;
            apiCmd.MxLIf.cmdTunerPwrReq.I2cSlaveAddr = devId;

            if( MXL_SUCCESS != MxLWare661_API_GetTunerStatus(u8TunerIndex, &apiCmd))
                return FALSE;
            *(float*)data = (float)(apiCmd.MxLIf.cmdTunerPwrReq.RxPwr)/100;
            break;

        case TUNER_EXT_FUNC_FINALIZE:
            break;

        case TUNER_EXT_FUNC_GET_FREQ_STEP:
            *(TUNER_EN_FREQ_STEP*)data = E_FREQ_STEP_62_5KHz;
            break;

        case TUNER_EXT_FUNC_GET_PEAKING_PARAMETER:
            pstData = (TUNER_EXT_FUNCTION_PARAM*)data;
            pstVIFUserFilter = (DEMOD_VIF_UserFilter*)(pstData->pParam);
            pstVIFUserFilter->VifSos21FilterC0  = VIF_SOS_21_FILTER_C0;
            pstVIFUserFilter->VifSos21FilterC1  = VIF_SOS_21_FILTER_C1;
            pstVIFUserFilter->VifSos21FilterC2  = VIF_SOS_21_FILTER_C2;
            pstVIFUserFilter->VifSos21FilterC3  = VIF_SOS_21_FILTER_C3;
            pstVIFUserFilter->VifSos21FilterC4  = VIF_SOS_21_FILTER_C4;
            pstVIFUserFilter->VifSos22FilterC0  = VIF_SOS_22_FILTER_C0;
            pstVIFUserFilter->VifSos22FilterC1  = VIF_SOS_22_FILTER_C1;
            pstVIFUserFilter->VifSos22FilterC2  = VIF_SOS_22_FILTER_C2;
            pstVIFUserFilter->VifSos22FilterC3  = VIF_SOS_22_FILTER_C3;
            pstVIFUserFilter->VifSos22FilterC4  = VIF_SOS_22_FILTER_C4;
            pstVIFUserFilter->VifSos31FilterC0  = VIF_SOS_31_FILTER_C0;
            pstVIFUserFilter->VifSos31FilterC1  = VIF_SOS_31_FILTER_C1;
            pstVIFUserFilter->VifSos31FilterC2  = VIF_SOS_31_FILTER_C2;
            pstVIFUserFilter->VifSos31FilterC3  = VIF_SOS_31_FILTER_C3;
            pstVIFUserFilter->VifSos31FilterC4  = VIF_SOS_31_FILTER_C4;
            pstVIFUserFilter->VifSos32FilterC0  = VIF_SOS_32_FILTER_C0;
            pstVIFUserFilter->VifSos32FilterC1  = VIF_SOS_32_FILTER_C1;
            pstVIFUserFilter->VifSos32FilterC2  = VIF_SOS_32_FILTER_C2;
            pstVIFUserFilter->VifSos32FilterC3  = VIF_SOS_32_FILTER_C3;
            pstVIFUserFilter->VifSos32FilterC4  = VIF_SOS_32_FILTER_C4;
            break;

         case TUNER_EXT_FUNC_GET_VIF_NOTCH_SOSFILTER:
            pstData = (TUNER_EXT_FUNCTION_PARAM*)data;
            pu16GetTunerData = pstData->pParam;
            bRet = _GetVIFNotchSOSFilter(pstData->u32Param1, pstData->u32Param2, pu16GetTunerData);
            break;

         case TUNER_EXT_FUNC_GET_VIF_CR:
            pstCR = (DEMOD_VIF_CARRIER_RECOVERY*)data;
            pstCR->VifCrRate_B = VIF_CR_RATE_B;
            pstCR->VifCrInvert_B = VIF_CR_INVERT_B;
            pstCR->VifCrRate_GH = VIF_CR_RATE_GH;
            pstCR->VifCrInvert_GH =VIF_CR_INVERT_GH;
            pstCR->VifCrRate_DK = VIF_CR_RATE_DK;
            pstCR->VifCrInvert_DK = VIF_CR_INVERT_DK;
            pstCR->VifCrRate_I = VIF_CR_RATE_I;
            pstCR->VifCrInvert_I = VIF_CR_INVERT_I;
            pstCR->VifCrRate_L = VIF_CR_RATE_L;
            pstCR->VifCrInvert_L = VIF_CR_INVERT_L;
            pstCR->VifCrRate_LL = VIF_CR_RATE_LL;
            pstCR->VifCrInvert_LL = VIF_CR_INVERT_LL;
            pstCR->VifCrRate_MN = VIF_CR_RATE_MN;
            pstCR->VifCrInvert_MN = VIF_CR_INVERT_MN;
            break;

         case TUNER_EXT_FUNC_GET_ATV_RF_BOUNDARY:
             pstData = (TUNER_EXT_FUNCTION_PARAM*)data;
             *(MS_U32*)pstData->pParam = u32FreqBound[pstData->u32Param1];
             break;

         case TUNER_EXT_FUNC_GET_TUNER_TYPE:
            *(MS_U8*)data = VIF_TUNER_TYPE;
            pstData = (TUNER_EXT_FUNCTION_PARAM*)data;

            break;

         case TUNER_EXT_RUNC_SET_TUNER_IN_SCAN_MODE:
            _SetTunerInScanMode(u8TunerIndex, *(MS_BOOL*)data);
            break;

         case TUNER_EXT_FUNC_SET_CON_INFO:
            pstData = (TUNER_EXT_FUNCTION_PARAM*)data;
            u8max_dev_num = pstData->u32Param1;
            pstCon = (TUNER_CON_CONFIG*)(pstData->pParam);
            if(!_variables_alloc())
            {
                _variables_free();
                return FALSE;
            }
            else
            {
                pstParam = pInitParam + u8TunerIndex;
                memcpy(&pstParam->stTUNCon, pstCon,sizeof(TUNER_CON_CONFIG));
                MxLWare661_OEM_SetConInfo(u8TunerIndex, pstCon, u8max_dev_num);
            }
        break;
        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n",fuction_type));
            break;
    }
    return bRet;
}

MS_BOOL MDrv_Tuner_MXL661_GetTunerIF(MS_U8 u8TunerIndex, MS_U32* u32IF_Freq)
{
    UINT16 ifFcw;
    UINT8 status = MXL_SUCCESS;
    UINT8 readData = 0;
    TUNER_MS_INIT_PARAM* pstParam = NULL;

    pstParam = pInitParam + u8TunerIndex;
  // Read back register for manual IF Out
  status |= MxLWare661_OEM_ReadRegister(u8TunerIndex,pstParam->u8SlaveID, IF_FREQ_SEL_REG, &readData);
     // IF out manual setting : bit<5>
  if((readData & 0x20) == 0x20)
  {
      //Get low 8 bit
      status |= MxLWare661_OEM_ReadRegister(u8TunerIndex,pstParam->u8SlaveID, IF_FCW_LOW_REG, &readData);
      ifFcw = (UINT16)readData;
      // Get high 4 bit
      status |= MxLWare661_OEM_ReadRegister(u8TunerIndex,pstParam->u8SlaveID, IF_FCW_HIGH_REG, &readData);
      ifFcw |= ((UINT16)(readData) & 0x0f) << 8;
      *u32IF_Freq = (MS_U32)((ifFcw * 216000)/8192);
  }
  else
  {
      // IF Freq <4:0>
      status |= MxLWare661_OEM_ReadRegister(u8TunerIndex,pstParam->u8SlaveID, IF_FREQ_SEL_REG, &readData);
      readData &= 0x1f;
      switch(readData)
      {
          case MXL661_IF_3_65MHz:
              *u32IF_Freq = 3650;
              break;
          case MXL661_IF_4MHz:
              *u32IF_Freq = 4000;
              break;
          case MXL661_IF_4_1MHz:
              *u32IF_Freq = 4100;
              break;
          case MXL661_IF_4_15MHz:
              *u32IF_Freq = 4150;
              break;
          case MXL661_IF_4_5MHz:
              *u32IF_Freq = 4500;
              break;
          case MXL661_IF_4_57MHz:
              *u32IF_Freq = 4570;
              break;
          case MXL661_IF_5MHz:
              *u32IF_Freq = 5000;
              break;
          case MXL661_IF_5_38MHz:
              *u32IF_Freq = 5380;
              break;
          case MXL661_IF_6MHz:
              *u32IF_Freq = 6000;
              break;
          case MXL661_IF_6_28MHz:
              *u32IF_Freq = 6280;
              break;
          case MXL661_IF_7_2MHz:
               *u32IF_Freq = 7200;
              break;
          case MXL661_IF_8_25MHz:
              *u32IF_Freq = 8250;
              break;
          case MXL661_IF_35_25MHz:
              *u32IF_Freq = 35250;
              break;
          case MXL661_IF_36MHz:
              *u32IF_Freq = 36000;
              break;
          case MXL661_IF_36_15MHz:
              *u32IF_Freq = 36150;
              break;
          case MXL661_IF_36_65MHz:
              *u32IF_Freq = 36650;
              break;
          case MXL661_IF_44MHz:
              *u32IF_Freq = 44000;
              break;
          default:
              *u32IF_Freq = 5000;
              break;
      }
  }

  if(MXL_SUCCESS == status)
    return TRUE;
  else
    return FALSE;
}

#if 0
MS_BOOL MDrv_Tuner_MXL661_GetPeakingParamter(MS_U8 u8TunerIndex, MS_U32 u32Band, MS_U32 u32IfFreq, DEMOD_VIFUserFilter* pstVifUserFilter)
{
    TUNER_EN_RFBAND eBand;
    DEMOD_IF_FREQ eIFFreq;

    eBand = (TUNER_EN_RFBAND)u32Band;
    eIFFreq = (DEMOD_IF_FREQ)eIFFreq;

    switch (eBand)
    {
        case E_RFBAND_VHF_LOW:
            if((eIFFreq == IF_FREQ_B)||(eIFFreq == IF_FREQ_G))
            {
                pstVifUserFilter->VifSos21FilterC0  = VIF_SOS_21_FILTER_C0;
                pstVifUserFilter->VifSos21FilterC1  = VIF_SOS_21_FILTER_C1;
                pstVifUserFilter->VifSos21FilterC2  = VIF_SOS_21_FILTER_C2;
                pstVifUserFilter->VifSos21FilterC3  = VIF_SOS_21_FILTER_C3;
                pstVifUserFilter->VifSos21FilterC4  = VIF_SOS_21_FILTER_C4;
                pstVifUserFilter->VifSos22FilterC0  = VIF_SOS_22_FILTER_C0;
                pstVifUserFilter->VifSos22FilterC1  = VIF_SOS_22_FILTER_C1;
                pstVifUserFilter->VifSos22FilterC2  = VIF_SOS_22_FILTER_C2;
                pstVifUserFilter->VifSos22FilterC3  = VIF_SOS_22_FILTER_C3;
                pstVifUserFilter->VifSos22FilterC4  = VIF_SOS_22_FILTER_C4;
                pstVifUserFilter->VifSos31FilterC0  = VIF_SOS_31_FILTER_C0;
                pstVifUserFilter->VifSos31FilterC1  = VIF_SOS_31_FILTER_C1;
                pstVifUserFilter->VifSos31FilterC2  = VIF_SOS_31_FILTER_C2;
                pstVifUserFilter->VifSos31FilterC3  = VIF_SOS_31_FILTER_C3;
                pstVifUserFilter->VifSos31FilterC4  = VIF_SOS_31_FILTER_C4;
                pstVifUserFilter->VifSos32FilterC0  = VIF_SOS_32_FILTER_C0;
                pstVifUserFilter->VifSos32FilterC1  = VIF_SOS_32_FILTER_C1;
                pstVifUserFilter->VifSos32FilterC2  = VIF_SOS_32_FILTER_C2;
                pstVifUserFilter->VifSos32FilterC3  = VIF_SOS_32_FILTER_C3;
                pstVifUserFilter->VifSos32FilterC4  = VIF_SOS_32_FILTER_C4;
            }
            else if(eIFFreq == IF_FREQ_DK)
            {
                pstVifUserFilter->VifSos21FilterC0  = VIF_SOS_21_FILTER_C0;
                pstVifUserFilter->VifSos21FilterC1  = VIF_SOS_21_FILTER_C1;
                pstVifUserFilter->VifSos21FilterC2  = VIF_SOS_21_FILTER_C2;
                pstVifUserFilter->VifSos21FilterC3  = VIF_SOS_21_FILTER_C3;
                pstVifUserFilter->VifSos21FilterC4  = VIF_SOS_21_FILTER_C4;
                pstVifUserFilter->VifSos22FilterC0  = VIF_SOS_22_FILTER_C0;
                pstVifUserFilter->VifSos22FilterC1  = VIF_SOS_22_FILTER_C1;
                pstVifUserFilter->VifSos22FilterC2  = VIF_SOS_22_FILTER_C2;
                pstVifUserFilter->VifSos22FilterC3  = VIF_SOS_22_FILTER_C3;
                pstVifUserFilter->VifSos22FilterC4  = VIF_SOS_22_FILTER_C4;
                pstVifUserFilter->VifSos31FilterC0  = VIF_SOS_31_FILTER_C0;
                pstVifUserFilter->VifSos31FilterC1  = VIF_SOS_31_FILTER_C1;
                pstVifUserFilter->VifSos31FilterC2  = VIF_SOS_31_FILTER_C2;
                pstVifUserFilter->VifSos31FilterC3  = VIF_SOS_31_FILTER_C3;
                pstVifUserFilter->VifSos31FilterC4  = VIF_SOS_31_FILTER_C4;
                pstVifUserFilter->VifSos32FilterC0  = VIF_SOS_32_FILTER_C0;
                pstVifUserFilter->VifSos32FilterC1  = VIF_SOS_32_FILTER_C1;
                pstVifUserFilter->VifSos32FilterC2  = VIF_SOS_32_FILTER_C2;
                pstVifUserFilter->VifSos32FilterC3  = VIF_SOS_32_FILTER_C3;
                pstVifUserFilter->VifSos32FilterC4  = VIF_SOS_32_FILTER_C4;
            }
            else if(eIFFreq == IF_FREQ_I)
            {
                pstVifUserFilter->VifSos21FilterC0  = VIF_SOS_21_FILTER_C0;
                pstVifUserFilter->VifSos21FilterC1  = VIF_SOS_21_FILTER_C1;
                pstVifUserFilter->VifSos21FilterC2  = VIF_SOS_21_FILTER_C2;
                pstVifUserFilter->VifSos21FilterC3  = VIF_SOS_21_FILTER_C3;
                pstVifUserFilter->VifSos21FilterC4  = VIF_SOS_21_FILTER_C4;

                pstVifUserFilter->VifSos22FilterC0  = VIF_SOS_22_FILTER_C0;
                pstVifUserFilter->VifSos22FilterC1  = VIF_SOS_22_FILTER_C1;
                pstVifUserFilter->VifSos22FilterC2  = VIF_SOS_22_FILTER_C2;
                pstVifUserFilter->VifSos22FilterC3  = VIF_SOS_22_FILTER_C3;
                pstVifUserFilter->VifSos22FilterC4  = VIF_SOS_22_FILTER_C4;

                pstVifUserFilter->VifSos31FilterC0  = VIF_SOS_31_FILTER_C0;
                pstVifUserFilter->VifSos31FilterC1  = VIF_SOS_31_FILTER_C1;
                pstVifUserFilter->VifSos31FilterC2  = VIF_SOS_31_FILTER_C2;
                pstVifUserFilter->VifSos31FilterC3  = VIF_SOS_31_FILTER_C3;
                pstVifUserFilter->VifSos31FilterC4  = VIF_SOS_31_FILTER_C4;
                pstVifUserFilter->VifSos32FilterC0  = VIF_SOS_32_FILTER_C0;
                pstVifUserFilter->VifSos32FilterC1  = VIF_SOS_32_FILTER_C1;
                pstVifUserFilter->VifSos32FilterC2  = VIF_SOS_32_FILTER_C2;
                pstVifUserFilter->VifSos32FilterC3  = VIF_SOS_32_FILTER_C3;
                pstVifUserFilter->VifSos32FilterC4  = VIF_SOS_32_FILTER_C4;
            }
            else if(eIFFreq == IF_FREQ_L)
            {
                pstVifUserFilter->VifSos21FilterC0  = VIF_SOS_21_FILTER_C0;
                pstVifUserFilter->VifSos21FilterC1  = VIF_SOS_21_FILTER_C1;
                pstVifUserFilter->VifSos21FilterC2  = VIF_SOS_21_FILTER_C2;
                pstVifUserFilter->VifSos21FilterC3  = VIF_SOS_21_FILTER_C3;
                pstVifUserFilter->VifSos21FilterC4  = VIF_SOS_21_FILTER_C4;

                pstVifUserFilter->VifSos22FilterC0  = VIF_SOS_22_FILTER_C0;
                pstVifUserFilter->VifSos22FilterC1  = VIF_SOS_22_FILTER_C1;
                pstVifUserFilter->VifSos22FilterC2  = VIF_SOS_22_FILTER_C2;
                pstVifUserFilter->VifSos22FilterC3  = VIF_SOS_22_FILTER_C3;
                pstVifUserFilter->VifSos22FilterC4  = VIF_SOS_22_FILTER_C4;

                pstVifUserFilter->VifSos31FilterC0  = VIF_SOS_31_FILTER_C0;
                pstVifUserFilter->VifSos31FilterC1  = VIF_SOS_31_FILTER_C1;
                pstVifUserFilter->VifSos31FilterC2  = VIF_SOS_31_FILTER_C2;
                pstVifUserFilter->VifSos31FilterC3  = VIF_SOS_31_FILTER_C3;
                pstVifUserFilter->VifSos31FilterC4  = VIF_SOS_31_FILTER_C4;
                pstVifUserFilter->VifSos32FilterC0  = VIF_SOS_32_FILTER_C0;
                pstVifUserFilter->VifSos32FilterC1  = VIF_SOS_32_FILTER_C1;
                pstVifUserFilter->VifSos32FilterC2  = VIF_SOS_32_FILTER_C2;
                pstVifUserFilter->VifSos32FilterC3  = VIF_SOS_32_FILTER_C3;
                pstVifUserFilter->VifSos32FilterC4  = VIF_SOS_32_FILTER_C4;
            }
            else if(eIFFreq == IF_FREQ_L_PRIME)
            {
                pstVifUserFilter->VifSos21FilterC0  = VIF_SOS_21_FILTER_C0;
                pstVifUserFilter->VifSos21FilterC1  = VIF_SOS_21_FILTER_C1;
                pstVifUserFilter->VifSos21FilterC2  = VIF_SOS_21_FILTER_C2;
                pstVifUserFilter->VifSos21FilterC3  = VIF_SOS_21_FILTER_C3;
                pstVifUserFilter->VifSos21FilterC4  = VIF_SOS_21_FILTER_C4;

                pstVifUserFilter->VifSos22FilterC0  = VIF_SOS_22_FILTER_C0;
                pstVifUserFilter->VifSos22FilterC1  = VIF_SOS_22_FILTER_C1;
                pstVifUserFilter->VifSos22FilterC2  = VIF_SOS_22_FILTER_C2;
                pstVifUserFilter->VifSos22FilterC3  = VIF_SOS_22_FILTER_C3;
                pstVifUserFilter->VifSos22FilterC4  = VIF_SOS_22_FILTER_C4;

                pstVifUserFilter->VifSos31FilterC0  = VIF_SOS_31_FILTER_C0;
                pstVifUserFilter->VifSos31FilterC1  = VIF_SOS_31_FILTER_C1;
                pstVifUserFilter->VifSos31FilterC2  = VIF_SOS_31_FILTER_C2;
                pstVifUserFilter->VifSos31FilterC3  = VIF_SOS_31_FILTER_C3;
                pstVifUserFilter->VifSos31FilterC4  = VIF_SOS_31_FILTER_C4;
                pstVifUserFilter->VifSos32FilterC0  = VIF_SOS_32_FILTER_C0;
                pstVifUserFilter->VifSos32FilterC1  = VIF_SOS_32_FILTER_C1;
                pstVifUserFilter->VifSos32FilterC2  = VIF_SOS_32_FILTER_C2;
                pstVifUserFilter->VifSos32FilterC3  = VIF_SOS_32_FILTER_C3;
                pstVifUserFilter->VifSos32FilterC4  = VIF_SOS_32_FILTER_C4;
            }
            else
            {
                   TUNER_ERR(("Error:%s(),%d\n",__func__,__LINE__));
                   return FALSE;
            }
            break;

        case E_RFBAND_VHF_HIGH:
            if((eIFFreq == IF_FREQ_B)||(eIFFreq == IF_FREQ_G))
            {
                pstVifUserFilter->VifSos21FilterC0  = VIF_SOS_21_FILTER_C0;
                pstVifUserFilter->VifSos21FilterC1  = VIF_SOS_21_FILTER_C1;
                pstVifUserFilter->VifSos21FilterC2  = VIF_SOS_21_FILTER_C2;
                pstVifUserFilter->VifSos21FilterC3  = VIF_SOS_21_FILTER_C3;
                pstVifUserFilter->VifSos21FilterC4  = VIF_SOS_21_FILTER_C4;

                pstVifUserFilter->VifSos22FilterC0  = VIF_SOS_22_FILTER_C0;
                pstVifUserFilter->VifSos22FilterC1  = VIF_SOS_22_FILTER_C1;
                pstVifUserFilter->VifSos22FilterC2  = VIF_SOS_22_FILTER_C2;
                pstVifUserFilter->VifSos22FilterC3  = VIF_SOS_22_FILTER_C3;
                pstVifUserFilter->VifSos22FilterC4  = VIF_SOS_22_FILTER_C4;

                pstVifUserFilter->VifSos31FilterC0  = VIF_SOS_31_FILTER_C0;
                pstVifUserFilter->VifSos31FilterC1  = VIF_SOS_31_FILTER_C1;
                pstVifUserFilter->VifSos31FilterC2  = VIF_SOS_31_FILTER_C2;
                pstVifUserFilter->VifSos31FilterC3  = VIF_SOS_31_FILTER_C3;
                pstVifUserFilter->VifSos31FilterC4  = VIF_SOS_31_FILTER_C4;
                pstVifUserFilter->VifSos32FilterC0  = VIF_SOS_32_FILTER_C0;
                pstVifUserFilter->VifSos32FilterC1  = VIF_SOS_32_FILTER_C1;
                pstVifUserFilter->VifSos32FilterC2  = VIF_SOS_32_FILTER_C2;
                pstVifUserFilter->VifSos32FilterC3  = VIF_SOS_32_FILTER_C3;
                pstVifUserFilter->VifSos32FilterC4  = VIF_SOS_32_FILTER_C4;
            }
            else if(eIFFreq == IF_FREQ_DK)
            {
                pstVifUserFilter->VifSos21FilterC0  = VIF_SOS_21_FILTER_C0;
                pstVifUserFilter->VifSos21FilterC1  = VIF_SOS_21_FILTER_C1;
                pstVifUserFilter->VifSos21FilterC2  = VIF_SOS_21_FILTER_C2;
                pstVifUserFilter->VifSos21FilterC3  = VIF_SOS_21_FILTER_C3;
                pstVifUserFilter->VifSos21FilterC4  = VIF_SOS_21_FILTER_C4;

                pstVifUserFilter->VifSos22FilterC0  = VIF_SOS_22_FILTER_C0;
                pstVifUserFilter->VifSos22FilterC1  = VIF_SOS_22_FILTER_C1;
                pstVifUserFilter->VifSos22FilterC2  = VIF_SOS_22_FILTER_C2;
                pstVifUserFilter->VifSos22FilterC3  = VIF_SOS_22_FILTER_C3;
                pstVifUserFilter->VifSos22FilterC4  = VIF_SOS_22_FILTER_C4;

                pstVifUserFilter->VifSos31FilterC0  = VIF_SOS_31_FILTER_C0;
                pstVifUserFilter->VifSos31FilterC1  = VIF_SOS_31_FILTER_C1;
                pstVifUserFilter->VifSos31FilterC2  = VIF_SOS_31_FILTER_C2;
                pstVifUserFilter->VifSos31FilterC3  = VIF_SOS_31_FILTER_C3;
                pstVifUserFilter->VifSos31FilterC4  = VIF_SOS_31_FILTER_C4;
                pstVifUserFilter->VifSos32FilterC0  = VIF_SOS_32_FILTER_C0;
                pstVifUserFilter->VifSos32FilterC1  = VIF_SOS_32_FILTER_C1;
                pstVifUserFilter->VifSos32FilterC2  = VIF_SOS_32_FILTER_C2;
                pstVifUserFilter->VifSos32FilterC3  = VIF_SOS_32_FILTER_C3;
                pstVifUserFilter->VifSos32FilterC4  = VIF_SOS_32_FILTER_C4;
            }
            else if(eIFFreq == IF_FREQ_I)
            {
                pstVifUserFilter->VifSos21FilterC0  = VIF_SOS_21_FILTER_C0;
                pstVifUserFilter->VifSos21FilterC1  = VIF_SOS_21_FILTER_C1;
                pstVifUserFilter->VifSos21FilterC2  = VIF_SOS_21_FILTER_C2;
                pstVifUserFilter->VifSos21FilterC3  = VIF_SOS_21_FILTER_C3;
                pstVifUserFilter->VifSos21FilterC4  = VIF_SOS_21_FILTER_C4;

                pstVifUserFilter->VifSos22FilterC0  = VIF_SOS_22_FILTER_C0;
                pstVifUserFilter->VifSos22FilterC1  = VIF_SOS_22_FILTER_C1;
                pstVifUserFilter->VifSos22FilterC2  = VIF_SOS_22_FILTER_C2;
                pstVifUserFilter->VifSos22FilterC3  = VIF_SOS_22_FILTER_C3;
                pstVifUserFilter->VifSos22FilterC4  = VIF_SOS_22_FILTER_C4;

                pstVifUserFilter->VifSos31FilterC0  = VIF_SOS_31_FILTER_C0;
                pstVifUserFilter->VifSos31FilterC1  = VIF_SOS_31_FILTER_C1;
                pstVifUserFilter->VifSos31FilterC2  = VIF_SOS_31_FILTER_C2;
                pstVifUserFilter->VifSos31FilterC3  = VIF_SOS_31_FILTER_C3;
                pstVifUserFilter->VifSos31FilterC4  = VIF_SOS_31_FILTER_C4;
                pstVifUserFilter->VifSos32FilterC0  = VIF_SOS_32_FILTER_C0;
                pstVifUserFilter->VifSos32FilterC1  = VIF_SOS_32_FILTER_C1;
                pstVifUserFilter->VifSos32FilterC2  = VIF_SOS_32_FILTER_C2;
                pstVifUserFilter->VifSos32FilterC3  = VIF_SOS_32_FILTER_C3;
                pstVifUserFilter->VifSos32FilterC4  = VIF_SOS_32_FILTER_C4;
            }
            else if(eIFFreq == IF_FREQ_L)
            {
                pstVifUserFilter->VifSos21FilterC0  = VIF_SOS_21_FILTER_C0;
                pstVifUserFilter->VifSos21FilterC1  = VIF_SOS_21_FILTER_C1;
                pstVifUserFilter->VifSos21FilterC2  = VIF_SOS_21_FILTER_C2;
                pstVifUserFilter->VifSos21FilterC3  = VIF_SOS_21_FILTER_C3;
                pstVifUserFilter->VifSos21FilterC4  = VIF_SOS_21_FILTER_C4;

                pstVifUserFilter->VifSos22FilterC0  = VIF_SOS_22_FILTER_C0;
                pstVifUserFilter->VifSos22FilterC1  = VIF_SOS_22_FILTER_C1;
                pstVifUserFilter->VifSos22FilterC2  = VIF_SOS_22_FILTER_C2;
                pstVifUserFilter->VifSos22FilterC3  = VIF_SOS_22_FILTER_C3;
                pstVifUserFilter->VifSos22FilterC4  = VIF_SOS_22_FILTER_C4;

                pstVifUserFilter->VifSos31FilterC0  = VIF_SOS_31_FILTER_C0;
                pstVifUserFilter->VifSos31FilterC1  = VIF_SOS_31_FILTER_C1;
                pstVifUserFilter->VifSos31FilterC2  = VIF_SOS_31_FILTER_C2;
                pstVifUserFilter->VifSos31FilterC3  = VIF_SOS_31_FILTER_C3;
                pstVifUserFilter->VifSos31FilterC4  = VIF_SOS_31_FILTER_C4;
                pstVifUserFilter->VifSos32FilterC0  = VIF_SOS_32_FILTER_C0;
                pstVifUserFilter->VifSos32FilterC1  = VIF_SOS_32_FILTER_C1;
                pstVifUserFilter->VifSos32FilterC2  = VIF_SOS_32_FILTER_C2;
                pstVifUserFilter->VifSos32FilterC3  = VIF_SOS_32_FILTER_C3;
                pstVifUserFilter->VifSos32FilterC4  = VIF_SOS_32_FILTER_C4;
            }
            else if(eIFFreq == IF_FREQ_L_PRIME)
            {
                pstVifUserFilter->VifSos21FilterC0  = VIF_SOS_21_FILTER_C0;
                pstVifUserFilter->VifSos21FilterC1  = VIF_SOS_21_FILTER_C1;
                pstVifUserFilter->VifSos21FilterC2  = VIF_SOS_21_FILTER_C2;
                pstVifUserFilter->VifSos21FilterC3  = VIF_SOS_21_FILTER_C3;
                pstVifUserFilter->VifSos21FilterC4  = VIF_SOS_21_FILTER_C4;

                pstVifUserFilter->VifSos22FilterC0  = VIF_SOS_22_FILTER_C0;
                pstVifUserFilter->VifSos22FilterC1  = VIF_SOS_22_FILTER_C1;
                pstVifUserFilter->VifSos22FilterC2  = VIF_SOS_22_FILTER_C2;
                pstVifUserFilter->VifSos22FilterC3  = VIF_SOS_22_FILTER_C3;
                pstVifUserFilter->VifSos22FilterC4  = VIF_SOS_22_FILTER_C4;

                pstVifUserFilter->VifSos31FilterC0  = VIF_SOS_31_FILTER_C0;
                pstVifUserFilter->VifSos31FilterC1  = VIF_SOS_31_FILTER_C1;
                pstVifUserFilter->VifSos31FilterC2  = VIF_SOS_31_FILTER_C2;
                pstVifUserFilter->VifSos31FilterC3  = VIF_SOS_31_FILTER_C3;
                pstVifUserFilter->VifSos31FilterC4  = VIF_SOS_31_FILTER_C4;
                pstVifUserFilter->VifSos32FilterC0  = VIF_SOS_32_FILTER_C0;
                pstVifUserFilter->VifSos32FilterC1  = VIF_SOS_32_FILTER_C1;
                pstVifUserFilter->VifSos32FilterC2  = VIF_SOS_32_FILTER_C2;
                pstVifUserFilter->VifSos32FilterC3  = VIF_SOS_32_FILTER_C3;
                pstVifUserFilter->VifSos32FilterC4  = VIF_SOS_32_FILTER_C4;
            }
            else
            {
                TUNER_ERR(("Error:%s(),%d\n",__func__,__LINE__));
                return FALSE;
            }
            break;

        case E_RFBAND_UHF:
            if((eIFFreq == IF_FREQ_B)||(eIFFreq == IF_FREQ_G))
            {
                pstVifUserFilter->VifSos21FilterC0  = VIF_SOS_21_FILTER_C0;
                pstVifUserFilter->VifSos21FilterC1  = VIF_SOS_21_FILTER_C1;
                pstVifUserFilter->VifSos21FilterC2  = VIF_SOS_21_FILTER_C2;
                pstVifUserFilter->VifSos21FilterC3  = VIF_SOS_21_FILTER_C3;
                pstVifUserFilter->VifSos21FilterC4  = VIF_SOS_21_FILTER_C4;

                pstVifUserFilter->VifSos22FilterC0  = VIF_SOS_22_FILTER_C0;
                pstVifUserFilter->VifSos22FilterC1  = VIF_SOS_22_FILTER_C1;
                pstVifUserFilter->VifSos22FilterC2  = VIF_SOS_22_FILTER_C2;
                pstVifUserFilter->VifSos22FilterC3  = VIF_SOS_22_FILTER_C3;
                pstVifUserFilter->VifSos22FilterC4  = VIF_SOS_22_FILTER_C4;

                pstVifUserFilter->VifSos31FilterC0  = VIF_SOS_31_FILTER_C0;
                pstVifUserFilter->VifSos31FilterC1  = VIF_SOS_31_FILTER_C1;
                pstVifUserFilter->VifSos31FilterC2  = VIF_SOS_31_FILTER_C2;
                pstVifUserFilter->VifSos31FilterC3  = VIF_SOS_31_FILTER_C3;
                pstVifUserFilter->VifSos31FilterC4  = VIF_SOS_31_FILTER_C4;
                pstVifUserFilter->VifSos32FilterC0  = VIF_SOS_32_FILTER_C0;
                pstVifUserFilter->VifSos32FilterC1  = VIF_SOS_32_FILTER_C1;
                pstVifUserFilter->VifSos32FilterC2  = VIF_SOS_32_FILTER_C2;
                pstVifUserFilter->VifSos32FilterC3  = VIF_SOS_32_FILTER_C3;
                pstVifUserFilter->VifSos32FilterC4  = VIF_SOS_32_FILTER_C4;
            }
            else if(eIFFreq == IF_FREQ_DK)
            {
                pstVifUserFilter->VifSos21FilterC0  = VIF_SOS_21_FILTER_C0;
                pstVifUserFilter->VifSos21FilterC1  = VIF_SOS_21_FILTER_C1;
                pstVifUserFilter->VifSos21FilterC2  = VIF_SOS_21_FILTER_C2;
                pstVifUserFilter->VifSos21FilterC3  = VIF_SOS_21_FILTER_C3;
                pstVifUserFilter->VifSos21FilterC4  = VIF_SOS_21_FILTER_C4;

                pstVifUserFilter->VifSos22FilterC0  = VIF_SOS_22_FILTER_C0;
                pstVifUserFilter->VifSos22FilterC1  = VIF_SOS_22_FILTER_C1;
                pstVifUserFilter->VifSos22FilterC2  = VIF_SOS_22_FILTER_C2;
                pstVifUserFilter->VifSos22FilterC3  = VIF_SOS_22_FILTER_C3;
                pstVifUserFilter->VifSos22FilterC4  = VIF_SOS_22_FILTER_C4;

                pstVifUserFilter->VifSos31FilterC0  = VIF_SOS_31_FILTER_C0;
                pstVifUserFilter->VifSos31FilterC1  = VIF_SOS_31_FILTER_C1;
                pstVifUserFilter->VifSos31FilterC2  = VIF_SOS_31_FILTER_C2;
                pstVifUserFilter->VifSos31FilterC3  = VIF_SOS_31_FILTER_C3;
                pstVifUserFilter->VifSos31FilterC4  = VIF_SOS_31_FILTER_C4;
                pstVifUserFilter->VifSos32FilterC0  = VIF_SOS_32_FILTER_C0;
                pstVifUserFilter->VifSos32FilterC1  = VIF_SOS_32_FILTER_C1;
                pstVifUserFilter->VifSos32FilterC2  = VIF_SOS_32_FILTER_C2;
                pstVifUserFilter->VifSos32FilterC3  = VIF_SOS_32_FILTER_C3;
                pstVifUserFilter->VifSos32FilterC4  = VIF_SOS_32_FILTER_C4;
            }
            else if(eIFFreq == IF_FREQ_I)
            {
                pstVifUserFilter->VifSos21FilterC0  = VIF_SOS_21_FILTER_C0;
                pstVifUserFilter->VifSos21FilterC1  = VIF_SOS_21_FILTER_C1;
                pstVifUserFilter->VifSos21FilterC2  = VIF_SOS_21_FILTER_C2;
                pstVifUserFilter->VifSos21FilterC3  = VIF_SOS_21_FILTER_C3;
                pstVifUserFilter->VifSos21FilterC4  = VIF_SOS_21_FILTER_C4;

                pstVifUserFilter->VifSos22FilterC0  = VIF_SOS_22_FILTER_C0;
                pstVifUserFilter->VifSos22FilterC1  = VIF_SOS_22_FILTER_C1;
                pstVifUserFilter->VifSos22FilterC2  = VIF_SOS_22_FILTER_C2;
                pstVifUserFilter->VifSos22FilterC3  = VIF_SOS_22_FILTER_C3;
                pstVifUserFilter->VifSos22FilterC4  = VIF_SOS_22_FILTER_C4;

                pstVifUserFilter->VifSos31FilterC0  = VIF_SOS_31_FILTER_C0;
                pstVifUserFilter->VifSos31FilterC1  = VIF_SOS_31_FILTER_C1;
                pstVifUserFilter->VifSos31FilterC2  = VIF_SOS_31_FILTER_C2;
                pstVifUserFilter->VifSos31FilterC3  = VIF_SOS_31_FILTER_C3;
                pstVifUserFilter->VifSos31FilterC4  = VIF_SOS_31_FILTER_C4;
                pstVifUserFilter->VifSos32FilterC0  = VIF_SOS_32_FILTER_C0;
                pstVifUserFilter->VifSos32FilterC1  = VIF_SOS_32_FILTER_C1;
                pstVifUserFilter->VifSos32FilterC2  = VIF_SOS_32_FILTER_C2;
                pstVifUserFilter->VifSos32FilterC3  = VIF_SOS_32_FILTER_C3;
                pstVifUserFilter->VifSos32FilterC4  = VIF_SOS_32_FILTER_C4;
            }
            else if(eIFFreq == IF_FREQ_L)
            {
                pstVifUserFilter->VifSos21FilterC0  = VIF_SOS_21_FILTER_C0;
                pstVifUserFilter->VifSos21FilterC1  = VIF_SOS_21_FILTER_C1;
                pstVifUserFilter->VifSos21FilterC2  = VIF_SOS_21_FILTER_C2;
                pstVifUserFilter->VifSos21FilterC3  = VIF_SOS_21_FILTER_C3;
                pstVifUserFilter->VifSos21FilterC4  = VIF_SOS_21_FILTER_C4;

                pstVifUserFilter->VifSos22FilterC0  = VIF_SOS_22_FILTER_C0;
                pstVifUserFilter->VifSos22FilterC1  = VIF_SOS_22_FILTER_C1;
                pstVifUserFilter->VifSos22FilterC2  = VIF_SOS_22_FILTER_C2;
                pstVifUserFilter->VifSos22FilterC3  = VIF_SOS_22_FILTER_C3;
                pstVifUserFilter->VifSos22FilterC4  = VIF_SOS_22_FILTER_C4;

                pstVifUserFilter->VifSos31FilterC0  = VIF_SOS_31_FILTER_C0;
                pstVifUserFilter->VifSos31FilterC1  = VIF_SOS_31_FILTER_C1;
                pstVifUserFilter->VifSos31FilterC2  = VIF_SOS_31_FILTER_C2;
                pstVifUserFilter->VifSos31FilterC3  = VIF_SOS_31_FILTER_C3;
                pstVifUserFilter->VifSos31FilterC4  = VIF_SOS_31_FILTER_C4;
                pstVifUserFilter->VifSos32FilterC0  = VIF_SOS_32_FILTER_C0;
                pstVifUserFilter->VifSos32FilterC1  = VIF_SOS_32_FILTER_C1;
                pstVifUserFilter->VifSos32FilterC2  = VIF_SOS_32_FILTER_C2;
                pstVifUserFilter->VifSos32FilterC3  = VIF_SOS_32_FILTER_C3;
                pstVifUserFilter->VifSos32FilterC4  = VIF_SOS_32_FILTER_C4;
            }
            else if(eIFFreq == IF_FREQ_L_PRIME)
            {
                pstVifUserFilter->VifSos21FilterC0  = VIF_SOS_21_FILTER_C0;
                pstVifUserFilter->VifSos21FilterC1  = VIF_SOS_21_FILTER_C1;
                pstVifUserFilter->VifSos21FilterC2  = VIF_SOS_21_FILTER_C2;
                pstVifUserFilter->VifSos21FilterC3  = VIF_SOS_21_FILTER_C3;
                pstVifUserFilter->VifSos21FilterC4  = VIF_SOS_21_FILTER_C4;

                pstVifUserFilter->VifSos22FilterC0  = VIF_SOS_22_FILTER_C0;
                pstVifUserFilter->VifSos22FilterC1  = VIF_SOS_22_FILTER_C1;
                pstVifUserFilter->VifSos22FilterC2  = VIF_SOS_22_FILTER_C2;
                pstVifUserFilter->VifSos22FilterC3  = VIF_SOS_22_FILTER_C3;
                pstVifUserFilter->VifSos22FilterC4  = VIF_SOS_22_FILTER_C4;

                pstVifUserFilter->VifSos31FilterC0  = VIF_SOS_31_FILTER_C0;
                pstVifUserFilter->VifSos31FilterC1  = VIF_SOS_31_FILTER_C1;
                pstVifUserFilter->VifSos31FilterC2  = VIF_SOS_31_FILTER_C2;
                pstVifUserFilter->VifSos31FilterC3  = VIF_SOS_31_FILTER_C3;
                pstVifUserFilter->VifSos31FilterC4  = VIF_SOS_31_FILTER_C4;
                pstVifUserFilter->VifSos32FilterC0  = VIF_SOS_32_FILTER_C0;
                pstVifUserFilter->VifSos32FilterC1  = VIF_SOS_32_FILTER_C1;
                pstVifUserFilter->VifSos32FilterC2  = VIF_SOS_32_FILTER_C2;
                pstVifUserFilter->VifSos32FilterC3  = VIF_SOS_32_FILTER_C3;
                pstVifUserFilter->VifSos32FilterC4  = VIF_SOS_32_FILTER_C4;
            }
            else
            {
                TUNER_ERR(("Error:%s(),%d\n",__func__,__LINE__));
                return FALSE;
            }
            break;
        default:
            TUNER_ERR(("Error:%s(),%d\n",__func__,__LINE__));
            return FALSE;
            break;
    }

    return TRUE;
}
#endif

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_MXL661) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_MXL661",          // demod system name
    .data               = TUNER_MXL661,            // private data value
    .Init               = MDrv_Tuner_MXL661_Init,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_Tuner_MXL661_SetTuner,
    .CheckExist         = MDrv_Tuner_MXL661_CheckExist,
    .GetTunerIF         = MDrv_Tuner_MXL661_GetTunerIF,
    .Extension_Function = MDrv_Tuner_MXL661_Extension_Function,
    .SetATVTuner        = MDrv_Tuner_MXL661_SetATVTuner
};


#endif
