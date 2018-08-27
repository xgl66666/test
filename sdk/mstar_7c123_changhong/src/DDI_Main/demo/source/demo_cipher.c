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
#if (DEMO_CIPHER_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsMemory.h"
#include "string.h"
#include "drvCIPHER.h"
#include "drvDSCMB.h"
#include "demo_utility.h"
#include "demo_cipher.h"
#include "demo_keyladder.h"

#define DDI_DEMO_AESDMA_UTPA2 0

#if DDI_DEMO_AESDMA_UTPA2
#include "utopia.h"
static void *pu32CipherDevice = NULL;
#endif

//--------------------------------------------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------------------------------------------
static MS_U8 u8TestKey_MSB[16] = {
    0x4E, 0xEB, 0x69, 0x2B, 0x8B, 0xF9, 0x35, 0x2A, 0xB0, 0x1C, 0x57, 0xA4, 0x1A, 0x25, 0x6C, 0x87};

static MS_U8 u8TestIV_MSB[16] = {
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

static MS_U8 u8TestDataOut_ECB_MSB_Key[128] = {
    0x8B, 0x2E, 0xE8, 0x45, 0x5B, 0x3B, 0x14, 0xD1, 0x4C, 0x0A, 0x3F, 0x85, 0x98, 0xE6, 0xF8, 0x51,
    0x36, 0x2C, 0x0C, 0x26, 0x77, 0x41, 0x18, 0x4B, 0x6D, 0x3F, 0xFC, 0x28, 0x01, 0x35, 0xB8, 0x9A,
    0x5E, 0x43, 0x7B, 0x2D, 0x42, 0x29, 0x82, 0xBB, 0x3D, 0xB2, 0xB8, 0x9B, 0x22, 0xA3, 0xCB, 0x96,
    0x77, 0x3B, 0x74, 0x70, 0x65, 0x45, 0x96, 0x7D, 0xB7, 0x9E, 0x39, 0x9E, 0x40, 0x46, 0x32, 0x9E,
    0xBB, 0xE4, 0xB0, 0xEB, 0x79, 0x45, 0x11, 0xB3, 0x01, 0x9D, 0x86, 0x54, 0x3A, 0x34, 0x9C, 0x10,
    0x17, 0xB6, 0x79, 0x0D, 0xAB, 0xFA, 0xA6, 0xBC, 0xDD, 0xA5, 0x9B, 0x3C, 0xCF, 0x8D, 0xFE, 0xA6,
    0x11, 0x29, 0x80, 0xC7, 0x62, 0x86, 0x3A, 0x05, 0x35, 0x3C, 0x1C, 0xBC, 0x5F, 0xB1, 0xB8, 0x46,
    0x40, 0x62, 0x0A, 0xBB, 0xC2, 0xF3, 0x17, 0x67, 0x56, 0x9A, 0x11, 0x53, 0x05, 0x32, 0xB8, 0xE5};

static MS_U8 u8TestDataOut_ECB_LSB_Key[128] = {
    0x0c, 0xa3, 0xd1, 0xb4, 0x0c, 0xbf, 0x51, 0xd3, 0x97, 0x9a, 0x7e, 0x8e, 0x91, 0x76, 0x88, 0x47,
    0xad, 0x9f, 0x54, 0x0c, 0x91, 0x4f, 0x9f, 0x62, 0xd3, 0xbc, 0x51, 0xf6, 0x41, 0xc2, 0xc0, 0xab,
    0xf9, 0x10, 0x4b, 0x5e, 0x8c, 0x29, 0x79, 0x30, 0xa5, 0x7b, 0x1d, 0x27, 0x09, 0x64, 0x73, 0xb8,
    0xec, 0x66, 0xa2, 0xb9, 0x09, 0xfa, 0xba, 0x4e, 0x21, 0xa3, 0x2b, 0xc8, 0xcb, 0x66, 0x04, 0x7a,
    0x37, 0xf6, 0xdf, 0x7d, 0xb3, 0x74, 0x1f, 0x2b, 0xe6, 0xdc, 0x8b, 0x92, 0x06, 0xb5, 0x56, 0xb4,
    0xb0, 0x75, 0x70, 0x77, 0x77, 0xd0, 0xf5, 0x9a, 0x0e, 0xd2, 0x79, 0xa8, 0xba, 0x91, 0xff, 0xe0,
    0x3c, 0xac, 0xc6, 0x8f, 0xa6, 0x3f, 0x16, 0x94, 0xde, 0x31, 0x6d, 0xee, 0x91, 0x14, 0x36, 0x92,
    0x23, 0x82, 0x85, 0x72, 0xd9, 0xb7, 0x5f, 0x4a, 0x43, 0x39, 0x72, 0x4c, 0x22, 0x73, 0xb9, 0xd3};

static MS_U8 u8TestDataOut_CBC_MSB_Key[128] = {
    0x9C, 0x1F, 0x3C, 0xDD, 0x63, 0x01, 0x9F, 0x43, 0xA2, 0x3F, 0xF4, 0x7E, 0xE3, 0x9B, 0x85, 0x42,
    0x1B, 0xB4, 0x6D, 0x8D, 0xC5, 0x58, 0xCE, 0x8C, 0x17, 0x3A, 0x58, 0x9C, 0xB2, 0xA8, 0xBD, 0x27,
    0x3A, 0x67, 0xB1, 0xA0, 0x80, 0x0A, 0xB7, 0xE7, 0x22, 0x7A, 0x17, 0x1A, 0x92, 0x12, 0xF3, 0x3A,
    0xA9, 0xAF, 0x72, 0x94, 0x3B, 0x0C, 0x37, 0xAD, 0xEA, 0xA4, 0x30, 0x5F, 0xD1, 0xAF, 0x61, 0xB1,
    0x54, 0x30, 0x75, 0xC4, 0x07, 0x3A, 0x2C, 0x63, 0xD3, 0x0F, 0x65, 0xFA, 0x16, 0x85, 0xAC, 0xB9,
    0x21, 0x87, 0x1E, 0xE5, 0x14, 0xE4, 0x44, 0x83, 0xD1, 0xD0, 0x39, 0x21, 0x9F, 0xDD, 0x60, 0x93,
    0xEB, 0x19, 0x98, 0xBF, 0x4A, 0xC7, 0x48, 0xE5, 0x31, 0x36, 0xF3, 0x4F, 0x62, 0x3E, 0x1C, 0x73,
    0xB4, 0x15, 0x25, 0x7F, 0x43, 0x4A, 0x50, 0x36, 0x87, 0xA8, 0x2E, 0x38, 0x35, 0xBB, 0x39, 0x15};

static MS_U8 u8TestDataOut_CBC_LSB_Key[128] = {
    0xc2, 0xa8, 0x50, 0x0c, 0x50, 0xd1, 0xb0, 0x71, 0xe4, 0x1d, 0xe2, 0xbd, 0xae, 0xd5, 0x8e, 0xfc,
    0x4b, 0x81, 0xa7, 0x55, 0xd7, 0x5e, 0xad, 0x4d, 0xd7, 0x80, 0x12, 0x9b, 0x01, 0x44, 0xe5, 0x7c,
    0x29, 0x1b, 0x06, 0x27, 0xb1, 0x03, 0x94, 0xf3, 0xf7, 0xbd, 0x40, 0x42, 0xf7, 0xe5, 0x09, 0x6b,
    0xf9, 0x3e, 0xbc, 0x2a, 0x6e, 0x3d, 0x79, 0x42, 0x82, 0x10, 0xf9, 0xcb, 0x5b, 0xec, 0x1e, 0xd5,
    0xaf, 0x9f, 0xc7, 0xf6, 0x72, 0x25, 0xcc, 0x48, 0xf4, 0xf6, 0x1e, 0x95, 0x68, 0xe0, 0x65, 0xcf,
    0xa3, 0x72, 0x2c, 0xf0, 0xe5, 0xbf, 0x77, 0xac, 0x26, 0x21, 0x9d, 0x72, 0x97, 0x72, 0x0a, 0x7d,
    0x9b, 0xb6, 0xd6, 0x6e, 0x67, 0x89, 0x09, 0x2a, 0xd3, 0xfc, 0x10, 0x27, 0xdd, 0xc3, 0x8f, 0x29,
    0xd2, 0x48, 0xa9, 0x83, 0x25, 0xcb, 0x21, 0x92, 0x5e, 0x77, 0x7e, 0x28, 0x9f, 0x36, 0xa8, 0x6b};

static MS_U8 u8KL_RootKey_MSB[16] = {
    0xa3, 0x05, 0xc3, 0x94, 0x4f, 0x9f, 0x83, 0x00, 0x22, 0x1c, 0x68, 0x19, 0x8e, 0xe5, 0xef, 0x9b};

// 3-Level KeyLadder input
static MS_U8 u8KL_AESDMA_Input_MSB[48] = {
    //Input Key 2 MSB
    0x48, 0xd0, 0xcf, 0xaa, 0x00, 0x23, 0x43, 0x6b, 0x09, 0xd9, 0x4b, 0xe2, 0xe3, 0x6e, 0x07, 0x2f,
    //Input Key 1 MSB
    0x95, 0x73, 0x93, 0x5f, 0x96, 0x8d, 0xec, 0xb6, 0x51, 0x85, 0xc3, 0xcd, 0xaa, 0x5b, 0x64, 0x18,
    //Input Key 0 (Encrypted Control Word) MSB
    0xda, 0x1d, 0xe9, 0xb9, 0xff, 0xf1, 0x2b, 0x8a, 0xff, 0xa2, 0x14, 0x0d, 0xfa, 0x99, 0xfc, 0x41};

static ST_DEMO_CIPHER_CONFIG  stDemoCIPHERConfig;

// For each of the 256 possible bit values, how many 1 bits are set
MS_U8 bits_table[256] = {
  0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3,
  4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4,
  4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2,
  3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5,
  4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3,
  4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3,
  3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5,
  6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6,
  4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5,
  6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
#define DEMO_CIPHER_DBG 0

#if DEMO_CIPHER_DBG
#define DemoCipher_print(fmt, args...)  { printf( "[%s][%d]"fmt"\n",__FUNCTION__,__LINE__, ## args); }
#else
#define DemoCipher_print(fmt, args...)
#endif

//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------
#define CAVID 0x0F
#define SHA1_SIZE (20)

#define DEMO_CIPHER_ALIGNMENT_LEN 16
#define DEMO_CIPHER_TIMEOUT 500  //ms
/// CIPHER speed in byte-per-ms, which is applied to estimate encrypt/decrypt time.
/// CIPHER AES ECB theoretical bit-rate is 50M/s
#define DEMO_CIPHER_BYTES_PER_MS    51200 // 50 * 1024 bytes

//FIPS 140-1
#define FIPS140_1_NUMBYTES             2500

#define FIPS140_1_MONOBIT_LOWBOUND     9654
#define FIPS140_1_MONOBIT_UPPERBOUND   10346

#define FIPS140_1_POKER_LOWBOUND       1.03
#define FIPS140_1_POKER_UPPERBOUND     57.4

#define FIPS140_1_RUNS_LENGTH   34
#define FIPS140_1_RUNS_1_LOW    2267
#define FIPS140_1_RUNS_1_UP     2733
#define FIPS140_1_RUNS_2_LOW    1079
#define FIPS140_1_RUNS_2_UP     1421
#define FIPS140_1_RUNS_3_LOW    502
#define FIPS140_1_RUNS_3_UP     748
#define FIPS140_1_RUNS_4_LOW    223
#define FIPS140_1_RUNS_4_UP     402
#define FIPS140_1_RUNS_5_LOW    90
#define FIPS140_1_RUNS_5_UP     223
#define FIPS140_1_RUNS_6_LOW    90
#define FIPS140_1_RUNS_6_UP     223

//-------------------------------------------------------------------------------------------------
// Local Function
//-------------------------------------------------------------------------------------------------
static void _DumpBuf(const MS_U8 *buf, MS_U32 len)
{
    MS_U32 i = 0;
    for (i = 0; i < len; i++) {
        printf("0x%02x%c", buf[i], ((i % 16) == 15) ? '\n' : ' ');
    }
    printf("\n");
}

MS_BOOL _FIPS140_1_Monobit_test(MS_U8 *data, MS_U32 u32Size)
{
    MS_U32 i = 0;
    MS_U32 u32Result = 0;

    if(FIPS140_1_NUMBYTES != u32Size)
    {
        printf("Invalid data size\n");
        return FALSE;
    }

    for (i = 0;i < FIPS140_1_NUMBYTES;i++)
    {
        u32Result += bits_table[data[i]];
    }

    return (u32Result > FIPS140_1_MONOBIT_LOWBOUND && u32Result < FIPS140_1_MONOBIT_UPPERBOUND);
}

MS_BOOL _FIPS140_1_Poker_test(MS_U8 *data, MS_U32 u32Size)
{
    MS_U32 i = 0;
    MS_U32 u32Counts[16] = {0};
    MS_U32 u32Sum = 0;
    MS_FLOAT fResult = 0;

    if(FIPS140_1_NUMBYTES != u32Size)
    {
        printf("Invalid data size\n");
        return FALSE;
    }

    for (i = 0;  i < FIPS140_1_NUMBYTES;  i++)
    {
        u32Counts[data[i] & 0xf]++;
        u32Counts[data[i] >> 4]++;
    }
    for (i = 0;  i < 16;  i++)
    {
        u32Sum += (u32Counts[i] * u32Counts[i]);
    }

    fResult = (16.0 / 5000) * (MS_FLOAT)u32Sum - 5000.0;

    return (fResult > FIPS140_1_POKER_LOWBOUND && fResult < FIPS140_1_POKER_UPPERBOUND);
}

MS_BOOL _FIPS140_1_Runs_test(MS_U8 *data, MS_U32 u32Size)
{
    MS_U32 u32Runs[2][7] = {{0},{0}};
    MS_U32 u32Cur_val = 0;
    MS_U32 u32RunsLen = 0;
    MS_U8  u32Curr = 0;
    MS_U32 i = 0;
    MS_U32 j = 0;

    if(FIPS140_1_NUMBYTES != u32Size)
    {
        printf("Invalid data size\n");
        return FALSE;
    }

    for (u32RunsLen = 0;i < FIPS140_1_NUMBYTES;i++)
    {
        u32Curr = data[i];
        for (j = 0;  j < 8;  j++)
        {
            /* Check to see if the current bit is the same as the last one */
            if ((u32Curr & 0x01) ^ u32Cur_val)
            {
                /* The bits are different. A run is over, and a new run of 1 has begun */
                if (u32RunsLen >= FIPS140_1_RUNS_LENGTH)
                    return 0;
                if (u32RunsLen > 6)
                    u32RunsLen = 6;

                u32Runs[u32Cur_val][u32RunsLen]++;
                u32RunsLen = 1;
                u32Cur_val = (u32Cur_val + 1) & 1; /* Switch the value. */
            }
            else
            {
                u32RunsLen++;
            }

            u32Curr >>= 1;
        }
    }

    return (u32Runs[0][1] > FIPS140_1_RUNS_1_LOW && u32Runs[0][1] < FIPS140_1_RUNS_1_UP &&
            u32Runs[0][2] > FIPS140_1_RUNS_2_LOW && u32Runs[0][2] < FIPS140_1_RUNS_2_UP &&
            u32Runs[0][3] > FIPS140_1_RUNS_3_LOW && u32Runs[0][3] < FIPS140_1_RUNS_3_UP &&
            u32Runs[0][4] > FIPS140_1_RUNS_4_LOW && u32Runs[0][4] < FIPS140_1_RUNS_4_UP &&
            u32Runs[0][5] > FIPS140_1_RUNS_5_LOW && u32Runs[0][5] < FIPS140_1_RUNS_5_UP &&
            u32Runs[0][6] > FIPS140_1_RUNS_6_LOW && u32Runs[0][6] < FIPS140_1_RUNS_6_UP &&
            u32Runs[1][1] > FIPS140_1_RUNS_1_LOW && u32Runs[1][1] < FIPS140_1_RUNS_1_UP &&
            u32Runs[1][2] > FIPS140_1_RUNS_2_LOW && u32Runs[1][2] < FIPS140_1_RUNS_2_UP &&
            u32Runs[1][3] > FIPS140_1_RUNS_3_LOW && u32Runs[1][3] < FIPS140_1_RUNS_3_UP &&
            u32Runs[1][4] > FIPS140_1_RUNS_4_LOW && u32Runs[1][4] < FIPS140_1_RUNS_4_UP &&
            u32Runs[1][5] > FIPS140_1_RUNS_5_LOW && u32Runs[1][5] < FIPS140_1_RUNS_5_UP &&
            u32Runs[1][6] > FIPS140_1_RUNS_6_LOW && u32Runs[1][6] < FIPS140_1_RUNS_6_UP);
}

static MS_U32  _Demo_CIPHER_MEMPA2VA_FUNC(MS_U32 u32PhyAdr)
{
    MS_U32 u32VirAdr=MsOS_PA2KSEG1(u32PhyAdr);
    return u32VirAdr;
}


static void _Demo_CIPHER_MEMFLUSH_FUNC(MS_U32 u32Adr,MS_U32 u32Len)
{
    //Only need when use cache memory
    //MsOS_FlushMemory();
    //MsOS_Dcache_Flush(u32Adr,u32Len);
}



static void _Demo_CIPHER_MEMREAD_FUNC(MS_U32 u32Adr,MS_U32 u32Len)
{
    //Only need when use cache memory
    //MsOS_ReadMemory();
    //MsOS_Dcache_Invalidate(u32Adr,u32Len);
}

static MS_BOOL _Demo_CIPHER_Init(void)
{
#if DDI_DEMO_AESDMA_UTPA2
    if(pu32CipherDevice == NULL)
    {
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_AESDMA, &pu32CipherDevice, 0x0, NULL))
        {
            printf("UtopiaOpen CIPHER fail\n");
            return FALSE;
        }
    }

	AESDMA_INIT pArgs;

	pArgs.u32miu0addr = 0; //Cipher don't care parameter
	pArgs.u32miu1addr = 0;
	pArgs.u32miunum = 0;

	if(UTOPIA_STATUS_SUCCESS != UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_Init, (void*)&pArgs))
	{
		printf("Error: MDrv_CMD_AESDMA_Init() failed\n");
        return FALSE;
	}
