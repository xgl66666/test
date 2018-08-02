/*****************************************************************************
 *                  COPYRIGHT 2006 IWEDIA TECHNOLOGIES            *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_FS MODULE
 *
 * FILE NAME:   TDAL_FS.C
 *   $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_fs/osplus/tdal_fs_vfs.c $
 *   $Rev: 414 $
 *   $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION
 *
 *****************************************************************************/

/*****************************************************************************
 *  Includes                                            *
 *****************************************************************************/
/*   os_20to21_map.h   should   not   be   included   when   including   osplus   headers   */
#define   DVD_EXCLUDE_OS20TO21_MACROS

#include <crules.h>
#include <string.h>   /*   for   strcpy   */

#include "cyg/libc/stdio/stdio.h"

#include <tbox.h>
#include <tdal_common.h>
#include "tdal_common_priv.h"
#include <errno.h>

#include <tdal_fs.h>
#include "tdal_fs_p.h"
#include "MsOS.h"
#include "MsFS.h"

#include "cyg/fileio/dirent.h"
#include "cyg/fileio/fileio.h"
#include "sys/stat.h"


#include "fcntl.h"


/*****************************************************************************
 *  Defines                                              *
 *****************************************************************************/
#define   kTDAL_FS_MAX_DIR_OPENED      40
#define   kTDAL_FS_VOLUME_NAME_LEN_MAX   16   /*   it   is   the   same   value   as   kILVM_VOLUME_NAME_LEN_MAX   in   ilvm.h   */

#define   kTDAL_FS_FAKE_CHECK_FOR_FAT32   0


#ifndef   TDAL_FS_RETRY_NBTIMES
   #define   TDAL_FS_RETRY_NBTIMES   0

   #ifndef   TDAL_FS_RETRY_DELAY
      #define   TDAL_FS_RETRY_DELAY   0
   #endif
#endif

#ifndef   TDAL_FS_READ_SLICE_SIZE
   #define   TDAL_FS_READ_SLICE_SIZE   0
#endif
#ifndef   TDAL_FS_WRITE_SLICE_SIZE
   #define   TDAL_FS_WRITE_SLICE_SIZE   0
#endif

#define   kTDAL_FS_VFS_UPDATE_THREAD_NAME    "TDAL_FS_VFS_Thread"
#define   kTDAL_FS_VFS_UPDATE_THREAD_PRIO    64
#define   kTDAL_FS_VFS_UPDATE_QUEUE_NAME     "VFS_UpdateQueue"
#define   kTDAL_FS_VFS_MAX_NUM_PARTITIONS    4

#define   kTDAL_FS_VFS_MAX_MOUNT_NAME_SIZE   20

/*****************************************************************************
 *  Macros                                               *
 *****************************************************************************/

/*****************************************************************************
 *  Typedefs                                            *
 *****************************************************************************/

/* tTDAL_FS_FsInfo */
typedef struct
{
    uint8_t         pMountPoint[20];
    tTDAL_FS_FsInfo stInfo;
} tTDAL_FS_ExtFsInfo;


/*****************************************************************************
 *  Global Variables (GLOBAL/IMPORT)                             *
 *****************************************************************************/
mTBOX_SET_MODULE(eTDAL_FS);


/*****************************************************************************
 *  Global Variables (GLOBAL/IMPORT)                             *
 *****************************************************************************/

char   *        TDAL_FS_VFS_FsName[] = { "e3fs" , "fatfs" , "fatfs" , "fatfs" ,"fatfs" , "ntfs" , "e3fs" ,"iso9660" , "smbfs" ,"unknown"  };

/*****************************************************************************
 *  Local File Variables (LOCAL)                              *
 *****************************************************************************/
 LOCAL MS_U8 tdal_mp_filename[1024] = {0};
LOCAL   char   *TDAL_FS_DirNameTable[kTDAL_FS_MAX_DIR_OPENED];
LOCAL   DIR   *      TDAL_FS_DirTable[kTDAL_FS_MAX_DIR_OPENED];
LOCAL   TDAL_mutex_id      TDAL_FS_DirMutex;

LOCAL   TDAL_mutex_id      TDAL_FS_OpsMutex;
LOCAL   uint8_t        TDAL_FS_OpsCounter;


LOCAL   TDAL_task_id    TDAL_FS_VFS_UpdateTaskId;
LOCAL   TDAL_queue_id          TDAL_FS_VFS_UpdateQueue = -1;
LOCAL   uint8_t         TDAL_FS_VFS_UpdateTaskStack[4096];

LOCAL   TDAL_mutex_id      TDAL_FS_VFS_CachedInfoMutex;
LOCAL   tTDAL_FS_ExtFsInfo TDAL_FS_VFS_CachedInfo[kTDAL_FS_VFS_MAX_NUM_PARTITIONS];

/*****************************************************************************
 *  Global Functions Declarations (IMPORT)                         *
 *****************************************************************************/
IMPORT   bool      TDAL_FSi_GetVolumeName(   tTDAL_FS_VolumeHandle   VolumeHandle   ,
                        char   *        pVolumeName);
IMPORT   tTDAL_FS_VolumeType   TDAL_FSi_GetFormatType(   tTDAL_FS_VolumeHandle   VolumeHandle   );


/*****************************************************************************
 *  Local Functions Declarations (LOCAL)                         *
 *****************************************************************************/
LOCAL   int   pTDAL_FS_getFileMode(uint8_t   Mode);
LOCAL   unsigned long  pTDAL_FS_getMountMode(   uint8_t   Mode);

/* This function is useful to count number of active operations
 * to perform a vfs_sync_all() if counter reach 0
 * Note:
 *  - in Open and OpenDir, we increase the counter
 *  - in Close and CloseDir, we decrease the counter
 *  - in Rename, Remove, MkDir and RmDir, we increase the counter at beginning
 *   and we decrease it at end.
 */
LOCAL   void      pTDAL_FS_IncreaseOpsCounter(   void   );
LOCAL   void      pTDAL_FS_DecreaseOpsCounter(   void   );

#if   0   /*   not   used   */
/*   a   debug   function   that   Print   DirtyFlag   for   FAT32   volumes   */
LOCAL   tTDAL_FS_Error      TDAL_FSd_PrintDirtyFlagForFAT32(   char   *   VolumeName   );
#endif

LOCAL void pTDAL_FS_VFS_Thread(void *data);
LOCAL tTDAL_FS_Error  pTDAL_FS_GetFsInfo( const char *pMountPointName,
                                          tTDAL_FS_FsInfo   *pFsInfo);


/*****************************************************************************
 *  Functions Definitions (LOCAL/GLOBAL)                         *
 *****************************************************************************/


/******************************************************************************
 * Function Name : pTDAL_FS_VFS_Init
 *
 * Description   : Module initialisation
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      pTDAL_FS_VFS_Init(   void   )
{
   tTDAL_FS_Error      result = eTDAL_FS_NO_ERROR;
   int                 error;
   uint8_t             index;

   mTBOX_FCT_ENTER("TDAL_VFS_Init");

   TDAL_CreateMutex(&TDAL_FS_DirMutex);
   TDAL_CreateMutex(&TDAL_FS_OpsMutex);

   TDAL_FS_OpsCounter = 0;

   for   (   index   =0;   index   <kTDAL_FS_MAX_DIR_OPENED;   index++)
   {
      TDAL_FS_DirNameTable[index] = NULL;
   }

   TDAL_CreateMutex(&TDAL_FS_VFS_CachedInfoMutex);

#if MSTAR_QUEUE
   TDAL_FS_VFS_UpdateQueue =  MsOS_CreateQueue(NULL,
                                               0,
                                               E_MSG_FIXED_SIZE,
                                               sizeof(tTDAL_FS_ExtFsInfo),
                                               E_MSOS_FIFO,
                                               kTDAL_FS_VFS_UPDATE_QUEUE_NAME);
#else
   TDAL_CreateQueue(TDAL_QUEUE_MSG_MAX_NO, sizeof(tTDAL_FS_ExtFsInfo), &TDAL_FS_VFS_UpdateQueue);
#endif
   if (TDAL_FS_VFS_UpdateQueue < 0)
   {
       mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_FS_Init: Failed to create update queue.\n"));
   }

   TDAL_FS_VFS_UpdateTaskId = TDAL_CreateTask(eTDAL_PRIORITY_NORMAL,
                                      kTDAL_FS_VFS_UPDATE_THREAD_NAME,
                                      TDAL_FS_VFS_UpdateTaskStack,
                                      sizeof(TDAL_FS_VFS_UpdateTaskStack),
                                      pTDAL_FS_VFS_Thread,
                                      NULL);
   if (TDAL_FS_VFS_UpdateTaskId == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Init() Failed to create thread\n"));
   }

   /*
   memset(   TDAL_FS_DirTable   ,
      0   ,
      kTDAL_FS_MAX_DIR_OPENED*sizeof(vfs_dir_t   *)   );
   memset(   TDAL_FS_DirNameTable   ,
      0   ,
      kTDAL_FS_MAX_DIR_OPENED*sizeof(tTDAL_FS_DirName)   );
   */

   /*   with   OS+   v2.x, we   can   configure   the   E2FS   cache   mode
      *   but   it   is   not   supported   by   OS+   1.2   so   we   put   this   code   between   flags
      *   These   flags   should   be   removed   when   we   would   use   OS+2   definitely   */
   #if   defined(E2FS_DEFAULT_CACHE_MODE) && defined(E2FS_REGISTRY_CACHE_MODE)
   if   (result == eTDAL_FS_NO_ERROR)
   {
      error = registry_key_set(   E2FS_REGISTRY_NODE,
                   E2FS_REGISTRY_CACHE_MODE,
                   (void   *)e2fs_cache_mode_soft_update   );
      if   (   error != 0   )
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Init:   registry_key_set(   CACHE_MODE   )   failed\n"));
      result = eTDAL_FS_ERROR_NOT_DONE;
      }
   }
   #endif

   if   (result == eTDAL_FS_NO_ERROR)
   {
      /*error = e2fs_vfs_init_p(   TheOsplusPartition   );
      if   (   error != 0   )
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Init:   e2fs_vfs_init()   failed\n"));
      result = eTDAL_FS_ERROR_NOT_DONE;
      }*/
   }

   if   (result == eTDAL_FS_NO_ERROR)
   {
      /*error = fat_vfs_init_p(   TheOsplusPartition   );
      if   (   error != 0   )
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Init:   fat_vfs_init()   failed\n"));
      result = eTDAL_FS_ERROR_NOT_DONE;
      }*/
   }

   return  result;
}

