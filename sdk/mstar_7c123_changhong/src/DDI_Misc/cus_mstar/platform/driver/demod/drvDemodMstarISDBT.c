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
/**********************************************************************
Copyright (c) 2006-2009 MStar Semiconductor, Inc.
All rights reserved.
Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
MStar Semiconductor Inc. and be kept in strict confidence
(MStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of MStar Confidential
Information is unlawful and strictly prohibited. MStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
 **********************************************************************/
#define _INTERN_ISDBT_C_
#include "Board.h"
#if defined(CHIP_KAPPA) || defined(CHIP_KIWI)

#include <math.h>
#include "MsCommon.h"
#include "drvDemod.h"
//#include "Tuner.h"
#include "MsOS.h"
//#include "debug.h"
#include "drvDemodMstarISDBT.h"
#include "drvDMD_common.h"
#include "drvDMD_ISDBT.h"       //dan add for integrate ISDBT utopia driver
#include "drvDemodNull.h"
#include "drvDMD_VD_MBX.h"
#define MEMALIGN(align, address)               ((((address)+(align)-1)/(align))*(align))
#define LOWMEMALIGN(align, address)            (((address)/(align))*(align))

#include "MsMemory.h"


#define ERR_DEMOD_MSB(x)          //  x
#define DBG_DOMOD_MSB(x)          //   x
#define DBG_DEMOD_CHECK_LOCK(x)   // x
#define DBG_DEMOD_INFO(x)   // x
#define DBG_SSI(x)        //  x
#define DBG_SQI(x)        //  x

#define usleep(x)             MsOS_DelayTaskUs(x)

#if TS_PARALLEL_OUTPUT
#define ISDBT_TS_SERIAL      0x00
#else
#define ISDBT_TS_SERIAL      0x01
#endif
// Division number of TS clock rate
// Formula: TS Clk Rate= 216 MHz/reg_isdbt_ts_clk_divnum
// Ex: ISDBT_TS_CLK_DIVNUM = 31 => TS Clk = 216/31= 6.967 MHz
#define ISDBT_TS_CLK_DIVNUM_SERIAL_MODE   4
#define ISDBT_TS_CLK_DIVNUM_PARALLEL_MODE  31

#if (ISDBT_TS_SERIAL)
#define ISDBT_TS_CLK_DIVNUM      ISDBT_TS_CLK_DIVNUM_SERIAL_MODE
#else
#define ISDBT_TS_CLK_DIVNUM      ISDBT_TS_CLK_DIVNUM_PARALLEL_MODE
#endif

#define ISDBT_TS_DATA_SWAP  0x00  // TS data inversion
#define ISDBT_TS_CLK_INV      TS_CLK_INV  // TS clock inverseion //this define will depend on hw main board
#define ISDBT_IQ_SWAP   0x00          // IQ Swap  //this define will depend on tuner
#define ISDBT_IFAGC_REF 0x500              //this define will depend on tuner

const static MS_U32 _u32IFrequency = 5000;
static DEMOD_ISDBT_Layer eISDBT_Layer = DEMOD_ISDBT_Layer_A;
static MS_U32 u32ISDBT_TDI_BUF_ADR = NULL;

//static BOOL s_bI2C_FastMode = false;

//float INTERN_ISDBT_GetSNR(MS_U8 u8DemodIndex);

MAPI_BOOL DeviceDemodCreate(void)
{
    return MAPI_TRUE;
}

MAPI_BOOL Connect(void)
{
    return MAPI_TRUE;
}
MAPI_BOOL Disconnect(void)
{
    return MAPI_TRUE;
}
void Reset(void)
{

}

MAPI_BOOL Set_PowerOn(void)
{
    return TRUE;
}

MAPI_BOOL Set_PowerOff(void)
{
    return TRUE;
}

