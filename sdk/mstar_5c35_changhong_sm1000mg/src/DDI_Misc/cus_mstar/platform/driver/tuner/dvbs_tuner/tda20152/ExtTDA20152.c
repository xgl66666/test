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
#include "ExtTDA20152.h"
#include "II2C.h"
#include "IBSP.h"
#include "Board.h"

#if IF_THIS_TUNER_INUSE(TUNER_TDA20152)

static TDA20152_CONTEXT stTunerContext;
static TDA20152_GAIN_MODE eSelectedGainMode;
static TDA20152_PWR_DETECT_THERM_VAL eSelectedGainModePowerDetectValue;
const TDA20152_GMC_BANDWIDTH_TABLE aGmcBandwidth[TDA20152_GMC_BANDWIDTH_TABLE_MAX] =
		  {{  2850, TDA20152_GMC_BW_2P3MHZ	},
		   {  3900, TDA20152_GMC_BW_3P4MHZ	},
		   {  4850, TDA20152_GMC_BW_4P4MHZ	},
		   {  5800, TDA20152_GMC_BW_5P3MHZ	},
		   {  6650, TDA20152_GMC_BW_6P3MHZ	},
		   {  7700, TDA20152_GMC_BW_7MHZ	},
		   {  8900, TDA20152_GMC_BW_8P4MHZ	},
		   {  9900, TDA20152_GMC_BW_9P4MHZ	},
		   { 10700, TDA20152_GMC_BW_10P4MHZ },
		   { 11400, TDA20152_GMC_BW_11MHZ	},
		   { 12500, TDA20152_GMC_BW_11P8MHZ },
		   { 13500, TDA20152_GMC_BW_13P2MHZ },
		   { 14550, TDA20152_GMC_BW_13P8MHZ },
		   { 15800, TDA20152_GMC_BW_15P3MHZ },
		   { 16800, TDA20152_GMC_BW_16P3MHZ },
		   { 17750, TDA20152_GMC_BW_17P3MHZ },
		   { 18350, TDA20152_GMC_BW_18P2MHZ },
		   { 19200, TDA20152_GMC_BW_18P5MHZ },
		   { 20200, TDA20152_GMC_BW_19P9MHZ },
		   { 21000, TDA20152_GMC_BW_20P5MHZ },
		   { 22450, TDA20152_GMC_BW_21P5MHZ },
		   { 23900, TDA20152_GMC_BW_23P4MHZ },
		   { 25000, TDA20152_GMC_BW_24P4MHZ },
		   { 26500, TDA20152_GMC_BW_25P6MHZ },
		   { 27900, TDA20152_GMC_BW_27P4MHZ },
		   { 28750, TDA20152_GMC_BW_28P4MHZ },
		   { 29700, TDA20152_GMC_BW_29P1MHZ },
		   { 30700, TDA20152_GMC_BW_30P3MHZ },
		   { 31450, TDA20152_GMC_BW_31P1MHZ },
		   { 33950, TDA20152_GMC_BW_31P8MHZ },
		   { 39000, TDA20152_GMC_BW_36P1MHZ },
		   { 42500, TDA20152_GMC_BW_41P9MHZ },
		   { 43950, TDA20152_GMC_BW_43P1MHZ },
		   { 47350, TDA20152_GMC_BW_44P8MHZ },
		   { 51700, TDA20152_GMC_BW_49P9MHZ },
		   { 57750, TDA20152_GMC_BW_53P5MHZ },
		   { 100000, TDA20152_GMC_BW_62MHZ	 }};

#ifndef UINT32_CALCULATION
/******************************************************************************
 * Description:
 *      Function used to make calculation on 64 bits
 *      in order to increase the precision
 *
 * Return Value:
 *       nothing
 *
 * Parameters:
 * I/O  Parameters            Descriptions
 * ----------------------------------------------------------------------------
 * IN
 ******************************************************************************/
static void LeftShift( unsigned long* ulNumMSB, unsigned long* ulNumLSB )
{
    *ulNumMSB = (*ulNumMSB << 1) + (*ulNumLSB >> (INTEGER_SIZE - 1));
    *ulNumLSB = (*ulNumLSB & SHIFT_MASK) << 1;

    return;
}
#if 0
static void RightShift( unsigned long* ulNumMSB, unsigned long* ulNumLSB )
{
    *ulNumLSB = (*ulNumLSB >> 1) + (*ulNumMSB & 0x1 << (INTEGER_SIZE - 1));
    *ulNumMSB = *ulNumMSB >> 1;

    return;
}
#endif
static void Add( unsigned long* pulResMSB, unsigned long* pulResLSB, unsigned long ulAddMSB, unsigned long ulAddLSB )
{
    unsigned long ulCarry = 0;

    if( (*pulResLSB >= (MAX_VALUE - ulAddLSB)) || (ulAddLSB >= (MAX_VALUE - *pulResLSB)) )
    {
        ulCarry = 1;
    }

    *pulResLSB += ulAddLSB;
    *pulResMSB += ulAddMSB + ulCarry;

    return;
}

static void Sub( unsigned long* pulResMSB, unsigned long* pulResLSB, unsigned long ulSubMSB, unsigned long ulSubLSB )
{
    unsigned long ulCarry = 0;

    /* left operande must always be bigger than right operande */
    if( *pulResLSB < ulSubLSB )
    {
        ulCarry = 1;

        *pulResLSB = MAX_VALUE - ( ulSubLSB - *pulResLSB ) + 1;
    }
    else
    {
        *pulResLSB -= ulSubLSB;
    }
    *pulResMSB -= (ulSubMSB + ulCarry);

    return;
}

static void Mult( unsigned long ulOp1, unsigned long ulOp2, unsigned long* pulResMSB, unsigned long* pulResLSB )
{
    unsigned long ulIndex = 0;
    unsigned long ulMul = 0;

    unsigned long ulPartResMSB;
    unsigned long ulPartResLSB;

    // intialise
    *pulResMSB = 0;
    *pulResLSB = 0;

    ulPartResMSB = 0;
    ulPartResLSB = ulOp1;

    ulIndex = 0;

    do
    {
        ulMul = ulOp2 >> ulIndex;
        if( (ulMul & MULT_MASK) == 1 )
        {
            Add( pulResMSB, pulResLSB, ulPartResMSB, ulPartResLSB );
        }

        LeftShift( &ulPartResMSB, &ulPartResLSB );
        ulIndex ++;
    } while( ulMul != 0 );

    return;
}

static BOOL IsBigger( unsigned long ulValMSB, unsigned long ulValLSB, unsigned long ulWithMSB, unsigned long ulWithLSB)
{
    BOOL bIsBigger = False;

    if( ulValMSB > ulWithMSB )
    {
        bIsBigger = True;
    }
    else if( ulValMSB < ulWithMSB )
    {
        bIsBigger = False;
    }
    else
    {
        /* both MSB have the same value */
        /* compare LSB */
        if( ulValLSB > ulWithLSB )
        {
            bIsBigger = True;
        }
        else
        {
            bIsBigger = False;
        }
    }

    return bIsBigger;
}

static void Div( unsigned long ulNumMSB, unsigned long ulNumLSB, unsigned long ulDenom, unsigned long* pulResMSB, unsigned long* pulResLSB)
{
    unsigned long ulDenomMSB;
    unsigned long ulDenomLSB;

    unsigned long ulDenomMSBtemp;
    unsigned long ulDenomLSBtemp;

    unsigned long ulResMSBtemp;
    unsigned long ulResLSBtemp;

    *pulResMSB = 0;
    *pulResLSB = 0;

    ulDenomMSBtemp = 0;
    ulDenomLSBtemp = ulDenom;

    while( IsBigger( ulNumMSB, ulNumLSB, ulDenomMSBtemp, ulDenomLSBtemp ) == True )
    {
        ulResMSBtemp = 0;
        ulResLSBtemp = 1;

        do
        {
            ulDenomMSB = ulDenomMSBtemp;
            ulDenomLSB = ulDenomLSBtemp;

            /* shift of numerator */
            LeftShift( &ulDenomMSBtemp, &ulDenomLSBtemp );

            if( IsBigger( ulNumMSB, ulNumLSB, ulDenomMSBtemp, ulDenomLSBtemp ) == True )
            {
                /* update temporary result */
                LeftShift( &ulResMSBtemp, &ulResLSBtemp );
            }

        } while( IsBigger( ulNumMSB, ulNumLSB, ulDenomMSBtemp, ulDenomLSBtemp ) == True );

        /* case of fully match (integer division) */
        if( (ulDenomMSBtemp == ulNumMSB) && (ulDenomLSBtemp == ulNumLSB) )
        {
            ulDenomMSB = ulDenomMSBtemp;
            ulDenomLSB = ulDenomLSBtemp;

            LeftShift( &ulResMSBtemp, &ulResLSBtemp );
        }

        /* add to result variable */
        Add( pulResMSB, pulResLSB, ulResMSBtemp, ulResLSBtemp );

        /* calculate new numerator */
        Sub( &ulNumMSB, &ulNumLSB, ulDenomMSB, ulDenomLSB );

        /* reintialise for next loop */
        ulDenomMSBtemp = 0;
        ulDenomLSBtemp = ulDenom;
    }

    return;
}
#endif

static void * handle;
static TDA20152_COMMUNICATION communication;

//////////////////////// REGISTER OPERATIONS /////////////////////////////
static unsigned long read_mask;
static unsigned long read_value;
static unsigned long write_mask;
static unsigned long write_value;
static unsigned long iostatus;
static unsigned long byte_order = (unsigned long)SAT_TUNER_BYTE_ORDER; /* Change the default in stinger_reg.h */

BOOL SAT_TUNER_RegRead(const TDA20152_SAT_TUNER_COMMUNICATION* p_communication,
                  unsigned short				 address,
                  unsigned short				 offset,
                  unsigned char					 start_bit,
                  unsigned char					 num_bit,
                  unsigned char					 num_bytes,
                  unsigned long*				 p_data);

BOOL SAT_TUNER_RegWrite(const TDA20152_SAT_TUNER_COMMUNICATION* p_communication,
				   unsigned short				  address,
				   unsigned short				  offset,
				   unsigned char				  start_bit,
				   unsigned char				  num_bit,
				   unsigned char				  num_bytes,
				   BOOL							  rmw,
				   unsigned char				  access,
				   unsigned long				  data);

/* globals */
#if 0
TDA20152_SAT_TUNER_GENERIC_READ	 sat_generic_rd = SAT_TUNER_RegRead;
TDA20152_SAT_TUNER_GENERIC_WRITE sat_generic_wr = SAT_TUNER_RegWrite;

#endif
/* Used by the EVK GUI */
void TDA20152_SAT_TUNER_SetByteOrder(BOOL little_endian) /* SAT_TUNER_LITTLE_ENDIAN or SAT_TUNER_BIG_ENDIAN */
{
	if (little_endian == True)
	{
		byte_order = SAT_TUNER_LITTLE_ENDIAN;
	}
	else
	{
		byte_order = SAT_TUNER_BIG_ENDIAN;
	}
}

/*****************************************************************************/
/* SAT_TUNER_RegRead()                                                         */
/*                                                                           */
/*****************************************************************************/
BOOL SAT_TUNER_RegRead(const TDA20152_SAT_TUNER_COMMUNICATION* p_communication,
                  unsigned short				 address,
                  unsigned short				 offset,
                  unsigned char					 start_bit,
                  unsigned char					 num_bit,
                  unsigned char					 num_bytes,
                  unsigned long*				 p_data)
{
    unsigned char* p_byte = (unsigned char*)&read_value;
    unsigned char loop;

	if (num_bit < 32)
	{
		read_mask = ( ((1UL << num_bit) - 1UL) << (unsigned long)start_bit );
	}
	else /* start_bit will always be 0 */
	{
		read_mask = 0xFFFFFFFFUL;
	}

    if (p_communication == 0)
    {
        return (False);
    }

    if (p_communication->read == 0)
    {
        return (False);
    }

    if (p_data == 0)
    {
        return (False);
    }

    if (num_bytes == 0)
    {
        num_bytes = 1;
    }
    if (num_bytes > 4)
    {
        num_bytes = 4;
    }

    read_value = 0;
    for (loop = 0; loop < num_bytes; loop++)
    {
        p_byte[num_bytes-loop-1] = p_communication->read(p_communication->p_handle, (unsigned char)(address+offset+loop), &iostatus);

	    if (iostatus != 0UL)
	    {
		    return (False);
	    }
    }

	if (byte_order == SAT_TUNER_BIG_ENDIAN)
	{
		SAT_TUNER_CHANGE_BYTE_ORDER_LONG(read_value);
	}

    *p_data = (read_value & read_mask) >> (unsigned long)start_bit;

    return (True);
}


/*****************************************************************************/
/* SAT_TUNER_RegWrite() */
/*   */
/*****************************************************************************/
BOOL SAT_TUNER_RegWrite(const TDA20152_SAT_TUNER_COMMUNICATION* p_communication,
				   unsigned short				  address,
				   unsigned short				  offset,
				   unsigned char				  start_bit,
				   unsigned char				  num_bit,
				   unsigned char				  num_bytes,
				   BOOL							  rmw,
				   unsigned char				  access,
				   unsigned long				  data)
{
    unsigned char* p_byte = (unsigned char*)&write_value;
    unsigned char loop, byte_value, wr_mask;

	if (num_bit < 32)
	{
		write_mask  = (unsigned long)( ((1UL << num_bit) - 1UL) << (unsigned long)start_bit);
	}
	else /* start_bit will always be 0 */
	{
		write_mask = 0xFFFFFFFFUL;
	}

	write_value = ((data << start_bit) & write_mask);

    if (p_communication == 0)
    {
        return (False);
    }

    if (p_communication->write == 0)
    {
        return (False);
    }

    if ((access & SAT_TUNER_REG_WRITEABLE) == 0) /* read-only */
    { /* WRITE ATTEMPTED ON READ-ONLY BITFIELD */
        return (True);
    }

    if (num_bytes == 0)
    {
        num_bytes = 1;
    }
    if (num_bytes > 4)
    {
        num_bytes = 4;
    }

	if (byte_order == SAT_TUNER_BIG_ENDIAN)
	{
		SAT_TUNER_CHANGE_BYTE_ORDER_LONG(write_value);
	}

    for (loop = 0; loop < num_bytes; loop++)
    {
		if (rmw == True)
		{
			byte_value = p_communication->read(p_communication->p_handle, (unsigned char)(address+offset+loop), &iostatus);
			wr_mask = (unsigned char)(write_mask >> (8UL * (unsigned long)loop));
			byte_value &= ~wr_mask;
			p_byte[num_bytes-loop-1] |= byte_value;
		}
        p_communication->write(p_communication->p_handle, (unsigned char)(address+offset+loop), p_byte[num_bytes-loop-1], &iostatus);

	    if (iostatus != 0UL)
	    {
		    return (False);
	    }
    }

    return (True);
}

void SAT_TUNER_strncpy(char *ptStrOut, const char *ptStrIn, unsigned int uiStrSize)
{
	unsigned int i;

	for (i = 0; i < uiStrSize; i++)
	{
		ptStrOut[i] = ptStrIn[i];
	}
}

void TDA20152_Sleep( unsigned long ulTimeUs )
{
    unsigned long ulTime;

    if( ulTimeUs >= 1000 )
        ulTime = ulTimeUs/1000;
    else
        ulTime = 1;

    AVL_DVBSx_IBSP_Delay(ulTime);

    return;
}


/*******************************************************************************************************
 * TDA20152_GetChipInfo()
 *
 * Descrition:
 *      Return the version of the HW component
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise.
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * --------------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * OUT  ppcTunerString      BLACKWIDOW chip type string.
 * OUT  pulChipVersion      BLACKWIDOW chip version.
 * OUT  pulChipId           BLACKWIDOW chip ID.
 *
 *******************************************************************************************************/
BOOL TDA20152_GetChipInfo( const TDA20152_COMMUNICATION* ptCommunication,
                        char**                          ppcTunerString,
                        unsigned long*                  pulChipVersion,
                        unsigned long*                  pulChipId )
{
    unsigned long ulRegValue;
    BOOL bError = True;

    /* retrieve the chip ID */
    bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_CHIP_ID, &ulRegValue );
    if( bError != False )
    {
        *pulChipId = ulRegValue;
    }

    /* retrieve the chip version */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_CHIP_VER, &ulRegValue );
    }

    if( bError != False )
    {
        *pulChipVersion = ulRegValue;
    }

    /* indicate the product name */
    *ppcTunerString = TDA20152_PRODUCT_NAME_STRING;

    return( bError );
}

/*********************************************************************************
 * TDA20152_GetDriverVersion()
 *
 * Description:
 *      Get the driver version number string.
 *
 * Return Value:
 *  TRUE - operation successful; FALSE - otherwise.
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * --------------------------------------------------------------------------------
 * OUT  ptDriverVersion    Returned product version string.
 *
 **********************************************************************************/
BOOL TDA20152_GetDriverVersion( TDA20152_SAT_TUNER_DRIVER_VERSION *ptDriverVersion )
{
    BOOL bError = True;

    if (ptDriverVersion == 0)
    {
        bError = False;
    }
    else
    {
        SAT_TUNER_strncpy( ptDriverVersion->version_str,
                 TDA20152_PRODUCT_VERSION_STRING,
                 sizeof(TDA20152_PRODUCT_VERSION_STRING) );
    }

    return( bError );
}

/*****************************************************************************
 * TDA20152_ProgramPllPor
 *
 * Description:
 *      Program PLL after POR
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 *****************************************************************************/
BOOL TDA20152_ProgramPllPor ( const TDA20152_COMMUNICATION*  ptCommunication )
{
    BOOL bError = True;

    if( ptCommunication == 0)
    {
        bError = False;
    }

    if( bError != False )
    {
        /* wait 1 us */
        TDA20152_Sleep( 1 );

        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PLL_EN_PLL_DIVIDERS_EN, 1 );
    }

    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PFD_CP1_CP_FORCE_UNLOCK, 1 );
    }

    if( bError != False )
    {
        /* wait 1us */
        TDA20152_Sleep( 1 );

        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PFD_CP1_CP_EN, 1 );
    }

    if( bError != False )
    {
        /* wait 1us */
        TDA20152_Sleep( 1 );

        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PLL_TEST1_DSM_RESET_N, 1 );
    }

    if( bError != False )
    {
        /* wait 1us */
        TDA20152_Sleep( 1 );

        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PFD_CP1_CP_FORCE_UNLOCK, 0 );
    }

    return( bError );
}


/*****************************************************************************
 * TDA20152_ProgramVcoPor
 *
 * Description:
 *      Program VCO after POR
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 *****************************************************************************/
BOOL TDA20152_ProgramVcoPor ( const TDA20152_COMMUNICATION*  ptCommunication )
{
    BOOL bError = True;
    unsigned long ulRegMask;
    unsigned long ulRegValue;
    unsigned long ulPollingLimit = TDA20152_ALC_T16_CHECK;

    if( ptCommunication == 0)
    {
        bError = False;
    }

    /************/
    /** step 1 **/
    /************/
    /* VCO_ALC1 */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_ALC1, &ulRegValue );
    }

    if( bError != False )
    {
        ulRegMask = TDA20152_VCO_ALC1_IVREF_EN_WR(TDA20152_CLEAN) |
                    TDA20152_VCO_ALC1_ALC_SPIC_EN_WR(TDA20152_CLEAN) |
                    TDA20152_VCO_ALC1_ALC_EN_WR(TDA20152_CLEAN);

        ulRegValue &= ~ulRegMask;

        /* Bandgap LDO ON */
        ulRegValue |= TDA20152_VCO_ALC1_IVREF_EN_WR(1);

        /* ALC OFF */
        ulRegValue |= TDA20152_VCO_ALC1_ALC_SPIC_EN_WR(0);
        ulRegValue |= TDA20152_VCO_ALC1_ALC_EN_WR(0);

        /* write registers */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_ALC1, ulRegValue );
    }

    /* VCO_TM1 */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_TM1, &ulRegValue );
    }

    if( bError != False )
    {
        ulRegMask = TDA20152_VCO_TM1_TM_SPIC_EN_WR(TDA20152_CLEAN) |
                    TDA20152_VCO_TM1_TM_EN_WR(TDA20152_CLEAN);

        ulRegValue &= ~ulRegMask;

        /* TM OFF */
        ulRegValue |= TDA20152_VCO_TM1_TM_SPIC_EN_WR(1);
        ulRegValue |= TDA20152_VCO_TM1_TM_EN_WR(0);

        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM1, ulRegValue );
    }

    /* VCO_CAL_CLK */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_CAL_CLK, &ulRegValue );
    }

    if( bError != False )
    {
        ulRegMask = TDA20152_VCO_CAL_CLK_CLKGEN_EN_WR(TDA20152_CLEAN) |
                    TDA20152_VCO_CAL_CLK_CLCKGEN_INTCLK_WR(TDA20152_CLEAN);

        ulRegValue &= ~ulRegMask;

        /* CALCLK ON */
        ulRegValue |= TDA20152_VCO_CAL_CLK_CLKGEN_EN_WR(1);
        ulRegValue |= TDA20152_VCO_CAL_CLK_CLCKGEN_INTCLK_WR(1);

        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_CAL_CLK, ulRegValue );
    }

    /************/
    /** step 2 **/
    /************/
    /* Change CALCLK rate if required */
    /*if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_CAL_CLK_CLKGEN_SPDSEL, 0x2 );
    }*/

    if( bError != False )
    {
        /* Internal Vtune forced to 2.8V */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_ALC2_VT_ALC_C, 1 );
    }

	if( bError != False )
    {
        /* Internal Vtune forced to 2.8V */
		/* Repeat to allow time for VCO Settling */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_ALC2_VT_ALC_C, 1 );
    }

    if( bError != False )
    {
        /* Enable ALC */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_ALC1_ALC_EN, 1 );
    }

    /************/
    /** step 3 **/
    /************/
    /* check alc_t16_over */
    /* read ALC_bias depending on the value */

    if( bError != False )
    {
        do
        {
            bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_ALC_OUT_ALC_T16_OVER, &ulRegValue );

            /* avoid infinite loop */
            if( (ulPollingLimit == 0) && (ulRegValue == 0) )
            {
                /* this is not supposed to happen! */
                bError = False;
            }
            ulPollingLimit --;

        } while( ulRegValue == 0 &&  (bError != False));
    }

    if( bError != False )
    {
        /* read ALC output */
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_ALC2_ALC_BIAS, &ulRegValue );
    }

    if( bError != False )
    {
        /* Store ALC results */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_ALC1_ALC_SPI_BIAS, ulRegValue );
    }

    if( bError != False )
    {
        /* turn on SPIC of ALC */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_ALC1_ALC_SPIC_EN, 1 );
    }

    if( bError != False )
    {
        /* disable ALC */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_ALC1_ALC_EN, 0 );
    }

	/* VCO_CAL_CLK */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_CAL_CLK, &ulRegValue );
    }

    if( bError != False )
    {
        /* VCO_CAL_CLK */
        ulRegMask = TDA20152_VCO_CAL_CLK_CLKGEN_EN_WR(TDA20152_CLEAN);

        ulRegValue &= ~ulRegMask;

        ulRegValue |= TDA20152_VCO_CAL_CLK_CLKGEN_EN_WR(0);

        /* write registers */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_CAL_CLK, ulRegValue );
    }

	if( bError != False )
    {
		/* set the power detector shift to 0 dB */
		bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PWR_DET_PWR_DET_CTRL, 1 );
	}

    return( bError );
}

