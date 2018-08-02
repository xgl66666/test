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

#ifndef __porting_misc__
#define __porting_misc__

#include "mmsdk_interface_def.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

/// @file
/// @defgroup porting_misc porting_misc functions
/// @{

#define PT_STATIC_ASSERT(_x_)                              \
            do {                                                \
                char c[(_x_)?(1):(-1)];          \
                c[0]='\0';                            \
                c[0]= c[0];                            \
            } while (0)


#define PT_ASSERT(_x_)                                                                         \
    do  {                                                                                   \
    if ( ! ( _x_ ) )                                                                    \
    {                                                                                   \
        printf("ASSERT FAIL: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);     \
        abort();                                                                        \
    }                                                                                   \
} while (0)



//------------------------------------------------------------------------------
/// For vdplayer run on co-processor only(vdplayer.bin)
/// @deprecated
/// @param u32PhyAdd            \b IN: vdplayer physical start address
/// @param u32BufferSize        \b IN: size of vdplayer
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MISC_LoadVdplayerBin(MMSDK_U32 u32PhyAdd, const MMSDK_U32 u32BufferSize);

//------------------------------------------------------------------------------
/// TSP FW Bin path(fwTSP.bin)
/// @param u32PhyAdd            \b IN: TSPfw physical start address
/// @param u32BufferSize        \b IN: size of TSPfw
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MISC_LoadTspFwBin(MMSDK_U32 u32PhyAdd, const MMSDK_U32 u32BufferSize);

//------------------------------------------------------------------------------
/// Get FBL threshold for bandwidth concern
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_U32 PT_MISC_GetFBLModeThreshold(void);

//------------------------------------------------------------------------------
/// Get Video Mirror Flag
///
/// @return TRUE: mirror enable
/// @return FALSE: mirror disable
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MISC_GetMirrorVideoFlag(void);

//------------------------------------------------------------------------------
/// Demux has been initialized or not
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MISC_Demux_IsInitialized(void);

//------------------------------------------------------------------------------
/// Get TSP setting information
/// @param pu32VQStart            \b IN: phycial start address of VQ
/// @param pu32VQSize             \b IN: size of VQ; Size of SN 0XAD000(708608)
/// @param pu8CPFilterStart          \b IN: start filter of co-processer
/// @param pu8CPFilterEnd         \b IN: end filter of co-processer
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MISC_Demux_GetSettingInfo(MMSDK_U32* pu32VQStart, MMSDK_U32* pu32VQSize, MMSDK_U8* pu8CPFilterStart, MMSDK_U8* pu8CPFilterEnd);

//------------------------------------------------------------------------------
/// When play continuous frames(normal play) then adapt PQ for more smoother,
/// disable it in trick mode(discontinuous frame)
/// @deprecated
/// @since version 1.01
/// @param bEnable            \b IN: enable or not
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MISC_PQ_AdaptContinuousFramePQ(const MMSDK_BOOL bEnable);

//------------------------------------------------------------------------------
/// Trigger set picture quality after set up xc window
/// @since version 1.04
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MISC_Set_PQ(void) __attribute__ ((weak));

//------------------------------------------------------------------------------
/// This object provides string kay:value mapping for iniparser.
//------------------------------------------------------------------------------
typedef void* PT_dictionary;

//------------------------------------------------------------------------------
/// Load ini file, the ini parser will create the dictionary object.
/// @param ininame            \b IN: the ini filename to read
///
/// @return pointer to PT_dictionary
//------------------------------------------------------------------------------
PT_dictionary * PT_iniparser_load(const MMSDK_U8 * ininame);

//------------------------------------------------------------------------------
/// Get the specific value with a key
/// The iniparser will search the section:key in the dictionary
/// and return def if not found
/// @param d                  \b IN: dictionary from PT_dictionary
/// @param key                \b IN: Look for this key in the dictionary
/// @param def                \b IN: return this default value if key not found.
///
/// @return   pointer to the matched string
//------------------------------------------------------------------------------
MMSDK_U8 * PT_iniparser_getstring(PT_dictionary * d, const MMSDK_U8 * key, MMSDK_U8 * def);

//------------------------------------------------------------------------------
/// Free iniparser related memory
/// @param d                  \b IN: dictionary from PT_dictionary
//------------------------------------------------------------------------------
void PT_iniparser_freedict(PT_dictionary * d);


//-------------------------------------------------------------------------------------------------
/// reset cocodec INI config file path
///
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_resetCfgFilePath(void);

//-------------------------------------------------------------------------------------------------
/// set cocodec INI config file path
/// @param pu8FilePath      \b IN: file full path exp: "/sda1/MM/config.ini"
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_setCfgFilePath(const MMSDK_U8 * pu8FilePath);

//-------------------------------------------------------------------------------------------------
/// get cocodec INI config file path
///
/// @return current cpcodec INI config file path
//-------------------------------------------------------------------------------------------------
const MMSDK_U8 * PT_getCfgFilePath(void);

//------------------------------------------------------------------------------
/// Encode RGB raw data to Jpeg format
/// @param pu8DstFullName       \b IN: encoded jpeg full name (include path and file name)
/// @param pu8RgbData           \b IN: RGB raw data address
/// @param u16Width             \b IN: source image width
/// @param u16Height            \b IN: source image height
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_encodeJpeg(const MMSDK_U8* pu8DstFullName, const MMSDK_U8 *pu8RgbData, const MMSDK_U16 u16Width, const MMSDK_U16 u16Height);

//-------------------------------------------------------------------------------------------------
/// Get the VDEC Codec capability, CSP use the informaation to decide provide which codec service
/// @param pstCodecCapInfo      \b IN/OUT: Capability information and query codec
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_GetVideoCodecCapability(ST_MMSDK_CODEC_CAP_INFO *pstCodecCapInfo) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// Adjust the current output frame rate by source frame rate(FPS30 = 30000)
/// @param u32FrameRate         \b IN: Source Frame Rate
/// @param pu32AdjustedFrameRate \b OUT: Adjusted output frame rate
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_AdjustOutputFrameRate(MMSDK_U32 u32FrameRate, MMSDK_U32 *pu32AdjustedFrameRate) __attribute__ ((weak));

//------------------------------------------------------------------------------
/// Generic api for mmsdk to interact with porting layer, add it to avoid add more api in the future
/// @param pu8MsgType           \b IN: message type (string)
/// @param pMsgInfo             \b IN: message info
/// @param pAckInfo             \b IN: Ack info
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_GenericFunction(const MMSDK_U8* const pu8MsgType, void* pMsgInfo, void* pAckInfo);

//------------------------------------------------------------------------------
/// Get the porting layer version
/// @since version 1.01
/// @param pu32Version           \b OUT: mmsdk porting layer version
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_GetPortingVersion(MMSDK_U32* pu32Version);

//------------------------------------------------------------------------------
/// Update playback status
/// Now use for dolby vision stream
/// @param eStatus              \b IN: playback status
///
/// @return N/A
//------------------------------------------------------------------------------
void PT_UpdatePlaybackStatus(const EN_MMSDK_PLAYBACK_STATUS eStatus) __attribute__ ((weak));

//------------------------------------------------------------------------------
/// Get the video delay from systeminfo.
/// @param ps16VideoDelayOffset     \b OUT: The Video delay
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_GetUseVideoDelayOffset(MMSDK_S16* ps16VideoDelayOffset) __attribute__ ((weak));

#ifdef __cplusplus
}
#endif //__cplusplus

/// @} // end of porting_misc

#endif //__porting_misc__
///*** please do not remove change list tag ***///
///***$Change: 1428652 $***///
