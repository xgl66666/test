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
// Copyright (c) 2006-2010 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   appDemo_Gpio.c
/// @author MStar Semiconductor Inc.
/// @brief  Gpio control Function Sample Demo
///
/// @name Gpio control Function Sample Command Usage
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "drvMMIO.h"
#include "msAPI_Gpio.h"
#include "appDemo_Gpio.h"
#include "Board.h"

//-------------------------------------------------------------------------------------------------
//                                MACROS
//-------------------------------------------------------------------------------------------------
#define REG16_PM(addr )         *((volatile MS_U16*)(_u32PmBase+ (addr*2)))
#define REG_FIQ_MASK REG16_PM(0x0E00)
#define REG_FIQ_CLR REG16_PM(0x0E08)
#define REG_FIQ_POL REG16_PM(0x0E0C)
#define REG_FIQ_FINAL_STATUS REG16_PM(0x0E14)

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern MS_S32 gs32CachedPoolID;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_S32 _s32GpioEvent;
static MS_BOOL _bInterruptInit;
static Task_Info _appDemo_Gpio_Task_Info = {-1, E_TASK_PRI_HIGH, NULL, (0x4000), "GPIO_Task"};
static MS_U32 _u32PmBase = 0;


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _appDemo_Gpio_ISR(InterruptNum eIntr)
{
    if(eIntr == E_INT_IRQ_PM)
    {
        MsOS_SetEvent(_s32GpioEvent, REG_FIQ_FINAL_STATUS);
        REG_FIQ_MASK |= REG_FIQ_FINAL_STATUS; // mask interrupt first
        MsOS_EnableInterrupt(E_INT_IRQ_PM);
    }
}