/******************************************************************************
 * TDA20152_SetPowerMode()
 *
 * Description:
 *      Set power configuration
 *      (High Power, Low Power, Sleep, Power Down)
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ----------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * IN   ePowerMode          configure power mode
 ******************************************************************************/
BOOL TDA20152_SetPowerMode( const TDA20152_COMMUNICATION* ptCommunication,
                              const TDA20152_POWER_MODE     ePowerMode )
{
    BOOL      bError = True;
	unsigned long ulRegValue = 0;
//    unsigned long ulRegMask = 0;

    /* update configuration depending on configuration */
    switch( ePowerMode )
    {
        case TDA20152_POWER_MODE_HIGH_POWER:
            /* Enable Blocks */
			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_XTAL1, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0x81; /* xtal_en = 1, pll_ref_en = 1 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_XTAL1, ulRegValue );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_FE, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0x39;
				ulRegValue &= 0x7F; /* lna_en = 0, lpt_en = 1, lna_hg_en = 1 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_FE, ulRegValue );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_ENABLE1, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0xAE; /* rfvga_en = 1, rf_mixer_en = 1, bb_en = 1, servo_en = 1, sp[1] = 1 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_ENABLE1, ulRegValue );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_ALC1, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0x80; /* ivref_en = 1 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_ALC1, ulRegValue );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PFD_CP1, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue &= 0x7F; /* cp_en = 0 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PFD_CP1, ulRegValue );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PLL_EN, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0xC0; /* sx_bias_en = 1, pll_dividers_en = 1 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PLL_EN, ulRegValue );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_LO_CHAIN1, 0xFF );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_LO_CHAIN2, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue &= 0xFE; /* dcc1bpf_nomcurr_enh = 0 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_LO_CHAIN2, ulRegValue );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_LO_CHAIN4, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0x08; /* lomux_en = 1 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_LO_CHAIN4, ulRegValue );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_DIG_LDO, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0xC0;
				ulRegValue &= 0xDF; /* ldo_en = 1, ldo_v[1:0] = 2 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_DIG_LDO, ulRegValue );
			}

			/* Enable LO High Current */
			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_LO_CHAIN3, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0x04;
				ulRegValue &= 0xCF; /* dbl_hicur_en = 0, dcc1bpf_hicur_en = 0, dcc1limit_hicur_en = 1 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_LO_CHAIN3, ulRegValue );
			}

			/* PWR DET */
			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PWR_DET, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0x20;
				ulRegValue &= 0xDF; /* pwr_det_ctrl[1:0] = 1 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PWR_DET, ulRegValue );
			}

			/* Set RF VGA = 5, VGA1 = 4, lobuf_hg_en = 0 */
			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VGA, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0xB0;
				ulRegValue &= 0xB1; /* rf_vga_off[2:0] = 5, vga1_off[2:0] = 4, lobuf_hg_en = 0 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VGA, ulRegValue );
			}

			/* Set VGA = 3, FLTA = 25 MHz */
			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_BB1, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0x6F;
				ulRegValue &= 0x7F; /* vga2_off[2:0] = 3, flta_bw[3:0] = 0xF */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_BB1, ulRegValue );
			}

			/* Set gmC Filter = 20 MHz */
			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_BB2, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0x7A;
				ulRegValue &= 0x7B; /* gmc_bw[6:0] = 0x3D */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_BB2, ulRegValue );
			}

			/* Set Amp Out Gain to 15 dB and DC */
			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_BB3, 0x00 );
			}

			/* Charge Pump - PFD Settings */
			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PFD_CP2, 0xFA );
			}

			/* Set PLL Settings */
			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PLL_TEST1, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue &= 0x7F; /* dsm_reset_n = 0 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PLL_TEST1, ulRegValue );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PLL4, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0x40; /* pll_dsm_frac_sel = 1 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PLL4, ulRegValue );
			}

			/* Set VCO Settings */
			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_ALC2, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0x90;
				ulRegValue &= 0xBF; /* alc_vth[1:0] = 2, psk8_en = 1 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_ALC2, ulRegValue );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_TM2, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0x20;
				ulRegValue &= 0xEF; /* tm_vrefl[1:0] = 2 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM2, ulRegValue );
			}

			/* Run PLL POR */
			if( bError != False )
			{
				bError = TDA20152_ProgramPllPor(ptCommunication);
			}

			/* Run VCO ALC Calibration */
			if( bError != False )
			{
				bError = TDA20152_ProgramVcoPor(ptCommunication);
			}
			break;

		case TDA20152_POWER_MODE_LOW_POWER:
			bError = False;
			break;

        case TDA20152_POWER_MODE_SLEEP:
		case TDA20152_POWER_MODE_POWER_DOWN:
			/* Toggle SPI Reset */
			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_SPI_CTRL, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0x80; /* spi_reset = 1 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_SPI_CTRL, ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue &= 0x7F; /* spi_reset = 0 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_SPI_CTRL, ulRegValue );
			}

			/* Disable Blocks */
			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_XTAL1, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue |= 0x80;
				ulRegValue &= 0xFE; /* xtal_en = 1, pll_ref_en = 0 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_XTAL1, ulRegValue );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_FE, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue &= 0x76; /* lna_en = 0, lpt_en = 0, lna_hg_en = 0 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_FE, ulRegValue );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_ENABLE1, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue &= 0x50; /* rfvga_en = 0, rf_mixer_en = 0, bb_en = 0, servo_en = 0, sp[1:0] = 0 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_ENABLE1, ulRegValue );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_ALC1, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue &= 0x7F; /* ivref_en = 0 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_ALC1, ulRegValue );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PFD_CP1, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue &= 0x7F; /* cp_en = 0 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PFD_CP1, ulRegValue );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PLL_EN, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue &= 0x3F; /* sx_bias_en = 0, pll_dividers_en = 0 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PLL_EN, ulRegValue );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_LO_CHAIN1, 0x00 );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_LO_CHAIN4, 0x00 );
			}

			if( bError != False )
			{
				bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_DIG_LDO, &ulRegValue );
			}
			if( bError != False )
			{
				ulRegValue &= 0x1F; /* ldo_en = 0, ldo_v[1:0] = 0 */
				bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_DIG_LDO, ulRegValue );
			}

			if (ePowerMode == TDA20152_POWER_MODE_POWER_DOWN)
			{
				if( bError != False )
				{
					bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_XTAL1, &ulRegValue );
				}
				if( bError != False )
				{
					ulRegValue &= 0x7F; /* xtal_en = 0 */
					bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_XTAL1, ulRegValue );
				}
			}
            break;

        default:
            bError = False;
            break;
    }

    return( bError );
}


/*******************************************************************************************************
 * TDA20152_Initialize()
 *
 * Descrition:
 *      This API is called once to intialize the BLACKWIDOW tuner chip.
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * --------------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * IN   ptConfig            BLACKWIDOW configuration.
 *******************************************************************************************************/
BOOL TDA20152_Initialize( const TDA20152_COMMUNICATION* ptCommunication,
                       const TDA20152_CONFIG*        ptConfig )
{
    BOOL bError = True;

    if( ptCommunication == 0 || ptConfig == 0 )
    {
        bError = False;
    }

    if( bError != False )
    {
        /* store xtal frequancy */
        stTunerContext.ulXtalFreqHz = ptConfig->ulCrystalFrequencyHz;

        /* set GainMode configuration to default value */
        stTunerContext.tPwrDetectThreshold.eThrehold1 = TDA20152_PWR_DETECT_THRESHOLD_1_DEFAULT;
        stTunerContext.tPwrDetectThreshold.eThrehold2 = TDA20152_PWR_DETECT_THRESHOLD_2_DEFAULT;
        stTunerContext.tPwrDetectThreshold.eThrehold3 = TDA20152_PWR_DETECT_THRESHOLD_3_DEFAULT;

        stTunerContext.tGainModeConfig.tMode0.tGainLowCurrent.bAmpOutLowCurrent   = TDA20152_GAIN_MODE_0_LOW_CURRENT_AMP_OUT_DEFAULT;
        stTunerContext.tGainModeConfig.tMode0.tGainLowCurrent.bMixerLowCurrent    = TDA20152_GAIN_MODE_0_LOW_CURRENT_MIXER_DEFAULT;
        stTunerContext.tGainModeConfig.tMode0.tGainLowCurrent.bRfVgaLowCurrent    = TDA20152_GAIN_MODE_0_LOW_CURRENT_RF_VGA_DEFAULT;
        stTunerContext.tGainModeConfig.tMode0.tGainLowCurrent.eLnaLowCurrent      = TDA20152_GAIN_MODE_0_LOW_CURRENT_LNA_DEFAULT;
        stTunerContext.tGainModeConfig.tMode0.tGainModeConfiguration.eAmpOut      = TDA20152_GAIN_MODE_0_AMP_OUT_DEFAULT;
        stTunerContext.tGainModeConfig.tMode0.tGainModeConfiguration.eLnaGain     = TDA20152_GAIN_MODE_0_LNA_DEFAULT;
        stTunerContext.tGainModeConfig.tMode0.tGainModeConfiguration.eLptGain     = TDA20152_GAIN_MODE_0_LPT_DEFAULT;
        stTunerContext.tGainModeConfig.tMode0.tGainModeConfiguration.eRfvgaOffset = TDA20152_GAIN_MODE_0_RF_VGA_DEFAULT;
        stTunerContext.tGainModeConfig.tMode0.tGainModeConfiguration.eVga1Offset  = TDA20152_GAIN_MODE_0_VGA1_DEFAULT;
        stTunerContext.tGainModeConfig.tMode0.tGainModeConfiguration.eVga2Offset  = TDA20152_GAIN_MODE_0_VGA2_DEFAULT;

        stTunerContext.tGainModeConfig.tMode1.tGainLowCurrent.bAmpOutLowCurrent   = TDA20152_GAIN_MODE_1_LOW_CURRENT_AMP_OUT_DEFAULT;
        stTunerContext.tGainModeConfig.tMode1.tGainLowCurrent.bMixerLowCurrent    = TDA20152_GAIN_MODE_1_LOW_CURRENT_MIXER_DEFAULT;
        stTunerContext.tGainModeConfig.tMode1.tGainLowCurrent.bRfVgaLowCurrent    = TDA20152_GAIN_MODE_1_LOW_CURRENT_RF_VGA_DEFAULT;
        stTunerContext.tGainModeConfig.tMode1.tGainLowCurrent.eLnaLowCurrent      = TDA20152_GAIN_MODE_1_LOW_CURRENT_LNA_DEFAULT;
        stTunerContext.tGainModeConfig.tMode1.tGainModeConfiguration.eAmpOut      = TDA20152_GAIN_MODE_1_AMP_OUT_DEFAULT;
        stTunerContext.tGainModeConfig.tMode1.tGainModeConfiguration.eLnaGain     = TDA20152_GAIN_MODE_1_LNA_DEFAULT;
        stTunerContext.tGainModeConfig.tMode1.tGainModeConfiguration.eLptGain     = TDA20152_GAIN_MODE_1_LPT_DEFAULT;
        stTunerContext.tGainModeConfig.tMode1.tGainModeConfiguration.eRfvgaOffset = TDA20152_GAIN_MODE_1_RF_VGA_DEFAULT;
        stTunerContext.tGainModeConfig.tMode1.tGainModeConfiguration.eVga1Offset  = TDA20152_GAIN_MODE_1_VGA1_DEFAULT;
        stTunerContext.tGainModeConfig.tMode1.tGainModeConfiguration.eVga2Offset  = TDA20152_GAIN_MODE_1_VGA2_DEFAULT;

        stTunerContext.tGainModeConfig.tMode2.tGainLowCurrent.bAmpOutLowCurrent   = TDA20152_GAIN_MODE_2_LOW_CURRENT_AMP_OUT_DEFAULT;
        stTunerContext.tGainModeConfig.tMode2.tGainLowCurrent.bMixerLowCurrent    = TDA20152_GAIN_MODE_2_LOW_CURRENT_MIXER_DEFAULT;
        stTunerContext.tGainModeConfig.tMode2.tGainLowCurrent.bRfVgaLowCurrent    = TDA20152_GAIN_MODE_2_LOW_CURRENT_RF_VGA_DEFAULT;
        stTunerContext.tGainModeConfig.tMode2.tGainLowCurrent.eLnaLowCurrent      = TDA20152_GAIN_MODE_2_LOW_CURRENT_LNA_DEFAULT;
        stTunerContext.tGainModeConfig.tMode2.tGainModeConfiguration.eAmpOut      = TDA20152_GAIN_MODE_2_AMP_OUT_DEFAULT;
        stTunerContext.tGainModeConfig.tMode2.tGainModeConfiguration.eLnaGain     = TDA20152_GAIN_MODE_2_LNA_DEFAULT;
        stTunerContext.tGainModeConfig.tMode2.tGainModeConfiguration.eLptGain     = TDA20152_GAIN_MODE_2_LPT_DEFAULT;
        stTunerContext.tGainModeConfig.tMode2.tGainModeConfiguration.eRfvgaOffset = TDA20152_GAIN_MODE_2_RF_VGA_DEFAULT;
        stTunerContext.tGainModeConfig.tMode2.tGainModeConfiguration.eVga1Offset  = TDA20152_GAIN_MODE_2_VGA1_DEFAULT;
        stTunerContext.tGainModeConfig.tMode2.tGainModeConfiguration.eVga2Offset  = TDA20152_GAIN_MODE_2_VGA2_DEFAULT;

        stTunerContext.tGainModeConfig.tMode3.tGainLowCurrent.bAmpOutLowCurrent   = TDA20152_GAIN_MODE_3_LOW_CURRENT_AMP_OUT_DEFAULT;
        stTunerContext.tGainModeConfig.tMode3.tGainLowCurrent.bMixerLowCurrent    = TDA20152_GAIN_MODE_3_LOW_CURRENT_MIXER_DEFAULT;
        stTunerContext.tGainModeConfig.tMode3.tGainLowCurrent.bRfVgaLowCurrent    = TDA20152_GAIN_MODE_3_LOW_CURRENT_RF_VGA_DEFAULT;
        stTunerContext.tGainModeConfig.tMode3.tGainLowCurrent.eLnaLowCurrent      = TDA20152_GAIN_MODE_3_LOW_CURRENT_LNA_DEFAULT;
        stTunerContext.tGainModeConfig.tMode3.tGainModeConfiguration.eAmpOut      = TDA20152_GAIN_MODE_3_AMP_OUT_DEFAULT;
        stTunerContext.tGainModeConfig.tMode3.tGainModeConfiguration.eLnaGain     = TDA20152_GAIN_MODE_3_LNA_DEFAULT;
        stTunerContext.tGainModeConfig.tMode3.tGainModeConfiguration.eLptGain     = TDA20152_GAIN_MODE_3_LPT_DEFAULT;
        stTunerContext.tGainModeConfig.tMode3.tGainModeConfiguration.eRfvgaOffset = TDA20152_GAIN_MODE_3_RF_VGA_DEFAULT;
        stTunerContext.tGainModeConfig.tMode3.tGainModeConfiguration.eVga1Offset  = TDA20152_GAIN_MODE_3_VGA1_DEFAULT;
        stTunerContext.tGainModeConfig.tMode3.tGainModeConfiguration.eVga2Offset  = TDA20152_GAIN_MODE_3_VGA2_DEFAULT;

		bError = TDA20152_SetPowerMode(ptCommunication, TDA20152_POWER_MODE_HIGH_POWER);
		eSelectedGainMode = TDA20152_GAIN_MODE_INVALID;
		eSelectedGainModePowerDetectValue = TDA20152_PWR_DETECT_THERM_VAL_INVALID;
    }

    return( bError );
}


/*****************************************************************************
 * TDA20152_GetPllDsmOrderAndFractCorrect
 *
 * Description:
 *      Configure Fractional Correction and DSM order
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 * OUT  peDsmOrder           DSM order (MASH11 or MASH111)
 * OUT  peDsmFrac            Fractional order
 *****************************************************************************/
BOOL TDA20152_GetPllDsmOrderAndFractCorrect ( const TDA20152_COMMUNICATION*  ptCommunication,
                                           TDA20152_DSM_MASH_MODE*        peDsmOrder,
                                           TDA20152_DSM_FRAC*             peDsmFrac )

{
    BOOL bError = True;
    unsigned long ulRegValue;

    if( ptCommunication == 0)
    {
        bError = False;
    }

    if( bError != False )
    {
        /* read fractional mode */
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PLL4_PLL_DSM_FRAC_SEL, &ulRegValue );
        *peDsmFrac = (TDA20152_DSM_FRAC)ulRegValue;
    }

    if( bError != False )
    {
        /* DSM configuration: mash mode (DSM number of stage) */
        /* Mash111 Mash11 */
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PLL_TEST1_DSM_MASH11_MODE_EN, &ulRegValue );
        *peDsmOrder = (TDA20152_DSM_MASH_MODE)ulRegValue;
    }

    return( bError );
}

/*****************************************************************************
 * TDA20152_GetLoConfig()
 *
 * Description:
 *      returns current LO settings
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 * IN   ptLoSettings         current LO settings
 *****************************************************************************/
BOOL TDA20152_GetLoConfig( const TDA20152_COMMUNICATION* ptCommunication,
                        TDA20152_LO_SETTINGS*         ptLoSettings )
{
    BOOL bError = True;
    unsigned long ulRegValue;

    if( (ptCommunication == 0) || (ptLoSettings == 0)  )
    {
        bError = False;
    }

    /*************************************************************
        TDA20152_LO_CHAIN1_DBL_EN        bDoublerEnable
        TDA20152_LO_CHAIN1_DCC1_EN       bDcc1Enable
        TDA20152_LO_CHAIN1_DCC2_EN       bDcc2Enable
        TDA20152_LO_CHAIN1_DIV2_EN       bFinalDividerEnable
        TDA20152_LO_CHAIN1_INBUF_EN      bInputBufEnable
        TDA20152_LO_CHAIN1_MAIN_DIV_EN   bMainDivEnable
        TDA20152_LO_CHAIN1_OP_MUX_EN     bOutputMuxEnable
        TDA20152_LO_CHAIN1_POLY_EN       bPpfEnable

        TDA20152_LO_CHAIN2_DIV1_MODE3    bDiv1ConfigInDivideBy3
        TDA20152_LO_CHAIN2_DIV2_MODE3    bDiv2ConfigInDivideBy3
        TDA20152_LO_CHAIN2_OP_4TO7_SEL   bSelectDivideBy4Or5Or6Or7Path
        TDA20152_LO_CHAIN2_OP_DIV8_SEL   bSelectDivideBy8Path

        TDA20152_LO_CHAIN4_LOMUX_EN      bInputMuxEnable
    *************************************************************/

    /* LO_CHAIN1 */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_LO_CHAIN1, &ulRegValue );
    }

    if( bError != False )
    {
        ptLoSettings->bDoublerEnable  = (BOOL) TDA20152_LO_CHAIN1_DBL_EN_RD( ulRegValue );
        ptLoSettings->bDcc1Enable  = (BOOL) TDA20152_LO_CHAIN1_DCC1_EN_RD( ulRegValue );
        ptLoSettings->bDcc2Enable = (BOOL) TDA20152_LO_CHAIN1_DCC2_EN_RD( ulRegValue );
        ptLoSettings->bFinalDividerEnable = (BOOL) TDA20152_LO_CHAIN1_DIV2_EN_RD( ulRegValue );
        ptLoSettings->bInputBufEnable = (BOOL) TDA20152_LO_CHAIN1_INBUF_EN_RD( ulRegValue );
        ptLoSettings->bMainDivEnable = (BOOL) TDA20152_LO_CHAIN1_MAIN_DIV_EN_RD( ulRegValue );
        ptLoSettings->bOutputMuxEnable = (BOOL) TDA20152_LO_CHAIN1_OP_MUX_EN_RD( ulRegValue );
        ptLoSettings->bPpfEnable = (BOOL) TDA20152_LO_CHAIN1_POLY_EN_RD( ulRegValue );
    }

    /* LO_CHAIN2 */
    /* one bit is missing to fill all the register */
    /* Divider is spitted into 2 parts: Div1 and Div2 */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_LO_CHAIN2, &ulRegValue );
    }

    if( bError != False )
    {
        /* Div1 have 2 configs: 2 or 3 */
        ptLoSettings->bDiv1ConfigInDivideBy3 = (BOOL) TDA20152_LO_CHAIN2_DIV1_MODE3_RD( ulRegValue );

        /* Div2 have 2 configs: 2 or 3 */
        ptLoSettings->bDiv2ConfigInDivideBy3 = (BOOL) TDA20152_LO_CHAIN2_DIV2_MODE3_RD( ulRegValue );

        /* Enable path with the divider */
        ptLoSettings->bSelectDivideBy4Or5Or6Or7Path = (BOOL) TDA20152_LO_CHAIN2_OP_4TO7_SEL_RD( ulRegValue );

        /* Enable path without the divider (used for divide by 8) */
        ptLoSettings->bSelectDivideBy8Path = (BOOL) TDA20152_LO_CHAIN2_OP_DIV8_SEL_RD( ulRegValue );
    }

    /* LO_CHAIN4 */
    if( bError != False )
    {
        /* Enable input mux (LO 2:1 MUX) */
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_LO_CHAIN4_LOMUX_EN, &ulRegValue );
        ptLoSettings->bInputMuxEnable = (BOOL) ulRegValue;
    }

    return( bError );
}


