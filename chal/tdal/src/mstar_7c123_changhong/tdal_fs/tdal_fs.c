/*****************************************************************************
 *                  COPYRIGHT 2006 IWEDIA TECHNOLOGIES            *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_FS MODULE
 *
 * FILE NAME:   TDAL_FS.C
 *   $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_fs/osplus/tdal_fs.c $
 *   $Rev: 414 $
 *   $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION
 *
 *****************************************************************************/

/*****************************************************************************
 *  Includes                                            *
 *****************************************************************************/
#include <crules.h>

#include "tdal_common.h"
#include "tdal_fs.h"
#include "tdal_fs_p.h"

#include "tbox.h"

#include "mdrv_types.h"

/*****************************************************************************
 *  Defines                                              *
 *****************************************************************************/
#ifdef   PRODUCT_USE_USB
#define   kTDAL_FS_OSPLUS_MEMORY_SIZE   (0x200000 + SCC_OSPLUS_NONCACHED_MEM_POOL_SIZE + 32   )
#else
#define   kTDAL_FS_OSPLUS_MEMORY_SIZE   0x200000
#endif

/*****************************************************************************
 *  Macros                                               *
 *****************************************************************************/


/*****************************************************************************
 *  Structures                                           *
 *****************************************************************************/


/*****************************************************************************
 *  Global Variables (GLOBAL/IMPORT)                             *
 *****************************************************************************/
mTBOX_SET_MODULE(eTDAL_FS);

//unsigned   char   TDAL_FS_OsplusMemory[kTDAL_FS_OSPLUS_MEMORY_SIZE];


/*****************************************************************************
 *  Local File Variables (LOCAL)                              *
 *****************************************************************************/
void   TDAl_FS_PrintMem(void)
{
      
}

/*****************************************************************************
 *  Global Functions Declarations (IMPORT)                         *
 *****************************************************************************/
/* extern   BOOL   SYSi_DCacheFlush(U8   *Buffer,U32   Size); */

/*****************************************************************************
 *  Local Functions Declarations (LOCAL)                         *
 *****************************************************************************/

/*****************************************************************************
 *  Functions Definitions (LOCAL/GLOBAL)                         *
 *****************************************************************************/


/******************************************************************************
 * Function Name : TDAL_FS_Init
 *
 * Description   : Module initialisation
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error   TDAL_FS_Init(void)
{
   tTDAL_FS_Error   result = eTDAL_FS_NO_ERROR;

   mTBOX_FCT_ENTER("TDAL_FS_Init");
   mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_Init()\n"));

#ifdef   PRODUCT_USE_USB
//   result = pTDAL_FS_USB_Init();
//   if   (   result != eTDAL_FS_NO_ERROR   )
//   {
//      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_USB_Init:   [ERROR]"));
//      mTBOX_RETURN(result);
//   }
#endif

   result = pTDAL_FS_LVM_Init();
   if   (   result != eTDAL_FS_NO_ERROR   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Init:   pTDAL_FS_LVM_Init:   [ERROR]"));
      mTBOX_RETURN(result);
   }
   result = pTDAL_FS_VFS_Init();
   if   (   result != eTDAL_FS_NO_ERROR   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Init:   pTDAL_FS_VFS_Init:   [ERROR]"));
      mTBOX_RETURN(result);
   }

   MsFS_Init(FALSE);
   mTBOX_RETURN(result);
}

/******************************************************************************
 * Function Name : TDAL_FS_Term
 *
 * Description   : Module Term
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error   TDAL_FS_Term(void)
{
   tTDAL_FS_Error   result = eTDAL_FS_NO_ERROR;

   mTBOX_FCT_ENTER("TDAL_FS_Term");
   mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_Term()\n"));

   result = pTDAL_FS_VFS_Term();
   if   (   result == eTDAL_FS_NO_ERROR   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_VFS_Term:   [ERROR]"));
      mTBOX_RETURN(result);
   }

   result = pTDAL_FS_LVM_Term();
   if   (   result == eTDAL_FS_NO_ERROR   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_LVM_Term:   [ERROR]"));
      mTBOX_RETURN(result);
   }

   mTBOX_RETURN(result);
}
