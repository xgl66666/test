//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
///
/// file    apiDigiTuner.c
/// @brief  Digital Tuner Low-Level API
/// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include "MsCommon.h"
#include "HbCommon.h"
//#include "Board.h"
//#include "drvTuner.h"
//#include "drvDemod.h"
#include "drvIIC.h"
#include "drvHWI2C.h"
#include "drvGPIO.h"
#include "drvDTC.h"
//#if MS_DVBS_INUSE
#include "math.h"
//#include "drvDish.h"
//#endif
#include "apiDigiTuner.h"
#include "MsCommon.h"
#if defined(KGC_USE_ON_CHIP_MSB1237) && (KGC_USE_ON_CHIP_MSB1237 == 1)
#include "drvMMIO.h"
#endif
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define TUNER_MUTEX_TIMEOUT         500

/* Lock Timeout*/
#define TUNE_T_TIMEOUT               8000
#define TUNE_T2_TIMEOUT              10000

#define X4CFE_ENABLE_MODE       TRUE
#define PPD_ENABLE_MODE         FALSE //TRUE
#define IQ_AUTO_ENABLE_MODE     TRUE
#define QAM_AUTO_ENABLE_MODE    FALSE
#define FHO_ENABLE_MODE         FALSE

#define ENTRY()                                                         \
    if (HB_ObtainMutex(_s32MutexId, TUNER_MUTEX_TIMEOUT) == FALSE)    \
    {   FE_ERR(("%s: Obtain mutex failed.\n", __FUNCTION__));              \
        return FALSE;                                                   \
    }

#define RETURN(_ret)                                                    \
    { HB_ReleaseMutex(_s32MutexId);                                     \
      return _ret; }

#if MS_DVBS_INUSE
//for diseqc USALS
#define TO_RADS (M_PI / 180.0)
#define TO_DEC (180.0 / M_PI)

//for blind scan
#define IF_MIN_FREQ      950
#define IF_MIDDLE_FREQ 1550
#define IF_MAX_FREQ     2150
#define IF_OFFSET_FREQ  25

#define DISABLE_SENT_SAME_DISEQC_CMD   FALSE
#endif

static MS_S32 gs32FECachedPoolID = INVALID_POOL_ID;
static MS_S32 gs32FENonCachedPoolID = INVALID_POOL_ID;

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//  Global Variables
//--------------------------------------------------------------------------------------------------
#if MS_DVBS_INUSE
extern MS_U16 MApi_DB_DFT_GetPCNBySatIDAndFreq(MS_U8 u8SatId,MS_U32 u32Freq,MS_U8 u8Poraly);
struct drv_dishtab_entry *drv_dishtab = NULL;
struct drv_dishtab_entry *drv_dishtab_end = NULL;
#endif
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
EXT_DMD_INFO* pu16ExtDMD_Info = NULL;
DEV_FRONTEND_TYPE** pFETabAddr = NULL;
MS_U8* _pu8Init = NULL;
MS_FE_CARRIER_PARAM* _ptunerParam = NULL;
MS_U32 FE_Reset_Pin[MAX_FRONTEND_NUM]= {};
static MS_BOOL FE_Load_Reset_Pin = FALSE;
static MS_BOOL FE_Detect_Done = FALSE;
static MS_U32 u32ch_acc[MAX_FRONTEND_NUM] = {0};
//static MS_U16 u16Demod_on_I2C[4] = {0,0,0,0};
static MS_U32 DefaultBroadcastType = BROADCAST_TYPE_NOT_SEETING;
static EN_FRONTEND_DETECT_MODE eDetect_Mode = DETECT_MODE_NOT_SETTING;

static MS_S32                          _s32MutexId = -1;
#if MS_DVBS_INUSE
MS_SAT_PARAM* _pstSATParam = NULL;
static MS_BOOL bIsHiLOF = FALSE;
#endif

#if MS_DVBT2_INUSE
static MS_U8    T2PlpID = 0xFF;
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _DigiTuner_Conflit_Check(MS_U8 u8I2C_Idx, MS_U32 u32DMDType, MS_U8 u8SlaveID)
{
    MS_U8 i;
    EXT_DMD_INFO* pu16Info = NULL;
    if(NULL == pu16ExtDMD_Info)
        return FALSE;

    for(i=0; i< MAX_FRONTEND_NUM; i++)
    {
        pu16Info = pu16ExtDMD_Info + i;
        if(pu16Info->bExist)
        {
            if(pu16Info->u8I2C_Port == u8I2C_Idx)
            {
                printf("== DMD Conflit Check: On the same I2C bus\n");
                if(pu16Info->u32DMD_TYPE == u32DMDType)
                {
                    printf("== DMD Conflit Check: Hit same Demod Type\n");
                    if(pu16Info->u8SlaveID== u8SlaveID)
                    {
                        printf("== DMD Conflit Check: Hit same slave ID\n");
                        return TRUE;
                    }
                }
            }
        }
    }

    for(i=0; i<= MAX_FRONTEND_NUM; i++)
    {
        pu16Info = pu16ExtDMD_Info + i;
        if(pu16Info->bExist == FALSE)
            break;
    }
    pu16Info->u8I2C_Port = u8I2C_Idx;
    pu16Info->u32DMD_TYPE = u32DMDType;
    pu16Info->u8SlaveID = u8SlaveID;
    pu16Info->bExist = TRUE;
    return FALSE;
}
static MS_U8 _DigiTuner_MaxFEIdx(void)
{
    MS_U8 u8tmp = 0, IIC_Idx;

    for(IIC_Idx=0; IIC_Idx< MAX_FRONTEND_NUM; IIC_Idx++)
    {
        if(u8tmp < u32ch_acc[IIC_Idx])
            u8tmp = u32ch_acc[IIC_Idx];
    }

    if(u8tmp != 0)
        return (u8tmp - 1);
    else
        return FEIndex_NONE;
}

static MS_U8 _DigiTuner_FEIdx2DevChanIdx(MS_U8 drv_frontend_index)
{
    MS_U8 u8Dev_Idx;
    MS_U8 ch_cnt = 0;

    for(u8Dev_Idx = 0; u8Dev_Idx< MAX_FRONTEND_NUM; u8Dev_Idx++)
    {
        if(drv_frontend_index < u32ch_acc[u8Dev_Idx])
            break;
    }

    if(!u8Dev_Idx)
        return drv_frontend_index;
    else
    {
        ch_cnt = u32ch_acc[u8Dev_Idx] - u32ch_acc[u8Dev_Idx - 1];
        return drv_frontend_index - u32ch_acc[u8Dev_Idx - 1] +  ch_cnt* u8Dev_Idx;
    }
}

static MS_U8 _DigiTuner_FEIdx2DevIdx(MS_U8 drv_frontend_index)
{
    MS_U8 u8Dev_Idx;

    for(u8Dev_Idx = 0; u8Dev_Idx< MAX_FRONTEND_NUM; u8Dev_Idx++)
    {
        if(drv_frontend_index < u32ch_acc[u8Dev_Idx])
            break;
    }

    if(u8Dev_Idx == MAX_FRONTEND_NUM)
        return (MAX_FRONTEND_NUM - 1);
    else
        return u8Dev_Idx;
}

static MS_BOOL _DigiTuner_CheckFEtab_Exist(MS_U8 drv_frontend_index)
{
    DEV_FRONTEND_TYPE* pFETab = NULL;
    if((drv_frontend_index > _DigiTuner_MaxFEIdx()) || (_DigiTuner_MaxFEIdx() == FEIndex_NONE) || (NULL == pFETabAddr))
        return FALSE;

    pFETab = *(pFETabAddr + drv_frontend_index);
    if(NULL == pFETab)
        return FALSE;

    return TRUE;
}
static void _DigiTuner_ReleaseFETab(void)
{
    DEV_FRONTEND_TYPE* pFETab = NULL;
    MS_U8 u8FEIdx, u8IICIdx;

    if(NULL != pFETabAddr)
    {
        for(u8FEIdx=0; u8FEIdx < _DigiTuner_MaxFEIdx(); u8FEIdx++)
        {
            pFETab = *(pFETabAddr + u8FEIdx);
            MsOS_FreeMemory(pFETab, gs32FECachedPoolID);
        }
        MsOS_FreeMemory(pFETabAddr, gs32FECachedPoolID);
        pFETabAddr = NULL;
    }

    if(NULL != _pu8Init)
    {
        MsOS_FreeMemory(_pu8Init, gs32FECachedPoolID);
        _pu8Init = NULL;
    }

    if(NULL != _ptunerParam)
    {
        MsOS_FreeMemory(_ptunerParam, gs32FECachedPoolID);
        _ptunerParam = NULL;
    }

#if MS_DVBS_INUSE
    if(NULL != _pstSATParam)
    {
        MsOS_FreeMemory(_pstSATParam, gs32FECachedPoolID);
        _pstSATParam = NULL;
    }
#endif
    for(u8IICIdx = 0; u8IICIdx < MAX_FRONTEND_NUM; u8IICIdx++)
        u32ch_acc[u8IICIdx] = 0;
}


#if MS_DVBS_INUSE
static MS_BOOL _DigiTuner_Satellite_Set22K(MS_U8 drv_frontend_index, MS_BOOL b22KOn)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevIdx(drv_frontend_index);
    MS_BOOL bRet = TRUE;
    DEV_FRONTEND_TYPE* pFETab;
    MS_SAT_PARAM* pSATParam;

    if((NULL == _pstSATParam)|| (FALSE == _DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        return FALSE;

    pSATParam = _pstSATParam + drv_frontend_index;
    pFETab = *(pFETabAddr + drv_frontend_index);

    if(b22KOn)
    {
        if(pSATParam->e22KOnOff != EN_22K_AUTO)
            pSATParam->e22KOnOff = EN_22K_ON;
        bRet = pFETab->dishtab->Set22k(u8Idx,EN_LNB22K_TYPE_ON);
    }
    else
    {
        if(pSATParam->e22KOnOff != EN_22K_AUTO)
            pSATParam->e22KOnOff = EN_22K_OFF;
        bRet = pFETab->dishtab->Set22k(u8Idx,EN_LNB22K_TYPE_OFF);
    }
    return bRet;
}

static MS_BOOL _DigiTuner_Satellite_Set22KTone(MS_U8 drv_frontend_index, EN_TONEBURST_TYPE enToneType)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevIdx(drv_frontend_index);
    MS_BOOL bRet = TRUE;
    DEV_FRONTEND_TYPE* pFETab;

    if(FALSE == _DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        return FALSE;

    pFETab = *(pFETabAddr + drv_frontend_index);
    bRet = pFETab->dishtab->SetTone(u8Idx,enToneType);

    return bRet;
}


static MS_BOOL _DigiTuner_SetLNBPower(MS_U8 drv_frontend_index, MS_EN_LNBPWR_ONOFF enLNBPower,MS_U8 u8Polarity,MS_BOOL bPorInvert)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    if(FALSE == _DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        return FALSE;

    pFETab = *(pFETabAddr + drv_frontend_index);

    switch(enLNBPower)
    {
    default:
    case  EN_LNBPWR_13OR18V:
    {
        if( (!bPorInvert && u8Polarity) || (bPorInvert && u8Polarity == 0)) //V
        {
            return pFETab->dishtab->SetLNBPower(u8Idx,EN_LNBPWR_TYPE_13V);
        }
        else  //H
        {
            return pFETab->dishtab->SetLNBPower(u8Idx, EN_LNBPWR_TYPE_18V);
        }
        break;
    }

    case EN_LNBPWR_OFF:
    {
        return pFETab->dishtab->SetLNBPower(u8Idx,EN_LNBPWR_TYPE_OFF);
        break;
    }

    case EN_LNBPWR_13V:
    {
        return pFETab->dishtab->SetLNBPower(u8Idx, EN_LNBPWR_TYPE_13V);
        break;
    }
    case EN_LNBPWR_18V:
    {
        return pFETab->dishtab->SetLNBPower(u8Idx, EN_LNBPWR_TYPE_18V);
        break;
    }
    }
    MsOS_DelayTask(100);
    return TRUE;
}

static MS_BOOL _DigiTuner_DiSEqC_SendCmd(MS_U8 drv_frontend_index, MS_U8* pCmd,MS_U8 u8CmdSize)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevIdx(drv_frontend_index);
    MS_BOOL bRet = TRUE;
    DEV_FRONTEND_TYPE* pFETab;
    MS_SAT_PARAM* pSATParam;
    MS_U32 u32DMD_TYPE;

    if((NULL == _pstSATParam)|| (FALSE == _DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        return FALSE;

    pFETab = *(pFETabAddr + drv_frontend_index);
    pSATParam = _pstSATParam + drv_frontend_index;
    u32DMD_TYPE = pFETab->demodtab->data;


#if DISABLE_SENT_SAME_DISEQC_CMD
    if (u8CmdSize < MAX_DISEQC_CMD_SIZE)
    {
        if (0 == memcmp(pCmd, pFETab->u8DiseqcCmd, u8CmdSize) && (pFETab->eCurrentLockStatus == E_DEMOD_LOCK))
        {
                FE_DBG(("%s, %d, same DiSeqc cmd \r\n", __FUNCTION__, __LINE__));
                return TRUE;
        }

        memset(pFETab->u8DiseqcCmd, 0x00, MAX_DISEQC_CMD_SIZE);
        memcpy(pFETab->u8DiseqcCmd, pCmd, u8CmdSize);
    }
    else
    {
        FE_DBG(("\n special diseqc cmd"));
    }
#endif

    if ((u32DMD_TYPE != DEMOD_MSB131X) && (u32DMD_TYPE != DEMOD_MSB124X) && (u32DMD_TYPE != DEMOD_MSB1245) &&\
            (u32DMD_TYPE != DEMOD_MSKRATOS_DVBS) && (u32DMD_TYPE != DEMOD_MSKRIS_DVBS) && (u32DMD_TYPE != DEMOD_MSKELTIC_DVBS))
    {
        _DigiTuner_Satellite_Set22K(drv_frontend_index, FALSE);
    }

    bRet = pFETab->dishtab->SendCmd(u8Idx, pCmd,u8CmdSize);
    if ((u32DMD_TYPE != DEMOD_MSB131X) && (u32DMD_TYPE != DEMOD_MSB124X) && (u32DMD_TYPE != DEMOD_MSB1245) &&\
            (u32DMD_TYPE != DEMOD_MSKRATOS_DVBS) && (u32DMD_TYPE != DEMOD_MSKRIS_DVBS) && (u32DMD_TYPE != DEMOD_MSKELTIC_DVBS))
    {
        if(pSATParam->e22KOnOff == EN_22K_ON
                ||(pSATParam->e22KOnOff == EN_22K_AUTO && bIsHiLOF))
        {
            _DigiTuner_Satellite_Set22K(drv_frontend_index, TRUE);
        }
    }
    return bRet;
}

static MS_BOOL _DiSEqC_SwitchPort(MS_U8 drv_frontend_index, MS_U8 u8Port,MS_BOOL bUncommitted)
{
    MS_U8 u8Cmd[]= {0xE2,0x10,0x38,0xF0};

    if(bUncommitted)
    {
        u8Cmd[2] = 0x39;
    }
    u8Cmd[3] = 0xF0+u8Port;

    return  _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, u8Cmd,sizeof(u8Cmd));
}

static MS_BOOL _DiSEqC_GotoSatPosition(MS_U8 drv_frontend_index, MS_U8 u8Pos)
{
    MS_U8 u8Cmd[]= {0xE0,0x31,0x6B,u8Pos};
    MS_SAT_PARAM* pSATParam;

    if(NULL == _pstSATParam || (FALSE == _DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        return FALSE;

    pSATParam = _pstSATParam + drv_frontend_index;
    u8Pos &= DISEQC12_USALS_POSITION_MASK;
    if(u8Pos == 0)
    {
        return FALSE;
    }
    pSATParam->u8Position = u8Pos;
    return  _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, u8Cmd,sizeof(u8Cmd));
}

static MS_S16 _CulculateAzimuth(MS_U16 u16SatLongitude,MS_S16 s16MyLongitude,MS_S16 s16MyLatitude)
{
    double P, Ue, Us, az, x, el, Azimuth;
    MS_S16 s16Azimuth;

    P = s16MyLatitude/10 + ((double)(s16MyLatitude%10)*0.1);
    P = P*TO_RADS;           // Earth Station Latitude
    Ue = s16MyLongitude/10 + ((double)(s16MyLongitude%10)*0.1);
    Ue = Ue*TO_RADS;           // Earth Station Longitude

    if(P < 0)
    {
        P = -P;
    }

    if(u16SatLongitude >1800)
    {
        u16SatLongitude = 3600 - u16SatLongitude;
        Us = -( u16SatLongitude/10 + ((double)(u16SatLongitude%10)*0.1));
    }
    else
    {
        Us = u16SatLongitude/10 + ((double)(u16SatLongitude%10)*0.1);
    }
    Us = Us*TO_RADS;          // Satellite Longitude

    az = M_PI+atan( tan( Us-Ue )/sin( P ) );
    x = acos( cos(Us-Ue)*cos(P) );
    el = atan( ( cos( x )-0.1513 )/sin( x ) );
    Azimuth = atan( ( -cos(el)*sin(az) )/( sin(el)*cos(P)-cos(el)*sin(P)*cos(az)) )* TO_DEC;
    s16Azimuth = (MS_S16)((Azimuth+0.05)*10);

    if(s16MyLatitude < 0)
    {
        s16Azimuth = -s16Azimuth;
    }

    return s16Azimuth;
}

static MS_BOOL _DiSEqC_GotoX(MS_U8 drv_frontend_index, MS_U16 u16SatLongitude,MS_S16 s16MyLongitude,MS_S16 s16MyLatitude)
{
    MS_U8 u8Cmd[5]= {0xE0,0x31,0x6E,0x00,0x00};
    MS_U8 DecimalLookup[10] = { 0x00, 0x02, 0x03, 0x05, 0x06, 0x08, 0x0A, 0x0B, 0x0D, 0x0E };
    MS_S16 s16Azimuth;
    MS_U16 u16Integer,u16Fraction;

    s16Azimuth = _CulculateAzimuth(u16SatLongitude,s16MyLongitude,s16MyLatitude);
    FE_DBG(("\n s16Azimuth = %d",s16Azimuth));
    if ( s16Azimuth > 0 )
        u8Cmd[3] = 0xE0;    // East
    else
        u8Cmd[3] = 0xD0;      // West

    u16Integer = abs( s16Azimuth );
    u16Fraction = u16Integer%10;
    u16Integer = u16Integer /10;

    while (u16Integer >= 16)
    {
        u8Cmd[3]++;
        u16Integer-= 16;
    }
    while (u16Integer >= 1)
    {
        u8Cmd[4]+=0x10;
        u16Integer--;
    }
    u8Cmd[4]+= DecimalLookup[u16Fraction];
    FE_DBG(("\nu8Cmd[3] = 0x%X",u8Cmd[3]));
    FE_DBG(("\nu8Cmd[4] = 0x%X",u8Cmd[4]));

    return _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, u8Cmd,sizeof(u8Cmd));
}

static MS_BOOL _MultiDiSEqc_SwitchPort(MS_U8 drv_frontend_index, MS_SAT_PARAM *pSATParam,MS_U8 u8Polarity,MS_BOOL bIsHiLOF)
{
    MS_U8 u8SubPort = 0;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;
    MS_SAT_PARAM * psatParam;
    psatParam = _pstSATParam + drv_frontend_index;

    if(FALSE == _DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        return FALSE;

    pFETab = *(pFETabAddr + drv_frontend_index);

    _DigiTuner_Satellite_Set22K(drv_frontend_index, FALSE);

    if(pSATParam->eToneburstType == EN_TONEBURST_1)
    {
        pFETab->dishtab->SetTone(u8Idx, EN_TONEBURST_TYPE_1);
    }
    else if(pSATParam->eToneburstType == EN_TONEBURST_0)
    {
        pFETab->dishtab->SetTone(u8Idx, EN_TONEBURST_TYPE_0);
    }
    MsOS_DelayTask(10);
    if(u8Polarity == 0)
        u8SubPort |= 0x02;
    if(bIsHiLOF)
        u8SubPort |= 0x01;
    switch(pSATParam->eSwt10Port)
    {
    case EN_SWT_PORT_1:
        u8SubPort = u8SubPort;
        break;
    case EN_SWT_PORT_2:
        u8SubPort = 0x04+ u8SubPort;
        break;
    case EN_SWT_PORT_3:
        u8SubPort = 0x08+ u8SubPort;
        break;
    case EN_SWT_PORT_4:
        u8SubPort = 0x0C+ u8SubPort;
        break;
    default:
        break;
    }

    if(pSATParam->eSwt10Port >= EN_SWT_PORT_1 && pSATParam->eSwt10Port <= EN_SWT_PORT_4)
    {
        _DiSEqC_SwitchPort(drv_frontend_index, u8SubPort,FALSE);
    }
    MsOS_DelayTask(50);
    if(pSATParam->eSwt11Port >= EN_SWT_PORT_1 && pSATParam->eSwt11Port <= EN_SWT_PORT_16)
    {
        _DiSEqC_SwitchPort(drv_frontend_index, (MS_U8)(pSATParam->eSwt11Port)-1,TRUE);
    }
    MsOS_DelayTask(50);
#if   DISEQC13_ONLY_GOTOX
    if(IsUSALS(*pSATParam)&&!IsUSALS(*pSATParam))
    {
        MS_USER_SYSTEM_SETTING stSysSetting;
        MApp_GetSysSetting(&stSysSetting);
        _DiSEqC_GotoX(drv_frontend_index, pSATParam->u16Angle,SetSATLocation[stSysSetting.u8Location].MyLongitude,SetSATLocation[stSysSetting.u8Location].MyLatitude);
        MsOS_DelayTask(50);
    }
    else if((pSATParam->u8Position&DISEQC12_USALS_POSITION_MASK) != (pSATParam->u8Position&DISEQC12_USALS_POSITION_MASK))
#else
    if((pSATParam->u8Position&DISEQC12_USALS_POSITION_MASK) != (psatParam->u8Position&DISEQC12_USALS_POSITION_MASK))
#endif
    {
#if 0
        AppCommand appCmd;
        appCmd.eCmd = E_APP_CMD_MOVE_LNB;
        appCmd.eFlg = E_APP_CMDFLG_NONE;
        appCmd.eSrcAppType = E_APP_TYPE_ALL;
        appCmd.eDstAppType = E_APP_TYPE_MENU;
        appCmd.u8Data[0] = SAT_MOVE_TIME;
        appMain_RecvNotify(&appCmd);
#endif
        _DiSEqC_GotoSatPosition(drv_frontend_index, (pSATParam->u8Position&DISEQC12_USALS_POSITION_MASK));
        MsOS_DelayTask(50);
    }

    if(pSATParam->e22KOnOff == EN_22K_ON
            ||(pSATParam->e22KOnOff == EN_22K_AUTO && bIsHiLOF))
    {
        _DigiTuner_Satellite_Set22K(drv_frontend_index, TRUE);
    }

    return TRUE;
}

static MS_BOOL  _DigiTuner_SwitchSatellite(MS_U8 drv_frontend_index, MS_SAT_PARAM *pSATParam,MS_U8 u8Polarity,MS_BOOL bIsHiLOF)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    if(FALSE == _DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        return FALSE;

    pFETab = *(pFETabAddr + drv_frontend_index);
    // switch the port
    _DigiTuner_Satellite_Set22K(drv_frontend_index, FALSE);

    switch(pSATParam->eDiseqcLevel)
    {
    default:
    case EN_DISEQC_LVL_OFF:
    {
        if(pSATParam->eToneburstType == EN_TONEBURST_1)
        {
            pFETab->dishtab->SetTone(u8Idx, EN_TONEBURST_TYPE_1);
        }
        else if(pSATParam->eToneburstType == EN_TONEBURST_0)
        {
            pFETab->dishtab->SetTone(u8Idx, EN_TONEBURST_TYPE_0);
        }
        break;
    }

    case EN_DISEQC_LVL_10:
    {
        MS_U8 u8SubPort = 0;
        if(pSATParam->eToneburstType == EN_TONEBURST_1)
        {
            pFETab->dishtab->SetTone(u8Idx, EN_TONEBURST_TYPE_1);
        }
        else if(pSATParam->eToneburstType == EN_TONEBURST_0)
        {
            pFETab->dishtab->SetTone(u8Idx, EN_TONEBURST_TYPE_0);
        }
        if(u8Polarity == 0)
            u8SubPort |= 0x02;
        if(bIsHiLOF)
            u8SubPort |= 0x01;
        switch(pSATParam->eSwt10Port)
        {
        case EN_SWT_PORT_1:
            u8SubPort = u8SubPort;
            break;
        case EN_SWT_PORT_2:
            u8SubPort = 0x04+ u8SubPort;
            break;
        case EN_SWT_PORT_3:
            u8SubPort = 0x08+ u8SubPort;
            break;
        case EN_SWT_PORT_4:
            u8SubPort = 0x0C+ u8SubPort;
            break;
        default:
            break;
        }
        if(pSATParam->eSwt10Port >= EN_SWT_PORT_1 && pSATParam->eSwt10Port <= EN_SWT_PORT_4)
        {
            _DiSEqC_SwitchPort(drv_frontend_index, u8SubPort,FALSE);
        }
        break;
    }
    case EN_DISEQC_LVL_11:
    {
        if(pSATParam->eSwt11Port >= EN_SWT_PORT_1 && pSATParam->eSwt11Port <= EN_SWT_PORT_16)
        {
            _DiSEqC_SwitchPort(drv_frontend_index, (MS_U8)(pSATParam->eSwt11Port)-1,TRUE);
        }
        break;
    }

    case EN_DISEQC_LVL_12:
    case EN_DISEQC_LVL_13:
    {
        if((pSATParam->u8Position&DISEQC12_USALS_POSITION_MASK) != (pSATParam->u8Position&DISEQC12_USALS_POSITION_MASK))
        {
            _DiSEqC_GotoSatPosition(drv_frontend_index, pSATParam->u8Position&DISEQC12_USALS_POSITION_MASK);
        }
        break;
    }

    case EN_DISEQC_LVL_MULTI:
    {
        _MultiDiSEqc_SwitchPort(drv_frontend_index, pSATParam,u8Polarity,bIsHiLOF);
    }
    }
    if(pSATParam->e22KOnOff == EN_22K_ON
            ||(pSATParam->e22KOnOff == EN_22K_AUTO && bIsHiLOF))
    {
        _DigiTuner_Satellite_Set22K(drv_frontend_index, TRUE);
    }
    return TRUE;
}

static  MS_BOOL  _DigiTuner_CalculateIF(MS_U8 drv_frontend_index,MS_SAT_PARAM *pSATParam,MS_FE_CARRIER_PARAM *pParam,DEMOD_MS_FE_CARRIER_PARAM* pdmdParam)
{
#if 0
    MS_S32 s32Freq;

    // calc Mid Freq & LNB control(22K on/off)
    if(pSATParam->u16LoLOF == pSATParam->u16HiLOF)
    {
        s32Freq  = pParam->u32Frequency;
        s32Freq -= pSATParam->u16LoLOF;
        if(s32Freq < 0) s32Freq = -s32Freq;
        pdmdParam->u32Frequency = s32Freq;
        bIsHiLOF = FALSE;
    }
    else // 2LOF
    {
        if(pSATParam->u16LoLOF <MAX_C_LOF_FREQ)  // c band
        {
            s32Freq  = pParam->u32Frequency;
            if(pSATParam->bPorInvert)
            {
                if(pParam->SatParam.u8Polarity == 1) //V ---> use low LOF
                {
                    bIsHiLOF = FALSE;
                    pdmdParam->u32Frequency = abs(pSATParam->u16LoLOF - s32Freq);
                }
                else   //H --->use high LOF
                {
                    bIsHiLOF = TRUE;
                    pdmdParam->u32Frequency =  abs(pSATParam->u16HiLOF - s32Freq) ;
                }
            }
            else
            {
                if(pParam->SatParam.u8Polarity == 0) //H ---> use low LOF
                {
                    bIsHiLOF = FALSE;
                    pdmdParam->u32Frequency = abs(pSATParam->u16LoLOF - s32Freq);
                }
                else   //V --->use high LOF
                {
                    bIsHiLOF = TRUE;
                    pdmdParam->u32Frequency =  abs(pSATParam->u16HiLOF - s32Freq) ;
                }
            }
        }
        else //Ku band
        {
            MS_U32 u32MidFreq;
            MS_S32 s32Offset = 0;
            s32Freq  = pParam->u32Frequency;
            u32MidFreq =  abs(s32Freq - pSATParam->u16LoLOF) ;
            s32Offset = pSATParam->u16LoLOF+2150 - pSATParam->u16HiLOF - 950;
            if(s32Offset < 0)
            {
                s32Offset = 0;
            }
            else
            {
                s32Offset /= 2;
            }
            if( u32MidFreq <= (2150-s32Offset))
            {
                bIsHiLOF = FALSE;
                pdmdParam->u32Frequency = u32MidFreq;
            }
            else
            {
                bIsHiLOF = TRUE;
                pdmdParam->u32Frequency =   abs(s32Freq - pSATParam->u16HiLOF) ;
            }
        }
    }
    if( pdmdParam->u32Frequency < 950 || pdmdParam->u32Frequency > 2150)
    {
        pdmdParam->u32Frequency = 945;
        bIsHiLOF = FALSE;
    }
    return TRUE;
#else
    MS_S32 s32Freq;
    TUNER_MS_SAT_PARAM stParam;
    MS_BOOL bret=TRUE;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    if(FALSE == _DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        return FALSE;

    pFETab = *(pFETabAddr + drv_frontend_index);

    s32Freq = pParam->u32Frequency;

     if(pSATParam->u16LoLOF <MAX_C_LOF_FREQ)  // c band
     {
         if(pSATParam->u16LoLOF == pSATParam->u16HiLOF)
         {
             bIsHiLOF = FALSE;
         }
         else if(pSATParam->bPorInvert)
         {
             if(pParam->SatParam.u8Polarity== 1) //V ---> use low LOF
             {
                 bIsHiLOF = FALSE;
             }
             else   //H --->use high LOF
             {
                 bIsHiLOF = TRUE;
             }
         }
         else
         {
             if(pParam->SatParam.u8Polarity  == 0) //H ---> use low LOF
             {
                 bIsHiLOF = FALSE;
             }
             else   //V --->use high LOF
             {
                 bIsHiLOF = TRUE;
             }
         }
    }
    else // ku band
    {
        stParam.u16HiLOF = pSATParam->u16HiLOF;
        stParam.u16LoLOF = pSATParam->u16LoLOF;
        stParam.u32RF_FREQ= pParam->u32Frequency;
        stParam.pbIsHiLOF = &bIsHiLOF;
        bret = pFETab->tunertab->Extension_Function(u8Idx,TUNER_EXT_FUNC_DECIDE_LNB_LO,&stParam);
    }

    if(bret)
    {
        if(bIsHiLOF)
            pdmdParam->u32Frequency = abs(s32Freq - pSATParam->u16HiLOF) ;
        else
            pdmdParam->u32Frequency = abs(s32Freq - pSATParam->u16LoLOF) ;
    }
    return bret;
#endif
}

#endif


//--------------------------------------------------------------------------------------------------
//  Global Functions
//--------------------------------------------------------------------------------------------------
#if DEMOD_SCAN_NUMBER > 0
extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_00TH_SCAN_DEVICE);
#endif
#if DEMOD_SCAN_NUMBER > 1
extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_01ST_SCAN_DEVICE);
#endif
#if DEMOD_SCAN_NUMBER > 2
extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_02ND_SCAN_DEVICE);
#endif
#if DEMOD_SCAN_NUMBER > 3
extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_03RD_SCAN_DEVICE);
#endif
#if DEMOD_SCAN_NUMBER > 4
extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_04TH_SCAN_DEVICE);
#endif
#if DEMOD_SCAN_NUMBER > 5
extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_05TH_SCAN_DEVICE);
#endif
#if DEMOD_SCAN_NUMBER > 6
extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_06TH_SCAN_DEVICE);
#endif
#if DEMOD_SCAN_NUMBER > 7
extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_07TH_SCAN_DEVICE);
#endif
#if DEMOD_SCAN_NUMBER > 8
extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_08TH_SCAN_DEVICE);
#endif
#if DEMOD_SCAN_NUMBER > 9
extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_09TH_SCAN_DEVICE);
#endif
#if DEMOD_SCAN_NUMBER > 10
extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_10TH_SCAN_DEVICE);
#endif
#if DEMOD_SCAN_NUMBER > 11
extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_11TH_SCAN_DEVICE);
#endif
#if DEMOD_SCAN_NUMBER > 12
extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_12TH_SCAN_DEVICE);
#endif
#if DEMOD_SCAN_NUMBER > 13
extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_13TH_SCAN_DEVICE);
#endif
#if DEMOD_SCAN_NUMBER > 14
extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_14TH_SCAN_DEVICE);
#endif

