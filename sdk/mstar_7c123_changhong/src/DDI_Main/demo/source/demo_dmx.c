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
#if (DEMO_DMX_TEST == 1)


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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   appDemo_DMX.c
/// @author MStar Semiconductor Inc.
/// @brief  DMX Function Sample Demo
///
/// @name DMX Function Sample Command Usage
/// @verbatim
/// ----------------------------------------------------------------------------
/// Note: DigiTuner_SetFreq must be called to set tuner/demod before any operations
///
/// 1. dmx_pat
/// Usage:dmx_pat
///
/// Sample UART Command:dmx_pat
///
/// ---------------------------------------------------------------------
///
/// 2. dmx_pmt [pid]
///    Implement with simple abstract dmx_function :
///                                   appDemo_Dmx_setup/appDemo_Dmx_start/appDemo_Dmx_close
/// Usage:dmx_pmt
///
/// Sample UART Command:dmx_pmt [Pid]
///
/// ---------------------------------------------------------------------
/// 3. dmx_sec
/// Usage:demux_sec Pid Pattern0 Pattern1 Mask0 Mask1
///     Pid: Pid filter in hex
///     Pattern0: match pattern from byte 0~3 in hex
///     Pattern1: match pattern from byte 4~6 in hex
///     mask0: mask from byte 0~3 in hex
///     mask1: mask from byte 4~7 in hex
///     *Note: our TSP support 16 bytes patterns and mask.
///            In this demo program, however, 8 bytes are only used for demo correctness
///
/// Sample UART Command:dmx_sec 0x0 0x00000000 0x00000000 0xFF000000 0x00000000
///                     dmx_sec [PmtPid] 0x02000000 0x00000000 0xFF000000 0x00000000
/// @endverbatim
///
///=================================================================================================
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsMemory.h"
#include "drvDSCMB.h"
#include "drvDTC.h"
#include "drvSYS.h"

#include "apiDMX.h"
#include "apiVDEC_EX.h"
#include "asmCPU.h"
#include "MsOS.h"
#include <sys/stat.h>
#include "MsFS.h"
#include <string.h>
#include "demo_dmx.h"
#include "demo_audio_common.h"
#include "demo_av.h"
#include "demo_vdec.h"
#include "demo_utility.h"

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static MS_S32                       _s32DataReadyEventId[2] = {-1 , -1 };

#if (DEMO_DMX_NEW_ARCHI_TEST == 1)
static MS_U8                        u8PCRFltStartIdx;
#endif

static MS_U8                        Data[1024*8];

//-------------------------------------------------------------------------------------------------
// Global Defines
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// Constant
//--------------------------------------------------------------------------------------------------
#define DMX_DBG             0
#define INVALID_FILTER_ID   0xFF
#define INVALID_PID_ID      0x1FFF
#define DATA_WAIT_TIME      5000

#define EASY_CMD            1

//--------------------------------------------------------------------------------------------------
// Enumerate
//--------------------------------------------------------------------------------------------------
typedef enum
{
    E_SI_STREAM_MPEG1_VID           = 0x01,
    E_SI_STREAM_MPEG2_VID           = 0x02,
    E_SI_STREAM_MPEG1_AUD           = 0x03,
    E_SI_STREAM_MPEG2_AUD           = 0x04,
    E_SI_STREAM_PRIVATE_DATA        = 0x06,
    E_SI_STREAM_DSMCC_DATA_TYPE_A   = 0x0A,
    E_SI_STREAM_DSMCC_DATA_TYPE_B   = 0x0B,
    E_SI_STREAM_DSMCC_DATA_TYPE_C   = 0x0C,
    E_SI_STREAM_DSMCC_DATA_TYPE_D   = 0x0D,
    E_SI_STREAM_DSMCC_DATA_TYPE_E   = 0x0E,
    E_SI_STREAM_AAC_AUD             = 0x0F,
    E_SI_STREAM_MPEG4_AUD           = 0x11,
    E_SI_STREAM_AVCH264_VID         = 0x1B,
    E_SI_STREAM_HEVC_VID            = 0x24,
    E_SI_STREAM_OP_MEPG2_VID        = 0x80,
    E_SI_STREAM_AC3_AUD             = 0x81,
    E_SI_STREAM_AVS_VID             = 0x42,
    E_SI_STREAM_AVS_AUD             = 0x43,
    E_SI_STREAM_AVS_PRIVATE_DATA    = 0x45,
}EN_SI_StreamType;

//--------------------------------------------------------------------------------------------------
// Structure Define
//--------------------------------------------------------------------------------------------------


typedef struct
{
      MS_U8 name[64] ;
      MS_U32 Counter;
} ST_NotifyParam;

//--------------------------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------------------------
#define DemoDmx_Print(fmt, args...)          { printf("[DemoDmx][%s][%06d]    ", __FUNCTION__, __LINE__); printf(fmt, ## args); }

// Event setting macro : wait data ready event of corresponding filter, use 2 32-bit event group to manage 64 filter event passing.
#define WAIT_DATA_EVENT(DmxID,Event)   MsOS_WaitEvent(_s32DataReadyEventId[DmxID>>5] , BIT(DmxID) , &Event, E_OR_CLEAR , DATA_WAIT_TIME)

//--------------------------------------------------------------------------------------------------
// Implementation Parts
//--------------------------------------------------------------------------------------------------
static ST_PCR_INFO* stPCR;

// channel database
static stFreqPG stChDB;

MS_U8 u8TSPFirmware[] = {
    #include "fwTSP.dat"
};

static MS_U8 Stream[] =
    {
    #include "../data/vertsp.dat"
};

/*********************************************************************  **
*   Define the Buffer Size and Supported buffer number
*   Please modify this definition to fullfill user requirment.
*   SECTION_BUF_ADR is Dram reserver section for DMX section usage(defined in MsMemory128.h) : size = 0x100000
***********************************************************************/
#define DMX_DEMO_RESERVED_ADDRESS     MEM_ADR_BY_MIU(SECTION_BUF_ADR, SECTION_BUF_MEMORY_TYPE)

#define PES_BUFFER_NUM        8   // 8 * 0x10000 = 0xc0000 << modify this to fit stream requirement
#define PES_BUFFER_SIZE       0x18000 // 96K  << modify this to fit stream requirement

#define SECTION_BUFFER_NUM    64  // 64 * 0x1000 = 0x40000
#define SECTION_BUFFER_SIZE   0x1000

typedef MS_U8 PES_BUF[PES_BUFFER_SIZE];
PES_BUF*      Pes_Buf_Addr =  (PES_BUF*)(DMX_DEMO_RESERVED_ADDRESS + 0x80000) ; // PES buffer size = 0x80000
MS_U8         Pes_Buf_AllocFlag[PES_BUFFER_NUM];            // allocation flag

typedef MS_U8 SEC_BUF[SECTION_BUFFER_SIZE];
SEC_BUF*      Section_Buf_Addr = (SEC_BUF*)DMX_DEMO_RESERVED_ADDRESS;

//==============================================================
//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
static void* _Demo_DMX_MemAlloc_Func(MS_U32 u32Size);
static MS_BOOL _Demo_DMX_MemFree_Func(void *pBuf);

static MS_U8 _Demo_DMX_toupper(MS_U8 input)
{
    if( input>='a' && input<='z')
    {
        return (input = input +'A' - 'a');
    }
    else
    {
        return input;
    }
}

static int _Demo_DMX_strcmp(const MS_U8* string1, const char* string2)
{
    int iRet, i;

    i = 0;
    while(string1[i] || string2[i])
    {
#if (EASY_CMD)
        iRet = _Demo_DMX_toupper(string1[i]) - _Demo_DMX_toupper(string2[i]);
#else
        iRet = string1[i] - string2[i];
#endif
        if(iRet)
        {
            return iRet;
        }
        i++;
    }
    return 0;
}

static DMX_FLOW _Demo_DMX_FlowMapping(EN_DEMO_DMX_FLOW eDmxFlow)
{
    switch ( eDmxFlow )
    {
        case E_DMX_FLOW_LIVE0:
#if (DEMO_DMX_PATH_SET_TEST == 1)
            return DMX_FLOW_PVR;
#else
            return DMX_FLOW_PLAYBACK;
#endif

        case E_DMX_FLOW_FILE0:
            return DMX_FLOW_PLAYBACK;

        case E_DMX_FLOW_LIVE1:
        case E_DMX_FLOW_FILE1:
            return DMX_FLOW_PLAYBACK1;

        case E_DMX_FLOW_LIVE2:
        case E_DMX_FLOW_FILE2:
            return DMX_FLOW_PLAYBACK2;

        case E_DMX_FLOW_LIVE3:
        case E_DMX_FLOW_FILE3:
            return DMX_FLOW_PLAYBACK3;

        default:
            DemoDmx_Print("Unsopprot DMX Flow %d!\n",eDmxFlow);
            return E_DMX_FLOW_INVALID;
    }
}

static DMX_FILTER_TYPE _Demo_DMX_FltTypeMapping(EN_DEMO_DMX_FLT_TYPE eFltType)
{
    switch ( eFltType )
    {
        case E_DMX_FLT_TYPE_VID0:
            return DMX_FILTER_TYPE_VIDEO;

        case E_DMX_FLT_TYPE_VID1:
            return DMX_FILTER_TYPE_VIDEO3D;

        case E_DMX_FLT_TYPE_AUD0:
            return DMX_FILTER_TYPE_AUDIO;

        case E_DMX_FLT_TYPE_AUD1:
            return DMX_FILTER_TYPE_AUDIO2;

        case E_DMX_FLT_TYPE_AUD2:
            return DMX_FILTER_TYPE_AUDIO3;

        case E_DMX_FLT_TYPE_AUD3:
            return DMX_FILTER_TYPE_AUDIO4;

        case E_DMX_FLT_TYPE_REC:
            return DMX_FILTER_TYPE_REC;

        default:
            DemoDmx_Print("Unsopprot DMX FltType %d!\n",eFltType);
            return E_DMX_FLT_TYPE_INVALID;
    }
}

static DMX_FLOW_INPUT _Demo_DMX_FlowInputMapping(EN_DEMO_DMX_FLOW_INPUT eDmxFlowInput)
{
    switch ( eDmxFlowInput )
    {
        case E_DMX_FLOW_INPUT_DEMOD0:
            return DMX_FLOW_INPUT_DEMOD;

        case E_DMX_FLOW_INPUT_DEMOD1:
            return DMX_FLOW_INPUT_DEMOD1;

        case E_DMX_FLOW_INPUT_MEM:
            return DMX_FLOW_INPUT_MEM;

        case E_DMX_FLOW_INPUT_EXT_INPUT0:
            return DMX_FLOW_INPUT_EXT_INPUT0;

        case E_DMX_FLOW_INPUT_EXT_INPUT1:
            return DMX_FLOW_INPUT_EXT_INPUT1;

        case E_DMX_FLOW_INPUT_EXT_INPUT2:
            return DMX_FLOW_INPUT_EXT_INPUT2;

        case E_DMX_FLOW_INPUT_EXT_INPUT3:
            return DMX_FLOW_INPUT_EXT_INPUT3;

        //case E_DMX_FLOW_INPUT_EXT_INPUT4:
        //    return DMX_FLOW_INPUT_EXT_INPUT4;

        //case E_DMX_FLOW_INPUT_EXT_INPUT5:
        //    return DMX_FLOW_INPUT_EXT_INPUT5;

#if (DEMO_DMX_3WIRE_TEST == 1)
        case E_DMX_FLOW_INPUT_EXT_INPUT0_3WIRE:
            return DMX_FLOW_INPUT_EXT_INPUT0_3WIRE;

        case E_DMX_FLOW_INPUT_EXT_INPUT1_3WIRE:
            return DMX_FLOW_INPUT_EXT_INPUT1_3WIRE;

        case E_DMX_FLOW_INPUT_EXT_INPUT2_3WIRE:
            return DMX_FLOW_INPUT_EXT_INPUT2_3WIRE;

        case E_DMX_FLOW_INPUT_EXT_INPUT3_3WIRE:
            return DMX_FLOW_INPUT_EXT_INPUT3_3WIRE;
#endif
        default:
            DemoDmx_Print("Unsopprot DMX FlowInput %d!\n",eDmxFlowInput);
            return E_DMX_FLOW_INPUT_INVALID;
    }
}

static DMX_FILTER_TYPE _Demo_DMX_FltSourceMapping(EN_DEMO_DMX_FLT_SOURCE eFltSource)
{
    switch ( eFltSource )
    {
        case E_DMX_FLT_SOURCE_LIVE0:
            return DMX_FILTER_SOURCE_TYPE_LIVE;

        case E_DMX_FLT_SOURCE_LIVE1:
            return DMX_FILTER_SOURCE_TYPE_TS1;

        case E_DMX_FLT_SOURCE_LIVE2:
            return DMX_FILTER_SOURCE_TYPE_TS2;

        case E_DMX_FLT_SOURCE_LIVE3:
            return DMX_FILTER_SOURCE_TYPE_TS3;

        case E_DMX_FLT_SOURCE_FILE0:
            return DMX_FILTER_SOURCE_TYPE_FILE;

        case E_DMX_FLT_SOURCE_FILE1:
            return DMX_FILTER_SOURCE_TYPE_FILE1;

        case E_DMX_FLT_SOURCE_FILE2:
            return DMX_FILTER_SOURCE_TYPE_FILE2;

        case E_DMX_FLT_SOURCE_FILE3:
            return DMX_FILTER_SOURCE_TYPE_FILE3;

        default:
            DemoDmx_Print("Unsopprot DMX FltSource %d!\n",eFltSource);
            return E_DMX_FLT_SOURCE_INVALID;
    }
}

EN_DEMO_DMX_FLT_SOURCE Demo_DMX_FlowToFltSrcMapping(EN_DEMO_DMX_FLOW eDmxFlow)
{
    switch(eDmxFlow)
    {
        case E_DMX_FLOW_LIVE0:
           return E_DMX_FLT_SOURCE_LIVE0;

        case E_DMX_FLOW_LIVE1:
           return E_DMX_FLT_SOURCE_LIVE1;

        case E_DMX_FLOW_LIVE2:
           return E_DMX_FLT_SOURCE_LIVE2;

        case E_DMX_FLOW_LIVE3:
           return E_DMX_FLT_SOURCE_LIVE3;

        case E_DMX_FLOW_FILE0:
           return E_DMX_FLT_SOURCE_FILE0;

        case E_DMX_FLOW_FILE1:
           return E_DMX_FLT_SOURCE_FILE1;

        case E_DMX_FLOW_FILE2:
           return E_DMX_FLT_SOURCE_FILE2;

        case E_DMX_FLOW_FILE3:
           return E_DMX_FLT_SOURCE_FILE3;

        default:
           DemoDmx_Print("Wrong DMX FLOW : %d!\n", eDmxFlow);
           return E_DMX_FLOW_INVALID;
    }

}

MS_U32 Demo_DMX_FlowToFQEngMapping(EN_DEMO_DMX_FLOW eDmxFlow)
{
    switch(eDmxFlow)
    {
        case E_DMX_FLOW_LIVE0:
           return 0;

        case E_DMX_FLOW_LIVE1:
           return 1;

        case E_DMX_FLOW_LIVE2:
           return 2;

        case E_DMX_FLOW_LIVE3:
           return 3;

        case E_DMX_FLOW_FILE0:
        case E_DMX_FLOW_FILE1:
        case E_DMX_FLOW_FILE2:
        case E_DMX_FLOW_FILE3:
           DemoDmx_Print("FileIn not Support FQ!\n");
           return 0;

        default:
           DemoDmx_Print("Wrong DMX FLOW : %d!\n", eDmxFlow);
           return E_DMX_FLOW_INVALID;
    }

}



static DMX_TSIF _Demo_DMX_PVRSrcMapping(EN_DEMO_DMX_PVR_SOURCE ePvrSrc)
{
    switch ( ePvrSrc )
    {
        case E_DMX_PVR_SOURCE_LIVE0:
            return DMX_TSIF_LIVE0;

        case E_DMX_PVR_SOURCE_LIVE1:
            return DMX_TSIF_LIVE1;

        case E_DMX_PVR_SOURCE_LIVE2:
            return DMX_TSIF_LIVE2;

        case E_DMX_PVR_SOURCE_LIVE3:
            return DMX_TSIF_LIVE3;

        case E_DMX_PVR_SOURCE_FILE0:
            return DMX_TSIF_FILE0;

        case E_DMX_PVR_SOURCE_FILE1:
            return DMX_TSIF_FILE1;

        case E_DMX_PVR_SOURCE_FILE2:
            return DMX_TSIF_FILE2;

        case E_DMX_PVR_SOURCE_FILE3:
            return DMX_TSIF_FILE3;

        default:
            DemoDmx_Print("Unsopprot DMX PvrSrc %d!\n",ePvrSrc);
            return E_DMX_PVR_SOURCE_INVALID;
    }
}

static DMX_PVR_ENG _Demo_DMX_PVREngMapping(EN_DEMO_DMX_PVR_ENG ePvrEng)
{
   switch ( ePvrEng )
   {
       case E_DMX_PVR_EGN0:
           return DMX_PVR_EGN0;

       case E_DMX_PVR_EGN1:
           return DMX_PVR_EGN1;

       case E_DMX_PVR_EGN2:
           return DMX_PVR_EGN2;

       case E_DMX_PVR_EGN3:
           return DMX_PVR_EGN3;

       case E_DMX_PVR_EGN4:
           return DMX_PVR_EGN4;

       case E_DMX_PVR_EGN5:
           return DMX_PVR_EGN5;

       default:
           DemoDmx_Print("Unsopprot DMX PvrEng %d!\n",ePvrEng);
           return E_DMX_PVR_ENG_INVALID;
   }
}

static EN_DEMO_DMX_FLOW _Demo_DMX_CMD_Mapping( MS_U8* pu8DmxFlow)
{
    if(_Demo_DMX_strcmp(pu8DmxFlow, "LIVE0")==0 )
        return E_DMX_FLOW_LIVE0;
    else if(_Demo_DMX_strcmp(pu8DmxFlow, "LIVE1")==0 )
        return E_DMX_FLOW_LIVE1;
    else if(_Demo_DMX_strcmp(pu8DmxFlow, "LIVE2")==0 )
        return E_DMX_FLOW_LIVE2;
    else if(_Demo_DMX_strcmp(pu8DmxFlow, "LIVE3")==0 )
        return E_DMX_FLOW_LIVE3;
    else if(_Demo_DMX_strcmp(pu8DmxFlow, "FILE0")==0 )
        return E_DMX_FLOW_FILE0;
    else if(_Demo_DMX_strcmp(pu8DmxFlow, "FILE1")==0 )
        return E_DMX_FLOW_FILE1;
    else if(_Demo_DMX_strcmp(pu8DmxFlow, "FILE2")==0 )
        return E_DMX_FLOW_FILE2;
    else if(_Demo_DMX_strcmp(pu8DmxFlow, "FILE3")==0 )
        return E_DMX_FLOW_FILE3;
    else
    {
        return E_DMX_FLOW_INVALID;
    }
}

// EN_DEMO_DMX_FLT_TYPE -> DMX_MMFI_FLTTYPE
DMX_MMFI_FLTTYPE Demo_DMX_MMFI_FltTypeMapping(EN_DEMO_DMX_FLT_TYPE eFltType)
{
    switch ( eFltType )
    {
        case E_DMX_FLT_TYPE_VID0:
            return DMX_MMFI_FLTTYPE_VD;

        case E_DMX_FLT_TYPE_VID1:
            return DMX_MMFI_FLTTYPE_VD3D;

        case E_DMX_FLT_TYPE_AUD0:
            return DMX_MMFI_FLTTYPE_AUD;

        case E_DMX_FLT_TYPE_AUD1:
            return DMX_MMFI_FLTTYPE_AUDB;

        case E_DMX_FLT_TYPE_AUD2:
            return DMX_MMFI_FLTTYPE_AUDC;

        case E_DMX_FLT_TYPE_AUD3:
            return DMX_MMFI_FLTTYPE_AUDD;

        default:
            DemoDmx_Print("Unsopprot DMX FltType %d!\n",eFltType);
            return DMX_MMFI_FLTTYPE_VD;
    }
}

// DMX_MMFI_FLTTYPE -> DMX_FILTER_TYPE
static DMX_FILTER_TYPE _Demo_DMX_MMFI_FltTypeInvertMapping(DMX_MMFI_FLTTYPE eFltType)
{
    switch ( eFltType )
    {
        case DMX_MMFI_FLTTYPE_VD:
            return DMX_FILTER_TYPE_VIDEO;

        case DMX_MMFI_FLTTYPE_VD3D:
            return DMX_FILTER_TYPE_VIDEO3D;

        case DMX_MMFI_FLTTYPE_AUD:
            return DMX_FILTER_TYPE_AUDIO;

        case DMX_MMFI_FLTTYPE_AUDB:
            return DMX_FILTER_TYPE_AUDIO2;

        case DMX_MMFI_FLTTYPE_AUDC:
            return DMX_FILTER_TYPE_AUDIO3;

        case DMX_MMFI_FLTTYPE_AUDD:
            return DMX_FILTER_TYPE_AUDIO4;

        default:
            DemoDmx_Print("Unsopprot DMX MMFI FltType %d!\n",eFltType);
            return DMX_FILTER_TYPE_VIDEO;
    }
}

