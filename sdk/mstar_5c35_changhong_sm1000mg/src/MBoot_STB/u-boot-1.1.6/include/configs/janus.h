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
#include "../../../sboot/inc/janus/board/Board.h"
#include <asm/spr_defs.h>
#include "cmd_config.h"

#define DEBUG_MEM_LAYOUT    1

#define MS_BOARD BOARD_NAME

#if (CONFIG_COMMANDS & CFG_CMD_NAND)
#define CONFIG_WITH_NAND_ON_BOARD       1
#else
#define CONFIG_WITH_NAND_ON_BOARD       0
#endif

#define CONFIG_JANUS            1

// Use unsigned long to avoid compile warning in Net.c
// Note: Be careful that CPU timer will easily overflow 2^32/384MHz ~= 13.76 sec
#if (ENABLE_MCU_AEONR2)
#define CPU_CLOCK_RATE     384000000UL             /* 384 MHz */
#else
#define CPU_CLOCK_RATE     172800000UL             /* 172.8 MHz */
#endif
#define DIVISOR_FOR_TIMER       2                       // (CPU_CLOCK_RATE)/(DIVISOR_FOR_TIMER) cycles to timer overflow and reset
#define CFG_HZ                  (CPU_CLOCK_RATE)   // it should be CPU_CLOCK_RATE but we follow mips setting

#if ( (CPU_CLOCK_RATE/DIVISOR_FOR_TIMER) >= SPR_TTMR_PERIOD)
#error  "WRONG DIVISOR_FOR_TIMER Setting"
#endif


#define CONFIG_CONS_INDEX       1
#define CONFIG_BOOTDELAY        3               /* autoboot after 5 seconds    */

/*
 * Serial console configuration
 */
#define CONFIG_BAUDRATE         38400
#define CFG_BAUDRATE_TABLE      { 115200 }

#define CONFIG_TIMESTAMP                        /* Print image info with timestamp */
//#define CONFIG_BZIP2                          /* Enable bzip2 decomp. */

#if (CONFIG_WITH_NAND_ON_BOARD)
#define CONFIG_LZO                              /* Enable lzo decomp. */
#endif

#ifdef CONFIG_LZO
#define CONFIG_JFFS2_LZO_LZARI
#endif

#define CONFIG_PREBOOT    "echo;" "echo Type \"help\" for more commands."

#undef CONFIG_BOOTARGS


/*-----------------------------------------------------------------------
 * Miscellaneous configurable options
 */
#define CFG_LONGHELP                            /* undef to save memory         */
#define CFG_PROMPT          "<< MStar >># "     /* Monitor Command Prompt    */
#define CFG_CBSIZE          384                 /* Console I/O Buffer Size   */ // REVIEW: please sync with COMMAND_LINE_SIZE @ kernel, 384 should be enough (for "bootargs" with "mtdparts"), but please check the stack size if using larger size for console buffer
#define CFG_PBSIZE          (CFG_CBSIZE+sizeof(CFG_PROMPT)+16)  /* Print Buffer Size */
#define CFG_MAXARGS         16                  /* max number of command args*/
#define CFG_BARGSIZE        CFG_CBSIZE          /* Boot Argument Buffer Size    */

//#define CFG_BOOTPARAMS_LEN    128*1024



/*-----------------------------------------------------------------------
 * Memory map
 */

