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
#ifndef __MAPI_MEMSET_INTERFACE_H__
#define __MAPI_MEMSET_INTERFACE_H__

#include "MsMMTypes.h"
#include "MsMMTypes.h"

#define MMPLAYERMEMSIZE_64M         0
#define MMPLAYERMEMSIZE_128M        1
#define MMPLAYERMEMSIZE_128x128     2

#define MMPLAYER_MIU1_USE           1
#define MMPLAYER_MIU1_UNUSE         0

typedef struct
{
    MS_U16  MemorySize;
    MS_U16  MemoryMIU1Flag;
    //Music
    MS_PHYADDR u32SecbufAddr;
    MS_U32 u32SecbufSize;
    MS_PHYADDR u32MusicLyricBufAddr ;
    MS_U32 u32MusicLyricBufSize;
    MS_PHYADDR u32MusicLyricTagAddr ;
    MS_U32 u32MusicLyricTagSize;
    MS_PHYADDR u32Mp3InfoReadAddr ;
    MS_U32 u32Mp3InfoReadSize;
    MS_PHYADDR u32M4ABufAddr ;
    MS_U32 u32M4ABufSize;
    MS_PHYADDR u32M4ASTCOBufAddr ;
    MS_U32 u32M4ASTCOBufSize;    
    
    //Photo
    MS_PHYADDR u32PhotoShareMemAddr ;
    MS_U32 u32PhotoShareMemSize;     
    MS_PHYADDR u32MADJpegDisplayAddr ;
    MS_U32 u32MADJpegDisplaySize; 
    MS_PHYADDR u32MADJpegOutAddr ;
    MS_U32 u32MADJpegOutSize;     
    MS_PHYADDR u32MADJpegReadBufAddr ;
    MS_U32 u32MADJpegReadBufSize; 
    MS_PHYADDR u32MADJpegInterBufAddr ;
    MS_U32 u32MADJpegInterBufSize; 
    MS_PHYADDR u32PhotoDataReadBufAddr ;
    MS_U32 u32PhotoDataReadBufSize;       
    
    //Video
    MS_PHYADDR u32VDPlayerDataAddr ;
    MS_U32 u32VDPlayerDataSize;     
    MS_PHYADDR u32VDPlayerBitstreamBufAddr ;
    MS_U32 u32VDPlayerBitstreamBufSize;     
    MS_PHYADDR u32VDPlayerBitstreamBufEXTAddr ;
    MS_U32 u32VDPlayerBitstreamBufEXTSize;  
    MS_PHYADDR u32VDPlayerSubtitleBitstreamBufAddr ;
    MS_U32 u32VDPlayerSubtitleBitstreamBufSize; 
    MS_PHYADDR u32MP4SubtitleBufAddr ;
    MS_U32 u32MP4SubtitleBufSize;
    MS_PHYADDR u32MP4SubtitleTAGAddr ;
    MS_U32 u32MP4SubtitleTAGSize;     
    MS_PHYADDR u32SubtitleAddr ;
    MS_U32 u32SubtitleSize;    
    MS_PHYADDR u32VDECFWBufAddr ;
    MS_U32 u32VDECFWBufSize;    
      
    //TXT
    MS_PHYADDR u32TEXTFILEAddr ;
    MS_U32 u32TEXTFILESize; 
    MS_PHYADDR u32PoolBufAddr ;
    MS_U32 u32PoolBufSize;

    MS_PHYADDR u32MAD_SE_BufAddr ;
    MS_U32 u32MAD_SE_BufSize;
    MS_PHYADDR u32MAD_DEC_BufAddr ;
    MS_U32 u32MAD_DEC_BufSize;      
    MS_PHYADDR u32MAD_Base_BufAddr ;
    MS_U32 u32MAD_Base_BufSize; 

    MS_PHYADDR u32ScalerDNRBufAddr;
    MS_U32 u32ScalerDNRBufSize;
    MS_PHYADDR u32TSPBufAddr;
    MS_U32 u32TSPBufSize;       
    MS_PHYADDR u32VDECAEONBufAddr;
    MS_U32 u32VDECAEONBufSize;
    MS_PHYADDR u32VDECFrameBufAddr;
    MS_U32 u32VDECFrameBufSize;
    MS_PHYADDR u32VDECBitstreamBufAddr;
    MS_U32 u32VDECBitstreamBufSize;
    MS_PHYADDR u32VDEC_SD_FrameBufAddr;
    MS_U32 u32VDEC_SD_FrameBufSize;
    MS_PHYADDR u32VDEC_SD_BitstreamBufAddr;
    MS_U32 u32VDEC_SD_BitstreamBufSize;    
    
    MS_PHYADDR u32photoEffectAddr;
    MS_U32 u32photoEffectSize;    
} MMPlayerMemSet, *P_MMPlayerMemSet;

