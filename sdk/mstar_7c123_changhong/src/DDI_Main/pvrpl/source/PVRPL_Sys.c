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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   PVRPL_Sys.h
/// @brief  CPL API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>

#include "PVRPL_Sys.h"
#include "PVRPL_ResourcePool.h"
#include "apiPVR.h"

#include "drvAESDMA.h"
#include "drvDSCMB.h"
#include "MsOS.h"
#include "drvBDMA.h"
#include "drvDTC.h"

extern MS_S32 gs32CachedPoolID ;
extern MS_S32 gs32NonCachedPoolID;

#define PVR_AESDMA_SPEED_TEST   0
#define ENABLE_2ND_ENCRYPTION   1
#define AESDMA_TIMEOUT          500  //ms
#define PVR_PA2VA_CACHED        0
#define INVALID_MUTEX_ID        -1
#define INVALID_ADDR            0xFFFFFFFF

#define AESDMA_BYTES_PER_MS     51200 // 50 * 1024 bytes
#define PVR_AES_ALIGNMENT_LEN   (16)
//Event Global const
#define PVRPL_EVENT_WAIT_FOREVER 0xffffffff

static MS_U32  _u32PVRPLSYSDbgLevel = PVRPLSYS_DBG_ERR;

#define PVRPL_SYS_DBGMSG(_level, msg, args...)      {if(_u32PVRPLSYSDbgLevel >= (_level)) printf("[%s][%d] " msg, __FUNCTION__, __LINE__, ## args);}
#define MOD_NAME                                    PVR

#if defined(HB_ERR)
    #define PVRPL_SYS_DBGMSG_ERR(msg, args...)      HB_ERR(msg, ##args)
#else
    #define PVRPL_SYS_DBGMSG_ERR(msg, args...)      PVRPL_SYS_DBGMSG(PVRPLSYS_DBG_ERR, msg, ##args)
#endif

#if defined(HB_INFO)
    #define PVRPL_SYS_DBGMSG_INFO(msg, args...)     HB_INFO(msg, ##args)
#else
    #define PVRPL_SYS_DBGMSG_INFO(msg, args...)     PVRPL_SYS_DBGMSG(PVRPLSYS_DBG_INFO, msg, ##args)
#endif

#if defined(HB_TRACE)
    #define PVRPL_SYS_DBGMSG_TRACE(msg, args...)    HB_TRACE(msg, ##args)
#else
    #define PVRPL_SYS_DBGMSG_TRACE(msg, args...)    PVRPL_SYS_DBGMSG(PVRPLSYS_DBG_TRACE, msg, ##args)
#endif

#if defined(HB_DBG)
    #define PVRPL_SYS_DBGMSG_DEBUG(msg, args...)    HB_DBG(msg, ##args)
#else
    #define PVRPL_SYS_DBGMSG_DEBUG(msg, args...)    PVRPL_SYS_DBGMSG(PVRPLSYS_DBG_FUNC, msg, ##args)
#endif

#if ENABLE_2ND_ENCRYPTION
    #define AESDAM_BUF_LENGTH   16
    #define AESDMA_KEY_LENGTH   4
#endif


#define ASSERT(_x_)                                     \
    do  {                                               \
        if ( ! ( _x_ ) )                                \
        {                                               \
            PVRPL_SYS_DBGMSG_DEBUG("ASSERT FAIL\n");    \
            abort();                                    \
        }                                               \
    } while (0)

#define PVRPL_64BITADDR_DETECT(Addr) {if(Addr > 0xFFFFFFFFUL){PVRPL_SYS_DBGMSG_ERR("Addr(%"DTC_MS_PHY_x") > 32bits!\n", Addr);}}
#define PVRPL_IsValidAddr(Addr, ValidAddr, Size) (Addr != INVALID_ADDR && Addr >= ValidAddr && Addr < ValidAddr + Size)

/****** Local variable ******/
//mutex for PVR_SYS_FUNC
static MS_S32 m_SysFuncMutex        = INVALID_MUTEX_ID;
static MS_S32 m_BDMAMutex           = INVALID_MUTEX_ID;
static MS_BOOL b_SysInit            = FALSE;


static MS_BOOL _gbIsBDMAInit = FALSE;
/***************************/

static MS_U32 _PA2VA(MS_U32 u32PhyAdr);
static void _MemoryFlush(MS_U32 u32VirSrcAdr,MS_U32 u32DataSize);


