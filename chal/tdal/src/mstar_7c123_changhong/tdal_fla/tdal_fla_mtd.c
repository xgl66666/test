/*****************************************************************************
 *                  COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_FLA
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_fla/tdal_fla_mtd.c $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description : Flash driver TDAL library implementation.
 *
 ****************************************************************************/


/****************************************************************************
 *  INCLUDES                                           *
 ****************************************************************************/

/* Generic Headers */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define __user
#include <mtd/mtd-user.h>

#include "crules.h"


/*   Include   TDAL/TBOX   header   file   */
#include   "tbox.h"
#if   0
#define   kTBOX_CRITICAL_ENABLED
#define   kTBOX_WARNING_ENABLED
#include   "tbox_debug.h"
#endif

#include   "tdal_common.h"
#include   "tdal_fla.h"

/*   Include   configuration   file   */
#include   "flash_cfg.h"

/****************************************************************************
 *  DEFINES                                            *
 ****************************************************************************/
#define   kTDAL_FLA_ERROR_WHEN_NO_RIGHT      eTDAL_FLA_NOT_DONE
#define   kTDAL_FLA_BLOCK_SIZE        0x20000   /*   128   kBytes   */
#define   kTDAL_FLA_MIN_ACCESS_WIDTH      2

/****************************************************************************
 *  MACROS                                              *
 ****************************************************************************/
mTBOX_SET_MODULE(eTDAL_FLA);


#define   mLockAccess(x)   TDAL_LockMutex(x)
#define   mUnLockAccess(x)   TDAL_UnlockMutex(x)

/****************************************************************************
 *  TYPEDEFS                                           *
 ****************************************************************************/
typedef   struct
{   
   uint32_t   addressOffset;
   uint32_t   size;
   uint32_t   mtdHandle;
}tTDAL_FLA_PartitionInfo;

/****************************************************************************
 *  GLOBAL VARIABLES (GLOBAL/IMPORT)                            *
 ****************************************************************************/

/****************************************************************************
 *  LOCAL MODULE VARIABLES (MODULE/IMPORT)                             *
 ****************************************************************************/

/****************************************************************************
 *  LOCAL FILE VARIABLES (LOCAL)                               *
 ****************************************************************************/
LOCAL   bool            TDAL_FLAi_bAlreadyInitialized = false;
LOCAL   tTDAL_FLA_Partition   *      TDAL_FLAi_pstPartitions       = NULL;
LOCAL   uint8_t            TDAL_FLAi_ucNbPartition       = 0;
LOCAL   bool            TDAL_FLAi_bIsPartitionSet   = false;
LOCAL   uint8_t            TDAL_FLAi_pucLayerBuffer[kTDAL_FLA_BLOCK_SIZE];
LOCAL   uint32_t            TDAL_FLAi_uiPartitionCount = 0;
LOCAL   tTDAL_FLA_PartitionInfo*   TDAL_FLAi_pstPartitionInfo = NULL;

/*   Mutex   to   avoid   concurrency   access   to   flash   */
LOCAL   TDAL_mutex_id      TDAL_FLAi_pstMutex;   


/****************************************************************************
 *  FUNCTIONS DECLARATIONS (LOCAL/GLOBAL)                        *
 ****************************************************************************/
bool   TDAL_FLAi_CheckIfReadEnable(   uint32_t   ulAddress   );
bool   TDAL_FLAi_CheckIfWriteEnable(   uint32_t   ulAddress   );

/****************************************************************************
 *  FUNCTIONS DEFINITIONS (LOCAL/GLOBAL)                        *
 ****************************************************************************/

/*===========================================================================
 *
 * TDAL_FLA_APIRevisionGet
 *
 * Parameters:
 *
 * Description:
 *      
 * Returns:
 *
 *===========================================================================*/
const   char   *   
TDAL_FLA_APIRevisionGet(void   )
{
   return   ("TDAL_FLA_REVISION");
}

/*===========================================================================
 *
 * TDAL_FLA_PlatformRevisionGet
 *
 * Parameters:
 *
 * Description:
 *
 * Returns:
 *
 *===========================================================================*/
