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
#define DEBUG_JFFS
#undef DEBUG_JFFS

#ifdef  DEBUG_JFFS
#define DEBUGF(fmt, args...) printf(fmt ,##args)
#else
#define DEBUGF(fmt, args...)
#endif

/* special size referring to all the remaining space in a partition */
#define SIZE_REMAINING 0xFFFFFFFF

/* special offset value, it is used when not provided by user
 *
 * this value is used temporarily during parsing, later such offests
 * are recalculated */
#define OFFSET_NOT_SPECIFIED 0xFFFFFFFF

/* minimum partition size */
#define MIN_PART_SIZE 4096

/* this flag needs to be set in part_info struct mask_flags
 * field for read-only partitions */
#define MTD_WRITEABLE_CMD 1

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


/* copies of last seen 'mtdids', 'mtdparts' and 'partition' env variables */
#define MTDIDS_MAXLEN 128
#define MTDPARTS_MAXLEN 512
#define PARTITION_MAXLEN 16
static char last_ids[MTDIDS_MAXLEN];
static char last_parts[MTDPARTS_MAXLEN];
static char last_partition[PARTITION_MAXLEN];

/* low level jffs2 cache cleaning routine */
extern void jffs2_free_cache(struct part_info *part);

extern int strcmp(const char * cs,const char * ct);
extern char * strncpy(char * dest,const char *src,size_t count);

/* mtdids mapping list, filled by parse_ids() */
struct list_head mtdids;

/* device/partition list, parse_cmdline() parses into here */
struct list_head devices;
#endif /* #ifdef CONFIG_JFFS2_CMDLINE */

/* current active device and partition number */
struct mtd_device *current_dev = NULL;
u8 current_partnum = 0;

struct part_info* jffs2_part_info(struct mtd_device *dev, unsigned int part_num);

/* command line only routines */
#if 1

static struct mtdids* id_find_by_mtd_id(const char *mtd_id, unsigned int mtd_id_len);
static int device_del(struct mtd_device *dev);

/**
 * Parses a string into a number.  The number stored at ptr is
 * potentially suffixed with K (for kilobytes, or 1024 bytes),
 * M (for megabytes, or 1048576 bytes), or G (for gigabytes, or
 * 1073741824).  If the number is suffixed with K, M, or G, then
 * the return value is the number multiplied by one kilobyte, one
 * megabyte, or one gigabyte, respectively.
 *
 * @param ptr where parse begins
 * @param retptr output pointer to next char after parse completes (output)
 * @return resulting unsigned int
 */
static unsigned long memsize_parse (const char *const ptr, const char **retptr)
{
    U32 addr = (U32)&retptr[0];
    char **pptr = (char**)addr;
    unsigned long ret = simple_strtoul(ptr, (char **)pptr, 0);

    switch (**retptr) {
        case 'G':
            /* fall through */
        case 'g':
            ret <<= 10;
            /* fall through */
        case 'M':
            /* fall through */
        case 'm':
            ret <<= 10;
            /* fall through */
        case 'K':
            /* fall through */
        case 'k':
            ret <<= 10;
            (*retptr)++;
            /* fall through */
        default:
            break;
    }

    return ret;
}

/**
 * Format string describing supplied size. This routine does the opposite job
 * to memsize_parse(). Size in bytes is converted to string and if possible
 * shortened by using k (kilobytes), m (megabytes) or g (gigabytes) suffix.
 *
 * Note, that this routine does not check for buffer overflow, it's the caller
 * who must assure enough space.
 *
 * @param buf output buffer
 * @param size size to be converted to string
 */
static void memsize_format(char *buf, u32 size)
{
#define SIZE_GB ((u32)1024*1024*1024)
#define SIZE_MB ((u32)1024*1024)
#define SIZE_KB ((u32)1024)

    if ((size % SIZE_GB) == 0)
        sprintf(buf, "%ug", size/SIZE_GB);
    else if ((size % SIZE_MB) == 0)
        sprintf(buf, "%um", size/SIZE_MB);
    else if (size % SIZE_KB == 0)
        sprintf(buf, "%uk", size/SIZE_KB);
    else
        sprintf(buf, "%u", size);
}

/**
 * This routine does global indexing of all partitions. Resulting index for
 * current partition is saved in 'mtddevnum'. Current partition name in
 * 'mtddevname'.
 */
static void index_partitions(void)
{
    char buf[16];
    u16 mtddevnum;
    //struct part_info *part;
    struct list_head *dentry;
    struct mtd_device *dev;

    DEBUGF("--- index partitions ---\n");

    if (current_dev) {
        mtddevnum = 0;
        list_for_each(dentry, &devices) {
            dev = list_entry(dentry, struct mtd_device, link);
            if (dev == current_dev) {
                mtddevnum += current_partnum;
                sprintf(buf, "%d", mtddevnum);
                // setenv("mtddevnum", buf); // TODO: review, deprecate?
                break;
            }
            mtddevnum += dev->num_parts;
        }
        jffs2_part_info(current_dev, current_partnum);

        //part = jffs2_part_info(current_dev, current_partnum);
        // setenv("mtddevname", part->name); // TODO: review, deprecate?

        //DEBUGF("=> mtddevnum %d,\n=> mtddevname %s\n", mtddevnum, part->name);
    }
    else {
    // setenv("mtddevnum", NULL); // TODO: review, deprecate?
    // setenv("mtddevname", NULL); // TODO: review, deprecate?

        DEBUGF("=> mtddevnum NULL\n=> mtddevname NULL\n");
    }
}

/**
 * Save current device and partition in environment variable 'partition'.
 */
void current_save(void)
{
    char buf[16];

    DEBUGF("--- current_save ---\n");

    if (current_dev) {
        sprintf(buf, "%s%d,%d", MTD_DEV_TYPE(current_dev->id->type),
            current_dev->id->num, current_partnum);

        setenv((char*)"partition", buf);
        //strncpy(last_partition, buf, 16);
        if(strlen(buf)<16){
            strncpy(last_partition, buf, strlen(buf)+1);
        }
        else{
            strncpy(last_partition, buf, 15);
            last_partition[15]='\0';
        }

        DEBUGF("=> partition %s\n", buf);
    }
    else {
        setenv((char*)"partition", NULL);
        last_partition[0] = '\0';

        DEBUGF("=> partition NULL\n");
    }
    index_partitions();
}

/**
 * Performs sanity check for supplied NOR flash partition. Table of existing
 * NOR flash devices is searched and partition device is located. Alignment
 * with the granularity of NOR flash sectors is verified.
 *
 * @param id of the parent device
 * @param part partition to validate
 * @return 0 if partition is valid, 1 otherwise
 */
static int part_validate_nor(struct mtdids *id, struct part_info *part)
{
#if (CONFIG_COMMANDS & CFG_CMD_FLASH)
    /* info for FLASH chips */
    extern flash_info_t flash_info[];
    flash_info_t *flash;
    int offset_aligned;
    u32 end_offset;
    int i;

    flash = &flash_info[id->num];

    offset_aligned = 0;
    for (i = 0; i < flash->sector_count; i++) {
        if ((flash->start[i] - flash->start[0]) == part->offset) {
            offset_aligned = 1;
            break;
        }
    }
    if (offset_aligned == 0) {
        printf("%s%d: partition (%s) start offset alignment incorrect\n",
            MTD_DEV_TYPE(id->type), id->num, part->name);
        return 1;
    }

    end_offset = part->offset + part->size;
    for (i = 0; i < flash->sector_count; i++) {
        if ((flash->start[i] - flash->start[0]) == end_offset)
        return 0;
    }

    if (flash->size == end_offset)
        return 0;

    printf("%s%d: partition (%s) size alignment incorrect\n",
        MTD_DEV_TYPE(id->type), id->num, part->name);
#endif
    return 1;
}