/******************************************************************************
 * Function Name : pTDAL_FS_VFS_Term
 *
 * Description   : Module term
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      pTDAL_FS_VFS_Term(   void   )
{
   tTDAL_FS_Error   result = eTDAL_FS_NO_ERROR;

   mTBOX_FCT_ENTER("TDAL_VFS_Term");

   TDAL_DeleteTask(&TDAL_FS_VFS_UpdateTaskId);

#if MSTAR_QUEUE
   MsOS_DeleteQueue(TDAL_FS_VFS_UpdateQueue);
#else
   TDAL_DeleteQueue(TDAL_FS_VFS_UpdateQueue);
#endif
   TDAL_DeleteMutex(TDAL_FS_VFS_CachedInfoMutex);
   TDAL_DeleteMutex(TDAL_FS_DirMutex);
   TDAL_DeleteMutex(TDAL_FS_OpsMutex);


   mTBOX_RETURN(result);
}


/******************************************************************************
 * Function Name : TDAL_FS_Mount
 *
 * Description   : Mount a file system on a volume
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_Mount(   tTDAL_FS_VolumeHandle      VolumeHandle   ,
               const   char   *      pMountPoint   ,
               uint8_t            Mode   )
{
	char volume_name[kTDAL_FS_VOLUME_NAME_LEN_MAX];
	tTDAL_FS_Error result = eTDAL_FS_NO_ERROR;
	bool b_result;
	int vfs_result;
	tTDAL_FS_VolumeType volume_type;

	mTBOX_FCT_ENTER("TDAL_FS_Mount");

	mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_Mount(   VolumeHandle=0x%x   , pMountPoint=\"%s\"   , Mode=0x%02x   )\n" ,
					VolumeHandle , pMountPoint , Mode ));

	b_result = TDAL_FSi_GetVolumeName(VolumeHandle, volume_name);
	if (b_result == TRUE)
	{
		volume_type = TDAL_FSi_GetFormatType(VolumeHandle);
		mTBOX_TRACE((kTBOX_NIV_1, "TDAL_FS_Mount:   VolumeName=\"%s\"\n", volume_name));
		vfs_result = mount(volume_name, pMountPoint, TDAL_FS_VFS_FsName[volume_type]);/*MsFS_Mount(volume_name, pMountPoint,
				TDAL_FS_VFS_FsName[volume_type],pTDAL_FS_getMountMode(Mode), NULL);*/
		if (vfs_result == -1)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Mount:   vfs_mount   failed   (errno=%d)\n", errno ));

			if (errno == EAGAIN)
			{
				result = eTDAL_FS_ERROR_CORRUPTED;
			}
#if   (TDAL_FS_RETRY_NBTIMES   !=0   )
			else if (errno == EIO)
			{
				uint8_t nbtimes = 0;
				do
				{
#if   (TDAL_FS_RETRY_DELAY   !=0   )
					task_delay((ST_GetClocksPerSecond()*TDAL_FS_RETRY_DELAY)/1000);
#endif
					vfs_result = mount(volume_name, pMountPoint, TDAL_FS_VFS_FsName[volume_type]);/*MsFS_Mount(volume_name, pMountPoint,
							TDAL_FS_VFS_FsName[volume_type],
							pTDAL_FS_getMountMode(Mode));*/
					nbtimes++;
				} while ((vfs_result == -1) && (errno == EIO)
						&& (nbtimes < TDAL_FS_RETRY_NBTIMES));

				if (vfs_result == -1)
				{
					result = eTDAL_FS_ERROR_BAD_ARG;
					if (errno == EIO)
					{
						mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Mount:   !!!!!   vfs_mount   failed   (errno=%d)   for   (%d)   successive   attempts   !!!!!\n", errno,TDAL_FS_RETRY_NBTIMES ));
					}
					else if (errno == EAGAIN)
					{
						mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Mount:   vfs_mount   failed   (errno=EAGAIN)\n"));
						result = eTDAL_FS_ERROR_CORRUPTED;
					}
				}
			}
#endif
			else
			{
				result = eTDAL_FS_ERROR_BAD_ARG;
			}
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_1,"TDAL_FS_Mount:   vfs_mount   succeed\n"));

			TDAL_LockMutex(TDAL_FS_VFS_CachedInfoMutex);

		    MS_U32 i = 0;

		    for(i=0 ; i< kTDAL_FS_VFS_MAX_NUM_PARTITIONS; i++)
		    {
		        if (TDAL_FS_VFS_CachedInfo[i].pMountPoint[0] == '\0')
		        {
                    strncpy(TDAL_FS_VFS_CachedInfo[i].pMountPoint, pMountPoint, kTDAL_FS_VFS_MAX_MOUNT_NAME_SIZE);

                    pTDAL_FS_GetFsInfo(TDAL_FS_VFS_CachedInfo[i].pMountPoint,
                                       &TDAL_FS_VFS_CachedInfo[i].stInfo);
                    break;
		        }
		    }

            TDAL_UnlockMutex(TDAL_FS_VFS_CachedInfoMutex);
		}
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Mount:   ILVMi_GetVolumeName(   vh=0x%x   )   failed\n", VolumeHandle));
		result = eTDAL_FS_ERROR_BAD_ARG;
	}

	mTBOX_RETURN(result);
}

/******************************************************************************
 * Function Name : TDAL_FS_Umount
 *
 * Description   : Unmount a volume
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_Umount(   const   char   *      pMountPoint   )
{
	tTDAL_FS_Error result = eTDAL_FS_NO_ERROR;
	mTBOX_FCT_ENTER("TDAL_FS_Umount");

	mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_Umount(   pMountPoint=\"%s\"   )\n" ,
					pMountPoint ));

	if (/*MsFS_Umount(pMountPoint) */umount(pMountPoint)!= 0)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Umount:   vfs_unmount   failed   (errno   :   %d)   -   %s\n",errno, pMountPoint));
		if (errno == EBUSY)
		{
			result = eTDAL_FS_ERROR_FILE_USED;
		}
		else
		{
			result = eTDAL_FS_ERROR_NOT_DONE;
		}
	}
	else
	{
        TDAL_LockMutex(TDAL_FS_VFS_CachedInfoMutex);

        MS_U32 i = 0;

        for(i=0 ; i< kTDAL_FS_VFS_MAX_NUM_PARTITIONS; i++)
        {
            if (strncmp(TDAL_FS_VFS_CachedInfo[i].pMountPoint, pMountPoint, kTDAL_FS_VFS_MAX_MOUNT_NAME_SIZE) == 0)
            {
                memset(&TDAL_FS_VFS_CachedInfo[i], 0, sizeof(tTDAL_FS_ExtFsInfo));
                break;
            }
        }

        TDAL_UnlockMutex(TDAL_FS_VFS_CachedInfoMutex);
	}

	mTBOX_RETURN(result);
}