// Sample Code for Static PES buffer Allocatation
static MS_BOOL Pes_Buf_Alloc(MS_U32 * idx)
{
    MS_U32 i = 0 ;
    *idx = PES_BUFFER_NUM ;
    for( i = 0 ; i < PES_BUFFER_NUM ; i ++)
    {
        if (Pes_Buf_AllocFlag[i] == 0 )
           break ;
    }

    if (i == PES_BUFFER_NUM ) return FALSE ;

    if (i <PES_BUFFER_NUM )
    {
        Pes_Buf_AllocFlag[i] = 1 ;
        *idx = i ;
        return TRUE ;
    }

    return FALSE ;
}

// Sample Code for Static PES buffer Free
static MS_BOOL Pes_Buf_Free(MS_U32 idx)
{
    if (idx == PES_BUFFER_NUM ) return FALSE ;

    if (idx<PES_BUFFER_NUM )
    {
        Pes_Buf_AllocFlag[idx] = 0 ;
        return TRUE ;
    }
    return FALSE ;
}

static void _Demo_DMX_PES_Parse(MS_U8* pu8Buf, MS_U32 BufSize)
{
    MS_U16 u16SectionLength;
    //MS_U8  uSectionNumber, uLastSectionNumber;  // todo: multiple section
    //MS_U8  *pu8SecIter;
    MS_U32 u32Pktprefix;
    //MS_U8  u8PatVerNum;
    //MS_U16 u16ActualTSid;
    MS_U8* pu8Section= pu8Buf;
    MS_U8  u8Sid;
    //MS_U16 u16Pid;

    // MApi_DMX_Init();

    u32Pktprefix=               (pu8Section[0]<<16)|(pu8Section[1]<<8)|pu8Section[2];
    u8Sid=                      pu8Section[3];
    u16SectionLength=           (pu8Section[4]<<8)|pu8Section[5];

    // Make sure multi section number doesn't exceed we can support.
    //uLastSectionNumber %= MAX_PAT_MULTI_SECTION_NUM;

    printf("================== PES information =====================\n");
    printf("packet start code prefix=   0x%06"DTC_MS_U32_x"\n", u32Pktprefix);
    printf("stream id=                  0x%02x\n", u8Sid);
    printf("packet length=              %d\n", u16SectionLength);
}



// Data callback function for event setting.
// Callback function inform user thread the section status.
// If in Polling mode, user thread need to polling filter status by themselves.
static void _Demo_DMX_DataCb(MS_U8 u8FltId, DMX_EVENT eFilterStatus)
{
    if(eFilterStatus == DMX_EVENT_DATA_READY)
    {
        MsOS_SetEvent(_s32DataReadyEventId[u8FltId    >>5 ], BIT(u8FltId));
    }
    else if(eFilterStatus == DMX_EVENT_BUF_OVERFLOW)
    {
        MApi_DMX_Stop(u8FltId);
        MApi_DMX_Start(u8FltId);
    }
}

static void _Demo_DMX_DataCb_Type2(MS_U8 u8DmxId, DMX_EVENT enFilterStatus,MS_U32 Type2NotifyParam1)
{


    if(enFilterStatus == DMX_EVENT_DATA_READY)
    {
        printf("======= DATA READY =======\n");
        ((ST_NotifyParam*)Type2NotifyParam1)->Counter ++ ;
        MsOS_SetEvent(_s32DataReadyEventId[u8DmxId    >>5 ], BIT(u8DmxId));
        printf("%s\n",((ST_NotifyParam*)Type2NotifyParam1)->name );
        printf("type2notify = %"DTC_MS_U32_d"\n", ((ST_NotifyParam*)Type2NotifyParam1)->Counter );

    }
    else if(enFilterStatus == DMX_EVENT_BUF_OVERFLOW)
    {
        MApi_DMX_Stop(u8DmxId);
        MApi_DMX_Start(u8DmxId);
        printf("======= BUFFER OVERFLOW =======\n");
    }

}

//------------------------------------------------------------------------------
/// @brief The sample code to setup a section filter.
/// @param eDmxFlow \b IN
/// @param *u32FltId \b IN
/// @param Pid \b IN match pid
/// @param bOneShot \b IN oneshot mode/conti mode
/// @param ipattern \b IN ipattern array
/// @param imask \b IN mask array
/// @param inmask \b IN inmask array
/// @return TRUE: success / FALSE: fail
/// @sa
/// @note  Abstraction for filter setup.
///
//------------------------------------------------------------------------------

static MS_BOOL _Demo_DMX_Setup(EN_DEMO_DMX_FLOW eDmxFlow, MS_U32* u32FltId,MS_U32 u32Pid, MS_BOOL bOneShot,MS_U8* ipattern, MS_U8* imask, MS_U8* inmask)
{
    MS_U8                                  u8FltIdSect;
    MS_U16                                 u16Pid;
    MS_U8                                  pattern[16];
    MS_U8                                  mask[16];
    MS_U8                                  nmask[16];
    MS_U8*                                 pu8BufAddr;
    DMX_Flt_info                           FltInfo;
    DMX_FILTER_TYPE                        FilterType;
    DMX_FILTER_TYPE                        eFltSrcType;

    u16Pid = (MS_U16)u32Pid ;

    memcpy(pattern,   ipattern, sizeof(pattern));
    memcpy(mask   ,   imask   , sizeof(mask));
    memcpy(nmask  ,   inmask  , sizeof(nmask));

    eFltSrcType = _Demo_DMX_FltSourceMapping(Demo_DMX_FlowToFltSrcMapping(eDmxFlow));

    FilterType = DMX_FILTER_TYPE_SECTION | eFltSrcType;

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(FilterType, &u8FltIdSect))
    {
        DemoDmx_Print("Allocate Filter Fail!\n");
        goto Demo_DMX_Setup_Fail ;
    }
    pu8BufAddr = (MS_U8*)Section_Buf_Addr[u8FltIdSect] ;

    FltInfo.Info.SectInfo.SectBufAddr =     (MS_U32)pu8BufAddr;
    FltInfo.Info.SectInfo.SectBufSize =     SECTION_BUFFER_SIZE;
    FltInfo.Info.SectInfo.Event       =     DMX_EVENT_DATA_READY | DMX_EVENT_CB_SELF | DMX_EVENT_BUF_OVERFLOW;
    FltInfo.Info.SectInfo.pNotify     =     _Demo_DMX_DataCb;

    if (bOneShot)
        FltInfo.Info.SectInfo.SectMode =  DMX_SECT_MODE_ONESHOT|DMX_SECT_MODE_CRCCHK ;
    else
       FltInfo.Info.SectInfo.SectMode  =  DMX_SECT_MODE_CRCCHK ;


    if (DMX_FILTER_STATUS_OK != MApi_DMX_Info( u8FltIdSect, &FltInfo, &FilterType, TRUE))
    {
        DemoDmx_Print("MApi_DMX_Info Fail!\n");
        goto Demo_DMX_Setup_Fail ;
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid( u8FltIdSect , &u16Pid , TRUE))
    {
        DemoDmx_Print("MApi_DMX_Pid fail!\n");
        goto Demo_DMX_Setup_Fail ;
    }

    MApi_DMX_SectReset(u8FltIdSect);

    MApi_DMX_SectPatternSet(u8FltIdSect, pattern, mask, nmask, 16);

    *u32FltId = (MS_U32)u8FltIdSect ;

    return TRUE;

Demo_DMX_Setup_Fail:

    MApi_DMX_Close(u8FltIdSect);
    return FALSE;

}

static st_PG* _ScanPgFind(MS_U32 u32ProgNum)
{
    MS_U32 i;
    for (i= 0; i< stChDB.u32PgNum; i++)
    {
        if (u32ProgNum == stChDB.stPgList[i].u32ProgNum)
        {
            return &(stChDB.stPgList[i]);
        }
    }
    return NULL;
}

static st_Audio_PG* _ScanAudioPgFind(st_PG* pstPG, MS_U32 u32PID)
{
    MS_U32 i;
    for (i= 0; i< MAX_AUDIO_PG; i++)
    {
        if (pstPG->Audio_PG[i].u32PidAudio == u32PID)
        {
            //pid exist, return pg
            return &pstPG->Audio_PG[i];
        }
        else if (pstPG->Audio_PG[i].u32PidAudio == 0)
        {
            //not found, return empty pg
            return &pstPG->Audio_PG[i];
        }
    }
    //not found and no empty pg
    return NULL;
}

