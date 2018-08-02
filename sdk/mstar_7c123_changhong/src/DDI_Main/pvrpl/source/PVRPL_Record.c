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
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   PVRPL_Record.c
/// @author MStar Semiconductor Inc.
/// @brief  FileIn Eng porting layer
///
/// @name how to port FileIn Eng
/// @verbatim
///=================================================================================================
#include "apiDMX.h"

#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
#include "apiSWPVR.h"
#endif

#include "drvDSCMB.h"
#include "MsMemory.h"
#include "MsCommon.h"
#include "PVRPL_Record.h"
#include "PVRPL_ResourcePool.h"
#include "drvDTC.h"
#include "apiPVR.h"

#define PVRPL_RECORD_DBGMSG(_level,_f) {if(_u32PVRPLRECORDDbgLevel >= (_level)) (_f);}
static  MS_U32  _u32PVRPLRECORDDbgLevel = E_PVRPL_RECORD_DBG_ERR;

extern  MS_S32 gs32CachedPoolID ;
extern  MS_S32 gs32NonCachedPoolID;


#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
static  MS_BOOL bActiveSWPVR = FALSE;
static  MS_U8   u8RecordSWEngNum = 0;
EN_PVRPL_DMX_PVR_ENG     eSWPVREng = E_PVRPL_DMX_PVR_EGN0;
EN_PVRPL_DMX_TSIF        eSWPVRTsif = E_PVRPL_DMX_TSIF_LIVE0;
static MS_BOOL _PVRPL_Record_SWPVR_Init(MS_U32 u32PhyAdr, MS_U32 u32Size, MS_U32 u32SWEngNum);
#endif

#if (DEMO_PVR_UTOPIA2L_64BIT_TEST)
#define PVRPL_UTOPIA64BIT_MASK(Dest, Source){Dest = Source & 0xFFFFFFFFUL;}
#else
#define PVRPL_UTOPIA64BIT_MASK(Dest, Source){Dest = Source;}
#endif

PVRPL_Rec_Filters _PVRRecFilters[128];

//@NOTE This function is implemented to
//1. Initiate all variables used in Record Eng
//2. Set reocrd pkt format
//3. Set playback flowset and PVR flowset
EN_PVRPL_RECORD_STATUS PVRPL_Record_Init(RecordResource *stResource,MS_U8 u8PKTMode,MS_U32 u32BufPhyAddr,MS_U32 u32BufSize,MS_BOOL bRecAll)
{

#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    #define PVRPL_SWPVR_MAX_ENG_NUM     4

    if((stResource->u8PathIdx & PVRPL_SWPVR_INDICATOR_MASK) != 0)
    {
        //@NOTE Init SWPVR
        if(_PVRPL_Record_SWPVR_Init(u32BufPhyAddr,u32BufSize,PVRPL_SWPVR_MAX_ENG_NUM) == TRUE)
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_TRACE,printf("[%s][%s][%d]Active SWPVR !!!\n",__FILE__,__FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_ACTIVE_SWPVR_SUCCESS;
        }
        else
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d]Active SWPVR Fail !!!\n",__FILE__,__FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_FAIL;
        }
    }

#endif


#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        if(stResource->u8PathIdx >= PVRPL_SWPVR_MAX_ENG_NUM)
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d]Wrong PVR Idx:%d !!!\n",__FILE__,__FUNCTION__,__LINE__,stResource->u8PathIdx));
            return E_PVRPL_RECORD_FAIL;
        }
    }
    else
#endif
    {
        MS_U32 u32MaxPVREngNum = 0;
        MApi_DMX_GetCap(DMX_CAP_PVR_ENG_NUM,(void *)(&u32MaxPVREngNum));

        if(stResource->u8PathIdx >= u32MaxPVREngNum)
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d]Wrong PVR Idx:%d !!!\n",__FILE__,__FUNCTION__,__LINE__,stResource->u8PathIdx));
            return E_PVRPL_RECORD_FAIL;
        }
    }

    if(stResource->bInit == TRUE)
    {
        PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d]Record Eng Already Init!\n",__FILE__,__FUNCTION__,__LINE__));
        return E_PVRPL_RECORD_FAIL;
    }

    //@NOTE record resource allocation