/******************************************************************************
 * Function Name : p_TDAL_FS_FakeCheckForFAT32
 *
 * Description   : Reset "dirty" flags of FAT.
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
#if   kTDAL_FS_FAKE_CHECK_FOR_FAT32
LOCAL   tTDAL_FS_Error      p_TDAL_FS_FakeCheckForFAT32(   char   *   VolumeName   )
{
   device_handle_t   *      hndl;
   uint8_t   *        buffer;
   int32_t            block_read;
   int32_t            block_written;
   lba_t            lba;

   /*   dirty   shutdown   flag   modification   */
   mTBOX_TRACE((kTBOX_NIV_1,"dirty   flag   modification\n"));
   if   ((hndl = device_open(VolumeName   ,OSPLUS_O_RDWR)) == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Check   :   could   not   open   volume\n"));
      return(eTDAL_FS_ERROR_BAD_ARG);
   }
   buffer = (unsigned   char   *)osplus_cache_safe_allocate(   NULL   , 512   );
   if   (   buffer == NULL   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Check   :   Buffer   malloc   failed\n"));
      return(eTDAL_FS_ERROR_BAD_ARG);
   }

   /*   FAT1   */
   lba = 32;   /*FAT1   sector*/

   block_read = device_block_read(hndl, buffer,lba, 1);
   if   (block_read == -1)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Check   :   device_block_read   failed\n"));
      return(eTDAL_FS_ERROR_BAD_ARG);
   }
   mTBOX_TRACE((kTBOX_NIV_1,"FAT1   :   read   1   :   buffer   7:   %08x\n", buffer[0x7]));
   buffer[0x7] = 0xFF;   /*   dirty   shutdown   flag   */
   block_written = device_block_write(hndl, buffer,lba, 1);
   if   (block_written == -1)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Check   :   device_block_write   failed\n"));
      return(eTDAL_FS_ERROR_BAD_ARG);
   }


   /*   FAT2   */
   lba = 0;   /*boot   sector*/

   /*   read   the   boot   sector   to   know   the   FAT   size   */
   block_read = device_block_read(hndl, buffer   ,lba   , 1);
   if   (block_read == -1)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Check   :   device_block_read   failed\n"));
      return(eTDAL_FS_ERROR_BAD_ARG);
   }
   mTBOX_TRACE((kTBOX_NIV_1,"FAT2   :   boot   :   buffer   24:   %08x\n", buffer[0x24]));
   mTBOX_TRACE((kTBOX_NIV_1,"FAT2   :   boot   :   buffer   25:   %08x\n", buffer[0x25]));

   lba = (buffer[0x25]*256   +   buffer[0x24])   +32;   /*FAT2   sector = FAT   size   +   FAT1   sector*/

   mTBOX_TRACE((kTBOX_NIV_1,"FAT2   :   lba:   %d\n", lba));

   block_read = device_block_read(hndl, buffer   ,lba   , 1);
   if   (block_read == -1)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Check   :   device_block_read   failed\n"));
      return(eTDAL_FS_ERROR_BAD_ARG);
   }
   mTBOX_TRACE((kTBOX_NIV_1,"FAT2   :   read   1   :   buffer   7:   %08x\n", buffer[0x7]));
   buffer[0x7] = 0xFF;
   block_written = device_block_write(hndl, buffer   ,lba   , 1);
   if   (block_written == -1)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Check   :   device_block_write   failed\n"));
      return(eTDAL_FS_ERROR_BAD_ARG);
   }

   osplus_cache_safe_deallocate(   NULL   , buffer   );
   if   (device_close(hndl) == -1)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Check   :   could   not   close   volume\n"));
      return(eTDAL_FS_ERROR_BAD_ARG);
   }

   return   eTDAL_FS_NO_ERROR;
}
#endif




/******************************************************************************
 * Function Name : TDAL_FS_Check
 *
 * Description   : check a volume
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_Check(   tTDAL_FS_VolumeHandle      VolumeHandle   )
{
   char            volume_name[kTDAL_FS_VOLUME_NAME_LEN_MAX];
   tTDAL_FS_Error      result = eTDAL_FS_NO_ERROR;
   bool            b_result;
   tTDAL_FS_VolumeType      volume_type;
   //clock_t        time_start;
   //clock_t        time_stop;

   mTBOX_FCT_ENTER("TDAL_FS_Check");
   mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_Check(   VolumeHandle=0x%x   )\n",VolumeHandle));

   //time_start = time_now();

   b_result = TDAL_FSi_GetVolumeName(   VolumeHandle   , volume_name   );
   if   (b_result == TRUE)
   {
      volume_type = TDAL_FSi_GetFormatType(VolumeHandle);
      #if   kTDAL_FS_FAKE_CHECK_FOR_FAT32
      if   (   (   volume_type == eTDAL_FS_VOLUME_TYPE_FAT32   )   ||
        (   volume_type == eTDAL_FS_VOLUME_TYPE_FAT32_LBA)   )
      {
      result = p_TDAL_FS_FakeCheckForFAT32(   volume_name   );
      mTBOX_TRACE((kTBOX_NIV_5, "TDAL_FS_Check:   p_TDAL_FS_FakeCheckForFAT32   returned   %d\n", result));
      mTBOX_PRINT((kTBOX_LF, "TDAL_FS_Check:   p_TDAL_FS_FakeCheckForFAT32   returned   %d\n", result));
      }
      else
      #endif
      /*if   (   vfs_fsck(volume_name, TDAL_FS_VFS_FsName[volume_type]) != 0   )
      {
      result = eTDAL_FS_ERROR_FILE_USED;
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Check:   vfs_fsck   failed   (errno=%d)\n", errno));
      }*/
   }
   else
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Check:   ILVMi_GetVolumeName(   vh=0x%x   )   failed\n", VolumeHandle));
      result = eTDAL_FS_ERROR_BAD_ARG;
   }

   //time_stop = time_now();

   //mTBOX_PRINT((kTBOX_LF,"TDAL_FS_Check:   nb   of   ticks   during   check   :   %d\n", time_minus(   time_stop, time_start)   ));

   mTBOX_RETURN(result);
}




