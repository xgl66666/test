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

#ifndef __mmsdk_porting_subtitle__
#define __mmsdk_porting_subtitle__
#include "mmsdk_interface_def.h"


#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

/// @file
/// @defgroup porting_subtitle porting_subtitle functions
/// @{


/// Define subtitle callback type.
typedef enum
{
    /// none
    E_MMSDK_SUBTITLE_CB_NONE = 0,
    /// get current PTS for subtitle rendor.
    /// *((MMSDK_U32 *)pMsgInfo) = Current PTS;
    E_MMSDK_SUBTITLE_CB_GET_CURRENT_PTS,
    /// *((MMSDK_U16 *)pMsgInfo) = VideoWidth;
    /// *((MMSDK_U16 *)pAckInfo) = VideoHeight;
    E_MMSDK_SUBTITLE_CB_GET_VIDEO_SIZE,
    /// TTF font table address
    /// *((MMSDK_U32 *)pMsgInfo) = attached file address
    E_MMSDK_SUBTITLE_CB_GET_ATTACHEDFILE_BUFF_ADDR,
    /// TTF font table size
    /// *((MMSDK_U32 *)pMsgInfo) = attached file size
    E_MMSDK_SUBTITLE_CB_GET_ATTACHEDFILE_SIZE,
    /// get current track language.
    /// *((MMSDK_U32 *)pMsgInfo) = subtitle language
    E_MMSDK_SUBTITLE_CB_GET_CURRENT_TRACK_LANGUAGE,
} EN_MMSDK_SUBTITLE_CB;

/// Define subtitle init status.
/// @since version 1.01
typedef enum
{
    /// Subtitle init unkonw error.
    E_MMSDK_SUBTITLE_INIT_UNKNOW_ERROR = 0,
    /// Subtitle init sucess.
    E_MMSDK_SUBTITLE_INIT_OK = 1,
    /// Can not get enough GOP to deal with subtitle.
    E_MMSDK_SUBTITLE_GOP_NOT_ENOUGH,
    /// Subtitle type error
    E_MMSDK_SUBTITLE_TYPE_ERROR,
    /// Ttf subtitle get attach error
    E_MMSDK_SUBTITLE_GET_ATTACH_ERROR,
    /// Subtitle not support
    E_MMSDK_SUBTITLE_NOT_SUPPORT,
    /// Subtitle init fail
    E_MMSDK_SUBTITLE_INIT_FAIL,
} EN_MMSDK_SUBTITLE_RESULT;

/// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_SET_SUBTITLE_PALETTE.
typedef struct
{
    /// Media object relative information.
    void* Media;
    /// Video subtitle type.
    EN_MMSDK_SUBTITLE_TYPE eType;
    /// Video subtitle palette table start address.
    const MMSDK_U8* pu8Palette;
    /// Video subtitle palette table size.
    MMSDK_U32 u32ByteCount;
} ST_MMSDK_SET_SUBTITLE_PALETTE_INFO;

/// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_SUBTITLE_INIT.
typedef struct
{
    /// Media object relative information.
    void* Media;
    /// Video subtitle type.
    EN_MMSDK_SUBTITLE_TYPE eType;
    /// BMP subtitle width.
    MMSDK_U16 u16BmpWidth;
    /// BMP subtitle height.
    MMSDK_U16 u16BmpHeight;
    /// check subtitle show or hide
    MMSDK_BOOL bShow;
    /// DVB subtitle composition page id
    MMSDK_U16 u16CPageID;
    /// DVB subtitle ancillary page id
    MMSDK_U16 u16APageID;
} ST_MMSDK_SUBTITLE_INIT_INFO;


/// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_SUBQUEUE_PUSH.
typedef struct
{
    /// Media object relative information.
    void* Media;
    /// Video subtitle type.
    EN_MMSDK_SUBTITLE_TYPE eType;
    /// Subtitle data buffer start (virtual address).
    MMSDK_U32 u32QueueStart;
    /// Subtitle data buffer size.
    MMSDK_U32 u32QueueSize;
    /// Subtitle presentation time stamp start.
    MMSDK_U32 u32PTSStart;
    /// Subtitle presentation time stamp end.
    MMSDK_U32 u32PTSEnd;
} ST_MMSDK_SUBTITLE_PUSH_INFO;


/// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_SUBTITLE_ENABLE.
typedef struct
{
    /// Media object relative information.
    void* Media;
    /// Video subtitle type.
    EN_MMSDK_SUBTITLE_TYPE eType;
    /// Enable/Disable subtitle show.
    MMSDK_BOOL bEnable;
    /// Video subtitle total track number.
    MMSDK_U32 u32SubtitleNum;
} ST_MMSDK_SBUTITLE_ENABLE_INFO;