#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    //@NOTE same as SWPVR PATH
    //the same HWPVR Enf of SWPVR
    if(bActiveSWPVR == TRUE)
    {
        stResource->ePVREng = Pool_GetEng(stResource->u8PathIdx);
        stResource->eTsif = eSWPVRTsif;
    }
    else
#endif
    {
        #if (DEMO_PVR_V4_TEST == 1)
        stResource->ePVREng = Pool_GetEng(stResource->u8PathIdx);
        #else
        stResource->ePVREng = Pool_GetRecEng();
        #endif
        //@FIXME bad naming
        DMX_FLOW_INPUT eDmxFlowInput;
        stResource->eTsif = Pool_GetSource(&eDmxFlowInput);
        MS_U32 u32EngNum;
        MApi_DMX_GetCap(DMX_CAP_PVR_ENG_NUM,&u32EngNum);
        if(stResource->ePVREng >= u32EngNum)
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> PVR ENG:%d\n",  __FILE__, __FUNCTION__,__LINE__,stResource->ePVREng));
            return E_PVRPL_RECORD_FAIL;
        }
    }

    PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_TRACE,printf("!!!!!!  Get Record Eng:%d Path:%d!!!!!\n",stResource->ePVREng,stResource->u8PathIdx));

    //Init stResource
    stResource->u8PVRPktMode = u8PKTMode;
    stResource->bRecAll = bRecAll;
    stResource->u32PausedTime = 0;
    stResource->u32RecordTime = 0;
    stResource->stDMXInfo.pPvrBuf0 = u32BufPhyAddr;
    stResource->stDMXInfo.u32PvrBufSize0 = (u32BufSize >> 1);
    stResource->stDMXInfo.pPvrBuf1 = u32BufPhyAddr + (u32BufSize >> 1);
    stResource->stDMXInfo.u32PvrBufSize1 = (u32BufSize >> 1);
    stResource->stDMXInfo.pNotify = NULL;
    stResource->stDMXInfo.u32Event = 0;

    if(stResource->u8PVRPktMode == 188)
    {
        printf("packet mode = 188 \n");

    #if (DEMO_PVR_SUPPORT_SWPVR_TEST)
        if(bActiveSWPVR == TRUE)
        {
            MApi_DMX_SWPVR_Eng_SetPacketMode((MS_U8)stResource->ePVREng,FALSE);
        }
        else
    #endif
        {
            MApi_DMX_Pvr_Eng_SetPacketMode(stResource->ePVREng,FALSE);
        }
    }
    else
    {
        printf("packet mode = 192 \n");

    #if (DEMO_PVR_SUPPORT_SWPVR_TEST)
        if(bActiveSWPVR == TRUE)
        {
            MApi_DMX_SWPVR_Eng_SetPacketMode((MS_U8)stResource->ePVREng,TRUE);
        }
        else
    #endif
        {
            MApi_DMX_Pvr_Eng_SetPacketMode(stResource->ePVREng,TRUE);
        }
    }

    DMX_Pvr_info stPVRInfo;
    stPVRInfo.pPvrBuf0      = (MS_PHYADDR)  stResource->stDMXInfo.pPvrBuf0;
    stPVRInfo.PvrBufSize0   = (MS_U32)      stResource->stDMXInfo.u32PvrBufSize0;
    stPVRInfo.pPvrBuf1      = (MS_PHYADDR)  stResource->stDMXInfo.pPvrBuf1;
    stPVRInfo.PvrBufSize1   = (MS_U32)      stResource->stDMXInfo.u32PvrBufSize1;
    stPVRInfo.pNotify       = (DMX_NotifyCb)stResource->stDMXInfo.pNotify;
    stPVRInfo.Event         = (DMX_EVENT)   stResource->stDMXInfo.u32Event;

