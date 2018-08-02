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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////
// DRAM memory map
//
// Every Module Memory Mapping need 4 define,
// and check code in "msAPI_Memory_DumpMemoryMap"
// 1. XXX_AVAILABLE : For get avaialble memory start address
// 2. XXX_ADR       : Real Address with Alignment
// 3. XXX_GAP_CHK   : For Check Memory Gap, for avoid memory waste
// 4. XXX_LEN       : For the Memory size of this Module usage
////////////////////////////////////////////////////////////////////////////////
//#define MIU_DRAM_LEN                0x0010000000
//#define MIU_DRAM_LEN0               0x0008000000
//#define MIU_DRAM_LEN1               0x0008000000

//#define MemoryMIU1OnOROff                1
//#define MIU_INTERVAL                0x0008000000
#include "MApi_MEMConfig_Interface.h"

////////////////////////////////////////////////////////////////////////////////
//MEMORY TYPE
////////////////////////////////////////////////////////////////////////////////
#undef MemAlign
#define MemAlign(n, unit)           ((((n)+(unit)-1)/(unit))*(unit))

#define SW                          (0x0000 << 1)
#define HW                          (0x0001 << 1)
#define SW_HW                       (0x0002 << 1)

#define UNCACHED                    (0x0000 << 3)
#define WRITE_THROUGH               (0x0001 << 3)
#define WRITE_COMBINING             (0x0002 << 3)
#define WRITE_PROTECT               (0x0003 << 3)
#define WRITE_BACK                  (0x0004 << 3)


/* SECBUF_START   */
//co_buffer L0
#define MM_SECBUF_START_AVAILABLE                                 SECBUF_STARTADR()
#define FUN_SECBUF_START_ADR                                       MM_SECBUF_START_AVAILABLE//MemAlign(SECBUF_START_AVAILABLE,0x10000)//0x0000820000  //Alignment 0x10000
#define FUN_SECBUF_START_LEN                                       SECBUF_STARTLEN()

/* MUSIC_LYRIC_BUFFER   */
//co_buffer L1
#define MM_MUSIC_LYRIC_BUFFER_AVAILABLE                           MUSIC_LYRIC_BUFFERADR()
#define FUN_MUSIC_LYRIC_BUFFER_ADR                                 MM_MUSIC_LYRIC_BUFFER_AVAILABLE//MemAlign(MUSIC_LYRIC_BUFFER_AVAILABLE,0x01000)//0x0000A27000  //Alignment 0x01000
#define FUN_MUSIC_LYRIC_BUFFER_LEN                                 MUSIC_LYRIC_BUFFERLEN()

/* MUSIC_LYRIC_TAG   */
//co_buffer L1
#define MM_MUSIC_LYRIC_TAG_AVAILABLE                              MUSIC_LYRIC_TAGADR()
#define FUN_MUSIC_LYRIC_TAG_ADR                                    MM_MUSIC_LYRIC_TAG_AVAILABLE//MemAlign(MUSIC_LYRIC_TAG_AVAILABLE,0x01000)//0x0000A37000  //Alignment 0x01000
#define FUN_MUSIC_LYRIC_TAG_LEN                                    MUSIC_LYRIC_TAGLEN()

/* MP3_INFO_READ   */
//co_buffer L1
#define MM_MP3_INFO_READ_AVAILABLE                                MP3_INFO_READADR()
#define FUN_MP3_INFO_READ_ADR                                      MM_MP3_INFO_READ_AVAILABLE//MemAlign(MP3_INFO_READ_AVAILABLE,0x01000)//0x0000A38000  //Alignment 0x01000
#define FUN_MP3_INFO_READ_LEN                                      MP3_INFO_READLEN()

/* M4A_BUFFER   */
//co_buffer L1
#define MM_M4A_BUFFER_AVAILABLE                                   M4A_BUFFERADR()
#define FUN_M4A_BUFFER_ADR                                         MM_M4A_BUFFER_AVAILABLE//MemAlign(M4A_BUFFER_AVAILABLE,0x01000)//0x0000A39000  //Alignment 0x01000
#define FUN_M4A_BUFFER_LEN                                         M4A_BUFFERLEN()

/* M4A_STCO   */
//co_buffer L1
#define MM_M4A_STCO_AVAILABLE                                     M4A_STCOADR()
#define FUN_M4A_STCO_ADR                                           MM_M4A_STCO_AVAILABLE//MemAlign(M4A_STCO_AVAILABLE,0x01000)//0x0000A3D000  //Alignment 0x01000
#define FUN_M4A_STCO_LEN                                           M4A_STCOLEN()