#ifdef NEW_VDPLAYER
typedef struct
{
    MS_U16 u16MemorySize;
    MS_U16 u16MemoryMIU1Flag;

    MS_U32 u32DemuxSecAddr;
    MS_U32 u32DemuxSecSize;
    MS_U32 u32MadDecodeAddr;
    MS_U32 u32MadDecodeSize;

    MS_U32 u32PhotoDisplayAddr;
    MS_U32 u32PhotoDisplaySize;
    MS_U32 u32PhotoDisplayMpoAddr;
    MS_U32 u32PhotoDisplayMpoSize;
    MS_U32 u32PhotoInternalAddr;
    MS_U32 u32PhotoInternalSize; 
    MS_U32 u32PhotoMemoryPoolAddr;
    MS_U32 u32PhotoMemoryPoolSize; 
    MS_U32 u32PhotoOutputAddr;
    MS_U32 u32PhotoOutputSize; 
    MS_U32 u32PhotoProgressiveAddr;
    MS_U32 u32PhotoProgressiveSize;
    MS_U32 u32PhotoReadAddr;
    MS_U32 u32PhotoReadSize;
    MS_U32 u32PhotoShareMemoryAddr;
    MS_U32 u32PhotoShareMemorySize;

    MS_U32 u32VDPlayerAeonDataAddr;
    MS_U32 u32VDPlayerAeonDataSize;     
    MS_U32 u32VDPlayerBitstreamAudioAddr;
    MS_U32 u32VDPlayerBitstreamAudioSize;     
    MS_U32 u32VDPlayerBitstreamSubtitleAddr;
    MS_U32 u32VDPlayerBitstreamSubtitleSize;  
    MS_U32 u32VDPlayerBitstreamVideoAddr;
    MS_U32 u32VDPlayerBitstreamVideoSize;  
    MS_U32 u32VDPlayerLoadCodeAddr;
    MS_U32 u32VDPlayerLoadCodeSize; 
    MS_U32 u32VDPlayerShareMemoryAddr;
    MS_U32 u32VDPlayerShareMemorySize; 

    MS_U32 u32SubtitleFontTableAddr;
    MS_U32 u32SubtitleFontTableSize;    
    MS_U32 u32SubtitlePushAddr;
    MS_U32 u32SubtitlePushSize;    
      
    MS_U32 u32VdecCPUAddr;
    MS_U32 u32VdecCPUSize;
    MS_U32 u32VdecFrameBufferAddr;
    MS_U32 u32VdecFrameBufferSize;
    MS_U32 u32VdecSubFrameBufferAddr;
    MS_U32 u32VdecSubFrameBufferSize;
    MS_U32 u32VdecFrameBufferMVCAddr;
    MS_U32 u32VdecFrameBufferMVCSize;
    MS_U32 u32VdecFrameBufferSDAddr;
    MS_U32 u32VdecFrameBufferSDSize;
    MS_U32 u32VdecStreamBufferAddr;
    MS_U32 u32VdecStreamBufferSize;   
    MS_U32 u32VdecSubStreamBufferAddr;
    MS_U32 u32VdecSubStreamBufferSize;   
    MS_U32 u32VdecStreamBufferMVCAddr;
    MS_U32 u32VdecStreamBufferMVCSize;  
    
    MS_U32 u32XCMainFBAddr;
    MS_U32 u32XCMainFBSize;   
    
    MS_U32 u32PHYAddrForCopyDataAddr;
    MS_U32 u32PHYAddrForCopyDataSize;
    
    MS_U32 u32MMTTXAddr;
    MS_U32 u32MMTTXSize;
} MMSDKPlayerMemSet, *P_MMSDKPlayerMemSet;

