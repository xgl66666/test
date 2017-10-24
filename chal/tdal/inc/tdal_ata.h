/******************************************************************************
 *            COPYRIGHT 2001 IWEDIA TECHNOLOGIES        *
 ******************************************************************************
 *
 * MODULE NAME: TDAL ATA
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_ata.h $
 *      $Rev: 66 $
 *      $Date: 2008-10-14 10:29:19 +0200 (mar., 14 oct. 2008) $
 *
 * PUBLIC
 *
 * DESCRIPTION - Public header for ATA driver
 *
 *****************************************************************************/

#ifndef _TDAL_ATA_H_
#define _TDAL_ATA_H_

/*******************************************************
 *       Includes              *
 *******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************
 *       Defines              *
 *******************************************************/

/*******************************************************
 *       Macros               *
 *******************************************************/


/*******************************************************
 *       Typedefs              *
 *******************************************************/

/* Return codes */
typedef enum TDAL_ATA_Error_e
{
   eTDAL_ATA_NO_ERROR,      /* there is no error */
   eTDAL_ATA_ERROR_WITH_DISK,   /* an error happened with the disk
                 or the native driver */
   eTDAL_ATA_ERROR_NO_DISK,    /* there is no disk */
   eTDAL_ATA_ERROR_BAD_PARAM,   /* a parameter passed to a function
                 was erroneous */
   eTDAL_ATA_ERROR       /* TDAL_ATA driver is not initialized
                 or the device is not opened */
} tTDAL_ATA_Error;

/* Device Identifier */
typedef  uint8_t   tTDAL_ATA_Device;

/* Transfer Modes */
typedef enum TDAL_ATA_TransferMode_e
{
   eTDAL_ATA_TRANSFER_INVALID = 0,
   eTDAL_ATA_TRANSFER_PIO   = 1,
   eTDAL_ATA_TRANSFER_DMA   = 2
} tTDAL_ATA_TransferMode;

/* Device Information */
typedef struct TDAL_ATA_DeviceInfo_s
{
   tTDAL_ATA_Device   device;         /* device identifier */
   char *       device_name;       /* human device name */
   uint32_t      supported_transfer_modes;   /* a mask value of ... */
} tTDAL_ATA_DeviceInfo;

/* Capabilities */
typedef struct TDAL_ATA_Capabilities_s
{
   uint8_t        nb_devices;
   tTDAL_ATA_DeviceInfo *  devices_array;
} tTDAL_ATA_Capabilities;



/*******************************************************
 *       Functions Declarations        *
 *******************************************************/

tTDAL_ATA_Error TDAL_ATA_Init( void );

tTDAL_ATA_Error TDAL_ATA_GetCapabilities(
   tTDAL_ATA_Capabilities * p_capabilities
);

tTDAL_ATA_Error TDAL_ATA_Open(
   tTDAL_ATA_Device    device,
   tTDAL_ATA_TransferMode  transfer
);

tTDAL_ATA_Error TDAL_ATA_IdentifyDevice(
   tTDAL_ATA_Device    device,
   uint16_t *       buffer
);

tTDAL_ATA_Error TDAL_ATA_Read(
   tTDAL_ATA_Device    device,
   uint32_t       lba,
   uint32_t       sector_count,
   uint8_t *       buffer
);

tTDAL_ATA_Error TDAL_ATA_Write(
   tTDAL_ATA_Device    device,
   uint32_t       lba,
   uint32_t       sector_count,
   uint8_t *       buffer
);

tTDAL_ATA_Error TDAL_ATA_Close(
   tTDAL_ATA_Device    device
);

tTDAL_ATA_Error TDAL_ATA_Term( void );


#ifdef __cplusplus
}
#endif


#endif /*_TDAL_ATA_H_*/
