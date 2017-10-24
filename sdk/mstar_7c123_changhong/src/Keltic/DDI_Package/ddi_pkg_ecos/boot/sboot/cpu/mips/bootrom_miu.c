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

#include "boot.h"
#include "chip.h"
#include "Board.h"

#if defined(MIU1_LEN) && (MIU1_LEN != 0)
#define MIU_NUMBERS 2
#else
#define MIU_NUMBERS 1
#endif

#define REG_MIU_BASE          (0xBF200000 + 0x2400)     // 0xbf202400: 0x101200
#define REG_MIU_TEST_CTRL     (REG_MIU_BASE + 0x70 * 4) // 0xbf2025c0: 0x1012e0
#define REG_MIU_TEST_BASE     (REG_MIU_BASE + 0x71 * 4) // 0xbf2025c4: 0x1012e2
#define REG_MIU_TEST_LENGTH_L (REG_MIU_BASE + 0x72 * 4) // 0xbf2025c8: 0x1012e4
#define REG_MIU_TEST_LENGTH_H (REG_MIU_BASE + 0x73 * 4) // 0xbf2025cc: 0x1012e6
#define REG_MIU_TEST_DATA     (REG_MIU_BASE + 0x74 * 4) // 0xbf2025d0: 0x1012e8
#define REG_MIU_SEL0          (REG_MIU_BASE + 0x78 * 4) // 0xbf2025e0: 0x1012f0
#define REG_MIU_ADCTEST_EN    (REG_MIU_BASE + 0x18 * 4) // 0xbf202460: 0x101230
#define MIU_TEST_LENGTH 0x800000
#define MIU_BIST_TIMEOUT 5000 // ms