/* DRAM Layout Diagram

   +---------------------+---- 0x00000000 --------+ => CFG_SDRAM_BASE, CFG_RAM_BASE
   +                     |                        |
   +---------------------|     0x00000100 --------+ => CFG_MONITOR_BASE, TEXT_BASE, CFG_LOAD_ADDR
   +                     |                        |
   +                     |                        |
   +                     |                        |
   +---------------------+----- 0x03EE:E000 ------| => CFG_MALLOC_BASE
   +                     |                        |
   + Malloc              | Len=CFG_MALLOC_LEN     |
   +                     |                        |
   +---------------------+----- 0x03FE:E000 ------| => CFG_MALLOC_END
   +                     |                        |
   +                     | Len=CFG_INIT_SP_OFFSET |
   +                     |                        |
   + Stack               +----- 0x03FF:DFFE ------+ => CFG_INIT_SP_OFFSET
   +                     |                        |
   +                     | Len=32                 |
   +                     |                        |
   +---------------------+----- 0x03FF:E000 ------+ => CFG_PROM_OFFSET
   +                     |                        |
   + PROM                | Len=CFG_PROM_SIZE      |
   +                     |                        |
   +---------------------+----- 0x3FFF:FF80 ------+ => CFG_GBL_DATA_OFFSET
   +                     |                        |
   + GLOBAL DATA         | Len=CFG_GBL_DATA_SIZE  |
   +                     |                        |
   +---------------------+----- 0x0400:0000 ------+ => CFG_SDRAM_END, CFG_RAM_END
                              CFG_SDRAM_SIZE

   Mim. BBT size = 4096blocks*2bits = 0x8Kbits = 1Kbytes = 2pages
   2bits defined by /nand_bbt.c/bbt_descr.options |= NAND_BBT_2BITS

*/

#define CFG_SDRAM_BASE      0x00000000          // 0x00000000: cached, 0x80000000: uncached
#define CFG_SDRAM_SIZE      0x04000000          // only use 64MB for chip, 64MB for FPGA
#define CFG_SDRAM_END       (CFG_SDRAM_BASE+CFG_SDRAM_SIZE)

/* Always Run U-Boot from SDRAM */
#define CFG_RAM_BASE        CFG_SDRAM_BASE
#define CFG_RAM_SIZE        CFG_SDRAM_SIZE
#define CFG_RAM_END         CFG_SDRAM_END

#define CFG_GBL_DATA_SIZE   2048                 /* size in bytes reserved for initial data and kernel boot params*/
#define CFG_GBL_DATA_OFFSET (CFG_RAM_END - CFG_GBL_DATA_SIZE)

#define CFG_PROM_SIZE       (8192-CFG_GBL_DATA_SIZE)
#define CFG_PROM_OFFSET     (CFG_GBL_DATA_OFFSET-CFG_PROM_SIZE)

#define CFG_INIT_SP_OFFSET  (CFG_PROM_OFFSET-32)        // QMEM: 0xB0001000
#define CFG_STACK_SIZE      (0x10000-32)                // 64KB - 32B

#define CFG_MALLOC_END      (CFG_INIT_SP_OFFSET-CFG_STACK_SIZE)
#define CFG_MALLOC_BASE     (CFG_MALLOC_END-CFG_MALLOC_LEN)

/* The following #defines are needed to get flash environment right */
#define CFG_LOAD_ADDR       TEXT_BASE           /* default load address */
#define CFG_MONITOR_BASE    TEXT_BASE
#define CFG_MONITOR_LEN     (256 << 10)         /* Reserve 192 kB for Monitor   */
#define CFG_MALLOC_LEN      (1024 << 10)         /* Reserve 1024 kB for malloc()  */
#define CFG_BOOTMAPSZ       (8 << 20)           /* Initial Memory map for Linux */

#define CFG_MEMTEST_START   0x00000000
#define CFG_MEMTEST_END     0x00800000

#if 0
#define CONFIG_NR_DRAM_BANKS    1
#define PHYS_SDRAM_1            0x80000000
#define PHYS_SDRAM_1_SIZE       0x08000000
#endif


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
#define CFG_UBI_OFFSET      0x7500000  //UBI size
#define CFG_UBI_SIZE           0x580000  // the total size of partitions before ubi
#define CONFIG_JFFS2_DEV
#define NAND_MAX_FLOORS     1
#define NAND_MAX_CHIPS      1
#define CFG_NANDDRV_DEBUG   0  /* 0:Disable/1:Enable driver-level debug message */


/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CFG_MAX_FLASH_BANKS     1           /* max number of memory banks */
#define CFG_MAX_FLASH_SECT      (128)       /* max number of sectors on one chip */
#define PHYS_FLASH_1            0xBFC00000  /* Flash Bank #1 */
#define PHYS_FLASH_1_SIZE       0x00040000

