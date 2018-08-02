/*****************************************************************************
 *                  COPYRIGHT 2001 IWEDIA TECHNOLOGIES            *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_FS MODULE
 *
 * FILE NAME:   TDAL_FS_P.H
 *   $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_fs/osplus/tdal_fs_lvm_p.h $
 *   $Rev: 414 $
 *   $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef   _TDAL_FS_LVM_P_H_
#define   _TDAL_FS_LVM_P_H_

#ifdef   __cplusplus
extern   "C"   {
#endif

/*****************************************************************************
 *  Defines                                              *
 *****************************************************************************/
#define   kTDAL_FS_MAX_DEVICES      5
#define   kTDAL_FS_MAX_VOLUMES_PER_DEVICE   4


/*****************************************************************************
 *  Macros                                               *
 *****************************************************************************/


/*****************************************************************************
 *  Structures                                           *
 *****************************************************************************/
struct   TDAL_FS_PrivateDeviceInfo_p
{
   tTDAL_FS_DeviceHandle   handle   ;
   tTDAL_FS_DeviceName   name   ;
   tTDAL_FS_DeviceType   type;
   uint32_t      block_size   ;
   uint32_t      block_count   ;
   
   /*   private   fields   */
   uint8_t        volumes_count;
};


struct   TDAL_FS_PrivateVolumeInfo_p
{
   uint8_t        valid_flag;
   
   /*   fields   from   public   structure   tTDAL_FS_VolumeInfo   */
   tTDAL_FS_VolumeHandle   handle   ;
   tTDAL_FS_VolumeName   name   ;
   uint32_t      size;   /*   in   blocks   (see   'block_size'   field   of   tTDAL_FS_DeviceInfo   structure)   */
   tTDAL_FS_VolumeType   type;
   
   /*   private   fields   */
   tTDAL_FS_DeviceHandle   device_handle;
}   ;


/*****************************************************************************
 *  Typedefs                                            *
 *****************************************************************************/
/*   TODEL   typedef   void   (*tTDAL_FS_DeviceNotificationCallback)   (
            char   *            pDeviceName   ,
            char   *            pDeviceType   ,
            tTDAL_FS_DeviceEvent      DeviceEvent   );   */



/*****************************************************************************
 *  Variables declarations (IMPORT)                              *
 *****************************************************************************/


/*****************************************************************************
 *  Functions Definitions                                    *
 *****************************************************************************/                      
/**   Init/Term   functions   */

/*   TODEL   tTDAL_FS_Error   TDAL_FSi_Init(void);*/

tTDAL_FS_Error      TDAL_FSi_SetupAtapiDevice(void);

/*   TODEL   tTDAL_FS_Error   TDAL_FSi_RegisterNotificationCallback(   tTDAL_FS_DeviceNotificationCallback   callback   );   */

/*   TODEL   tTDAL_FS_Error   TDAL_FSi_Term(void);   */



#ifdef   __cplusplus
}
#endif


#endif   /*_TDAL_FS_LVM_P_H_*/

