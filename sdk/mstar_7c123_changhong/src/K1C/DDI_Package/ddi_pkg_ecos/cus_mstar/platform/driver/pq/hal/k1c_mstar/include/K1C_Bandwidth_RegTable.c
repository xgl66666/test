//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. (" MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms (" Terms") and to
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
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (; MStar; Confidential; Information; ) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//


//****************************************************
// DDR1333MHz
//****************************************************
code U8 BWTABLE_COM_MIU_128[][REG_ADDR_SIZE+REG_BANK_SIZE+REG_MASK_SIZE+1] =
{//Reg          Bank  Mask  Value
 { REG(0x1240), 0xFF, 0xFF, 0x15 },
 { REG(0x1241), 0xFF, 0xFF, 0x80 },
 { REG(0x1242), 0xFF, 0xFF, 0x08 },
 { REG(0x1243), 0xFF, 0xFF, 0x20 },
 { REG(0x1244), 0xFF, 0xFF, 0x00 },
 { REG(0x1245), 0xFF, 0xFF, 0x04 },
 { REG(0x1248), 0xFF, 0xFF, 0xFF },
 { REG(0x1249), 0xFF, 0xFF, 0xFF },
 { REG(0x124A), 0xFF, 0xFF, 0x10 },//Same mark
 { REG(0x124B), 0xFF, 0xFF, 0x32 },//Same mark
 { REG(0x124C), 0xFF, 0xFF, 0x54 },//Same mark
 { REG(0x124D), 0xFF, 0xFF, 0x76 },//Same mark
 { REG(0x124E), 0xFF, 0xFF, 0x98 },//Same mark
 { REG(0x124F), 0xFF, 0xFF, 0xBA },//Same mark
 { REG(0x1250), 0xFF, 0xFF, 0xDC },//Same mark
 { REG(0x1251), 0xFF, 0xFF, 0xFE },//Same mark
 { REG(0x1260), 0xFF, 0xFF, 0x15 },
 { REG(0x1261), 0xFF, 0xFF, 0x80 },
 { REG(0x1262), 0xFF, 0xFF, 0x20 },
 { REG(0x1263), 0xFF, 0xFF, 0x20 },
 { REG(0x1264), 0xFF, 0xFF, 0x00 },
 { REG(0x1265), 0xFF, 0xFF, 0x04 },
 { REG(0x1268), 0xFF, 0xFF, 0xFF },
 { REG(0x1269), 0xFF, 0xFF, 0xFF },
 { REG(0x126A), 0xFF, 0xFF, 0x21 },//Same mark
 { REG(0x126B), 0xFF, 0xFF, 0x43 },//Same mark
 { REG(0x126C), 0xFF, 0xFF, 0x65 },//Same mark
 { REG(0x126D), 0xFF, 0xFF, 0x87 },//Same mark
 { REG(0x126E), 0xFF, 0xFF, 0xA9 },//Same mark
 { REG(0x126F), 0xFF, 0xFF, 0xCB },//Same mark
 { REG(0x1270), 0xFF, 0xFF, 0xED },//Same mark
 { REG(0x1271), 0xFF, 0xFF, 0x0F },//Same mark
 { REG(0x1278), 0xFF, 0xFF, 0xEF },
 { REG(0x1279), 0xFF, 0xFF, 0xBF },
 { REG(0x1280), 0xFF, 0xFF, 0x15 },
 { REG(0x1281), 0xFF, 0xFF, 0x80 },
 { REG(0x1282), 0xFF, 0xFF, 0x08 },
 { REG(0x1283), 0xFF, 0xFF, 0x20 },
 { REG(0x1284), 0xFF, 0xFF, 0x00 },
 { REG(0x1285), 0xFF, 0xFF, 0x04 },
 { REG(0x1288), 0xFF, 0xFF, 0xFF },
 { REG(0x1289), 0xFF, 0xFF, 0xFF },
 { REG(0x128A), 0xFF, 0xFF, 0x10 },//Same mark
 { REG(0x128B), 0xFF, 0xFF, 0x32 },//Same mark
 { REG(0x128C), 0xFF, 0xFF, 0x54 },//Same mark
 { REG(0x128D), 0xFF, 0xFF, 0x76 },//Same mark
 { REG(0x128E), 0xFF, 0xFF, 0x98 },//Same mark
 { REG(0x128F), 0xFF, 0xFF, 0xBA },//Same mark
 { REG(0x1290), 0xFF, 0xFF, 0xDC },//Same mark
 { REG(0x1291), 0xFF, 0xFF, 0xFE },//Same mark
 { REG(0x12A0), 0xFF, 0xFF, 0x15 },
 { REG(0x12A1), 0xFF, 0xFF, 0x80 },
 { REG(0x12A2), 0xFF, 0xFF, 0x08 },
 { REG(0x12A3), 0xFF, 0xFF, 0x20 },
 { REG(0x12A4), 0xFF, 0xFF, 0x00 },
 { REG(0x12A5), 0xFF, 0xFF, 0x04 },
 { REG(0x12A8), 0xFF, 0xFF, 0xFF },
 { REG(0x12A9), 0xFF, 0xFF, 0xFF },
 { REG(0x12AA), 0xFF, 0xFF, 0x10 },//Same mark
 { REG(0x12AB), 0xFF, 0xFF, 0x32 },//Same mark
 { REG(0x12AC), 0xFF, 0xFF, 0x54 },//Same mark
 { REG(0x12AD), 0xFF, 0xFF, 0x76 },//Same mark
 { REG(0x12AE), 0xFF, 0xFF, 0x98 },//Same mark
 { REG(0x12AF), 0xFF, 0xFF, 0xBA },//Same mark
 { REG(0x12B0), 0xFF, 0xFF, 0xDC },//Same mark
 { REG(0x12B1), 0xFF, 0xFF, 0xFE },//Same mark
 { REG(0x12FE), 0xFF, 0xFF, 0xE1 },
 { REG(0x12FF), 0xFF, 0xFF, 0x09 },
 { REG(0x15E0), 0xFF, 0xFF, 0x02 },
 { REG(0x15E1), 0xFF, 0xFF, 0x00 },
 { REG(0x15E2), 0xFF, 0xFF, 0x1E },
 { REG(0x15E3), 0xFF, 0xFF, 0x00 },
 { REG(0x15E8), 0xFF, 0xFF, 0x18 },
 { REG(0x15E9), 0xFF, 0xFF, 0x00 },
 { REG(0x15EA), 0xFF, 0xFF, 0x20 },
 { REG(0x15EB), 0xFF, 0xFF, 0x20 },
 { REG(0x15EC), 0xFF, 0xFF, 0x01 },
 { REG(0x15ED), 0xFF, 0xFF, 0x01 },
 { REG(0x15F8), 0xFF, 0xFF, 0xE1 },
 { REG(0x15F9), 0xFF, 0xFF, 0xFF },
 { REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 *BWTABLE_COM_MIU_128X128 = NULL;

code U8 BWTABLE[][REG_ADDR_SIZE+REG_BANK_SIZE+REG_MASK_SIZE+BWTABLE_NUMS]=
{//Reg          Bank  Mask  Value
 { REG(REG_TABLE_END), 0x00, 0x00 }
};


