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

#ifndef __mw_porting_dmx__
#define __mw_porting_dmx__

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#include "apiDMX.h"
///Define DMX section operation

#define MPL_DMX_PID_NULL               DMX_PID_NULL

// Please don't change this value if you know exactly what you are doing
/// DMX Filter pattern depth
#define MPL_DMX_SECT_FLT_DEPTH         DMX_SECT_FLT_DEPTH

#define MPL_DMX_DMXID_NULL               DMX_DMXID_NULL
#define MPL_MSIF_DMX_LIB_CODE            MSIF_DMX_LIB_CODE     //Lib code
#define MPL_MSIF_DMX_LIBVER              MSIF_DMX_LIBVER           //LIB version
#define MPL_MSIF_DMX_BUILDNUM            MSIF_DMX_BUILDNUM           //Build Number
#define MPL_MSIF_DMX_CHANGELIST          MSIF_DMX_CHANGELIST//P4 ChangeList Number

typedef struct
{
    MS_U8               *pMatch;
    MS_U8               *pMask;
    MS_U8               *pNMask;
    MS_U32              u32MatchSize;
    MS_PHYADDR          u32SectBufAddr;        ///< DMX section buffer in physical address
    MS_U32              u32SectBufSize;        ///< DMX section buffer size
    DMX_SECT_MODE       eSectMode;           ///< DMX section mode
    DMX_EVENT           eEvent;              ///< DMX section event
    DMX_NotifyCb        pNotify;            ///< DMX section callback
    DMX_Type2NotifyCb   pNotify2;            ///< DMX section callback
    MS_U32              u32NotifyParam;
    DMX_FILTER_TYPE     eFltType;
} MPL_DMX_FLT_INFO;

#define MPL_DMX_FILTER_TYPE MS_U32

#define MPL_DMX_FILTER_TYPE_VIDEO                   DMX_FILTER_TYPE_VIDEO   ///< DMX video filter type
#define MPL_DMX_FILTER_TYPE_AUDIO                   DMX_FILTER_TYPE_AUDIO   ///< DMX audio filter type
#define MPL_DMX_FILTER_TYPE_AUDIO2                  DMX_FILTER_TYPE_AUDIO2   ///< DMX audio2 filter type
#define MPL_DMX_FILTER_TYPE_SECTION                 DMX_FILTER_TYPE_SECTION   ///< DMX section filter type
#define MPL_DMX_FILTER_TYPE_PES                     DMX_FILTER_TYPE_PES   ///< DMX PES filter type
#define MPL_DMX_FILTER_TYPE_PACKET                  DMX_FILTER_TYPE_PACKET   ///< DMX TS packet filter type
#define MPL_DMX_FILTER_TYPE_PCR                     DMX_FILTER_TYPE_PCR   ///< DMX PCR filter type
#define MPL_DMX_FILTER_TYPE_TELETEXT                DMX_FILTER_TYPE_TELETEXT   ///< DMX teletext filter type
#define MPL_DMX_FILTER_TYPE_VIDEO3D                 DMX_FILTER_TYPE_VIDEO3D   ///< DMX V3D filter type
#define MPL_DMX_FILTER_TYPE_AUDIO3                  DMX_FILTER_TYPE_AUDIO3   ///< DMX audio 3 filter type
#define MPL_DMX_FILTER_TYPE_AUDIO4                  DMX_FILTER_TYPE_AUDIO4   ///< DMX audio 4 filter type
#define MPL_DMX_FILTER_TYPE_REC                     DMX_FILTER_TYPE_REC   ///< DMX record filter type no destination needs to be set
#define MPL_DMX_FILTER_TYPE_SCMBCHK                 DMX_FILTER_TYPE_SCMBCHK   ///< DMX filter type for checking TS scrambling status
#define MPL_DMX_FILTER_TYPE_SECTION_VER             DMX_FILTER_TYPE_SECTION_VER   ///< DMX teletext filter type

