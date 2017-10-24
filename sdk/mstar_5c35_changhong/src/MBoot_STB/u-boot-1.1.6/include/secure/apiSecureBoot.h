////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   apiSecureBoot.h
/// @brief  Secure Boot Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_SECUREBOOT_H_
#define _API_SECUREBOOT_H_

#ifdef API_SECUREBOOT_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#include <sys/common/MsTypes.h>
#include <sys/drv/drvAESDMA.h>

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define SECUREBOOT_MBX_QUEUESIZE        8

#define MBX_CTRL_FIRE            BIT(0)
#define MBX_CTRL_READBACK        BIT(1)
#define MBX_CTRL_INSTANT         BIT(2)
#define MBX_STATE1_OVERFLOW      BIT(13)
#define MBX_STATE1_ERROR         BIT(14)
#define MBX_STATE1_BUSY          BIT(15)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// emurate SECUREBOOT fucntion return result
typedef enum
{
    /// SECUREBOOT result for failure
    E_SECUREBOOT_FAIL =0,
    /// SECUREBOOT result for OK
    E_SECUREBOOT_OK = 1,

} SECUREBOOT_Result;


///Define SECUREBOOT Acknoledge Flags
typedef enum
{
    E_SECUREBOOT_ACKFLG_WAIT_INIT       = (1<<0),               /// Ack flag for Init
    E_SECUREBOOT_ACKFLG_WAIT_CONFIG     = (1<<1),             /// Ack flag for Config
    E_SECUREBOOT_ACKFLG_WAIT_GETKEY     = (1<<2),             /// Ack flag for Get Key
    E_SECUREBOOT_ACKFLG_WAIT_GETKEYDONE = (1<<3),             /// Ack flag for Get Key Done
    E_SECUREBOOT_ACKFLG_WAIT_AUTHDONE   = (1<<4),             /// Ack flag for Authentication
    E_SECUREBOOT_ACKFLG_WAIT_AESENCODE  = (1<<5),             /// Ack flag for AES Encode Done
    E_SECUREBOOT_ACKFLG_WAIT_AESDECODE  = (1<<6),             /// Ack flag for AES Decode Done
    E_SECUREBOOT_ACKFLG_WAIT_1KPSRAM    = (1<<7)             /// Ack flag for 1K PSRAM Done

} SECUREBOOT_AckFlags;


/// emurate SECUREBOOT mailbox commands from cpu to mcu51
typedef enum
{
    /// mbx command for initialization
    E_SECUREBOOT_CPUTo51_CMD_INIT       = 0x00,
    /// mbx command for configuration
    E_SECUREBOOT_CPUTo51_CMD_CONFIG     = 0x01,
    /// mbx command for key code
    E_SECUREBOOT_CPUTo51_CMD_GETKEY     = 0x02,
    /// mbx command for get key code done
    E_SECUREBOOT_CPUTo51_CMD_GETKEYDONE = 0x03,
    /// mbx command for authentication
    E_SECUREBOOT_CPUTo51_CMD_AUTHDONE   = 0x04,
    /// mbx command for AES encode
    E_SECUREBOOT_CPUTo51_CMD_AES_ENCODE = 0x05,
    /// mbx command for AES decode
    E_SECUREBOOT_CPUTo51_CMD_AES_DECODE = 0x06,
    /// mbx command for 1K PSRAM
    E_SECUREBOOT_CPUTo51_CMD_1KPSRAM    = 0x07

} SECUREBOOT_CPUTo51CmdIdx;

/// emurate SECUREBOOT mailbox commands ack from mcu51 to cpu
typedef enum
{
    //(1) Acknowledge from MCU51
    /// ack mbx command for initialization
    E_SECUREBOOT_51ToCPU_CMD_ACK_INIT       = 0x00,
    /// ack mbx command for configuration
    E_SECUREBOOT_51ToCPU_CMD_ACK_CONFIG     = 0x01,
    /// ack mbx command for get key
    E_SECUREBOOT_51ToCPU_CMD_ACK_GETKEY     = 0x02,
    /// ack mbx command for Get Key done
    E_SECUREBOOT_51ToCPU_CMD_ACK_GETKEYDONE = 0x03,
    /// ack mbx command for auth done
    E_SECUREBOOT_51ToCPU_CMD_ACK_AUTHDONE   = 0x04,
    /// ack mbx command for AES encode done
    E_SECUREBOOT_51ToCPU_CMD_ACK_AES_ENCODE = 0x05,
    /// ack mbx command for AES decode done
    E_SECUREBOOT_51ToCPU_CMD_ACK_AES_DECODE = 0x06,
    /// ack mbx command for 1K PSRAM
    E_SECUREBOOT_51ToCPU_CMD_ACK_1KPSRAM    = 0x07

    //(2) Notification from MCU51

} SECUREBOOT_51ToCPUCmdIdx;

typedef struct
{
    MS_U32  u32Pm51ToCpuAddr;
    MS_U32  u32Length;
    MS_U8   u8Status;
} SECUREBOOT_MbxRetStruct;

typedef struct
{
    MS_U32 pChiperBuf;
    MS_U32 pPlainBuf;
    MS_U32 *pKey;
    MS_U32 *pIV;
    MS_U32 u32Len;
    DrvAESDMA_CipherMode eMode;
}AES_ParamStruct;

#if 0
/// Callback function which is called in SECUREBOOT send MAILBOX ISR.
typedef void ( *SECUREBOOT_Callback ) (void* pData);
#endif

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL Secure_MailBox_Init(void);
INTERFACE SECUREBOOT_Result Secure_MailBox_GetKey(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct);
INTERFACE SECUREBOOT_Result Secure_MailBox_KeyDone(void);
INTERFACE SECUREBOOT_Result Secure_MailBox_AESEncode(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U32 u32Addr, U32 u32Len, U8 u8Mode);
INTERFACE SECUREBOOT_Result Secure_MailBox_AESDecode(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U32 u32Addr, U32 u32Len, U8 u8Mode);
INTERFACE SECUREBOOT_Result Secure_MailBox_1KPSRAM(SECUREBOOT_MbxRetStruct *pSecureBootMbxRetStruct, U8 u8Mode);
INTERFACE U32  Secure_AES_Performance(U32 u32Len,  DrvAESDMA_CipherMode eMode, MS_BOOL bEnc);

#ifdef __cplusplus
}
#endif

#undef INTERFACE
#endif // _API_SECUREBOOT_H_

