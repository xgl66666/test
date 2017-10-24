/*
 * This file is part of UBIFS.
 *
 * Copyright (C) 2006-2008 Nokia Corporation.
 *
 * (C) Copyright 2008-2009
 * Stefan Roese, DENX Software Engineering, sr@denx.de.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Authors: Artem Bityutskiy (?и???кий ????м)
 *          Adrian Hunter
 */

#include "ubifs.h"
#include <u-boot/zlib.h>
#include <configs/uboot_module_config.h>

#if !defined(CONFIG_SYS_64BIT_VSPRINTF)
#warning Please define CONFIG_SYS_64BIT_VSPRINTF for correct output!
#endif

DECLARE_GLOBAL_DATA_PTR;
extern int strcmp(const char * cs,const char * ct);
extern char * strncpy(char * dest,const char *src,size_t count);

/* compress.c */

#ifdef CONFIG_LZO
extern int lzo_decompress_ubi(const unsigned char *in, size_t in_len,
			   unsigned char *out, size_t *out_len);
#endif
/*
 * We need a wrapper for zunzip() because the parameters are
 * incompatible with the lzo decompressor.
 */
static int gzip_decompress(const unsigned char *in, size_t in_len,
			   unsigned char *out, size_t *out_len)
{
#if defined(CONFIG_GZIP)
	unsigned long len = in_len;
	return zunzip(out, *out_len, (unsigned char *)in, &len, 0, 0);
#else
        printf("not support gzip in UBIFS\n");
        return -1;
#endif
}

/* Fake description object for the "none" compressor */
static struct ubifs_compressor none_compr = {
	.compr_type = UBIFS_COMPR_NONE,
	.name = "no compression",
	.capi_name = "",
	.decompress = NULL,
};

#if (CONFIG_COMMANDS & CFG_CMD_JFFS2)
#ifdef CONFIG_LZO
static struct ubifs_compressor lzo_compr = {
	.compr_type = UBIFS_COMPR_LZO,
	.name = "LZO",
	.capi_name = "lzo",
	.decompress = lzo_decompress_ubi,
};
#endif
#endif

static struct ubifs_compressor zlib_compr = {
	.compr_type = UBIFS_COMPR_ZLIB,
	.name = "zlib",
	.capi_name = "deflate",
	.decompress = gzip_decompress,
};

/* All UBIFS compressors */
struct ubifs_compressor *ubifs_compressors[UBIFS_COMPR_TYPES_CNT];

/**
 * ubifs_decompress - decompress data.
 * @in_buf: data to decompress
 * @in_len: length of the data to decompress
 * @out_buf: output buffer where decompressed data should
 * @out_len: output length is returned here
 * @compr_type: type of compression
 *
 * This function decompresses data from buffer @in_buf into buffer @out_buf.
 * The length of the uncompressed data is returned in @out_len. This functions
 * returns %0 on success or a negative error code on failure.
 */
int ubifs_decompress(const void *in_buf, int in_len, void *out_buf,
		     int *out_len, int compr_type)
{
	int err=0;
	struct ubifs_compressor *compr=NULL;

	if (unlikely(compr_type < 0 || compr_type >= UBIFS_COMPR_TYPES_CNT)) {
		ubifs_err("invalid compression type %d", compr_type);
		return -EINVAL;
	}

	compr = ubifs_compressors[compr_type];

	if (unlikely(!compr->capi_name)) {
		ubifs_err("%s compression is not compiled in", compr->name);
		return -EINVAL;
	}

	if (compr_type == UBIFS_COMPR_NONE) {
		memcpy(out_buf, in_buf, in_len);
		*out_len = in_len;
		return 0;
	}

	err = compr->decompress(in_buf, in_len, out_buf, (size_t *)out_len);
	if (err)
		ubifs_err("cannot decompress %d bytes, compressor %s, "
			  "error %d", in_len, compr->name, err);

	return err;
}

/**
 * compr_init - initialize a compressor.
 * @compr: compressor description object
 *
 * This function initializes the requested compressor and returns zero in case
 * of success or a negative error code in case of failure.
 */
static int __init compr_init(struct ubifs_compressor *compr)
{
	ubifs_compressors[compr->compr_type] = compr;
	ubifs_compressors[compr->compr_type]->name += gd->reloc_off;
	ubifs_compressors[compr->compr_type]->capi_name += gd->reloc_off;
	ubifs_compressors[compr->compr_type]->decompress += gd->reloc_off;
	return 0;
}

