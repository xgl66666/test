/*
 * Driver for NAND support, Rick Bronson
 * borrowed heavily from:
 * (c) 1999 Machine Vision Holdings, Inc.
 * (c) 1999, 2000 David Woodhouse <dwmw2@infradead.org>
 *
 * Added 16-bit nand support
 * (C) 2004 Texas Instruments
 */

#include <common.h>

#include "../drivers/mstar/unfd/inc/common/drvNAND.h"




#ifndef CFG_NAND_LEGACY
/*
 *
 * New NAND support
 *
 */
#include <common.h>

#if (CONFIG_COMMANDS & CFG_CMD_FAT)
#include <fat.h>
#endif

#if (CONFIG_COMMANDS & CFG_CMD_NAND)

#include <command.h>
#include <watchdog.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include "uboot_mmap.h"

#ifdef CONFIG_SHOW_BOOT_PROGRESS
# include <status_led.h>
# define SHOW_BOOT_PROGRESS(arg)  show_boot_progress(arg)
#else
# define SHOW_BOOT_PROGRESS(arg)
#endif

#include <jffs2/jffs2.h>
#include <nand.h>
#include <asm/errno.h>
#define READ_BUFFER_LEN 0x100000
// This function is defined in cmd_fat.c
#if (CONFIG_COMMANDS & CFG_CMD_FAT)
extern block_dev_desc_t *get_dev (const char* ifname, int dev);
#endif
/* parition handling routines */
int mtdparts_init(void);
int id_parse(const char *id, const char **ret_id, u8 *dev_type, u8 *dev_num);
int find_dev_and_part(const char *id, struct mtd_device **dev,
    u8 *part_num, struct part_info **part);
#if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)
#define KL_IN_MTD    1
#endif
#define NAND_CLK_LEN                 2
extern nand_info_t nand_info[];       /* info for NAND chips */

extern int strcmp(const char * cs,const char * ct);

extern unsigned int cfg_env_offset;

static int nand_dump_oob(nand_info_t *nand, ulong off)
{
  return 0;
}

