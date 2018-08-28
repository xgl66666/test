/******************************************************************************
 *                         COPYRIGHT 2005 IWEDIA TECHNOLOGIES                 *
 ******************************************************************************
 *
 * MODULE NAME: CABINSTALL CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/cabinstall_cfg.c $
 *            $Rev: 379 $
 *            $Date: 2011-09-16 09:28:26 +0200 (Fri, 16 Sep 2011) $
 *
 * DESCRIPTION :
 *
 *****************************************************************************/


/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"
#include "cabinstall_cfg.h"
#include "tkel.h"

#ifdef __USE_CONFIG_FILE__
#include "config_file.h"
#endif

/********************************************************/
/*              Defines                                 */
/********************************************************/

/*
 * iDecode Test
 */
#define CABINSTALL_CFG_NETWORK_ID              0xFFFF
#define CABINSTALL_CFG_HOMETS_FREQUENCY		  400000000 /* Hz */
#define CABINSTALL_CFG_HOMETS_SYMBOL_RATE 	  5057000 /* Bauds */
#define CABINSTALL_CFG_HOMETS_MODULATION     eCABINSTALL_CFG_MOD_QAM64
#define CABINSTALL_CFG_HOMETS_SPECTRUM       eCABINSTALL_CFG_SPECTRUM_AUTO /*eCABINSTALL_CFG_SPECTRUM_AUTO*/
/*
 * Cas 1 ki marche
 */
#ifdef _CAS_1
#define CABINSTALL_CFG_NETWORK_ID                 1
#define CABINSTALL_CFG_HOMETS_FREQUENCY		586000000 /* Hz */
#define CABINSTALL_CFG_HOMETS_SYMBOL_RATE 	  6875000 /* Bauds */
#define CABINSTALL_CFG_HOMETS_MODULATION     eCABINSTALL_CFG_MOD_QAM64
#define CABINSTALL_CFG_HOMETS_SPECTRUM       eCABINSTALL_CFG_SPECTRUM_INVERTED /*eCABINSTALL_CFG_SPECTRUM_AUTO*/
#endif

#ifdef _CAS_2
/*
 * Cas 2 qui marche aussi
 */
#define CABINSTALL_CFG_NETWORK_ID                10
#define CABINSTALL_CFG_HOMETS_FREQUENCY      297000000 /* Hz */
#define CABINSTALL_CFG_HOMETS_SYMBOL_RATE 	  5360000 /* Bauds */
#define CABINSTALL_CFG_HOMETS_MODULATION     eCABINSTALL_CFG_MOD_QAM256
#define CABINSTALL_CFG_HOMETS_SPECTRUM       eCABINSTALL_CFG_SPECTRUM_INVERTED /*eCABINSTALL_CFG_SPECTRUM_AUTO*/
#endif

#define CABINSTALL_CFG_START_SCAN_FREQUENCY  290000 /* kHz */
#define CABINSTALL_CFG_STOP_SCAN_FREQUENCY   850000 /* kHz */


/********************************************************/
/*			  Local File Variables (LOCAL)			*/
/********************************************************/
tCABINSTALL_CFG_InstallInformation			CABINSTALL_CFG_ScanInstall = {true, true, false};
tCABINSTALL_CFG_InstallInformation			CABINSTALL_CFG_NetworkInstall = {false, false, false};


#ifdef __USE_CONFIG_FILE__
LOCAL bool vVCFGi_isProdDVBCDataLoaded = FALSE;
LOCAL tCFGF_LocDVBC vVCFGi_stProdDVBC;
#endif
/********************************************************/
/*			  Functions Definitions (GLOBAL)			*/
/********************************************************/
#ifdef __USE_CONFIG_FILE__
LOCAL bool VCFGi_GetProdDVBCFromConfigFile (void);
#endif

