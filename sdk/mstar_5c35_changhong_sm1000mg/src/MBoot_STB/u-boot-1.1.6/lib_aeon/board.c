/*
 * (C) Copyright 2009, Libertron Corporation <www.libertron.com>
 * JhoonKim <jhkim@libertron.com>
 *
 * (C) Copyright 2003, Psyent Corporation <www.psyent.com>
 * Scott McNutt <smcnutt@psyent.com>
 *
 * (C) Copyright 2000-2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
#include <malloc.h>
#include <devices.h>
#include <config.h>
#include <net.h>
#include <linux/list.h>
#include <version.h>


DECLARE_GLOBAL_DATA_PTR;

ulong monitor_flash_len;
extern ulong bss_start;
extern ulong aeonboot_start;

#if (CONFIG_COMMANDS & CFG_CMD_NAND)
void nand_init (void);
#endif

/*
 * All attempts to come up with a "common" initialization sequence
 * that works for all boards and architectures failed: some of the
 * requirements are just _too_ different. To get rid of the resulting
 * mess of board dependend #ifdef'ed code we now make the whole
 * initialization sequence configurable to the user.
 *
 * The requirements for any new initalization function is simple: it
 * receives a pointer to the "global data" structure as it's only
 * argument, and returns an integer return code, where 0 means
 * "continue" and != 0 means "fatal error, hang the system".
 */

extern int serial2_init (void);
extern int timer_init(void);
extern void malloc_bin_reloc (void);
extern unsigned int cfg_env_offset;

typedef int (init_fnc_t) (void);

const char version_string[] =	U_BOOT_VERSION" (" __DATE__ " - " __TIME__ ")";

static char *failed = "*** failed ***\n";

/*
 * Begin and End of memory area for malloc(), and current "brk"
 */
static	ulong	mem_malloc_start = 0;
static	ulong	mem_malloc_end	 = 0;
static	ulong	mem_malloc_brk	 = 0;

/*
 * The Malloc area is immediately below the monitor copy in RAM
 */
static void mem_malloc_init (void)
{
	mem_malloc_start = CFG_MALLOC_BASE;
	mem_malloc_end = mem_malloc_start + CFG_MALLOC_LEN;
	mem_malloc_brk = mem_malloc_start;
	memset ((void *) mem_malloc_start,
		0,
		mem_malloc_end - mem_malloc_start);
}

void *sbrk (ptrdiff_t increment)
{
	ulong old = mem_malloc_brk;
	ulong new = old + increment;

	if ((new < mem_malloc_start) || (new > mem_malloc_end)) {
		return (NULL);
	}
	mem_malloc_brk = new;
	return ((void *) old);
}


/************************************************************************
 * Initialization sequence						*
 ***********************************************************************/

static int init_func_ram (void)
{
#ifdef	CONFIG_BOARD_TYPES
	int board_type = gd->board_type;
#else
	int board_type = 0;	/* use dummy arg */
#endif
	puts ("DRAM:  ");

	if ((gd->ram_size = initdram (board_type)) > 0) {
		print_size (gd->ram_size, "\n");
		return (0);
	}
	puts (failed);
	return (1);
}

static int display_enable_module(void)
{
    printf("Module: ");
    if(ENABLE_MODULE_KEYPAD == 1)
        printf("KEYPAD ");
    if(ENABLE_MODULE_USB == 1)
        printf("USB ");
    if(ENABLE_MODULE_FAT == 1)
        printf("FAT ");
    if(ENABLE_MODULE_NAND_FLASH == 1)
        printf("NAND ");
    if(ENABLE_MODULE_FLASH == 1)
        printf("FLASH ");
    if(ENABLE_MODULE_UBI == 1)
        printf("UBI ");
    if(ENABLE_MODULE_UBI_FS == 1)
        printf("UBIFS ");
    if(ENABLE_MODULE_SECURITY_BOOT == 1)
        printf("SEC_BOOT ");
    if(ENABLE_MODULE_NET == 1)
        printf("NET ");
    if(ENABLE_MODULE_USB_LAN == 1)
        printf("USB_LAN ");
    if(ENABLE_MODULE_SPI == 1)
        printf("SPI ");
    if(ENABLE_MODULE_EEPROM == 1)
        printf("EEPROM ");
    if(ENABLE_MODULE_STANDBY == 1)
        printf("STANDBY ");
    if(ENABLE_MODULE_JFFS2 == 1)
        printf("JFFS2 ");
    if(ENABLE_MODULE_DISPLAY_LOGO == 1)
        printf("LOGO ");
    if(ENABLE_MODULE_MICOM == 1)
        printf("MICOM ");

    printf("\n");
    return 0;
}

