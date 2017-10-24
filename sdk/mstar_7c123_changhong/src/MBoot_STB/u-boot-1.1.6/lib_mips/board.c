/*
 * (C) Copyright 2003
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
#include <version.h>
#include <net.h>
#include <environment.h>
#include <linux/list.h>

#include <sys/common/MsTypes.h>
#include <sys/common/MsDevice.h>
#include <sys/common/c_riubase.h>
#include <sys/common/MsIRQ.h>
#include <sys/common/MsOS.h>
#include <sys/common/c_riubase.h>
#include <sys/drv/drvUART.h>
#include <sys/drv/drvMIU.h>
#include <sys/drv/drvGPIO.h>
#include <sys/drv/drvISR.h>
#include <sys/drv/drvMMIO.h>
#include <sys/drv/drvSEM.h>
#include <sys/drv/drvPM.h>
#include <configs/uboot_module_config.h>

DECLARE_GLOBAL_DATA_PTR;
gd_t *gd = NULL;
bd_t *bd = NULL;

#if ( ((CFG_ENV_ADDR+CFG_ENV_SIZE) < CFG_MONITOR_BASE) || \
      (CFG_ENV_ADDR >= (CFG_MONITOR_BASE + CFG_MONITOR_LEN)) ) || \
    defined(CFG_ENV_IS_IN_NVRAM)
#define	TOTAL_MALLOC_LEN	(CFG_MALLOC_LEN + CFG_ENV_SIZE)
#else
#define	TOTAL_MALLOC_LEN	CFG_MALLOC_LEN
#endif

#undef DEBUG
extern int timer_init(void);
extern int incaip_set_cpuclk(void);

#if (CONFIG_COMMANDS & CFG_CMD_NAND)
void nand_init (void);
#endif

extern ulong uboot_end_data;
extern U32 __heap;
extern U32 __heap_end;

ulong monitor_flash_len=0;
const char version_string[] = U_BOOT_VERSION" (" __DATE__ " - " __TIME__ ")";
static char *failed = "*** failed ***\n";

extern void MDrv_Timer_ISR_Register(void);
extern void set_miu4boot(void);
extern void msIR_Initialize(U8 irclk_mhz);

unsigned long MS_RIU_MAP;

void MDrv_Timer_Init(void) // Now support mode 1
{
    gTimerCount0 = 0;
    gTimerDownCount0 = 0;
    gTimerDownCount1 = 0;
    gu8100msTH = 0;
}

#if ((defined(CONFIG_TITANIA4) || defined(CONFIG_TITANIA12) || defined(CONFIG_URANUS4) || defined(CONFIG_TITANIA8) || defined(CONFIG_AMBER2) || defined(CONFIG_AMBER2) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_KRONUS) || defined(CONFIG_KAPPA)|| defined(CONFIG_KRITI) || defined(CONFIG_KELTIC) || defined(CONFIG_KAISERIN) || defined(CONFIG_KERES)) && ((CONFIG_COMMANDS & CFG_CMD_STANDBY)||(CONFIG_COMMANDS & CFG_CMD_SECURE)))
#include <sys/drv/drvMBX.h>
#include <sys/drv/drvBDMA.h>
#include <msKeypad.h>
#include <secure/apiSecureBoot.h>

extern BOOLEAN msAPI_IR_Initialize(void);
extern BOOLEAN msAPI_KeyPad_Initialize(void);
/******************************************************************************/
/// API for Secure Boot init Mailbox:
/// Secure_MailBox_Init
/// @param \b IN:
/// @param \b IN:
/// @return BOOLEAN: Success or not
/// - TRUE: Success
/// - FALSE: Fail
/******************************************************************************/
MS_BOOL MApi_MBX_Init(void)
{
    MBX_CPU_ID eHKCPU;
    MS_U32 u32TimeoutMillSecs = 1500;

    eHKCPU = E_MBX_CPU_MIPS;
    if( E_MBX_SUCCESS != MDrv_MBX_Init(eHKCPU, E_MBX_ROLE_HK, u32TimeoutMillSecs))
    {
        printf("Error> MBX init failed !!\n");
        while(1);
    }
    else
    {
        MDrv_MBX_Enable(TRUE);
        return 1;
    }
}

