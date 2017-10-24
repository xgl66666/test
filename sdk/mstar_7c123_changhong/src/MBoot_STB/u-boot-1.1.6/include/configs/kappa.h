////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __CONFIG_H
#define __CONFIG_H
#include <cmd_confdefs.h>

#include "../../../sboot/include/autoconf.h"
#include "../../../sboot/board/kappa/Board.h"
#include <configs/uboot_board_config.h>
#include "cmd_config.h"

#define MS_BOARD BOARD_NAME

#if (CONFIG_COMMANDS & CFG_CMD_NAND)
#define CONFIG_WITH_NAND_ON_BOARD      1
#else
#define CONFIG_WITH_FLASH_ON_BOARD     1
#endif
#define CONFIG_MIPS32		1	/* MIPS 4Kc CPU core	*/
#define CONFIG_KAPPA		1


// Use unsigned long to avoid compile warning in Net.c
#define CPU_CLOCK_RATE	    CPU_CLOCK   /* 552 MHz clock for the MIPS core */

#define CPU_TCLOCK_RATE     16588800	/* 16.5888 MHz for TClock */

#define CONFIG_CONS_INDEX	1
#define CONFIG_BOOTDELAY	0	/* autoboot after 0 seconds	*/

#define CONFIG_BAUDRATE		115200  //9600 //19200 //38400 //57600
#define CFG_BAUDRATE_TABLE	{ 115200   }

#define CONFIG_TIMESTAMP		/* Print image info with timestamp */
#define CONFIG_BZIP2            /* Enable bzip2 decomp. */

#if (CONFIG_WITH_NAND_ON_BOARD | CONFIG_WITH_FLASH_ON_BOARD)
#define CONFIG_LZO              /* Enable lzo decomp. */
#endif

#ifdef CONFIG_LZO
#define CONFIG_JFFS2_LZO_LZARI
#endif

#define CONFIG_PREBOOT	"echo;" \
	"echo Type \"help\" for more commands."

#undef	CONFIG_BOOTARGS


/*
 * Miscellaneous configurable options
 */
#define CFG_LONGHELP				        /* undef to save memory	     */
#define CFG_PROMPT		    "<< MStar >># " /* Monitor Command Prompt    */
#define CFG_CBSIZE		    512 // 256		        /* Console I/O Buffer Size   */ // REVIEW: please sync with COMMAND_LINE_SIZE @ kernel, 384 should be enough (for "bootargs" with "mtdparts"), but please check the stack size if using larger size for console buffer
#define CFG_PBSIZE          (CFG_CBSIZE+sizeof(CFG_PROMPT)+16)  /* Print Buffer Size */
#define CFG_MAXARGS		    32		        /* max number of command args*/

#define CFG_MALLOC_LEN		1024*1024

#define CFG_BOOTPARAMS_LEN	128*1024

#define CFG_HZ				(CPU_CLOCK_RATE/2)  //its more like a HZ

#define CFG_SDRAM_BASE		0x80000000

/* default load address */
/* Warning: This address should match the TEXT_BASE in board\titania\config.mk */
#define CFG_LOAD_ADDR		0x80800000

#define CFG_MEMTEST_START	0x80000000
#define CFG_MEMTEST_END		0x80800000

/*-----------------------------------------------------------------------
 * NAND FLASH related
 */

/* NAND FLASH Layout Diagram

   +---------------------+ TOP 0x4000000 ---------+
   + BBT 1 = 2pages      | Len=0x4000             |
   +        -------------|                        |
   +    30pages reserved |                        |
   +---------------------+ 0x3ffc000              |
   + BBT Mirror = 2pages | Len=0x4000             | => Total 5blocks = 16K*5
   +        -------------|                        |      = 80Kbytes
   +    30pages reserved |                        |
   +---------------------+ 0x3ff8000              |
   + RESERVED 2blocks    | Len=0x4000             |
   +                     |                        |
   +---------------------+ 0x3ff0000              |
   + U-BOOT ENV          | Len=0x4000             |
   +---------------------| 0x3fec000 -------------+
   +                     |
   +                     |
   +---------------------+ BOTTOM 0

   Mim. BBT size = 4096blocks*2bits = 0x8Kbits = 1Kbytes = 2pages
   2bits defined by /nand_bbt.c/bbt_descr.options |= NAND_BBT_2BITS

*/


