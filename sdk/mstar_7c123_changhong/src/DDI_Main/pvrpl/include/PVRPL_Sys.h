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
/// @file   PVRPL_Audio.h
/// @brief  CPL API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __API_PVRPL_SYS_H__
#define __API_PVRPL_SYS_H__

#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif

/// PVR CPL status indicator
typedef enum
{
    PVRPL_SYS_STATUS_OK,               ///< PVR CPL status OK
    PVRPL_SYS_STATUS_ERROR,            ///< PVR CPL status ERROR
} PVRPL_SYS_STATUS;

typedef enum
{
    EN_PVRPL_MMAP_ID_DOWNLOAD = 0,
    EN_PVRPL_MMAP_ID_UPLOAD,
    EN_PVRPL_MMAP_ID_BUFFER,
    EN_PVRPL_MMAP_ID_THUMBNAIL,
    EN_PVRPL_MMAP_ID_BROWSER_BUFFER,
    EN_PVRPL_MMAP_ID_SWBUFFER,
///[[[ ES Player
    EN_PVRPL_MMAP_ID_ES_VIDEO,
    EN_PVRPL_MMAP_ID_ES_AUDIO,
///]]]
    EN_PVRPL_MMAP_ID_MAX,
} PVRPL_MMAP_ID;


typedef enum
{
    EN_PVRPL_DECRYPT,
    EN_PVRPL_ENCRYPT
} enPLAESDMAMode;

typedef enum
{
    EN_PVRPL_ENCRYPT_NONE    = 0,
    EN_PVRPL_ENCRYPT_DEFAULT = 1,//default-defination  key
    EN_PVRPL_ENCRYPT_CIPLUS  = 2,//not support for now
    EN_PVRPL_ENCRYPT_USER    = 3,//user-defination key
    EN_PVRPL_ENCRYPT_SMARTCARD  = 4,//Key-ladder generated key
    EN_PVRPL_ENCRYPT_AESDMA_NUM,//AESDMA(Block) Encryption above
    //!!!Notice: Before release record clear stream lib or source code to customer, need follow formal release rule!!!//
    //!!!for details, please contact the HQ PVR module owner about the release flow!!!//
    EN_PVRPL_ENCRYPT_CLEAR  = 0x100,//record to clear from scrambled, no encryption anymore
    EN_PVRPL_ENCRYPT_MAX,
} PVRPL_ENCRYPT_TYPE;

typedef enum
{
    EN_PVRPL_CREATE_EVENTGROUP = 0,
    EN_PVRPL_DELELTE_EVENTGROUP,
    EN_PVRPL_SET_EVENT,
    EN_PVRPL_CLEAR_EVENT,
    EN_PVRPL_WAIT_EVENT
} PVRPL_EVENT_CMD;

// Sys DBG enum
typedef enum
{
    PVRPLSYS_DBG_NONE,
    PVRPLSYS_DBG_ERR,
    PVRPLSYS_DBG_WARN,
    PVRPLSYS_DBG_INFO,
    PVRPLSYS_DBG_FUNC,
    PVRPLSYS_DBG_TRACE,
} PVRPLSYS_DBGMSG_LEVEL;
//--------------------------------------------------------

typedef struct
{
    char* Pattern;
    char* ExcludeBit;
    MS_U32 u32PatternLength;
} ST_INFO_PATTERNSEARCH;

PVRPL_SYS_STATUS PVRPL_SysInit(void);
MS_U32 PVRPL_GetSysTime(void);
PVRPL_SYS_STATUS PVRPL_PA2VA(MS_U32 u32PhyAdr, MS_U32* u32VirAdr);
PVRPL_SYS_STATUS PVRPL_VA2PA(MS_U32 u32VirAdr, MS_U32* u32PhyAdr);
PVRPL_SYS_STATUS PVRPL_SleepNS(MS_U32 u32NanoSec);
PVRPL_SYS_STATUS PVRPL_SleepMS(MS_U32 u32MSec);
//PVR_MMapInfo_t* PVRPL_GetMmap(PVRPL_MMAP_ID id);
//void* PVRPL_SetMmap(PVRPL_MMAP_ID id,MS_U32 u32Addr,MS_U32 u32Size);

//Below 3 functions is enable AESDMA

PVRPL_SYS_STATUS PVRPL_SetAESDMAEnabled(MS_BOOL bAESDMAEnabled);

PVRPL_SYS_STATUS PVRPL_AESDMAEncryptDecrypt(MS_U32 u32Address, MS_U32 u32Length,
            enPLAESDMAMode eMode, PVRPL_ENCRYPT_TYPE enEncryptType);
PVRPL_SYS_STATUS PVRPL_AESDMASetKey(PVRPL_ENCRYPT_TYPE enEncryptType,
            MS_U32 u32Key1, MS_U32 u32Key2, MS_U32 u32Key3, MS_U32 u32Key4);
PVRPL_SYS_STATUS PVRPL_AESDMAGetKey(PVRPL_ENCRYPT_TYPE enEncryptType,
            MS_U32 *u32Key1, MS_U32 *u32Key2, MS_U32 *u32Key3, MS_U32 *u32Key4);

MS_U32 PVRPL_GetAESDMABufLength(void);

PVRPL_SYS_STATUS PVRPL_MemoryCopy(MS_U32 u32PhySrcAdr,MS_U32 u32PhyDstAdr,MS_U32 u32Len);
PVRPL_SYS_STATUS PVRPL_MemoryFlush(MS_U32 u32VirSrcAdr,MS_U32 u32DataSize);
//PVRPL_STATUS InsertInterrupt(InterruptNum eIntNum, InterruptCb pIntCb);
//PVRPL_STATUS DeleteInterrupt(InterruptNum eIntNum);
void* PVRPL_AllocMem(MS_U32 u32Size);
PVRPL_SYS_STATUS PVRPL_FreeMem(void *pBuf);
PVRPL_SYS_STATUS PVRPL_CreateMutex(MS_S32 *ps32MutexID, MS_U8 *pu8MutexName);
PVRPL_SYS_STATUS PVRPL_ObtainMutex(MS_S32 s32MutexId);
PVRPL_SYS_STATUS PVRPL_ReleaseMutex(MS_S32 s32MutexId);
PVRPL_SYS_STATUS PVRPL_DeleteMutex(MS_S32 s32MutexId);
PVRPL_SYS_STATUS PVRPL_EventCMD(PVRPL_EVENT_CMD EventCMD,MS_U8 *pName,MS_S32 *s32EventId,MS_U32 u32EventFlag);
MS_U32 PVRPL_PatternSearch(MS_U32 u32VirAdr, MS_U32 u32SearchLength, ST_INFO_PATTERNSEARCH PatternInfo, void* pParam);


#ifdef __cplusplus
}
#endif

#endif // #ifndef __API_PVRPL_SYS_H__