/**
 * ubifs_compressors_init - initialize UBIFS compressors.
 *
 * This function initializes the compressor which were compiled in. Returns
 * zero in case of success and a negative error code in case of failure.
 */
int __init ubifs_compressors_init(void)
{
	int err=0;

#if (CONFIG_COMMANDS & CFG_CMD_JFFS2)
#ifdef CONFIG_LZO
	err = compr_init(&lzo_compr);
	if (err)
		return err;
#endif
#endif

	err = compr_init(&zlib_compr);
	if (err)
		return err;

	err = compr_init(&none_compr);
	if (err)
		return err;

	return 0;
}

/*
 * ubifsls...
 */


static int filldir(struct ubifs_info *c, const char *name, int namlen,
		   u64 ino, unsigned int d_type)
{
	struct inode *inode=NULL;
	char filetime[32];

	switch (d_type) {
	case UBIFS_ITYPE_REG:
		printf("\t");
		break;
	case UBIFS_ITYPE_DIR:
		printf("<DIR>\t");
		break;
	case UBIFS_ITYPE_LNK:
		printf("<LNK>\t");
		break;
	default:
		printf("other\t");
		break;
	}

	inode = ubifs_iget(c->vfs_sb, ino);
	if (IS_ERR(inode)) {
		printf("%s: Error in ubifs_iget(), ino=%lld ret=%p!\n",
		       __func__, ino, inode);
		return -1;
	}
	ctime_r((time_t *)&inode->i_mtime, filetime);
	printf("%ld  %24.24s  ", (unsigned long)inode->i_size, filetime);
	ubifs_iput(inode);

	printf("%s\n", name);

	return 0;
}

static int ubifs_printdir(struct file *file, void *dirent)
{
	int err = 0, over = 0;
	struct qstr nm;
	union ubifs_key key;
	struct ubifs_dent_node *dent=NULL;
	struct inode *dir = file->f_path.dentry->d_inode;
	struct ubifs_info *c = dir->i_sb->s_fs_info;

	dbg_gen("dir ino %lu, f_pos %#llx", dir->i_ino, file->f_pos);

	if (file->f_pos > UBIFS_S_KEY_HASH_MASK || file->f_pos == 2)
		/*
		 * The directory was seek'ed to a senseless position or there
		 * are no more entries.
		 */
		return 0;

	if (file->f_pos == 1) {
		/* Find the first entry in TNC and save it */
		lowest_dent_key(c, &key, dir->i_ino);
		nm.name = NULL;
		dent = ubifs_tnc_next_ent(c, &key, &nm);
		if (IS_ERR(dent)) {
			err = PTR_ERR(dent);
			goto out;
		}

		file->f_pos = key_hash_flash(c, &dent->key);
		file->private_data = dent;
	}

	dent = file->private_data;
	if (!dent) {
		/*
		 * The directory was seek'ed to and is now readdir'ed.
		 * Find the entry corresponding to @file->f_pos or the
		 * closest one.
		 */
		dent_key_init_hash(c, &key, dir->i_ino, file->f_pos);
		nm.name = NULL;
		dent = ubifs_tnc_next_ent(c, &key, &nm);
		if (IS_ERR(dent)) {
			err = PTR_ERR(dent);
			goto out;
		}
		file->f_pos = key_hash_flash(c, &dent->key);
		file->private_data = dent;
	}

	while (1) {
		dbg_gen("feed '%s', ino %llu, new f_pos %#x",
			dent->name, (unsigned long long)le64_to_cpu(dent->inum),
			key_hash_flash(c, &dent->key));
//		ubifs_assert(le64_to_cpu(dent->ch.sqnum) > ubifs_inode(dir)->creat_sqnum);

		nm.len = le16_to_cpu(dent->nlen);
		over = filldir(c, (char *)dent->name, nm.len,
			       le64_to_cpu(dent->inum), dent->type);
		if (over)
			return 0;

		/* Switch to the next entry */
		key_read(c, &dent->key, &key);
		nm.name = (char *)dent->name;
		dent = ubifs_tnc_next_ent(c, &key, &nm);
		if (IS_ERR(dent)) {
			err = PTR_ERR(dent);
			goto out;
		}

		kfree(file->private_data);
		file->f_pos = key_hash_flash(c, &dent->key);
		file->private_data = dent;
		cond_resched();
	}

out:
	if (err != -ENOENT) {
		ubifs_err("cannot find next direntry, error %d", err);
		return err;
	}

	kfree(file->private_data);
	file->private_data = NULL;
	file->f_pos = 2;
	return 0;
}

