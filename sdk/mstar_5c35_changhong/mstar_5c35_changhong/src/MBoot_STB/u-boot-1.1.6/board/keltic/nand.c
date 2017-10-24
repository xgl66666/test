#include <common.h>

#if (CONFIG_COMMANDS & CFG_CMD_NAND)

#include <linux/mtd/compat.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>

#include "../../drivers/mstar/unfd/inc/common/drvNAND.h"

U32 u32CurRow = 0;
U32 u32CurCol = 0;
U32 u32CIFDIdx = 0;
U16 u16ByteIdxofPage = 0;
U32 u32WriteLen = 0;

U8 u8MainBuf[8192] UNFD_ALIGN1;
U8 u8SpareBuf[512] UNFD_ALIGN1;

#ifndef CONFIG_NANDDRV_DEBUG
#define CONFIG_NANDDRV_DEBUG 0 //default disable debug msg.
#endif

#if (CONFIG_NANDDRV_DEBUG)
#define NAND_DEBUG(fmt, args...) printf(fmt, ##args)
#else
#define NAND_DEBUG(fmt, args...)
#endif

/* These really don't belong here, as they are specific to the NAND Model */
static uint8_t scan_ff_pattern[] = {0xff};

/* struct nand_bbt_descr - bad block table descriptor */
static struct nand_bbt_descr _titania_nand_bbt_descr = {
  .options = NAND_BBT_2BIT | NAND_BBT_LASTBLOCK | NAND_BBT_VERSION | NAND_BBT_CREATE | NAND_BBT_WRITE,
  .offs = 5,
  .len = 1,
  .pattern = scan_ff_pattern
};

static struct nand_oobinfo _titania_nand_oob_16 = {
  .useecc = MTD_NANDECC_AUTOPLACE,
  .eccbytes = 10,
  .eccpos = {6,7,8,9,10,11,12,13,14,15},
  .oobfree = { {0, 5}}
};

static struct nand_oobinfo _titania_nand_oob_64 = {
  .useecc = MTD_NANDECC_AUTOPLACE,
  .eccbytes = 40,
  .eccpos =
  { 0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
    0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
    0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
    0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
  },
  .oobfree =
  {
    { 1,5},
    { 0x10,6},
    { 0x20,6},
    { 0x30,6}
  }
};


static struct nand_oobinfo _titania_nand_oob_128 = {
  .useecc = MTD_NANDECC_AUTOPLACE,
    .eccbytes   = 80,
    .eccpos     =
    {
      0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
      0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
      0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
      0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
      0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
      0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
      0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
      0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F
    },
    .oobfree    =
    {
      { 1,    5   },
        { 0x10,  6  },
        { 0x20,  6  },
        { 0x30,  6  },
    { 0x40,  6  },
        { 0x50,  6  },
        { 0x60,  6  },
        { 0x70,  6  }
    }
};


static void _titania_nand_hwcontrol(struct mtd_info *mtdinfo, int cmd)
{
  NAND_DEBUG("NANDDrv_HWCONTROL()\n");

  switch(cmd)
  {
    case NAND_CTL_SETNCE:
    NAND_DEBUG("NAND_CTL_SETNCE\n");
    // Let FCIE3 control the CE
    break;
    case NAND_CTL_CLRNCE:
    NAND_DEBUG("NAND_CTL_CLRNCE\n");
    // Let FCIE3 control the CE

    break;
    case NAND_CTL_SETCLE:
    NAND_DEBUG("NAND_CTL_SETCLE\n");
    // We have no way to control CLE in NFIE/FCIE2/FCIE3
    break;
    case NAND_CTL_CLRCLE:
    NAND_DEBUG("NAND_CTL_CLRCLE\n");
    // We have no way to control CLE in NFIE/FCIE2/FCIE3
    break;

    case NAND_CTL_SETALE:
    NAND_DEBUG("NAND_CTL_SETALE\n");
    // We have no way to control CLE in NFIE/FCIE2/FCIE3
    break;
    case NAND_CTL_CLRALE:
    NAND_DEBUG("NAND_CTL_CLRALE\n");
    // We have no way to control CLE in NFIE/FCIE2/FCIE3
    break;

    default:
    NAND_DEBUG("UNKNOWN CMD\n");
    break;
  }

  return;
}

