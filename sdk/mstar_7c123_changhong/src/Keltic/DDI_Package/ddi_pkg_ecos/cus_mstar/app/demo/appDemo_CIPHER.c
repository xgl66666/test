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

//-------------------------------------------------------------------------------------------------
///
/// @file   appDemo_AESDMA.c
/// @author MStar Semiconductor Inc.
/// @brief  AESDMA demo code
///
/// @name TV App Command Usage
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsMemory.h"
#include "string.h"
#include "drvCIPHER.h"
#include "drvDSCMB.h"

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
extern MS_S32 gs32NonCachedPoolID;

//--------------------------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------------------------------------------
static MS_U8 u8TestKey[16] = {
    0x4E, 0xEB, 0x69, 0x2B, 0x8B, 0xF9, 0x35, 0x2A, 0xB0, 0x1C, 0x57, 0xA4, 0x1A, 0x25, 0x6C, 0x87};

static MS_U8 u8TestIV[16] = {
    0x15, 0xBF, 0xD1, 0x27, 0x7B, 0x23, 0x0D, 0x05, 0x3D, 0x25, 0x87, 0xFF, 0xE5, 0x68, 0x71, 0x17};

static MS_U8 u8TestData[128] = {
    0x37, 0xD8, 0xF7, 0x7B, 0xAF, 0x82, 0x1D, 0x3E, 0xAE, 0xEC, 0x4E, 0x86, 0xB8, 0x7C, 0xEB, 0x9B,
    0x69, 0x2B, 0x0E, 0x33, 0x03, 0xFF, 0x49, 0x2F, 0xE3, 0xA8, 0x57, 0x95, 0x8C, 0xA6, 0xE7, 0x75,
    0x6D, 0x88, 0x05, 0xBF, 0x64, 0x84, 0x32, 0xB3, 0x06, 0x83, 0x44, 0x5D, 0x2E, 0x9D, 0xF6, 0x18,
    0xAD, 0xDD, 0x02, 0x2D, 0x28, 0x67, 0x8E, 0xFA, 0x2A, 0x01, 0x56, 0x0A, 0x16, 0xA1, 0x7E, 0xA5,
    0x42, 0x00, 0x3C, 0x07, 0x9C, 0x40, 0x36, 0x65, 0x23, 0xCB, 0x1A, 0x08, 0x52, 0xE0, 0x89, 0xF8,
    0xCF, 0x4B, 0x14, 0x15, 0x7F, 0x98, 0x92, 0xB4, 0xE2, 0x21, 0x54, 0xD3, 0xE4, 0xB7, 0xC3, 0x8F,
    0xA2, 0x6B, 0xC5, 0xAA, 0xE1, 0x6A, 0x8B, 0x34, 0x3D, 0x46, 0x73, 0x78, 0x4D, 0xDA, 0x61, 0x1C,
    0xB1, 0xC4, 0x5A, 0x66, 0xC8, 0x0B, 0xE9, 0x55, 0x17, 0xA9, 0x1B, 0xBD, 0x1F, 0x79, 0xED, 0xF9};

static MS_U8 u8TestDataOut_ECB[128] = {
    0x8B, 0x2E, 0xE8, 0x45, 0x5B, 0x3B, 0x14, 0xD1, 0x4C, 0x0A, 0x3F, 0x85, 0x98, 0xE6, 0xF8, 0x51,
    0x36, 0x2C, 0x0C, 0x26, 0x77, 0x41, 0x18, 0x4B, 0x6D, 0x3F, 0xFC, 0x28, 0x01, 0x35, 0xB8, 0x9A,
    0x5E, 0x43, 0x7B, 0x2D, 0x42, 0x29, 0x82, 0xBB, 0x3D, 0xB2, 0xB8, 0x9B, 0x22, 0xA3, 0xCB, 0x96,
    0x77, 0x3B, 0x74, 0x70, 0x65, 0x45, 0x96, 0x7D, 0xB7, 0x9E, 0x39, 0x9E, 0x40, 0x46, 0x32, 0x9E,
    0xBB, 0xE4, 0xB0, 0xEB, 0x79, 0x45, 0x11, 0xB3, 0x01, 0x9D, 0x86, 0x54, 0x3A, 0x34, 0x9C, 0x10,
    0x17, 0xB6, 0x79, 0x0D, 0xAB, 0xFA, 0xA6, 0xBC, 0xDD, 0xA5, 0x9B, 0x3C, 0xCF, 0x8D, 0xFE, 0xA6,
    0x11, 0x29, 0x80, 0xC7, 0x62, 0x86, 0x3A, 0x05, 0x35, 0x3C, 0x1C, 0xBC, 0x5F, 0xB1, 0xB8, 0x46,
    0x40, 0x62, 0x0A, 0xBB, 0xC2, 0xF3, 0x17, 0x67, 0x56, 0x9A, 0x11, 0x53, 0x05, 0x32, 0xB8, 0xE5};