#else
    // init with default CAVId 0x0F
    if (DRV_CIPHER_OK != MDrv_CIPHER_Init()) {
        printf("Error: MDrv_CIPHER_Init() failed\n");
        return FALSE;
    }

#endif

    return TRUE;
}

static void _Demo_CIPHER_AES_ECB(MS_U8 *pu8DataIn, MS_U8 *pu8DataOut, MS_U32 u32Size)
{

    ST_DEMO_CIPHER_CONFIG stCIPHERInfo;

    printf("CIPHER: AES ECB MSB Key test\n");

    memcpy(pu8DataIn, u8TestData, u32Size);
    memset(pu8DataOut, 0, u32Size);


    // CryptoDMA
    memset(&stCIPHERInfo, 0, sizeof(ST_DEMO_CIPHER_CONFIG));
    // Algo
    stCIPHERInfo.eMode = E_DEMO_CIPHER_MODE_AES_ECB;
    //Key
    stCIPHERInfo.eKeySrc = E_DEMO_CIPHER_KSRC_CPU;
    stCIPHERInfo.u8KLIndex = 0;
    stCIPHERInfo.puCPU8Key = u8TestKey_MSB;
    stCIPHERInfo.pu8IV = NULL;
    //memory
    stCIPHERInfo.pMemFlushFunc = _Demo_CIPHER_MEMFLUSH_FUNC;
    stCIPHERInfo.pMemReadFunc = _Demo_CIPHER_MEMREAD_FUNC;
    stCIPHERInfo.pMemPA2VA = _Demo_CIPHER_MEMPA2VA_FUNC;
    //CAVid
    stCIPHERInfo.u8CAVID = CAVID;

    Demo_CIPHER_Config(stCIPHERInfo);

    //In/Out Data and Encrypt
    Demo_CIPHER_EncryptDecrypt(FALSE, MsOS_VA2PA((MS_U32)pu8DataIn), MsOS_VA2PA((MS_U32)pu8DataOut), u32Size, 50);


    if(memcmp(pu8DataOut, u8TestDataOut_ECB_MSB_Key, u32Size) == 0) {
        printf("CIPHER: AES ECB MSB Key test success\n\n");
    } else {
        printf("CIPHER: AES ECB MSB Key test failed\n\n");
        _DumpBuf(pu8DataOut, u32Size);
    }
}


