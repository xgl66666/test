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
/// @file   appDemo_MountNotifier.h
/// @author MStar Semiconductor Inc.
/// @brief  Mount notifier Function Sample Demo
///
/// @name Mount notifier Function Sample Command Usage
/// @verbatim
/// @endverbatim
///////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#if (DEMO_LINUX_MOUNT_NOTIFIER_TEST == 1)
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/prctl.h>
#include <dirent.h>

#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/mount.h>
#include <linux/fs.h>

#include <linux/types.h>
#include <linux/netlink.h>
#include <stdlib.h>

#include <stdio.h>
#include <syslog.h>

#include <sys/stat.h>

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mount.h>
#include <string.h>

#include "MsCommon.h"
#include "MsFS.h"
#include "demo_linux_mount_notifier.h"
#include "demo_utility.h"

//-------------------------------------------------------------------------------------------------
//                                MACROS
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define APP_DEMO_MOUNTNOTIFIER_TASK_STACK         (4096)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static void *_pTaskStack = NULL;
static MS_S32 _s32TaskId = -1;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
/// app demo of mount notifier Task, which handle all the Signal from Linux kenel.
/// The Mount notifier task parser the signal and mount the USB device to /tmp.
/// @return None
//-------------------------------------------------------------------------------------------------
static void _Demo_MountNotifier_Task(MS_U32 argc, void *argv)
{
    const int buffersize = 16 * 1024;
    struct      sockaddr_nl nls;
    struct      pollfd pfd;
    char buf_cache[1024*2] = {0};
    int ret;
    ssize_t byte_recv=0;

    // string token
    const char * pADD = NULL;
    const char * pREMOVE = NULL;
    const char * pSDA1 = NULL;

    //prctl(PR_SET_NAME, (unsigned long)"MountNotifier Task");
    // Initial socket
    memset(&nls, 0, sizeof(nls));

    nls.nl_family = AF_NETLINK;
    nls.nl_pid = getpid();
    nls.nl_groups = -1;

    pfd.events = POLLIN;
    pfd.fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if(pfd.fd == -1)
    {
        printf("Not root\n");
        return;
    }

    setsockopt(pfd.fd, SOL_SOCKET, SO_RCVBUFFORCE, &buffersize, sizeof(buffersize));

    if(bind(pfd.fd, ((struct sockaddr*)&nls), sizeof(nls)))
    {
        close(pfd.fd);
        printf("bind failed\n");
        return;
    }

    while(1)
    {
        memset(buf_cache, 0 ,sizeof(buf_cache));
        byte_recv=recv(pfd.fd,buf_cache, sizeof(buf_cache), 0);
        if((byte_recv > 0) && (byte_recv < sizeof(buf_cache)))
        {
           *(buf_cache + byte_recv) = '\0';
        }
        else
        {
            break;
        }
        //printf("======buf_cache: %s\n", buf_cache);

        pADD = strstr(buf_cache, "add@");
        if(pADD != NULL)
        {

            pSDA1 = strstr(buf_cache, "block/sda/sda1");

            if(pSDA1 != NULL)
            {
                printf("USB plugin : block/sda/sda1\n");
                ret = mount("/dev/sda1","/tmp","vfat",0,0);
                if(ret != 0)
                {
                    printf("mount fail \n");
                }
                else
                {
                    printf("mount /dev/sda1 --> /tmp success!! \n");
                }
            }
        }

        pREMOVE = strstr(buf_cache, "remove@");

        if(pREMOVE != NULL)
        {
            pSDA1 = strstr(buf_cache, "block/sda/sda1");

            if(pSDA1 != NULL)
            {
                printf("USB plugout : block/sda/sda1\n");
                ret = umount("/tmp");
                if(ret != 0)
                {
                    printf("umount fail \n");
                }
                else
                {
                    printf("umount /tmp success!! \n");
                }
            }
        }
    }
    close(pfd.fd);
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief The Mount notifier demo function. \n
///        The demo flow: \n
///          (1) Create mount notifier task. \n
///          (2) create and bind Netlink between Kernel and AP. \n
///          (3) recv the signal from Kernel. \n
///          (4) Parsing the siganl for USB hot plugin/plugout. \n
///          (5) mount/unmount USB device from "/dev/sda1" to "/tmp". \n
/// @param[in] None
/// @return TRUE - Success
/// @return FALSE - Failure
/// @sa
/// @note
/// Command: \b Gpio_Init \n
///
/// Sample Command: \b Gpio_Init \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_MountNotifier_Init(void)
{
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&s32MstarCachedPoolID);

    // create app demo task of input device
    _pTaskStack = MsOS_AllocateMemory( APP_DEMO_MOUNTNOTIFIER_TASK_STACK, s32MstarCachedPoolID);
    if (_pTaskStack == NULL)
    {
        printf("%s:MsOS_AllocateMemory[APP_DEMO_MOUNTNOTIFIER_TASK_STACK] Fail!!\n", __func__);
        return FALSE;
    }
    _s32TaskId = MsOS_CreateTask(_Demo_MountNotifier_Task,
                                         NULL,
                                         E_TASK_PRI_HIGH, // E_TASK_PRI_MEDIUM,
                                         TRUE,
                                         _pTaskStack,
                                         APP_DEMO_MOUNTNOTIFIER_TASK_STACK,
                                         "Demo mount notifier Tsk");
    if (_s32TaskId < 0)
    {
        printf("%s:MsOS_CreateTask[_Demo_MountNotifier_Task] Fail!!\n", __func__);
        return FALSE;

    }

    return TRUE;
}
#endif