#ifdef CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE
#ifdef CONFIG_MSTAR_KAPPA
#else
MS_U32 ChkBists(void)
{
    MS_U16 i;
    for(i = 0x81; i < 0x8C; i += 2)
    {
        MS_U32 BIST_REG = 0xBF2025C0;
        MS_U16 tmp_reg = 0;

        WREG16(BIST_REG, 0);
        DelayMS(1);

        // start test bist
        WREG16(BIST_REG, i);

        do
        {
            tmp_reg = RREG16(BIST_REG);
        } while(!(tmp_reg & 0x8000));

        if(tmp_reg & 0x6000)
            return 0;           // for false
    }
    return 1;                   // for true
}
#endif
#ifdef CONFIG_MSTAR_KAPPA
//Kappa has been modified to change only single 4 bit each time
static void HAL_GoodPhaseScan(void)
{
    MS_U16 def_phase;
    MS_U16 tmp_val;

    MS_U16 i, j, k;


    {
        //choose miu0
        WREG16(0xBF2025E0, 0);//0x1012F0
        // store default phase setting
        def_phase = RREG16(0xBF221ADC);//0x110D6E
    }


    //_PUTW(def_phase); //print default phase

    WREG16(0xBF2025C4, 0x0000);//0x1012E2
    WREG16(0xBF2025C8, 0x8001);//0x1012E4

    WREG16(0xBF2025CC, 0x0000);//0x1012E6
    WREG16(0xBF2025D0, 0x00FF);//0x1012E8

    for (i=0;i<16;i+=4)
    {
        //_PUTC('i');
        MS_U16 min=0, max=0;
        tmp_val = 0xf;
        def_phase = (def_phase &(~(tmp_val<<i)));
        for (j=0;j<16;j++)
        {
            tmp_val = (def_phase)|(j<<i);

            WREG16(0xBF221ADC, tmp_val);//0x110D0E

            for(k = 0x81; k < 0x8C; k += 2)
            {
                WREG16(0xBF2025C0, 0);
                //DelayMS(1);

                WREG16(0xBF2025C0, k);

                do
                {
                    tmp_val = RREG16(0xBF2025C0);
                } while(!(tmp_val & 0x8000));

                if(tmp_val & 0x6000)
                {
                    tmp_val = 0;
                    break;
                }
                tmp_val=1;
            }

            if(tmp_val == 1)
            {
                if (min==0)
                    min=j;
                max = j;

                //_PUTX(j & 0xF);     // print phase num if success
            }
        }
        //_PUTC('j');
//_PUTC('k');
_PUTC('L');
        def_phase = (def_phase)|(((min+max)>>1)<<(i));
_PUTC('M');

    }
    WREG16(0xBF221ADC, def_phase);//0x110D6E

    _PUTW(def_phase); //print default phase

    WREG16(0xBF2025C0, 0x0000);//0x1012E0

    WREG8(0xBF2025CE, 0x00);//0x1012E0

    WREG16(0xBF2025E0, 0x0000);//0x1012F0
}
#else
void HAL_GoodPhaseScan(MS_U32 MIU_NUM)
{
    MS_U16 def_phase;
    MS_U16 tmp_val;
    MS_U16 good_phase_cnt = 0, good_phase_sum = 0;
    MS_U16 i;

    if(MIU_NUM == 0)            // MIU0
    {
        //choose miu0
        WREG16(0xBF2025E0, 0);
        // store default phase setting
        def_phase = RREG16(0xBF221ADC);
    }
    else                        // MIU1
    {
        //choose miu1
        WREG16(0xBF2025E0, 0x8000);
        // store default phase setting
        def_phase = RREG16(0xBF221BDC);
    }

    //_PUTW(def_phase); //print default phase
    _PUTC('[');

#ifdef CONFIG_MSTAR_KAISERIN
    WREG16(0xBF2025C4, 0x1FFF);
    WREG16(0xBF2025C8, 0x1000);
#elif defined(CONFIG_MSTAR_KRONUS)
    WREG16(0xBF2025C4, 0x0000);
    WREG16(0xBF2025C8, 0x8001);
#elif defined(CONFIG_MSTAR_KAPPA)
    WREG16(0xBF2025C4, 0x0000);
    WREG16(0xBF2025C8, 0x8001);
#endif
    WREG16(0xBF2025CC, 0x0000);
    WREG16(0xBF2025D0, 0x00FF);

    for(i = 0; i <= 32; i++)
    {
        if((i & 0xF) == 0)
        {
            if(i <= 16)         // 0, 16
            {
                if(MIU_NUM == 0)        // MIU0
                    WREG8(0xBF2025CE, (0x55 << (i >> 4)));      // set dqs_mask to 0x55 || 0xaa
                else            // MIU1
                    WREG8(0xBF200DCE, (0x55 << (i >> 4)));      // set dqs_mask to 0x55 || 0xaa
            }
        }

        //clk rst
        tmp_val = RREG8(0xBF221A1C);
        tmp_val = ((tmp_val & 0x9F) | 0x60);
        WREG8(0xBF221A1C, tmp_val);

        if(i < 16)              // x1
        {
            tmp_val = (((i & 0xF) << 4) | (def_phase & 0xf));
            if(MIU_NUM == 0)    // MIU0
                WREG16(0xBF221ADC, tmp_val);
            else                // MIU1
                WREG16(0xBF221BDC, tmp_val);
        }
        else if((i == 16) || (i == 32)) //x0
        {
            if(good_phase_cnt == 0)
            {
                good_phase_sum = def_phase;
            }
            else
            {
                good_phase_sum = good_phase_sum / good_phase_cnt;
                if(i == 16)     // 16
                {
                    good_phase_sum = good_phase_sum << 4;
                }
                else            // 32
                {
                    good_phase_sum = (good_phase_sum | (def_phase & 0xF0));
                }
            }
            def_phase = good_phase_sum;

            // clear value
            good_phase_cnt = 0;
            good_phase_sum = 0;

            tmp_val = ((i & 0xF) | (def_phase & 0xf0));

            if(MIU_NUM == 0)    // MIU0
                WREG16(0xBF221ADC, def_phase);
            else
                WREG16(0xBF221BDC, def_phase);
        }
        else                    // x0
        {
            tmp_val = ((i & 0xF) | (def_phase & 0xf0));
            if(MIU_NUM == 0)    // MIU0
                WREG16(0xBF221ADC, tmp_val);
            else                // MIU1
                WREG16(0xBF221BDC, tmp_val);
        }

        // clk unreset
        tmp_val = RREG8(0xBF221A1C);
        tmp_val = ((tmp_val & 0x9F) | 0x20);
        WREG8(0xBF221A1C, tmp_val);

        if(ChkBists() == 1)     // 1 for true
        {
            if(i == 32)
                break;
            good_phase_cnt++;
            good_phase_sum += (i & 0xF);
            _PUTX(i & 0xF);     // print phase num if success
        }
    }

    _PUTC(']');

    WREG16(0xBF2025C0, 0x0000);

    if(MIU_NUM == 0)            // MIU0
        WREG8(0xBF2025CE, 0x00);
    else                        // MIU1
        WREG8(0xBF200DCE, 0x00);

    WREG16(0xBF2025E0, 0x0000);
}
#endif