//-------------------------------------------------------------------------------------------------
// Demo Functions
//-------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// @brief AESDMA Test
// @param[in] None.
// @return TRUE is successful, otherwise FALSE
//--------------------------------------------------------------------------------------------------
static void _Demo_AESDMA_AES_ECB_MSB_Key(MS_U8 *pu8DataIn, MS_U8 *pu8DataOut, MS_U32 u32Size)
{
    printf("AESDMA: AES ECB MSB Key test\n");

#if DDI_DEMO_AESDMA_UTPA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32CipherDevice == NULL)
    {
        DemoCipher_print("Error:Cipher instance NULL");
        return;
    }

    // AESDMA reset
    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_Reset, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    memcpy(pu8DataIn, u8TestData, u32Size);
    memset(pu8DataOut, 0, u32Size);


    // AESDMA select engine
    AESDMA_SEL_ENG pArgs;
    pArgs.eMode = E_DRVAESDMA_CIPHER_ECB;
    pArgs.bDescrypt = FALSE;

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_SelEng, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    AESDMA_SET_KEY_EX pArgsKey;
    pArgsKey.pu8Key = u8TestKey_MSB;
    pArgsKey.u32Len = 16;

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_SetKey_Ex, (void*)&pArgsKey);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }


    // set file in out
    AESDMA_FILE_INOUT pArgsInOut;
    pArgsInOut.u32FileinAddr = MsOS_VA2PA((MS_U32)pu8DataIn);
    pArgsInOut.u32FileInNum  = u32Size;
    pArgsInOut.u32FileOutSAddr = MsOS_VA2PA((MS_U32)pu8DataOut);
    pArgsInOut.u32FileOutEAddr = MsOS_VA2PA((MS_U32)pu8DataOut + u32Size - 1);

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_SetFileInOut, (void*)&pArgsInOut);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    MsOS_FlushMemory();


    // launch AESDMA
    MS_BOOL bStart = TRUE;
    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_Start, &bStart);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }


    // check AESDMA is finished
    while(UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_IsFinished, NULL) != DRV_CIPHER_OK)
          ;

#else
    MDrv_AESDMA_Reset();

    memcpy(pu8DataIn, u8TestData, u32Size);
    memset(pu8DataOut, 0, u32Size);

    MDrv_AESDMA_SelEng(E_DRVAESDMA_CIPHER_ECB, FALSE); // AES ECB Encrypt
    MDrv_AESDMA_SetKey_Ex(u8TestKey_MSB, 16);
    MDrv_AESDMA_SetFileInOut(
            MsOS_VA2PA((MS_U32)pu8DataIn), u32Size,
            MsOS_VA2PA((MS_U32)pu8DataOut),
            MsOS_VA2PA((MS_U32)pu8DataOut + u32Size - 1));

    MsOS_FlushMemory();

    MDrv_AESDMA_Start(TRUE);
    while(MDrv_AESDMA_IsFinished() != DRV_CIPHER_OK)
        ;

#endif

    MsOS_ReadMemory();

    if(memcmp(pu8DataOut, u8TestDataOut_ECB_MSB_Key, u32Size) == 0)
    {
        printf("AESDMA: AES ECB MSB Key test success\n\n");
    }
    else
    {
        printf("AESDMA: AES ECB MSB Key test failed\n\n");
        _DumpBuf(pu8DataOut,128);
    }
}

