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
  @file demo_usb.c
  @brief The demo code for the USB test functions
*/
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#if (DEMO_USB_ECOS_TEST == 1)
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
#include "drvUSB_eCos.h" // utopia2

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

static MS_S32 s32USB2WrEventID = INVALID_EVENT_ID;
struct test_setting {
    char *pMountPath;
    unsigned int loop;
};
//-------------------------------------------------------------------------------------------------
// For USB Test
//-------------------------------------------------------------------------------------------------
extern MS_BOOL MApi_SystemDeviceHost_Init(void);
void USB_init_close_test(int option)
{
    if (option == 1)
    {
        MsOS_DelayTask(5000);
        printf("\n[TOP] USB Port Close\n");
        MDrv_USB_Port_Close(0);
        MDrv_USB_Port_Close(1);
        printf("\n\n");
        MsOS_DelayTask(5000);

        MDrv_USB_Port_Init(0);
        MDrv_USB_Port_Init(1);
    }
    else
    {
        MsOS_DelayTask(5000);
        printf("\n[TOP] USB Port Close\n");
        MDrv_USB_Port_Close(0);
        MDrv_USB_Port_Close(1);
        printf("[TOP] USB Driver Close\n");
        MDrv_UsbClose();
        printf("\n\n");
        MsOS_DelayTask(5000);

        MApi_SystemDeviceHost_Init();
    }
}

MS_BOOL Demo_USB_driver_load_test(MS_U32* u32TestCase, MS_U32* u32Mode)
{
    int option = 0;

    printf("Hello USB world!\n");
    if (!*u32TestCase) {
        printf("format: usb_driver_load_test count\n");
        return TRUE;
    }
    if (*u32Mode) {
        printf("Port init/close\n");
        option = 1;
    }
    else
        printf("All USB load/unload\n");

    //if (0)
    {
        int loop = *u32TestCase;

        while (loop) {
            printf("\n\t\t<<<<<<<<<<<<<<<< %d >>>>>>>>>>>>>>>>\n", loop--);
            USB_init_close_test(option);
            printf("..........................................\n");
            MsOS_DelayTask(10000);
        }
        printf("End of loop #%d\n", *u32TestCase);
    }
    return TRUE;
}

/* ==================================================================== */
#define BYTE_PER_SEC    512
//120K
#define BYTE_PER_RUN    (BYTE_PER_SEC*240)
//1000M
#define TOTAL_TEST_BYTE    (1024*1024*100*10)
//MS_U8 read_buf1[BYTE_PER_RUN+4096+512];    //120K Bytes