const   char   *   
TDAL_FLA_PlatformRevisionGet(void   )
{
   return   ("TDAL_FLA_PLATEFORM");
}
/**========================================================================**
 * Function    : TDAL_FLA_Init
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_FLA_ErrorCode
TDAL_FLA_Init(void)
{
   uint32_t   i=1;
   uint32_t   count=0;
   
   mTBOX_FCT_ENTER(   "TDAL_FLA_Init"   );

   /*   Check   primary   error   */
   if   (TDAL_FLAi_bAlreadyInitialized == true)
   {
      mTBOX_TRACE((kTBOX_NIV_1,   "[TDAL_FLA_Init]   Already   Initialized\n"));
      mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
   }
   
   /*   Get   the   information   about   the   number   of   partion   in   flash   */
   FLASH_CFG_GetPartitionCount(&count);

   TDAL_FLAi_pstPartitionInfo = (tTDAL_FLA_PartitionInfo*)   TDAL_Malloc(count   *   sizeof(   tTDAL_FLA_PartitionInfo));
   if   (TDAL_FLAi_pstPartitionInfo == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_Init]   Malloc   failed\n"));
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }
   
   TDAL_FLAi_uiPartitionCount = count;

   TDAL_CreateMutex(&TDAL_FLAi_pstMutex);

   /*   Open   all   MTD   partitions   */

   for   (i=0;   i < TDAL_FLAi_uiPartitionCount;   i++   )
   {
      tFLASH_CFG_PartitionInfo   partitionInfo;
   
      if   (FLASH_CFG_GetPartitionInfo(i,   &partitionInfo) == TRUE)
      {
      TDAL_FLAi_pstPartitionInfo[i].addressOffset = partitionInfo.addressOffset;
      TDAL_FLAi_pstPartitionInfo[i].size       = partitionInfo.size;
   
      TDAL_FLAi_pstPartitionInfo[i].mtdHandle = open(partitionInfo.name,   O_RDWR);
      }
   
      if   (TDAL_FLAi_pstPartitionInfo[i].mtdHandle == -1)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_Init]   Unable   to   open   the   MTD   RAW   flash   handle\n"));
      mTBOX_RETURN(eTDAL_FLA_ERROR);
      }
   }

   mTBOX_TRACE((kTBOX_NIV_1,"FLASH_Init   success\n"));

   TDAL_FLAi_bAlreadyInitialized = true;
   TDAL_FLAi_bIsPartitionSet = false;   

   FLASH_CFG_InitMemoryMappingStructure();

   mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
   
}
/**========================================================================**
 * Function Name :   TDAL_FLA_GetPartitionHandle
 *
 * Description   : Return the name of the partition
 *
 * Side effects  :
 *
 * Comment       :
 *
 **========================================================================**/
uint32_t
TDAL_FLA_GetPartitionHandle(uint32_t   ulAddress,   uint32_t*   pOffset)   
{
   uint8_t      index;

   for   (index=0;   index<   TDAL_FLAi_uiPartitionCount;   index++)
   {
      if   ((ulAddress >= TDAL_FLAi_pstPartitionInfo[index].addressOffset) && 
      (ulAddress < (TDAL_FLAi_pstPartitionInfo[index].addressOffset+TDAL_FLAi_pstPartitionInfo[index].size)))
      {
      *pOffset = TDAL_FLAi_pstPartitionInfo[index].addressOffset;
      return   TDAL_FLAi_pstPartitionInfo[index].mtdHandle;   
      }
   }
   
   mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_GetPartitionHandle]   partition   not   found\n"));
   
   *pOffset = 0;
   return   -1;
   
}

/**========================================================================**
 * Function Name : TDAL_FLA_Term
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment       :
 *
 **========================================================================**/
