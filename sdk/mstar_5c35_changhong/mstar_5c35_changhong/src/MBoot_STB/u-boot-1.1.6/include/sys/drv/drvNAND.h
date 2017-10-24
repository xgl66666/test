////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvNAND.h
/// @brief  NAND Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_NAND_H_
#define _DRV_NAND_H_

#define AUTO_FORMAT_FTL 1

#ifdef __cplusplus
extern "C"
{
#endif
//datatype
#define REG_OFFSET_SHIFT_BITS    2
#define REG(Reg_Addr)           (*(volatile MS_U16*)(Reg_Addr))
#define GET_REG_ADDR(x, y)      ((x)+((y) << REG_OFFSET_SHIFT_BITS))

#define REG_WRITE_UINT16(reg_addr, val)      REG((reg_addr)) = (val)
#define REG_READ_UINT16(reg_addr, val)       val = REG((reg_addr))
#define REG_SET_BITS_UINT16(reg_addr, val)   REG((reg_addr)) |= (val)
#define REG_CLR_BITS_UINT16(reg_addr, val)   REG((reg_addr)) &= ~(val)
#define REG_W1C_BITS_UINT16(reg_addr, val)   REG_WRITE_UINT16((reg_addr), REG((reg_addr))&(val))

#if 0
#define BIT0            0x00000001
#define BIT1            0x00000002
#define BIT2            0x00000004
#define BIT3            0x00000008
#define BIT4            0x00000010
#define BIT5            0x00000020
#define BIT6            0x00000040
#define BIT7            0x00000080
#define BIT8            0x00000100
#define BIT9            0x00000200
#define BIT10           0x00000400
#define BIT11           0x00000800
#define BIT12           0x00001000
#define BIT13           0x00002000
#define BIT14           0x00004000
#define BIT15           0x00008000
#endif
#define BIT16           0x00010000
#define BIT17           0x00020000
#define BIT18           0x00040000
#define BIT19           0x00080000
#define BIT20           0x00100000
#define BIT21           0x00200000
#define BIT22           0x00400000
#define BIT23           0x00800000
#define BIT24           0x01000000
#define BIT25           0x02000000
#define BIT26           0x04000000
#define BIT27           0x08000000
#define BIT28           0x10000000
#define BIT29           0x20000000
#define BIT30           0x40000000
#define BIT31           0x80000000

#define UNFD_FTL_VERSION    0
#define UNFD_ROOT_BLK_CNT   1
#define MAX_LUT_CNT                 5
#define LUT_PBA_CNT                 0x400
#define UNFD_FREE_BLK_CNT_PER_1K    16 // [CAUTION]: logical area at least need to reserved 16 blocks
                                       // FreeBlk Cnt includes (also used to cover) bad blks
#define LUT_LBA_CNT                 (LUT_PBA_CNT-UNFD_FREE_BLK_CNT_PER_1K)

#define UNFD_CACHE_LINE   128
//=====================================================
// tool-chain attributes
//=====================================================
#define UNFD_PACK         __attribute__((packed))
#define UNFD_ALIGN0       //__attribute((aligned(UNFD_CACHE_LINE)))
#define UNFD_ALIGN1       __attribute((aligned(UNFD_CACHE_LINE)))

#include "drvNAND_config.h"
#include "drvNAND_err_codes.h"

//===================================================
// Blk IDs
#define UNFD_FTL_BLK_ID_ROOT    0
#define UNFD_FTL_BLK_ID_CTRL    1
#define UNFD_FTL_BLK_ID_LUT     2
#define UNFD_FTL_BLK_ID_PPB     4 // partial page blk
#define UNFD_FTL_BLK_ID_DATA    5
#define UNFD_FTL_BLK_ID_WLECNT  6
#define UNFD_FTL_BLK_ID_FMAP    7
#define UNFD_FTL_BLK_ID_RD      8
#define UNFD_FTL_BLK_ID_END     UNFD_FTL_BLK_ID_RD
#define UNFD_FTL_BLK_ID_CODE    9

//configure
#ifndef MSOS_TYPE_NOS
    #define UNFD_FTL_WBQ    1
    #define UNFD_FTL_FMAP   1
    #define UNFD_FTL_WL     1
    #define UNFD_FTL_RD     1
    #define BLD_LD_OS_RD    0
    #define UNFD_FTL_WL_TEST    1
    #define UNFD_FTL_RD_TEST    0
#else
    #define UNFD_FTL_WBQ    0
    #define UNFD_FTL_FMAP   1
    #define UNFD_FTL_WL     0
    #define UNFD_FTL_RD     0
    #define BLD_LD_OS_RD    0
    #define UNFD_FTL_WL_TEST    0
    #define UNFD_FTL_RD_TEST    0
#endif
//===================================================
#if UNFD_FTL_WBQ
#define MAX_WBQ_CNT  4
#endif

#if UNFD_FTL_FMAP
//----------------------------------------
// [CAUTION]: any change for this feature,
//            we have to change UNFD_FTL_VERSION to do Low Level Format.
//----------------------------------------
#define FMAP_MAX_SECTOR_CNT     0x500
#define FMAP_SPACE_FACTOR       4 // one blk in FMAP holds the 1/FMAP_SPACE_FACTOR sectors of original blk
#define FMAP_MAX_LBA_CNT        40
#define FMAP_MAX_BLKPAGE_CNT    0x80 // [CAUTION]
#endif

#if UNFD_FTL_WL
#define WL_EC_THRESHOLD		16
#define WL_SAVE_EC_TIMES	100
#define WL_CHECK_EC_MASK	(0x0004-1)
#endif


#if UNFD_FTL_WBQ
typedef struct _WBQ {

    volatile MS_U16 u16_LBA;
    volatile MS_U16 u16_OldPBA;
    volatile MS_U16 u16_NewPBA;    // pick up from FreeBlk

    volatile MS_U16 u16_PagePos;   // points to the "next & free" page, in the u16_NewPBA.
    volatile MS_U16 u16_SectorPos; // if have partial page data (page not fulfilled)
	                            // co-work with pNandDrv->u16_PPB & u16_PPBPageIdx
    volatile MS_U16 u16_PPBPageIdx;// if SectorPos is 0, then don't care PPBPageIdx
} WBQ_t, *P_WBQ_t;
//extern MS_U32 nand_FlushWBQ(MS_U8 u8_WBQIdx);
#endif  //UNFD_FTL_WBQ

#define BIT_MAP_PHY 0

//===================================================
typedef struct _DATA_BLK_SPARE{
    MS_U8  u8_BadBlkMark;
	MS_U8  u8_BlkID;
	MS_U16 u16_LBA;
	MS_U8  u8_RDD4;
	MS_U8  u8_RDD5;

} UNFD_PACK DATA_BLK_SPARE_t, *P_DATA_BLK_SPARE_t;

//----------------------------
// RootBlk
typedef struct _ROOT_BLK_DATA{
    MS_U32 u32_Tmp;

} UNFD_PACK ROOT_BLK_DATA_t, *P_ROOT_BLK_DATA_t;


typedef struct _ROOT_BLK_SPARE{
    MS_U8 u8_BadBlkMark;
	MS_U8 u8_BlkID;
	MS_U8 u8_FTLVerNum;
	MS_U8 u8_RDD3;
	MS_U8 u8_RDD4;
	MS_U8 u8_RDD5;

} UNFD_PACK ROOT_BLK_SPARE_t, *P_ROOT_BLK_SPARE_t;


#if UNFD_FTL_WBQ
//----------------------------
// PartialPageBlk
typedef struct _PPB_BLK_SPARE{
    MS_U8  u8_BadBlkMark;
	MS_U8  u8_BlkID;
	MS_U8  u8_WBQIdx;
	MS_U8  u8_RDD3;
	MS_U8  u8_RDD4;
	MS_U8  u8_RDD5;

} UNFD_PACK PPB_BLK_SPARE_t, *P_PPB_BLK_SPARE_t;
#endif

#if UNFD_FTL_FMAP
//----------------------------
// FMapBlk
typedef struct _FMAP_BLK_SPARE{
    MS_U8  u8_BadBlkMark;
	MS_U8  u8_BlkID;
	MS_U16 u16_LBA;
	MS_U8  u8_PageIdx;
	MS_U8  u8_RDD5;

} UNFD_PACK FMAP_BLK_SPARE_t, *P_FMAP_BLK_SPARE_t;
#endif

#if UNFD_FTL_WL
//----------------------------
// WLECntBlk
typedef struct _WLECNT_BLK_SPARE{
    MS_U8  u8_BadBlkMark;
	MS_U8  u8_BlkID;
	MS_U8  u8_ECntIdx;
	MS_U8  u8_ECntSubIdx;
	MS_U8  u8_RDD4;
	MS_U8  u8_RDD5;

} UNFD_PACK WLECNT_BLK_SPARE_t, *P_WLECNT_BLK_SPARE_t;
#endif

#if UNFD_FTL_RD
//----------------------------
// RDBlk
typedef struct _RD_BLK_DATA{
    MS_U16 u16_SrcPBA;
	MS_U16 u16_DestPBA;
	MS_U32 u32_Checksum;

} UNFD_PACK RD_BLK_DATA_t, *P_RD_BLK_DATA_t;

typedef struct _RD_BLK_SPARE{
    MS_U8  u8_BadBlkMark;
	MS_U8  u8_BlkID;
	MS_U8  u8_St;
	MS_U8  u8_RDD3;
	MS_U8  u8_RDD4;
	MS_U8  u8_RDD5;

} UNFD_PACK RD_BLK_SPARE_t, *P_RD_BLK_SPARE_t;

#define UNFD_FTL_RD_ST_IDLE    0
#define UNFD_FTL_RD_ST_START   1
#endif


void MDrv_Create_CIS(MS_U16 u16_CISReserveBlkCnt, MS_U16 u16_CodeReserveBlkCnt, MS_U16 u16_LastReserveBlkCnt);
#if 0
static MS_U8 drvNAND_ParseID(void);
static MS_U8 drvNAND_ParseID_2(void);
static MS_U8 drvNandCheckFlashType(void);
#endif

MS_U32 drvNAND_CheckAlignPack(MS_U8 u8_AlignByteCnt);
MS_BOOL MDrv_NAND_Init(void);
int drv_unfd_init(void);
MS_U32 drvNAND_Init(void);
MS_U32 nand_Init_FTL(void);

#if 0
static MS_U32 nand_init_parameters(void);
static MS_U32 nand_search_RootBlk(void);
static MS_U32 nand_build_LUT0(void);
static void nand_build_LUT1(void);
static MS_U16 nand_DuplicatePBAHandler(MS_U16 u16_PBA0, MS_U16 u16_PBA1, MS_U16 u16_LBA);
static void nand_set_FreeBlk(MS_U16 u16_PBA);
static void nand_erase_and_set_FreeBlk(MS_U16 u16_PBA);
#endif

MS_U32 nand_Low_Level_Format(void);
MS_U32 drvNAND_IsGoodBlk(MS_U16 u16_BlkIdx);

#if 0
static MS_U32 nand_format_DataBlk(MS_U16 u16_PBA, MS_U16 u16_LBA);
#endif

MS_U32 nand_WriteFlow(MS_U32 u32_StartSectorIdx, MS_U32 u32_SectorCnt, MS_U32 u32_RWBufferAddress);
MS_U32 nand_WriteFlow_X(MS_U32 u32_StartSectorIdx, MS_U32 u32_SectorCnt, MS_U32 u32_RWBufferAddress);

#if 0
static MS_U16 nand_get_FreeBlk_Zone0(void);
static MS_U16 nand_get_FreeBlk_Zone1(void);
static MS_U32 nand_copy_data(MS_U16 u16_ToBlk, MS_U16 u16_SectorInBlk, MS_U16 u16_FromBlk, MS_U16 u16_SectorCnt);
#endif

MS_U32 nand_ReadFlow(MS_U32 u32_StartSectorIdx, MS_U32 u32_SectorCnt, MS_U32 u32_RWBufferAddress);
MS_U32 nand_ReadFlow_X(MS_U32 u32_StartSectorIdx, MS_U32 u32_SectorCnt, MS_U32 u32_RWBufferAddress);

#if 0
static MS_U32 nand_ReadPages(MS_U32 u32_PhyRowIdx, MS_U8 *pu8_DataBuf, MS_U8 *pu8_SpareBuf, MS_U32 u32_PageCnt);
static MS_U32 nand_ReadSectors(MS_U32 u32_PhyRowIdx, MS_U8 u8_SectorInPage, MS_U8 *pu8_DataBuf, MS_U8 *pu8_SpareBuf, MS_U32 u32_SectorCnt);
static MS_U32 nand_WritePages(MS_U32 u32_PhyRowIdx, MS_U8 *pu8_DataBuf, MS_U8 *pu8_SpareBuf, MS_U32 u32_PageCnt);
#endif

void *drvNAND_get_DrvContext_address(void);
void *drvNAND_get_DrvContext_PartInfo(void);

MS_U32 drvNAND_FlushCache(void);
#if UNFD_FTL_WBQ

#if 0
static void nand_reset_WBQs(void);
static MS_U32 nand_WBQ_WriteBack(MS_U8 u8_WBQIdx, MS_U16 u16_SectorInBlkCopyEnd);
static void nand_WBQ_UpdatePPB(void);
#endif

MS_U32 nand_FlushWBQ(MS_U8 u8_WBQIdx);
#endif
#if UNFD_FTL_FMAP

#if 0
static void nand_FM_MoveBlk(MS_U8 u8_i, MS_U8 u8_j);
static MS_U32 nand_FM_ReadFlow(MS_U32 u32_StartSectorIdx, MS_U32 u32_SectorCnt, MS_U32 u32_RWBufferAddress);
static MS_U32 nand_FM_WriteFlow(MS_U32 u32_StartSectorIdx, MS_U32 u32_SectorCnt, MS_U32 u32_RWBufferAddress);
static MS_U32 nand_format_FMapBlk(MS_U16 u16_PBA, MS_U16 u16_LBA);
#endif

#endif
#if UNFD_FTL_WL

#if 0
static MS_U32 nand_format_ECBlk(MS_U16 u16_PBA, MS_U8 u8_ECntIdx);
static MS_U16 nand_DuplicateECHandler(MS_U16 u16_PBA0, MS_U16 u16_PBA1, MS_U8 u8_ECntIdx);
static MS_U32 nand_FindMaxECntFreeBlkInZone0(MS_U16 *pu16_PBA, MS_U16 *pu16_MaxECnt, MS_U8 *pu8_Pos);
static MS_U32 nand_FindMaxECntFreeBlkInZone1(MS_U16 *pu16_PBA, MS_U16 *pu16_MaxECnt, MS_U8 *pu8_Pos);
static MS_U32 nand_delete_FreeBlk_Zone1(MS_U8 u8_FBPos);
#endif

void nand_SaveEraseCounter(MS_U8 u8_EraseCounterZoneIndex);
void nand_Wear_Leveling_X(void);
void nand_Wear_Leveling(void);
void nand_Wear_Leveling1_X(void);
void nand_Wear_Leveling1(void);
#endif
#if UNFD_FTL_RD

#if 0
static void nand_validate_WLRD_PBA(MS_U16 u16_PBA, MS_U8 u8_ActID);
static void nand_RD_ErrorHandler(void);
static void nand_RD_UpdateRDB(void);
static void nand_RD_WriteRDBPage(MS_U8 u8_RD_Status);
#endif

void nand_ReadDisturbance(void);
#endif

//drvNAND_utl.c
MS_U32 drvNAND_ProbeReadSeq(void);
MS_U8 drvNAND_CountBits(MS_U32 u32_x);
MS_U32 drvNAND_SearchCIS(void);
MS_U32 drvNAND_CheckSum(MS_U8 *pu8_Data, MS_U16 u16_ByteCnt);
MS_U32 drvNAND_CheckPartInfo(MS_U32 u32_PageIdx);
MS_U32 drvNAND_CompareCISTag(MS_U8 *tag);
MS_U32 drvNAND_MarkBadBlk(MS_U16 u16_BlkIdx);

#if 0
static MS_U16 nand_FindFreePage(MS_U16 u16_PBA);
#endif

//drvNAND_hal.h
MS_U32 NC_ConfigContext(void);
MS_U32 NC_Init(void);
MS_U32 NC_ResetFCIE(void);
MS_U32 NC_ResetNandFlash(void);
MS_U32 NC_ProbeReadSeq(void);
MS_U32 NC_ReadID(void);
MS_U32 NC_ReadSectors(MS_U32 u32_PhyRowIdx, MS_U8 u8_SectorInPage, MS_U8 *pu8_DataBuf, MS_U8 *pu8_SpareBuf, MS_U32 u32_SectorCnt);
MS_U32 NC_ReadPages(MS_U32 u32_PhyRowIdx, MS_U8 *pu8_DataBuf, MS_U8 *pu8_SpareBuf, MS_U32 u32_PageCnt);
MS_U32 NC_WritePages(MS_U32 u32_PhyRowIdx, MS_U8 *pu8_DataBuf, MS_U8 *pu8_SpareBuf, MS_U32 u32_PageCnt);
MS_U32 NC_WriteSectors(MS_U32 u32_PhyRowIdx, MS_U8 u8_SectorInPage, MS_U8 *pu8_DataBuf, MS_U8 *pu8_SpareBuf, MS_U32 u32_SectorCnt);
MS_U32 NC_WaitComplete(MS_U16 u16_WaitEvent, MS_U32 u32_MicroSec);
void NC_GetCIFD(MS_U8 *pu8_Buf, MS_U32 u32_CIFDPos, MS_U32 u32_ByteCnt);
void NC_SetCIFD(MS_U8 *pu8_Buf, MS_U32 u32_CIFDPos, MS_U32 u32_ByteCnt);
void NC_SetCIFD_Const(MS_U8 u8_Data, MS_U32 u32_CIFDPos, MS_U32 u32_ByteCnt);
void NC_ReConfig(void);
void NC_Config(void);
void NC_waitFifoClkReady(void);
MS_U32 NC_wait_MIULastDone(void);
MS_U32 NC_EraseBlk(MS_U32 u32_PhyRowIdx);
MS_U32 NC_CheckEWStatus(MS_U8 u8_OpType);
MS_U32 NC_ReadSector_RIUMode(MS_U32 u32_PhyRowIdx, MS_U8 u8_SectorInPage, MS_U8 *pu8_DataBuf, MS_U8 *pu8_SpareBuf);
MS_U32 NC_WaitComplete_RM(MS_U16 u16_WaitEvent, MS_U32 u32_MicroSec);
MS_U32 NC_PageCopy(MS_U32 u32_SrcPhyRowIdx, MS_U32 u32_DstPhyRowIdx, MS_U8 *pu8_DataBuf, MS_U8 *pu8_SpareBuf, MS_U32 u32_PageCnt);
MS_U32 Get_Capacity(void);
//drvNAND_platform.h
MS_U32 nand_hw_timer_delay(MS_U32 u32usTick);
MS_U32 NC_PlatformInit(void);
MS_U32 nand_pads_switch(MS_U32 u32EnableFCIE);
MS_U32 nand_clock_setting(MS_U32 u32ClkParam);
MS_U32 nand_translate_DMA_address_Ex(MS_U32 u32_DMAAddr, MS_U32 u32_ByteCnt);

#ifndef MSOS_TYPE_NOS
void drvNAND_CreateWriteBackTimer(void);
void drvNAND_SendMsg2BkgndTask(MS_U16 TypMsg);
void drvNAND_BkgndWriteBackParser(MS_U32 stTimer, MS_U32 u32Data);
void drvNAND_BkgndWearLevelingParser(MS_U32 stTimer, MS_U32 u32Data);
void drvNAND_BkgndWearLeveling1Parser(MS_U32 stTimer, MS_U32 u32Data);
void nandBackgroundTask(void);
void drvNAND_ResetTimers(void);
#endif

MS_U32 nand_WriteFlow_Phy(MS_U8 u8PartitionNo, MS_U32 buf, MS_U32 len, MS_U32 u32_SecNum);
MS_U16 drvNAND_FindNextGoodBlockForImageWrite(MS_U16 u16Start, MS_U16 u16End);
MS_U32 nand_ReadFlow_Phy(MS_U8 u8PartitionNo, MS_U32 buf, MS_U32 len, MS_U32 u32_SecNum);
MS_U16 drvNAND_FindNextGoodBlock(MS_U16 u16Start, MS_U16 u16End);
MS_U32 nand_ErasePartition_Phy(MS_U8 u8PartitionNo);

#if BIT_MAP_PHY
void Getbitmap(void);
#endif

MS_U8   gu8_ftl_ready;
//drvNAND_reg_v3.h
//------------------------------------------------------------------
#define RIU_BASE                0xBF200000U //0x74000000U

// #define REG_BANK_MPLL        0x0980U	// FIXME
#define REG_BANK_CLKGEN         0x0580U	// FIXME
#define REG_BANK_CHIPTOP        0x0F00U	// FIXME
#define REG_BANK_FCIE0          0x8980U //0x1400U
#define REG_BANK_FCIE1          0x89E0U //0x1480U
#define REG_BANK_FCIE2          0x8A00U //0x1500U
#define REG_BANK_FCIE3          0x8A80U //0x1580U

#define CLKGEN_BASE             GET_REG_ADDR(RIU_BASE, REG_BANK_CLKGEN)
#define CHIPTOP_BASE			GET_REG_ADDR(RIU_BASE, REG_BANK_CHIPTOP)
#define FCIE0_BASE              GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE0)
#define FCIE1_BASE              GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE1)
#define FCIE2_BASE              GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2)
#define FCIE3_BASE              GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE3)

#define CLKGEN_BASE_ADDR        CLKGEN_BASE
#define FCIE_REG_BASE_ADDR      FCIE0_BASE
#define FCIE_NC_CIFC_ADDR       FCIE1_BASE
#define FCIE_NC_CIFD_BASE       FCIE2_BASE

#define CLK_NFIE            GET_REG_ADDR(CLKGEN_BASE_ADDR, 0x64)
#define CLK_NFIE_DRIVE      GET_REG_ADDR(CLKGEN_BASE_ADDR, 0x65)

#define NC_MIE_EVENT        GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x00)
#define NC_MIE_INT_EN       GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x01)
#define NC_MMA_PRI_REG      GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x02)
#define NC_MIU_DMA_SEL      GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x03)
#define NC_CARD_DET         GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x07)
#define NC_FORCE_INT        GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x08)
#define NC_PATH_CTL         GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x0A)
#define NC_JOB_BL_CNT       GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x0B)
#define NC_TR_BK_CNT        GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x0C)
#define NC_SDIO_CTL         GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x1B)
#define NC_SDIO_ADDR0       GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x1C)
#define NC_SDIO_ADDR1       GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x1D)
#define NC_R2N_STAT         GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x1F)
#define NC_R2N_CTRL         GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x20)
#define NC_R2N_DATA_RD      GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x21)
#define NC_R2N_DATA_WR      GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x22)
#define NC_CIF_FIFO_CTL     GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x25)
#define FCIE_NC_REORDER     GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x2D)
#define NC_MIU_OFFSET       GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x2E)
#define NC_REG_PAD_SWITCH   GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x2F)
#define NC_TEST_MODE        GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x30)
#define NC_DEBUG_DBUS0      GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x31)
#define NC_DEBUG_DBUS1      GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x32)
#define NC_PWRSAVE_MASK     GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x34)
#define NC_PWRSAVE_CTL      GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x35)
#define NC_CELL_DELAY       GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x37)
#define NC_MIU_WPRT_L1      GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x38)
#define NC_MIU_WPRT_L0      GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x39)
#define NC_MIU_WPRT_H1      GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x3A)
#define NC_MIU_WPRT_H0      GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x3B)
#define NC_MIU_WPRT_ER1     GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x3C)
#define NC_MIU_WPRT_ER0     GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x3D)
#define NC_FCIE_VERSION     GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x3F)
#define NC_SIGNAL           GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x40)
#define NC_WIDTH            GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x41)
#define NC_STAT_CHK         GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x42)
#define NC_AUXREG_ADR       GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x43)
#define NC_AUXREG_DAT       GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x44)
#define NC_CTRL             GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x45)
#define NC_ST_READ          GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x46)
#define NC_PART_MODE        GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x47)
#define NC_SPARE            GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x48)
#define NC_SPARE_SIZE       GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x49)
#define NC_ADDEND           GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x4A)
#define NC_SIGN_DAT         GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x4B)
#define NC_SIGN_ADR         GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x4C)
#define NC_SIGN_CTRL        GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x4D)
#define NC_SPARE_DMA_ADR0   GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x4E)
#define NC_SPARE_DMA_ADR1   GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x4F)
#define NC_ECC_CTRL         GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x50)
#define NC_ECC_STAT0        GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x51)
#define NC_ECC_STAT1        GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x52)
#define NC_ECC_STAT2        GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x53)
#define NC_ECC_LOC          GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x54)
#define NC_RAND_R_CMD       GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x55)
#define NC_RAND_W_CMD       GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x56)
#define NC_ECO_FIX          GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x57)

#define NC_CIFD_ADDR(u16_pos)   GET_REG_ADDR(FCIE_NC_CIFD_BASE, u16_pos) // 256 x 16 bits

#define NC_MAX_SECTOR_BYTE_CNT        (BIT12-1)
#define NC_MAX_SECTOR_SPARE_BYTE_CNT  (BIT8-1)
#define NC_MAX_TOTAL_SPARE_BYTE_CNT   (BIT11-1)

#define ECC_TYPE_RS                 1
#define ECC_TYPE_4BIT               2
#define ECC_TYPE_8BIT               3
#define ECC_TYPE_12BIT              4
#define ECC_TYPE_16BIT              5
#define ECC_TYPE_20BIT              6
#define ECC_TYPE_24BIT              7
#define ECC_TYPE_24BIT1KB           8
#define ECC_TYPE_32BIT1KB           9

#define ECC_CODE_BYTECNT_RS         10
#define ECC_CODE_BYTECNT_4BIT        7
#define ECC_CODE_BYTECNT_8BIT       13
#define ECC_CODE_BYTECNT_12BIT      20
#define ECC_CODE_BYTECNT_16BIT      26
#define ECC_CODE_BYTECNT_20BIT      33
#define ECC_CODE_BYTECNT_24BIT      39
#define ECC_CODE_BYTECNT_24BIT1KB   42
#define ECC_CODE_BYTECNT_32BIT1KB   56

/* NC_MIE_INT_EN 0x01 */
#define BIT_MMA_DATA_END           BIT0
#define BIT_NC_JOB_END             BIT9
#define BIT_NC_R2N_RDY             BIT10
#define BIT_NC_R2N_ECC             BIT12
#define BIT_NC_POWER_SAVE          BIT13

/* NC_MMA_PRI_REG 0x02 */
#define BIT_NC_DMA_DIR_W           BIT2
#define BIT_NC_FIFO_CLKRDY         BIT5

/* NC_PATH_CTL 0x0A */
#define BIT_MMA_EN                 BIT0
#define BIT_NC_EN                  BIT5
#define BIT_MIU_LAST_DONE          BIT7

/* NC_SDIO_CTL 0x1B */
#define MASK_SDIO_BLK_SIZE_MASK    (BIT12-1)
#define BIT_SDIO_BLK_MODE          BIT15

/* NC_R2N_CTRL_GET 0x1F */
#define BIT_RIU_RDY_MMA						BIT0
#define R_DMA_RD4_NFC						BIT_RIU_RDY_MMA
#define R_RIU2NAND_DIR						BIT1

/* NC_R2N_CTRL_SET 0x20 */
#define R_RIU2NAND_LB_EN					BIT8
#define BIT_R2N_DO_END						BIT6
#define R_RIU2NAND_W_END					BIT_R2N_DO_END
#define BIT_R2N_DO_EN						BIT5
#define R_RIU2NAND_W_EN						BIT_R2N_DO_EN
#define BIT_R2N_DO_START					BIT4
#define R_RIU2NAND_W_STR					BIT_R2N_DO_START
#define BIT_R2N_DI_END						BIT3
#define R_RIU2NAND_R_END					BIT_R2N_DI_END
#define BIT_R2N_DI_EN						BIT2
#define R_RIU2NAND_R_EN						BIT_R2N_DI_EN
#define BIT_R2N_DI_START					BIT1
#define R_RIU2NAND_R_STR					BIT_R2N_DI_START
#define BIT_R2N_MODE_EN						BIT0
#define R_RIU2NAND_EN						BIT_R2N_MODE_EN

/* NC_TEST_MODE 0x30 */
#define BIT_FCIE_BIST_FAIL         (BIT0|BIT1|BIT2|BIT3)
#define BIT_FCIE_DEBUG_MODE_SHIFT  8
#define BIT_FCIE_DEBUG_MODE        (BIT10|BIT9|BIT8)
#define BIT_FCIE_SOFT_RST          BIT12
#define BIT_FCIE_PPFIFO_CLK        BIT14

/* NC_SIGNAL 0x40 */
#define BIT_NC_CE_SEL_MASK         (BIT2-1)
#define BIT_NC_CE_H                BIT2
#define BIT_NC_CE_AUTO             BIT3
#define BIT_NC_WP_H                BIT4
#define BIT_NC_WP_AUTO             BIT5
#define BIT_NC_CHK_RB_HIGH         BIT6
#define BIT_NC_CHK_RB_EDGEn        BIT7
#define DEF_REG0x40_VAL            BIT_NC_CE_H

/* NC_CTRL 0x45 */
#define BIT_NC_JOB_START           BIT0
#define BIT_NC_CIFD_ACCESS         BIT1
#define BIT_NC_IF_DIR_W            BIT3

/* NC_ST_READ 0x46 */
#define BIT_ST_READ_FAIL           BIT0
#define BIT_ST_READ_BUSYn          BIT6
#define BIT_ST_READ_PROTECTn       BIT7

/* NC_PART_MODE 0x47*/
#define BIT_PARTIAL_MODE_EN        BIT0
#define BIT_START_SECTOR_CNT_MASK  (BIT6|BIT5|BIT4|BIT3|BIT2|BIT1)
#define BIT_START_SECTOR_CNT_SHIFT 1
#define BIT_START_SECTOR_IDX_MASK  (BIT12|BIT11|BIT10|BIT9|BIT8|BIT7)
#define BIT_START_SECTOR_IDX_SHIFT 7

/* NC_SPARE 0x48 */
#define BIT_NC_SECTOR_SPARE_SIZE_MASK     (BIT8-1)
#define BIT_NC_SPARE_DEST_MIU             BIT8
#define BIT_NC_SPARE_NOT_R_IN_CIFD        BIT9
#define BIT_NC_RANDOM_RW_OP_EN            BIT11
#define BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE BIT12
#define BIT_NC_ONE_COL_ADDR               BIT13

/* NC_ECC_CTRL 0x50*/
#define BIT_NC_PAGE_SIZE_MASK      (BIT3-1)
#define BIT_NC_PAGE_SIZE_512Bn     BIT_NC_PAGE_SIZE_MASK
#define BIT_NC_PAGE_SIZE_2KB       BIT0
#define BIT_NC_PAGE_SIZE_4KB       BIT1
#define BIT_NC_PAGE_SIZE_8KB       (BIT1|BIT0)
#define BIT_NC_PAGE_SIZE_16KB      BIT2
#define BIT_NC_PAGE_SIZE_32KB      (BIT2|BIT0)
#define BIT_NC_ECC_TYPE_MASK       (BIT3|BIT4|BIT5|BIT6)
#define BIT_NC_ECC_TYPE_4b512Bn    BIT_NC_ECC_TYPE_MASK
#define BIT_NC_ECC_TYPE_8b512B     BIT3
#define BIT_NC_ECC_TYPE_12b512B    BIT4
#define BIT_NC_ECC_TYPE_24b1KB     (BIT5|BIT4)
#define BIT_NC_ECC_TYPE_32b1KB     (BIT5|BIT4|BIT3)
#define BIT_NC_ECC_TYPE_RS         BIT6
#define BIT_NC_ECCERR_NSTOP        BIT7
#define BIT_NC_DYNAMIC_OFFCLK      BIT8
#define BIT_NC_ALL0xFF_ECC_CHECK   BIT9
#define BIT_NC_BYPASS_ECC          BIT10
#define BIT_NC_SHARE_PAD_EN        BIT14
#define BIT_NC_WORD_MODE           BIT15

/* NC_ECC_STAT0 0x51*/
#define BIT_NC_ECC_FAIL            BIT0
#define BIT_NC_ECC_MAX_BITS_MASK   (BIT6|BIT5|BIT4|BIT3|BIT2|BIT1)
#define BIT_NC_ECC_MAX_BITS_SHIFT  1
#define BIT_NC_ECC_SEC_CNT_MASK    (BIT13|BIT12|BIT11|BIT10|BIT9|BIT8)

/* NC_ECC_STAT2 0x53*/
#define BIT_NC_ECC_FLGA_MASK				(BIT1|BIT0)
#define BIT_NC_ECC_FLGA_NOERR				0
#define BIT_NC_ECC_FLGA_CORRECT				BIT0
#define BIT_NC_ECC_FLGA_FAIL				BIT1
#define BIT_NC_ECC_FLGA_CODE				BIT_NC_ECC_FLGA_MASK
#define BIT_NC_ECC_CORRECT_CNT				(BIT7|BIT6|BIT5|BIT4|BIT3|BIT2)
#define BIT_NC_ECC_CNT_SHIFT				2
#define BIT_NC_ECC_SEL_LOC_MASK				(BIT12|BIT11|BIT10|BIT9|BIT8)
#define BIT_NC_ECC_SEL_LOC_SHIFT			8


#define REG50_ECC_CTRL_INIT_VALUE    (BIT_NC_SHARE_PAD_EN)
#define REG30_TEST_MODE_INIT_VALUE   0x5800

//------------------------------------------------------------------
/* AUX Reg Address */
#define AUXADR_CMDREG8    0x08
#define AUXADR_CMDREG9    0x09
#define AUXADR_CMDREGA    0x0A
#define AUXADR_ADRSET     0x0B
#define AUXADR_RPTCNT     0x18 // Pepeat Count
#define AUXADR_RAN_CNT    0x19
#define AUXADR_RAN_POS    0x1A // offset
#define AUXADR_ST_CHECK   0x1B
#define AUXADR_IDLE_CNT   0x1C
#define AUXADR_INSTQUE    0x20

/* OP Codes: Command */
#define CMD_0x00          0x00
#define CMD_0x30          0x01
#define CMD_0x35          0x02
#define CMD_0x90          0x03
#define CMD_0xFF          0x04
#define CMD_0x80          0x05
#define CMD_0x10          0x06
#define CMD_0x15          0x07
#define CMD_0x85          0x08
#define CMD_0x60          0x09
#define CMD_0xD0          0x0A
#define CMD_0x05          0x0B
#define CMD_0xE0          0x0C
#define CMD_0x70          0x0D
#define CMD_0x50          0x0E
#define CMD_0x01          0x0F
#define CMD_REG8          0x10
#define CMD_REG9          0x11
#define CMD_REGA          0x12

/* OP Code: Address */
#define OP_ADR_CYCLE_00   (0x4 << 4)
#define OP_ADR_CYCLE_01   (0x5 << 4)
#define OP_ADR_CYCLE_10   (0x6 << 4)
#define OP_ADR_CYCLE_11   (0x7 << 4)
#define OP_ADR_TYPE_COL   (0x0 << 2)
#define OP_ADR_TYPE_ROW   (0x1 << 2)
#define OP_ADR_TYPE_FUL   (0x2 << 2)
#define OP_ADR_TYPE_ONE   (0x3 << 2)
#define OP_ADR_SET_0      (0x0 << 0)
#define OP_ADR_SET_1      (0x1 << 0)
#define OP_ADR_SET_2      (0x2 << 0)
#define OP_ADR_SET_3      (0x3 << 0)

#define ADR_C2TRS0        (OP_ADR_CYCLE_00|OP_ADR_TYPE_ROW|OP_ADR_SET_0)
#define ADR_C2T1S0        (OP_ADR_CYCLE_00|OP_ADR_TYPE_ONE|OP_ADR_SET_0)
#define ADR_C3TRS0        (OP_ADR_CYCLE_01|OP_ADR_TYPE_ROW|OP_ADR_SET_0)
#define ADR_C3TFS0        (OP_ADR_CYCLE_00|OP_ADR_TYPE_FUL|OP_ADR_SET_0)
#define ADR_C4TFS0        (OP_ADR_CYCLE_01|OP_ADR_TYPE_FUL|OP_ADR_SET_0)
#define ADR_C4TFS1        (OP_ADR_CYCLE_01|OP_ADR_TYPE_FUL|OP_ADR_SET_1)
#define ADR_C5TFS0        (OP_ADR_CYCLE_10|OP_ADR_TYPE_FUL|OP_ADR_SET_0)
#define ADR_C6TFS0        (OP_ADR_CYCLE_11|OP_ADR_TYPE_FUL|OP_ADR_SET_0)

/* OP Code: Action */
#define ACT_WAITRB        0x80
#define ACT_CHKSTATUS     0x81
#define ACT_WAIT_IDLE     0x82
#define ACT_WAIT_MMA      0x83
#define ACT_BREAK         0x88
#define ACT_SER_DOUT      0x90 /* for column addr == 0 */
#define ACT_RAN_DOUT      0x91 /* for column addr != 0 */
//#define ACT_WR_REDU     0x92
//#define ACT_LUT_DWLOAD  0x93
//#define ACT_LUT_DWLOAD1 0x94
#define ACT_SER_DIN       0x98 /* for column addr == 0 */
#define ACT_RAN_DIN       0x99 /* for column addr != 0 */
//#define ACT_RD_REDU     0x9A
//#define ACT_LUT_UPLOAD  0x9B
#define ACT_PAGECOPY_US   0xA0
#define ACT_PAGECOPY_DS   0xA1
#define ACT_REPEAT        0xB0


typedef struct _TEST_ALIGN_PACK {

	MS_U8	u8_0;
	MS_U16	u16_0;
	MS_U32	u32_0, u32_1;

} UNFD_PACK TEST_ALIGN_PACK_t;
//extern MS_U32 drvNAND_CheckAlignPack(MS_U8 u8_AlignByteCnt);

//===========================================================
// driver parameters
//===========================================================
#define NAND_ID_BYTE_CNT     15

typedef struct NAND_DRIVER_PLATFORM_DEPENDENT
{
	MS_U8 		*pu8_PageSpareBuf;
	MS_U8		*pu8_PageDataBuf;
	MS_U32		u32_DMAAddrOffset;
	MS_U32		u32_RAMBufferOffset;
	MS_U32		u32_RAMBufferLen;
}NAND_DRIVER_PLAT_CTX, *P_NAND_DRIVER_PLAT;

//===========================================================
// Partition Info parameters
//===========================================================
#define UNFD_LOGI_PART		0x8000 // bit-or if the partition needs Wear-Leveling
#define UNFD_HIDDEN_PART	0x4000 // bit-or if this partition is hidden, normally it is set for the LOGI PARTs.
typedef struct _PARTITION_RECORD {

    MS_U16 u16_StartBlk;     // the start block index, reserved for UNFD internal use.
    MS_U16 u16_BlkCnt;       // project team defined
    MS_U16 u16_PartType;     // project team defined, e.g. UNFD_PART_XXX_0
    MS_U16 u16_BackupBlkCnt; // reserved good blocks count for backup, UNFD internal use.
                          // e.g. u16BackupBlkCnt  = u16BlkCnt * 0.03 + 2
} UNFD_PACK PARTITION_RECORD_t, *P_PARTITION_RECORD_t;



typedef struct _PARTITION_INFO {

	MS_U32	u32_ChkSum;
	MS_U32	u32_BLSectorCnt;
	MS_U32	u32_OSSectorCnt;
	MS_U16	u16_PartCnt;
	MS_U16	u16_UnitByteCnt;
	//ttt for 1 PARTITION_RECORD_t records[];
	PARTITION_RECORD_t records[4];  //ttt for 1

} UNFD_PACK PARTITION_INFO_t;



typedef struct _NAND_INFO {

	MS_U8	au8_Tag[16];
	MS_U8	u8_IDByteCnt;
	MS_U8	au8_ID[NAND_ID_BYTE_CNT];
	MS_U32	u32_ChkSum;
	MS_U16	u16_SpareByteCnt;
	MS_U16	u16_PageByteCnt;
	MS_U16	u16_BlkPageCnt;
	MS_U16	u16_BlkCnt;
	MS_U32	u32_Config;
	MS_U16	u16_ECCType;

} UNFD_PACK NAND_INFO_t;



typedef struct _BlK_INFO {

	MS_U8	u8_BadBlkMark;
	MS_U8	u8_PartType;
	MS_U16	u16_BlkAddr;
	MS_U8	au8_Misc[2];
	MS_U8	au8_ECC[10];

} UNFD_PACK BLK_INFO_t;



//===========================================================
// Logical Disk Info
//===========================================================
typedef struct _DISK_INFO {

    MS_U32 u32_StartSector;
    MS_U32 u32_SectorCnt;
} DISK_INFO_t, *P_DISK_INFO_t;

#define UNFD_MAX_DISK_NUM    16 // [CAUTION]: max logical partition count



typedef struct _NAND_DRIVER
{
    MS_U8   u8test1;


	MS_U8  au8_ID[NAND_ID_BYTE_CNT];
	MS_U8  u8_IDByteCnt;

	MS_U8 u8_CISBlk;
	//-----------------------------
	// HAL parameters
	//-----------------------------
    #if 1//tNC_SEL_FCIE3
	volatile MS_U16 u16_Reg1B_SdioCtrl;
	volatile MS_U16 u16_Reg40_Signal;
	volatile MS_U16 u16_Reg48_Spare;
	volatile MS_U16 u16_Reg49_SpareSize;
	volatile MS_U16 u16_Reg50_EccCtrl;
	#elif 0

	#endif
	//-----------------------------
	// NAND Info (listed by their init_sequence)
	//-----------------------------
	MS_U8 u8_WordMode;
	MS_U8 u8_OpCode_RW_AdrCycle;
	MS_U8 u8_OpCode_Erase_AdrCycle;

    MS_U16 u16_BlkCnt;
	MS_U16 u16_BlkPageCnt;
	MS_U16 u16_PageByteCnt;
    MS_U16 u16_SpareByteCnt;
	MS_U16 u16_ECCType;
	MS_U32 u32_Config;

	MS_U16 u16_BlkSectorCnt;
	MS_U16 u16_PageSectorCnt;
	MS_U16 u16_SectorByteCnt;
	MS_U16 u16_SectorSpareByteCnt;
	MS_U16 u16_ECCCodeByteCnt;

    MS_U8 u8_BlkPageCntBits;
    MS_U8 u8_BlkSectorCntBits;
	MS_U8 u8_PageByteCntBits;
	MS_U8 u8_SpareByteCntBits;
	MS_U8 u8_PageSectorCntBits;
	MS_U8 u8_SectorByteCntBits;
	MS_U8 u8_SectorSpareByteCntBits;

	MS_U16 u16_BlkPageCntMask;
	MS_U16 u16_BlkSectorCntMask;
	MS_U16 u16_PageByteCntMask;
    MS_U16 u16_SpareByteCntMask;
	MS_U16 u16_PageSectorCntMask;
	MS_U16 u16_SectorByteCntMask;

	NAND_DRIVER_PLAT_CTX PlatCtx_t;

	//-----------------------------
	// Partition Info
	//-----------------------------
	PARTITION_INFO_t *pPartInfo;

    //-----------------------------
	// UNFD parameters
	//-----------------------------
	DISK_INFO_t	DiskInfo_t[UNFD_MAX_DISK_NUM];
	MS_U16 u16_FirstPBA_LogiArea;
	MS_U16 u16_RootBlkPhyIdx;
	MS_U16 u16_LogiDataBlkCnt, u16_LogiFreeBlkCnt, u16_BadBlkCnt;


    // for first zone (the first 1000 logi blocks)
    MS_U16 u16_FirstPBA_Zone0; // = u16_FirstPBA_LogiArea + UNFD_ROOT_BLK_CNT
	MS_U16 au16_L2P_0[1024];

	#if UNFD_FTL_FMAP
    MS_U16 au16_FBIdx_0[UNFD_FREE_BLK_CNT_PER_1K+FMAP_MAX_LBA_CNT*FMAP_SPACE_FACTOR];
	#else
	MS_U16 au16_FBIdx_0[UNFD_FREE_BLK_CNT_PER_1K]; // free blk must be physical index
	#endif
	MS_U8 u8_FBHead_0, u8_FBTail_0, u8_FBCnt_0, u8_FBMaxCnt_0;
	MS_U16 u16_Zone0LBACnt;

	// for the folowing zones
	MS_U16 u16_FirstPBA_Zone1;
	MS_U16 au16_ActiveZoneIdx[MAX_LUT_CNT-1];
	MS_U16 au16_L2P_1[(MAX_LUT_CNT-1)*1024];
	MS_U16 au16_FBIdx_1[(MAX_LUT_CNT-1)*UNFD_FREE_BLK_CNT_PER_1K]; // free blk must be physical index
	MS_U8 u8_FBHead_1, u8_FBTail_1, u8_FBCnt_1;
	MS_U8 u8_Zone1SubZoneCnt;

	#if UNFD_FTL_WBQ
	WBQ_t aWBQ_t[MAX_WBQ_CNT];
	MS_U16   u16_PPB, u16_PPB_Page;
	#endif

	#if UNFD_FTL_FMAP
    MS_U16 u16_FM_FirstLBA, u16_FM_LBACnt, u16_FM_PBACnt;
	MS_U8 u8_FM_PBAPageCnt;
	// R: use au16_FM_BL2P + au8_FM_PL2P
	// W: use au16_FM_BL2P + au8_FM_FreePagePos,
	//    update au16_FM_BL2P, au8_FM_PL2P, au8_FM_FreePagePos
	MS_U16 au16_FM_BL2P[FMAP_MAX_LBA_CNT][FMAP_SPACE_FACTOR]; // value: PBA
	MS_U8 au8_FM_FreePagePos[FMAP_MAX_LBA_CNT][FMAP_SPACE_FACTOR]; // value: 0 ~ BlkPageCnt
	MS_U8 au8_FM_PL2P[FMAP_MAX_LBA_CNT][FMAP_MAX_BLKPAGE_CNT]; // value: 0 ~ BlkPageCnt
	#endif

	#if UNFD_FTL_RD || NAND_BLD_LD_OS
    MS_U16 u16_RD_SrcPBA; // source blk
	MS_U16 u16_RDB, u16_RDB_Page; // store RD info for physical area

		#if BLD_LD_OS_RD
    MS_U16 u16_RD_FreeBlk;
		#endif

	    #if UNFD_FTL_RD_TEST
	MS_U16 u16_RD_TestCnt;
	    #endif
	#endif

	#if UNFD_FTL_WL
	MS_U16 au16_WL_ECntPBA[MAX_LUT_CNT];
	MS_U16 au16_WL_ECntPBA_Page[MAX_LUT_CNT];
	MS_U32 au32_ZoneTotalECnt[MAX_LUT_CNT];

	    #if UNFD_FTL_WL_TEST
    MS_U16 u16_WL_TestCnt;
		#endif
	#endif

    MS_U8   u8_Background_FlagMarco;
	MS_U8   u8_TimerActive_FlagMarco;

	#if BIT_MAP_PHY
	MS_U8 pImageBlockBitMap[32];
	#endif

	//-----------------------------
	#if 0   //IF_IP_VERIFY
    MS_U8 u8_IfECCTesting;
	#endif

	volatile MS_U16 u16_Test0, u16_Test1;

} NAND_DRIVER, *P_NAND_DRIVER;


#define WRITEBACK_FLAG          BIT0
#define WEARLEVELING_FLAG       BIT1
#define WEARLEVELING1_FLAG      BIT2
#define FORCEWEARLEVELING_FLAG  BIT3
#define FORCEWEARLEVELING1_FLAG  BIT4





#ifdef __cplusplus
}
#endif


#endif // _DRV_NAND_H_

