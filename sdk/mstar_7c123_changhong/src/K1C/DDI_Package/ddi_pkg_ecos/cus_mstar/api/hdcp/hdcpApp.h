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

#ifndef __HDCP_APP_H__
#define __HDCP_APP_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "hdcpCommon.h"
#include <inttypes.h>
#ifndef DLL_PUBLIC
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#endif

#define IPADDR_LENGTH    16
#define STREAM_CTR_NUM   5

/// hdcp common data
typedef struct ST_HDCP_COMMON_DATA
{
    void*               CommonDataAddress;
    int                 eDeviceType;
    char                szPeerIPAddr[IPADDR_LENGTH+1];
    int                 usHdcpPort;
    int                 sockListen;                     /// useless, keep to sync Gen4 and MCT
    int                 sock;                           /// useless, keep to sync Gen4 and MCT

    int                 eLogLevel;                      /// useless, keep to sync Gen4 and MCT
    long                lTimeoutMultiplier;
    int                 hdcp_canceled;                  /// useless, keep to sync Gen4 and MCT
    int                 bRx;
    unsigned long long  aullInputCtr[STREAM_CTR_NUM];
    Fn_HDCP22_Error_CBF pFn_HDCP22_Error_CBF;
    int                 nThreadRunning;                 /// non-zero: thread is running
    int                 nStopActivated;                 /// non-zero: stop miracast is running
    int                 bRepeater;

    int                 nShmID;
    int                 nSemID;
    int                 bSharedMemoryInited;
} ST_HDCP_COMMON_DATA;


//////////////////////////////////////////////////////////////////////
/// HDCP2 New APIs definition
/// Note: HDCP2 prefix means new interfaces
//////////////////////////////////////////////////////////////////////

/*** General ***/

/// Set drm key config file path(drmkey.ini)
/// Note: Please call this function before any function calls in this library.
/// @param szDrmKeyConfigPath  [IN]   drm key config file path
/// @param nLen                [IN]   file path string length
DLL_PUBLIC int HDCP2_SetDrmKeyConfigPath(const char *const szDrmKeyConfigPath, int nLen);


/// Set config file path(hdcp2.ini)
/// Note1: Please modify make file to untar libhdcp.tar.gz
///        and move the hdcp2.ini to specific path as you expected
/// Note2: Please call this function before any function calls in this library.
/// @param szConfigPath        [IN]   config file path
/// @param nLen                [IN]   file path string length
DLL_PUBLIC int HDCP2_SetConfigPath(const char *const szConfigPath, int nLen);


/// Get HDCP key version
/// Note: this function is only work for TEE
/// @param pnVersion          [OUT]   HDCP KEY version (0: 2.0, 2: 2.2)
DLL_PUBLIC int HDCP2_GetHdcpRxKeyVersion(int *pnVersion);


/// Get HDCP Receiver ID
/// Note: this function is only work for TEE
/// @param pu8RecriverId       [OUT] Receiver ID in HDCP rx key
/// @param nLength             [IN]  Length of the receiver id
DLL_PUBLIC int HDCP2_GetHdcpReceiverId(unsigned char *pu8RecriverId, const int nLength);


/// Set HDCP stream manage type
/// @param u8Type              [IN] Type 0(0x00): May be transmitted by the HDCP Repeater to all HDCP devices
///                                 Type 1(0x01): Must not be transmitted by the HDCP Repeater to HDCP 1.x-compliant
///                                 Devices and HDCP 2.0-compliant Repeaters
DLL_PUBLIC int HDCP2_SetStreamManageType(unsigned char u8Type);


/*** Miracast ***/

/// Start Miracast authentation process.
/// @param pstHdcpData         [IN]   common data instance pointer
DLL_PUBLIC int HDCP2_StartMiracast(ST_HDCP_COMMON_DATA *pstHdcpData);


/// Stop Miracast
/// @param pstHdcpData         [IN]   common data instance pointer
DLL_PUBLIC int HDCP2_StopMiracast(ST_HDCP_COMMON_DATA *pstHdcpData);


/// Connect dmx filter to dscmb filter
/// @param pstHdcpData         [IN]   common data instance pointer
/// @param u8DmxFltId          [IN]   dmx filter id
DLL_PUBLIC int HDCP2_DscmbFltConnectDmxFlt(ST_HDCP_COMMON_DATA *pstHdcpData, unsigned char u8DmxFltId);


