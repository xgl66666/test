/* spr_defs.h -- Defines BA architecture specific special-purpose Registers
   Copyright (C) 1999 Damjan Lampret, lampret@opencores.org

This file is part of BA Architectural Simulator.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

/* This file is also used by microkernel test bench. Among
others it is also used in assembly file(s). */

/* Definition of special-purpose Registers (SPRs) */

#ifndef _SPR_DEFS_H
#define _SPR_DEFS_H

#define MAX_GRPS (32)
#define MAX_SPRS_PER_GRP_BITS (11)
#define MAX_SPRS_PER_GRP (1 << MAX_SPRS_PER_GRP_BITS)
#define MAX_SPRS (0x10000)

/* Base addresses for the groups */
#define SPRGROUP_SYS	(0<< MAX_SPRS_PER_GRP_BITS)	 /* System Control and Status Registers*/
#define SPRGROUP_DMMU	(1<< MAX_SPRS_PER_GRP_BITS)	 /* Data MMU */
#define SPRGROUP_IMMU	(2<< MAX_SPRS_PER_GRP_BITS)	 /* Instruction MMU */
#define SPRGROUP_DC	(3<< MAX_SPRS_PER_GRP_BITS)	 /* Data Cache */
#define SPRGROUP_IC	(4<< MAX_SPRS_PER_GRP_BITS)	 /* Instruction Cache */
#define SPRGROUP_MAC	(5<< MAX_SPRS_PER_GRP_BITS)	 /* MAC Unit */
#define SPRGROUP_D	(6<< MAX_SPRS_PER_GRP_BITS)	 /* Debug Unit */
#define SPRGROUP_PC	(7<< MAX_SPRS_PER_GRP_BITS)	 /* Performance Counters Unit */
#define SPRGROUP_PM	(8<< MAX_SPRS_PER_GRP_BITS)	 /* Power Management */
#define SPRGROUP_PIC	(9<< MAX_SPRS_PER_GRP_BITS)	 /* PIC */
#define SPRGROUP_TT	(10<< MAX_SPRS_PER_GRP_BITS)	 /* Tick Timer */

/* System control and status group */
#define SPR_VR		(SPRGROUP_SYS + 0)	/* Version Register */
#define SPR_UPR		(SPRGROUP_SYS + 1)	/* Unit Present Register */
#define SPR_CPUCFGR	(SPRGROUP_SYS + 2)	/* CPU config Register */
#define SPR_DMMUCFGR	(SPRGROUP_SYS + 3)	/* Data MMU config Register */
#define SPR_IMMUCFGR	(SPRGROUP_SYS + 4)	/* Instruction MMU config Register */
#define SPR_DCCFGR	(SPRGROUP_SYS + 5)	/* Data Cache configuration Register */
#define SPR_ICCFGR	(SPRGROUP_SYS + 6)	/* Instruction Cache configuration Register */
#define SPR_DCFGR	(SPRGROUP_SYS + 7)	/* Debug configuration Register */
#define SPR_PCCFGR	(SPRGROUP_SYS + 8)	/* Performance Counters configuration Register */
#define SPR_NPC         (SPRGROUP_SYS + 16) 	/* CHECK */
#define SPR_SR		(SPRGROUP_SYS + 17)	/* Supervisor Register */
#define SPR_PPC         (SPRGROUP_SYS + 18) 	/* CHECK */
#define SPR_EPCR_BASE	(SPRGROUP_SYS + 32)	/* Exception PC Register */
#define SPR_EPCR_LAST	(SPRGROUP_SYS + 47)	/* Last EPCR */
#define SPR_EEAR_BASE	(SPRGROUP_SYS + 48)	/* Exception EA Register */
#define SPR_EEAR_LAST	(SPRGROUP_SYS + 63)	/* Last EEAR */
#define SPR_ESR_BASE	(SPRGROUP_SYS + 64)	/* Exception SR Register */
#define SPR_ESR_LAST	(SPRGROUP_SYS + 79)	/* Last ESR */
#define SPR_EPOS	(SPRGROUP_SYS + 80)	/* Exception Position Offset Register */
#define SPR_GPR(x)	(SPRGROUP_SYS + 0x400 + (x)) /* GPRs mapped to SPR space - Software should NOT use these! */

