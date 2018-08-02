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
#if (DEMO_AESDMA_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsMemory.h"
#include "string.h"
#include "drvDSCMB.h"
#include "demo_utility.h"
#include "drvAESDMA.h"
#include "demo_aesdma.h"

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

static ST_AESDMA_CONFIG  stDemoAESDMAConfig;

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
#define DEMO_AESDMA_DBG 0

#if DEMO_AESDMA_DBG
#define DemoAESDMA_print(fmt, args...)  { printf( fmt, ## args); }
#else
#define DemoAESDMA_print(fmt, args...)
#endif

//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------
#define DEMO_AESDMA_ALIGNMENT_LEN 16
#define DEMO_AESDMA_TIMEOUT 500  //ms

/// AESDMA speed in byte-per-ms, which is applied to estimate encrypt/decrypt time.
/// AESDMA AES ECB theoretical bit-rate is about 50M/sec
#define DEMO_AESDMA_BYTES_PER_MS    51200 // 50 * 1024 bytes

//-------------------------------------------------------------------------------------------------
// Local Function
//-------------------------------------------------------------------------------------------------
static MS_U32  _Demo_AESDMA_MEMPA2VA_FUNC(MS_U32 u32PhyAdr)
{
    MS_U32 u32VirAdr=MsOS_PA2KSEG1(u32PhyAdr);
    return u32VirAdr;
}

static void _Demo_AESDMA_MEMFLUSH_FUNC(MS_U32 u32Adr,MS_U32 u32Len)
{
    //Only need when use cache memory
    //MsOS_FlushMemory();
    //MsOS_Dcache_Flush(u32Adr,u32Len);
}

static void _Demo_AESDMA_MEMREAD_FUNC(MS_U32 u32Adr,MS_U32 u32Len)
{
    //Only need when use cache memory
    //MsOS_ReadMemory();
    //MsOS_Dcache_Invalidate(u32Adr,u32Len);
}

static void _Demo_AESDMA_AES_ECB(MS_U8 *pu8DataIn, MS_U8 *pu8DataOut, MS_U32 u32Size)
{

    ST_AESDMA_CONFIG stAESDMAInfo;

    printf("AESDMA: AES ECB test\n");


    memcpy(pu8DataIn, u8TestData, u32Size);
    memset(pu8DataOut, 0, u32Size);

    memset(&stAESDMAInfo, 0, sizeof(ST_AESDMA_CONFIG));
    // Algo
    stAESDMAInfo.eMode = E_DEMO_AESDMA_MODE_ECB;
    //Key
    stAESDMAInfo.eKeySrc = E_DEMO_AESDMA_KSRC_CPU;
    stAESDMAInfo.puACPU8Key = u8TestKey;
    stAESDMAInfo.pu8IV = NULL;
    //memory
    stAESDMAInfo.pMemFlushFunc = _Demo_AESDMA_MEMFLUSH_FUNC;
    stAESDMAInfo.pMemReadFunc = _Demo_AESDMA_MEMREAD_FUNC;
    stAESDMAInfo.pMemPA2VA = _Demo_AESDMA_MEMPA2VA_FUNC;

    Demo_AESDMA_Config(stAESDMAInfo);

    //In/Out Data and Encrypt
    Demo_AESDMA_EncryptDecrypt(FALSE, MsOS_VA2PA((MS_U32)pu8DataIn), MsOS_VA2PA((MS_U32)pu8DataOut), u32Size, 0);

    if(memcmp(pu8DataOut, u8TestDataOut_ECB, u32Size) == 0) {
        printf("AESDMA: AES ECB test success\n");
    } else {
        printf("AESDMA: AES ECB test failed\n");
    }
}

static void _Demo_AESDMA_AES_CBC(MS_U8 *pu8DataIn, MS_U8 *pu8DataOut, MS_U32 u32Size)
{
    ST_AESDMA_CONFIG stAESDMAInfo;

    printf("AESDMA: AES CBC test\n");

    memcpy(pu8DataIn, u8TestData, u32Size);
    memset(pu8DataOut, 0, u32Size);

    memset(&stAESDMAInfo, 0, sizeof(ST_AESDMA_CONFIG));
    // Algo
    stAESDMAInfo.eMode = E_DEMO_AESDMA_MODE_CBC;
    //Key
    stAESDMAInfo.eKeySrc = E_DEMO_AESDMA_KSRC_CPU;
    stAESDMAInfo.puACPU8Key = u8TestKey;
    stAESDMAInfo.pu8IV = u8TestIV;
    //memory
    stAESDMAInfo.pMemFlushFunc = _Demo_AESDMA_MEMFLUSH_FUNC;
    stAESDMAInfo.pMemReadFunc = _Demo_AESDMA_MEMREAD_FUNC;
    stAESDMAInfo.pMemPA2VA = _Demo_AESDMA_MEMPA2VA_FUNC;

    Demo_AESDMA_Config(stAESDMAInfo);

    //In/Out Data and Encrypt
    Demo_AESDMA_EncryptDecrypt(FALSE, MsOS_VA2PA((MS_U32)pu8DataIn), MsOS_VA2PA((MS_U32)pu8DataOut), u32Size, 0);

    if(memcmp(pu8DataOut, u8TestDataOut_CBC, u32Size) == 0) {
        printf("AESDMA: AES CBC test success\n");
    } else {
        printf("AESDMA: AES CBC test failed\n");
    }
}