static void _Demo_AESDMA_AES_ECB_LSB_Key(MS_U8 *pu8DataIn, MS_U8 *pu8DataOut, MS_U32 u32Size)
{
    printf("AESDMA: AES ECB LSB Key test\n");


#if DDI_DEMO_AESDMA_UTPA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32CipherDevice == NULL)
    {
        DemoCipher_print("Error:Cipher instance NULL");
        return;
    }

    // AESDMA reset
    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_Reset, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    memcpy(pu8DataIn, u8TestData, u32Size);
    memset(pu8DataOut, 0, u32Size);

    // AESDMA select engine
    AESDMA_SEL_ENG pArgs;
    pArgs.eMode = E_DRVAESDMA_CIPHER_ECB;
    pArgs.bDescrypt = FALSE;

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_SelEng, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

	u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_SetKey, u8TestKey_MSB); //transfer LSB to HW
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    // set file in out
    AESDMA_FILE_INOUT pArgsInOut;
    pArgsInOut.u32FileinAddr = MsOS_VA2PA((MS_U32)pu8DataIn);
    pArgsInOut.u32FileInNum  = u32Size;
    pArgsInOut.u32FileOutSAddr = MsOS_VA2PA((MS_U32)pu8DataOut);
    pArgsInOut.u32FileOutEAddr = MsOS_VA2PA((MS_U32)pu8DataOut + u32Size - 1);

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_SetFileInOut, (void*)&pArgsInOut);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    MsOS_FlushMemory();

    // launch AESDMA
    MS_BOOL bStart = TRUE;
    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_Start, &bStart);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }


    // check AESDMA is finished
    while(UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_IsFinished, NULL) != DRV_CIPHER_OK)
          ;

#else
    MDrv_AESDMA_Reset();

    memcpy(pu8DataIn, u8TestData, u32Size);
    memset(pu8DataOut, 0, u32Size);

    MDrv_AESDMA_SelEng(E_DRVAESDMA_CIPHER_ECB, FALSE); // AES ECB Encrypt

	MDrv_AESDMA_SetKey((MS_U32 *)u8TestKey_MSB);//transfer LSB to HW

    MDrv_AESDMA_SetFileInOut(
            MsOS_VA2PA((MS_U32)pu8DataIn), u32Size,
            MsOS_VA2PA((MS_U32)pu8DataOut),
            MsOS_VA2PA((MS_U32)pu8DataOut + u32Size - 1));

    MsOS_FlushMemory();

    MDrv_AESDMA_Start(TRUE);
    while(MDrv_AESDMA_IsFinished() != DRV_CIPHER_OK)
        ;

#endif

    MsOS_ReadMemory();

    if(memcmp(pu8DataOut, u8TestDataOut_ECB_LSB_Key, u32Size) == 0)
    {
        printf("AESDMA: AES ECB LSB Key test success\n\n");
    }
    else
    {
        printf("AESDMA: AES ECB LSB Key test failed\n\n");
        _DumpBuf(pu8DataOut,128);
    }
}


static void _Demo_AESDMA_AES_CBC_MSB_Key(MS_U8 *pu8DataIn, MS_U8 *pu8DataOut, MS_U32 u32Size)
{
    printf("AESDMA: AES CBC MSB Key test\n");

#if DDI_DEMO_AESDMA_UTPA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32CipherDevice == NULL)
    {
        DemoCipher_print("Error:Cipher instance NULL");
        return;
    }

    // AESDMA reset
    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_Reset, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    memcpy(pu8DataIn, u8TestData, u32Size);
    memset(pu8DataOut, 0, u32Size);

    // AESDMA select engine
    AESDMA_SEL_ENG pArgs;
    pArgs.eMode = E_DRVAESDMA_CIPHER_CBC;
    pArgs.bDescrypt = FALSE;

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_SelEng, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    AESDMA_SET_KEY_EX pArgsKey;
    pArgsKey.pu8Key = u8TestKey_MSB;
    pArgsKey.u32Len = 16;

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_SetKey_Ex, (void*)&pArgsKey);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    AESDMA_SET_IV_EX pArgsIV;
    pArgsIV.pu8IV = u8TestIV_MSB;
    pArgsIV.u32Len = 16;

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_SetIV_Ex, (void*)&pArgsIV);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    // set file in out
    AESDMA_FILE_INOUT pArgsInOut;
    pArgsInOut.u32FileinAddr = MsOS_VA2PA((MS_U32)pu8DataIn);
    pArgsInOut.u32FileInNum  = u32Size;
    pArgsInOut.u32FileOutSAddr = MsOS_VA2PA((MS_U32)pu8DataOut);
    pArgsInOut.u32FileOutEAddr = MsOS_VA2PA((MS_U32)pu8DataOut + u32Size - 1);

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_SetFileInOut, (void*)&pArgsInOut);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    MsOS_FlushMemory();


    // launch AESDMA
    MS_BOOL bStart = TRUE;
    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_Start, &bStart);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    // check AESDMA is finished
    while(UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_IsFinished, NULL) != DRV_CIPHER_OK)

#else
    MDrv_AESDMA_Reset();

    memcpy(pu8DataIn, u8TestData, u32Size);
    memset(pu8DataOut, 0, u32Size);

    MDrv_AESDMA_SelEng(E_DRVAESDMA_CIPHER_CBC, FALSE); // AES CBC Encrypt

    MDrv_AESDMA_SetKey_Ex(u8TestKey_MSB, 16);
    MDrv_AESDMA_SetIV_Ex(u8TestIV_MSB, 16);

    MDrv_AESDMA_SetFileInOut(
            MsOS_VA2PA((MS_U32)pu8DataIn), u32Size,
            MsOS_VA2PA((MS_U32)pu8DataOut),
            MsOS_VA2PA((MS_U32)pu8DataOut + u32Size - 1));

    MsOS_FlushMemory();

    MDrv_AESDMA_Start(TRUE);
    while(MDrv_AESDMA_IsFinished() != DRVAESDMA_OK)
        ;
#endif

    MsOS_ReadMemory();

    if(memcmp(pu8DataOut, u8TestDataOut_CBC_MSB_Key, u32Size) == 0) {
        printf("AESDMA: AES CBC MSB Key test success\n\n");
    } else {
        printf("AESDMA: AES CBC MSB Key test failed\n\n");
        _DumpBuf(pu8DataOut,128);
    }
}

static void _Demo_AESDMA_AES_CBC_LSB_Key(MS_U8 *pu8DataIn, MS_U8 *pu8DataOut, MS_U32 u32Size)
{
    printf("AESDMA: AES CBC LSB Key test\n");

#if DDI_DEMO_AESDMA_UTPA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    if(pu32CipherDevice == NULL)
    {
        DemoCipher_print("Error:Cipher instance NULL");
        return;
    }

    // AESDMA reset
    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_Reset, NULL);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    memcpy(pu8DataIn, u8TestData, u32Size);
    memset(pu8DataOut, 0, u32Size);

    // AESDMA select engine
    AESDMA_SEL_ENG pArgs;
    pArgs.eMode = E_DRVAESDMA_CIPHER_CBC;
    pArgs.bDescrypt = FALSE;

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_SelEng, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

	u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_SetKey, u8TestKey_MSB); //transfer LSB to HW
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_SetIV, u8TestIV_MSB); //transfer LSB to HW
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    // set file in out
    AESDMA_FILE_INOUT pArgsInOut;
    pArgsInOut.u32FileinAddr = MsOS_VA2PA((MS_U32)pu8DataIn);
    pArgsInOut.u32FileInNum  = u32Size;
    pArgsInOut.u32FileOutSAddr = MsOS_VA2PA((MS_U32)pu8DataOut);
    pArgsInOut.u32FileOutEAddr = MsOS_VA2PA((MS_U32)pu8DataOut + u32Size - 1);

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_SetFileInOut, (void*)&pArgsInOut);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    MsOS_FlushMemory();


    // launch AESDMA
    MS_BOOL bStart = TRUE;
    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_Start, &bStart);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

    // check AESDMA is finished
    while(UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_IsFinished, NULL) != DRV_CIPHER_OK)