static void HAL_AutoPhaseScan(void)
{
    // Mask MIU0 group
    WREG16(0xBF20248C, 0x7FFE);

    // call good phase
#ifdef CONFIG_MSTAR_KAPPA
    HAL_GoodPhaseScan();
#else
    HAL_GoodPhaseScan(0);
#endif

    // UnMask MIU0 Group
    WREG16(0xBF20248C, 0x0000);

#if MIU_NUMBERS > 1
    // Mask MIU1 group
    WREG16(0xBF200C8C, 0x7FFE);

    // call good phase
    HAL_GoodPhaseScan(1);

    // UnMask MIU1 Group
    WREG16(0xBF200C8C, 0x0000);
#endif
    //Bist Test
    //HAL_BistTest();

    // stop bist test
    WREG16(0xBF2025C0, 0x0000);
}

#else // CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE

static void _MIU_BistTest(void)
{
    MS_U16 result;
    MS_U32 tmo = (MS_U32)(MIU_BIST_TIMEOUT) * (CPU_CLK_FREQ / 2 / 1000);
    MS_U32 count;

    WREG16(REG_MIU_TEST_CTRL, 0x0003);  // bitst test mode:3

    asm volatile ("mtc0 $0, $11" : :); // clear cp0 compare
    asm volatile ("mtc0 $0, $9" : :);  // clear cp0

    do
    {
        result = RREG16(REG_MIU_TEST_CTRL);
        asm volatile ("mfc0 %0, $9" : "=r" (count) :);
    } while((count < tmo) && (result & 0x8000) == 0);

    if((count < tmo) && ((result & 0x6000) == 0))
    {
        _PUTS("OK\n");
        WREG16(REG_MIU_SEL0, 0x0000);       // reset 0x1012f0 bit_15=0
        return;
    }

    _PUTS("FAIL\n");
    _Fail(ERROR_MIU_BIST);
}

#if MIU_NUMBERS > 1
static void _MIU_Bist1(void)
{
    MS_U16 u16Lo = (MIU_TEST_LENGTH >> 3) & 0xff;
    MS_U16 u16Hi = ((MIU_TEST_LENGTH >> 3) >> 16) & 0xff;

    WREG_DBG(0xA004, 2);
    _PUTS("BIST1_");

    WREG16(REG_MIU_TEST_CTRL, 0x0000);  //reset bist test
    WREG16(REG_MIU_TEST_BASE, 0x0000);
    WREG16(REG_MIU_TEST_LENGTH_L, u16Lo);
    WREG16(REG_MIU_TEST_LENGTH_H, u16Hi);

    WREG16(REG_MIU_SEL0, 0x8000);       //bit_15=1 choice to MIU_1

    _MIU_BistTest();
    WREG_DBG(0xA005, 2);
}
#endif

