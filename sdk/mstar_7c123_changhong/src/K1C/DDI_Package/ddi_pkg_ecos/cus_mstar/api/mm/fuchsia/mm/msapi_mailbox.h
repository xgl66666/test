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
///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   apiMailBox.h
/// @author MStar Semiconductor Inc.
/// @brief  API of MailBox between 8051 and Aeon
/// @note   Most of the APIs involved with eCos kernel object
///
///
/// @note
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MSAPI_MAILBOX_H_
#define _MSAPI_MAILBOX_H_

#ifdef __cplusplus
extern "C" {
#endif  // #ifdef __cplusplus

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
//#include "mm_common.h"
#include "drvMBX.h"
//-------------------------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------------------------
#undef INTERFACE
#ifdef  _MSAPI_MAILBOX_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif  // #ifdef  _MSAPI_MAILBOX_H_

#ifndef VIRTUAL_MAILBOX
#define VIRTUAL_MAILBOX        1
#endif

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------


//==============================================================================

#define MB_PARAM_SIZE   10
typedef struct
{
    MBX_ROLE_ID      eRoleID;
    MBX_MSG_Type     eMsgType;
    U8             Control;
    U8             CommandClass;
    U8             CommandIndex;
    U8             ParamCount;
    U8             Params[MB_PARAM_SIZE];
    U8             Status0;
    U8             Status1;
}MApiMB_Command;

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------
//INTERFACE BOOL  MApiMB_Init(void);
//INTERFACE S32   MApiMB_RegisterCommandClass(MBX_Class eMsgClass, MBX_MAIL_ARRIVE_NOTIFY_FUNC pMB_CommandCB, char *pcCommandClassName);
//INTERFACE BOOL  MApiMB_RecvCommandFromQueue(MBX_Class eMsgClass, MApiMB_Command *pMApiMB_Command, U32 u32Flag);
//INTERFACE BOOL  MApiMB_SendCommand(MApiMB_Command MApiMB_OutCommand);


#if VIRTUAL_MAILBOX
/*
#define MB_PARAM_SIZE   10
typedef struct
{
    MS_U8             Control;
    MS_U8             CommandClass;
    MS_U8             CommandIndex;
    MS_U8             ParamCount;
    MS_U8             Params[MB_PARAM_SIZE];
    MS_U8             Status0;
    MS_U8             Status1;
}MApiMB_Command;
*/
#define _BIT0           (0x00000001UL)
#define _BIT1           (0x00000002UL)
#define _BIT2           (0x00000004UL)
#define _BIT3           (0x00000008UL)
#define _BIT4           (0x00000010UL)
#define _BIT5           (0x00000020UL)
#define _BIT6           (0x00000040UL)
#define _BIT7           (0x00000080UL)
#define _BIT8           (0x00000100UL)

INTERFACE  MS_S32 _s32MB_Semaphore_ID;
INTERFACE  MS_S32 _s32MB_Semaphore_ID_Recv;

INTERFACE   MS_S32 _s32MB_Semaphore_ID_HK;
INTERFACE   MS_S32 _s32MB_Semaphore_ID_Recv_HK;

INTERFACE MS_S32 _s32MB_Semaphore_ID_ThirdParty;
INTERFACE  MS_S32 _s32MB_Semaphore_ID_Recv_MM_ThirdParty;
INTERFACE  MS_S32 _s32MB_Semaphore_ID_Recv_HK_ThirdParty;

typedef enum
{
    /// MBX_CLASS_SYSTEM
    E_VS_MBX_CLASS_SYSTEM         = 0,
    ///
    E_VS_MBX_CLASS_VDPLAYER_FS    = 1,
    /// MBX_CLASS_VDPLAYER_FS
    E_VS_MBX_CLASS_VDPLAYER       = 2,
    /// MBX_CLASS_STILLIMAGE_CMD_IO
    E_VS_MBX_CLASS_STILLIMAGE_CMD_IO = 3,
    /// MBX_CLASS_STILLIMAGE_DATA_IO_FAST
    E_VS_MBX_CLASS_STILLIMAGE_DATA_IO_FAST = 4,
    /// MBX_CLASS_MSTILLIMAGE_DATA_IO
    E_VS_MBX_CLASS_STILLIMAGE_DATA_IO = 5,
    
    E_VS_MBX_CLASS_NUM,
    /// The End NUmber of Max Class ID
    E_VS_MBX_CLASS_MAX = E_VS_MBX_CLASS_NUM
}VIRTUAL_MBX_Class;

typedef enum
{
    MB_MM,
    MB_HK,
    MB_ThirdParty,
    MB_ALL,
}MB_THREAD_MODE;

typedef enum
{
    MB_NONE                                 =0,
    MM_SEND_FLAG                     =_BIT0,
    HK_SEND_FLAG_FS                    =_BIT1,
    HK_SEND_FLAG                    =_BIT2,
    MB2_MM_SEND_FLAG            =_BIT3,
    MB2_HK_SEND_FLAG            =_BIT4,

    MM_RECV_FLAG                    =_BIT5,
    HK_RECV_FLAG                    =_BIT6,
    MB_MM_RECV_FLAG            =_BIT7,
    MB_HK_RECV_FLAG            =_BIT8,
}MAIL_BOX_ACTION;

//typedef MS_U8 ( *MB_CommandCB ) (MS_U8 *pMB_InCommand, MS_U8 *bNeedCopyToQueue);
typedef void ( *MB_CommandCB ) (MBX_Msg *pMsg, MS_BOOL *pbAddToQueue);//(MS_U8 *pMB_InCommand, MS_U8 *bNeedCopyToQueue);

typedef struct
{
    MS_U8             bUsed;
    MS_S32              s32MutexID;
    MS_U32              u32QueueDepth;
    MS_U32              u32QueueAvailCnt;
    MS_S32              s32QueueReadIdx;
    MS_S32              s32MsgQueueID;
    MApiMB_Command   *pMApiMB_CommandQueue;
    MB_CommandCB     pMB_CommandCB;
} MApiMB_CommandInfo;

INTERFACE MS_U32 _MApiVirctualMB_CreateQueue (MAIL_BOX_ACTION MBaction,MS_U32 u32CommandClass, MS_U32 u32QueueDepth);
INTERFACE MS_U8 _MApiVirctualMB_SendToQueue (MApiMB_CommandInfo *pCmdInfo, MS_U32 u32CommandClass, MS_U8 *pu8Message, MS_U32 u32Size);
INTERFACE MS_U8 _MApiVirctualMB_RecvFromQueue (MApiMB_CommandInfo *pCmdInfo,MS_U32 u32CommandClass, MS_U8 *pu8Message, MS_U32 u32Size);

INTERFACE MS_U8 MApiMB_SendCommand(MAIL_BOX_ACTION MBaction,MApiMB_Command MApiMB_OutCommand);
INTERFACE MS_U8 MApiMB_RecvCommandFromQueue(MAIL_BOX_ACTION MBaction,MS_U32 u32CommandClass, MApiMB_Command *pMApiMB_Command,MS_U32 u32Flag);
INTERFACE MS_S32 MApiMB_RegisterCommandClass(MAIL_BOX_ACTION MBaction,MS_U32 u32CommandClass,
                      MB_CommandCB   pMB_CommandCB,
                      char         *pu32CommandClassName);
INTERFACE MS_U8 MApiVirctualMB_Init(MB_THREAD_MODE eThreadMode);
INTERFACE U32  MApiMB_GetMailBoxMsgCount (U32 u32CommandClass);
INTERFACE void MApiMB_Reset(void);
INTERFACE U32 MApiMB_MailBox_Clear(MAIL_BOX_ACTION MBaction,U32 u32CommandClass);

#endif
#undef INTERFACE

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus


#endif // #ifndef _MSAPI_MAILBOX_H_