/// Debug usage
/// Set AES and RIV key to DSCMB engine
/// @param pu8ContentKey       [IN]   content key (16 Bytes)
/// @param pu8RIV              [IN]   riv key (8 Bytes)
DLL_PUBLIC void HDCP2_SetDscmbAesRivKey(const unsigned char *pu8ContentKey, const unsigned char *pu8RIV);


/// Decrypted payload data, store the result in secure buffer for later framing.
/// Note: This function is only used in AN Miracast currently.
/// @param handle              [IN] Handle for IPC
/// @param enType              [IN] Payload type of the encrypted payload
/// @param pEncryptedPayload   [IN] Encrypted payload buffer address (heap).
/// @param nPayloadLen         [IN] Length of the payload buffer, not necessarily 16-byte aligned.
/// @param pHdcpPrivateData    [IN] HDCP private data located at PES header.
/// @param pstOutBuffer        [IN] The destination buffer to write decrypted data.
///                                 If NULL, the decrypted data will be stored in internal buffer.
///                            [OUT] HDCP will update the write pointer addr.
/// @param llPts               [IN] PTS associated with this PES payload.
DLL_PUBLIC int HDCP2_DecryptPayload(HDCP_HANDLE handle, EN_HDCP_PAYLOAD_TYPE enType,
                                    unsigned char *puEncryptedPayload, unsigned int u32PayloadLen,
                                    const unsigned char *pu8HdcpPrivateData, const ST_HDCP_OUT_BUFFER *pstOutBuffer,
                                    int64_t llPts);


/// Parse framing buffer, return the next complete frame with PTS information.
/// Note: This function is only used in AN Miracast currently.
/// @param handle              [IN] Handle for IPC
/// @param enType              [IN] Payload type of the encrypted payload
/// @param pnFrameLen          [OUT] Address to receive the length of the next frame.
/// @param pllPts              [OUT] Address to receive the pts of the next frame.
///                                  Return -1 to signal the next frame does not have valid PTS attached.
DLL_PUBLIC int HDCP2_GetNextFrame(HDCP_HANDLE handle, EN_HDCP_PAYLOAD_TYPE enType,
                                  unsigned int *pu32FrameLen, int64_t *pllPts);


/// Copy frame buffer to specified physical address.
/// Note: This function is only used in AN Miracast currently.
/// @param handle              [IN] Handle for IPC
/// @param enType              [IN] Payload type of the encrypted payload
/// @param pstOutBuffer        [IN] The destination buffer to write decrypted data.
///                                 If NULL, the decrypted data will be stored in internal buffer.
DLL_PUBLIC int HDCP2_CopyFrame(HDCP_HANDLE handle, EN_HDCP_PAYLOAD_TYPE enType, const ST_HDCP_OUT_BUFFER *pstOutBuffer);


/// Reset the internal framing buffer.
/// Note: This function is only used in AN Miracast currently.
/// @param handle              [IN] Handle for IPC
/// @param enType              [IN] Payload type of the encrypted payload
DLL_PUBLIC int HDCP2_FlushPendingBuffer(HDCP_HANDLE handle, EN_HDCP_PAYLOAD_TYPE enType);


/// Set Miracast HDCP status callback function
/// @param pFunc               [IN]   callback function
DLL_PUBLIC void HDCP2_SetMiracastHdcpStatusCB(Fn_HDCP22_GetMiracastHdcpStatus pFunc);


/// Customized the miracast configuration.
/// Note: MT usage
/// @param pFunc               [IN] Miracast customized configuration callback function
//DLL_PUBLIC void HDCP2_SetMiracastCustomizedConfigCB(Fn_MiracastCustomizedConfig_CBF pFunc);


/*** HDMI ***/

/// Set hdcp key callback function that used to get hdcp key
/// @param pFunc              [IN]   callback function
DLL_PUBLIC void HDCP2_SetHdcpKeyCB(Fn_HDCP22_GetHdcpKey pFunc);


/// Set HDCP HDMI work type
/// @param enType              [IN]   HDMI of HDCP work type
DLL_PUBLIC void HDCP2_SetHDMIWorkType(EN_HDCP_HDMI_WORK_TYPE enType);