/*****************************************************************************
 * TDA20152_SetLoConfig()
 *
 * Description:
 *      Configure Local Oscillator
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 * IN   TDA20152_LO_SETTINGS  LO settings to apply
 *****************************************************************************/
BOOL TDA20152_SetLoConfig( const TDA20152_COMMUNICATION* ptCommunication,
                        const TDA20152_LO_SETTINGS*   ptLoSettings )
{
    BOOL bError = True;
    unsigned long ulRegValue;
    unsigned long ulRegMask;

    if( (ptCommunication == 0) || (ptLoSettings == 0)  )
    {
        bError = False;
    }

    /*************************************************************
        TDA20152_LO_CHAIN1_DBL_EN        bDoublerEnable
        TDA20152_LO_CHAIN1_DCC1_EN       bDcc1Enable
        TDA20152_LO_CHAIN1_DCC2_EN       bDcc2Enable
        TDA20152_LO_CHAIN1_DIV2_EN       bFinalDividerEnable
        TDA20152_LO_CHAIN1_INBUF_EN      bInputBufEnable
        TDA20152_LO_CHAIN1_MAIN_DIV_EN   bMainDivEnable
        TDA20152_LO_CHAIN1_OP_MUX_EN     bOutputMuxEnable
        TDA20152_LO_CHAIN1_POLY_EN       bPpfEnable

        TDA20152_LO_CHAIN2_DIV1_MODE3    bDiv1ConfigInDivideBy3
        TDA20152_LO_CHAIN2_DIV2_MODE3    bDiv2ConfigInDivideBy3
        TDA20152_LO_CHAIN2_OP_4TO7_SEL   bSelectDivideBy4Or5Or6Or7Path
        TDA20152_LO_CHAIN2_OP_DIV8_SEL   bSelectDivideBy8Path

        TDA20152_LO_CHAIN4_LOMUX_EN      bInputMuxEnable
    *************************************************************/

    /* LO_CHAIN1 */
    /* all the register is read, no need to use a mask */
    ulRegValue = TDA20152_LO_CHAIN1_DBL_EN_WR( ptLoSettings->bDoublerEnable ) |
                 TDA20152_LO_CHAIN1_DCC1_EN_WR( ptLoSettings->bDcc1Enable ) |
                 TDA20152_LO_CHAIN1_DCC2_EN_WR( ptLoSettings->bDcc2Enable ) |
                 TDA20152_LO_CHAIN1_DIV2_EN_WR( ptLoSettings->bFinalDividerEnable ) |
                 TDA20152_LO_CHAIN1_INBUF_EN_WR( ptLoSettings->bInputBufEnable ) |
                 TDA20152_LO_CHAIN1_MAIN_DIV_EN_WR( ptLoSettings->bMainDivEnable ) |
                 TDA20152_LO_CHAIN1_OP_MUX_EN_WR( ptLoSettings->bOutputMuxEnable ) |
                 TDA20152_LO_CHAIN1_POLY_EN_WR( ptLoSettings->bPpfEnable );

    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_LO_CHAIN1, ulRegValue );
    }

    /* LO_CHAIN2 */
    /* one bit is missing to fill all the register */
    /* Divider is spitted into 2 parts: Div1 and Div2 */

    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_LO_CHAIN2, &ulRegValue );

        ulRegMask = TDA20152_LO_CHAIN2_DIV1_MODE3_WR(TDA20152_CLEAN) |
                    TDA20152_LO_CHAIN2_DIV2_MODE3_WR(TDA20152_CLEAN) |
                    TDA20152_LO_CHAIN2_OP_4TO7_SEL_WR(TDA20152_CLEAN) |
                    TDA20152_LO_CHAIN2_OP_DIV8_SEL_WR(TDA20152_CLEAN);

        ulRegValue &= ~ulRegMask;
    }

    if( bError != False )
    {
        /* Div1 have 2 configs: 2 or 3 */
        ulRegValue |= TDA20152_LO_CHAIN2_DIV1_MODE3_WR( (unsigned long) ptLoSettings->bDiv1ConfigInDivideBy3) ;

        /* Div2 have 2 configs: 2 or 3 */
        ulRegValue |= TDA20152_LO_CHAIN2_DIV2_MODE3_WR( (unsigned long) ptLoSettings->bDiv2ConfigInDivideBy3 );

        /* Enable path with the divider */
        ulRegValue |= TDA20152_LO_CHAIN2_OP_4TO7_SEL_WR( (unsigned long) ptLoSettings->bSelectDivideBy4Or5Or6Or7Path );

        /* Enable path without the divider (used for divide by 8) */
        ulRegValue |= TDA20152_LO_CHAIN2_OP_DIV8_SEL_WR((unsigned long) ptLoSettings->bSelectDivideBy8Path );

        /* Write LO_CHAIN2 new value */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_LO_CHAIN2, ulRegValue );
    }

    /* LO_CHAIN4 */
    if( bError != False )
    {
        /* Enable input mux (LO 2:1 MUX) */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_LO_CHAIN4_LOMUX_EN,
                                  (unsigned long) ptLoSettings->bInputMuxEnable );
    }

    return( bError );
}

/*****************************************************************************
 * SetLoDivider()
 *
 * Description:
 *      Configure the LO divider depending on LO frequency
 *      and return ratio
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * IN   ulFreqkHz           LO frequency
 * OUT  pulNLo              LO ratio applied
 *****************************************************************************/
static BOOL SetLoDivider( const TDA20152_COMMUNICATION* ptCommunication,
                          unsigned long ulFreqkHz,
						  TDA20152_LO_DIVIDER loDiv,
                          unsigned long *pulNLo )
{
    BOOL                   bError;
    TDA20152_LO_SETTINGS tLoSettings;

    /* retrive current LO settings */
    bError = TDA20152_GetLoConfig( ptCommunication, &tLoSettings );

	if (loDiv == TDA20152_LO_DIVIDER_4)
	{
		*pulNLo = 4;
	}
	else if (loDiv == TDA20152_LO_DIVIDER_5)
	{
		*pulNLo = 5;
	}
	else if (loDiv == TDA20152_LO_DIVIDER_6)
	{
		*pulNLo = 6;
	}
	else if (loDiv == TDA20152_LO_DIVIDER_7)
	{
		*pulNLo = 7;
	}
	else if (loDiv == TDA20152_LO_DIVIDER_8)
	{
		*pulNLo = 8;
	}
	else if( ulFreqkHz <= TDA20152_LO_DIV_BY_8_LO_MAX_KHZ )
	{
		*pulNLo = 8;
	}
	else if( ulFreqkHz <= TDA20152_LO_DIV_BY_7_LO_MAX_KHZ )
    {
        *pulNLo = 7;
    }
    else if ( ulFreqkHz <= TDA20152_LO_DIV_BY_6_LO_MAX_KHZ )
    {
        *pulNLo = 6;
    }
    else if ( ulFreqkHz <= TDA20152_LO_DIV_BY_5_LO_MAX_KHZ )
    {
        *pulNLo = 5;
    }
    else
    {
        *pulNLo = 4;
    }

	tLoSettings.bDoublerEnable = True;
	tLoSettings.bDcc1Enable = True;
	tLoSettings.bDcc2Enable = True;
	tLoSettings.bFinalDividerEnable = True;
	tLoSettings.bInputBufEnable = True;
	tLoSettings.bMainDivEnable = True;
	tLoSettings.bOutputMuxEnable = True;
	tLoSettings.bPpfEnable = True;

	switch (*pulNLo)
	{
		case 8:
			// div by 8
			tLoSettings.bDiv1ConfigInDivideBy3 = False;
			tLoSettings.bDiv2ConfigInDivideBy3 = False;
			tLoSettings.bSelectDivideBy4Or5Or6Or7Path = False;
			tLoSettings.bSelectDivideBy8Path = True;

			tLoSettings.bDoublerEnable = False;
			tLoSettings.bDcc1Enable = False;
			tLoSettings.bDcc2Enable = False;
			tLoSettings.bPpfEnable = False;
			break;
		case 7:
			// div by 7
			tLoSettings.bDiv1ConfigInDivideBy3 = True;
			tLoSettings.bDiv2ConfigInDivideBy3 = True;
			tLoSettings.bSelectDivideBy4Or5Or6Or7Path = True;
			tLoSettings.bSelectDivideBy8Path = False;
			break;
		case 6:
			// div by 6
			tLoSettings.bDiv1ConfigInDivideBy3 = False;
			tLoSettings.bDiv2ConfigInDivideBy3 = True;
			tLoSettings.bSelectDivideBy4Or5Or6Or7Path = True;
			tLoSettings.bSelectDivideBy8Path = False;
			break;
		case 5:
			// div by 5
			tLoSettings.bDiv1ConfigInDivideBy3 = True;
			tLoSettings.bDiv2ConfigInDivideBy3 = False;
			tLoSettings.bSelectDivideBy4Or5Or6Or7Path = True;
			tLoSettings.bSelectDivideBy8Path = False;
			break;
		case 4:
			// div by 4
			tLoSettings.bDiv1ConfigInDivideBy3 = False;
			tLoSettings.bDiv2ConfigInDivideBy3 = False;
			tLoSettings.bSelectDivideBy4Or5Or6Or7Path = True;
			tLoSettings.bSelectDivideBy8Path = False;
			break;
	}

    if( bError != False )
    {
        /* configure LO */
        bError = TDA20152_SetLoConfig( ptCommunication, &tLoSettings );
    }

#ifdef FOR_DEBUG_PURPOSE
    /* LO frequency */
    stTunerContext.ulLoFreqkHz = ulFreqkHz;
    /* VCO frequency */
    stTunerContext.ulVcoFreqkHz = *pulNLo * ulFreqkHz;
#endif

    return bError;
}


/*****************************************************************************
 * CalculateRAndPremain()
 *
 * Description:
 *      Calculate R and Npremain values depending on NFloat
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ulNFloatBy10Max     Minimum allowaed value for NFloat
 * IN   ulNFloatBy10Min     Maximum allowaed value for NFloat
 * IN/OUT   pulNFloat       IN:  NFloat value without applying R and Npremain
 *                          OUT: NFloat value with R and Npremain applyed
 * IN   ulPrecision         Calculation precision of NFloat Min and Max
 * OUT  pulR                R ratio
 * OUT  pulPremain          Npremain ratio
 *****************************************************************************/
static BOOL CalculateRAndPremain( unsigned long ulNFloatBy10Max,
                                  unsigned long ulNFloatBy10Min,
                                  unsigned long *pulNFloat,
                                  unsigned long ulPrecision,
                                  unsigned long *pulR,
                                  unsigned long *pulPremain )
{
    BOOL bError = True;
    unsigned long ulNFloat;
    unsigned long ulNFloatBy10;

    /* calculate nfloat value used for range test */
    ulNFloatBy10 = (2* *pulNFloat) + ulPrecision;
    ulNFloatBy10 = (ulNFloatBy10/2) / ulPrecision;

    /* R and Premain are not calculated the same way */
    /* depending on the frequency */
    if( stTunerContext.ulXtalFreqHz <= 27000000 )
    {
        /* initial ratio 0.5 */
        *pulR = 1;
        *pulPremain = 2;
        ulNFloat = (*pulR * ulNFloatBy10) / *pulPremain;

        if( ulNFloat > ulNFloatBy10Max )
        {
            /* ratio 0.25 */
            *pulR = 1;
            *pulPremain = 4;
            ulNFloat = (*pulR * ulNFloatBy10) / *pulPremain;

            if( ulNFloat > ulNFloatBy10Max )
            {
                /* ratio 0.75 */
                *pulR = 3;
                *pulPremain = 4;
                ulNFloat = (*pulR * ulNFloatBy10) / *pulPremain;
            }
        }
        else if( ulNFloat < ulNFloatBy10Min )
        {
            /* ratio 1 */
            *pulR = 2;
            *pulPremain = 2;
            ulNFloat = (*pulR * ulNFloatBy10) / *pulPremain;

            if( ulNFloat > ulNFloatBy10Max )
            {
                /* ratio 0.75 */
                *pulR = 3;
                *pulPremain = 4;
                ulNFloat = (*pulR * ulNFloatBy10) / *pulPremain;
            }
            else if( ulNFloat < ulNFloatBy10Min )
            {
                /* ratio 1.5 */
                *pulR = 3;
                *pulPremain = 2;
                ulNFloat = (*pulR * ulNFloatBy10) / *pulPremain;
            }
        }
    }
    else
    {
        /* initial ratio 1 */
        *pulR = 2;
        *pulPremain = 2;
        ulNFloat = (*pulR * ulNFloatBy10) / *pulPremain;

        if( ulNFloat >= ulNFloatBy10Max )
        {
            /* ratio 0.5 */
            *pulR = 2;
            *pulPremain = 4;
            ulNFloat = (*pulR * ulNFloatBy10) / *pulPremain;

            if( ulNFloat < ulNFloatBy10Min )
            {
                 /* ratio 0.75 */
                *pulR = 3;
                *pulPremain = 4;
                ulNFloat = (*pulR * ulNFloatBy10) / *pulPremain;
            }
        }
        else if( ulNFloat < ulNFloatBy10Min )
        {
            /* ratio 1.5 */
            *pulR = 3;
            *pulPremain = 2;
            ulNFloat = (*pulR * ulNFloatBy10) / *pulPremain;

            if( ulNFloat > ulNFloatBy10Max )
            {
                /* ratio 0.75 */
                *pulR = 3;
                *pulPremain = 4;
                ulNFloat = (*pulR * ulNFloatBy10) / *pulPremain;
            }
        }
    }

    /* check if ulNfloat is in the range */
    if( ulNFloat < ulNFloatBy10Min || ulNFloat > ulNFloatBy10Max )
    {
        /* error: out of range */
        bError = False;
    }
    else
    {
        switch( *pulR*100 / *pulPremain )
        {
            case 25:
                *pulNFloat = *pulNFloat >> 2;
                break;
            case 50:
                *pulNFloat = *pulNFloat >> 1;
                break;
            case 75:
                *pulNFloat = (*pulNFloat >> 1) + (*pulNFloat >> 2);
                break;
            case 100:
                break;
            case 150:
                *pulNFloat = (*pulNFloat >> 1) + *pulNFloat;
                break;
            default:
                /* error */
                bError = False;
                break;
        }

        /* rounded value */
#ifndef DOUBLE_CALCULATION
        *pulNFloat = (2* *pulNFloat) + 10;
        *pulNFloat = (*pulNFloat/2) / 10;
        *pulNFloat *= 10;
#endif
    }

    return bError;
}


/*****************************************************************************
 * TDA20152_SetPllDividerConfig
 *
 * Description:
 *      Configure PLL Divider
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication       Pointer to blackwidow communication object.
 * IN   ptPllDividerSettings  PLL Divider configuration
 *
 *****************************************************************************/
BOOL TDA20152_SetPllDividerConfig( const TDA20152_COMMUNICATION*      ptCommunication,
                                const TDA20152_PLL_DIVIDER_CONFIG* ptPllDividerSettings )
{
    BOOL bError = True;

    if( (ptCommunication == 0) || (ptPllDividerSettings == 0) )
    {
        bError = False;
    }

    if( bError != False )
    {
        if( ptPllDividerSettings->ePredividerRatio < TDA20152_PLL_PREDIV_RATIO_INVALID )
        {
            /* configure division ratio for PreDivider (2 or 4) */
            bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PLL_EN_PREDIV_SEL,
                                      ptPllDividerSettings->ePredividerRatio );
        }
        else
        {
            bError = False;
        }
    }

    if( bError != False )
    {
        if( ptPllDividerSettings->ulDsmIntegerBits <= TDA20152_PLL1_DSM_INT_IN_MAX)
        {
            /* Integer bits to DSM for PLL frequency programming */
            bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PLL1_DSM_INT_IN,
                                      ptPllDividerSettings->ulDsmIntegerBits );
        }
        else
        {
            bError = False;
        }
    }

    if( bError != False )
    {
        if( ptPllDividerSettings->ulDsmFractionalBits <= TDA20152_PLL2_DSM_FRAC_IN_MAX)
        {
            /* Fractional bits for PLL frequency programming   */
            bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PLL2_DSM_FRAC_IN,
                                      ptPllDividerSettings->ulDsmFractionalBits );
        }
        else
        {
            bError = False;
        }
    }

    return( bError );
}


/*******************************************************************************************************
 * TDA20152_SetFreq()
 *
 * Descrition:
 *      This API is called to set the frequency, without calling the tuning machine routine
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * --------------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * IN   ulFreqKhz           Frequency in kHz
 *******************************************************************************************************/