void performance_test(MS_U8 host_id)
{
//TONY
    MS_U32 ii;
    MS_S32 s32NonCachedPoolID = INVALID_POOL_ID;
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32NonCachedPoolID);

    printf("[USB] performance test at port %d\n", host_id);
    MS_U32 read_buf1 = (MS_U32)MsOS_AllocateMemory(BYTE_PER_RUN + 0x2000,s32NonCachedPoolID);
    MS_U32 read_buf = (read_buf1 + 0x1000) & ~(0xfff);
    printf("[USB] Hello tony buf1 0x%08X\n\r", read_buf1);
    printf("[USB] Hello tony buf 0x%08X\n\r", read_buf);

    printf("[USB] Hello tony performance test\n\r");

    for(ii=0;ii < 20 ; ii++)
    {
        printf("[USB] Test Start %d ================ \n\r", ii);
        //Read performance
        if(1){
            MS_U32 s_time;
            MS_U32 e_time;
            MS_U32 i;
            MS_BOOL b;

            //printf("[USB] Hello tony Read test start... \n\r");
            //printf("[USB] Hello tony buf addr 0x%08X \n\r", read_buf);
            //set start time
            s_time = MsOS_GetSystemTime();

            //test body
            for(i=0; i < (TOTAL_TEST_BYTE/BYTE_PER_RUN); i++ )
            {
                b = MDrv_UsbBlockReadToMIUEx(7+host_id*8, 0, 0x1000, (BYTE_PER_RUN/BYTE_PER_SEC), (MS_U32)read_buf);
                //printf("  -> R:%d %d\n\r", i, b);
                if(b == FALSE)
                {
                    printf("[USB] Read Fail\n\r");
                    return;
                }
            }

            //set end time
            e_time = MsOS_GetSystemTime();
            //cal performance
            printf("[USB] Read Performance %d-%d performance: %f MB/S\n\r",
                s_time, e_time, ((float)TOTAL_TEST_BYTE / ((float)(e_time-s_time)/1000))/(1024*1024));
        }

        //Write performance
        if(1){
            MS_U32 s_time;
            MS_U32 e_time;
            MS_U32 i;
            MS_BOOL b;

            //printf("[USB] Hello tony Write test start XD... \n\r");

            for(i = 0; i< BYTE_PER_RUN; i++)
                *((MS_U8*)read_buf + i) = i;

            //set start time
            s_time = MsOS_GetSystemTime();

            //test body
            for(i=0; i < (TOTAL_TEST_BYTE/BYTE_PER_RUN); i++ )
            {
                b = MDrv_UsbBlockWriteFromMIUEx(7+host_id*8, 0, 0x1000, (BYTE_PER_RUN/BYTE_PER_SEC), (MS_U32)read_buf);
                //printf("  -> R:%d %d\n\r", i, b);
                if(b == FALSE)
                {
                    printf("[USB] Write Fail\n\r");
                    return;
                }
            }

            //set end time
            e_time = MsOS_GetSystemTime();
            //cal performance
            printf("[USB] Write Performance %d-%d performance: %f MB/S\n\r",
                s_time, e_time, ((float)TOTAL_TEST_BYTE / ((float)(e_time-s_time)/1000))/(1024*1024));
        }
    } // end of for loop

    if (FALSE ==  MsOS_FreeMemory((void*) read_buf1, s32NonCachedPoolID))
        printf("!!!! ERROR: FreeNoncachedMem 0x%x fail \n",  (unsigned)read_buf1);

} // end of performance_test

MS_BOOL Demo_USB_API_perf(void)
{
    printf("Hello USB world!\n");
    if ( MDrv_UsbDeviceConnectBitEx(0) )
    {
        performance_test(0);
        return TRUE;
    }
    else
    {
        printf("please plug USB device into port 0!\n");
        return FALSE;
    }
}

/* ==================================================================== */
MS_U8 usb_buf[1024];
MS_BOOL USB_Disk_RW_test(char *pMountPath, MS_U32 seed)
{
    FILE *fp_r, *fp_w;
    char path_r[256]={0};
    char path_w[256]={0};
    MS_U32 ii;
    MS_U8 retv = 1;

    //sprintf(path_r,"%s/%s","/root/sdx1","testrw.bin");
    //sprintf(path_w,"%s/%s","/root/sdx1","testrw.bin");
    sprintf(path_w,"%s/%s",pMountPath,"testrw.bin");
    sprintf(path_r,"%s/%s",pMountPath,"testrw.bin");

    fp_w = MsFS_Fopen(path_w, "w");
    if(fp_w == NULL)
    {
       printf("can't open file %s\n", path_w);
       MsFS_Fclose(fp_w);
       return 0;
    }

    // prepare the output data
    for (ii=0; ii<1024; ii++)
        usb_buf[ii] = (ii + seed) % 256;

    // write 1M data to file
    for (ii=0; ii<1024; ii++)
        MsFS_Fwrite((void *)usb_buf, sizeof(MS_U8), 1024, fp_w);

    MsFS_Fflush(fp_w);
    MsFS_Fclose(fp_w);

    // read 1M data into buffer and compare it with current seed
    fp_r = MsFS_Fopen(path_r, "r");
    if(fp_r == NULL)
    {
       printf("can't open file %s\n", path_r);
       MsFS_Fclose(fp_r);
       return 0;
    }
    for (ii=0; ii<1024; ii++)
    {
        MS_U32 i;
        MsFS_Fread((void *)usb_buf, sizeof(MS_U8), 1024, fp_r);
        for (i=0; i<1024; i++)
            if (usb_buf[i] != ((i + seed) % 256))
            {
                printf("Compare fail[%d]!!!\n", i);
                retv = 0;
                break;
            }
        if (i < 1024)
        {
            printf("Break loop %d\n", ii);
            break;
        }
    }

    // Close read file
    MsFS_Fflush(fp_r);
    MsFS_Fclose(fp_r);

    // report message
    if (retv)
        printf("USB Disk read/write test OK (%d)\n", seed);
    else
        printf("!!!USB Disk read/write test FAIL (%d)\n", seed);

    return retv;
}

