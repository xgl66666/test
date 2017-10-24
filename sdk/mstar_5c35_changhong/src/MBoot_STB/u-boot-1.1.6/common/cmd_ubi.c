/*
 * Unsorted Block Image commands
 *
 *  Copyright (C) 2008 Samsung Electronics
 *  Kyungmin Park <kyungmin.park@samsung.com>
 *
 * Copyright 2008-2009 Stefan Roese <sr@denx.de>, DENX Software Engineering
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include <command.h>
#include <linux/stddef.h>
#include <linux/string.h>
#if (CONFIG_COMMANDS & CFG_CMD_UBI)

#include <exports.h>
#include <nand.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <ubi_uboot.h>
#include <asm/errno.h>
#include <jffs2/load_kernel.h>
#include "uboot_mmap.h"

#define DEV_TYPE_NONE		0
#define DEV_TYPE_NAND		1
#define DEV_TYPE_ONENAND	2
#define DEV_TYPE_NOR		3


extern int mtdparts_init(void);
extern int id_parse(const char *id, const char **ret_id, u8 *dev_type, u8 *dev_num);
extern int find_dev_and_part(const char *id, struct mtd_device **dev,
        u8 *part_num, struct part_info **part);

extern struct mtd_info *get_mtd_device_nm(const char *name);

extern int strcmp(const char * cs,const char * ct);

extern char * strncpy(char * dest,const char *src,size_t count);

/* Private own data */
static struct ubi_device *ubi=NULL;
static char buffer[80];
static int ubi_initialized=0;

struct selected_dev {
	char part_name[80];
	int selected;
	int nr;
	struct mtd_info *mtd_info;
};

static struct selected_dev ubi_dev;

static void ubi_dump_vol_info(const struct ubi_volume *vol)
{
    printf("volume information dump:");
    printf("vol_id          %d", vol->vol_id);
    printf("reserved_pebs   %d", vol->reserved_pebs);
    printf("alignment       %d", vol->alignment);
    printf("data_pad        %d", vol->data_pad);
    printf("vol_type        %d", vol->vol_type);
    printf("name_len        %d", vol->name_len);
    printf("usable_leb_size %d", vol->usable_leb_size);
    printf("used_ebs        %d", vol->used_ebs);
    printf("used_bytes      %lld", vol->used_bytes);
    printf("last_eb_bytes   %d", vol->last_eb_bytes);
    printf("corrupted       %d", vol->corrupted);
    printf("upd_marker      %d", vol->upd_marker);

	if (vol->name_len <= UBI_VOL_NAME_MAX &&
		strnlen(vol->name, vol->name_len + 1) == vol->name_len) {
        printf("name            %s", vol->name);
	} else {
        printf("the 1st 5 characters of the name: %c%c%c%c%c",
				vol->name[0], vol->name[1], vol->name[2],
				vol->name[3], vol->name[4]);
	}
	printf("\n");
}

static void display_volume_info(struct ubi_device *ubi)
{
	int i;

	for (i = 0; i < (ubi->vtbl_slots + 1); i++) {
		if (!ubi->volumes[i])
			continue;	/* Empty record */
		ubi_dump_vol_info(ubi->volumes[i]);
	}
}

static void display_ubi_info(struct ubi_device *ubi)
{
	ubi_msg("MTD device name:            \"%s\"", ubi->mtd->name);
    ubi_msg("MTD device size:            %ld MiB", (int)(ubi->flash_size >> 20));
    printf("physical eraseblock size:   %d bytes (%d KiB)",
    		ubi->peb_size, ubi->peb_size >> 10);
    printf("logical eraseblock size:    %d bytes", ubi->leb_size);
    printf("number of good PEBs:        %d", ubi->good_peb_count);
    printf("number of bad PEBs:         %d", ubi->bad_peb_count);
    printf("smallest flash I/O unit:    %d", ubi->min_io_size);
    printf("VID header offset:          %d (aligned %d)",
    		ubi->vid_hdr_offset, ubi->vid_hdr_aloffset);
    printf("data offset:                %d", ubi->leb_start);
    printf("max. allowed volumes:       %d", ubi->vtbl_slots);
    printf("wear-leveling threshold:    %d", CONFIG_MTD_UBI_WL_THRESHOLD);
    printf("number of internal volumes: %d", UBI_INT_VOL_COUNT);
    printf("number of user volumes:     %d",
    		ubi->vol_count - UBI_INT_VOL_COUNT);
    printf("available PEBs:             %d", ubi->avail_pebs);
    printf("total number of reserved PEBs: %d", ubi->rsvd_pebs);
    printf("number of PEBs reserved for bad PEB handling: %d",
    		ubi->beb_rsvd_pebs);
    printf("max/mean erase counter: %d/%d", ubi->max_ec, ubi->mean_ec);
}

static int ubi_info(int layout)
{
	if (layout)
		display_volume_info(ubi);
	else
		display_ubi_info(ubi);

	return 0;
}

