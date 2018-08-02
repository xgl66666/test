
#include "eMMC_kaiser_uboot.h"
#include "eMMC_err_codes.h"
extern U8 gau8_eMMC_SectorBuf[];
extern U8 gau8_eMMC_PartInfoBuf[];

//===========================================================
// macro for Spec.
//===========================================================
#define ADDRESSING_MODE_BYTE      1 // 1 byte
#define ADDRESSING_MODE_SECTOR    2 // 512 bytes
#define ADDRESSING_MODE_4KB       3 // 4KB bytes

#define eMMC_SPEED_OLD            0
#define eMMC_SPEED_HIGH           1
#define eMMC_SPEED_HS200          2

#define eMMC_DRIVING_TYPE0	0
#define eMMC_DRIVING_TYPE1	BIT4
#define eMMC_DRIVING_TYPE2	BIT5
#define eMMC_DRIVING_TYPE3	(BIT5|BIT4)

#define eMMC_FLAG_TRIM            BIT0
#define eMMC_FLAG_HPI_CMD12       BIT1
#define eMMC_FLAG_HPI_CMD13       BIT2

#define eMMC_PwrOffNotif_OFF      0
#define eMMC_PwrOffNotif_ON       1
#define eMMC_PwrOffNotif_SHORT    2
#define eMMC_PwrOffNotif_LONG     3
///////////////////////////////////////////////
#define eMMC_FLAG_TRIM            BIT0
#define eMMC_FLAG_HPI_CMD12       BIT1
#define eMMC_FLAG_HPI_CMD13       BIT2

//===========================================================
// driver flag (u32_DrvFlag)
//===========================================================
#define DRV_FLAG_INIT_DONE       BIT0 // include eMMC identify done

#define DRV_FLAG_GET_PART_INFO   BIT1
#define DRV_FLAG_RSP_WAIT_D0H    BIT2 // currently only R1b
#define DRV_FLAG_DDR_MODE        BIT3
#define DRV_FLAG_DDR_TUNING      BIT4

#define DRV_FLAG_SPEED_MASK      (BIT5|BIT6)
#define DRV_FLAG_SPEED_HIGH      BIT5
#define DRV_FLAG_SPEED_HS200     BIT6

#define DRV_FLAG_TESTING         BIT7

#define DRV_FLAG_PwrOffNotif_MASK   (BIT8|BIT9)
#define DRV_FLAG_PwrOffNotif_OFF    0
#define DRV_FLAG_PwrOffNotif_ON     BIT8
#define DRV_FLAG_PwrOffNotif_SHORT  BIT9
#define DRV_FLAG_PwrOffNotif_LONG   (BIT8|BIT9)

#define DRV_FLAG_RSPFROMRAM_SAVE BIT10
#define DRV_FLAG_ERROR_RETRY     BIT11


typedef struct _eMMC_DRIVER
{
	// U32 u32_ChkSum; // [8th ~ last-512] bytes
	// U8 au8_Sig[4];  // 'e','M','M','C'

	//----------------------------------------
    //FCIE
    //----------------------------------------
	U16 u16_RCA;
	U32 u32_DrvFlag;//, u32_LastErrCode;
	U8  au8_Rsp[eMMC_MAX_RSP_BYTE_CNT];
	U8  au8_CSD[eMMC_MAX_RSP_BYTE_CNT];
	U8  au8_CID[eMMC_MAX_RSP_BYTE_CNT];
	U16 u16_Reg10_Mode;
	U32 u32_ClkKHz;
	U16 u16_ClkRegVal, u8_PadType;
	//eMMC_FCIE_DDRT_TABLE_t DDRTable;

    //----------------------------------------
    //eMMC
    //----------------------------------------
	//CSD
	U8  u8_SPEC_VERS;
	U8  u8_R_BL_LEN, u8_W_BL_LEN; // supported max blk len
	U16 u16_C_SIZE;
	U8  u8_TAAC, u8_NSAC, u8_Tran_Speed;
	 U8  u8_C_SIZE_MULT;
	 U8  u8_ERASE_GRP_SIZE, u8_ERASE_GRP_MULT;
	U8  u8_R2W_FACTOR;

	U8  u8_IfSectorMode;
	 U32 u32_eMMCFlag;
	 U32 u32_EraseUnitSize;

	//ExtCSD
	 U32 u32_SEC_COUNT;
	 U32 u32_BOOT_SEC_COUNT;
	 U8  u8_BUS_WIDTH, u8_ErasedMemContent;
	 U16 u16_ReliableWBlkCnt;
	 U8  u8_ECSD185_HsTiming, u8_ECSD192_Ver, u8_ECSD196_DevType, u8_ECSD197_DriverStrength;
	 U8  u8_ECSD248_CMD6TO, u8_ECSD247_PwrOffLongTO, u8_ECSD34_PwrOffCtrl;

// #ifdef CONFIG_TSD
	//----------------------------------------
	//tSD
	//----------------------------------------
	// U8 u8_EraseTimeout; // unit: second
	// U32 u32_EraseSize; // unit: sector(512B)
// #endif

    //----------------------------------------
    //CIS
    //----------------------------------------
	//nni
	// U8 u8_IDByteCnt, au8_ID[eMMC_ID_BYTE_CNT];
	// U8 au8_Vendor[16], au8_PartNumber[16];

	//pni
    // U32 au32_Pad[2]; // don't move

	// U32 u32_PartDevNodeStartSector;
	// U16 u16_PartDevNodeSectorCnt;
	// U32 u32_FATSectorCnt;

    //----------------------
	// #if defined(eMMC_RSP_FROM_RAM) && eMMC_RSP_FROM_RAM
	//rsp from ram
	// U8 au8_AllRsp[eMMC_SECTOR_512BYTE]; // last 4 bytes are CRC
	// #endif

	//----------------------
	// #if defined(eMMC_BURST_LEN_AUTOCFG) && eMMC_BURST_LEN_AUTOCFG
	//eMMC_FCIE_WLen_TABLE_t  BurstWriteLen_t;
	// #endif

	//----------------------
	// #if defined(eMMC_PROFILE_WR) && eMMC_PROFILE_WR
	// U32 u32_CNT_CMD17, u32_CNT_CMD24, u32_CNT_CMD18, u32_CNT_CMD25;
	// U64 u64_CNT_TotalRBlk, u64_CNT_TotalWBlk;

	// U32 u32_CNT_MinRBlk, u32_CNT_MinWBlk, u32_CNT_MaxRBlk, u32_CNT_MaxWBlk;
	// U32 u32_RBlk_tmp, u32_WBlk_tmp;
	// U32 au32_CNT_MinRBlk[0x200], au32_CNT_MinWBlk[0x200]; // for blk count < 0x200, how many times

	// U32 u32_Addr_RLast, u32_Addr_WLast;
	// U32 u32_Addr_RHitCnt, u32_Addr_WHitCnt;

	// U32 u32_temp_count;
	// #endif


} eMMC_DRIVER, *P_eMMC_DRIVER;