static int nand_dump(nand_info_t *nand, ulong off)
{
  int i=0;
  u_char *buf=NULL, *p=NULL;

  buf = malloc(nand->oobblock + nand->oobsize);
  if (!buf) {
    puts("No memory for page buffer\n");
    return 1;
  }
  off &= ~(nand->oobblock - 1);
  i = nand_read_raw(nand, buf, off, nand->oobblock, nand->oobsize);
  if (i < 0) {
    printf("Error (%d) reading page %08lx\n", i, off);
    free(buf);
    return 1;
  }
  printf("Page %08lx dump:\n", off);
  i = nand->oobblock >> 4; p = buf;
  while (i--) {
    printf( "\t%02x %02x %02x %02x %02x %02x %02x %02x"
      "  %02x %02x %02x %02x %02x %02x %02x %02x\n",
      p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7],
      p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
    p += 16;
  }
  puts("OOB:\n");
  i = nand->oobsize >> 3;
  while (i--) {
    printf( "\t%02x %02x %02x %02x %02x %02x %02x %02x\n",
      p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
    p += 8;
  }
  free(buf);

  return 0;
}

/* ------------------------------------------------------------------------- */

static inline int str2long(char *p, ulong *num)
{
    char *endptr=NULL;

    *num = simple_strtoul(p, &endptr, 16);
    return (*p != '\0' && *endptr == '\0') ? 1 : 0;
}

EN_NAND_FS_FORMAT nand_fs_format = NAND_FS_FORMAT_NORMAL;


static int arg_off_size(int argc, char *argv[], nand_info_t *nand, ulong *off, ulong *size)
{
  int idx = nand_curr_device;
  struct mtd_device *dev=NULL;
  struct part_info *part=NULL;
  u8 pnum=0;

  if (argc >= 1 && !(str2long(argv[0], off))) {
    if ((mtdparts_init() == 0) &&
        (find_dev_and_part(argv[0], &dev, &pnum, &part) == 0)) {
      if (dev->id->type != MTD_DEV_TYPE_NAND) {
        puts("not a NAND device\n");
        return -1;
      }
      *off = part->offset;
      if (argc >= 2) {
        if (!(str2long(argv[1], size))) {
          printf("'%s' is not a number\n", argv[1]);
          return -1;
        }
        if ((*size > part->size) && (nand_fs_format != NAND_FS_FORMAT_RAW)
                                     && (nand_fs_format != NAND_FS_FORMAT_MKBIN))
            {
          *size = part->size;
            }
      } else {
        *size = part->size;
      }
      idx = dev->id->num;
      *nand = nand_info[idx];
      goto out;
    }
  }

  if (argc >= 1) {
    if (!(str2long(argv[0], off))) {
      printf("'%s' is not a number\n", argv[0]);
      return -1;
    }
  } else {
    *off = 0;
  }

  if (argc >= 2) {
    if (!(str2long(argv[1], size))) {
      printf("'%s' is not a number\n", argv[1]);
      return -1;
    }
  } else {
    *size = nand->size - *off;
  }

out:
#if (!CONFIG_MSTAR_SILENT)
  printf("device %d ", idx);
  if (*size == nand->size)
    puts("whole chip\n");
  else
    printf("offset 0x%lx, size 0x%lx\n", *off, *size);
#endif
  return 0;
}

extern void drvNAND_BIST_Test(void);
extern void drvNAND_Stable_Test(void);


int do_nand(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
    int i=0, dev, ret = 0;
    ulong addr=0, off=0, size=0;
    char *cmd=NULL, *s=NULL;
    nand_info_t *nand=NULL;
    int quiet = 0;
    const char *quiet_str = getenv("quiet");
    int eraseloop=0;
  //int clean, o;

#if defined(NAND_SPEED_TEST) && NAND_SPEED_TEST
  HWTimer_Start();
#endif

  /* at least two arguments please */
  if (argc < 2)
    goto usage;
    #if defined (CONFIG_BACKUP_PARTITION)
    {
        char *pEnv=NULL;
        pEnv=getenv("updateKLPermit");
        if(pEnv!=NULL){
            if(strcmp(pEnv,"NO")==0){
                if((strncmp(argv[1],"erase",5)==0)||(strncmp(argv[1],"write.e",7)==0)){
                    printf("updateKLPermit=NO");
                    return 0;
                }
            }
        }
    }
    #endif
  if (quiet_str)
    quiet = simple_strtoul(quiet_str, NULL, 0) != 0;

  cmd = argv[1];

  if (strcmp(cmd, "info") == 0) {

    putc('\n');
		for (i = 0; i < CFG_MAX_NAND_DEVICE; i++) {
      if (nand_info[i].name)
        printf("Device %d: %s, sector size %u KiB\n",
          i, nand_info[i].name,
          (u_int32_t)(nand_info[i].erasesize >> 10));
    }
    return 0;
  }

  if (strcmp(cmd, "device") == 0) {
        #if 0
    if (argc < 3) {
      if ((nand_curr_device < 0) ||
			    (nand_curr_device >= CFG_MAX_NAND_DEVICE))
        puts("\nno devices available\n");
      else
        printf("\nDevice %d: %s\n", nand_curr_device,
          nand_info[nand_curr_device].name);
      return 0;
    }
    #endif
    dev = (int)simple_strtoul(argv[2], NULL, 10);
		if (dev < 0 || dev >= CFG_MAX_NAND_DEVICE || !nand_info[dev].name) {
      puts("No such device\n");
      return 1;
    }
    printf("Device %d: %s", dev, nand_info[dev].name);
    puts("... is now current device\n");
    nand_curr_device = dev;

#ifdef CFG_NAND_SELECT_DEVICE
    /*
     * Select the chip in the board/cpu specific driver
     */
    board_nand_select_device(nand_info[dev].priv, dev);
#endif

    return 0;
  }

  if (strcmp(cmd, "bad") != 0 && strcmp(cmd, "erase") != 0 &&
      strncmp(cmd, "dump", 4) != 0 && strcmp(cmd, "eloop") != 0 &&
      strncmp(cmd, "read", 4) != 0 && strncmp(cmd, "write", 5) != 0 &&
      strcmp(cmd, "scrub") != 0 && strcmp(cmd, "markbad") != 0 &&
      strcmp(cmd, "biterr") != 0 &&
      strcmp(cmd, "lock") != 0 && strcmp(cmd, "unlock") != 0 &&
#if ENABLE_MODULE_BFN_WRITE_NAND
        strcmp(cmd, "bfn") != 0 &&
#endif
      strcmp(cmd, "bist") != 0 && strcmp(cmd, "stable") != 0 )
    goto usage;
    #if 0
  /* the following commands operate on the current device */
  if (nand_curr_device < 0 || nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE ||
      !nand_info[nand_curr_device].name) {
    puts("\nno devices available\n");
    return 1;
  }
  #endif
  nand = &nand_info[nand_curr_device];

  if (strcmp(cmd, "bad") == 0) {
    printf("\nDevice %d bad blocks:\n", nand_curr_device);
    for (off = 0; off < nand->size; off += nand->erasesize)
      if (nand_block_isbad(nand, off))
        printf("  %08lx\n", off);
    return 0;
  }
#if ENABLE_MODULE_BFN_WRITE_NAND
	/*
	 * Syntax is:
	 *   0    1     2       3    4
	 *   nand bfn [hash/miu/ldr/app] [blk addr][miu address]
	 */
	if (strcmp(cmd, "bfn") == 0){
        printf("burn boot from nand data\n");
        if (argc < 4)
        {
            printf("argument wrong!\n");
            return 0;
        }
        U32 addr = simple_strtoul(argv[4], NULL, 16);
        U32 blk = simple_strtoul(argv[3], NULL, 16);
        U32 bfn_type = 0;
        if (strncmp(argv[2],"miu",3)==0)
        {
            printf("burn the MIU param\n");
            bfn_type = 1;
        }
        else if (strncmp(argv[2],"ldr",3)==0)
        {
            printf("burn the LDR param\n");
            bfn_type = 2;
        }
        else if (strncmp(argv[2],"app",3)==0)
        {
            printf("burn the APP param\n");
            bfn_type = 3;
        }
        else if (strncmp(argv[2],"hash",4)==0)
        {
            printf("burn the Hash0 param\n");
            bfn_type = 4;
        }
        else if (strncmp(argv[2],"newcis",6)==0)
        {
            printf("burn the new CIS and MIU format\n");
            bfn_type = 5;
        }
        else if (strncmp(argv[2],"chkcis",6)==0)
        {
            printf("check the new CIS and MIU format\n");
            bfn_type = 6;
        }

        else
        {
            printf("error boot from nand parameter\n");
            return 0;
        }
        extern U32 drvNAND_BFN_FLASH_INFO(U32 bfn_type,U32 bfn_blk,U32 bfn_addr);
        drvNAND_BFN_FLASH_INFO(bfn_type,blk,addr);
	    return ret == 0 ? 0 : 1;
	}
#endif

  /*
   * Syntax is:
   *   0    1     2       3    4
   *   nand erase [clean] [off size]
   */
  if (strcmp(cmd, "erase") == 0 || strcmp(cmd, "scrub") == 0
    ||strcmp(cmd, "eloop") == 0 ) {
    nand_erase_options_t opts;
    /* "clean" at index 2 means request to write cleanmarker */

    int clean = argc > 2 && !strcmp("clean", argv[2]);
    int o = clean ? 3 : 2;

    int scrub = !strcmp(cmd, "scrub");
        int eloop = !strcmp(cmd, "eloop");

    printf("\nNAND %s: ", scrub ? "scrub" : "erase");
    /* skip first two or three arguments, look for offset and size */

    if(strcmp(cmd, "eloop") == 0 )
    {
        clean = 0;
        o     = 2;
      off   = 0;
      //size  = 0x4000000;
      size = nand->size;
    }
    else
    {
        if (arg_off_size(argc - o, argv + o, nand, &off, &size) != 0)
          return 1;
    }
        //printf("clean is %2x\n o is %2x\n off is %2x \n size is %2x",clean,o,off,size);

    memset(&opts, 0, sizeof(opts));
    opts.offset = off;
    opts.length = size;
    opts.jffs2  = clean;
    opts.quiet  = quiet;
        //printf("offset is %2x\n length is %2x\n jffs2 is %2x \n size is %2x",off,size,clean);
    if (scrub) {
      puts("Warning: "
           "scrub option will erase all factory set "
           "bad blocks!\n"
           "         "
           "There is no reliable way to recover them.\n"
           "         "
           "Use this command only for testing purposes "
           "if you\n"
           "         "
           "are sure of what you are doing!\n"
           "\nReally scrub this NAND flash? <y/N>\n");

      if (getc() == 'y' && getc() == '\r') {
        opts.scrub = 1;
      } else {
        puts("scrub aborted\n");
        return -1;
      }
    }

        if(eloop)
        {
            eraseloop = simple_strtoul(argv[2], NULL, 16);
      printf("eraseloop is %x",eraseloop);
    }
    else
    {
            eraseloop = 1;
    }

    for(i=0; i<eraseloop; i++)
    {
        ret = nand_erase_opts(nand, &opts);
        printf("%4x %s\n",i,ret ? "ERROR" : "OK");
    }
         #if defined (CONFIG_BACKUP_PARTITION)
            if(ret!=0){
                printf("setenv updateKLPermit NO\n");
                run_command("setenv updateKLPermit NO",0);
                //run_command("saveenv",0);
            }

         #endif
    return ret == 0 ? 0 : 1;
  }

  if (strncmp(cmd, "dump", 4) == 0) {
    if (argc < 3)
      goto usage;

    s = strchr(cmd, '.');
    off = (int)simple_strtoul(argv[2], NULL, 16);

    if (s != NULL && strcmp(s, ".oob") == 0)
      ret = nand_dump_oob(nand, off);
    else
      ret = nand_dump(nand, off);

    return ret == 0 ? 1 : 0;

  }

  /* read write */
  if (strncmp(cmd, "read", 4) == 0 || strncmp(cmd, "write", 5) == 0) {
    int read;

    if (argc < 4)
      goto usage;

        addr = (0 == strncmp(argv[2], UPGRADE_ADDR_STR, strlen(UPGRADE_ADDR_STR))) ? UPGRADE_BUFFER_ADDR : simple_strtoul(argv[2], NULL, 16);

    read = strncmp(cmd, "read", 4) == 0; /* 1 = read, 0 = write */
#if (!CONFIG_MSTAR_SILENT)
    printf("\nNAND %s: ", read ? "read" : "write");
#endif
    if (arg_off_size(argc - 3, argv + 3, nand, &off, &size) != 0)
      return 1;

        nand_fs_format=NAND_FS_FORMAT_NORMAL;

    s = strchr(cmd, '.');
    if (s != NULL &&
        (!strcmp(s, ".jffs2") || !strcmp(s, ".e") || !strcmp(s, ".i") || !strcmp(s, ".raw") || !strcmp(s, ".mkbin"))) {

        if(!strcmp(s, ".jffs2"))
        {
            nand_fs_format=NAND_FS_FORMAT_JFFS2;
        }
        else if(!strcmp(s, ".e"))
        {
            nand_fs_format=NAND_FS_FORMAT_E;
        }
        else if(!strcmp(s, ".i"))
        {
            nand_fs_format=NAND_FS_FORMAT_I;
        }
                  else if(!strcmp(s, ".raw"))
        {
            nand_fs_format=NAND_FS_FORMAT_RAW;
        }
                  else if(!strcmp(s, ".mkbin"))
        {
            nand_fs_format=NAND_FS_FORMAT_MKBIN;
        }

      if (read) {
        /* read */
        nand_read_options_t opts;
        memset(&opts, 0, sizeof(opts));
        opts.buffer = (u_char*) addr;
        opts.length = size;
        opts.offset = off;
        opts.quiet      = quiet;
                            if ((NAND_FS_FORMAT_RAW == nand_fs_format) || (NAND_FS_FORMAT_MKBIN == nand_fs_format))
                            {
                                opts.fatfile_name = "nanddump.bin";
                                opts.readoob = 1;
                            }
        ret = nand_read_opts(nand, &opts);
                flush_cache(addr, size);
      } else {
        /* write */
        nand_write_options_t opts;
        memset(&opts, 0, sizeof(opts));
        opts.buffer = (u_char*) addr;
        opts.length = size;
        opts.offset = off;
        /* opts.forcejffs2 = 1; */
        opts.pad  = 1;
        opts.blockalign = 1;
        opts.quiet      = quiet;
                flush_cache(addr, size);
        ret = nand_write_opts(nand, &opts);
                #if defined (CONFIG_BACKUP_PARTITION)
                 if(ret!=0){
                    printf("setenv updateKLPermit NO\n");
                    run_command("setenv updateKLPermit NO",0);
                    //run_command("saveenv",0);
                }

                #endif
		}
        }else if (  s != NULL && !strcmp(s, ".yaffs"))
        {
            if (read) {

                nand_read_options_t opts;
                memset(&opts, 0, sizeof(opts));
                opts.buffer = (u_char*) addr;
                opts.length = size;
                opts.offset = off;
                opts.readoob = 0;
                opts.quiet      = quiet;
                ret = nand_read_opts(nand, &opts);
            } else {

                nand_write_options_t opts;
                memset(&opts, 0, sizeof(opts));
                opts.buffer = (u_char*) addr;
                opts.length = size;
                opts.offset = off;
                opts.noecc = 0;
                opts.writeoob = 0;
                opts.blockalign = 1;
                opts.quiet      = quiet;
                opts.pad	= 1;
                flush_cache(addr, size);
                ret = nand_write_opts(nand, &opts);
            }
        }
        else {
      if (read) {
        ret = nand_read(nand, off, &size, (u_char *)addr);
                flush_cache(addr, size);
            } else {
                flush_cache(addr, size);
        ret = nand_write(nand, off, &size, (u_char *)addr);
            }
    }

#if (!CONFIG_MSTAR_SILENT)
    printf(" %lu bytes %s: %s\n", size,
           read ? "read" : "written", ret ? "ERROR" : "OK");
#endif

    #if defined(NAND_SPEED_TEST) && NAND_SPEED_TEST
    if( size )
    {
      float speed = 0, tmp;
      unsigned int a, b = 0;
      U32 u32TotalNANDRWTime = 0;

      u32TotalNANDRWTime = HWTimer_End();

      speed = (((float)size)/(((float)u32TotalNANDRWTime)/((float)12000000)))/((float)(1024*1024));
    a = (int)speed;

      tmp = (speed - ((float)a))*1000;
      b = (int)tmp;

      printf("total time = %d.%03ds(%d)\n", u32TotalNANDRWTime/12000000, u32TotalNANDRWTime/12000, u32TotalNANDRWTime);
      printf("total size = %X\n", size);
      printf("nand %s speed = %d.%03d MB/s\n", cmd, a, b);
    }
    #endif

    return ret == 0 ? 0 : 1;
  }

  if (strcmp(cmd, "markbad") == 0) {
    addr = (ulong)simple_strtoul(argv[2], NULL, 16);

    int ret = nand->block_markbad(nand, addr);
    if (ret == 0) {
      printf("block 0x%08lx successfully marked as bad\n",
             (ulong) addr);
      return 0;
    } else {
      printf("block 0x%08lx NOT marked as bad! ERROR %d\n",
             (ulong) addr, ret);
    }
    return 1;
  }
  if (strcmp(cmd, "biterr") == 0) {
    /* todo */
    return 1;
  }

  if (strcmp(cmd, "lock") == 0) {
    int tight  = 0;
    int status = 0;
    if (argc == 3) {
      if (!strcmp("tight", argv[2]))
        tight = 1;
      if (!strcmp("status", argv[2]))
        status = 1;
    }

    if (status) {
      ulong block_start = 0;
      ulong off;
      int last_status = -1;

      struct nand_chip *nand_chip = nand->priv;
      /* check the WP bit */
      nand_chip->cmdfunc (nand, NAND_CMD_STATUS, -1, -1);
      printf("device is %swrite protected\n",
             (nand_chip->read_byte(nand) & 0x80 ?
        "NOT " : "" ) );

      for (off = 0; off < nand->size; off += nand->oobblock) {
        int s = nand_get_lock_status(nand, off);

        /* print message only if status has changed
         * or at end of chip
         */
        if (off == nand->size - nand->oobblock
            || (s != last_status && off != 0))  {

          printf("%08lu - %08lu: %8lu pages %s%s%s\n",
                 block_start,
                 off-1,
                 (ulong)((off-block_start)/nand->oobblock),
                 ((last_status & NAND_LOCK_STATUS_TIGHT) ? "TIGHT " : ""),
                 ((last_status & NAND_LOCK_STATUS_LOCK) ? "LOCK " : ""),
                 ((last_status & NAND_LOCK_STATUS_UNLOCK) ? "UNLOCK " : ""));
        }

        last_status = s;
           }
    } else {
      if (!nand_lock(nand, tight)) {
        puts("NAND flash successfully locked\n");
      } else {
        puts("Error locking NAND flash\n");
        return 1;
      }
    }
    return 0;
  }

  if (strcmp(cmd, "unlock") == 0) {
    if (arg_off_size(argc - 2, argv + 2, nand, &off, &size) < 0)
      return 1;

    if (!nand_unlock(nand, off, size)) {
      puts("NAND flash successfully unlocked\n");
    } else {
      puts("Error unlocking NAND flash, "
           "write and erase will probably fail\n");
      return 1;
    }
    return 0;
  }

  if( strcmp(cmd, "bist") == 0 ) {

    int loop = 0;

    if( !strcmp("forever", argv[2]) )
      loop = 1;

    puts("Warnning:\n"
       "bist option will damage all of your data\n"
       "including the kernel, file system\n"
       "and environments in NAND flash.\n\n"
       "Do you really want to do it? (y/n)\n\n");
    if (getc() == 'y' && getc() == '\r') {
        do {
          drvNAND_BIST_Test();
        } while( loop == 1 );
      } else {
        puts("bist aborted\n");
        return -1;
      }
    return 0;
  }

  if( strcmp(cmd, "stable") == 0 )
  {
    puts("Warnning:\n"
       "stable test option will damage all of your data\n"
       "including the kernel, file system\n"
       "and environments in NAND flash.\n\n"
       "Do you really want to do it? (y/n)\n\n");
    if (getc() == 'y' && getc() == '\r')
    {
      drvNAND_Stable_Test();
    }
    else
    {
      puts("stable test aborted\n");
      return -1;
    }
    return 0;
  }

usage:
  printf("Usage:\n%s\n", cmdtp->usage);
  return 1;
}

static int nand_load_image(cmd_tbl_t *cmdtp, nand_info_t *nand,
         ulong offset, ulong addr, char *cmd)
{
  int r=0;
  char *ep=NULL;
  ulong cnt=0;
  image_header_t *hdr=NULL;

  printf("\nLoading from %s, offset 0x%lx\n", nand->name, offset);

  cnt = nand->oobblock;
  r = nand_read(nand, offset, &cnt, (u_char *) addr);
  if (r) {
    puts("** Read error\n");
    SHOW_BOOT_PROGRESS(-1);
    return 1;
  }

  hdr = (image_header_t *) addr;

  if (ntohl(hdr->ih_magic) != IH_MAGIC) {
    printf("\n** Bad Magic Number 0x%x **\n", hdr->ih_magic);
    SHOW_BOOT_PROGRESS(-1);
    return 1;
  }

  print_image_hdr(hdr);

  cnt = (ntohl(hdr->ih_size) + sizeof (image_header_t));

  r = nand_read(nand, offset, &cnt, (u_char *) addr);
  if (r) {
    puts("** Read error\n");
    SHOW_BOOT_PROGRESS(-1);
    return 1;
  }

  /* Loading ok, update default load address */

  load_addr = addr;

  /* Check if we should attempt an auto-start */
  if (((ep = getenv("autostart")) != NULL) && (strcmp(ep, "yes") == 0)) {
    char *local_args[2];
    extern int do_bootm(cmd_tbl_t *, int, int, char *[]);

    local_args[0] = cmd;
    local_args[1] = NULL;

    printf("Automatic boot of image at addr 0x%08lx ...\n", addr);

    do_bootm(cmdtp, 0, 1, local_args);
    return 1;
  }
  return 0;
}

int do_nandboot(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
  char *boot_device = NULL;
  int idx=0;
  ulong addr=0, offset = 0;
#if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)
  struct mtd_device *dev=NULL;
  struct part_info *part=NULL;
  u8 pnum=0;

  if (argc >= 2) {
    char *p = (argc == 2) ? argv[1] : argv[2];
    if (!(str2long(p, &addr)) && (mtdparts_init() == 0) &&
        (find_dev_and_part(p, &dev, &pnum, &part) == 0)) {
      if (dev->id->type != MTD_DEV_TYPE_NAND) {
        puts("Not a NAND device\n");
        return 1;
      }
      if (argc > 3)
        goto usage;
      if (argc == 3)
        addr = simple_strtoul(argv[2], NULL, 16);
      else
        addr = CFG_LOAD_ADDR;
      return nand_load_image(cmdtp, &nand_info[dev->id->num],
                 part->offset, addr, argv[0]);
    }
  }
#endif

  switch (argc) {
  case 1:
		addr = CFG_LOAD_ADDR;
    boot_device = getenv("bootdevice");
    break;
  case 2:
    addr = simple_strtoul(argv[1], NULL, 16);
    boot_device = getenv("bootdevice");
    break;
  case 3:
    addr = simple_strtoul(argv[1], NULL, 16);
    boot_device = argv[2];
    break;
  case 4:
    addr = simple_strtoul(argv[1], NULL, 16);
    boot_device = argv[2];
    offset = simple_strtoul(argv[3], NULL, 16);
    break;
  default:
#if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)
usage:
#endif
    printf("Usage:\n%s\n", cmdtp->usage);
    SHOW_BOOT_PROGRESS(-1);
    return 1;
  }

  if (!boot_device) {
    puts("\n** No boot device **\n");
    SHOW_BOOT_PROGRESS(-1);
    return 1;
  }

  idx = simple_strtoul(boot_device, NULL, 16);

	if (idx < 0 || idx >= CFG_MAX_NAND_DEVICE || !nand_info[idx].name) {
    printf("\n** Device %d not available\n", idx);
    SHOW_BOOT_PROGRESS(-1);
    return 1;
  }

  return nand_load_image(cmdtp, &nand_info[idx], offset, addr, argv[0]);
}



int do_nandclk(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
    ushort mode=0;
    uint nandclk=0;
    uint shownandclk=0;

    if ((argc < NAND_CLK_LEN) || (argc > NAND_CLK_LEN))
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    mode = simple_strtoul(argv[1], NULL, 16);

    switch (mode)
    {
        #if defined(CONFIG_TITANIA8) || defined(CONFIG_TITANIA9) || defined(CONFIG_JANUS2)

        case 0:
      nandclk = NFIE_CLK_5_4M;
      shownandclk = 5;
            break;
        case 1:
      nandclk = NFIE_CLK_27M;
      shownandclk = 27;
            break;
        case 2:
      nandclk=NFIE_CLK_32M;
      shownandclk=32;
            break;
        case 3:
      nandclk = NFIE_CLK_43M;
      shownandclk = 43;
      break;

        #elif defined(CONFIG_TITANIA12) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_AMBER6) || defined(CONFIG_AMBER7)


    case 0:
      nandclk = NFIE_CLK_5_4M;
      shownandclk = 5;
      break;
    case 1:
      nandclk=NFIE_CLK_27M;
      shownandclk=27;
            break;
    case 2:
      nandclk = NFIE_CLK_32M;
      shownandclk = 32;
      break;
    case 3:
      nandclk = NFIE_CLK_36M;
      shownandclk = 36;
      break;
        case 4:
      nandclk = NFIE_CLK_40M;
      shownandclk = 40;
            break;
        case 5:
      nandclk = NFIE_CLK_43M;
      shownandclk = 43;
      break;

        #if defined(CONFIG_AMBER1) || defined(CONFIG_AMBER6) || defined(CONFIG_AMBER7)


        case 6:
            nandclk = NFIE_CLK_54M;
            shownandclk = 54;
            break;
        case 7:
            nandclk = NFIE_CLK_62M;
            shownandclk = 62;
            break;
        case 8:
            nandclk = NFIE_CLK_72M;
            shownandclk = 72;
            break;
        case 9:
            nandclk = NFIE_CLK_80M;
            shownandclk = 80;
            break;
        case 10:
            nandclk = NFIE_CLK_86M;
            shownandclk = 86;
            break;
        #endif

        #elif defined(CONFIG_AMBER5)


        case 0:
            nandclk = NFIE_CLK_300K;
            shownandclk = 300;
            break;
        case 1:
            nandclk = NFIE_CLK_20M;
            shownandclk = 20;
            break;
        case 2:
            nandclk = NFIE_CLK_24M;
            shownandclk = 24;
            break;
        case 3:
            nandclk=NFIE_CLK_27M;
            shownandclk=27;
            break;
        case 4:
            nandclk = NFIE_CLK_32M;
            shownandclk = 32;
            break;
        case 5:
            nandclk = NFIE_CLK_36M;
            shownandclk = 36;
            break;
        case 6:
            nandclk = NFIE_CLK_40M;
            shownandclk = 40;
            break;
        case 7:
            nandclk = NFIE_CLK_43M;
            shownandclk = 43;
            break;
        case 8:
            nandclk = NFIE_CLK_48M;
            shownandclk = 48;
            break;
        case 9:
            nandclk = NFIE_CLK_54M;
            shownandclk = 54;
            break;
        case 10:
            nandclk = NFIE_CLK_62M;
            shownandclk = 62;
            break;
        case 11:
            nandclk = NFIE_CLK_72M;
            shownandclk = 72;
            break;
        case 12:
            nandclk = NFIE_CLK_80M;
            shownandclk = 80;
            break;
        case 13:
            nandclk = NFIE_CLK_86M;
            shownandclk = 86;
            break;
        #elif defined(CONFIG_AMBER2)
        case 0:
            nandclk = NFIE_CLK_300K;
            shownandclk = 300;
            break;
        case 1:
            nandclk = NFIE_CLK_20M;
            shownandclk = 20;
            break;
        case 2:
            nandclk = NFIE_CLK_24M;
            shownandclk = 24;
            break;
        case 3:
            nandclk=NFIE_CLK_27M;
            shownandclk=27;
            break;
        case 4:
            nandclk = NFIE_CLK_32M;
            shownandclk = 32;
            break;
        case 5:
            nandclk = NFIE_CLK_36M;
            shownandclk = 36;
            break;
        case 6:
            nandclk = NFIE_CLK_40M;
            shownandclk = 40;
            break;
        case 7:
            nandclk = NFIE_CLK_43M;
            shownandclk = 43;
            break;
        case 8:
            nandclk = NFIE_CLK_48M;
            shownandclk = 48;
            break;
        case 9:
            nandclk = NFIE_CLK_54M;
            shownandclk = 54;
            break;
        case 10:
            nandclk = NFIE_CLK_62M;
            shownandclk = 62;
            break;
        case 11:
            nandclk = NFIE_CLK_72M;
            shownandclk = 72;
            break;
        case 12:
            nandclk = NFIE_CLK_80M;
            shownandclk = 80;
            break;
        case 13:
            nandclk = NFIE_CLK_86M;
            shownandclk = 86;
            break;
        #elif  defined(CONFIG_AMETHYST)
        case 0:
            nandclk = NFIE_CLK_300K;
            shownandclk = 300;
            break;
        case 1:
            nandclk = NFIE_CLK_12M;
            shownandclk = 12;
            break;
        case 2:
            nandclk = NFIE_CLK_20M;
            shownandclk = 20;
            break;
        case 3:
            nandclk = NFIE_CLK_24M;
            shownandclk = 24;
            break;
        case 4:
            nandclk=NFIE_CLK_27M;
            shownandclk=27;
            break;
        case 5:
            nandclk = NFIE_CLK_32M;
            shownandclk = 32;
            break;
        case 6:
            nandclk = NFIE_CLK_36M;
            shownandclk = 36;
            break;
        case 7:
            nandclk = NFIE_CLK_40M;
            shownandclk = 40;
            break;
        case 8:
            nandclk = NFIE_CLK_43_2M;
            shownandclk = 43;
            break;
        case 9:
            nandclk = NFIE_CLK_48M;
            shownandclk = 48;
            break;
        case 10:
            nandclk = NFIE_CLK_54M;
            shownandclk = 54;
            break;
        case 11:
            nandclk = NFIE_CLK_62M;
            shownandclk = 62;
            break;
        case 12:
            nandclk = NFIE_CLK_72M;
            shownandclk = 72;
            break;
        case 13:
            nandclk = NFIE_CLK_80M;
            shownandclk = 80;
            break;
        case 14:
            nandclk = NFIE_CLK_86M;
            shownandclk = 86;
            break;
    #else

      printf("Please modify cmd.nand.c for new nfie clk\n");
    #endif

        default:
      printf("Please select correct clock!");
            return 1;
  }

    #if defined(CONFIG_TITANIA8) || defined(CONFIG_TITANIA9) || defined(CONFIG_JANUS2) || \
        defined(CONFIG_TITANIA12) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || \
        defined(CONFIG_AMBER6) || defined(CONFIG_AMBER7) || defined(CONFIG_AMBER5) || \
        defined(CONFIG_AMETHYST)
        printf ("NAND Clock mode :%d and Clock:%d MHz\n", mode, shownandclk);
    #else
    printf ("NAND Clock mode :%d and Clock:%d %cHz\n", mode, shownandclk, (mode<2) ? 'K' : 'M');
    #endif


  nand_clock_setting(nandclk);

    return 0;
}

/*
 * This function tag a number of blocks which located at the end of UBI partition
 * as bad block on purpose (rsv bad). So UBI will not use them temproarily.
 *
 * It can also erase bad block table (rsv good). So the tags setted above will be cleared.
 *
 * The funtion and corresponding commond are only uesd for nand programming
 * in mass production.
 */
int do_nand_write_rsv(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    #if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)
  struct mtd_device *dev=NULL;
  struct part_info *part=NULL;
       nand_erase_options_t erase_opts;
       nand_info_t *nand=NULL;
       ulong blockrsv_num=0, rsvsize=0, rsvstart=0;
       u8 pnum=0;
       int i=0, ret=0;

       memset(&erase_opts, 0 , sizeof(erase_opts));
       nand = &nand_info[0];
       blockrsv_num = 8;  // if user dose not provide the parameter, use defaule value
        if (argc < 2 || ((2 == argc) && (0 != strcmp(argv[1], "good"))))
        {
            printf("Input error!Command format is: rsv bad [rsv_num] / rsv good\r\n");
            return -1;
        }

        if (0 == strcmp(argv[1], "bad"))
        {
            // find the offset and size of UBI partition
            if ((mtdparts_init() == 0) &&
                (find_dev_and_part("UBI", &dev, &pnum, &part) == 0)) {
                if (dev->id->type != MTD_DEV_TYPE_NAND) {
                  puts("not a NAND device\n");
                  return -1;
                }
            }
            if(part == NULL)
            {
                printf("%s: Error: null part, at %d\n", __func__, __LINE__);
                return -1;
            }
            if(dev == NULL)
            {
                printf("%s: Error: null dev, at %d\n", __func__, __LINE__);
                return -1;
            }

           /*
            * |-----------------------------------------|-----------------|
            * UBI start(offset)                                  reserved start     UBI end(offset + size)
            */
            if (3 == argc)
            {
                if (!str2long(argv[2], &blockrsv_num))
                {
                    return -1;
                }
            }
            rsvsize = blockrsv_num * nand->erasesize;
            rsvstart = part->offset + part->size - rsvsize;

            for (i = 0; i < blockrsv_num; i++)
            {
            #if 0
                memset(&erase_opts, 0, sizeof(erase_opts));
                nand->oobblock;
                erase_opts.length = nand->erasesize;
                erase_opts.offset = rsvstart + i * nand->erasesize;
                erase_opts.quiet = 0;
                erase_opts.jffs2  = 0;

                ret = nand->block_isbad(nand, write_opts.offset);

                if (ret < 0)
                {
                    printf("Bad block check failed\n");
                    return -1;
                }
                if (ret == 1)
                {
                    if (!write_opts.quiet)
                    {
                        printf("\rBad block at 0x%lx "
                               "in erase block from\n",
                               (long) (write_opts.offset));

                        continue;
                    }
                }
            #endif

                ret = nand->block_markbad(nand, (rsvstart + i * nand->erasesize));
                if (ret)
                {
                    printf("Can not mark %lu bad, error %d\r\n", erase_opts.offset, ret);
                    return -1;
                }

                // erase first
                //ret = nand_erase_opts(nand, &erase_opts);
               //printf("%4x %s\n",i,ret ? "ERROR" : "OK");

                //flush_cache((ulong)buf, write_opts.length);
                //ret = nand_write_opts(nand, &write_opts);
            }
        }

        if  (0 == strcmp(argv[1], "good"))
        {
            memset(&erase_opts, 0, sizeof(erase_opts));
            erase_opts.length = (ulong)(nand->bbt_block_num * nand->erasesize);
            erase_opts.offset = (ulong)(nand->size - nand->bbt_block_num * nand->erasesize);
            erase_opts.quiet = 0;
            erase_opts.jffs2  = 0;
            erase_opts.scrub = 1;

            // erase bad block table and the bbt will be recreated by MBoot
            nand_erase_opts(nand, &erase_opts);
        }

        return 0;
    #else
        return 0;
    #endif
}

#ifdef CONFIG_BOOT_FROM_NAND
extern U32 drvNAND_WriteCIS_from_DRAM(U32 nandInfoAddr, U32 partInfoAddr);
extern U32 drvNAND_Write_DDRT_ML(U32 u32_DDR_Off, U8* pu8DDR_Buf, U32 u32_DDRTByteCount, U32 u32_ML_Off, U8* pu8ML_Buf, U32 u32_MLByteCount);
int do_nand_write_cis(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    U32 nandInfoAddr=0;
    U32 partInfoAddr=0;
    U32 Buf_Addr = 0;
    U32 u32_CIS_off, u32_DDR_off, u32_DDR_Len,
    u32_DDR_NAND_off, u32_ML_off, u32_ML_Len, u32_ML_NAND_off;
    U32 u32_Err = 0;
    if(argc<3){
        printf("[ERROR] Too few parameters\n");
        return 1;
    }
    printf("start write bootimage.bin\n");
    Buf_Addr = simple_strtoul(argv[1], NULL, 16);//argv[1];
    u32_CIS_off = simple_strtoul(argv[2], NULL, 16);//argv[1];
    nandInfoAddr= Buf_Addr + u32_CIS_off;
    partInfoAddr= nandInfoAddr + 0x800;
    //printf("nandInfoAddr=0X%x\n",nandInfoAddr);
    //printf("partInfoAddr=0X%x\n",partInfoAddr);

    u32_Err = drvNAND_WriteCIS_from_DRAM(nandInfoAddr,partInfoAddr);

    u32_DDR_off = simple_strtoul(argv[3], NULL, 16);
    u32_DDR_NAND_off = simple_strtoul(argv[4], NULL, 16);
    u32_DDR_Len = simple_strtoul(argv[5], NULL, 16);
    u32_ML_off = simple_strtoul(argv[6], NULL, 16);
    u32_ML_NAND_off = simple_strtoul(argv[7], NULL, 16);
    u32_ML_Len = simple_strtoul(argv[8], NULL, 16);

    if(u32_Err){
        printf("write bootimage.bin error,u32_Err=%ld\n",u32_Err);
        return u32_Err;
    }
    u32_Err = drvNAND_Write_DDRT_ML(u32_DDR_NAND_off, (U8*)(Buf_Addr+u32_DDR_off), u32_DDR_Len,
    u32_ML_NAND_off, (U8*)(Buf_Addr+u32_ML_off), u32_ML_Len);
     printf("write bootimage.bin end\n");

    return u32_Err;
}
#endif
/* read out ubi partition and store it to usb disk */
int do_ubi_partition_mkbin(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
#if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)
    struct mtd_device *dev=NULL;
    struct part_info *part=NULL;
    nand_info_t *nand=NULL;
    nand_read_options_t opts;
    u8 pnum=0;
    int ret=0;

    run_command("usb start", 0);

#if defined(KL_IN_MTD)
    if ((mtdparts_init() == 0) &&
        (find_dev_and_part("KL", &dev, &pnum, &part) == 0)) {
        if (dev->id->type != MTD_DEV_TYPE_NAND) {
          puts("not a NAND device\n");
          return -1;
        }
    }
    if(part == NULL)
    {
        printf("%s: Error: null part, at %d\n", __func__, __LINE__);
        return -1;
    }
    if(dev == NULL)
    {
        printf("%s: Error: null dev, at %d\n", __func__, __LINE__);
        return -1;
    }
    nand = &nand_info[dev->id->num];

    if (part->size & (nand->erasesize - 1))
    {
        printf("KL partition size is not aligned with nand block size\r\n    part size=0x%x, block size=0x%x\r\n",
                    part->size, nand->erasesize);

        return -1;
    }
    if (part->offset & (nand->erasesize - 1))
    {
        printf("KL partition offset is not aligned with nand block size\r\n    part offset=0x%x, block size=0x%x\r\n",
                    part->offset, nand->erasesize);

        return -1;
    }

    memset(&opts, 0, sizeof(opts));
    opts.buffer = (u_char*)UPGRADE_BUFFER_ADDR;
    opts.fatfile_name = "part0.bin";
    opts.length = (part->size / nand->oobblock) * (nand->oobblock + nand->oobsize);
    opts.offset = part->offset;
    opts.quiet      = 0;
    opts.readoob = 1;
    nand_fs_format = NAND_FS_FORMAT_MKBIN;

    printf("KL partition: \r\n    offset=0x%x,\r\n    size=0x%x(by main:%d), size=0x%x(by main+spare:%d),\r\n    off block=%d, size block=%d\r\n",
                part->offset, part->size, nand->oobblock, ((part->size / nand->oobblock) * (nand->oobblock + nand->oobsize)),
                (nand->oobblock + nand->oobsize), (part->offset / nand->erasesize), (part->size / nand->erasesize));

    ret = nand_read_opts(nand, &opts);
    flush_cache((ulong)opts.buffer, opts.length);
#endif

    if ((mtdparts_init() == 0) &&
        (find_dev_and_part("UBI", &dev, &pnum, &part) == 0)) {
        if (dev->id->type != MTD_DEV_TYPE_NAND) {
          puts("not a NAND device\n");
          return -1;
        }
    }
    nand = &nand_info[dev->id->num];

    if (part->size & (nand->erasesize - 1))
    {
        printf("UBI partition size is not aligned with nand block size\r\n    part size=0x%x, block size=0x%x\r\n",
                    part->size, nand->erasesize);

        return -1;
    }
    if (part->offset & (nand->erasesize - 1))
    {
        printf("UBI partition offset is not aligned with nand block size\r\n    part offset=0x%x, block size=0x%x\r\n",
                    part->offset, nand->erasesize);

        return -1;
    }

    memset(&opts, 0, sizeof(opts));
    opts.buffer = (u_char*)UPGRADE_BUFFER_ADDR;
    opts.fatfile_name = "part1.bin";
    opts.length = (part->size / nand->oobblock) * (nand->oobblock + nand->oobsize);
    opts.offset = part->offset;
    opts.quiet      = 0;
    opts.readoob = 1;
    nand_fs_format = NAND_FS_FORMAT_MKBIN;
    printf("UBI partition: \r\n    offset=0x%x,\r\n    size=0x%x(by main:%d), size=0x%x(by main+spare:%d),\r\n    off block=%d, size block=%d\r\n",
                part->offset, part->size, nand->oobblock, ((part->size / nand->oobblock) * (nand->oobblock + nand->oobsize)),
                (nand->oobblock + nand->oobsize), (part->offset / nand->erasesize), (part->size / nand->erasesize));

    ret = nand_read_opts(nand, &opts);
    flush_cache((ulong)opts.buffer, opts.length);

    return ret;
#else
    return 0;
#endif
}

/* read out ubi partition and environment parameter at once and store it to usb disk */
int do_nand_mkbin2(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    run_command("ubibin", 0);

#if defined(CONFIG_ENV_IS_IN_NAND)
    run_command("envbin", 0);
#endif

    return 0;
}

/* print ubi partition and environment parameter information */
int do_print_nandbin_info(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
#if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)
    struct mtd_device *dev=NULL;
    struct part_info *part=NULL;
    nand_info_t *nand=NULL;
    u8 pnum=0;
    u32 page_size=0;
    u32 off_blk=0, size_blk=0;
    u32 end=0, end_blk=0;

#if defined(KL_IN_MTD)
    if ((mtdparts_init() == 0) &&
        (find_dev_and_part("KL", &dev, &pnum, &part) == 0)) {
        if (dev->id->type != MTD_DEV_TYPE_NAND) {
          puts("not a NAND device\n");
          return -1;
        }
    }
    if(part == NULL)
    {
        printf("%s: Error: null part, at %d\n", __func__, __LINE__);
        return -1;
    }
    if(dev == NULL)
    {
        printf("%s: Error: null dev, at %d\n", __func__, __LINE__);
        return -1;
    }
    nand = &nand_info[dev->id->num];
    page_size = nand->oobblock + nand->oobsize;

    if (part->size & (nand->erasesize - 1))
    {
        printf("Warning: KL partition size is not aligned with nand block size\r\n    part size=0x%x, block size=0x%x\r\n",
                    part->size, nand->erasesize);
    }
    if (part->offset & (nand->erasesize - 1))
    {
        printf("Warning: KL partition offset is not aligned with nand block size\r\n    part offset=0x%x, block size=0x%x\r\n",
                    part->offset, nand->erasesize);
    }

    off_blk = part->offset / nand->erasesize;
    size_blk = part->size / nand->erasesize;
    end = part->offset + part->size - 1;
    end_blk = (part->offset + part->size) / nand->erasesize - 1;

    printf("KL partition: \r\n    offset=0x%x(by main:%d),  offset=0x%x(by main+spare:%d)\r\n",
                part->offset, nand->oobblock,
                ((part->offset / nand->oobblock) * page_size), page_size);
    printf("    offset block=%d(Hex:0x%x)\r\n",
                off_blk, off_blk);
    printf("    size=0x%x(main),  size=0x%x(main+spare)\r\n",
                part->size, ((part->size / nand->oobblock) * page_size));
    printf("    size block=%d(Hex:0x%x)\r\n",
                size_blk, size_blk);
    printf("    end=0x%x(main),  end=0x%x(main+spare)\r\n",
                end, (((end + 1) / nand->oobblock) * page_size - 1));
    printf("    end block=%d(Hex:0x%x)\r\n\n",
                end_blk, end_blk);
#endif

    if ((mtdparts_init() == 0) &&
        (find_dev_and_part("UBI", &dev, &pnum, &part) == 0)) {
        if (dev->id->type != MTD_DEV_TYPE_NAND) {
          puts("not a NAND device\n");
          return -1;
        }
    }
    nand = &nand_info[dev->id->num];

    if (part->size & (nand->erasesize - 1))
    {
        printf("Warning: UBI partition size is not aligned with nand block size\r\n    part size=0x%x, block size=0x%x\r\n",
                    part->size, nand->erasesize);
    }
    if (part->offset & (nand->erasesize - 1))
    {
        printf("Warning: UBI partition offset is not aligned with nand block size\r\n    part offset=0x%x, block size=0x%x\r\n",
                    part->offset, nand->erasesize);
    }

    off_blk = part->offset / nand->erasesize;
    size_blk = part->size / nand->erasesize;
    end = part->offset + part->size - 1;
    end_blk = (part->offset + part->size) / nand->erasesize - 1;

    printf("UBI partition: \r\n    offset=0x%x(by main:%d),  offset=0x%x(by main+spare:%d)\r\n",
                part->offset, nand->oobblock,
                ((part->offset / nand->oobblock) * page_size), page_size);
    printf("    offset block=%d(Hex:0x%x)\r\n",
                off_blk, off_blk);
    printf("    size=0x%x(main),  size=0x%x(main+spare)\r\n",
                part->size, ((part->size / nand->oobblock) * page_size));
    printf("    size block=%d(Hex:0x%x)\r\n",
                size_blk, size_blk);
    printf("    end=0x%x(main),  end=0x%x(main+spare)\r\n",
                end, (((end + 1) / nand->oobblock) * page_size - 1));
    printf("    end block=%d(Hex:0x%x)\r\n\n",
                end_blk, end_blk);

#if defined(CONFIG_ENV_IS_IN_NAND)
    u32 env_off=0, env_off_blk=0, env_size=0, env_size_blk=0;
    u32 env_end=0, env_end_blk=0;

    env_off = CONFIG_ENV_OFFSET;
    env_size = CONFIG_ENV_SIZE;
    env_end = env_off + env_size - 1;

    if (env_size & (nand->erasesize - 1))
    {
        printf("Warning: Environment parameter size is not align with nand block size\r\n    part size=0x%x, block size=0x%x\r\n",
                    env_size, nand->erasesize);
    }
    if (env_off & (nand->erasesize - 1))
    {
        printf("Warning: Environment parameter offset is not align with nand block size\r\n    part offset=0x%x, block offset=0x%x\r\n",
                    env_off, nand->erasesize);
    }

    env_off_blk = env_off / nand->erasesize;
    env_size_blk = env_size / nand->erasesize;
    env_end_blk = (env_off + env_size) / nand->erasesize - 1;

    printf("Environment parameter: \r\n    offset=0x%x(by main:%d),  offset=0x%x(by main+spare:%d)\r\n",
                env_off, nand->oobblock,
                ((env_off / nand->oobblock) * page_size), page_size);
    printf("    offset block=%d(Hex:0x%x)\r\n",
                env_off_blk, env_off_blk);
    printf("    size=0x%x(main),  size=0x%x(main+spare)\r\n",
                env_size, ((env_size / nand->oobblock) * page_size));
    printf("    size block=%d(Hex:0x%x)\r\n",
                env_size_blk, env_size_blk);
    printf("    end=0x%x(main),  end=0x%x(main+spare)\r\n",
                env_end, (((env_end + 1) / nand->oobblock) * page_size - 1));
    printf("    end block=%d(Hex:0x%x)\r\n\n",
                env_end_blk, env_end_blk);
#endif

    printf("Nand Flash basic information: \r\n    main=%d bytes(Hex:0x%x),  spare=%d bytes(Hex:0x%x)\r\n",
                nand->oobblock, nand->oobblock, nand->oobsize, nand->oobsize);
    printf("    main+spare=%d bytes(Hex:0x%x),  page number per block=%d\r\n",
                page_size, page_size, (nand->erasesize / nand->oobblock));
    printf("    block size=0x%x(%dKB, main),  block size=0x%x(main+spare)\r\n\n",
                nand->erasesize, (nand->erasesize >> 10), ((nand->erasesize / nand->oobblock) * page_size));

    return 0;
#endif
    return -1;
}

/* write the length of size 0xff to the file pointed by name, append mode */
int write_empty_fatfile(char *name, ulong size)
{
#if (CONFIG_COMMANDS & CFG_CMD_FAT)
    block_dev_desc_t *dev_desc=NULL;
    int dev=0;
    int part=1;
    char *ep=NULL;
    int i=0, fd=0;
    u_char *buf_end = (u_char *)UPGRADE_BUFFER_ADDR;

    /* Write to usb for binary file making
    Initialize something and open usb file */
    dev = (int)simple_strtoul ("0", &ep, 16);
    dev_desc=get_dev("usb", dev);
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
        printf ("\n** Unable to use %s %d:%d for fatwrite **\n","usb",dev, part);
        return -1;
    }

    fd = file_fat_open(name, "a");
    if (fd < 0)
    {
        printf("Invalid file descriptor: %d\r\n", fd);
        return -1;
    }

    for (i = 0; i < READ_BUFFER_LEN; i++)
    {
        buf_end[i] = 0xff;
    }
    flush_cache((ulong)buf_end, READ_BUFFER_LEN);

    while (size >=  READ_BUFFER_LEN)
    {
        file_fat_write(fd, (unsigned char *) buf_end, READ_BUFFER_LEN);
        size = size - READ_BUFFER_LEN;
    }

    if (size > 0)
    {
        file_fat_write(fd, (unsigned char *) buf_end, size);
    }

    file_fat_close(fd);
#if 0 // can not enter, dead code
    if(fd == -1) {
        printf("\n** Unable to write \"%s\" from %s %d:%d **\n", name, "usb", dev, part);
       return -1;
    }
#endif

#endif

    return 0;
}

