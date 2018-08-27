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
#include "Board.h"
#include "apiDigiTuner.h"

#if IF_THIS_TUNER_INUSE(TUNER_MXL_MXL5007)
#include "MxL5007_API.h"
#include "MxL5007_Common.h"
#if defined (MSOS_TYPE_LINUX)
#include <linux/string.h>
#endif


#define TUNER_DEV_ADR                   0xC0//MxL_I2C_ADDR_96 << 2 // MT2060, Env77h11d5,FS803A=0xc0, Maxlinear=0xc6, QT1010=0xc4
#define TUNER_IF_FREQ                   (MxL_IF_4_57_MHZ)/1000//(MxL_IF_36_15_MHZ / 1000)     // 36125:  Tuner IF Frequency (KHz) or(36167, (4570 for Maxlinear))

#define SysEnterCriticalSection()
#define SysLeaveCriticalSection()
#define DBG_MSB(x)          x

static MxL5007_TunerConfigS MxL5007_TunerPara;


//**********************************************************************
// Set Tuner to desired frequency
//
//**********************************************************************
void MXL5007_SetTuner(
    MS_U32      dwFreq,    // KHz
    MxL5007_BW_MHz       ucBw       // MHz
)
{
    MS_BOOL bLock = FALSE;
    DBG_MSB(printf("\n MXL5007 SetTuner dwFreq = %ld, ucBw = %d \n",(MS_U32)(dwFreq * 1000),ucBw));
    if (MxL_Tuner_RFTune(&MxL5007_TunerPara, (MS_U32)(dwFreq * 1000), (MxL5007_BW_MHz)ucBw))
    {
        DBG_MSB(printf("\n\n#### %s Fail\n\n", __FUNCTION__));
    }

    MxL_RFSynth_Lock_Status(&MxL5007_TunerPara, &bLock);
    DBG_MSB(printf("RFSynth bLock = %d \n",bLock));

    MxL_REFSynth_Lock_Status(&MxL5007_TunerPara, &bLock);
    DBG_MSB(printf("REFSynth bLock = %d \n",bLock));

    // Release ownership of the critical section
    SysLeaveCriticalSection();
}

//**********************************************************************
// This function is not needed for LG_TDTMG551D.
//
// Please just keep it empty.
//
//**********************************************************************
static MS_U32 InitTuner(void)
{

    MxL5007_TunerConfigS myTuner;
    myTuner.BW_MHz = MxL_BW_8MHz;
    myTuner.RF_Freq_Hz = 0;
    myTuner.I2C_Addr = TUNER_DEV_ADR;
    myTuner.Mode = MxL_MODE_DVBT;
    myTuner.IF_Diff_Out_Level = 0;
    myTuner.Xtal_Freq = MxL_XTAL_24_MHZ;
    myTuner.IF_Freq = MxL_IF_4_57_MHZ;
    myTuner.IF_Spectrum = MxL_NORMAL_IF;
    myTuner.ClkOut_Setting = MxL_CLKOUT_DISABLE;
    myTuner.ClkOut_Amp = MxL_CLKOUT_AMP_0;

    memcpy(&MxL5007_TunerPara, &myTuner, sizeof(MxL5007_TunerConfigS));
    //Init Tuner
    if (MxL_Tuner_Init(&MxL5007_TunerPara))
    {
        DBG_MSB(printf("%s fail \n", __FUNCTION__));
        return (ERR_WRITE_TUNER_FAIL);
    }

    return (ERR_NO_ERROR);

}

MS_BOOL MDrv_Tuner_Init(void)
{
    if(InitTuner() == ERR_NO_ERROR)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


//**********************************************************************
// Set Tuner to desired frequency
//
//**********************************************************************
MS_U32 MDrv_Tuner_SetTuner(MS_U32 dwFreq /*Khz*/, MS_U8 ucBw /*MHz*/)
{
    MS_U32   dwError = ERR_NO_ERROR;
    MxL5007_BW_MHz MxL5007_ucBw;

    DBG_MSB(printf("\n MXL5007T SetTuner \n"));

    switch (ucBw)
    {
        case BW_MODE_6MHZ:
            MxL5007_ucBw = MxL_BW_6MHz;
        break;

        case BW_MODE_7MHZ:
            MxL5007_ucBw = MxL_BW_7MHz;
        break;

        case BW_MODE_8MHZ:
            MxL5007_ucBw = MxL_BW_8MHz;
        break;
        default:
            MxL5007_ucBw = MxL_BW_8MHz;
        break;
    }


    MXL5007_SetTuner(dwFreq,MxL5007_ucBw);


    // Release ownership of the critical section
    //SysLeaveCriticalSection();
    return (dwError);
}


//**********************************************************************
// This function is not needed for MXL5007T.
//
// Please just keep it empty.
//
//**********************************************************************
MS_U32 AdjustTuner(void)
{
    return (ERR_NO_ERROR);
}

//**********************************************************************
// This function is not needed for MXL5007T.
//
// Please just keep it empty.
//
//**********************************************************************
MS_U32 CalibrateTuner(void)
{
    return (ERR_NO_ERROR);
}

//**********************************************************************
// This function is not needed for MXL5007T.
//
// Please just keep it empty.
//
//**********************************************************************
MS_U32 MDrv_Tuner_Loop_Through_On(MS_BOOL bOn)
{
    MxL5007_LoopThru isOn;
    if(bOn)
    {
        isOn = MxL_LT_ENABLE;
    }
    else
    {
        isOn = MxL_LT_DISABLE;
    }
    MxL_Loop_Through_On(&MxL5007_TunerPara,isOn);
    return (ERR_NO_ERROR);
}

//**********************************************************************
// This function is not needed for MXL5007T.
//
// Please just keep it empty.
//
//**********************************************************************
MS_U32 MDrv_Tuner_Stand_By(void)
{
    MxL_Stand_By(&MxL5007_TunerPara);
    return (ERR_NO_ERROR);
}

//**********************************************************************
// This function is not needed for MXL5007T.
//
// Please just keep it empty.
//
//**********************************************************************
MS_U32 MDrv_Tuner_Wake_Up(void)
{
    MxL_Wake_Up(&MxL5007_TunerPara);
    return (ERR_NO_ERROR);
}


#endif

