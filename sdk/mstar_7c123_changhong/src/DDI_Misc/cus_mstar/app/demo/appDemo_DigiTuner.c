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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   appDemo_DigiTuner.c
/// @author MStar Semiconductor Inc.
/// @brief A sample code for illustrating how to control frontend
///////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include "MsCommon.h"
#include "Board.h"
#include "MsOS.h"
#include "apiDigiTuner.h"
#include "appDemo_DigiTuner.h"
#include "drvDTC.h"

#define NON_EXTERN_SAT_INFO
#include "satConstInfo.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
#define CHSCAN_TASK_DELAY           30 // ms
#define X4CFE_ENABLE_MODE       TRUE
#define PPD_ENABLE_MODE         FALSE
#define IQ_AUTO_ENABLE_MODE     TRUE
#define QAM_AUTO_ENABLE_MODE    FALSE
#define FHO_ENABLE_MODE         FALSE
#define DMD_TUNER_FREQ          FALSE

#if(MS_DVBS_INUSE == 1)
#define MAX_TPSOFSAT_NUM   3000 //max physical channel of one satellite
#endif

typedef enum _DVBC_QamType1
{
    E_DVBC_QAM16  = 0,                                                  ///< QAM 16
    E_DVBC_QAM32  = 1,                                                  ///< QAM 32
    E_DVBC_QAM64  = 2,                                                  ///< QAM 64
    E_DVBC_QAM128 = 3,                                                  ///< QAM 128
    E_DVBC_QAM256 = 4,                                                  ///< QAM 256

    E_DVBC_QAM_LASTNUM,
} __attribute__((packed)) DVBC_QAMType1;

//--------------------------
typedef enum _DVBC_IQType1
{
    E_DVBC_IQ_NORMAL = 0,                                               ///< IQ Normal
    E_DVBC_IQ_INVERT = 1,                                               ///< IQ Invert
} __attribute__((packed)) DVBC_IQType1;

//--------------------------
typedef enum _DVBC_TapAssign1
{
    E_DVBC_TAP_PRE  = 0,                                                ///< PreTap assign
    E_DVBC_TAP_POST = 1,                                                ///< PostTap assign
} __attribute__((packed)) DVBC_TapAssign1;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define DVBS2_VCM_ISID_NULL 0xFFFF
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
//extern DEV_FRONTEND_TYPE frontendtab[MAX_FRONTEND_NUM];
//DVBS/S2
static char*  cConStr_S[] = {"QPSK", "8PSK", "16APSK", "16APSK", "32APSK", "8APSK", "8+8APSK", "4+8+4+16APSK"};
static char*  cCRStr[] = {"1/2", "1/3", "2/3", "1/4", "3/4", "2/5","3/5", \
                   "4/5", "5/6", "7/8", "8/9", "9/10", "2/9", "13/45",\
                   "9/20", "90/180", "96/180", "11/20", "100/180", "104/180",\
                   "26/45","18/30","28/45","23/36","116/180", "20/30", "124/180",\
                   "13/18", "132/180", "22/30", "135/180", "140/180", \
                   "7/9", "154/180", "11/45", "4/15", "14/45", "7/15",\
                   "8/15", "26/45", "32/45"};
static char*  cRollOffStr[] = {"0.35", "0.25", "0.20"};
static char*  cIQMode[] = {"Normal", "Inverse"};
//DVBT/T2
static char*  cConStr_T[] = {"QPSK", "16QAM", "64QAM", "256QAM"};
static char*  cFFTStr[] = {"2k", "8k", "4k", "1k", "16k", "32k", "8k", "32k"};
static char*  cHierarchy[]= {"Non-hierarchy", "Hierarchy alpha = 1", "Hierarchy alpha = 2", "Hierarchy alpha = 4"};
static char*  cGI[]= {"1/32", "1/16", "1/8", "1/4", "1/128", "19/128", "19/256"};
//DVBC
static char*  cConStr_C[] = {"16QAM", "32QAM", "64QAM", "128QAM", "256QAM"};
//ISDBT
static char*  cConStr_ISDBT[] = {"DQPSK", "QPSK", "16QAM", "64QAM", "N/A"};
static char*  cCRStr_ISDBT[] = {"1/2", "2/3", "3/4", "5/6", "7/8", "N/A"};
static char*  cGI_ISDBT[]= {"1/4", "1/8", "1/16", "1/32", "N/A"};
static char*  cFFTStr_ISDBT[] = {"2k", "4k", "8k", "N/A"};
static char*  cTDIStr_ISDBT[] = {"0", "4", "8", "16", "0", "2", "4", "8", "0", "1", "2", "4", "N/A"};
//J83B
static char*  cConStr_J83B[] = {"VSB", "64QAM", "256QAM", "16QAM", "32QAM", "128QAM"};

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U8 u8DVBS2_VCM_ISID_TBL[32];
static MS_U16 u16DVBS2_VCM_ISID = DVBS2_VCM_ISID_NULL;
static MS_U8 gu8TunerIndex = 0;

static MS_U32 _u32NowTime = 0;
#if (MS_DVBS_INUSE == 1)
static MS_EN_LNB_POLARITY eLNBPolarity[2] = {EN_LNB_POL_H,EN_LNB_POL_H};
static EN_CABLE_SELECT geCableSel = EN_CABLE_LNB_NOT_SET;
static MS_SAT_PARAM gsat_param[2] = {
                    {
                    .e0V12VOnOff = EN_0V12V_ONOFF_ON,
                    .e22KOnOff = EN_22K_AUTO,
                    .eDiseqcLevel = EN_DISEQC_LVL_OFF,
                    .eLNBPwrOnOff = EN_LNBPWR_13OR18V,
                    .eSwt10Port = EN_SWT_PORT_OFF,
                    .eSwt11Port = EN_SWT_PORT_OFF,
                    .eToneburstType = EN_TONEBURST_NONE,
                    .u16Angle = 0,
                    .u16HiLOF = 10600,
                    .u16LoLOF = 9750,
                    .u8Position = 0,
                    .bPorInvert = FALSE,
                    .bIsHiLOF = FALSE,
                    },
                    {
                    .e0V12VOnOff = EN_0V12V_ONOFF_ON,
                    .e22KOnOff = EN_22K_AUTO,
                    .eDiseqcLevel = EN_DISEQC_LVL_OFF,
                    .eLNBPwrOnOff = EN_LNBPWR_13OR18V,
                    .eSwt10Port = EN_SWT_PORT_OFF,
                    .eSwt11Port = EN_SWT_PORT_OFF,
                    .eToneburstType = EN_TONEBURST_NONE,
                    .u16Angle = 0,
                    .u16HiLOF = 10600,
                    .u16LoLOF = 9750,
                    .u8Position = 0,
                    .bPorInvert = FALSE,
                    .bIsHiLOF = FALSE,
                    }
                    };
