/*****************************************************************************
 *                  COPYRIGHT 2006 IWEDIA TECHNOLOGIES            *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_FS MODULE
 *
 * FILE NAME:   TDAL_FS.C
 *   $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_fs/osplus/tdal_fs_lvm.c $
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

#include <stdio.h>
#include <string.h>      /*   for   strcpy()   */
#include <stdlib.h>
//#include "pltf_hardcfg.h"
#include <tdal_common.h>
#include <tdal_fs.h>
#include "tdal_fs_lvm_p.h"
#include "tdal_fs_p.h"
#include "tdal_mp_module_priv.h"
#include "tdal_common_priv.h"

#include <tbox.h>

#include <errno.h>

#include "MsOS.h"

//#include "drvUSB.h"
#include "drvUSB_eCos.h"
#include <cyg/io/io.h>
#include <cyg/io/disk.h>


/*****************************************************************************
 *  Defines                                              *
 *****************************************************************************/
#define   kTDAL_FS_MSGQUEUE_MAX      20

#define   INVALID_INDEX        0xFF

#define   kTDAL_FS_DEVICE_HANDLE_PREFIX   0x4448   /*   'DH'   in   ASCII   */
#define   kTDAL_FS_VOLUME_HANDLE_PREFIX   0x5648   /*   'VH'   in   ASCII   */

#define   VALID_INFO            1
#define   INVALID_INFO        0

#define   kTDAL_FS_LVM_POLLING_FREQ      1

/*****************************************************************************
 *  Macros                                               *
 *****************************************************************************/
#define   mTDAL_FS_MakeHandle(   prefix   , index   )   (   (prefix   <<   16)+index   )
#define   mTDAL_FS_GetHandlePrefix(   handle   )   (   (uint16_t)((handle&0xffff0000)   >>   16   )   )
#define   mTDAL_FS_GetHandleIndex(   handle   )      (   (uint16_t)(handle&0x0000ffff)   )


/*****************************************************************************
 *  Structures                                           *
 *****************************************************************************/
struct   TDAL_FS_Message
{
   tTDAL_FS_DeviceName   device_name;
   tTDAL_FS_DeviceType      device_type;
   tTDAL_FS_DeviceEvent   event;
   bool        toBeRenotified;
};


/*****************************************************************************
 *  Global Variables (GLOBAL/IMPORT)                             *
 *****************************************************************************/
mTBOX_SET_MODULE(eTDAL_FS);


/*****************************************************************************
 *  Global Variables (GLOBAL/IMPORT)                             *
 *****************************************************************************/


/*****************************************************************************
 *  Local File Variables (LOCAL)                              *
 *****************************************************************************/
/*   Array   */
LOCAL   struct   TDAL_FS_PrivateDeviceInfo_p   TDAL_FS_DeviceArray[kTDAL_FS_MAX_DEVICES];
LOCAL   struct   TDAL_FS_PrivateVolumeInfo_p   TDAL_FS_VolumeArray[kTDAL_FS_MAX_DEVICES   *   kTDAL_FS_MAX_VOLUMES_PER_DEVICE];
/*   Mutexes   */
LOCAL   TDAL_mutex_id            TDAL_FS_DeviceMutex;
LOCAL   TDAL_mutex_id            TDAL_FS_VolumeMutex;
/*   Item   counter   */
LOCAL   uint8_t            TDAL_FS_DeviceCount = 0;
LOCAL   uint8_t            TDAL_FS_VolumeCount = 0;

LOCAL   tTDAL_FS_DeviceCallback      TDAL_FS_DeviceCallback;

LOCAL   struct   TDAL_FS_Message      TDAL_FS_MessageToBeRenotified[5];
LOCAL   int8_t            TDAL_FS_IndexMessage = 0;
LOCAL	bool bUSB_Init = FALSE;

/*****************************************************************************
 *  Global Functions Declarations (IMPORT)                         *
 *****************************************************************************/

IMPORT MS_BOOL MApi_SystemDeviceHost_Init(void);


#ifdef USE_TDAL_PVR 
IMPORT void TDAL_PVRm_UsbRemoved();

#endif

/*****************************************************************************
 *  Local Functions Declarations (LOCAL)                         *
 *****************************************************************************/
LOCAL   uint8_t   pTDAL_FS_CheckDeviceHandle(   tTDAL_FS_DeviceHandle   DeviceHandle   );
LOCAL   uint8_t   pTDAL_FS_CheckVolumeHandle(   tTDAL_FS_VolumeHandle   VolumeHandle   );

LOCAL   void   pTDAL_FS_device_callback_fn(   MS_U8 u8Event, MS_U32 u32Para,char *s);
 LOCAL   void   pTDAL_FS_DeviceNotificationCallback(   char   *   , tTDAL_FS_DeviceEvent   );

LOCAL   bool   pTDAL_FS_DeviceInserted(   char   *   , tTDAL_FS_DeviceType   device_type, tTDAL_FS_DeviceHandle   *   , uint32_t   *   , uint32_t   *   );
LOCAL   bool   pTDAL_FS_DeviceRemoved(   char   *   );
LOCAL bool pTDAL_FS_VolumeInserted(tTDAL_FS_DeviceHandle DeviceHandle);
LOCAL bool pTDAL_FS_VolumeRemoved(char * device_name);
LOCAL   tTDAL_FS_VolumeType   pTDAL_FS_ConvertVolumeType_FromRawType(   uint8_t   );
#if 0
LOCAL   uint8_t      pTDAL_FS_ConvertVolumeType_ToRawType(   tTDAL_FS_VolumeType   );
#endif

LOCAL bool pTDAL_FS_GetDeviceInfo(char * device_name,  cyg_disk_info_t * pInfo, bool init);
LOCAL void pTDAL_FS_GetVolumeName(uint8_t deviceIndex, uint8_t volumeIndex, char * volumeName, uint32_t volumeNameLength);

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
tTDAL_FS_Error      pTDAL_FS_LVM_Init(   void   )
{
   tTDAL_FS_Error   result = eTDAL_FS_NO_ERROR;

#ifdef   PRODUCT_USE_ATAPI
   tTDAL_FS_Error   tdal_fs_err;
#endif

   mTBOX_FCT_ENTER("pTDAL_FS_LVM_Init");

   mTBOX_TRACE((kTBOX_NIV_5,"pTDAL_FS_LVM_Init()\n"));


   /* if   (   lvm_initialize(kTDAL_FS_LVM_POLLING_FREQ) != 0   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Init:   [ERROR]   lvm_initialize(   %d   Hz   )   failed\n", kTDAL_FS_LVM_POLLING_FREQ));
      result = eTDAL_FS_ERROR_NOT_DONE;
   }*/


   memset(   TDAL_FS_DeviceArray   ,
      0   ,
      kTDAL_FS_MAX_DEVICES   *   sizeof(struct   TDAL_FS_PrivateDeviceInfo_p)   );
   memset(   TDAL_FS_VolumeArray   ,
      0   ,
      kTDAL_FS_MAX_DEVICES   *   kTDAL_FS_MAX_VOLUMES_PER_DEVICE   *   sizeof(struct   TDAL_FS_PrivateVolumeInfo_p)   );


   /**   Create   'Devices'   and   'Volumes'   mutexes   **/
   TDAL_CreateMutex(   &TDAL_FS_DeviceMutex   );
   TDAL_CreateMutex(   &TDAL_FS_VolumeMutex   );

   /**   Init   the   task   **/
   if(!bUSB_Init)
   {
      MApi_SystemDeviceHost_Init();
      MDrv_USB_RegisterCallBack(pTDAL_FS_device_callback_fn);
      bUSB_Init = TRUE;
   }
#ifdef   PRODUCT_USE_ATAPI
   tdal_fs_err = TDAL_FSi_SetupAtapiDevice();
   if   (tdal_fs_err != eTDAL_FS_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_Init:   TDAL_FSi_SetupAtapiDevice   failed   (tdal_fs_err=%d)\n",tdal_fs_err));
      /*mTBOX_RETURN(   tdal_fs_err   );*/
   }
#endif


   #if   0
   {
      int   i;
      i = lvm_partition_delete_all(   "STATAPI1"   );
      printf("lvm_partition_delete_all   returns   %d\n", i);
   }
   #endif

   mTBOX_RETURN(result);
}