/**
 * Performs sanity check for supplied NAND flash partition. Table of existing
 * NAND flash devices is searched and partition device is located. Alignment
 * with the granularity of nand erasesize is verified.
 *
 * @param id of the parent device
 * @param part partition to validate
 * @return 0 if partition is valid, 1 otherwise
 */
static int part_validate_nand(struct mtdids *id, struct part_info *part)
{
#if defined(CONFIG_JFFS2_NAND) && (CONFIG_COMMANDS & CFG_CMD_NAND)
    /* info for NAND chips */
    nand_info_t *nand;

    nand = &nand_info[id->num];

    if ((unsigned long)(part->offset) % nand->erasesize) {
        printf("%s%d: partition (%s) start offset alignment incorrect\n",
            MTD_DEV_TYPE(id->type), id->num, part->name);
        return 1;
    }

    if (part->size % nand->erasesize) {
        printf("%s%d: partition (%s) size alignment incorrect\n",
            MTD_DEV_TYPE(id->type), id->num, part->name);
        return 1;
    }

    return 0;
#else
    return 1;
#endif
}

/**
 * Performs sanity check for supplied partition. Offset and size are verified
 * to be within valid range. Partition type is checked and either
 * parts_validate_nor() or parts_validate_nand() is called with the argument
 * of part.
 *
 * @param id of the parent device
 * @param part partition to validate
 * @return 0 if partition is valid, 1 otherwise
 */
static int part_validate(struct mtdids *id, struct part_info *part)
{
    if (part->size == SIZE_REMAINING)
        part->size = id->size - part->offset;

    if (part->offset > id->size) {
        printf("%s: offset %08x beyond flash size %08x\n",
            id->mtd_id, part->offset, id->size);
        return 1;
    }

    if ((part->offset + part->size) <= part->offset) {
        printf("%s%d: partition (%s) size too big\n",
            MTD_DEV_TYPE(id->type), id->num, part->name);
        return 1;
    }

    if (part->offset + part->size > id->size) {
        printf("%s: partitioning exceeds flash size\n", id->mtd_id);
        return 1;
    }

    if (id->type == MTD_DEV_TYPE_NAND)
        return part_validate_nand(id, part);
    else if (id->type == MTD_DEV_TYPE_NOR)
        return part_validate_nor(id, part);
    else
        DEBUGF("part_validate: invalid dev type\n");

    return 1;
}

/**
 * Delete selected partition from the partion list of the specified device.
 *
 * @param dev device to delete partition from
 * @param part partition to delete
 * @return 0 on success, 1 otherwise
 */
static int part_del(struct mtd_device *dev, struct part_info *part)
{
    u8 current_save_needed = 0;

    /* if there is only one partition, remove whole device */
    if (dev->num_parts == 1)
        return device_del(dev);

    /* otherwise just delete this partition */

    if (dev == current_dev) {
        /* we are modyfing partitions for the current device,
        * update current */
        struct part_info *curr_pi;
        curr_pi = jffs2_part_info(current_dev, current_partnum);

        if (curr_pi) {
            if (curr_pi == part) {
                printf("current partition deleted, resetting current to 0\n");
                current_partnum = 0;
            }
            else if (part->offset <= curr_pi->offset) {
                current_partnum--;
            }
            current_save_needed = 1;
        }
    }

#ifdef CFG_NAND_LEGACY
    jffs2_free_cache(part);
#endif
    list_del(&part->link);
    free(part);
    dev->num_parts--;

    if (current_save_needed > 0)
        current_save();
    else
        index_partitions();

    return 0;
}

/**
 * Delete all partitions from parts head list, free memory.
 *
 * @param head list of partitions to delete
 */
static void part_delall(struct list_head *head)
{
    struct list_head *entry, *n;
    struct part_info *part_tmp;

    /* clean tmp_list and free allocated memory */
    list_for_each_safe(entry, n, head) {
        part_tmp = list_entry(entry, struct part_info, link);

#ifdef CFG_NAND_LEGACY
        jffs2_free_cache(part_tmp);
#endif
        list_del(entry);
        free(part_tmp);
    }
}

/**
 * Add new partition to the supplied partition list. Make sure partitions are
 * sorted by offset in ascending order.
 *
 * @param head list this partition is to be added to
 * @param new partition to be added
 */
static int part_sort_add(struct mtd_device *dev, struct part_info *part)
{
    struct list_head *entry;
    struct part_info *new_pi, *curr_pi;

    /* link partition to parrent dev */
    part->dev = dev;

    if (list_empty(&dev->parts)) {
        DEBUGF("part_sort_add: list empty\n");
        list_add(&part->link, &dev->parts);
        dev->num_parts++;
        index_partitions();
        return 0;
    }

    new_pi = list_entry(&part->link, struct part_info, link);

    /* get current partition info if we are updating current device */
    curr_pi = NULL;
    if (dev == current_dev)
        curr_pi = jffs2_part_info(current_dev, current_partnum);

    list_for_each(entry, &dev->parts) {
        struct part_info *pi;

        pi = list_entry(entry, struct part_info, link);

        /* be compliant with kernel cmdline, allow only one partition at offset zero */
        if ((new_pi->offset == pi->offset) && (pi->offset == 0)) {
            printf("cannot add second partition at offset 0\n");
            return 1;
        }

        if (new_pi->offset <= pi->offset) {
            list_add_tail(&part->link, entry);
            dev->num_parts++;

            if (curr_pi && (pi->offset <= curr_pi->offset)) {
                /* we are modyfing partitions for the current
                 * device, update current */
                current_partnum++;
                current_save();
            }
            else {
                index_partitions();
            }
            return 0;
        }
    }

    list_add_tail(&part->link, &dev->parts);
    dev->num_parts++;
    index_partitions();
    return 0;
}

/**
 * Add provided partition to the partition list of a given device.
 *
 * @param dev device to which partition is added
 * @param part partition to be added
 * @return 0 on success, 1 otherwise
 */
static int part_add(struct mtd_device *dev, struct part_info *part)
{
    /* verify alignment and size */
    if (part_validate(dev->id, part) != 0)
        return 1;

    /* partition is ok, add it to the list */
    if (part_sort_add(dev, part) != 0)
        return 1;

    return 0;
}

/**
 * Parse one partition definition, allocate memory and return pointer to this
 * location in retpart.
 *
 * @param partdef pointer to the partition definition string i.e. <part-def>
 * @param ret output pointer to next char after parse completes (output)
 * @param retpart pointer to the allocated partition (output)
 * @return 0 on success, 1 otherwise
 */
