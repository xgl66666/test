////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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

/*
 * Tuner handling routines
 */

#include <common.h>
#include <command.h>
#include <configs/uboot_module_config.h>
#include <sys/common/MsTypes.h>
#include <sys/common/MsIRQ.h>
#include <sys/common/MsOS.h>
#include <sys/drv/drvNAND.h>

#if(ENABLE_MODULE_MSTAR_NAND)

int do_createnandcis(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    U32 CISReserveByteCnt;
    U32 CodeReserveByteCnt;
    U16 LastReserveBlkCnt;

    CISReserveByteCnt     = (U32)simple_strtoul(argv[1], NULL, 16);
    CodeReserveByteCnt  = (U32)simple_strtoul(argv[2], NULL, 16);
    LastReserveBlkCnt  = (U16)simple_strtoul(argv[3], NULL, 16);

    MDrv_Create_CIS(CISReserveByteCnt,CodeReserveByteCnt,LastReserveBlkCnt);

    return 0;
}


int do_initnand(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{

    if(MDrv_NAND_Init() == true)
    {
        return 0;
    }
    else
    {
        return 1;
    }

}

int do_getnandcapacity(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{

    U32 u32Capacity;

    printf ("---------------------------------Get NAND Capacity-----------------------------------\n");

    u32Capacity = Get_Capacity();

    printf ("Get NAND Capacity sector number = 0x%lx \n",u32Capacity);
    return 0;
}


int do_readnand(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{

    U32 u32_StartSectorIdx;
    U32 u32_SectorCnt;
    U32 u32_RWBufferAddress;


    u32_StartSectorIdx     = (U32)simple_strtoul(argv[1], NULL, 16);
    u32_SectorCnt  = (U32)simple_strtoul(argv[2], NULL, 16);
    u32_RWBufferAddress  = (U32)simple_strtoul(argv[3], NULL, 16);


    printf ("do_readnand u32_StartSectorIdx = 0x%lx,  u32_SectorCnt = 0x%lx, u32_RWBufferAddress = 0x%lx \n",u32_StartSectorIdx,u32_SectorCnt, u32_RWBufferAddress);

    nand_ReadFlow(u32_StartSectorIdx, u32_SectorCnt, u32_RWBufferAddress);
    MsOS_Dcache_Invalidate(u32_RWBufferAddress, u32_SectorCnt << 9);


    return 0;



}


int do_writenand(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    U32 u32_StartSectorIdx;
    U32 u32_SectorCnt;
    U32 u32_RWBufferAddress;


    u32_StartSectorIdx     = (U32)simple_strtoul(argv[1], NULL, 16);
    u32_SectorCnt  = (U32)simple_strtoul(argv[2], NULL, 16);
    u32_RWBufferAddress  = (U32)simple_strtoul(argv[3], NULL, 16);

    printf ("do_writenand u32_StartSectorIdx = 0x%lx,  u32_SectorCnt = 0x%lx, u32_RWBufferAddress = 0x%lx \n",u32_StartSectorIdx,u32_SectorCnt, u32_RWBufferAddress);

    nand_WriteFlow(u32_StartSectorIdx, u32_SectorCnt, u32_RWBufferAddress);

    return 0;



}

int do_readnand_phy(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{

    U32 u32_PartitionNumber = 0;
    U32 u32_StartSectorIdx = 0;
    U32 u32_SectorCnt = 0;
    U32 u32_RWBufferAddress = 0;

    u32_PartitionNumber = (U32)simple_strtoul(argv[1], NULL, 16);
    u32_StartSectorIdx     = (U32)simple_strtoul(argv[2], NULL, 16);
    u32_SectorCnt  = (U32)simple_strtoul(argv[3], NULL, 16);
    u32_RWBufferAddress  = (U32)simple_strtoul(argv[4], NULL, 16);


    printf ("do_readnand_phy u32_StartSectorIdx = 0x%lx,  u32_SectorCnt = 0x%lx, u32_RWBufferAddress = 0x%lx \n",u32_StartSectorIdx,u32_SectorCnt, u32_RWBufferAddress);

    nand_ReadFlow_Phy(u32_PartitionNumber, u32_RWBufferAddress, u32_SectorCnt, u32_StartSectorIdx);
    MsOS_Dcache_Invalidate(u32_RWBufferAddress, u32_SectorCnt << 9);


    return 0;



}


int do_writenand_phy(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    U32 u32_PartitionNumber = 0;
    U32 u32_StartSectorIdx = 0;
    U32 u32_SectorCnt = 0;
    U32 u32_RWBufferAddress = 0;

    u32_PartitionNumber = (U32)simple_strtoul(argv[1], NULL, 16);
    u32_StartSectorIdx     = (U32)simple_strtoul(argv[2], NULL, 16);
    u32_SectorCnt  = (U32)simple_strtoul(argv[3], NULL, 16);
    u32_RWBufferAddress  = (U32)simple_strtoul(argv[4], NULL, 16);

    printf ("do_writenand_phy u32_StartSectorIdx = 0x%lx,  u32_SectorCnt = 0x%lx, u32_RWBufferAddress = 0x%lx \n",u32_StartSectorIdx,u32_SectorCnt, u32_RWBufferAddress);

    nand_WriteFlow_Phy(u32_PartitionNumber, u32_RWBufferAddress, u32_SectorCnt, u32_StartSectorIdx);

    return 0;

}

int do_erasenand_phy(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    U32 u32_PartitionNumber = 0;

    u32_PartitionNumber = (U32)simple_strtoul(argv[1], NULL, 16);

    printf ("do_erasenand_phy u32_PartitionNumber = 0x%lx\n",u32_PartitionNumber);

    nand_ErasePartition_Phy(u32_PartitionNumber);

    return 0;

}

U_BOOT_CMD(
	mstar_createnandcis,	4,	1,	do_createnandcis,
	"mstar_createnandcis     -create nand cis info\n"
	"arg[1]: CIS Reserve Byte Cnt\n"
	"arg[2]: Code Reserve Byte Cnt \n"
	"arg[3]: Last Reserve Blk Cnt \n",
);

U_BOOT_CMD(
	mstar_initnand,	1,	1,	do_initnand,
	"mstar_initnand     - init nand\n",
);

U_BOOT_CMD(
	mstar_getnandcapacity,	1,	1,	do_getnandcapacity,
	"mstar_getnandcapacity     - Get NAND Capacity sector number\n"
	"command - mstar_getnandcapacity\n",
);


U_BOOT_CMD(
	mstar_readnand,	4,	1,	do_readnand,
	"mstar_readnand     - read data from nand\n"
	"arg[1]: Start Sector Index \n"
	"arg[2]: Sector Count \n"
	"arg[3]: Read Buffer Address \n",
);


U_BOOT_CMD(
	mstar_writenand, 4,	1,	do_writenand,
	"mstar_writenand     - write data to nand\n"
	"arg[1]: Start Sector Index \n"
	"arg[2]: Sector Count \n"
	"arg[3]: write Buffer Address \n",
);

U_BOOT_CMD(
	mstar_readnand_phy,	5,	1,	do_readnand_phy,
	"mstar_readnand     - read data from nand\n"
	"arg[1]: Partition Number \n"
	"arg[2]: Start Sector Index \n"
	"arg[3]: Sector Count \n"
	"arg[4]: Read Buffer Address \n",
);


U_BOOT_CMD(
	mstar_writenand_phy, 5,	1,	do_writenand_phy,
	"mstar_writenand     - write data to nand\n"
	"arg[1]: Partition Number \n"
	"arg[2]: Start Sector Index \n"
	"arg[3]: Sector Count, minimum write size is one page. Eg, if a nand with 2Kbytes page, at least you should write 4 sector once \n"
	"arg[4]: write Buffer Address \n",
);

U_BOOT_CMD(
	mstar_erasenand_phy, 2,	1,	do_erasenand_phy,
	"mstar_writenand     - write data to nand\n"
	"arg[1]: Partition Number \n",
);
/* =============================================================== */

#endif /* (CONFIG_COMMANDS) */

