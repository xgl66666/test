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
// File Name:
//		msAPI_Subtitle_h
// Description:
//		DVB subtitle interface api.
// Note:
//		MStar Semi.
//-------------------------------------------------------------------------------------------------

#ifndef _MSAPI_DVB_SUBTITLE_H
#define _MSAPI_DVB_SUBTITLE_H

//#include "MsTypes.h"
#include "apiGFX.h"
#include "apiGOP.h"
#include "apiMsMWCommon.h"

#include "apiVDEC_EX.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    SBTL_LIVE_MODE,
    SBTL_FILE_MODE
} SUB_TSP_MODE;

typedef enum
{
    SBTL_OSD_DST_AUTO,
    SBTL_OSD_DST_IP,
    SBTL_OSD_DST_OP,
} SUB_OSD_DST;

typedef struct
{
    MS_U8 u8_R;
    MS_U8 u8_G;
    MS_U8 u8_B;
    MS_U8 u8_A;  // Transparency
} DvbSubPalette_Colour;


typedef enum
{
    SBTL_CONFIG_SD_ONLY, //param: (MS_BOOL*), TRUE or FALSE
} SUB_CONFIG;

typedef struct
{
    MS_U8 u8TspId;
    MS_U16 u16Pid;
    MS_U16 u16CPID;
    MS_U16 u16APDI;
}SUB_STREAM_INFO;

typedef int (*MsMW_SubOsdEnable)(int bEnable);
typedef int (*MsMW_SubOsdAlloc)(GFX_BufferInfo* pBufInfo, GFX_Buffer_Format ColorFmt, unsigned char* pu8Buf, unsigned int u16Width, unsigned int u16Height);
typedef int (*MsMW_SubOsdFree)(void);
typedef int (*MsMW_SubOsdResulotionSet)(unsigned int u16X, unsigned int u16Y, unsigned int u16W, unsigned int u16H,
                                            unsigned int u16ScaledW, unsigned int u16ScaledH, MS_BOOL bHScale, MS_BOOL bVScale);
typedef int (*MsMW_SubOsdReset)(EN_GOP_DST_TYPE eGOPDstType);
typedef int (*MsMW_SubOsdPaletteSet)(unsigned char* pPalArray, unsigned char u32PalCount, unsigned int u32PalOffset);
typedef int (*MsMW_SubOsdDestinationSet)(SUB_OSD_DST *pGOPDstType);
typedef int (*MsMW_SubOsdStreamIDSet)(SUB_STREAM_INFO *pStreamInfo, VDEC_StreamId *pStreamId);

typedef struct MsMW_SubOsdOp
{
    MsMW_SubOsdAlloc            pOsdAlloc;
    MsMW_SubOsdFree             pOsdFree;
    MsMW_SubOsdReset            pOsdReset;
    MsMW_SubOsdResulotionSet    pOsdResulotionSet;
    MsMW_SubOsdEnable           pOsdEnable;
    MsMW_SubOsdPaletteSet       pOsdPaletteSet;
    MsMW_SubOsdDestinationSet   pOsdDestinationSet;
    MsMW_SubOsdStreamIDSet      pOsdStreamIDSet;
} MsMW_SubOsdOp_t;

typedef struct MsMW_SubMemOp
{
    MsMW_Alloc                  pAllocCache;
    MsMW_Free                   pFreeCache;
    MsMW_Alloc                  pAllocNonCache;
    MsMW_Free                   pFreeNonCache;
    MsMW_VA2PA                  pVa2Pa;
    MsMW_PA2VA                  pPa2Va;
} MsMW_SubMemOp_t;

typedef int (*MsMW_SubTspFlowSet)(void);

extern void msAPI_Subtitle_Start(void);
extern void msAPI_Subtitle_Stop(void);
extern void msAPI_Subtitle_Clear(void);
extern void msAPI_Subtitle_EnableDisplay(MS_BOOL blEnableDisplay);
extern void msAPI_Subtitle_Init(MsMW_SubOsdOp_t* pOsdOp, MsMW_SubMemOp_t* pMemOp);
extern void msAPI_Subtitle_DeInit(void);
extern void msAPI_Subtitle_Reset(void);
extern void msAPI_Subtitle_SetGOPNum(MS_U8 u8GOP);
extern void msAPI_Subtitle_SetPageIDs( MS_U16 u16CPID, MS_U16 u16APID );
extern void msAPI_Subtitle_SetPid(MS_U16 u16Pid);
extern void msAPI_Subtitle_SetTSPMode(SUB_TSP_MODE eSetTSPMode);
extern void msAPI_Subtitle_SetDMXFltSrc(MS_U32 u32DmxFltSrc);
extern MS_BOOL msAPI_Subtitle_SetConfig(SUB_CONFIG eConfig, void *pSetValue);
extern MS_BOOL msAPI_Subtitle_SetDVBMode(MS_BOOL bDVB_Mode);
extern MS_BOOL msAPI_Subtitle_GetDVBMode(void);

#define GOP_RENDER 0

#ifdef __cplusplus
}
#endif

#endif // _MSAPI_SUBTITLE_H
