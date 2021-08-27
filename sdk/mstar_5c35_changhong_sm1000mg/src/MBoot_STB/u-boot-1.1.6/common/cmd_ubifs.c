/*
 * (C) Copyright 2008
 * Stefan Roese, DENX Software Engineering, sr@denx.de.
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
 *
 */


/*
 * UBIFS command support
 */

#undef DEBUG

#include <common.h>
#include <config.h>
#include <command.h>

#if (CONFIG_COMMANDS & CFG_CMD_UBIFS)
#include "uboot_mmap.h"

int ubifs_initialized;
int ubifs_mounted;

/* Prototypes */
int ubifs_init(void);
int ubifs_mount(char *vol_name);
void ubifs_cmd_umount(void);
int ubifs_ls(char *dir_name);
int ubifs_load(char *filename, u32 addr, u32 size);
int ubifs_part_load(char *filename, u32 addr, u32 offset, u32 size);
int ubifs_filesize(char *filename, u32* size);

int do_ubifs_mount(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char *vol_name;
	int ret;

	vol_name = argv[1];
	printf("Using volume %s\n", vol_name);

	if (ubifs_initialized == 0) {
		ubifs_init();
		ubifs_initialized = 1;
	}

	ret = ubifs_mount(vol_name);
	if (ret)
	{
	    printf("ubifs_mount FAIL !!\n");
		return -1;
	}
    else
    {
        printf("ubifs_mount SUCCESS !!\n");
	    ubifs_mounted = 1;
	    return 0;
    }
}

int do_ubifs_umount(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    if (ubifs_initialized == 0) {
        ubifs_init();
        ubifs_initialized = 1;
    }

    /* avoid parasoft's warning */
    cmdtp = cmdtp;
    argv = argv;

    ubifs_cmd_umount();
    ubifs_mounted = 0;

    return 0;
}

int do_ubifs_ls(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char *filename = "/";
	int ret;

	if (!ubifs_mounted) {
		printf("UBIFS not mounted, use ubifs mount to mount volume first!\n");
		return -1;
	}

	if (argc == 2)
		filename = argv[1];
	printf("Using filename %s\n", filename);

	ret = ubifs_ls(filename);
	if (ret)
		printf("%s not found!\n", filename);

	return ret;
}

int do_ubifs_load(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char *filename;
	int ret;
	u32 addr;
	u32 size = 0;

	if (!ubifs_mounted) {
		printf("UBIFS not mounted, use ubifs mount to mount volume first!\n");
		return -1;
	}

	if (argc < 3) {
		printf("Usage:\n%s\n", cmdtp->usage);
		return -1;
	}

    addr = (0 == strncmp(argv[1], UPGRADE_ADDR_STR, strlen(UPGRADE_ADDR_STR))) ? UPGRADE_BUFFER_ADDR : simple_strtoul(argv[1], NULL, 16);
	filename = argv[2];

	if (argc == 4)
		size = simple_strtoul(argv[3], NULL, 16);
	printf("Loading file '%s' to address 0x%08x (size %d)\n", filename, addr, size);

	ret = ubifs_load(filename, addr, size);
	if (ret)
		printf("%s not found!\n", filename);

	return ret;
}

int do_ubifs_part_load(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *filename;
    int ret;
    u32 addr = 0;
    u32 size = 0;
    u32 offset = 0;

    if (!ubifs_mounted) {
        printf("UBIFS not mounted, use ubifs mount to mount volume first!\n");
        return -1;
    }

    if (argc < 5) {
        printf("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }

    addr = (0 == strncmp(argv[1], UPGRADE_ADDR_STR, strlen(UPGRADE_ADDR_STR))) ? UPGRADE_BUFFER_ADDR : simple_strtoul(argv[1], NULL, 16);
    filename = argv[2];

    //if (argc == 4)
    //    size = simple_strtoul(argv[3], NULL, 16);
    if (argc == 5)
    {
        offset = simple_strtoul(argv[3], NULL, 16);
        size = simple_strtoul(argv[4], NULL, 16);
    }
    printf("Loading file '%s' to address 0x%08x (size %d)\n", filename, addr, size);

    ret = ubifs_part_load(filename, addr, offset, size);
    if (ret)
        printf("%s not found!\n", filename);

    return ret;
}

int do_ubifs_filesize(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *filename;
    int ret;
    U32* size;

    if (!ubifs_mounted) {
        printf("UBIFS not mounted, use ubifs mount to mount volume first!\n");
        return -1;
    }

    if (argc !=3) {
        printf("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }

    filename = argv[1];
    size=(U32*)simple_strtoul(argv[2], NULL, 16);

    ret = ubifs_filesize(filename, size);
    if (ret)
        printf("%s not found!\n", filename);

	return ret;
}

U_BOOT_CMD(
    ubifsfilesize, 3, 0, do_ubifs_filesize,
    "get file size in UBIFS volume",
    "\n");

U_BOOT_CMD(
	ubifsmount, 2, 0, do_ubifs_mount,
	"mount UBIFS volume",
	"\n");

U_BOOT_CMD(
        ubifsumount, 1, 0, do_ubifs_umount,
        "umount UBIFS volume already mounted",
        "\n");

U_BOOT_CMD(ubifsls, 2, 0, do_ubifs_ls,
	   "list files in a directory",
	   "[directory]\n"
	   "    - list files in a 'directory' (default '/')\n");

U_BOOT_CMD(ubifsload, 4, 0, do_ubifs_load,
	   "load file from an UBIFS filesystem",
	   "<addr> <filename> [bytes]\n"
	   "    - load file 'filename' to address 'addr'\n");

U_BOOT_CMD(ubifspartload, 5, 0, do_ubifs_part_load,
    "load part of a file from an UBIFS filesystem",
    "<addr> <filename> <offset> <size>\n"
    "    - load file 'filename' to address 'addr'\n");

#endif
