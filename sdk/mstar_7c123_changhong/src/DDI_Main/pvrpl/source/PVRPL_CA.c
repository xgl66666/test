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
/// @file   PVRPL_CA.h
/// @brief  CPL API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>

#include "PVRPL_CA.h"
#include "PVRPL_Sys.h"
#include "apiPVR.h"

#include "drvAESDMA.h"
#include "drvDSCMB.h"
#include "MsOS.h"
#include "drvDTC.h"


#define ENABLE_2ND_ENCRYPTION           1

#if ENABLE_2ND_ENCRYPTION

#define PVR_AESDMA_SPEED_TEST   0

#define AESDMA_TIMEOUT          500  //ms
#define PVR_PA2VA_CACHED        0
#define INVALID_MUTEX_ID        -1
#define AESDMA_BYTES_PER_MS     51200 // 50 * 1024 bytes
#define PVR_AES_ALIGNMENT_LEN   (16)

#define AESDMA_KEY_LENGTH       4
#define PVR_KEYLADDER           1
#define DEFAULT_AESDMA_KEY1     0x4D535450
#define DEFAULT_AESDMA_KEY2     0x56523039
#define DEFAULT_AESDMA_KEY3     0x32313731
#define DEFAULT_AESDMA_KEY4     0x36303036


//Event Global const
#define PVRPL_EVENT_WAIT_FOREVER 0xffffffff