/* Data MMU group */
#define SPR_DMMUCR	(SPRGROUP_DMMU + 0)	/* Control Register implemented */
#define SPR_DTLBEIR     (SPRGROUP_DMMU + 2)     /* Data TLB Entry invalidate Register */
#define SPR_DTLBMR      (SPRGROUP_DMMU + 4)     /* Data TLB Match Register */
#define SPR_DTLBTR      (SPRGROUP_DMMU + 6)     /* Data TLB Translate Register */
#define SPR_DTLBMR_BASE(WAY)	(SPRGROUP_DMMU + 0x200 + (WAY) * 0x100)
#define SPR_DTLBMR_LAST(WAY)	(SPRGROUP_DMMU + 0x27f + (WAY) * 0x100)
#define SPR_DTLBTR_BASE(WAY)	(SPRGROUP_DMMU + 0x280 + (WAY) * 0x100)
#define SPR_DTLBTR_LAST(WAY)	(SPRGROUP_DMMU + 0x2ff + (WAY) * 0x100)

/* Instruction MMU group */
#define SPR_IMMUCR	(SPRGROUP_IMMU + 0)	/* Control Register implemented */
#define SPR_ITLBEIR     (SPRGROUP_IMMU + 2)     /* Instruction TLB Entry Invalidate Register*/
#define SPR_ITLBMR      (SPRGROUP_IMMU + 4)     /* Instruction TLB Match Register */
#define SPR_ITLBTR      (SPRGROUP_IMMU + 6)     /* Instruction TLB Translate Register */
#define SPR_ITLBMR_BASE(WAY)	(SPRGROUP_IMMU + 0x200 + (WAY) * 0x100)
#define SPR_ITLBMR_LAST(WAY)	(SPRGROUP_IMMU + 0x27f + (WAY) * 0x100)
#define SPR_ITLBTR_BASE(WAY)	(SPRGROUP_IMMU + 0x280 + (WAY) * 0x100)
#define SPR_ITLBTR_LAST(WAY)	(SPRGROUP_IMMU + 0x2ff + (WAY) * 0x100)

/* Data cache group */
#define SPR_DCCR	(SPRGROUP_DC + 0)	/* Control Register implemented */
#define SPR_DCBPR	(SPRGROUP_DC + 1)	/* Data Cache Block Prefetch Register */
#define SPR_DCBFR	(SPRGROUP_DC + 2)       /* Data Cache Block Flush Register */
#define SPR_DCBIR	(SPRGROUP_DC + 3)       /* Data Cache Block Invalidate Register */
#define SPR_DCBWR	(SPRGROUP_DC + 4)       /* Data Cache Block Write-back Register */
#define SPR_DCBLR	(SPRGROUP_DC + 5)       /* Data Cache Block Lock Register */
#define SPR_DCR_BASE(WAY)	(SPRGROUP_DC + 0x200 + (WAY) * 0x200)
#define SPR_DCR_LAST(WAY)	(SPRGROUP_DC + 0x3ff + (WAY) * 0x200)
#define SPR_DCDBG	(SPRGROUP_DC + 6)       /* Data Cache Debugging Interface */
#define SPR_DCDBG_TAG(WAY)	((0x0 << 29) | (WAY<<26)) 
#define SPR_DCDBG_DATA(WAY)	((0x1 << 29) | (WAY<<26))
#define SPR_DCDBG_LRU		 (0x2 << 29)    /* WAY is either 0,1,2 or 3 */

/* Instruction cache group */
#define SPR_ICCR	(SPRGROUP_IC + 0)	/* Control Register implemented */
#define SPR_ICBPR	(SPRGROUP_IC + 1)	/* Instruction Cache Block Prefetch Register */
#define SPR_ICBIR	(SPRGROUP_IC + 2)	/* Instruction Cache Block Invalidate Register */
#define SPR_ICBLR	(SPRGROUP_IC + 5)	/* Instruction Cache Block Lock Register */
#define SPR_ICR_BASE(WAY)	(SPRGROUP_IC + 0x200 + (WAY) * 0x200)
#define SPR_ICR_LAST(WAY)	(SPRGROUP_IC + 0x3ff + (WAY) * 0x200)
#define SPR_ICDBG	(SPRGROUP_IC + 6)	/* Instruction Cache Debugging Interface */
#define SPR_ICDBG_TAG(WAY)	((0x0 << 29) | (WAY<<26)) 
#define SPR_ICDBG_DATA(WAY)	((0x1 << 29) | (WAY<<26))
#define SPR_ICDBG_LRU		 (0x2 << 29) /* WAY is either 0,1,2 or 3 */

