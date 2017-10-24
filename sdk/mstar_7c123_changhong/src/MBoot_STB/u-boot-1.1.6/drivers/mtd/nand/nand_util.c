/*
 * drivers/nand/nand_util.c
 *
 * Copyright (C) 2006 by Weiss-Electronic GmbH.
 * All rights reserved.
 *
 * @author:	Guido Classen <clagix@gmail.com>
 * @descr:	NAND Flash support
 * @references: borrowed heavily from Linux mtd-utils code:
 *		flash_eraseall.c by Arcom Control System Ltd
 *		nandwrite.c by Steven J. Hill (sjhill@realitydiluted.com)
 *			       and Thomas Gleixner (tglx@linutronix.de)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */

#include <common.h>

#if (CONFIG_COMMANDS & CFG_CMD_FAT)
#include <fat.h>
#endif

#if (CONFIG_COMMANDS & CFG_CMD_NAND) && !defined(CFG_NAND_LEGACY)

#include <command.h>
#include <watchdog.h>
#include <malloc.h>
#include <div64.h>

#include <linux/mtd/mtd.h>
#include <nand.h>
#include <jffs2/jffs2.h>

typedef struct erase_info erase_info_t;
typedef struct mtd_info	  mtd_info_t;

/* support only for native endian JFFS2 */
#define cpu_to_je16(x) (x)
#define cpu_to_je32(x) (x)

#define MAX_PAGE_SIZE	        8192
#define MAX_OOB_SIZE	        432
#define NAND_CMD_LOCK		    0x2a
#define NAND_CMD_LOCK_TIGHT	    0x2c
#define NAND_CMD_UNLOCK1	    0x23
#define NAND_CMD_UNLOCK2	    0x24
#define NAND_CMD_LOCK_STATUS	0x7a
 /* write some data to memory at once, this value should be aligned with main+spare size */
#define FAT_BUFFER_LEN              0x9AB000

// This function is defined in cmd_fat.c
#if (CONFIG_COMMANDS & CFG_CMD_FAT)
extern block_dev_desc_t *get_dev (const char* ifname, int dev);
#endif

/*****************************************************************************/
static int nand_block_bad_scrub(struct mtd_info *mtd, loff_t ofs, int getchip)
{
	return 0;
}

/**
 * nand_erase_opts: - erase NAND flash with support for various options
 *		      (jffs2 formating)
 *
 * @param meminfo	NAND device to erase
 * @param opts		options,  @see struct nand_erase_options
 * @return		0 in case of success
 *
 * This code is ported from flash_eraseall.c from Linux mtd utils by
 * Arcom Control System Ltd.
 */
int nand_erase_opts(nand_info_t *meminfo, const nand_erase_options_t *opts)
{
	struct jffs2_unknown_node cleanmarker;
	int clmpos = 0;
	int clmlen = 8;
	erase_info_t erase;
	ulong erase_length;
	int isNAND;
	int bbtest = 1;
	int result;
	int percent_complete = -1;
	int (*nand_block_bad_old)(struct mtd_info *, loff_t, int) = NULL;
	const char *mtd_device = meminfo->name;
        struct mtd_oob_ops tempOobOps;

	memset(&erase, 0, sizeof(erase));

	erase.mtd = meminfo;
	erase.len  = meminfo->erasesize;
	erase.addr = opts->offset;
	erase_length = opts->length;

	isNAND = meminfo->type == MTD_NANDFLASH ? 1 : 0;

	if (opts->jffs2) {
	cleanmarker.magic = cpu_to_je16 (JFFS2_MAGIC_BITMASK);
	cleanmarker.nodetype = cpu_to_je16 (JFFS2_NODETYPE_CLEANMARKER);
		if (isNAND) {
			struct nand_oobinfo *oobinfo = &meminfo->oobinfo;

			/* check for autoplacement */
			if (oobinfo->useecc == MTD_NANDECC_AUTOPLACE) {
				/* get the position of the free bytes */
				if (!oobinfo->oobfree[0][1]) {
					printf(" Eeep. Autoplacement selected "
					       "and no empty space in oob\n");
					return -1;
				}
				clmpos = oobinfo->oobfree[0][0];
				clmlen = oobinfo->oobfree[0][1];
				if (clmlen > 8)
					clmlen = 8;
			} else {
				/* legacy mode */
				switch (meminfo->oobsize) {
				case 8:
					clmpos = 6;
					clmlen = 2;
					break;
				case 16:
					clmpos = 8;
					clmlen = 8;
					break;
				case 64:
					clmpos = 16;
					clmlen = 8;
					break;
				}
			}

	cleanmarker.totlen = cpu_to_je32(8);
		} else {
			cleanmarker.totlen =
				cpu_to_je32(sizeof(struct jffs2_unknown_node));
		}
		cleanmarker.hdr_crc =  cpu_to_je32(
			crc32_no_comp(0, (unsigned char *) &cleanmarker,
				      sizeof(struct jffs2_unknown_node) - 4));
	}

	/* scrub option allows to erase badblock. To prevent internal
	 * check from erase() method, set block check method to dummy
	 * and disable bad block table while erasing.
	 */
	if (opts->scrub) {
		struct nand_chip *priv_nand = meminfo->priv;

		nand_block_bad_old = priv_nand->block_bad;
		priv_nand->block_bad = nand_block_bad_scrub;
		/* we don't need the bad block table anymore...
		 * after scrub, there are no bad blocks left!
		 */
		if (priv_nand->bbt) {
			kfree(priv_nand->bbt);
		}
		priv_nand->bbt = NULL;
	}

	for (;
	     erase.addr < opts->offset + erase_length;
	     erase.addr += meminfo->erasesize) {

		WATCHDOG_RESET ();

		if (!opts->scrub && bbtest) {
			int ret = meminfo->block_isbad(meminfo, erase.addr);
			if (ret > 0) {
				if (!opts->quiet)
					printf("\rSkipping bad block at  "
					       "0x%08x                   "
					       "                         \n",
					       erase.addr);
				continue;

			} else if (ret < 0) {
				printf("\n%s: MTD get bad block failed: %d\n",
				       mtd_device,
				       ret);
				return -1;
			}
		}

		result = meminfo->erase(meminfo, &erase);
		if (result != 0) {
			printf("\n%s: MTD Erase failure: %d\n",
			       mtd_device, result);
			       
		    result = meminfo->block_markbad(meminfo, erase.addr);
            if (result)
            {
                printf("Can not mark %llu bad, error 0x%x\r\n", erase.addr, result);
            }
			else
			{
			    printf("Add a new bad block 0x%llx to BBT\n", erase.addr);
			}
			
			continue;
		}

		/* format for JFFS2 ? */
		if (opts->jffs2) {

			/* write cleanmarker */
			if (isNAND) {
                                tempOobOps.ooblen=clmlen;
                                tempOobOps.oobbuf=(unsigned char *)&cleanmarker;
			result = meminfo->write_oob(meminfo,
			                            erase.addr+clmpos,
			                            &tempOobOps);
			if (result != 0) {
				printf("\n%s: MTD writeoob failure: %d\n",
				       mtd_device, result);
				continue;
			}
			} else {
				printf("\n%s: this erase routine only supports"
				       " NAND devices!\n",
				       mtd_device);
			}
		}

		if (!opts->quiet){
			int percent = (int)
				((unsigned long long)
				(erase.addr + meminfo->erasesize - opts->offset)
				 * 100 / erase_length);

			/* output progress message only at whole percent
			 * steps to reduce the number of messages printed
			 * on (slow) serial consoles
			 */
			if (percent != percent_complete) {
				percent_complete = percent;

				printf("\rErasing at 0x%x -- %3d%% complete.",
				       erase.addr, percent);

				if (opts->jffs2 && result == 0)
					printf(" Cleanmarker written at 0x%x.",
					       erase.addr);
			}
		}
	}
	if (!opts->quiet)
		printf("\n");

	if (nand_block_bad_old) {
		struct nand_chip *priv_nand = meminfo->priv;

		priv_nand->block_bad = nand_block_bad_old;
		priv_nand->scan_bbt(meminfo);
	}

	return 0;
}