BOOL TDA20152_SetFreq( const TDA20152_COMMUNICATION* ptCommunication,
					const unsigned long             ulFreqkHz,
					TDA20152_LO_DIVIDER           loDiv )
{
    BOOL bError = True;

    unsigned long ulPrecision;
    unsigned long ulBy10Precision;
    unsigned long ul_1;
    unsigned long ul_0_25;
    unsigned long ul_0_5;
    unsigned long ul_0_75;

    unsigned long ulNLo;

    TDA20152_DSM_MASH_MODE      eDsmOrder;
    TDA20152_DSM_FRAC           eDsmFrac;
    TDA20152_XTAL_REF_DIV       ePllRefClkRatio =TDA20152_XTAL_REF_DIV_INVALID;
    TDA20152_PLL_DIVIDER_CONFIG tPllDividerSettings;

    unsigned long ulR;
    unsigned long ulPremain;
    unsigned long ulNFloatBy10Min = 0;
    unsigned long ulNFloatBy10Max = 0;

    unsigned long uldfrac;

    unsigned long DsmIntInReg;
    unsigned long DsmFracInReg;

    unsigned long ulNFloat;

    unsigned long ulNum;
    unsigned long ulDenom;
    unsigned long ulIndex;
    unsigned long ulCalculPrecision;

    unsigned long ulNint;
    unsigned long ulkint;

#ifndef UINT32_CALCULATION
    unsigned long ulNumMsb;
    unsigned long ulNumLsb;
    unsigned long ulResMsb;
    unsigned long ulResLsb;
#endif

#ifdef DOUBLE_CALCULATION
    double dblNFloat;
    double dblNint;
    double dblkint;
#endif

#ifdef UINT32_CALCULATION
    unsigned long ulTmp;
    unsigned long ulPrecisionRes, ulPrecisionC;
#endif

#ifdef FOR_DEBUG_PURPOSE
    stTunerContext.ulTiming = (unsigned long)clock();
#endif

    if( ptCommunication == 0 )
    {
        bError = False;
    }

    /* check that xtal frequency has been set */
    if( stTunerContext.ulXtalFreqHz == 0)
    {
        bError = False;
    }

    /* LO frequency boundaries */
    if( (ulFreqkHz < TDA20152_LO_FREQ_MIN_KHZ) || (ulFreqkHz > TDA20152_LO_FREQ_MAX_KHZ) )
    {
        bError = False;
    }

    if( bError != False )
    {
        /* retrieve K and DSM order values */
        bError = TDA20152_GetPllDsmOrderAndFractCorrect(
            ptCommunication,
            &eDsmOrder,
            &eDsmFrac );
    }

    if( bError != False )
    {
        /* integer calculation settings */
        /* Precision of division */
        ulPrecision = 1000000;
        ulBy10Precision = ulPrecision / 10;

#ifdef UINT32_CALCULATION
        ulPrecision = 1000000;
        ulBy10Precision = ulPrecision / 10;
#endif

        /* fractional values matching with the precision */
        ul_1 = ulPrecision;
        ul_0_5  = ul_1 >> 1 ;
        ul_0_25 = ul_1 >> 2 ;
        ul_0_75 = ul_0_5  + ul_0_25;

        /**********/
        /* STEP 2 */
        /**********/
        /* Determine the LO divider ratio */
        bError = SetLoDivider( ptCommunication, ulFreqkHz, loDiv, &ulNLo );

        /******************************/
        /* STEP 5 - reference divider */
        /******************************/
        /* set NFloat threshold used in the algorithm */
        if( eDsmOrder == TDA20152_DSM_MASH_MODE_MASH11 )
        {
            /* DSM order 2 */
            if( eDsmFrac == TDA20152_DSM_FRAC_0P25_TO_0P75 )
            {
                /* Frational correction */
                ulNFloatBy10Min = TDA20152_N_FLOAT_BY_10_MIN_DSM_MASH11_FRAC;
                ulNFloatBy10Max = TDA20152_N_FLOAT_BY_10_MAX_DSM_MASH11_FRAC;
            }
            else
            {
                /* Integer correction */
                ulNFloatBy10Min = TDA20152_N_FLOAT_BY_10_MIN_DSM_MASH11_INT;
                ulNFloatBy10Max = TDA20152_N_FLOAT_BY_10_MAX_DSM_MASH11_INT;
            }
        }
        else
        {
            /* DSM order 3 */
            if( eDsmFrac == TDA20152_DSM_FRAC_0P25_TO_0P75 )
            {
                /* Frational correction */
                ulNFloatBy10Min = TDA20152_N_FLOAT_BY_10_MIN_DSM_MASH111_FRAC;
                ulNFloatBy10Max = TDA20152_N_FLOAT_BY_10_MAX_DSM_MASH111_FRAC;
            }
            else
            {
                /* Integer correction */
                ulNFloatBy10Min = TDA20152_N_FLOAT_BY_10_MIN_DSM_MASH111_INT;
                ulNFloatBy10Max = TDA20152_N_FLOAT_BY_10_MAX_DSM_MASH111_INT;
            }
        }

#ifdef UINT32_CALCULATION
        /******************************************/
        /* calculate Freq/XtalFreq (with 32 bits) */
        /*    a = Freq                            */
        /*    b = XtalFreq                        */
        /* ResI is the integer part of a/b        */
        /*  a/b = ResI + c/b                      */
        /*    c = a - ResI * b                    */
        /******************************************/
        ulNum = ulFreqkHz * 1000;
        ulDenom = stTunerContext.ulXtalFreqHz;

        ulNFloat = 0;
        ulPrecisionRes = 1;

        do
        {
            ulPrecisionC = 1;
            /* get precision */
            while( ((ulNum * ulPrecisionC) < 0x19999999) &&
                   ((ulPrecisionRes * ulPrecisionC) < ulPrecision) )
            {
                ulPrecisionC *= 10;
            }

            ulTmp = (ulNum * ulPrecisionC) / ulDenom;

            /* update result */
            ulNFloat = (ulNFloat * ulPrecisionC) + ulTmp;

            /* update resolution */
            ulPrecisionRes *= ulPrecisionC;

            /* update numerator for next iteration */
            ulNum = (ulNum * ulPrecisionC) - (ulTmp * ulDenom);

        } while( (ulPrecisionRes < ulPrecision) && (ulNum != 0) );

        /* adjust precision to match the calculation precision */
        if( ulPrecision > ulPrecisionRes )
        {
            ulNFloat = ulNFloat * (ulPrecision / ulPrecisionRes);
        }

        /* apply NLo */
        ulNFloat = ulNLo * ulNFloat;
#else
        /* BCD calculation */

        ulNum = ulNLo * ulFreqkHz * 100;
        ulDenom = stTunerContext.ulXtalFreqHz / 10;

        /* apply the precision */
        Mult( ulNum,
              ulPrecision,
              &ulNumMsb,
              &ulNumLsb );

        /* get the result of the division */
        Div( ulNumMsb, ulNumLsb,
             ulDenom,
             &ulResMsb, &ulResLsb );

        /* ulResMsb should be NULL ( concidering the precision ) */
        if( ulResMsb != 0 )
        {
            bError = False;
        }
        ulNFloat = ulResLsb;
#endif


#ifdef DOUBLE_CALCULATION
        /* calculate NFloat */
        /*  (LO_freq * N_LO) / Xtal_freq */
        dblNFloat = (double)(ulNLo * ulFreqkHz) * 1000 / (double)stTunerContext.ulXtalFreqHz;
        ulNFloat = (unsigned long)(dblNFloat * (double)ulPrecision);
#endif

        /*************************************/
        /* STEP 4 - evaluate R and Npre_main */
        /*************************************/
        /* Calculate R and PreMain */
        bError = CalculateRAndPremain( ulNFloatBy10Max, ulNFloatBy10Min,
                                       &ulNFloat, ulBy10Precision,
                                       &ulR, &ulPremain );
        switch( ulR )
        {
            case 1:
                 ePllRefClkRatio = TDA20152_XTAL_REF_DIV_BY_1;
                break;
            case 2:
                 ePllRefClkRatio = TDA20152_XTAL_REF_DIV_BY_2;
                break;
            case 3:
                 ePllRefClkRatio = TDA20152_XTAL_REF_DIV_BY_3;
                break;
            default:
                bError = False;
                break;
        }

        switch( ulPremain )
        {
            case 2:
                tPllDividerSettings.ePredividerRatio = TDA20152_PLL_PREDIV_RATIO_2;
                break;
            case 4:
                tPllDividerSettings.ePredividerRatio = TDA20152_PLL_PREDIV_RATIO_4;
                break;

            default:
                bError = False;
                break;
        }

#ifdef DOUBLE_CALCULATION
        /*calculate NFloat */
        dblNFloat = dblNFloat * ulR / ulPremain;
#endif

#ifdef FOR_DEBUG_PURPOSE
        stTunerContext.ulR = ulR;
        stTunerContext.ulPremain = ulPremain;

        /* compare interger and double values */
        stTunerContext.ulNFloat_ = ulNFloat;
#endif

        /************************************/
        /* STEP 6 - calculate Nint and kint */
        /************************************/
        /* initial value */
        uldfrac = 0;

#ifdef DOUBLE_CALCULATION
        if( eDsmFrac == TDA20152_DSM_FRAC_0P25_TO_0P75 )
        {
            dblNint = (int)(dblNFloat / 2);
            dblkint = (dblNFloat / 2) - dblNint;

            /* calculate kshift, dfrac and Nshift */
            /* to avoid cyclic errors and fractional spurs */
            if( dblkint > 0.75 )
            {
                uldfrac = 1;
                dblkint -= 0.5;
            }
            else if( dblkint < 0.25 )
            {
                uldfrac = 1;
                dblNint -= 1;
                dblkint += 0.5;
            }

            dblNint = (2*dblNint) + uldfrac - 128;
            dblkint = dblkint * 0x10000;
        }
        else
        {
            dblNint = (int)dblNFloat;
            dblkint = dblNFloat - dblNint;

            dblNint -= 128;
            dblkint *= 0x10000;
        }

        DsmIntInReg = (int)dblNint;
        DsmFracInReg = (int)dblkint;

#ifdef FOR_DEBUG_PURPOSE
        stTunerContext.DsmIntInRegCheck = (int)dblNint;
        stTunerContext.DsmFracInRegCheck = (int)dblkint;

        stTunerContext.DsmIntInReg = DsmIntInReg;
        stTunerContext.DsmFracInReg = DsmFracInReg;
#endif
#endif

        if( eDsmFrac == TDA20152_DSM_FRAC_0P25_TO_0P75 )
        {
            /* integer part of Nfloat divided by 2 */
            ulNint = (ulNFloat >> 1) / ul_1;
            /* fractional part of Nfloat divided by 2 */
            ulkint = (ulNFloat >> 1) % ul_1;

            /* rounded value */
            ulkint = (ulkint*2)+10;
            ulkint = ((ulkint/2)/10)*10;

            /* calculate kshift, dfrac and Nshift */
            /* to avoid cyclic errors and fractional spurs */
            uldfrac = 0;

            if( ulkint > ul_0_75 )
            {
                uldfrac = 1;
                ulkint = ulkint - ul_0_5;
            }
            else if( ulkint < ul_0_25 )
            {
                uldfrac = 1;
                ulNint -= 1;
                ulkint = ulkint + ul_0_5;
            }

            /* calculate dsm_int_in */
            DsmIntInReg = (ulNint << 1) + uldfrac;

            if( DsmIntInReg < 128 )
            {
                /* there is a problem in the calculation because the result can not be negative */
                bError = False;
            }
        }
        else
        {
            /* in case of no k correction */
            /* the settings are based on Nfloat and not Nfloat/2 */

            /* integer part */
            DsmIntInReg = ulNFloat / ul_1;

            /* fractional part */
            ulkint = ulNFloat % ul_1;
        }

        DsmIntInReg -= 128;

        /* Calculate dsm_frac_in: ulkint * 2^16 */
        /* For higher values, the result goes out of 32 bits */
        /* that's why the precision is ajusted depending of the result */
        ulCalculPrecision = ulPrecision;

        DsmFracInReg = ulkint;

        for( ulIndex = 0; ulIndex < 16; ulIndex ++ )
        {
            DsmFracInReg = 2*DsmFracInReg;
            if( (DsmFracInReg > 0x0FFFFFFF) && (ulIndex != 15) )
            {
                DsmFracInReg /= 10;
                ulCalculPrecision /= 10;
            }
        }

        DsmFracInReg = DsmFracInReg / ulCalculPrecision;

#ifdef FOR_DEBUG_PURPOSE
        stTunerContext.DsmIntInReg = DsmIntInReg;
        stTunerContext.DsmFracInReg = DsmFracInReg;
#endif
    }

    /***********************/
    /* configure the tuner */
    /***********************/
    if( bError != False )
    {
        /* set R register (division ratio for PLL reference clock) */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_XTAL2_REF_DIV_SEL, ePllRefClkRatio );
    }

    if( bError != False )
    {
        /* NPreMain register */
        /* Integer bits to DSM for PLL frequency programming */
        /* Fractional bits for PLL frequency programming */
        tPllDividerSettings.ulDsmIntegerBits = DsmIntInReg;
        tPllDividerSettings.ulDsmFractionalBits = DsmFracInReg;

        bError = TDA20152_SetPllDividerConfig( ptCommunication, &tPllDividerSettings );
    }

	return bError;
}

/*****************************************************************************
 * TDA20152_ProgramVcoChannelChange
 *
 * Description:
 *      Program VCO during Channel Change
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 *****************************************************************************/
BOOL TDA20152_ProgramVcoChannelChange ( const TDA20152_COMMUNICATION*  ptCommunication )
{
    BOOL bError = True;
    unsigned long ulRegValue;
    unsigned long ulRegMask;
    unsigned long ulPollingLimit = TDA20152_TM_LOCK_LO;

    if( ptCommunication == 0)
    {
        bError = False;
    }

    /************/
    /** step 4 **/
    /************/
	if( bError != False )
    {
        /* disable tuning machine clock */
        /* read registers */
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_CAL_CLK, &ulRegValue );
    }
	if( bError != False )
    {
        /* VCO_CAL_CLK */
        ulRegMask = TDA20152_VCO_CAL_CLK_CLKGEN_EN_WR(TDA20152_CLEAN);
        ulRegValue &= ~ulRegMask;

        ulRegValue |= TDA20152_VCO_CAL_CLK_CLCKGEN_INTCLK_WR(1);

        /* write registers */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_CAL_CLK, ulRegValue );
    }

	if( bError != False )
    {
        /* turn on tuning machine */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM1_TM_DELAY_S, 2 );
    }

    /*if( bError != False )
    {*/
        /* Change CALCLK rate if required */
        /* note: currently always set to the same value */
        /*bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_CAL_CLK_CLKGEN_SPDSEL, 2 );
    }*/

    if( bError != False )
    {
        /* turn on tuning machine */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM1_TM_VCOA_SPIC, 0 );
    }

    if( bError != False )
    {
        /* turn on tuning machine */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM1_TM_SPIC_EN, 1 );
    }

    /* write the register twice to be sure */
    if( bError != False )
    {
        /* turn on tuning machine */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM1_TM_SPIC_EN, 1 );
    }

    /* to be sure */
    if( bError != False )
    {
        /* turn on tuning machine */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM1_TM_VCOA_SPIC, 0 );
    }

	if( bError != False )
    {
        /* turn on tuning machine */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM1_TM_EN, 1 );
    }


    /************/
    /** step 5 **/
    /************/
    if( bError != False )
    {
        /* wait for PLL is locked */
        do{
            bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_TM_OUT_TM_LOCK_LO, &ulRegValue );

            /* avoid infinite loop */
            ulPollingLimit --;
        } while( (ulRegValue != 1) && (ulPollingLimit > 0));
    }

    if( ulRegValue == 1 )
    {
		if( bError != False )
		{
			/* turn off tuning machine */
			bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM1_TM_EN, 0 );
		}

        if( bError != False )
        {
            /* disable tuning machine clock */
            /* read registers */
            bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_CAL_CLK, &ulRegValue );
        }
        if( bError != False )
        {
            /* VCO_CAL_CLK */
            ulRegMask = TDA20152_VCO_CAL_CLK_CLKGEN_EN_WR(TDA20152_CLEAN);
            ulRegValue &= ~ulRegMask;

            /* write registers */
            bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_CAL_CLK, ulRegValue );
        }
    }

    return( bError );
}


/*******************************************************************************************************
 * TDA20152_SetFrequency()
 *
 * Descrition:
 *      This API is called to set the frequency
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * --------------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * IN   ulFreqKhz           Frequency in kHz
 *******************************************************************************************************/
BOOL TDA20152_SetFrequency( const TDA20152_COMMUNICATION* ptCommunication,
                         const unsigned long             ulFreqkHz )
{
#ifdef FOR_DEBUG_PURPOSE
    clock_t clkTime;
#endif
    BOOL bError = TDA20152_SetFreq(ptCommunication, ulFreqkHz, TDA20152_LO_DIVIDER_INVALID);

    if( bError != False )
    {
        /* wait 1us */
        TDA20152_Sleep( 1 );

        /* once the PLL is programmed */
        /* Program the VCO */
        bError = TDA20152_ProgramVcoChannelChange( ptCommunication );
    }

#ifdef FOR_DEBUG_PURPOSE

    clkTime = (clock_t)clock();

    if( (unsigned long)clkTime >= stTunerContext.ulTiming )
    {
        stTunerContext.ulTiming = (unsigned long)clkTime - stTunerContext.ulTiming;
    }
    else
    {
        stTunerContext.ulTiming = 0;
    }
#endif


    return bError;
}


/*****************************************************************************
 * TDA20152_SetBandwidthConfig()
 *
 * Description:
 *      Configure FLTA and GmC filters
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 * IN   eFilterBandwidth     FLTA filter value
 * IN   eGmcBandwidth        GmC filter value
 *****************************************************************************/
BOOL TDA20152_SetBandwidthConfig( const TDA20152_COMMUNICATION* ptCommunication,
                               const TDA20152_FLTA_BW        eFilterBandwidth,
                               const TDA20152_GMC_BW         eGmcBandwidth )
{
    BOOL bError = True;

    /* check parameter */
    if( ptCommunication == 0 )
    {
        bError = False;
    }

    /* configure FLTA */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_BB1_FLTA_BW, (unsigned long) eFilterBandwidth );
    }

    /* configure GmC */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_BB2_GMC_BW, (unsigned long) eGmcBandwidth );
    }

    return( bError );
}


/*******************************************************************************************************
 * TDA20152_SetFilterBandwidth()
 *
 * Descrition:
 *      This API is called to configure tuner bandwidth
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * --------------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * IN   ulBandwidthKhz      Bandwidth in kHz
 *******************************************************************************************************/
BOOL TDA20152_SetFilterBandwidth( const TDA20152_COMMUNICATION* ptCommunication,
                               const unsigned long             ulBandwidthKhz)
{
    BOOL bError = True;
	TDA20152_FLTA_BW eFilterBandwidth;
	unsigned long gmcBandwidthKhz, ulIndex;

	/* Set Filter A BW */
	if (ulBandwidthKhz >= 45000)
	{
		eFilterBandwidth = TDA20152_FLTA_BW_PRIMARY_70MHZ;
	}
	else if (ulBandwidthKhz >= 35000)
	{
		eFilterBandwidth = TDA20152_FLTA_BW_PRIMARY_45MHZ;
	}
	else if (ulBandwidthKhz >= 25000)
	{
		eFilterBandwidth = TDA20152_FLTA_BW_PRIMARY_35MHZ;
	}
	else
	{
		eFilterBandwidth = TDA20152_FLTA_BW_PRIMARY_25MHZ;
	}

	/* Set gmC Filter BW */
	gmcBandwidthKhz = ulBandwidthKhz - 2000;

	/* select the right Bandwidth for Gmc filter */
    ulIndex = 0;
    while ((gmcBandwidthKhz >= aGmcBandwidth[ulIndex].ulBandwidthKHz) && (ulIndex < (TDA20152_GMC_BANDWIDTH_TABLE_MAX - 1)))
    {
        ulIndex++;
    }

	bError = TDA20152_SetBandwidthConfig( ptCommunication, eFilterBandwidth, aGmcBandwidth[ulIndex].eGmcBandwidth );

    return( bError );
}
/******************************************************************************
 * CheckThresholdValidity()
 *
 * Description:
 *      Check that Threshold parameters are coherent
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters            Descriptions
 * ----------------------------------------------------------------------------
 * IN   ptThrehold            Pointer to threshold structure
 ******************************************************************************/
static BOOL CheckThresholdValidity(const TDA20152_PWR_DETECT*     ptThrehold)
{
    BOOL bError = True;

    if( ptThrehold->eThrehold1 >= TDA20152_PWR_DETECT_THERM_VAL_INVALID )
    {
        /* no need to check other values since they must be smaller */
        /* Input is out of range */
        bError = False;
    }

    if( ptThrehold->eThrehold1 < ptThrehold->eThrehold2 )
    {
        if( ptThrehold->eThrehold2 >= ptThrehold->eThrehold3 )
        {
            bError = False;
        }
    }
    else
    {
        bError = False;
    }

    return bError;
}

/**********************************************************************************
 * TDA20152_ConfigureGainMode()
 *
 * Description:
 *      This function makes a measurement using RF PWR DET
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * --------------------------------------------------------------------------------
 * IN   ptThrehold          structure containing thresholds
 * IN   ptGainModeConfiguration    gain configuration for each gain mode
 ***********************************************************************************/
BOOL TDA20152_ConfigureGainMode( const TDA20152_PWR_DETECT*       ptThrehold,
                              const TDA20152_GAIN_MODE_CONFIG* ptGainModeConfiguration )
{
    BOOL bError = True;

    if( (ptThrehold == 0) || (ptGainModeConfiguration == 0) )
    {
        bError = False;
    }

    /* Check that Threshold are coherent */
    if( bError != False )
    {
        bError = CheckThresholdValidity( ptThrehold );
    }

    /* those informations must be stored in a specific context */
    if( bError != False )
    {
        stTunerContext.tPwrDetectThreshold.eThrehold1 = ptThrehold->eThrehold1;
        stTunerContext.tPwrDetectThreshold.eThrehold2 = ptThrehold->eThrehold2;
        stTunerContext.tPwrDetectThreshold.eThrehold3 = ptThrehold->eThrehold3;

        stTunerContext.tGainModeConfig.tMode0 = ptGainModeConfiguration->tMode0;
        stTunerContext.tGainModeConfig.tMode1 = ptGainModeConfiguration->tMode1;
        stTunerContext.tGainModeConfig.tMode2 = ptGainModeConfiguration->tMode2;
        stTunerContext.tGainModeConfig.tMode3 = ptGainModeConfiguration->tMode3;
    }

    return bError;
}

/*****************************************************************************
 * TDA20152_GetPowerDetect()
 *
 * Description:
 *      Returns power detector temperature value
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication       Pointer to blackwidow communication object.
 * OUT  ptPowerDetect         PWR detect value
 *****************************************************************************/
BOOL TDA20152_GetPowerDetect( const TDA20152_COMMUNICATION* ptCommunication,
                                TDA20152_PWR_DETECT_THERM_VAL*      ptPowerDetect )
{
    BOOL          bError = True;
    unsigned long ulRegValue;
	unsigned long ulPwrDetEnable;

    /* retrieve the value from register */
	/* retrieve power detector status */
    /* 1 enable - 0 disable */
    bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PWR_DET_PWR_DET_EN, &ulPwrDetEnable);

    if( bError != False )
    {
        /* enable power detector if it is currently disable */
        if( ulPwrDetEnable == 0 )
        {
            bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PWR_DET_PWR_DET_EN, TDA20152_ENABLE );
        }
    }

    bError = TDA20152_SAT_TUNER_READ(ptCommunication, TDA20152_PWR_DET_PD_THERM, &ulRegValue);

    if( bError != False )
    {
        if( ulRegValue < TDA20152_PWR_DETECT_THERM_VAL_INVALID )
        {
           *ptPowerDetect = (TDA20152_PWR_DETECT_THERM_VAL)ulRegValue;
        }
        else
        {
            bError = False;
        }
    }

	/* disable power detector if it was previous disable */
    if( bError != False )
    {
        if( ulPwrDetEnable == 0 )
        {
            bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PWR_DET_PWR_DET_EN, TDA20152_DISABLE );
        }
    }

    return( bError );
}


/**********************************************************************************
 * TDA20152_GetGainMode()
 *
 * Description:
 *      This function makes a measurement using RF PWR DET
 *      and return the location of the result depending on thresholds defined as input.
 *      Following describes how modes are defined (TH is threshold input value and Mode is output)
 *             TH3     TH2     TH1
 *        Mode3 | Mode2 | Mode1 | Mode0
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * --------------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * IN   ptThrehold          structure containing thresholds
 * OUT  peGainMode          returns mode that match threshold values and PWR DET measure
 ***********************************************************************************/
BOOL TDA20152_GetGainMode( const TDA20152_COMMUNICATION*  ptCommunication,
                        const TDA20152_PWR_DETECT*     ptThrehold,
                        TDA20152_GAIN_MODE*            peGainMode )
{
    BOOL bError = True;
    TDA20152_PWR_DETECT_THERM_VAL  ePowerDetectValue;

    if( (ptCommunication == 0) || (ptThrehold == 0) || (peGainMode == 0) )
    {
        bError = False;
    }

    /* check threshold validity */
    /* threhold3 > threhold2 > threhold1 */
    if( bError != False )
    {
        bError = CheckThresholdValidity(ptThrehold);
    }

    /* Make the measurement */
    if( bError != False )
    {
        bError = TDA20152_GetPowerDetect( ptCommunication, &ePowerDetectValue );
    }

    if( bError != False )
    {
		eSelectedGainModePowerDetectValue = ePowerDetectValue;

        /* Determinate the current mode using following rule: */
        /*         TH3     TH2     TH1        */
        /*    Mode3 | Mode2 | Mode1 | Mode0   */

        if( ePowerDetectValue <= ptThrehold->eThrehold1 )
        {
            /* mode 0 */
            *peGainMode = TDA20152_GAIN_MODE_0;
        }
        else
        {
            if( ePowerDetectValue <= ptThrehold->eThrehold2 )
            {
                /* mode 1 */
                *peGainMode = TDA20152_GAIN_MODE_1;
            }
            else
            {
                if( ePowerDetectValue <= ptThrehold->eThrehold3 )
                {
                    /* mode 2 */
                    *peGainMode = TDA20152_GAIN_MODE_2;
                }
                else
                {
                    /* mode 3 */
                    *peGainMode = TDA20152_GAIN_MODE_3;
                }
            }
        }
    }

    return( bError );
}