static MS_U8 u8TestDataOut_CBC[128] = {
    0x9C, 0x1F, 0x3C, 0xDD, 0x63, 0x01, 0x9F, 0x43, 0xA2, 0x3F, 0xF4, 0x7E, 0xE3, 0x9B, 0x85, 0x42,
    0x1B, 0xB4, 0x6D, 0x8D, 0xC5, 0x58, 0xCE, 0x8C, 0x17, 0x3A, 0x58, 0x9C, 0xB2, 0xA8, 0xBD, 0x27,
    0x3A, 0x67, 0xB1, 0xA0, 0x80, 0x0A, 0xB7, 0xE7, 0x22, 0x7A, 0x17, 0x1A, 0x92, 0x12, 0xF3, 0x3A,
    0xA9, 0xAF, 0x72, 0x94, 0x3B, 0x0C, 0x37, 0xAD, 0xEA, 0xA4, 0x30, 0x5F, 0xD1, 0xAF, 0x61, 0xB1,
    0x54, 0x30, 0x75, 0xC4, 0x07, 0x3A, 0x2C, 0x63, 0xD3, 0x0F, 0x65, 0xFA, 0x16, 0x85, 0xAC, 0xB9,
    0x21, 0x87, 0x1E, 0xE5, 0x14, 0xE4, 0x44, 0x83, 0xD1, 0xD0, 0x39, 0x21, 0x9F, 0xDD, 0x60, 0x93,
    0xEB, 0x19, 0x98, 0xBF, 0x4A, 0xC7, 0x48, 0xE5, 0x31, 0x36, 0xF3, 0x4F, 0x62, 0x3E, 0x1C, 0x73,
    0xB4, 0x15, 0x25, 0x7F, 0x43, 0x4A, 0x50, 0x36, 0x87, 0xA8, 0x2E, 0x38, 0x35, 0xBB, 0x39, 0x15};

static MS_U8 u8KL_RootKey[16] = {
    0xa3, 0x05, 0xc3, 0x94, 0x4f, 0x9f, 0x83, 0x00, 0x22, 0x1c, 0x68, 0x19, 0x8e, 0xe5, 0xef, 0x9b};


// 3-Level KeyLadder input
static MS_U8 u8KL_AESDMA_Input[48] = {
    //Input Key 2
    0x48, 0xd0, 0xcf, 0xaa, 0x00, 0x23, 0x43, 0x6b, 0x09, 0xd9, 0x4b, 0xe2, 0xe3, 0x6e, 0x07, 0x2f,
    //Input Key 1
    0x95, 0x73, 0x93, 0x5f, 0x96, 0x8d, 0xec, 0xb6, 0x51, 0x85, 0xc3, 0xcd, 0xaa, 0x5b, 0x64, 0x18,
    //Input Key 0 (Encrypted Control Word)
    0xda, 0x1d, 0xe9, 0xb9, 0xff, 0xf1, 0x2b, 0x8a, 0xff, 0xa2, 0x14, 0x0d, 0xfa, 0x99, 0xfc, 0x41};


//--------------------------------------------------------------------------------------------------
// Local Defines
//--------------------------------------------------------------------------------------------------
#define CAVID 0x0F
#define SHA1_SIZE (20)

//--------------------------------------------------------------------------------------------------
// Local Functions
//--------------------------------------------------------------------------------------------------
static void _DumpBuf(const MS_U8 *buf, MS_U32 len)
{
    MS_U32 i = 0;
    for (i = 0; i < len; i++) {
        printf("0x%02x%c", buf[i], ((i % 16) == 15) ? '\n' : ' ');
    }
    printf("\n");
}


