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

#define REG_ADDR_BASE         0x1F000000 
#define REG_MIU_BASE          (0x1F200000 + 0x2400)     // 0xbf202400: 0x101200
#define REG_MIU_TEST_CTRL     (REG_MIU_BASE + 0x70 * 4) // 0xbf2025c0: 0x1012e0
#define REG_MIU_TEST_BASE     (REG_MIU_BASE + 0x71 * 4) // 0xbf2025c4: 0x1012e2
#define REG_MIU_TEST_LENGTH_L (REG_MIU_BASE + 0x72 * 4) // 0xbf2025c8: 0x1012e4
#define REG_MIU_TEST_LENGTH_H (REG_MIU_BASE + 0x73 * 4) // 0xbf2025cc: 0x1012e6
#define REG_MIU_TEST_DATA     (REG_MIU_BASE + 0x74 * 4) // 0xbf2025d0: 0x1012e8
#define REG_MIU_SEL0          (REG_MIU_BASE + 0x78 * 4) // 0xbf2025e0: 0x1012f0

#define MIU_TEST_LENGTH 0x800000
#define MIU_BIST_TIMEOUT 5000 // ms

enum 
{
    AC_ON = 1,
    DC_ON = 2,
    
};

#if CONFIG_MSTAR_STR_ENABLE

#define STR_IS_AC_FLOW_CONTROL 	0x1F001CDC

static void _Reset_AVD(void)
{
    MS_U16 Temp;
    Temp = RREG16(REG_ADDR_BASE + (0x103612 << 1));                
    WREG16(REG_ADDR_BASE + (0x103612 << 1), Temp & (~0x4000));  
          
    Temp = RREG16(REG_ADDR_BASE + (0x103612 << 1));                
    WREG16(REG_ADDR_BASE + (0x103612 << 1), Temp | 0x4000);    
        
    Temp = RREG16(REG_ADDR_BASE + (0x103612 << 1));                
    WREG16(REG_ADDR_BASE + (0x103612 << 1), Temp & (~0x4000));        
}

static void _Enter_Refresh_Mode(void)
{
    MS_U16 Temp=0;
    WREG16(REG_ADDR_BASE + (0x1012e0 << 1), 0x0000);  
   
    WREG16(REG_ADDR_BASE + (0x101246 << 1), 0xFFFE);
    WREG16(REG_ADDR_BASE + (0x101266 << 1), 0xFFFF);
    WREG16(REG_ADDR_BASE + (0x101286 << 1), 0xFFFF);
    WREG16(REG_ADDR_BASE + (0x1012A6 << 1), 0xFFFF);
    
    WREG16(REG_ADDR_BASE + (0x101218 << 1), 0x0400);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x002F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x042F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x052F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x002F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x022F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x032F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x002F);
    
    WREG16(REG_ADDR_BASE + (0x101246 << 1), 0xFFFF);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x202F);   
    
#if MIU_NUMBERS > 1    
    WREG16(REG_ADDR_BASE + (0x100646 << 1), 0xFFFE);
    WREG16(REG_ADDR_BASE + (0x100666 << 1), 0xFFFF);
    WREG16(REG_ADDR_BASE + (0x100686 << 1), 0xFFFF);
    WREG16(REG_ADDR_BASE + (0x1006A6 << 1), 0xFFFF);
    
    WREG16(REG_ADDR_BASE + (0x100618 << 1), 0x0400);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x002F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x042F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x052F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x002F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x022F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x032F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x002F);
    
    WREG16(REG_ADDR_BASE + (0x100646 << 1), 0xFFFF);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x202F);
#endif     
}

static void _Exit_Refresh_Mode(void)
{
    MS_U16 Temp=0;
      
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x022F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x032F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x002F);
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x000F);      
#if MIU_NUMBERS > 1      
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x022F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x032F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x002F);
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x000F);
#endif     
    
    DelayMS(50);
    
    WREG16(REG_ADDR_BASE + (0x101200 << 1), 0x001F);
#if MIU_NUMBERS > 1
    WREG16(REG_ADDR_BASE + (0x100600 << 1), 0x001F);
#endif
    
    DelayMS(5);
      
    WREG16(REG_ADDR_BASE + (0x101246 << 1), 0x0000);
    WREG16(REG_ADDR_BASE + (0x101266 << 1), 0x0000);
    WREG16(REG_ADDR_BASE + (0x101286 << 1), 0x0000);
    WREG16(REG_ADDR_BASE + (0x1012A6 << 1), 0x0000);
