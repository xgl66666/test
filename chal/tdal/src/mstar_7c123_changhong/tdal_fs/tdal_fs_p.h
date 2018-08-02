/*****************************************************************************
 *                  COPYRIGHT 2007 IWEDIA TECHNOLOGIES            *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_FS MODULE
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_fs/osplus/tdal_fs_p.h $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef   _TDAL_FS_P_H_
#define   _TDAL_FS_P_H_

#ifdef   __cplusplus
extern   "C"   {
#endif

/*****************************************************************************
 *  Defines                                              *
 *****************************************************************************/

/*****************************************************************************
 *  Macros                                               *
 *****************************************************************************/


/*****************************************************************************
 *  Typedefs                                            *
 *****************************************************************************/


/*****************************************************************************
 *  Variables declarations (IMPORT)                              *
 *****************************************************************************/

IMPORT char   *        TDAL_FS_VFS_FsName[];

/*****************************************************************************
 *  Functions Definitions                                    *
 *****************************************************************************/
IMPORT   tTDAL_FS_Error   pTDAL_FS_LVM_Init(void);
IMPORT   tTDAL_FS_Error   pTDAL_FS_LVM_Term(void);

IMPORT   tTDAL_FS_Error   pTDAL_FS_FS_Init(void);
IMPORT   tTDAL_FS_Error   pTDAL_FS_FS_Term(void);

/**   Init/Term   VFS   functions   */
IMPORT   tTDAL_FS_Error   pTDAL_FS_VFS_Init(void);
IMPORT   tTDAL_FS_Error   pTDAL_FS_VFS_Term(void);

/**   Init   USB   functions   **/
   tTDAL_FS_Error      pTDAL_FS_USB_Init(void);

   tTDAL_FS_Error      mTDAL_FS_PVR_NotifyRecordedContent(uint8_t *pMountPoint, uint32_t sizeKB);

IMPORT   tTDAL_FS_Error mTDAL_FS_MP_GetFileName(uint8_t* pFileName);
#ifdef   __cplusplus
}
#endif


#endif   /*_TDAL_FS_P_H_*/
