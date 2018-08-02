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
/// @file   demo_rsa.c
/// @author MStar Semiconductor Inc.
/// @brief  AESDMA demo code
///
/// @name TV App Command Usage
//-------------------------------------------------------------------------------------------------
#if (DEMO_RSA_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsMemory.h"
#include "string.h"
#include "drvAESDMA.h"
#include "demo_utility.h"

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------------------------------------------
static const MS_U8 rsa_a_256[256] = {
    0x02, 0x15, 0x35, 0x24, 0xc0, 0x89, 0x5e, 0x81, 0x84, 0x84, 0xd6, 0x09, 0xb1, 0xf0, 0x56, 0x63,
    0x06, 0xb9, 0x7b, 0x0d, 0x46, 0xdf, 0x99, 0x8d, 0xb2, 0xc2, 0x84, 0x65, 0x89, 0x37, 0x52, 0x12,
    0x00, 0xf3, 0xe3, 0x01, 0x06, 0xd7, 0xcd, 0x0d, 0x3b, 0x23, 0xf1, 0x76, 0x1e, 0x8d, 0xcd, 0x3d,
    0x76, 0xd4, 0x57, 0xed, 0x46, 0x2d, 0xf7, 0x8c, 0x7c, 0xfd, 0xe9, 0xf9, 0xe3, 0x37, 0x24, 0xc6,
    0xe2, 0xf7, 0x84, 0xc5, 0xd5, 0x13, 0xd2, 0xaa, 0x72, 0xaf, 0xf7, 0xe5, 0xbb, 0xd2, 0x72, 0x77,
    0x89, 0x32, 0xd6, 0x12, 0x47, 0xec, 0xdb, 0x8f, 0x79, 0x30, 0x69, 0xf2, 0xe7, 0x76, 0x96, 0xce,
    0xf4, 0x00, 0x7a, 0xe8, 0xe2, 0xca, 0x4e, 0xc5, 0x2e, 0x58, 0x49, 0x5c, 0xde, 0x8e, 0x28, 0xbd,
    0x96, 0xab, 0x58, 0x2d, 0xb2, 0xa7, 0x26, 0x65, 0xb1, 0xef, 0x62, 0x63, 0x05, 0x73, 0x87, 0x0a,
    0xc0, 0x3b, 0x22, 0x80, 0x10, 0x64, 0x21, 0x20, 0x55, 0x78, 0x45, 0xaa, 0xce, 0xcc, 0xcc, 0x9d,
    0xcb, 0x20, 0x3e, 0x96, 0x89, 0x83, 0xb8, 0x13, 0x86, 0xbc, 0x38, 0x0d, 0xa9, 0xa7, 0xd6, 0x53,
    0x35, 0x9f, 0xdd, 0x6b, 0xea, 0xa6, 0x2a, 0xd5, 0x81, 0x17, 0x4a, 0x02, 0xd7, 0x56, 0x3e, 0xae,
    0x0e, 0xff, 0xe9, 0x1d, 0xe7, 0xc5, 0x72, 0xcf, 0x11, 0x84, 0x49, 0x23, 0x05, 0x09, 0x65, 0x0a,
    0xe5, 0x73, 0x0a, 0xca, 0x9e, 0x31, 0x4c, 0x3c, 0x79, 0x68, 0xbd, 0xf2, 0x45, 0x2e, 0x61, 0x8a,
    0x20, 0xc4, 0xb3, 0x41, 0xec, 0x4b, 0x34, 0xd8, 0x3c, 0x20, 0xf3, 0x78, 0xc4, 0x8a, 0x12, 0x89,
    0x75, 0xc5, 0x0d, 0xeb, 0x5b, 0x02, 0x65, 0xb6, 0x63, 0x4b, 0xf9, 0xc6, 0x57, 0x15, 0x13, 0xae,
    0xde, 0x75, 0x02, 0xbc, 0x15, 0x0f, 0xdd, 0x2a, 0x85, 0xd7, 0x9a, 0x0b, 0xb8, 0x97, 0xbe, 0x71,
};