/*
 * buffer array used for writing data
 */
static unsigned char data_buf[MAX_PAGE_SIZE + MAX_OOB_SIZE];
static unsigned char oob_buf[MAX_OOB_SIZE];

/* OOB layouts to pass into the kernel as default */
static struct nand_oobinfo none_oobinfo = {
	.useecc = MTD_NANDECC_OFF,
};

static struct nand_oobinfo jffs2_oobinfo = {
	.useecc = MTD_NANDECC_PLACE,
	.eccbytes = 6,
	.eccpos = { 0, 1, 2, 3, 6, 7 }
};

static struct nand_oobinfo yaffs_oobinfo = {
	.useecc = MTD_NANDECC_PLACE,
	.eccbytes = 6,
	.eccpos = { 8, 9, 10, 13, 14, 15}
};

static struct nand_oobinfo autoplace_oobinfo = {
	.useecc = MTD_NANDECC_AUTOPLACE
};

extern EN_NAND_FS_FORMAT nand_fs_format;

/**
 * _nand_write_opts: - write image to NAND flash with support for various options
 *
 * @param meminfo        AND device to erase
 * @param opts              write options (@see nand_write_options)
 * @param nandoffset     nand offset where image writen to
 * @return                      in case of success
 *
 * This code is ported from nandwrite.c from Linux mtd utils by
 * Strawfly.
 */