//-------------------------------------------------------------------------------------------------
// Demo Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL Demo_AESDMA_DoAESDMADemo(void)
{
    MS_S32 NON_CACHE_POOL_ID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&NON_CACHE_POOL_ID);

    if (DRVAESDMA_OK != MDrv_AESDMA_Init(0, 0, 1)) {
        printf("Error: MDrv_AESDMA_Init() failed\n");
        return FALSE;
    }

    MS_U32 u32Size = sizeof(u8TestData);
    MS_U8 *u8Buf = MsOS_AllocateMemory(u32Size * 2, NON_CACHE_POOL_ID);
    MS_U8 *pu8DataIn = u8Buf;
    MS_U8 *pu8DataOut = u8Buf + u32Size;

    if(u8Buf == NULL) {
        printf("Error: failed\n");
        return FALSE;
    }

    memset(pu8DataIn, 0xff, u32Size);
    memset(pu8DataOut, 0xff, u32Size);
    _Demo_AESDMA_AES_ECB(pu8DataIn, pu8DataOut, u32Size);

    memset(pu8DataIn, 0xff, u32Size);
    memset(pu8DataOut, 0xff, u32Size);
    _Demo_AESDMA_AES_CBC(pu8DataIn, pu8DataOut, u32Size);


    MsOS_FreeMemory(u8Buf, NON_CACHE_POOL_ID);

    return TRUE;
}


MS_BOOL Demo_AESDMA_Config(ST_AESDMA_CONFIG stInfo)
{
    memcpy(&stDemoAESDMAConfig,&stInfo,sizeof(ST_AESDMA_CONFIG));
    return TRUE;
}