//event related setup functions
static MS_BOOL _PVRPL_CreateEventGroup(MS_S32 *s32EventId,MS_U8 *pName);
static MS_BOOL _PVRPL_DeleteEventGroup(MS_S32 *s32EventId);
static MS_BOOL _PVRPL_SetEvent(MS_S32 *s32EventId,MS_U32 u32EventFlag);
static MS_BOOL _PVRPL_ClearEvent(MS_S32 *s32EventId,MS_U32 u32EventFlag);
static MS_BOOL _PVRPL_WaitEvent(MS_S32 *s32EventId,MS_U32 u32WaitEventFlag);

PVRPL_SYS_STATUS PVRPL_SysInit()
{
    if(b_SysInit == TRUE)
    {
        return PVRPL_SYS_STATUS_OK;
    }

    m_SysFuncMutex = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"PVRSysFuncMutex", MSOS_PROCESS_SHARED);
    MS_ASSERT(-1!=m_SysFuncMutex);

    _gbIsBDMAInit = FALSE;
    b_SysInit = TRUE;
    return PVRPL_SYS_STATUS_OK;
}

MS_U32 PVRPL_GetSysTime()
{
    return MsOS_GetSystemTime();
}

PVRPL_SYS_STATUS PVRPL_PA2VA(MS_U32 u32PhyAdr, MS_U32* u32VirAdr)
{
    MsOS_ObtainMutex(m_SysFuncMutex,MSOS_WAIT_FOREVER);
    *u32VirAdr = _PA2VA( u32PhyAdr);
    MsOS_ReleaseMutex(m_SysFuncMutex);
    return PVRPL_SYS_STATUS_OK;
}

PVRPL_SYS_STATUS PVRPL_VA2PA(MS_U32 u32VirAdr, MS_U32* u32PhyAdr)
{
    MS_PHY PHYAdr = 0;

    MsOS_ObtainMutex(m_SysFuncMutex,MSOS_WAIT_FOREVER);
    PHYAdr = MsOS_VA2PA(u32VirAdr);
    PVRPL_64BITADDR_DETECT(PHYAdr);
    *u32PhyAdr = PHYAdr;
    ASSERT(u32PhyAdr);
    MsOS_ReleaseMutex(m_SysFuncMutex);
    return PVRPL_SYS_STATUS_OK;
}

PVRPL_SYS_STATUS PVRPL_SleepNS(MS_U32 u32NanoSec)
{
    ASSERT(0);
    return PVRPL_SYS_STATUS_OK;
}


PVRPL_SYS_STATUS PVRPL_SleepMS(MS_U32 u32MSec)
{
    MsOS_DelayTask(u32MSec);
    return PVRPL_SYS_STATUS_OK;
}

/*
PVR_MMAPInfo_t* PVRPL_GetMmap(PVRPL_MMAP_ID u32PvrId)
{

    PVR_MMAP_ID id = EN_PVR_MMAP_ID_DOWNLOAD;
    switch(u32PvrId)
    {
        case PVR_ID_DOWNLOAD:
            id=EN_PVR_MMAP_ID_DOWNLOAD;
            break;
        case PVR_ID_UPLOAD:
            id=EN_PVR_MMAP_ID_UPLOAD;
            break;
        case PVR_ID_BUFFER:
            id=EN_PVR_MMAP_ID_BUFFER;
            break;
        case PVR_ID_THUMBNAIL:
            id=EN_PVR_MMAP_ID_THUMBNAIL;
            break;
        case PVR_ID_BROWSER_BUFFER:
            id=EN_PVR_MMAP_ID_BROWSER_BUFFER;
            break;
        case PVR_ID_SWBUFFER:
            id=EN_PVR_MMAP_ID_SWBUFFER;
            break;
        case PVR_ID_ES_VIDEO_BUFFER:
            id=EN_PVR_MMAP_ID_ES_VIDEO;
            break;
        case PVR_ID_ES_AUDIO_BUFFER:
            id=EN_PVR_MMAP_ID_ES_AUDIO;
            break;
        default:
            PVRPL_SYS_DBGMSG_ERR("[MWPVR]  Invalid id 0x%"DTC_MS_U32_x"\n",u32PvrId);
            return FALSE;
    }

}

void* PVRPL_SetMmap(PVRPL_MMAP_ID u32PvrId,MS_U32 u32Addr,MS_U32 u32Size)
{

    PVR_MMAP_ID id = EN_PVR_MMAP_ID_DOWNLOAD;
    switch(u32PvrId)
    {
        case PVR_ID_DOWNLOAD:
            id=EN_PVR_MMAP_ID_DOWNLOAD;
            break;
        case PVR_ID_UPLOAD:
            id=EN_PVR_MMAP_ID_UPLOAD;
            break;
        case PVR_ID_BUFFER:
            id=EN_PVR_MMAP_ID_BUFFER;
            break;
        case PVR_ID_THUMBNAIL:
            id=EN_PVR_MMAP_ID_THUMBNAIL;
            break;
        case PVR_ID_BROWSER_BUFFER:
            id=EN_PVR_MMAP_ID_BROWSER_BUFFER;
            break;
        case PVR_ID_SWBUFFER:
            id=EN_PVR_MMAP_ID_SWBUFFER;
            break;
        case PVR_ID_ES_VIDEO_BUFFER:
            id=EN_PVR_MMAP_ID_ES_VIDEO;
            break;
        case PVR_ID_ES_AUDIO_BUFFER:
            id=EN_PVR_MMAP_ID_ES_AUDIO;
            break;
        default:
            PVRPL_SYS_DBGMSG_ERR("[MWPVR]  Invalid id 0x%"DTC_MS_U32_x"\n",u32PvrId);
            return FALSE;
    }
}
*/

