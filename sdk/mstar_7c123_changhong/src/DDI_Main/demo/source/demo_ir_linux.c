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
/// @file   appDemo_Input.h
/// @author MStar Semiconductor Inc.
/// @brief  Input Device Function Sample Demo
///
/// @name Input Device Function Sample Command Usage
/// @verbatim
/// @endverbatim
///////////////////////////////////////////////////////////////////////////////////////////////////
#if (DEMO_IR_LINUX_TEST == 1)
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stropts.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include "MsCommon.h"
#include "drvMMIO.h"
#include "demo_ir_linux.h"
#include "nec_keydef.h"
#include "demo_utility.h"
#include "drvDTC.h"

//-------------------------------------------------------------------------------------------------
//                                MACROS
//-------------------------------------------------------------------------------------------------
#define IR_IOC_MAGIC                'u'

#define MDRV_IR_INIT                _IO(IR_IOC_MAGIC, 0)
#define MDRV_IR_SET_DELAYTIME       _IOW(IR_IOC_MAGIC, 1, int)
#define MDRV_IR_GET_KEY             _IOW(IR_IOC_MAGIC, 2, int)
#define MDRV_IR_GET_LASTKEYTIME     _IOW(IR_IOC_MAGIC, 3, int)
#define MDRV_IR_PARSE_KEY           _IOW(IR_IOC_MAGIC, 4, int)
#define MDRV_IR_TEST                _IOW(IR_IOC_MAGIC, 5, int)
#define MDRV_IR_ENABLE_IR           _IOW(IR_IOC_MAGIC, 6, int)
#define MDRV_IR_IS_FANTASY_PROTOCOL_SUPPORTED _IOR(IR_IOC_MAGIC, 7, int)
#define MDRV_IR_ENABLE_FANTASY_DATA_TRANSFER _IOW(IR_IOC_MAGIC, 8, int)
#define MDRV_IR_SET_MASTER_PID       _IOW(IR_IOC_MAGIC, 9, int)
#define MDRV_IR_GET_MASTER_PID       _IOW(IR_IOC_MAGIC, 10, int)
#define IR_IOC_MAXNR                11

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define APP_DEMO_INPUT_TASK_STACK         (4096)
#define IR_1ST_DELAY_TIME_MS    800
#define IR_2ST_DELAY_TIME_MS    100

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static int gIR_fd = -1;
static MS_S32 stRCUSem = -1;
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static void *_pTaskStack = NULL;
static MS_S32 _s32TaskId = -1;
static MS_BOOL bExitIRDemoTask = FALSE;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL IrInput_Exit(void)
{
    MS_S32 s32MstarCachedPoolID = 0;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&s32MstarCachedPoolID);

    MsOS_DeleteMutex(stRCUSem);
    stRCUSem = -1;
    _s32TaskId = -1;

    if(_pTaskStack != NULL)
    {
        MsOS_FreeMemory(_pTaskStack, s32MstarCachedPoolID);
        _pTaskStack = NULL;
    }

    return TRUE;
}
//-------------------------------------------------------------------------------------------------
/// IrInput module initialization function.
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
static MS_BOOL IrInput_init(void)
{
    MS_VIRT virtBaseAddr = 0;
    MS_PHY phyBaseSize = 0;
    if(stRCUSem < 0)
    {
        stRCUSem = MsOS_CreateMutex (E_MSOS_FIFO, "MUTEX_IrInput", MSOS_PROCESS_PRIVATE);
    }

    if(FALSE == MsOS_ObtainMutex(stRCUSem, MSOS_WAIT_FOREVER))
    {
        return FALSE;
    }

    if(gIR_fd >= 0)
    {
        if(FALSE == MsOS_ReleaseMutex(stRCUSem))
        {
            return FALSE;
        }
        return TRUE;
    }

    if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &phyBaseSize, MS_MODULE_PM))
    {
        if(FALSE == MsOS_ReleaseMutex(stRCUSem))
        {
            return FALSE;
        }
        return FALSE;
    }
    printf("[%s][%d] virtBaseAddr=0x%08"DTC_MS_VIRT_x" phyBaseSize=0x%08"DTC_MS_PHY_x"\n",
        __FUNCTION__, __LINE__, virtBaseAddr, phyBaseSize);

    gIR_fd = open("/dev/ir", O_RDWR);
    if (gIR_fd < 0)
    {
        fprintf(stderr, "Fail to open IR Kernal Module\n");
        if(FALSE == MsOS_ReleaseMutex(stRCUSem))
        {
            return FALSE;
        }
        return FALSE;
    }

    ioctl(gIR_fd, MDRV_IR_INIT);

    MS_IR_DelayTime time;
    time.u32_1stDelayTimeMs = IR_1ST_DELAY_TIME_MS;
    time.u32_2ndDelayTimeMs = IR_2ST_DELAY_TIME_MS;
    ioctl(gIR_fd, MDRV_IR_SET_DELAYTIME, &time);

    if(FALSE == MsOS_ReleaseMutex(stRCUSem))
        {
            return FALSE;
        }
    return TRUE;
}

