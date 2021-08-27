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
#include <asm/inca-ip.h>
#include <asm/mipsregs.h>
#include <asm/cacheops.h>
#include <asm/reboot.h>

#define cache_op(op,addr)						\
	__asm__ __volatile__(						\
	"	.set	push					\n"	\
	"	.set	noreorder				\n"	\
	"	.set	mips3\n\t				\n"	\
	"	cache	%0, %1					\n"	\
	"	.set	pop					\n"	\
	:								\
	: "i" (op), "R" (*(unsigned char *)(addr)))

static void MDrv_Write4Byte(U16 u16Reg, U32 u32Value)
{
    RIU[u16Reg] = u32Value;
    RIU[u16Reg + 2] = (u32Value >> 16);
}

#if (defined(CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || defined(CONFIG_JANUS2))
void MDrv_Sys_WholeChipReset( void )
{
    MDrv_Write4Byte(0x0EA0, 0x51685168);
    MDrv_WriteRegBit(0x2E52, 1, 0x0080);
    MDrv_WriteRegBit(0x2E52, 1, 0x0040);

    MDrv_WriteByte( 0x2E5C, 0x79 );
    while(1);
}
#else
void MDrv_Sys_WholeChipReset( void )
{
    MDrv_Write4Byte(0x0EA0, 0x51685168);
    MDrv_WriteRegBit(0x0E52, 1, 0x0080);
    MDrv_WriteRegBit(0x0E52, 1, 0x0040);

    MDrv_WriteByte( 0x0E5C, 0x79 );
    while(1);
}
#endif

int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
#if defined(CONFIG_INCA_IP)
	*INCA_IP_WDT_RST_REQ = 0x3f;
#elif defined(CONFIG_PURPLE) || defined(CONFIG_TB0229)
	void (*f)(void) = (void *) TEXT_BASE;

	f();
#endif
#if 0
	void (*f)(void) = (void *) (TEXT_BASE-0xFA00); //0x875F0600
	f();

	fprintf(stderr, "*** reset failed ***\n");
#endif

    MDrv_Sys_WholeChipReset();
	return 0;
}

extern void Chip_Read_Memory_Range(unsigned long , unsigned long);
#if !defined(CONFIG_JANUS) && !defined(CONFIG_MARIA10)
void Chip_Read_Memory(void)
{
    #if defined(CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || defined(CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_KAISERIN)
		Chip_Read_Memory_Range(0, 0x8000); //32KB
	#else
   	volatile unsigned int *pu8;
    volatile unsigned int t ;

    // Transfer the memory to noncache memory
    pu8 = ((volatile unsigned int *)0xA0380000);
    t = pu8[0] ;
    t = pu8[64] ;
	#endif
}
#endif

extern void Chip_Flush_Memory_Range(unsigned long, unsigned long);
void Chip_Flush_Memory(void)
{
	#if defined(CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || defined(CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_KAISERIN)
		Chip_Flush_Memory_Range( 0 , 0xFFFFFFFF );
	#else
    static unsigned int u8_4Lines[64];
    volatile unsigned int *pu8;
    volatile unsigned int tmp ;

    #if (defined(CONFIG_KRONUS)||defined(CONFIG_KAPPA)||defined(CONFIG_KRITI))
        // kronus_mips_L2_cache_reg.xls
        if((*(volatile unsigned int *)(0xBF203104) & 0x0100) != 0)
        {
            // @FIXME: the chiptop dummy reead should be turn off since HW should fix this in T12/T13
            *(volatile unsigned int *) (0xBF003C00);        //Dummy Read
            *(volatile unsigned int *) 0xBF203104 = 0x04;
            // @FIXME: the chiptop dummy reead should be turn off since HW should fix this in T12/T13
            *(volatile unsigned int *) (0xBF003C00);        //Dummy Read
            *(volatile unsigned int *) 0xBF203104 = 0x14;
            while(1)
            {
                // @FIXME: the chiptop dummy reead should be turn off since HW should fix this in T12/T13
                *(volatile unsigned int *) (0xBF003C00);    //Dummy Read
                if((*(volatile unsigned int *)(0xBF203104) & 0x0400) != 0)
                {
                    break;
                }
            }
        }
    #endif
    // MIU0
    // Transfer the memory to noncache memory
    pu8 = ((volatile unsigned int *)(((unsigned int)u8_4Lines) | 0xa0000000));

    // Flush the data from pipe and buffer in MIU
    pu8[0] = pu8[16] = pu8[32] = pu8[48] = 1;

    // Flush the data in the EC bridge buffer
    asm volatile (
        "sync;"
    );
    // final read back
    tmp = pu8[48] ;
	#endif
}

extern void Chip_L2_cache_wback_inv( unsigned long addr, unsigned long size);
void flush_cache(ulong start_addr, ulong size)
{
	unsigned long lsize = CFG_CACHELINE_SIZE;
	unsigned long addr = start_addr & ~(lsize - 1);
	unsigned long aend = (start_addr + size - 1) & ~(lsize - 1);

	while (1) {
		cache_op(Hit_Writeback_Inv_D, addr);
		cache_op(Hit_Invalidate_I, addr);
		if (addr == aend)
			break;
		addr += lsize;
	}
	#if defined(CONFIG_TITANIA8) || defined(CONFIG_TITANIA12) || defined(CONFIG_AMBER2) || defined(CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_KAISERIN)
	if(  ( 0x80000000 <= addr )   && ( addr < 0xA0000000) )
    {
        Chip_L2_cache_wback_inv( addr&0x0FFFFFFF , size);
    }
    else //  high mem, user space
    {
        Chip_L2_cache_wback_inv( 0 , 0xFFFFFFFF );
    }
	#endif
}

void write_one_tlb( int index, u32 pagemask, u32 hi, u32 low0, u32 low1 ){
	write_32bit_cp0_register(CP0_ENTRYLO0, low0);
	write_32bit_cp0_register(CP0_PAGEMASK, pagemask);
	write_32bit_cp0_register(CP0_ENTRYLO1, low1);
	write_32bit_cp0_register(CP0_ENTRYHI, hi);
	write_32bit_cp0_register(CP0_INDEX, index);
	tlb_write_indexed();
}