static int display_banner(void)
{

	printf ("\n\n%s\n\n", version_string);
	return (0);
}

static int init_baudrate (void)
{
	char tmp[64];	/* long enough for environment variables */
	int i = getenv_r ("baudrate", tmp, sizeof (tmp));

	gd->baudrate = (i > 0)
			? (int) simple_strtoul (tmp, NULL, 10)
			: CONFIG_BAUDRATE;

	return (0);
}


/*
 * Breath some life into the board...
 *
 * The first part of initialization is running from Flash memory;
 * its main purpose is to initialize the RAM so that we
 * can relocate the monitor code to RAM.
 */

/*
 * All attempts to come up with a "common" initialization sequence
 * that works for all boards and architectures failed: some of the
 * requirements are just _too_ different. To get rid of the resulting
 * mess of board dependend #ifdef'ed code we now make the whole
 * initialization sequence configurable to the user.
 *
 * The requirements for any new initalization function is simple: it
 * receives a pointer to the "global data" structure as it's only
 * argument, and returns an integer return code, where 0 means
 * "continue" and != 0 means "fatal error, hang the system".
 */

init_fnc_t *init_sequence[] = {
	timer_init,
	env_init,		/* initialize environment */
	init_baudrate,		/* initialze baudrate settings */
	serial_init,		/* serial communications setup */
	console_init_f,
	display_banner,		/* say that we are here */
	checkboard,
	init_func_ram,
	NULL,			/* Terminate this list */
};
#if 0
    #define RIU_MAP 0xA0000000
    #define RIU     ((unsigned short volatile *) RIU_MAP)
    #define RIU8    ((unsigned char  volatile *) RIU_MAP)
    #define    BDMA_REG_BASE             RIUBASE_BDMA       // 0x100900 ~ 0x1097F
    #define    RIUBASE_BDMA                0x100900    //0x100900 ~ 0x10097f

#define MST_MACRO_START     do {
#define MST_MACRO_END       } while (0)

#define __unlikely(_x_) __builtin_expect (!!(_x_), 0)

U8 __MDrv_ReadByte( U32 u32Reg )
{
    return RIU8[(u32Reg << 1) - (u32Reg & 1)];
}

U16 __MDrv_Read2Byte( U32 u32Reg )
{
    if (__unlikely(u32Reg & 0x01))
    {
        u32Reg <<= 1;
        return RIU8[u32Reg - 1] + (RIU8[u32Reg + 2] << 8);
    }
    else
    {
        return RIU[u32Reg];
    }
}

U32 __MDrv_Read3Byte( U32 u32Reg )
{
    if (__unlikely(u32Reg & 0x01))
    {
        return (RIU8[(u32Reg << 1) - 1] + (RIU[u32Reg + 1] << 8));
    }
    else
    {
        return (RIU[u32Reg] + (RIU8[(u32Reg + 2) << 1] << 16));
    }
}

U32 __MDrv_Read4Byte( U32 u32Reg )
{
    if (__unlikely(u32Reg & 0x01))
    {
        return (RIU8[(u32Reg << 1) - 1] + (RIU[u32Reg + 1] << 8) + (RIU8[(u32Reg + 3) << 1] << 24));
    }
    else
    {
        return (RIU[u32Reg] + (RIU[u32Reg + 2] << 16));
    }
}

