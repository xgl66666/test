################################################################################
#
# Copyright (c) 2006-2007 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# (¡§MStar Confidential Information¡¨) by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#
################################################################################



#include "boot.inc"
#include "macro.inc"
#include "drv_uart.inc"
#include "drv_cache.inc"
#include "drv_pl310.inc"
@#include "boot1.inc"

#[*] Copy AP from ROM to DRAM
@-------------------------------------------------
@Macro: BOOTRAM_LoadAPRAM
@-------------------------------------------------
.macro BOOTRAM_LoadAPRAM
    LDR     r10, =0x14020000
    LDR     r11, =0x20100000
    LDR     r12, =0x20200000
    LDR     r9,  =0x20100000
    SUB     r12, r12, r9
    MEMCPY32
.endm

#-------------------------------------------------------------------------------------------------
# BOOTRAM_Entry:
# @param  None
# @return None
# @note   Entry point of Boot code
#-------------------------------------------------------------------------------------------------
    .globl  BOOTRAM_Entry
BOOTRAM_Entry:
    PUTK '_'

  .if(EMMC_BOOT == 0)
    @clear bss
    ldr     r10, =_ld_bootbss_start
    ldr     r11, =_ld_bootbss_end
    mov     r12, #0
    MEMSET32
  .endif

    # Initialize system stack pointer for unzip subroutine
    ldr     r13, =ADDR_STACK_POINTER

    .if(IS_FPGA == 1) 
    PUTK    'F'
    PUTK    'P'
    PUTK    'G'
    PUTK    'A'       

    .endif
    
    .if(EMMC_BOOT == 0)
    bl      DoMSDecompression 
    PUTK    '_'
    MEMORY_FLUSH
    .endif

    @RREG  r0,0x1f006050  @read timer0 to r0
    @RREG  r1,0x1f006054  @read timer0 to r1
    WREG  0x1f006040, 0x0000 @disable timer

BOOT_POINT:
    PUTK 'D'
    # UBoot binary entry
    .if (CONFIG_NON_OS==1)
    ldr     r0, =0x14020000
    .else
    ldr     r0, =0x14030000
    .endif

    ldr     pc, [r0,#16]

    PUTK 'b'

    .globl  core1_loop
core1_loop:
    @WREG 0x1f2067bc, 0x1122  @debug
    WFI
    @WREG 0x1f2067bc, 0x5EE5  @debug
    RREG r0, 0x1f206744
    and  r0, #0x3
    cmp  r0, #0
    @WREG 0x1f2024E8, 0x5567  @debug
    bne core1_loop   @make sure start address of secondary is legal
    @WREG 0x1f2024E8, 0x3344  @debug
    RREG r0, 0x1f206740
    RREG r1, 0x1f206744
    add r2, r1,r0,lsl #16
    mov pc, r2

    #ldr   pc,=__secondary_a_core
    @ core1 will not run here
    loop:
    ldr pc, =loop

#-------------------------------------------------------------------------------------------------
# BOOTRAM_Pad
# @param  None
# @return None
# @note   Pad with zeros (Reserve SIZE_BOOTRAM_RESERVED Bytes for bootram)
#-------------------------------------------------------------------------------------------------
    @.org    (SIZE_BOOTRAM_RESERVED-4)
    @.word   0
