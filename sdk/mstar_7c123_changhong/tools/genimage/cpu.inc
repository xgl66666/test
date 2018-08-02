################################################################################
#
# Copyright (c) 2008-2009 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# ("MStar Confidential Information") by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#
################################################################################


#-------------------------------------------------------------------------------------------------
# Initialize Regs & Initialize Cache
#-------------------------------------------------------------------------------------------------

M_DebugCOUNTDM          = (0x1  << 25)

M_StatusCU              = (0xf  << 28)
M_StatusRP              = (0x1  << 27)
M_StatusRE              = (0x1  << 25)
M_StatusBEV             = (0x1  << 22)
M_StatusTS              = (0x1  << 21)
M_StatusSR              = (0x1  << 20)
M_StatusNMI             = (0x1  << 19)
M_StatusIM7             = (0x1  << 15)
M_StatusIM2             = (0x1  << 10)
M_StatusIM              = (0xff << 8)
M_StatusUM              = (0x1  << 4)
M_StatusERL             = (0x1  << 2)
M_StatusEXL             = (0x1  << 1)
M_StatusIE              = (0x1  << 0)

M_CauseIV               = (0x1  << 23)
M_CauseWP               = (0x1  << 22)
M_CauseIP1              = (0x1  << 9)
M_CauseIP0              = (0x1  << 8)

M_ConfigK23             = (0x7  << 28)
M_ConfigKU              = (0x7  << 25)
M_ConfigMT              = (0x7  << 7)
M_ConfigK0              = (0x7  << 0)
K_CacheAttrCN           = (0x3  << 0)   #Cacheable, noncoherent, write-back, write allocate.
S_Config1IS             = 22
M_Config1IS             = (0x7  << S_Config1IS)
S_Config1IL             = 19
M_Config1IL             = (0x7  << S_Config1IL)
S_Config1IA             = 16
M_Config1IA             = (0x7  << S_Config1IA)
S_Config1DS             = 13
M_Config1DS             = (0x7  << S_Config1DS)
S_Config1DL             = 10
M_Config1DL             = (0x7  << S_Config1DL)
S_Config1DA             = 7
M_Config1DA             = (0x7  << S_Config1DA)

# R_zero                  0
# R_AT                    1
# R_v0                    2
# R_v1                    3
# R_a0                    4
# R_a1                    5
# R_a2                    6
# R_a3                    7
# R_t0                    8
# R_t1                    9
# R_t2                    10
# R_t3                    11
# R_t4                    12
# R_t5                    13
# R_t6                    14
# R_t7                    15
# R_s0                    16
# R_s1                    17
# R_s2                    18
# R_s3                    19
# R_s4                    20
# R_s5                    21
# R_s6                    22
# R_s7                    23
# R_t8                    24
# R_t9                    25
# R_repc                  25
# R_k0                    26
# R_k1                    27
# R_gp                    28
# R_sp                    29
# R_fp                    30
# R_s8                    30
# R_tid                   30
# R_ra                    31
# R_hi                    32                      /* Hi register */
# R_lo                    33                      /* Lo register */
#
# R_C0_Config             16
# R_C0_SelConfig0         0
# R_C0_SelConfig1         1
# R_C0_SelConfig2         2
#
# C0_TagHi                $29
# R_C0_TagHi              29
# R_C0_SelTagHi           0
# C0_ITagHi               $29,0
# R_C0_ITagHi             29
# R_C0_SelITagHi          0
# C0_DTagHi               $29,2
# R_C0_DTagHi             29
# R_C0_SelDTagHi          2


INITIAL_STATUS          = 0x10400000
INITIAL_CONFIG0         = 0x00000002    #Uncached

Idx_WB_IV_D             = 0x1
Idx_S_Tag_I             = 0x8
Idx_S_Tag_D             = 0x9

A_DSPRAM                = 0x08000000    # 0x88000000
A_KSEG0BASE             = 0x80000000


#-------------------------------------------------------------------------------------------------
# General register usage macro
#-------------------------------------------------------------------------------------------------
        .macro MFC0_OP  dst, src, sel   # (e.g.) R_zero, R_C0_ITagLo, R_C0_SelITagLo
        .word   (0x40000000 | ((\dst)<<16) | ((\src)<<11) | (\sel))
        .endm


        .macro MTC0_OP  src, dst, sel
        .word   (0x40800000 | ((\src)<<16) | ((\dst)<<11) | (\sel))
        .endm


        .macro  RBASE   reg
        lui     $8,     (\reg >> 16)
        .endm


