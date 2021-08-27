/*
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * Robert Schwebel, Pengutronix, <r.schwebel@pengutronix.de>
 *
 * (C) Copyright 2003
 * Kai-Uwe Bloem, Auerswald GmbH & Co KG, <linux-development@auerswald.de>
 *
 * (C) Copyright 2005
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 *   Added support for reading flash partition table from environment.
 *   Parsing routines are based on driver/mtd/cmdline.c from the linux 2.4
 *   kernel tree.
 *
 *   $Id: cmdlinepart.c,v 1.17 2004/11/26 11:18:47 lavinen Exp $
 *   Copyright 2002 SYSGO Real-Time Solutions GmbH
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

/*
 * Three environment variables are used by the parsing routines:
 *
 * 'partition' - keeps current partition identifier
 *
 * partition  := <part-id>
 * <part-id>  := <dev-id>,part_num
 *
 *
 * 'mtdids' - linux kernel mtd device id <-> u-boot device id mapping
 *
 * mtdids=<idmap>[,<idmap>,...]
 *
 * <idmap>    := <dev-id>=<mtd-id>
 * <dev-id>   := 'nand'|'nor'<dev-num>
 * <dev-num>  := mtd device number, 0...
 * <mtd-id>   := unique device tag used by linux kernel to find mtd device (mtd->name)
 *
 *
 * 'mtdparts' - partition list
 *
 * mtdparts=mtdparts=<mtd-def>[;<mtd-def>...]
 *
 * <mtd-def>  := <mtd-id>:<part-def>[,<part-def>...]
 * <mtd-id>   := unique device tag used by linux kernel to find mtd device (mtd->name)
 * <part-def> := <size>[@<offset>][<name>][<ro-flag>]
 * <size>     := standard linux memsize OR '-' to denote all remaining space
 * <offset>   := partition start offset within the device
 * <name>     := '(' NAME ')'
 * <ro-flag>  := when set to 'ro' makes partition read-only (not used, passed to kernel)
 *
 * Notes:
 * - each <mtd-id> used in mtdparts must albo exist in 'mtddis' mapping
 * - if the above variables are not set defaults for a given target are used
 *
 * Examples:
 *
 * 1 NOR Flash, with 1 single writable partition:
 * mtdids=nor0=edb7312-nor
 * mtdparts=mtdparts=edb7312-nor:-
 *
 * 1 NOR Flash with 2 partitions, 1 NAND with one
 * mtdids=nor0=edb7312-nor,nand0=edb7312-nand
 * mtdparts=mtdparts=edb7312-nor:256k(ARMboot)ro,-(root);edb7312-nand:-(home)
 *
 */

/*
 * JFFS2/CRAMFS support
 */
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <jffs2/jffs2.h>
#include <linux/list.h>
#include <linux/ctype.h>

#include <cramfs/cramfs_fs.h>
#include "uboot_mmap.h"

#if (CONFIG_COMMANDS & CFG_CMD_NAND)
#ifdef CFG_NAND_LEGACY
#include <linux/mtd/nand_legacy.h>
#else /* !CFG_NAND_LEGACY */
#include <linux/mtd/nand.h>
#include <nand.h>
#endif /* !CFG_NAND_LEGACY */
#endif /* (CONFIG_COMMANDS & CFG_CMD_NAND) */
/* enable/disable debugging messages */
#define	DEBUG_JFFS
#undef	DEBUG_JFFS

#ifdef  DEBUG_JFFS
# define DEBUGF(fmt, args...)	printf(fmt ,##args)
#else
# define DEBUGF(fmt, args...)
#endif

#if (CONFIG_COMMANDS & CFG_CMD_JFFS2)
/* special size referring to all the remaining space in a partition */
#define SIZE_REMAINING		0xFFFFFFFF

/* special offset value, it is used when not provided by user
 *
 * this value is used temporarily during parsing, later such offests
 * are recalculated */
#define OFFSET_NOT_SPECIFIED	0xFFFFFFFF

/* minimum partition size */
#define MIN_PART_SIZE		4096

/* this flag needs to be set in part_info struct mask_flags
 * field for read-only partitions */
#define MTD_WRITEABLE_CMD		1

#ifdef CONFIG_JFFS2_CMDLINE
/* default values for mtdids and mtdparts variables */
#if defined(MTDIDS_DEFAULT)
static const char *const mtdids_default = MTDIDS_DEFAULT;
#else
#warning "MTDIDS_DEFAULT not defined!"
static const char *const mtdids_default = NULL;
#endif