int _nand_write_opts(nand_info_t *meminfo, const nand_write_options_t *opts, ulong nandoffset)
{
        int imglen = 0;
        int pagelen;
        int baderaseblock;
        int blockstart = -1;
        loff_t offs, offsnoOob;
        int readlen;
        int oobinfochanged = 0;
        int percent_complete = -1;
        struct nand_oobinfo old_oobinfo;
        ulong mtdoffset = opts->offset;
        ulong erasesize_blockalign;
        u_char *buffer = opts->buffer;
        size_t written;
        int result;
        int ublankpage = 0, eccscan;
        ulong nandsize_oob, mtdoffsetnoOob;
        struct mtd_oob_ops tempOobOps;

    // For usb read
#if (CONFIG_COMMANDS & CFG_CMD_FAT)
    block_dev_desc_t *dev_desc=NULL;
    int ret, dev=0;
    int part=1;
    char *ep;
    int fat_read_len = 0, is_fatread = 1;
    long lPartno = 0;
    char * cPartno = getenv("partno");

    if (NAND_FS_FORMAT_MKBIN == nand_fs_format)
    {
        if(cPartno == NULL) {
            part = 1;
            lPartno = 1;
        }
        else {
            lPartno = simple_strtol(cPartno, NULL, 10);
            part = lPartno;
            if (part == 0)//env haven't set
                part = 1;
        }

        dev = (int)simple_strtoul ("0", &ep, 16);
        dev_desc=get_dev("usb",dev);
        if (dev_desc==NULL) {
            puts ("\n** Invalid boot device **\n");
            return -1;
        }
        if (*ep) {
            if (*ep != ':') {
                puts ("\n** Invalid boot device, use `dev[:part]' **\n");
                return -1;
            }
            ++ep;
            part = (int)simple_strtoul(ep, NULL, 16);
        }
        if (fat_register_device(dev_desc, part)!=0) {
            printf ("\n** Unable to use %s %d:%d for fatload **\n", "usb", dev, part);
            return -1;
        }
    }
#endif

        if (opts->pad && opts->writeoob)
        {
            printf("Can't pad when oob data is present.\n");
            return -1;
        }

        /* set erasesize to specified number of blocks - to match
        * jffs2 (virtual) block size */
        if (opts->blockalign == 0)
        {
            erasesize_blockalign = (meminfo->erasesize / meminfo->oobblock)
                                    * (meminfo->oobblock + meminfo->oobsize);
        }
        else
        {
            erasesize_blockalign = (meminfo->erasesize / meminfo->oobblock)
                                    * (meminfo->oobblock + meminfo->oobsize) * opts->blockalign;
        }

        /* make sure device page sizes are valid */
        if ((!((meminfo->oobsize == 16) && (meminfo->oobblock == 512)))
            && (!((meminfo->oobsize == 8U) && (meminfo->oobblock == 256U)))
            && (!((meminfo->oobsize == 64U) && (meminfo->oobblock == 2048U)))
            && (!((meminfo->oobsize == 128U) && (meminfo->oobblock == 2048U)))
            && (!((meminfo->oobsize == 128U) && (meminfo->oobblock == 4096U)))
            && (!((meminfo->oobsize == 432U) && (meminfo->oobblock == 8192U))))
        {
            printf("Unknown flash (not normal NAND)\n");
            return -1;
        }

        /* read the current oob info */
        memcpy(&old_oobinfo, &meminfo->oobinfo, sizeof(old_oobinfo));

        /* write without ecc? */
        if (opts->noecc)
        {
            memcpy(&meminfo->oobinfo, &none_oobinfo,
                    sizeof(meminfo->oobinfo));
            oobinfochanged = 1;
        }

        /* autoplace ECC? */
        if (opts->autoplace && (old_oobinfo.useecc != MTD_NANDECC_AUTOPLACE))
        {

            memcpy(&meminfo->oobinfo, &autoplace_oobinfo,
                    sizeof(meminfo->oobinfo));
            oobinfochanged = 1;
        }

        /* force OOB layout for jffs2 or yaffs? */
        if (opts->forcejffs2 || opts->forceyaffs)
        {
            struct nand_oobinfo *oobsel =
                    opts->forcejffs2 ? &jffs2_oobinfo : &yaffs_oobinfo;

            if (meminfo->oobsize == 8)
            {
                if (opts->forceyaffs)
                {
                        printf("YAFSS cannot operate on "
                                "256 Byte page size\n");
                        goto restoreoob;
                }
                /* Adjust number of ecc bytes */
                jffs2_oobinfo.eccbytes = 3;
            }

            memcpy(&meminfo->oobinfo, oobsel, sizeof(meminfo->oobinfo));
        }

        /* get image length */
        imglen = opts->length;
        pagelen = meminfo->oobblock + meminfo->oobsize;

        /* check, if file is pagealigned */
        if ((!opts->pad) && ((imglen % pagelen) != 0))
        {
            printf("Input block length is not page aligned\n");
            goto restoreoob;
        }

        /* check, if length fits into device */
        nandsize_oob = (meminfo->size / meminfo->oobblock) * (meminfo->oobblock + meminfo->oobsize);
//        printk("imglen=0x%lx, nandsize_oob - opts->offset=0x%lx, opts->offset=0x%lx\n",
//                    imglen, nandsize_oob - opts->offset, opts->offset);
        if (imglen > (nandsize_oob - opts->offset - nandoffset))
        {
//            printf("Image %d bytes, NAND page %d bytes, "
//                    "OOB area %ld bytes, device size %ld bytes\n",
//                    imglen, pagelen, meminfo->oobsize, nandsize_oob);
            printf("Input block does not fit into device\n");
            goto restoreoob;
        }

        if (!opts->quiet)
        {
            printf("\n");
        }

        /* get data from input and write to the device */
        while (imglen && (mtdoffset < (nandsize_oob - nandoffset)))
        {

            WATCHDOG_RESET ();

            /*
            * new eraseblock, check for bad block(s). Stay in the
            * loop to be sure if the offset changes because of
            * a bad block, that the next block that will be
            * written to is also checked. Thus avoiding errors if
            * the block(s) after the skipped block(s) is also bad
            * (number of blocks depending on the blockalign
            */
            while (blockstart != (mtdoffset & (~erasesize_blockalign+1)))
            {
                    blockstart = mtdoffset & (~erasesize_blockalign+1);
                    offs = blockstart;
                    baderaseblock = 0;

                    /* check all the blocks in an erase block for
                    * bad blocks */
                    do {
                            offsnoOob = offs / (meminfo->oobblock + meminfo->oobsize) * meminfo->oobblock;
                            int ret = meminfo->block_isbad(meminfo, offsnoOob + nandoffset);

                            if ((ret < 0) & (offsnoOob < (meminfo->size - nandoffset)))
                            {
                                printf("Bad block check failed\n");
                                goto restoreoob;
                            }
                            if (ret == 1)
                            {
                                baderaseblock = 1;
                                if (!opts->quiet)
                                {
                                        printf("\rBad block at 0x%lx "
                                                "in erase block from "
                                                "0x%x will be skipped\n",
                                                (long) offsnoOob,
                                                blockstart);
                                }
                            }

                            if (baderaseblock)
                            {
                                mtdoffset = blockstart
                                        + erasesize_blockalign;
//                                printk("mtdoffset = 0x%lx\n",mtdoffset);
                            }
                            offs += erasesize_blockalign
                                    / opts->blockalign;
                    } while ((offs < blockstart + erasesize_blockalign) &&
                                                (offs <= nandsize_oob - erasesize_blockalign - nandoffset));
            }

          #if (CONFIG_COMMANDS & CFG_CMD_FAT)
              if (NAND_FS_FORMAT_MKBIN == nand_fs_format)
              {
                  if (1 == is_fatread)
                  {
                      if (imglen >= FAT_BUFFER_LEN)
                      {
                          fat_read_len = FAT_BUFFER_LEN;
                      }
                      else
                      {
                          fat_read_len = imglen;
                      }
                      extern int file_fat_part_read(char *,unsigned char *,int,int);
                      ret = file_fat_part_read (opts->fatfile_name, (unsigned char *)(opts->buffer), mtdoffset, fat_read_len);
                      flush_cache((ulong)opts->buffer, fat_read_len);
                      is_fatread = 0;
                      buffer = opts->buffer;

                      if(ret == -1) {
                          printf("\n** Unable to read \"%s\" from %s %d:%d **\n", opts->fatfile_name, "usb", dev, part);
                          return 1;
                      }
                  }
              }
          #endif

              readlen = meminfo->oobblock;

            if (opts->pad && (imglen - meminfo->oobsize < readlen))
            {
                readlen = imglen - meminfo->oobsize;
                if(readlen < sizeof(data_buf))
                {
                    if(meminfo->oobblock < sizeof(data_buf))
                    {
                        memset(data_buf + readlen, 0xff,
                            meminfo->oobblock - readlen);
                    }
                    else
                    {
                        memset(data_buf + readlen, 0xff,
                            sizeof(data_buf) - readlen);
                    }
                }
                else
                {
                    printf("%s: Error: readlen exceeds the size of data_buf, at %d\n", __func__, __LINE__);
                }
            }

          #if (CONFIG_COMMANDS & CFG_CMD_FAT)
              if ((NAND_FS_FORMAT_MKBIN == nand_fs_format) && (0x800 == meminfo->oobblock))
              {
                  memcpy(data_buf, buffer, meminfo->oobblock);
                  flush_cache((ulong)buffer, meminfo->oobblock);
                  for(eccscan=meminfo->oobsize; eccscan>0; eccscan--){
                        if(buffer[meminfo->oobblock + eccscan] != 0xff){
                            ublankpage = 1;
                            break;
                            }
                  }
              }
              else
              {
                    /* read page data from input memory buffer */
                    if(readlen < sizeof(data_buf))
                    {
                        memcpy(data_buf, buffer, readlen);
                    }
                    else
                    {
                        memcpy(data_buf, buffer, sizeof(data_buf));
                    }
              }
              if (NAND_FS_FORMAT_MKBIN == nand_fs_format)
              {
                  buffer += (meminfo->oobblock + meminfo->oobsize);
                  fat_read_len = fat_read_len - meminfo->oobblock - meminfo->oobsize;
                  if (fat_read_len <= 0)
                  {
                      is_fatread = 1;
                  }
              }
              else
              {
                  buffer += readlen;
              }
          #else
              /* read page data from input memory buffer */
              memcpy(data_buf, buffer, readlen);
              buffer += readlen;
          #endif

            if (opts->writeoob)
            {
                    /* read OOB data from input memory block, exit
                    * on failure */
                    memcpy(oob_buf, buffer, meminfo->oobsize);
                    buffer += meminfo->oobsize;

                    /* write OOB data first, as ecc will be placed
                    * in there*/
                    tempOobOps.ooblen = meminfo->oobsize;

                    tempOobOps.oobbuf = (unsigned char *)&oob_buf;
                    result = meminfo->write_oob(meminfo, mtdoffset, &tempOobOps);
                    written=tempOobOps.oobretlen;
                    if (result != 0)
                    {
                        printf("\nMTD writeoob failure: %d\n",
                                    result);
                        goto restoreoob;
                    }
                    imglen -= meminfo->oobsize;
            }


            if(ublankpage && (mtdoffset < (nandsize_oob - nandoffset))){
                    ublankpage = 0;
                    mtdoffsetnoOob = mtdoffset / (meminfo->oobblock + meminfo->oobsize) * meminfo->oobblock;
//                    printk("mtdoffset=0x%lx, mtdoffsetnoOob=0x%lx\n",mtdoffset,mtdoffsetnoOob);
                    /* write out the page data */
                    result = meminfo->write(meminfo,
                                    (mtdoffsetnoOob + nandoffset),
                                    meminfo->oobblock,
                                    &written,
                                    (unsigned char *) &data_buf);

                    if (result != 0)
                    {
                        printf("writing NAND page at offset 0x%lx failed\n",
                                (mtdoffsetnoOob + nandoffset));
                        goto restoreoob;
                    }
            }
            imglen -= (readlen + meminfo->oobsize);

            if (!opts->quiet)
            {
                    int percent = (int)
                            ((unsigned long long)
                            (opts->length-imglen) * 100
                            / opts->length);
                    /* output progress message only at whole percent
                    * steps to reduce the number of messages printed
                    * on (slow) serial consoles
                    */
                    if (percent != percent_complete)
                    {
                        printf("\rWriting data at 0x%lux "
                                "-- %3d%% complete.",
                                mtdoffset, percent);
                        percent_complete = percent;
                    }
            }

            mtdoffset += (meminfo->oobblock + meminfo->oobsize);
        }

        if (!opts->quiet)
        {
            printf("\n");
        }

restoreoob:
        if (oobinfochanged)
        {
            memcpy(&meminfo->oobinfo, &old_oobinfo,
                        sizeof(meminfo->oobinfo));
        }

        if (imglen > 0)
        {
            printf("Data did not fit into device, due to bad blocks, imglen= 0x%x\n",imglen);
            return -1;
        }

        /* return happy */
        return 0;
}

