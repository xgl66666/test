/********************************************************************************
 *                       COPYRIGHT 2005 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME: FLASH CFG
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/flash_cfg.c $
 *            $Rev: 379 $
 *            $Date: 2011-09-16 09:28:26 +0200 (Fri, 16 Sep 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/

#include "crules.h"
#include "string.h"
#include "stdio.h"

#include "tdal_fla.h"

#include "flipflop_cfg.h"
#include "flash_cfg.h"

#if !defined(CHAL_VALIDATOR)
#include "flash_array.h"
#else
tFLASH_CFG_Location FLASH_CFG_FLASH_AreaLocationArray[30];
tFLASH_CFG_Location FLASH_CFG_Pack_AreaLocationArray[4];    
#endif

#define FLASH_CFG_BASE_ADDRESS   0x00000000
/*
#define FLASH_CFG_BASE_ADDRESS   0x40000000
*/
/* ---------------------
   FLASH PHYSICAL MEMORY MAPPING

   The physical flash memory mapping is updated by function FLASH_CFG_InitMemoryMappingStructure during the call to TDAL_FLA_INIT

*/

tFLASH_CFG_Location FLASH_CFG_FLASH_Location = {FLASH_CFG_BASE_ADDRESS, 0x00800000};

/* Currently the NVM is shared with api in dialgo area block */
#define FLASH_CFG_NVM_IN_FLASH      TRUE

/* This is the non-volatile (eeprom) memory mapping.
 *
 * This area is emulated in flash (see FLASH_CFG_NVM_IN_FLASH), and accessed through TDAL_NVM.
 *
 * TODO: add wear-levelling support to TDAL_NVM, and span data over the four sectors.
 *
 * TODO: remove the bootloader dialog sector from this mapping when implementing wear-levelling,
 * as data addresses are required to be fixed in that exchange area.
 */
tFLASH_CFG_Location FLASH_CFG_NVM_AreaLocationArray[eFLASH_CFG_NVM_NB_AREA]=
{
    /* WARNING Offset from OFFSET_NVM */
    {0x00000000, 0x00000000}, /* eFLASH_CFG_NVM_AREA_NVM_EX */
    {0x00000000, 0x00004000}, /* eFLASH_CFG_NVM_AREA_CA_1 */
    {0x00004000, 0x00008000}, /* eFLASH_CFG_NVM_AREA_CA_2 */
    {0x0000C000, 0x00000000}, /* eFLASH_CFG_NVM_AREA_CA_CSC */
    {0x0000C000, 0x00004000}, /* eFLASH_CFG_NVM_AREA_CA_STORAGE */
};

/* ---------------------
   FLASH LOGICAL MEMORY MAPPING

   The logical flash memory mapping is used to describe the flash mapping for the following goal :
    _ define the abstraction of the miroring (for usage with GENIUS)
    _ give tbe partition id to be used to get the physical flash mapping
*/


/* Partition ID for
** The order of the partition Here defines the Logical mapping
*/
const uint32_t
FLASH_CFG_PartitionId[] =
{
    eFLASH_CFG_APPLICATION_PARTITION,       /* eFLASH_CFG_FLASH_AREA_APPLICATION_PARTITION_MIRROR_x => Comedia */
    eFLASH_CFG_RESOURCES_PARTITION,           /* eFLASH_CFG_FLASH_AREA_RESOURCES_PARTITION_MIRROR_x => Partition A */
    /* reserved for linux */
    /* kFLASH_CFG_INVALID_PARTITION_ID,    eFLASH_CFG_FLASH_AREA_PARTITION_3_MIRROR_x => Not yet defined */
    /* kFLASH_CFG_INVALID_PARTITION_ID,    eFLASH_CFG_FLASH_AREA_PARTITION_4_MIRROR_x => Not yet defined*/
};
#define kFLASH_CFG_LOADABLE_PARTITION_COUNT         (sizeof(FLASH_CFG_PartitionId)/sizeof(FLASH_CFG_PartitionId[0]))

typedef struct
{
	uint32_t nPartitionId;
	uint32_t nPartitionLogicalSize;
	uint32_t nMirror1AreaId;
	uint32_t nMirror2AreaId;
}tFLASH_CFG_stMirrorMapping;