void msAPI_Power_PowerDown_EXEC(void)
{
    MBX_Result mbxResult;
    PM_Result pmResult;
    static PM_PowerDownCfg PmPowerDownCfg;

    MDrv_BDMA_SetSPIOffsetForMCU();

    mbxResult = MDrv_MBX_RegisterMSG(E_MBX_CLASS_PM_WAIT, 6);
    if(E_MBX_SUCCESS != mbxResult)
    {
        printf("E_MBX_CLASS_PM_WAIT - MBX register msg error\n");
    }

    //MDrv_MBX_SetDbgLevel(MBX_DBG_LEVEL_ALL);
    //MDrv_PM_SetDbgLevel(E_PM_DBGLV_ALL);

    // Initialize IR before power down
    msAPI_IR_Initialize();

    // Initialize SAR before power down
    msAPI_KeyPad_Initialize();

    static PM_WakeCfg PmWakeCfg =
    {
        .bPmWakeEnableIR = TRUE,
        .bPmWakeEnableSAR = TRUE,
        .bPmWakeEnableGPIO0 = FALSE,
        .bPmWakeEnableGPIO1 = FALSE,
        .bPmWakeEnableUART1 = FALSE,
        .bPmWakeEnableSYNC = TRUE,
        .bPmWakeEnableESYNC = FALSE,

        .bPmWakeEnableRTC0 = TRUE,
        .bPmWakeEnableRTC1 = FALSE,
        .bPmWakeEnableDVI0 = FALSE,
        .bPmWakeEnableDVI2 = FALSE,
        .bPmWakeEnableCEC = FALSE,
        .bPmWakeEnableAVLINK = FALSE,

        .u8PmWakeIR =
        {   //IR wake-up key define
            0x46, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF
        }
    };
    printf("PM_Init...\n");
    pmResult= MDrv_PM_Init((PM_WakeCfg*)&PmWakeCfg);


    PmPowerDownCfg.u8WakeAddress = E_PM_WAKE_LAST;
    PmPowerDownCfg.u8PowerDownMode = E_PM_STANDBY;//E_PM_STANDBY;//E_PM_SLEEP;

    printf("\n\r==== Enter Standby Mode !! ====\n",pmResult);
    MDrv_PM_PowerDown(&PmPowerDownCfg);

    while(1);
}
#endif

static int init_func_ram (void)
{
#ifdef	CONFIG_BOARD_TYPES
	int board_type = gd->board_type;
#else
	int board_type = 0;	/* use dummy arg */
#endif
#if (!CONFIG_MSTAR_SILENT)
#if defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA) || defined(CONFIG_KELTIC)|| defined(CONFIG_KRITI) || defined(CONFIG_KERES)
#else
	printf ("DRAM:  ");
#endif
#endif

	if ((gd->ram_size = initdram (board_type)) > 0) {
#if (!CONFIG_MSTAR_SILENT)
#if defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA) ||  defined(CONFIG_KELTIC)|| defined(CONFIG_KRITI) || defined(CONFIG_KERES)
#else
		printf("%d MBytes\n", (gd->ram_size/0x100000));
#endif
#endif
		return (0);
	}
	printf (failed);
	return (1);
}

#if (!CONFIG_MSTAR_SILENT)
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
    if(ENABLE_DISPLAY_OSD == 1)
        printf("OSD ");
    if(ENABLE_MODULE_MICOM == 1)
        printf("MICOM ");
    printf("%s ",ENV_CFG);
    printf("\n");
    return 0;
}
#endif

static int display_banner(void)
{
#if (!CONFIG_MSTAR_SILENT)
	printf ("\n\n%s\n\n", version_string);
#endif
	return (0);
}

#if 0
static void display_flash_config(ulong size)
{
	printf ("Flash: ");
	print_size (size, "\n");
}
#endif

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
typedef int (init_fnc_t) (void);