#if MIU_NUMBERS > 1  
    WREG16(REG_ADDR_BASE + (0x100646 << 1), 0x0000);
    WREG16(REG_ADDR_BASE + (0x100666 << 1), 0x0000);
    WREG16(REG_ADDR_BASE + (0x100686 << 1), 0x0000);
    WREG16(REG_ADDR_BASE + (0x1006A6 << 1), 0x0000);
#endif
    
    WREG16(REG_ADDR_BASE + (0x10121e << 1), 0x0c08);
    WREG16(REG_ADDR_BASE + (0x1012e0 << 1), 0x0000);
}


#endif

static void _Disable_ATOP_SW(void)
{
    MS_U16 Temp;

    
    Temp = RREG16(REG_ADDR_BASE + (0x101202 << 1));
    WREG16(REG_ADDR_BASE + (0x101202 << 1), Temp & (~0xF000));  
    
    Temp = RREG16(REG_ADDR_BASE + (0x110D54 << 1));
    WREG16(REG_ADDR_BASE + (0x110D54 << 1), Temp & (~0x0070));  
    
    Temp = RREG16(REG_ADDR_BASE + (0x110D08 << 1));
    WREG16(REG_ADDR_BASE + (0x110D08 << 1), Temp | 0x003F);
    
#if MIU_NUMBERS > 1   
    Temp = RREG16(REG_ADDR_BASE + (0x100602 << 1));
    WREG16(REG_ADDR_BASE + (0x100602 << 1), Temp & (~0xF000)); 
    
    Temp = RREG16(REG_ADDR_BASE + (0x110DD4 << 1));
    WREG16(REG_ADDR_BASE + (0x110DD4 << 1), Temp & (~0x0070));
    
    Temp = RREG16(REG_ADDR_BASE + (0x110D88 << 1));
    WREG16(REG_ADDR_BASE + (0x110D88 << 1), Temp | 0x003F); 
#endif
}

#if 1
#ifdef CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE
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