const tFLASH_CFG_stMirrorMapping FLASH_CFG_stMirrorMapping[] =
{
#if defined(TELESYSTEM_BOOTLOADER) || defined(SKYWORTH_BOOTLOADER) || defined(TELESYSTEM_MAP51_BOOTLOADER) || defined(KAON_MSD5C59_BOOTLOADER)
	{ eFLASH_CFG_APPLICATION_PARTITION, 0x150000, eFLASH_CFG_FLASH_AREA_APPLICATION_PARTITION_MIRROR_1, kFLASH_CFG_INVALID_MIRROR_ID },
#elif defined(TELESYSTEM_MAP34_BOOTLOADER)
    { eFLASH_CFG_APPLICATION_PARTITION, 0x3E0000, eFLASH_CFG_FLASH_AREA_APPLICATION_PARTITION_MIRROR_1, kFLASH_CFG_INVALID_MIRROR_ID },
#else
	{ eFLASH_CFG_APPLICATION_PARTITION, 0x120000, eFLASH_CFG_FLASH_AREA_APPLICATION_PARTITION_MIRROR_1, kFLASH_CFG_INVALID_MIRROR_ID },
#endif
	{ eFLASH_CFG_RESOURCES_PARTITION, 0x30000, eFLASH_CFG_FLASH_AREA_RESOURCES_PARTITION_MIRROR_1, kFLASH_CFG_INVALID_MIRROR_ID },
	/* Unused partitions */
	{ kFLASH_CFG_INVALID_PARTITION_ID, 0, kFLASH_CFG_INVALID_MIRROR_ID, kFLASH_CFG_INVALID_MIRROR_ID },
	{ kFLASH_CFG_INVALID_PARTITION_ID, 0, kFLASH_CFG_INVALID_MIRROR_ID, kFLASH_CFG_INVALID_MIRROR_ID },
	/* Not loadable partitions */
	{ eFLASH_CFG_USER_PARTITION, 0x30000, eFLASH_CFG_FLASH_AREA_USER_PARTITION, kFLASH_CFG_INVALID_MIRROR_ID },
	{ eFLASH_CFG_USER_CTRLB_PARTITION, 0x10000, eFLASH_CFG_FLASH_AREA_USER_PARTITION_CTRL_BLOCK, kFLASH_CFG_INVALID_MIRROR_ID },

};
#define kFLASH_CFG_PARTITION_COUNT  (sizeof(FLASH_CFG_stMirrorMapping)/sizeof(FLASH_CFG_stMirrorMapping[0]))

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
void FLASH_CFG_InitMemoryMappingStructure(void)
{
	/* Since static mapping nothing to do */
	/* It is usefull when Mapping is given by the BOOTSTRAP */
	return;
}


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
bool
FLASH_CFG_GetFlashLocation(tFLASH_CFG_Location *pLocation)
{
	bool result = FALSE;

	if (pLocation != NULL)
	{
		memcpy(pLocation,
               &FLASH_CFG_FLASH_Location,
               sizeof(tFLASH_CFG_Location));
		result = TRUE;
	}


	return result;
}

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
bool
FLASH_CFG_GetFlashAreaLocation(tFLASH_CFG_FLASH_Area area,
                               tFLASH_CFG_Location* pAreaLocation)
{
	bool result = FALSE;

	if ((pAreaLocation != NULL) && (area < eFLASH_CFG_FLASH_NB_AREA))
	{
		memcpy(pAreaLocation,
               &FLASH_CFG_FLASH_AreaLocationArray[area],
               sizeof(tFLASH_CFG_Location));
		result = TRUE;
	}
	return result;
}

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
bool
FLASH_CFG_GetNvmLocation(tFLASH_CFG_Location *pLocation)
{
	bool result = FALSE;

	if (pLocation != NULL)
	{
		if (FLASH_CFG_NVM_IN_FLASH == TRUE)
		{
			/*  search for eFLASH_CFG_FLASH_AREA_EMUL_EEPROM area */
			if ((FLASH_CFG_FLASH_AreaLocationArray[eFLASH_CFG_FLASH_AREA_EMUL_EEPROM].addressOffset != 0)
				&&(FLASH_CFG_FLASH_AreaLocationArray[eFLASH_CFG_FLASH_AREA_EMUL_EEPROM].size > 0))
			{
				pLocation->addressOffset = FLASH_CFG_FLASH_AreaLocationArray[eFLASH_CFG_FLASH_AREA_EMUL_EEPROM].addressOffset;
				pLocation->size = FLASH_CFG_FLASH_AreaLocationArray[eFLASH_CFG_FLASH_AREA_EMUL_EEPROM].size;
				result = TRUE;
			}
		}
	}
	return result;
}

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
bool
FLASH_CFG_GetNvmAreaLocation(tFLASH_CFG_NVM_Area area,
                             tFLASH_CFG_Location* pAreaLocation)
{
	bool result = FALSE;

	if ((pAreaLocation != NULL) && (area < eFLASH_CFG_NVM_NB_AREA))
	{
		memcpy(pAreaLocation,
               &FLASH_CFG_NVM_AreaLocationArray[area],
               sizeof(tFLASH_CFG_Location));
		result = TRUE;

	}
	return result;
}

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
bool FLASH_CFG_IsNVMInFlash(void)
{
	return FLASH_CFG_NVM_IN_FLASH;
}

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
bool FLASH_CFG_IsCRCFileTestedAtInit(void)
{
    return true;
}
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
GLOBAL uint32_t FLASH_CFG_GetLoadablePartitionCount(void)
{
    uint32_t count = kFLASH_CFG_LOADABLE_PARTITION_COUNT;

    while ((count>0) && (FLASH_CFG_PartitionId[count-1] == kFLASH_CFG_INVALID_PARTITION_ID))
    {
        count--; /* Not usefull to count end unused partitions */
    }
    return count;

} /* FLASH_CFG_GetLoadablePartitionCount() */

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
GLOBAL tFLASH_CFG_FLASH_Area FLASH_CFG_GetCurrentPartition(tFLASH_CFG_PartitionID nPartitionId)
{
	tFLASH_CFG_FLASH_Area nArea = kFLASH_CFG_INVALID_MIRROR_ID;
	int32_t nPartitionIndex;

	for (nPartitionIndex=0; nPartitionIndex<kFLASH_CFG_PARTITION_COUNT; nPartitionIndex++)
	{
		if (nPartitionId == FLASH_CFG_stMirrorMapping[nPartitionIndex].nPartitionId)
			break;
	}
	if (nPartitionIndex == kFLASH_CFG_PARTITION_COUNT)
	{
		/* not found */
		nArea = kFLASH_CFG_INVALID_MIRROR_ID;
	}
	else if (nPartitionIndex >= kFLASH_CFG_LOADABLE_PARTITION_COUNT)
	{
		/* not mirrored partition */
		nArea = FLASH_CFG_stMirrorMapping[nPartitionIndex].nMirror1AreaId;
	}
	else
	{
		/* select current mirror */
    switch (FLIPFLOP_GetCurrentMirrorID(nPartitionId))
    {
    	case 1:
    			nArea = FLASH_CFG_stMirrorMapping[nPartitionIndex].nMirror1AreaId;
    			break;
    	case 2:
    			nArea = FLASH_CFG_stMirrorMapping[nPartitionIndex].nMirror2AreaId;
    			break;
    	default:
				nArea = kFLASH_CFG_INVALID_MIRROR_ID;
    		break;
		}
	}
	return nArea;
}
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
GLOBAL bool FLASH_CFG_GetLoadablePartition(uint32_t uPartition, tFLASH_CFG_stLoadablePartitionInfo *pstPartitionInfo)
{
    uint32_t uLogicalOffset = 0;
    uint32_t i=0;
    uint32_t j=0;
    uint32_t nPartitionId = kFLASH_CFG_INVALID_PARTITION_ID;
    uint32_t nMirrorAreaId = kFLASH_CFG_INVALID_MIRROR_ID;
    tFLASH_CFG_Location stAreaLocation;


    if ((pstPartitionInfo == NULL) || (uPartition >= kFLASH_CFG_LOADABLE_PARTITION_COUNT))
    {
        /*** bad parameter
        printf("FLASH_CFG_GetLoadablePartition(%d, %#x)   bad parameter\n", uPartition, (uint32_t)pstPartitionInfo);
        ***/
        return FALSE;
    }
    /* Compute logical offset of the partition (the sum of size of previous partitions ) */
    for (i=0; i < uPartition; i++)
    {
    	nPartitionId = FLASH_CFG_PartitionId[i];
    	for ( j=0; j < kFLASH_CFG_LOADABLE_PARTITION_COUNT; j++ )
    	{
    		if (nPartitionId == FLASH_CFG_stMirrorMapping[j].nPartitionId)
    		{
    			uLogicalOffset += FLASH_CFG_stMirrorMapping[j].nPartitionLogicalSize;
    			break;
    		}
    	}
    }
    /* point the target partition mirror */
  	nPartitionId = FLASH_CFG_PartitionId[uPartition];
  	for ( j=0; j < kFLASH_CFG_LOADABLE_PARTITION_COUNT; j++ )
  	{
  		if (nPartitionId == FLASH_CFG_stMirrorMapping[j].nPartitionId)
  		{
		    pstPartitionInfo -> iPartitionID = nPartitionId;
		    pstPartitionInfo -> uPartitionSize = FLASH_CFG_stMirrorMapping[uPartition].nPartitionLogicalSize;
		    pstPartitionInfo -> uLogicalOffset = uLogicalOffset;
		    pstPartitionInfo -> iMirrorID = FLIPFLOP_GetLoadableMirrorID(nPartitionId);
		    switch (pstPartitionInfo -> iMirrorID)
		    {
		    	case 1:
		    			nMirrorAreaId = FLASH_CFG_stMirrorMapping[uPartition].nMirror1AreaId;
		    			break;
		    	case 2:
		    			nMirrorAreaId = FLASH_CFG_stMirrorMapping[uPartition].nMirror2AreaId;
		    			break;
				}
  		}
  	}
  	/* compute Physical offset */
		if (FLASH_CFG_GetFlashAreaLocation(nMirrorAreaId, &stAreaLocation))
		{
		    pstPartitionInfo -> uPhysicalOffset = stAreaLocation.addressOffset;
          /****
		    printf("\n**\n***FLASH_CFG_GetLoadablePartition(%d, %#x)   succeeded -> %d %#x\n",
		        uPartition, (uint32_t)pstPartitionInfo, pstPartitionInfo->iMirrorID, pstPartitionInfo->uPhysicalOffset);
         ***/

		    return TRUE;
		 }
		 return FALSE;

} /* FLASH_CFG_GetLoadablePartition() */

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
GLOBAL bool FLASH_CFG_GetLoadableAreaLocation(tFLASH_CFG_Location* pAreaLocation)
{
    int32_t i=0;
    uint32_t uAreaSize = 0;

    if (pAreaLocation == NULL)
    {
        /* bad parameter */
        return FALSE;
    }
		/* sum of all valid partition, do not take care of ID order */
    for (i=0; i < kFLASH_CFG_LOADABLE_PARTITION_COUNT; i++)
    {
        if ((FLASH_CFG_stMirrorMapping[i].nPartitionId != kFLASH_CFG_INVALID_PARTITION_ID)
        	&&(FLASH_CFG_stMirrorMapping[i].nPartitionLogicalSize > 0))
        {
            uAreaSize += FLASH_CFG_stMirrorMapping[i].nPartitionLogicalSize;
        }
    }
    if (uAreaSize > 0)
    {
    	/* By convention the logical offset is NULL */
        pAreaLocation->addressOffset = 0;
        pAreaLocation->size = uAreaSize;
        /****
        printf("[FLASH_CFG_GetLoadableAreaLocation] returns : addressOffset=%#x & uAreaSize=%d\n",
            pAreaLocation->addressOffset, pAreaLocation->size);
        ****/
        return TRUE;
    }
    return FALSE;
} /* FLASH_CFG_GetLoadableAreaLocation() */

