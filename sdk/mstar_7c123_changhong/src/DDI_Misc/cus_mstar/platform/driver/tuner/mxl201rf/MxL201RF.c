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
/*
 MxL201RF Source Code : V6.2.5.0

 Copyright, Maxlinear, Inc.
 All Rights Reserved

 File Name:      MxL201RF.c

 Description: The source code is for MxL201RF user to quickly integrate MxL201RF into their software.
    There are two functions the user can call to generate a array of I2C command that's require to
    program the MxL201RF tuner. The user should pass an array pointer and an integer pointer in to the
    function. The funciton will fill up the array with format like follow:

        addr1
        data1
        addr2
        data2
        ...

    The user can then pass this array to their I2C function to perform progromming the tuner.
*/
/* #include "StdAfx.h" */


#include "MsCommon.h"
#include "MsTypes2.h"
#include "Board.h"
#if IF_THIS_TUNER_INUSE(TUNER_MXL201RF)
#include "MxL201RF_Common.h"
#include "MxL201RF.h"



UINT32 MxL201RF_Init(UINT8* pArray,                /* a array pointer that store the addr and data pairs for I2C write */
                    UINT32* Array_Size,            /* a integer pointer that store the number of element in above array */
                    UINT8 Mode,
                    UINT32 Xtal_Freq_Hz,
                    UINT32 IF_Freq_Hz,
                    UINT8 Invert_IF,
                    UINT8 Clk_Out_Enable,
                    UINT8 Clk_Out_Amp,
                    UINT8 Xtal_Cap
                    )
{

    UINT32 Reg_Index=0;
    UINT32 Array_Index=0;

    /* Terrestial register settings */
    IRVType IRV_Init[]=
    {
        /* { Addr, Data} */
        { 0x02, 0x06},
        { 0x03, 0x1A},
        { 0x04, 0x14},
        { 0x05, 0x0E},
        { 0x0C, 0x00},
        { 0x07, 0x01},
        { 0x93, 0xD7},
        { 0x95, 0x61},
        { 0xA2, 0x00},
        { 0x2F, 0x00},
        { 0x60, 0x60},
        { 0x70, 0x00},
        { 0xB9, 0x10},
        { 0x8E, 0x57},
        { 0x58, 0x08},
        { 0x5C, 0x00},
        { 0x01, 0x01}, /*TOP_MASTER_ENABLE=1 */
        { 0, 0}
    };

    IRVType IRV_Init_Cable[]=
    {
        /*{ Addr, Data}     */
        { 0x02, 0x06},
        { 0x03, 0x1A},
        { 0x04, 0x14},
        { 0x05, 0x0E},
        { 0x0C, 0x00},
        { 0x07, 0x14},
        { 0x29, 0x03},
        { 0x45, 0x01},
        { 0x7A, 0xCF},
        { 0x7C, 0x7C},
        { 0x7E, 0x27},
        { 0x93, 0xD7},
        { 0x99, 0x40},
        { 0x2F, 0x00},
        { 0x60, 0x60},
        { 0x70, 0x00},
        { 0xB9, 0x10},
        { 0x8E, 0x57},
        { 0x58, 0x08},
        { 0x5C, 0x00},
        { 0x48, 0x37},
        { 0x55, 0x06},
        { 0x01, 0x01}, /*TOP_MASTER_ENABLE=1 */
        { 0, 0}
    };
    /*edit Init setting here */

    PIRVType myIRV = NULL;

    switch(Mode)
    {

    case MxL_MODE_DVBT: /*DVBT Mode    */
        myIRV = IRV_Init;
        SetIRVBit(myIRV, 0x07, 0x1F, 0x01);
        SetIRVBit(myIRV, 0xA2, 0xFF, 0x02);
        break;
    case MxL_MODE_ATSC: /*ATSC Mode    */
        myIRV = IRV_Init;
        SetIRVBit(myIRV, 0x07, 0x1F, 0x02);
        break;
    case MxL_MODE_CAB_OPT1: /*Cable Option 1 Mode */
        myIRV = IRV_Init_Cable;
        SetIRVBit(myIRV, 0x29, 0xFF, 0x09);
        SetIRVBit(myIRV, 0x45, 0xFF, 0x01);
        SetIRVBit(myIRV, 0x7A, 0xFF, 0x6F);
        SetIRVBit(myIRV, 0x7C, 0xFF, 0x1C);
        SetIRVBit(myIRV, 0x7E, 0xFF, 0x5A);
        SetIRVBit(myIRV, 0x93, 0xFF, 0xF7);
        break;
    case MxL_MODE_CAB_STD: /*Cable Standard Mode */
        myIRV = IRV_Init_Cable;
        SetIRVBit(myIRV, 0x45, 0xFF, 0x01);
        SetIRVBit(myIRV, 0x7A, 0xFF, 0x6F);
        SetIRVBit(myIRV, 0x7C, 0xFF, 0x1C);
        SetIRVBit(myIRV, 0x7E, 0xFF, 0x7C);
        SetIRVBit(myIRV, 0x93, 0xFF, 0xE7);
        break;
    default:
        return MxL_ERR_INIT;
    }

    switch(IF_Freq_Hz)
    {
    case MxL_IF_4_MHZ:
        SetIRVBit(myIRV, 0x02, 0x0F, 0x01);
        break;
    case MxL_IF_4_5_MHZ:
        SetIRVBit(myIRV, 0x02, 0x0F, 0x02);
        break;
    case MxL_IF_4_57_MHZ:
        SetIRVBit(myIRV, 0x02, 0x0F, 0x03);
        break;
    case MxL_IF_5_MHZ:
        SetIRVBit(myIRV, 0x02, 0x0F, 0x04);
        break;
    case MxL_IF_5_38_MHZ:
        SetIRVBit(myIRV, 0x02, 0x0F, 0x05);
        break;
    case MxL_IF_6_MHZ:
        SetIRVBit(myIRV, 0x02, 0x0F, 0x06);
        break;
    case MxL_IF_6_28_MHZ:
        SetIRVBit(myIRV, 0x02, 0x0F, 0x07);
        break;
    case MxL_IF_7_2_MHZ:
        SetIRVBit(myIRV, 0x02, 0x0F, 0x08);
        break;
    case MxL_IF_35_25_MHZ:
        SetIRVBit(myIRV, 0x02, 0x0F, 0x09);
        break;
    case MxL_IF_36_MHZ:
        SetIRVBit(myIRV, 0x02, 0x0F, 0x0A);
        break;
    case MxL_IF_36_15_MHZ:
        SetIRVBit(myIRV, 0x02, 0x0F, 0x0B);
        break;
    case MxL_IF_44_MHZ:
        SetIRVBit(myIRV, 0x02, 0x0F, 0x0C);
        break;
    default:
        return MxL_ERR_IF_FREQ;
    }


    if(Invert_IF)
        SetIRVBit(myIRV, 0x02, 0x10, 0x10);   /*Invert IF*/
    else
        SetIRVBit(myIRV, 0x02, 0x10, 0x00);   /*Normal IF*/


    switch(Xtal_Freq_Hz)
    {
    case MxL_XTAL_16_MHZ:
        SetIRVBit(myIRV, 0x04, 0x0F, 0x00);
        SetIRVBit(myIRV, 0x58, 0x03, 0x03);
        SetIRVBit(myIRV, 0x5C, 0xFF, 0x36);
        break;
    case MxL_XTAL_20_MHZ:
        SetIRVBit(myIRV, 0x04, 0x0F, 0x01);
        SetIRVBit(myIRV, 0x58, 0x03, 0x03);
        SetIRVBit(myIRV, 0x5C, 0xFF, 0x2B);
        break;
    case MxL_XTAL_20_25_MHZ:
        SetIRVBit(myIRV, 0x04, 0x0F, 0x02);
        SetIRVBit(myIRV, 0x58, 0x03, 0x03);
        SetIRVBit(myIRV, 0x5C, 0xFF, 0x2A);
        break;
    case MxL_XTAL_20_48_MHZ:
        SetIRVBit(myIRV, 0x04, 0x0F, 0x03);
        SetIRVBit(myIRV, 0x58, 0x03, 0x03);
        SetIRVBit(myIRV, 0x5C, 0xFF, 0x2A);
        break;
    case MxL_XTAL_24_MHZ:
        SetIRVBit(myIRV, 0x04, 0x0F, 0x04);
        SetIRVBit(myIRV, 0x58, 0x03, 0x00);
        SetIRVBit(myIRV, 0x5C, 0xFF, 0x48);
        break;
    case MxL_XTAL_25_MHZ:
        SetIRVBit(myIRV, 0x04, 0x0F, 0x05);
        SetIRVBit(myIRV, 0x58, 0x03, 0x00);
        SetIRVBit(myIRV, 0x5C, 0xFF, 0x45);
        break;
    case MxL_XTAL_25_14_MHZ:
        SetIRVBit(myIRV, 0x04, 0x0F, 0x06);
        SetIRVBit(myIRV, 0x58, 0x03, 0x00);
        SetIRVBit(myIRV, 0x5C, 0xFF, 0x44);
        break;
    case MxL_XTAL_28_8_MHZ:
        SetIRVBit(myIRV, 0x04, 0x0F, 0x08);
        SetIRVBit(myIRV, 0x58, 0x03, 0x00);
        SetIRVBit(myIRV, 0x5C, 0xFF, 0x3C);
        break;
    case MxL_XTAL_32_MHZ:
        SetIRVBit(myIRV, 0x04, 0x0F, 0x09);
        SetIRVBit(myIRV, 0x58, 0x03, 0x00);
        SetIRVBit(myIRV, 0x5C, 0xFF, 0x36);
        break;
    case MxL_XTAL_40_MHZ:
        SetIRVBit(myIRV, 0x04, 0x0F, 0x0A);
        SetIRVBit(myIRV, 0x58, 0x03, 0x00);
        SetIRVBit(myIRV, 0x5C, 0xFF, 0x2B);
        break;
    case MxL_XTAL_44_MHZ:
        SetIRVBit(myIRV, 0x04, 0x0F, 0x0B);
        SetIRVBit(myIRV, 0x58, 0x03, 0x02);
        SetIRVBit(myIRV, 0x5C, 0xFF, 0x4E);
        break;
    case MxL_XTAL_48_MHZ:
        SetIRVBit(myIRV, 0x04, 0x0F, 0x0C);
        SetIRVBit(myIRV, 0x58, 0x03, 0x02);
        SetIRVBit(myIRV, 0x5C, 0xFF, 0x48);
        break;
    case MxL_XTAL_49_3811_MHZ:
        SetIRVBit(myIRV, 0x04, 0x0F, 0x0D);
        SetIRVBit(myIRV, 0x58, 0x03, 0x02);
        SetIRVBit(myIRV, 0x5C, 0xFF, 0x45);
        break;
    default:
        return MxL_ERR_XTAL_FREQ;
    }

    if(!Clk_Out_Enable) /*default is enable  */
        SetIRVBit(myIRV, 0x03, 0x10, 0x00);

    /* Clk_Out_Amp */
    SetIRVBit(myIRV, 0x03, 0x0F, Clk_Out_Amp);

    /* Xtal Capacitor */
    if (Xtal_Cap >0 && Xtal_Cap <=25)
        SetIRVBit(myIRV, 0x05, 0xFF, Xtal_Cap);
    else if (Xtal_Cap == 0)
        SetIRVBit(myIRV, 0x05, 0xFF, 0x3F);
    else
        return MxL_ERR_INIT;


    /* Generate one Array that Contain Data, Address */
    while (myIRV[Reg_Index].Num || myIRV[Reg_Index].Val)
    {
        pArray[Array_Index++] = myIRV[Reg_Index].Num;
        pArray[Array_Index++] = myIRV[Reg_Index].Val;
        Reg_Index++;
    }

    *Array_Size=Array_Index;
    return MxL_OK;
}