static int verify_mkvol_req(const struct ubi_device *ubi,
			    const struct ubi_mkvol_req *req)
{
	int n, err = -EINVAL;

	if (req->bytes < 0 || req->alignment < 0 || req->vol_type < 0 ||
	    req->name_len < 0)
		goto bad;

	if ((req->vol_id < 0 || req->vol_id >= ubi->vtbl_slots) &&
	    req->vol_id != UBI_VOL_NUM_AUTO)
		goto bad;

	if (req->alignment == 0)
		goto bad;

	if (req->bytes == 0)
		goto bad;

	if (req->vol_type != UBI_DYNAMIC_VOLUME &&
	    req->vol_type != UBI_STATIC_VOLUME)
		goto bad;

	if (req->alignment > ubi->leb_size)
		goto bad;

	n = req->alignment % ubi->min_io_size;
	if (req->alignment != 1 && n)
		goto bad;

	if (req->name_len > UBI_VOL_NAME_MAX) {
		err = -ENAMETOOLONG;
		goto bad;
	}

	return 0;
bad:
	printf("bad volume creation request");
	return err;
}

static int ubi_create_vol(char *volume, int size, int dynamic)
{
	struct ubi_mkvol_req req;
	int err;

	if (dynamic)
		req.vol_type = UBI_DYNAMIC_VOLUME;
	else
		req.vol_type = UBI_STATIC_VOLUME;

	req.vol_id = UBI_VOL_NUM_AUTO;
	req.alignment = 1;
	req.bytes = size;

	//strcpy(req.name, volume);
	if(strlen(volume)>=sizeof(req.name)){
    	strncpy(req.name, volume, sizeof(req.name)-1);
        req.name[sizeof(req.name)-1]='\0';
    }
    else{
        strcpy(req.name, volume);
    }
	req.name_len = strlen(volume);
	req.name[req.name_len] = '\0';
	req.padding1 = 0;
	/* It's duplicated at drivers/mtd/ubi/cdev.c */
	err = verify_mkvol_req(ubi, &req);
	if (err) {
		printf("verify_mkvol_req failed %d\n", err);
		return err;
	}
	printf("Creating %s volume %s of size %d\n",
		dynamic ? "dynamic" : "static", volume, size);
	/* Call real ubi create volume */
	return ubi_create_volume(ubi, &req);
}

#define VOLUME_BLACK_MAX 10
#define VOLUME_BLACK_NAME_MAX 32
static char g_volume_black_list[VOLUME_BLACK_MAX][VOLUME_BLACK_NAME_MAX];
static int g_volume_black_cnt = 0;

static int ubi_dontoverwrite_init(void)
{
    memset(g_volume_black_list, 0, (VOLUME_BLACK_MAX * VOLUME_BLACK_NAME_MAX));
    g_volume_black_cnt = 0;
    return 0;
}

static int ubi_dontoverwrite_vol(char *volume)
{
    int err=-EINVAL, length=0, idx=0;
    if((volume == NULL) || (strlen(volume)<1))
    {
        return err;
    }
    for(idx=0; (idx<VOLUME_BLACK_MAX)&&(idx<g_volume_black_cnt); idx++)
    {
        if((strlen(volume)==strlen(g_volume_black_list[idx])) && (0 == strncmp(volume, g_volume_black_list[idx], strlen(volume))))
        {
            return 0;
        }
    }
    {
        // check if volume cound be mount
        char buf[128];
        extern int snprintf(char *str, size_t size, const char *fmt, ...);
        memset(buf, 0 , sizeof(buf));
        snprintf(buf, sizeof(buf), "ubifsmount %s", volume);
        err=run_command(buf, 0);
        run_command("ubi reinit", 0);
        if(err==-1)
        {
            printf("cmd %s failed\n", buf);
            return 0; //return err;
        }
    }
    length = MIN(strlen(volume), VOLUME_BLACK_NAME_MAX-1);
    memset(g_volume_black_list[g_volume_black_cnt], 0, sizeof(VOLUME_BLACK_NAME_MAX));
    strncpy(g_volume_black_list[g_volume_black_cnt], volume, length);
    g_volume_black_cnt++;
#if 0 //debug
    {
        int idx=0;
        printf("cnt: %d\n", g_volume_black_cnt);
        for(idx=0; (idx<VOLUME_BLACK_MAX)&&(idx<g_volume_black_cnt); idx++)
        {
            printf("vol(%d): %s\n", idx, g_volume_black_list[idx]);
        }
    }
#endif
    return 0;
}

static int ubi_dontoverwrite_vol_check(const char *volume)
{
    int err=-EINVAL, idx=0;
    if((volume == NULL) || (strlen(volume)<1))
    {
        return err;
    }
    for(idx=0; (idx<VOLUME_BLACK_MAX)&&(idx<g_volume_black_cnt); idx++)
    {
        if((strlen(volume)==strlen(g_volume_black_list[idx])) && (0 == strncmp(volume, g_volume_black_list[idx], strlen(volume))))
        {
            return 0;
        }
    }
#if 0 //debug
    {
        int idx=0;
        printf("cnt: %d\n", g_volume_black_cnt);
        for(idx=0; (idx<VOLUME_BLACK_MAX)&&(idx<g_volume_black_cnt); idx++)
        {
            printf("vol(%d): %s\n", idx, g_volume_black_list[idx]);
        }
    }
#endif
    return err;
}