#else
    MDrv_AESDMA_Reset();

    memcpy(pu8DataIn, u8TestData, u32Size);
    memset(pu8DataOut, 0, u32Size);

    MDrv_AESDMA_SelEng(E_DRVAESDMA_CIPHER_CBC, FALSE); // AES CBC Encrypt
	MDrv_AESDMA_SetKey((MS_U32 *)u8TestKey_MSB); //transfer LSB to HW
    MDrv_AESDMA_SetIV((MS_U32 *)u8TestIV_MSB);

    MDrv_AESDMA_SetFileInOut(
            MsOS_VA2PA((MS_U32)pu8DataIn), u32Size,
            MsOS_VA2PA((MS_U32)pu8DataOut),
            MsOS_VA2PA((MS_U32)pu8DataOut + u32Size - 1));

    MsOS_FlushMemory();

    MDrv_AESDMA_Start(TRUE);
    while(MDrv_AESDMA_IsFinished() != DRVAESDMA_OK)
        ;
#endif


    MsOS_ReadMemory();

    if(memcmp(pu8DataOut, u8TestDataOut_CBC_LSB_Key, u32Size) == 0) {
        printf("AESDMA: AES CBC LSB Key test success\n\n");
    } else {
        printf("AESDMA: AES CBC LSB Key test failed\n\n");
        _DumpBuf(pu8DataOut,128);
    }
}

static void _Demo_AESDMA_RandomNumber(void)
{
    printf( "AESDMA: Random Number test\n");
    MS_U8 u8Error = 0;

    MS_S32 NON_CACHE_POOL_ID = INVALID_POOL_ID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&NON_CACHE_POOL_ID);

    MS_U8 *pu8RandomNumber = (MS_U8 *)MsOS_AllocateMemory(FIPS140_1_NUMBYTES, NON_CACHE_POOL_ID);
    MS_U32 u32RandomNumber = MsOS_VA2PA((MS_U32)pu8RandomNumber);

    memset(pu8RandomNumber, 0, FIPS140_1_NUMBYTES);

#if DDI_DEMO_AESDMA_UTPA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    AESDMA_RAND pArgs;

    if(pu32CipherDevice == NULL)
    {
        DemoCipher_print("Error:Cipher instance NULL");
        return;
    }

    pArgs.u32PABuf = (MS_U32 *)u32RandomNumber;
    pArgs.u32Size  = FIPS140_1_NUMBYTES;

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_AESDMA_Rand, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

#else

    if(DRVAESDMA_OK != MDrv_AESDMA_Rand((MS_U32 *)u32RandomNumber,FIPS140_1_NUMBYTES))
    {
        printf("AESDMA: Get random number fail\n");
        goto AESDMA_RNDNUM_EXIT;

    }
#endif

    if(FALSE == _FIPS140_1_Monobit_test(pu8RandomNumber, FIPS140_1_NUMBYTES))
    {
        printf("AESDMA: Random number monobit test fail\n");
        u8Error++;
    }

    if(FALSE == _FIPS140_1_Poker_test(pu8RandomNumber, FIPS140_1_NUMBYTES))
    {
        printf("AESDMA: Random number poker test fail\n");
        u8Error++;
    }

    if(FALSE == _FIPS140_1_Runs_test(pu8RandomNumber, FIPS140_1_NUMBYTES))
    {
        printf("AESDMA: Random number runs & long runs test fail\n");
        u8Error++;
    }

    if(0 == u8Error)
    {
        printf("AESDMA: Random number test success\n\n");
    }

AESDMA_RNDNUM_EXIT:

    MsOS_FreeMemory(pu8RandomNumber, NON_CACHE_POOL_ID);

    return;
}


#if (DEMO_DSCMB_ADVANCED_TEST == 1)

static MS_BOOL _Demo_CIPHER_Setup_KeyLadder(void)
{
    DEMOKLCONFIG stKLInfo;

    memset(&stKLInfo, 0, sizeof(DEMOKLCONFIG));
    stKLInfo.eKLMode = E_DEMO_KL_AES_MODE;
    stKLInfo.eSrc = E_DEMO_KL_SRC_ACPU;
    stKLInfo.eDst = E_DEMO_KL_DST_DMA_SK0;
    stKLInfo.eKeyType = 0;
    stKLInfo.u32DscmbId = 0;
    stKLInfo.u32EngID = 0;
    stKLInfo.u32Level = 3;
    stKLInfo.pu8KeyACPU = u8KL_RootKey_MSB;
    stKLInfo.pu8KeyKLIn = u8KL_AESDMA_Input_MSB;
    stKLInfo.u32CAVid = CAVID;
    return DEMO_KeyLadder_Setup(stKLInfo);
}

static void _Demo_CIPHER_AES_CBC_with_KeyLadder(MS_U8 *pu8DataIn, MS_U8 *pu8DataOut, MS_U32 u32Size)
{

    ST_DEMO_CIPHER_CONFIG stCIPHERInfo;

    printf("AESDMA: AES CBC MSB Key test with KeyLadder\n");


    // CryptoDMA
    memcpy(pu8DataIn, u8TestData, u32Size);
    memset(pu8DataOut, 0, u32Size);
    memset(&stCIPHERInfo, 0, sizeof(ST_DEMO_CIPHER_CONFIG));

    // Algo
    stCIPHERInfo.eMode = E_DEMO_CIPHER_MODE_AES_CBC;
    //Key
    stCIPHERInfo.eKeySrc = E_DEMO_CIPHER_KSRC_KL;
    stCIPHERInfo.u8KLIndex = 0;
    stCIPHERInfo.puCPU8Key = NULL;
    stCIPHERInfo.pu8IV = u8TestIV_MSB;
    // Key Ladder callback function
    stCIPHERInfo.pKLCallbackFunc = _Demo_CIPHER_Setup_KeyLadder;
    //memory
    stCIPHERInfo.pMemFlushFunc = _Demo_CIPHER_MEMFLUSH_FUNC;
    stCIPHERInfo.pMemReadFunc = _Demo_CIPHER_MEMREAD_FUNC;
    stCIPHERInfo.pMemPA2VA = _Demo_CIPHER_MEMPA2VA_FUNC;
    //CAVid
    stCIPHERInfo.u8CAVID = CAVID;

    Demo_CIPHER_Config(stCIPHERInfo);

    //In/Out Data and Encrypt
    Demo_CIPHER_EncryptDecrypt(FALSE, MsOS_VA2PA((MS_U32)pu8DataIn), MsOS_VA2PA((MS_U32)pu8DataOut), u32Size, 50);


    if(memcmp(pu8DataOut, u8TestDataOut_CBC_MSB_Key, u32Size) == 0)
    {
        printf("CIPHER: AES CBC MSB Key test with KeyLadder success\n\n");
    }
    else
    {
        printf("CIPHER: AES CBC MSB Key test with KeyLadder failed\n\n");
        _DumpBuf(pu8DataOut,128);
    }
}

#endif

//////////////////////////////////////////////////////////////////////////
static MS_U8 msg0[64]  = {0x1c,0x76,0x30,0x95,0x25,0x70,0xf0,0x61,0xf3,0xc3,0xe0,0x2a,0x02,0x09,0x7f,0x3b,0xc0,0x49,0xa1,0x02,0x6b,0x68,0x76,0x77,0x35,0xce,0xd1,0xc4,0xe0,0x0d,0xea,0xfd,0x84,0x1a,0x92,0xa9,0x8a,0x82,0x0a,0x7e,0x45,0xea,0xa8,0x47,0xf4,0x27,0x82,0xb4,0x70,0x23,0xb7,0xdb,0x8b,0x2d,0x53,0xc0,0xfb,0x24,0x84,0xdc,0x32,0x6e,0xd9,0xb6};
static MS_U8 msg1[64]  = {0x89,0x6b,0x5f,0x13,0xed,0x6a,0x91,0x32,0x54,0x39,0x79,0x48,0x60,0xfb,0xfd,0xd0,0x1f,0xb4,0xac,0xaa,0xe1,0xff,0x6b,0xdd,0x23,0xef,0xb9,0x55,0x5e,0x92,0x0b,0xe7,0xfd,0x6b,0xfa,0xea,0xd5,0x8c,0x1c,0x29,0xc5,0x95,0x72,0x26,0x90,0x6f,0xf6,0xaf,0x23,0xa2,0x5a,0x04,0xa1,0xc5,0xe1,0xc5,0xb4,0x9a,0x1a,0x12,0x2c,0x26,0xf9,0x29};
static MS_U8 msg2[60]  = {0x91,0xf4,0x13,0x66,0x80,0x2f,0x8f,0x45,0xc4,0x01,0x6b,0x55,0x70,0x62,0x04,0x93,0x04,0x5e,0x98,0xa6,0x23,0x79,0x6b,0xd8,0x14,0x85,0xea,0x40,0xab,0xe4,0x6a,0x3c,0xd8,0x7d,0xa2,0x58,0xad,0x32,0x9d,0x71,0x33,0x09,0xc6,0xa4,0x6b,0xcb,0x37,0x6f,0x29,0xcf,0x15,0x4d,0x49,0x80,0x25,0x5d,0x06,0x0f,0x9d,0xb1};