/**
 * nand_write_opts: - write image to NAND flash with support for various options
 *
 * @param meminfo       AND device to erase
 * @param opts              write options (@see nand_write_options)
 * @return                      in case of success
 *
 * This code is ported from nandwrite.c from Linux mtd utils by
 * Steven J. Hill and Thomas Gleixner.
 */
int nand_write_opts(nand_info_t *meminfo, const nand_write_options_t *opts)
{
	int imglen = 0;
	int pagelen;
	int baderaseblock;
	int blockstart = -1;
	loff_t offs;
	int readlen;
	int oobinfochanged = 0;
	int percent_complete = -1;
	struct nand_oobinfo old_oobinfo;
	ulong mtdoffset = opts->offset;
	ulong erasesize_blockalign;
	u_char *buffer = opts->buffer;
	size_t written;
	int result;
        struct mtd_oob_ops tempOobOps;

    // For usb read
#if (CONFIG_COMMANDS & CFG_CMD_FAT)
    block_dev_desc_t *dev_desc=NULL;
    int i, ret, dev=0;
    int part=1;
    char *ep;
    int fat_read_len = 0, is_fatread = 1;
    long lPartno = 0;
    char * cPartno = getenv("partno");

    if (NAND_FS_FORMAT_MKBIN == nand_fs_format)
    {
        if(cPartno == NULL) {
            part = 1;
            lPartno = 1;
        }
        else {
            lPartno = simple_strtol(cPartno, NULL, 10);
        	part = lPartno;
            if (part == 0)//env haven't set
                part = 1;
        }

        dev = (int)simple_strtoul ("0", &ep, 16);
        dev_desc=get_dev("usb",dev);
        if (dev_desc==NULL) {
        	puts ("\n** Invalid boot device **\n");
        	return -1;
        }
        if (*ep) {
        	if (*ep != ':') {
        		puts ("\n** Invalid boot device, use `dev[:part]' **\n");
        		return -1;
        	}
        	part = (int)simple_strtoul(++ep, NULL, 16);
        }
        if (fat_register_device(dev_desc, part)!=0) {
        	printf ("\n** Unable to use %s %d:%d for fatload **\n", "usb", dev, part);
        	return -1;
        }
    }
#endif

	if (opts->pad && opts->writeoob)
    {
		printf("Can't pad when oob data is present.\n");
		return -1;
	}

	/* set erasesize to specified number of blocks - to match
	 * jffs2 (virtual) block size */
	if (opts->blockalign == 0)
    {
		erasesize_blockalign = meminfo->erasesize;
	}
    else
    {
		erasesize_blockalign = meminfo->erasesize * opts->blockalign;
	}

	/* make sure device page sizes are valid */
	if (!(meminfo->oobsize == 16 && meminfo->oobblock == 512)
            && (!((meminfo->oobsize == 8U) && (meminfo->oobblock == 256U)))
            && (!((meminfo->oobsize == 64U) && (meminfo->oobblock == 2048U)))
            && (!((meminfo->oobsize == 128U) && (meminfo->oobblock == 2048U)))
            && (!((meminfo->oobsize == 128U) && (meminfo->oobblock == 4096U)))
            && (!((meminfo->oobsize == 432U) && (meminfo->oobblock == 8192U))))
	{
		printf("Unknown flash (not normal NAND)\n");
		return -1;
	}

	/* read the current oob info */
	memcpy(&old_oobinfo, &meminfo->oobinfo, sizeof(old_oobinfo));

	/* write without ecc? */
	if (opts->noecc)
    {
		memcpy(&meminfo->oobinfo, &none_oobinfo,
		       sizeof(meminfo->oobinfo));
		oobinfochanged = 1;
	}

	/* autoplace ECC? */
	if (opts->autoplace && (old_oobinfo.useecc != MTD_NANDECC_AUTOPLACE))
    {

		memcpy(&meminfo->oobinfo, &autoplace_oobinfo,
		       sizeof(meminfo->oobinfo));
		oobinfochanged = 1;
	}

	/* force OOB layout for jffs2 or yaffs? */
	if (opts->forcejffs2 || opts->forceyaffs)
    {
		struct nand_oobinfo *oobsel =
			opts->forcejffs2 ? &jffs2_oobinfo : &yaffs_oobinfo;

		if (meminfo->oobsize == 8)
        {
			if (opts->forceyaffs)
            {
				printf("YAFSS cannot operate on "
				       "256 Byte page size\n");
				goto restoreoob;
			}
			/* Adjust number of ecc bytes */
			jffs2_oobinfo.eccbytes = 3;
		}

		memcpy(&meminfo->oobinfo, oobsel, sizeof(meminfo->oobinfo));
	}

	/* get image length */
	imglen = opts->length;
	pagelen = meminfo->oobblock
		+ ((opts->writeoob != 0) ? meminfo->oobsize : 0);

	/* check, if file is pagealigned */
	if ((!opts->pad) && ((imglen % pagelen) != 0))
    {
		printf("Input block length is not page aligned\n");
		goto restoreoob;
	}

	/* check, if length fits into device */
	if (((imglen / pagelen) * meminfo->oobblock)
	     > (meminfo->size - opts->offset))
	{
		printf("Image %d bytes, NAND page %d bytes, "
		       "OOB area %u bytes, device size %u bytes\n",
		       imglen, pagelen, meminfo->oobblock, meminfo->size);
		printf("Input block does not fit into device\n");
		goto restoreoob;
	}

	if (!opts->quiet)
	{
		printf("\n");
	}

	/* get data from input and write to the device */
	while (imglen && (mtdoffset < meminfo->size))
    {

		WATCHDOG_RESET ();

		/*
		 * new eraseblock, check for bad block(s). Stay in the
		 * loop to be sure if the offset changes because of
		 * a bad block, that the next block that will be
		 * written to is also checked. Thus avoiding errors if
		 * the block(s) after the skipped block(s) is also bad
		 * (number of blocks depending on the blockalign
		 */
		while (blockstart != (mtdoffset & (~erasesize_blockalign+1)))
        {
			blockstart = mtdoffset & (~erasesize_blockalign+1);
			offs = blockstart;
			baderaseblock = 0;

			/* check all the blocks in an erase block for
			 * bad blocks */
			do {
				int ret = meminfo->block_isbad(meminfo, offs);

				if (ret < 0)
                {
					printf("Bad block check failed\n");
					goto restoreoob;
				}
				if (ret == 1)
                {
					baderaseblock = 1;
					if (!opts->quiet)
					{
						printf("\rBad block at 0x%lx "
						       "in erase block from "
						       "0x%x will be skipped\n",
						       (long) offs,
						       blockstart);
					}
				}

				if (baderaseblock)
                {
					mtdoffset = blockstart
						+ erasesize_blockalign;
				}
				offs +=	 erasesize_blockalign
					/ opts->blockalign;
			} while (offs < blockstart + erasesize_blockalign);
		}

          #if (CONFIG_COMMANDS & CFG_CMD_FAT)
              if (NAND_FS_FORMAT_MKBIN == nand_fs_format)
              {
                  if (1 == is_fatread)
                  {
                      if (imglen >= FAT_BUFFER_LEN)
                      {
                          fat_read_len = FAT_BUFFER_LEN;
                      }
                      else
                      {
                          fat_read_len = (imglen / meminfo->oobblock) * (meminfo->oobblock + meminfo->oobsize);
                      }
                	 ret = file_fat_read (opts->fatfile_name, (unsigned char *)(opts->buffer), fat_read_len);
                      flush_cache((ulong)opts->buffer, fat_read_len);
                      is_fatread = 0;
                      buffer = opts->buffer;

                	 if(ret == -1) {
                          printf("\n** Unable to read \"%s\" from %s %d:%d **\n", opts->fatfile_name, "usb", dev, part);
                          return 1;
                      }
                  }
              }
          #endif

              readlen = meminfo->oobblock;

            if (opts->pad && (imglen < readlen))
            {
                readlen = imglen;
                if(readlen < sizeof(data_buf))
                {
                    if(meminfo->oobblock < sizeof(data_buf))
                    {
                        memset(data_buf + readlen, 0xff,
                            meminfo->oobblock - readlen);
                    }
                    else
                    {
                        memset(data_buf + readlen, 0xff,
                            sizeof(data_buf) - readlen);
                    }
                }
                else
                {
                    printf("%s: Error: readlen exceeds the size of data_buf, at %d\n", __func__, __LINE__);
                }
            }

          #if (CONFIG_COMMANDS & CFG_CMD_FAT)
              if ((NAND_FS_FORMAT_MKBIN == nand_fs_format) && (0x800 == meminfo->oobblock))
              {
                  for (i = 0; i < 4; i++)
                  {
                      memcpy(data_buf + 512 * i, buffer + 528 * i, 512);
                      flush_cache((ulong)buffer, 2048);
                  }
              }
              else
              {
                    /* read page data from input memory buffer */
                    if(readlen < sizeof(data_buf))
                    {
                        memcpy(data_buf, buffer, readlen);
                    }
                    else
                    {
                        memcpy(data_buf, buffer, sizeof(data_buf));
                    }
              }
              if (NAND_FS_FORMAT_MKBIN == nand_fs_format)
              {
		    buffer += (meminfo->oobblock + meminfo->oobsize);
                  fat_read_len = fat_read_len - meminfo->oobblock - meminfo->oobsize;
                  if (fat_read_len <= 0)
                  {
                      is_fatread = 1;
                  }
              }
              else
              {
                  buffer += readlen;
              }
          #else
              /* read page data from input memory buffer */
              memcpy(data_buf, buffer, readlen);
              buffer += readlen;
          #endif

		if (opts->writeoob)
        {
			/* read OOB data from input memory block, exit
			 * on failure */
			memcpy(oob_buf, buffer, meminfo->oobsize);
			buffer += meminfo->oobsize;

			/* write OOB data first, as ecc will be placed
			 * in there*/
			tempOobOps.ooblen = meminfo->oobsize;

                        tempOobOps.oobbuf = (unsigned char *)&oob_buf;
			result = meminfo->write_oob(meminfo, mtdoffset, &tempOobOps);
                        written=tempOobOps.oobretlen;
			if (result != 0)
            {
				printf("\nMTD writeoob failure: %d\n",
				       result);
				goto restoreoob;
			}
			imglen -= meminfo->oobsize;
		}

		/* write out the page data */
		result = meminfo->write(meminfo,
					mtdoffset,
					meminfo->oobblock,
					&written,
					(unsigned char *) &data_buf);

		if (result != 0)
        {
			printf("writing NAND page at offset 0x%lx failed\n",
			       mtdoffset);
			goto restoreoob;
		}
		imglen -= readlen;

		if (!opts->quiet)
        {
			int percent = (int)
				((unsigned long long)
				 (opts->length-imglen) * 100
				 / opts->length);
			/* output progress message only at whole percent
			 * steps to reduce the number of messages printed
			 * on (slow) serial consoles
			 */
			if (percent != percent_complete)
            {
				printf("\rWriting data at 0x%lux "
				       "-- %3d%% complete.",
				       mtdoffset, percent);
				percent_complete = percent;
			}
		}

		mtdoffset += meminfo->oobblock;
	}

	if (!opts->quiet)
	{
		printf("\n");
	}

restoreoob:
	if (oobinfochanged)
    {
		memcpy(&meminfo->oobinfo, &old_oobinfo,
		       sizeof(meminfo->oobinfo));
	}

	if (imglen > 0)
    {
		printf("Data did not fit into device, due to bad blocks\n");
		return -1;
	}

	/* return happy */
	return 0;
}