extern  eMMC_DRIVER g_eMMCDrv;

#if defined(eMMC_UPDATE_FIRMWARE) && (eMMC_UPDATE_FIRMWARE)
#define TIME_WAIT_DAT0_HIGH            (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_FCIE_RESET           (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_FCIE_RST_TOGGLE_CNT  (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_FIFOCLK_RDY          (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_CMDRSP_END           (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_1_BLK_END            (HW_TIMER_DELAY_1s*5)
#define TIME_WAIT_n_BLK_END            (HW_TIMER_DELAY_1s*10) // safe for 512 blocks
#else
#define TIME_WAIT_DAT0_HIGH            (HW_TIMER_DELAY_1s*60*10) //10*60 s
#define TIME_WAIT_ERASE_DAT0_HIGH      (HW_TIMER_DELAY_1s*60*10) //10*60 s
#define TIME_WAIT_FCIE_RESET           HW_TIMER_DELAY_500ms
#define TIME_WAIT_FCIE_RST_TOGGLE_CNT  HW_TIMER_DELAY_1us
#define TIME_WAIT_FIFOCLK_RDY          HW_TIMER_DELAY_500ms
#define TIME_WAIT_CMDRSP_END           HW_TIMER_DELAY_1s
#define TIME_WAIT_1_BLK_END            (HW_TIMER_DELAY_1s*10)
#define TIME_WAIT_n_BLK_END            (HW_TIMER_DELAY_1s*2) // safe for 512 blocks
#endif

#if 0
#define eMMC_printf    printf                       // <-- [FIXME]
#define eMMC_debug(dbg_lv, tag, str, ...)	do{PUTS(str);}while(0)	
#else /* eMMC_DEBUG_MSG */
#define eMMC_printf(...)
#define eMMC_debug(enable, tag, str, ...)	do{}while(0)
#endif /* eMMC_DEBUG_MSG */

typedef eMMC_PACK0 struct _eMMC_TEST_ALIGN_PACK {

	U8	u8_0;
	U16	u16_0;
	U32	u32_0, u32_1;

} eMMC_PACK1 eMMC_TEST_ALIGN_PACK_t;


#define EMMC_ENABLE_PRINT 0
#if EMMC_ENABLE_PRINT
#define EMMC_PUTC PUTC
#define EMMC_PUTS LDR_PUTS
#define EMMC_PUTDW LDR_PUTDW
#else
#define EMMC_PUTC 
#define EMMC_PUTS 
#define EMMC_PUTDW 
#endif


extern U32 eMMC_hw_timer_delay(U32 u32us);
extern U32 eMMC_hw_timer_sleep(U32 u32ms);
extern U32 eMMC_hw_timer_start(void);
extern U32 eMMC_hw_timer_tick(void);
extern void eMMC_set_WatchDog(U8 u8_IfEnable);
extern void eMMC_reset_WatchDog(void);
extern void eMMC_flush_data_cache_buffer(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern void eMMC_Invalidate_data_cache_buffer(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern void eMMC_flush_miu_pipe(void);
extern U32 eMMC_pads_switch(U32 u32_FCIE_IF_Type);
extern U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern U32 eMMC_clock_setting(U16 u16_ClkParam);
extern U32 eMMC_clock_gating(void);
extern U32 eMMC_PlatformResetPre(void);
extern U32 eMMC_PlatformResetPost(void);
extern U32 eMMC_PlatformInit(void);
extern U32 eMMC_FCIE_Init(void);