extern BOOLEAN MMSDK_Memory_Config(P_MMSDKPlayerMemSet PMM_MemorySetting);
#endif
extern MS_U32 SECBUF_STARTADR(void);

extern MS_U32 SECBUF_STARTLEN(void);

extern MS_U32 MUSIC_LYRIC_BUFFERADR(void);

extern MS_U32 MUSIC_LYRIC_BUFFERLEN(void);

extern MS_U32 MUSIC_LYRIC_TAGADR(void);

extern MS_U32 MUSIC_LYRIC_TAGLEN(void);

extern MS_U32 MP3_INFO_READADR(void);

extern MS_U32 MP3_INFO_READLEN(void);

extern MS_U32 M4A_BUFFERADR(void);

extern MS_U32 M4A_BUFFERLEN(void);

extern MS_U32 M4A_STCOADR(void);

extern MS_U32 M4A_STCOLEN(void);

//////////////////////////////////////////////PHOTO////////////////////////////////////////////

extern MS_U32 PHOTO_SHARE_MEMADR(void);

extern MS_U32 PHOTO_SHARE_MEMLEN(void);

extern MS_U32 MAD_JPEG_DISPLAYADR(void);

extern MS_U32 MAD_JPEG_DISPLAYLEN(void);

extern MS_U32 PHOTO_DATA_READADR(void);

extern MS_U32 PHOTO_DATA_READLEN(void);

extern MS_U32 MAD_JPEG_OUTADR(void);

extern MS_U32 MAD_JPEG_OUTLEN(void);

extern MS_U32 MAD_JPEG_READBUFFADR(void);

extern MS_U32 MAD_JPEG_READBUFFLEN(void);

extern MS_U32 MAD_JPEG_INTERBUFFADR(void);

extern MS_U32 MAD_JPEG_INTERBUFFLEN(void);

//////////////////////////////////////////////Video////////////////////////////////////////////

extern MS_U32 VDPLAYER_DATAADR(void);

extern MS_U32 VDPLAYER_DATALEN(void);

extern MS_U32 VDPLAYER_BITSTREAM_BUFFADR(void);
extern MS_U32 VDPLAYER_BITSTREAM_BUFFLEN(void);

extern MS_U32 VDPLAYER_BITSTREAM_BUFF_EXTADR(void);

extern MS_U32 VDPLAYER_BITSTREAM_BUFF_EXTLEN(void);

extern MS_U32 VDPLAYER_SUBTITLE_BITSTREAM_BUFFADR(void);

extern MS_U32 VDPLAYER_SUBTITLE_BITSTREAM_BUFFLEN(void);

extern MS_U32 MP4_SUBTITLE_BUFFERADR(void);

extern MS_U32 MP4_SUBTITLE_BUFFERLEN(void);

extern MS_U32 MP4_SUBTITLE_TAGADR(void);

extern MS_U32 MP4_SUBTITLE_TAGLEN(void);

extern MS_U32 SUBTITLEADR(void);

extern MS_U32 SUBTITLELEN(void);

extern MS_U32 VDEC_FW_BUFADDR(void);

extern MS_U32 VDEC_FW_BUFLEN(void);

//////////////////////////////////////////////TXT////////////////////////////////////////////

extern MS_U32 TEXT_FILEADR(void);

extern MS_U32 TEXT_FILELEN(void);

extern MS_U32 POOL_BUFFERADR(void);

extern MS_U32 POOL_BUFFERLEN(void);