/* read out the whole contents of nand and store it to usb disk */
int do_nand_mkbin(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
#if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)
    struct mtd_device *dev=NULL;
    struct part_info *part=NULL;
    nand_info_t *nand=NULL;
    nand_read_options_t opts;
    ulong len=0;
    u8 pnum=0;
    int ret=0;

#if defined(KL_IN_MTD)
    if ((mtdparts_init() == 0) &&
        (find_dev_and_part("KL", &dev, &pnum, &part) == 0)) {
        if (dev->id->type != MTD_DEV_TYPE_NAND) {
          puts("not a NAND device\n");
          return -1;
        }
    }
#else
    if ((mtdparts_init() == 0) &&
        (find_dev_and_part("UBI", &dev, &pnum, &part) == 0)) {
        if (dev->id->type != MTD_DEV_TYPE_NAND) {
          puts("not a NAND device\n");
          return -1;
        }
    }
#endif
    if(part == NULL)
    {
        printf("%s: Error: null part, at %d\n", __func__, __LINE__);
        return -1;
    }
    if(dev == NULL)
    {
        printf("%s: Error: null dev, at %d\n", __func__, __LINE__);
        return -1;
    }
    nand = &nand_info[dev->id->num];
    nand_fs_format = NAND_FS_FORMAT_MKBIN;

#if 0
    if (part->size & (nand->erasesize - 1))
    {
        printf("UBI partition size is not aligned with nand block size\r\n    part size=0x%x, block size=0x%x\r\n",
                    part->size, nand->erasesize);

        return -1;
    }
    if (part->offset & (nand->erasesize - 1))
    {
        printf("UBI partition offset is not aligned with nand block size\r\n    part offset=0x%x, block size=0x%x\r\n",
                    part->offset, nand->erasesize);

        return -1;
    }

    printf("UBI partition: \r\n    offset=0x%x,\r\n    size=0x%x(by main:%d), size=0x%x(by main+spare:%d),\r\n    off block=%d, size block=%d\r\n\n",
            part->offset, part->size, nand->oobblock, ((part->size / nand->oobblock) * (nand->oobblock + nand->oobsize)),
            (nand->oobblock + nand->oobsize), (part->offset / nand->erasesize), (part->size / nand->erasesize));
#endif

    run_command("usb start", 0);

    /* Firstly, write an empty head */
    len = ((part->offset / nand->oobblock) * (nand->oobblock + nand->oobsize));
    ret = write_empty_fatfile("nanddump.bin", len);
    if (ret)
        return ret;

    memset(&opts, 0, sizeof(opts));
    opts.buffer = (u_char*)UPGRADE_BUFFER_ADDR;
    opts.fatfile_name = "nanddump.bin";
    opts.length = (part->size / nand->oobblock) * (nand->oobblock + nand->oobsize);
    opts.offset = part->offset;
    opts.quiet      = 0;
    opts.readoob = 1;

    /* Secondly, write UBI partition */
    ret = nand_read_opts(nand, &opts);
    flush_cache((ulong)opts.buffer, opts.length);

#if defined(KL_IN_MTD)
    if ((mtdparts_init() == 0) &&
        (find_dev_and_part("UBI", &dev, &pnum, &part) == 0)) {
        if (dev->id->type != MTD_DEV_TYPE_NAND) {
          puts("not a NAND device\n");
          return -1;
        }
    }
    nand = &nand_info[dev->id->num];

    //memset(&opts, 0, sizeof(opts));
    //opts.buffer = (u_char*)UPGRADE_BUFFER_ADDR;
    //opts.fatfile_name = "nanddump.bin";
    opts.length = (part->size / nand->oobblock) * (nand->oobblock + nand->oobsize);
    opts.offset = part->offset;
    //opts.quiet      = 0;
    //opts.readoob = 1;

    /* Secondly, write UBI partition */
    ret = nand_read_opts(nand, &opts);
    flush_cache((ulong)opts.buffer, opts.length);
#endif

#if defined(CONFIG_ENV_IS_IN_NAND)
    ulong env_size = CONFIG_ENV_SIZE;
    ulong env_off = CONFIG_ENV_OFFSET;
    int env_blk_size = CONFIG_ENV_SIZE / nand->erasesize;
    int i=0, good_blk_num = 0;

    if (env_size & (nand->erasesize - 1))
    {
        printf("Environment parameter size is not align with nand block size\r\n    part size=0x%x, block size=0x%x\r\n",
                    env_size, nand->erasesize);

        return -1;
    }
    if (env_off & (nand->erasesize - 1))
    {
        printf("Environment parameter offset is not align with nand block size\r\n    part offset=0x%x, block offset=0x%x\r\n",
                    env_off, nand->erasesize);

        return -1;
    }

    printf("Environment parameter: \r\n    offset=0x%x,\r\n    size=0x%x(by main:%d), size=0x%x(by main+spare:%d),\r\n    off block=%d, size block=%d\r\n",
                env_off, env_size, nand->oobblock, ((env_size / nand->oobblock) * (nand->oobblock + nand->oobsize)),
                (nand->oobblock + nand->oobsize), (env_off / nand->erasesize), (env_size / nand->erasesize));

    for (i = 0; i < CONFIG_ENV_BLOCK_NUM; i++)
    {
        if (!nand->block_isbad(nand, (env_off + i * nand->erasesize)))
            good_blk_num++;
    }
    if (good_blk_num < env_blk_size)
    {
        printf("Read env error, change a board: good block number=%d,need number=%d.\r\n", good_blk_num, env_blk_size);

        return -1;
    }

    /* Thirdly, write the middle empty file */
    len = ((env_off - part->offset - part->size) / nand->oobblock) * (nand->oobblock + nand->oobsize);
    ret = write_empty_fatfile(opts.fatfile_name, len);
    if (ret)
        return ret;

    opts.buffer = (u_char*) UPGRADE_BUFFER_ADDR;
    opts.length = (env_size / nand->oobblock) * (nand->oobblock + nand->oobsize);
    opts.offset = env_off;

    /* Finally, write environment parameter */
    ret = nand_read_opts(nand, &opts);
    flush_cache((ulong)opts.buffer, opts.length);
#endif
    return ret;
#endif
    return -1;
}