static int ubifs_finddir(struct super_block *sb, char *dirname,
			 unsigned long root_inum, unsigned long *inum)
{
	int err=0, ret=0;
	struct qstr nm;
	union ubifs_key key;
	struct ubifs_dent_node *dent=NULL;
	struct ubifs_info *c=NULL;
	struct file *file=NULL;
	struct dentry *dentry=NULL;
	struct inode *dir=NULL;

	file = (struct file *)kzalloc(sizeof(struct file), 0);
    if(!file) return -ENOMEM;
    
	dentry = (struct dentry *)kzalloc(sizeof(struct dentry), 0);
    if(!dentry){
        kfree(file);
        return -ENOMEM;
    }
	dir = (struct inode *)kzalloc(sizeof(struct inode), 0);

    if(!dir){
       kfree(file);
       kfree(dentry);
       return -ENOMEM;
    }
	/*if (!file || !dentry || !dir) {
		printf("%s: Error, no memory for malloc!\n", __func__);
		err = -ENOMEM;
		goto out;
	}*/
	
	memset(file, 0, sizeof(struct file));
	memset(dentry, 0, sizeof(struct dentry));
	memset(dir, 0, sizeof(struct inode));

	dir->i_sb = sb;
	file->private_data = NULL;
	file->f_path.dentry = dentry;
	file->f_path.dentry->d_parent = dentry;
	file->f_path.dentry->d_inode = dir;
	file->f_path.dentry->d_inode->i_ino = root_inum;
	c = sb->s_fs_info;

	dbg_gen("dir ino %lu, f_pos %#llx", dir->i_ino, file->f_pos);

	/* Find the first entry in TNC and save it */
	lowest_dent_key(c, &key, dir->i_ino);
	nm.name = NULL;
	dent = ubifs_tnc_next_ent(c, &key, &nm);
	if (IS_ERR(dent)) {
		err = PTR_ERR(dent);
		goto out;
	}

	file->f_pos = key_hash_flash(c, &dent->key);
	file->private_data = dent;

	while (1) {
		dbg_gen("feed '%s', ino %llu, new f_pos %#x",
			dent->name, (unsigned long long)le64_to_cpu(dent->inum),
			key_hash_flash(c, &dent->key));
//		ubifs_assert(le64_to_cpu(dent->ch.sqnum) > ubifs_inode(dir)->creat_sqnum);

		nm.len = le16_to_cpu(dent->nlen);
		if ((strncmp(dirname, (char *)dent->name, nm.len) == 0) &&
		    (strlen(dirname) == nm.len)) {
			*inum = le64_to_cpu(dent->inum);
			ret = 1;
			goto out;
		}

		/* Switch to the next entry */
		key_read(c, &dent->key, &key);
		nm.name = (char *)dent->name;
		dent = ubifs_tnc_next_ent(c, &key, &nm);
		if (IS_ERR(dent)) {
			err = PTR_ERR(dent);
			goto out;
		}

		kfree(file->private_data);
		file->f_pos = key_hash_flash(c, &dent->key);
		file->private_data = dent;
		cond_resched();
	}

out:
	if (file)
	{
		if (file->private_data)
			kfree(file->private_data);
		file->private_data = NULL;
		file->f_pos = 2;
		free(file);
	}
	if (dentry)
		free(dentry);
	if (dir)
		free(dir);
	if ((ret == 0) && (err != -ENOENT)) {
		ubifs_err("cannot find next direntry, error %d", err);
		return err;
	}
	return ret;
}

