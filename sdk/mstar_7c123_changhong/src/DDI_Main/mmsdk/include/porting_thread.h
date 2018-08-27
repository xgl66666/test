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

#ifndef __porting_thread__
#define __porting_thread__
#include "mmsdk_interface_def.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

/// @file
/// @defgroup porting_thread porting_thread functions
/// @{

/// define mmsdk stack size
#define MMSDK_PTHREAD_STACK_SIZE 0x10000 //64K


/// define mutex multiple-instance item
typedef void* PT_MUTEXITEM;

/// define condition variable multiple-instance item
/// @since version 1.01
typedef void* PT_CONDVARIABLEITEM;


/// define the task priority
typedef enum
{
    ///< System priority task(interrupt level driver, e.g. TSP, SMART)
    E_MMSDK_TASK_PRI_SYS      = 0,
    ///< Highest priority task(background monitor driver, e.g. DVBC, HDMI)
    E_MMSDK_TASK_PRI_HIGHEST  = 4,
    ///< High priority task(service task)
    E_MMSDK_TASK_PRI_HIGH     = 8,
    ///< Medium priority task(application task)
    E_MMSDK_TASK_PRI_MEDIUM   = 12,
    ///< Low priority task(nonbusy application task)
    E_MMSDK_TASK_PRI_LOW      = 16,
    ///< Lowest priority task(idle application task)
    E_MMSDK_TASK_PRI_LOWEST   = 24,
    ///< super priority task and set Round-robin schedule
    E_MMSDK_TASK_PRI_SUPER_SCHEDULE_RR = 100,
    ///< The start offset for set Round-robin schedule
    E_MMSDK_TASK_PRI_OFFSET_SCHEDULE_RR = 0x100,
    ///< Low priority task and set Round-robin schedule
    E_MMSDK_TASK_PRI_LOW_SCHEDULE_RR = E_MMSDK_TASK_PRI_OFFSET_SCHEDULE_RR + 1,
    ///< Medium priority task and set Round-robin schedule
    E_MMSDK_TASK_PRI_MEDIUM_SCHEDULE_RR = E_MMSDK_TASK_PRI_OFFSET_SCHEDULE_RR + 50,
    ///< super priority task and set Round-robin schedule(same setting with E_MMSDK_TASK_PRI_SUPER_SCHEDULE_RR)
    E_MMSDK_TASK_PRI_HIGHEST_SCHEDULE_RR = E_MMSDK_TASK_PRI_OFFSET_SCHEDULE_RR + 99,
    ///< The start offset for set FIFO schedule
    E_MMSDK_TASK_PRI_OFFSET_SCHEDULE_FIFO = 0x200,
} EN_MMSDK_TASK_PRIORITY;

//------------------------------------------------------------------------------
/// Create thread
/// @param pu32ThreadId           \b OUT: thread id
/// @param start_routine          \b IN: The function to be run as the new threads start routine
/// @param pTaskArg               \b IN: An address for the argument for the threads start routine
/// @param eTaskPriority          \b IN: task priority
/// @param pStackEntry            \b IN: stack entry
/// @param u32StackSize           \b IN: size of stack
/// @param pTaskName              \b IN: name of task
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Thread_ThreadCreate(MMSDK_U32 *pu32ThreadId,
                                                             void *(*start_routine) (void *),
                                                             void* pTaskArg,
                                                             const EN_MMSDK_TASK_PRIORITY eTaskPriority,
                                                             void * pStackEntry,
                                                             const MMSDK_U32 u32StackSize,
                                                             const char *pTaskName);

//------------------------------------------------------------------------------
/// Naming thread
/// @param strName                \b IN: thread name
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Thread_ThreadSetName(const char * strName);

//------------------------------------------------------------------------------
/// Join
/// @param u32ThreadId            \b IN: thread id
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Thread_ThreadJoin(const MMSDK_U32 u32ThreadId);

