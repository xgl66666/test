/******************************************************************************
 *                           COPYRIGHT 2005 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: MMI-CA CFG 
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/mmi_ca_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _MMI_CA_CFG_H_
#define _MMI_CA_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************/
/*               typedef                               */
/*******************************************************/
extern void MMI_CFG_Init(void);
extern void MMI_CFG_Terminate(void);	

extern void MMI_CFG_SetLanguage(char* language);
extern char *MMI_CFG_GetString(unsigned long StringId);


extern bool MMI_CFG_CreateMailArea(uint32_t size);
extern bool MMI_CFG_DeleteMailArea(void);
extern bool MMI_CFG_CreateNVMArea(uint32_t size);
extern bool MMI_CFG_DeleteNVMArea(void);


/*===========================================================================
 *
 * MMI_CFG_StoreMail
 *
 * Parameters:
 *      None
 *
 * Description:
 *      Sets a structure for the EIM capabilities.
 *   
 * Returns:
 *      
 *
 *===========================================================================*/
extern bool MMI_CFG_StoreMail(uint32_t offset, uint8_t *buffer, uint32_t size);
/*===========================================================================
 *
 * MMI_CFG_RestoreMail
 *
 * Parameters:
 *      None
 *
 * Description:
 *      	buffer allocated by client CCAS package
 *   
 * Returns:
 *      
 *
*===========================================================================*/
extern bool MMI_CFG_RestoreMail(uint32_t offset, uint8_t *Buffer, uint32_t size);
/*===========================================================================
 *
 * MMI_CFG_StoreBufferInNVM
 *
 * Parameters:
 *      None
 *
 * Description:
 *      	store data in NVM at a specific offset from the start address
 *			start address is not managed by mmi_ca_cfg.c ...	
 *   
 * Returns: true if OK else false
 *      
 *
*===========================================================================*/
extern bool MMI_CFG_StoreBufferInNVM(uint32_t offset,uint8_t *buffer, uint32_t size);
/*===========================================================================
 *
 * MMI_CFG_RestoreBufferFromNVM
 *
 * Parameters:
 *      None
 *
 * Description:
 *      	restore data from NVM at a specific offset from the start address
 *			start address is not managed by mmi_ca_cfg.c ...	
 *   
 * Returns:
 *      
 *
*===========================================================================*/
extern bool MMI_CFG_RestoreBufferFromNVM(uint32_t offset, uint32_t size, uint8_t *buffer);

#ifdef __cplusplus
}
#endif

#endif /* _MMI_CA_CFG_H_ */