/// Define the attached information for callback process. The callback message is E_CPCODEC_MSG_SET_SUBTITLE_TRACK.
typedef struct
{
    /// Media object relative information.
    void* Media;
    /// Video subtitle type.
    EN_MMSDK_SUBTITLE_TYPE eType;
    /// DVB composition_page_id.
    MMSDK_U16 u16CPageID;
    /// DVB ancillary_page_id.
    MMSDK_U16 u16APageID;
} ST_MMSDK_SBUTITLE_SET_TRACK_INFO;

/// subtitle callback function
/// u32Msg plz use EN_MMSDK_SUBTITLE_CB as callback message.
typedef MMSDK_BOOL (*pfnMmsdkSubtitleCallback)(void* pClass, MMSDK_U32 u32Msg, void* pMsgInfo, void* pAckInfo);

/// define subtitle multiple-instance item
/// @since version 1.01
typedef void* PT_SUBTITLEITEM;

//------------------------------------------------------------------------------
/// Initialized
/// @param pClass            \b IN: callback target class
/// @param pCallback         \b IN: callback function
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Subtitle_Initialize(void* pClass,  const pfnMmsdkSubtitleCallback pCallback);

//------------------------------------------------------------------------------
/// Finailized
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Subtitle_Finalize(void);

//------------------------------------------------------------------------------
/// Set palltte
/// @param pstInfo            \b IN: subtitle palette info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_SubtitlePalette(const ST_MMSDK_SET_SUBTITLE_PALETTE_INFO *pstInfo);

//------------------------------------------------------------------------------
/// Set subtitle init
/// @param pstInfo            \b IN: subtitle init info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_SubtitleInit(const ST_MMSDK_SUBTITLE_INIT_INFO *pstInfo);

//------------------------------------------------------------------------------
/// Set subtitle deinit
/// @param pstInfo            \b IN: subtitle init info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_SubtitleDeInit(const ST_MMSDK_SUBTITLE_INIT_INFO *pstInfo);

//------------------------------------------------------------------------------
/// Push subtitle info
/// After subtitle info has been pushed from MMSDK, MMSDK will recycle the buffer in pushed subtitle info,
/// so porting layer or lower layer (subtitle decoder) should buffer the pushed subtitle info by themself.
/// @param pstInfo            \b IN: subtitle info for push
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_SubtitlePush(const ST_MMSDK_SUBTITLE_PUSH_INFO *pstInfo);

//TODO: doxygen only for DVB Subtitle
//------------------------------------------------------------------------------
/// Set subtitle Track
/// @param pstInfo            \b IN: subtitle track info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_SubtitleSetTrack(const ST_MMSDK_SBUTITLE_SET_TRACK_INFO *pstInfo);

//------------------------------------------------------------------------------
/// Set subtitle display show or not show
/// @param pstInfo            \b IN: subtitle init info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_SubtitleDisplay(const ST_MMSDK_SUBTITLE_INIT_INFO *pstInfo);

//------------------------------------------------------------------------------
/// Set video subtitle trick Mode
/// in order to clear subtitle queue data when backward.
/// @param eTrickMode         \b IN: trick mode
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_SubtitleSetTrickMode(const EN_MMSDK_TRICK_MODE eTrickMode);

//------------------------------------------------------------------------------
/// Set video subtitle rectangle
/// @param pstSubDispRect     \b IN: subtitle rectangle
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_SubtitleSetRectangle(const ST_MMSDK_RECT* pstSubDispRect) __attribute__ ((weak));

/***************** multi-instance subtitle added at version 1.01 *****************/

//------------------------------------------------------------------------------
/// Set max support image and txt subtitle count
/// @since version 1.01
/// @param u8MaxImageSubtitleNum          \b IN: Max support image subtitle count
/// @param u8MaxTxtSubtitleNum            \b IN: Max support txt subtitle count
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_New_SubtitleCapacity(MMSDK_U8 u8MaxImageSubtitleNum, MMSDK_U8 u8MaxTxtSubtitleNum);

//------------------------------------------------------------------------------
/// Initialized and set subtitle init
/// @since version 1.01
/// @param pSubtitleItem      \b OUT: subtitle handle
/// @param pstInfo            \b IN: subtitle init info
/// @param u8DisplayWin       \b IN: subtitle display window(main or sub)
/// @param pClass             \b IN: media object relative information.
/// @param pCallback          \b IN: callback function
///
/// @return EN_MMSDK_SUBTITLE_RESULT
///     - E_MMSDK_SUBTITLE_INIT_UNKNOW_ERROR: init fail
///     - E_MMSDK_SUBTITLE_INIT_OK: init ok
///     - E_MMSDK_SUBTITLE_GOP_NOT_ENOUGH: has no gop to use
///     - E_MMSDK_SUBTITLE_TYPE_ERROR: subtitle codec type error
///     - E_MMSDK_SUBTITLE_GET_ATTACH_ERROR: ttf subtitle get font error
///     - E_MMSDK_SUBTITLE_NOT_SUPPORT: subtitle not support
///     - E_MMSDK_SUBTITLE_INIT_FAIL: subtitle init fail
//------------------------------------------------------------------------------
EN_MMSDK_SUBTITLE_RESULT PT_New_SubtitleInit(PT_SUBTITLEITEM* pSubtitleItem, const ST_MMSDK_SUBTITLE_INIT_INFO *pstInfo, const MMSDK_U8 u8DisplayWin, void* pClass, const pfnMmsdkSubtitleCallback pCallback);

