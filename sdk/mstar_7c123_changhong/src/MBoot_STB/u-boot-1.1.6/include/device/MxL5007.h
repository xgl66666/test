/*

 Driver APIs for MxL5007 Tuner

 Copyright, Maxlinear, Inc.
 All Rights Reserved

 File Name:      MxL5007.h

 */
#ifndef _DRV_TUNER5007_H_
#define _DRV_TUNER5007_H_


#include "MxL5007_Common.h"


typedef struct
{
	MS_U8 Num;	//Register number
	MS_U8 Val;	//Register value
} IRVType,*PIRVType;




MS_U32 MxL5007_Init(MS_U8* pArray,				// a array pointer that store the addr and data pairs for I2C write
					MS_U32* Array_Size,			// a integer pointer that store the number of element in above array
					MS_U8 Mode,
					SINT32 IF_Diff_Out_Level,
					MS_U32 Xtal_Freq_Hz,
					MS_U32 IF_Freq_Hz,
					MS_U8 Invert_IF,
					MS_U8 Clk_Out_Enable,
					MS_U8 Clk_Out_Amp
					);

MS_U32 MxL5007_RFTune(MS_U8* pArray, MS_U32* Array_Size,
					 MS_U32 RF_Freq,			// RF Frequency in Hz
					 MS_U8 BWMHz		// Bandwidth in MHz
					 );
MS_U32 SetIRVBit(PIRVType pIRV, MS_U8 Num, MS_U8 Mask, MS_U8 Val);

#endif