tTDAL_FLA_ErrorCode   
TDAL_FLA_Term   (void)
{   
   mTBOX_FCT_ENTER("TDAL_FLA_Term");

   if   (TDAL_FLAi_bAlreadyInitialized == false)
   {
      mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
   }

   TDAL_DeleteMutex(TDAL_FLAi_pstMutex);
   
   TDAL_FLAi_bAlreadyInitialized = false;

   mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_FLA_SetPartition
 *
 * Parameters:
 *       Pointer on allocated and filled tTDAL_FLA_Partition array
 * Description:
 *
 *
 * Returns:
 *      eTDAL_FLA_NO_ERROR       no error
 *      eTDAL_FLA_BAD_PARAMETER  wrong argument value
 *      eTDAL_FLA_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_FLA_ErrorCode   
TDAL_FLA_SetPartition   (uint8_t            ucNbPartition,
            tTDAL_FLA_Partition   *   pstPartition)
{
   mTBOX_FCT_ENTER("TDAL_FLA_SetPartition");   

   mLockAccess(TDAL_FLAi_pstMutex);

   if   ((pstPartition == NULL)   ||   
      (ucNbPartition == 0))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_SetPartition]   Check   Primary   Error   Failed\n"));      
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(eTDAL_FLA_BAD_PARAMETER);
   }

   /*   For   trace   purpose   */
   {
      int   i;   
      for   (i=0;   i<ucNbPartition;   i++)
      {   
      mTBOX_TRACE((kTBOX_NIV_WARNING,"[TDAL_FLA_SetPartition]   %d/%d   _   Start=0x%x   _   End=0x%x   _   Right=0x%x\n",   
               i,   
               ucNbPartition,   
               pstPartition[i].StartAddress,
               pstPartition[i].EndAddress,
               pstPartition[i].Rights));
      }
   }

   if   (TDAL_FLAi_pstPartitions   !=   NULL)
   {
      TDAL_Free(TDAL_FLAi_pstPartitions);
   }
      
   TDAL_FLAi_pstPartitions = (tTDAL_FLA_Partition*)   TDAL_Malloc(ucNbPartition   *   sizeof(tTDAL_FLA_Partition));
   if   (TDAL_FLAi_pstPartitions == NULL)
   {
      TDAL_FLAi_ucNbPartition = 0;
      TDAL_FLAi_bIsPartitionSet = false;

      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_SetPartition]   Malloc   failed\n"));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }

   /*   Recopy   the   partition   array   */
   memcpy(TDAL_FLAi_pstPartitions,   
      pstPartition,   
      ucNbPartition   *   sizeof(tTDAL_FLA_Partition));

   TDAL_FLAi_ucNbPartition = ucNbPartition;
   TDAL_FLAi_bIsPartitionSet = true;

   mUnLockAccess(TDAL_FLAi_pstMutex);
   mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
}

/**========================================================================**
 * Function Name : TDAL_FLA_GetBlockNumber
 *
 * Description   : Get block number from address
 *
 * Side effects  :
 *
 * Comment       :
 *
 **========================================================================**/
uint16_t      
TDAL_FLA_GetBlockNumber(uint32_t   ulAddress)
{
   uint16_t        Block = 0xFFFF;   
   tFLASH_CFG_FLASH_Area   eArea;
   tFLASH_CFG_Location      sAreaLocation;

   mTBOX_FCT_ENTER("TDAL_FLA_GetBlockNumber");

   mLockAccess(TDAL_FLAi_pstMutex);

   for   (eArea=0;   eArea<eFLASH_CFG_FLASH_NB_AREA;   eArea++)
   {
      if(FLASH_CFG_GetFlashAreaLocation(eArea,   &sAreaLocation))
      {
      if   ((ulAddress >= sAreaLocation.addressOffset)   &&
        (ulAddress < (sAreaLocation.addressOffset+sAreaLocation.size)))   
      {
        Block = ulAddress   /   kTDAL_FLA_BLOCK_SIZE;
        break;
      }
      }      
   }

   if   ((Block == 0xFFFF) && 
      (FLASH_CFG_IsNVMInFlash() == true))
   {
      if(FLASH_CFG_GetNvmLocation(&sAreaLocation))
      {
      if   ((ulAddress >= sAreaLocation.addressOffset)   &&
        (ulAddress < (sAreaLocation.addressOffset+sAreaLocation.size)))
      {
        Block = ulAddress   /   kTDAL_FLA_BLOCK_SIZE;        
      }
      }      
   }

   if   (Block == 0xFFFF)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_GetBlockNumber]   @   0x%x   out   of   bound\r\n",ulAddress));
   }

   mTBOX_TRACE((kTBOX_NIV_2,   "[TDAL_FLA_GetBlockNumber]   Block=%d   for   @=0x%x\n",   Block,   ulAddress));

   mUnLockAccess(TDAL_FLAi_pstMutex);
   mTBOX_RETURN(   Block   );
}

/**========================================================================**
 * Function Name : TDAL_FLA_GetBlockSize
 *
 * Description   : Return the size of the block number
 *
 * Side effects  :
 *
 * Comment       :
 *
 **========================================================================**/
