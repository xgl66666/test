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
#if 1
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

#include "MsCommon.h"
#include "MsMemory.h"
#include "drvDSCMB.h"
#include "apiDMX.h"
#include "apiVDEC.h"
#include "appDemo_AV.h"
//#include "ver_dtv_comm.h"
#include "asmCPU.h"
#include "MsOS.h"
#include <string.h>
#include "appDemo_DigiTuner.h"
#include "apiDigiTuner.h"
#ifdef ENABLE_CI_PACKAGE
#include "msAPP_CI.h"
#endif
//--------------------------------------------------------------------------------------------------
// constant
//--------------------------------------------------------------------------------------------------


#define appDemoDmx_print(fmt, args...)          { printf("[appDemoDmx][%06d]    ", __LINE__); printf(fmt, ## args); }

#define MAX_PG_IN_PAT           8
#define DATA_WAIT_TIME          5000

// Event setting macro : wait data ready event of corresponding filter, use 2 32-bit event group to manage 64 filter event passing.
#define WAIT_DATA_EVENT(DmxID,Event)   MsOS_WaitEvent(_s32DataReadyEventId[DmxID>>5] , BIT(DmxID) , &Event, E_OR_CLEAR , DATA_WAIT_TIME)

//--------------------------------------------------------------------------------------------------
// enumerate
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
    E_SI_STREAM_OP_MEPG2_VID        = 0x80,
    E_SI_STREAM_AC3_AUD             = 0x81,
    E_SI_STREAM_AVS_VID             = 0x42,
    E_SI_STREAM_AVS_AUD             = 0x43,
    E_SI_STREAM_AVS_PRIVATE_DATA    = 0x45,
} E_SI_StreamType;


typedef enum
{
    SiNOT_VALID                    = 0x00,
    SiMPEG                         = 0x01,
    SiAC3                          = 0x02,
    SiMPEG_AD                      = 0x03,
    SiMPEG4                        = 0x04,
    SiAAC                          = 0x05,
    SiAC3P                         = 0x06,
    SiAC3_AD                       = 0x07,
} SI_AudioFormat;

//--------------------------------------------------------------------------------------------------
// Structure define
//--------------------------------------------------------------------------------------------------

typedef struct
{
    MS_U32 u32PidVideo;
    MS_U32 u32PidAudio;
    MS_U32 u32PidPcr;
    MS_U32 u32StreamTypeVideo;
    MS_U32 u32StreamTypeAudio;
    MS_U32 u32ProgNum;
    MS_U32 u32PidPmt;
    MS_BOOL b264;
    SI_AudioFormat eAudioFmt;
} stPG;

typedef struct
{
//    MS_U32 u32Bandth;
//    MS_U32 u32Freq;
    stPG stPgList[MAX_PG_IN_PAT];
    MS_U32 u32PgNum;
} stFreqPG;

typedef struct
{
      MS_U8 name[64] ;
      MS_U32 Counter;
} NotifyParam;

//--------------------------------------------------------------------------------------------------
// Implementation parts
//--------------------------------------------------------------------------------------------------
static MS_S32                       _s32DataReadyEventId[2] = {-1 , -1 };

#ifdef ENABLE_PVR_PACKAGE
static MS_BOOL                      _bIsPVREnable= FALSE;
#endif /*ENABLE_PVR_PACKAGE*/

static MS_U8                       Data[1024*8];
//--------------------------------------------------------------------------------------------------
// Implementation parts
//--------------------------------------------------------------------------------------------------
extern MS_S32 gs32NonCachedPoolID;

// channel database
static stFreqPG stChDB;

static MS_U8 u8TSPFirmware[] = {
    #include "fwTSP.dat"
};


static MS_U8 Stream[] =
    {
    #include "bindata/vertsp.dat"
    };
//--------------------------------------------------------------------------------------------------
// local function
//--------------------------------------------------------------------------------------------------

/*********************************************************************  **
*   Define the Buffer Size and Supported buffer number
*   Please modify this definition to fullfill user requirment.
*   SECTION_BUF_ADDR is Dram reserver section for DMX section usage(defined in MsMemory128.h) : size = 0x100000
***********************************************************************/
#define DMX_DEMO_RESERVED_ADDRESS     SECTION_BUF_ADR

#define PES_BUFFER_NUM        8   // 8 * 0x10000 = 0xc0000 << modify this to fit stream requirement
#define PES_BUFFER_SIZE       0x18000 // 96K  << modify this to fit stream requirement

#define SECTION_BUFFER_NUM    64  // 64 * 0x1000 = 0x40000
#define SECTION_BUFFER_SIZE   0x1000

typedef MS_U8 PES_BUF[PES_BUFFER_SIZE];
static PES_BUF*      Pes_Buf_Addr =  (PES_BUF*)(DMX_DEMO_RESERVED_ADDRESS + 0x80000) ; // PES buffer size = 0x80000
static MS_U8         Pes_Buf_AllocFlag[PES_BUFFER_NUM];            // allocation flag

typedef MS_U8 SEC_BUF[SECTION_BUFFER_SIZE];
static SEC_BUF*      Section_Buf_Addr = (SEC_BUF*)DMX_DEMO_RESERVED_ADDRESS;

//==============================================================



//------------------------------------------------------------------------------
/// @brief The sample code for Static PES buffer Allocatation
/// @return TRUE  : process success .
/// @return FALSE : process fail.
/// @sa
/// @note
//------------------------------------------------------------------------------

MS_BOOL Pes_Buf_Alloc(MS_U32 * idx)
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

//------------------------------------------------------------------------------
/// @brief The sample code for Static PES buffer Free
/// @return TRUE  : process success .
/// @return FALSE : process fail.
/// @sa
/// @note
//------------------------------------------------------------------------------

MS_BOOL Pes_Buf_Free(MS_U32    idx)
{
    if (idx == PES_BUFFER_NUM ) return FALSE ;

    if (idx<PES_BUFFER_NUM )
    {
        Pes_Buf_AllocFlag[idx] = 0 ;
        return TRUE ;
    }
    return FALSE ;
}


//------------------------------------------------------------------------------
/// @brief The sample code for Data callback function for event setting.
/// @brief Callback function inform user thread the section status.
/// @brief If in Polling mode, user thread need to polling filter status by themselves.
/// @param[in] u8DmxId	dmx id
/// @param[in] enFilterStatus	fileter event status
/// @return None
/// @sa
/// @note
//------------------------------------------------------------------------------

static void _appDemo_DataCb(MS_U8 u8DmxId, DMX_EVENT enFilterStatus)
{
    if(enFilterStatus == DMX_EVENT_DATA_READY)
    {
        MsOS_SetEvent(_s32DataReadyEventId[u8DmxId    >>5 ], BIT(u8DmxId));
    }
    else if(enFilterStatus == DMX_EVENT_BUF_OVERFLOW)
    {
        MApi_DMX_Stop(u8DmxId);
        MApi_DMX_Start(u8DmxId);
    }
}
//------------------------------------------------------------------------------
/// @brief The sample code for Data callback function for TYPE2-event setting.
/// @brief Callback function inform user thread the section status.
/// @brief If in Polling mode, user thread need to polling filter status by themselves.
/// @param[in] u8DmxId	dmx id
/// @param[in] enFilterStatus	fileter event status
/// @return None
/// @sa
/// @note
//------------------------------------------------------------------------------

static void _appDemo_DataCb_Type2(MS_U8 u8DmxId, DMX_EVENT enFilterStatus,MS_U32 Type2NotifyParam1)
{


    if(enFilterStatus == DMX_EVENT_DATA_READY)
    {
        printf("======= DATA READY =======\n");
        ((NotifyParam*)Type2NotifyParam1)->Counter ++ ;
        MsOS_SetEvent(_s32DataReadyEventId[u8DmxId    >>5 ], BIT(u8DmxId));
        printf("%s\n",((NotifyParam*)Type2NotifyParam1)->name );
        printf("type2notify = %ld\n",((NotifyParam*)Type2NotifyParam1)->Counter );

    }
    else if(enFilterStatus == DMX_EVENT_BUF_OVERFLOW)
    {
        MApi_DMX_Stop(u8DmxId);
        MApi_DMX_Start(u8DmxId);
        printf("======= BUFFER OVERFLOW =======\n");
    }
}


//------------------------------------------------------------------------------
/// @brief The sample code to parse PAT table
/// @param[in] pu8Buf PAT buffer address
/// @param[in] BufSize	 PAT buffer size
/// @sa
/// @note
//------------------------------------------------------------------------------

static void _appDemo_Pat_Parse(MS_U8* pu8Buf, MS_U32 BufSize)
{
    MS_U16 u16ProgramNumber, u16SectionLength;
    MS_U8  uSectionNumber, uLastSectionNumber;  // todo: multiple section
    MS_U8  *pu8SecIter;
    MS_U8  i;
    MS_U32 u32NumOfItem;
    MS_U8  u8PatVerNum;
    MS_U16 u16ActualTSid;
    MS_U8* pu8Section= pu8Buf;
    MS_U8  u8Tid;
    MS_U16 u16Pid;

    // MApi_DMX_Init();

    u8Tid=                      pu8Section[0];
    u16SectionLength=           (pu8Section[1] & 0x0f ) << 8;
    u16SectionLength|=          pu8Section[2];
    u32NumOfItem=               (u16SectionLength - 9)/ 4;
    u16ActualTSid=              (pu8Section[3]<< 8)| pu8Section[4];
    u8PatVerNum=                (pu8Section[5]>> 1) & 0x1F;
    uSectionNumber=             pu8Section[6];
    uLastSectionNumber=         pu8Section[7];
    // Make sure multi section number doesn't exceed we can support.
    //uLastSectionNumber %= MAX_PAT_MULTI_SECTION_NUM;

    printf("================== PAT information =====================\n");
    printf("table id=                   0x%02x\n", u8Tid);
    printf("section length=             %d\n", u16SectionLength);
    printf("number of service=          %ld\n", u32NumOfItem);
    printf("Ts Id=                      0x%04x\n", u16ActualTSid);
    printf("section number=             %d\n", uSectionNumber);
    printf("section number last=        %d\n", uLastSectionNumber);
    printf("version number=             %d\n", u8PatVerNum);

    pu8SecIter = pu8Section + 8;
    for ( i = 0; i < u32NumOfItem; i++ )
    {
        u16ProgramNumber =       (pu8SecIter[0]<<8)| pu8SecIter[1];
        u16Pid =                 ((pu8SecIter[2] & 0x1F)<< 8)| pu8SecIter[3];
        printf("(program number, PID)= (0x%04x, 0x%04x)\n", u16ProgramNumber, u16Pid);
        pu8SecIter += 4;
    }
}