int ubi_get_black_volume_cnt(void)
{
    return g_volume_black_cnt;
}

int ubi_check_black_volume(const char *volume)
{
    return ubi_dontoverwrite_vol_check(volume);
}

static int ubi_remove_vol(char *volume)
{
	int i, err, reserved_pebs;
	int found = 0, vol_id = 0;
	struct ubi_volume *vol = NULL;

	for (i = 0; i < ubi->vtbl_slots; i++) {
		vol = ubi->volumes[i];
		if (vol && !strcmp(vol->name, volume)) {
			printf("Volume %s found at valid %d\n", volume, i);
			vol_id = i;
			found = 1;
			break;
		}
	}
	if (!found) {
		printf("%s volume not found\n", volume);
		return -ENODEV;
	}
	printf("remove UBI volume %s (id %d)\n", vol->name, vol->vol_id);

	if (ubi->ro_mode) {
		printf("It's read-only mode\n");
		err = -EROFS;
		goto out_err;
	}

	err = ubi_change_vtbl_record(ubi, vol_id, NULL);
	if (err) {
		printf("Error changing Vol tabel record err=%x\n", err);
		goto out_err;
	}
	reserved_pebs = vol->reserved_pebs;
	for (i = 0; i < vol->reserved_pebs; i++) {
		err = ubi_eba_unmap_leb(ubi, vol, i);
		if (err)
			goto out_err;
	}

	if(vol->eba_tbl)
	{
		kfree(vol->eba_tbl);
	}
	ubi->volumes[vol_id]->eba_tbl = NULL;
	ubi->volumes[vol_id] = NULL;

	ubi->rsvd_pebs -= reserved_pebs;
	ubi->avail_pebs += reserved_pebs;
	i = ubi->beb_rsvd_level - ubi->beb_rsvd_pebs;
	if (i > 0) {
		i = ubi->avail_pebs >= i ? i : ubi->avail_pebs;
		ubi->avail_pebs -= i;
		ubi->rsvd_pebs += i;
		ubi->beb_rsvd_pebs += i;
		if (i > 0)
			ubi_msg("reserve more %d PEBs", i);
	}
	ubi->vol_count -= 1;

	return 0;
out_err:
	ubi_err("cannot remove volume %d, error %d", vol_id, err);
	return err;
}


static int ubi_volume_write_cont(char *volume, void *buf, size_t size)
{
	int i = 0, err = -1;
//	int rsvd_bytes = 0;
	int found = 0;
	struct ubi_volume *vol;

	for (i = 0; i < ubi->vtbl_slots; i++) {
		vol = ubi->volumes[i];
		if (vol && !strcmp(vol->name, volume)) {
			printf("Volume \"%s\" found at volume id %d\n", volume, i);
			found = 1;
			break;
		}
	}
	if (!found) {
		printf("%s volume not found\n", volume);
		return 1;
	}

/*
	rsvd_bytes = vol->reserved_pebs * (ubi->leb_size - vol->data_pad);
	if (size < 0 || size > rsvd_bytes) {
		printf("rsvd_bytes=%d vol->reserved_pebs=%d ubi->leb_size=%d\n",
		     rsvd_bytes, vol->reserved_pebs, ubi->leb_size);
		printf("vol->data_pad=%d\n", vol->data_pad);
		printf("Size > volume size !!\n");
		return 1;
	}

	err = ubi_start_update(ubi, vol, size);
	if (err < 0) {
		printf("Cannot start volume update=%d\n",err);
		return err;
	}
*/

	err = ubi_more_update_data(ubi, vol, buf, size);
	if (err < 0) {
		printf("Couldnt or partially wrote data \n");
		return err;
	}

	if (err) {
		size = err;

		err = ubi_check_volume(ubi, vol->vol_id);
		if ( err < 0 )
			return err;

		if (err) {
			ubi_warn("volume %d on UBI device %d is corrupted",
					vol->vol_id, ubi->ubi_num);
			vol->corrupted = 1;
		}

		vol->checked = 1;
		ubi_gluebi_updated(vol);
	}

	return 0;
}

static int ubi_volume_write(char *volume, void *buf, size_t size, size_t total_size)
{
	int i = 0, err = -1;
	int rsvd_bytes = 0;
	int found = 0;
	struct ubi_volume *vol;

	for (i = 0; i < ubi->vtbl_slots; i++) {
		vol = ubi->volumes[i];
		if (vol && !strcmp(vol->name, volume)) {
			printf("Volume \"%s\" found at volume id %d\n", volume, i);
			found = 1;
			break;
		}
	}
	if (!found) {
		printf("%s volume not found\n", volume);
		return 1;
	}
	rsvd_bytes = vol->reserved_pebs * (ubi->leb_size - vol->data_pad);
	//if (size < 0 || size > rsvd_bytes) {
	if (size > rsvd_bytes) {
		printf("rsvd_bytes=%d vol->reserved_pebs=%d ubi->leb_size=%d\n",
		     rsvd_bytes, vol->reserved_pebs, ubi->leb_size);
		printf("vol->data_pad=%d\n", vol->data_pad);
		printf("Size > volume size !!\n");
		return 1;
	}

	err = ubi_start_update(ubi, vol, total_size);
	if (err < 0) {
		printf("Cannot start volume update=%d\n",err);
		return err;
	}

	err = ubi_more_update_data(ubi, vol, buf, size);
	if (err < 0) {
		printf("Couldnt or partially wrote data \n");
		return err;
	}

	if (err) {
		size = err;

		err = ubi_check_volume(ubi, vol->vol_id);
		if ( err < 0 )
			return err;

		if (err) {
			ubi_warn("volume %d on UBI device %d is corrupted",
					vol->vol_id, ubi->ubi_num);
			vol->corrupted = 1;
		}

		vol->checked = 1;
		ubi_gluebi_updated(vol);
	}

	return 0;
}


