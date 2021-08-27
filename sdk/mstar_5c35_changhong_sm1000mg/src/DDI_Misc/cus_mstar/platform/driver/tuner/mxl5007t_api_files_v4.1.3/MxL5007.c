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
 MxL5007 Source Code : V4.1.3

 Copyright, Maxlinear, Inc.
 All Rights Reserved

 File Name:      MxL5007.c

 Description: The source code is for MxL5007 user to quickly integrate MxL5007 into their software.
	There are two functions the user can call to generate a array of I2C command that's require to
	program the MxL5007 tuner. The user should pass an array pointer and an integer pointer in to the
	function. The funciton will fill up the array with format like follow:

		addr1
		data1
		addr2
		data2
		...

	The user can then pass this array to their I2C function to perform progromming the tuner.
*/
//#include "StdAfx.h"
#include "MsCommon.h"
#include "MsTypes2.h"
#include "Board.h"
#if IF_THIS_TUNER_INUSE(TUNER_MXL_MXL5007)
#include "MxL5007_Common.h"
#include "MxL5007.h"



MS_U32 MxL5007_Init(MS_U8* pArray,				// a array pointer that store the addr and data pairs for I2C write
					MS_U32* Array_Size,			// a integer pointer that store the number of element in above array
					MS_U8 Mode,
					SINT32 IF_Diff_Out_Level,
					MS_U32 Xtal_Freq_Hz,
					MS_U32 IF_Freq_Hz,
					MS_U8 Invert_IF,
					MS_U8 Clk_Out_Enable,
					MS_U8 Clk_Out_Amp
					)
{

	MS_U32 Reg_Index=0;
	MS_U32 Array_Index=0;

	IRVType IRV_Init[]=
	{
		//{ Addr, Data}
		{ 0x02, 0x06},
		{ 0x03, 0x48},
		{ 0x05, 0x04},
		{ 0x06, 0x10},
		{ 0x2E, 0x15},  //Override
		{ 0x30, 0x10},  //Override
		{ 0x45, 0x58},  //Override
		{ 0x48, 0x19},  //Override
		{ 0x52, 0x03},  //Override
		{ 0x53, 0x44},  //Override
		{ 0x6A, 0x4B},  //Override
		{ 0x76, 0x00},  //Override
		{ 0x78, 0x18},  //Override
		{ 0x7A, 0x17},  //Override
		{ 0x85, 0x06},  //Override
		{ 0x01, 0x01}, //TOP_MASTER_ENABLE=1
		{ 0, 0}
	};


	IRVType IRV_Init_Cable[]=
	{
		//{ Addr, Data}
		{ 0x02, 0x06},
		{ 0x03, 0x48},
		{ 0x05, 0x04},
		{ 0x06, 0x10},
		{ 0x09, 0x3F},
		{ 0x0A, 0x3F},
		{ 0x0B, 0x3F},
		{ 0x2E, 0x15},  //Override
		{ 0x30, 0x10},  //Override
		{ 0x45, 0x58},  //Override
		{ 0x48, 0x19},  //Override
		{ 0x52, 0x03},  //Override
		{ 0x53, 0x44},  //Override
		{ 0x6A, 0x4B},  //Override
		{ 0x76, 0x00},  //Override
		{ 0x78, 0x18},  //Override
		{ 0x7A, 0x17},  //Override
		{ 0x85, 0x06},  //Override
		{ 0x01, 0x01}, //TOP_MASTER_ENABLE=1
		{ 0, 0}
	};
	//edit Init setting here

	PIRVType myIRV=IRV_Init;

	switch(Mode)
	{
	case MxL_MODE_ISDBT: //ISDB-T Mode
		myIRV = IRV_Init;
		SetIRVBit(myIRV, 0x06, 0x1F, 0x10);
		break;
	case MxL_MODE_DVBT: //DVBT Mode
		myIRV = IRV_Init;
		SetIRVBit(myIRV, 0x06, 0x1F, 0x11);
		break;
	case MxL_MODE_ATSC: //ATSC Mode
		myIRV = IRV_Init;
		SetIRVBit(myIRV, 0x06, 0x1F, 0x12);
		break;
	case MxL_MODE_CABLE:
		myIRV = IRV_Init_Cable;
		SetIRVBit(myIRV, 0x09, 0xFF, 0xC1);
		SetIRVBit(myIRV, 0x0A, 0xFF, 8-IF_Diff_Out_Level);
		SetIRVBit(myIRV, 0x0B, 0xFF, 0x17);
		break;


	}

	switch(IF_Freq_Hz)
	{
	case MxL_IF_4_MHZ:
		SetIRVBit(myIRV, 0x02, 0x0F, 0x00);
		break;
	case MxL_IF_4_5_MHZ: //4.5MHz
		SetIRVBit(myIRV, 0x02, 0x0F, 0x02);
		break;
	case MxL_IF_4_57_MHZ: //4.57MHz
		SetIRVBit(myIRV, 0x02, 0x0F, 0x03);
		break;
	case MxL_IF_5_MHZ:
		SetIRVBit(myIRV, 0x02, 0x0F, 0x04);
		break;
	case MxL_IF_5_38_MHZ: //5.38MHz
		SetIRVBit(myIRV, 0x02, 0x0F, 0x05);
		break;
	case MxL_IF_6_MHZ:
		SetIRVBit(myIRV, 0x02, 0x0F, 0x06);
		break;
	case MxL_IF_6_28_MHZ: //6.28MHz
		SetIRVBit(myIRV, 0x02, 0x0F, 0x07);
		break;
	case MxL_IF_9_1915_MHZ: //9.1915MHz
		SetIRVBit(myIRV, 0x02, 0x0F, 0x08);
		break;
	case MxL_IF_35_25_MHZ:
		SetIRVBit(myIRV, 0x02, 0x0F, 0x09);
		break;
	case MxL_IF_36_15_MHZ:
		SetIRVBit(myIRV, 0x02, 0x0F, 0x0a);
		break;
	case MxL_IF_44_MHZ:
		SetIRVBit(myIRV, 0x02, 0x0F, 0x0B);
		break;
	}

	if(Invert_IF)
		SetIRVBit(myIRV, 0x02, 0x10, 0x10);   //Invert IF
	else
		SetIRVBit(myIRV, 0x02, 0x10, 0x00);   //Normal IF


	switch(Xtal_Freq_Hz)
	{
	case MxL_XTAL_16_MHZ:
		SetIRVBit(myIRV, 0x03, 0xF0, 0x00);  //select xtal freq & Ref Freq
		SetIRVBit(myIRV, 0x05, 0x0F, 0x00);
		break;
	case MxL_XTAL_20_MHZ:
		SetIRVBit(myIRV, 0x03, 0xF0, 0x10);
		SetIRVBit(myIRV, 0x05, 0x0F, 0x01);
		break;
	case MxL_XTAL_20_25_MHZ:
		SetIRVBit(myIRV, 0x03, 0xF0, 0x20);
		SetIRVBit(myIRV, 0x05, 0x0F, 0x02);
		break;
	case MxL_XTAL_20_48_MHZ:
		SetIRVBit(myIRV, 0x03, 0xF0, 0x30);
		SetIRVBit(myIRV, 0x05, 0x0F, 0x03);
		break;
	case MxL_XTAL_24_MHZ:
		SetIRVBit(myIRV, 0x03, 0xF0, 0x40);
		SetIRVBit(myIRV, 0x05, 0x0F, 0x04);
		break;
	case MxL_XTAL_25_MHZ:
		SetIRVBit(myIRV, 0x03, 0xF0, 0x50);
		SetIRVBit(myIRV, 0x05, 0x0F, 0x05);
		break;
	case MxL_XTAL_25_14_MHZ:
		SetIRVBit(myIRV, 0x03, 0xF0, 0x60);
		SetIRVBit(myIRV, 0x05, 0x0F, 0x06);
		break;
	case MxL_XTAL_27_MHZ:
		SetIRVBit(myIRV, 0x03, 0xF0, 0x70);
		SetIRVBit(myIRV, 0x05, 0x0F, 0x07);
		break;
	case MxL_XTAL_28_8_MHZ:
		SetIRVBit(myIRV, 0x03, 0xF0, 0x80);
		SetIRVBit(myIRV, 0x05, 0x0F, 0x08);
		break;
	case MxL_XTAL_32_MHZ:
		SetIRVBit(myIRV, 0x03, 0xF0, 0x90);
		SetIRVBit(myIRV, 0x05, 0x0F, 0x09);
		break;
	case MxL_XTAL_40_MHZ:
		SetIRVBit(myIRV, 0x03, 0xF0, 0xA0);
		SetIRVBit(myIRV, 0x05, 0x0F, 0x0A);
		break;
	case MxL_XTAL_44_MHZ:
		SetIRVBit(myIRV, 0x03, 0xF0, 0xB0);
		SetIRVBit(myIRV, 0x05, 0x0F, 0x0B);
		break;
	case MxL_XTAL_48_MHZ:
		SetIRVBit(myIRV, 0x03, 0xF0, 0xC0);
		SetIRVBit(myIRV, 0x05, 0x0F, 0x0C);
		break;
	case MxL_XTAL_49_3811_MHZ:
		SetIRVBit(myIRV, 0x03, 0xF0, 0xD0);
		SetIRVBit(myIRV, 0x05, 0x0F, 0x0D);
		break;
	}

	if(!Clk_Out_Enable) //default is enable
		SetIRVBit(myIRV, 0x03, 0x08, 0x00);

	//Clk_Out_Amp
	SetIRVBit(myIRV, 0x03, 0x07, Clk_Out_Amp);

	//Generate one Array that Contain Data, Address
	while (myIRV[Reg_Index].Num || myIRV[Reg_Index].Val)
	{
		pArray[Array_Index++] = myIRV[Reg_Index].Num;
		pArray[Array_Index++] = myIRV[Reg_Index].Val;
		Reg_Index++;
	}

	*Array_Size=Array_Index;
	return 0;
}


