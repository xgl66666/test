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
/// @file   PVRPL_FileIn.c
/// @author MStar Semiconductor Inc.
/// @brief  FileIn Eng porting layer
///
/// @name how to port FileIn Eng
/// @verbatim
///=================================================================================================
#include <string.h>

#include "apiDMX.h"
#include "MsMemory.h"
#include "MsCommon.h"
#include "PVRPL_FileIn.h"
#include "PVRPL_Sys.h"
#include "PVRPL_Video.h"
#include "PVRPL_ResourcePool.h"
#include "drvDTC.h"


extern MS_S32 gs32NonCachedPoolID;

#define CMDQ_TIMEOUT        1000
#define FILEIN_ENG_NUM      2

// Debug level
static PVRPL_DBGMSG_LEVEL _ePVRPL_FileInDbgLevel = PVRPL_DBG_ERR;

#define PVRPL_FILEIN_DBGMSG(_level,msg, args...)        {if(_ePVRPL_FileInDbgLevel >= (_level)) printf("[%s][%d] " msg, __FUNCTION__, __LINE__, ## args);}
#define MOD_NAME                                        PVR

#if defined(HB_ERR)
    #define PVRPL_FILEIN_DBGMSG_ERR(msg, args...)       HB_ERR(msg, ##args)
#else
    #define PVRPL_FILEIN_DBGMSG_ERR(msg, args...)       PVRPL_FILEIN_DBGMSG(PVRPL_DBG_ERR, msg, ##args)
#endif

#if defined(HB_INFO)
    #define PVRPL_FILEIN_DBGMSG_INFO(msg, args...)      HB_INFO(msg, ##args)
#else
    #define PVRPL_FILEIN_DBGMSG_INFO(msg, args...)      PVRPL_FILEIN_DBGMSG(PVRPL_DBG_INFO, msg, ##args)
#endif

#if defined(HB_TRACE)
    #define PVRPL_FILEIN_DBGMSG_TRACE(msg, args...)     HB_TRACE(msg, ##args)
#else
    #define PVRPL_FILEIN_DBGMSG_TRACE(msg, args...)     PVRPL_FILEIN_DBGMSG(PVRPL_DBG_TRACE, msg, ##args)
#endif

#if defined(HB_DBG)
    #define PVRPL_FILEIN_DBGMSG_DEBUG(msg, args...)     HB_DBG(msg, ##args)
#else
    #define PVRPL_FILEIN_DBGMSG_DEBUG(msg, args...)     PVRPL_FILEIN_DBGMSG(PVRPL_DBG_FUNC, msg, ##args)
#endif

//@TODO consider record start time is not 0

//@NOTE
//Let User to select which filein eng
static DMX_FILTER_TYPE _PVRPL_FileIn_Mapping_DmxFltSrc(EN_PVRPL_FILEIN_PATH enFileInPath)
{
    switch(enFileInPath)
    {
        case E_PVRPL_FILEIN_TSIF0:
            return DMX_FILTER_SOURCE_TYPE_FILE;
        case E_PVRPL_FILEIN_TSIF1:
            return  DMX_FILTER_SOURCE_TYPE_FILE1;
        case E_PVRPL_FILEIN_TSIF2:
            return  DMX_FILTER_SOURCE_TYPE_FILE2;
        case E_PVRPL_FILEIN_TSIF3:
            return  DMX_FILTER_SOURCE_TYPE_FILE3;
        default:
            PVRPL_FILEIN_DBGMSG_ERR("Wrong PVRPL FileIn Path\n");
            return DMX_FILTER_SOURCE_TYPE_FILE;
    }
}

//@NOTE
//Let User to select which A/V FIFO
static DMX_FILTER_TYPE _PVRPL_FileIn_Mapping_DmxFltType(MS_U32 u32PVRPLFltType)
{
    switch(u32PVRPLFltType)
    {
        case PVRPL_FILEIN_FILTER_TYPE_VIDEO:
            return DMX_FILTER_TYPE_VIDEO;
        case PVRPL_FILEIN_FILTER_TYPE_VIDEO3D:
            return  DMX_FILTER_TYPE_VIDEO3D;
        case PVRPL_FILEIN_FILTER_TYPE_AUDIO:
            return  DMX_FILTER_TYPE_AUDIO;
        case PVRPL_FILEIN_FILTER_TYPE_AUDIO2:
            return  DMX_FILTER_TYPE_AUDIO2;
        case PVRPL_FILEIN_FILTER_TYPE_AUDIO3:
            return  DMX_FILTER_TYPE_AUDIO3;
        case PVRPL_FILEIN_FILTER_TYPE_AUDIO4:
            return  DMX_FILTER_TYPE_AUDIO4;
        case PVRPL_FILEIN_FILTER_TYPE_PCR:
            return  DMX_FILTER_TYPE_PCR;
        case PVRPL_FILEIN_FILTER_TYPE_SECTION:
            return  DMX_FILTER_TYPE_SECTION;
        case PVRPL_FILEIN_FILTER_TYPE_PES:
            return  DMX_FILTER_TYPE_PES;
        default:
            PVRPL_FILEIN_DBGMSG_ERR("Wrong PVRPL Flt Type:%"DTC_MS_U32_d"\n",u32PVRPLFltType);
            return DMX_FILTER_TYPE_VIDEO;
    }
}
static DMX_FILEIN_PATH _PVRPL_FileIn_Mapping_DmxFileInPath(EN_PVRPL_FILEIN_PATH ePVRPLPath)
{
    switch(ePVRPLPath)
    {
        case E_PVRPL_FILEIN_TSIF0:
            return DMX_FILEIN_TSIF0;
        case E_PVRPL_FILEIN_TSIF1:
            return  DMX_FILEIN_TSIF1;
        case E_PVRPL_FILEIN_TSIF2:
            return  DMX_FILEIN_TSIF2;
        case E_PVRPL_FILEIN_TSIF3:
            return  DMX_FILEIN_TSIF3;
        default:
            PVRPL_FILEIN_DBGMSG_ERR("Wrong PVRPL FileIn Path:%d\n",ePVRPLPath);
            return E_PVRPL_FILEIN_TSIF0;
    }

}

static EN_PVRPL_FILEIN_PATH _DmxFileInPath_Mapping_PVRPL_FileIn(DMX_FILEIN_PATH eDMXPath)
{
    switch(eDMXPath)
    {
        case DMX_FILEIN_TSIF0:
            return E_PVRPL_FILEIN_TSIF0;
        case DMX_FILEIN_TSIF1:
            return E_PVRPL_FILEIN_TSIF1;
        case DMX_FILEIN_TSIF2:
            return E_PVRPL_FILEIN_TSIF2;
        case DMX_FILEIN_TSIF3:
            return E_PVRPL_FILEIN_TSIF3;
        default:
            PVRPL_FILEIN_DBGMSG_ERR("Wrong DMX FileIn Path:%d\n",eDMXPath);
            return E_PVRPL_FILEIN_TSIF0;
    }

}