//------------------------------------------------------------------------------
/// Finailized and set subtitle deinit
/// @since version 1.01
/// @param pSubtitleItem      \b IN: info of subtitle item
/// @param pstInfo            \b IN: subtitle init info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_New_SubtitleDeInit(PT_SUBTITLEITEM* pSubtitleItem, const ST_MMSDK_SUBTITLE_INIT_INFO *pstInfo);

//------------------------------------------------------------------------------
/// Push subtitle info and Finailized
/// After subtitle info has been pushed from MMSDK, MMSDK will recycle the buffer in pushed subtitle info,
/// so porting layer or lower layer (subtitle decoder) should buffer the pushed subtitle info by themself.
/// @since version 1.01
/// @param pSubtitleItem      \b IN: info of subtitle item
/// @param pstInfo            \b IN: subtitle info for push
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------

MMSDK_BOOL PT_New_SubtitlePush(PT_SUBTITLEITEM pSubtitleItem, const ST_MMSDK_SUBTITLE_PUSH_INFO *pstInfo);

//TODO: doxygen only for DVB Subtitle
//------------------------------------------------------------------------------
/// Set subtitle Track
/// @since version 1.01
/// @param pSubtitleItem      \b IN: info of subtitle item
/// @param pstInfo            \b IN: subtitle track info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------

MMSDK_BOOL PT_New_SubtitleSetTrack(PT_SUBTITLEITEM pSubtitleItem, const ST_MMSDK_SBUTITLE_SET_TRACK_INFO *pstInfo);

//------------------------------------------------------------------------------
/// Set subtitle display show or not show
/// @since version 1.01
/// @param pSubtitleItem      \b IN: info of subtitle item
/// @param pstInfo            \b IN: subtitle init info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_New_SubtitleDisplay(PT_SUBTITLEITEM pSubtitleItem, const ST_MMSDK_SUBTITLE_INIT_INFO *pstInfo);

//------------------------------------------------------------------------------
/// Set video subtitle trick Mode
/// in order to clear subtitle queue data when backward.
/// @since version 1.01
/// @param pSubtitleItem      \b IN: info of subtitle item
/// @param enTrickMode        \b IN: trick mode
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_New_SubtitleSetTrickMode(PT_SUBTITLEITEM pSubtitleItem, const EN_MMSDK_TRICK_MODE enTrickMode);

//------------------------------------------------------------------------------
/// Set video subtitle rectangle
/// @param pSubtitleItem      \b IN: info of subtitle item
/// @param pstSubDispRect     \b IN: subtitle rectangle
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_New_SubtitleSetRectangle(PT_SUBTITLEITEM pSubtitleItem, const ST_MMSDK_RECT* pstSubDispRect) __attribute__ ((weak));

//------------------------------------------------------------------------------
/// Set palltte
/// @since version 1.01
/// @param pSubtitleItem      \b IN: info of subtitle item
/// @param pstInfo            \b IN: subtitle palette info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_New_SubtitlePalette(PT_SUBTITLEITEM pSubtitleItem,const ST_MMSDK_SET_SUBTITLE_PALETTE_INFO *pstInfo);

/*************************** multi-instance subtitle end ***************************/

//------------------------------------------------------------------------------
/// Sequence diagram of DVB Subtitle at SN MMSDK
/// \file   porting_subtitle.h
/// \brief  DVB Subtitle working flow
/// \image html  porting_subtitle/DVB_Subtitle_sequence_dia_init.jpg "DVB Subtitle initialize"
/// \n
/// \image html  porting_subtitle/DVB_Subtitle_sequence_dia_switch_track.jpg "Siwtch DVB Subtitle track"
/// \n
/// \image html  porting_subtitle/DVB_Subtitle_sequence_dia_disable.jpg "Disable DVB Subtitle"
/// \n
/// \image html  porting_subtitle/DVB_Subtitle_sequence_dia_deinit.jpg "Deinit DVB Subtitle"
//------------------------------------------------------------------------------

/// @} // end of porting_subtitle

#ifdef __cplusplus
}
#endif //__cplusplus


#endif// __mmsdk_porting_subtitle__
///*** please do not remove change list tag ***///
///***$Change: 1220340 $***///