#if defined(MTDPARTS_DEFAULT)
static const char *const mtdparts_default = MTDPARTS_DEFAULT;
#else
#warning "MTDPARTS_DEFAULT not defined!"
static const char *const mtdparts_default = NULL;
#endif

/* low level jffs2 cache cleaning routine */
extern void jffs2_free_cache(struct part_info *part);

extern int strcmp(const char * cs,const char * ct);
extern char * strncpy(char * dest,const char *src,size_t count);
extern void current_save(void);
extern int mtdparts_init(void);
extern int find_dev_and_part(const char *id, struct mtd_device **dev,
        u8 *part_num, struct part_info **part);
extern struct part_info* jffs2_part_info(struct mtd_device *dev, unsigned int part_num);

/* mtdids mapping list, filled by parse_ids() */
struct list_head mtdids;

/* device/partition list, parse_cmdline() parses into here */
struct list_head devices;
#endif /* #ifdef CONFIG_JFFS2_CMDLINE */

/* current active device and partition number */
extern struct mtd_device *current_dev;
extern u8 current_partnum;

extern int cramfs_check (struct part_info *info);
extern int cramfs_load (char *loadoffset, struct part_info *info, char *filename);
extern int cramfs_ls (struct part_info *info, char *filename);
extern int cramfs_info (struct part_info *info);

/***************************************************/
/* U-boot commands				   */
/***************************************************/

/**
 * Routine implementing fsload u-boot command. This routine tries to load
 * a requested file from jffs2/cramfs filesystem on a current partition.
 *
 * @param cmdtp command internal data
 * @param flag command flag
 * @param argc number of arguments supplied to the command
 * @param argv arguments list
 * @return 0 on success, 1 otherwise
 */
int do_jffs2_fsload(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char *fsname;
	char *filename;
	int size;
	struct part_info *part;
	ulong offset = load_addr;
#if defined (CONFIG_SUPERNOVA_OAD)
	ulong size_addr=0;
#endif

	/* pre-set Boot file name */
	if ((filename = getenv("bootfile")) == NULL) {
		filename = "uImage";
	}

	if (argc == 2) {
		filename = argv[1];
	}
	if (argc == 3) {
		offset = (0 == strncmp(argv[1], UPGRADE_ADDR_STR, strlen(UPGRADE_ADDR_STR))) ? UPGRADE_BUFFER_ADDR : simple_strtoul(argv[1], NULL, 16);
		load_addr = offset;
		filename = argv[2];
	}
#if defined (CONFIG_SUPERNOVA_OAD)
    if (argc == 4) {
		offset = (0 == strncmp(argv[1], UPGRADE_ADDR_STR, strlen(UPGRADE_ADDR_STR))) ? UPGRADE_BUFFER_ADDR : simple_strtoul(argv[1], NULL, 16);
		load_addr = offset;
		filename = argv[2];
		size_addr = simple_strtoul(argv[3], NULL, 16);
	}
#endif

	/* make sure we are in sync with env variables */
	if (mtdparts_init() !=0)
		return 1;

	if ((part = jffs2_part_info(current_dev, current_partnum))){

#if (CONFIG_COMMANDS & CFG_CMD_CRAMFS)
		/* check partition type for cramfs */
		fsname = (cramfs_check(part) ? "CRAMFS" : "JFFS2");
#else
		fsname="JFFS2";
#endif
		printf("### %s loading '%s' to 0x%lx\n", fsname, filename, offset);

#if (CONFIG_COMMANDS & CFG_CMD_CRAMFS)
		if (cramfs_check(part)) {
			size = cramfs_load ((char *) offset, part, filename);
		} else
#endif
		{
			/* if this is not cramfs assume jffs2 */
			size = jffs2_1pass_load((char *)offset, part, filename);
#if defined (CONFIG_SUPERNOVA_OAD)
			if(size_addr != 0)
		    {
		        memcpy((char*)size_addr, &size, sizeof(size));
		    }
#endif
		}

		if (size > 0) {
			char buf[10];
			printf("### %s load complete: %d bytes loaded to 0x%lx\n",
				fsname, size, offset);
			sprintf(buf, "%x", size);
			setenv("filesize", buf);
		} else {
			printf("### %s LOAD ERROR<%x> for %s!\n", fsname, size, filename);
		}

		return !(size > 0);
	}
	return 1;
}