#define ASSERT(_x_)                                                                         \
    do  {                                                                                   \
        if ( ! ( _x_ ) )                                                                    \
        {                                                                                   \
            printf("ASSERT FAIL: %s %s %d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);     \
            abort();                                                                        \
        }                                                                                   \
    } while (0)

// Debug level
static MS_U32  _u32PVRPLCADbgLevel = PVRPLCA_DBG_ERR;

#define PVRPL_CA_DBGMSG(_level, msg, args...)       {if(_u32PVRPLCADbgLevel >= (_level)) printf("[%s][%d] " msg, __FUNCTION__, __LINE__, ## args);}
#define MOD_NAME                                    PVR

#if defined(HB_ERR)
    #define PVRPL_CA_DBGMSG_ERR(msg, args...)       HB_ERR(msg, ##args)
#else
    #define PVRPL_CA_DBGMSG_ERR(msg, args...)       PVRPL_CA_DBGMSG(PVRPLCA_DBG_ERR, msg, ##args)
#endif

#if defined(HB_INFO)
    #define PVRPL_CA_DBGMSG_INFO(msg, args...)      HB_INFO(msg, ##args)
#else
    #define PVRPL_CA_DBGMSG_INFO(msg, args...)      PVRPL_CA_DBGMSG(PVRPLCA_DBG_INFO, msg, ##args)
#endif

#if defined(HB_TRACE)
    #define PVRPL_CA_DBGMSG_TRACE(msg, args...)     HB_TRACE(msg, ##args)
#else
    #define PVRPL_CA_DBGMSG_TRACE(msg, args...)     PVRPL_CA_DBGMSG(PVRPLCA_DBG_TRACE, msg, ##args)
#endif

#if defined(HB_DBG)
    #define PVRPL_CA_DBGMSG_DEBUG(msg, args...)     HB_DBG(msg, ##args)
#else
    #define PVRPL_CA_DBGMSG_DEBUG(msg, args...)     PVRPL_CA_DBGMSG(PVRPLCA_DBG_FUNC, msg, ##args)
#endif

//mutex for PVR_SYS_FUNC
static MS_S32 m_SysFuncMutex    = INVALID_MUTEX_ID;
static MS_BOOL b_SysInit        = FALSE;


static MS_S32 m_AESDMAMutex     = INVALID_MUTEX_ID;

typedef struct AESDMAKeyInfo
{
    PVRPL_ENCRYPTION_TYPE enType;
    MS_U32 u32Key[AESDMA_KEY_LENGTH];
}KeyInfo;

//static MS_U32 u32AESDMAKeySet[EN_PVRPL_ENCRYPTION_AESDMA_NUM][AESDMA_KEY_LENGTH];
static KeyInfo stKeyInfo[DYNAMIC_KEY_MAX] = // @NOTE DynamicKey Default DYNAMIC_KEY_MAX = 0
{
    [0].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [0].u32Key ={0x4D535450, 0x4D535450, 0x4D535450, 0x4D535450},
    [1].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [1].u32Key ={0x12345678, 0x12345678, 0x12345678, 0x12345678},
    [2].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [2].u32Key ={0x23456789, 0x23456789, 0x23456789, 0x23456789},
    [3].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [3].u32Key ={0x34567890, 0x34567890, 0x34567890, 0x34567890},
    [4].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [4].u32Key ={0x45678901, 0x45678901, 0x45678901, 0x45678901},

    [5].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [5].u32Key ={0x56789012, 0x56789012, 0x56789012, 0x56789012},
    [6].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [6].u32Key ={0x67890123, 0x67890123, 0x67890123, 0x67890123},
    [7].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [7].u32Key ={0x78901234, 0x78901234, 0x78901234, 0x78901234},
    [8].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [8].u32Key ={0x89012345, 0x89012345, 0x89012345, 0x89012345},
    [9].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [9].u32Key ={0x90123456, 0x90123456, 0x90123456, 0x90123456},

    [10].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [10].u32Key ={0x01234567, 0x01234567, 0x01234567, 0x4D535450},
    [11].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [11].u32Key ={0x98765432, 0x98765432, 0x98765432, 0x98765432},
    [12].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [12].u32Key ={0x87654321, 0x87654321, 0x87654321, 0x87654321},
    [13].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [13].u32Key ={0x76543210, 0x76543210, 0x76543210, 0x76543210},
    [14].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [14].u32Key ={0x65432109, 0x65432109, 0x65432109, 0x65432109},

    [15].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [15].u32Key ={0x54321098, 0x54321098, 0x54321098, 0x54321098},
    [16].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [16].u32Key ={0x43210987, 0x43210987, 0x43210987, 0x43210987},
    [17].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [17].u32Key ={0x32109876, 0x32109876, 0x32109876, 0x32109876},
    [18].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [18].u32Key ={0x21098765, 0x21098765, 0x21098765, 0x21098765},
    [19].enType = EN_PVRPL_ENCRYPTION_DEFAULT, [19].u32Key ={0x10987654, 0x10987654, 0x10987654, 0x10987654},
};



static MS_BOOL _CALock(void);
static MS_BOOL _CAUnlock(void);
static PVRPL_CA_STATUS _PVRPL_SetKey(PVRPL_AESDMA_INFO *stAESDMAInfo);


PVRPL_CA_STATUS PVRPL_CAInit(PVRPL_ENCRYPTDECRYPT_INFO *sCurEncryptDecryptInfo)
{

    if(b_SysInit == TRUE)
    {
        return PVRPL_CA_STATUS_OK;
    }

    MS_PHY PHYmiu0addr = 0;
    MS_PHY PHYmiu1addr = 0x20000000;
    //Init AES DMA, this SHOULD be set properly with MMAP.
    MDrv_AESDMA_Init(PHYmiu0addr, PHYmiu1addr, 2);

    MS_U8 i = 0;
    for(i=0; i< DYNAMIC_KEY_MAX; i++)
    {
        PVRPL_CA_DBGMSG_TRACE("[%d][%d],(%"DTC_MS_U32_u", %"DTC_MS_U32_u", %"DTC_MS_U32_u", %"DTC_MS_U32_u")]\n", i, stKeyInfo[i].enType, stKeyInfo[i].u32Key[0], stKeyInfo[i].u32Key[1], stKeyInfo[i].u32Key[2], stKeyInfo[i].u32Key[3]);
    }

    m_AESDMAMutex = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"CaAESDMAMutex", MSOS_PROCESS_SHARED);
    MS_ASSERT(-1!=m_AESDMAMutex);


    m_SysFuncMutex = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"CaPVRSysFuncMutex", MSOS_PROCESS_SHARED);
    MS_ASSERT(-1!=m_SysFuncMutex);

    b_SysInit = TRUE;
    PVRPL_CA_DBGMSG_ERR("CA init, (Path:Mode:Idx) (%u:%u:%u)\n",sCurEncryptDecryptInfo->u8PathIdx,(MAPI_U8)GET_PVR_PATH_MODE(sCurEncryptDecryptInfo->u8PathIdx),(MAPI_U8)GET_PVR_PATH_IDX(sCurEncryptDecryptInfo->u8PathIdx));
    return PVRPL_CA_STATUS_OK;
}

MS_U32 PVRPL_GetCABufLength(void)
{
    return (MS_U32)AESDAM_BUF_LENGTH;
}

PVRPL_CA_STATUS PVRPL_CAEncryptDecrypt(PVRPL_ENCRYPTDECRYPT_INFO *sCurEncryptDecryptInfo, MS_U32 u32Address, MS_U32 u32Length,
            PVRPL_AESDMA_Mode eMode, PVRPL_AESDMA_INFO *stAESDMAInfo, void * pParam)
{
    if (FALSE == sCurEncryptDecryptInfo->bEnable)//AESDMA Disable
    {
        return PVRPL_CA_STATUS_OK;
    }
    ASSERT(EN_PVRPL_ENCRYPTION_NONE < stAESDMAInfo->enEncryptType);
    ASSERT(EN_PVRPL_ENCRYPTION_AESDMA_NUM > stAESDMAInfo->enEncryptType);

    MS_BOOL bDescrypt=FALSE;
    MS_U32 u32CurrTime = 0;

    MS_U32 u32AESPhyAddr;
    MS_U32 u32AESVirAddr;
    MS_U32 u32DelayTimeMS = (u32Length / AESDMA_BYTES_PER_MS);


#if 0//for aesdma debug using
    PVRPL_CA_DBGMSG_ERR("eMode : %u\n", eMode);
    PVRPL_CA_DBGMSG_ERR("u32AESDMAKeySet[%u]\n", stAESDMAInfo->enEncryptType);
    MS_U8 index=0;
    for(index=0;index<AESDMA_KEY_LENGTH;index++)
        PVRPL_CA_DBGMSG_ERR(" 0x%08lX ",stAESDMAInfo->unKeySet.u32AESDMAKey[index]);
    PVRPL_CA_DBGMSG_ERR("============================================================================\n");
#endif

    _CALock();

    bDescrypt=(eMode == EN_PVRPL_ENCRYPTION)?FALSE:TRUE;

    u32AESPhyAddr = u32Address;
    PVRPL_PA2VA(u32AESPhyAddr, &u32AESVirAddr);
    //Init AES DMA
    //MDrv_AESDMA_Init(0, 0, 1);

    //Reset AES DMA
    MDrv_AESDMA_Reset();

    //Min. byte limitation (16X)
    if((u32Length <PVR_AES_ALIGNMENT_LEN) || (u32Length%PVR_AES_ALIGNMENT_LEN !=0) || (u32AESPhyAddr%PVR_AES_ALIGNMENT_LEN!=0))
    {
        PVRPL_CA_DBGMSG_ERR("[PVR] Error: AES Write Buffer %"DTC_MS_U32_x",%"DTC_MS_U32_x" is invalid in mode %d. \n",u32AESPhyAddr,u32Length,eMode);
        u32Length = 0;
        _CAUnlock();
        return PVRPL_CA_STATUS_ERROR;
    }

#if PVR_KEYLADDER
    if( (EN_PVRPL_ENCRYPTION_SMARTCARD == stAESDMAInfo->enEncryptType))
    {
        MS_U8 u8KLInputKey[48]={0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
                                0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
                                0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f};
        MS_U8 key[16] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};
        DSCMB_KL_Status KL_Status = 0;
        MS_U8 ACPU_Out[16] = {0x00};

        DSCMB_KLCfg_All KLConfigAll =
        {
            .eAlgo = E_DSCMB_KL_AES,
            .eSrc = E_DSCMB_KL_SRC_ACPU,
            .eDst = E_DSCMB_KL_DST_DMA_AES,
            .eOutsize = E_DSCMB_KL_128_BITS,
            .eKeyType = 0,  // Don't care when dst is Crypto DMA
            .u32Level = 3,
            .u32EngID = 0,  //Set to zero in STB chip, Don't care when dst is Crypto DMA
            .u32DscID = 0,  // Don't care when dst is Crypto DMA
            .u8KeyACPU = key,
            .pu8KeyKLIn = u8KLInputKey,
            .bDecrypt = TRUE,
            .bInverse = FALSE,
            .eKLSel   = E_DSCMB_KL_SEL_DEFAULT,
            // For CIPHE key source from KL and set to Secrete key (eSrc = E_DEMO_KL_SRC_SECRET_1 ~E_DEMO_KL_SRC_SECRET_4),
            // the CAVID in Secrete key need equal to CIPHER and KL
            .u32CAVid = 0xF, //CA vender ID , 0xF: engineering sample (security LV: the most loosely)
        };

        // Key Ladder
        if(MDrv_DSCMB2_KLadder_AtomicExec(&KLConfigAll, ACPU_Out, &KL_Status) == FALSE)
        {
            PVRPL_CA_DBGMSG_ERR("Key Ladder: Fail!!! Status = 0x%x\n", (unsigned int)KL_Status);
            _CAUnlock();
            return PVRPL_CA_STATUS_ERROR;
        }
    }
#endif

    PVRPL_MemoryFlush(u32AESVirAddr, u32Length);

    //Set read/write buffer
    MS_PHY PHYAESPhyAddr = (MS_PHY)u32AESPhyAddr;
    MDrv_AESDMA_SetFileInOut(PHYAESPhyAddr, u32Length, PHYAESPhyAddr, PHYAESPhyAddr+u32Length-1);

    //Set key. @NOTE KEYLADDER do not need to set key
    _PVRPL_SetKey(stAESDMAInfo);

    //Set encrypt/descrypt
    MDrv_AESDMA_SelEng(E_DRVAESDMA_CIPHER_ECB, bDescrypt);

    //Start to act
    MDrv_AESDMA_Start(TRUE);

    if (u32DelayTimeMS > 0)
    {
        MsOS_DelayTask(u32DelayTimeMS);
    }

    u32CurrTime = MsOS_GetSystemTime();
    while(MDrv_AESDMA_IsFinished() != DRVAESDMA_OK)
    {
        if((MsOS_GetSystemTime()-u32CurrTime)>AESDMA_TIMEOUT)//aesdma process timeout
        {
            PVRPL_CA_DBGMSG_ERR("!!!!!!!!!!!!!!!!!!!!!!!!! aesdma process timeout !!!!!!!!!!!!!!!!!!!!!!!!!\n");
            break;
        }
        MsOS_DelayTask(10);
    }

    PVRPL_MemoryFlush(u32AESVirAddr, u32Length);
    _CAUnlock();

#if PVR_AESDMA_SPEED_TEST
    PVRPL_CA_DBGMSG_ERR("============= AESDMA Speed test =============\n");

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
        PVRPL_CA_DBGMSG_ERR("delta = %"DTC_MS_U32_u", length = %"DTC_MS_U32_u", u16Index = %u\n", _time1 - _time0, u32Length, u16Index);
        u32Length /= 2;
    }
    PVRPL_CA_DBGMSG_ERR("=============================================\n");
    _CAUnlock();