#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        if(DMX_FILTER_STATUS_OK != MApi_DMX_SWPVR_Eng_Open((MS_U8)stResource->ePVREng,&stPVRInfo))
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Eng_Open() or MApi_DMX_SWPVR_Eng_Open()\n",  __FILE__, __FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_FAIL;
        }
    }
    else
#endif
    {
        if (DMX_FILTER_STATUS_OK != MApi_DMX_Pvr_Eng_Open(stResource->ePVREng,&stPVRInfo))
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Eng_Open() or MApi_DMX_SWPVR_Eng_Open()\n",  __FILE__, __FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_FAIL;
        }
    }

#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        MApi_DMX_SWPVR_Eng_SetRecordStamp((MS_U8)stResource->ePVREng,0);
    }
    else
#endif
    {
        MApi_DMX_Pvr_Eng_SetRecordStamp(stResource->ePVREng,0);
    }

#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        u8RecordSWEngNum++;
    }
    else
#endif
    {
        //TSP ECO Mode Setup
        MDrv_DSCMB2_PVR_RecCtrl(0, FALSE);

        #if (DEMO_PVR_V4_TEST == 1)
        MApi_DMX_PVR_FlowSet(stResource->ePVREng,stResource->eTsif,FALSE);
        #endif
    }

    stResource->bInit = TRUE;
    int i = 0;
    printf("[%s][%d]\n",__FUNCTION__,__LINE__);
    for (i = 0 ; i < 128 ; i++)
    {
        _PVRRecFilters[i].u8FilterID = INVALID_FILTER_ID;
        _PVRRecFilters[i].u32PID = INVALID_PID;
    }
    return E_PVRPL_RECORD_SUCCESS;
}

EN_PVRPL_RECORD_STATUS PVRPL_Record_EnableRecordEng(RecordResource *stResource,MS_BOOL bEnable)
{
    //Start
    if(bEnable == TRUE)
    {
    #if (DEMO_PVR_SUPPORT_SWPVR_TEST)
        if(bActiveSWPVR == TRUE)
        {
            if(MApi_DMX_SWPVR_Eng_Start((MS_U8)stResource->ePVREng)==DMX_FILTER_STATUS_ERROR)
            {
                PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] MApi_DMX_PVR_Eng_Start Fail\n",  __FILE__, __FUNCTION__,__LINE__));
                return E_PVRPL_RECORD_FAIL;
            }
        }
        else
    #endif
        {
            if(MApi_DMX_Pvr_Eng_Start(stResource->ePVREng,stResource->bRecAll)==DMX_FILTER_STATUS_ERROR)
            {
                PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] MApi_DMX_PVR_Eng_Start Fail\n",  __FILE__, __FUNCTION__,__LINE__));
                return E_PVRPL_RECORD_FAIL;
            }
        }
    }
    //Stop
    else
    {
    #if (DEMO_PVR_SUPPORT_SWPVR_TEST)
        if(bActiveSWPVR == TRUE)
        {
            if(MApi_DMX_SWPVR_Eng_Stop((MS_U8)stResource->ePVREng)==DMX_FILTER_STATUS_ERROR)
            {
                PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] MApi_DMX_PVR_Eng_Stop Fail\n",  __FILE__, __FUNCTION__,__LINE__));
                return E_PVRPL_RECORD_FAIL;
            }
        }
        else
    #endif
        {
            if(MApi_DMX_Pvr_Eng_Stop(stResource->ePVREng)==DMX_FILTER_STATUS_ERROR)
            {
                PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] MApi_DMX_PVR_Eng_Stop Fail\n",  __FILE__, __FUNCTION__,__LINE__));
                return E_PVRPL_RECORD_FAIL;
            }
        }
     }

    PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_TRACE,printf("[%s][%s][%d] RecordEng:%d bEnable:%d \n",__FILE__, __FUNCTION__,__LINE__,stResource->ePVREng,bEnable));
    return E_PVRPL_RECORD_SUCCESS;
}

