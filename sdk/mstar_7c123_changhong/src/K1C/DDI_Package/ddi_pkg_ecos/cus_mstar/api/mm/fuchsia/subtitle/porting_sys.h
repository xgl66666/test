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

#ifndef __mw_porting_sys__
#define __mw_porting_sys__

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

typedef enum
{
    E_MEM_TYPE_CACHED = 0,
    E_MEM_TYPE_NONCACHED,

    E_MEM_TYPE_ALL
}eMPL_SYS_MEM_TYPE;

//-------------------------------------------------------------------------------------------------
/// Create a task
/// @param  pTaskEntry                  \b IN: Task entry point
/// @param  u32TaskEntryData      \b IN: Task entry data
/// @param  eTaskPriority               \b IN: Task priority
/// @param  bAutoStart                  \b IN: Start immediately or later
/// @param  pStackBase                 \b IN: Task stack
/// @param  u32StackSize              \b IN: Stack size
/// @param  pTaskName                 \b IN: Task name
/// @return >=0 : assigned Task ID
/// @return < 0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MPL_SYS_CreateTask(TaskEntry pTaskEntry,
                        MS_U32 u32TaskEntryData,
                        TaskPriority eTaskPriority,
                        MS_BOOL bAutoStart,
                        void *pStackEntry,
                        MS_U32 u32StackSize,
                        char *pTaskNam);

//-------------------------------------------------------------------------------------------------
/// Delete Task
/// @param s32TaskId                 \b IN: Task ID
/// @return TRUE: Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_SYS_DeleteTask(MS_S32 s32TaskId);

//-------------------------------------------------------------------------------------------------
/// Suspend the calling task for u32Ms milliseconds
/// @param  u32Ms  \b IN: delay 1 ~ MSOS_WAIT_FOREVER ms
/// @return None
//-------------------------------------------------------------------------------------------------
void MPL_SYS_DelayTask(MS_U32 u32Ms);

//-------------------------------------------------------------------------------------------------
/// Create Event
/// @param pName                 \b IN: Event Name
/// @return >=0 : assigned Event Id
/// @return <0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MPL_SYS_CreateEvent(char *pName);

//-------------------------------------------------------------------------------------------------
/// Delete Event
/// @param s32EventGroupId                 \b IN: Event Group ID
/// @return TRUE: Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_SYS_DeleteEvent(MS_S32 s32EventGroupId);

//-------------------------------------------------------------------------------------------------
/// Set the Event Flag
/// @param s32EventGroupId          \b IN: Event Group ID
/// @param u32EventFlag                \b IN: Event flag value
/// @return TRUE: Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_SYS_SetEvent(MS_S32 s32EventGroupId, MS_U32 u32EventFlag);

//-------------------------------------------------------------------------------------------------
/// Clear Event
/// @param s32EventGroupId          \b IN: Event Group ID
/// @param u32EventFlag                \b IN: Event flag value
/// @return TRUE: Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_SYS_ClearEvent(MS_S32 s32EventGroupId, MS_U32 u32EventFlag);

//-------------------------------------------------------------------------------------------------
/// Wait Event
/// @param s32EventGroupId              \b IN: Event Group ID
/// @param u32WaitEventFlag             \b IN: Wait event flag value
/// @param pu32RetrievedEventFlag   \b OUT: Retrieved event flag value
/// @param eWaitMode                        \b IN: E_AND/E_OR/E_AND_CLEAR/E_OR_CLEAR
/// @param  u32WaitMs                       \b IN: 0 ~ MSOS_WAIT_FOREVER
/// @return TRUE: Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_SYS_WaitEvent(MS_S32  s32EventGroupId,
                        MS_U32  u32WaitEventFlag,
                        MS_U32  *pu32RetrievedEventFlag,
                        EventWaitMode eWaitMode,
                        MS_U32  u32WaitMs);

//-------------------------------------------------------------------------------------------------
/// Create Mutex
/// @param eAttribute               \b IN: Suspension order
/// @param pMutexName          \b IN: Mutex name
/// @param u32Flag                  \b IN: Process attribute
/// @return >=0 : assigned mutex Id
/// @return <0 : fail
//-------------------------------------------------------------------------------------------------
MS_S32 MPL_SYS_CreateMutex(MsOSAttribute eAttribute, char *pMutexName, MS_U32 u32Flag);