static int part_parse(const char *const partdef, const char **ret, struct part_info **retpart)
{
    struct part_info *part;
    unsigned long size;
    unsigned long offset;
    const char *name;
    int name_len;
    unsigned int mask_flags;
    const char *p;

    p = partdef;
    *retpart = NULL;
    *ret = NULL;

    /* fetch the partition size */
    if (*p == '-') {
        /* assign all remaining space to this partition */
        DEBUGF("'-': remaining size assigned\n");
        size = SIZE_REMAINING;
        p++;
    }
    else {
        size = memsize_parse(p, &p);
        if (size < MIN_PART_SIZE) {
            printf("partition size too small (%lx)\n", size);
            return 1;
        }
    }

    /* check for offset */
    offset = OFFSET_NOT_SPECIFIED;
    if (*p == '@') {
        p++;
        offset = memsize_parse(p, &p);
    }

    /* now look for the name */
    if (*p == '(') {
        p += 1;
        name = p;
        p = strchr(name, ')');
        if (p == NULL) {
            printf("no closing ) found in partition name\n");
            return 1;
        }
        name_len = p - name + 1;
        if ((name_len - 1) == 0) {
            printf("empty partition name\n");
            return 1;
        }
        p++;
    }
    else {
        /* 0x00000000@0x00000000 */
        name_len = 22;
        name = NULL;
    }

    /* test for options */
    mask_flags = 0;
    if (strncmp(p, "ro", 2) == 0) {
        mask_flags |= MTD_WRITEABLE_CMD;
        p += 2;
    }

    /* check for next partition definition */
    if (*p == ',') {
        if (size == SIZE_REMAINING) {
            *ret = NULL;
            printf("no partitions allowed after a fill-up partition\n");
            return 1;
        }
        p+=1;
        *ret = p;
    }
    else if ((*p == ';') || (*p == '\0')) {
        *ret = p;
    }
    else {
        printf("unexpected character '%c' at the end of partition\n", *p);
        *ret = NULL;
        return 1;
    }

    /*  allocate memory */
    part = (struct part_info *)malloc(sizeof(struct part_info) + name_len);
    if (!part) {
        printf("out of memory\n");
        return 1;
    }
    memset(part, 0, sizeof(struct part_info) + name_len);
    part->size = size;
    part->offset = offset;
    part->mask_flags = mask_flags;
    part->name = (char *)(part + 1);

    if (name) {
        /* copy user provided name */
        strncpy(part->name, name, name_len - 1);
        part->auto_name = 0;
    }
    else {
        /* auto generated name in form of size@offset */
        sprintf(part->name, "0x%08lx@0x%08lx", size, offset);
        part->auto_name = 1;
    }

    part->name[name_len - 1] = '\0';
    INIT_LIST_HEAD(&part->link);

    DEBUGF("+ partition: name %-22s size 0x%08x offset 0x%08x mask flags %d\n",
    part->name, part->size,
    part->offset, part->mask_flags);

    *retpart = part;
    return 0;
}
#endif/* #ifdef CONFIG_JFFS2_CMDLINE */

/**
 * Check device number to be within valid range for given device type.
 *
 * @param dev device to validate
 * @return 0 if device is valid, 1 otherwise
 */
static int device_validate(u8 type, u8 num, u32 *size)
{
    if (type == MTD_DEV_TYPE_NOR) {
#if (CONFIG_COMMANDS & CFG_CMD_FLASH)
        if (num < CFG_MAX_FLASH_BANKS) {
            extern flash_info_t flash_info[];
            *size = flash_info[num].size;

            return 0;
        }

        printf("no such FLASH device: %s%d (valid range 0 ... %d\n",
        MTD_DEV_TYPE(type), num, CFG_MAX_FLASH_BANKS - 1);
#else
        printf("support for FLASH devices not present\n");
#endif
    }
    else if (type == MTD_DEV_TYPE_NAND) {
#if defined(CONFIG_JFFS2_NAND) && (CONFIG_COMMANDS & CFG_CMD_NAND)
        if (num < CFG_MAX_NAND_DEVICE) {
#ifndef CFG_NAND_LEGACY
            *size = nand_info[num].size;
#else
            extern struct nand_chip nand_dev_desc[CFG_MAX_NAND_DEVICE];
            *size = nand_dev_desc[num].totlen;
#endif
            return 0;
        }

        printf("no such NAND device: %s%d (valid range 0 ... %d)\n",
        MTD_DEV_TYPE(type), num, CFG_MAX_NAND_DEVICE - 1);
#else
        printf("support for NAND devices not present\n");
#endif
    }

    return 1;
}

#if 1
/**
 * Delete all mtd devices from a supplied devices list, free memory allocated for
 * each device and delete all device partitions.
 *
 * @return 0 on success, 1 otherwise
 */
static int device_delall(struct list_head *head)
{
    struct list_head *entry, *n;
    struct mtd_device *dev_tmp;

    /* clean devices list */
    list_for_each_safe(entry, n, head) {
        dev_tmp = list_entry(entry, struct mtd_device, link);
        list_del(entry);
        part_delall(&dev_tmp->parts);
        free(dev_tmp);
    }
    INIT_LIST_HEAD(&devices);

    return 0;
}

/**
 * If provided device exists it's partitions are deleted, device is removed
 * from device list and device memory is freed.
 *
 * @param dev device to be deleted
 * @return 0 on success, 1 otherwise
 */
static int device_del(struct mtd_device *dev)
{
    part_delall(&dev->parts);
    list_del(&dev->link);
    free(dev);

    if (dev == current_dev) {
        /* we just deleted current device */
        if (list_empty(&devices)) {
            current_dev = NULL;
        }
        else {
            /* reset first partition from first dev from the
             * devices list as current */
            current_dev = list_entry(devices.next, struct mtd_device, link);
            current_partnum = 0;
        }
        current_save();
        return 0;
    }

    index_partitions();
    return 0;
}

/**
 * Search global device list and return pointer to the device of type and num
 * specified.
 *
 * @param type device type
 * @param num device number
 * @return NULL if requested device does not exist
 */
static struct mtd_device* device_find(u8 type, u8 num)
{
    struct list_head *entry;
    struct mtd_device *dev_tmp;

    list_for_each(entry, &devices) {
        dev_tmp = list_entry(entry, struct mtd_device, link);

        if ((dev_tmp->id->type == type) && (dev_tmp->id->num == num))
            return dev_tmp;
    }

    return NULL;
}

/**
 * Add specified device to the global device list.
 *
 * @param dev device to be added
 */
static void device_add(struct mtd_device *dev)
{
    u8 current_save_needed = 0;

    if (list_empty(&devices)) {
        current_dev = dev;
        current_partnum = 0;
        current_save_needed = 1;
    }

    list_add_tail(&dev->link, &devices);

    if (current_save_needed > 0)
        current_save();
    else
        index_partitions();
}

/**
 * Parse device type, name and mtd-id. If syntax is ok allocate memory and
 * return pointer to the device structure.
 *
 * @param mtd_dev pointer to the device definition string i.e. <mtd-dev>
 * @param ret output pointer to next char after parse completes (output)
 * @param retdev pointer to the allocated device (output)
 * @return 0 on success, 1 otherwise
 */