/******************************************************************************
 * Function Name : TDAL_FS_Format
 *
 * Description   : Format a volume
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_Format(   tTDAL_FS_VolumeHandle      VolumeHandle   ,
                 tTDAL_FS_FormatParam   *   pFormatParam   )
{
   char                volume_name[kTDAL_FS_VOLUME_NAME_LEN_MAX];
   char                volume_label[kTDAL_FS_VOLUME_NAME_LEN_MAX];
   tTDAL_FS_Error      result = eTDAL_FS_NO_ERROR;
   bool                b_result;
   tTDAL_FS_VolumeType volume_type;
   void   *            p_format_param = NULL;
   uint32_t            err;

   mTBOX_FCT_ENTER("TDAL_FS_Format");

   memset(volume_label, 0, kTDAL_FS_VOLUME_NAME_LEN_MAX);

   b_result = TDAL_FSi_GetVolumeName(   VolumeHandle   , volume_name   );
   if   (b_result == FALSE)
   {
       result = eTDAL_FS_ERROR_BAD_ARG;
       mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Format:   TDAL_FSi_GetVolumeName(   vh=0x%x   )   failed\n", VolumeHandle));
   }

   if (result == eTDAL_FS_NO_ERROR)
   {
      volume_type = TDAL_FSi_GetFormatType(VolumeHandle);
      if   (   volume_type == eTDAL_FS_VOLUME_TYPE_UNKNOWN   )
      {
          result = eTDAL_FS_ERROR_BAD_ARG;
          mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Format:   TDAL_FSi_GetFormatType(   vh=0x%x   )   returns   UNKNOWN   type\n", VolumeHandle));
      }

   }

   if (result == eTDAL_FS_NO_ERROR)
   {
      err = format(volume_name, volume_label, TDAL_FS_VFS_FsName[volume_type]);
      if (err != 0)
      {
          result = eTDAL_FS_ERROR_NOT_DONE;
          mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Format:   format (   vh=0x%x   )   returns   UNKNOWN   type\n", VolumeHandle));

      }

   }

   if (result == eTDAL_FS_NO_ERROR)
   {
       mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_Format() Successfully finished!\n"));
   }

   mTBOX_RETURN(result);
}

/******************************************************************************
 * Function Name : TDAL_FS_Open
 *
 * Description   : Close the File
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_File      TDAL_FS_Open(   const   char   *   pFilename   ,
               uint8_t      Mode   )
{
	int fileDes;
	tTDAL_FS_File result = kTDAL_FS_INVALID_HANDLE;

	mTBOX_FCT_ENTER("TDAL_FS_Open");

	mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_Open(   %s   , %d   )\n", pFilename, Mode ));

	fileDes = open(pFilename, pTDAL_FS_getFileMode(Mode));
	if (fileDes < 0)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Open:   fopen(   %s   )   failed   (errno=%d)\n",pFilename,errno));

#if   (TDAL_FS_RETRY_NBTIMES   !=0   )
		if (errno == EIO)
		{
			uint8_t nbtimes = 0;
			do
			{
			    fileDes = open(pFilename, pTDAL_FS_getFileMode(Mode));
				//File = MsFS_Fopen(pFilename, pTDAL_FS_getFileMode(Mode));
				nbtimes++;
			} while ((fileDes < 0) && (errno == EIO)
					&& (nbtimes < TDAL_FS_RETRY_NBTIMES));

			if ((fileDes < 0) && (errno == EIO))
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Open:   !!!!!   fopen   failed   (errno=%d)   for   (%d)   successive   attempts   !!!!!\n", errno,TDAL_FS_RETRY_NBTIMES ));
			}
		}
#endif

	}

	if (fileDes >= 0)
	{
		pTDAL_FS_IncreaseOpsCounter();
		result = (tTDAL_FS_File) fileDes;
	}
        memset(tdal_mp_filename,0,sizeof(tdal_mp_filename));
        if (strlen(tdal_mp_filename)<= 1024)
        {
            strcpy(tdal_mp_filename,pFilename);
        }
	mTBOX_RETURN(result);
}

/******************************************************************************
 * Function Name : TDAL_FS_Close
 *
 * Description   : Close the File
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_Close(   tTDAL_FS_File   File   )
{
    tTDAL_FS_Error result = eTDAL_FS_NO_ERROR;
    int vfs_result;

    mTBOX_FCT_ENTER("TDAL_FS_Close");

    vfs_result = close((int) File); //MsFS_Fclose((FILE   *)   File);
    if (vfs_result != 0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Close:   fclose   failed   (errno=%d)\n",errno));

#if   (TDAL_FS_RETRY_NBTIMES   !=0   )
        if (errno == EIO)
        {
            uint8_t nbtimes = 0;
            do
            {
#if   (TDAL_FS_RETRY_DELAY   !=0   )
                task_delay((ST_GetClocksPerSecond()*TDAL_FS_RETRY_DELAY)/1000);
#endif
                vfs_result = close((int) File); //MsFS_Fclose((FILE   *)   File);
                nbtimes++;
            } while ((vfs_result != 0) && ( errno==EIO ) && ( nbtimes<TDAL_FS_RETRY_NBTIMES ) );

            if ((vfs_result != 0) && (errno == EIO))
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Close:   !!!!!   fclose   failed   (errno=%d)   for   (%d)   successive   attempts   !!!!!\n", errno,TDAL_FS_RETRY_NBTIMES ));
            }

            if (vfs_result != 0)
            {
                result = eTDAL_FS_ERROR_FILE_USED;
            }
        }
#else
        result = eTDAL_FS_ERROR_FILE_USED;
#endif
    }

    if (result == eTDAL_FS_NO_ERROR) {
        pTDAL_FS_DecreaseOpsCounter();
    }

    mTBOX_RETURN(result);
}



/******************************************************************************
 * Function Name : p_TDAL_FS_Read
 *
 * Description   : Read a File
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Size      p_TDAL_FS_Read(   tTDAL_FS_File   File   ,
               void   *      pBuffer   ,
               tTDAL_FS_Size   Size   )
{
	tTDAL_FS_Size byte_read;

	if (pBuffer == NULL)
	{
		return (-1);
	}

	if (Size == 0)
	{
		return (0);
	}

	byte_read = read((int) File, pBuffer, Size); //fread(pBuffer, sizeof(char), Size, (FILE *) File);
	if (byte_read <= 0)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Read:   fread   failed   (errno=%d)\n", errno ));

#if   (TDAL_FS_RETRY_NBTIMES   !=0   )
		if (errno == EIO)
		{
			uint8_t nbtimes = 0;
			do
			{
#if   (TDAL_FS_RETRY_DELAY   !=0   )
				task_delay((ST_GetClocksPerSecond()*TDAL_FS_RETRY_DELAY)/1000);
#endif
				byte_read = read((int) File, pBuffer, Size);
				nbtimes++;
			} while ((byte_read <= 0) && (errno == EIO)
					&& (nbtimes < TDAL_FS_RETRY_NBTIMES));

			if ((byte_read <= 0) && (errno == EIO))
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Read:   !!!!!   fread   failed   (errno=%d)   for   (%d)   successive   attempts   !!!!!\n", errno,TDAL_FS_RETRY_NBTIMES ));
			}
		}
#endif

	}

	return (byte_read);
}



/******************************************************************************
 * Function Name : TDAL_FS_Read
 *
 * Description   : Read a File
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Size      TDAL_FS_Read(   tTDAL_FS_File   File   ,
               void   *      pBuffer   ,
               tTDAL_FS_Size   Size   )
{
   tTDAL_FS_Size   TotalBytesRead;
#if   (TDAL_FS_READ_SLICE_SIZE != 0)
   tTDAL_FS_Size   byte_read   ,SizeToRead;
#endif

   mTBOX_FCT_ENTER("TDAL_FS_Read");

   if   (pBuffer == NULL)
   {
      mTBOX_RETURN(-1);
   }

   if   (Size == 0)
   {
      mTBOX_RETURN(0);
   }

   mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_FS_Read] file pointer = %p, size = %d\n", File, Size));

#if   (TDAL_FS_READ_SLICE_SIZE == 0)
   TotalBytesRead = p_TDAL_FS_Read(File, pBuffer, Size);
#else

    TotalBytesRead = 0;
    while(TotalBytesRead < Size)
    {
        SizeToRead = ( (Size - TotalBytesRead)>TDAL_FS_READ_SLICE_SIZE ? TDAL_FS_READ_SLICE_SIZE : (Size - TotalBytesRead) );

        byte_read = p_TDAL_FS_Read(File, &( ((char *)pBuffer)[TotalBytesRead]), SizeToRead);
        if ( byte_read > 0 )
        {
            TotalBytesRead += byte_read;
        }
        else
        {
            break;
        }
    }
#endif

    mTBOX_RETURN(TotalBytesRead);
}


/******************************************************************************
 * Function Name : p_TDAL_FS_Write
 *
 * Description   : Write in a File
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Size      p_TDAL_FS_Write(   tTDAL_FS_File   File   ,
               void   *      p_buffer   ,
               tTDAL_FS_Size   Size   )
{
	tTDAL_FS_Size byte_written;

	if (p_buffer == NULL)
	{
		return (-1);
	}

	if (Size == 0)
	{
		return (0);
	}

	byte_written = write((int) File, p_buffer, (size_t) Size);
	if (byte_written == -1)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Write:   fwrite   failed   (errno=%d)\n", errno ));
		if (errno == ENOSPC)
		{
			byte_written = 0;
		}
#if   (TDAL_FS_RETRY_NBTIMES   !=0   )
		if (errno == EIO)
		{
			uint8_t nbtimes = 0;
			do
			{
#if   (TDAL_FS_RETRY_DELAY   !=0   )
				task_delay((ST_GetClocksPerSecond()*TDAL_FS_RETRY_DELAY)/1000);
#endif
				byte_written = write((int) File, p_buffer, (size_t) Size);
				;
				nbtimes++;
			} while ((byte_written <= 0) && (errno == EIO)
					&& (nbtimes < TDAL_FS_RETRY_NBTIMES));

			if ((byte_written <= 0) && (errno == EIO))
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Write:   !!!!!   fwrite   failed   (errno=%d)   for   (%d)   successive   attempts   !!!!!\n", errno,TDAL_FS_RETRY_NBTIMES ));
			}
		}
#endif

	}

	return (byte_written);
}

/******************************************************************************
 * Function Name : TDAL_FS_Write
 *
 * Description   : Write in a File
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Size      TDAL_FS_Write(   tTDAL_FS_File   File   ,
               void   *      p_buffer   ,
               tTDAL_FS_Size   Size   )
{
	tTDAL_FS_Size TotalBytesWritten;
#if   (TDAL_FS_WRITE_SLICE_SIZE != 0)
	tTDAL_FS_Size byte_written ,SizeToWrite;
#endif

	mTBOX_FCT_ENTER("TDAL_FS_Write");

	if (p_buffer == NULL)
	{
		mTBOX_RETURN(-1);
	}

	if (Size == 0)
	{
		mTBOX_RETURN(0);
	}

#if   (TDAL_FS_WRITE_SLICE_SIZE == 0)
	TotalBytesWritten = p_TDAL_FS_Write(File, p_buffer, Size);
#else

	TotalBytesWritten = 0;
	while(TotalBytesWritten < Size)
	{
		SizeToWrite = ( (Size - TotalBytesWritten)>TDAL_FS_WRITE_SLICE_SIZE ? TDAL_FS_WRITE_SLICE_SIZE : (Size - TotalBytesWritten) );

		byte_written = p_TDAL_FS_Write(File, &( ((char *)p_buffer)[TotalBytesWritten]), SizeToWrite);
		if ( byte_written > 0 )
		{
			TotalBytesWritten += byte_written;
		}
		else
		{
			break;
		}
	}
#endif

	mTBOX_RETURN(TotalBytesWritten);
}



/******************************************************************************
 * Function Name : TDAL_FS_Flush
 *
 * Description   : Flush a File
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_Flush(   tTDAL_FS_File   File   )
{
   tTDAL_FS_Error   result = eTDAL_FS_NO_ERROR;

   mTBOX_FCT_ENTER("TDAL_FS_Flush");
   mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_Flush()\n"));

   if   (fsync((int) File) == EOF)
   {
      result = eTDAL_FS_ERROR_FILE_NOT_FOUND;
   }

   mTBOX_RETURN(result);
}

/******************************************************************************
 * Function Name : TDAL_FS_Seek
 *
 * Description   : Seek in a File
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_Seek(   tTDAL_FS_File      File   ,
                   tTDAL_FS_Size      Size   ,
                   tTDAL_FS_Seek_Origin   Origin   )
{
   tTDAL_FS_Error   result = eTDAL_FS_NO_ERROR;
   mTBOX_FCT_ENTER("TDAL_FS_Seek");
   mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_Seek()-   size   :   %d   -   Origin   :   %d\n",Size,Origin));
   int whence = SEEK_SET;

   if   (   (Size < 0) && (Origin == eTDAL_FS_SEEK_ORIGIN_BEGIN)   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Seek:   cannot   perform   a   negative   jump   from   origin\n"));
      result = eTDAL_FS_ERROR_BAD_ARG;
      mTBOX_RETURN(result);
   }

   if ((Size > 0) && (Origin == eTDAL_FS_SEEK_ORIGIN_END))
   {
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Seek:   cannot   perform   a   positive   jump   from   end\n"));
		result = eTDAL_FS_ERROR_BAD_ARG;
		mTBOX_RETURN(result);
   }

   if   (Origin > eTDAL_FS_SEEK_ORIGIN_END)
   {
      result = eTDAL_FS_ERROR_BAD_ARG;
      mTBOX_RETURN(result);
   }

   switch (Origin)
   {
   case eTDAL_FS_SEEK_ORIGIN_BEGIN:
       whence = SEEK_SET;
       break;
   case eTDAL_FS_SEEK_ORIGIN_CURRENT:
       whence = SEEK_CUR;
       break;
   case eTDAL_FS_SEEK_ORIGIN_END:
       whence = SEEK_END;
       break;
   default:
       mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_FS_Seek] Unsupported whence param"));
       break;
   }

    if (lseek((int) File, (off_t) Size, whence) == -1)
    {
        result = eTDAL_FS_ERROR_NOT_DONE; /*FILE_NOT_FOUND;*/
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Seek:   lseek(   fileDes = %d, size=%d, whence = %d  )   failed, errno=%d\n",File, Size, whence, errno));
    }

   mTBOX_RETURN(result);

}

