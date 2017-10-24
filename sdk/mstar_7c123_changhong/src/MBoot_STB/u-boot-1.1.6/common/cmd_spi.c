/*
 * (C) Copyright 2002
 * Gerald Van Baren, Custom IDEAS, vanbaren@cideas.com
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
 * SPI Read/Write Utilities
 */
#include <common.h>
#include <command.h>
#include <jffs2/jffs2.h>
#if (CONFIG_COMMANDS & CFG_CMD_SPI)

#include <spi.h>

#include <sys/drv/drvSERFLASH.h>
#include "uboot_mmap.h"
#include <sys/drv/drvBDMA.h>
#include <malloc.h>

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define KSEG02KSEG1(addr)   ((void *)((U32)(addr)|0x80000000))

#define	SPI_TRACE(fmt,args...)	//printf (fmt ,##args)
#define	SPI_ERROR(fmt,args...)	printf(fmt ,##args)

#ifndef MAX_SPI_BYTES
#define MAX_SPI_BYTES 0x1000    /* Maximum number of bytes we can handle */
#endif
#define READ_ID_LENGTH 3

//input total char
#define BASE_CMD_INPUT_LEN             5
#define INIT_INPUT_LEN                 1
#define READ_ID_INPUT_LEN              1
#define READ_INPUT_LEN                 3
#define WRITE_INPUT_LEN                3
#define ERASE_CHIP_INPUT_LEN           1
#define ERASE_SEC_INPUT_LEN            3
#define WRITE_PRO_INPUT_LEN            2
#define WRITE_BUFF_INPUT_LEN           4
#define READ_BUFF_INPUT_LEN            3
#define GET_CHIP_REV_INPUT_LEN         1
#define GET_FLASH_INFO_INPUT_LEN       1
#define READ_STATUS_INPUT_LEN          1
#define DMA_INPUT_LEN                  4
#define READ_CODE_INPUT_LEN            4
#define WRITE_CODE_INPUT_LEN           4
#define WRITE_PARTITION_INPUT_LEN      5
#define READ_PARTITION_INPUT_LEN       5
#define CREATE_PARTITION_INPUT_LEN     3
#define REMOVE_PARTITION_INPUT_LEN     2
#define ERASE_PARTITION_INPUT_LEN      2
#define RMGPT_INPUT_LEN                1
#define INIT_PARTITION_INPUT_LEN       1
#define PART_INPUT_LEN                 1
#define SPI_FLAS_BASE_ADDRESS 0xBFC00000
#define SPI_UBOOT_END_ADDRESS 0xBFC70000
#define CMD_BUF 128

#define SERFLASH_BLOCK_SIZE (64 * 1024) // <-@@@ please sync with SERFLASH_BLOCK_SIZE @ spiflash.c

#define SPI_PARTITON_NAME_SIZE 32
typedef struct{
    char name[SPI_PARTITON_NAME_SIZE];
    unsigned int num;
    unsigned int offset;
    unsigned int size;
    struct list_head list;
}ST_SPI_PARTITION;

#define ENV_SPI_LAYOUT "mtdparts"
#define SPI_LAYOUT_ID "mtdparts=edb8M-SPI:"
#define MBOOT_PARTITION "MBOOT"
#if 1
#define MAIN_PART_OFFSET "MainPartOffset"
#endif

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------
extern int snprintf(char *str, size_t size, const char *fmt, ...);
extern int strcmp(const char * cs,const char * ct);
extern char * strncpy(char * dest,const char *src,size_t count);
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
/*
 * External table of chip select functions (see the appropriate board
 * support for the actual definition of the table).
 */
// spi_chipsel_type spi_chipsel[]; // UNUSED
int spi_chipsel_cnt;
uchar ubuffer[MAX_SPI_BYTES];//uint *wbuffer;//uchar *ubuffer;//
/*
 * Values from last command.
 */
static int   device;
static int   bitlen;
static uchar dout[MAX_SPI_BYTES];
//static uchar din[MAX_SPI_BYTES];
const SERFLASH_Info *pFlashInfo;
static uchar spiInitFlag = 0;
#if defined(CONFIG_BOOT_SPI_LINUX)
static struct list_head spi_partition_list;
static unsigned int partition_list_ready = 0;
static unsigned int spi_size = 0;
static SERFLASH_Info spiInfo;
#endif

#if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)

/* parition handling routines */
int mtdparts_init(void);
int id_parse(const char *id, const char **ret_id, u8 *dev_type, u8 *dev_num);
int find_dev_and_part(const char *id, struct mtd_device **dev,
		u8 *part_num, struct part_info **part);
#endif

static inline int str2long_nor(char *p, unsigned int *num)
{
	char *endptr;

	*num = simple_strtoul(p, &endptr, 16);
	return (*p != '\0' && *endptr == '\0') ? 1 : 0;
}

static int arg_off_size_nor(int argc, char *argv[], unsigned int *off, unsigned int *size)
{
#if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;

	if (argc >= 1 && !(str2long_nor(argv[0], off))) {
		if ((mtdparts_init() == 0) &&
		    (find_dev_and_part(argv[0], &dev, &pnum, &part) == 0)) {
			if (dev->id->type != MTD_DEV_TYPE_NOR) {
				puts("not a NOR device\n");
				return -1;
			}
			*off = part->offset;
			if (argc >= 2) {
				if (!(str2long_nor(argv[1], size))) {
					printf("'%s' is not a number\n", argv[1]);
					return -1;
				}
			} else {
				*size = part->size;
			}

			goto out;
		}
	}
#endif

	if (argc >= 1) {
		if (!(str2long_nor(argv[0], off))) {
			printf("'%s' is not a number\n", argv[0]);
			return -1;
		}
	} else {
		*off = 0;
	}

	if (argc >= 2) {
		if (!(str2long_nor(argv[1], size))) {
			printf("'%s' is not a number\n", argv[1]);
			return -1;
		}
	} else {
	       pFlashInfo = MDrv_SERFLASH_GetInfo();
		*size = pFlashInfo->u32TotalSize - *off;
	}

#if (CONFIG_COMMANDS & CFG_CMD_JFFS2) && defined(CONFIG_JFFS2_CMDLINE)
out:
#endif

	printf("offset 0x%x, size 0x%x\n", *off, *size);
	return 0;
}

static int spi_flash_init(void)
{
    MDrv_SERFLASH_Init();
    spiInitFlag = 1;
    printf("initialization done!\n");
    return 0;
}

