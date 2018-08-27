/******************************************************************************
 *                           COPYRIGHT 2005 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: CFG FLASH/NVM
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/flash_cfg.h $
 *            $Rev: 332 $
 *            $Date: 2011-09-07 13:51:23 +0200 (Wed, 07 Sep 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _FLASH_CFG_H_
#define _FLASH_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif





/*******************************************************/
/*               typedef                               */
/*******************************************************/
typedef enum
{
   eFLASH_CFG_FLASH_AREA_BOOTSTRAP,  /* a hardware  protected area */
	eFLASH_CFG_FLASH_AREA_BOOTLOADER,
	eFLASH_CFG_FLASH_AREA_PERSONALIZATION,  /* Personalization area */
	eFLASH_CFG_FLASH_AREA_PLS_DOWNLOADER, 	/* PLS downloader */
	eFLASH_CFG_FLASH_AREA_REGULAR_DA2, 		/* DA2 & Config File regular same sector*/	
	eFLASH_CFG_FLASH_AREA_REGULAR_CFG, 		
	eFLASH_CFG_FLASH_AREA_REDUNDANT_CFG, 	/* DA2 & Config File redundant */
   eFLASH_CFG_FLASH_AREA_APPLICATION_PARTITION_MIRROR_1,                      
   eFLASH_CFG_FLASH_AREA_APPLICATION_PARTITION_MIRROR_2,            
   eFLASH_CFG_FLASH_AREA_RESOURCES_PARTITION_MIRROR_1,             
   eFLASH_CFG_FLASH_AREA_RESOURCES_PARTITION_MIRROR_2,
   eFLASH_CFG_FLASH_AREA_PARTITION_3_MIRROR_1,
   eFLASH_CFG_FLASH_AREA_PARTITION_3_MIRROR_2,
   eFLASH_CFG_FLASH_AREA_PARTITION_4_MIRROR_1,
   eFLASH_CFG_FLASH_AREA_PARTITION_4_MIRROR_2,
   eFLASH_CFG_FLASH_AREA_PARTITION_5_MIRROR_1,                      
   eFLASH_CFG_FLASH_AREA_PARTITION_5_MIRROR_2,            
   eFLASH_CFG_FLASH_AREA_PARTITION_6_MIRROR_1,             
   eFLASH_CFG_FLASH_AREA_PARTITION_6_MIRROR_2,
   eFLASH_CFG_FLASH_AREA_PARTITION_7_MIRROR_1,
   eFLASH_CFG_FLASH_AREA_PARTITION_7_MIRROR_2,
   eFLASH_CFG_FLASH_AREA_PARTITION_8_MIRROR_1,
   eFLASH_CFG_FLASH_AREA_PARTITION_8_MIRROR_2,
   eFLASH_CFG_FLASH_AREA_USER_PARTITION,           
   eFLASH_CFG_FLASH_AREA_USER_PARTITION_CTRL_BLOCK,    
#ifdef CAK_DLK
   eFLASH_CFG_FLASH_AREA_DLK_PARTITION,           
   eFLASH_CFG_FLASH_AREA_DLK_PARTITION_CTRL_BLOCK,    
#endif
   eFLASH_CFG_FLASH_AREA_EXTERN_PARTITION,           
   eFLASH_CFG_FLASH_AREA_EXTERN_PARTITION_CTRL_BLOCK,   
   eFLASH_CFG_FLASH_AREA_EMUL_EEPROM,    
   eFLASH_CFG_FLASH_AREA_BURN_REPORT,    
   eFLASH_CFG_FLASH_AREA_BOOTSTRAP_DATA,    
   eFLASH_CFG_FLASH_NB_AREA
}tFLASH_CFG_FLASH_Area;
/* defines for ascendant compatibility */
#define kFLASH_CFG_MIRRORING_IMPLEMENTED	1
#define eFLASH_CFG_FLASH_AREA_COMEDIA 						eFLASH_CFG_FLASH_AREA_APPLICATION_PARTITION_MIRROR_1
#define eFLASH_CFG_FLASH_AREA_PARTITION_A 				eFLASH_CFG_FLASH_AREA_RESOURCES_PARTITION_MIRROR_1
#define eFLASH_CFG_FLASH_AREA_PARTITION_B 				eFLASH_CFG_FLASH_AREA_USER_PARTITION
#define eFLASH_CFG_FLASH_AREA_PARTITION_B_CTRLBLOCK 	eFLASH_CFG_FLASH_AREA_USER_PARTITION_CTRL_BLOCK
#define eFLASH_CFG_FLASH_AREA_PARTITION_C 				eFLASH_CFG_FLASH_AREA_EXTERN_PARTITION
#define eFLASH_CFG_FLASH_AREA_PARTITION_C_CTRLBLOCK 	eFLASH_CFG_FLASH_AREA_EXTERN_PARTITION_CTRL_BLOCK
#define eFLASH_CFG_FLASH_AREA_BOOT_CODE_DIALOG 			eFLASH_CFG_FLASH_AREA_BOOTSTRAP_DATA
#ifdef CAK_DLK
/**
 *  Parition N (DLK via VFS):
 *  - If partition N is a real partition, define the next define
 *  - If partition N is an Alias on the parition B do not define the next define
 */ 
