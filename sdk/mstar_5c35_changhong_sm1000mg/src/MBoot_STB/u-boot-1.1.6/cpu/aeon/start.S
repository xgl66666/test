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
        /* Invalidate I-Cache */
        l.addi  r3,r0,0
        l.addi  r4,r0,CFG_ICACHE_SIZE
    1:
        l.mtspr r0,r3,SPR_ICBIR
        l.sfne  r3,r4
        l.bf    1b
        l.addi  r3,r3,CFG_ICACHE_LINE_SIZE

        /* Enable I-Cache */
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
        # Invalidate DC
        l.addi  r3,r0,0
        l.ori   r4,r0,CFG_DCACHE_SIZE
    1:
        l.invalidate_line   0(r3),0  # invalidate line index=r3, way=0
        l.invalidate_line   0(r3),1  # invalidate line index=r3, way=1
        l.sfne  r3,r4
        l.bf    1b
        l.addi  r3,r3,CFG_DCACHE_LINE_SIZE

        # Enable D-Cache
        l.mfspr r3,r0,SPR_SR
        l.ori   r3,r3,SPR_SR_DCE
        l.mtspr r0,r3,SPR_SR
.endm

.org 0x100

_start:
    hal_icache_init
#if (CONFIG_ENABLE_DCACHE)
    hal_dcache_init
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
        l.j     1b                  // delay slot for R1
        l.addi  r3,r3,4
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