static const MS_U8 rsa_d_256[256] = {
    0x0a, 0xe0, 0x83, 0x1b, 0x3f, 0x14, 0x04, 0x28, 0x25, 0xae, 0xff, 0x25, 0x65, 0xa8, 0xf7, 0x70,
    0xd1, 0xdf, 0x5c, 0xa1, 0x64, 0x11, 0x37, 0x9d, 0x98, 0x95, 0x9c, 0x1e, 0xe5, 0x3b, 0x83, 0xd5,
    0xd1, 0x8d, 0x24, 0x4b, 0x50, 0xa1, 0x34, 0x46, 0x87, 0x22, 0x70, 0xb5, 0x9c, 0x97, 0xbb, 0x2e,
    0x70, 0xe0, 0x8f, 0xe6, 0xf4, 0x83, 0x7a, 0x1c, 0x23, 0xf3, 0x06, 0x29, 0x2b, 0xca, 0x01, 0x20,
    0xcd, 0xc2, 0x25, 0xc7, 0x75, 0x64, 0x72, 0x12, 0x18, 0x9a, 0x34, 0xf6, 0x51, 0x12, 0xe0, 0xe1,
    0xc9, 0x50, 0x76, 0x50, 0x7c, 0x50, 0xb9, 0xe8, 0x33, 0x49, 0xdc, 0x5d, 0x7e, 0x49, 0x99, 0xc4,
    0xce, 0x6d, 0xdd, 0xcf, 0xdd, 0x27, 0x76, 0x5f, 0x5e, 0x65, 0x31, 0x72, 0xc2, 0xcb, 0xac, 0x1b,
    0x95, 0x70, 0x19, 0x92, 0xd8, 0x29, 0x96, 0xe0, 0xe0, 0x9b, 0xe3, 0x22, 0xac, 0x59, 0x17, 0x82,
    0xb1, 0x60, 0x4e, 0x46, 0x4c, 0x5f, 0xfd, 0x0e, 0x9a, 0xe0, 0xc0, 0x8f, 0xb5, 0xa9, 0x0a, 0x9a,
    0x53, 0x14, 0xb7, 0x85, 0xfa, 0x34, 0x82, 0x0c, 0x25, 0x3a, 0xdd, 0xbd, 0xbb, 0x7b, 0x56, 0x2f,
    0x22, 0x7d, 0x3e, 0xb4, 0x49, 0xd9, 0x7d, 0x6a, 0xd1, 0x8e, 0x0c, 0x82, 0xf3, 0xda, 0xe3, 0x1a,
    0x04, 0x7b, 0xc5, 0x41, 0xab, 0xd4, 0x3d, 0x2d, 0x68, 0xea, 0x95, 0x63, 0x93, 0xb7, 0xc1, 0xf4,
    0x05, 0x2c, 0x24, 0x57, 0xbe, 0x9a, 0x9f, 0x2d, 0x51, 0x41, 0xee, 0xed, 0xe2, 0x59, 0x64, 0xd1,
    0x28, 0x87, 0xd5, 0xe9, 0xf5, 0x19, 0xe2, 0x36, 0xa3, 0xea, 0x8d, 0xc8, 0xab, 0x2c, 0x0b, 0x4c,
    0x94, 0x23, 0x2e, 0xe3, 0x90, 0xd2, 0xa9, 0x47, 0xa6, 0x60, 0xda, 0xfa, 0x09, 0x62, 0xc8, 0xb5,
    0x42, 0xf0, 0x86, 0xc1, 0x29, 0x6c, 0x93, 0xc9, 0xd8, 0x44, 0xe3, 0x83, 0x6a, 0xc8, 0x55, 0x21,
};

static const MS_U8 rsa_e_256[4] = {
    0x00, 0x01, 0x00, 0x01
};

static const MS_U8 rsa_n_256[256] = {
    0x94, 0x6a, 0xb4, 0xac, 0x98, 0x66, 0x15, 0x02, 0x48, 0x8c, 0x8b, 0x07, 0x06, 0x77, 0xfb, 0xd4,
    0xca, 0x56, 0x28, 0xde, 0x31, 0x11, 0x84, 0xd0, 0x4e, 0xa0, 0xe5, 0xa4, 0xb7, 0x44, 0x70, 0xda,
    0x9a, 0x9f, 0x09, 0xfd, 0xcf, 0x86, 0x4f, 0x9d, 0xae, 0xed, 0xd0, 0xfd, 0xfd, 0x9e, 0xa9, 0x80,
    0xd4, 0x4b, 0x4d, 0x19, 0x72, 0x3e, 0xb6, 0xcc, 0x76, 0x48, 0x11, 0x47, 0x94, 0x4a, 0x3e, 0x15,
    0x94, 0x99, 0x0d, 0x92, 0x0d, 0x1b, 0x50, 0x75, 0x67, 0x0b, 0x8a, 0xfb, 0x26, 0xd0, 0x14, 0xb7,
    0x41, 0x84, 0xdb, 0x6e, 0xf3, 0x6e, 0xb7, 0x4b, 0x7b, 0xb6, 0x6d, 0x71, 0x9d, 0xfc, 0x7c, 0x35,
    0xf3, 0x3e, 0x6d, 0x5b, 0x76, 0x06, 0x0d, 0x0c, 0xac, 0x4b, 0xad, 0x7d, 0x36, 0xf9, 0xb4, 0x98,
    0x18, 0x13, 0x22, 0xa4, 0x5c, 0xfa, 0x75, 0x7f, 0x85, 0x06, 0x38, 0xf4, 0x4c, 0xbd, 0x50, 0x6c,
    0x98, 0xd2, 0x92, 0x7d, 0xfa, 0xc0, 0x0b, 0x53, 0x83, 0xbf, 0xf6, 0xbb, 0xb8, 0xbd, 0x25, 0xf2,
    0x95, 0xc6, 0x29, 0xcd, 0x45, 0xa6, 0x9e, 0x8d, 0x45, 0xa3, 0x85, 0x11, 0xaa, 0x5e, 0x8f, 0x40,
    0xb7, 0xc8, 0x95, 0x10, 0x6a, 0x69, 0x62, 0x5a, 0x20, 0x09, 0x5b, 0xec, 0x3b, 0xe9, 0x46, 0xed,
    0x25, 0xd5, 0x5a, 0xfc, 0xb6, 0xa7, 0x9d, 0xe9, 0x60, 0xb8, 0x23, 0xc9, 0xff, 0xc6, 0xc6, 0x33,
    0xab, 0x51, 0xbf, 0x50, 0x2c, 0x5e, 0xd4, 0x9a, 0x3c, 0x3e, 0x1b, 0x0c, 0x41, 0x45, 0x7b, 0x02,
    0xff, 0x8a, 0x5b, 0x19, 0x1e, 0x06, 0x73, 0x3f, 0x30, 0x5c, 0x6d, 0x64, 0x6b, 0xdb, 0x39, 0x7f,
    0xcd, 0x77, 0x81, 0xb7, 0x75, 0xed, 0x71, 0x6a, 0xa6, 0xa2, 0x9d, 0x01, 0x6d, 0xef, 0x83, 0x17,
    0x49, 0x14, 0xe5, 0x10, 0x72, 0x48, 0x94, 0x23, 0x57, 0x36, 0x0f, 0x15, 0x32, 0x8e, 0x5a, 0xed,
};

