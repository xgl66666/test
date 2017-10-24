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

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "MsCommon.h"
#include "MsFS.h"
#include "MsMemory.h"
#include "apiDMX.h"

//debug macro
#define _DMX_DBG 1
#if _DMX_DBG
#define APP_DMX_DBG(_fmt,_args...)             {printf("[%s][%d]",__FUNCTION__,__LINE__); printf(_fmt,##_args);} 
#else
#define APP_DMX_DBG(_fmt,_args...)             { }
#endif

#define _TSP_CACHE_SEL gs32CachedPoolID 
#define IR_TASK_STACK_SIZE      3072
#define IR_TASK_MONITOR_DELAY   30 

//We choose FILEIN_UNIT_SIZE 0x34E00 == 216576 == 188*192*6 since 
//1) the file-in size must be align to pktSize that is 188 or 192
//2) the read size from USB HD is within 128KBytes to 256 KBytes
#define FILEIN_UNIT_SIZE    (0x34E00)
#define FILEIN_BUFS_NUM     (6)

//CMDQ_SLOT_NUM and CMDQ_FILEIN_UNI are TSP HW configurations
#define CMDQ_SLOT_NUM       (16)
#define CMDQ_FILEIN_UNI     (5)

typedef struct tsp_bufs_ring
{
    MS_U32* pBuf;
    MS_U32* pSize;
    MS_U32 num;
    MS_U32 uni;
    MS_U32 rd;
    MS_U32 wrt;
    MS_U32 init;
} TSP_BUFS_RING;

typedef struct tsp_flowset_status
{
    DMX_FLOW        DmxFlow;
    DMX_FLOW_INPUT  DmxFlowInput;
    MS_BOOL         bClkInv; 
    MS_BOOL         bExtSync; 
    MS_BOOL         bParallel;
    MS_BOOL         bEnable;
} TSP_FLOWSET_STATUS;

extern MS_S32 gs32CachedPoolID ;
extern MS_S32 gs32NonCachedPoolID;

static TSP_BUFS_RING _tspBufsRing = {.init = FALSE, .num = 0, .uni = 0, .rd = 0, .wrt = 0};
static TSP_FLOWSET_STATUS _orgFlowset = {.bEnable = FALSE}; 

static MS_U8 _fileIn_StackBuffer[IR_TASK_STACK_SIZE];
static MS_U8 _rdFile_StackBuffer[IR_TASK_STACK_SIZE];

static MS_S32 _s32_fileIn_taskId = -1;
static MS_S32 _s32_rdFile_taskId = -1;

static FILE* srcFilePtr = NULL;
static MS_U32 pktSize = 188;

static MS_U8 fileIn_en   = FALSE;
static MS_U8 fileIn_cont = FALSE;

static MS_U8 rdFile_en   = FALSE;
static MS_U8 rdFile_cont = FALSE;

MS_U32 _bufs_ring_init(TSP_BUFS_RING* pBufsRing, MS_U32 bufsNum, MS_U32 uniSize)
{
    MS_U32 i = 0;

    if(pBufsRing->init == TRUE)
    {
        APP_DMX_DBG("The bufs-ring is already initialized\n");
        return FALSE;
    }

    pBufsRing->pBuf  = MsOS_AllocateMemory(bufsNum * sizeof(MS_U32) , _TSP_CACHE_SEL);
    if(pBufsRing->pBuf == NULL)
    {
        APP_DMX_DBG("Memory allocation failed\n");
    }

    pBufsRing->pSize = MsOS_AllocateMemory(bufsNum * sizeof(MS_U32) , _TSP_CACHE_SEL);
    if(pBufsRing->pSize == NULL)
    {
        APP_DMX_DBG("Memory allocation failed\n");
    }

    for(i = 0; i < bufsNum; i++)
    {
        *(pBufsRing->pBuf + i)  = (MS_U32)MsOS_AllocateMemory(uniSize, _TSP_CACHE_SEL);
        *(pBufsRing->pSize + i) = 0; 
    }

    pBufsRing->num  = bufsNum;
    pBufsRing->uni  = uniSize;
    pBufsRing->rd   = 0;
    pBufsRing->wrt  = 0;
    pBufsRing->init = TRUE;
    return TRUE;
}

