FCIE0_BASE                   = 0x1F222600
FCIE_MIE_EVENT               = (FCIE0_BASE + 0x00*4)
FCIE_MIE_INT_EN              = (FCIE0_BASE + 0x01*4)
FCIE_MMA_PRI_REG             = (FCIE0_BASE + 0x02*4)
FCIE_MIU_DMA_26_16           = (FCIE0_BASE + 0x03*4)
FCIE_MIU_DMA_15_0            = (FCIE0_BASE + 0x04*4)
FCIE_PATH_CTL                = (FCIE0_BASE + 0x0A*4)
FCIE_JOB_BL_CNT              = (FCIE0_BASE + 0x0B*4)
FCIE_SD_MODE                 = (FCIE0_BASE + 0x10*4)
FCIE_SD_CTRL                 = (FCIE0_BASE + 0x11*4)
FCIE_SDIO_CTL                = (FCIE0_BASE + 0x1B*4)
FCIE_BOOT_CONFIG             = (FCIE0_BASE + 0x2F*4)
FCIE_REG_0X2D                  = (FCIE0_BASE + 0x2D*4)
@@ CHIPTOP_BASE                 = 0x1F2203C0
CHIPTOP_BASE                 = 0x1F203C00
REG_CHIPTOP_0x06             = (CHIPTOP_BASE + 0x06*4)
REG_CHIPTOP_0x26             = (CHIPTOP_BASE + 0x26*4)
REG_CHIPTOP_0x29             = (CHIPTOP_BASE + 0x29*4)
REG_CHIPTOP_0x35             = (CHIPTOP_BASE + 0x35*4)
REG_CHIPTOP_0x50             = (CHIPTOP_BASE + 0x50*4)

@@ BANK_1018_BASE               = 0x1F220300
BANK_1018_BASE               = 0x1F203000
REG_BANK_1018_0x60           = (BANK_1018_BASE + 0x60*4)

BANK_100B_BASE               = 0x1F201600
REG_FCIE_CLK           		 = (BANK_100B_BASE + 0x64*4)


REG_MMA_PRI                  = (FCIE0_BASE + 0x02*4)
REG_MIU_ADDR_H               = (FCIE0_BASE + 0x03*4)
REG_MIU_ADDR_L               = (FCIE0_BASE + 0x04*4)
REG_MMA_PATH_CTL             = (FCIE0_BASE + 0x0A*4)
REG_JOB_BL_CNT               = (FCIE0_BASE + 0x0B*4)
REG_SD_MODE                  = (FCIE0_BASE + 0x10*4)
REG_SD_CTRL                  = (FCIE0_BASE + 0x11*4)
REG_BOOT_CONFIG              = (FCIE0_BASE + 0x2F*4)

EMMC_MAX_BLK_CNT = 4095