void TDAL_FSi_DeviceCallback (tTDALm_WorkerThreadsDescriptor * descriptor)
{
    bool b_result;
    tTDAL_FS_DeviceHandle device_handle;
    tTDAL_FS_DeviceHandle removed_device_handle = 0;
    tTDAL_FS_DeviceInfo device_info;
    uint32_t block_size;
    uint32_t block_count;
    uint8_t uc_loop;
    struct   TDAL_FS_Message * pmsg;

    pmsg = (struct   TDAL_FS_Message *) descriptor->pointer;

    if(pmsg->event == eTDAL_FS_DEVICE_EVENT_REMOVED)
    {
        //TDAL_MPm_UsbRemoved();

#ifdef USE_TDAL_PVR         
        TDAL_PVRm_UsbRemoved();
#endif
    }

    //  Process   the   message
    mTBOX_TRACE((kTBOX_NIV_2,"pTDAL_FS_MessageHandlerTaskFct:   Dequeued   msg   :   \"%s\"   %s\n",
                      pmsg->device_name ,
                    ( pmsg->event == eTDAL_FS_DEVICE_EVENT_INSERTED ?
                            "inserted" : "removed" ) ));
    if ((pmsg->event == eTDAL_FS_DEVICE_EVENT_INSERTED)
            && (pmsg->toBeRenotified == false))
    {
        b_result = pTDAL_FS_DeviceInserted(pmsg->device_name,
                pmsg->device_type, &device_handle, &block_size,
                &block_count);
        if (b_result == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_MessageHandlerTaskFct:   [ERROR]   pTDAL_FS_DeviceInserted(   %s   )   failed\n", pmsg->device_name));
        }
        else
        {
            b_result = pTDAL_FS_VolumeInserted(device_handle);
            if (b_result == FALSE)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_MessageHandlerTaskFct:   [ERROR]   pTDAL_FS_VolumeInserted()   failed\n"));
            }
        }
    }
    else if ((pmsg->event == eTDAL_FS_DEVICE_EVENT_REMOVED)
            && (pmsg->toBeRenotified == false))
    {
        for (uc_loop = 0; uc_loop < kTDAL_FS_MAX_DEVICES; uc_loop++)
        {
            if (strcmp(pmsg->device_name,
                    TDAL_FS_DeviceArray[uc_loop].name) == 0)
            {
                removed_device_handle =
                        TDAL_FS_DeviceArray[uc_loop].handle;
                break;
            }
        }

        b_result = pTDAL_FS_VolumeRemoved(pmsg->device_name);
        if (b_result == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_MessageHandlerTaskFct:   [ERROR]   pTDAL_FS_VolumeRemoved(   %s   )   failed\n",pmsg->device_name));
        }
        b_result = pTDAL_FS_DeviceRemoved(pmsg->device_name);
        if (b_result == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_MessageHandlerTaskFct:   [ERROR]   pTDAL_FS_DeviceRemoved(   %s   )   failed\n",pmsg->device_name));
        }
    }

    //   Notify   clients'   callbacks
    if ((pmsg->event == eTDAL_FS_DEVICE_EVENT_INSERTED)
            && (TDAL_FS_DeviceCallback != NULL))
    {
        b_result = false;
        for (uc_loop = 0; uc_loop < kTDAL_FS_MAX_DEVICES; uc_loop++)
        {
            if (strcmp(pmsg->device_name,
                    TDAL_FS_DeviceArray[uc_loop].name) == 0)
            {
                b_result = true;
                break;
            }
        }

        if (b_result)
        {
            strcpy(device_info.name, pmsg->device_name);
            device_info.type = TDAL_FS_DeviceArray[uc_loop].type;
            device_info.handle = TDAL_FS_DeviceArray[uc_loop].handle;
            device_info.block_size =
                    TDAL_FS_DeviceArray[uc_loop].block_size;
            device_info.block_count =
                    TDAL_FS_DeviceArray[uc_loop].block_count;
            (*TDAL_FS_DeviceCallback)(&device_info, pmsg->event);
        }

    }
    else if ((pmsg->event == eTDAL_FS_DEVICE_EVENT_REMOVED)
            && (TDAL_FS_DeviceCallback != NULL))
    {

        device_info.handle = removed_device_handle;
        strcpy(device_info.name, pmsg->device_name);
        device_info.type = pmsg->device_type;

        (*TDAL_FS_DeviceCallback)(&device_info, pmsg->event);

    }

    if (TDAL_FS_DeviceCallback == NULL)
    {
        strncpy(
                TDAL_FS_MessageToBeRenotified[TDAL_FS_IndexMessage].device_name,
                pmsg->device_name, kTDAL_FS_DEVICE_NAME_LEN_MAX);
        TDAL_FS_MessageToBeRenotified[TDAL_FS_IndexMessage].device_type =
                pmsg->device_type;
        TDAL_FS_MessageToBeRenotified[TDAL_FS_IndexMessage].event =
                pmsg->event;
        TDAL_FS_MessageToBeRenotified[TDAL_FS_IndexMessage].toBeRenotified =
                true;
        TDAL_FS_IndexMessage++;
    }

    TDAL_Free(descriptor->pointer);
    descriptor->pointer = NULL;
}


tTDAL_FS_Error      TDAL_FS_RegisterDeviceCallback(   tTDAL_FS_DeviceCallback   callback   )
{
	tTDAL_FS_Error result;
	struct TDAL_FS_Message * pmsg;
	uint8_t i;
	tTDALm_WorkerThreadsDescriptor desc;

	mTBOX_FCT_ENTER("TDAL_FS_RegisterDeviceCallback"); mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_RegisterDeviceCallback()\n"));

	result = eTDAL_FS_NO_ERROR;

	/*   save   Callback   variable   */
	TDAL_FS_DeviceCallback = callback;

	/*   put   msg   for   devices   that   must   be   renotified   */
	for (i = 0; i < (TDAL_FS_IndexMessage); i++)
	{
        pmsg = TDAL_Malloc(sizeof(struct TDAL_FS_Message));
        if (pmsg)
        {
            mTBOX_TRACE((kTBOX_NIV_2,"TDAL_FS_RegisterDeviceCallback   :   %s\n",TDAL_FS_MessageToBeRenotified[i].device_name));
            strncpy(pmsg->device_name, TDAL_FS_MessageToBeRenotified[i].device_name,
                    kTDAL_FS_DEVICE_NAME_LEN_MAX);
            pmsg->device_type = TDAL_FS_MessageToBeRenotified[i].device_type;
            pmsg->event = TDAL_FS_MessageToBeRenotified[i].event;
            pmsg->toBeRenotified = TDAL_FS_MessageToBeRenotified[i].toBeRenotified;

            desc.pointer = pmsg;

            if (!TDALm_WorkerThreadsSendRequest(TDAL_FSi_DeviceCallback, &desc))
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_FS_RegisterDeviceCallback: TDALm_WorkerThreadsSendRequest error\n"));
            }
        }
        else
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_FS_RegisterDeviceCallback: Could not allocate msg\n"));
        }
	}

	mTBOX_RETURN(result);
}