static void appDemo_CIPHER_AES_ECB(MS_U8 *pu8DataIn, MS_U8 *pu8DataOut, MS_U32 u32Size)
{
    DRV_CIPHER_RET ret = DRV_CIPHER_FAIL;
    MS_U32  u32CmdId = 0;
    MS_U32  u32Exception = 0;
    DRV_CIPHER_DMACFG stCfg = {{0}};


    printf("CIPHER: AES ECB test\n");

    memcpy(pu8DataIn, u8TestData, u32Size);
    memset(pu8DataOut, 0, u32Size);
    memset(&stCfg, 0, sizeof(stCfg));

    //CAVid
    stCfg.u32CAVid = CAVID;

    // Algo
    stCfg.stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
    stCfg.stAlgo.eSubAlgo = E_CIPHER_SUB_ECB;
    stCfg.stAlgo.eResAlgo = E_CIPHER_RES_CLR;
    stCfg.stAlgo.eSBAlgo = E_CIPHER_SB_CLR;

    //Key
    stCfg.stKey.eKeySrc = E_CIPHER_KSRC_CPU;
    stCfg.stKey.u8KeyIdx = 0;
    stCfg.stKey.u8KeyLen = sizeof(u8TestKey);
    stCfg.stKey.pu8KeyData = u8TestKey;

    //Encrypt
    stCfg.bDecrypt = FALSE;

    //InData
    stCfg.stInput.u32Addr = MsOS_VA2PA((MS_U32)pu8DataIn);
    stCfg.stInput.u32Size = u32Size;

    //OutData
    stCfg.stOutput.u32Addr = MsOS_VA2PA((MS_U32)pu8DataOut);
    stCfg.stOutput.u32Size = u32Size;

    MsOS_FlushMemory();

    ret = MDrv_CIPHER_DMACalc(stCfg, &u32CmdId);

    if(ret != DRV_CIPHER_OK) {printf("DMA FAIL!!!\n"); return;}

    while(FALSE == MDrv_CIPHER_IsDMADone(u32CmdId,&u32Exception))
    {
        MsOS_DelayTask(50);
    }

    if(u32Exception != 0){printf("Exception[0x%08x]!!!\n",(unsigned int)u32Exception);}

    MsOS_ReadMemory();

    if(memcmp(pu8DataOut, u8TestDataOut_ECB, u32Size) == 0) {
        printf("CIPHER: AES ECB test success\n\n");
    } else {
        printf("CIPHER: AES ECB test failed\n\n");
        _DumpBuf(pu8DataOut, u32Size);
    }
}




//--------------------------------------------------------------------------------------------------
// @brief AESDMA Test
// @param[in] None.
// @return TRUE is successful, otherwise FALSE
//--------------------------------------------------------------------------------------------------
static void appDemo_AESDMA_AES_ECB(MS_U8 *pu8DataIn, MS_U8 *pu8DataOut, MS_U32 u32Size)
{
    printf("AESDMA: AES ECB test\n");

    MDrv_AESDMA_Reset();

    memcpy(pu8DataIn, u8TestData, u32Size);
    memset(pu8DataOut, 0, u32Size);

    MDrv_AESDMA_SelEng(E_DRVAESDMA_CIPHER_ECB, FALSE); // AES ECB Encrypt
    MDrv_AESDMA_SetKey_Ex(u8TestKey, 16);

    MDrv_AESDMA_SetFileInOut(
            MsOS_VA2PA((MS_U32)pu8DataIn), u32Size,
            MsOS_VA2PA((MS_U32)pu8DataOut),
            MsOS_VA2PA((MS_U32)pu8DataOut + u32Size - 1));

    MsOS_FlushMemory();

    MDrv_AESDMA_Start(TRUE);
    while(MDrv_AESDMA_IsFinished() != DRV_CIPHER_OK)
        ;

    MsOS_ReadMemory();

    if(memcmp(pu8DataOut, u8TestDataOut_ECB, u32Size) == 0) {
        printf("AESDMA: AES ECB test success\n\n");
    } else {
        printf("AESDMA: AES ECB test failed\n\n");
    }
}

static void appDemo_AESDMA_AES_CBC(MS_U8 *pu8DataIn, MS_U8 *pu8DataOut, MS_U32 u32Size)
{
    printf("AESDMA: AES CBC test\n");

    MDrv_AESDMA_Reset();

    memcpy(pu8DataIn, u8TestData, u32Size);
    memset(pu8DataOut, 0, u32Size);

    MDrv_AESDMA_SelEng(E_DRVAESDMA_CIPHER_CBC, FALSE); // AES CBC Encrypt
    MDrv_AESDMA_SetKey_Ex(u8TestKey, 16);
    MDrv_AESDMA_SetIV_Ex(u8TestIV, 16);

    MDrv_AESDMA_SetFileInOut(
            MsOS_VA2PA((MS_U32)pu8DataIn), u32Size,
            MsOS_VA2PA((MS_U32)pu8DataOut),
            MsOS_VA2PA((MS_U32)pu8DataOut + u32Size - 1));

    MsOS_FlushMemory();

    MDrv_AESDMA_Start(TRUE);
    while(MDrv_AESDMA_IsFinished() != DRVAESDMA_OK)
        ;

    MsOS_ReadMemory();

    if(memcmp(pu8DataOut, u8TestDataOut_CBC, u32Size) == 0) {
        printf("AESDMA: AES CBC test success\n\n");
    } else {
        printf("AESDMA: AES CBC test failed\n\n");
    }
}


