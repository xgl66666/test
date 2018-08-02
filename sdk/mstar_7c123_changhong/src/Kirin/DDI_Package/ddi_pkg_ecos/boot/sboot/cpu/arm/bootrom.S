################################################################################
#
# Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
#include "chip.inc"
#include "cpu.inc"
@#include "drv_miu.inc"
#include "drv_uart.inc"
@#include "drv_clk.inc"
#include "drv_arm.inc"
#include "drv_misc.inc"

#[*] Copy bootram from ROM to DRAM
@-------------------------------------------------
@Macro: BOOTROM_LoadBootRAM
@-------------------------------------------------
.macro BOOTROM_LoadBootRAM
    LDR     r10, =_ld_bootram_ROM_start
    LDR     r11, =_ld_bootram_RAM_start
    LDR     r12, =_ld_bootram_RAM_end
    LDR     r9,  =_ld_bootram_RAM_start
    SUB     r12, r12, r9
    MEMCPY32
.endm

#-------------------------------------------------------------------------------------------------
# BOOTROM_CopyBin
# @param  None
# @return None
# @note   Copy LDR & APP from ROM to DRAM (DMA or CPU)
# @src(r10), dst(r11), len(r12) should be 32bit align
#-------------------------------------------------------------------------------------------------
.macro BOOTROM_CopyBin
    LDR     r10, =_ld_LDR_load_start
    LDR     r9, =_var_LDR_run_base
    LDR     r11, [r9]
    LDR     r9, =_var_LDR_size
    LDR     r12, [r9]
    MEMCPY32
.endm

#-------------------------------------------------------------------------------------------------
# BOOTROM_Entry:
# @param  None
# @return None
# @note   Entry point of Boot code
#-------------------------------------------------------------------------------------------------
    .globl  BOOTROM_Entry
BOOTROM_Entry:

    # Power on Sequence
    CHIP_PowerOnChip
    
    WREG_DBG  0x0005
    
    CHIP_InitDSPRAM
    
    WREG_DBG  0x0007
    
    CHIP_InitUART
    WREG_DBG  0x0009

BOOTROM_RAMLoader:
    # Initialize system stack pointer for unzip subroutine
    #ldr     r13, =ADDR_STACK_POINTER
    ldr     r13, =_ld_STACK_end

    bl  BOOTROM_C_InitMIU_ENTRY
   
    nop
    WREG_B    0x1F203104, 0x84      @switch L3 CLK to MIU2X

    drv_arm_exldr_exstr_enable
    #  drv_misc_riu32_bitmask
    drv_clk_L2_clk_ratio

    WREG_DBG  0x0006
    
    ARM_INIT   

    drv_arm_cache_init

    .extern _boot_flow_mode
    ldr     r0, =_boot_flow_mode
    ldr     r0, [r0]
    and     r0, r0 , #0xff

    cmp     r0, #1
    bne     _Exit
    
    BOOTROM_CopyBin
    nop

    bl    BOOTROM_C_GetHashTwoInfo
    nop

    # Clean intermediate data
    bl     _Exit
    nop

#-------------------------------------------------------------------------------------------------
# _Exit
# @param  None
# @return None
# @note   Clear intermediate data
#-------------------------------------------------------------------------------------------------
_Exit:
    # LDR stack pointer is trusted and set
    ldr     r9,     =_var_LDR_stack_end
    ldr     r13,    [r9]

    # Load HASH2 entry point to $2
    ldr     r9,     =_var_LDR_entry_point
    ldr     r2,     [r9]

    # Clear SPRAM and cache
    #ldr     BOOT_Clear
    nop

    PUTK 'J'
    PUTK '2'
    PUTK 'L'
    PUTK 'D'
    PUTK 'R'
    PUTK '\n'
    PUTK '\r'
    # jump to HASH2
    mov     pc,     r2
    nop

    loop:
    WREG_DBG  0xDEAD
    b     loop
@    ldr pc, =exception_vector

