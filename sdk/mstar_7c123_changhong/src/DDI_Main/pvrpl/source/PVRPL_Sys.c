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

#include "PVRPL_ResourcePool.h"
#include "PVRPL_Sys.h"
#include "apiPVR.h"

#include "drvAESDMA.h"
#include "drvDSCMB.h"
#include "MsOS.h"
#include "drvDTC.h"
#include "drvBDMA.h"

extern MS_S32 gs32CachedPoolID ;
extern MS_S32 gs32NonCachedPoolID;


#define PVR_AESDMA_SPEED_TEST 0
#define ENABLE_AESDMA 1
#define AESDMA_TIMEOUT 500  //ms
#define PVR_PA2VA_CACHED 0
#define INVALID_ADDR 0xFFFFFFFF

#define AESDMA_BYTES_PER_MS    51200 // 50 * 1024 bytes
#define PVR_AES_ALIGNMENT_LEN (16)

#define PVRPL_SYS_DBGMSG(_level,_f) {if(_u32PVRPLSYSDbgLevel >= (_level)) (_f);}
static MS_U32  _u32PVRPLSYSDbgLevel = PVRPLSYS_DBG_ERR;

#if ENABLE_AESDMA
#define AESDAM_BUF_LENGTH 16
#define AESDMA_KEY_LENGTH 4
#define PVR_KEYLADDER 1
#define DEFAULT_AESDMA_KEY1 0x4D535450
#define DEFAULT_AESDMA_KEY2 0x56523039
#define DEFAULT_AESDMA_KEY3 0x32313731
#define DEFAULT_AESDMA_KEY4 0x36303036
#endif

//Event Global const
#define DEMO_EVENT_WAIT_FOREVER 0xffffffff