MS_U32 _bufs_ring_exit(TSP_BUFS_RING* pBufsRing)
{
    MS_U32 i = 0;

    if(pBufsRing->init == FALSE)
    {
        APP_DMX_DBG("The bufs-ring has not been initialized yet.\n");
        return FALSE;
    }

    pBufsRing->init = FALSE;

    for(i = 0; i < pBufsRing->num; i++)
    {
        MsOS_FreeMemory((void*)(*(pBufsRing->pBuf + i)), _TSP_CACHE_SEL);
    }

    MsOS_FreeMemory(pBufsRing->pBuf, _TSP_CACHE_SEL);
    MsOS_FreeMemory(pBufsRing->pSize, _TSP_CACHE_SEL);

    pBufsRing->num  = 0;
    pBufsRing->uni  = 0;
    pBufsRing->rd   = 0;
    pBufsRing->wrt  = 0;

    return TRUE;
}

MS_U32 _bufs_ring_empty(TSP_BUFS_RING* pBufsRing)
{
    if(pBufsRing->init == FALSE)
    {
        APP_DMX_DBG("The bufs-ring has not been initialized yet.\n");
        return FALSE;
    }

    MS_U32 rd  = pBufsRing->rd;
    MS_U32 wrt = pBufsRing->wrt;
    MS_U32 num = pBufsRing->num;

    if(wrt < rd)
    {
        return rd - wrt - 1;
    }
    else
    {
        return num - wrt + rd - 0 - 1; 
    }
}

MS_U32 _bufs_ring_data(TSP_BUFS_RING* pBufsRing)
{
    if(pBufsRing->init == FALSE)
    {
        APP_DMX_DBG("The bufs-ring has not been initialized yet.\n");
        return FALSE;
    }

    MS_U32 rd  = pBufsRing->rd;
    MS_U32 wrt = pBufsRing->wrt;
    MS_U32 num = pBufsRing->num;

    if(rd <= wrt)
    {
        return wrt - rd;
    }
    else
    {
        return num - rd + wrt - 0; 
    }
}


