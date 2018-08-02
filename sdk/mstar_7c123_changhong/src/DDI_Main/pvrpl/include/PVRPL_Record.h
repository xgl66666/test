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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   PVRPL_Record.h
/// @brief  CPL API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __API_PVRPL_Record_H__
#define __API_PVRPL_Record_H__

#include "MsCommon.h"
#include "PVRPL_Common.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define RECORD_ERROR_Handler 0xff


typedef enum
{
    E_PVRPL_RECORD_DBG_NONE,
    E_PVRPL_RECORD_DBG_ERR,
    E_PVRPL_RECORD_DBG_WARN,
    E_PVRPL_RECORD_DBG_INFO,
    E_PVRPL_RECORD_DBG_FUNC,
    E_PVRPL_RECORD_DBG_TRACE,
}EN_PVRPL_RECORD_DBGMSG_LEVEL;


typedef enum
{
 E_PVRPL_RECORD_SUCCESS,
 E_PVRPL_RECORD_FAIL,
 E_PVRPL_RECORD_ACTIVE_SWPVR_SUCCESS,
}EN_PVRPL_RECORD_STATUS;


typedef struct
{
    MS_U8 u8FltID;
    MS_U32 u32PID;

}PVRFlt;

//This enum. is mapped to DMX_PVR_ENG
typedef enum
{
    E_PVRPL_DMX_PVR_EGN0,
    E_PVRPL_DMX_PVR_EGN1,
    E_PVRPL_DMX_PVR_EGN2,
    E_PVRPL_DMX_PVR_EGN3,
    E_PVRPL_DMX_PVR_EGN4,
    E_PVRPL_DMX_PVR_EGN5,
    E_PVRPL_DMX_PVR_ENG_NUM,
} EN_PVRPL_DMX_PVR_ENG;

//This enum. is mapped to DMX_TSIF
typedef enum
{
    E_PVRPL_DMX_TSIF_LIVE0,
    E_PVRPL_DMX_TSIF_LIVE1,
    E_PVRPL_DMX_TSIF_LIVE2,
    E_PVRPL_DMX_TSIF_LIVE3,
    E_PVRPL_DMX_TSIF_FILE0,
    E_PVRPL_DMX_TSIF_FILE1,
    E_PVRPL_DMX_TSIF_FILE2,
    E_PVRPL_DMX_TSIF_FILE3,
    E_PVRPL_DMX_TSIF_MAX,
} EN_PVRPL_DMX_TSIF;

typedef struct
{
    MS_PHYADDR                  pPvrBuf0;
    MS_PHYADDR                  pPvrBuf1;
    MS_U32                      u32PvrBufSize0;
    MS_U32                      u32PvrBufSize1;
    MS_U32                      u32Event;
    void*                       pNotify;
    MS_U16                      u16CaPvrModel;
    MS_BOOL                     bSpsEnable;
} PVRPL_DMX_Pvr_info;

typedef struct
{
    //FileEng Info
    EN_PVRPL_DMX_PVR_ENG    ePVREng;
    EN_PVRPL_DMX_TSIF       eTsif;
    PVRPL_DMX_Pvr_info      stDMXInfo;
    MS_BOOL                 bRecAll;
    MS_U8                   u8PVRPktMode;
    MS_U32                  u32PausedTime;
    MS_U32                  u32RecordTime;
    PVR_PATH                u8PathIdx;
    MS_BOOL                 bInit;
}RecordResource;

typedef struct PVRPL_Rec_Filters_t
{
    MS_U8 u8FilterID;
    MS_U32 u32PID;
} PVRPL_Rec_Filters;
#define INVALID_PID 0x1FFF
#define INVALID_FILTER_ID                       0xFF
#define PVRPL_SWPVR_INDICATOR_MASK  0x80

EN_PVRPL_RECORD_STATUS PVRPL_Record_Init(RecordResource *stResource,MS_U8 u8PKTMode,MS_U32 u32BufPhyAddr,MS_U32 u32BufSize,MS_BOOL bRecAll);
EN_PVRPL_RECORD_STATUS PVRPL_Record_DMX_PVR_PIDFilterControl(RecordResource *stResource,MS_BOOL bAddDelete,MS_U8 *pu8FltID,MS_U16 u16PID,DMX_FILTER_TYPE eDMXFltType);
EN_PVRPL_RECORD_STATUS PVRPL_Record_GetRecordAddr(RecordResource *stResource,MS_U32 *pu32PhyAddr);
EN_PVRPL_RECORD_STATUS PVRPL_Record_GetRecordTime(RecordResource *stResource,MS_U32 *pu32RecordTime);
EN_PVRPL_RECORD_STATUS PVRPL_Record_SetRecordTime(RecordResource *stResource,MS_U32  u32RecordTime);
EN_PVRPL_RECORD_STATUS PVRPL_Record_EnableRecordPause(RecordResource *stResource);
EN_PVRPL_RECORD_STATUS PVRPL_Record_EnableRecordResume(RecordResource *stResource);
EN_PVRPL_RECORD_STATUS PVRPL_Record_EnableRecordEng(RecordResource *stResource,MS_BOOL bEnable);
EN_PVRPL_RECORD_STATUS PVRPL_Record_ResetRecordEng(RecordResource *stResource);
EN_PVRPL_RECORD_STATUS PVRPL_Record_Exit(RecordResource *stResource);
MS_BOOL PVRPL_Record_SetFilters(MS_U8 FilterID, MS_U32 PID);
PVRPL_Rec_Filters *PVRPL_Record_GetFilters(void);



#ifdef __cplusplus
}
#endif

#endif // #ifndef __API_PVRPL_Record__
