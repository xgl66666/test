//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2015 MStar Semiconductor, Inc. All rights reserved.
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

#ifndef __HDCP1X_APP_H__
#define __HDCP1X_APP_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "hdcp1xCommon.h"

#ifndef DLL_PUBLIC
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#endif

//----------------------------------------------------------------
/// HDCP1xTx_APP_LoadKey()
/// Get current HDCP2.2 RX status
/// @pu8Hdcp1xKey       \bIN:pointer of hdcp1x encrypted key
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
DLL_PUBLIC int HDCP1xTx_APP_LoadKey(unsigned char* pu8Hdcp1xKey, int nKeySize, unsigned char bIsKmNewMode);

//----------------------------------------------------------------
/// HDCP1xTx_APP_LoadExternalAn()
/// Load External An content
/// @pu8An       \bIN:pointer of external AN
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
DLL_PUBLIC int HDCP1xTx_APP_LoadExternalAn(unsigned char* pu8An);

//----------------------------------------------------------------
/// HDCP1xTx_APP_NotifyHpdLow()
/// notify HPD is low
/// @u8PortNumber		\b IN: HDMI Port Index
/// @return int, TRUE: success; FALSE: failed
//----------------------------------------------------------------
DLL_PUBLIC int HDCP1xTx_APP_NotifyHpdLow(unsigned char u8PortNumber);

//----------------------------------------------------------------
/// HDCP1xTx_APP_NotifyHpdHighEdidReady()
/// notify HPD is high and EDID checking process is done;
/// @u8PortNumber		\b IN: HDMI Port Index
/// @return int, TRUE: success; FALSE: failed
//----------------------------------------------------------------
DLL_PUBLIC int HDCP1xTx_APP_NotifyHpdHighEdidReady(unsigned char u8PortNumber);

//----------------------------------------------------------------
/// HDCP1xTx_APP_MultiRxNotifyH1XRxM0Ready()
/// Notify h1x driver when hdcp1x rx m0 is ready
/// @param u8UpstreamPtIdx      \b IN: HDMI Rx Port Index
/// @param u8DownstreamPtIdx    \b IN: HDMI Tx port index
/// @return int, TRUE: success; FALSE: failed
//----------------------------------------------------------------
DLL_PUBLIC int HDCP1xTx_APP_MultiRxNotifyH1XRxM0Ready(unsigned char u8UpstreamPtIdx, unsigned char u8DownStreamPtIdx);

//----------------------------------------------------------------
/// HDCP1xTx_APP_MultiRxConvertGetVPrime()
/// Using this function to get V' when upstream is hdcp1x, and downstream is hdcp2.2
/// @param u8UpstreamPtIdx      \b IN: HDMI Rx Port Index
/// @param u8DownstreamPtIdx    \b IN: HDMI Tx port index
/// @param bHDMIMode    \b IN: indicate current mode; TRUE: HDMI mode; FALSE: DVI Mode
/// @param pu8RxInfo     \b IN: pointer of RxInfo from sink
/// @param pu8RecvIDList    \b IN: pointer of RecvID list from sink
/// @param nRecvIDListLen    \b IN: length of receiver ID list
/// @param pu8Vprime    \b IN: pointer for return V' value
/// @return int, TRUE: success; FALSE: failed
//----------------------------------------------------------------
DLL_PUBLIC int HDCP1xTx_APP_MultiRxConvertGetVPrime(unsigned char u8UpstreamPtIdx, unsigned char u8DownStreamPtIdx, unsigned char bHDMIMode, unsigned char* pu8RxInfo, unsigned char* pu8RecvIDList, unsigned int nRecvIDListLen, unsigned char* pu8Vprime);

//----------------------------------------------------------------
/// HDCP1xTx_APP_NotifyH1XRxM0Ready()
/// Notify h1x driver when hdcp1x rx m0 is ready
/// @param u8PortIdx      \b IN: HDMI Port Index
/// @return int, TRUE: success; FALSE: failed
//----------------------------------------------------------------
DLL_PUBLIC int HDCP1xTx_APP_NotifyH1XRxM0Ready(unsigned char u8PortIdx);