MS_U8 SHA1_Golden_MSB[20] = {0x1a, 0x4e, 0x03, 0x01, 0x92, 0xe0, 0xb5, 0x2a, 0x2e, 0xf7, 0x3e, 0xc5, 0x49, 0x0d, 0x50, 0xe5,0x6e, 0xab, 0x0e, 0xa6};
MS_U8 SHA1_Golden_LSB[20] = {0xa6, 0x0e, 0xab, 0x6e, 0xe5, 0x50, 0x0d, 0x49, 0xc5, 0x3e, 0xf7, 0x2e, 0x2a, 0xb5, 0xe0, 0x92,0x01, 0x03, 0x4e, 0x1a};

//MS_U8 SHA1_Result[20] = {0x0};

void Demo_CIPHER_HASH_SHA1(MS_U8 *pu8MsgIn, MS_U8 *pu8Digest_Buffer)
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

void Demo_CIPHER_HASH_Manual_SHA1(MS_U8 *pu8MsgIn, MS_U8 *pu8Digest_Buffer)
{
    DRV_CIPHER_HASHCFG_MANUAL first_cfg = {0};
    DRV_CIPHER_HASHCFG_MANUAL update_cfg = {0};
    DRV_CIPHER_HASHCFG_MANUAL last_cfg = {0};
    MS_U32 u32CmdId = 0;
    MS_U32 u32Exception = 0;

    DRV_CIPHER_RET ret;
    MS_S32 NON_CACHE_POOL_ID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&NON_CACHE_POOL_ID);

    //NOTE: !!!!! Hash input message must be 16bytes alignment  !!!!!!!
    MS_U8 *u8TmpIV =        (MS_U8*)MsOS_AllocateMemory(SHA1_SIZE, NON_CACHE_POOL_ID);
    MS_U8 *u8PrevIV =       (MS_U8*)MsOS_AllocateMemory(SHA1_SIZE, NON_CACHE_POOL_ID);

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

    MsOS_FreeMemory(u8TmpIV, NON_CACHE_POOL_ID);
    MsOS_FreeMemory(u8PrevIV, NON_CACHE_POOL_ID);
}

void Demo_AESDMA_HASH_SHA1(MS_U8 *pu8MsgIn, MS_U8 *pu8Digest_Buffer)
{
    printf("AESDMA: SHA1 Auto test\n");


    DrvAESDMA_SHAMode eMode = E_DRVAESDMA_SHA1;

    memcpy(pu8MsgIn       ,msg0, 64);
    memcpy(pu8MsgIn+64    ,msg1, 64);
    memcpy(pu8MsgIn+64+64 ,msg2, 60);


#if DDI_DEMO_AESDMA_UTPA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    SHA_CALCULATE pArgs;
    if(pu32CipherDevice == NULL)
    {
        DemoCipher_print("Error:Cipher instance NULL");
        return;
    }

    pArgs.eMode = eMode;
    pArgs.u32PAInBuf  = MsOS_VA2PA((MS_U32)pu8MsgIn);
    pArgs.u32Size     = 188;
    pArgs.u32PAOutBuf = MsOS_VA2PA((MS_U32)pu8Digest_Buffer);

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_SHA_Calculate, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }
#else
    DRVAESDMA_RESULT ret = DRVAESDMA_FAIL;
    ret = MDrv_SHA_Calculate(eMode,MsOS_VA2PA((MS_U32)pu8MsgIn), 188, MsOS_VA2PA((MS_U32)pu8Digest_Buffer));
    if(ret != 0) {printf("HASH FAIL!!!\n"); return;}
#endif


    if(memcmp(SHA1_Golden_LSB, pu8Digest_Buffer, SHA1_SIZE) == 0) {
        printf("AESDMA: SHA1 Auto test success\n\n");
    } else {
        _DumpBuf(pu8Digest_Buffer, SHA1_SIZE);
        printf("AESDMA: SHA1 Auto test failed\n\n");
    }

}

void Demo_AESDMA_HASH_Manual_SHA1(MS_U8 *pu8MsgIn, MS_U8 *pu8Digest_Buffer)
{
    DrvAESDMA_HASHCFG first_cfg = {0};
    DrvAESDMA_HASHCFG update_cfg = {0};
    DrvAESDMA_HASHCFG last_cfg = {0};

    printf( "AESDMA: SHA1 Manual test\n");

    MS_S32 NON_CACHE_POOL_ID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&NON_CACHE_POOL_ID);

    //NOTE: !!!!! Hash input message must be 16bytes alignment  !!!!!!!
    MS_U8 *u8TmpIV =        (MS_U8*)MsOS_AllocateMemory(SHA1_SIZE, NON_CACHE_POOL_ID);
    MS_U8 *u8PrevIV =       (MS_U8*)MsOS_AllocateMemory(SHA1_SIZE, NON_CACHE_POOL_ID);

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

#if DDI_DEMO_AESDMA_UTPA2
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    SHA_CALCULATE_MANUAL pArgs;
    if(pu32CipherDevice == NULL)
    {
        DemoCipher_print("Error:Cipher instance NULL");
        return;
    }

    pArgs.stCfg        = first_cfg;
    pArgs.eStage       = E_DRVAESDMA_HASH_STAGE_FIRST;
    pArgs.u32DoneBytes = 0;
    pArgs.pu8SetIV     = NULL;

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_SHA_CalculateManual, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }
#else
    DRVAESDMA_RESULT ret;
    ret = MDrv_SHA_CalculateManual(first_cfg,E_DRVAESDMA_HASH_STAGE_FIRST,0,NULL);
#endif


    memcpy(pu8MsgIn,msg1,sizeof(msg1));
    memcpy(u8PrevIV, u8TmpIV, SHA1_SIZE);
    memset(u8TmpIV, 0, SHA1_SIZE);

    update_cfg.eMode = E_CIPHER_HASH_ALGO_SHA1;
    update_cfg.stInput.u32Addr = (MS_U32)pu8MsgIn;
    update_cfg.stInput.u32Size = sizeof(msg1);
    update_cfg.stOutput.u32Addr = (MS_U32)u8TmpIV;
    update_cfg.stOutput.u32Size = SHA1_SIZE;


#if DDI_DEMO_AESDMA_UTPA2
    pArgs.stCfg        = update_cfg;
    pArgs.eStage       = E_DRVAESDMA_HASH_STAGE_UPDATE;
    pArgs.u32DoneBytes = sizeof(msg0);
    pArgs.pu8SetIV     = u8PrevIV;

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_SHA_CalculateManual, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }

#else
    ret = MDrv_SHA_CalculateManual(update_cfg,E_DRVAESDMA_HASH_STAGE_UPDATE,sizeof(msg0),u8PrevIV);
