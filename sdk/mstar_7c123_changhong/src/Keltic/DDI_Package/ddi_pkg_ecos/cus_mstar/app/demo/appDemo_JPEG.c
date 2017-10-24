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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file    appDemo_JPEG.c
/// @brief  Demo code for JPEG decoding
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------
// Include Files
//--------------------------------------------------------------------------------------------------
#include "MsMemory.h"
#include "MsCommon.h"
#include "MsOS.h"
#include "drvMVOP.h"
#include "drvXC_IOPort.h"
#include "drvTVEncoder.h"
#include "apiXC.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "apiPNL.h"
#include "apiHDMITx.h"
#include "apiJPEG.h"
#include "apiDAC.h"
#include "msAPI_XC.h"
#include "msAPI_VE.h"
#include "appDemo_JPEG.h"
#include "appDemo_Video.h"


//-------------------------------------------------------------------------------------------------
// Debug Macros
//-------------------------------------------------------------------------------------------------
static MS_BOOL _stappDemoJPEG_dbgSwitch = FALSE;

#define db_print(fmt, args...)                                          \
    {                                                                   \
        if( (_stappDemoJPEG_dbgSwitch) != FALSE )                                \
        {                                                               \
            printf("[%s][%d]" fmt, __FUNCTION__, __LINE__, ## args);    \
        }                                                               \
    }

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
#if defined(MSOS_TYPE_ECOS)
#define MSOS_API
#endif

//define jpd library
#define UTOPIA_JPD_OLD  1
#define UTOPIA_JPD_NEW  2
#define MS_JPD_API UTOPIA_JPD_NEW

//define slide show test
//#define SLIDE_SHOW_TEST

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------

#define JPEGBASEWIDTHMAX    1600
#define JPEGBASEHEIGHTMAX   1200
#define JPEGPROWIDTHMAX     1024
#define JPEGPROHEIGHTMAX    768

#define verJPD_DEBUG(x)     x

#if defined(MSOS_API)
#define VERJPD_TASK_STACK   4096
#endif

#if defined(CHIP_T3) || defined(CHIP_T4) || defined(CHIP_T7) || defined(CHIP_JANUS) || defined(CHIP_U4)
#define VERJPD_EVENT_ALL    (E_JPEG_EVENT_DEC_DONE | E_JPEG_EVENT_DEC_ERROR_MASK | E_JPEG_EVENT_DEC_MRB_DONE | E_JPEG_EVENT_DEC_MWB_FULL)
#else
#define VERJPD_EVENT_ALL    (E_JPEG_EVENT_DEC_DONE | E_JPEG_EVENT_DEC_ERROR_MASK | E_JPEG_EVENT_DEC_MRB_DONE)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_JPEG_IDLE = 0x01,
    E_JPEG_DECODE_ERR = 0x02,
    E_JPEG_DECODE_HEADER = 0x03,
    E_JPEG_DECODE_BEGIN = 0x04,
    E_JPEG_DECODE_DONE = 0x05,
    E_JPEG_WAITDONE = 0x06,
    E_JPEG_INIT = 0x07,
} EN_JPEG_DECODE_STATE;

/* //have been removed to apiJPEG.h
typedef enum
{
    E_DECODE_NONE = 0,
    E_DECODE_DONE,
    E_DECODING,
    E_DECODE_ERR,
    E_STREAM_READ_ERR
} EN_JPEG_DECODE_STATUS;
*/

typedef enum
{
    E_JPD_BASELINE_TEST     = 1
  , E_JPD_PROGRESSIVE_TEST
  , E_JPD_CMYK_TEST
} EN_JPEG_TEST;

typedef enum
{
    E_JPD_MAIN_DECODE       = 1
  , E_JPD_THUMBNAIL_DECODE
} EN_JPEG_DECODE_TYPE;

typedef enum
{
    MVOP_SOURCE_FIELD
  , MVOP_OUTPUT_FIELD
} MVOP_OP_FIELD_TYPE;

/// JPEG decoder file system
typedef struct JPD_FILE_FileSystem_s
{
    void ( *constructor )( struct JPD_FILE_FileSystem_s *structPtr );
    void ( *destructor )( struct JPD_FILE_FileSystem_s *structPtr );
    MS_BOOL (*open)( struct JPD_FILE_FileSystem_s *structPtr );
    void ( *close )( struct JPD_FILE_FileSystem_s *structPtr );
    MS_S32 (*read)( MS_U8 *Pbuf, MS_U32 max_bytes_to_read, MS_BOOL *Peof_flag,
        struct JPD_FILE_FileSystem_s *structPtr);//, MS_U8 Position);
    MS_BOOL ( *get_error_status )( struct JPD_FILE_FileSystem_s *structPtr);
    MS_BOOL ( *reset )( struct JPD_FILE_FileSystem_s *structPtr );
    MS_S32 ( *get_size )( struct JPD_FILE_FileSystem_s *structPtr );
    MS_BOOL eof_flag, error_flag;
} JPD_FILE_FileSystem_t, *PJPD_FILE_FileSystem_t;

