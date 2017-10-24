/*****************************************************************************
 *            COPYRIGHT 2001 IWEDIA TECHNOLOGIES       *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_FS MODULE
 *
 * FILE NAME:   TDAL_FS.H
 *   $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_fs.h $
 *   $Rev: 66 $
 *   $Date: 2008-10-14 10:29:19 +0200 (mar., 14 oct. 2008) $
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _TDAL_FS_H_
#define _TDAL_FS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *  Defines                             *
 *****************************************************************************/
/** Mode defines **/
#define kTDAL_FS_MODE_READ   0x01
#define kTDAL_FS_MODE_WRITE   0x02
#define kTDAL_FS_MODE_RDWR   ( kTDAL_FS_MODE_READ | kTDAL_FS_MODE_WRITE )
#define kTDAL_FS_MODE_APPEND   0x04
#define kTDAL_FS_MODE_TRUNC   0x08
#define kTDAL_FS_MODE_CREATE   0x10
#define kTDAL_FS_MODE_EXCL   0x20

/** Flag defines **/
#define kTDAL_FS_FLAG_DIR   0x01
#define kTDAL_FS_FLAG_FILE   0x02

/** max length for 'name' field of DirEntry structure **/
#define kTDAL_FS_DIRENTRY_NAME_LEN_MAX  255
/** max length for 'volume_label' field of FormatParam.u.e2fs structure **/
#define kTDAL_FS_E2FS_VOLUME_LABEL_LEN_MAX 16
/** size for 'volume_id' field of FormatParam.u.e2fs structure **/
#define kTDAL_FS_E2FS_VOLUME_ID_SIZE    16

/** value for an invalid handle **/
#define kTDAL_FS_INVALID_HANDLE       0xFFFFFFFF

/** max length for 'tTDAL_FS_DeviceName' type **/
#define kTDAL_FS_DEVICE_NAME_LEN_MAX  16
/** max length for 'tTDAL_FS_VolumeName' type **/
#define kTDAL_FS_VOLUME_NAME_LEN_MAX  16
/** max length for 'tTDAL_FS_MountPointNameName' type **/
#define kTDAL_FS_MOUNTPOINT_NAME_LEN_MAX 16


#define kTDAL_FS_EXTENDEDINFO_NAME_LEN_MAX  255
/*****************************************************************************
 *  Macros                              *
 *****************************************************************************/


/*****************************************************************************
 *  Typedefs                            *
 *****************************************************************************/
/* tTDAL_FS_Error */
typedef enum
{
   eTDAL_FS_NO_ERROR ,
   eTDAL_FS_ERROR_BAD_ARG ,
   eTDAL_FS_ERROR_FILE_NOT_FOUND ,
   eTDAL_FS_ERROR_FILE_EXISTS ,
   eTDAL_FS_ERROR_DIR_EXISTS ,
   eTDAL_FS_ERROR_DIR_NOT_FOUND ,
   eTDAL_FS_ERROR_FILE_USED ,
   eTDAL_FS_ERROR_DIR_NOT_EMPTY,
   eTDAL_FS_ERROR_NO_MEMORY,
   eTDAL_FS_ERROR_NOT_FORMATED,
   eTDAL_FS_ERROR_NOT_INIT,
   eTDAL_FS_ERROR_CORRUPTED,
   eTDAL_FS_ERROR_NOT_DONE,
   eTDAL_FS_ERROR_ALREADY_INIT,
   eTDAL_FS_ERROR_LVM_FAILED

}   tTDAL_FS_Error; /* TDAL_FS error messages */

typedef enum
{
   eTDAL_FS_DEVICETYPE_UNKNOWN = 0,
   eTDAL_FS_DEVICETYPE_FIXED_STORAGE,
   eTDAL_FS_DEVICETYPE_REMOVABLE_STORAGE
}   tTDAL_FS_DeviceType;


