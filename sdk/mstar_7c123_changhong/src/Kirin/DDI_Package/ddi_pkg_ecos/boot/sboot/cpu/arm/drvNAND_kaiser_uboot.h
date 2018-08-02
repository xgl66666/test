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
#ifndef __UNFD_KAISER_LINUX_H__
#define __UNFD_KAISER_LINUX_H__

#define REG_BANK_CLKGEN               (0x580)
#define REG_BANK_CHIPTOP              (0xF00)
#define REG_BANK_FCIE0                (0x8980)
#define REG_BANK_FCIE2                (0x8A00)
#define REG_BANK_TIMER0               (0x1810)

#define RIU_PM_BASE                   0x1F000000
#define RIU_BASE                      0x1F200000
#define REG_OFFSET_SHIFT_BITS         2

#define MPLL_CLK_REG_BASE_ADDR        (RIU_BASE+(REG_BANK_CLKGEN<<REG_OFFSET_SHIFT_BITS))
#define CHIPTOP_REG_BASE_ADDR         (RIU_BASE+(REG_BANK_CHIPTOP<<REG_OFFSET_SHIFT_BITS))
#define FCIE_REG_BASE_ADDR            (RIU_BASE+(REG_BANK_FCIE0<<REG_OFFSET_SHIFT_BITS))
#define FCIE_NC_CIFD_BASE             (RIU_BASE+(REG_BANK_FCIE2<<REG_OFFSET_SHIFT_BITS))
#define TIMER0_REG_BASE_ADDR          (RIU_PM_BASE+(REG_BANK_TIMER0<<REG_OFFSET_SHIFT_BITS))

#define REG(Reg_Addr)                 (*(volatile MS_U16*)(Reg_Addr))
#define GET_REG_ADDR(x, y)            (x+((y)<<REG_OFFSET_SHIFT_BITS))

#define REG_WRITE_UINT16(reg_addr, val)   REG(reg_addr) = val
#define REG_READ_UINT16(reg_addr, val)    val = REG(reg_addr)
#define HAL_WRITE_UINT16(reg_addr, val)   (REG(reg_addr) = val)
#define HAL_READ_UINT16(reg_addr, val)    val = REG(reg_addr)
#define REG_SET_BITS_UINT16(reg_addr, val)  REG(reg_addr) |= (val)
#define REG_CLR_BITS_UINT16(reg_addr, val)  REG(reg_addr) &= ~(val)
#define REG_W1C_BITS_UINT16(reg_addr, val)  REG_WRITE_UINT16(reg_addr, REG(reg_addr)&(val))

#define REG50_ECC_CTRL_INIT_VALUE     0

//=====================================================
// Nand Driver configs
//=====================================================

#define UNFD_CACHE_LINE           0x80
//=====================================================
// tool-chain attributes
//=====================================================
#define CONFIG_MIU0_BUSADDR       0x20000000

//=====================================================
// HW Timer for Delay
//=====================================================
#define TIMER0_ENABLE             GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x00)
#define TIMER0_HIT                GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x01)
#define TIMER0_MAX_LOW            GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x02)
#define TIMER0_MAX_HIGH           GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x03)
#define TIMER0_CAP_LOW            GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x04)
#define TIMER0_CAP_HIGH           GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x05)

//===========================================================
// Time Dalay, do NOT edit the following content, for NC_WaitComplete use.
//===========================================================
#define DELAY_100us_in_us           100
#define DELAY_500us_in_us           500
#define DELAY_1ms_in_us             1000
#define DELAY_10ms_in_us            10000
#define DELAY_100ms_in_us           100000
#define DELAY_500ms_in_us           500000
#define DELAY_1s_in_us              1000000

#define WAIT_ERASE_TIME             (DELAY_1s_in_us<<2)
#define WAIT_WRITE_TIME             (DELAY_1s_in_us<<2)
#define WAIT_READ_TIME              (DELAY_1ms_in_us<<10)  //DELAY_500us_in_us
#define WAIT_PROBE_TIME             (DELAY_100us_in_us<<2)
#define WAIT_RESET_TIME             (WAIT_ERASE_TIME)


//=====================================================
// Pads Switch
//=====================================================
#define reg_fcie2macro_sd_bypass        GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x35)
#define BIT_SD_USE_BYPASS               BIT7
#define BIT_FCIE2MACRO_SD_BYPASS        BIT6
#define BIT_EMMC_SWRSTZ_EN              BIT5
#define BIT_EMMC_SWRSTZ                 BIT4
#define BIT_PAD_IN_SEL_SD               BIT1
#define BIT_PAD_IN_SEL_SDIO             BIT0

#define reg_allpad_in                   GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x50)
#define BIT_ALL_PAD_IN                  BIT15

#define reg_nf_en                       GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x06)
#define CFG_EMMC_PAD_MASK               (BIT12|BIT13)
#define CFG_SD_PAD_MASK                 (BIT14|BIT15)
#define CFG_NAND_PAD_MASK               (BIT0|BIT1)
#define BIT_NAND_PAD_MODE2               BIT1


//=====================================================
// set FCIE clock
//=====================================================
#define NFIE_CLK_MASK           (BIT5|BIT4|BIT3|BIT2)


#define NFIE_CLK_300K           (0 << 2)
#define NFIE_CLK_750K           (1 << 2)
#define NFIE_CLK_5_4M           (2 << 2)
#define NFIE_CLK_13M            (3 << 2)
#define NFIE_CLK_18M            (4 << 2)
#define NFIE_CLK_22_7M          (5 << 2)
#define NFIE_CLK_27M            (6 << 2)
#define NFIE_CLK_32M            (7 << 2)
#define NFIE_CLK_43_2M          (8 << 2)
#define NFIE_CLK_54M            (9 << 2)
#define NFIE_CLK_72M            (10 << 2)
#define NFIE_CLK_86M            (11 << 2)
#define NFIE_CLK_XTAL           (13 << 2)
#define NFIE_CLK_62M            (14 << 2)


#define reg_ckg_fcie            GET_REG_ADDR(MPLL_CLK_REG_BASE_ADDR, 0x64)

//=====================================================
// transfer DMA Address
//=====================================================
#define MIU_BUS_WIDTH_BITS          3 // Need to confirm
#define MIU_CHECK_LAST_DONE         1

//=====================================================
// misc
//=====================================================
#define NC_REG_MIU_LAST_DONE        NC_MIE_EVENT
#define NANDINFO_ECC_TYPE ECC_TYPE_40BIT1KB

#endif

