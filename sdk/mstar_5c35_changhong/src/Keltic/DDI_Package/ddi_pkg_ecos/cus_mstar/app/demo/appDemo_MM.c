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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   appDemo_MM.h
/// @author MStar Semiconductor Inc.
/// @brief Multi-Media file playback control Demo
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_MM_PACKAGE
#include<ctype.h>
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsTypes2.h"
#include "MsMemory.h"
#include <string.h>
#include "Board.h"
#include "apiAUDIO.h"
#include "apiDAC.h"
#include "drvAUDIO.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "apiDMX.h"
#include "apiHDMITx.h"
#include "MsOS.h"
#include "drvTVEncoder.h"
#include "drvMVOP.h"

#include "msAPI_MM.h"
#include "MSrv_ZmplayerSubtitleDecoder.h"
#include "appDemo_MM_SubRender.h"

#include "drvGPIO.h"
#include "apiAUDIO.h"

#ifdef ENABLE_RTSP_PACKAGE
//#include "msAPI_RTSP_ES.h"
#include "msAPI_RTSP_TS.h"
#include "RTSP_clientAPI.hh"
#include "msAPI_RTSP.h"
#include "rtsp_client_api.h"
#endif

#include "appDemo_XC.h"
#include "appDemo_GE.h"
#include "msAPI_XC.h"
#include "msAPI_VE.h"
#include "appDemo_AV.h"
#include "appDemo_MM.h"
#include "appDemo_DMX.h"
#include "appDemo_Audio.h"
#include "MsFS.h"
#define MM_DBG 0
#if MM_DBG
#define mm_print(fmt, args...)  printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define mm_print(fmt, args...)  while(0);
#endif

/* MM co-procesor   */
#define MM_COPROCESSOR_ADDR                     0x00000000  /* Alignment 0 */
#define MM_COPROCESSOR_LEN                      0x00200000 /* 2M */
#define MM_COPROCESSOR_TYPE                     (MIU0)


//will handle in middleware
#define MM_REMOVE_TXT_SUBTITLE 1
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_RTSP_PACKAGE
static MS_U32 VEsCounter;
static MS_U32 u32PtsCnt0,u32PrevPts0;
static MS_U32 AEsCounter;
static MS_U32  u32PtsCnt1, u32PrevPts1;

static MS_U32 u32VEsDataAddr;
static MS_U32 u32VEsDataLen;

static MS_U32 u32AEsDataAddr;
static MS_U32 u32AEsDataLen;

//static char *pDataAddr = NULL;
extern MS_S32 gs32NonCachedPoolID;

//#define MALLOC_SIZE (1024 * 1024 * 3 )
#endif


#ifdef ENABLE_RTSP_PACKAGE
//char utf8fn[FULLPATH_NAME_SIZE+1];
rtsp_error_code err;
rtsp_media_info mediainfo;
rtsp_handle g_rhandle;//from pplayer
unsigned long rtsptask;
int rtsplength = 0;//from pplayer
U32 es_buffer=60;

#endif
MS_BOOL b_MM_Initied = FALSE;
static MS_U8 u8TSPFirmwareForRTSP[] = {
    #include "../../../bsp/lib/fw/fwTSP.dat"
};
static MS_U32 u32Current = 0;
static MS_U32 u32Previous = 0;
static EN_MPLAYER_FILE_MODE eZmpMode = MPLAYER_FILE_MODE;
static EN_MPLAYER_MEDIA_TYPE eMediaType = E_MPLAYER_MEDIA_MOVIE;
static MPLAYER_STREAM_IOCB _s_sStreamHandle;

#define FILE_NAME_LENGTH 1024
static MS_U32 _u32FileOption = MPLAYER_MODE_INVALID;
static MS_U8 _pu8FileName[FILE_NAME_LENGTH];
static MS_U8 u8IsRepeat = FALSE;
static MS_BOOL b_InternalSubtitleInitialize = FALSE;

/* common data & functions */
///  the struct of queue element
typedef struct _QueueElement
{
    /// the pointer of the queue element buffer
    MS_U8 *pu8Buf;
    /// the queue length, the maximum PES length is 64k
    MS_U16 u16Len;
    /// the queue PTS
    MS_U32 u32PTS;
    /// the queue duration for display
    MS_U32 u32Duration;
} QueueElement;
#if MM_REMOVE_TXT_SUBTITLE == 0
#define Q_SIZE 100 //just an arbitrary num
QueueElement m_QElement[Q_SIZE] = {};
static MS_S32 s32QFront = 0;
static MS_S32 s32QBack = 0;

static MS_BOOL _bExitSubTask = TRUE;
static S32 _s32QMutex = -1;
static S32 _s32SubTaskExitEvt = -1;
static Task_Info m_InternalSubThread = {-1, E_TASK_PRI_HIGH, NULL, 8*(16<<10), "InternalSub"};
#endif
/* Definitions */
#define I_SUBTITLE_DFB_CHECK(x)                          \
    if ((DFBResult) x != DFB_OK)                                \
    {                                               \
        printf("  SBT DFB fail:0x%x ASSERT: %s %s %d\n", x, __FILE__, __FUNCTION__, __LINE__);     \
    }

static const Z_File_Ext_Info _zmm_support_ext_Table[] =
{
    //Auio support Suffixes name
    {"AAC",      E_MPLAYER_MEDIA_MUSIC},
    {"M4A",      E_MPLAYER_MEDIA_MUSIC},
    {"MP3",      E_MPLAYER_MEDIA_MUSIC},
    {"WMA",     E_MPLAYER_MEDIA_MUSIC},
    {"OGG",     E_MPLAYER_MEDIA_MUSIC},
    {"WAV",     E_MPLAYER_MEDIA_MUSIC},
    {"PCM",     E_MPLAYER_MEDIA_MUSIC},
    {"FLAC",     E_MPLAYER_MEDIA_MUSIC},
    //{"AC3",     EN_MM_FILTER_MUSIC},

    //Video support Suffixes name
    {"MP4",     E_MPLAYER_MEDIA_MOVIE},
    {"MPG",     E_MPLAYER_MEDIA_MOVIE},
    {"AVI",      E_MPLAYER_MEDIA_MOVIE},
    {"DAT",     E_MPLAYER_MEDIA_MOVIE},
    {"VOB",     E_MPLAYER_MEDIA_MOVIE},
    {"MOV",     E_MPLAYER_MEDIA_MOVIE},
    {"MKV",     E_MPLAYER_MEDIA_MOVIE},
    {"DIV",      E_MPLAYER_MEDIA_MOVIE},
    {"DIVX",    E_MPLAYER_MEDIA_MOVIE},
    {"TS",        E_MPLAYER_MEDIA_MOVIE},
    {"MPEG",    E_MPLAYER_MEDIA_MOVIE},
    {"DAT",      E_MPLAYER_MEDIA_MOVIE},
    {"MPA",      E_MPLAYER_MEDIA_MOVIE},
    {"TP",        E_MPLAYER_MEDIA_MOVIE},
    {"TP",        E_MPLAYER_MEDIA_MOVIE},
    {"M2TS",    E_MPLAYER_MEDIA_MOVIE},
    {"M2T1",    E_MPLAYER_MEDIA_MOVIE},
    {"MPE",      E_MPLAYER_MEDIA_MOVIE},
    {"TRP",      E_MPLAYER_MEDIA_MOVIE},
    {"TP",        E_MPLAYER_MEDIA_MOVIE},
    {"3GP",     E_MPLAYER_MEDIA_MOVIE},
    {"RMVB",     E_MPLAYER_MEDIA_MOVIE},
    {"RM",     E_MPLAYER_MEDIA_MOVIE},
    {"FLV",     E_MPLAYER_MEDIA_MOVIE},
    {"ASF",     E_MPLAYER_MEDIA_MOVIE},
    {"WMV",     E_MPLAYER_MEDIA_MOVIE},

    //Photo support Suffixes name
    {"BMP",     E_MPLAYER_MEDIA_PHOTO},
    {"PNG",     E_MPLAYER_MEDIA_PHOTO},
    {"JPG",      E_MPLAYER_MEDIA_PHOTO},
    {"JPEG",    E_MPLAYER_MEDIA_PHOTO},
    {"JPE",      E_MPLAYER_MEDIA_PHOTO},
};

extern MS_S32 gs32CachedPoolID;
extern MS_S32 gs32NonCachedPoolID;
extern unsigned char Customer_info[49];
extern MS_U8 Customer_hash[];
MS_BOOL appDemo_MM_Play(MS_U32 *pFileOption, MS_U8* U8FileName);

#if MM_REMOVE_TXT_SUBTITLE == 0
static void _MM_TxtSubtitleInit(void);
static void _MM_TxtSubtitleFinalize(void);
#endif
//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _appDemo_MM_MemInit(MM_MEM_INFO *pMem_Info, MM_MEMMAP_INFO *pMemMap_Info)
{
    MS_U32 i;
    if(pMemMap_Info == NULL)
        return FALSE;

    for(i = 0; i < pMem_Info->u32Mmap_items; i++)
    {
        if(i == E_MMAP_ID_VDEC_CPU)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VDEC_CPU;
            (pMemMap_Info+i)->u32Addr = VDEC_AEON_ADR;
            (pMemMap_Info+i)->u32Size = VDEC_AEON_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000010;
            (pMemMap_Info+i)->bIs_miu0 = (VDEC_AEON_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_VDEC_FRAMEBUFFER)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VDEC_FRAMEBUFFER;
            (pMemMap_Info+i)->u32Addr = VDEC_FRAME_BUF_ADR;
            (pMemMap_Info+i)->u32Size = VDEC_FRAME_BUF_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000200;
            (pMemMap_Info+i)->bIs_miu0 = (VDEC_FRAME_BUF_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_VDEC_BITSTREAM)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VDEC_BITSTREAM;
            (pMemMap_Info+i)->u32Addr = VDEC_BIT_STREAM_ADR;
            (pMemMap_Info+i)->u32Size = VDEC_BIT_STREAM_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000008;
            (pMemMap_Info+i)->bIs_miu0 = (VDEC_BIT_STREAM_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_XC_MAIN_FB)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_XC_MAIN_FB;
            (pMemMap_Info+i)->u32Addr = SC0_MAIN_FB_ADR;
            (pMemMap_Info+i)->u32Size = SC0_MAIN_FB_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000010;
            (pMemMap_Info+i)->bIs_miu0 = (SC0_MAIN_FB_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_VE)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VE;
            (pMemMap_Info+i)->u32Addr = VE_FRAME_BUF_ADR;
            (pMemMap_Info+i)->u32Size = VE_FRAME_BUF_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000008;
            (pMemMap_Info+i)->bIs_miu0 = (VE_FRAME_BUF_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_MAD_DEC)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_MAD_DEC;
            (pMemMap_Info+i)->u32Addr = MAD_DEC_BUF_ADR;
            (pMemMap_Info+i)->u32Size = MAD_DEC_BUF_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00001000;
            (pMemMap_Info+i)->bIs_miu0 = (MAD_DEC_BUF_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_COPROCESSOR)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_COPROCESSOR;
            (pMemMap_Info+i)->u32Addr = MM_COPROCESSOR_ADDR;
            (pMemMap_Info+i)->u32Size = MM_COPROCESSOR_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000000;
            (pMemMap_Info+i)->bIs_miu0 = (MM_COPROCESSOR_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_VDPLAYER_DATA)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VDPLAYER_DATA;
            (pMemMap_Info+i)->u32Addr = VDPLAYER_DATA_ADR;
            (pMemMap_Info+i)->u32Size = VDPLAYER_DATA_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000000;
            (pMemMap_Info+i)->bIs_miu0 = (VDPLAYER_DATA_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_VDPLAYER_BITSTREAM)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VDPLAYER_BITSTREAM;
            (pMemMap_Info+i)->u32Addr = VDPLAYER_BS_ADR;
            (pMemMap_Info+i)->u32Size = VDPLAYER_BS_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000000;
            (pMemMap_Info+i)->bIs_miu0 = (VDPLAYER_BS_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_VDPLAYER_BITSTREAM_EXT)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VDPLAYER_BITSTREAM_EXT;
            (pMemMap_Info+i)->u32Addr = VDPLAYER_BS_EXT_ADR;
            (pMemMap_Info+i)->u32Size = VDPLAYER_BS_EXT_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000000;
            (pMemMap_Info+i)->bIs_miu0 = (VDPLAYER_BS_EXT_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_VDPLAYER_SUBTITLE_BITSTREAM_BUFF)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_VDPLAYER_SUBTITLE_BITSTREAM_BUFF;
            (pMemMap_Info+i)->u32Addr = VDPLAYER_SUB_BS_ADR;
            (pMemMap_Info+i)->u32Size = VDPLAYER_SUB_BS_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000000;
            (pMemMap_Info+i)->bIs_miu0 = (VDPLAYER_SUB_BS_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_MM_SUBTITLE_BUFFER)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_MM_SUBTITLE_BUFFER;
            (pMemMap_Info+i)->u32Addr = VDPLAYER_SUB_ADR;
            (pMemMap_Info+i)->u32Size = VDPLAYER_SUB_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00000000;
            (pMemMap_Info+i)->bIs_miu0 = (VDPLAYER_SUB_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_DMX_SECBUF)
        {
            (pMemMap_Info+i)->u32ID = E_MMAP_ID_DMX_SECBUF;
            (pMemMap_Info+i)->u32Addr = VDPLAYER_DMX_SEC_ADR;
            (pMemMap_Info+i)->u32Size = VDPLAYER_DMX_SEC_LEN;
            (pMemMap_Info+i)->u8Layer = 0;
            (pMemMap_Info+i)->u32Align = 0x00010000;
            (pMemMap_Info+i)->bIs_miu0 = (VDPLAYER_DMX_SEC_MEMORY_TYPE & 0x01) ? 0:1;

        }
        else if(i == E_MMAP_ID_PHOTO_INTER)
        {
             (pMemMap_Info+i)->u32ID = E_MMAP_ID_PHOTO_INTER;
             (pMemMap_Info+i)->u32Addr = JPD_INTER_BUF_ADR;
             (pMemMap_Info+i)->u32Size = JPD_INTER_BUF_LEN;
             (pMemMap_Info+i)->u8Layer = 0;
             (pMemMap_Info+i)->u32Align = 0x00001000;
             (pMemMap_Info+i)->bIs_miu0 = (JPD_INTER_BUF_MEMORY_TYPE & 0x01) ? 0:1;
        }
        else if(i == E_MMAP_ID_JPD_READ)
        {
             (pMemMap_Info+i)->u32ID = E_MMAP_ID_JPD_READ;
             (pMemMap_Info+i)->u32Addr = JPD_READ_BUF_ADR;
             (pMemMap_Info+i)->u32Size = JPD_READ_BUF_LEN;
             (pMemMap_Info+i)->u8Layer = 0;
             (pMemMap_Info+i)->u32Align = 0x00001000;
             (pMemMap_Info+i)->bIs_miu0 = (JPD_READ_BUF_MEMORY_TYPE & 0x01) ? 0:1;
        }
        else if(i == E_MMAP_ID_JPD_WRITE)
        {
             (pMemMap_Info+i)->u32ID = E_MMAP_ID_JPD_WRITE;
             (pMemMap_Info+i)->u32Addr = JPD_OUT_ADR;
             (pMemMap_Info+i)->u32Size = JPD_OUT_LEN;
             (pMemMap_Info+i)->u8Layer = 0;
             (pMemMap_Info+i)->u32Align = 0x00001000;
             (pMemMap_Info+i)->bIs_miu0 = (JPD_OUT_MEMORY_TYPE & 0x01) ? 0:1;
        }
        else if(i == E_MMAP_ID_PHOTO_SHAREMEM)
        {
             (pMemMap_Info+i)->u32ID = E_MMAP_ID_PHOTO_SHAREMEM;
             (pMemMap_Info+i)->u32Addr = PHOTO_SHARE_MEM_ADR;
             (pMemMap_Info+i)->u32Size =PHOTO_SHARE_MEM_LEN;
             (pMemMap_Info+i)->u8Layer = 0;
             (pMemMap_Info+i)->u32Align = 0x00001000;
             (pMemMap_Info+i)->bIs_miu0 = (PHOTO_SHARE_MEM_MEMORY_TYPE & 0x01) ? 0:1;
        }
        else if(i == E_MMAP_ID_DMX_SECBUF)
        {
             (pMemMap_Info+i)->u32ID = E_MMAP_ID_DMX_SECBUF;
             (pMemMap_Info+i)->u32Addr = TSP_FW_BUF_ADR;
             (pMemMap_Info+i)->u32Size =TSP_FW_BUF_LEN;
             (pMemMap_Info+i)->u8Layer = 0;
             (pMemMap_Info+i)->u32Align = 0x00000000;
             (pMemMap_Info+i)->bIs_miu0 = (TSP_FW_BUF_MEMORY_TYPE & 0x01) ? 0:1;
        }
        else if(i == E_MMAP_ID_DUMMY2)
        {
             (pMemMap_Info+i)->u32ID = E_MMAP_ID_DUMMY2;
             (pMemMap_Info+i)->u32Addr = TSP_VQ_BUF_ADR;
             (pMemMap_Info+i)->u32Size =TSP_VQ_BUF_LEN;
             (pMemMap_Info+i)->u8Layer = 0;
             (pMemMap_Info+i)->u32Align = 0x00000000;
             (pMemMap_Info+i)->bIs_miu0 = (TSP_VQ_BUF_MEMORY_TYPE & 0x01) ? 0:1;
        }


    }
    return TRUE;
}
static ST_CPCODEC_MSG_SUBTITLE_INIT_INFO _prestSubtitleInit;