#define CFG_FLASH_BASE          PHYS_FLASH_1
#define CFG_FLASH_SIZE          PHYS_FLASH_1_SIZE

/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT    (20 * CFG_HZ)   /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT    (2 * CFG_HZ)    /* Timeout for Flash Write */

#if ((CONFIG_COMMANDS & CONFIG_ENV_IN_NAND) || (CONFIG_COMMANDS & CFG_CMD_ENV_IN_UBI))
#define CFG_ENV_IS_IN_NAND      1
#define CFG_ENV_OFFSET          cfg_env_offset
#define CFG_ENV_SIZE            0x20000             // 128KB
/* the number of block allocated to environment, this should be larger than CFG_ENV_SIZE */
#define CFG_ENV_BLOCK_NUM       1

#elif (CONFIG_COMMANDS & CONFIG_ENV_IN_SERIAL)
/* Address and size of Primary Environment Sector    */
//#define CFG_ENV_IS_IN_FLASH     1
//#define CFG_ENV_SIZE            0x20000             // 128 KB
////#define CFG_ENV_ADDR          0xBFC40000          // 256 KB
//#define CFG_ENV_ADDR            (CFG_FLASH_BASE+CFG_FLASH_SIZE-CFG_ENV_SIZE)    // temp to put ENV in the end of flash
#define CFG_ENV_IS_IN_SPIFLASH  1
#define CFG_ENV_SIZE		    0x10000
#define CFG_ENV_ADDR		    cfg_env_offset
#define CFG_ENV_ADDR_BAK        (CFG_ENV_ADDR + CFG_ENV_SIZE)
#else
#error "Incorrect env setting!!"
#endif

#if ((CONFIG_COMMANDS & CFG_CMD_USB) || (CONFIG_COMMANDS & CONFIG_OAD))
#if (CONFIG_COMMANDS & CONFIG_INFO_IN_UBI_FILE)
#define CFG_INFO_IS_IN_UBI_FILE 1
#elif (CONFIG_COMMANDS & CONFIG_INFO_IN_SERIAL_ENV)
#define CFG_INFO_IS_IN_SPI_ENV  1
#else
#error "Incorrect info exchange place setting!!"
#endif
#endif

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_ENABLE_ICACHE    1
#define CONFIG_ENABLE_DCACHE    1

#if (ENABLE_MCU_AEONR2)
#define CFG_ICACHE_SIZE         16384
#define CFG_ICACHE_LINE_SIZE    16

#define CFG_DCACHE_SIZE         32768
#define CFG_DCACHE_LINE_SIZE    16
#define CFG_DCACHE_WAYS         4
#else
#define CFG_ICACHE_SIZE         8192
#define CFG_ICACHE_LINE_SIZE    16

#define CFG_DCACHE_SIZE         8192        // should be 4096 but we use large number to init
#define CFG_DCACHE_LINE_SIZE    16
#define CFG_DCACHE_WAYS         2
#endif


/*-----------------------------------------------------------------------
 * Serial Configuration
 */

/*-----------------------------------------------------------------------
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

/*-----------------------------------------------------------------------
 * NET configuration
 */
#if (CONFIG_COMMANDS & CFG_CMD_NET)
#define     CONFIG_NET_MULTI
#endif
//#define     BOOT_AUTO_RUN

#ifdef BOOT_AUTO_RUN
#define    CONFIG_EXTRA_ENV_SETTINGS                           \
        "bootp  \0"                                           \
        "bootdelay=3           \0"                         \
        "ipaddr=172.16.88.66   \0"                         \
        "serverip=172.16.88.17 \0"                         \
        "bootcmd=usb start;tftp 80100000 uranus.bin \0"    \
        ""
#endif

#define CONFIG_SYS_64BIT_VSPRINTF        1
#define CONFIG_MTD_PARTITIONS

#endif /* __CONFIG_H */