static const MS_U8 rsa_a_128[128] = {
    0x02, 0x15, 0x35, 0x24, 0xc0, 0x89, 0x5e, 0x81, 0x84, 0x84, 0xd6, 0x09, 0xb1, 0xf0, 0x56, 0x63,
    0x06, 0xb9, 0x7b, 0x0d, 0x46, 0xdf, 0x99, 0x8d, 0xb2, 0xc2, 0x84, 0x65, 0x89, 0x37, 0x52, 0x12,
    0x00, 0xf3, 0xe3, 0x01, 0x06, 0xd7, 0xcd, 0x0d, 0x3b, 0x23, 0xf1, 0x76, 0x1e, 0x8d, 0xcd, 0x3d,
    0x76, 0xd4, 0x57, 0xed, 0x46, 0x2d, 0xf7, 0x8c, 0x7c, 0xfd, 0xe9, 0xf9, 0xe3, 0x37, 0x24, 0xc6,
    0xe2, 0xf7, 0x84, 0xc5, 0xd5, 0x13, 0xd2, 0xaa, 0x72, 0xaf, 0xf7, 0xe5, 0xbb, 0xd2, 0x72, 0x77,
    0x89, 0x32, 0xd6, 0x12, 0x47, 0xec, 0xdb, 0x8f, 0x79, 0x30, 0x69, 0xf2, 0xe7, 0x76, 0x96, 0xce,
    0xf4, 0x00, 0x7a, 0xe8, 0xe2, 0xca, 0x4e, 0xc5, 0x2e, 0x58, 0x49, 0x5c, 0xde, 0x8e, 0x28, 0xbd,
    0x96, 0xab, 0x58, 0x2d, 0xb2, 0xa7, 0x26, 0x65, 0xb1, 0xef, 0x62, 0x63, 0x05, 0x73, 0x87, 0x0a,
};

static const MS_U8 rsa_d_128[128] = {
    0x1a, 0x15, 0xb2, 0x45, 0x65, 0x70, 0xf2, 0x6b, 0x34, 0x4a, 0xe9, 0xb5, 0x8d, 0x73, 0x45, 0x2a,
    0x8d, 0xac, 0x14, 0x7f, 0xc6, 0x03, 0xd3, 0x18, 0x8f, 0x67, 0xe3, 0xe8, 0xcf, 0xd4, 0xdd, 0x0d,
    0x4f, 0xa5, 0x63, 0x3d, 0x89, 0x74, 0xfa, 0x04, 0x33, 0x99, 0x44, 0xf5, 0xfb, 0x10, 0xeb, 0x65,
    0x84, 0xd0, 0x7d, 0x55, 0xd8, 0xf3, 0x27, 0xed, 0x3e, 0x44, 0xb9, 0xc8, 0x43, 0x29, 0x36, 0x51,
    0x93, 0x34, 0x65, 0x48, 0x4b, 0xe1, 0xf9, 0xe7, 0x43, 0x99, 0xcf, 0x81, 0x06, 0x6a, 0xd9, 0xb6,
    0x0f, 0x20, 0xd2, 0x9f, 0x9c, 0x52, 0x53, 0xea, 0x7c, 0xfe, 0x12, 0x0d, 0xf0, 0xf1, 0x7b, 0xff,
    0xea, 0x26, 0xf2, 0x73, 0xd3, 0x99, 0x68, 0x89, 0xdc, 0x0a, 0x5e, 0xe1, 0x5b, 0x72, 0xac, 0x6e,
    0xcd, 0x0e, 0x9e, 0x88, 0xd9, 0x57, 0x79, 0x69, 0x7f, 0x7d, 0x95, 0xd8, 0x07, 0xc4, 0xec, 0x61,
};

static const MS_U8 rsa_e_128[4] = {
    0x00, 0x01, 0x00, 0x01
};