static void _SubInitHandler(unsigned int  u32Info)
{
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    mm_print("_SubInitHandler()\n");
    ST_CPCODEC_MSG_SUBTITLE_INIT_INFO *stSubtitle = (ST_CPCODEC_MSG_SUBTITLE_INIT_INFO *)u32Info;

    if(stSubtitle->bInit && !b_InternalSubtitleInitialize)
    {
        VDEC_EX_DispInfo   info = {0};
        MApi_VDEC_EX_GetDispInfo(stVDECStreamId,&info);
        if((info.u16HorSize== 0) || (info.u16VerSize == 0))
        {
            printf("Post to Check\n");
            return;
        }
        // to prevent from UI doesn't close subtitle correctly
        MApi_MPlayer_Close();
        MApi_MPlayer_Disconnect();


        SUBTITLE_IRenderTarget m_RenderTarge =
        {
            MM_SubRender_Create,
            MM_SubRender_GetVideoSizeFromVdec,
            MM_SubRender_Open,
            MM_SubRender_Close,
            MM_SubRender_CreateWindow,
            MM_SubRender_DestroyWindow,
            MM_SubRender_SetPalette,
            MM_SubRender_Bitblt,
            MM_SubRender_ClearWindowByIndex,
            MM_SubRender_ClearWindowByColor,
            MM_SubRender_GetWindowInfo,
            MM_SubRender_Lock,
            MM_SubRender_UnLock,
            MM_SubRender_RenderFont,
            MM_SubRender_Show,
            MM_SubRender_Unshow
        };
        switch(stSubtitle->eType)
        {
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TEXT:
                printf("Subtitle Type: TEXT\n");
                MApi_MPlayer_Set_Subtitle_Type(E_SUBTITLE_TYPE_TTF);
                break;
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS_PGS:
                printf("Subtitle Type: ts-pgs\n");
                MApi_MPlayer_Set_Subtitle_Type(E_SUBTITLE_TYPE_TS);
                break;
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS_BMP:
                printf("Subtitle Type: ts-bmp(dvb-subtitle)\n");
                MApi_MPlayer_Set_Subtitle_Type(E_SUBTITLE_TYPE_TS);
                break;
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_IMAGE_BMP:
                printf("Subtitle Type: image-bmp\n");
                MApi_MPlayer_Set_Subtitle_Type(E_SUBTITLE_TYPE_BMP);
                break;
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_IMAGE_VOBSUB:
                printf("Subtitle Type: vob-sub\n");
                MApi_MPlayer_Set_Subtitle_Type(E_SUBTITLE_TYPE_VOBSUB);
                break;
            default:
                printf("Subtitle Type: unknown\n");
                MApi_MPlayer_Set_Subtitle_Type(E_SUBTITLE_TYPE_UNKNOWN);
                break;
        }


        switch(stSubtitle->eType)
        {
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_IMAGE_BMP:
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_IMAGE_VOBSUB:
                //use default dimension
                //MApi_MPlayer_Set_Subtitle_Dimension(stSubtitle->u16BmpWidth, stSubtitle->u16BmpHeight);
                MApi_MPlayer_Connect(&m_RenderTarge);
                MApi_MPlayer_Open();
                break;
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS_PGS:
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TEXT:
            case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS_BMP:
                MApi_MPlayer_Connect(&m_RenderTarge);
                MApi_MPlayer_Open();
                break;
            default:
                break;
        }

        b_InternalSubtitleInitialize = TRUE;
    }
    //De-INIT
    else if(!stSubtitle->bInit && b_InternalSubtitleInitialize)
    {
        MApi_MPlayer_Close();
        MApi_MPlayer_Disconnect();
        b_InternalSubtitleInitialize = FALSE;
    }
    else
        mm_print("Subtitle init or de-init fail!!\n");
}

static void _SubQueuePushHandler(unsigned int  u32Info)
{
    mm_print("_SubQueuePushHandler()\n");
    ST_CPCODEC_MSG_SUBQUEUE_PUSH_INFO *stQueueInfo = (ST_CPCODEC_MSG_SUBQUEUE_PUSH_INFO *)u32Info;
    switch(stQueueInfo->eType)
    {
        case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS_PGS:
            MApi_MPlayer_Push_SpuQueue((U8 *)stQueueInfo->u32QueueStar, (U16)stQueueInfo->u32QueueSize, (stQueueInfo->u32PTSStart), E_SUBTITLE_TYPE_TS, 0);
            break;
        case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS_BMP:
            MApi_MPlayer_Push_SpuQueue((U8 *)stQueueInfo->u32QueueStar, (U16)stQueueInfo->u32QueueSize, (stQueueInfo->u32PTSStart), E_SUBTITLE_TYPE_TS, 0);
            break;
        case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TEXT:
            MApi_MPlayer_Push_SpuQueue((U8 *)stQueueInfo->u32QueueStar, (U16)stQueueInfo->u32QueueSize, ( stQueueInfo->u32PTSStart), E_SUBTITLE_TYPE_TTF, (stQueueInfo->u32PTSEnd - stQueueInfo->u32PTSStart));
            break;
        case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_IMAGE_BMP :
            MApi_MPlayer_Push_SpuQueue((U8 *)stQueueInfo->u32QueueStar, (U16)stQueueInfo->u32QueueSize, (stQueueInfo->u32PTSStart * 90), E_SUBTITLE_TYPE_BMP, 0);
            break;
        case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_IMAGE_VOBSUB:
            MApi_MPlayer_Push_SpuQueue((U8 *)stQueueInfo->u32QueueStar, (U16)stQueueInfo->u32QueueSize, (stQueueInfo->u32PTSStart * 90), E_SUBTITLE_TYPE_VOBSUB, 0);
            break;
#if MM_REMOVE_TXT_SUBTITLE == 0
        case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TEXT:
            if(_bExitSubTask)
                break;

            MsOS_ObtainMutex(_s32QMutex, MSOS_WAIT_FOREVER);
            if((s32QBack + 1) % Q_SIZE == s32QFront)
            {
                printf("@@@Queue is full!!!@@@\n");
                break;//drop data
            }
            m_QElement[s32QBack].pu8Buf = (MS_U8*)MsOS_AllocateMemory(stQueueInfo->u32QueueSize, gs32NonCachedPoolID);
            memcpy(m_QElement[s32QBack].pu8Buf, (void*)stQueueInfo->u32QueueStar, stQueueInfo->u32QueueSize * sizeof(MS_U8));
            m_QElement[s32QBack].u16Len = stQueueInfo->u32QueueSize;
            m_QElement[s32QBack].u32PTS = stQueueInfo->u32PTSStart;
            m_QElement[s32QBack].u32Duration = stQueueInfo->u32PTSStart - stQueueInfo->u32PTSEnd;

            s32QBack++;
            if(s32QBack >= Q_SIZE)
                s32QBack = 0;
            MsOS_ReleaseMutex(_s32QMutex);
            break;
#endif
        default:
            break;
    }
}

static void _SubtitleDisplayHandler(unsigned int  u32Info)
{
    ST_CPCODEC_MSG_SUBTITLE_INIT_INFO *stSubtitle = (ST_CPCODEC_MSG_SUBTITLE_INIT_INFO *)u32Info;
    switch(stSubtitle->eType)
    {
        case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_TS_BMP:
        case E_CPCODEC_INTERNAL_SUBTITLE_TYPE_IMAGE_VOBSUB:
            if(stSubtitle->bShow)
            {
                MApi_MPlayer_Open();
            }
            else
            {
                MApi_MPlayer_Close();
            }
         break;
    default:
        break;
    }

}