#endif /*PVR_AESDMA_SPEED_TEST*/
    return PVRPL_CA_STATUS_OK;
}

PVRPL_CA_STATUS _PVRPL_SetKey(PVRPL_AESDMA_INFO *stAESDMAInfo)
{
    MS_U32 u32KeyIdx = 0; // for Default/USER usage
#if PVR_KEYLADDER
    //Set key.
    if(EN_PVRPL_ENCRYPTION_USER == stAESDMAInfo->enEncryptType)
    {
        stKeyInfo[u32KeyIdx].enType =  stAESDMAInfo->enEncryptType;
        memset(stKeyInfo[u32KeyIdx].u32Key, 0, sizeof(stKeyInfo[u32KeyIdx].u32Key));
        memcpy(stKeyInfo[u32KeyIdx].u32Key, stAESDMAInfo->unKeySet.u32AESDMAKey, sizeof(stAESDMAInfo->unKeySet.u32AESDMAKey));
    }
    else if(EN_PVRPL_ENCRYPTION_DYNAMICKEY == stAESDMAInfo->enEncryptType)
    {
        u32KeyIdx = stAESDMAInfo->unKeySet.u32KeyIdx;
    }

    PVRPL_CA_DBGMSG_TRACE("\033[1;36m=== Idx:%"DTC_MS_U32_u", Type:%d, Key:{0x%"DTC_MS_U32_x",0x%"DTC_MS_U32_x",0x%"DTC_MS_U32_x",0x%"DTC_MS_U32_x"} ===\n\033[m",
        u32KeyIdx, stKeyInfo[u32KeyIdx].enType,
        stKeyInfo[u32KeyIdx].u32Key[0], stKeyInfo[u32KeyIdx].u32Key[1], stKeyInfo[u32KeyIdx].u32Key[2], stKeyInfo[u32KeyIdx].u32Key[3]);

    switch(stKeyInfo[u32KeyIdx].enType)
    {
        case EN_PVRPL_ENCRYPTION_SMARTCARD:
            MDrv_AESDMA_SetKey(NULL);
            break;
        case  EN_PVRPL_ENCRYPTION_DYNAMICKEY:
            MDrv_AESDMA_SetKey(stKeyInfo[u32KeyIdx].u32Key);
            break;
        default:
            MDrv_AESDMA_SetKey(stKeyInfo[u32KeyIdx].u32Key);
            break;
    }

#else
    MDrv_AESDMA_SetKey(stKeyInfo[u32KeyIdx].u32Key);
#endif

    return PVRPL_CA_STATUS_OK;
}

static MS_BOOL _CALock(void)
{
    MsOS_ObtainMutex(m_AESDMAMutex,MSOS_WAIT_FOREVER);

    return MDrv_AESDMA_Lock();
}

static MS_BOOL _CAUnlock(void)
{
    MsOS_ReleaseMutex(m_AESDMAMutex);

    return MDrv_AESDMA_Unlock();
}
#endif