/* read ubi partition file and restore it to nand */
int do_ubi_partition_restore(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
#if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)
    struct mtd_device *dev=NULL;
    struct part_info *part=NULL;
    nand_info_t *nand=NULL;
    nand_write_options_t opts;
    u8 pnum=0;
    int ret=0;

    if ((mtdparts_init() == 0) &&
        (find_dev_and_part("UBI", &dev, &pnum, &part) == 0)) {
        if (dev->id->type != MTD_DEV_TYPE_NAND) {
          puts("not a NAND device\n");
          return -1;
        }
    }
    if(part == NULL)
    {
        printf("%s: Error: null part, at %d\n", __func__, __LINE__);
        return -1;
    }
    if(dev == NULL)
    {
        printf("%s: Error: null dev, at %d\n", __func__, __LINE__);
        return -1;
    }
    nand = &nand_info[dev->id->num];

    if (part->size & (nand->erasesize - 1))
    {
        printf("UBI partition size is not aligned with nand block size\r\n    part size=0x%x, block size=0x%x\r\n",
                    part->size, nand->erasesize);

        return -1;
    }
    if (part->offset & (nand->erasesize - 1))
    {
        printf("UBI partition offset is not aligned with nand block size\r\n    part offset=0x%x, block size=0x%x\r\n",
                    part->offset, nand->erasesize);

        return -1;
    }

    memset(&opts, 0, sizeof(opts));
    opts.buffer = (u_char*)UPGRADE_BUFFER_ADDR;
    opts.fatfile_name = "part1.bin";
    opts.length = part->size;
    opts.offset = part->offset;
    opts.blockalign = 1;
    opts.quiet      = 0;
    opts.pad  = 1;
    nand_fs_format = NAND_FS_FORMAT_MKBIN;

    printf("UBI partition: \r\n    offset=0x%x,\r\n    size=0x%x(by main:%d), size=0x%x(by main+spare:%d),\r\n    off block=%d, size block=%d\r\n",
                part->offset, part->size, nand->oobblock, ((part->size / nand->oobblock) * (nand->oobblock + nand->oobsize)),
                (nand->oobblock + nand->oobsize), (part->offset / nand->erasesize), (part->size / nand->erasesize));

    run_command("usb start", 0);
    flush_cache((ulong)opts.buffer, opts.length);
    ret = nand_write_opts(nand, &opts);

    return ret;
#else
    return 0;
#endif
}

