@###############################################################################
@
@ Copyright (c) 2006-2009 MStar Semiconductor, Inc.
@ All rights reserved.
@
@ Unless otherwise stipulated in writing, any and all information contained
@ herein regardless in any format shall remain the sole proprietary of
@ MStar Semiconductor Inc. and be kept in strict confidence
@ ("MStar Confidential Information") by the recipient.
@ Any unauthorized act including without limitation unauthorized disclosure,
@ copying, use, reproduction, sale, distribution, modification, disassembling,
@ reverse engineering and compiling of the contents of MStar Confidential
@ Information is unlawful and strictly prohibited. MStar hereby reserves the
@ rights to any and all damages, losses, costs and expenses resulting therefrom.
@
@###############################################################################

#include "drv_arm.inc"

.globl core1_loop
core1_loop:
    ldr  r0,=(CONFIG_KERNEL_START_ADDRESS + 0x8000)
    mov r1,#0 
    str r1,[r0]
    ARM_INIT
wfi_loop:
    wfi
    ldr r0,= (CONFIG_KERNEL_START_ADDRESS + 0x8000)
    ldr r1,[r0]
    ldr r2,=0xbabe             @check magic number
    cmp r1,r2
    bne wfi_loop 
    ldr pc,[r0,#4]
loop:
    b .
 