//////////////////////////////////////////////////////////////////////////////////////////////////////////

/* PHOTO_SHARE_MEM   */
//co_buffer L0
#define MM_PHOTO_SHARE_MEM_AVAILABLE                              PHOTO_SHARE_MEMADR()
#define FUN_PHOTO_SHARE_MEM_ADR                                    MM_PHOTO_SHARE_MEM_AVAILABLE//MemAlign(PHOTO_SHARE_MEM_AVAILABLE,0x00004)//0x0000A6C000  //Alignment 0x00004
#define FUN_PHOTO_SHARE_MEM_LEN                                    PHOTO_SHARE_MEMLEN()

/* MAD_JPEG_DISPLAY   */
//co_buffer L6
#define MM_MAD_JPEG_DISPLAY_AVAILABLE                             MAD_JPEG_DISPLAYADR()
#define FUN_MAD_JPEG_DISPLAY_ADR                                   MM_MAD_JPEG_DISPLAY_AVAILABLE//0x00034F0000  //Alignment 0
#define FUN_MAD_JPEG_DISPLAY_LEN                                   MAD_JPEG_DISPLAYLEN()

#define MM_MAD_PHOTO_EFFECT_AVAILABLE                             PHOTO_EFFECT_ADDR()
#define FUN_MAD_PHOTO_EFFECT_ADR                                   MM_MAD_PHOTO_EFFECT_AVAILABLE//0x00034F0000  //Alignment 0
#define FUN_MAD_PHOTO_EFFECT_LEN                                   PHOTO_EFFECT_BUFLEN()

////////////////////////////////////////////////////////////////////////////////////////////////

/* VDPLAYER_DATA   */
//co_buffer L9
#define MM_VDPLAYER_DATA_AVAILABLE                                VDPLAYER_DATAADR()
#define FUN_VDPLAYER_DATA_ADR                                      MemAlign(MM_VDPLAYER_DATA_AVAILABLE,0x01000)//0x0002A6D000  //Alignment 0x01000
#define FUN_VDPLAYER_DATA_LEN                                      VDPLAYER_DATALEN()

/* VDPLAYER_BITSTREAM_BUFF   */
//co_buffer L9
#define MM_VDPLAYER_BITSTREAM_BUFF_AVAILABLE                      VDPLAYER_BITSTREAM_BUFFADR()
#define FUN_VDPLAYER_BITSTREAM_BUFF_ADR                            MM_VDPLAYER_BITSTREAM_BUFF_AVAILABLE//0x0002DED000  //Alignment 0
#define FUN_VDPLAYER_BITSTREAM_BUFF_LEN                            VDPLAYER_BITSTREAM_BUFFLEN()

/* VDPLAYER_BITSTREAM_BUFF_EXT   */
//co_buffer L9
#define MM_VDPLAYER_BITSTREAM_BUFF_EXT_AVAILABLE                  VDPLAYER_BITSTREAM_BUFF_EXTADR()
#define FUN_VDPLAYER_BITSTREAM_BUFF_EXT_ADR                        MM_VDPLAYER_BITSTREAM_BUFF_EXT_AVAILABLE//0x00031ED000  //Alignment 0
#define FUN_VDPLAYER_BITSTREAM_BUFF_EXT_LEN                        VDPLAYER_BITSTREAM_BUFF_EXTLEN()

/* VDPLAYER_SUBTITLE_BITSTREAM_BUFF   */
//co_buffer L9
#define MM_VDPLAYER_SUBTITLE_BITSTREAM_BUFF_AVAILABLE             VDPLAYER_SUBTITLE_BITSTREAM_BUFFADR()
#define FUN_VDPLAYER_SUBTITLE_BITSTREAM_BUFF_ADR                   MM_VDPLAYER_SUBTITLE_BITSTREAM_BUFF_AVAILABLE//0x0003789000  //Alignment 0
#define FUN_VDPLAYER_SUBTITLE_BITSTREAM_BUFF_LEN                   VDPLAYER_SUBTITLE_BITSTREAM_BUFFLEN()

/* MP4_SUBTITLE_BUFFER   */
//co_buffer L9
#define MM_MP4_SUBTITLE_BUFFER_AVAILABLE                          MP4_SUBTITLE_BUFFERADR()
#define FUN_MP4_SUBTITLE_BUFFER_ADR                                MemAlign(MM_MP4_SUBTITLE_BUFFER_AVAILABLE,0x01000)//0x0003989000  //Alignment 0x01000
#define FUN_MP4_SUBTITLE_BUFFER_LEN                                MP4_SUBTITLE_BUFFERLEN()

