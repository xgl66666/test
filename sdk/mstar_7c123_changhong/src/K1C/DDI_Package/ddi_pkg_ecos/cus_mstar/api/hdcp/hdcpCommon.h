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

//32-bit
#ifndef H_HDCP_COMMON_H
#define H_HDCP_COMMON_H

#ifndef DLL_PUBLIC
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define BUILD_32BIT     1

typedef struct
{
    int nLogPriority;                 // #0&1:default,:2:trace,3:info,4:warning,5:debug,6:error,7:fatal,8:silent
    unsigned char bFacsimileTxEnable; // 0: disable facsimile tx, 1: enable facsimile tx
    unsigned char bFacsimileRxEnable; // 0: disable facsimile rx, 1: enable facsimile rx
} ST_HDCP_COMMON_SECURE_CONFIG;

typedef struct
{
    int bDscmbEnable;               // enable dscmb or not, ex: nugget doesn't have dscmb feature.
    int nDscmbConnectType;          // dscmb operation type.
                                    // 0: create all dscmb filter and connect all dmx filter
                                    // 1: base on specified dmx filter count to create dscmb filter and connect them
} ST_HDCP_MIRACAST_SECURE_CONFIG;

typedef struct
{
    int TBD;                        // To be defined. A dummy variable for build-pass in Android
} ST_HDCP_HDMI_SECURE_CONFIG;

typedef enum
{
    EN_HDCP_ERR_NA = 0,
    EN_HDCP_ERR_NULL_MESSAGE,
    EN_HDCP_ERR_AKE_INIT,
    EN_HDCP_ERR_AKE_SEND_CERT,
    EN_HDCP_ERR_AKE_NO_STORED_KM,
    EN_HDCP_ERR_AKE_STORED_KM,
    EN_HDCP_ERR_AKE_SEND_RRX,
    EN_HDCP_ERR_AKE_SEND_H_PRIME,
    EN_HDCP_ERR_AKE_SEND_PAIRING_INFO,
    EN_HDCP_ERR_LC_INIT,
    EN_HDCP_ERR_LC_SEND_L_PRIME,
    EN_HDCP_ERR_SKE_SEND_EKS,
    EN_HDCP_ERR_REPEATERAUTH_SEND_RECEIVERID_LIST,
    EN_HDCP_ERR_RTT_READY,
    EN_HDCP_ERR_RTT_CHALLENGE,
    EN_HDCP_ERR_REPEATERAUTH_SEND_ACK,
    EN_HDCP_ERR_REPEATERAUTH_STREAM_MANAGE,
    EN_HDCP_ERR_REPEATERAUTH_STREAM_READY,
    EN_HDCP_ERR_RECEIVER_AUTHSTATUS,
    EN_HDCP_ERR_AKE_TRANSMITTER_INFO,
    EN_HDCP_ERR_AKE_RECEIVER_INFO,

    EN_HDCP_ERR_HDCP_KEY_IS_NOT_EXISTED,

} EN_HDCP_ERROR_TYPE;

typedef enum
{
    EN_HDCP_UNKNOWN = 0,
    EN_HDCP_IDLE,
    EN_HDCP_AUTH_FAIL,
    EN_HDCP_AUTH_SUCCESS,
    EN_HDCP_AUTH_PROCESSING,

    EN_HDCP_AUTH_MAX,

} EN_HDCP_STATUS;


/*** Android Miracast ***/
typedef long long HDCP_HANDLE;
typedef unsigned long long PHYS_ADDR;

typedef struct ST_HDCP_OUT_BUFFER
{
    PHYS_ADDR phyStartAddress;
    PHYS_ADDR phyWritePointer;
    unsigned int u32BufferLen;

    // for policy setting usage on optee
    unsigned int u32SvpPipeLineID;
    unsigned long long u64StreamID;
    unsigned long long u64XCID;
}ST_HDCP_OUT_BUFFER;

typedef enum
{
    E_HDCP_PAYLOAD_AUDIO,
    E_HDCP_PAYLOAD_VIDEO,
} EN_HDCP_PAYLOAD_TYPE;