//--------------------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------------------
static MS_U32 u32OutBuffVirAddr = 0;

extern MS_BOOL GE_DrawJpeg(void);
//--------------------------------------------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------------------------------------------
static EN_JPEG_DECODE_STATE g_state = E_JPEG_INIT;
static JPD_FILE_FileSystem_t _input_stream;
static PJPD_FILE_FileSystem_t _Pinput_stream = &_input_stream;
static MS_U32 u32ReadBuffVirAddr = 0;
static MS_U32 u32IntBuffVirAddr = 0;

static MS_U16 u16ImageWidth = 0, u16ImageHeight = 0;
static MS_U16 u16ImageOriWidth = 0, u16ImageOriHeight = 0;
static MS_U16 u16ImageNonAlignWidth = 0, u16ImageNonAlignHeight = 0;
static MS_U16 u16ImageAlignPitch = 0, u16ImageAlignWidth = 0, u16ImageAlignHeight = 0;
static MS_U16 u16ErrorCode = 0;
static MS_U8 u8ScaleDownFactor = 0;
static MS_BOOL bIsProgressive = FALSE;
static MS_BOOL bThumbnailFound = FALSE;
static MS_U8 u8DebugLevel = 0x0;

static MS_U8 *u8InputDataAddr = NULL;
static MS_U32 u32InputDataLength = 0;
static MS_U32 u32ReadPosition = 0;

static MS_S32 s32InitByteRead = 0;
static MS_BOOL u8EOF_flag = FALSE;

static JPEG_Info stJPEG_Info;
static JPEG_Status stJPEG_Status;
static const MSIF_Version *pu8JPEG_Ver;

static JPEG_EXIF_DateTime _gEXIF_DateTime;
static JPEG_EXIF_Orientation _gEXIF_Orientation;

#ifdef SLIDE_SHOW_TEST
static MS_U8 slide_count = 0;
#endif

MS_U8 u8BaseLineJPEG[] = {
    #include "bindata/colorbar_400x400_jpg.dat"
};

//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------
static void verJPD_FILE_constructor(JPD_FILE_FileSystem_t *thisPtr);
static void verJPD_FILE_destructor(JPD_FILE_FileSystem_t *thisPtr);
static MS_BOOL verJPD_FILE_open(JPD_FILE_FileSystem_t *thisPtr);
static void verJPD_FILE_close(JPD_FILE_FileSystem_t *thisPtr);
static MS_S32 verJPD_FILE_read(MS_U8 *Pbuf, MS_U32 max_bytes_to_read, MS_BOOL *Peof_flag, JPD_FILE_FileSystem_t *thisPtr);
static MS_BOOL verJPD_FILE_get_error_status(JPD_FILE_FileSystem_t *thisPtr);
static MS_BOOL verJPD_FILE_reset(JPD_FILE_FileSystem_t *thisPtr);
static MS_S32 verJPD_FILE_get_size(JPD_FILE_FileSystem_t *thisPtr);
static void verJPD_InitFileSystem(void);
static MS_S32 verJPD_FillHdrFunc(MS_U32 BufAddr, MS_U32 BufLength);
static MS_BOOL verJPD_PreLoadBuffer(JPEG_BuffLoadType u8MRBuffType);
static EN_JPEG_DECODE_STATUS verJPD_WaitDone(void);
static void verJPD_SetStatus(void);
static void verJPD_Init(EN_JPEG_TEST u8Source);
static void verJPD_StartDecode(EN_JPEG_DECODE_TYPE decode_type);

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//for file read
// APP need to provide his own I/O functions
static void verJPD_FILE_constructor(JPD_FILE_FileSystem_t *thisPtr)
{
    thisPtr->eof_flag = FALSE;
    thisPtr->error_flag = FALSE;
}

static void verJPD_FILE_destructor(JPD_FILE_FileSystem_t *thisPtr)
{
    thisPtr->close( thisPtr );
}

static MS_BOOL verJPD_FILE_open(JPD_FILE_FileSystem_t *thisPtr)
{
    thisPtr->close( thisPtr );
    thisPtr->eof_flag = FALSE;
    thisPtr->error_flag = FALSE;

    return TRUE;
}

static void verJPD_FILE_close(JPD_FILE_FileSystem_t *thisPtr)
{
    thisPtr->eof_flag = FALSE;
    thisPtr->error_flag = FALSE;
}