#if defined(CONFIG_BOOT_SPI_LINUX)
static int add2MtdPart(char *name, unsigned int size)
{
    #define MTD_LAYOUT_SIZE 512
    char *pEnv = NULL;
    char *mtdlayout = NULL;
    char env_exist = 0;

    if (name == NULL)
    {
        SPI_ERROR("'name' is a null pointer\n");
        return 0;
    }

    if (size == 0)
    {
        SPI_ERROR("The size of partition is zero\n");
        return -1;
    }

    if ((strlen(name)+1) > SPI_PARTITON_NAME_SIZE)
    {
        SPI_ERROR("The length of %s is over 0x%x\n", name, SPI_PARTITON_NAME_SIZE);
        return -1;
    }

    mtdlayout = malloc(MTD_LAYOUT_SIZE);
    if (mtdlayout == NULL)
    {
        SPI_ERROR("malloc for mtdlayout fail\n");
        return -1;
    }

    memset(mtdlayout, 0, MTD_LAYOUT_SIZE);
    pEnv = getenv(ENV_SPI_LAYOUT);
    if (pEnv == NULL)
    {
        snprintf(mtdlayout, MTD_LAYOUT_SIZE, "%s", SPI_LAYOUT_ID);
        env_exist = 1;
    }
    else
    {
        strncpy(mtdlayout, pEnv, strlen(pEnv)+1);
    }

    if (strstr(mtdlayout, name) != NULL)
    {
        free(mtdlayout);
        SPI_TRACE("%s exist in mtdparts\n", name);
        SPI_TRACE("OK\n");
        return 0;
    }

    SPI_TRACE("mtdparts=%s\n", mtdlayout);
    //Add a ','
    if (env_exist == 0)
    {
       snprintf(mtdlayout, MTD_LAYOUT_SIZE, "%s,", mtdlayout);
    }

    if ((size/1024)) // size is larger than 1KBytes
    {
        snprintf(mtdlayout, MTD_LAYOUT_SIZE, "%s%dk(%s)", mtdlayout, ((size/1024)), name);
    }
    else
    {
        snprintf(mtdlayout, MTD_LAYOUT_SIZE, "%s%d(%s)", mtdlayout, (size), name);
    }
    SPI_TRACE("mtdparts=%s\n", mtdlayout);

    setenv(ENV_SPI_LAYOUT,mtdlayout);
    free(mtdlayout);

    SPI_TRACE("OK\n");
    return 0;
}

static int delMtdPart(void)
{
    char *buf = NULL;
    int ret = 0;

    buf = malloc(CMD_BUF);
    if (buf == NULL)
    {
        SPI_ERROR("malloc fail\n");
        return -1;
    }

    memset(buf, 0, CMD_BUF);
    snprintf(buf, CMD_BUF, "setenv %s", ENV_SPI_LAYOUT);
    ret = run_command(buf, 0);
    if (ret == 0)
    {
        SPI_TRACE("OK\n");
    }
    else
    {
        SPI_ERROR("delet %s fail\n", ENV_SPI_LAYOUT);
    }
    free(buf);

    return 0;
}

static int getSpiSize(unsigned int *u32SpiSize)
{
    MS_BOOL ret = 0;

    if (spi_size == 0)
    {
        ret = MDrv_SERFLASH_DetectSize((MS_U32 *)&spi_size);
        if (ret != TRUE)
        {
            SPI_ERROR("detect SPI size fail\n");
            return -1;
        }
    }
    SPI_TRACE("spi_size=0x%x\n", spi_size);
    *u32SpiSize = (unsigned int)spi_size;

    SPI_TRACE("OK\n");
    return 0;
}

static int getSpiInfo(SERFLASH_Info *info)
{
    int ret = -1;
    const SERFLASH_Info *temp = NULL;

    if (info == NULL)
    {
        SPI_ERROR("The input parameter 'info' is a null pointer\n");
        return ret;
    }

    ret = run_command("spi_in", 0);
    if (ret == -1)
    {
        SPI_ERROR("do 'spi init' fail\n");
        return ret;
    }

    temp = MDrv_SERFLASH_GetInfo();

    if (temp == NULL)
    {
        SPI_ERROR("do 'MDrv_SERFLASH_GetInfo' fail\n");
        return ret;
    }
    memcpy(info, temp, sizeof(SERFLASH_Info));

    SPI_TRACE("OK\n");
    return 0;
}

static char *get_next_member_info(char *source, char *name, unsigned int *size)
{
    char *pS = NULL;
    char *pE = NULL;
    char *pSize = NULL;
    char *pName = NULL;
    char *p = NULL;

    if (source == NULL)
    {
        SPI_ERROR("'source' is a null pointer\n");
        return NULL;
    }

    if (size == NULL)
    {
        SPI_ERROR("'source' is a null pointer\n");
        return NULL;
    }

    //avoid mtdparts=xxxx:
    if (strchr(source, ':') != NULL)
    {
       source = strchr(source, ':');
       source += 1;
    }
    else
    {
        //avoid mtdparts=xxxx:5M(KL),2M(RFS),.....
        if(strchr(source, ',') != NULL)
        {
           source = strchr(source, ',');
           source += 1;
        }
    }

    pS = strchr(source, '(');
    if (pS == NULL)
    {
        SPI_ERROR("No symbol '('\n");
        return NULL;
    }

    pE = strchr(source, ')');
    if (pS == pE)
    {
        SPI_ERROR("No symbol ')'\n");
        return NULL;
    }

    *pE='\0';
    *pS='\0';

    pName = pS + 1;
    SPI_TRACE("member name =%s\n", pName);
    pSize = source;
    SPI_TRACE("member size =%s\n", pSize);
    if ((strchr(pSize,'m') != NULL)||(strchr(pSize,'M') != NULL))
    {

         p=(char *)((unsigned int)pSize+strlen(pSize));
        *p='\0';
        *size = simple_strtol(pSize, NULL, 10);
        SPI_TRACE("size=0x%xMBytes\n", *size);
        *size=(*size << 20);

    }
    else if ((strchr(pSize,'k') != NULL)||(strchr(pSize,'K') != NULL))
    {
         p = (char *)((unsigned int)pSize+strlen(pSize));
        *p='\0';
        *size = simple_strtol(pSize, NULL, 10);
        SPI_TRACE("size=0x%xKBytes\n", *size);
        *size = (*size << 10);

    }
    else
    {
        *size = simple_strtol(pSize, NULL, 10);
    }

    if((strlen(pName)+1) > SPI_PARTITON_NAME_SIZE)
    {
        SPI_ERROR("The lenght of partition '%s' is over 0x%x\n", pName, SPI_PARTITON_NAME_SIZE);
        return NULL;
    }
    strcpy(name, (const char *)pName);

    SPI_TRACE("OK\n");
    return (pE+1);
}