/******************************************************************************
 * Function Name : TDAL_FS_Tell
 *
 * Description   : Seek in a File
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_Tell(   tTDAL_FS_File   File   ,
               tTDAL_FS_Size   *   pSize   )
{
   tTDAL_FS_Error   result = eTDAL_FS_NO_ERROR;

   mTBOX_FCT_ENTER("TDAL_FS_Tell");

   mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_Tell()\n"));

   if   (pSize == NULL)
   {
      result = eTDAL_FS_ERROR_BAD_ARG;
      mTBOX_RETURN(result);
   }

   *pSize = lseek((int) File, (off_t) 0, SEEK_CUR);
   if   (*pSize == -1)
   {
      result = eTDAL_FS_ERROR_FILE_NOT_FOUND;
      mTBOX_RETURN(result);
   }

   mTBOX_TRACE((kTBOX_NIV_1, "TDAL_FS_Tell returns %d\n", *pSize));

   mTBOX_RETURN(result);
}


/******************************************************************************
 * Function Name : TDAL_FS_Stat
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_Stat(   tTDAL_FS_File   File   ,
                   tTDAL_FS_Stat   *   pStat   )
{
   tTDAL_FS_Error   result = eTDAL_FS_NO_ERROR;
   struct stat     fStat;

   mTBOX_FCT_ENTER("TDAL_FS_Stat");
   mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_Stat()\n"));

   if   (pStat == NULL)
   {
      result = eTDAL_FS_ERROR_BAD_ARG;
      mTBOX_RETURN(result);
   }

   if (fstat ( (int) File, &fStat) != 0)
   {
      result = eTDAL_FS_ERROR_FILE_NOT_FOUND;
   }

	if (result == eTDAL_FS_NO_ERROR)
	{
		pStat->Mode = (uint8_t) fStat.st_mode;
		pStat->Size = (uint32_t) fStat.st_size;
		pStat->uiBlockSize = (uint32_t) 0;
		pStat->liATime = (uint32_t) fStat.st_atime;
		pStat->liMTime = (uint32_t) fStat.st_mtime;
		pStat->liCTime = (uint32_t) fStat.st_ctime;
	}

	mTBOX_RETURN(result);
}


/******************************************************************************
 * Function Name : TDAL_FS_Remove
 *
 * Description   : Remove a File
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_Remove(   const   char   *   pFilename   )
{
   tTDAL_FS_Error   result = eTDAL_FS_NO_ERROR;
   struct stat fStat;

   mTBOX_FCT_ENTER("TDAL_FS_Remove");

   mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_Remove(   pFilename=\"%s\"   )\n",pFilename));

   pTDAL_FS_IncreaseOpsCounter();

   if (pFilename == NULL)
   {
       result = eTDAL_FS_ERROR_BAD_ARG;
   }

   if (stat(pFilename, &fStat) == 0)
   {
       if (!S_ISREG(fStat.st_mode))
       {
           mTBOX_TRACE((kTBOX_NIV_1, "TDAL_FS_FileExist, filename = %s not file, but directory\n", pFilename));
           result = eTDAL_FS_ERROR_FILE_NOT_FOUND;
       }
   }

   if   ( unlink(pFilename)==-1   )
   {
      if   (errno == EBUSY)
      {
          result = eTDAL_FS_ERROR_FILE_USED;
      }
      else   if   (errno == ENOENT)
      {
          result = eTDAL_FS_ERROR_FILE_NOT_FOUND;
      }
      else
      {
          result = eTDAL_FS_ERROR_NOT_DONE;
          mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Remove:   vfs_unlink(   %s   )   failed   (errno=%d)\n", pFilename, errno   ));
      }
   }
   else
   {
       TDAL_LockMutex(TDAL_FS_VFS_CachedInfoMutex);

       MS_U32 i = 0;

       for(i=0 ; i< kTDAL_FS_VFS_MAX_NUM_PARTITIONS; i++)
       {
           if (strstr (pFilename, TDAL_FS_VFS_CachedInfo[i].pMountPoint) != NULL)
           {
               TDAL_FS_VFS_CachedInfo[i].stInfo.freeBlocks += (uint32_t) (fStat.st_size >> 9);
               break;
           }
       }

       TDAL_UnlockMutex(TDAL_FS_VFS_CachedInfoMutex);
   }

   pTDAL_FS_DecreaseOpsCounter();

   mTBOX_RETURN(result);
}

/******************************************************************************
 * Function Name : TDAL_FS_Rename
 *
 * Description   : Rename a File
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_Rename(   const   char   *   pOldFilename   ,
                 const   char   *   pNewFilename   )
{
   tTDAL_FS_Error   result = eTDAL_FS_NO_ERROR;

   mTBOX_FCT_ENTER("TDAL_FS_Rename");
   mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_Rename(   pOldFilename=\"%s\"   , pNewFilename=\"%s\"   )\n",pOldFilename,pNewFilename));

   pTDAL_FS_IncreaseOpsCounter();

   TDAL_LockMutex(TDAL_FS_DirMutex);

   if   (   rename(pOldFilename, pNewFilename) == -1   )
   {
      if   (errno == EBUSY)
      {
          result = eTDAL_FS_ERROR_FILE_USED;
      }
      else   if   (errno == ENOENT)
      {
          result = eTDAL_FS_ERROR_FILE_NOT_FOUND;
      }
      else
      {
          result = eTDAL_FS_ERROR_NOT_DONE;
          mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Rename:   vfs_rename(%s,%s)   failed   (errno=%d)\n", pOldFilename,pNewFilename,errno   ));
      }
   }

   TDAL_UnlockMutex(TDAL_FS_DirMutex);

   pTDAL_FS_DecreaseOpsCounter();

   mTBOX_RETURN(result);
}

/******************************************************************************
 * Function Name : TDAL_FS_FileExist
 *
 * Description   : test if the file exists
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_FileExist(   const   char   *   pFilename   )
{
   tTDAL_FS_Error      result = eTDAL_FS_NO_ERROR;
   FILE   *   File;
   struct stat fStat;

   mTBOX_FCT_ENTER("TDAL_FS_FileExist");

   if (pFilename == NULL)
   {
	   mTBOX_RETURN(eTDAL_FS_ERROR_BAD_ARG);
   }

   mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_FileExist(   pFilename=\"%s\"   )\n",pFilename));

   if (stat(pFilename, &fStat) == 0)
   {
	   if (!S_ISREG(fStat.st_mode))
	   {
		   mTBOX_TRACE((kTBOX_NIV_1, "TDAL_FS_FileExist, filename = %s not file, but directory\n", pFilename));
		   result = eTDAL_FS_ERROR_FILE_NOT_FOUND;
	   }
   }
   else
   {
	   mTBOX_TRACE((kTBOX_NIV_1, "TDAL_FS_FileExist, filename = %s not found\n", pFilename));
	   result = eTDAL_FS_ERROR_FILE_NOT_FOUND;
   }

   mTBOX_RETURN(result);
//
//   File = MsFS_Fopen(pFilename,"r");
//   if   (   File == NULL   )
//   {
//      result = eTDAL_FS_ERROR_FILE_NOT_FOUND;
//      mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_FileExist:   MsFS_Fopen   failed   =>   file   doesn't   exist\n"));
//   }
//   else
//   {
//      if   (   MsFS_Fclose(File) == EOF   )
//      {
//      result = eTDAL_FS_ERROR_FILE_USED;
//      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_FileExist:   vfs_close   failed\n"));
//      }
//   }

}

/******************************************************************************
 * Function Name : TDAL_FS_MkDir
 *
 * Description   : Create a dir
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_MkDir(      const   char   *      pDirName   )
{
	tTDAL_FS_Error result = eTDAL_FS_NO_ERROR;
	mTBOX_FCT_ENTER("TDAL_FS_MkDir"); mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_MkDir(   pDirName=\"%s\"   )\n",pDirName));

	pTDAL_FS_IncreaseOpsCounter();

	TDAL_LockMutex(TDAL_FS_DirMutex);

	if (mkdir(pDirName, 0777) == -1)
	{
		if (errno == EEXIST)
		{
			result = eTDAL_FS_ERROR_DIR_EXISTS;
		}
		else
		{
			result = eTDAL_FS_ERROR_BAD_ARG;
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_MkDir:   mkdir(   %s   )   failed   (errno=%d)\n",pDirName,errno));
		}
	}

	TDAL_UnlockMutex(TDAL_FS_DirMutex);

	pTDAL_FS_DecreaseOpsCounter();

	mTBOX_RETURN(result);
}

/******************************************************************************
 * Function Name : TDAL_FS_OpenDir
 *
 * Description   : Open a dir
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Dir      TDAL_FS_OpenDir(   const   char   *      pDirName   )
{
	DIR * dir;
	tTDAL_FS_Dir result = kTDAL_FS_INVALID_HANDLE;
	uint16_t index = 0;

	mTBOX_FCT_ENTER("TDAL_FS_OpenDir");


	mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_OpenDir(   pDirName=\"%s\"   )\n",pDirName));

	TDAL_LockMutex(TDAL_FS_DirMutex);

	/*   find   a   free   item   in   TDAL_FS_DirTable   */
	for (index = 0; index < kTDAL_FS_MAX_DIR_OPENED; index++)
	{
		if (TDAL_FS_DirTable[index] == NULL)
		{
			break;
		}
	}
	if (index >= kTDAL_FS_MAX_DIR_OPENED)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_OpenDir:   vfs_opendir(\"%s\")   failed   -   too   much   dir   opened\n", pDirName));
	}
	else
	{
		/*   open   the   directory   */
		dir = opendir(pDirName);
		if (dir == NULL)
		{
			switch (errno)
			{
			case ENOENT: /*   warning   traces   */
				mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_FS_OpenDir:   vfs_opendir(\"%s\")   failed   -   errno=%d\n", pDirName, errno));
				break;
			default: /*   critical   traces   */
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_OpenDir:   vfs_opendir(\"%s\")   failed   -   errno=%d\n", pDirName, errno));
				break;
			}
		}
		else
		{
			result = (tTDAL_FS_Dir) index;
			TDAL_FS_DirTable[index] = dir;

			if (TDAL_FS_DirNameTable[index] != NULL)
			{
				TDAL_Free(TDAL_FS_DirNameTable[index]);
				TDAL_FS_DirNameTable[index] = NULL;
			}
			TDAL_FS_DirNameTable[index] = TDAL_Malloc(strlen(pDirName) + 1);
			if (TDAL_FS_DirNameTable[index] != NULL)
			{
				strcpy(TDAL_FS_DirNameTable[index], pDirName);
			}

			pTDAL_FS_IncreaseOpsCounter();
		}
	}

	TDAL_UnlockMutex(TDAL_FS_DirMutex);

	mTBOX_RETURN(result);
}