/**
 * @fn zgu32 zal_IrInput_xlate_nec(zgu32 uiKey, zgu32 uiMask)
 * @brief
 *     Transform the key vlaue of NEC into Zinwell
 *
 * @param[in] uiKey The key value of NEC
 * @param[in] uiMask The Key mask of NEC
 *
 * @return The Key value of Zinwell
 */
//-------------------------------------------------------------------------------------------------
/// Transform the key vlaue of NEC into MstarDTVIR
/// @param[in] uiKey The key value of NEC
/// @param[in] uiMask The Key mask of NEC
/// @return The Key value of MstarDTVIR
//-------------------------------------------------------------------------------------------------
static MS_U32 IrInput_xlate_nec(MS_U32 uiKey, MS_U32 uiMask)
{
    switch(uiKey & uiMask)
    {
        case NEC_KEY_SLCT:          uiKey = DTV_KEY_OK;          break;
        case NEC_KEY_EXIT:          uiKey = DTV_KEY_EXIT;        break;
        case NEC_KEY_MENU:          uiKey = DTV_KEY_MENU;        break;
        case NEC_KEY_SUBT:          uiKey = DTV_KEY_SUBT;        break;
        case NEC_KEY_TTX:           uiKey = DTV_KEY_TTX;         break;
        case NEC_KEY_LEFT:          uiKey = DTV_KEY_LEFT;        break;
        case NEC_KEY_RIGHT:         uiKey = DTV_KEY_RIGHT;       break;
        case NEC_KEY_UP:            uiKey = DTV_KEY_UP;          break;
        case NEC_KEY_DOWN:          uiKey = DTV_KEY_DOWN;        break;
        case NEC_KEY_CH_UP:         uiKey = DTV_KEY_CHUP;        break;
        case NEC_KEY_CH_DOWN:       uiKey = DTV_KEY_CHDOWN;      break;
        case NEC_KEY_0:             uiKey = DTV_KEY_0;           break;
        case NEC_KEY_1:             uiKey = DTV_KEY_1;           break;
        case NEC_KEY_2:             uiKey = DTV_KEY_2;           break;
        case NEC_KEY_3:             uiKey = DTV_KEY_3;           break;
        case NEC_KEY_4:             uiKey = DTV_KEY_4;           break;
        case NEC_KEY_5:             uiKey = DTV_KEY_5;           break;
        case NEC_KEY_6:             uiKey = DTV_KEY_6;           break;
        case NEC_KEY_7:             uiKey = DTV_KEY_7;           break;
        case NEC_KEY_8:             uiKey = DTV_KEY_8;           break;
        case NEC_KEY_9:             uiKey = DTV_KEY_9;           break;
        case NEC_KEY_VFORMAT:       uiKey = DTV_KEY_VFORMAT;     break;
        case NEC_KEY_TV_RADIO:      uiKey = DTV_KEY_TV;          break;
        case NEC_KEY_PIP:           uiKey = DTV_KEY_PIP;         break;
        case NEC_KEY_AR:            uiKey = DTV_KEY_ARATIO;      break;
        case NEC_KEY_AUDIO:         uiKey = DTV_KEY_AUDIO;       break;
        case NEC_KEY_INFO:          uiKey = DTV_KEY_INFO;        break;
        case NEC_KEY_FAV:           uiKey = DTV_KEY_FAV;         break;
        case NEC_KEY_LIST:          uiKey = DTV_KEY_LIST;        break;
        case NEC_KEY_RED:           uiKey = DTV_KEY_RED;         break;
        case NEC_KEY_GREEN:         uiKey = DTV_KEY_GREEN;       break;
        case NEC_KEY_YELLOW:        uiKey = DTV_KEY_YELLOW;      break;
        case NEC_KEY_BLUE:          uiKey = DTV_KEY_BLUE;        break;
        case NEC_KEY_VOL_UP:        uiKey = DTV_KEY_VUP;         break;
        case NEC_KEY_VOL_DOWN:      uiKey = DTV_KEY_VDOWN;       break;
        case NEC_KEY_MUTE:          uiKey = DTV_KEY_MUTE;        break;
        case NEC_KEY_STANDBY:       uiKey = DTV_KEY_STDBY;       break;

        case NEC_KEY_PP:            uiKey = DTV_KEY_PP;          break;
        case NEC_KEY_PLAY:          uiKey = DTV_KEY_PLAY;        break;
        case NEC_KEY_STOP:          uiKey = DTV_KEY_STOP;        break;
        case NEC_KEY_PAUSE:         uiKey = DTV_KEY_PAUSE;       break;
        case NEC_KEY_FF:            uiKey = DTV_KEY_FF;          break;
        case NEC_KEY_REW:           uiKey = DTV_KEY_FR;          break;
        case NEC_KEY_FILE:          uiKey = DTV_KEY_FILE;        break;
        case NEC_KEY_PICTURE:       uiKey = DTV_KEY_PICTURE;     break;
        case NEC_KEY_EXT:           uiKey = DTV_KEY_EXT;         break;
        case NEC_KEY_REC_LIST:      uiKey = DTV_KEY_REC_LIST;    break;
        case NEC_KEY_REC:           uiKey = DTV_KEY_REC;         break;

#ifdef NEW_REMOTE
        case NEC_KEY_CC:            uiKey = DTV_KEY_SUBT;        break;
        case NEC_KEY_EPG:           uiKey = DTV_KEY_EPG;         break;
        case NEC_KEY_TEXT:          uiKey = DTV_KEY_TEXT;        break;
#endif

        default:
                                    return 0;
                                    break;
    }

    return uiKey;
}