#        .macro  WREG    reg, value
#        li      $9,     (\value)
#        sw      $9,     (\reg & 0xFFFF)($8)
#        .endm


        .macro  DBGREG  value
#        WREG    R_DEBUG, (\value)
        .endm


        .macro  DELAYMS ms              # ms < 0x7FFFFFFF/CLK*2*1000, up to 1900ms for 216Mhz
        li      $26,    (\ms * (CPU_CLK_FREQ/2/1000)) #ticks
        mtc0    $0,     $9              #tick1 = 0
1:
        mfc0    $10,    $9              #tick2
        blt     $10,    $26, 1b
        nop
        .endm


        .macro  DELAYUS us              # ms < 0x7FFFFFFF/CLK*2*1000000, up to 19000000us for 216Mhz
        li      $26,    (\us * (CPU_CLK_FREQ/2/1000000)) #ticks
        mtc0    $0,     $9              #tick1 = 0
1:
        mfc0    $10,    $9              #tick2
        blt     $10,    $26, 1b
        nop
        .endm

        .macro  DELAYUS_12M us              # us < 0x7FFFFFFF/CLK*2*1000000, up to 19000000us
        li      $26,    (\us * (12000000/2/1000000)) #ticks
        mtc0    $0,     $9              #tick1 = 0
1:
        mfc0    $10,    $9              #tick2
        blt     $10,    $26, 1b
        nop
        .endm

#-------------------------------------------------------------------------------------------------
# CPU_Init
# @param  None
# @return None
# @note   Init CPU Registers
#-------------------------------------------------------------------------------------------------
        .macro  CPU_Init
#CPU_Init:

        # INIT DEBUG
        mfc0    $8,     $23
        and     $8,     (~M_DebugCOUNTDM)
        mtc0    $8,     $23

        # CLEAR COUNT
        mtc0    $0,     $9

        # INIT STATUS
# @FIXME: the below instructions will make T3 hang? Why?
#        li      $8,     INITIAL_STATUS
#        mtc0    $8,     $12

        # INIT CAUSE
        mtc0    $0,     $13

        # INIT CONFIG
        li      $8,     INITIAL_CONFIG0
        mtc0    $8,     $16

        .endm


#-------------------------------------------------------------------------------------------------
# CPU_InitIC
# @param  None
# @return None
# @note   Invalide I-cache
#-------------------------------------------------------------------------------------------------
        .macro  CPU_InitIC
#CPU_InitIC:

        # Clear TagLo & TagHi
        mtc0    $0,     $28, 0                              # TagLo / ITagLo
        mtc0    $0,     $28, 2                              # DTagLo
        mtc0    $0,     $29, 0                              # TagHi / ITagHi
        mtc0    $0,     $29, 2                              # DTagHi

        # Config1
        mfc0    $8,     $16, 1

        ### INIT I-CACHE ###

        # Line Size $10
        li      $9,     M_Config1IL
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1IL
        beqz    $9,     2f                                  # no Icache
        nop
        li      $10,    2
        sll     $10,    $10, $9

        # Sets per way $11
        li      $9,     M_Config1IS
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1IS
        li      $11,    64
        sll     $11,    $11, $9

        # Associativity $12
        li      $9,     M_Config1IA
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1IA
        add     $12,    $9, 1

        li      $14,    A_KSEG0BASE
        move    $15,    $11

1:
        # Invalidate each tag entry
        cache   Idx_S_Tag_I, 0($14)
        add     $14,    $10
        sub     $15,    1
        bnez    $15,    1b
        nop
        move    $15,    $11
        sub     $12,    1
        bnez    $12,    1b
        nop

2:
        .endm


#-------------------------------------------------------------------------------------------------
# CPU_InitDC
# @param  None
# @return None
# @note   Invalidate D-cache
#-------------------------------------------------------------------------------------------------
        .macro  CPU_InitDC
#CPU_InitDC:

        # Clear TagLo & TagHi
        mtc0    $0,     $28, 0                              # TagLo / ITagLo
        mtc0    $0,     $28, 2                              # DTagLo
        mtc0    $0,     $29, 0                              # TagHi / ITagHi
        mtc0    $0,     $29, 2                              # DTagHi

        # Config1
        mfc0    $8,     $16, 1

        ### INIT D-CACHE ###

        # Line Size $10
        li      $9,     M_Config1DL
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1DL
        beqz    $9,     2f                                  # no Dcache
        nop
        li      $10,    2
        sll     $10,    $10, $9

        # Sets per way $11
        li      $9,     M_Config1DS
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1DS
        li      $11,    64
        sll     $11,    $11, $9

        # Associativity $12
        li      $9,     M_Config1DA
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1DA
        add     $12,    $9, 1

        li      $14,    A_KSEG0BASE
        move    $15,    $11