/*===========================================================================
 *
 * FLASH_CFG_SetPartitionRight
 *
 * Parameters:
 *      iPartitionID got from FLASH_CFG_GetLoadablePartition()
 *      iMirrorID got from FLASH_CFG_GetLoadablePartition()
 *      iRight : enum { kFLASH_CFG_NONE_RIGHT, kFLASH_CFG_READONLY_RIGHT, kFLASH_CFG_READWRITE_RIGHT}
 *
 * Description:
 *
 *
 * Returns:
 *      Return TRUE if done else FALSE
 *
 *===========================================================================*/
GLOBAL bool FLASH_CFG_SetPartitionRight( int32_t iPartitionID, int32_t iMirrorID, int32_t iRight)
{
    int i;
    bool bPartitionFound = FALSE;
    uint32_t uPhysicalAddr = 0;
    uint32_t uPartitionSize = 0;
    uint8_t uTdalRights = 0;
    tFLASH_CFG_FLASH_Area nAreaId;
#if 0
    tTDAL_FLA_ErrorCode eFlaErr = eTDAL_FLA_NO_ERROR;
#endif

    if (iPartitionID == kFLASH_CFG_INVALID_PARTITION_ID)
  	{
  		return FALSE;
  	}

    for (i=0; i < kFLASH_CFG_LOADABLE_PARTITION_COUNT; i++)
    {
        if (FLASH_CFG_stMirrorMapping[i].nPartitionId == iPartitionID)
        {
            bPartitionFound = TRUE;
            break;
         }
    }
    if (bPartitionFound == FALSE)
    {
        return FALSE;
    }
    switch(iMirrorID)
    {
        case 1:
            nAreaId = FLASH_CFG_stMirrorMapping[i].nMirror1AreaId;
            break;
        case 2:
            nAreaId = FLASH_CFG_stMirrorMapping[i].nMirror2AreaId;
            break;
        default:
            return FALSE;
            break;
    }
		uPhysicalAddr = FLASH_CFG_FLASH_AreaLocationArray[nAreaId].addressOffset;
		uPartitionSize = FLASH_CFG_FLASH_AreaLocationArray[nAreaId].size;
    switch (iRight)
    {
        case kFLASH_CFG_NONE_RIGHT:
            uTdalRights = 0;
            break;
        case kFLASH_CFG_READONLY_RIGHT:
            uTdalRights = TDAL_FLA_READ_ENABLE;
            break;
        case kFLASH_CFG_READWRITE_RIGHT:
            uTdalRights = TDAL_FLA_READ_ENABLE | TDAL_FLA_WRITE_ENABLE;
            break;
        default:
            return FALSE;
            break;
    }
#if 0
    eFlaErr = TDAL_FLA_ChangePartitionRight (uPhysicalAddr,uPartitionSize, uTdalRights);
    if(eFlaErr != eTDAL_FLA_NO_ERROR)
    {
       /****
        printf("[FLASH_CFG_SetPartitionRight] TDAL_FLA_ChangePartitionRight(%#x,%#x,%#x) return an error\n",
                uPhysicalAddr, uPartitionSize, uTdalRights);
      ****/
        return (FALSE);
    }
#else
    return TRUE;
#endif
} /* FLASH_CFG_SetPartitionRight() */