#define MPL_DMX_FILTER_FLT_MASK                     DMX_FILTER_FLT_MASK
#define MPL_DMX_FILTER_SOURCE_TYPE_MASK             DMX_FILTER_SOURCE_TYPE_MASK                   ///< DMX filter source mask
#define MPL_DMX_FILTER_SOURCE_TYPE_LIVE             DMX_FILTER_SOURCE_TYPE_LIVE                   ///< DMX live stream filter source
#define MPL_DMX_FILTER_SOURCE_TYPE_FILE             DMX_FILTER_SOURCE_TYPE_FILE                   ///< DMX file in filter type
#define MPL_DMX_FILTER_SOURCE_TYPE_LIVEB            DMX_FILTER_SOURCE_TYPE_FILE  ///< Input from TS1 to File PID filter
#define MPL_DMX_FILTER_SOURCE_TYPE_TS1              DMX_FILTER_SOURCE_TYPE_TS1                  ///< DMX live stream filter source 1
#define MPL_DMX_FILTER_SOURCE_TYPE_TS2              DMX_FILTER_SOURCE_TYPE_TS2                   ///< DMX live stream filter source 2
#define MPL_DMX_FILTER_SOURCE_TYPE_TS3              DMX_FILTER_SOURCE_TYPE_TS3                   ///< DMX live stream filter source 3
#define MPL_DMX_FILTER_SOURCE_TYPE_FILE1            DMX_FILTER_SOURCE_TYPE_FILE1                   ///< DMX file1 fource from TS1
#define MPL_DMX_FILTER_SOURCE_TYPE_FILE2            DMX_FILTER_SOURCE_TYPE_FILE2                   ///< DMX file2 fource from TS2
#define MPL_DMX_FILTER_SOURCE_TYPE_FILE3            DMX_FILTER_SOURCE_TYPE_FILE3

// Only supported by TSP ver2.0 driver
#define MPL_DMX_FILTER_SOURCE_TYPE_PLAYBACK         DMX_FILTER_SOURCE_TYPE_LIVE
#define MPL_DMX_FILTER_SOURCE_TYPE_PVR0             DMX_FILTER_SOURCE_TYPE_PVR0   ///< DMX PVR0 stream filter source
#define MPL_DMX_FILTER_SOURCE_TYPE_PVR1             DMX_FILTER_SOURCE_TYPE_PVR1   ///< DMX PVR1 stream filter source

#define MPL_DMX_FILTER_FLT_SCMB_MASK                DMX_FILTER_FLT_SCMB_MASK
#define MPL_DMX_FILTER_FLT_TYPE_SCMB                DMX_FILTER_FLT_TYPE_SCMB      ///< DMX descramble filter type
#define MPL_DMX_FILTER_FLT_TYPE_SCMB_SHAREKEY       DMX_FILTER_FLT_TYPE_SCMB_SHAREKEY      ///< DMX descramble filter type with sharekey

#define MPL_DMX_EVENT MS_U32
#define MPL_DMX_EVENT_DATA_READY          DMX_EVENT_DATA_READY ///< DMX event ready
#define MPL_DMX_EVENT_BUF_OVERFLOW        DMX_EVENT_BUF_OVERFLOW ///< DMX event overflow
#define MPL_DMX_EVENT_SEC_CRCERROR        DMX_EVENT_SEC_CRCERROR ///< DMX event CRC error
#define MPL_DMX_EVENT_PVRBUF_FULL         DMX_EVENT_PVRBUF_FULL ///< DMX event PVR buffer overflow
#define MPL_DMX_EVENT_PVR1BUF_FULL        DMX_EVENT_PVR1BUF_FULL ///< DMX event Channel Browser buffer overflow
#define MPL_DMX_EVENT_PVRBUF_SIZE_MET    DMX_EVENT_PVRBUF_SIZE_MET ///< DMX event PVR buffer callback size meet. note: Not support for all chips

/// TSP self task callback // optional --> default is CB by poll
#define MPL_DMX_EVENT_CB_MASK             DMX_EVENT_CB_MASK ///< DMX callback mask
#define MPL_DMX_EVENT_CB_POLL             DMX_EVENT_CB_POLL ///< DMX callback by polling // OBSOLETE
#define MPL_DMX_EVENT_CB_SELF             DMX_EVENT_CB_SELF ///< DMX callback by demux module
#define MPL_DMX_EVENT_CB_SELF_TYPE2       DMX_EVENT_CB_SELF_TYPE2 ///< DMX callback by demux module