EN_PVRPL_RECORD_STATUS PVRPL_Record_EnableRecordPause(RecordResource *stResource)
{
    //Pause
#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        MApi_DMX_SWPVR_Eng_GetRecordStamp((MS_U8)stResource->ePVREng,&(stResource->u32PausedTime));
        MApi_DMX_SWPVR_Eng_Stop((MS_U8)stResource->ePVREng);
    }
    else
#endif
    {
        MApi_DMX_Pvr_Eng_GetRecordStamp(stResource->ePVREng,&(stResource->u32PausedTime));
        MApi_DMX_Pvr_Eng_Stop(stResource->ePVREng);
    }

    PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_TRACE,printf("[%s][%s][%d] Pause Record\n",  __FILE__, __FUNCTION__,__LINE__));
    return E_PVRPL_RECORD_SUCCESS;
}

EN_PVRPL_RECORD_STATUS PVRPL_Record_EnableRecordResume(RecordResource *stResource)
{
    MS_U32 u32TmpTime = 0;

//get record time
#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        MApi_DMX_SWPVR_Eng_GetRecordStamp((MS_U8)stResource->ePVREng,&u32TmpTime);
    }
    else
#endif
    {
        MApi_DMX_Pvr_Eng_GetRecordStamp(stResource->ePVREng,&u32TmpTime);
    }

    if(stResource->u32PausedTime > u32TmpTime)
    {
        PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] pause time error => pause time:%"DTC_MS_U32_d" current time:%"DTC_MS_U32_d"\n",  __FILE__, __FUNCTION__,__LINE__,stResource->u32PausedTime,u32TmpTime));
        return E_PVRPL_RECORD_FAIL;
    }

//reset record time
#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        MApi_DMX_SWPVR_Eng_SetRecordStamp((MS_U8)stResource->ePVREng,stResource->u32PausedTime);
    }
    else
#endif
    {
        MApi_DMX_Pvr_Eng_SetRecordStamp(stResource->ePVREng,stResource->u32PausedTime);
    }

//start record eng
#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        if (DMX_FILTER_STATUS_OK !=MApi_DMX_SWPVR_Eng_Start((MS_U8)stResource->ePVREng))
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_PVR_Eng_Start()!\n",  __FILE__, __FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_FAIL;
        }
    }
    else
#endif
    {
        if (DMX_FILTER_STATUS_OK !=MApi_DMX_Pvr_Eng_Start(stResource->ePVREng,stResource->bRecAll))
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_PVR_Eng_Start()!\n",  __FILE__, __FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_FAIL;
        }
    }

    PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_TRACE,printf("[%s][%s][%d] Resume Record\n",  __FILE__, __FUNCTION__,__LINE__));
    return E_PVRPL_RECORD_SUCCESS;
}

EN_PVRPL_RECORD_STATUS PVRPL_Record_GetRecordTime(RecordResource *stResource,MS_U32 *pu32RecordTime)
{

#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        if (DMX_FILTER_STATUS_OK != MApi_DMX_SWPVR_Eng_GetRecordStamp((MS_U8)stResource->ePVREng,pu32RecordTime))
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_PVR_Eng_GetRecordStamp()!\n",  __FILE__, __FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_FAIL;
        }
    }
    else
#endif
    {
        if (DMX_FILTER_STATUS_OK != MApi_DMX_Pvr_Eng_GetRecordStamp(stResource->ePVREng,pu32RecordTime))
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_PVR_Eng_GetRecordStamp()!\n",  __FILE__, __FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_FAIL;
        }
    }

    PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_TRACE,printf("[%s][%s][%d] Get Record Time:%"DTC_MS_U32_d"\n",  __FILE__, __FUNCTION__,__LINE__,(*pu32RecordTime)));
    return E_PVRPL_RECORD_SUCCESS;
}

EN_PVRPL_RECORD_STATUS PVRPL_Record_SetRecordTime(RecordResource *stResource,MS_U32  u32RecordTime)
{

#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        if (DMX_FILTER_STATUS_OK != MApi_DMX_SWPVR_Eng_SetRecordStamp((MS_U8)stResource->ePVREng,u32RecordTime))
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_PVR_Eng_SetRecordStamp()!\n",  __FILE__, __FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_FAIL;
        }
    }
    else
