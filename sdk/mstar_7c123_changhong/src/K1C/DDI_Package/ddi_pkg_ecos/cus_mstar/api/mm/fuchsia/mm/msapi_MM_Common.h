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
#ifndef __MSAPI_MM_COMMOND_H__
#define __MSAPI_MM_COMMOND_H__


#include "MsMMTypes.h"
#include "mapp_photo_display.h"
#include "apiVDEC_EX.h"

typedef enum
{
    BUF_ID_ITC,                     ///< Buffer ID ITC
    BUF_ID_EVENT_INFO,              ///< Buffer ID Event Info
    BUF_ID_DESCRIPTOR,              ///< Buffer ID Descriptor
    BUF_ID_VCH,                     ///< Buffer ID VCH
    BUF_ID_PAT,                     ///< Buffer ID PAT
    BUF_ID_DTV_PG_INDEX,            ///< Buffer ID DTV PG Index
    BUF_ID_FLASH,                   ///< Buffer ID Flash
    BUF_ID_GLYPH_DISP_INFO,         ///< Buffer ID Glyph Display Info
    BUF_ID_DRAW_TEXT,               ///< Buffer ID Draw Text
    BUF_ID_SUBTITLE_SEC,            ///< Buffer ID Subtitle section buffer
    BUF_ID_MEM_REALLOC,             ///< Buffer ID Memory reallocate
    BUF_ID_AEON_MMAP,               ///< Buffer ID Aeon MMAP
    BUF_ID_FILEBROWER,              ///< Buffer ID Filebrowser
    BUF_ID_FILECNTL,                ///< Buffer ID File control
    BUF_ID_USB_DOWNLOAD,            ///< Buffer ID USB download
    BUF_ID_ZUI,                     ///< Buffer ID ZUI service
    BUF_ID_OAD_DOWNLOAD,            ///< Buffer ID OAD download
    BUF_ID_USB_HOST,                ///< Buffer ID USB Host
    BUF_ID_CC,                      ///< Buffer ID Closed-caption
    BUF_ID_ATSC,                    ///< Buffer ID ATSC
    BUF_ID_NIT_SCAN,                    ///< Buffer ID Netwok scan
    BUF_ID_NEW_MULTIPLEX,                   ///< Buffer ID new multiplex
    BUF_ID_MSB1210_DOWNLOAD,           ///< Buffer ID msb1210 download
} EN_BUFFER_ID;

#define __ASSERT(expr) (void)((expr) || printf("%s %d\n", __FILE__, __LINE__));

#define Swap16(x) ((x << 8) | (x >> 8))
#define Swap32(x) \
    ((x >> 24) | ((x & 0x00FF0000) >>  8) | \
     ((x & 0x0000FF00) <<  8) | (x << 24))

extern BOOLEAN msapi_Init_Mediacodec(void);
extern BOOLEAN msapi_Init_Mediacodec_EX(VDEC_StreamId *pStreamId);
extern BOOLEAN msapi_Exit_Mediacodec(void);
U16 msAPI_MIU_Copy(MS_PHYADDR srcaddr, MS_PHYADDR dstaddr, U32 len, MEMCOPYTYPE eType);
void msAPI_MIU_Clear(U8 miu, U32 addr, U32 len, U8 clear_value);
BOOLEAN msAPI_Flash_BlockErase(U32 u32StartBlock, U32 u32EndBlock, BOOLEAN bWait);
BOOLEAN msAPI_Flash_Write(U32 u32StartAddr, U32 u32WriteSize, U8 * user_buffer);
void msAPI_Memory_SetPoolID(S32 s32PoolID); // set Non-Cached Pool ID from AP code. ex:gs32NonCachedPoolID is from HB
S32 msAPI_Memory_GetPoolID(void);
void * msAPI_Memory_Allocate(U16 u16Number_of_bytes);
U8 msAPI_Memory_Free(void *pFree);
void *msAPI_Memory_ReAllocate(void *pAlloc, U16 newsize);
extern U32 msAPI_MM_Timer_GetTime0(void);
U32 msAPI_Timer_DiffTimeFromNow(U32 u32TaskTimer);


#endif