static int ubi_volume_partial_read(char *volume, char *buf, size_t size, size_t partition_offset)
{
	int err, lnum, off, len, tbuf_size, i = 0;
	size_t count_save = size;
	void *tbuf;
	unsigned long long tmp;
	struct ubi_volume *vol = NULL;
	loff_t offp = 0;
        size_t offset;
        size_t bnum;

	for (i = 0; i < ubi->vtbl_slots; i++) {
		vol = ubi->volumes[i];
		if (vol && !strcmp(vol->name, volume)) {
			printf("Volume %s found at volume id %d\n",
				volume, vol->vol_id);
			break;
		}
	}
	if ((i == ubi->vtbl_slots)||(vol==NULL)) {
		printf("%s volume not found\n", volume);
		return -ENODEV;
	}

	printf("read %d bytes from volume %d to %x(buf address)\n",
	       (int) size, vol->vol_id, (unsigned)buf);

	if (vol->updating) {
		printf("updating");
		return -EBUSY;
	}
	if (vol->upd_marker) {
		printf("damaged volume, update marker is set");
		return -EBADF;
	}
	if (offp == vol->used_bytes)
		return 0;

	if (size == 0) {
		printf("Read [%lu] bytes\n", (unsigned long) vol->used_bytes);
		size = vol->used_bytes;
	}

	if (vol->corrupted)
		printf("read from corrupted volume %d", vol->vol_id);
	if (offp + size > vol->used_bytes)
		count_save = size = vol->used_bytes - offp;

	tbuf_size = vol->usable_leb_size;
	if (size < tbuf_size)
		tbuf_size = ALIGN(size, ubi->min_io_size);
	tbuf = malloc(tbuf_size);
	if (!tbuf) {
		printf("NO MEM\n");
		return -ENOMEM;
	}
	len = size > tbuf_size ? tbuf_size : size;

	tmp = offp;
        offset=partition_offset%len;
        bnum=partition_offset/len;
	off = offset;//do_div(tmp, vol->usable_leb_size);
	lnum = tmp;
#if 1
    do {
		if (off + len >= vol->usable_leb_size)
			len = vol->usable_leb_size - off;

		err = ubi_eba_read_leb(ubi, vol, (lnum+bnum), tbuf, off, len, 0);

		if (err) {
			printf("read err %x\n", err);
			break;
		}
		off += len;
		if (off == vol->usable_leb_size) {
			lnum += 1;
			off -= vol->usable_leb_size;
		}

		size -= len;
		offp += len;

		memcpy(buf, tbuf, len);

		buf += len;
		len = size > tbuf_size ? tbuf_size : size;
	} while (size);
		if (err) {
			printf("read err %x\n", err);
		}


#else
    do {
		if (off + len >= vol->usable_leb_size)
			len = vol->usable_leb_size - off;

		err = ubi_eba_read_leb(ubi, vol, lnum, tbuf, off, len, 0);
        printf("\n\n1111====off=0x%x,len=0x%x,lnum=0x%x, tbuf=0x%x=====\n\n",off,len,lnum,tbuf);
		if (err) {
			printf("read err %x\n", err);
			break;
		}
		off += len;
		if (off == vol->usable_leb_size) {
			lnum += 1;
			off -= vol->usable_leb_size;
        printf("\n\n2222====off=0x%x,len=0x%x,vol->usable_leb_size=0x%x=====\n\n",off,len,vol->usable_leb_size);
		}

		size -= len;
		offp += len;

		memcpy(buf, tbuf, len);

		buf += len;
		len = size > tbuf_size ? tbuf_size : size;
	} while (size);
    #endif

	free(tbuf);
    //return err ? err : count_save - size;
    return err;
}