#endif

    memcpy(pu8MsgIn,msg2,sizeof(msg2));
    memcpy(u8PrevIV, u8TmpIV, SHA1_SIZE);
    memset(u8TmpIV, 0, SHA1_SIZE);

    last_cfg.eMode = E_CIPHER_HASH_ALGO_SHA1;
    last_cfg.stInput.u32Addr = (MS_U32)pu8MsgIn;
    last_cfg.stInput.u32Size = sizeof(msg2);
    last_cfg.stOutput.u32Addr = (MS_U32)pu8Digest_Buffer;
    last_cfg.stOutput.u32Size = SHA1_SIZE;


#if DDI_DEMO_AESDMA_UTPA2
    pArgs.stCfg        = last_cfg;
    pArgs.eStage       = E_DRVAESDMA_HASH_STAGE_LAST;
    pArgs.u32DoneBytes = sizeof(msg0) + sizeof(msg1);
    pArgs.pu8SetIV     = u8PrevIV;

    u32Ret = UtopiaIoctl(pu32CipherDevice, MDrv_CMD_SHA_CalculateManual, (void*)&pArgs);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        DemoCipher_print("Error:UtopiaIoctl return value 0x%x", (unsigned int)u32Ret);
        return;
    }
#else
    ret = MDrv_SHA_CalculateManual(last_cfg,E_DRVAESDMA_HASH_STAGE_LAST,(sizeof(msg0) + sizeof(msg1)),u8PrevIV);
    if(ret != 0) {printf("HASH FAIL!!!\n"); return;}
#endif



    if(memcmp(SHA1_Golden_LSB, pu8Digest_Buffer, SHA1_SIZE) == 0) {
        printf("AESDMA: SHA1 Manual test success\n\n");
    } else {
        _DumpBuf(pu8Digest_Buffer, SHA1_SIZE);
        printf("AESDMA: SHA1 Manual test failed\n\n");
    }

    MsOS_FreeMemory(u8TmpIV, NON_CACHE_POOL_ID);
    MsOS_FreeMemory(u8PrevIV, NON_CACHE_POOL_ID);
}


MS_BOOL Demo_CIPHER_DoCIPHERDemo(void)
{
    if(FALSE == _Demo_CIPHER_Init())
    {
        printf("CIPHER: driver init failed\n");
    }

    MS_S32 NON_CACHE_POOL_ID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&NON_CACHE_POOL_ID);

    //DMA Test Buffer
    MS_U32 u32Size = sizeof(u8TestData);
    MS_U8 *u8Buf = MsOS_AllocateMemory(u32Size * 2, NON_CACHE_POOL_ID);
    MS_U8 *pu8DataIn = u8Buf;
    MS_U8 *pu8DataOut = u8Buf + u32Size;

    //HASH Test Buffer
    //NOTE: !!!!! Hash input message must be 16bytes alignment  !!!!!!!
    MS_U32 u32MsgSize = sizeof(msg0) + sizeof(msg1) + sizeof(msg2);
    MS_U8 *pu8MsgIn   = u8Buf;
    MS_U8 *pu8Digest_Buffer = u8Buf + u32MsgSize;
    MS_U32 u32AlignSize = 0;                     // alignment
    u32AlignSize = (int)pu8Digest_Buffer;
    if((u32AlignSize%16)!=0)
        {
            pu8Digest_Buffer = pu8Digest_Buffer + 16 -(u32AlignSize%16) ;
        }

    if(u8Buf == NULL) {
        printf("Error: failed\n");
        return FALSE;
    }

    memset(pu8DataIn, 0xff, u32Size);
    memset(pu8DataOut, 0xff, u32Size);
    _Demo_CIPHER_AES_ECB(pu8DataIn, pu8DataOut, u32Size);

#if (DEMO_DSCMB_ADVANCED_TEST == 1)
    memset(pu8DataIn, 0xff, u32Size);
    memset(pu8DataOut, 0xff, u32Size);
    _Demo_CIPHER_AES_CBC_with_KeyLadder(pu8DataIn, pu8DataOut, u32Size);
#endif

    memset(pu8MsgIn, 0xff, u32MsgSize);
    memset(pu8Digest_Buffer, 0xff, SHA1_SIZE);
    Demo_CIPHER_HASH_SHA1(pu8MsgIn, pu8Digest_Buffer);

    memset(pu8MsgIn, 0xff, u32MsgSize);
    memset(pu8Digest_Buffer, 0xff, SHA1_SIZE);
    Demo_CIPHER_HASH_Manual_SHA1(pu8MsgIn, pu8Digest_Buffer);

    printf("\n=======Old API Compatible Test Start==========\n");

    if (DRV_CIPHER_OK != MDrv_AESDMA_Init(0,0,0)) {
        printf("Error: MDrv_AESDMA_Init() failed\n");
        return FALSE;
    }

    memset(pu8DataIn, 0xff, u32Size);
    memset(pu8DataOut, 0xff, u32Size);
    _Demo_AESDMA_AES_ECB_MSB_Key(pu8DataIn, pu8DataOut, u32Size);

    memset(pu8DataIn, 0xff, u32Size);
    memset(pu8DataOut, 0xff, u32Size);
    _Demo_AESDMA_AES_ECB_LSB_Key(pu8DataIn, pu8DataOut, u32Size);

    memset(pu8DataIn, 0xff, u32Size);
    memset(pu8DataOut, 0xff, u32Size);
    _Demo_AESDMA_AES_CBC_MSB_Key(pu8DataIn, pu8DataOut, u32Size);

    memset(pu8DataIn, 0xff, u32Size);
    memset(pu8DataOut, 0xff, u32Size);
    _Demo_AESDMA_AES_CBC_LSB_Key(pu8DataIn, pu8DataOut, u32Size);

    memset(pu8MsgIn, 0xff, u32MsgSize);
    memset(pu8Digest_Buffer, 0xff, SHA1_SIZE);
    Demo_AESDMA_HASH_SHA1(pu8MsgIn, pu8Digest_Buffer);

    memset(pu8MsgIn, 0xff, u32MsgSize);
    memset(pu8Digest_Buffer, 0xff, SHA1_SIZE);
    Demo_AESDMA_HASH_Manual_SHA1(pu8MsgIn, pu8Digest_Buffer);

    printf("\n=======Old API Compatible Test Finish==========\n");

    //Free memory
    MsOS_FreeMemory(u8Buf, NON_CACHE_POOL_ID);

    return TRUE;
}


MS_BOOL Demo_CIPHER_Config(ST_DEMO_CIPHER_CONFIG stInfo)
{
    memcpy(&stDemoCIPHERConfig,&stInfo,sizeof(ST_DEMO_CIPHER_CONFIG));
    return TRUE;
}