/** Devices Typedefs **/

   /* tTDAL_FS_DeviceHandle */
   typedef uint32_t   tTDAL_FS_DeviceHandle;

   /* tTDAL_FS_DeviceName */
   typedef char    tTDAL_FS_DeviceName[kTDAL_FS_DEVICE_NAME_LEN_MAX];

   /* tTDAL_FS_DeviceInfo */
   typedef struct
   {
    tTDAL_FS_DeviceHandle   handle ;
    tTDAL_FS_DeviceName   name ;
    tTDAL_FS_DeviceType   type;
    uint32_t       block_size ;
    uint32_t       block_count ;
   } tTDAL_FS_DeviceInfo;

   typedef char    tTDAL_FS_ExtendedInfo_Name[kTDAL_FS_EXTENDEDINFO_NAME_LEN_MAX];

   typedef struct
   {
    tTDAL_FS_ExtendedInfo_Name     manufacturer;
    tTDAL_FS_ExtendedInfo_Name     product;
    tTDAL_FS_ExtendedInfo_Name     serial;
   } tTDAL_FS_DeviceExtendedInfo;

   /* tTDAL_FS_DeviceEvent */
   typedef enum
   {
    eTDAL_FS_DEVICE_EVENT_INSERTED ,
    eTDAL_FS_DEVICE_EVENT_REMOVED
   } tTDAL_FS_DeviceEvent;

   /* tTDAL_FS_DeviceCallback */
   typedef void (*tTDAL_FS_DeviceCallback) (
           tTDAL_FS_DeviceInfo *   pDeviceInfo ,
           tTDAL_FS_DeviceEvent    DeviceEvent );

/** Volumes Typedefs **/

   /* tTDAL_FS_VolumeHandle */
   typedef uint32_t   tTDAL_FS_VolumeHandle;

   /* tTDAL_FS_VolumeName */
   typedef char    tTDAL_FS_VolumeName[kTDAL_FS_VOLUME_NAME_LEN_MAX];

   /* tTDAL_FS_MountPointName */
   typedef char    tTDAL_FS_MountPointName[kTDAL_FS_MOUNTPOINT_NAME_LEN_MAX];

   /* tTDAL_FS_VolumeType */
   typedef enum
   {
    eTDAL_FS_VOLUME_TYPE_E2FS ,
    eTDAL_FS_VOLUME_TYPE_FAT12 ,
    eTDAL_FS_VOLUME_TYPE_FAT16 ,
    eTDAL_FS_VOLUME_TYPE_FAT32 ,
    eTDAL_FS_VOLUME_TYPE_FAT32_LBA ,
    eTDAL_FS_VOLUME_TYPE_NTFS ,
    eTDAL_FS_VOLUME_TYPE_E3FS ,
    eTDAL_FS_VOLUME_TYPE_ISO9660 ,
    eTDAL_FS_VOLUME_TYPE_SMBFS ,
    eTDAL_FS_VOLUME_TYPE_UNKNOWN
   } tTDAL_FS_VolumeType;

   /* tTDAL_FS_VolumeInfo */
   typedef struct
   {
    tTDAL_FS_VolumeHandle   handle ;
    tTDAL_FS_VolumeName    name ;
    uint32_t         size; /* in blocks (see 'block_size' field of tTDAL_FS_DeviceInfo structure) */
    tTDAL_FS_VolumeType    type;
   } tTDAL_FS_VolumeInfo;

   typedef struct
   {
    tTDAL_FS_VolumeName     VolumeName;
   } tTDAL_FS_VolumeExtendedInfo;


   /* tTDAL_FS_FsInfo */
   typedef struct
   {
    uint32_t         freeBlocks ;   /*Number of free 512 bytes blocks*/
    uint32_t         allocationUnit ; /*Number of 512 bytes blocks in a cluster*/
    uint32_t         totalSize ; /*total size of the volume in 512 bytes blocks*/
   } tTDAL_FS_FsInfo;

/* tTDAL_FS_Seek_Origin */
typedef enum
{
  eTDAL_FS_SEEK_ORIGIN_BEGIN = 0,
  eTDAL_FS_SEEK_ORIGIN_CURRENT = 1,
  eTDAL_FS_SEEK_ORIGIN_END = 2
} tTDAL_FS_Seek_Origin; /* defines the origin for TDAL_FS_Seek */