/*===========================================================================
 *
 * FLASH_CFG_SetUpgradedPartition
 *
 * Parameters:
 *      iPartitionID got from FLASH_CFG_GetLoadablePartition()
 *      iMirrorID got from FLASH_CFG_GetLoadablePartition()
 *      uUpdateId : got from Download process
 *
 * Description: set the upgrade ID for a mirror partition new loaded
 *
 *
 * Returns:
 *      Return TRUE if done else FALSE
 *
 *===========================================================================*/
GLOBAL bool FLASH_CFG_SetUpgradedPartition(int32_t iPartitionID,
                                           int32_t iMirrorID,
                                           uint32_t uUpdateId)
{
	#if 0
    gmeOAD_Err_t eOadErr = OAD_ERR_SUCCESS;
    gmtOAD_FlipFlopInfo_t stFlipFlopInfo;

    /****
    printf("[FLASH_CFG_SetUpgradedPartition] iPartitionID=%d iMirrorID=%d uUpdateId=0x%x\n",
           iPartitionID,
           iMirrorID,
           uUpdateId);
   ***/

    eOadErr = gmOAD_GetFlipFlopInfo(iPartitionID, &stFlipFlopInfo );
    if (eOadErr != OAD_ERR_SUCCESS)
    {
        return FALSE;
    }
    switch (iMirrorID)
    {
        case 2:
            stFlipFlopInfo.Partition_2_UpgradeId = uUpdateId;
            stFlipFlopInfo.Partition_2_rejected_by_system = 0;
            break;

        case 1:
                stFlipFlopInfo.Partition_1_UpgradeId = uUpdateId;
                stFlipFlopInfo.Partition_1_rejected_by_system = 0;
                break;
    default:

                return FALSE;
                break;
    } /* switch (iMirrorID) */


    eOadErr = gmOAD_SetFlipFlopInfo(iPartitionID,
                                    stFlipFlopInfo );

    if (eOadErr != OAD_ERR_SUCCESS)
    {
        return FALSE;
    }
    return TRUE;
    #else
    /* TO DO */
    return FALSE;
    #endif
}   /* FLASH_CFG_SetUpgradedPartition() */


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
GLOBAL bool  FLASH_CFG_IsUpdateRejected(uint32_t *puUpdateId)
{
	/* BOOTSTRAO is not designed for refusing doxnload */
    return FALSE; /* Not rejected */
} /* FLASH_CFG_IsUpdateRejected() */