static int add_partition(char *name, unsigned size)
{
    int ret = 0;
    unsigned int _spi_size = 0;
    unsigned int cur_offset = 0;
    unsigned int last_num = 0;
    ST_SPI_PARTITION *stSpiPartition = NULL;

    if (partition_list_ready == 0)
    {
        SPI_ERROR("spi partition list is not ready\n");
        return -1;
    }

    if (name == NULL)
    {
        SPI_ERROR("'name' is a null pointer\n");
        return -1;
    }

    if (size == 0)
    {
        SPI_ERROR("The size of 0x%x is zero\n",size);
        return -1;
    }

    if ((strlen(name)+1) > SPI_PARTITON_NAME_SIZE)
    {
        SPI_ERROR("The length of %s is over 0x%x\n", name, SPI_PARTITON_NAME_SIZE);
        return -1;
    }

    //alignment to block size
    SPI_TRACE("origin size = 0x%x\n", size);
    if ((size%spiInfo.u32SecSize) != 0)
    {
        size += (spiInfo.u32SecSize-size%spiInfo.u32SecSize);
    }
    SPI_TRACE("final size = 0x%x\n", size);

    ret = getSpiSize(&_spi_size);
    if (ret != 0)
    {
        SPI_ERROR("get spi's size fail\n");
        return -1;
    }

    if (size > _spi_size)
    {
        SPI_ERROR("This partition size is larger than this SPI size\n");
        return -1;
    }
    //Check if this partition exist or not
    list_for_each_entry(stSpiPartition, &spi_partition_list, list)
    {
        SPI_TRACE("stSpiPartition->name=%s\n", stSpiPartition->name);
        if (strcmp(stSpiPartition->name, name) == 0)
        {
            SPI_ERROR("This partition alraedy exist\n");
            return -1;
        }
        cur_offset += stSpiPartition->size;
        last_num = stSpiPartition->num;
    }

    //Check whether or not the total size is over the SPI size.
    SPI_TRACE("cur_offset=0x%x\n", cur_offset);
    if ((cur_offset+size) > _spi_size)
    {
        SPI_ERROR("The total size is over the size of this SPI\n");
        return -1;
    }

    //Create a partition info for this new partition
    stSpiPartition = malloc(sizeof(ST_SPI_PARTITION));
    if (stSpiPartition == NULL)
    {
        SPI_ERROR("malloc for stSpiPartition fail\n");
        return -1;
    }
    memset(stSpiPartition,0,sizeof(ST_SPI_PARTITION));

    ret = add2MtdPart(name, size);
    if (ret != 0)
    {
        free(stSpiPartition);
        SPI_ERROR("edit env 'mtdpart' fail\n");
        return -1;
    }

    INIT_LIST_HEAD(&stSpiPartition->list);
    strcpy(stSpiPartition->name, name);
    stSpiPartition->offset = cur_offset;
    stSpiPartition->size = size;
    stSpiPartition->num = last_num + 1;
    SPI_TRACE("stSpiPartition->name=%s\n", stSpiPartition->name);
    SPI_TRACE("stSpiPartition->offset=0x%x\n", stSpiPartition->offset);
    SPI_TRACE("stSpiPartition->size=0x%x\n", stSpiPartition->size);
    SPI_TRACE("stSpiPartition->num=0x%x\n", stSpiPartition->num);
    list_add_tail(&stSpiPartition->list, &spi_partition_list);


    SPI_TRACE("OK\n");
    return 0;
}

static int add_mboot_partition(void)
{
   int ret = 0;
   unsigned size = 0;

   SPI_TRACE("The end rom address of uboot is 0x%x\n", SPI_UBOOT_END_ADDRESS);

   size = SPI_UBOOT_END_ADDRESS - SPI_FLAS_BASE_ADDRESS;
   ret = add_partition(MBOOT_PARTITION, SPI_UBOOT_END_ADDRESS - SPI_FLAS_BASE_ADDRESS);

   if (ret != 0)
   {
       SPI_ERROR("set partition '%s' fail\n", MBOOT_PARTITION);
       return -1;
   }


   //alignment to block size
   SPI_TRACE("origin size=0x%x\n", size);
   if ((size%spiInfo.u32SecSize) != 0)
   {
       size += (spiInfo.u32SecSize-size%spiInfo.u32SecSize);
   }
   SPI_TRACE("final size=0x%x\n", size);

   char *buf = NULL;
   buf = malloc(CMD_BUF);
   if (buf == NULL)
   {
       SPI_ERROR("malloc fail\n");
       return -1;
   }

   memset(buf,0,CMD_BUF);
   snprintf(buf,CMD_BUF,"setenv %s 0x%x", MAIN_PART_OFFSET, size);
   ret = run_command(buf, 0);

   if (ret == 0)
   {
       SPI_TRACE("OK\n");
   }
   else
   {
       SPI_ERROR("Set %s Failed.\n", MAIN_PART_OFFSET);
   }
   free(buf);

   SPI_TRACE("OK\n");
   return 0;
}

static int del_all_partitions(void)
{
    ST_SPI_PARTITION *stSpiPartition = NULL;

    if (partition_list_ready == 0)
    {
        SPI_ERROR("spi partition list is not ready\n");
        return -1;
    }

    if (list_empty(&spi_partition_list) == 1)
    {
        SPI_TRACE("no partition\n");
        SPI_TRACE("OK\n");
        return 0;
    }

    while(list_empty(&spi_partition_list) != 1)
    {
        stSpiPartition = list_entry(spi_partition_list.next, ST_SPI_PARTITION, list);
        SPI_TRACE("delete %s\n", stSpiPartition->name);
        list_del(&stSpiPartition->list);
        free(stSpiPartition);
    }

    if (delMtdPart() != 0)
    {
        printf("delete all partitions infor fail\n");
        return -1;
    }

    SPI_TRACE("OK\n");
    return 0;
}

static int parse_mtdpart(void)
{
    char member_name[SPI_PARTITON_NAME_SIZE];
    unsigned int member_size = 0;
    char *pEnv = NULL;
    char *p1 = NULL, *p2 = NULL;
    int ret = 0;

    if (partition_list_ready == 0)
    {
        SPI_ERROR("partition_list_ready=0\n");
        return -1;
    }
    pEnv = getenv(ENV_SPI_LAYOUT);
    if (pEnv == NULL)
    {
        SPI_ERROR("get env '%s' fail\n", ENV_SPI_LAYOUT);
        return -1;
    }
    p1 = malloc(strlen(pEnv)+1);
    if (p1 == NULL)
    {
        SPI_ERROR("malloc fail\n");
        return -1;
    }
    p2 = p1;
    memset(p1, 0, strlen(pEnv)+1);
    strcpy(p1, pEnv);
    SPI_TRACE("p1=%s\n", p1);
    while((p1 = get_next_member_info(p1, member_name, &member_size))!=NULL)
    {
        ret = add_partition(member_name,member_size);
        if (ret != 0)
        {
            free(p2);
            SPI_ERROR("parse mtdpart fail\n");
            return -1;
        }
    }

    free(p1);
    SPI_TRACE("OK\n");
    return 0;
}

static int get_spi_partition_info(char *name, unsigned int *offset, unsigned int *size)
{
    ST_SPI_PARTITION *stSpiPartition = NULL;
    char exist = 0;

    if (name == NULL)
    {
        SPI_ERROR("'name' is a null pointer\n");
        return -1;
    }

    if (offset == NULL)
    {
        SPI_ERROR("'offset' is a null pointer\n");
        return -1;
    }

    if (size == NULL)
    {
        SPI_ERROR("'size' is a null pointer\n");
        return -1;
    }
    if (list_empty(&spi_partition_list) == 1)
    {
        SPI_ERROR("no partition\n");
        return -1;
    }

    list_for_each_entry(stSpiPartition, &spi_partition_list, list)
    {
        if (strcmp(stSpiPartition->name, name) == 0)
        {
            exist=1;
            break;
        }
    }

    if (exist == 1)
    {
        *offset = stSpiPartition->offset;
        *size = stSpiPartition->size;
        SPI_TRACE("OK\n");
        return 0;
    }
    else
    {
        SPI_ERROR("%s doesn't exist\n", name);
        return -1;
    }

}

static int init_spi_partition(void)
{
    int ret = 0;

    INIT_LIST_HEAD(&spi_partition_list);
    partition_list_ready = 1;

    ret = getSpiInfo(&spiInfo);
    if (ret != 0)
    {
        SPI_ERROR("get spi info fail\n");
        return -1;
    }

    parse_mtdpart();

    ret = add_mboot_partition();

    if (ret != 0)
    {
        SPI_ERROR("Add MBoot partition fail\n");
        return -1;
    }

    SPI_TRACE("OK\n");
    return 0;
}