static int device_parse(const char *const mtd_dev, const char **ret, struct mtd_device **retdev)
{
    struct mtd_device *dev;
    struct part_info *part;
    struct mtdids *id;
    const char *mtd_id;
    unsigned int mtd_id_len;
    const char *p;//, *pend;
    LIST_HEAD(tmp_list);
    struct list_head *entry, *n;
    u16 num_parts;
    u32 offset;
    int err = 1;

    if(mtd_dev==NULL) return 1;

    p = mtd_dev;
    *retdev = NULL;
    *ret = NULL;

    DEBUGF("===device_parse===\n");

    /* fetch <mtd-id> */
    mtd_id = p;
    p = strchr(mtd_id, ':');
    if (!p) {
        printf("no <mtd-id> identifier\n");
        return 1;
    }
    mtd_id_len = p - mtd_id + 1;
    p+=1;

    /* verify if we have a valid device specified */
    id = id_find_by_mtd_id(mtd_id, mtd_id_len - 1);
    if (id == NULL) {
        printf("invalid mtd device '%.*s'\n", mtd_id_len - 1, mtd_id);
        return 1;
    }

    DEBUGF("dev type = %d (%s), dev num = %d, mtd-id = %s\n",
        id->type, MTD_DEV_TYPE(id->type),
        id->num, id->mtd_id);
    //pend = strchr(p, ';');
    //DEBUGF("parsing partitions %.*s\n", (pend ? pend - p : strlen(p)), p);


    /* parse partitions */
    num_parts = 0;

    offset = 0;
    dev = device_find(id->type, id->num);
    if (dev != NULL) {
        /* if device already exists start at the end of the last partition */
        part = list_entry(dev->parts.prev, struct part_info, link);
        offset = part->offset + part->size;
    }

    while (p && (*p != '\0') && (*p != ';')) {
        err = 1;
        if ((part_parse(p, &p, &part) != 0) || (!part))
            break;

        /* calculate offset when not specified */
        if (part->offset == OFFSET_NOT_SPECIFIED)
            part->offset = offset;
        else
            offset = part->offset;

        /* verify alignment and size */
        if (part_validate(id, part) != 0)
            break;

        offset += part->size;

        /* partition is ok, add it to the list */
        list_add_tail(&part->link, &tmp_list);
        num_parts++;
        err = 0;
    }
    if (err == 1) {
        part_delall(&tmp_list);
        return 1;
    }

    if (num_parts == 0) {
        printf("no partitions for device %s%d (%s)\n",
            MTD_DEV_TYPE(id->type), id->num, id->mtd_id);
        return 1;
    }

    DEBUGF("\ntotal partitions: %d\n", num_parts);

    /* check for next device presence */
    if (p) {
        if (*p == ';') {
            p+=1;
            *ret = p;
        }
        else if (*p == '\0') {
            *ret = p;
        }
        else {
            printf("unexpected character '%c' at the end of device\n", *p);
            *ret = NULL;
            return 1;
        }
    }

    /* allocate memory for mtd_device structure */
    dev = (struct mtd_device *)malloc(sizeof(struct mtd_device));
    if (dev == NULL) {
        printf("out of memory\n");
        return 1;
    }
    memset(dev, 0, sizeof(struct mtd_device));
    dev->id = id;
    dev->num_parts = 0; /* part_sort_add increments num_parts */
    INIT_LIST_HEAD(&dev->parts);
    INIT_LIST_HEAD(&dev->link);

    /* move partitions from tmp_list to dev->parts */
    list_for_each_safe(entry, n, &tmp_list) {
        part = list_entry(entry, struct part_info, link);
        list_del(entry);
        if (part_sort_add(dev, part) != 0) {
            device_del(dev);
            return 1;
        }
    }

    *retdev = dev;

    DEBUGF("===\n\n");
    return 0;
}

/**
 * Initialize global device list.
 *
 * @return 0 on success, 1 otherwise
 */
static int devices_init(void)
{
    last_parts[0] = '\0';
    current_dev = NULL;
    current_save();

    return device_delall(&devices);
}

/*
 * Search global mtdids list and find id of requested type and number.
 *
 * @return pointer to the id if it exists, NULL otherwise
 */
static struct mtdids* id_find(u8 type, u8 num)
{
    struct list_head *entry;
    struct mtdids *id;

    list_for_each(entry, &mtdids) {
    id = list_entry(entry, struct mtdids, link);

    if ((id->type == type) && (id->num == num))
        return id;
    }

    return NULL;
}

/**
 * Search global mtdids list and find id of a requested mtd_id.
 *
 * Note: first argument is not null terminated.
 *
 * @param mtd_id string containing requested mtd_id
 * @param mtd_id_len length of supplied mtd_id
 * @return pointer to the id if it exists, NULL otherwise
 */
static struct mtdids* id_find_by_mtd_id(const char *mtd_id, unsigned int mtd_id_len)
{
    struct list_head *entry;
    struct mtdids *id;

    DEBUGF("--- id_find_by_mtd_id: '%.*s' (len = %d)\n",
        mtd_id_len, mtd_id, mtd_id_len);

    list_for_each(entry, &mtdids) {
        id = list_entry(entry, struct mtdids, link);

        DEBUGF("entry: '%s' (len = %d)\n",
        id->mtd_id, strlen(id->mtd_id));

        if (mtd_id_len != strlen(id->mtd_id))
            continue;
        if (strncmp(id->mtd_id, mtd_id, mtd_id_len) == 0)
            return id;
    }

    return NULL;
}
#endif /* #ifdef CONFIG_JFFS2_CMDLINE */

/**
 * Parse device id string <dev-id> := 'nand'|'nor'<dev-num>, return device
 * type and number.
 *
 * @param id string describing device id
 * @param ret_id output pointer to next char after parse completes (output)
 * @param dev_type parsed device type (output)
 * @param dev_num parsed device number (output)
 * @return 0 on success, 1 otherwise
 */
int id_parse(const char *id, const char **ret_id, u8 *dev_type, u8 *dev_num)
{
    const char *p = id;
    char *pp=NULL;

    *dev_type = 0;
    if (strncmp(p, "nand", 4) == 0) {
        *dev_type = MTD_DEV_TYPE_NAND;
        p += 4;
    }
    else if (strncmp(p, "nor", 3) == 0) {
        *dev_type = MTD_DEV_TYPE_NOR;
        p += 3;
    }
    else {
        printf("incorrect device type in %s\n", id);
        return 1;
    }

    if (!isdigit(*p)) {
        printf("incorrect device number in %s\n", id);
        return 1;
    }

    U32 addr = (U32)&p[0];
    pp = (char*)addr;
    *dev_num = simple_strtoul(p, (char **)&pp, 0);
    if (ret_id)
        *ret_id = pp;
    return 0;
}

/**
 * Process all devices and generate corresponding mtdparts string describing
 * all partitions on all devices.
 *
 * @param buf output buffer holding generated mtdparts string (output)
 * @param buflen buffer size
 * @return 0 on success, 1 otherwise
 */
static int generate_mtdparts(char *buf, u32 buflen)
{
    struct list_head *pentry, *dentry;
    struct mtd_device *dev;
    struct part_info *part, *prev_part;
    char *p = buf;
    char tmpbuf[32];
    u32 size, offset, len, part_cnt;
    u32 maxlen = buflen - 1;

    DEBUGF("--- generate_mtdparts ---\n");

    if (list_empty(&devices)) {
        buf[0] = '\0';
        return 0;
    }

    sprintf(p, "mtdparts=");
    p += 9;

    list_for_each(dentry, &devices) {
        dev = list_entry(dentry, struct mtd_device, link);

        /* copy mtd_id */
        len = strlen(dev->id->mtd_id) + 1;
        if (len > maxlen)
            goto cleanup;
        memcpy(p, dev->id->mtd_id, len - 1);
        p += len - 1;
        *p = ':';
        p+=1;
        maxlen -= len;

        /* format partitions */
        prev_part = NULL;
        part_cnt = 0;
        list_for_each(pentry, &dev->parts) {
            part = list_entry(pentry, struct part_info, link);
            size = part->size;
            offset = part->offset;
            part_cnt++;

            /* partition size */
            memsize_format(tmpbuf, size);
            len = strlen(tmpbuf);
            if (len > maxlen)
                goto cleanup;
            memcpy(p, tmpbuf, len);
            p += len;
            maxlen -= len;


            /* add offset only when there is a gap between
            * partitions */
            if (((!prev_part) && (offset != 0)) ||
                ((prev_part) && ((prev_part->offset + prev_part->size) != part->offset))) {

                memsize_format(tmpbuf, offset);
                len = strlen(tmpbuf) + 1;
                if (len > maxlen)
                    goto cleanup;
                *p = '@';
                p+=1;
                memcpy(p, tmpbuf, len - 1);
                p += len - 1;
                maxlen -= len;
            }

            /* copy name only if user supplied */
            if(!part->auto_name) {
                len = strlen(part->name) + 2;
                if (len > maxlen)
                    goto cleanup;

                *p = '(';
                p+=1;
                memcpy(p, part->name, len - 2);
                p += len - 2;
                *p = ')';
                p+=1;
                maxlen -= len;
            }

            /* ro mask flag */
            if (part->mask_flags && MTD_WRITEABLE_CMD) {
                len = 2;
                if (len > maxlen)
                    goto cleanup;
                *p = 'r';
                p+=1;
                *p = 'o';
                p+=1;
                maxlen -= 2;
            }

            /* print ',' separator if there are other partitions
            * following */
            if (dev->num_parts > part_cnt) {
                if (1 > maxlen)
                    goto cleanup;
                *p = ',';
                p+=1;
                maxlen--;
            }
            prev_part = part;
        }
        /* print ';' separator if there are other devices following */
        if (dentry->next != &devices) {
            if (1 > maxlen)
                goto cleanup;
            *p = ';';
            p+=1;
            maxlen--;
        }
    }

    /* we still have at least one char left, as we decremented maxlen at
    * the begining */
    *p = '\0';

    return 0;

cleanup:
    last_parts[0] = '\0';
    return 1;
}