/* read nand bin file and restore it to nand */
int do_nand_bin_restore(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
#if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)
    nand_info_t *nand=NULL;
    nand_write_options_t opts;
    int ret=0;
    ulong nandsize_oob, imagesize;
    ulong offset;

    nand = &nand_info[0];
    nandsize_oob = (nand->size / nand->oobblock) * (nand->oobblock + nand->oobsize);
    memset(&opts, 0, sizeof(opts));
    opts.buffer = (u_char*)UPGRADE_BUFFER_ADDR;
    opts.length = 0x1EF0000;
    opts.blockalign = 1;
    opts.quiet      = 0;
//    opts.pad  = 1;
//    opts.writeoob = 1;
//    opts.noecc = 1;
//    opts.autoplace = 1;
    nand_fs_format = NAND_FS_FORMAT_MKBIN;

    run_command("usb start", 0);

#if (CONFIG_COMMANDS & CFG_CMD_FAT)
    block_dev_desc_t *dev_desc=NULL;
    int dev=0;
    int part=1;
    char *ep;
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

    if(argc < 3){
        opts.fatfile_name = "nanddump.bin";
        offset = 0;
    }
    else if(simple_strtoul(argv[2], NULL, 16) < nandsize_oob){
        opts.fatfile_name = argv[1];
        offset = simple_strtoul(argv[2], NULL, 16);
    }
    else{
        printf("Please input the right args!\n");
        return -1;
    }

    imagesize = 0; //file_fat_filesize(opts.fatfile_name);
    if(imagesize > (nandsize_oob - offset)){
        printf("\n Error: image too big, check the nand size and the image file name!\n");
        return -1;
    }

    for(opts.offset = 0; opts.offset < imagesize;){
        memset(opts.buffer, 0xff, opts.length);
        ret = _nand_write_opts(nand, &opts, offset);
        flush_cache((ulong)opts.buffer, opts.length);
        if(ret)
            return ret;
        opts.offset += opts.length;
        if(opts.offset + opts.length > imagesize)
            opts.length = imagesize - opts.offset;
    }
    return 0;
#else
    return 0;
#endif
}

