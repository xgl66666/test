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

//-----------------------------------------------------------------------------
//
/// @file   mm_cfg_ini.c
/// @author MStar Semiconductor Inc.
/// @brief  the ini file of mm config
//
// @name Box App Command Usage
// @verbatim
//-----------------------------------------------------------------------------

#include "MsCommon.h"
#include "mm_cfg_ini.h"

char u8IniInfo[STRING_LENGTH];

#define END_OF_LINE "\n"
#define NEW_STRING(_str) \
    do{ \
        snprintf(u8IniInfo, STRING_LENGTH, "%s%s", _str, END_OF_LINE); \
    }while(0)
#define ADD_STRING(_str) \
    do{ \
        strncat(u8IniInfo, _str, STRING_LENGTH - strlen(u8IniInfo) -1); \
        strncat(u8IniInfo, END_OF_LINE, STRING_LENGTH - strlen(u8IniInfo) -1); \
    }while(0)

//#==================# ShareMemory Setting #==================#
//# Note:
//# allow dynamic add key by user,
//# ShareMemKeyNum: number of share memory key to be set.
//# ShareMemKeyK: The K'th key of share memory
//# Value should set as ((( ShareMemory enum ) << 32) | setting value )
//#
//# ---- ShareMemory enum : enum value ----
//# E_SHAREMEM_ENABLE_AUTOPLAY : 0x000003b2
//# E_SHAREMEM_ENABLE_DYNSCALING : 0x000002cd
//# E_SHAREMEM_DISABLE_FMP4_PREPARSING : 0x00000390
//# E_SHAREMEM_IS_SUPPORT_AUDIO_ONLY : 0x00000384
//# E_SHAREMEM_ENABLE_PAUSE_AT_PLAYEND : 0x000003ae
//# E_SHAREMEM_PRELOAD_SIZE : 0x00000310
//# E_SHAREMEM_READ_HEADER_REQUIRE_SIZE : 0x00000387
//# E_SHAREMEM_READ_HEADER_PROBE_OPTION : 0x00000388
//# E_SHAREMEM_ENABLE_ROUGH_INDEX_PARSING : 0x0000038c
//# (E_SHAREMEM_BUFFER_INFO+E_BUFFER_INFO_LAST_PTS_LEN) : 0x000002d9
//#
//#==================# Other Setting #==================#
//# TS_PreloadSize : pre-load size when container format already know as TS
//# TS_HeaderRequireSize : header required size already when container format  know as TS
//# TS_ProbeOption : Probe option when container format  know as TS

void MM_INI_setIniInfo(void)
{
memset(u8IniInfo, '\0', STRING_LENGTH);

//# normal
NEW_STRING("[app0]");
ADD_STRING("ShareMemKeyNum = 4");
//#E_SHAREMEM_IS_SUPPORT_AUDIO_ONLY = yes
ADD_STRING("ShareMemKey1 = 0x0000038400000001");
//#E_SHAREMEM_ENABLE_DIRECTLY_SEEK = yes
ADD_STRING("ShareMemKey2 = 0x000003e100000001");
//#E_SHAREMEM_ENABLE_PAUSE_AT_PLAYEND
ADD_STRING("ShareMemKey3 = 0x000003ae00000001");
//#E_SHAREMEM_EXT_FRAME_CONTROL_DEBUG_MESSAGE_DURATION
ADD_STRING("ShareMemKey4 = 0x001a0000000005dc");

//Force 8bit mode
ADD_STRING("[Force_8bit]");
ADD_STRING("ShareMemKeyNum = 5");
//#E_SHAREMEM_IS_SUPPORT_AUDIO_ONLY = yes
ADD_STRING("ShareMemKey1 = 0x0000038400000001");
//#E_SHAREMEM_ENABLE_FORCE_8BIT_DEC_MODE = yes
ADD_STRING("ShareMemKey2 = 0x000003f400000001");
//#E_SHAREMEM_ENABLE_DIRECTLY_SEEK = yes
ADD_STRING("ShareMemKey3 = 0x000003e100000001");
//#E_SHAREMEM_ENABLE_PAUSE_AT_PLAYEND
ADD_STRING("ShareMemKey4 = 0x000003ae00000001");
//#E_SHAREMEM_EXT_FRAME_CONTROL_DEBUG_MESSAGE_DURATION
ADD_STRING("ShareMemKey5 = 0x001a0000000005dc");

//# netflix
ADD_STRING("[app1]");

//# dlna
ADD_STRING("[app2]");

//# hbbtv
ADD_STRING("[app3]");

//# web browser
ADD_STRING("[app4]");

//# wmdrm 10
ADD_STRING("[app5]");

//# android usb
ADD_STRING("[app6]");

//# android streaming
ADD_STRING("[app7]");

//# rvu
ADD_STRING("[app8]");

//# youtube
ADD_STRING("[app9]");

//# advert
ADD_STRING("[app10]");
//#@CRC=0x91E4

}