typedef enum
{
    E_HDCP_OK = 1,             // Succeeded.
    E_HDCP_MEMORY_ALLOC,       // Memory allocation failed.
    E_HDCP_OVERFLOW,           // Buffer overflow.
    E_HDCP_NOT_SECURE,         // The output buffer is not in secure range.
    E_HDCP_TRY_AGAIN,          // Not enough buffer for a complete frame.
    E_HDCP_INVALID_PARAMETER,  // The destination buffer is already written.

    E_HDCP2_FAIL,              // Other fail cases

} EN_HDCP_RETURN_VALUE;
///////////////////////////////////////////////

/// HDMI usage

typedef enum
{
    EN_HDCP_HDMI_NORMAL_TYPE = 0,   // normal type
    EN_HDCP_HDMI_SEPARATE_TYPE,     // separate type(raptors)

} EN_HDCP_HDMI_WORK_TYPE;

// definition: u8OffsetAddr
#define OFFSET_HDCP2_VERSION 0x50
#define OFFSET_WRITE_MESSAGE 0x60
#define OFFSET_RXSTATUS      0x70
#define OFFSET_READ_MESSAGE  0x80
#define OFFSET_DBG           0xC0

// definition: u8OpCode
#define OP_WRITE             0x00
#define OP_READ              0x01

typedef void (*Fn_HDCP22_Recv_CBF)(unsigned char, unsigned char, unsigned char*, unsigned long, void*); //type, portIdx, msg, msg length, context

typedef void (*Fn_MDrv_HDCP22_InitCBFunc)(Fn_HDCP22_Recv_CBF pCBFunc, void *pContext);
typedef void (*Fn_MDrv_HDCP22_PortInit)(unsigned char ucPortIdx);
typedef unsigned char (*Fn_MDrv_HDCP22_PollingReadDone)(unsigned char ucPortIdx);
typedef void (*Fn_MDrv_HDCP22_EnableCipher)(unsigned char ucPortType, unsigned char ucPortIdx, unsigned char bIsEnable);
typedef void (*Fn_MDrv_HDCP22_SendMsg)(unsigned char ucPortType, unsigned char ucPortIdx, unsigned char *pucData, unsigned long dwDataLen, void *pDummy);
typedef void (*Fn_MDrv_HDCP22_Handler)(unsigned char ucPortIdx);
typedef void (*Fn_MDrv_HDCP22_FillCipherKey)(unsigned char pucPortIdx, unsigned char *pucRiv, unsigned char *pucSessionKey);
typedef void (*Fn_MApi_HDMITx_HDCP2TxInit)(unsigned char u8PortIdx, unsigned char bEnable);
typedef unsigned char (*Fn_MApi_HDMITx_HDCP2AccessX74)(unsigned char u8PortIdx, unsigned char u8OffsetAddr, unsigned char u8OpCode, unsigned char *pu8RdBuf, unsigned short u16RdLen, unsigned char *pu8WRBuff, unsigned short u16WrLen);
typedef void (*Fn_MApi_HDMITx_HDCP2TxEnableEncrypt)(unsigned char u8PortIdx, unsigned char bEnable);
typedef void (*Fn_MApi_HDMITx_HDCP2TxFillCipherKey)(unsigned char u8PortIdx, unsigned char *pu8Riv, unsigned char *pucKsXORLC128);


/// HDCP Status callback
typedef void (*Fn_HDCP22_GetHdcpStatus)(unsigned char u8PortIdx, EN_HDCP_STATUS enHdcpStatus);

/// HDCP Status callback for Miracast
typedef void (*Fn_HDCP22_GetMiracastHdcpStatus)(EN_HDCP_STATUS enHdcpStatus);

/// Error callback
typedef void (*Fn_HDCP22_Error_CBF)(unsigned int u32State, unsigned int u32SubState, unsigned char *strMsg);

/// Get HDCP key
typedef int (*Fn_HDCP22_GetHdcpKey)(char *strHdcpKeyPath, void *pKeyParam);

/// Get receiver id list from Raptor
typedef int (*Fn_HDCP22_GetHdcpRcvIdList)(unsigned int nPortNumber, unsigned char *pu8RcvList, unsigned int *pu32Len);

/// Get receiver id list for converter
typedef int (*Fn_HDCP22_GetHdcpRcvIdListConverter)(unsigned int nPortNumber, unsigned char* pu8RxInfo, unsigned char* pu8RcvList, unsigned int u32LenOfRcvList);


#ifdef __cplusplus
}
#endif


#endif