#define MPL_DMX_SECT_MODE MS_U32
#define MPL_DMX_SECT_MODE_CONTI        DMX_SECT_MODE_CONTI ///< DMX receive section contiguously
#define MPL_DMX_SECT_MODE_ONESHOT      DMX_SECT_MODE_ONESHOT ///< DMX only receive one section
#define MPL_DMX_SECT_MODE_PESSCMCHK    DMX_SECT_MODE_PESSCMCHK ///< DMX checking PES SCMB status

// optional
#define MPL_DMX_SECT_MODE_CRCCHK       DMX_SECT_MODE_CRCCHK ///< DMX enables CRC check
#define MPL_DMX_SECT_MODE_AUTO_ADDR    DMX_SECT_MODE_AUTO_ADDR ///< DMX align read pointer to write pointer when get write pointer // OBSOLETE

typedef void (*MPL_DMX_NotifyCb)(MS_U8 u8DmxId, MPL_DMX_EVENT enEvent);
typedef void (*MPL_DMX_Type2NotifyCb)(MS_U8 u8DmxId, MPL_DMX_EVENT enEvent, MS_U32 u32Param);
typedef MS_BOOL (*MPL_DMX_CheckCb)(MS_U8 u8DmxId, MS_U8* start, MS_U8* end, MS_U8* read, MS_U8* write);

typedef struct
{
    MS_PHYADDR                  SectBufAddr;        ///< DMX section buffer in physical address
    MS_U32                      SectBufSize;        ///< DMX section buffer size
    MPL_DMX_SECT_MODE           SectMode;           ///< DMX section mode
    MPL_DMX_EVENT               Event;              ///< DMX section event
    MPL_DMX_NotifyCb            pNotify;            ///< DMX section callback
    MPL_DMX_Type2NotifyCb       pNotify2;
    MS_U32                      u32NotifyParam;     ///< DMX section callback parameter
} MPL_DMX_Sect_info;