#define CFG_NAND_BASE       0x00000000
#define CFG_MAX_NAND_DEVICE 1
#define CONFIG_JFFS2_CMDLINE
#if (CONFIG_WITH_NAND_ON_BOARD)
#define CONFIG_JFFS2_NAND   1
#define MTDIDS_DEFAULT      {"nand0=edb64M-nand"}
#define MTDPARTS_DEFAULT    {"mtdparts=edb64M-nand:256k(nand_partition_tbl),256k(linux_boot_param),8m(kernel),512k(chakra_boot_param),8m(charkra_bin),8m(rootfs),39m(usr)"} // {"mtdparts=edb64M-nand:512k(uboot),5m(kernel0),5m(kernel1),2m(rootfs),51m(app)"} // {"mtdparts=edb64M-nand:8m(kernel),8m(rootfs),48m(user)"}
#else
#define MTDIDS_DEFAULT      {"nor0=edb8M-nor"}
#define MTDPARTS_DEFAULT    {"mtdparts=edb8M-nor:0x40000(NPT),0x40000(BL0),0x80000(BL1),0x180000(kernel),0x280000(rootfs),0x10000(conf),-(NA)"}
#endif
//#define MTDPARTS_DEFAULT    {"nand0:512k@0(uboot)"}
#define CFG_UBI_OFFSET      0x7500000  //UBI size
#define CFG_UBI_SIZE           0x580000  // the total size of partitions before ubi
#if (CONFIG_WITH_NAND_ON_BOARD)
#define CONFIG_JFFS2_DEV
#else
#endif
#define NAND_MAX_FLOORS     1
#define NAND_MAX_CHIPS      1
#define CFG_NANDDRV_DEBUG   0  /* 0:Disable/1:Enable driver-level debug message */

#if ((CONFIG_COMMANDS & CONFIG_ENV_IN_NAND) || (CONFIG_COMMANDS & CFG_CMD_ENV_IN_UBI))
#define CFG_ENV_IS_IN_NAND      1
#define CFG_ENV_OFFSET          cfg_env_offset
#define CFG_ENV_SIZE            0x20000
/* the number of block allocated to environment, this should be larger than CFG_ENV_SIZE */
#define CFG_ENV_BLOCK_NUM       8
#elif (CONFIG_COMMANDS & CONFIG_ENV_IN_SERIAL)
#define CFG_ENV_IS_IN_SPIFLASH  1
#define CFG_ENV_SIZE		    CONFIG_CFG_SPI_ENV_SIZE
#define CFG_ENV_ADDR		    cfg_env_offset
#define CFG_ENV_ADDR_BAK        (CFG_ENV_ADDR + CFG_ENV_SIZE)
#else
#error "Incorrect env setting!!"
#endif

#if ((CONFIG_COMMANDS & CFG_CMD_USB) || (CONFIG_COMMANDS & CONFIG_OAD))
#if (CONFIG_COMMANDS & CONFIG_INFO_IN_UBI_FILE)
#define CFG_INFO_IS_IN_UBI_FILE 1
#elif (CONFIG_COMMANDS & CONFIG_INFO_IN_NAND_ENV)
#define CFG_INFO_IS_IN_NAND_ENV  1
#elif (CONFIG_COMMANDS & CONFIG_INFO_IN_SERIAL_ENV)
#define CFG_INFO_IS_IN_SPI_ENV  1
#else
#error "Incorrect info exchange place setting!!"
#endif
#endif

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CFG_MAX_FLASH_BANKS	1           /* max number of memory banks */
#define CFG_MAX_FLASH_SECT	(128)       /* max number of sectors on one chip */
#define PHYS_FLASH_1		0xbfc00000  /* Flash Bank #1 */

/* The following #defines are needed to get flash environment right */
#define CFG_MONITOR_BASE	TEXT_BASE
#define CFG_MONITOR_LEN		(192 << 10)

#define CFG_INIT_SP_OFFSET	0x400000

#define CFG_FLASH_BASE		PHYS_FLASH_1

/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT	(20 * CFG_HZ)   /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(2 * CFG_HZ)    /* Timeout for Flash Write */


//#define CFG_DIRECT_FLASH_TFTP

#define CONFIG_NR_DRAM_BANKS	1
#define PHYS_SDRAM_1            0x80000000
#define PHYS_SDRAM_1_SIZE       0x08000000

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CFG_DCACHE_SIZE		4096
#define CFG_ICACHE_SIZE		4096
#define CFG_CACHELINE_SIZE	16

/*-----------------------------------------------------------------------
 * Serial Configuration
 */

/*----------------------------------------------------------------------- */
/*
 * USB configuration
 */
#if (CONFIG_COMMANDS & CFG_CMD_USB)
#define     CONFIG_USB_EHCI
#define     CONFIG_USB_HOST0
#define     CONFIG_USB_STORAGE
#else
#undef     CONFIG_USB_EHCI
#undef     CONFIG_USB_HOST0
#undef     CONFIG_USB_STORAGE
#endif

/* Partitions */
#define     CONFIG_DOS_PARTITION

#if (CONFIG_COMMANDS & CFG_CMD_NET)
#define     CONFIG_NET_MULTI
#endif
//#define     BOOT_AUTO_RUN

#ifdef BOOT_AUTO_RUN
#define	CONFIG_EXTRA_ENV_SETTINGS	                       \
	    "bootp  \0"	                                       \
	    "bootdelay=3           \0"                         \
	    "ipaddr=172.16.88.66   \0"                         \
	    "serverip=172.16.88.17 \0"                         \
	    "bootcmd=usb start;tftp 80100000 uranus.bin \0"    \
	    ""
#endif

#define CONFIG_SYS_64BIT_VSPRINTF		1
#define CONFIG_MTD_PARTITIONS

#endif /* __CONFIG_H */