/******************************************************************************
 * Function Name : RewindDir
 *
 * Description   : Rewind a dir
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_RewindDir(   tTDAL_FS_Dir      Dir   )
{
	tTDAL_FS_Error result = eTDAL_FS_NO_ERROR;
	DIR * dir;

	mTBOX_FCT_ENTER("TDAL_FS_RewindDir");

	if (Dir == kTDAL_FS_INVALID_HANDLE)
	{
	    mTBOX_RETURN(eTDAL_FS_ERROR_BAD_ARG);
	}

	TDAL_LockMutex(TDAL_FS_DirMutex);

	dir = TDAL_FS_DirTable[Dir];
	rewinddir(dir);
	int errsv = errno;
	if (errsv != 0)
	{
		result = eTDAL_FS_ERROR_DIR_NOT_FOUND;
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_RewindDir:   vfs_rewinddir()   failed\n"));
	}

	TDAL_UnlockMutex(TDAL_FS_DirMutex);

	mTBOX_RETURN(result);
}

void TDAL_FSi_ConvertToAsciiAndCopy(uint8_t * destPtr, uint32_t destSize, uint8_t * sourcePtr, uint32_t sourceSize)
{
	int i;
	int j = 0;

	for (i = 0; i < sourceSize - 1; i++)
	{
		if (j + 1 >= destSize)
		{
			break;
		}
		if (sourcePtr[i] == 0 && sourcePtr[i + 1] == 0)
		{
			break;
		}
		else if (sourcePtr[i] == 0)
		{

		}
		else if (sourcePtr[i] >= 32 && sourcePtr[i] < 127)
		{
			destPtr[j] = sourcePtr[i];
			j++;
		}
		else
		{
			destPtr[j] = '?';
			j++;
		}
	}

	destPtr[j] = 0;


}

/******************************************************************************
 * Function Name : ReadDir
 *
 * Description   : Read a dir
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_ReadDir(   tTDAL_FS_Dir      Dir   ,
                 tTDAL_FS_DirEntry   *   pDirEntry   )
{
	tTDAL_FS_Error result = eTDAL_FS_NO_ERROR;
	struct dirent entry;
	struct dirent * dirresult;
	int r;
	DIR * dir;
	bool no_more_file = FALSE;
	uint32_t alloc_size;

	mTBOX_FCT_ENTER("TDAL_FS_ReadDir");
	mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_ReadDir()\n"));

	if ((Dir == kTDAL_FS_INVALID_HANDLE) || (pDirEntry == NULL))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_ReadDir:   Bad   arguments   :   Dir=0x%x   and   pDirEntry=0x%x\n", Dir, pDirEntry));
		mTBOX_RETURN(eTDAL_FS_ERROR_BAD_ARG);
	}

	memset(&entry, 0, sizeof(entry));

	TDAL_LockMutex(TDAL_FS_DirMutex);

	dir = TDAL_FS_DirTable[Dir];
	if ((r = readdir_r(dir, &entry, &dirresult)) == 0)
	{
		if (dirresult == NULL)
		{
			no_more_file = TRUE;
		}
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "[TDAL_FS_ReadDir] Could not read dir\n"));
		result = eTDAL_FS_ERROR_NOT_DONE;
	}

	if (result == eTDAL_FS_NO_ERROR)
	{
		if (no_more_file == FALSE)
		{
			if (entry.mode == 0)
			{
				mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_ReadDir:   mode   dir\n"));
				pDirEntry->flag = kTDAL_FS_FLAG_DIR;
				pDirEntry->size = 0;
			}
			else
			{
				pDirEntry->flag = kTDAL_FS_FLAG_FILE;
				pDirEntry->size = entry.st_size;

				if (TDAL_FS_DirNameTable != NULL)
				{
				}
				else
				{
					result = eTDAL_FS_ERROR_DIR_NOT_FOUND;
				}
			}

			TDAL_FSi_ConvertToAsciiAndCopy(pDirEntry->name, sizeof(pDirEntry->name), entry.d_name, sizeof(entry.d_name));
			mTBOX_TRACE(( kTBOX_NIV_5,"TDAL_FS_ReadDir:   entry   name=\"%s\"   mode=%d\n",
							pDirEntry->name,
							entry.mode));
		}
		else
		{
			pDirEntry->name[0] = '\0';
			pDirEntry->flag = 0;
			pDirEntry->size = 0;
			/*   Specs:   in   this   case, return   value   should   be   NO_ERROR   *
			 result = eTDAL_FS_ERROR_FILE_NOT_FOUND;*/
		}
	}

	TDAL_UnlockMutex(TDAL_FS_DirMutex);
        mTBOX_RETURN(result);
}