/*===========================================================================
 *
 * FLASH_CFG_InitFlaPartitionRight
 *
 * Parameters:
 *
 * Description: According to flip flop status gives the standard partition right
 *
 *
 * Returns:
 *
 *===========================================================================*/
bool FLASH_CFG_InitFlaPartitionRight(void)
{


    tTDAL_FLA_Partition FLAPartition[eFLASH_CFG_FLASH_NB_AREA];
    uint16_t	        nbPartition;
    tFLASH_CFG_FLASH_Area nCurrentAreaId = kFLASH_CFG_INVALID_MIRROR_ID;
    tFLASH_CFG_FLASH_Area nOtherAreaId = kFLASH_CFG_INVALID_MIRROR_ID;
    int32_t i = 0;
    tFLASH_CFG_Location stLocationTemp;

    nbPartition = 0;

    /* init not loadable readonly  partitions */

    for (i=0; i<0; i++)
    {
        FLAPartition[nbPartition].StartAddress = 0;
        FLAPartition[nbPartition].EndAddress = 0;
        FLAPartition[nbPartition].Rights = TDAL_FLA_READ_ENABLE;
        nbPartition++;
    }
    /* init not loadable read/write partitions */
    for (i=0; i<0; i++)
    {
        FLAPartition[nbPartition].StartAddress = 0;
        FLAPartition[nbPartition].EndAddress = 0;
        FLAPartition[nbPartition].Rights = TDAL_FLA_READ_ENABLE | TDAL_FLA_WRITE_ENABLE;
        nbPartition++;
    }

     /* init loadable partitions */
    for (i=0; i<kFLASH_CFG_LOADABLE_PARTITION_COUNT; i++)
    {
    	if (FLASH_CFG_stMirrorMapping[i].nPartitionId != kFLASH_CFG_INVALID_PARTITION_ID)
    	{
    		nCurrentAreaId = kFLASH_CFG_INVALID_MIRROR_ID;
    		nOtherAreaId = kFLASH_CFG_INVALID_MIRROR_ID;
    		switch( FLIPFLOP_GetCurrentMirrorID(FLASH_CFG_stMirrorMapping[i].nPartitionId))
    		{
    			case 1 :
    				nCurrentAreaId = FLASH_CFG_stMirrorMapping[i].nMirror1AreaId;
    				nOtherAreaId = FLASH_CFG_stMirrorMapping[i].nMirror2AreaId;
    				break;
    			case 2 :
    				nCurrentAreaId = FLASH_CFG_stMirrorMapping[i].nMirror2AreaId;
    				nOtherAreaId = FLASH_CFG_stMirrorMapping[i].nMirror1AreaId;
    				break;
    		}
    			/* current mirror readable */
    		if (nCurrentAreaId != kFLASH_CFG_INVALID_MIRROR_ID)
    		{
		        FLAPartition[nbPartition].StartAddress = FLASH_CFG_FLASH_AreaLocationArray[nCurrentAreaId].addressOffset;

				if ( FLASH_CFG_FLASH_AreaLocationArray[nCurrentAreaId].size != 0)
				{
			        FLAPartition[nbPartition].EndAddress =
			        		FLASH_CFG_FLASH_AreaLocationArray[nCurrentAreaId].addressOffset
			        		+ FLASH_CFG_FLASH_AreaLocationArray[nCurrentAreaId].size -1;
				}
				else
				{
					FLAPartition[nbPartition].EndAddress = FLAPartition[nbPartition].StartAddress;
				}
		        FLAPartition[nbPartition].Rights = TDAL_FLA_READ_ENABLE;
		        nbPartition++;
    		}
	        /* other mirror no right */
    		if (nOtherAreaId != kFLASH_CFG_INVALID_MIRROR_ID)
    		{
		        FLAPartition[nbPartition].StartAddress = FLASH_CFG_FLASH_AreaLocationArray[nOtherAreaId].addressOffset;
				if ( FLASH_CFG_FLASH_AreaLocationArray[nOtherAreaId].size != 0)
				{
			        FLAPartition[nbPartition].EndAddress =
			        		FLASH_CFG_FLASH_AreaLocationArray[nOtherAreaId].addressOffset
			        		+ FLASH_CFG_FLASH_AreaLocationArray[nOtherAreaId].size -1;
				}
				else
				{
					FLAPartition[nbPartition].EndAddress = FLAPartition[nbPartition].StartAddress;
				}
		        /* @@@ Other miror Read & write Rights, to not have to change dynamicaly later */
		        FLAPartition[nbPartition].Rights = TDAL_FLA_READ_ENABLE | TDAL_FLA_WRITE_ENABLE;
		        nbPartition++;
    		}
    	}
    }

    /* Channel database partition */
    FLASH_CFG_GetFlashAreaLocation(eFLASH_CFG_FLASH_AREA_USER_PARTITION,
                                   &stLocationTemp);
    if (stLocationTemp.size != 0)
    {
        FLAPartition[nbPartition].StartAddress = stLocationTemp.addressOffset;
		if ( stLocationTemp.size != 0 )
		{
	        FLAPartition[nbPartition].EndAddress = stLocationTemp.addressOffset + stLocationTemp.size - 1;
		}
		else
		{
			FLAPartition[nbPartition].EndAddress = stLocationTemp.addressOffset;
		}

        /* Read/Write Enable on Channel Database partition  */
        FLAPartition[nbPartition].Rights = TDAL_FLA_READ_ENABLE | TDAL_FLA_WRITE_ENABLE;

        nbPartition++;
    }

    FLASH_CFG_GetFlashAreaLocation(eFLASH_CFG_FLASH_AREA_USER_PARTITION_CTRL_BLOCK,
                                   &stLocationTemp);
    if (stLocationTemp.size != 0)
    {
        FLAPartition[nbPartition].StartAddress = stLocationTemp.addressOffset;
		if ( stLocationTemp.size != 0 )
		{
	        FLAPartition[nbPartition].EndAddress = stLocationTemp.addressOffset + stLocationTemp.size - 1;
		}
		else
		{
			FLAPartition[nbPartition].EndAddress = stLocationTemp.addressOffset;
		}
        /* Read/Write Enable on Channel Database partition Ctrl Blck */
        FLAPartition[nbPartition].Rights = TDAL_FLA_READ_ENABLE | TDAL_FLA_WRITE_ENABLE;
        nbPartition++;
    }

    /* NVM */
    FLASH_CFG_GetNvmLocation(&stLocationTemp);
    if (FLASH_CFG_IsNVMInFlash() == TRUE)
    {
        FLAPartition[nbPartition].StartAddress = stLocationTemp.addressOffset;
		if ( stLocationTemp.size != 0 )
		{
	        FLAPartition[nbPartition].EndAddress = stLocationTemp.addressOffset + stLocationTemp.size - 1;
		}
		else
		{
			FLAPartition[nbPartition].EndAddress = stLocationTemp.addressOffset;
		}
        FLAPartition[nbPartition].Rights = TDAL_FLA_READ_ENABLE | TDAL_FLA_WRITE_ENABLE;
        nbPartition++;
    }

    if(TDAL_FLA_SetPartition (nbPartition,
                              FLAPartition) != eTDAL_FLA_NO_ERROR)
    {
       /****
        printf("[FLASH_CFG_InitFlaPartitionRight] TDAL_FLA_SetPartition(-) return an error\n");
        ***/
        return (false);
    }

    return(true);
} /* FLASH_CFG_InitFlaPartitionRight() */


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
bool FLASH_CFG_SwitchUpgradedPartitions(uint32_t uNewUpdateID)
{
    return FALSE;
} /*        FLASH_CFG_SwitchUpgradedPartitions() */


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
 *===========================================================================