/*****************************************************************************
 * TDA20152_SetGainLowCurrentConfig()
 *
 * Description:
 *      configure Low current mode for LNA, RF VGA, AMP OUT
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * IN   ptGainLowCurrent    Gain Values to apply
 *****************************************************************************/
BOOL TDA20152_SetGainLowCurrentConfig( const TDA20152_COMMUNICATION*    ptCommunication,
                                    const TDA20152_GAIN_LOW_CURRENT* ptGainLowCurrent )
{
    BOOL bError = True;
    unsigned long ulRegValue = 0;
    unsigned long ulRegMask = 0;

    /* check paramters */
    if( (ptCommunication == 0) || (ptGainLowCurrent == 0) )
    {
        bError = False;
    }

    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_ENABLE1, &ulRegValue );

        /* calculate mask to clean trageted fields */
        ulRegMask = TDA20152_ENABLE1_RF_VGA_LOWC_WR(TDA20152_CLEAN) |
                    TDA20152_ENABLE1_AMPOUT_CUR_WR(TDA20152_CLEAN) |
                    TDA20152_ENABLE1_RF_MIX_LOWC_WR(TDA20152_CLEAN) |
                    TDA20152_ENABLE1_LNA_HALFREF_EN_WR(TDA20152_CLEAN);

        /* set all bits to zero */
        ulRegValue &= ~ulRegMask;
    }

    if( bError != False )
    {
        /* RF VGA low Current */
        if( ptGainLowCurrent->bRfVgaLowCurrent == True )
        {
            /* set RF VGA to low current: register set to 1 */
            ulRegValue |= TDA20152_ENABLE1_RF_VGA_LOWC_WR(1);
        }

        /* AMP OUT low current */
        if( ptGainLowCurrent->bAmpOutLowCurrent != True )
        {
            /* set AMP OUT to high current: register set to 1 */
            ulRegValue |= TDA20152_ENABLE1_AMPOUT_CUR_WR(1);
        }

        /* Mixer low current */
        if( ptGainLowCurrent->bMixerLowCurrent == True )
        {
            /* set Mix to low current: register set to 1 */
            ulRegValue |= TDA20152_ENABLE1_RF_MIX_LOWC_WR(1);
        }
    }

    /* LNA low current                        */
    /*             lna_halfref_en  lna_lowc   */
    /*  Mode 1 ->     0               0       */
    /*  Mode 2 ->     0               1       */
    /*  Mode 3 ->     1               0       */
    /*  Mode 3 ->     1               1       */
    switch( ptGainLowCurrent->eLnaLowCurrent )
    {
        case  TDA20152_LNA_POWER_MODE_1:
            if( bError != False )
            {
                bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_FE_LNA_LOWC, 0 );
            }
            if( bError != False )
            {
                bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_ENABLE1, ulRegValue );
            }

            break;

        case  TDA20152_LNA_POWER_MODE_2:
            if( bError != False )
            {
                bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_FE_LNA_LOWC, 1 );
            }
                        if( bError != False )
            {
                bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_ENABLE1, ulRegValue );
            }
            break;

        case  TDA20152_LNA_POWER_MODE_3:
            if( bError != False )
            {
                bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_FE_LNA_LOWC, 0 );
            }
                        if( bError != False )
            {
                ulRegValue |= TDA20152_ENABLE1_LNA_HALFREF_EN_WR(1);
                bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_ENABLE1, ulRegValue );
            }
            break;

        case  TDA20152_LNA_POWER_MODE_4:
            if( bError != False )
            {
                bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_FE_LNA_LOWC, 1 );
            }
            if( bError != False )
            {
                ulRegValue |= TDA20152_ENABLE1_LNA_HALFREF_EN_WR(1);
                bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_ENABLE1, ulRegValue );
            }

            break;

        default:
            /* invalid parameter */
            bError = False;
            break;
    }

    return( bError );
}

/*****************************************************************************
 * TDA20152_SetGainConfig()
 *
 * Description:
 *      Gain configuration for
 *      LNA, LPT
 *      AMP OUT,
 *      RF VGA offset, VGA1 offset, VGA2 offset,
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * IN   ptGain              Pointer to Gain settings
 *****************************************************************************/
BOOL TDA20152_SetGainConfig( const TDA20152_COMMUNICATION*  ptCommunication,
                          TDA20152_GAIN*                 ptGain )
{
    BOOL bError = True;
    unsigned long ulRegValue;
    unsigned long ulRegMask;

    if( (ptCommunication == 0) || (ptGain == 0) )
    {
        bError = False;
    }

    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_FE, &ulRegValue );

        /* calculate mask to clean trageted fields */
        ulRegMask = TDA20152_FE_LNA_EN_WR(TDA20152_CLEAN) |
                    TDA20152_FE_LNA_GC_WR(TDA20152_CLEAN) |
                    TDA20152_FE_LPT_GC_WR(TDA20152_CLEAN) |
                    TDA20152_FE_LNA_HG_EN_WR(TDA20152_CLEAN);

        /* set all bits to zero */
        ulRegValue &= ~ulRegMask;
    }

    if( bError != False )
    {
        /* LNA */
        if( ptGain->eLnaGain == TDA20152_LNA_GAIN_NEGATIVE_11dB )
        {
            /* enable LNA high gain */
            ulRegValue |= TDA20152_FE_LNA_HG_EN_WR( TDA20152_ENABLE );

            /* note that LNA must be disable */
            /* by default it is set to disable*/
        }
        else
        {
            /* enable LNA */
            ulRegValue |= TDA20152_FE_LNA_EN_WR( TDA20152_ENABLE );

            if( ptGain->eLnaGain == TDA20152_LNA_GAIN_18dB )
            {
                /* Enable High gain mode in case of high gain */
                ulRegValue |= TDA20152_FE_LNA_HG_EN_WR( TDA20152_ENABLE );
            }

            /* apply LNA gain */
            if( ptGain->eLnaGain < TDA20152_LNA_GAIN_INVALID )
            {
                ulRegValue |= TDA20152_FE_LNA_GC_WR( (unsigned long)ptGain->eLnaGain );
            }
        }

        /* LPT */
        if( ptGain->eLptGain < TDA20152_LPT_GAIN_INVALID )
        {
            ulRegValue |= TDA20152_FE_LPT_GC_WR( (unsigned long)ptGain->eLptGain );
        }
        else
        {
            /* out of range */
            bError = False;
        }
    }

    /* fill FE register with updated value*/
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_FE, (unsigned long)ulRegValue );
    }

    if( bError != False )
    {
        ulRegValue = 0;
        ulRegMask = 0;

        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VGA, &ulRegValue );

        /* calculate mask to clean tragete */
        ulRegMask = TDA20152_VGA_RF_VGA_OFF_WR(TDA20152_CLEAN) |
                    TDA20152_VGA_VGA1_OFF_WR(TDA20152_CLEAN);

        /* set all bits to zero */
        ulRegValue &= ~ulRegMask;
    }

    if( bError != False )
    {
        /* RF VGA offset */
        if( ptGain->eRfvgaOffset < TDA20152_RFVGA_OFFSET_INVALID )
        {
            ulRegValue |= TDA20152_VGA_RF_VGA_OFF_WR( (unsigned long)ptGain->eRfvgaOffset );
        }
        else
        {
            /* out of range */
            bError = False;
        }

        /* VGA1 offset */
        if( ptGain->eVga1Offset < TDA20152_VGA1_OFFSET_INVALID )
        {
            ulRegValue |= TDA20152_VGA_VGA1_OFF_WR( (unsigned long)ptGain->eVga1Offset );
        }
        else
        {
            /* out of range */
            bError = False;
        }
    }

    /* fill VGA register with updated value*/
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VGA, (unsigned long)ulRegValue );
    }

    /* VGA2 offset */
    if( bError != False )
    {
        if( ptGain->eVga2Offset < TDA20152_VGA2_OFFSET_INVALID )
        {
            bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_BB1_VGA2_OFF, (unsigned long) ptGain->eVga2Offset  );
        }
        else
        {
            /* out of range */
            bError = False;
        }
    }

    /* AMPOUT */
    if( bError != False )
    {
        /* following test does not check every cases */
        if( ptGain->eAmpOut < TDA20152_AMPOUT_INVALID )
        {
            bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_BB3_AMPOUT, (unsigned long)ptGain->eAmpOut );
        }
        else
        {
            /* out of range */
            bError = False;
        }
    }

    return( bError );
}

/**********************************************************************************
 * TDA20152_SetGainMode()
 *
 * Description:
 *      This function allow to set the gain mode
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * --------------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * OUT  peGainMode          returns mode that match threshold values and PWR DET measure
 ***********************************************************************************/
BOOL TDA20152_SetGainMode( const TDA20152_COMMUNICATION*  ptCommunication,
                        TDA20152_GAIN_MODE             eGainMode )
{
    BOOL                        bError = True;
    TDA20152_GAIN_LOW_CURRENT tGainLowCurrent;
    TDA20152_GAIN             tGainModeConfiguration;

    if( ptCommunication == 0 )
    {
        bError = False;
    }

    if( bError != False )
    {
        /* configure gain settings depending on gain mode */
        /* with configuration recorded in the context */
        switch (eGainMode)
        {
            case TDA20152_GAIN_MODE_0:
                tGainLowCurrent = stTunerContext.tGainModeConfig.tMode0.tGainLowCurrent;
                tGainModeConfiguration = stTunerContext.tGainModeConfig.tMode0.tGainModeConfiguration;
                break;
            case TDA20152_GAIN_MODE_1:
                tGainLowCurrent = stTunerContext.tGainModeConfig.tMode1.tGainLowCurrent;
                tGainModeConfiguration = stTunerContext.tGainModeConfig.tMode1.tGainModeConfiguration;
                break;
            case TDA20152_GAIN_MODE_2:
                tGainLowCurrent = stTunerContext.tGainModeConfig.tMode2.tGainLowCurrent;
                tGainModeConfiguration = stTunerContext.tGainModeConfig.tMode2.tGainModeConfiguration;
                break;
            case TDA20152_GAIN_MODE_3:
                tGainLowCurrent = stTunerContext.tGainModeConfig.tMode3.tGainLowCurrent;
                tGainModeConfiguration = stTunerContext.tGainModeConfig.tMode3.tGainModeConfiguration;
                break;
            default:
                bError = False;
                break;
        }
    }

    if( bError != False )
    {
        bError = TDA20152_SetGainLowCurrentConfig( ptCommunication, &tGainLowCurrent );
    }

    if( bError != False )
    {
        bError = TDA20152_SetGainConfig( ptCommunication, &tGainModeConfiguration );
    }

	if( bError != False )
    {
		eSelectedGainMode = eGainMode;
	}

    return bError;
}

/**********************************************************************************
 * TDA20152_SetGain()
 *
 * Description:
 *      This function set the gain mode according to PowerDetectorValue
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * --------------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 ***********************************************************************************/
BOOL TDA20152_SetGain( const TDA20152_COMMUNICATION*  ptCommunication )
{
    BOOL bError = True;
    TDA20152_GAIN_MODE   eGainMode;
    TDA20152_PWR_DETECT  tThrehold;

    if( ptCommunication == 0)
    {
        bError = False;
    }

    if( bError != False )
    {
        /* retrieve the threshold stored in the context*/
        tThrehold.eThrehold1 = stTunerContext.tPwrDetectThreshold.eThrehold1;
        tThrehold.eThrehold2 = stTunerContext.tPwrDetectThreshold.eThrehold2;
        tThrehold.eThrehold3 = stTunerContext.tPwrDetectThreshold.eThrehold3;

        /* retrieve measurement */
        bError = TDA20152_GetGainMode( ptCommunication, &tThrehold, &eGainMode );
    }

    if( bError != False )
    {
        /* set gain according to the mode */
        bError = TDA20152_SetGainMode( ptCommunication, eGainMode );
    }

    return bError;
}


/*****************************************************************************
 * TDA20152_GetSelectedGainMode()
 *
 * Description:
 *      Get the gain mode selected from calling TDA20152_SetGain or
 *      TDA20152_SetGainMode and the power detect value
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * OUT   peGainMode          gain mode selected
 * OUT   pePowerDetectValue  power detect value
 *****************************************************************************/
BOOL TDA20152_GetSelectedGainMode( TDA20152_GAIN_MODE* peGainMode, TDA20152_PWR_DETECT_THERM_VAL* pePowerDetectValue )
{
	*peGainMode = eSelectedGainMode;
	*pePowerDetectValue = eSelectedGainModePowerDetectValue;
	return True;
}

/*****************************************************************************
 * TDA20152_GetVcoStatus()
 *
 * Description:
 *      Returns current VCO status
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 * OUT  ptStatus            VCO Status
 *****************************************************************************/
BOOL TDA20152_GetVcoStatus( const TDA20152_COMMUNICATION* ptCommunication,
                         TDA20152_VCO_STATUS*          ptStatus )
{
    BOOL            bError = True;
    unsigned long   ulRegValue;

    if( (ptCommunication == 0) || (ptStatus == 0) )
    {
        bError = False;
    }

    if( bError != False )
    {
        /* Bias value */
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_ALC2_ALC_BIAS, &ulRegValue );
        ptStatus->ulBiasValue = ulRegValue;
    }

    /* VCO_ALC_OUT */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_ALC_OUT, &ulRegValue );
    }

    if( bError != False )
    {
        /* Calibration finished */
        ptStatus->bCalibrationDone = (BOOL)TDA20152_VCO_ALC_OUT_ALC_DONE_RD( ulRegValue );

        /* indicate amplitude change */
        ptStatus->bAlcAmplitudeStable = (BOOL)TDA20152_VCO_ALC_OUT_ALC_AM_STABLE_RD( ulRegValue );

        /* V power detector amplitude */
        ptStatus->eVpdAmplitude = (TDA20152_VPD_AMPLITUDE)TDA20152_VCO_ALC_OUT_ALC_AM_LOW_RD( ulRegValue );

        /* indicate whether calibration has been done over 16 ticks */
        ptStatus->bCalibrationOvertime = (BOOL)TDA20152_VCO_ALC_OUT_ALC_T16_OVER_RD( ulRegValue );

        /* calibration hits counter limit */
        ptStatus->bCalibrationHitLimit = (BOOL)TDA20152_VCO_ALC_OUT_ALC_HIT_LIMIT_RD( ulRegValue );
    }

    /* VLC TM OUT */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_TM_OUT, &ulRegValue );
    }

    if( bError != False )
    {
        /* Vtune is higher then specified threshold at high side */
        ptStatus->bVTuneAboveThreshold = (BOOL)TDA20152_VCO_TM_OUT_TM_VT_HIGH_RD( ulRegValue );

        /* Vtune is lower then specified threshold at high side */
        ptStatus->bVTuneBelowThreshold = (BOOL)TDA20152_VCO_TM_OUT_TM_VT_LOW_RD( ulRegValue );

        /* which VCO is enabled
         *   | VCO B | VCO A |
         * 0 |       |   x   |
         * 1 |   x   |       |
         */
        ptStatus->eVcoEnabled = (TDA20152_VCO)TDA20152_VCO_TM_OUT_VCOB_IND_RD( ulRegValue );

        /* Vtune is in the right range to provide margin */
        ptStatus->bVTuneMargin = (BOOL)TDA20152_VCO_TM_OUT_TM_LOCK_LO_RD( ulRegValue );
    }

    if( bError != False )
    {
        /* manual configuration */
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_ALC1_ALC_SPIC_EN, &ulRegValue );
        ptStatus->bBiasControlBySpiEnable = (BOOL)ulRegValue;
    }

    /* VLC TM1 */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_TM1, &ulRegValue );
    }

    if( bError != False )
    {
        ptStatus->bVcoSelectionBySpi = (BOOL)TDA20152_VCO_TM1_TM_VCOA_SPIC_RD( ulRegValue );

        ptStatus->bTuningMachineEnabledBySpi = (BOOL)TDA20152_VCO_TM1_TM_SPIC_EN_RD( ulRegValue );

        ptStatus->bTuningMachineClockEnableBySpi = (BOOL)TDA20152_VCO_TM1_TM_EN_RD( ulRegValue );
    }

    return( bError );
}

/******************************************************************************
 * GetLoDivider()
 *
 * Description:
 *      returns the LoDivider value
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters            Descriptions
 * ----------------------------------------------------------------------------
 * IN   ptCommunication       Pointer to blackwidow communication object.
 * OUT  peLoDivider           Pointer to LO divider enum
 ******************************************************************************/
static BOOL GetLoDivider( const TDA20152_COMMUNICATION* ptCommunication,
              TDA20152_LO_DIVIDER*          peLoDivider )
{
    BOOL                      bError = True;
    TDA20152_LO_SETTINGS    tLoSettings;

    /* default value */
    *peLoDivider = TDA20152_LO_DIVIDER_INVALID;

    /* retrieve LO settings */
    bError = TDA20152_GetLoConfig( ptCommunication, &tLoSettings );

    /* check parameter coherency */
    if( bError != False )
    {
        if( tLoSettings.bSelectDivideBy8Path == True &&
            tLoSettings.bSelectDivideBy4Or5Or6Or7Path == True )
        {
            bError = False;
        }

        if( tLoSettings.bSelectDivideBy8Path == False &&
            tLoSettings.bSelectDivideBy4Or5Or6Or7Path == False )
        {
            bError = False;
        }
    }

    /* determine LO divider */
    if( bError != False )
    {
        if( tLoSettings.bSelectDivideBy8Path == True )
        {
            // div by 8
            *peLoDivider = TDA20152_LO_DIVIDER_8;
        }
        else
        {
            if( tLoSettings.bDiv1ConfigInDivideBy3 == True &&
                tLoSettings.bDiv2ConfigInDivideBy3 == True )
            {
                // div by 7
                *peLoDivider = TDA20152_LO_DIVIDER_7;
            }
            else if( tLoSettings.bDiv1ConfigInDivideBy3 == False &&
                     tLoSettings.bDiv2ConfigInDivideBy3 == True )
            {
                // div by 6
                *peLoDivider = TDA20152_LO_DIVIDER_6;
            }
            else if( tLoSettings.bDiv1ConfigInDivideBy3 == True &&
                     tLoSettings.bDiv2ConfigInDivideBy3 == False )
            {
                // div by 5
                *peLoDivider = TDA20152_LO_DIVIDER_5;
            }
            else if( tLoSettings.bDiv1ConfigInDivideBy3 == False &&
                     tLoSettings.bDiv2ConfigInDivideBy3 == False )
            {
                // div by 4
                *peLoDivider = TDA20152_LO_DIVIDER_4;
            }
        }
    }

    return bError;
}

/*****************************************************************************
 * TDA20152_GetStatus
 *
 * Description:
 *      returns tuner status
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 * OUT  ptStatus             return tuner status (PLL and VCO)
 *****************************************************************************/
BOOL TDA20152_GetStatus( const TDA20152_COMMUNICATION* ptCommunication,
                           TDA20152_STATUS*              ptStatus )
{
    BOOL                    bError = True;
    TDA20152_VCO_STATUS   tVcoStatus;
    unsigned long           ulRegValue;

    if( (ptCommunication == 0) || (ptStatus == 0) )
    {
        bError = False;
    }

    /* read PLL lock status bit */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PLL_TEST2_PLL_LOCK_DET, &ulRegValue );
    }

    /* fill the output structure */
    if( bError != False )
    {
        if( ulRegValue == TDA20152_ENABLE )
        {
            ptStatus->bPllLocked = True;
        }
        else
        {
            ptStatus->bPllLocked = False;
        }
    }

    /* Vco Status */
    if( bError != False )
    {
        TDA20152_GetVcoStatus( ptCommunication, &tVcoStatus);
    }

    /* fill the output structure */
    if( bError != False )
    {
        ptStatus->bCalibrationDone = tVcoStatus.bCalibrationDone;
        ptStatus->bAlcAmplitudeStable = tVcoStatus.bAlcAmplitudeStable;
        ptStatus->eVpdAmplitude = tVcoStatus.eVpdAmplitude;
        ptStatus->bCalibrationOvertime = tVcoStatus.bCalibrationOvertime;
        ptStatus->bCalibrationHitLimit = tVcoStatus.bCalibrationHitLimit;
        ptStatus->bVTuneAboveThreshold = tVcoStatus.bVTuneAboveThreshold;
        ptStatus->bVTuneBelowThreshold = tVcoStatus.bVTuneBelowThreshold;
        ptStatus->eVcoEnabled = tVcoStatus.eVcoEnabled;
        ptStatus->bVTuneMargin = tVcoStatus.bVTuneMargin;
    }

    /* Retrieve LO status */
    if( bError != False )
    {
        bError = GetLoDivider( ptCommunication, &(ptStatus->eLoDividerRatio) );
    }

#ifdef FOR_DEBUG_PURPOSE
    if( bError != False )
    {
        ptStatus->ulLoFreqkHz = stTunerContext.ulLoFreqkHz;
        ptStatus->ulVcoFreqkHz = stTunerContext.ulVcoFreqkHz;
        ptStatus->ulNFloat = stTunerContext.ulNFloat_;
        ptStatus->DsmIntInReg = stTunerContext.DsmIntInReg;
        ptStatus->DsmIntInRegCheck = stTunerContext.DsmIntInRegCheck;
        ptStatus->DsmFracInReg = stTunerContext.DsmFracInReg;
        ptStatus->DsmFracInRegCheck = stTunerContext.DsmFracInRegCheck;
        ptStatus->ulR = stTunerContext.ulR;
        ptStatus->ulPremain  = stTunerContext.ulPremain;
        ptStatus->ulTiming = stTunerContext.ulTiming;
    }
#endif
    return( bError );
}

/*****************************************************************************
 * TDA20152_GetPllDividerConfig
 *
 * Description:
 *      Returns PLL divider configuration
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication       Pointer to blackwidow communication object.
 * OUT  ptPllDividerSettings  returns PLL divider configuration
 *
 *****************************************************************************/