int do_nand_raw_read(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    ulong nand_offset   = simple_strtoul(argv[1], NULL, 16);
    ulong nand_blk_num  = simple_strtoul(argv[2], NULL, 16);
    ulong buffer_offset = simple_strtoul(argv[3], NULL, 16);
    ulong buffer_size   = simple_strtoul(argv[4], NULL, 16);
    ulong total;
    int ret, uclean = 0;
    int i, env_blk_size, good_blk_num = 0;
    nand_info_t *nand;
    u_char *current_offset;

    nand = &nand_info[0];
    env_blk_size = buffer_size / nand->erasesize;

    printf("%s [Memory]0x%lx 0x%lx bytes from [Nand]0x%lx %ld blocks\r\n",__FUNCTION__,buffer_offset,buffer_size,nand_offset,nand_blk_num);

    //=====   alignment check   =====//
    if (buffer_size & (nand->erasesize - 1))
    {
        printf("buffer size is not align with nand block size\r\n    buffer size=0x%lx, block size=0x%x\r\n",
                    buffer_size, nand->erasesize);
        return -1;
    }
    if (nand_offset & (nand->erasesize - 1))
    {
        printf("nand offset is not align with nand block size\r\n    nand offset=0x%lx, block offset=0x%x\r\n",
                    nand_offset, nand->erasesize);

        return -1;
    }

    //=====   by pass bad block read process   =====//
    for (i = 0; i < nand_blk_num; i++)
    {
        if (nand->block_isbad(nand, nand_offset))
        {
           nand_offset += nand->erasesize;
           continue;
        }

        //=====   reading raw data from nand process   =====//
        total = nand->erasesize;
        current_offset = (u_char *)buffer_offset + good_blk_num * nand->erasesize;
        ret = nand_read(nand, nand_offset, &total, current_offset);
        flush_cache((ulong)current_offset, total);
        if ((ret && ret != -EUCLEAN) || total != nand->erasesize)
        {
           if (ret)
               return ret;
           else
               return -2;
        }
        if (ret == -EUCLEAN)
        {
           uclean = 1;
           printf("*Warning-Bit flip error, although we correct it! offs=0x%lx\n", nand_offset);
        }

        nand_offset += nand->erasesize;
        good_blk_num++;

        //=====   reading complete check   =====//
        if (good_blk_num == env_blk_size)
            break;
    }

    //=====   read block count check   =====//
    if (0 == good_blk_num)
    {
        printf("All the blocks reserved for backup are bad, no data is read!!!.\r\n");
        return -1;
    }
    if ((good_blk_num < env_blk_size) && (good_blk_num > 0))
    {
        printf("Only %d blocks of data is read because of bad block,\r\n%d blocks should be read.\r\n",
                        good_blk_num, env_blk_size);
        return 0;
    }

    puts ("done\n");
    return uclean;
}