#define eFLASH_CFG_FLASH_AREA_PARTITION_N 				eFLASH_CFG_FLASH_AREA_DLK_PARTITION
#define eFLASH_CFG_FLASH_AREA_PARTITION_N_CTRLBLOCK 	eFLASH_CFG_FLASH_AREA_DLK_PARTITION_CTRL_BLOCK

#endif
#define eFLASH_CFG_FLASH_AREA_UNDEFINED     				0xFFFFFFFF

/* defines for integrated loader */
typedef enum
{
   eFLASH_CFG_RESOURCES_PARTITION	= 0xA0, /* FSA*/
   eFLASH_CFG_USER_PARTITION			= 0xB0, /* FSB */
   eFLASH_CFG_USER_CTRLB_PARTITION	= 0xB1, 
   eFLASH_CFG_APPLICATION_PARTITION	= 0xC0, /* Comedia */
   eFLASH_CFG_OS_DRIVER_PARTITION	= 0xD0,
   eFLASH_CFG_OS_PARTITION				= 0xE0
} tFLASH_CFG_PartitionID;

typedef enum
{
   eFLASH_CFG_NVM_AREA_NVM_EX,      /* Located at the end of the dialog area block */
   eFLASH_CFG_NVM_AREA_CA_1,
   eFLASH_CFG_NVM_AREA_CA_2,
   eFLASH_CFG_NVM_AREA_CA_CSC,
   eFLASH_CFG_NVM_AREA_CA_STORAGE,
#ifdef CAK_DLK
   eFLASH_CFG_NVM_AREA_EXCLUDED_AREA, /* To specify the NVM unused area but used by another feature */
#endif
   eFLASH_CFG_NVM_NB_AREA
}tFLASH_CFG_NVM_Area;

typedef struct
{
	uint32_t addressOffset;
	uint32_t size;
}tFLASH_CFG_Location;


#define kFLASH_CFG_INVALID_PARTITION_ID         (-1)
#define kFLASH_CFG_INVALID_MIRROR_ID            (-1)
#define kFLASH_CFG_INVALID_UPDATE_ID            (-1)

enum {kFLASH_CFG_NONE_RIGHT, 
      kFLASH_CFG_READONLY_RIGHT, 
      kFLASH_CFG_READWRITE_RIGHT};

typedef struct 
{
	int32_t iPartitionID;
	uint32_t uPartitionSize;
	uint32_t uLogicalOffset;
	int32_t iMirrorID;
	uint32_t uPhysicalOffset;
} tFLASH_CFG_stLoadablePartitionInfo;

#if defined(NEWLAND_BOOTLOADER) || defined(CHANGHONG_BOOTLOADER) || defined(SKYWORTH_BOOTLOADER) || defined(TELESYSTEM_BOOTLOADER) || defined(TELESYSTEM_MAP51_BOOTLOADER) || defined(TELESYSTEM_MAP34_BOOTLOADER) || (TECHNICOLOR_BOOTLOADER)

typedef enum
{
    eFLASH_CFG_PACK_PART_APPLICATION_AREA,
   eFLASH_CFG_PACK_PART_A_AREA,
   eFLASH_CFG_PACK_PART_C_CTRL_AREA,
   eFLASH_CFG_PACK_PART_C_DATA_AREA,
   eFLASH_CFG_PACK_NB_AREA
} tFLASH_CFG_Pack_Area;

#endif

/*===========================================================================
 *
 * FLASH_CFG_GetLoadablePartitionCount
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the count of loadable partition
 *
 * Returns:
 *      Return 
 *
 *===========================================================================*/
GLOBAL uint32_t FLASH_CFG_GetLoadablePartitionCount(void);
/*===========================================================================
 *
 * FLASH_CFG_GetCurrentPartition
 *
 * Parameters:
 *      nPartitionId 
 *
 * Description:
 *      fill in the description of a loadable partition
 *
 * Returns:
 *      nAreaId
 *
 *===========================================================================*/
GLOBAL tFLASH_CFG_FLASH_Area FLASH_CFG_GetCurrentPartition(tFLASH_CFG_PartitionID nPartitionId);
/*===========================================================================
 *
 * FLASH_CFG_GetLoadablePartition
 *
 * Parameters:
 *      uPartition (from 0 to FLASH_CFG_GetLoadablePartitionCount-1)
 *
 * Description:
 *      fill in the description of a loadable partition
 *
 * Returns:
 *      Return TRUE for known partition else FALSE
 *
 *===========================================================================*/
GLOBAL bool FLASH_CFG_GetLoadablePartition(uint32_t uPartition, tFLASH_CFG_stLoadablePartitionInfo *pstPartitionInfo);
/*===========================================================================
 *
 * FLASH_CFG_GetLoadableAreaLocation
 *
 * Parameters:
 *      out *pAreaLocation
 *
 * Description:
 *      fill in the description of the loadable area
 *
 * Returns:
 *      Return TRUE for known partition else FALSE
 *
 *===========================================================================*/
