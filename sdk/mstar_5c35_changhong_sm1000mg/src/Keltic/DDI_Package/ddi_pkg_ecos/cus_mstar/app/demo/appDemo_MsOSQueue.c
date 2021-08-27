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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   appDemo_MsOSQueue.c
/// @author MStar Semiconductor Inc.
/// @brief  The demo code for the MsOS Queue functions 
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
   @file appDemo_MsOSQueue.c
   @brief The demo code for the MsOS Queue functions

*/
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "appDemo_MsOSQueue.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------

#define APP_DEMO_QUEUESEND_TASK_STACK         (4096)
#define APP_DEMO_QUEUERECV_TASK_STACK         (4096)

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------

extern MS_S32 gs32CachedPoolID;

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

static void *_pQueueSendTaskStack = NULL;
static MS_S32 _s32QueueSendTaskId = -1;

static void *_pQueueRecvTaskStack = NULL;
static MS_S32 _s32QueueRecvTaskId = -1;

static MS_S32 s32DemoQueueID = -1;
//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// app demo of Recv queue to handle queue events.
/// @return None
//-------------------------------------------------------------------------------------------------
static void _appDemo_QueueSend_Task(MS_U32 argc, void *argv)
{



    #if 1
    MS_BOOL bRet = FALSE;
    MS_U32 u32Message = 0x55;
    MS_U8 *pu8Message = (MS_U8 *)&u32Message;

    printf("Demo_QueueSend_Task\n");


    printf("Queue Send Tsk --> send message : 0x%lx to Queue \n",u32Message);

    bRet = MsOS_SendToQueue (s32DemoQueueID, pu8Message, sizeof(MS_U32),  5000 /*MSOS_WAIT_FOREVER*/);

    if(bRet == FALSE)
    {
        printf("MsOS_SendToQueue FAIL\n");
        GEN_EXCEP;
    }
    else
    {
        printf("MsOS_SendToQueue SUCCESS\n");
    }

    MsOS_DelayTask(3000);

    u32Message = 0xaa;

    printf("Queue Send Tsk --> send message : 0x%lx to Queue \n",u32Message);

    bRet = MsOS_SendToQueue (s32DemoQueueID, pu8Message, sizeof(MS_U32), 5000 /*MSOS_WAIT_FOREVER*/);

    if(bRet == FALSE)
    {
        printf("MsOS_SendToQueue FAIL\n");
        GEN_EXCEP;
    }
    else
    {
        printf("MsOS_SendToQueue SUCCESS\n");
    }

    #endif
    while(1)
    {
        MsOS_DelayTask(10000);
    }

}



//-------------------------------------------------------------------------------------------------
/// app demo of Recv queue to handle queue events.
/// @return None
//-------------------------------------------------------------------------------------------------
static void _appDemo_QueueRecv_Task(MS_U32 argc, void *argv)
{
    #if 1
    MS_BOOL bRet;
    MS_U32 u32Message;
    MS_U32 u32MessageSize;

    printf("Demo_QueueRecv_Task\n");

    bRet = MsOS_RecvFromQueue(s32DemoQueueID, (MS_U8 *)&u32Message, sizeof(MS_U32), &u32MessageSize, MSOS_WAIT_FOREVER);

    if(bRet == FALSE)
    {
        printf("MsOS_RecvFromQueue FAIL\n");
        GEN_EXCEP;
    }
    else
    {
        printf("Queue Recv Tsk --> recv message : 0x%lx from Queue \n",u32Message);
    }

    bRet = MsOS_PeekFromQueue(s32DemoQueueID, (MS_U8 *)&u32Message, sizeof(MS_U32), &u32MessageSize);

    if(bRet == FALSE)
    {
        printf("MsOS_PeekFromQueue FAIL\n");
        //GEN_EXCEP;
    }
    else
    {
        printf("Queue Recv Tsk --> peek message : 0x%lx from Queue \n",u32Message);
    }

     MsOS_DelayTask(5000);

    bRet = MsOS_PeekFromQueue(s32DemoQueueID, (MS_U8 *)&u32Message, sizeof(MS_U32), &u32MessageSize);

    if(bRet == FALSE)
    {
        printf("MsOS_PeekFromQueue FAIL\n");
        GEN_EXCEP;
    }
    else
    {
        printf("Queue Recv Tsk --> peek message : 0x%lx from Queue \n",u32Message);
    }
    #endif
    while(1)
    {
        MsOS_DelayTask(10000);
    }


}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/// @brief The MsOS Queue demo function. \n
///        The demo flow: \n
///          (1) Create Queue. \n
///          (2) create sent queue tsk. \n
///          (3) create receive queue tsk. \n
///          (4) sent queue tsk sent message to queue. \n
///          (5) receive queue tsk pick the message from queue. \n
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b Queue_Init \n
///
/// Sample Command: \b Queue_Init
///
//------------------------------------------------------------------------------
MS_BOOL Demo_MsOSQueue_Init(void)
{
    printf("Demo_MsOSQueue_Init \n");


    //

    s32DemoQueueID = MsOS_CreateQueue(NULL,         //It is useless now, can pass NULL.
                                    NULL,               // queue size (byte unit) : now fixed as 64 * u32MessageSize
                                    E_MSG_FIXED_SIZE,   // E_MSG_VAR_SIZE has not been supported yet
                                    sizeof(MS_U32),     // message size (byte unit) for E_MSG_FIXED_SIZE
                                    E_MSOS_FIFO,        // E_MSOS_FIFO suspended in FIFO order
                                    "Demo Queue");      // queue name
    if (s32DemoQueueID < 0)
    {
        printf("MsOS_CreateQueue FAIL\n");
        GEN_EXCEP;
    }

    // create app Queue Recv task for recv queue
    _pQueueSendTaskStack = MsOS_AllocateMemory( APP_DEMO_QUEUESEND_TASK_STACK, gs32CachedPoolID);
    if (_pQueueSendTaskStack == NULL)
    {
        GEN_EXCEP;
    }
    _s32QueueSendTaskId = MsOS_CreateTask(_appDemo_QueueSend_Task,
                                         NULL,
                                         E_TASK_PRI_HIGH, // E_TASK_PRI_MEDIUM,
                                         TRUE,
                                         _pQueueSendTaskStack,
                                         APP_DEMO_QUEUESEND_TASK_STACK,
                                         "Queue Send Tsk");
    if (_s32QueueSendTaskId < 0)
    {
        printf("MsOS_CreateTask QueueSend FAIL\n");
        GEN_EXCEP;
    }


    // create app Queue Recv task for recv queue
    _pQueueRecvTaskStack = MsOS_AllocateMemory( APP_DEMO_QUEUERECV_TASK_STACK, gs32CachedPoolID);
    if (_pQueueRecvTaskStack == NULL)
    {
        GEN_EXCEP;
    }
    _s32QueueRecvTaskId = MsOS_CreateTask(_appDemo_QueueRecv_Task,
                                         NULL,
                                         E_TASK_PRI_HIGH, // E_TASK_PRI_MEDIUM,
                                         TRUE,
                                         _pQueueRecvTaskStack,
                                         APP_DEMO_QUEUERECV_TASK_STACK,
                                         "Queue Recv Tsk");
    if (_s32QueueRecvTaskId < 0)
    {
        printf("MsOS_CreateTask QueueRecv FAIL\n");
        GEN_EXCEP;
    }



    return TRUE;

}


