#ifndef CYGONCE_HAL_VARIANT_INC
#define CYGONCE_HAL_VARIANT_INC
##=============================================================================
##
##	variant.inc
##
##	MIPS 32/64 family assembler header file
##
##=============================================================================
#####ECOSGPLCOPYRIGHTBEGIN####
## -------------------------------------------
## This file is part of eCos, the Embedded Configurable Operating System.
## Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
## Copyright (C) 2005 eCosCentric Limited
##
## eCos is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free
## Software Foundation; either version 2 or (at your option) any later version.
##
## eCos is distributed in the hope that it will be useful, but WITHOUT ANY
## WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
## for more details.
##
## You should have received a copy of the GNU General Public License along
## with eCos; if not, write to the Free Software Foundation, Inc.,
## 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
##
## As a special exception, if other files instantiate templates or use macros
## or inline functions from this file, or you compile this file and link it
## with other works to produce a work based on this file, this file does not
## by itself cause the resulting work to be covered by the GNU General Public
## License. However the source code for this file must still be made available
## in accordance with section (3) of the GNU General Public License.
##
## This exception does not invalidate any other reasons why a work based on
## this file might be covered by the GNU General Public License.
##
## Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
## at http://sources.redhat.com/ecos/ecos-license/
## -------------------------------------------
#####ECOSGPLCOPYRIGHTEND####
##=============================================================================
#######DESCRIPTIONBEGIN####
##
## Author(s): 	dmoseley
## Contributors:	dmoseley
## Date:	2000-06-07
## Purpose:	MIPS32 family definitions.
## Description:	This file contains various definitions and macros that are
##              useful for writing assembly code for the MIPS32 CPU family.
## Usage:
##		#include <cyg/hal/variant.inc>
##		...
##
##
######DESCRIPTIONEND####
##
##=============================================================================

#include <pkgconf/hal.h>

#include <cyg/hal/mips.inc>

#include <cyg/hal/platform.inc>

#include <cyg/hal/mstar_reg.h>

#define CYGARC_HAL_COMMON_EXPORT_CPU_MACROS
#include <cyg/hal/mips-regs.h>
#include <cyg/hal/hal_arch.h>

##-----------------------------------------------------------------------------
## Define CPU variant for architecture HAL.

#define	CYG_HAL_MIPS_MIPS32

#------------------------------------------------------------------------------
# Cache macros.

#ifndef CYGPKG_HAL_MIPS_CACHE_DEFINED

        .macro  hal_cache_init

	# Setup a temporary stack pointer for running C code.
	la	a0,__interrupt_stack
	move	sp,a0
        CYGARC_ADDRESS_REG_UNCACHED(sp)

        # Read the CONFIG1 register into a0
        mfc0	a0, $16, 1
	nop
	nop
	nop

	# Jump to C-code to initialize caches (uncached)
	lar	k0, hal_c_cache_init

	#kevinhuang10262006, comment
	#CYGARC_ADDRESS_REG_UNCACHED(k0)

	jalr	k0
	nop
        .endm

#define CYGPKG_HAL_MIPS_CACHE_DEFINED

#endif

#------------------------------------------------------------------------------
# Monitor initialization.

#ifndef CYGPKG_HAL_MIPS_MON_DEFINED

#if	defined(CYG_HAL_STARTUP_ROM) ||			\
	(	defined(CYG_HAL_STARTUP_RAM) &&		\
		!defined(CYGSEM_HAL_USE_ROM_MONITOR))
	# If we are starting up from ROM, or we are starting in
	# RAM and NOT using a ROM monitor, initialize the VSR table.

	.macro	hal_mon_init  #kevinhuang10262006, need ???
        # Set default exception VSR for all vectors
        ori     a0,zero,16  # CYGNUM_HAL_VSR_COUNT
	la	a1,__default_exception_vsr
	la	a2,hal_vsr_table
1:      sw      a1,0(a2)
        addi    a2,a2,4
        addi    a0,a0,-1
        bne     a0,zero,1b
         nop

        # Now set special VSRs
       	la	a0,hal_vsr_table
        # Set interrupt VSR
	la	a1,__default_interrupt_vsr
        sw      a1,0*4(a0)  # CYGNUM_HAL_VECTOR_INTERRUPT
        # Add special handler on breakpoint vector to allow GDB and
        # GCC to both use 'break' without conflicts.
	la	a1,__break_vsr_springboard
	sw	a1,9*4(a0)  # CYGNUM_HAL_VECTOR_BREAKPOINT
        # Set exception handler on special vectors
        # FIXME: Should use proper definitions
	la	a1,__default_exception_vsr
	sw	a1,32*4(a0)             # debug
	sw	a1,33*4(a0)             # utlb
	sw	a1,34*4(a0)             # nmi
	.endm

#elif defined(CYG_HAL_STARTUP_RAM) && defined(CYGSEM_HAL_USE_ROM_MONITOR)

	# Initialize the VSR table entries
	# We only take control of the interrupt vector,
	# the rest are left to the ROM for now...

	.macro	hal_mon_init
	la	a0,__default_interrupt_vsr
	la	a3,hal_vsr_table
	sw	a0,0(a3)
	.endm