static int ubi_volume_read(char *volume, char *buf, size_t size)
{
	int err, lnum, off, len, tbuf_size, i = 0;
	size_t count_save = size;
	void *tbuf;
	unsigned long long tmp;
	struct ubi_volume *vol = NULL;
	loff_t offp = 0;

	for (i = 0; i < ubi->vtbl_slots; i++) {
		vol = ubi->volumes[i];
		if (vol && !strcmp(vol->name, volume)) {
			printf("Volume %s found at volume id %d\n",
				volume, vol->vol_id);
			break;
		}
	}
	if ((i == ubi->vtbl_slots)||(vol==NULL)) {
		printf("%s volume not found\n", volume);
		return -ENODEV;
	}

	printf("read %d bytes from volume %d to %x(buf address)\n",
	       (int) size, vol->vol_id, (unsigned)buf);

	if (vol->updating) {
		printf("updating");
		return -EBUSY;
	}
	if (vol->upd_marker) {
		printf("damaged volume, update marker is set");
		return -EBADF;
	}
	if (offp == vol->used_bytes)
		return 0;

	if (size == 0) {
		printf("Read [%lu] bytes\n", (unsigned long) vol->used_bytes);
		size = vol->used_bytes;
	}

	if (vol->corrupted)
		printf("read from corrupted volume %d", vol->vol_id);
	if (offp + size > vol->used_bytes)
		count_save = size = vol->used_bytes - offp;

	tbuf_size = vol->usable_leb_size;
	if (size < tbuf_size)
		tbuf_size = ALIGN(size, ubi->min_io_size);
	tbuf = malloc(tbuf_size);
	if (!tbuf) {
		printf("NO MEM\n");
		return -ENOMEM;
	}
	len = size > tbuf_size ? tbuf_size : size;

	tmp = offp;
	off = do_div(tmp, vol->usable_leb_size);
	lnum = tmp;
	do {
		if (off + len >= vol->usable_leb_size)
			len = vol->usable_leb_size - off;

		err = ubi_eba_read_leb(ubi, vol, lnum, tbuf, off, len, 0);
		if (err) {
			printf("read err %x\n", err);
			break;
		}
		off += len;
		if (off == vol->usable_leb_size) {
			lnum += 1;
			off -= vol->usable_leb_size;
		}

		size -= len;
		offp += len;

		memcpy(buf, tbuf, len);

		buf += len;
		len = size > tbuf_size ? tbuf_size : size;
	} while (size);

	free(tbuf);
	return err ? err : count_save - size;
}

static int ubi_dev_scan(struct mtd_info *info, char *ubidev)
{
    struct mtd_device *dev=NULL;
    struct part_info *part=NULL;
    struct mtd_partition mtd_part;
    u8 pnum=0;
    int err=0;

	if (find_dev_and_part(ubidev, &dev, &pnum, &part) != 0)
		return 1;

	sprintf(buffer, "mtd=%d", pnum);
	memset(&mtd_part, 0, sizeof(mtd_part));
	mtd_part.name = buffer;
	mtd_part.size = part->size;
	mtd_part.offset = part->offset;
	add_mtd_partitions(info, &mtd_part, 1);

	err = ubi_mtd_param_parse(buffer, NULL);
	if (err) {
		del_mtd_partitions(info);
		return err;
	}

	err = ubi_init();
	if (err) {
        ubi_exit();
		del_mtd_partitions(info);
		return err;
	}

	ubi_initialized = 1;

	return 0;
}

#if (CONFIG_COMMANDS & CFG_CMD_ENV_IN_UBI)
static int ubi_dev_scan_simple(struct mtd_info *info, char *ubidev)
{
       struct mtd_partition mtd_part;
	int err;

	sprintf(buffer, "mtd=%d", 3);
	memset(&mtd_part, 0, sizeof(mtd_part));
	mtd_part.name = buffer;
	mtd_part.size = CFG_UBI_OFFSET;//part->size;//
	mtd_part.offset = CFG_UBI_SIZE;//part->offset;//
	add_mtd_partitions(info, &mtd_part, 1);

	err = ubi_mtd_param_parse(buffer, NULL);
	if (err) {
		del_mtd_partitions(info);
		return err;
	}

	err = ubi_init();
	if (err) {
		ubi_exit();
		del_mtd_partitions(info);
		return err;
	}

	ubi_initialized = 1;

	return 0;
}
#endif

extern nand_info_t nand_info[];