static unsigned long ubifs_findfile(struct super_block *sb, char *filename)
{
	int ret=0;
	char *next=NULL;
	char fpath[128];
	char *name = fpath;
	unsigned long root_inum = 1;
	unsigned long inum=0;

	if(filename == NULL)
	{
		return 0;
	}
	strncpy(fpath, filename, sizeof(fpath)-1);

	/* Remove all leading slashes */
	while (*name == '/')
		name++;

	/*
	 * Handle root-direcoty ('/')
	 */
	inum = root_inum;
	//if (!name || *name == '\0')
	if (*name == '\0')
		return inum;

	for (;;) {
		/* Extract the actual part from the pathname.  */
		next = strchr(name, '/');
		if (next) {
			/* Remove all leading slashes.  */
			while (*next == '/')
				*(next++) = '\0';
		}

		ret = ubifs_finddir(sb, name, root_inum, &inum);

		/*
		 * Check if directory with this name exists
		 */

		/* Found the node!  */
		if (!next || *next == '\0') {
			if (ret)
				return inum;

			break;
		}

		root_inum = inum;
		name = next;
	}

	return 0;
}

int ubifs_ls(char *filename)
{
	struct ubifs_info *c = ubifs_sb->s_fs_info;
	struct file *file=NULL;
	struct dentry *dentry=NULL;
	struct inode *dir=NULL;
	void *dirent = NULL;
	unsigned long inum=0;
	int ret = 0;

	c->ubi = ubi_open_volume(c->vi.ubi_num, c->vi.vol_id, UBI_READONLY);
	if (IS_ERR(c->ubi)) {
		ret  = PTR_ERR(c->ubi);
		goto out_no_close;
	}
	inum = ubifs_findfile(ubifs_sb, filename);
	if (!inum) {
		ret = -1;
		goto out;
	}

	file = (struct file *)kzalloc(sizeof(struct file), 0);
	dentry = (struct dentry *)kzalloc(sizeof(struct dentry), 0);
	dir = (struct inode *)kzalloc(sizeof(struct inode), 0);
	if (!file || !dentry || !dir) {
		printf("%s: Error, no memory for malloc!\n", __func__);
		ret = -ENOMEM;
		goto out_mem;
	}
	memset(file, 0, sizeof(struct file));
	memset(dentry, 0, sizeof(struct dentry));
	memset(dir, 0, sizeof(struct inode));

	dir->i_sb = ubifs_sb;
	file->f_path.dentry = dentry;
	file->f_path.dentry->d_parent = dentry;
	file->f_path.dentry->d_inode = dir;
	file->f_path.dentry->d_inode->i_ino = inum;
	file->f_pos = 1;
	file->private_data = NULL;
	ubifs_printdir(file, dirent);

out_mem:
	if (file)
		free(file);
	if (dentry)
		free(dentry);
	if (dir)
		free(dir);

out:
	ubi_close_volume(c->ubi);
out_no_close:
	return ret;
}

/*
 * ubifsload...
 */

/* file.c */

static inline void *kmap(struct page *page)
{
	return page->addr;
}

static int read_block(struct inode *inode, void *addr, unsigned int block,
		      struct ubifs_data_node *dn)
{
	struct ubifs_info *c = inode->i_sb->s_fs_info;
	int err=0, len=0, out_len=0;
	union ubifs_key key;
	unsigned int dlen=0;

	data_key_init(c, &key, inode->i_ino, block);
	err = ubifs_tnc_lookup(c, &key, dn);
	if (err) {
		if (err == -ENOENT)
			/* Not found, so it must be a hole */
			memset(addr, 0, UBIFS_BLOCK_SIZE);
		return err;
	}

//	ubifs_assert(le64_to_cpu(dn->ch.sqnum) > ubifs_inode(inode)->creat_sqnum);

	len = le32_to_cpu(dn->size);
	if (len <= 0 || len > UBIFS_BLOCK_SIZE)
		goto dump;

	dlen = le32_to_cpu(dn->ch.len) - UBIFS_DATA_NODE_SZ;
	out_len = UBIFS_BLOCK_SIZE;
	err = ubifs_decompress(&dn->data, dlen, addr, &out_len,
			       le16_to_cpu(dn->compr_type));
	if (err || len != out_len)
		goto dump;

	/*
	 * Data length can be less than a full block, even for blocks that are
	 * not the last in the file (e.g., as a result of making a hole and
	 * appending data). Ensure that the remainder is zeroed out.
	 */
	if (len < UBIFS_BLOCK_SIZE)
		memset(addr + len, 0, UBIFS_BLOCK_SIZE - len);

	return 0;

dump:
	ubifs_err("bad data node (block %u, inode %lu)",
		  block, inode->i_ino);
	dbg_dump_node(c, dn);
	return -EINVAL;
}