static MS_S32 verJPD_FILE_read(MS_U8 *Pbuf, MS_U32 max_bytes_to_read, MS_BOOL *Peof_flag, JPD_FILE_FileSystem_t *thisPtr)
{
    MS_U8* u8RequestDataAddr = 0;
    MS_S32 bytes_read = 0;
    MS_S32 i = 0;

    u8RequestDataAddr = Pbuf;

    verJPD_DEBUG(printf("verJPD_FILE_read,u8RequestDataAddr = 0x%lX, FilePosition = %lu, FileLength = %lu\n"
        , (MS_U32)u8RequestDataAddr, u32ReadPosition, u32InputDataLength));

    if (u32ReadPosition <= u32InputDataLength)
    {
        bytes_read = max_bytes_to_read;
        if ((u32ReadPosition + max_bytes_to_read) >= u32InputDataLength)
        {
            bytes_read = u32InputDataLength - u32ReadPosition;
            thisPtr->eof_flag = TRUE;
            *Peof_flag = TRUE;
        }

        for(i = 0; i < bytes_read; i++)
        {
            u8RequestDataAddr[i] = u8InputDataAddr[u32ReadPosition + i];
#if 0
            if(u8RequestDataAddr[i] != u8InputDataAddr[u32ReadPosition + i])
            {
                printf("Error read file.....i = %ld\n", i);
                bytes_read = -1;
                break;
            }
#endif
        }
        MsOS_FlushMemory();
    }
    else
    {
        thisPtr->eof_flag = TRUE;
        *Peof_flag = TRUE;
    }
    u32ReadPosition += bytes_read;
    verJPD_DEBUG(printf("verJPD_FILE_read, bytes_read = %lu\n", bytes_read));
    return bytes_read;
}

static MS_BOOL verJPD_FILE_get_error_status(JPD_FILE_FileSystem_t *thisPtr)
{
    return (thisPtr->error_flag);
}

static MS_BOOL verJPD_FILE_reset(JPD_FILE_FileSystem_t *thisPtr)
{
    thisPtr->error_flag = FALSE;
    thisPtr->eof_flag = FALSE;

    return TRUE;
}

static MS_S32 verJPD_FILE_get_size(JPD_FILE_FileSystem_t *thisPtr)
{
    MS_S32 size = 0;
    thisPtr = thisPtr;
    return size;
}

//--------------------------------------------------------------------------------------------------
static void verJPD_InitFileSystem(void)
{
    _Pinput_stream->constructor = verJPD_FILE_constructor;
    _Pinput_stream->destructor = verJPD_FILE_destructor;
    _Pinput_stream->open = verJPD_FILE_open;
    _Pinput_stream->close = verJPD_FILE_close;
    _Pinput_stream->read = verJPD_FILE_read;
    _Pinput_stream->get_error_status = verJPD_FILE_get_error_status;
    _Pinput_stream->reset = verJPD_FILE_reset;
    _Pinput_stream->get_size = verJPD_FILE_get_size;
}

//--------------------------------------------------------------------------------------------------
static MS_S32 verJPD_FillHdrFunc(MS_PHYADDR BufAddr, MS_U32 BufLength)
{
    MS_S32 bytes_read = 0;
    MS_U32 buf_left = 0;
    MS_U32 bufVirAddr = MS_PA2KSEG1((MS_U32)BufAddr);
    verJPD_DEBUG(printf("verJPD_FillHdrFunc, BufAddr = %lu, bufVirAddr = %lu\n", BufAddr, bufVirAddr));
    do
    {
        bytes_read = _Pinput_stream->read((MS_U8 *)(bufVirAddr + buf_left), BufLength - buf_left, &u8EOF_flag, _Pinput_stream);

        if (bytes_read < 0)
        {
            return bytes_read;
        }

        buf_left += bytes_read;
    } while(( buf_left < BufLength ) && ( !u8EOF_flag ));
    MApi_JPEG_UpdateReadInfo(buf_left, u8EOF_flag);
    return buf_left;
}

//--------------------------------------------------------------------------------------------------
static MS_BOOL verJPD_PreLoadBuffer(JPEG_BuffLoadType u8MRBuffType)
{
    MS_S32 bytes_read = 0;
    MS_U32 buf_left = 0;
    MS_U32 u32RequestDataAddr, u32RequestDataSize;

    if(u8EOF_flag)
    {
        MApi_JPEG_ProcessEOF(u8MRBuffType);
        return TRUE;
    }

    u32RequestDataAddr = u32ReadBuffVirAddr;
    u32RequestDataSize = JPD_READ_BUF_LEN/2;

    if(u8MRBuffType == E_JPEG_BUFFER_HIGH)
    {
        u32RequestDataAddr += u32RequestDataSize;
    }

    do
    {
        bytes_read = _Pinput_stream->read((MS_U8 *)(u32RequestDataAddr + buf_left), u32RequestDataSize - buf_left, &u8EOF_flag, _Pinput_stream);

        if (bytes_read == -1)
        {
            MApi_JPEG_SetErrCode( E_JPEG_STREAM_READ );
            return FALSE;
        }

        buf_left += bytes_read;
    } while(( buf_left < u32RequestDataSize ) && ( !u8EOF_flag ));
    MApi_JPEG_UpdateReadInfo(buf_left, u8EOF_flag);
    if(buf_left > 0)
    {
        MApi_JPEG_SetMRBufferValid(u8MRBuffType);
    }
    return TRUE;
}