GLOBAL bool FLASH_CFG_GetLoadableAreaLocation(tFLASH_CFG_Location* pAreaLocation);
/*===========================================================================
 *
 * FLASH_CFG_SetPartitionRight
 *
 * Parameters:
 *      uPartition (from 0 to FLASH_CFG_GetLoadablePartitionCount-1)
 *
 * Description:
 *      fill in the description of a loadable partition
 *
 * Returns:
 *      Return TRUE for known partition else FALSE
 *
 *===========================================================================*/
GLOBAL bool FLASH_CFG_SetPartitionRight( int32_t iPartitionID, int32_t iMirrorID, int32_t iRight);
/*===========================================================================
 *
 * FLASH_CFG_IsUpdateRejected
 *
 * Parameters:
 *      *puUpdateId IN/OUT
 *
 * Description: if updateId rejected return another one previoulsly loaded
 *     
 *
 * Returns:
 *      Return TRUE if rejected else FALSE
 *
 *===========================================================================*/
GLOBAL bool  FLASH_CFG_IsUpdateRejected(uint32_t *puUpdateId);

extern tFLASH_CFG_Location FLASH_CFG_FLASH_AreaLocationArray[];

/*===========================================================================
 *
 * FLASH_CFG_GetFlashLocation
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return 
 *
 * Returns:
 *      Return 
 *
 *===========================================================================*/
bool FLASH_CFG_GetFlashLocation(tFLASH_CFG_Location *pLocation);

/*===========================================================================
 *
 * FLASH_CFG_GetFlashAreaLocation
 *
 * Parameters:
 *      IN : Area.
 *		OUT: Area Location.
  * Description:
 *
 * Returns:
 *      Return TRUE if succeed, else FALSE
 *
 *===========================================================================*/
bool FLASH_CFG_GetFlashAreaLocation(tFLASH_CFG_FLASH_Area area, 
                                    tFLASH_CFG_Location* pAreaLocation);


/*===========================================================================
 *
 * FLASH_CFG_GetNvmLocation
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return 
 *
 * Returns:
 *      Return 
 *
 *===========================================================================*/
bool FLASH_CFG_GetNvmLocation(tFLASH_CFG_Location *pLocation);


/*===========================================================================
 *
 * FLASH_CFG_GetNvmAreaLocation
 *
 * Parameters:
 *      IN : Area.
 *		OUT: Area Location.
  * Description:
 *
 * Returns:
 *      Return TRUE if succeed, else FALSE
 *
 *===========================================================================*/
bool FLASH_CFG_GetNvmAreaLocation(tFLASH_CFG_NVM_Area area, 
                                  tFLASH_CFG_Location* pAreaLocation);

/*===========================================================================
 *
 * FLASH_CFG_IsNVMInFlash
 *
 * Parameters:
 *      None
 *		
  * Description:
 *
 * Returns:
 *      Return TRUE if NVM is simulated in Flash, else FALSE
 *
 *===========================================================================*/
bool FLASH_CFG_IsNVMInFlash(void);


#ifndef _USE_DATAMNGR_JFS_
/*===========================================================================
 *
 * FLASH_CFG_IsCRCFileTestedAtInit
 *
 * Parameters:
 *      None
 *		
  * Description:
 *
 * Returns:
 *      Return TRUE if CRC file must be tested at init time
 *
 *===========================================================================*/
bool FLASH_CFG_IsCRCFileTestedAtInit(void);
#endif


/*===========================================================================
 *
 * FLASH_CFG_InitFlaPartitionRight
 *
 * Parameters:
 *		
 * Description:
 *
 * Returns:
 *
 *===========================================================================*/
bool FLASH_CFG_InitFlaPartitionRight(void);

/*===========================================================================
 *
 * FLASH_CFG_InitMemoryMappingStructure
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return 
 *
 * Returns:
 *      Return 
 *
 *===========================================================================*/
void FLASH_CFG_InitMemoryMappingStructure(void);

/*===========================================================================
 *
 * FLASH_CFG_SwitchUpgradedPartitions
 *
 * Parameters: uNewUpdateID the upgrade ID to switch to
 *		
 * Description:
 *
 * Returns:
 *
 *===========================================================================*/
bool FLASH_CFG_SwitchUpgradedPartitions(uint32_t uNewUpdateID);

/*===========================================================================
 *
 * FLASH_CFG_GetCurrentUpgradeId
 *
 * Parameters: compute the higher updateId of a partition current mirror 
 *		
 * Description:
 *
 * Returns:
 *
 *===========================================================================*/
bool FLASH_CFG_GetCurrentUpgradeId(uint32_t *puCurrentUpdateID);

/*===========================================================================
 *
 * FLASH_CFG_GetPartitionName
 *
 * Parameters:  
 *		
 * Description:
 *
 * Returns:
 *
 *===========================================================================*/
unsigned char * FLASH_CFG_GetPartitionName(tFLASH_CFG_FLASH_Area nArea);


#ifdef NEWLAND_BOOTLOADER
bool
FLASH_CFG_GetPackingAreaLocation(tFLASH_CFG_Pack_Area area,
                             tFLASH_CFG_Location* pAreaLocation);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _FLASH_CFG_H_ */
