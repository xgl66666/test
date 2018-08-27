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
  @file demo_bootlinux.c
  @brief The demo code for Linux kernel booting
*/

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#if (DEMO_ECOS_BOOT_LINUX == 1)
#include "MsCommon.h"
#include "demo_bootlinux.h"
#include "demo_utility.h"
#include "MsOS.h"
#include "MsFS.h"

static struct tag *params;

static void _Boot_Linux(void) __attribute__((section(".bootlinux_start")));
static void _MMU_Table_Change(void) __attribute__((section(".bootlinux")));

static MS_U32 _CPU_MMU_TTB_Eead (void) __attribute__((section(".bootlinux")));
static void _CPU_MMU_Map_Memory (unsigned int va, unsigned int pa, unsigned int size, int access, int attributes) __attribute__((section(".bootlinux")));

static void _Setup_Start_Tag (void) __attribute__((section(".bootlinux")));
static void _Setup_Commandline_Tag (char *commandline) __attribute__((section(".bootlinux")));
static void _Setup_End_Tag (void) __attribute__((section(".bootlinux")));

static LINUX_KERNEL_MODE LinuxKernelMode = MODE_32BIT;

static unsigned int _CPU_MMU_TTB_Eead (void)
{
    MS_U32 ttb;

    asm volatile ("mrc p15, 0, %0, c2, c0, 0"
                   : "=r" (ttb)
                   :
                   : "memory");
    return ttb;
}

static void _CPU_MMU_Map_Memory (unsigned int va, unsigned int pa, unsigned int size, int access, int attributes)
{
    MS_U32 ttb;
    MS_U32 *ptr;

    // Round up to MB so that we only use sections
    size += 0x000FFFFF;
    size &= ~0x000FFFFF;

    while (size > 0)
    {   
        // Generate the page tables
        int temp = 0;
        
        ttb  = _CPU_MMU_TTB_Eead ();
        ptr  = (unsigned int *)LEVEL_1_ADDRESS(va,ttb);
        *ptr = pa | (access << 10) | (attributes << 2) | 2;
        
        temp = 0x00100000;

        va += temp;
        pa += temp;
        size -= temp;
    }
}

static void _MMU_Table_Change(void)
{
    MS_U32 ttb;

    ttb  = _CPU_MMU_TTB_Eead (); 

    memset((MS_U8 *)ttb, 0, 0x4000);

    __asm__ (
        "mrc  p15, 0, r0, c1, c0, 0\n"
        "bic  r0, r0, #0x1\n"
        "mcr  p15, 0, r0, c1, c0, 0\n"
        :::"r0"
    );

    _CPU_MMU_Map_Memory(0x15000000, 0x15000000, 0x0b000000, MMU_AP_FULL, 0);
    _CPU_MMU_Map_Memory(0x14000000, 0x14000000, 0x01000000, MMU_AP_FULL, MMU_CACHED | MMU_BUFFERED);
    _CPU_MMU_Map_Memory(0x20000000, 0x20000000, 0x20000000, MMU_AP_FULL, MMU_CACHED | MMU_BUFFERED);
    _CPU_MMU_Map_Memory(0x40000000, 0x20000000, 0x20000000, MMU_AP_FULL, MMU_BUFFERED);
    _CPU_MMU_Map_Memory(0x60000000, 0x60000000, 0x20000000, MMU_AP_FULL, MMU_CACHED | MMU_BUFFERED);
    _CPU_MMU_Map_Memory(0x80000000, 0x60000000, 0x20000000, MMU_AP_FULL, MMU_BUFFERED);

    __asm__ (
        "mrc  p15, 0, r0, c1, c0, 0\n"
        "orr  r0, r0, #0x1\n"
        "mcr  p15, 0, r0, c1, c0, 0\n"
        :::"r0"
    );

}

static void _Setup_Start_Tag (void)
{
    params = (struct tag *) 0x20200100;

    params->hdr.tag = ATAG_CORE;
    params->hdr.size = tag_size (tag_core);

    params->u.core.flags = 0;
    params->u.core.pagesize = 0;
    params->u.core.rootdev = 0;

    params = tag_next (params);
}

