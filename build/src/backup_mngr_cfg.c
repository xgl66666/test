/********************************************************************************
 *                       COPYRIGHT 2005 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: APPLI CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/backup_mngr_cfg.c $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include "crules.h"
#include "backup_mngr_cfg.h"





/********************************************************/
/*              Defines                                 */
/********************************************************/
#define K 1024
/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/


/*===========================================================================
 *
 * BAM_CFG_GetBackupMode
 * Parameters:
 *
 *      none
 *
 * Description: 
 *      
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
tBAM_CFG_BackupMode BAM_CFG_GetBackupMode(void)
{
	/*eBAM_CFG_InSeveralShot_IfOneShotHasFailed				: Try in one shot first, then in several shot 		*/
	/*eBAM_CFG_InSeveralShot_Only,							: Do not try in one shot, only in several shot 	*/
	/*eBAM_CFG_InSeveralShot_IfOneShotHasFailed_or_LargeSize	: Do no try in one shot if the size is too big 		*/

	return eBAM_CFG_InSeveralShot_IfOneShotHasFailed_or_LargeSize;
}

/*===========================================================================
 *
 * BAM_CFG_GetCompressionLevel
 *
 * Parameters:
 *      none
 *
 * Description: 
 *      
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
uint8_t BAM_CFG_GetCompressionLevel(void)
{	
	return 6/*6 = Z_DEFAULT_COMPRESSION*/;
}



/*===========================================================================
 *
 * BAM_CFG_GetBufferSizes
 *
 * Parameters:
 *      none
 *
 * Description: For Several Shot mode : give a list of size to test.
 *      
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
void BAM_CFG_GetBufferSizeList(uint8_t *puc_nbSize,uint32_t **pui_SizeList)
{
	uint32_t		List[4]={400*K, 200*K, 100*K, 10*K};

	*puc_nbSize = 4;
	*pui_SizeList = List;
	
	return;
}


/*===========================================================================
 *
 * BAM_CFG_GetBufferSizes
 *
 * Parameters:
 *      none
 *
 * Description: For eBAM_CFG_InSeveralShot_IfOneShotHasFailed_or_LargeSize : give the size which triggers the several shot mode
 *      
 *
 * Returns:
 *      boolean
 *
 *===========================================================================*/
uint32_t BAM_CFG_GetTriggerSize(void)
{
	return 400*K;
}