//-------------------------------------------------------------------------------------------------
/// Activate a demux filter
/// @param u8DmxId \b IN: the demux filer Id to activate
/// @return TRUE - Success
/// @return SUCCESS - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_Start(MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Deactivate a demux filter
/// @param u8DmxId \b IN: the demux filer Id to deactivate
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_Stop(MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Open a demux filter
/// @param DmxFltType \b IN: the filter information to allocate
/// @param pu8DmxId \b OUT: the available demux filer Id
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_Open(MPL_DMX_FILTER_TYPE DmxFltType, MS_U8* u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Close a demux filter
/// @param u8DmxId \b IN: the demux filer Id to free
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_Close(MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Get a demux filter status
/// @param u8DmxId \b IN: the demux filer Id to activate
/// @param pbEnable \b OUT: the demux filer enable/disable
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_IsStart(MS_U8 u8DmxId, MS_BOOL* pbEnable);

//-------------------------------------------------------------------------------------------------
/// Set the read pointer of demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @param Read \b IN: the read pointer to be set
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_SectReadSet(MS_U8 u8DmxId, MS_PHYADDR Read);

//-------------------------------------------------------------------------------------------------
/// Get STC counter
/// @param pu32Stc32 \b OUT: STC counter (bit 32)
/// @param pu32Stc \b OUT: STC counter (bit 31 to bit 0)
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_Stc_Get(MS_U32* pu32Stc32, MS_U32* pu32Stc);

//-------------------------------------------------------------------------------------------------
/// Setup and start DMX section filter
/// @param u8DmxId          \b IN
/// @param uPid                 \b IN/OUT: PID to set
/// @param pFltInfo            \b IN: Fliter information
/// @return TRUE - Success
/// @return FALSE - Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_FlowStart(MS_U8 *u8DmxId, MS_U32 uPid, MPL_DMX_FLT_INFO *pFltInfo);

//-------------------------------------------------------------------------------------------------
/// Set demux filer match patterns
/// @param u8DmxId \b IN: the target demux filer Id
/// @param pPattern \b IN: the match patterns
/// @param pMask \b IN: the bit mask for match patterns
/// @param pu8NotMask \b IN: the bit mask for negative match patterns
/// @param MatchSize \b IN: the size in bytes for match patterns.
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
/// The match pattern size is 16 bytes
///     match mask -- must set 1 to be compare (customer request)
///     not match mask -- must set 1 to compare
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_SectPatternSet(MS_U8 u8DmxId, MS_U8* pPattern, MS_U8* pMask, MS_U8 *pu8NotMask, MS_U32 MatchSize);

//-------------------------------------------------------------------------------------------------
/// Set a demux filter PID
/// @param u8DmxId \b IN: the demux filer Id to set
/// @param pu16Pid \b IN/OUT: PID to set
/// @param bSet \b IN: set/get
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_Pid(MS_U8 u8DmxId, MS_U16* pu16Pid, MS_BOOL bSet);

//-------------------------------------------------------------------------------------------------
/// Get a demux filter information
/// @param u8DmxId \b IN: the demux filer Id to activate
/// @param pDmxFltInfo \b OUT: the demux filer info
/// @param pDmxFltType \b OUT: the demux filer type
/// @param bSet \b IN: set/get
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_Info(MS_U8 u8DmxId, MPL_DMX_Sect_info* pDmxFltInfo, MPL_DMX_FILTER_TYPE* pDmxFltType, MS_BOOL bSet);

//--------------------------------------------------------------------------------------------------
/// Copy section data to assigned buffer
/// @param  u8DmxId                 \b IN: demux ID
/// @param  pu8Buf                  \b OUT: section buffer address
/// @param  u32BufSize              \b IN: section buffer size
/// @param  pu32ActualSize          \b IN: section data size
/// @param  pu32RmnSize             \b OUT: section buffer remainning data size
/// @param  pfCheckCB               \b IN: callback function to check data correct or not
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//--------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_CopyData(MS_U8 u8DmxId, MS_U8* pu8Buf, MS_U32 u32BufSize, MS_U32* pu32ActualSize, MPL_DMX_CheckCb pfCheckCB);

//-------------------------------------------------------------------------------------------------
/// Reset demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @return MPL_DMX_FILTER_STATUS_OK - Success
/// @return MPL_DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_SectReset(MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Restart demux
/// @param u8DmxId \b IN: the target demux filer Id
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_ReStart(MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Get the read pointer of demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @param pRead \b OUT: the pointer to store the obtained read pointer
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_SectReadGet(MS_U8 u8DmxId, MS_PHYADDR* pRead);

//-------------------------------------------------------------------------------------------------
/// Get the write pointer of demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @param pWrite \b OUT: the pointer to store the obtained write pointer
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_SectWriteGet(MS_U8 u8DmxId, MS_PHYADDR* pWrite);

//-------------------------------------------------------------------------------------------------
/// Get the start pointer of demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @param pStart \b OUT: the pointer to store the obtained start pointer
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_SectStartGet(MS_U8 u8DmxId, MS_PHYADDR* pStart);

//-------------------------------------------------------------------------------------------------
/// Get the end pointer of demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @param pEnd \b OUT: the pointer to store the obtained end pointer
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_SectEndGet(MS_U8 u8DmxId, MS_PHYADDR* pEnd);

//-------------------------------------------------------------------------------------------------
/// Get data length in the demux filter buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @param u32data_length\b OUT: the data length in buffer
/// @return TRUE - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_DataLenGet(MS_U8 u8DmxId,MS_U32*  u32data_length);

//-------------------------------------------------------------------------------------------------
/// Get dmx flow input status
/// @param DmxFlow                      \b IN: DMX_FLOW_PLAYBACK for playback flow
/// @param pDmxFlowInput            \b OUT: pointer to store DMX input flow value.
/// @param pbClkInv                       \b OUT: pointer to store clock phase inversion
/// @param pbExtSync                    \b OUT: pointer to store sync by external signal
/// @param pbParallel                     \b OUT: pointer to store parallel interface or serial interface
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_DMX_Get_FlowInput_Status(DMX_FLOW DmxFlow, DMX_FLOW_INPUT *pDmxFlowInput, MS_BOOL *pbClkInv, MS_BOOL *pbExtSync, MS_BOOL *pbParallel);

#ifdef __cplusplus
}
#endif //__cplusplus
#endif //__mw_porting_dmx__