static int _titania_nand_device_ready(struct mtd_info *mtdinfo)
{
  NAND_DEBUG("NANDDrv_DEVICE_READY()\n");

#if 0 // We have wait ready in each HAL functions
  drvNAND_WaitReady(1000);

#endif

  return 1;
}

static void _titania_nand_write_buf(struct mtd_info *mtd, const u_char *buf, int len)
{

  NAND_DEBUG("NANDDrv_WRITE_BUF() 0x%X\r\n",len);


    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U8 *pu8MainBuf;
    U8 *pu8SpareBuf;

    if( len >= pNandDrv->u16_PageByteCnt ) // whole page or Main area only
    {
      if( len > pNandDrv->u16_PageByteCnt ) // whole page
      {
        if( ((U32)buf) >= 0xC0000000 ) // For MIU1
        {
          memcpy(u8MainBuf, buf, pNandDrv->u16_PageByteCnt);
          memcpy(u8SpareBuf, buf + (pNandDrv->u16_PageByteCnt), pNandDrv->u16_SpareByteCnt);
          pu8MainBuf = u8MainBuf;
          pu8SpareBuf = u8SpareBuf;
        }
        else // For MIU0
        {
          pu8MainBuf = (U8*)buf;
          pu8SpareBuf = (U8*)buf + (pNandDrv->u16_PageByteCnt);
        }

        NC_WritePages(u32CurRow, pu8MainBuf, pu8SpareBuf, 1);
      }
      else // main area only
      {
        memcpy(u8MainBuf, buf, len);
        u32WriteLen += len;
        u16ByteIdxofPage += len;
      }
    }
    else
    {
      if((u32WriteLen==0) && (u16ByteIdxofPage>=pNandDrv->u16_PageByteCnt)) // mtd skip prepare main area, default all oxff
      {
        memset(u8MainBuf, 0xFF, pNandDrv->u16_PageByteCnt);
        memset(u8SpareBuf, 0xFF, pNandDrv->u16_SpareByteCnt);
      }

      memcpy(u8SpareBuf, buf, len);
      u32WriteLen += len;
      u16ByteIdxofPage += len;

      if( u32WriteLen == (pNandDrv->u16_PageByteCnt + pNandDrv->u16_SpareByteCnt) )
      NC_WritePages(u32CurRow, u8MainBuf, u8SpareBuf, 1);
    }


  return;
}

static void _titania_nand_read_buf(struct mtd_info *mtd, u_char* const buf, int len)
{


  NAND_DEBUG("NANDDrv_READ_BUF()0x%X\n",len);


    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U8 *pu8MainBuf;
    U8 *pu8SpareBuf;

    u16ByteIdxofPage = len;

    if( len >= pNandDrv->u16_PageByteCnt )
    {
      if( len > pNandDrv->u16_PageByteCnt )
      {
        if( ((U32)buf) >= 0xC0000000 ) // For MIU1
        {
          pu8MainBuf = u8MainBuf;
          pu8SpareBuf = u8SpareBuf;
        }
        else // For MIU0
        {
          pu8MainBuf = buf;
          pu8SpareBuf = (U8*)buf + (pNandDrv->u16_PageByteCnt);
        }

        NC_ReadPages(u32CurRow, pu8MainBuf, pu8SpareBuf, 1);

        if( buf != pu8MainBuf ) // If MIU1, copy data from temp buffer
        {
          memcpy(buf, u8MainBuf, pNandDrv->u16_PageByteCnt);
          memcpy((U8*)buf + (pNandDrv->u16_PageByteCnt), u8SpareBuf, pNandDrv->u16_SpareByteCnt);
        }
      }
      else
      {
        if( ((U32)buf) >= 0xC0000000 ) // For MIU1
        {
          pu8MainBuf = u8MainBuf;
        }
        else // For MIU0
        {
          pu8MainBuf = buf;
        }
        pu8SpareBuf = u8SpareBuf; // Preserve spare data in temp buffer for next read_buf()

        NC_ReadPages(u32CurRow, pu8MainBuf, pu8SpareBuf, 1);

        if( buf != pu8MainBuf ) // If MIU1, copy data from temp buffer
        memcpy(buf, u8MainBuf, pNandDrv->u16_PageByteCnt);
      }
    }
    else
    {
      memcpy(buf, u8SpareBuf, len);
    }


  return;
}