tTDAL_FS_Error      pTDAL_FS_LVM_Term(   void   )
{
	tTDAL_FS_Error result = eTDAL_FS_NO_ERROR;

	mTBOX_FCT_ENTER("TDAL_VFS_Term");

	MDrv_USB_RegisterCallBack(NULL);

	TDAL_DeleteMutex(TDAL_FS_DeviceMutex);
	TDAL_DeleteMutex(TDAL_FS_VolumeMutex);

	mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_Term()\n"));
	mTBOX_RETURN(result);
}


tTDAL_FS_Error      TDAL_FS_ListDevices(
            uint8_t   *      pDevicesCount   ,
            tTDAL_FS_DeviceInfo   **      ppDevicesArray   )
{
	tTDAL_FS_Error result = eTDAL_FS_NO_ERROR;
	uint8_t devices_count;
	tTDAL_FS_DeviceInfo * devices_array;
	uint8_t uc_loop;
	uint8_t uc_loop1;

	mTBOX_FCT_ENTER("TDAL_FS_ListDevices");

	mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_ListDevices()\n"));

	/*   check   param   */
	if ((pDevicesCount == NULL) || (ppDevicesArray == NULL))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_ListDevices:   [ERROR]   Bad   argument   'pDevicesCount'=0x%x   and   'ppDevicesArray'=0x%x\n", pDevicesCount, ppDevicesArray));
		result = eTDAL_FS_ERROR_BAD_ARG;
	}

	if (result == eTDAL_FS_NO_ERROR)
	{
		devices_count = TDAL_FS_DeviceCount;
		if (devices_count != 0)
		{
			devices_array = TDAL_Malloc(
					devices_count * sizeof(tTDAL_FS_DeviceInfo));
			if (devices_array == NULL)
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_ListDevices:   [ERROR]   Can't   allocate   the   'devices_array'\n"));
				result = eTDAL_FS_ERROR_NOT_DONE;
			}
		}
		else
		{
			devices_array = NULL;
		}
	}

	if (result == eTDAL_FS_NO_ERROR)
	{
		if (devices_count != 0)
		{
			uc_loop1 = 0;
			for (uc_loop = 0; uc_loop < kTDAL_FS_MAX_DEVICES; uc_loop++)
			{
				if (TDAL_FS_DeviceArray[uc_loop].handle != 0)
				{
					devices_array[uc_loop1].handle =
							TDAL_FS_DeviceArray[uc_loop].handle;
					memcpy(devices_array[uc_loop1].name,
							TDAL_FS_DeviceArray[uc_loop].name,
							kTDAL_FS_DEVICE_NAME_LEN_MAX);
					devices_array[uc_loop1].type =
							TDAL_FS_DeviceArray[uc_loop].type;
					devices_array[uc_loop1].block_size =
							TDAL_FS_DeviceArray[uc_loop].block_size;
					devices_array[uc_loop1].block_count =
							TDAL_FS_DeviceArray[uc_loop].block_count;
					uc_loop1++;
				}
			}
		}

		*pDevicesCount = devices_count;
		*ppDevicesArray = devices_array;
	}

	mTBOX_RETURN(result);
}


tTDAL_FS_Error      TDAL_FS_FreeDevicesArray(
            tTDAL_FS_DeviceInfo   *   pDevicesArray   )
{
	tTDAL_FS_Error result = eTDAL_FS_NO_ERROR;

	mTBOX_FCT_ENTER("TDAL_FS_FreeDevicesArray");

	mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_FreeDevicesArray()\n"));

	if (pDevicesArray != NULL)
	{
		TDAL_Free(pDevicesArray);
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_FreeDevicesArray:   [ERROR]   Bad   argument   'pDevicesArray'=0x%x\n", pDevicesArray));
		result = eTDAL_FS_ERROR_BAD_ARG;
	}

	mTBOX_RETURN(result);
}


tTDAL_FS_Error      TDAL_FS_GetDeviceInfo(   tTDAL_FS_DeviceHandle      DeviceHandle,
                                     tTDAL_FS_DeviceInfo   *Info)
{
	tTDAL_FS_Error result = eTDAL_FS_NO_ERROR;
	uint8_t device_index;

	mTBOX_FCT_ENTER("TDAL_FS_GetDeviceInfo");

	if (Info == NULL)
	{
		result = eTDAL_FS_ERROR_BAD_ARG;
	}

	if (result == eTDAL_FS_NO_ERROR)
	{
		device_index = pTDAL_FS_CheckDeviceHandle(DeviceHandle);
		if (device_index == INVALID_INDEX)
		{
			result = eTDAL_FS_ERROR_BAD_ARG;
		}
	}

	if (result == eTDAL_FS_NO_ERROR)
	{
		if (TDAL_FS_DeviceArray[device_index].handle != 0)
		{
			Info->handle = TDAL_FS_DeviceArray[device_index].handle;
			memcpy(Info->name, TDAL_FS_DeviceArray[device_index].name,
					kTDAL_FS_DEVICE_NAME_LEN_MAX);
			Info->type = TDAL_FS_DeviceArray[device_index].type;
			Info->block_size = TDAL_FS_DeviceArray[device_index].block_size;
			Info->block_count = TDAL_FS_DeviceArray[device_index].block_count;
		}
	}

	mTBOX_RETURN(result);
}

tTDAL_FS_Error      TDAL_FS_GetVolumeExtendedInfo(tTDAL_FS_VolumeHandle   VolumeHandle,
                                     tTDAL_FS_VolumeExtendedInfo   *ExtendedInfo)
{
   tTDAL_FS_Error      result = eTDAL_FS_NO_ERROR;
#if   0
   uint8_t            volume_index;

   device_handle_t   *      device_hndl=NULL;
   uint8_t   *        buffer=NULL;
   lba_t            lba;
   int32_t            block_read;
#endif

   mTBOX_FCT_ENTER("TDAL_FS_GetVolumeExtendedInfo");

   printf("TDAL_FS_GetVolumeExtendedInfo NOT IMPLEMENTED");

   strcpy(ExtendedInfo->VolumeName, "UNKNOWN VOLUME");
   return eTDAL_FS_NO_ERROR;

#if   0
   if   (   ExtendedInfo == NULL)
   {
      result = eTDAL_FS_ERROR_BAD_ARG;
   }

   if   (   result == eTDAL_FS_NO_ERROR   )
   {
      volume_index = pTDAL_FS_CheckVolumeHandle(   VolumeHandle   );
      if   (volume_index == INVALID_INDEX || TDAL_FS_VolumeArray[volume_index].valid_flag == INVALID_INFO   )
      {
      result = eTDAL_FS_ERROR_BAD_ARG;
      }
   }

   if   (   result == eTDAL_FS_NO_ERROR   )
   {
      memset(ExtendedInfo,0,sizeof(tTDAL_FS_VolumeExtendedInfo));

      switch(   TDAL_FS_VolumeArray[volume_index].type   )
      {
      case   eTDAL_FS_VOLUME_TYPE_FAT12:
      case   eTDAL_FS_VOLUME_TYPE_FAT16:
      case   eTDAL_FS_VOLUME_TYPE_FAT32:
      case   eTDAL_FS_VOLUME_TYPE_FAT32_LBA:
      {
      /*   Read   FAT   boot   sector   to   extract   Volume   Name   */
      if   ((device_hndl = device_open(TDAL_FS_VolumeArray[volume_index].name   ,OSPLUS_O_RDWR)) == NULL)
      {
        result = eTDAL_FS_ERROR_NOT_DONE;
      }
      else
      {

        if   (   result == eTDAL_FS_NO_ERROR   )
        {
            buffer = (unsigned   char   *)osplus_cache_safe_allocate(   NULL   , 512   );
            if   (   buffer == NULL   )
            {
            result = eTDAL_FS_ERROR_NO_MEMORY;
            }
        }

        if   (   result == eTDAL_FS_NO_ERROR   )
        {
            lba = 0; /*boot   sector*/

            block_read = device_block_read(device_hndl, buffer,lba, 1);
            if   (block_read == -1)
            {
            result = eTDAL_FS_ERROR_NOT_DONE;
            }
        }

        if   (   result == eTDAL_FS_NO_ERROR   )
        {
            if   (   TDAL_FS_VolumeArray[volume_index].type == eTDAL_FS_VOLUME_TYPE_FAT12   ||
            TDAL_FS_VolumeArray[volume_index].type == eTDAL_FS_VOLUME_TYPE_FAT16   )
            {
            strncpy(ExtendedInfo->VolumeName, (char*)&(buffer[43]), 11);
            }
            else
            {
            strncpy(ExtendedInfo->VolumeName, (char*)&(buffer[71]), 11);
            }
        }

        osplus_cache_safe_deallocate(   NULL   , buffer   );
        device_close(device_hndl);
      }
      break;
      }
      default   :
      {
      /*   none   */
      break;
      }
      }
   }
/*
   if   (   result == eTDAL_FS_NO_ERROR   )
   {
      printf("--vol   ext   info   ---\n");
      printf("%s\n",ExtendedInfo->VolumeName   );
      printf("---------------\n");
   }
*/
#endif
   mTBOX_RETURN(result);
}



