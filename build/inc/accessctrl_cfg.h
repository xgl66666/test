/******************************************************************************
 *                           COPYRIGHT 2005 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: ACCESSCTRL CFG 
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/accessctrl_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _ACCESSCTRL_CFG_H_
#define _ACCESSCTRL_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************/
/*               typedef                               */
/*******************************************************/


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
 *      
 *
 *===========================================================================*/
uint16_t ACCESSCTRL_CFG_GetDefaultPinCode(void);

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
 *      
 *
 *===========================================================================*/
void ACCESSCTRL_CFG_GetMasterPinCode(uint8_t	**Sequence,uint8_t *NbKeys);

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
void ACCESSCTRL_CFG_GetParentalCtrlStatus(uint8_t	*status);
	
#ifdef __cplusplus
}
#endif

#endif /* _ACCESSCTRL_CFG_H_ */