static const MS_U8 rsa_n_128[128] = {
    0x8e, 0xbe, 0xc4, 0x8e, 0x1a, 0x21, 0x00, 0x6e, 0x45, 0xe6, 0xfd, 0x71, 0xdf, 0xb2, 0x6b, 0x01,
    0x41, 0xfa, 0xea, 0x89, 0x08, 0x1d, 0x44, 0x27, 0xba, 0x3c, 0xf3, 0x30, 0x79, 0xb5, 0x5f, 0xa3,
    0xed, 0x3f, 0x29, 0xf5, 0xab, 0x3b, 0xc1, 0x9b, 0x65, 0x5a, 0xa5, 0x85, 0x37, 0x62, 0x20, 0x49,
    0x89, 0xca, 0x59, 0x66, 0xe9, 0x1b, 0x35, 0x88, 0xc4, 0xb3, 0xbb, 0x31, 0x3b, 0x86, 0x0b, 0x14,
    0x81, 0x86, 0xee, 0xac, 0x29, 0xd6, 0x79, 0x0e, 0x50, 0x5d, 0x36, 0x41, 0x3c, 0x2e, 0x4a, 0x4f,
    0xb4, 0x33, 0x69, 0xd5, 0x94, 0x00, 0x72, 0x67, 0x9c, 0x82, 0x76, 0x9d, 0x46, 0xc9, 0xe3, 0x96,
    0xbe, 0x38, 0xe0, 0x10, 0xa5, 0x3b, 0x86, 0xe7, 0x6d, 0xf3, 0x19, 0xae, 0x11, 0xb2, 0x55, 0x54,
    0xaf, 0xb0, 0xf2, 0x3e, 0xb4, 0x24, 0xab, 0x5d, 0x78, 0x53, 0xdf, 0x4b, 0xb5, 0x04, 0xb7, 0x05,
};

static const MS_U8 rsa_a_32[32] = {
    0x02, 0x15, 0x35, 0x24, 0xc0, 0x89, 0x5e, 0x81, 0x84, 0x84, 0xd6, 0x09, 0xb1, 0xf0, 0x56, 0x63,
    0x06, 0xb9, 0x7b, 0x0d, 0x46, 0xdf, 0x99, 0x8d, 0xb2, 0xc2, 0x84, 0x65, 0x89, 0x37, 0x52, 0x12,
};

static const MS_U8 rsa_d_32[32] = {
    0x8d, 0x6e, 0xb7, 0x9b, 0xb4, 0xa7, 0x1f, 0x63, 0xa2, 0x51, 0x6c, 0x75, 0x69, 0xfe, 0x98, 0x9f,
    0x38, 0xec, 0x3a, 0x57, 0x85, 0x2f, 0xc8, 0x17, 0xa0, 0x11, 0x29, 0x19, 0xe9, 0xee, 0x2d, 0xe1,
};

static const MS_U8 rsa_e_32[4] = {
    0x00, 0x01, 0x00, 0x01
};

static const MS_U8 rsa_n_32[32] = {
    0x8f, 0x81, 0x91, 0xd7, 0x5a, 0x0d, 0x9a, 0x4e, 0xf9, 0x8b, 0x33, 0x29, 0xe7, 0x4c, 0xbd, 0x13,
    0x03, 0x4c, 0x97, 0xd4, 0x13, 0xff, 0x43, 0xce, 0x76, 0xfa, 0xe4, 0x08, 0xa0, 0xed, 0xdd, 0xa9
};

//--------------------------------------------------------------------------------------------------
// Local Defines
//--------------------------------------------------------------------------------------------------
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

static void _Demo_RSA_ByteSwap(MS_U32 *pu32RSAOut)
{
    MS_U8 *pu8Data;
    MS_U32 u32i;

    for( u32i=0 ; u32i<64 ; u32i++ )
    {
        pu8Data = (MS_U8 *)(&pu32RSAOut[u32i]);
        pu32RSAOut[u32i] = ((MS_U32) pu8Data[0])<<24 |((MS_U32) pu8Data[1])<<16 | ((MS_U32) pu8Data[2])<<8 |(MS_U32) pu8Data[3];
    }
}

static void _Demo_RSA_Swap(MS_U32 *pu32RSAOut, MS_U32 u32Size)
{
    MS_U32 u32TmpArray[64]= {0}, u32i;

    memset( u32TmpArray, 0, 64*4);
    for( u32i=0 ; u32i<u32Size ; u32i++ )
    {
        u32TmpArray[(u32Size-1)-u32i]= pu32RSAOut[u32i];
    }
    memcpy( pu32RSAOut, u32TmpArray, 64*4);
}

