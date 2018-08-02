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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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


/* INPUT_SW_PROJECT
Chakra      -   01
POLLUX      -   02
ARCHIMEDES  -   03
Chakra2     ¡V   04
OBAMA       ¡V   05
*/

/* INPUT_PRODUCT_TYPE
TV-01
STB-02
*/

/* INPUT_TV_SYSTEM
ATSC-01
DVBT-02
DVBC-03
DVBS-04
DMBT-05
ATV Only-06
*/

//**************************************************************************
//********************** Customer Info Input Area **************************
//**************************************************************************
//Customer ID (Example:0009 = 0x0009) (Example: 20= 0x0014)
#define INPUT_CUSTOMER_ID_LOW_BYTE  0x66
#define INPUT_CUSTOMER_ID_HIGH_BYTE 0x66

//Model ID (Example:0001 = 0x0001)
#define INPUT_MODEL_ID_LOW_BYTE     0x01
#define INPUT_MODEL_ID_HIGH_BYTE    0x00

//Chip ID (Example:000B = 0x000B)
#define INPUT_CHIP_ID_LOW_BYTE      0x66
#define INPUT_CHIP_ID_HIGH_BYTE     0x66

#define INPUT_SW_PROJECT            0x04
#define INPUT_SW_GENERATION         0x01
#define INPUT_PRODUCT_TYPE          0x01
#define INPUT_TV_SYSTEM             0x01

//Label (Example:00000456 = 0x0001C8)
#define INPUT_LABEL_LOW_BYTE        0x02
#define INPUT_LABEL_MIDDLE_BYTE     0x00
#define INPUT_LABEL_HIGH_BYTE       0x00

//CL(Change-List) (Example:00101234 = 0x18B72)
#define INPUT_CL_LOW_BYTE           0xAC
#define INPUT_CL_MIDDLE_BYTE        0x06
#define INPUT_CL_HIGH_BYTE          0x02



//  SW Quality
//  01-BOOTLEG
//  02-Demo
//  03-Pre-Alpha
//  04-Alpha
//  05-Beta
//  06-RC
//  07-RTM
//  Quality flag can be modified by release engineer only

#define INPUT_RELEASE_QUALITY       0x01

//CPU TYPE
//01-MIPS
//02-AEON
#define  INPUT_CPU_TYPE 0x01

#define code
//Customer IP
unsigned char code IP_Cntrol_Mapping_1[8] = "00000000"; //Customer IP Control-1
unsigned char code IP_Cntrol_Mapping_2[8] = "00000000"; //Customer IP Control-2
unsigned char code IP_Cntrol_Mapping_3[8] = "00000000"; //Customer IP Control-3
unsigned char code IP_Cntrol_Mapping_4[8] = "08F80000"; //Customer IP Control-4


MS_U8 Customer_hash[] = {0x1a,0x30,0x4f,0x0a,0x3a,0x42,0xd9,0x4b,0x08,0xd5,0x74,0xa2,0xfd,0xf1,0x69,0x3f};


//**************************************************************************
//**************************************************************************
//**************************************************************************





unsigned char code CID_Buf[32] =  {
//Fix Value: Do not Modify
        'M', 'S', 'I', 'F',         // (Do not modify)Mstar Information:MSIF
        '0', '1',                   // (Do not modifyCustomer Info Class: 01

//Customer Info area
        INPUT_CUSTOMER_ID_LOW_BYTE,
        INPUT_CUSTOMER_ID_HIGH_BYTE,

        INPUT_MODEL_ID_LOW_BYTE,
        INPUT_MODEL_ID_HIGH_BYTE,

        INPUT_CHIP_ID_LOW_BYTE,
        INPUT_CHIP_ID_HIGH_BYTE,

        INPUT_SW_PROJECT,
        INPUT_SW_GENERATION,
        INPUT_PRODUCT_TYPE,
        INPUT_TV_SYSTEM,

        INPUT_LABEL_LOW_BYTE,
        INPUT_LABEL_MIDDLE_BYTE,
        INPUT_LABEL_HIGH_BYTE,

        INPUT_CL_LOW_BYTE,
        INPUT_CL_MIDDLE_BYTE,
        INPUT_CL_HIGH_BYTE,

        INPUT_RELEASE_QUALITY,

        INPUT_CPU_TYPE,
//Reserve
        '0', '0', '0', '0', '0', '0', '0', '0'   // Reserve
        };