void __MDrv_WriteByte( U32 u32Reg, U8 u8Value )
{
    RIU8[(u32Reg << 1) - (u32Reg & 1)] = u8Value;
}

void __MDrv_Write2Byte( U32 u32Reg, U16 u16Value )
{
    if (__unlikely(u32Reg & 0x01))
    {
        RIU8[(u32Reg << 1) - 1] = (U8)((u16Value));
        RIU8[(u32Reg + 1) << 1] = (U8)((u16Value) >> 8);
    }
    else
    {
        RIU[u32Reg] = u16Value;
    }
}

void __MDrv_Write3Byte( U32 u32Reg, U32 u32Value )
{
    if (__unlikely(u32Reg & 0x01))
    {
        RIU8[(u32Reg << 1) - 1] = u32Value;
        RIU[u32Reg + 1] = (u32Value >> 8);
    }
    else
    {
        RIU[u32Reg] = u32Value;
        RIU8[(u32Reg + 2) << 1] = (u32Value >> 16);
    }
}

void __MDrv_Write4Byte( U32 u32Reg, U32 u32Value )
{
    if (__unlikely(u32Reg & 0x01))
    {
        RIU8[(u32Reg << 1) - 1] = u32Value;
        RIU[u32Reg + 1] = (u32Value >> 8);
        RIU8[((u32Reg + 3) << 1)] = (u32Value >> 24);
    }
    else
    {
        RIU[u32Reg] = u32Value;
        RIU[u32Reg + 2] = (u32Value >> 16);
    }
}
#define MDrv_ReadByte( u32Reg )                                                         \
    (__builtin_constant_p( u32Reg )  ?                            \
        (((u32Reg) & 0x01) ? RIU8[(u32Reg) * 2 - 1] : RIU8[(u32Reg) * 2]) :             \
        __MDrv_ReadByte( u32Reg ))

#define MDrv_Read2Byte( u32Reg )                                                        \
    (__builtin_constant_p( u32Reg ) && !((u32Reg) & 0x01) && ((u32Reg) < 0x4000) ?      \
        RIU[u32Reg] : __MDrv_Read2Byte( u32Reg ))

#define MDrv_Read3Byte( u32Reg )    __MDrv_Read3Byte( u32Reg )

#define MDrv_Read4Byte( u32Reg )                                                        \
    ({                                                                                  \
    U32 value;                                                                          \
    if ( __builtin_constant_p( u32Reg ) && !((u32Reg) & 0x01)  )    \
    {                                                                                   \
        value = ((U32)RIU[(u32Reg) + 2] << 16) | RIU[u32Reg];                                \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        value = __MDrv_Read4Byte( u32Reg );                                             \
    }                                                                                   \
    value;                                                                              \
    })

#define MDrv_ReadRegBit( u32Reg, u8Mask )                                               \
        (RIU8[(u32Reg) * 2 - ((u32Reg) & 1)] & (u8Mask))

#define MDrv_WriteRegBit( u32Reg, bEnable, u8Mask )                                     \
    MST_MACRO_START                                                                     \
    U32  u32Regx = ((u32Reg) * 2) - ((u32Reg) & 1);                                      \
    RIU8[u32Regx] = (bEnable) ? (RIU8[u32Regx] |  (u8Mask)) :                           \
                                (RIU8[u32Regx] & ~(u8Mask));                            \
    MST_MACRO_END

#define MDrv_WriteByte( u32Reg, u8Val )                                                 \
    MST_MACRO_START                                                                     \
    if ( __builtin_constant_p( u32Reg )  )                          \
    {                                                                                   \
        RIU8[((u32Reg) * 2) - ((u32Reg) & 1)] = u8Val;                                  \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        __MDrv_WriteByte( (U32)u32Reg, u8Val );                                              \
    }                                                                                   \
    MST_MACRO_END