MAPI_BOOL MDrv_MStar_ISDBT_Power_On_Initialization(MS_U8 u8DemodIndex,DEMOD_MS_INIT_PARAM* pParam)
{
    MAPI_BOOL   bResult = true;
    DMD_ISDBT_InitData sDMD_ISDBT_InitData;
	MS_U32 u32IF_Freq = 0;
    static MS_U8 u8DMD_ISDBT_InitExt[]={1}; // RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning

    MDrv_DMD_PreInit();
	
	//if(!pParam->pstTunertab->GetTunerIF(u8DemodIndex, &u32IF_Freq))
	if(1)
    {
        u32IF_Freq = _u32IFrequency;
        DMD_DBG(("INTERN_ISDBT Get Tuner IF FAIL, use default Tuner IF %d kHz\n",(int)u32IF_Freq));
    }
    else
    {
       DMD_DBG(("INTERN_ISDBT Get Tuner IF %d kHz\n", (int)u32IF_Freq));
    }

    // copy tuner config
    //mapi_interface::Get_mapi_pcb()->GetIsdbTuner(0)->ExtendCommand(mapi_tuner_datatype::E_TUNER_SUBCMD_GET_IQ_SWAP, E_TUNER_DTV_ISDB_MODE, NULL, &u8IqSwap);
    //mapi_interface::Get_mapi_pcb()->GetIsdbTuner(0)->ExtendCommand(mapi_tuner_datatype::E_TUNER_SUBCMD_GET_DTV_IF_FREQ, E_TUNER_DTV_ISDB_MODE, NULL, &u16DtvIFFreqKHz);
    sDMD_ISDBT_InitData.u16IF_KHZ = u32IF_Freq;//u16DtvIFFreqKHz;
    sDMD_ISDBT_InitData.bIQSwap = ISDBT_IQ_SWAP;
    sDMD_ISDBT_InitData.u16AgcReferenceValue = ISDBT_IFAGC_REF;
    sDMD_ISDBT_InitData.u16ISDBTFECLockCheckTime = 2000;
    sDMD_ISDBT_InitData.u16ISDBTIcfoChExistCheckTime = 300;
    sDMD_ISDBT_InitData.u8DMD_ISDBT_InitExt = u8DMD_ISDBT_InitExt;

#ifdef ISDBT_TDI_BUF_ADR
    sDMD_ISDBT_InitData.u32TdiStartAddr= ISDBT_TDI_BUF_ADR/16;
#else
    if(NULL == u32ISDBT_TDI_BUF_ADR)
        return FALSE;
    else
        sDMD_ISDBT_InitData.u32TdiStartAddr= u32ISDBT_TDI_BUF_ADR/16;
#endif

    //TS[begin]
    //Parallel mode
    //sDMD_ISDBT_InitData.u5TsConfigByte_DivNum = ISDBT_TS_CLK_DIVNUM;
    //sDMD_ISDBT_InitData.u1TsConfigByte_ClockInv = ISDBT_TS_CLK_INV;
    //sDMD_ISDBT_InitData.u1TsConfigByte_DataSwap = 0;
    //sDMD_ISDBT_InitData.u1TsConfigByte_SerialMode = ISDBT_TS_SERIAL;
    //Serial mode
    sDMD_ISDBT_InitData.u5TsConfigByte_DivNum = ISDBT_TS_CLK_DIVNUM;
    sDMD_ISDBT_InitData.u1TsConfigByte_ClockInv = ISDBT_TS_CLK_INV;
    sDMD_ISDBT_InitData.u1TsConfigByte_DataSwap = ISDBT_TS_DATA_SWAP;
    sDMD_ISDBT_InitData.u1TsConfigByte_SerialMode = ISDBT_TS_SERIAL;

    //TS[end]

    sDMD_ISDBT_InitData.bIsExtDemod = FALSE;
    MDrv_SYS_DMD_VD_MBX_Init();
    MDrv_DMD_ISDBT_Initial_Hal_Interface();
    MDrv_DMD_ISDBT_Init(&sDMD_ISDBT_InitData, sizeof(sDMD_ISDBT_InitData));


    return bResult;
}

MAPI_BOOL MDrv_MStar_ISDBT_Lock(MS_U8 u8DemodIndex,EN_LOCK_STATUS *peLockStatus)
{
    DMD_ISDBT_LOCK_STATUS eLockStatus = DMD_ISDBT_UNLOCK;
    COFDM_LOCK_STATUS eStatus = COFDM_FEC_LOCK;
    //printf("dan COFDM_LOCK_STATUS = %d\n", (MS_U8)eStatus);

    if(eStatus == COFDM_ICFO_CH_EXIST_FLAG)
        eLockStatus = MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK_ICFO_CH_EXIST_LOCK);
    else if(eStatus == COFDM_FEC_LOCK)
    {
        eLockStatus = MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK_FEC_LOCK);
    }


    eLockStatus = MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK);
    switch (eLockStatus) // _UTOPIA
    {
        case DMD_ISDBT_LOCK:
            *peLockStatus = E_DEMOD_LOCK;
            break;
        case DMD_ISDBT_CHECKEND:
            *peLockStatus = E_DEMOD_CHECKEND;
            break;
        case DMD_ISDBT_UNLOCK:
            *peLockStatus = E_DEMOD_UNLOCK;
            break;
        case DMD_ISDBT_CHECKING:
        default:
            *peLockStatus = E_DEMOD_CHECKING;
            break;
    }

    return TRUE;
}
MAPI_BOOL MDrv_MStar_ISDBT_Config(RF_CHANNEL_BANDWIDTH BW, MAPI_BOOL bSerialTS, MAPI_BOOL bPalBG, MAPI_BOOL bLPSel)
{
    BW = BW;
    bSerialTS = bSerialTS;
    bPalBG = bPalBG;
    bLPSel = bLPSel;
    return true;
}