uint32_t   
TDAL_FLA_GetBlockSize(uint16_t   usBlockNo)
{
   mTBOX_FCT_ENTER("TDAL_FLA_GetBlockSize");

   mLockAccess(TDAL_FLAi_pstMutex);

   if   (usBlockNo >= TDAL_FLA_GetNumberOfBlocks()   )
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_GetBlockSize]   BlockNo   %d   out   of   bound\r\n",usBlockNo));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(0);
   }

   mUnLockAccess(TDAL_FLAi_pstMutex);
   mTBOX_RETURN(kTDAL_FLA_BLOCK_SIZE);
}

/**========================================================================**
 * Function Name : TDAL_FLA_GetBlockAddress
 *
 * Description   : return the begin address of the block number
 *
 * Side effects  :
 *
 * Comment       :
 *
 **========================================================================**/
uint32_t   
TDAL_FLA_GetBlockAddress(uint16_t   usBlockNo)
{
   
   mTBOX_FCT_ENTER("TDAL_FLA_GetBlockAddress");

   mLockAccess(TDAL_FLAi_pstMutex);

   if   (usBlockNo >= TDAL_FLA_GetNumberOfBlocks())
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_GetBlockAddress]   BlockNo   %d   out   of   bound\r\n",usBlockNo));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(   0xFFFFFFFF   );
   }

   mTBOX_TRACE((kTBOX_NIV_2,"[TDAL_FLA_GetBlockAddress]   @=0x%x   for   blockNo=%d\n",   
            usBlockNo*kTDAL_FLA_BLOCK_SIZE,   
            usBlockNo));
   mUnLockAccess(TDAL_FLAi_pstMutex);
   mTBOX_RETURN(usBlockNo*kTDAL_FLA_BLOCK_SIZE);
}

/**========================================================================**
 * Function Name : TDAL_FLA_GetNumberOfBlocks
 *
 * Description   : Return the number of all blocks
 *
 * Side effects  :
 *
 * Comment       :
 *
 **========================================================================**/
uint16_t   
TDAL_FLA_GetNumberOfBlocks(void)
{
   uint16_t        uiBlocks;
   tFLASH_CFG_Location   sFLASH_CFG_FLASH_Location;
   
   mTBOX_FCT_ENTER(TDAL_FLA_GetNumberOfBlocks);

   mLockAccess(TDAL_FLAi_pstMutex);

   if   (FLASH_CFG_GetFlashLocation(&sFLASH_CFG_FLASH_Location))
   {
      uiBlocks = sFLASH_CFG_FLASH_Location.size   /   kTDAL_FLA_BLOCK_SIZE;
      mTBOX_TRACE((kTBOX_NIV_2,
            "[TDAL_FLA_GetNumberOfBlocks]   Total   Size = 0x%x   _   Block   Size = 0x%x   _   Nb   Blocks = %d\n",   
            sFLASH_CFG_FLASH_Location.size,
            kTDAL_FLA_BLOCK_SIZE,
            uiBlocks));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(uiBlocks);
   }
   else
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
            "[TDAL_FLA_GetNumberOfBlocks]   FLASH_CFG_GetFlashLocation   call   failed\n"));
   }   
   mUnLockAccess(TDAL_FLAi_pstMutex);
   mTBOX_RETURN(0);
}


/**========================================================================**
 * Function Name : TDAL_FLA_Open
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment       :
 *
 **========================================================================**/
tTDAL_FLA_ErrorCode   
TDAL_FLA_Open(void)
{
   mTBOX_FCT_ENTER("TDAL_FLA_Open");
   mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
}

/**========================================================================**
 * Function Name : TDAL_FLA_Close
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment       :
 *
 **========================================================================**/
tTDAL_FLA_ErrorCode   
TDAL_FLA_Close(void)
{
   mTBOX_FCT_ENTER("TDAL_FLA_Close");   
   mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
}


/**========================================================================**
 * Function Name : TDAL_FLA_Erase
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment       :
 *
 **========================================================================**/
tTDAL_FLA_ErrorCode   
TDAL_FLA_Erase(uint32_t   ulAddress,   
        uint32_t   ulNumberToErase)
{
   uint32_t        erasedata[2];
   uint32_t        offset = 0;

   mTBOX_FCT_ENTER("TDAL_FLA_Erase");

   mLockAccess(TDAL_FLAi_pstMutex);

   mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_FLA_Erase]   Adress = 0x%x   for   %d   bytes\n",   ulAddress,   ulNumberToErase));

