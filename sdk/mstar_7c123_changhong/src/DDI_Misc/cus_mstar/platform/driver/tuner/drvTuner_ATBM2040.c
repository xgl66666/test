#ifndef __KERNEL__
#include <stdio.h>
#else
#include <linux/slab.h>
#define free vfree
#define malloc(size) vmalloc((size), GFP_KERNEL)
#endif
#include "error.h"
#include "MsCommon.h"
#include "Board.h"
#include "drvTuner.h"
#include "drvTunerNull.h"


#if IF_THIS_TUNER_INUSE(TUNER_ATBM2040)
#include "ATBM2040Api.h"
#include "ATBM2040Driver.h"

static TUNER_MS_INIT_PARAM InitParam[MAX_FRONTEND_NUM]; //MAX_FRONTEND_NUM
static MS_U8 u8Possible_SLAVE_IDs[2] = {0xC0, 0xD0};

MS_BOOL MDrv_ATBM2040_Init(MS_U8 u8TunerIndex, TUNER_MS_INIT_PARAM* pParam)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    ATBM2040InitConfig_t InitConfig;
    MS_U32 DevId = 0;

    /*Get the default configuration from ATBM2040 SDK. */
    if (ATBM2040_NO_ERROR != ATBM2040DefaultCfgGet(u8TunerIndex, &InitConfig))
    {
        return FALSE;
    }

    if (pParam->pCur_Broadcast_type == NULL)
        return FALSE;
    else
        InitParam[u8TunerIndex].pCur_Broadcast_type = pParam->pCur_Broadcast_type;

    /*Change some setting according to actual requirement.*/
    InitConfig.I2CAddr = InitParam[u8TunerIndex].u8SlaveID;
    InitConfig.OSCCap.CalibValue = 0x1F; /*OSC PPM calibration, from 0x00 to 0x1F. The larger the value, the higher the clock frequency*/
    InitConfig.Mode = ATBM2040_SIGNAL_MODE_DVBC;
    InitConfig.DtvIFOut.IFOutFreqHz = 5000000;/* 5MHz */
    InitConfig.DtvIFOut.IFOutLevel = ATBM2040_IF_OUT_LEVEL2;
    InitConfig.AtvIFOut.IFOutFreqHz = 5000000;/* 5MHz */
    InitConfig.AtvIFOut.IFOutLevel = ATBM2040_IF_OUT_LEVEL1;
    /*Initialize with custumer parameters.*/
    ret = ATBM2040Init(u8TunerIndex, DevId, &InitConfig);
    if (ATBM2040_NO_ERROR != ret)
    {
        return FALSE;
    }

    return TRUE;

}
MS_BOOL MDrv_ATBM2040_SetTuner(MS_U8 u8TunerIndex, MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/)
{
    ATBM2040_ERROR_e ret = ATBM2040_NO_ERROR;
    MS_U32 DevId = 0;
    MS_U32 BandWidthKHz;
    BOOL LockStatus;
    ATBM2040_SIGNAL_MODE_e Mode;
    if (*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBC)
        Mode = ATBM2040_SIGNAL_MODE_DVBC;
    else if ((*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBT) || (*InitParam[u8TunerIndex].pCur_Broadcast_type == DVBT2))
        Mode =  ATBM2040_SIGNAL_MODE_DVBT;
    else if (*InitParam[u8TunerIndex].pCur_Broadcast_type == DTMB)
        Mode = ATBM2040_SIGNAL_MODE_DTMB;
    else
        Mode = ATBM2040_SIGNAL_MODE_ATV;
    switch (ucBw)
    {
        case 0:
            BandWidthKHz = 6000;
            break;
        case 1:
            BandWidthKHz = 7000;
            break;
        case 2:
            BandWidthKHz = 8000;
            break;
        default:
            BandWidthKHz = 8000;
            break;
    }

    ret = ATBM2040ChannelTune(u8TunerIndex, DevId, Mode, dwFreq, BandWidthKHz, ATBM2040_SPECTRUM_NORMAL);
    if (ATBM2040_NO_ERROR != ret)
    {
        return FALSE;
    }
    /*Check tuner lock status.*/
    ret = ATBM2040GetLockStatus(u8TunerIndex, DevId, &LockStatus);
    if (LockStatus)
    {
        printf("Tuner is locked!\n");
        return TRUE;
    }
    else
    {
        printf("Tuner is not locked!\n");
        return FALSE;
    }

}
MS_BOOL MDrv_ATBM2040_CheckExist(MS_U8 u8TunerIndex, MS_U32* pu32channel_cnt)
{
    MS_U8  chipId;
    MS_U8 i, u8CurID;

    for (i = 0; i < sizeof(u8Possible_SLAVE_IDs); i++)
    {
        u8CurID = u8Possible_SLAVE_IDs[i];
        if (ATBM2040_NO_ERROR != ATBM2040RegRead(u8TunerIndex, u8CurID, 0x00, 0x00, &chipId))
        {
            TUNER_ERR(("[ATBM2040] Read chip ID fail with slave ID 0x%x\n", u8CurID));
        }
        else
        {
            InitParam[u8TunerIndex].u8SlaveID = u8CurID;
            break;
        }
    }

    TUNER_DBG(("[ATBM2040] read id =0x%x\n", chipId));

    if (chipId == ATBM_LEOG_CHIP_ID || chipId == ATBM_LEOF_CHIP_ID || chipId == ATBM_LEOE_CHIP_ID || chipId == ATBM_LEOD_CHIP_ID || \
            chipId == ATBM_LEOC_CHIP_ID || chipId == ATBM_LEOB_CHIP_ID || chipId == ATBM_LEO_CHIP_ID)
    {
        if (NULL != pu32channel_cnt)
            *(pu32channel_cnt) += 1;
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

MS_BOOL MDrv_ATBM2040_GetTunerIF(MS_U8 u8TunerIndex, MS_U32* u32IF_Freq)
{
    *u32IF_Freq = 5000;
    return TRUE;
}

MS_BOOL MDrv_ATBM2040_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_U32 DevId = 0;
    SINT32 RSSI;
    switch (fuction_type)
    {
        case TUNER_EXT_FUNC_GET_POWER_LEVEL:
            ATBM2040GetRSSI(u8TunerIndex, DevId, &RSSI);
            *(float*)data = RSSI;
            break;
        case TUNER_EXT_FUNC_POWER_ON_OFF:
            if (FALSE == *(MS_BOOL *)data) //power off
            {
                ATBM2040Standby(u8TunerIndex, DevId);
            }
            else
            {
                ATBM2040Wakeup(u8TunerIndex, DevId);
            }
            break;
        case TUNER_EXT_FUNC_STANDBY:
            ATBM2040Standby(u8TunerIndex, DevId);
            break;
        case TUNER_EXT_FUNC_WAKEUP:
            ATBM2040Wakeup(u8TunerIndex, DevId);
            break;

        default:
            TUNER_DBG(("Request extension function (%x) does not exist\n", fuction_type));
            break;
    }
    return TRUE;
}

DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_ATBM2040) DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_ATBM2040",          // demod system name
    .data               = TUNER_ATBM2040,            // private data value
    .Init               = MDrv_ATBM2040_Init,
    .SetFreq            = MDrv_Tuner_Null_SetFreq,
    .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_Null_GetLock,
    .SetTuner           = MDrv_ATBM2040_SetTuner,
    .CheckExist         = MDrv_ATBM2040_CheckExist,
    .GetTunerIF         = MDrv_ATBM2040_GetTunerIF,
    .Extension_Function = MDrv_ATBM2040_Extension_Function
};
#endif

