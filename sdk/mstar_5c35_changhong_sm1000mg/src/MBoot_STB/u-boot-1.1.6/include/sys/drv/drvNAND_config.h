#ifndef __UNFD_CONFIG_H__
#define __UNFD_CONFIG_H__

#if 0   //ttt
//=====================================================
// select a HW platform:
//   - 1: enable, 0: disable.
//   - only one platform can be 1, others have to be 0.
//=====================================================
#define NAND_DRV_C5_ROM       0
#define NAND_DRV_C5_ECOS      0
//#define NAND_DRV_C5_NPRG      0 // programmer is attached with all platforms
#define NAND_DRV_OS_NOS       0
#define NAND_DRV_B3_ROM       1
#define NAND_DRV_B3_RTK       0
#define NAND_DRV_G1_ROM       0
#define NAND_DRV_G1_RTK       0
#define NAND_DRV_LINUX        0




//=====================================================
// do NOT edit the following content.
//=====================================================
#if defined(NAND_DRV_C5_ROM) && NAND_DRV_C5_ROM
  #include "config_nand/drvNAND_c5_rom.h"
#elif defined(NAND_DRV_C5_ECOS) && NAND_DRV_C5_ECOS
  #include "config_nand/drvNAND_c5_ecos.h"
#elif defined(NAND_DRV_C5_NPRG) && NAND_DRV_C5_NPRG /*NPRG same with eCos*/
  #include "config_nand/drvNAND_c5_ecos.h"
#elif defined(NAND_DRV_B3_ROM) && NAND_DRV_B3_ROM
  #include "config_nand/drvNAND_b3_rom.h"
#elif defined(NAND_DRV_B3_RTK) && NAND_DRV_B3_RTK
  #include "drvNAND_b3_rtk.h"
#elif defined(NAND_DRV_G1_ROM) && NAND_DRV_G1_ROM
  #include "config_nand/drvNAND_g1_ROM.h"
#elif defined(NAND_DRV_G1_RTK) && NAND_DRV_G1_RTK
  #include "drvNAND_g1_rtk.h"
#elif defined(NAND_DRV_LINUX) && NAND_DRV_LINUX
  #include "config_nand/drvNAND_linux.h"
#else
  #error "Error! no platform selected."
#endif


//do NOT edit the following content, for HAL use.

//=====================================================
// misc. do NOT edit the following content.
//=====================================================
#define NAND_DMA_RACING_PATCH        0
#define NAND_DMA_PATCH_WAIT_TIME     10000 // us -> 10ms
#define NAND_DMA_RACING_PATTERN      (((U32)'D'<<24)|((U32)2<<16)|((U32)'B'<<8)|(U32)8)

#define FCIE3_SW_DEFAULT_CLK  NFIE_CLK_26M

extern void nand_reset_WatchDog(void);
extern U32 nand_clock_setting(MS_U32 u32ClkParam);
extern U32 nand_translate_DMA_address_Ex(MS_U32 u32_DMAAddr, MS_U32 u32_ByteCnt);
extern U32 nand_WaitCompleteIntr(MS_U16 u16_WaitEvent, MS_U32 u32_MicroSec);
extern void nand_enable_intr_mode(void);

#endif //ttt

//=====================================================
// HW Timer for Delay
//=====================================================
#define HW_TIMER_DELAY_1us	    1
#define HW_TIMER_DELAY_10us     10
#define HW_TIMER_DELAY_100us	100
#define HW_TIMER_DELAY_1ms	    (1000 * HW_TIMER_DELAY_1us)
#define HW_TIMER_DELAY_5ms	    (5    * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_10ms	    (10   * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_100ms	(100  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_500ms	(500  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_1s	    (1000 * HW_TIMER_DELAY_1ms)

//===========================================================
// Time Dalay, do NOT edit the following content, for NC_WaitComplete use.
//===========================================================
#define DELAY_100us_in_us  100
#define DELAY_500us_in_us  500
#define DELAY_1ms_in_us    1000
#define DELAY_10ms_in_us   10000
#define DELAY_100ms_in_us  100000
#define DELAY_500ms_in_us  500000
#define DELAY_1s_in_us     1000000
#define WAIT_ERASE_TIME    (DELAY_1s_in_us<<2)
#define WAIT_WRITE_TIME    (DELAY_1s_in_us<<2)
#define WAIT_READ_TIME     (DELAY_1ms_in_us<<10)	//DELAY_500us_in_us
#define WAIT_PROBE_TIME    (DELAY_100us_in_us<<2)
#define WAIT_RESET_TIME    (WAIT_ERASE_TIME)

//=====================================================
// set FCIE clock
//=====================================================
//#define NFIE_CLK_MASK         (BIT4-1)
#define NFIE_CLK_300K		0
#define NFIE_CLK_750K		1
#define NFIE_CLK_5_4M		2
#define NFIE_CLK_13_5M		3
#define NFIE_CLK_18M		4
#define NFIE_CLK_22_7M		5
#define NFIE_CLK_27M		6
#define NFIE_CLK_32M		7
#define NFIE_CLK_43_2M		8
#define NFIE_CLK_54M		9
#define NFIE_CLK_72M		10
#define NFIE_CLK_86M		11

#define FCIE3_SW_DEFAULT_CLK  NFIE_CLK_27M

#define NAND_PARTITAION_BYTE_CNT    0x200
#endif /* __UNFD_CONFIG_H__ */