//------------------------------------------------------------------------------
/// @brief The sample code to parse PMT table
/// @param[in] pu8Buf PMT buffer address
/// @param[in] BufSize	 PMT buffer size
/// @sa
/// @note
//------------------------------------------------------------------------------

static void _appDemo_Pmt_Parse(MS_U8* pu8Buf, MS_U32 BufSize)
{
    MS_U16 u16SectionLength;
    MS_U8  uSectionNumber, uLastSectionNumber;  // todo: multiple section
    //MS_U8  *pu8SecIter;
    //MS_U32 u32NumOfItem;
    MS_U8  u8PatVerNum;
    MS_U16 u16ActualTSid;
    MS_U8* pu8Section= pu8Buf;
    MS_U8  u8Tid;
    //MS_U16 u16Pid;

    // MApi_DMX_Init();

    u8Tid=                      pu8Section[0];
    u16SectionLength=           (pu8Section[1] & 0x0f ) << 8;
    u16SectionLength|=          pu8Section[2];
    u16ActualTSid=              (pu8Section[3]<< 8)| pu8Section[4];
    u8PatVerNum=                (pu8Section[5]>> 1) & 0x1F;
    uSectionNumber=             pu8Section[6];
    uLastSectionNumber=         pu8Section[7];
    // Make sure multi section number doesn't exceed we can support.
    //uLastSectionNumber %= MAX_PAT_MULTI_SECTION_NUM;

    printf("================== PMT information =====================\n");
    printf("table id=                   0x%02x\n", u8Tid);
    printf("section length=             %d\n", u16SectionLength);
    printf("program number=             0x%04x\n", u16ActualTSid);
    printf("section number=             %d\n", uSectionNumber);
    printf("section number last=        %d\n", uLastSectionNumber);
    printf("version number=             %d\n", u8PatVerNum);

}

//------------------------------------------------------------------------------
/// @brief The sample code to parse PES table
/// @param[in] pu8Buf PES buffer address
/// @param[in] BufSize	 PES buffer size
/// @sa
/// @note
//------------------------------------------------------------------------------

static void _appDemo_Pes_Parse(MS_U8* pu8Buf, MS_U32 BufSize)
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
    printf("packet start code prefix=   0x%06lx\n", u32Pktprefix);
    printf("stream id=                  0x%02x\n", u8Sid);
    printf("packet length=              %d\n", u16SectionLength);
}


//------------------------------------------------------------------------------
/// @brief The sample code to find program nums
/// @param[in] u32ProgNum program nums
/// @sa
/// @note
//------------------------------------------------------------------------------

static stPG* _ScanPgFind(MS_U32 u32ProgNum)
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


//------------------------------------------------------------------------------
/// @brief The sample code to dump buffer data
/// @param[in] pu8Buf buffer address
/// @param[in] BufSize buffer size
/// @sa
/// @note
//------------------------------------------------------------------------------

static void data_dump(MS_U8* pu8Buf, MS_U32 BufSize)
{
    MS_U32 i ;

    for( i = 0 ; i < BufSize ; i ++ ){
        if((i%16) == 0 ) printf("\n");
        printf("%02X ",*(pu8Buf + i));
    }
}

//------------------------------------------------------------------------------
/// @brief The sample code to display program infos
/// @sa
/// @note
//------------------------------------------------------------------------------