#endif

//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------

static void _demod_callback(MS_U8 u8FEIndex,MS_U8 u8arg)
{
  DEMOD_EVENT_TYPE eEvtType;

  eEvtType = (DEMOD_EVENT_TYPE)(u8arg & 0x0f);

  printf("[DMD Event][Frontend_Index %d] ", (int)u8FEIndex);
  switch(eEvtType)
  {
     case DEMOD_EVT_LOCK:
        printf("Lock!! SYS Time = %d\n", (int)(MsOS_GetSystemTime()));
        break;
     case DEMOD_EVT_UNLOCK:
        printf("Unlock !!! SYS TIme = %d\n", (int)(MsOS_GetSystemTime()));
        break;
     case DEMOD_EVT_UNKNOWN:
     default:
        printf("Unknown event\n");
        break;
  }
}

static MS_BOOL _Check_ATSC_SyncLock(MS_U8 u8TunerIndex, EN_LOCK_STATUS* pStatus)
{
    MS_U8 i;

    MApi_DigiTuner_GetPreLock(u8TunerIndex, pStatus);
     for( i=0; i<3; ++i )
    {
         MsOS_DelayTask(100);
        if(MApi_DigiTuner_GetPreLock(u8TunerIndex, pStatus) )
        {
          if( E_DEMOD_LOCK == *pStatus)
              break;
        }
         //else
         //    return FALSE;
     }
     if(E_DEMOD_LOCK != *pStatus) return FALSE;

     if(E_DEMOD_LOCK == *pStatus)
     {
         *pStatus = E_DEMOD_CHECKING;
         for( i=0; i<10; ++i )
         {
             MsOS_DelayTask(100);
             if(MApi_DigiTuner_GetFlameSyncLock(u8TunerIndex, pStatus) )
             {
                 if( E_DEMOD_LOCK == *pStatus)
                     break;
             }
             //else
             //   return FALSE;
         }
         if(E_DEMOD_LOCK != *pStatus) return FALSE;
     }

    return TRUE;
}

static MS_BOOL _Tuner_TryLock(void)
{
    MS_U8 u8TunerIndex = 0;
    MS_U32 u32frontendtype = 0;
    EN_LOCK_STATUS eDemodstatus = E_DEMOD_CHECKING;

    u8TunerIndex = gu8TunerIndex;
    u32frontendtype = MApi_DigiTuner_GetFrontendType(u8TunerIndex);

    if((u32frontendtype == ATSC) && (_Check_ATSC_SyncLock(u8TunerIndex,&eDemodstatus)))
    {
       if( E_DEMOD_LOCK != eDemodstatus) //CHANNEL EMPTY
        {
            printf("ATSC channel is empty\n");
            return FALSE;
        }
    }

    eDemodstatus = E_DEMOD_CHECKING;
    //MS_U32 _u32NowTime = MsOS_GetSystemTime();
    MS_U32 _u32LockTimeMax = 0;
    MApi_DigiTuner_GetMaxLockTime(u8TunerIndex, FE_TUNE_MANUAL, &_u32LockTimeMax);
    _u32NowTime = MsOS_GetSystemTime();

    while ((MsOS_GetSystemTime() - _u32NowTime) < _u32LockTimeMax)
    {
        //printf(".");
        if((MApi_DigiTuner_GetLock(u8TunerIndex,&eDemodstatus)== TRUE))
        {
            if( E_DEMOD_LOCK == eDemodstatus)
            {
                //printf("== time = %d (ms)\n",(int)(MsOS_GetSystemTime() - _u32NowTime));
                u16DVBS2_VCM_ISID = DVBS2_VCM_ISID_NULL;
                return TRUE;//found a lock channel
            }
        }
        MsOS_DelayTask(CHSCAN_TASK_DELAY);
    }

    return FALSE;
}

//#if(MS_DVBS_INUSE == 1)
//static MS_BOOL _DigiTuner_SetLNBPower(MS_EN_LNBPWR_ONOFF eLNBPower)
//{
    //gsat_param.eLNBPwrOnOff = eLNBPower;
//    MApi_DigiTuner_Satellite_SetLNBPower(gu8TunerIndex, eLNBPower,0,0);
 //   return TRUE;
//}
//#endif

//------------------------------------------------------------------------------
/// @brief The sample code to select which Tuner should be served now.
/// @param[in] TunerIndex select index of tuner to be served.
/// @return TRUE: Success
/// @sa
/// @note
/// Command: \b DigiTuner_SetIndex \n
///
/// Sample Command: \n \b DigiTuner_SetIndex 0 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_DigiTuner_SetIndex(MS_U32 *TunerIndex)
{
    MS_U32 u32MaxFEIdx = 0;

    u32MaxFEIdx = MApi_DigiTuner_GetMaxFEIdx();

    //if(FEIndex_NONE == u32MaxFEIdx)
    //{
    //    return FALSE;
    //}

    if ( (*TunerIndex) <= u32MaxFEIdx )
    {
        gu8TunerIndex =(MS_U8)(*TunerIndex);
        printf("Set gu8TunerIndex=%d\n",gu8TunerIndex);
        return TRUE;
    }
    else
    {
        printf("Input index=%"DTC_MS_U32_d" is larger than the board limitation:%d\n", *TunerIndex,(int)(u32MaxFEIdx-1));
        return FALSE;
    }
}

//------------------------------------------------------------------------------
/// @brief The sample code to get Tuner Initialial Status
/// @param[in] TunerIndex select index of tuner to get init status.
/// @return TRUE: Init already
/// @return FALSE: not init
/// @sa
/// @note
/// Command: \b DigiTuner_GetIndex \n
///
/// Sample Command: \n \b DigiTuner_GetTunerStatus 0 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_DigiTuner_GetTunerStatus(MS_U32 *TunerIndex)
{
    return MApi_DigiTuner_Get_FE_InitStatus(*TunerIndex);
}

//------------------------------------------------------------------------------
/// @brief The sample code to get Tuner Index
/// @return Tuner Index
/// @sa
/// @note
/// Command: \b DigiTuner_GetIndex \n
//------------------------------------------------------------------------------
MS_U8 appDemo_DigiTuner_GetIndex(void)
{
    return gu8TunerIndex;
}

