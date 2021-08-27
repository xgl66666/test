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

#ifndef _BOOT_H_
#define _BOOT_H_

//------------------------------------------------------------------------------------------------
// Type Definition and Structure
//------------------------------------------------------------------------------------------------

#ifndef NULL
#define NULL ((void *)0)
#endif

#define _SECTION_OTPCFG_            __attribute__((section(".otpcfg")))

// Chip Init Parameter
//[CAUTION] hardware reg spec can not over this
#define CHIPCFG_CMD_BASE            0xFE00
#define CHIPCFG_CMD_BASE_8BITS      0xFE01
#define CHIPCFG_CMD_WAIT            0xFF00
#define CHIPCFG_CMD_DBG             0xFFE0
#define CHIPCFG_CMD_END             0xFFF0

// Condition parameters
// Format: {16 bits, 16 bits}
// {CHIPCFG_CMD_IF, ADDR_LO},
// {CMP_VALUE, CMP_MASK},
//     {...}, // parameters
// {CHIPCFG_CMD_ENDIF, 0xFFFF},
// Action:
// if ((*(rbase << 16) | ADDR_LO) & CMP_MASK == CMP_VALUE) then
//    Process the parameters between CHIPCFG_CMD_IF and CHIPCFG_CMD_ENDIF
// endif
#define CHIPCFG_CMD_IF              0xFE10
#define CHIPCFG_CMD_ENDIF           0xFE13

#define _SECTION_HASH_0_            __attribute__((section(".hash0")))
#define _SECTION_SIG_0_             __attribute__((section(".sig0")))
#define _SECTION_HASH_1_            __attribute__((section(".hash1")))
#define _SECTION_SIG_1_             __attribute__((section(".sig1")))
#define _SECTION_HASH_2_            __attribute__((section(".hash2")))
#define _SECTION_SIG_2_             __attribute__((section(".sig2")))

#define _SECTION_LDR_ENTRY_         __attribute__((section(".ldr.entry")))
#define _SECTION_LDR_STACK_         __attribute__((section(".ldr.stack")))

// Type definition
typedef unsigned long               MS_U32;
typedef signed long                 MS_S32;
typedef unsigned short              MS_U16;
typedef signed short                MS_S16;
typedef unsigned char               MS_U8;
typedef signed char                 MS_S8;

typedef struct _Integer2048
{
    MS_U8                           Byte[256];
} Integer2048;

typedef struct _Integer32
{
    MS_U8                           Byte[4];
} Integer32;

typedef struct _OTP_Cfg
{
    // 0x1F28
    MS_U32 Bootcode_subversion:8;            // [7 :0 ]
    MS_U32 Bootcode_version:8;               // [15:8 ]
    MS_U32 Bootcode_project:8;               // [23:16]
    MS_U32 Bootcode_reserve:8;               // [31:24]
} OTP_Cfg;

//-------------------------------------------------------------------------------------------------
// Global Variable
//-------------------------------------------------------------------------------------------------

#define KSEG0_ADDR 0x80000000
#define KSEG1_ADDR 0xA0000000
#define DRAM_SIZE  0x10000000


#define ERROR_NO       0xE000
#define ERROR_MIU_BIST 0xE001

#if (ENABLE_CPU_288MHZ == 1)
#define CPU_CLK_FREQ                288000000           //[USER]
#elif (ENABLE_CPU_312MHZ == 1)
#define CPU_CLK_FREQ                312000000           //[USER]
#elif (ENABLE_CPU_324MHZ == 1)
#define CPU_CLK_FREQ                324000000           //[USER]
#elif (ENABLE_CPU_336MHZ == 1)
#define CPU_CLK_FREQ                336000000           //[USER]
#elif (ENABLE_CPU_396MHZ == 1)
#define CPU_CLK_FREQ                396000000           //[USER]
#elif (ENABLE_CPU_432MHZ == 1)
#define CPU_CLK_FREQ                432000000           //[USER]
#elif (ENABLE_CPU_456MHZ == 1)
#define CPU_CLK_FREQ                456000000           //[USER]
#elif (ENABLE_CPU_504MHZ == 1)
#define CPU_CLK_FREQ                504000000           //[USER]
#elif (ENABLE_CPU_552MHZ == 1)
#define CPU_CLK_FREQ                552000000           //[USER]
#elif (ENABLE_CPU_600MHZ == 1)
#define CPU_CLK_FREQ                600000000           //[USER]
#elif (ENABLE_CPU_648MHZ == 1)
#define CPU_CLK_FREQ                648000000           //[USER]
#elif (ENABLE_CPU_696MHZ == 1)
#define CPU_CLK_FREQ                696000000           //[USER]
#elif (ENABLE_CPU_720MHZ == 1)
#define CPU_CLK_FREQ                720000000           //[USER]
#elif (ENABLE_CPU_792MHZ == 1)
#define CPU_CLK_FREQ                792000000           //[USER]
#elif (ENABLE_CPU_840MHZ == 1)
#define CPU_CLK_FREQ                840000000           //[USER]
#elif (ENABLE_CPU_900MHZ == 1)
#define CPU_CLK_FREQ                900000000           //[USER]
#elif (ENABLE_CPU_984MHZ == 1)
#define CPU_CLK_FREQ                984000000           //[USER]
#elif (ENABLE_CPU_1008MHZ == 1)
#define CPU_CLK_FREQ                1008000000           //[USER]
#else
#define CPU_CLK_FREQ                12000000           //[USER]
#endif

//-------------------------------------------------------------------------------------------------
// Global Macro and Functions
//-------------------------------------------------------------------------------------------------
#define WREG_DBG(value, offset) (*(volatile MS_U16 *)(R_DEBUG + (offset) * 4) = (value))

#define WREG16(addr, value) (*(volatile MS_U16 *)(addr) = (value))
#define RREG16(addr) (*(volatile MS_U16 *)(addr))

#define WREG8(addr, value) (*(volatile MS_U8 *)(addr) = (value))
#define RREG8(addr) (*(volatile MS_U8 *)(addr))

#define _Fail(reason) \
    do { \
        WREG_DBG(reason, 0xf); \
        asm volatile ( \
            ".set push\n" \
            ".set noreorder\n" \
            "b .\n" \
            "nop\n" \
            ".set push" \
        ); \
    } while(0) \

void DelayMS(MS_U32 msec);

void _PUTC(int c);
void _PUTS(const char *s);

MS_U32 BOOTROM_C_InitParams(void *pParams);

#endif // _BOOT_H_