1:
        # Invalidate each tag entry
        cache   Idx_S_Tag_D, 0($14)
        add     $14,    $10
        sub     $15,    1
        bnez    $15,    1b
        nop
        move    $15,    $11
        sub     $12,    1
        bnez    $12,    1b
        nop

2:
        .endm


#-------------------------------------------------------------------------------------------------
# CPU_FlushDC                                               # ( $2 should not be touch )
# @param  None
# @return None
# @note   Flush D-cache
#-------------------------------------------------------------------------------------------------
        .macro  CPU_FlushDC
#CPU_FlushDC:

        # Config1
        mfc0    $8,     $16, 1

        ### FLUSH D-CACHE ###

        # Line Size $10
        li      $9,     M_Config1DL
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1DL
        beqz    $9,     2f                                  # no Dcache
        nop
        li      $10,    2
        sll     $10,    $10, $9

        # Sets per way $11
        li      $9,     M_Config1DS
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1DS
        li      $11,    64
        sll     $11,    $11, $9

        # Associativity $12
        li      $9,     M_Config1DA
        and     $9,     $8, $9
        srl     $9,     $9, S_Config1DA
        add     $12,    $9, 1

        li      $14,    A_KSEG0BASE
        move    $15,    $11

1:
        # Invalidate each tag entry
        cache   Idx_WB_IV_D, 0($14)
        add     $14,    $10
        sub     $15,    1
        bnez    $15,    1b
        nop
        move    $15,    $11
        sub     $12,    1
        bnez    $12,    1b
        nop

2:
        .endm


#-------------------------------------------------------------------------------------------------
# CPU_InitDSPRAM                                            # ( $2 should not be touch )
# @param  None
# @return None
# @note   Init Data ScratchPad RAM
#-------------------------------------------------------------------------------------------------
        .macro CPU_InitDSPRAM
#CPU_InitDSPRAM:

        li      $17,    0x10000000                          # enable write to spram
        mtc0    $17,    $26

        li      $16,    0xFFFFFFFF
#        ehb
        mtc0    $16,    $28
#        ehb
        li      $17,    0x80000000
        cache   Idx_S_Tag_D, 0($17)


        li      $16,    A_DSPRAM                            # DSPRAM base offset 0x04000000
#        ehb
        mtc0    $16,    $28
#        ehb
        cache   Idx_S_Tag_D, 0x10($17)

        mtc0    $0,     $28
        mtc0    $0,     $29
        mtc0    $0,     $26

        li      $15,    0x80000000
        or      $16,    $16, $17                            # DSPRAM base 0x84000000

        li      $15,    0x00001000
1:
        sw      $0,     0($16)
        add     $16,    4
        sub     $15,    4
        bnez    $15,    1b
        nop

        .endm


#-------------------------------------------------------------------------------------------------
# CPU_DisableDSPRAM                                         # ( $2 should not be touch )
# @param  None
# @return None
# @note   Disable ScratchPad RAM
#-------------------------------------------------------------------------------------------------
        .macro CPU_DisableDSPRAM
#CPU_DisableDSPRAM:

        li      $17,    0x10000000                          # enable write to spram
        mtc0    $17,    $26

#        ehb
        mtc0    $0,    $28
#        ehb
        li      $17,    0x80000000
        cache   Idx_S_Tag_D, 0($17)


#        ehb
        mtc0    $0,    $28
#        ehb
        cache   Idx_S_Tag_D, 0x10($17)

        mtc0    $0,     $28
        mtc0    $0,     $29
        mtc0    $0,     $26

        li      $15,    0x80000000
        or      $16,    $16, $17                            # DSPRAM base 0x84000000

        .endm


#-------------------------------------------------------------------------------------------------
# CPU_EnableCache
# @param  None
# @return None
# @note   Enable Cache
#-------------------------------------------------------------------------------------------------
        .macro CPU_EnableCache
#CPU_EnableCache:

        # Enable cache
        mfc0    $8,     $16
        and     $8,     (~M_ConfigK0)
        li      $8,     (K_CacheAttrCN)
        mtc0    $8,     $16
        .endm


#-------------------------------------------------------------------------------------------------
# CPU_DisableCache
# @param  None
# @return None
# @note   Disable Cache and Scratchpad
#-------------------------------------------------------------------------------------------------
        .macro CPU_DisableCache
#CPU_DisableCache:

        # Disble cache
        li      $8,     INITIAL_CONFIG0
#        ehb
        mtc0    $8,     $16
#        ehb
        .endm