static MS_BOOL _Demo_RSA_DEMO_Routine(const MS_U8 *pu8RSAIn, MS_U32 u32RSAInLen, const MS_U8 *pu8KeyE, MS_U32 u32KeyELen, const MS_U8 *pu8KeyN, MS_U32 u32KeyNLen, MS_U8 *pu8RSAOut, DrvAESDMA_RSAMode eMode)
{
    DrvAESDMA_RSASig stSig;
    DrvAESDMA_RSAKey stKey;
    DrvAESDMA_RSAOut *stRSAOut = (DrvAESDMA_RSAOut *) pu8RSAOut;
    MS_U32 u32OffsetE, u32OffsetN, u32OffsetA, u32KeyLen= 0;

    memset(&stSig, 0, sizeof(DrvAESDMA_RSASig));
    memset(&stKey, 0, sizeof(DrvAESDMA_RSAKey));
    memset(stRSAOut, 0, sizeof(DrvAESDMA_RSAOut));

    switch (eMode)
    {
    case E_DRVAESDMA_RSA1024_PUBLIC:
    case E_DRVAESDMA_RSA1024_PRIVATE:
        u32KeyLen = 128;
        break;
    case E_DRVAESDMA_RSA2048_PUBLIC:
    case E_DRVAESDMA_RSA2048_PRIVATE:
        u32KeyLen = 256;
        break;
    case E_DRVAESDMA_RSA256_PUBLIC:
    case E_DRVAESDMA_RSA256_PRIVATE:
        u32KeyLen = 32;
        break;
    default:
        return DRVAESDMA_INVALID_PARAM;
    }
    /// LSB Input
    u32OffsetE = (sizeof(stKey.u32KeyE)-u32KeyELen) / sizeof(MS_U32);
    memcpy( &(stKey.u32KeyE[u32OffsetE]), pu8KeyE, u32KeyELen);
    _Demo_RSA_Swap( stKey.u32KeyE, 64);

    /// MSB Input
    u32OffsetN = (sizeof(stKey.u32KeyN)-u32KeyNLen) / sizeof(MS_U32);
    memcpy( &stKey.u32KeyN[u32OffsetN], pu8KeyN, u32KeyNLen);

    /// MSB Input
    u32OffsetA = (sizeof(stSig.u32Sig)-u32RSAInLen) / sizeof(MS_U32);
    memcpy( &stSig.u32Sig[u32OffsetA], pu8RSAIn , u32RSAInLen);


    if( DRVAESDMA_OK!=MDrv_RSA_Calculate(&stSig, &stKey, eMode) )
    {
        printf("Error: MDrv_RSA_Calculate failed\n");
        return DRVAESDMA_FAIL;
    }
    while(MDrv_RSA_IsFinished()!=DRVAESDMA_OK)
    {
        ;
    }

    MDrv_RSA_Output(eMode, stRSAOut);
    _Demo_RSA_Swap(stRSAOut->u32RSAOut, (u32KeyLen/4));
    _Demo_RSA_ByteSwap(stRSAOut->u32RSAOut);

    return DRVAESDMA_OK;
}

static MS_BOOL Demo_RSA_DEMO_256(void)
{
    MS_S32   NON_CACHE_POOL_ID = INVALID_POOL_ID;
    MS_U32   u32Size           = 0;
    MS_U8    *pu8Buf           = NULL;
    MS_U8    *pu8Cipher        = NULL;
    MS_U8    *pu8Result        = NULL;
    MS_BOOL  ret               = DRVAESDMA_FAIL;

    // Memory Allocation
    if( TRUE!=Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&NON_CACHE_POOL_ID) ){
        printf("Error: Demo_Util_GetSystemPoolID() failed\n");
        return DRVAESDMA_FAIL;
    }

    u32Size   = sizeof(rsa_a_256);
    pu8Buf    = MsOS_AllocateMemory( u32Size*2, NON_CACHE_POOL_ID);
    if( NULL==pu8Buf ){
        printf("Error: MsOS_AllocateMemory() failed\n");
        return DRVAESDMA_FAIL;
    }
    pu8Cipher = pu8Buf;
    pu8Result = pu8Buf + u32Size;


    printf("\nDemo_RSA_DEMO_256==>\n");
    printf("PlainText:\n");  _DumpBuf(rsa_a_256, 256);

    ret = _Demo_RSA_DEMO_Routine( rsa_a_256, 256, rsa_e_256, 4, rsa_n_256, 256, pu8Cipher, E_DRVAESDMA_RSA2048_PUBLIC);
    if(ret == DRVAESDMA_FAIL)
    {
        printf("RSA Encryption Fail!!!\n");
        return DRVAESDMA_FAIL;
    }
    printf("CipherText:\n");  _DumpBuf(pu8Cipher, 256);

    ret = _Demo_RSA_DEMO_Routine( pu8Cipher, 256, rsa_d_256, 256, rsa_n_256, 256, pu8Result, E_DRVAESDMA_RSA2048_PRIVATE);
    if(ret == DRVAESDMA_FAIL)
    {
        printf("RSA Decryption Fail!!!\n");
        return DRVAESDMA_FAIL;
    }
    printf("Result:\n");  _DumpBuf(pu8Result, 256);

    if( memcmp(pu8Result, rsa_a_256, u32Size) == 0 )
    {
        printf("RSA Demo (256-byte mode) Success!\n");
    }
    else
    {
        printf("RSA Demo (256-byte mode) Failed!!!\n");
    }

    // Free memory
    MsOS_FreeMemory( pu8Buf, NON_CACHE_POOL_ID);

    return DRVAESDMA_OK;
}

