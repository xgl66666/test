/*****************************************************************************
 *                  COPYRIGHT 2006 IWEDIA TECHNOLOGIES            *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_FS MODULE
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_fs/osplus/tdal_fs_usb.c $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION
 *
 *****************************************************************************/

/*****************************************************************************
 *  Includes                                            *
 *****************************************************************************/
/*****   osplus   includes   *****/
/*   os_20to21_map.h   should   not   be   included   when   including   osplus   headers   */
#define   DVD_EXCLUDE_OS20TO21_MACROS

#include <crules.h>
#include "stdevice.h"
#include <stddefs.h>
#include "stsys.h"
#include "stos.h"

#include <tdal_common.h>
#include <pltf_hardcfg.h>

#include <usblink.h>
#include <osplus/registry.h>

#include <tdal_fs.h>
#include "tdal_fs_p.h"

#include <tbox.h>

IMPORT   U32   SYS_ChipVersion(void);   /*   extern   from   tdal_common   */

/*****************************************************************************
 *  Defines                                              *
 *****************************************************************************/
#define   USB_OHCI_CONFIG_PATH   "/hardware/device/usb/ohci0"
#define   USB_EHCI_CONFIG_PATH   "/hardware/device/usb/ehci0"
#define   USB_CONFIG_INT_LEVEL   "host   int   level"


/*****************************************************************************
 *  Macros                                               *
 *****************************************************************************/


/*****************************************************************************
 *  Typedefs                                            *
 *****************************************************************************/


/*****************************************************************************
 *  Global Variables (GLOBAL/IMPORT)                             *
 *****************************************************************************/
mTBOX_SET_MODULE(eTDAL_FS);


/*****************************************************************************
 *  Global Variables (GLOBAL/IMPORT)                             *
 *****************************************************************************/

IMPORT   ST_Partition_t      *TheOsplusPartition;
/*****************************************************************************
 *  Local File Variables (LOCAL)                              *
 *****************************************************************************/


/*****************************************************************************
 *  Global Functions Declarations (IMPORT)                         *
 *****************************************************************************/


/*****************************************************************************
 *  Local Functions Declarations (LOCAL)                         *
 *****************************************************************************/


/*****************************************************************************
 *  Functions Definitions (LOCAL/GLOBAL)                         *
 *****************************************************************************/



#if   defined(ST_7100) || defined(ST_7710)
static   U32   USBi_RegistryDisable(void)
{
   return(0);
}
#endif
/******************************************************************************
 * Function Name : pTDAL_FS_USB_Init
 *
 * Description   : Module initialisation
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error   pTDAL_FS_USB_Init(void)
{
   void   *NonCachedMem,*NonCachedMem_Aligned;
   tTDAL_FS_Error   result = eTDAL_FS_NO_ERROR;

   mTBOX_FCT_ENTER("pTDAL_FS_USB_Init");
   mTBOX_TRACE((kTBOX_NIV_5,"pTDAL_FS_USB_Init()\n"));

 /* USB special init on 7109 */
 /* ------------------------ */
 /* To be checked ? */

   NonCachedMem = STOS_MemoryAllocate(TheOsplusPartition,
                     SCC_OSPLUS_NONCACHED_MEM_POOL_SIZE+32);
   if (NonCachedMem==NULL)
   {
       mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_USB_Init:"
            "[ERROR] STOS_MemoryAllocate(non cached) failed\n"));
      result = eTDAL_FS_ERROR_NOT_DONE;
   }
   else
   {
   NonCachedMem_Aligned=(U8 *)((((U32)NonCachedMem)+31)&0xFFFFFFE0);
   }

   if(result == eTDAL_FS_NO_ERROR)
   {
#if   0
   /*   Configure   the   priorities   of   the   USB   tasks   */
   /*   -----------------------------------------   */
   registry_key_set(   USB_CONFIG_PATH,   USB_CONFIG_ENU_TASK_PRI,   (void   *)   MAX_USER_PRIORITY);
   registry_key_set(   USB_CONFIG_PATH,   USB_CONFIG_URB_TASK_PRI,   (void   *)   MAX_USER_PRIORITY);
#else
      if(registry_key_set(USB_CONFIG_PATH,
            USB_CONFIG_ENU_TASK_PRI,
            (void   *)(((MAX_USER_PRIORITY)/2)+2))   !=   0)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_USB_Init:"
            "[ERROR]   registry_key_set(USB_CONFIG_PATH,   ENU)   failed\n"));
      result = eTDAL_FS_ERROR_NOT_DONE;
      }
   
      if(registry_key_set(USB_CONFIG_PATH,
            USB_CONFIG_URB_TASK_PRI,
            (void   *)(((MAX_USER_PRIORITY)/2)+2))   !=   0)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_USB_Init:"
            "[ERROR]   registry_key_set(USB_CONFIG_PATH,   URB)   failed\n"));
      result = eTDAL_FS_ERROR_NOT_DONE;
      }
#endif
   }

   if(result == eTDAL_FS_NO_ERROR)
   {
/* Disable SOF workaround if not automatic */
 /* ----------------------------------------- */
      if (registry_key_set("/hardware/device/usb/ehci0",
                  "bad SOFs",
                  (void *)USBi_RegistryDisable) !=0)
      {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_USB_Init:"
                "[ERROR] registry_key_set(/hardware/device/usb/ehci0,bad SOFs) failed\n"));
        result = eTDAL_FS_ERROR_NOT_DONE;
      }
      if (registry_key_set("/hardware/device/usb/ohci0",
                  "bad SOFs",
                  (void *)USBi_RegistryDisable)!=0)
      {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_USB_Init:"
                "[ERROR] registry_key_set(/hardware/device/usb/ohci0,bad SOFs) failed\n"));
        result = eTDAL_FS_ERROR_NOT_DONE;
      }
   }

   if(result == eTDAL_FS_NO_ERROR)
   {
   /*   SoftConnex   USBLink   initialisation   */
   /*   ---------------------------------   */
      if(USBLINK_Initialize(NonCachedMem_Aligned,
            SCC_OSPLUS_NONCACHED_MEM_POOL_SIZE   ) == (-1))
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_USB_Init:"
            "[ERROR]   USBLINK_Initialize()   failed\n"));
      result = eTDAL_FS_ERROR_NOT_DONE;
   }
   }
   mTBOX_RETURN(result);
}