#define ASSERT(_x_)                                                                         \
    do  {                                                                                   \
        if ( ! ( _x_ ) )                                                                    \
        {                                                                                   \
            printf("ASSERT FAIL: %s %s %d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);     \
            abort();                                                                        \
        }                                                                                   \
    } while (0)

#if (DEMO_PVR_UTOPIA2L_64BIT_TEST)
#define PVRPL_UTOPIA64BIT_MASK(Dest, Source){Dest = Source & 0xFFFFFFFFUL;}
#else
#define PVRPL_UTOPIA64BIT_MASK(Dest, Source){Dest = Source;}
#endif



/****** Local variable ******/
//mutex for PVR_SYS_FUNC
static MS_S32 m_SysFuncMutex = -1;
static MS_BOOL b_SysInit = FALSE;
static MS_S32 m_AESDMAMutex = -1;
static MS_BOOL m_bIsAESDMAEnabled = FALSE;

static MS_BOOL _gbIsBDMAInit = FALSE;
/***************************/


#if ENABLE_AESDMA

static MS_U32 u32AESDMAKeySet[EN_PVRPL_ENCRYPT_AESDMA_NUM][AESDMA_KEY_LENGTH];

#endif

static MS_U32 _PA2VA(MS_U32 u32PhyAdr);


static void _MemoryFlush(MS_U32 u32VirSrcAdr,MS_U32 u32DataSize);


static MS_BOOL _AESDMALock(void);

static MS_BOOL _AESDMAUnlock(void);

#if ENABLE_AESDMA
static void _AESDMASetKey(MS_U32 u32KeyArray[AESDMA_KEY_LENGTH],
    const MS_U32 key1, const MS_U32 key2, const MS_U32 key3, const MS_U32 key4);
static void _AESDMAGetKey(const MS_U32 u32KeyArray[AESDMA_KEY_LENGTH],
    MS_U32 *key1, MS_U32 *key2, MS_U32 *key3, MS_U32 * key4);
#endif


//event related setup functions
static MS_BOOL _PVRPL_CreateEventGroup(MS_S32 *s32EventId,MS_U8 *pName);
static MS_BOOL _PVRPL_DeleteEventGroup(MS_S32 *s32EventId);
static MS_BOOL _PVRPL_SetEvent(MS_S32 *s32EventId,MS_U32 u32EventFlag);
static MS_BOOL _PVRPL_ClearEvent(MS_S32 *s32EventId,MS_U32 u32EventFlag);
static MS_BOOL _PVRPL_WaitEvent(MS_S32 *s32EventId,MS_U32 u32WaitEventFlag);




PVRPL_SYS_STATUS PVRPL_SysInit()
{
    MS_PHY PHYmiu0addr = 0;
    MS_PHY PHYmiu1addr = 0x20000000;

    if(b_SysInit == TRUE)
    {
        return PVRPL_SYS_STATUS_OK;
    }

#if ENABLE_AESDMA
    //Init AES DMA, this SHOULD be set properly with MMAP.
    MDrv_AESDMA_Init(PHYmiu0addr, PHYmiu1addr, 2);
    memset(u32AESDMAKeySet,0,sizeof(u32AESDMAKeySet));
    m_bIsAESDMAEnabled = TRUE; //default is enabled if ENABLE_AESDMA is set to TRUE.

    MS_U8 i=0;
    for(i=0;i<EN_PVRPL_ENCRYPT_AESDMA_NUM;i++)//set all key set to default
        _AESDMASetKey(u32AESDMAKeySet[i], DEFAULT_AESDMA_KEY1, DEFAULT_AESDMA_KEY2, DEFAULT_AESDMA_KEY3, DEFAULT_AESDMA_KEY4);
    m_AESDMAMutex=MsOS_CreateMutex(E_MSOS_FIFO, (char*)"AESDMAMutex", MSOS_PROCESS_SHARED);
    MS_ASSERT(-1!=m_AESDMAMutex);
#endif

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
     //printf("[%s][%s] %d\n", __FILE__,__FUNCTION__, __LINE__); 
    PHYAdr = MsOS_VA2PA(u32VirAdr);
    PVRPL_UTOPIA64BIT_MASK(*u32PhyAdr,PHYAdr);
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
            printf("[MWPVR]  Invalid id 0x%lx\n",u32PvrId);
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
            printf("[MWPVR]  Invalid id 0x%lx\n",u32PvrId);
            return FALSE;
    }

}
*/

#if (ENABLE_AESDMA)

PVRPL_SYS_STATUS PVRPL_SetAESDMAEnabled(MS_BOOL bAESDMAEnabled)
{
    m_bIsAESDMAEnabled = bAESDMAEnabled;
    return PVRPL_SYS_STATUS_OK;
}

MS_U32 PVRPL_GetAESDMABufLength(void)
{
    return (MS_U32)AESDAM_BUF_LENGTH;
}

PVRPL_SYS_STATUS PVRPL_AESDMAEncryptDecrypt(MS_U32 u32Address, MS_U32 u32Length,
            enPLAESDMAMode eMode, PVRPL_ENCRYPT_TYPE enEncryptType)
{
    if (FALSE == m_bIsAESDMAEnabled)//AESDMA Disable
    {
        return PVRPL_SYS_STATUS_OK;
    }
    ASSERT(EN_PVRPL_ENCRYPT_NONE < enEncryptType);
    ASSERT(EN_PVRPL_ENCRYPT_AESDMA_NUM > enEncryptType);

    MS_BOOL bDescrypt=FALSE;
    MS_U32 u32CurrTime = 0;

    MS_U32 u32AESPhyAddr;
    MS_PHY PHYAESPhyAddr;
    MS_U32 u32AESVirAddr;
    MS_U32 u32DelayTimeMS = (u32Length / AESDMA_BYTES_PER_MS);


#if 0//for aesdma debug using
    printf("[%s:%s:%d]eMode : %u\n",__FILE__,__FUNCTION__,__LINE__,eMode);
    printf("[%s:%s:%d]u32AESDMAKeySet[%u]\n",__FILE__,__FUNCTION__,__LINE__,enEncryptType);
    for(MS_U8 index=0;index<AESDMA_KEY_LENGTH;index++)
        printf(" 0x%08lX ",u32AESDMAKeySet[enEncryptType][index]);
    printf("============================================================================\n");
#endif

    _AESDMALock();

    bDescrypt=(eMode == EN_PVRPL_ENCRYPT)?FALSE:TRUE;

    u32AESPhyAddr = u32Address;
    PHYAESPhyAddr = (MS_PHY)u32AESPhyAddr;
    u32AESVirAddr = _PA2VA(u32AESPhyAddr);
    //Init AES DMA
    //MDrv_AESDMA_Init(0, 0, 1);

    //Reset AES DMA
    MDrv_AESDMA_Reset();

    //Min. byte limitation (16X)
    if((u32Length <PVR_AES_ALIGNMENT_LEN) || (u32Length%PVR_AES_ALIGNMENT_LEN !=0) || (u32AESPhyAddr%PVR_AES_ALIGNMENT_LEN!=0))
    {
        PVRPL_SYS_DBGMSG(PVRPLSYS_DBG_NONE,printf("[PVR] Error: AES Write Buffer(%8"DTC_MS_U32_u",%8"DTC_MS_U32_u") is invalid in mode %d. \n",u32AESPhyAddr,u32Length,eMode));
        u32Length = 0;
        _AESDMAUnlock();
        return PVRPL_SYS_STATUS_ERROR;
    }



#if PVR_KEYLADDER
    if( (EN_PVRPL_ENCRYPT_SMARTCARD == enEncryptType))
    {
// to do
// add _appDemo_PVR_KLADDER_FUNC in appdemo_pvr.c
    }

    //Set read/write buffer
    PVRPL_MemoryFlush(u32AESVirAddr,u32Length);
    MDrv_AESDMA_SetFileInOut(PHYAESPhyAddr, u32Length, PHYAESPhyAddr, PHYAESPhyAddr+u32Length-1);


    //Set encrypt/descrypt
    MDrv_AESDMA_SelEng(E_DRVAESDMA_CIPHER_ECB, bDescrypt);
    if((EN_PVRPL_ENCRYPT_SMARTCARD == enEncryptType))
    {
        //Set key.
        MDrv_AESDMA_SetKey(NULL);
    }
    else
    {
        MDrv_AESDMA_SetKey(u32AESDMAKeySet[enEncryptType]);
    }
    //Start to act
    MDrv_AESDMA_Start(TRUE);

    if (u32DelayTimeMS > 0)
    {
        PVRPL_SleepMS(u32DelayTimeMS);
    }

    u32CurrTime = MsOS_GetSystemTime();
    while(MDrv_AESDMA_IsFinished() != DRVAESDMA_OK)
    {
        if((MsOS_GetSystemTime()-u32CurrTime)>AESDMA_TIMEOUT)//aesdma process timeout
        {
            PVRPL_SYS_DBGMSG(PVRPLSYS_DBG_ERR,printf("!!!!!!!!!!!!!!!!!!!!!!!!! aesdma process timeout !!!!!!!!!!!!!!!!!!!!!!!!!\n"));
            break;
        }
        PVRPL_SleepMS(10);
    }

    PVRPL_MemoryFlush(u32AESVirAddr,u32Length);
    _AESDMAUnlock();
    return PVRPL_SYS_STATUS_OK;

#else /*PVR_KEYLADDER*/


    PVRPL_MemoryFlush(u32AESVirAddr,u32Length);
    //Set read/write buffer
    MDrv_AESDMA_SetFileInOut(PHYAESPhyAddr, u32Length, PHYAESPhyAddr, PHYAESPhyAddr+u32Length-1);

    //Set key.
    MDrv_AESDMA_SetKey(u32AESDMAKeySet[enEncryptType]);
    //Start to encrypt/descrypt

    MDrv_AESDMA_SelEng(E_DRVAESDMA_CIPHER_ECB, bDescrypt);

    MDrv_AESDMA_Start(TRUE);

    if (u32DelayTimeMS > 0)
    {
        PVRPL_SleepMS(u32DelayTimeMS);
    }

    u32CurrTime = MsOS_GetSystemTime();
    while(MDrv_AESDMA_IsFinished() != DRVAESDMA_OK)
    {
        if((MsOS_GetSystemTime()-u32CurrTime)>AESDMA_TIMEOUT)//aesdma process timeout
        {
            PVRPL_SYS_DBGMSG(PVRPLSYS_DBG_ERR,printf("!!!!!!!!!!!!!!!!!!!!!!!!! aesdma process timeout !!!!!!!!!!!!!!!!!!!!!!!!!\n"));
            break;
        }
        PVRPL_SleepMS(10);
    }


    PVRPL_MemoryFlush(u32AESVirAddr,u32Length);
    _AESDMAUnlock();
#endif /*PVR_KEYLADDER*/

#if PVR_AESDMA_SPEED_TEST
    PVRPL_SYS_DBGMSG(PVRPLSYS_DBG_NONE,printf("============= AESDMA Speed test =============\n"));
    for(MS_U8 u8Count = 0; u8Count < 4; ++u8Count)
    {
        //Init AES DMA
        //MDrv_AESDMA_Init(0, 0, 1);

        //Reset AES DMA
        MDrv_AESDMA_Reset();

        //Min. byte limitation (16X)
        if(u32Length < 16)
        {
            break;
        }

        //Set read/write buffer
        MDrv_AESDMA_SetFileInOut(PHYAESPhyAddr, u32Length, PHYAESPhyAddr, PHYAESPhyAddr+u32Length-1);

        if(eMode == EN_PVRPL_ENCRYPT)
        {
            //Set key.
            MDrv_AESDMA_SetKey(u32EncpAESDMAKey);
            //Start to encrypt
            MDrv_AESDMA_SelEng(E_DRVAESDMA_CIPHER_ECB, TRUE);
            MDrv_AESDMA_Start(TRUE);
        }
        else
        {
            //Set key.
            MDrv_AESDMA_SetKey(u32DecpAESDMAKey);
            //Start to decrypt
            MDrv_AESDMA_SelEng(E_DRVAESDMA_CIPHER_ECB, FALSE);
            MDrv_AESDMA_Start(TRUE);
        }

        MS_U32 _time0 = MsOS_GetSystemTime();
        MS_U16 u16Index = 0; 
        for (u16Index = 0; u16Index < 65535; u16Index++)
        {
            if (DRVAESDMA_OK == MDrv_AESDMA_IsFinished())
                break;
        }

        MS_U32 _time1 = MsOS_GetSystemTime();
        PVRPL_SYS_DBGMSG(PVRPLSYS_DBG_NONE,printf("delta = %lu, length = %lu, u16Index = %u\n", _time1 - _time0, u32Length, u16Index));
        u32Length /= 2;
    }
    PVRPL_SYS_DBGMSG(PVRPLSYS_DBG_NONE,printf("=============================================\n"));
    _AESDMAUnlock();
#endif /*PVR_AESDMA_SPEED_TEST*/
    return PVRPL_SYS_STATUS_OK;
}


PVRPL_SYS_STATUS PVRPL_AESDMASetKey(PVRPL_ENCRYPT_TYPE enEncryptType,
            MS_U32 u32Key1, MS_U32 u32Key2, MS_U32 u32Key3, MS_U32 u32Key4)
{
    ASSERT(EN_PVRPL_ENCRYPT_NONE < enEncryptType);
    ASSERT(EN_PVRPL_ENCRYPT_AESDMA_NUM > enEncryptType);
    _AESDMASetKey(u32AESDMAKeySet[enEncryptType], u32Key1, u32Key2, u32Key3, u32Key4);
    return PVRPL_SYS_STATUS_OK;
}


PVRPL_SYS_STATUS PVRPL_AESDMAGetKey(PVRPL_ENCRYPT_TYPE enEncryptType,
            MS_U32 *u32Key1, MS_U32 *u32Key2, MS_U32 *u32Key3, MS_U32 *u32Key4)
{
    ASSERT(EN_PVRPL_ENCRYPT_NONE < enEncryptType);
    ASSERT(EN_PVRPL_ENCRYPT_AESDMA_NUM > enEncryptType);
    _AESDMAGetKey(u32AESDMAKeySet[enEncryptType], u32Key1, u32Key2, u32Key3, u32Key4);
    return PVRPL_SYS_STATUS_OK;
}

void _AESDMASetKey(MS_U32 u32KeyArray[AESDMA_KEY_LENGTH],
    const MS_U32 key1, const MS_U32 key2, const MS_U32 key3, const MS_U32 key4)
{
    u32KeyArray[0] = key1;
    u32KeyArray[1] = key2;
    u32KeyArray[2] = key3;
    u32KeyArray[3] = key4;
}
void _AESDMAGetKey(const MS_U32 u32KeyArray[AESDMA_KEY_LENGTH],
    MS_U32 *key1, MS_U32 *key2, MS_U32 *key3, MS_U32 * key4)
{
    *key1 = u32KeyArray[0];
    *key2 = u32KeyArray[1];
    *key3 = u32KeyArray[2];
    *key4 = u32KeyArray[3];
}

#endif

PVRPL_SYS_STATUS PVRPL_MemoryCopy(MS_U32 u32PhySrcAdr,MS_U32 u32PhyDstAdr,MS_U32 u32Len)
{
    MsOS_ObtainMutex(m_SysFuncMutex,MSOS_WAIT_FOREVER);
    // Current memory size is less than 256 MB
    if (u32Len> 0x10000000)
    {
        PVRPL_SYS_DBGMSG(PVRPLSYS_DBG_NONE,printf("Warning!!! DMA size: %x\n", (unsigned int)u32Len));
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
            PVRPL_SYS_DBGMSG(PVRPLSYS_DBG_ERR,printf("[PVRPL]  No such event cmd %d\n",EventCMD));
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
    (*ps32MutexID) = -1;
    (*ps32MutexID) = MsOS_CreateMutex(E_MSOS_FIFO, (char*)pu8MutexName, MSOS_PROCESS_SHARED);

    if((*ps32MutexID) == -1)
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
        MDrv_BDMA_Init(Pool_GetMIUAddress(E_MEM_MIU1));
        _gbIsBDMAInit = TRUE;
        PVRPL_SYS_DBGMSG(PVRPLSYS_DBG_ERR,printf("MIU addr:%lx\n",(MS_U32)Pool_GetMIUAddress(E_MEM_MIU1)));
    }
    
    PVRPL_VA2PA(u32VirAdr,&u32PhyAdr);
    u32SearchPhAdr = MDrv_BDMA_Search((MS_PHY)u32PhyAdr,u32SearchLength,u32Pattern,u32Exclubit,E_BDMA_SRCDEV_MIU0);
    if(u32SearchPhAdr != INVALID_ADDR)
    {
        PVRPL_PA2VA(u32SearchPhAdr,&u32SearchVirAdr);    
        return u32SearchVirAdr;
    }
    else
    {
        return 0;
    }
}



MS_BOOL _PVRPL_CreateEventGroup(MS_S32 *s32EventId,MS_U8 *pName)
{
    *s32EventId = MsOS_CreateEventGroup((char*)pName);

    if(*s32EventId == -1)
    {
        PVRPL_SYS_DBGMSG(PVRPLSYS_DBG_ERR,printf("PVR PL Create Event Group Failed!\n"));
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
                        DEMO_EVENT_WAIT_FOREVER);
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
    return MsOS_FreeMemory(pBuf, gs32NonCachedPoolID);
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



MS_BOOL _AESDMALock(void)
{
    MsOS_ObtainMutex(m_AESDMAMutex,MSOS_WAIT_FOREVER);

    return MDrv_AESDMA_Lock();
}

MS_BOOL _AESDMAUnlock(void)
{
    MsOS_ReleaseMutex(m_AESDMAMutex);

    return MDrv_AESDMA_Unlock();
}


