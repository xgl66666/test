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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////


#if (DEMO_7SEGMENTS_TEST == 1)

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "string.h"
#include "Board.h"
#include "drvGPIO.h"
#include "demo_7segments.h"


//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------
#define TASK_DISPLAYTIME_STACKSIZE       0x2000
MS_S32 s32DisplayTimeTaskID = -1;
static MS_U8 _u8TaskDisplayTime[TASK_DISPLAYTIME_STACKSIZE];
//                   A
//              ______
//
//         F |     G   |   B
//              ______
//
//         E |     D   |   C
//              ______
//                              .  Pointer

#define DIGITAL_A_GPIO  BOARD_DIGITAL_A_GPIO
#define DIGITAL_B_GPIO  BOARD_DIGITAL_B_GPIO
#define DIGITAL_C_GPIO  BOARD_DIGITAL_C_GPIO
#define DIGITAL_D_GPIO  BOARD_DIGITAL_D_GPIO
#define DIGITAL_E_GPIO  BOARD_DIGITAL_E_GPIO
#define DIGITAL_F_GPIO  BOARD_DIGITAL_F_GPIO
#define DIGITAL_G_GPIO  BOARD_DIGITAL_G_GPIO
#define DIGITAL_DP_GPIO BOARD_DIGITAL_DP_GPIO

#define DIGITAL_DIGI01_GPIO  BOARD_DIGITAL_DIGI01_GPIO
#define DIGITAL_DIGI02_GPIO  BOARD_DIGITAL_DIGI02_GPIO
#define DIGITAL_DIGI03_GPIO  BOARD_DIGITAL_DIGI03_GPIO
#define DIGITAL_DIGI04_GPIO  BOARD_DIGITAL_DIGI04_GPIO

#define DIGITAL_L1AND2_GPIO  BOARD_DIGITAL_L1AND2_GPIO
/* show time Minutes:Seconds  */