static MS_U32 IrInput_GetKey(void)
{
    fd_set rfds;
    int ret;

    MsOS_ObtainMutex(stRCUSem, MSOS_WAIT_FOREVER);

    if(gIR_fd < 0)
    {
        MsOS_ReleaseMutex(stRCUSem);
        return 0;
    }

    FD_ZERO(&rfds);
    FD_SET(gIR_fd, &rfds);
    ret = select(gIR_fd + 1, &rfds, NULL, NULL, NULL);

    if (ret <= 0 || !FD_ISSET(gIR_fd, &rfds))
    {
        MsOS_ReleaseMutex(stRCUSem);
        return 0;
    }

    unsigned int u32Data = 0;
    if(read(gIR_fd, &u32Data, sizeof(u32Data)) != sizeof(u32Data))
    {
        MsOS_ReleaseMutex(stRCUSem);
        return 0;
    }

    printf("u8Key = 0x%02x\n",(u32Data >>8)&0xFFFF);

    if(((u32Data >>8)&0xFFFF) == NEC_CUST_CODE)
    {
        u32Data = IrInput_xlate_nec(u32Data, 0xFFFF);

        MsOS_ReleaseMutex(stRCUSem);
        return u32Data;
    }

    MsOS_ReleaseMutex(stRCUSem);
    return 0;
}

//-------------------------------------------------------------------------------------------------
/// app demo of input device to handle all interrupts and events.
/// @return None
//-------------------------------------------------------------------------------------------------
static void _Demo_Input_Task(MS_U32 argc, void *argv)
{
    IrInput_init();
    while(bExitIRDemoTask == FALSE)
    {
        //printf("IrInput_GetKey: 0x%lx \n", IrInput_GetKey());
        IrInput_GetKey();
    }
    bExitIRDemoTask = FALSE;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief Create the app demo task of IR input device and initialze all parameters.
/// @param[in] None
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b IR_Init \n
///
/// Sample Command: \b IR_Init \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_Input_Init_linux(void)
{
    MS_S32 s32MstarCachedPoolID = 0;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&s32MstarCachedPoolID);
    // create app demo task of input device
    _pTaskStack = MsOS_AllocateMemory( APP_DEMO_INPUT_TASK_STACK, s32MstarCachedPoolID);
    if (_pTaskStack == NULL)
    {
        printf("Alloc Memory for APP_DEMO_INPUT_TASK_STACK Fail !!\n");
        return FALSE;
    }

    _s32TaskId = MsOS_CreateTask(_Demo_Input_Task,
                                         NULL,
                                         E_TASK_PRI_HIGH, // E_TASK_PRI_MEDIUM,
                                         TRUE,
                                         _pTaskStack,
                                         APP_DEMO_INPUT_TASK_STACK,
                                         "Demo Input Tsk");
    if (_s32TaskId < 0)
    {
        printf("Creat demo task of IR input Fail !!\n");
        MsOS_FreeMemory(_pTaskStack, s32MstarCachedPoolID);
        _pTaskStack = NULL;
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Exit the app demo task of IR
/// @param[in] None
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b IR_Init \n
///
/// Sample Command: \b IR_Init \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_Input_Exit_linux(void)
{
    bExitIRDemoTask = TRUE;
    return IrInput_Exit();
}

//------------------------------------------------------------------------------
/// @brief The IR User input demo help.
/// @param[in] None
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b HelpIR \n
///
/// Sample Command: \b HelpIR \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_Input_Help_linux(void)
{
    printf ("------------------------------------IR Help--------------------------------------\n");
    printf ("press \"IR_Init\" to Initial IR\n");
    printf ("---------------------------------End of IR Help----------------------------------\n");

    return TRUE;
}
#endif