tTDAL_FS_Error      TDAL_FS_GetDeviceExtendedInfo(   tTDAL_FS_DeviceHandle      DeviceHandle,
                                     tTDAL_FS_DeviceExtendedInfo   *ExtendedInfo)
{
	tTDAL_FS_Error result = eTDAL_FS_NO_ERROR;
	uint8_t device_index;

	mTBOX_FCT_ENTER("TDAL_FS_GetDeviceExtendedInfo");

	if (ExtendedInfo == NULL)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_FS_GetDeviceExtendedInfo bad argument\n"));
		result = eTDAL_FS_ERROR_BAD_ARG;
	}

	if (result == eTDAL_FS_NO_ERROR)
	{
		memset(ExtendedInfo, 0, sizeof(tTDAL_FS_DeviceExtendedInfo));

		device_index = pTDAL_FS_CheckDeviceHandle(DeviceHandle);
		if (device_index == INVALID_INDEX)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_FS_GetDeviceExtendedInfo] bad device handle\n"));
			result = eTDAL_FS_ERROR_BAD_ARG;
		}
	}

	if (result == eTDAL_FS_NO_ERROR)
	{
		if (TDAL_FS_DeviceArray[device_index].type
				== eTDAL_FS_DEVICETYPE_REMOVABLE_STORAGE)
		{
			cyg_disk_info_t info;
			if (pTDAL_FS_GetDeviceInfo(TDAL_FS_DeviceArray[device_index].name,  &info, false))
			{
				strncpy(ExtendedInfo->manufacturer,info.ident.firmware_rev,  kTDAL_FS_EXTENDEDINFO_NAME_LEN_MAX);
				ExtendedInfo->manufacturer[kTDAL_FS_EXTENDEDINFO_NAME_LEN_MAX - 1] = 0;

				strncpy(ExtendedInfo->product,info.ident.model_num, kTDAL_FS_EXTENDEDINFO_NAME_LEN_MAX);
				ExtendedInfo->product[kTDAL_FS_EXTENDEDINFO_NAME_LEN_MAX - 1] = 0;

				strncpy(ExtendedInfo->serial, info.ident.serial, kTDAL_FS_EXTENDEDINFO_NAME_LEN_MAX);
				ExtendedInfo->serial[kTDAL_FS_EXTENDEDINFO_NAME_LEN_MAX - 1] = 0;
			}
			else
			{
				result = eTDAL_FS_NO_ERROR;
				strcpy(ExtendedInfo->manufacturer, "-?-");
			}

		}
		else if (TDAL_FS_DeviceArray[device_index].type
				== eTDAL_FS_DEVICETYPE_FIXED_STORAGE)
		{
			strcpy(ExtendedInfo->manufacturer, "INTERNAL   HDD");
		}
		else
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_FS_GetDeviceExtendedInfo] unknown device type %d\n", TDAL_FS_DeviceArray[device_index].type));
			result = eTDAL_FS_ERROR_NOT_DONE;
		}
	}

	strncpy(ExtendedInfo->manufacturer, "Generic",
			kTDAL_FS_EXTENDEDINFO_NAME_LEN_MAX);
	ExtendedInfo->manufacturer[kTDAL_FS_EXTENDEDINFO_NAME_LEN_MAX - 1] = 0;

	strncpy(ExtendedInfo->product, "USB Drive",
			kTDAL_FS_EXTENDEDINFO_NAME_LEN_MAX);
	ExtendedInfo->product[kTDAL_FS_EXTENDEDINFO_NAME_LEN_MAX - 1] = 0;

	strncpy(ExtendedInfo->serial, "181098472-2813",
			kTDAL_FS_EXTENDEDINFO_NAME_LEN_MAX);
	ExtendedInfo->serial[kTDAL_FS_EXTENDEDINFO_NAME_LEN_MAX - 1] = 0;

	mTBOX_TRACE((kTBOX_NIV_1, "--dev   ext   info   ---\n"));
	mTBOX_TRACE((kTBOX_NIV_1, "MANUFACTURER: %s\n",ExtendedInfo->manufacturer));
	mTBOX_TRACE((kTBOX_NIV_1, "PRODUCT:      %s\n",ExtendedInfo->product));
	mTBOX_TRACE((kTBOX_NIV_1, "SERIAL:       %s\n",ExtendedInfo->serial));
	mTBOX_TRACE((kTBOX_NIV_1, "---------------\n"));

	mTBOX_RETURN(result);
}

tTDAL_FS_Error      TDAL_FS_ListVolumes(
            tTDAL_FS_DeviceHandle      DeviceHandle   ,
            uint8_t   *      pVolumesCount   ,
            tTDAL_FS_VolumeInfo   **      ppVolumesArray   )
{
	tTDAL_FS_Error result = eTDAL_FS_NO_ERROR;
	uint8_t volumes_count;
	tTDAL_FS_VolumeInfo * volumes_array;
	uint8_t uc_loop;
	uint8_t uc_loop2;
	uint8_t device_index;

	mTBOX_FCT_ENTER("TDAL_FS_ListVolumes");

	mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_ListVolumes()\n"));

	/***   Check   params   ***/
	if ((pVolumesCount == NULL) || (ppVolumesArray == NULL))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_ListDevices:   [ERROR]   Bad   argument   'pVolumesCount'=0x%x   and   'ppVolumesArray'=0x%x\n", pVolumesCount, ppVolumesArray));
		result = eTDAL_FS_ERROR_BAD_ARG;
	}

	if (result == eTDAL_FS_NO_ERROR)
	{
		device_index = pTDAL_FS_CheckDeviceHandle(DeviceHandle);
		if (device_index == INVALID_INDEX)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_ListVolumes:   [ERROR]   pTDAL_FS_CheckDeviceHandle()   failed\n"));
			result = eTDAL_FS_ERROR_BAD_ARG;
		}
	}

	/***   Main   operations   ***/
	/*   1.   get   volumes   number   and   information   */
	volumes_count = TDAL_FS_DeviceArray[device_index].volumes_count;

	/*   2.   allocate   the   VolumeInfo   array   to   give   to   client   */
	if (result == eTDAL_FS_NO_ERROR)
	{
		if (volumes_count != 0)
		{
			mTBOX_TRACE((kTBOX_NIV_2,"TDAL_FS_ListVolumes:   volumes_array[%d]   allocated\n",
							volumes_count));
			volumes_array = TDAL_Malloc(
					volumes_count * sizeof(tTDAL_FS_VolumeInfo));
			if (volumes_array == NULL)
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_ListVolumes:   [ERROR]   Can't   allocate   the   'volumes_array'\n"));
				result = eTDAL_FS_ERROR_NOT_DONE;
			}
		}
		else
		{
			volumes_array = NULL;
		}
	}

	/*   3.   fill   the   VolumeInfo   array   (if   necessary)   */
	if (result == eTDAL_FS_NO_ERROR)
	{
		if (volumes_count != 0)
		{
			TDAL_LockMutex(TDAL_FS_VolumeMutex);
			uc_loop2 = 0;
			for (uc_loop = 0; uc_loop < kTDAL_FS_MAX_VOLUMES_PER_DEVICE;
					uc_loop++)
			{
				if (TDAL_FS_VolumeArray[uc_loop].valid_flag == VALID_INFO)
				{
					if (TDAL_FS_VolumeArray[uc_loop].device_handle
							== DeviceHandle)
					{
						memcpy(volumes_array[uc_loop2].name,
								TDAL_FS_VolumeArray[uc_loop].name,
								kTDAL_FS_VOLUME_NAME_LEN_MAX);
						volumes_array[uc_loop2].handle =
								TDAL_FS_VolumeArray[uc_loop].handle;
						volumes_array[uc_loop2].size =
								TDAL_FS_VolumeArray[uc_loop].size;
						volumes_array[uc_loop2].type =
								TDAL_FS_VolumeArray[uc_loop].type;
						mTBOX_TRACE((kTBOX_NIV_2,"TDAL_FS_ListVolumes:   volumes_array[%d]   filled\n",
										uc_loop2));
						uc_loop2++;
					}
				}
			}
			TDAL_UnlockMutex(TDAL_FS_VolumeMutex);
		}

		*pVolumesCount = volumes_count;
		*ppVolumesArray = volumes_array;
		mTBOX_TRACE((kTBOX_NIV_2,"TDAL_FS_ListVolumes:   volumes_array[%d]=0x%x\n",
						volumes_count,
						volumes_array));
	}

	mTBOX_RETURN(result);
}