DRV_DEMOD_TABLE_TYPE* gp_demod_entry[DEMOD_SCAN_NUMBER]=
{
#if DEMOD_SCAN_NUMBER > 0
    &GET_DEMOD_ENTRY_NODE(DEMOD_00TH_SCAN_DEVICE)
#endif
#if DEMOD_SCAN_NUMBER > 1
    ,&GET_DEMOD_ENTRY_NODE(DEMOD_01ST_SCAN_DEVICE)
#endif
#if DEMOD_SCAN_NUMBER > 2
    ,&GET_DEMOD_ENTRY_NODE(DEMOD_02ND_SCAN_DEVICE)
#endif
#if DEMOD_SCAN_NUMBER > 3
    ,&GET_DEMOD_ENTRY_NODE(DEMOD_03RD_SCAN_DEVICE)
#endif
#if DEMOD_SCAN_NUMBER > 4
    ,&GET_DEMOD_ENTRY_NODE(DEMOD_04TH_SCAN_DEVICE)
#endif
#if DEMOD_SCAN_NUMBER > 5
    ,&GET_DEMOD_ENTRY_NODE(DEMOD_05TH_SCAN_DEVICE)
#endif
#if DEMOD_SCAN_NUMBER > 6
    ,&GET_DEMOD_ENTRY_NODE(DEMOD_06TH_SCAN_DEVICE)
#endif
#if DEMOD_SCAN_NUMBER > 7
    ,&GET_DEMOD_ENTRY_NODE(DEMOD_07TH_SCAN_DEVICE)
#endif
#if DEMOD_SCAN_NUMBER > 8
    ,&GET_DEMOD_ENTRY_NODE(DEMOD_08TH_SCAN_DEVICE)
#endif
#if DEMOD_SCAN_NUMBER > 9
    ,&GET_DEMOD_ENTRY_NODE(DEMOD_09TH_SCAN_DEVICE)
#endif
#if DEMOD_SCAN_NUMBER > 10
    ,&GET_DEMOD_ENTRY_NODE(DEMOD_10TH_SCAN_DEVICE)
#endif
#if DEMOD_SCAN_NUMBER > 11
    ,&GET_DEMOD_ENTRY_NODE(DEMOD_11TH_SCAN_DEVICE)
#endif
#if DEMOD_SCAN_NUMBER > 12
    ,&GET_DEMOD_ENTRY_NODE(DEMOD_12TH_SCAN_DEVICE)
#endif
#if DEMOD_SCAN_NUMBER > 13
    ,&GET_DEMOD_ENTRY_NODE(DEMOD_13TH_SCAN_DEVICE)
#endif
#if DEMOD_SCAN_NUMBER > 14
    ,&GET_DEMOD_ENTRY_NODE(DEMOD_14TH_SCAN_DEVICE)
#endif
};

#if TUNER_SCAN_NUMBER > 0
extern DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_00TH_SCAN_DEVICE);
#endif
#if TUNER_SCAN_NUMBER > 1
extern DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_01ST_SCAN_DEVICE);
#endif
#if TUNER_SCAN_NUMBER > 2
extern DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_02ND_SCAN_DEVICE);
#endif
#if TUNER_SCAN_NUMBER > 3
extern DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_03RD_SCAN_DEVICE);
#endif
#if TUNER_SCAN_NUMBER > 4
extern DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_04TH_SCAN_DEVICE);
#endif
#if TUNER_SCAN_NUMBER > 5
extern DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_05TH_SCAN_DEVICE);
#endif
#if TUNER_SCAN_NUMBER > 6
extern DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_06TH_SCAN_DEVICE);
#endif
#if TUNER_SCAN_NUMBER > 7
extern DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_07TH_SCAN_DEVICE);
#endif
#if TUNER_SCAN_NUMBER > 8
extern DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_08TH_SCAN_DEVICE);
#endif
#if TUNER_SCAN_NUMBER > 9
extern DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_09TH_SCAN_DEVICE);
#endif
#if TUNER_SCAN_NUMBER > 10
extern DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_10TH_SCAN_DEVICE);
#endif
#if TUNER_SCAN_NUMBER > 11
extern DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_11TH_SCAN_DEVICE);
#endif
#if TUNER_SCAN_NUMBER > 12
extern DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_12TH_SCAN_DEVICE);
#endif
#if TUNER_SCAN_NUMBER > 13
extern DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_13TH_SCAN_DEVICE);
#endif
#if TUNER_SCAN_NUMBER > 14
extern DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_14TH_SCAN_DEVICE);
#endif

DRV_TUNER_TABLE_TYPE* gp_tuner_entry[TUNER_SCAN_NUMBER] =
{
#if TUNER_SCAN_NUMBER > 0
    &GET_TUNER_ENTRY_NODE(TUNER_00TH_SCAN_DEVICE)
#endif
#if TUNER_SCAN_NUMBER > 1
    ,&GET_TUNER_ENTRY_NODE(TUNER_01ST_SCAN_DEVICE)
#endif
#if TUNER_SCAN_NUMBER > 2
    ,&GET_TUNER_ENTRY_NODE(TUNER_02ND_SCAN_DEVICE)
#endif
#if TUNER_SCAN_NUMBER > 3
    ,&GET_TUNER_ENTRY_NODE(TUNER_03RD_SCAN_DEVICE)
#endif
#if TUNER_SCAN_NUMBER > 4
    ,&GET_TUNER_ENTRY_NODE(TUNER_04TH_SCAN_DEVICE)
#endif
#if TUNER_SCAN_NUMBER > 5
    ,&GET_TUNER_ENTRY_NODE(TUNER_05TH_SCAN_DEVICE)
#endif
#if TUNER_SCAN_NUMBER > 6
    ,&GET_TUNER_ENTRY_NODE(TUNER_06TH_SCAN_DEVICE)
#endif
#if TUNER_SCAN_NUMBER > 7
    ,&GET_TUNER_ENTRY_NODE(TUNER_07TH_SCAN_DEVICE)
#endif
#if TUNER_SCAN_NUMBER > 8
    ,&GET_TUNER_ENTRY_NODE(TUNER_08TH_SCAN_DEVICE)
#endif
#if TUNER_SCAN_NUMBER > 9
    ,&GET_TUNER_ENTRY_NODE(TUNER_09TH_SCAN_DEVICE)
#endif
#if TUNER_SCAN_NUMBER > 10
    ,&GET_TUNER_ENTRY_NODE(TUNER_10TH_SCAN_DEVICE)
#endif
#if TUNER_SCAN_NUMBER > 11
    ,&GET_TUNER_ENTRY_NODE(TUNER_11TH_SCAN_DEVICE)
#endif
#if TUNER_SCAN_NUMBER > 12
    ,&GET_TUNER_ENTRY_NODE(TUNER_12TH_SCAN_DEVICE)
#endif
#if TUNER_SCAN_NUMBER > 13
    ,&GET_TUNER_ENTRY_NODE(TUNER_13TH_SCAN_DEVICE)
#endif
#if TUNER_SCAN_NUMBER > 14
    ,&GET_TUNER_ENTRY_NODE(TUNER_14TH_SCAN_DEVICE)
#endif
};
#if DEMOD_SCAN_NUMBER > 0

static char* MApi_XthScanDemodName(MS_U32 U32Index)
{
    switch(U32Index)
    {
    case DEMOD_MSB1236C:
        return "DEMOD_MSB1236C" ;
        break;
    case DEMOD_MSB1233C:
        return "DEMOD_MSB1233C" ;
        break;
    case DEMOD_MSB123X:
        return "DEMOD_MSB123X" ;
        break;
    case DEMOD_MSB1237:
        return "DEMOD_MSB1237" ;
        break;
    case DEMOD_MSB1238:
        return "DEMOD_MSB1238" ;
        break;
    case DEMOD_MSB131X:
        return "DEMOD_MSB131X" ;
        break;
    case DEMOD_MXL254:
        return "DEMOD_MXL254" ;
        break;
    case DEMOD_MSB124X:
        return "DEMOD_MSB124X" ;
        break;
    case DEMOD_MSB1245:
        return "DEMOD_MSB1245" ;
        break;
    default:
        FE_DBG(("Unknown demod name\n"));
        return "Unknown demod name" ;
    }
}
#endif
MS_U8 MApi_DigiTuner_GetMaxFEIdx(void)
{
    return _DigiTuner_MaxFEIdx();
}

void MApi_PrintDemodTypes(void)
{
    FE_MSG(("=========================== Demod Scan List ==========================\n"));

#if DEMOD_SCAN_NUMBER > 0
    FE_MSG((" index  0 scan device -> %s\n",MApi_XthScanDemodName(DEMOD_00TH_SCAN_DEVICE)));
#endif
#if DEMOD_SCAN_NUMBER > 1
    FE_MSG((" index  1 scan device -> %s\n",MApi_XthScanDemodName(DEMOD_01ST_SCAN_DEVICE)));
#endif
#if DEMOD_SCAN_NUMBER > 2
    FE_MSG((" index  2 scan device -> %s\n",MApi_XthScanDemodName(DEMOD_02ND_SCAN_DEVICE)));
#endif
#if DEMOD_SCAN_NUMBER > 3
    FE_MSG((" index  3 scan device -> %s\n",MApi_XthScanDemodName(DEMOD_03RD_SCAN_DEVICE)));
#endif
#if DEMOD_SCAN_NUMBER > 4
    FE_MSG((" index  4 scan device -> %s\n",MApi_XthScanDemodName(DEMOD_04TH_SCAN_DEVICE)));
#endif
#if DEMOD_SCAN_NUMBER > 5
    FE_MSG((" index  5 scan device -> %s\n",MApi_XthScanDemodName(DEMOD_05TH_SCAN_DEVICE)));
#endif
#if DEMOD_SCAN_NUMBER > 6
    FE_MSG((" index  6 scan device -> %s\n",MApi_XthScanDemodName(DEMOD_06TH_SCAN_DEVICE)));
#endif
#if DEMOD_SCAN_NUMBER > 7
    FE_MSG((" index  7 scan device -> %s\n",MApi_XthScanDemodName(DEMOD_07TH_SCAN_DEVICE)));
#endif
#if DEMOD_SCAN_NUMBER > 8
    FE_MSG((" index  8 scan device -> %s\n",MApi_XthScanDemodName(DEMOD_08TH_SCAN_DEVICE)));
#endif
#if DEMOD_SCAN_NUMBER > 9
    FE_MSG((" index  9 scan device -> %s\n",MApi_XthScanDemodName(DEMOD_09TH_SCAN_DEVICE)));
#endif
#if DEMOD_SCAN_NUMBER > 10
    FE_MSG((" index 10 scan device -> %s\n",MApi_XthScanDemodName(DEMOD_10TH_SCAN_DEVICE)));
#endif
#if DEMOD_SCAN_NUMBER > 11
    FE_MSG((" index 11 scan device -> %s\n",MApi_XthScanDemodName(DEMOD_11TH_SCAN_DEVICE)));
#endif
#if DEMOD_SCAN_NUMBER > 12
    FE_MSG((" index 12 scan device -> %s\n",MApi_XthScanDemodName(DEMOD_12TH_SCAN_DEVICE)));
#endif
#if DEMOD_SCAN_NUMBER > 13
    FE_MSG((" index 13 scan device -> %s\n",MApi_XthScanDemodName(DEMOD_13TH_SCAN_DEVICE)));
#endif
#if DEMOD_SCAN_NUMBER > 14
    FE_MSG((" index 14 scan device -> %s\n",MApi_XthScanDemodName(DEMOD_14TH_SCAN_DEVICE)));
#endif

    FE_MSG((" index %d scan device -> Internal Demod \n",DEMOD_SCAN_NUMBER));
    FE_MSG(("=========================== End Demod Scan List ======================\n"));
}