//--------------------------------------------------------------------------------------------------
static EN_JPEG_DECODE_STATUS verJPD_WaitDone(void)
{

    EN_JPEG_DECODE_STATUS eDecodeStatus = E_DECODING;
    switch(MApi_JPEG_WaitDone())
    {
        case E_JPEG_DEC_DONE:
            verJPD_DEBUG(printf("Decode Done\n"));
            eDecodeStatus = E_DECODE_DONE;
            break;
        case E_JPEG_DEC_FAILED:
        case E_JPEG_DEC_RST_ERROR:
        case E_JPEG_DEC_BITSTREAM_ERROR:
            verJPD_DEBUG(printf("Baseline decode error\n"));
            eDecodeStatus = E_DECODE_ERR;
            break;
        case E_JPEG_DEC_MRBL_DONE:
            if(!verJPD_PreLoadBuffer(E_JPEG_BUFFER_LOW))
            {
                if(MApi_JPEG_GetErrorCode() == E_JPEG_STREAM_READ)
                    eDecodeStatus = E_STREAM_READ_ERR;
                else
                    eDecodeStatus = E_DECODE_ERR;
            }
            else
            {
                eDecodeStatus = E_DECODING;
            }
            break;
        case E_JPEG_DEC_MRBH_DONE:
            if(!verJPD_PreLoadBuffer(E_JPEG_BUFFER_HIGH))
            {
                if(MApi_JPEG_GetErrorCode() == E_JPEG_STREAM_READ)
                    eDecodeStatus = E_STREAM_READ_ERR;
                else
                    eDecodeStatus = E_DECODE_ERR;
            }
            else
            {
                eDecodeStatus = E_DECODING;
            }
            break;
        case E_JPEG_DEC_DECODING:
        default:
            eDecodeStatus = E_DECODING;
            break;
    }
    return eDecodeStatus;
}

//--------------------------------------------------------------------------------------------------
static void verJPD_SetStatus(void)
{
    if( g_state == E_JPEG_DECODE_DONE )
    {
        verJPD_DEBUG(printf("verJPD_SetStatus:E_JPEG_DECODE_DONE\n"));
        u16ImageWidth = MApi_JPEG_GetWidth();
        u16ImageHeight = MApi_JPEG_GetHeight();
        bIsProgressive = MApi_JPEG_IsProgressive();
        u16ImageOriWidth = MApi_JPEG_GetOriginalWidth();
        u16ImageOriHeight = MApi_JPEG_GetOriginalHeight();
        u16ImageNonAlignWidth = MApi_JPEG_GetNonAlignmentWidth();
        u16ImageNonAlignHeight = MApi_JPEG_GetNonAlignmentHeight();
        u8ScaleDownFactor = MApi_JPEG_GetScaleDownFactor();
        u8DebugLevel = MApi_JPEG_GetDbgLevel();
        MApi_JPEG_GetInfo(&stJPEG_Info);
        MApi_JPEG_GetStatus(&stJPEG_Status);
        MApi_JPEG_GetLibVer(&pu8JPEG_Ver);
        u16ImageAlignPitch = MApi_JPEG_GetAlignedPitch(),
        u16ImageAlignWidth = MApi_JPEG_GetAlignedWidth(),
        u16ImageAlignHeight = MApi_JPEG_GetAlignedHeight();
        bThumbnailFound = MApi_JPEG_ThumbnailFound();
        MApi_JPEG_GetEXIFDateTime(&_gEXIF_DateTime);
        MApi_JPEG_GetEXIFOrientation(&_gEXIF_Orientation);
    }
    else if( g_state == E_JPEG_DECODE_ERR )
    {
        verJPD_DEBUG(printf("verJPD_SetStatus:E_JPEG_DECODE_ERR\n"));
        u16ImageWidth = MApi_JPEG_GetWidth();
        u16ImageHeight = MApi_JPEG_GetHeight();
        bIsProgressive = MApi_JPEG_IsProgressive();
        u16ImageOriWidth = MApi_JPEG_GetOriginalWidth();
        u16ImageOriHeight = MApi_JPEG_GetOriginalHeight();
        u16ImageNonAlignWidth = MApi_JPEG_GetNonAlignmentWidth();
        u16ImageNonAlignHeight = MApi_JPEG_GetNonAlignmentHeight();
        u8ScaleDownFactor = MApi_JPEG_GetScaleDownFactor();
        u16ErrorCode = MApi_JPEG_GetErrorCode();
        u8DebugLevel = MApi_JPEG_GetDbgLevel();
        MApi_JPEG_GetInfo(&stJPEG_Info);
        MApi_JPEG_GetStatus(&stJPEG_Status);
        MApi_JPEG_GetLibVer(&pu8JPEG_Ver);
        u16ImageAlignPitch = MApi_JPEG_GetAlignedPitch(),
        u16ImageAlignWidth = MApi_JPEG_GetAlignedWidth(),
        u16ImageAlignHeight = MApi_JPEG_GetAlignedHeight();
        bThumbnailFound = MApi_JPEG_ThumbnailFound();
        MApi_JPEG_GetEXIFDateTime(&_gEXIF_DateTime);
        MApi_JPEG_GetEXIFOrientation(&_gEXIF_Orientation);
    }
}