static DMX_FLOW _PVRPL_DmxFlow_Mapping_DmxFlow(EN_PVRPL_DMX_FLOW ePVRPLDmxFlow)
{
    switch(ePVRPLDmxFlow)
    {

        case E_PVRPL_DMX_FLOW_PLAYBACK:
            return DMX_FLOW_PLAYBACK;
        case E_PVRPL_DMX_FLOW_PVR:
            return DMX_FLOW_PVR;
        case E_PVRPL_DMX_FLOW_PVR1:
            return  DMX_FLOW_PVR1;

        case E_PVRPL_DMX_FLOW_PVRCA:
            return  DMX_FLOW_PVRCA;

        case E_PVRPL_DMX_FLOW_FILEIN_MM:
            return  DMX_FLOW_FILEIN_MM;

        case E_PVRPL_DMX_FLOW_FILEIN_MM3D:
            return  DMX_FLOW_FILEIN_MM3D;


        case E_PVRPL_DMX_FLOW_CIPHSS_PLAYBACK:
            return  DMX_FLOW_CIPHSS_PLAYBACK;
        case E_PVRPL_DMX_FLOW_CIPHSS_PVRCA:
            return  DMX_FLOW_CIPHSS_PVRCA;

        case E_PVRPL_DMX_FLOW_PVR2:
            return  DMX_FLOW_PVR2;
        case E_PVRPL_DMX_FLOW_PVR3:
            return  DMX_FLOW_PVR3;
        case E_PVRPL_DMX_FLOW_PVR4:
            return  DMX_FLOW_PVR4;
        case E_PVRPL_DMX_FLOW_PVR5:
            return  DMX_FLOW_PVR5;

        case E_PVRPL_DMX_FLOW_TSO_PLAYBACK:
            return  DMX_FLOW_TSO_PLAYBACK;
        case E_PVRPL_DMX_FLOW_PLAYBACK1:
            return  DMX_FLOW_PLAYBACK1;
        case E_PVRPL_DMX_FLOW_PVRCA1:
            return  DMX_FLOW_PVRCA1;

        case E_PVRPL_DMX_FLOW_TSO_PLAYBACK1:
            return  DMX_FLOW_TSO_PLAYBACK1;
        case E_PVRPL_DMX_FLOW_PLAYBACK2:
            return  DMX_FLOW_PLAYBACK2;
        case E_PVRPL_DMX_FLOW_PLAYBACK3:
            return  DMX_FLOW_PLAYBACK3;


        default:
            PVRPL_FILEIN_DBGMSG_ERR("Wrong PVRPL Flt Type:%d\n",ePVRPLDmxFlow);
            return DMX_FLOW_PLAYBACK;
    }
}
static EN_PVRPL_DMX_FLOW _DmxFlow_Mapping_PVRPL_DmxFlow(DMX_FLOW eDmxFlow)
{
    switch(eDmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
            return E_PVRPL_DMX_FLOW_PLAYBACK;
        case DMX_FLOW_PVR:
            return E_PVRPL_DMX_FLOW_PVR;
        case DMX_FLOW_PVR1:
            return  E_PVRPL_DMX_FLOW_PVR1;

        case DMX_FLOW_PVRCA:
            return  E_PVRPL_DMX_FLOW_PVRCA;

        case DMX_FLOW_FILEIN_MM:
            return  E_PVRPL_DMX_FLOW_FILEIN_MM;

        case DMX_FLOW_FILEIN_MM3D:
            return  E_PVRPL_DMX_FLOW_FILEIN_MM3D;


        case DMX_FLOW_CIPHSS_PLAYBACK:
            return  E_PVRPL_DMX_FLOW_CIPHSS_PLAYBACK;
        case DMX_FLOW_CIPHSS_PVRCA:
            return  E_PVRPL_DMX_FLOW_CIPHSS_PVRCA;

        case DMX_FLOW_PVR2:
            return  E_PVRPL_DMX_FLOW_PVR2;
        case DMX_FLOW_PVR3:
            return  E_PVRPL_DMX_FLOW_PVR3;
        case DMX_FLOW_PVR4:
            return  E_PVRPL_DMX_FLOW_PVR4;
        case DMX_FLOW_PVR5:
            return  E_PVRPL_DMX_FLOW_PVR5;

        case DMX_FLOW_TSO_PLAYBACK:
            return  E_PVRPL_DMX_FLOW_TSO_PLAYBACK;
        case DMX_FLOW_PLAYBACK1:
            return  E_PVRPL_DMX_FLOW_PLAYBACK1;
        case DMX_FLOW_PVRCA1:
            return  E_PVRPL_DMX_FLOW_PVRCA1;

        case DMX_FLOW_TSO_PLAYBACK1:
            return  E_PVRPL_DMX_FLOW_TSO_PLAYBACK1;
        case DMX_FLOW_PLAYBACK2:
            return  E_PVRPL_DMX_FLOW_PLAYBACK2;
        case DMX_FLOW_PLAYBACK3:
            return  E_PVRPL_DMX_FLOW_PLAYBACK3;

        default:
            PVRPL_FILEIN_DBGMSG_ERR("Wrong DMX Flow:%d\n",eDmxFlow);
            return E_PVRPL_DMX_FLOW_PLAYBACK;
    }
}

static DMX_FILTER_TYPE _PVRPL_FileIn_Mapping_DmxFltDst(EN_PVRPL_FILEIN_PATH enFileInPath,DMX_FILTER_TYPE u32FltType)
{
    DMX_FILTER_TYPE sFltDst=0;

    switch(enFileInPath)
    {
        case E_PVRPL_FILEIN_TSIF0:
            if((u32FltType&0xF) == DMX_FILTER_TYPE_VIDEO)
            {
                PVRPL_FILEIN_DBGMSG_TRACE("To Video FIFO\n");
                sFltDst = DMX_FILTER_TYPE_VIDEO;
            }
            else if((u32FltType&0xF) == DMX_FILTER_TYPE_AUDIO)
            {
                PVRPL_FILEIN_DBGMSG_TRACE("To Audio FIFO\n");
                sFltDst = DMX_FILTER_TYPE_AUDIO;
            }
            else if((u32FltType&0xF) == DMX_FILTER_TYPE_PCR)
            {
                PVRPL_FILEIN_DBGMSG_TRACE("To PCR FIFO\n");
                sFltDst = DMX_FILTER_TYPE_PCR;
            }
            else
            {
                sFltDst = u32FltType;
            }
            break;
        case E_PVRPL_FILEIN_TSIF1:
            if((u32FltType&0xF) == DMX_FILTER_TYPE_VIDEO)
            {
                PVRPL_FILEIN_DBGMSG_TRACE("To Video3D FIFO\n");
                sFltDst = DMX_FILTER_TYPE_VIDEO3D;
            }
            else if((u32FltType&0xF) == DMX_FILTER_TYPE_AUDIO)
            {
                PVRPL_FILEIN_DBGMSG_TRACE("To Audio2 FIFO\n");
                sFltDst = DMX_FILTER_TYPE_AUDIO2;
            }
            else if((u32FltType&0xF) == DMX_FILTER_TYPE_PCR)
            {
                PVRPL_FILEIN_DBGMSG_TRACE("To Pcr FIFO\n");
                sFltDst = DMX_FILTER_TYPE_PCR;
            }
            else
            {
                sFltDst = u32FltType;
            }
            break;
        default:
            PVRPL_FILEIN_DBGMSG_ERR("Wrong Path:%d\n",enFileInPath);
            sFltDst = u32FltType;
            break;
    }

    return sFltDst;
}