#endif
    {
        if (DMX_FILTER_STATUS_OK != MApi_DMX_Pvr_Eng_SetRecordStamp(stResource->ePVREng,u32RecordTime))
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_PVR_Eng_SetRecordStamp()!\n",  __FILE__, __FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_FAIL;
        }
    }

    PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_TRACE,printf("[%s][%s][%d] Set Record Time:%"DTC_MS_U32_d"\n",  __FILE__, __FUNCTION__,__LINE__,u32RecordTime));
    return E_PVRPL_RECORD_SUCCESS;
}

EN_PVRPL_RECORD_STATUS PVRPL_Record_ResetRecordEng(RecordResource *stResource)
{
    DMX_Pvr_info stPVRInfo;
    stPVRInfo.pPvrBuf0      = (MS_PHYADDR)  stResource->stDMXInfo.pPvrBuf0;
    stPVRInfo.PvrBufSize0   = (MS_U32)      stResource->stDMXInfo.u32PvrBufSize0;
    stPVRInfo.pPvrBuf1      = (MS_PHYADDR)  stResource->stDMXInfo.pPvrBuf1;
    stPVRInfo.PvrBufSize1   = (MS_U32)      stResource->stDMXInfo.u32PvrBufSize1;
    stPVRInfo.pNotify       = (DMX_NotifyCb)stResource->stDMXInfo.pNotify;
    stPVRInfo.Event         = (DMX_EVENT)   stResource->stDMXInfo.u32Event;

#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        MApi_DMX_SWPVR_Eng_GetRecordStamp((MS_U8)stResource->ePVREng,&(stResource->u32PausedTime));
    }
    else
#endif
    {
        MApi_DMX_Pvr_Eng_GetRecordStamp(stResource->ePVREng,&(stResource->u32PausedTime));
    }


#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        MApi_DMX_SWPVR_Eng_Stop((MS_U8)stResource->ePVREng);
    }
    else
#endif
    {
        MApi_DMX_Pvr_Eng_Stop(stResource->ePVREng);
    }

#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        MApi_DMX_SWPVR_Eng_Open((MS_U8)stResource->ePVREng,&stPVRInfo);
    }
    else
#endif
    {
        MApi_DMX_Pvr_Eng_Open(stResource->ePVREng,&stPVRInfo);
    }

    if(stResource->u8PVRPktMode == 192)
    {
#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
        if(bActiveSWPVR == TRUE)
        {
            MApi_DMX_SWPVR_Eng_SetPacketMode((MS_U8)stResource->ePVREng,TRUE);
        }
        else
#endif
        {
            MApi_DMX_Pvr_Eng_SetPacketMode(stResource->ePVREng,TRUE);
        }
    }
    else
    {
#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
        if(bActiveSWPVR == TRUE)
        {
            MApi_DMX_SWPVR_Eng_SetPacketMode((MS_U8)stResource->ePVREng,FALSE);
        }
        else
#endif
        {
            MApi_DMX_Pvr_Eng_SetPacketMode(stResource->ePVREng,FALSE);
        }
    }

    //reset record time
#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        MApi_DMX_SWPVR_Eng_SetRecordStamp((MS_U8)stResource->ePVREng,stResource->u32PausedTime);
    }
    else
#endif
    {
        MApi_DMX_Pvr_Eng_SetRecordStamp(stResource->ePVREng,stResource->u32PausedTime);
    }

    //start record eng
#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        MApi_DMX_SWPVR_Eng_Start((MS_U8)stResource->ePVREng);
    }
    else
#endif
    {
        MApi_DMX_Pvr_Eng_Start(stResource->ePVREng,stResource->bRecAll);
    }

    PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_TRACE,printf("[%s][%s][%d] Reset Record Eng\n",  __FILE__, __FUNCTION__,__LINE__));
    return E_PVRPL_RECORD_SUCCESS;
}