/******************************************************************************
 * Function Name : CloseDir
 *
 * Description   : Close a dir
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_CloseDir(   tTDAL_FS_Dir      Dir   )
{
	tTDAL_FS_Error result = eTDAL_FS_NO_ERROR;
	DIR * vfs_dir;

	mTBOX_FCT_ENTER("TDAL_FS_CloseDir"); mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_CloseDir()\n"));

	if (Dir == kTDAL_FS_INVALID_HANDLE)
	{
	    mTBOX_RETURN(eTDAL_FS_ERROR_BAD_ARG);
	}

	TDAL_LockMutex(TDAL_FS_DirMutex);

	vfs_dir = TDAL_FS_DirTable[Dir];

	if (closedir(vfs_dir) == -1)
	{
		result = eTDAL_FS_ERROR_DIR_NOT_FOUND;
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_CloseDir:   vfs_closedir()   failed\n"));
	}
	TDAL_FS_DirTable[Dir] = NULL;

	if (TDAL_FS_DirNameTable[Dir] != NULL)
	{
		TDAL_Free(TDAL_FS_DirNameTable[Dir]);
		TDAL_FS_DirNameTable[Dir] = NULL;
	}

	TDAL_UnlockMutex(TDAL_FS_DirMutex);

	if (result == eTDAL_FS_NO_ERROR)
	{
		pTDAL_FS_DecreaseOpsCounter();
	}

	mTBOX_RETURN(result);
}

/******************************************************************************
 * Function Name : RmDir
 *
 * Description   : Remove a dir
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_RmDir(      const   char   *      pDirName   )
{
	tTDAL_FS_Error result = eTDAL_FS_NO_ERROR;
	mTBOX_FCT_ENTER("TDAL_FS_RmDir");
	mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_RmDir(   pDirName=\"%s\"   )\n",pDirName));

	pTDAL_FS_IncreaseOpsCounter();

	TDAL_LockMutex(TDAL_FS_DirMutex);

	if (rmdir(pDirName) == -1)
	{
		if (errno == ENOTEMPTY)
		{
			result = eTDAL_FS_ERROR_DIR_NOT_EMPTY;
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_RmDir:   MsFS_RmDir(   %s   )   failed   (errno=%d)\n", pDirName, errno ));
			result = eTDAL_FS_ERROR_DIR_NOT_FOUND;
		}
	}

	TDAL_UnlockMutex(TDAL_FS_DirMutex);

	pTDAL_FS_DecreaseOpsCounter();

	mTBOX_RETURN(result);
}

/******************************************************************************
 * Function Name : TDAL_FS_GetFsInfo
 *
 * Description   : Get info about a Mount Point
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      TDAL_FS_GetFsInfo( const char      *pMountPointName,
                                       tTDAL_FS_FsInfo *pFsInfo)
{
    bool     found = false;
    uint16_t i=0;


    if (pMountPointName == NULL || pFsInfo == NULL)
    {
        return eTDAL_FS_ERROR_BAD_ARG;
    }

    TDAL_LockMutex(TDAL_FS_VFS_CachedInfoMutex);

    for (i = 0; i<kTDAL_FS_VFS_MAX_NUM_PARTITIONS; i++)
    {
        if (strncmp(TDAL_FS_VFS_CachedInfo[i].pMountPoint, pMountPointName, kTDAL_FS_VFS_MAX_MOUNT_NAME_SIZE) == 0)
        {
            found = true;
            break;
        }
    }

    if (found == true)
    {
        pFsInfo->freeBlocks     = TDAL_FS_VFS_CachedInfo[i].stInfo.freeBlocks;
        pFsInfo->allocationUnit = TDAL_FS_VFS_CachedInfo[i].stInfo.allocationUnit;
        pFsInfo->totalSize      = TDAL_FS_VFS_CachedInfo[i].stInfo.totalSize;
    }

    TDAL_UnlockMutex(TDAL_FS_VFS_CachedInfoMutex);

    return (found == TRUE ? eTDAL_FS_NO_ERROR : eTDAL_FS_ERROR_BAD_ARG);
}

/******************************************************************************
 * Function Name : TDAL_FS_GetFsInfo
 *
 * Description   : Get info about a Mount Point
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error TDAL_FS_UpdateFsInfo(const char *pMountPointName)
{
    tTDAL_FS_ExtFsInfo pMsgFsInfo;

    strncpy(pMsgFsInfo.pMountPoint, pMountPointName, kTDAL_FS_VFS_MAX_MOUNT_NAME_SIZE);

#if MSTAR_QUEUE
    MsOS_SendToQueue(TDAL_FS_VFS_UpdateQueue,
                     (uint8_t *) &pMsgFsInfo,
                     sizeof(tTDAL_FS_ExtFsInfo),
                     0);
#else
    TDAL_Enqueue(TDAL_FS_VFS_UpdateQueue, &pMsgFsInfo);
#endif
}

/******************************************************************************
 * Function Name : TDAL_FS_GetFsInfo
 *
 * Description   : Get info about a Mount Point
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
tTDAL_FS_Error      pTDAL_FS_GetFsInfo(      const   char   *      pMountPointName   ,
                   tTDAL_FS_FsInfo   *      pFsInfo   )
{

    tTDAL_FS_Error result = eTDAL_FS_NO_ERROR;
    
    mTBOX_FCT_ENTER("TDAL_FS_GetFsInfo");
    mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_GetFsInfo(   pMountPointName=\"%s\"   )\n", pMountPointName));
    MApi_FsInfo MsFsInfo;
    
    if (pMountPointName == NULL)
    {
        //TBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_GetFsInfo(   pMountPointName=\"%s\"   )\n", pMountPointName));
        return eTDAL_FS_ERROR_BAD_ARG;
    }

    if ( MApi_FS_Info(pMountPointName, FALSE, &MsFsInfo) == FALSE)
    {
        return eTDAL_FS_ERROR_CORRUPTED;
    }

    uint32_t BlocksNoInCluster = MsFsInfo.u32ClusSize>>9;
    
    pFsInfo->allocationUnit = BlocksNoInCluster;
    pFsInfo->freeBlocks     = MsFsInfo.u32ClusFree * BlocksNoInCluster;   
    pFsInfo->totalSize      = MsFsInfo.u32ClusTotal * BlocksNoInCluster;

    /* vfs_info_t      vfs_info;


     if   (   vfs_fscntl(   pMountPointName, VFS_FSCNTL_INFO_GET, (void   *)   &vfs_info) != 0)
     {
     mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_GetFsInfo:   vfs_fscntl   failed   (errno=%d)\n",errno));
     result = eTDAL_FS_ERROR_BAD_ARG;
     }
     else
     {
     pFsInfo->freeBlocks = vfs_info.free_blocks;      // Number   of   free   512   bytes   blocks
     pFsInfo->allocationUnit = vfs_info.allocation_unit;   // Number   of   512   bytes   blocks   in   a   cluster
     pFsInfo->totalSize = vfs_info.total_size;        // total   size   of   the   volume   in   512   bytes   blocks
     } */

    mTBOX_RETURN(result);
}