static void _MIU_Bist0(void)
{
    MS_U16 u16Lo = (MIU_TEST_LENGTH >> 3) & 0xff;
    MS_U16 u16Hi = ((MIU_TEST_LENGTH >> 3) >> 16) & 0xff;

    WREG_DBG(0xA002, 2);
    _PUTS("\nBIST0_");

    WREG16(REG_MIU_TEST_CTRL, 0x0000);  //reset bist test
    WREG16(REG_MIU_TEST_BASE, 0x0000);  //start address base unit
    WREG16(REG_MIU_TEST_LENGTH_L, u16Lo);
    WREG16(REG_MIU_TEST_LENGTH_H, u16Hi);

    WREG16(REG_MIU_TEST_DATA, 0x5aa5);  //bist test pattern

    _MIU_BistTest();
    WREG_DBG(0xA003, 2);
}

static void _MIU_Bist()
{
    WREG_DBG(0xA001, 2);

    _MIU_Bist0();

#if MIU_NUMBERS > 1
    _MIU_Bist1();
#endif

    WREG16(REG_MIU_TEST_CTRL, 0x0000);  //reset bist test
}

#endif // CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE


static void _InitParam_End(void)
{
#ifdef CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE
    HAL_AutoPhaseScan();
#else
    _MIU_Bist();
#endif

#ifdef CONFIG_MSTAR_KELTIC
    WREG16(REG_MIU_ADCTEST_EN, 0x4000);  // TSP  reg_adc_test_en
#endif

}

typedef struct _Entry
{
    union
    {
        struct
        {
            MS_U16 addr;
            MS_U16 value;
        } reg;

        struct
        {
            MS_U16 param1;
            MS_U16 param2;
        } cmd;
    };
} Entry;

#ifdef CONFIG_MSTAR_KAPPA

MS_U32 BOOTROM_C_InitParams(void *pParams)
{
    MS_U32 rbase = (0xbf20 << 16);
    MS_U32 width = 16;
    MS_U16 *gParse = pParams;

    while(1)
    {
        if((*(gParse)) == CHIPCFG_CMD_END)
            break;

        switch ((*(gParse)))
        {
            case CHIPCFG_CMD_IF:
                {
                    volatile MS_U16 *addr = (void *)(rbase + (*(gParse+1)));
                    gParse+=2;
                    if((*(gParse)) == CHIPCFG_CMD_END)
                    {
                        return 2;
                    }

                    MS_U16 value = (*(gParse));
                    MS_U16 mask = (*(gParse+1));

                    if((*addr & mask) != value)
                    {
                        // skip to CHIPCFG_CMD_ENDIF
                        //Entry *e;
                        do
                        {
                            //e = gParse++;
                            //if(e->cmd.param1 == CHIPCFG_CMD_END)
                            if((*(gParse)) == CHIPCFG_CMD_END)
                            {
                                gParse+=2;
                                return 3;
                            }
                            gParse+=2;
                        } while((*(gParse)) != CHIPCFG_CMD_ENDIF);
                    }
                }
                break;

            case CHIPCFG_CMD_ENDIF:
                // ignore
                break;

            case CHIPCFG_CMD_BASE:
                rbase = ((*(gParse+1)) << 16);
                width = 16;
                break;

            case CHIPCFG_CMD_BASE_8BITS:
                rbase = ((*(gParse+1)) << 16);
                width = 8;
                break;

            case CHIPCFG_CMD_WAIT:
                DelayMS((*(gParse+1)));
                break;

            case CHIPCFG_CMD_DBG:
                WREG_DBG((*(gParse+1)), 3);
                break;

            default:
                if(width == 8)
                {
                    MS_U8 old = RREG8(rbase + (*(gParse)));
                    MS_U8 val = ((*(gParse+1)) >> 8) & 0xff;
                    MS_U8 mask = (*(gParse+1)) & 0xff;
                    WREG8(rbase + (*(gParse)), (old & ~mask) | (val & mask));
                }
                else
                {
                    WREG16(rbase + (*(gParse)), (*(gParse+1)));
                }
                break;
        }
        gParse+=2;
    }

    //For Kappa U02 DDR3 patch, write phase setting
#if ( MIU_INTERFACE == DDR3_INTERFACE_QFP_128)
    if (RREG16(0xBF203D9C)>>8)  //Revision ID
    {
        WREG16(0xBF221A70, 0x0045);  //0x110d38
    }
#endif
    _InitParam_End();
    WREG_DBG(0xF001, 1);
    return 1;
}