//------------------------------------------------------------------------------
/// @brief The sample code to check if frontend locked or not
/// @return TRUE: Frontend locked
/// @return FALSE: Frontend not lock
/// @sa
/// @note
/// Command: \b DigiTuner_GetLock \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_DigiTuner_CheckLock(void)
{
    MS_U16 u16SNR = 0;
    MS_U8 TunerIndex = 0;
    float fltBER = 0;
    MS_BOOL _bIsLock = FALSE;
    //MS_BOOL bLockStatus;
    MS_U32 _u32NowTime = MsOS_GetSystemTime();
    MS_U32 _u32LockTimeMax = 0;
    EN_LOCK_STATUS eDemodstatus = E_DEMOD_CHECKING;

    TunerIndex = gu8TunerIndex;

    MApi_DigiTuner_GetMaxLockTime((MS_U8)(TunerIndex),FE_TUNE_MANUAL, &_u32LockTimeMax);
    while ((MsOS_GetSystemTime() - _u32NowTime) < _u32LockTimeMax)
    {
        MApi_DigiTuner_GetSNR((MS_U8)(TunerIndex),&u16SNR);
        MApi_DigiTuner_GetBER((MS_U8)(TunerIndex),&fltBER);
        printf("[%s][%d] Signal quality %d %f\n",
            __FILE__, __LINE__,
            u16SNR, fltBER);
        if((MApi_DigiTuner_GetLock(TunerIndex,&eDemodstatus)== TRUE) && (E_DEMOD_LOCK == eDemodstatus))
        {
            printf("[%s][%d] Lock\n",
                __FILE__, __LINE__);
            _bIsLock = TRUE;
            break;//found a lock channel
        }
        MsOS_DelayTask(CHSCAN_TASK_DELAY);
    }
    return _bIsLock;
}

MS_BOOL appDemo_DigiTuner_SetFreqCheck(const MS_U32 *Freq, const MS_U32 *u32Arg1, const MS_U32 *u32Arg2)
{
    MS_U8  TunerIndex = 0;
    MS_U32 u32FrontendType = 0;

    TunerIndex = gu8TunerIndex;

    if(!MApi_DigiTuner_Get_FE_InitStatus(TunerIndex))
    {
        MApi_DigiTuner_SetDMDCallBack(TunerIndex, _demod_callback);
        if(!appDemo_Frontend_Init((MS_U32*)&TunerIndex))
            return FALSE;
    }

    u32FrontendType = MApi_DigiTuner_GetFrontendType(TunerIndex);

    if(u32FrontendType == DVBC)
    {
        if (u32Arg2 != NULL)
        {
            if ((*Freq == 0) /*|| (*u32Arg1 > 4) || (*u32Arg2 == 0)*/)
            {
                printf("[error]Invalid arguments, DigiTuner_SetFreq [Frequency] [constellation => {0: QAM16, 1:QAM32, 2:QAM64, 3:QAM128, 4:QAM256}] [Symbol rate]\n");
                return FALSE;
            }
        }
    }
    else if(u32FrontendType == J83B)
    {
        if (*Freq == 0)
        {
            printf("[error]Invalid arguments, DigiTuner_SetFreq [Frequency]\n");
            return FALSE;
        }

    }
    else
    {
        if ((*Freq == 0) || (*u32Arg1 == 0))
        {
            printf("[error]Invalid arguments, DigiTuner_SetFreq [frequency] [bandwidth or symbol rate]\n");
            return FALSE;
        }
    }

#if(MS_DVBT2_INUSE == 1)
    if((DVBT == u32FrontendType) && (E_DEMOD_TYPE_T != MApi_DigiTuner_GetCurrentDemodType(TunerIndex)))
    {
        if(!MApi_DigiTuner_SetCurrentDemodType(TunerIndex,E_DEMOD_TYPE_T))
            return FALSE;
    }
    else if ((DVBT2 == u32FrontendType) && (E_DEMOD_TYPE_T2 != MApi_DigiTuner_GetCurrentDemodType(TunerIndex)))
    {
        if(!MApi_DigiTuner_SetCurrentDemodType(TunerIndex,E_DEMOD_TYPE_T2))
            return FALSE;
    }
#endif

    if (MApi_DigiTuner_PowerOnOff((MS_U8)(TunerIndex), TRUE) == FALSE)
    {
        printf("[error]%s: Tuner power on failed.\n", __FUNCTION__);
        return FALSE;
    }

    return TRUE;
}