//////////////////////////////////////////////VDEC////////////////////////////////////////////
extern MS_U32 VDEC_AEONADR(void);

extern MS_U32 VDEC_AEONLEN(void);

extern MS_U32 VDEC_FRAMEBUFFERADR(void);

extern MS_U32 VDEC_FRAMEBUFFERLEN(void);

extern MS_U32 VDEC_BITSTREAMADR(void);

extern MS_U32 VDEC_BITSTREAMLEN(void);

extern MS_U32 VDEC_FRAMEBUFFER_SDADR(void);

extern MS_U32 VDEC_FRAMEBUFFER_SDLEN(void);

extern MS_U32 VDEC_BITSTREAM_SDADR(void);

extern MS_U32 VDEC_BITSTREAM_SDLEN(void);

//////////////////////////////////////////////MAD////////////////////////////////////////////
extern MS_U32 MAD_SE_BUFFERADR(void);

extern MS_U32 MAD_SE_BUFFERLEN(void);

extern MS_U32 MAD_DEC_BUFFERADR(void);

extern MS_U32 MAD_DEC_BUFFERLEN(void);

extern MS_U32 MAD_BASE_BUFFERADR(void);

extern MS_U32 MAD_BASE_BUFFERLEN(void);

extern MS_U16 MM_MemorySize(void);

extern MS_U16 MemoryMIU1OnOROff(void);

extern MS_U32 TSP_BUF_ADDR(void);

extern MS_U32 TSP_BUFLEN(void);

extern MS_U32 SCALER_DNR_BUF_ADDR(void);

extern MS_U32 SCALER_DNR_BUFLEN(void);

extern MS_U32 TSP_BUF_ADDR(void);
extern MS_U32 TSP_BUFLEN(void);
extern BOOLEAN MM_Memory_Config(P_MMPlayerMemSet PMM_MemorySetting);