static int do_ubi(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	size_t size = 0;
	ulong addr = 0;
	size_t total_size =0;
	int err = 0;
        size_t partition_offset;
	if (argc < 2) {
		//cmd_usage(cmdtp);
		return -EPERM;
	}

    #if defined (CONFIG_BACKUP_PARTITION)
    {
        char *pEnv=NULL;
        pEnv=getenv("updateUBIRPermit");
        if(pEnv!=NULL){
            if(strcmp(pEnv,"NO")==0){
                if(strncmp(argv[1],"write",5)==0){
                    printf("updateUBIRPermit=NO");
                    return 0;
                }
            }
        }
    }
    #endif
#if  !(CONFIG_COMMANDS & CFG_CMD_ENV_IN_UBI)
	if (mtdparts_init() != 0) {
		printf("Error initializing mtdparts!\n");
		return -EPERM;
	}
#endif

	if (strcmp(argv[1], "part") == 0) {
		char mtd_dev[16];
		struct mtd_device *dev;
		struct part_info *part;
		u8 pnum;

		/* Print current partition */
		if (argc == 2) {
			if (!ubi_dev.selected) {
				printf("Error, no UBI device/partition selected!\n");
				return -EPERM;
			}

			printf("Device %d: %s, partition %s\n",
			       ubi_dev.nr, ubi_dev.mtd_info->name, ubi_dev.part_name);
			return 0;
		}
        /*
		if (argc < 3) {
			//cmd_usage(cmdtp);
			return -EPERM;
		}*/
#ifndef ENABLE_MULTI_UBI_DEVICE //support multi ubi device
		if(ubi_initialized)
		{
		    printf("UBI already initialized!!\n");
		    //return 0;
		}
#endif
		/* todo: get dev number for NAND... */
		ubi_dev.nr = 0;

		/*
		 * Call ubi_exit() before re-initializing the UBI subsystem
		 */
		if (ubi_initialized) {
			ubi_exit();
			del_mtd_partitions(ubi_dev.mtd_info);
		}

		/*
		 * Search the mtd device number where this partition
		 * is located
		 */
		if (find_dev_and_part(argv[2], &dev, &pnum, &part)) {
			printf("Partition %s not found!\n", argv[2]);
			return -EPERM;
		}
		//sprintf(mtd_dev, "%s%d", MTD_DEV_TYPE(dev->id->type), dev->id->num);
		//ubi_dev.mtd_info = get_mtd_device(NULL,dev->id->num);

		ubi_dev.mtd_info=&nand_info[dev->id->num];

		if (IS_ERR(ubi_dev.mtd_info)) {
			printf("Partition %s not found on device %s!\n", argv[2], mtd_dev);
			return -EPERM;
		}

//		/**/
#if 0 //seems we do not need to worried MTD device
		{
		    struct list_head *pentry;
		    struct part_info *part;
		    struct mtd_partition *mpart;

		    list_for_each(pentry, &dev->parts) {

                part = list_entry(pentry, struct part_info, link);
                mpart=kmalloc(sizeof(*mpart),GFP_KERNEL);
                if(mpart==NULL)
                {
                    printf("[%s]: No Memory!!\n",__FUNCTION__);

                    return -1;
                }

		        mpart->name=part->name;
		        mpart->size = part->size;
		        mpart->offset = part->offset;

		        add_mtd_partitions(ubi_dev.mtd_info,mpart,1);

		    }
		}
#endif
		ubi_dev.selected = 1;

		//strcpy(ubi_dev.part_name, argv[2]);
		if(strlen(argv[2])>=sizeof(ubi_dev.part_name)){
            strncpy(ubi_dev.part_name, argv[2],sizeof(ubi_dev.part_name)-1);
            ubi_dev.part_name[sizeof(ubi_dev.part_name)-1]='\0';
        }
        else
		strcpy(ubi_dev.part_name, argv[2]);
		err = ubi_dev_scan(ubi_dev.mtd_info, ubi_dev.part_name);
		if (err) {
			printf("UBI init error %d\n", err);
			ubi_dev.selected = 0;
			return err;
		}

		ubi = ubi_devices[0];

		return 0;
	}

	if ((strcmp(argv[1], "part") != 0) && (!ubi_dev.selected)) {
		printf("Error, no UBI device/partition selected!\n");
		return -EPERM;
	}

	if (strcmp(argv[1], "info") == 0) {
		int layout = 0;
		if (argc > 2 && !strncmp(argv[2], "l", 1))
			layout = 1;
		return ubi_info(layout);
	}

	if (strncmp(argv[1], "create", 6) == 0) {
		int dynamic = 1;	/* default: dynamic volume */

            if(0 != g_volume_black_cnt)
            {
                printf("%s: bypass ubi create '%s', at %d\n", __func__, argv[2], __LINE__);
                return 0;
            }
		/* Use maximum available size */
		size = 0;

		/* E.g., create volume size type */
		if (argc == 5) {
			if (strncmp(argv[4], "s", 1) == 0)
				dynamic = 0;
			else if (strncmp(argv[4], "d", 1) != 0) {
				printf("Incorrect type\n");
				return -EPERM;
			}
			argc--;
		}
		/* E.g., create volume size */
		if (argc == 4) {
			size = simple_strtoul(argv[3], NULL, 16);
			argc--;
		}
		/* Use maximum available size */
		if (!size)
			size = ubi->avail_pebs * ubi->leb_size;
		/* E.g., create volume */
		if (argc == 3)
			return ubi_create_vol(argv[2], size, dynamic);
	}

	if (strncmp(argv[1], "remove", 6) == 0) {
		/* E.g., remove volume */
		if (argc == 3)
			return ubi_remove_vol(argv[2]);
	}

        if (strncmp(argv[1], "dont_overwrite_init", 19) == 0) {
            /* E.g., init for don't overwrite volume */
            if (argc == 2)
            {
                return ubi_dontoverwrite_init();
            }
        }

        if (strncmp(argv[1], "dont_overwrite", 14) == 0) {
            /* E.g., don't overwrite volume */
            if (argc == 3)
            {
                return ubi_dontoverwrite_vol(argv[2]);
            }
        }

	if (strncmp(argv[1], "write_cont", 10) == 0) {
		if (argc < 5) {
			printf("Please see usage\n");
			return -EPERM;
		}

        addr = (0 == strncmp(argv[2], UPGRADE_ADDR_STR, strlen(UPGRADE_ADDR_STR))) ? UPGRADE_BUFFER_ADDR : simple_strtoul(argv[2], NULL, 16);
		size = simple_strtoul(argv[4], NULL, 16);
        err=ubi_volume_write_cont(argv[3], (void *)addr, size);
        #if defined (CONFIG_BACKUP_PARTITION)
        if(err!=0){
             printf("setenv updateUBIRPermit NO\n");
             run_command("setenv updateUBIRPermit NO",0);
             //run_command("saveenv",0);
        }

        #endif
        return err;
	}

	if (strncmp(argv[1], "write", 5) == 0) {
		if (argc < 5) {
			printf("Please see usage\n");
			return -EPERM;
		}

        addr = (0 == strncmp(argv[2], UPGRADE_ADDR_STR, strlen(UPGRADE_ADDR_STR))) ? UPGRADE_BUFFER_ADDR : simple_strtoul(argv[2], NULL, 16);
		size = simple_strtoul(argv[4], NULL, 16);

            if(argc == 6)
            {
                total_size = simple_strtoul(argv[5], NULL, 16);
                printf("total size = %d\n", total_size);
            }
            else
            {
                total_size = size;
            }
            if(0 == ubi_dontoverwrite_vol_check((const char*)argv[3]))
            {
                printf("%s: bypass ubi write '%s', at %d\n", __func__, argv[3], __LINE__);
                return 0;
            }
            err=ubi_volume_write(argv[3], (void *)addr, size, total_size);
#if defined (CONFIG_BACKUP_PARTITION)
            if(err!=0){
                printf("setenv updateUBIRPermit NO\n");
                run_command("setenv updateUBIRPermit NO",0);
                //run_command("saveenv",0);
            }

#endif
            return err;
        }


        if(argc == 6)
        {
            if (strncmp(argv[1], "partial_read", 12) == 0)
            {
                addr = simple_strtoul(argv[2], NULL, 16);
                size= simple_strtoul(argv[4], NULL, 16);
                partition_offset = simple_strtoul(argv[5], NULL, 16);

                return ubi_volume_partial_read(argv[3], (char *)addr, size ,partition_offset);
            }
        }
	if (strncmp(argv[1], "read", 4) == 0) {
		size = 0;

		/* E.g., read volume size */
		if (argc == 5) {
			size = simple_strtoul(argv[4], NULL, 16);
			argc--;
		}

		/* E.g., read volume */
		if (argc == 4) {
			addr = (0 == strncmp(argv[2], UPGRADE_ADDR_STR, strlen(UPGRADE_ADDR_STR))) ? UPGRADE_BUFFER_ADDR : simple_strtoul(argv[2], NULL, 16);
			argc--;
		}

		if (argc == 3)
			return ubi_volume_read(argv[3], (char *)addr, size);
	}

    	if (strncmp(argv[1], "reinit", 6) == 0) {
		/* E.g., reinit ubi partition */
		if (argc == 2)
		{
			/*
			 * Call ubi_exit() before re-initializing the UBI subsystem
			 */
			if (ubi_initialized) {
				ubi_exit();
				del_mtd_partitions(ubi_dev.mtd_info);
				ubi_initialized = 0;
				return 0;
			}
		}
	}

        if (strncmp(argv[1], "dump", 4) == 0) {
              ulong lebnum, pebnum, dumpoff=0;
              int     volnum, i, dpret, j;
              struct ubi_volume *vol = NULL, *voltmp = NULL, *volterm = NULL;
              u_char *buf = NULL, *p = NULL;
              if(argc < 4)
                    goto usage;

              volnum = (int)simple_strtoul(argv[2], NULL, 16);
              lebnum = (int)simple_strtoul(argv[3], NULL, 16);
              if(argc > 4)
                    dumpoff = (int)simple_strtoul(argv[4], NULL, 16);
              if((volnum > 0) && (volnum < ubi->vtbl_slots))
                      vol = ubi->volumes[volnum];

              if(vol){
                    printf("Volume %s found at volume id %d\n",
                            vol->name, volnum);
                    goto dump;
              }

              for (i = 0; i < ubi->vtbl_slots; i++) {
                    voltmp = ubi->volumes[i];
                    if (voltmp && (!strcmp(voltmp->name, argv[2]))) {
                            printf("Volume %s found at volume id %d\n",
                                argv[2], voltmp->vol_id);
                            goto dump;
                    }
              }
              printf("volume not found!\n");
              goto usage;

dump:
                if(argc <= 4)
                    dumpoff = 0;

                if(vol)
                    volterm = vol;
                else if(voltmp && (!strcmp(voltmp->name, argv[2])))
                    volterm = voltmp;
                else{
                    printf("volume not found!\n");
                    goto usage;
                    }

                buf = vmalloc(ubi->min_io_size);
                if (!buf) {
                    printf("NO MEM\n");
                    return -ENOMEM;
                }
                pebnum = volterm->eba_tbl[lebnum];
                if(!pebnum)
                {
                     printf("Can not found the corresponding Peb!\n");
                     goto usage;
                }
                printf("lebnum:%ld, pebnum:%ld\n ", lebnum, pebnum);
                dumpoff = ALIGN(dumpoff, ubi->min_io_size);
                dpret = ubi_eba_read_leb(ubi, volterm, lebnum, buf, dumpoff, ubi->min_io_size, 0);
                printf("Page offset in block: 0x%lx\n", dumpoff);
                j = ubi->min_io_size >> 4; p = buf;
                while (j--) {
                    printf( "\t%02x %02x %02x %02x %02x %02x %02x %02x"
                            "  %02x %02x %02x %02x %02x %02x %02x %02x\n",
                            p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7],
                            p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
                    p += 16;
                }
                vfree(buf);
            return dpret == 0 ? 1 : 0;
            }

        if (strncmp(argv[1], "read.e", 6) == 0) {
              ulong lebnum, pebnum, memaddr;
              int     volnum, i, dpret=0, dumplebnum=0;
              struct ubi_volume *vol = NULL, *voltmp = NULL, *volterm = NULL;

              if(argc < 5)
                    goto usage;

              memaddr = (int)simple_strtoul(argv[2], NULL, 16);
              volnum = (int)simple_strtoul(argv[3], NULL, 16);
              lebnum = (int)simple_strtoul(argv[4], NULL, 16);

              if(argc > 5)
                      dumplebnum = (int)simple_strtoul(argv[5], NULL, 16);

              if((volnum > 0) && (volnum < ubi->vtbl_slots))
                      vol = ubi->volumes[volnum];

              if(vol){
                    printf("Volume %s found at volume id %d\n",
                            vol->name, volnum);
                    goto readdump;
              }

              for (i = 0; i < ubi->vtbl_slots; i++) {
                    voltmp = ubi->volumes[i];
                    if (voltmp && (!strcmp(voltmp->name, argv[3]))) {
                            printf("Volume %s found at volume id %d\n",
                                argv[2], voltmp->vol_id);
                            goto readdump;
                    }
              }
              printf("volume not found!\n");
              goto usage;

readdump:

                for(i = 0;;i ++){
                        if(vol)
                            volterm = vol;
                        else if(voltmp && (!strcmp(voltmp->name, argv[2])))
                            volterm = voltmp;
                        else{
                            printf("volume not found!\n");
                            goto usage;
                            }
                        pebnum = volterm->eba_tbl[lebnum + i];
                        if(!pebnum)
                        {
                            printf("Can not found the corresponding Peb!\n");
                            continue;
                            }
                        if(!dumplebnum)
                            break;

                        dpret = ubi_eba_read_leb(ubi, volterm, lebnum+i,
                            (u_char*) memaddr, 0, volterm->usable_leb_size, 0);
                        printf("lebnum:%ld, pebnum:%ld\n ", lebnum+i, pebnum);
                        flush_cache(memaddr, volterm->usable_leb_size);

                        if(dumplebnum > 0){
                            --dumplebnum;
                            memaddr += volterm->usable_leb_size;
                            }
                        else
                            break;
                    }

            return dpret == 0 ? 1 : 0;
            }
usage:
	printf("Please see usage\n");
	return -1;
}