static char* MApi_XthScanTunerName(MS_U32 U32Index)
{
    switch(U32Index)
    {
    case TUNER_MXL603:
        return "TUNER_MXL603" ;
        break;
    case TUNER_MXL608:
        return "TUNER_MXL608" ;
        break;
    case TUNER_TDA18250A:
        return "TUNER_TDA18250A" ;
        break;
    case TUNER_TDA18250B:
        return "TUNER_TDA18250B" ;
        break;
    case TUNER_TDA18260:
        return "TUNER_TDA18260" ;
        break;
    case TUNER_PHILIPS_TDA18250HN:
        return "TUNER_PHILIPS_TDA18250HN" ;
        break;
    case TUNER_AV2011:
        return "TUNER_AV2011" ;
        break;
    case TUNER_AV2012:
        return "TUNER_AV2012" ;
        break;
    case TUNER_AV2018:
        return "TUNER_AV2018" ;
        break;
    case TUNER_AV2028:
        return "TUNER_AV2028" ;
        break;
    case TUNER_MXL254:
        return "TUNER_MXL254";
        break;
    case TUNER_RDA5815M:
        return "TUNER_RDA5815M" ;
        break;
    case TUNER_R848:
        return "TUNER_R848" ;
        break;
    case TUNER_R836:
        return "TUNER_R836" ;
        break;
    case TUNER_RT710:
        return "TUNER_RT710" ;
        break;
    default:
        return "Unknown demod name" ;
        break;
    }
}

void MApi_PrintTunerTypes(void)
{
    FE_MSG(("=========================== Tuner Scan List ==========================\n"));

#if TUNER_SCAN_NUMBER > 0
    FE_MSG((" index  0 scan device -> %s\n",MApi_XthScanTunerName(TUNER_00TH_SCAN_DEVICE)));
#endif
#if TUNER_SCAN_NUMBER > 1
    FE_MSG((" index  1 scan device -> %s\n",MApi_XthScanTunerName(TUNER_01ST_SCAN_DEVICE)));
#endif
#if TUNER_SCAN_NUMBER > 2
    FE_MSG((" index  2 scan device -> %s\n",MApi_XthScanTunerName(TUNER_02ND_SCAN_DEVICE)));
#endif
#if TUNER_SCAN_NUMBER > 3
    FE_MSG((" index  3 scan device -> %s\n",MApi_XthScanTunerName(TUNER_03RD_SCAN_DEVICE)));
#endif
#if TUNER_SCAN_NUMBER > 4
    FE_MSG((" index  4 scan device -> %s\n",MApi_XthScanTunerName(TUNER_04TH_SCAN_DEVICE)));
#endif
#if TUNER_SCAN_NUMBER > 5
    FE_MSG((" index  5 scan device -> %s\n",MApi_XthScanTunerName(TUNER_05TH_SCAN_DEVICE)));
#endif
#if TUNER_SCAN_NUMBER > 6
    FE_MSG((" index  6 scan device -> %s\n",MApi_XthScanTunerName(TUNER_06TH_SCAN_DEVICE)));
#endif
#if TUNER_SCAN_NUMBER > 7
    FE_MSG((" index  7 scan device -> %s\n",MApi_XthScanTunerName(TUNER_07TH_SCAN_DEVICE)));
#endif
#if TUNER_SCAN_NUMBER > 8
    FE_MSG((" index  8 scan device -> %s\n",MApi_XthScanTunerName(TUNER_08TH_SCAN_DEVICE)));
#endif
#if TUNER_SCAN_NUMBER > 9
    FE_MSG((" index  9 scan device -> %s\n",MApi_XthScanTunerName(TUNER_09TH_SCAN_DEVICE)));
#endif
#if TUNER_SCAN_NUMBER > 10
    FE_MSG((" index 10 scan device -> %s\n",MApi_XthScanTunerName(TUNER_10TH_SCAN_DEVICE)));
#endif
#if TUNER_SCAN_NUMBER > 11
    FE_MSG((" index 11 scan device -> %s\n",MApi_XthScanTunerName(TUNER_11TH_SCAN_DEVICE)));
#endif
#if TUNER_SCAN_NUMBER > 12
    FE_MSG((" index 12 scan device -> %s\n",MApi_XthScanTunerName(TUNER_12TH_SCAN_DEVICE)));
#endif
#if TUNER_SCAN_NUMBER > 13
    FE_MSG((" index 13 scan device -> %s\n",MApi_XthScanTunerName(TUNER_13TH_SCAN_DEVICE)));
#endif
#if TUNER_SCAN_NUMBER > 14
    FE_MSG((" index 14 scan device -> %s\n",MApi_XthScanTunerName(TUNER_14TH_SCAN_DEVICE)));
#endif

    FE_MSG(("=========================== End Tuner Scan List ======================\n"));
}

DRV_DEMOD_TABLE_TYPE* MApi_get_InternalDemodtab_entry(MS_U8 drv_frontend_index)
{
    DEV_FRONTEND_TYPE* pFETab;
    MS_U32 dvb_type = BROADCAST_TYPE_NOT_SEETING;

    if(pFETabAddr != NULL)
    {
        pFETab = *(pFETabAddr + drv_frontend_index);
        if (pFETab !=NULL)
        {
            dvb_type = pFETab->dvb_type;
        }
    }
    //else
    //{
    //    if(!FE_Detect_Done)
    //         dvb_type = DefaultBroadcastType;
    //}

#if  defined(CHIP_KRONUS)
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC);
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBT);
    if(!FE_Detect_Done)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC);
#elif defined(CHIP_KAISERIN)
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC);
    //extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKAISERIN_DVBT);
    if(!FE_Detect_Done)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC);
#elif defined(CHIP_KAISER)
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKAISER_DVBC);
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSATSC_C);
    if(!FE_Detect_Done)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSKAISER_DVBC);
#elif defined(CHIP_KAPPA)
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC);
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBT);
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_ISDBT);
    if(!FE_Detect_Done)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC);
#elif defined(CHIP_KRITI)
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKRITI_DVBT2);
    if(!FE_Detect_Done)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSKRITI_DVBT2);
#elif defined(CHIP_KELTIC)
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC);
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKELTIC_ATSC);
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKELTIC_DVBS);
    if(!FE_Detect_Done)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC);
#elif defined(CHIP_KENYA)
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKENYA_DVBC);
    if(!FE_Detect_Done)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSKENYA_DVBC);
#elif defined(CHIP_KERES) || defined(CHIP_KIRIN)
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC);
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKERES_ATSC);
    if(!FE_Detect_Done)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC);
#elif defined(CHIP_KRIS)
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKRIS_DVBS);
    if(!FE_Detect_Done)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSKRIS_DVBS);
#elif defined(CHIP_KRATOS)
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSKRATOS_DVBS);
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC);
    if(!FE_Detect_Done)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC);
#elif defined(CHIP_KIWI)
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_ISDBT);
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC);
    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBT);
    if(!FE_Detect_Done)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC);
#endif

    extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_NULL);

    switch(dvb_type)
    {
    case DVBC:
    {
#if  defined(CHIP_KRONUS) || defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_KRATOS) || defined(CHIP_KIWI) ||\
     defined(CHIP_KAISERIN) || defined(CHIP_KAPPA) || defined(CHIP_KELTIC)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBC);
#elif defined(CHIP_KAISER)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSKAISER_DVBC);
#elif defined(CHIP_KENYA)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSKENYA_DVBC);
#endif
        break;
    }

    case DVBT:
    case DVBT2:
    {
#if  defined(CHIP_KRONUS) || defined(CHIP_KAPPA) || defined(CHIP_KIWI)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_DVBT);
#elif defined(CHIP_KRITI)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSKRITI_DVBT2);
#endif
        break;
    }

    case ISDBT:
    {
#if  defined(CHIP_KAPPA) || defined(CHIP_KIWI)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_ISDBT);
#endif
        break;
    }

    case ATSC:
    {
        break;
    }

    case J83B:
    {
#if defined(CHIP_KELTIC)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSKELTIC_ATSC);   // Keltic with ATSC_C (J83B) internal only
#elif defined(CHIP_KAISER)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSATSC_C);
#elif defined(CHIP_KERES) || defined(CHIP_KIRIN)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSKERES_ATSC);
#endif
        break;
    }
    case DVBS:
    case DVBS2:
    {
#if defined(CHIP_KELTIC)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSKELTIC_DVBS);
#elif defined(CHIP_KRIS)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSKRIS_DVBS);
#elif defined(CHIP_KRATOS)
        return & GET_DEMOD_ENTRY_NODE(DEMOD_MSKRATOS_DVBS);
#endif
        break;
    }

    default  :
    {
        FE_DBG(("no corresponding internal demod\n"));
        FE_DBG(("(broadcast type = 0x%"DTC_MS_U32_x")\n", dvb_type));
        return & GET_DEMOD_ENTRY_NODE(DEMOD_NULL);
    }
    }
    return & GET_DEMOD_ENTRY_NODE(DEMOD_NULL);
}

DRV_DEMOD_TABLE_TYPE* MApi_get_Demodtab_entry(MS_U8 IIC_index, DEV_FRONTEND_TYPE* pFEtab)
{
    MS_U32 i = 0;
    HWI2C_PORT hwi2c_port;
    MS_U8 u8Idx, u8SlaveID;

    hwi2c_port = getI2CPort(IIC_index);
    if (hwi2c_port < E_HWI2C_PORT_1)
        u8Idx = 0;
    else if (hwi2c_port < E_HWI2C_PORT_2)
        u8Idx = 1;
    else if (hwi2c_port < E_HWI2C_PORT_3)
        u8Idx = 2;
    else
    {
        pFEtab->eInExTernalPath = FE_INTERNAL;
        return  MApi_get_InternalDemodtab_entry(IIC_index);
    }

    while(i < DEMOD_SCAN_NUMBER)
    {
        if((gp_demod_entry[i])->CheckExist(IIC_index, &u8SlaveID))
        {
            if(!_DigiTuner_Conflit_Check(u8Idx, (gp_demod_entry[i])->data, u8SlaveID)) // FALSE: no conflit condition
            {
                pFEtab->eInExTernalPath = FE_EXTERNAL;
                return gp_demod_entry[i];
            }
        }

        i++;
    }

    //MApi_Frontend_SetInExteralPath(IIC_index,FE_INTERNAL, pFEtab);
    pFEtab->eInExTernalPath = FE_INTERNAL;
    return  MApi_get_InternalDemodtab_entry(IIC_index);
}

DRV_TUNER_TABLE_TYPE* MApi_get_Tunertab_entry(MS_U8 IIC_index, MS_BOOL* bExist, MS_U32* pu32Ch_Cnt, DEV_FRONTEND_TYPE* pFEtab)
{
    extern DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_NULL);
    MS_U32 i = 0;
    MS_U32 tmp;

    tmp = *pu32Ch_Cnt;
    *bExist = TRUE;

    while(i < TUNER_SCAN_NUMBER)
    {
        // Patch for MXL254
        // Note: TUNER_MXL254 tuner and DEMOD_MXL254 demod MSUT be a pair
        if(gp_tuner_entry[i]->data == TUNER_MXL254)
        {
            if(pFEtab->demodtab->data == DEMOD_MXL254)
            {
                //Do tuner check exist just for some initial setting
                (gp_tuner_entry[i])->CheckExist(IIC_index, pu32Ch_Cnt);
                return gp_tuner_entry[i];
            }
            else
            {
                i++;
                continue;
            }
        }//End of patch for MXL254

        // Patch for MXL542
        // Note: TUNER_MXL254 tuner and DEMOD_MXL254 demod MSUT be a pair
        if(gp_tuner_entry[i]->data == TUNER_MXL542)
        {
            if(pFEtab->demodtab->data == DEMOD_MXL542)
            {
                //Do tuner check exist just for some initial setting
                (gp_tuner_entry[i])->CheckExist(IIC_index, pu32Ch_Cnt);
                return gp_tuner_entry[i];
            }
            else
            {
                i++;
                continue;
            }
        }//End of patch for MXL542


        // Patch for MSR1742
        // Note: TUNER_MSR1742 tuner and DEMOD_MSR1742 demod MSUT be a pair
        if(gp_tuner_entry[i]->data == TUNER_MSR1742)
        {
            if(pFEtab->demodtab->data == DEMOD_MSR1742)
            {
                //Do tuner check exist just for some initial setting
                (gp_tuner_entry[i])->CheckExist(IIC_index, pu32Ch_Cnt);
                return gp_tuner_entry[i];
            }
            else
            {
                i++;
                continue;
            }
        }//End of patch for MSR1742

        pFEtab->demodtab->Extension_Function(IIC_index, DEMOD_EXT_FUNC_FINALIZE, NULL);
        if(pFEtab->demodtab->Extension_FunctionPreSetting != NULL)
            pFEtab->demodtab->Extension_FunctionPreSetting(IIC_index, DEMOD_EXT_FUNC_FINALIZE, NULL);
        MApi_Demod_HWReset(IIC_index);
        if(pFEtab->eInExTernalPath == FE_EXTERNAL)
        {
            if( (gp_tuner_entry[i])->CheckExist(IIC_index, pu32Ch_Cnt) )
            {
                *pu32Ch_Cnt = tmp;
            }
            else
            {
                pFEtab->demodtab->I2CByPass(IIC_index,TRUE);
                if( (gp_tuner_entry[i])->CheckExist(IIC_index, pu32Ch_Cnt))
                {
                    pFEtab->demodtab->I2CByPass(IIC_index,FALSE);
                    return gp_tuner_entry[i];
                }
                pFEtab->demodtab->I2CByPass(IIC_index,FALSE);
            }
        }
        else
        {
            pFEtab->demodtab->I2CByPass(IIC_index,TRUE);
            if( (gp_tuner_entry[i])->CheckExist(IIC_index, pu32Ch_Cnt))
            {
                pFEtab->demodtab->I2CByPass(IIC_index,FALSE);
                return gp_tuner_entry[i];
            }
            pFEtab->demodtab->I2CByPass(IIC_index,FALSE);
        }
        i++;
    }
    //pFEtab->demodtab->Extension_Function(IIC_index,DEMOD_EXT_FUNC_IIC_BYPASS_OFF,NULL);
    FE_ERR(("Can not find tuner devices\n"));
    *bExist = FALSE;
    return  & GET_TUNER_ENTRY_NODE(TUNER_NULL);
}



#if MS_DVBS_INUSE
static struct drv_dishtab_entry *MApi_get_Dishtab_entry(MS_U8 IIC_index)
{
    struct drv_dishtab_entry *dishtab_entry[MAX_FRONTEND_NUM] = {0};

#if (DISH_TYPE == DISH_A8293) || \
    (defined(DISH_TYPE1) && (DISH_TYPE1 == DISH_A8293)) || \
    (defined(DISH_TYPE2) && (DISH_TYPE2 == DISH_A8293)) || \
    (defined(DISH_TYPE3) && (DISH_TYPE3 == DISH_A8293))
    extern struct drv_dishtab_entry dish_entry_DISH_A8293;
#endif

#if (DISH_TYPE == DISH_A8296) || \
    (defined(DISH_TYPE1) && (DISH_TYPE1 == DISH_A8296)) || \
    (defined(DISH_TYPE2) && (DISH_TYPE2 == DISH_A8296)) || \
    (defined(DISH_TYPE3) && (DISH_TYPE3 == DISH_A8296))
    extern struct drv_dishtab_entry dish_entry_DISH_A8296;
#endif

#if (DISH_TYPE == DISH_A8304) || \
    (defined(DISH_TYPE1) && (DISH_TYPE1 == DISH_A8304)) || \
    (defined(DISH_TYPE2) && (DISH_TYPE2 == DISH_A8304)) || \
    (defined(DISH_TYPE3) && (DISH_TYPE3 == DISH_A8304))
    extern struct drv_dishtab_entry dish_entry_DISH_A8304;
#endif

#if (DISH_TYPE == DISH_A8297) || \
    (defined(DISH_TYPE1) && (DISH_TYPE1 == DISH_A8297)) || \
    (defined(DISH_TYPE2) && (DISH_TYPE2 == DISH_A8297)) || \
    (defined(DISH_TYPE3) && (DISH_TYPE3 == DISH_A8297))
    extern struct drv_dishtab_entry dish_entry_DISH_A8297;
#endif


#if(DISH_TYPE == DISH_A8293)
    dishtab_entry[0] = &dish_entry_DISH_A8293;
#elif(DISH_TYPE == DISH_A8296)
    dishtab_entry[0] = &dish_entry_DISH_A8296;
#elif(DISH_TYPE == DISH_A8304)
    dishtab_entry[0] = &dish_entry_DISH_A8304;
#elif(DISH_TYPE == DISH_A8297)
    dishtab_entry[0] = &dish_entry_DISH_A8297;
#endif

#if(defined(DISH_TYPE1) && (DISH_TYPE1 == DISH_A8293) && (MAX_FRONTEND_NUM > 1))
    dishtab_entry[1] = &dish_entry_DISH_A8293;
#elif(defined(DISH_TYPE1) && (DISH_TYPE1 == DISH_A8296) && (MAX_FRONTEND_NUM > 1))
    dishtab_entry[1] = &dish_entry_DISH_A8296;
#elif(defined(DISH_TYPE1) && (DISH_TYPE1 == DISH_A8304) && (MAX_FRONTEND_NUM > 1))
    dishtab_entry[1] = &dish_entry_DISH_A8304;
#elif(defined(DISH_TYPE1) && (DISH_TYPE1 == DISH_A8297) && (MAX_FRONTEND_NUM > 1))
    dishtab_entry[1] = &dish_entry_DISH_A8297;
#else
    FE_DBG(("LNB control IC on I2C%x not set\n", IIC_index));
#endif

#if(defined(DISH_TYPE2) && (DISH_TYPE2 == DISH_A8293) && (MAX_FRONTEND_NUM > 2))
    dishtab_entry[2] = &dish_entry_DISH_A8293;
#elif(defined(DISH_TYPE2) && (DISH_TYPE2 == DISH_A8296) && (MAX_FRONTEND_NUM > 2))
    dishtab_entry[2] = &dish_entry_DISH_A8296;
#elif(defined(DISH_TYPE2) && (DISH_TYPE2 == DISH_A8304) && (MAX_FRONTEND_NUM > 2))
    dishtab_entry[2] = &dish_entry_DISH_A8304;
#elif(defined(DISH_TYPE2) && (DISH_TYPE2 == DISH_A8297) && (MAX_FRONTEND_NUM > 2))
    dishtab_entry[2] = &dish_entry_DISH_A8297;
#else
    FE_DBG(("LNB control IC on I2C%x not set\n", IIC_index));
#endif


#if(defined(DISH_TYPE3) && (DISH_TYPE3 == DISH_A8293) && (MAX_FRONTEND_NUM > 3))
    dishtab_entry[3] = &dish_entry_DISH_A8293;
#elif(defined(DISH_TYPE3) && (DISH_TYPE3 == DISH_A8296) && (MAX_FRONTEND_NUM > 3))
    dishtab_entry[3] = &dish_entry_DISH_A8296;
#elif(defined(DISH_TYPE3) && (DISH_TYPE3 == DISH_A8304) && (MAX_FRONTEND_NUM > 3))
    dishtab_entry[3] = &dish_entry_DISH_A8304;
#elif(defined(DISH_TYPE2) && (DISH_TYPE2 == DISH_A8297) && (MAX_FRONTEND_NUM > 3))
    dishtab_entry[2] = &dish_entry_DISH_A8297;
#else
    FE_DBG(("LNB control IC on I2C%x not set\n", IIC_index));
#endif

    return dishtab_entry[IIC_index];
}
#endif

static MS_U32 MApi_get_dmx_input_path(MS_U8 drv_frontend_index)
{
    MS_U32 u32dmx_input_path = 0;

    switch(drv_frontend_index)
    {
    case 0:
#ifdef DMX_EXTERNAL_PATH_0
        u32dmx_input_path = DMX_EXTERNAL_PATH_0;
#elif defined(DMX_INPUT0)
        u32dmx_input_path = DMX_INPUT0;
#else
        u32dmx_input_path = DMX_INPUT_EXT_INPUT0;
#endif
        break;

    case 1:
#ifdef DMX_EXTERNAL_PATH_1
        u32dmx_input_path = DMX_EXTERNAL_PATH_1;
#elif defined(DMX_INPUT1)
        u32dmx_input_path = DMX_INPUT1;
#else
        u32dmx_input_path = DMX_INPUT_EXT_INPUT1;
#endif

        break;

    case 2:
#ifdef DMX_EXTERNAL_PATH_2
        u32dmx_input_path = DMX_EXTERNAL_PATH_2;
#elif defined(DMX_INPUT2)
        u32dmx_input_path = DMX_INPUT2;
#else
        u32dmx_input_path = DMX_INPUT_EXT_INPUT2;
#endif

        break;

    case 3:
#ifdef DMX_EXTERNAL_PATH_3
        u32dmx_input_path = DMX_EXTERNAL_PATH_3;
#elif defined(DMX_INPUT3)
        u32dmx_input_path = DMX_INPUT3;
#else
        u32dmx_input_path = DMX_INPUT_EXT_INPUT3;
#endif

        break;

    default:
#ifdef DMX_EXTERNAL_PATH_0
        u32dmx_input_path = DMX_EXTERNAL_PATH_0;
#elif defined(DMX_INPUT0)
        u32dmx_input_path = DMX_INPUT0;
#else
        u32dmx_input_path = DMX_INPUT_EXT_INPUT0;
#endif

        break;
    }

    return u32dmx_input_path;
}