MS_BOOL Demo_AESDMA_EncryptDecrypt
(MS_BOOL bDescrypt, MS_U32 u32PhyAddressIn,
MS_U32 u32PhyAddressOut,MS_U32 u32Length,
MS_U32 u32Loop_wait_time)
{
    MS_U32 u32CurrTime = 0;
    MS_U32 u32AESInVirAddr;
    MS_U32 u32AESOutVirAddr;
    MS_U32 u32DelayTimeMS = (u32Length / DEMO_AESDMA_BYTES_PER_MS);

#if DEMO_AESDMA_DBG
    MS_U32 u32CurrTime_1 = 0;
    MS_U32 u32CurrTime_2 = 0;
    MS_U32 u32CurrTime_3 = 0;
    MS_U32 u32CurrTime_4 = 0;
    MS_U32 u32CurrTime_5 = 0;
    MS_U32 u32CurrTime_11 = 0;
    MS_U32 u32CurrTime_12 = 0;
    MS_U32 loop_cnt = 0;
#endif

    MDrv_AESDMA_Lock();

#if DEMO_AESDMA_DBG
    u32CurrTime_1 = MsOS_GetSystemTime();
#endif
    if (stDemoAESDMAConfig.eKeySrc >= E_DEMO_AESDMA_KSRC_INVALID)
    {
        printf("[%s][%d]Error Key source(%d)!",__FUNCTION__,__LINE__,stDemoAESDMAConfig.eKeySrc);
        MDrv_AESDMA_Unlock();
        return FALSE;
    }


    if (((stDemoAESDMAConfig.eKeySrc == E_DEMO_AESDMA_KSRC_CPU) && (stDemoAESDMAConfig.puACPU8Key == NULL)) ||
        ((stDemoAESDMAConfig.eKeySrc == E_DEMO_AESDMA_KSRC_KL) && (stDemoAESDMAConfig.pKLCallbackFunc == NULL)) ||
        ((stDemoAESDMAConfig.eMode == E_DEMO_AESDMA_MODE_CBC) && (stDemoAESDMAConfig.pu8IV == NULL)))
    {
        printf("[%s][%d]Error Key! [KeySrc, CPUKey, KLCallback]:[%d,%8lX,%8lX] , [Mode, IVKey]:[%d,%8lX]",__FUNCTION__,__LINE__,stDemoAESDMAConfig.eKeySrc,(MS_U32)stDemoAESDMAConfig.puACPU8Key,(MS_U32)stDemoAESDMAConfig.pKLCallbackFunc,stDemoAESDMAConfig.eMode,(MS_U32)stDemoAESDMAConfig.pu8IV);
        MDrv_AESDMA_Unlock();
        return FALSE;
    }

    //Min. byte limitation (16X)
    if((u32Length <DEMO_AESDMA_ALIGNMENT_LEN) || (u32Length%DEMO_AESDMA_ALIGNMENT_LEN !=0)
        || (u32PhyAddressIn%DEMO_AESDMA_ALIGNMENT_LEN!=0) || (u32PhyAddressOut%DEMO_AESDMA_ALIGNMENT_LEN!=0))
    {
        printf("[%s][%d]Error: AES Write Buffer (%8lX,%8lX,%8lX) is invalid in mode(%d). \n",__FUNCTION__,__LINE__,u32PhyAddressIn,u32PhyAddressOut,u32Length,bDescrypt);
        MDrv_AESDMA_Unlock();
        return FALSE;
    }

    u32AESInVirAddr = stDemoAESDMAConfig.pMemPA2VA(u32PhyAddressIn);
    u32AESOutVirAddr = stDemoAESDMAConfig.pMemPA2VA(u32PhyAddressOut);

    //Reset AES DMA
    MDrv_AESDMA_Reset();

#if DEMO_AESDMA_DBG
    u32CurrTime_11 = MsOS_GetSystemTime();
#endif

    //Key-Ladder setting
    if (stDemoAESDMAConfig.eKeySrc == E_DEMO_AESDMA_KSRC_KL)
    {
        stDemoAESDMAConfig.pKLCallbackFunc();
    }

#if DEMO_AESDMA_DBG
    u32CurrTime_11 = MsOS_GetSystemTime() - u32CurrTime_11;
    u32CurrTime_12 = MsOS_GetSystemTime();
#endif
    //memory flush
    if (stDemoAESDMAConfig.pMemFlushFunc != NULL)
    {
        stDemoAESDMAConfig.pMemFlushFunc(u32AESInVirAddr,u32Length);
    }
#if DEMO_AESDMA_DBG
    u32CurrTime_12 = MsOS_GetSystemTime() - u32CurrTime_12;
#endif
    //Set read/write buffer
     MDrv_AESDMA_SetFileInOut(u32PhyAddressIn, u32Length, u32PhyAddressOut, u32PhyAddressOut+u32Length-1);



    //Set encrypt/descrypt and key
    if (stDemoAESDMAConfig.eKeySrc == E_DEMO_AESDMA_KSRC_CPU)
    {
        MDrv_AESDMA_SetKey_Ex(stDemoAESDMAConfig.puACPU8Key, 16);
    }
    else if (stDemoAESDMAConfig.eKeySrc == E_DEMO_AESDMA_KSRC_KL)
    {
        MDrv_AESDMA_SetKey_Ex(NULL, 0);
    }
    if (stDemoAESDMAConfig.eMode == E_DEMO_AESDMA_MODE_ECB)
    {
        MDrv_AESDMA_SelEng(E_DRVAESDMA_CIPHER_ECB, bDescrypt);
    }
    else if (stDemoAESDMAConfig.eMode == E_DEMO_AESDMA_MODE_CBC)
    {
        MDrv_AESDMA_SelEng(E_DRVAESDMA_CIPHER_CBC, bDescrypt);
        MDrv_AESDMA_SetIV_Ex(stDemoAESDMAConfig.pu8IV, 16);
    }


    //Start to act
    MDrv_AESDMA_Start(TRUE);
#if DEMO_AESDMA_DBG
    u32CurrTime_2 = MsOS_GetSystemTime();
#endif
    if (u32DelayTimeMS > 0)
    {
        MsOS_DelayTask(u32DelayTimeMS);
    }
#if DEMO_AESDMA_DBG
    u32CurrTime_3 = MsOS_GetSystemTime();
#endif
    u32CurrTime = MsOS_GetSystemTime();
    while(MDrv_AESDMA_IsFinished() != DRVAESDMA_OK)
    {
        if((MsOS_GetSystemTime()-u32CurrTime)> DEMO_AESDMA_TIMEOUT)//aesdma process timeout
        {
            printf("!!!!!!!!!!!!!!!!!!!!!!!!! aesdma process timeout !!!!!!!!!!!!!!!!!!!!!!!!!\n");
            break;
        }
        MsOS_DelayTask(u32Loop_wait_time);
#if DEMO_AESDMA_DBG
        loop_cnt++;
#endif
    }

#if DEMO_AESDMA_DBG
    u32CurrTime_4 = MsOS_GetSystemTime();
#endif

    //memory invalidate
    if (stDemoAESDMAConfig.pMemReadFunc != NULL)
    {
        stDemoAESDMAConfig.pMemReadFunc(u32AESOutVirAddr,u32Length);
    }

#if DEMO_AESDMA_DBG
    u32CurrTime_5 = MsOS_GetSystemTime();
    printf("A[%lu(%lu:%lu),%lu,%lu,%lu:%lu]  ",u32CurrTime_2-u32CurrTime_1,u32CurrTime_11,u32CurrTime_12,u32CurrTime_3-u32CurrTime_2,u32CurrTime_4-u32CurrTime_3,u32CurrTime_5-u32CurrTime_4,loop_cnt);
#endif
    MDrv_AESDMA_Unlock();
    return TRUE;

}

#endif