static int do_readpage(struct ubifs_info *c, struct inode *inode, struct page *page)
{
	void *addr=0;
	int err = 0, i=0;
	unsigned int block=0, beyond=0;
	struct ubifs_data_node *dn=NULL;
	loff_t i_size = inode->i_size;

	dbg_gen("ino %lu, pg %lu, i_size %lld",
		inode->i_ino, page->index, i_size);

	addr = kmap(page);

	block = page->index << UBIFS_BLOCKS_PER_PAGE_SHIFT;
	beyond = (i_size + UBIFS_BLOCK_SIZE - 1) >> UBIFS_BLOCK_SHIFT;
	if (block >= beyond) {
		/* Reading beyond inode */
		memset(addr, 0, PAGE_CACHE_SIZE);
		goto out;
	}

	dn = kmalloc(UBIFS_MAX_DATA_NODE_SZ, GFP_NOFS);
	if (!dn)
		return -ENOMEM;

	i = 0;
	while (1) {
		int ret;

		if (block >= beyond) {
			/* Reading beyond inode */
			err = -ENOENT;
			memset(addr, 0, UBIFS_BLOCK_SIZE);
		} else {
			ret = read_block(inode, addr, block, dn);
			if (ret) {
				err = ret;
				if (err != -ENOENT)
					break;
			} else if (block + 1 == beyond) {
				int dlen = le32_to_cpu(dn->size);
				int ilen = i_size & (UBIFS_BLOCK_SIZE - 1);

				if (ilen && ilen < dlen)
					memset(addr + ilen, 0, dlen - ilen);
			}
		}
		if (++i >= UBIFS_BLOCKS_PER_PAGE)
			break;
		block += 1;
		addr += UBIFS_BLOCK_SIZE;
	}
	if (err) {
		if (err == -ENOENT) {
			/* Not found, so it must be a hole */
			dbg_gen("hole");
			goto out_free;
		}
		ubifs_err("cannot read page %lu of inode %lu, error %d",
			  page->index, inode->i_ino, err);
		goto error;
	}

out_free:
	free(dn);
out:
	return 0;

error:
	free(dn);
	return err;
}

U32 ubisize;
int ubifs_load(char *filename, u32 addr, u32 size)
{
	struct ubifs_info *c = ubifs_sb->s_fs_info;
	unsigned long inum=0;
	struct inode *inode;
	struct page page;
	int err = 0;
	int i=0;
	int count=0;
	char link_name[64];
	struct ubifs_inode *ui=NULL;

	c->ubi = ubi_open_volume(c->vi.ubi_num, c->vi.vol_id, UBI_READONLY);
	if (IS_ERR(c->ubi)) {
		err = PTR_ERR(c->ubi);
		goto out_no_close;
	}
	inum = ubifs_findfile(ubifs_sb, filename);
	if (!inum) {
		err = -1;
		goto out;
	}

	/*
	 * Read file inode
	 */
	inode = ubifs_iget(ubifs_sb, inum);
	if (IS_ERR(inode)) {
		printf("%s: Error reading inode %ld!\n", __func__, inum);
		err = PTR_ERR(inode);
		goto out;
	}

	/*
	 * Check for symbolic link
	 */
	ui = ubifs_inode(inode);
	if (((inode->i_mode & S_IFMT) == S_IFLNK) && ui->data_len) {
		memcpy(link_name, ui->data, ui->data_len);
		link_name[ui->data_len] = '\0';
		printf("%s is linked to %s!\n", filename, link_name);
		ubifs_iput(inode);

		/*
		 * Now we have the "real" filename, call ubifs_load()
		 * again (recursive call) to load this file instead
		 */
		return ubifs_load(link_name, addr, size);
	}

	/*
	 * If no size was specified or if size bigger than filesize
	 * set size to filesize
	 */
	if ((size == 0) || (size > inode->i_size))
              ubisize=size = inode->i_size;

	count = (size + UBIFS_BLOCK_SIZE - 1) >> UBIFS_BLOCK_SHIFT;
	printf("Loading file '%s' to addr 0x%08x with size %d (0x%08x)...\n",
	       filename, addr, size, size);

	page.addr = (void *)addr;
	page.index = 0;
	page.inode = inode;
	for (i = 0; i < count; i++) {
		err = do_readpage(c, inode, &page);
		if (err)
			break;

		page.addr += PAGE_SIZE;
		page.index++;
	}

	if (err)
		printf("Error reading file '%s'\n", filename);
	else
		printf("Done\n");

	ubifs_iput(inode);

out:
	ubi_close_volume(c->ubi);
out_no_close:
	return err;
}

