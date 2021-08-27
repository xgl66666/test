/*
 * fat.c
 *
 * R/O (V)FAT 12/16/32 filesystem implementation by Marcus Sundberg
 *
 * 2002-07-28 - rjones@nexus-tech.net - ported to ppcboot v1.1.6
 * 2003-03-10 - kharris@nexus-tech.net - ported to uboot
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
#include <config.h>
#include <fat.h>
#include <asm/byteorder.h>
#include <part.h>
#include <configs/uboot_module_config.h>

extern int fileNum ,lookup ,jffs2Num ;
#if (CONFIG_COMMANDS & CFG_CMD_FAT)

extern int strcmp(const char * cs,const char * ct);

/*
 * Convert a string to lowercase.
 */
static void
downcase(char *str)
{
    while (*str != '\0') {
        TOLOWER(*str);
        str++;
    }
}

/*
 * Convert a string to lowercase.
 */
static void
upcase(char *str)
{
    while (*str != '\0') {
        TOHIGHER(*str);
        str++;
    }
}

static  block_dev_desc_t *cur_dev = NULL;
static unsigned long part_offset = 0;
static int cur_part = 1;

#define DOS_PART_TBL_OFFSET     0x1be
#define DOS_PART_MAGIC_OFFSET   0x1fe
#define DOS_FS_TYPE_OFFSET      0x36
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is to match filename
// [in] char * l_name  is filename pointer
// return 0 is not match
// return 1 is match
// date: 2009/08/17
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int validate_script_ready(char * l_name)
{
   int ret = 0;
   int i,j;
   char * const_filename[] ={
    "[[application_usb",
    "[[chakra_usb",
    "[[conf_usb",
    "[[font_usb",
    "[[kernel_usb",
    "[[opt_usb",
    "[[rootfs_usb",
    "[[vendor_usb",
    "set_config_usb",
    "usb_auto_update.txt",
    NULL
    };
   for(i=0;i<10;i++)
    {
       j = strcmp(l_name,const_filename[i]);
       if(j==0)
        {
           lookup++;
           ret = 1;
           break;

        }
    }

   return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This function is to match filename
// [in] char * l_name  is filename pointer
// return 0 is not match
// return 1 is match
// date: 2009/08/17
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int validate_image_ready(char * l_name)
{
   int ret = 0;
   int i,j;
   char * const_filename[] ={
    "application.jffs2",
    "chakra.jffs2",
    "conf.jffs2",
    "font.jffs2",
    "opt.jffs2",
    "res.bin",
    "rootfs.jffs2",
    "uimage.lzo",
    "vendor.jffs2",
    NULL
    };
   for(i=0;i<9;i++)
    {
       j = strcmp(l_name,const_filename[i]);
       if(j==0)
        {
           jffs2Num++;
           ret = 1;
           break;

        }
    }

   return ret;
}
int disk_read (__u32 startblock, __u32 getsize, __u8 * bufptr)
{
    FAT_DPRINT("disk_read(%d, %d, %p)\n", startblock, getsize, bufptr);

    startblock += part_offset;
    if (cur_dev == NULL)
        return -1;
    if (cur_dev->block_read) {
        return cur_dev->block_read (cur_dev->dev, startblock, getsize, (unsigned long *)bufptr);
    }
    return -1;
}


int disk_write (__u32 startblock, __u32 putsize, __u8 * bufptr)
{
    FAT_DPRINT("disk_write(%d, %d, %p)\n", startblock, putsize, bufptr);

    startblock += part_offset;
    if (cur_dev == NULL)
        return -1;
    if (cur_dev->block_write) {
        return cur_dev->block_write(cur_dev->dev, startblock, putsize, (unsigned long *)bufptr);
    }
    return -1;
}

/* If single partition, only support FAT32 */
static inline int is_partition_supported(unsigned char *buf)
{
    return ((buf[3]=='M' && buf[4]=='S' && buf[5]=='D' && buf[6]=='O') ||
        (buf[3]=='M' && buf[4]=='S' && buf[5]=='W' && buf[6]=='I') ||
        (buf[3]=='m' && buf[4]=='k' && buf[5]=='d' && buf[6]=='o') ||
        (buf[54]=='F' && buf[55]=='A' && buf[56]=='T') ||
        (buf[82]=='F' && buf[83]=='A' && buf[84]=='T')
        );
}

int
fat_register_device(block_dev_desc_t *dev_desc, int part_no)
{
    unsigned char buffer[MAX_SECTOR_SIZE];

    if (!dev_desc->block_read)
        return -1;
    cur_dev=dev_desc;
    /* check if we have a MBR (on floppies we have only a PBR) */
    if (dev_desc->block_read (dev_desc->dev, 0, 1, (ulong *) buffer) != 1) {
        printf ("** Can't read from device %d **\n", dev_desc->dev);
        return -1;
    }
    if (buffer[DOS_PART_MAGIC_OFFSET] != 0x55 ||
        buffer[DOS_PART_MAGIC_OFFSET + 1] != 0xaa) {
        /* no signature found */
        return -1;
    }

    {
#if (CONFIG_COMMANDS & CFG_CMD_IDE) || (CONFIG_COMMANDS & CFG_CMD_SCSI) || \
    (CONFIG_COMMANDS & CFG_CMD_USB) || defined(CONFIG_SYSTEMACE)

#if defined(CONFIG_MAC_PARTITION) || \
	defined(CONFIG_DOS_PARTITION) || \
	defined(CONFIG_ISO_PARTITION) || \
	defined(CONFIG_AMIGA_PARTITION)

        disk_partition_t info;
        if(!get_partition_info(dev_desc, part_no, &info)) {
            part_offset = info.start;
            cur_part = part_no;
        }
        else {
           if(((buffer[0] == 0xEB) || (buffer[0] == 0xE9)) &&
               is_partition_supported(buffer)
               ) {
                /* ok, we assume we are on a PBR only */
                cur_part = 1;
                part_offset=0;
            }
            else
            {
                printf ("** Partition %d not valid on device %d **\n",part_no,dev_desc->dev);
                return -1;
            }
        }
#else
        /* FIXME we need to determine the start block of the
         * partition where the DOS FS resides. This can be done
         * by using the get_partition_info routine. For this
         * purpose the libpart must be included.
         */
        part_offset=32;
        cur_part = 1;
#endif
#else
        /* FIXME we need to determine the start block of the
         * partition where the DOS FS resides. This can be done
         * by using the get_partition_info routine. For this
         * purpose the libpart must be included.
         */
        part_offset=32;
        cur_part = 1;
#endif
    }
    return 0;
}


/*
 * Get the first occurence of a directory delimiter ('/' or '\') in a string.
 * Return index into string if found, -1 otherwise.
 */
static int
dirdelim(char *str)
{
    char *start = str;

    while (*str != '\0') {
        if (ISDIRDELIM(*str)) return str - start;
        str++;
    }
    return -1;
}


/*
 * Extract zero terminated short name from a directory entry.
 */
static void get_name (dir_entry *dirent, char *s_name)
{
    char *ptr;

    memcpy (s_name, dirent->name, 8);
    s_name[8] = '\0';
    ptr = s_name;
    while (*ptr && *ptr != ' ')
        ptr++;
    if (dirent->ext[0] && dirent->ext[0] != ' ') {
        *ptr = '.';
        ptr++;
        memcpy (ptr, dirent->ext, 3);
        ptr[3] = '\0';
        while (*ptr && *ptr != ' ')
            ptr++;
    }
    *ptr = '\0';
    if (*s_name == DELETED_FLAG)
        *s_name = '\0';
    else if (*s_name == aRING)
        *s_name = DELETED_FLAG;
    downcase (s_name);
}


/*
 * Get free cluster in FAT
*  getno alway 1
*/
__u32
getfree_cluster(fsdata *mydata, int  * getno)
{
    __u32 bufnum = 0;
    __u32 offset = 0;
    __u32 u32Result = 0x00;
	__u32 entry = 0;
	__u32 ret = -1;
	__u32 u32foundno = 0;
	__u32 u32lastentry = 0;
	//mydata->fatbufnum = -1;
	//printf("getfree_cluster:%d||", *getno);
    if (mydata->fatbufnum != -1)
    {
        entry = mydata->fatbufnum * FAT32BUFSIZE;
    }
    while(1) {
	    switch (mydata->fatsize) {
	    case 32:
	        bufnum = entry / FAT32BUFSIZE;
	        offset = entry - bufnum * FAT32BUFSIZE;
	        break;
	    case 16:
	        bufnum = entry / FAT16BUFSIZE;
	        offset = entry - bufnum * FAT16BUFSIZE;
	        break;
	    case 12:
	        bufnum = entry / FAT12BUFSIZE;
	        offset = entry - bufnum * FAT12BUFSIZE;
	        break;

	    default:
	        /* Unsupported FAT size */
	        return -1;
	    }

	    if (bufnum != mydata->fatbufnum) {
	        int getsize = FATBUFSIZE/FS_BLOCK_SIZE;
	        __u8 *bufptr = mydata->fatbuf;
	        __u32 fatlength = mydata->fatlength;
	        __u32 startblock = bufnum * FATBUFBLOCKS;

	        fatlength *= SECTOR_SIZE;    /* We want it in bytes now */
	        startblock += mydata->fat_sect;    /* Offset from start of disk */

	        if (getsize > fatlength) getsize = fatlength;
	        if (disk_read(startblock, getsize, bufptr) < 0) {
	            FAT_DPRINT("Error reading FAT blocks\n");
	            return -1;
	        }
	        mydata->fatbufnum = bufnum;
	    }

	    /* Get the actual entry from the table */
	    switch (mydata->fatsize) {
	    case 32:
	        u32Result = FAT2CPU32(((__u32*)mydata->fatbuf)[offset]);
	        break;
	    case 16:
	        u32Result = FAT2CPU16(((__u16*)mydata->fatbuf)[offset]);
	        break;
	    case 12: {
	        __u32 off16 = (offset*3)/4;
	        __u16 val1, val2;

	        switch (offset & 0x3) {
	        case 0:
	            u32Result = FAT2CPU16(((__u16*)mydata->fatbuf)[off16]);
	            u32Result &= 0xfff;
	            break;
	        case 1:
	            val1 = FAT2CPU16(((__u16*)mydata->fatbuf)[off16]);
	            val1 &= 0xf000;
	            val2 = FAT2CPU16(((__u16*)mydata->fatbuf)[off16+1]);
	            val2 &= 0x00ff;
	            u32Result = (val2 << 4) | (val1 >> 12);
	            break;
	        case 2:
	            val1 = FAT2CPU16(((__u16*)mydata->fatbuf)[off16]);
	            val1 &= 0xff00;
	            val2 = FAT2CPU16(((__u16*)mydata->fatbuf)[off16+1]);
	            val2 &= 0x000f;
	            u32Result = (val2 << 8) | (val1 >> 8);
	            break;
	        case 3:
	            u32Result = FAT2CPU16(((__u16*)mydata->fatbuf)[off16]);;
	            u32Result = (u32Result & 0xfff0) >> 4;
	            break;
	        default:
	            break;
	        }
	    }
	    break;
	    }


		if(u32Result == 0x0) {  // found a free cluster
		       if(u32foundno == 0) {
			    ret = entry;
			    u32lastentry =entry;
		       }
			u32foundno ++;

		       if( entry - u32lastentry > 1 )
			   	break;
			//break;
			u32lastentry = entry;
			if(u32foundno >= *getno)
				break;
		}

		if(entry > mydata->fatlength * SECTOR_SIZE / 4) {// only support fat32
			FAT_DPRINT ("Reach the end\n");
			break;
		}
		entry++;

	}
    *getno = u32foundno;
	//printf("GOTfree_cluster:%d||", *getno);
    return ret;
}

/*
 * Clear the files cluster in FAT.
 */
int
clear_fatent(fsdata *mydata, __u32 entry)
{
    __u32 bufnum;
    __u32 offset;
    __u32 u32Result = 0x00;

	mydata->fatbufnum = -1;

	while(1) {

	    switch (mydata->fatsize) {
	    case 32:
	        bufnum = entry / FAT32BUFSIZE;
	        offset = entry - bufnum * FAT32BUFSIZE;
	        break;
	    case 16:
	        bufnum = entry / FAT16BUFSIZE;
	        offset = entry - bufnum * FAT16BUFSIZE;
	        break;
	    case 12:
	        bufnum = entry / FAT12BUFSIZE;
	        offset = entry - bufnum * FAT12BUFSIZE;
	        break;

	    default:
	        /* Unsupported FAT size */
	        return -1;
	    }
        printf("entry = %d\n",entry);

	    /* Read a new block of FAT entries into the cache. */
	    if (bufnum != mydata->fatbufnum) {
	        int getsize = FATBUFSIZE/FS_BLOCK_SIZE;
	        __u8 *bufptr = mydata->fatbuf;
	        __u32 fatlength = mydata->fatlength;
	        __u32 startblock = bufnum * FATBUFBLOCKS;

			if ( mydata->fatbufnum > -1) {
				if(disk_write(mydata->fatbufnum* FATBUFBLOCKS + mydata->fat_sect, getsize, bufptr) < 0) {
					FAT_DPRINT ("Error writing FAT blocks\n");
				    return -1;
				}
			}

	        fatlength *= SECTOR_SIZE;    /* We want it in bytes now */
	        startblock += mydata->fat_sect;    /* Offset from start of disk */

	        if (getsize > fatlength) getsize = fatlength;
	        if (disk_read(startblock, getsize, bufptr) < 0) {
	            FAT_DPRINT("Error reading FAT blocks\n");
	            return -1;
	        }
	        mydata->fatbufnum = bufnum;
	    }

	    /* Get the actual entry from the table */
	    switch (mydata->fatsize) {
	    case 32:
	        u32Result = FAT2CPU32(((__u32*)mydata->fatbuf)[offset]);
	        break;
	    case 16:
	        u32Result = FAT2CPU16(((__u16*)mydata->fatbuf)[offset]);
	        break;
	    case 12: {
	        __u32 off16 = (offset*3)/4;
	        __u16 val1, val2;

	        switch (offset & 0x3) {
	        case 0:
	            u32Result = FAT2CPU16(((__u16*)mydata->fatbuf)[off16]);
	            u32Result &= 0xfff;
	            break;
	        case 1:
	            val1 = FAT2CPU16(((__u16*)mydata->fatbuf)[off16]);
	            val1 &= 0xf000;
	            val2 = FAT2CPU16(((__u16*)mydata->fatbuf)[off16+1]);
	            val2 &= 0x00ff;
	            u32Result = (val2 << 4) | (val1 >> 12);
	            break;
	        case 2:
	            val1 = FAT2CPU16(((__u16*)mydata->fatbuf)[off16]);
	            val1 &= 0xff00;
	            val2 = FAT2CPU16(((__u16*)mydata->fatbuf)[off16+1]);
	            val2 &= 0x000f;
	            u32Result = (val2 << 8) | (val1 >> 8);
	            break;
	        case 3:
	            u32Result = FAT2CPU16(((__u16*)mydata->fatbuf)[off16]);;
	            u32Result = (u32Result & 0xfff0) >> 4;
	            break;
	        default:
	            break;
	        }
	    }
	    break;
	    }

		entry = u32Result;
		printf("offset = %d bufnum = %d\n",offset,bufnum);
		((__u32*)mydata->fatbuf)[offset] = 0;

		if(entry >= 0x0fffffff) {
			if(disk_write(mydata->fatbufnum* FATBUFBLOCKS + mydata->fat_sect, FATBUFSIZE/FS_BLOCK_SIZE, mydata->fatbuf) < 0) {
				FAT_DPRINT ("Error writing FAT blocks\n");
			    return -1;
			}
			break;
		}

		if(entry > mydata->fatlength * SECTOR_SIZE / 4) {// only support fat32
			FAT_DPRINT ("Reach the end\n");
			break;
		}

	    //FAT_DPRINT("ret: %d, offset: %d\n", ret, offset);
	}

    return 0;
}


long get_entloc(fsdata *mydata, __u32 entry, __u32 *bufnum, __u32 *offset)
{

	    switch (mydata->fatsize) {
	    case 32:
	        *bufnum = entry / FAT32BUFSIZE;
	        *offset = entry - *bufnum * FAT32BUFSIZE;
	        break;
	    case 16:
	        *bufnum = entry / FAT16BUFSIZE;
	        *offset = entry - *bufnum * FAT16BUFSIZE;
	        break;
	    case 12:
	        *bufnum = entry / FAT12BUFSIZE;
	        *offset = entry - *bufnum * FAT12BUFSIZE;
	        break;

	    default:
	        /* Unsupported FAT size */
	        return -1;
	    }
		return 0;

}
/*
 * put the entry at index 'entry' in a FAT (12/16/32) table.
 * On failure 0x00 is returned.
 */
static __u32
put_fatent(fsdata *mydata, __u32 entry, __u32 free, int num)
{
    __u32 bufnum;
    __u32 offset;
	//__u32 nextcluster = free;

    if(entry == free) {
	    if(get_entloc(mydata,entry, &bufnum, &offset) <0)
			return -1;
        //printf("entry = %d  bufnum= %d   offset = %d\n",entry,bufnum,offset);

		((__u32*)mydata->fatbuf)[offset] = 0xffffffff;
		if(disk_write(mydata->fatbufnum* FATBUFBLOCKS + mydata->fat_sect, FATBUFSIZE/FS_BLOCK_SIZE, mydata->fatbuf) < 0) {
			FAT_DPRINT ("Error writing FAT blocks\n");
		    return -1;
		}
		return 0;
    }

//old
    if(get_entloc(mydata,entry, &bufnum, &offset) <0)
		return -1;

	mydata->fatbufnum = bufnum;
	if(disk_read(bufnum * FATBUFBLOCKS + mydata->fat_sect, FATBUFSIZE/FS_BLOCK_SIZE, mydata->fatbuf) < 0) {
		FAT_DPRINT ("Error writing FAT blocks\n");
	    return -1;
	}

    while(1) {
	((__u32*)mydata->fatbuf)[offset] = free;
	    if(get_entloc(mydata,free, &bufnum, &offset) <0)
			return -1;
              if( mydata->fatbufnum != bufnum ) {
	if(disk_write(mydata->fatbufnum* FATBUFBLOCKS + mydata->fat_sect, FATBUFSIZE/FS_BLOCK_SIZE, mydata->fatbuf) < 0) {
		FAT_DPRINT ("Error writing FAT blocks\n");
	    return -1;
	}
	mydata->fatbufnum = bufnum;

	if(disk_read(bufnum * FATBUFBLOCKS + mydata->fat_sect, FATBUFSIZE/FS_BLOCK_SIZE, mydata->fatbuf) < 0) {
		FAT_DPRINT ("Error writing FAT blocks\n");
	    return -1;
	}
              }

	((__u32*)mydata->fatbuf)[offset] = 0xffffffff;
		entry++;
		free++;
		num--;
		if(num <= 0)
			break;

    	}
	if(disk_write(mydata->fatbufnum* FATBUFBLOCKS + mydata->fat_sect, FATBUFSIZE/FS_BLOCK_SIZE, mydata->fatbuf) < 0) {
		FAT_DPRINT ("Error writing FAT blocks\n");
	    return -1;
	}


#if 0
	while(1) {
	    if(get_entloc(mydata,entry, &bufnum, &offset) <0)
			return -1;
		((__u32*)mydata->fatbuf)[offset] = nextcluster;

	    // Read a new block of FAT entries into the cache.
	    if (bufnum != mydata->fatbufnum) {
	        int getsize = FATBUFSIZE/FS_BLOCK_SIZE;
	        __u8 *bufptr = mydata->fatbuf;
	        __u32 fatlength = mydata->fatlength;
	        __u32 startblock = bufnum * FATBUFBLOCKS;

			if ( mydata->fatbufnum > -1) {
				if(disk_write(mydata->fatbufnum* FATBUFBLOCKS + mydata->fat_sect, getsize, bufptr) < 0) {
					FAT_DPRINT ("Error writing FAT blocks\n");
				    return -1;
				}
			}

	        fatlength *= SECTOR_SIZE;    /* We want it in bytes now */
	        startblock += mydata->fat_sect;    /* Offset from start of disk */

	        if (getsize > fatlength) getsize = fatlength;
	        if (disk_read(startblock, getsize, bufptr) < 0) {
	            FAT_DPRINT("Error reading FAT blocks\n");
	            return -1;
	        }
	        mydata->fatbufnum = bufnum;
	    }


		entry = nextcluster;
		nextcluster++;
		num--;

		if(num <= 0) {

		    if(get_entloc(mydata,entry, &bufnum, &offset) <0)
				return -1;

			((__u32*)mydata->fatbuf)[offset] = 0xffffffff;

			if(disk_write(mydata->fatbufnum* FATBUFBLOCKS + mydata->fat_sect, FATBUFSIZE/FS_BLOCK_SIZE, mydata->fatbuf) < 0) {
				FAT_DPRINT ("Error writing FAT blocks\n");
			    return -1;
			}

			break;
		}

		if(entry > mydata->fatlength * SECTOR_SIZE / 4) {// only support fat32
			FAT_DPRINT ("Reach the end\n");
			break;
		}

	}
	#endif

    return 0;
}

/*
 * Get the entry at index 'entry' in a FAT (12/16/32) table.
 * On failure 0x00 is returned.
 */
static __u32
get_fatent(fsdata *mydata, __u32 entry)
{
    __u32 bufnum;
    __u32 offset;
    __u32 ret = 0x00;

    switch (mydata->fatsize) {
    case 32:
        bufnum = entry / FAT32BUFSIZE;
        offset = entry - bufnum * FAT32BUFSIZE;
        break;
    case 16:
        bufnum = entry / FAT16BUFSIZE;
        offset = entry - bufnum * FAT16BUFSIZE;
        break;
    case 12:
        bufnum = entry / FAT12BUFSIZE;
        offset = entry - bufnum * FAT12BUFSIZE;
        break;

    default:
        /* Unsupported FAT size */
        return ret;
    }



    /* Read a new block of FAT entries into the cache. */
    if (bufnum != mydata->fatbufnum) {
        int getsize = FATBUFSIZE/FS_BLOCK_SIZE;
        __u8 *bufptr = mydata->fatbuf;
        __u32 fatlength = mydata->fatlength;
        __u32 startblock = bufnum * FATBUFBLOCKS;

        fatlength *= SECTOR_SIZE;    /* We want it in bytes now */
        startblock += mydata->fat_sect;    /* Offset from start of disk */

        if (getsize > fatlength) getsize = fatlength;
        if (disk_read(startblock, getsize, bufptr) < 0) {
            FAT_DPRINT("Error reading FAT blocks\n");
            return ret;
        }
        mydata->fatbufnum = bufnum;
    }

    /* Get the actual entry from the table */
    switch (mydata->fatsize) {
    case 32:
        ret = FAT2CPU32(((__u32*)mydata->fatbuf)[offset]);
        break;
    case 16:
        ret = FAT2CPU16(((__u16*)mydata->fatbuf)[offset]);
        break;
    case 12: {
        __u32 off16 = (offset*3)/4;
        __u16 val1, val2;

        switch (offset & 0x3) {
        case 0:
            ret = FAT2CPU16(((__u16*)mydata->fatbuf)[off16]);
            ret &= 0xfff;
            break;
        case 1:
            val1 = FAT2CPU16(((__u16*)mydata->fatbuf)[off16]);
            val1 &= 0xf000;
            val2 = FAT2CPU16(((__u16*)mydata->fatbuf)[off16+1]);
            val2 &= 0x00ff;
            ret = (val2 << 4) | (val1 >> 12);
            break;
        case 2:
            val1 = FAT2CPU16(((__u16*)mydata->fatbuf)[off16]);
            val1 &= 0xff00;
            val2 = FAT2CPU16(((__u16*)mydata->fatbuf)[off16+1]);
            val2 &= 0x000f;
            ret = (val2 << 8) | (val1 >> 8);
            break;
        case 3:
            ret = FAT2CPU16(((__u16*)mydata->fatbuf)[off16]);;
            ret = (ret & 0xfff0) >> 4;
            break;
        default:
            break;
        }
    }
    break;
    }
    FAT_DPRINT("ret: %d, offset: %d\n", ret, offset);

    return ret;
}


/*
 * Read at most 'size' bytes from the specified cluster into 'buffer'.
 * Return 0 on success, -1 otherwise.
 */
static int
get_cluster(fsdata *mydata, __u32 clustnum, __u8 *buffer, unsigned long size)
{
    int idx = 0;
    __u32 startsect;

    if (clustnum > 0) {
        startsect = mydata->data_begin + clustnum*mydata->clust_size;
    } else {
        startsect = mydata->rootdir_sect;
    }
    /*show this in the windows add by yanlong.zhang 2009/08/14*/
    FAT_DPRINT("gc - clustnum: %d, startsect: %d\n", clustnum, startsect);

    if (disk_read(startsect, size/FS_BLOCK_SIZE , buffer) < 0) {
        FAT_DPRINT("Error reading data\n");
        return -1;
    }
    if(size % FS_BLOCK_SIZE) {
        __u8 tmpbuf[MAX_SECTOR_SIZE]; // TODO: review, stack size
        idx= size/FS_BLOCK_SIZE;
        if (disk_read(startsect + idx, 1, tmpbuf) < 0) {
            FAT_DPRINT("Error reading data\n");
            return -1;
        }
        buffer += idx*FS_BLOCK_SIZE;

        memcpy(buffer, tmpbuf, size % FS_BLOCK_SIZE);
        return 0;
    }

    return 0;
}


/*
 * Read at most 'maxsize' bytes from the file associated with 'dentptr'
 * into 'buffer'.
 * Return the number of bytes read or -1 on fatal errors.
 */
static long
get_contents(fsdata *mydata, dir_entry *dentptr, __u8 *buffer,
         unsigned long maxsize)
{
    unsigned long filesize = FAT2CPU32(dentptr->size), gotsize = 0;
    unsigned int bytesperclust = mydata->clust_size * SECTOR_SIZE;
    __u32 curclust = START(dentptr);
    __u32 endclust, newclust;
    unsigned long actsize;

    FAT_DPRINT("Filesize: %ld bytes\n", filesize);

    if (maxsize > 0 && filesize > maxsize) filesize = maxsize;

    FAT_DPRINT("Reading: %ld bytes\n", filesize);

    actsize=bytesperclust;
    endclust=curclust;
    do {
        /* search for consecutive clusters */
        while(actsize < filesize) {
            newclust = get_fatent(mydata, endclust);
            if((newclust -1)!=endclust)
                goto getit;
            if (CHECK_CLUST(newclust, mydata->fatsize)) {
                FAT_DPRINT("curclust: 0x%x\n", newclust);
                FAT_DPRINT("Invalid FAT entry\n");
                return gotsize;
            }
            endclust=newclust;
            actsize+= bytesperclust;
        }
        /* actsize >= file size */
        actsize -= bytesperclust;
        /* get remaining clusters */
        if (get_cluster(mydata, curclust, buffer, (int)actsize) != 0) {
            FAT_ERROR("Error reading cluster\n");
            return -1;
        }
        /* get remaining bytes */
        gotsize += (int)actsize;
        filesize -= actsize;
        buffer += actsize;
        actsize= filesize;
        if (get_cluster(mydata, endclust, buffer, (int)actsize) != 0) {
            FAT_ERROR("Error reading cluster\n");
            return -1;
        }
        gotsize+=actsize;
        return gotsize;
getit:
        if (get_cluster(mydata, curclust, buffer, (int)actsize) != 0) {
            FAT_ERROR("Error reading cluster\n");
            return -1;
        }
        gotsize += (int)actsize;
        filesize -= actsize;
        buffer += actsize;
        curclust = get_fatent(mydata, endclust);
        if (CHECK_CLUST(curclust, mydata->fatsize)) {
            FAT_DPRINT("curclust: 0x%x\n", curclust);
            FAT_ERROR("Invalid FAT entry\n");
            return gotsize;
        }
        actsize=bytesperclust;
        endclust=curclust;
    } while (1);
}


#ifdef CONFIG_SUPPORT_VFAT
/*
 * Extract the file name information from 'slotptr' into 'l_name',
 * starting at l_name[*idx].
 * Return 1 if terminator (zero byte) is found, 0 otherwise.
 */
static int
slot2str(dir_slot *slotptr, char *l_name, int *idx)
{
    int j;

    for (j = 0; j <= 8; j += 2) {
        l_name[*idx] = slotptr->name0_4[j];
        if (l_name[*idx] == 0x00) return 1;
        (*idx)++;
    }
    for (j = 0; j <= 10; j += 2) {
        l_name[*idx] = slotptr->name5_10[j];
        if (l_name[*idx] == 0x00) return 1;
        (*idx)++;
    }
    for (j = 0; j <= 2; j += 2) {
        l_name[*idx] = slotptr->name11_12[j];
        if (l_name[*idx] == 0x00) return 1;
        (*idx)++;
    }

    return 0;
}


/*
 * Extract the full long filename starting at 'retdent' (which is really
 * a slot) into 'l_name'. If successful also copy the real directory entry
 * into 'retdent'
 * Return 0 on success, -1 otherwise.
 */
__attribute__ ((__aligned__(__alignof__(dir_entry))))
__u8     get_vfatname_block[MAX_CLUSTSIZE];
static int
get_vfatname(fsdata *mydata, int curclust, __u8 *cluster,
         dir_entry *retdent, char *l_name)
{
    dir_entry *realdent;
    dir_slot  *slotptr = (dir_slot*) retdent;
    __u8      *nextclust = cluster + mydata->clust_size * SECTOR_SIZE;
    __u8       counter = (slotptr->id & ~LAST_LONG_ENTRY_MASK) & 0xff;
    int idx = 0;

    while ((__u8*)slotptr < nextclust) {
        if (counter == 0) break;
        if (((slotptr->id & ~LAST_LONG_ENTRY_MASK) & 0xff) != counter)
            return -1;
        slotptr++;
        counter--;
    }

    if ((__u8*)slotptr >= nextclust) {
        dir_slot *slotptr2;

#if 0
        slotptr--; // this "--" is not necessary, it exists below while loop
#endif
        curclust = get_fatent(mydata, curclust);
        if (CHECK_CLUST(curclust, mydata->fatsize)) {
            FAT_DPRINT("curclust: 0x%x\n", curclust);
            FAT_ERROR("Invalid FAT entry\n");
            return -1;
        }
        if (get_cluster(mydata, curclust, get_vfatname_block,
                mydata->clust_size * SECTOR_SIZE) != 0) {
            FAT_DPRINT("Error: reading directory block\n");
            return -1;
        }
        slotptr2 = (dir_slot*) get_vfatname_block;
#if 0
        while (slotptr2->id > 0x01) {
#else
        while (slotptr2->id > 0x01 && counter > 0) { // "counter == 0" if the first slot of the next cluster is the real directory entry
#endif
            slotptr2++;
        }
        /* Save the real directory entry */
        if (counter == 0)
        {
            realdent = (dir_entry*)slotptr2;
            slotptr2--;
        }
        else
        {
        realdent = (dir_entry*)slotptr2 + 1;
        }
        while ((__u8*)slotptr2 >= get_vfatname_block) {
            slot2str(slotptr2, l_name, &idx);
            slotptr2--;
        }
    } else {
        /* Save the real directory entry */
        realdent = (dir_entry*)slotptr;
    }

    do {
        slotptr--;
        if (slot2str(slotptr, l_name, &idx)) break;
    } while (!(slotptr->id & LAST_LONG_ENTRY_MASK));

    l_name[idx] = '\0';
    if (*l_name == DELETED_FLAG) *l_name = '\0';
    else if (*l_name == aRING) *l_name = DELETED_FLAG;
    downcase(l_name);

    /* Return the real directory entry */
    memcpy(retdent, realdent, sizeof(dir_entry));

    return 0;
}


/* Calculate short name checksum */
static __u8
mkcksum(const char *str)
{
    int i;
    __u8 ret = 0;

    for (i = 0; i < 11; i++) {
        ret = (((ret&1)<<7)|((ret&0xfe)>>1)) + str[i];
    }

    return ret;
}
#endif

extern U32 KL_Size;
/*
 * Get the directory entry associated with 'filename' from the directory
 * starting at 'startsect'
 */
__attribute__ ((__aligned__(__alignof__(dir_entry))))
__u8 get_dentfromdir_block[MAX_CLUSTSIZE];
static dir_entry *get_dentfromdir (fsdata * mydata, int startsect,
                   char *filename, dir_entry * retdent,
                   int dols)
{
    __u16 prevcksum = 0xffff;
    __u32 curclust = START (retdent);
    int files = 0, dirs = 0;
    /*show in the windows*/
    FAT_DPRINT ("get_dentfromdir: %s\n", filename);

    while (1) {
    dir_entry *dentptr;
    int i;
    if (get_cluster (mydata, curclust, get_dentfromdir_block,
         mydata->clust_size * SECTOR_SIZE) != 0) {
        FAT_DPRINT ("Error: reading directory block\n");
        return NULL;
    }
    dentptr = (dir_entry *) get_dentfromdir_block;

    for (i = 0; i < DIRENTSPERCLUST; i++) {

        char s_name[14], l_name[256];

        l_name[0] = '\0';
        if (dentptr->name[0] == DELETED_FLAG) {
            dentptr++;
            continue;
        }
        if ((dentptr->attr & ATTR_VOLUME)) {
#ifdef CONFIG_SUPPORT_VFAT
        if ((dentptr->attr & ATTR_VFAT) &&
            (dentptr->name[0] & LAST_LONG_ENTRY_MASK))
         {
            prevcksum = ((dir_slot *) dentptr)->alias_checksum;

            get_vfatname (mydata, curclust, get_dentfromdir_block,dentptr, l_name);
            if (dols) {
            int isdir = (dentptr->attr & ATTR_DIR);
            char dirc;
            int doit = 0;

            if (isdir) {
                dirs++;
                dirc = '/';
                doit = 1;
            } else {
                dirc = ' ';
                if (l_name[0] != 0) {
                files++;
                doit = 1;
                }
            }
            if (doit) {
                if (dirc == ' ')
                {
#if (ENABLE_MODULE_SECURITY_BOOT)
                    if(strcmp(l_name, "uimage.aes") == 0)
                    {
                        KL_Size = (long) FAT2CPU32 (dentptr->size);
                        //printf("@@ %ld\n", KL_Size);
                    }
#endif
                    printf (" %8ld   %s%c\n",(long) FAT2CPU32 (dentptr->size),l_name, dirc);//get long file name
                    validate_image_ready(l_name);
                    printf("print the image's number jffs2Num: %d\n", jffs2Num);
                    validate_script_ready(l_name);
                    printf("print the script's number lookup: %d\n",lookup);

                }
                else
                {
                    printf ("            %s%c\n", l_name, dirc);
                }
            }
            dentptr++;
            continue;
            }
            FAT_DPRINT ("vfatname: |%s|\n", l_name);
        }
        else
#endif
        {
            /* Volume label or VFAT entry */
            dentptr++;
            continue;
        }
        }


        if (dentptr->name[0] == 0) {
        if (dols) {
             printf ("\n%d file(s), %d dir(s)\n\n", files, dirs);//print sub_directory number of file and folderd
             fileNum= files;
        }
        FAT_DPRINT ("Dentname == NULL - %d\n", i);
        return NULL;
        }
#ifdef CONFIG_SUPPORT_VFAT
        if (dols && mkcksum (dentptr->name) == prevcksum) {
        dentptr++;
        continue;
        }
#endif

        get_name (dentptr, s_name);
        if (dols) {
        int isdir = (dentptr->attr & ATTR_DIR);
        char dirc;
        int doit = 0;

        if (isdir) {
            dirs++;
            dirc = '/';
            doit = 1;
        } else {
            dirc = ' ';
            if (s_name[0] != 0) {
            files++;
            doit = 1;
            }
        }
        if (doit) {
            if (dirc == ' ') {
            printf (" %8ld   %s%c\n",(long) FAT2CPU32 (dentptr->size), s_name,dirc);//s_name is little 8 byte, that is small filename
            validate_image_ready(s_name);
            printf("print the image's number jffs2Num_2: %d\n",jffs2Num);
            } else {
            printf ("            %s%c\n", s_name, dirc);
            }
        }
        dentptr++;
        continue;
        }
        if (strcmp (filename, s_name) && strcmp (filename, l_name)) {
        FAT_DPRINT ("Mismatch: |%s|%s|\n", s_name, l_name);
        dentptr++;
        continue;
        }
        memcpy (retdent, dentptr, sizeof (dir_entry));
        FAT_DPRINT ("DentName: %s", s_name);
        FAT_DPRINT (", start: 0x%x", START (dentptr));
        FAT_DPRINT (", size:  0x%x %s\n",FAT2CPU32 (dentptr->size),(dentptr->attr & ATTR_DIR) ? "(DIR)" : "");

        return retdent;
    }
    curclust = get_fatent (mydata, curclust);
    if (CHECK_CLUST(curclust, mydata->fatsize)) {
        FAT_DPRINT ("curclust: 0x%x\n", curclust);
        FAT_ERROR ("Invalid FAT entry\n");
        return NULL;
    }
    }

    return NULL;
}


/*
 * Read boot sector and volume info from a FAT filesystem
 */
static int
read_bootsectandvi(boot_sector *bs, volume_info *volinfo, int *fatsize)
{
    __u8 block[MAX_SECTOR_SIZE];
    volume_info *vistart;

    if (disk_read(0, 1, block) < 0) {
        FAT_DPRINT("Error: reading block\n");
        return -1;
    }

    memcpy(bs, block, sizeof(boot_sector));
    bs->reserved    = FAT2CPU16(bs->reserved);
    bs->fat_length    = FAT2CPU16(bs->fat_length);
    bs->secs_track    = FAT2CPU16(bs->secs_track);
    bs->heads    = FAT2CPU16(bs->heads);
#if 0 /* UNUSED */
    bs->hidden    = FAT2CPU32(bs->hidden);
#endif
    bs->total_sect    = FAT2CPU32(bs->total_sect);

    /* FAT32 entries */
    if (bs->fat_length == 0) {
        /* Assume FAT32 */
        bs->fat32_length = FAT2CPU32(bs->fat32_length);
        bs->flags     = FAT2CPU16(bs->flags);
        bs->root_cluster = FAT2CPU32(bs->root_cluster);
        bs->info_sector  = FAT2CPU16(bs->info_sector);
        bs->backup_boot  = FAT2CPU16(bs->backup_boot);
        vistart = (volume_info*) (block + sizeof(boot_sector));
        *fatsize = 32;
    } else {
        vistart = (volume_info*) &(bs->fat32_length);
        *fatsize = 0;
    }
    memcpy(volinfo, vistart, sizeof(volume_info));

    if (*fatsize == 32) {
        if (strncmp(FAT32_SIGN, vistart->fs_type, SIGNLEN) == 0) {
            return 0;
        }
    } else {
        if (strncmp(FAT12_SIGN, vistart->fs_type, SIGNLEN) == 0) {
            *fatsize = 12;
            return 0;
        }
        if (strncmp(FAT16_SIGN, vistart->fs_type, SIGNLEN) == 0) {
            *fatsize = 16;
            return 0;
        }
    }

    FAT_DPRINT("Error: broken fs_type sign\n");
    return -1;
}

__attribute__ ((__aligned__(__alignof__(dir_entry))))
__u8 do_fat_write_block[MAX_CLUSTSIZE];

__attribute__ ((__aligned__(__alignof__(dir_entry))))
__u8 do_fat_write_sector[MAX_SECTOR_SIZE * 2];

long do_fat_write (const char *filename, void *buffer, unsigned long maxsize,
         int dols)
{
    int fd=file_fat_open(filename, "w");

    if(fd < 0) {
    	FAT_DPRINT ("Error: file open failed\n");
    	return -1;
    }

    if(file_fat_write(fd, buffer, maxsize)< 0) {
    	FAT_DPRINT ("Error: file write failed\n");
        return -1;
    }

    file_fat_close(fd);

    return 0;

}


__attribute__ ((__aligned__(__alignof__(dir_entry))))
__u8 do_fat_read_block[MAX_CLUSTSIZE];
long
do_fat_read (const char *filename, void *buffer, unsigned long maxsize,
         int dols)
{
#if 1 // CONFIG_NIOS /* NIOS CPU cannot access big automatic arrays */
    static
#endif
    char fnamecopy[2048];
    boot_sector bs;
    volume_info volinfo;
    fsdata datablock;
    fsdata *mydata = &datablock;
    dir_entry *dentptr;
    __u16 prevcksum = 0xffff;
    char *subname = "";
    int rootdir_size, cursect;
    int idx, isdir = 0;
    int files = 0, dirs = 0;
    long ret = 0;
    int firsttime;
    __u32 cur_cluster;

    /*read fat boot sector and volume info from filesystem */
    if (read_bootsectandvi (&bs, &volinfo, &mydata->fatsize)) {
    FAT_DPRINT ("Error: reading boot sector\n");
    return -1;
    }

    /*affirm fat format is 32 or 16*/
    if (mydata->fatsize == 32) {
    mydata->fatlength = bs.fat32_length;
    } else {
    mydata->fatlength = bs.fat_length;
    }

    mydata->fat_sect = bs.reserved;
    cursect = mydata->rootdir_sect
        = mydata->fat_sect + mydata->fatlength * bs.fats;
    mydata->clust_size = bs.cluster_size;
    cur_cluster = ROOT_CLUSTER;
    /*fat format is 32*/
    if (mydata->fatsize == 32) {
    rootdir_size = mydata->clust_size;
    mydata->data_begin = mydata->rootdir_sect   /* + rootdir_size */
        - (mydata->clust_size * 2);
    }
    else {
    rootdir_size = ((bs.dir_entries[1] * (int) 256 + bs.dir_entries[0])
            * sizeof (dir_entry)) / SECTOR_SIZE;
    mydata->data_begin = mydata->rootdir_sect + rootdir_size
        - (mydata->clust_size * 2);
    }/*fat format is other,for example: 16*/

    mydata->fatbufnum = -1;
    /*FAT 32 fatlength:998 ----->print fat format and disk storage*/
    FAT_DPRINT ("FAT%d, fatlength: %d\n", mydata->fatsize,
        mydata->fatlength);
    FAT_DPRINT ("Rootdir begins at sector: %d, offset: %x, size: %d\n"
        "Data begins at: %d\n",
        mydata->rootdir_sect, mydata->rootdir_sect * SECTOR_SIZE,
        rootdir_size, mydata->data_begin);
    FAT_DPRINT ("Cluster size: %d\n", mydata->clust_size);
    /*before read print some info by yanlong.zhang */

    /* "cwd" is always the root... */
    while (ISDIRDELIM (*filename))
    filename++;
    /* Make a copy of the filename and convert it to lowercase */
    strcpy (fnamecopy, filename);
    downcase (fnamecopy);// convert it to lowercase

    if (*fnamecopy == '\0') {
    if (!dols)
        return -1;
    dols = LS_ROOT;
    }
    else if ((idx = dirdelim (fnamecopy)) >= 0) {
    isdir = 1;
    fnamecopy[idx] = '\0';
    subname = fnamecopy + idx + 1;
    /* Handle multiple delimiters */
    while (ISDIRDELIM (*subname))
        subname++;
    }
    else if (dols) {
    isdir = 1;
    }

    while (1) {
    int i;
    if (disk_read (cursect, mydata->clust_size, do_fat_read_block) < 0) {
        FAT_DPRINT ("Error: reading rootdir block\n");
        return -1;
    }
    dentptr = (dir_entry *) do_fat_read_block;/*data is in da_fat_read_block*/

#if 0
        for (i = 0; i < DIRENTSPERBLOCK; i++)
#else
        for (i = 0; i < DIRENTSPERBLOCK * (mydata->clust_size); i++)
#endif
        {
        char s_name[14], l_name[256];

        l_name[0] = '\0';

        if((u8)dentptr->name[0]== 0xE5)
        {
            //skip the free dentry slot
            dentptr++;
            continue;
        }


        if ((dentptr->attr & ATTR_VOLUME)) {
#ifdef CONFIG_SUPPORT_VFAT
        if ((dentptr->attr & ATTR_VFAT) &&
            (dentptr->name[0] & LAST_LONG_ENTRY_MASK)) {
            prevcksum = ((dir_slot *) dentptr)->alias_checksum;
#if 0
            get_vfatname (mydata, 0, do_fat_read_block, dentptr, l_name);
#else

            get_vfatname (mydata, cur_cluster, do_fat_read_block, dentptr, l_name);
#endif

            if (dols == LS_ROOT) {
            int isdir = (dentptr->attr & ATTR_DIR);
            char dirc;
            int doit = 0;

            if (isdir) {
                dirs++;
                dirc = '/';
                doit = 1;
            } else {
                dirc = ' ';
                if (l_name[0] != 0) {
                files++;
                doit = 1;
                }
            }
            if (doit)
            {
                if (dirc == ' ') {
                printf (" %8ld   %s%c\n",
                    (long) FAT2CPU32 (dentptr->size),
                    l_name, dirc);
                } else {
                printf ("            %s%c\n", l_name, dirc);
                }
            }
            dentptr++;
            continue;
            }
            FAT_DPRINT ("Rootvfatname: |%s|\n", l_name);
        }
        else
#endif
        {
            /* Volume label or VFAT entry */
            dentptr++;
            continue;
        }

        }/*   */

      /*this segment is not run*/
        else if (dentptr->name[0] == 0)
        {
        FAT_DPRINT ("RootDentname == NULL - %d\n", i);
        if (dols == LS_ROOT) {
            printf ("\n%d file(s), %d dir(s)\n\n", files, dirs);//root dirctory print number of file and folder
            return 0;
        }
        return -1;
        }


#ifdef CONFIG_SUPPORT_VFAT
        else if (dols == LS_ROOT
             && mkcksum (dentptr->name) == prevcksum)
        {
        dentptr++;
        continue;
        }
#endif
        get_name (dentptr, s_name);
#if defined(CONFIG_KRONUS)||defined(CONFIG_URANUS4) || defined(CONFIG_KAISERIN) ||defined(CONFIG_KELTIC) || defined(CONFIG_KAPPA) ||defined(CONFIG_KERES)// scan for update file entry
        int ret = 0;
        char buffercmp[50];
        int compare = 0;
        //printf("ret = %d    %s  %s  %x %x ; %x  %x\n",ret,dentptr->name,s_name,dentptr->cdate,dentptr->ctime,dentptr->date,dentptr->time);
        if(!strcmp(s_name,"flash.bin"))
        {
            //printf("Find Update file \n");
            if ((getenv("LUpgradeDate")== NULL) || (simple_strtol(getenv("USBUpdateFlag"), NULL, 16)==1) )
            {
                //printf("First time Update \n");
                memset(buffercmp,0,50);
                sprintf(buffercmp,"setenv LUpgradeDate %x",dentptr->date);
                ret = run_command(buffercmp, 0);

                memset(buffercmp,0,50);
                sprintf(buffercmp,"setenv LUpgradeTime %x",dentptr->time);
                ret = run_command(buffercmp, 0);

                compare = 1;
                sprintf(buffercmp,"setenv USBUpdateFlag %d",compare);
                ret = run_command(buffercmp, 0);


            }
            else
            {

                compare = simple_strtol(getenv("LUpgradeDate"), NULL, 16);
                //printf("compare %x  %x \n",compare,dentptr->date);
                if(compare > dentptr->date)
                {

                }
                else
                {
                    if(compare < dentptr->date)ret = 1;
                    else ret = 0;
                    compare = simple_strtol(getenv("LUpgradeTime"), NULL, 16);
                    //printf("compare %x  %x \n",compare,dentptr->time);
                    if(compare >= dentptr->time && ret == 0)
                    {

                    }
                    else
                    {

                        memset(buffercmp,0,50);
                        sprintf(buffercmp,"setenv LUpgradeDate %x",dentptr->date);
                        ret = run_command(buffercmp, 0);

                        memset(buffercmp,0,50);
                        sprintf(buffercmp,"setenv LUpgradeTime %x",dentptr->time);
                        ret = run_command(buffercmp, 0);

                        memset(buffercmp,0,50);
                        compare = 1;
                        sprintf(buffercmp,"setenv USBUpdateFlag %d",compare);
                        ret = run_command(buffercmp, 0);


                    }
                }
            }


        }

#endif
        if (dols == LS_ROOT) {
        int isdir = (dentptr->attr & ATTR_DIR);
        char dirc;
        int doit = 0;

        if (isdir) {
            dirc = '/';
            if (s_name[0] != 0) {
            dirs++;
            doit = 1;
            }
        }
        else {
            dirc = ' ';
            if (s_name[0] != 0) {
            files++;
            doit = 1;
            }
        }
        if (doit) {
            if (dirc == ' ') {
            printf (" %8ld   %s%c\n",(long) FAT2CPU32 (dentptr->size), s_name,dirc);
            }
            else {
            printf ("            %s%c\n", s_name, dirc);
            }
        }
        dentptr++;
        continue;
        }

        if (strcmp (fnamecopy, s_name) && strcmp (fnamecopy, l_name)) {
        FAT_DPRINT ("RootMismatch: |%s|%s|\n", s_name, l_name);
        dentptr++;
        continue;
        }
        if (isdir && !(dentptr->attr & ATTR_DIR))
        return -1;
        FAT_DPRINT ("RootName: %s", s_name);/*root dirterory*/
        FAT_DPRINT (", start: 0x%x", START (dentptr));
        FAT_DPRINT (", size:  0x%x %s\n", FAT2CPU32 (dentptr->size), isdir ? "(DIR)" : "");

        goto rootdir_done;  /* We got a match */
    }
#if 0
    cursect++;
#else
        cur_cluster = get_fatent (mydata, cur_cluster);

        if (CHECK_CLUST(cur_cluster, mydata->fatsize))
        {
            FAT_DPRINT ("RootDentname == NULL - %d\n", i);
            if (dols == LS_ROOT)
            {
                printf ("\n%d file(s), %d dir(s)\n\n", files, dirs);
                return 0;
            }
            return -1;
        }
        else
        {
            cursect = mydata->fat_sect + mydata->fatlength * bs.fats + (cur_cluster - ROOT_CLUSTER) * mydata->clust_size;
        }
#endif
    }

  rootdir_done:
    firsttime = 1;
    while (isdir) {
    int startsect = mydata->data_begin
        + START (dentptr) * mydata->clust_size;
    dir_entry dent;
    char *nextname = NULL;

    dent = *dentptr;
    dentptr = &dent;

    idx = dirdelim (subname);
    if (idx >= 0) {
        subname[idx] = '\0';
        nextname = subname + idx + 1;
        /* Handle multiple delimiters */
        while (ISDIRDELIM (*nextname))
        nextname++;
        if (dols && *nextname == '\0')
        firsttime = 0;
    }
    else {
        if (dols && firsttime) {
        firsttime = 0;
        }
        else {
        isdir = 0;
        }
    }
    /*DentName: ., start: 0x4, size:  0x0 (DIR)*/
    if (get_dentfromdir (mydata, startsect, subname, dentptr,
                 isdir ? 0 : dols) == NULL) {

        if (dols && !isdir)
        return 0;
        return -1;
    }

    if (idx >= 0) {
        if (!(dentptr->attr & ATTR_DIR))
        return -1;
        subname = nextname;
    }
    }
    ret = get_contents (mydata, dentptr, buffer, maxsize);
    FAT_DPRINT ("Size: %d, got: %ld\n", FAT2CPU32 (dentptr->size), ret);
    return ret;
}


int
file_fat_detectfs(void)
{
    boot_sector    bs;
    volume_info    volinfo;
    int        fatsize;
    char    vol_label[12];

    if(cur_dev==NULL) {
        printf("No current device\n");
        return 1;
    }
#if (CONFIG_COMMANDS & CFG_CMD_IDE) || (CONFIG_COMMANDS & CFG_CMD_SCSI) || \
    (CONFIG_COMMANDS & CFG_CMD_USB) || (CONFIG_MMC)
    printf("Interface:  ");
    switch(cur_dev->if_type) {
        case IF_TYPE_IDE :      printf("IDE"); break;
        case IF_TYPE_SCSI :     printf("SCSI"); break;
        case IF_TYPE_ATAPI :    printf("ATAPI"); break;
        case IF_TYPE_USB :      printf("USB"); break;
        case IF_TYPE_DOC :      printf("DOC"); break;
        case IF_TYPE_MMC :      printf("MMC"); break;
        default :               printf("Unknown");
    }
    printf("\n  Device %d: ",cur_dev->dev);
    dev_print(cur_dev);
#endif
    if(read_bootsectandvi(&bs, &volinfo, &fatsize)) {
        printf("\nNo valid FAT fs found\n");
        return 1;
    }
    memcpy (vol_label, volinfo.volume_label, 11);
    vol_label[11] = '\0';
    volinfo.fs_type[5]='\0';
    printf("Partition %d: Filesystem: %s \"%s\"\n",cur_part,volinfo.fs_type,vol_label);
    return 0;
}


int
file_fat_ls(const char *dir)
{
    return do_fat_read(dir, NULL, 0, LS_YES);
}


long
file_fat_read(const char *filename, void *buffer, unsigned long maxsize)
{
    printf("reading %s\n",filename);
    return do_fat_read(filename, buffer, maxsize, LS_NO);
}

long
file_fat_writefile(const char *filename, void *buffer, unsigned long maxsize)
{
    printf("writing %s\n",filename);
    return do_fat_write(filename, buffer, maxsize, LS_NO);
}



__u8 do_fat_cache_sector[MAX_CLUSTSIZE];

//Supported mode:
//w
//a
//
//
struct filedescipter gfd[MAXFILEBUFFER];
static int iNumOfFile=0;


long file_fat_write(int fd, void *buffer, unsigned long maxsize)
{
    boot_sector bs;
    volume_info volinfo;
    fsdata datablock;
    fsdata *mydata = &datablock;
    dir_entry *dentptr;

    __u32 cur_cluster;
    __u32 start_cluster;
    __u32 old_cluster;
    __u32 fileoffset = 0;
    long writesize = 0;
    int sharpnum = 1;
    int getno = 1;

    int rootdir_size, cursect;

    if(fd<0 || fd>15) {
		printf("Invaild file num\n");
		return -1;
    }

    if(gfd[fd].fd == 0)
    	return -1;

    /*read fat boot sector and volume info from filesystem */
    if (read_bootsectandvi (&bs, &volinfo, &mydata->fatsize)) {
	    FAT_DPRINT ("Error: reading boot sector\n");
	    return -1;
    }

    /*affirm fat format is 32 or 16*/
    if (mydata->fatsize == 32) {
        mydata->fatlength = bs.fat32_length;
    } else {
        mydata->fatlength = bs.fat_length;
    }

    mydata->fat_sect = bs.reserved;
    cursect = mydata->rootdir_sect
        = mydata->fat_sect + mydata->fatlength * bs.fats;
    mydata->clust_size = bs.cluster_size;
    cur_cluster = ROOT_CLUSTER;
    /*fat format is 32*/
    if (mydata->fatsize == 32) {
    rootdir_size = mydata->clust_size;
    mydata->data_begin = mydata->rootdir_sect   /* + rootdir_size */
        - (mydata->clust_size * 2);
    }
    else {
    rootdir_size = ((bs.dir_entries[1] * (int) 256 + bs.dir_entries[0])
            * sizeof (dir_entry)) / SECTOR_SIZE;
    mydata->data_begin = mydata->rootdir_sect + rootdir_size
        - (mydata->clust_size * 2);
    }/*fat format is other,for example: 16*/

    mydata->fatbufnum = -1;
    /*FAT 32 fatlength:998 ----->print fat format and disk storage*/
    FAT_DPRINT ("FAT%d, fatlength: %d\n", mydata->fatsize,
        mydata->fatlength);
    FAT_DPRINT ("Rootdir begins at sector: %d, offset: %x, size: %d\n"
        "Data begins at: %d\n",
        mydata->rootdir_sect, mydata->rootdir_sect * SECTOR_SIZE,
        rootdir_size, mydata->data_begin);
    FAT_DPRINT ("Cluster size: %d\n", mydata->clust_size);

    cur_cluster = gfd[fd].ent_cluser;

    cursect = mydata->fat_sect + mydata->fatlength * bs.fats
                   + (cur_cluster - ROOT_CLUSTER) * mydata->clust_size;

    printf("fd:%d, cur_cluster:%d, cursect:%d, offset:%d\n",fd, cur_cluster, cursect, gfd[fd].ent_offset);
    if (disk_read (cursect, mydata->clust_size, do_fat_read_block) < 0) {
        FAT_DPRINT ("Error: reading rootdir block\n");
        return -1;
    }
    dentptr = (dir_entry *)(do_fat_read_block + (gfd[fd].ent_offset * sizeof(dir_entry))) ;

    mydata->fatbufnum = -1;
    //if file size == 0, must find new start cluster
    if(dentptr->size == 0) {
    	start_cluster = getfree_cluster(mydata, &getno);
    	dentptr->starthi = (u16)(start_cluster>>16);
    	dentptr->start   = (u16)start_cluster;
    	old_cluster =
    	cur_cluster = start_cluster;
    }
    else {
        start_cluster = START(dentptr);
        old_cluster =
        cur_cluster = start_cluster;
        while(1) {
            cur_cluster = get_fatent(mydata, cur_cluster);
            if(CHECK_CLUST(cur_cluster, mydata->fatsize)) {
                break;
            }
            old_cluster = cur_cluster;
    	}

        cur_cluster = old_cluster;
    }

    fileoffset = dentptr->size % (mydata->clust_size * SECTOR_SIZE);
    memset(do_fat_cache_sector, 0, mydata->clust_size * SECTOR_SIZE);

    if(fileoffset != 0 || dentptr->size == 0 ) {
        cursect = mydata->fat_sect + mydata->fatlength * bs.fats + (old_cluster - ROOT_CLUSTER) * mydata->clust_size;
        //read the file tile
        if (disk_read (cursect, mydata->clust_size, do_fat_cache_sector) < 0) {
            FAT_DPRINT ("Error: reading block\n");
            return -1;
        }

        if(maxsize <= (mydata->clust_size * SECTOR_SIZE) - fileoffset) {
            memcpy(do_fat_cache_sector + fileoffset, buffer, maxsize);
            writesize = maxsize;
            maxsize = 0;
        }else {
            memcpy(do_fat_cache_sector + fileoffset, buffer, (mydata->clust_size * SECTOR_SIZE) - fileoffset);
            writesize = (mydata->clust_size * SECTOR_SIZE) - fileoffset;
            maxsize -= (mydata->clust_size * SECTOR_SIZE) - fileoffset;
        }
        // write first block
        if (disk_write (cursect, mydata->clust_size, do_fat_cache_sector) < 0) {
            FAT_DPRINT ("Error: writing block\n");
            return -1;
        }
        put_fatent(mydata,old_cluster,cur_cluster,1);
        printf("#");
    }
    //if(dentptr->size == 0)
    getno = MAX_CLUSTSIZE / (mydata->clust_size * SECTOR_SIZE);
    printf("set getno = %d and clust_size = %d\n",getno, mydata->clust_size );
    mydata->fatbufnum = -1;
    //write left blocks
    while(maxsize != 0) {
        getno = MAX_CLUSTSIZE / (mydata->clust_size * SECTOR_SIZE);
        printf("#");
        if (sharpnum++ % 64 == 0)
            printf("\n");

        memset(do_fat_cache_sector, 0, mydata->clust_size * SECTOR_SIZE);
        old_cluster = cur_cluster;
        cur_cluster = getfree_cluster(mydata, &getno);

        //put_fatent(mydata,old_cluster,cur_cluster,getno);//update file entry

        cursect = mydata->fat_sect + mydata->fatlength * bs.fats
                    + (cur_cluster - ROOT_CLUSTER) * mydata->clust_size;

        if(maxsize <= mydata->clust_size * SECTOR_SIZE * getno) {
            getno = maxsize / (mydata->clust_size * SECTOR_SIZE);
            //if(maxsize < mydata->clust_size * SECTOR_SIZE * getno)
            if((maxsize % (mydata->clust_size * SECTOR_SIZE)) > 0 )
                getno ++;

            memcpy(do_fat_cache_sector, buffer + writesize, maxsize);
            if (disk_write (cursect, mydata->clust_size * getno, do_fat_cache_sector) < 0) {
                FAT_DPRINT ("Error: Writing block\n");
                return -1;
            }
            writesize += maxsize;
            maxsize = 0;
            put_fatent(mydata,old_cluster,cur_cluster,getno);//update file entry
            break;
        }
        else {
            memcpy(do_fat_cache_sector, buffer + writesize, mydata->clust_size * SECTOR_SIZE * getno);
            if (disk_write (cursect, mydata->clust_size * getno, do_fat_cache_sector) < 0) {
                FAT_DPRINT ("Error: Writing block\n");
                return -1;
            }
            writesize += mydata->clust_size * SECTOR_SIZE  * getno;
            maxsize -= mydata->clust_size * SECTOR_SIZE  * getno;
        }
        put_fatent(mydata,old_cluster,cur_cluster,getno);//update file entry
        cur_cluster += (getno - 1);
    }

    //write file dir
    dentptr->size += writesize;
    cur_cluster = gfd[fd].ent_cluser;

    cursect = mydata->fat_sect + mydata->fatlength * bs.fats + (cur_cluster - ROOT_CLUSTER) * mydata->clust_size;

    if (disk_write (cursect, mydata->clust_size, do_fat_read_block) < 0) {
        FAT_DPRINT ("Error: reading rootdir block\n");
        return -1;
    }
    printf("\n");

    return 0;

}

int file_fat_open(const char *filename, const char *mode)
{
    int i;
    int fd = 0;
    int iNeedCreate = 0;

    if(strlen(filename)>12 || strlen(filename)<1)
        return -1;

    if(0==iNumOfFile)
        memset(gfd, 0, sizeof(struct filedescipter) * MAXFILEBUFFER);

    for(i=0; i<MAXFILEBUFFER; i++) {
        if(gfd[i].fd == 0) {
            gfd[i].fd = 1;
            fd = i;
            strcpy(gfd[i].filename, filename);
            break;
        }
    }

    if(strcmp("w",mode) == 0) {
    	iNeedCreate = 1;
    	gfd[fd].mode = MODECREATE;
    }else if(strcmp("a",mode) == 0) {
        iNeedCreate = 1;
        gfd[fd].mode = MODEAPPEND;
    }else {
        printf("file open failed: unsupported mode\n");
        return -1;
    }

    if(iNeedCreate) {
        if(!file_fat_search(fd))  //file exist
        {
            /* Don't recreate file in append mode */
            if(strcmp("a",mode) == 0)
            {
                iNumOfFile++;
	         return fd;
            }
            printf("file exist, and will be recreate\n");
        }
        else
            gfd[fd].mode = MODENEW;

        file_fat_create(fd, mode);
    }
    else {
        if(!file_fat_search(fd)) {
            printf("file exist, in append mode\n");
        }
        else {
            printf("file not found\n");
            return -1;
        }
    }

    iNumOfFile++;

    return fd;
}

void file_fat_close(int fd)
{
    if(fd<0 || fd>15) {
		printf("Invaild file num\n");
		return ;
    }

	memset(&gfd[fd], 0, sizeof(struct filedescipter));
	return;

}



long file_fat_create(int fd, const char *mode)
{
    char fnamecopy[2048];

    boot_sector bs;
    volume_info volinfo;
    fsdata datablock;
    fsdata *mydata = &datablock;
    dir_entry *dentptr;

    __u32 cur_cluster;
    __u32 start_cluster;
	int i,j;
	int bfoundempty = 0;


    int rootdir_size, cursect;

    if(fd<0 || fd>15) {
		printf("Invaild file num\n");
		return -1;
    }

    /*read fat boot sector and volume info from filesystem */
    if (read_bootsectandvi (&bs, &volinfo, &mydata->fatsize)) {
	    FAT_DPRINT ("Error: reading boot sector\n");
	    return -1;
    }

    /*affirm fat format is 32 or 16*/
    if (mydata->fatsize == 32) {
    mydata->fatlength = bs.fat32_length;
    } else {
    mydata->fatlength = bs.fat_length;
    }

    mydata->fat_sect = bs.reserved;
    cursect = mydata->rootdir_sect
        = mydata->fat_sect + mydata->fatlength * bs.fats;
    mydata->clust_size = bs.cluster_size;
    cur_cluster = ROOT_CLUSTER;
    /*fat format is 32*/
    if (mydata->fatsize == 32) {
    rootdir_size = mydata->clust_size;
    mydata->data_begin = mydata->rootdir_sect   /* + rootdir_size */
        - (mydata->clust_size * 2);
    }
    else {
    rootdir_size = ((bs.dir_entries[1] * (int) 256 + bs.dir_entries[0])
            * sizeof (dir_entry)) / SECTOR_SIZE;
    mydata->data_begin = mydata->rootdir_sect + rootdir_size
        - (mydata->clust_size * 2);
    }/*fat format is other,for example: 16*/

    mydata->fatbufnum = -1;
    /*FAT 32 fatlength:998 ----->print fat format and disk storage*/
    FAT_DPRINT ("FAT%d, fatlength: %d\n", mydata->fatsize,
        mydata->fatlength);
    FAT_DPRINT ("Rootdir begins at sector: %d, offset: %x, size: %d\n"
        "Data begins at: %d\n",
        mydata->rootdir_sect, mydata->rootdir_sect * SECTOR_SIZE,
        rootdir_size, mydata->data_begin);
    FAT_DPRINT ("Cluster size: %d\n", mydata->clust_size);


	if(gfd[fd].mode == MODENEW) // didn't support long name
	{

		while(1){
		    if(disk_read(cursect, mydata->clust_size, do_fat_read_block) < 0) {
		        FAT_DPRINT ("Error: reading rootdir block\n");
		        return -1;
		    }


		    dentptr = (dir_entry *) do_fat_read_block;/*data is in da_fat_read_block*/

#if 0
		        for (i = 0; i < DIRENTSPERBLOCK; i++)
#else
		        for (i = 0; i < DIRENTSPERBLOCK * (mydata->clust_size); i++)
#endif
		        {
		            if(dentptr->name[0] == DELETED_FLAG)//Free entry
		            {
		                bfoundempty = 1;
		                break;
		            }
		            else if(dentptr->name[0] == 0x0) //last free entry
		            {
		                bfoundempty = 1;
		                break;
		            }
					dentptr++;
		        }

				if(bfoundempty) // didn't support long name
				{
					gfd[fd].ent_cluser = cur_cluster;
					gfd[fd].ent_offset = i;
				    memset(fnamecopy,0,2048);
					memcpy(fnamecopy,gfd[fd].filename,12);
				    memset(&dentptr->name[0],0x20,11);
					upcase(fnamecopy);
					j=0;
				    for(i=0;i<8;i++) {
						if(fnamecopy[i] == '.') {
							j=i+1;
							break;
						}
				        dentptr->name[i] = fnamecopy[i];
				    }

				    for(i=0;i<3;i++) {
					    if(fnamecopy[j+i] == '\0')
							break;
				        dentptr->ext[i] = fnamecopy[j+i];
				    }


					dentptr->ctime =
					dentptr->time  =
					dentptr->ctime_ms = 0;
					dentptr->date  = 0x21;
					dentptr->adate = 0x3d6b;
					dentptr->cdate = 0x21;
					dentptr->start = 0;
					dentptr->starthi = 0;
					dentptr->size = 0;
					dentptr->attr = 0x20;

					break;

				}

		        cur_cluster = get_fatent (mydata, cur_cluster);

		        if (CHECK_CLUST(cur_cluster, mydata->fatsize))
		        {
		            FAT_DPRINT ("RootDentname == NULL - %d\n", i);
					break;
		        }
		        else
		        {
		            cursect = mydata->fat_sect + mydata->fatlength * bs.fats + (cur_cluster - ROOT_CLUSTER) * mydata->clust_size;
		        }

			//FAT16 special handle
			if((mydata->fatlength != 32) && cur_cluster == ROOT_CLUSTER) {
				if(0) {
				    FAT_DPRINT ("Error: FAT16 root dir full\n");
					return -1;
				}
			}
		}
	}
	else {

		cur_cluster = gfd[fd].ent_cluser;
		cursect = mydata->fat_sect + mydata->fatlength * bs.fats + (cur_cluster - ROOT_CLUSTER) * mydata->clust_size;


		printf("fd:%d, cur_cluster:%d, cursect:%d, offset:%d\n",fd, cur_cluster, cursect, gfd[fd].ent_offset);
	    if (disk_read (cursect, mydata->clust_size, do_fat_read_block) < 0) {
	        FAT_DPRINT ("Error: reading rootdir block\n");
	        return -1;
	    }
	    dentptr = (dir_entry *)(do_fat_read_block + (gfd[fd].ent_offset * sizeof(dir_entry))) ;

	    start_cluster = START(dentptr);
	    printf("start_cluster= %d\n",start_cluster);
		if(clear_fatent(mydata, start_cluster) < 0) {
			FAT_DPRINT("error at clear fat ent");
			return -1;
		}

	}


//FIXME: didn't check the dentptr

    dentptr->start = 0;
    dentptr->starthi= 0;
    dentptr->size = 0;

	//save fat table

	if(disk_write(cursect, mydata->clust_size, do_fat_read_block) < 0) {
        FAT_DPRINT ("Error: writing rootdir block\n");
        return -1;
    }

	return 0;

}

long file_fat_search(int fd)
{
#if 1 // CONFIG_NIOS /* NIOS CPU cannot access big automatic arrays */
    static
#endif
    char fnamecopy[2048];
    boot_sector bs;
    volume_info volinfo;
    fsdata datablock;
    fsdata *mydata = &datablock;
    dir_entry *dentptr;
    __u16 prevcksum = 0xffff;
    char *subname = "";
    int rootdir_size, cursect;
    int idx, isdir = 0;
    int files = 0, dirs = 0;
    //long ret = 0;
    __u32 cur_cluster;
	__u32 bfnameexist = 0;
	int i ;
	//char *pStr=NULL;
	int dols = LS_NO;

    if(fd<0 || fd>15) {
		printf("Invaild file num\n");
		return -1;
    }

	if(gfd[fd].fd == 0)
		return -1;

    /*read fat boot sector and volume info from filesystem */
    if (read_bootsectandvi (&bs, &volinfo, &mydata->fatsize)) {
    FAT_DPRINT ("Error: reading boot sector\n");
    return -1;
    }

    /*affirm fat format is 32 or 16*/
    if (mydata->fatsize == 32) {
    mydata->fatlength = bs.fat32_length;
    } else {
    mydata->fatlength = bs.fat_length;
    }

    mydata->fat_sect = bs.reserved;
    cursect = mydata->rootdir_sect
        = mydata->fat_sect + mydata->fatlength * bs.fats;
    mydata->clust_size = bs.cluster_size;
    cur_cluster = ROOT_CLUSTER;
    /*fat format is 32*/
    if (mydata->fatsize == 32) {
    rootdir_size = mydata->clust_size;
    mydata->data_begin = mydata->rootdir_sect   /* + rootdir_size */
        - (mydata->clust_size * 2);
    }
    else {
    rootdir_size = ((bs.dir_entries[1] * (int) 256 + bs.dir_entries[0])
            * sizeof (dir_entry)) / SECTOR_SIZE;
    mydata->data_begin = mydata->rootdir_sect + rootdir_size
        - (mydata->clust_size * 2);
    }/*fat format is other,for example: 16*/

    mydata->fatbufnum = -1;
    /*FAT 32 fatlength:998 ----->print fat format and disk storage*/
    FAT_DPRINT ("FAT%d, fatlength: %d\n", mydata->fatsize,
        mydata->fatlength);
    FAT_DPRINT ("Rootdir begins at sector: %d, offset: %x, size: %d\n"
        "Data begins at: %d\n",
        mydata->rootdir_sect, mydata->rootdir_sect * SECTOR_SIZE,
        rootdir_size, mydata->data_begin);
    FAT_DPRINT ("Cluster size: %d\n", mydata->clust_size);

    /* Make a copy of the filename and convert it to lowercase */
    strcpy (fnamecopy, gfd[fd].filename);
    downcase (fnamecopy);// convert it to lowercase

    if (*fnamecopy == '\0') {
    if (!dols)
    	{
    	printf("write 0");
        return -1;
    	}
    dols = LS_ROOT;
    }
    else if ((idx = dirdelim (fnamecopy)) >= 0) {
    isdir = 1;
    fnamecopy[idx] = '\0';
    subname = fnamecopy + idx + 1;
    /* Handle multiple delimiters */
    while (ISDIRDELIM (*subname))
        subname++;
    }
    else if (dols) {
    isdir = 1;
    }

///dir fetch
	while (1) {

	    if (disk_read (cursect, mydata->clust_size, do_fat_write_block) < 0) {
	        FAT_DPRINT ("Error: reading rootdir block\n");
	        return -1;
	    }
	    dentptr = (dir_entry *) do_fat_write_block;/*data is in da_fat_read_block*/

#if 0
	        for (i = 0; i < DIRENTSPERBLOCK; i++)
#else
	        for (i = 0; i < DIRENTSPERBLOCK * (mydata->clust_size); i++)
#endif
	        {
	        char s_name[14], l_name[256];

	        l_name[0] = '\0';

	        if ((dentptr->attr & ATTR_VOLUME)) {
#ifdef CONFIG_SUPPORT_VFAT
	        if ((dentptr->attr & ATTR_VFAT) &&
	            (dentptr->name[0] & LAST_LONG_ENTRY_MASK)) {
	            prevcksum = ((dir_slot *) dentptr)->alias_checksum;
#if 0
	            get_vfatname (mydata, 0, do_fat_read_block, dentptr, l_name);
#else

	            get_vfatname (mydata, cur_cluster, do_fat_write_block, dentptr, l_name);
#endif

	            if (dols == LS_ROOT) {
	            int isdir = (dentptr->attr & ATTR_DIR);
	            char dirc;
	            int doit = 0;

	            if (isdir) {
	                dirs++;
	                dirc = '/';
	                doit = 1;
	            } else {
	                dirc = ' ';
	                if (l_name[0] != 0) {
	                files++;
	                doit = 1;
	                }
	            }
	            if (doit)
	            {
	                if (dirc == ' ') {
	                printf (" %8ld   %s%c\n",
	                    (long) FAT2CPU32 (dentptr->size),
	                    l_name, dirc);
	                } else {
	                printf ("            %s%c\n", l_name, dirc);
	                }
	            }
	            dentptr++;
	            continue;
	            }
	            FAT_DPRINT ("Rootvfatname: |%s|\n", l_name);
	        }
	        else
#endif
	        {
	            /* Volume label or VFAT entry */
	            dentptr++;
	            continue;
	        }

	        }/*   */

	      /*this segment is not run*/
	        else if (dentptr->name[0] == 0)
	        {
	        FAT_DPRINT ("RootDentname == NULL - %d\n", i);
	        if (dols == LS_ROOT) {
	            printf ("\n%d file(s), %d dir(s)\n\n", files, dirs);//root dirctory print number of file and folder
	            return 0;
	        }
			break;
	        //return -1;
	        }


#ifdef CONFIG_SUPPORT_VFAT
	        else if (dols == LS_ROOT
	             && mkcksum (dentptr->name) == prevcksum)
	        {
	        dentptr++;
	        continue;
	        }
#endif
	        get_name (dentptr, s_name);

	        if (dols == LS_ROOT) {
	        int isdir = (dentptr->attr & ATTR_DIR);
	        char dirc;
	        int doit = 0;

	        if (isdir) {
	            dirc = '/';
	            if (s_name[0] != 0) {
	            dirs++;
	            doit = 1;
	            }
	        }
	        else {
	            dirc = ' ';
	            if (s_name[0] != 0) {
	            files++;
	            doit = 1;
	            }
	        }
	        if (doit) {
	            if (dirc == ' ') {
	            printf (" %8ld   %s%c\n",(long) FAT2CPU32 (dentptr->size), s_name,dirc);
	            }
	            else {
	            printf ("            %s%c\n", s_name, dirc);
	            }
	        }
	        dentptr++;
	        continue;
	        }

	        if (strcmp (fnamecopy, s_name) && strcmp (fnamecopy, l_name)) {
	        FAT_DPRINT ("RootMismatch: |%s|%s|\n", s_name, l_name);
	        dentptr++;
	        continue;
	        }
	        if (isdir && !(dentptr->attr & ATTR_DIR)) {
            printf("write 1\n");
			return -1;
	        }
	        FAT_DPRINT ("RootName: %s", s_name);/*root dirterory*/
	        FAT_DPRINT (", start: 0x%x", START (dentptr));
	        FAT_DPRINT (", size:  0x%x %s\n", FAT2CPU32 (dentptr->size), isdir ? "(DIR)" : "");

            bfnameexist = 1;
	        goto rootdir_done;  /* We got a match */
	    }
#if 0
	    cursect++;
#else
	        cur_cluster = get_fatent (mydata, cur_cluster);

	        if (CHECK_CLUST(cur_cluster, mydata->fatsize))
	        {
	            FAT_DPRINT ("RootDentname == NULL - %d\n", i);
	            if (dols == LS_ROOT)
	            {
	                printf ("\n%d file(s), %d dir(s)\n\n", files, dirs);
	                return 0;
	            }
				break;
	        }
	        else
	        {
	            cursect = mydata->fat_sect + mydata->fatlength * bs.fats + (cur_cluster - ROOT_CLUSTER) * mydata->clust_size;
	        }
#endif
	}


rootdir_done:


	if(bfnameexist) {
		printf("file %s found\n",gfd[fd].filename);
		memcpy(&(gfd[fd].dirEnt), dentptr, sizeof(struct dir_entry));
		gfd[fd].ent_cluser = cur_cluster;
		gfd[fd].ent_offset = i;
		printf("gfd[%d].ent_offset %d %d ent_cluser:%d",fd,i,gfd[fd].ent_offset,gfd[fd].ent_cluser);
		return 0;// fname exist
	}
	else {
		printf("file %s not found\n",gfd[fd].filename);
		return -1; //
	}


	return 0;
}

#endif /* #if (CONFIG_COMMANDS & CFG_CMD_FAT) */