#define MDrv_Write2Byte( u32Reg, u16Val )                                               \
    MST_MACRO_START                                                                     \
    if ( __builtin_constant_p( u32Reg )  )                          \
    {                                                                                   \
        if ( ((u32Reg) & 0x01) )                                                        \
        {                                                                               \
            RIU8[((u32Reg) * 2) - 1] = (U8)((u16Val));                                  \
            RIU8[((u32Reg) + 1) * 2] = (U8)((u16Val) >> 8);                             \
        }                                                                               \
        else                                                                            \
        {                                                                               \
            RIU[u32Reg] = u16Val;                                                       \
        }                                                                               \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        __MDrv_Write2Byte( (U32)u32Reg, u16Val );                                            \
    }                                                                                   \
    MST_MACRO_END

#define MDrv_Write3Byte( u32Reg, u32Val )   __MDrv_Write3Byte( u32Reg, u32Val )

#define MDrv_Write4Byte( u32Reg, u32Val )                                               \
    MST_MACRO_START                                                                     \
    if ( __builtin_constant_p( u32Reg ) && !((u32Reg) & 0x01)  )    \
    {                                                                                   \
        RIU[u32Reg] = (U16)(u32Val);                                                    \
        RIU[(u32Reg) + 2] = (U16)((u32Val) >> 16);                                      \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        __MDrv_Write4Byte( u32Reg, u32Val );                                            \
    }                                                                                   \
    MST_MACRO_END

#define MDrv_WriteByteMask( u32Reg, u8Val, u8Msk )                                      \
    MST_MACRO_START                                                                     \
    U32 u32Reg8 = ((u32Reg) * 2) - ((u32Reg) & 1);                                      \
    RIU8[u32Reg8] = (RIU8[u32Reg8] & ~(u8Msk)) | ((u8Val) & (u8Msk));                   \
    MST_MACRO_END





#define MDrv_WriteByte32( u32Reg, u8Val )                                                 \
    MST_MACRO_START                                                                     \
    if ( __builtin_constant_p( u32Reg )  )                          \
    {                                                                                   \
        RIU8[((u32Reg) * 2) - ((u32Reg) & 1)] = u8Val;                                  \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        __MDrv_WriteByte( u32Reg, u8Val );                                              \
    }                                                                                   \
    MST_MACRO_END