//------------------------------------------------------------------------------
/// @brief The sample code to parse PAT Table and save program service information
/// @param[in] u8PatId pat id
/// @sa
/// @note
//------------------------------------------------------------------------------
MS_BOOL _ScanFilterParsePAT(MS_U8 u8PatId)
{
//    MS_U8       u8PatBuf[1024];
    MS_U32      u32ActualSize;
    MS_U32      u32RmnSize;
    MS_U32      u32LoopLen;
    MS_U8*      pu8Loop;
    MS_U32      i;
    MS_U32      u32ProgNum;

    if (DMX_FILTER_STATUS_OK!= MApi_DMX_CopyData(u8PatId, Data, 1024, &u32ActualSize, &u32RmnSize, NULL))
    {
        printf("[%s][%d] error\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if (u32ActualSize == 0 ) {printf("no data\n") ; return false ;}

    u32LoopLen = ((Data[1] & 0x0f ) << 8) | Data[2];
    if (9> u32LoopLen)
    {
        printf("[%s][%d] error\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    u32LoopLen -= 5; // PAT header
    u32LoopLen -= 4; // CRC
    if (0x3 & u32LoopLen)
    {
        printf("[%s][%d] error\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    pu8Loop = Data + 8;

    stChDB.u32PgNum = 0;
    for (i = 0; i< u32LoopLen >> 2; i++)
    {
        if (MAX_PG_IN_PAT_DDI <= stChDB.u32PgNum)
        {
            break;
        }
        u32ProgNum = ((MS_U32)(((MS_U8*)(pu8Loop))[0]<<8)|(((MS_U8*)(pu8Loop))[1]));
        if (0 == u32ProgNum)
        {
            goto pat_next_loop;
        }
        stChDB.stPgList[stChDB.u32PgNum].u32ProgNum = u32ProgNum;
        stChDB.stPgList[stChDB.u32PgNum].u32PidPmt = ((MS_U32)(((MS_U8*)(pu8Loop))[2]<<8)|(((MS_U8*)(pu8Loop))[3])) & 0x1FFF;

        printf("u32PNum:%"DTC_MS_U32_x" ProgNum:%"DTC_MS_U32_x" PidPmt%"DTC_MS_U32_x"\n", stChDB.u32PgNum, stChDB.stPgList[stChDB.u32PgNum].u32ProgNum, stChDB.stPgList[stChDB.u32PgNum].u32PidPmt);
        stChDB.u32PgNum++;

pat_next_loop:
        pu8Loop+= 4;
    }
    // printf("[%s][%d] %d programs\n", __FUNCTION__, __LINE__, stChDB.u32PgNum);
    return TRUE;
}




//------------------------------------------------------------------------------
/// @brief The sample code to parse PMT Table and save program service information
/// @param[in] u8PatId pat id
/// @sa
/// @note
//------------------------------------------------------------------------------

MS_BOOL _ScanFilterParsePMT(MS_U8 u8PmtId)
{
    MS_U8       u8PmtBuf[1024];
    MS_U32      u32ActualSize;
    MS_U32      u32RmnSize;
    MS_U32      s32LoopLen;
    MS_U8*      pu8Loop;
    MS_U32      u32ProgInfoLen;
    MS_U32      u32ProgInfoCount = 0;
    st_PG*      pstPG= NULL;
    MS_U32      u32Pid;
    MS_U32      u32Pcr;
    MS_U32      u32ProgNum;
    MS_U32      u32TempLoopLen;
    MS_U16      u16DescriptorLength;
    MS_U8       *pu8Descriptor;
    MS_S16      s16TotalDescriptorLength;
    MS_U8       u8Index = 0;

    if (DMX_FILTER_STATUS_OK!= MApi_DMX_CopyData(u8PmtId, u8PmtBuf, 1024, &u32ActualSize, &u32RmnSize, NULL))
    {
        printf("[%s][%d] error\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if (u8PmtBuf[0] != 0x2 )
    {
        return FALSE ;
    }

    s32LoopLen = ((u8PmtBuf[1] & 0x0f ) << 8) | u8PmtBuf[2];
    if (12 > s32LoopLen)
    {
        return FALSE;
    }
    u32ProgInfoLen = ((u8PmtBuf[10] & 0x0f ) << 8) | u8PmtBuf[11];
    if ((13 + u32ProgInfoLen)> s32LoopLen)
    {
        return FALSE;
    }
    s32LoopLen -= 9; // PMT header
    s32LoopLen -= 4; // CRC

    u32Pcr = ((MS_U32)(((MS_U8*)(u8PmtBuf))[8]<<8)|(((MS_U8*)(u8PmtBuf))[9])) & 0x1FFF;
    u32ProgNum = (u8PmtBuf[3] << 8) | u8PmtBuf[4];

    pu8Loop = u8PmtBuf + 12 + u32ProgInfoLen;


    if (NULL == (pstPG= _ScanPgFind(u32ProgNum)))
    {
        return FALSE;
    }

    pstPG->u32PidPcr = u32Pcr;
    pstPG->eVideoFmt = E_SI_VID_INVALID ;

    for (u8Index = 0;u8Index<MAX_AUDIO_PG;u8Index++)
    {
        pstPG->Audio_PG[u8Index].eAudioFmt = E_SI_AUD_INVALID;
    }

    while (u32ProgInfoCount < u32ProgInfoLen)
    {
        //descriptior, current not used

        u32ProgInfoCount++;
    }

    s32LoopLen -= u32ProgInfoLen;

    while (5 <= s32LoopLen)
    {
        st_Audio_PG* pstAudio_PG = NULL;

        u32Pid = ((MS_U32)(((MS_U8*)(pu8Loop))[1]<<8)|(((MS_U8*)(pu8Loop))[2])) & 0x1FFF;

        u32TempLoopLen = ((pu8Loop[3] & 0x0f ) << 8) | pu8Loop[4];

        switch (pu8Loop[0])
        {
        case E_SI_STREAM_AVCH264_VID:
        case E_SI_STREAM_AVS_VID:
        case E_SI_STREAM_MPEG1_VID:
        case E_SI_STREAM_MPEG2_VID:
        case E_SI_STREAM_HEVC_VID:
            printf("pstPG->u32PidVideo:%"DTC_MS_U32_x"\n", pstPG->u32PidVideo);
            if (!pstPG->u32PidVideo)
            {
                printf("update PidVideo:%"DTC_MS_U32_x"\n", u32Pid);

                pstPG->u32PidVideo = u32Pid;
                pstPG->u32StreamTypeVideo = pu8Loop[0];
                if ( pu8Loop[0] == E_SI_STREAM_AVCH264_VID )
                {
                    pstPG->eVideoFmt = E_SI_VID_H264 ;
                }
                else if(pu8Loop[0] == E_SI_STREAM_AVS_VID)
                {
                    pstPG->eVideoFmt = E_SI_VID_AVS;
                }
                else if(pu8Loop[0] == E_SI_STREAM_HEVC_VID)
                {
                    pstPG->eVideoFmt = E_SI_VID_HEVC;
                }
                else
                {
                    pstPG->eVideoFmt = E_SI_VID_MPEG ;
                }
            }

            break;
        case E_SI_STREAM_MPEG1_AUD:
            // MPEG-1 Audio
        case E_SI_STREAM_MPEG2_AUD:
            // MPEG-2 Audio
        case E_SI_STREAM_AC3_AUD:
            // 13818-7 AAC
        case E_SI_STREAM_AAC_AUD:
            // 14496-3 MPEG4
        case E_SI_STREAM_MPEG4_AUD:
        case E_SI_STREAM_PRIVATE_DATA:
        case E_SI_STREAM_AVS_PRIVATE_DATA:

            if ((pu8Loop[0] == E_SI_STREAM_MPEG1_AUD) || (pu8Loop[0] == E_SI_STREAM_MPEG2_AUD)
                || (pu8Loop[0] == E_SI_STREAM_AC3_AUD) || (pu8Loop[0] == E_SI_STREAM_AAC_AUD)
                || (pu8Loop[0] == E_SI_STREAM_MPEG4_AUD))
            {
                pstAudio_PG = _ScanAudioPgFind(pstPG, u32Pid);

                if(pstAudio_PG == NULL)
                {
                    printf("[%s][%d] Warning!!! No empty audio program, for PidAudio:%"DTC_MS_U32_x"\n", __FUNCTION__, __LINE__, u32Pid);
                    return FALSE;
                }

                printf("pstPG->u32PidAudio:%"DTC_MS_U32_x"\n", pstAudio_PG->u32PidAudio);

                if(!pstAudio_PG->u32PidAudio)
                {
                    printf("update PidAudio:%"DTC_MS_U32_x"\n", u32Pid);

                    pstAudio_PG->u32PidAudio = u32Pid;
                    pstAudio_PG->u32StreamTypeAudio = pu8Loop[0];
                    if ( pu8Loop[0] == E_SI_STREAM_AC3_AUD )
                    {
                        pstAudio_PG->eAudioFmt = E_SI_AUD_AC3 ;
                    }
                    else if(pu8Loop[0] == E_SI_STREAM_MPEG4_AUD)
                    {
                        pstAudio_PG->eAudioFmt = E_SI_AUD_MPEG4;
                    }
                    else if(pu8Loop[0] == E_SI_STREAM_AAC_AUD)
                    {
                        pstAudio_PG->eAudioFmt = E_SI_AUD_AAC;
                    }
                    else
                    {
                        pstAudio_PG->eAudioFmt = E_SI_AUD_MPEG ;
                    }
                }
            }

            s16TotalDescriptorLength = u32TempLoopLen;
            pu8Descriptor = pu8Loop+5;

            while(s16TotalDescriptorLength > 0)
            {
                switch (pu8Descriptor[0])
                {
                    case E_SI_DESC_TAG_ISO_639_LANG:
                        {
                            MS_U8 u8Descriptor_length = pu8Descriptor[1];
                            pstAudio_PG = _ScanAudioPgFind(pstPG, u32Pid);
                            if (pstAudio_PG == NULL)
                            {
                                printf("[%s][%d] Warning!!! No empty audio program, for PidAudio:%"DTC_MS_U32_x"\n", __FUNCTION__, __LINE__, u32Pid);
                                return FALSE;
                            }

                            pstAudio_PG->bISO_Lang_exist = TRUE;

                            pstAudio_PG->u32PidAudio = u32Pid;

                            for (u8Index=0;u8Index<u8Descriptor_length;u8Index++)
                            {
                                memcpy(&pstAudio_PG->u8Lang, &pu8Descriptor[2+u8Index], 3);
                                u8Index += 3;
                                pstAudio_PG->u8Lang_audio_type = pu8Descriptor[2+u8Index];
                                u8Index += 1;
                            }
                        }
                        break;
                    case E_SI_DESC_TAG_STREAM_ID:
                        break;
                    case E_SI_DESC_TAG_AC3:
                    case E_SI_DESC_TAG_E_AC3:
                        pstAudio_PG = _ScanAudioPgFind(pstPG, u32Pid);
                        if (pstAudio_PG == NULL)
                        {
                            printf("[%s][%d] Warning!!! No empty audio program, for PidAudio:%"DTC_MS_U32_x"\n", __FUNCTION__, __LINE__, u32Pid);
                            return FALSE;
                        }
                        printf("update PidAudio:%"DTC_MS_U32_x"\n", u32Pid);
                        pstAudio_PG->u32PidAudio = u32Pid;
                        pstAudio_PG->u32StreamTypeAudio = pu8Loop[0];
                        if (pu8Descriptor[0] == E_SI_DESC_TAG_AC3)
                        {
                            pstAudio_PG->eAudioFmt = E_SI_AUD_AC3 ;
                        }
                        else if (pu8Descriptor[0] == E_SI_DESC_TAG_E_AC3)
                        {
                            pstAudio_PG->eAudioFmt = E_SI_AUD_AC3P ;
                        }
                        break;
                    case E_SI_DESC_TAG_EXTENSION:
                        switch(pu8Descriptor[2])
                        {
                            //supplementary audio descriptor
                            case 0x06:
                                {
                                    pstAudio_PG = _ScanAudioPgFind(pstPG, u32Pid);
                                    if (pstAudio_PG == NULL)
                                    {
                                        printf("[%s][%d] Warning!!! No empty audio program, for PidAudio:%"DTC_MS_U32_x"\n", __FUNCTION__, __LINE__, u32Pid);
                                        return FALSE;
                                    }
                                    printf("update PidAudio:%"DTC_MS_U32_x"\n", u32Pid);
                                    pstAudio_PG->u32PidAudio = u32Pid;
                                    pstAudio_PG->u32StreamTypeAudio = pu8Loop[0];

                                    pstAudio_PG->bSupplematary_Audio_exist = TRUE;
                                    MS_U8 u8Lang_present = (pu8Descriptor[3] & 0x01);
                                    pstAudio_PG->u8Supplementary_mix_type = (pu8Descriptor[3] & 0x80);
                                    pstAudio_PG->u8Supplementary_classification = (pu8Descriptor[3] & 0x7C) >> 2;
                                    if(u8Lang_present)
                                    {
                                        memcpy(pstAudio_PG->u8Supplementary_Lang, &pu8Descriptor[4], 3);
                                    }
                                }
                                break;
                            default:
                                break;
                        }

                        break;
                    default:
                        break;
                }
                /* move to next descriptor */
                u16DescriptorLength = pu8Descriptor[1] + 2;
                s16TotalDescriptorLength -= u16DescriptorLength;
                pu8Descriptor += u16DescriptorLength;
            }
            break;
        default:
            break;
        }
        pu8Loop += (5+ u32TempLoopLen);
        s32LoopLen -= (5+ u32TempLoopLen);
    }
    return TRUE;
}


static void data_dump(MS_U8* pu8Buf, MS_U32 BufSize)
{
    MS_U32 i ;

    for( i = 0 ; i < BufSize ; i ++ ){
        if((i%16) == 0 ) printf("\n");
        printf("%02X ",*(pu8Buf + i));
    }
}


static EN_VDEC_DDI_CodecType _VideoCodecType_Mapping(EN_SI_VideoFormat eVideoFmt)
{
    switch ( eVideoFmt )
    {
        case E_SI_VID_MPEG :
            return E_VDEC_DDI_CODEC_TYPE_MPEG2;

        case E_SI_VID_H264 :
            return E_VDEC_DDI_CODEC_TYPE_H264;
        case E_SI_VID_AVS:
            return E_VDEC_DDI_CODEC_TYPE_AVS;
        case E_SI_VID_HEVC:
            return E_VDEC_DDI_CODEC_TYPE_HEVC;
        default:
            //DemoDmx_Print("Unsopprot Video Format %ld!\n",eVideoFmt);
            return E_VDEC_DDI_CODEC_TYPE_MPEG2;
    }
}

static EN_AUDIO_CODEC_TYPE _AudioCodecType_Mapping(EN_SI_AudioFormat eAudioFmt)
{
    switch ( eAudioFmt )
    {
        case E_SI_AUD_MPEG :
        case E_SI_AUD_MPEG_AD:
            return AUDIO_CODEC_MPEG;

        case E_SI_AUD_AC3 :
        case E_SI_AUD_AC3_AD :
            return AUDIO_CODEC_AC3;

        case E_SI_AUD_AC3P :
            return AUDIO_CODEC_AC3P;

        case E_SI_AUD_MPEG4:
        case E_SI_AUD_AAC:
            return AUDIO_CODEC_AAC;

        default:
            //DemoDmx_Print("Unsopprot Audio Format %ld!\n",eAudioFmt);
            return AUDIO_CODEC_NONE ;
    }

}



static void _DumpCh(void)
{
    st_PG* pstPG = NULL;
    MS_U32 i, j;
    char VideoFmtStr[][21]={"SiNOT_VALID","SiMPEG","SiH264","SiAVS","SiHEVC"};
    char AudioFmtStr[][21]={"SiNOT_VALID","SiMPEG","SiAC3","SiMPEG_AD","SiMPEG4","SiAAC","SiAC3P","SiAC3_AD"};

    printf("[%s][%d] start =======================================\n", __FUNCTION__, __LINE__);
    for (i= 0; i< stChDB.u32PgNum; i++)
    {
        printf("[%s][%d] %d\n", __FUNCTION__, __LINE__, (int)i);
        pstPG = &(stChDB.stPgList[i]);
        printf("[%s][%d]     [VIDEO]\n", __FUNCTION__, __LINE__);
        printf("[%s][%d]         video pid   = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidVideo);
        printf("[%s][%d]         videoFmt    = %-12s       Video Codec Type = %x\n", __FUNCTION__, __LINE__, VideoFmtStr[pstPG->eVideoFmt], _VideoCodecType_Mapping(pstPG->eVideoFmt));
        printf("[%s][%d]         v type      = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32StreamTypeVideo);

        for (j= 0; j< MAX_AUDIO_PG; j++)
        {
            if(pstPG->Audio_PG[j].u32PidAudio)
            {
                printf("[%s][%d]     [AUDIO %ld]\n", __FUNCTION__, __LINE__, j+1);
                printf("[%s][%d]         audio pid   = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->Audio_PG[j].u32PidAudio);
                printf("[%s][%d]         audioFmt    = %-12s       Audio Codec Type = %x\n", __FUNCTION__, __LINE__, AudioFmtStr[pstPG->Audio_PG[j].eAudioFmt], _AudioCodecType_Mapping(pstPG->Audio_PG[j].eAudioFmt) );
                printf("[%s][%d]         a type      = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->Audio_PG[j].u32StreamTypeAudio);
                if (pstPG->Audio_PG[j].bISO_Lang_exist)
                {
                    if (strcmp((char*)pstPG->Audio_PG[j].u8Lang, "") == 0)
                    {
                        printf("[%s][%d]         lang        = N/A\n", __FUNCTION__, __LINE__);
                    }
                    else
                    {
                        printf("[%s][%d]         lang        = %s\n", __FUNCTION__, __LINE__, pstPG->Audio_PG[j].u8Lang);
                    }

                    switch (pstPG->Audio_PG[j].u8Lang_audio_type)
                    {
                        case 0x00:
                            printf("[%s][%d]         lang type   = Undefined\n", __FUNCTION__, __LINE__);
                            break;
                        case 0x01:
                            printf("[%s][%d]         lang type   = Clean effects\n", __FUNCTION__, __LINE__);
                            break;
                        case 0x02:
                            printf("[%s][%d]         lang type   = Hearing impaired\n", __FUNCTION__, __LINE__);
                            break;
                        case 0x03:
                            printf("[%s][%d]         lang type   = Visual impaired commentary\n", __FUNCTION__, __LINE__);
                            break;
                        default:
                            break;
                    }
                }

                if (pstPG->Audio_PG[j].bSupplematary_Audio_exist)
                {
                    if (strcmp((char*)pstPG->Audio_PG[j].u8Supplementary_Lang, "") == 0)
                    {
                        printf("[%s][%d]         Supplementary lang           = N/A\n", __FUNCTION__, __LINE__);
                    }
                    else
                    {
                        printf("[%s][%d]         Supplementary lang           = %s\n", __FUNCTION__, __LINE__, pstPG->Audio_PG[j].u8Supplementary_Lang);
                    }

                    if (pstPG->Audio_PG[j].u8Supplementary_mix_type)
                    {
                        printf("[%s][%d]         Supplementary mix type       = Complete and independent stream\n", __FUNCTION__, __LINE__);
                    }
                    else
                    {
                        printf("[%s][%d]         Supplementary mix type       = Supplementary stream\n", __FUNCTION__, __LINE__);
                    }

                    switch (pstPG->Audio_PG[j].u8Supplementary_classification)
                    {
                        case 0x00:
                            break;
                            printf("[%s][%d]         Supplementary classification = Main audio\n", __FUNCTION__, __LINE__);
                        case 0x01:
                            printf("[%s][%d]         Supplementary classification = Audio description for the visual impaired\n", __FUNCTION__, __LINE__);
                            break;
                        case 0x02:
                            printf("[%s][%d]         Supplementary classification = Clean audio for the hearing impaired\n", __FUNCTION__, __LINE__);
                            break;
                        case 0x03:
                            printf("[%s][%d]         Supplementary classification = Spoken subtitle for the visual impaired commentary\n", __FUNCTION__, __LINE__);
                            break;
                        default:
                            break;
                    }
                }
            }
            else
            {
                continue;
            }
        }
        printf("[%s][%d]     pcr   pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidPcr);
        printf("[%s][%d]     pmt   pid = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidPmt);
        printf("[%s][%d]     prog  num = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32ProgNum);
    }
    printf("[%s][%d] end   =======================================\n", __FUNCTION__, __LINE__);
}

static unsigned char _convert(char ch)
{
    //printf("%c--",ch);
    char out = 0 ;
    if ((ch > 47) && (ch < 58))
        out = ch - 48 ;
    if ((ch > 64) && (ch < 71))
        out = ch - 55;
    if ((ch > 96) && (ch < 103))
        out = ch - 87;
    //printf("%x\n",out);
     return out;
}

//-------------------------------------------------------------------------------------------------
// Global Functions
//-------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// API implmentation
//-------------------------------------------------------------------------------------------------

MS_BOOL Demo_DMX_CheckAudioPgExist(st_PG* pstPG, MS_U32 u32PID)
{
    MS_U32 i;
    for (i= 0; i< MAX_AUDIO_PG; i++)
    {
        if (pstPG->Audio_PG[i].u32PidAudio == u32PID)
        {
            //pid exist
            return TRUE;
        }
    }
    //not found
    return FALSE;
}
//--------------------------------------------------------------------------------------------------
/// Reset AVFIFO
/// @param  eAVFltType          \b IN: demux type
/// @param  bRst                \b IN:
/// @return TRUE  - Success
/// @return FALSE - Failure
/// @note
//--------------------------------------------------------------------------------------------------
MS_BOOL Demo_DMX_AVFifo_Reset(EN_DEMO_DMX_FLT_TYPE eAVFltType, MS_BOOL bRst)
{
    DMX_FILTER_TYPE eFltType = _Demo_DMX_FltTypeMapping(eAVFltType);

    if(DMX_FILTER_STATUS_OK != MApi_DMX_AVFifo_Reset(eFltType, bRst) )
    {
        DemoDmx_Print("Reset AV FIFO Fail !\n");
        return FALSE;
    }
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Open a demux filter and set a demux filter PID
/// @param  eFltSource \b IN: the filter information to allocate
//  @param  eFltType   \b IN: the filter information to allocate
/// @param  pu8FltId   \b OUT: the available demux filer Id
/// @param  u16Pid     \b IN: PID to set
/// @return TRUE  - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_DMX_FltOpen(EN_DEMO_DMX_FLT_SOURCE eFltSource, EN_DEMO_DMX_FLT_TYPE eFltType, MS_U8* pu8FltId, MS_U16 u16Pid)
{

    if(TRUE != Demo_DMX_Open(eFltSource, eFltType, pu8FltId))
    {
       DemoDmx_Print("[%s][%d]Open Filter Fail !\n", __FUNCTION__, __LINE__);
       return FALSE;
    }

#if 0
#if (DEMO_DMX_DSCMB_TEST == 1)

    if( bDSCMB )
    {
    // Callback Function for Demo_DSCMB
         /*
         u32DscmbId = MDrv_DSCMB2_FltAlloc(0); //allocate dscmb
         Drv_DSCMB2_FltConnectFltId(0, u32DscmbId, u8FltId); //connect to tsp
         */
    }
#endif
#endif

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(*pu8FltId, &u16Pid, TRUE))
    {
        DemoDmx_Print("Set Pid:%hu to Filter:%u Fail !\n", u16Pid, *pu8FltId);
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Open a demux filter
/// @param  eFltSource \b IN: the filter information to allocate
//  @param  eFltType   \b IN: the filter information to allocate
/// @param  pu8FltId   \b OUT: the available demux filer Id
/// @param  u16Pid     \b IN: PID to set
/// @return TRUE  - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_DMX_Open(EN_DEMO_DMX_FLT_SOURCE eFltSource, EN_DEMO_DMX_FLT_TYPE eFltType, MS_U8* pu8FltId)
{
    DMX_FILTER_TYPE eDmxFltSource = _Demo_DMX_FltSourceMapping(eFltSource);
    DMX_FILTER_TYPE eDmxFltType   = _Demo_DMX_FltTypeMapping(eFltType);

    if(DMX_FILTER_STATUS_OK != MApi_DMX_Open((DMX_FILTER_TYPE)(eDmxFltType|eDmxFltSource), pu8FltId))
    {
        DemoDmx_Print("[%s][%d]Open Filter Fail !\n", __FUNCTION__, __LINE__);
       return FALSE;
    }
    return TRUE;
}

#if 0
//-------------------------------------------------------------------------------------------------
/// Activate a demux filter
/// @param  u8FltId \b IN: the demux filer Id to activate
/// @return TRUE  - Success
/// @return FALSE - Failure
/// @note
//------------------------------------------------------------------------------------------------
MS_BOOL Demo_DMX_FltStart(MS_U8 u8FltId)
{
    if(DMX_FILTER_STATUS_OK != MApi_DMX_Start(u8FltId))
    {
        DemoDmx_Print("Start Filter %lu Fail !\n", u8FltId);
        return FALSE;
    }
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Deactivate a demux filter
/// @param  u8FltId \b IN: the demux filer Id to deactivate
/// @return TRUE  - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_DMX_FltStop(MS_U8 u8FltId)
{
    if(DMX_FILTER_STATUS_OK != MApi_DMX_Stop(u8FltId))
    {
        DemoDmx_Print("Stop Filter %lu Fail !\n", u8FltId);
        return FALSE;
    }
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Close a demux filter
/// @param  u8FltId \b IN: the demux filer Id to free
/// @return TRUE  - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_DMX_FltClose(MS_U8 u8FltId)
{
    if(DMX_FILTER_STATUS_OK != MApi_DMX_Close(u8FltId))
    {
        DemoDmx_Print("Close Filter %lu Fail !\n", u8FltId);
        return FALSE;
    }
    return TRUE;
}
#endif

//-------------------------------------------------------------------------------------------------
/// Open a demux PCR filter and set a demux PCR filter PID
/// @param  eFltSource  \b IN: the filter information to allocate
/// @param  u16Pid      \b IN: PID to set
/// @param  u8PCREngId \b OUT: the allocated demux PCR Engine Id
/// @return TRUE  - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_DMX_PCR_FltOpen(EN_DEMO_DMX_FLT_SOURCE eFltSource, MS_U16 u16Pid,  MS_U8* u8PCREngId)
{
    MS_U8  u8FltId;
    DMX_FILTER_TYPE eFltSrc = _Demo_DMX_FltSourceMapping(eFltSource);

    if(DMX_FILTER_STATUS_OK != MApi_DMX_Open((DMX_FILTER_TYPE)(DMX_FILTER_TYPE_PCR|eFltSrc), &u8FltId))
    {
       DemoDmx_Print("Open PCR Filter Fail !\n");
       return FALSE;
    }

#if 0
#if (DEMO_DMX_DSCMB_TEST == 1)

    if( bDSCMB )
    {
    // Callback Function for Demo_DSCMB
         /*
         u32DscmbId = MDrv_DSCMB2_FltAlloc(0); //allocate dscmb
         Drv_DSCMB2_FltConnectFltId(0, u32DscmbId, u8FltId); //connect to tsp
         */
    }

#endif
#endif

#if (DEMO_DMX_NEW_ARCHI_TEST == 1)
    *u8PCREngId = u8FltId - u8PCRFltStartIdx;
#else
    *u8PCREngId = 0;
#endif

    stPCR[*u8PCREngId].u8FltId = u8FltId;

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(u8FltId, &u16Pid, TRUE))
    {
        DemoDmx_Print("Set Pid:%hu to PCR Filter:%u Fail !\n", u16Pid, *u8PCREngId);
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Activate a demux PCR filter
/// @param  u8PCREngId \b IN: the demux PCR Engine Id to activate
/// @return TRUE  - Success
/// @return FALSE - Failure
/// @note
//------------------------------------------------------------------------------------------------
MS_BOOL Demo_DMX_PCR_FltStart(MS_U8 u8PCREngId)
{
    if(DMX_FILTER_STATUS_OK != MApi_DMX_Start(stPCR[u8PCREngId].u8FltId))
    {
        DemoDmx_Print("Start PCR Filter %u Fail !\n", u8PCREngId);
        return FALSE;
    }
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Deactivate a demux PCR filter
/// @param  u8PCREngId \b IN: the demux PCR Engine Id to deactivate
/// @return TRUE  - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_DMX_PCR_FltStop(MS_U8 u8PCREngId)
{
    if(DMX_FILTER_STATUS_OK != MApi_DMX_Stop(stPCR[u8PCREngId].u8FltId))
    {
        DemoDmx_Print("Stop PCR Filter %u Fail !\n", u8PCREngId);
        return FALSE;
    }
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Close a demux PCR filter
/// @param  u8PCREngId \b IN: the demux PCR Engine Id to free
/// @return TRUE  - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_DMX_PCR_FltClose(MS_U8 u8PCREngId)
{
    if(DMX_FILTER_STATUS_OK != MApi_DMX_Close(stPCR[u8PCREngId].u8FltId))
    {
        DemoDmx_Print("Stop PCR Filter %u Fail !\n", u8PCREngId);
        return FALSE;
    }
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Set Demux Flow
/// @param eDmxFlow         \b IN: EN_DEMO_DMX_FLOW
/// @param eDmxFlowInput \b IN: EN_DEMO_DMX_FLOW_INPUT
/// @param bClkInv            \b IN: TSin options: clock phase inversion
/// @param bExtSync          \b IN: TSin options: sync by external signal
/// @param bParallel           \b IN: TSin is parallel interface or serial interface
/// @return TRUE  - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_DMX_FlowSet(EN_DEMO_DMX_FLOW eDmxFlow, EN_DEMO_DMX_FLOW_INPUT eDmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel)
{
    DMX_FLOW       eFlow      = _Demo_DMX_FlowMapping(eDmxFlow);
    DMX_FLOW_INPUT eFlowInput = _Demo_DMX_FlowInputMapping(eDmxFlowInput) ;


#if (DEMO_DMX_PATH_SET_TEST == 1)
    if (eFlow == DMX_FLOW_PVR)
    {
        if(DMX_FILTER_STATUS_OK != MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, eFlowInput, bClkInv, bExtSync, bParallel))
        {
            DemoDmx_Print("DMX FlowSet Fail !\n");
            return FALSE;
        }
    }
#endif

    if(DMX_FILTER_STATUS_OK != MApi_DMX_FlowSet(eFlow, eFlowInput, bClkInv, bExtSync, bParallel))
    {
        DemoDmx_Print("DMX FlowSet Fail !\n");
        return FALSE;
    }
    return TRUE;
}

//--------------------------------------------------------------------------------------------------
/// Setting PVR flow
/// @param  ePvrEng                     \b IN: PVR Engine ID
/// @param  ePvrEng                     \b IN: Select the record source
/// @param  bDscmb                      \b IN: TRUE for recording dscmbled stream; FALSE for recording orignal stream
/// @return TRUE  - Success
/// @return FALSE - Failure
/// @note
//--------------------------------------------------------------------------------------------------
MS_BOOL Demo_DMX_PVR_FlowSet(EN_DEMO_DMX_PVR_ENG ePvrEng, EN_DEMO_DMX_PVR_SOURCE ePvrSrc, MS_BOOL bDscmb)
{
#if (DEMO_DMX_NEW_ARCHI_TEST == 1)

    DMX_PVR_ENG eDmxPvrEng = _Demo_DMX_PVREngMapping(ePvrEng);
    DMX_TSIF    eDmxTSIF   = _Demo_DMX_PVRSrcMapping(ePvrSrc);

    if(DMX_FILTER_STATUS_OK != MApi_DMX_PVR_FlowSet( eDmxPvrEng, eDmxTSIF, bDscmb))
    {
        DemoDmx_Print("DMX PVR FlowSet Fail !\n");
        return FALSE;
    }
#endif
    return TRUE;
}

MS_BOOL Demo_DMX_GetProgramInfo(st_PG** ppstPG, MS_U32* pu32PgNum)
{
    *ppstPG = &stChDB.stPgList[0];
    *pu32PgNum = stChDB.u32PgNum;

    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Hardware general config for driver interface
/// @return TRUE  - Success
/// @return FALSE - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_DMX_PreInit( void )
{
   #if (DEMO_DMX_PATH_SET_TEST == 1)

   if(DMX_FILTER_STATUS_OK != MApi_DMX_CMD_Run(0xF0010001 , 1, 0 , NULL))
   {
        DemoDmx_Print("DMX CMD Run Fail !\n");
        return FALSE;
   }

   #endif

   return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Initialize Demux API
/// @return TRUE  - Success
/// @return FALSE - Failure
/// @note
/// It should be called before calling any other Demo Demux API functions.
//-------------------------------------------------------------------------------------------------
#if (DEMO_DMX_TV_SERIES_TEST)
MS_BOOL Demo_DMX_Init(void)
{
    DMX_FLOW_INPUT inSrc0,inSrc1;
    MS_U32 u32num;
    MS_U32 u32Idx;

    printf("Demo_DMX_Init\n");

    MApi_DMX_SetHK(TRUE);
    {   // set VQ for TSP, set FW/VQ for TSP2
        DMX_TSPParam    param;

        memset(&param,0,sizeof(param));
        memcpy((void*)MS_PA2KSEG1((MS_U32)MEM_ADR_BY_MIU(TSP_FW_BUF_ADR, TSP_FW_BUF_MEMORY_TYPE)/*u8TSPFirmware*/) , u8TSPFirmware, sizeof(u8TSPFirmware));
        MApi_DMX_SetFW(((MS_PHYADDR)MEM_ADR_BY_MIU(TSP_FW_BUF_ADR, TSP_FW_BUF_MEMORY_TYPE)/*u8TSPFirmware*/), sizeof(u8TSPFirmware));

        param.phyFWAddr = (MS_PHYADDR) MEM_ADR_BY_MIU(TSP_FW_BUF_ADR, TSP_FW_BUF_MEMORY_TYPE);
        param.u32FWSize = sizeof(u8TSPFirmware);
        param.phyVQAddr = (MS_PHYADDR)MEM_ADR_BY_MIU(TSP_VQ_BUF_ADR, TSP_VQ_BUF_MEMORY_TYPE);
        param.u32VQSize = TSP_VQ_BUF_LEN;
        //param.phyTSO_VQAddr = 0;
        //param.u32TSO_VQSize = 0;
        printf("FWAddr = %08lX , Size = %08lX , VQAddr = %08lX , VQSize = %08lX \n",param.phyFWAddr,param.u32FWSize,param.phyVQAddr,param.u32VQSize);

        MApi_DMX_Init();
        if (MApi_DMX_TSPInit(&param) != DMX_FILTER_STATUS_OK )
        {
            return FALSE ;
        }
    }

    MApi_DMX_GetCap(DMX_CAP_SEC_FILTER_NUM, &u32num);
    MApi_DMX_SetOwner(0, u32num-1, TRUE);

    MDrv_DSCMB_Init();

    _s32DataReadyEventId[0]= MsOS_CreateEventGroup("Data ready Event ID  0 - 31 \n ");
    _s32DataReadyEventId[1]= MsOS_CreateEventGroup("Data ready Event ID 32 - 63\n ");

    // Enable or Disable to remove TEI  audio or vedio error packets.
    MApi_DMX_TEI_RemoveErrorPkt( E_DMX_TEI_REMOVE_AUDIO_PKT, FALSE);
    MApi_DMX_TEI_RemoveErrorPkt( E_DMX_TEI_REMOVE_VIDEO_PKT, FALSE);

    // Get the PCR engine number
    MApi_DMX_GetCap(DMX_CAP_VFIFO_NUM, &u32num);

    stPCR = malloc(sizeof(ST_PCR_INFO)*u32num);

    for( u32Idx=0; u32Idx<u32num; u32Idx++)
    {
        stPCR[u32Idx].u8FltId = INVALID_FILTER_ID;
    }

    return TRUE ;
}
#else
MS_BOOL Demo_DMX_Init(void)
{
    MS_U32 u32Idx;
    MS_U32 u32PCREngNum;

    printf("Demo_DMX_Init\n");

    Demo_DMX_PreInit();

    MApi_DMX_Init();
    {   // set VQ for TSP, set FW/VQ for TSP2

        DMX_TSPParam    param;
        memset(&param,0,sizeof(param));
        memcpy((void*)MS_PA2KSEG1((MS_U32)MEM_ADR_BY_MIU(TSP_FW_BUF_ADR, TSP_FW_BUF_MEMORY_TYPE)/*u8TSPFirmware*/) , u8TSPFirmware, sizeof(u8TSPFirmware));
        MApi_DMX_SetFW(((MS_PHYADDR)MEM_ADR_BY_MIU(TSP_FW_BUF_ADR, TSP_FW_BUF_MEMORY_TYPE)/*u8TSPFirmware*/), sizeof(u8TSPFirmware));
        param.phyFWAddr = (MS_PHYADDR) MEM_ADR_BY_MIU(TSP_FW_BUF_ADR, TSP_FW_BUF_MEMORY_TYPE);
        param.u32FWSize = sizeof(u8TSPFirmware);
        param.phyVQAddr = (MS_PHYADDR)MEM_ADR_BY_MIU(TSP_VQ_BUF_ADR, TSP_VQ_BUF_MEMORY_TYPE);
        param.u32VQSize = TSP_VQ_BUF_LEN;
        printf("FWAddr = %08"DTC_MS_PHY_X" , Size = %08"DTC_MS_U32_X" , VQAddr = %08"DTC_MS_PHY_X" , VQSize = %08"DTC_MS_U32_X" \n", param.phyFWAddr, param.u32FWSize, param.phyVQAddr, param.u32VQSize);
        if (MApi_DMX_TSPInit(&param) != DMX_FILTER_STATUS_OK )
        {
           return FALSE ;
        }
    }
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, TRUE);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, TRUE);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO2, TRUE);

    MDrv_DSCMB_Init();

    _s32DataReadyEventId[0]= MsOS_CreateEventGroup("Data ready Event ID  0 - 31 \n ");
    _s32DataReadyEventId[1]= MsOS_CreateEventGroup("Data ready Event ID 32 - 63\n ");

    //  Enable or Disable to remove TEI  audio or vedio error packets.
    MApi_DMX_TEI_RemoveErrorPkt( E_DMX_TEI_REMOVE_AUDIO_PKT, FALSE);
    MApi_DMX_TEI_RemoveErrorPkt( E_DMX_TEI_REMOVE_VIDEO_PKT, FALSE);

    // Get the PCR engine number
    MApi_DMX_GetCap(DMX_CAP_VFIFO_NUM, &u32PCREngNum);

#if (DEMO_DMX_NEW_ARCHI_TEST == 1)
    MApi_DMX_GetCap(DMX_CAP_PCRFLT_START_IDX, &u8PCRFltStartIdx);
#endif

    stPCR = malloc(sizeof(ST_PCR_INFO)*u32PCREngNum);

    for( u32Idx=0; u32Idx<u32PCREngNum; u32Idx++)
    {
        stPCR[u32Idx].u8FltId = INVALID_FILTER_ID;
    }

    return TRUE ;
}
#endif

MS_BOOL Demo_DMX_Exit(void)
{
    if(DMX_FILTER_STATUS_OK != MApi_DMX_Exit() )
    {
        DemoDmx_Print("DMX Exit Fail !\n");
        return FALSE;
    }

    MsOS_DeleteEventGroup(_s32DataReadyEventId[0]);
    MsOS_DeleteEventGroup(_s32DataReadyEventId[1]);
    _s32DataReadyEventId[0] =  -1;
    _s32DataReadyEventId[1] =  -1;

    return TRUE;
}




//-------------------------------------------------------------------------------------------------
// DMX Demo Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @Brief the command interface for sample code to demo PCR filter characteristics
/// @param[in] pu8DmxFlow
///                   LIVE0
///                   LIVE1
///                   LIVE2
///                   LIVE3
/// @param[in] pu16Pid Pid value of stream PCR.
/// @return TRUE  : testing success, print out pcr value .
/// @return FALSE : error  occur while opening filter.
/// @sa
/// @note
/// Command: \b dmx_pcr [DMX flow] [Pid] \n    EX: dmx_pcr LIVE0  [Pid]
//------------------------------------------------------------------------------

MS_BOOL Demo_DMX_PCR_CMD(MS_U8* pu8DmxFlow, MS_U16* pu16Pid)
{
    EN_DEMO_DMX_FLOW eDmxFlow = _Demo_DMX_CMD_Mapping(pu8DmxFlow);

    if(eDmxFlow==E_DMX_FLOW_INVALID)
    {
        DemoDmx_Print("Wrong DMX FLOW!\n");
        return FALSE;
    }

    if( Demo_DMX_PCR(eDmxFlow, *pu16Pid))
    {
        DemoDmx_Print("Demo_DMX_PCR Success!\n");
        return TRUE;
    }
    else
    {
        DemoDmx_Print("Demo_DMX_PCR Failed!\n");
        return FALSE;
    }


}

//------------------------------------------------------------------------------
/// @Brief the sample code to demo PCR filter characteristics
/// @param[in] eDmxFlow :
///                   E_DMX_FLOW_LIVE0
///                   E_DMX_FLOW_LIVE1
///                   E_DMX_FLOW_LIVE2
///                   E_DMX_FLOW_LIVE3
/// @param[in] u16Pid Pid value of stream PCR.
/// @return TRUE  : testing success, print out pcr value .
/// @return FALSE : error  occur while opening filter.
/// @sa
/// @note
//------------------------------------------------------------------------------
MS_BOOL Demo_DMX_PCR(EN_DEMO_DMX_FLOW eDmxFlow, MS_U16 u16Pid)
{
    MS_U32 u32Idx;
    MS_U32 u32PCR_H=0, u32PCR=0, u32PrePCR=0;
    MS_U8  u8PCREngId;
    EN_DEMO_DMX_FLT_SOURCE eFltSrcType;

    eFltSrcType = Demo_DMX_FlowToFltSrcMapping(eDmxFlow);

    printf("PCR PID = %hx \n",u16Pid);

    if (TRUE != Demo_DMX_PCR_FltOpen(eFltSrcType, u16Pid, &u8PCREngId))
    {
        DemoDmx_Print("Demo_DMX_PCR_FltOpen Failed!\n");
        return false;
    }
    else
    {
        printf("PCR Eng ID:%u Open Success!\n\n", u8PCREngId);
    }

    if (TRUE != Demo_DMX_PCR_FltStart(u8PCREngId))
    {
        DemoDmx_Print("Demo_DMX_PCR_FltStart Failed!\n");
        goto Demo_DMX_PCR_FAIL;
    }

    MsOS_DelayTask(300);

    for (u32Idx=0; u32Idx<10; u32Idx++)
    {
        MApi_DMX_Pcr_Get(&u32PCR_H,&u32PCR);
        printf("PCR Value = %"DTC_MS_U32_x"%08"DTC_MS_U32_x"\n", u32PCR_H, u32PCR);

        if (u32PrePCR > u32PCR)
        {
            goto Demo_DMX_PCR_FAIL;
        }

        u32PrePCR = u32PCR;
        MsOS_DelayTask(300);
    }

    if (TRUE != Demo_DMX_PCR_FltClose(u8PCREngId))
    {
        DemoDmx_Print("Demo_DMX_PCR_FltClose Failed!\n");
        return FALSE;
    }
    else
    {
        printf("PCR Eng ID:%u Close Success!\n", u8PCREngId);
    }


    return TRUE;

Demo_DMX_PCR_FAIL:

    DemoDmx_Print("Demo_DMX_PCR Failed!\n");
    Demo_DMX_PCR_FltClose(u8PCREngId);

    return FALSE;

}


//------------------------------------------------------------------------------
/// @Brief the command interface for sample code to scan program information.
/// @param[in] pu8DmxFlow
///                   LIVE0
///                   LIVE1
///                   LIVE2
///                   LIVE3
/// @return TRUE: success, print out program information\n
/// @return FALSE: scan fail.
/// @sa
/// @note   Scan program and parse program information \n
/// Command: \b dmx_scan [DMX flow] \n    EX: dmx_scan LIVE0
//------------------------------------------------------------------------------

MS_BOOL Demo_DMX_Scan_CMD(MS_U8* pu8DmxFlow)
{
    EN_DEMO_DMX_FLOW eDmxFlow = _Demo_DMX_CMD_Mapping(pu8DmxFlow);

    if(eDmxFlow==E_DMX_FLOW_INVALID)
    {
        DemoDmx_Print("Wrong DMX FLOW!\n");
        return FALSE;
    }

    if( Demo_DMX_Scan(eDmxFlow, FALSE))
    {
        DemoDmx_Print("Demo_DMX_Scan Success!\n");
        return TRUE;
    }
    else
    {
        DemoDmx_Print("Demo_DMX_Scan Failed!\n");
        return FALSE;
    }
}


//------------------------------------------------------------------------------
/// @Brief the sample code to scan program information.
/// @param[in] eDmxFlow DMX flow
///                   0: E_DMX_FLOW_LIVE0
///                   1: E_DMX_FLOW_LIVE1
///                   2: E_DMX_FLOW_LIVE2
///                   3: E_DMX_FLOW_LIVE3
/// @return TRUE: success, print out program information\n
/// @return FALSE: scan fail.
/// @sa
/// @note   Scan program and parse program information \n
//------------------------------------------------------------------------------

MS_BOOL Demo_DMX_Scan(EN_DEMO_DMX_FLOW eDmxFlow, MS_BOOL bDisableDump)
{
    MS_U32 u32FltId;
    MS_U8  pattern[16],mask[16],nmask[16];
    MS_U32 u32Event = 0 ;
    MS_U32 u32Idx ;
    MS_U32 u32PidPAT = 0;

    memset(&stChDB, 0x0, sizeof(stChDB));

    memset(pattern , 0x0 , sizeof(pattern));
    memset(mask    , 0x0 , sizeof(mask));
    memset(nmask   , 0x0 , sizeof(nmask));

    // simple abstraction for section setup
    if (!_Demo_DMX_Setup(eDmxFlow, &u32FltId,u32PidPAT,true,pattern,mask,nmask))
    {
        goto Demo_DMX_Scan_FAIL;
    }
    else
        printf("DMX Set Section Pattern Up ... \n");

    // Start section filter
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start((MS_U8)u32FltId))
    {
        DemoDmx_Print("Enable Section Filter Fail!\n");
        goto Demo_DMX_Scan_FAIL;
    }
    else
        printf("DMX Section Filter Start ...  \n");


    // wait until data ready event or timeout
    //MsOS_WaitEvent(_s32EventId, _appDemo_Data_Ready, &u32Event, E_OR_CLEAR, 2000);

    WAIT_DATA_EVENT(u32FltId, u32Event);
    if (!u32Event)
    {
        DemoDmx_Print("Wait PAT Time Out!\n");

        // Stop Filter
        MApi_DMX_Stop(u32FltId);
        // Free Filter.
        MApi_DMX_Close(u32FltId);

        goto Demo_DMX_Scan_FAIL;
    }

    if(!bDisableDump)
        printf("dmx_copy data ...\n");
    
    _ScanFilterParsePAT(u32FltId);

    MApi_DMX_Stop(u32FltId);
    MApi_DMX_Close(u32FltId);

    for (u32Idx= 0; u32Idx< stChDB.u32PgNum; u32Idx++)
    {
        _Demo_DMX_Setup(eDmxFlow, &u32FltId ,stChDB.stPgList[u32Idx].u32PidPmt,true,pattern,mask,nmask);
         // Start section filter
        if (DMX_FILTER_STATUS_OK != MApi_DMX_Start((MS_U8)u32FltId))
        {
            DemoDmx_Print("Enable Section Filter Fail!\n");
            goto Demo_DMX_Scan_FAIL;
        }
        else
        {   
            if(!bDisableDump)
                printf("DMX Section Filter Start ...  \n");
        }


        if(!bDisableDump)
            printf("[%s][%d]u32FltId:%"DTC_MS_U32_x" PMT PID:%"DTC_MS_U32_x"\n",__FUNCTION__,__LINE__, u32FltId, stChDB.stPgList[u32Idx].u32PidPmt);


        //MsOS_WaitEvent(_s32EventId, _appDemo_Data_Ready, &u32Event, E_OR_CLEAR, 5000);
        WAIT_DATA_EVENT(u32FltId, u32Event);
        if (u32Event)
        {
            _ScanFilterParsePMT(u32FltId);
        }

        MApi_DMX_Stop(u32FltId);
        MApi_DMX_Close(u32FltId);
    }

       if(!bDisableDump)
            _DumpCh();


    return TRUE ;

Demo_DMX_Scan_FAIL:
    return FALSE;

}


//------------------------------------------------------------------------------
/// @Brief the command interface for sample code to parse section for test demux.
/// @param[in] pu8DmxFlow
///                   LIVE0
///                   LIVE1
///                   LIVE2
///                   LIVE3
/// @param pu16Pid       \b IN Pid
/// @param pu32Pat0     \b IN match pattern0 (byte 0~3)
/// @param pu32Pat1     \b IN match pattern1 (byte 4~7)
/// @param pu32Mask0  \b IN mask0 (byte 0~3)
/// @param pu32Mask1  \b IN mask1 (byte 4~7)
/// @return TRUE: success / FALSE: fail
/// @sa
/// @note
/// Command: \b dmx_sec  [DMX flow][Pid] [MatchPattern1] [MatchPattern2] [MatchMask1] [MatchMask2]\n
//------------------------------------------------------------------------------

MS_BOOL Demo_DMX_Sec_CMD(MS_U8* pu8DmxFlow, MS_U16* pu16Pid , MS_U32* pu32Pat0, MS_U32* pu32Pat1, MS_U32* pu32Mask0 , MS_U32* pu32Mask1)
{
    EN_DEMO_DMX_FLOW eDmxFlow = _Demo_DMX_CMD_Mapping(pu8DmxFlow);

    if(eDmxFlow==E_DMX_FLOW_INVALID)
    {
        DemoDmx_Print("Wrong DMX FLOW!\n");
        return FALSE;
    }

    if( Demo_DMX_Sec(eDmxFlow, *pu16Pid, pu32Pat0, pu32Pat1, pu32Mask0, pu32Mask1))
    {
        DemoDmx_Print("Demo_DMX_Sec Success!\n");
        return TRUE;
    }
    else
    {
        DemoDmx_Print("Demo_DMX_Sec Failed!\n");
        return FALSE;
    }
}


//------------------------------------------------------------------------------
/// @Brief the sample code to parse section for test demux
/// @param[in] eDmxFlow DMX flow
///                   0: E_DMX_FLOW_LIVE0
///                   1: E_DMX_FLOW_LIVE1
///                   2: E_DMX_FLOW_LIVE2
///                   3: E_DMX_FLOW_LIVE3
/// @param u16Pid         \b IN Pid
/// @param pu32Pat0     \b IN match pattern0 (byte 0~3)
/// @param pu32Pat1     \b IN match pattern1 (byte 4~7)
/// @param pu32Mask0  \b IN mask0 (byte 0~3)
/// @param pu32Mask1  \b IN mask1 (byte 4~7)
/// @return TRUE: success / FALSE: fail
/// @sa
/// @note
//------------------------------------------------------------------------------
MS_BOOL Demo_DMX_Sec(EN_DEMO_DMX_FLOW eDmxFlow, MS_U16 u16Pid , MS_U32* pu32Pat0, MS_U32* pu32Pat1, MS_U32* pu32Mask0 , MS_U32* pu32Mask1)
{
    MS_PHY                                 phyRead;
    MS_PHY                                 phyWrite;
    MS_U8                                  DmxIdSect;
    MS_U8                                  pattern[16];
    MS_U8                                  mask[16];
    MS_U8                                  nmask[16];
    DMX_Flt_info                           FltInfo;
    MS_U32                                 u32Event;
    MS_U8*                                 pu8BufAddr;
    DMX_FILTER_TYPE                        FilterType;
    DMX_FILTER_TYPE                        eFltSrcType;


    eFltSrcType = _Demo_DMX_FltSourceMapping(Demo_DMX_FlowToFltSrcMapping(eDmxFlow));

    memset(pattern, 0x0, sizeof(pattern));
    memset(mask,    0x0, sizeof(mask));
    memset(nmask,   0x0, sizeof(nmask));
/*
    pat  = ipattern;
    lmask = imask;
    lnmask = inmask;

    for ( i = 0 ; i < 8 ; i ++ )
        {
            pattern[i] = pat[i];
            mask[i]  = lmask[i]    ;
            nmask[i] = lnmask[i]   ;
            }
*/

    pattern[0] = (*pu32Pat0) >> 24 ;
    pattern[1] = ((*pu32Pat0) >> 16)& 0xFF ;
    pattern[2] = ((*pu32Pat0) >> 8)& 0xFF ;
    pattern[3] = ((*pu32Pat0))& 0xFF ;

    pattern[4] = (*pu32Pat1) >> 24 ;
    pattern[5] = ((*pu32Pat1) >> 16)& 0xFF ;
    pattern[6] = ((*pu32Pat1) >> 8)& 0xFF ;
    pattern[7] = ((*pu32Pat1))& 0xFF ;

    mask[0] = (*pu32Mask0) >> 24 ;
    mask[1] = ((*pu32Mask0) >> 16)& 0xFF ;
    mask[2] = ((*pu32Mask0) >> 8)& 0xFF ;
    mask[3] = ((*pu32Mask0))& 0xFF ;

    mask[4] = (*pu32Mask1) >> 24 ;
    mask[5] = ((*pu32Mask1) >> 16)& 0xFF ;
    mask[6] = ((*pu32Mask1) >> 8)& 0xFF ;
    mask[7] = ((*pu32Mask1))& 0xFF ;


    // Set Parameter of Allocate Demux Filter type and input Source type.
    FilterType = DMX_FILTER_TYPE_SECTION | eFltSrcType;

    // Allocate a Filter and set Filter Basic Type
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(FilterType, &DmxIdSect))
    {
        DemoDmx_Print("[%s] Allocate filter fail \n",__FUNCTION__);
    }

    // Here we use pre-defined physical address of reserved section buffer.
    // [Note] The section buffe MUST be continus in physical address space.(for DMA usage)
    pu8BufAddr = (MS_U8*)Section_Buf_Addr[DmxIdSect] ;
    memset((MS_U8*)MsOS_PA2KSEG1((MS_PHYADDR)pu8BufAddr), 0 , SECTION_BUFFER_SIZE);

    // Transfer Virtual Address to Phisical Hardware Address
    // Section buffer is structed as ring buffer, keep 4 pointer of start,end ,read and write.
    FltInfo.Info.SectInfo.SectBufAddr = (MS_PHYADDR)pu8BufAddr ;

    DemoDmx_Print("Physical Address = %"DTC_MS_PHY_x"\n" , FltInfo.Info.SectInfo.SectBufAddr);
    // Set buffer size
    FltInfo.Info.SectInfo.SectBufSize =     SECTION_BUFFER_SIZE;
    DemoDmx_Print("Size = %08"DTC_MS_U32_x"\n" , FltInfo.Info.SectInfo.SectBufSize);


    FltInfo.Info.SectInfo.SectMode    =  DMX_SECT_MODE_CRCCHK ;

    // <DMX_EVENT_DATA_READY/DMX_EVENT_BUF_OVERFLOW>
    // Event trigger condition for driver, Driver will call ap's callback  < CallBack Mode Type2 >
    FltInfo.Info.SectInfo.Event        =     DMX_EVENT_DATA_READY | DMX_EVENT_BUF_OVERFLOW | DMX_EVENT_CB_SELF ;

    // Set the pointer of the event CB function into Demux driver
    FltInfo.Info.SectInfo.pNotify =    &_Demo_DMX_DataCb;

    // Set Advanced Filter infomation
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Info( DmxIdSect, &FltInfo, &FilterType, TRUE))
    {
        DemoDmx_Print("[%s] MApi_DMX_Info fail \n",__FUNCTION__);
    }

    // Set Filter PID --> Section PID
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid( DmxIdSect , &u16Pid , TRUE))
    {
        DemoDmx_Print("[%s] MApi_DMX_Pid fail \n",__FUNCTION__);
    }

    // Set Section Match pattern
    // The Match pattern is used for Getting specific section
    // Pattern[16] =  Pattern for match
    // Mask[16]    =  Match Mask : set 1 for match / 0 for ignore
    MApi_DMX_SectPatternSet(DmxIdSect, pattern, mask, nmask, 16);

    // Reset Section filter and section Buffer status;
    MApi_DMX_SectReset(DmxIdSect);

    // Start Filter and record section into Section Buffer.
    if (DMX_FILTER_STATUS_OK!= MApi_DMX_Start(DmxIdSect))
    {
        DemoDmx_Print("enable section filter fail\n");
    }

    // Wait event until TSP driver call callback function
    // Wait event "_appDemo_Pat_Ready"
    // (Data Ready)-->TSP driver-->(callback)-->_appDemo_PatCb-->(SetEvent"_appDemo_Pat_Ready")
    //             -->contitue execution
    WAIT_DATA_EVENT(DmxIdSect,u32Event);
    if (!u32Event)
    {
        goto DMX_SEC_FAIL;
    }

    // Get Buffer Read Pointer.
    MApi_DMX_SectReadGet(DmxIdSect, (MS_PHY*)(&phyRead));
    DemoDmx_Print("Read pointer = 0x%08"DTC_MS_U32_x"\n", (MS_U32)phyRead);

    // Get Buffer Write Pointer.
    MApi_DMX_SectWriteGet(DmxIdSect, (MS_PHY*)(&phyWrite));
    DemoDmx_Print("Write pointer = 0x%08"DTC_MS_U32_x"\n", (MS_U32)phyWrite);

    //_appDemo_Pat_Parse((MS_U8*)MsOS_PA2KSEG1(pu8Read), pu8Write- pu8Read);
    if ((phyWrite- phyRead) == 0 )
        {DemoDmx_Print("\n !! No Section Get - Wait timeout \n" );}
    else
        {data_dump((MS_U8*)MsOS_PA2KSEG1(phyRead),phyWrite- phyRead);}


    // Stop Filter
    MApi_DMX_Stop(DmxIdSect);

    // Free Filter.
    MApi_DMX_Close(DmxIdSect);

    return TRUE;

DMX_SEC_FAIL:

    MApi_DMX_Close(DmxIdSect);

    return FALSE ;

}

//------------------------------------------------------------------------------
/// @Brief the command interface for sample code to get DMX Packet
/// @param[in] pu8DmxFlow
///                   LIVE0
///                   LIVE1
///                   LIVE2
///                   LIVE3
/// @param pu16Pid       \b IN Pid
/// @return TRUE: success / FALSE: fail
/// @sa
/// @note
/// Command: \b dmx_pkt  [DMX flow][Pid] \n
//------------------------------------------------------------------------------

MS_BOOL Demo_DMX_Packet_CMD(MS_U8* pu8DmxFlow, MS_U16* pu16Pid)
{
    EN_DEMO_DMX_FLOW eDmxFlow = _Demo_DMX_CMD_Mapping(pu8DmxFlow);

    if(eDmxFlow==E_DMX_FLOW_INVALID)
    {
        DemoDmx_Print("Wrong DMX FLOW!\n");
        return FALSE;
    }

    if( Demo_DMX_Packet(eDmxFlow, *pu16Pid))
    {
        DemoDmx_Print("Demo_DMX_Packet Success!\n");
        return TRUE;
    }
    else
    {
        DemoDmx_Print("Demo_DMX_Packet Failed!\n");
        return FALSE;
    }
}

//------------------------------------------------------------------------------
/// @Brief the sample code to get DMX Packet
/// @param[in] eDmxFlow DMX flow
///                   0: E_DMX_FLOW_LIVE0
///                   1: E_DMX_FLOW_LIVE1
///                   2: E_DMX_FLOW_LIVE2
///                   3: E_DMX_FLOW_LIVE3
/// @param u16Pid       \b IN Pid
/// @return TRUE: success / FALSE: fail
/// @sa
/// @note
//------------------------------------------------------------------------------

MS_BOOL Demo_DMX_Packet(EN_DEMO_DMX_FLOW eDmxFlow, MS_U16 u16Pid)
{
    MS_U8                                  DmxId;
    MS_U32                                 DataLen,DataRmn;
    DMX_Flt_info                           FltInfo;
    MS_U8                                  pattern[16];
    MS_U8                                  mask[16];
    MS_U8                                  nmask[16];
    MS_U32                                 u32Event;
    MS_U8*                                 pu8BufAddr;
    MS_U32                                 u32Idx;
    ST_NotifyParam                            Pkt_Param;
    DMX_FILTER_TYPE                        FilterType;
    DMX_FILTER_TYPE                        eFltSrcType;


    eFltSrcType = _Demo_DMX_FltSourceMapping(Demo_DMX_FlowToFltSrcMapping(eDmxFlow));


    memset(pattern, 0x0, sizeof(pattern));
    memset(mask,    0x0, sizeof(mask));
    memset(nmask,   0x0, sizeof(nmask));

    // Set Parameter of Allocate Demux Filter type and input Source type.
    FilterType = DMX_FILTER_TYPE_PACKET| eFltSrcType;

    // Allocate a Filter and set Filter Basic Type
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(FilterType, &DmxId))
    {
        DemoDmx_Print("[%s] Allocate filter fail \n",__FUNCTION__);
        goto DMX_PKT_FAIL;
    }

    // Here we use pre-defined physical address of reserved section buffer.
    // [Note] The section buffe MUST be continus in physical address space.(for DMA usage)
    if (!Pes_Buf_Alloc(&u32Idx)) goto DMX_PKT_FAIL;

    pu8BufAddr = (MS_U8*)Pes_Buf_Addr[u32Idx] ;

    memset((MS_U8*)MsOS_PA2KSEG1((MS_PHYADDR)pu8BufAddr), 0 , PES_BUFFER_SIZE);

    // Transfer Virtual Address to Phisical Hardware Address
    // Section buffer is structed as ring buffer, keep 4 pointer of start,end ,read and write.
    FltInfo.Info.SectInfo.SectBufAddr = (MS_PHYADDR)pu8BufAddr ;

    DemoDmx_Print("Physical Address = %"DTC_MS_PHY_x"\n" , FltInfo.Info.SectInfo.SectBufAddr);
    // Set buffer size
    FltInfo.Info.SectInfo.SectBufSize =     PES_BUFFER_SIZE;
    DemoDmx_Print("Size = %08"DTC_MS_U32_x"\n" , FltInfo.Info.SectInfo.SectBufSize);

    FltInfo.Info.SectInfo.SectMode    =  DMX_SECT_MODE_CRCCHK | DMX_SECT_MODE_ONESHOT ;

    // <DMX_EVENT_DATA_READY/DMX_EVENT_BUF_OVERFLOW>
    // Event trigger condition for driver, Driver will call ap's callback  < CallBack Mode Type2 >
    FltInfo.Info.SectInfo.Event        =     DMX_EVENT_DATA_READY | DMX_EVENT_BUF_OVERFLOW | DMX_EVENT_CB_SELF_TYPE2 ;  // DMX_EVENT_DATA_READY

    // Set the pointer of the event CB function into Demux driver
    FltInfo.Info.SectInfo.pType2Notify =    &_Demo_DMX_DataCb_Type2;

    strcpy((char*)Pkt_Param.name,"APP Demo Packet filter Notify parameter test");
    Pkt_Param.Counter = 0 ;

    FltInfo.Info.SectInfo.Type2NotifyParam1 = (MS_U32)&Pkt_Param;

    // Set Advanced Filter infomation
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Info( DmxId, &FltInfo, &FilterType, TRUE))
    {
        DemoDmx_Print("[%s] MApi_DMX_Info fail \n",__FUNCTION__);
        goto DMX_PKT_FAIL;
    }

    // Set Filter PID --> Section PID
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid( DmxId, &u16Pid , TRUE))
    {
        DemoDmx_Print("[%s] MApi_DMX_Pid fail \n",__FUNCTION__);
        goto DMX_PKT_FAIL;
    }

    // Set Section Match pattern
    // The Match pattern is used for Getting specific section
    // Pattern[16] =  Pattern for match
    // Mask[16]    =  Match Mask : set 1 for match / 0 for ignore
    MApi_DMX_SectPatternSet(DmxId, pattern, mask, nmask, 16);

    // Reset Section filter and section Buffer status;
    MApi_DMX_SectReset(DmxId);

    printf("dmx_set up ... \n");

    // Start Filter and record section into Section Buffer.
    if (DMX_FILTER_STATUS_OK!= MApi_DMX_Start(DmxId))
    {
        DemoDmx_Print("enable section filter fail\n");
        goto DMX_PKT_FAIL;
    }

    // Wait event until TSP driver call callback function
    // Wait event "_appDemo_Pat_Ready"
    // (Data Ready)-->TSP driver-->(callback)-->_appDemo_PatCb-->(SetEvent"_appDemo_Pat_Ready")
    //             -->contitue execution.
    WAIT_DATA_EVENT(DmxId, u32Event);

    if (!u32Event )
    {
        printf("Wait DATA ready timeout !\n");
        goto DMX_PKT_FAIL;
    }

    printf("dmx_copy data ...\n");
    //DMA_Api for user to Copy data from HW section Buffer into user data Buffer.
    MApi_DMX_CopyData(DmxId,Data,1024,&DataLen,&DataRmn,NULL);


    if (DataLen != 0)
    {
        data_dump(Data,DataLen);
    }
    else
    {
        printf("\n !! No Section Get - Wait timeout \n");
        goto DMX_PKT_FAIL;
    }

    // Stop Filter
    MApi_DMX_Stop(DmxId);

    // Free Filter.
    MApi_DMX_Close(DmxId);

    //MsOS_FreeMemory(pu8BufAddr , 0);
    Pes_Buf_Free(u32Idx);

    return TRUE;

    DMX_PKT_FAIL:
    MApi_DMX_Close(DmxId);
    printf("\n[FAIL][%s]\n\n", __FUNCTION__);

    return FALSE ;
}


//------------------------------------------------------------------------------
/// @Brief the command interface for sample code to parse PES for test demux.
/// @param[in] pu8DmxFlow
///                   LIVE0
///                   LIVE1
///                   LIVE2
///                   LIVE3
/// @param pu16Pid       \b IN Pid
/// @return TRUE: success / FALSE: fail
/// @sa
/// @note
/// Command: \b dmx_pes  [DMX flow][Pid] \n
//------------------------------------------------------------------------------

MS_BOOL Demo_DMX_PES_CMD(MS_U8* pu8DmxFlow, MS_U16* pu16Pid)
{
    EN_DEMO_DMX_FLOW eDmxFlow = _Demo_DMX_CMD_Mapping(pu8DmxFlow);

    if(eDmxFlow==E_DMX_FLOW_INVALID)
    {
        DemoDmx_Print("Wrong DMX FLOW!\n");
        return FALSE;
    }

    if( Demo_DMX_PES(eDmxFlow, *pu16Pid))
    {
        DemoDmx_Print("Demo_DMX_PES Success!\n");
        return TRUE;
    }
    else
    {
        DemoDmx_Print("Demo_DMX_PES Failed!\n");
        return FALSE;
    }
}


//------------------------------------------------------------------------------
/// @Brief the command interface for sample code to parse PES for test demux.
/// @param[in] pu8DmxFlow
///                   LIVE0
///                   LIVE1
///                   LIVE2
///                   LIVE3
/// @param u16Pid       \b IN Pid
/// @return TRUE: success / FALSE: fail
/// @sa
/// @note
//------------------------------------------------------------------------------

MS_BOOL Demo_DMX_PES(EN_DEMO_DMX_FLOW eDmxFlow, MS_U16 u16Pid)
{
    MS_U8                                  DmxId;
    MS_U32                                 DataLen,DataRmn;
    DMX_Flt_info                           FltInfo;
    MS_U8                                  pattern[16];
    MS_U8                                  mask[16];
    MS_U8                                  nmask[16];
    MS_U32                                 u32Event;
    MS_U8*                                 pu8BufAddr;
    MS_U32                                 u32Idx ;
    ST_NotifyParam                            Pes_Param;
    DMX_FILTER_TYPE                        FilterType;
    DMX_FILTER_TYPE                        eFltSrcType;

    eFltSrcType = _Demo_DMX_FltSourceMapping(Demo_DMX_FlowToFltSrcMapping(eDmxFlow));

    memset(pattern, 0x0, sizeof(pattern));
    memset(mask,    0x0, sizeof(mask));
    memset(nmask,   0x0, sizeof(nmask));


    // Set Parameter of Allocate Demux Filter type and input Source type.
    FilterType = DMX_FILTER_TYPE_PES| eFltSrcType;

    // Allocate a Filter and set Filter Basic Type
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(FilterType, &DmxId))
    {
        DemoDmx_Print("[%s] Allocate filter fail \n",__FUNCTION__);
    }

    if (!Pes_Buf_Alloc(&u32Idx)) goto DMX_PES_FAIL;

    pu8BufAddr = (MS_U8*)Pes_Buf_Addr[u32Idx] ;

    memset((MS_U8*)MsOS_PA2KSEG1((MS_PHYADDR)pu8BufAddr), 0 , PES_BUFFER_SIZE);

    FltInfo.Info.SectInfo.SectBufAddr  =  (MS_PHYADDR)pu8BufAddr ;
    FltInfo.Info.SectInfo.SectBufSize  =  PES_BUFFER_SIZE;
    FltInfo.Info.SectInfo.SectMode     =  DMX_SECT_MODE_CRCCHK | DMX_SECT_MODE_ONESHOT ;
    FltInfo.Info.SectInfo.Event        =  DMX_EVENT_DATA_READY | DMX_EVENT_BUF_OVERFLOW | DMX_EVENT_CB_SELF_TYPE2 ;  // DMX_EVENT_DATA_READY
    FltInfo.Info.SectInfo.pType2Notify  =  &_Demo_DMX_DataCb_Type2;


    strcpy((char*)Pes_Param.name , "APP Demo Pes filter Notify parameter test");
    Pes_Param.Counter = 0 ;
    FltInfo.Info.SectInfo.Type2NotifyParam1 = (MS_U32)&Pes_Param;

    DemoDmx_Print("[%s] Allocate PES buffer = 0x%"DTC_MS_PHY_x" , Size = 0x%x  \n",__FUNCTION__, FltInfo.Info.SectInfo.SectBufAddr, PES_BUFFER_SIZE);

    // Set Advanced Filter infomation
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Info( DmxId, &FltInfo, &FilterType, TRUE))
    {
        DemoDmx_Print("[%s] MApi_DMX_Info fail \n",__FUNCTION__);
        goto DMX_PES_FAIL;
    }

    // Set Filter PID --> Section PID
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid( DmxId, &u16Pid , TRUE))
    {
        DemoDmx_Print("[%s] MApi_DMX_Pid fail \n",__FUNCTION__);
        goto DMX_PES_FAIL;
    }

    MApi_DMX_SectPatternSet(DmxId, pattern, mask, nmask, 16);
    MApi_DMX_SectReset(DmxId);

    printf("dmx_set up ... \n");

    // Start Filter and record section into Section Buffer.
    if (DMX_FILTER_STATUS_OK!= MApi_DMX_Start(DmxId))
    {
        DemoDmx_Print("enable section filter fail\n");
    }

    //MsOS_WaitEvent(_s32EventId, _appDemo_Data_Ready, &u32Event, E_OR_CLEAR, 2000);
    WAIT_DATA_EVENT(DmxId, u32Event);
    if (!u32Event)
    {
        printf("\n !! No Section Get - Wait timeout \n");
        goto DMX_PES_FAIL ;
    }

    printf("dmx_copy data ...\n");
    MApi_DMX_CopyData(DmxId,Data,1024,&DataLen,&DataRmn,NULL);

    if (DataLen != 0)
    {
        data_dump(Data,DataLen);
        printf(" \n");
        _Demo_DMX_PES_Parse(Data, DataLen);
    }
    else
    {
        printf("\n !! No Section Get - Wait timeout \n");
        goto DMX_PES_FAIL;
    }

    // Stop Filter
    MApi_DMX_Stop(DmxId);

    // Free Filter.
    MApi_DMX_Close(DmxId);

    //MsOS_FreeMemory(pu8BufAddr , 0);
    Pes_Buf_Free(u32Idx);

    return TRUE;

    DMX_PES_FAIL:
    printf("\n[FAIL][%s]\n\n", __FUNCTION__);
    MApi_DMX_Close(DmxId);
    Pes_Buf_Free(u32Idx);

    return FALSE ;
}


//------------------------------------------------------------------------------
/// @Brief the command interface for sample code to demo NMASK characteristics
/// @param[in] pu8DmxFlow
///                   FILE0
///                   FILE1
///                   FILE2
///                   FILE3
/// @param pu8Pat       \b input match pattern
/// @param pu8Mask     \b input match mask
/// @param pu8Nmask  \b input not match mask (NMASK)
/// @return TRUE: success, print out filter data.
/// @return FALSE: Error occured;
/// @sa
/// @note   Support 16 bytes match pattern depth, demo code only use 4 bytes for pattern match \n
/// Command: \b dmx_nmask [DMX flow][Pattern] [Mask] [Nmask]  \n
///                  \b dmx_nmask FILE0 40800000 FFFF0000 00000000 \n
///                  \b dmx_nmask FILE0 40800000 FFFF0000 F0000000 \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DMX_Nmask_CMD(MS_U8* pu8DmxFlow, MS_U8 *pu8Pat, MS_U8 *pu8Mask, MS_U8 *pu8Nmask )
{
    EN_DEMO_DMX_FLOW eDmxFlow = _Demo_DMX_CMD_Mapping(pu8DmxFlow);

    if(eDmxFlow==E_DMX_FLOW_INVALID)
    {
        DemoDmx_Print("Wrong DMX FLOW!\n");
        return FALSE;
    }

    if( Demo_DMX_Nmask(eDmxFlow, pu8Pat, pu8Mask, pu8Nmask))
    {
        DemoDmx_Print("Demo_DMX_Nmask Success!\n");
        return TRUE;
    }
    else
    {
        DemoDmx_Print("Demo_DMX_Nmask Failed!\n");
        return FALSE;
    }
}


//------------------------------------------------------------------------------
/// @brief The sample code to demo NMASK characteristics
/// @param pu8Pat   \b input match pattern
/// @param pu8Mask  \b input match mask
/// @param pu8Nmask \b input not match mask (NMASK)
/// @return TRUE: success, print out filter data.
/// @return FALSE: Error occured;
/// @sa
/// @note   Support 16 bytes match pattern depth, demo code only use 4 bytes for pattern match \n
//------------------------------------------------------------------------------

MS_BOOL Demo_DMX_Nmask(EN_DEMO_DMX_FLOW eDmxFlow, MS_U8 *pu8Pat, MS_U8 *pu8Mask, MS_U8 *pu8Nmask )
{
    MS_U32 DmxId;
    MS_U8  pattern[16],mask[16],nmask[16];
    MS_U32 u32Event;
    DMX_Filein_info FileinInfo;
    MS_U8  *FileinAddr;
    MS_U32 DataLen,DataRmn;
    MS_U32 i ;


    memset(pattern , 0x0 , sizeof(pattern));
    memset(mask    , 0x0 , sizeof(mask));
    memset(nmask   , 0x0 , sizeof(nmask));

    for( i = 0; i < 4 ; i ++)
        pattern[i] = (_convert(pu8Pat  [2*i]) << 4 ) |  _convert(pu8Pat  [2*i+1]) ;
    for( i = 0; i < 4 ; i ++)
        mask[i]    = (_convert(pu8Mask [2*i]) << 4 ) |  _convert(pu8Mask [2*i+1]) ;
    for( i = 0; i < 4 ; i ++)
        nmask[i]   = (_convert(pu8Nmask[2*i]) << 4 ) |  _convert(pu8Nmask[2*i+1]) ;

    // simple abstraction for section setup
    if (!_Demo_DMX_Setup(eDmxFlow, &DmxId ,0xaaa ,false ,pattern ,mask ,nmask))
    {
        goto DMX_NMASK_FAIL;
    }
    printf("dmx_set up ... \n");

    // Start section filter
    if (DMX_FILTER_STATUS_OK!= MApi_DMX_Start(DmxId))
    {
        DemoDmx_Print("enable section filter fail\n");
    }

    printf("dmx start ...  \n");

    // File in Setting
    // Set Packet size 188 = TS packet size(188/192/204)
    FileinInfo.PKT_Mode = DMX_PKTMODE_188;
    // Set filein input delay 0x0 ~ 0xFFFFFF
    FileinInfo.Rate = 0x0000ffff;
    // set filein parameter
    MApi_DMX_Filein_Info(&FileinInfo);

    // Star filein Mode => [DMX_PES_NO_BYPASS_FIFO/DMX_PES_AUDIO2_FIFO/DMX_PES_VIDEO_FIFO/DMX_PES_AUDIO_FIFO]
    // [Note] Cause TSP Filein DMA only work with continuous physical memory space,
    //        we have to copy filein data from VA(might discrete) to PA(continuous).
    //        Here we use 0x0 as temp filein buffer[unused section].
    FileinAddr =  _Demo_DMX_MemAlloc_Func(sizeof(Stream));
    memcpy( (MS_U8*)(FileinAddr),Stream,sizeof(Stream));
    MApi_DMX_Filein_Start(DMX_PES_NO_BYPASS_FIFO, MsOS_VA2PA((MS_VIRT) FileinAddr), sizeof(Stream));

    //MsOS_WaitEvent(_s32EventId, _appDemo_Data_Ready, &u32Event, E_OR_CLEAR, 5000);
    WAIT_DATA_EVENT(DmxId, u32Event);

    if (!u32Event )
    {
        printf("Wait DATA ready timeout !\n");
        goto DMX_NMASK_FAIL;
    }


    MApi_DMX_CopyData(DmxId,(MS_U8*)Data,1024,&DataLen,&DataRmn,NULL);
    printf("Copy data ! \n");


    while(DataLen)
    {
        if (DataLen != 0)
        {
            data_dump((MS_U8*)Data,DataLen);
        }
        else
        {
            printf("Should not happened!\n");
        }
        MApi_DMX_CopyData(DmxId,Data,1024,&DataLen,&DataRmn,NULL);

    }

    // Stop Filter
    MApi_DMX_Stop(DmxId);

    // Free Filter.
    MApi_DMX_Close(DmxId);


    _Demo_DMX_MemFree_Func(FileinAddr);

    return TRUE ;

DMX_NMASK_FAIL:

    return FALSE ;

}

//-------------------------------------------------------
//DMX Demo Record
//-------------------------------------------------------

MS_U8 u8PCR_PVRFltId,u8Video_PVRFltId,u8Audio_PVRFltId;
DMX_PVR_ENG eDmxPvrEng;
MS_U8 u8TaskStack[4096];
MS_PHY            _pDmxRec = 0;
static Task_Info _DMX_RECTask = {-1, E_TASK_PRI_SYS, (void *)u8TaskStack, 4096, "DMX Rec Task"};
#define DMX_DOWNLOAD_LEN                                0x0000360000


//-------------------------------------------------------
/// @brief Abstraction to demo get ts data from PVR Eng and record into a File
/// @return none
/// @sa
/// @note
//-------------------------------------------------------

static MS_BOOL _DMX_REC(void)
{
    MS_PHY phyRecordAdr = _pDmxRec,u32PreRecordAdr = _pDmxRec;
    size_t u32RecordBufSize = DMX_DOWNLOAD_LEN,u32WaterLevel = 0;
    size_t u32Gap = 0;
    MS_U8 *pu8Addr;
    FILE *Fptr = MsFS_Fopen("/mnt/sda1/DMX_REC.ts","w");
    MS_S32 s32Threadhold = 100;

    while(1)
    {

        MApi_DMX_Pvr_Eng_WriteGet(eDmxPvrEng,&phyRecordAdr);
        if(phyRecordAdr < u32PreRecordAdr)
        {
            u32WaterLevel = (u32RecordBufSize - u32PreRecordAdr)+phyRecordAdr;
            u32Gap = (_pDmxRec + u32RecordBufSize) - u32PreRecordAdr;
        }
        else
        {
            u32WaterLevel = phyRecordAdr - u32PreRecordAdr;
            u32Gap = 0;
        }

        printf("phyRecordAdr:%"DTC_MS_PHY_x"\n", phyRecordAdr);

        pu8Addr = (MS_U8 *)MsOS_PA2KSEG1(u32PreRecordAdr);

        if(u32WaterLevel)
        {
            if(u32Gap > 0)
            {
                MsFS_Fwrite(pu8Addr,u32Gap,1,Fptr);
                pu8Addr = (MS_U8 *)MsOS_PA2KSEG1(_pDmxRec);
                u32WaterLevel -= u32Gap;
            }

            MsFS_Fwrite(pu8Addr,u32WaterLevel,1,Fptr);
        }
        u32PreRecordAdr = phyRecordAdr;
        s32Threadhold--;
        MsOS_DelayTask(10);

        if(s32Threadhold < 0)
        {
            printf("Record Done!!!\n");
            break;
        }

    }

    MsFS_Fclose(Fptr);
    MApi_DMX_Pvr_Eng_Pid_Close(eDmxPvrEng,u8PCR_PVRFltId);
    MApi_DMX_Pvr_Eng_Pid_Close(eDmxPvrEng,u8Video_PVRFltId);
    MApi_DMX_Pvr_Eng_Pid_Close(eDmxPvrEng,u8Audio_PVRFltId);
    MApi_DMX_Pvr_Eng_Stop(eDmxPvrEng);
    MApi_DMX_Pvr_Eng_Close(eDmxPvrEng);
    return TRUE;
}



//------------------------------------------------------------------------------
/// @Brief the command interface for sample code to record a live stream
/// @param[in] pu8Live
///                   LIVE0
///                   LIVE1
///                   LIVE2
///                   LIVE3
/// @param pu8Eng
/// @param pu32VideoPid       \b input VideoPid
/// @param pu32AudioPid       \b input AudioPid
/// @param pu32PCRPid         \b input PCRPid
/// @return TRUE: success.
/// @return FALSE: Error occured;
/// @sa
/// @note
/// Command: \b DMX_REC \n
///
/// Sample Command:DMX_REC LIVE0 ENG0 [VideoPid] [AudioPid] [PCRPid]
//------------------------------------------------------------------------------
MS_BOOL Demo_DMX_Record_CMD(MS_U8 *pu8Live, MS_U8 *pu8Eng, MS_U32 *pu32VideoPid,MS_U32 *pu32AudioPid,MS_U32 *pu32PCRPid)
{
    EN_DEMO_DMX_PVR_SOURCE ePvrSrc = E_DMX_PVR_SOURCE_INVALID;
    EN_DEMO_DMX_PVR_ENG    ePvrEng = E_DMX_PVR_ENG_INVALID;


    if(_Demo_DMX_strcmp(pu8Live, "LIVE0")==0 )
        ePvrSrc = E_DMX_PVR_SOURCE_LIVE0;
    else if(_Demo_DMX_strcmp(pu8Live, "LIVE1")==0 )
        ePvrSrc = E_DMX_PVR_SOURCE_LIVE1;
    else if(_Demo_DMX_strcmp(pu8Live, "LIVE2")==0 )
        ePvrSrc = E_DMX_PVR_SOURCE_LIVE2;
    else if(_Demo_DMX_strcmp(pu8Live, "LIVE3")==0 )
        ePvrSrc = E_DMX_PVR_SOURCE_LIVE3;
    else if(_Demo_DMX_strcmp(pu8Live, "FILE0")==0 )
        ePvrSrc = E_DMX_PVR_SOURCE_FILE0;
    else if(_Demo_DMX_strcmp(pu8Live, "FILE1")==0 )
        ePvrSrc = E_DMX_PVR_SOURCE_FILE1;
    else if(_Demo_DMX_strcmp(pu8Live, "FILE2")==0 )
        ePvrSrc = E_DMX_PVR_SOURCE_FILE2;
    else if(_Demo_DMX_strcmp(pu8Live, "FILE3")==0 )
        ePvrSrc = E_DMX_PVR_SOURCE_FILE3;
    else
        ePvrSrc = E_DMX_PVR_SOURCE_INVALID;

    if(_Demo_DMX_strcmp(pu8Eng, "ENG0")==0 )
        ePvrEng = E_DMX_PVR_EGN0;
    else if(_Demo_DMX_strcmp(pu8Eng, "ENG1")==0 )
        ePvrEng = E_DMX_PVR_EGN1;
    else if(_Demo_DMX_strcmp(pu8Eng, "ENG2")==0 )
        ePvrEng = E_DMX_PVR_EGN2;
    else if(_Demo_DMX_strcmp(pu8Eng, "ENG3")==0 )
        ePvrEng = E_DMX_PVR_EGN3;
    else
        ePvrEng = E_DMX_PVR_ENG_INVALID;


    if(ePvrSrc==E_DMX_PVR_SOURCE_INVALID || ePvrEng==E_DMX_PVR_ENG_INVALID)
    {
        DemoDmx_Print("Wrong DMX PVR Source and Engine!\n");
        return FALSE;
    }


    if( Demo_DMX_Record(ePvrSrc, ePvrEng, *pu32VideoPid, *pu32AudioPid, *pu32PCRPid))
    {
        DemoDmx_Print("Demo_DMX_Record Success!\n");
        return TRUE;
    }
    else
    {
        DemoDmx_Print("Demo_DMX_Record Failed!\n");
        return FALSE;
    }
}


//------------------------------------------------------------------------------
/// @Brief the sample code to record a live stream
/// @param[in] u8Eng    The number of PVR ENG for recording
/// @param[in] u8Live   The number of LIVE TV for recording
/// @param[in] VideoPid Video PID of the current program
/// @param[in] AudioPid Audio PID of the current program
/// @param[in] PCRPid   PCR PID of the current program
/// @sa
/// @note
//------------------------------------------------------------------------------

MS_BOOL Demo_DMX_Record(EN_DEMO_DMX_PVR_SOURCE ePvrSrc, EN_DEMO_DMX_PVR_ENG ePvrEng, MS_U32 u32VideoPid,MS_U32 u32AudioPid,MS_U32 u32PCRPid)
{
    printf("Input: Video:%"DTC_MS_U32_u" Audio:%"DTC_MS_U32_u" PCR:%"DTC_MS_U32_u" Eng:%d Live:%d\n", u32VideoPid, u32AudioPid, u32PCRPid,ePvrSrc,ePvrEng);

    DMX_TSIF eDmxTSIF;

    void *_pTempDmxRec = _Demo_DMX_MemAlloc_Func (DMX_DOWNLOAD_LEN); //3.375m
    _pDmxRec =  MsOS_MPool_VA2PA((MS_VIRT)_pTempDmxRec);

    eDmxTSIF = _Demo_DMX_PVRSrcMapping(ePvrSrc);

    eDmxPvrEng = _Demo_DMX_PVREngMapping(ePvrEng);


    //call new PVR flowset to connect PVR src to TSIF
    printf("PVR Flowset eDmxPvrEng:%d eDmxTSIF:%d\n",eDmxPvrEng,eDmxTSIF);

#if (DEMO_DMX_NEW_ARCHI_TEST == 1)
    MApi_DMX_PVR_FlowSet(eDmxPvrEng, eDmxTSIF,TRUE);
#endif

    //open PVR Eng and PVR Eng pid flts
    DMX_Pvr_info pvrInfo;

    memset(&pvrInfo,0,sizeof(DMX_Pvr_info));
    pvrInfo.pPvrBuf0 = _pDmxRec;
    pvrInfo.PvrBufSize0 = DMX_DOWNLOAD_LEN >> 1;
    pvrInfo.pPvrBuf1 = (_pDmxRec + (DMX_DOWNLOAD_LEN >>1));
    pvrInfo.PvrBufSize1 = DMX_DOWNLOAD_LEN >> 1;
    pvrInfo.pNotify = NULL;
    MApi_DMX_Pvr_Eng_Open(eDmxPvrEng,&pvrInfo);


    MApi_DMX_Pvr_Eng_Pid_Open(eDmxPvrEng,u32PCRPid,&u8PCR_PVRFltId);
    MApi_DMX_Pvr_Eng_Pid_Open(eDmxPvrEng,u32AudioPid,&u8Audio_PVRFltId);
    MApi_DMX_Pvr_Eng_Pid_Open(eDmxPvrEng,u32VideoPid,&u8Video_PVRFltId);

    printf("PVRAudioFlt:%d PVRVideoFlt:%d PVRPCRFlt:%d\n",u8Audio_PVRFltId,u8Video_PVRFltId,u8PCR_PVRFltId);


    MApi_DMX_Pvr_Eng_SetPacketMode(eDmxPvrEng,TRUE);
    MApi_DMX_Pvr_Eng_SetRecordStamp(eDmxPvrEng,0);
    MApi_DMX_Pvr_Eng_Start(eDmxPvrEng,FALSE);


    if (-1 == MsOS_CreateTask ((TaskEntry) _DMX_REC,
                           (MS_U32)NULL,
                           _DMX_RECTask.ePriority,
                           TRUE,
                           _DMX_RECTask.pStack,
                           _DMX_RECTask.u32StackSize,
                           (char *)_DMX_RECTask.szName))
       {
           DemoDmx_Print("Open Thread Failed\n");
       }

    return TRUE;
}


#if (DEMO_DMX_TSO_10_TEST == 1) && (DEMO_DMX_TSO_20_TEST == 1)

#error "TSO 1.0 / 2.0 could not enable concurrently";

#elif (DEMO_DMX_TSO_10_TEST == 1) || (DEMO_DMX_TSO_20_TEST == 1)

MS_BOOL Demo_DMX_TSO(MS_U8* pu8TSOEng, MS_U8* pu8Pad)
{
    DMX_FLOW DmxFlow = DMX_FLOW_NUM;

    if(*pu8TSOEng == 0)
        DmxFlow = DMX_FLOW_TSO_PLAYBACK;
    else if(*pu8TSOEng == 1)
        DmxFlow = DMX_FLOW_TSO_PLAYBACK1;
    else
        DemoDmx_Print("Unsupproted TSO Eng : %d !!\n", *pu8TSOEng);

#if (DEMO_DMX_TSO_10_TEST == 1)

    DMX_FLOW_INPUT eSrc = DMX_FLOW_INPUT_DEMOD;
    MS_BOOL bParallel = TRUE;

#elif (DEMO_DMX_TSO_20_TEST == 1)

    if (DMX_FILTER_STATUS_OK != MApi_DMX_TSO_SVQBuf_Set(*pu8TSOEng, (MS_PHY) MEM_ADR_BY_MIU(TSO_SVQ_BUF_ADR, TSO_SVQ_BUF_MEMORY_TYPE), TSO_SVQ_BUF_LEN))
    {
        DemoDmx_Print("Set SVQ buffer fail!\n");
        return FALSE;
    }

    #if (DEMO_DMX_3WIRE_TEST == 1)

    DMX_FLOW_INPUT eSrc = DMX_FLOW_INPUT_EXT_INPUT0_3WIRE;
    MS_BOOL bParallel = FALSE;

    #else

    DMX_FLOW_INPUT eSrc = DMX_FLOW_INPUT_EXT_INPUT0;
    MS_BOOL bParallel = TRUE;

    #endif

#endif

    if (DMX_FILTER_STATUS_OK!= MApi_DMX_FlowSet(DmxFlow, eSrc, FALSE, TRUE, bParallel))
    {
        DemoDmx_Print("Switch to demod fail!\n");
        return FALSE;
    }

    if(*pu8Pad == 0)
        MDrv_SYS_SetPadMux(E_TS0_PAD_SET,E_PARALLEL_OUT_TSO);
    else if(*pu8Pad == 1)
        MDrv_SYS_SetPadMux(E_TS1_PAD_SET,E_PARALLEL_OUT_TSO);
    else
        DemoDmx_Print("Unsupproted Pad : %d !!\n", *pu8Pad);

    return TRUE;
}

#endif

#define FILEIN_ALIGN_TO(value, align)  ( ((value) + ((align)-1)) & ~((align)-1) )

MS_BOOL Demo_DMX_MMFI(DMX_MMFI_FLTTYPE MMFI_FltType,MS_U8 PacketMode,MS_U16 Pid,const char *FileName,MS_U64 FileLength)
{
    MS_BOOL ret = TRUE;
    MS_U8 u8DmxId;
    //int fd = -1;
    FILE *fptr = NULL;
    DMX_Filein_info FileinInfo;
    MS_U8 *pBufAddr;
    MS_U32 BufLength;
    DMX_MMFI_DST MMFIDst = DMX_MMFI_PES_NO_BYPASS_TS;
    DMX_FIFO_STATUS FIFO_Status;


    if(MApi_DMX_MMFI_Pid_Open(MMFI_FltType,Pid,&u8DmxId) != DMX_FILTER_STATUS_OK)
    {
        DemoDmx_Print("MApi_DMX_MMFI_Pid_Open() failed!!\n");
        return FALSE;
    }

    if(MApi_DMX_AVFifo_Reset(_Demo_DMX_MMFI_FltTypeInvertMapping(MMFI_FltType & ~DMX_MMFI1_FLTTYPE_MASK),TRUE) != DMX_FILTER_STATUS_OK)
    {
        DemoDmx_Print("MApi_DMX_AVFifo_Reset() failed!!\n");
        ret = FALSE;
        goto exit;
    }

    if(PacketMode == 188)//just support 188/192...
    {
        FileinInfo.Rate = 0;
        FileinInfo.PKT_Mode = DMX_PKTMODE_188;
    }
    else
    {
        FileinInfo.Rate = 0;
        FileinInfo.PKT_Mode = DMX_PKTMODE_192;
    }

    if(MApi_DMX_MMFI_Filein_Info(((MMFI_FltType & DMX_MMFI1_FLTTYPE_MASK)? DMX_MMFI_PATH1 : DMX_MMFI_PATH0), &FileinInfo) != DMX_FILTER_STATUS_OK)
    {
        DemoDmx_Print("MApi_DMX_MMFI_Filein_Info() failed!!\n");
        ret = FALSE;
        goto exit;
    }

    if((fptr = MsFS_Fopen(FileName,"rb")) < 0)
    {
        DemoDmx_Print("MsFS_Open() failed!!\n");
        ret = FALSE;
        goto exit;
    }
    else
    {
        pBufAddr = (MS_U8*)Pes_Buf_Addr[0];
        BufLength = (FileLength > PES_BUFFER_SIZE)? PES_BUFFER_SIZE : (MS_U32)FileLength;
        BufLength = FILEIN_ALIGN_TO(BufLength,PacketMode);
        memset(pBufAddr,0,BufLength * sizeof(MS_U8));

        if(MsFS_Fread(pBufAddr,1,BufLength,fptr) != BufLength)
        {
            DemoDmx_Print("MsFS_Read() size is mismatch!!\n");
            ret = FALSE;
        }

        MsFS_Fclose(fptr);
        if(ret == FALSE)
            goto exit;
    }

    if(MMFI_FltType & DMX_MMFI1_FLTTYPE_MASK)//MMFI #1
        MMFIDst |= DMX_MMFI1_PES_TYPE_MASK;

    if(MApi_DMX_MMFI_Filein_Start(MMFIDst,(MS_PHYADDR)pBufAddr,BufLength) != DMX_FILTER_STATUS_OK)
    {
        DemoDmx_Print("MApi_DMX_MMFI_Filein_Start() failed!!\n");
        ret = FALSE;
        goto exit;
    }

    while(MApi_DMX_MMFI_Filein_IsBusy(((MMFI_FltType & DMX_MMFI1_FLTTYPE_MASK)? DMX_MMFI_PATH1 : DMX_MMFI_PATH0)) == TRUE)
    {
        MsOS_DelayTask(10);
    }

    DemoDmx_Print("Check FIFO status\n");

    if(MApi_DMX_AVFifo_Status(_Demo_DMX_MMFI_FltTypeInvertMapping(MMFI_FltType & ~DMX_MMFI1_FLTTYPE_MASK),&FIFO_Status) != DMX_FILTER_STATUS_OK)
    {
        DemoDmx_Print("MApi_DMX_AVFifo_Status() failed!!\n");
        ret = FALSE;
        goto exit;
    }

    DemoDmx_Print("FIFO State: %"DTC_MS_U32_u"\n", FIFO_Status);
    DemoDmx_Print("FIFO LEVEL: %lu\n", (FIFO_Status& DMX_FIFO_STATUS_LV_USAGE_MASK)>> DMX_FIFO_STATUS_LV_USAGE_SHIFT);
    DemoDmx_Print("FIFO EMPTY: %lu\n", FIFO_Status& DMX_FIFO_STATUS_LV_EMPTY);

exit:
    MApi_DMX_MMFI_Pid_Close(u8DmxId);
    return ret;
}


MS_BOOL Demo_DMX_MMFI_CMD(MS_U8 *pu8MMFIEng,MS_U8 *FltType,MS_U8 *PacketMode,MS_U16 *Pid,const char *FileName)
{
    DMX_MMFI_FLTTYPE MMFI_FltType = DMX_MMFI_FLTTYPE_VD;
    struct stat buf;


    if(*pu8MMFIEng == 0)
    {
        MMFI_FltType &= ~DMX_MMFI1_FLTTYPE_MASK;
    }
    else if(*pu8MMFIEng == 1)
    {
        MMFI_FltType |= DMX_MMFI1_FLTTYPE_MASK;
    }
    else
    {
        DemoDmx_Print("MMFI Eng num not supported!!\n");
        goto fail;
    }

    MMFI_FltType |= Demo_DMX_MMFI_FltTypeMapping(*FltType);//EN_DEMO_DMX_FLT_TYPE -> DMX_MMFI_FLTTYPE

    if(*PacketMode != 188 && *PacketMode != 192)
    {
        DemoDmx_Print("MMFI packet mode not supported!!\n");
        goto fail;
    }

    if(*Pid >= DMX_PID_NULL)//exclude NULL packet
    {
        DemoDmx_Print("MMFI pid filter num not supported!!\n");
        goto fail;
    }

    if(FileName == NULL || strlen(FileName) == 0)
    {
        DemoDmx_Print("FileName is empry!!\n");
        goto fail;
    }
    else
    {
        if(MsFS_Stat(FileName,&buf) < 0)
        {
            DemoDmx_Print("File is not exist!!\n");
            goto fail;
        }
        /*
        else if(buf.st_size == 0)
        {
            DemoDmx_Print("File is empty!!\n");
            goto fail;
        }
        */
    }

    if(Demo_DMX_MMFI(MMFI_FltType,*PacketMode,*Pid,FileName,(MS_U64)buf.st_size))
    {
        DemoDmx_Print("Demo_DMX_MMFI() Success!!\n");
        goto success;
    }
    else
    {
        DemoDmx_Print("Demo_DMX_MMFI() failed!!\n");
        goto fail;
    }

success:
    return TRUE;
fail:
    return FALSE;
}

#define INVALID_TASK_ID         -1

#define IR_TASK_STACK_SIZE      3072
//We choose FILEIN_UNIT_SIZE 0x34E00 == 216576 == 188*192*6 since
//1) the file-in size must be align to pktSize that is 188 or 192
//2) the read size from USB HD is within 128KBytes to 256 KBytes
#define FILEIN_UNIT_SIZE       (0x34E00)
#define FILEIN_BUFS_NUM        (6)

//CMDQ_SLOT_NUM and CMDQ_FILEIN_UNIT_NUM are TSP HW configurations
#define MMFI_CMDQ_SLOT_NUM     (8)
#define FILE_CMDQ_SLOT_NUM     (16)
#define CMDQ_FILEIN_UNIT_NUM   (5)

//#define DEMO_DMX_MMFI_MAX_ENGNUM 2
#define DEMO_DMX_MAX_ENGNUM    6

typedef struct
{
    MS_U32  *pu32BufferArray,*pu32SizeArray;
    MS_U32  u32ArrayNum;
    MS_U32  u32BufferLength;
    MS_U32  u32ReadPos,u32WritePos;
    MS_U8   u8CMDQ_Slot_Num;
    MS_BOOL bInit;
} TSP_RING_BUFFER;

typedef struct
{
    FILE                        *FilePointer;
    EN_DMX_FILEIN_FILE_FORMAT eFormat;
    TSP_RING_BUFFER             stRingBuffer;
    MS_U32                      u32FileSize;
    MS_U8                       u8PacketSize;
} _DEMO_DMX_BUF_INFO;

typedef struct
{
    MS_S32                   s32FileInTaskId,s32ReadFileTaskId;
    void                     *FileInStack,*ReadFileStack;
    MS_BOOL                  bFileIn_Enable,bFileIn_Count;
    MS_BOOL                  bReadFile_Enable,bReadFile_Count;
    EN_DMX_FILEIN_ENG_TYPE FileIn_EngType;
    MS_U8                    u8FileIn_EngID;
} _DEMO_DMX_TASK_INFO;

typedef struct
{
    MS_BOOL (*IsEngineBusy)(MS_U8);
    DMX_FILTER_STATUS (*GetCMDQEmptyNum)(MS_U8,MS_U32*);
    DMX_FILTER_STATUS (*EngineStart)(DMX_MMFI_DST,MS_U8,MS_PHYADDR,MS_U32);
} _DEMO_DMX_FUNCTION_SET;

static _DEMO_DMX_BUF_INFO FileIn_BufInfo[DEMO_DMX_MAX_ENGNUM] = {
                                                                 {NULL,E_FILE_NOT_SUPPORT,{.bInit = FALSE, .u32ArrayNum = 0, .u32BufferLength = 0, .u32ReadPos = 0, .u32WritePos = 0, .u8CMDQ_Slot_Num = 0},-1,-1},
                                                                 {NULL,E_FILE_NOT_SUPPORT,{.bInit = FALSE, .u32ArrayNum = 0, .u32BufferLength = 0, .u32ReadPos = 0, .u32WritePos = 0, .u8CMDQ_Slot_Num = 0},-1,-1},
                                                                 {NULL,E_FILE_NOT_SUPPORT,{.bInit = FALSE, .u32ArrayNum = 0, .u32BufferLength = 0, .u32ReadPos = 0, .u32WritePos = 0, .u8CMDQ_Slot_Num = 0},-1,-1},
                                                                 {NULL,E_FILE_NOT_SUPPORT,{.bInit = FALSE, .u32ArrayNum = 0, .u32BufferLength = 0, .u32ReadPos = 0, .u32WritePos = 0, .u8CMDQ_Slot_Num = 0},-1,-1},
                                                                 {NULL,E_FILE_NOT_SUPPORT,{.bInit = FALSE, .u32ArrayNum = 0, .u32BufferLength = 0, .u32ReadPos = 0, .u32WritePos = 0, .u8CMDQ_Slot_Num = 0},-1,-1},
                                                                 {NULL,E_FILE_NOT_SUPPORT,{.bInit = FALSE, .u32ArrayNum = 0, .u32BufferLength = 0, .u32ReadPos = 0, .u32WritePos = 0, .u8CMDQ_Slot_Num = 0},-1,-1}
                                                                };
static _DEMO_DMX_TASK_INFO FileIn_TaskInfo[DEMO_DMX_MAX_ENGNUM] = {
                                                                   {INVALID_TASK_ID,INVALID_TASK_ID,NULL,NULL,FALSE,FALSE,FALSE,FALSE,E_ENG_NOT_SUPPORT,0},
                                                                   {INVALID_TASK_ID,INVALID_TASK_ID,NULL,NULL,FALSE,FALSE,FALSE,FALSE,E_ENG_NOT_SUPPORT,0},
                                                                   {INVALID_TASK_ID,INVALID_TASK_ID,NULL,NULL,FALSE,FALSE,FALSE,FALSE,E_ENG_NOT_SUPPORT,0},
                                                                   {INVALID_TASK_ID,INVALID_TASK_ID,NULL,NULL,FALSE,FALSE,FALSE,FALSE,E_ENG_NOT_SUPPORT,0},
                                                                   {INVALID_TASK_ID,INVALID_TASK_ID,NULL,NULL,FALSE,FALSE,FALSE,FALSE,E_ENG_NOT_SUPPORT,0},
                                                                   {INVALID_TASK_ID,INVALID_TASK_ID,NULL,NULL,FALSE,FALSE,FALSE,FALSE,E_ENG_NOT_SUPPORT,0}
                                                                  };

inline static DMX_FILTER_STATUS _Demo_DMX_Engine_Start(DMX_MMFI_DST eMMFIDst,MS_U8 u8EngineID,MS_PHYADDR PhyAddr,MS_U32 u32Size)
{
    DMX_FILTER_STATUS bRet;

    if(eMMFIDst != 0) // MMFI
    {
        bRet = MApi_DMX_MMFI_Filein_Start(eMMFIDst,PhyAddr,u32Size);
    }
    else
    {
        bRet = MApi_DMX_Filein_Eng_Start(u8EngineID,DMX_PES_NO_BYPASS_FIFO,PhyAddr,u32Size);
    }

    return bRet;
}

static _DEMO_DMX_FUNCTION_SET stFunctionSet = {
                                                .IsEngineBusy = NULL,
                                                .GetCMDQEmptyNum = NULL,
                                                .EngineStart = _Demo_DMX_Engine_Start
                                              };

void* _Demo_DMX_MemAlloc_Func(MS_U32 u32Size)
{
    MS_S32 s32NonCachedPoolID = 0;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32NonCachedPoolID);
    return MsOS_AllocateMemory (u32Size, s32NonCachedPoolID);
}

MS_BOOL _Demo_DMX_MemFree_Func(void *pBuf)
{
    MS_S32 s32NonCachedPoolID = 0;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32NonCachedPoolID);
    return MsOS_FreeMemory(pBuf, s32NonCachedPoolID);
}

MS_U32 _Ring_Buffer_Init(TSP_RING_BUFFER *pstRingBuffer, MS_U32 u32BufferNum, MS_U32 u32BufferLength,MS_U8 u8CMDQ_Slot_Num)
{
    MS_U32 i = 0;

    if(pstRingBuffer->bInit == TRUE)
    {
        DemoDmx_Print("The ring buffer is already initialized !!\n");
        return FALSE;
    }

    pstRingBuffer->pu32BufferArray  = _Demo_DMX_MemAlloc_Func(u32BufferNum * sizeof(MS_U32));
    if(pstRingBuffer->pu32BufferArray == NULL)
    {
        DemoDmx_Print("Memory allocation failed !!\n");
        goto fail;
    }

    pstRingBuffer->pu32SizeArray = _Demo_DMX_MemAlloc_Func(u32BufferNum * sizeof(MS_U32));
    if(pstRingBuffer->pu32SizeArray == NULL)
    {
        DemoDmx_Print("Memory allocation failed !!\n");
        goto fail;
    }

    for(i = 0; i < u32BufferNum; i++)
    {
        *(pstRingBuffer->pu32BufferArray + i)  = (MS_U32)_Demo_DMX_MemAlloc_Func(u32BufferLength);
        *(pstRingBuffer->pu32SizeArray + i) = 0;
    }

    pstRingBuffer->u32ArrayNum = u32BufferNum;
    pstRingBuffer->u32BufferLength = u32BufferLength;
    pstRingBuffer->u32ReadPos = 0;
    pstRingBuffer->u32WritePos = 0;
    pstRingBuffer->bInit = TRUE;
    pstRingBuffer->u8CMDQ_Slot_Num = u8CMDQ_Slot_Num;
    return TRUE;

fail:
    for(i = 0; i < u32BufferNum; i++)
    {
        _Demo_DMX_MemFree_Func((void*)*(pstRingBuffer->pu32BufferArray + i));
    }
    _Demo_DMX_MemFree_Func(pstRingBuffer->pu32BufferArray);
    _Demo_DMX_MemFree_Func(pstRingBuffer->pu32SizeArray);
    return FALSE;
}

MS_U32 _Ring_Buffer_Exit(TSP_RING_BUFFER *pstRingBuffer)
{
    MS_U32 i = 0;

    if(pstRingBuffer->bInit == FALSE)
    {
        DemoDmx_Print("The ring buffer has not been initialized yet !!\n");
        return FALSE;
    }

    pstRingBuffer->bInit = FALSE;

    for(i = 0; i < pstRingBuffer->u32ArrayNum; i++)
    {
        _Demo_DMX_MemFree_Func((void*)(*(pstRingBuffer->pu32BufferArray + i)));
    }

    _Demo_DMX_MemFree_Func(pstRingBuffer->pu32BufferArray);
    _Demo_DMX_MemFree_Func(pstRingBuffer->pu32SizeArray);

    pstRingBuffer->u32ArrayNum     = 0;
    pstRingBuffer->u32BufferLength = 0;
    pstRingBuffer->u32ReadPos      = 0;
    pstRingBuffer->u32WritePos     = 0;
    pstRingBuffer->u8CMDQ_Slot_Num = 0;
    return TRUE;
}

MS_U32 _Ring_Buffer_Empty(TSP_RING_BUFFER *pstRingBuffer)
{
    if(pstRingBuffer->bInit == FALSE)
    {
        DemoDmx_Print("The ring buffer has not been initialized yet !!\n");
        return 0;
    }

    MS_U32 u32ReadPos  = pstRingBuffer->u32ReadPos;
    MS_U32 u32WritePos = pstRingBuffer->u32WritePos;
    MS_U32 u32ArrayNum = pstRingBuffer->u32ArrayNum;

    if(u32WritePos < u32ReadPos)
    {
        return u32ReadPos - u32WritePos - 1;
    }
    else
    {
        return u32ArrayNum - u32WritePos + u32ReadPos - 0 - 1;
    }
}

MS_U32 _Ring_Buffer_WaterLevel(TSP_RING_BUFFER *pstRingBuffer)
{
    if(pstRingBuffer->bInit == FALSE)
    {
        DemoDmx_Print("The ring buffer has not been initialized yet !!\n");
        return 0;
    }

    MS_U32 u32ReadPos  = pstRingBuffer->u32ReadPos;
    MS_U32 u32WritePos = pstRingBuffer->u32WritePos;
    MS_U32 u32ArrayNum = pstRingBuffer->u32ArrayNum;

    if(u32ReadPos <= u32WritePos)
    {
        return u32WritePos - u32ReadPos;
    }
    else
    {
        return u32ArrayNum - u32ReadPos + u32WritePos - 0;
    }
}

MS_BOOL _Ring_Buffer_WriteFromFile(TSP_RING_BUFFER *pstRingBuffer, FILE *FilePointer)
{
    MS_U32 u32Amount = 0;


    if(pstRingBuffer->bInit == FALSE)
    {
        DemoDmx_Print("The ring buffer has not been initialized yet.\n");
        return FALSE;
    }

    if(_Ring_Buffer_Empty(pstRingBuffer) > 0)
    {
        u32Amount = MsFS_Fread((void*)(*(pstRingBuffer->pu32BufferArray + pstRingBuffer->u32WritePos)), 1, pstRingBuffer->u32BufferLength, FilePointer);
        if(u32Amount > 0)
        {
            *(pstRingBuffer->pu32SizeArray + pstRingBuffer->u32WritePos) = u32Amount;
            pstRingBuffer->u32WritePos = (pstRingBuffer->u32WritePos + 1) % pstRingBuffer->u32ArrayNum;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return TRUE;
    }
}

MS_BOOL _Ring_Buffer_ReadFromBuffer(TSP_RING_BUFFER *pstRingBuffer, MS_U32 *pu32BufferAddr, MS_U32 *pu32BufferSize)
{
    if(pstRingBuffer->bInit == FALSE)
    {
        DemoDmx_Print("The ring buffer has not been initialized yet !!\n");
        return FALSE;
    }

    if(_Ring_Buffer_WaterLevel(pstRingBuffer) > 0)
    {
        *pu32BufferAddr  = *(pstRingBuffer->pu32BufferArray + pstRingBuffer->u32ReadPos);
        *pu32BufferSize = *(pstRingBuffer->pu32SizeArray + pstRingBuffer->u32ReadPos);
        pstRingBuffer->u32ReadPos = (pstRingBuffer->u32ReadPos + 1) % pstRingBuffer->u32ArrayNum;
        return TRUE;
    }
    else
    {
        *pu32BufferAddr  = (MS_U32)NULL;
        *pu32BufferSize = (MS_U32)NULL;
        return FALSE;
    }
}

void _appDemo_ReadFile_task(MS_U32 u32Index)
{
    MS_BOOL bRet = FALSE;
    MS_U32  u32FileInNum = 0;
    MS_U32  u32EmptyCmdQSlot = 0;


    DemoDmx_Print("index = %"DTC_MS_U32_u" !!\n", u32Index);

    while(FileIn_TaskInfo[u32Index].bReadFile_Enable)
    {
        bRet = stFunctionSet.IsEngineBusy(FileIn_TaskInfo[u32Index].u8FileIn_EngID);

        if(bRet)
        {
            bRet = stFunctionSet.GetCMDQEmptyNum(FileIn_TaskInfo[u32Index].u8FileIn_EngID,&u32EmptyCmdQSlot);
            u32FileInNum = (FileIn_BufInfo[u32Index].stRingBuffer.u8CMDQ_Slot_Num - u32EmptyCmdQSlot)/CMDQ_FILEIN_UNIT_NUM + 1;
        }
        else
        {
            u32FileInNum = 0;
        }

        if(_Ring_Buffer_Empty(&FileIn_BufInfo[u32Index].stRingBuffer) > u32FileInNum)
        {
            bRet = _Ring_Buffer_WriteFromFile(&FileIn_BufInfo[u32Index].stRingBuffer,FileIn_BufInfo[u32Index].FilePointer);

            if(bRet == FALSE)
            {
                DemoDmx_Print("file end\n");
                break;
            }
        }
        else
        {
            MsOS_DelayTaskUs(50);
        }
    }

    FileIn_TaskInfo[u32Index].bReadFile_Count = FALSE;
}

void _appDemo_FileIn_task(MS_U32 u32Index)
{
    MS_BOOL bRet = FALSE;
    MS_U32  u32BufferAddr = (MS_U32)NULL;
    MS_U32  u32BufferSize = 0;
    MS_U32  u32EmptyCmdQSlot;
    MS_U32  u32Residual = 0;
    DMX_MMFI_DST eMMFIDst = 0;


    DemoDmx_Print("index = %"DTC_MS_U32_u" !!\n", u32Index);

    if(FileIn_TaskInfo[u32Index].FileIn_EngType == E_ENG_MMFI)
    {
        switch(FileIn_BufInfo[u32Index].eFormat)
        {
            case E_FILE_TS:
                /*eMMFIDst = DMX_MMFI_PES_APID_BYPASS
                         | DMX_MMFI_PES_APIDB_BYPASS
                         | DMX_MMFI_PES_VPID_BYPASS; */
                eMMFIDst = DMX_MMFI_PES_VPID_BYPASS;
                break;
            case E_FILE_PES:
                eMMFIDst = DMX_MMFI_PES_NO_BYPASS_TS;
                break;
            case E_FILE_PS:
                /*eMMFIDst = DMX_MMFI_PES_PS_AU
                         | DMX_MMFI_PES_PS_AUB
                         | DMX_MMFI_PES_PS_VD;*/
                eMMFIDst = DMX_MMFI_PES_PS_VD;
                break;
            default:
                DemoDmx_Print("No such format !!\n");
                break;
        }

        if(FileIn_TaskInfo[u32Index].u8FileIn_EngID == 1) //MMFI #1
        {
            eMMFIDst |= DMX_MMFI1_PES_TYPE_MASK;
        }
    }

    while(FileIn_TaskInfo[u32Index].bFileIn_Enable)
    {
        bRet = stFunctionSet.GetCMDQEmptyNum(FileIn_TaskInfo[u32Index].u8FileIn_EngID,&u32EmptyCmdQSlot);

        if((_Ring_Buffer_WaterLevel(&FileIn_BufInfo[u32Index].stRingBuffer) > 0) && (u32EmptyCmdQSlot > CMDQ_FILEIN_UNIT_NUM))
        {
            bRet = _Ring_Buffer_ReadFromBuffer(&FileIn_BufInfo[u32Index].stRingBuffer, &u32BufferAddr, &u32BufferSize);

            if(bRet == TRUE)
            {
                u32Residual = u32BufferSize % FileIn_BufInfo[u32Index].u8PacketSize;

                if(u32Residual != 0)
                {
                    u32BufferSize -= u32Residual;
                }

                MsOS_FlushMemory();
                bRet = stFunctionSet.EngineStart(eMMFIDst,FileIn_TaskInfo[u32Index].u8FileIn_EngID,(MS_PHYADDR)MS_VA2PA(u32BufferAddr), u32BufferSize);
            }
            else
            {
                DemoDmx_Print("_bufs_ring_rd() failed\n");
            }
        }
        else
        {
            MsOS_DelayTaskUs(50);
        }
    }

    FileIn_TaskInfo[u32Index].bFileIn_Count = FALSE;
}

inline static EN_DMX_FILEIN_FILE_FORMAT _Demo_DMX_FileIn_Format_Mapping(MS_U8 *pu8Format)
{
    if(_Demo_DMX_strcmp(pu8Format, "TS")==0 )      //remove ts header
    {
        return E_FILE_TS;
    }
    else if(_Demo_DMX_strcmp(pu8Format, "PES")==0 )//normal
    {
        return E_FILE_PES;
    }
    else if(_Demo_DMX_strcmp(pu8Format, "PS")==0 ) //PS path
    {
        return E_FILE_PS;
    }
    else
    {
        return E_FILE_NOT_SUPPORT;
    }
}

inline static MS_S8 _Demo_DMX_FileIn_GetFreeIndex(void)
{
    MS_S8 s8Index = 0;

    for(s8Index = 0;s8Index < DEMO_DMX_MAX_ENGNUM;++s8Index)
    {
        if((FileIn_TaskInfo[s8Index].s32FileInTaskId == INVALID_TASK_ID) && (FileIn_TaskInfo[s8Index].s32ReadFileTaskId == INVALID_TASK_ID))
        {
            break;
        }
    }

    if(s8Index >= DEMO_DMX_MAX_ENGNUM)
    {
        return -1;
    }
    else
    {
        return s8Index;
    }
}

inline static MS_S8 _Demo_DMX_FileIn_GetMappingIndex(EN_DMX_FILEIN_ENG_TYPE eDmxEngineType,MS_U8 *pu8EngineID)
{
    MS_S8 s8Index = 0;

    for(s8Index = 0;s8Index < DEMO_DMX_MAX_ENGNUM;++s8Index)
    {
        if((FileIn_TaskInfo[s8Index].FileIn_EngType == eDmxEngineType) && (FileIn_TaskInfo[s8Index].u8FileIn_EngID == *pu8EngineID))
        {
            break;
        }
    }

    if(s8Index >= DEMO_DMX_MAX_ENGNUM)
    {
        return -1;
    }
    else
    {
        return s8Index;
    }
}

inline static MS_U8 is_big_endian(void)
{
    union {
        MS_U32 i;
        MS_U8 c[4];
    } e = { 0x01000000 };

    return e.c[0];
}


MS_BOOL Demo_DMX_FileIn_Start(MS_U8 *pu8DmxEngine,MS_U8 *pu8EngineID, const char *ps8FileName, MS_U8 *pu8Format, MS_U8 *pu8PacketMode)
{
    EN_DMX_FILEIN_FILE_FORMAT eFormat = _Demo_DMX_FileIn_Format_Mapping(pu8Format);
    struct stat buf;
    DMX_Filein_info tsFileInInfo;
    MS_BOOL bByPass = TRUE;
    MS_S8 s8Index = 0;
    MS_U32 u32InitPlaybackTimestamp = 0;
    char s8TaskName[30] = {0};
    MS_U8 u8CMDQ_SlotNum = 0;


    if((_Demo_DMX_strcmp(pu8DmxEngine,"MMFI") != 0) && (_Demo_DMX_strcmp(pu8DmxEngine,"FILE") != 0))
    {
        DemoDmx_Print("Wrong DMX Engine type !!\n");
        goto fail;
    }

    if(eFormat == E_FILE_NOT_SUPPORT)
    {
        DemoDmx_Print("Wrong DMX FileIn format !!\n");
        goto fail;
    }

    if(ps8FileName == NULL || strlen(ps8FileName) == 0)
    {
        DemoDmx_Print("FileName is empty !!\n");
        goto fail;
    }
    else
    {
        DemoDmx_Print("FileName is %s !!\n",ps8FileName);

        if(MsFS_Stat(ps8FileName,&buf) < 0)
        {
            DemoDmx_Print("File is not exist !!\n");
            perror("[Error]");
            goto fail;
        }
    #if 0
        else if(buf.st_size == 0)
        {
            DemoDmx_Print("File is empty !!\n");
            goto fail;
        }
    #endif
    }

    s8Index = _Demo_DMX_FileIn_GetFreeIndex();
    DemoDmx_Print("index = %d !!\n",s8Index);

    if(s8Index < 0)
    {
        DemoDmx_Print("index allocation fail !!\n");
        goto fail;
    }

    if((FileIn_BufInfo[s8Index].FilePointer = MsFS_Fopen(ps8FileName,"rb")) < 0)
    {
        DemoDmx_Print("MsFS_Open() fail !!\n");
        goto fail;
    }
    else if(*pu8PacketMode == 192) //get 1st packet timestamp
    {
        MsFS_Fread((void*)(&u32InitPlaybackTimestamp), sizeof(MS_U32), 1, FileIn_BufInfo[s8Index].FilePointer);

        if(!is_big_endian())// little-endian ?
        {
            MS_U8  i;
            MS_U32 u32Temp = 0;

            // big-endian -> little-endian
            for (i = 0; i < sizeof(MS_U32); i++)
                ((MS_U8*)(&u32Temp))[i] = ((MS_U8*)(&u32InitPlaybackTimestamp))[sizeof(MS_U32) - i - 1];

            u32InitPlaybackTimestamp = u32Temp;
        }
    }

    FileIn_BufInfo[s8Index].eFormat = eFormat;
    FileIn_BufInfo[s8Index].u8PacketSize = *pu8PacketMode;
    MsFS_Fseek(FileIn_BufInfo[s8Index].FilePointer,0,SEEK_END);
    FileIn_BufInfo[s8Index].u32FileSize = (MS_U32)MsFS_Ftell(FileIn_BufInfo[s8Index].FilePointer);
    FileIn_BufInfo[s8Index].u32FileSize = FILEIN_ALIGN_TO(FileIn_BufInfo[s8Index].u32FileSize,*pu8PacketMode);
    DemoDmx_Print("File Length = %"DTC_MS_U32_u" !!\n", FileIn_BufInfo[s8Index].u32FileSize);
    MsFS_Rewind(FileIn_BufInfo[s8Index].FilePointer);

    switch(*pu8PacketMode)
    {
        case 188:
            bByPass = TRUE;
            tsFileInInfo.PKT_Mode = DMX_PKTMODE_188;
            tsFileInInfo.Rate = 0xA;
            break;
        case 192:
            bByPass = FALSE;
            tsFileInInfo.PKT_Mode = DMX_PKTMODE_192;
            tsFileInInfo.Rate = 0xA;
            break;
        case 204:
            bByPass = TRUE;
            tsFileInInfo.PKT_Mode = DMX_PKTMODE_204;
            tsFileInInfo.Rate = 0xA;
            break;
        default:
            DemoDmx_Print("Packet mode not support !!\n");
            break;
    }

    if(_Demo_DMX_strcmp(pu8DmxEngine,"FILE")==0) //FileIn
    {
        if(eFormat != E_FILE_PS)
        {
            DMX_FLOW FileInFlow;
            DMX_FILEIN_PATH FileInPath;

            //flowSet of file-in mode
            switch(*pu8EngineID)
            {
                case 0:
                    FileInFlow = DMX_FLOW_PLAYBACK;
                    FileInPath = DMX_FILEIN_TSIF0;
                    break;
                case 1:
                    FileInFlow = DMX_FLOW_PLAYBACK1;
                    FileInPath = DMX_FILEIN_TSIF1;
                    break;
                case 2:
                    FileInFlow = DMX_FLOW_PLAYBACK2;
                    FileInPath = DMX_FILEIN_TSIF2;
                    break;
                case 3:
                    FileInFlow = DMX_FLOW_PLAYBACK3;
                    FileInPath = DMX_FILEIN_TSIF3;
                    break;
                default:
                    FileInFlow = DMX_FLOW_PLAYBACK;
                    FileInPath = DMX_FILEIN_TSIF0;
                    break;
            }

            MApi_DMX_FlowSet(FileInFlow, DMX_FLOW_INPUT_MEM, FALSE, FALSE, TRUE);
            MApi_DMX_Filein_Eng_Info(FileInPath,&tsFileInInfo);

            if(*pu8PacketMode == 192)
            {
                MApi_DMX_Filein_Eng_PlaybackTimeStampEnable(FileInPath);
                MApi_DMX_Filein_Eng_BypassFileInTimeStamp(FileInPath,bByPass);
                MApi_DMX_Filein_Eng_SetPlaybackStamp(FileInPath, u32InitPlaybackTimestamp);
            }
        }

        FileIn_TaskInfo[s8Index].FileIn_EngType = E_ENG_FILE;
        u8CMDQ_SlotNum = FILE_CMDQ_SLOT_NUM;

        stFunctionSet.IsEngineBusy = (void*)MApi_DMX_Filein_Eng_IsBusy;
        stFunctionSet.GetCMDQEmptyNum = (void*)MApi_DMX_Filein_Eng_CMDQ_GetEmptyNum;
    }
    else if(_Demo_DMX_strcmp(pu8DmxEngine,"MMFI")==0) //MMFI
    {
        if(eFormat != E_FILE_PS)
        {
            DMX_MMFI_PATH MMFI_Path = (*pu8EngineID == 0)? DMX_MMFI_PATH0 : DMX_MMFI_PATH1;

            MApi_DMX_MMFI_Filein_Info(MMFI_Path,&tsFileInInfo);

            if(*pu8PacketMode == 192)
            {
                MApi_DMX_MMFI_TimeStampEnable(MMFI_Path);
                MApi_DMX_MMFI_Filein_BypassTimeStamp(MMFI_Path,bByPass);
                MApi_DMX_MMFI_SetPlaybackTimeStamp(MMFI_Path, u32InitPlaybackTimestamp);
            }
        }

        FileIn_TaskInfo[s8Index].FileIn_EngType = E_ENG_MMFI;
        u8CMDQ_SlotNum = MMFI_CMDQ_SLOT_NUM;

        stFunctionSet.IsEngineBusy = (void*)MApi_DMX_MMFI_Filein_IsBusy;
        stFunctionSet.GetCMDQEmptyNum = (void*)MApi_DMX_MMFI_Filein_CMDQ_GetEmptyNum;
    }
    else
    {
        DemoDmx_Print("No need to setup File-In info !!\n");
    }

    //bufs-ring init
    if(!_Ring_Buffer_Init(&FileIn_BufInfo[s8Index].stRingBuffer, FILEIN_BUFS_NUM, FILEIN_UNIT_SIZE, u8CMDQ_SlotNum))
    {
        DemoDmx_Print("Allocate ring buffer fail !!\n");
        goto fail;
    }

    FileIn_TaskInfo[s8Index].u8FileIn_EngID = *pu8EngineID;

#if 0
    //disable AV FIFO reset
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, FALSE);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, FALSE);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO2, TRUE);
#endif

    //create task
    sprintf(s8TaskName,"ReadFileTask_%s_%d",pu8DmxEngine,*pu8EngineID);
    printf("create Task: %s !!\n",s8TaskName);
    FileIn_TaskInfo[s8Index].ReadFileStack = _Demo_DMX_MemAlloc_Func(IR_TASK_STACK_SIZE);
    if(FileIn_TaskInfo[s8Index].ReadFileStack == NULL)
    {
        DemoDmx_Print("Allocate ReadFile Stack fail !!\n");
        goto fail;
    }
    FileIn_TaskInfo[s8Index].bReadFile_Enable  = TRUE;
    FileIn_TaskInfo[s8Index].bReadFile_Count   = TRUE;
    FileIn_TaskInfo[s8Index].s32ReadFileTaskId = MsOS_CreateTask((TaskEntry)_appDemo_ReadFile_task,
                                                                 s8Index,
                                                                 E_TASK_PRI_MEDIUM,
                                                                 TRUE,
                                                                 FileIn_TaskInfo[s8Index].ReadFileStack,
                                                                 IR_TASK_STACK_SIZE,
                                                                 s8TaskName);
    if(FileIn_TaskInfo[s8Index].s32ReadFileTaskId == INVALID_TASK_ID)
    {
        FileIn_TaskInfo[s8Index].bReadFile_Enable = FALSE;
        FileIn_TaskInfo[s8Index].bReadFile_Count  = FALSE;
        DemoDmx_Print("Create ReadFile Task fail !!\n");
        goto fail;
    }

    MsOS_DelayTask(5);

    sprintf(s8TaskName,"FileInTask_%s_%d",pu8DmxEngine,*pu8EngineID);
    printf("create Task: %s !!\n",s8TaskName);
    FileIn_TaskInfo[s8Index].FileInStack = _Demo_DMX_MemAlloc_Func(IR_TASK_STACK_SIZE);
    if(FileIn_TaskInfo[s8Index].FileInStack == NULL)
    {
        DemoDmx_Print("Allocate FileIn Stack fail !!\n");
        goto fail;
    }
    FileIn_TaskInfo[s8Index].bFileIn_Enable   = TRUE;
    FileIn_TaskInfo[s8Index].bFileIn_Count    = TRUE;
    FileIn_TaskInfo[s8Index].s32FileInTaskId  = MsOS_CreateTask((TaskEntry)_appDemo_FileIn_task,
                                                                s8Index,
                                                                E_TASK_PRI_MEDIUM,
                                                                TRUE,
                                                                FileIn_TaskInfo[s8Index].FileInStack,
                                                                IR_TASK_STACK_SIZE,
                                                                s8TaskName);

    if(FileIn_TaskInfo[s8Index].s32FileInTaskId == INVALID_TASK_ID)
    {
        FileIn_TaskInfo[s8Index].bFileIn_Enable   = FALSE;
        FileIn_TaskInfo[s8Index].bFileIn_Count = FALSE;
        DemoDmx_Print("Create FileIn task fail !!\n");
    }

    return TRUE;
fail:
    Demo_DMX_FileIn_Stop(pu8DmxEngine,pu8EngineID);
    return FALSE;
}

MS_BOOL Demo_DMX_FileIn_Stop(MS_U8 *pu8DmxEngine,MS_U8 *pu8EngineID)
{
    EN_DMX_FILEIN_ENG_TYPE eEngType;
    MS_S8 s8Index;


    if(_Demo_DMX_strcmp(pu8DmxEngine,"MMFI")==0)
    {
        eEngType = E_ENG_MMFI;
    }
    else if(_Demo_DMX_strcmp(pu8DmxEngine,"FILE")==0)
    {
        eEngType = E_ENG_FILE;
    }
    else
    {
        DemoDmx_Print("DMX engine not support !!\n");
        return FALSE;
    }

    s8Index = _Demo_DMX_FileIn_GetMappingIndex(eEngType,pu8EngineID);

    DemoDmx_Print("index = %u !!\n",s8Index);

    if(s8Index < 0)
    {
        DemoDmx_Print("Index is out of range !!\n");
        return FALSE;
    }

    //delete task
    if(FileIn_TaskInfo[s8Index].s32FileInTaskId != INVALID_TASK_ID)
    {
        FileIn_TaskInfo[s8Index].bFileIn_Enable = FALSE;

        while(FileIn_TaskInfo[s8Index].bFileIn_Count)
        {
            MsOS_DelayTask(1);
        }

        MsOS_DeleteTask(FileIn_TaskInfo[s8Index].s32FileInTaskId);
        FileIn_TaskInfo[s8Index].s32FileInTaskId = INVALID_TASK_ID;
    }

    if(FileIn_TaskInfo[s8Index].s32ReadFileTaskId != INVALID_TASK_ID)
    {
        FileIn_TaskInfo[s8Index].bReadFile_Enable = FALSE;

        while(FileIn_TaskInfo[s8Index].bReadFile_Count)
        {
            MsOS_DelayTask(1);
        }

        MsOS_DeleteTask(FileIn_TaskInfo[s8Index].s32ReadFileTaskId);
        FileIn_TaskInfo[s8Index].s32ReadFileTaskId = INVALID_TASK_ID;
    }

#if 0
    //AV FIFO reset
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, TRUE);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, TRUE);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO2, TRUE);
#endif

    _Ring_Buffer_Exit(&FileIn_BufInfo[s8Index].stRingBuffer);

    if(FileIn_BufInfo[s8Index].FilePointer != NULL)
    {
        MsFS_Fclose(FileIn_BufInfo[s8Index].FilePointer);
    }
    FileIn_BufInfo[s8Index].FilePointer = NULL;
    FileIn_BufInfo[s8Index].eFormat = E_FILE_NOT_SUPPORT;
    FileIn_BufInfo[s8Index].u32FileSize = -1;
    FileIn_BufInfo[s8Index].u8PacketSize = -1;
    _Demo_DMX_MemFree_Func(FileIn_TaskInfo[s8Index].ReadFileStack);
    _Demo_DMX_MemFree_Func(FileIn_TaskInfo[s8Index].FileInStack);
    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief The help function for demo demux app
/// @return TRUE: success / FALSE: fail
/// @sa
/// @note
/// Command: \b HelpDMX \n
//------------------------------------------------------------------------------
MS_BOOL Demo_DMX_Help(void)
{

    printf("*************************Demux function List*************************\n");
    #if 0
    printf("1. DMX_Pat\n");
    printf("Usage:dmx_pat\n");
    printf("\n");
    printf("Sample UART Command:dmx_pat\n");
    printf("\n");
    printf("---------------------------------------------------------------------\n");
    printf("2. DMX_Pmt\n");
    printf("Usage:dmx_pmt\n");
    printf("\n");
    printf("Sample UART Command:dmx_Pmt PID \n");
    printf("\n");
    #endif
    printf("---------------------------------------------------------------------\n");
    printf("1. DMX_Sec\n");
    printf("Usage:dmx_sec [DMX Flow] [Pid] [Pattern0] [Pattern1] [Mask0] [Mask1]\n");
    printf("   DMX_Flow: LIVE0~LIVE3  *Note:LIVE flow number support depend on chip.\n");
    printf("   Pid:      Pid filter in hex\n");
    printf("   Pattern0: match pattern from byte 0~3 in hex\n");
    printf("   Pattern1: match pattern from byte 4~7 in hex\n");
    printf("   mask0:    mask from byte 0~3 in hex\n");
    printf("   mask1:    mask from byte 4~7 in hex\n");
    printf("   *Note: our TSP support 16 bytes patterns and mask.\n");
    printf("          In this demo program, however, 2 bytes are only used for demo correctness\n");
    printf("\n");
    printf("Sample UART Command:DMX_Sec LIVE0  0x0         0x0 0x0 0xFF000000 0x0\n");
    printf("Sample UART Command:DMX_Sec LIVE0 [Pid] 0x02000000 0x0 0xFF000000 0x0\n");
    printf("\n");
    printf("---------------------------------------------------------------------\n");
    printf("2. DMX_Scan : Scan Demo code\n");
    printf("Usage:dmx_scan [DMX Flow]\n");
    printf("   DMX_Flow: LIVE0~LIVE3  Note:LIVE flow number support depend on chip.\n");
    printf("   Scan programs of current frequency\n");
    printf("Sample UART Command: dmx_scan LIVE0\n");
    printf("\n");
    printf("---------------------------------------------------------------------\n");
    printf("3. DMX_Nmask : demo code for not Match mask \n");
    printf("Usage:dmx_Nmask [DMX Flow] [pattern] [mask] [nmask] \n");
    printf("   DMX_Flow: FILE0~FILE3  Note:FILE flow number support depend on chip.\n");
    printf("\n");
    printf("Sample UART Command: dmx_nmask FILE0 40800000 FFFF0000 00000000\n");
    printf("                     dmx_nmask FILE0 40800000 FFFF0000 F0000000\n");
    printf("                     dmx_nmask FILE0 40800000 FFFF0000 0F000000\n");
    printf("                     dmx_nmask FILE0 40800000 FFFF0000 FF000000\n");
    printf("---------------------------------------------------------------------\n");
    printf("4. DMX_Pcr\n");
    printf("Usage:dmx_pcr [DMX Flow] [PID]\n");
    printf("   DMX_Flow: LIVE0~LIVE3  Note:LIVE flow number support depend on chip.\n");
    printf("\n");
    printf("Sample UART Command: dmx_pcr LIVE0 [Pid] \n");
    printf("\n");
    printf("---------------------------------------------------------------------\n");
    printf("5. DMX_Pes : Filter PES packet data into buffer/\n");
    printf("Usage:dmx_Pes [DMX Flow] [PID]  \n");
    printf("   DMX_Flow: LIVE0~LIVE3  Note:LIVE flow number support depend on chip.\n");
    printf("\n");
    printf("Sample UART Command: dmx_pes 0x302\n");
    printf("---------------------------------------------------------------------\n");
    printf("6. DMX_Pkt : filter target Pid Packet into buffer. \n");
    printf("Usage:dmx_pkt [DMX Flow] [PID]\n");
    printf("   DMX_Flow: LIVE0~LIVE3  Note:LIVE flow number support depend on chip.\n");
    printf("\n");
    printf("Sample UART Command: dmx_pkt  [Pid] \n");
    printf("---------------------------------------------------------------------\n");
    #if 0
    printf("9. DMX_FileIn_Start : start to file-in a TS file. \n");
    printf("Usage:dmx_filein_start  [TS file] [pktSize 188/192]\n");
    printf("\n");
    printf("Sample UART Command: dmx_filein_start fileName.ts 188\n");
    printf("---------------------------------------------------------------------\n");
    printf("a. DMX_FileIn_Stop: stop the file-in procedure. \n");
    printf("Usage:dmx_filein_stop\n");
    printf("\n");
    printf("Sample UART Command: dmx_filein_stop\n");
    printf("\n");
    #endif
    printf("***********************End of Demux function List*************************\n");
    return TRUE;
}
#endif




