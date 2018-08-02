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
//		msAPI_Subtitle_Decoder.h
// Description:
//		This file is a header file that defines the types, structures and functions of DVB subtitle decode engine
// Note:
//		MStar Semi.
//-------------------------------------------------------------------------------------------------

#ifndef _MSAPI_SUBTITLE_DECODER_H
#define _MSAPI_SUBTITLE_DECODER_H

//-------------------------------------------------------------------------------------------------
// Standard include files:
//-------------------------------------------------------------------------------------------------
#include "MsTypes.h"
#include "apiSubtitle_StreamData.h"


#ifdef __cplusplus
extern "C"
{
#endif


//-------------------------------------------------------------------------------------------------
// Project include files:
//-------------------------------------------------------------------------------------------------
#ifdef _APISUBTITLE_DECODER_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
#define PES_START_CODE_0                   0x00
#define PES_START_CODE_1                   0x00
#define PES_START_CODE_2                   0x01
#define PRIVATE_STREAM_1                   0xbd
#define STUFFING                           0xff

#define SUBTITLE_DATA_IDENTIFIER           0x20
#define SUBTITLE_STREAM_ID                 0x00

#define SYNC_BYTE                          0x0f
#define END_OF_PES_DATA_FIELD_MARKER_1_4_1 0xff
#define END_OF_PES_DATA_FIELD_MARKER_1_3_1 0x3f
#define PAGE_COMPOSITION_SEGMENT           0x10
#define REGION_COMPOSITION_SEGMENT         0x11
#define CLUT_DEFINITION_SEGMENT            0x12
#define OBJECT_DATA_SEGMENT                0x13
#define DISPLAY_DEFINITION_SEGMENT         0x14
#define END_OF_DISPLAY_SET_SEGMENT         0x80
#define END_OF_PES_DATA_MARKER             0xff

#define X_RESOLUTION                       720 // the horizontal resolution
#define Y_RESOLUTION                       576 // the vertical resolution
#define PCR_FREQUENCY                      90 //90 KHz, in ms
#define MAX_DISPLAY_SUPPORTED              1
#define MAX_PAGE_SUPPORTED                 1
#define MAX_REGION_SUPPORTED               8                       // DBOOK4.0
#define MAX_CLUT_SUPPORTED                 MAX_REGION_SUPPORTED    // DBOOK4.0
#define MAX_OBJECT_SUPPORTED               128                     // DBOOK4.0
#define MAX_SUBTITLE_QUEUE_SIZE            30                      // max size is 40

#define MAX_GOP_PALETTE_ENTRY              GOP_PALETTE_ENTRY_NUM   // max gop palette

#define TRANSPARENT_COLOR_KEY              0x0000
#define SBTL_DBG(x)

#define INVALIDVERNO                       0xff
#define INVALIDRGEIONID                    0xff
#define INVALIDCLUTID                      0xff
#define INVALIDIDX                         0xff


#define SBTL_ERR(x)                        x
#define SBTL_TRACE(x)                      //x
#define SBTL_INFO(x)                       //x
#define SBTL_TIMER(x)                      //x
#define SBTL_TEST(x)                       //x

#define FULL_PAGE_BUFFER                   0
#define PALETTE_FORMAT_ARGB6666            0x00
#define PALETTE_FORMAT_ARGB8888            0x01
#define SBTL_PALETTE_FORMAT                PALETTE_FORMAT_ARGB6666

#define FREERUN_TIME                       15
#define FIXED_NO_PTS_PACKET                TRUE

#define Subtitle_Destination_IP            0
#define Subtitle_H_Stretch                 1
#define Subtitle_V_Stretch                 1
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#ifndef ROUND_UP
#define ROUND_UP(size, granule) ((size)%(granule) ? (granule)*((size)/(granule)+1) : (size))
#endif
#ifndef ROUND_DOWN
#define ROUND_DOWN(size, granule) ((size)/(granule)*(granule))
#endif

#define MemAlign(n, unit)           ((((n)+(unit)-1)/(unit))*(unit))

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

typedef enum
{
    SBTL_RESULT_NOT_ENOUGH_DATA = 0,
    SBTL_RESULT_SUCCESS,
    SBTL_RESULT_BYPASS,
    SBTL_RESULT_FAILURE,
} DECODING_RESULTS;


typedef struct
{
    MS_U8 u8_R;
    MS_U8 u8_G;
    MS_U8 u8_B;
    MS_U8 u8_A;  // Transparency
} DvbSubDecoder_Colour;

typedef enum
{
    SBTL_PAGE_STATE_NORMAL_CASE = 0,    // 0x00
    SBTL_PAGE_STATE_ACQUISITION_POINT,  // 0x01
    SBTL_PAGE_STATE_MODE_CHANGE         // 0x02
} PAGE_STATE;

typedef struct _QueueElement
{
    MS_U32 u32Address;
    MS_U16 u16Len;      // The maximum PES length is 64k
    MS_U32 u32PTS;
}QueueElement;

typedef struct
{
    MS_U16 u16_x;
    MS_U16 u16_y;
    MS_U16 u16_width;
    MS_U16 u16_height;
    MS_U32 u32PixelBuffer;

    MS_U8  st_handle;  // BMPHANDLE
    MS_BOOL b_usage;
    MS_BOOL b_candidate;
} REGION_RECT; // the rectangle of region

typedef struct
{
    REGION_RECT st_dialog[MAX_REGION_SUPPORTED];
} GOP_PARAM;

typedef struct _Queue
{
    QueueElement    contents[MAX_SUBTITLE_QUEUE_SIZE];      // 10 bytes * MAX_SUBTITLE_QUEUE_SIZE
    MS_U8              u8Front;
    MS_U8              u8Count;
} PESQueue;

typedef struct _ACTIVE_REGION_INFO
{
    MS_U8      u8ID;
    MS_U16     u16X;
    MS_U16     u16Y;
}ACTIVE_REGION_INFO;

typedef struct DISPLAY
{
    MS_U8      u8VerNO;
    MS_U8      u8_display_window_flag;
    MS_U16 u16_display_width;
    MS_U16 u16_display_height;
    MS_U16 u16_display_window_left;
    MS_U16 u16_display_window_right;
    MS_U16 u16_display_window_top;
    MS_U16 u16_display_window_bottom;

} SUBTITLE_DISPLAY;

typedef struct PAGE
{
    MS_U16     u16ID;
    MS_U8      u8TimeOut;                                              // seconds
    MS_U8      u8State;                                                //PAGE_STATE  u8State;
    MS_U8      u8VerNO;
    MS_U32     u32_PTS_high;
    MS_U32     u32_PTS_low;
    MS_U8      u8RegionsCount;                                         // number of regions in this page
    ACTIVE_REGION_INFO  ActiveRegions[MAX_REGION_SUPPORTED];        //40 bytes
    MS_U16     u16TotalColours;
    MS_U8      u8ActiveRegions;    //New added in one PCS
}SUBTITLE_PAGE;

typedef struct REGION
{
    MS_U8      u8ID;
    MS_U8      u8VerNO;                  // 4
    MS_U8      u8FillFlag;               // 1
    MS_U16     u16Width;
    MS_U16     u16Pitch;
    MS_U16     u16Height;
    MS_U8      u8LevelofCompatibility;   // 3
    MS_U8      u8Depth;                  // 3
    MS_U8      u8CLUTID;
    MS_U8      u8PixelCode;
    MS_U32     u32PixelBuffer;
    MS_U8      u8PixelOffset;
    MS_U8      u8IdxObjects;
    MS_U8      u8IdxNext;
    MS_U8      u8ObjectsCount;           // number of objects in this page
    MS_U8      bFillRegion;
}SUBTITLE_REGION;

typedef struct CLUT
{
    MS_U8      u8ID;
    MS_U8      u8VerNO;
    MS_U8      _2_bit_CLUT[16];
    MS_U8      _4_bit_CLUT[64];
    MS_U8      _8_bit_CLUT[1024];
}SUBTITLE_CLUT;

typedef struct OBJECT
{
    MS_U16     u16ID;
    MS_U8      u8Type;
    MS_U16     u16X;
    MS_U16     u16Y;
    MS_U8      u8FGPixelCode;
    MS_U8      u8BGPixelCode;
    MS_U8      u8VerNO;
    MS_U8      u8CodingMethod;
    MS_U8      u8NonModifyingColourFlag;
    MS_U8      u8IdxNext;
}SUBTITLE_OBJECT;

typedef struct _PixelBuffer
{
    MS_U32     u32Address;
    MS_U32     u32Len;
}SUBTTILE_PIXELBUFFER;

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------
/*INTERFACE SUBTITLE_PAGE         *page;
INTERFACE SUBTITLE_REGION       *regions;
INTERFACE SUBTITLE_OBJECT       *objects;
INTERFACE SUBTITLE_CLUT         *CLUTs;
INTERFACE SUBTTILE_PIXELBUFFER  *PixelBuffer;
INTERFACE PESQueue              *queue;*/

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL msAPI_Subtitle_Decoder_Init(void);
INTERFACE MS_BOOL msAPI_Subtitle_Decoder_Main(void);
INTERFACE MS_BOOL msAPI_Subtitle_Decoder_Initialize(void);
INTERFACE MS_BOOL msAPI_Subtitle_Decoder_DecodeMain(void);

INTERFACE void msAPI_Subtitle_Queue_Init(void);
INTERFACE void msAPI_Subtitle_Renderer_Init(void);
INTERFACE void msAPI_Subtitle_Renderer_DeInit(void);
INTERFACE void msAPI_Subtitle_Renderer_Reset( void );
INTERFACE void msAPI_Subtitle_Renderer_Clear(void);
INTERFACE void msAPI_Subtitle_Renderer_Clear_All(void);
INTERFACE void msAPI_Subtitle_Renderer_Enable(MS_BOOL blEnable);
INTERFACE void msAPI_Subtitle_Decoder_SetPageIDs(MS_U16 u16CPID, MS_U16 u16APID);

INTERFACE void msAPI_Subtitle_RingBuffer_Reset(void);

INTERFACE MS_U32  msAPI_Subtitle_Queue_GetSDRAMAddress(MS_U16 u16Len);
INTERFACE void    msAPI_Subtitle_Queue_Reset(void);
INTERFACE MS_BOOL msAPI_Subtitle_Queue_Push(MS_U32 u32Address, MS_U16 u16Len, MS_U32 u32PTS);
INTERFACE MS_BOOL msAPI_Subtitle_Queue_Pop(QueueElement *qe);
INTERFACE MS_BOOL msAPI_Subtitle_Queue_PeekN(QueueElement *qe, MS_U8 u8N);
INTERFACE MS_BOOL msAPI_Subtitle_Decoder_CheckTimeOut(void);
INTERFACE void msAPI_Subtitle_DataControlAddr_Set(void);
INTERFACE void msAPI_Subtitle_DataControlMemory_Clear(void);
INTERFACE void msAPI_Subtitle_SetSF(MS_U8 u8SubtitleCurrentAspect); //-jp-20070825
INTERFACE void msAPI_Subtitle_SetMemoryLayout(void);
INTERFACE MS_U32 msAPI_Subtitle_Decoder_GetDemuxBuffer(void);
INTERFACE void msAPI_Subtitle_Decoder_Start(void);
INTERFACE void msAPI_Subtitle_Decoder_Stop(void);
INTERFACE MS_BOOL msAPI_Subtitle_Decoder_SetPid(MS_U8 u8TspId, MS_U16 u16Pid);
INTERFACE void msAPI_Subtitle_Decoder_SetGOPNum(MS_U8 u8Gop);
INTERFACE void msAPI_Subtitle_Decoder_DeInit( void );
//#if ( FULL_PAGE_BUFFER == 1)
void msAPI_Subtitle_Renderer_Create_OffScreenBuffer(void);
void msAPI_Subtitle_Renderer_Clear_OffScreenBuffer(void);
INTERFACE void msAPI_Subtitle_SetTSP(SUB_TSP_MODE eSetTSPMode);
INTERFACE void msAPI_Subtitle_Decoder_SetDMXFltSrc(MS_U32 u32DmxFltSrc);
INTERFACE MS_BOOL msAPI_Subtitle_SetSDOnly(MS_BOOL bEnable);

//#endif

//-------------------------------------------------------------------------------------------------
// Memory Layout
//-------------------------------------------------------------------------------------------------
INTERFACE MS_U32 SUBTITLE_DATACONTROL_ADR;
INTERFACE MS_U16 SUBTITLE_DATACONTROL_LEN;

INTERFACE MS_U32 SUBTITLE_PAGE_ADR;
INTERFACE MS_U8  SUBTITLE_PAGE_LEN;

INTERFACE MS_U32 SUBTITLE_REGION_ADR;
INTERFACE MS_U8  SUBTITLE_REGION_LEN;

INTERFACE MS_U32 SUBTITLE_CLUT_ADR;
INTERFACE MS_U16 SUBTITLE_CLUT_LEN;

INTERFACE MS_U32 SUBTITLE_OBJECT_ADR;
INTERFACE MS_U16 SUBTITLE_OBJECT_LEN;

INTERFACE MS_U32 SUBTITLE_BITMAP_ADR;
INTERFACE MS_U8  SUBTITLE_BITMAP_LEN;

INTERFACE MS_U32 SUBTITLE_PESQUE_ADR;
INTERFACE MS_U16 SUBTITLE_PESQUE_LEN;

INTERFACE MS_U32 SUBTITLE_PIXBUF_ADR;
INTERFACE MS_U8  SUBTITLE_PIXBUF_LEN;

// Chunk 2:  For decoded CLUT
INTERFACE MS_U32 SUBTITLE_CLUT_2BIT_ADR;
INTERFACE MS_U8  SUBTITLE_CLUT_2BIT_SIZE;
INTERFACE MS_U32 SUBTITLE_CLUT_2BIT_LEN;

INTERFACE MS_U32 SUBTITLE_CLUT_4BIT_ADR;
INTERFACE MS_U8  SUBTITLE_CLUT_4BIT_SIZE;
INTERFACE MS_U32 SUBTITLE_CLUT_4BIT_LEN;

INTERFACE MS_U32 SUBTITLE_CLUT_8BIT_ADR;
INTERFACE MS_U16 SUBTITLE_CLUT_8BIT_SIZE;
INTERFACE MS_U32 SUBTITLE_CLUT_8BIT_LEN;

// Chunk 3: Memory pool for decoded pixel buffer, Off-Screen Buffer
INTERFACE MS_U32 SUBTITLE_MEMPOOL_ADR;
INTERFACE MS_U32 SUBTITLE_MEMPOOL_LEN;

// Chunk 4: The queue buffer to store the PES payloads from Demux PES section Buffer 128K + 64K (Top & Bottom Field Data for RLD)
INTERFACE MS_U32 SBT_STREAM_BUF_ADR;
INTERFACE MS_U32 SBT_STREAM_BUF_LEN;

// Chunk 5: The buffer for parsing PES data
INTERFACE MS_U32 SBT_CACHE_BUF_ADR;
INTERFACE MS_U32 SBT_CACHE_BUF_LEN;

//cc:
INTERFACE MS_U32 SUBTITLE_NONMODIFY_ADR;
INTERFACE MS_U32 SUBTITLE_NONMODIFY_LEN;

INTERFACE MS_U32 SBTL_QUEUE_ADR;
INTERFACE MS_U32 SBTL_QUEUE_LEN;

INTERFACE MS_U32 SBTL_QUEUE_TOP_ADR;
INTERFACE MS_U32 SBTL_QUEUE_BOT_ADR;

//-------------------------------------------------------------------------------------------------

#undef INTERFACE



#ifdef __cplusplus
}
#endif


#endif // _MSAPI_SUBTITLE_DECODER_H