BOOL TDA20152_GetPllDividerConfig( const TDA20152_COMMUNICATION*   ptCommunication,
                                TDA20152_PLL_DIVIDER_CONFIG*    ptPllDividerSettings )
{
    BOOL          bError = True;
    unsigned long ulRegValue;

    if( (ptCommunication == 0) || (ptPllDividerSettings == 0) )
    {
        bError = False;
    }

    if( bError != False )
    {
        /* configure division ratio for PreDivider (2 or 4) */
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PLL_EN_PREDIV_SEL, &ulRegValue );
        ptPllDividerSettings->ePredividerRatio = (TDA20152_PLL_PREDIV_RATIO)ulRegValue;
    }

    if( bError != False )
    {
        /* Integer bits to DSM for PLL frequency programming */
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PLL1_DSM_INT_IN, &ulRegValue );
        ptPllDividerSettings->ulDsmIntegerBits = ulRegValue;
    }

    if( bError != False )
    {
        /* Fractional bits for PLL frequency programming   */
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PLL2_DSM_FRAC_IN, &ulRegValue );
        ptPllDividerSettings->ulDsmFractionalBits = ulRegValue;
    }

    return( bError );
}


/**********************************************************************************
 * TDA20152_GetPLLFrequency()
 *
 * Description:
 *      Returns PLL frequency
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * --------------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * OUT  pulPllFrequencyKhz  Current PLL frequency in kHz
 ***********************************************************************************/
BOOL TDA20152_GetPLLFrequency( const TDA20152_COMMUNICATION* ptCommunication,
                            unsigned long*                  pulPllFrequencyKhz )
{
    BOOL      bError = True;

    TDA20152_PLL_DIVIDER_CONFIG tPllDividerSettings;
    TDA20152_XTAL_REF_DIV       ePllRefClkRatio;
    TDA20152_LO_SETTINGS        tLoSettings;

    TDA20152_DSM_MASH_MODE      eDsmOrder;
    TDA20152_DSM_FRAC           eDsmFrac;

    unsigned long ulRegister;
    unsigned long ulNLo;
    unsigned long ulTmpLsb;

    unsigned long ulFractional;
    unsigned long ulNFloat;
    unsigned long ulResolutionMax;
    unsigned long ulResolution;
    unsigned long ulIndex;

#ifdef DOUBLE_CALCULATION
    double dblNFloat;
#endif

#ifdef UINT32_CALCULATION
    unsigned long ulXtalFreq;
    unsigned long ulXtalFreqDiv;
    unsigned long ulNFloatRes;
    unsigned long ulNFloatTmp;
#else
    unsigned long ulNFloatLsb;
    unsigned long ulTmpMsb;
    unsigned long ulNFloatMsb;
#endif

    if( (ptCommunication == 0) || (pulPllFrequencyKhz == 0) )
    {
        bError = False;
    }

    if( bError != False )
    {
        /* retrive current LO settings to get LO divider ratio */
        bError = TDA20152_GetLoConfig( ptCommunication, &tLoSettings );

        /* calculate ulNLo */
        ulNLo = 4 + ((tLoSettings.bDiv1ConfigInDivideBy3 == False) ? 0 : 1) +
                    ((tLoSettings.bDiv2ConfigInDivideBy3 == False) ? 0 : 2) +
                    ((tLoSettings.bSelectDivideBy8Path == False) ? 0 : 4);
    }

    if( bError != False )
    {
        /* retrieve K and DSM order values */
        bError = TDA20152_GetPllDsmOrderAndFractCorrect(
            ptCommunication,
            &eDsmOrder,
            &eDsmFrac );
    }

    if( bError != False )
    {
        /* retrieve PLL configuration */
        bError = TDA20152_GetPllDividerConfig( ptCommunication, &tPllDividerSettings );
    }

    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_XTAL2_REF_DIV_SEL, &ulRegister );
        ePllRefClkRatio = (TDA20152_XTAL_REF_DIV)ulRegister;
    }

    if( bError != False )
    {
        /* calculate ulFloat */
        /* evalutate maximum resolution with integer part */
        ulNFloat = tPllDividerSettings.ulDsmIntegerBits + 128;

        ulResolutionMax = 1;
        while( (ulNFloat * ulResolutionMax) <= 0x19999999 )
        {
            ulResolutionMax *= 10;
            ulNFloat *= 10;
        }

        /* get fractional part */
        ulFractional = tPllDividerSettings.ulDsmFractionalBits;

        ulResolution = 1;
        do{
            ulFractional *= 10;
            ulResolution *= 10;
        } while ( (ulFractional < 0x19999999) && (ulResolution < ulResolutionMax) );

        /* divide fractional part by 2pow16 */
        for( ulIndex = 0; ulIndex < 16; ulIndex ++ )
        {
            ulFractional = ulFractional >> 1;

            if( (ulFractional < 0x19999999) && (ulResolution < ulResolutionMax) )
            {
                /* increment the resolution */
                /* but reduce the precision */
                ulFractional *= 10;
                ulResolution *= 10;
            }
        }

        /* calculate NFloat */
        if( eDsmFrac == TDA20152_DSM_FRAC_0P25_TO_0P75 )
        {
            ulNFloat += (ulFractional * 2);
        }
        else
        {
            ulNFloat += ulFractional;
        }

#ifdef DOUBLE_CALCULATION
        /* fractionnal part */
        dblNFloat = (double)tPllDividerSettings.ulDsmFractionalBits / (double)0x10000;

        /* if K correction is enable, multiply by 2 */
        if( eDsmFrac == TDA20152_DSM_FRAC_0P25_TO_0P75 )
        {
            dblNFloat *= 2;
        }

        /* add integer part */
        dblNFloat += (tPllDividerSettings.ulDsmIntegerBits + 128);
#endif

        /* Apply R and Npre main */
        /* first multiply by premain */
        /* then divide by R */
        if( tPllDividerSettings.ePredividerRatio == TDA20152_PLL_PREDIV_RATIO_2 )
        {
            switch( ePllRefClkRatio )
            {
                case TDA20152_XTAL_REF_DIV_BY_1:
                    /* ratio is 2 */
                    ulNFloat = ulNFloat << 1;
#ifdef DOUBLE_CALCULATION
                    dblNFloat *= 2;
#endif
                    break;

                case TDA20152_XTAL_REF_DIV_BY_2:
                    /* ratio is one */
                    break;

                case TDA20152_XTAL_REF_DIV_BY_3:
                    /* ratio is  2/3 */

                    ulNFloat = ulNFloat << 1;
                    ulNFloat /= 3;
#ifdef DOUBLE_CALCULATION
                    dblNFloat *= 2;
                    dblNFloat /= 3;
#endif
                    break;

                case TDA20152_XTAL_REF_DIV_BY_4:
                    /* ratio is 0.5 */
                    /* but it should never happen */
                    ulNFloat = ulNFloat >> 1;
#ifdef DOUBLE_CALCULATION
                    dblNFloat /= 2;
#endif
                    break;
				default:
					break;
            }
        }
        else
        {
            switch( ePllRefClkRatio )
            {
                case TDA20152_XTAL_REF_DIV_BY_1:
                    /* ratio is 4 */
                    ulNFloat = ulNFloat << 2;
#ifdef DOUBLE_CALCULATION
                    dblNFloat *= 4;
#endif
                    break;
                case TDA20152_XTAL_REF_DIV_BY_2:
                    /* ratio is 2 */
                    ulNFloat = ulNFloat << 1;
#ifdef DOUBLE_CALCULATION
                    dblNFloat *= 2;
#endif
                    break;
                case TDA20152_XTAL_REF_DIV_BY_3:
                    /* ratio is 4 / 3 */
                    ulNFloat = ulNFloat << 2;
                    ulNFloat /= 3;
#ifdef DOUBLE_CALCULATION
                    dblNFloat *= 4;
                    dblNFloat /= 3;
#endif
                    break;
                case TDA20152_XTAL_REF_DIV_BY_4:
                    /* ratio is one */
                    /* but it should never happen */
                    break;
				default:
					break;
            }
        }

        /* calculate the Frequency multiplied by resolution  */
        /* NFloat = (Freq * NLo) / Xtal Freq */
        /* so Freq = (NFloat / NLo) * Xtal Freq */

#ifdef UINT32_CALCULATION

        ulNFloat /= ulNLo;

        ulNFloatRes = 0;
        ulXtalFreqDiv = 100000000;

        do{
            ulNFloatTmp = ulNFloat % ulXtalFreqDiv;

            ulXtalFreq = stTunerContext.ulXtalFreqHz;
            ulXtalFreqDiv = 1;

            /* Xtal frequency precision */
            while( ulNFloatTmp >= (0xFFFFFFFF / ulXtalFreq) )
            {
                ulXtalFreq /= 10;
                ulXtalFreqDiv *= 10;
            }

            ulNFloatTmp = ulNFloatTmp/ulXtalFreqDiv;
            ulNFloatTmp *= stTunerContext.ulXtalFreqHz;

            ulNFloatTmp /= ulResolution;
            ulNFloatTmp *= ulXtalFreqDiv;

            ulNFloatRes += ulNFloatTmp;

        } while( ulXtalFreqDiv != 1);

        ulTmpLsb = ulNFloatRes;
#else
        /* BSD calculation */

        ulNFloat /= ulNLo;
        Mult( ulNFloat, stTunerContext.ulXtalFreqHz, &ulNFloatMsb, &ulNFloatLsb);

        /* get the result in Hz */
        Div( ulNFloatMsb, ulNFloatLsb, ulResolution, &ulTmpMsb, &ulTmpLsb );
#endif

#ifdef DOUBLE_CALCULATION
        dblNFloat /= (double) ulNLo;
        dblNFloat *= (double)stTunerContext.ulXtalFreqHz;
        ulTmpLsb = (unsigned long)dblNFloat;
#endif


        /* then in KHz */
        *pulPllFrequencyKhz = ulTmpLsb / 1000;
    }

    return( bError );
}

/******************************************************************************
 * TDA20152_GetPLLLockStatus()
 *
 * Description:
 *      return PLL configuration
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ----------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * OUT  pbPllLockStatus     PLL Lock Status
 ******************************************************************************/
BOOL TDA20152_GetPLLLockStatus( const TDA20152_COMMUNICATION* ptCommunication,
                             BOOL*                           pbPllLockStatus )
{
    BOOL      bError = True;
    unsigned long ulRegValue;

    if( (ptCommunication == 0) || (pbPllLockStatus == 0) )
    {
        bError = False;
    }

    /* read PLL lock status bit */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_TM_OUT_TM_LOCK_LO, &ulRegValue );
    }

    /* fill the output structure */
    if( bError != False )
    {
        if( ulRegValue == TDA20152_ENABLE )
        {
            *pbPllLockStatus = True;
        }
        else
        {
            *pbPllLockStatus = False;
        }
    }

    return( bError );
}

/******************************************************************************
 * TDA20152_GetPowerMode()
 *
 * Description:
 *      returns power configuration
 *      (High Power, Low Power, Sleep, Power Down)
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ----------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * OUT  pePowerMode         current power mode
 ******************************************************************************/
BOOL TDA20152_GetPowerMode( const TDA20152_COMMUNICATION* ptCommunication,
                              TDA20152_POWER_MODE*          pePowerMode )
{
    BOOL          bError = True;
    unsigned long ulRegValue = 0;
	unsigned char xtal1Reg = 0,
 				  feReg = 0,
 				  enable1Reg= 0,
 				  vcoAlc1Reg=0,
 				  pfdCp1Reg=0,
 				  pllEnReg=0,
 				  loChain1Reg=0,
 				  loChain4Reg=0,
 				  digLdoReg=0;

	*pePowerMode = TDA20152_POWER_MODE_MANUAL;

    if( bError != False )
	{
		bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_XTAL1, &ulRegValue );
		xtal1Reg = (unsigned char)ulRegValue;
	}

	if( bError != False )
	{
		bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_FE, &ulRegValue );
		feReg = (unsigned char)ulRegValue;
	}

	if( bError != False )
	{
		bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_ENABLE1, &ulRegValue );
		enable1Reg = (unsigned char)ulRegValue;
	}

	if( bError != False )
	{
		bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_ALC1, &ulRegValue );
		vcoAlc1Reg = (unsigned char)ulRegValue;
	}

	if( bError != False )
	{
		bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PFD_CP1, &ulRegValue );
		pfdCp1Reg = (unsigned char)ulRegValue;
	}

	if( bError != False )
	{
		bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PLL_EN, &ulRegValue );
		pllEnReg = (unsigned char)ulRegValue;
	}

	if( bError != False )
	{
		bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_LO_CHAIN1, &ulRegValue );
		loChain1Reg = (unsigned char)ulRegValue;
	}

	if( bError != False )
	{
		bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_LO_CHAIN4, &ulRegValue );
		loChain4Reg = (unsigned char)ulRegValue;
	}

	if( bError != False )
	{
		bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_DIG_LDO, &ulRegValue );
		digLdoReg = (unsigned char)ulRegValue;
	}

	if (((xtal1Reg & 0x81) == 0x81) && ((feReg & 0x89) == 0x89) && ((enable1Reg & 0xAE) == 0xAE) &&
		((vcoAlc1Reg & 0x80) == 0x80) && ((pfdCp1Reg & 0x80) == 0x80) && ((pllEnReg & 0xC0) == 0xC0) &&
		(loChain1Reg == 0xFF) && ((loChain4Reg & 0x08) == 0x08) && ((digLdoReg & 0xE0) == 0xC0) )
	{
		if( bError != False )
		{
			*pePowerMode = TDA20152_POWER_MODE_HIGH_POWER;
		}
	}
	else if (((xtal1Reg & 0x01) == 0x00) && ((feReg & 0x89) == 0x00) && ((enable1Reg & 0xAF) == 0x00) &&
			((vcoAlc1Reg & 0x80) == 0x00) && ((pfdCp1Reg & 0x80) == 0x00) && ((pllEnReg & 0xC0) == 0x00) &&
			(loChain1Reg == 0x00) && (loChain4Reg == 0x00) && ((digLdoReg & 0xE0) == 0x00) )
	{
		if ((xtal1Reg & 0x80) == 0x80)
		{
			*pePowerMode = TDA20152_POWER_MODE_SLEEP;
		}
		else
		{
			*pePowerMode = TDA20152_POWER_MODE_POWER_DOWN;
		}
	}

    return( bError );
}

/*****************************************************************************
 * TDA20152_SetFrequencyAndBandwidth
 *
 * Description:
 *      Program the tuner with frequency, symbol rate and roll-off Factor
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 * IN   ptChannelObj         configuration
 *****************************************************************************/
BOOL TDA20152_SetFrequencyAndBandwidth (const TDA20152_COMMUNICATION* ptCommunication,
									 const TDA20152_SAT_TUNER_CHANOBJ*        ptChannelObj)
{
    BOOL bError = True;
    unsigned long ulFilterRollOffConst;
    unsigned long ulBandwidthKhz;

    if( (ptCommunication == 0) || (ptChannelObj == 0) )
    {
        bError = False;
    }

	/* set frequency */
    if( bError != False )
    {
        bError = TDA20152_SetFrequency( ptCommunication, ptChannelObj->frequency_khz );
    }

    if( bError != False )
    {
        /* filter roll off */
        switch( ptChannelObj->roll_off )
        {
            case TDA20152_SAT_TUNER_ROLLOFF_02:
                ulFilterRollOffConst = 120;
                break;
            case TDA20152_SAT_TUNER_ROLLOFF_025:
                ulFilterRollOffConst = 125;
                break;
            case TDA20152_SAT_TUNER_ROLLOFF_035:
                ulFilterRollOffConst = 135;
                break;
            default:
                ulFilterRollOffConst = 120;
                break;
        }

		/* bandwidth = ((symbol rate * (1 + rolloff) / 2) + search range) / .72 + 2 */
        ulBandwidthKhz = (((ptChannelObj->symbol_rate_ksps * ulFilterRollOffConst) / 2UL) +
                           (unsigned long)ptChannelObj->requested_search_range_khz * 100) / 72 + 2000UL;

		if( bError != False )
		{
			bError = TDA20152_SetFilterBandwidth( ptCommunication, ulBandwidthKhz);
		}
    }

	return( bError );
}

/*****************************************************************************
 * TDA20152_Tune
 *
 * Description:
 *      Program the tuner with frequency, symbol rate and roll-off Factor
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 * IN   ptChannelObj         configuration
 *****************************************************************************/
BOOL TDA20152_Tune (const TDA20152_COMMUNICATION* ptCommunication,
                 const TDA20152_SAT_TUNER_CHANOBJ*        ptChannelObj)
{
	BOOL bError =  TDA20152_SetFrequencyAndBandwidth(ptCommunication, ptChannelObj);

    if( bError != False )
    {
        /* set gain with predefined modes */
        bError = TDA20152_SetGain(ptCommunication);
    }

    return( bError );
}

/*****************************************************************************
 * TDA20152_EnableLoopThrough
 *
 * Description:
 *      Program the tuner with frequency, symbol rate and roll-off Factor
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 * IN   ptChannelObj         configuration
 *****************************************************************************/
BOOL TDA20152_EnableLoopThrough( const TDA20152_COMMUNICATION* ptCommunication,
								   BOOL                            bEnable)
{
	BOOL bError = True;
	unsigned long ulRegValue = 0;

    /* check parameters */
    if( ptCommunication == 0 )
    {
        bError = False;
    }

    if( bError != False )
    {
        /* retrieve FE register value */
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_FE, &ulRegValue );
    }

    if( bError != False )
    {
        /* LPT */
        // 1 enable
        // 0 disable
		ulRegValue &= 0xF7; /* clear the lpt_en bit */
        ulRegValue |= TDA20152_FE_LPT_EN_WR( bEnable ); /* set the lpt_en bit */

        /* update FE register */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_FE, ulRegValue );
    }

	return( bError );
}

/*****************************************************************************/
/* INTERNAL FUNCTION  DEFINITION                                             */
/*****************************************************************************/
/* note prototype is not present in nay header file */
/* so declaration of the function must be added in the source file */

/******************************************************************************
 * TDA20152_GetXtalFreqHz()
 *
 * Description:
 *      Returns XTAL frequency setting
 *
 * Return Value:
 *      XTAL frequency setting
 *
 * Parameters:
 * I/O  Parameters            Descriptions
 * ----------------------------------------------------------------------------
 * none
 ******************************************************************************/
unsigned long TDA20152_GetXtalFreqHz(void)
{
    return stTunerContext.ulXtalFreqHz;
}

/******************************************************************************
 * TDA20152_SetXtalFreqHz()
 *
 * Description:
 *      Set XTAL frequency setting
 *
 * Return Value:
 *      XTAL frequency setting
 *
 * Parameters:
 * I/O  Parameters            Descriptions
 * ----------------------------------------------------------------------------
 * IN   freq_hz               Xtal frequency
 ******************************************************************************/
void TDA20152_SetXtalFreqHz( unsigned long ulFreqHz )
{
    stTunerContext.ulXtalFreqHz = ulFreqHz;
    return;
}

/*****************************************************************************
 * TDA20152_SetEnableBlocks()
 *
 * Description:
 *      Enable following HW blocks
 *      LNA, LPT, RF VGA, RF MIXER, BaseBand, DC offset, Charge pump, power detector
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * IN   ptEnableBlocks      Defines blocks to enable
 *****************************************************************************/
BOOL TDA20152_SetEnableBlocks( const TDA20152_COMMUNICATION* ptCommunication,
                            const TDA20152_ENABLE_BLOCKS* ptEnableBlocks )
{
    BOOL bError = True;
    unsigned long ulRegValue = 0;
    unsigned long ulRegMask = 0;

    /* check parameters */
    if( ptCommunication == 0 || ptEnableBlocks == 0 )
    {
        bError = False;
    }

    if( bError != False )
    {
        /* retrieve FE register value */
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_FE, &ulRegValue );

        /* calculate mask to clean tragete */
        ulRegMask = TDA20152_FE_LNA_EN_WR(TDA20152_CLEAN) |
                    TDA20152_FE_LPT_EN_WR(TDA20152_CLEAN);

        /* set all bits to zero */
        ulRegValue &= ~ulRegMask;
    }

    if( bError != False )
    {
        /* LNA */
        // 1 enable
        // 0 disable
        ulRegValue |= TDA20152_FE_LNA_EN_WR( ptEnableBlocks->bLna );

        /* LPT */
        // 1 enable
        // 0 disable
        ulRegValue |= TDA20152_FE_LPT_EN_WR( ptEnableBlocks->bLpt );

        /* update FE register */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_FE, ulRegValue );
    }

    if( bError != False )
    {
        ulRegValue = 0;
        ulRegMask = 0;

        /* retrieve FE register value */
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_ENABLE1, &ulRegValue );

        /* calculate mask to clean trageted register */
        ulRegMask = TDA20152_ENABLE1_RF_VGA_EN_WR(TDA20152_CLEAN) |
                    TDA20152_ENABLE1_RF_MIX_EN_WR(TDA20152_CLEAN) |
                    TDA20152_ENABLE1_BB_EN_WR(TDA20152_CLEAN) |
                    TDA20152_ENABLE1_SERVO_EN_WR(TDA20152_CLEAN);

        /* set all bits to zero */
        ulRegValue &= ~ulRegMask;
    }

    if( bError != False )
    {
        /* Enable RF VGA */
        // 1 enable
        // 0 disable
        ulRegValue |= TDA20152_ENABLE1_RF_VGA_EN_WR( ptEnableBlocks->bRfvga );

        /* RF MIXER */
        // 1 enable
        // 0 disable
        ulRegValue |= TDA20152_ENABLE1_RF_MIX_EN_WR( ptEnableBlocks->bRfMixer );

        /* BaseBand */
        // 1 enable
        // 0 disable
        ulRegValue |= TDA20152_ENABLE1_BB_EN_WR( ptEnableBlocks->bBaseband );

        /* enable DC offset */
        // 1 enable
        // 0 disable
        ulRegValue |= TDA20152_ENABLE1_SERVO_EN_WR( ptEnableBlocks->bDcOffset );

        /* update FE register */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_ENABLE1, ulRegValue );
    }

    /* enable charge pump */
    if( bError != False )
    {
        // 1 enable
        // 0 disable
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PFD_CP1_CP_EN, ptEnableBlocks->bChargePump );
    }

    /* enable power detector */
    if( bError != False )
    {
        // 1 enable
        // 0 disable
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PWR_DET_PWR_DET_EN, ptEnableBlocks->bPowerDetect );
    }

    /* Vco */
    if( bError != False )
    {
        // 1 enable
        // 0 disable
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_ALC1_IVREF_EN, ptEnableBlocks->bVco );
    }

    /* PLL EN register  */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PLL_EN, &ulRegValue );

        ulRegMask = TDA20152_PLL_EN_SX_BIAS_EN_WR(TDA20152_CLEAN) |
                    TDA20152_PLL_EN_PLL_DIVIDERS_EN_WR(TDA20152_CLEAN);

        ulRegValue &= ~ulRegMask;
    }

    if( bError != False )
    {
        /* enable BIAS core */
        ulRegValue |= TDA20152_PLL_EN_SX_BIAS_EN_WR(
                                 (unsigned long)ptEnableBlocks->bBiasCoreEnable );

        /* enable pre-divider and main-divider */
        ulRegValue |= TDA20152_PLL_EN_PLL_DIVIDERS_EN_WR(
                                 (unsigned long)ptEnableBlocks->bEnableBothDividers );

        /* update register */
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PLL_EN, ulRegValue );
    }

    return( bError );
}