/************************************************************************************/
/* Reset FileIn Queue                                                                                                                  */
/* @param    sCurFileInInfo                           \b IN: pointer of FileInQueue info                              */
/************************************************************************************/
void _FlushFileInQueue(FileInResource *sCurFileInQueueInfo)
{
    sCurFileInQueueInfo->u32NumofFeeded = 0;
    sCurFileInQueueInfo->u8Feeded2HW = 0;
    sCurFileInQueueInfo->u8QueFront = 0;
    sCurFileInQueueInfo->u8QueRear = 0;
    sCurFileInQueueInfo->bQueFullTag = FALSE;
}

static MS_U8 _CalculateFileInQueueWaterLevel(FileInResource *sCurFileInInfo)
{
    MS_U32 u32CmdEmptyCount = 0;
    MS_U8 u8CmdWRLevel = 0;
    MApi_DMX_Filein_Eng_CMDQ_GetEmptyNum(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath),&u32CmdEmptyCount);

    // each CMD uses 5 blocks
    if (u32CmdEmptyCount <= 1)       //  0 - 1
    {
        u8CmdWRLevel = 0x04;
    }
    else if (u32CmdEmptyCount <= 6)  //  2 - 6
    {
        u8CmdWRLevel = 0x03;
    }
    else if (u32CmdEmptyCount <= 11) //  7 - 11
    {
        u8CmdWRLevel = 0x02;
    }
    else if (!MApi_DMX_Filein_Eng_IsIdle(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath))) // 12 - 16 && Busy
    {
        u8CmdWRLevel = 0x01;
    }
    else    // 12 - 16 && Idle
    {
        u8CmdWRLevel = 0x0;
    }

    PVRPL_FILEIN_DBGMSG_TRACE("u32CmdCount:%"DTC_MS_U32_u" u8CmdWRLevel:%d\n", u32CmdEmptyCount,u8CmdWRLevel);

    return u8CmdWRLevel;
}

static EN_PVRPL_FILEIN_STATUS _PVRPL_FileIn_MoveFileInBlockToFileInEng(FileInResource *sCurFileInInfo)
{
    sCurFileInInfo->u32CurWaterLevel = _CalculateFileInQueueWaterLevel(sCurFileInInfo);

    if ( sCurFileInInfo->u32CurWaterLevel < sCurFileInInfo->u32NumofFeeded)
    {
        PVRPL_FILEIN_DBGMSG_TRACE("[Consumer]------- (Cur:FeedNum)(%"DTC_MS_U32_u":%"DTC_MS_U32_u") -Not Full-\n",sCurFileInInfo->u32CurWaterLevel,sCurFileInInfo->u32NumofFeeded);
        MS_U32 u32CousumedCnt =  sCurFileInInfo->u32NumofFeeded - sCurFileInInfo->u32CurWaterLevel;
        sCurFileInInfo->u8QueFront = (sCurFileInInfo->u8QueFront + u32CousumedCnt)% sCurFileInInfo->u8MaxQueNum;
        sCurFileInInfo->u32NumofFeeded = sCurFileInInfo->u32NumofFeeded - u32CousumedCnt;
        sCurFileInInfo->bQueFullTag = FALSE;
    }

    if ((sCurFileInInfo->u8QueRear == sCurFileInInfo->u8Feeded2HW) /*&& (sCurFileInInfo->bQueFullTag == FALSE)*/)
    {
        // no data can be feed to TSP HW
        PVRPL_FILEIN_DBGMSG_TRACE("[Consumer]------- -Empty-\n");
        PVRPL_FILEIN_DBGMSG_TRACE("FileInBlock Queue is Empty\n");
    }
    else
    {
        // has data to feed
        //@TODO need to use normal or trick mode variable
        if (sCurFileInInfo->u32CurWaterLevel > sCurFileInInfo->eQueThreshold)
        {
            //TSP full cannot feed data

            PVRPL_FILEIN_DBGMSG_TRACE("FileIn Eng is too busy to send cmdq\n");
            PVRPL_FILEIN_DBGMSG_TRACE("[Consumer]------- (Cur:%"DTC_MS_U32_u") -TSP BUSY-\n",sCurFileInInfo->u32CurWaterLevel);
            PVRPL_FILEIN_DBGMSG_TRACE("FileIn Eng is too busy to send cmdq => WaterLevel:%d Threshold:%d\n", _CalculateFileInQueueWaterLevel(sCurFileInInfo),sCurFileInInfo->eQueThreshold);
            return E_PVRPL_FILEIN_FAIL;
        }
        else
        {
            // TSP had slot to feed
            MS_U32 u32PhyAddr = sCurFileInInfo->psFileInQueue[sCurFileInInfo->u8Feeded2HW].u32PhyAddr;
            MS_U32 u32Size = sCurFileInInfo->psFileInQueue[sCurFileInInfo->u8Feeded2HW].u32Size;
            PVRPL_FILEIN_DBGMSG_TRACE("[Consumer] (Front:Addr)(%d:%"DTC_MS_U32_x")\n",sCurFileInInfo->u8Feeded2HW,u32PhyAddr);

            sCurFileInInfo->u8Feeded2HW = (sCurFileInInfo->u8Feeded2HW +1)% sCurFileInInfo->u8MaxQueNum;
            sCurFileInInfo->u32NumofFeeded++;

            MS_PHY PHYPhyAddr = (MS_PHY)u32PhyAddr;
            MApi_DMX_Filein_Eng_Start(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath),DMX_PES_NO_BYPASS_FIFO,PHYPhyAddr,u32Size);

            //debug
            MS_U32 u32viraddr = MsOS_PA2KSEG1(PHYPhyAddr);

            if(((MS_U8 *)u32viraddr)[4]!=0x47)
                PVRPL_FILEIN_DBGMSG_TRACE("Path %d Consume ====> TimeStamp to FILE-IN Eng:0x%x 0x%x 0x%x 0x%x 0x%x \n",GET_PVR_PATH_IDX(sCurFileInInfo->u8PathIdx),((MS_U8 *)u32viraddr)[0],((MS_U8 *)u32viraddr)[1],((MS_U8 *)u32viraddr)[2],((MS_U8 *)u32viraddr)[3],((MS_U8 *)u32viraddr)[4]);
            //    video_fifo, audio_fifo,(video_fifo&DMX_FIFO_STATUS_LV_EMPTY), (audio_fifo&DMX_FIFO_STATUS_LV_EMPTY));
            //MS_U32 u32viraddr = MsOS_PA2KSEG1(u32PhyAddr);
            //PVRPL_FILEIN_DBGMSG_TRACE("Consume ====> TimeStamp to FILE-IN Eng:0x%x 0x%x 0x%x 0x%x 0x%x  VIDEO State: %lu  AUDIO State: %lu VIDEO EMPTY: %lu  AUDIO EMPTY: %lu\n",((MS_U8 *)u32viraddr)[0],((MS_U8 *)u32viraddr)[1],((MS_U8 *)u32viraddr)[2],((MS_U8 *)u32viraddr)[3],((MS_U8 *)u32viraddr)[4],
            //    video_fifo, audio_fifo,(video_fifo&DMX_FIFO_STATUS_LV_EMPTY), (audio_fifo&DMX_FIFO_STATUS_LV_EMPTY));
            PVRPL_FILEIN_DBGMSG_TRACE("Path:%d Consume =>  u8QueRear:%d u8QueFront:%d PhyAddr:%"DTC_MS_U32_x" Size:%"DTC_MS_U32_x"\n", GET_PVR_PATH_IDX(sCurFileInInfo->u8PathIdx),sCurFileInInfo->u8QueRear,sCurFileInInfo->u8QueFront,u32PhyAddr,u32Size);
        }

    }

    return E_PVRPL_FILEIN_SUCESS;
}

