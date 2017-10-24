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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvIR.h
/// @brief  IR Driver Interface
/// @author MStar Semiconductor Inc.
///
/// Driver to initialize and access IR.
///     - Provide functions to initialize IR timing, and enable IR interrupt.
///     - Provide IR ISR.
///     - Provide IR callback function registration for AP.
///     - Provide function to get IR key.
///
/// \b [Example]
/// @code
///
/// // Initalize the IR in the boot time.
/// MDrv_IR_Init();
///
/// // *****************************************************************************
///
/// // Set the delay time of IR. First repeat key code is sent after one second.
/// // The following repeat key code is sent after 0.5 seconds.
/// MDrv_IR_SetDelayTime(1000, 500);
///
/// // Please refer to the following diagram. Assume that when users press and hold
/// // IR button, the repeat key is sent every 200ms.
/// // The 1st press is sent, and the return repeat code is 0.
/// // The 5th repeat key is sent because of the 1st delay time is 1000ms.
/// // The 8th repeat key is sent because of the 2nd delay time is 500ms, and
/// // the time between the 5th and the 8th repeat key is 600ms which is greater
/// // than 500ms.
/// // Note: Do not support RELEASE event.
///
/// @endcode
///
/// @image html IR_delay.JPG "IR delay time"
///
/// @code
/// // *****************************************************************************
///
/// // Set the callback function. The function MApi_IR_SetCallback is called if
/// // the IR interrupt is generated and the delay time setting is matched.
/// void MApi_IR_SetCallback(U8 *pu8Key, U8 *pu8Flg);
///
/// MDrv_IR_Set_Callback(MApi_IR_SetCallback);
///
/// // *****************************************************************************
///
/// // Polling & get the IR key directly. Users can call the MDrv_IR_GetKey to get
/// // the IR key if it returns TRUE.
/// U8 u8Key, u8Flg;
///
/// MDrv_IR_GetKey(&u8Key, &u8Flg);
///
/// @endcode
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_IR_H_
#define _MDRV_IR_H_

//#include <asm/types.h>
#include "mdrv_ir_st.h"
//#include "mdrv_ir_io.h"
#include "mdrv_types.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define IR_TYPE_FULLDECODE_MODE 1
#define IR_TYPE_RAWDATA_MODE    2
#define IR_TYPE_SWDECODE_MODE   3
#define IR_TYPE_HWRC_MODE       4


typedef enum
{
    IR_DECMODE_HWFULL = 0,
    IR_DECMODE_HWRAW =1,
    IR_DECMODE_HWRC5,
    IR_DECMODE_HWRC5X,
    IR_DECMODE_HWRC6,
    IR_DECMODE_SW,
    IR_DECMODE_SHOT,

} IR_DECODE_MODE;

///IR data sequence format select for NEC-like (PPM modulation) formats.
//Note:
//S = System Code.
//C = Customer Code Bits, (ex: C8= customer code 8bits).
//D = Data (Key) Code Bits, (ex: D8= data code 8bits).
//P = Format with Parity Check (ex: 3th byte and 4th byte of NEC format).
typedef enum
{
    IR_XFM_NOTDEF = 0,      //Format not define
    IR_XFM_C16D8D8P=1,      //ex: NEC, Toshiba format
    IR_XFM_C8D8=2,          //ex: Mitsubushi, Konka format
    IR_XFM_C4D8C4D8P=3,	    //ex: RCA format
    IR_XFM_C26D8D8P=4,
    IR_XFM_C32D8D8P=5,
    IR_XFM_C5D6C5D6P=6,
    IR_XFM_C6D6C6D6P=7,
    IR_XFM_D7C6=8,          //ex: Sony-D7C6
    IR_XFM_D7C8=9,          //ex: Sony-D7C8
    IR_XFM_D8C6=10,         //ex: Sony-D8C6
    IR_XFM_D5_only=11,      //ex: MV500
    IR_XFM_S1C4D6=12,       //ex: IRT1250
    IR_XFM_C5D6D4=13,       //ex: LR3715M
    IR_XFM_R1T1C3D6=14,     //ex: M3004 LAB1-Carrier
    IR_XFM_RESERVED=15,     //Reserved

} IR_EXT_FORMAT;

/// emurate IR fucntion return result
typedef enum
{
    /// IR result for failure
    E_IR_FAIL =0,
    /// IR result for OK
    E_IR_OK = 1,

} IR_Result;

/// Callback function which is called in IR ISR.
typedef void ( *IR_Callback ) (MS_U8 u8Key, MS_U8 u8RepeatFlag);
typedef void ( *IR_RawModCallback ) (MS_U8* pu8RawData, MS_U8 u8RepeatFlag);
typedef void ( *IR_RawModCallback64 ) (unsigned long long u64RawData);
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void MDrv_IR_HK_Init(void);

//-------------------------------------------------------------------------------------------------
/// IR get key code function.
/// @param pu8Key \b OUT: Get IR key code
/// @param pu8Repeat \b OUT: Get IR key repeat status
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_HK_GetKeyCode(MS_U8 *pu8Key, MS_U8 *pu8Repeat);


//-------------------------------------------------------------------------------------------------
/// Set IR callback function when receive IR key. Support only one callback. If call it twice,
/// the first callback does not be called.
/// Note: The callback runs at interrupt mode.
/// @param pCallback \b IN: Set the callback function when generate IR interrupt.
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_HK_SetCallback(IR_Callback pCallback);
//-------------------------------------------------------------------------------------------------
/// Set IR callback function when receive IR Raw data. Support only one callback. If call it twice,
/// the first callback does not be called.
/// Note: The callback runs at interrupt mode.
/// @param pCallback \b IN: Set the callback function when generate IR interrupt.
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_HK_SetRawModCallback(IR_RawModCallback pRawModCallback);
//-------------------------------------------------------------------------------------------------
/// Get IR callback function which receive IR key.
/// @return the callback function when generate IR interrupt
//-------------------------------------------------------------------------------------------------
IR_Callback MDrv_IR_HK_GetCallback(void);
//-------------------------------------------------------------------------------------------------
/// Get IR Raw Mode callback function which receive IR key.
/// @return the callback function when generate IR interrupt
//-------------------------------------------------------------------------------------------------
IR_RawModCallback MDrv_IR_HK_GetRowModCallback(void);
//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_HK_Enable(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Setting support protocol in muti-protocol mode
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_HK_SetProtocol(MS_MultiProtocolCfg *pstProtocolCfg);

//-------------------------------------------------------------------------------------------------
/// Set IR callback function when receive IR Raw data in multi-protocol mode.
/// Support only one callback. If call it twice, the first callback does not be called.
/// Note: The callback runs at interrupt mode.
/// @param pCallback \b IN: Set the callback function when generate IR interrupt.
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_HK_SetRawModCallback64(IR_RawModCallback64 pRawModCallback);

#ifdef DDI_MISC_INUSE
#if (IR_MODE_SEL==IR_TYPE_FULLDECODE_MODE)
void MDrv_IR_HK_SetDelayTime(MS_U32 u32_1stDelayTimeMs, MS_U32 u32_2ndDelayTimeMs);
#endif
#endif
#endif // _MDRV_IR_H_