/*****************************************************************************
 * TDA20152_GetEnableBlocks()
 *
 * Description:
 *      Indicates enabled HW blocks
 *      LNA, LPT, RF VGA, RF MIXER, BaseBand, DC offset, Charge pump, power detector
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * OUT  ptEnableBlocks      Enabled blocks
 *****************************************************************************/
BOOL TDA20152_GetEnableBlocks( const TDA20152_COMMUNICATION* ptCommunication,
                            TDA20152_ENABLE_BLOCKS*       ptEnableBlocks )
{
    BOOL bError = True;
    unsigned long ulRegValue = 0;

    /* check parameters */
    if( ptCommunication == 0 || ptEnableBlocks == 0 )
    {
        bError = False;
    }

    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_FE, &ulRegValue );
    }

    if( bError != False )
    {
        /* LNA */
        // 1 enable
        // 0 disable
        ptEnableBlocks->bLna = TDA20152_FE_LNA_EN_RD(ulRegValue);

        /* LPT */
        // 1 enable
        // 0 disable
        ptEnableBlocks->bLpt = TDA20152_FE_LPT_EN_RD(ulRegValue);
    }

    if( bError != False )
    {
        ulRegValue = 0;
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_ENABLE1, &ulRegValue );
    }

    if( bError != False )
    {
        /* Enable RF VGA */
        // 1 enable
        // 0 disable
        ptEnableBlocks->bRfvga = TDA20152_ENABLE1_RF_VGA_EN_RD(ulRegValue);

        /* RF MIXER */
        // 1 enable
        // 0 disable
        ptEnableBlocks->bRfMixer = TDA20152_ENABLE1_RF_MIX_EN_RD(ulRegValue);

        /* BaseBand */
        // 1 enable
        // 0 disable
        ptEnableBlocks->bBaseband = TDA20152_ENABLE1_BB_EN_RD(ulRegValue);

        /* enable DC offset */
        // 1 enable
        // 0 disable
        ptEnableBlocks->bDcOffset = TDA20152_ENABLE1_SERVO_EN_RD(ulRegValue);
    }

    /* enable charge pump */
    if( bError != False )
    {
        // 1 enable
        // 0 disable
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PFD_CP1_CP_EN, &ulRegValue );
    }
	if( bError != False )
    {
		ptEnableBlocks->bChargePump = (BOOL)ulRegValue;
	}

    /* enable power detector */
    if( bError != False )
    {
        // 1 enable
        // 0 disable
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PWR_DET_PWR_DET_EN, &ulRegValue );
    }
	if( bError != False )
    {
		ptEnableBlocks->bPowerDetect = (BOOL)ulRegValue;
	}

    /* Vco */
    if( bError != False )
    {
        // 1 enable
        // 0 disable
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_ALC1_IVREF_EN, &ulRegValue );
    }
	if( bError != False )
    {
		ptEnableBlocks->bVco = (BOOL)ulRegValue;
	}

    /* PLL configuration */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PLL_EN, &ulRegValue );
    }

    if( bError != False )
    {
        /* enable BIAS core */
        ptEnableBlocks->bBiasCoreEnable = (BOOL)TDA20152_PLL_EN_SX_BIAS_EN_RD( ulRegValue );

        /* enable pre-divider and main-divider */
        ptEnableBlocks->bEnableBothDividers = (BOOL)TDA20152_PLL_EN_PLL_DIVIDERS_EN_RD( ulRegValue );
    }

    return( bError );
}

/*****************************************************************************
 * TDA20152_SetPowerDetectorConfig()
 *
 * Description:
 *      Configure Gain shift used for Power Detector measures
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * IN   eGainShiftValue     Gain Shift Value
 *****************************************************************************/
BOOL TDA20152_SetPowerDetectorConfig( const TDA20152_COMMUNICATION*      ptCommunication,
                                   const TDA20152_PWR_DETECT_SHIFT    eGainShiftValue )
{
    BOOL bError = True;

    /* check that value is not out of boundary */
    if( eGainShiftValue < TDA20152_PWR_DETECT_SHIFT_INVALID )
    {
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_PWR_DET_PWR_DET_CTRL, (unsigned long)eGainShiftValue );
    }
    else
    {
        /* out of range */
        bError = False;
    }

    return( bError );
}



/*****************************************************************************
 * TDA20152_GetGainLowCurrentConfig()
 *
 * Description:
 *      Returns low current status for LNA, RF VGA, AMP OUT
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication     Pointer to blackwidow communication object.
 * OUT  ptGainLowCurrent    Current Gain Values
 *****************************************************************************/
BOOL TDA20152_GetGainLowCurrentConfig( const TDA20152_COMMUNICATION*  ptCommunication,
                                    TDA20152_GAIN_LOW_CURRENT*     ptGainLowCurrent )
{
    BOOL          bError = True;
    unsigned long ulRegValue;

    /* check paramters */
    if( (ptCommunication == 0) || (ptGainLowCurrent == 0) )
    {
        bError = False;
    }

    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_ENABLE1, &ulRegValue );
    }

    if( bError != False )
    {
        /* RF VGA low Current */
        if( TDA20152_ENABLE1_RF_VGA_LOWC_RD( ulRegValue ) == 1 )
        {
            /* RF VGA in low current: register set to 1 */
            ptGainLowCurrent->bRfVgaLowCurrent = True;
        }
        else
        {
            /* RF VGA in normal current: register set to 0 */
            ptGainLowCurrent->bRfVgaLowCurrent = False;
        }

        /* AMP OUT low Current */
        if( TDA20152_ENABLE1_AMPOUT_CUR_RD( ulRegValue ) == 0 )
        {
            /* AMP OUT in low current: register set to 0 */
            ptGainLowCurrent->bAmpOutLowCurrent = True;
        }
        else
        {
            /* AMP OUT in high current: register set to 1 */
            ptGainLowCurrent->bAmpOutLowCurrent = False;
        }

        /* Mixer low current */
        if( TDA20152_ENABLE1_RF_MIX_LOWC_RD( ulRegValue ) == 0 )
        {
            /* RF mixer in high current: register set to 0 */
            ptGainLowCurrent->bMixerLowCurrent = False;
        }
        else
        {
            /* RF mixer in low current: register set to 1 */
            ptGainLowCurrent->bMixerLowCurrent = True;
        }
    }

    /* LNA low current                        */
    /*             lna_halfref_en  lna_lowc   */
    /*  Mode 1 ->     0               0       */
    /*  Mode 2 ->     0               1       */
    /*  Mode 3 ->     1               0       */
    /*  Mode 3 ->     1               1       */

    /* retrieve regiters values */
    if( TDA20152_ENABLE1_LNA_HALFREF_EN_RD( ulRegValue ) == 0 )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_FE, &ulRegValue );

        if( bError != False )
        {
            if( TDA20152_FE_LNA_LOWC_RD( ulRegValue) == 0 )
            {
                ptGainLowCurrent->eLnaLowCurrent = TDA20152_LNA_POWER_MODE_1;
            }
            else
            {
                ptGainLowCurrent->eLnaLowCurrent = TDA20152_LNA_POWER_MODE_2;
            }
        }
    }
    else
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_FE, &ulRegValue );

        if( bError != False )
        {
            if( TDA20152_FE_LNA_LOWC_RD( ulRegValue) == 0 )
            {
                ptGainLowCurrent->eLnaLowCurrent = TDA20152_LNA_POWER_MODE_3;
            }
            else
            {
                ptGainLowCurrent->eLnaLowCurrent = TDA20152_LNA_POWER_MODE_4;
            }
        }
    }

    return( bError );
}



/*****************************************************************************
 * TDA20152_SetXtalSettings()
 *
 * Description:
 *      Configure the xtal oscillator
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 * IN   ptXtalSettings      cristal configuration
 *****************************************************************************/
BOOL TDA20152_SetXtalSettings( const TDA20152_COMMUNICATION* ptCommunication,
                            TDA20152_XTAL_SETTINGS*       ptXtalSettings )
{
    BOOL bError = True;
    unsigned long ulRegVal = 0;
    unsigned long ulRegMask = 0;

    if( (ptXtalSettings == 0) || (ptCommunication == 0) )
    {
        bError = False;
    }

    /* XTAL1 */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_XTAL1, &ulRegVal );

        ulRegMask = TDA20152_XTAL1_XTAL_EN_WR(TDA20152_CLEAN) |
                    TDA20152_XTAL1_XTAL_SU_BOOST_DIS_WR(TDA20152_CLEAN) |
                    TDA20152_XTAL1_XTAL_ALC_DIS_WR(TDA20152_CLEAN) |
                    TDA20152_XTAL1_XTAL_ALC_LVL_CTRL_WR(TDA20152_CLEAN) |
                    TDA20152_XTAL1_XTAL_EXT_SQUARWAVE_FORCE_WR(TDA20152_CLEAN) |
                    TDA20152_XTAL1_XTAL_EXT_INPUT_EN_WR(TDA20152_CLEAN) |
                    TDA20152_XTAL1_PLL_REF_EN_WR(TDA20152_CLEAN);

        ulRegVal &= ~ulRegMask;
    }

    if( bError != False )
    {
        /* enable crystal oscillator */
        if( ptXtalSettings->bXtalOscEnable == True )
        {
            /* Enable -> set to 1 */
            ulRegVal |= TDA20152_XTAL1_XTAL_EN_WR(TDA20152_ENABLE);
        }
        else
        {
            /* Disable -> set to 0 */
            /* ulRegVal = TDA20152_XTAL1_XTAL_EN_WR(TDA20152_DISABLE); */
        }

        /* start up boost enable */
        if( ptXtalSettings->bXtalStartupBoostEnable == True )
        {
            /* Enable -> set to 0 */
            /* ulRegVal |= TDA20152_XTAL1_XTAL_SU_BOOST_DIS_WR(0); */
        }
        else
        {
            /* Disable -> set to 1 */
            ulRegVal |= TDA20152_XTAL1_XTAL_SU_BOOST_DIS_WR(1);
        }

        /* Enable Auto Level Control */
        if( ptXtalSettings->bXtalAlcEnable == True )
        {
            /* Enable -> set to 0 */
            /* ulRegVal |= TDA20152_XTAL1_XTAL_ALC_DIS_WR(0); */
        }
        else
        {
            /* Disable -> set to 1 */
            ulRegVal |= TDA20152_XTAL1_XTAL_ALC_DIS_WR(1);
        }

        /* Automatic level control value */
        if( ptXtalSettings->eXtalAlcLevel < TDA20152_XTAL_ALC_LVL_CNTL_INVALID )
        {
            ulRegVal |= TDA20152_XTAL1_XTAL_ALC_LVL_CTRL_WR( (unsigned long)ptXtalSettings->eXtalAlcLevel );
        }
        else
        {
            /* Out of boundary value */
            bError = False;
        }

        /* in the XTAL Osc output to external output path do not use divider and filter */
        /*      note: divider and filter are not shut down */
        /*      note: if bExtClockEnable is disabled, no ext output */
        if( ptXtalSettings->bForceSquareWave == True )
        {
            /* Forced -> set to 1 */
            ulRegVal |= TDA20152_XTAL1_XTAL_EXT_SQUARWAVE_FORCE_WR(1);
        }
        else
        {
            /* Do not force -> set to 0 */
            /* ulRegVal |= TDA20152_XTAL1_XTAL_EXT_SQUARWAVE_FORCE_WR(0); */
        }

        /* to be enable when the input is not a Xtal but a ref clk from another tuner */
        if( ptXtalSettings->bXtalExtInputEnable == True )
        {
            /* external input -> set to 1 */
            ulRegVal |= TDA20152_XTAL1_XTAL_EXT_INPUT_EN_WR(1);
        }
        else
        {
            /* use the Xtal -> set to 0 */
            /* ulRegVal |= TDA20152_XTAL1_XTAL_EXT_INPUT_EN_WR(0); */
        }

        /* Enable XTAL Osc output to PLL path */
        if( ptXtalSettings->bPllRefEnable == True )
        {
            /* Enable -> set to 1 */
            ulRegVal |= TDA20152_XTAL1_PLL_REF_EN_WR(1);
        }
        else
        {
            /* Disable -> set to 0 */
            /* ulRegVal |= TDA20152_XTAL1_PLL_REF_EN_WR(0); */
        }

        /* Fill register */
        if( bError != False )
        {
            bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_XTAL1, ulRegVal );
        }
    }

    /* XTAL2 */
    if( bError != False )
    {
        ulRegMask = 0;
        ulRegVal = 0;

        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_XTAL2, &ulRegVal );

        ulRegMask = TDA20152_XTAL2_REF_DIV_SEL_WR(TDA20152_CLEAN) |
                    TDA20152_XTAL2_EXT_CLK_FILT_SEL_WR(TDA20152_CLEAN) |
                    TDA20152_XTAL2_EXT_DIV_SEL_WR(TDA20152_CLEAN) |
                    TDA20152_XTAL2_EXT_CLK_EN_WR(TDA20152_CLEAN) |
                    TDA20152_XTAL2_EXT_CLK_OVR_WR(TDA20152_CLEAN);

        ulRegVal &= ~ulRegMask;
    }

    /* Configure divider between XTAL Osc output and PLL */
    if( bError != False )
    {
        if( ptXtalSettings->eXtalRefDivider < TDA20152_XTAL_REF_DIV_INVALID )
        {
            ulRegVal |= TDA20152_XTAL2_REF_DIV_SEL_WR( (unsigned long)ptXtalSettings->eXtalRefDivider );
        }
        else
        {
            /* Out of boundary value */
            bError = False;
        }

        /* Configure divider between XTAL Osc output and external output */
        if( ptXtalSettings->eXtalExtFilter < TDA20152_XTAL_EXT_FLT_INVALID )
        {
            ulRegVal |= TDA20152_XTAL2_EXT_CLK_FILT_SEL_WR( (unsigned long)ptXtalSettings->eXtalExtFilter );
        }
        else
        {
            /* Out of boundary value */
            bError = False;
        }

        /* Configure filter  between XTAL Osc output and external output */
        if( ptXtalSettings->eXtalExtDivider < TDA20152_XTAL_EXT_DIV_INVALID )
        {
            ulRegVal |= TDA20152_XTAL2_EXT_DIV_SEL_WR( (unsigned long)ptXtalSettings->eXtalExtDivider );
        }
        else
        {
            /* Out of boundary value */
            bError = False;
        }

        /* Enable divider, filter and mux between XTAL Osc output and external output */
        if( ptXtalSettings->bExtClockEnable == True )
        {
            /* Enable -> set to 1 */
            ulRegVal |= TDA20152_XTAL2_EXT_CLK_EN_WR( TDA20152_ENABLE );
        }
        else
        {
            /* Disbale -> set to 0 */
            /* ulRegVal |= TDA20152_XTAL2_EXT_CLK_EN_WR( TDA20152_DISABLE ); */
        }

        /* extDivider is driven by ExtPin when disabled */
        if( ptXtalSettings->bExtClockOverride == True )
        {
            /* Enable -> set to 1 */
            ulRegVal |= TDA20152_XTAL2_EXT_CLK_OVR_WR( TDA20152_ENABLE );
        }
        else
        {
            /* Disbale -> set to 0 */
            /* ulRegVal |= TDA20152_XTAL2_EXT_CLK_OVR_WR( TDA20152_DISABLE ); */
        }

        if( bError != False )
        {
            bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_XTAL2, ulRegVal );
        }
    }

    /* ALC high-pass to low frequency */
    if( bError != False )
    {
        if( ptXtalSettings->eAlcHighPathToLowFreq < TDA20152_XTAL_HPF_LOW_FREQ_INVALID )
        {
            bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_SPI_CTRL_XTAL_HPF_LOW_FREQ,
                         (unsigned long)ptXtalSettings->eAlcHighPathToLowFreq );
        }
    }

    return( bError );
}

/*****************************************************************************
 * TDA20152_GetXtalSettings()
 *
 * Description:
 *      Get the xtal oscillator
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 * OUT  ptXtalSettings       crystal configuration
 *****************************************************************************/
BOOL TDA20152_GetXtalSettings( const TDA20152_COMMUNICATION* ptCommunication,
                            TDA20152_XTAL_SETTINGS*       ptXtalSettings )
{
    BOOL bError = True;
    unsigned long ulRegVal = 0;

    if( (ptXtalSettings == 0) || (ptXtalSettings == 0) )
    {
        bError = False;
    }

    /* XTAL1 */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_XTAL1, &ulRegVal );
    }

    if( bError != False )
    {
        /* enable crystal oscillator */
        if( TDA20152_XTAL1_XTAL_EN_RD( ulRegVal ) == TDA20152_ENABLE )
        {
            /* Enable -> set to 1 */
            ptXtalSettings->bXtalOscEnable = True;
        }
        else
        {
            /* Disable -> set to 0 */
            ptXtalSettings->bXtalOscEnable = False;
        }

        /* start up boost enable */
        if( TDA20152_XTAL1_XTAL_SU_BOOST_DIS_RD(ulRegVal) == 0 )
        {
            /* Enable -> set to 0 */
            ptXtalSettings->bXtalStartupBoostEnable = True;
        }
        else
        {
            /* Disable -> set to 1 */
            ptXtalSettings->bXtalStartupBoostEnable = False;
        }

        /* Enable Auto Level Control */
        if( TDA20152_XTAL1_XTAL_ALC_DIS_RD(ulRegVal) == 0 )
        {
            /* Enable -> set to 0 */
            ptXtalSettings->bXtalAlcEnable = True;
        }
        else
        {
            /* Disable -> set to 1 */
            ptXtalSettings->bXtalAlcEnable = False;
        }

        /* Automatic level control value */
        ptXtalSettings->eXtalAlcLevel = (TDA20152_XTAL_ALC_LVL_CNTL)TDA20152_XTAL1_XTAL_ALC_LVL_CTRL_RD( ulRegVal );

        /* in the XTAL Osc output to external output path do not use divider and filter */
        /*      note: divider and filter are not shut down */
        /*      note: if bExtClockEnable is disabled, no ext output */
        if( TDA20152_XTAL1_XTAL_EXT_SQUARWAVE_FORCE_RD( ulRegVal ) == 1 )
        {
            /* Forced -> set to 1 */
            ptXtalSettings->bForceSquareWave = True;
        }
        else
        {
            /* Do not force -> set to 0 */
            ptXtalSettings->bForceSquareWave = False;
        }

        /* to be enable when the input is not a Xtal but a ref clk from another tuner */
        if(  TDA20152_XTAL1_XTAL_EXT_INPUT_EN_RD(ulRegVal) == 1 )
        {
            /* external input -> set to 1 */
            ptXtalSettings->bXtalExtInputEnable = True;
        }
        else
        {
            /* use the Xtal -> set to 0 */
            ptXtalSettings->bXtalExtInputEnable = False;
        }

        /* Enable XTAL Osc output to PLL path */
        if( TDA20152_XTAL1_PLL_REF_EN_RD(ulRegVal) == 1 )
        {
            /* Enable -> set to 1 */
            ptXtalSettings->bPllRefEnable = True;
        }
        else
        {
            /* Disable -> set to 0 */
            ptXtalSettings->bPllRefEnable = False;
        }
    }

    /* XTAL1 */
    if( bError != False )
    {
        ulRegVal = 0;

        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_XTAL2, &ulRegVal );
    }

    /* Configure divider between XTAL Osc output and PLL */
    if( bError != False )
    {
        ptXtalSettings->eXtalRefDivider = (TDA20152_XTAL_REF_DIV)TDA20152_XTAL2_REF_DIV_SEL_RD( ulRegVal );

        /* Configure divider between XTAL Osc output and external output */
        ptXtalSettings->eXtalExtFilter = (TDA20152_XTAL_EXT_FLT)TDA20152_XTAL2_EXT_CLK_FILT_SEL_RD( ulRegVal );

        /* Configure filter  between XTAL Osc output and external output */
        ptXtalSettings->eXtalExtDivider = (TDA20152_XTAL_EXT_DIV)TDA20152_XTAL2_EXT_DIV_SEL_RD( ulRegVal );

        /* Enable divider, filter and mux between XTAL Osc output and external output */
        if( TDA20152_XTAL2_EXT_CLK_EN_RD(ulRegVal) == TDA20152_ENABLE )
        {
            ptXtalSettings->bExtClockEnable = True;
        }
        else
        {
            ptXtalSettings->bExtClockEnable = False;
        }

        /* extDivider is driven by ExtPin when disabled */
        if( TDA20152_XTAL2_EXT_CLK_OVR_RD( ulRegVal ) == TDA20152_ENABLE )
        {
            /* Enable -> set to 1 */
            ptXtalSettings->bExtClockOverride = True;
        }
        else
        {
            /* Disbale -> set to 0 */
           ptXtalSettings->bExtClockOverride = False;
        }
    }

    /* ALC high-pass to low frequency */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_SPI_CTRL_XTAL_HPF_LOW_FREQ,
                                 &ulRegVal );
        ptXtalSettings->eAlcHighPathToLowFreq = (TDA20152_XTAL_HPF_LOW_FREQ)ulRegVal;
    }

    return( bError );
}

