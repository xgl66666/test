/******************************************************************************
 *                           COPYRIGHT 2005 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: CFG TERINSTALL
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/terinstall_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _TERINSTALL_CFG_H_
#define _TERINSTALL_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*               typedef                               */
/*******************************************************/
typedef enum
{
	eTERINSTALL_CFG_SPECTRUM_AUTO,
	eTERINSTALL_CFG_SPECTRUM_NORMAL,
	eTERINSTALL_CFG_SPECTRUM_INVERTED
} tTERINSTALL_CFG_SpecInv;

typedef enum
{
	eTERINSTALL_2K,
	eTERINSTALL_8K
} tTERINSTALL_CFG_SignalMode;

typedef struct
{
	bool						bInstallPATIfNoSDT;		/* install on PAT if no SDT found 						*/
	bool						bInstallSdtOnly;		/* install services in SDT whithout cross checking NIT  */
	bool						bLCNMngt;				/* LCN mngt ; yes = true								*/
} tTERINSTALL_CFG_InstallInformation;

typedef	enum
{
	eTERINSTALL_CFG_SERVICE_ALL			= 0,
	eTERINSTALL_CFG_SERVICE_SCRAMBLED,
	eTERINSTALL_CFG_SERVICE_FREE
}cmTerinstall_CFG_CAInstallMode_t;

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
tTERINSTALL_CFG_SignalMode TERINSTALL_CFG_GetSignalMode(void);

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
void TERINSTALL_CFG_SetCountry(uint32_t country_index);

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
uint32_t TERINSTALL_CFG_GetVHFbandwidth(void);

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
uint32_t TERINSTALL_CFG_GetUHFbandwidth(void);

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
uint32_t TERINSTALL_CFG_GetVHFlowFreq(void);

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
uint32_t TERINSTALL_CFG_GetVHFhighFreq(void);

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
uint32_t TERINSTALL_CFG_GetUHFlowFreq(void);

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
uint32_t TERINSTALL_CFG_GetUHFhighFreq(void);

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
tTERINSTALL_CFG_SpecInv TERINSTALL_CFG_GetSpectralInversion(void);

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
bool TERINSTALL_CFG_IsScanRangeSupported(void);

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
bool TERINSTALL_CFG_GetScanningInstallationConfig(tTERINSTALL_CFG_InstallInformation **ppstScanInfo);

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
uint32_t TERINSTALL_CFG_GetNumberOfTS(void);

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
uint32_t TERINSTALL_CFG_GetTsListFrequency(uint32_t index);

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
uint32_t TERINSTALL_CFG_GetTsListBandwidth(uint32_t index);


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
uint32_t TERINSTALL_CFG_ChannelNumber2Frequency(uint32_t channelNb, uint32_t bandwidth);

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
uint32_t TERINSTALL_CFG_Frequency2ChannelNumber(uint32_t frequency, uint32_t bandwidth);




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
cmTerinstall_CFG_CAInstallMode_t TERINSTALL_CFG_Get_Default_CA_Install_Mode(void);

/*===========================================================================
 *
 * TERINSTALL_CFG_Get_CA_Automatic_Install_Mode
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
cmTerinstall_CFG_CAInstallMode_t TERINSTALL_CFG_Get_ASU_CA_Install_Mode(void);

#ifdef __cplusplus
}
#endif

#endif /* _TERINSTALL_CFG_H_ */