static void appDemo_CIPHER_AES_CBC_with_KeyLadder(MS_U8 *pu8DataIn, MS_U8 *pu8DataOut, MS_U32 u32Size)
{
    printf("AESDMA: AES CBC test with KeyLadder\n");

    DSCMB_KL_Status KL_Status = 0;
    MS_U8 ACPU_Out[16] = {0x00};
    DRV_CIPHER_DMACFG stCfg = {{0}};
    MS_U32 u32CmdId = 0;
    MS_U32 u32Exception = 0;

    MS_BOOL ret = FALSE;

    DSCMB_KLCfg_All KLConfigAll = {
        .eAlgo = E_DSCMB_KL_AES,
        .eSrc = E_DSCMB_KL_SRC_ACPU,
        .eDst = E_DSCMB_KL_DST_DMA_SK0,
        .eOutsize = E_DSCMB_KL_128_BITS,
        .eKeyType = 0,                      // Don't care when dst is Crypto DMA
        .u32Level = 3,
        .u32EngID = 0,                      // Don't care when dst is Crypto DMA
        .u32DscID = 0,                      // Don't care when dst is Crypto DMA
        .u8KeyACPU = u8KL_RootKey,
        .pu8KeyKLIn = u8KL_AESDMA_Input,
        .bDecrypt = TRUE,
        .bInverse = FALSE,
        .u32CAVid = CAVID,                 //CAVid for output key, the same of DMA
    };

    // Key Ladder
    ret = MDrv_DSCMB_KLadder_AtomicExec(&KLConfigAll , ACPU_Out, &KL_Status );
    if(ret == FALSE)
    {
        printf("Key Ladder: Fail!!! 0x%x\n",(unsigned int)KL_Status);
        return;
    }

    if(KLConfigAll.eDst == E_DSCMB_KL_DST_ACPU)
    {
        _DumpBuf(ACPU_Out,16);
    }

    // CryptoDMA
    memcpy(pu8DataIn, u8TestData, u32Size);
    memset(pu8DataOut, 0, u32Size);
    memset(&stCfg, 0, sizeof(stCfg));

    //CAVid
    stCfg.u32CAVid = CAVID;

    // Algo
    stCfg.stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
    stCfg.stAlgo.eSubAlgo = E_CIPHER_SUB_CBC;
    stCfg.stAlgo.eResAlgo = E_CIPHER_RES_CLR;
    stCfg.stAlgo.eSBAlgo = E_CIPHER_SB_CLR;

    //Key
    stCfg.stKey.eKeySrc = E_CIPHER_KSRC_KL;
    stCfg.stKey.u8KeyIdx = 0;
    stCfg.stKey.u8KeyLen = 0;
    stCfg.stKey.pu8KeyData = NULL;

    //IV
    stCfg.stKey.pu8IVData = u8TestIV;
    stCfg.stKey.u8IVLen = 16;

    //Encrypt
    stCfg.bDecrypt = FALSE;

    //InData
    stCfg.stInput.u32Addr = MsOS_VA2PA((MS_U32)pu8DataIn);
    stCfg.stInput.u32Size = u32Size;

    //OutData
    stCfg.stOutput.u32Addr = MsOS_VA2PA((MS_U32)pu8DataOut);
    stCfg.stOutput.u32Size = u32Size;

    MsOS_FlushMemory();

    ret = MDrv_CIPHER_DMACalc(stCfg, &u32CmdId);

    if(ret != DRV_CIPHER_OK) {printf("DMA FAIL!!!\n"); return;}

    while(FALSE == MDrv_CIPHER_IsDMADone(u32CmdId,&u32Exception))
    {
        MsOS_DelayTask(50);
    }

    if(u32Exception != 0){printf("Exception[0x%08x]!!!\n",(unsigned int)u32Exception);}

    MsOS_ReadMemory();

    if(memcmp(pu8DataOut, u8TestDataOut_CBC, u32Size) == 0)
    {
        printf("AESDMA: AES CBC test with KeyLadder success\n\n");
    }
    else
    {
        printf("AESDMA: AES CBC test with KeyLadder failed, KL Status = 0x%x\n\n",(unsigned int)KL_Status);
        _DumpBuf(pu8DataOut,128);
    }

}


