////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __MSAPI_DMX_H__
#define __MSAPI_DMX_H__

////////////////////////////////////////////////////////////////////////////////
///
/// @file msAPI_demxu.h
/// @author MStar Semiconductor Inc.
/// @brief Demux API wrapper
///
////////////////////////////////////////////////////////////////////////////////

/******************************************************************************/
/*                                Include files                               */
/******************************************************************************/
#include "linux/types.h"
#include "sys/common/MsTypes.h"
#include "sys/api/apiDMX.h"


/******************************************************************************/
/*                                   Define                                   */
/******************************************************************************/
// For New TSP driver
// To Do: This define constant should be remove after test and verification, 2009/04/03
#define ENABLE_NEW_DMX_INTERFACE            TRUE
#define CSA_ENABLE 0
#if (ENABLE_NEW_DMX_INTERFACE)
#define MSAPI_DMX_INVALID_PID               0x1fff
#define MSAPI_DMX_INVALID_FLT_ID            DMX_DMXID_NULL
#define MSAPI_DMX_TIMEOUT_INFINITE          0xffff
#define MSAPI_DMX_MATCH_PATTERN_LENGTH      16

    #if 0 //( MS_SW_CUS_SEL == SW_CONFIG_TRUNK_ATSC )

        #define MSAPI_DMX_SEC1K_FILTER_NUMBER       16
        #define MSAPI_DMX_SEC4K_FILTER_NUMBER       16
        #define MSAPI_DMX_MAX_FILTER_NUMBER         40//(MSAPI_DMX_SEC1K_FILTER_NUMBER+MSAPI_DMX_SEC4K_FILTER_NUMBER)
        #define MSAPI_DMX_USABLE_FILTER_NUMBER      32

    #else
        #define MSAPI_DMX_SEC1K_FILTER_NUMBER       8
        #define MSAPI_DMX_SEC4K_FILTER_NUMBER       8
        #define MSAPI_DMX_MAX_FILTER_NUMBER         32//(MSAPI_DMX_SEC1K_FILTER_NUMBER+MSAPI_DMX_SEC4K_FILTER_NUMBER)
        #define MSAPI_DMX_USABLE_FILTER_NUMBER      16

    #endif

#define MSAPI_DMX_SECTION_1K                1024
#define MSAPI_DMX_SECTION_4K                4096
#endif
#define MSAPI_DMX_RECORD_FILTER_NUMBER      16
#if(CSA_ENABLE == 1)
#define CSA_FILTER_ID 100
#endif
/******************************************************************************/
/*                                   Macro                                    */
/******************************************************************************/
#define ENABLE_DEMUX_SW_CRC_CHECK         FALSE

/******************************************************************************/
/*                                   Enum                                     */
/******************************************************************************/
#if (ENABLE_NEW_DMX_INTERFACE)

#define MSAPI_DMX_FILTER_TYPE_VIDEO             1           // to   video    fifo
#define MSAPI_DMX_FILTER_TYPE_AUDIO             2           // to   audio    fifo
#define MSAPI_DMX_FILTER_TYPE_AUDIO2            3           // to   audio2   fifo
#define MSAPI_DMX_FILTER_TYPE_SUBTITLE          4           // to   subtitle buffer
#define MSAPI_DMX_FILTER_TYPE_TELETEXT          5           // to   teletext buffer
#define MSAPI_DMX_FILTER_TYPE_PES               6           // to   section  buffer
#define MSAPI_DMX_FILTER_TYPE_SECTION_1K        7           // to   section  buffer
#define MSAPI_DMX_FILTER_TYPE_SECTION_4K        8           // to   section  buffer
#define MSAPI_DMX_FILTER_TYPE_SECTION_VERSION   9           // to   section  buffer
#define MSAPI_DMX_FILTER_TYPE_SECTION_CRC       10          // to   section  buffer
#define MSAPI_DMX_FILTER_TYPE_PCR               11          // no   output
#define MSAPI_DMX_FILTER_TYPE_PACKET            12          // to   section  buffer
#define MSAPI_DMX_FILTER_TYPE_PACKET_PUSI       13          // to   section  buffer
#define MSAPI_DMX_FILTER_TYPE_PACKET_AF         14          // to   section  buffer
#define MSAPI_DMX_FILTER_TYPE_OAD_MONITOR       15          // to   oad      buffer
#define MSAPI_DMX_FILTER_TYPE_OAD_DOWNLOAD      16          // to   oad      buffer
#define MSAPI_DMX_FILTER_TYPE_RECORD            17          // to   record   buffer
#define MSAPI_DMX_FILTER_TYPE_CSA_PACKET            0xFF
#define MSAPI_DMX_FILTER_SOURCE_TYPE_MASK       0xC0000000  // DMX filter source mask
#define MSAPI_DMX_FILTER_SOURCE_TYPE_LIVE       0x00000000  // DMX live stream filter source
#define MSAPI_DMX_FILTER_SOURCE_TYPE_FILE       0x80000000  // DMX file in filter type
#define MSAPI_DMX_FILTER_TYPE U32