static MS_BOOL Demo_RSA_DEMO_128(void)
{
    MS_S32   NON_CACHE_POOL_ID = INVALID_POOL_ID;
    MS_U32   u32Size           = 0;
    MS_U8    *pu8Buf           = NULL;
    MS_U8    *pu8Cipher        = NULL;
    MS_U8    *pu8Result        = NULL;
    MS_BOOL  ret               = DRVAESDMA_FAIL;

    // Memory Allocation
    if( TRUE!=Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&NON_CACHE_POOL_ID) ){
        printf("Error: Demo_Util_GetSystemPoolID() failed\n");
        return DRVAESDMA_FAIL;
    }

    u32Size   = sizeof(rsa_a_128);
    pu8Buf    = MsOS_AllocateMemory( u32Size*2, NON_CACHE_POOL_ID);
    if( NULL==pu8Buf ){
        printf("Error: MsOS_AllocateMemory() failed\n");
        return DRVAESDMA_FAIL;
    }
    pu8Cipher = pu8Buf;
    pu8Result = pu8Buf + u32Size;


    printf("\nDemo_RSA_DEMO_128==>\n");
    printf("PlainTest:\n");  _DumpBuf(rsa_a_128, 128);

    ret = _Demo_RSA_DEMO_Routine( rsa_a_128, 128, rsa_e_128, 4, rsa_n_128, 128, pu8Cipher, E_DRVAESDMA_RSA1024_PUBLIC);
    if(ret == DRVAESDMA_FAIL)
    {
        printf("RSA Encryption Fail!!!\n");
        return DRVAESDMA_FAIL;
    }
    printf("CipherText:\n");  _DumpBuf(pu8Cipher, 128);

    ret = _Demo_RSA_DEMO_Routine( pu8Cipher, 128, rsa_d_128, 128, rsa_n_128, 128, pu8Result, E_DRVAESDMA_RSA1024_PRIVATE);
    if(ret == DRVAESDMA_FAIL)
    {
        printf("RSA Decryption Fail!!!\n");
        return DRVAESDMA_FAIL;
    }
    printf("Result:\n");  _DumpBuf(pu8Result, 128);

    if( memcmp(pu8Result, rsa_a_128, u32Size) == 0 )
    {
        printf("RSA Demo (128-byte mode) Success!\n");
    }
    else
    {
        printf("RSA Demo (128-byte mode) Failed!!!\n");
    }

    // Free memory
    MsOS_FreeMemory( pu8Buf, NON_CACHE_POOL_ID);

    return DRVAESDMA_OK;
}

static MS_BOOL Demo_RSA_DEMO_32(void)
{
    MS_S32   NON_CACHE_POOL_ID = INVALID_POOL_ID;
    MS_U32   u32Size           = 0;
    MS_U8    *pu8Buf           = NULL;
    MS_U8    *pu8Cipher        = NULL;
    MS_U8    *pu8Result        = NULL;
    MS_BOOL  ret               = DRVAESDMA_FAIL;

    // Memory Allocation
    if( TRUE!=Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&NON_CACHE_POOL_ID) ){
        printf("Error: Demo_Util_GetSystemPoolID() failed\n");
        return DRVAESDMA_FAIL;
    }

    u32Size   = sizeof(rsa_a_32);
    pu8Buf    = MsOS_AllocateMemory( u32Size*2, NON_CACHE_POOL_ID);
    if( NULL==pu8Buf ){
        printf("Error: MsOS_AllocateMemory() failed\n");
        return DRVAESDMA_FAIL;
    }
    pu8Cipher = pu8Buf;
    pu8Result = pu8Buf + u32Size;


    printf("\nDemo_RSA_DEMO_32==>\n");
    printf("PlainTest:\n");  _DumpBuf(rsa_a_32, 32);

    ret = _Demo_RSA_DEMO_Routine( rsa_a_32, 32, rsa_e_32, 4, rsa_n_32, 32, pu8Cipher, E_DRVAESDMA_RSA256_PUBLIC);
    if(ret == DRVAESDMA_FAIL)
    {
        printf("RSA Encryption Fail!!!\n");
        return DRVAESDMA_FAIL;
    }
    printf("CipherText:\n");  _DumpBuf(pu8Cipher, 32);

    ret = _Demo_RSA_DEMO_Routine( pu8Cipher, 32, rsa_d_32, 32, rsa_n_32, 32, pu8Result, E_DRVAESDMA_RSA256_PRIVATE);
    if(ret == DRVAESDMA_FAIL)
    {
        printf("RSA Decryption Fail!!!\n");
        return DRVAESDMA_FAIL;
    }
    printf("Result:\n");  _DumpBuf(pu8Result, 32);

    if( memcmp(pu8Result, rsa_a_32, u32Size) == 0 )
    {
        printf("RSA Demo (32-byte mode) Success!\n");
    }
    else
    {
        printf("RSA Demo (32-byte mode) Failed!!!\n");
    }

    // Free memory
    MsOS_FreeMemory( pu8Buf, NON_CACHE_POOL_ID);

    return DRVAESDMA_OK;
}