/// Set HDCP Repeater mode for HDMI
/// @param nPortNumber         [IN]   hdmi port number
/// @param bIsRepeater         [IN]   Is repeater or not
DLL_PUBLIC void HDCP2_SetHDMIRepeaterMode(int nPortNumber, int bIsRepeater);


/// Reset HDCP Receiver id list
/// @param nPortNumber         [IN]   hdmi port number
DLL_PUBLIC void HDCP2_ResetHDMIRecvIdList(int nPortNumber);


/// Set RxInfo and Receiver ID list
/// @param u32PortNumber      [IN]   port number
/// @param pu8RecvIdList      [IN]   RxInfo and Receiver id list
/// @param pu32Len            [IN]   lenght of RxInfo/Receiver Id list
DLL_PUBLIC void HDCP2_SetRecvIdList(unsigned int u32PortNumber, const unsigned char *pu8RecvIdList, const unsigned int *pu32Len);


/// Get HDCP status of HDMI Tx/Rx for specific port
/// @param u8PortIdx          [IN]    HDMI port
/// @param bIsRx              [IN]    TRUE: Rx, FALSE: Tx
/// @return HDCP status of HDMI Tx/Rx
DLL_PUBLIC EN_HDCP_STATUS HDCP2_GetHdmiHdcpStatus(unsigned char u8PortIdx, unsigned char bIsRx);


/// Load HDCP device key.
/// @param bIsRx               [IN]   RX or Tx
DLL_PUBLIC int HDCP2_LoadKey(int bIsRx);


/// Initialize HDMI for HDCP handling.
/// Note: Please set callback function in advance.
/// @param bIsRx              [IN]   RX or Tx
/// @param nPortCount         [IN]   Port count
DLL_PUBLIC int HDCP2_InitHDMI(int bIsRx, int nPortCount);


/// Finialize HDMI for HDCP handling.
/// @param bIsRx              [IN]   RX or Tx
DLL_PUBLIC void HDCP2_DeInitHDMI(int bIsRx);


/// Suspend HDMI for HDCP handling.
/// @param bIsRx              [IN]   RX or Tx
DLL_PUBLIC void HDCP2_SuspendHDMI(int bIsRx);


/// Resume HDMI for HDCP handling.
/// @param bIsRx              [IN]   RX or Tx
DLL_PUBLIC void HDCP2_ResumeHDMI(int bIsRx);


/// Set HDMI InitCBFunc callback function.
/// @param pFunc              [IN]   callback function
DLL_PUBLIC void HDCP2_SetInitCBFuncCB(Fn_MDrv_HDCP22_InitCBFunc pFunc);


/// Set HDMI PortInit callback function.
/// @param pFunc              [IN]   callback function
DLL_PUBLIC void HDCP2_SetPortInitCB(Fn_MDrv_HDCP22_PortInit pFunc);


/// Set HDMI PollingReadDone callback function.
/// @param pFunc              [IN]   callback function
DLL_PUBLIC void HDCP2_SetPollingReadDoneCB(Fn_MDrv_HDCP22_PollingReadDone pFunc);


/// Set HDMI EnableCipher callback function.
/// @param pFunc              [IN]   callback function
DLL_PUBLIC void HDCP2_SetEnableCipherCB(Fn_MDrv_HDCP22_EnableCipher pFunc);


/// Set HDMI HDCP get receiver id list callback function.
/// @param pFunc              [IN]   callback function
DLL_PUBLIC void HDCP2_SetGetHdcpRcvIdListCB(Fn_HDCP22_GetHdcpRcvIdList pFunc);


/// Set HDMI FillCipherKey callback function.
/// @param pFunc              [IN]   callback function
DLL_PUBLIC void HDCP2_SetFillCipherKeyCB(Fn_MDrv_HDCP22_FillCipherKey pFunc);


/// Set HDMI SendMsg callback function.
/// @param pFunc              [IN]   callback function
DLL_PUBLIC void HDCP2_SetSendMsgCB(Fn_MDrv_HDCP22_SendMsg pFunc);


/// Set HDMI Handler callback function.
/// @param pFunc              [IN]   callback function
DLL_PUBLIC void HDCP2_SetHandlerCB(Fn_MDrv_HDCP22_Handler pFunc);


/// Set HDMI Tx HDCP2 initialization callback function
/// @param pFunc              [IN]   callback function
DLL_PUBLIC void HDCP2_SetHDMITxHDCP2TxInitCB(Fn_MApi_HDMITx_HDCP2TxInit pFunc);