int ubifs_part_load(char *filename, u32 addr, u32 offset, u32 size)
{
    struct ubifs_info *c = ubifs_sb->s_fs_info;
    unsigned long inum=0;
    struct inode *inode=NULL;
    struct page page;
    int err = 0;
    int i=0;
    int count=0;
    char link_name[64];
    struct ubifs_inode *ui=NULL;
    
    if(offset % PAGE_SIZE != 0)
    {
        printf("%s: offset(0x%x) should align to page size(0x%x), at %d\n", __func__, offset, PAGE_SIZE, __LINE__);
        return 1;
    }

    c->ubi = ubi_open_volume(c->vi.ubi_num, c->vi.vol_id, UBI_READONLY);
    if (IS_ERR(c->ubi)) {
        err = PTR_ERR(c->ubi);
        goto out_no_close;
    }
    inum = ubifs_findfile(ubifs_sb, filename);
    if (!inum) {
        err = -1;
        goto out;
    }

    /*
    * Read file inode
    */
    inode = ubifs_iget(ubifs_sb, inum);
    if (IS_ERR(inode)) {
        printf("%s: Error reading inode %ld!\n", __func__, inum);
        err = PTR_ERR(inode);
        goto out;
    }

    /*
    * Check for symbolic link
    */
    ui = ubifs_inode(inode);
    if (((inode->i_mode & S_IFMT) == S_IFLNK) && ui->data_len) {
        memcpy(link_name, ui->data, ui->data_len);
        link_name[ui->data_len] = '\0';
        printf("%s is linked to %s!\n", filename, link_name);
        ubifs_iput(inode);

        /*
         * Now we have the "real" filename, call ubifs_load()
         * again (recursive call) to load this file instead
         */
        return ubifs_part_load(link_name, addr, offset, size);
    }

    /*
    * If no size was specified or if size bigger than filesize
    * set size to filesize
    */
    if ((size == 0) || (size > inode->i_size))
        ubisize=size = inode->i_size;

    count = (size + UBIFS_BLOCK_SIZE - 1) >> UBIFS_BLOCK_SHIFT;
    printf("Loading file '%s' to addr 0x%08x with size %d (0x%08x)...\n",
        filename, addr, size, size);

    page.addr = (void *)addr;
    page.index = (offset / PAGE_SIZE);
    page.inode = inode;
    //offset
    for (i = 0; i < count; i++) {
        err = do_readpage(c, inode, &page);
        if (err)
            break;

        page.addr += PAGE_SIZE;
        page.index++;
    }

    if (err)
        printf("Error reading file '%s'\n", filename);
    else
        printf("Done\n");

    ubifs_iput(inode);

out:
    ubi_close_volume(c->ubi);
out_no_close:
    return err;
}

int ubifs_filesize(char *filename, u32* size)
{
    struct ubifs_info *c = ubifs_sb->s_fs_info;
    unsigned long inum=0;
    struct inode *inode=NULL;
    int err = 0;

    c->ubi = ubi_open_volume(c->vi.ubi_num, c->vi.vol_id, UBI_READONLY);
    if (IS_ERR(c->ubi)) {
        err = PTR_ERR(c->ubi);
    	goto out_no_close;
    }
    inum = ubifs_findfile(ubifs_sb, filename);
    if (!inum) {
        err = -1;
        goto out;
    }

    /*
     * Read file inode
     */
    inode = ubifs_iget(ubifs_sb, inum);
    if (IS_ERR(inode)) {
        printf("%s: Error reading inode %ld!\n", __func__, inum);
        err = PTR_ERR(inode);
        goto out;
    }

    /*
     * If no size was specified or if size bigger than filesize
     * set size to filesize
     */
    //if ((size == 0) || (size > inode->i_size))
     *size = inode->i_size;
     printf("ubifs_filesize  %08x \n", *size);

    //return 0;
out:
    ubi_close_volume(c->ubi);
out_no_close:
    return err;

}