int do_nand_raw_write(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    ulong nand_offset   = simple_strtoul(argv[1], NULL, 16);
    ulong nand_blk_num  = simple_strtoul(argv[2], NULL, 16);
    ulong buffer_offset = simple_strtoul(argv[3], NULL, 16);
    ulong buffer_size   = simple_strtoul(argv[4], NULL, 16);
    ulong total;
    int ret = 0;
    int i, env_blk_size, good_blk_num = 0;
    nand_info_t *nand;
    u_char *current_offset;

    nand = &nand_info[0];
    env_blk_size = buffer_size / nand->erasesize;

    printf("%s [Memory]0x%lx 0x%lx bytes to [Nand]0x%lx %ld blocks\r\n",__FUNCTION__,buffer_offset,buffer_size,nand_offset,nand_blk_num);

    //=====   alignment check   =====//
    if (buffer_size & (nand->erasesize - 1))
    {
        printf("buffer size is not align with nand block size\r\n    buffer size=0x%lx, block size=0x%x\r\n",
                    buffer_size, nand->erasesize);
    }
    if (nand_offset & (nand->erasesize - 1))
    {
        printf("nand offset is not align with nand block size\r\n    nand offset=0x%lx, block offset=0x%x\r\n",
                    nand_offset, nand->erasesize);
        return -1;
    }

    //=====   by pass bad block write process   =====//
    for (i = 0; i < nand_blk_num; i++)
    {
        if(nand->block_isbad(nand, nand_offset))
        {
            nand_offset += nand->erasesize;
            continue;
        }

        if (nand_erase(nand, nand_offset, nand->erasesize))
            return 1;

        //=====   writing raw data to nand process   =====//
        total = nand->erasesize;
        current_offset = (u_char *)buffer_offset + good_blk_num * nand->erasesize;
        flush_cache((ulong)current_offset, total);
        ret = nand_write(nand, nand_offset, &total, current_offset);
        if (ret || total != nand->erasesize)
        	return 1;

        nand_offset += nand->erasesize;
        good_blk_num++;

        //=====   writing complete check   =====//
        if (good_blk_num == env_blk_size)
            break;
    }

    //=====   written block count check   =====//
    if (0 == good_blk_num)
    {
        printf("All the blocks reserved are bad, no data is written!\r\n");
        return ret;
    }
    if ((good_blk_num < env_blk_size) && (good_blk_num > 0))
    {
        printf("Only %d blocks of backup data is written because of bad block,\r\n%d blocks should be written.\r\n",
                        good_blk_num, env_blk_size);
    }

    puts ("done\n");
    return ret;
}

U_BOOT_CMD(nand_raw_read, 5, 1, do_nand_raw_read,
  "nand_raw_read    - read raw data from nand to memory\n",
  "nand_raw_read    [nand_offset] [nand_blk_num] [buffer_offset] [buffer_size]\n");

U_BOOT_CMD(nand_raw_write, 5, 1, do_nand_raw_write,
  "nand_raw_write   - write raw data from memory to nand\n",
  "nand_raw_write   [nand_offset] [nand_blk_num] [buffer_offset] [buffer_size]\n");

U_BOOT_CMD(nand, 5, 1, do_nand,
  "nand    - NAND sub-system\n",
  "info                  - show available NAND devices\n"
  "nand device [dev]     - show or set current device\n"
  "nand read[.jffs2]     - addr off|partition size\n"
  "nand write[.jffs2]    - addr off|partiton size - read/write `size' bytes starting\n"
  "    at offset `off' to/from memory address `addr'\n"
    "nand read[.yaffs]     - addr off size - read the `size' byte yaffs image starting\n"
    "    at offset `off' to memory address `addr'\n"
    "nand write[.yaffs]    - addr off size - write the `size' byte yaffs image starting\n"
    "    at offset `off' from memory address `addr'\n"
  "nand erase [clean] [off size] - erase `size' bytes from\n"
  "nand eloop [loop]- erase all for loop \n"
  "    offset `off' (entire device if not specified)\n"
  "nand bad - show bad blocks\n"
  "nand dump[.oob] off - dump page\n"
  "nand scrub - really clean NAND erasing bad blocks (UNSAFE)\n"
  "nand markbad off - mark bad block at offset (UNSAFE)\n"
  "nand biterr off - make a bit error at offset (UNSAFE)\n"
  "nand lock [tight] [status] - bring nand to lock state or display locked pages\n"
  "nand unlock [offset] [size] - unlock section\n"
  "nand bist [forever] - BIST test\n"
  "nand stable - Stable test\n"
);

U_BOOT_CMD(nboot, 4, 1, do_nandboot,
  "nboot   - boot from NAND device\n",
  "[partition] | [[[loadAddr] dev] offset]\n");

U_BOOT_CMD(
  nandclk, 2, 1, do_nandclk,
  "nandclk - tune nand clock\n",
  "nandclk mode      \n"

    #if defined(CONFIG_TITANIA8) || defined(CONFIG_TITANIA9) || defined(CONFIG_JANUS2)
    "mode=0 NFIE_CLK_5_4M \n"
    "mode=1 NFIE_CLK_27M \n"
    "mode=2 NFIE_CLK_32M \n"
    "mode=3 NFIE_CLK_43M \n"
    #elif defined(CONFIG_TITANIA12) || defined(CONFIG_TITANIA13) || defined(CONFIG_AMBER1) || defined(CONFIG_AMBER1) || defined(CONFIG_AMBER6) || defined(CONFIG_AMBER7) || defined(CONFIG_AMBER5) || defined(CONFIG_AMBER2) || defined(CONFIG_AMETHYST)
  "mode=0 NFIE_CLK_5_4M \n"
    "mode=1 NFIE_CLK_27M \n"
    "mode=2 NFIE_CLK_32M \n"
    "mode=3 NFIE_CLK_36M \n"
    "mode=4 NFIE_CLK_40M \n"
    "mode=5 NFIE_CLK_43M \n"
    #else
  "mode=0 NFIE_CLK_300K \n"
    "mode=1 NFIE_CLK_750K \n"
    "mode=2 NFIE_CLK_5_4M \n"
    "mode=3 NFIE_CLK_13M \n"
    "mode=4 NFIE_CLK_18M \n"
    "mode=5 NFIE_CLK_22M \n"
    "mode=6 NFIE_CLK_27M \n"
    "mode=7 NFIE_CLK_32M \n"
    "mode=8 NFIE_CLK_43M \n"
    #endif
);
#ifdef CONFIG_BOOT_FROM_NAND
U_BOOT_CMD(
    nandwritecis, 9, 1, do_nand_write_cis,
    "nandwritecis - write 2 cis into first 2 blocks of first 10 blocks\n",
    "command: nandwritecis\n"
    "nandwritecis [nandInfoAddr] [partInfoAddr]"
);
#endif

U_BOOT_CMD(
  rsv, 3, 1,  do_nand_write_rsv,
  "rsv - Set reserved block as bad or good\n",
  "rsv bad [rsv_num] or rsv good, rsv_num refers to reserved block number\n"
);

#if (CONFIG_COMMANDS & CFG_CMD_FAT)
U_BOOT_CMD(
  ubibin, 1, 1, do_ubi_partition_mkbin,
  "ubibin - read out ubi partition and store it to usb disk\n",
  "command: ubibin\n"
);

U_BOOT_CMD(
  nandbin2, 1, 1, do_nand_mkbin2,
  "nandbin2 - read out ubi partition and environment parameter at once and store it to usb disk\n",
  "command: nandbin2\n"
);

U_BOOT_CMD(
  nandbin, 1, 1,  do_nand_mkbin,
  "nandbin - read out the whole contents of nand and store it to usb disk\n",
  "command: nandbin\n"
);

U_BOOT_CMD(
  ubi2nand, 1, 1, do_ubi_partition_restore,
  "ubi2nand - read ubi partition file and restore it to nand\n",
  "command: ubi2nand\n"
);

U_BOOT_CMD(
  nandbin2nand, 3, 1, do_nand_bin_restore,
  "nandbin2nand - read binary files and restore it to nand\n",
  "command: nandbin2nand\n"
);
#endif

U_BOOT_CMD(
  bininfo, 1, 1,  do_print_nandbin_info,
  "bininfo - print ubi partition and environment parameter information\n",
  "command: bininfo\n"
);

#endif        /* (CONFIG_COMMANDS & CFG_CMD_NAND) */

#else /* CFG_NAND_LEGACY */
/*
 *
 * Legacy NAND support - to be phased out
 *
 */
#include <command.h>
#include <malloc.h>
#include <asm/io.h>
#include <watchdog.h>

#ifdef CONFIG_SHOW_BOOT_PROGRESS
# include <status_led.h>
# define SHOW_BOOT_PROGRESS(arg)  show_boot_progress(arg)
#else
# define SHOW_BOOT_PROGRESS(arg)
#endif

#if (CONFIG_COMMANDS & CFG_CMD_NAND)
#include <linux/mtd/nand_legacy.h>
#if 0
#include <linux/mtd/nand_ids.h>
#include <jffs2/jffs2.h>
#endif

#ifdef CONFIG_OMAP1510
void archflashwp(void *archdata, int wp);
#endif

#define ROUND_DOWN(value,boundary)      ((value) & (~((boundary)-1)))

#undef  NAND_DEBUG
#undef  PSYCHO_DEBUG

/* ****************** WARNING *********************
 * When ALLOW_ERASE_BAD_DEBUG is non-zero the erase command will
 * erase (or at least attempt to erase) blocks that are marked
 * bad. This can be very handy if you are _sure_ that the block
 * is OK, say because you marked a good block bad to test bad
 * block handling and you are done testing, or if you have
 * accidentally marked blocks bad.
 *
 * Erasing factory marked bad blocks is a _bad_ idea. If the
 * erase succeeds there is no reliable way to find them again,
 * and attempting to program or erase bad blocks can affect
 * the data in _other_ (good) blocks.
 */
#define  ALLOW_ERASE_BAD_DEBUG 0

#define CONFIG_MTD_NAND_ECC  /* enable ECC */
#define CONFIG_MTD_NAND_ECC_JFFS2

/* bits for nand_legacy_rw() `cmd'; or together as needed */
#define NANDRW_READ 0x01
#define NANDRW_WRITE  0x00
#define NANDRW_JFFS2  0x02
#define NANDRW_JFFS2_SKIP 0x04

/*
 * Imports from nand_legacy.c
 */
extern struct nand_chip nand_dev_desc[CONFIG_SYS_MAX_NAND_DEVICE];
extern int curr_device;
extern int nand_legacy_erase(struct nand_chip *nand, size_t ofs,
          size_t len, int clean);
extern int nand_legacy_rw(struct nand_chip *nand, int cmd, size_t start,
       size_t len, size_t *retlen, u_char *buf);
extern void nand_print(struct nand_chip *nand);
extern void nand_print_bad(struct nand_chip *nand);
extern int nand_read_oob(struct nand_chip *nand, size_t ofs,
             size_t len, size_t *retlen, u_char *buf);
extern int nand_write_oob(struct nand_chip *nand, size_t ofs,
        size_t len, size_t *retlen, const u_char *buf);