init_fnc_t *init_sequence[] = {
	timer_init,
	env_init,		/* initialize environment */
#ifdef CONFIG_INCA_IP
	incaip_set_cpuclk,	/* set cpu clock according to environment variable */
#endif
	console_init_f,
	display_banner,		/* say that we are here */
	checkboard,
	init_func_ram,
	NULL,
};

static void console_init(void)
{
    //bit 12 UART Enable, bit 11 UART RX Enable
    *(volatile U32*)(0xbf000000+(0x0709*4)) |= 0x0800;

    // Switch UART0 to PIU UART 0
    // Clear UART_SEL0
    *(volatile U32*)(0xbf200000+(0x0F53*4)) &= ~0x000F;
    // UART_SEL0 --> PIU UART0
    *(volatile U32*)(0xbf200000+(0x0F53*4)) |= 0x0004;
    // <-------

    // UART mode
    *(volatile U32*)(0xbf200000+(0x0F6E*4)) &= ~0x0F00;

    //enable reg_uart_rx_enable;
    *(volatile U32*)(0xbf200000+(0x0F01*4)) |= (1<<10);

    MDrv_UART_Init(E_UART_PIU_UART0, CONFIG_BAUDRATE);

}

void writeBackToMem(U32 u32ptr, U32 u32len)
{
    MsOS_Dcache_Flush(u32ptr, u32len);
    MsOS_FlushMemory();
    printf("u32ptr = 0x%x, u32len = 0x%x\n", u32ptr, u32len);
}