/*void MM_Player_Memoryconfig(void)
{
   MMPlayerMemSet MM_Mem;
    
    MM_Mem.MemorySize = MMPLAYERMEMSIZE_64M;
    MM_Mem.MemoryMIU1Flag = MMPLAYER_MIU1_UNUSE;
    //Music
    MM_Mem.u32SecbufAddr = ((SECBUF_START_MEMORY_TYPE & MIU1) ? (SECBUF_START_ADR | MIU_INTERVAL) : (SECBUF_START_ADR));
    MM_Mem.u32SecbufSize = SECBUF_START_LEN;
    MM_Mem.u32MusicLyricBufAddr = ((MUSIC_LYRIC_BUFFER_MEMORY_TYPE & MIU1) ? (MUSIC_LYRIC_BUFFER_ADR | MIU_INTERVAL) : (MUSIC_LYRIC_BUFFER_ADR));
    MM_Mem.u32MusicLyricBufSize = MUSIC_LYRIC_BUFFER_LEN;
    MM_Mem.u32MusicLyricTagAddr = ((MUSIC_LYRIC_TAG_MEMORY_TYPE & MIU1) ? (MUSIC_LYRIC_TAG_ADR | MIU_INTERVAL) : (MUSIC_LYRIC_TAG_ADR));
    MM_Mem.u32MusicLyricTagSize = MUSIC_LYRIC_TAG_LEN;
    MM_Mem.u32Mp3InfoReadAddr = ((MP3_INFO_READ_MEMORY_TYPE & MIU1) ? (MP3_INFO_READ_ADR | MIU_INTERVAL) : (MP3_INFO_READ_ADR));
    MM_Mem.u32Mp3InfoReadSize = MP3_INFO_READ_LEN;
    MM_Mem.u32M4ABufAddr = ((M4A_BUFFER_MEMORY_TYPE & MIU1) ? (M4A_BUFFER_ADR | MIU_INTERVAL) : (M4A_BUFFER_ADR));
    MM_Mem.u32M4ABufSize = M4A_BUFFER_LEN;
    MM_Mem.u32M4ASTCOBufAddr = ((M4A_STCO_MEMORY_TYPE & MIU1) ? (M4A_STCO_ADR | MIU_INTERVAL) : (M4A_STCO_ADR));
    MM_Mem.u32M4ASTCOBufSize = M4A_STCO_LEN;    
    
    //Photo
    MM_Mem.u32PhotoShareMemAddr = ((PHOTO_SHARE_MEM_MEMORY_TYPE & MIU1) ? (PHOTO_SHARE_MEM_ADR | MIU_INTERVAL) : (PHOTO_SHARE_MEM_ADR));
    MM_Mem.u32PhotoShareMemSize = PHOTO_SHARE_MEM_LEN;     
    MM_Mem.u32MADJpegDisplayAddr = ((MAD_JPEG_DISPLAY_MEMORY_TYPE & MIU1) ? (MAD_JPEG_DISPLAY_ADR | MIU_INTERVAL) : (MAD_JPEG_DISPLAY_ADR));
    MM_Mem.u32MADJpegDisplaySize = MAD_JPEG_DISPLAY_LEN; 
    MM_Mem.u32MADJpegOutAddr = ((MAD_JPEG_OUT_MEMORY_TYPE & MIU1) ? (MAD_JPEG_OUT_ADR | MIU_INTERVAL) : (MAD_JPEG_OUT_ADR));
    MM_Mem.u32MADJpegOutSize = MAD_JPEG_OUT_LEN;     
    MM_Mem.u32MADJpegReadBufAddr = ((MAD_JPEG_READBUFF_MEMORY_TYPE & MIU1) ? (MAD_JPEG_READBUFF_ADR | MIU_INTERVAL) : (MAD_JPEG_READBUFF_ADR));
    MM_Mem.u32MADJpegReadBufSize = MAD_JPEG_READBUFF_LEN; 
    MM_Mem.u32MADJpegInterBufAddr = ((MAD_JPEG_INTERBUFF_MEMORY_TYPE & MIU1) ? (MAD_JPEG_INTERBUFF_ADR | MIU_INTERVAL) : (MAD_JPEG_INTERBUFF_ADR));
    MM_Mem.u32MADJpegInterBufSize = MAD_JPEG_INTERBUFF_LEN; 
    MM_Mem.u32PhotoDataReadBufAddr = ((PHOTO_DATA_READ_MEMORY_TYPE & MIU1) ? (PHOTO_DATA_READ_ADR | MIU_INTERVAL) : (PHOTO_DATA_READ_ADR));
    MM_Mem.u32PhotoDataReadBufSize = PHOTO_DATA_READ_LEN;       
    
    //Video
    #if(MEMORY_MAP==MMAP_128MB)
    MM_Mem.u32VDPlayerDataAddr = ((VDPLAYER_DATA_MEMORY_TYPE & MIU1) ? (VDPLAYER_DATA_ADR | MIU_INTERVAL) : (VDPLAYER_DATA_ADR));
    MM_Mem.u32VDPlayerDataSize = VDPLAYER_DATA_LEN;
    #else
    MM_Mem.u32VDPlayerDataAddr = MsOS_AllocateMemory(VDPLAYER_DATA_LEN, gs32NonCachedPoolID);
    MM_Mem.u32VDPlayerDataAddr = MsOS_VA2PA(MM_Mem.u32VDPlayerDataAddr);
    MM_Mem.u32VDPlayerDataSize = VDPLAYER_DATA_LEN;
    #endif
         
    MM_Mem.u32VDPlayerBitstreamBufAddr = ((VDPLAYER_BITSTREAM_BUFF_MEMORY_TYPE & MIU1) ? (VDPLAYER_BITSTREAM_BUFF_ADR | MIU_INTERVAL) : (VDPLAYER_BITSTREAM_BUFF_ADR));
    MM_Mem.u32VDPlayerBitstreamBufSize = VDPLAYER_BITSTREAM_BUFF_LEN;     
    MM_Mem.u32VDPlayerBitstreamBufEXTAddr = ((VDPLAYER_BITSTREAM_BUFF_EXT_MEMORY_TYPE & MIU1) ? (VDPLAYER_BITSTREAM_BUFF_EXT_ADR | MIU_INTERVAL) : (VDPLAYER_BITSTREAM_BUFF_EXT_ADR));
    MM_Mem.u32VDPlayerBitstreamBufEXTSize = VDPLAYER_BITSTREAM_BUFF_EXT_LEN;
    #if(MEMORY_MAP==MMAP_128MB)  
    MM_Mem.u32VDPlayerSubtitleBitstreamBufAddr = ((VDPLAYER_SUBTITLE_BITSTREAM_BUFF_MEMORY_TYPE & MIU1) ? (VDPLAYER_SUBTITLE_BITSTREAM_BUFF_ADR | MIU_INTERVAL) : (VDPLAYER_SUBTITLE_BITSTREAM_BUFF_ADR));
    MM_Mem.u32VDPlayerSubtitleBitstreamBufSize = VDPLAYER_SUBTITLE_BITSTREAM_BUFF_LEN;
    MM_Mem.u32MP4SubtitleBufAddr = ((MP4_SUBTITLE_BUFFER_MEMORY_TYPE & MIU1) ? (MP4_SUBTITLE_BUFFER_ADR | MIU_INTERVAL) : (MP4_SUBTITLE_BUFFER_ADR));
    MM_Mem.u32MP4SubtitleBufSize = MP4_SUBTITLE_BUFFER_LEN;
    MM_Mem.u32MP4SubtitleTAGAddr = ((MP4_SUBTITLE_TAG_MEMORY_TYPE & MIU1) ? (MP4_SUBTITLE_TAG_ADR | MIU_INTERVAL) : (MP4_SUBTITLE_TAG_ADR));
    MM_Mem.u32MP4SubtitleTAGSize = MP4_SUBTITLE_TAG_LEN;
    #else
    MM_Mem.u32VDPlayerSubtitleBitstreamBufAddr = MsOS_AllocateMemory(VDPLAYER_SUBTITLE_BITSTREAM_BUFF_LEN, gs32NonCachedPoolID);
    MM_Mem.u32VDPlayerSubtitleBitstreamBufAddr = MsOS_VA2PA(MM_Mem.u32VDPlayerSubtitleBitstreamBufAddr);
    MM_Mem.u32VDPlayerSubtitleBitstreamBufSize = VDPLAYER_SUBTITLE_BITSTREAM_BUFF_LEN;
    
    MM_Mem.u32MP4SubtitleBufAddr = MsOS_AllocateMemory(MP4_SUBTITLE_BUFFER_LEN, gs32NonCachedPoolID);
    MM_Mem.u32MP4SubtitleBufAddr = MsOS_VA2PA(MM_Mem.u32MP4SubtitleBufAddr);
    MM_Mem.u32MP4SubtitleBufSize = MP4_SUBTITLE_BUFFER_LEN;
    
    MM_Mem.u32MP4SubtitleTAGAddr = MsOS_AllocateMemory(MP4_SUBTITLE_TAG_LEN, gs32NonCachedPoolID);
    MM_Mem.u32MP4SubtitleTAGAddr = MsOS_VA2PA(MM_Mem.u32MP4SubtitleTAGAddr);
    MM_Mem.u32MP4SubtitleTAGSize = MP4_SUBTITLE_TAG_LEN;            
    #endif          
    MM_Mem.u32SubtitleAddr = ((SUBTITLE_MEMORY_TYPE & MIU1) ? (SUBTITLE_ADR | MIU_INTERVAL) : (SUBTITLE_ADR));
    MM_Mem.u32SubtitleSize = SUBTITLE_LEN;    
    MM_Mem.u32VDECFWBufAddr = ((VDEC_FW_BUF_MEMORY_TYPE & MIU1) ? (VDEC_FW_BUF_ADDR | MIU_INTERVAL) : (VDEC_FW_BUF_ADDR));
    MM_Mem.u32VDECFWBufSize = VDEC_FW_BUF_LEN;    
      
    //TXT
    MM_Mem.u32TEXTFILEAddr = ((TEXT_FILE_MEMORY_TYPE & MIU1) ? (TEXT_FILE_ADR | MIU_INTERVAL) : (TEXT_FILE_ADR));
    MM_Mem.u32TEXTFILESize = TEXT_FILE_LEN; 
    MM_Mem.u32PoolBufAddr = ((POOL_BUFFER_MEMORY_TYPE & MIU1) ? (POOL_BUFFER_ADR | MIU_INTERVAL) : (POOL_BUFFER_ADR));
    MM_Mem.u32PoolBufSize = POOL_BUFFER_LEN;
    //Audio
    MM_Mem.u32MAD_SE_BufAddr = ((MAD_SE_BUFFER_MEMORY_TYPE & MIU1) ? (MAD_SE_BUFFER_ADR | MIU_INTERVAL) : (MAD_SE_BUFFER_ADR));
    MM_Mem.u32MAD_SE_BufSize = MAD_SE_BUFFER_LEN;
    MM_Mem.u32MAD_DEC_BufAddr = ((MAD_DEC_BUFFER_MEMORY_TYPE & MIU1) ? (MAD_DEC_BUFFER_ADR | MIU_INTERVAL) : (MAD_DEC_BUFFER_ADR)) ;
    MM_Mem.u32MAD_DEC_BufSize = MAD_DEC_BUFFER_LEN;     
    MM_Mem.u32MAD_Base_BufAddr = ((MAD_BASE_BUFFER_MEMORY_TYPE & MIU1) ? (MAD_BASE_BUFFER_ADR | MIU_INTERVAL) : (MAD_BASE_BUFFER_ADR));
    MM_Mem.u32MAD_Base_BufSize = MAD_BASE_BUFFER_LEN; 
    //VDEC XC TSP
    MM_Mem.u32ScalerDNRBufAddr = MAIN_FB_START_ADDR;
    MM_Mem.u32ScalerDNRBufSize = SCALER_DNR_BUF_LEN;
    MM_Mem.u32TSPBufAddr = TSP_FW_BUF;
    MM_Mem.u32TSPBufSize = TSP_FW_LEN;
    MM_Mem.u32VDECAEONBufAddr = ((VDEC_AEON_MEMORY_TYPE & MIU1) ? (VDEC_AEON_ADR | MIU_INTERVAL) : (VDEC_AEON_ADR));
    MM_Mem.u32VDECAEONBufSize = VDEC_AEON_LEN;
    MM_Mem.u32VDECFrameBufAddr = ((VDEC_FRAMEBUFFER_MEMORY_TYPE & MIU1) ? (VDEC_FRAMEBUFFER_ADR | MIU_INTERVAL) : (VDEC_FRAMEBUFFER_ADR));
    MM_Mem.u32VDECFrameBufSize = VDEC_FRAMEBUFFER_LEN;
    MM_Mem.u32VDECBitstreamBufAddr = ((VDEC_BITSTREAM_MEMORY_TYPE & MIU1) ? (VDEC_BITSTREAM_ADR | MIU_INTERVAL) : (VDEC_BITSTREAM_ADR));
    MM_Mem.u32VDECBitstreamBufSize = VDEC_BITSTREAM_LEN;
    MM_Mem.u32VDEC_SD_FrameBufAddr = MM_Mem.u32VDECFrameBufAddr;
    MM_Mem.u32VDEC_SD_FrameBufSize = MM_Mem.u32VDECFrameBufSize;
    MM_Mem.u32VDEC_SD_BitstreamBufAddr = MM_Mem.u32VDECBitstreamBufAddr;
    MM_Mem.u32VDEC_SD_BitstreamBufSize = MM_Mem.u32VDECBitstreamBufSize;  

    MM_Memory_Config(&MM_Mem);
}*/

#endif