static void _MApp_Mplayer_Movie_CallBack(EN_MPLAYER_COMMAND_TYPE eCmd, unsigned int  u32Param, unsigned int  u32Info)
{
    mm_print("\n[%d]Movie CallBack[0x%x][0x%x][0x%x]\n", __LINE__, eCmd, u32Param, u32Info);
    switch (eCmd)
    {
        case MPLAYER_COMMAND_NOTIFY:
            mm_print("Got notify from zmplayer[%d]\n", u32Param);
            switch(u32Param)
            {
                case MPLAYER_FRAME_READY:
                    _SubInitHandler((MS_U32)&_prestSubtitleInit);
                    mm_print("Movie frame ready!\n");
                    //appDemo_XC_PlayVideo_UsingDefaultSetting();
                    break;
                case MPLAYER_EXIT_OK:
                    mm_print("Movie exit ok!\n");
#if MM_REMOVE_TXT_SUBTITLE == 0
                    _MM_TxtSubtitleFinalize();
#endif
                    MApi_MPlayer_Stop(E_MPLAYER_MEDIA_MOVIE);
                    msAPI_VE_SetVideoMute(ENABLE);
                    if(u8IsRepeat)
                        appDemo_MM_Play(&_u32FileOption, _pu8FileName);
                    break;
                case MPLAYER_START_PLAY:
                    mm_print("Movie Init ok!\n");
                    break;

                case MPLAYER_EXIT_ERROR_FILE:
                    printf("Movie unsupport file!\n");
#if MM_REMOVE_TXT_SUBTITLE == 0
                    _MM_TxtSubtitleFinalize();
#endif
                    MApi_MPlayer_Stop(E_MPLAYER_MEDIA_MOVIE);
                    msAPI_VE_SetVideoMute(ENABLE);
                    break;

                case MPLAYER_SUFFICIENT_DATA:
                    mm_print("Movie sufficient data!\n");
                    break;
                case MPLAYER_BLACKSCREEN_OFF:
                    appDemo_XC_PlayVideo_UsingDefaultSetting();
                    break;
            }
            break;

        case MPLAYER_COMMAND_SUBTITLE:
            mm_print("Got subtitle [%d][%x]\n", u32Param, u32Info);
            switch(u32Param)
            {
                case MPLAYER_SET_SUBTITLE_PALETTE:
                    mm_print("MPLAYER_SET_SUBTITLE_PALETTE\n");
                    break;
                case MPLAYER_SUBTITLE_INIT:
                    memcpy(&_prestSubtitleInit, (void*)u32Info, sizeof(ST_CPCODEC_MSG_SUBTITLE_INIT_INFO));
                    _SubInitHandler((MS_U32)&_prestSubtitleInit);
                    break;
                case MPLAYER_SUBQUEUE_PUSH:
                    _SubQueuePushHandler(u32Info);
                    break;
                case MPLAYER_SUBTITLE_ENABLE:
                    mm_print("MPLAYER_SUBTITLE_ENABLE\n");
                    break;
                case MPLAYER_SET_SUBTITLE_TRACK:
                    mm_print("MPLAYER_SET_SUBTITLE_TRACK\n");
                    break;
                case MPLAYER_TS_SUBTITLE_PROGRAM_CHANGED:
                    mm_print("MPLAYER_TS_SUBTITLE_PROGRAM_CHANGED\n");
                    break;
                case MPLAYER_SUBTITLE_DISPLAY:
                    _SubtitleDisplayHandler(u32Info);
                    mm_print("MPLAYER_SUBTITLE_DISPLAY\n");
                    break;
            }
            break;

        default:
            break;
    }

}