/*
static MS_U32 MApi_get_dvb_type(MS_U8 drv_frontend_index)
{
    MS_U32 u32dvb_type = 0;

    switch(drv_frontend_index)
    {
        case 0:
            u32dvb_type = MS_DVB_TYPE_SEL;
        break;

        case 1:
            u32dvb_type = MS_DVB_TYPE_SEL1;
        break;

        case 2:
            u32dvb_type = MS_DVB_TYPE_SEL2;
        break;

        case 3:
            u32dvb_type = MS_DVB_TYPE_SEL3;
        break;

        default:
            u32dvb_type = MS_DVB_TYPE_SEL;
        break;

    }

    return u32dvb_type;
}
*/

//-------------------------------------------------------------------------------------------------
/// Initialize digital tuner
/// @param eFE                      \b IN: Frontend id
/// @return TRUE:  Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------

static MS_BOOL MApi_Demod_lookup(MS_U8 IIC_index, DEV_FRONTEND_TYPE* pFEtab)
{

    if(NULL == pFEtab)
        return FALSE;

    pFEtab->demodtab = MApi_get_Demodtab_entry(IIC_index, pFEtab);
    FE_DBG(("[FE] Scanned demod device = %s\n",pFEtab->demodtab->name));
    return TRUE;
}

static MS_BOOL MApi_Tuner_lookup(MS_U8 IIC_index, DEV_FRONTEND_TYPE* pFEtab, MS_U32* pu32Ch_Cnt)
{
    MS_BOOL bRet;

    if(NULL == pFEtab)
        return FALSE;

    pFEtab->tunertab = MApi_get_Tunertab_entry(IIC_index,&bRet, pu32Ch_Cnt, pFEtab);
    FE_DBG(("[FE] Scanned tuner device = %s\n",pFEtab->tunertab->name));
    return bRet;
}