MS_BOOL Demo_RSA_DoRSADemo(void)
{
    MS_BOOL  ret = DRVAESDMA_FAIL;

    // Do RSA_DEMO_256 (256-byte mode)
    ret = Demo_RSA_DEMO_256();
    if(ret == DRVAESDMA_FAIL)
    {
        printf("Error: Demo_RSA_DEMO_256() failed\n");
        return DRVAESDMA_FAIL;
    }

    // Do RSA_DEMO_128 (128-byte mode)
    ret = Demo_RSA_DEMO_128();
    if(ret == DRVAESDMA_FAIL)
    {
        printf("Error: Demo_RSA_DEMO_128() failed\n");
        return DRVAESDMA_FAIL;
    }

    // Do RSA_DEMO_32 (32-byte mode)
    ret = Demo_RSA_DEMO_32();
    if(ret == DRVAESDMA_FAIL)
    {
        printf("Error: Demo_RSA_DEMO_32() failed\n");
        return DRVAESDMA_FAIL;
    }

    return TRUE;
}



MS_BOOL Demo_RSA_RSADemo_HwKey_Pri(void)
{
    MS_S32            NON_CACHE_POOL_ID = INVALID_POOL_ID;
    MS_U32            u32Size           = 0;
    MS_U8             *pu8Buf           = NULL;
    DrvAESDMA_RSASig  stSig;
    DrvAESDMA_RSAOut  *stRSAOut         = NULL;
    MS_U32            u32OffsetA        = 0;
    MS_U32            u32KeyLen         = 0;

    // Memory Allocation
    if( TRUE!=Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&NON_CACHE_POOL_ID) ){
        printf("Error: Demo_Util_GetSystemPoolID() failed\n");
        return DRVAESDMA_FAIL;
    }

    u32Size   = sizeof(rsa_a_256);
    pu8Buf    = MsOS_AllocateMemory( u32Size, NON_CACHE_POOL_ID);
    if( NULL==pu8Buf ){
        printf("Error: MsOS_AllocateMemory() failed\n");
        return DRVAESDMA_FAIL;
    }
    stRSAOut  = (DrvAESDMA_RSAOut *)pu8Buf;
    u32KeyLen = sizeof(rsa_a_256);

    memset(&stSig, 0, sizeof(DrvAESDMA_RSASig));
    memset(stRSAOut, 0, sizeof(DrvAESDMA_RSAOut));


    /// MSB Input
    u32OffsetA = (sizeof(stSig.u32Sig)-sizeof(rsa_a_256)) / sizeof(MS_U32);
    memcpy( &stSig.u32Sig[u32OffsetA], rsa_a_256 , sizeof(rsa_a_256));

    if( DRVAESDMA_OK!=MDrv_RSA_Calculate_Hw_Key(&stSig, E_DRVAESDMA_RSA_HWKEY_PRIVATE) )
    {
        printf("Error: MDrv_RSA_Calculate_Hw_Key failed\n");
        return DRVAESDMA_FAIL;
    }
    while(MDrv_RSA_IsFinished()!=DRVAESDMA_OK)
    {
        ;
    }

    MDrv_RSA_Output(E_DRVAESDMA_RSA2048_PRIVATE, stRSAOut);
    _Demo_RSA_Swap(stRSAOut->u32RSAOut, (u32KeyLen/4));
    _Demo_RSA_ByteSwap(stRSAOut->u32RSAOut);


    printf("Demo_RSA_RSADemo_HwKey_Pri() ==>\n");  _DumpBuf(pu8Buf, 256);


    // Free memory
    MsOS_FreeMemory( pu8Buf, NON_CACHE_POOL_ID);

    return DRVAESDMA_OK;
}

MS_BOOL Demo_RSA_RSADemo_HwKey_Pub1(void)
{
    MS_S32            NON_CACHE_POOL_ID = INVALID_POOL_ID;
    MS_U32            u32Size           = 0;
    MS_U8             *pu8Buf           = NULL;
    DrvAESDMA_RSASig  stSig;
    DrvAESDMA_RSAOut  *stRSAOut         = NULL;
    MS_U32            u32OffsetA        = 0;
    MS_U32            u32KeyLen         = 0;

    // Memory Allocation
    if( TRUE!=Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&NON_CACHE_POOL_ID) ){
        printf("Error: Demo_Util_GetSystemPoolID() failed\n");
        return DRVAESDMA_FAIL;
    }

    u32Size   = sizeof(rsa_a_256);
    pu8Buf    = MsOS_AllocateMemory( u32Size, NON_CACHE_POOL_ID);
    if( NULL==pu8Buf ){
        printf("Error: MsOS_AllocateMemory() failed\n");
        return DRVAESDMA_FAIL;
    }
    stRSAOut  = (DrvAESDMA_RSAOut *)pu8Buf;
    u32KeyLen = sizeof(rsa_a_256);

    memset(&stSig, 0, sizeof(DrvAESDMA_RSASig));
    memset(stRSAOut, 0, sizeof(DrvAESDMA_RSAOut));

    /// MSB Input
    u32OffsetA = (sizeof(stSig.u32Sig)-sizeof(rsa_a_256)) / sizeof(MS_U32);
    memcpy( &stSig.u32Sig[u32OffsetA], rsa_a_256 , sizeof(rsa_a_256));

    if( DRVAESDMA_OK!=MDrv_RSA_Calculate_Hw_Key(&stSig, E_DRVAESDMA_RSA_HWKEY_PUBLIC1) )
    {
        printf("Error: MDrv_RSA_Calculate_Hw_Key failed\n");
        return DRVAESDMA_FAIL;
    }
    while(MDrv_RSA_IsFinished()!=DRVAESDMA_OK)
    {
        ;
    }

    MDrv_RSA_Output(E_DRVAESDMA_RSA2048_PUBLIC, stRSAOut);
    _Demo_RSA_Swap(stRSAOut->u32RSAOut, (u32KeyLen/4));
    _Demo_RSA_ByteSwap(stRSAOut->u32RSAOut);


    printf("Demo_RSA_RSADemo_HwKey_Pub1() ==>\n");  _DumpBuf(pu8Buf, 256);


    // Free memory
    MsOS_FreeMemory( pu8Buf, NON_CACHE_POOL_ID);

    return DRVAESDMA_OK;
}