static EN_PVRPL_RECORD_STATUS _PVRPL_Record_PVRPIDFilterControl(RecordResource *stResource,MS_BOOL bAddDelete, MS_U8 *pu8PVRFltID, MS_U32 u32PID)
{
    if(bAddDelete == TRUE)
    {
    #if (DEMO_PVR_SUPPORT_SWPVR_TEST)
        if(bActiveSWPVR == TRUE)
        {
            if (DMX_FILTER_STATUS_OK != MApi_DMX_SWPVR_Eng_Pid_Open((MS_U8)stResource->ePVREng,u32PID,pu8PVRFltID))
            {
                PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Pvr_Eng_Pid_Open()!\n",  __FILE__, __FUNCTION__,__LINE__));
                return E_PVRPL_RECORD_FAIL;
            }
        }
        else
    #endif
        {
            if (DMX_FILTER_STATUS_OK != MApi_DMX_Pvr_Eng_Pid_Open(stResource->ePVREng,u32PID,pu8PVRFltID))
            {
                PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Pvr_Eng_Pid_Open()!\n",  __FILE__, __FUNCTION__,__LINE__));
                return E_PVRPL_RECORD_FAIL;
            }
            PVRPL_Record_SetFilters(*pu8PVRFltID,u32PID);
        }

    }
    else
    {
    #if (DEMO_PVR_SUPPORT_SWPVR_TEST)
        if(bActiveSWPVR == TRUE)
        {
            if (DMX_FILTER_STATUS_OK != MApi_DMX_SWPVR_Eng_PID_Close((MS_U8)stResource->ePVREng,(*pu8PVRFltID)))
            {
                PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Pvr_Eng_Pid_Close()!\n",  __FILE__, __FUNCTION__,__LINE__));
                return E_PVRPL_RECORD_FAIL;
            }
        }
        else
    #endif
        {
            if (DMX_FILTER_STATUS_OK != MApi_DMX_Pvr_Eng_Pid_Close(stResource->ePVREng,(*pu8PVRFltID)))
            {
                PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Pvr_Eng_Pid_Close()!\n",  __FILE__, __FUNCTION__,__LINE__));
                return E_PVRPL_RECORD_FAIL;
            }
        }
    }

    PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_TRACE,printf("[%s][%s][%d] AddDelete:%d PVRFltID:%d PID:0x%"DTC_MS_U32_x"\n",  __FILE__,__FUNCTION__,__LINE__,bAddDelete,*pu8PVRFltID,u32PID));
    return E_PVRPL_RECORD_SUCCESS;
}

EN_PVRPL_RECORD_STATUS PVRPL_Record_DMX_PVR_PIDFilterControl(RecordResource *stResource,MS_BOOL bAddDelete,MS_U8 *pu8FltID,MS_U16 u16PID,DMX_FILTER_TYPE eDMXFltType)
{
    //open pvr flt
    if(bAddDelete == TRUE)
    {
        if(E_PVRPL_RECORD_FAIL ==_PVRPL_Record_PVRPIDFilterControl(stResource,bAddDelete,pu8FltID,(MS_U32)u16PID))
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] Open PVR Flt Fail\n",__FILE__,__FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_FAIL;
        }

    }
    //close pvr flt
    else
    {
        if(E_PVRPL_RECORD_FAIL ==_PVRPL_Record_PVRPIDFilterControl(stResource,bAddDelete,pu8FltID,(MS_U32)u16PID))
        {
            return E_PVRPL_RECORD_FAIL;
        }
    }

    return E_PVRPL_RECORD_SUCCESS;

}

EN_PVRPL_RECORD_STATUS PVRPL_Record_GetRecordAddr(RecordResource *stResource,MS_U32 *pu32PhyAddr)
{

#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        if (DMX_FILTER_STATUS_OK != MApi_DMX_SWPVR_Eng_WriteGet((MS_U8)stResource->ePVREng,pu32PhyAddr))
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Pvr_Eng_WriteGet()!\n",  __FILE__, __FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_FAIL;
        }
    }
    else