MAPI_BOOL MDrv_MStar_ISDBT_Serial_Control(MAPI_BOOL bEnable)
{
    MS_U8  u8TsConfigData;

    u8TsConfigData = 0;
    //printf("dan enter MSB1400_Serial_Control\n");
    if(bEnable == TRUE)  //serial mode
    {
        u8TsConfigData |= 0x01;
        u8TsConfigData |= ISDBT_TS_CLK_DIVNUM_SERIAL_MODE << 3;  //0x04
    }
    else
    {
        u8TsConfigData &= ~0x01;
        u8TsConfigData |= ISDBT_TS_CLK_DIVNUM_PARALLEL_MODE << 3; //0x31
    }

    u8TsConfigData |= ISDBT_TS_DATA_SWAP << 1;
    u8TsConfigData |= ISDBT_TS_CLK_INV << 2;

    return MDrv_DMD_ISDBT_SetSerialControl(u8TsConfigData);
}


/************************************************************************************************
  Subject:    TS1 output control
  Function:   INTERN_DVBT_PAD_TS1_Enable
  Parmeter:   flag : TRUE For Turn on TS1, FALSE For Turn off TS1
  Return:     void
  Remark:
*************************************************************************************************/
void INTERN_ISDBT_PAD_TS1_Enable(MAPI_BOOL flag)
{
    printf(" @INTERN_ISDBT_PAD_TS1_Enable... \n");
    if(flag) // PAD_TS1 Enable TS CLK PAD
    {

    }
    else // PAD_TS1 Disable TS CLK PAD
    {

    }
}

MS_BOOL MDrv_MStar_ISDBT_Demod_GetSNR(MS_U8 u8DemodIndex, float* pfSNR)
{
    MS_BOOL bRtn = FALSE;
    bRtn = MDrv_DMD_ISDBT_GetSNR(pfSNR);
    return bRtn;
}

MS_BOOL MDrv_MStar_ISDBT_Demod_GetBER(MS_U8 u8DemodIndex, float* pfBER)
{
    MS_BOOL bRtn = FALSE;
    bRtn = MDrv_DMD_ISDBT_GetPostViterbiBer((EN_ISDBT_Layer)eISDBT_Layer, pfBER);
    return bRtn;
}

MS_BOOL MDrv_MStar_ISDBT_Demod_GetPWR(MS_U8 u8DemodIndex,MS_S32 *ps32Signal)
{
    MS_BOOL bRtn = FALSE;
    bRtn = MDrv_DMD_ISDBT_GetSignalStrength((MS_U16*)ps32Signal);
    return bRtn;
}

MS_BOOL MDrv_MStar_ISDBT_Demod_GetSignalQuality(MS_U8 u8DemodIndex, MS_U16* u16Quality)
{
    FUNCTION_RESULT ret = E_RESULT_SUCCESS;

    *u16Quality = MDrv_DMD_ISDBT_GetSignalQuality();
    if ( *u16Quality > 100 )
        ret = E_RESULT_FAILURE;

    return ret;
}

MS_BOOL MDrv_MStar_ISDBT_Demod_GetPacketError(MS_U8 u8DemodIndex, MS_U16 *u16PktErr)
{
    MS_BOOL bRtn = FALSE;

    bRtn = MDrv_DMD_ISDBT_Read_PKT_ERR(E_ISDBT_Layer_A, u16PktErr);
    return bRtn;
}

MS_BOOL MDrv_MStar_ISDBT_Demod_Restart(MS_U8 u8DemodIndex, DEMOD_MS_FE_CARRIER_PARAM* pParam,MS_U32 u32BroadCastType)
{
    MS_BOOL bRtn = FALSE;
    bRtn = MDrv_DMD_ISDBT_SetConfig(TRUE);
    return bRtn;
}

MS_BOOL MDrv_MStar_ISDBT_Demod_Open(MS_U8 u8DemodIndex)
{
    MDrv_DMD_PreInit();
    return TRUE;
}

