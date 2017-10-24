/********************************************************************************
 *                       COPYRIGHT 2005 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: ACCESSCTRL CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/accessctrl_cfg.c $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include <string.h>

#include "crules.h"
#include "accessctrl_cfg.h"





/********************************************************/
/*              Defines                                 */
/********************************************************/
#define kACCESSCTRL_CFG_DEFAULT_PINCODE	0000

#define kCOLORKEY_RED				71
#define kCOLORKEY_GREEN			72
#define kCOLORKEY_YELLOW			73
#define kCOLORKEY_BLUE				74
#define kCOLORKEY_WHITE			38


#define kACCESSCTRL_CFG_MasterPinCode_NbKeys			8
#define kACCESSCTRL_CFG_MasterPinCode_Sequence           {   kCOLORKEY_RED,            \
		                                                        kCOLORKEY_RED,            \
		                                                        kCOLORKEY_GREEN,          \
		                                                        kCOLORKEY_GREEN,          \
		                                                        kCOLORKEY_YELLOW,         \
		                                                        kCOLORKEY_YELLOW,         \
		                                                        kCOLORKEY_BLUE,           \
		                                                        kCOLORKEY_BLUE                \
										}

/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/
uint64_t kACCESSCTRL_CFG_DEFAULT_ZONEID = 0xFFFFFFFFFFFFFFFF;
LOCAL uint8_t	ACCESSCTRL_CFG_MasterPinCode_Sequence[kACCESSCTRL_CFG_MasterPinCode_NbKeys] = kACCESSCTRL_CFG_MasterPinCode_Sequence;

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/


/*===========================================================================
 *
 * ACCESSCTRL_CFG_GetDefaultPinCode
 *
 * Parameters:
 *      none
 *
 * Description:
 *      
 *
 * Returns:
 *      the default value of the pin code
 *
 *===========================================================================*/
uint16_t ACCESSCTRL_CFG_GetDefaultPinCode(void)
{

    return kACCESSCTRL_CFG_DEFAULT_PINCODE;
}

/*===========================================================================
 *
 * ACCESSCTRL_CFG_GetMasterPinCode
 *
 * Parameters:
 *      none
 *
 * Description:
 *      
 *
 * Returns:
 *      the default value of the pin code
 *
 *===========================================================================*/
void ACCESSCTRL_CFG_GetMasterPinCode(uint8_t	**Sequence,uint8_t *NbKeys)
{
	*Sequence = ACCESSCTRL_CFG_MasterPinCode_Sequence;
	*NbKeys = kACCESSCTRL_CFG_MasterPinCode_NbKeys;
	return;
}

/*===========================================================================
 *
 * ACCESSCTRL_CFG_GetParentalCtrlStatus
 *
 * Parameters:
 *     The 'status' output parameter is filled with 
 *     default Parental Control status:
 *      		0 : Disabled
 *				1 : Enabled
 *
 * Description:
 *      Fill 'status' parameter with default Parental Control status.
 *
 * Returns:
 *      None
 *
 *===========================================================================*/
void ACCESSCTRL_CFG_GetParentalCtrlStatus(uint8_t	*status)
{
	if (status != NULL)
	{
		*status = 0;
	}	
}

/*===========================================================================
 *
 * ACCESSCTRL_CFG_GetDefaultZoneId

 *
 * Parameters:
 *      none
 *
 * Description:
 *      
 *
 * Returns:
 *      the default value of the pin code
 *
 *===========================================================================*/
uint64_t ACCESSCTRL_CFG_GetDefaultZoneId(void)
{
    return kACCESSCTRL_CFG_DEFAULT_ZONEID;
}