//////////////////////////////////////////////////////////////////////////
static MS_U8 msg0[64]  = {0x1c,0x76,0x30,0x95,0x25,0x70,0xf0,0x61,0xf3,0xc3,0xe0,0x2a,0x02,0x09,0x7f,0x3b,0xc0,0x49,0xa1,0x02,0x6b,0x68,0x76,0x77,0x35,0xce,0xd1,0xc4,0xe0,0x0d,0xea,0xfd,0x84,0x1a,0x92,0xa9,0x8a,0x82,0x0a,0x7e,0x45,0xea,0xa8,0x47,0xf4,0x27,0x82,0xb4,0x70,0x23,0xb7,0xdb,0x8b,0x2d,0x53,0xc0,0xfb,0x24,0x84,0xdc,0x32,0x6e,0xd9,0xb6};
static MS_U8 msg1[64]  = {0x89,0x6b,0x5f,0x13,0xed,0x6a,0x91,0x32,0x54,0x39,0x79,0x48,0x60,0xfb,0xfd,0xd0,0x1f,0xb4,0xac,0xaa,0xe1,0xff,0x6b,0xdd,0x23,0xef,0xb9,0x55,0x5e,0x92,0x0b,0xe7,0xfd,0x6b,0xfa,0xea,0xd5,0x8c,0x1c,0x29,0xc5,0x95,0x72,0x26,0x90,0x6f,0xf6,0xaf,0x23,0xa2,0x5a,0x04,0xa1,0xc5,0xe1,0xc5,0xb4,0x9a,0x1a,0x12,0x2c,0x26,0xf9,0x29};
static MS_U8 msg2[60]  = {0x91,0xf4,0x13,0x66,0x80,0x2f,0x8f,0x45,0xc4,0x01,0x6b,0x55,0x70,0x62,0x04,0x93,0x04,0x5e,0x98,0xa6,0x23,0x79,0x6b,0xd8,0x14,0x85,0xea,0x40,0xab,0xe4,0x6a,0x3c,0xd8,0x7d,0xa2,0x58,0xad,0x32,0x9d,0x71,0x33,0x09,0xc6,0xa4,0x6b,0xcb,0x37,0x6f,0x29,0xcf,0x15,0x4d,0x49,0x80,0x25,0x5d,0x06,0x0f,0x9d,0xb1};

MS_U8 SHA1_Golden_MSB[20] = {0x1a, 0x4e, 0x03, 0x01, 0x92, 0xe0, 0xb5, 0x2a, 0x2e, 0xf7, 0x3e, 0xc5, 0x49, 0x0d, 0x50, 0xe5,0x6e, 0xab, 0x0e, 0xa6};
MS_U8 SHA1_Golden_LSB[20] = {0xa6, 0x0e, 0xab, 0x6e, 0xe5, 0x50, 0x0d, 0x49, 0xc5, 0x3e, 0xf7, 0x2e, 0x2a, 0xb5, 0xe0, 0x92,0x01, 0x03, 0x4e, 0x1a};

void appDemo_CIPHER_HASH_SHA1(MS_U8 *pu8MsgIn, MS_U8 *pu8Digest_Buffer)
{
    DRV_CIPHER_HASHCFG stcfg = {0};
    DRV_CIPHER_RET ret = DRV_CIPHER_FAIL;
    MS_U32 u32Exception = 0;
    MS_U32 u32CmdId = 0;

    memset(&stcfg, 0, sizeof(stcfg));

    memcpy(pu8MsgIn       ,msg0, 64);
    memcpy(pu8MsgIn+64    ,msg1, 64);
    memcpy(pu8MsgIn+64+64 ,msg2, 60);

    stcfg.u32CAVid = CAVID;
    stcfg.eAlgo = E_CIPHER_HASH_ALGO_SHA1;
    stcfg.stInput.u32Addr = MsOS_VA2PA((MS_U32)pu8MsgIn);
    stcfg.stInput.u32Size = 188;
    stcfg.pu8Digest_Buf = pu8Digest_Buffer;
    stcfg.u32Digest_Buf_Size = SHA1_SIZE;

    ret = MDrv_CIPHER_HASH(stcfg, &u32CmdId);

    if(ret != 0) {printf("HASH FAIL!!!\n"); return;}

    while(FALSE == MDrv_CIPHER_IsHASHDone(u32CmdId,&u32Exception))
    {
        MsOS_DelayTask(20);
    }

    if(memcmp(SHA1_Golden_MSB, pu8Digest_Buffer, SHA1_SIZE) == 0) {
        printf("CIPHER: SHA1 test success\n\n");
    } else {
        _DumpBuf(pu8Digest_Buffer, SHA1_SIZE);
        printf("CIPHER: SHA1 test failed\n\n");
    }

}