typedef enum
{
    MSAPI_DMX_FILTER_STATUS_SECAVAILABLE    =  DMX_EVENT_DATA_READY,
    MSAPI_DMX_FILTER_STATUS_PESAVAILABLE    =  DMX_EVENT_DATA_READY,
    MSAPI_DMX_FILTER_STATUS_OVERFLOW        =  DMX_EVENT_BUF_OVERFLOW,
    MSAPI_DMX_FILTER_STATUS_OK,
    MSAPI_DMX_FILTER_STATUS_TIMEOUT,
    MSAPI_DMX_FILTER_STATUS_CRCINCORRECT,
    MSAPI_DMX_FILTER_STATUS_REQUESTMEET,
    MSAPI_DMX_FILTER_STATUS_COPYSECERR,
    MSAPI_DMX_FILTER_STATUS_ERROR
} MSAPI_DMX_FILTER_STATUS;

#endif

#if 0
#define MSAPI_DMX_BYPASS_PID                0x1fff


//<<filter status>>==============================================================================//
typedef enum
{
    MSAPI_DMX_FILTER_STATUS_OK              =  1,
    MSAPI_DMX_FILTER_STATUS_TIMEOUT         =  2,
    MSAPI_DMX_FILTER_STATUS_SECAVAILABLE    =  3,
    MSAPI_DMX_FILTER_STATUS_CRCINCORRECT    =  4,
    MSAPI_DMX_FILTER_STATUS_REQUESTMEET     =  5,
    MSAPI_DMX_FILTER_STATUS_PESAVAILABLE    =  6,
    MSAPI_DMX_FILTER_STATUS_COPYSECERR      =  7,
    MSAPI_DMX_FILTER_STATUS_OVERFLOW        =  8,
    MSAPI_DMX_FILTER_STATUS_ERROR           =  9
} MSAPI_DMX_FILTER_STATUS;

//<<filter type>>================================================================================//
typedef enum
{
    MSAPI_DMX_FILTER_TYPE_VIDEO             =  1,       // to   video    fifo
    MSAPI_DMX_FILTER_TYPE_AUDIO             =  2,       // to   audio    fifo
    MSAPI_DMX_FILTER_TYPE_AUDIO2             =  3,        // to   audio2    fifo
    MSAPI_DMX_FILTER_TYPE_SUBTITLE          =  4,       // to   subtitle buffer
    MSAPI_DMX_FILTER_TYPE_TELETEXT          =  5,       // to   teletext buffer
    MSAPI_DMX_FILTER_TYPE_PES               =  6,       // to   section  buffer
    MSAPI_DMX_FILTER_TYPE_SECTION_1K        =  7,       // to   section  buffer
    MSAPI_DMX_FILTER_TYPE_SECTION_4K        =  8,       // to   section  buffer
    MSAPI_DMX_FILTER_TYPE_SECTION_VERSION   =  9,       // to   section  buffer
    MSAPI_DMX_FILTER_TYPE_SECTION_CRC       = 10,       // to   section  buffer
    MSAPI_DMX_FILTER_TYPE_PCR                = 11,        // no   output
    MSAPI_DMX_FILTER_TYPE_PACKET            = 12,        // to   section  buffer
    MSAPI_DMX_FILTER_TYPE_PACKET_PUSI       = 13,       // to   section  buffer
    MSAPI_DMX_FILTER_TYPE_PACKET_AF         = 14,       // to   section  buffer
    MSAPI_DMX_FILTER_TYPE_OAD                = 15,        // to   oad      buffer
    MSAPI_DMX_FILTER_TYPE_RECORD            = 16,       // to   record   buffer
    MSAPI_DMX_FILTER_TYPE_SECTION_VER_1K    = 17,       // to   section  buffer
    MSAPI_DMX_FILTER_TYPE_SECTION_CRC_1K    = 18        // to   section  buffer
} MSAPI_DMX_FILTER_TYPE;

#endif


/******************************************************************************/
/*                                 Structure                                  */
/******************************************************************************/
#if (ENABLE_NEW_DMX_INTERFACE)
typedef struct _MS_DMX_SECTIONMONITOR_PARA
{
    U16 u16PID;
    U32 u32SectionFltType;
    U8 au8MatchByte[MSAPI_DMX_MATCH_PATTERN_LENGTH];
    U8 au8MaskByte[MSAPI_DMX_MATCH_PATTERN_LENGTH];
    U8 au8NotMatchByte[MSAPI_DMX_MATCH_PATTERN_LENGTH];
    void (*pfNotify)(U8 u8FID, DMX_EVENT enEvent);
    U16 u16TimeoutMilliSec;
    DMX_SECT_MODE SectMode;
    U8* pu8FID;
} MS_DMX_SECTIONMONITOR_PARA;