void start(void)
{
    // Added by coverity_0210
#if defined(CONFIG_KRONUS)||defined(CONFIG_KAISERIN)||defined(CONFIG_KAPPA)|| defined(CONFIG_KELTIC)||defined(CONFIG_KRITI)|| defined(CONFIG_KERES)
    U8 zero_array[8] = {0, 0, 0, 0, 0, 0, 0, 0};
#else
    U8 zero_array[4] = {0, 0, 0, 0};
#endif
    U32 u32Tmp;
#if defined(CONFIG_KAISERIN)|| defined(CONFIG_KELTIC)|| defined(CONFIG_KERES)
    U32 pageshift =0 ;
#endif

    extern U8  _readonly_start[];
    extern U8  _readonly_end[];

    //extern void uart_init(U32 aeon_clock, U32 uart_clock);    //T3 FPGA test
    extern int  main(void);
    extern void MDrv_Pad_Init(void);
    extern void MDrv_ISR_Init(void);
    extern void mhal_stack_init(void *stack);

    MDrv_MMIO_GetBASE((MS_U32 *)&MS_RIU_MAP, (MS_U32 *)&u32Tmp, MS_MODULE_PM);
#if (1)//PM_LOCK_SUPPORT == 1)
    MDrv_SEM_Init();
#endif
    MDrv_MIU_SetIOMapBase();

    // Modified by coverity_0210
#if defined(CONFIG_KAISERIN)||defined(CONFIG_KELTIC)|| defined(CONFIG_KERES)
    u32Tmp = MsOS_VA2PA((U32)_readonly_end) - MsOS_VA2PA((U32)_readonly_start); // Size
    MDrv_MIU_ProtectAlign((MS_U32 *)&pageshift);
    MDrv_MIU_Protect(0, zero_array, ALIGN(MsOS_VA2PA((U32)_readonly_start), pageshift), ALIGN(MsOS_VA2PA((U32)_readonly_start) + u32Tmp -1, pageshift), ENABLE);
#else
    u32Tmp = MsOS_VA2PA((U32)_readonly_end);
    MDrv_MIU_Protect(0, zero_array, MsOS_VA2PA((U32)_readonly_start), u32Tmp, ENABLE);
#endif

#if (defined(CONFIG_KELTIC)|| defined(CONFIG_KERES))
#if defined(CONFIG_MSTAR_MMAP_128MB)
    MsOS_CPU_SetEBASE(0x75F0000);
#elif defined(CONFIG_MSTAR_MMAP_64MB)
    MsOS_CPU_SetEBASE(0x35F0000);
#elif defined(CONFIG_MSTAR_MMAP_32MB)
    MsOS_CPU_SetEBASE(0x15F0000);
#else
    MsOS_CPU_SetEBASE(0x35F0000);
#endif
#else
    MsOS_CPU_SetEBASE(0x75F0000);
#endif

    //MDrv_Pad_Init();

#if ((CONFIG_COMMANDS & CFG_CMD_BOOTLOGO) || (CONFIG_COMMANDS & CFG_CMD_EEPROM))
    mdrv_gpio_init();
#endif

    console_init();
    //printf("@@ %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    static PM_RtcParam PmRtcParam =
    {
        .u8PmRtcIndex = 0,
        .u32RtcCtrlWord = 12000000ul,
        .u32RtcSetMatchCounter = 0,
        .u32RtcGetMatchCounter = 0,
        .u32RtcSetCounter = 0,
        .u32RtcGetCounter = 0
    };
    MDrv_PM_RtcInit((PM_RtcParam*)MsOS_VA2PA((MS_U32)&PmRtcParam));

#if (!CONFIG_MSTAR_SILENT)
    printf("\n\rHello U-Boot\n");
#endif
    {
       U32 u32FreeSize;
       u32FreeSize = ((unsigned int)&__heap_end) - ((unsigned int)&__heap);
       //printf("heap_start = 0x%x, heap_end = 0x%x \n",&__heap, &__heap_end );
       if ((&__heap_end) <= (&__heap))
       {
           printf("## ERROR!! MMAP lyout for CPU code is not enough!!!\n");
       }
       else if (u32FreeSize < 0x4B000)
       {
           printf("## heap size is %dK under 300K!!\n", (int)u32FreeSize/1024);
       }
    }
    //MDrv_IIC_Init();

    {
        extern void ProcessSysTrap( MHAL_SavedRegisters *pHalReg, MS_U32 vector );
        extern void ProcessEx0Isr( MHAL_SavedRegisters *pHalReg, MS_U32 vector );
        extern void ProcessEx1Isr( MHAL_SavedRegisters *pHalReg, MS_U32 vector );
        extern void ProcessTimer0Isr( MHAL_SavedRegisters *pHalReg, MS_U32 vector );

        //MsOS_CPU_AttachException( E_EXCEPTION_TRAP, ProcessSysTrap, E_EXCEPTION_TRAP );

        MsOS_CPU_AttachInterrupt( E_INTERRUPT_TICK_TIMER, ProcessTimer0Isr, 0 );
        MsOS_Init();
        MDrv_ISR_Init();
    }

    MDrv_Timer_ISR_Register();
    MDrv_Timer_Init();
    MsOS_CPU_EnableInterrupt();

#if (defined(CONFIG_TITANIA4) || defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAPPA) || defined(CONFIG_KRITI)|| defined(CONFIG_KELTIC) || defined(CONFIG_KAISERIN) || defined(CONFIG_KERES)) && (CONFIG_COMMANDS & CFG_CMD_STANDBY)
    if(MDrv_ReadByte(0x0E6F) == 0xFF)
    {
        MApi_MBX_Init();
        msAPI_Power_PowerDown_EXEC();
    }
#endif

    set_miu4boot();

#if (defined(CONFIG_TITANIA8) || defined(CONFIG_AMBER2) || defined(CONFIG_TITANIA12))
    msIR_Initialize(12);
#endif

    // jump to board_init() should never return
    board_init();
    while(1);

}

void board_init(void)
{
	init_fnc_t **init_fnc_ptr;
	ulong addr, size = 0x800000;
    char *s, *e;
    int i;
#ifndef CFG_ENV_IS_NOWHERE
	extern char * env_name_spec;
#endif
#ifdef CONFIG_PURPLE
	void copy_code (ulong);
#endif

    gd = (gd_t *)malloc(sizeof(gd_t));
	memset (gd, 0, sizeof (gd_t));

    bd = (bd_t *)malloc(sizeof(bd_t));
	memset (bd, 0, sizeof (bd_t));
    gd->bd = bd;

	for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr) {
		if ((*init_fnc_ptr)() != 0) {
			hang ();
		}
	}

    addr = CFG_MONITOR_BASE;        //static mapping u-boot address to TEXT_BASE

#if (!CONFIG_MSTAR_SILENT)
    printf ("U-Boot is running at DRAM 0x%08lx\n", addr);
