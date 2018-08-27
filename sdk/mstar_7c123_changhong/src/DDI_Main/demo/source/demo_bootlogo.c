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
#if (DEMO_BOOTLOGO_TEST == 1)

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------

#include <string.h>
#include <time.h>
#include "MsCommon.h"
#include "MsMemory.h"
#include "MsOS.h"

#include "apiGOP.h"

#if (DEMO_XC_DUALXC_TEST == 1)
#include "apiPNL_EX.h"
#else
#include "apiPNL.h"
#endif

#include "demo_bootlogo.h"
#include "demo_env.h"
#include "demo_main.h"
#include "demo_utility.h"

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Definition
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------

#define BOOTLOGO_DBG                0

#if BOOTLOGO_DBG
#define db_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define db_print(fmt, args...)  while(0);
#endif

#if (DEMO_XC_DUALXC_TEST == 1)
#define IPANEL(dev, func) g_IPanelEx.func(dev)
#else
#define IPANEL(dev, func) g_IPanel.func()
#endif

#define ENV_BL_DFB_FRAMEBUFFER "bl_dfb_framebuffer_addr"

#define ENV_BL_JPD_INTER_ADDR "bl_jpd_inter_addr"
#define ENV_BL_JPD_INTER_SIZE "bl_jpd_inter_size"

#define ENV_BL_JPD_READ_ADDR "bl_jpd_read_addr"
#define ENV_BL_JPD_READ_SIZE "bl_jpd_read_size"

#define ENV_BL_JPD_WRITE_ADDR "bl_jpd_write_addr"
#define ENV_BL_JPD_WRITE_SIZE "bl_jpd_write_size"

#define ENV_VE_BUFFER_ADDR "bl_ve_buffer_addr"

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

#if (DEMO_XC_DUALXC_TEST == 1)
static PNL_DeviceId gDevId = {0,E_PNL_EX_DEVICE0};
#endif

typedef struct
{
    MS_S8 *ps8Id;
    MS_U32 u32Val;
}BL_ENV_PARAM_LIST;

static const BL_ENV_PARAM_LIST stBLParamList[]=
{
    {(MS_S8 *)ENV_BL_DFB_FRAMEBUFFER, VDEC_FRAME_BUF_ADR},

    {(MS_S8 *)ENV_BL_JPD_INTER_ADDR, JPD_INTER_BUF_ADR},
    {(MS_S8 *)ENV_BL_JPD_INTER_SIZE, JPD_INTER_BUF_LEN},

    {(MS_S8 *)ENV_BL_JPD_READ_ADDR, JPD_READ_BUF_ADR},
    {(MS_S8 *)ENV_BL_JPD_READ_SIZE, JPD_READ_BUF_LEN},

    {(MS_S8 *)ENV_BL_JPD_WRITE_ADDR, JPD_OUT_ADR},
    {(MS_S8 *)ENV_BL_JPD_WRITE_SIZE, JPD_OUT_LEN},

    {(MS_S8 *)ENV_VE_BUFFER_ADDR, VE_FRAME_BUF_ADR},

};

//-------------------------------------------------------------------------------------------------
// Global function
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
// Local function
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief Close the Bootlogo \n
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:  \b Bootlogo_close \n
//------------------------------------------------------------------------------

MS_BOOL Demo_BOOTLOGO_Close(void)
{
    GOP_InitInfo pGopInit;

    pGopInit.u16PanelWidth = IPANEL(&gDevId ,Width);
    pGopInit.u16PanelHeight = IPANEL(&gDevId ,Height);
    pGopInit.u16PanelHStr = IPANEL(&gDevId ,HStart);

    db_print("====u16PanelWidth=%d===\n",pGopInit.u16PanelWidth);
    db_print("====u16PanelHeight=%d===\n",pGopInit.u16PanelHeight);
    db_print("====u16PanelHStr=%d===\n",pGopInit.u16PanelHStr);

    pGopInit.u32GOPRBAdr = MEM_ADR_BY_MIU(GOP_GWIN_RB_ADR, GOP_GWIN_RB_MEMORY_TYPE);
    pGopInit.u32GOPRBLen = GOP_GWIN_RB_LEN;
    pGopInit.u32GOPRegdmaAdr = MEM_ADR_BY_MIU(GOP_REG_DMA_BASE_ADR, GOP_REG_DMA_BASE_MEMORY_TYPE);
    pGopInit.u32GOPRegdmaLen = GOP_REG_DMA_BASE_LEN;
    pGopInit.bEnableVsyncIntFlip = FALSE;

    MApi_GOP_Init(&pGopInit);

    db_print("GOP_GWIN_RB_ADR : %lx, GOP_REG_DMA_BASE_ADR: %lx\n", pGopInit.u32GOPRBAdr, pGopInit.u32GOPRegdmaAdr);
    db_print("driver GOP init ok\n");

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief check Bootlogo env by MMAP \n
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Sample Command:  \b Bootlogo_checkenv \n
//------------------------------------------------------------------------------

MS_BOOL Demo_BOOTLOGO_CheckEnv(void)
{
    MS_U8 u8Val[16];
    MS_U8* pu8EnvStr;
    MS_U32 u32Val;
    MS_U32 u32Idx;
    MS_BOOL bModify = FALSE;

    printf("====== BOOTLOGO Check Env Start ======\n");

    if(Demo_ENV_LoadEnv() == FALSE)
    {
        printf("Read Flash Data Failed!!\n");
        return FALSE;
    }

    //check correctness of bootlogo buffer env setting
    for(u32Idx=0; u32Idx<sizeof(stBLParamList)/sizeof(stBLParamList[0]); u32Idx++)
    {
        db_print("=================START=================\n");
        db_print("=================ps8Id : %s ===========\n",stBLParamList[u32Idx].ps8Id);
        if((pu8EnvStr = Demo_ENV_GetEnv((MS_U8 *)stBLParamList[u32Idx].ps8Id)) == NULL)
        {
            snprintf((char *)u8Val, 16, "0x%08lx", stBLParamList[u32Idx].u32Val);
            Demo_ENV_SetEnv((MS_U8 *)stBLParamList[u32Idx].ps8Id, (MS_U8 *)u8Val);
            bModify = TRUE;
        }
        else
        {
            u32Val = Demo_Util_DHConvert((char *)pu8EnvStr,1);

            if(u32Val != stBLParamList[u32Idx].u32Val)
            {
                snprintf((char *)u8Val, 16, "0x%08lx", stBLParamList[u32Idx].u32Val);
                Demo_ENV_SetEnv((MS_U8 *)stBLParamList[u32Idx].ps8Id, (MS_U8 *)u8Val);
                bModify = TRUE;
            }
        }

    }

    if(bModify == TRUE)
    {
        Demo_ENV_StoreEnv();
    }

    printf("====== BOOTLOGO Check Env END ======\n");

    return TRUE;
}


#endif
