/********************************************************************************
 *                       COPYRIGHT 2005 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: FLIP FLOP CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/flipflop_cfg.c $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/

#include "crules.h"
#include "string.h"

#include "flipflop_cfg.h"

static uint32_t gFLIPFLOP_nCurrentMirrorID = 1;
static uint32_t gFLIPFLOP_nLoadableMirrorID = 2;

/*===========================================================================
 *
 * FLIPFLOP_GetCurrentMirrorID
 *
 * Parameters: partition ID
 *		
 * Description:
 *
 * Returns:the current mirror ID (1 or 2)for this partition, -1 in case of error
 *
 *===========================================================================*/
int32_t FLIPFLOP_GetCurrentMirrorID(uint32_t nPartitionId)
{
	return gFLIPFLOP_nCurrentMirrorID;
}

/*===========================================================================
 *
 * FLIPFLOP_GetLoadableMirrorID
 *
 * Parameters: partition ID
 *		
 * Description:
 *
 * Returns:the loadable mirror ID (1 or 2)for this partition, -1 in case of error
 *
 *===========================================================================*/
int32_t FLIPFLOP_GetLoadableMirrorID(uint32_t nPartitionId)
{
	return gFLIPFLOP_nLoadableMirrorID;
}

/*===========================================================================
 *
 * FLIPFLOP_SetCurrentMirrorID
 *
 * Parameters: partition ID, use 0 for All partitions
 *		
 * Description:  define the new miror ID
 *
 * Returns: true but error
 *
 *===========================================================================*/
bool FLIPFLOP_SetCurrentMirrorID(uint32_t nPartitionId, uint32_t nMirrorId)
{
	bool bRet = FALSE;
	switch(nMirrorId)
	{
		case 1:
			gFLIPFLOP_nCurrentMirrorID = 1;
			gFLIPFLOP_nLoadableMirrorID = 2;
			bRet = TRUE;
		case 2:
			gFLIPFLOP_nCurrentMirrorID = 2;
			gFLIPFLOP_nLoadableMirrorID = 1;
			bRet = TRUE;
	}
	return bRet;
}