static EN_PVRPL_FILEIN_STATUS _FlushFileInQueueAndEng(FileInResource *sCurFileInInfo)
{
    PVRPL_FILEIN_DBGMSG_TRACE("FUCHSIA CALL ============> FLUSH Start !!!\n");

    MS_U32 u32CurrTime=0;
    u32CurrTime = MsOS_GetSystemTime();

    #if 0
    while(PVRPL_FileIn_IsFileInBlockEmpty() == FALSE)
    {
         _PVRPL_FileIn_MoveFileInBlockToFileInEng();
         while (!(MApi_DMX_Filein_IsIdle()))
         {
            // Delay 3 ms.
            // Reset AV FIFO with 384K * 3 takes 15-20 ms if bypass file-in timestamp.
            if (MsOS_GetSystemTime() - u32CurrTime > CMDQ_TIMEOUT)
            {
                printf("PVR_TSP::WaitForEmptyCMDQ Fail! IsFileInIdle() time out!\n");
                PVRPL_FILEIN_DBGMSG_ERR("<ERR> IsFileInIdle() time out! duration:%lu CMDQ_TIMEOUT:%d\n", (MsOS_GetSystemTime() - u32CurrTime),CMDQ_TIMEOUT);
                MsOS_ReleaseMutex(s32FileinMutex);

                printf("FUCHSIA CALL ============> FLUSH Done TimeOut!!!\n");
                return E_PVRPL_FILEIN_FAIL;
            }
            MsOS_DelayTask(3);
         }
    }
    #endif

     _FlushFileInQueue(sCurFileInInfo);

    while(MApi_DMX_Filein_Eng_IsIdle(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath)) != TRUE)
    {
        // Delay 3 ms.
        // Reset AV FIFO with 384K * 3 takes 15-20 ms if bypass file-in timestamp.
        if (MsOS_GetSystemTime() - u32CurrTime > CMDQ_TIMEOUT)
        {
            PVRPL_FILEIN_DBGMSG_TRACE("PVR_TSP::WaitForEmptyCMDQ Fail! IsFileInIdle() time out!\n");
            PVRPL_FILEIN_DBGMSG_ERR("<ERR> MApi_DMX_Filein_Eng_IsIdle() time out! duration:%"DTC_MS_U32_u"\n", (MsOS_GetSystemTime() - u32CurrTime));

            PVRPL_FILEIN_DBGMSG_ERR("FUCHSIA CALL ============> MApi_DMX_Filein_Eng_IsIdle TimeOut!!!\n");
            return E_PVRPL_FILEIN_FAIL;
        }
        MsOS_DelayTask(3);
    }

    PVRPL_FILEIN_DBGMSG_TRACE("\n");
    return E_PVRPL_FILEIN_SUCESS;
}


