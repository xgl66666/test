/*
 *  Startup Code for AEON CPU-core
 *
 *  Copyright (c) 2009 Jae Hoon, Kim <jhkim@libertron.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <version.h>
#include "../../include/asm-aeon/spr_defs.h"


_TEXT_BASE:
	.word	TEXT_BASE

.globl _start
	.word	_start

.globl _aeonboot_start
_aeonboot_start:
	.word _start

.globl _bss_start
_bss_start:
	.word 	__bss_start

.globl _bss_end
_bss_end:
	.word 	_end


.macro  hal_icache_init
        /* Disable I-Cache */
        l.mfspr r3,r0,SPR_SR
        l.addi  r4,r0,-1
        l.xori  r4,r4,SPR_SR_ICE
        l.and   r4,r3,r4
        l.mtspr r0,r4,SPR_SR

        /* Invalidate I-Cache */
        l.addi  r3,r0,0
        l.addi  r4,r0,CFG_ICACHE_SIZE
1:
        l.ori   r5,r0,SPR_ICBIR
        l.mtspr r5,r3,0
        l.sfleu r3,r4
        l.addi  r3,r3,CFG_ICACHE_LINE_SIZE
        l.bf    1b

        /* Enable I-Cache */
        // There is no "un-cached instruction area" in OpenRISC, therefore turn it off
        l.mfspr r3,r0,SPR_SR
        l.ori   r3,r3,SPR_SR_ICE
        l.mtspr r0,r3,SPR_SR

        /* Flush instructions out of instruction buffer */
        l.nop
        l.nop
        l.nop
        l.nop
        l.nop
.endm

.macro  hal_dcache_init
        /* Disable D-Cache */
        l.mfspr r3,r0,SPR_SR
        l.addi  r4,r0,-1
        l.xori  r4,r4,SPR_SR_DCE
        l.and   r3,r3,r4
        l.mtspr r0,r3,SPR_SR

        /* Invalidate D-Cache */
        l.addi  r3,r0,0
        l.ori   r4,r0,(CFG_DCACHE_SIZE)         //l.ori   r4,r0,(CFG_DCACHE_SIZE/CFG_DCACHE_WAYS)

#if 1
1:
        l.invalidate_line   0(r3),0
        l.invalidate_line   0(r3),1
        l.invalidate_line   0(r3),2
        l.invalidate_line   0(r3),3
        l.addi  r3,r3,CFG_DCACHE_LINE_SIZE
        l.sfleu r3,r4
        l.bf    1b
#else   // can not work while using SPR_DCBIR to invalidate, need to check after 2009/12/25
1:
        l.ori   r5,r0,SPR_DCBIR
        l.mtspr r5,r3,0

        l.sfne  r3,r4
        l.bf    1b
        l.addi  r3,r3,CFG_DCACHE_LINE_SIZE
#endif

        /* Enable DC */
        l.mfspr r3,r0,SPR_SR
        l.ori   r3,r3,SPR_SR_DCE
        l.mtspr r0,r3,SPR_SR
        l.nop
        l.nop
        l.nop
        l.nop
        l.nop
.endm

.org 0x100

_start:
    #if (CONFIG_ENABLE_ICACHE)
        hal_icache_init             // Enable I-Cache
    #endif

    #if (CONFIG_ENABLE_DCACHE)
        hal_dcache_init             // Enable D-Cache
    #endif


_stack_setup:
    	l.movhi r1, hi(CFG_SDRAM_BASE + CFG_INIT_SP_OFFSET)
    	l.ori   r1, r1, lo(CFG_SDRAM_BASE + CFG_INIT_SP_OFFSET)

    	l.nop
    	l.nop
    	l.nop


_clear_bss:
        l.movhi r3,hi(__bss_start)
        l.ori   r3,r3,lo(__bss_start)
        l.movhi r4,hi(_end)
        l.ori   r4,r4,lo(_end)

_clbss_l:

1:      l.sfgeu r3,r4
        l.bf    2f
        l.nop
        l.sw    0(r3),r0
        l.addi  r3,r3,4
        l.j     1b                  // NOTE:!!! no delay slot for R2
2:


_jump_to_init:
    l.movhi r3,hi(_board_init)
    l.ori   r3,r3,lo(_board_init)
    l.jalr  r3
    l.nop

#if !defined(CONFIG_IDENT_STRING)
#define CONFIG_IDENT_STRING "Janus AeonR2 platform"
#endif
        .data
        .globl  _version_string_start

_version_string_start:
        .ascii U_BOOT_VERSION
        .ascii " (", __DATE__, " - ", __TIME__, ")"
        .ascii CONFIG_IDENT_STRING, "\0"

