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



//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
//Case 1: Basic CA without KeyLadder: For example, Kriti, Kappa, Kenya
//Case 2: Advanced CA with KeyLadder: For example, Kaiser, Keltic, Keres, Kirin, Kano
#if (DEMO_DSCMB_ADVANCED_TEST == 1)
#include <string.h>
//	#include <ctype.h>
#include "MsCommon.h"
#include "MsMemory.h"
#if (DEMO_DSCMB_UTPA2_TEST == 1)
#include "utopia.h"
#include "drvDSCMB_v2.h"
#else
#include "drvDSCMB.h"
#endif
#include "demo_keyladder.h"

static void _KeyLadder_Dump_Buf(const MS_U8 *buf, MS_U32 len)
{
    MS_U32 i = 0;
    for (i = 0; i < len; i++)
    {
        printf("%02x%c", buf[i], ((i % 16) == 15) ? '\n' : ' ');
    }
    printf("\n");
}
#if (DEMO_DSCMB_UTPA2_TEST == 1)
MS_BOOL DEMO_KeyLadder_Setup(DEMOKLCONFIG stKLInfo)
{
    DSCMB_KL_Status KL_Status = 0;
    MS_U8 ACPU_Out[16] = {0x00};
    DSCMB_KL_ATOMICEXEC pArgs;
    void *pu32DscmbInst = NULL;

    DSCMB_KLCfg_All KLConfigAll =
    {
        .eAlgo = stKLInfo.eKLMode,
        .eSrc = stKLInfo.eSrc,
        .eDst = stKLInfo.eDst,
        .eOutsize = E_DSCMB_KL_128_BITS,
        .eKeyType = stKLInfo.eKeyType,    // Don't care when dst is Crypto DMA
        .u32Level = stKLInfo.u32Level,
        .u32EngID = 0,                    //Set to zero in STB chip, Don't care when dst is Crypto DMA
        .u32DscID = stKLInfo.u32DscmbId,  // Don't care when dst is Crypto DMA
        .u8KeyACPU = stKLInfo.pu8KeyACPU,
        .pu8KeyKLIn = stKLInfo.pu8KeyKLIn,
        .bDecrypt = TRUE,
        .bInverse = FALSE,
        .eKLSel   = E_DSCMB_KL_SEL_DEFAULT,
        .u32CAVid = stKLInfo.u32CAVid,
    };


    memset(&pArgs, 0, sizeof(DSCMB_KL_ATOMICEXEC));

    pArgs.KLCfg     = &KLConfigAll;
    pArgs.ACPU_Out  = ACPU_Out;
    pArgs.u32Status = &KL_Status;

    if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_DSCMB, &pu32DscmbInst, 0x0, NULL))
    {
        printf("UtopiaOpen DSCMB fail\n");
        goto DEMO_KeyLadder_Setup_FAIL;
    }

    if(UTOPIA_STATUS_SUCCESS != UtopiaIoctl(pu32DscmbInst, E_MDRV_CMD_DSCMB_KLadder_AtomicExec, (void*)&pArgs))
    {
        printf("[%s][%d]E_MDRV_CMD_DSCMB_KLadder_AtomicExec failed\n", __FUNCTION__,__LINE__);
        printf("Key Ladder: Fail!!! Status = 0x%X\n", (unsigned int)KL_Status);
        goto DEMO_KeyLadder_Setup_FAIL;
    }

    if(UTOPIA_STATUS_FAIL == UtopiaClose(pu32DscmbInst))
    {
        printf("UtopiaClose DSCMB fail\n");
        goto DEMO_KeyLadder_Setup_FAIL;
    }

    // Key Ladder
    if(KLConfigAll.eDst == E_DSCMB_KL_DST_CPU_ACPU)
    {
        _KeyLadder_Dump_Buf(ACPU_Out, 16);
    }

    return TRUE;

DEMO_KeyLadder_Setup_FAIL:

    return FALSE;
}
#else

MS_BOOL DEMO_KeyLadder_Setup(DEMOKLCONFIG stKLInfo)
{
    DSCMB_KL_Status KL_Status = 0;
    MS_U8 ACPU_Out[16] = {0x00};
    MS_BOOL ret = FALSE;

    DSCMB_KLCfg_All KLConfigAll =
    {
        .eAlgo = stKLInfo.eKLMode,
        .eSrc = stKLInfo.eSrc,
        .eDst = stKLInfo.eDst,
        .eOutsize = E_DSCMB_KL_128_BITS,
        .eKeyType = stKLInfo.eKeyType,// Don't care when dst is Crypto DMA
        .u32Level = stKLInfo.u32Level,
        .u32EngID = 0,                      //Set to zero in STB chip, Don't care when dst is Crypto DMA
        .u32DscID = stKLInfo.u32DscmbId,// Don't care when dst is Crypto DMA
        .u8KeyACPU = stKLInfo.pu8KeyACPU,
        .pu8KeyKLIn = stKLInfo.pu8KeyKLIn,
        .bDecrypt = TRUE,
        .bInverse = FALSE,
        .eKLSel   = E_DSCMB_KL_SEL_DEFAULT,
        .u32CAVid = stKLInfo.u32CAVid,
    };

    // Key Ladder
    ret = MDrv_DSCMB2_KLadder_AtomicExec(&KLConfigAll , ACPU_Out, &KL_Status );
    if(ret == FALSE)
    {
        printf("Key Ladder: Fail!!! Status = 0x%X\n", (unsigned int)KL_Status);
        return FALSE;
    }
    if(KLConfigAll.eDst == E_DSCMB_KL_DST_CPU_ACPU)
    {
        _KeyLadder_Dump_Buf(ACPU_Out, 16);
    }

    return TRUE;
}

#endif //DEMO_DSCMB_UTPA2_TEST == 1
#endif //DEMO_DSCMB_ADVANCED_TEST == 1