void SetBdmaSrcDstDevice( U8 SrcDEv,U8 SrcDataWidth, U8 DstDEv , U8 DstDataWidth)
{
   MDrv_Write2Byte( (BDMA_REG_BASE+0x04) ,  ( (DstDEv|DstDataWidth)  <<8) |(SrcDataWidth|SrcDEv) );

   //Both with 1 byte data width for FPGA test
}
void Copy2MIU(U32 dst, U32 src, U32 size)
{
    //printf("SPI %lx -> MIU %lx  length: %lx\n", spi_addr, miu_addr, length);




    while ( RIU8[(BDMA_REG_BASE + 0x02)*2]  & 0x01);   ///queue ==0?
    MDrv_Write4Byte((BDMA_REG_BASE + 0x08 ) , src  );

    MDrv_Write4Byte((BDMA_REG_BASE + 0x0c ),  dst  );

    MDrv_Write4Byte((BDMA_REG_BASE + 0x10 ), (size + 0x0f) & ~0x0fuL);

    MDrv_WriteByte((BDMA_REG_BASE + 0x06),0x00);   //increasing

#define    DRAM_MIU_0                          0x30
#define     DW_8BYTE	                           0x30

    SetBdmaSrcDstDevice( DRAM_MIU_0 , DW_8BYTE, DRAM_MIU_0, DW_8BYTE);
    MDrv_WriteByte( (BDMA_REG_BASE + 0x00), 0x01 );

	while(1) //for(k=0;k<12000;k++)
	{
    	if ( (RIU8[(BDMA_REG_BASE+0x02)*2] & 0x08) == 0x08 )
	    {
		    return;        //pass
	     }
	}
}
#endif
/***********************************************************************/
void board_init (void)
{
	bd_t *bd;
	init_fnc_t **init_fnc_ptr;
	char *s, *e;
	int i;

    // init gd
    // Note: We use a pointer to instead of register
	gd = (gd_t *)CFG_GBL_DATA_OFFSET;

	__asm__ __volatile__("": : :"memory");  /* compiler optimization barrier needed for GCC >= 3.4 */

	memset( (void*)gd, 0, CFG_GBL_DATA_SIZE );

	gd->bd              = (bd_t *)(gd+1);
	gd->baudrate        = CONFIG_BAUDRATE;
	gd->cpu_clk         = CPU_CLOCK_RATE;
    //printf ("<< %s >> LINE %d\r\n", __FILE__, __LINE__);
    //printf ("gd = %p, gd->bd = %p\r\n", gd, gd->bd);

    // init gd->bd
    // Note: gd->bd locates in the end of global data
	bd = gd->bd;
	bd->bi_baudrate	    = CONFIG_BAUDRATE;
	bd->bi_memstart	    = CFG_SDRAM_BASE;
	bd->bi_memsize      = CFG_SDRAM_SIZE;
	bd->bi_flashstart   = CFG_FLASH_BASE;
	bd->bi_flashsize    = CFG_FLASH_SIZE;
    bd->bi_params_count = CFG_GBL_DATA_OFFSET + 0x100;
    bd->bi_boot_params  = CFG_GBL_DATA_OFFSET + 0x104;

    //printf ("%-12s= 0x%08lX\n", "boot_params", (ulong)bd->bi_boot_params);

	for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr) {
		if ((*init_fnc_ptr) () != 0) {
			hang ();
		}
	}



#if 0   // test code
    udelay(1000000);

    {
        unsigned char *TestData = (unsigned char *)0x1100000;
        unsigned char *ptr0     = (unsigned char *)0x1200000;
        ulong test_size         = 0x10000;

        ulong i = 0;

        printf ("Test %p, non-Cache = %X\r\n", TestData, (ulong)TestData + 0x80000000);
        printf ("ptr0 %p, non-Cache = %X\r\n", ptr0, (ulong)ptr0 + 0x80000000);
    #if 0
    {
        ulong value = 0;

        value = mfspr(SPR_SR);
        printf ("SR = 0x%X\r\n", value);

        value = mfspr(SPR_DCCFGR);
        printf ("DCCFGR = 0x%X\r\n", value);

        value = mfspr(SPR_ICCFGR);
        printf ("ICCFGR = 0x%X\r\n", value);

        value = mfspr(SPR_DCCR);
        printf ("DCCR = 0x%X\r\n", value);

        value = mfspr(SPR_ICCR);
        printf ("ICCR = 0x%X\r\n", value);

    }
    #endif

    {
        ulong time1 = get_timer(0);
        ulong time2 = 0;

        memset (TestData, 0x99, test_size);
        memset (ptr0, 0x7C, test_size);

        time2 = get_timer(0);
        printf ("Diff time = %u msec\r\n", (time2 - time1)/336000);

        //Copy2MIU((ulong)ptr0, (ulong)TestData, 35000);
        Copy2MIU((ulong)TestData, (ulong)ptr0, test_size);

        for (i = 0; i<test_size; i++)
        {
            ptr0 += 0x8000000;

            if(TestData[i]!=0x7C)
                printf("error:%x %x\n",i,TestData[i]);

            if (*(TestData + i + 0x80000000) != *(ptr0+i+0x80000000))
                printf("%d >> <%X, %X>\n",i,*(TestData + i + 0x80000000) , *(ptr0+i+0x80000000));

        }
    }
}
//    while(1);

#endif


	/* configure available FLASH banks */
	monitor_flash_len = bss_start - aeonboot_start;
	bd->bi_flashsize = flash_init();

    display_enable_module();

	/* initialize malloc() area */
	mem_malloc_init();
    malloc_bin_reloc();