tTDAL_FS_Error      TDAL_FS_FreeVolumesArray(
            tTDAL_FS_VolumeInfo   *   pVolumesArray   )
{
	tTDAL_FS_Error result = eTDAL_FS_NO_ERROR;

	mTBOX_FCT_ENTER("TDAL_FS_FreeVolumesArray");

	mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_FreeVolumesArray()\n"));

	if (pVolumesArray != NULL)
	{
		mTBOX_TRACE((kTBOX_NIV_2,"TDAL_FS_FreeVolumesArray:   volumes_array=0x%x\n",
						pVolumesArray));

		TDAL_Free(pVolumesArray);
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_FreeVolumesArray:   [ERROR]   Bad   argument   'pVolumesArray'=0x%x\n", pVolumesArray));
		result = eTDAL_FS_ERROR_BAD_ARG;
	}

	mTBOX_RETURN(result);
}

tTDAL_FS_Error     pTDAL_FS_CreateParititons(tTDAL_FS_DeviceHandle      DeviceHandle,  tTDAL_FS_VolumeInfo   *   pVolumeInfo)
{

    tTDAL_FS_Error result;
    uint8_t      device_index;
    uint8_t      volume_index;
    do_partitions_info     buf;
    cyg_io_handle_t device;
    cyg_uint32 len;
    MS_S32 err;
    uint8_t partitionCount;
    uint32_t partitionSizeMB;

    char volume[30];

    mTBOX_FCT_ENTER(pTDAL_FS_CreateParititons);

    result = eTDAL_FS_NO_ERROR;
    partitionCount = 0;
    partitionSizeMB = 0;

    if (result == eTDAL_FS_NO_ERROR)
    {
        device_index = pTDAL_FS_CheckDeviceHandle(DeviceHandle);
        if (device_index == INVALID_INDEX)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_CreateVolume:   [ERROR]   pTDAL_FS_CheckDeviceHandle()   failed\n"));
            result = eTDAL_FS_ERROR_BAD_ARG;
        }
    }

    if ( result == eTDAL_FS_NO_ERROR )
    {
        err = cyg_io_lookup( TDAL_FS_DeviceArray[device_index].name , &device );
        if (err != ENOERR)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not find device %s\n", TDAL_FS_DeviceArray[device_index].name ));
            result = eTDAL_FS_ERROR_NOT_DONE;
        }
    }

    if (result == eTDAL_FS_NO_ERROR && pVolumeInfo != NULL)
    {
        partitionCount = 1;
        partitionSizeMB = TDAL_FS_DeviceArray[device_index].block_size * (pVolumeInfo->size / 1024) / 1024;
    }
    else
    {
        partitionCount = 1;
        partitionSizeMB = 0;
    }
    
    if   (   result == eTDAL_FS_NO_ERROR   )
    {
        /**   Create   the   volume   **/
        len = sizeof(do_partitions_info);

        buf.devname = TDAL_FS_DeviceArray[device_index].name;
        buf.part_count = partitionCount;
        buf.p1_size_MB = partitionSizeMB;

        err = cyg_io_set_config(device, CYG_IO_SET_CONFIG_DISK_SET_PARTITIONS, &buf, &len);
        if (err != ENOERR)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not create volume on device %s\n", TDAL_FS_DeviceArray[device_index].name ));
            result = eTDAL_FS_ERROR_NOT_DONE;
        }
    }

    if   (   result == eTDAL_FS_NO_ERROR   )
    {
        /**   Add   the   created   volume   into   the   VolumeArray   **/
        TDAL_LockMutex(TDAL_FS_VolumeMutex);

        for (volume_index = 0; volume_index < kTDAL_FS_MAX_VOLUMES_PER_DEVICE;
                volume_index++)
        {
            TDAL_FS_VolumeArray[volume_index].valid_flag = INVALID_INFO;
        }

        TDAL_UnlockMutex(TDAL_FS_VolumeMutex);

        if (pTDAL_FS_VolumeInserted(DeviceHandle) == FALSE)
        {
            result = eTDAL_FS_ERROR_NOT_DONE;
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_CreateVolume:   Could not add volume to list   UNKNOWN   type\n"));
        }

    }

    mTBOX_RETURN(result);
}


tTDAL_FS_Error      TDAL_FS_CreateVolume(
            tTDAL_FS_DeviceHandle      DeviceHandle   ,
            tTDAL_FS_VolumeInfo   *   pVolumeInfo   )
{
   tTDAL_FS_Error      result = eTDAL_FS_NO_ERROR;

   mTBOX_FCT_ENTER("TDAL_FS_CreateVolume");

   mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_CreateVolume()\n"));


   /***   Check   params   ***/
    if (pVolumeInfo == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_CreateVolume:   [ERROR]   Bad   argument   'pVolumeInfo'=0x%x\n", pVolumeInfo));
        result = eTDAL_FS_ERROR_BAD_ARG;
    }

    if (result == eTDAL_FS_NO_ERROR)
    {
        result = pTDAL_FS_CreateParititons(DeviceHandle, pVolumeInfo);
    }


    if (result == eTDAL_FS_NO_ERROR)
    {
        /**   Fill   the   pVolumeInfo   structure   **/
        int volume_index = 0;
        if (TDAL_FS_VolumeArray[volume_index].valid_flag  == VALID_INFO)
        {
            pVolumeInfo->handle = TDAL_FS_VolumeArray[volume_index].handle;
            strncpy(   pVolumeInfo->name   ,
              TDAL_FS_VolumeArray[volume_index].name   ,
              kTDAL_FS_VOLUME_NAME_LEN_MAX   );
            pVolumeInfo->size = TDAL_FS_VolumeArray[volume_index].size;
            pVolumeInfo->type = TDAL_FS_VolumeArray[volume_index].type;
        }
        else
        {
            result = eTDAL_FS_ERROR_NOT_DONE;
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_CreateVolume:   No volume found in list\n"));
        }
    }

    mTBOX_RETURN(result);

}


