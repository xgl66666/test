/********************************************************************************
 *                       COPYRIGHT 2005 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: TERINSTALL CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/terinstall_cfg.c $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include <unistd.h>
#include <string.h>
#include <stdio.h>


#include "crules.h"
#include "tkel.h"
#include "country_cfg.h"
#include "terinstall_cfg.h"
#include "terinstall_analog_cfg.h"

/********************************************************/
/*              Defines                                 */
/********************************************************/
typedef enum
{
    eTERINSTALL_CFG_COUNTRY_ITALY,
    eTERINSTALL_CFG_COUNTRY_FACTORY_TEST,
    eTERINSTALL_CFG_COUNTRY_OTHER
} tTERINSTALL_CFG_Country;

#define kTERINSTALL_CFG_BW_6MHZ                     6000000
#define kTERINSTALL_CFG_BW_7MHZ                     7000000
#define kTERINSTALL_CFG_BW_8MHZ                     8000000

#define kTERINSTALL_CFG_FIRSTCHANNEL_UHF_6MHZ       14
#define kTERINSTALL_CFG_LASTCHANNEL_UHF_6MHZ        83
#define kTERINSTALL_CFG_NB_UHF_6MHZ                 ((kTERINSTALL_CFG_LASTCHANNEL_UHF_6MHZ-kTERINSTALL_CFG_FIRSTCHANNEL_UHF_6MHZ)+1)
#define kTERINSTALL_CFG_LOWFREQ_UHF_6MHZ            473000000
#define kTERINSTALL_CFG_HIGHFREQ_UHF_6MHZ           (kTERINSTALL_CFG_LOWFREQ_UHF_6MHZ + (kTERINSTALL_CFG_LASTCHANNEL_UHF_6MHZ-kTERINSTALL_CFG_FIRSTCHANNEL_UHF_6MHZ)*kTERINSTALL_CFG_BW_6MHZ)

#define kTERINSTALL_CFG_FIRSTCHANNEL_UHF_7MHZ       27
#define kTERINSTALL_CFG_LASTCHANNEL_UHF_7MHZ        69
#define kTERINSTALL_CFG_NB_UHF_7MHZ                 ((kTERINSTALL_CFG_LASTCHANNEL_UHF_7MHZ-kTERINSTALL_CFG_FIRSTCHANNEL_UHF_7MHZ)+1)
#define kTERINSTALL_CFG_LOWFREQ_UHF_7MHZ            522500000
#define kTERINSTALL_CFG_HIGHFREQ_UHF_7MHZ           (kTERINSTALL_CFG_LOWFREQ_UHF_7MHZ + (kTERINSTALL_CFG_LASTCHANNEL_UHF_7MHZ-kTERINSTALL_CFG_FIRSTCHANNEL_UHF_7MHZ)*kTERINSTALL_CFG_BW_7MHZ)

#define kTERINSTALL_CFG_FIRSTCHANNEL_UHF_8MHZ       21
#define kTERINSTALL_CFG_LASTCHANNEL_UHF_8MHZ        69
#define kTERINSTALL_CFG_NB_UHF_8MHZ                 ((kTERINSTALL_CFG_LASTCHANNEL_UHF_8MHZ-kTERINSTALL_CFG_FIRSTCHANNEL_UHF_8MHZ)+1)
#define kTERINSTALL_CFG_LOWFREQ_UHF_8MHZ            474000000
#define kTERINSTALL_CFG_HIGHFREQ_UHF_8MHZ           (kTERINSTALL_CFG_LOWFREQ_UHF_8MHZ + (kTERINSTALL_CFG_LASTCHANNEL_UHF_8MHZ-kTERINSTALL_CFG_FIRSTCHANNEL_UHF_8MHZ)*kTERINSTALL_CFG_BW_8MHZ)