#endif
	bd->bi_boot_params = (U32)malloc(CFG_BOOTPARAMS_LEN);
	memset ((void *)(bd->bi_boot_params), 0, CFG_BOOTPARAMS_LEN);

	/* Save local variables to board info struct
	 */
	bd->bi_memstart	= CFG_SDRAM_BASE;	/* start of  DRAM memory */
	bd->bi_memsize	= gd->ram_size;		/* size  of  DRAM memory in bytes */
	bd->bi_baudrate	= gd->baudrate;		/* Console Baudrate */

	/* On the purple board we copy the code in a special way
	 * in order to solve flash problems
	 */
#ifdef CONFIG_PURPLE
	copy_code(addr);
#endif

	gd->flags |= GD_FLG_RELOC;	/* tell others: relocation done */

#if (CONFIG_SILENT_CONSOLE)
#if (!CONFIG_MSTAR_SILENT)
    printf("Enable Silent Mode\n");
#endif
    gd->flags |= GD_FLG_SILENT;
#endif

    gd->reloc_off = 0;

	monitor_flash_len = (ulong)&uboot_end_data - addr;

	/* there are some other pointer constants we must deal with */
#ifndef CFG_ENV_IS_NOWHERE
	env_name_spec += gd->reloc_off;
#endif

#if (!CONFIG_MSTAR_SILENT)
    display_enable_module();
#endif
	bd = gd->bd;
	bd->bi_flashstart = CFG_FLASH_BASE;
	bd->bi_flashsize = size;
#if CFG_MONITOR_BASE == CFG_FLASH_BASE
	bd->bi_flashoffset = monitor_flash_len;	/* reserved area for U-Boot */
#else
	bd->bi_flashoffset = 0;
#endif

    {
        extern struct list_head mtd_partitions;
        mtd_partitions.next=NULL;
        mtd_partitions.prev=NULL;
    }

    flash_init();

#if (CONFIG_COMMANDS & CFG_CMD_NAND)
	nand_init();		/* go init the NAND */
#endif

#if (CONFIG_COMMANDS & CFG_CMD_ENV_IN_UBI)
    extern int ubi_init_simple(void);
    ubi_init_simple();
#endif

	/* relocate environment function pointers etc. */
	env_relocate();

	/* board MAC address */
	s = getenv ("ethaddr");
	for (i = 0; i < 6; ++i) {
		bd->bi_enetaddr[i] = s ? simple_strtoul (s, &e, 16) : 0;
		if (s)
			s = (*e) ? e + 1 : e;
	}

	/* IP Address */
	bd->bi_ip_addr = getenv_IPaddr("ipaddr");

#if defined(CONFIG_PCI)
	/*
	 * Do pci configuration
	 */
	pci_init();
#endif

/** leave this here (after malloc(), environment and PCI are working) **/
	/* Initialize devices */
	devices_init ();

	jumptable_init ();

	/* Initialize the console (after the relocation and devices init) */
	console_init_r ();
/** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **/

	/* Initialize from environment */
	if ((s = getenv ("loadaddr")) != NULL) {
		load_addr = simple_strtoul (s, NULL, 16);
	}

#if (CONFIG_COMMANDS & CFG_CMD_NET)
	if ((s = getenv ("bootfile")) != NULL) {
		copy_filename (BootFile, s, sizeof (BootFile));
	}
#endif	/* CFG_CMD_NET */

#if defined(CONFIG_MISC_INIT_R)
	/* miscellaneous platform dependent initialisations */
	misc_init_r ();
#endif

#if (CONFIG_COMMANDS & CFG_CMD_NET)
#if defined(CONFIG_NET_MULTI)
#if (!CONFIG_MSTAR_SILENT)
	printf ("Net:   ");
#endif
#endif
	eth_initialize(gd->bd);
#endif

	/* main_loop() can return to retry autoboot, if so just run it again. */
	for (;;) {
		main_loop ();
	}

	/* NOTREACHED - no way out of command loop except booting */
}

void hang (void)
{
	printf ("### ERROR ### Please RESET the board ###\n");
	for (;;);
}

