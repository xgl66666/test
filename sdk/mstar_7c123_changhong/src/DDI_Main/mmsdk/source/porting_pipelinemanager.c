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
#ifdef OPTEE_ENABLE
#include <stdio.h>
#include <string.h>     /* memset */
#include "porting_pipelinemanager.h"
#include "porting_os.h"
#include "pipelinemgr.h"
#include "pipeline_engine.h"

#define FLOW(fmt, arg...)        printf("\033[1;33m######[%s]######"fmt" \033[0m\n",__FUNCTION__,##arg)
#define ERR(fmt, arg...)         printf("\033[1;31m######[%s][%d]######"fmt" \033[0m\n",__FUNCTION__,__LINE__,##arg)

typedef struct
{
    /// pipeline enable or not
    MMSDK_BOOL bSvpPplEnable;
    /// pipeline id
    MMSDK_U32 u32SvpPplID;
}_ST_MMSDK_PIPELINE_INSTANCE;

MMSDK_BOOL PT_PipeLineManager_CreatePipeLine(PT_PIPELINEITEM* pPipeItem, const ST_MMSDK_PIPE_INFO * pstPipeInfo)
{
    FLOW("");
    MMSDK_BOOL bRet = FALSE;
    MMSDK_U32 u32SvpPplID = 0;

    _ST_MMSDK_PIPELINE_INSTANCE* pPipeLineInstance = NULL;
    if ((pPipeItem == NULL) ||(pstPipeInfo == NULL))
    {
        ERR("pPipeItem=%p\n",pPipeItem);
        ERR("pstPipeInfo=%p\n",pstPipeInfo);
        return bRet;
    }

    if (pstPipeInfo->ePipeType != E_MMSDK_PIPE_ENGINE_TYPE_MASTER)
    {
        ERR("Wrong pipe type, Pipe Type:0x%x\n",(MMSDK_U32)pstPipeInfo->ePipeType);
        return bRet;
    }

    pPipeLineInstance = (_ST_MMSDK_PIPELINE_INSTANCE*) PT_MsOS_AllocateMemory(sizeof(_ST_MMSDK_PIPELINE_INSTANCE));

    if (pPipeLineInstance !=  NULL)
    {
        // create pipe line
        if (SvpPplCreate(&u32SvpPplID) == E_PP_SUCCESS)
        {
            FLOW("u32SvpPplID = %d", u32SvpPplID);
            pPipeLineInstance->u32SvpPplID = u32SvpPplID;
            pPipeLineInstance->bSvpPplEnable = TRUE;
            *pPipeItem = (void*)pPipeLineInstance;
            bRet = TRUE;
        }
        else
        {
            PT_MsOS_FreeMemory(pPipeLineInstance);
            pPipeLineInstance = NULL;
            *pPipeItem = NULL;
            ERR("Create PipeLine Master fail\n");
        }
    }

    return bRet;
}

MMSDK_BOOL PT_PipeLineManager_DeletePipeLine(PT_PIPELINEITEM* pPipeItem, const ST_MMSDK_PIPE_INFO * pstPipeInfo)
{
    FLOW("");
    MMSDK_BOOL bRet = FALSE;
    _ST_MMSDK_PIPELINE_INSTANCE* pPipeLineInstance = (_ST_MMSDK_PIPELINE_INSTANCE*)(*pPipeItem);

    if ((pPipeItem == NULL) || (pstPipeInfo == NULL))
    {
        ERR("pPipeItem=%p\n",pPipeItem);
        ERR("pstPipeInfo=%p\n",pstPipeInfo);
        return FALSE;
    }

    if (pstPipeInfo->ePipeType != E_MMSDK_PIPE_ENGINE_TYPE_MASTER)
    {
        ERR("Wrong pipe type, Pipe Type:0x%x\n",(MMSDK_U32)pstPipeInfo->ePipeType);
        return FALSE;
    }

    if ((pPipeLineInstance->u32SvpPplID !=pstPipeInfo->u32SvpPplID) || (pPipeLineInstance->bSvpPplEnable != pstPipeInfo->bPipeEnable))
    {
        ERR("Pipe info of Master is Mis-match, Enable(0x%x, 0x%x), PipeID(0x%x, 0x%x)\n",
            pPipeLineInstance->bSvpPplEnable, pstPipeInfo->bPipeEnable,
            pPipeLineInstance->u32SvpPplID, pstPipeInfo->u32SvpPplID);
        return FALSE;
    }

    // deleate pipe line
    if (SvpPplDelete(pstPipeInfo->u32SvpPplID) == E_PP_SUCCESS)
    {
        PT_MsOS_FreeMemory(*pPipeItem);
        *pPipeItem = NULL;
        bRet = TRUE;
    }
    else
    {
        ERR("SvpPplDelete Fail\n");
        bRet = FALSE;
    }

    return bRet;

}