/******************************************************************************
 * Function Name : CABINSTALL_CFG_GetDefaultNetworkId
 *
 * Description   : This function give the default Network Id
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/

uint32_t  CABINSTALL_CFG_GetDefaultNetworkId(void)
{
	
#ifdef __USE_CONFIG_FILE__

	if (VCFGi_GetProdDVBCFromConfigFile())
	{
		if  (vVCFGi_stProdDVBC.ui16NetworkID != 0xFFFF)
		{
			return(vVCFGi_stProdDVBC.ui16NetworkID); 
		}
	}
#endif

	return CABINSTALL_CFG_NETWORK_ID;
}


 /******************************************************************************
 * Function Name : CABINSTALL_CFG_GetDefaultFrequency
 *
 * Description   : This function give the default Frequency
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
uint32_t  CABINSTALL_CFG_GetDefaultFrequency(void)
{

#ifdef __USE_CONFIG_FILE__
    

	if (VCFGi_GetProdDVBCFromConfigFile())
	{
		if  (vVCFGi_stProdDVBC.ui32Frequency != 0xFFFFFFFF)
		{
			return(vVCFGi_stProdDVBC.ui32Frequency); 
		}
	}
#endif

	return CABINSTALL_CFG_HOMETS_FREQUENCY;
}


 /******************************************************************************
 * Function Name : CABINSTALL_CFG_GetDefaultSymbolRate
 *
 * Description   : This function give the default Symbol rate
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
uint32_t  CABINSTALL_CFG_GetDefaultSymbolRate(void)
{

#ifdef __USE_CONFIG_FILE__
	if (VCFGi_GetProdDVBCFromConfigFile())
	{
		if  (vVCFGi_stProdDVBC.ui32SymbolRate != 0xFFFFFFFF)
		{
			return(vVCFGi_stProdDVBC.ui32SymbolRate); 
		}
	}
#endif

	return CABINSTALL_CFG_HOMETS_SYMBOL_RATE;
}

 /******************************************************************************
 * Function Name : CABINSTALL_CFG_GetDefaultModulation
 *
 * Description   : This function give the default Modulation
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
tCABINSTALL_CFG_Modulation_t  CABINSTALL_CFG_GetDefaultModulation(void)
{
   tCABINSTALL_CFG_Modulation_t modulation = CABINSTALL_CFG_HOMETS_MODULATION;

#ifdef __USE_CONFIG_FILE__

	if (VCFGi_GetProdDVBCFromConfigFile())
	{
      switch (vVCFGi_stProdDVBC.ui8Modulation)
      {
      case kCFGF_DVBC_MODULATION_BPSK:   modulation = eCABINSTALL_CFG_MOD_BPSK;   break;
      case kCFGF_DVBC_MODULATION_QPSK:   modulation = eCABINSTALL_CFG_MOD_QPSK;   break;
      case kCFGF_DVBC_MODULATION_QAM4:   modulation = eCABINSTALL_CFG_MOD_QAM4;   break;
      case kCFGF_DVBC_MODULATION_8PSK:   modulation = eCABINSTALL_CFG_MOD_8PSK;   break;
      case kCFGF_DVBC_MODULATION_QAM16:  modulation = eCABINSTALL_CFG_MOD_QAM16;  break;
      case kCFGF_DVBC_MODULATION_QAM32:  modulation = eCABINSTALL_CFG_MOD_QAM32;  break;
      case kCFGF_DVBC_MODULATION_QAM64:  modulation = eCABINSTALL_CFG_MOD_QAM64;  break;
      case kCFGF_DVBC_MODULATION_QAM128: modulation = eCABINSTALL_CFG_MOD_QAM128; break;
      case kCFGF_DVBC_MODULATION_QAM256: modulation = eCABINSTALL_CFG_MOD_QAM256; break;
      case kCFGF_DVBC_MODULATION_AUTO:   modulation = eCABINSTALL_CFG_MOD_ANALOG; break;
      case kCFGF_DVBC_MODULATION_ANALOG: modulation = eCABINSTALL_CFG_MOD_MAX;    break;
      /* Default */
      case 0xFF:
      default:                           modulation = CABINSTALL_CFG_HOMETS_MODULATION;
      }
	}