MS_U32 _bufs_ring_wrtFromFile(TSP_BUFS_RING* pBufsRing, FILE* filePtr)
{
    if(pBufsRing->init == FALSE)
    {
        APP_DMX_DBG("The bufs-ring has not been initialized yet.\n");
        return FALSE;
    }

    MS_U32 amount = 0;

    if(_bufs_ring_empty(pBufsRing) > 0)
    {
        amount = MsFS_Fread((void*)(*(pBufsRing->pBuf + pBufsRing->wrt)), 1, pBufsRing->uni, filePtr);
        if(amount > 0)
        {
            *(pBufsRing->pSize + pBufsRing->wrt) = amount;
            pBufsRing->wrt = (pBufsRing->wrt + 1) % pBufsRing->num;
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

MS_U32 _bufs_ring_rd(TSP_BUFS_RING* pBufsRing, MS_U32* pBuf, MS_U32* pSize)
{
    if(pBufsRing->init == FALSE)
    {
        APP_DMX_DBG("The bufs-ring has not been initialized yet.\n");
        return FALSE;
    }

    if(_bufs_ring_data(pBufsRing) > 0)
    {
        *pBuf  = *(pBufsRing->pBuf + pBufsRing->rd);
        *pSize = *(pBufsRing->pSize + pBufsRing->rd);
        pBufsRing->rd = (pBufsRing->rd + 1) % pBufsRing->num;
        return TRUE;
    }
    else
    {
        *pBuf  = (MS_U32)NULL;
        *pSize = (MS_U32)NULL;
        return FALSE;
    }
}

void _appDemo_rdFile_task(void)
{
    MS_BOOL bRet = FALSE;
    MS_U32  fileInNum = 0;  
    MS_U32 tsEmptyCmdQSlot = 0;

    while(rdFile_en)
    {
        if(MApi_DMX_Filein_IsBusy())
        {
            MApi_DMX_Filein_CMDQ_GetEmptyNum(&tsEmptyCmdQSlot);
            fileInNum = (CMDQ_SLOT_NUM - tsEmptyCmdQSlot)/CMDQ_FILEIN_UNI + 1;
        }
        else
        {
            fileInNum = 0;
        }

        if(_bufs_ring_empty(&_tspBufsRing) > fileInNum)
        {
            bRet = _bufs_ring_wrtFromFile(&_tspBufsRing, srcFilePtr);

            if(bRet == FALSE)
            {
                APP_DMX_DBG("file end\n");
                break;
            }
        }
        else
        {
            MsOS_DelayTaskUs(50);
        }
    }

    rdFile_cont = FALSE;
}

void _appDemo_fileIn_task(void)
{
    MS_BOOL bRet = FALSE;
    MS_U32  buf = (MS_U32)NULL;
    MS_U32  size = 0;
    MS_U32  tsEmptyCmdQSlot;
    MS_U32  residual = 0;

    if(pktSize == 188)
    {
        MApi_DMX_Pvr_TimeStampDisable();
    }
    else
    {
        MApi_DMX_Pvr_TimeStampEnable();
        MApi_DMX_Pvr_SetPlaybackStamp(0);
    }

    while(fileIn_en)
    {
        MApi_DMX_Filein_CMDQ_GetEmptyNum(&tsEmptyCmdQSlot);
        if((_bufs_ring_data(&_tspBufsRing) > 0) && (tsEmptyCmdQSlot > CMDQ_FILEIN_UNI))
        {
            bRet = _bufs_ring_rd(&_tspBufsRing, &buf, &size);

            if(bRet == TRUE)
            {
                residual = size % pktSize;
                if(residual != 0)
                {
                    APP_DMX_DBG("pktSize: %lu residual: %lu\n", pktSize, residual);
                    size -= residual;
                }

                MsOS_FlushMemory();
                MApi_DMX_Filein_Start(DMX_PES_NO_BYPASS_FIFO, (MS_PHYADDR)MS_VA2PA(buf), size);
            }
            else
            {
                APP_DMX_DBG("_bufs_ring_rd() failed\n")
            }
        }
        else
        {
            MsOS_DelayTaskUs(50);
        }
    }

    fileIn_cont = FALSE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to demo playback
/// @param[in] srcFileName 
/// @param[in] pPktSize 
/// @return TRUE  : process success.
/// @return FALSE : process fail.
/// @sa
/// @note
/// Command:	\b DMX_FileIn_Start [srcfilename] [pktsize] \n
///				\b DMX_FileIn_Start a.ts 192 \n
//------------------------------------------------------------------------------

MS_BOOL appDemo_DmxFileIn_Start(char* srcFileName, MS_U32* pPktSize)
{
    //open file
    srcFilePtr = MsFS_Fopen(srcFileName, "rb");
    DMX_Filein_info tsFileInInfo; 

    MsFS_Fseek(srcFilePtr, 0, SEEK_SET);

    if(srcFilePtr != NULL)
    {
        APP_DMX_DBG("Open file ok, srcFileName: %s srcFilePtr: %lX\n", srcFileName, (MS_U32)srcFilePtr);
    }
    else
    {
        APP_DMX_DBG("Open file %s failed\n", srcFileName);
        return FALSE;
    }

    //Record the original flowset status
    MApi_DMX_Get_FlowInput_Status(_orgFlowset.DmxFlow, &(_orgFlowset.DmxFlowInput), &(_orgFlowset.bClkInv), &(_orgFlowset.bExtSync), &(_orgFlowset.bParallel));
    _orgFlowset.bEnable = TRUE;

    //flowSet of file-in mode
    MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, DMX_FLOW_INPUT_MEM, FALSE, FALSE, TRUE); 

    if(*pPktSize == 188)
    {
        tsFileInInfo.PKT_Mode = DMX_PKTMODE_188; 
        tsFileInInfo.Rate = 2;
        MApi_DMX_Filein_Info(&tsFileInInfo);
        MApi_DMX_BypassFileInTimeStamp(TRUE); 
    }
    else if(*pPktSize == 192)
    {
        tsFileInInfo.PKT_Mode = DMX_PKTMODE_192; 
        tsFileInInfo.Rate = 2;
        MApi_DMX_Filein_Info(&tsFileInInfo);
        MApi_DMX_BypassFileInTimeStamp(FALSE); 
    }
    else
    {
        APP_DMX_DBG("Illegal pktSize: %lu\n", *pPktSize);
        return FALSE;
    }

    pktSize = *pPktSize;

    //disable AV FIFO reset
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, FALSE);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, FALSE);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO2, TRUE);

    //bufs-ring init
    if(!_bufs_ring_init(&_tspBufsRing, FILEIN_BUFS_NUM, FILEIN_UNIT_SIZE))
    {
        APP_DMX_DBG("Allocate _tspBufsRing failed\n");
        return FALSE;
    }

    rdFile_en   = TRUE;
    rdFile_cont = TRUE;
    _s32_rdFile_taskId = MsOS_CreateTask((TaskEntry)_appDemo_rdFile_task,
                                           (MS_U32)NULL,
                                           E_TASK_PRI_HIGH,
                                           TRUE,
                                           (void*)_rdFile_StackBuffer,
                                           IR_TASK_STACK_SIZE,
                                           "_RDFILE_TASK");
    if(_s32_rdFile_taskId == -1)
    {
        rdFile_en   = FALSE;
        rdFile_cont = FALSE;
        APP_DMX_DBG("Create _appDemo_rdFile_task() failed\n");
        return FALSE;
    }

    MsOS_DelayTask(5);

    fileIn_en   = TRUE;
    fileIn_cont = TRUE;
    _s32_fileIn_taskId = MsOS_CreateTask((TaskEntry)_appDemo_fileIn_task,
                                           (MS_U32)NULL,
                                           E_TASK_PRI_HIGH,
                                           TRUE,
                                           (void*)_fileIn_StackBuffer,
                                           IR_TASK_STACK_SIZE,
                                           "_FILEIN_TASK");

    if(_s32_fileIn_taskId == -1)
    {
        fileIn_en   = FALSE;
        fileIn_cont = FALSE;
        APP_DMX_DBG("Create _appDemo_fileIn_task() failed\n");
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief The sample code to demo playback stop
/// @return TRUE  : process success.
/// @return FALSE : process fail.
/// @sa
/// @note
/// Command:	\b DMX_FileIn_Stop \n
//------------------------------------------------------------------------------

MS_BOOL appDemo_DmxFileIn_Stop(void)
{
    DMX_Filein_info tsFileInInfo; 


    //delete task
    if(_s32_fileIn_taskId != -1)
    {
        fileIn_en = FALSE;
        while(fileIn_cont)
        {
            MsOS_DelayTask(1);
        }
        
        MsOS_DeleteTask(_s32_fileIn_taskId);
        _s32_fileIn_taskId = -1;
    }

    if(_s32_rdFile_taskId != -1)
    {
        rdFile_en = FALSE;
        while(rdFile_cont)
        {
            MsOS_DelayTask(1);
        }

        MsOS_DeleteTask(_s32_rdFile_taskId);
        _s32_fileIn_taskId = -1;
    }

    //reset CMDQ
    MApi_DMX_Filein_CMDQ_Reset();

    //-------- file-in abort start --------
    if(pktSize == 192)
    {
        MApi_DMX_BypassFileInTimeStamp(TRUE);
    }

    MsOS_DelayTask(1);

    //AV FIFO reset
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_VIDEO, TRUE);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO, TRUE);
    MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE_AUDIO2, TRUE);

    MsOS_DelayTask(5); //Delay time depends on FILEIN_UNIT_SIZE

    if(pktSize == 192)
    {
        MApi_DMX_BypassFileInTimeStamp(FALSE);
    }
    //-------- file-in abort end --------
    
    //-------- revert to default setting start --------
    MApi_DMX_Pvr_TimeStampDisable();

    tsFileInInfo.PKT_Mode = DMX_PKTMODE_188; 
    tsFileInInfo.Rate = 2;
    MApi_DMX_Filein_Info(&tsFileInInfo);

    if(_orgFlowset.bEnable == TRUE)
    {
        MApi_DMX_FlowSet(_orgFlowset.DmxFlow, _orgFlowset.DmxFlowInput, _orgFlowset.bClkInv, _orgFlowset.bExtSync, _orgFlowset.bParallel);
    }
    //-------- revert to default setting end --------

    _bufs_ring_exit(&_tspBufsRing);

    if(srcFilePtr != NULL)
    {
        MsFS_Fclose(srcFilePtr);
        srcFilePtr = NULL;
    }
    return TRUE;
}


