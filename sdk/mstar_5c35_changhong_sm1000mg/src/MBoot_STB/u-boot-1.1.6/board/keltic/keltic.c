/*
 * Board initialize code for TANBAC Evaluation board TB0229.
 *
 * (C) Masami Komiya <mkomiya@sonare.it> 2004
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 */

#include <common.h>
#include <command.h>
#include <asm/addrspace.h>
#include <pci.h>

unsigned long mips_io_port_base = 0;

long int initdram(int board_type)
{
	return 0x8000000;		// Patch: return DRAM to avoid using MIU1
}


int checkboard (void)
{
	printf("Board: MSTAR KELTIC ");
	printf("(CPU Speed %d MHz)\n", (int)CPU_CLOCK_RATE/1000000);
	printf("DRAM:  %d X %d MBytes\n", MIU0_LEN/0x100000, MIU1_LEN/0x100000);
	return 0;
}