/* MP4_SUBTITLE_TAG   */
//co_buffer L9
#define MM_MP4_SUBTITLE_TAG_AVAILABLE                             MP4_SUBTITLE_TAGADR()
#define FUN_MP4_SUBTITLE_TAG_ADR                                   MemAlign(MM_MP4_SUBTITLE_TAG_AVAILABLE,0x01000)//0x0003A89000  //Alignment 0x01000
#define FUN_MP4_SUBTITLE_TAG_LEN                                   MP4_SUBTITLE_TAGLEN()

/* SUBTITLE   */
//co_buffer L0
#define MM_SUBTITLE_AVAILABLE                                     SUBTITLEADR()
#define FUN_SUBTITLE_ADR                                           MemAlign(MM_SUBTITLE_AVAILABLE,0x10000)//0x0004000000  //Alignment 0x10000
#define FUN_SUBTITLE_LEN                                           SUBTITLELEN()

#define MM_VDEC_FW_BUF_AVAILABLE                                  VDEC_FW_BUFADDR()
#define FUN_VDEC_FW_BUF_ADDR                                       MM_VDEC_FW_BUF_AVAILABLE//0x0004000000
#define FUN_VDEC_FW_BUF_LEN                                        VDEC_FW_BUFLEN()

//////////////////////////////////////////////////////////////////////////////////////////////////

/* TEXT_FILE   */
//co_buffer L5
#define MM_TEXT_FILE_AVAILABLE                                    TEXT_FILEADR()
#define FUN_TEXT_FILE_ADR                                          MM_TEXT_FILE_AVAILABLE//0x0002A6C400  //Alignment 0x00200
#define FUN_TEXT_FILE_LEN                                          TEXT_FILELEN()

/* MAD_JPEG_OUT   */
//co_buffer L6
#define MM_MAD_JPEG_OUT_AVAILABLE                                 MAD_JPEG_OUTADR()
#define FUN_MAD_JPEG_OUT_ADR                                       MM_MAD_JPEG_OUT_AVAILABLE//0x0002A6C340  //Alignment 0x00008
#define FUN_MAD_JPEG_OUT_LEN                                       MAD_JPEG_OUTLEN()

/* MAD_JPEG_READBUFF   */
//co_buffer L6
#define MM_MAD_JPEG_READBUFF_AVAILABLE                            MAD_JPEG_READBUFFADR()
#define FUN_MAD_JPEG_READBUFF_ADR                                  MM_MAD_JPEG_READBUFF_AVAILABLE//0x0002E69000  //Alignment 0x01000
#define FUN_MAD_JPEG_READBUFF_LEN                                  MAD_JPEG_READBUFFLEN()

/* MAD_JPEG_INTERBUFF   */
//co_buffer L6
#define MM_MAD_JPEG_INTERBUFF_AVAILABLE                           MAD_JPEG_INTERBUFFADR()
#define FUN_MAD_JPEG_INTERBUFF_ADR                                 MM_MAD_JPEG_INTERBUFF_AVAILABLE//0x0002EC0000  //Alignment 0x10000
#define FUN_MAD_JPEG_INTERBUFF_LEN                                 MAD_JPEG_INTERBUFFLEN()

/* MAD_SE_BUFFER   */
//co_buffer L1
#define MM_MAD_SE_BUFFER_AVAILABLE                                MAD_SE_BUFFERADR()//0x0000A6D000
#define FUN_MAD_SE_BUFFER_ADR                                      (MM_MAD_SE_BUFFER_AVAILABLE)  //Alignment 0
//#define MAD_SE_BUFFER_GAP_CHK                                  0x0000000000
#define FUN_MAD_SE_BUFFER_LEN                                      MAD_SE_BUFFERLEN()
//#define MAD_SE_BUFFER_MEMORY_TYPE                              (MIU0 | SW | WRITE_BACK)

/* MAD_DEC_BUFFER   */
//co_buffer L1
#define MM_MAD_DEC_BUFFER_AVAILABLE                               MAD_DEC_BUFFERADR()
#define FUN_MAD_DEC_BUFFER_ADR                                     (MM_MAD_DEC_BUFFER_AVAILABLE)//0x0000B6D000  //Alignment 0
//#define MAD_DEC_BUFFER_GAP_CHK                                 0x0000000000
#define FUN_MAD_DEC_BUFFER_LEN                                     MAD_DEC_BUFFERLEN()
//#define MAD_DEC_BUFFER_MEMORY_TYPE                             (MIU0 | SW | WRITE_BACK)