bool FLASH_CFG_GetCurrentUpgradeId(uint32_t *puCurrentUpdateID)*/

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
#define PARTITION_NAME(NAME)     case     eFLASH_CFG_FLASH_AREA_##NAME : pName=#NAME ; break;
unsigned char * FLASH_CFG_GetPartitionName(tFLASH_CFG_FLASH_Area nArea)
{
	uint16_t i;
	int16_t nPartitionId = kFLASH_CFG_INVALID_PARTITION_ID;
	char *pName = "Unused partition";
	static char szPartitionName[256];

	/* search partition ID */
	for (i=0; i<kFLASH_CFG_PARTITION_COUNT; i++)
	{
		if ((nArea == FLASH_CFG_stMirrorMapping[i].nMirror1AreaId)
			||(nArea == FLASH_CFG_stMirrorMapping[i].nMirror2AreaId))
		{
			nPartitionId = FLASH_CFG_stMirrorMapping[i].nPartitionId;
			break;
		}
	}
	/* ID 's name */
	switch (nArea)
	{
        PARTITION_NAME(BOOTSTRAP)
        PARTITION_NAME(BOOTLOADER)
        PARTITION_NAME(APPLICATION_PARTITION_MIRROR_1)
        PARTITION_NAME(APPLICATION_PARTITION_MIRROR_2)
        PARTITION_NAME(RESOURCES_PARTITION_MIRROR_1)
        PARTITION_NAME(RESOURCES_PARTITION_MIRROR_2)
        PARTITION_NAME(PARTITION_3_MIRROR_1)
        PARTITION_NAME(PARTITION_3_MIRROR_2)
        PARTITION_NAME(PARTITION_4_MIRROR_1)
        PARTITION_NAME(PARTITION_4_MIRROR_2)
        PARTITION_NAME(PARTITION_5_MIRROR_1)
        PARTITION_NAME(PARTITION_5_MIRROR_2)
        PARTITION_NAME(PARTITION_6_MIRROR_1)
        PARTITION_NAME(PARTITION_6_MIRROR_2)
        PARTITION_NAME(PARTITION_7_MIRROR_1)
        PARTITION_NAME(PARTITION_7_MIRROR_2)
        PARTITION_NAME(PARTITION_8_MIRROR_1)
        PARTITION_NAME(PARTITION_8_MIRROR_2)
        PARTITION_NAME(USER_PARTITION)
        PARTITION_NAME(USER_PARTITION_CTRL_BLOCK)
#ifdef CAK_DLK
        PARTITION_NAME(DLK_PARTITION)
        PARTITION_NAME(DLK_PARTITION_CTRL_BLOCK)
#endif
        PARTITION_NAME(EMUL_EEPROM)
        case eFLASH_CFG_FLASH_NB_AREA:
        default:
           /****
            printf("FLASH_CFG_GetPartitionName bad parameter (%d)\n", nArea);
            ****/
            break;
    } /* switch */
    /* usual names
	switch (nArea)
	{
        PARTITION_NAME(COMEDIA)
        PARTITION_NAME(PARTITION_A)
        PARTITION_NAME(PARTITION_B)
        PARTITION_NAME(PARTITION_B_CTRLBLOCK)
      }   */

  if (nPartitionId == kFLASH_CFG_INVALID_PARTITION_ID)
  {
  	sprintf(szPartitionName, "%s", pName);
  }
  else
  {
  	sprintf(szPartitionName, "%s (0x%04x)", pName, nPartitionId);
  }
  return (unsigned char*)szPartitionName;
}


#if defined(NEWLAND_BOOTLOADER) || defined(CHANGHONG_BOOTLOADER) || defined(SKYWORTH_BOOTLOADER) || defined(TELESYSTEM_BOOTLOADER) || defined(TELESYSTEM_MAP51_BOOTLOADER) || defined(TELESYSTEM_MAP34_BOOTLOADER) || defined(TECHNICOLOR_BOOTLOADER) || defined(KAON_MSD5C59_BOOTLOADER)
/*===========================================================================
 *
 * FLASH_CFG_GePackingAreaLocation
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
bool
FLASH_CFG_GetPackingAreaLocation(tFLASH_CFG_Pack_Area area,
                             tFLASH_CFG_Location* pAreaLocation)
{
	bool result = FALSE;

	if ((pAreaLocation != NULL) && (area < eFLASH_CFG_PACK_NB_AREA))
	{
		memcpy(pAreaLocation,
               &FLASH_CFG_Pack_AreaLocationArray[area],
               sizeof(tFLASH_CFG_Location));
		result = TRUE;
	}
	return result;
}

#endif


