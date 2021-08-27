/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
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

#include <common.h>
#include <command.h>
#include <asm/spr_defs.h>

int checkcpu (void)
{
	printf ("CPU   : OpenRISC OR1xxx\n");

	// System ID : Print

	return (0);
}

int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    ulong	addr;
    addr = simple_strtoul(argv[1], NULL, 16);
    printf("aeonR2 reset from DRAM addr 0x%x !!\n", addr*0x100000);
    *(volatile unsigned short *)(0xA0200000+(0x2B4*2)) = addr*0x10;

    while(1);
	return 0;
}
extern void mhal_dcache_flush(U32 u32Base, U32 u32Size);

void flush_cache(ulong start_addr, ulong size)
{
#if (CONFIG_ENABLE_DCACHE)
    mhal_dcache_flush(start_addr, size);
#endif
}