#else
static const Entry *gEntrys;

static const Entry *_Next(void)
{
    const Entry *e = gEntrys++;

    if(e->cmd.param1 == CHIPCFG_CMD_END)
        return NULL;

    return e;
}

MS_U32 BOOTROM_C_InitParams(void *pParams)
{
    MS_U32 rbase = (0xbf20 << 16);
    MS_U32 width = 16;

    gEntrys = pParams;

    MS_U8 trim_read = 0,trim_write = 0;
    /*  Read DRAM ZQ TRIM DATA  */
    trim_read = *(volatile unsigned char *)(0xBF320000 + 0x3d79);
    trim_write = *(volatile unsigned char *)(0xBF000000 + (0x110d7c<<1));
    trim_write = trim_write | (trim_read << 4);
    *(volatile unsigned char *)(0xBF000000 + (0x110d7c<<1)) = trim_write;
    
    while(1)
    {
        const Entry *eCur = _Next();
        if(eCur == NULL)
            break;

        //_PUTW(rbase);
        //_PUTW(eCur->cmd.param1);
        //_PUTW(eCur->cmd.param2);


        switch (eCur->cmd.param1)
        {
            case CHIPCFG_CMD_IF:
                {
                    volatile MS_U16 *addr = (void *)(rbase + eCur->cmd.param2);
                    const Entry *eCmp = _Next();
                    if(eCmp == NULL)
                    {
                        WREG_DBG(0xF002, 1);
                        return 2;
                    }

                    MS_U16 value = eCmp->cmd.param1;
                    MS_U16 mask = eCmp->cmd.param2;

                    if((*addr & mask) != value)
                    {
                        // skip to CHIPCFG_CMD_ENDIF
                        const Entry *e;
                        do
                        {
                            e = _Next();
                            if(e == NULL)
                            {
                                WREG_DBG(0xF003, 1);
                                return 3;
                            }
                        } while(e->cmd.param1 != CHIPCFG_CMD_ENDIF);
                    }
                }
                break;

            case CHIPCFG_CMD_ENDIF:
                // ignore
                break;

            case CHIPCFG_CMD_BASE:
                rbase = (eCur->cmd.param2 << 16);
                width = 16;
                break;

            case CHIPCFG_CMD_BASE_8BITS:
                rbase = (eCur->cmd.param2 << 16);
                width = 8;
                break;

            case CHIPCFG_CMD_WAIT:
                DelayMS(eCur->cmd.param2);
                break;

            case CHIPCFG_CMD_DBG:
                WREG_DBG(eCur->cmd.param2, 3);
                break;

            default:
                if(width == 8)
                {
                    MS_U8 old = RREG8(rbase + eCur->reg.addr);
                    MS_U8 val = (eCur->reg.value >> 8) & 0xff;
                    MS_U8 mask = eCur->reg.value & 0xff;

                    WREG8(rbase + eCur->reg.addr, (old & ~mask) | (val & mask));
                }
                else
                {
                    WREG16(rbase + eCur->reg.addr, eCur->reg.value);
                }
                break;
        }
    }


#ifdef CONFIG_MSTAR_KELTIC
#if defined(CONFIG_DRAM_MOBF)
    WREG16(MIU_MOBF_CONTROL, 0x0005); /* Enable 64 bit scramble & 64 bits Pipeline*/
    WREG16(MOBF_ENABLE,0xc000); /* Triggler MOBF */
    //WREG16(MIU_MOBF_PROTECT,0x0008); /* Enable Write Protect -> Don't set bit3 to 1 to enable this feature*/
#else
    /* no MOBF and Disable MIU write Cache*/
    WREG16(MIU_WRITE_CACHE,0x0001);
#endif
#endif

    _InitParam_End();
    WREG_DBG(0xF001, 1);
    return 1;
}
#endif