/**
 * Call generate_mtdparts to process all devices and generate corresponding
 * mtdparts string, save it in mtdparts environment variable.
 *
 * @param buf output buffer holding generated mtdparts string (output)
 * @param buflen buffer size
 * @return 0 on success, 1 otherwise
 */
static int generate_mtdparts_save(char *buf, u32 buflen)
{
    int ret;

    ret = generate_mtdparts(buf, buflen);

    if ((buf[0] != '\0') && (ret == 0))
        setenv((char*)"mtdparts", buf);
    else
        setenv((char*)"mtdparts", NULL);

    return ret;
}

/**
 * Format and print out a partition list for each device from global device
 * list.
 */
static void list_partitions(void)
{
    struct list_head *dentry, *pentry;
    struct part_info *part;
    struct mtd_device *dev;
    int part_num;

    DEBUGF("\n---list_partitions---\n");
    list_for_each(dentry, &devices) {
        dev = list_entry(dentry, struct mtd_device, link);
        printf("\ndevice %s%d <%s>, # parts = %d\n",
            MTD_DEV_TYPE(dev->id->type), dev->id->num,
            dev->id->mtd_id, dev->num_parts);
        printf(" #: name\t\t\tsize\t\toffset\t\tmask_flags\n");

        /* list partitions for given device */
        part_num = 0;
        list_for_each(pentry, &dev->parts) {
            part = list_entry(pentry, struct part_info, link);
            printf("%2d: %-20s0x%08x\t0x%08x\t%d\n",
                part_num, part->name, part->size,
                part->offset, part->mask_flags);

            part_num++;
        }
    }
    if (list_empty(&devices))
        printf("no partitions defined\n");

    /* current_dev is not NULL only when we have non empty device list */
    if (current_dev) {
        part = jffs2_part_info(current_dev, current_partnum);
        if (part) {
            //printf("\nactive partition: %s%d,%d - (%s) 0x%08lx @ 0x%08lx\n",
            printf("\nactive partition: %s%ud,%ud - (%s) 0x%08ux @ 0x%08ux\n",
                MTD_DEV_TYPE(current_dev->id->type),
                current_dev->id->num, current_partnum,
                part->name, part->size, part->offset);
        }
        else {
            printf("could not get current partition info\n\n");
        }
    }

    printf("\ndefaults:\n");
    printf("mtdids  : %s\n", mtdids_default);
    printf("mtdparts: %s\n", mtdparts_default);
}

/**
 * Given partition identifier in form of <dev_type><dev_num>,<part_num> find
 * corresponding device and verify partition number.
 *
 * @param id string describing device and partition or partition name
 * @param dev pointer to the requested device (output)
 * @param part_num verified partition number (output)
 * @param part pointer to requested partition (output)
 * @return 0 on success, 1 otherwise
 */
int find_dev_and_part(const char *id, struct mtd_device **dev,
    u8 *part_num, struct part_info **part)
{
    struct list_head *dentry, *pentry;
    u8 type, dnum, pnum;
    const char *p;
    char *pp=NULL;
    U32 addr=0;

    DEBUGF("--- find_dev_and_part ---\nid = %s\n", id);

    list_for_each(dentry, &devices) {
        *part_num = 0;
        *dev = list_entry(dentry, struct mtd_device, link);
        list_for_each(pentry, &(*dev)->parts) {
            *part = list_entry(pentry, struct part_info, link);
            if (strcmp((*part)->name, id) == 0)
                return 0;
            (*part_num)+=1;
        }
    }

    p = id;
    *dev = NULL;
    *part = NULL;
    *part_num = 0;

    if (id_parse(p, &p, &type, &dnum) != 0)
        return 1;

    if ((*p != ',') || (*(p+1) == '\0')) {
        printf("no partition number specified\n");
        return 1;
    }
    p+=1;
    addr = (U32)&p[0];
    pp = (char*)addr;
    pnum = simple_strtoul(p, (char **)&pp, 0);
    if (*pp != '\0') {
        printf("unexpected trailing character '%c'\n", *pp);
        return 1;
    }

    *dev = device_find(type, dnum);
    if (*dev == NULL) {
        printf("no such device %s%d\n", MTD_DEV_TYPE(type), dnum);
        return 1;
    }

    *part = jffs2_part_info(*dev, pnum);
    if (*part == NULL) {
        printf("no such partition\n");
        *dev = NULL;
        return 1;
    }

    *part_num = pnum;

    return 0;
}

/**
 * Find and delete partition. For partition id format see find_dev_and_part().
 *
 * @param id string describing device and partition
 * @return 0 on success, 1 otherwise
 */
static int delete_partition(const char *id)
{
    u8 pnum;
    struct mtd_device *dev;
    struct part_info *part;

    if (find_dev_and_part(id, &dev, &pnum, &part) == 0) {

        DEBUGF("delete_partition: device = %s%d, partition %d = (%s) 0x%08lx@0x%08lx\n",
            MTD_DEV_TYPE(dev->id->type), dev->id->num, pnum,
            part->name, part->size, part->offset);

        if (part_del(dev, part) != 0)
            return 1;

        if (generate_mtdparts_save(last_parts, MTDPARTS_MAXLEN) != 0) {
            printf("generated mtdparts too long, reseting to null\n");
            return 1;
        }
        return 0;
    }

    printf("partition %s not found\n", id);
    return 1;
}

/**
 * Accept character string describing mtd partitions and call device_parse()
 * for each entry. Add created devices to the global devices list.
 *
 * @param mtdparts string specifing mtd partitions
 * @return 0 on success, 1 otherwise
 */
static int parse_mtdparts(const char *const mtdparts)
{
    const char *p = mtdparts;
    struct mtd_device *dev=NULL;
    int err = 1;

    DEBUGF("\n---parse_mtdparts---\nmtdparts = %s\n\n", p);

    /* delete all devices and partitions */
    if (devices_init() != 0) {
        printf("could not initialise device list\n");
        return err;
    }

    /* re-read 'mtdparts' variable, devices_init may be updating env */
    p = getenv("mtdparts");
    if(p==NULL) return err;

    if (strncmp(p, "mtdparts=", 9) != 0) {
        printf("mtdparts variable doesn't start with 'mtdparts='\n");
        return err;
    }
    p += 9;

    //while (p && (*p != '\0')) {
    while ((p?(*p != '\0'):0)) {
        err = 1;
        if ((device_parse(p, &p, &dev) != 0) || (!dev))
            break;

        DEBUGF("+ device: %s\t%d\t%s\n", MTD_DEV_TYPE(dev->id->type),
            dev->id->num, dev->id->mtd_id);

        /* check if parsed device is already on the list */
        if (device_find(dev->id->type, dev->id->num) != NULL) {
            printf("device %s%d redefined, please correct mtdparts variable\n",
                MTD_DEV_TYPE(dev->id->type), dev->id->num);
            device_del(dev);
            break;
        }

        list_add_tail(&dev->link, &devices);
        err = 0;
    }
    if (err == 1) {

        device_delall(&devices);
        return 1;
    }

    return 0;
}