#if (ENABLE_2ND_ENCRYPTION)
PVRPL_SYS_STATUS PVRPL_SetAESDMAEnabled(MS_BOOL bAESDMAEnabled)
{

    return PVRPL_SYS_STATUS_OK;
}

MS_U32 PVRPL_GetAESDMABufLength(void)
{
    return (MS_U32)AESDAM_BUF_LENGTH;
}

PVRPL_SYS_STATUS PVRPL_AESDMAEncryptDecrypt(MS_U32 u32Address, MS_U32 u32Length,
            enPLAESDMAMode eMode, PVRPL_ENCRYPT_TYPE enEncryptType)
{



    return PVRPL_SYS_STATUS_OK;
}

PVRPL_SYS_STATUS PVRPL_AESDMASetKey(PVRPL_ENCRYPT_TYPE enEncryptType,
            MS_U32 u32Key1, MS_U32 u32Key2, MS_U32 u32Key3, MS_U32 u32Key4)
{

    return PVRPL_SYS_STATUS_OK;
}

PVRPL_SYS_STATUS PVRPL_AESDMAGetKey(PVRPL_ENCRYPT_TYPE enEncryptType,
            MS_U32 *u32Key1, MS_U32 *u32Key2, MS_U32 *u32Key3, MS_U32 *u32Key4)
{

    return PVRPL_SYS_STATUS_OK;
}

void _AESDMASetKey(MS_U32 u32KeyArray[AESDMA_KEY_LENGTH],
    const MS_U32 key1, const MS_U32 key2, const MS_U32 key3, const MS_U32 key4)
{

}
void _AESDMAGetKey(const MS_U32 u32KeyArray[AESDMA_KEY_LENGTH],
    MS_U32 *key1, MS_U32 *key2, MS_U32 *key3, MS_U32 * key4)
{
}
#endif

PVRPL_SYS_STATUS PVRPL_MemoryCopy(MS_U32 u32PhySrcAdr,MS_U32 u32PhyDstAdr,MS_U32 u32Len)
{
    MsOS_ObtainMutex(m_SysFuncMutex,MSOS_WAIT_FOREVER);
    // Current memory size is less than 256 MB
    if (u32Len> 0x10000000)
    {
        PVRPL_SYS_DBGMSG_ERR("Warning!!! DMA size: %x\n", (unsigned int)u32Len);
        MsOS_ReleaseMutex(m_SysFuncMutex);
        return PVRPL_SYS_STATUS_OK;
    }
    MS_U32 u32DstAdr = _PA2VA(u32PhyDstAdr);
    _MemoryFlush(u32DstAdr,u32Len);

    MS_U32 u32SrcAdr=_PA2VA(u32PhySrcAdr);
    memcpy((void*)u32DstAdr,(void*)u32SrcAdr,u32Len);
    MsOS_ReleaseMutex(m_SysFuncMutex);
    return PVRPL_SYS_STATUS_OK;
}