int do_nand (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    int rcode = 0;

    switch (argc) {
    case 0:
    case 1:
  printf ("Usage:\n%s\n", cmdtp->usage);
  return 1;
    case 2:
  if (strcmp(argv[1],"info") == 0) {
    int i;

    putc ('\n');

    for (i=0; i<CONFIG_SYS_MAX_NAND_DEVICE; ++i) {
      if(nand_dev_desc[i].ChipID == NAND_ChipID_UNKNOWN)
        continue; /* list only known devices */
      printf ("Device %d: ", i);
      nand_print(&nand_dev_desc[i]);
    }
    return 0;
    #if 0
  } else if (strcmp(argv[1],"device") == 0) {
    if ((curr_device < 0) || (curr_device >= CONFIG_SYS_MAX_NAND_DEVICE)) {
      puts ("\nno devices available\n");
      return 1;
    }
    printf ("\nDevice %d: ", curr_device);
    nand_print(&nand_dev_desc[curr_device]);
    return 0;

  } else if (strcmp(argv[1],"bad") == 0) {
    if ((curr_device < 0) || (curr_device >= CONFIG_SYS_MAX_NAND_DEVICE)) {
      puts ("\nno devices available\n");
      return 1;
    }
    printf ("\nDevice %d bad blocks:\n", curr_device);
    nand_print_bad(&nand_dev_desc[curr_device]);
    return 0;
    #endif
  }
  printf ("Usage:\n%s\n", cmdtp->usage);
  return 1;
    case 3:
  if (strcmp(argv[1],"device") == 0) {
    int dev = (int)simple_strtoul(argv[2], NULL, 10);

    printf ("\nDevice %d: ", dev);
    if (dev >= CONFIG_SYS_MAX_NAND_DEVICE) {
      puts ("unknown device\n");
      return 1;
    }
    nand_print(&nand_dev_desc[dev]);
    /*nand_print (dev);*/

    if (nand_dev_desc[dev].ChipID == NAND_ChipID_UNKNOWN) {
      return 1;
    }

    curr_device = dev;

    puts ("... is now current device\n");

    return 0;
  }
  else if (strcmp(argv[1],"erase") == 0 && strcmp(argv[2], "clean") == 0) {
    struct nand_chip* nand = &nand_dev_desc[curr_device];
    ulong off = 0;
    ulong size = nand->totlen;
    int ret=0;

    printf ("\nNAND erase: device %d offset %ld, size %ld ... ",
      curr_device, off, size);

    ret = nand_legacy_erase (nand, off, size, 1);

    printf("%s\n", ret ? "ERROR" : "OK");

    return ret;
  }

  printf ("Usage:\n%s\n", cmdtp->usage);
  return 1;
    default:
  /* at least 4 args */

  if (strncmp(argv[1], "read", 4) == 0 ||
      strncmp(argv[1], "write", 5) == 0) {
      ulong addr = (0 == strncmp(argv[2], UPGRADE_ADDR_STR, strlen(UPGRADE_ADDR_STR))) ? UPGRADE_BUFFER_ADDR : simple_strtoul(argv[2], NULL, 16);
    ulong off  = simple_strtoul(argv[3], NULL, 16);
    ulong size = simple_strtoul(argv[4], NULL, 16);
    int cmd    = (strncmp(argv[1], "read", 4) == 0) ?
        NANDRW_READ : NANDRW_WRITE;
    int ret=0, total=0;
    char* cmdtail = strchr(argv[1], '.');

    if (cmdtail && !strncmp(cmdtail, ".oob", 2)) {
      /* read out-of-band data */
      if (cmd & NANDRW_READ) {
        ret = nand_read_oob(nand_dev_desc + curr_device,
                off, size, (size_t *)&total,
                (u_char*)addr);
      }
      else {
        ret = nand_write_oob(nand_dev_desc + curr_device,
                 off, size, (size_t *)&total,
                 (u_char*)addr);
      }
      return ret;
    }
    else if (cmdtail && !strncmp(cmdtail, ".jffs2", 2))
      cmd |= NANDRW_JFFS2;  /* skip bad blocks */
    else if (cmdtail && !strncmp(cmdtail, ".jffs2s", 2)) {
      cmd |= NANDRW_JFFS2;  /* skip bad blocks (on read too) */
      if (cmd & NANDRW_READ)
        cmd |= NANDRW_JFFS2_SKIP; /* skip bad blocks (on read too) */
    }
#ifdef SXNI855T
    /* need ".e" same as ".j" for compatibility with older units */
    else if (cmdtail && !strcmp(cmdtail, ".e"))
      cmd |= NANDRW_JFFS2;  /* skip bad blocks */
#endif
#ifdef CFG_NAND_SKIP_BAD_DOT_I
    /* need ".i" same as ".jffs2s" for compatibility with older units (esd) */
    /* ".i" for image -> read skips bad block (no 0xff) */
    else if (cmdtail && !strcmp(cmdtail, ".i")) {
      cmd |= NANDRW_JFFS2;  /* skip bad blocks (on read too) */
      if (cmd & NANDRW_READ)
        cmd |= NANDRW_JFFS2_SKIP; /* skip bad blocks (on read too) */
    }
#endif /* CFG_NAND_SKIP_BAD_DOT_I */
    else if (cmdtail) {
      printf ("Usage:\n%s\n", cmdtp->usage);
      return 1;
    }

    printf ("\nNAND %s: device %d offset %ld, size %ld ...\n",
      (cmd & NANDRW_READ) ? "read" : "write",
      curr_device, off, size);

    ret = nand_legacy_rw(nand_dev_desc + curr_device, cmd, off, size,
           (size_t *)&total, (u_char*)addr);

    printf (" %d bytes %s: %s\n", total,
      (cmd & NANDRW_READ) ? "read" : "written",
      ret ? "ERROR" : "OK");

    return ret;
  } else if (strcmp(argv[1],"erase") == 0 &&
       (argc == 4 || strcmp("clean", argv[2]) == 0)) {
    int clean = argc == 5;
    ulong off = simple_strtoul(argv[2 + clean], NULL, 16);
    ulong size = simple_strtoul(argv[3 + clean], NULL, 16);
    int ret=0;

    printf ("\nNAND erase: device %d offset %ld, size %ld ...\n",
      curr_device, off, size);

    ret = nand_legacy_erase (nand_dev_desc + curr_device,
          off, size, clean);

    printf("%s\n", ret ? "ERROR" : "OK");

    return ret;
  } else {
    printf ("Usage:\n%s\n", cmdtp->usage);
    rcode = 1;
  }

  return rcode;
    }
}

U_BOOT_CMD(
  nand, 5,  1,  do_nand,
  "nand    - legacy NAND sub-system\n",
  "info  - show available NAND devices\n"
  "nand device [dev] - show or set current device\n"
  "nand read[.jffs2[s]]  addr off size\n"
  "nand write[.jffs2] addr off size - read/write `size' bytes starting\n"
  "    at offset `off' to/from memory address `addr'\n"
  "nand erase [clean] [off size] - erase `size' bytes from\n"
  "    offset `off' (entire device if not specified)\n"
  "nand bad - show bad blocks\n"
  "nand read.oob addr off size - read out-of-band data\n"
  "nand write.oob addr off size - read out-of-band data\n"
);

int do_nandboot (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
  char *boot_device = NULL;
  char *ep=NULL;
  int dev=0;
  ulong cnt=0;
  ulong addr=0;
  ulong offset = 0;
  image_header_t *hdr=NULL;
  int rcode = 0;
  switch (argc) {
  case 1:
    addr = CONFIG_SYS_LOAD_ADDR;
    boot_device = getenv ("bootdevice");
    break;
  case 2:
    addr = simple_strtoul(argv[1], NULL, 16);
    boot_device = getenv ("bootdevice");
    break;
  case 3:
    addr = simple_strtoul(argv[1], NULL, 16);
    boot_device = argv[2];
    break;
  case 4:
    addr = simple_strtoul(argv[1], NULL, 16);
    boot_device = argv[2];
    offset = simple_strtoul(argv[3], NULL, 16);
    break;
  default:
    printf ("Usage:\n%s\n", cmdtp->usage);
    SHOW_BOOT_PROGRESS (-1);
    return 1;
  }

  if (!boot_device) {
    puts ("\n** No boot device **\n");
    SHOW_BOOT_PROGRESS (-1);
    return 1;
  }

  dev = simple_strtoul(boot_device, &ep, 16);

  if ((dev >= CONFIG_SYS_MAX_NAND_DEVICE) ||
      (nand_dev_desc[dev].ChipID == NAND_ChipID_UNKNOWN)) {
    printf ("\n** Device %d not available\n", dev);
    SHOW_BOOT_PROGRESS (-1);
    return 1;
  }

  printf ("\nLoading from device %d: %s at 0x%lx (offset 0x%lx)\n",
    dev, nand_dev_desc[dev].name, nand_dev_desc[dev].IO_ADDR,
    offset);

  if (nand_legacy_rw (nand_dev_desc + dev, NANDRW_READ, offset,
      SECTORSIZE, NULL, (u_char *)addr)) {
    printf ("** Read error on %d\n", dev);
    SHOW_BOOT_PROGRESS (-1);
    return 1;
  }

  hdr = (image_header_t *)addr;

  if (ntohl(hdr->ih_magic) == IH_MAGIC) {

    print_image_hdr (hdr);

    cnt = (ntohl(hdr->ih_size) + sizeof(image_header_t));
    cnt -= SECTORSIZE;
  } else {
    printf ("\n** Bad Magic Number 0x%x **\n", ntohl(hdr->ih_magic));
    SHOW_BOOT_PROGRESS (-1);
    return 1;
  }

  if (nand_legacy_rw (nand_dev_desc + dev, NANDRW_READ,
      offset + SECTORSIZE, cnt, NULL,
      (u_char *)(addr+SECTORSIZE))) {
    printf ("** Read error on %d\n", dev);
    SHOW_BOOT_PROGRESS (-1);
    return 1;
  }

  /* Loading ok, update default load address */

  load_addr = addr;

  /* Check if we should attempt an auto-start */
  if (((ep = getenv("autostart")) != NULL) && (strcmp(ep,"yes") == 0)) {
    char *local_args[2];
    extern int do_bootm (cmd_tbl_t *, int, int, char *[]);

    local_args[0] = argv[0];
    local_args[1] = NULL;

    printf ("Automatic boot of image at addr 0x%08lx ...\n", addr);

    do_bootm (cmdtp, 0, 1, local_args);
    rcode = 1;
  }
  return rcode;
}

U_BOOT_CMD(
  nboot,  4,  1,  do_nandboot,
  "nboot   - boot from NAND device\n",
  "loadAddr dev\n"
);

#define READ_NAND_ID_LENGTH    5
int do_nand_readID( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    uchar buffer[READ_NAND_ID_LENGTH];
    uint len=0;

    if ((argc < 1) || (argc > 1))
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }
    memset(buffer, 0, sizeof(buffer));

    len = READ_NAND_ID_LENGTH;
    drvNAND_Read_ID();

    printf("Manufacturer ID is:");
    printf("%4x\n",buffer[0]);
    printf("Device ID is:");
    printf("%4x",buffer[1]);
    printf("%4x\n",buffer[2]);
    return 0;
}

U_BOOT_CMD(
  nandid,    1,        1, do_nand_readID,
  "nand read id  - nand read ID\n",
  "command: nandid\n"
);

#endif /* (CONFIG_COMMANDS & CFG_CMD_NAND) */

#endif /* CFG_NAND_LEGACY */