/**
 * Parse provided string describing mtdids mapping (see file header for mtdids
 * variable format). Allocate memory for each entry and add all found entries
 * to the global mtdids list.
 *
 * @param ids mapping string
 * @return 0 on success, 1 otherwise
 */
static int parse_mtdids(const char *const ids)
{
    const char *p = ids;
    const char *mtd_id;
    int mtd_id_len;
    struct mtdids *id;
    struct list_head *entry, *n;
    struct mtdids *id_tmp;
    u8 type, num;
    u32 size;
    int ret = 1;

    DEBUGF("\n---parse_mtdids---\nmtdids = %s\n\n", ids);

    /* clean global mtdids list */
    list_for_each_safe(entry, n, &mtdids) {
        id_tmp = list_entry(entry, struct mtdids, link);
        DEBUGF("mtdids del: %d %d\n", id_tmp->type, id_tmp->num);
        list_del(entry);
        free(id_tmp);
    }
    last_ids[0] = '\0';
    INIT_LIST_HEAD(&mtdids);

    while(p && (*p != '\0')) {

        ret = 1;
        /* parse 'nor'|'nand'<dev-num> */
        if (id_parse(p, &p, &type, &num) != 0)
            break;

        if (*p != '=') {
            printf("mtdids: incorrect <dev-num>\n");
            break;
        }
        p+=1;

        /* check if requested device exists */
        if (device_validate(type, num, &size) != 0)
            return 1;

        /* locate <mtd-id> */
        mtd_id = p;
        p = strchr(mtd_id, ',');
        if (p != NULL) {
            mtd_id_len = p - mtd_id + 1;
            p+=1;
        }
        else {
            mtd_id_len = strlen(mtd_id) + 1;
        }
        if (mtd_id_len == 0) {
            printf("mtdids: no <mtd-id> identifier\n");
            break;
        }

        /* check if this id is already on the list */
        int double_entry = 0;
        list_for_each(entry, &mtdids) {
            id_tmp = list_entry(entry, struct mtdids, link);
            if ((id_tmp->type == type) && (id_tmp->num == num)) {
                double_entry = 1;
                break;
            }
        }
        if (double_entry) {
            printf("device id %s%d redefined, please correct mtdids variable\n",
                MTD_DEV_TYPE(type), num);
            break;
        }

        /* allocate mtdids structure */
        id = (struct mtdids *)malloc(sizeof(struct mtdids) + mtd_id_len);
        if (!id) {
            printf("out of memory\n");
            break;
        }
        memset(id, 0, sizeof(struct mtdids) + mtd_id_len);
        id->num = num;
        id->type = type;
        id->size = size;
        id->mtd_id = (char *)(id + 1);
        strncpy(id->mtd_id, mtd_id, mtd_id_len - 1);
            id->mtd_id[mtd_id_len - 1] = '\0';
        INIT_LIST_HEAD(&id->link);

        DEBUGF("+ id %s%d\t%16d bytes\t%s\n",
            MTD_DEV_TYPE(id->type), id->num,
            id->size, id->mtd_id);

        list_add_tail(&id->link, &mtdids);
        ret = 0;
    }
    if (ret == 1) {
        /* clean mtdids list and free allocated memory */
        list_for_each_safe(entry, n, &mtdids) {
            id_tmp = list_entry(entry, struct mtdids, link);
            list_del(entry);
            free(id_tmp);
        }
        return 1;
    }

    return 0;
}

/**
 * Parse and initialize global mtdids mapping and create global
 * device/partition list.
 *
 * @return 0 on success, 1 otherwise
 */
int mtdparts_init(void)
{
    static int initialized = 0;
    const char *ids, *parts;
    const char *current_partition;
    int ids_changed;
    char tmp_ep[PARTITION_MAXLEN];

    DEBUGF("\n---mtdparts_init---\n");
    if (!initialized) {
        INIT_LIST_HEAD(&mtdids);
        INIT_LIST_HEAD(&devices);
        memset(last_ids, 0, MTDIDS_MAXLEN);
        memset(last_parts, 0, MTDPARTS_MAXLEN);
        memset(last_partition, 0, PARTITION_MAXLEN);
        initialized = 1;
    }

    /* get variables */
    ids = getenv("mtdids");
    parts = getenv("mtdparts");
    current_partition = getenv("partition");

    /* save it for later parsing, cannot rely on current partition pointer
    * as 'partition' variable may be updated during init */
    tmp_ep[0] = '\0';
    if (current_partition)
        strncpy(tmp_ep, current_partition, PARTITION_MAXLEN);

    DEBUGF("last_ids  : %s\n", last_ids);
    DEBUGF("env_ids   : %s\n", ids);
    DEBUGF("last_parts: %s\n", last_parts);
    DEBUGF("env_parts : %s\n\n", parts);

    DEBUGF("last_partition : %s\n", last_partition);
    DEBUGF("env_partition  : %s\n", current_partition);

    /* if mtdids varible is empty try to use defaults */
    if (!ids) {
        if (mtdids_default) {
            char *ptr = NULL;
            U32 addr=0;
            DEBUGF("mtdids variable not defined, using default\n");
            ids = mtdids_default;
            addr = (U32)&ids[0];
            ptr = (char*)addr;
            setenv((char*)"mtdids", (char *)ptr);
        }
        else {
            printf("mtdids not defined, no default present\n");
            return 1;
        }
    }
    if (strlen(ids) > MTDIDS_MAXLEN - 1) {
        printf("mtdids too long (> %d)\n", MTDIDS_MAXLEN);
        return 1;
    }

    /* do no try to use defaults when mtdparts variable is not defined,
    * just check the length */
    //==================================================================
    // Derek modify 2008/07/05 make default partition
    //==================================================================
    if (!parts)
    {
        if (mtdparts_default) {
            char *ptr = NULL;
            U32 addr=0;
            DEBUGF("mtdparts variable not defined, using default\n");
            parts = mtdparts_default;
            addr = (U32)&parts[0];
            ptr = (char*)addr;
            //if(parts==NULL) return 1;
            setenv((char*)"mtdparts", (char *)ptr);
#if (CONFIG_COMMANDS & CFG_CMD_ENV)
            //saveenv(); // save default partition to nand
#endif
        }
        else {
            printf("mtdparts not defined, no default present\n");
            return 1;
        }
    }
    //if (!parts)
    //    printf("mtdparts variable not set, see 'help mtdparts'\n");
    //==================================================================
    if ((strlen(parts) > MTDPARTS_MAXLEN - 1)) {
        //if (parts && (strlen(parts) > MTDPARTS_MAXLEN - 1)) {

        printf("mtdparts too long (> %d)\n", MTDPARTS_MAXLEN);
        return 1;
    }

    /* check if we have already parsed those mtdids */
    if ((last_ids[0] != '\0') && (strcmp(last_ids, ids) == 0)) {
        ids_changed = 0;
    }
    else {
        ids_changed = 1;

        if (parse_mtdids(ids) != 0) {
            devices_init();
            return 1;
        }

        /* ok it's good, save new ids */
        //strncpy(last_ids, ids, MTDIDS_MAXLEN);
        if(strlen(ids)>MTDIDS_MAXLEN){
            strncpy(last_ids, ids, MTDIDS_MAXLEN-1);
            last_ids[MTDIDS_MAXLEN-1]='\0';
        }
        else
            strncpy(last_ids, ids, MTDIDS_MAXLEN);
    }

    /* parse partitions if either mtdparts or mtdids were updated */
    //if (parts && ((last_parts[0] == '\0') || ((strcmp(last_parts, parts) != 0)) || ids_changed)) {
    if (((last_parts[0] == '\0') || ((strcmp(last_parts, parts) != 0)) || ids_changed)) {
        if (parse_mtdparts(parts) != 0)
            return 1;

        if (list_empty(&devices)) {
            printf("mtdparts_init: no valid partitions\n");
            return 1;
        }

        /* ok it's good, save new parts */
        //strncpy(last_parts, parts, MTDPARTS_MAXLEN);
        if(strlen(parts)<MTDPARTS_MAXLEN)
            strncpy(last_parts, parts, strlen(parts)+1);
        else{
            strncpy(last_parts, parts, MTDPARTS_MAXLEN-1);
            last_parts[MTDPARTS_MAXLEN-1]='\0';
        }

        /* reset first partition from first dev from the list as current */
        current_dev = list_entry(devices.next, struct mtd_device, link);
        current_partnum = 0;
        current_save();

        DEBUGF("mtdparts_init: current_dev  = %s%d, current_partnum = %d\n",
            MTD_DEV_TYPE(current_dev->id->type),
            current_dev->id->num, current_partnum);
    }

    /* mtdparts variable was reset to NULL, delete all devices/partitions */
    /*if (!parts && (last_parts[0] != '\0'))
        return devices_init();*/

    /* do not process current partition if mtdparts variable is null */
    /*if (!parts)
        return 0;*/

    /* is current partition set in environment? if so, use it */
    if ((tmp_ep[0] != '\0') && (strcmp(tmp_ep, last_partition) != 0)) {
        struct part_info *p;
        struct mtd_device *cdev;
        u8 pnum;

        DEBUGF("--- getting current partition: %s\n", tmp_ep);

        if (find_dev_and_part(tmp_ep, &cdev, &pnum, &p) == 0) {
            current_dev = cdev;
            current_partnum = pnum;
            current_save();
        }
    }
    else if (getenv("partition") == NULL) {
        DEBUGF("no partition variable set, setting...\n");
            current_save();
    }

    return 0;
}