/* tTDAL_FS_FILE */
typedef uint32_t  tTDAL_FS_File;   /* file handle */


/* tTDAL_FS_DIR */
typedef uint32_t   tTDAL_FS_Dir;   /* directory handle */


/* tTDAL_FS_Size */
typedef int32_t   tTDAL_FS_Size;


/* tTDAL_FS_DirEntry */
typedef struct
{
   char     name[kTDAL_FS_DIRENTRY_NAME_LEN_MAX];
   tTDAL_FS_Size    size;
   uint8_t    flag;   /* File | Directory */
} tTDAL_FS_DirEntry;   /* directory entry */


/* tTDAL_FS_FatType */
typedef enum
{
   eTDAL_FS_FAT_TYPE_FAT12 ,
   eTDAL_FS_FAT_TYPE_FAT16 ,
   eTDAL_FS_FAT_TYPE_FAT32 ,
   eTDAL_FS_FAT_TYPE_ANY
} tTDAL_FS_FatType;


/* tTDAL_FS_FormatParam */
typedef struct
{
   bool   delete_all;
   union {
    struct {
      uint16_t   block_size;
      uint8_t   inode_ratio;
      uint8_t   volume_id[kTDAL_FS_E2FS_VOLUME_ID_SIZE];
      char    volume_label[kTDAL_FS_E2FS_VOLUME_LABEL_LEN_MAX];
    } e2fs;
    struct {
      tTDAL_FS_FatType   type;
    } fat;
   } u;
} tTDAL_FS_FormatParam;

typedef struct
{
   uint8_t  Mode;
   uint32_t Size;
   uint32_t uiBlockSize;
   uint32_t liATime;
   uint32_t liMTime ;
   uint32_t liCTime ;
} tTDAL_FS_Stat;
/*****************************************************************************
 *  Variables declarations (IMPORT)                  *
 *****************************************************************************/


/*****************************************************************************
 *  Functions Definitions                       *
 *****************************************************************************/

/**  Init/Term module functions */
   tTDAL_FS_Error   TDAL_FS_Init( void );

   tTDAL_FS_Error   TDAL_FS_Term( void );

/**  Revision module functions */
   const char    *TDAL_FS_APIRevisionGet(void );
   
   const char   *TDAL_FS_PlatformRevisionGet(void );


/**  Init/Term LVM functions */

   tTDAL_FS_Error   TDAL_FS_RegisterDeviceCallback( tTDAL_FS_DeviceCallback callback );


/**  Devices functions */

   tTDAL_FS_Error   TDAL_FS_ListDevices(
              uint8_t *       pDevicesCount ,
              tTDAL_FS_DeviceInfo **   ppDevicesArray );

   tTDAL_FS_Error   TDAL_FS_FreeDevicesArray(
              tTDAL_FS_DeviceInfo *   pDevicesArray );

   tTDAL_FS_Error   TDAL_FS_EraseDevice(
            tTDAL_FS_DeviceHandle   DeviceHandle );


tTDAL_FS_Error   TDAL_FS_GetDeviceInfo( tTDAL_FS_DeviceHandle   DeviceHandle,
                                         tTDAL_FS_DeviceInfo  *Info);

tTDAL_FS_Error   TDAL_FS_GetDeviceExtendedInfo( tTDAL_FS_DeviceHandle   DeviceHandle,
                                         tTDAL_FS_DeviceExtendedInfo  *ExtendedInfo);