static void _appDemo_Gpio_Task(MS_U32 argc, VOID *argv)
{
    MS_U32 u32Event;
    printf("GPIO TASK START\n");
    while(1)
    {
        if(MsOS_WaitEvent(
            _s32GpioEvent,
            0xFFFFFFFF,
            &u32Event,
            E_OR_CLEAR,
            0xFFFFFFFF)
        )
        {
            printf("GPIO INT %08lu\n", u32Event);

            /* Do what you want to do here */

            REG_FIQ_CLR = u32Event & 0xFFFF; // Write clear
            REG_FIQ_MASK &= ~(u32Event & 0xFFFF); // unmask interrupt

        }
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------
/// @brief Initialze Gpio control
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b Gpio_Init \n
///
/// Sample Command: \n \b Gpio_Init \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_Gpio_Init(void)
{
    MS_U32 u32Size;
    msAPI_Gpio_Init();
    MDrv_MMIO_GetBASE(&_u32PmBase, &u32Size, MS_MODULE_PM);
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Set the Gpio type to input/output.
///        If the type is output, the output value and invert are set.
/// @param[in] gpioNum The gpio number.
/// @param[in] inout The Gpio type is input/output.
///         0: output
///         1: input
/// @param[in] invert The output value is invert.
///         0: not invert
///         1: invert
/// @param[in] outputonoff The output on/off.
///         0: output off
///         1: output on
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b Gpio_Set \n
///
/// Sample Command: \n \b Gpio_Set 20 1 0 0 \n
///                 \b Gpio_Set 20 0 0 1 \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_Gpio_Set(MS_U32 *gpioNum, MS_U32 *inout, MS_U32 *invert, MS_U32 *outputonoff)
{
    GPIOInfo_t gpioInfo;
    gpioInfo.gID = 0;
    gpioInfo.gInvertor = *invert;
    gpioInfo.gPadNo = *gpioNum;

    msAPI_Gpio_SetgpioPtr(&gpioInfo);

    if(*inout == 1) //input
    {
        msAPI_Gpio_SetInput();
    }
    else // output
    {
        if(*outputonoff == 1)
        {
            msAPI_Gpio_SetOn();
        }
        else
        {
            msAPI_Gpio_SetOff();
        }
    }

    return TRUE;
}


//------------------------------------------------------------------------------
/// @brief Get the Gpio input level.
/// @param[in] gpioNum The gpio number.
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b Gpio_getLevel \n
///
/// Sample Command: \b Gpio_getLevel 20 \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_Gpio_GetLevel(MS_U32 *gpioNum)
{
    GPIOInfo_t gpioInfo;
    MS_U32 InputLevel;

    gpioInfo.gID = 0;
    gpioInfo.gInvertor = 0;
    gpioInfo.gPadNo = *gpioNum;

    msAPI_Gpio_SetgpioPtr(&gpioInfo);

    InputLevel = msAPI_Gpio_GetLevel();
    printf("GPIO[%lu], level[%lu]\n",*gpioNum,InputLevel);

    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief Enable interrupt for GPIO. Should set the GPIO as input first.
/// @param[in] gpioNum The gpio number.
/// @param[in] polarity High trigger or Low trigger
///         0: high
///         1: low
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b Gpio_EnableInterrupt \n
///
/// Sample Command: \n \b Gpio_EnableInterrupt 20 1 \n
///                 \b Gpio_EnableInterrupt 20 0 \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_Gpio_EnableInterrupt(MS_U32 *gpioNum, MS_U32 *polarity)
{
    printf("GPIO Num %lu\n", *gpioNum);

    if(_bInterruptInit == FALSE)
    {


        _appDemo_Gpio_Task_Info.pStack = MsOS_AllocateMemory(_appDemo_Gpio_Task_Info.u32StackSize, gs32CachedPoolID);

        if(!_appDemo_Gpio_Task_Info.pStack)
        {
            printf("Stack memory allocation fail\n");
            GEN_EXCEP;
        }

        _appDemo_Gpio_Task_Info.iId = MsOS_CreateTask( _appDemo_Gpio_Task,
                                                NULL,
                                                _appDemo_Gpio_Task_Info.ePriority,
                                                TRUE,
                                                _appDemo_Gpio_Task_Info.pStack,
                                                _appDemo_Gpio_Task_Info.u32StackSize,
                                                _appDemo_Gpio_Task_Info.szName);

        if(_appDemo_Gpio_Task_Info.iId < 0)
        {
            printf("MsOS_CreateTask fail\n");
            GEN_EXCEP;
        }

        _s32GpioEvent = MsOS_CreateEventGroup("GPIO_EVENT");

        if (_s32GpioEvent < 0)
        {
            printf("MsOS_CreateEventGroup fail\n");
            GEN_EXCEP;
        }



        REG_FIQ_MASK = 0xFFFF; // Reset reg_wk_fiq_mask
        REG_FIQ_POL = 0; // Reset reg_wk_fiq_pol

        _bInterruptInit = TRUE;
    }
    MS_U16 u16Mask = 0;

    switch(*gpioNum)
    {
#if PAD_S_GPIO0 != 9999
        case PAD_GPIO_PM0:
            u16Mask = 1 << 0;
            break;
#endif
#if PAD_S_GPIO1 != 9999

        case PAD_GPIO_PM1:
            u16Mask = 1 << 1;
            break;
#endif
#if PAD_S_GPIO2 != 9999
        case PAD_GPIO_PM2:
            u16Mask = 1 << 2;
            break;
#endif
#if PAD_S_GPIO3 != 9999
        case PAD_GPIO_PM3:
            u16Mask = 1 << 3;
            break;
#endif
#if PAD_S_GPIO4 != 9999
        case PAD_GPIO_PM4:
            u16Mask = 1 << 4;
            break;
#endif
#if PAD_S_GPIO5 != 9999
        case PAD_GPIO_PM5:
            u16Mask = 1 << 5;
            break;
#endif
        default:
            u16Mask = 0;
            printf("GPIO Interrupt not support\n");
            break;
    }

    if(u16Mask == 0)
    {
        return FALSE;
    }

    if(*polarity)
    {
        REG_FIQ_POL &= u16Mask; // Set reg_wk_fiq_pol
    }

    MsOS_AttachInterrupt(E_INT_IRQ_PM, _appDemo_Gpio_ISR);
    MsOS_EnableInterrupt(E_INT_IRQ_PM);

    REG_FIQ_MASK &= (MS_U16)~(u16Mask & 0xFFFF); // Set reg_wk_fiq_mask


    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Disable interrupt for GPIO.
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b Gpio_DisableInterrupt \n
///
/// Sample Command: \n \b Gpio_DisableInterrupt \n
///
//------------------------------------------------------------------------------
MS_BOOL appDemo_Gpio_DisableInterrupt(void)
{

    REG_FIQ_MASK = 0xFFFF; // Reset reg_wk_fiq_mask
    REG_FIQ_POL = 0; // Reset reg_wk_fiq_pol

    MsOS_DetachInterrupt(E_INT_IRQ_PM);
    MsOS_DisableInterrupt(E_INT_IRQ_PM);

    return TRUE;
}