#if   1   /*   For   debug   purpose   */
#endif

   /*   check   right   access   */
   if   (TDAL_FLAi_bIsPartitionSet == true)
   {
      if   (!TDAL_FLAi_CheckIfWriteEnable(ulAddress))
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_Erase]   No   Right   Erase   @   0x%x\r\n",
               ulAddress));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(kTDAL_FLA_ERROR_WHEN_NO_RIGHT);
      }
   }
/*
   else
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_Erase]   Partition   is   not   set\r\n"));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(kTDAL_FLA_ERROR_WHEN_NO_RIGHT);
   }
*/

   if   (kTDAL_FLA_BLOCK_SIZE   !=   ulNumberToErase)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,
            "[TDAL_FLA_Erase]   NumberToErase   doesn't   match   the   block   size\n"));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(eTDAL_FLA_BAD_PARAMETER);
   }   

   if(lseek(TDAL_FLA_GetPartitionHandle(ulAddress,   &offset),   0,   SEEK_SET) == -1)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_Erase]   Unable   to   seek   the   flash   block\n"));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }
#if   1   /*   BELB   _   18/10/07   _   Disable   if   problem   with   new   flash   layout   */
   erasedata[0] = ulAddress   -   offset;
   erasedata[1] = kTDAL_FLA_BLOCK_SIZE;
   if   (ioctl(TDAL_FLA_GetPartitionHandle(ulAddress,   &offset),   MEMERASE,   &erasedata) == -1)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_Erase]   ioctl   return   an   error\n"));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }
#endif
   
   mUnLockAccess(TDAL_FLAi_pstMutex);
   
   mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
}

/**========================================================================**
 * Function Name : TDAL_FLA_Read
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment       :
 *
 **========================================================================**/
uint32_t   
TDAL_FLA_Read(uint32_t   ulAddress,
        uint8_t   *   pucBuffer,
        uint32_t   ulNumberToRead)
{   
   int      bytes_read = 0;   
   uint32_t      offset = 0;

   mTBOX_FCT_ENTER("TDAL_FLA_Read");

   mLockAccess(TDAL_FLAi_pstMutex);

   mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_FLA_Read]   Adress = 0x%x   for   %d   bytes\n",   ulAddress,   ulNumberToRead));

   if   (TDAL_FLAi_bIsPartitionSet == true)
   {
      if   (!TDAL_FLAi_CheckIfReadEnable(ulAddress))
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_Read]   Read   No   Right   @   0x%x\r\n",
            ulAddress));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(0);
      }
   }
   if(   ulNumberToRead == 0   ||   pucBuffer == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,   "[TDAL_FLA_Read]   Bad   Parameter\n"));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN   (0);
   }

   if(lseek(TDAL_FLA_GetPartitionHandle(ulAddress,   &offset),   ulAddress-offset,   SEEK_SET) == -1)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_Read]   Unable   to   seek   the   flash   block\n"));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }   
   
   bytes_read = read(TDAL_FLA_GetPartitionHandle(ulAddress,   &offset),   
            pucBuffer,   
            ulNumberToRead);

   if(bytes_read   !=   ulNumberToRead)
   {
      perror("READ   FAILED");
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_Read]   Unable   to   read   a   flash   block\n"));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(0);
   }
   
   mUnLockAccess(TDAL_FLAi_pstMutex);
   mTBOX_RETURN(bytes_read);
}

/**========================================================================**
 * Function Name : TDAL_FLA_Write
 *
 * Description   :
 *
 * Side effects  :
 *
 * Comment       :
 *
 **========================================================================**/