tTDAL_FS_Error      TDAL_FS_DeleteVolume(
            tTDAL_FS_DeviceHandle      DeviceHandle   ,
            tTDAL_FS_VolumeHandle      VolumeHandle   )
{
   tTDAL_FS_Error   result = eTDAL_FS_NO_ERROR;

   mTBOX_FCT_ENTER("TDAL_FS_DeleteVolume");

   mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_DeleteVolume()\n"));

   result = pTDAL_FS_CreateParititons(DeviceHandle, NULL);

   mTBOX_RETURN(result);
}


tTDAL_FS_Error      TDAL_FS_ChangeVolumeType(
            tTDAL_FS_DeviceHandle      DeviceHandle   ,
            tTDAL_FS_VolumeHandle      VolumeHandle   ,
            tTDAL_FS_VolumeType      VolumeType   )
{
   tTDAL_FS_Error   result = eTDAL_FS_NO_ERROR;
   mTBOX_FCT_ENTER("TDAL_FS_ChangeVolumeType");
   mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FS_ChangeVolumeType()   is   not   implemented\n"));
   mTBOX_RETURN(result);
}

tTDAL_FS_Error      TDAL_FS_EraseDevice(
            tTDAL_FS_DeviceHandle      DeviceHandle   )
{
   tTDAL_FS_Error   result = eTDAL_FS_NO_ERROR;

   mTBOX_FCT_ENTER("TDAL_FS_EraseDevice");
   mTBOX_TRACE((kTBOX_NIV_5,"TDAL_FS_EraseDevice()\n"));

   result = pTDAL_FS_CreateParititons(DeviceHandle, NULL);

   mTBOX_RETURN(result);
}


GLOBAL   bool   TDAL_FSi_GetVolumeName(   tTDAL_FS_VolumeHandle   VolumeHandle   ,
                 char   *      pVolumeName)
{
	uint8_t volume_index;
	bool result = FALSE;

	if (pVolumeName != NULL)
	{
		volume_index = pTDAL_FS_CheckVolumeHandle(VolumeHandle);
		if (volume_index == INVALID_INDEX)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FSi_GetVolumeName:   [ERROR]   pTDAL_FS_CheckVolumeHandle()   failed\n"));
		}
		else
		{
			if (TDAL_FS_VolumeArray[volume_index].valid_flag == INVALID_INFO)
			{
				mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FSi_GetVolumeName:   [ERROR]   TDAL_FS_VolumeArray[%d]   with   flag=INVALID\n", volume_index));
			}
			result = TRUE;
			memcpy(pVolumeName, TDAL_FS_VolumeArray[volume_index].name,
					kTDAL_FS_VOLUME_NAME_LEN_MAX);
		}
	}

	return result;
}


GLOBAL   tTDAL_FS_VolumeType   TDAL_FSi_GetFormatType(tTDAL_FS_VolumeHandle   VolumeHandle)
{
	uint8_t volume_index;
	tTDAL_FS_VolumeType result = eTDAL_FS_VOLUME_TYPE_UNKNOWN;

	volume_index = pTDAL_FS_CheckVolumeHandle(VolumeHandle);
	if (volume_index == INVALID_INDEX)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FSi_GetFormatType:   [ERROR]   pTDAL_FS_CheckVolumeHandle()   failed\n"));
	}
	else
	{
		if (TDAL_FS_VolumeArray[volume_index].valid_flag == INVALID_INFO)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FSi_GetFormatType:   [ERROR]   TDAL_FS_VolumeArray[%d]   with   flag=INVALID\n", volume_index));
		}
		result = TDAL_FS_VolumeArray[volume_index].type;
	}

	return result;
}


LOCAL   uint8_t   pTDAL_FS_CheckDeviceHandle(   tTDAL_FS_DeviceHandle   DeviceHandle   )
{
	uint8_t index;

	if (mTDAL_FS_GetHandlePrefix(DeviceHandle) != kTDAL_FS_DEVICE_HANDLE_PREFIX)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_CheckDeviceHandle:   [ERROR]   The   'DeviceHandle'=0x%08x   prefix   seems   to   be   incorrect\n",DeviceHandle));
		return INVALID_INDEX;
	}

	index = mTDAL_FS_GetHandleIndex( DeviceHandle );
	if (TDAL_FS_DeviceArray[index].handle != DeviceHandle)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_CheckDeviceHandle:   [ERROR]   The   'DeviceHandle'=0x%08x   and   index=%d   seems   to   be   incorrect\n",DeviceHandle, index));
		return INVALID_INDEX;
	}

	return index;
}


LOCAL   uint8_t   pTDAL_FS_CheckVolumeHandle(   tTDAL_FS_VolumeHandle   VolumeHandle   )
{
	uint8_t index;

	if (mTDAL_FS_GetHandlePrefix(VolumeHandle) != kTDAL_FS_VOLUME_HANDLE_PREFIX)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_CheckVolumeHandle:   [ERROR]   The   'VolumeHandle'=0x%08x   prefix   seems   to   be   incorrect\n",VolumeHandle));
		return INVALID_INDEX;
	}

	index = mTDAL_FS_GetHandleIndex( VolumeHandle );
	if (TDAL_FS_VolumeArray[index].valid_flag == INVALID_INFO)
	{
		return INVALID_INDEX;
	}

	return index;
}




/********************************************************************************
 * Callback to be executed on every device insertion/extraction. This will
 * basically send a message for each USB event.
 *******************************************************************************/
void   pTDAL_FS_device_callback_fn(MS_U8 u8Event, MS_U32 u32Para,char *s)
{
	tTDAL_FS_DeviceEvent TDAL_FS_event;
    MS_U8 inputStr[30] = {0}; // /dev/sdx/
    MS_U8 *pch = NULL;
    char delim[] = "/";

	mTBOX_FCT_ENTER("pTDAL_FS_device_callback_fn");

    if (s == NULL || strlen(s) < 6) //  s = "/dev/sdx/", at least should be "/dev/x"
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "invalid mount device:%s\n", (s == NULL) ? "NULL" : s));
        mTBOX_RETURN;
    }

    if (u8Event == USB_PLUG_IN)
    {
    	TDAL_FS_event = eTDAL_FS_DEVICE_EVENT_INSERTED;
    }
    else if (u8Event == USB_PLUG_OUT)
    {
    	TDAL_FS_event = eTDAL_FS_DEVICE_EVENT_REMOVED;
    }
    else
    {
    	mTBOX_ASSERTm(FALSE, "Unknown event\n");
    	mTBOX_RETURN;
    }

    // copy input string for strtok usage
    strncpy(inputStr, s, sizeof(inputStr));
    inputStr[sizeof(inputStr - 1)] = 0;
    pch = strtok(inputStr, delim); // now pch = "dev"

    if (pch == NULL || strcmp(pch, "dev") != 0)
    {
    	mTBOX_TRACE((kTBOX_NIV_WARNING, "invalid mount device path:%s\n", (s == NULL) ? "NULL" : s));
    	mTBOX_RETURN ;
    }

//    pch = strtok (NULL, delim); // now pch = "sdx"
//    if (pch == NULL || strlen(pch) == 0)
//    {
//    	mTBOX_TRACE((kTBOX_NIV_WARNING, "invalid  mount path:%s\n", (s == NULL) ? "NULL" : s));
//        mTBOX_RETURN;
//    }

    pTDAL_FS_DeviceNotificationCallback(s, TDAL_FS_event);
}