UINT32 MxL201RF_RFTune(UINT8* pArray,
                       UINT32* Array_Size,
                       UINT32 RF_Freq,
                       UINT8 BWMHz,
                       UINT8 Mode
                       )

{
    IRVType IRV_RFTune[]=
    {
    /*{ Addr, Data} */
        { 0x10, 0x00},  /*abort tune*/
        { 0x0D, 0x15},
        { 0x0E, 0x40},
        { 0x0F, 0x0E},
        { 0xAB, 0x10},
        { 0x91, 0x00},
        { 0, 0}
    };

    UINT32 dig_rf_freq=0;
    UINT32 rf_freq_MHz = 0;
    UINT32 temp = 0 ;
    UINT32 Reg_Index=0;
    UINT32 Array_Index=0;
    UINT32 i = 0;
    UINT32 frac_divider = 1000000;

    rf_freq_MHz = RF_Freq/MHz;

    switch(Mode)
    {
        case MxL_MODE_CAB_OPT1: /* CABLE */
        case MxL_MODE_DVBT:        /* DVB-T */
            switch(BWMHz)
            {
                case MxL_BW_6MHz:
                    SetIRVBit(IRV_RFTune, 0x0D, 0xFF, 0x95);
                break;
                case MxL_BW_7MHz:
                    SetIRVBit(IRV_RFTune, 0x0D, 0xFF, 0xAA);
                break;
                case MxL_BW_8MHz:
                    SetIRVBit(IRV_RFTune, 0x0D, 0xFF, 0xBF);
                break;
                default:
                    return MxL_ERR_RFTUNE;
            }
        break;

        case MxL_MODE_ATSC: /*ATSC */
            SetIRVBit(IRV_RFTune, 0x0D, 0xFF, 0x99);
        break;

        case MxL_MODE_CAB_STD:    /*CABLE */
            switch(BWMHz)
            {
                case MxL_BW_6MHz:
                    SetIRVBit(IRV_RFTune, 0x0D, 0xFF, 0x49);
                break;
                case MxL_BW_7MHz:
                    SetIRVBit(IRV_RFTune, 0x0D, 0xFF, 0x5A);
                break;
                case MxL_BW_8MHz:
                    SetIRVBit(IRV_RFTune, 0x0D, 0xFF, 0x6F);
                break;
                default:
                    return MxL_ERR_RFTUNE;
            }
        break;

        default:
            return MxL_ERR_RFTUNE;
    }

    /*Convert RF frequency into 16 bits => 10 bit integer (MHz) + 6 bit fraction */
    dig_rf_freq = RF_Freq / MHz; /*Whole number portion of RF freq (in MHz) */
    temp = RF_Freq % MHz; /*Decimal portion of RF freq (in MHz) */
    for(i=0; i<6; i++)
    {
        dig_rf_freq <<= 1;
        frac_divider /=2;
        if(temp > frac_divider) /* Carryover from decimal */
        {
            temp -= frac_divider;
            dig_rf_freq++;
        }
    }

    /*add to have shift center point by 7.8124 kHz */
    if(temp > 7812)
        dig_rf_freq ++;

    SetIRVBit(IRV_RFTune, 0x0E, 0xFF, (UINT8)dig_rf_freq);
    SetIRVBit(IRV_RFTune, 0x0F, 0xFF, (UINT8)(dig_rf_freq>>8));

    /* Updated on 2010-0508 for V6.2.5*/
    if(Mode == MxL_MODE_CAB_STD || Mode == MxL_MODE_CAB_OPT1)
    {
        if(RF_Freq < 333000000)
            SetIRVBit(IRV_RFTune, 0xAB, 0xFF, 0x70);
        else if(RF_Freq < 667000000)
            SetIRVBit(IRV_RFTune, 0xAB, 0xFF, 0x20);
        else
            SetIRVBit(IRV_RFTune, 0xAB, 0xFF, 0x10);
    }
    else    /* If Terrestrial modes ... */
    {
        if(RF_Freq < 444000000)
            SetIRVBit(IRV_RFTune, 0xAB, 0xFF, 0x70);
        else if(RF_Freq < 667000000)
            SetIRVBit(IRV_RFTune, 0xAB, 0xFF, 0x20);
        else
            SetIRVBit(IRV_RFTune, 0xAB, 0xFF, 0x10);
    }


    if (RF_Freq <= 334000000)
        SetIRVBit(IRV_RFTune, 0x91, 0x40, 0x40);
    else
        SetIRVBit(IRV_RFTune, 0x91, 0x40, 0x00);


    /*Generate one Array that Contain Data, Address  */
    while (IRV_RFTune[Reg_Index].Num || IRV_RFTune[Reg_Index].Val)
    {
        pArray[Array_Index++] = IRV_RFTune[Reg_Index].Num;
        pArray[Array_Index++] = IRV_RFTune[Reg_Index].Val;
        Reg_Index++;
    }

    *Array_Size=Array_Index;

    return MxL_OK;
}

/* ========================== Local functions called by Init and RFTune ============================ */
UINT32 SetIRVBit(PIRVType pIRV, UINT8 Num, UINT8 Mask, UINT8 Val)
{
    while (pIRV->Num || pIRV->Val)
    {
        if (pIRV->Num==Num)
        {
            pIRV->Val&=~Mask;
            pIRV->Val|=Val;
        }
        pIRV++;

    }
    return MxL_OK;
}
#endif