/**
 * Routine implementing u-boot ls command which lists content of a given
 * directory on a current partition.
 *
 * @param cmdtp command internal data
 * @param flag command flag
 * @param argc number of arguments supplied to the command
 * @param argv arguments list
 * @return 0 on success, 1 otherwise
 */
int do_jffs2_ls(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char *filename = "/";
	int ret;
	struct part_info *part;

	if (argc == 2)
		filename = argv[1];

	/* make sure we are in sync with env variables */
	if (mtdparts_init() !=0)
		return 1;

	if ((part = jffs2_part_info(current_dev, current_partnum))){
#if (CONFIG_COMMANDS & CFG_CMD_CRAMFS)
		/* check partition type for cramfs */
		if (cramfs_check(part)) {
			ret = cramfs_ls (part, filename);
		} else
#endif
		{
			/* if this is not cramfs assume jffs2 */
			ret = jffs2_1pass_ls(part, filename);
		}

		return ret ? 0 : 1;
	}
	return 1;
}

/**
 * Routine implementing u-boot fsinfo command. This routine prints out
 * miscellaneous filesystem informations/statistics.
 *
 * @param cmdtp command internal data
 * @param flag command flag
 * @param argc number of arguments supplied to the command
 * @param argv arguments list
 * @return 0 on success, 1 otherwise
 */
int do_jffs2_fsinfo(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	struct part_info *part;
	char *fsname;
	int ret;

	/* make sure we are in sync with env variables */
	if (mtdparts_init() !=0)
		return 1;

	if ((part = jffs2_part_info(current_dev, current_partnum))){
#if (CONFIG_COMMANDS & CFG_CMD_CRAMFS)
		/* check partition type for cramfs */
		fsname = (cramfs_check(part) ? "CRAMFS" : "JFFS2");
#else
		fsname="JFFS2";
#endif
		printf("### filesystem type is %s\n", fsname);
#if (CONFIG_COMMANDS & CFG_CMD_CRAMFS)
		if (cramfs_check(part)) {
			ret = cramfs_info (part);
		} else
#endif
		{
			/* if this is not cramfs assume jffs2 */
			ret = jffs2_1pass_info(part);
		}

		return ret ? 0 : 1;
	}
	return 1;
}

/* command line only */
#ifdef CONFIG_JFFS2_CMDLINE
/**
 * Routine implementing u-boot chpart command. Sets new current partition based
 * on the user supplied partition id. For partition id format see find_dev_and_part().
 *
 * @param cmdtp command internal data
 * @param flag command flag
 * @param argc number of arguments supplied to the command
 * @param argv arguments list
 * @return 0 on success, 1 otherwise
 */
int do_jffs2_chpart(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
/* command line only */
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;

	if (mtdparts_init() !=0)
		return 1;

	if (argc < 2) {
		printf("no partition id specified\n");
		return 1;
	}

	if (find_dev_and_part(argv[1], &dev, &pnum, &part) != 0)
		return 1;

	current_dev = dev;
	current_partnum = pnum;
	current_save();

	printf("partition changed to %s%d,%d\n",
			MTD_DEV_TYPE(dev->id->type), dev->id->num, pnum);

	return 0;
}

#endif /* #ifdef CONFIG_JFFS2_CMDLINE */

/***************************************************/
U_BOOT_CMD(
#if defined (CONFIG_SUPERNOVA_OAD)
	fsload,	4,	0,	do_jffs2_fsload,
#else
	fsload,	3,	0,	do_jffs2_fsload,
#endif
	"fsload\t- load binary file from a filesystem image\n",
	"[ off ] [ filename ]\n"
	"    - load binary file from flash bank\n"
	"      with offset 'off'\n"
);
U_BOOT_CMD(
	ls,	2,	1,	do_jffs2_ls,
	"ls\t- list files in a directory (default /)\n",
	"[ directory ]\n"
	"    - list files in a directory.\n"
);

U_BOOT_CMD(
	fsinfo,	1,	1,	do_jffs2_fsinfo,
	"fsinfo\t- print information about filesystems\n",
	"    - print information about filesystems\n"
);

#ifdef CONFIG_JFFS2_CMDLINE
U_BOOT_CMD(
	chpart,	2,	0,	do_jffs2_chpart,
	"chpart\t- change active partition\n",
	"part-id\n"
	"    - change active partition (e.g. part-id = nand0,1)\n"
);

#endif /* #ifdef CONFIG_JFFS2_CMDLINE */

/***************************************************/

#endif /* CFG_CMD_JFFS2 */