//event Setting
PVRPL_SYS_STATUS PVRPL_EventCMD(PVRPL_EVENT_CMD EventCMD,MS_U8 *pName,MS_S32 *s32EventId,MS_U32 u32EventFlag)
{
    MS_BOOL bret = FALSE;
    switch(EventCMD)
    {
        case EN_PVRPL_CREATE_EVENTGROUP:
            bret = _PVRPL_CreateEventGroup(s32EventId,pName);
            break;
        case EN_PVRPL_DELELTE_EVENTGROUP:
            bret = _PVRPL_DeleteEventGroup(s32EventId);
            break;
        case EN_PVRPL_SET_EVENT:
            bret = _PVRPL_SetEvent(s32EventId,u32EventFlag);
            break;
        case EN_PVRPL_CLEAR_EVENT:
            bret = _PVRPL_ClearEvent(s32EventId,u32EventFlag);
            break;
        case EN_PVRPL_WAIT_EVENT:
            bret = _PVRPL_WaitEvent(s32EventId,u32EventFlag);
            break;
        default:
            PVRPL_SYS_DBGMSG_ERR("[PVRPL]  No such event cmd %d\n",EventCMD);
            bret = FALSE;
            break;
    }

    if(bret == FALSE)
    {
        return PVRPL_SYS_STATUS_ERROR;
    }
    return PVRPL_SYS_STATUS_OK;

}

PVRPL_SYS_STATUS PVRPL_CreateMutex(MS_S32 *ps32MutexID, MS_U8 *pu8MutexName)
{
    (*ps32MutexID) = INVALID_MUTEX_ID;
    (*ps32MutexID) = MsOS_CreateMutex(E_MSOS_FIFO, (char*)pu8MutexName, MSOS_PROCESS_SHARED);

    if((*ps32MutexID) == INVALID_MUTEX_ID)
    {
        return PVRPL_SYS_STATUS_ERROR;
    }

    return PVRPL_SYS_STATUS_OK;
}

PVRPL_SYS_STATUS PVRPL_ObtainMutex(MS_S32 s32MutexId)
{
    if(MsOS_ObtainMutex(s32MutexId,MSOS_WAIT_FOREVER) == TRUE)
    {
        return PVRPL_SYS_STATUS_OK;
    }
    else
    {
        return PVRPL_SYS_STATUS_ERROR;
    }
}

PVRPL_SYS_STATUS PVRPL_ReleaseMutex(MS_S32 s32MutexId)
{
    if(MsOS_ReleaseMutex(s32MutexId) == TRUE)
    {
        return PVRPL_SYS_STATUS_OK;
    }
    else
    {
        return PVRPL_SYS_STATUS_ERROR;
    }
}

PVRPL_SYS_STATUS PVRPL_DeleteMutex(MS_S32 s32MutexId)
{
    if(MsOS_DeleteMutex(s32MutexId) == TRUE)
    {
        return PVRPL_SYS_STATUS_OK;
    }
    else
    {
        return PVRPL_SYS_STATUS_ERROR;
    }
}

MS_U32 PVRPL_PatternSearch(MS_U32 u32VirAdr, MS_U32 u32SearchLength, ST_INFO_PATTERNSEARCH PatternInfo, void* pParam)
{
    MS_U32 u32PhyAdr = 0 ;
    MS_U32 u32Pattern = (MS_U32)PatternInfo.Pattern;
    MS_U32 u32Exclubit = (MS_U32)PatternInfo.ExcludeBit;
    MS_U32 u32SearchPhAdr = 0;
    MS_U32 u32SearchVirAdr = 0;

    if(FALSE == _gbIsBDMAInit)
    {
        if(MDrv_BDMA_Init(Pool_GetMIUAddress(E_MEM_MIU1)) != E_BDMA_OK)
        {
           PVRPL_SYS_DBGMSG_ERR("MDrv_BDMA_Init failed\n");
           return 0;
        }
        _gbIsBDMAInit = TRUE;
        PVRPL_SYS_DBGMSG_ERR("MIU addr:%"DTC_MS_U32_x"\n",(MS_U32)Pool_GetMIUAddress(E_MEM_MIU1));
    }

    PVRPL_VA2PA(u32VirAdr,&u32PhyAdr);

    /*  it is the temporary solution, and it will be removed when drvBDMA updating  */
    MsOS_ObtainMutex(m_BDMAMutex,MSOS_WAIT_FOREVER);
    u32SearchPhAdr = MDrv_BDMA_Search((MS_PHY)u32PhyAdr,u32SearchLength,u32Pattern,u32Exclubit,E_BDMA_SRCDEV_MIU0);
    MsOS_ReleaseMutex(m_BDMAMutex);

    if(PVRPL_IsValidAddr(u32SearchPhAdr,u32PhyAdr,u32SearchLength))
    {
        PVRPL_PA2VA(u32SearchPhAdr,&u32SearchVirAdr);
        return u32SearchVirAdr;
    }
    else
    {
        if(u32SearchPhAdr != INVALID_ADDR)
        {
            PVRPL_SYS_DBGMSG_ERR("Phy addr (0x%"DTC_MS_U32_x") from BDMA is invalid! It should be 0x%"DTC_MS_U32_x" - 0x%"DTC_MS_U32_x" \n",u32SearchPhAdr,u32PhyAdr,u32PhyAdr + u32SearchLength - 1)
        }
        return 0;
    }
}

