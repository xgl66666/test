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
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   demo_os.h
/// @author MStar Semiconductor Inc.
/// @brief  The demo code for the watchdog timer functions
///////////////////////////////////////////////////////////////////////////////////////////////////
/**
  @file demo_os.c
  @brief The demo code for the OS functions
*/
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#if (DEMO_OS_TEST == 1)
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>

#if defined(MSOS_TYPE_LINUX)
#include <unistd.h>  // for sysconf
#endif

#include "MsMemory.h"
#include "MsCommon.h"
#include "MsOS.h"
#include "MsFS.h"
#include "demo_os.h"
#include "drvMMIO.h"
#include "demo_utility.h"
#include "drvDTC.h"
#include "SysBsp.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
#define TOTAL_TEST_CASE                 10
#define TEST_TASK_STACK_SIZE            0x2000
#define APP_DEMO_QUEUESEND_TASK_STACK   (4096)
#define APP_DEMO_QUEUERECV_TASK_STACK   (4096)
#define TimerPeriod                     3000 // Check timer period
#define REG_INT_IRQ_BASE                (0x1960)  //interrupt ctrl HOST1
#define REG_INT_IRQ_SET(x)              (REG_INT_IRQ_BASE + 2 * (x))

#define INVALID_TASK_ID     -1
#define INVALID_MUTEX_ID    -1
#define INVALID_SEM_ID      -1
#define INVALID_EVENT_ID    -1
#define INVALID_MEMPOOL_ID  -1

#define USB_TEST_WRITE_COMPLETE_EVENT     0x00000010
#define USB_TEST_READ_COMPLETE_EVENT     0x00000020
#define USB_TEST_READ_WRITE_COMPLETE_EVENT     0x00000030

#define TEST_EVENT_GROUP     0x0000000F
#define TEST_EVENT1          0x00000001
#define TEST_EVENT2          0x00000002
#define TEST_EVENT3          0x00000004
#define TEST_EVENT4          0x00000008
#define TEST_NO_EVENT        0x00000000

#define TEST_POOL_LENGTH     0x00010000

// Check OS_BDMA0_IRQ_REG and OS_BDMA0_ENABLE
#ifndef OS_BDMA0_IRQ_REG
#error "OS_BDMA0_IRQ_REG not defined in SysBsp.h"
#endif
#ifndef OS_BDMA0_ENABLE
#error "OS_BDMA0_ENABLE not defined in SysBsp.h"
#endif

//-------------------------------------------------------------------------------------------------
// For USB Test
//-------------------------------------------------------------------------------------------------

#if (DEMO_ZUI_TEST != 1)
typedef struct _USBTestArgs
{
    char *pMountPath;
    MS_U32 u32BlockSize;
    MS_U32 u32Iteration;
} USBTestArgs, *pUSBTestArgs;

static void _USB_Test_Clear_Cache(void);
static MS_U32 _USB_Test_Approximate(MS_U32 u32Numerator, MS_U32 u32Denominator);
#endif
static MS_BOOL _USB_Test_Prepare_Data(pUSBTestArgs setting);
static void _USB_Test_Write_Test(pUSBTestArgs setting);
static void _USB_Test_Read_Test(pUSBTestArgs setting);

static MS_S32 s32USBTestEventID = INVALID_EVENT_ID;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
typedef MS_BOOL (*func_ptr)(void *u32Input0);

typedef struct _OSFun
{
    MS_BOOL (*func_ptr)(void *u32Input0);
} OSFun, *pOSFun;

static MS_BOOL _OS_Test_Init(void);
static MS_BOOL _OS_Test_Task(void);
static MS_BOOL _OS_Test_Mutex(void);
static MS_BOOL _OS_Test_Sem(void);
static MS_BOOL _OS_Test_Event(void);
static MS_BOOL _OS_Test_Queue(void);
static MS_BOOL _OS_Test_Interrupt(void);
static MS_BOOL _OS_Test_Timer(void);
static MS_BOOL _OS_Test_Address(void);
static MS_BOOL _OS_Test_Pool(void);
#if defined(MSOS_TYPE_LINUX) && defined(KERNEL_POOL2_ADR) && defined(KERNEL_POOL2_LEN)
static MS_BOOL _OS_Test_LX2(void);
#endif

static OSFun OSDemoFun[]=
{
    {(func_ptr)&_OS_Test_Init},
    {(func_ptr)&_OS_Test_Task},
    {(func_ptr)&_OS_Test_Mutex},
    {(func_ptr)&_OS_Test_Sem},
    {(func_ptr)&_OS_Test_Event},
    {(func_ptr)&_OS_Test_Queue},
    {(func_ptr)&_OS_Test_Interrupt},
    {(func_ptr)&_OS_Test_Timer},
    {(func_ptr)&_OS_Test_Address},
    {(func_ptr)&_OS_Test_Pool}
#if defined(MSOS_TYPE_LINUX) && defined(KERNEL_POOL2_ADR) && defined(KERNEL_POOL2_LEN)
    ,{(func_ptr)&_OS_Test_LX2}
#endif
};

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U8 _OS_Test_Task_T1_Counter = 0;
static MS_U8 _OS_Test_Task_T2_Counter = 0;
static void *_pQueueSendTaskStack = NULL;
static void *_pQueueRecvTaskStack = NULL;
static MS_S32 _s32QueueSendTaskId = -1;
static MS_S32 _s32QueueRecvTaskId = -1;
static MS_S32 s32DemoQueueID = -1;
static MS_S32 _s32TestMutexId = INVALID_MUTEX_ID;
static MS_S32 _s32TestSemId = INVALID_SEM_ID;
static MS_S32 _s32TestEventID = INVALID_EVENT_ID;
static MS_S32 _s32TestMemPoolID = INVALID_MEMPOOL_ID;
static MS_S32 _s32CheckRxTimerId = -1;
static MS_U32 u32NonPmBase = 0;

static MS_BOOL bTask3Active = FALSE;
static MS_BOOL bTask4Active = FALSE;
static MS_BOOL bTask5Active = FALSE;
static MS_BOOL bTimerCB = FALSE;
static MS_BOOL bTaskSendActive = TRUE;
static MS_BOOL bTaskRecvActive = TRUE;
static MS_BOOL bInterruptCB = FALSE;

//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------
#define NonPM_Write2Byte(addr)  *((volatile MS_U16 *)(u32NonPmBase + ((addr) << 1)))

MS_U16 NonPM_Read2Byte(MS_U32 addr)
{
    return *((volatile MS_U16 *)(u32NonPmBase + (addr << 1)));
}

static void _OS_Timeout_Handler(MS_U32 stTimer, MS_U32 u32TimerID)
{
    bTimerCB = TRUE;
    printf("[%s] Current time %"DTC_MS_U32_d"\n", __FUNCTION__, MsOS_GetSystemTime());
}

static void _OS_Interrupt_Handler(MS_U32 u32Vector)
{
    bInterruptCB = TRUE;
    printf("[%s] Interrupt success!!!\n", __FUNCTION__);
    NonPM_Write2Byte(OS_BDMA0_IRQ_REG) = NonPM_Read2Byte(OS_BDMA0_IRQ_REG) & ~OS_BDMA0_ENABLE;
}

static MS_BOOL _OS_Test_Init(void)
{
    MS_BOOL bRet = TRUE;
    // prepare resource for OS Test
    // makesure MsOS_Init is called

    return bRet;
}

static void _OS_Test_Task_T1(void)
{
    MS_U8 i;

    for (i = 0; i < 10; i++)
    {
        printf("Tick of T1 %d\n", i);
        MsOS_DelayTask(100);
        _OS_Test_Task_T1_Counter++;
    }
}