/// Set HDMI Tx HDCP2 access x74 offset callback function
/// @param pFunc              [IN]   callback function
DLL_PUBLIC void HDCP2_SetHDMITxHDCP2AccessX74CB(Fn_MApi_HDMITx_HDCP2AccessX74 pFunc);


/// Set HDMI Tx HDCP2 enable encrypt callback function
/// @param pFunc              [IN]   callback function
DLL_PUBLIC void HDCP2_SetHDMITxHDCP2TxEnableEncryptCB(Fn_MApi_HDMITx_HDCP2TxEnableEncrypt pFunc);


/// Set HDMI Tx HDCP2 fill cipher key callback function
/// @param pFunc              [IN]   callback function
DLL_PUBLIC void HDCP2_SetHDMITxHDCP2TxFillCipherKeyCB(Fn_MApi_HDMITx_HDCP2TxFillCipherKey pFunc);


/// Set HDCP status callback function
/// @param pFunc              [IN]   callback function
DLL_PUBLIC void HDCP2_SetHdcpStatusCB(Fn_HDCP22_GetHdcpStatus pFunc);


/// Set HDMI Error message callback function.
/// @param pFunc              [IN]   callback function
DLL_PUBLIC void HDCP2_SetErrorCB(Fn_HDCP22_Error_CBF pFunc);


/// Notify HDCP of HDMI Tx for HPD high and EDID ready
/// @param u8PortNumber      [IN]    HDMI Tx port number
DLL_PUBLIC unsigned int HDCP2_NotifyTxHpdHighEdidReady(unsigned char u8PortNumber);


/// Notify HDCP of HDMI Tx for HPD low
/// @param u8PortNumber      [IN]    HDMI Tx port number
DLL_PUBLIC unsigned int HDCP2_NotifyTxHpdLow(unsigned char u8PortNumber);


/// Set converter mode for HDMI
/// @param u8PortNumber        [IN]  HDMI Tx port number
/// @param bIsConverter        [IN]  Enable converter mode or not
DLL_PUBLIC unsigned int HDCP2_SetHdmiTxConverterMode(unsigned char u8PortNumber, unsigned char bIsConverter);


/// Set get receiverid list callback function.
/// @param pFunc              [IN]   callback function
DLL_PUBLIC void HDCP2_SetGetHdcpRcvIdListConverterCB(Fn_HDCP22_GetHdcpRcvIdListConverter pFunc);


/// Set content stream type for specific port index to output policy manager
/// @param u8PortNumber        [IN]  HDMI Tx port number
/// @param u8Type              [IN]  content stream type
DLL_PUBLIC unsigned int HDCP2_SetHdmiContentStreamType(unsigned char u8PortNumber, unsigned char u8Type);


/// Initialize HDMI with HDCP22 Tx module or not
/// @param u8PortNumber        [IN]  HDMI Tx port number
/// @param bEnable             [IN]  Init/Deinit or not
DLL_PUBLIC unsigned int HDCP2_InitHdmiModuleTx(unsigned char u8PortNumber, unsigned char bEnable);


/// Debug usage
/// Get HDMI with HDCP2.2 cipher state
/// @param pu32State         [IN]     Port cipher state
/// @Note bit-0:port 1, bit-1: port2, ..., etc (HW level)
DLL_PUBLIC int HDCP_Get_Cipher_State(unsigned int *pu32State);


/// Compute HDMI of HDCP1.4 V
/// @param nPortNumber       [IN]     HDMI port number
/// @param pu8V              [OUT]    V
/// @param u32VLen           [IN]     V length
/// @param pu8KsvBstatus     [IN]     KSV list and Bstatus
/// @param u32KBLen          [IN]     KSV list and Bstatus length
DLL_PUBLIC int HDCP2_HDCP14ComputeV(int nPortNumber, unsigned char *pu8V, unsigned int u32VLen,
                                    const unsigned char *pu8KsvBstatus, unsigned int u32KBLen);


//////////////////////////////////////////////////////////////////////
/// Old interface for Miracast Rx
//////////////////////////////////////////////////////////////////////