//----------------------------------------------------------------
/// HDCP1xTx_APP_ConvertGetVPrime()
/// Notify h1x driver when hdcp1x rx m0 is ready
/// @param u8PortIdx      \b IN: HDMI Port Index
/// @param bHDMIMode    \b IN: indicate current mode; TRUE: HDMI mode; FALSE: DVI Mode
/// @param pu8RxInfo     \b IN: pointer of RxInfo from sink
/// @param pu8RecvIDList    \b IN: pointer of RecvID list from sink
/// @param nRecvIDListLen    \b IN: length of receiver ID list
/// @param pu8Vprime    \b IN: pointer for return V' value
/// @return int, TRUE: success; FALSE: failed
//----------------------------------------------------------------
DLL_PUBLIC int HDCP1xTx_APP_ConvertGetVPrime(unsigned char u8PortIdx, unsigned char bHDMIMode, unsigned char* pu8RxInfo, unsigned char* pu8RecvIDList, unsigned int nRecvIDListLen, unsigned char* pu8Vprime);

//----------------------------------------------------------------
/// HDCP1xTx_APP_SetAnProperty()
/// Get current HDCP2.2 RX status
/// @nPortNumber	        \b IN: HDMI Port Index
/// @u8UseInternalAn        \b IN: indicate using internal or external AN
/// @return void
//----------------------------------------------------------------
DLL_PUBLIC void HDCP1xTx_APP_SetAnProperty(unsigned int nPortNumber, unsigned char u8UseInternalAn);

//----------------------------------------------------------------
/// HDCP1xTx_APP_SetAccessX74FuncCB()
/// set callback function of accessing x74
/// @pFunc		\b IN: function pointer of DDC access utopia function
/// @return void
//----------------------------------------------------------------
DLL_PUBLIC void HDCP1xTx_APP_SetAccessX74FuncCB(Fn_HDCP1X_AccessX74 pFunc);

//----------------------------------------------------------------
/// HDCP1xTx_APP_SetNotifyStatusFuncCB()
/// set callback functon for getting current authentication status;
/// @pFunc		\b IN: function pointer of getting authentication status
/// @return void
//----------------------------------------------------------------
DLL_PUBLIC void HDCP1xTx_APP_SetNotifyStatusFuncCB(Fn_HDCP1X_NotifyAuthStatus pFunc);

//----------------------------------------------------------------
/// HDCP1xTx_Internal_SetGetOutputModeFuncCB()
/// set callback functon for getting current output mode;
/// @pFunc		\b IN: function pointer of getting output mode
/// @return void
//----------------------------------------------------------------
DLL_PUBLIC void HDCP1xTx_APP_SetGetOutputModeFuncCB(Fn_HDCP1X_GetOutputMode pFunc);

//----------------------------------------------------------------
/// HDCP1xTx_APP_SetNotifyRepeaterInfoFuncCB()
/// set callback functon for getting BStatus, KSV list, V' for hdcp1x RX
/// @pFunc		\b IN: function pointer of repeater relative information
/// @return void
//----------------------------------------------------------------
DLL_PUBLIC void HDCP1xTx_APP_SetNotifyRepeaterInfoFuncCB(Fn_HDCP1X_NotifyRepeaterInfo pFunc);

//----------------------------------------------------------------
/// HDCP1xTx_APP_SetCompareRiFuncCB()
/// set callback functon for comparing Ri
/// @pFunc		\b IN: function pointer of repeater relative information
/// @u32CmdIdx  \b IN: command index
/// @return void
//----------------------------------------------------------------
DLL_PUBLIC void HDCP1xTx_APP_SetCompareRiFuncCB(Fn_HDCP1X_CompareRi pFunc, unsigned long u32CmdIdx);

//----------------------------------------------------------------
/// HDCP1xTx_APP_DeInitHdcp()
/// de-init hdcp1x process
/// @void
/// @return void
//----------------------------------------------------------------
DLL_PUBLIC void HDCP1xTx_APP_DeInitHdcp(void);

//----------------------------------------------------------------
/// HDCP1xTx_APP_Init()
/// init whole hdcp1x tx ports
/// @nPortCount		\b IN: total number of ports which support hdcp1x tx
/// @return int, TRUE: success; FALSE: failed
//----------------------------------------------------------------
DLL_PUBLIC int HDCP1xTx_APP_Init(int nPortCount);

//----------------------------------------------------------------
/// HDCP1xRx_APP_FillKey()
/// Load HDCP 1.4 Rx key
/// @pu8Hdcp1xKey       \b IN:pointer of hdcp1x Rx encrypted key
/// @nKeySize               \b IN: HDCP 1.4 Rx key size
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
DLL_PUBLIC int HDCP1xRx_APP_FillKey(unsigned char* pu8Hdcp1xKey, int nKeySize);

#ifdef __cplusplus
}
#endif

#endif //#ifndef __HDCP1X_APP_H__