#if (CONFIG_COMMANDS & CFG_CMD_NAND)
    {
        extern struct list_head mtd_partitions;
        mtd_partitions.next=NULL;
        mtd_partitions.prev=NULL;
    }

	puts ("NAND:  ");
	nand_init();		/* go init the NAND */
#endif

    //printf ("<< %s>> LINE %d\r\n", __FILE__, __LINE__);
	/* relocate environment function pointers etc. */
	env_relocate();
    //printf ("<< %s>> LINE %d\r\n", __FILE__, __LINE__);

	/* IP Address */
	bd->bi_ip_addr = getenv_IPaddr("ipaddr");

	/* board MAC address */
	s = getenv ("ethaddr");

	for (i = 0; i < 6; ++i) {
		bd->bi_enetaddr[i] = s ? simple_strtoul (s, &e, 16) : 0;
		if (s) s = (*e) ? e + 1 : e;
	}


	/** leave this here (after malloc(), environment and PCI are working) **/
	/* Initialize devices */
	devices_init ();

	jumptable_init ();

	/* Initialize the console (after the relocation and devices init) */
	console_init_r ();


	/* Initialize from environment */
	if ((s = getenv ("loadaddr")) != NULL) {
		load_addr = simple_strtoul (s, NULL, 16);
	}

#if (CONFIG_COMMANDS & CFG_CMD_NET)
	if ((s = getenv ("bootfile")) != NULL) {
		copy_filename (BootFile, s, sizeof (BootFile));
	}
#endif	/* CFG_CMD_NET */

#if (CONFIG_COMMANDS & CFG_CMD_NET)
#if defined(CONFIG_NET_MULTI)
	puts ("Net:   ");
#endif
	eth_initialize(gd->bd);
#endif

#if defined(CONFIG_BOARD_LATE_INIT)
	board_late_init ();
#endif

    //printf ("<< %s>> LINE %d\r\n", __FILE__, __LINE__);
	/*
	 * Now that we have DRAM mapped and working, we can
	 * relocate the code and continue running from DRAM.
	 *
	 * Reserve memory at end of RAM for (top down in that order):
	 *  - kernel log buffer
	 *  - protected RAM
	 *  - LCD framebuffer
	 *  - monitor code
	 *  - board info struct
	 */
#ifdef DEBUG_MEM_LAYOUT
	printf("CFG_MONITOR_BASE:       0x%lX\n",       CFG_MONITOR_BASE);
	#if (CONFIG_WITH_NAND_ON_BOARD)
	printf("CFG_ENV_OFFSET:         0x%lX\n",       CFG_ENV_OFFSET);
    #else
	printf("CFG_ENV_ADDR:           0x%lX\n",       CFG_ENV_ADDR);
    #endif
	//printf("CFG_RELOC_MONITOR_BASE: 0x%lx (%d)\n", CFG_RELOC_MONITOR_BASE,  CFG_MONITOR_LEN);

	printf("CFG_MALLOC_BASE:        0x%lX (%d)\n", CFG_MALLOC_BASE,         CFG_MALLOC_LEN);
	printf("CFG_INIT_SP_OFFSET:     0x%lX (%d)\n", CFG_INIT_SP_OFFSET,      CFG_STACK_SIZE);
	printf("CFG_PROM_OFFSET:        0x%lX (%d)\n", CFG_PROM_OFFSET,         CFG_PROM_SIZE);
	printf("CFG_GBL_DATA_OFFSET:    0x%lX (%d)\n", CFG_GBL_DATA_OFFSET,     CFG_GBL_DATA_SIZE);
#endif


	/* main_loop() can return to retry autoboot, if so just run it again. */
	for (;;) {
		main_loop ();
	}

	/* NOTREACHED - no way out of command loop except booting */
}


/***********************************************************************/

void hang (void)
{
	disable_interrupts ();
	puts("### ERROR ### Please reset board ###\n");
	for (;;);
}