MS_U32 MxL5007_RFTune(MS_U8* pArray, MS_U32* Array_Size, MS_U32 RF_Freq, MS_U8 BWMHz)
{
	IRVType IRV_RFTune[]=
	{
	//{ Addr, Data}
		{ 0x0F, 0x00},  //abort tune
		{ 0x0C, 0x15},
		{ 0x0D, 0x40},
		{ 0x0E, 0x0E},
		{ 0x1F, 0x87},  //Override
		{ 0x20, 0x1F},  //Override
		{ 0x21, 0x87},  //Override
		{ 0x22, 0x1F},  //Override
		{ 0x80, 0x01},  //Freq Dependent Setting
		{ 0x0F, 0x01},	//start tune
		{ 0, 0}
	};

	MS_U32 dig_rf_freq=0;
	MS_U32 temp;
	MS_U32 Reg_Index=0;
	MS_U32 Array_Index=0;
	MS_U32 i;
	MS_U32 frac_divider = 1000000;

	switch(BWMHz)
	{
	case MxL_BW_6MHz: //6MHz
			SetIRVBit(IRV_RFTune, 0x0C, 0x3F, 0x15);  //set DIG_MODEINDEX, DIG_MODEINDEX_A, and DIG_MODEINDEX_CSF
		break;
	case MxL_BW_7MHz: //7MHz
			SetIRVBit(IRV_RFTune, 0x0C, 0x3F, 0x2A);
		break;
	case MxL_BW_8MHz: //8MHz
			SetIRVBit(IRV_RFTune, 0x0C, 0x3F, 0x3F);
		break;
	}


	//Convert RF frequency into 16 bits => 10 bit integer (MHz) + 6 bit fraction
	dig_rf_freq = RF_Freq / MHz;
	temp = RF_Freq % MHz;
	for(i=0; i<6; i++)
	{
		dig_rf_freq <<= 1;
		frac_divider /=2;
		if(temp > frac_divider)
		{
			temp -= frac_divider;
			dig_rf_freq++;
		}
	}

	//add to have shift center point by 7.8124 kHz
	if(temp > 7812)
		dig_rf_freq ++;

	SetIRVBit(IRV_RFTune, 0x0D, 0xFF, (MS_U8)dig_rf_freq);
	SetIRVBit(IRV_RFTune, 0x0E, 0xFF, (MS_U8)(dig_rf_freq>>8));

	if (RF_Freq >=333*MHz)
		SetIRVBit(IRV_RFTune, 0x80, 0x40, 0x40);

	//Generate one Array that Contain Data, Address
	while (IRV_RFTune[Reg_Index].Num || IRV_RFTune[Reg_Index].Val)
	{
		pArray[Array_Index++] = IRV_RFTune[Reg_Index].Num;
		pArray[Array_Index++] = IRV_RFTune[Reg_Index].Val;
		Reg_Index++;
	}

	*Array_Size=Array_Index;

	return 0;
}

//local functions called by Init and RFTune
MS_U32 SetIRVBit(PIRVType pIRV, MS_U8 Num, MS_U8 Mask, MS_U8 Val)
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
	return 0;
}
#endif