MS_BOOL _PVRPL_CreateEventGroup(MS_S32 *s32EventId,MS_U8 *pName)
{
    *s32EventId = MsOS_CreateEventGroup((char*)pName);

    if(*s32EventId == INVALID_MUTEX_ID)
    {
        PVRPL_SYS_DBGMSG_ERR("PVR PL Create Event Group Failed!\n");
        return FALSE;
    }
    return TRUE;
}

MS_BOOL _PVRPL_DeleteEventGroup(MS_S32 *s32EventId)
{
    return MsOS_DeleteEventGroup(*s32EventId);
}

MS_BOOL _PVRPL_SetEvent(MS_S32 *s32EventId,MS_U32 u32EventFlag)
{
    return MsOS_SetEvent(*s32EventId,u32EventFlag);
}

MS_BOOL _PVRPL_ClearEvent(MS_S32 *s32EventId,MS_U32 u32EventFlag)
{
    return MsOS_ClearEvent(*s32EventId,u32EventFlag);
}

MS_BOOL _PVRPL_WaitEvent(MS_S32 *s32EventId,MS_U32 u32WaitEventFlag)
{
    MS_U32 u32Event;
    MS_BOOL bret = MsOS_WaitEvent (*s32EventId,
                        u32WaitEventFlag,
                        &u32Event,
                        E_AND_CLEAR,
                        PVRPL_EVENT_WAIT_FOREVER);
    return bret;
}

void _MemoryFlush(MS_U32 u32VirSrcAdr,MS_U32 u32DataSize)
{

    //if(pFunc.pMemFlushFunc!=NULL)
    //{
    //    pFunc.pMemFlushFunc(u32VirSrcAdr,u32DataSize);
    //}
    //else
    {
#if PVR_PA2VA_CACHED
        MsOS_Dcache_Flush(u32VirSrcAdr,u32DataSize);
#endif
        MsOS_Sync();
        MsOS_FlushMemory();
    }
}

PVRPL_SYS_STATUS PVRPL_MemoryFlush(MS_U32 u32VirSrcAdr,MS_U32 u32DataSize)
{
    MsOS_ObtainMutex(m_SysFuncMutex,MSOS_WAIT_FOREVER);
    _MemoryFlush( u32VirSrcAdr, u32DataSize);
    MsOS_ReleaseMutex(m_SysFuncMutex);
    return PVRPL_SYS_STATUS_OK;
}

//PVRPL_STATUS InsertInterrupt(InterruptNum eIntNum, InterruptCb pIntCb);
//PVRPL_STATUS DeleteInterrupt(InterruptNum eIntNum);
void* PVRPL_AllocMem(MS_U32 u32Size)
{
    return MsOS_AllocateMemory (u32Size, gs32NonCachedPoolID);
}

PVRPL_SYS_STATUS PVRPL_FreeMem(void *pBuf)
{
    MS_BOOL bRet = FALSE;

    bRet = MsOS_FreeMemory(pBuf, gs32NonCachedPoolID);
    if(bRet == FALSE)
    {
        return PVRPL_SYS_STATUS_ERROR;
    }
    else
    {
        return PVRPL_SYS_STATUS_OK;
    }
}

MS_U32 _PA2VA(MS_U32 u32PhyAdr)
{
    MS_U32 u32VirAdr = 0;
    MS_PHY PHYAdr = (MS_PHY)u32PhyAdr;

    //if(pFunc.pPA2VAFunc!=NULL)
    //{
    //    u32VirAdr = pFunc.pPA2VAFunc(u32PhyAdr);
    //}
    //else
    {
#if PVR_PA2VA_CACHED
        u32VirAdr = MsOS_PA2KSEG0(PHYAdr);
#else  //default
        u32VirAdr = MsOS_PA2KSEG1(PHYAdr);
#endif
    }
    ASSERT(u32VirAdr);

    return u32VirAdr;
}