/* MAC group */
#define SPR_MACLO 	(SPRGROUP_MAC + 1)	/* MAC Low */
#define SPR_MACHI 	(SPRGROUP_MAC + 2)	/* MAC High */

/* Debug group */
#define SPR_DVR(N)	(SPRGROUP_D + (N))	/* Debug Value Registers */
#define SPR_DCR(N)	(SPRGROUP_D + 8 + (N))	/* Debug Control Registers */
#define SPR_DMR1	(SPRGROUP_D + 16)	/* Debug Mode Register 1 */
#define SPR_DMR2	(SPRGROUP_D + 17)	/* Debug Mode Register 2 */
#define SPR_DWCR0	(SPRGROUP_D + 18)	/* Debug Watchpoint Counter Register 0 */
#define SPR_DWCR1	(SPRGROUP_D + 19)	/* Debug Watchpoint Counter Regi
ster 1 */
#define SPR_DSR		(SPRGROUP_D + 20)	/* Debug Stop Register */
#define SPR_DRR		(SPRGROUP_D + 21)	/* Debug Reason Register */
#define SPR_DRAR	(SPRGROUP_D + 22)	/* Debug Reason Address Register */
#define SPR_DREASON	SPR_DRAR		/* Compatibility */
 	
/* Trace Buffer */
#define SPR_DTBRP	(SPRGROUP_D + 0xfe)	/* Trace Buffer Read Ponter */
#define SPR_DTBWP	(SPRGROUP_D + 0xff)	/* Trace Buffer Write Pointer */
#define SPR_DTBRBASE	(SPRGROUP_D + 0x100)	/* Program Counters */
#define SPR_DTBIA	(SPRGROUP_D + 0x200)	/* High 4 bytes of Instruction */
#define SPR_DTBAR	(SPRGROUP_D + 0x300)	/* Data written to Register File */
#define SPR_DTBTS	(SPRGROUP_D + 0x400)	/* Lower 2 bytes of the Instruction [31:16], Free Running Counter [15:0] */
#define SPR_DTBFC	(SPRGROUP_D + 0x500)	/* Non-architectural state */

/* Performance counters group */
#define SPR_PCCR(N)	(SPRGROUP_PC + (N))	/* Performance Counters Count Registers */
#define SPR_PCMR(N)	(SPRGROUP_PC + 8 + (N))	/* Performance Counters Count Mode Registers */

/* Power management group */
#define SPR_PMR (SPRGROUP_PM + 0)	/* Power Management Register */
#define SPR_PM_EVENT_CTRL (SPRGROUP_PM + 0)	/* CHECK */
#define SPR_PM_UNITS_PD   (SPRGROUP_PM + 2)	/* CHECK */

/* PIC group */
#define SPR_PICMR (SPRGROUP_PIC + 0)	/* PIC Mask Register */
#define SPR_PICPR (SPRGROUP_PIC + 1)	/* CHECK */
#define SPR_PICSR (SPRGROUP_PIC + 2)	/* PIC Status Register */

/* Tick Timer group */
#define SPR_TTMR (SPRGROUP_TT + 0)	/* Tick Timer Mode Register */
#define SPR_TTCR (SPRGROUP_TT + 1)	/* Tick Timer Count Register */
#define SPR_TTUR (SPRGROUP_TT + 31)	/* CHECK */

/*
 * Bit definitions for the Version Register
 *
 */
#define SPR_VR_VER	0xffffff00  /* Processor version */
#define SPR_VR_CFG	0x00ff0000  /* Processor configuration */
#define SPR_VR_REV	0x0000003f  /* Processor revision */

/*
 * Bit definitions for the Unit Present Register
 *
 */