static u16 _titania_nand_read_word(struct mtd_info *mtd)
{
  NAND_DEBUG("NANDDrv_READ_WORD()\n");
  return 0;
}

static u_char _titania_nand_read_byte(struct mtd_info *mtd)
{
  U8 u8Ret = 0;



  NC_GetCIFD(&u8Ret, u32CIFDIdx, 1);
  u32CIFDIdx++;

  NAND_DEBUG("NANDDrv_READ_BYTE()=0x%X\n",u8Ret);

  return (u8Ret);
}

/**
 * nand_wait - [DEFAULT]  wait until the command is done
 * @mtd:  MTD device structure
 * @this: NAND chip structure
 * @state:  state to select the max. timeout value
 *
 * Wait for command done. This applies to erase and program only
 * Erase can take up to 400ms and program up to 20ms according to
 * general NAND and SmartMedia specs
 *
 */
static int _titania_nand_wait(struct mtd_info *mtd, struct nand_chip *this, int state)
{
  NAND_DEBUG("NANDDrv_WAIT()\n");

  return REG(NC_ST_READ);
}

static void _titania_nand_cmdfunc(struct mtd_info *mtd, unsigned command, int column, int page_addr)
{
  NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

  switch (command) {
    case NAND_CMD_READ0:
    NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_READ0, page_addr: 0x%x, column: 0x%x.\n", page_addr, (column>>1));
    u32CurRow = page_addr;
    u32CurCol = column;
    break;

    case NAND_CMD_READ1:
    NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_READ1.\n");

    NAND_DEBUG("\033[31mUNFD not support READ1(CMD 0x01) now!!!\033[m\n");
    break;

    case NAND_CMD_READOOB:
    NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_READOOB.\n");

    NAND_DEBUG("\033[34mReadOOB (CMD 0x50)\033[m\n");
    u32CIFDIdx = 0;
    u16ByteIdxofPage = 0;
    NC_Read_RandomIn(page_addr, column, u8SpareBuf, pNandDrv->u16_SpareByteCnt);

    break;

    case NAND_CMD_READID:
    NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_READID.\n");

    u32CIFDIdx = 0;
    NC_ReadID();
    break;

    case NAND_CMD_PAGEPROG:
    /* sent as a multicommand in NAND_CMD_SEQIN */
    NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_PAGEPROG.\n");

    // We have done page program in write_buf()
    break;

    case NAND_CMD_ERASE1:
    NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_ERASE1,  page_addr: 0x%x, column: 0x%x.\n", page_addr, column);
    NC_EraseBlk(page_addr);

    break;

    case NAND_CMD_ERASE2:
    NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_ERASE2.\n");
    // We do all erase function in Erase1 command.

    break;

    case NAND_CMD_SEQIN:
    /* send PAGE_PROG command(0x1080) */
    NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_SEQIN/PAGE_PROG,  page_addr: 0x%x, column: 0x%x.\n", page_addr, column);

    u32CurRow = page_addr;
    u32CurCol = column;
    u16ByteIdxofPage = column;
    u32WriteLen = 0;
    break;

    case NAND_CMD_STATUS:
    NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_STATUS.\n");

    u32CIFDIdx = 0;
    NC_ReadStatus();

    break;

    case NAND_CMD_RESET:
    NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_RESET.\n");

    NC_ResetNandFlash();

    break;

    case NAND_CMD_STATUS_MULTI:
    NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_STATUS_MULTI.\n");

    u32CIFDIdx = 0;
    NC_ReadStatus();
    break;

    case NAND_CMD_READSTART:
    NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_READSTART.\n");

    // We have done this command in NC_ReadPages()
    break;

    case NAND_CMD_CACHEDPROG:
    NAND_DEBUG("_titania_nand_cmdfunc: NAND_CMD_CACHEDPROG.\n");

    NAND_DEBUG("\033[31mUNFD not support CACHEPROG (CMD 0x15) now!!!\033[m\n");

    break;

    default:
    NAND_DEBUG("_titania_nand_cmdfunc: error, unsupported command.\n");
    break;
  }

  return;
}

