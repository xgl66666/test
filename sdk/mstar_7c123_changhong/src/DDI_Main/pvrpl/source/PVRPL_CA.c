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
#include "drvCIPHER.h"
#include "drvDTC.h"


#define ENABLE_AESDMA (1)
#if ENABLE_AESDMA
#define AESDMA_KEY_LENGTH   (4)
#define AESDMA_TIMEOUT  (500)  //ms
#define AESDMA_BYTES_PER_MS (51200) // 50 * 1024 bytes
#define PVR_AES_ALIGNMENT_LEN   (16)
#define PVR_KEYLADDER   (1)
#endif

#define ASSERT(_x_)                                                                         \
    do  {                                                                                   \
        if ( ! ( _x_ ) )                                                                    \
        {                                                                                   \
            printf("ASSERT FAIL: %s %s %d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);     \
            abort();                                                                        \
        }                                                                                   \
    } while (0)

#ifndef NARGA_CAVID
#define NARGA_CAVID    (0x02)
#endif

#define PVRPL_CA_DBGMSG(_level,_f) {if(_u32PVRPLCADbgLevel >= (_level)) (_f);}


static MS_U32  _u32PVRPLCADbgLevel = PVRPLCA_DBG_ERR;
static MS_BOOL _gbPVRPLCAInit = FALSE;
static MS_S32 m_AESDMAMutex = -1;

#if ENABLE_AESDMA
typedef struct AESDMAKeyInfo
{
    PVRPL_ENCRYPTION_TYPE enType;
    MS_U32 u32Key[AESDMA_KEY_LENGTH];
}KeyInfo;

typedef struct
{
    MS_BOOL bDescrypt;
    MS_U32 u32PhyAddr;
    MS_U32 u32Length;
    MS_U32 u32CipherKey[AESDMA_KEY_LENGTH]; // for USER MODE
    MS_U32 return_Val;
}ST_CIPHER_R2R_START;

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
static DSCMB_KLDst _GetDSCMBKeyLadderDstByCipherId(MS_U32 u32CipherId);
static MS_BOOL _KeyLadder(MS_U32 u32CipherId);
static MS_BOOL _Cipher_R2R_Start(ST_CIPHER_R2R_START *pstCipherR2RData);
#endif


#if (ENABLE_AESDMA)
static MS_BOOL _CALock(void)
{
    return MsOS_ObtainMutex(m_AESDMAMutex, MSOS_WAIT_FOREVER);
}

static MS_BOOL _CAUnlock(void)
{
    return MsOS_ReleaseMutex(m_AESDMAMutex);
}

static DSCMB_KLDst _GetDSCMBKeyLadderDstByCipherId(MS_U32 u32CipherId)
{
    switch(u32CipherId)
    {
        case 0:                                 {return E_DSCMB_KL_DST_DMA_SK0;}
        case 1:                                 {return E_DSCMB_KL_DST_DMA_SK1;}
        case 2:                                 {return E_DSCMB_KL_DST_DMA_SK2;}
        case 3:                                 {return E_DSCMB_KL_DST_DMA_SK3;}
        default:
        {
            PVRPL_CA_DBGMSG(PVRPLCA_DBG_ERR, printf("[%s][%d] Warning: u32CipherId(%lu) undefined, using E_DSCMB_KL_DST_DMA_SK0 instead!!\n", __FUNCTION__, __LINE__, (unsigned long)u32CipherId));
            return E_DSCMB_KL_DST_DMA_SK0;
        }
    }
}

static MS_BOOL _KeyLadder(MS_U32 u32CipherId)
{
    MS_U8  u8KLInputKey[48]={0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f, \
                            0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,\
                            0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f};
    DSCMB_KL_Status KL_Status = 0;

    DSCMB_KLCfg_All KLConfigAll = {
        .eAlgo = E_DSCMB_KL_TDES,
        .eSrc = E_DSCMB_KL_SRC_SECRET_4,//KLSrc,
        .eDst = E_DSCMB_KL_DST_DMA_SK0,
        .eOutsize = E_DSCMB_KL_128_BITS,
        .eKeyType = 0,
        .u32Level = 3,
        .u32EngID = 0,
        .u32DscID = 0,
        .u8KeyACPU = 0,
        .pu8KeyKLIn = u8KLInputKey,
        .bDecrypt = TRUE,
        .bInverse = FALSE,
        .eKLSel = E_DSCMB_KL_SEL_CW,
        .u32CAVid = NARGA_CAVID, //Nagra CAVID
    };
    KLConfigAll.eDst = _GetDSCMBKeyLadderDstByCipherId(u32CipherId);
    if(FALSE == MDrv_DSCMB2_KLadder_AtomicExec(&KLConfigAll , NULL, &KL_Status ))
    {
        PVRPL_CA_DBGMSG(PVRPLCA_DBG_ERR, printf("\033[1;31m""Key Ladder: Fail!!! 0x%x""\033[m\n",(unsigned int)KL_Status));
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL _Cipher_R2R_Start(ST_CIPHER_R2R_START *pstCipherR2RData)
{
    MS_U32 u32DelayTimeMS = (pstCipherR2RData->u32Length / AESDMA_BYTES_PER_MS);
    MS_U32 u32CurrTime = 0;
    MS_U32 u32CipherId = 0;
    MS_U32 u32CmdId = 0;
    MS_U32 u32Exception = 0;
    DRV_CIPHER_RET enRet = DRV_CIPHER_FAIL;
    DRV_CIPHER_DMACFG stCfg = {
        .stAlgo.eMainAlgo = E_CIPHER_MAIN_TDES,
        .stAlgo.eSubAlgo = E_CIPHER_SUB_ECB,
        .stAlgo.eResAlgo = E_CIPHER_RES_CLR,
        .stAlgo.eSBAlgo = E_CIPHER_SB_CLR,
        .stKey.eKeySrc = E_CIPHER_KSRC_KL,
        .stKey.u8KeyIdx = 0,
        //Set read buffer
        .stInput.u32Addr = pstCipherR2RData->u32PhyAddr,
        .stInput.u32Size = pstCipherR2RData->u32Length,
        //Set write buffer
        .stOutput.u32Addr = pstCipherR2RData->u32PhyAddr,
        .stOutput.u32Size = pstCipherR2RData->u32Length,
        //Set encrypt/descrypt
        .bDecrypt = pstCipherR2RData->bDescrypt,
        .u32CAVid = NARGA_CAVID, //Nagra CAVID
    };

    enRet = MDrv_CIPHER_Alloc(&u32CipherId);
    if(DRV_CIPHER_OK != enRet)
    {
        printf("[%s][%d] Allocate Cipher slot failed(%lu)!!\n", __FUNCTION__, __LINE__,  (unsigned long)enRet);
        return FALSE;
    }

    stCfg.stKey.u8KeyIdx = (MS_U8)u32CipherId;
    if(0x0 == pstCipherR2RData->u32CipherKey[0])
    {
        _KeyLadder(u32CipherId);
    }
    else
    {
        stCfg.stKey.eKeySrc = E_CIPHER_KSRC_CPU;
        stCfg.stKey.u8KeyLen = (MS_U8)sizeof(pstCipherR2RData->u32CipherKey);
        stCfg.stKey.pu8KeyData = (MS_U8 *)pstCipherR2RData->u32CipherKey;
    }

    enRet = MDrv_CIPHER_DMAConfigure(u32CipherId, stCfg);
    if(DRV_CIPHER_OK != enRet)
    {
        PVRPL_CA_DBGMSG(PVRPLCA_DBG_ERR, printf("[%s][%d] Cipher DMA configure failed(%lu)!!!\n", __FUNCTION__, __LINE__, (unsigned long) enRet));
        return FALSE;
    }

    enRet = MDrv_CIPHER_DMAStart(u32CipherId, &u32CmdId);
    if(DRV_CIPHER_OK != enRet)
    {
        PVRPL_CA_DBGMSG(PVRPLCA_DBG_ERR, printf("[%s][%d] Cipher DMA start failed(%lu)!!!\n", __FUNCTION__, __LINE__,  (unsigned long)enRet));
        return FALSE;
    }

    if (u32DelayTimeMS > 0)
    {
        MsOS_DelayTask(u32DelayTimeMS);
    }

    u32CurrTime = MsOS_GetSystemTime();
    while(TRUE != MDrv_CIPHER_IsDMADone(u32CmdId, &u32Exception))
    {
        if(AESDMA_TIMEOUT < (MsOS_GetSystemTime()-u32CurrTime))//Cipher process timeout
        {
            PVRPL_CA_DBGMSG(PVRPLCA_DBG_ERR, printf("[%s][%d]!!!!!!!!!!!!!!!!!!!!!!!!! Cipher process timeout(%u) !!!!!!!!!!!!!!!!!!!!!!!!!\n", __FUNCTION__, __LINE__, AESDMA_TIMEOUT));
            break;
        }
        MsOS_DelayTask(10);
    }
    enRet = MDrv_CIPHER_Free(u32CipherId);
    if(DRV_CIPHER_OK != enRet)
    {
        PVRPL_CA_DBGMSG(PVRPLCA_DBG_ERR, printf("[%s][%d] Free Cipher slot(%lu) failed(%lu)!!\n", __FUNCTION__, __LINE__,  (unsigned long)u32CipherId,  (unsigned long)enRet));
        return FALSE;
    }

    return TRUE;
}
#endif // #if (ENABLE_AESDMA)

PVRPL_CA_STATUS PVRPL_CAInit(PVRPL_ENCRYPTDECRYPT_INFO *sCurEncryptDecryptInfo)
{
    if(_gbPVRPLCAInit == TRUE)
    {
        return PVRPL_CA_STATUS_OK;
    }

#if ENABLE_AESDMA
    MDrv_CIPHER_Init();

    MS_U8 i = 0;
    for(i=0; i< DYNAMIC_KEY_MAX; i++)
    {
        PVRPL_CA_DBGMSG(PVRPLCA_DBG_TRACE,printf("[%d][%d],(%"DTC_MS_U32_u", %"DTC_MS_U32_u", %"DTC_MS_U32_u", %"DTC_MS_U32_u")]\n", i, stKeyInfo[i].enType, stKeyInfo[i].u32Key[0], stKeyInfo[i].u32Key[1], stKeyInfo[i].u32Key[2], stKeyInfo[i].u32Key[3]));
    }

    m_AESDMAMutex = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"AESDMAMutex", MSOS_PROCESS_SHARED);
    MS_ASSERT(-1 != m_AESDMAMutex);
#endif

    _gbPVRPLCAInit = TRUE;
    PVRPL_CA_DBGMSG(PVRPLCA_DBG_NONE,printf("CA init, (Path:Mode:Idx) (%x:%x:%x)\n",sCurEncryptDecryptInfo->u8PathIdx,(MAPI_U8)GET_PVR_PATH_MODE(sCurEncryptDecryptInfo->u8PathIdx),(MAPI_U8)GET_PVR_PATH_IDX(sCurEncryptDecryptInfo->u8PathIdx)));
    return PVRPL_CA_STATUS_OK;
}

PVRPL_CA_STATUS PVRPL_CAEncryptDecrypt(PVRPL_ENCRYPTDECRYPT_INFO *sCurEncryptDecryptInfo, MS_U32 u32Address, MS_U32 u32Length,
            PVRPL_AESDMA_Mode eMode, PVRPL_AESDMA_INFO *stAESDMAInfo, void * pParam)
{
    ST_CIPHER_R2R_START stCipherR2RData = {};
    MS_U32 u32AESPhyAddr;
    MS_U32 u32AESVirAddr;
    MS_BOOL bDescrypt = (eMode == EN_PVRPL_ENCRYPTION)?FALSE:TRUE;

    if (FALSE == sCurEncryptDecryptInfo->bEnable)//AESDMA Disable
    {
        return PVRPL_CA_STATUS_OK;
    }
    ASSERT(EN_PVRPL_ENCRYPTION_NONE < stAESDMAInfo->enEncryptType);
    ASSERT(EN_PVRPL_ENCRYPTION_AESDMA_NUM > stAESDMAInfo->enEncryptType);

#if 0//for aesdma debug using
    printf("[%s:%d]eMode : %u\n",__FUNCTION__,__LINE__,eMode);
    printf("[%s:%d]u32AESDMAKeySet[%u]\n",__FUNCTION__,__LINE__,stAESDMAInfo->enEncryptType);
    MS_U8 index=0;
    for(index=0;index<AESDMA_KEY_LENGTH;index++)
        printf(" 0x%08lX ",stAESDMAInfo->unKeySet.u32AESDMAKey[index]);
    printf("============================================================================\n");
#endif

    u32AESPhyAddr = u32Address;
    PVRPL_PA2VA(u32AESPhyAddr, &u32AESVirAddr);

    //Min. byte limitation (16X)
    if((u32Length <PVR_AES_ALIGNMENT_LEN) || (u32Length%PVR_AES_ALIGNMENT_LEN !=0) || (u32AESPhyAddr%PVR_AES_ALIGNMENT_LEN!=0))
    {
        PVRPL_CA_DBGMSG(PVRPLCA_DBG_ERR, printf("[PVR] Error: AES Write Buffer %"DTC_MS_U32_x",%"DTC_MS_U32_x" is invalid in mode %d. \n",u32AESPhyAddr,u32Length,eMode));
        u32Length = 0;
        return PVRPL_CA_STATUS_ERROR;
    }

    stCipherR2RData.bDescrypt = bDescrypt;
    stCipherR2RData.u32PhyAddr = u32AESPhyAddr;
    stCipherR2RData.u32Length = u32Length;
    switch(stAESDMAInfo->enEncryptType)
    {
#if PVR_KEYLADDER
        case EN_PVRPL_ENCRYPTION_SMARTCARD:
            stCipherR2RData.u32CipherKey[0]=0x0;
            break;
#endif
        case EN_PVRPL_ENCRYPTION_DYNAMICKEY:
            memcpy(stCipherR2RData.u32CipherKey, stKeyInfo[stAESDMAInfo->unKeySet.u32KeyIdx].u32Key, AESDMA_KEY_LENGTH);
            break;
        default:
            memcpy(stCipherR2RData.u32CipherKey, stAESDMAInfo->unKeySet.u32AESDMAKey, AESDMA_KEY_LENGTH);
            break;
    }

    _CALock();
    PVRPL_MemoryFlush(u32AESVirAddr, u32Length);
     if(TRUE != _Cipher_R2R_Start(&stCipherR2RData))
    {
        PVRPL_CA_DBGMSG(PVRPLCA_DBG_ERR, printf("\033[1;31m[%s][%d] Do cipher encrypt/decrypt with keyladder failed!!\033[m\n", __FUNCTION__, __LINE__));
        _CAUnlock();
        return PVRPL_CA_STATUS_ERROR;
    }
    PVRPL_MemoryFlush(u32AESVirAddr, u32Length);
    _CAUnlock();

    return PVRPL_CA_STATUS_OK;
}

MS_U32 PVRPL_GetCABufLength(void)
{
    return (MS_U32)AESDMA_BUF_LENGTH;
}