typedef struct _MS_DMX_SECTIONGET_PARA
{
    U16 u16PID;
    U32 u32SectionFltType;
    U8 au8MatchByte[MSAPI_DMX_MATCH_PATTERN_LENGTH];
    U8 au8MaskByte[MSAPI_DMX_MATCH_PATTERN_LENGTH];
    U8 au8NotMatchByte[MSAPI_DMX_MATCH_PATTERN_LENGTH];
    U8* pu8Section;
    U16 u16MaxSecSize;
    U16 u16TimeoutMilliSec;
    BOOLEAN bCheckCrc;
} MS_DMX_SECTIONGET_PARA;
#endif

#if 0
/*data structure ================================================================================*/
//<<section monitor parameter>>==================================================================//
typedef struct msAPI_Dmx_SectionMonitor_Para
{
    U16 u16PID;
    MSAPI_DMX_FILTER_TYPE enSectionFltType;
    U8 au8MatchByte[MSAPI_DMX_MATCH_PATTERN_LENGTH];
    U8 au8MaskByte[MSAPI_DMX_MATCH_PATTERN_LENGTH];
    U8 au8NotMatchByte[MSAPI_DMX_MATCH_PATTERN_LENGTH];
    void (*pfNotify)(U8 u8FID, MSAPI_DMX_FILTER_STATUS enFilterStatus);
    U16 u16TimeoutMilliSec;
    BOOLEAN bOneShot;
    U8* pu8FID;
} _msAPI_Dmx_SectionMonitor_Para;

//<<section get parameter>>======================================================================//
typedef struct msAPI_Dmx_SectionGet_Para
{
    U16 u16PID;
    MSAPI_DMX_FILTER_TYPE enSectionFltType;
    U8 au8MatchByte[MSAPI_DMX_MATCH_PATTERN_LENGTH];
    U8 au8MaskByte[MSAPI_DMX_MATCH_PATTERN_LENGTH];
    U8 au8NotMatchByte[MSAPI_DMX_MATCH_PATTERN_LENGTH];
    U8* pu8Section;
    U16 u16MaxSecSize;
    U16 u16TimeoutMilliSec;
    BOOLEAN bCheckCrc;
} _msAPI_Dmx_SectionGet_Para;
#endif


/******************************************************************************/
/*                                 Variables                                  */
/******************************************************************************/


/******************************************************************************/
/*                            Function Prototypes                             */
/******************************************************************************/
#ifdef MSAPI_DEMUX_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

INTERFACE BOOLEAN msAPI_DMX_CheckCRC32(U8* pu8Section);
INTERFACE DMX_FILTER_STATUS msAPI_DMX_Close(U8 u8FltID);
INTERFACE DMX_FILTER_STATUS msAPI_DMX_Stop(U8 u8FltID);
INTERFACE DMX_FILTER_STATUS msAPI_DMX_Reset(U8 u8FltID);
INTERFACE DMX_FILTER_STATUS msAPI_DMX_Start(U8 u8FltID);
INTERFACE DMX_FILTER_STATUS msAPI_DMX_Pid(U8 u8FltID, U16* pu16Pid, BOOLEAN bSet);
INTERFACE DMX_FILTER_STATUS msAPI_DMX_Proc(U8 u8DmxId);
INTERFACE DMX_FILTER_STATUS msAPI_DMX_StartFilter(U16 u16Pid, U32 u32FltType, U8 *pu8FID);
INTERFACE DMX_FILTER_STATUS msAPI_DMX_SectionMonitor(MS_DMX_SECTIONMONITOR_PARA * pstSectionMonitor, BOOLEAN bFileIn);
INTERFACE DMX_FILTER_STATUS msAPI_DMX_SectionGet(MS_DMX_SECTIONGET_PARA * pstSectionGet);
INTERFACE DMX_FILTER_STATUS msAPI_DMX_Open(MS_DMX_SECTIONMONITOR_PARA * pstSectionMonitor, U32 *u32StartAddress, U32 *u32Size);

// Function to manage demux filter
INTERFACE void msAPI_DMX_CloseFilter(U8 u8FltID);
INTERFACE void msAPI_DMX_RestartFilter(U8 u8FltID);
INTERFACE DMX_FILTER_STATUS msAPI_DMX_CopyData(MS_U8 u8DmxId, MS_U8* pu8Buf, MS_U32 u32BufSize, MS_U32* pu32ActualSize, MS_U32* pu32RmnSize, DMX_CheckCb pfCheckCB);

INTERFACE BOOLEAN msAPI_DMX_IsCSA_ON(void);
#if(CSA_ENABLE == 1)
INTERFACE void msAPI_DMX_CSA_Enable(BOOLEAN bEnable, U8 u8FilterGroup);
#endif
#undef INTERFACE
#endif  /* MSAPI_DEMUX_H */