void appDemo_CIPHER_HASH_Manual_SHA1(MS_U8 *pu8MsgIn, MS_U8 *pu8Digest_Buffer)
{
    DRV_CIPHER_HASHCFG_MANUAL first_cfg  = {0};
    DRV_CIPHER_HASHCFG_MANUAL update_cfg = {0};
    DRV_CIPHER_HASHCFG_MANUAL last_cfg   = {0};
    MS_U32 u32CmdId = 0;
    MS_U32 u32Exception = 0;

    DRV_CIPHER_RET ret = DRV_CIPHER_FAIL;

    //NOTE: !!!!! Hash input message must be 16bytes alignment  !!!!!!!
    //!!!!First stage and Update stage block size must be  64 bytes alignment!!!!
    MS_U8 *u8TmpIV  = (MS_U8*)MsOS_AllocateMemory(SHA1_SIZE, gs32NonCachedPoolID);
    MS_U8 *u8PrevIV = (MS_U8*)MsOS_AllocateMemory(SHA1_SIZE, gs32NonCachedPoolID);

    memcpy(pu8MsgIn,msg0,sizeof(msg0));
    memset(u8TmpIV, 0, sizeof(SHA1_SIZE));
    memset(u8PrevIV, 0, sizeof(SHA1_SIZE));
    memset(&first_cfg, 0, sizeof(first_cfg));
    memset(&update_cfg, 0, sizeof(update_cfg));
    memset(&last_cfg, 0, sizeof(last_cfg));

    first_cfg.u32CAVid = CAVID;

    first_cfg.eAlgo = E_CIPHER_HASH_ALGO_SHA1;
    first_cfg.stInput.u32Addr = MsOS_VA2PA((MS_U32)pu8MsgIn);
    first_cfg.stInput.u32Size = sizeof(msg0);
    first_cfg.pu8Digest_Buf = u8TmpIV;
    first_cfg.u32Digest_Buf_Size = SHA1_SIZE;

    first_cfg.eStage = E_CIPHER_HASH_STAGE_FIRST;
    first_cfg.u32StartByte = 0;
    first_cfg.pu8SetIV = NULL;
    first_cfg.eIVMode = E_CIPHER_HASH_IV_FIPS;

    ret = MDrv_CIPHER_HASHManual(first_cfg, &u32CmdId);

    if(ret != 0) {printf("HASH MANUAL FAIL!!!\n"); return;}

    while(FALSE == MDrv_CIPHER_IsHASHDone(u32CmdId,&u32Exception))
    {
        MsOS_DelayTask(20);
    }

    memcpy(pu8MsgIn,msg1,sizeof(msg1));
    memcpy(u8PrevIV, u8TmpIV, SHA1_SIZE);
    memset(u8TmpIV, 0, SHA1_SIZE);

    update_cfg.u32CAVid = CAVID;

    update_cfg.eAlgo = E_CIPHER_HASH_ALGO_SHA1;
    update_cfg.stInput.u32Addr = MsOS_VA2PA((MS_U32)pu8MsgIn);
    update_cfg.stInput.u32Size = sizeof(msg1);
    update_cfg.pu8Digest_Buf = u8TmpIV;
    update_cfg.u32Digest_Buf_Size = SHA1_SIZE;

    update_cfg.eStage = E_CIPHER_HASH_STAGE_UPDATE;
    update_cfg.u32StartByte = sizeof(msg0);
    update_cfg.pu8SetIV = u8PrevIV;
    update_cfg.eIVMode = E_CIPHER_HASH_IV_CMD;

    ret = MDrv_CIPHER_HASHManual(update_cfg, &u32CmdId);

    if(ret != 0) {printf("HASH MANUAL FAIL!!!\n"); return;}

    while(FALSE == MDrv_CIPHER_IsHASHDone(u32CmdId,&u32Exception))
    {
        MsOS_DelayTask(20);
    }

    memcpy(pu8MsgIn,msg2,sizeof(msg2));
    memcpy(u8PrevIV, u8TmpIV, SHA1_SIZE);
    memset(u8TmpIV, 0, SHA1_SIZE);

    last_cfg.u32CAVid = CAVID;

    last_cfg.eAlgo = E_CIPHER_HASH_ALGO_SHA1;
    last_cfg.stInput.u32Addr = MsOS_VA2PA((MS_U32)pu8MsgIn);
    last_cfg.stInput.u32Size = sizeof(msg2);
    last_cfg.pu8Digest_Buf = pu8Digest_Buffer;
    last_cfg.u32Digest_Buf_Size = SHA1_SIZE;

    last_cfg.eStage = E_CIPHER_HASH_STAGE_LAST;
    last_cfg.u32StartByte = sizeof(msg0) + sizeof(msg1);
    last_cfg.pu8SetIV = u8PrevIV;
    last_cfg.eIVMode = E_CIPHER_HASH_IV_CMD;

    ret = MDrv_CIPHER_HASHManual(last_cfg, &u32CmdId);

    if(ret != 0) {printf("HASH MANUAL FAIL!!!\n"); return;}

    while(FALSE == MDrv_CIPHER_IsHASHDone(u32CmdId,&u32Exception))
    {
        MsOS_DelayTask(20);
    }

    if(memcmp(SHA1_Golden_MSB, pu8Digest_Buffer, SHA1_SIZE) == 0) {
        printf("CIPHER: SHA1 Manual test success\n\n");
    } else {
        _DumpBuf(pu8Digest_Buffer, SHA1_SIZE);
        printf("CIPHER: SHA1 Manual test failed\n\n");
    }

    MsOS_FreeMemory(u8TmpIV, gs32NonCachedPoolID);
    MsOS_FreeMemory(u8PrevIV, gs32NonCachedPoolID);
}