MS_BOOL Demo_CIPHER_EncryptDecrypt
(MS_BOOL bDescrypt, MS_U32 u32PhyAddressIn,
MS_U32 u32PhyAddressOut,MS_U32 u32Length,
MS_U32 u32Loop_wait_time)
{
    MS_U32 u32CurrTime = 0;
    MS_U32 u32CIPHERInVirAddr;
    MS_U32 u32CIPHEROutVirAddr;
    MS_U32 u32DelayTimeMS = (u32Length / DEMO_CIPHER_BYTES_PER_MS);

    DRV_CIPHER_DMACFG stCfg = {{0}};
    MS_U32 u32CmdId = 0;
    MS_U32 u32Exception = 0;
    DRV_CIPHER_RET enRet = DRV_CIPHER_FAIL;

#if DEMO_CIPHER_DBG
    MS_U32 u32CurrTime_1 = 0;
    MS_U32 u32CurrTime_2 = 0;
    MS_U32 u32CurrTime_3 = 0;
    MS_U32 u32CurrTime_4 = 0;
    MS_U32 u32CurrTime_5 = 0;
    MS_U32 u32CurrTime_6 = 0;
    MS_U32 u32CurrTime_7 = 0;
    MS_U32 u32CurrTime_11 = 0;
    MS_U32 loop_cnt = 0;
    u32CurrTime_1 = MsOS_GetSystemTime();
#endif

    if (stDemoCIPHERConfig.eKeySrc >= E_DEMO_CIPHER_KSRC_INVALID)
    {
        printf("[%s][%d]Error Key source(%d)!",__FUNCTION__,__LINE__,stDemoCIPHERConfig.eKeySrc);
        return FALSE;
    }

    if (((stDemoCIPHERConfig.eKeySrc == E_DEMO_CIPHER_KSRC_CPU) && (stDemoCIPHERConfig.puCPU8Key == NULL)) ||
        ((stDemoCIPHERConfig.eKeySrc == E_DEMO_CIPHER_KSRC_KL) && (stDemoCIPHERConfig.pKLCallbackFunc == NULL)) ||
        ((stDemoCIPHERConfig.eMode == E_DEMO_CIPHER_MODE_AES_CBC) && (stDemoCIPHERConfig.pu8IV == NULL)))
    {
        printf("[%s][%d]Error Key! [KeySrc, CPUKey, KLCallback]:[%d,%08x,%08x] , [Mode, IVKey]:[%d,%08x]",__FUNCTION__,__LINE__,stDemoCIPHERConfig.eKeySrc,(unsigned int)stDemoCIPHERConfig.puCPU8Key,(unsigned int)stDemoCIPHERConfig.pKLCallbackFunc,stDemoCIPHERConfig.eMode,(unsigned int)stDemoCIPHERConfig.pu8IV);
        return FALSE;
    }

    //Min. byte limitation (16X)
    if((u32Length <DEMO_CIPHER_ALIGNMENT_LEN) || (u32Length%DEMO_CIPHER_ALIGNMENT_LEN !=0)
        || (u32PhyAddressIn%DEMO_CIPHER_ALIGNMENT_LEN!=0) || (u32PhyAddressOut%DEMO_CIPHER_ALIGNMENT_LEN!=0))
    {
        printf("[%s][%d]Error: CIPHER Write Buffer (%08x,%08x,%08x) is invalid in mode(%d). \n",__FUNCTION__,__LINE__,(unsigned int)u32PhyAddressIn,(unsigned int)u32PhyAddressOut,(unsigned int)u32Length,bDescrypt);
        return FALSE;
    }

#if DEMO_CIPHER_DBG
    u32CurrTime_11 = MsOS_GetSystemTime();
#endif
    //Key-Ladder setting
    if (stDemoCIPHERConfig.eKeySrc == E_DEMO_CIPHER_KSRC_KL)
    {
        stDemoCIPHERConfig.pKLCallbackFunc();
    }
#if DEMO_CIPHER_DBG
    u32CurrTime_11 = MsOS_GetSystemTime() - u32CurrTime_11;
#endif
    memset(&stCfg, 0, sizeof(stCfg));

    u32CIPHERInVirAddr = stDemoCIPHERConfig.pMemPA2VA(u32PhyAddressIn);
    u32CIPHEROutVirAddr = stDemoCIPHERConfig.pMemPA2VA(u32PhyAddressOut);

    //Set CAVid
    stCfg.u32CAVid = stDemoCIPHERConfig.u8CAVID;

    // Set Algorithm
    switch(stDemoCIPHERConfig.eMode)
    {
        case E_DEMO_CIPHER_MODE_AES_ECB:
            stCfg.stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
            stCfg.stAlgo.eSubAlgo = E_CIPHER_SUB_ECB;
            stCfg.stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            stCfg.stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;

        case E_DEMO_CIPHER_MODE_AES_CBC:
            stCfg.stAlgo.eMainAlgo = E_CIPHER_MAIN_AES;
            stCfg.stAlgo.eSubAlgo = E_CIPHER_SUB_CBC;
            stCfg.stAlgo.eResAlgo = E_CIPHER_RES_CLR;
            stCfg.stAlgo.eSBAlgo = E_CIPHER_SB_CLR;
            break;

        default:
            printf("[%s][%d] Error mode(%d)\n",__FUNCTION__,__LINE__,stDemoCIPHERConfig.eMode);
            return FALSE;
            break;
    }


    //Key information
    if (stDemoCIPHERConfig.eKeySrc == E_DEMO_CIPHER_KSRC_CPU)
    {
        stCfg.stKey.eKeySrc = E_CIPHER_KSRC_CPU;
        stCfg.stKey.u8KeyLen = 16;
        stCfg.stKey.pu8KeyData = stDemoCIPHERConfig.puCPU8Key;
    }
    else if (stDemoCIPHERConfig.eKeySrc == E_DEMO_CIPHER_KSRC_KL)
    {
        stCfg.stKey.eKeySrc = E_CIPHER_KSRC_KL;
        stCfg.stKey.u8KeyIdx = stDemoCIPHERConfig.u8KLIndex;
    }
    //IV
    if (stDemoCIPHERConfig.eMode == E_DEMO_CIPHER_MODE_AES_CBC)
    {
        stCfg.stKey.pu8IVData = stDemoCIPHERConfig.pu8IV;
        stCfg.stKey.u8IVLen = 16;
    }

    //Set encrypt/descrypt
    stCfg.bDecrypt = bDescrypt;

    //Set read buffer
    stCfg.stInput.u32Addr = u32PhyAddressIn;
    stCfg.stInput.u32Size = u32Length;

    //Set write buffer
    stCfg.stOutput.u32Addr = u32PhyAddressOut;
    stCfg.stOutput.u32Size = u32Length;
#if DEMO_CIPHER_DBG
    u32CurrTime_2 = MsOS_GetSystemTime();
#endif
    //memory flush
    if (stDemoCIPHERConfig.pMemFlushFunc != NULL)
    {
        stDemoCIPHERConfig.pMemFlushFunc(u32CIPHERInVirAddr,u32Length);
    }
    //Start to act
#if DEMO_CIPHER_DBG
    u32CurrTime_3 = MsOS_GetSystemTime();
#endif
    enRet = MDrv_CIPHER_DMACalc(stCfg, &u32CmdId);
#if DEMO_CIPHER_DBG
    u32CurrTime_4 = MsOS_GetSystemTime();
#endif
    if(enRet != DRV_CIPHER_OK)
    {
        printf("[%s][%d]CIPHER DMA FAIL!!!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if (u32DelayTimeMS > 0)
    {
        MsOS_DelayTask(u32DelayTimeMS);
    }
#if DEMO_CIPHER_DBG
    u32CurrTime_5 = MsOS_GetSystemTime();
#endif

    //Wait for DMA done
    u32CurrTime = MsOS_GetSystemTime();
    while(MDrv_CIPHER_IsDMADone(u32CmdId,&u32Exception) != TRUE)
    {
        if((MsOS_GetSystemTime()-u32CurrTime)> DEMO_CIPHER_TIMEOUT)//dma process timeout
        {
            printf("!!!!!!!!!!!!!!!!!!!!!!!!! cipher process timeout !!!!!!!!!!!!!!!!!!!!!!!!!\n");
            break;
        }
        MsOS_DelayTask(u32Loop_wait_time);
#if DEMO_CIPHER_DBG
        loop_cnt++;
#endif
    }
#if DEMO_CIPHER_DBG
    u32CurrTime_6 = MsOS_GetSystemTime();
#endif
    //memory invalidate
    if (stDemoCIPHERConfig.pMemReadFunc != NULL)
    {
        stDemoCIPHERConfig.pMemReadFunc(u32CIPHEROutVirAddr,u32Length);
    }
#if DEMO_CIPHER_DBG
    u32CurrTime_7 = MsOS_GetSystemTime();
    DemoCipher_print("C[(%lu(%lu),%lu,%lu,%lu,%lu,%lu:%lu]  ",u32CurrTime_2-u32CurrTime_1,u32CurrTime_11,u32CurrTime_3-u32CurrTime_2,u32CurrTime_4-u32CurrTime_3,u32CurrTime_5-u32CurrTime_4,u32CurrTime_6-u32CurrTime_5,u32CurrTime_7-u32CurrTime_6,loop_cnt);
#endif
    return TRUE;

}

MS_BOOL Demo_CIPHER_RandonNumber(void)
{
    if(DRV_CIPHER_OK != MDrv_AESDMA_Init(0,0,0))
    {
        printf("Error: MDrv_AESDMA_Init() failed\n");
        return FALSE;
    }
    //Random number test : FIPS140-1
    _Demo_AESDMA_RandomNumber();

    return TRUE;
}


#endif