#define kTERINSTALL_CFG_FIRSTCHANNEL_VHF_6MHZ       2
#define kTERINSTALL_CFG_LASTCHANNEL_VHF_6MHZ        13
#define kTERINSTALL_CFG_NB_VHF_6MHZ                 ((kTERINSTALL_CFG_LASTCHANNEL_VHF_6MHZ-kTERINSTALL_CFG_FIRSTCHANNEL_VHF_6MHZ)+1)
#define kTERINSTALL_CFG_LOWFREQ_VHF_6MHZ            57000000
#define kTERINSTALL_CFG_HIGHFREQ_VHF_6MHZ           213000000

#define kTERINSTALL_CFG_FIRSTCHANNEL_VHF_7MHZ       5
#define kTERINSTALL_CFG_LASTCHANNEL_VHF_7MHZ        12
#define kTERINSTALL_CFG_NB_VHF_7MHZ                 ((kTERINSTALL_CFG_LASTCHANNEL_VHF_7MHZ-kTERINSTALL_CFG_FIRSTCHANNEL_VHF_7MHZ)+1)
#define kTERINSTALL_CFG_LOWFREQ_VHF_7MHZ            177500000
#define kTERINSTALL_CFG_HIGHFREQ_VHF_7MHZ           (kTERINSTALL_CFG_LOWFREQ_VHF_7MHZ + (kTERINSTALL_CFG_LASTCHANNEL_VHF_7MHZ-kTERINSTALL_CFG_FIRSTCHANNEL_VHF_7MHZ)*kTERINSTALL_CFG_BW_7MHZ)

/* VHF 8MHz standard (for information only) */
/*#define kTERINSTALL_CFG_FIRSTCHANNEL_VHF_8MHZ       6 */
/*#define kTERINSTALL_CFG_LASTCHANNEL_VHF_8MHZ        12 */
/*#define kTERINSTALL_CFG_NB_VHF_8MHZ                 ((kTERINSTALL_CFG_LASTCHANNEL_VHF_8MHZ-kTERINSTALL_CFG_FIRSTCHANNEL_VHF_8MHZ)+1) */
/*#define kTERINSTALL_CFG_LOWFREQ_VHF_8MHZ            178000000 */
/*#define kTERINSTALL_CFG_HIGHFREQ_VHF_8MHZ           (kTERINSTALL_CFG_LOWFREQ_VHF_8MHZ + (kTERINSTALL_CFG_LASTCHANNEL_VHF_8MHZ-kTERINSTALL_CFG_FIRSTCHANNEL_VHF_8MHZ)*kTERINSTALL_CFG_BW_8MHZ) */

/* VHF 8MHz South Africa */
#define kTERINSTALL_CFG_FIRSTCHANNEL_VHF_8MHZ       4
#define kTERINSTALL_CFG_LASTCHANNEL_VHF_8MHZ        13
#define kTERINSTALL_CFG_NB_VHF_8MHZ                 ((kTERINSTALL_CFG_LASTCHANNEL_VHF_8MHZ-kTERINSTALL_CFG_FIRSTCHANNEL_VHF_8MHZ)+1)
#define kTERINSTALL_CFG_LOWFREQ_VHF_8MHZ            178000000
#define kTERINSTALL_CFG_HIGHFREQ_VHF_8MHZ           (kTERINSTALL_CFG_LOWFREQ_VHF_8MHZ + (kTERINSTALL_CFG_LASTCHANNEL_VHF_8MHZ-kTERINSTALL_CFG_FIRSTCHANNEL_VHF_8MHZ)*kTERINSTALL_CFG_BW_8MHZ)

#define kTERINSTALL_CFG_FREQERROR                   50000

#define kTERINSTALL_CFG_FACTORY_TS_FREQ             585500000
#define kTERINSTALL_CFG_FACTORY_TS_BW               kTERINSTALL_CFG_BW_7MHZ

/********************************************************/
/*              Local File Variables (LOCAL)            */
/********************************************************/
LOCAL tTERINSTALL_CFG_Country    TERINSTALL_CFG_country;
LOCAL uint32_t TERINSTALL_CFG_VHFbandwidth;
LOCAL uint32_t TERINSTALL_CFG_UHFbandwidth;
LOCAL tTERINSTALL_CFG_SpecInv    TERINSTALL_CFG_SpecInv;
LOCAL tTERINSTALL_CFG_SignalMode	TERINSTALL_CFG_SignalMode = eTERINSTALL_8K;
LOCAL tTERINSTALL_CFG_InstallInformation	TERINSTALL_CFG_Install;
/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/
/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/