#if (CONFIG_COMMANDS & CFG_CMD_ENV_IN_UBI)
int ubi_init_simple(void)
{
	int err = 0;

#ifndef ENABLE_MULTI_UBI_DEVICE //support multi ubi device
#ifndef CONFIG_SUPERNOVA_OAD
	if(ubi_initialized)
	{
	    printf("UBI already initialized!!\n");
	    return 0;
	}
#endif
#endif
	/* todo: get dev number for NAND... */
	ubi_dev.nr = 0;

	/*
	 * Call ubi_exit() before re-initializing the UBI subsystem
	 */
	if (ubi_initialized) {
		ubi_exit();
	}

	ubi_dev.mtd_info=&nand_info[0];

	if (IS_ERR(ubi_dev.mtd_info)) {
		printf("Partition UBI not found!\n");
		return 1;
	}

	ubi_dev.selected = 1;

	strcpy(ubi_dev.part_name, "UBI");
	err = ubi_dev_scan_simple(ubi_dev.mtd_info, ubi_dev.part_name);
	if (err) {
		printf("UBI init error %d\n", err);
		ubi_dev.selected = 0;
		return err;
	}

	ubi = ubi_devices[0];

	return 0;
}
#endif

U_BOOT_CMD(ubi, 6, 1, do_ubi,
	"ubi commands",
	"part [part]"
		" - Show or set current partition\n"
	"ubi info [l[ayout]]"
		" - Display volume and ubi layout information\n"
	"ubi create[vol] volume [size] [type]"
		" - create volume name with size\n"
	"ubi write[vol] address volume size"
		" - Write volume from address with size\n"
	"ubi read[vol] address volume [size]"
		" - Read volume to address with size\n"
	"ubi remove[vol] volume"
		" - Remove volume\n"
	"[Legends]\n"
	" volume: charater name\n"
	" size: KiB, MiB, GiB, and bytes\n"
	" type: s[tatic] or d[ynamic] (default=dynamic)\n"
);

#endif