void appDemo_AESDMA_HASH_SHA1(MS_U8 *pu8MsgIn, MS_U8 *pu8Digest_Buffer)
{
    DRVAESDMA_RESULT ret = DRVAESDMA_FAIL;
    DrvAESDMA_SHAMode eMode = E_DRVAESDMA_SHA1;

    memcpy(pu8MsgIn       ,msg0, 64);
    memcpy(pu8MsgIn+64    ,msg1, 64);
    memcpy(pu8MsgIn+64+64 ,msg2, 60);

    eMode = E_DRVAESDMA_SHA1;

    ret = MDrv_SHA_Calculate(eMode,MsOS_VA2PA((MS_U32)pu8MsgIn), 188, MsOS_VA2PA((MS_U32)pu8Digest_Buffer));
    if(ret != 0) {printf("HASH FAIL!!!\n"); return;}

    if(memcmp(SHA1_Golden_LSB, pu8Digest_Buffer, SHA1_SIZE) == 0) {
        printf("AESDMA: SHA1 Auto test success\n\n");
    } else {
        _DumpBuf(pu8Digest_Buffer, SHA1_SIZE);
        printf("AESDMA: SHA1 Auto test failed\n\n");
    }

}

void appDemo_AESDMA_HASH_Manual_SHA1(MS_U8 *pu8MsgIn, MS_U8 *pu8Digest_Buffer)
{
    DrvAESDMA_HASHCFG first_cfg = {0};
    DrvAESDMA_HASHCFG update_cfg = {0};
    DrvAESDMA_HASHCFG last_cfg = {0};

    DRVAESDMA_RESULT ret = DRVAESDMA_FAIL;

    //NOTE: !!!!! Hash input message must be 16bytes alignment  !!!!!!!
    MS_U8 *u8TmpIV =        (MS_U8*)MsOS_AllocateMemory(SHA1_SIZE, gs32NonCachedPoolID);
    MS_U8 *u8PrevIV =       (MS_U8*)MsOS_AllocateMemory(SHA1_SIZE, gs32NonCachedPoolID);

    memcpy(pu8MsgIn,msg0, sizeof(msg0));
    memset(pu8Digest_Buffer, 0, SHA1_SIZE);
    memset(&first_cfg, 0, sizeof(first_cfg));
    memset(&update_cfg, 0, sizeof(update_cfg));
    memset(&last_cfg, 0, sizeof(last_cfg));

    first_cfg.eMode = E_DRVAESDMA_SHA1;
    first_cfg.stInput.u32Addr = (MS_U32)pu8MsgIn;
    first_cfg.stInput.u32Size = sizeof(msg0);
    first_cfg.stOutput.u32Addr = (MS_U32)u8TmpIV;
    first_cfg.stOutput.u32Size = SHA1_SIZE;

    ret = MDrv_SHA_CalculateManual(first_cfg,E_DRVAESDMA_HASH_STAGE_FIRST,0,NULL);

    memcpy(pu8MsgIn,msg1,sizeof(msg1));
    memcpy(u8PrevIV, u8TmpIV, SHA1_SIZE);
    memset(u8TmpIV, 0, SHA1_SIZE);

    update_cfg.eMode = E_CIPHER_HASH_ALGO_SHA1;
    update_cfg.stInput.u32Addr = (MS_U32)pu8MsgIn;
    update_cfg.stInput.u32Size = sizeof(msg1);
    update_cfg.stOutput.u32Addr = (MS_U32)u8TmpIV;
    update_cfg.stOutput.u32Size = SHA1_SIZE;

    ret = MDrv_SHA_CalculateManual(update_cfg,E_DRVAESDMA_HASH_STAGE_UPDATE,sizeof(msg0),u8PrevIV);

    memcpy(pu8MsgIn,msg2,sizeof(msg2));
    memcpy(u8PrevIV, u8TmpIV, SHA1_SIZE);
    memset(u8TmpIV, 0, SHA1_SIZE);

    last_cfg.eMode = E_CIPHER_HASH_ALGO_SHA1;
    last_cfg.stInput.u32Addr = (MS_U32)pu8MsgIn;
    last_cfg.stInput.u32Size = sizeof(msg2);
    last_cfg.stOutput.u32Addr = (MS_U32)pu8Digest_Buffer;
    last_cfg.stOutput.u32Size = SHA1_SIZE;

    ret = MDrv_SHA_CalculateManual(last_cfg,E_DRVAESDMA_HASH_STAGE_LAST,(sizeof(msg0) + sizeof(msg1)),u8PrevIV);

    if(ret != 0) {printf("HASH FAIL!!!\n"); return;}

    if(memcmp(SHA1_Golden_LSB, pu8Digest_Buffer, SHA1_SIZE) == 0) {
        printf("AESDMA: SHA1 Manual test success\n\n");
    } else {
        _DumpBuf(pu8Digest_Buffer, SHA1_SIZE);
        printf("AESDMA: SHA1 Manual test failed\n\n");
    }

    MsOS_FreeMemory(u8TmpIV, gs32NonCachedPoolID);
    MsOS_FreeMemory(u8PrevIV, gs32NonCachedPoolID);
}


