/******************************************************************************
 *                           COPYRIGHT 2005 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: backup mngr CFG 
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/backup_mngr_cfg.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _BACKUPMNGR_CFG_H_
#define _BACKUPMNGR_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif


/********************************************************/
/*														*/
/********************************************************/
typedef enum
{
	eBAM_CFG_InSeveralShot_IfOneShotHasFailed,				/* Try in one shot first, then in several shot 	*/
	eBAM_CFG_InSeveralShot_Only,								/* Do not try in one shot, only in several shot 	*/
	eBAM_CFG_InSeveralShot_IfOneShotHasFailed_or_LargeSize	/* Do no try in one shot if the size is too big 	*/
}tBAM_CFG_BackupMode;

/********************************************************/
/*														*/
/********************************************************/
tBAM_CFG_BackupMode BAM_CFG_GetBackupMode(void);
uint8_t BAM_CFG_GetCompressionLevel(void);
void BAM_CFG_GetBufferSizeList(uint8_t *puc_nbSize,uint32_t **pui_SizeList);
uint32_t 				BAM_CFG_GetTriggerSize(void);

	
#ifdef __cplusplus
}
#endif

#endif /* _BACKUPMNGR_CFG_H_ */