static void _MApp_Mplayer_Photo_CallBack(EN_MPLAYER_COMMAND_TYPE eCmd, unsigned int  u32Param, unsigned int  u32Info)
{
    mm_print("\n[%d]Photo CallBack[0x%x][0x%x][0x%x]\n", __LINE__, eCmd, u32Param, u32Info);

    switch (eCmd)
    {
        case MPLAYER_COMMAND_NOTIFY:
            mm_print("Got notify [%d]\n", u32Param);
            switch(u32Param)
            {
                case MPLAYER_PHOTO_DECODE_DONE:
                    printf("---------------->Decode Successfully!\n");
                    break;
                case MPLAYER_PHOTO_DECODE_FAILED:
                    printf("---------------->Decode Error! \n");
                    break;
                case MPLAYER_EXIT_ERROR_FILE:
                    printf("---------------->Unsupported File! \n");
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
}

static void _MApp_Mplayer_Music_CallBack(EN_MPLAYER_COMMAND_TYPE eCmd, unsigned int  u32Param, unsigned int  u32Info)
{
    mm_print("\n[%d]Music CallBack[0x%x][0x%x][0x%x]\n", __LINE__, eCmd, u32Param, u32Info);
    switch (eCmd)
    {
        case MPLAYER_COMMAND_NOTIFY:
            mm_print("Got notify [%d]\n", u32Param);
            switch(u32Param)
            {
                case MPLAYER_START_PLAY:
                    printf("Music Init ok!\n");
                    break;

                case MPLAYER_EXIT_ERROR_FILE:
                    printf("Music unsupport file!\n");
                    printf("Music play fail!\n");
                    break;

                case MPLAYER_EXIT_OK:
                    printf("Music exit ok!\n");
                    MApi_MPlayer_Stop(E_MPLAYER_MEDIA_MUSIC);
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }

}


static void * MM_Simulate_Stream_Open_np(const char * pathname, const char * mode)
{
    mm_print("MM_Simulate_Stream_Open(%s, %s)\n", pathname, mode);

    FILE* fp = fopen(pathname, "rb");
    if (NULL==fp)
    {
        mm_print("fopen fail\n");
    }
    mm_print("FILE* fp %d\n", (int)fp);
    return (void *)fp;
}

static int MM_Simulate_Stream_Close_np(void * stream)
{
    mm_print("MM_Simulate_Stream_Close(%d)\n", (int)stream);
    return (fclose((FILE*)stream));
}

static MS_U64 MM_Simulate_Stream_Length_np(void * stream)
{
    MS_U64 len = 0;

    if(stream)
    {
        fseek((FILE*)stream, 0, SEEK_END);
        len = ftell((FILE*)stream);
        fseek((FILE*)stream, 0, SEEK_SET);
    }
    mm_print("MM_Simulate_Stream_Length_np len %lld\n",len);
    return len;
}

static int MM_Simulate_Stream_Seek_np(void * stream, MS_U64 offset, int fromwhere)
{
    mm_print("MM_Simulate_Stream_Seek\n");
    int ret;
    ret = fseek((FILE*)stream, offset, fromwhere);
    return ret;
}


static int MM_Simulate_Stream_Read_np(void *ptr, int size, int nitems, void *stream)
{
   mm_print("MM_Simulate_Stream_Read_np size %d nitems %d\n",size,nitems);
   /*
    struct timeval tv;
    fd_set myset;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    FD_ZERO(&myset);
    FD_SET(m_sockfd, &myset);
    if (select(m_sockfd+1, &myset, NULL, NULL, &tv) > 0)
    {
        if (FD_ISSET(m_sockfd, &myset))
            return read((int)stream, ptr, size*nitems);
    }
    else
    {
        mm_print("MSrv_MMProcess::MApi_NonSeekAble_Stream_Read read NULL\n");
        return 0;
    }
    return 0;
    */
   MS_S32 retSize = 0;

   u32Current = MsOS_GetSystemTime();
   //if(u32Current - u32Previous > 1000)
   {
       u32Previous = u32Current;
       retSize = fread(ptr, size, 20*1024, (FILE *)stream);
   }
   return retSize;
}


static char *pDataAddr = NULL;
#define MALLOC_SIZE (1024 * 1024 * 2)

//-----------------------------------------------------------------------------
#define RTSP_VES_RESERVE    0x8000
#define RTSP_AES_LENGTH     0xE0000
#define RTSP_AES_RESERVE    0x40//0x40000
#define RTSP_PES_LENGTH     0x20000

static U32 u32VEsDataAddr;
static U32 u32VEsDataLen;

static U32 u32AEsDataAddr;
static U32 u32AEsDataLen;

#define ADTS_MASK   0xFFFFFFFF
#define ADTS_BYTES  4
static U32 u32AdtsID;

static U32 u32VEsDataIndex;
static U32 u32VEsDataSize;
static U8 u8VEsPoolRing;

static U32 u32AEsDataIndex;
static U32 u32AEsDataSize;
static U8 u8AEsPoolRing;

static U32 u32PEsDataAddr;
static U32 u32PEsDataLen;

static FILE * audiofile = NULL;
static rtsp_audio_type eAudioType;
static BOOLEAN bEnablePes;

#define AU_PES_FILE      0
#if (AU_PES_FILE == 0)
//static U8 u8PESHead0[9] = {0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x80, 0x00, 0x00};
static U8 u8PESHead1[14] = {0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x80, 0x80, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00};
#define PES_HEAD    u8PESHead1
#endif

#define VD_PES_FILE     0
#define VD_PES_HEAD0     0x48454144
#define VD_PES_HEAD1     0x505453
#define VD_PES_MASK0   0xFFFFFFFF
#define VD_PES_MASK1   0xFFFFFF
#define VD_PES_BYTES  7

static void * MM_Simulate_Rtsp_Open(const char * rtsp_url, video_decoder_init_info* video_dec_info, audio_decoder_init_info* audio_dec_info)
{
    FILE * file;
	//const char *rtsp_url = "/usb/sda1/rtsp/out_wlkh_track1.h264";
	//const char *rtsp_url = "/usb/sda1/ch_pplive1.h264";//"/usb/sda/ch_pplive.h264";
    printf("MApi_Rtsp_Open video: %s\n", rtsp_url);
    file = fopen(rtsp_url, "rb");
    if (file == NULL)
    {
        printf("MApi_Rtsp_Open error!\n");
        return NULL;
    }

    video_dec_info->video_type = rtsp_h264;
	video_dec_info->avsync_enable = FALSE;
    audio_dec_info->audio_type = rtsp_mp3;

    audio_dec_info->avsync_enable = (AU_PES_FILE && VD_PES_FILE);
    audio_dec_info->errcnt_enable = ENABLE;
    audio_dec_info->pes_enable = TRUE;
    audio_dec_info->pes_head_length = 14;

    eAudioType = audio_dec_info->audio_type;
    bEnablePes = audio_dec_info->pes_enable;

    rtsp_url = "/usb/sda1/b.mp3";
    printf("MApi_Rtsp_Open audio: %s\n", rtsp_url);
#if 0
    int i = strlen(rtsp_url);
    if (i < 256)
    {
        char fname[256];
        strcpy(fname, rtsp_url);
        fname[i-6] = '2';
        fname[i-5] = '.';
    #if (AU_PES_FILE)
        fname[i-4] = 'p';
        fname[i-3] = 'e';
        fname[i-2] = 's';
    #else
        if (eAudioType == rtsp_mp4a)
        {
            fname[i-4] = 'a';
            fname[i-3] = 'a';
            fname[i-2] = 'c';
        }
        else
        {
            fname[i-4] = 'm';
            fname[i-3] = 'p';
            fname[i-2] = '3';
        }
    #endif
        fname[i-1] = 0;

        printf("Audio_Open = %s\n", fname);
        if ((audiofile = fopen(fname, "rb")) == NULL)
        {
            printf("Audio_Open error!\n");
        }
    }
#endif
    printf("Audio_Open = %s\n", rtsp_url);
    if ((audiofile = fopen(rtsp_url, "rb")) == NULL)
    {
        printf("Audio_Open error!\n");
    }
    return file;
}

static int MM_Simulate_Rtsp_Close(void * rhandle)
{
    printf("MApi_Rtsp_Close[0x%x]\n", (int)rhandle);
    if(pDataAddr != NULL)
    {
        MsOS_FreeMemory(pDataAddr,gs32NonCachedPoolID);
        pDataAddr = NULL;
    }

    if (audiofile != NULL)
    {
        fclose(audiofile);
        audiofile = NULL;
    }
    if(rhandle != NULL)
    {
        fclose((FILE *)rhandle);
        rhandle = NULL;
    }
    return 0;

}



static int MM_Simulate_Rtsp_Play(void * rhandle, unsigned int start_pos, rtsp_play_mode mode)
{
    U32 u32DataAddr, u32DateSize;

    pDataAddr = (char *)MsOS_AllocateMemory((MALLOC_SIZE),gs32NonCachedPoolID);

    printf("MApi_Rtsp_Play[0x%x, %d, %d]\n", (int)rhandle, start_pos, mode);
    u32DataAddr = (U32)pDataAddr;//MApi_MPlayer_GetOption(E_MPLAYER_MEDIA_MOVIE, MPLAYER_OPTION_GET_BUFFER_INFO, MPLAYER_BUFFER_RTSP_AUDIO_ADDR);
    u32DateSize = MALLOC_SIZE; //MApi_MPlayer_GetOption(E_MPLAYER_MEDIA_MOVIE, MPLAYER_OPTION_GET_BUFFER_INFO, MPLAYER_BUFFER_RTSP_AUDIO_LEN);

    u32VEsDataAddr = u32DataAddr;
    u32VEsDataLen = u32DateSize - RTSP_AES_LENGTH - RTSP_PES_LENGTH - RTSP_VES_RESERVE;
    u32AEsDataAddr = u32VEsDataAddr + u32VEsDataLen + RTSP_VES_RESERVE;
    u32AEsDataLen = RTSP_AES_LENGTH - RTSP_AES_RESERVE;
    printf("VES[0x%x, 0x%x]\n", (int)u32VEsDataAddr, (int)u32VEsDataLen);
    printf("AES[0x%x, 0x%x]\n", (int)u32AEsDataAddr, (int)u32AEsDataLen);

    u32PEsDataAddr = u32AEsDataAddr + u32AEsDataLen + RTSP_AES_RESERVE;
    u32PEsDataLen = RTSP_PES_LENGTH;
    printf("PES[0x%x, 0x%x]\n", (int)u32PEsDataAddr, (int)u32PEsDataLen);

    //find first video packet
#if (VD_PES_FILE)
    u8VEsPoolRing = 0;
    u32VEsDataIndex = 0;
    u32VEsDataSize = u32VEsDataLen;
    fread((U8 *)u32VEsDataAddr, 1, u32VEsDataLen, (FILE *)rhandle);
#else
    U8 *u8Ptr;
    U32 u32VEsPos;
    U8 u8ZeroCount;

    fread((U8 *)u32VEsDataAddr, 1, u32VEsDataLen, (FILE *)rhandle);
    u8VEsPoolRing = 0;
    u32VEsDataIndex = 0;
    u32VEsDataSize = u32VEsDataLen;
    u32VEsPos = 0;
    u8ZeroCount = 0;
    u8Ptr = (U8 *)u32VEsDataAddr;
    printf("run here 1 \n");
    while (u32VEsPos < u32VEsDataLen)
    {
        if (*u8Ptr == 0x00)
        {
            u8ZeroCount++;
        }
        else if (u8ZeroCount >= 2 && *u8Ptr == 0x01)
        {
            u32VEsDataIndex = u32VEsPos - 2;
            break;
        }
        else
        {
            u8ZeroCount = 0;
        }
        u8Ptr++;
        u32VEsPos++;
    }
#endif
    printf("run here 2 \n");
    //find first audio packet
    if ((audiofile != NULL)
#if (AU_PES_FILE == 0)
         && (eAudioType == rtsp_mp4a)
 #endif
        )
    {
        printf("run here 3\n");
        fread((U8 *)u32AEsDataAddr, 1, u32AEsDataLen, audiofile);
        u8AEsPoolRing = 0;
        u32AEsDataIndex = 0;
        u32AEsDataSize = u32AEsDataLen;
        u32AdtsID = *((U32 *)u32AEsDataAddr);
    #if (ADTS_BYTES == 3)
        u32AdtsID = ((u32AdtsID&0xFF)<<16) | (((u32AdtsID>>8)&0xFF)<<8) | ((u32AdtsID>>16)&0xFF);
    #else
        u32AdtsID = ((u32AdtsID&0xFF)<<24) | (((u32AdtsID>>8)&0xFF)<<16) | (((u32AdtsID>>16)&0xFF)<<8) | ((u32AdtsID>>24)&0xFF);
    #endif
    }

#if (VD_PES_FILE)
    return 0;
#else
    printf("VES[0x%x][0x%x], AES[0x%08x]\n", (int)u32VEsPos, (int)u32VEsDataIndex, (int)u32AdtsID);
    if (u32VEsPos < u32VEsDataLen)
        return 0;
    else
        return -1;
#endif
}

static int MM_Simulate_Rtsp_Pause(void * rhandle)
{
    printf("MApi_Rtsp_Pause[0x%x]\n", (int)rhandle);
    return 0;
}

static int MM_Simulate_Rtsp_Resume(void * rhandle)
{
    printf("MApi_Rtsp_Resume[0x%x]\n", (int)rhandle);
    return 0;
}


static int _MApi_Rtsp_Get_VidPkt(void * rhandle, U32 *u32Offset, U32 *u32Size)
{
#if (VD_PES_FILE)
    U8 *u8Ptr;
    U32 u32VEsID0;
    U32 u32VEsID1;
    U32 u32Pos = 0;
    S32 s32TmpSize;
    static S32 s32ReadSize = 0;

    if ((u8VEsPoolRing) && (u32VEsDataIndex >= u32VEsDataLen))
    {
        u8VEsPoolRing = 0;
        memcpy((U8 *)u32VEsDataAddr, (U8 *)(u32VEsDataAddr+u32VEsDataLen), s32ReadSize);
        if ((s32TmpSize = fread((U8 *)(u32VEsDataAddr+s32ReadSize), 1, u32VEsDataLen-s32ReadSize, (FILE *)rhandle)) < 0)
            return FALSE;
        else
            u32VEsDataSize = s32ReadSize + s32TmpSize;

        if (u32VEsDataSize > u32VEsDataLen)
            return FALSE;
    }

    u32VEsID0 = 0;
    u32VEsID1 = 0;
    u32VEsDataIndex %= u32VEsDataLen;
    u32Pos = u32VEsDataIndex + VD_PES_BYTES;
    u8Ptr = (U8 *)(u32VEsDataAddr + u32Pos);
    while (u32Pos < (u32VEsDataSize+u8VEsPoolRing*s32ReadSize))
    {
        U32 U32Tmp = u32VEsID0 >> 24;
        u32VEsID1 <<= 8;
        u32VEsID1 |= U32Tmp;
        u32VEsID1 &= VD_PES_MASK1;
        u32VEsID0 <<= 8;
        u32VEsID0 |= *u8Ptr;
        u32VEsID1 &= VD_PES_MASK0;

        if ((u32VEsID0 == VD_PES_HEAD0) && (u32VEsID1 == VD_PES_HEAD1))
        {
            *u32Offset = u32VEsDataIndex;
            *u32Size = u32Pos - u32VEsDataIndex - (VD_PES_BYTES - 1);
            u32VEsDataIndex = u32Pos - (VD_PES_BYTES - 1);

            //printf("VES0:[0x%x][0x%x]\n", *u32Offset, *u32Size);
            return TRUE;
        }

        u8Ptr++;
        u32Pos++;
    }

    if (u8VEsPoolRing == 1)
        return FALSE;

    if (u32VEsDataSize < u32VEsDataLen)
    {
        if (u32VEsDataIndex < u32VEsDataSize)
        {
            *u32Offset = u32VEsDataIndex;
            *u32Size = u32VEsDataSize - u32VEsDataIndex;
            u32VEsDataIndex = u32VEsDataSize;
            printf("VES2:Reach end[0x%x][0x%x]\n", *u32Offset, *u32Size);
            return TRUE;
        }
        else
        {
            printf("VES3:Reach end[0x%x]!\n", (int)u32VEsDataIndex);
            return FALSE;
        }
    }

    u8VEsPoolRing = 1;
    if ((s32ReadSize = fread((U8 *)(u32VEsDataAddr+u32VEsDataLen), 1, RTSP_VES_RESERVE, (FILE *)rhandle)) < 0)
        return FALSE;
    while (u32Pos < (u32VEsDataLen+s32ReadSize))
    {
        U32 U32Tmp = u32VEsID0 >> 24;
        u32VEsID1 <<= 8;
        u32VEsID1 |= U32Tmp;
        u32VEsID1 &= VD_PES_MASK1;
        u32VEsID0 <<= 8;
        u32VEsID0 |= *u8Ptr;
        u32VEsID1 &= VD_PES_MASK0;

        if ((u32VEsID0 == VD_PES_HEAD0) && (u32VEsID1 == VD_PES_HEAD1))
        {
            *u32Offset = u32VEsDataIndex;
            *u32Size = u32Pos - u32VEsDataIndex - (VD_PES_BYTES - 1);
            u32VEsDataIndex = u32Pos - (VD_PES_BYTES - 1);

            //printf("VES1:[0x%x][0x%x]\n", *u32Offset, *u32Size);
            return TRUE;
        }

        u8Ptr++;
        u32Pos++;
    }

    if (s32ReadSize == 0)
    {
        *u32Offset = u32VEsDataIndex;
        *u32Size = u32VEsDataLen - u32VEsDataIndex;
        u32VEsDataSize = u32VEsDataLen - 1;
        u32VEsDataIndex = u32VEsDataSize;
        u8VEsPoolRing = 0;
        printf("VES4:Reach end[0x%x][0x%x]\n", *u32Offset, *u32Size);
        return TRUE;
    }
    return FALSE;
#else
    U8 *u8Ptr;
    U8 u8ZeroCount = 0;
    U32 u32Pos = 0;
    S32 s32TmpSize;
    static S32 s32ReadSize = 0;

    if ((u8VEsPoolRing) && (u32VEsDataIndex >= u32VEsDataLen))
    {

        u8VEsPoolRing = 0;
        memcpy((U8 *)u32VEsDataAddr, (U8 *)(u32VEsDataAddr+u32VEsDataLen), s32ReadSize);
        if ((s32TmpSize = fread((U8 *)(u32VEsDataAddr+s32ReadSize), 1, u32VEsDataLen-s32ReadSize, (FILE *)rhandle)) < 0)
        {
            printf("return 1\n");
            return FALSE;
        }
        else
            u32VEsDataSize = s32ReadSize + s32TmpSize;

        if (u32VEsDataSize > u32VEsDataLen)
        {
            printf("return 2\n");
            return FALSE;
        }
    }

    u32VEsDataIndex %= u32VEsDataLen;
    u32Pos = u32VEsDataIndex + 3;
    u8Ptr = (U8 *)(u32VEsDataAddr + u32Pos);
    while (u32Pos < (u32VEsDataSize+u8VEsPoolRing*s32ReadSize))
    {
        if (*u8Ptr == 0x00)
        {
            u8ZeroCount++;
        }
        else if (u8ZeroCount >= 2 && *u8Ptr == 0x01)
        {
            *u32Offset = u32VEsDataIndex;
            *u32Size = u32Pos - u32VEsDataIndex - u8ZeroCount;
			if(u8ZeroCount == 2)
				printf(".........u32Size u8ZeroCount is %d, %d\n",(int)*u32Size, (int)u8ZeroCount);
            u32VEsDataIndex = u32Pos - 2;

            //printf("VES0:[0x%x][0x%x]\n", *u32Offset, *u32Size);
            return TRUE;
        }
        else
        {
            u8ZeroCount = 0;
        }

        u8Ptr++;
        u32Pos++;
    }

    if (u8VEsPoolRing == 1)
        return FALSE;

    if (u32VEsDataSize < u32VEsDataLen)
    {
        if (u32VEsDataIndex < u32VEsDataSize)
        {
            *u32Offset = u32VEsDataIndex;
            *u32Size = u32VEsDataSize - u32VEsDataIndex;
            u32VEsDataIndex = u32VEsDataSize;
            printf("VES2:Reach end[0x%x][0x%x]\n", (int)*u32Offset, (int)*u32Size);
            return TRUE;
        }
        else
        {
            printf("VES3:Reach end[0x%x]!\n", (int)u32VEsDataIndex);
            return FALSE;
        }
    }

    u8VEsPoolRing = 1;
    if ((s32ReadSize = fread((U8 *)(u32VEsDataAddr+u32VEsDataLen), 1, RTSP_VES_RESERVE, (FILE *)rhandle)) < 0)
            return FALSE;
    while (u32Pos < (u32VEsDataLen+s32ReadSize))
    {
        if (*u8Ptr == 0x00)
        {
            u8ZeroCount++;
        }
        else if (u8ZeroCount >= 2 && *u8Ptr == 0x01)
        {
            *u32Offset = u32VEsDataIndex;
            *u32Size = u32Pos - u32VEsDataIndex - u8ZeroCount;
			if(u8ZeroCount == 2)
				printf(".........u32Size u8ZeroCount is %d, %d\n",(int)*u32Size, (int)u8ZeroCount);
            u32VEsDataIndex = u32Pos - 2;

            printf("VES1:[0x%x][0x%x]\n", (int)*u32Offset, (int)*u32Size);
            return TRUE;
        }
        else
        {
            u8ZeroCount = 0;
        }

        u8Ptr++;
        u32Pos++;
    }

    if (s32ReadSize == 0)
    {
        *u32Offset = u32VEsDataIndex;
        *u32Size = u32VEsDataLen - u32VEsDataIndex;
        u32VEsDataSize = u32VEsDataLen - 1;
        u32VEsDataIndex = u32VEsDataSize;
        u8VEsPoolRing = 0;
        printf("VES4:Reach end[0x%x][0x%x]\n", (int)*u32Offset, (int)*u32Size);
        return TRUE;
    }

    return FALSE;
#endif
}

static int _MApi_Rtsp_Get_AudPkt(void * rhandle, U32 *u32Offset, U32 *u32Size)
{
    U8 *u8Ptr;
    U32 u32AEsID, u32Pos = 0;
    S32 s32TmpSize;
    static S32 s32ReadSize = 0;

    if ((u8AEsPoolRing) && (u32AEsDataIndex >= u32AEsDataLen))
    {
        u8AEsPoolRing = 0;
        memcpy((U8 *)u32AEsDataAddr, (U8 *)(u32AEsDataAddr+u32AEsDataLen), s32ReadSize);
        if ((s32TmpSize = fread((U8 *)(u32AEsDataAddr+s32ReadSize), 1, u32AEsDataLen-s32ReadSize, (FILE *)rhandle)) < 0)
            return FALSE;
        else
            u32AEsDataSize = s32ReadSize + s32TmpSize;

        if (u32AEsDataSize > u32AEsDataLen)
            return FALSE;
    }

    u32AEsID = 0;
    u32AEsDataIndex %= u32AEsDataLen;
    u32Pos = u32AEsDataIndex + ADTS_BYTES;
    u8Ptr = (U8 *)(u32AEsDataAddr + u32Pos);
    while (u32Pos < (u32AEsDataSize+u8AEsPoolRing*s32ReadSize))
    {
        u32AEsID <<= 8;
        u32AEsID |= *u8Ptr;
        u32AEsID &= ADTS_MASK;

        if (u32AEsID == u32AdtsID)
        {
            *u32Offset = u32AEsDataIndex;
            *u32Size = u32Pos - u32AEsDataIndex - (ADTS_BYTES - 1);
            u32AEsDataIndex = u32Pos - (ADTS_BYTES - 1);

            //printf("AES0:[0x%x][0x%x]\n", *u32Offset, *u32Size);
            return TRUE;
        }

        u8Ptr++;
        u32Pos++;
    }

    if (u8AEsPoolRing == 1)
        return FALSE;

    if (u32AEsDataSize < u32AEsDataLen)
    {
        if (u32AEsDataIndex < u32AEsDataSize)
        {
            *u32Offset = u32AEsDataIndex;
            *u32Size = u32AEsDataSize - u32AEsDataIndex;
            u32AEsDataIndex = u32AEsDataSize;
            printf("AES2:Reach end[0x%x][0x%x]\n", (int)*u32Offset, (int)*u32Size);
            return TRUE;
        }
        else
        {
            printf("AES3:Reach end[0x%x]!\n", (int)u32AEsDataIndex);
            return FALSE;
        }
    }

    u8AEsPoolRing = 1;
    if ((s32ReadSize = fread((U8 *)(u32AEsDataAddr+u32AEsDataLen), 1, RTSP_AES_RESERVE, (FILE *)rhandle)) < 0)
        return FALSE;
    while (u32Pos < (u32AEsDataLen+s32ReadSize))
    {
        u32AEsID <<= 8;
        u32AEsID |= *u8Ptr;
        u32AEsID &= ADTS_MASK;

        if (u32AEsID == u32AdtsID)
        {
            *u32Offset = u32AEsDataIndex;
            *u32Size = u32Pos - u32AEsDataIndex - (ADTS_BYTES - 1);
            u32AEsDataIndex = u32Pos - (ADTS_BYTES - 1);

            //printf("AES1:[0x%x][0x%x]\n", *u32Offset, *u32Size);
            return TRUE;
        }

        u8Ptr++;
        u32Pos++;
    }

    if (s32ReadSize == 0)
    {
        *u32Offset = u32AEsDataIndex;
        *u32Size = u32AEsDataLen - u32AEsDataIndex;
        u32AEsDataSize = u32AEsDataLen - 1;
        u32AEsDataIndex = u32AEsDataSize;
        u8AEsPoolRing = 0;
        printf("AES4:Reach end[0x%x][0x%x]\n", (int)*u32Offset, (int)*u32Size);
        return TRUE;
    }
    return FALSE;
}



static int MM_Simulate_Rtsp_Get_Payload(void * rhandle, rtsp_payload_type type, rtsp_block* block)
{
    U32 u32Offset = 0, u32Size = 0;

    if (type == video_stream)
    {
        if (_MApi_Rtsp_Get_VidPkt(rhandle, &u32Offset, &u32Size))
        {
        #if (VD_PES_FILE)
            U8 *u8Ptr;
            U64 u64Pts;

            u8Ptr = (U8 *)(u32Offset + u32VEsDataAddr);
            if (*u8Ptr != 0x50)
            {
                block->payload_buffer = (S8 * )(u32Offset + u32VEsDataAddr);
                block->payload_len = u32Size;
                block->pts_timestamp = 0xFFFFFFFF;
                block->dts_timestamp = 0xFFFFFFFF;
                return 0;
            }
            else if (u32Size >= 12)
            {
                u8Ptr = (U8 *)(u32Offset + u32VEsDataAddr+7);
                u64Pts = (*u8Ptr >> 1) & 0x07;
                u8Ptr++;
                u64Pts <<= 8;
                u64Pts |= (*u8Ptr) & 0xFF;
                u8Ptr++;
                u64Pts <<= 7;
                u64Pts |= (*u8Ptr >> 1) & 0x7F;
                u8Ptr++;
                u64Pts <<= 8;
                u64Pts |= (*u8Ptr) & 0xFF;
                u8Ptr++;
                u64Pts <<= 7;
                u64Pts |= (*u8Ptr >> 1) & 0x7F;
                block->payload_buffer = (S8 * )(u32Offset + u32VEsDataAddr+12);
                block->payload_len = u32Size-12;
                block->pts_timestamp = u64Pts;
				ZMPLAYER_DBG("In MApi_Rtsp_Get_Payload, pts_timestamp is %x, %x\n", (int)(u64Pts>>32), (int)(u64Pts));
                block->dts_timestamp = 0xFFFFFFFF;
                return 0;
            }
        #else
            block->payload_buffer = (char * )(u32Offset + u32VEsDataAddr);
            block->payload_len = u32Size;
            block->pts_timestamp = 0xFFFFFFFF;
            block->dts_timestamp = 0xFFFFFFFF;
            return 0;
        #endif
        }
        printf("_MApi_Rtsp_Get_VidPkt_1\n");
        return -1;
    }
    else if (type == audio_stream)
    {
        if (audiofile != NULL)
        {
        #if (AU_PES_FILE == 0)
            if (eAudioType == rtsp_mp4a)
        #endif
            {
                if (_MApi_Rtsp_Get_AudPkt((void*)audiofile, &u32Offset, &u32Size))
                {
                    block->payload_buffer = (char * )(u32Offset + u32AEsDataAddr);
                    block->payload_len = u32Size;
                    block->pts_timestamp = 0xFFFFFFFF;
                    block->dts_timestamp = 0xFFFFFFFF;
                }
                else
                {
                    printf("_MApi_Rtsp_Get_AudPkt_1\n");
                    return -1;
                }
            }
        #if (AU_PES_FILE == 0)
            else
            {
                u32Size = fread((U8 *)u32AEsDataAddr, 1, 4096, audiofile);
                block->payload_buffer = (char * )u32AEsDataAddr;
                block->payload_len = u32Size;
                block->pts_timestamp = 0xFFFFFFFF;
                block->dts_timestamp = 0xFFFFFFFF;
            }

            if ((block->payload_len > 0) && bEnablePes)
            {
                u32Size = block->payload_len + sizeof(PES_HEAD) - 6;
                PES_HEAD[4] = (u32Size >> 8) & 0xFF;
                PES_HEAD[5] = u32Size & 0xFF;
                if (sizeof(PES_HEAD) > 9)
                {
                    PES_HEAD[9] = 0x21;
                    PES_HEAD[10] = 0x00;
                    PES_HEAD[11] = 0x01;
                    PES_HEAD[12] = 0x00;
                    PES_HEAD[13] = 0x01;
                }
                memcpy((U8 *)u32PEsDataAddr, PES_HEAD, sizeof(PES_HEAD));
                memcpy((U8 *)(u32PEsDataAddr+sizeof(PES_HEAD)), block->payload_buffer, block->payload_len);
                block->payload_buffer = (char * )u32PEsDataAddr;
                block->payload_len += sizeof(PES_HEAD);
            }
        #endif
        }
        else
        {
            block->payload_buffer = NULL;
            block->payload_len = 0;
            block->pts_timestamp = 0xFFFFFFFF;
            block->dts_timestamp = 0xFFFFFFFF;
        }
        return 0;
    }
    return -1;
}


#ifdef ENABLE_RTSP_PACKAGE

static void * MApi_Rtsp_Open(const char * rtsp_url, video_decoder_init_info* video_dec_info_es, audio_decoder_init_info* audio_dec_info_es)

{
    //FILE * file;
	//const char *rtsp_url = "/usb/sda1/rtsp/out_wlkh_track1.h264";
	//const char *rtsp_url = "/usb/sda1/ch_pplive.h264";//"/usb/sda/ch_pplive.h264";

     video_decoder_init_info* video_dec_info = video_dec_info_es;
     audio_decoder_init_info* audio_dec_info = audio_dec_info_es;

#if (ES_RTSP)
	const char* result = NULL;

    result = RTSP_open(rtsp_url,video_dec_info,audio_dec_info);

    if (result != NULL)
    {
        //printf("open rtsp url=%s success!\n",rtsp_url);
    }
    else
    {
        //printf("open rtsp url=%s fail!\n ",rtsp_url);
    }

    video_dec_info->video_type = rtsp_h264;
	video_dec_info->avsync_enable = true;
    video_dec_info->drop_error_frame = false;
    //audio_dec_info->audio_type = rtsp_audio_mp4a;

    audio_dec_info->audio_type = rtsp_mp4a;
    audio_dec_info->avsync_enable = false;
    audio_dec_info->errcnt_enable = true;
    audio_dec_info->pes_enable = true;//george
    audio_dec_info->pes_head_length = 14;

    return (void*)result;

#else

	const char* result = NULL;
    if (result != NULL)
    {
        //printf("open rtsp url=%s success!\n",rtsp_url);
    }
    else
    {
        //printf("open rtsp url=%s fail!\n ",rtsp_url);
    }

    video_dec_info->video_type = rtsp_h264;
	video_dec_info->avsync_enable = true;
    video_dec_info->drop_error_frame = false;
    //audio_dec_info->audio_type = rtsp_audio_mp4a;

    audio_dec_info->audio_type = rtsp_mp4a;
    audio_dec_info->avsync_enable = (AU_PES_FILE && VD_PES_FILE);
    audio_dec_info->errcnt_enable = true;
    audio_dec_info->pes_enable = true;//george
    audio_dec_info->pes_head_length = 14;

    eVideoType = video_dec_info->video_type;
    eAudioType = audio_dec_info->audio_type;
    bEnablePes = audio_dec_info->pes_enable;


    int i = strlen(rtsp_url);
    if (i < 256)
    {
        char fname[256];
        strcpy(fname, rtsp_url);
        fname[i-6] = '2';
        fname[i-5] = '.';
    #if (AU_PES_FILE)
        fname[i-4] = 'p';
        fname[i-3] = 'e';
        fname[i-2] = 's';
    #else
        if (eAudioType == rtsp_mp4a)
        {
            fname[i-4] = 'a';
            fname[i-3] = 'a';
            fname[i-2] = 'c';
        }
        else
        {
            fname[i-4] = 'm';
            fname[i-3] = 'p';
            fname[i-2] = '3';
        }
    #endif
        fname[i-1] = 0;

        //printf("Audio_Open = %s\n", fname);
        if ((audiofile = fopen(fname, "rb")) == NULL)
        {
            //printf("Audio_Open error!\n");
        }
    }
   // printf("\n[%d] %s\n",__LINE__, __FILE__);

    return NULL;
#endif
}

static int MApi_Rtsp_Close(void * rhandle)
{
    //receive data from server
#if (ES_RTSP)
    RTSP_close((const char*)rhandle);
    MsOS_FreeMemory(pDataAddr,gs32NonCachedPoolID);

#endif
    //printf("MApi_Rtsp_Close_end\n");
    return 0;
    //receive data from server

    //receive data from file
    /*printf("MApi_Rtsp_Close[0x%x]\n", (unsigned int)rhandle);
    if (audiofile != NULL)
        fclose(audiofile);
    return fclose((FILE *)rhandle);*/
}

static int MApi_Rtsp_Play(void * rhandle, unsigned int start_pos,rtsp_play_mode mode_es)
{
    rtsp_play_mode mode = mode_es;
    MP_U32 u32DataAddr, u32DateSize;
    pDataAddr = (char *)MsOS_AllocateMemory(MALLOC_SIZE,gs32NonCachedPoolID);
    //printf("MApi_Rtsp_Play[0x%x, %d, %d]\n", (MP_U32)rhandle, start_pos, mode);
    u32DataAddr = (MP_U32 )pDataAddr;//MApi_MPlayer_GetOption(E_MPLAYER_MEDIA_MOVIE, MPLAYER_OPTION_GET_BUFFER_INFO, MPLAYER_BUFFER_RTSP_AUDIO_ADDR);
    u32DateSize = MALLOC_SIZE;//MApi_MPlayer_GetOption(E_MPLAYER_MEDIA_MOVIE, MPLAYER_OPTION_GET_BUFFER_INFO, MPLAYER_BUFFER_RTSP_AUDIO_LEN);
    //printf("u32DataAddr is %lx\n",u32DataAddr);
    u32VEsDataAddr = u32DataAddr;

    u32VEsDataLen = u32DateSize - RTSP_AES_LENGTH - RTSP_PES_LENGTH - RTSP_VES_RESERVE;
    u32AEsDataAddr = u32VEsDataAddr + u32VEsDataLen + RTSP_VES_RESERVE;
    u32AEsDataLen = RTSP_AES_LENGTH - RTSP_AES_RESERVE;



    //printf("VES[0x%x, 0x%x]\n", (unsigned int)u32VEsDataAddr, (unsigned int)u32VEsDataLen);
    //printf("AES[0x%x, 0x%x]\n", (unsigned int)u32AEsDataAddr, (unsigned int)u32AEsDataLen);

    ////////////////////////////////////////////////

    //m_u32MsgDump = 0;
    VEsCounter = 0;
    AEsCounter = 0;
    u32PtsCnt0 = 0, u32PrevPts0 = 0;
    u32PtsCnt1 = 0, u32PrevPts1 = 0;

    int rt = 0;
    rt = RTSP_play(u32VEsDataAddr,u32AEsDataAddr,(const char*)rhandle,0.0f,mode);
    if (rt < 0)
    {
        //printf("start play failed!\n");
    }
    //printf("start play done!\n");
    return rt;

}

static int MApi_Rtsp_Pause(void * rhandle)
{
    printf("MApi_Rtsp_Pause[0x%x]\n", (unsigned int)rhandle);
    return 0;
}

static int MApi_Rtsp_Resume(void * rhandle)
{
    printf("MApi_Rtsp_Resume[0x%x]\n", (unsigned int)rhandle);
    return 0;
}
static int MApi_Rtsp_Get_Payload(void* rhandle, rtsp_payload_type type_es, rtsp_block* block_es)
{
    //printf("RTSP_GET_PAYLOAD\n");
    rtsp_payload_type type = type_es;
    rtsp_block* block = block_es;

    if (type == video_stream)
    {
        //printf(" VES address is %lx \n",u32VEsDataAddr);
        block->payload_buffer = (char*)u32VEsDataAddr;
        if(block->payload_buffer == NULL)
        {
            //printf("block->payload_buffer is NULL\n");
        }
        block->payload_len = u32VEsDataLen;
       // printf("video payload buffer is %lx,payload_len is %d\b",(long int)block->payload_buffer,block->payload_len);
        if (RTSP_get_payload((const char*)rhandle, type, block) < 0)
        {
            //printf("rtsp get video payload failed!\n");
            return -1;
        }

        //block->payload_buffer=(char *)u32VEsDataAddr;
        //if ((block->payload_buffer != (char *)u32VEsDataAddr) || (block->payload_len > (int)u32VEsDataLen))
        if(block->payload_len > (int)u32VEsDataLen)
        {
            //printf("video payload overflow[%x][%x]!\n", (MP_U32)(block->payload_buffer), block->payload_len);
            return -1;
        }

        //block->pts_timestamp &= U33_MAX;
        //block->dts_timestamp &= U33_MAX;
        block->pts_timestamp = 0xFFFFFFFF;
        block->dts_timestamp = 0xFFFFFFFF;
        if (block->payload_len > 0)
        {
            VEsCounter++;

            if ((MP_U32)(block->pts_timestamp) < u32PrevPts0)
            {
                u32PtsCnt0++;
            }
            u32PrevPts0 = (MP_U32)(block->pts_timestamp);
            //printf("\n [%s] [%s] [%d] !\n",__FILE__,__FUNCTION__,__LINE__);
            //printf("video[%08d][%08d][%08x][%08x][%08x] [%08x][%08x]\n", VEsCounter, u32PtsCnt0, block->payload_len, (MP_U32)(block->pts_timestamp>>32), (MP_U32)(block->pts_timestamp), *((MP_U32*)(u32VEsDataAddr)), *((MP_U32*)(u32VEsDataAddr+4)));

        }

        return 0;
    }
    else if (type == audio_stream)
    {
        block->payload_buffer = (char *)u32AEsDataAddr;
        block->payload_len = u32AEsDataLen;
        //printf("1:first audio payload buffer is %lx,payload_len is %d\b",(unsigned long)block->payload_buffer,block->payload_len);
        if (RTSP_get_payload((const char*)rhandle, type, block) < 0)
        {
            //printf("rtsp get audio  payload failed!\n");
            return -1;
        }
        if ((block->payload_buffer != (char *)u32AEsDataAddr) || (block->payload_len > (int)u32AEsDataLen))
        {
            //printf("audio payload overflow[%x][%x]!\n", (MP_U32)(block->payload_buffer), block->payload_len);
            return -1;
        }

        //block->pts_timestamp &= U33_MAX;
        //block->dts_timestamp &= U33_MAX;
        block->pts_timestamp = 0xFFFFFFFF;
        block->dts_timestamp = 0xFFFFFFFF;
        if (block->payload_len > 0)
        {
            AEsCounter++;

            if ((MP_U32)(block->pts_timestamp) < u32PrevPts1)
            {
                u32PtsCnt1++;
            }
            u32PrevPts1 = (MP_U32)(block->pts_timestamp);

        }

        return 0;
    }
    else
    {
        //printf("MApi_Rtsp_Get_Payload_1\n");
        return -1;
    }
}
#endif

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief The sample code to initialize multi-media function
/// @param none
/// @return TRUE: Initial success.
/// @return FALSE: Initial fail.
/// @sa
/// @note
/// Command: \b MM_INIT \n
//------------------------------------------------------------------------------

MS_BOOL appDemo_MM_Init(void)
{
    MS_BOOL B_Ret = TRUE;
    MM_MEM_INFO MM_MemInfo;
    AUDIO_INFO MM_AudioInfo;
    MM_TSP_INFO MM_TSPInfo;
    void * ptmpMem = NULL;

    MS_U8 u8Volume = DEFAULT_VOLUME;
    MS_BOOL bMute = FALSE;

    if(b_MM_Initied)
        return FALSE;

    memset(_pu8FileName, 0x00, sizeof(MS_U8)*FILE_NAME_LENGTH);




    MM_MemInfo.u32Miu0_mem_size = MIU_DRAM_LEN0;
#if (ENABLE_MIU_1)
    MM_MemInfo.u32Miu1_mem_size = MIU1_LEN;
#else
    MM_MemInfo.u32Miu1_mem_size = ENABLE_MIU_1;
#endif

#if (ENABLE_MIU_1)
    MM_MemInfo.u32Miu_boundary = MIU_INTERVAL;
#else
    MM_MemInfo.u32Miu_boundary = MIU_DRAM_LEN;
#endif

    MM_MemInfo.u32Mmap_items = 60;
    MM_MemInfo.u32Total_mem_size = MM_MemInfo.u32Miu1_mem_size + MM_MemInfo.u32Miu0_mem_size;
    ptmpMem = MsOS_AllocateMemory(sizeof(MM_MEMMAP_INFO)* MM_MemInfo.u32Mmap_items,gs32NonCachedPoolID);
    if(!ptmpMem)
    {
        return FALSE;
    }
    memset(ptmpMem, 0x00, sizeof(MM_MEMMAP_INFO)*MM_MemInfo.u32Mmap_items);

    _appDemo_MM_MemInit(&MM_MemInfo, (MM_MEMMAP_INFO *)ptmpMem);

    MM_AudioInfo.u32AudioSrc= AUDIO_SOURCE_DTV;
    MM_AudioInfo.u32AudioPath= AUDIO_PATH_MAIN_SPEAKER;
    MM_AudioInfo.u32AudioOutput = AUDIO_OUTPUT_MAIN_SPEAKER;

    MM_TSPInfo.pAddr = u8TSPFirmwareForRTSP;
    MM_TSPInfo.u32Size = sizeof(u8TSPFirmwareForRTSP);
    mm_print("DDI TspFW %x size %d\n",(unsigned int)u8TSPFirmwareForRTSP,sizeof(u8TSPFirmwareForRTSP));

    if(MApi_DMX_ChkAlive() != DMX_FILTER_STATUS_OK)
    {
        appDemo_TSP_Init();
    }
    DemoAV_TSP_StopDmxFlt();///stop live flt

    MApp_MPlayer_SetMemPool(gs32CachedPoolID, 1);
    MApp_MPlayer_SetMemPool(gs32NonCachedPoolID, 0);
    MApp_MPlayer_SetCustomerInfo(Customer_info,49,Customer_hash,16);
    B_Ret=MApi_MPlayer_Initialize(&MM_AudioInfo,&MM_TSPInfo,(void*)&MM_MemInfo, ptmpMem);
    if(B_Ret)
    {
        MApi_MPlayer_RegisterCallBack(E_MPLAYER_MEDIA_MOVIE, _MApp_Mplayer_Movie_CallBack);
        MApi_MPlayer_RegisterCallBack(E_MPLAYER_MEDIA_PHOTO, _MApp_Mplayer_Photo_CallBack);
        MApi_MPlayer_RegisterCallBack(E_MPLAYER_MEDIA_MUSIC, _MApp_Mplayer_Music_CallBack);
        b_MM_Initied = TRUE;
    }
    else
    {
        appDemo_MM_DeInit();
        b_MM_Initied = FALSE;
    }

    if(MsOS_FreeMemory(ptmpMem,gs32NonCachedPoolID))
    {
        ptmpMem=NULL;
    }

    //audio de-mute and initial volume setting , move from mm mdw
    bMute = FALSE;
    appDemo_Audio_SetMute(&bMute);

    u8Volume = DEFAULT_VOLUME;
    appDemo_Audio_SetAbsoluteVolume(&u8Volume);

    return B_Ret;
}

//------------------------------------------------------------------------------
/// @brief The sample code to De-Initialize multi-media function
/// @param none
/// @return TRUE: De-Initial success.
/// @return FALSE: De-Initial fail.
/// @sa
/// @note
/// Command: \b MM_DEINIT \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_MM_DeInit(void)
{

    MS_BOOL B_Ret = FALSE;
    int flow = DMX_FLOW_PLAYBACK;
#if (DMX_INPUT == DMX_INPUT_DEMOD)
    int inSrc = DMX_FLOW_INPUT_DEMOD;
#elif (DMX_INPUT == DMX_INPUT_EXT_INPUT0)
    int inSrc = DMX_FLOW_INPUT_EXT_INPUT0;
#elif (DMX_INPUT == DMX_INPUT_EXT_INPUT1)
    int inSrc = DMX_FLOW_INPUT_EXT_INPUT1;
#else
    int inSrc = DMX_FLOW_INPUT_DEMOD;
#endif

    int clkInv = 0;
    int extSync = 1;
    int parallel = 1;

    if(b_MM_Initied == FALSE)
    {
        return B_Ret;
    }

    memset(_pu8FileName, 0x00, sizeof(MS_U8)*FILE_NAME_LENGTH);
    MApi_MPlayer_Finalize();
    MApi_MPlayer_RegisterCallBack(E_MPLAYER_MEDIA_MOVIE, NULL);
    MApi_MPlayer_RegisterCallBack(E_MPLAYER_MEDIA_PHOTO, NULL);
    MApi_MPlayer_RegisterCallBack(E_MPLAYER_MEDIA_MUSIC, NULL);

    B_Ret = TRUE;
    b_MM_Initied = FALSE;

    if(eMediaType==E_MPLAYER_MEDIA_MOVIE)
    {
        // user would swith to live after watching ts file
        DemoAV_Tuner_Config(&flow, &inSrc, &clkInv, &extSync, &parallel);
    }


    return B_Ret;
}

//------------------------------------------------------------------------------
/// @brief The sample code to goto specific position of multi-media file
/// @param[in] pGotoTimeMs seek position, unit is millisecond
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b MM_SEEK \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_MM_Seek(MS_U32 *pGotoTimeMs)
{
#if MM_REMOVE_TXT_SUBTITLE == 0
    _MM_TxtSubtitleFinalize();//currently won't demo subtitle after this playback
#endif
    MS_BOOL B_Ret = FALSE;
    MS_U32 u32GotoMs = *pGotoTimeMs;

    if(eMediaType == E_MPLAYER_MEDIA_MOVIE)//for mantis 0242114
    {
        MS_U32 u32Value = 0;
        u32Value = MApi_MPlayer_GetOption(eMediaType, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_TOTAL_TIME);

        if(u32Value > 0 && u32GotoMs > u32Value)
        {
            printf("Goto time exceeds file length (=%d ms)\n",u32Value);
            B_Ret = FALSE;
            return B_Ret;
        }
    }

	if(eMediaType == E_MPLAYER_MEDIA_MUSIC)
	{
		MS_U32 u32Value = 0;
		u32Value = MApi_MPlayer_GetOption(eMediaType, MPLAYER_OPTION_MUSIC_INFO, MPLAYER_INFO_TOTAL_TIME);

		if(u32Value > 0 && u32GotoMs > u32Value * 1000)
		{
			printf("Goto time exceeds file length (=%d s)\n", u32Value);
			B_Ret = FALSE;
			return B_Ret;
		}
	}

    if(MApi_MPlayer_Seek(eMediaType, (int)u32GotoMs) == MPLAYER_TRICK_NORMAL)
        B_Ret = TRUE;

    return B_Ret;
}

//------------------------------------------------------------------------------
/// @brief The sample code to fast-forward multi-media file
/// @param[in] None
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b MM_FF \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_MM_FF(void)
{
#if MM_REMOVE_TXT_SUBTITLE == 0
    _MM_TxtSubtitleFinalize();//currently won't demo subtitle after this playback
#endif
    EN_MPLAYER_TRICK_MODE B_Ret;
    B_Ret = MApi_MPlayer_FF(eMediaType);
	if(B_Ret == MPLAYER_TRICK_FF_2X)
    {
        mm_print("MPLAYER_TRICK_FF_2X\n");
    }
    else if(B_Ret == MPLAYER_TRICK_FF_4X)
    {
        mm_print("MPLAYER_TRICK_FF_4X\n");
    }
    else if(B_Ret == MPLAYER_TRICK_FF_8X)
    {
        mm_print("MPLAYER_TRICK_FF_8X\n");
    }
    else if(B_Ret == MPLAYER_TRICK_FF_16X)
    {
        mm_print("MPLAYER_TRICK_FF_16X\n");
    }
    else if(B_Ret == MPLAYER_TRICK_NORMAL)
    {
        mm_print("MPLAYER_TRICK_NORMAL\n");
    }
    //else
        //return FALSE;
    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief The sample code to back-forward multi-media file
/// @param[in] None
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b MM_RW \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_MM_RW(void)
{
#if MM_REMOVE_TXT_SUBTITLE == 0
    _MM_TxtSubtitleFinalize();//currently won't demo subtitle after this playback
#endif
    EN_MPLAYER_TRICK_MODE B_Ret;
    B_Ret = MApi_MPlayer_RW(eMediaType);
    if(B_Ret == MPLAYER_TRICK_FB_2X)
    {
        mm_print("MPLAYER_TRICK_FB_2X\n");
    }
    else if(B_Ret == MPLAYER_TRICK_FB_4X)
    {
		    mm_print("MPLAYER_TRICK_FB_4X\n");
    }
    else if(B_Ret == MPLAYER_TRICK_FB_8X)
    {
        mm_print("MPLAYER_TRICK_FB_8X\n");
    }
    else if(B_Ret == MPLAYER_TRICK_FB_16X)
    {
        mm_print("MPLAYER_TRICK_FB_16X\n");
    }
    else if(B_Ret == MPLAYER_TRICK_NORMAL)
    {
        mm_print("MPLAYER_TRICK_NORMAL\n");
    }
    //else
        //return FALSE;
    return TRUE;

}

//------------------------------------------------------------------------------
/// @brief The sample code to pause multi-media file
/// @param[in] None
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b MM_PAUSE \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_MM_Pause(void)
{
    if(MApi_MPlayer_Pause(eMediaType) == MPLAYER_TRICK_PAUSE)
        return TRUE;
    else
        return FALSE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to resume multi-media file
/// @param[in] None
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b MM_RESUME \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_MM_Resume(void)
{
    if(MApi_MPlayer_Resume(eMediaType) == MPLAYER_TRICK_NORMAL)
        return TRUE;
    else
        return FALSE;

}

//------------------------------------------------------------------------------
/// @brief The sample code to stop play multi-media file
/// @param[in] None
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b MM_STOP \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_MM_Stop(void)
{
#if MM_REMOVE_TXT_SUBTITLE == 0
    _MM_TxtSubtitleFinalize();
#endif
    if(eMediaType==E_MPLAYER_MEDIA_PHOTO)
    {
        appDemo_MM_ClearPhoto();
    }

    if(MApi_MPlayer_Stop(eMediaType) == TRUE)
    {
        //for mantis 0211689
        msAPI_VE_SetVideoMute(ENABLE);
        msAPI_XC_SetVideoMute(ENABLE, 0);
        return TRUE;
    }
    return FALSE;
}
//------------------------------------------------------------------------------
/// @brief The sample code to repeatly play multi-media file
/// @param[in] pu32On: 1 for repeat on, 0 for repeat off
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b MM_Repeat \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_MM_Repeat(MS_U32 *pu32On)
{
#if MM_REMOVE_TXT_SUBTITLE == 0
    _MM_TxtSubtitleFinalize();//currently won't demo subtitle after this playback
#endif
    if((*pu32On != 0) && (*pu32On != 1))
        return FALSE;

    if(((_u32FileOption == MPLAYER_MODE_INVALID) || (_u32FileOption == MPLAYER_FILE_MODE)))
    {
        if(*pu32On)
            u8IsRepeat = TRUE;
        else
            u8IsRepeat = FALSE;

        return TRUE;
    }
    else
    {
        u8IsRepeat=FALSE;
        return FALSE;
}
}

//------------------------------------------------------------------------------
/// @brief The sample code to back-forward replay multi-media file , need set before mmplay and after mm init
/// @param[in] pu32On: 1 for back-forward replay on, 0 for back-forward replay off
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b MM_RW_Replay \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_MM_RW_Replay(MS_U32 *pu32On)
{
    if((*pu32On != 0) && (*pu32On != 1))
        return FALSE;

    if(((_u32FileOption == MPLAYER_MODE_INVALID) || (_u32FileOption == MPLAYER_FILE_MODE)))
    {
        MApi_MPlayer_SetOption(E_MPLAYER_MEDIA_MOVIE, MPLAYER_OPTION_FB_REPLAY, (MS_BOOL)*pu32On);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//------------------------------------------------------------------------------
/// @brief The sample code to get information of multi-media file
/// @param[in] pOption enumeration of EN_MM_OPTION_TYPE in msAPI_MM.h
/// @param[in] pMMInfo enumeration of EN_MM_INFO in msAPI_MM.h
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
/// Command: \b MM_GETOPTION \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_MM_GetOption(MS_U32 *pOption, MS_U32 *pMMInfo)
{
    MS_U32 u32Value;
    u32Value = MApi_MPlayer_GetOption(eMediaType, (EN_MPLAYER_OPTION_TYPE)*pOption, (unsigned int)*pMMInfo);
    mm_print("GetOption Result = %ld\n",u32Value);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to play multi-media file
/// @param[in] pFileOption enumeration of EN_MM_OPTION_TYPE in msAPI_MM.h
///     0: File Mode
///     1: ES Mode
///     2: Seekable Stream Mode
///     3: Unseekable Stream Mode
/// @param[in] U8FileName File path
/// @sa
/// @note
/// Command: \b MM_Play \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_MM_Play(MS_U32 *pFileOption, MS_U8* U8FileName)
{
    MS_BOOL B_Ret = FALSE;
    MPLAYER_MEDIA sMplayerMeida;
    U32 u32FileNameLen;
    MS_U8 U8File[FILE_NAME_LENGTH];
    MS_U8 *pu8ExtName=NULL;

    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_GAME_IN);
    MApi_AUDIO_InputSwitch(AUDIO_DSP1_DVB_INPUT,AUDIO_PATH_MAIN);

    if(!b_MM_Initied)
        return B_Ret;

	if((pFileOption == NULL) || (U8FileName == NULL)) //MM_PLAY has no option will crash
	{
		printf("please input the FileOption and FileName\n");
		return B_Ret;
	}

    if(strlen((char*)U8FileName) >= FILE_NAME_LENGTH-1)
    {
        mm_print("file length too long\n");
        return B_Ret;
    }
    strcpy((char*)U8File, (char*)U8FileName);
    memset(&sMplayerMeida, 0, sizeof(MPLAYER_MEDIA));
    memset(&_s_sStreamHandle, 0, sizeof(MPLAYER_STREAM_IOCB));

    _u32FileOption = *pFileOption;
    strcpy((char*)_pu8FileName, (char*)U8File);

    if(*pFileOption != MPLAYER_FILE_MODE)
        u8IsRepeat = FALSE; //currently only support MPLAYER_FILE_MODE



    if(MApi_VDEC_EX_IsAlive(stVDECStreamId)==E_VDEC_EX_OK)
    {
        MApi_VDEC_EX_Exit(stVDECStreamId);
    }

#if 1 // def MM_MPEG2SD_PROFILE Temp Remove
    MApi_MPlayer_SetOption(E_MPLAYER_MEDIA_MOVIE, MPLAYER_OPTION_DISABLE_CONTAINER_TYPE,
        //MPLAYER_CONTAINER_TYPE_MP4|MPLAYER_CONTAINER_TYPE_MKV|
        //MPLAYER_CONTAINER_TYPE_FLV|MPLAYER_CONTAINER_TYPE_ASF|
        MPLAYER_CONTAINER_TYPE_RM |MPLAYER_CONTAINER_TYPE_FLV );
        //MPLAYER_CONTAINER_TYPE_DSS|MPLAYER_CONTAINER_TYPE_SWF|
        //MPLAYER_CONTAINER_TYPE_ESDATA
#endif

    if(*pFileOption == MPLAYER_FILE_MODE)
    {
        u32FileNameLen = strlen((char *)U8FileName);
        pu8ExtName = (U8*)U8FileName + u32FileNameLen - 1;

        while(pu8ExtName > U8FileName)
        {
            if(pu8ExtName[0] == '.')
            {
                pu8ExtName++;
                break;
            }
            else if(pu8ExtName[0] == '/')
            {   // no extension name.
                return B_Ret;
            }
            pu8ExtName--;
        }
        if(pu8ExtName > U8FileName)
        {
            U32 i = 0;
            while(pu8ExtName[i])
            {
                pu8ExtName[i] = toupper(pu8ExtName[i]);
                i++;
            }
            if (strcmp("PCM", (char *)pu8ExtName) == 0)
            {
                //=========================PCM========================
                MApi_MPlayer_SetOption(E_MPLAYER_MEDIA_MUSIC, MPLAYER_OPTION_SET_MUSIC_TYPE, MPLAYER_SUBTYPE_PCM);
                MPLAYER_PCM_PARAM* pPCM_Param;
                pPCM_Param = (MPLAYER_PCM_PARAM*)malloc(sizeof(MPLAYER_PCM_PARAM));
                pPCM_Param->eFormatTag = PCM_FORMAT_PCM;
                pPCM_Param->u16Channels = 2;
                pPCM_Param->u16BitsPerSample = 16;
                pPCM_Param->u32SamplesPerSec = 48000;
                pPCM_Param->bBigEndian = TRUE;
                MApi_MPlayer_SetOption(E_MPLAYER_MEDIA_MUSIC, MPLAYER_OPTION_MUSIC_SET_PCM_PARAM, (unsigned int)pPCM_Param);
            }

            for (i = 0; i < sizeof(_zmm_support_ext_Table) / sizeof(Z_File_Ext_Info); i++)
            {
                if (strcmp(&_zmm_support_ext_Table[i].FileExt[0], (char *)pu8ExtName) == 0)
                {
                    eMediaType = _zmm_support_ext_Table[i].FileType;
                    break;
                }
            }
            if(i == sizeof(_zmm_support_ext_Table) / sizeof(Z_File_Ext_Info))
                return B_Ret; //extension name is not support yet
        }

        mm_print("\n[%d] %s :   -> %s %s\n", __LINE__, __FILE__, U8FileName,U8File);

        eZmpMode = MPLAYER_FILE_MODE;
        _s_sStreamHandle.pfnStreamOpen = NULL;//_appDemo_MM_Open;
        _s_sStreamHandle.pfnStreamClose =NULL;// _appDemo_MM_Close;
        _s_sStreamHandle.pfnStreamLength = NULL;//_appDemo_MM_FileLength;
        _s_sStreamHandle.pfnStreamSeek = NULL;//_appDemo_MM_Seek;
        _s_sStreamHandle.pfnStreamRead = NULL;//_appDemo_MM_Read;
        _s_sStreamHandle.pfnRtspOpen = NULL;
        _s_sStreamHandle.pfnRtspClose = NULL;
        _s_sStreamHandle.pfnRtspPlay = NULL;
        _s_sStreamHandle.pfnRtspPause = NULL;
        _s_sStreamHandle.pfnRtspResume = NULL;
        _s_sStreamHandle.pfnRtspGetPayload = NULL;

        //set display window, {0, 0, 0, 0} means full panel
        sMplayerMeida.eFileMode = MPLAYER_FILE_MODE;
        sMplayerMeida.u32DispX = 0;
        sMplayerMeida.u32DispY = 0;
        sMplayerMeida.u32DispW = 1920;
        sMplayerMeida.u32DispH = 1080;
        sMplayerMeida.u32GotoTimeMs = 0;
        sMplayerMeida.filename = (char*)U8File;//utf8fn;
#if 0
        _MM_TxtSubtitleInit();
#endif
        if(eMediaType == E_MPLAYER_MEDIA_PHOTO)
        {
            msAPI_VE_SetSourceType(E_MSAPI_VE_SOURCE_SCALER_OP2);
        }
        else
        {
            msAPI_VE_SetSourceType(E_MSAPI_VE_SOURCE_AUTO);
        }
        msAPI_VE_SetMode();
        B_Ret = MApi_MPlayer_PlayFile(eMediaType, &sMplayerMeida, &_s_sStreamHandle);
    }
    else if((*pFileOption == MPLAYER_SEEKABLE_STREAM_MODE) || (*pFileOption == MPLAYER_UNSEEKABLE_STREAM_MODE))
    {
        eZmpMode = MPLAYER_SEEKABLE_STREAM_MODE;
        eMediaType = E_MPLAYER_MEDIA_MOVIE;
        msAPI_VE_SetSourceType(E_MSAPI_VE_SOURCE_AUTO);
        msAPI_VE_SetMode();
#ifdef ENABLE_RTSP_PACKAGE

        mediainfo.media_length = 0;
        rtsp_set_heartbeatinterval(40);
        _s_sStreamHandle.pfnStreamOpen = MM_SeekAble_Stream_RTSP_Open;
        _s_sStreamHandle.pfnStreamClose = MM_SeekAble_Stream_RTSP_Close;
        _s_sStreamHandle.pfnStreamLength = MM_SeekAble_Stream_RTSP_Length;
        _s_sStreamHandle.pfnStreamSeek = MM_SeekAble_Stream_RTSP_Seek;
        _s_sStreamHandle.pfnStreamRead = MM_SeekAble_Stream_RTSP_Read;
        _s_sStreamHandle.pfnRtspOpen = NULL;
        _s_sStreamHandle.pfnRtspClose = NULL;
        _s_sStreamHandle.pfnRtspPlay = NULL;
        _s_sStreamHandle.pfnRtspPause = NULL;
        _s_sStreamHandle.pfnRtspResume = NULL;
        _s_sStreamHandle.pfnRtspGetPayload = NULL;
#else
        _s_sStreamHandle.pfnStreamOpen = MM_Simulate_Stream_Open_np;
        _s_sStreamHandle.pfnStreamClose = MM_Simulate_Stream_Close_np;
        _s_sStreamHandle.pfnStreamLength = MM_Simulate_Stream_Length_np;
        _s_sStreamHandle.pfnStreamSeek = MM_Simulate_Stream_Seek_np;
        _s_sStreamHandle.pfnStreamRead = MM_Simulate_Stream_Read_np;
        _s_sStreamHandle.pfnRtspOpen = NULL;
        _s_sStreamHandle.pfnRtspClose = NULL;
        _s_sStreamHandle.pfnRtspPlay = NULL;
        _s_sStreamHandle.pfnRtspPause = NULL;
        _s_sStreamHandle.pfnRtspResume = NULL;
        _s_sStreamHandle.pfnRtspGetPayload = NULL;
#endif
        sMplayerMeida.eFileMode=MPLAYER_SEEKABLE_STREAM_MODE;
        sMplayerMeida.sSubtitle.eSubtitleType = MPLAYER_SUBTITLE_INVALID;
        sMplayerMeida.sSubtitle.SubtitleFileName = NULL;
        sMplayerMeida.sSubtitle.SubtitleSUBFileName = NULL;
        sMplayerMeida.filename = (char*)U8File;//utf8fn;
        sMplayerMeida.u32DispX = 0;
        sMplayerMeida.u32DispY = 0;
        sMplayerMeida.u32DispW = 0;
        sMplayerMeida.u32DispH = 0;
        sMplayerMeida.u32GotoTimeMs = 0;
#ifdef ENABLE_RTSP_PACKAGE
        err = (rtsp_error_code)rtsp_open((char *)U8FileName,(void*) (&mediainfo), &g_rhandle);
        if(RTSP_NO_ERROR != err)
        {
            printf("rtsp_open error, errno = %d\n", err);
            err = (rtsp_error_code)rtsp_close(g_rhandle);
            if (RTSP_NO_ERROR != err)
            {
                printf("rtsp_close error, errno = %d\n", err);
            }
        }
        printf("mediainfo.media_length = %f\n", mediainfo.media_length);
        rtsplength = (int)mediainfo.media_length;

        //for rtsp_play
        err = (rtsp_error_code)rtsp_play(g_rhandle, (double)0.0, normal_play);
        if (RTSP_NO_ERROR != err)
        {
            printf("rtsp_play error, errno = %d\n", err);
            err = (rtsp_error_code)rtsp_close(g_rhandle);
            if (RTSP_NO_ERROR != err)
            {
                printf("rtsp_close error, errno = %d\n", err);
            }
        }
#endif
        B_Ret = MApi_MPlayer_PlayFile(eMediaType, &sMplayerMeida, &_s_sStreamHandle);
    }
    else if((*pFileOption == MPLAYER_RTSP_MODE))
    {
        eZmpMode = MPLAYER_RTSP_MODE;
        eMediaType = E_MPLAYER_MEDIA_MOVIE;
        msAPI_VE_SetSourceType(E_MSAPI_VE_SOURCE_AUTO);
        msAPI_VE_SetMode();
#ifdef ENABLE_RTSP_PACKAGE
        _s_sStreamHandle.pfnStreamOpen = NULL;
        _s_sStreamHandle.pfnStreamClose = NULL;
        _s_sStreamHandle.pfnStreamLength = NULL;
        _s_sStreamHandle.pfnStreamSeek = NULL;
        _s_sStreamHandle.pfnStreamRead = NULL;
        _s_sStreamHandle.pfnRtspOpen = MApi_Rtsp_Open;
        _s_sStreamHandle.pfnRtspClose = MApi_Rtsp_Close;
        _s_sStreamHandle.pfnRtspPlay = MApi_Rtsp_Play;
        _s_sStreamHandle.pfnRtspPause = MApi_Rtsp_Pause;
        _s_sStreamHandle.pfnRtspResume = MApi_Rtsp_Pause;
        _s_sStreamHandle.pfnRtspGetPayload = MApi_Rtsp_Get_Payload;
#else
        _s_sStreamHandle.pfnStreamOpen = NULL;
        _s_sStreamHandle.pfnStreamClose = NULL;
        _s_sStreamHandle.pfnStreamLength = NULL;
        _s_sStreamHandle.pfnStreamSeek = NULL;
        _s_sStreamHandle.pfnStreamRead = NULL;
        _s_sStreamHandle.pfnRtspOpen = MM_Simulate_Rtsp_Open;
        _s_sStreamHandle.pfnRtspClose = MM_Simulate_Rtsp_Close;
        _s_sStreamHandle.pfnRtspPlay = MM_Simulate_Rtsp_Play;
        _s_sStreamHandle.pfnRtspPause = MM_Simulate_Rtsp_Pause;
        _s_sStreamHandle.pfnRtspResume = MM_Simulate_Rtsp_Resume;
        _s_sStreamHandle.pfnRtspGetPayload = MM_Simulate_Rtsp_Get_Payload;
#endif
        sMplayerMeida.eFileMode=MPLAYER_RTSP_MODE;
        sMplayerMeida.sSubtitle.eSubtitleType = MPLAYER_SUBTITLE_INVALID;
        sMplayerMeida.sSubtitle.SubtitleFileName = NULL;
        sMplayerMeida.sSubtitle.SubtitleSUBFileName = NULL;
        sMplayerMeida.filename = (char*)U8File;//utf8fn;
        sMplayerMeida.u32DispX = 0;
        sMplayerMeida.u32DispY = 0;
        sMplayerMeida.u32DispW = 0;
        sMplayerMeida.u32DispH = 0;
        sMplayerMeida.u32GotoTimeMs = 0;
        B_Ret = MApi_MPlayer_PlayFile(eMediaType, &sMplayerMeida, &_s_sStreamHandle);

    }

    return B_Ret;
}

//------------------------------------------------------------------------------
/// @brief The sample code to change Audio track
/// @param u32TrackId:
/// @return TRUE:  success.
/// @return FALSE:  fail.
/// @sa
/// @note
/// Command: \b MM_AudioTrack \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_MM_SetAudioTrack(MS_U32 *u32TrackId)
{
        MS_U32 u32AudioTrackNum;
        if (eMediaType !=E_MPLAYER_MEDIA_MOVIE)
        {
            return FALSE;
        }
        u32AudioTrackNum = MApi_MPlayer_GetOption(E_MPLAYER_MEDIA_MOVIE,MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_NB_AUDIO_TRACK);
        printf("Audio Track Num  = %ld\n", u32AudioTrackNum);
        if (u32AudioTrackNum <= *u32TrackId)
        {
            printf("Track Id(%ld) must <= u32AudioTrackNum\n",u32AudioTrackNum);
            return FALSE;
        }
        MApi_MPlayer_SetOption(E_MPLAYER_MEDIA_MOVIE, MPLAYER_OPTION_CHANGE_AUDIO,*u32TrackId);
        return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to query if MM is init or not.
/// @return TRUE:  Init state.
/// @return FALSE:  Un-init state.
/// @sa
/// @note
//------------------------------------------------------------------------------
MS_BOOL appDemo_MM_IsInit(void)
{
    return b_MM_Initied;
}
//------------------------------------------------------------------------------
/// @brief The sample code to get MM-photo decode buffer physical address
/// @return decode buffer physical address
/// @sa
/// @note
//------------------------------------------------------------------------------
MS_U32 appDemo_MM_GetOutAdr(void)
{
    return JPD_OUT_ADR;
}
//------------------------------------------------------------------------------
/// @brief The sample code to change subtitle track
/// @param u32TrackId:
/// @return TRUE:  success.
/// @return FALSE:  fail.
/// @sa
/// @note
/// Command: \b MM_SubtitleTrack \n
//------------------------------------------------------------------------------

MS_BOOL appDemo_MM_SetSubtitleTrack(MS_U32 *u32TrackId)
{
    MS_U32 u32SubtitleTrackNum;
    if (eMediaType !=E_MPLAYER_MEDIA_MOVIE)
    {
        return FALSE;
    }
    u32SubtitleTrackNum = MApi_MPlayer_GetOption(E_MPLAYER_MEDIA_MOVIE,MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_NB_SUBTITLE_TRACK);
    printf("Subtitle Track Num  = %ld\n", u32SubtitleTrackNum);
    if (u32SubtitleTrackNum<= *u32TrackId)
    {
        printf("Track Id must <= u32SubtitleTrackNum\n");
        return FALSE;
    }
    MApi_MPlayer_SetOption(E_MPLAYER_MEDIA_MOVIE, MPLAYER_OPTION_CHANGE_INTERNAL_SUBTITLE,*u32TrackId);
    return TRUE;
}


#if MM_REMOVE_TXT_SUBTITLE == 0
static void __TxtSbt_thread(U32 argc, void *arg)
{
    MS_S32 i;
    while(!_bExitSubTask)
    {
        MsOS_DelayTask(30);
        MsOS_ObtainMutex(_s32QMutex, MSOS_WAIT_FOREVER);
        if(s32QFront == s32QBack)
        {
            mm_print("Queue is empty!!!\n");
            MsOS_ReleaseMutex(_s32QMutex);
            continue;
        }

        MS_U32 u32Value;
        u32Value = MApi_MPlayer_GetOption(eMediaType, MPLAYER_OPTION_MOVIE_INFO, MPLAYER_INFO_CUR_TIME);
        if(m_QElement[s32QFront].u32PTS <= u32Value)
        {
            mm_print("curr time= %d\n",u32Value);
            printf("===================\n");
            for(i=0;i<m_QElement[s32QFront].u16Len;i++)
                printf("%c", ((char*)m_QElement[s32QFront].pu8Buf)[i]);
            printf("\n===================\n");

            if(MsOS_FreeMemory(m_QElement[s32QFront].pu8Buf, gs32NonCachedPoolID))
                m_QElement[s32QFront].pu8Buf = NULL;

            s32QFront++;
            if(s32QFront >= Q_SIZE)
                s32QFront = 0;
        }
        MsOS_ReleaseMutex(_s32QMutex);
    }
    MsOS_SetEvent(_s32SubTaskExitEvt, 1);// 1 for exiting subtitle task
}

static MS_BOOL _TxtSubtitleTaskInit(void)
{
    if(m_InternalSubThread.iId == -1)
    {
        m_InternalSubThread.pStack = MsOS_AllocateMemory(m_InternalSubThread.u32StackSize, gs32NonCachedPoolID);
        if(!m_InternalSubThread.pStack)
            return FALSE;

        _bExitSubTask = FALSE;
        m_InternalSubThread.iId = MsOS_CreateTask( __TxtSbt_thread,
                                        (MS_U32)NULL,
                                        m_InternalSubThread.ePriority,
                                        TRUE,
                                        m_InternalSubThread.pStack,
                                        m_InternalSubThread.u32StackSize,
                                        m_InternalSubThread.szName);

        if(m_InternalSubThread.iId == -1)
        {
            printf("????? Create pthread sbt_thread error!\n");
            _bExitSubTask = TRUE;
            return FALSE;
        }
    }

    return TRUE;
}

static void _MM_TxtSubtitleInit(void)
{
    if(eMediaType == E_MPLAYER_MEDIA_MOVIE && _u32FileOption == MPLAYER_FILE_MODE)
    {
        if(!_bExitSubTask)
            return;

        _s32QMutex = MsOS_CreateMutex(E_MSOS_FIFO, "QMutex", MSOS_PROCESS_SHARED);
        _s32SubTaskExitEvt = MsOS_CreateEventGroup("SubTaskExitEvt");
        _TxtSubtitleTaskInit();
    }
}

static void _MM_TxtSubtitleFinalize(void)
{
    if(eMediaType == E_MPLAYER_MEDIA_MOVIE && _u32FileOption == MPLAYER_FILE_MODE)
    {//for stopping the subtitle related
        if(_bExitSubTask)
            return;

        _bExitSubTask = TRUE;
        MS_U32  pu32RetrievedEventFlag;
        MsOS_WaitEvent(_s32SubTaskExitEvt, 1, &pu32RetrievedEventFlag, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        if(MsOS_DeleteTask(m_InternalSubThread.iId) == TRUE)
            m_InternalSubThread.iId = -1;

        if(m_InternalSubThread.pStack != NULL)
        {
            MsOS_FreeMemory (m_InternalSubThread.pStack, gs32NonCachedPoolID);
            m_InternalSubThread.pStack = NULL;
        }

        MsOS_DeleteEventGroup(_s32SubTaskExitEvt);
        MsOS_DeleteMutex(_s32QMutex);

        while(s32QBack != s32QFront)
        {
            if(MsOS_FreeMemory(m_QElement[s32QFront].pu8Buf, gs32NonCachedPoolID))
                m_QElement[s32QFront].pu8Buf = NULL;

            s32QFront++;
            if(s32QFront >= Q_SIZE)
                s32QFront = 0;
        }
    }
}
#endif

#endif