/*===========================================================================
 *
 * TERINSTALL_CFG_SetCountry
 *
 * Parameters:
 *      country_index
 *
 * Description:
 *      Set the current country
 *
 * Returns:
 *      nothing
 *
 *===========================================================================*/
void TERINSTALL_CFG_SetCountry(uint32_t country_index)
{
   	if (strncmp(COUNTRY_CFG_GetAvailableCountryDVBtrigram(country_index), "ita", 3) == 0)
    {
        /* !!! ITALIAN VHF channels are different from the other european ones !!! */
    	TERINSTALL_CFG_VHFbandwidth = kTERINSTALL_CFG_BW_7MHZ;
		TERINSTALL_CFG_UHFbandwidth = kTERINSTALL_CFG_BW_8MHZ;
        TERINSTALL_CFG_country = eTERINSTALL_CFG_COUNTRY_ITALY;
    }
	else if (strncmp(COUNTRY_CFG_GetAvailableCountryDVBtrigram(country_index), "deu", 3) == 0)
	{
    	TERINSTALL_CFG_VHFbandwidth = kTERINSTALL_CFG_BW_7MHZ;
		TERINSTALL_CFG_UHFbandwidth = kTERINSTALL_CFG_BW_8MHZ;
		TERINSTALL_CFG_country = eTERINSTALL_CFG_COUNTRY_OTHER;
	}
    else if (strncmp(COUNTRY_CFG_GetAvailableCountryDVBtrigram(country_index), "saf", 3) == 0)
	{
    	TERINSTALL_CFG_VHFbandwidth = kTERINSTALL_CFG_BW_8MHZ;
		TERINSTALL_CFG_UHFbandwidth = kTERINSTALL_CFG_BW_8MHZ;
		TERINSTALL_CFG_country = eTERINSTALL_CFG_COUNTRY_OTHER;
	}
    else if (strncmp(COUNTRY_CFG_GetAvailableCountryDVBtrigram(country_index), "fac", 3) == 0)
    {
    	TERINSTALL_CFG_VHFbandwidth = kTERINSTALL_CFG_BW_7MHZ;
		TERINSTALL_CFG_UHFbandwidth = kTERINSTALL_CFG_BW_8MHZ;
        /* !!! Factory test !!! */
        TERINSTALL_CFG_country = eTERINSTALL_CFG_COUNTRY_FACTORY_TEST;
    }
	else
	{
		/* other countries */
    	TERINSTALL_CFG_VHFbandwidth = 0;
		TERINSTALL_CFG_UHFbandwidth = kTERINSTALL_CFG_BW_8MHZ;
    	TERINSTALL_CFG_country = eTERINSTALL_CFG_COUNTRY_OTHER;
	}

    TERINSTALL_CFG_SpecInv = eTERINSTALL_CFG_SPECTRUM_NORMAL;

#ifdef ANALOG
    TERINSTALL_ANALOG_CFG_SetCountry(country_index);
#endif /* ANALOG */
}

/*===========================================================================
 *
 * TERINSTALL_CFG_GetSignalMode
 *
 * Parameters:
 *      none
 *
 * Description:
 *
 *
 * Returns:
 *      2K/8K
 *
 *===========================================================================*/
tTERINSTALL_CFG_SignalMode TERINSTALL_CFG_GetSignalMode(void)
{
    return TERINSTALL_CFG_SignalMode;
}

