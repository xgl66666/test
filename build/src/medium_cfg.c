/********************************************************************************
 *                       COPYRIGHT 2005 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: MEDIUM CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/medium_cfg.c $
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
#include "medium_cfg.h"





/********************************************************/
/*              Defines                                 */
/********************************************************/

/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/


#define MED_CFG_MAX_NO_OF_MULTIPLEX                                   200
#define MED_CFG_MAX_NO_OF_ANALOG_RESERVED_MULTIPLEX                   0
#define MED_CFG_MAX_NO_OF_SERVICES                                     2000
#define MED_CFG_MAX_NO_OF_ES_DESCRIPTORS                         20

#define MED_CFG_MANUEL_INSTALL_SIGNAL_QUALITY               -1



/*===========================================================================
 *
 * MEDIUM_CFG_GetMaxNumberOf...
 *
 * Parameters:
 *      none
 *
 * Description: 
 *      
 *
 * Returns:
 *      int
 *
 *===========================================================================*/
uint16_t MEDIUM_CFG_GetMaxNumberOfMultiplex(void)
{
    return MED_CFG_MAX_NO_OF_MULTIPLEX;
}

uint16_t MEDIUM_CFG_GetMaxNumberOfAnalogMultiplex(void)
{
    return MED_CFG_MAX_NO_OF_ANALOG_RESERVED_MULTIPLEX;
}

uint16_t MEDIUM_CFG_GetMaxNumberOfServices(void)
{
    return MED_CFG_MAX_NO_OF_SERVICES;
}
uint8_t MEDIUM_CFG_GetMaxNumberOfESDescriptors(void)
{
    return MED_CFG_MAX_NO_OF_ES_DESCRIPTORS;
}


/*===========================================================================
 *
 * MEDIUM_CFG_GetManualInstallationSignalQuality
 *
 * Parameters:
 *      none
 *
 * Description: 
 *      
 *
 * Returns:
 *      int
 *
 *===========================================================================*/
int16_t MEDIUM_CFG_GetManualInstallationSignalQuality(void)
{
    return MED_CFG_MANUEL_INSTALL_SIGNAL_QUALITY;
}