void HAL_GoodPhaseScan(MS_U32 MIU_NUM)
{
    MS_U16 def_phase;
    MS_U16 tmp_val;
    MS_U16 good_phase_cnt = 0, good_phase_sum = 0;
    MS_U16 i;

    if(MIU_NUM == 0)            // MIU0
    {
        //choose miu0
        WREG16(0x1F2025E0, 0);
        // store default phase setting
        def_phase = RREG16(0x1F221ADC);
    }
    else                        // MIU1
    {
        //choose miu1
        WREG16(0x1F2025E0, 0x8000);
        // store default phase setting
        def_phase = RREG16(0x1F221BDC);
    }

    //_PUTW(def_phase); //print default phase
    _PUTC('[');

#ifdef CONFIG_MSTAR_KAISERIN
    WREG16(0x1F2025C4, 0x1FFF);
    WREG16(0x1F2025C8, 0x1000);
#elif defined(CONFIG_MSTAR_KRONUS)
    WREG16(0x1F2025C4, 0x0000);
    WREG16(0x1F2025C8, 0x8001);
#endif
    WREG16(0x1F2025CC, 0x0000);
    WREG16(0x1F2025D0, 0x00FF);

    for(i = 0; i <= 32; i++)
    {
        if((i & 0xF) == 0)
        {
            if(i <= 16)         // 0, 16
            {
                if(MIU_NUM == 0)        // MIU0
                    WREG8(0x1F2025CE, (0x55 << (i >> 4)));      // set dqs_mask to 0x55 || 0xaa
                else            // MIU1
                    WREG8(0x1F200DCE, (0x55 << (i >> 4)));      // set dqs_mask to 0x55 || 0xaa
            }
        }

        //clk rst
        tmp_val = RREG8(0x1F221A1C);
        tmp_val = ((tmp_val & 0x9F) | 0x60);
        WREG8(0x1F221A1C, tmp_val);

        if(i < 16)              // x1
        {
            tmp_val = (((i & 0xF) << 4) | (def_phase & 0xf));
            if(MIU_NUM == 0)    // MIU0
                WREG16(0x1F221ADC, tmp_val);
            else                // MIU1
                WREG16(0x1F221BDC, tmp_val);
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
                WREG16(0x1F221ADC, def_phase);
            else
                WREG16(0x1F221BDC, def_phase);
        }
        else                    // x0
        {
            tmp_val = ((i & 0xF) | (def_phase & 0xf0));
            if(MIU_NUM == 0)    // MIU0
                WREG16(0x1F221ADC, tmp_val);
            else                // MIU1
                WREG16(0x1F221BDC, tmp_val);
        }

        // clk unreset
        tmp_val = RREG8(0x1F221A1C);
        tmp_val = ((tmp_val & 0x9F) | 0x20);
        WREG8(0x1F221A1C, tmp_val);

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

    WREG16(0x1F2025C0, 0x0000);

    if(MIU_NUM == 0)            // MIU0
        WREG8(0x1F2025CE, 0x00);
    else                        // MIU1
        WREG8(0x1F200DCE, 0x00);

    WREG16(0x1F2025E0, 0x0000);
}

void HAL_AutoPhaseScan(void)
{
    // Mask MIU0 group
    WREG16(0x1F20248C, 0x7FFE);

    // call good phase
    HAL_GoodPhaseScan(0);

    // UnMask MIU0 Group
    WREG16(0x1F20248C, 0x0000);

#if MIU_NUMBERS > 1
    // Mask MIU1 group
    WREG16(0x1F200C8C, 0x7FFE);

    // call good phase
    HAL_GoodPhaseScan(1);

    // UnMask MIU1 Group
    WREG16(0x1F200C8C, 0x0000);
#endif
    //Bist Test
    //HAL_BistTest();

    // stop bist test
    WREG16(0x1F2025C0, 0x0000);
}

#else // CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE

static void _MIU_BistTest(void)
{
    MS_U16 result;


    WREG16(REG_MIU_TEST_CTRL, 0x0003);  // bitst test mode:3


    do
    {
        result = RREG16(REG_MIU_TEST_CTRL);
        //asm volatile ("mfc0 %0, $9" : "=r" (count) :);
    } while((result & 0x8000) == 0);

    if((result & 0x6000) == 0)
    {
        _PUTS("OK\n");
        WREG16(REG_MIU_SEL0, 0x0000);       // reset 0x1012f0 bit_15=0
        return;
    }

    _PUTS("FAIL\n");
}

#if MIU_NUMBERS > 1
static void _MIU_Bist1(void)
{
    MS_U16 u16Lo = (MIU_TEST_LENGTH >> MIU_BIST_LENGTH_UNIT) & 0xff;
    MS_U16 u16Hi = ((MIU_TEST_LENGTH >> MIU_BIST_LENGTH_UNIT) >> 16) & 0xff;

    WREG_DBG(0xA004, 2);
    _PUTS("BIST1_");
    WREG16((REG_ADDR_BASE + (0x100646 <<1 )) , 0x7FFE);//   Mask MIU1_group_0
   
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
    MS_U16 u16Lo = (MIU_TEST_LENGTH >> MIU_BIST_LENGTH_UNIT) & 0xff;
    MS_U16 u16Hi = ((MIU_TEST_LENGTH >> MIU_BIST_LENGTH_UNIT) >> 16) & 0xff;

    WREG_DBG(0xA002, 2);
    _PUTS("\nBIST0_");
    WREG16((REG_ADDR_BASE + (0x101246 <<1 )) , 0x7FFE);//   Mask MIU0_group_0
    
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
    WREG16((REG_ADDR_BASE  + (0x1012fe <<1 )) , 0x09E1);  // close miu BIST, 7f[11]  : reg_miu_test_off (MIU BIST use the same MIU client with TSP3)
}

#endif // CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE

#endif
static void _MIU0_InitCycle(void)
{
    MS_U16 u16CheckInit = 0;
    // MIU0 INIT Cycle    
    WREG16    ((REG_ADDR_BASE + (0x101200 <<1 )), 0x0000); // hold miu_0    
    WREG16    ((REG_ADDR_BASE + (0x101200 <<1 )), 0x0008); // DRAM reset    
    WREG16    ((REG_ADDR_BASE + (0x101200 <<1 )), 0x000c); // DRAM reset+CS    R
    DelayMS(1); //DELAYUS 1000  @ delay 1ms
    WREG16    ((REG_ADDR_BASE + (0x101200 <<1 )), 0x000e); // DRAM reset+CS+CKE    
    DelayMS(2);//DELAYUS 2000   @ delay 2ms    
    WREG16    ((REG_ADDR_BASE + (0x101200 <<1 )), 0x001f); //  cke to cmd > tXP
    u16CheckInit = RREG16(REG_ADDR_BASE + (0x101200 <<1 ));
    _PUTS("\nWait MIU0...\n");
    while((u16CheckInit >> 15) == 0)
    {
        u16CheckInit = RREG16(REG_ADDR_BASE + (0x101200 <<1 ));
    }
    _PUTS("MIU0 Init Done \n");
    DelayMS(2);//DELAYUS 2000   @ delay 2ms    
    WREG16   ((REG_ADDR_BASE + (0x101200 <<1 )), 0x001f); // cke to cmd > tXPR
}

static void _MIU1_InitCycle(void)
{
    MS_U16 u16CheckInit = 0;
    // MIU0 INIT Cycle    
    WREG16    ((REG_ADDR_BASE + (0x100600 <<1 )), 0x0000); // hold miu_1 
    WREG16    ((REG_ADDR_BASE + (0x100600 <<1 )), 0x0008); // DRAM reset    
    WREG16    ((REG_ADDR_BASE + (0x100600 <<1 )), 0x000c); // DRAM reset+CS    R
    DelayMS(1); //DELAYUS 1000  @ delay 1ms
    WREG16    ((REG_ADDR_BASE + (0x100600 <<1 )), 0x000e); // DRAM reset+CS+CKE    
    DelayMS(2);//DELAYUS 2000   @ delay 2ms    
    WREG16    ((REG_ADDR_BASE + (0x100600 <<1 )), 0x001f); //  cke to cmd > tXP
    u16CheckInit = RREG16(REG_ADDR_BASE + (0x100600 <<1 ));
    _PUTS("\nWait MIU1...\n");
    while((u16CheckInit >> 15) == 0)
    {
        u16CheckInit = RREG16(REG_ADDR_BASE + (0x100600 <<1 ));
    }
    _PUTS("MIU1 Init Done \n");
    DelayMS(2);//DELAYUS 2000   @ delay 2ms    
    WREG16   ((REG_ADDR_BASE + (0x100600 <<1 )), 0x001f); // cke to cmd > tXPRf
}

static void _MIU_InitPriority(void)
{
    MS_U16 u16Mask = 0, u16write = 0,u16RegMask = 0;
    u16Mask = RREG16(REG_ADDR_BASE + (0x101240 <<1 ));
    u16Mask = u16Mask & 0x0001;

    if (u16Mask != 0x0001)
    {
        u16RegMask = 0x0002;
        u16write = RREG16(REG_ADDR_BASE + (0x101240 <<1 ));
        u16write = u16write | u16RegMask;
        WREG16    ((REG_ADDR_BASE + (0x101240 <<1 )), u16write); 

        u16write = RREG16(REG_ADDR_BASE + (0x101240 <<1 ));
        u16write = u16write & (!u16RegMask);
        WREG16    ((REG_ADDR_BASE + (0x101240 <<1 )), u16write); 
    }
    //miu0_rq0_skip

    u16Mask = RREG16(REG_ADDR_BASE + (0x101260 <<1 ));
    u16Mask = u16Mask & 0x0001;
    if (u16Mask != 0x0001)
    {
        u16RegMask = 0x0002;
        u16write = RREG16(REG_ADDR_BASE + (0x101260 <<1 ));
        u16write = u16write | u16RegMask;
        WREG16    ((REG_ADDR_BASE + (0x101260 <<1 )), u16write); 

        u16write = RREG16(REG_ADDR_BASE + (0x101260 <<1 ));
        u16write = u16write & (!u16RegMask);
        WREG16    ((REG_ADDR_BASE + (0x101260 <<1 )), u16write); 
    }
    //miu0_rq1_skip
    u16Mask = RREG16(REG_ADDR_BASE + (0x101280 <<1 ));
    u16Mask = u16Mask & 0x0001;
    if (u16Mask != 0x0001)
    {
        u16RegMask = 0x0002;
        u16write = RREG16(REG_ADDR_BASE + (0x101280 <<1 ));
        u16write = u16write | u16RegMask;
        WREG16    ((REG_ADDR_BASE + (0x101280 <<1 )), u16write); 

        u16write = RREG16(REG_ADDR_BASE + (0x101280 <<1 ));
        u16write = u16write & (!u16RegMask);
        WREG16    ((REG_ADDR_BASE + (0x101280 <<1 )), u16write); 
    }
    //miu0_rq2_skip
    u16Mask = RREG16(REG_ADDR_BASE + (0x1012A0 <<1 ));
    u16Mask = u16Mask & 0x0001;
    if (u16Mask != 0x0001)
    {
        u16RegMask = 0x0002;
        u16write = RREG16(REG_ADDR_BASE + (0x1012A0 <<1 ));
        u16write = u16write | u16RegMask;
        WREG16    ((REG_ADDR_BASE + (0x1012A0 <<1 )), u16write); 

        u16write = RREG16(REG_ADDR_BASE + (0x1012A0 <<1 ));
        u16write = u16write & (!u16RegMask);
        WREG16    ((REG_ADDR_BASE + (0x1012A0 <<1 )), u16write); 
    }
    //miu0_rq3_skip
#if MIU_NUMBERS > 1

    u16Mask = RREG16(REG_ADDR_BASE + (0x100640 <<1 ));
    u16Mask = u16Mask & 0x0001;

    if (u16Mask != 0x0001)
    {
        u16RegMask = 0x0002;
        u16write = RREG16(REG_ADDR_BASE + (0x100640 <<1 ));
        u16write = u16write | u16RegMask;
        WREG16    ((REG_ADDR_BASE + (0x100640 <<1 )), u16write); 

        u16write = RREG16(REG_ADDR_BASE + (0x100640 <<1 ));
        u16write = u16write & (!u16RegMask);
        WREG16    ((REG_ADDR_BASE + (0x100640 <<1 )), u16write); 
    }
    //miu1_rq0_skip

    u16Mask = RREG16(REG_ADDR_BASE + (0x100660 <<1 ));
    u16Mask = u16Mask & 0x0001;
    if (u16Mask != 0x0001)
    {
        u16RegMask = 0x0002;
        u16write = RREG16(REG_ADDR_BASE + (0x100660 <<1 ));
        u16write = u16write | u16RegMask;
        WREG16    ((REG_ADDR_BASE + (0x100660 <<1 )), u16write); 

        u16write = RREG16(REG_ADDR_BASE + (0x100660 <<1 ));
        u16write = u16write & (!u16RegMask);
        WREG16    ((REG_ADDR_BASE + (0x100660 <<1 )), u16write); 
    }
    //miu1_rq1_skip
    u16Mask = RREG16(REG_ADDR_BASE + (0x100680 <<1 ));
    u16Mask = u16Mask & 0x0001;
    if (u16Mask != 0x0001)
    {
        u16RegMask = 0x0002;
        u16write = RREG16(REG_ADDR_BASE + (0x100680 <<1 ));
        u16write = u16write | u16RegMask;
        WREG16    ((REG_ADDR_BASE + (0x100680 <<1 )), u16write); 

        u16write = RREG16(REG_ADDR_BASE + (0x100680 <<1 ));
        u16write = u16write & (!u16RegMask);
        WREG16    ((REG_ADDR_BASE + (0x100680 <<1 )), u16write); 
    }
    //miu1_rq2_skip
    u16Mask = RREG16(REG_ADDR_BASE + (0x1006A0 <<1 ));
    u16Mask = u16Mask & 0x0001;
    if (u16Mask != 0x0001)
    {
        u16RegMask = 0x0002;
        u16write = RREG16(REG_ADDR_BASE + (0x1006A0 <<1 ));
        u16write = u16write | u16RegMask;
        WREG16    ((REG_ADDR_BASE + (0x1006A0 <<1 )), u16write); 

        u16write = RREG16(REG_ADDR_BASE + (0x1006A0 <<1 ));
        u16write = u16write & (!u16RegMask);
        WREG16    ((REG_ADDR_BASE + (0x1006A0 <<1 )), u16write); 
    }
    //miu1_rq3_skip
#endif
     _PUTS("MIU Priority Init Done \n");
}
static void _MIU_ClearIPMask(void)
{
    // open all MIU0 request mask (All IP can access MIU resource)  
    WREG16((REG_ADDR_BASE + (0x101246 <<1 )) , 0x0000);   //Mask MIU0_group_0  
    WREG16((REG_ADDR_BASE + (0x101266 <<1 )) , 0x0000);   //Mask MIU0_group_1   
    WREG16((REG_ADDR_BASE + (0x101286 <<1 )) , 0x0000);   //Mask MIU0_group_2    
    WREG16((REG_ADDR_BASE + (0x1012a6 <<1 )) , 0x0000);   // Mask MIU0_group_3
#if MIU_NUMBERS > 1
    // open all MIU1 request mask (All IP can access MIU resource)   
    WREG16((REG_ADDR_BASE + (0x100646 <<1 )) , 0x0000);  //Mask MIU0_group_0    
    WREG16((REG_ADDR_BASE + (0x100666 <<1 )) , 0x0000);  //Mask MIU0_group_1    
    WREG16((REG_ADDR_BASE + (0x100686 <<1 )) , 0x0000);  //Mask MIU0_group_2  
    WREG16((REG_ADDR_BASE + (0x1006a6 <<1 )) , 0x0000);  //Mask MIU0_group_3   
    // all MIU1 IP select MIU0 in bringup    
    WREG16((REG_ADDR_BASE + (0x1006f0 <<1 )), 0x0000);
    WREG16((REG_ADDR_BASE + (0x1006f2 <<1 )), 0x0000);
    WREG16((REG_ADDR_BASE + (0x1006f4 <<1 )), 0x0000);  
    WREG16((REG_ADDR_BASE + (0x1006f6 <<1 )), 0x0000);
#endif
    WREG16((REG_ADDR_BASE + (0x10121e <<1 )), 0x0c08);  // SW initial done and turn on arbitor     
    // WREG    (MS_BUS_RIU_ADDR + (0x1012fc <<1 )), 0x8400
 #if MIU_NUMBERS > 1 
    WREG16((REG_ADDR_BASE + (0x10061e <<1 )), 0x0c08);  // SW initial done and turn on arbitor     
    // WREG    (MS_BUS_RIU_ADDR + (0x1006fc <<1 )), 0x8400
#endif

}

static void _InitParam_End(MS_U8 u8POWER_Flag)
{
    if (u8POWER_Flag == AC_ON)
    {
#if CONFIG_MSTAR_STR_ENABLE
        MS_U16 Temp;
        Temp = RREG16(REG_ADDR_BASE + (0x110D00 << 1));
        WREG16(REG_ADDR_BASE + (0x110D00 << 1), Temp & (~0x0008));
#if MIU_NUMBERS > 1   
        Temp = RREG16(REG_ADDR_BASE + (0x110D80 << 1));
        WREG16(REG_ADDR_BASE + (0x110D80 << 1), Temp & (~0x0008)); 
#endif
        _Disable_ATOP_SW();
#endif
    }
    
    _MIU0_InitCycle();
#if MIU_NUMBERS > 1
    _MIU1_InitCycle();
#endif

#if CONFIG_MSTAR_STR_ENABLE
    if (u8POWER_Flag == DC_ON)
    {  
        //--------------- STR ---------------//
        _Enter_Refresh_Mode();
        _Disable_ATOP_SW();
        _Exit_Refresh_Mode();
        //--------------- STR ---------------//
    }
#endif
    
    _MIU_InitPriority();

    if (u8POWER_Flag == AC_ON) 
    {
#ifdef CONFIG_MSTAR_DRAM_AUTO_PHASE_ENABLE
        HAL_AutoPhaseScan();
#else
        _MIU_Bist();
#endif
    }
    
    _MIU_ClearIPMask();
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
    MS_U32 rbase = (0x1f20 << 16);
    MS_U32 width = 16;
    MS_U8  u8POWER_Flag=0;
#if CONFIG_MSTAR_STR_ENABLE
    

    if ((RREG16(STR_IS_AC_FLOW_CONTROL) & 0xFF00) == 0xFF00)
    {
        u8POWER_Flag = AC_ON;
        _PUTS("SPI AC ON\n\r");
    }
    else 
    {
        u8POWER_Flag = DC_ON;
        _PUTS("SPI STR ON\n\r");
    }
#else
    u8POWER_Flag = AC_ON;
#endif

    gEntrys = pParams;
    //_PUTC('A');
    while(1)
    {
        const Entry *eCur = _Next();
        if(eCur == NULL)
            break;

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


            case CHIPCFG_CMD_AVD_RESET:
#if CONFIG_MSTAR_STR_ENABLE
                if (u8POWER_Flag == DC_ON)
                    _Reset_AVD();
#endif
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
   // _PUTC('B');
                
    _InitParam_End(u8POWER_Flag);
    WREG_DBG(0xF001, 1);
    return 1;
}