/*****************************************************************************
 * TDA20152_SetXtalExtClkSettings()
 *
 * Description:
 *      Configure the xtal external clock settings
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 * IN   extClkOvr            Override the hardwired control of the external clock.
 * IN   extClkEn             Enable/disable the external divider, filter and output.
 * IN   extClkFiltSel        External clock divider ratio.
 * IN   extDivSel            External clock filter bandwidth.
 *****************************************************************************/
BOOL TDA20152_SetXtalExtClkSettings( const TDA20152_COMMUNICATION* ptCommunication,
                            BOOL extClkOvr,
							BOOL extClkEn,
							TDA20152_XTAL_EXT_FLT extClkFiltSel,
							TDA20152_XTAL_EXT_DIV extDivSel)
{
    BOOL bError = True;
    unsigned long ulRegVal = 0;
    unsigned long ulRegMask = 0;

    if( ptCommunication == 0 )
    {
        bError = False;
    }

    /* XTAL2 */
    if( bError != False )
    {
        ulRegMask = 0;
        ulRegVal = 0;

        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_XTAL2, &ulRegVal );

        ulRegMask = TDA20152_XTAL2_EXT_CLK_FILT_SEL_WR(TDA20152_CLEAN) |
                    TDA20152_XTAL2_EXT_DIV_SEL_WR(TDA20152_CLEAN) |
                    TDA20152_XTAL2_EXT_CLK_EN_WR(TDA20152_CLEAN) |
                    TDA20152_XTAL2_EXT_CLK_OVR_WR(TDA20152_CLEAN);

        ulRegVal &= ~ulRegMask;
    }

    /* Configure divider between XTAL Osc output and PLL */
    if( bError != False )
    {
        /* Configure divider between XTAL Osc output and external output */
        if( extClkFiltSel < TDA20152_XTAL_EXT_FLT_INVALID )
        {
            ulRegVal |= TDA20152_XTAL2_EXT_CLK_FILT_SEL_WR( (unsigned long)extClkFiltSel );
        }
        else
        {
            /* Out of boundary value */
            bError = False;
        }

        /* Configure filter  between XTAL Osc output and external output */
        if( extDivSel < TDA20152_XTAL_EXT_DIV_INVALID )
        {
            ulRegVal |= TDA20152_XTAL2_EXT_DIV_SEL_WR( (unsigned long)extDivSel );
        }
        else
        {
            /* Out of boundary value */
            bError = False;
        }

        /* Enable divider, filter and mux between XTAL Osc output and external output */
        if( extClkEn == True )
        {
            /* Enable -> set to 1 */
            ulRegVal |= TDA20152_XTAL2_EXT_CLK_EN_WR( TDA20152_ENABLE );
        }
        else
        {
            /* Disbale -> set to 0 */
            /* ulRegVal |= TDA20152_XTAL2_EXT_CLK_EN_WR( TDA20152_DISABLE ); */
        }

        /* extDivider is driven by ExtPin when disabled */
        if( extClkOvr == True )
        {
            /* Enable -> set to 1 */
            ulRegVal |= TDA20152_XTAL2_EXT_CLK_OVR_WR( TDA20152_ENABLE );
        }
        else
        {
            /* Disbale -> set to 0 */
            /* ulRegVal |= TDA20152_XTAL2_EXT_CLK_OVR_WR( TDA20152_DISABLE ); */
        }

        if( bError != False )
        {
            bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_XTAL2, ulRegVal );
        }
    }

    return( bError );
}

/*****************************************************************************
 * TDA20152_GetXtalExtClkSettings()
 *
 * Description:
 *      Get the xtal external clock settings
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 * OUT  extClkOvr            Override the hardwired control of the external clock.
 * OUT  extClkEn             Enable/disable the external divider, filter and output.
 * OUT  extClkFiltSel        External clock divider ratio.
 * OUT  extDivSel            External clock filter bandwidth.
 *****************************************************************************/
BOOL TDA20152_GetXtalExtClkSettings( const TDA20152_COMMUNICATION* ptCommunication,
                            BOOL *pExtClkOvr,
							BOOL *pExtClkEn,
							TDA20152_XTAL_EXT_FLT *pExtClkFiltSel,
							TDA20152_XTAL_EXT_DIV *pExtDivSel )
{
    BOOL bError = True;
    unsigned long ulRegVal = 0;

    if( ptCommunication == 0 )
    {
        bError = False;
    }

    /* XTAL2 */
    if( bError != False )
    {
        ulRegVal = 0;

        bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_XTAL2, &ulRegVal );
    }

    /* Configure divider between XTAL Osc output and PLL */
    if( bError != False )
    {
        /* Configure divider between XTAL Osc output and external output */
        *pExtClkFiltSel = (TDA20152_XTAL_EXT_FLT)TDA20152_XTAL2_EXT_CLK_FILT_SEL_RD( ulRegVal );

        /* Configure filter  between XTAL Osc output and external output */
        *pExtDivSel = (TDA20152_XTAL_EXT_DIV)TDA20152_XTAL2_EXT_DIV_SEL_RD( ulRegVal );

        /* Enable divider, filter and mux between XTAL Osc output and external output */
        if( TDA20152_XTAL2_EXT_CLK_EN_RD(ulRegVal) == TDA20152_ENABLE )
        {
            *pExtClkEn = True;
        }
        else
        {
            *pExtClkEn = False;
        }

        /* extDivider is driven by ExtPin when disabled */
        if( TDA20152_XTAL2_EXT_CLK_OVR_RD( ulRegVal ) == TDA20152_ENABLE )
        {
            /* Enable -> set to 1 */
            *pExtClkOvr = True;
        }
        else
        {
            /* Disbale -> set to 0 */
			*pExtClkOvr = False;
        }
    }

    return( bError );
}


/*****************************************************************************
 * TDA20152_ProgramVcoChannelChangeSpi
 *
 * Description:
 *      Program VCO during Channel Change
 *
 * Return Value:
 *      TRUE - operation successful; FALSE - otherwise
 *
 * Parameters:
 * I/O  Parameters          Descriptions
 * ---------------------------------------------------------------------------
 * IN   ptCommunication      Pointer to blackwidow communication object.
 *****************************************************************************/
BOOL TDA20152_ProgramVcoChannelChangeSpi ( const TDA20152_COMMUNICATION*  ptCommunication )
{
    BOOL bError = True;
    unsigned long ulRegValue;
    unsigned long ulPollingLimit = TDA20152_TM_LOCK_LO_SPI;

    BOOL bPllLocked = False;

    unsigned long ulTmVtHigh = 0;
    unsigned long ulTmVtLow = 0;

    TDA20152_VCO eVcoEnabled ;

    if( ptCommunication == 0)
    {
        bError = False;
    }

    /* disable TM */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM1_TM_SPIC_EN, 1 );
    }

    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM1_TM_EN, 0 );
    }

    /* disable clock */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_CAL_CLK_CLKGEN_EN, 0 );
    }

    /* enable VCO control by SPI */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM1_TM_VCOA_SPIC, 1 );
    }

    /* enable VCO B by SPI */
    if( bError != False )
    {
        eVcoEnabled = TDA20152_VCO_B;
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM1_TM_VCOA_EN, 0 );
    }

    /* initialise LO_lock_flag */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM_OUT_LO_LOCK_FLAG, 0 );
    }

    /* enable comparator */
    if( bError != False )
    {
        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM1_TM_COMP_EN, 1 );
    }

    if( bError != False )
    {
        do{
            /* avoid infinite loop */
            ulPollingLimit --;
            if( ulPollingLimit == 0 )
            {
                bError = False;
            }

            if( bError != False )
            {
                /* wait for some ms, the time the PLL is locked */
                TDA20152_Sleep( TDA20152_PLL_LOCK_DELAY_US );

                /* check whether the PLL is locked */
                bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_PLL_TEST2_PLL_LOCK_DET, &ulRegValue );
                bPllLocked = (BOOL) ulRegValue;

                /* read status registers */
                if( (bError != False) && (bPllLocked == True) )
                {
                    bError = TDA20152_SAT_TUNER_READ( ptCommunication, TDA20152_VCO_TM_OUT, &ulRegValue );

                    ulTmVtHigh = TDA20152_VCO_TM_OUT_TM_VT_HIGH_RD( ulRegValue );
                    ulTmVtLow  = TDA20152_VCO_TM_OUT_TM_VT_LOW_RD( ulRegValue );

                     /* and configure LO_lock_flag accordingly */
                     if( bError != False )
                     {
                        if( (ulTmVtHigh == 0) && ( ulTmVtLow == 0 ) )
                        {
                            bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM_OUT_LO_LOCK_FLAG, 3);
                        }
                        else if( (ulTmVtLow == 1) && ( eVcoEnabled == TDA20152_VCO_B ) )
                        {
                            bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM_OUT_LO_LOCK_FLAG, 2 );
                        }
                        else if( (ulTmVtHigh == 1) && ( eVcoEnabled == TDA20152_VCO_A ) )
                        {
                            bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM_OUT_LO_LOCK_FLAG, 1 );
                        }
                        else
                        {
                            /* does not match  */
                            bPllLocked = False;
                        }
                    }
                }

                if( (bError != False) && (bPllLocked == False) )
                {
                    /* Select the other VCO if the PLL is not locked */
                    /* then loop the while loop */
                    if( eVcoEnabled == TDA20152_VCO_B )
                    {
                        eVcoEnabled = TDA20152_VCO_A;
                        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM1_TM_VCOA_EN, 1 );
                    }
                    else
                    {
                        eVcoEnabled = TDA20152_VCO_B;
                        bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM1_TM_VCOA_EN, 0 );
                    }
                }
            }
        } while( (bPllLocked == False) && (bError != False) );
    }


    if( bError != False )
    {
         bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_TM1_TM_COMP_EN, 0);
    }

	/*
    if( bError != False )
    {
         bError = TDA20152_SAT_TUNER_WRITE( ptCommunication, TDA20152_VCO_CAL_CLK_CLCKGEN_INTCLK, 0);
    }
    */

    return bError;

}

static void tuner_write(void* handle, SAT_TUNER_ADDRESS address, unsigned char value, SAT_TUNER_IOSTATUS status)
{
    struct AVL_Tuner * pTuner;
    AVL_DVBSx_ErrorCode r;
    AVL_uchar ucTemp[2];

    pTuner = (struct AVL_Tuner *)(handle);
    ucTemp[0] = address;
    ucTemp[1] = value;
    r = AVL_DVBSx_II2CRepeater_SendData((AVL_uchar)(pTuner->m_uiSlaveAddress), ucTemp, 2, pTuner->m_pAVLChip);
    *status = r;
}

static unsigned char tuner_read(void* handle, SAT_TUNER_ADDRESS address, SAT_TUNER_IOSTATUS status)
{
    struct AVL_Tuner * pTuner;
    AVL_DVBSx_ErrorCode r;
    AVL_uchar ucTemp;

    pTuner = (struct AVL_Tuner *)(handle);
    r = AVL_DVBSx_II2CRepeater_ReadData_Multi((AVL_uchar)(pTuner->m_uiSlaveAddress), &ucTemp, address, 1, pTuner->m_pAVLChip);
	*status = r;

	return ucTemp;
}
#if 0
static AVL_DVBSx_ErrorCode Frequency_LPF_Adjustment(struct AVL_Tuner * pTuner,AVL_uint16 *uiAdjustFreq)
{
	AVL_DVBSx_ErrorCode r;
	AVL_uint32 uitemp1;
	AVL_uint16 uitemp2;
	AVL_uint16 minimum_LPF_100kHz;
	AVL_uint16 carrierFrequency_100kHz;

	r = AVL_DVBSx_II2C_Read32(pTuner->m_pAVLChip, 0x263E, &uitemp1);
	r |= AVL_DVBSx_II2C_Read16(pTuner->m_pAVLChip, 0x2642, &uitemp2);
	if(r != AVL_DVBSx_EC_OK)
	{
		*uiAdjustFreq = pTuner->m_uiFrequency_100kHz;
		return r;
	}

	if(pTuner->m_uiSymbolRate_Hz <= uitemp1)
	{
		carrierFrequency_100kHz =(AVL_uint16 )((uitemp2/10)+ pTuner->m_uiFrequency_100kHz);
		if(carrierFrequency_100kHz > 21500)
		{
			carrierFrequency_100kHz = 21500;
		}

		minimum_LPF_100kHz = (pTuner->m_uiSymbolRate_Hz/100000 )*135/200 +  (uitemp2/10) + 50;
		if(pTuner->m_uiLPF_100kHz < minimum_LPF_100kHz)
		{
			pTuner->m_uiLPF_100kHz = (AVL_uint16 )(minimum_LPF_100kHz);
		}
	}
	else
	{
		carrierFrequency_100kHz = pTuner->m_uiFrequency_100kHz;
	}

	*uiAdjustFreq = carrierFrequency_100kHz;

	return AVL_DVBSx_EC_OK;
}
#endif
AVL_DVBSx_ErrorCode ExtTDA20152_Initialize(struct AVL_Tuner * pTuner)
{
    BOOL bResult;
    TDA20152_CONFIG config;
    TDA20152_PWR_DETECT powerDetectThreshold;
    TDA20152_GAIN_MODE_CONFIG gainModeConfig;
    AVL_DVBSx_ErrorCode r;

	r = AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_slave_addr_addr, pTuner->m_uiSlaveAddress);
	r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_use_internal_control_addr, 0);
	r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_LPF_margin_100kHz_addr, 0);	//clean up the LPF margin for blind scan. for external driver, this must be zero.
	r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_tuner_max_LPF_100kHz_addr, 320);	//set up the right LPF for blind scan to regulate the freq_step. This field should corresponding the flat response part of the LPF.
	r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_blind_scan_tuner_spectrum_inversion_addr, 0);
#if 0
	//Following code sets up a register to indicate the cumulative effect of tuner spectrum inversion and IQ swap on the board.
	if(BOARD_IQ_SWAP ^ TUNER_SPECTRUM_INVERT)
	{
		// Should be programmed to 1 only if either the tuner or the board inverts the spectrum.
		r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_blind_scan_tuner_spectrum_inversion_addr, 1);
	}
	else
	{
		// Should be programmed to 0 if neither tuner nor the board does an inversion or if both do the inversion.
		r |= AVL_DVBSx_II2C_Write16(pTuner->m_pAVLChip, rc_blind_scan_tuner_spectrum_inversion_addr, 0);
	}
	#endif
	r |= AVL_DVBSx_II2CRepeater_Initialize(pTuner->m_uiI2CBusClock_kHz, pTuner->m_pAVLChip);

    //config.eChipType = SAT_TUNER_CHIP_TYPE_TDA20140;
    config.ulCrystalFrequencyHz = OSCILLATION_FREQUENCY_HZ;

    handle = (void *)pTuner;
    communication.p_handle = handle;
    communication.write = &tuner_write;
    communication.read = &tuner_read;

    bResult = TDA20152_Initialize(&communication, &config);
    /* Configure the gain modes */
    if (bResult == TRUE)
    {
        powerDetectThreshold.eThrehold1 = TDA20152_PWR_DETECT_THERM_THRES_VAL_45;
        powerDetectThreshold.eThrehold2 = TDA20152_PWR_DETECT_THERM_THRES_VAL_30;
        powerDetectThreshold.eThrehold3 = TDA20152_PWR_DETECT_THERM_THRES_VAL_15;
        gainModeConfig.tMode0.tGainLowCurrent.eLnaLowCurrent = TDA20152_LNA_POWER_MODE_1;
        gainModeConfig.tMode0.tGainLowCurrent.bRfVgaLowCurrent = False;
        gainModeConfig.tMode0.tGainLowCurrent.bAmpOutLowCurrent = False;
        gainModeConfig.tMode0.tGainLowCurrent.bMixerLowCurrent = False;
        gainModeConfig.tMode0.tGainModeConfiguration.eLnaGain = TDA20152_LNA_GAIN_18dB;
        gainModeConfig.tMode0.tGainModeConfiguration.eLptGain = TDA20152_LPT_GAIN_NEGATIVE_8dB;
        gainModeConfig.tMode0.tGainModeConfiguration.eRfvgaOffset = TDA20152_RFVGA_OFFSET_5;
        gainModeConfig.tMode0.tGainModeConfiguration.eVga1Offset = TDA20152_VGA1_OFFSET_4;
        gainModeConfig.tMode0.tGainModeConfiguration.eVga2Offset = TDA20152_VGA2_OFFSET_3;
        gainModeConfig.tMode0.tGainModeConfiguration.eAmpOut = TDA20152_AMPOUT_21DB;
        gainModeConfig.tMode1.tGainLowCurrent.eLnaLowCurrent = TDA20152_LNA_POWER_MODE_1;
        gainModeConfig.tMode1.tGainLowCurrent.bRfVgaLowCurrent = False;
        gainModeConfig.tMode1.tGainLowCurrent.bAmpOutLowCurrent = False;
        gainModeConfig.tMode1.tGainLowCurrent.bMixerLowCurrent = False;
        gainModeConfig.tMode1.tGainModeConfiguration.eLnaGain = TDA20152_LNA_GAIN_13dB;
        gainModeConfig.tMode1.tGainModeConfiguration.eLptGain = TDA20152_LPT_GAIN_NEGATIVE_8dB;
        gainModeConfig.tMode1.tGainModeConfiguration.eRfvgaOffset = TDA20152_RFVGA_OFFSET_5;
        gainModeConfig.tMode1.tGainModeConfiguration.eVga1Offset = TDA20152_VGA1_OFFSET_4;
        gainModeConfig.tMode1.tGainModeConfiguration.eVga2Offset = TDA20152_VGA2_OFFSET_3;
        gainModeConfig.tMode1.tGainModeConfiguration.eAmpOut = TDA20152_AMPOUT_24DB;
        gainModeConfig.tMode2.tGainLowCurrent.eLnaLowCurrent = TDA20152_LNA_POWER_MODE_1;
        gainModeConfig.tMode2.tGainLowCurrent.bRfVgaLowCurrent = False;
        gainModeConfig.tMode2.tGainLowCurrent.bAmpOutLowCurrent = False;
        gainModeConfig.tMode2.tGainLowCurrent.bMixerLowCurrent = False;
        gainModeConfig.tMode2.tGainModeConfiguration.eLnaGain = TDA20152_LNA_GAIN_7dB;
        gainModeConfig.tMode2.tGainModeConfiguration.eLptGain = TDA20152_LPT_GAIN_NEGATIVE_8dB;
        gainModeConfig.tMode2.tGainModeConfiguration.eRfvgaOffset = TDA20152_RFVGA_OFFSET_5;
        gainModeConfig.tMode2.tGainModeConfiguration.eVga1Offset = TDA20152_VGA1_OFFSET_4;
        gainModeConfig.tMode2.tGainModeConfiguration.eVga2Offset = TDA20152_VGA2_OFFSET_3;
        gainModeConfig.tMode2.tGainModeConfiguration.eAmpOut = TDA20152_AMPOUT_33DB;
        gainModeConfig.tMode3.tGainLowCurrent.eLnaLowCurrent = TDA20152_LNA_POWER_MODE_1;
        gainModeConfig.tMode3.tGainLowCurrent.bRfVgaLowCurrent = False;
        gainModeConfig.tMode3.tGainLowCurrent.bAmpOutLowCurrent = False;
        gainModeConfig.tMode3.tGainLowCurrent.bMixerLowCurrent = False;
        gainModeConfig.tMode3.tGainModeConfiguration.eLnaGain =TDA20152_LNA_GAIN_NEGATIVE_11dB;
        gainModeConfig.tMode3.tGainModeConfiguration.eLptGain = TDA20152_LPT_GAIN_NEGATIVE_8dB;
        gainModeConfig.tMode3.tGainModeConfiguration.eRfvgaOffset = TDA20152_RFVGA_OFFSET_5;
        gainModeConfig.tMode3.tGainModeConfiguration.eVga1Offset = TDA20152_VGA1_OFFSET_4;
        gainModeConfig.tMode3.tGainModeConfiguration.eVga2Offset = TDA20152_VGA2_OFFSET_3;
        gainModeConfig.tMode3.tGainModeConfiguration.eAmpOut = TDA20152_AMPOUT_24DB;
        bResult = TDA20152_ConfigureGainMode(&powerDetectThreshold, &gainModeConfig);
    }

    if (bResult == TRUE)
    {
         return AVL_DVBSx_EC_OK;
    }
    else
    {
        return AVL_DVBSx_EC_GeneralFail;
    }
}

AVL_DVBSx_ErrorCode ExtTDA20152_Lock(struct AVL_Tuner * pTuner)
{
    TDA20152_SAT_TUNER_CHANOBJ chanObj;
    BOOL bResult;
    AVL_uint16 carrierFrequency_100kHz;

	carrierFrequency_100kHz = pTuner->m_uiFrequency_100kHz;
    //Frequency_LPF_Adjustment(pTuner, &carrierFrequency_100kHz);

    chanObj.frequency_khz = carrierFrequency_100kHz*100;
	chanObj.symbol_rate_ksps = pTuner->m_uiLPF_100kHz*100000;
    chanObj.roll_off = TDA20152_SAT_TUNER_ROLLOFF_02 ; //0 = 0.2 Roll-Off
    chanObj.requested_search_range_khz = 5000; //5MHz

    bResult = TDA20152_Tune(&communication, &chanObj);
    if(bResult == FALSE)
    {
        return AVL_DVBSx_EC_GeneralFail;
    }
    else
    {
        return AVL_DVBSx_EC_OK;
    }
}

AVL_DVBSx_ErrorCode ExtTDA20152_GetLockStatus(struct AVL_Tuner * pTuner )
{
    BOOL pll_lock;
    BOOL bResult;

    bResult = TDA20152_GetPLLLockStatus(&communication, &pll_lock);
    if ((TRUE == bResult)&&(TRUE == pll_lock))
    {
        return AVL_DVBSx_EC_OK;
    }
    else
    {
        return AVL_DVBSx_EC_GeneralFail;
    }
}
#endif