//@NOTE This function is implemented to
//1. Initiate all variables used in File-In Eng
//2. Create a queue to store file-in cmds
//3. Get plabyack destination and audio/video resource information from resource pools
//4. Set playback flow from Memory and playback destination
//5. Enable Playback timestamp
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_Init(FileInResource *sCurFileInInfo)
{
    MS_U8 u8PathID = GET_PVR_PATH_IDX(sCurFileInInfo->u8PathIdx);

    if(u8PathID>= FILEIN_ENG_NUM)
    {
        PVRPL_FILEIN_DBGMSG_ERR("No Free FileIn Eng!!! Init Fail\n");
        return E_PVRPL_FILEIN_FAIL;
    }
    else
    {
        if(sCurFileInInfo->bInit == TRUE)
        {
            PVRPL_FILEIN_DBGMSG_ERR("File In Eng Already Init!\n");
            return E_PVRPL_FILEIN_FAIL;
        }
        sCurFileInInfo->bInit=TRUE;
    }

    //reset AV FIFO
    MApi_DMX_AVFifo_Reset(Pool_GetVFifoType(u8PathID), TRUE);
    MApi_DMX_AVFifo_Reset(Pool_GetAFifoType(u8PathID), TRUE);

    //@TODO get resource from resource controller
    // RS_POOL_FileIn_Get().....
    sCurFileInInfo->eAVFIFO.eAudioType = (MS_U32)Pool_GetAFifoType(u8PathID);
    sCurFileInInfo->eAVFIFO.eVideoType = (MS_U32)Pool_GetVFifoType(u8PathID);
    sCurFileInInfo->eDMXFlow =  _DmxFlow_Mapping_PVRPL_DmxFlow(Pool_GetPVRDMXFlow(u8PathID));
    sCurFileInInfo->bByPassFileIn = FALSE;
    sCurFileInInfo->eFileInPath = _DmxFileInPath_Mapping_PVRPL_FileIn(Pool_GetFileInSouce(u8PathID));

    sCurFileInInfo->psFileInQueue = (FileInBlock *)MsOS_AllocateMemory((sizeof(FileInBlock))*(sCurFileInInfo->u8MaxQueNum),gs32NonCachedPoolID);
    sCurFileInInfo->u32NumofFeeded = 0;
    sCurFileInInfo->u32CurWaterLevel = 0;
    sCurFileInInfo->u8QueFront = 0;
    sCurFileInInfo->u8QueRear= 0;
    sCurFileInInfo->u8Feeded2HW = 0;
    sCurFileInInfo->bQueFullTag = FALSE;
    sCurFileInInfo->eQueThreshold = EN_FILEIN_NORMAL_MODE;
    sCurFileInInfo->u8STCEng = (MS_U8)E_PVRPL_PCR_ENG_INVALID;

    DMX_Filein_info tsFileInInfo;

    PVRPL_FILEIN_DBGMSG_TRACE("eDMXFlow:%d u8FileInPktMode:%d\n", sCurFileInInfo->eDMXFlow,sCurFileInInfo->u8FileInPktMode);
    PVRPL_FILEIN_DBGMSG_TRACE("eFileInPath:%d u8STCEng:%d eAudioType:%"DTC_MS_U32_u" eVideoType:%"DTC_MS_U32_u" u8MaxQueNum:%d\n",sCurFileInInfo->eFileInPath,sCurFileInInfo->u8STCEng,
                        sCurFileInInfo->eAVFIFO.eAudioType,sCurFileInInfo->eAVFIFO.eVideoType,sCurFileInInfo->u8MaxQueNum);

    //MApi_DMX_FlowSet(sFileInInfo.eDMXFlow,DMX_FLOW_INPUT_MEM,FALSE,FALSE,TRUE);//spike original
    MApi_DMX_FlowSet(_PVRPL_DmxFlow_Mapping_DmxFlow(sCurFileInInfo->eDMXFlow),DMX_FLOW_INPUT_MEM,FALSE,TRUE,TRUE);

    if(sCurFileInInfo->u8FileInPktMode == 192)
    {
        tsFileInInfo.PKT_Mode = DMX_PKTMODE_192;
        tsFileInInfo.Rate = 0;
        MApi_DMX_Filein_Eng_PlaybackTimeStampEnable(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath));
        MApi_DMX_Filein_Eng_SetPlaybackStamp(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath),0);  //mike temporily added
        PVRPL_FILEIN_DBGMSG_TRACE("FileIn PacketMode = 192\n");
    }
    else if(sCurFileInInfo->u8FileInPktMode == 188)
    {
        tsFileInInfo.PKT_Mode = DMX_PKTMODE_188;
        tsFileInInfo.Rate = 0;
        MApi_DMX_Filein_Eng_PlaybackTimeStampDisable(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath));
        PVRPL_FILEIN_DBGMSG_TRACE("FileIn PacketMode = 188\n");
    }
    else
    {
        PVRPL_FILEIN_DBGMSG_ERR("<ERR>\n");
        return E_PVRPL_FILEIN_FAIL;
    }

    PVRPL_FILEIN_DBGMSG_TRACE("sFileInInfo.u8FileInPktMode:%d tsFileInInfo.PKT_Mode:%d tsFileInInfo.Rate:%"DTC_MS_U32_u"\n",
                        sCurFileInInfo->u8FileInPktMode,tsFileInInfo.PKT_Mode,tsFileInInfo.Rate);

    if(MApi_DMX_Filein_Eng_Info(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath),&tsFileInInfo) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_FILEIN_DBGMSG_ERR("Filein Info fail\n");
        return E_PVRPL_FILEIN_FAIL;
    }

    return E_PVRPL_FILEIN_SUCESS;
}

//@NOTE This function is implemented to add/delete PID Filters
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_PIDFilterControl(FileInResource *sCurFileInInfo,MS_BOOL bAdd,MS_U8 *pu8FltID,MS_U16 u16PID,MS_U32 u32PVRPLFltType)
{
    if(bAdd == TRUE)
    {
        //@NOTE
        //Let AP user select filein-eng by path to play video(audio) or video3d(audio2)
        DMX_FILTER_TYPE stDmxFltSrc =   _PVRPL_FileIn_Mapping_DmxFltSrc(sCurFileInInfo->eFileInPath);
        DMX_FILTER_TYPE stDmxFltType =  _PVRPL_FileIn_Mapping_DmxFltDst(sCurFileInInfo->eFileInPath,_PVRPL_FileIn_Mapping_DmxFltType(u32PVRPLFltType));

        PVRPL_FILEIN_DBGMSG_TRACE("Path:%d Ori_Type:%"DTC_MS_U32_x" Src_TypeFlt:%"DTC_MS_U32_x" \n", sCurFileInInfo->eFileInPath,u32PVRPLFltType,(stDmxFltType|stDmxFltSrc));

        if(MApi_DMX_Open((DMX_FILTER_TYPE)(stDmxFltType|stDmxFltSrc),pu8FltID) == DMX_FILTER_STATUS_ERROR)
        {
            PVRPL_FILEIN_DBGMSG_ERR("Open PID FLT ERROR\n");
            return E_PVRPL_FILEIN_FAIL;
        }

        if(MApi_DMX_Pid((*pu8FltID),(&u16PID),TRUE) == DMX_FILTER_STATUS_ERROR)
        {
            PVRPL_FILEIN_DBGMSG_ERR("SET PID ERROR\n");
            return E_PVRPL_FILEIN_FAIL;
        }

        if(MApi_DMX_Start((*pu8FltID)) == DMX_FILTER_STATUS_ERROR)
        {
            PVRPL_FILEIN_DBGMSG_ERR("START PID Flt\n");
            return E_PVRPL_FILEIN_FAIL;
        }

        if((DMX_FILTER_TYPE)stDmxFltType == DMX_FILTER_TYPE_PCR)
        {
            MS_U32 u32EngId = E_PVRPL_PCR_ENG_INVALID;
            if(MApi_DMX_Pcr_Get_MapSTC((MS_U32)*pu8FltID, &u32EngId) == DMX_FILTER_STATUS_ERROR)
            {
                PVRPL_FILEIN_DBGMSG_ERR("PCR Get Map STC failed!\n");
                return E_PVRPL_FILEIN_FAIL;
            }
            sCurFileInInfo->u8STCEng = (MS_U8)u32EngId;
            PVRPL_FILEIN_DBGMSG_TRACE("STC eng: %d \n", sCurFileInInfo->u8STCEng);
        }

        PVRPL_FILEIN_DBGMSG_TRACE("Open FltType:0x%"DTC_MS_U32_x" FltID:0x%x PID:0x%x \n", u32PVRPLFltType,(*pu8FltID),u16PID);
    }
    else
    {
        if(MApi_DMX_Stop((*pu8FltID)) == DMX_FILTER_STATUS_ERROR)
        {
            PVRPL_FILEIN_DBGMSG_ERR("Stop PID FLT:0x%x ERROR\n", (*pu8FltID));
            return E_PVRPL_FILEIN_FAIL;
        }

        if(MApi_DMX_Close((*pu8FltID)) == DMX_FILTER_STATUS_ERROR)
        {
            PVRPL_FILEIN_DBGMSG_ERR("CLOSE PID FLT:0x%x ERROR\n", (*pu8FltID));
            return E_PVRPL_FILEIN_FAIL;
        }

        PVRPL_FILEIN_DBGMSG_TRACE("Close FltType:0x%"DTC_MS_U32_x" FltID:0x%x PID:0x%x \n", u32PVRPLFltType,(*pu8FltID),u16PID);
    }

    return E_PVRPL_FILEIN_SUCESS;
}

EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_SetPlaybackTime(FileInResource *sCurFileInInfo,MS_U32 u32PlaybakTime)
{
    if(MApi_DMX_Filein_Eng_SetPlaybackStamp(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath), u32PlaybakTime) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_FILEIN_DBGMSG_ERR("<ERR>\n");
        return E_PVRPL_FILEIN_FAIL;
    }

    PVRPL_FILEIN_DBGMSG_TRACE("u32PlaybakTime:%"DTC_MS_U32_x"\n", u32PlaybakTime);
    return E_PVRPL_FILEIN_SUCESS;
}

EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_EnableFileInPause(FileInResource *sCurFileInInfo,MS_BOOL bEnable)
{
    if(bEnable == TRUE)
    {
        if(MApi_DMX_Filein_Eng_Pause(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath)) == DMX_FILTER_STATUS_ERROR)
        {
            PVRPL_FILEIN_DBGMSG_ERR("<ERR>\n");
            return E_PVRPL_FILEIN_FAIL;
        }

        //@NOTE handle pause resume time
        if(MApi_DMX_Filein_Eng_GetPlaybackStamp(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath),&sCurFileInInfo->u32PausedTime) == DMX_FILTER_STATUS_ERROR)
        {
            PVRPL_FILEIN_DBGMSG_ERR("<ERR>\n");
            return E_PVRPL_FILEIN_FAIL;
        }
    }
    else
    {
        MApi_DMX_Filein_Eng_SetPlaybackStamp(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath),sCurFileInInfo->u32PausedTime);
        if(MApi_DMX_Filein_Eng_Resume(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath)) == DMX_FILTER_STATUS_ERROR)
        {
            PVRPL_FILEIN_DBGMSG_ERR("<ERR>\n");
            return E_PVRPL_FILEIN_FAIL;
        }
    }

    PVRPL_FILEIN_DBGMSG_TRACE("bEnable:%d u32PausedTime:%"DTC_MS_U32_u"\n", bEnable,sCurFileInInfo->u32PausedTime);
    return E_PVRPL_FILEIN_SUCESS;
}

EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_EnablePlaybackTime(FileInResource *sCurFileInInfo,MS_BOOL bEnable)
{
    if(MApi_DMX_Filein_Eng_PlaybackTimeStampEnable(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath)) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_FILEIN_DBGMSG_ERR("<ERR>\n");
        return E_PVRPL_FILEIN_FAIL;
    }

    PVRPL_FILEIN_DBGMSG_TRACE("bEnable:%d\n", bEnable);
    return E_PVRPL_FILEIN_SUCESS;
}

EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_EnableBypassFileInTime(FileInResource *sCurFileInInfo,MS_BOOL bBypass)
{
    MApi_DMX_Filein_Eng_BypassFileInTimeStamp(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath),bBypass);
    sCurFileInInfo->bByPassFileIn = bBypass;

    PVRPL_FILEIN_DBGMSG_TRACE("bBypass:%d\n", bBypass);
    return E_PVRPL_FILEIN_SUCESS;
}

EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_FeedFileInBlock(FileInResource *sCurFileInInfo,MS_U32 u32PhyAddr,MS_U32 u32Size)
{
    if(sCurFileInInfo->bQueFullTag == TRUE)
    {
        PVRPL_FILEIN_DBGMSG_ERR("FileInBlock Queue is Full\n");
        return E_PVRPL_FILEIN_FAIL;
    }
    else
    {
        // Feed a block at the position of u8QueRear
        PVRPL_FILEIN_DBGMSG_TRACE("[FileIn] (Rear:Addr)(%d:%"DTC_MS_U32_x")\n", sCurFileInInfo->u8QueRear, u32PhyAddr);
        sCurFileInInfo->psFileInQueue[sCurFileInInfo->u8QueRear].u32PhyAddr = u32PhyAddr;
        sCurFileInInfo->psFileInQueue[sCurFileInInfo->u8QueRear].u32Size = u32Size;

        sCurFileInInfo->u8QueRear = (sCurFileInInfo->u8QueRear+1) % sCurFileInInfo->u8MaxQueNum;

        // SW queue is FULL
        PVRPL_FILEIN_DBGMSG_TRACE("[FileIn]------- (Q:F) (%d:%d)",sCurFileInInfo->u8QueRear,sCurFileInInfo->u8QueFront);
        if(sCurFileInInfo->u8QueRear == sCurFileInInfo->u8QueFront)
        {
            PVRPL_FILEIN_DBGMSG_TRACE("[FileIn] -----FULL!!!\n");
            sCurFileInInfo->bQueFullTag = TRUE;
        }
        //debug
        //MS_U32 u32viraddr = MsOS_PA2KSEG1(u32PhyAddr);
        //printf("FEED ====> TimeStamp to Queue:0x%x 0x%x 0x%x 0x%x 0x%x\n",((MS_U8 *)u32viraddr)[0],((MS_U8 *)u32viraddr)[1],((MS_U8 *)u32viraddr)[2],((MS_U8 *)u32viraddr)[3],((MS_U8 *)u32viraddr)[4]);

        PVRPL_FILEIN_DBGMSG_TRACE("Feed =====> u8QueRear:%d u8QueFront:%d  u8QueRearu32PhyAddr:%"DTC_MS_U32_u" u32Size:%"DTC_MS_U32_u" bQueFullTag:%d\n", sCurFileInInfo->u8QueRear,sCurFileInInfo->u8QueFront,
           sCurFileInInfo->psFileInQueue[sCurFileInInfo->u8QueRear].u32PhyAddr,sCurFileInInfo->psFileInQueue[sCurFileInInfo->u8QueRear].u32Size,sCurFileInInfo->bQueFullTag);
    }

    return E_PVRPL_FILEIN_SUCESS;
}

EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_ConsumeFileInBlock(FileInResource *sCurFileInInfo)
{
    EN_PVRPL_FILEIN_STATUS eResult = _PVRPL_FileIn_MoveFileInBlockToFileInEng(sCurFileInInfo);
    return eResult;
}

EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_ChangeFileInMode(FileInResource *sCurFileInInfo,PVRPL_FileInMode eFileInMode)
{
    sCurFileInInfo->eQueThreshold = eFileInMode;
    PVRPL_FILEIN_DBGMSG_TRACE("Success\n");
    return E_PVRPL_FILEIN_SUCESS;
}

//@TODO Is it correct?
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_ResetFileInEng(FileInResource *sCurFileInInfo)
{
    if(MApi_DMX_Filein_Eng_CMDQ_Reset(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath)) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_FILEIN_DBGMSG_ERR("<ERR> reset file-in eng fail\n");
        return E_PVRPL_FILEIN_FAIL;
    }

    _FlushFileInQueue(sCurFileInInfo);
    PVRPL_FILEIN_DBGMSG_TRACE("Success\n");
    return E_PVRPL_FILEIN_SUCESS;
}

EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_CaliberatePlaybackTime(FileInResource *sCurFileInInfo,MS_BOOL bForward,MS_U32 u32Time)
{
    MS_U32 u32FileinTimeStamp, u32PlaybackStamp;

    if(MApi_DMX_Filein_Eng_GetFileInTimeStamp(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath),&u32FileinTimeStamp) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_FILEIN_DBGMSG_ERR("<ERR> Get file-in timestamp failed\n");
        return E_PVRPL_FILEIN_FAIL;
    }

    if(MApi_DMX_Filein_Eng_GetPlaybackStamp(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath),&u32PlaybackStamp) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_FILEIN_DBGMSG_ERR("<ERR> Get playback timestamp failed\n");
        return E_PVRPL_FILEIN_FAIL;
    }
    //forward
    if(bForward == TRUE)
    {
        if ((u32FileinTimeStamp > u32PlaybackStamp) && (u32FileinTimeStamp - u32PlaybackStamp > 0x20000))
        {
            PVRPL_FILEIN_DBGMSG_TRACE("u32FileinTimeStamp:%"DTC_MS_U32_x" u32PlaybackStamp:%"DTC_MS_U32_x"\n", u32FileinTimeStamp,u32PlaybackStamp);
            u32PlaybackStamp = u32FileinTimeStamp + 0x1000;
            MApi_DMX_Filein_Eng_SetPlaybackStamp(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath),u32PlaybackStamp);
            return E_PVRPL_FILEIN_SUCESS;
        }
        else
        {
            PVRPL_FILEIN_DBGMSG_ERR("<ERR>: Forward Timestamp is wrong\n");
            return E_PVRPL_FILEIN_FAIL;
        }
    }
    //backward
    else
    {
        if ((u32PlaybackStamp > u32FileinTimeStamp) && (u32PlaybackStamp - u32FileinTimeStamp > 0x20000))
        {
            PVRPL_FILEIN_DBGMSG_TRACE("u32FileinTimeStamp:%"DTC_MS_U32_x" u32PlaybackStamp:%"DTC_MS_U32_x"\n", u32FileinTimeStamp,u32PlaybackStamp);
            u32PlaybackStamp = u32FileinTimeStamp + 0x1000;
            MApi_DMX_Filein_Eng_SetPlaybackStamp(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath),u32PlaybackStamp);
            return E_PVRPL_FILEIN_SUCESS;
        }
        else
        {
            PVRPL_FILEIN_DBGMSG_ERR("<ERR>: Backward Timestamp is wrong\n");
            return E_PVRPL_FILEIN_FAIL;
        }
    }

    return E_PVRPL_FILEIN_SUCESS;
}

EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_GetPlaybackTime(FileInResource *sCurFileInInfo,MS_U32 *pu32PlaybackTime)
{
    if(MApi_DMX_Filein_Eng_GetPlaybackStamp(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath),pu32PlaybackTime) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_FILEIN_DBGMSG_ERR("<ERR>\n");
        return E_PVRPL_FILEIN_FAIL;
    }

    PVRPL_FILEIN_DBGMSG_TRACE("PlaybackTime:%"DTC_MS_U32_x"\n", *pu32PlaybackTime);
    return E_PVRPL_FILEIN_SUCESS;
}

EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_GetFileInTime(FileInResource *sCurFileInInfo,MS_U32 *pu32FileInTime)
{
    if(MApi_DMX_Filein_Eng_GetFileInTimeStamp(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath),pu32FileInTime) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_FILEIN_DBGMSG_ERR("<ERR>\n");
        return E_PVRPL_FILEIN_FAIL;
    }

    PVRPL_FILEIN_DBGMSG_TRACE("FileInTime:%"DTC_MS_U32_x"\n", *pu32FileInTime);
    return E_PVRPL_FILEIN_SUCESS;
}

MS_U8 PVRPL_FileIn_GetPCREng(FileInResource *sCurFileInInfo)
{
    return sCurFileInInfo->u8STCEng;
}

EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_ResetAVFIFO(FileInResource *sCurFileInInfo)
{
    if(MApi_DMX_AVFifo_Reset((DMX_FILTER_TYPE)(sCurFileInInfo->eAVFIFO.eAudioType), TRUE) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_FILEIN_DBGMSG_ERR("<ERR>: Audio\n");
        return E_PVRPL_FILEIN_FAIL;
    }

    if(MApi_DMX_AVFifo_Reset((DMX_FILTER_TYPE)(sCurFileInInfo->eAVFIFO.eVideoType), TRUE) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_FILEIN_DBGMSG_ERR("<ERR>: Video\n");
        return E_PVRPL_FILEIN_FAIL;
    }

    _FlushFileInQueueAndEng(sCurFileInInfo);

    if(MApi_DMX_AVFifo_Reset((DMX_FILTER_TYPE)(sCurFileInInfo->eAVFIFO.eAudioType), FALSE) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_FILEIN_DBGMSG_ERR("<ERR>: Audio\n");
        return E_PVRPL_FILEIN_FAIL;
    }

    if(MApi_DMX_AVFifo_Reset((DMX_FILTER_TYPE)(sCurFileInInfo->eAVFIFO.eVideoType), FALSE) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_FILEIN_DBGMSG_ERR("<ERR>: Video\n");
        return E_PVRPL_FILEIN_FAIL;
    }

    PVRPL_FILEIN_DBGMSG_TRACE("Success\n");
    return E_PVRPL_FILEIN_SUCESS;
}