LOCAL   void   pTDAL_FS_DeviceNotificationCallback(
        char   *        pDeviceName   ,
        tTDAL_FS_DeviceEvent   DeviceEvent
      )
{
	bool acceptIt;
	struct TDAL_FS_Message * pmsg;
	tTDAL_FS_DeviceType DeviceType;
	tTDALm_WorkerThreadsDescriptor desc;

	mTBOX_FCT_ENTER("TDAL_FS_DeviceNotificationCallback");

	DeviceType = eTDAL_FS_DEVICETYPE_UNKNOWN;
	acceptIt = false;
	pmsg     = NULL;

	if (strstr(pDeviceName, "sdx") != 0)
	{
		// USB
		DeviceType = eTDAL_FS_DEVICETYPE_REMOVABLE_STORAGE;
		acceptIt = true;
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Unsupported device type %s", pDeviceName));
	}

	if (acceptIt)
	{
	    pmsg = TDAL_Malloc(sizeof(struct TDAL_FS_Message));
        if (pmsg)
        {
            strncpy(pmsg->device_name, pDeviceName,
                    kTDAL_FS_DEVICE_NAME_LEN_MAX);
            pmsg->device_name[kTDAL_FS_DEVICE_NAME_LEN_MAX-1] = 0;
            pmsg->device_type = DeviceType;
            pmsg->event = DeviceEvent;
            pmsg->toBeRenotified = false;

            desc.pointer = (void *) pmsg;

            //strncpy(TDAL_FS_MessageToBeRenotified[TDAL_FS_IndexMessage].device_name, pDeviceName   , kTDAL_FS_DEVICE_NAME_LEN_MAX   );
            //TDAL_FS_MessageToBeRenotified[TDAL_FS_IndexMessage].device_type = DeviceType;
            //TDAL_FS_MessageToBeRenotified[TDAL_FS_IndexMessage].event = DeviceEvent;
            //TDAL_FS_MessageToBeRenotified[TDAL_FS_IndexMessage].toBeRenotified = true;
            //TDAL_FS_IndexMessage   ++;

            if (!TDALm_WorkerThreadsSendRequest(TDAL_FSi_DeviceCallback, &desc))
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDALm_WorkerThreadsSendRequest error\n"));
            }
        }
	}

	mTBOX_RETURN;
}



/*LOCAL   struct   TDAL_FS_Message   static_msg;*/

LOCAL bool pTDAL_FS_GetDeviceInfo(char * device_name,  cyg_disk_info_t * pInfo, bool init)
{
	int result;
	cyg_uint32 len = 1;
	char buf = 1;
	cyg_io_handle_t device;
	char dev_path[30];

	strncpy(dev_path, device_name, sizeof(dev_path));
	dev_path[sizeof(dev_path) - 1] = 0;
	strncat(dev_path, "0", sizeof(dev_path));

	result = cyg_io_lookup(dev_path, &device);

	if (init)
	{
		result = cyg_io_set_config(device, CYG_IO_SET_CONFIG_DISK_MOUNT, &buf,
				&len);
	}

	len = sizeof(cyg_disk_info_t);
	result = cyg_io_get_config(device, CYG_IO_GET_CONFIG_DISK_INFO, pInfo,
			&len);

	return result == 0;
}

LOCAL   bool   pTDAL_FS_DeviceInserted(   char   *      device_name   ,
                              tTDAL_FS_DeviceType      device_type,
                              tTDAL_FS_DeviceHandle   *   p_device_handle   ,
                              uint32_t   *      p_block_size   ,
                              uint32_t   *      p_block_count   )
{
	uint8_t uc_loop;
	bool b_result = TRUE;
	cyg_disk_info_t info;

	mTBOX_FCT_ENTER( "pTDAL_FS_DeviceInserted" );

	if ((p_block_size == NULL) || (p_block_count == NULL))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_DeviceInserted:   [ERROR]   one   of   the   parameter   is   NULL\n" ));
		b_result = FALSE;
	}
	else
	{
		b_result = pTDAL_FS_GetDeviceInfo(device_name, &info, true);
	}

	if (b_result == FALSE)
	{
		/*   error   with   the   Device   */
		/*TDAL_FS_DeviceCount = 0;*/
		*p_device_handle = (tTDAL_FS_DeviceHandle) NULL;
		*p_block_size = 0;
		*p_block_count = 0;
	}
	else
	{
		for (uc_loop = 0; uc_loop < kTDAL_FS_MAX_DEVICES; uc_loop++)
		{
			if (TDAL_FS_DeviceArray[uc_loop].handle == 0)
			{
				mTBOX_TRACE((kTBOX_NIV_1,"pTDAL_FS_DeviceInserted:   DEVICE_INDEX   :%d\n",uc_loop ));
				break;
			}
		}

		TDAL_LockMutex(TDAL_FS_DeviceMutex);

		TDAL_FS_DeviceArray[uc_loop].handle =
				mTDAL_FS_MakeHandle( kTDAL_FS_DEVICE_HANDLE_PREFIX , uc_loop );
		strncpy(TDAL_FS_DeviceArray[uc_loop].name, device_name,
				kTDAL_FS_DEVICE_NAME_LEN_MAX);
		TDAL_FS_DeviceArray[uc_loop].type = device_type;
		TDAL_FS_DeviceArray[uc_loop].block_size = info.block_size ;
		TDAL_FS_DeviceArray[uc_loop].block_count = info.blocks_num;

		/*   fields   'volume_count'   and   'volume_array'
		 *   would   be   updated  in   the   function   pTDAL_FS_VolumeInserted()   */

		TDAL_FS_DeviceCount++;
		*p_device_handle = TDAL_FS_DeviceArray[uc_loop].handle;
		*p_block_size = info.block_size;
		*p_block_count = info.blocks_num;

		TDAL_UnlockMutex(TDAL_FS_DeviceMutex);
	}

	mTBOX_RETURN(b_result);
}

LOCAL void pTDAL_FS_GetVolumeName(uint8_t deviceIndex, uint8_t volumeIndex, char * volumeName, uint32_t volumeNameLength)
{
    char num[10];

    snprintf(num, sizeof(num), "%d", volumeIndex);

    strncpy(volumeName, TDAL_FS_DeviceArray[deviceIndex].name, volumeNameLength);
    volumeName[volumeNameLength - 1] = 0;
    strncat(volumeName, num, volumeNameLength);
}