/*===========================================================================
 *
 * TERINSTALL_CFG_GetVHFbandwidth
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the current VHF bandwidth (corresponding to the country set by a
 *      prior call to TERINSTALL_CFG_SetCountry) or 0 if no VHF.
 *
 * Returns:
 *      VHF bandwidth or 0 if no VHF
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_GetVHFbandwidth(void)
{
    return TERINSTALL_CFG_VHFbandwidth;
}

/*===========================================================================
 *
 * TERINSTALL_CFG_GetUHFbandwidth
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the current UHF bandwidth (corresponding to the country set by a
 *      prior call to TERINSTALL_CFG_SetCountry) or 0 if no UHF.
 *
 * Returns:
 *      UHF bandwidth or 0 if no UHF
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_GetUHFbandwidth(void)
{
    return TERINSTALL_CFG_UHFbandwidth;
}

/*===========================================================================
 *
 * TERINSTALL_CFG_GetVHFlowFreq
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the current VHF low frequency (corresponding to the country
 *      set by a prior call to TERINSTALL_CFG_SetCountry)
 *
 * Returns:
 *      VHF low frequency or 0 if no VHF
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_GetVHFlowFreq(void)
{
    if (TERINSTALL_CFG_VHFbandwidth == 0)
    {
        return 0;
    }
    else if (TERINSTALL_CFG_VHFbandwidth == kTERINSTALL_CFG_BW_7MHZ)
    {
        return kTERINSTALL_CFG_LOWFREQ_VHF_7MHZ;
    }
    else if (TERINSTALL_CFG_VHFbandwidth == kTERINSTALL_CFG_BW_8MHZ)
    {
        return kTERINSTALL_CFG_LOWFREQ_VHF_8MHZ;
    }

    return 0;
}

/*===========================================================================
 *
 * TERINSTALL_CFG_GetVHFhighFreq
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the current VHF high frequency (corresponding to the country
 *      set by a prior call to TERINSTALL_CFG_SetCountry)
 *
 * Returns:
 *      VHF high frequency or 0 if no VHF
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_GetVHFhighFreq(void)
{
    if (TERINSTALL_CFG_VHFbandwidth == 0)
    {
        return 0;
    }
    else if (TERINSTALL_CFG_VHFbandwidth == kTERINSTALL_CFG_BW_7MHZ)
    {
        return kTERINSTALL_CFG_HIGHFREQ_VHF_7MHZ;
    }
    else if (TERINSTALL_CFG_VHFbandwidth == kTERINSTALL_CFG_BW_8MHZ)
    {
        return kTERINSTALL_CFG_HIGHFREQ_VHF_8MHZ;
    }

    return 0;
}

/*===========================================================================
 *
 * TERINSTALL_CFG_GetUHFlowFreq
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the current UHF low frequency (corresponding to the country
 *      set by a prior call to TERINSTALL_CFG_SetCountry)
 *
 * Returns:
 *      UHF low frequency or 0 if no UHF
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_GetUHFlowFreq(void)
{
    if (TERINSTALL_CFG_UHFbandwidth == 0)
    {
        return 0;
    }
    else if (TERINSTALL_CFG_UHFbandwidth == kTERINSTALL_CFG_BW_8MHZ)
    {
        return kTERINSTALL_CFG_LOWFREQ_UHF_8MHZ;
    }

    return 0;
}

/*===========================================================================
 *
 * TERINSTALL_CFG_GetUHFhighFreq
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the current UHF high frequency (corresponding to the country
 *      set by a prior call to TERINSTALL_CFG_SetCountry)
 *
 * Returns:
 *      UHF high frequency or 0 if no UHF
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_GetUHFhighFreq(void)
{
    if (TERINSTALL_CFG_UHFbandwidth == 0)
    {
        return 0;
    }
    else if (TERINSTALL_CFG_UHFbandwidth == kTERINSTALL_CFG_BW_8MHZ)
    {
        return kTERINSTALL_CFG_HIGHFREQ_UHF_8MHZ;
    }

    return 0;
}

/*===========================================================================
 *
 * TERINSTALL_CFG_GetSpectralInversion
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the current spectral inversion (corresponding to the country
 *      set by a prior call to TERINSTALL_CFG_SetCountry)
 *
 * Returns:
 *      nothing
 *
 *===========================================================================*/