//@NOTE the flush data need to enable bypass and disable pause file-in eng...
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_FlushData(FileInResource *sCurFileInInfo)
{
    MS_BOOL bLastPsueFileInState = MApi_DMX_Filein_Eng_IsPause(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath));

    PVRPL_FileIn_EnableFileInPause(sCurFileInInfo, FALSE);
    PVRPL_FileIn_ResetAVFIFO(sCurFileInInfo);
    PVRPL_FileIn_EnableFileInPause(sCurFileInInfo, bLastPsueFileInState);

    PVRPL_FILEIN_DBGMSG_TRACE("Success\n");
    return E_PVRPL_FILEIN_SUCESS;
}

EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_GetSTC(FileInResource *sCurFileInInfo,MS_U32 *pu32Stc32,MS_U32 *pu32Stc)
{
    if(MApi_DMX_Stc_Eng_Get(sCurFileInInfo->u8STCEng,pu32Stc32,pu32Stc) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_FILEIN_DBGMSG_ERR("<ERR>\n");
        return E_PVRPL_FILEIN_FAIL;
    }

    PVRPL_FILEIN_DBGMSG_TRACE("u32Stc32:%"DTC_MS_U32_x" u32Stc:%"DTC_MS_U32_x"\n", *pu32Stc32,*pu32Stc);
    return E_PVRPL_FILEIN_SUCESS;
}

EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_SetSTC(FileInResource *sCurFileInInfo,MS_U32 u32Stc32,MS_U32 u32Stc)
{
    if(MApi_DMX_Stc_Eng_Set(sCurFileInInfo->u8STCEng,u32Stc32,u32Stc) == DMX_FILTER_STATUS_ERROR)
    {
        PVRPL_FILEIN_DBGMSG_ERR("<ERR>\n");
        return E_PVRPL_FILEIN_FAIL;
    }

    PVRPL_FILEIN_DBGMSG_TRACE("u32Stc32:%"DTC_MS_U32_x" u32Stc:%"DTC_MS_U32_x"\n", u32Stc32,u32Stc);
    return E_PVRPL_FILEIN_SUCESS;
}

//@TODO need to fix
EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_IsFileInEnd(FileInResource *sCurFileInInfo)
{
    if((MApi_DMX_Filein_Eng_IsIdle(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath))== TRUE) && (PVRPL_FileIn_IsFileInBlockEmpty(sCurFileInInfo) == TRUE))
    {
        PVRPL_FILEIN_DBGMSG_TRACE("FileEnd\n");
        return E_PVRPL_FILEIN_SUCESS;
    }
    else
    {
        PVRPL_FILEIN_DBGMSG_TRACE("Not FileEnd\n");
        return E_PVRPL_FILEIN_FAIL;
    }
}

MS_BOOL PVRPL_FileIn_IsFileInBlockFull(FileInResource *sCurFileInInfo)
{
    if(sCurFileInInfo->bQueFullTag == TRUE)
    {
        PVRPL_FILEIN_DBGMSG_TRACE("Queue is Full\n");
        return TRUE;
    }
    else
    {
        PVRPL_FILEIN_DBGMSG_TRACE("Queue is not Full\n");
        return FALSE;
    }
}

MS_BOOL PVRPL_FileIn_IsFileInBlockEmpty(FileInResource *sCurFileInInfo)
{
    if((sCurFileInInfo->u8QueRear == sCurFileInInfo->u8Feeded2HW))
    {
        PVRPL_FILEIN_DBGMSG_TRACE("Queue is Empty\n");
        return TRUE;
    }
    else
    {
        PVRPL_FILEIN_DBGMSG_TRACE("Queue is not Empty\n");
        return FALSE;
    }
}

EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_Exit(FileInResource *sCurFileInInfo)
{
    MApi_DMX_AVFifo_Reset((DMX_FILTER_TYPE)(sCurFileInInfo->eAVFIFO.eAudioType), TRUE);
    MApi_DMX_AVFifo_Reset((DMX_FILTER_TYPE)(sCurFileInInfo->eAVFIFO.eVideoType), TRUE);
    MsOS_FreeMemory(sCurFileInInfo->psFileInQueue,gs32NonCachedPoolID);
    sCurFileInInfo->bInit= FALSE;
    MApi_DMX_Filein_Eng_Stop(_PVRPL_FileIn_Mapping_DmxFileInPath(sCurFileInInfo->eFileInPath));
    PVRPL_FILEIN_DBGMSG_TRACE("PVRPL_FileIn_Exit Success\n");
    return E_PVRPL_FILEIN_SUCESS;
}

void PVRPL_FileIn_SetDBGLevel(FileInResource *sCurFileInInfo,PVRPL_DBGMSG_LEVEL eDBGLevel)
{
    _ePVRPL_FileInDbgLevel = eDBGLevel;
}

MS_U32 PVRPL_FileIn_Mapping_DmxFltType_To_PVRPLFltType(MS_U32 u32DmxFltType)
{
    switch((DMX_FILTER_TYPE)u32DmxFltType)
    {
        case DMX_FILTER_TYPE_VIDEO:
            return PVRPL_FILEIN_FILTER_TYPE_VIDEO;
        case DMX_FILTER_TYPE_AUDIO:
            return  PVRPL_FILEIN_FILTER_TYPE_AUDIO;
        case DMX_FILTER_TYPE_SECTION:
            return  PVRPL_FILEIN_FILTER_TYPE_SECTION;
        case DMX_FILTER_TYPE_PCR:
            return  PVRPL_FILEIN_FILTER_TYPE_PCR;
        case DMX_FILTER_TYPE_PES:
            return  PVRPL_FILEIN_FILTER_TYPE_PES;
        default:
            PVRPL_FILEIN_DBGMSG_ERR("Wrong DMX Flt Type:%"DTC_MS_U32_x"\n",u32DmxFltType);
            return PVRPL_FILEIN_FILTER_TYPE_VIDEO;
    }
}

EN_PVRPL_FILEIN_STATUS PVRPL_FileIn_GetInfo(FileInResource *sCurFileInInfo, EN_FILEIN_INFO FileInInfo, MS_U32 *u32pmtr, void* structure)
{
    MS_U32 u32value = 0;

    switch(FileInInfo)
    {
        case EN_FILEIN_INFO_PCR:
            MApi_DMX_Pcr_Eng_Get(sCurFileInInfo->u8STCEng,&u32value,u32pmtr);
            break;
        case EN_FILEIN_INFO_PCR_HIGH_BIT:
            MApi_DMX_Pcr_Eng_Get(sCurFileInInfo->u8STCEng,u32pmtr,&u32value);
            break;
        default:
            return E_PVRPL_FILEIN_FAIL;
    }
    return E_PVRPL_FILEIN_SUCESS;
}