LOCAL bool pTDAL_FS_VolumeInserted(tTDAL_FS_DeviceHandle DeviceHandle)
{
	cyg_disk_partition_t * p_partition_info;
	char volume_name[30];
	uint8_t device_index;
	bool result = TRUE;
	uint8_t uc_loop;
	uint8_t uc_loop1;
	uint8_t volumes_count;
	uint8_t volumes_count_added;
	cyg_disk_info_t  info;

	mTBOX_FCT_ENTER( "pTDAL_FS_VolumeInserted" );

	device_index = pTDAL_FS_CheckDeviceHandle(DeviceHandle);
	if (device_index == INVALID_INDEX)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_VolumeInserted:   [ERROR]   pTDAL_FS_CheckDeviceHandle()   failed\n"));
		result = FALSE;
	}

	if (result == TRUE)
	{
		volumes_count = 0;
		volumes_count_added = 0;

		result = pTDAL_FS_GetDeviceInfo(TDAL_FS_DeviceArray[device_index].name, &info, false);

		if (result == FALSE)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_VolumeInserted:   pTDAL_FS_GetDeviceInfo returns error\n"));
		}

		for (uc_loop = 0; uc_loop < MBR_PART_NUM_MAX; uc_loop++)
		{
			if (info.partitions[uc_loop].type != 0) // fat32:0xb ntfs:0x7
			{
				++volumes_count;
			}
		}

		mTBOX_TRACE((kTBOX_NIV_1,"pTDAL_FS_VolumeInserted:   volumes_count=%d\n", volumes_count));
	}

	if (result == TRUE)
	{
		TDAL_LockMutex(TDAL_FS_VolumeMutex);
		for (uc_loop = 0; uc_loop < volumes_count; uc_loop++)
		{
			p_partition_info = &info.partitions[uc_loop];

			pTDAL_FS_GetVolumeName(device_index, uc_loop + 1, volume_name, sizeof(volume_name));

			if (p_partition_info != NULL)
			{
				if (p_partition_info->type == 0)
				{
					printf(
							"pTDAL_FS_VolumeInserted:   volume:\"%s\"   raw_type=0   size=%d   =>   volume   ignored\n",
							volume_name, p_partition_info->size);
					mTBOX_TRACE((kTBOX_NIV_CRITICAL,"pTDAL_FS_VolumeInserted:   volume:\"%s\"   raw_type=0   size=%d   =>   volume   ignored\n", volume_name, p_partition_info->size));
				}
				else
				{
					volumes_count_added++;
					for (uc_loop1 = 0;
							uc_loop1
									< (kTDAL_FS_MAX_DEVICES
											* kTDAL_FS_MAX_VOLUMES_PER_DEVICE);
							uc_loop1++)
					{
						if (TDAL_FS_VolumeArray[uc_loop1].valid_flag
								== INVALID_INFO)
						{
							mTBOX_TRACE((kTBOX_NIV_1,"pTDAL_FS_VolumeInserted:   VOLUME_INDEX   :%d\n",uc_loop1 ));
							break;
						}
					}

					TDAL_FS_VolumeArray[uc_loop1].valid_flag = VALID_INFO;
					TDAL_FS_VolumeArray[uc_loop1].handle =
							mTDAL_FS_MakeHandle( kTDAL_FS_VOLUME_HANDLE_PREFIX , uc_loop1 );
					TDAL_FS_VolumeArray[uc_loop1].device_handle =
							TDAL_FS_DeviceArray[device_index].handle;
					TDAL_FS_VolumeArray[uc_loop1].name[0] = '\0';
					TDAL_FS_VolumeArray[uc_loop1].size = 0;
					TDAL_FS_VolumeArray[uc_loop1].type =
							eTDAL_FS_VOLUME_TYPE_UNKNOWN;

					strncpy(TDAL_FS_VolumeArray[uc_loop1].name, volume_name,
							kTDAL_FS_VOLUME_NAME_LEN_MAX);
					TDAL_FS_VolumeArray[uc_loop1].size = p_partition_info->size;
					TDAL_FS_VolumeArray[uc_loop1].type =
							pTDAL_FS_ConvertVolumeType_FromRawType(
									p_partition_info->type);
					mTBOX_TRACE((kTBOX_NIV_2,"pTDAL_FS_VolumeInserted:   Name[%d] = \"%s\"      size=%d   type=%d\n", /*   raw_type=%d\n", */
									uc_loop,
									TDAL_FS_VolumeArray[uc_loop1].name,
									TDAL_FS_VolumeArray[uc_loop1].size,
									TDAL_FS_VolumeArray[uc_loop1].type ));
				}
			}
		} /*   for   */

		TDAL_FS_VolumeCount = TDAL_FS_VolumeCount + volumes_count_added;
		TDAL_UnlockMutex(TDAL_FS_VolumeMutex);

		TDAL_FS_DeviceArray[device_index].volumes_count = volumes_count_added;
	}

	mTBOX_RETURN(result);
}

LOCAL bool pTDAL_FS_VolumeRemoved(char * device_name)
{
	uint8_t uc_loop;
	uint8_t uc_loop1;
	uint8_t device_index = INVALID_INDEX;

	for (uc_loop1 = 0; uc_loop1 < kTDAL_FS_MAX_DEVICES; uc_loop1++)
	{
		if (strcmp(TDAL_FS_DeviceArray[uc_loop1].name, device_name) == 0)
		{
			device_index = uc_loop1;
			break;
		}
	}

	if (device_index != INVALID_INDEX)
	{
		TDAL_LockMutex(TDAL_FS_VolumeMutex);
		for (uc_loop = 0;
				uc_loop < kTDAL_FS_MAX_DEVICES * kTDAL_FS_MAX_VOLUMES_PER_DEVICE;
				uc_loop++)
		{
			if (TDAL_FS_DeviceArray[device_index].handle
					== TDAL_FS_VolumeArray[uc_loop].device_handle)
			{
				TDAL_FS_VolumeArray[uc_loop].valid_flag = INVALID_INFO;
				strcpy(TDAL_FS_VolumeArray[uc_loop].name, "");
				TDAL_FS_VolumeArray[uc_loop].handle = 0;
				TDAL_FS_VolumeArray[uc_loop].size = 0;
				TDAL_FS_VolumeArray[uc_loop].type = 0;
				TDAL_FS_VolumeArray[uc_loop].device_handle = 0;
			}
		}
		TDAL_FS_VolumeCount = TDAL_FS_VolumeCount
				- TDAL_FS_DeviceArray[device_index].volumes_count;
		TDAL_FS_DeviceArray[device_index].volumes_count = 0;
		TDAL_UnlockMutex(TDAL_FS_VolumeMutex);
	}
	return TRUE;
}


LOCAL   bool   pTDAL_FS_DeviceRemoved(   char   *   device_name   )
{
	uint8_t uc_loop1;
	uint8_t device_index = INVALID_INDEX;

	for (uc_loop1 = 0; uc_loop1 < kTDAL_FS_MAX_DEVICES; uc_loop1++)
	{
		if (strcmp(TDAL_FS_DeviceArray[uc_loop1].name, device_name) == 0)
		{
			/*device_index = pTDAL_FS_CheckDeviceHandle(   TDAL_FS_DeviceArray[uc_loop1].handle   );*/
			device_index = uc_loop1;
			break;
		}
	}

	if (device_index != INVALID_INDEX)
	{
		TDAL_LockMutex(TDAL_FS_DeviceMutex);

		TDAL_FS_DeviceArray[device_index].handle = 0;
		strcpy(TDAL_FS_DeviceArray[device_index].name, "");
		TDAL_FS_DeviceArray[device_index].block_size = 0;
		TDAL_FS_DeviceArray[device_index].block_count = 0;

		/*   fields   'volume_count'   and   'volume_array'   were   updated
		 *   in   the   function   pTDAL_FS_VolumeRemoved()   */

		TDAL_FS_DeviceCount--;

		TDAL_UnlockMutex(TDAL_FS_DeviceMutex);
	}

	return TRUE;
}


LOCAL   tTDAL_FS_VolumeType   pTDAL_FS_ConvertVolumeType_FromRawType(
            uint8_t   raw_type
            )
{
	switch (raw_type)
	{
	case 0x01:
	case 0x11:
		return eTDAL_FS_VOLUME_TYPE_FAT12;
	case 0x04:
	case 0x06:
	case 0x0E:
	case 0x14:
	case 0x16:
	case 0x1E:
		return eTDAL_FS_VOLUME_TYPE_FAT16;
	case 0x0B:
	case 0x0C:
	case 0x1B:
	case 0x1C:
		return eTDAL_FS_VOLUME_TYPE_FAT32;
	case 0x83:
		return eTDAL_FS_VOLUME_TYPE_E2FS;
	case 0x07:
		return eTDAL_FS_VOLUME_TYPE_NTFS;
	default:
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "raw_type=0x%x   format   ignored\n", raw_type ));
		return eTDAL_FS_VOLUME_TYPE_UNKNOWN;
	}
}

#if 0
LOCAL   uint8_t   pTDAL_FS_ConvertVolumeType_ToRawType(
            tTDAL_FS_VolumeType   type
            )
{
   switch(   type   )
   {
      case   eTDAL_FS_VOLUME_TYPE_E2FS:
      return   0x83;
      case   eTDAL_FS_VOLUME_TYPE_FAT32:
      return   0x0C;
      default:
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "type=0x%x   format   can't   be   converted   into   raw   type\n", type   ));
      return   0x00;
   }
}

#endif