tTERINSTALL_CFG_SpecInv TERINSTALL_CFG_GetSpectralInversion(void)
{
    return TERINSTALL_CFG_SpecInv;
}

/*===========================================================================
 *
 * TERINSTALL_CFG_IsScanRangeSupported
 *
 * Parameters:
 *      none
 *
 * Description:
 *      This function is called to know if scan range is supported.
 *
 * Returns:
 *      true if scan range is supported, false otherwise
 *
 *===========================================================================*/
bool TERINSTALL_CFG_IsScanRangeSupported(void)
{
/*    if ((TERINSTALL_CFG_country == eTERINSTALL_CFG_COUNTRY_ITALY) ||
        (TERINSTALL_CFG_country == eTERINSTALL_CFG_COUNTRY_FACTORY_TEST))
    {
        return false;
    }*/

    return false;
}

/*===========================================================================
 *
 * TERINSTALL_CFG_GetScanningInstallationConfig
 *
 * Parameters:
 *
 *
 * Description:
 *      .
 *
 * Returns:
 *      true if succeed else false
 *
 *===========================================================================*/
bool TERINSTALL_CFG_GetScanningInstallationConfig(tTERINSTALL_CFG_InstallInformation **ppstScanInfo)
{
	TERINSTALL_CFG_Install.bInstallPATIfNoSDT = TRUE;
	TERINSTALL_CFG_Install.bInstallSdtOnly = TRUE; /* No cross checking */
#ifdef LCN_YES
	TERINSTALL_CFG_Install.bLCNMngt = TRUE;
#else
	TERINSTALL_CFG_Install.bLCNMngt = FALSE;
#endif

	if(ppstScanInfo != NULL)
	{
		*ppstScanInfo = &TERINSTALL_CFG_Install;
		return true;
	}
	return false;
}

/*===========================================================================
 *
 * TERINSTALL_CFG_GetNumberOfTS
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the number of TS in the current TS list
 *
 * Returns:
 *      number of TS in the current TS list
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_GetNumberOfTS(void)
{
    uint32_t nbTs = 0;

    if (TERINSTALL_CFG_VHFbandwidth == kTERINSTALL_CFG_BW_7MHZ)
    {
        nbTs += kTERINSTALL_CFG_NB_VHF_7MHZ;
    }
    else if (TERINSTALL_CFG_VHFbandwidth == kTERINSTALL_CFG_BW_8MHZ)
    {
        nbTs += kTERINSTALL_CFG_NB_VHF_8MHZ;
    }

    if (TERINSTALL_CFG_UHFbandwidth == kTERINSTALL_CFG_BW_8MHZ)
    {
        nbTs += kTERINSTALL_CFG_NB_UHF_8MHZ;
    }


    if(TERINSTALL_CFG_country == eTERINSTALL_CFG_COUNTRY_FACTORY_TEST)
    {
        nbTs += 1;
    }

    return nbTs;
}

/*===========================================================================
 *
 * TERINSTALL_CFG_GetTsListFrequency
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the frequency of the TS
 *
 * Returns:
 *
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_GetTsListFrequency(uint32_t index)
{
    uint32_t freq = 0;

	

    if(TERINSTALL_CFG_country == eTERINSTALL_CFG_COUNTRY_FACTORY_TEST)
    {
        if (index == 0)
        {
            return kTERINSTALL_CFG_FACTORY_TS_FREQ;
        }
        else
        {
            index--;
        }
    }


    if (TERINSTALL_CFG_VHFbandwidth == kTERINSTALL_CFG_BW_7MHZ)
    {
        if (index < kTERINSTALL_CFG_NB_VHF_7MHZ)
        {
            /* VHF - 7MHz */
            freq = kTERINSTALL_CFG_LOWFREQ_VHF_7MHZ + kTERINSTALL_CFG_BW_7MHZ*index;

            if (TERINSTALL_CFG_country == eTERINSTALL_CFG_COUNTRY_ITALY)
            {
                /* specific mapping for ITALY */
                switch(index)
                {
                    case 1: freq = 186000000; break;
					case 2: freq = 0; break;
                    case 3: freq = 194500000; break;
                    case 4: freq = 203500000; break;
                }
            }
        }
        else if (TERINSTALL_CFG_UHFbandwidth == kTERINSTALL_CFG_BW_8MHZ)
        {
            /* UHF - 8MHz*/
            freq = kTERINSTALL_CFG_LOWFREQ_UHF_8MHZ + kTERINSTALL_CFG_BW_8MHZ*(index - kTERINSTALL_CFG_NB_VHF_7MHZ);
        }
    }
    else if (TERINSTALL_CFG_VHFbandwidth == kTERINSTALL_CFG_BW_8MHZ)
    {
        if (index < kTERINSTALL_CFG_NB_VHF_8MHZ)
        {
            /* VHF - 8MHz */
            freq = kTERINSTALL_CFG_LOWFREQ_VHF_8MHZ + kTERINSTALL_CFG_BW_8MHZ*index;
        }
        else if (TERINSTALL_CFG_UHFbandwidth == kTERINSTALL_CFG_BW_8MHZ)
        {
            /* UHF - 8MHz */
            freq = kTERINSTALL_CFG_LOWFREQ_UHF_8MHZ + kTERINSTALL_CFG_BW_8MHZ*(index - kTERINSTALL_CFG_NB_VHF_8MHZ);
        }
    }
    else if (TERINSTALL_CFG_VHFbandwidth == 0)
    {
        if (TERINSTALL_CFG_UHFbandwidth == kTERINSTALL_CFG_BW_8MHZ)
        {
            freq = kTERINSTALL_CFG_LOWFREQ_UHF_8MHZ + kTERINSTALL_CFG_BW_8MHZ*index;
        }
    }

    return freq;
}