/* Command: USB_BurnIn mount_path loop# i
 * ex/ USB_BurnIn /root/sdx1 10
 */
MS_BOOL Demo_USB_BurnIn(char *pMountPath, MS_U32* u32TestCase)
{
    //MS_BOOL bRet = FALSE;

    printf("Hello USB world!\n");
    if (*u32TestCase == 0) {
        printf("format: usb_burnin mount_path count\n");
        return TRUE;
    }
    printf("loop %d\n", *u32TestCase);
    //if (!*u32TestCase)
    {
        int loop = *u32TestCase;
        int bRet = 1;

        while (loop && bRet)
            bRet = USB_Disk_RW_test(pMountPath, loop--);
        return bRet;
    }
    //return bRet;
}

MS_U8 usb_buf2[512];
#if 0
static void USB_Disk_Write_test(char *path_w, MS_U32 seed)
{
    FILE *fp_w;
    //MS_U32 ii;

    fp_w = MsFS_Fopen(path_w, "w");
    if(fp_w == NULL)
    {
        printf("can't open file %s\n", path_w);
        MsFS_Fclose(fp_w);
        return;
    }

    /* prepare the output data */
    //for (ii=0; ii<1024; ii++)
    //    usb_buf[ii] = (ii + seed) % 256;

    /* write 1M data to file */
    //for (ii=0; ii<1024; ii++)
    MsFS_Fwrite((void *)usb_buf, sizeof(MS_U8), 512, fp_w);

    MsFS_Fflush(fp_w);
    MsFS_Fclose(fp_w);
}

static void USB_Disk_Write2_test(char *path_w, MS_U32 seed)
{
    FILE *fp_w;
    //MS_U32 ii;

    fp_w = MsFS_Fopen(path_w, "w");
    if(fp_w == NULL)
    {
        printf("can't open file %s\n", path_w);
        MsFS_Fclose(fp_w);
        return;
    }

    /* prepare the output data */
    //for (ii=0; ii<512; ii++)
    //    usb_buf2[ii] = (ii + seed) % 256;

    /* write 1M data to file */
    //for (ii=0; ii<1024; ii++)
    MsFS_Fwrite((void *)usb_buf2, sizeof(MS_U8), 512, fp_w);

    MsFS_Fflush(fp_w);
    MsFS_Fclose(fp_w);
}
#endif

static void _USB_Write_Test(struct test_setting *setting)
{
    FILE *fp_w;
    char path_w[256] = {0};
    unsigned int loop = setting->loop;

    sprintf(path_w,"%s/%s",setting->pMountPath,"test_w.bin");
    fp_w = MsFS_Fopen(path_w, "w");
    if(fp_w == NULL)
    {
        printf("can't open file %s\n", path_w);
        MsFS_Fclose(fp_w);
        return;
    }

    while(loop) {
        //printf("W[1][%d]\n", loop);
        //USB_Disk_Write_test(path_w, loop--);
        MsFS_Fwrite((void *)usb_buf, sizeof(MS_U8), 512, fp_w);
        MsFS_Fflush(fp_w);
        --loop;
        MsOS_DelayTask(1);
    }
    MsFS_Fclose(fp_w);

    MsOS_SetEvent(s32USB2WrEventID, 0x100);
}