#define SPR_UPR_UP	0x00000001  /* UPR present */
#define SPR_UPR_DCP	0x00000002  /* Data cache present */
#define SPR_UPR_ICP	0x00000004  /* Instruction cache present */
#define SPR_UPR_DMP	0x00000008  /* Data MMU present */
#define SPR_UPR_IMP	0x00000010  /* Instruction MMU present */
#define SPR_UPR_MP	0x00000020  /* MAC Present */
#define SPR_UPR_DUP	0x00000040  /* Debug unit present */
#define SPR_UPR_PCUP	0x00000080  /* Performance counters unit present */
#define SPR_UPR_PMP	0x00000100  /* Power management present */
#define SPR_UPR_PICP	0x00000200  /* PIC present */
#define SPR_UPR_TTP	0x00000400  /* Tick timer present */
#define SPR_UPR_SRP	0x00010000  /* Shadow Registers present */
#define SPR_UPR_RES	0x00fe0000  /* ORVDX32 present */
#define SPR_UPR_CUST	0xff000000  /* Custom units */

/*
 * Bit definitions for the Supervision Register
 *
 */
#define SPR_SR_CID      0xf0000000  /* Context ID */
#define SPR_SR_SUMRA    0x00010000  /* Supervisor SPR read access */
#define SPR_SR_FO       0x00008000  /* Fixed one */
#define SPR_SR_EPH      0x00004000  /* Exception Prefix High */
#define SPR_SR_DSX      0x00002000  /* Delay Slot Exception */
#define SPR_SR_TED      0x00001000  /* Trap Exception Disabled */
#define SPR_SR_OV       0x00000800  /* Overflow flag */
#define SPR_SR_CY       0x00000400  /* Carry flag */
#define SPR_SR_F        0x00000200  /* Condition Flag */
#define SPR_SR_SBE      0x00000100  /* Store Buffer Enable */
#define SPR_SR_LEE      0x00000080  /* Little Endian Enable */
#define SPR_SR_IME      0x00000040  /* Instruction MMU Enable */
#define SPR_SR_DME      0x00000020  /* Data MMU Enable */
#define SPR_SR_ICE      0x00000010  /* Instruction Cache Enable */
#define SPR_SR_DCE      0x00000008  /* Data Cache Enable */
#define SPR_SR_IEE      0x00000004  /* Interrupt Exception Enable */
#define SPR_SR_TEE      0x00000002  /* Tick timer Exception Enable */
#define SPR_SR_SM       0x00000001  /* Supervisor Mode */

/*
 * Bit definitions for the Data MMU Control Register
 *
 */
#define SPR_DMMUCR_P2S	0x0000003e  /* Level 2 Page Size */
#define SPR_DMMUCR_P1S	0x000007c0  /* Level 1 Page Size */
#define SPR_DMMUCR_VADDR_WIDTH	0x0000f800  /* Virtual ADDR Width */
#define SPR_DMMUCR_PADDR_WIDTH	0x000f0000  /* Physical ADDR Width */

/*
 * Bit definitions for the Instruction MMU Control Register
 *
 */
#define SPR_IMMUCR_P2S	0x0000003e  /* Level 2 Page Size */
#define SPR_IMMUCR_P1S	0x000007c0  /* Level 1 Page Size */
#define SPR_IMMUCR_VADDR_WIDTH	0x0000f800  /* Virtual ADDR Width */
#define SPR_IMMUCR_PADDR_WIDTH	0x000f0000  /* Physical ADDR Width */

/*
 * Bit definitions for the Data TLB Match Register
 *
 */
#define SPR_DTLBMR_V	0x00000001  /* Valid */
#define SPR_DTLBMR_PL1	0x00000002  /* Page Level 1 (if 0 then PL2) */
#define SPR_DTLBMR_CID	0x0000003c  /* Context ID */
#define SPR_DTLBMR_LRU	0x000000c0  /* Least Recently Used */
#define SPR_DTLBMR_VPN	0xffffe000  /* Virtual Page Number */

/*
 * Bit definitions for the Data TLB Translate Register
 *
 */