static int list_partition(void)
{
    ST_SPI_PARTITION *stSpiPartition = NULL;

    if (partition_list_ready == 0)
    {
        SPI_ERROR("spi partition list is not ready\n");
        return -1;
    }

    if (list_empty(&spi_partition_list) == 1)
    {
        SPI_ERROR("no partition\n");
        return -1;
    }

    printf("The partiton layout in SPI\n");
    printf("num     name    offset      size\n");

    list_for_each_entry(stSpiPartition, &spi_partition_list, list)
    {
        //UBOOT_INFO("num     name    offset      size\n");
          printf("%d     %s     0x%x    0x%x\n", stSpiPartition->num, stSpiPartition->name, stSpiPartition->offset, stSpiPartition->size);
    }

    SPI_TRACE("OK\n");
    return 0;
}

int _spi_rdc(unsigned int dram_addr, unsigned int flash_addr, unsigned int len)
{
    SPI_TRACE("dram_addr=0x%x\n", dram_addr);
    SPI_TRACE("flash_addr=0x%x\n", flash_addr);
    SPI_TRACE("len=0x%x\n", len);

    /* check alignment and show warning*/
    if (flash_addr % SERFLASH_BLOCK_SIZE)
    {
        SPI_ERROR("WARNING: it is better to set flash start addr aligned to %d !!!\n", SERFLASH_BLOCK_SIZE);
    }

    if (dram_addr % SERFLASH_BLOCK_SIZE)
    {
        SPI_ERROR("WARNING: it is better to set dram start addr aligned to %d !!!\n", SERFLASH_BLOCK_SIZE);
    }

    if (len % SERFLASH_BLOCK_SIZE)
    {
        SPI_ERROR("WARNING: it is better to set total length aligned to %d !!!\n", SERFLASH_BLOCK_SIZE);
    }


    /* init SPI flash first */
    if (0 == spiInitFlag)
    {
        spi_flash_init();
    }

    /* read from flash to dram */
    if (0 == MDrv_SERFLASH_Read(flash_addr, len, (MS_U8 *)dram_addr))
    {
        SPI_ERROR("ERROR: SPI DMA fail !!!\n");
        return 1;
    }
    else
    {
        SPI_TRACE("OK\n");
        return 0;
    }
}

int _spi_wrc(unsigned int dram_addr, unsigned int flash_addr, unsigned int len)
{
    MS_U8 *dram_addr_for_verify = NULL;

    SPI_TRACE("dram_addr=0x%x\n", dram_addr);
    SPI_TRACE("flash_addr=0x%x\n", flash_addr);
    SPI_TRACE("len=0x%x\n", len);

    /* init SPI flash first */
    if (0 == spiInitFlag)
    {
        spi_flash_init();
    }

    /* SPI erase */
    printf("Erasing...\n");
    MDrv_SERFLASH_WriteProtect(FALSE);

    if (len >= 0x10000) // a bank size is 64KBytes
    {
        printf("block erase\n");
        if (0 == MDrv_SERFLASH_AddressErase(flash_addr, len, TRUE))
        {
            SPI_ERROR("FAIL !!!\n");
            return 1;
        }
    }
    else
    {
        if (len % 0x1000 != 0) // Check 4K alignment
        {
            SPI_ERROR("Wrong Alignment Length, FAIL !!!\n");
            return 1;
        }
        printf("sector erase\n");
        if (0 == MDrv_SERFLASH_SectorErase(flash_addr, flash_addr+len-1))
        {
            SPI_ERROR("FAIL !!!\n");
            return 1;
        }
    }

    /* SPI write */
    printf("Writing...\n");
    MDrv_SERFLASH_Write(flash_addr, len, (MS_U8 *)dram_addr);
    MDrv_SERFLASH_WriteProtect(TRUE);

    /* SPI verify */
    unsigned int u32VerifySize = 0;
    unsigned int u32VerifyOffset = 0;
    printf("Verifying...\n");
    dram_addr_for_verify = malloc(SERFLASH_BLOCK_SIZE);//((dram_addr+len-1+SERFLASH_BLOCK_SIZE) / SERFLASH_BLOCK_SIZE) * SERFLASH_BLOCK_SIZE;/* fix expression of dram_end_address for readability */
    if (dram_addr_for_verify == NULL)
    {
        SPI_ERROR("malloc fail\n");
        return -1;
    }
    while (len > 0)
    {
        if (len >= SERFLASH_BLOCK_SIZE)
        {
            u32VerifySize = SERFLASH_BLOCK_SIZE;
        }
        else
        {
            u32VerifySize=len;
        }

        if (0 != MDrv_SERFLASH_Read(flash_addr+u32VerifyOffset, u32VerifySize, dram_addr_for_verify))
        {
            if(memcmp((void*)(dram_addr+u32VerifyOffset),dram_addr_for_verify,u32VerifySize) != 0)
            {
               free(dram_addr_for_verify);
               SPI_ERROR("FAIL !!!\n");
               return -1;
            }
        }
        len -= u32VerifySize;
        u32VerifyOffset += SERFLASH_BLOCK_SIZE;
    }
    free(dram_addr_for_verify);
    SPI_TRACE("OK\n");
    return 0;

}
#endif

/*
 * SPI init
 * Syntax
 * spi_init"
 */

int do_spi_init ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    return spi_flash_init();
}

int do_spi_get_Chip_Rev ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    uchar uGetChipRev = 0;
    //TODO: uGetChipRev=MDrv_SERFLASH_GetChipRev();
    printf("Get Chip Rev:\t0x%02x\n", uGetChipRev);
    return 0;
}

int do_spi_get_flash_info ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    pFlashInfo = MDrv_SERFLASH_GetInfo();
    printf(" Section Num:\t0x%04x\n",      pFlashInfo->u32SecNum);
    printf(" Section Size:\t0x%08x\n",     pFlashInfo->u32SecSize);
    printf(" Total Size:\t0x%08x\n",     pFlashInfo->u32TotalSize);
    printf(" Access Width:\t0x%02x\n",   pFlashInfo->u32AccessWidth);
    return 0;
}

int do_spi_read_status ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    uchar u8StatusReg;
    MDrv_SERFLASH_ReadStatusRegister(&u8StatusReg);
    printf(" u8StatusReg:\t0x%02X\n", u8StatusReg);
    return 0;
}

int do_spi_dma ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    uint uflhaddr,udmaaddr;
    uint len;

    printf("DMA mode doesn't supported\n");
    return 1;

#ifdef DMA_TEST
    uint idx;
    uchar buffer[MAX_SPI_BYTES];
#endif
    if ((argc < DMA_INPUT_LEN) || (argc > DMA_INPUT_LEN))
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    uflhaddr = simple_strtoul(argv[1], NULL, 16);
    udmaaddr = simple_strtoul(argv[2], NULL, 16);
    len      = simple_strtoul(argv[3], NULL, 16);

    printf ("SPI flash start addr:\t0x%4x\n",uflhaddr);
    printf ("SPI DMA start addr:\t0x%4x\n",udmaaddr);
    printf ("SPI DMA length:\t0x%4x\n",len);

    if (len > MAX_SPI_BYTES)
    {
        printf ("SPI DMA: length is out of range\n");
        return 1;
    }
#ifdef DMA_TEST
        //////DMA Test start
        for( idx=0; idx<len; idx++)
        {
            buffer[idx] = (uchar)idx;
        }
        MDrv_SERFLASH_Write(uflhaddr, len, buffer);

        flush_cache(udmaaddr,len);
        MDrv_SERFLASH_DMA (uflhaddr, udmaaddr, len);

        for( idx=0; idx<len; idx++)
        {
            if( *(volatile uchar *)( (uint)KSEG02KSEG1(udmaaddr)+idx ) != (uchar)idx )
            {
                printf ("SPI DMA fail!\n");
            }
        }
        printf ("SPI DMA compare pass!\n");
        //////DMA Test end