MS_U32 MApi_DigiTuner_GetFrontendType(MS_U8 drv_frontend_index)
{
    DEV_FRONTEND_TYPE* pFETab;

    if(FALSE == _DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        return DefaultBroadcastType;

    pFETab = *(pFETabAddr + drv_frontend_index);
    return pFETab->dvb_type;
}

MS_BOOL MApi_DigiTuner_ReInit(MS_U8 drv_frontend_index)
{

    if(NULL == _pu8Init || (FALSE == _DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        return FALSE;

    if(*(_pu8Init + drv_frontend_index) == 0)
    {
        FE_ERR(("Not Init yet, call MApi_DigiTuner_Init\n"));
        return FALSE;
    }

    if(MApi_DigiTuner_DeInit(drv_frontend_index)==FALSE)
        return FALSE;

    return MApi_DigiTuner_Init(drv_frontend_index);
}

MS_BOOL MApi_DigiTuner_DeInit(MS_U8 drv_frontend_index)
{
    DEV_FRONTEND_TYPE* pFETab;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);

    if((NULL == _pu8Init) || (FALSE == _DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        return FALSE;

    pFETab = *(pFETabAddr + drv_frontend_index);
    //if(*(_pu8Init + drv_frontend_index) == 0)
    //    return TRUE;

    //if(_s32MutexId!=-1)
    //{
    //    MsOS_DeleteMutex(_s32MutexId);
    //    _s32MutexId=-1;
    //}

    if (pFETab->demodtab->Extension_Function(u8Idx, DEMOD_EXT_FUNC_FINALIZE, NULL) == FALSE)
        return FALSE;

    if (pFETab->tunertab->Extension_Function(u8Idx, TUNER_EXT_FUNC_FINALIZE, NULL) == FALSE)
        return FALSE;

    *(_pu8Init + drv_frontend_index) = 0;
    return TRUE;

}


MS_BOOL MApi_Frontend_SetTunerDemod(MS_U8 IIc_index,MS_U32 u32ScanTunerIndex, MS_U32 u32ScanDemodIndex)
{
    DEV_FRONTEND_TYPE FETab;
    MS_U8 u8FEIdx = 0,i;
    MS_U32 u32FEIdx_Cnt[MAX_FRONTEND_NUM], u32FEIdx_Cnt_Cur;
    // signed char shift = 0, FEIdxMax = 0;
    DEV_FRONTEND_TYPE* u32NewpFETabAddr[16];
    DEV_FRONTEND_TYPE* pFETab = NULL;
    MS_U8 u8SlaveID;
    HWI2C_PORT hwi2c_port;


    if(INVALID_POOL_ID == gs32FECachedPoolID)
    {
        FE_ERR(("INVALID_POOL_ID !!!\n"));
        return FALSE;
    }

    if(eDetect_Mode != USER_DEFINE)
        return FALSE;

    if(!FE_Load_Reset_Pin)
    {
        MApi_Frontend_LoadResetPin();
        FE_Load_Reset_Pin = TRUE;
    }

    if(IIc_index >= MAX_FRONTEND_NUM)
    {
        FE_ERR(("INVALID_IIC_INDEX !!!\n"));
        return FALSE;
    }

    if((FE_Detect_Done) && (eDetect_Mode == USER_DEFINE))
    {
        FE_MSG(("Alread dectect before , not need to check demod/tuner again\n"));
        return TRUE;
    }

    FE_DBG(("### %x %x %x\n", (MS_U8)IIc_index, (MS_U8)u32ScanTunerIndex, (MS_U8)u32ScanDemodIndex));
    hwi2c_port = getI2CPort(IIc_index);
    MDrv_IIC_Init(hwi2c_port);
    // get current FE index of each I2C port
    for(i=0; i< MAX_FRONTEND_NUM; i++)
    {
        if(!i)
            u32FEIdx_Cnt[i] = u32ch_acc[i];
        else
            u32FEIdx_Cnt[i] = u32ch_acc[i] - u32ch_acc[i-1];
    }

    // First Set, create max possible RF channel count: 16
    if(NULL == pFETabAddr)
    {
        pFETabAddr = (DEV_FRONTEND_TYPE**)MsOS_AllocateMemory(sizeof(MS_U32)*16, gs32FECachedPoolID);
        if(NULL == pFETabAddr)
            return FALSE;
        memset(pFETabAddr, 0x00, sizeof(MS_U32)*16);
    }

    if(NULL ==  _pu8Init)
    {
        _pu8Init = (MS_U8*)MsOS_AllocateMemory(sizeof(MS_U8)*16,gs32FECachedPoolID);
        if(NULL == _pu8Init)
            return FALSE;

        for(i=0; i<16; i++)
            *(_pu8Init + i) = 0;
    }

    if(NULL == _ptunerParam)
    {
        _ptunerParam = (MS_FE_CARRIER_PARAM*)MsOS_AllocateMemory(sizeof(MS_FE_CARRIER_PARAM)*16,gs32FECachedPoolID);
        if(NULL == _ptunerParam)
            return FALSE;

        for(i=0; i<16; i++)
            memset(_ptunerParam + i, 0x00, sizeof(MS_FE_CARRIER_PARAM));
    }

#if MS_DVBS_INUSE
    if(NULL == _pstSATParam)
    {
        _pstSATParam = (MS_SAT_PARAM*)MsOS_AllocateMemory(sizeof(MS_SAT_PARAM)*16,gs32FECachedPoolID);
        if(NULL == _pstSATParam)
            return FALSE;
    }
#endif

    // set tuner
    if (u32ScanTunerIndex >= TUNER_SCAN_NUMBER)
    {
        FE_ERR(("Scan tuner index exceeds entry max number\n"));
        return FALSE;
    }

    FETab.tunertab = gp_tuner_entry[u32ScanTunerIndex];


    //set demod
    if (u32ScanDemodIndex > DEMOD_SCAN_NUMBER)
    {
        FE_ERR(("Scan demod index exceeds entry max number\n"));
        return FALSE;
    }

    if ( u32ScanDemodIndex == DEMOD_SCAN_NUMBER )
    {
        extern DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_NULL);
        MApi_Frontend_SetInExteralPath(IIc_index,FE_INTERNAL, &FETab);
        FETab.demodtab = MApi_get_InternalDemodtab_entry(IIc_index);
        if ( FETab.demodtab == &GET_DEMOD_ENTRY_NODE(DEMOD_NULL) )
        {
            FE_ERR(("NO Internal DEMOD\n"));
            return FALSE;
        }
    }
    else
    {
        MApi_Frontend_SetInExteralPath(IIc_index,FE_EXTERNAL, &FETab);
        FETab.demodtab = gp_demod_entry[u32ScanDemodIndex];
    }

    MApi_Demod_HWReset(IIc_index);
    // Do check exist for some setting or parameters set during check exist flow
    // External demod only
    if(u32ScanDemodIndex < DEMOD_SCAN_NUMBER)
    {
        if(!FETab.demodtab->CheckExist(IIc_index, &u8SlaveID))
        {
            FE_ERR(("WARNING!!! DEMOD check exist FAIL\n"));
        }
    }

    // Check if demod/tuner is matchable
    if(((FETab.tunertab->data == TUNER_MXL254) && (FETab.demodtab->data != TUNER_MXL254)) || \
            ((FETab.tunertab->data != TUNER_MXL254) && (FETab.demodtab->data == TUNER_MXL254)))
    {
        return FALSE;
    }


    // Do check exist for getting channel number
    u32FEIdx_Cnt_Cur = 0;
    FETab.demodtab->Extension_Function(IIc_index, DEMOD_EXT_FUNC_FINALIZE, NULL);
    if(FETab.demodtab->Extension_FunctionPreSetting != NULL)
        FETab.demodtab->Extension_FunctionPreSetting(IIc_index, DEMOD_EXT_FUNC_FINALIZE, NULL);
    MApi_Demod_HWReset(IIc_index);
    if(FETab.eInExTernalPath == FE_EXTERNAL)
    {
        if(FETab.tunertab->CheckExist(IIc_index, &u32FEIdx_Cnt_Cur) )
        {
        }
        else
        {
            FETab.demodtab->I2CByPass(IIc_index,TRUE);
            if( FETab.tunertab->CheckExist(IIc_index, &u32FEIdx_Cnt_Cur))
            {
                FE_ERR(("WARNING!!! TUNER check exist FAIL\n"));
                FETab.demodtab->I2CByPass(IIc_index,FALSE);
            }
            FETab.demodtab->I2CByPass(IIc_index,FALSE);
        }
    }
    else
    {
        FETab.demodtab->I2CByPass(IIc_index,TRUE);
        if(FETab.tunertab->CheckExist(IIc_index, &u32FEIdx_Cnt_Cur))
        {
            FE_ERR(("WARNING!!! TUNER check exist FAIL\n"));
            FETab.demodtab->I2CByPass(IIc_index,FALSE);
        }
        FETab.demodtab->I2CByPass(IIc_index,FALSE);
    }

    //if(FETab.tunertab->data == TUNER_MXL254)
    //   u32FEIdx_Cnt_Cur = 4;
    //else
    //    u32FEIdx_Cnt_Cur = 1;

    FETab.dvb_type = DefaultBroadcastType;
    FETab.next = NULL;

    if(IIc_index == 0)
        u8FEIdx = 0;
    else
        u8FEIdx = u32ch_acc[IIc_index-1];

    MApi_Demod_HWReset(IIc_index);
    for(i=u8FEIdx; i<(u8FEIdx + u32FEIdx_Cnt_Cur); i++)
    {
        //Note : assume that ths assigned tuner/demod is new device, not to replace an old one
        pFETab = (DEV_FRONTEND_TYPE*)MsOS_AllocateMemory(sizeof(DEV_FRONTEND_TYPE),gs32FECachedPoolID);
        memcpy(pFETab, &FETab, sizeof(DEV_FRONTEND_TYPE));
        u32NewpFETabAddr[i] = pFETab;
        pFETab->demodtab->Extension_Function(i, DEMOD_EXT_FUNC_FINALIZE, NULL);
    }



    u32FEIdx_Cnt[IIc_index] = u32FEIdx_Cnt_Cur;
    for(i=0; i<MAX_FRONTEND_NUM; i++)
    {
        if(!i)
            u32ch_acc[i] = u32FEIdx_Cnt[i];
        else
            u32ch_acc[i] = u32FEIdx_Cnt[i] + u32FEIdx_Cnt[i-1];
    }

    //FE_DBG(("_DigiTuner_MaxFEIdx() = %x\n", _DigiTuner_MaxFEIdx());

    if(_DigiTuner_MaxFEIdx() != FEIndex_NONE)
    {
        for(i=u8FEIdx; i<= _DigiTuner_MaxFEIdx(); i++)
        {
            *(pFETabAddr + i) = u32NewpFETabAddr[i];
            FE_DBG((" *(pFETabAddr + %d) = %"DTC_MS_U32_x", u32NewpFETabAddr[%d] = %"DTC_MS_U32_x"\n", i, (MS_U32)(*(pFETabAddr + i)), i, (MS_U32)u32NewpFETabAddr[i]));
        }
        FE_DBG(("[FE] IIC Port %x Set tuner = %s Set demod = %s OK!\n", IIc_index,FETab.tunertab->name, FETab.demodtab->name));
    }

    return TRUE;
}

MS_BOOL MApi_Frontend_SetDetectMode(MS_U8 drv_frontend_index,EN_FRONTEND_DETECT_MODE eDetectMode)
{

    FE_MSG(("1:AUTO_SCAN\n"));
    FE_MSG(("2:USER_DEFINE\n"));

    if( eDetectMode > USER_DEFINE)
    {
        FE_ERR(("Entered detection mode=%d is out of range\n",eDetectMode));
        return FALSE;
    }

    if( eDetectMode == eDetect_Mode)
    {
        FE_ERR(("Entered detection mode=%d have been set\n",eDetectMode));
        return TRUE;
    }
    else
    {
        _DigiTuner_ReleaseFETab();
        if(eDetectMode == AUTO_SCAN)
            FE_Detect_Done = FALSE;
        else
        {
            MApi_PrintTunerTypes();
            MApi_PrintDemodTypes();
        }
    }

    FE_DBG(("Set frontend detection mode=%d in frontend 0x%x OK!\n",eDetectMode,drv_frontend_index));
    eDetect_Mode = eDetectMode;
    return TRUE;
}

MS_BOOL MApi_Frontend_SetBroadcastType(MS_U8 drv_frontend_index,MS_U32 u32BroadcastType)
{
    DEV_FRONTEND_TYPE* pFETab;
    FE_MSG(("=========================== Broadcast Type List ==========================\n"));
    FE_MSG((" 0x%x -> BROADCAST_TYPE_NOT_SEETING\n",BROADCAST_TYPE_NOT_SEETING));
    FE_MSG((" 0x%x -> DVBC\n",DVBC));
    FE_MSG((" 0x%x -> DVBT\n",DVBT));
    FE_MSG((" 0x%x -> DVBT2\n",DVBT2));
    FE_MSG((" 0x%x -> ISDBT\n",ISDBT));
    FE_MSG((" 0x%x -> DVBS\n",DVBS));
    FE_MSG((" 0x%x -> ATSC\n",ATSC));
    FE_MSG((" 0x%x -> J83B\n",J83B));
    FE_MSG((" 0x%x -> DTMB\n",DTMB));
    FE_MSG((" 0x%x -> DVBS2\n",DVBS2));
    FE_MSG(("========================== End Broadcast Type List =======================\n"));

    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
    {
        DefaultBroadcastType = u32BroadcastType;
        FE_DBG(("\n[FE]Set default broadcast type=0x%"DTC_MS_U32_x" OK!\n", DefaultBroadcastType));
    }
    else
    {
        pFETab = *(pFETabAddr + drv_frontend_index);
        pFETab->dvb_type = u32BroadcastType;
        FE_DBG(("\n[FE]Set broadcast type=0x%"DTC_MS_U32_x" in frontend path 0x%x OK!\n", pFETab->dvb_type,drv_frontend_index));
    }

    return TRUE;
}

MS_U32 MApi_GetDmxInputPath(MS_U8 drv_frontend_index)
{
    DEV_FRONTEND_TYPE* pFETab;

    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        return 0xffff;
    pFETab = *(pFETabAddr + drv_frontend_index);
    return pFETab->dmx_input_path;
}

MS_BOOL MApi_Frontend_InExteralPath_Arrange(EN_IN_EX_TERNAL_PATH eInExTernalPath,DEV_FRONTEND_TYPE* pFEtab)
{
    static MS_U8 u8DMX_intDMD_Cnt = 0, u8DMX_extDMD_Cnt = 0;

    if(NULL == pFEtab)
        return FALSE;
    // set DMX input path
    if(eInExTernalPath == FE_EXTERNAL)
    {
        switch(u8DMX_extDMD_Cnt)
        {
        case 0:
#ifdef DMX_EXTERNAL_PATH_0
            pFEtab->dmx_input_path = DMX_EXTERNAL_PATH_0;
#else
            pFEtab->dmx_input_path = DMX_INPUT_EXT_INPUT0;
#endif
            break;
#if(defined(DMX_INPUT_EXT_INPUT1))
        case 1:
#ifdef DMX_EXTERNAL_PATH_1
            pFEtab->dmx_input_path = DMX_EXTERNAL_PATH_1;
#else
            pFEtab->dmx_input_path = DMX_INPUT_EXT_INPUT1;
#endif
            break;
#endif
#if(defined(DMX_INPUT_EXT_INPUT2))
        case 2:
            pFEtab->dmx_input_path = DMX_INPUT_EXT_INPUT2;
            break;
#endif
#if(defined(DMX_INPUT_EXT_INPUT3))
        case 3:
            pFEtab->dmx_input_path = DMX_INPUT_EXT_INPUT3;
            break;
#endif
        default:
            return FALSE;
        }
        u8DMX_extDMD_Cnt++;
    }
    else if(eInExTernalPath == FE_INTERNAL)
    {
        switch(u8DMX_intDMD_Cnt)
        {
        case 0:
            pFEtab->dmx_input_path = DMX_INPUT_DEMOD;
            break;
#if(defined(DMX_INPUT_DEMOD1))
        case 1:
            pFEtab->dmx_input_path = DMX_INPUT_DEMOD1;
            break;
#endif
        default:
            return FALSE;
        }
        u8DMX_intDMD_Cnt++;
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}
MS_BOOL MApi_Frontend_SetInExteralPath(MS_U8 IIC_index,EN_IN_EX_TERNAL_PATH eInExTernalPath, DEV_FRONTEND_TYPE* pFEtab)
{
    FE_MSG(("Set inter/exteranl path=%d\n",eInExTernalPath));
    FE_MSG(("(1 -> INTERNAL,2 -> EXTERNAL) \n\n"));
    pFEtab->eInExTernalPath = eInExTernalPath;

    switch(IIC_index)
    {
    default:
    case 0:
    {
        if (eInExTernalPath == FE_INTERNAL)
        {
            pFEtab->dmx_input_path = DMX_INPUT_DEMOD;
        }
        else
        {
#ifdef DMX_EXTERNAL_PATH_0
            pFEtab->dmx_input_path = DMX_EXTERNAL_PATH_0;
#else
            pFEtab->dmx_input_path = DMX_INPUT_EXT_INPUT0;
#endif
        }
        break;
    }

    case 1:
    {
        if (eInExTernalPath == FE_INTERNAL)
        {
            pFEtab->dmx_input_path = DMX_INPUT_DEMOD1;
        }
        else
        {
#ifdef DMX_EXTERNAL_PATH_1
            pFEtab->dmx_input_path = DMX_EXTERNAL_PATH_1;
#else
            pFEtab->dmx_input_path = DMX_INPUT_EXT_INPUT1;
#endif
        }
        break;
    }

    }

    return TRUE;
}

MS_BOOL MApi_DigiTuner_Init(MS_U8 drv_frontend_index)
{
    HWI2C_PORT hwi2c_port;
    DEMOD_MS_INIT_PARAM Demod_Init_Param = {NULL};
    TUNER_MS_INIT_PARAM Tuner_Init_Param = {NULL};
#if(MS_DVBS_INUSE == 1)
    DISH_MS_INIT_PARAM Dish_Init_Param = {NULL};
#endif
    MS_U8 u8Idx = 0;
    DEV_FRONTEND_TYPE* pFETab = NULL;

    if(NULL != pFETabAddr)
    {
        if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))//detected forntend index < request index
            return FALSE;
        pFETab = *(pFETabAddr + drv_frontend_index);
    }

    if(!FE_Load_Reset_Pin)
    {
        MApi_Frontend_LoadResetPin();
        FE_Load_Reset_Pin = TRUE;
    }

    if(_pu8Init != NULL)
    {
        if(*(_pu8Init + drv_frontend_index))
            return TRUE;
    }

    if(eDetect_Mode == DETECT_MODE_NOT_SETTING)
    {
        eDetect_Mode = AUTO_SCAN;
    }

    if(((pFETab == NULL) && (DefaultBroadcastType == BROADCAST_TYPE_NOT_SEETING)) ||\
            ((pFETab != NULL) && (pFETab->dvb_type== BROADCAST_TYPE_NOT_SEETING)))
    {
        FE_MSG(("Please set the broadcast type by using cmd: FE_SetBroadcastType arg0\n"));
        return FALSE;
    }

    if (_s32MutexId < 0)
    {
        _s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO, "DigiTuner_Mutex",MSOS_PROCESS_SHARED);
        FE_DBG(("MApi_DigiTuner_Init mutex id %"DTC_MS_U32_d" \n", _s32MutexId));
        if (_s32MutexId < 0)
        {
            FE_ERR(("%s: Create mutex failed.\n", __FUNCTION__));
            return FALSE;
        }
    }

    if((!FE_Detect_Done) && (eDetect_Mode == AUTO_SCAN))
    {
        if(!MApi_DigiTuner_Frontend_Scan())
            return FALSE;
        MApi_DigiTuner_Frontend_List_Show();
        if((drv_frontend_index > _DigiTuner_MaxFEIdx()) || (_DigiTuner_MaxFEIdx() == FEIndex_NONE))//detected forntend index < request index
            return FALSE;
        pFETab =  *(pFETabAddr + drv_frontend_index);
        pFETab->dvb_type = DefaultBroadcastType;
    }
    else if((!FE_Detect_Done) && (eDetect_Mode == USER_DEFINE))
    {
        FE_Detect_Done = TRUE;
    }

    pFETab->dmx_input_path = MApi_get_dmx_input_path(_DigiTuner_FEIdx2DevIdx(drv_frontend_index));
    hwi2c_port = getI2CPort(_DigiTuner_FEIdx2DevIdx(drv_frontend_index));
    MDrv_IIC_Init(hwi2c_port);


    if(pFETab->eInExTernalPath == FE_INTERNAL)
    {
        pFETab->demodtab = MApi_get_InternalDemodtab_entry(drv_frontend_index);
        if( DEMOD_NULL == pFETab->demodtab->data)
        {
            FE_ERR((" Internal Demod Not Support broadcast type: %x !!!\n", (MS_U8)pFETab->dvb_type));
            return FALSE;
        }
        FE_DBG(("Change internal DMD depend on Broadcast Type = %s\n",pFETab->demodtab->name));
    }

    u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    FE_DBG(("Idex of demod/tuner mapping from drv_frontend_index = %x\n", u8Idx));
#if(MS_DVBS_INUSE == 1)
    if((pFETab->dvb_type == DVBS) ||\
        (pFETab->dvb_type == DVBS2) ||\
       (pFETab->tunertab->data == TUNER_R848) ||\
       (pFETab->demodtab->data == DEMOD_MSB124X))
    {
        pFETab->dishtab = MApi_get_Dishtab_entry(_DigiTuner_FEIdx2DevIdx(drv_frontend_index));
        if(pFETab->dishtab != NULL)
        {
            hwi2c_port = getDishI2CPort(_DigiTuner_FEIdx2DevIdx(drv_frontend_index));
            MDrv_IIC_Init(hwi2c_port);
            Dish_Init_Param.pstDemodtab = pFETab->demodtab;
            if(pFETab->dishtab->Init(_DigiTuner_FEIdx2DevIdx(drv_frontend_index),&Dish_Init_Param) == FALSE)
            {
                FE_ERR(("%s: Init Dish fail\n", __FUNCTION__));
            }
        }
    }


#if DISABLE_SENT_SAME_DISEQC_CMD
     memset(pFETab->u8DiseqcCmd, 0x00, MAX_DISEQC_CMD_SIZE);
#endif

#endif

    //If KGC using on-chip msb1237,
    //It should close IQ control to enhance signal quility
#if defined(KGC_USE_ON_CHIP_MSB1237) && (KGC_USE_ON_CHIP_MSB1237 == 1)
    if (pFETab->demodtab->data == DEMOD_MSB1237)
    {
#define DMDBANK_OFFSET 0x12800
#define REG_IQ_ENABLE 0x1
#define REG_IQ_CTRL 0x3b
        FE_DBG(("Using ON_CHIP_MSB1237 close internal IQ\n"));
        MS_U32 _u32NONPMBase,u32NONPMBaseSize;
        if(!MDrv_MMIO_GetBASE(&_u32NONPMBase, &u32NONPMBaseSize, MS_MODULE_DMD))
        {
            FE_ERR(("Get MS_MODULE_DMD Base faill!\n"));
            return FALSE;
        }
        *((volatile MS_U16*)(_u32NONPMBase + (DMDBANK_OFFSET << 1) ) + (REG_IQ_ENABLE<<1)) = 0xF;
        *((volatile MS_U16*)(_u32NONPMBase + (DMDBANK_OFFSET << 1) ) + (REG_IQ_CTRL<<1)) = 0x3;
    }
#endif


    if((pFETab->bReset) == FALSE)
        MApi_Demod_HWReset(_DigiTuner_FEIdx2DevIdx(drv_frontend_index));

    Demod_Init_Param.pstTunertab= pFETab->tunertab;
    Demod_Init_Param.u32DmxInputPath = pFETab->dmx_input_path;
    Tuner_Init_Param.pCur_Broadcast_type= &(pFETab->dvb_type);

    if (pFETab->demodtab->init(u8Idx, &Demod_Init_Param) == FALSE)
    {
        FE_ERR(("Demod init fail, break! \n"));
        return FALSE;
    }
    else
    {
        FE_DBG(("Demod init ok \n"));
    }

    pFETab->demodtab->I2CByPass(u8Idx,TRUE);
    if (pFETab->tunertab->Init(u8Idx,&Tuner_Init_Param) == FALSE)
    {
        //pFETab->bReset = FALSE;
        pFETab->demodtab->I2CByPass(u8Idx,FALSE);
        FE_ERR(("Tuner init fail, break! \n"));
        return FALSE;
    }
    else
    {
        pFETab->demodtab->I2CByPass(u8Idx,FALSE);
        FE_DBG(("Tuner init ok \n"));
    }


    if (pFETab->demodtab->Extension_Function(u8Idx,DEMOD_EXT_FUNC_OPEN,NULL) == FALSE)
    {
        FE_ERR(("%s: Open demodulator fail\n", __FUNCTION__));
        return FALSE;
    }
    else
    {
        FE_DBG(("MDrv_Demod_Open ok \n"));
    }

    *(_pu8Init + drv_frontend_index) = 1;
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Power on,off digital tuner
/// @param eFE                      \b IN: Frontend id
/// @param bEnPower                 \b IN: TRUE - Power on
///                                        FALSE - Power off
/// @return TRUE:  Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_PowerOnOff(MS_U8 drv_frontend_index, MS_BOOL bEnPower)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
    {
        RETURN(FALSE);
    }

    pFETab = *(pFETabAddr + drv_frontend_index);

    if((pFETab->demodtab == NULL) || (pFETab->tunertab == NULL) )
    {
        RETURN(FALSE);
    }

#if MS_DVBS_INUSE
    if(pFETab->dvb_type == DVBS || pFETab->dvb_type == DVBS2)
    {
        if((pFETab->dishtab == NULL))
        {
            FE_ERR(("LNB Control IC is NOT assigned, Please set it in board header file\n"));
            RETURN(FALSE);
        }
    }
#endif
    pFETab->demodtab->I2CByPass(u8Idx,TRUE);
    pFETab->tunertab->Extension_Function(u8Idx,TUNER_EXT_FUNC_POWER_ON_OFF,&bEnPower);
    pFETab->demodtab->I2CByPass(u8Idx,FALSE);
    pFETab->demodtab->Extension_Function(u8Idx,DEMOD_EXT_FUNC_POWER_ON_OFF,&bEnPower);

    RETURN(TRUE);
}


//-------------------------------------------------------------------------------------------------
/// Tune tuner to RF channel by auto or manual mode
/// @param eFE                      \b IN: Frontend id
/// @param pstParam                 \b IN: Carrier parameters
/// @param eMode                    \b IN: Tuning mode (Auto or Manual)
/// @return TRUE:  Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
#if(MS_DVBS_INUSE == 1)
MS_BOOL MApi_DigiTuner_Tune2RfCh_DVBS(MS_U8 drv_frontend_index, MS_SAT_PARAM *pSATParam,MS_FE_CARRIER_PARAM *pParam, EN_FE_TUNE_MODE eMode)
{
    DEMOD_MS_FE_CARRIER_PARAM dmdParam;
    Demod_Mode dmdMode;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;
    MS_FE_CARRIER_PARAM* ptunerParam;
    MS_SAT_PARAM * psatParam;
#if DISABLE_SENT_SAME_DISEQC_CMD
    EN_LOCK_STATUS eCurLockStatus = E_DEMOD_UNLOCK;
#endif

    if((NULL == _ptunerParam) || (NULL == _pstSATParam) || (!_DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        return FALSE;

    memset(&dmdParam, 0x0, sizeof(DEMOD_MS_FE_CARRIER_PARAM));
    pFETab = *(pFETabAddr + drv_frontend_index);
    ptunerParam = _ptunerParam + drv_frontend_index;
    psatParam = _pstSATParam + drv_frontend_index;

    ENTRY();

#if DISABLE_SENT_SAME_DISEQC_CMD
        if (TRUE != pFETab->demodtab->GetLock(drv_frontend_index,&eCurLockStatus))
           pFETab->eCurrentLockStatus = E_DEMOD_UNLOCK;
        else
           pFETab->eCurrentLockStatus = eCurLockStatus;
#endif


    bIsHiLOF = FALSE;
    _DigiTuner_CalculateIF(drv_frontend_index,pSATParam,pParam,&dmdParam);
    _DigiTuner_SetLNBPower(drv_frontend_index, pSATParam->eLNBPwrOnOff,pParam->SatParam.u8Polarity,pSATParam->bPorInvert);
    _DigiTuner_SwitchSatellite(drv_frontend_index, pSATParam,pParam->SatParam.u8Polarity,bIsHiLOF);
    *ptunerParam = *pParam; //Must move here
    *psatParam = *pSATParam;

    if(FE_INTERNAL == pFETab->eInExTernalPath)
    {
        if (pFETab->tunertab->SetFreqS2(u8Idx, dmdParam.u32Frequency, (MS_U32)(pParam->SatParam.u16SymbolRate | (pParam->SatParam.u16ExtSymbolRate<<16))) == FALSE)
        {
            FE_ERR(("%s: Set frequency failed.\n", __FUNCTION__));
            RETURN(FALSE);
        }

        MsOS_DelayTask(50);

        memset(&dmdMode, 0x0, sizeof(Demod_Mode));

        if (FE_TUNE_AUTO == eMode)
        {
            // channel scan
            FE_DBG((" ==== FE_TUNE_AUTO ==== \n"));
            dmdMode.bX4CFE_en       = X4CFE_ENABLE_MODE; // X4CFE_ENABLE_MODE;
            dmdMode.bPPD_en        = PPD_ENABLE_MODE; // PPD_ENABLE_MODE;

            dmdMode.bIQAutoSwap_en = IQ_AUTO_ENABLE_MODE;
            dmdMode.bQAMScan_en    = QAM_AUTO_ENABLE_MODE;
            dmdMode.bFHO_en        = FHO_ENABLE_MODE;
        }
        else
        {
            // channel switch

            memset(&dmdMode, 0, sizeof(dmdMode));
            FE_DBG(("non auto \n"));
            dmdMode.bX4CFE_en       = X4CFE_ENABLE_MODE; // X4CFE_ENABLE_MODE;
            dmdMode.bPPD_en        = PPD_ENABLE_MODE; // PPD_ENABLE_MODE;
            dmdMode.bIQAutoSwap_en    = IQ_AUTO_ENABLE_MODE;
            dmdMode.bQAMScan_en     = QAM_AUTO_ENABLE_MODE;
            dmdMode.bFHO_en         = FHO_ENABLE_MODE;
        }

        dmdMode.fptTunerSet     = FALSE;

        dmdParam.SatParam.u32SymbolRate = pParam->SatParam.u16SymbolRate | (pParam->SatParam.u16ExtSymbolRate << 16);
        dmdParam.SatParam.u32SymbolRate *= 1000;
    }
    else
    {
        dmdParam.SatParam.u32SymbolRate = pParam->SatParam.u16SymbolRate | (pParam->SatParam.u16ExtSymbolRate << 16);
        dmdParam.SatParam.u32SymbolRate *= 1000;
    }

    if (pFETab->demodtab->Restart(u8Idx,&dmdParam,pFETab->dvb_type) == FALSE)
    {
        FE_ERR(("%s: COfdm DMD restart failed.\n", __FUNCTION__));
        RETURN(FALSE);
    }


    FE_DBG(("Tuner Success \n"));
    RETURN(TRUE);
}

#ifdef SUPPORT_DUAL_LNB
MS_BOOL MApi_DigiTuner_Tune2RfCh_DVBS_Ext(MS_U8 drv_frontend_index, MS_SAT_PARAM *pSATParam,MS_FE_CARRIER_PARAM *pParam, EN_CABLE_SELECT eCableID)
{
    DEMOD_MS_FE_CARRIER_PARAM dmdParam;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;
    MS_FE_CARRIER_PARAM* ptunerParam;
    MS_SAT_PARAM * psatParam;
#if DISABLE_SENT_SAME_DISEQC_CMD
    EN_LOCK_STATUS eCurLockStatus = E_DEMOD_UNLOCK;
#endif
    EN_CABLE_SELECT eCable_Sel;

    if((NULL == _ptunerParam) || (NULL == _pstSATParam) || (!_DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        return FALSE;

    memset(&dmdParam, 0x0, sizeof(DEMOD_MS_FE_CARRIER_PARAM));
    pFETab = *(pFETabAddr + drv_frontend_index);
    ptunerParam = _ptunerParam + drv_frontend_index;
    psatParam = _pstSATParam + drv_frontend_index;

    ENTRY();
    eCable_Sel = eCableID;
    pFETab->dishtab->SetCable(_DigiTuner_FEIdx2DevIdx(drv_frontend_index), eCableID);

    pFETab->tunertab->Extension_Function(u8Idx, TUNER_EXT_FUNC_SET_CABLE_INDEX, &eCable_Sel);

#if DISABLE_SENT_SAME_DISEQC_CMD
        if (TRUE != pFETab->demodtab->GetLock(drv_frontend_index,&eCurLockStatus))
           pFETab->eCurrentLockStatus = E_DEMOD_UNLOCK;
        else
           pFETab->eCurrentLockStatus = eCurLockStatus;
#endif


    bIsHiLOF = FALSE;


    _DigiTuner_CalculateIF(drv_frontend_index,pSATParam,pParam,&dmdParam);
    _DigiTuner_SetLNBPower(drv_frontend_index, pSATParam->eLNBPwrOnOff,pParam->SatParam.u8Polarity,pSATParam->bPorInvert);
    _DigiTuner_SwitchSatellite(drv_frontend_index, pSATParam,pParam->SatParam.u8Polarity,bIsHiLOF);
    *ptunerParam = *pParam; //Must move here
    *psatParam = *pSATParam;


    dmdParam.SatParam.u32SymbolRate = pParam->SatParam.u16SymbolRate | (pParam->SatParam.u16ExtSymbolRate << 16);
    dmdParam.SatParam.u32SymbolRate *= 1000;


    if (pFETab->demodtab->Restart(u8Idx,&dmdParam,pFETab->dvb_type) == FALSE)
    {
        FE_ERR(("%s: COfdm DMD restart failed.\n", __FUNCTION__));
        RETURN(FALSE);
    }


    FE_DBG(("Tuner Success \n"));
    RETURN(TRUE);
}
#endif

void MApi_DigiTuner_ForceRetune(MS_U8 drv_frontend_index)
{
    DEMOD_MS_FE_CARRIER_PARAM    dmdParam;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;
    MS_FE_CARRIER_PARAM* ptunerParam;
    MS_SAT_PARAM * psatParam;


    if((NULL == _ptunerParam) || (NULL == _pstSATParam) || (!_DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        return;

    pFETab = *(pFETabAddr + drv_frontend_index);
    ptunerParam = _ptunerParam + drv_frontend_index;
    psatParam = _pstSATParam + drv_frontend_index;

    _DigiTuner_SetLNBPower(drv_frontend_index, psatParam->eLNBPwrOnOff,ptunerParam->SatParam.u8Polarity,psatParam->bPorInvert);
    _DigiTuner_SwitchSatellite(drv_frontend_index, psatParam, ptunerParam->SatParam.u8Polarity,bIsHiLOF);

    _DigiTuner_CalculateIF(drv_frontend_index, psatParam,ptunerParam,&dmdParam);
    dmdParam.SatParam.u32SymbolRate = ptunerParam->SatParam.u16SymbolRate | (ptunerParam->SatParam.u16ExtSymbolRate << 16);
    dmdParam.SatParam.u32SymbolRate *= 1000;

    if (pFETab->demodtab->Restart(u8Idx,&dmdParam,pFETab->dvb_type) == FALSE)
    {
        FE_ERR(("%s: COfdm DMD restart failed.\n", __FUNCTION__));
    }
}
#endif

MS_BOOL MApi_DigiTuner_Tune2RfCh(MS_U8 drv_frontend_index, MS_FE_CARRIER_PARAM *pParam, EN_FE_TUNE_MODE eMode)
{
#if (FE_DEBUG_OPTIONS & FE_EN_DBG)
    MS_U32 u32DemodType;
    MS_U32 u32TunerType;
#endif
    MS_U32 u32frontendtype;
    MS_U32 u32Timeout;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;
    MS_FE_CARRIER_PARAM* ptunerParam;
    MS_U8 u8CAB_TunerBW = TUNER_BW_MODE_8MHZ;

    ENTRY();

    if((NULL == _ptunerParam)||(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        RETURN(FALSE);

    if ( !MApi_DigiTuner_Get_FE_InitStatus(drv_frontend_index))
    {
        RETURN(FALSE);
    }

    pFETab = *(pFETabAddr + drv_frontend_index);
    ptunerParam = _ptunerParam + drv_frontend_index;
#if (FE_DEBUG_OPTIONS & FE_EN_DBG)
    u32DemodType = pFETab->demodtab->data;
    u32TunerType = pFETab->tunertab->data;
#endif
    u32frontendtype = MApi_DigiTuner_GetFrontendType(drv_frontend_index);

#ifdef DDI_MISC_INUSE
#if MS_DVBT2_INUSE
    T2PlpID = 0xFF;
#endif
#endif

    FE_DBG(("u32DemodType: %"DTC_MS_U32_d" u32TunerType:%"DTC_MS_U32_d" u32frontendtype:%"DTC_MS_U32_d"\n", u32DemodType, u32TunerType, u32frontendtype));

    memcpy(ptunerParam, pParam, sizeof(MS_FE_CARRIER_PARAM)); //Must move here

    if( (u32frontendtype == DVBC) || (u32frontendtype == J83B))
    {
        Demod_Mode dmdMode;
        DEMOD_MS_FE_CARRIER_PARAM dmdParam;

        memset(&dmdMode, 0x0, sizeof(Demod_Mode));
        memset(&dmdParam, 0x0, sizeof(DEMOD_MS_FE_CARRIER_PARAM));

        dmdMode.fptTunerSet     = FALSE;

        pFETab->demodtab->Extension_Function(u8Idx,DEMOD_EXT_FUNC_SET_MODE,&dmdMode);

        dmdParam.CabParam.eConstellation = pParam->CabParam.eConstellation;
        dmdParam.CabParam.u16SymbolRate = pParam->CabParam.u16SymbolRate;
        dmdParam.CabParam.eIQMode = pParam->CabParam.eIQMode;
#ifdef SUPPORT_DVBC_DMD_BW_CHANGE
#if SUPPORT_DVBC_DMD_BW_CHANGE
        dmdParam.CabParam.eBandWidth  = pParam->CabParam.eBandWidth;
#endif
#endif
        dmdParam.u32Frequency  = pParam->u32Frequency;

        pFETab->demodtab->I2CByPass(u8Idx,TRUE);
        switch(pParam->CabParam.eBandWidth)
        {
        case DEMOD_CAB_BW_6M:
            u8CAB_TunerBW = TUNER_BW_MODE_6MHZ;
            break;
        case DEMOD_CAB_BW_7M:
            u8CAB_TunerBW = TUNER_BW_MODE_7MHZ;
            break;
        case DEMOD_CAB_BW_8M:
        default:
            u8CAB_TunerBW = TUNER_BW_MODE_8MHZ;
            break;

        }
        if (pFETab->tunertab->SetTuner(u8Idx,pParam->u32Frequency, u8CAB_TunerBW)==FALSE)
        {
            FE_ERR(("%s: Set frequency failed.\n", __FUNCTION__));
            pFETab->demodtab->I2CByPass(u8Idx,FALSE);
            RETURN(FALSE);
        }
        else
        {
            pFETab->demodtab->I2CByPass(u8Idx,FALSE);
            FE_DBG(("%s:set tuner ok \n", __FUNCTION__));
        }



        u32Timeout = MsOS_GetSystemTime();
        while (pFETab->demodtab->Restart(u8Idx,&dmdParam,pFETab->dvb_type) != TRUE)
        {
            if ((MsOS_GetSystemTime()-u32Timeout) >= 5000)
            {
                FE_ERR(("%s: demodulator restart fail.\n", __FUNCTION__));
                RETURN(FALSE);
            }
        }

    }
    else
    {
        DEMOD_MS_FE_CARRIER_PARAM    dmdParam;

        memset(&dmdParam, 0x0, sizeof(DEMOD_MS_FE_CARRIER_PARAM));
        dmdParam.u32Frequency         = pParam->u32Frequency;
        dmdParam.TerParam.eBandWidth  = pParam->TerParam.eBandWidth;//u16BandWidth / 1000;
        dmdParam.TerParam.u8PlpID     = pParam->TerParam.u8PlpID;
#ifdef DDI_MISC_INUSE
#if(MS_DVBT2_INUSE == 1)
        T2PlpID = pParam->TerParam.u8PlpID;
#endif
        dmdParam.TerParam.u8ScanType  = pParam->TerParam.u8ScanType;
#endif

        pFETab->demodtab->I2CByPass(u8Idx,TRUE);

        if(pFETab->tunertab->SetTuner(u8Idx,pParam->u32Frequency,pParam->TerParam.eBandWidth)==FALSE)
        {
            pFETab->demodtab->I2CByPass(u8Idx,FALSE);
            FE_ERR(("%s:set tuner  failed.\n", __FUNCTION__));
            RETURN(FALSE);
        }
        else
        {
            pFETab->demodtab->I2CByPass(u8Idx,FALSE);
            FE_DBG(("%s:set tuner ok \n", __FUNCTION__));
        }

        if (pFETab->demodtab->Restart(u8Idx,&dmdParam,pFETab->dvb_type) == FALSE)
        {
            FE_ERR(("%s: Demod restart failed.\n", __FUNCTION__));
            RETURN(FALSE);
        }
    }
    FE_DBG(("Tune 2 RF ok!\n"));
    RETURN(TRUE);
}

//-------------------------------------------------------------------------------------------------
/// Report Estimated Maximum Lock Time of Tune2RF mode
/// @param eMode                    \b IN: Tuning mode (Auto or Manual)
/// @return TRUE:  Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetMaxLockTime(MS_U8 drv_frontend_index, EN_FE_TUNE_MODE eMode, MS_U32 *u32LockTime)
{
    MS_U32 u32DemodType;
    MS_U32 u32AutoTuneTimeOut;
    MS_U32 u32ManTuneTimeOut;
#if MS_DVBT2_INUSE
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
#endif
    DEV_FRONTEND_TYPE* pFETab;

    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        return FALSE;

    pFETab = *(pFETabAddr + drv_frontend_index);
    u32DemodType = pFETab->demodtab->data;
    if (u32DemodType == DEMOD_MSINTERN_DVBC ||
            u32DemodType == DEMOD_MSKENYA_DVBC)
    {
        u32AutoTuneTimeOut = 2000;
        u32ManTuneTimeOut = 1500;
    }
    else if(u32DemodType == DEMOD_MSB1400)
    {
        u32AutoTuneTimeOut = 2300;
        u32ManTuneTimeOut = 2300;
    }
    else
    {
        u32AutoTuneTimeOut = 2000;
        u32ManTuneTimeOut = 600;
    }

#if MS_DVBT2_INUSE
    if(u32DemodType ==DEMOD_MSB1236C ||
            (u32DemodType == DEMOD_MSB124X && (pFETab->dvb_type == DVBT || pFETab->dvb_type == DVBT2)) ||
            u32DemodType == DEMOD_MSKRITI_DVBT2)
        if (E_DEMOD_TYPE_T == pFETab->demodtab->GetCurrentDemodType(u8Idx))
        {
            *u32LockTime = TUNE_T_TIMEOUT;
        }
        else
        {
            *u32LockTime = TUNE_T2_TIMEOUT;
        }
    else
#endif
        //if (u32DemodType == DEMOD_MSB131X ||
        //        (u32DemodType == DEMOD_MSB124X && (pFETab->dvb_type == DVBS || pFETab->dvb_type == DVBS2)) ||
        //        u32DemodType == DEMOD_MSB1245)
       if (pFETab->dvb_type == DVBS || pFETab->dvb_type == DVBS2)
        {
            *u32LockTime = 3000 * 2;
        }
        else
        {
            if (FE_TUNE_AUTO == eMode)
            {
                *u32LockTime = u32AutoTuneTimeOut;
            }
            else
            {
                *u32LockTime = u32ManTuneTimeOut;
            }
        }
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Get tuner lock status
/// @param eFE                      \b IN: Frontend id
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetLock(MS_U8 drv_frontend_index, EN_LOCK_STATUS *peLockStatus)
{
    MS_BOOL bRet = FALSE;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    if((pFETab->demodtab == NULL) || (pFETab->tunertab == NULL) )
    {
        RETURN(FALSE);
    }

    bRet = pFETab->demodtab->GetLock(u8Idx,peLockStatus);
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_GetPreLock(MS_U8 drv_frontend_index, EN_LOCK_STATUS *peLockStatus)
{
    MS_BOOL bRet = FALSE;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    if((pFETab->demodtab == NULL) || (pFETab->tunertab == NULL) )
    {
        RETURN(FALSE);
    }

    // Note: So far(2014/02/12) only MSB1237 support this function
    bRet = pFETab->demodtab->Extension_Function(u8Idx,DEMOD_EXT_FUNC_ATSC_GET_PRELOCK,peLockStatus);
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_GetFlameSyncLock(MS_U8 drv_frontend_index, EN_LOCK_STATUS *peLockStatus)
{
    MS_BOOL bRet = FALSE;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    if((pFETab->demodtab == NULL) || (pFETab->tunertab == NULL) )
    {
        RETURN(FALSE);
    }
    // Note: So far(2014/02/12) only MSB1237 support this function
    bRet = pFETab->demodtab->Extension_Function(u8Idx,DEMOD_EXT_FUNC_ATSC_GET_FLAME_SYNC_LOCK,peLockStatus);
    RETURN(bRet);
}



//-------------------------------------------------------------------------------------------------
/// Get tuner SNR
/// @param pu32SNR                  \b OUT: Signal noise ratio
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetSNR(MS_U8 drv_frontend_index, MS_U16* pu16SNR)
{
    float fSNR;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    if((pFETab->demodtab == NULL) || (pFETab->tunertab == NULL) )
    {
        RETURN(FALSE);
    }

    if (pFETab->demodtab->GetSNR(u8Idx,&fSNR) != TRUE)
    {
        FE_ERR(("FAIL to Get SNR\n"));
        fSNR = 0;
    }
    FE_DBG(("%s: %f dB\n", __FUNCTION__, fSNR));
    *pu16SNR = (MS_U16)(fSNR);
    RETURN(TRUE);
}


//-------------------------------------------------------------------------------------------------
/// Get tuner BER
/// @param pu32BER                  \b OUT: Bit error ratio (n * 2^-32)
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetBER(MS_U8 drv_frontend_index, float *pfltBER)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    if((pFETab->demodtab == NULL) || (pFETab->tunertab == NULL) )
    {
        FE_DBG(("FAIL to Get BER\n"));
        RETURN(FALSE);
    }

    pFETab->demodtab->GetBER(u8Idx,pfltBER);
    FE_DBG(("%s: %8.3e\n", __FUNCTION__, *pfltBER));
    RETURN(TRUE);
}


//-------------------------------------------------------------------------------------------------
/// Get tuner Signal Power Strength
/// @param pu8PWR                   \b OUT: Signal power strength
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetPWR(MS_U8 drv_frontend_index, MS_S16* ps16PWR)
{
    MS_S32  s32PWR;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    if((pFETab->demodtab == NULL) || (pFETab->tunertab == NULL) )
    {
        RETURN(FALSE);
    }

    if (pFETab->demodtab->GetPWR(u8Idx,&s32PWR) == FALSE)
    {
        RETURN(FALSE);
    }
    *ps16PWR = (MS_S16)(s32PWR);
    FE_DBG(("%s: %d dBm", __FUNCTION__, *ps16PWR));
#if 1 // byKOR, added
    if (pFETab->tunertab->Extension_Function(u8Idx, TUNER_EXT_FUNC_GET_POWER_LEVEL, &s32PWR) == FALSE)
    {
        RETURN(FALSE);
	}
    *ps16PWR = (MS_S16)(s32PWR);
    FE_DBG((" ==> Total(RF+IF): %d dBuV\n", *ps16PWR));
#endif
    RETURN(TRUE);
}


//-------------------------------------------------------------------------------------------------
/// Get tuner Signal Power Strength
/// @param pu8PWR                   \b OUT: Signal power strength
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetSignalQuality(MS_U8 drv_frontend_index, MS_U16* pu16quality)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    if((pFETab->demodtab == NULL) || (pFETab->tunertab == NULL) )
    {
        RETURN(FALSE);
    }

    if (pFETab->demodtab->GetQuality(u8Idx,pu16quality) == FALSE)
    {
        FE_ERR(("FAIL to Get SQI\n"));
        RETURN(FALSE);
    }

    FE_DBG(("%s: %d (0 ~ 100)\n", __FUNCTION__, *pu16quality));
    RETURN(TRUE);
}

//-------------------------------------------------------------------------------------------------
/// Get tuner Signal Power Strength from Tuner
/// @param pu8PWR                   \b OUT: Signal power level
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetPWRFromTuner(MS_U8 drv_frontend_index, float* pfPWR)
{
    float fPowerLevel = 0;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    if((pFETab->demodtab == NULL) || (pFETab->tunertab == NULL) )
    {
        RETURN(FALSE);
    }

    pFETab->demodtab->I2CByPass(u8Idx,TRUE);
    // get from tuner
    if(pFETab->tunertab->Extension_Function(u8Idx, TUNER_EXT_FUNC_GET_POWER_LEVEL, &fPowerLevel) == FALSE)
    {
        pFETab->demodtab->I2CByPass(u8Idx,FALSE);
        RETURN(FALSE);
    }
    pFETab->demodtab->I2CByPass(u8Idx,FALSE);
    *pfPWR = fPowerLevel;
    FE_DBG(("%s: %lf dBm\n", __FUNCTION__, fPowerLevel));
    RETURN(TRUE);
}

//-------------------------------------------------------------------------------------------------
/// Get packet error
/// @param RegData                   \b OUT: packet error
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_Get_Packet_Error(MS_U8 drv_frontend_index, MS_U16 *RegData)
{
    MS_BOOL bRet = FALSE;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    if((pFETab->demodtab == NULL) || (pFETab->tunertab == NULL) )
    {
        RETURN(FALSE);
    }

    bRet = pFETab->demodtab->Get_Packet_Error(u8Idx, RegData);
    FE_DBG(("%s: Packet_Error %x \n", __FUNCTION__, *RegData));
    RETURN(bRet);

}


//-------------------------------------------------------------------------------------------------
/// Get tuner parameter setting
/// @param eFE                      \b IN: Frontend id
/// @param pstParam                 \b OUT: TPS carrier parameters
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_GetTPSInfo(MS_U8 drv_frontend_index, MS_FE_CARRIER_PARAM* pParam)
{
    //MS_U32 u32DemodType;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;
    MS_FE_CARRIER_PARAM* ptunerParam;
    DEMOD_MS_FE_CARRIER_PARAM dmdParam;

    ENTRY();
    if((NULL == _ptunerParam)||(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);
    ptunerParam = _ptunerParam + drv_frontend_index;

    if((pFETab->demodtab == NULL) || (pFETab->tunertab == NULL) )
    {
        RETURN(FALSE);
    }

    //u32DemodType = pFETab->demodtab->data;


    // Get current frequency
    memcpy(pParam, ptunerParam, sizeof(MS_FE_CARRIER_PARAM));

    if (pFETab->demodtab->GetParam(u8Idx,&dmdParam) == FALSE)
    {
        FE_ERR(("%s: failed.\n", __FUNCTION__));
        RETURN(FALSE);
    }

    if(pFETab->dvb_type == DVBC)
    {
        pParam->CabParam.eConstellation = (EN_CAB_CONSTEL_TYPE)dmdParam.CabParam.eConstellation;
        pParam->CabParam.eIQMode        = (EN_CAB_IQ_MODE)dmdParam.CabParam.eIQMode;
        pParam->CabParam.u8TapAssign    = dmdParam.CabParam.u8TapAssign;
        pParam->CabParam.fCFO           = dmdParam.CabParam.fCFO;
        pParam->CabParam.u8TuneFreqOffset = dmdParam.CabParam.u8TuneFreqOffset;
    }
    else if((pFETab->dvb_type == DVBT) || (pFETab->dvb_type == DVBT2))
    {
        pParam->TerParam.eConstellation = (EN_TER_CONSTEL_TYPE)dmdParam.TerParam.eConstellation;
        pParam->TerParam.eHierarchy = (EN_TER_HIE_TYPE)dmdParam.TerParam.eHierarchy;
        pParam->TerParam.eLPCodeRate = (DEMOD_EN_CONV_CODE_RATE_TYPE)dmdParam.TerParam.eLPCodeRate;
        pParam->TerParam.eHPCodeRate = (DEMOD_EN_CONV_CODE_RATE_TYPE)dmdParam.TerParam.eHPCodeRate;
        pParam->TerParam.eGuardInterval = (DEMOD_EN_TER_GI_TYPE)dmdParam.TerParam.eGuardInterval;
        pParam->TerParam.eFFT_Mode = (DEMOD_EN_TER_FFT_MODE)dmdParam.TerParam.eFFT_Mode;
    }
    else if(pFETab->dvb_type == DVBS || pFETab->dvb_type == DVBS2)
    {
        pParam->SatParam.eConstellation = (EN_SAT_CONSTEL_TYPE)dmdParam.SatParam.eConstellation;
        pParam->SatParam.eCodeRate      = (EN_CONV_CODE_RATE_TYPE)dmdParam.SatParam.eCodeRate;
        pParam->SatParam.eRollOff       = (EN_SAT_ROLL_OFF_TYPE)dmdParam.SatParam.eRollOff;
        pParam->SatParam.fCFO           = dmdParam.SatParam.fCFO;
    }

    RETURN(TRUE);
}

MS_BOOL MApi_DigiTuner_Loop_Through_On(MS_U8 drv_frontend_index, MS_BOOL bOn)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);
    if((pFETab->demodtab == NULL) || (pFETab->tunertab == NULL) )
    {
        RETURN(FALSE);
    }

    pFETab->demodtab->I2CByPass(u8Idx,TRUE);
    pFETab->tunertab->Extension_Function(u8Idx,TUNER_EXT_FUNC_LOOP_THROUGH,&bOn);
    pFETab->demodtab->I2CByPass(u8Idx,FALSE);

    RETURN(TRUE);
}

MS_BOOL MApi_DigiTuner_Stand_By(MS_U8 drv_frontend_index)
{
    MS_U32 u32DemodType;
    MS_U32 u32TunerType;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    if((pFETab->demodtab == NULL) || (pFETab->tunertab == NULL) )
    {
        RETURN(FALSE);
    }

    u32DemodType = pFETab->demodtab->data;
    u32TunerType = pFETab->tunertab->data;


    if ((u32DemodType == DEMOD_MSB123X)||(u32DemodType == DEMOD_MSB1233C)||(u32DemodType == DEMOD_MSB1236C))
    {
        if((u32TunerType==TUNER_MXL201RF) || (u32TunerType==TUNER_NXP_TDA18219))
        {
            pFETab->demodtab->I2CByPass(u8Idx,TRUE);
        }
    }
    pFETab->tunertab->Extension_Function(u8Idx,TUNER_EXT_FUNC_STANDBY,NULL);
    if ((u32DemodType == DEMOD_MSB123X)||(u32DemodType == DEMOD_MSB1233C)||(u32DemodType == DEMOD_MSB1236C))
    {
        if((u32TunerType==TUNER_MXL201RF) || (u32TunerType==TUNER_NXP_TDA18219))
        {
            pFETab->demodtab->I2CByPass(u8Idx,FALSE);
        }
    }
    RETURN(TRUE);
}

MS_BOOL MApi_DigiTuner_Wake_Up(MS_U8 drv_frontend_index)
{
    MS_U32 u32DemodType;
    MS_U32 u32TunerType;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    if((pFETab->demodtab == NULL) || (pFETab->tunertab == NULL) )
    {
        RETURN(FALSE);
    }

    u32DemodType = pFETab->demodtab->data;
    u32TunerType = pFETab->tunertab->data;

    if ((u32DemodType == DEMOD_MSB123X)||
            (u32DemodType == DEMOD_MSB1233C)||
            (u32DemodType == DEMOD_MSB1236C))
    {
        if((u32TunerType==TUNER_MXL201RF) || (u32TunerType==TUNER_NXP_TDA18219))
        {
            pFETab->demodtab->I2CByPass(u8Idx,TRUE);
        }
    }
    pFETab->tunertab->Extension_Function(u8Idx,TUNER_EXT_FUNC_WAKEUP,NULL);
    if ((u32DemodType == DEMOD_MSB123X)||
            (u32DemodType == DEMOD_MSB1233C)||
            (u32DemodType == DEMOD_MSB1236C))
    {
        if((u32TunerType==TUNER_MXL201RF) || (u32TunerType==TUNER_NXP_TDA18219))
        {
            pFETab->demodtab->I2CByPass(u8Idx,FALSE);
        }
    }
    RETURN(TRUE);
}

MS_BOOL MApi_Frontend_LoadResetPin(void)
{
    MS_U8 u8Port_Idx;

    for(u8Port_Idx=0; u8Port_Idx< MAX_FRONTEND_NUM; u8Port_Idx++)
    {
        switch(u8Port_Idx)
        {
        case 0:
            FE_Reset_Pin[u8Port_Idx] = GPIO_FE_RST;
            break;
#if (MAX_FRONTEND_NUM > 1)
        case 1:
            FE_Reset_Pin[u8Port_Idx] = GPIO_FE_RST1;
            break;
#endif
#if (MAX_FRONTEND_NUM > 2)
        case 2:
            FE_Reset_Pin[u8Port_Idx] = GPIO_FE_RST2;
            break;
#endif
#if (MAX_FRONTEND_NUM > 3)
        case 3:
            FE_Reset_Pin[u8Port_Idx] = GPIO_FE_RST3;
            break;
#endif
        }
    }
    return TRUE;
}

//------------------------------------------------------
// function callback by  demod layer
//
//--------------------------------------------------------
//demod  reset pin
void MApi_Demod_HWReset(MS_U8 u8Port_Idx)
{
    MS_U32 gpio_fe_rst_num = 0;
    MS_U8 i,u8FEIdx, u8FE1stIdx = 0;
    DEV_FRONTEND_TYPE* pFETab;

    if(!FE_Load_Reset_Pin)
    {
        MApi_Frontend_LoadResetPin();
        FE_Load_Reset_Pin = TRUE;
    }

    gpio_fe_rst_num = FE_Reset_Pin[u8Port_Idx];

    FE_DBG(("%s: Pin %d\n", __FUNCTION__, (int)gpio_fe_rst_num));
    for(i=0; i< MAX_FRONTEND_NUM; i++)
    {
        if(gpio_fe_rst_num == FE_Reset_Pin[i])
        {
            if(FE_Detect_Done)
            {
                if(i)
                {
                    u8FE1stIdx = u32ch_acc[i-1];
                }

                for(u8FEIdx=u8FE1stIdx; u8FEIdx<u32ch_acc[i]; u8FEIdx++)
                {
                    pFETab = *(pFETabAddr + u8FEIdx);
                    pFETab->bReset = TRUE;       // record that HW reset is done in when multipule FEs share this reset pin
                    MApi_DigiTuner_DeInit(u8FEIdx);
                }
            }

        }
    }
    mdrv_gpio_set_high(gpio_fe_rst_num);
    MsOS_DelayTask(5);
    mdrv_gpio_set_low(gpio_fe_rst_num);
    MsOS_DelayTask(10);
    mdrv_gpio_set_high(gpio_fe_rst_num);
    MsOS_DelayTask(5);

}



MS_BOOL MApi_DigiTuner_Get_FE_InitStatus(MS_U8 drv_frontend_index)
{
    if((NULL == _pu8Init) || (!_DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        return FALSE;
    else
        return *(_pu8Init+drv_frontend_index);
}

MS_BOOL MApi_DigiTuner_Set_DMD_BUFFER(MS_U8 drv_frontend_index, MS_U32 u32StartAddr)
{
    DEV_FRONTEND_TYPE* pFETab;
    MS_U32 u32Addr;
    MS_BOOL bret=TRUE;

    u32Addr = u32StartAddr;
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        return FALSE;

    pFETab = *(pFETabAddr + drv_frontend_index);
    bret = pFETab->demodtab->Extension_Function(drv_frontend_index, DEMOD_EXT_FUNC_SET_BUFFER_ADDR, &u32Addr);

    return bret;
}

/*
MS_BOOL MApi_DigiTuner_SetStateChange(MS_BOOL bOn)
{
    ENTRY();
    StateChg = bOn;
    RETURN(TRUE);

}
*/

#if(MS_DVBT2_INUSE == 1)

MS_U8 MApi_DigiTuner_GetCurrentDemodType(MS_U8 drv_frontend_index)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    MS_U8 ret;
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);
    ret = pFETab->demodtab->GetCurrentDemodType(u8Idx);
    RETURN(ret);
}


MS_BOOL MApi_DigiTuner_SetCurrentDemodType(MS_U8 drv_frontend_index, MS_U8 type)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    MS_BOOL ret;
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    ret =  pFETab->demodtab->SetCurrentDemodType(u8Idx, type);
    RETURN(ret);
}

MS_BOOL MApi_DigiTuner_SetPlpGroupID(MS_U8 drv_frontend_index, MS_U8 u8PlpID, MS_U8 u8GroupID)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    MS_BOOL ret;
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    ret = pFETab->demodtab->SetPlpGroupID(u8Idx, u8PlpID, u8GroupID);
    RETURN(ret);
}

MS_BOOL MApi_DigiTuner_GetPlpGroupID(MS_U8 drv_frontend_index, MS_U8 u8PlpID, MS_U8* u8GroupID)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    MS_BOOL ret;
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    ret = pFETab->demodtab->GetPlpGroupID(u8Idx, u8PlpID, u8GroupID);
    RETURN(ret);
}

MS_BOOL MApi_DigiTuner_GetPlpBitMap(MS_U8 drv_frontend_index, MS_U8* u8PlpBitMap)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    MS_BOOL ret;
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    ret = pFETab->demodtab->GetPlpBitMap(u8Idx, u8PlpBitMap);
    RETURN(ret);
}

MS_BOOL MApi_DigiTuner_GetPlpType(MS_U8 drv_frontend_index, MS_U8 u8PlpID, DEMOD_DVBT2_PLP_TYPE* pePLPTYPE)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    MS_BOOL ret;
    DEV_FRONTEND_TYPE* pFETab;
    MS_U8 u8GroupID;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    ret = pFETab->demodtab->GetPlpGroupID(u8Idx, u8PlpID, &u8GroupID);
    ret = pFETab->demodtab->SetPlpGroupID(u8Idx, u8PlpID, u8GroupID);
    ret = pFETab->demodtab->GetPLPType(u8Idx, pePLPTYPE);

    RETURN(ret);

}


#endif


#if MS_DVBS_INUSE


MS_BOOL MApi_DigiTuner_Demod_ClearStatus(MS_U8 drv_frontend_index)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    MS_BOOL bRet = TRUE;
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    if(pFETab->demodtab->data == DEMOD_MSB1245)
    {
        bRet = pFETab->demodtab->Extension_Function(u8Idx, DEMOD_EXT_FUNC_CLEAR_STATUS,NULL);
    }

    RETURN(bRet);;
}

MS_U16 MApi_DigiTuner_getCurRFSignalSNR_Persent(MS_U8 drv_frontend_index)
{
#define DEFAULT_SNR_MIN 0
#define DEFAULT_SNR_MAX 35
    MS_U16 _u16snr = 0;

    MApi_DigiTuner_GetSNR(drv_frontend_index, &_u16snr);
    return _u16snr * 100 / (DEFAULT_SNR_MAX - DEFAULT_SNR_MIN);
}

MS_BOOL MApi_DigiTuner_GetRollOff(MS_U8 drv_frontend_index, EN_SAT_ROLL_OFF_TYPE *pRollOff)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    MS_BOOL bRet = FALSE;
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);
    bRet = pFETab->demodtab->Extension_Function(u8Idx,DEMOD_EXT_FUNC_GET_ROLL_OFF,pRollOff);
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_GetRFOffset(MS_U8 drv_frontend_index, MS_S16 *ps16RFOffset)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    MS_BOOL bRet = FALSE;
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);
    bRet = pFETab->demodtab->Extension_Function(u8Idx,DEMOD_EXT_FUNC_GET_RF_OFFSET,ps16RFOffset);

    RETURN(bRet);
}