MS_BOOL appDemo_CIPHER_DoCIPHERDemo(void)
{
    // init with default CAVId 0x0F
    if (DRV_CIPHER_OK != MDrv_CIPHER_Init()) {
        printf("Error: MDrv_Cipher_Init() failed\n");
        return FALSE;
    }

    //DMA Test Buffer
    MS_U32 u32Size = sizeof(u8TestData);
    MS_U8 *u8Buf   = MsOS_AllocateMemory(u32Size * 2, gs32NonCachedPoolID);
    MS_U8 *pu8DataIn = u8Buf;
    MS_U8 *pu8DataOut = u8Buf + u32Size;

    //HASH Test Buffer
    //NOTE: !!!!! Hash input message must be 16bytes alignment  !!!!!!!
    MS_U32 u32MsgSize = sizeof(msg0) + sizeof(msg1) + sizeof(msg2);
    MS_U8 *pu8MsgIn   = u8Buf;
    MS_U8 *pu8Digest_Buffer = u8Buf + u32MsgSize;

    if(u8Buf == NULL) {
        printf("Error: failed\n");
        return FALSE;
    }

    memset(pu8DataIn, 0xff, u32Size);
    memset(pu8DataOut, 0xff, u32Size);
    appDemo_CIPHER_AES_ECB(pu8DataIn, pu8DataOut, u32Size);

    memset(pu8DataIn, 0xff, u32Size);
    memset(pu8DataOut, 0xff, u32Size);
    appDemo_CIPHER_AES_CBC_with_KeyLadder(pu8DataIn, pu8DataOut, u32Size);

    memset(pu8MsgIn, 0xff, u32MsgSize);
    memset(pu8Digest_Buffer, 0xff, SHA1_SIZE);
    appDemo_CIPHER_HASH_SHA1(pu8MsgIn, pu8Digest_Buffer);

    memset(pu8MsgIn, 0xff, u32MsgSize);
    memset(pu8Digest_Buffer, 0xff, SHA1_SIZE);
    appDemo_CIPHER_HASH_Manual_SHA1(pu8MsgIn, pu8Digest_Buffer);

    printf("\n=======Old API Compatible Test Start==========\n");
    memset(pu8DataIn, 0xff, u32Size);
    memset(pu8DataOut, 0xff, u32Size);
    appDemo_AESDMA_AES_ECB(pu8DataIn, pu8DataOut, u32Size);

    memset(pu8DataIn, 0xff, u32Size);
    memset(pu8DataOut, 0xff, u32Size);
    appDemo_AESDMA_AES_CBC(pu8DataIn, pu8DataOut, u32Size);

    memset(pu8MsgIn, 0xff, u32MsgSize);
    memset(pu8Digest_Buffer, 0xff, SHA1_SIZE);
    appDemo_AESDMA_HASH_SHA1(pu8MsgIn, pu8Digest_Buffer);

    memset(pu8MsgIn, 0xff, u32MsgSize);
    memset(pu8Digest_Buffer, 0xff, SHA1_SIZE);
    appDemo_AESDMA_HASH_Manual_SHA1(pu8MsgIn, pu8Digest_Buffer);
    printf("\n=======Old API Compatible Test Finish==========\n");

    //Free memory
    MsOS_FreeMemory(u8Buf, gs32NonCachedPoolID);

    return TRUE;
}