//MIU_1_START
/* VDEC_AEON   */
//co_buffer L0
#define MM_VDEC_AEON_AVAILABLE                                    VDEC_AEONADR()
#define FUN_VDEC_AEON_ADR                                          MemAlign(VDEC_AEONADR(),0x00010)  //Alignment 0x00010
//#define VDEC_AEON_GAP_CHK                                      0x0000000000
#define FUN_VDEC_AEON_LEN                                          VDEC_AEONLEN()
//#define VDEC_AEON_MEMORY_TYPE                                  (MIU0 | SW | WRITE_BACK)

/* VDEC_FRAMEBUFFER   */
//co_buffer L0
#define MM_VDEC_FRAMEBUFFER_AVAILABLE                             VDEC_FRAMEBUFFERADR()
#define FUN_VDEC_FRAMEBUFFER_ADR                                   MemAlign(VDEC_FRAMEBUFFERADR(),0x00200)  //Alignment 0x00200
//#define VDEC_FRAMEBUFFER_GAP_CHK                               0x0000000000
#define FUN_VDEC_FRAMEBUFFER_LEN                                   VDEC_FRAMEBUFFERLEN()
//#define VDEC_FRAMEBUFFER_MEMORY_TYPE                           (MIU0 | SW | WRITE_BACK)

/* VDEC_BITSTREAM   */
//co_buffer L0
#define MM_VDEC_BITSTREAM_AVAILABLE                               VDEC_BITSTREAMADR()
#define FUN_VDEC_BITSTREAM_ADR                                     MemAlign(VDEC_BITSTREAMADR(),0x00008)  //Alignment 0x00008
//#define VDEC_BITSTREAM_GAP_CHK                                 0x0000000000
#define FUN_VDEC_BITSTREAM_LEN                                     VDEC_BITSTREAMLEN()
//#define VDEC_BITSTREAM_MEMORY_TYPE                             (MIU0 | SW | WRITE_BACK)

/////////////////////////////////////////////////////////////////////////////////////////////////////
/* MAD_BASE_BUFFER   */
//co_buffer L0
//------------Unuse in MM player code,but Keep it in this code-----------------
#define MM_MAD_BASE_BUFFER_AVAILABLE                              MAD_BASE_BUFFERADR()
#define FUN_MAD_BASE_BUFFER_ADR                                    MemAlign(MM_MAD_BASE_BUFFER_AVAILABLE,0x01000)//0x0000A6D000  //Alignment 0x01000
#define FUN_MAD_BASE_BUFFER_LEN                                    MAD_BASE_BUFFERLEN()

/* POOL_BUFFER   */
//co_buffer L0
//------------Unuse in MM player code,but Keep it in this code-----------------
#define MM_POOL_BUFFER_AVAILABLE                                  POOL_BUFFERADR()
#define FUN_POOL_BUFFER_ADR                                        MM_POOL_BUFFER_AVAILABLE//0x0002A6C340  //Alignment 0x00020
#define FUN_POOL_BUFFER_LEN                                        POOL_BUFFERLEN()

/* PHOTO_DATA_READ   */
//co_buffer L7
//------------Unuse in MM player code,but Keep it in this code-----------------
#define MM_PHOTO_DATA_READ_AVAILABLE                              PHOTO_DATA_READADR()
#define FUN_PHOTO_DATA_READ_ADR                                    MM_PHOTO_DATA_READ_AVAILABLE//0x0002E69000  //Alignment 0x01000
#define FUN_PHOTO_DATA_READ_LEN                                    PHOTO_DATA_READLEN()

/* VDEC_FRAMEBUFFER_SD   */
//co_buffer L1
//------------Unuse in MM player code,but Keep it in this code-----------------
#define MM_VDEC_FRAMEBUFFER_SD_AVAILABLE                          VDEC_FRAMEBUFFER_SDADR()
#define FUN_VDEC_FRAMEBUFFER_SD_ADR                                MM_VDEC_FRAMEBUFFER_SD_AVAILABLE//0x0004400000  //Alignment 0x00200
#define FUN_VDEC_FRAMEBUFFER_SD_LEN                                VDEC_FRAMEBUFFER_SDLEN()

/* VDEC_BITSTREAM_SD   */
//co_buffer L1
//------------Unuse in MM player code,but Keep it in this code-----------------
#define MM_VDEC_BITSTREAM_SD_AVAILABLE                            VDEC_BITSTREAM_SDADR()
#define FUN_VDEC_BITSTREAM_SD_ADR                                  MM_VDEC_BITSTREAM_SD_AVAILABLE//0x0005300000  //Alignment 0x00010
#define FUN_VDEC_BITSTREAM_SD_LEN                                  VDEC_BITSTREAM_SDLEN()


//MIU_END