/**
 * nand_read_opts: - read image from NAND flash with support for various options
 *
 * @param meminfo	NAND device to erase
 * @param opts		read options (@see struct nand_read_options)
 * @return		0 in case of success
 *
 */
int nand_read_opts(nand_info_t *meminfo, const nand_read_options_t *opts)
{
	int imglen = opts->length;
	int pagelen;
	int baderaseblock;
	int blockstart = -1;
	int percent_complete = -1;
	loff_t offs;
	size_t readlen, readlenoob;
	ulong mtdoffset = opts->offset;
	u_char *buffer = opts->buffer;
	int result;
    struct mtd_oob_ops tempOobOps;

    // For usb write
#if (CONFIG_COMMANDS & CFG_CMD_FAT)
    block_dev_desc_t *dev_desc=NULL;
    int dev=0;
    int part=1;
    char *ep;
    int fd = 0;
    u_char *buf_end = buffer;
    int fat_write_len = 0, is_fatwrite = 0;
#endif

	/* make sure device page sizes are valid */
	if (!(meminfo->oobsize == 16 && meminfo->oobblock == 512)
            && (!((meminfo->oobsize == 8U) && (meminfo->oobblock == 256U)))
            && (!((meminfo->oobsize == 64U) && (meminfo->oobblock == 2048U)))
            && (!((meminfo->oobsize == 128U) && (meminfo->oobblock == 2048U)))
            && (!((meminfo->oobsize == 128U) && (meminfo->oobblock == 4096U)))
            && (!((meminfo->oobsize == 432U) && (meminfo->oobblock == 8192U))))
	{
		printf("Unknown flash (not normal NAND)\n");
		return -1;
	}

	pagelen = meminfo->oobblock + ((opts->readoob != 0) ? meminfo->oobsize : 0);

	/* check, if length is not larger than device */
	if (((imglen / pagelen) * meminfo->oobblock) > (meminfo->size - opts->offset))
	{
		printf("Image %d bytes, NAND page %d bytes, "
		       "OOB area %u bytes, device size %u bytes\n",
		       imglen, pagelen, meminfo->oobblock, meminfo->size);
		printf("Input block is larger than device\n");
		return -1;
	}

	if (!opts->quiet)
	{
		printf("\n");
	}

       /* Write to usb for binary file making
        Initialize something and open usb file */
   #if (CONFIG_COMMANDS & CFG_CMD_FAT)
       if (NAND_FS_FORMAT_MKBIN == nand_fs_format)
       {
              dev = (int)simple_strtoul ("0", &ep, 16);
        	dev_desc=get_dev("usb",dev);
        	if (dev_desc==NULL) {
        		puts ("\n** Invalid boot device **\n");
        		return -1;
        	}
        	if (*ep) {
        		if (*ep != ':') {
        			puts ("\n** Invalid boot device, use `dev[:part]' **\n");
        			return -1;
        		}
        		part = (int)simple_strtoul(++ep, NULL, 16);
        	}
           	if (fat_register_device(dev_desc,part)!=0) {
		printf ("\n** Unable to use %s %d:%d for fatwrite **\n","usb",dev,part);
		return -1;
	    }

            fd = file_fat_open(opts->fatfile_name, "a");
			if(fd < 0) {
         		printf ("Error: file open %s failed\n",opts->fatfile_name);
				return -1;
			}
       }
   #endif

	/* get data from input and write to the device */
	while (imglen && (mtdoffset < meminfo->size))
    {
		WATCHDOG_RESET ();

        if (opts->readoob)
        {
    		/*
    		 * new eraseblock, check for bad block(s). Stay in the
    		 * loop to be sure if the offset changes because of
    		 * a bad block, that the next block that will be
    		 * written to is also checked. Thus avoiding errors if
    		 * the block(s) after the skipped block(s) is also bad
    		 * (number of blocks depending on the blockalign
    		 */
    		while (blockstart != (mtdoffset & (~meminfo->erasesize+1)))
            {
    			blockstart = mtdoffset & (~meminfo->erasesize+1);
    			offs = blockstart;
    			baderaseblock = 0;

    			/* check all the blocks in an erase block for
    			 * bad blocks */
    			do {
    				int ret = meminfo->block_isbad(meminfo, offs);

    				if (ret < 0)
                    {
    					printf("Bad block check failed\n");
    					return -1;
    				}

    				if (ret == 1)
                    {
    					baderaseblock = 1;
    					if (!opts->quiet)
    					{
    						printf("\rBad block at 0x%lx "
    						       "in erase block from "
    						       "0x%x will be skipped\n",
    						       (long) offs,
    						       blockstart);
    					}
    				}

    				if (baderaseblock)
                    {
    					mtdoffset = blockstart
    						+ meminfo->erasesize;
    				}
    				offs +=	 meminfo->erasesize;

    			} while (offs < blockstart + meminfo->erasesize);
    		}

    		/* read page data to memory buffer */
            if ((NAND_FS_FORMAT_RAW == nand_fs_format)
                    || (NAND_FS_FORMAT_MKBIN == nand_fs_format))
            {
                result = nand_read_raw(meminfo, data_buf, mtdoffset, meminfo->oobblock, meminfo->oobsize);

                flush_cache((ulong)data_buf, (meminfo->oobblock + meminfo->oobsize));
                readlen = meminfo->oobblock + meminfo->oobsize;
            }
            else
            {
                result = meminfo->read(meminfo,
    				       mtdoffset,
    				       meminfo->oobblock,
    				       &readlen,
    				       (unsigned char *) &data_buf);
            }

    		if (result != 0)
            {
    			printf("reading NAND page at offset 0x%lx failed\n",
    			       mtdoffset);
    			return -1;
    		}

    		if (imglen <= readlen)
            {
    			readlen = imglen;
            #if (CONFIG_COMMANDS & CFG_CMD_FAT)
                if (NAND_FS_FORMAT_MKBIN == nand_fs_format)
                {
                     is_fatwrite = 1;
                 }
            #endif
    		}

            /* NAND_FS_FORMAT_RAW means read both main and oob area and comply with the
              special oob layout of FCIE2 (512 16, 512 16, 512 16, 512 16 instead of 2048 64) */
            if ((NAND_FS_FORMAT_RAW == nand_fs_format) && (0x800 == meminfo->oobblock))
            {
                //for (i = 0; i < 4; i++)
                //{
                    //memcpy((buffer + 528 * i), (data_buf + 512 * i), 512);
                    //memcpy((buffer + 528 * i + 512), (data_buf + 2048 + 16 * i), 16);
                //}
                memcpy(buffer, data_buf, readlen);
            }
        #if (CONFIG_COMMANDS & CFG_CMD_FAT)
            else if (NAND_FS_FORMAT_MKBIN == nand_fs_format)
            {
                //if (0x800 == meminfo->oobblock)
                //{
                    //for (i = 0; i < 4; i++)
                    //{
                        //memcpy((buf_end + 528 * i), (data_buf + 512 * i), 512);
                        //memcpy((buf_end + 528 * i + 512), (data_buf + 2048 + 16 * i), 16);
                    //}
                //}
                //else if (0x200 == meminfo->oobblock)
                //{
                    memcpy(buf_end, data_buf, readlen);
                //}

                buf_end += readlen;
                if (((buf_end - opts->buffer) >= FAT_BUFFER_LEN) || (1 == is_fatwrite))
                {
                    fat_write_len = buf_end - opts->buffer;
                    buf_end = opts->buffer;
                    is_fatwrite = 1;
                }
            }
        #endif
            else
            {
                memcpy(buffer, data_buf, readlen);
            }

            /* Write to usb for binary file making.
                 writing one page to usb each time can save memory consuming, so append writing mode is used */
        #if (CONFIG_COMMANDS & CFG_CMD_FAT)
            if (NAND_FS_FORMAT_MKBIN == nand_fs_format)
            {
                if((is_fatwrite == 1) && (fd >= 0))
                {
                    flush_cache((ulong)buf_end, fat_write_len);
                    file_fat_write(fd, (unsigned char *) buf_end, fat_write_len);
                    is_fatwrite = 0;
                }
            }
        #endif

    		buffer += readlen;
    		imglen -= readlen;

    		if ((opts->readoob) && (NAND_FS_FORMAT_RAW != nand_fs_format)
                        && (NAND_FS_FORMAT_MKBIN != nand_fs_format))
            {
                tempOobOps.ooblen = meminfo->oobsize;

                tempOobOps.oobbuf = (unsigned char *)&oob_buf;
    			result = meminfo->read_oob(meminfo, mtdoffset, &tempOobOps);
                        readlenoob=tempOobOps.oobretlen;

    			if (result != 0)
                {
    				printf("\nMTD readoob failure: %d\n",
    				       result);
    				return -1;
    			}


    			if (imglen < readlenoob)
                {
    				readlenoob = imglen;
    			}
                     else
                     {
                        memcpy(buffer, oob_buf, readlenoob);
                     }

    			buffer += readlenoob;
    			imglen -= readlenoob;
    		}

    		if (!opts->quiet)
            {
    			int percent = (int)
    				((unsigned long long)
    				 (opts->length-imglen) * 100
    				 / opts->length);
    			/* output progress message only at whole percent
    			 * steps to reduce the number of messages printed
    			 * on (slow) serial consoles
    			 */
    			if (percent != percent_complete)
                {
        			if (!opts->quiet)
        			{
#if (!CONFIG_MSTAR_SILENT)
        				printf("\rReading data from 0x%lux "
        				       "-- %3d%% complete.",
        				       mtdoffset, percent);
#endif
        				percent_complete = percent;
        			}
    			}
    		}

    		mtdoffset += meminfo->oobblock;
        }
        else
        {
            size_t wanted_readlen;

            wanted_readlen = 0;
            baderaseblock = 0;
            blockstart = mtdoffset & (~meminfo->erasesize+1);

            while (blockstart < mtdoffset + imglen) // bad block check does not exceed the image boundary
            {
                int ret = meminfo->block_isbad(meminfo, blockstart);

                if (ret < 0)
                {
                    printf("Bad block check failed\n");
                    return -1;
                }

                // bad block found
                if (ret == 1)
                {
                    baderaseblock = 1;

                    if (!opts->quiet)
                    {
                        printf("\rBad block at 0x%lx "
                               "in erase block from "
                               "0x%x will be skipped\n",
                               (long) blockstart,
                               blockstart);
                    }

                    break; // stop to seek bad block and copy the data first
                }

                // printf("mtdoffset = %X, blockstart = %X, meminfo->erasesize = %X, imglen = %X\n", mtdoffset, blockstart, meminfo->erasesize, imglen); // <-@@@ DEBUG MSG

                blockstart += meminfo->erasesize;

                wanted_readlen = (blockstart < mtdoffset + imglen) ? (blockstart - mtdoffset) :
                                                                     (imglen);
            }

            // printf("mtdoffset = %X, wanted_readlen = %X\n", mtdoffset, wanted_readlen); // <-@@@ DEBUG MSG

            /* read data to memory buffer */
            result = meminfo->read(meminfo,
                           mtdoffset,
                           wanted_readlen,
                           &readlen,
                           (unsigned char *) buffer);

    		if (result != 0)
            {
                printf("reading NAND page at offset 0x%lx failed\n", mtdoffset);
                return -1;
    		}


    		if (imglen < readlen)
            {
                readlen = imglen;
            }

            buffer += readlen;
            imglen -= readlen;

    		if (!opts->quiet)
            {
                int percent = (int)((unsigned long long)(opts->length-imglen) * 100 / opts->length);

                /* output progress message only at whole percent
                 * steps to reduce the number of messages printed
                 * on (slow) serial consoles
                 */
                if (percent != percent_complete)
                {
#if (!CONFIG_MSTAR_SILENT)
                    printf("\rReading data from 0x%lux " "-- %3d%% complete.", mtdoffset, percent);
#endif
                    percent_complete = percent;
    			}
    		}

    		mtdoffset += readlen;

            if (baderaseblock == 1)
            {
                mtdoffset = (mtdoffset + meminfo->erasesize) & (~meminfo->erasesize+1);
            }
        }
	}

	if (!opts->quiet)
	{
		printf("\n");
	}

	if (imglen > 0)
    {
		printf("Could not read entire image due to bad blocks\n");
		return -1;
	}

   #if (CONFIG_COMMANDS & CFG_CMD_FAT)
       if (NAND_FS_FORMAT_MKBIN == nand_fs_format)
       {
           file_fat_close(fd);
            /*if(fd==-1) {
                        printf("\n** Unable to write \"%s\" from %s %d:%d **\n",opts->fatfile_name,"usb",dev,part);
                        return 1;
                    }*/
       }
   #endif

	/* return happy */
	return 0;
}

