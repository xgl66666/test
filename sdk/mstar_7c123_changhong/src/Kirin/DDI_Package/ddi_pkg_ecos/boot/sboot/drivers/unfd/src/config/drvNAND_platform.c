//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>

#include "drvNAND.h"

#if (defined(NAND_DRV_TV_MBOOT)&&NAND_DRV_TV_MBOOT)
U32 nand_hw_timer_delay(U32 u32usTick)
{
    U32 u32HWTimer = 0;
    volatile U16 u16TimerLow = 0;
    volatile U16 u16TimerHigh = 0;

    // reset HW timer
    REG_WRITE_UINT16(TIMER1_MAX_LOW, 0xFFFF);
    REG_WRITE_UINT16(TIMER1_MAX_HIGH, 0xFFFF);
    REG_WRITE_UINT16(TIMER1_ENABLE, 0);

    // start HW timer
    REG_SET_BITS_UINT16(TIMER1_ENABLE, 0x0001);

    while( u32HWTimer < 12*u32usTick ) // wait for u32usTick micro seconds
    {
        REG_READ_UINT16(TIMER1_CAP_LOW, u16TimerLow);
        REG_READ_UINT16(TIMER1_CAP_HIGH, u16TimerHigh);

        u32HWTimer = (u16TimerHigh<<16) | u16TimerLow;
    }

    REG_WRITE_UINT16(TIMER1_ENABLE, 0);

    return u32usTick+1;
}

U32 nand_pads_init(NAND_DRIVER *pNandDrv, U16 u16_NandMode)
{
    pNandDrv = pNandDrv;

#if defined(CONFIG_MSTAR_KERES)
    REG_CLR_BITS_UINT16(reg_nf_en, REG_NAND_MODE_MASK);
    REG_SET_BITS_UINT16(reg_nf_en, u16_NandMode);

    if(NAND_MODE1 == u16_NandMode )
    {
        REG_SET_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
        REG_SET_BITS_UINT16(reg_ci_c_pe, 0x03FF);
        REG_SET_BITS_UINT16(reg_ci_d_pe, 0x00FF);
    }
    else if(NAND_MODE2 == u16_NandMode)
    {
        REG_SET_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
        REG_CLR_BITS_UINT16(reg_ci_c_pe, 0x03FF);
        REG_CLR_BITS_UINT16(reg_ci_d_pe, 0x00FF);
    }
    else if(NAND_MODE3 == u16_NandMode)
    {
        REG_CLR_BITS_UINT16(reg_ci_a_pe, 0x7FFF);
        REG_CLR_BITS_UINT16(reg_ci_c_pe, 0x03FF);
        REG_CLR_BITS_UINT16(reg_ci_d_pe, 0x00FF);
    }

    REG_SET_BITS_UINT16(reg_gpio_pe, BIT11|BIT3);
#endif

    return UNFD_ST_SUCCESS;
}


U32 nand_pads_switch(NAND_DRIVER *pNandDrv, U32 u32EnableFCIE)
{
    pNandDrv = pNandDrv;
    u32EnableFCIE = u32EnableFCIE;

    // Disable all pad in
    REG_CLR_BITS_UINT16(reg_allpad_in, BIT15);

#if defined(CONFIG_MSTAR_KERES)
    // Only support SDR mode, do nothing
#endif // ENABLE_NUGGET

    return UNFD_ST_SUCCESS;
}


U32 nand_clock_setting(NAND_DRIVER *pNandDrv, U32 u32ClkParam, U16 u16_ReLatch)
{
#if defined(CONFIG_MSTAR_KERES)
    /*div4 enable*/
    REG_SET_BITS_UINT16(reg_ckg_fcie, BIT_CLK_ENABLE);

    /*set FCIE 4x clock*/
    REG_CLR_BITS_UINT16(REG_CLK_EMMC, BIT1|BIT0);
    REG_CLR_BITS_UINT16(REG_CLK_EMMC, EMMC_CLK_MASK);
    REG_SET_BITS_UINT16(REG_CLK_EMMC, u32ClkParam << 0);

    pNandDrv->u16_Reg57_RELatch &= ~BIT_NC_LATCH_DATA_MASK;
    pNandDrv->u16_Reg57_RELatch |= u16_ReLatch;

    NC_Config(pNandDrv);
#endif

    return UNFD_ST_SUCCESS;
}


U32 NC_PlatformResetPre(NAND_DRIVER *pNandDrv)
{
    pNandDrv = pNandDrv;
    return UNFD_ST_SUCCESS;
}

U32 NC_PlatformResetPost(NAND_DRIVER *pNandDrv)
{
    U16 u16_Reg;

    REG_WRITE_UINT16(NC_PATH_CTL, BIT_NC_EN);

    REG_READ_UINT16(FCIE_NC_CIFD_BASE, u16_Reg); // dummy read for CIFD clock

    REG_READ_UINT16(FCIE_NC_CIFD_BASE, u16_Reg); // dummy read for CIFD clock	twice said by designer

    REG_WRITE_UINT16(NC_WIDTH, FCIE_REG41_VAL);  // duty cycle 3:1 in 86Mhz (depends on chip config)

#if defined(CONFIG_MSTAR_KERES)

    /*HW bug
      In order to fix the front data overlapped by tail data of spare in CIFD when spare byte > 512
      But when ecc correctable Not in first sector, the following sector spare data will not be copied to CIFD.

      when spare > 512 using spare dma
      */
    REG_SET_BITS_UINT16(NC_DDR_CTRL, BIT7);
#endif

    return UNFD_ST_SUCCESS;
}

#if !(defined(ENABLE_NAND_RIU_MODE) && ENABLE_NAND_RIU_MODE)
U32 nand_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
    flush_cache();
    flush_L2();
    return  (u32_DMAAddr & 0x1fffffff);
}
#endif

U32 NC_PlatformInit(NAND_DRIVER *pNandDrv)
{
    REG_WRITE_UINT16(NC_PATH_CTL, 0x20);

    nand_pads_switch(pNandDrv, NAND_PAD_BYPASS_MODE);

    return UNFD_ST_SUCCESS;
}


#else
  #error "Error! no platform functions."
#endif