#endif
    {
        MS_PHY PHYAddr = 0;
        if (DMX_FILTER_STATUS_OK != MApi_DMX_Pvr_Eng_WriteGet(stResource->ePVREng,&PHYAddr))
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Pvr_Eng_WriteGet()!\n",  __FILE__, __FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_FAIL;
        }
        PVRPL_UTOPIA64BIT_MASK(*pu32PhyAddr,PHYAddr);
    }

    // for debug
    /*
    MS_U32 video_fifo1 = 0;
    MS_U32 audio_fifo1 = 0;
    MS_U32 video_fifo0 = 0;
    MS_U32 audio_fifo0 = 0;
    MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE_VIDEO, &video_fifo0);
    MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE_AUDIO, &audio_fifo0);
    MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE_VIDEO3D, &video_fifo1);
    MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE_AUDIO2, &audio_fifo1);
    printf("VIDEO State: %lu  AUDIO State: %lu\n", video_fifo0, audio_fifo0);
    printf("VIDEO LEVEL: %lu  AUDIO LEVEL: %lu\n", (video_fifo0& DMX_FIFO_STATUS_LV_USAGE_MASK)>> DMX_FIFO_STATUS_LV_USAGE_SHIFT, (audio_fifo0& DMX_FIFO_STATUS_LV_USAGE_MASK)>> DMX_FIFO_STATUS_LV_USAGE_SHIFT);
    printf("VIDEO EMPTY: %lu  AUDIO EMPTY: %lu\n", video_fifo0&DMX_FIFO_STATUS_LV_EMPTY, audio_fifo0&DMX_FIFO_STATUS_LV_EMPTY);
    printf("VIDE1 State: %lu  AUDI1 State: %lu\n", video_fifo1, audio_fifo1);
    printf("VIDE1 LEVEL: %lu  AUDI1 LEVEL: %lu\n", (video_fifo1& DMX_FIFO_STATUS_LV_USAGE_MASK)>> DMX_FIFO_STATUS_LV_USAGE_SHIFT, (audio_fifo1& DMX_FIFO_STATUS_LV_USAGE_MASK)>> DMX_FIFO_STATUS_LV_USAGE_SHIFT);
    printf("VIDE1 EMPTY: %lu  AUDI1 EMPTY: %lu\n", video_fifo1& DMX_FIFO_STATUS_LV_EMPTY, audio_fifo1&DMX_FIFO_STATUS_LV_EMPTY);
    */
    PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_TRACE,printf("[%s][%s][%d] Get RecordAddr:0x%"DTC_MS_U32_x" \n",  __FILE__, __FUNCTION__,__LINE__,(*pu32PhyAddr)));
    return E_PVRPL_RECORD_SUCCESS;
}


EN_PVRPL_RECORD_STATUS PVRPL_Record_Exit(RecordResource *stResource)
{

#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        if (DMX_FILTER_STATUS_OK != MApi_DMX_SWPVR_Eng_Stop((MS_U8)stResource->ePVREng))
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Pvr_Eng_WriteGet()!\n",  __FILE__, __FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_FAIL;
        }
    }
    else
#endif
    {
        if (DMX_FILTER_STATUS_OK != MApi_DMX_Pvr_Eng_Stop(stResource->ePVREng))
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Pvr_Eng_WriteGet()!\n",  __FILE__, __FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_FAIL;
        }
    }


#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        if (DMX_FILTER_STATUS_OK != MApi_DMX_SWPVR_Eng_Close((MS_U8)stResource->ePVREng))
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Pvr_Eng_WriteGet()!\n",  __FILE__, __FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_FAIL;
        }
    }
    else
#endif
    {
        if (DMX_FILTER_STATUS_OK != MApi_DMX_Pvr_Eng_Close(stResource->ePVREng))
        {
            PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_ERR,printf("[%s][%s][%d] <ERR> Fail to MApi_DMX_Pvr_Eng_WriteGet()!\n",  __FILE__, __FUNCTION__,__LINE__));
            return E_PVRPL_RECORD_FAIL;
        }
    }