//--------------------------------------------------------------------------------------------------
/// API for JPEG decoder initialization::
static void verJPD_Init(EN_JPEG_TEST u8Source)
{
    verJPD_DEBUG(printf("verJPD_Init\n"));

    //for test code
    g_state = E_JPEG_INIT;
    u16ImageWidth = 0;
    u16ImageHeight = 0;
    u16ImageOriWidth = 0;
    u16ImageOriHeight = 0;
    u16ImageNonAlignWidth = 0;
    u16ImageNonAlignHeight = 0;
    u16ErrorCode = 0;
    u8ScaleDownFactor = 0;
    bIsProgressive = FALSE;

    u8DebugLevel = 0x0;

    s32InitByteRead = 0;
    u8EOF_flag = FALSE;

      
    u32ReadBuffVirAddr = MS_PA2KSEG1((MS_U32)JPD_READ_BUF_ADR);
    u32IntBuffVirAddr = MS_PA2KSEG1((MS_U32)JPD_INTER_BUF_ADR);
    u32OutBuffVirAddr = MS_PA2KSEG1((MS_U32)JPD_OUT_ADR);
    
    printf("u32ReadBuffVirAddr = %lx, u32IntBuffVirAddr = %lx, u32OutBuffVirAddr = %lx\n", u32ReadBuffVirAddr, u32IntBuffVirAddr, u32OutBuffVirAddr);

        u8InputDataAddr = (MS_U8 *)u8BaseLineJPEG;
        u32InputDataLength = sizeof(u8BaseLineJPEG);

    u32ReadPosition = 0;


    _gEXIF_DateTime.bHasDataTime = FALSE;
    _gEXIF_DateTime.u32Year = 0;
    _gEXIF_DateTime.u32Month = 0;
    _gEXIF_DateTime.u32Day = 0;
    _gEXIF_DateTime.u32Hour = 0;
    _gEXIF_DateTime.u32Minute = 0;
    _gEXIF_DateTime.u32Second = 0;
    _gEXIF_Orientation = E_JPEG_EXIF_ORIENT_NOT_FOUND;

    verJPD_InitFileSystem();

    MApi_JPEG_SetDbgLevel(E_JPEG_DEBUG_ALL);
}