MS_BOOL MDrv_MStar_ISDBT_Extension_Function(MS_U8 u8DemodIndex, DEMOD_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    MS_BOOL bret = TRUE;

    switch(fuction_type)
    {
        case DEMOD_EXT_FUNC_OPEN:
            bret &= MDrv_MStar_ISDBT_Demod_Open(u8DemodIndex);
            break;
        case DEMOD_EXT_FUNC_SET_ISDBT_LAYER:
            eISDBT_Layer = *(DEMOD_ISDBT_Layer*)data;
            break;
        case DEMOD_EXT_FUNC_FINALIZE:
            bret &= MDrv_DMD_ISDBT_Exit();
            break;
        case DEMOD_EXT_FUNC_SET_BUFFER_ADDR:
            u32ISDBT_TDI_BUF_ADR = *(MS_U32*)data;
            break;
        default:
            printf("Request extension function (%x) does not exist\n",fuction_type);
            break;
    }
    return bret ;
}

MS_BOOL MDrv_MStar_ISDBT_Demod_I2C_ByPass(MS_U8 u8DemodIndex,MS_BOOL bOn)
{
  if(GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_ISDBT).I2CByPassPreSetting != NULL)
  {
      return GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_ISDBT).I2CByPassPreSetting(u8DemodIndex,bOn);
  }
  else
      return MDrv_Demod_null_I2C_ByPass(u8DemodIndex,bOn);
}


DRV_DEMOD_TABLE_TYPE GET_DEMOD_ENTRY_NODE(DEMOD_MSINTERN_ISDBT) DDI_DRV_TABLE_ENTRY(demodtab) =
{
     .name                         = "DEMOD_MSINTERN_ISDBT",
     .data                         = DEMOD_MSINTERN_ISDBT,
     .init                         = MDrv_MStar_ISDBT_Power_On_Initialization,
     .GetLock                      = MDrv_MStar_ISDBT_Lock,
     .GetSNR                       = MDrv_MStar_ISDBT_Demod_GetSNR,
     .GetBER                       = MDrv_MStar_ISDBT_Demod_GetBER,
     .GetPWR                       = MDrv_MStar_ISDBT_Demod_GetPWR,
     .GetQuality                   = MDrv_MStar_ISDBT_Demod_GetSignalQuality,
     .GetParam                     = MDrv_Demod_null_GetParam,
     .Restart                      = MDrv_MStar_ISDBT_Demod_Restart,
     .I2CByPass                    = MDrv_MStar_ISDBT_Demod_I2C_ByPass,
     .I2CByPassPreSetting          = NULL,
     .Extension_Function           = MDrv_MStar_ISDBT_Extension_Function,
     .Extension_FunctionPreSetting = NULL,
     .Get_Packet_Error             = MDrv_MStar_ISDBT_Demod_GetPacketError,     
#if MS_DVBT2_INUSE
     .SetCurrentDemodType          = MDrv_Demod_null_SetCurrentDemodType,
     .GetCurrentDemodType          = MDrv_Demod_null_GetCurrentDemodType,
     .GetPlpBitMap                 = MDrv_Demod_null_GetPlpBitMap,
     .GetPlpGroupID                = MDrv_Demod_null_GetPlpGroupID,
     .SetPlpGroupID                = MDrv_Demod_null_SetPlpGroupID,
     .GetNextPLPID                 = MDrv_Demod_null_GetNextPLPID,
     .GetPLPType                   = MDrv_Demod_null_GetPLPType,
#endif
#if MS_DVBS_INUSE
     .BlindScanStart               = MDrv_Demod_null_BlindScan_Start,
     .BlindScanNextFreq            = MDrv_Demod_null_BlindScan_NextFreq,
     .BlindScanWaitCurFreqFinished = MDrv_Demod_null_BlindScan_WaitCurFreqFinished,
     .BlindScanCancel              = MDrv_Demod_null_BlindScan_Cancel,
     .BlindScanEnd                 = MDrv_Demod_null_BlindScan_End,
     .BlindScanGetChannel          = MDrv_Demod_null_BlindScan_GetChannel,
     .BlindScanGetCurrentFreq      = MDrv_Demod_null_BlindScan_GetCurrentFreq,
     .DiSEqCSetTone                = MDrv_Demod_null_DiSEqC_SetTone,
     .DiSEqCSetLNBOut              = MDrv_Demod_null_DiSEqC_SetLNBOut,
     .DiSEqCGetLNBOut              = MDrv_Demod_null_DiSEqC_GetLNBOut,
     .DiSEqCSet22kOnOff            = MDrv_Demod_null_DiSEqC_Set22kOnOff,
     .DiSEqCGet22kOnOff            = MDrv_Demod_null_DiSEqC_Get22kOnOff,
     .DiSEqC_SendCmd               = MDrv_Demod_null_DiSEqC_SendCmd
#endif
};

#endif
#undef _INTERN_ISDBT_C_