//MS_BOOL MApi_DigiTuner_GetSatInfo(MS_SAT_PARAM* pSatInfo)
//{
//    ENTRY();
//    memcpy(pSatInfo, &_stSATParam, sizeof(MS_SAT_PARAM));
//    RETURN(TRUE);
//}

MS_BOOL MApi_DigiTuner_GetCurrentLOF(MS_U8 drv_frontend_index, MS_U16* pLOF)
{
    MS_SAT_PARAM* psatParam;

    ENTRY();
    if((NULL == _pstSATParam) || (!_DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        RETURN(FALSE);

    psatParam = _pstSATParam + drv_frontend_index;
    if(bIsHiLOF)
    {
        *pLOF = psatParam->u16HiLOF;
    }
    else
    {
        *pLOF = psatParam->u16LoLOF;
    }

    RETURN(TRUE);
}

MS_BOOL MApi_DigiTuner_IsOverCurrent(MS_U8 drv_frontend_index)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    MS_BOOL bRet = TRUE;
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);


    bRet = pFETab->dishtab->IsOverCurrent(u8Idx);
    RETURN(bRet);

}


//---------------------------
// Function For Satellite
//----------------------------
MS_BOOL MApi_DigiTuner_Satellite_Set22K(MS_U8 drv_frontend_index, MS_BOOL b22KOn)
{
    MS_BOOL bRet = TRUE;
    ENTRY();
    bRet = _DigiTuner_Satellite_Set22K(drv_frontend_index, b22KOn);
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_Satellite_SetLNBPower(MS_U8 drv_frontend_index, MS_EN_LNBPWR_ONOFF enLNBPowe,MS_U8 u8Polarity,MS_BOOL bPorInvert)
{
    MS_BOOL bRet = TRUE;
    ENTRY();
    bRet = _DigiTuner_SetLNBPower(drv_frontend_index, enLNBPowe,u8Polarity,bPorInvert);
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_Satellite_Set22KTone(MS_U8 drv_frontend_index, MS_U32 u32ToneType)
{
    MS_BOOL bRet = TRUE;
    ENTRY();
    bRet = _DigiTuner_Satellite_Set22KTone(drv_frontend_index, (EN_TONEBURST_TYPE)u32ToneType);
    RETURN(bRet);
}

#ifdef SUPPORT_DUAL_LNB
MS_BOOL MApi_DigiTuner_Satellite_Set22K_Ext(MS_U8 drv_frontend_index, MS_BOOL b22KOn, EN_CABLE_SELECT eCableID)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    if(FALSE == _DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        return FALSE;

    pFETab = *(pFETabAddr + drv_frontend_index);

    ENTRY();
    bRet = pFETab->dishtab->SetCable(u8Idx, eCableID);
    bRet = _DigiTuner_Satellite_Set22K(drv_frontend_index, b22KOn);
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_Satellite_SetLNBPower_Ext(MS_U8 drv_frontend_index, MS_EN_LNBPWR_ONOFF enLNBPowe,MS_U8 u8Polarity,MS_BOOL bPorInvert, EN_CABLE_SELECT eCableID)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    if(FALSE == _DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        return FALSE;

    pFETab = *(pFETabAddr + drv_frontend_index);

    ENTRY();
    bRet = pFETab->dishtab->SetCable(u8Idx, eCableID);
    bRet = _DigiTuner_SetLNBPower(drv_frontend_index, enLNBPowe,u8Polarity,bPorInvert);
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_Satellite_Set22KTone_Ext(MS_U8 drv_frontend_index, MS_U32 u32ToneType, EN_CABLE_SELECT eCableID)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    if(FALSE == _DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        return FALSE;

    pFETab = *(pFETabAddr + drv_frontend_index);

    ENTRY();
    bRet = pFETab->dishtab->SetCable(u8Idx, eCableID);
    bRet = _DigiTuner_Satellite_Set22KTone(drv_frontend_index, (EN_TONEBURST_TYPE)u32ToneType);
    RETURN(bRet);
}
#endif
//---------------------------
// Function For Blind scan
//----------------------------
MS_BOOL MApi_DigiTuner_BlindScan_Start(MS_U8 drv_frontend_index, MS_SAT_PARAM *pSATParam,MS_U8 u8Polarity,MS_BOOL bIsHiLOF)
{
    MS_BOOL bRet = TRUE, bTunerBlindScanMode = TRUE;
    MS_U16 u16StartFreq = IF_MIN_FREQ, u16EndFreq = IF_MAX_FREQ;
    MS_U16 u16maxfreq,u16minfreq,u16TempFreq;
    DEV_FRONTEND_TYPE* pFETab;
    MS_SAT_PARAM* psatParam;

    ENTRY();
    if((NULL == _pstSATParam) || (!_DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);
    psatParam = _pstSATParam + drv_frontend_index;

    _DigiTuner_SetLNBPower(drv_frontend_index, pSATParam->eLNBPwrOnOff,u8Polarity,pSATParam->bPorInvert);
    _DigiTuner_SwitchSatellite(drv_frontend_index, pSATParam,u8Polarity,bIsHiLOF);
    *psatParam = *pSATParam;

    if(pSATParam->u16LoLOF != pSATParam->u16HiLOF) // 2LOF
    {
        if(pSATParam->u16HiLOF <MAX_C_LOF_FREQ)  // c band
        {
            if(bIsHiLOF)
            {
                u16StartFreq = IF_MIDDLE_FREQ;
                u16EndFreq = IF_MAX_FREQ;
            }
            else
            {
                u16StartFreq = IF_MIN_FREQ;
                u16EndFreq = IF_MIDDLE_FREQ;
            }
        }
        else  // Ku band
        {
            if( pSATParam->u16HiLOF>=pSATParam->u16LoLOF)
            {
                u16maxfreq = pSATParam->u16HiLOF;
                u16minfreq = pSATParam->u16LoLOF;
            }
            else
            {
                u16maxfreq = pSATParam->u16LoLOF;
                u16minfreq = pSATParam->u16HiLOF;
            }

            u16TempFreq = u16maxfreq;
            u16maxfreq = u16minfreq + IF_MAX_FREQ;
            u16minfreq =  u16TempFreq + IF_MIN_FREQ;

            if(u16maxfreq > u16minfreq)
            {
                u16minfreq = (u16maxfreq - u16minfreq) /2;
            }
            else
            {
                u16minfreq = 0;
            }

            if(bIsHiLOF)
            {
                u16StartFreq = IF_MIN_FREQ + u16minfreq;
                u16EndFreq = IF_MAX_FREQ;
                if(u16minfreq > 0)
                {
                    u16StartFreq -= IF_OFFSET_FREQ;
                }
            }
            else
            {
                u16StartFreq = u16StartFreq;
                u16EndFreq = IF_MAX_FREQ - u16minfreq;
                if(u16minfreq > 0)
                {
                    u16EndFreq += IF_OFFSET_FREQ;
                }
            }

            if( pSATParam->u16HiLOF < pSATParam->u16LoLOF)
            {
                u16minfreq = u16StartFreq;
                u16StartFreq = u16EndFreq;
                u16EndFreq = u16minfreq;
            }
        }

        _DigiTuner_Satellite_Set22K(drv_frontend_index,bIsHiLOF);
    }
    else
    {
        if(pSATParam->e22KOnOff == EN_22K_ON)
        {
            _DigiTuner_Satellite_Set22K(drv_frontend_index,TRUE);
        }
        else
        {
            _DigiTuner_Satellite_Set22K(drv_frontend_index, FALSE);
        }
    }

    bRet = pFETab->demodtab->BlindScanStart(_DigiTuner_FEIdx2DevChanIdx(drv_frontend_index),u16StartFreq,u16EndFreq);
    bRet = pFETab->tunertab->Extension_Function(_DigiTuner_FEIdx2DevChanIdx(drv_frontend_index),TUNER_EXT_FUNC_BLIND_SCAN_MODE,&bTunerBlindScanMode);
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_BlindScan_NextFreq(MS_U8 drv_frontend_index, MS_BOOL* bBlindScanEnd)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    MS_BOOL bRet = TRUE;
    DEV_FRONTEND_TYPE* pFETab;
    MS_FE_CARRIER_PARAM* ptunerParam;

    ENTRY();
    if((NULL == _ptunerParam)||(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);
    ptunerParam = _ptunerParam + drv_frontend_index;

    ptunerParam->u32Frequency = 0;// force to set tuner after blindscan

    bRet = pFETab->demodtab->BlindScanNextFreq(u8Idx,bBlindScanEnd);
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_BlindScan_WaitCurFeqFinished(MS_U8 drv_frontend_index, MS_U8* u8Progress,MS_U8* u8FindNum)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    MS_BOOL bRet = TRUE;
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    bRet = pFETab->demodtab->BlindScanWaitCurFreqFinished(u8Idx,u8Progress,u8FindNum);
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_BlindScan_GetChannel(MS_U8 drv_frontend_index, MS_U16 u16ReadStart,MS_U16* u16TPNum,MS_FE_CARRIER_PARAM *pTPTable)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    MS_BOOL bRet = TRUE;
    MS_U16 idx;
    DEMOD_MS_FE_CARRIER_PARAM TPList[MAX_TPS_LPF];
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);


    bRet = pFETab->demodtab->BlindScanGetChannel(u8Idx,u16ReadStart,u16TPNum,TPList);
    if(bRet && *u16TPNum > 0)
    {
        for(idx = 0; idx < *u16TPNum; idx ++)
        {
            pTPTable[idx].u32Frequency = TPList[idx].u32Frequency;
            pTPTable[idx].SatParam.u16SymbolRate = TPList[idx].SatParam.u32SymbolRate;
            pTPTable[idx].SatParam.u16ExtSymbolRate = TPList[idx].SatParam.u32SymbolRate>>16;
            pTPTable[idx].SatParam.bIs_DVBS2 = TPList[idx].SatParam.bIsDVBS2;
        }
    }
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_BlindScan_Cancel(MS_U8 drv_frontend_index)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    MS_BOOL bRet = TRUE, bTunerBlindScanMode = FALSE;
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);


    bRet = pFETab->demodtab->BlindScanCancel(u8Idx);
    bRet = pFETab->tunertab->Extension_Function(u8Idx,TUNER_EXT_FUNC_BLIND_SCAN_MODE,&bTunerBlindScanMode);
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_BlindScan_End(MS_U8 drv_frontend_index)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    MS_BOOL bRet = TRUE, bTunerBlindScanMode = FALSE;
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);


    bRet = pFETab->demodtab->BlindScanEnd(u8Idx);
    bRet = pFETab->tunertab->Extension_Function(u8Idx,TUNER_EXT_FUNC_BLIND_SCAN_MODE,&bTunerBlindScanMode);
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_BlindScan_GetCurrentFreq(MS_U8 drv_frontend_index, MS_U32 *u32Freq)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    MS_BOOL bRet = TRUE;
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);


    bRet = pFETab->demodtab->BlindScanGetCurrentFreq(u8Idx,u32Freq);
    RETURN(bRet);
}
//------------------------------------------------------
// DiSEqC 1.0/1.1
//------------------------------------------------------

MS_BOOL MApi_DigiTuner_DiSEqC_SendCommand(MS_U8 drv_frontend_index, MS_U8* pCmd,MS_U8 u8CmdSize)
{
    MS_BOOL bRet = TRUE;
    ENTRY();
    bRet = _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, pCmd, u8CmdSize);
    RETURN(bRet);
}

//u8Port PORT1~PORT4
//u8SubPort  H/L-LOF     V
MS_BOOL MApi_DigiTuner_DiSEqC_SwitchPort(MS_U8 drv_frontend_index, MS_U8 u8Port,MS_BOOL bUnCommited)
{

    MS_BOOL bRet = TRUE;
    MS_SAT_PARAM* pstSATParam = NULL;
    MS_FE_CARRIER_PARAM* pTunerParam = NULL;
    MS_U8 u8SubPort = 0;

    ENTRY();
    if((NULL == _pstSATParam) ||(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        RETURN(FALSE);

    pTunerParam = _ptunerParam + drv_frontend_index;
    pstSATParam = _pstSATParam + drv_frontend_index;

    if(pstSATParam->e22KOnOff == EN_22K_ON ||(pstSATParam->e22KOnOff == EN_22K_AUTO && bIsHiLOF))
    {
        _DigiTuner_Satellite_Set22K(drv_frontend_index, TRUE);
    }
    else
    {
        _DigiTuner_Satellite_Set22K(drv_frontend_index, FALSE);
    }


    if(!bUnCommited)
    {
        if(pTunerParam->SatParam.u8Polarity == 0)
            u8SubPort |= 0x02;

        if (bIsHiLOF)
            u8SubPort |= 0x01;

        switch((MS_EN_SWT_PORT)(u8Port+1))
        {
        case EN_SWT_PORT_1:
            u8SubPort = 0x00+ u8SubPort;
            break;
        case EN_SWT_PORT_2:
            u8SubPort = 0x04+ u8SubPort;
            break;
        case EN_SWT_PORT_3:
            u8SubPort = 0x08+ u8SubPort;
            break;
        case EN_SWT_PORT_4:
            u8SubPort = 0x0C+ u8SubPort;
            break;
        default:
            break;
        }
    }
    else
    {
        u8SubPort = u8Port;
    }

    bRet = _DiSEqC_SwitchPort(drv_frontend_index, u8Port,bUnCommited);

    RETURN(bRet);
}

#ifdef SUPPORT_DUAL_LNB
MS_BOOL MApi_DigiTuner_DiSEqC_SwitchPort_Ext(MS_U8 drv_frontend_index, MS_U8 u8Port,MS_BOOL bUnCommited, EN_CABLE_SELECT eCableID)
{

    MS_BOOL bRet = TRUE;
    MS_SAT_PARAM* pstSATParam = NULL;
    MS_FE_CARRIER_PARAM* pTunerParam = NULL;
    MS_U8 u8SubPort = 0;
    DEV_FRONTEND_TYPE* pFETab;

    if(FALSE == _DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        return FALSE;

    ENTRY();
    if((NULL == _pstSATParam) ||(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);
    pTunerParam = _ptunerParam + drv_frontend_index;
    pstSATParam = _pstSATParam + drv_frontend_index;
    bRet &= pFETab->dishtab->SetCable(_DigiTuner_FEIdx2DevIdx(drv_frontend_index), eCableID);

    if(pstSATParam->e22KOnOff == EN_22K_ON ||(pstSATParam->e22KOnOff == EN_22K_AUTO && bIsHiLOF))
    {
        _DigiTuner_Satellite_Set22K(drv_frontend_index, TRUE);
    }
    else
    {
        _DigiTuner_Satellite_Set22K(drv_frontend_index, FALSE);
    }


    if(!bUnCommited)
    {
        if(pTunerParam->SatParam.u8Polarity == 0)
            u8SubPort |= 0x02;

        if (bIsHiLOF)
            u8SubPort |= 0x01;

        switch((MS_EN_SWT_PORT)(u8Port+1))
        {
        case EN_SWT_PORT_1:
            u8SubPort = 0x00+ u8SubPort;
            break;
        case EN_SWT_PORT_2:
            u8SubPort = 0x04+ u8SubPort;
            break;
        case EN_SWT_PORT_3:
            u8SubPort = 0x08+ u8SubPort;
            break;
        case EN_SWT_PORT_4:
            u8SubPort = 0x0C+ u8SubPort;
            break;
        default:
            break;
        }
    }
    else
    {
        u8SubPort = u8Port;
    }

    bRet &= _DiSEqC_SwitchPort(drv_frontend_index, u8Port,bUnCommited);

    RETURN(bRet);
}
#endif

MS_BOOL MApi_DigiTuner_DiSEqC_Reset(MS_U8 drv_frontend_index)
{
    MS_U8 u8Cmd[]= {0xE0,0x31,0x00};
    MS_BOOL bRet = TRUE;

    ENTRY();
    bRet = _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, u8Cmd,sizeof(u8Cmd));
    RETURN(bRet);
}
//------------------------------------------------------
// DiSEqC 1.2
//------------------------------------------------------
MS_BOOL MApi_DigiTuner_DiSEqC_SetEastLimit(MS_U8 drv_frontend_index)
{
    MS_U8 u8Cmd[]= {0xE0,0x31,0x66};
    MS_BOOL bRet = TRUE;

    ENTRY();
    bRet = _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, u8Cmd,sizeof(u8Cmd));
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_DiSEqC_SetWestLimit(MS_U8 drv_frontend_index)
{
    MS_U8 u8Cmd[]= {0xE0,0x31,0x67};
    MS_BOOL bRet = TRUE;
    ENTRY();
    bRet = _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, u8Cmd,sizeof(u8Cmd));
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_DiSEqC_DisableLimit(MS_U8 drv_frontend_index)
{
    MS_U8 u8Cmd[]= {0xE0,0x31,0x63};
    MS_BOOL bRet = TRUE;
    ENTRY();
    bRet = _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, u8Cmd,sizeof(u8Cmd));
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_DiSEqC_GoReferencePoint(MS_U8 drv_frontend_index)
{
    MS_U8 u8Cmd[]= {0xE0,0x31,0x6B,0x00};
    MS_BOOL bRet = TRUE;
    ENTRY();
    bRet = _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, u8Cmd,sizeof(u8Cmd));
    RETURN(bRet);
}

#if   DISEQC13_ONLY_GOTOX
MS_BOOL MApi_DigiTuner_DiSEqC_GoSatPos(MS_SAT_PARAM *pSATParam)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8SatPos = pSATParam->u8Position;
    ENTRY();

    if(IsUSALS(*pSATParam))
    {
        MS_USER_SYSTEM_SETTING stSysSetting;
        MApp_GetSysSetting(&stSysSetting);
        _DiSEqC_GotoX(pSATParam->u16Angle,SetSATLocation[stSysSetting.u8Location].MyLongitude,SetSATLocation[stSysSetting.u8Location].MyLatitude);
    }
    else
    {
        u8SatPos &= DISEQC12_USALS_POSITION_MASK;
        bRet =_DiSEqC_GotoSatPosition(u8SatPos);
    }
    RETURN(bRet);
}
#else
MS_BOOL MApi_DigiTuner_DiSEqC_GoSatPos(MS_U8 drv_frontend_index, MS_U8 u8SatPos)
{
    MS_BOOL bRet = TRUE;
    ENTRY();

    u8SatPos &= DISEQC12_USALS_POSITION_MASK;
    bRet =_DiSEqC_GotoSatPosition(drv_frontend_index, u8SatPos);

    RETURN(bRet);
}
#endif

MS_BOOL MApi_DigiTuner_DiSEqC_SaveSatPos(MS_U8 drv_frontend_index, MS_U8 u8SatPos)
{
    MS_U8 u8Cmd[]= {0xE0,0x31,0x6A,u8SatPos};
    MS_BOOL bRet = TRUE;
    ENTRY();
    u8SatPos &= DISEQC12_USALS_POSITION_MASK;
    u8Cmd[3] = u8SatPos;
    bRet = _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, u8Cmd,sizeof(u8Cmd));
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_DiSEqC_MotorConinuteEast(MS_U8 drv_frontend_index)
{
    MS_U8 u8Cmd[]= {0xE0,0x31,0x68,0x00};
    MS_BOOL bRet = TRUE;
    ENTRY();
    bRet = _DigiTuner_DiSEqC_SendCmd(drv_frontend_index,u8Cmd,sizeof(u8Cmd));
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_DiSEqC_MotorConinuteWest(MS_U8 drv_frontend_index)
{
    MS_U8 u8Cmd[]= {0xE0,0x31,0x69,0x00};
    MS_BOOL bRet = TRUE;
    ENTRY();
    bRet = _DigiTuner_DiSEqC_SendCmd(drv_frontend_index,u8Cmd,sizeof(u8Cmd));
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_DiSEqC_MotorStepEast(MS_U8 drv_frontend_index, MS_U8 u8Step)
{
    MS_U8 u8Cmd[]= {0xE0,0x31,0x68,0xFE};
    MS_BOOL bRet = TRUE;
    ENTRY();
    u8Cmd[3] = 0xFF - u8Step;
    bRet = _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, u8Cmd,sizeof(u8Cmd));
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_DiSEqC_MotorStepWest(MS_U8 drv_frontend_index, MS_U8 u8Step)
{
    MS_U8 u8Cmd[]= {0xE0,0x31,0x69,0xFE};
    MS_BOOL bRet = TRUE;
    ENTRY();
    u8Cmd[3] = 0xFF - u8Step;
    bRet = _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, u8Cmd,sizeof(u8Cmd));
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_DiSEqC_HaltMotor(MS_U8 drv_frontend_index)
{
    MS_U8 u8Cmd[]= {0xE0,0x31,0x60};
    MS_BOOL bRet = TRUE;
    ENTRY();
    bRet = _DigiTuner_DiSEqC_SendCmd(drv_frontend_index, u8Cmd,sizeof(u8Cmd));
    RETURN(bRet);
}
//------------------------------------------------------
// DiSEqC USALS
//------------------------------------------------------
MS_BOOL MApi_DigiTuner_DiSEqC_GotoX(MS_U8 drv_frontend_index, MS_U16 u16SatLongitude,MS_S16 s16MyLongitude,MS_S16 s16MyLatitude)
{
    MS_BOOL bRet = TRUE;
    ENTRY();

    bRet = _DiSEqC_GotoX(drv_frontend_index, u16SatLongitude,s16MyLongitude,s16MyLatitude);

    RETURN(bRet);
}


MS_BOOL MApi_DigiTuner_IsNeedCheckCurrent(void)
{
#if 0
    MS_SAT_PARAM stSatParam;
    MApi_DB_SAT_GetSATParam(_tunerParam.SatParam.u8SatID,&stSatParam);
    if(stSatParam.eLNBPwrOnOff == EN_LNBPWR_OFF)
    {
        return FALSE;
    }
#endif
    return TRUE;
}
#endif
MS_BOOL MApi_DigiTuner_Frontend_List_Show(void)
{
    MS_U8 u8FE_Idx = 0, u8I2C_Port_Number = 0;
    DEV_FRONTEND_TYPE* pFETab;
    MS_BOOL bret = TRUE;
    HWI2C_PORT hwi2c_port;
    MS_BOOL bIsMCP = FALSE;
    const char* cMCP="(MCP)";
    const char* cEXT="(EXT)";
    char* cDMD_NAME = (char *)MsOS_AllocateMemory(20, gs32FECachedPoolID);

    if((!FE_Detect_Done) && (eDetect_Mode != USER_DEFINE))
    {
        if(!FE_Load_Reset_Pin)
        {
            MApi_Frontend_LoadResetPin();
            FE_Load_Reset_Pin = TRUE;
        }
        bret &= MApi_DigiTuner_Frontend_Scan();
    }

    FE_MSG(("===================================================================================================\n"));
    while(_DigiTuner_CheckFEtab_Exist(u8FE_Idx))
    {
        pFETab = *(pFETabAddr + u8FE_Idx);
        hwi2c_port = getI2CPort(_DigiTuner_FEIdx2DevIdx(u8FE_Idx));
        if (hwi2c_port < E_HWI2C_PORT_1)
        {
            u8I2C_Port_Number = 0;
        }
        else if (hwi2c_port < E_HWI2C_PORT_2)
        {
            u8I2C_Port_Number = 1;
        }

        strcpy(cDMD_NAME, pFETab->demodtab->name);
        if((pFETab->demodtab->data == DEMOD_MSB1245) || ((pFETab->demodtab->data == DEMOD_MSB124X)))
        {
            pFETab->demodtab->Extension_Function(_DigiTuner_FEIdx2DevIdx(u8FE_Idx), DEMOD_EXT_FUNC_GET_PACKGE_INFO, &bIsMCP);
            if(bIsMCP)
            {
                cDMD_NAME = strcat((char*)cDMD_NAME, cMCP);
            }
            else
            {
                cDMD_NAME = strcat((char*)cDMD_NAME, cEXT);
            }
        }

        FE_MSG(("[FrontEnd_Index %d]  Demod: %-25sTuner: %-25sI2C_Port:I2C%d\n", (int)u8FE_Idx,(char*)cDMD_NAME, pFETab->tunertab->name,(int)u8I2C_Port_Number));
        u8FE_Idx++;
    }
    if(!u8FE_Idx)
        FE_MSG(("NO Frontend !!\n"));
    FE_MSG(("===================================================================================================\n"));
    MsOS_FreeMemory(cDMD_NAME, gs32FECachedPoolID);
    return bret;

}

MS_BOOL MApi_DigiTuner_Frontend_Scan(void)
{
    MS_U8 u8Port_Idx = 0,u8FE_Idx = 0,j;
    MS_U32 u32ch_cnt = 0;
    HWI2C_PORT hwi2c_port;
    DEV_FRONTEND_TYPE* p1stFEtab = NULL;
    DEV_FRONTEND_TYPE* pPreFEtab = NULL;
    DEV_FRONTEND_TYPE* pCurFEtab = NULL;

    if(INVALID_POOL_ID == gs32FECachedPoolID)
    {
        FE_ERR(("INVALID_POOL_ID !!!\n"));
        return FALSE;
    }

    pu16ExtDMD_Info = (EXT_DMD_INFO*)MsOS_AllocateMemory(sizeof(EXT_DMD_INFO) * MAX_FRONTEND_NUM, gs32FECachedPoolID);
    if(NULL == pu16ExtDMD_Info)
        return FALSE;
    else
        memset(pu16ExtDMD_Info, 0x00, sizeof(EXT_DMD_INFO)*MAX_FRONTEND_NUM);

    for(u8Port_Idx=0; u8Port_Idx < MAX_FRONTEND_NUM; u8Port_Idx++)
    {

        hwi2c_port = getI2CPort(u8Port_Idx);
        MDrv_IIC_Init(hwi2c_port);
        MApi_Demod_HWReset(u8Port_Idx);

        pCurFEtab = (DEV_FRONTEND_TYPE*)MsOS_AllocateMemory(sizeof(DEV_FRONTEND_TYPE), gs32FECachedPoolID);
        if(NULL == pCurFEtab)
            return FALSE;
        else
            pCurFEtab->next = NULL;

        pCurFEtab->dvb_type = DefaultBroadcastType;
        // demod detect
        if(u8Port_Idx != 0)
            u8FE_Idx = u32ch_acc[u8Port_Idx - 1];

        MApi_Demod_lookup(u8Port_Idx,pCurFEtab);
        if(pCurFEtab->demodtab->data == DEMOD_NULL)
        {
            MsOS_FreeMemory(pCurFEtab, gs32FECachedPoolID);
            pCurFEtab = NULL;
            if(u8Port_Idx !=0)
                u32ch_acc[u8Port_Idx] = u32ch_acc[u8Port_Idx - 1];
            continue;
        }

        pCurFEtab->bReset = FALSE;//will do HW reset before its initialze and set this flag as TRUE
        MApi_Tuner_lookup(u8Port_Idx, pCurFEtab,&u32ch_acc[u8Port_Idx]);//FALSE ==> no tuner is found
        if(pCurFEtab->tunertab->data == TUNER_NULL)
        {
            MsOS_FreeMemory(pCurFEtab, gs32FECachedPoolID);
            pCurFEtab = NULL;
            if(u8Port_Idx != 0)
                u32ch_acc[u8Port_Idx] = u32ch_acc[u8Port_Idx - 1];
            continue;
        }


        if(u8Port_Idx != 0)
        {
            u32ch_acc[u8Port_Idx] += u32ch_acc[u8Port_Idx - 1];
        }

        //Link to previous FETab
        if(NULL == p1stFEtab)
            p1stFEtab = pCurFEtab;
        else
            pPreFEtab->next = pCurFEtab;

        // set DMX input path
        if(FALSE == MApi_Frontend_InExteralPath_Arrange(pCurFEtab->eInExTernalPath, pCurFEtab))
            return FALSE;

        pPreFEtab = pCurFEtab;

        //Wide band tuner duplicate frontend table
        for(u8FE_Idx++ ; u8FE_Idx < u32ch_acc[u8Port_Idx] ; u8FE_Idx++)
        {
            pCurFEtab = (DEV_FRONTEND_TYPE*)MsOS_AllocateMemory(sizeof(DEV_FRONTEND_TYPE), gs32FECachedPoolID);
            if(NULL == pCurFEtab)
                return FALSE;
            memcpy(pCurFEtab, pPreFEtab, sizeof(DEV_FRONTEND_TYPE));
            pCurFEtab->next = NULL;//copy all except 'next'
            // set DMX input path
            if(FALSE == MApi_Frontend_InExteralPath_Arrange(pCurFEtab->eInExTernalPath, pCurFEtab))
            {
                FE_ERR(("Warning: Set default DMX path ERROR!!!\n"));
            }
            pPreFEtab->next = pCurFEtab;
            pPreFEtab = pCurFEtab;
        }
    }

    MsOS_FreeMemory(pu16ExtDMD_Info, gs32FECachedPoolID);
    pu16ExtDMD_Info = NULL;

    if(_DigiTuner_MaxFEIdx() == FEIndex_NONE)//no FE found
        return FALSE;
    else
        u32ch_cnt = _DigiTuner_MaxFEIdx() + 1;

    _pu8Init = (MS_U8*)MsOS_AllocateMemory(sizeof(MS_U8)*u32ch_cnt, gs32FECachedPoolID);
    pFETabAddr = (DEV_FRONTEND_TYPE**)MsOS_AllocateMemory(sizeof(MS_U32)*u32ch_cnt, gs32FECachedPoolID);
    _ptunerParam = (MS_FE_CARRIER_PARAM*)MsOS_AllocateMemory(sizeof(MS_FE_CARRIER_PARAM)*u32ch_cnt,gs32FECachedPoolID);
#if MS_DVBS_INUSE
    _pstSATParam = (MS_SAT_PARAM*)MsOS_AllocateMemory(sizeof(MS_SAT_PARAM)*u32ch_cnt,gs32FECachedPoolID);
#endif

#if MS_DVBS_INUSE
    if((NULL == _pu8Init) || (NULL == pFETabAddr) || (NULL == _ptunerParam) ||(NULL == _pstSATParam))
#else
    if((NULL == _pu8Init) || (NULL == pFETabAddr) || (NULL == _ptunerParam))
#endif
    {
        _DigiTuner_ReleaseFETab();
        return FALSE;
    }

    //record FETab
    pCurFEtab = p1stFEtab;
    j=0;
    while(pCurFEtab != NULL)
    {
        *(pFETabAddr + j) = pCurFEtab;
        j++;
        pCurFEtab = pCurFEtab->next;
    }

    for(u8FE_Idx = 0; u8FE_Idx < u32ch_cnt; u8FE_Idx++)
    {
        *(_pu8Init + u8FE_Idx) = 0;
        memset(_ptunerParam + u8FE_Idx, 0x00, sizeof(MS_FE_CARRIER_PARAM));
        pCurFEtab = *(pFETabAddr + u8FE_Idx);
    }

    FE_Detect_Done = TRUE;

    return TRUE;
}

MS_BOOL MApi_DigiTuner_GetFEtab(MS_U8 drv_frontend_index, DEV_FRONTEND_TYPE** pFETab)
{
    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);
    *pFETab = *(pFETabAddr + drv_frontend_index);
    RETURN(TRUE);
}

MS_BOOL MApi_DigiTuner_Set_MemPool(E_DDI_FE_SYSPOOLID eSYSPOOL,MS_S32 s32PoolID)
{
    if (eSYSPOOL == E_DDI_FE_POOL_SYS_CACHE)
    {
        gs32FECachedPoolID = s32PoolID;
    }
    else if (eSYSPOOL == E_DDI_FE_POOL_SYS_NONCACHE)
    {
        gs32FENonCachedPoolID = s32PoolID;
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

MS_BOOL MApi_DigiTuner_SetPowerState(EN_POWER_MODE emod)
{
    MS_U8 u8FEIdx, u8FEIdxMax = _DigiTuner_MaxFEIdx(), u8Port_Idx;
    DEV_FRONTEND_TYPE* pFETab = NULL;
    MS_BOOL bret = TRUE;

    switch(emod)
    {
    case E_POWER_SUSPEND:
        if(FEIndex_NONE == u8FEIdxMax)
            break;

        for(u8FEIdx = 0; u8FEIdx <= u8FEIdxMax; u8FEIdx++)
        {
            bret &= MApi_DigiTuner_DeInit(u8FEIdx);
            memset(_ptunerParam + u8FEIdx, 0x00, sizeof(MS_FE_CARRIER_PARAM));
            pFETab = *(pFETabAddr + u8FEIdx);
            pFETab->bReset = FALSE;
        }

        if(_s32MutexId!=-1)
        {
            MsOS_DeleteMutex(_s32MutexId);
            _s32MutexId=-1;
        }
        break;
    case  E_POWER_RESUME:
        for(u8Port_Idx=0; u8Port_Idx < MAX_FRONTEND_NUM; u8Port_Idx++)
        {
            MApi_Demod_HWReset(u8Port_Idx);
        }
        break;
    default:
        break;
    }
    return bret;
}

MS_BOOL MApi_DigiTuner_GetTunerParam(MS_U8 drv_frontend_index,MS_FE_CARRIER_PARAM* pParam)
{
    MS_FE_CARRIER_PARAM* pCurParam = NULL;
    DEV_FRONTEND_TYPE* pFETab = NULL;
    MS_BOOL bret = TRUE;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);

    if((NULL == _ptunerParam)||(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
    {
        FE_DBG(("Get Tuner Parameter pointer FAIL!!\n"));
        return FALSE;
    }

    ENTRY();

    pCurParam = _ptunerParam + drv_frontend_index;
    pFETab = *(pFETabAddr + drv_frontend_index);
    pFETab->demodtab->GetParam(u8Idx, (DEMOD_MS_FE_CARRIER_PARAM*)pCurParam);

    memcpy(pParam, pCurParam, sizeof(MS_FE_CARRIER_PARAM));
    RETURN(bret);
}

MS_BOOL MApi_DigiTuner_SetISDBTLayer(MS_U8 drv_frontend_index,DEMOD_ISDBT_Layer eISDBT_Layer)
{
    DEV_FRONTEND_TYPE* pFETab = NULL;
    MS_BOOL bret = TRUE;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEMOD_ISDBT_Layer eLayer;

    if((NULL == _ptunerParam)||(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
    {
        FE_DBG(("Get Tuner Parameter pointer FAIL!!\n"));
        return FALSE;
    }

    ENTRY();

    pFETab = *(pFETabAddr + drv_frontend_index);
    eLayer = eISDBT_Layer;

    if(pFETab->dvb_type == ISDBT)
    {
        bret = pFETab->demodtab->Extension_Function(u8Idx, DEMOD_EXT_FUNC_SET_ISDBT_LAYER, &eLayer);
    }
    else
    {
        FE_DBG(("%s Broadcast type is Not ISDBT!!\n", __FUNCTION__));
        bret = FALSE;
    }

    RETURN(bret);
}


MS_BOOL MApi_DigiTuner_FEIdx_to_DevIdx(MS_U8 drv_frontend_index, MS_U8* pu8DevIdx)
{
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
    {
        return FALSE;
    }

    *pu8DevIdx = _DigiTuner_FEIdx2DevIdx(drv_frontend_index);
    return TRUE;
}

MS_BOOL MApi_DigiTuner_SetDMD_PIDFilter(MS_U8 drv_frontend_index, DEMOD_PID_FILTER* pstPIDFlt)
{
    MS_BOOL bret;
    DEV_FRONTEND_TYPE* pFETab = NULL;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);

    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
    {
        return FALSE;
    }

    ENTRY();

    pFETab = *(pFETabAddr + drv_frontend_index);

    bret = pFETab->demodtab->Extension_Function(u8Idx, DEMOD_EXT_FUNC_SET_PID_FILTER, pstPIDFlt);

    RETURN(bret);
}


#ifdef DDI_MISC_INUSE
#ifdef FE_AUTO_TEST
//-------------------------------------------------------------------------------------------------
/// Get Demod Read Reg
/// @param eFE                      \b IN: Frontend id
/// @return TRUE:  Success
/// @return FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DigiTuner_ReadReg(MS_U8 drv_frontend_index, MS_U16 RegAddr, MS_U8 *pu8Data)
{
    MS_BOOL bRet = FALSE;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    if((pFETab->demodtab == NULL) || (pFETab->tunertab == NULL) )
    {
        RETURN(FALSE);
    }

    bRet = pFETab->demodtab->ReadReg(u8Idx,RegAddr,pu8Data);
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_WriteReg(MS_U8 drv_frontend_index, MS_U16 RegAddr, MS_U16 RegData)
{
    MS_BOOL bRet = FALSE;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    if((pFETab->demodtab == NULL) || (pFETab->tunertab == NULL) )
    {
        RETURN(FALSE);
    }

    bRet = pFETab->demodtab->WriteReg(u8Idx,RegAddr,RegData);
    RETURN(bRet);
}


MS_BOOL MApi_DigiTuner_Reset(MS_U8 drv_frontend_index, DEMOD_MS_FE_CARRIER_PARAM* pParam)
{
    DEMOD_MS_FE_CARRIER_PARAM    dmdParam;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;
    MS_U32 u32DemodType;

    ENTRY();
#if(MS_DVBS_INUSE == 1)
    if((NULL == _ptunerParam) || (NULL == _pstSATParam) || (!_DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
#else
    if((NULL == _ptunerParam)||(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
#endif
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);

    dmdParam.u32Frequency = pParam->u32Frequency;
    dmdParam.SatParam.u32SymbolRate = pParam->SatParam.u32SymbolRate;

    u32DemodType = pFETab->demodtab->data;
    if(u32DemodType == DEMOD_MSKELTIC_DVBS || (u32DemodType == DEMOD_MSKRIS_DVBS) || (u32DemodType == DEMOD_MSKRATOS_DVBS))
    {
        if (pFETab->tunertab->SetFreqS2(u8Idx, pParam->u32Frequency, pParam->SatParam.u32SymbolRate) == FALSE)
        {
            FE_ERR(("%s: Set frequency failed.\n", __FUNCTION__));
            RETURN(FALSE);
        }
    }

    if (pFETab->demodtab->Restart(u8Idx,&dmdParam,pFETab->dvb_type) == FALSE)
    {
        FE_ERR(("%s: COfdm DMD restart failed.\n", __FUNCTION__));
        RETURN(FALSE);
    }

    RETURN(TRUE);
}

#endif
#endif

#if MS_DVBT2_INUSE
void MApi_DigiTuner_CtrlResetDJBFlag(MS_U8 drv_frontend_index, MS_U8 Ctrl)
{
    //MDrv_Demod_CtrlResetDJBFlag(Ctrl);
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        return;

    pFETab = *(pFETabAddr + drv_frontend_index);
    pFETab->demodtab->Extension_Function(u8Idx, DEMOD_EXT_FUNC_CTRL_RESET_DJB_FLAG, &Ctrl);
}

MS_BOOL MApi_DigiTuner_T2MI_Restart(MS_U8 drv_frontend_index)
{
    //MDrv_Demod_T2MI_Restart();
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);
    pFETab->demodtab->Extension_Function(u8Idx, DEMOD_EXT_FUNC_T2MI_RESTART, NULL);
    RETURN(TRUE);
}

MS_BOOL MApi_DigiTuner_InitParameter(MS_U8 drv_frontend_index)
{
    //return MDrv_Demod_InitParameter();
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);
    pFETab->demodtab->Extension_Function(u8Idx, DEMOD_EXT_FUNC_INIT_PARAMETER, NULL);
    RETURN(TRUE);
}

MS_U8 MApi_DigiTuner_GetPlpIDList(MS_U8 drv_frontend_index)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;
    MS_U8 data=0;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(0);

    pFETab = *(pFETabAddr + drv_frontend_index);
    pFETab->demodtab->Extension_Function(u8Idx, DEMOD_EXT_FUNC_GET_PLPID_LIST, &data);
    RETURN(data);
}

MS_U8 MApi_DigiTuner_GetNextPlpID(MS_U8 drv_frontend_index,MS_U8 u8Index)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;
    MS_U8 data=0;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(0);

    pFETab = *(pFETabAddr + drv_frontend_index);
    pFETab->demodtab->GetNextPLPID(u8Idx,u8Index,&data);
    RETURN(data);
}

MS_BOOL MApi_DigiTuner_SetScanTypeStatus(MS_U8 drv_frontend_index,MS_U8 status)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(FALSE);

    pFETab = *(pFETabAddr + drv_frontend_index);
    pFETab->demodtab->SetScanTypeStatus(u8Idx,status);
    RETURN(TRUE);

}

MS_U8 MApi_DigiTuner_GetScanTypeStatus(MS_U8 drv_frontend_index)
{
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;
    MS_U8 status=0;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(0xff);

    pFETab = *(pFETabAddr + drv_frontend_index);
    status = pFETab->demodtab->GetScanTypeStatus(u8Idx);
    RETURN(status);
}

MS_BOOL MApi_DigiTuner_SetT2Reset(MS_U8 drv_frontend_index)
{
    MS_BOOL bRet = FALSE;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(bRet);

    pFETab = *(pFETabAddr + drv_frontend_index);
    bRet = pFETab->demodtab->Extension_Function(u8Idx, DEMOD_EXT_FUNC_T2_RESET, NULL);
    RETURN(bRet);
}

MS_BOOL MApi_DigiTuner_SetT2Restart(MS_U8 drv_frontend_index)
{
    MS_BOOL bRet = FALSE;
    MS_U8 u8Idx = _DigiTuner_FEIdx2DevChanIdx(drv_frontend_index);
    DEV_FRONTEND_TYPE* pFETab;

    ENTRY();
    if(!_DigiTuner_CheckFEtab_Exist(drv_frontend_index))
        RETURN(bRet);

    pFETab = *(pFETabAddr + drv_frontend_index);
    bRet = pFETab->demodtab->Extension_Function(u8Idx, DEMOD_EXT_FUNC_T2_RESTART, NULL);
    RETURN(bRet);
}

MS_U8 MApi_DigiTuner_Get_Current_Plp_Id(void)
{
    return T2PlpID;
}

#endif //#if MS_DVBT2_INUSE

#ifdef DDI_MISC_INUSE
#if (DDI_MISC_INUSE==1)

#if (BOOT_TYPE == BOOT_TYPE_FAST)
void MApi_DigiTuner_Wakeup(MS_U8 drv_frontend_index)
{
#if(MS_DVBS_INUSE == 1)
    if((NULL == _ptunerParam) || (NULL == _pstSATParam) || (!_DigiTuner_CheckFEtab_Exist(drv_frontend_index)))
        return;

    MS_SAT_PARAM * psatParam = _pstSATParam + drv_frontend_index;
#endif

    MS_U32 u32frontendtype;

    MApi_Demod_HWReset(_DigiTuner_FEIdx2DevIdx(drv_frontend_index));
    MS_FE_CARRIER_PARAM* pParam = _ptunerParam + drv_frontend_index;
    MApi_DigiTuner_Init(drv_frontend_index);

    u32frontendtype = MApi_DigiTuner_GetFrontendType(drv_frontend_index);
#if(MS_DVBS_INUSE == 1)
    if (u32frontendtype == DVBS || u32frontendtype == DVBS2)
    {
        MApi_DigiTuner_Tune2RfCh_DVBS(drv_frontend_index, psatParam, pParam, FE_TUNE_AUTO);
    }
    else
#endif
    {
        MApi_DigiTuner_Tune2RfCh(drv_frontend_index,pParam, FE_TUNE_AUTO);
    }

}
#endif

#endif //#if DDI_MISC_INUSE
#endif //#ifdef DDI_MISC_INUSE