static MS_BOOL _bDisplayRun = FALSE;
//-------------------------------------------------------------------------------------------------
// Local Function
//-------------------------------------------------------------------------------------------------
/* show time Minutes:Seconds  */
static void _Cleardigi(void)
{
    mdrv_gpio_set_high(DIGITAL_A_GPIO);
    mdrv_gpio_set_high(DIGITAL_B_GPIO);
    mdrv_gpio_set_high(DIGITAL_C_GPIO);
    mdrv_gpio_set_high(DIGITAL_D_GPIO);
    mdrv_gpio_set_high(DIGITAL_E_GPIO);
    mdrv_gpio_set_high(DIGITAL_F_GPIO);
    mdrv_gpio_set_high(DIGITAL_G_GPIO);
    mdrv_gpio_set_high(DIGITAL_DP_GPIO);


    mdrv_gpio_set_high(DIGITAL_DIGI01_GPIO);
    mdrv_gpio_set_high(DIGITAL_DIGI02_GPIO);
    mdrv_gpio_set_high(DIGITAL_DIGI03_GPIO);
    mdrv_gpio_set_high(DIGITAL_DIGI04_GPIO);
}
static void _DisplayDigi(MS_U8 digi, MS_U8 value)
{
    /*   Reset GPIO*/

    mdrv_gpio_set_high(DIGITAL_A_GPIO);
    mdrv_gpio_set_high(DIGITAL_B_GPIO);
    mdrv_gpio_set_high(DIGITAL_C_GPIO);
    mdrv_gpio_set_high(DIGITAL_D_GPIO);
    mdrv_gpio_set_high(DIGITAL_E_GPIO);
    mdrv_gpio_set_high(DIGITAL_F_GPIO);
    mdrv_gpio_set_high(DIGITAL_G_GPIO);
//    mdrv_gpio_set_high(DIGITAL_DP_GPIO);


    mdrv_gpio_set_high(DIGITAL_DIGI01_GPIO);
    mdrv_gpio_set_high(DIGITAL_DIGI02_GPIO);
    mdrv_gpio_set_high(DIGITAL_DIGI03_GPIO);
    mdrv_gpio_set_high(DIGITAL_DIGI04_GPIO);


    switch(digi)
    {
        case 0:
            mdrv_gpio_set_low(DIGITAL_DIGI01_GPIO);
            break;
        case 1:
            mdrv_gpio_set_low(DIGITAL_DIGI02_GPIO);
            break;
        case 2:
            mdrv_gpio_set_low(DIGITAL_DIGI03_GPIO);
            break;
        case 3:
            mdrv_gpio_set_low(DIGITAL_DIGI04_GPIO);
            break;
        default:
            printf("[%s][%d] no match selection \n",__FUNCTION__,__LINE__);
            break;
    }

    switch(value)
    {
        case 0:
            mdrv_gpio_set_low(DIGITAL_A_GPIO);
            mdrv_gpio_set_low(DIGITAL_B_GPIO);
            mdrv_gpio_set_low(DIGITAL_F_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            mdrv_gpio_set_low(DIGITAL_E_GPIO);
            mdrv_gpio_set_low(DIGITAL_D_GPIO);
            break;
        case 1:
            mdrv_gpio_set_low(DIGITAL_B_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            break;
        case 2:
            mdrv_gpio_set_low(DIGITAL_A_GPIO);
            mdrv_gpio_set_low(DIGITAL_B_GPIO);
            mdrv_gpio_set_low(DIGITAL_G_GPIO);
            mdrv_gpio_set_low(DIGITAL_E_GPIO);
            mdrv_gpio_set_low(DIGITAL_D_GPIO);
            break;
        case 3:
            mdrv_gpio_set_low(DIGITAL_A_GPIO);
            mdrv_gpio_set_low(DIGITAL_B_GPIO);
            mdrv_gpio_set_low(DIGITAL_G_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            mdrv_gpio_set_low(DIGITAL_D_GPIO);
            break;
        case 4:
            mdrv_gpio_set_low(DIGITAL_F_GPIO);
            mdrv_gpio_set_low(DIGITAL_G_GPIO);
            mdrv_gpio_set_low(DIGITAL_B_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            break;
        case 5:
            mdrv_gpio_set_low(DIGITAL_A_GPIO);
            mdrv_gpio_set_low(DIGITAL_F_GPIO);
            mdrv_gpio_set_low(DIGITAL_G_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            mdrv_gpio_set_low(DIGITAL_D_GPIO);
            break;
        case 6:
            mdrv_gpio_set_low(DIGITAL_A_GPIO);
            mdrv_gpio_set_low(DIGITAL_F_GPIO);
            mdrv_gpio_set_low(DIGITAL_G_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            mdrv_gpio_set_low(DIGITAL_D_GPIO);
            mdrv_gpio_set_low(DIGITAL_E_GPIO);
            break;
        case 7:
            mdrv_gpio_set_low(DIGITAL_A_GPIO);
            mdrv_gpio_set_low(DIGITAL_B_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            mdrv_gpio_set_low(DIGITAL_F_GPIO);
            break;
        case 8:
            mdrv_gpio_set_low(DIGITAL_A_GPIO);
            mdrv_gpio_set_low(DIGITAL_B_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            mdrv_gpio_set_low(DIGITAL_D_GPIO);
            mdrv_gpio_set_low(DIGITAL_E_GPIO);
            mdrv_gpio_set_low(DIGITAL_F_GPIO);
            mdrv_gpio_set_low(DIGITAL_G_GPIO);
            break;
        case 9:
            mdrv_gpio_set_low(DIGITAL_A_GPIO);
            mdrv_gpio_set_low(DIGITAL_B_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            mdrv_gpio_set_low(DIGITAL_D_GPIO);
            mdrv_gpio_set_low(DIGITAL_F_GPIO);
            mdrv_gpio_set_low(DIGITAL_G_GPIO);
            break;
        default:
            printf("[%s][%d] no match selection \n",__FUNCTION__,__LINE__);
            break;
    }


}

static void DisplayTime_Task(void)
{
    MS_U32 u32MiniSeconds = 0,Seconds = 0,Minutes =0;
    MS_U8  u8Digi[4] = {0}, Lastseond = 0;
    MS_BOOL bFlash = false;
    _bDisplayRun = TRUE;
    while(_bDisplayRun)
    {
        MsOS_DelayTask(2); // Change pre Seconds

        u32MiniSeconds = MsOS_GetSystemTime();
        Seconds = (u32MiniSeconds / 1000)%60;
        Minutes = ((u32MiniSeconds / 1000) / 60)%60 ;
        u8Digi[3] = Seconds % 10;
        if (Lastseond != u8Digi[3])
        {
            bFlash = !bFlash;
        }
        Lastseond = u8Digi[3];
        u8Digi[2] = Seconds / 10;
        u8Digi[1] = Minutes % 10;
        u8Digi[0] = Minutes / 10;
        //printf("%d %d : %d %d\n",u8Digi[3],u8Digi[2],u8Digi[1],u8Digi[0]);
        //_Cleardigi();
        _DisplayDigi(0,u8Digi[0]);
        MsOS_DelayTask(2);
        _DisplayDigi(1,u8Digi[1]);
        MsOS_DelayTask(2);
        _DisplayDigi(2,u8Digi[2]);
        MsOS_DelayTask(2);
        _DisplayDigi(3,u8Digi[3]);
        if (bFlash)
        {
            //printf("0 \n");
            mdrv_gpio_set_low(DIGITAL_DP_GPIO);
            mdrv_gpio_set_low(DIGITAL_L1AND2_GPIO);
        }
        else
        {
            //printf("1 \n");
            mdrv_gpio_set_high(DIGITAL_DP_GPIO);
            mdrv_gpio_set_high(DIGITAL_L1AND2_GPIO);
        }

    }
}

//-------------------------------------------------------------------------------------------------
// Demo Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief Create and start the app demo task of 7SegmentStartTime to show system time on 7Segments device
/// @param[in] None
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b 7SegmentStartTime \n
///
/// Sample Command: \b 7SegmentStartTime \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_DisplayTime_Start(void)
{

    if (s32DisplayTimeTaskID < 0)
    {
        memset(_u8TaskDisplayTime,0,sizeof(MS_U8)*TASK_DISPLAYTIME_STACKSIZE);
        s32DisplayTimeTaskID = MsOS_CreateTask((TaskEntry) DisplayTime_Task,
                                        (MS_U32)NULL,
                                        E_TASK_PRI_HIGHEST,
                                        TRUE,
                                        _u8TaskDisplayTime,
                                        TASK_DISPLAYTIME_STACKSIZE,
                                        "Display Time" );
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief 7SegmentStopTime to stop show system time on 7Segments device
/// @param[in] None
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b 7SegmentStopTime \n
///
/// Sample Command: \b 7SegmentStopTime \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_DisplayTime_Stop(void)
{
    if (s32DisplayTimeTaskID > 0)
    {
        //MsOS_DeleteTask(s32DisplayTimeTaskID);
        _bDisplayRun = FALSE;
        s32DisplayTimeTaskID = -1;
        _Cleardigi();

    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief  The sample code to set 7-Segments Display
/// @param[in] 0 : Digital
/// @param[in] 1 : Value
/// @param[]in 2 : DP is Set or not
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b 7SegmentSet \n
///
/// Sample Command: \b  7SegmentSet 3 5 1  \n
/// show "5" on 3rd digits
//------------------------------------------------------------------------------
MS_BOOL Demo_DisplayDigiNum(MS_U32* arg0, MS_U32* arg1, MS_U32* arg2)
{
    MS_BOOL uDP = FALSE;
    MS_U8   u8Value = 0, u8DiguNum = 0;
    printf("[%s][%d] \n",__FUNCTION__,__LINE__);


    if ((arg0 == NULL) || (arg1 == NULL) ||(arg2 == NULL))
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    u8DiguNum = *arg0;
    u8Value   = *arg1;
    uDP       = *arg2;

    /* if G can't be set, please check reg_spi_is_gpio[2] = 1  , reg[0e6a]#2 = 1*/

    /*   Reset GPIO*/
    mdrv_gpio_set_high(DIGITAL_A_GPIO);
    mdrv_gpio_set_high(DIGITAL_B_GPIO);
    mdrv_gpio_set_high(DIGITAL_C_GPIO);
    mdrv_gpio_set_high(DIGITAL_D_GPIO);
    mdrv_gpio_set_high(DIGITAL_E_GPIO);
    mdrv_gpio_set_high(DIGITAL_F_GPIO);
    mdrv_gpio_set_high(DIGITAL_G_GPIO);
    mdrv_gpio_set_high(DIGITAL_DP_GPIO);


    mdrv_gpio_set_high(DIGITAL_DIGI01_GPIO);
    mdrv_gpio_set_high(DIGITAL_DIGI02_GPIO);
    mdrv_gpio_set_high(DIGITAL_DIGI03_GPIO);
    mdrv_gpio_set_high(DIGITAL_DIGI04_GPIO);

    mdrv_gpio_set_high(DIGITAL_L1AND2_GPIO);

    if (uDP == TRUE)
    {
        mdrv_gpio_set_low(DIGITAL_DP_GPIO);
    }
    switch(u8DiguNum)
    {
        case 0:
            mdrv_gpio_set_low(DIGITAL_DIGI01_GPIO);
            break;
        case 1:
            mdrv_gpio_set_low(DIGITAL_DIGI02_GPIO);
            break;
        case 2:
            mdrv_gpio_set_low(DIGITAL_DIGI03_GPIO);
            break;
        case 3:
            mdrv_gpio_set_low(DIGITAL_DIGI04_GPIO);
            break;
        default:
            printf("[%s][%d] no match selection \n",__FUNCTION__,__LINE__);
            break;
    }

    switch(u8Value)
    {
        case 0:
            mdrv_gpio_set_low(DIGITAL_A_GPIO);
            mdrv_gpio_set_low(DIGITAL_B_GPIO);
            mdrv_gpio_set_low(DIGITAL_F_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            mdrv_gpio_set_low(DIGITAL_E_GPIO);
            mdrv_gpio_set_low(DIGITAL_D_GPIO);
            break;
        case 1:
            mdrv_gpio_set_low(DIGITAL_B_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            break;
        case 2:
            mdrv_gpio_set_low(DIGITAL_A_GPIO);
            mdrv_gpio_set_low(DIGITAL_B_GPIO);
            mdrv_gpio_set_low(DIGITAL_G_GPIO);
            mdrv_gpio_set_low(DIGITAL_E_GPIO);
            mdrv_gpio_set_low(DIGITAL_D_GPIO);
            break;
        case 3:
            mdrv_gpio_set_low(DIGITAL_A_GPIO);
            mdrv_gpio_set_low(DIGITAL_B_GPIO);
            mdrv_gpio_set_low(DIGITAL_G_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            mdrv_gpio_set_low(DIGITAL_D_GPIO);
            break;
        case 4:
            mdrv_gpio_set_low(DIGITAL_F_GPIO);
            mdrv_gpio_set_low(DIGITAL_G_GPIO);
            mdrv_gpio_set_low(DIGITAL_B_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            break;
        case 5:
            mdrv_gpio_set_low(DIGITAL_A_GPIO);
            mdrv_gpio_set_low(DIGITAL_F_GPIO);
            mdrv_gpio_set_low(DIGITAL_G_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            mdrv_gpio_set_low(DIGITAL_D_GPIO);
            break;
        case 6:
            mdrv_gpio_set_low(DIGITAL_A_GPIO);
            mdrv_gpio_set_low(DIGITAL_F_GPIO);
            mdrv_gpio_set_low(DIGITAL_G_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            mdrv_gpio_set_low(DIGITAL_D_GPIO);
            mdrv_gpio_set_low(DIGITAL_E_GPIO);
            break;
        case 7:
            mdrv_gpio_set_low(DIGITAL_A_GPIO);
            mdrv_gpio_set_low(DIGITAL_B_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            mdrv_gpio_set_low(DIGITAL_F_GPIO);
            break;
        case 8:
            mdrv_gpio_set_low(DIGITAL_A_GPIO);
            mdrv_gpio_set_low(DIGITAL_B_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            mdrv_gpio_set_low(DIGITAL_D_GPIO);
            mdrv_gpio_set_low(DIGITAL_E_GPIO);
            mdrv_gpio_set_low(DIGITAL_F_GPIO);
            mdrv_gpio_set_low(DIGITAL_G_GPIO);
            break;
        case 9:
            mdrv_gpio_set_low(DIGITAL_A_GPIO);
            mdrv_gpio_set_low(DIGITAL_B_GPIO);
            mdrv_gpio_set_low(DIGITAL_C_GPIO);
            mdrv_gpio_set_low(DIGITAL_D_GPIO);
            mdrv_gpio_set_low(DIGITAL_F_GPIO);
            mdrv_gpio_set_low(DIGITAL_G_GPIO);
            break;
        default:
            printf("[%s][%d] no match selection \n",__FUNCTION__,__LINE__);
            break;
    }
    return TRUE;
}


#endif