MMSDK_BOOL PT_PipeLineManager_RegisterPipe(PT_PIPELINEITEM PipeItem, ST_MMSDK_PIPE_INFO * pstPipeInfo)
{
    FLOW("");

    _ST_MMSDK_PIPELINE_INSTANCE* pPipeLineInstance = (_ST_MMSDK_PIPELINE_INSTANCE*)PipeItem;
    MMSDK_BOOL bRet = FALSE;
    SvpPplPipe svpPplPipe;
    memset(&svpPplPipe, 0, sizeof(SvpPplPipe));

    if ((PipeItem == NULL) || (pstPipeInfo == NULL))
    {
        ERR("PipeItem=%p\n",PipeItem);
        ERR("pstPipeInfo=%p\n",pstPipeInfo);
        return bRet;
    }

    if (!pPipeLineInstance->bSvpPplEnable)
    {
        ERR("Can not register pipe id, please create PipeLine first.\n");
        return bRet;
    }

    switch(pstPipeInfo->ePipeType)
    {
        case E_MMSDK_PIPE_ENGINE_TYPE_VDEC:
            svpPplPipe.u32Type = VDEC_Engine;
            svpPplPipe.u64ID = pstPipeInfo->u32SvpPplID;
            break;
        case E_MMSDK_PIPE_ENGINE_TYPE_XC:
            svpPplPipe.u32Type = XC_Engine;
            svpPplPipe.u64ID = pstPipeInfo->u32SvpPplID;
            break;
        default:
            ERR("Pipe Type is 0x%x, not support register pipe!!!\n", svpPplPipe.u32Type);
            return bRet;
            break;
    }

    if (!pstPipeInfo->bPipeEnable)
    {
        if(SvpPplRegisterPipe(pPipeLineInstance->u32SvpPplID, &svpPplPipe) == E_PP_SUCCESS)
        {
            bRet = TRUE;
        }
        else
        {
            ERR("RegisterPipe Fail.\n");
            bRet = FALSE;
        }
    }

    return bRet;
}

MMSDK_BOOL PT_PipeLineManager_UnRegisterPipe(PT_PIPELINEITEM PipeItem, ST_MMSDK_PIPE_INFO * pstPipeInfo)
{
    FLOW("");

    _ST_MMSDK_PIPELINE_INSTANCE* pPipeLineInstance = (_ST_MMSDK_PIPELINE_INSTANCE*)PipeItem;
    MMSDK_BOOL bRet = FALSE;
    SvpPplPipe svpPplPipe;
    memset(&svpPplPipe, 0, sizeof(SvpPplPipe));

    if ((PipeItem == NULL) || (pstPipeInfo == NULL))
    {
        ERR("PipeItem=%p\n",PipeItem);
        ERR("pstPipeInfo=%p\n",pstPipeInfo);
        return bRet;
    }

    if (!pPipeLineInstance->bSvpPplEnable)
    {
        ERR("No need  un-register pipe id, because PipeLine no create.\n");
        return bRet;
    }

    switch(pstPipeInfo->ePipeType)
    {
        case E_MMSDK_PIPE_ENGINE_TYPE_VDEC:
            svpPplPipe.u32Type = VDEC_Engine;
            svpPplPipe.u64ID = pstPipeInfo->u32SvpPplID;
            break;
        case E_MMSDK_PIPE_ENGINE_TYPE_XC:
            svpPplPipe.u32Type = XC_Engine;
            svpPplPipe.u64ID = pstPipeInfo->u32SvpPplID;
            break;
        default:
            ERR("Pipe Type is 0x%x, not support register pipe!!!\n", svpPplPipe.u32Type);
            return bRet;
            break;
    }

    if (pstPipeInfo->bPipeEnable)
    {
        if(SvpPplUnRegisterPipe(pPipeLineInstance->u32SvpPplID, &svpPplPipe) == E_PP_SUCCESS)
        {
            bRet = TRUE;
        }
        else
        {
            ERR("UnRegisterPipe Fail.\n");
            bRet = FALSE;
        }
    }

    return bRet;
}

#endif