#endif

	return modulation;
}

/******************************************************************************
 * Function Name : CABINSTALL_CFG_GetDefaultSpectrum
 *
 * Description   : This function give the default spectrum
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
tCABINSTALL_CFG_Spectrum CABINSTALL_CFG_GetDefaultSpectrum(void)
{
	return CABINSTALL_CFG_HOMETS_SPECTRUM;
}

/******************************************************************************
 * Function Name : CABINSTALL_CFG_GetStartScanFrequency
 *
 * Description   : This function give the start scan frequency in Hz
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
uint32_t  CABINSTALL_CFG_GetStartScanFrequency(void)
{
	return (CABINSTALL_CFG_START_SCAN_FREQUENCY*1000);
}


/******************************************************************************
 * Function Name : CABINSTALL_CFG_GetStopScanFrequency
 *
 * Description   : This function give the stop scan frequency in Hz
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
uint32_t  CABINSTALL_CFG_GetStopScanFrequency(void)
{
	return (CABINSTALL_CFG_STOP_SCAN_FREQUENCY*1000);
}

/******************************************************************************
 * Function Name : CABINSTALL_CFG_GetStopScanFrequency
 *
 * Description   : This function give information is scanning installation is available
 *				   give the flags for installation
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
bool CABINSTALL_CFG_GetScanningInstallationConfig(tCABINSTALL_CFG_InstallInformation **scanInfo)
{

#ifdef LCN_YES
	CABINSTALL_CFG_ScanInstall.bLCNMngt = TRUE;
#else
	CABINSTALL_CFG_ScanInstall.bLCNMngt = FALSE;
#endif

	if(scanInfo != NULL)
	{
		*scanInfo = &CABINSTALL_CFG_ScanInstall;
		return true;
	}

	/* don't use scanning in cabinstall */
	return false;
}

/******************************************************************************
 * Function Name : CABINSTALL_CFG_GetStopScanFrequency
 *
 * Description   : This function give the stop scan frequency in Hz
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
bool CABINSTALL_CFG_GetNetworkInstallationConfig(tCABINSTALL_CFG_InstallInformation **NetInfo)
{
#ifdef LCN_YES
	CABINSTALL_CFG_NetworkInstall.bLCNMngt = TRUE;
#else
	CABINSTALL_CFG_NetworkInstall.bLCNMngt = FALSE;
#endif
	if(NetInfo != NULL)
	{
		*NetInfo = &CABINSTALL_CFG_NetworkInstall;
		return true;
	}
	return false;
}

/******************************************************************************
* Locales Functions Implementation
******************************************************************************/

#ifdef __USE_CONFIG_FILE__
LOCAL bool VCFGi_GetProdDVBCFromConfigFile 
(
   void
)
{
   tCFGF_Configuration *p_cfg;

   if (vVCFGi_isProdDVBCDataLoaded) return TRUE;

   p_cfg = CFGF_Read(kCFGF_ID_LOC_DVBC);
   if (p_cfg == NULL) return FALSE;
   memcpy(&vVCFGi_stProdDVBC,p_cfg->pstTableDVBC,sizeof(tCFGF_LocDVBC));
   CFGF_Free(p_cfg);
   vVCFGi_isProdDVBCDataLoaded = TRUE;

   return TRUE;
}

#endif


