/******************************************************************************
 *                           COPYRIGHT 2008 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: CFG FLIP/FLOP
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/flipflop_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _FLIPFLOP_CFG_H_
#define _FLIPFLOP_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif





/*******************************************************/
/*               typedef                               */
/*******************************************************/

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
int32_t FLIPFLOP_GetCurrentMirrorID(uint32_t nPartitionId);

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
int32_t FLIPFLOP_GetLoadableMirrorID(uint32_t nPartitionId);

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
bool FLIPFLOP_SetCurrentMirrorID(uint32_t nPartitionId, uint32_t nMirrorId);

#ifdef __cplusplus
}
#endif

#endif /* _FLASH_CFG_H_ */