static void _Setup_Commandline_Tag (char *commandline)
{
    char *p;

    if (!commandline)
        return;

    /* eat leading white space */
    for (p = commandline; *p == ' '; p++);

    /* skip non-existent command lines so the kernel will still
     * use its default command line.
     */
    if (*p == '\0')
        return;

    params->hdr.tag = ATAG_CMDLINE;
    params->hdr.size =
        (sizeof (struct tag_header) + strlen (p) + 1 + 4) >> 2;

    strcpy (params->u.cmdline.cmdline, p);

    params = tag_next (params);
}

static void _Setup_End_Tag (void)
{
    params->hdr.tag = ATAG_NONE;
    params->hdr.size = 0;

}

static void _Boot_Linux(void)
{
    void    (*kernel_entry)(MS_U32 $0, MS_U32 arch, MS_U32 params);
    char * cmd_ptr = (char *)COMMAND_LINE_ADDRESS;
    unsigned long cmd_addr = (unsigned long)cmd_ptr;

    kernel_entry = (void (*)(MS_U32, MS_U32, MS_U32))LINUX_32BIT_KERNEL_ENTRY;

    _MMU_Table_Change();

    if (LinuxKernelMode == MODE_32BIT)
    {
        char *commandline = LINUX_KERNEL_COMMAND_LINE;
        // Setup command line
        _Setup_Start_Tag();
        _Setup_Commandline_Tag(commandline);
        _Setup_End_Tag();
        
        kernel_entry(0, MACHINE_ID, COMMAND_LINE_ADDRESS);
    }
    else if (LinuxKernelMode == MODE_64BIT)
    {
        char *commandline = LINUX_KERNEL_COMMAND_LINE;
        strncpy(cmd_ptr,commandline,strlen(commandline));
        cmd_ptr = cmd_ptr + strlen(commandline);
        *cmd_ptr = 0x00;
        
        __asm__ __volatile__(
            "ldr     r3, =0x16001080\n\t"
            "ldr     r2, [r3]\n\t"
            "ldr     r4, =(0x3 << 30)\n\t"
            "orr     r2, r2, r4\n\t"
            "str     r2, [r3]\n\t"
            "ldr     r3, =0x16001000\n\t"
            "mov     r2, #3\n\t"
            "str     r2, [r3]\n\t"
            "ldr     r3, =0x16002000\n\t"
            "mov     r2, #3\n\t"
            "str     r2, [r3]\n\t"
            "ldr     r3, =0x16002004\n\t"
            "mov     r2, #0xf8\n\t"
            "str     r2, [r3]\n\t"
            "ldr  r3, =0x0    \n\t"
            "ldr  r4, =0x0    \n\t"
            "ldr  r2, [%0]    \n\t"
            :: "r"(&cmd_addr): "r3","r4","r2"
        );
        __asm__ __volatile__("smc #0": );
    }
    else if (LinuxKernelMode == MODE_64BIT_OPTEE)
    {
        char *commandline = LINUX_KERNEL_COMMAND_LINE_OPTEE;
        strncpy(cmd_ptr,commandline,strlen(commandline));
        cmd_ptr = cmd_ptr + strlen(commandline);
        *cmd_ptr = 0x00;
        __asm__ __volatile__(
            "ldr  r0, =0x8400000F\n\t"
            "ldr  r3, =0x0  \n\t"
            "ldr  r4, =0x0  \n\t"
            "ldr  r2, [%0]  \n\t"
            "smc #0                 \n\t"
            ::"r"(&cmd_addr)
            : "r0","r3","r4","r2"
       );
    }
    else
    {
        printf("Warning - LinuxKernelMode[%d] is not support\n", LinuxKernelMode);
    }
}