uint32_t   
TDAL_FLA_Write(uint32_t   ulAddress,
        uint8_t*   pucBuffer,
        uint32_t   ulNumberToWrite)
{
   uint16_t      blockNumber;
   uint32_t      bytes_written = 0;
   uint32_t      bytes_read = 0;   
   uint32_t      realAddr;      /*   real   start   address   of   write      */
   uint32_t      realCount;      /*   real   number   of   bytes   write        */
   uint32_t      begin;      /*   number   bytes   between   addr   and   realAddr   */
   uint32_t      end;      /*   number   bytes   between   (addr+count)      */
   uint32_t      offset;

   mTBOX_FCT_ENTER("TDAL_FLA_Write");

   mLockAccess(TDAL_FLAi_pstMutex);

   mTBOX_TRACE((kTBOX_NIV_1,"[TDAL_FLA_Write]   Adress = 0x%x   for   %d   bytes\n",   ulAddress,   ulNumberToWrite));

   blockNumber = TDAL_FLA_GetBlockNumber(ulAddress);

   if   (blockNumber==0xFFFF)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_Write]   @   0x%x   out   of   bound\r\n",
            ulAddress));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(   0   );
   }
   
   /*   check   right   access   */
   if   (TDAL_FLAi_bIsPartitionSet == true)
   {
      if   (!TDAL_FLAi_CheckIfWriteEnable(ulAddress))
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_Write]   No   Right   Write   @   0x%x\r\n",
               ulAddress));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(   0   );
      }
   }
   
   if(ulNumberToWrite == 0   ||   pucBuffer == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,   "[TDAL_FLA_Write]   Bad   Parameter\n"));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(   0   );
   }

   begin      =(uint32_t)ulAddress   %   (uint32_t)kTDAL_FLA_MIN_ACCESS_WIDTH;
   end      =(uint32_t)(ulAddress   +   ulNumberToWrite)   %   (uint32_t)kTDAL_FLA_MIN_ACCESS_WIDTH;

   mTBOX_TRACE((kTBOX_NIV_4,"[TDAL_FLA_Write]   begin=%d   end=%d\n",begin,end));

   /*   minAccessWidth=16   or   32   bits,   or   start   address   is   missaligned   */
   realCount = ulNumberToWrite;
   if   (begin   >   0)
   {
      /*   align   realAddr   and   update   realCount   */
      realAddr = (uint32_t)(ulAddress   -   begin);
      realCount = realCount   +   (uint32_t)begin;
   }
   else
   {
      /*   addr   is   already   aligned   */
      realAddr = ulAddress;
   }
   if   (end   >   0)
   {
      /*   end   of   buffer   is   not   aligned   :   update   realCount   */
      realCount = realCount   +   (uint32_t)(kTDAL_FLA_MIN_ACCESS_WIDTH   -   end);
   }   

   /*read   the   buffer*/
   if(lseek(TDAL_FLA_GetPartitionHandle(ulAddress,   &offset),   realAddr-offset,   SEEK_SET) == -1)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_Write]   Unable   to   seek   the   flash   block\n"));
      mUnLockAccess(TDAL_FLAi_pstMutex);      
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }
   
   bytes_read = read(TDAL_FLA_GetPartitionHandle(ulAddress,   &offset),   TDAL_FLAi_pucLayerBuffer,   realCount);
   if(bytes_read!=realCount)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_Write]   problem   with   read   before   write\n"));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(eTDAL_FLA_NOT_DONE);
   }

   memcpy((TDAL_FLAi_pucLayerBuffer+begin),   pucBuffer,   ulNumberToWrite);

   if(lseek(TDAL_FLA_GetPartitionHandle(ulAddress,   &offset),   realAddr-offset,   SEEK_SET) == -1)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_Write]   Unable   to   seek   the   flash   block\n"));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }

#if   1   /*   BELB   _   18/10/07   _   Disable   if   problem   with   new   flash   layout   */
   bytes_written = write(TDAL_FLA_GetPartitionHandle(ulAddress,   &offset),   
               TDAL_FLAi_pucLayerBuffer,   
               realCount);
#endif

   if(bytes_written   !=   realCount)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,   "[TDAL_FLA_Write]   Unable   to   write   a   flash   block\n"));
      mUnLockAccess(TDAL_FLAi_pstMutex);
      mTBOX_RETURN(   0   );
   }
      
   bytes_written = bytes_written-begin;

   if   (end   >   0)
   {
      bytes_written = bytes_written   -   (kTDAL_FLA_MIN_ACCESS_WIDTH   -   end);
   }

   mUnLockAccess(TDAL_FLAi_pstMutex);
   mTBOX_RETURN(bytes_written);
}

/*===========================================================================
 *
 * TDAL_FLAi_CheckIfWriteEnable
 *
 * Parameters:
 *
 * Description:
 *
 *
 * Returns:
 *
 *===========================================================================*/