#else
        MDrv_SERFLASH_DMA (uflhaddr, udmaaddr, len);
        printf ("SPI DMA Done!\n");
#endif

    return 0;
}

int do_spi_readID( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    uchar buffer[READ_ID_LENGTH];
    uint len;

    if ((argc < READ_ID_INPUT_LEN) || (argc > READ_ID_INPUT_LEN))
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    len = READ_ID_LENGTH;
    MDrv_SERFLASH_ReadID (buffer, len);

    printf("Manufacturer ID:\t0x%02X\n", buffer[0]);
    printf("Device ID:\t0x%02X 0x%02X\n", buffer[1], buffer[2]);

    return 0;
}

/*
 * SPI read
 * Syntax
 * spi_r ID(hex) addr(hex) len(hex)\n"
 */
int do_spi_read ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    uint addr;
    uint len;
    uint idx;

    if ((argc < READ_INPUT_LEN) || (argc > READ_INPUT_LEN))
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    addr = simple_strtoul(argv[1], NULL, 16);
    len  = simple_strtoul(argv[2], NULL, 16);

    printf ("SPI read addr:0x%08x and len:0x%08x\n",addr,len);

    if (len > MAX_SPI_BYTES)
    {
        printf ("SPI read: length is out of range\n");
        return 1;
    }

    MDrv_SERFLASH_Read ((uint)addr, len, ubuffer);//uID

    for(idx=0;idx<len;idx++)
    {
        if((idx%16)==0)
        {
            printf("[0x%08x~0x%08x]: ", (addr+idx), (addr+idx+15));
        }

        printf("%02X ", ubuffer[idx]);

        if(((idx+1)%16)==0)
        {
            printf("\n");
        }
    }

    printf("Read SPI flash done!\n");

    return 0;
}

int do_spi_read_buff ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    uint addr;
    uint len;
    uint idx;

    if ((argc < READ_BUFF_INPUT_LEN) || (argc > READ_BUFF_INPUT_LEN))
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    addr = simple_strtoul(argv[1], NULL, 16);
    len  = simple_strtoul(argv[2], NULL, 16);

    for(idx=0;idx<len;idx++)
    {
        if((idx%16)==0)
        {
            printf("[0x%08x~0x%08x]: ", (addr+idx), (addr+idx+15));
        }

        printf("%02X ",ubuffer[idx]);

        if(((idx+1)%16)==0)
        {
            printf("\n");
        }
    }
    printf("Read buffer done!\n");

    return 0;
}

int do_spi_write_protect ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    uchar uEnable;
    if ((argc < WRITE_PRO_INPUT_LEN) || (argc > WRITE_PRO_INPUT_LEN))
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    uEnable = simple_strtoul(argv[1], NULL, 16);

    if(MDrv_SERFLASH_WriteProtect(uEnable)==0)
    {
        printf ("SPI write protect fail\n");
    }
    else
    {
        printf ("SPI write protect pass\n");
    }

    return 0;
}

int do_spi_write_buff ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    uint addr;
    uint val;
    uint len;
    uint i;

    if ((argc < WRITE_BUFF_INPUT_LEN) || (argc > WRITE_BUFF_INPUT_LEN))
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    addr    = simple_strtoul(argv[1], NULL, 16);
    val     = simple_strtoul(argv[2], NULL, 16);
    len     = simple_strtoul(argv[3], NULL, 16);

    if (len > MAX_SPI_BYTES)
    {
        printf ("SPI write: length is out of range\n");
        return 1;
    }

    for(i=0; i<len; i++)
    {
        ubuffer[i]=val;
        //printf("buffer %x is %08X\n",i,val);
    }
    printf("Write buffer done!\n");

    return 0;
}

/*
 * SPI write
 */
int do_spi_write ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    uint addr;
    uint len;

    if ((argc < WRITE_INPUT_LEN) || (argc > WRITE_INPUT_LEN))
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    addr    = simple_strtoul(argv[1], NULL, 16);
    len     = simple_strtoul(argv[2], NULL, 16);

    if (len > MAX_SPI_BYTES)
    {
        printf ("SPI write: length is out of range\n");
        return 1;
    }

#ifdef SPI_WRITE_TEST //for testing
    for(i=0;i<len;i++)
    {
        ubuffer[i]=i;
    }
#endif

    MDrv_SERFLASH_Write ((uint)addr, len, ubuffer);
    printf("write SPI flash done!\n");

    return 0;
}

int do_spi_erase_chip ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    MDrv_SERFLASH_EraseChip ();

    return 0;
}

int do_spi_erase_block ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    if ((argc < ERASE_SEC_INPUT_LEN) || (argc > ERASE_SEC_INPUT_LEN))
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    uint ustartAddr = simple_strtoul(argv[1], NULL, 16);
    uint uendAddr = simple_strtoul(argv[2], NULL, 16);

    if (ustartAddr > uendAddr)
    {
        printf("Start Addr > End Addr");
        return 1;
    }

    printf("ustartAddr = 0x%X, uendAddr = 0x%X\n", ustartAddr, uendAddr);
    return MDrv_SERFLASH_AddressErase (ustartAddr, uendAddr - ustartAddr, TRUE) ? 0 : -1;
}

/*
 * SPI read/write
 *
 * Syntax:
 *   spi {dev} {num_bits} {dout}
 *     {dev} is the device number for controlling chip select (see TBD)
 *     {num_bits} is the number of bits to send & receive (base 10)
 *     {dout} is a hexadecimal string of data to send
 * The command prints out the hexadecimal string received via SPI.
 */

int do_spi (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    char  *cp = 0;
    uchar tmp;
    int   j;
    int   rcode = 0;

    /*
     * We use the last specified parameters, unless new ones are
     * entered.
     */

    if ((flag & CMD_FLAG_REPEAT) == 0)
    {
        if (argc >= 2)
        {
            device = simple_strtoul(argv[1], NULL, 10);
        }

        if (argc >= 3)
        {
            bitlen = simple_strtoul(argv[2], NULL, 10);
        }

        if (argc >= 4)
        {
            cp = argv[3];
            for(j = 0; *cp; j++, cp++)
            {
                tmp = *cp - '0';
                if(tmp > 9)
                {
                    tmp -= ('A' - '0') - 10;
                }

                if(tmp > 15)
                {
                    tmp -= ('a' - 'A');
                }

                if(tmp > 15)
                {
                    printf("Hex conversion error on %c, giving up.\n", *cp);
                    return 1;
                }

                if((j % 2) == 0)
                {
                    dout[j / 2] = (tmp << 4);
                }
                else
                {
                    dout[j / 2] |= tmp;
                }
            }
        }
    }

    if ((device < 0) || (device >=  spi_chipsel_cnt))
    {
        printf("Invalid device %d, giving up.\n", device);
        return 1;
    }

    if ((bitlen < 0) || (bitlen >  (MAX_SPI_BYTES * 8)))
    {
        printf("Invalid bitlen %d, giving up.\n", bitlen);
        return 1;
    }

#if 0 //Test
    if(spi_xfer(spi_chipsel[device], bitlen, dout, din) != 0) {
        printf("Error with the SPI transaction.\n");
        rcode = 1;
    } else {
        cp = (char *)din;
        for(j = 0; j < ((bitlen + 7) / 8); j++) {
            printf("%02X", *cp++);
        }
        printf("\n");
    }
#endif
    return rcode;
}