#else

	.macro	hal_mon_init
	.endm

#endif


#define CYGPKG_HAL_MIPS_MON_DEFINED

#endif

#------------------------------------------------------------------------------
# Decide whether the VSR table is defined externally, or is to be defined
# here.

#if defined(CYGPKG_HAL_MIPS_SIM) ||        \
    ( defined(CYGPKG_HAL_MIPS_ATLAS) &&    \
      defined(CYG_HAL_STARTUP_RAM) &&      \
      !defined(CYGSEM_HAL_USE_ROM_MONITOR) \
    )

## VSR table defined in linker script

#else

#define CYG_HAL_MIPS_VSR_TABLE_DEFINED

#endif


#------------------------------------------------------------------------------
# MStar L2 cache operation
# 

#ifndef CYGPKG_HAL_MIPS_MSTAR_L2_CACHE_DEFINED

	.macro  hal_mstar_rreg reg
	li	t1,\reg
	lh	t0,0(t1)
	.endm

	.macro  hal_mstar_wreg reg,val
	li	t1,\reg
	ori	t0,zero,\val
	sh	t0,0(t1)
	.endm

	.macro  hal_mstar_cache_op_fire reg,mode
	li	t1,\reg
	ori	t0,zero,\mode
	ori	t2,zero,REG_CACHE_OP_FIRE
	add	t0,t0,t2
	sh	t0,0(t1)
	.endm


	##### Flush and invalid L2 cache #####
	.macro  hal_mstar_L2_cache_flush_invalid
	# Check if L2 cache is enabled
	hal_mstar_rreg REG_PREFTECH_L2_CONFIG
	ori	t1,zero,PREFTECH_L2_CONFIG_DISABLE
	and	t0,t0,t1
	bne     t0,zero,1f
	nop

	# Check if L2 cache is initialized
	hal_mstar_rreg REG_PREFTECH_L2_CONTROL
	ori	t1,zero,PREFTECH_L2_CONTROL_MODE0DONE
	and	t0,t0,t1
	beq     t0,zero,1f
	nop

	# Start to send command to flush and invalid L2 cache
	hal_mstar_rreg R_DEBUG # Dummy read
	hal_mstar_wreg REG_PREFTECH_L2_CONTROL,L2_OP_MODE_FLUSH_ALL_AND_INVALID_CACHE
	hal_mstar_rreg R_DEBUG # Dummy read
	hal_mstar_cache_op_fire REG_PREFTECH_L2_CONTROL,L2_OP_MODE_FLUSH_ALL_AND_INVALID_CACHE
	
2:
	hal_mstar_rreg R_DEBUG # Dummy read
	hal_mstar_rreg REG_PREFTECH_L2_CONTROL
	and	t0,t0,PREFTECH_L2_CONTROL_MODE34DONE
	beq     t0,zero,2b
	nop
	li	t1,REG_PREFTECH_L2_CONTROL
	sh	zero,0(t1)
	li	t1,REG_PREFTECH_L2_CONFIG
	lh	t0,0(t1)
	and	t0,t0,~0x2000   #HW limit: cannot not close uca buffer
        or	t0,t0,0x2
	sh      t0,0(t1)

1:
	sync
	.endm
	

	##### Invalid L2 cache #####
	.macro  hal_mstar_L2_cache_invalid
	# Check if L2 cache is enabled
	hal_mstar_rreg REG_PREFTECH_L2_CONFIG
	ori	t1,zero,PREFTECH_L2_CONFIG_DISABLE
	and	t0,t0,t1
	bne     t0,zero,1f
	nop

	# Check if L2 cache is initialized
	hal_mstar_rreg REG_PREFTECH_L2_CONTROL
	ori	t1,zero,PREFTECH_L2_CONTROL_MODE0DONE
	and	t0,t0,t1
	beq     t0,zero,1f
	nop


	# Start to send command to invalid L2 cache
	hal_mstar_rreg R_DEBUG # Dummy read
	hal_mstar_wreg REG_PREFTECH_L2_CONTROL,L2_OP_MODE_INITIAL_ALL_INVALID
	hal_mstar_rreg R_DEBUG # Dummy read
	hal_mstar_cache_op_fire REG_PREFTECH_L2_CONTROL,L2_OP_MODE_INITIAL_ALL_INVALID
	
2:
	hal_mstar_rreg R_DEBUG # Dummy read
	hal_mstar_rreg REG_PREFTECH_L2_CONTROL
	and	t0,t0,PREFTECH_L2_CONTROL_MODE0DONE
	beq     t0,zero,2b
	nop
	li	t1,REG_PREFTECH_L2_CONTROL
	sh	zero,0(t1)

1:
	sync
	.endm

#define CYGPKG_HAL_MIPS_MSTAR_L2_CACHE_DEFINED

#endif

#------------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_VARIANT_INC
# end of variant.inc