#define SPR_DTLBTR_CC	0x00000001  /* Cache Coherency */
#define SPR_DTLBTR_CI	0x00000002  /* Cache Inhibit */
#define SPR_DTLBTR_WBC	0x00000004  /* Write-Back Cache */
#define SPR_DTLBTR_WOM	0x00000008  /* Weakly-Ordered Memory */
#define SPR_DTLBTR_A	0x00000010  /* Accessed */
#define SPR_DTLBTR_D	0x00000020  /* Dirty */
#define SPR_DTLBTR_URE	0x00000040  /* User Read Enable */
#define SPR_DTLBTR_UWE	0x00000080  /* User Write Enable */
#define SPR_DTLBTR_SRE	0x00000100  /* Supervisor Read Enable */
#define SPR_DTLBTR_SWE	0x00000200  /* Supervisor Write Enable */
#define SPR_DTLBTR_PPN	0xffffe000  /* Physical Page Number */

/*
 * Bit definitions for the Instruction TLB Match Register
 *
 */
#define SPR_ITLBMR_V	0x00000001  /* Valid */
#define SPR_ITLBMR_PL1	0x00000002  /* Page Level 1 (if 0 then PL2) */
#define SPR_ITLBMR_CID	0x0000003c  /* Context ID */
#define SPR_ITLBMR_LRU	0x000000c0  /* Least Recently Used */
#define SPR_ITLBMR_VPN	0xffffe000  /* Virtual Page Number */

/*
 * Bit definitions for the Instruction TLB Translate Register
 *
 */
#define SPR_ITLBTR_CC	0x00000001  /* Cache Coherency */
#define SPR_ITLBTR_CI	0x00000002  /* Cache Inhibit */
#define SPR_ITLBTR_WBC	0x00000004  /* Write-Back Cache */
#define SPR_ITLBTR_WOM	0x00000008  /* Weakly-Ordered Memory */
#define SPR_ITLBTR_A	0x00000010  /* Accessed */
#define SPR_ITLBTR_D	0x00000020  /* Dirty */
#define SPR_ITLBTR_SXE	0x00000040  /* User Read Enable */
#define SPR_ITLBTR_UXE	0x00000080  /* User Write Enable */
#define SPR_ITLBTR_PPN	0xffffe000  /* Physical Page Number */

/*
 * Bit definitions for Data Cache Control Register
 *
 */
#define SPR_DCCR_EW	0x000000ff  /* Enable ways */

/*
 * Bit definitions for Insn Cache Control Register
 *
 */
#define SPR_ICCR_EW	0x000000ff  /* Enable ways */

/*
 * Bit definitions for Debug Control Register
 *
 */
#define SPR_DCFGR_NDP	0x0000000f	/* DVR/DCR pairs */
#define SPR_DCFGR_WPCI	0x00000010	/* performance counters */

/*
 * Bit definitions for Trace Buffer
 *
 */
#define SPR_DCFGR_TB		0x00000020	/* trace buffer implemented */
#define SPR_DCFGR_TB_SIZE	0x000003c0	/* log2 trace buffer depth */

/*
 * Bit definitions for the CPU Configuration Register
 *
 */
#define SPR_CPUCFGR_NSGF	0x0000000f /* Number of shadow GPR files */
#define SPR_CPUCFGR_CGF		0x00000010 /* Custom GPR file */
#define SPR_CPUCFGR_OB32S	0x00000020 /* ORBIS32 Supported */
#define SPR_CPUCFGR_OB64S	0x00000040 /* ORBIS64 Supported */
#define SPR_CPUCFGR_OF32S	0x00000080 /* ORFPX32 Supported */
#define SPR_CPUCFGR_OF64S	0x00000100 /* ORFPX64P Supported */
#define SPR_CPUCFGR_ORVDX64	0x00000200 /* ORVDX63 Supported */

/*
 * Bit definitions for Data Cache Configuration Register
 *
 */

#define SPR_DCCFGR_NCW		0x0000000F	/* number of ways */
#define SPR_DCCFGR_NCS		0x000000F0	/* log2 of number of sets */
#define SPR_DCCFGR_CBS		0x00000F00	/* lof2 of block size (in words) */
#define SPR_DCCFGR_CWS		0x00001000	/* write strategy */
#define SPR_DCCFGR_CBIRI	0x00002000	/* cache invalidate block */
#define SPR_DCCFGR_CBPRI	0x00004000	/* cache prefetch Register */
#define SPR_DCCFGR_CBLRI	0x00008000	/* cache lock Register */
#define SPR_DCCFGR_CBFRI	0x00010000	/* cache flush Register */
#define SPR_DCCFGR_CBWBRI	0x00020000	/* cache write back Register */