static void _titania_nand_enable_hwecc(struct mtd_info *mtd, int mode)
{
  NAND_DEBUG("enable_hwecc\r\n");
  // default enable
}

static int _titania_nand_calculate_ecc(struct mtd_info *mtd, const u_char *dat, u_char *ecc_code)
{
  NAND_DEBUG("calculate_ecc\r\n");

  // NFIE/FCIE2/FCIE3 will calculate it.

  return 0;
}

static int _titania_nand_correct_data(struct mtd_info *mtd, u_char *dat, u_char *read_ecc, u_char *calc_ecc)
{

  int s32ECCStatus = 0;
  NC_CheckECC(&s32ECCStatus);
  return s32ECCStatus;
}

void nand_unfd_init(void)
{
  NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

  REG_WRITE_UINT16(NC_PATH_CTL, 0x20);  //NOAH TBD - NAND ENABLE

  nand_pads_switch(1);

  printf("%s nand_pads_switch done\n",__FUNCTION__);

  nand_clock_setting(pNandDrv->u32_Clk);

  printf("%s nand_clock_setting done\n",__FUNCTION__);

  NC_ResetFCIE(); // Reset FCIE3

  printf("%s NC_ResetFCIE done\n",__FUNCTION__);


  // We need to config Reg 0x40 first because we we need to get nand ID first
  pNandDrv->u16_Reg40_Signal =
  (BIT_NC_WP_AUTO | BIT_NC_WP_H | BIT_NC_CE_AUTO | BIT_NC_CE_H) &
  ~(BIT_NC_CHK_RB_EDGEn | BIT_NC_CE_SEL_MASK);
  REG_WRITE_UINT16(NC_SIGNAL, pNandDrv->u16_Reg40_Signal);

  // clean the RIU mode if existed
  REG_WRITE_UINT16(NC_R2N_CTRL, 0);

#if defined(ENABLE_NAND_INTERRUPT_MODE) && ENABLE_NAND_INTERRUPT_MODE
  nand_enable_intr_mode();
#endif


  NC_ResetNandFlash(); // Reset NAND flash

  printf("%s NC_ResetNandFlash done\n",__FUNCTION__);

  NC_ReadID();

  printf("%s NC_ReadID done\n",__FUNCTION__);

  drvNAND_CHECK_FLASH_TYPE();

  printf("%s drvNAND_CHECK_FLASH_TYPE done\n",__FUNCTION__);

  NC_PlatformInit();

  printf("%s NC_PlatformInit done\n",__FUNCTION__);

  NC_ConfigContext();

  printf("%s NC_ConfigContext done\n",__FUNCTION__);

  // config NC
  pNandDrv->u16_Reg1B_SdioCtrl = (pNandDrv->u16_SectorByteCnt +
      (pNandDrv->u16_SpareByteCnt >> pNandDrv->u8_PageSectorCntBits)) &~ 0x1UL;

  if (NC_MAX_SECTOR_BYTE_CNT < pNandDrv->u16_Reg1B_SdioCtrl)
  {
    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR: invalid Sector Size: %Xh bytes!\r\n", pNandDrv->u16_Reg1B_SdioCtrl);
    nand_die();
  }
  pNandDrv->u16_Reg1B_SdioCtrl |= BIT_SDIO_BLK_MODE;

  pNandDrv->u16_Reg48_Spare = (pNandDrv->u16_SpareByteCnt >> pNandDrv->u8_PageSectorCntBits);
  if (NC_MAX_SECTOR_SPARE_BYTE_CNT < pNandDrv->u16_Reg48_Spare || (pNandDrv->u16_Reg48_Spare & 1))
  {
    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR: invalid Sector Spare Size: %Xh bytes!\r\n", pNandDrv->u16_Reg48_Spare);
    nand_die();
  }
#if IF_SPARE_AREA_DMA
  pNandDrv->u16_Reg48_Spare |= BIT_NC_SPARE_DEST_MIU;
#endif

  pNandDrv->u16_Reg49_SpareSize = pNandDrv->u16_SpareByteCnt;
  if (NC_MAX_TOTAL_SPARE_BYTE_CNT < pNandDrv->u16_Reg49_SpareSize || (pNandDrv->u16_Reg49_SpareSize & 1))
  {
    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR: invalid Total Spare Size: %Xh bytes!\r\n", pNandDrv->u16_Reg49_SpareSize);
    nand_die();
  }

  pNandDrv->u16_Reg50_EccCtrl = REG50_ECC_CTRL_INIT_VALUE;
  if (pNandDrv->u8_WordMode)
  pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_WORD_MODE;
  nand_debug(UNFD_DEBUG_LEVEL, 1 ,"UNFD, WordMode:%X\r\n", pNandDrv->u8_WordMode);

  switch (pNandDrv->u16_PageByteCnt)
  {
    case 0x0200:
    pNandDrv->u16_Reg50_EccCtrl &= ~BIT_NC_PAGE_SIZE_512Bn;
    pNandDrv->u16_Reg48_Spare |= BIT_NC_ONE_COL_ADDR;
    break;
    case 0x0800: pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_2KB; break;
    case 0x1000:
        printf("%s u16_PageByteCnt=0x1000 (4096)\n",__FUNCTION__);
        pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_4KB; break;
    case 0x2000: pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_8KB; break;
    case 0x4000: pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_16KB; break;
    case 0x8000: pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_PAGE_SIZE_32KB; break;
    default:
    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR: invalid Page Size: %Xh bytes!\r\n", pNandDrv->u16_PageByteCnt);
    nand_die();
break;
  }

  switch (pNandDrv->u16_ECCType)
  {
    case ECC_TYPE_RS:
    pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_RS;
    pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_RS;
    break;
    case ECC_TYPE_4BIT:
    printf("%s ECC_TYPE_4BIT\n",__FUNCTION__);
    pNandDrv->u16_Reg50_EccCtrl &= ~BIT_NC_ECC_TYPE_4b512Bn;
    pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_4BIT;
    break;
    case ECC_TYPE_8BIT:
    pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_8b512B;
    pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_8BIT;
    break;
    case ECC_TYPE_12BIT:
    pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_12b512B;
    pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_12BIT;
    break;
    case ECC_TYPE_24BIT1KB:
    pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_24b1KB;
    pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_24BIT1KB;
    break;
    case ECC_TYPE_32BIT1KB:
    pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECC_TYPE_32b1KB;
    pNandDrv->u16_ECCCodeByteCnt = ECC_CODE_BYTECNT_32BIT1KB;
    break;
    default:
    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR: invalid ECC Type: %Xh \r\n", pNandDrv->u16_ECCType);
    nand_die();
break;
  }
  pNandDrv->u16_Reg50_EccCtrl |= BIT_NC_ECCERR_NSTOP;

  NC_Config();

  printf("%s NC_Config done\n",__FUNCTION__);

}

