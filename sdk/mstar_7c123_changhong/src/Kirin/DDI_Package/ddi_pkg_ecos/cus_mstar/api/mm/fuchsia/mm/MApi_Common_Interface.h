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
#ifndef __MAPI_COMMON_INTERFACE_H__
#define __MAPI_COMMON_INTERFACE_H__


//#include "mapp_videoplayer.h"
#include "MsMMTypes.h"
#include "MM_Player_Def.h"



//////////////////// MM Interface Function ///////////////////////////////////////////
//------------------------------------------------------------------------------------

#ifdef __MAPI_COMMON_INTERFACE_C__
#define INTERFACE
#else
#define INTERFACE extern
#endif


typedef enum
{
    EN_MM_PLAY_SUBTYPE_JPG,              /// JPEG
    EN_MM_PLAY_SUBTYPE_BMP,              /// BMP
    EN_MM_PLAY_SUBTYPE_PNG,              /// PNG
    EN_MM_PLAY_SUBTYPE_GIF,              /// GIF

    EN_MM_PLAY_SUBTYPE_MOVIE,            /// MPEG4

    EN_MM_PLAY_SUBTYPE_MP3,              /// MP3
    EN_MM_PLAY_SUBTYPE_AAC,              /// AAC
    EN_MM_PLAY_SUBTYPE_PCM,              /// PCM
    EN_MM_PLAY_SUBTYPE_WAV,              /// WAV
    EN_MM_PLAY_SUBTYPE_WMA,              /// WMA
    EN_MM_PLAY_SUBTYPE_OGG,              /// WMA
    EN_MM_PLAY_SUBTYPE_FLAC,              /// WMA
#if 1
    EN_MM_PLAY_SUBTYPE_PVR,
#endif
    EN_MM_PLAY_SUBTYPE_INVALID,          /// invalid mode
} EN_MM_PLAY_SUBTYPE;

typedef enum
{
    EN_MUSIC_PLAY_SUBTYPE_MP3,              /// MP3
    EN_MUSIC_PLAY_SUBTYPE_AAC,              /// AAC
    EN_MUSIC_PLAY_SUBTYPE_PCM,              /// PCM
    EN_MUSIC_PLAY_SUBTYPE_WAV,              /// WAV
    EN_MUSIC_PLAY_SUBTYPE_WMA,              /// WMA
    #ifdef OGG_PARSER
    EN_MUSIC_PLAY_SUBTYPE_OGG,              /// OGG
    #endif
    EN_MUSIC_PLAY_SUBTYPE_FLAC,              /// OGG
    EN_MUSIC_PLAY_SUBTYPE_INVALID,          /// invalid mode
} EN_MUSIC_SUBTYPE;

#ifdef NEW_VDPLAYER
typedef enum
{
    EN_MM_TYPE_MOVIE = 0,
    EN_MM_TYPE_MUSIC,
    EN_MM_TYPE_PHOTO,
    EN_MM_TYPE_NUM,
}EN_MM_TYPE;

typedef MS_BOOL mm_create_callback(EN_RET ePlayRet);

INTERFACE MS_BOOL MApi_Common_CreateMediaDataItem(MS_U8* photoAddr,MS_U32 photoSize,
                                    int media_type,
                                    MEDIAITEM *p_media_item,
                                    EN_MMSDK_CREATE_MODE create_mode,
                                    pfnMMSDKCallBack cb_process);
INTERFACE MS_BOOL MApi_Common_Data_IO_Callback(int type, int flie_op, void *pMsgInfo, void *pAckInfo);
INTERFACE MS_BOOL MApi_Common_CreateMediaItem(char *OpenFileName,
                                 int media_type,
                                 MEDIAITEM *p_media_item,
                                 EN_MMSDK_CREATE_MODE create_mode,
                                 pfnMMSDKCallBack cb_process);
INTERFACE MS_BOOL MApi_Common_CreateMediaItemID3(char *OpenFileName,
                                    int media_type,
                                    MEDIAITEM *p_media_item,
                                    EN_MMSDK_CREATE_MODE create_mode,
                                    pfnMMSDKCallBack cb_process,
                                    MS_U32 u32offset,MS_U32 u32len);
INTERFACE void MApi_Common_DestroyMediaItem(int type, MEDIAITEM media_item);
INTERFACE void MApi_Common_DestroyMediaItemID3(int type,MEDIAITEM media_item);
INTERFACE MS_BOOL MApi_Common_File_IO_Callback(int type, int flie_op, void *pMsgInfo, void *pAckInfo);
INTERFACE void MApi_Common_Initialize(void);
INTERFACE void MApi_Common_Finalize(void);
INTERFACE void MM_wprintf(const MS_U16* pu16string);
INTERFACE MS_U8 MM_wstrcmp(const MS_U16 *str1, const MS_U16 *str2);
INTERFACE MS_U64 MApi_Common_GetFileSize(EN_MM_TYPE enMMType);
#endif
INTERFACE void MApi_MPlayer_Initital(void);
INTERFACE void MApi_MPlayer_Exit(void);
INTERFACE MS_BOOL MApi_MusicFile_PrepareParser(char *pu16FileName, EN_MUSIC_SUBTYPE* pPlayType);
INTERFACE void Audio_Init(void);
#if 1
INTERFACE MS_U16 *MM_wcsncpy( MS_U16 *strDest, const MS_U16 *strSource, int count );
//INTERFACE MS_U16 *wcsupr(MS_U16 *string);
//INTERFACE int wcsncmp( const MS_U16 *string1, const MS_U16 *string2, int count );
//INTERFACE int wcscmp(const MS_U16 *string1, const MS_U16 *string2);
INTERFACE int MM_wcslen(const MS_U16 *pStr);
INTERFACE MS_U16 *MM_wcscpy(MS_U16 *strDst, const MS_U16 *strSrc);
INTERFACE MS_U16 *MM_wcsrchr (const MS_U16 *string,MS_U16 ch);
//INTERFACE MS_U16 *wcschr(const MS_U16 *string,int chr);
#endif

#undef INTERFACE

#endif