//--------------------------------------------------------------------------------------------------
/// API for starting JPEG data decoding
//--------------------------------------------------------------------------------------------------
static void verJPD_StartDecode(EN_JPEG_DECODE_TYPE decode_type)
{
    if(g_state != E_JPEG_INIT)
    {
        verJPD_DEBUG(printf("verJPD_StartDecode failed, state = %d\n", g_state));
        return;
    }

    // init JPEG DECODE BUFFER
    {
        JPEG_InitParam InitParam;

        verJPD_DEBUG(printf("verJPD_StartDecode: INIT\n"));

        MApi_JPEG_SetMaxDecodeResolution(JPEGBASEWIDTHMAX, JPEGBASEHEIGHTMAX);
        MApi_JPEG_SetProMaxDecodeResolution(JPEGPROWIDTHMAX, JPEGPROHEIGHTMAX);

        InitParam.u32MRCBufAddr = JPD_READ_BUF_ADR;
        InitParam.u32MRCBufSize = JPD_READ_BUF_LEN;
        InitParam.u32MWCBufAddr = JPD_OUT_ADR;
        InitParam.u32MWCBufSize = JPD_OUT_LEN;
        InitParam.u32InternalBufAddr = JPD_INTER_BUF_ADR;
        InitParam.u32InternalBufSize = JPD_INTER_BUF_LEN;
        InitParam.u32DecByteRead = (MS_U32)s32InitByteRead;
        InitParam.bEOF = u8EOF_flag;
        if(E_JPD_MAIN_DECODE == decode_type)
        {
            InitParam.u8DecodeType = E_JPEG_TYPE_MAIN;
        }
        else
        {
            InitParam.u8DecodeType = E_JPEG_TYPE_THUMBNAIL;
        }
        InitParam.bInitMem = TRUE;
        InitParam.pFillHdrFunc = (JPEG_FillHdrFunc)verJPD_FillHdrFunc;

        MApi_JPEG_Init(&InitParam);

        if(MApi_JPEG_GetErrorCode() == E_JPEG_NO_ERROR)
        {
            g_state = E_JPEG_DECODE_HEADER;
        }
        else
        {
            g_state = E_JPEG_DECODE_ERR;
            verJPD_DEBUG(printf("verJPD_StartDecode: Init failed\n"));
        }
    }

    verJPD_DEBUG(printf("verJPD_StartDecode: INIT DONE : EventFlag = 0x%04X\n", MApi_JPEG_GetJPDEventFlag()));

    //parse JPEG Header
    if(g_state == E_JPEG_DECODE_HEADER)
    {
        verJPD_DEBUG(printf("verJPD_StartDecode: DECODE HEADER\n"));
        if(E_JPEG_FAILED == MApi_JPEG_DecodeHdr())
        {
            if(MApi_JPEG_GetErrorCode() == E_JPEG_STREAM_READ)
            {
                verJPD_DEBUG(printf("verJPD_StartDecode: DecodeHdr read failed\n"));
                g_state = E_JPEG_IDLE;
            }
            else
            {
                verJPD_DEBUG(printf("verJPD_StartDecode: DecodeHdr failed\n"));
                g_state = E_JPEG_DECODE_ERR;
            }
        }
        else
        {
            g_state = E_JPEG_DECODE_BEGIN;
        }
    }
    verJPD_DEBUG(printf("verJPD_StartDecode: DECODE HEADER DONE : EventFlag = 0x%04X\n", MApi_JPEG_GetJPDEventFlag()));

    if(g_state == E_JPEG_IDLE)
    {
        verJPD_DEBUG(printf("verJPD_StartDecode: IDLE\n"));
        MApi_JPEG_Rst();
        MApi_JPEG_Exit();
        g_state = E_JPEG_INIT;
        return;
    }


    //begin decode
    while(g_state == E_JPEG_DECODE_BEGIN)
    {
        verJPD_DEBUG(printf("verJPD_StartDecode: DECODE BEGIN\n"));
        switch(MApi_JPEG_Decode())
        {
            case E_JPEG_DONE:
                g_state = E_JPEG_DECODE_BEGIN;
                break;
            case E_JPEG_OKAY:
                g_state = E_JPEG_WAITDONE;
                break;
            case E_JPEG_FAILED:
            default:
                verJPD_DEBUG(printf("verJPD_StartDecode: DecodeBegin failed\n"));
                g_state = E_JPEG_DECODE_ERR;
                break;
        }
    }
    verJPD_DEBUG(printf("verJPD_StartDecode: DECODE BEGIN DONE : EventFlag = 0x%04X\n", MApi_JPEG_GetJPDEventFlag()));

    //wait done
    verJPD_DEBUG(printf("verJPD_StartDecode: WAITDONE\n"));
    while(g_state == E_JPEG_WAITDONE)
    {
        switch(verJPD_WaitDone())
        {
            case E_DECODE_DONE:
                g_state = E_JPEG_DECODE_DONE;
                break;

            case E_DECODING:
                g_state = E_JPEG_WAITDONE;
                break;

            case E_STREAM_READ_ERR:
            case E_DECODE_ERR:
            default:
                verJPD_DEBUG(printf("verJPD_StartDecode: WaitDone failed\n"));
                g_state = E_JPEG_DECODE_ERR;
                MApi_JPEG_Rst();
                break;
        }
    }

    verJPD_SetStatus();

    GE_DrawJpeg();

    MApi_JPEG_Exit();
    g_state = E_JPEG_INIT;
}
//------------------------------------------------------------------------------
/// @brief The sample code to decode JPEG picture to YUV422 format and then bitblt to GOP frame buffer
/// @param none
/// @return TRUE: Decode finished
/// @return FALSE: Decode failed
/// @sa
/// @note
/// Command: \b JPEG_MAIN \n
//------------------------------------------------------------------------------
MS_BOOL verJPD_Main(void)
{
    verJPD_DEBUG(printf("verJPD_Task start!!\n"));
    if ((JPD_READ_BUF_ADR == 0) ||(JPD_OUT_LEN == 0)||(JPD_INTER_BUF_LEN== 0))
    {
        printf("[%s][%d] Memory Space not support JPD command \n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    verJPD_Init(E_JPD_BASELINE_TEST);

    //Preload data for init JPEG decoder data.
    s32InitByteRead = _Pinput_stream->read((MS_U8 *)u32ReadBuffVirAddr, JPD_READ_BUF_LEN, &u8EOF_flag, _Pinput_stream);

    if(s32InitByteRead < 0)
    {
        verJPD_DEBUG(printf("Initial read file error!!\n"));
    }

    verJPD_StartDecode(E_JPD_MAIN_DECODE);

    verJPD_DEBUG(printf("verJPD_Task End!!\n"));

    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief The sample code to decode a JPEG picture and display it using XC path
/// @param none
/// @return TRUE: Process success
/// @return FALSE: Process fail
/// @sa
/// @note
/// Command: \b JPEG_XC_DISPLAY \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_JPEG_XC_Display(void)
{
    MS_WINDOW_TYPE tCropWin, tDstWin;
    VDEC_EX_DispInfo stVidStatus = {0};
    MSAPI_XC_JPEG_INFO stJpegInfo = {0};
    EN_JPEG_DECODE_TYPE decode_type = E_JPD_MAIN_DECODE;
    VDEC_StreamId *stVDECStreamId = NULL;
    stVDECStreamId = appDemo_Video_GetStreamID();

    if ((JPD_READ_BUF_ADR == 0) ||(JPD_OUT_LEN == 0)||(JPD_INTER_BUF_LEN== 0))
    {
        printf("[%s][%d] Memory Space not support JPD command \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    db_print("Demo display still JPEG start!!\n");

    // JPD tasks start
    db_print("JPD start!!\n");

    verJPD_Init(E_JPD_BASELINE_TEST);

    //Preload data for init JPEG decoder data.
    s32InitByteRead = _Pinput_stream->read((MS_U8 *)u32ReadBuffVirAddr, JPD_READ_BUF_LEN, &u8EOF_flag, _Pinput_stream);

    if(s32InitByteRead < 0)
    {
        db_print("Initial read file error!!\n");
    }

    // Paste verJPD_StartDecode codes start
    if(g_state != E_JPEG_INIT)
    {
        db_print("JPD decode failed, state = %d\n", g_state);
        return FALSE;
    }

    // init JPEG DECODE BUFFER
    {
        JPEG_InitParam InitParam;

        db_print("JPD init\n");

        MApi_JPEG_SetMaxDecodeResolution(JPEGBASEWIDTHMAX, JPEGBASEHEIGHTMAX);
        MApi_JPEG_SetProMaxDecodeResolution(JPEGPROWIDTHMAX, JPEGPROHEIGHTMAX);

        if(E_JPD_MAIN_DECODE == decode_type)
        {
            InitParam.u8DecodeType = E_JPEG_TYPE_MAIN;
        }
        else
        {
            InitParam.u8DecodeType = E_JPEG_TYPE_THUMBNAIL;
        }
        InitParam.u32MRCBufAddr         = JPD_READ_BUF_ADR;
        InitParam.u32MRCBufSize         = JPD_READ_BUF_LEN;
        InitParam.u32MWCBufAddr         = JPD_OUT_ADR;
        InitParam.u32MWCBufSize         = JPD_OUT_LEN;
        InitParam.u32InternalBufAddr     = JPD_INTER_BUF_ADR;
        InitParam.u32InternalBufSize    = JPD_INTER_BUF_LEN;
        InitParam.u32DecByteRead        = (MS_U32)s32InitByteRead;
        InitParam.bEOF                  = u8EOF_flag;
        InitParam.bInitMem              = TRUE;
        InitParam.pFillHdrFunc          = (JPEG_FillHdrFunc)verJPD_FillHdrFunc;

        MApi_JPEG_Init(&InitParam);

        if(MApi_JPEG_GetErrorCode() == E_JPEG_NO_ERROR)
        {
            g_state = E_JPEG_DECODE_HEADER;
        }
        else
        {
            g_state = E_JPEG_DECODE_ERR;
            db_print("JPD init failed\n");
        }
    }

    db_print("JPD init done, EventFlag = 0x%04X\n", MApi_JPEG_GetJPDEventFlag());

    //parse JPEG Header
    if(g_state == E_JPEG_DECODE_HEADER)
    {
        db_print("JPD decode header\n");

        if(E_JPEG_FAILED == MApi_JPEG_DecodeHdr())
        {
            if(MApi_JPEG_GetErrorCode() == E_JPEG_STREAM_READ)
            {
                db_print("JPD decode header read failed\n");
                g_state = E_JPEG_IDLE;
            }
            else
            {
                db_print("JPD decode header failed\n");
                g_state = E_JPEG_DECODE_ERR;
            }
        }
        else
        {
            g_state = E_JPEG_DECODE_BEGIN;
        }
    }

    db_print("JPD decoder header done, EventFlag = 0x%04X\n", MApi_JPEG_GetJPDEventFlag());

    if(g_state == E_JPEG_IDLE)
    {
        db_print("JPD idle\n");
        MApi_JPEG_Rst();
        MApi_JPEG_Exit();
        g_state = E_JPEG_INIT;
        return FALSE;
    }

    //begin decode
    while(g_state == E_JPEG_DECODE_BEGIN)
    {
        db_print("JPD decode begin\n");

        switch(MApi_JPEG_Decode())
        {
            case E_JPEG_DONE:
                g_state = E_JPEG_DECODE_BEGIN;
                break;
            case E_JPEG_OKAY:
                g_state = E_JPEG_WAITDONE;
                break;
            case E_JPEG_FAILED:
            default:
                db_print("JPD decode begin failed\n");
                g_state = E_JPEG_DECODE_ERR;
                break;
        }
    }

    db_print("JPD decode begin done, EventFlag = 0x%04X\n", MApi_JPEG_GetJPDEventFlag());

    //wait done
    db_print("JPD waitdone\n");

    while(g_state == E_JPEG_WAITDONE)
    {
        switch(verJPD_WaitDone())
        {
            case E_DECODE_DONE:
                g_state = E_JPEG_DECODE_DONE;
                break;
            case E_DECODING:
                g_state = E_JPEG_WAITDONE;
                break;
            case E_STREAM_READ_ERR:
            case E_DECODE_ERR:
            default:
                db_print("JPD waitdone failed\n");
                g_state = E_JPEG_DECODE_ERR;
                MApi_JPEG_Rst();
                break;
        }
    }

    verJPD_SetStatus();
    MApi_JPEG_Exit();

    g_state = E_JPEG_INIT;
    db_print("JPD end!!\n");
    // Paste verJPD_StartDecode codes end

    // Paste image_plane_jpeg_bitblt2MVOP codes start
    msAPI_XC_SetVideoMute(ENABLE, MAIN_WINDOW);

    // Set Input Information
    if (g_IPanel.DefaultVFreq() == 600)
    {
        stVidStatus.u32FrameRate  = 30000;
    }
    else
    {
        stVidStatus.u32FrameRate  = 25000;
    }
    if (bIsProgressive == TRUE)
    {
        stVidStatus.u8Interlace   = 0;
        db_print("JPEG is progressive!\n");
    }
    else
    {
        stVidStatus.u8Interlace   = 1;
        db_print("JPEG is NOT progressive!\n");
    }
    stVidStatus.u16HorSize      = (u16ImageWidth + 7) & 0xfff8;
    stVidStatus.u16VerSize      = u16ImageHeight;
    stVidStatus.u8AspectRate    = 2;
    stVidStatus.u16SarWidth     = 1;
    stVidStatus.u16SarHeight    = 1;
    stVidStatus.u16CropRight    = 0;
    stVidStatus.u16CropLeft     = 0;
    stVidStatus.u16CropBottom   = 0;
    stVidStatus.u16CropTop      = 0;
    stVidStatus.u32AspectWidth  = (u16ImageWidth + 7) & 0xfff8;
    stVidStatus.u32AspectHeight = u16ImageHeight;

    msAPI_XC_SetVDECInfo_EX(stVDECStreamId,&stVidStatus);

    //set JPEG still image config
    stJpegInfo.u8MiuSel = (JPD_OUT_ADR & 0x08000000) ? 1 : 0;
    stJpegInfo.u32BufAddr = JPD_OUT_ADR;
    db_print("JPD output fb addr = 0x%lX, MIU = %d", stJpegInfo.u32BufAddr, stJpegInfo.u8MiuSel);
    msAPI_XC_SetJPEGInfo(stJpegInfo);

    //set MVOP config
    msAPI_XC_ForceMVOPDramType(E_MSAPI_XC_MVOP_SOURCE_TYPE_422);
    msAPI_XC_SetMVOPConfig(MVOP_INPUT_DRAM);

    tCropWin.x = 0;
    tCropWin.y = 0;
    tCropWin.width = u16ImageWidth & 0xfffe;
    tCropWin.height = u16ImageHeight;

    tDstWin.x = 0;
    tDstWin.y = 0;
    if(u16ImageWidth > g_IPanel.Width())
    {
        tDstWin.width = g_IPanel.Width();
    }
    else
    {
        tDstWin.width = u16ImageWidth;
    }
    if(u16ImageHeight > g_IPanel.Width())
    {
        tDstWin.height = u16ImageHeight;
    }
    else
    {
        tDstWin.height = u16ImageHeight;
    }

    //printf("\n[%d]  %d %d %d %d\n",__LINE__,tCropWin.x, tCropWin.y, tCropWin.width, tCropWin.height);
    //printf("\n[%d]  %d %d %d %d\n",__LINE__,tDstWin.x, tDstWin.y, tDstWin.width, tDstWin.height);
    msAPI_XC_SetWin(NULL, &tCropWin, &tDstWin, MAIN_WINDOW);
    msAPI_VE_SetMode();

    //MVOP control for interlace JPEG, select STB_DC output field
    MDrv_MVOP_SEL_OP_FIELD(MVOP_OUTPUT_FIELD);

    // wait 4 input vsync before turn on scalar
    MApi_XC_WaitInputVSync(4, 200, MAIN_WINDOW);

    //MVOP control for interlace JPEG, select STB_DC source field
    MDrv_MVOP_SEL_OP_FIELD(MVOP_SOURCE_FIELD);

    msAPI_XC_SetVideoMute(DISABLE, MAIN_WINDOW);
    // Paste image_plane_jpeg_bitblt2MVOP codes end

    db_print("Demo display still JPEG end!!\n");
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
///Entry function for JPD testing.
//--------------------------------------------------------------------------------------------------