/*
 * Board-specific NAND initialization.
 * - hwcontrol: hardwarespecific function for accesing control-lines
 * - dev_ready: hardwarespecific function for  accesing device ready/busy line
 * - eccmode: mode of ecc, see defines
 */
void board_nand_init(struct nand_chip *nand)
{
  NAND_DEBUG("NANDDrv_BOARD_NAND_INIT().\n");

  printf("MBOOT UNFD-----------------------------------------------------------0711 %s\n",__TIME__);

  NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

  nand_unfd_init();

  /* please refer to include/linux/nand.h for more info. */
  nand->dev_ready = _titania_nand_device_ready;
  nand->hwcontrol = _titania_nand_hwcontrol;

  if( pNandDrv->u16_PageByteCnt == 4096 )
  {
    printf(">>>pagesize = 4096\n");

    nand->eccmode = NAND_ECC_HW80_4096;
    nand->autooob = &_titania_nand_oob_128;
    _titania_nand_bbt_descr.offs = 0;

  }
  else if( pNandDrv->u16_PageByteCnt == 2048 )
  {
      printf(">>>pagesize = 2048\n");
    nand->eccmode = NAND_ECC_HW40_2048;
    nand->autooob = &_titania_nand_oob_64;
    _titania_nand_bbt_descr.offs = 0;

  }
  else
  {
    printf(">>>pagesize = 512\n");
    nand->eccmode = NAND_ECC_HW10_512;
    nand->autooob = &_titania_nand_oob_16; //using default oob layout.

  }

  nand->enable_hwecc = _titania_nand_enable_hwecc;
  nand->correct_data = _titania_nand_correct_data;
  nand->calculate_ecc = _titania_nand_calculate_ecc;

  if( pNandDrv->u16_PageByteCnt > 512 )
  nand->options = NAND_USE_FLASH_BBT | NAND_NO_AUTOINCR;
  else
  nand->options = NAND_USE_FLASH_BBT;

  nand->options = NAND_USE_FLASH_BBT;
  nand->waitfunc = _titania_nand_wait;
  nand->read_byte = _titania_nand_read_byte;
  nand->read_word = _titania_nand_read_word;
  nand->read_buf = _titania_nand_read_buf;
  nand->write_buf = _titania_nand_write_buf;
  nand->chip_delay = 20; //@FIXME: unite: us, please refer to nand_base.c 20us is default.
  nand->cmdfunc = _titania_nand_cmdfunc;
  nand->badblock_pattern = &_titania_nand_bbt_descr; //using default badblock pattern.



}