/// Run the encryption function
/// @param pu8ContentKey     [IN]     Content key
/// @param pu8IV             [IN]     IV
/// @param nDataBufLen       [IN]     Buffer length
/// @param pDataBuf          [IN/OUT] Encrypted content buffer/Decrypted content buffer
DLL_PUBLIC int HDCP_Decrypt_AesCtr(const unsigned char *pu8ContentKey, const unsigned char *pu8IV, unsigned int nDataBufLen, unsigned char *pDataBuf);


/// Run the decryption function for TS packet
/// @param pu32PidArray      [IN]     TS packet PID array list
/// @param u32PidArraySize   [IN]     TS packet PID array size
/// @param pu8Buffer         [IN/OUT] Source and destination buffer
/// @param nBufLen           [IN]     Buffer length
/// @param pu8AesKey         [IN]     AES key
/// @param pu8RivKey         [IN]     RIV key(Note: please fill 0x00 to meet 16 bytes due to hw request)
DLL_PUBLIC int HDCP_Aes_Decrypt_TS(const unsigned int *pu32PidArray, unsigned int u32PidArraySize,
                                   unsigned char *pu8Buffer, unsigned int nBufLen,
                                   const unsigned char *pu8AesKey, const unsigned char *pu8RivKey);


//////////////////////////////////////////////////////////////////////
/// Old interface for Miracast Tx
//////////////////////////////////////////////////////////////////////


/// Initialize HDCP per configuration settings
/// @param pstHdcpData       [IN]     HDCP common data
/// @Note1  for rx, set bRx to TRUE, instead tx, set to FALSE
/// @Note2  pleaes give peer ip for tx case
DLL_PUBLIC int HDCP_Initialize(ST_HDCP_COMMON_DATA *pstHdcpData);


/// Finialize HDCP related controls per HDC common data
/// @param pstHdcpData       [IN]     HDCP common data
DLL_PUBLIC int HDCP_Finalize(ST_HDCP_COMMON_DATA *pstHdcpData);


/// Run the TX HDCP authentication
/// @param pstHdcpData       [IN]     HDCP common data
DLL_PUBLIC int HDCP_TX_StartAuth(ST_HDCP_COMMON_DATA *pstHdcpData);


/// Initialize encryption
/// @param pstHdcpData       [IN]     HDCP common data
DLL_PUBLIC int HDCP_Begin_Crypto(ST_HDCP_COMMON_DATA *pstHdcpData);


/// Finialize encryption
/// @param pstHdcpData       [IN]     HDCP common data
DLL_PUBLIC int HDCP_End_Crypto(ST_HDCP_COMMON_DATA *pstHdcpData);


/// Run the encryption function
/// @param pstHdcpData       [IN]     HDCP common data
/// @param pu8Dst            [OUT]    After encryption content buffer
/// @param pu8Src            [IN]     Before encryption content buffer
/// @param nOctetSize        [IN]     Size
/// @param u32StreamCtr      [IN]     Stream counter used for AES-CTR
/// @param pullInputCtr      [OUT]    Input counter used for AES-CTR
DLL_PUBLIC int HDCP_Encrypt(ST_HDCP_COMMON_DATA *pstHdcpData, unsigned char *pu8Dst, const unsigned char *pu8Src,
                            int nOctetSize, unsigned int u32StreamCtr, unsigned long long *pullInputCtr);


// TODO:
/// Run the decryption function
/// @param pstHdcpData       [IN]     HDCP common data
DLL_PUBLIC int HDCP_Decrypt(ST_HDCP_COMMON_DATA *pstHdcpData);

//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
/// Bad design functions, it should be removed in future.
/// Keep to compatible current design.
////////////////////////////////////////////////////////////////////

/// Initiate HDCP connection with specified server.
/// Note: This function is only used in AN Miracast currently.
/// HDCP module will also init TEE module and prepare secure buffer for storing decrypted data.
/// @param pstHdcpData         [IN] common data instance pointer
/// @param pHandle             [OUT] Handle for IPC
DLL_PUBLIC int HDCP2_Connect(ST_HDCP_COMMON_DATA *pstHdcpData, HDCP_HANDLE *pHandle);


/// Disconnect HDCP session, and return allocated resources.
/// Note: This function is only used in AN Miracast currently.
/// @param pstHdcpData         [IN]   common data instance pointer
DLL_PUBLIC int HDCP2_Disconnect(ST_HDCP_COMMON_DATA *pstHdcpData);

////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // __HDCP_APP_H__