/******************************************************************************
 * Support for locking / unlocking operations of some NAND devices
 *****************************************************************************/
/**
 * nand_lock: Set all pages of NAND flash chip to the LOCK or LOCK-TIGHT
 *	      state
 *
 * @param meminfo	nand mtd instance
 * @param tight		bring device in lock tight mode
 *
 * @return		0 on success, -1 in case of error
 *
 * The lock / lock-tight command only applies to the whole chip. To get some
 * parts of the chip lock and others unlocked use the following sequence:
 *
 * - Lock all pages of the chip using nand_lock(mtd, 0) (or the lockpre pin)
 * - Call nand_unlock() once for each consecutive area to be unlocked
 * - If desired: Bring the chip to the lock-tight state using nand_lock(mtd, 1)
 *
 *   If the device is in lock-tight state software can't change the
 *   current active lock/unlock state of all pages. nand_lock() / nand_unlock()
 *   calls will fail. It is only posible to leave lock-tight state by
 *   an hardware signal (low pulse on _WP pin) or by power down.
 */
int nand_lock(nand_info_t *meminfo, int tight)
{
	int ret = 0;
	int status;
	struct nand_chip *this = meminfo->priv;

	/* select the NAND device */
	this->select_chip(meminfo, 0);

	this->cmdfunc(meminfo,
		      (tight ? NAND_CMD_LOCK_TIGHT : NAND_CMD_LOCK),
		      -1, -1);

	/* call wait ready function */
	status = this->waitfunc(meminfo, this, FL_WRITING);

	/* see if device thinks it succeeded */
	if (status & 0x01) {
		ret = -1;
	}

	/* de-select the NAND device */
	this->select_chip(meminfo, -1);
	return ret;
}

