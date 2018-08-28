/******************************************************************************
 *                         COPYRIGHT 2005 IWEDIA TECHNOLOGIES                         *
 ******************************************************************************
 *
 * MODULE NAME:
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/cabinstall_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: functions needed
 *
 *****************************************************************************/


#ifndef _CABINSTALL_CFG_H_
#define _CABINSTALL_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    eCABINSTALL_CFG_MOD_BPSK = 0,
    eCABINSTALL_CFG_MOD_QPSK,
    eCABINSTALL_CFG_MOD_QAM4 = eCABINSTALL_CFG_MOD_QPSK,
    eCABINSTALL_CFG_MOD_8PSK,
    eCABINSTALL_CFG_MOD_QAM16,
    eCABINSTALL_CFG_MOD_QAM32,
    eCABINSTALL_CFG_MOD_QAM64,
    eCABINSTALL_CFG_MOD_QAM128,
    eCABINSTALL_CFG_MOD_QAM256,
    eCABINSTALL_CFG_MOD_AUTO,
    eCABINSTALL_CFG_MOD_ANALOG,
    eCABINSTALL_CFG_MOD_MAX
} tCABINSTALL_CFG_Modulation_t;

typedef enum
{
	eCABINSTALL_CFG_SPECTRUM_AUTO = 0,
	eCABINSTALL_CFG_SPECTRUM_NORMAL,
	eCABINSTALL_CFG_SPECTRUM_INVERTED,
	eCABINSTALL_CFG_SPECTRUM_MAX
} tCABINSTALL_CFG_Spectrum;


typedef struct
{
	bool						bInstallPATIfNoSDT;		/* install on PAT if no SDT found 						*/
	bool						bInstallSdtOnly;		/* install services in SDT whithout cross checking NIT  */
	bool						bLCNMngt;				/* LCN mngt ; yes = true								*/
} tCABINSTALL_CFG_InstallInformation;

#ifdef NAGRA_NVOD
typedef struct 
{
	uint16_t 			ONID;
	uint16_t			network_id;
	uint8_t				numberOfNvodServices;
	uint16_t			*ts_ids;
	uint16_t			*lcns;
	uint16_t			*service_ids;
	uint32_t			frequency;
	uint32_t			symbol_rate;
	uint8_t 			modulation;
} tCABINSTALL_CFG_NVOD_InstallInformation;

typedef enum
{
	eCABINSTALL_CFG_NVOD_PARAMS_OK,
	eCABINSTALL_CFG_NVOD_PARAM_NID_ERROR,
	eCABINSTALL_CFG_NVOD_PARAM_ONID_ERROR,
	eCABINSTALL_CFG_NVOD_PARAM_TS_IDS_ERROR,
	eCABINSTALL_CFG_NVOD_PARAM_LCN_ERROR,
	eCABINSTALL_CFG_NVOD_PARAM_SERVICE_IDS_ERROR,
	eCABINSTALL_CFG_NVOD_PARAM_NUMBER_OF_SERVICES_ERROR,
	eCABINSTALL_CFG_NVOD_PARAM_SYMBOL_RATE_ERROR,
	eCABINSTALL_CFG_NVOD_PARAM_FREQUENCY_ERROR,
	eCABINSTALL_CFG_NVOD_PARAMS_COUNT_MISMATCH_ERROR
	
} tCABINSTALL_CFG_NVOD_Report;
#endif /*NAGRA_NVOD*/

/*******************************************************/
/*              Functions                              */
/*******************************************************/
uint32_t  CABINSTALL_CFG_GetDefaultNetworkId(void);
uint32_t  CABINSTALL_CFG_GetDefaultFrequency(void);
uint32_t  CABINSTALL_CFG_GetDefaultSymbolRate(void);
tCABINSTALL_CFG_Modulation_t  CABINSTALL_CFG_GetDefaultModulation(void);
tCABINSTALL_CFG_Spectrum CABINSTALL_CFG_GetDefaultSpectrum(void);
uint32_t  CABINSTALL_CFG_GetStartScanFrequency(void);
uint32_t  CABINSTALL_CFG_GetStopScanFrequency(void);
bool CABINSTALL_CFG_GetScanningInstallationConfig(tCABINSTALL_CFG_InstallInformation **scanInfo);
bool CABINSTALL_CFG_GetNetworkInstallationConfig(tCABINSTALL_CFG_InstallInformation **NetInfo);

#ifdef NAGRA_NVOD
tCABINSTALL_CFG_NVOD_Report CABINSTALL_CFG_GetNvodInstallationParameters(tCABINSTALL_CFG_NVOD_InstallInformation* pNvodParams);
void CABINSTALL_CFG_FreeNvodInstallationParameters(tCABINSTALL_CFG_NVOD_InstallInformation* pNvodParams);
#endif /*NAGRA_NVOD*/


#ifdef __cplusplus
}
#endif

#endif /*_CABINSTALL_CFG_H_*/
