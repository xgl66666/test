/******************************************************************************
 *                         COPYRIGHT 2005 IWEDIA TECHNOLOGIES                 *
 ******************************************************************************
 *
 * MODULE NAME: SYS
 *
 * FILE NAME: $URL $
 *            $Rev $
 *            $Date $
 *
 * DESCRIPTION : functions needed to get information about the system version
 *
 *****************************************************************************/


/********************************************************/
/*              Includes                                */
/********************************************************/
#include "crules.h"
#include "satinstall_cfg.h"

/********************************************************/
/*              Defines                                 */
/********************************************************/
#define kSATINSTALL_CFG_NB_SYMBOL_RATE 3
/********************************************************/
/*			  Local File Variables (LOCAL)			*/
/********************************************************/

/********************************************************/
/*			  Functions Definitions (GLOBAL)			*/
/********************************************************/
uint32_t SATINSTALL_CFG_DefaultSymbolRate[kSATINSTALL_CFG_NB_SYMBOL_RATE+1]=
{
	22000000,
	27500000,
	30000000,
	0
};	

/******************************************************************************
 * Function Name : SATINSTALL_CFG_GetDefaultSymbolRateList
 *
 * Description   : This function give the default Symbol rate list
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
uint32_t* SATINSTALL_CFG_GetDefaultSymbolRateList(void)
{
	return SATINSTALL_CFG_DefaultSymbolRate;
}
 
/******************************************************************************
 * Function Name : SATINSTALL_CFG_GetNbSymbolRate
 *
 * Description   : This function give nb of Symbol rate
 *
 * Side effects  : none
 *
 * Comment	     : none
 *
 *****************************************************************************/
uint8_t SATINSTALL_CFG_GetNbSymbolRate(void)
{
	return kSATINSTALL_CFG_NB_SYMBOL_RATE;
}