/**
 * Return pointer to the partition of a requested number from a requested
 * device.
 *
 * @param dev device that is to be searched for a partition
 * @param part_num requested partition number
 * @return pointer to the part_info, NULL otherwise
 */
struct part_info* jffs2_part_info(struct mtd_device *dev, unsigned int part_num)
{
    struct list_head *entry;
    struct part_info *part;
    int num;

    if (!dev)
        return NULL;

    DEBUGF("\n--- jffs2_part_info: partition number %d for device %s%d (%s)\n",
        part_num, MTD_DEV_TYPE(dev->id->type),
        dev->id->num, dev->id->mtd_id);

    if (part_num >= dev->num_parts) {
        printf("invalid partition number %d for device %s%d (%s)\n",
            part_num, MTD_DEV_TYPE(dev->id->type),
            dev->id->num, dev->id->mtd_id);
        return NULL;
    }

    /* locate partition number, return it */
    num = 0;
    list_for_each(entry, &dev->parts) {
        part = list_entry(entry, struct part_info, link);

        if (part_num == num) {
            return part;
        }
        num+=1;
    }

    return NULL;
}

/**
 * Routine implementing u-boot mtdparts command. Initialize/update default global
 * partition list and process user partition request (list, add, del).
 *
 * @param cmdtp command internal data
 * @param flag command flag
 * @param argc number of arguments supplied to the command
 * @param argv arguments list
 * @return 0 on success, 1 otherwise
 */
int do_jffs2_mtdparts(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    if (argc == 2) {
        if (strcmp(argv[1], "default") == 0) {
            U32 addr=0;
            char *p_mtdids = NULL;
            char *p_mtdparts = NULL;
            addr = (U32)&mtdids_default[0];
            p_mtdids = (char*)addr;
            setenv((char*)"mtdids", (char *)p_mtdids);
            addr = (U32)&mtdparts_default[0];
            p_mtdparts = (char*)addr;
            setenv((char*)"mtdparts", (char *)p_mtdparts);
            setenv((char*)"partition", NULL);


            if (mtdparts_init() != 0)
                return 1;

            return 0;
        }
        else if (strcmp(argv[1], "delall") == 0) {
            /* this may be the first run, initialize lists if needed */
            if (mtdparts_init() != 0)
                return 1;

            setenv((char*)"mtdparts", NULL);

            /* devices_init() calls current_save() */
            return devices_init();
        }
    }

    /* make sure we are in sync with env variables */
    if (mtdparts_init() != 0)
        return 1;

    if (argc == 1) {
        list_partitions();
        return 0;
    }

    /* mtdparts add <mtd-dev> <size>[@<offset>] <name> [ro] */
    if (((argc == 5) || (argc == 6)) && (strcmp(argv[1], "add") == 0)) {
#define PART_ADD_DESC_MAXLEN 64
        char tmpbuf[PART_ADD_DESC_MAXLEN];
        u8 type, num, len;
        struct mtd_device *dev = NULL;
        struct mtd_device *dev_tmp = NULL;
        struct mtdids *id = NULL;
        struct part_info *p = NULL;
        //=====================================================
        // Derek modify 2008/07/04, for device_parse use.
        //=====================================================
//        char dummy[]=(const char []){"dummy\0"};
        const char *dummy = "dummy\0";
        const char *dummy2 = dummy;
        //=====================================================
        if (id_parse(argv[2], NULL, &type, &num) != 0)
            return 1;

        id = id_find(type, num);
        if (id == NULL) {
            printf("no such device %s defined in mtdids variable\n", argv[2]);
            return 1;
        }

        len = strlen(id->mtd_id) + 1; /* 'mtd_id:' */
        len += strlen(argv[3]); /* size@offset */
        len += strlen(argv[4]) + 2; /* '(' name ')' */
        if (argv[5] && (strlen(argv[5]) == 2))
            len += 2; /* 'ro' */

        if (len >= PART_ADD_DESC_MAXLEN) {
            printf("too long partition description\n");
            return 1;
        }
        sprintf(tmpbuf, "%s:%s(%s)%s",
        id->mtd_id, argv[3], argv[4], argv[5] ? argv[5] : "");
        DEBUGF("add tmpbuf: %s\n", tmpbuf);
        //=====================================================
        // Derek modify 2008/07/04, NULL will cause system hang
        //=====================================================
        //if ((device_parse(tmpbuf, NULL, &dev) != 0) || (!dev))
        if ((device_parse(tmpbuf, &dummy2, &dev) != 0) || (!dev))
        {
            return 1;
        }

        DEBUGF("+ %s\t%d\t%s\n", MTD_DEV_TYPE(dev->id->type),
        dev->id->num, dev->id->mtd_id);

        dev_tmp = device_find(dev->id->type, dev->id->num);
        if (dev_tmp == NULL) {
            device_add(dev);
        }
        else {
            /* merge new partition with existing ones*/
            p = list_entry(dev->parts.next, struct part_info, link);
            if (part_add(dev_tmp, p) != 0) {
                device_del(dev);
                return 1;
            }
        }

        if (generate_mtdparts_save(last_parts, MTDPARTS_MAXLEN) != 0) {
            printf("generated mtdparts too long, reseting to null\n");
            device_del(dev);
            return 1;
        }

        return 0;
    }

    /* mtdparts del part-id */
    if ((argc == 3) && (strcmp(argv[1], "del") == 0)) {
        DEBUGF("del: part-id = %s\n", argv[2]);

        return delete_partition(argv[2]);
    }

    printf ("Usage:\n%s\n", cmdtp->usage);
    return 1;
}