/*===========================================================================
 *
 * TERINSTALL_CFG_GetTsListBandwidth
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the symbol rate of the TS
 *
 * Returns:
 *
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_GetTsListBandwidth(uint32_t index)
{
    uint32_t symbolRate = 0;

    if(TERINSTALL_CFG_country == eTERINSTALL_CFG_COUNTRY_FACTORY_TEST)
    {
        if (index == 0)
        {
            return kTERINSTALL_CFG_FACTORY_TS_BW;
        }
        else
        {
            index--;
        }
    }


    if (TERINSTALL_CFG_VHFbandwidth == kTERINSTALL_CFG_BW_7MHZ)
    {
        if (index < kTERINSTALL_CFG_NB_VHF_7MHZ)
        {
            /* VHF - 7MHz */
            symbolRate = kTERINSTALL_CFG_BW_7MHZ;
        }
        else if (TERINSTALL_CFG_UHFbandwidth == kTERINSTALL_CFG_BW_8MHZ)
        {
            /* UHF - 8MHz */
            symbolRate = kTERINSTALL_CFG_BW_8MHZ;
        }
    }
    else if (TERINSTALL_CFG_VHFbandwidth == kTERINSTALL_CFG_BW_8MHZ)
    {
        if (index < kTERINSTALL_CFG_NB_VHF_8MHZ)
        {
            /* VHF - 8MHz */
            symbolRate = kTERINSTALL_CFG_BW_8MHZ;
        }
        else if (TERINSTALL_CFG_UHFbandwidth == kTERINSTALL_CFG_BW_8MHZ)
        {
            /* UHF - 8MHz */
            symbolRate = kTERINSTALL_CFG_BW_8MHZ;
        }
    }
    else if (TERINSTALL_CFG_VHFbandwidth == 0)
    {
        if (TERINSTALL_CFG_UHFbandwidth == kTERINSTALL_CFG_BW_8MHZ)
        {
            /* UHF - 8MHz */
            symbolRate = kTERINSTALL_CFG_BW_8MHZ;
        }
    }

    return symbolRate;
}