/*
 * Bit definitions for Instruction Cache Configuration Register
 *
 */
#define SPR_ICCFGR_NCW		0x0000000F	/* number of ways */
#define SPR_ICCFGR_NCS		0x000000F0	/* log2 of number of sets */
#define SPR_ICCFGR_CBS		0x00000F00	/* log2 of block size (in words) */
#define SPR_ICCFGR_CBIRI	0x00002000	/* cache invalidate Register */
#define SPR_ICCFGR_CBPRI	0x00004000	/* cache prefetch Register */
#define SPR_ICCFGR_CBLRI	0x00008000	/* cache lock Register */

/*
 * Bit definitions for Data MMU Configuration Register
 *
 */

#define SPR_DMMUCFGR_NTW	0x0000000F	/* number of ways */
#define SPR_DMMUCFGR_NTS	0x000000F0	/* log2 of number of sets */
#define SPR_DMMUCFGR_CRI	0x00000100	/* control Register implemented */
#define SPR_DMMUCFGR_PRI        0x00000200	/* protection Register */
#define SPR_DMMUCFGR_TEIRI	0x00000400	/* TLB invalidate Register */
#define SPR_DMMUCFGR_HTR	0x00000800	/* HW TLB reload */

/*
 * Bit definitions for Instruction MMU Configuration Register
 *
 */

#define SPR_IMMUCFGR_NTW	0x0000000F	/* number of ways */
#define SPR_IMMUCFGR_NTS	0x000000F0	/* log2 of number of sets */
#define SPR_IMMUCFGR_CRI	0x00000100	/* control Register implemented */
#define SPR_IMMUCFGR_PRI	0x00000200	/* protection Register */
#define SPR_IMMUCFGR_TEIRI	0x00000400	/* TLB invalidate Register */
#define SPR_IMMUCFGR_HTR	0x00000800	/* HW TLB reload */

/*
 * Bit definitions for Debug Control Registers
 *
 */
#define SPR_DCR_DP	0x00000001  /* DVR/DCR present */
#define SPR_DCR_CC	0x0000000e  /* Compare condition */
#define SPR_DCR_SC	0x00000010  /* Signed compare */
#define SPR_DCR_CT	0x000000e0  /* Compare to */

/* Bit results with SPR_DCR_CC mask */
#define SPR_DCR_CC_MASKED 0x00000000	/* masked */
#define SPR_DCR_CC_EQUAL  0x00000002	/* equal */
#define SPR_DCR_CC_LESS   0x00000004	/* less than */
#define SPR_DCR_CC_LESSE  0x00000006	/* less than or equal */
#define SPR_DCR_CC_GREAT  0x00000008	/* greater than */
#define SPR_DCR_CC_GREATE 0x0000000a	/* greater than or equal */ 
#define SPR_DCR_CC_NEQUAL 0x0000000c	/* reserved */

/* Bit results with SPR_DCR_CT mask */
#define SPR_DCR_CT_DISABLED 0x00000000 /* comparison disabled */
#define SPR_DCR_CT_IFEA     0x00000020 /* instruction fetch EA */
#define SPR_DCR_CT_LEA      0x00000040 /* load EA */
#define SPR_DCR_CT_SEA      0x00000060 /* store EA */
#define SPR_DCR_CT_LD       0x00000080 /* load data */
#define SPR_DCR_CT_SD       0x000000a0 /* store data */
#define SPR_DCR_CT_LSEA     0x000000c0 /* load/store EA */
#define SPR_DCR_CT_LSD	    0x000000e0 /* load/sotre data */
/* SPR_DCR_CT_LSD doesn't seem to be implemented anywhere in basim. 2004-1-30 HP */

/*
 * Bit definitions for Debug Mode 1 Register
 *
 */