static void _DumpCh(void)
{
    stPG* pstPG = NULL;
    MS_U32 i;
    char AudioFmtStr[][21]={"SiNOT_VALID","SiMPEG","SiAC3","SiMPEG_AD","SiMPEG4","SiAAC","SiAC3P","SiAC3_AD"};

    printf("[%s][%d] start =======================================\n", __FUNCTION__, __LINE__);
    for (i= 0; i< stChDB.u32PgNum; i++)
    {
        printf("[%s][%d] %d\n", __FUNCTION__, __LINE__, (int)i);
        pstPG = &(stChDB.stPgList[i]);
        printf("[%s][%d]     video pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidVideo);
        printf("[%s][%d]     audio pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidAudio);
        printf("[%s][%d]     pcr   pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidPcr);
        printf("[%s][%d]     pmt   pid = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidPmt);
        printf("[%s][%d]     v    type = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32StreamTypeVideo);
        printf("[%s][%d]     a    type = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32StreamTypeAudio);
        printf("[%s][%d]     prog  num = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32ProgNum);
        printf("[%s][%d]     264       = %s\n", __FUNCTION__, __LINE__, (pstPG->b264)? "TRUE": "FALSE");
        printf("[%s][%d]     AudioFmt  = %s\n", __FUNCTION__, __LINE__, AudioFmtStr[pstPG->eAudioFmt]);
    }
    printf("[%s][%d] end   =======================================\n", __FUNCTION__, __LINE__);
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
        if (MAX_PG_IN_PAT<= stChDB.u32PgNum)
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
    stPG*       pstPG= NULL;
    MS_U32      u32Pid;
    MS_U32      u32Pcr;
    MS_U32      u32ProgNum;
    MS_U32      u32TempLoopLen;
    MS_U16      u16DescriptorLength;
    MS_U8       *pu8Descriptor;
    MS_S16      s16TotalDescriptorLength;
    MS_BOOL     bIsAC3D = false;

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
    s32LoopLen -= u32ProgInfoLen; // program_info_length
    s32LoopLen -= 4; // CRC

    u32Pcr = ((MS_U32)(((MS_U8*)(u8PmtBuf))[8]<<8)|(((MS_U8*)(u8PmtBuf))[9])) & 0x1FFF;
    u32ProgNum = (u8PmtBuf[3] << 8) | u8PmtBuf[4];

    pu8Loop = u8PmtBuf + 12 + u32ProgInfoLen;


    if (NULL == (pstPG= _ScanPgFind(u32ProgNum)))
    {
        return FALSE;
    }

    pstPG->u32PidPcr = u32Pcr;
    while (5 <= s32LoopLen)
    {
        u32TempLoopLen = ((pu8Loop[3] & 0x0f ) << 8) | pu8Loop[4];
        u32Pid = ((MS_U32)(((MS_U8*)(pu8Loop))[1]<<8)|(((MS_U8*)(pu8Loop))[2])) & 0x1FFF;
        switch (pu8Loop[0])
        {
        case E_SI_STREAM_AVCH264_VID:
            pstPG->b264 = TRUE;
        case E_SI_STREAM_MPEG1_VID:
        case E_SI_STREAM_MPEG2_VID:
        case E_SI_STREAM_AVS_VID:
            if (!pstPG->u32PidVideo)
            {
                pstPG->u32PidVideo = u32Pid;
                pstPG->u32StreamTypeVideo = pu8Loop[0];
            }
            break;
            // [5-2] Audio
        case E_SI_STREAM_MPEG1_AUD:
            // MPEG-1 Audio
        case E_SI_STREAM_MPEG2_AUD:
            // MPEG-2 Audio
        case E_SI_STREAM_AC3_AUD:
            // 13818-7 AAC
        case E_SI_STREAM_AAC_AUD:
            // 14496-3 MPEG4
        case E_SI_STREAM_MPEG4_AUD:
            if (!pstPG->u32PidAudio)
            {
                pstPG->u32PidAudio = u32Pid;
                pstPG->u32StreamTypeAudio = pu8Loop[0];
                if ( pu8Loop[0] == E_SI_STREAM_AC3_AUD )
                {
                    pstPG->eAudioFmt = SiAC3;
                }
                else if(pu8Loop[0] == E_SI_STREAM_MPEG4_AUD)
                {
                    pstPG->eAudioFmt = SiMPEG4;
                }
                else if(pu8Loop[0] == E_SI_STREAM_AAC_AUD)
                {
                    pstPG->eAudioFmt = SiAAC;
                }
                else
                {
                    pstPG->eAudioFmt = SiMPEG;
                }
            }
            break;
        case E_SI_STREAM_PRIVATE_DATA:
        case E_SI_STREAM_AVS_PRIVATE_DATA:
            s16TotalDescriptorLength = u32ProgInfoLen;
            pu8Descriptor = pu8Loop+5;
            while(s16TotalDescriptorLength > 0)
            {
                /* got desired descriptor */
                if(pu8Descriptor[0] == 0x6A)
                {
                    break;
                }
                /* move to next descriptor */
                u16DescriptorLength = pu8Descriptor[1] + 2;
                s16TotalDescriptorLength -= u16DescriptorLength;
                pu8Descriptor += u16DescriptorLength;
            }
            if(pu8Descriptor == NULL)
            {
                s16TotalDescriptorLength = u32ProgInfoLen;
                pu8Descriptor = pu8Loop+5;
                while(s16TotalDescriptorLength > 0)
                {
                    /* got desired descriptor */
                    if(pu8Descriptor[0] == 0x7A)
                    {
                        break;
                    }
                    /* move to next descriptor */
                    u16DescriptorLength = pu8Descriptor[1] + 2;
                    s16TotalDescriptorLength -= u16DescriptorLength;
                    pu8Descriptor += u16DescriptorLength;
                }
            }
            else
            {
                bIsAC3D = TRUE;
            }
            if (NULL == pu8Descriptor) //this stream is AC3 audio stream.
            {
                 break;
            }
            if (!pstPG->u32PidAudio)
            {
                pstPG->u32PidAudio = u32Pid;
                pstPG->u32StreamTypeAudio = pu8Loop[0];
                pstPG->eAudioFmt = bIsAC3D ? SiAC3 : SiAC3P;
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


//------------------------------------------------------------------------------
/// @brief The sample code to parse PMT buffer
/// @param[in] u8PmtBuf pmt buffer
/// @sa
/// @note
//------------------------------------------------------------------------------

MS_BOOL _ScanFilterParsePMT_single(MS_U8* u8PmtBuf)
{
    MS_U32      s32LoopLen;
    MS_U8*      pu8Loop;
    MS_U32      u32ProgInfoLen;
    stPG*       pstPG= NULL;
    MS_U32      u32Pid;
    MS_U32      u32Pcr;
    MS_U32      u32ProgNum;
    MS_U32      u32TempLoopLen;
    MS_U16      u16DescriptorLength;
    MS_U8       *pu8Descriptor;
    MS_S16      s16TotalDescriptorLength;
    MS_BOOL     bIsAC3D = false;

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
    s32LoopLen -= u32ProgInfoLen; // program_info_length
    s32LoopLen -= 4; // CRC

    u32Pcr = ((MS_U32)(((MS_U8*)(u8PmtBuf))[8]<<8)|(((MS_U8*)(u8PmtBuf))[9])) & 0x1FFF;
    u32ProgNum = (u8PmtBuf[3] << 8) | u8PmtBuf[4];

    pu8Loop = u8PmtBuf + 12 + u32ProgInfoLen;

   pstPG = &(stChDB.stPgList[0]);
   stChDB.u32PgNum = 1 ;

    pstPG->u32PidPcr = u32Pcr;
    while (5 <= s32LoopLen)
    {
        u32TempLoopLen = ((pu8Loop[3] & 0x0f ) << 8) | pu8Loop[4];
        u32Pid = ((MS_U32)(((MS_U8*)(pu8Loop))[1]<<8)|(((MS_U8*)(pu8Loop))[2])) & 0x1FFF;
        switch (pu8Loop[0])
        {
        case E_SI_STREAM_AVCH264_VID:
            pstPG->b264 = TRUE;
        case E_SI_STREAM_MPEG1_VID:
        case E_SI_STREAM_MPEG2_VID:
        case E_SI_STREAM_AVS_VID:
            if (!pstPG->u32PidVideo)
            {
                pstPG->u32PidVideo = u32Pid;
                pstPG->u32StreamTypeVideo = pu8Loop[0];
            }
            break;
            // [5-2] Audio
        case E_SI_STREAM_MPEG1_AUD:
            // MPEG-1 Audio
        case E_SI_STREAM_MPEG2_AUD:
            // MPEG-2 Audio
        case E_SI_STREAM_AC3_AUD:
            // 13818-7 AAC
        case E_SI_STREAM_AAC_AUD:
            // 14496-3 MPEG4
        case E_SI_STREAM_MPEG4_AUD:
            if (!pstPG->u32PidAudio)
            {
                pstPG->u32PidAudio = u32Pid;
                pstPG->u32StreamTypeAudio = pu8Loop[0];
                if ( pu8Loop[0] == E_SI_STREAM_AC3_AUD )
                {
                    pstPG->eAudioFmt = SiAC3;
                }
                else if(pu8Loop[0] == E_SI_STREAM_MPEG4_AUD)
                {
                    pstPG->eAudioFmt = SiMPEG4;
                }
                else if(pu8Loop[0] == E_SI_STREAM_AAC_AUD)
                {
                    pstPG->eAudioFmt = SiAAC;
                }
                else
                {
                    pstPG->eAudioFmt = SiMPEG;
                }
            }
            break;
        case E_SI_STREAM_PRIVATE_DATA:
        case E_SI_STREAM_AVS_PRIVATE_DATA:
            s16TotalDescriptorLength = u32ProgInfoLen;
            pu8Descriptor = pu8Loop+5;
            while(s16TotalDescriptorLength > 0)
            {
                /* got desired descriptor */
                if(pu8Descriptor[0] == 0x6A)
                {
                    break;
                }
                /* move to next descriptor */
                u16DescriptorLength = pu8Descriptor[1] + 2;
                s16TotalDescriptorLength -= u16DescriptorLength;
                pu8Descriptor += u16DescriptorLength;
            }
            if(pu8Descriptor == NULL)
            {
                s16TotalDescriptorLength = u32ProgInfoLen;
                pu8Descriptor = pu8Loop+5;
                while(s16TotalDescriptorLength > 0)
                {
                    /* got desired descriptor */
                    if(pu8Descriptor[0] == 0x7A)
                    {
                        break;
                    }
                    /* move to next descriptor */
                    u16DescriptorLength = pu8Descriptor[1] + 2;
                    s16TotalDescriptorLength -= u16DescriptorLength;
                    pu8Descriptor += u16DescriptorLength;
                }
            }
            else
            {
                bIsAC3D = TRUE;
            }
            if (NULL == pu8Descriptor) //this stream is AC3 audio stream.
            {
                 break;
            }
            if (!pstPG->u32PidAudio)
            {
                pstPG->u32PidAudio = u32Pid;
                pstPG->u32StreamTypeAudio = pu8Loop[0];
                pstPG->eAudioFmt = bIsAC3D ? SiAC3 : SiAC3P;
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

static DMX_FLOW_INPUT _GetDMXFowInput(void)
{
    MS_U32 u32DmxPath = MApi_GetDmxInputPath(0);

    if ( u32DmxPath == DMX_INPUT_DEMOD )
    {
        return DMX_FLOW_INPUT_DEMOD;
    }
    else if ( u32DmxPath == DMX_INPUT_EXT_INPUT0 )
    {
        return DMX_FLOW_INPUT_EXT_INPUT0;
    }
    else if ( u32DmxPath == DMX_INPUT_EXT_INPUT1 )
    {
        return DMX_FLOW_INPUT_EXT_INPUT1;
    }
    else
    {
        return DMX_FLOW_INPUT_DEMOD;
    }
}
//==================================
//--------------------------------------------------------------------------------------------------
// API implmentation
//--------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief The sample code to int TSP
/// @return TRUE  : process success .
/// @return FALSE : process fail.
/// @sa
/// @note
//------------------------------------------------------------------------------
MS_BOOL appDemo_TSP_Init(void)
{
    DMX_FLOW_INPUT inSrc = _GetDMXFowInput();

    MApi_DMX_CMD_Run( 0xF0010001 , 1, 0 , NULL );

    MApi_DMX_Init();
    {   // set VQ for TSP, set FW/VQ for TSP2
        DMX_TSPParam    param;
        memset(&param,0,sizeof(param));
        memcpy((void*)MS_PA2KSEG1((MS_U32)TSP_FW_BUF_ADR/*u8TSPFirmware*/) , u8TSPFirmware, sizeof(u8TSPFirmware));
        MApi_DMX_SetFW(((MS_PHYADDR)TSP_FW_BUF_ADR/*u8TSPFirmware*/), sizeof(u8TSPFirmware));
        param.phyFWAddr = (MS_PHYADDR) TSP_FW_BUF_ADR;
        param.u32FWSize = sizeof(u8TSPFirmware);
        param.phyVQAddr = (MS_PHYADDR)(TSP_VQ_BUF_ADR);
        param.u32VQSize = TSP_VQ_BUF_LEN;
        printf("FWAddr = %08lX , Size = %08lX , VQAddr = %08lX , VQSize = %08lX \n",param.phyFWAddr,param.u32FWSize,param.phyVQAddr,param.u32VQSize);
        if (MApi_DMX_TSPInit(&param) != DMX_FILTER_STATUS_OK )
        {
           return FALSE ;
        }
    }
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, TRUE);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, TRUE);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO2, TRUE);
    MDrv_DSCMB_Init();


#if 1 // internal demod
    if (DMX_FILTER_STATUS_OK!= MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, inSrc ,FALSE,TRUE,TRUE))
    {
        appDemoDmx_print("switch to demod fail\n");
    }
#else // playcard
    MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, DMX_FLOW_INPUT_EXT_INPUT0 ,FALSE, TRUE, TRUE);
#endif

    if (DMX_FILTER_STATUS_OK!= MApi_DMX_FlowSet(DMX_FLOW_PVR, inSrc ,FALSE,TRUE,TRUE))
    {
        appDemoDmx_print("switch to demod fail\n");
    }

    _s32DataReadyEventId[0]= MsOS_CreateEventGroup("Data ready Event ID  0 - 31 \n ");
    _s32DataReadyEventId[1]= MsOS_CreateEventGroup("Data ready Event ID 32 - 63\n ");

    //  Enable or Disable to remove TEI  audio or vedio error packets.
    MApi_DMX_TEI_RemoveErrorPkt( E_DMX_TEI_REMOVE_AUDIO_PKT, FALSE);
    MApi_DMX_TEI_RemoveErrorPkt( E_DMX_TEI_REMOVE_VIDEO_PKT, FALSE);

    return TRUE ;

}


//------------------------------------------------------------------------------
/// @brief The sample code to exit TSP
/// @return TRUE  : process success .
/// @return FALSE : process fail.
/// @sa
/// @note
//------------------------------------------------------------------------------

void appDemo_TSP_Exit(void)
{

    MApi_DMX_Exit();

    MsOS_DeleteEventGroup(_s32DataReadyEventId[0]);
    MsOS_DeleteEventGroup(_s32DataReadyEventId[1]);
    _s32DataReadyEventId[0] =  -1;
    _s32DataReadyEventId[1] =  -1;

}

//------------------------------------------------------------------------------
/// @brief The sample code to parse PAT for test demux
/// @return TRUE: success / FALSE: fail
/// @sa appDemo_Dmx_Pmt , appDemo_Dmx_Scan , appDemo_Dmx_Sec
/// @note
/// Command: \b demux_pat [pid]\n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Dmx_Pat(void)
{
    MS_U32                                 u32Event;
    MS_U8                                 *pu8Read;
    MS_U8                                 *pu8Write;
    MS_U8                                  DmxIdSect;
    MS_U8                                  pattern[16];
    MS_U8                                  mask[16];
    MS_U8                                  nmask[16];
    DMX_Flt_info                           FltInfo;
    MS_U8                                 *pu8BufAddr;
    MS_U16                                 u16PID;
    DMX_FILTER_TYPE                        FilterType;


    // Set Parameter of Allocatcation Demux Filter type and input Source type.
    FilterType = DMX_FILTER_TYPE_SECTION | DMX_FILTER_SOURCE_TYPE_LIVE;

    // Allocate a Filter and set Filter Basic Type
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(FilterType, &DmxIdSect))
    {
        appDemoDmx_print("[%s] Allocate filter fail \n",__FUNCTION__);
    }

    // Here we use pre-defined physical address
    // [Note] The section buffer MUST be continus in physical address space.(for DMA usage)
    pu8BufAddr = (MS_U8*)Section_Buf_Addr[DmxIdSect];

    //MsOS_PA2KSEG1 : transfer physical address to virtual address.
    memset((MS_U8*)MsOS_PA2KSEG1((MS_U32)pu8BufAddr), 0 , SECTION_BUFFER_SIZE);

    // Transfer Virtual Address to Phisical Hardware Address
    // Section buffer is structed as ring buffer, keep 4 pointer of start,end ,read and write.
    FltInfo.Info.SectInfo.SectBufAddr = (MS_U32)pu8BufAddr ;
    appDemoDmx_print("Physical Address = %08lx\n" ,FltInfo.Info.SectInfo.SectBufAddr);
    // Set buffer size
    FltInfo.Info.SectInfo.SectBufSize =     SECTION_BUFFER_SIZE;
    appDemoDmx_print("Size = %08lx\n" ,FltInfo.Info.SectInfo.SectBufSize);


    // We only need one section of PAT so we choose "DMX_SECT_MODE_ONESHOT" instead of "DMX_SECT_MODE_CONTI"
    FltInfo.Info.SectInfo.SectMode    =     (DMX_SECT_MODE_ONESHOT| DMX_SECT_MODE_CRCCHK);

    // <DMX_EVENT_DATA_READY/DMX_EVENT_BUF_OVERFLOW>
    // Event trigger condition for driver, Driver will call ap's callback  < CallBack Mode >
    // Cause We set SectMode as "DMX_SECT_MODE_ONESHOT" , we don't need to worry about Overflow event.
    // The Default event setting is Polling mode, but for performance consideration, Callback mode is recommended.
    FltInfo.Info.SectInfo.Event       =     DMX_EVENT_DATA_READY | DMX_EVENT_CB_SELF   | DMX_EVENT_BUF_OVERFLOW ;

    // Set the pointer of the event CB function into Demux driver
    FltInfo.Info.SectInfo.pNotify     =    _appDemo_DataCb;

    // Set Advanced Filter infomation
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Info( DmxIdSect, &FltInfo, &FilterType, TRUE))
    {
        appDemoDmx_print("[%s] MApi_DMX_Info fail \n",__FUNCTION__);
        goto DMX_PAT_FAIL;
    }

    // Set Filter PID --> PAT PID = 0x0
    u16PID = 0x0 ;
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid( DmxIdSect , &u16PID , TRUE))
    {
        appDemoDmx_print("[%s] MApi_DMX_Pid fail \n",__FUNCTION__);
        goto DMX_PAT_FAIL;
    }

    // Set Section Match pattern
    // The Match pattern is used for Getting specific section
    // Pattern[16] =  Pattern for match
    // Mask[16]    =  Match Mask : set 1 for match / 0 for ignore
    memset(pattern, 0x0, sizeof(pattern));
    memset(mask,    0x0, sizeof(mask));
    memset(nmask,   0x0, sizeof(nmask));
    pattern[0]=     0x00 ;
    mask[0]=        0xFF ;
    MApi_DMX_SectPatternSet(DmxIdSect, pattern, mask, nmask, 16);

    // Reset Section filter and section Buffer status;
    MApi_DMX_SectReset(DmxIdSect);

    // Start Filter and record section into Section Buffer.
    if (DMX_FILTER_STATUS_OK!= MApi_DMX_Start(DmxIdSect))
    {
        appDemoDmx_print("enable section filter fail\n");
        goto DMX_PAT_FAIL;
    }

    // Wait event until TSP driver call callback function
    // Wait event "_appDemo_Pat_Ready"
    // (Data Ready)-->TSP driver-->(callback)-->_appDemo_PatCb-->(SetEvent"_appDemo_Pat_Ready")
    //             -->contitue execution.
    WAIT_DATA_EVENT(DmxIdSect,u32Event);

    if (!u32Event)
    {
        goto DMX_PAT_FAIL;
    }

    // Get Buffer Read Pointer.
    MApi_DMX_SectReadGet(DmxIdSect, (MS_PHYADDR*)(&pu8Read));
    appDemoDmx_print("Read pointer = 0x%08lx\n", (MS_U32)pu8Read);


    // Get Buffer Write Pointer.
    // Note : We can get Hw section Buffer data with "MApi_DMX_CopyData", by copying data into a
    //        local data buffer[recomended] ==> dmx_pmt.
    //        But here we just pass pointers of seciton buffer into Pat parse function.
    //        Buffer wrap condition is considered when calling function MApi_DMX_CopyData.
    MApi_DMX_SectWriteGet(DmxIdSect, (MS_PHYADDR*)(&pu8Write));
    appDemoDmx_print("Write pointer = 0x%08lx\n", (MS_U32)pu8Write);

    if (pu8Write == pu8Read)
    {
        goto DMX_PAT_FAIL;
    }
    _appDemo_Pat_Parse((MS_U8*)MsOS_PA2KSEG1((MS_U32)pu8Read), pu8Write- pu8Read);

    _ScanFilterParsePAT(DmxIdSect);

    // Stop Filter
    MApi_DMX_Stop(DmxIdSect);

    // Free Filter.
    MApi_DMX_Close(DmxIdSect);

    return TRUE;

DMX_PAT_FAIL:
    MApi_DMX_Close(DmxIdSect);
    return FALSE;

}

//------------------------------------------------------------------------------
/// @brief The sample code to parse section for test demux
/// @param pid \b IN Pid
/// @param pat0 \b IN match pattern0 (byte 0~3)
/// @param pat1 \b IN match pattern1 (byte 4~7)
/// @param msak0 \b IN mask0 (byte 0~3)
/// @param mask1 \b IN mask1 (byte 4~7)
/// @return TRUE: success / FALSE: fail
/// @sa
/// @note
/// Command: \b dmx_sec [Pid] [MatchPattern1] [MatchPattern2] [MatchMask1] [MatchMask2]\n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Dmx_Sec(MS_U32* pid ,MS_U32* pat0, MS_U32* pat1, MS_U32* mask0 , MS_U32* mask1)
{
    MS_U8*                                 pu8Read;
    MS_U8*                                 pu8Write;
    MS_U8                                  DmxIdSect;
    MS_U8                                  pattern[16];
    MS_U8                                  mask[16];
    MS_U8                                  nmask[16];
    DMX_Flt_info                           FltInfo;
    MS_U16                                 u16Pid;
    MS_U32                                 u32Event;
    DMX_FILTER_TYPE                        FilterType;
    MS_U8*                                 pu8BufAddr;

    u16Pid = *pid ;

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

    pattern[0] = (*pat0) >> 24 ;
    pattern[1] = ((*pat0) >> 16)& 0xFF ;
    pattern[2] = ((*pat0) >> 8)& 0xFF ;
    pattern[3] = ((*pat0))& 0xFF ;

    pattern[4] = (*pat1) >> 24 ;
    pattern[5] = ((*pat1) >> 16)& 0xFF ;
    pattern[6] = ((*pat1) >> 8)& 0xFF ;
    pattern[7] = ((*pat1))& 0xFF ;

    mask[0] = (*mask0) >> 24 ;
    mask[1] = ((*mask0) >> 16)& 0xFF ;
    mask[2] = ((*mask0) >> 8)& 0xFF ;
    mask[3] = ((*mask0))& 0xFF ;

    mask[4] = (*mask1) >> 24 ;
    mask[5] = ((*mask1) >> 16)& 0xFF ;
    mask[6] = ((*mask1) >> 8)& 0xFF ;
    mask[7] = ((*mask1))& 0xFF ;


    // Set Parameter of Allocate Demux Filter type and input Source type.
    FilterType = DMX_FILTER_TYPE_SECTION | DMX_FILTER_SOURCE_TYPE_LIVE;

    // Allocate a Filter and set Filter Basic Type
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(FilterType, &DmxIdSect))
    {
        appDemoDmx_print("[%s] Allocate filter fail \n",__FUNCTION__);
    }

    // Here we use pre-defined physical address of reserved section buffer.
    // [Note] The section buffe MUST be continus in physical address space.(for DMA usage)
    pu8BufAddr = (MS_U8*)Section_Buf_Addr[DmxIdSect] ;
    memset((MS_U8*)MsOS_PA2KSEG1((MS_U32)pu8BufAddr), 0 , SECTION_BUFFER_SIZE);

    // Transfer Virtual Address to Phisical Hardware Address
    // Section buffer is structed as ring buffer, keep 4 pointer of start,end ,read and write.
    FltInfo.Info.SectInfo.SectBufAddr = (MS_U32)pu8BufAddr ;

    appDemoDmx_print("Physical Address = %08lx\n" ,FltInfo.Info.SectInfo.SectBufAddr);
    // Set buffer size
    FltInfo.Info.SectInfo.SectBufSize =     SECTION_BUFFER_SIZE;
    appDemoDmx_print("Size = %08lx\n" ,FltInfo.Info.SectInfo.SectBufSize);


    FltInfo.Info.SectInfo.SectMode    =  DMX_SECT_MODE_CRCCHK ;

    // <DMX_EVENT_DATA_READY/DMX_EVENT_BUF_OVERFLOW>
    // Event trigger condition for driver, Driver will call ap's callback  < CallBack Mode Type2 >
    FltInfo.Info.SectInfo.Event        =     DMX_EVENT_DATA_READY | DMX_EVENT_BUF_OVERFLOW | DMX_EVENT_CB_SELF ;

    // Set the pointer of the event CB function into Demux driver
    FltInfo.Info.SectInfo.pNotify =    &_appDemo_DataCb;

    // Set Advanced Filter infomation
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Info( DmxIdSect, &FltInfo, &FilterType, TRUE))
    {
        appDemoDmx_print("[%s] MApi_DMX_Info fail \n",__FUNCTION__);
    }

    // Set Filter PID --> Section PID
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid( DmxIdSect , &u16Pid , TRUE))
    {
        appDemoDmx_print("[%s] MApi_DMX_Pid fail \n",__FUNCTION__);
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
        appDemoDmx_print("enable section filter fail\n");
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
    MApi_DMX_SectReadGet(DmxIdSect, (MS_PHYADDR*)(&pu8Read));
    appDemoDmx_print("Read pointer = 0x%08lx\n", (MS_U32)pu8Read);

    // Get Buffer Write Pointer.
    MApi_DMX_SectWriteGet(DmxIdSect, (MS_PHYADDR*)(&pu8Write));
    appDemoDmx_print("Write pointer = 0x%08lx\n", (MS_U32)pu8Write);

    //_appDemo_Pat_Parse((MS_U8*)MsOS_PA2KSEG1(pu8Read), pu8Write- pu8Read);
    if ((pu8Write- pu8Read) == 0 )
        {appDemoDmx_print("\n !! No Section Get - Wait timeout \n" );}
    else
        {data_dump((MS_U8*)MsOS_PA2KSEG1((MS_U32)pu8Read),pu8Write- pu8Read);}


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
/// @brief The sample code to setup a section filter.
/// @param *DmxId \b IN
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

#if 1
MS_BOOL appDemo_Dmx_setup(MS_U32* DmxId,MS_U32 Pid, MS_BOOL bOneShot,MS_U8* ipattern, MS_U8* imask, MS_U8* inmask,MS_BOOL filein)
{
    MS_U8                                  DmxIdSect;
    MS_U16                                 u16Pid;
    MS_U8                                  pattern[16];
    MS_U8                                  mask[16];
    MS_U8                                  nmask[16];
    DMX_FILTER_TYPE                        FilterType;
    MS_U8*                                 pu8BufAddr;
    DMX_Flt_info                           FltInfo;


    u16Pid = Pid ;

    memcpy(pattern,   ipattern, sizeof(pattern));
    memcpy(mask   ,   imask   , sizeof(mask));
    memcpy(nmask  ,   inmask  , sizeof(nmask));

    if (filein)
        FilterType = DMX_FILTER_TYPE_SECTION | DMX_FILTER_SOURCE_TYPE_FILE;
    else
        FilterType = DMX_FILTER_TYPE_SECTION | DMX_FILTER_SOURCE_TYPE_LIVE;

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(FilterType, &DmxIdSect))
    {
        appDemoDmx_print("[%s] Allocate filter fail \n",__FUNCTION__);
        goto appDemo_Dmx_setup_Fail ;
    }
    pu8BufAddr = (MS_U8*)Section_Buf_Addr[DmxIdSect] ;

    FltInfo.Info.SectInfo.SectBufAddr = (MS_U32)pu8BufAddr;
    FltInfo.Info.SectInfo.SectBufSize =     SECTION_BUFFER_SIZE;
    FltInfo.Info.SectInfo.Event       =     DMX_EVENT_DATA_READY | DMX_EVENT_CB_SELF | DMX_EVENT_BUF_OVERFLOW;
    FltInfo.Info.SectInfo.pNotify     =    _appDemo_DataCb;

    if (bOneShot)
        FltInfo.Info.SectInfo.SectMode =  DMX_SECT_MODE_ONESHOT|DMX_SECT_MODE_CRCCHK ;
    else
       FltInfo.Info.SectInfo.SectMode  =  DMX_SECT_MODE_CRCCHK ;


    if (DMX_FILTER_STATUS_OK != MApi_DMX_Info( DmxIdSect, &FltInfo, &FilterType, TRUE))
    {
        appDemoDmx_print("[%s] MApi_DMX_Info fail \n",__FUNCTION__);
        goto appDemo_Dmx_setup_Fail ;
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid( DmxIdSect , &u16Pid , TRUE))
    {
        appDemoDmx_print("[%s] MApi_DMX_Pid fail \n",__FUNCTION__);
        goto appDemo_Dmx_setup_Fail ;
    }

    MApi_DMX_SectReset(DmxIdSect);

    MApi_DMX_SectPatternSet(DmxIdSect, pattern, mask, nmask, 16);

    *DmxId = DmxIdSect ;

    return TRUE;

appDemo_Dmx_setup_Fail:

    MApi_DMX_Close(DmxIdSect);
    return FALSE;

}


//-------------------------------------------------------
/// @brief Abstraction for start a filter.
/// @param DmxIdSect: Demux Filter ID
/// @return none
/// @sa
/// @note
//-------------------------------------------------------

void appDemo_Dmx_start(MS_U32 DmxIdSect)
{
    // Start Filter and record section into Section Buffer.
    if (DMX_FILTER_STATUS_OK!= MApi_DMX_Start(DmxIdSect))
    {
        appDemoDmx_print("enable section filter fail\n");
    }

}


//-------------------------------------------------------
/// @brief Abstraction for close a filter.
/// @param DmxIdSect: Demux Filter ID
/// @return none
/// @sa
/// @note
//-------------------------------------------------------
void appDemo_Dmx_close(MS_U32 DmxIdSect)
{

    // Stop Filter
    MApi_DMX_Stop(DmxIdSect);

    // Free Filter.
    MApi_DMX_Close(DmxIdSect);

    //MsOS_FreeMemory(pu8BufAddr , 0);


}

#endif

//------------------------------------------------------------------------------
/// @brief The sample code to filte PMT information.
/// @param Pid \b IN match pid
/// @return TRUE: success, print out pmt section.
/// @return FALSE: fail.
/// @sa
/// @note   Use simple abstraction of appDemo_Dmx_setup/appDemo_Dmx_start/appDemo_Dmx_close to complete this function \n
///      Command: \b dmx_pmt [Pid] \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Dmx_Pmt(MS_U32* Pid)
{
    MS_U32 DmxId;
    MS_U32 DataLen,DataRmn;
    MS_U8  pattern[16],mask[16],nmask[16];
    MS_U32 u32Event;
    stPG*  pstPG = NULL;
    char   AudioFmtStr[][21]={"SiNOT_VALID","SiMPEG","SiAC3","SiMPEG_AD","SiMPEG4","SiAAC","SiAC3P","SiAC3_AD"};

    memset(pattern , 0x0 , sizeof(pattern));
    memset(mask    , 0x0 , sizeof(mask));
    memset(nmask   , 0x0 , sizeof(nmask));

    printf("Wait PMT PID = %04lX\n",*Pid);

    // simple abstraction for section setup
    if (!appDemo_Dmx_setup(&DmxId,*Pid,true,pattern,mask,nmask,false))
    {
        appDemoDmx_print("Filter Setup Fail\n");
        goto DMX_PMT_FAIL;
    }

    printf("dmx_set up ... \n");

    // Start section filter
    appDemo_Dmx_start(DmxId);

    printf("dmx start ...  \n");

    // wait until data ready event or timeout
    //MsOS_WaitEvent(_s32EventId, _appDemo_Data_Ready, &u32Event, E_OR_CLEAR, 2000);
    WAIT_DATA_EVENT(DmxId, u32Event);

    if (!u32Event)
    {
        printf("Wait PMT time out!\n");
        goto DMX_PMT_FAIL;
    }

    printf("dmx_copy data ...\n");
    //DMA_Api for user to Copy data from HW section Buffer into user data Buffer.
    MApi_DMX_CopyData(DmxId,Data,1024,&DataLen,&DataRmn,NULL);

#ifdef ENABLE_CI_PACKAGE
    msAPP_CI_notifyPMTUpdate(Data, (MS_U16)DataLen);
#endif

    if (DataLen != 0)
    {
        data_dump(Data,DataLen);
        printf(" \n");
        _appDemo_Pmt_Parse(Data, DataLen);
    }
    else
    {
        printf("Wait PMT time out!\n");
    }

    if(Data[0] == 0x2 )
    {
        memset(&stChDB , 0x0 , sizeof(stFreqPG));
        _ScanFilterParsePMT_single(Data);
        printf("[%s][%d] start =======================================\n", __FUNCTION__, __LINE__);
        pstPG = &(stChDB.stPgList[0]);
        printf("[%s][%d]     video pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidVideo);
        printf("[%s][%d]     audio pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidAudio);
        printf("[%s][%d]     pcr   pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidPcr);
        printf("[%s][%d]     pmt   pid = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidPmt);
        printf("[%s][%d]     v    type = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32StreamTypeVideo);
        printf("[%s][%d]     a    type = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32StreamTypeAudio);
        printf("[%s][%d]     prog  num = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32ProgNum);
        printf("[%s][%d]     264       = %s\n", __FUNCTION__, __LINE__, (pstPG->b264)? "TRUE": "FALSE");
        printf("[%s][%d]     AudioFmt  = %s\n", __FUNCTION__, __LINE__, AudioFmtStr[pstPG->eAudioFmt]);
        printf("[%s][%d] end   =======================================\n", __FUNCTION__, __LINE__);
    }

    appDemo_Dmx_close(DmxId);
    return TRUE;

DMX_PMT_FAIL:
    appDemo_Dmx_close(DmxId);
    return FALSE ;

}

//------------------------------------------------------------------------------
/// @brief The sample code to Scan program information.
/// @param none \b none
/// @return TRUE: success, print out program information\n
/// @return FALSE: scan fail.
/// @sa
/// @note   Scan program and parse program information \n
/// Command: \b dmx_scan \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Dmx_Scan(void)
{
    MS_U32 DmxId;
    MS_U8  pattern[16],mask[16],nmask[16];
    MS_U32 u32Event = 0 ;
    MS_U32 i ;


    //appDemo_DigiTuner_SetFreq(418000 , 8 );

    memset(pattern , 0x0 , sizeof(pattern));
    memset(mask    , 0x0 , sizeof(mask));
    memset(nmask   , 0x0 , sizeof(nmask));

    // simple abstraction for section setup
    if (!appDemo_Dmx_setup(&DmxId,0x0,true,pattern,mask,nmask,false))
    {
        goto DMX_SCAN_FAIL;
    }

    printf("dmx_set pat up ... \n");
    // Start section filter
    appDemo_Dmx_start(DmxId);
    printf("dmx pat start ...  \n");
    // wait until data ready event or timeout
    //MsOS_WaitEvent(_s32EventId, _appDemo_Data_Ready, &u32Event, E_OR_CLEAR, 2000);
    WAIT_DATA_EVENT(DmxId, u32Event);
    if (!u32Event)
    {
        printf("Wait PAT time out ! \n");
        appDemo_Dmx_close(DmxId);
        goto DMX_SCAN_FAIL;
    }


    printf("dmx_copy data ...\n");
    _ScanFilterParsePAT(DmxId);
    appDemo_Dmx_close(DmxId);

    for (i= 0; i< stChDB.u32PgNum; i++)
    {
        appDemo_Dmx_setup(&DmxId ,stChDB.stPgList[i].u32PidPmt,true,pattern,mask,nmask,false);
        appDemo_Dmx_start(DmxId);
        //MsOS_WaitEvent(_s32EventId, _appDemo_Data_Ready, &u32Event, E_OR_CLEAR, 5000);
        WAIT_DATA_EVENT(DmxId, u32Event);
        if (u32Event)
        {
            _ScanFilterParsePMT(DmxId);
        }
        appDemo_Dmx_close(DmxId);
    }

#ifdef ENABLE_PVR_PACKAGE
    if(_bIsPVREnable==FALSE)
#endif /* ENABLE_PVR_PACKAGE */
    _DumpCh();

    return TRUE ;

DMX_SCAN_FAIL:
    return FALSE;

}

#ifdef ENABLE_PVR_PACKAGE

//------------------------------------------------------------------------------
/// @brief The sample code for Data callback function for event setting. Callback function inform user thread the section status. If in Polling mode, user thread need to polling filter status by themselves.
/// @param[in] u8DmxId	dmx id
/// @param[in] enFilterStatus	fileter event status
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
//------------------------------------------------------------------------------

MS_BOOL appDemo_Dmx_GetCurProgram(MS_U32 u32PCRPid,MS_U32 u32VideoPid,MS_U32 u32AudioPid,MS_U16 *pu16PtmPid,MS_U16 *pu16LCN)
{
    stPG* pstPG = NULL;
    MS_U32 i=0;
    //printf("[%s][%d]     u16PCRPid=0x%04x, u16VideoPid0x%04x, u16AudioPid0x%04x\n", __FUNCTION__, __LINE__, u16PCRPid, u16VideoPid, u16AudioPid);
    _bIsPVREnable= TRUE;
    if(appDemo_Dmx_Scan()==TRUE)
    {
        _bIsPVREnable= FALSE;
        for (i= 0; i< stChDB.u32PgNum; i++)
        {
            pstPG = &(stChDB.stPgList[i]);
            if( (u32PCRPid == pstPG->u32PidPcr) && (u32VideoPid == pstPG->u32PidVideo) && ( u32AudioPid == pstPG->u32PidAudio))
            {

        printf("[%s][%d]     video pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidVideo);
        printf("[%s][%d]     audio pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidAudio);
        printf("[%s][%d]     pcr   pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidPcr);
        printf("[%s][%d]     pmt   pid = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidPmt);
        printf("[%s][%d]     v    type = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32StreamTypeVideo);
        printf("[%s][%d]     a    type = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32StreamTypeAudio);
        printf("[%s][%d]     prog  num = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32ProgNum);
        printf("[%s][%d]     264       = %s\n", __FUNCTION__, __LINE__, (pstPG->b264)? "TRUE": "FALSE");
        printf("[%s][%d]     AudioFmt  = 0x%08x\n", __FUNCTION__, __LINE__, pstPG->eAudioFmt);

                *pu16PtmPid = (MS_U16)pstPG->u32PidPmt;
                *pu16LCN = (MS_U16)pstPG->u32ProgNum;
                return TRUE;
            }
        }
    }
    _bIsPVREnable= FALSE;
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code for Data callback function for event setting. Callback function inform user thread the section status. If in Polling mode, user thread need to polling filter status by themselves.
/// @param[in] u8DmxId  dmx id
/// @param[in] enFilterStatus   fileter event status
/// @return TRUE: Process success.
/// @return FALSE: Process fail.
/// @sa
/// @note
//------------------------------------------------------------------------------
MS_BOOL appDemo_Dmx_GetLiveProgram(MS_U32 *u32PCRPid,MS_U32 *u32VideoPid,MS_U32 *u32AudioPid, MS_U32 *u32VCodec, MS_U32 *u32ACodec,MS_U16 *pu16PtmPid,MS_U16 *pu16LCN)
{
    stPG* pstPG = NULL;
    MS_U32 i=0;
    _bIsPVREnable= TRUE;

    pCodecPIDs pliveProgramPID = DemoAV_TSP_GetLiveTVPIDs();

    if(pliveProgramPID->bPlayLiveTV != TRUE)
    {
        printf("Not Play Live TV    !!!\n");
        _bIsPVREnable = FALSE;
        return FALSE;
    }


    printf("[%s][%d]Live Program     u32PCRPid=0x%04lx, u32VideoPid0x%04lx, u32AudioPid0x%04lx\n", __FUNCTION__, __LINE__, pliveProgramPID->u32PCRPid, pliveProgramPID->u32VideoPid, pliveProgramPID->u32AudioPid);

    *u32AudioPid = (MS_U32)pliveProgramPID->u32AudioPid;
    *u32VideoPid = (MS_U32)pliveProgramPID->u32VideoPid;
    *u32PCRPid =  (MS_U32)pliveProgramPID->u32PCRPid;
    *u32VCodec = (MS_U32)pliveProgramPID->u32VCodec;
    *u32ACodec= (MS_U32)pliveProgramPID->u32ACodec;

    if(appDemo_Dmx_Scan()==TRUE)
    {
        _bIsPVREnable= FALSE;
        for (i= 0; i< stChDB.u32PgNum; i++)
        {
            pstPG = &(stChDB.stPgList[i]);

            if( (pliveProgramPID->u32PCRPid== pstPG->u32PidPcr) && (pliveProgramPID->u32VideoPid== pstPG->u32PidVideo) && ( pliveProgramPID->u32AudioPid== pstPG->u32PidAudio))
            {
                *pu16PtmPid = (MS_U16)pstPG->u32PidPmt;
                *pu16LCN = (MS_U16)pstPG->u32ProgNum;

                /*
                 printf("[%s][%d]     video pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidVideo);
                    printf("[%s][%d]     audio pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidAudio);
                    printf("[%s][%d]     pcr   pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidPcr);
                    printf("[%s][%d]     pmt   pid = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidPmt);
                    printf("[%s][%d]     v    type = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32StreamTypeVideo);
                    printf("[%s][%d]     a    type = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32StreamTypeAudio);
                    printf("[%s][%d]     prog  num = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32ProgNum);
                    printf("[%s][%d]     264       = %s\n", __FUNCTION__, __LINE__, (pstPG->b264)? "TRUE": "FALSE");
                     printf("[%s][%d]     AudioFmt  = 0x%08x\n", __FUNCTION__, __LINE__, pstPG->eAudioFmt);
                */
                return TRUE;
            }
        }
    }

   _bIsPVREnable= FALSE;
   return FALSE;
}

#endif /* ENABLE_PVR_PACKAGE END */
//------------------


//------------------------------------------------------------------------------
/// @brief The sample code to find char number
/// @return TRUE  : process success .
/// @return FALSE : process fail.
/// @sa
/// @note
//------------------------------------------------------------------------------

unsigned char convert(char ch)
{
//printf("%c--",ch);
char out = ch;
if ((ch > 47) && (ch < 58))
    out = ch - 48 ;
if ((ch > 64) && (ch < 71))
    out = ch - 55;
if ((ch > 96) && (ch < 103))
    out = ch - 87;
//printf("%x\n",out);
 return out;
}


//------------------------------------------------------------------------------
/// @brief The sample code to demo NMASK characteristics
/// @param ipat   \b input match pattern
/// @param imask  \b input match mask
/// @param inmask \b input not match mask (NMASK)
/// @return TRUE: success, print out filter data.
/// @return FALSE: Error occured;
/// @sa
/// @note   Support 16 bytes match pattern depth, demo code only use 4 bytes for pattern match \n
///         Command:\n
///         \b dmx_nmask [Pattern] [Mask] [Nmask]   \n
///         \b dmx_nmask 40800000 FFFF0000 00000000 \n
///         \b dmx_nmask 40800000 FFFF0000 F0000000 \n
//------------------------------------------------------------------------------

MS_BOOL appDemo_Dmx_Nmask(MS_U8 *ipat, MS_U8 *imask, MS_U8 *inmask )
{
    MS_U32 DmxId;
    MS_U8  pattern[16],mask[16],nmask[16];
    MS_U32 u32Event;
    DMX_Filein_info FileinInfo;
    MS_U8  *FileinAddr;
    MS_U32 DataLen,DataRmn;
    MS_U32 i ;
    DMX_FLOW_INPUT inSrc ;

    memset(pattern , 0x0 , sizeof(pattern));
    memset(mask    , 0x0 , sizeof(mask));
    memset(nmask   , 0x0 , sizeof(nmask));

    for( i = 0; i < 4 ; i ++)
        pattern[i] = (convert(ipat  [2*i]) << 4 ) |  convert(ipat  [2*i+1]) ;
    for( i = 0; i < 4 ; i ++)
        mask[i]    = (convert(imask [2*i]) << 4 ) |  convert(imask [2*i+1]) ;
    for( i = 0; i < 4 ; i ++)
        nmask[i]   = (convert(inmask[2*i]) << 4 ) |  convert(inmask[2*i+1]) ;

    inSrc = _GetDMXFowInput();
    if (DMX_FILTER_STATUS_OK!= MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, DMX_FLOW_INPUT_MEM ,FALSE,TRUE,TRUE))
    {
        appDemoDmx_print("switch to memory input fail\n");
    }

    // simple abstraction for section setup
    if (!appDemo_Dmx_setup(&DmxId ,0xaaa ,false ,pattern ,mask ,nmask ,true))
    {
        goto DMX_NMASK_FAIL;
    }
    printf("dmx_set up ... \n");

    // Start section filter
    appDemo_Dmx_start(DmxId);
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
    FileinAddr = MsOS_AllocateMemory(sizeof(Stream),  gs32NonCachedPoolID);
    memcpy( (MS_U8*)(FileinAddr),Stream,sizeof(Stream));
    MApi_DMX_Filein_Start(DMX_PES_NO_BYPASS_FIFO, MsOS_VA2PA((MS_U32) FileinAddr), sizeof(Stream));

    //MsOS_WaitEvent(_s32EventId, _appDemo_Data_Ready, &u32Event, E_OR_CLEAR, 5000);
    WAIT_DATA_EVENT(DmxId, u32Event);

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

    appDemo_Dmx_close(DmxId);

    if (DMX_FILTER_STATUS_OK!= MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, inSrc ,FALSE,TRUE,TRUE))
    {
        appDemoDmx_print("switch to demod fail\n");
    }


    MsOS_FreeMemory(FileinAddr,gs32NonCachedPoolID);

    return TRUE ;

DMX_NMASK_FAIL:

    if (DMX_FILTER_STATUS_OK!= MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, inSrc ,FALSE,TRUE,TRUE))
    {
        appDemoDmx_print("switch to demod fail\n");
    }

    return FALSE ;

}



//------------------------------------------------------------------------------
/// @brief The sample code to demo PCR filter characteristics
/// @param[in] u32Pid Pid value of steam PCR.
/// @return TRUE  : testing success, print out pcr value .
/// @return FALSE : error  occur while opening filter.
/// @sa
/// @note
/// Command: \b dmx_pcr [Pid] \n
//------------------------------------------------------------------------------

MS_BOOL appDemo_Dmx_PCR(MS_U32 *u32Pid)
{

    MS_U32 i , DmxID ,PCR_H,PCR,PID;


    printf("PCR PID = %lx \n",*u32Pid);

    PID = *u32Pid;

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open((DMX_FILTER_TYPE)DMX_FILTER_TYPE_PCR, (MS_U8*)&DmxID))
    {
        printf("[%s][%d] FAIL !\n",__FUNCTION__,__LINE__);
        return false;
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(DmxID, (MS_U16*)&PID, TRUE))
    {
        printf("[%s][%d] FAIL !\n",__FUNCTION__,__LINE__);
        goto TSP_CREATE_FAIL;
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(DmxID))
    {
        printf("[%s][%d] FAIL !\n",__FUNCTION__,__LINE__);
        goto TSP_CREATE_FAIL;
    }


    for ( i = 0 ; i < 10 ; i ++)
    {

        MApi_DMX_Pcr_Get(&PCR_H,&PCR);
        printf("PCR value = %lx%08lx\n",PCR_H,PCR);
        MsOS_DelayTask(300);
    }

    return TRUE;

TSP_CREATE_FAIL:

    MApi_DMX_Close(DmxID);

    return FALSE;

}

MS_BOOL appDemo_Dmx_PES(MS_U32* pid)
{
    MS_U8                                  DmxId;
    MS_U32                                 DataLen,DataRmn;
    DMX_Flt_info                           FltInfo;
    MS_U16                                 u16Pid;
    MS_U8                                  pattern[16];
    MS_U8                                  mask[16];
    MS_U8                                  nmask[16];
    MS_U32                                 u32Event;
    DMX_FILTER_TYPE                        FilterType;
    MS_U8*                                 pu8BufAddr;
    MS_U32                                 u32Idx ;
    NotifyParam                            Pes_Param;

    u16Pid = *pid ;

    memset(pattern, 0x0, sizeof(pattern));
    memset(mask,    0x0, sizeof(mask));
    memset(nmask,   0x0, sizeof(nmask));




    // Set Parameter of Allocate Demux Filter type and input Source type.
    FilterType = DMX_FILTER_TYPE_PES| DMX_FILTER_SOURCE_TYPE_LIVE;

    // Allocate a Filter and set Filter Basic Type
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(FilterType, &DmxId))
    {
        appDemoDmx_print("[%s] Allocate filter fail \n",__FUNCTION__);
    }

    if (!Pes_Buf_Alloc(&u32Idx)) goto DMX_PES_FAIL;

    pu8BufAddr = (MS_U8*)Pes_Buf_Addr[u32Idx] ;

    memset((MS_U8*)MsOS_PA2KSEG1((MS_U32)pu8BufAddr), 0 , PES_BUFFER_SIZE);

    FltInfo.Info.SectInfo.SectBufAddr  =  (MS_U32)pu8BufAddr ;
    FltInfo.Info.SectInfo.SectBufSize  =  PES_BUFFER_SIZE;
    FltInfo.Info.SectInfo.SectMode     =  DMX_SECT_MODE_CRCCHK ;
    FltInfo.Info.SectInfo.Event        =  DMX_EVENT_DATA_READY | DMX_EVENT_BUF_OVERFLOW | DMX_EVENT_CB_SELF_TYPE2 ;  // DMX_EVENT_DATA_READY
    FltInfo.Info.SectInfo.pType2Notify  =  &_appDemo_DataCb_Type2;


    strcpy((char*)Pes_Param.name , "APP Demo Pes filter Notify parameter test");
    Pes_Param.Counter = 0 ;
    FltInfo.Info.SectInfo.Type2NotifyParam1 = (MS_U32)&Pes_Param;

    appDemoDmx_print("[%s] Allocate PES buffer = 0x%08lx , Size = 0x%x  \n",__FUNCTION__,FltInfo.Info.SectInfo.SectBufAddr,PES_BUFFER_SIZE);

    // Set Advanced Filter infomation
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Info( DmxId, &FltInfo, &FilterType, TRUE))
    {
        appDemoDmx_print("[%s] MApi_DMX_Info fail \n",__FUNCTION__);
        goto DMX_PES_FAIL;
    }

    // Set Filter PID --> Section PID
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid( DmxId, &u16Pid , TRUE))
    {
        appDemoDmx_print("[%s] MApi_DMX_Pid fail \n",__FUNCTION__);
        goto DMX_PES_FAIL;
    }

    MApi_DMX_SectPatternSet(DmxId, pattern, mask, nmask, 16);
    MApi_DMX_SectReset(DmxId);

    printf("dmx_set up ... \n");

    // Start Filter and record section into Section Buffer.
    if (DMX_FILTER_STATUS_OK!= MApi_DMX_Start(DmxId))
    {
        appDemoDmx_print("enable section filter fail\n");
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
        _appDemo_Pes_Parse(Data, DataLen);
    }
    else
    {
        printf("\n !! No Section Get - Wait timeout \n");
    }

    // Stop Filter
    MApi_DMX_Stop(DmxId);

    // Free Filter.
    MApi_DMX_Close(DmxId);

    //MsOS_FreeMemory(pu8BufAddr , 0);
    Pes_Buf_Free(u32Idx);

    return TRUE;

    DMX_PES_FAIL:
    MApi_DMX_Close(DmxId);
    Pes_Buf_Free(u32Idx);

    return FALSE ;
}


//------------------------------------------------------------------------------
/// @brief The sample code to get DMX Packet
/// @param pid	packet pid
/// @return TRUE: process success.
/// @return FALSE: process fail.
/// @sa
/// @note
///	Command:	\b DMX_Pkt	[PID] \n
///         		\b DMX_Pkt	2001 \n
//------------------------------------------------------------------------------

MS_BOOL appDemo_Dmx_Packet(MS_U32* pid)
{
    MS_U8                                  DmxId;
    MS_U32                                 DataLen,DataRmn;
    DMX_Flt_info                           FltInfo;
    MS_U16                                 u16Pid;
    MS_U8                                  pattern[16];
    MS_U8                                  mask[16];
    MS_U8                                  nmask[16];
    MS_U32                                 u32Event;
    DMX_FILTER_TYPE                        FilterType;
    MS_U8*                                 pu8BufAddr;
    u16Pid = *pid ;
    MS_U32                                 u32Idx;
    NotifyParam                            Pkt_Param;

    memset(pattern, 0x0, sizeof(pattern));
    memset(mask,    0x0, sizeof(mask));
    memset(nmask,   0x0, sizeof(nmask));

    // Set Parameter of Allocate Demux Filter type and input Source type.
    FilterType = DMX_FILTER_TYPE_PACKET| DMX_FILTER_SOURCE_TYPE_LIVE;

    // Allocate a Filter and set Filter Basic Type
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(FilterType, &DmxId))
    {
        appDemoDmx_print("[%s] Allocate filter fail \n",__FUNCTION__);
    }

    // Here we use pre-defined physical address of reserved section buffer.
    // [Note] The section buffe MUST be continus in physical address space.(for DMA usage)
    if (!Pes_Buf_Alloc(&u32Idx)) goto DMX_PKT_FAIL;

    pu8BufAddr = (MS_U8*)Pes_Buf_Addr[u32Idx] ;

    memset((MS_U8*)MsOS_PA2KSEG1((MS_U32)pu8BufAddr), 0 , PES_BUFFER_SIZE);

    // Transfer Virtual Address to Phisical Hardware Address
    // Section buffer is structed as ring buffer, keep 4 pointer of start,end ,read and write.
    FltInfo.Info.SectInfo.SectBufAddr = (MS_U32)pu8BufAddr ;

    appDemoDmx_print("Physical Address = %08lx\n" ,FltInfo.Info.SectInfo.SectBufAddr);
    // Set buffer size
    FltInfo.Info.SectInfo.SectBufSize =     PES_BUFFER_SIZE;
    appDemoDmx_print("Size = %08lx\n" ,FltInfo.Info.SectInfo.SectBufSize);

    FltInfo.Info.SectInfo.SectMode    =  DMX_SECT_MODE_CRCCHK ;

    // <DMX_EVENT_DATA_READY/DMX_EVENT_BUF_OVERFLOW>
    // Event trigger condition for driver, Driver will call ap's callback  < CallBack Mode Type2 >
    FltInfo.Info.SectInfo.Event        =     DMX_EVENT_DATA_READY | DMX_EVENT_BUF_OVERFLOW | DMX_EVENT_CB_SELF_TYPE2 ;  // DMX_EVENT_DATA_READY

    // Set the pointer of the event CB function into Demux driver
    FltInfo.Info.SectInfo.pType2Notify =    &_appDemo_DataCb_Type2;

    strcpy((char*)Pkt_Param.name,"APP Demo Packet filter Notify parameter test");
    Pkt_Param.Counter = 0 ;

    FltInfo.Info.SectInfo.Type2NotifyParam1 = (MS_U32)&Pkt_Param;

    // Set Advanced Filter infomation
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Info( DmxId, &FltInfo, &FilterType, TRUE))
    {
        appDemoDmx_print("[%s] MApi_DMX_Info fail \n",__FUNCTION__);
        goto DMX_PKT_FAIL;
    }

    // Set Filter PID --> Section PID
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid( DmxId, &u16Pid , TRUE))
    {
        appDemoDmx_print("[%s] MApi_DMX_Pid fail \n",__FUNCTION__);
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
        appDemoDmx_print("enable section filter fail\n");
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

    return FALSE ;
}

#if 0
//------------------------------------------------------------------------------
/// @brief The sample code to demo N Filter to 1 Buffer characteristics only supported by some chip.
/// @return TRUE: success / FALSE: fail
/// @sa
/// @note   Demo Nto1 characteristics and show how it works
/// Command: \b  \n
//------------------------------------------------------------------------------

void appDemo_Dmx_Nto1(void)
{
    MS_U32 DmxId[2];
    MS_U32 DmxPid[2];
    MS_U8  pattern[16],mask[16],nmask[16];
    MS_U32 u32Event;
    DMX_FILTER_TYPE                        FilterType;
    MS_U8 Data[1024];
    MS_U32 DataLen,DataRmn;
    DMX_Flt_info                           FltInfo;
    MS_U32 i ;
    DMX_Filein_info FileinInfo;

    printf("\n[%s] start!\n", __FUNCTION__);

    DmxPid[0] = 0xaaa;
    DmxPid[1] = 0x1555;

    memset(pattern , 0x0 , sizeof(pattern));
    memset(mask    , 0x0 , sizeof(mask));
    memset(nmask   , 0x0 , sizeof(nmask));

    appDemoDmx_Create();

    if (DMX_FILTER_STATUS_OK != MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, DMX_FLOW_INPUT_MEM, FALSE, TRUE, TRUE))
    {
            printf("[verDMX][%06d] switch to file-in fail\n", __LINE__);
    }
    appDemo_Dmx_setup(&DmxId[0] ,DmxPid[0] ,false ,pattern ,mask ,nmask ,true);
    printf("dmx_set up ... \n");
    // Start section filter
    appDemo_Dmx_start(DmxId[0]);
    printf("dmx_start Filter %ld Pid = %04lx... \n",DmxId[0],DmxPid[0]);


    FilterType = DMX_FILTER_TYPE_SECTION | DMX_FILTER_SOURCE_TYPE_FILE;
    // Open New Filter and attach it with target Filter
    // DmxId[1] will share the same buffer with DmxId[0]
    //
    if(DMX_FILTER_STATUS_OK != MApi_DMX_Open_MultiFlt(FilterType ,(MS_U8*)&DmxId[1],DmxId[0]))
    {
        printf("[%d] Error\n" ,__LINE__);
    }
    printf("DmxId[%d] = %ld\n", (int)0x1555, DmxId[1]);

    // DmxId[1] share the same buffer object with DmxId[0]
    // If we Pass Different buffer setting into MApi_DMX_Info, the Buffer setting will be updated
    // to new one.
    // [NOTE] We should disable all running filters when trying to setup a new filter
    // and attach it with the same buffer
    FltInfo.Info.SectInfo.SectBufAddr   =   RESERVED_BUFFER_ADDRESS ; //the same buffer setting
    FltInfo.Info.SectInfo.SectBufSize   =   appDemoDmx_Sec_BufSize ;  //the same buffer setting
    FltInfo.Info.SectInfo.SectMode      =   DMX_SECT_MODE_CONTI|DMX_SECT_MODE_CRCCHK; // filter 2 setting
    FltInfo.Info.SectInfo.Event         =   DMX_EVENT_DATA_READY | DMX_EVENT_CB_SELF ; // DMX_EVENT_BUF_OVERFLOW
    FltInfo.Info.SectInfo.pNotify       =   _appDemo_DataCb;

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Info(DmxId[1], &FltInfo, &FilterType, TRUE))
    {
        printf("[%d] Error\n" ,__LINE__);
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(DmxId[1], (MS_U16*)&DmxPid[1], TRUE))
    {
        printf("[%d] Error\n" ,__LINE__);
    }
    if (DMX_FILTER_STATUS_OK != MApi_DMX_SectPatternSet(DmxId[1], pattern, mask, nmask, 16))
    {
        printf("[%d] Error\n" ,__LINE__);
    }
    MApi_DMX_SectReset(DmxId[1]);

    // [NOTE] Section Buffer will be reset when any associated filter start
    // User have to copy the data in the buufer if they don't want to lose received data.
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Start(DmxId[1]))
    {
        printf("[%d] Error\n" ,__LINE__);
    }
    printf("dmx_start Filter %ld Pid = %04lx... \n",DmxId[1],DmxPid[1]);



    FileinInfo.PKT_Mode = DMX_PKTMODE_188;
    // Set filein input delay 0x0 ~ 0xFFFFFF
    FileinInfo.Rate = 0x0000ffff;
    // set filein parameter
    printf("FileinStart init\n");
    MApi_DMX_Filein_Info(&FileinInfo);
    memcpy( (MS_U8*)MsOS_PA2KSEG1(0x0),Stream,sizeof(Stream));
    MApi_DMX_Filein_Start(DMX_PES_NO_BYPASS_FIFO, (MS_PHYADDR)(0x0), sizeof(Stream));
    printf("FileinStart  buf = %08lx \n",(MS_U32)RESERVED_BUFFER_ADDRESS);



    MsOS_WaitEvent(_s32EventId, _appDemo_Data_Ready, &u32Event, E_OR_CLEAR, 5000);
    MApi_DMX_CopyData(DmxId[0],(MS_U8*)&Data,1024,&DataLen,&DataRmn,NULL);

    while(DataLen)
    {
        printf("\nCopy_Data loop\n");
        data_dump(Data,DataLen);
        MApi_DMX_CopyData(DmxId[0],(MS_U8*)&Data,1024,&DataLen,&DataRmn,NULL);
    }

    for (i = 0; i < 2; i++)
    {
         MApi_DMX_Stop(DmxId[i]);
         MApi_DMX_Close(DmxId[i]);
    }
    MsOS_DelayTask(1000);

    appDemoDmx_Delete();
    printf("\n[%s] end!\n", __FUNCTION__);
}

#endif

MS_BOOL appDemo_Dmx_TSO(MS_U8* pu8TSOEng)
{
    DMX_FLOW DmxFlow = DMX_FLOW_NUM;
    if(*pu8TSOEng == 0)
        DmxFlow = DMX_FLOW_TSO_PLAYBACK;
    else if(*pu8TSOEng == 1)
        DmxFlow = DMX_FLOW_TSO_PLAYBACK1;
    else
        printf("TSO Eng num not supported!!\n");

    DMX_FLOW_INPUT eDMXFlowInput = _GetDMXFowInput();
    if (DMX_FILTER_STATUS_OK!= MApi_DMX_FlowSet(DmxFlow, eDMXFlowInput ,FALSE,TRUE,TRUE))
    {
        printf("switch to demod fail\n");
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The help function for demo demux app
/// @return TRUE: success / FALSE: fail
/// @sa
/// @note
/// Command: \b HelpDMX \n
//------------------------------------------------------------------------------
MS_BOOL appDemo_Dmx_help(void)
{

    printf("*************************Demux function List*************************\n");
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
    printf("---------------------------------------------------------------------\n");
    printf("3. DMX_Sec\n");
    printf("Usage:dmx_sec [Pid] [Pattern0] [Pattern1] [Mask0] [Mask1]\n");
    printf("    Pid: Pid filter in hex\n");
    printf("    Pattern0: match pattern from byte 0~3 in hex\n");
    printf("    Pattern1: match pattern from byte 4~7 in hex\n");
    printf("    mask0: mask from byte 0~3 in hex\n");
    printf("    mask1: mask from byte 4~7 in hex\n");
    printf("    *Note: our TSP support 16 bytes patterns and mask.\n");
    printf("           In this demo program, however, 2 bytes are only used for demo correctness\n");
    printf("\n");
    printf("Sample UART Command:DMX_Sec  0x0         0x0 0x0 0xFF000000 0x0\n");
    printf("Sample UART Command:DMX_Sec [Pid] 0x02000000 0x0 0xFF000000 0x0\n");
    printf("\n");
    printf("---------------------------------------------------------------------\n");
    printf("4. DMX_Scan : Scan Demo code\n");
    printf("Usage:dmx_scan\n");
    printf("      Scan programs of current frequency\n");
    printf("Sample UART Command: dmx_scan \n");
    printf("\n");
    printf("---------------------------------------------------------------------\n");
    printf("5. DMX_Nmask : demo code for not Match mask \n");
    printf("Usage:dmx_Nmask [pattern] [mask] [nmask] \n");
    printf("\n");
    printf("Sample UART Command: dmx_nmask 40800000 FFFF0000 00000000\n");
    printf("                     dmx_nmask 40800000 FFFF0000 F0000000\n");
    printf("                     dmx_nmask 40800000 FFFF0000 0F000000\n");
    printf("                     dmx_nmask 40800000 FFFF0000 FF000000\n");
    printf("---------------------------------------------------------------------\n");
    printf("6. DMX_Pcr\n");
    printf("Usage:dmx_pcr [PID]\n");
    printf("\n");
    printf("Sample UART Command: dmx_pcr  [Pid] \n");
    printf("\n");
    printf("---------------------------------------------------------------------\n");
    printf("7. DMX_Pes : Filter PES packet data into buffer/\n");
    printf("Usage:dmx_Pes [PID]  \n");
    printf("\n");
    printf("Sample UART Command: dmx_pes 0x302\n");
    printf("---------------------------------------------------------------------\n");
    printf("8. DMX_Pkt : filter target Pid Packet into buffer. \n");
    printf("Usage:dmx_pkt [PID]\n");
    printf("\n");
    printf("Sample UART Command: dmx_pkt  [Pid] \n");
    printf("---------------------------------------------------------------------\n");
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
    printf("***********************End of Demux function List*************************\n");
    return TRUE;
}
#endif