/*===========================================================================
 *
 * TERINSTALL_CFG_ChannelNumber2Frequency
 *
 * Parameters:
 *      channelNb
 *      bandwidth
 *
 * Description:
 *      Return the frequency corresponding to the channel number
 *
 * Returns:
 *      frequency
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_ChannelNumber2Frequency(uint32_t channelNb, uint32_t bandwidth)
{

    if(TERINSTALL_CFG_country == eTERINSTALL_CFG_COUNTRY_FACTORY_TEST)
    {
        if (channelNb == 0)
        {
            return kTERINSTALL_CFG_FACTORY_TS_FREQ;
        }
    }


    if (bandwidth == kTERINSTALL_CFG_BW_8MHZ)
    {
        /* 8 MHz */
        if (channelNb >= kTERINSTALL_CFG_FIRSTCHANNEL_UHF_8MHZ &&  channelNb <= kTERINSTALL_CFG_LASTCHANNEL_UHF_8MHZ)
        {
            /* UHF - 8 MHz */
            if (TERINSTALL_CFG_VHFbandwidth == kTERINSTALL_CFG_BW_7MHZ)
            {
                return(TERINSTALL_CFG_GetTsListFrequency(channelNb - kTERINSTALL_CFG_FIRSTCHANNEL_UHF_8MHZ + kTERINSTALL_CFG_NB_VHF_7MHZ));
            }
            else
            {
                return(TERINSTALL_CFG_GetTsListFrequency(channelNb - kTERINSTALL_CFG_FIRSTCHANNEL_UHF_8MHZ));
            }
        }
        else if (channelNb >= kTERINSTALL_CFG_FIRSTCHANNEL_VHF_8MHZ &&  channelNb <= kTERINSTALL_CFG_LASTCHANNEL_VHF_8MHZ)
        {
            /* VHF - 8 MHz */
            return (TERINSTALL_CFG_GetTsListFrequency(channelNb - kTERINSTALL_CFG_FIRSTCHANNEL_VHF_8MHZ));
        }
    }
    else if (bandwidth == kTERINSTALL_CFG_BW_7MHZ)
    {
        /* 7 MHz */
        if (channelNb >= kTERINSTALL_CFG_FIRSTCHANNEL_VHF_7MHZ &&  channelNb <= kTERINSTALL_CFG_LASTCHANNEL_VHF_7MHZ)
        {
            /* VHF - 7 MHz */
            return (TERINSTALL_CFG_GetTsListFrequency(channelNb - kTERINSTALL_CFG_FIRSTCHANNEL_VHF_7MHZ));
        }
    }
    else
    {
        /* bandwidth not handled */
    }

    return (0);
}