MS_BOOL Demo_RSA_RSADemo_HwKey_Pub2(void)
{
    MS_S32            NON_CACHE_POOL_ID = INVALID_POOL_ID;
    MS_U32            u32Size           = 0;
    MS_U8             *pu8Buf           = NULL;
    DrvAESDMA_RSASig  stSig;
    DrvAESDMA_RSAOut  *stRSAOut         = NULL;
    MS_U32            u32OffsetA        = 0;
    MS_U32            u32KeyLen         = 0;

    // Memory Allocation
    if( TRUE!=Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&NON_CACHE_POOL_ID) ){
        printf("Error: Demo_Util_GetSystemPoolID() failed\n");
        return DRVAESDMA_FAIL;
    }

    u32Size   = sizeof(rsa_a_256);
    pu8Buf    = MsOS_AllocateMemory( u32Size, NON_CACHE_POOL_ID);
    if( NULL==pu8Buf ){
        printf("Error: MsOS_AllocateMemory() failed\n");
        return DRVAESDMA_FAIL;
    }
    stRSAOut  = (DrvAESDMA_RSAOut *)pu8Buf;
    u32KeyLen = sizeof(rsa_a_256);

    memset(&stSig, 0, sizeof(DrvAESDMA_RSASig));
    memset(stRSAOut, 0, sizeof(DrvAESDMA_RSAOut));

    /// MSB Input
    u32OffsetA = (sizeof(stSig.u32Sig)-sizeof(rsa_a_256)) / sizeof(MS_U32);
    memcpy( &stSig.u32Sig[u32OffsetA], rsa_a_256 , sizeof(rsa_a_256));

    if( DRVAESDMA_OK!=MDrv_RSA_Calculate_Hw_Key(&stSig, E_DRVAESDMA_RSA_HWKEY_PUBLIC2) )
    {
        printf("Error: MDrv_RSA_Calculate_Hw_Key failed\n");
        return DRVAESDMA_FAIL;
    }
    while(MDrv_RSA_IsFinished()!=DRVAESDMA_OK)
    {
        ;
    }

    MDrv_RSA_Output(E_DRVAESDMA_RSA2048_PUBLIC, stRSAOut);
    _Demo_RSA_Swap(stRSAOut->u32RSAOut, (u32KeyLen/4));
    _Demo_RSA_ByteSwap(stRSAOut->u32RSAOut);


    printf("Demo_RSA_RSADemo_HwKey_Pub2() ==>\n");  _DumpBuf(pu8Buf, 256);


    // Free memory
    MsOS_FreeMemory( pu8Buf, NON_CACHE_POOL_ID);

    return DRVAESDMA_OK;
}

MS_BOOL Demo_RSA_DoRSADemo_HwKey(void)
{
    //
    // Note:
    //     This demo APP will use both OTP_RSA_1 and OTP_RSA_2 keys as below:
    //     Demo_RSA_RSADemo_HwKey_Pri() : calculate A exp OTP_RSA_1 mod OTP_RSA_2
    //     Demo_RSA_RSADemo_HwKey_Pub1(): calculate A exp 65537 mod OTP_RSA_1
    //     Demo_RSA_RSADemo_HwKey_Pub2(): calculate A exp 65537mod OTP_RSA_2
    //
    //     Since OTP_RSA_1 and OTP_RSA_2 are uncertain, the program will show the claculation result only.
    //

    MS_BOOL ret = DRVAESDMA_FAIL;

    // Do Demo_RSA_RSADemo_HwKey_Pri
    ret = Demo_RSA_RSADemo_HwKey_Pri();
    if(ret == DRVAESDMA_FAIL)
    {
        printf("Error: Demo_RSA_RSADemo_HwKey_Pri() failed\n");
        return DRVAESDMA_FAIL;
    }

    // Demo_RSA_RSADemo_HwKey_Pub1
    ret = Demo_RSA_RSADemo_HwKey_Pub1();
    if(ret == DRVAESDMA_FAIL)
    {
        printf("Error: Demo_RSA_RSADemo_HwKey_Pub1() failed\n");
        return DRVAESDMA_FAIL;
    }

    // Demo_RSA_RSADemo_HwKey_Pub2
    ret = Demo_RSA_RSADemo_HwKey_Pub2();
    if(ret == DRVAESDMA_FAIL)
    {
        printf("Error: Demo_RSA_RSADemo_HwKey_Pub2() failed\n");
        return DRVAESDMA_FAIL;
    }

    return TRUE;
}

#endif