MS_BOOL appDemo_DigiTuner_Tune2RF(MS_FE_CARRIER_PARAM* pstParam)
{
    MS_U8 u8TunerIndex = 0;
    MS_U32 u32frontendtype = 0;

    u8TunerIndex = gu8TunerIndex;
    u32frontendtype = MApi_DigiTuner_GetFrontendType(u8TunerIndex);

#if(MS_DVBS_INUSE == 1)
    if(u32frontendtype == DVBS || u32frontendtype == DVBS2)
    {
#ifdef SUPPORT_DUAL_LNB
        if((EN_CABLE_LNB_NOT_SET == geCableSel) || (EN_CABLE_LNB_NUM <= geCableSel))
            MApi_DigiTuner_Tune2RfCh_DVBS(u8TunerIndex, &gsat_param[0], pstParam, FE_TUNE_MANUAL);
        else
            MApi_DigiTuner_Tune2RfCh_DVBS_Ext(u8TunerIndex, &gsat_param[geCableSel-1], pstParam, geCableSel);
#else
       MApi_DigiTuner_Tune2RfCh_DVBS(u8TunerIndex, &gsat_param[0], pstParam, FE_TUNE_MANUAL);
#endif
    }
    else
#endif
    {
        MApi_DigiTuner_Tune2RfCh(u8TunerIndex, pstParam, FE_TUNE_MANUAL);
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to set tuner frequency and bandwidth
/// @param[in] Freq Frequency (kHz)
/// @param[in] u32Arg1 for DVB-T and DVB-T2 means {1, 2, 3} => {6MHz, 7MHz 8MHz}; \n
///                    for DVB-C means {0, 1, 2, 3, 4} => {QAM16, QAM32, QAM64, QAM128, QAM256}; \n
///                    for DVB-S2 means symbol rate;
/// @param[in] u32Arg2 for DVB-C means symbol rate in Ksyms
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DigiTuner_SetFreq 258000 3 (DVB-T, DVB-T2)\n
///          \b DigiTuner_SetFreq 3940000 2 6875 (DVB-C)\n
///          \b DigiTuner_SetFreq 11428 27500 (DVB-S2)\n
//------------------------------------------------------------------------------
MS_BOOL appDemo_DigiTuner_SetFreq(const MS_U32 *Freq, const MS_U32 *u32Arg1, const MS_U32 *u32Arg2)
{
    if(!appDemo_DigiTuner_SetFreqCheck(Freq, u32Arg1, u32Arg2))
        return FALSE;

    MS_U8 u8TunerIndex = 0;
    MS_U32 u32frontendtype = 0;
    MS_FE_CARRIER_PARAM stParam;

    u8TunerIndex = gu8TunerIndex;
    u32frontendtype = MApi_DigiTuner_GetFrontendType(u8TunerIndex);
    stParam.u32Frequency = *Freq;

    if(u32frontendtype == DVBC)
    {
        stParam.CabParam.u16SymbolRate = (long)*u32Arg2;
        stParam.CabParam.eConstellation = (EN_CAB_CONSTEL_TYPE)(*u32Arg1);
        stParam.CabParam.eIQMode = CAB_IQ_NORMAL;
        stParam.CabParam.eBandWidth = CAB_BW_8M;
    }
#if(MS_DVBS_INUSE == 1)
    else if(u32frontendtype == DVBS || u32frontendtype == DVBS2)
    {
        stParam.SatParam.u16SymbolRate = *u32Arg1;
        stParam.SatParam.u16ExtSymbolRate = *u32Arg1 >> 16;
        if(((EN_CABLE_SELECT)(*u32Arg2) < EN_CABLE_LNB_NUM) && ((EN_CABLE_SELECT)(*u32Arg2) > EN_CABLE_LNB_NOT_SET))
        {
            geCableSel = (EN_CABLE_SELECT)(*u32Arg2);
            stParam.SatParam.u8Polarity = (MS_U8)(eLNBPolarity[geCableSel-1]);
        }
        else
        {
            geCableSel = EN_CABLE_LNB_NOT_SET;
            stParam.SatParam.u8Polarity = (MS_U8)(eLNBPolarity[0]);
        }
        //DVBS2 VCM mode
        if(u16DVBS2_VCM_ISID == DVBS2_VCM_ISID_NULL)
        {
            stParam.SatParam.eVCM_MODE = SAT_VCM_AUTO_MODE;
            stParam.SatParam.u8VCM_IS_ID = 0;
        }
        else
        {
            stParam.SatParam.eVCM_MODE = SAT_VCM_FORCED_MODE;
            stParam.SatParam.u8VCM_IS_ID = (MS_U8)u16DVBS2_VCM_ISID;
        }

    }
#endif
    else if(u32frontendtype == ISDBT)
    {
        stParam.ISDBTParam.eBandWidth = (EN_TER_BW_MODE)(*u32Arg1 - 1);
    }
    else if(u32frontendtype == J83B)
    {
        stParam.J83BParam.eBandWidth = CAB_BW_6M;
    }
    else
    {
        stParam.TerParam.eBandWidth = (EN_TER_BW_MODE)(*u32Arg1 - 1);
        stParam.TerParam.eLevelSel = TER_HP_SEL;
        stParam.TerParam.u8PlpID = 0;
    }

    if(!appDemo_DigiTuner_Tune2RF(&stParam))
    {
        printf("[%s][%d] appDemo_DigiTuner_Tune2RF failed.\n", __FILE__, __LINE__);
    }

    printf("[%s][%d] (Freq) = (%"DTC_MS_U32_d") Try Lock\n", __FILE__, __LINE__, *Freq);
    if(_Tuner_TryLock())
        printf("\n[%s][%d] (Freq) = (%"DTC_MS_U32_d") Lock\n", __FILE__, __LINE__, *Freq);

    return TRUE;
}

#if(MS_DVBT2_INUSE == 1)
//------------------------------------------------------------------------------
/// @brief The sample code to get/print DVBT2 Plp related information
/// @return none.
/// @sa
/// @note
/// Command: \b DigiTuner_GetPlpInfo \n
//------------------------------------------------------------------------------
void appDemo_DigiTuner_GetPlpInfo(void)
{
    MS_U8 u8PlpBitMap[32];
    MS_U8 u8GroupID = 0;
    MS_U8 j,k;
    MS_BOOL bRet;
    MS_U8 TunerIndex = 0;

    TunerIndex = gu8TunerIndex;

    memset(u8PlpBitMap, 0, sizeof(u8PlpBitMap));
    MApi_DigiTuner_GetPlpBitMap((MS_U8)(TunerIndex), u8PlpBitMap);
    printf("===== Plp Info====\n");
    for(k = 0; k < 32; k++)
    {
        for(j = 0; j < 8; j++)
        {
            if((u8PlpBitMap[k] >> j) & 1)
            {
                bRet = MApi_DigiTuner_GetPlpGroupID((MS_U8)(TunerIndex),k * 8 + j,&u8GroupID);
                if(bRet == TRUE)
                {
                    printf("PlpID = 0x%x, PlpGroupID = 0x%x\n",k * 8 + j,u8GroupID);
                }
            }
        }
    }
    printf("===== Plp Info end====\n");
}



//------------------------------------------------------------------------------
/// @brief The sample code to Set DVBT2 MPLP filter
/// @param[in] u32PlpID Plp ID
/// @param[in] u32GroupID Group ID
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DigiTuner_SetMPLP \n
///
/// Sample Command: \b DigiTuner_SetMPLP 0 0 \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_DigiTuner_SetMPLP(const MS_U32 *u32PlpID, const MS_U32 *u32GroupID)
{
    MS_U8 u8PlpID;
    MS_U8 u8GroupID;
    MS_U8 TunerIndex = 0;
    EN_LOCK_STATUS eDemodstatus = E_DEMOD_CHECKING;

    u8PlpID = (MS_U8)(*u32PlpID);
    u8GroupID = (MS_U8)(*u32GroupID);

    TunerIndex = gu8TunerIndex;

    printf("[%s][%d] (plpID, u8GroupID) = (%"DTC_MS_U32_u" %"DTC_MS_U32_u") Lock\n",__FILE__, __LINE__, *u32PlpID, *u32GroupID);
    MApi_DigiTuner_SetPlpGroupID((MS_U8)(TunerIndex), u8PlpID,u8GroupID);
    MsOS_DelayTask(100);
    MS_BOOL _bIsLock = FALSE;
    MS_U32 _u32NowTime = MsOS_GetSystemTime();
    MS_U32 _u32LockTimeMax = 0;
    MApi_DigiTuner_GetMaxLockTime((MS_U8)(TunerIndex), FE_TUNE_MANUAL, &_u32LockTimeMax);
    while ((MsOS_GetSystemTime() - _u32NowTime) < _u32LockTimeMax)
    {
        if((MApi_DigiTuner_GetLock((MS_U8)(TunerIndex),&eDemodstatus)== TRUE) && (E_DEMOD_LOCK == eDemodstatus))
        {
            _bIsLock = TRUE;
            printf("[%s][%d] (plpID, u8GroupID) = (%"DTC_MS_U32_u" %"DTC_MS_U32_u") Lock\n",__FILE__, __LINE__, *u32PlpID, *u32GroupID);
            break;//found a lock channel
        }
        MsOS_DelayTask(CHSCAN_TASK_DELAY);
    }
    return _bIsLock;
}

#endif

#if(MS_DVBS_INUSE == 1)
MS_BOOL appDemo_DigiTuner_GetSatParam(MS_U32 CableID, MS_SAT_PARAM** ppstSatParam)
{
    MS_U8 u8CableID=0;

    if(((EN_CABLE_SELECT)(CableID) < EN_CABLE_LNB_NUM) && ((EN_CABLE_SELECT)(CableID) > EN_CABLE_LNB_NOT_SET))
    {
        u8CableID = (MS_U8)(CableID-1);
    }

    if(u8CableID>2)
        return FALSE;
    else
    {
        *ppstSatParam = (&gsat_param[u8CableID]);
        return TRUE;
    }
}

MS_BOOL appDemo_DigiTuner_SetLNBLOF(const MS_U32 *HiLOF, const MS_U32 *LoLOF, const MS_U32 *CableID)
{
    MS_U8 u8CableID=0;

    if(((EN_CABLE_SELECT)(*CableID) < EN_CABLE_LNB_NUM) && ((EN_CABLE_SELECT)(*CableID) > EN_CABLE_LNB_NOT_SET))
    {
        u8CableID = (MS_U8)((*CableID)-1);
    }

    if(u8CableID>2)
        return FALSE;

    if((MS_U16)(*LoLOF) == 0)
    {
        gsat_param[u8CableID].u16HiLOF = (MS_U16)(*HiLOF);
        gsat_param[u8CableID].u16LoLOF = (MS_U16)(*HiLOF);
    }
    else
    {
        if((MS_U16)(*LoLOF) < (MS_U16)(*HiLOF))
        {
            gsat_param[u8CableID].u16HiLOF = (MS_U16)(*HiLOF);
            gsat_param[u8CableID].u16LoLOF = (MS_U16)(*LoLOF);
        }
        else
        {
            gsat_param[u8CableID].u16HiLOF = (MS_U16)(*LoLOF);
            gsat_param[u8CableID].u16LoLOF = (MS_U16)(*HiLOF);
        }
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set LNB power to 13V, 18V or 0V
/// @param[in] LNBPower 0 is for 0V, 2 is for 13V, 3 is for 18V
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b DigiTuner_SetLNBPower \n
///
/// Sample Command: \b DigiTuner_SetLNBPower 2
//------------------------------------------------------------------------------
MS_BOOL appDemo_DigiTuner_SetLNBPower(const MS_U32 *LNBPower, const MS_U32 *CableID)
{
    MS_EN_LNBPWR_ONOFF eLNBPower=EN_LNBPWR_OFF;
    MS_BOOL bPorInvert = FALSE;
    MS_U8 u8CableID=0;

    eLNBPower = (MS_EN_LNBPWR_ONOFF)(*LNBPower);

    if(((EN_CABLE_SELECT)(*CableID) < EN_CABLE_LNB_NUM) && ((EN_CABLE_SELECT)(*CableID) > EN_CABLE_LNB_NOT_SET))
    {
        u8CableID = (MS_U8)((*CableID)-1);
    }


    bPorInvert = gsat_param[u8CableID].bPorInvert;
    if((!bPorInvert && (eLNBPower == EN_LNBPWR_13V)) || (bPorInvert && (eLNBPower == EN_LNBPWR_18V)))
    {
        eLNBPolarity[u8CableID] = EN_LNB_POL_V;
    }
    else
    {
        eLNBPolarity[u8CableID] = EN_LNB_POL_H;
    }
#ifdef SUPPORT_DUAL_LNB
    EN_CABLE_SELECT eCableSel;

    eCableSel = (EN_CABLE_SELECT)(*CableID);

    if(EN_CABLE_LNB_NOT_SET == eCableSel)
        MApi_DigiTuner_Satellite_SetLNBPower(gu8TunerIndex, eLNBPower,0,0);
    else
        MApi_DigiTuner_Satellite_SetLNBPower_Ext(gu8TunerIndex, eLNBPower,0,0, eCableSel);
#else
     MApi_DigiTuner_Satellite_SetLNBPower(gu8TunerIndex, eLNBPower,0,0);
#endif

    return TRUE;
}

MS_BOOL appDemo_DigiTuner_Satellite_Set22K(const MS_U32 * b22KOn, const MS_U32 *CableID)
{
    MS_U8 u8CableID=0;
#ifdef SUPPORT_DUAL_LNB
    EN_CABLE_SELECT eCableSel;

    eCableSel = (EN_CABLE_SELECT)(*CableID);

    if(EN_CABLE_LNB_NOT_SET == eCableSel)
        MApi_DigiTuner_Satellite_Set22K(gu8TunerIndex,*b22KOn);
    else
        MApi_DigiTuner_Satellite_Set22K_Ext(gu8TunerIndex,*b22KOn, eCableSel);
#else
    MApi_DigiTuner_Satellite_Set22K(gu8TunerIndex,*b22KOn);
#endif
    if(((EN_CABLE_SELECT)(*CableID) < EN_CABLE_LNB_NUM) && ((EN_CABLE_SELECT)(*CableID) > EN_CABLE_LNB_NOT_SET))
    {
        u8CableID = (MS_U8)((*CableID)-1);
    }

    gsat_param[u8CableID].e22KOnOff = (MS_EN_22K_ONOFF)(*b22KOn);

    return TRUE;
}


MS_BOOL appDemo_DigiTuner_Satellite_Set22KTone(const MS_U32* ToneType, const MS_U32 *CableID)
{
    MS_U8 u8CableID=0;
#ifdef SUPPORT_DUAL_LNB
    EN_CABLE_SELECT eCableSel;

    eCableSel = (EN_CABLE_SELECT)(*CableID);

    if(EN_CABLE_LNB_NOT_SET == eCableSel)
        MApi_DigiTuner_Satellite_Set22KTone(gu8TunerIndex,*ToneType);
    else
        MApi_DigiTuner_Satellite_Set22KTone_Ext(gu8TunerIndex,*ToneType, eCableSel);
#else
     MApi_DigiTuner_Satellite_Set22KTone(gu8TunerIndex,*ToneType);
#endif
    if(((EN_CABLE_SELECT)(*CableID) < EN_CABLE_LNB_NUM) && ((EN_CABLE_SELECT)(*CableID) > EN_CABLE_LNB_NOT_SET))
    {
        u8CableID = (MS_U8)((*CableID)-1);
    }

    gsat_param[u8CableID].eToneburstType = (MS_EN_TONEBURST_TYPE)(*ToneType + 1);

    return TRUE;
}

MS_BOOL appDemo_FE_DiseqcCmd(const MS_U32 *u32CmdType,const MS_U32 *u32Arg0,const MS_U32 *u32Arg1,const MS_U32 *u32Arg2, const MS_U32 *CableID)
{
#ifdef SUPPORT_DUAL_LNB
    EN_CABLE_SELECT eCableSel;
    eCableSel = (EN_CABLE_SELECT)(*CableID);
#endif

    printf("FE_DiseqcCmd format => FE_DiseqcCmd u32CmdType u32Arg0 u32Arg1 u32Arg2 \n");
    printf("u32CmdType=0 -> DiSEqC_SwitchPort(drv_frontend_index(u32Arg0),u8Port(u32Arg1),bUnCommited(u32Arg2))\n");
    printf("u32CmdType=1 -> DiSEqC_Reset(drv_frontend_index(u32Arg0))\n");
    printf("u32CmdType=2 -> DiSEqC_MotorConinuteEast(drv_frontend_index(u32Arg0))\n");
    printf("u32CmdType=3 -> DiSEqC_MotorConinuteWest(drv_frontend_index(u32Arg0))\n");
    printf("u32CmdType=4 -> DiSEqC_HaltMotor(drv_frontend_index(u32Arg0))\n");

    switch(*u32CmdType)
    {
        case 0:
 #ifdef SUPPORT_DUAL_LNB
            if(EN_CABLE_LNB_NOT_SET == eCableSel)
                MApi_DigiTuner_DiSEqC_SwitchPort(*u32Arg0,*u32Arg1,*u32Arg2);
            else
                MApi_DigiTuner_DiSEqC_SwitchPort_Ext(*u32Arg0,*u32Arg1,*u32Arg2, eCableSel);
 #else
            MApi_DigiTuner_DiSEqC_SwitchPort(*u32Arg0,*u32Arg1,*u32Arg2);
 #endif

            printf("DiSEqC SwitchPort drv_frontend_index:0x%"DTC_MS_U32_x", u8Port:0x%"DTC_MS_U32_x", bUnCommited: 0x%"DTC_MS_U32_x"\n",
                *u32Arg0, *u32Arg1, *u32Arg2);
            break;
        case 1:
            MApi_DigiTuner_DiSEqC_Reset(*u32Arg0);
            printf("DiSEqC_Reset drv_frontend_index:0x%"DTC_MS_U32_x"\n", *u32Arg0);
            printf("The diseqc cmd should be {0xE0,0x31,0x00}\n");
            break;
        case 2:
            MApi_DigiTuner_DiSEqC_MotorConinuteEast(*u32Arg0);
            printf("DiSEqC_MotorConinuteEast drv_frontend_index:0x%"DTC_MS_U32_x"\n", *u32Arg0);
            printf("The diseqc cmd should be {0xE0,0x31,0x68,0x00}\n");
            break;
        case 3:
            MApi_DigiTuner_DiSEqC_MotorConinuteWest(*u32Arg0);
            printf("DiSEqC_MotorConinuteWest drv_frontend_index:0x%"DTC_MS_U32_x"\n", *u32Arg0);
            printf("The diseqc cmd should be {0xE0,0x31,0x69,0x00}\n");
            break;
        case 4:
            MApi_DigiTuner_DiSEqC_HaltMotor(*u32Arg0);
            printf("DiSEqC_HaltMotor drv_frontend_index:0x%"DTC_MS_U32_x"\n", *u32Arg0);
            printf("The diseqc cmd should be {0xE0,0x31,0x60}\n");
            break;
        default:
            printf("Unknown cmd type\n");
            break;
    }
    return TRUE;
}
#endif

MS_BOOL appDemo_DigiTuner_InitTuner (void)
{
#if((MS_DVB_TYPE_SEL == DVBC) || (MS_DVB_TYPE_SEL == J83B))
    MApi_DigiTuner_Init(0);
    MApi_DigiTuner_Loop_Through_On(0,TRUE);
#elif MS_DVBS_INUSE
    MApi_DigiTuner_Init(0);
    MApi_DigiTuner_Loop_Through_On(0,TRUE);
    //_DigiTuner_LNBConnect(*Freq, *u32Arg1);
#else
    if(!MApi_DigiTuner_Get_FE_InitStatus(0))
    {
        MApi_DigiTuner_Init(0);
        MApi_DigiTuner_Loop_Through_On(0,TRUE);
    }
#endif

#if(MS_DVB_TYPE_SEL != DVBT2)

    printf("before MApi_DigiTuner_PowerOnOff \n");
    if (MApi_DigiTuner_PowerOnOff(0,TRUE) == FALSE)
    {
        printf("%s: Tuner power on failed.\n", __FUNCTION__);

        return FALSE;
    }
#endif

    return TRUE;
}

MS_BOOL appDemo_Frontend_SetDetectMode(const MS_U32 *u32DetectMode)
{
    return MApi_Frontend_SetDetectMode(gu8TunerIndex,*(EN_FRONTEND_DETECT_MODE*)u32DetectMode);
}

MS_BOOL appDemo_Frontend_SetBroadcastType(const MS_U32 *u32BroadcastType)
{
    printf("=========================== Command Format ==========================\n");
    printf("[FE_SetBroadcastType] [arg0]\n");
    printf("[arg0]: Broadcast type number\n");
    printf("=========================== End Command Format ======================\n");
    return MApi_Frontend_SetBroadcastType(gu8TunerIndex,*u32BroadcastType);
}

MS_BOOL appDemo_Frontend_SetInExteralPath(const MS_U32 *u32InExTernalPath)
{
    DEV_FRONTEND_TYPE* pFETab = NULL;
    if(MApi_DigiTuner_GetFEtab(gu8TunerIndex, &pFETab))
        return MApi_Frontend_SetInExteralPath(gu8TunerIndex,*(EN_IN_EX_TERNAL_PATH*)u32InExTernalPath, pFETab);
    else
        return FALSE;
}

MS_BOOL appDemo_Frontend_SetTunerDemod(const MS_U8 *u8IICPortIdx,const MS_U32 *u32TunerIndex, const MS_U32 *u32DemodIndex)
{
    MS_BOOL bret = TRUE;
    printf("=========================== Command Format ==========================\n");
    printf("[FE_SetFE] [arg0] [arg1] [arg2]\n");
    printf("[arg0]: I2C Port index \n");
    printf("[arg1]: Tuner index \n");
    printf("[arg2]: Demod index \n");
    printf("=========================== End Command Format ======================\n");
    bret = MApi_Frontend_SetTunerDemod(*u8IICPortIdx,*u32TunerIndex, *u32DemodIndex);
    return bret;
}

MS_BOOL appDemo_Frontend_Init(const MS_U32 *pu32TunerIndex)
{
    if(!MApi_DigiTuner_Init((MS_U8)(*pu32TunerIndex)) )
         return FALSE;
    // MApi_DigiTuner_Loop_Through_On((MS_U8)(*pu32TunerIndex), TRUE);

    return TRUE;
}

MS_BOOL appDemo_Frontend_HWReset(const MS_U32 *u32TunerIndex)
{
   MApi_Demod_HWReset((MS_U8)(*u32TunerIndex));
   printf("!!!! FE Reset %x\n",(MS_U8)(*u32TunerIndex));
   return TRUE;
}

MS_BOOL appDemo_Frontend_FEListShow(void)
{
   return MApi_DigiTuner_Frontend_List_Show();
}

MS_BOOL appDemo_Frontend_SetDMD_PIDFlt(const MS_U32 *pu32PID, const MS_U32 *pu32Drop, const MS_U32 *pu32PIDRemap)
{
   MS_FE_PID_FILTER stPIDFlt;

   stPIDFlt.u16PIDValue = (MS_U16)(*pu32PID);
   stPIDFlt.u16PIDRemapValue= (MS_U16)(*pu32PIDRemap);
   stPIDFlt.bPIDDrop= (MS_BOOL)(*pu32Drop);

   return MApi_DigiTuner_SetDMD_PIDFilter(gu8TunerIndex, &stPIDFlt);
}

//------------------------------------------------------------------------------
/// @brief The sample code to get/print DVBS2 VCM related information
/// @return none.
/// @sa
/// @note
/// Command: \b DigiTuner_GetPlpInfo \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_DigiTuner_GetVCM_ISID_Info(void)
{
    MS_U8 TunerIndex = 0;
    MS_U8 u8CurISID = 0;
    MS_U8 i, j, u8bitmap;

    TunerIndex = gu8TunerIndex;

    if(MApi_DigiTuner_GetVCM_ISID_INFO(TunerIndex, &u8CurISID, u8DVBS2_VCM_ISID_TBL))
    {
        printf("===== DVBS2 VCM ISID Info====\n");
        for(i = 0; i < 32; i++)
        {
          u8bitmap = u8DVBS2_VCM_ISID_TBL[i];
          //printf("u8DVBS2_VCM_ISID_TBL[i] = %x\n", u8bitmap);
          for(j = 0; j < 8; j++)
          {
             if((u8bitmap >> j) & 0x01)
             {
                printf("ISID = 0x%x \n", i*8 + j);
             }
          }
          printf("===== DVBS2 VCM ISID Info end====\n");
       }
       return TRUE;
    }
    else
    {
       return FALSE;
    }
}

MS_BOOL appDemo_DigiTuner_SetVCM_ISID(const MS_U32 *u32ISID, const MS_U32 *IsNextCH)
{

    MS_U8 TunerIndex = 0;
    MS_U8 u8ISID = 0;
    EN_LOCK_STATUS eDemodstatus;
    MS_BOOL bSetForNextFreq;

    TunerIndex = gu8TunerIndex;
    u8ISID = *(MS_U8*)u32ISID;
    bSetForNextFreq = *(MS_BOOL*)IsNextCH;

    if(bSetForNextFreq)
    {
       u16DVBS2_VCM_ISID = *(MS_U16*)u32ISID; // will be set at next zapping
    }
    else
    {
        if((MApi_DigiTuner_GetLock(TunerIndex,&eDemodstatus)== TRUE))
        {
            if( E_DEMOD_LOCK == eDemodstatus)
            {
                MApi_DigiTuner_SetVCM_ISID(TunerIndex, u8ISID);
            }
        }
        else
        {
           printf("Demod Unlock\n");
           return FALSE;
        }
    }

    return TRUE;
}

MS_BOOL appDemo_DigiTuner_GetParam(const MS_U32 *u32Param)
{
    MS_U8 TunerIndex = 0,i;
    MS_BOOL bRet;
    MS_FE_CARRIER_PARAM stParam;
    MS_U32 u32BroadcastType;


    TunerIndex = gu8TunerIndex;
    if(MApi_Frontend_GetBroadcastType(TunerIndex, &u32BroadcastType))
    {
        bRet = MApi_DigiTuner_GetTPSInfo(TunerIndex,&stParam);
        if((u32BroadcastType == DVBS) || (u32BroadcastType == DVBS2))
        {
            if(stParam.SatParam.bIs_DVBS2)
            {
                if(stParam.SatParam.eConstellation<(sizeof(cConStr_S)/sizeof(char*)))
                {
                    printf("Constellation: %s\n", cConStr_S[stParam.SatParam.eConstellation]);
                }
                else
                {
                    printf("Constellation: Unknown\n");
                }

                if(stParam.SatParam.eRollOff<(sizeof(cRollOffStr)/sizeof(char*)))
                {
                    printf("Roll-Off     : %s\n", cRollOffStr[stParam.SatParam.eRollOff]);
                }
                else
                {
                    printf("Roll-Off     : Unknown\n");
                }

                if(stParam.SatParam.eIQ_Mode<(sizeof(cIQMode)/sizeof(char*)))
                {
                    printf("Spectrum     : %s\n", cIQMode[stParam.SatParam.eIQ_Mode]);
                }
                else
                {
                    printf("Spectrum     : Unknown\n");
                }
            }


            if(stParam.SatParam.eCodeRate<(sizeof(cCRStr)/sizeof(char*)))
            {
                printf("Code Rate    : %s\n", cCRStr[stParam.SatParam.eCodeRate]);
            }
            else
            {
                printf("Code Rate    : Unknown\n");
            }

            printf("CFO          : %lf KHz\n", stParam.SatParam.fCFO);
        }
        else if((u32BroadcastType == DVBT) || (u32BroadcastType == DVBT2))
        {
            if(stParam.TerParam.eConstellation<(sizeof(cConStr_T)/sizeof(char*)))
            {
                printf("Constellation: %s\n", cConStr_T[stParam.TerParam.eConstellation]);
            }
            else
            {
                printf("Constellation: Unknown\n");
            }

            if(stParam.TerParam.eFFT_Mode<(sizeof(cFFTStr)/sizeof(char*)))
            {
                printf("FFT size: %s\n", cFFTStr[stParam.TerParam.eFFT_Mode]);
            }
            else
            {
                printf("FFT size: Unknown\n");
            }

            if(stParam.TerParam.eHPCodeRate<(sizeof(cCRStr)/sizeof(char*)))
            {
                printf("HP Code Rate    : %s\n", cCRStr[stParam.TerParam.eHPCodeRate]);
            }
            else
            {
                printf("HP Code Rate    : Unknown\n");
            }

            if(stParam.TerParam.eGuardInterval<(sizeof(cGI)/sizeof(char*)))
            {
                printf("Guard Interval  : %s\n", cGI[stParam.TerParam.eGuardInterval]);
            }
            else
            {
                printf("Guard Interval  : Unknown\n");
            }

            if(u32BroadcastType == DVBT)
            {
                if(stParam.TerParam.eHierarchy<(sizeof(cHierarchy)/sizeof(char*)))
                {
                    printf("Hierarchy: %s\n", cHierarchy[stParam.TerParam.eHierarchy]);
                }
                else
                {
                    printf("Hierarchy: Unknown\n");
                }

                if(stParam.TerParam.eHierarchy != TER_HIE_NONE)
                {
                    if(stParam.TerParam.eLPCodeRate<(sizeof(cCRStr)/sizeof(char*)))
                    {
                        printf("LP Code Rate    : %s\n", cCRStr[stParam.TerParam.eLPCodeRate]);
                    }
                    else
                    {
                        printf("LP Code Rate    : Unknown\n");
                    }
                }
            }
        }
        else if(u32BroadcastType == DVBC)
        {
            if(stParam.CabParam.eConstellation<(sizeof(cConStr_C)/sizeof(char*)))
            {
                printf("Constellation: %s\n", cConStr_C[stParam.CabParam.eConstellation]);
            }
            else
            {
                printf("Constellation: Unknown\n");
            }

            printf("SR           : %d kHz\n", (int)stParam.CabParam.u16SymbolRate);
            printf("CFO          : %lf KHz\n", stParam.CabParam.fCFO);
        }
        else if(u32BroadcastType == J83B)
        {
            if(stParam.J83BParam.eJ83BConstellation<(sizeof(cConStr_J83B)/sizeof(char*)))
            {
                printf("Constellation: %s\n", cConStr_J83B[stParam.J83BParam.eJ83BConstellation]);
            }
            else
            {
                printf("Constellation: Unknown\n");
            }
        }
        else if(u32BroadcastType == ISDBT)
        {
            for(i=0;i<3;i++)
            {
                if(i==0)
                    printf("\nLayer A: ");
                else if(i == 1)
                    printf("\nLayer B: ");
                else if(i == 2)
                    printf("\nLayer C: ");

                if(stParam.ISDBTParam.stLayerX_Param[i].eIsdbtConstellation<(sizeof(cConStr_ISDBT)/sizeof(char*)))
                {
                    printf("  Constellation: %-5s",\
                    cConStr_ISDBT[stParam.ISDBTParam.stLayerX_Param[i].eIsdbtConstellation]);
                }
                else
                {
                    printf("  Constellation: Unknown");
                }

                if(stParam.ISDBTParam.stLayerX_Param[i].eIsdbtCodeRate<(sizeof(cCRStr_ISDBT)/sizeof(char*)))
                {
                    printf("  Code Rate: %-5s",\
                    cCRStr_ISDBT[stParam.ISDBTParam.stLayerX_Param[i].eIsdbtCodeRate]);
                }
                else
                {
                    printf("  Code Rate: Unknown");
                }

                if(stParam.ISDBTParam.stLayerX_Param[i].eIsdbtGI<(sizeof(cGI_ISDBT)/sizeof(char*)))
                {
                    printf("  Guard Interval: %-5s",\
                    cGI_ISDBT[stParam.ISDBTParam.stLayerX_Param[i].eIsdbtGI]);
                }
                else
                {
                    printf("  Guard Interval: Unknown");
                }

                if(stParam.ISDBTParam.stLayerX_Param[i].eIsdbtTDI<(sizeof(cTDIStr_ISDBT)/sizeof(char*)))
                {
                    printf("  TDI: %-5s",\
                    cTDIStr_ISDBT[stParam.ISDBTParam.stLayerX_Param[i].eIsdbtTDI]);
                }
                else
                {
                    printf("  TDI: Unknown");
                }

                if(stParam.ISDBTParam.stLayerX_Param[i].eIsdbtFFT<(sizeof(cFFTStr_ISDBT)/sizeof(char*)))
                {
                    printf("  FFT Size: %-5s",\
                    cFFTStr_ISDBT[stParam.ISDBTParam.stLayerX_Param[i].eIsdbtFFT]);
                }
                else
                {
                    printf("  FFT Size: Unknown");
                }
            }
            printf("\n");
        }
        else if(u32BroadcastType == DTMB)
        {
             if(stParam.DTMBParam.stMOD.u8SiNR)
             {
                 printf("Constellation: 4QAM-NR\n");
             }
             else
             {
                 printf("Constellation: %dQAM\n", (int)stParam.DTMBParam.stMOD.u8SiQamMode);
             }
             printf("InterLeaver: %d\n", (int)stParam.DTMBParam.stMOD.u16SiInterLeaver);
             printf("Frame Header: PN%d\n", (int)stParam.DTMBParam.stMOD.u16PNM);

             if(stParam.DTMBParam.stMOD.u8SiCarrierMode)
                 printf("Carrier Mode: Single Carrier\n");
             else
                 printf("Carrier Mode: Multiple Carrier\n");

             if(stParam.DTMBParam.stMOD.u8PNC)
                 printf("PN Phase: Constant\n");
             else
                 printf("PN Phase: Variable\n");
         }
    }
    else
    {
        return FALSE;
    }

    return bRet;
}

static void _StatusPolling(void)
{
    MS_U16 u16SNR = 0, u16Quality = 0, u16SSI = 0, u16pkterr = 0;
    float fltBER = 0.0;
    MS_S16 sPWR = 0;
    EN_LOCK_STATUS eLockStatus;


            if((MApi_DigiTuner_GetLock(gu8TunerIndex,&eLockStatus)== TRUE))
            {
                if( E_DEMOD_LOCK == eLockStatus)
                {
                    if(MApi_DigiTuner_GetSNR(gu8TunerIndex, &u16SNR))
                    {
                        printf("SNR = %d dB, ", (int)u16SNR);
                    }

                    if(MApi_DigiTuner_GetBER(gu8TunerIndex, &fltBER))
                    {
                        printf("BER = %8.3e, ", fltBER);
                    }

                    if(MApi_DigiTuner_GetSignalQuality(gu8TunerIndex, &u16Quality))
                    {
                        printf("SQI = %d, ", (int)u16Quality);
                    }

                    if(MApi_DigiTuner_GetSSI(gu8TunerIndex, &u16SSI))
                    {
                       printf("SSI = %d, ", (int)u16SSI);
                    }

                    if(MApi_DigiTuner_GetPWR(gu8TunerIndex, &sPWR))
                    {
                       printf("Signal Strength = %d dBm, ", (int)sPWR);
                    }

                    if(MApi_DigiTuner_Get_Packet_Error(gu8TunerIndex, &u16pkterr))
                    {
                       if(u16pkterr != 0)
                           printf("Packet error exist!! \n");
                       else
                           printf("No packet error.. \n");
                    }

                }
                else
                    printf("Demod unlock\n");
            }
            else
            {
               //TBD
            }

}

MS_BOOL appDemo_DigiTuner_GetSignalStatus(const MS_U32 *pu32PollingCnt)
{
  MS_U32 u32Cnt, i;

  u32Cnt = *pu32PollingCnt;

  for(i=0;i<u32Cnt;i++)
  {
   printf("\n");
    _StatusPolling();
   printf("\n");
   MsOS_DelayTask(500);
  }
    return TRUE;
}