int do_spi_rdc (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    unsigned int flash_addr;
    unsigned int dram_addr;
    unsigned int len;

    /* check argc */
    if (argc != READ_CODE_INPUT_LEN)
    {
        printf("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    arg_off_size_nor(argc - 2, argv + 2, &flash_addr, &len);
    //flash_addr = simple_strtoul(argv[2], NULL, 16);
	dram_addr = (0 == strncmp(argv[1], UPGRADE_ADDR_STR, strlen(UPGRADE_ADDR_STR))) ? UPGRADE_BUFFER_ADDR : simple_strtoul(argv[1], NULL, 16);
    //len = simple_strtoul(argv[3], NULL, 16);

    /* check alignment and show warning*/
    if (flash_addr % SERFLASH_BLOCK_SIZE)
    {
        printf("WARNING: it is better to set flash start addr aligned to %d !!!\n", SERFLASH_BLOCK_SIZE);
    }

    if (dram_addr % SERFLASH_BLOCK_SIZE)
    {
        printf("WARNING: it is better to set dram start addr aligned to %d !!!\n", SERFLASH_BLOCK_SIZE);
    }

    if (len % SERFLASH_BLOCK_SIZE)
    {
        printf("WARNING: it is better to set total length aligned to %d !!!\n", SERFLASH_BLOCK_SIZE);
    }

    /* init SPI flash first */
    if (0 == spiInitFlag)
    {
        spi_flash_init();
    }

    BDMA_Result ret;
    ret = MDrv_BDMA_CopyHnd(flash_addr,dram_addr,len,E_BDMA_FLASH2SDRAM,0x8);
	printf("Start read %lx data from serial device... \n",len);
    if(ret != E_BDMA_OK)
    {
      printf("ERROR: BDMA fail and Change to use the Spi_read!!! %s %d\n",__func__,__LINE__);
       /* read from flash to dram */
      if(0 == MDrv_SERFLASH_Read(flash_addr, len, (MS_U8 *)dram_addr))
      {
        printf("ERROR: SPI DMA fail !!!\n");
        return 1;
      }
      else
      {
        return 0;
      }
    }
    else
    {
      MsOS_Dcache_Flush(dram_addr, len);
      MsOS_FlushMemory();
      return 0;
    }
}
int do_spi_wrc (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    unsigned int dram_addr;
    unsigned int flash_addr;
    unsigned int len;
    unsigned int dram_addr_for_verify;

    /* check argc */
    if (argc != WRITE_CODE_INPUT_LEN)
    {
        printf("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    arg_off_size_nor(argc - 2, argv + 2, &flash_addr, &len);
	dram_addr = (0 == strncmp(argv[1], UPGRADE_ADDR_STR, strlen(UPGRADE_ADDR_STR))) ? UPGRADE_BUFFER_ADDR : simple_strtoul(argv[1], NULL, 16);
    //flash_addr = simple_strtoul(argv[2], NULL, 16);
    //len = simple_strtoul(argv[3], NULL, 16);

    /* check alignment and show warning*/
    if (dram_addr % SERFLASH_BLOCK_SIZE)
    {
        printf("WARNING: it is better to set dram start addr aligned to %d !!!\n", SERFLASH_BLOCK_SIZE);
    }

    if (flash_addr % SERFLASH_BLOCK_SIZE)
    {
        printf("WARNING: it is better to set flash start addr aligned to %d !!!\n", SERFLASH_BLOCK_SIZE);
    }

    if (len % SERFLASH_BLOCK_SIZE)
    {
        printf("WARNING: it is better to set total length aligned to %d !!!\n", SERFLASH_BLOCK_SIZE);
    }

    /* init SPI flash first */
    if (0 == spiInitFlag)
    {
        spi_flash_init();
    }

    pFlashInfo = MDrv_SERFLASH_GetInfo();

    if((flash_addr + len) > pFlashInfo->u32TotalSize)
    {
        printf("FAIL : execute the total size of SPI Flash \n");
        return 1;
    }

    /* SPI erase */
    printf("Erasing...\n");
    MDrv_SERFLASH_WriteProtect(FALSE);
    MDrv_SERFLASH_AddressErase(flash_addr, len, TRUE);

    /* SPI write */
    printf("Writing...\n");
    MDrv_SERFLASH_Write(flash_addr, len, (MS_U8 *)dram_addr);
    MDrv_SERFLASH_WriteProtect(TRUE);

    /* SPI verify */
    printf("Verifying...");
    dram_addr_for_verify = ((dram_addr+len-1+SERFLASH_BLOCK_SIZE) / SERFLASH_BLOCK_SIZE) * SERFLASH_BLOCK_SIZE;/* fix expression of dram_end_address for readability */
    if (0 != MDrv_SERFLASH_Read(flash_addr, len, (MS_U8 *)dram_addr_for_verify))
    {
        int i;
        for (i = 0; i < len; i++)
        {
            if (((unsigned char *)dram_addr)[i] != ((unsigned char *)dram_addr_for_verify)[i])
            {
                break;
            }
        }

        if (i == len)
        {
            printf("OK !!!\n");
            return 0;
        }
    }

    printf("FAIL !!!\n");

    return 1;
}

int do_spi_init_partition(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
#if defined(CONFIG_BOOT_SPI_LINUX)
    int ret = 0;

    if (argc != 1)
    {
        SPI_ERROR("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }
    ret = init_spi_partition();
    if (ret == 0)
    {
        SPI_TRACE("OK\n");
    }
    else
    {   SPI_ERROR("init spi partition fail\n");
    }
    return ret;
#else
    printf("This function doesn't support\n");
    return -1;
#endif
}

int do_spi_add_partition(char *name, unsigned size)
{
#if defined(CONFIG_BOOT_SPI_LINUX)
    int ret = 0;
    unsigned int _spi_size = 0;
    unsigned int cur_offset = 0;
    unsigned int last_num = 0;
    ST_SPI_PARTITION *stSpiPartition = NULL;

    if (partition_list_ready == 0)
    {
        SPI_ERROR("spi partition list is not ready\n");
        return -1;
    }

    if (name == NULL)
    {
        SPI_ERROR("'name' is a null pointer\n");
        return -1;
    }

    if (size == 0)
    {
        SPI_ERROR("The size of 0x%x is zero\n", size);
        return -1;
    }

    if((strlen(name)+1) > SPI_PARTITON_NAME_SIZE)
    {
        SPI_ERROR("The length of %s is over 0x%x\n", name, SPI_PARTITON_NAME_SIZE);
        return -1;
    }

    //alignment to block size
    SPI_TRACE("origin size=0x%x\n", size);
    if ((size%spiInfo.u32SecSize) != 0)
    {
        size+=(spiInfo.u32SecSize-size%spiInfo.u32SecSize);
    }
    SPI_TRACE("final size=0x%x\n",size);

    ret = getSpiSize(&_spi_size);
    if (ret!=0)
    {
        SPI_ERROR("get spi's size fail\n");
        return -1;
    }

    if (size > _spi_size)
    {
        SPI_ERROR("This partition size is larger than this SPI size\n");
        return -1;
    }

    //Check if this partition exist or not
     list_for_each_entry(stSpiPartition, &spi_partition_list, list)
     {
         SPI_TRACE("stSpiPartition->name=%s\n", stSpiPartition->name);
         if (strcmp(stSpiPartition->name, name) == 0)
         {
             SPI_ERROR("This partition alraedy exist\n");
             return -1;
         }
         cur_offset+=stSpiPartition->size;
         last_num=stSpiPartition->num;
     }


    //Check whether or not the total size is over the SPI size.
    SPI_TRACE("cur_offset=0x%x\n", cur_offset);
    if ((cur_offset+size) > _spi_size)
    {
        SPI_ERROR("The total size is over the size of this SPI\n");
        return -1;
    }

    //Create a partition info for this new partition
    stSpiPartition = malloc(sizeof(ST_SPI_PARTITION));
    if (stSpiPartition == NULL)
    {
        SPI_ERROR("malloc for stSpiPartition fail\n");
        return -1;
    }
    memset(stSpiPartition, 0, sizeof(ST_SPI_PARTITION));

    ret = add2MtdPart(name,size);
    if (ret != 0)
    {
        free(stSpiPartition);
        SPI_ERROR("edit env 'mtdpart' fail\n");
        return -1;
    }

    INIT_LIST_HEAD(&stSpiPartition->list);
    strcpy(stSpiPartition->name, name);
    stSpiPartition->offset = cur_offset;
    stSpiPartition->size = size;
    stSpiPartition->num = last_num + 1;
    SPI_TRACE("stSpiPartition->name=%s\n", stSpiPartition->name);
    SPI_TRACE("stSpiPartition->offset=0x%x\n", stSpiPartition->offset);
    SPI_TRACE("stSpiPartition->size=0x%x\n", stSpiPartition->size);
    SPI_TRACE("stSpiPartition->num=0x%x\n", stSpiPartition->num);
    list_add_tail(&stSpiPartition->list, &spi_partition_list);
    SPI_TRACE("OK\n");
    return 0;
#else
    printf("This function doesn't support\n");
    return -1;
#endif
}

int do_spi_create (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
#if defined(CONFIG_BOOT_SPI_LINUX)
    unsigned int size = 0;
    int ret = 0;

    if (argc != CREATE_PARTITION_INPUT_LEN)
    {
        SPI_ERROR("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }

    size = simple_strtol(argv[2], NULL, 16);
    ret = do_spi_add_partition(argv[1], size);

    if (ret == 0)
    {
        SPI_TRACE("OK\n");
    }
    else
    {
        SPI_ERROR("create partition fail\n");
    }
    return ret;
#else
    printf("This function doesn't support\n");
    return -1;
#endif
}

int do_spi_rdc_partition (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
#if defined(CONFIG_BOOT_SPI_LINUX)
    #define DRAM_ADDR argv[1]
    #define PARTITION_NAME argv[2]
    #define READ_OFFSET argv[3]
    #define READ_LEN argv[4]

    unsigned int dram_addr;
    unsigned int flash_addr;
    unsigned int roffset;
    unsigned int rlen;
    unsigned int partiton_offset = 0;
    unsigned int partiton_size = 0;
    int ret = 0;

    SPI_TRACE("DRAM_ADDR=%s\n", DRAM_ADDR);
    SPI_TRACE("PARTITION_NAME=%s\n", PARTITION_NAME);
    SPI_TRACE("READ_OFFSET=%s\n", READ_OFFSET);
    SPI_TRACE("READ_LEN=%s\n", READ_LEN);

    /* check argc */
    if (argc != READ_PARTITION_INPUT_LEN)
    {
        SPI_ERROR("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    rlen = simple_strtoul(READ_LEN, NULL, 16);
    roffset = simple_strtoul(READ_OFFSET, NULL, 16);
    dram_addr = (0 == strncmp(DRAM_ADDR, UPGRADE_ADDR_STR, strlen(UPGRADE_ADDR_STR))) ? UPGRADE_BUFFER_ADDR : simple_strtoul(DRAM_ADDR, NULL, 16);
    ret = get_spi_partition_info(PARTITION_NAME, &partiton_offset, &partiton_size);

    if (ret != 0)
    {
        SPI_ERROR("get %s info fail\n", PARTITION_NAME);
        return -1;
    }

    if ((roffset+rlen) > partiton_size)
    {
        SPI_ERROR("The read area is over this partition range\n");
        return -1;
    }

    flash_addr = roffset+partiton_offset;

    SPI_TRACE("flash addr=0x%x\n", flash_addr);


    ret = _spi_rdc(dram_addr, flash_addr, rlen);
    if (ret == 0)
    {
        SPI_TRACE("OK\n");
    }
    else
    {
        SPI_ERROR("spi read fail\n");
    }
    return ret;

    #undef DRAM_ADDR
    #undef PARTITION_NAME
    #undef READ_OFFSET
    #undef READ_LEN
#else
    printf("This function doesn't support\n");
    return -1;
#endif
}

int do_spi_wrc_partition (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
#if defined(CONFIG_BOOT_SPI_LINUX)
    #define DRAM_ADDR argv[1]
    #define PARTITION_NAME argv[2]
    #define WRITE_OFFSET argv[3]
    #define WRITE_LEN argv[4]

    unsigned int dram_addr;
    unsigned int flash_addr;
    unsigned int woffset;
    unsigned int wlen;
    unsigned int partiton_offset = 0;
    unsigned int partiton_size = 0;
    int ret = 0;

    SPI_TRACE("DRAM_ADDR=%s\n", DRAM_ADDR);
    SPI_TRACE("PARTITION_NAME=%s\n", PARTITION_NAME);
    SPI_TRACE("WRITE_OFFSET=%s\n", WRITE_OFFSET);
    SPI_TRACE("WRITE_LEN=%s\n", WRITE_LEN);

    /* check argc */
    if (argc != WRITE_PARTITION_INPUT_LEN)
    {
        SPI_ERROR("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }

    dram_addr = (0 == strncmp(DRAM_ADDR, UPGRADE_ADDR_STR, strlen(UPGRADE_ADDR_STR))) ? UPGRADE_BUFFER_ADDR : simple_strtoul(DRAM_ADDR, NULL, 16);
    wlen = simple_strtoul(WRITE_LEN, NULL, 16);
    woffset = simple_strtoul(WRITE_OFFSET, NULL, 16);
    ret = get_spi_partition_info(PARTITION_NAME, &partiton_offset, &partiton_size);
    if (ret != 0)
    {
        SPI_ERROR("get %s info fail\n", PARTITION_NAME);
        return -1;
    }

    if ((woffset+wlen) > partiton_size)
    {
        SPI_ERROR("The write area is over this partition range\n");
        return -1;
    }

    flash_addr = woffset + partiton_offset;
    SPI_TRACE("flash addr=0x%x\n", flash_addr);
    ret=_spi_wrc(dram_addr,flash_addr,wlen);
    if(ret==0)
    {
        SPI_TRACE("OK\n");
    }
    else
    {
        SPI_ERROR("write partition fail\n");
    }
    return ret;

    #undef DRAM_ADDR
    #undef PARTITION_NAME
    #undef WRITE_OFFSET
    #undef WRITE_LEN
#else
    printf("This function doesn't support\n");
    return -1;
#endif
}

int do_spi_rmgpt(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
#if defined(CONFIG_BOOT_SPI_LINUX)
    int ret = 0;

    if (argc != RMGPT_INPUT_LEN)
    {
        SPI_ERROR("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }

    ret = del_all_partitions();
    if (ret == 0)
    {
        ret = add_mboot_partition();
        if (ret == -1)
        {
            SPI_ERROR("add mboot partition fail\n");
            return -1;
        }
    }
    else
    {
        SPI_ERROR("remove partition fail\n");
    }

    return ret;
#else
    printf("This function doesn't support\n");
    return -1;
#endif
}

int do_spi_part (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
#if defined(CONFIG_BOOT_SPI_LINUX)
    int ret = 0;

    if (argc!=1)
    {
        SPI_ERROR("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }
    ret = list_partition();

    if (ret == 0)
    {
        SPI_TRACE("OK\n");
    }
    else
    {   SPI_ERROR("spi part fail\n");
    }
    return ret;
#else
    printf("This function doesn't support\n");
    return -1;
#endif

}

/***************************************************/

U_BOOT_CMD(
    sspi,       BASE_CMD_INPUT_LEN,           1,    do_spi,
    "sspi    - SPI utility commands\n",
    "<device> <bit_len> <dout> - Send <bit_len> bits from <dout> out the SPI\n"
    "<device>  - Identifies the chip select of the device\n"
    "<bit_len> - Number of bits to send (base 10)\n"
    "<dout>    - Hexadecimal string that gets sent\n"
);

U_BOOT_CMD(
    spi_in,     INIT_INPUT_LEN,               1,    do_spi_init,
    "spi_in  - SPI initialization\n",
    "command: spi_in\n"
);

U_BOOT_CMD(
    spi_id,     READ_ID_INPUT_LEN,           1,    do_spi_readID,
    "spi_id  - SPI read ID\n",
    "command: spi_id\n"
);

U_BOOT_CMD(
    spi_r,      READ_INPUT_LEN,               1,    do_spi_read,
    "spi_r   - SPI read commands\n",
    "command: spi_r addr(hex) len(hex)\n"
);

U_BOOT_CMD(
    spi_w,      WRITE_INPUT_LEN,           1,    do_spi_write,
    "spi_w   - SPI write commands\n",
    "command: spi_w addr(hex) len(hex) \n"
);

U_BOOT_CMD(
    spi_ea,     ERASE_CHIP_INPUT_LEN,       1,    do_spi_erase_chip,
    "spi_ea  - SPI erase all\n",
    "command: spi_ea\n"
);

U_BOOT_CMD(
    spi_eb,     ERASE_SEC_INPUT_LEN,       1,    do_spi_erase_block,
    "spi_eb  - SPI erase block\n",
    "command: spi_es start addr(hex) end addr(hex)\n"
);

U_BOOT_CMD(
    spi_wp,     WRITE_PRO_INPUT_LEN,       1,    do_spi_write_protect,
    "spi_wp  - SPI write protect \n",
    "command: spi_wp (enable(1)or disable(0))\n"
);

U_BOOT_CMD(
    spi_gr,     GET_CHIP_REV_INPUT_LEN,       1,    do_spi_get_Chip_Rev,
    "spi_gr  - SPI get Chip Rev\n",
    "command: spi_gr \n"
);

U_BOOT_CMD(
    spi_gfo,    GET_FLASH_INFO_INPUT_LEN,  1,    do_spi_get_flash_info,
    "spi_gfo - SPI get flash info\n",
    "command:spi_gfo\n"
);

U_BOOT_CMD(
    spi_rs,     READ_STATUS_INPUT_LEN,       1,    do_spi_read_status,
    "spi_rs  - SPI read status\n",
    "command:spi_rs\n"
);

U_BOOT_CMD(
    spi_dma,    DMA_INPUT_LEN,           1,    do_spi_dma,
    "spi_dma - SPI copy data from flash to DRAM by PIU DMA\n",
    "command: spi_dma flash start addr(hex) DRAM start addr(hex) len(hex)\n"
);

U_BOOT_CMD(
    spi_wb,     WRITE_BUFF_INPUT_LEN,       1,    do_spi_write_buff,
    "spi_wb  - SPI write buffer\n",
    "command: spi_ewb edit addr(hex) value(hex) len(hex)\n"
);

U_BOOT_CMD(
    spi_rb,     READ_BUFF_INPUT_LEN,        1,    do_spi_read_buff,
    "spi_rb  - SPI read buffer\n",
    "command: spi_rb addr(hex) len(hex)\n"
);

U_BOOT_CMD(
    spi_rdc,    READ_CODE_INPUT_LEN,    1,  do_spi_rdc,
    "spi_rdc - SPI read code from SPI flash to DRAM\n",
    "to_dram_addr(hex) from_flash_addr(hex) len(hex)\n"
    "    - to_dram_addr: dram start address (hex, flash erase size aligned)\n"
    "    - from_flash_addr: flash start address (hex, flash erase size aligned)\n"
    "    - len: total lenght to move data (hex, flash erase size aligned)\n"
);

U_BOOT_CMD(
    spi_wrc,    WRITE_CODE_INPUT_LEN,   1,  do_spi_wrc,
    "spi_wrc - SPI write code from DRAM to SPI flash\n",
    "from_dram_addr(hex) to_flash_addr(hex) len(hex)\n"
    "    - from_dram_addr: dram start address (hex, flash erase size aligned)\n"
    "    - to_flash_addr: flash start address (hex, flash erase size aligned)\n"
    "    - len: total lenght to move data (hex, flash erase size aligned)\n"
);

U_BOOT_CMD(
    spi_rdc_p,    READ_PARTITION_INPUT_LEN,   1,  do_spi_rdc_partition,
    "spi_rdc_p  : read data from DRAM to SPI partiton\n",
    "    [dram_addr(hex)] [partition name][offset(hex)] [len(hex)]\n"
    "    - dram_addr: dram start address (hex, flash erase size aligned)\n"
    "    - partition name: The partition name\n"
    "    - offset: offset in partition (hex, flash erase size aligned)\n"
    "    - len: total lenght to move data (hex, flash erase size aligned)\n"
);

U_BOOT_CMD(
    spi_wrc_p,    WRITE_PARTITION_INPUT_LEN,   1,  do_spi_wrc_partition,
    "spi_wrc_p  : write data from DRAM to SPI partiton\n",
    "    [dram_addr(hex)] [partition name][offset(hex)] [len(hex)]\n"
    "    - dram_addr: dram start address (hex, flash erase size aligned)\n"
    "    - partition name: The partition name\n"
    "    - offset: offset in partition (hex, flash erase size aligned)\n"
    "    - len: total lenght to move data (hex, flash erase size aligned)\n"
);

U_BOOT_CMD(
    spi_create,    CREATE_PARTITION_INPUT_LEN,   1,  do_spi_create,
    "spi_create : create a partition\n",
    "   -spi create [name] [size(hex)]\n"
);

U_BOOT_CMD(
    spi_rmgpt,    RMGPT_INPUT_LEN,   1,  do_spi_rmgpt,
    "spi_rmgpt : remove all partition\n",
);

U_BOOT_CMD(
    spi_ptinit,    INIT_PARTITION_INPUT_LEN,   1,  do_spi_init_partition,
    "spi_ptinit : init partition table\n",
);

U_BOOT_CMD(
    spi_part,    PART_INPUT_LEN,   1,  do_spi_part,
    "spi_part : list all partition info\n",
);

#endif