static MS_BOOL _Load_Linux_Image_USB(void)
{
    MS_S32 s32NonCachedPoolID = INVALID_POOL_ID;
    char *filename = "/Image.bin";
    char *pMountPath = "/root/sda1";
    char *fullpath;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32NonCachedPoolID);

    fullpath = MsOS_AllocateMemory(strlen(pMountPath) + strlen(filename) + 1, s32NonCachedPoolID);
    memset(fullpath, 0, strlen(pMountPath) + strlen(filename) + 1);
    if(!fullpath)
    {
        printf("[%s][%d] allocate memory failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    strncpy(fullpath, pMountPath, strlen(pMountPath));
    strncat(fullpath, filename, strlen(filename));

    if(strlen(fullpath) == 0)
    {
        printf("[%s][%d] string concat failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    FILE* readFile = MsFS_Fopen(fullpath, "rb");
    if(!readFile)
    {
        printf("[%s][%d] open file failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if (LinuxKernelMode == MODE_32BIT)
    {
        MsFS_Fread((void *)LINUX_32BIT_KERNEL_ENTRY, (size_t)LINUX_KERNEL_SIZE32, 1, readFile);
    }
    else
    {
        MsFS_Fread((void *)LINUX_64BIT_KERNEL_ENTRY, (size_t)LINUX_KERNEL_SIZE64, 1, readFile);
    }

    MsFS_Fclose(readFile);

    return TRUE;

}

static MS_BOOL _Load_Linux_DTB_USB(void)
{
    MS_S32 s32NonCachedPoolID = INVALID_POOL_ID;
    char *filename = "/ddi.dtb";
    char *pMountPath = "/root/sda1";
    char *fullpath;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE, &s32NonCachedPoolID);

    fullpath = MsOS_AllocateMemory(strlen(pMountPath) + strlen(filename) + 1, s32NonCachedPoolID);
    memset(fullpath, 0, strlen(pMountPath) + strlen(filename) + 1);
    if(!fullpath)
    {
        printf("[%s][%d] allocate memory failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    strncpy(fullpath, pMountPath, strlen(pMountPath));
    strncat(fullpath, filename, strlen(filename));

    if(strlen(fullpath) == 0)
    {
        printf("[%s][%d] string concat failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    FILE* readFile = MsFS_Fopen(fullpath, "rb");
    if(!readFile)
    {
        printf("[%s][%d] open file failed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    MsFS_Fread((void *)LINUX_DTB_ADR, (size_t)LINUX_DTB_SIZE, 1, readFile);

    MsFS_Fclose(readFile);

    return TRUE;

}


extern MS_U32 __none_cache_la_addr;
extern MS_U32 __none_cache_va_start;
extern MS_U32 __none_cache_va_end;
extern MS_U32 __none_cache_stack_end;

static MS_BOOL _eCos_Boot_Linux(void)
{
    MS_BOOL bRet = TRUE;
    MS_U32 none_cache_code_la_addr = (MS_U32)&__none_cache_la_addr;
    MS_U32 none_cache_code_va_addr = (MS_U32)&__none_cache_va_start;
    MS_U32 none_cache_code_va_end = (MS_U32)&__none_cache_va_end;
    MS_U32 none_cache_code_size = none_cache_code_va_end - none_cache_code_va_addr;

    printf("Load Linux Image from USB\n");
    bRet = _Load_Linux_Image_USB();
    if (bRet != TRUE)
    {
        printf("Load Linux Image Fail\n");
        return FALSE;
    }
    printf("Load Linux Image Finish\n");

    if ((LinuxKernelMode == MODE_64BIT) || (LinuxKernelMode == MODE_64BIT_OPTEE))
    {
        printf("Load Linux DTB from USB\n");
        bRet = _Load_Linux_DTB_USB();
        if (bRet != TRUE)
        {
            printf("Load Linux DTB Fail\n");
            return FALSE;
        }
        printf("Load Linux DTB Finish\n");
    }

    // Copy boot linux demo from logical address to virtual address, also mean cache memory to none-cache memory
    memcpy((void *)none_cache_code_va_addr, (void *)none_cache_code_la_addr, none_cache_code_size);

    // disable interrupt and jump to none-cache code entry
    __asm__ (
        "mrs r0, cpsr\n"
        "orr r0, r0, #0x80\n"
        "msr cpsr_c, r0\n"
        "mov sp, %1\n"
        "mov pc, %0\n"
        ::"r"(none_cache_code_va_addr), "r"(&__none_cache_stack_end):"r0", "r1","r13"
    );

    return bRet;
}

MS_BOOL Demo_Boot_Linux32(void)
{
    MS_BOOL bRet = TRUE;

    LinuxKernelMode = MODE_32BIT;

    bRet = _eCos_Boot_Linux();

    return bRet;
}

MS_BOOL Demo_Boot_Linux64(void)
{
    MS_BOOL bRet = TRUE;

    LinuxKernelMode = MODE_64BIT;

    bRet = _eCos_Boot_Linux();

    return bRet;
}
MS_BOOL Demo_Boot_Linux64_OPTEE(void)
{
    MS_BOOL bRet = TRUE;
    LinuxKernelMode = MODE_64BIT_OPTEE;
    bRet = _eCos_Boot_Linux();
    return bRet;
}
#endif