#if ENABLE_MODULE_BFN_WRITE_NAND

//=====================ted add
#define CHIPCFG_CMD_BASE       0xFE00
#define CHIPCFG_CMD_END        0xFFF0
#define CHIPCFG_CMD_WAIT       0xFF00
#define CHIPCFG_CMD_DBG        0xFFE0

unsigned char *nand_buffer;
static unsigned char nand_buffer2[16384] __attribute((aligned(64)));
static unsigned char nand_buffer_spare[16384] __attribute((aligned(64)));

#define MIUIDTAG    (0x4D495530)      // 'MIU0'
#define LDRIDTAG    (0x4C445230)      // 'LDR0'
#define BINIDTAG    (0x42494E30)      // 'BIN0'

static void BFN_WritePage_MIU(U32 u32Row, U8 *u8Buf, U8 *u8Spare, U16 u16Repeat, U16 autodis)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    if(autodis)
    {
        pNandDrv->u16_Reg48_Spare |= BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();
    }

    NC_WritePages( u32Row, u8Buf, u8Spare, u16Repeat);

    if(autodis)
    {
        pNandDrv->u16_Reg48_Spare &= ~BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
        NC_Config();
    }
}

static void BFN_ReadSector_RIU(U32 u32Row, U32 u32Col, U8 *u8Buf, U8 *u8SprBuf )
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    pNandDrv->u16_Reg48_Spare |= BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
    NC_Config();

    NC_ReadSector_RIUMode(u32Row, u32Col, u8Buf, u8SprBuf);

    pNandDrv->u16_Reg48_Spare &= ~BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE;
    NC_Config();
}




int drvNAND_ReadMIUParam(U32 u32MIUPos)
{
    int              retlen=0;
    while (1)
    {
        U16 reg = *(U16*)u32MIUPos;

        retlen += 4;

        if (reg == CHIPCFG_CMD_END)
            return retlen;

        u32MIUPos += 4;
    }
    return 0;
}


U32 drvNAND_WriteMIUParam(int MIUBLOCK, U32 MIUPOS)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    int ParamMIUSize;
    int ProgramPage=0;

    NC_EraseBlk( MIUBLOCK * pNandDrv->u16_BlkPageCnt );//use first block as test item

    //==============write MIU position==============
    // read MIU parameter

    ParamMIUSize = drvNAND_ReadMIUParam(MIUPOS);

    *((U32*)(nand_buffer2+0x00)) = MIUIDTAG;//0x4D495530
    *((U32*)(nand_buffer2+0x04)) = pNandDrv->u16_PageByteCnt;
    *((U32*)(nand_buffer2+0x10)) = ParamMIUSize>>9; // number of MIU page (in 512 bytes size)
    *((U32*)(nand_buffer2+0x14)) = pNandDrv->u16_BlkPageCnt;

    memset(nand_buffer_spare, -1, sizeof(nand_buffer_spare));

    printf("MIU parameter size %d\n",ParamMIUSize);
    BFN_WritePage_MIU(MIUBLOCK * pNandDrv->u16_BlkPageCnt + ProgramPage, (U8*)nand_buffer2,(U8*)nand_buffer_spare,1,1);
    ProgramPage++;
    printf("Write First Page, ParamMIUSize left %d program page number %d\n",ParamMIUSize,ProgramPage);

    while(ParamMIUSize > 0)
    {
        BFN_WritePage_MIU(MIUBLOCK * pNandDrv->u16_BlkPageCnt+ProgramPage,(U8*)nand_buffer+((ProgramPage-1)*(512)), (U8*)nand_buffer_spare,1,1);
        ParamMIUSize -= 512;
        ProgramPage ++;
        printf("Write next Page, ParamMIUSize left %d program page number %d\n",ParamMIUSize,ProgramPage);
    }

    return 1;
}


