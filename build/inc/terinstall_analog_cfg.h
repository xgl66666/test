/******************************************************************************
 *                           COPYRIGHT 2005 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: CFG TERINSTALL
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/terinstall_analog_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _TERINSTALL_ANALOG_CFG_H_
#define _TERINSTALL_ANALOG_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*               typedef                               */
/*******************************************************/

/*===========================================================================
 *
 * TERINSTALL_ANALOG_CFG_SetCountry
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
void TERINSTALL_ANALOG_CFG_SetCountry(uint32_t country_index);
    
/*===========================================================================
 *
 * TERINSTALL_CFG_GetAnalogVHFbandwidth
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the current Analog VHF bandwidth (corresponding to the country set by a
 *      prior call to TERINSTALL_CFG_SetCountry) or 0 if no analog VHF.
 *
 * Returns:
 *      Analog VHF bandwidth or 0 if no Analog VHF
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_GetAnalogVHFbandwidth(void);


/*===========================================================================
 *
 * TERINSTALL_CFG_GetAnalogUHFbandwidth
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the current Analog UHF bandwidth (corresponding to the country set by a
 *      prior call to TERINSTALL_CFG_SetCountry) or 0 if no analog UHF.
 *
 * Returns:
 *      Analog VHF bandwidth or 0 if no Analog VHF
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_GetAnalogUHFbandwidth(void);


/*===========================================================================
 *
 * TERINSTALL_CFG_GetAnalogVHFlowFreq
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the current analog VHF low frequency (corresponding to the country
 *      set by a prior call to TERINSTALL_CFG_SetCountry)
 *
 * Returns:
 *      VHF low frequency or 0 if no VHF
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_GetAnalogVHFlowFreq(void);

/*===========================================================================
 *
 * TERINSTALL_CFG_GetAnalogVHFhighFreq
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the current analog VHF high frequency (corresponding to the country
 *      set by a prior call to TERINSTALL_CFG_SetCountry)
 *
 * Returns:
 *      VHF high frequency or 0 if no VHF
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_GetAnalogVHFhighFreq(void);


/*===========================================================================
 *
 * TERINSTALL_CFG_GetAnalogUHFlowFreq
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the current Analog UHF low frequency (corresponding to the country
 *      set by a prior call to TERINSTALL_CFG_SetCountry)
 *
 * Returns:
 *      UHF low frequency or 0 if no UHF
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_GetAnalogUHFlowFreq(void);

/*===========================================================================
 *
 * TERINSTALL_CFG_GetAnalogUHFhighFreq
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the current Analog UHF high frequency (corresponding to the country
 *      set by a prior call to TERINSTALL_CFG_SetCountry)
 *
 * Returns:
 *      UHF high frequency or 0 if no UHF
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_GetAnalogUHFhighFreq(void);




/*===========================================================================
 *
 * TERINSTALL_CFG_AnalogChannelNumber2Frequency
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
uint32_t TERINSTALL_CFG_AnalogChannelNumber2Frequency(uint32_t channelNb, uint32_t bandwidth);

/*===========================================================================
 *
 * TERINSTALL_CFG_AnalogFrequency2ChannelNumber
 *
 * Parameters:
 *      frequency
 *      bandwidth
 *
 * Description:
 *      Return the analog frequency corresponding to the channel number
 *
 * Returns:
 *      channelNb
 *
 *===========================================================================*/
uint32_t TERINSTALL_CFG_AnalogFrequency2ChannelNumber(uint32_t frequency, uint32_t bandwidth);



#ifdef __cplusplus
}
#endif

#endif /* _TERINSTALL_ANALOG_CFG_H_ */