#define ENV_DYNPARTS "dynparts"
#define MTDPARTS_STR_HEADER "mtdparts="
#if (CONFIG_COMMANDS & CFG_CMD_NAND)
extern nand_info_t nand_info[];
#endif

static int parse_dynparts(const char *const dynparts, char *mtdparts)
{
    int err;
    const char *p;
    const char *mtd_id;
    unsigned int mtd_id_len;
    struct mtdids *id;
#if (CONFIG_COMMANDS & CFG_CMD_NAND)
    nand_info_t *nand;
    u32 i;
#endif
    struct part_info *part;
    u16 num_parts;
    u32 offset;
    char *size_suffix;
    u32 part_size;
    char *last_mtdparts;

    err = 1;
    p = dynparts;

    /* check dynparts length */
    if (strlen(dynparts) > MTDPARTS_MAXLEN)
    {
        goto exit;
    }

    /* fetch <mtd-id> */
    mtd_id = p;

    p = strchr(mtd_id, ':');
    if (p == NULL)
    {
        goto exit;
    }

    mtd_id_len = p - mtd_id + 1;
    p++;

    id = id_find_by_mtd_id(mtd_id, mtd_id_len - 1);
    if ( (id == NULL)
        || ((id->type != MTD_DEV_TYPE_NAND)  && (id->type != MTD_DEV_TYPE_NOR))) // verify if we have a valid device specified
    {
        goto exit;
    }

    DEBUGF("dev type = %d (%s), dev num = %d, mtd-id = %s\n",
        id->type,
        MTD_DEV_TYPE(id->type),
        id->num,
        id->mtd_id
    );

    strcpy(mtdparts,MTDPARTS_STR_HEADER);       // "mtdparts="
    mtdparts += strlen(MTDPARTS_STR_HEADER);

    strncpy(mtdparts, mtd_id, mtd_id_len);      // <mtd-id>
    mtdparts += mtd_id_len;

    /* parse partitions */
    num_parts = 0;
    offset = 0;

    while (p && (*p != '\0') && (*p != ';'))
    {
        if ((part_parse(p, &p, &part) != 0) || (!part))
        {
            break;
        }

        if (part->offset == OFFSET_NOT_SPECIFIED) // calculate offset when not specified
        {
            part->offset = offset;
        }
        else
        {
            offset = part->offset;
        }

        if (part_validate(id, part) != 0)
        {
            err = 1;
            break;
        }

#if (CONFIG_COMMANDS & CFG_CMD_NAND)
        nand = &nand_info[id->num];
        for (i = offset; (i < (offset + part->size)) && (i < nand->size); i += nand->erasesize)
        {
            if (nand->block_isbad(nand, i))
            {
                DEBUGF("bad block @ 0x%08X\n", i);
                //part->size += nand->erasesize;
            }
        }
#endif

        part_size = part->size;
        if (1) // ((part_size) % 1024 == 0) // mark it for optimization (because size must be aligned to erase size)
        {
            part_size /= 1024;

            if ((part_size) % 1024 == 0)
            {
                part_size /= 1024;

                if ((part_size) % 1024 == 0)
                {
                    part_size /= 1024;
                    size_suffix = (char*)"g";
                }
                else
                {
                    size_suffix = (char*)"m";
                }
            }
            else
            {
                size_suffix = (char*)"k";
            }
        }
        else
        {
            size_suffix = "";
        }

        DEBUGF("%X\t%X\t%d%s\t%s\n",part->offset, part->size, part_size, size_suffix, part->name);

        sprintf(mtdparts, "%d%s(%s),", part_size, size_suffix, part->name);
        last_mtdparts = mtdparts; // keep for the last partition
        mtd_id_len = strlen(mtdparts);
        mtdparts += mtd_id_len;

        offset += part->size;
        num_parts++;

        err = 0;
    }

    if (err == 0)
    {
        sprintf(last_mtdparts, "-(%s)", part->name); // set the last partition size to SIZE_REMAINING
    }

exit:
    return err;
}

int do_dynpart(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char *dynparts;
    char *mtdparts;
    int err;

    err = 1;

    /* check <mtd-def>/'dynparts' first */
    dynparts = getenv(ENV_DYNPARTS);
    if (argc > 1)
    {
        dynparts = argv[1];
    }
    else if (dynparts == NULL)
    {
        printf("Error: set'"ENV_DYNPARTS"' first!!!\n");
        goto exit;
    }

    /* allocate temp buffer */
    mtdparts = malloc(MTDPARTS_MAXLEN);
    if (mtdparts == NULL)
    {
        printf("Error: malloc for temp buffer fail!!!\n");
        goto exit;
    }

    /* parse dynparts */
    if (parse_dynparts(dynparts, mtdparts) == 0) // OK
    {
        setenv((char*)"mtdparts", mtdparts);
        printf("%s\n", mtdparts);
        err = 0;
    }
    else
    {
        printf("Error: parse '"ENV_DYNPARTS"' fail!!!\n");
    }

    if (mtdparts != NULL)
    {
        free(mtdparts);
    }

exit:
    return err;
}

/***************************************************/
U_BOOT_CMD(
    mtdparts, 6, 0, do_jffs2_mtdparts,
    "mtdparts- define flash/nand partitions\n",
    "\n"
    "    - list partition table\n"
    "mtdparts delall\n"
    "    - delete all partitions\n"
    "mtdparts del part-id\n"
    "    - delete partition (e.g. part-id = nand0,1)\n"
    "mtdparts add <mtd-dev> <size>[@<offset>] [<name>] [ro]\n"
    "    - add partition\n"
    "mtdparts default\n"
    "    - reset partition table to defaults\n\n"
    "-----\n\n"
    "this command uses three environment variables:\n\n"
    "'partition' - keeps current partition identifier\n\n"
    "partition  := <part-id>\n"
    "<part-id>  := <dev-id>,part_num\n\n"
    "'mtdids' - linux kernel mtd device id <-> u-boot device id mapping\n\n"
    "mtdids=<idmap>[,<idmap>,...]\n\n"
    "<idmap>    := <dev-id>=<mtd-id>\n"
    "<dev-id>   := 'nand'|'nor'<dev-num>\n"
    "<dev-num>  := mtd device number, 0...\n"
    "<mtd-id>   := unique device tag used by linux kernel to find mtd device (mtd->name)\n\n"
    "'mtdparts' - partition list\n\n"
    "mtdparts=mtdparts=<mtd-def>[;<mtd-def>...]\n\n"
    "<mtd-def>  := <mtd-id>:<part-def>[,<part-def>...]\n"
    "<mtd-id>   := unique device tag used by linux kernel to find mtd device (mtd->name)\n"
    "<part-def> := <size>[@<offset>][<name>][<ro-flag>]\n"
    "<size>     := standard linux memsize OR '-' to denote all remaining space\n"
    "<offset>   := partition start offset within the device\n"
    "<name>     := '(' NAME ')'\n"
    "<ro-flag>  := when set to 'ro' makes partition read-only (not used, passed to kernel)\n"
);

U_BOOT_CMD(
    dynpart, 2, 1, do_dynpart,
    "dynpart [<mtd-def>]\t- dynamically calculate partition table based on <mtd-def>/'"ENV_DYNPARTS"' and BBT (it is better to set the last partition size to SIZE_REMAINING)\n",
    "[<mtd-def>]\n"
    "    - sets 'mtdparts' according to <mtd-def>/'"ENV_DYNPARTS"' and BBT (it is better to set the last partition size to SIZE_REMAINING)\n"
);

/***************************************************/