U32 drvNAND_WriteLoader(int LOADERBLOCK, U8* u8LDRPos)
{
    U32             Ldr2MIUPos = 0;
    U32             LdrSize = 0;
    U32             LdrEntryOffset = 0; // means the actually position offset from u8BINPos for bin file to store
    int i=0,j=LOADERBLOCK;

    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();


    Ldr2MIUPos = *((U32*)(u8LDRPos));
    LdrSize = *((U32*)(u8LDRPos+12));
    LdrEntryOffset = *((U32*)(u8LDRPos+4));
    Ldr2MIUPos += LdrEntryOffset;
    u8LDRPos += LdrEntryOffset;
    LdrSize -= LdrEntryOffset;
    *((U32*)(nand_buffer2+0x00)) = LDRIDTAG;
    *((U32*)(nand_buffer2+0x04)) = Ldr2MIUPos;// the position we are going to put to MIU and jump
    *((U32*)(nand_buffer2+0x10)) = LDRIDTAG;
    *((U32*)(nand_buffer2+0x14)) = LdrSize;
    //*((U32*)(nand_buffer_spare+16)) = APPEntryOffset;//start point of AP

    printf("[%x]MIU pos %x\r\nLoader file size %x\r\nLoader Entry offset %x\r\n",(U32)u8LDRPos,Ldr2MIUPos,LdrSize,LdrEntryOffset);

    memset(nand_buffer_spare, -1, sizeof(nand_buffer_spare));
    while(LdrSize)
    {
        if (i==0)
        {
            printf("<flow control>issue erase [block:%x] operation!\n",j);
            NC_EraseBlk(j * pNandDrv->u16_BlkPageCnt);
            BFN_WritePage_MIU((j * pNandDrv->u16_BlkPageCnt)+i,(U8*)nand_buffer2,(U8*)nand_buffer_spare,1,0);
            i++;
            continue;
        }

        BFN_WritePage_MIU((j * pNandDrv->u16_BlkPageCnt)+i,(U8*)u8LDRPos,(U8*)nand_buffer_spare,1,0);

        u8LDRPos += ((LdrSize < pNandDrv->u16_PageByteCnt ) ? LdrSize : pNandDrv->u16_PageByteCnt);
        LdrSize -= ((LdrSize < pNandDrv->u16_PageByteCnt ) ? LdrSize : pNandDrv->u16_PageByteCnt);
        i++;

        if (i== pNandDrv->u16_BlkPageCnt)
        {
            j++;
            i=0;
            printf("<flow control>reset i back to 0 and check block to next [block:%x]\n",j);
        }
    }

    return 1;
}


static U16 HEADER_OFFSET = 0x100;