/*===========================================================================
 *
 * TERINSTALL_CFG_Frequency2ChannelNumber
 *
 * Parameters:
 *      frequency
 *      bandwidth
 *
 * Description:
 *      Return the frequency corresponding to the channel number
 *
 * Returns:
 *      channelNb
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_Frequency2ChannelNumber(uint32_t frequency, uint32_t bandwidth)
{

    if(TERINSTALL_CFG_country == eTERINSTALL_CFG_COUNTRY_FACTORY_TEST)
    {
        if (frequency == kTERINSTALL_CFG_FACTORY_TS_FREQ)
        {
            return 0;
        }
    }

    if (bandwidth == kTERINSTALL_CFG_BW_8MHZ)
    {
        /* 8MHz */
        if (frequency >= (kTERINSTALL_CFG_LOWFREQ_UHF_8MHZ - kTERINSTALL_CFG_FREQERROR) &&  frequency <= (kTERINSTALL_CFG_HIGHFREQ_UHF_8MHZ + kTERINSTALL_CFG_FREQERROR))
        {
            /* UHF - 8 MHz */
            int temp = frequency - kTERINSTALL_CFG_LOWFREQ_UHF_8MHZ;
            temp = temp + (kTERINSTALL_CFG_BW_8MHZ / 2);
            temp = temp / kTERINSTALL_CFG_BW_8MHZ;
            return (temp + kTERINSTALL_CFG_FIRSTCHANNEL_UHF_8MHZ);
        }
        else if (frequency >= (kTERINSTALL_CFG_LOWFREQ_VHF_8MHZ - kTERINSTALL_CFG_FREQERROR) &&  frequency <= (kTERINSTALL_CFG_HIGHFREQ_VHF_8MHZ + kTERINSTALL_CFG_FREQERROR))
        {
            /* VHF - 8 MHz */
            int temp = frequency - kTERINSTALL_CFG_LOWFREQ_VHF_8MHZ;
            temp = temp + (kTERINSTALL_CFG_BW_8MHZ / 2);
            temp = temp / kTERINSTALL_CFG_BW_8MHZ;
            return (temp + kTERINSTALL_CFG_FIRSTCHANNEL_VHF_8MHZ);
        }
    }
    else if (bandwidth == kTERINSTALL_CFG_BW_7MHZ)
    {
        /* 7MHz - ITALY specific mapping */
        if (TERINSTALL_CFG_country == eTERINSTALL_CFG_COUNTRY_ITALY)
        {
            if ((frequency >= (kTERINSTALL_CFG_LOWFREQ_VHF_7MHZ - kTERINSTALL_CFG_FREQERROR) &&  frequency <= (kTERINSTALL_CFG_HIGHFREQ_VHF_7MHZ + kTERINSTALL_CFG_FREQERROR)) || frequency == 0)
            {
                /* VHF - 7 MHz */
                int temp;
                int iChannelNumber = frequency - kTERINSTALL_CFG_LOWFREQ_VHF_7MHZ;
                iChannelNumber = iChannelNumber + (kTERINSTALL_CFG_BW_7MHZ / 2);
                iChannelNumber = iChannelNumber / kTERINSTALL_CFG_BW_7MHZ;
                iChannelNumber = iChannelNumber + kTERINSTALL_CFG_FIRSTCHANNEL_VHF_7MHZ;

                temp  = frequency;
                temp += 50000;
                temp /= 100000;

                /* specific cases */
                switch (temp)
                {
                    case 1860: iChannelNumber = 6; break;
   					case 0: iChannelNumber = 7; break;
                    case 1945: iChannelNumber = 8; break;
                    case 2035: iChannelNumber = 9; break;
                }

                return (iChannelNumber);
            }
        }

        /* 7MHz - Standard */
        if (frequency >= (kTERINSTALL_CFG_LOWFREQ_VHF_7MHZ - kTERINSTALL_CFG_FREQERROR) &&  frequency <= (kTERINSTALL_CFG_HIGHFREQ_VHF_7MHZ + kTERINSTALL_CFG_FREQERROR))
        {
            /* VHF - 7 MHz */
            int temp = frequency - kTERINSTALL_CFG_LOWFREQ_VHF_7MHZ;
            temp = temp + (kTERINSTALL_CFG_BW_7MHZ / 2);
            temp = temp / kTERINSTALL_CFG_BW_7MHZ;
            return(temp + kTERINSTALL_CFG_FIRSTCHANNEL_VHF_7MHZ);
        }
    }
    else
    {
        /* bandwidth not handled */
    }

    return (0);
}


/*===========================================================================
 *
 * TERINSTALL_CFG_Get_Default_CA_Install_Mode
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the the default CA intall mode :
 *			default choice in the menu or install mode if no menu
 *
 * Returns:
 *      CA Install Mode
 *
 *===========================================================================*/
cmTerinstall_CFG_CAInstallMode_t TERINSTALL_CFG_Get_Default_CA_Install_Mode(void)
{
	return eTERINSTALL_CFG_SERVICE_ALL;
}

/*===========================================================================
 *
 * TERINSTALL_CFG_Get_ASU_CA_Install_Mode
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the the CA intall mode for automatic service update.
 *
 * Returns:
 *      CA Install Mode
 *
 *===========================================================================*/
cmTerinstall_CFG_CAInstallMode_t TERINSTALL_CFG_Get_ASU_CA_Install_Mode(void)
{
	return eTERINSTALL_CFG_SERVICE_ALL;
}