//------------------------------------------------------------------------------
/// Make the u32ThreadId as detached, it will release the resource automatically
/// if the u32ThreadId terminates.
/// @param u32ThreadId            \b IN: thread id
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Thread_ThreadDetach(const MMSDK_U32 u32ThreadId) __attribute__ ((weak));

//------------------------------------------------------------------------------
/// Exit thread
/// @param retval                \b IN/OUT: save return value after thread exited
///
/// @return N/A
//------------------------------------------------------------------------------
void PT_Thread_ThreadExit(void* retval);

//------------------------------------------------------------------------------
/// Mutex init
/// @param pMutex                \b IN/OUT: mutex multiple-instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Thread_MutexInit(PT_MUTEXITEM*pMutex);

//------------------------------------------------------------------------------
/// Mutex destroy
/// @param pMutex                \b IN/OUT: mutex multiple-instance item
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Thread_MutexDestroy(PT_MUTEXITEM*pMutex);

//------------------------------------------------------------------------------
/// Lock mutex
/// @param pMutex                \b IN: mutex multiple-instance item
///
/// @return N/A
//------------------------------------------------------------------------------
void PT_Thread_MutexLock(PT_MUTEXITEM pMutex);

//------------------------------------------------------------------------------
/// Unlock mutex
/// @param pMutex                \b IN: mutex multiple-instance item
///
/// @return N/A
//------------------------------------------------------------------------------
void PT_Thread_MutexUnlock(PT_MUTEXITEM pMutex);

/***************** condition variabvle added at version 1.01 *****************/

//------------------------------------------------------------------------------
/// Condition variables init
/// @since version 1.01
/// @param pCond                 \b OUT: the condition variable item to be initialized
/// @param pAttr                 \b IN: the condition attributes object for initialization
/// @param pMutex                \b IN: mutex multiple-instance item(ecos requires)
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Thread_Cond_Init(PT_CONDVARIABLEITEM* pCond, const void* pAttr, PT_MUTEXITEM *pMutex) __attribute__ ((weak));

//------------------------------------------------------------------------------
/// Condition variables destory
/// @since version 1.01
/// @param pCond                 \b IN: the condition variable to be destroyed
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Thread_Cond_Destroy(PT_CONDVARIABLEITEM* pCond) __attribute__ ((weak));

//------------------------------------------------------------------------------
/// Block with the condition variable until signal or broadcast
/// @since version 1.01
/// @param pCond                 \b IN: the condition variable to be waited
/// @param pMutex                \b IN: the mutex to be associated with the contiation variable
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Thread_Cond_Wait(PT_CONDVARIABLEITEM pCond, PT_MUTEXITEM pMutex) __attribute__ ((weak));

//------------------------------------------------------------------------------
/// Block with the condition variable until signal or broadcast or timeout
/// @since version 1.01
/// @param pCond                 \b IN: the condition variable to be waited
/// @param pMutex                \b IN: the mutex to be associated with the contiation variable
/// @param u32RelatvieWaitTimeMs \b IN: the relateive wait time(in Ms), convert to abs time in this function
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Thread_Cond_TimeWait(PT_CONDVARIABLEITEM pCond, PT_MUTEXITEM pMutex, const MMSDK_U32 u32RelatvieWaitTimeMs) __attribute__ ((weak));

//------------------------------------------------------------------------------
/// Wakes up least one thread which is waiting the condition variable
/// @since version 1.01
/// @param pCond                 \b IN: the condition variable to be signaled
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Thread_Cond_Signal(PT_CONDVARIABLEITEM pCond) __attribute__ ((weak));

//------------------------------------------------------------------------------
/// Wakes up all threads which are waiting the condition variable
/// @since version 1.01
/// @param pCond                 \b IN: the condition variable to be broadcasted
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_Thread_Cond_Broadcast(PT_CONDVARIABLEITEM pCond) __attribute__ ((weak));

/*************************** condition variable end ***************************/

/// @} // end of porting_thread

#ifdef __cplusplus
}
#endif //__cplusplus


#endif //__porting_thread__
///*** please do not remove change list tag ***///
///***$Change: 1429000 $***///