#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
    if(bActiveSWPVR == TRUE)
    {
        u8RecordSWEngNum--;
        if(u8RecordSWEngNum == 0)
        {
            bActiveSWPVR = FALSE;
        }
    }
#endif

    stResource->bInit = FALSE;
    PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_TRACE,printf("[%s][%s][%d] Exit \n",  __FILE__, __FUNCTION__,__LINE__));
    return E_PVRPL_RECORD_SUCCESS;
}

#if (DEMO_PVR_SUPPORT_SWPVR_TEST)
MS_BOOL _PVRPL_Record_SWPVR_Init(MS_U32 u32PhyAdr, MS_U32 u32Size, MS_U32 u32SWEngNum)
{

    SWPVR_ThreadInfo    sThreadInfo;
    SWPVR_EventInfo     sEventInfo;
    SWPVR_BufferInfo    sBufInfo;

    sEventInfo.pNotify  = NULL;
    sEventInfo.eEvent   = SWPVR_EVENT_NONE;

    sThreadInfo.u32ParserDelayTime = 10; //ms
    sThreadInfo.eParserPriority = E_TASK_PRI_HIGH;

    sBufInfo.pSWPVRBuf = u32PhyAdr;
    sBufInfo.u32SWPVRBufSize = u32Size;

    //@NOTE
    //Not supporting change SWPVR and HWPVR mode
    if(MApi_DMX_SWPVR_Init(&sThreadInfo,&sEventInfo,&sBufInfo,u32SWEngNum,NULL) == DMX_FILTER_STATUS_OK)
    {
        bActiveSWPVR = TRUE;

    #if (DEMO_PVR_V4_TEST == 1)
        DMX_FLOW_INPUT eDmxInput;
        eSWPVRTsif = Pool_GetSource(&eDmxInput);
        eSWPVREng = E_PVRPL_DMX_PVR_EGN0;
        //@NOTE Taking PVR Eng0 to run SWPVR
        //Flowset for SWPVR
        //for TSP2
        //@TODO There is a bug which SWPVR can't record data with calling MApi_DMX_FlowSet and MApi_DMX_PVR_FlowSet concurrently
        //MDrv_DSCMB2_PVR_RecCtrl(0, FALSE);
        //MApi_DMX_FlowSet(DMX_FLOW_PVR,eDmxInput,FALSE,FALSE,TRUE);

        //for TSP4
        //@NOTE Only support same frequency recording
        MApi_DMX_PVR_FlowSet(eSWPVREng,eSWPVRTsif,FALSE);
    #endif

        PVRPL_RECORD_DBGMSG(E_PVRPL_RECORD_DBG_TRACE,printf("[%s][%s][%d]Active SWPVR!!!\n",__FILE__,__FUNCTION__,__LINE__));

        return TRUE;
    }
    else
    {
        bActiveSWPVR = FALSE;
        return FALSE;
    }
}
#endif

MS_BOOL PVRPL_Record_SetFilters(MS_U8 FilterID, MS_U32 PID)
{
    int i = 0;
    if (FilterID == INVALID_FILTER_ID || PID == INVALID_PID)
    {
        printf("Invalid Parameters : FilterID%d PID:%d",FilterID,PID);
        return FALSE;
    }

    for (i = 0 ; i < 128 ; i++)
    {
        if (_PVRRecFilters[i].u8FilterID == INVALID_FILTER_ID 
            && _PVRRecFilters[i].u32PID == INVALID_PID)
        {
            break;
        }
    }
    
    if (i == 128)
    {
        printf("Filter sets buffer already full\n");
        return FALSE;
    }
    _PVRRecFilters[i].u8FilterID = FilterID;
    _PVRRecFilters[i].u32PID = PID;
    return TRUE;
}

PVRPL_Rec_Filters *PVRPL_Record_GetFilters(void)
{
    int i = 0;
    for (i = 0 ; i < 128 ; i++)
    {
        if (_PVRRecFilters[i].u8FilterID == INVALID_FILTER_ID 
            && _PVRRecFilters[i].u32PID == INVALID_PID)
        {
            break;
        }
    }
    return _PVRRecFilters;
}