#define SPR_DMR1_CW0	0x00000003  /* Chain watchpoint 0 */
#define SPR_DMR1_CW1	0x0000000c  /* Chain watchpoint 1 */
#define SPR_DMR1_CW2	0x00000030  /* Chain watchpoint 2 */
#define SPR_DMR1_CW3	0x000000c0  /* Chain watchpoint 3 */
#define SPR_DMR1_CW4	0x00000300  /* Chain watchpoint 4 */
#define SPR_DMR1_CW5	0x00000c00  /* Chain watchpoint 5 */
#define SPR_DMR1_CW6	0x00003000  /* Chain watchpoint 6 */
#define SPR_DMR1_CW7	0x0000c000  /* Chain watchpoint 7 */
#define SPR_DMR1_CW8	0x00030000  /* Chain watchpoint 8 */
#define SPR_DMR1_CW9	0x000c0000  /* Chain watchpoint 9 */
#define SPR_DMR1_CW10	0x00300000  /* Chain watchpoint 10 */
#define SPR_DMR1_ST	0x00400000  /* Single-step trace*/
#define SPR_DMR1_BT	0x00800000  /* Branch trace */
#define SPR_DMR1_DXFW	0x01000000  /* Disable external force watchpoint */

#define SPR_DMR1_CHAIN_01 0x4

/*
 * Bit definitions for Debug Mode 2 Register
 *
 */
#define SPR_DMR2_WCE0	0x00000001  /* Watchpoint counter 0 enable */
#define SPR_DMR2_WCE1	0x00000002  /* Watchpoint counter 1 enable */
#define SPR_DMR2_AWTC	0x00000ffc  /* Assign watchpoints to counters */
#define SPR_DMR2_WGB	0x003ff000  /* Watchpoints generating breakpoint */
#define SPR_DMR2_WBS	0xffc00000  /* Watchpoints generated breakpoint */

/*
 * Bit definitions for Debug watchpoint counter Registers
 *
 */
#define SPR_DWCR_COUNT	0x0000ffff  /* Count */
#define SPR_DWCR_MATCH	0xffff0000  /* Match */

/*
 * Bit definitions for Debug stop Register
 *
 */
#define SPR_DSR_RSTE	0x00000001  /* Reset exception */
#define SPR_DSR_BUSEE	0x00000002  /* Bus error exception */
#define SPR_DSR_DPFE	0x00000004  /* Data Page Fault exception */
#define SPR_DSR_IPFE	0x00000008  /* Insn Page Fault exception */
#define SPR_DSR_TTE	0x00000010  /* Tick Timer exception */
#define SPR_DSR_AE	0x00000020  /* Alignment exception */
#define SPR_DSR_IIE	0x00000040  /* Illegal Instruction exception */
#define SPR_DSR_IE	0x00000080  /* Interrupt exception */
#define SPR_DSR_DME	0x00000100  /* DTLB miss exception */
#define SPR_DSR_IME	0x00000200  /* ITLB miss exception */
#define SPR_DSR_RE	0x00000400  /* Range exception */
#define SPR_DSR_SCE	0x00000800  /* System call exception */
#define SPR_DSR_FPE     0x00001000  /* Floating Point Exception */
#define SPR_DSR_TE	0x00002000  /* Trap exception */

/*
 * Bit definitions for Debug reason Register
 *
 */
#define SPR_DRR_RSTE	0x00000001  /* Reset exception */
#define SPR_DRR_BUSEE	0x00000002  /* Bus error exception */
#define SPR_DRR_DPFE	0x00000004  /* Data Page Fault exception */
#define SPR_DRR_IPFE	0x00000008  /* Insn Page Fault exception */
#define SPR_DRR_TTE	0x00000010  /* Tick Timer exception */
#define SPR_DRR_AE	0x00000020  /* Alignment exception */
#define SPR_DRR_IIE	0x00000040  /* Illegal Instruction exception */
#define SPR_DRR_IE	0x00000080  /* Interrupt exception */
#define SPR_DRR_DME	0x00000100  /* DTLB miss exception */
#define SPR_DRR_IME	0x00000200  /* ITLB miss exception */
#define SPR_DRR_RE	0x00000400  /* Range exception */
#define SPR_DRR_SCE	0x00000800  /* System call exception */
#define SPR_DRR_FPE     0x00001000  /* Floating Point Exception */
#define SPR_DRR_TE	0x00002000  /* Trap exception */