static void _USB_Write2_Test(struct test_setting *setting)
{
    FILE *fp_w;
    char path_w[256] = {0};
    unsigned int loop = setting->loop;

    sprintf(path_w,"%s/%s",setting->pMountPath,"test_w2.bin");
    fp_w = MsFS_Fopen(path_w, "w");
    if(fp_w == NULL)
    {
        printf("can't open file %s\n", path_w);
        MsFS_Fclose(fp_w);
        return;
    }

    while(loop) {
        //printf("W[2][%d]\n", loop);
        //USB_Disk_Write2_test(path_w, loop--);
        MsFS_Fwrite((void *)usb_buf2, sizeof(MS_U8), 512, fp_w);
        MsFS_Fflush(fp_w);
        --loop;
        MsOS_DelayTask(1);
    }
    MsFS_Fclose(fp_w);

    MsOS_SetEvent(s32USB2WrEventID, 0x200);
}

#define USB_TASK_STACK_SIZE 0x1000
static MS_U8 TaskStack0[USB_TASK_STACK_SIZE];
static MS_U8 TaskStack1[USB_TASK_STACK_SIZE];
MS_BOOL Demo_USB_BurnIn_2W(char *pMountPath, MS_U32* u32TestCase)
{
    //MS_S32 s32NonCachedPoolID = INVALID_POOL_ID;
    Task_Info writeTask = {-1, E_TASK_PRI_HIGH, NULL, (USB_TASK_STACK_SIZE), "Write Task"};
    Task_Info writeTask_2 = {-1, E_TASK_PRI_HIGH, NULL, (USB_TASK_STACK_SIZE), "Write Task 2"};
    MS_U32 u32Events = TEST_NO_EVENT;
    struct test_setting u_setting;

    printf("Hello USB world! 2 parallel write case\n");
    if (*u32TestCase == 0) {
        printf("format: usb_burnin_2w mount_path count\n");
        return FALSE;
    }

    s32USB2WrEventID = MsOS_CreateEventGroup("USB_2WR_Event");
    if (s32USB2WrEventID == INVALID_EVENT_ID) {
        printf("[%s][%d] create event group failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    u_setting.pMountPath = pMountPath;
    u_setting.loop = *u32TestCase;

    writeTask.pStack = TaskStack0;
    writeTask_2.pStack = TaskStack1;

    writeTask.iId = MsOS_CreateTask((TaskEntry)_USB_Write_Test,
                                        (MS_U32)&u_setting,
                                        writeTask.ePriority,
                                        TRUE,
                                        writeTask.pStack,
                                        writeTask.u32StackSize,
                                        writeTask.szName);

    writeTask_2.iId = MsOS_CreateTask((TaskEntry)_USB_Write2_Test,
                                        (MS_U32)&u_setting,
                                        writeTask_2.ePriority,
                                        TRUE,
                                        writeTask_2.pStack,
                                        writeTask_2.u32StackSize,
                                        writeTask_2.szName);

    int flag = 0;
    while (1)
    {
        MsOS_WaitEvent(s32USB2WrEventID, 0x300, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        //printf("[%s][%d] u32Events %"DTC_MS_U32_d"\n", __FUNCTION__, __LINE__, u32Events);
        flag += u32Events;
        if (flag == 0x300)
        {
            printf("\e[1m\e[21m 2 Write Test Completed! \e[0m\n");
            break;
        }
    }

    if (MsOS_DeleteEventGroup(s32USB2WrEventID) == FALSE)
    {
        printf("[%s][%d] delete eventgroup failed\n", __FUNCTION__, __LINE__);
    }

    if (MsOS_DeleteTask(writeTask.iId) == FALSE || MsOS_DeleteTask(writeTask_2.iId) == FALSE)
    {
        printf("[%s][%d] delete tasks failed\n", __FUNCTION__, __LINE__);
    }

    return TRUE;
}
#endif