/**
 * nand_get_lock_status: - query current lock state from one page of NAND
 *			   flash
 *
 * @param meminfo	nand mtd instance
 * @param offset	page address to query (muss be page aligned!)
 *
 * @return		-1 in case of error
 *			>0 lock status:
 *			  bitfield with the following combinations:
 *			  NAND_LOCK_STATUS_TIGHT: page in tight state
 *			  NAND_LOCK_STATUS_LOCK:  page locked
 *			  NAND_LOCK_STATUS_UNLOCK: page unlocked
 *
 */
int nand_get_lock_status(nand_info_t *meminfo, ulong offset)
{
	int ret = 0;
	int chipnr;
	int page;
	struct nand_chip *this = meminfo->priv;

	/* select the NAND device */
        chipnr = (int)((offset >> (unsigned long)(this->phys_erase_shift)) / this->blocknum);
        this->select_chip(meminfo, chipnr);


	if ((offset & (meminfo->oobblock - 1)) != 0) {
		printf ("nand_get_lock_status: "
			"Start address must be beginning of "
			"nand page!\n");
		ret = -1;
		goto out;
	}

	/* check the Lock Status */
	page = (int)(offset >> this->page_shift);
	this->cmdfunc(meminfo, NAND_CMD_LOCK_STATUS, -1, page & this->pagemask);

	ret = this->read_byte(meminfo) & (NAND_LOCK_STATUS_TIGHT
					  | NAND_LOCK_STATUS_LOCK
					  | NAND_LOCK_STATUS_UNLOCK);

 out:
	/* de-select the NAND device */
	this->select_chip(meminfo, -1);
	return ret;
}