//-------------------------------------------------------------------------------------------------
/// Delete Mutex
/// @param s32MutexId                 \b IN: Mutex ID
/// @return TRUE: Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_SYS_DeleteMutex(MS_S32 s32MutexId);

//-------------------------------------------------------------------------------------------------
/// Lock Mutex
/// @param s32MutexId                \b IN: Mutex ID
/// @param u32WaitMs                 \b IN: 0 ~ MSOS_WAIT_FOREVER
/// @return TRUE: Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_SYS_LockMutex(MS_S32 s32MutexId, MS_U32 u32WaitMs);

//-------------------------------------------------------------------------------------------------
/// Unlock Mutex
/// @param s32MutexId                 \b IN: Mutex ID
/// @return TRUE: Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_SYS_UnlockMutex(MS_S32 s32MutexId);

//-------------------------------------------------------------------------------------------------
/// Get System Time in ms
/// @return                 \b OUT: System time in ms
//-------------------------------------------------------------------------------------------------
MS_U32 MPL_SYS_GetSystemTime(void);

//-------------------------------------------------------------------------------------------------
/// Get trail one
/// @param  u32Flags        \b IN: 32-bit flag
/// @return Trail one position
//-------------------------------------------------------------------------------------------------
MS_U8 MPL_SYS_CPU_GetTrailOne(MS_U32 u32Flags);

//-------------------------------------------------------------------------------------------------
/// Set memory pool ID
/// @param  s32CachePoolID              \b IN: Cache pool ID
/// @param  s32NonCachePoolID        \b IN: Noncache pool ID
/// @return TRUE: Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_SYS_SetMemPoolID(MS_S32 s32CachePoolID, MS_S32 s32NonCachePoolID);

//-------------------------------------------------------------------------------------------------
/// Allocate memory
/// @param  u32Size        \b IN: Requested size
/// @param  s32PoolId      \b IN: Pool ID
/// @return NULL: Failure
/// @return non-NULL: Success
//-------------------------------------------------------------------------------------------------
void *MPL_SYS_AllocateMemory (MS_U32 u32Size, eMPL_SYS_MEM_TYPE eMemType);

//-------------------------------------------------------------------------------------------------
/// Allocate memory
/// @param  pAddress       \b IN: Address to relase
/// @param  s32PoolId      \b IN: Pool ID
/// @return TRUE: Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_SYS_FreeMemory (void *pAddress, eMPL_SYS_MEM_TYPE eMemType);

//-------------------------------------------------------------------------------------------------
/// Virtual address to Physical address
/// @param  addr        \b IN: Virtural address
/// @return Physcial address
//-------------------------------------------------------------------------------------------------
MS_PHY MPL_SYS_VA2PA(MS_VIRT addr);

//-------------------------------------------------------------------------------------------------
/// Physical address to Virtual address
/// @param  addr       \b IN:  Physical address
/// @return Virtual address
//-------------------------------------------------------------------------------------------------
MS_VIRT MPL_SYS_PA2VA(MS_PHY phyAddr, eMPL_SYS_MEM_TYPE eMemType);

//-------------------------------------------------------------------------------------------------
/// Get task state
/// @param  pTaskInfo       \b IN: Task Info
/// @param  pTaskState      \b IN: Task state
/// @return TRUE: Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
#include "MsOS.h"
MS_BOOL MPL_SYS_GetTaskStatus (Task_Info* pTaskInfo, TaskStatus* pTaskState);

//-------------------------------------------------------------------------------------------------
/// Write back if dirty & Invalidate the cache lines in the given range
/// @param  u32Start \b IN: start address (must be 16-B aligned and in cacheable area)
/// @param  u32Size  \b IN: size (must be 16-B aligned)
/// @return TRUE : Succeed
/// @return FALSE : Fail due to invalide parameter
//-------------------------------------------------------------------------------------------------
MS_BOOL MPL_SYS_FlushMemory(MS_U32 u32Start , MS_U32 u32Size);

#ifdef __cplusplus
}
#endif //__cplusplus
#endif //__mw_porting_sys__