#ifdef NAGRA_NVOD
/******************************************************************************
 * Function Name : CABINSTALL_CFG_GetNvodInstallationParameters
 *
 * Description   : Sets the parameters for NVOD installation. Change the parameters inside the function
 *			    to install NVOD services from a different location
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/

tCABINSTALL_CFG_NVOD_Report CABINSTALL_CFG_GetNvodInstallationParameters(tCABINSTALL_CFG_NVOD_InstallInformation* pNvodParams)
{
	/************************************************/
	/* 			Change these to edit nvod settings		*/
	/************************************************/
	 
	 uint16_t ts_ids[] = {0x1001,0x1001,0x1001,0x1001,0x1001};
	 uint16_t ONID = 0x233a ;
	 uint16_t networkId =0x300d ;
	 uint16_t service_ids[]={0x123f,0x10ff,0x113f,0x1081,0x1041};
	 uint8_t numberOfNvodServices = 5;
	 uint32_t symbol_rate = 6875;
	 uint32_t frequency = 666000;
	 uint8_t modulation = 0x3;//eTABLE_DVB_CABLE_MODULATION_QAM64;
#ifdef QA_RELEASE
	 uint16_t lcns[]=  {389,212,204,262,306};
#else
	 uint16_t lcns[]= {2000,2001,2002,2003,2004};
#endif
	 
	/************************************************/
	/* 												*/
	/************************************************/
	
	 uint8_t i;
	 uint8_t count_ts,count_sid,count_lcns;

	 count_ts = count_sid = count_lcns = 0;
	 
	 if(pNvodParams != NULL)
	 {

			pNvodParams->ONID = ONID;
			pNvodParams->network_id = networkId;
			pNvodParams->modulation = modulation;
			
			if(numberOfNvodServices != 0)
			{
				pNvodParams->numberOfNvodServices = numberOfNvodServices;
			}
			else
				return eCABINSTALL_CFG_NVOD_PARAM_NUMBER_OF_SERVICES_ERROR;

			if(symbol_rate != 0)
			{
				pNvodParams->symbol_rate = symbol_rate;
			}
			else
				return eCABINSTALL_CFG_NVOD_PARAM_SYMBOL_RATE_ERROR;
			
			if(frequency != 0)
			{
				pNvodParams->frequency = frequency;
			}
			else
				return eCABINSTALL_CFG_NVOD_PARAM_FREQUENCY_ERROR;


			//check for argument count mismatch
			count_ts = sizeof(ts_ids)/sizeof(ts_ids[0]);
			count_sid = sizeof(service_ids)/sizeof(service_ids[0]);
			count_lcns = sizeof(lcns)/sizeof(lcns[0]);


			// check if there is a mismatch between a number of arguments in arrays. number of elements in array must be equal to a number of services param.
			if((count_ts != numberOfNvodServices)||(count_sid != numberOfNvodServices)||(count_lcns != numberOfNvodServices))
			{
				return eCABINSTALL_CFG_NVOD_PARAMS_COUNT_MISMATCH_ERROR;
			}
			else
			{
			
				TKEL_Malloc(numberOfNvodServices*sizeof(ts_ids[0]),&(pNvodParams->ts_ids));
				TKEL_Malloc(numberOfNvodServices*sizeof(lcns[0]),&(pNvodParams->lcns));
				TKEL_Malloc(numberOfNvodServices*sizeof(service_ids[0]),&(pNvodParams->service_ids));
				
				memcpy(pNvodParams->ts_ids, ts_ids, numberOfNvodServices*sizeof(ts_ids[0]));
				memcpy(pNvodParams->lcns, lcns, numberOfNvodServices*sizeof(lcns[0]));
				memcpy(pNvodParams->service_ids, service_ids, numberOfNvodServices*sizeof(service_ids[0]));
				
				
			}
			
		
				
				
	 }
	return eCABINSTALL_CFG_NVOD_PARAMS_OK;
}

void CABINSTALL_CFG_FreeNvodInstallationParameters(tCABINSTALL_CFG_NVOD_InstallInformation* pNvodParams)
{
    TKEL_Free(pNvodParams->ts_ids);
    TKEL_Free(pNvodParams->lcns);
    TKEL_Free(pNvodParams->service_ids);

    pNvodParams->ts_ids = NULL;
    pNvodParams->lcns = NULL;
    pNvodParams->service_ids = NULL;
}
#endif