bool   
TDAL_FLAi_CheckIfWriteEnable(uint32_t   ulAddress)
{
   uint8_t   index;
   bool   bFoundArea = false;

   mTBOX_FCT_ENTER("TDAL_FLAi_CheckIfWriteEnable");

   if   (TDAL_FLAi_pstPartitions == NULL)
   {      
      /*   If   partition   not   set   _   Write   is   enable   on   all   flash   area   contain   in   FLASH_CFG_FLASH_AreaLocationArray   */
      for   (index=0;   index<eFLASH_CFG_FLASH_NB_AREA;   index++)
      {
      if   ((ulAddress >= FLASH_CFG_FLASH_AreaLocationArray[index].addressOffset) && 
        (ulAddress < (FLASH_CFG_FLASH_AreaLocationArray[index].addressOffset+FLASH_CFG_FLASH_AreaLocationArray[index].size)))
      {
        bFoundArea = true;
      }
      }      
      
      mTBOX_RETURN   bFoundArea;
   }

   for   (index = 0;index<TDAL_FLAi_ucNbPartition;index++)
   {
      if   (ulAddress >= TDAL_FLAi_pstPartitions[index].StartAddress)
      {
      if   ((ulAddress <= TDAL_FLAi_pstPartitions[index].EndAddress)   &&
        (TDAL_FLAi_pstPartitions[index].Rights & TDAL_FLA_WRITE_ENABLE))
      {
        mTBOX_RETURN   true;
      }
      }
   }

   mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_FLAi_CheckIfWriteEnable]   WRITE   not   enable   for   @=0x%x\n",   ulAddress));
   mTBOX_RETURN   false;
}


/*===========================================================================
 *
 * TDAL_FLAi_CheckIfReadEnable
 *
 * Parameters:
 *
 * Description:
 *
 *
 * Returns:
 *
 *===========================================================================*/
bool   
TDAL_FLAi_CheckIfReadEnable(uint32_t   ulAddress)
{
   uint8_t   index;

   mTBOX_FCT_ENTER("TDAL_FLAi_CheckIfReadEnable");

   if   (TDAL_FLAi_pstPartitions == NULL)
   {      
      /*   If   partition   not   set   _   Read   is   enable   on   all   flash   area   */
      mTBOX_RETURN   true;
   }      

   for   (index = 0;
      index<TDAL_FLAi_ucNbPartition;
      index++)
   {
      if   (ulAddress >= TDAL_FLAi_pstPartitions[index].StartAddress)
      {
      if   ((ulAddress <= TDAL_FLAi_pstPartitions[index].EndAddress)   &&
        (TDAL_FLAi_pstPartitions[index].Rights & TDAL_FLA_READ_ENABLE))
      {
        mTBOX_RETURN   true;
      }
      }
   }

      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_FLAi_CheckIfReadEnable]   READ   not   enable   for   @=0x%x\n",   ulAddress));

   mTBOX_RETURN   false;
}

/*===========================================================================
 *
 * TDAL_FLA_ChangePartitionRight
 *
 * Parameters:
 *
 * Description:
 *
 *
 * Returns:
 *
 *===========================================================================*/
tTDAL_FLA_ErrorCode   TDAL_FLA_ChangePartitionRight(uint32_t   ulAddress,   uint32_t   ulSize,   uint8_t   uRights)
{
   int32_t   index=0;
   tTDAL_FLA_Partition   stPartition;
   
   mTBOX_FCT_ENTER("TDAL_FLA_ChangePartitionRight");

   if   (TDAL_FLAi_pstPartitions   !=   NULL)
   for   (index = 0;index<TDAL_FLAi_ucNbPartition;index++)
   {
      if   (ulAddress == TDAL_FLAi_pstPartitions[index].StartAddress)
      {
      if   ((ulAddress+ulSize-1) == TDAL_FLAi_pstPartitions[index].EndAddress)   
      {
        TDAL_FLAi_pstPartitions[index].Rights = uRights;
        return   eTDAL_FLA_NO_ERROR;
      }
      }
   }
   mTBOX_TRACE((kTBOX_NIV_CRITICAL,"[TDAL_FLA_ChangePartitionRight]   partition   not   found\n"));
   mTBOX_RETURN   eTDAL_FLA_NOT_DONE;
}