U32 drvNAND_WriteBINParam(int BINBLOCK,U8* u8BINPos)
{
    U32             Bin2MIUPos = 0;
    U32             BinSize = 0;
    U32             APPEntryOffset = 0; // means the actually position offset from u8BINPos for bin file to store

    int i=0,j=BINBLOCK;

    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();


    Bin2MIUPos = *((U32*)(u8BINPos));
    BinSize = *((U32*)(u8BINPos+8));
    APPEntryOffset = *((U32*)(u8BINPos+4));
    Bin2MIUPos += HEADER_OFFSET; //APPEntryOffset;
    u8BINPos += HEADER_OFFSET;//APPEntryOffset;
    BinSize -= HEADER_OFFSET;//APPEntryOffset;
    APPEntryOffset -= HEADER_OFFSET;
    *((U32*)(nand_buffer2+0x00)) = BINIDTAG;
    *((U32*)(nand_buffer2+0x04)) = Bin2MIUPos;
    *((U32*)(nand_buffer2+0x10)) = BINIDTAG; // the position we are going to put to MIU and jump
    *((U32*)(nand_buffer2+0x14)) = BinSize;
    *((U32*)(nand_buffer2+0x20)) = BINIDTAG; // the position we are going to put to MIU and jump
    *((U32*)(nand_buffer2+0x24)) = APPEntryOffset;
    //*((U32*)(nand_buffer_spare+16)) = APPEntryOffset;//start point of AP
    printf("APPEntryOffset %8x \n",APPEntryOffset);
    printf("[%x]MIU pos %x\r\nBin file size %x\r\nApp Entry offset %x\r\n",(U32)u8BINPos,Bin2MIUPos,BinSize,APPEntryOffset);
    
    memset(nand_buffer_spare, -1, sizeof(nand_buffer_spare));

    while(BinSize)
    {
        if (i==0)
        {
            printf("<flow control>issue erase [block:%x] operation!\n",j);
            NC_EraseBlk(j * pNandDrv->u16_BlkPageCnt);
            BFN_WritePage_MIU((j * pNandDrv->u16_BlkPageCnt)+i,(U8*)nand_buffer2,(U8*)nand_buffer_spare,1,0);
            i++;
            continue;
        }

        BFN_WritePage_MIU((j * pNandDrv->u16_BlkPageCnt)+i,(U8*)u8BINPos,(U8*)nand_buffer_spare,1,0);

        u8BINPos += ((BinSize < pNandDrv->u16_PageByteCnt ) ? BinSize : pNandDrv->u16_PageByteCnt);
        BinSize -= ((BinSize < pNandDrv->u16_PageByteCnt ) ? BinSize : pNandDrv->u16_PageByteCnt);
        i++;

        if (i== pNandDrv->u16_BlkPageCnt)
        {
            j++;
            i=0;
            printf("<flow control>reset i back to 0 and check block to next [block:%x]\n",j);
        }
    }

    return 1;
}


U32 drvNAND_ReadNANDMIUParam(void)
{
    U32 row_pos = 0;
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    while(1) {
        printf("next row position %x\n",row_pos);
        BFN_ReadSector_RIU(row_pos, 0,nand_buffer,nand_buffer_spare);

        printf("%x\n",*((U32*)(nand_buffer_spare)));
        if (*((U32*)(nand_buffer_spare)) == MIUIDTAG)
        {
            printf("tag correct\n");
            printf("read next block\n");

            //for 2K pagesize only
            BFN_ReadSector_RIU(row_pos, 1,nand_buffer+0x200,nand_buffer_spare+0x10);

            printf("page number %x\n",nand_buffer_spare[20]);
            break;
        }

        row_pos += pNandDrv->u16_BlkPageCnt;
        if (row_pos > (0x10*pNandDrv->u16_BlkPageCnt ))
            return 1;

    }
    return 0;
}




U32 drvNAND_BFN_FLASH_INFO(U32 bfn_type,U32 bfn_blk,U32 bfn_addr)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();

    printf("byte per page %d page per block %d total block %d\n", pNandDrv->u16_PageByteCnt, pNandDrv->u16_BlkPageCnt, pNandDrv->u16_BlkCnt);

    nand_buffer = (unsigned char *)bfn_addr;

    switch (bfn_type)
    {
    case 1://miu
        // if Partial Mode (if not 512B/page, set Partial Mode)
        drvNAND_WriteMIUParam(bfn_blk,bfn_addr);
        //drvNAND_ReadNANDMIUParam(); /* for Test */
        break;
    case 2://ldr
        drvNAND_WriteLoader(bfn_blk,(U8*)bfn_addr);
        break;
    case 3://app
        printf("bfn block %x bfn address %x\n",bfn_blk,bfn_addr);
        drvNAND_WriteBINParam(bfn_blk,(U8*)bfn_addr);
        break;
    default:
        return 0;
    }

    return 1;
}
//=====================
#endif

#endif //(CONFIG_COMMANDS & CFG_CMD_NAND)
