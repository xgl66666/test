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

#ifndef __mmsdk_porting_msos__
#define __mmsdk_porting_msos__
#include "mmsdk_interface_def.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

/// @file
/// @defgroup porting_msos porting_msos functions
/// @{

/// define semaphore multiple-instance item
/// @since version 1.03
typedef void* PT_SEMAPHOREITEM;

//-------------------------------------------------------------------------------------------------
/// Allocate memory pool
/// @param s32Reserved           \b IN: Reserved
/// @param bReserved           \b IN: Reserved
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_MsOS_MemoryInit(MMSDK_S32 s32Reserved, MMSDK_BOOL bReserved);

//-------------------------------------------------------------------------------------------------
/// Release memory pool
/// @return TRUE: Success.
/// @return FALSE: Failure.
//-------------------------------------------------------------------------------------------------
MMSDK_BOOL PT_MsOS_MemoryDeinit(void);

//-------------------------------------------------------------------------------------------------
/// Allocate memory block, Allocates a block of size bytes of memory, returning a pointer to the beginning of the block
/// @param u32Size           \b IN: Size of the memory block, in bytes. u32Size is an unsigned integral type.
/// @return Success a pointer to the memory block allocated by the function.. If the function failed to allocate the requested block of memory, a null pointer is returned.
//-------------------------------------------------------------------------------------------------
void * PT_MsOS_AllocateMemory (MMSDK_U32 u32Size);

//-------------------------------------------------------------------------------------------------
/// Reallocate memory block, Changes the u32NewSize of the memory block pointed to by pOrgAddress.
/// @param pOrgAddress           \b IN: Pointer to a memory block previously allocated with malloc, calloc or realloc. Alternatively, this can be a null pointer, in which case a new block is allocated (as if malloc was called)
/// @param u32NewSize            \b IN: Size of the memory block, in bytes. u32NewSize is an unsigned integral type.
/// @return  A pointer to the reallocated memory block, which may be either the same as ptr or a new location.
//-------------------------------------------------------------------------------------------------
void * PT_MsOS_ReallocateMemory (void *pOrgAddress, MMSDK_U32 u32NewSize);

//-------------------------------------------------------------------------------------------------
/// Deallocate memory block
/// @param pAddress           \b IN: Pointer to a memory block previously allocated with malloc, calloc or realloc.
//-------------------------------------------------------------------------------------------------
void PT_MsOS_FreeMemory (void *pAddress);

//-------------------------------------------------------------------------------------------------
/// This function will convert physical address to bus address
/// @param addr                 \b IN: physical address
/// @return                     \b OUT: bus address
//-------------------------------------------------------------------------------------------------
MMSDK_U32 PT_MsOS_PA2BA(MMSDK_U32 addr);

//-------------------------------------------------------------------------------------------------
/// This function will convert bus address to physical address
/// @param addr           \b IN: bus address
/// @return                     \b OUT: physical address
//-------------------------------------------------------------------------------------------------
MMSDK_U32 PT_MsOS_BA2PA(MMSDK_U32 addr);

//-------------------------------------------------------------------------------------------------
/// This function will return the current system time with MsOS_GetSystemTime
/// @since version 1.04
/// @return                     \b OUT: system time in ms
//-------------------------------------------------------------------------------------------------
MMSDK_U64 PT_MsOS_GetSystemTime(void) __attribute__ ((weak));

//-------------------------------------------------------------------------------------------------
/// This function delay the current task for u32Ms milliseconds
/// @param u32Ms           \b IN: delay time in ms
/// @since version 1.04
//-------------------------------------------------------------------------------------------------
void PT_MsOS_DelayTask(MMSDK_U32 u32Ms) __attribute__ ((weak));

/********************** Semaphore added at version 1.03 **********************/

//------------------------------------------------------------------------------
/// Semaphore init
/// @since version 1.03
/// @param pSem                 \b OUT: the semaphore item to be initialized
/// @param s32SemVal            \b IN: Specify the initial semaphore value
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MsOS_Sem_Init(PT_SEMAPHOREITEM* pSem, MMSDK_S32 s32SemVal) __attribute__ ((weak));

//------------------------------------------------------------------------------
/// Semaphore destory
/// @since version 1.03
/// @param pSem                 \b IN: the semaphore item to be destroyed
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MsOS_Sem_Destroy(PT_SEMAPHOREITEM pSem) __attribute__ ((weak));

//------------------------------------------------------------------------------
/// Unlock the semaphore
/// @since version 1.03
/// @param pSem                 \b IN: the semaphore item to be unlocked
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MsOS_Sem_Post(PT_SEMAPHOREITEM pSem) __attribute__ ((weak));

//------------------------------------------------------------------------------
/// Block with the semaphore until post
/// @since version 1.03
/// @param pSem                 \b IN: the semaphore item to be waited
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MsOS_Sem_Wait(PT_SEMAPHOREITEM pSem) __attribute__ ((weak));

//------------------------------------------------------------------------------
/// The same as PT_MsOS_Sem_Wait, but immediately return.
/// @since version 1.03
/// @param pSem                 \b IN: the semaphore item to be tried
/// @param s32Errno             \b OUT: the errno if return fail
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MsOS_Sem_TryWait(PT_SEMAPHOREITEM pSem, MMSDK_S32 *s32Errno) __attribute__ ((weak));

//------------------------------------------------------------------------------
/// Block with the semaphore until post or time out of the relatvie waited time
/// @since version 1.03
/// @param pSem                  \b IN: the semaphore item to be waited
/// @param u32RelatvieWaitTimeMs \b IN: the relateive wait time(in Ms), convert to abs time in this function
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MsOS_Sem_TimedWait(PT_SEMAPHOREITEM pSem, const MMSDK_U32 u32RelatvieWaitTimeMs) __attribute__ ((weak));

//------------------------------------------------------------------------------
/// Get the value of a semaphore
/// @since version 1.03
/// @param pSem                 \b IN: The specific semaphore to get value
/// @param ps32SemVal           \b OUT: Get the value of a semaphore
///
/// @return TRUE: Success.
/// @return FALSE: Failure.
//------------------------------------------------------------------------------
MMSDK_BOOL PT_MsOS_Sem_GetValue(PT_SEMAPHOREITEM pSem, MMSDK_S32 *ps32SemVal) __attribute__ ((weak));

/******************************* Semephore end *******************************/


/// @} // end of porting_msos

#ifdef __cplusplus
}
#endif //__cplusplus
#endif //__mmsdk_porting_msos__
///*** please do not remove change list tag ***///
///***$Change: 1429000 $***///