/*
 * Bit definitions for Performance counters mode Registers
 *
 */
#define SPR_PCMR_CP	0x00000001  /* Counter present */
#define SPR_PCMR_UMRA	0x00000002  /* User mode read access */
#define SPR_PCMR_CISM	0x00000004  /* Count in supervisor mode */
#define SPR_PCMR_CIUM	0x00000008  /* Count in user mode */
#define SPR_PCMR_LA	0x00000010  /* Load access event */
#define SPR_PCMR_SA	0x00000020  /* Store access event */
#define SPR_PCMR_IF	0x00000040  /* Instruction fetch event*/
#define SPR_PCMR_DCM	0x00000080  /* Data cache miss event */
#define SPR_PCMR_ICM	0x00000100  /* Insn cache miss event */
#define SPR_PCMR_IFS	0x00000200  /* Insn fetch stall event */
#define SPR_PCMR_LSUS	0x00000400  /* LSU stall event */
#define SPR_PCMR_BS	0x00000800  /* Branch stall event */
#define SPR_PCMR_DTLBM	0x00001000  /* DTLB miss event */
#define SPR_PCMR_ITLBM	0x00002000  /* ITLB miss event */
#define SPR_PCMR_DDS	0x00004000  /* Data dependency stall event */
#define SPR_PCMR_WPE	0x03ff8000  /* Watchpoint events */

/* 
 * Bit definitions for the Power management Register
 *
 */
/* BA22 */
#define SPR_PM_EVENT_TT     0x2
#define SPR_PM_EVENT_PIC    0x1

#define SPR_PM_DU_CLK_EN 0x1

/* BA12 */
#define SPR_PMR_SDF	0x0000000f  /* Slow down factor */
#define SPR_PMR_DME	0x00000010  /* Doze mode enable */
#define SPR_PMR_SME	0x00000020  /* Sleep mode enable */
#define SPR_PMR_DCGE	0x00000040  /* Dynamic clock gating enable */
#define SPR_PMR_SUME	0x00000080  /* Suspend mode enable */

/*
 * Bit definitions for PICMR
 *
 */
#define SPR_PICMR_IUM	0xffffffff  /* Interrupt unmask */

/*
 * Bit definitions for PICPR
 *
 */
#define SPR_PICPR_IPRIO	0xfffffffc  /* Interrupt priority */

/*
 * Bit definitions for PICSR
 *
 */
#define SPR_PICSR_IS	0xffffffff  /* Interrupt status */

/*
 * Bit definitions for Tick Timer Control Register
 *
 */
#define SPR_TTCR_PERIOD	0x0fffffff  /* Time Period */
#define SPR_TTMR_PERIOD	SPR_TTCR_PERIOD
#define SPR_TTMR_IP	0x10000000  /* Interrupt Pending */
#define SPR_TTMR_IE	0x20000000  /* Interrupt Enable */
#define SPR_TTMR_RT	0x40000000  /* Restart tick */
#define SPR_TTMR_SR     0x80000000  /* Single run */
#define SPR_TTMR_CR     0xc0000000  /* Continuous run */
#define SPR_TTMR_M      0xc0000000  /* Tick mode */

#define SPR_TTUR_ICTM   0x00000001  /* Instruction Counting Timer Mode */

/*
 * Bit definitions for Tick Timer Undocumented Register
 *
 */
#define SPR_TTUR_CNT_INSNS 0x00000001
#define SPR_TTUR_CNT_TICKS 0x00000000

/*
 * l.nop constants
 *
 */
#define NOP_NOP         0x0000      /* Normal nop instruction */
#define NOP_EXIT        0x0001      /* End of simulation */
#define NOP_REPORT      0x0002      /* Simple report */
#define NOP_PRINTF      0x0003      /* Simprintf instruction */
#define NOP_CNT_RESET   0x0005	    /* Reset statistics counters */
#define NOP_SIM_SYS_CALL 0x0006     /* Simulator syscall emulation */
#define NOP_REPORT_FIRST 0x0400     /* Report with number */
#define NOP_REPORT_LAST 0x03ff      /* Report with number */

#endif