static void _OS_Test_Task_T2(void)
{
    MS_U8 i ;

    for (i = 0; i < 10; i++)
    {
        printf("Tick of T2 %d\n", i);
        MsOS_DelayTask(50);
        _OS_Test_Task_T2_Counter++;
    }
}
#if 1
static void _OS_Test_Task_T3(void)
{
    printf("[%s][%d] Start %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, MsOS_GetSystemTime());
    MsOS_ObtainMutex(_s32TestMutexId, MSOS_WAIT_FOREVER);
    bTask3Active = TRUE;
    MsOS_DelayTask(1000);
    MsOS_ReleaseMutex(_s32TestMutexId);
    printf("[%s][%d] End %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, MsOS_GetSystemTime());
}
#endif
static void _OS_Test_Task_T4(void)
{
    printf("[%s][%d] Start %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, MsOS_GetSystemTime());
    MsOS_DelayTask(100);
    printf("[%s][%d] Release Semaphore %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, MsOS_GetSystemTime());
    MsOS_ReleaseSemaphore(_s32TestSemId);

    MsOS_DelayTask(1000);
    printf("[%s][%d] Obtain Semaphore %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, MsOS_GetSystemTime());
    MsOS_ObtainSemaphore(_s32TestSemId, MSOS_WAIT_FOREVER);
    bTask4Active = TRUE;
    printf("[%s][%d] End %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, MsOS_GetSystemTime());
}

static void _OS_Test_Task_T5(void)
{
    MS_U8 i;

    printf("[%s][%d] Start %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, MsOS_GetSystemTime());
    for (i = 0; i < 4; i++)
    {
        MsOS_DelayTask(1000);
        MsOS_SetEvent(_s32TestEventID, TEST_EVENT1);
        MsOS_DelayTask(1000);
        MsOS_SetEvent(_s32TestEventID, TEST_EVENT2);
        MsOS_DelayTask(1000);
        MsOS_SetEvent(_s32TestEventID, TEST_EVENT3);
        MsOS_DelayTask(1000);
        MsOS_SetEvent(_s32TestEventID, TEST_EVENT4);
    }
    bTask5Active = TRUE;

    printf("[%s][%d] End %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, MsOS_GetSystemTime());
}

static MS_BOOL _OS_Test_Task_0(TaskPriority ePriotiry1, TaskPriority ePriotiry2, MS_S32* pTask1_id, MS_S32* pTask2_id, void* pStackBase1, void* pStackBase2)
{
    MS_BOOL bRet = FALSE;

    /*  Create Task 1 */
    *pTask1_id = MsOS_CreateTask((TaskEntry)_OS_Test_Task_T1,
                                 0,
                                 ePriotiry1, // E_TASK_PRI_MEDIUM,
                                 TRUE,
                                 pStackBase1,
                                 TEST_TASK_STACK_SIZE,
                                 "_OS_Test_Task_T1");
    if (INVALID_TASK_ID == *pTask1_id)
    {
        printf("[%s][%d] Create Task Failed  INVALID_TASK_ID\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
        return bRet;
    }
    else
    {
        bRet = TRUE;
        printf("[%s][%d] %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, *pTask1_id);
    }
    *pTask2_id = MsOS_CreateTask((TaskEntry)_OS_Test_Task_T2,
                                 0,
                                 ePriotiry2, // E_TASK_PRI_MEDIUM,
                                 TRUE,
                                 pStackBase2,
                                 TEST_TASK_STACK_SIZE,
                                 "_OS_Test_Task_T2");
    if (INVALID_TASK_ID == *pTask2_id)
    {
        printf("[%s][%d] Create Task Failed INVALID_TASK_ID\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
        return bRet;
    }
    else
    {
        bRet = TRUE;
        printf("[%s][%d] %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, *pTask2_id);
    }

    return bRet;
}

static MS_BOOL _OS_Test_Task(void)
{
    MS_BOOL bRet = FALSE;
    TaskPriority ePriotiry1 = E_TASK_PRI_HIGH, ePriotiry2 = E_TASK_PRI_HIGH;
    void *pStackbase1 = NULL, *pStackbase2 = NULL;
    MS_S32 s32Task1_id = INVALID_TASK_ID, s32Task2_id = INVALID_TASK_ID;
    MS_BOOL bFailedFlag = FALSE;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);

    printf("[%s] Start\n", __FUNCTION__);
    _OS_Test_Task_T1_Counter = 0;
    _OS_Test_Task_T2_Counter = 0;

    pStackbase1 = MsOS_AllocateMemory(TEST_TASK_STACK_SIZE, s32MstarNonCachedPoolID);
    if (pStackbase1 == NULL)
    {
        printf("[%s][%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    pStackbase2 = MsOS_AllocateMemory(TEST_TASK_STACK_SIZE,s32MstarNonCachedPoolID);
    if (pStackbase2 == NULL)
    {
        bRet = MsOS_FreeMemory(pStackbase1,s32MstarNonCachedPoolID);
        printf("[%s][%d] bRet %d\n", __FUNCTION__, __LINE__,bRet);
        return FALSE;
    }

    bRet = _OS_Test_Task_0(ePriotiry1,ePriotiry2,&s32Task1_id ,&s32Task2_id ,pStackbase1,pStackbase2);

    while((s32Task1_id != INVALID_TASK_ID) || (s32Task2_id != INVALID_TASK_ID) )
    {
        if ((s32Task1_id != INVALID_TASK_ID) && (_OS_Test_Task_T1_Counter == 10))
        {
            /* not to call delete task */
            bRet = MsOS_FreeMemory(pStackbase1,s32MstarNonCachedPoolID);
            printf("[%s][%d] bRet %d  %"DTC_MS_U32_x"\n", __FUNCTION__, __LINE__,bRet, (MS_U32)pStackbase1);
            s32Task1_id = INVALID_TASK_ID;
        }

        if ((s32Task2_id != INVALID_TASK_ID) && (_OS_Test_Task_T2_Counter == 10))
        {
            /* not to call delete task */
            bRet = MsOS_FreeMemory(pStackbase2,s32MstarNonCachedPoolID);
            printf("[%s][%d] bRet %d  %"DTC_MS_U32_x"\n", __FUNCTION__, __LINE__,bRet, (MS_U32)pStackbase2);
            s32Task2_id = INVALID_TASK_ID;
        }
        MsOS_DelayTask(50);
        printf("[%s][%d] Check ing %d  %d\n", __FUNCTION__, __LINE__,_OS_Test_Task_T1_Counter,_OS_Test_Task_T2_Counter);
    }

    bRet = bRet & (!bFailedFlag);
    return bRet;
}
//-------------------------------------------------------------------------------------------------
/// app demo of Recv queue to handle queue events.
/// @return None
//-------------------------------------------------------------------------------------------------
static void _OS_Test_QueueSend_Task(MS_U32 argc, void *argv)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32Message = 0x55;
    MS_U8 *pu8Message = (MS_U8 *)&u32Message;

    bTaskSendActive = TRUE;
    printf("Demo_QueueSend_Task\n");

    printf("Queue Send Tsk --> send message : 0x%"DTC_MS_U32_x" to Queue\n", u32Message);

    bRet = MsOS_SendToQueue(s32DemoQueueID, pu8Message, sizeof(MS_U32), 5000 /*MSOS_WAIT_FOREVER*/);

    if (bRet == FALSE)
    {
        printf("MsOS_SendToQueue FAIL\n");
        return ;
    }
    else
    {
        printf("MsOS_SendToQueue SUCCESS\n");
    }

    MsOS_DelayTask(3000);

    u32Message = 0xaa;

    printf("Queue Send Tsk --> send message : 0x%"DTC_MS_U32_x" to Queue\n", u32Message);

    bRet = MsOS_SendToQueue(s32DemoQueueID, pu8Message, sizeof(MS_U32), 5000 /*MSOS_WAIT_FOREVER*/);

    if (bRet == FALSE)
    {
        printf("MsOS_SendToQueue FAIL\n");
        return ;
    }
    else
    {
        printf("MsOS_SendToQueue SUCCESS\n");
    }

    MsOS_DelayTask(1000);
    bTaskSendActive = FALSE;
}

static void _OS_Test_QueueRecv_Task(MS_U32 argc, void *argv)
{
    MS_BOOL bRet;
    MS_U32 u32Message;
    MS_U32 u32MessageSize;

    bTaskRecvActive = TRUE;
    printf("Demo_QueueRecv_Task\n");

    bRet = MsOS_RecvFromQueue(s32DemoQueueID, (MS_U8 *)&u32Message, sizeof(MS_U32), &u32MessageSize, MSOS_WAIT_FOREVER);

    if (bRet == FALSE)
    {
        printf("MsOS_RecvFromQueue FAIL\n");
        return ;
    }
    else
    {
        printf("Queue Recv Tsk --> recv message : 0x%"DTC_MS_U32_x" from Queue\n", u32Message);
    }

    bRet = MsOS_PeekFromQueue(s32DemoQueueID, (MS_U8 *)&u32Message, sizeof(MS_U32), &u32MessageSize);

    if (bRet == FALSE)
    {
        printf("MsOS_PeekFromQueue FAIL\n");
    }
    else
    {
        printf("Queue Recv Tsk --> peek message : 0x%"DTC_MS_U32_x" from Queue\n", u32Message);
    }

    MsOS_DelayTask(5000);

    bRet = MsOS_PeekFromQueue(s32DemoQueueID, (MS_U8 *)&u32Message, sizeof(MS_U32), &u32MessageSize);

    if (bRet == FALSE)
    {
        printf("MsOS_PeekFromQueue FAIL\n");
        return ;
    }
    else
    {
        printf("Queue Recv Tsk --> peek message : 0x%"DTC_MS_U32_x" from Queue\n", u32Message);
    }

    MsOS_DelayTask(1000);
    bTaskRecvActive = FALSE;
}

static MS_BOOL _OS_Test_Mutex(void)
{
    MS_BOOL bRet = FALSE, bFailedFlag = FALSE;
    void *pStackbase3 = NULL;
    MS_S32 s32Task3_id = INVALID_TASK_ID;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);

    printf("[%s] Start\n", __FUNCTION__);
    bTask3Active = FALSE;

    /* Mutex Create */
    _s32TestMutexId = INVALID_MUTEX_ID;
    _s32TestMutexId = MsOS_CreateMutex(E_MSOS_FIFO, "TestMutex", MSOS_PROCESS_SHARED);
    if (_s32TestMutexId == INVALID_MUTEX_ID)
    {
        printf("[%s][%d] Mutex Create Failed\n", __FUNCTION__, __LINE__);
        return bRet;
    }
    else
    {
        printf("[%s][%d] Create Mutex OK\n", __FUNCTION__, __LINE__);
    }

    /* Create another thread to test mutex */
    /* Create Task 3 */
    pStackbase3 = MsOS_AllocateMemory(TEST_TASK_STACK_SIZE, s32MstarNonCachedPoolID);
    if (pStackbase3 == NULL)
    {
        printf("[%s][%d]\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    s32Task3_id = MsOS_CreateTask((TaskEntry)_OS_Test_Task_T3,
                                  0,
                                  E_TASK_PRI_MEDIUM,
                                  TRUE,
                                  pStackbase3,
                                  TEST_TASK_STACK_SIZE,
                                  "Test_Task_T3");
    if (INVALID_TASK_ID == s32Task3_id)
    {
        printf("[%s][%d] Create Task Failed  INVALID_TASK_ID\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
        return bRet;
    }
    else
    {
        bRet = TRUE;
        printf("[%s][%d] %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, s32Task3_id);
    }

    while (1)
    {
        if (bTask3Active)
        {
            bTask3Active = FALSE;
            printf("[%s][%d] Check should be Mutex obtain failed\n", __FUNCTION__, __LINE__);
            if (MsOS_ObtainMutex(_s32TestMutexId, 100) == FALSE)
            {
                printf("[%s][%d] Success - Obtain 1 must fail\n", __FUNCTION__, __LINE__);
            }
            else
            {
                bFailedFlag = TRUE;
                break;
            }

            MsOS_DelayTask(1000);
            printf("[%s][%d] Check should be Mutex obtain success\n", __FUNCTION__, __LINE__);
            if (MsOS_ObtainMutex(_s32TestMutexId, 100) == TRUE)
            {
                printf("[%s][%d] Obtain 2 OK\n", __FUNCTION__, __LINE__);
                MsOS_ReleaseMutex(_s32TestMutexId);
                break;
            }
            else
            {
                bFailedFlag = TRUE;
                break;
            }
        }
        MsOS_DelayTask(100);
    }
    /* not to call delete task */

    if (MsOS_DeleteMutex(_s32TestMutexId) == FALSE)
    {
        printf("[%s][%d] Delete Mutex Failed\n", __FUNCTION__, __LINE__);
        bFailedFlag = TRUE;
    }
    else
    {
        printf("[%s][%d] Delete Mutex OK\n", __FUNCTION__, __LINE__);
    }

    if (MsOS_FreeMemory(pStackbase3,s32MstarNonCachedPoolID) == FALSE)
    {
        bFailedFlag = TRUE;
    }

    if (bFailedFlag == TRUE)
    {
        bRet = FALSE;
    }
    else
    {
        bRet = TRUE;
    }

    return bRet;
}

static MS_BOOL _OS_Test_Sem(void)
{
    MS_BOOL bRet = FALSE,bFailedFlag = FALSE;
    MS_U32 u32InitCnt = 0;
    MsOSAttribute sMsOSAttribute = E_MSOS_FIFO;
    MS_U8 u8char[20] = {0};
    void *pStackbase4 = NULL;
    MS_S32 s32Task4_id = INVALID_TASK_ID;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);

    printf("[%s] Start\n",__FUNCTION__);

    _s32TestSemId = INVALID_SEM_ID;
    bTask4Active = FALSE;

    _s32TestSemId = MsOS_CreateSemaphore(0,E_MSOS_FIFO,"Test_Sem");

    if (_s32TestSemId == INVALID_SEM_ID)
    {
        printf("[%s][%d] Create Semaphore Failed\n", __FUNCTION__, __LINE__);
        return bRet;
    }
    else
    {
        printf("[%s][%d] Create Semaphore OK\n", __FUNCTION__, __LINE__);
        if (MsOS_InfoSemaphore(_s32TestSemId ,&u32InitCnt,&sMsOSAttribute,(char *)&u8char[0]) == FALSE)
        {
            bFailedFlag = TRUE;
            printf("[%s][%d] Get Semaphore Information Failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            printf("[%s][%d] Get Semaphore Information OK\n", __FUNCTION__, __LINE__);
            printf("[%s][%d] Init Cnt          %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, u32InitCnt);
            printf("[%s][%d] sMsOSAttribute    %d\n", __FUNCTION__, __LINE__,sMsOSAttribute);
            printf("[%s][%d] Name          %s\n", __FUNCTION__, __LINE__,u8char);
        }
    }
    /* Create another thread to test mutex */
    /*  Create Task 4 */
    pStackbase4 = MsOS_AllocateMemory(TEST_TASK_STACK_SIZE,s32MstarNonCachedPoolID);
    if (pStackbase4 == NULL)
    {
        printf("[%s][%d]\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    s32Task4_id = MsOS_CreateTask((TaskEntry)_OS_Test_Task_T4,
                                  0,
                                  E_TASK_PRI_MEDIUM,
                                  TRUE,
                                  pStackbase4,
                                  TEST_TASK_STACK_SIZE,
                                  "Test_Task_T4");
    if (INVALID_TASK_ID == s32Task4_id)
    {
        printf("[%s][%d] Create Task Failed  INVALID_TASK_ID\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
        return bRet;
    }
    else
    {
        bRet = TRUE;
        printf("[%s][%d] %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, s32Task4_id);
    }

    MsOS_DelayTask(1000);
    printf("[%s][%d] Obtain Semaphore %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, MsOS_GetSystemTime());
    if (MsOS_InfoSemaphore(_s32TestSemId ,&u32InitCnt,&sMsOSAttribute,(char *)&u8char[0]) == TRUE)
    {
        printf("[%s][%d] Get Semaphore Information OK\n", __FUNCTION__, __LINE__);
        printf("[%s][%d] Init Cnt          %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, u32InitCnt);
        printf("[%s][%d] sMsOSAttribute    %d\n", __FUNCTION__, __LINE__,sMsOSAttribute);
        printf("[%s][%d] Name          %s\n", __FUNCTION__, __LINE__,u8char);
    }

    if (MsOS_ObtainSemaphore(_s32TestSemId,MSOS_WAIT_FOREVER)== FALSE)
    {
        printf("[%s][%d] Obtain Semaphore Failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        printf("[%s][%d] Obtain Semaphore OK\n", __FUNCTION__, __LINE__);
    }
    MsOS_DelayTask(100);
    printf("[%s][%d] Release Semaphore %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, MsOS_GetSystemTime());
    if (MsOS_ReleaseSemaphore(_s32TestSemId) == FALSE)
    {
        printf("[%s][%d] Release Semaphore Failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        printf("[%s][%d] Release Semaphore OK\n", __FUNCTION__, __LINE__);
    }

    while(bTask4Active == FALSE)
    {
        printf("Waiting for Task 4 Finish\n");
        MsOS_DelayTask(100);
    }

    printf("Task 4 Finished\n");

    /* not to call delete task */

    if (MsOS_DeleteSemaphore(_s32TestSemId)== FALSE)
    {
        printf("[%s][%d] Delete Semaphore Failed\n", __FUNCTION__, __LINE__);
        bFailedFlag = TRUE;
    }
    else
    {
        printf("[%s][%d] Delete Semaphore OK\n", __FUNCTION__, __LINE__);
    }

    if (MsOS_FreeMemory(pStackbase4,s32MstarNonCachedPoolID)== FALSE)
    {
        bFailedFlag = TRUE;
    }

    if (bFailedFlag == TRUE)
    {
        bRet = FALSE;
    }
    else
    {
        bRet = TRUE;
    }

    return bRet;
}

static MS_BOOL _OS_Test_Event(void)
{
    MS_BOOL bRet = FALSE, bFailedFlag = FALSE;
    void *pStackbase5 = NULL;
    MS_S32 s32Task5_id = INVALID_TASK_ID;
    MS_U32 u32Events = TEST_NO_EVENT;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);

    printf("[%s] Start\n",__FUNCTION__);
    _s32TestEventID = INVALID_EVENT_ID;

    _s32TestEventID = MsOS_CreateEventGroup("Test_Event");

    if (_s32TestEventID == INVALID_EVENT_ID)
    {
        printf("[%s][%d] Create Event Group Failed\n", __FUNCTION__, __LINE__);
        return bRet;
    }
    else
    {
        printf("[%s][%d] Create Event Group OK\n", __FUNCTION__, __LINE__);
    }

    /* Create another thread to test mutex */
    /*  Create Task 5 */
    pStackbase5 = MsOS_AllocateMemory(TEST_TASK_STACK_SIZE,s32MstarNonCachedPoolID);
    if (pStackbase5 == NULL)
    {
        printf("[%s][%d]\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    s32Task5_id = MsOS_CreateTask((TaskEntry)_OS_Test_Task_T5,
                                  0,
                                  E_TASK_PRI_MEDIUM,
                                  TRUE,
                                  pStackbase5,
                                  TEST_TASK_STACK_SIZE,
                                  "Test_Task_T5");
    if (INVALID_TASK_ID == s32Task5_id)
    {
        printf("[%s][%d] Create Task Failed  INVALID_TASK_ID\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
        return bRet;
    }
    else
    {
        bRet = TRUE;
        printf("[%s][%d] %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, s32Task5_id);
    }

    while(1)
    {
        // Wait Event Demo
        MsOS_WaitEvent(_s32TestEventID, TEST_EVENT_GROUP, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        printf("[%s][%d] u32Events %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, u32Events);
        if (u32Events == TEST_EVENT4)
        {
            printf("[%s][%d] Pass Case 1\n", __FUNCTION__, __LINE__);
            break;
        }
    }

    while(1)
    {
        // Wait Event Demo
        MsOS_WaitEvent(_s32TestEventID, TEST_EVENT_GROUP, &u32Events, E_OR, MSOS_WAIT_FOREVER);
        printf("[%s][%d] u32Events %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, u32Events);

        // you need to cleae event at this case or system will recieve, even you do not send any events....
        MsOS_ClearEvent(_s32TestEventID,u32Events);
        if (u32Events == TEST_EVENT4)
        {
            printf("[%s][%d] Pass Case 2\n", __FUNCTION__, __LINE__);
            break;
        }
    }

    while(1)
    {
        // Wait Event Demo
        MsOS_WaitEvent(_s32TestEventID, TEST_EVENT1, &u32Events, E_AND, MSOS_WAIT_FOREVER);
        printf("[%s][%d] u32Events %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, u32Events);
        // you need to cleae event at this case or system will recieve, even you do not send any events....
        MsOS_ClearEvent(_s32TestEventID,u32Events);

        if (u32Events == TEST_EVENT1)
        {
            printf("[%s][%d] Pass Case 3-1\n", __FUNCTION__, __LINE__);
        }

        MsOS_WaitEvent(_s32TestEventID, TEST_EVENT2, &u32Events, E_AND, MSOS_WAIT_FOREVER);
        printf("[%s][%d] u32Events %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, u32Events);
        // you need to cleae event at this case or system will recieve, even you do not send any events....
        MsOS_ClearEvent(_s32TestEventID,u32Events);

        if (u32Events == TEST_EVENT2)
        {
            printf("[%s][%d] Pass Case 3-2\n", __FUNCTION__, __LINE__);
        }

        MsOS_WaitEvent(_s32TestEventID, TEST_EVENT3, &u32Events, E_AND, MSOS_WAIT_FOREVER);
        printf("[%s][%d] u32Events %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, u32Events);
        // you need to cleae event at this case or system will recieve, even you do not send any events....
        MsOS_ClearEvent(_s32TestEventID,u32Events);

        if (u32Events == TEST_EVENT3)
        {
            printf("[%s][%d] Pass Case 3-3\n", __FUNCTION__, __LINE__);
        }

        MsOS_WaitEvent(_s32TestEventID, TEST_EVENT4, &u32Events, E_AND, MSOS_WAIT_FOREVER);
        printf("[%s][%d] u32Events %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, u32Events);
        // you need to cleae event at this case or system will recieve, even you do not send any events....
        MsOS_ClearEvent(_s32TestEventID,u32Events);

        if (u32Events == TEST_EVENT4)
        {
            printf("[%s][%d] Pass Case 3-4\n", __FUNCTION__, __LINE__);
            break;
        }
    }

    while(1)
    {
        // Wait Event Demo
        MsOS_WaitEvent(_s32TestEventID, TEST_EVENT1, &u32Events, E_AND_CLEAR, MSOS_WAIT_FOREVER);
        printf("[%s][%d] u32Events %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, u32Events);


        if (u32Events == TEST_EVENT1)
        {
            printf("[%s][%d] Pass Case 4-1\n", __FUNCTION__, __LINE__);
        }

        MsOS_WaitEvent(_s32TestEventID, TEST_EVENT2, &u32Events, E_AND_CLEAR, MSOS_WAIT_FOREVER);
        printf("[%s][%d] u32Events %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, u32Events);


        if (u32Events == TEST_EVENT2)
        {
            printf("[%s][%d] Pass Case 4-2\n", __FUNCTION__, __LINE__);
        }

        MsOS_WaitEvent(_s32TestEventID, TEST_EVENT3, &u32Events, E_AND_CLEAR, MSOS_WAIT_FOREVER);
        printf("[%s][%d] u32Events %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, u32Events);


        if (u32Events == TEST_EVENT3)
        {
            printf("[%s][%d] Pass Case 4-3\n", __FUNCTION__, __LINE__);
        }

        MsOS_WaitEvent(_s32TestEventID, TEST_EVENT4, &u32Events, E_AND_CLEAR, MSOS_WAIT_FOREVER);
        printf("[%s][%d] u32Events %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, u32Events);


        if (u32Events == TEST_EVENT4)
        {
            printf("[%s][%d] Pass Case 4-4\n", __FUNCTION__, __LINE__);
            break;
        }

    }

    while(bTask5Active == FALSE)
    {
        printf("Waiting for Task 5 Finish\n");
        MsOS_DelayTask(100);
    }

    printf("Task 5 Finished\n");

    /* not to call delete task */

    if (MsOS_DeleteEventGroup(_s32TestEventID)== FALSE)
    {
        printf("[%s][%d] Delete Semaphore Failed\n", __FUNCTION__, __LINE__);
        bFailedFlag = TRUE;
    }
    else
    {
        printf("[%s][%d] Delete Semaphore OK\n", __FUNCTION__, __LINE__);
    }

    if (MsOS_FreeMemory(pStackbase5,s32MstarNonCachedPoolID)== FALSE)
    {
        bFailedFlag = TRUE;
    }

    if (bFailedFlag == TRUE)
    {
        bRet = FALSE;
    }
    else
    {
        bRet = TRUE;
    }

    return bRet;
}

static MS_BOOL _OS_Test_Queue(void)
{
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE, &s32MstarCachedPoolID);

    printf("[%s] Start\n",__FUNCTION__);
    s32DemoQueueID = MsOS_CreateQueue(NULL,               //It is useless now, can pass NULL.
                                      (MS_U32)NULL,               // queue size (byte unit) : now fixed as 10 * u32MessageSize
                                      E_MSG_FIXED_SIZE,   // E_MSG_VAR_SIZE has not been supported yet
                                      sizeof(MS_U32),     // message size (byte unit) for E_MSG_FIXED_SIZE
                                      E_MSOS_FIFO,        // E_MSOS_FIFO suspended in FIFO order
                                      "Demo Queue");      // queue name
    if (s32DemoQueueID < 0)
    {
        printf("MsOS_CreateQueue FAIL\n");
        return FALSE;
    }

    // create app Queue Recv task for recv queue
    _pQueueSendTaskStack = MsOS_AllocateMemory(APP_DEMO_QUEUESEND_TASK_STACK, s32MstarCachedPoolID);
    if (_pQueueSendTaskStack == NULL)
    {
        printf("MsOS_AllocateMemory FAIL\n");
        return FALSE;
    }
    _s32QueueSendTaskId = MsOS_CreateTask(_OS_Test_QueueSend_Task,
                                          (MS_VIRT)NULL,
                                          E_TASK_PRI_HIGH, // E_TASK_PRI_MEDIUM,
                                          TRUE,
                                          _pQueueSendTaskStack,
                                          APP_DEMO_QUEUESEND_TASK_STACK,
                                          "Queue Send Tsk");
    if (_s32QueueSendTaskId < 0)
    {
        printf("MsOS_CreateTask QueueSend FAIL\n");
        return FALSE;
    }

    // create app Queue Recv task for recv queue
    _pQueueRecvTaskStack = MsOS_AllocateMemory(APP_DEMO_QUEUERECV_TASK_STACK, s32MstarCachedPoolID);
    if (_pQueueRecvTaskStack == NULL)
    {
        printf("MsOS_AllocateMemory FAIL\n");
        return FALSE;
    }
    _s32QueueRecvTaskId = MsOS_CreateTask(_OS_Test_QueueRecv_Task,
                                          (MS_VIRT)NULL,
                                          E_TASK_PRI_HIGH, // E_TASK_PRI_MEDIUM,
                                          TRUE,
                                          _pQueueRecvTaskStack,
                                          APP_DEMO_QUEUERECV_TASK_STACK,
                                          "Queue Recv Tsk");

    if (_s32QueueRecvTaskId < 0)
    {
        printf("MsOS_CreateTask QueueRecv FAIL\n");
        return FALSE;
    }

    while (bTaskSendActive || bTaskRecvActive)
    {
        MsOS_DelayTask(1000);
    }

    /* not to call delete task */

    return TRUE;
}

static MS_BOOL _OS_Test_Interrupt(void)
{
    MS_BOOL bRet = TRUE;
    MS_VIRT virtMMIOBaseAdr;
    MS_PHY phyBankSize;
    MS_U32 u32Count = 0;

    printf("[%s] Start test with BDMA0 pin\n", __FUNCTION__);
    if (!MDrv_MMIO_GetBASE(&virtMMIOBaseAdr, &phyBankSize, MS_MODULE_HW))
    {
        printf("Get IOMap failure\n");
        return FALSE;
    }
    else
    {
        printf("Get IOMap u32NonPmBase = 0x%"DTC_MS_VIRT_x"\n", virtMMIOBaseAdr);
    }

    bInterruptCB = FALSE;
    u32NonPmBase = virtMMIOBaseAdr;
    MsOS_AttachInterrupt(E_INT_IRQ_BDMA0, (InterruptCb)_OS_Interrupt_Handler);
    MsOS_EnableInterrupt(E_INT_IRQ_BDMA0);
    MsOS_DelayTask(1000);
    NonPM_Write2Byte(OS_BDMA0_IRQ_REG) = NonPM_Read2Byte(OS_BDMA0_IRQ_REG) | OS_BDMA0_ENABLE;

    while(u32Count++ < 3)
    {
        MsOS_DelayTask(1000);
        if (bInterruptCB)
        {
            break;
        }
        else
        {
            printf("[%s] Waiting for BDMA0 interrupt...\n", __FUNCTION__);
        }
    }
    if (bInterruptCB)
    {
        printf("[%s] Interrupt test OK!!\n", __FUNCTION__);
    }
    else
    {
        printf("[%s] Interrupt test FAILED!!, please check your IRQ table\n", __FUNCTION__);
    }

    return bRet;
}

static MS_BOOL _OS_Test_Timer(void)
{
    MS_BOOL bRet = TRUE;

    // Create timer for timing monitor
    if (_s32CheckRxTimerId < 0)
    {
        printf("[%s] Start timer at %"DTC_MS_U32_d", wait 3s\n", __FUNCTION__, MsOS_GetSystemTime());
        _s32CheckRxTimerId = MsOS_CreateTimer(_OS_Timeout_Handler,
                                              TimerPeriod,
                                              TimerPeriod,
                                              TRUE,
                                              "TimerCheck");
        if (_s32CheckRxTimerId < 0)
        {
            bRet = FALSE;
            printf("[%s][%d] Create timer fail!!\n", __FUNCTION__, __LINE__);
        }
    }
    else
    {
        MsOS_StartTimer(_s32CheckRxTimerId);
    }

    MsOS_DelayTask(4000);
    MsOS_DeleteTimer(_s32CheckRxTimerId);
    _s32CheckRxTimerId = -1;

    return bRet;
}

static MS_BOOL _OS_Test_Address(void)
{
    MS_BOOL bRet = TRUE;
    MS_U32 *TAG = NULL,*TAG_C = NULL;
    MS_U32 TAG_PA = (MS_U32)NULL;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);
    printf("[%s] Start\n",__FUNCTION__);
    // 1.  VA2PA
    TAG = MsOS_AllocateMemory(sizeof(MS_U32),s32MstarNonCachedPoolID);
    if (TAG == NULL)
    {
        printf("[%s][%d]\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    *TAG = 0xDEADBEEF;
    printf("TAG VA 0x%"DTC_MS_U32_x"\n", (MS_U32)TAG);
    printf("TAG VA Value = 0x%08"DTC_MS_U32_x"\n", *TAG);
    TAG_PA= MsOS_VA2PA((MS_U32)TAG);
    if (TAG_PA == (MS_U32)NULL)
    {
        printf("[%s][%d] MsOS_VA2PA Failed\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    printf("[%s][%d] Please Check PA TAG  with Tool PA 0x%"DTC_MS_U32_x"\n", __FUNCTION__, __LINE__, TAG_PA);

    TAG = (MS_U32 *)MsOS_PA2KSEG1(TAG_PA);
    TAG_C = (MS_U32 *)MsOS_PA2KSEG0(TAG_PA);


    if (TAG == NULL)
    {
        printf("[%s][%d] MsOS_PA2KSEG1 Failed\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        printf("[%s][%d] TAG SEG1 0x%"DTC_MS_U32_x"\n", __FUNCTION__, __LINE__, (MS_U32)TAG);
    }

    if (TAG_C == NULL)
    {
        printf("[%s][%d] MsOS_PA2KSEG0 Failed\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        printf("[%s][%d] TAG SEG0 0x%"DTC_MS_U32_x"\n", __FUNCTION__, __LINE__, (MS_U32)TAG_C);
    }

    if (MsOS_FreeMemory(TAG,s32MstarNonCachedPoolID)== FALSE)
    {
        printf("[%s][%d]\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }


    return bRet;
}

static MS_BOOL _OS_Test_Pool(void)
{
    MS_BOOL bRet = TRUE;
    MS_U32 u32TestPoolAddr = (MS_U32)NULL, u32TestAllocate = (MS_U32)NULL;
    printf("[%s] Start\n",__FUNCTION__);
    _s32TestMemPoolID = INVALID_MEMPOOL_ID;
    MS_S32 s32MstarNonCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32MstarNonCachedPoolID);
    // allocate buffer from system memory pool
    u32TestPoolAddr = (MS_U32)MsOS_AllocateMemory(sizeof(MS_U8)*TEST_POOL_LENGTH,s32MstarNonCachedPoolID);
    if (u32TestPoolAddr == (MS_U32)NULL)
    {
        printf("[%s][%d]\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    MemoryPool_Info sTestMemPool_Info[1] =
            {
                {-1, TEST_POOL_LENGTH, 0x0, MsOS_VA2PA(u32TestPoolAddr), E_MSOS_FIFO, "TestPool"},
            };

    _s32TestMemPoolID = MsOS_CreateMemoryPool( sTestMemPool_Info[0].uPoolSize,
                                               sTestMemPool_Info[0].u32MinAllocation,
                                               (void*)MS_PA2KSEG1(sTestMemPool_Info[0].u32Addr),
                                               sTestMemPool_Info[0].eAttribute,
                                               sTestMemPool_Info[0].szName);

    if (_s32TestMemPoolID == INVALID_MEMPOOL_ID)
    {
        printf("[%s][%d] Create Memory Pool Failed\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        printf("[%s][%d] Create Memory Pool OK\n", __FUNCTION__, __LINE__);
    }


    u32TestAllocate = (MS_U32)MsOS_AllocateMemory(sizeof(MS_U8)*0x100,_s32TestMemPoolID);
    if (u32TestAllocate == (MS_U32)NULL)
    {
        printf("[%s][%d] Allocated from test pool failed\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        printf("[%s][%d] Allocated from test pool OK\n", __FUNCTION__, __LINE__);
    }

    if (MsOS_FreeMemory((void*)u32TestAllocate,_s32TestMemPoolID)== FALSE)
    {
        printf("[%s][%d]\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }

    if (MsOS_DeleteMemoryPool(_s32TestMemPoolID) == FALSE)
    {
        printf("[%s][%d] Delete Memory Pool Failed\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }
    else
    {
        printf("[%s][%d] Delete Memory Pool OK\n", __FUNCTION__, __LINE__);
    }
    _s32TestMemPoolID = INVALID_MEMPOOL_ID;

    if (MsOS_FreeMemory((void*)u32TestPoolAddr,s32MstarNonCachedPoolID)== FALSE)
    {
        printf("[%s][%d]\n", __FUNCTION__, __LINE__);
        bRet = FALSE;
    }

    return bRet;
}

#if defined(MSOS_TYPE_LINUX) && defined(KERNEL_POOL2_ADR) && defined(KERNEL_POOL2_LEN)
static MS_BOOL _OS_Test_ConvKbytesStrToNum(MS_U8 *pu8KbytesStr, MS_U32 *pu32RetVal)
{
    MS_U8 *pu8Keyword = NULL;
    MS_U8 *pu8Travel = pu8KbytesStr;

    pu8Keyword = (MS_U8*)strstr((char*)pu8KbytesStr, "k");
    if (pu8Keyword)
    {
        *pu32RetVal = 0;
        while (pu8Travel < pu8Keyword)
        {
            *pu32RetVal = *pu32RetVal * 10 + (*pu8Travel - 0x30);
            pu8Travel++;
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL _OS_Test_LX2(void)
{
    FILE *pPipe = NULL;
    MS_U8 au8Buffer[128];
    MS_U8 *pu8Travel = NULL;
    MS_U32 u32FreeMem;
    MS_U32 u32LowMem;
    MS_U32 u32HighMem;
    MS_U32 u32ReservedMem;
    MS_U32 u32LX1AlignAddrStart, u32LX1AlignAddrEnd, u32LX1LenKbytes;
    MS_U32 u32LX2AlignAddrStart, u32LX2AlignAddrEnd, u32LX2LenKbytes;
    MS_U32 u32PageSize = sysconf(_SC_PAGE_SIZE);
    MS_U8 au8Nrm[] =  "\x1B[0m";
    MS_U8 au8Red[] =  "\x1B[31m";
    MS_U8 au8Gre[] =  "\x1B[32m";

    // If KERNEL_POOL2_LEN is zero, just return TRUE and do nothing
    if (KERNEL_POOL2_LEN <= 0)
    {
        printf("Not support!! LX2 memory size is 0x000000\n");
        return TRUE;
    }

    // Get memory information from log
    if ((pPipe = popen("dmesg|grep Memory", "r")) == NULL)
    {
        printf("Failed to get dmesg\n");
        return FALSE;
    }
    if (fgets((char*)au8Buffer, sizeof(au8Buffer), pPipe) == NULL)
    {
        printf("Failed to get memory information\n");
        pclose(pPipe);
        return FALSE;
    }
    pclose(pPipe);

    //Get free memory size
    pu8Travel = (MS_U8*)(strstr((char*)au8Buffer, "Memory: ") + strlen("Memory: "));
    if (_OS_Test_ConvKbytesStrToNum(pu8Travel, &u32FreeMem) == FALSE)
    {
        printf("Failed to paring free memory information\n");
        return FALSE;
    }

    //Get low memory size
    pu8Travel = (MS_U8*)(strstr((char*)au8Buffer, "/") + 1);
    if (_OS_Test_ConvKbytesStrToNum(pu8Travel, &u32LowMem) == FALSE)
    {
        printf("Failed to paring low memory information\n");
        return FALSE;
    }

    //Get reserved memory size
    pu8Travel = (MS_U8*)(strstr((char*)au8Buffer, "code, ") + strlen("code, "));
    if (_OS_Test_ConvKbytesStrToNum(pu8Travel, &u32ReservedMem) == FALSE)
    {
        printf("Failed to paring reserved memory information\n");
        return FALSE;
    }

    //Get high memory size
    pu8Travel = (MS_U8*)(strstr((char*)au8Buffer, "init, ") + strlen("init, "));
    if (_OS_Test_ConvKbytesStrToNum(pu8Travel, &u32HighMem) == FALSE)
    {
        printf("Failed to paring high memory information\n");
        return FALSE;
    }

    // Show informations
    printf("\n===============================\n");
    printf("  LX Info of User\n");
    printf("-------------------------------\n");
    printf(" LX1: 0x%.10X -> %#.10X\n", KERNEL_POOL_ADR, KERNEL_POOL_ADR + KERNEL_POOL_LEN);
    printf(" LX2: %#.10X -> %#.10X\n", KERNEL_POOL2_ADR, KERNEL_POOL2_ADR + KERNEL_POOL2_LEN);
    printf("===============================\n");

    printf("\n===============================\n");
    printf("  Meminfo of Kernel\n");
    printf("-------------------------------\n");
    printf(" Free Mem : %"DTC_MS_U32_u"k\n", u32FreeMem);
    printf(" Low  Mem : %"DTC_MS_U32_u"k\n", u32LowMem);
    printf(" High Mem : %"DTC_MS_U32_u"k\n", u32HighMem);
    printf(" Reserved : %"DTC_MS_U32_u"k\n", u32ReservedMem);
    printf("===============================\n\n");

    // Align address with page size
    if (KERNEL_POOL_ADR & (u32PageSize - 1))
        u32LX1AlignAddrStart = (KERNEL_POOL_ADR & ~(u32PageSize - 1)) + u32PageSize;
    else
        u32LX1AlignAddrStart = KERNEL_POOL_ADR;

    if (KERNEL_POOL2_ADR & (u32PageSize - 1))
        u32LX2AlignAddrStart = (KERNEL_POOL2_ADR & ~(u32PageSize - 1)) + u32PageSize;
    else
        u32LX2AlignAddrStart = KERNEL_POOL2_ADR;

    u32LX1AlignAddrEnd = (KERNEL_POOL_ADR + KERNEL_POOL_LEN) & ~(u32PageSize - 1);
    u32LX2AlignAddrEnd = (KERNEL_POOL2_ADR + KERNEL_POOL2_LEN) & ~(u32PageSize - 1);

    u32LX1LenKbytes = (u32LX1AlignAddrEnd - u32LX1AlignAddrStart) >> 10;
    u32LX2LenKbytes = (u32LX2AlignAddrEnd - u32LX2AlignAddrStart) >> 10;

    // Check if LX2 is configured correctly, conditions are:
    //1. Aligned LX1 + LX2 - Reserved Mem MUST be equal to Free Mem which captured from log
    //2. The sum of Low Mem and High Mem that captured from log MUST be equal to the sum of Aligned LX1 + LX2
    if ((u32LX1LenKbytes + u32LX2LenKbytes -u32ReservedMem) == u32FreeMem &&
            (u32LowMem + u32HighMem) == (u32LX1LenKbytes + u32LX2LenKbytes))
    {
        printf("\nTest Result => %sPASS\n%sLX2 is configured correctly, the size is %"DTC_MS_U32_u"k (Aligned to %"DTC_MS_U32_u" bytes)\n",
            (char*)au8Gre, (char*)au8Nrm, u32LX2LenKbytes, u32PageSize);
    }
    else
    {
        printf("\nTest Result => %sFAIL\n%sLX2 is NOT configured correctly, please check it\n", (char*)au8Red, (char*)au8Nrm);
    }

    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/// @brief The Watch dog demo function.\n
///        The demo flow:\n
///          (1) initial WDT.\n
///          (2) enable WDT interrupt and set WDT call back function.\n
///          (3) reset WDT.\n
///          (4) set WDT reset system time.\n
///          (5) set WDT interrupt time.\n
/// @param[in] None
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b WDT_Init\n
///
/// Sample Command: \b WDT_Init
///
//------------------------------------------------------------------------------
MS_BOOL Demo_OS_Test(MS_U32* u32TestCase, MS_U32 *Reserved0)
{
    MS_BOOL bRet = FALSE;
    void *gInput0 ;

    gInput0 = Reserved0;
    printf("Start Appdemo OS Test %"DTC_MS_U32_d"\n", *u32TestCase);
    //printf("Size 1  %d  Size 2 %d\n", sizeof(OSFun), sizeof(OSDemoFun));
    if ((*u32TestCase == 0) || (*u32TestCase >= (sizeof(OSDemoFun) / sizeof(OSFun))))
    {
        printf("=======================================================\n");
        printf("=======        Supported Test Items on Linux    =======\n");
        printf("=======================================================\n");
        printf("1.  OS Task Test\n");
        printf("2.  OS Mutex Test\n");
        printf("3.  OS Sem Test\n");
        printf("4.  OS Event Test\n");
        printf("5.  OS Queue Test\n");
        printf("6.  OS Interrupt Test\n");
        printf("7.  OS Timer Test\n");
        printf("8.  OS Address Translation Test\n");
        printf("9.  OS Pool API Test\n");
#if defined(MSOS_TYPE_LINUX) && defined(KERNEL_POOL2_ADR) && defined(KERNEL_POOL2_LEN)
        printf("10.  OS LX2 Mem Test\n");
#endif
        return TRUE;
    }

    bRet = OSDemoFun[*u32TestCase].func_ptr(gInput0);
    if (bRet == FALSE)
    {
        printf("Appdemo OS Test %"DTC_MS_U32_d" FAIL\n", *u32TestCase);
    }
    return bRet;
}

//------------------------------------------------------------------------------
/// @brief USB read/write speed test function.\n
/// @param[in] pMountPath The mount path of an USB storage
/// @param[in] u32BlockSizeInKB The buffer size of read/write test. Unit: KB
/// @param[in] u32Iteration A read/write test would repeat reading/writing u32BlockSize for (# of u32Iteration) times.
///                         So the total read/write size of a read/write test would be u32BlockSize * u32Iteration KB.
/// @param[in] bParallel 0: sequential - run the write test, and then run the read test.
///                      1: parallel - start write/read test tasks simultaneously.
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b usb_test\n
///
/// Sample Command: \b usb_test /root/sdx1 384 8 1
///                 \b usb_test /root/sdx1 384 8 0 or usb_test /root/sdx1 384 8
///
//------------------------------------------------------------------------------
MS_BOOL Demo_USB_Test(char *pMountPath, MS_U32* u32BlockSizeInKB, MS_U32* u32Iteration, MS_U32* bParallel)
{
    // create arguments setting object
    MS_S32 s32NonCachedPoolID = INVALID_POOL_ID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32NonCachedPoolID);

    USBTestArgs setting = {pMountPath, *u32BlockSizeInKB * 1024, *u32Iteration};
    pUSBTestArgs pSetting = &setting;

    // prepare read data in advance
    if(_USB_Test_Prepare_Data(pSetting) == FALSE)
    {
        printf("[%s][%d] prepare data failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    // sequential
    if(!*bParallel || *bParallel != 1)
    {
        _USB_Test_Write_Test(pSetting);
        _USB_Test_Read_Test(pSetting);
        printf("\e[1m\e[21m Test Completed! \e[0m\n");
    }
    // create task
    else
    {

        printf("\e[1m\e[21m Parallel Test Start: \e[0m\n");

        Task_Info writeTask = {-1, E_TASK_PRI_HIGH, NULL, (0x8000), "Write Task"};
        Task_Info readTask = {-1, E_TASK_PRI_HIGH, NULL, (0x8000), "Read Task"};

        MS_U32 u32Events = TEST_NO_EVENT;

        s32USBTestEventID = MsOS_CreateEventGroup("USB_Test_Event");
        if (s32USBTestEventID == INVALID_EVENT_ID)
        {
            printf("[%s][%d] create event group failed\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        writeTask.pStack = MsOS_AllocateMemory(writeTask.u32StackSize, s32NonCachedPoolID);
        if(!writeTask.pStack)
        {
            printf("[%s][%d] allocate write memory failed\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        readTask.pStack = MsOS_AllocateMemory(readTask.u32StackSize, s32NonCachedPoolID);
        if(!readTask.pStack)
        {
            printf("[%s][%d] allocate read memory failed\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        writeTask.iId = MsOS_CreateTask((TaskEntry)_USB_Test_Write_Test,
                                        (MS_U32)pSetting,
                                        writeTask.ePriority,
                                        TRUE,
                                        writeTask.pStack,
                                        writeTask.u32StackSize,
                                        writeTask.szName);

        readTask.iId = MsOS_CreateTask((TaskEntry)_USB_Test_Read_Test,
                                        (MS_U32)pSetting,
                                        readTask.ePriority,
                                        TRUE,
                                        readTask.pStack,
                                        readTask.u32StackSize,
                                        readTask.szName);

        char flag = 0;

        while(1)
        {
            MsOS_WaitEvent(s32USBTestEventID, USB_TEST_READ_WRITE_COMPLETE_EVENT, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
            //printf("[%s][%d] u32Events %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, u32Events);
            flag += u32Events;
            if (flag == USB_TEST_READ_WRITE_COMPLETE_EVENT)
            {
                printf("\e[1m\e[21m Test Completed! \e[0m\n");
                break;
            }
        }

        if (MsOS_DeleteEventGroup(s32USBTestEventID) == FALSE)
        {
            printf("[%s][%d] delete eventgroup failed\n", __FUNCTION__, __LINE__);
        }

        if (MsOS_DeleteTask(writeTask.iId) == FALSE || MsOS_DeleteTask(readTask.iId) == FALSE)
        {
            printf("[%s][%d] delete tasks failed\n", __FUNCTION__, __LINE__);
        }

    }

    return TRUE;
}

void _USB_Test_Clear_Cache()
{
    #if defined(MSOS_TYPE_LINUX)

    const char* magic = "3";

    sync();

    FILE* cache = MsFS_Fopen("/proc/sys/vm/drop_caches", "wb");
    if(!cache)
    {
        printf("[%s][%d] clear cache failed\n", __FUNCTION__, __LINE__);
        return;
    }

    MsFS_Fwrite(magic, 1, sizeof(char), cache);
    MsFS_Fclose(cache);

    #endif
}

MS_U32 _USB_Test_Approximate(MS_U32 u32Numerator, MS_U32 u32Denominator)
{
    MS_U32 u32Reminder = u32Numerator % u32Denominator;
    MS_U32 u32Interval = u32Denominator / 8;
    MS_S32 i;
    MS_U32 u32Result = 0, u32Level = u32Interval;
    for(i=1;i<=7;i++)
    {
        if(u32Reminder < u32Level)
        {
            break;
        }
        u32Level += u32Interval;
        u32Result += 125;
    }
    return u32Result;
}

static MS_BOOL _USB_Test_Prepare_Data(pUSBTestArgs setting)
{
    printf("\e[1m\e[21m Prepare Data ... \e[0m\n");

    MS_S32 s32NonCachedPoolID = 0;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32NonCachedPoolID);

    char* filename = "/read.bin";
    char* fullpath;

    fullpath = MsOS_AllocateMemory(strlen(setting->pMountPath) + strlen(filename) + 1, s32NonCachedPoolID);
    if(!fullpath)
    {
        printf("[%s][%d] allocate memory failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    memset(fullpath, 0, strlen(setting->pMountPath) + strlen(filename) + 1);

    strncat(fullpath, setting->pMountPath, strlen(setting->pMountPath));
    strncat(fullpath, filename, strlen(filename));

    if(strlen(fullpath) == 0)
    {
        printf("[%s][%d] string concat failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    FILE* writeFile = MsFS_Fopen(fullpath, "wb");
    if(!writeFile)
    {
        printf("[%s][%d] open file failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    MS_U8* array;

    array = MsOS_AllocateMemory(setting->u32BlockSize, s32NonCachedPoolID);
    if(!array)
    {
        printf("[%s][%d] allocate data memory failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    MS_U8 hex = (MS_U8)(MsOS_GetSystemTime() % 256);
    MS_S32 i;
    for(i=0;i<setting->u32BlockSize;i++)
    {
        array[i] = hex++;
    }

    for(i=0;i<setting->u32Iteration;i++)
    {
        MsFS_Fwrite(array, 1, setting->u32BlockSize, writeFile);
    }

    MsFS_Fclose(writeFile);

    _USB_Test_Clear_Cache();

    if (MsOS_FreeMemory(array, s32NonCachedPoolID) == FALSE || MsOS_FreeMemory(fullpath, s32NonCachedPoolID) == FALSE)
    {
        printf("[%s][%d] free memory failed\n", __FUNCTION__, __LINE__);
    }

    return TRUE;
}

static void _USB_Test_Write_Test(pUSBTestArgs setting)
{
    printf("\e[1m\e[21m Write Test Start! \e[0m\n");

    MS_S32 s32NonCachedPoolID = 0;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32NonCachedPoolID);

    MS_U32 start, end;

    char* filename = "/write.bin";
    char* fullpath;

    fullpath = MsOS_AllocateMemory(strlen(setting->pMountPath) + strlen(filename) + 1, s32NonCachedPoolID);
    if(!fullpath)
    {
        printf("[%s][%d] allocate memory failed\n", __FUNCTION__, __LINE__);
        MsOS_SetEvent(s32USBTestEventID, USB_TEST_WRITE_COMPLETE_EVENT);
        return;
    }
    memset(fullpath, 0, strlen(setting->pMountPath) + strlen(filename) + 1);

    strncpy(fullpath, setting->pMountPath, strlen(setting->pMountPath));
    strncat(fullpath, filename, strlen(filename));

    if(strlen(fullpath) == 0)
    {
        printf("[%s][%d] string concat failed\n", __FUNCTION__, __LINE__);
        MsOS_SetEvent(s32USBTestEventID, USB_TEST_WRITE_COMPLETE_EVENT);
        return;
    }

    FILE* writeFile = MsFS_Fopen(fullpath, "wb");
    if(!writeFile)
    {
        printf("[%s][%d] open file failed\n", __FUNCTION__, __LINE__);
        MsOS_SetEvent(s32USBTestEventID, USB_TEST_WRITE_COMPLETE_EVENT);
        return;
    }

    MS_U8* array;

    array = MsOS_AllocateMemory(setting->u32BlockSize, s32NonCachedPoolID);
    if(!array)
    {
        printf("[%s][%d] allocate write data memory failed\n", __FUNCTION__, __LINE__);
        MsOS_SetEvent(s32USBTestEventID, USB_TEST_WRITE_COMPLETE_EVENT);
        return;
    }

    // coverity[dont_call]
    MS_U8 hex = (MS_U8) rand();
    MS_S32 i;
    for(i=0;i<setting->u32BlockSize;i++)
    {
        array[i] = hex++;
    }

    MS_U32 cumulativeCounter = 0;

    for(i=0;i<setting->u32Iteration;i++)
    {
        start = MsOS_GetSystemTime();
        MsFS_Fwrite(array, 1, setting->u32BlockSize, writeFile);
        end = MsOS_GetSystemTime();
        cumulativeCounter += end - start;
    }

    MsFS_Fclose(writeFile);

    printf(" write size: %"DTC_MS_U32_d" KB\n", setting->u32BlockSize * setting->u32Iteration / 1024);
    printf(" write processing time: %"DTC_MS_U32_d" ms\n", cumulativeCounter);
    MS_U32 denominator = 1024 * cumulativeCounter / 1000 * 1024;
    if (denominator == 0)
    {
        printf("Error, denominator is zero, can't process _USB_Test_Approximate correctly\n");
        return;
    }
    printf("\e[96m Write Speed: %"DTC_MS_U32_d".%"DTC_MS_U32_d" MB/s\e[0m\n", setting->u32BlockSize * setting->u32Iteration / denominator, _USB_Test_Approximate(setting->u32BlockSize * setting->u32Iteration, denominator));

    _USB_Test_Clear_Cache();

    if (MsOS_FreeMemory(array,s32NonCachedPoolID)== FALSE || MsOS_FreeMemory(fullpath,s32NonCachedPoolID)== FALSE)
    {
        printf("[%s][%d] free memory failed\n", __FUNCTION__, __LINE__);
    }

    MsOS_SetEvent(s32USBTestEventID, USB_TEST_WRITE_COMPLETE_EVENT);
}

static void _USB_Test_Read_Test(pUSBTestArgs setting)
{
    printf("\e[1m\e[21m Read Test Start! \e[0m\n");

    MS_S32 s32NonCachedPoolID = 0;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32NonCachedPoolID);

    MS_U32 start, end;

    char* filename = "/read.bin";
    char* fullpath;

    fullpath = MsOS_AllocateMemory(strlen(setting->pMountPath) + strlen(filename) + 1, s32NonCachedPoolID);
    if(!fullpath)
    {
        printf("[%s][%d] allocate memory failed\n", __FUNCTION__, __LINE__);
        MsOS_SetEvent(s32USBTestEventID, USB_TEST_READ_COMPLETE_EVENT);
        return;
    }
    memset(fullpath, 0, strlen(setting->pMountPath) + strlen(filename) + 1);

    strncpy(fullpath, setting->pMountPath, strlen(setting->pMountPath));
    strncat(fullpath, filename, strlen(filename));

    if(strlen(fullpath) == 0)
    {
        printf("[%s][%d] string concat failed\n", __FUNCTION__, __LINE__);
        MsOS_SetEvent(s32USBTestEventID, USB_TEST_READ_COMPLETE_EVENT);
        return;
    }

    FILE* readFile = MsFS_Fopen(fullpath, "rb");
    if(!readFile)
    {
        printf("[%s][%d] open file failed\n", __FUNCTION__, __LINE__);
        MsOS_SetEvent(s32USBTestEventID, USB_TEST_READ_COMPLETE_EVENT);
        return;
    }

    MS_U8* array;

    array = MsOS_AllocateMemory(setting->u32BlockSize, s32NonCachedPoolID);
    if(!array)
    {
        printf("[%s][%d] allocate read data memory failed\n", __FUNCTION__, __LINE__);
        MsOS_SetEvent(s32USBTestEventID, USB_TEST_READ_COMPLETE_EVENT);
        return;
    }

    MS_S32 i;
    MS_U32 cumulativeCounter = 0;

    for(i=0;i<setting->u32Iteration;i++)
    {
        start = MsOS_GetSystemTime();
        MsFS_Fread(array, setting->u32BlockSize, 1, readFile);
        end = MsOS_GetSystemTime();
        cumulativeCounter += end - start;
    }

    MsFS_Fclose(readFile);

    printf(" read size: %"DTC_MS_U32_d" KB\n", setting->u32BlockSize * setting->u32Iteration / 1024);
    printf(" read processing time: %"DTC_MS_U32_d" ms\n", cumulativeCounter);
    MS_U32 denominator = 1024 * cumulativeCounter / 1000 * 1024;
    if (denominator == 0)
    {
        printf("Error, denominator is zero, can't process _USB_Test_Approximate correctly\n");
        return;
    }
    printf("\e[96m Read Speed: %"DTC_MS_U32_d".%"DTC_MS_U32_d" MB/s\e[0m\n", setting->u32BlockSize * setting->u32Iteration / denominator, _USB_Test_Approximate(setting->u32BlockSize * setting->u32Iteration, denominator));

    _USB_Test_Clear_Cache();

    if (MsOS_FreeMemory(array, s32NonCachedPoolID) == FALSE || MsOS_FreeMemory(fullpath, s32NonCachedPoolID) == FALSE)
    {
        printf("[%s][%d] free memory failed\n", __FUNCTION__, __LINE__);
    }

    MsOS_SetEvent(s32USBTestEventID, USB_TEST_READ_COMPLETE_EVENT);
}


#endif