/**  Volumes functions */

   tTDAL_FS_Error   TDAL_FS_ListVolumes(
              tTDAL_FS_DeviceHandle   DeviceHandle ,
              uint8_t *       pVolumesCount ,
              tTDAL_FS_VolumeInfo **   ppVolumesArray );

   tTDAL_FS_Error   TDAL_FS_FreeVolumesArray(
              tTDAL_FS_VolumeInfo *   pVolumesArray );

   tTDAL_FS_Error   TDAL_FS_CreateVolume(
              tTDAL_FS_DeviceHandle   DeviceHandle ,
              tTDAL_FS_VolumeInfo *   pVolumeInfo );

   tTDAL_FS_Error   TDAL_FS_DeleteVolume(
              tTDAL_FS_DeviceHandle   DeviceHandle ,
              tTDAL_FS_VolumeHandle   VolumeHandle );

   tTDAL_FS_Error   TDAL_FS_ChangeVolumeType(
              tTDAL_FS_DeviceHandle   DeviceHandle ,
              tTDAL_FS_VolumeHandle   VolumeHandle ,
              tTDAL_FS_VolumeType    VolumeType );

   tTDAL_FS_Error   TDAL_FS_GetVolumeExtendedInfo(
              tTDAL_FS_VolumeHandle VolumeHandle,
              tTDAL_FS_VolumeExtendedInfo  *ExtendedInfo);


/**  Mount/Umount/Format functions */

   tTDAL_FS_Error   TDAL_FS_Mount( tTDAL_FS_VolumeHandle   VolumeHandle ,
              const char *      pMountPoint ,
              uint8_t        Mode );

   tTDAL_FS_Error   TDAL_FS_Umount( const char *      pMountPoint );

   tTDAL_FS_Error   TDAL_FS_Check( tTDAL_FS_VolumeHandle   VolumeHandle );

   tTDAL_FS_Error   TDAL_FS_Format( tTDAL_FS_VolumeHandle   VolumeHandle ,
               tTDAL_FS_FormatParam *   pFormatParam );


/**  File access functions */

  /* Open/Close */
   tTDAL_FS_File   TDAL_FS_Open( const char * pFilename ,
              uint8_t   Mode );

   tTDAL_FS_Error   TDAL_FS_Close( tTDAL_FS_File  File );

  /* Read/Write/Flush */
   tTDAL_FS_Size   TDAL_FS_Read( tTDAL_FS_File   File ,
              void *    pBuffer ,
              tTDAL_FS_Size   Size );

   tTDAL_FS_Size   TDAL_FS_Write( tTDAL_FS_File  File ,
              void *   p_buffer ,
              tTDAL_FS_Size  Size );

   tTDAL_FS_Error   TDAL_FS_Flush( tTDAL_FS_File  File );

  /* Position functions */
   tTDAL_FS_Error   TDAL_FS_Seek( tTDAL_FS_File      File ,
              tTDAL_FS_Size      Size ,
              tTDAL_FS_Seek_Origin   Origin );

   tTDAL_FS_Error   TDAL_FS_Tell( tTDAL_FS_File   File ,
              tTDAL_FS_Size * pSize );

   tTDAL_FS_Error   TDAL_FS_Stat( tTDAL_FS_File   File ,
                tTDAL_FS_Stat * pStat );

  /* Operations on files */
   tTDAL_FS_Error   TDAL_FS_Remove( const char * pFilename );

   tTDAL_FS_Error   TDAL_FS_Rename( const char * pOldFilename ,
               const char * pNewFilename );

   tTDAL_FS_Error   TDAL_FS_FileExist( const char * pFilename );


/** Operations on directories **/

   tTDAL_FS_Error   TDAL_FS_MkDir(   const char *    pDirName );

   tTDAL_FS_Dir    TDAL_FS_OpenDir(   const char *    pDirName );

   tTDAL_FS_Error   TDAL_FS_RewindDir( tTDAL_FS_Dir    Dir );

   tTDAL_FS_Error   TDAL_FS_ReadDir(   tTDAL_FS_Dir    Dir ,
                  tTDAL_FS_DirEntry * pDirEntry );

   tTDAL_FS_Error   TDAL_FS_CloseDir(  tTDAL_FS_Dir    Dir );

   tTDAL_FS_Error   TDAL_FS_RmDir(   const char *    pDirName );


/** File System Information **/
   tTDAL_FS_Error   TDAL_FS_GetFsInfo( const char *    pMountPointName ,
                  tTDAL_FS_FsInfo *   pFsInfo );

   tTDAL_FS_Error TDAL_FS_UpdateFsInfo(const char *pMountPointName);

#ifdef __cplusplus
}
#endif


#endif /*_TDAL_FS_H_*/