/**
 * nand_unlock: - Unlock area of NAND pages
 *		  only one consecutive area can be unlocked at one time!
 *
 * @param meminfo	nand mtd instance
 * @param start		start byte address
 * @param length	number of bytes to unlock (must be a multiple of
 *			page size nand->oobblock)
 *
 * @return		0 on success, -1 in case of error
 */
int nand_unlock(nand_info_t *meminfo, ulong start, ulong length)
{
	int ret = 0;
	int chipnr;
	int status;
	int page;
	struct nand_chip *this = meminfo->priv;
	printf ("nand_unlock: start: %08x, length: %d!\n",
		(int)start, (int)length);

	/* select the NAND device */
        chipnr = (int)((start >> (unsigned long)(this->phys_erase_shift)) / this->blocknum);
        this->select_chip(meminfo, chipnr);

	/* check the WP bit */
	this->cmdfunc(meminfo, NAND_CMD_STATUS, -1, -1);
	if ((this->read_byte(meminfo) & 0x80) == 0) {
		printf ("nand_unlock: Device is write protected!\n");
		ret = -1;
		goto out;
	}

	if ((start & (meminfo->oobblock - 1)) != 0) {
		printf ("nand_unlock: Start address must be beginning of "
			"nand page!\n");
		ret = -1;
		goto out;
	}

	if (length == 0 || (length & (meminfo->oobblock - 1)) != 0) {
		printf ("nand_unlock: Length must be a multiple of nand page "
			"size!\n");
		ret = -1;
		goto out;
	}

	/* submit address of first page to unlock */
	page = (int)(start >> this->page_shift);
	this->cmdfunc(meminfo, NAND_CMD_UNLOCK1, -1, page & this->pagemask);

	/* submit ADDRESS of LAST page to unlock */
	page += (int)(length >> this->page_shift) - 1;
	this->cmdfunc(meminfo, NAND_CMD_UNLOCK2, -1, page & this->pagemask);

	/* call wait ready function */
	status = this->waitfunc(meminfo, this, FL_WRITING);
	/* see if device thinks it succeeded */
	if (status & 0x01) {
		/* there was an error */
		ret = -1;
		goto out;
	}

 out:
	/* de-select the NAND device */
	this->select_chip(meminfo, -1);
	return ret;
}

#endif /* (CONFIG_COMMANDS & CFG_CMD_NAND) && !defined(CFG_NAND_LEGACY) */