@__init_neon_vfp:
@    mrc p15, 0, r0, c1, c0, 2
@    orr r0, r0, #(0x3 <<20)
@    orr r0, r0, #(0x3 <<22)
@    mcr p15, 0, r0, c1, c0, 2
@    mov r0, #0
@    orr r0, r0, #(0x1 << 30)
@    FMXR FPEXC, r0
@    mov pc, lr

#ifdef CONFIG_MSTAR_VID_ENABLE
@//-------------------------------------------------------------------------------------------------
@// BootRom_InitVid
@// @param
@// @return None
@// @note
@ SAR4    SAR1      Voltage(VDDC/VDDC_CPU)
@   0          0          1.05(1.061)
@   0          1          1.10(1.113) -> FF
@   1          0          1.15(1.164) -> TT
@   1          1          1.20(1.216) -> SS
@//-------------------------------------------------------------------------------------------------
BootRom_InitVid:
    @dsable all pad in
    WREG    (MS_BUS_RIU_ADDR + (0x101EA0 << 1)), 0x0000

    @Set GPIO GPIO_VID_1(SAR4) = high and GPIO_VID_0(SAR1) = high , VDDC/VDDC_CPU =1.2V
    WREG    (MS_BUS_RIU_ADDR + (0x001422 << 1)), 0x0D0D  @reg_sar_aisel setting , SAR4 and SAR1 for GPIO , reg_oen_sar_gpio , SAR4 and SAR1 is GPIO_OUT
    WREG_B  (MS_BUS_RIU_ADDR + (0x001424 << 1)), 0x12    @reg_i_sar_gpio , SAR4 and SAR1 is high(1.2V)

    @Read eFuse(SIDD[9:0]) to set the GPIO level , SIDD[9:0] address = 0x00200e[5:0](MSB), 0x00200d[7:4](LSB)
    @  eFuse <10'b 125                         to set 1.20(1.216) Voltage(VDDC/VDDC_CPU)
    @  10'b 125 <= eFuse < 10'b 400     			 to set 1.15(1.164) Voltage(VDDC/VDDC_CPU)
    @  eFuse >= 10'b 400                       to set  1.10(1.113) Voltage(VDDC/VDDC_CPU)
    RREG    r0, (MS_BUS_RIU_ADDR + (0x00200E << 1))
    RREG    r1, (MS_BUS_RIU_ADDR + (0x00200C << 1))
    and     r0, r0, #0x003F
    mov     r0, r0, lsl #4
    and     r1, r1, #0xF000
    mov     r1, r1, lsr #12
    orr     r1, r1, r0

    ldr     r0, = 125
    cmp     r1,r0
    blt	    To_set120  			@  eFuse <10'b 125
    nop
    ldr	    r0, = 400
    cmp     r1,r0
    bge     To_set110       @  eFuse >= 10'b 400
    nop
To_set115:
    WREG_B  (MS_BUS_RIU_ADDR + (0x001424 << 1)), 0x10  @reg_i_sar_gpio
    b		    BootRom_InitVid_Exit
    nop
To_set120:
    WREG_B  (MS_BUS_RIU_ADDR + (0x001424 << 1)), 0x12  @reg_i_sar_gpio
    b 		  BootRom_InitVid_Exit
    nop
To_set110:
    WREG_B  (MS_BUS_RIU_ADDR + (0x001424 << 1)), 0x02  @reg_i_sar_gpio

BootRom_InitVid_Exit:
    bx          lr
#endif

#-------------------------------------------------------------------------------------------------
# Local variable
#-------------------------------------------------------------------------------------------------
        .bss                                                @ All data will be lost after BOOT_Clear
        .global _var_LDR_run_base
        .global _var_LDR_size
        .global _var_LDR_entry_point
        .global _var_LDR_stack_end
_var_LDR_run_base:
        .word   0xDEADBEEF
_var_LDR_size:
        .word   0xDEADBEEF
_var_LDR_entry_point:
        .word   0xDEADBEEF
_var_LDR_stack_end:
        .word   0xDEADBEEF        