/******************************************************************************
 * Function Name : pTDAL_FS_getOsplusMode
 *
 * Description   : Change TDAL_FS mode in OSPlus mode
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
int pTDAL_FS_getFileMode(   uint8_t   Mode)
{
   int openFlags = 0;

   if ((Mode & kTDAL_FS_MODE_READ) != 0)    {  openFlags|= O_RDONLY; }
   if ((Mode & kTDAL_FS_MODE_WRITE) != 0)   {  openFlags|= O_WRONLY; }
   if ((Mode & kTDAL_FS_MODE_APPEND) != 0)  {  openFlags|= O_APPEND; }
   if ((Mode & kTDAL_FS_MODE_TRUNC) != 0)   {  openFlags|= O_TRUNC; }
   if ((Mode & kTDAL_FS_MODE_CREATE) != 0)  {  openFlags|= O_CREAT; }
   if ((Mode & kTDAL_FS_MODE_EXCL) != 0)    {  openFlags|= O_EXCL; }

   printf("File open, mode  = %d\n", openFlags);

   return openFlags;
}

LOCAL unsigned long  pTDAL_FS_getMountMode(   uint8_t   Mode)
{
	switch (Mode)
	{
	case kTDAL_FS_MODE_READ:
	case kTDAL_FS_MODE_WRITE:
	case kTDAL_FS_MODE_RDWR:
	case kTDAL_FS_MODE_APPEND:
		return 0xFFFF;
	default:
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_getFileMode:   unhandled   mode = 0x%x\n", Mode));
		return 0;
	}
}

LOCAL   void      pTDAL_FS_IncreaseOpsCounter(   void   )
{
   TDAL_LockMutex(   TDAL_FS_OpsMutex   );
   /*printf("OpsCounter:   %d   ->   %d\n", TDAL_FS_OpsCounter, TDAL_FS_OpsCounter+1);*/
   TDAL_FS_OpsCounter++;
   TDAL_UnlockMutex(   TDAL_FS_OpsMutex   );
}

LOCAL   void      pTDAL_FS_DecreaseOpsCounter(   void   )
{
   int      res;
   TDAL_LockMutex(   TDAL_FS_OpsMutex   );
   /*printf("OpsCounter:   %d   ->   %d\n", TDAL_FS_OpsCounter, TDAL_FS_OpsCounter-1);*/
   TDAL_FS_OpsCounter--;
   if   (   TDAL_FS_OpsCounter == 0   )
   {
      //res = vfs_sync_all();
      /*TDAL_FSd_PrintDirtyFlagForFAT32(   "/volume0"   );*/
      if   (   res == -1   )
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_DecreaseOpsCounter:   vfs_sync_all   failed   (errno=%d)\n",errno));
      }
   }
   TDAL_UnlockMutex(   TDAL_FS_OpsMutex   );
}


#if   0   /*   not   used   */
LOCAL   tTDAL_FS_Error      TDAL_FSd_PrintDirtyFlagForFAT32(   char   *   VolumeName   )
{
   device_handle_t   *      hndl = NULL;
   uint8_t   *        buffer = NULL;
   int32_t            block_read;
   lba_t            lba;
   tTDAL_FS_Error      my_err = eTDAL_FS_NO_ERROR;

   if   (   my_err == eTDAL_FS_NO_ERROR   )
   {
      if   ((hndl = device_open(VolumeName   ,OSPLUS_O_RDWR)) == NULL)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FSd_PrintDirtyFlagForFAT32   :   could   not   open   volume\n"));
      my_err = eTDAL_FS_ERROR_BAD_ARG;
      }
   }

   if   (   my_err == eTDAL_FS_NO_ERROR   )
   {
      buffer = (unsigned   char   *)osplus_cache_safe_allocate(   NULL   , 512   );
      if   (   buffer == NULL   )
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FSd_PrintDirtyFlagForFAT32   :   Buffer   malloc   failed\n"));
      my_err = eTDAL_FS_ERROR_BAD_ARG;
      }
   }

   if   (   my_err == eTDAL_FS_NO_ERROR   )
   {
      /*   FAT1   */
      lba = 32;   /*FAT1   sector*/
      block_read = device_block_read(hndl, buffer,lba, 1);
      if   (block_read == -1)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FSd_PrintDirtyFlagForFAT32   :   device_block_read   failed\n"));
      my_err = eTDAL_FS_ERROR_BAD_ARG;
      }
      else
      {
      mTBOX_PRINT((kTBOX_LF,"TDAL_FSd_PrintDirtyFlagForFAT32:   in   FAT1, buffer[7]=%08x   @   LBA=32\n", buffer[0x7]));
      }
   }

   if   (   my_err == eTDAL_FS_NO_ERROR   )
   {
      /*   FAT2   */
      lba = 0;   /*boot   sector*/
      /*   read   the   boot   sector   to   know   the   FAT   size   */
      block_read = device_block_read(hndl, buffer   ,lba   , 1);
      if   (block_read == -1)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FSd_PrintDirtyFlagForFAT32   :   device_block_read   failed\n"));
      my_err = eTDAL_FS_ERROR_BAD_ARG;
      }
   }

   if   (   my_err == eTDAL_FS_NO_ERROR   )
   {
      lba = (buffer[0x25]*256   +   buffer[0x24])   +32;   /*FAT2   sector = FAT   size   +   FAT1   sector*/
      block_read = device_block_read(hndl, buffer   ,lba   , 1);
      if   (block_read == -1)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FSd_PrintDirtyFlagForFAT32   :   device_block_read   failed\n"));
      my_err = eTDAL_FS_ERROR_BAD_ARG;
      }
      else
      {
      mTBOX_PRINT((kTBOX_LF,"TDAL_FSd_PrintDirtyFlagForFAT32:   in   FAT2, buffer[7]=%08x   @   LBA=%d\n", buffer[0x7], lba));
      }
   }

   if   (   buffer != NULL   )
   {
      osplus_cache_safe_deallocate(   NULL   , buffer   );
   }

   if   (   hndl != NULL   )
   {
      if   (device_close(hndl) == -1)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FSd_PrintDirtyFlagForFAT32   :   could   not   close   volume\n"));
      my_err = eTDAL_FS_ERROR_BAD_ARG;
      }
   }

   if   (   my_err != eTDAL_FS_NO_ERROR   )
   {
      mTBOX_PRINT((kTBOX_LF,"TDAL_FSd_PrintDirtyFlagForFAT32:   something   bad   happens\n"));
   }

   return   my_err;
}
#endif

void pTDAL_FS_VFS_Thread(void *data)
{
    tTDAL_FS_ExtFsInfo *pMsgFsInfo;
    MS_U32           size;
    MS_U32           i=0;
    tTDAL_FS_Error   error;

#if MSTAR_QUEUE
    while (MsOS_RecvFromQueue(TDAL_FS_VFS_UpdateQueue,
                              (MS_U8 *) pMsgFsInfo,
                              sizeof(tTDAL_FS_ExtFsInfo),
                              &size,
                              MSOS_WAIT_FOREVER))
#else
    while (TDAL_Dequeue(TDAL_FS_VFS_UpdateQueue, &size))
#endif
    {
        if (size == sizeof(tTDAL_FS_ExtFsInfo))
        {

            TDAL_LockMutex(TDAL_FS_VFS_CachedInfoMutex);

            for(i=0 ; i< kTDAL_FS_VFS_MAX_NUM_PARTITIONS; i++)
            {
                if (strncmp(TDAL_FS_VFS_CachedInfo[i].pMountPoint, pMsgFsInfo->pMountPoint, kTDAL_FS_VFS_MAX_MOUNT_NAME_SIZE) == 0)
                {
                    error = pTDAL_FS_GetFsInfo(TDAL_FS_VFS_CachedInfo[i].pMountPoint,
                                               &TDAL_FS_VFS_CachedInfo[i].stInfo);

                    break;
                }
            }

            TDAL_UnlockMutex(TDAL_FS_VFS_CachedInfoMutex);
        }
    }
}


tTDAL_FS_Error mTDAL_FS_PVR_NotifyRecordedContent(uint8_t *pMountPoint, uint32_t sizeKB)
{
    uint16_t i=0;

    if (pMountPoint == NULL)
    {
        return eTDAL_FS_ERROR_BAD_ARG;
    }

    TDAL_LockMutex(TDAL_FS_VFS_CachedInfoMutex);

    for(i=0 ; i< kTDAL_FS_VFS_MAX_NUM_PARTITIONS; i++)
    {
        if (strstr(TDAL_FS_VFS_CachedInfo[i].pMountPoint, pMountPoint) != NULL)
        {
            TDAL_FS_VFS_CachedInfo[i].stInfo.freeBlocks -= sizeKB * 2;

            break;
        }
    }

    TDAL_UnlockMutex(TDAL_FS_VFS_CachedInfoMutex);

    return eTDAL_FS_NO_ERROR;
}

tTDAL_FS_Error mTDAL_FS_MP_GetFileName(uint8_t*  pFileName)
{
    if (strlen(tdal_mp_filename) > 0)
    {
        memcpy(pFileName,tdal_mp_filename,sizeof(tdal_mp_filename));
    }
}

