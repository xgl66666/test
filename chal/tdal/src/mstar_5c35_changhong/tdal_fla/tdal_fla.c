/*****************************************************************************
 *                  COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_FLA
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_fla/tdal_fla.c $
 *        $Rev: 673 $
 *        $Date: 2011-10-28 15:10:43 +0200 (Fri, 28 Oct 2011) $
 *
 * Description : Flash driver TDAL library implementation.
 *
 ****************************************************************************/


/****************************************************************************
 *  INCLUDES                                           *
 ****************************************************************************/
/*   Generic   Headers   */
#include <stdio.h>
#include "string.h"   /*   needed   for   memcpy()   */
#include "crules.h"
#include "tbox.h"

#include "tdal_common.h"
#include "tdal_fla.h"

#include "flash_cfg.h"
#include "MsMemory.h"
#include "drvSERFLASH.h"
#include "drvBDMA.h"

#ifdef USE_LOCK_SPLS
#include "tdal_perso.h"
#endif

/****************************************************************************
 *  DEFINES                                            *
 ****************************************************************************/
#define      ERROR_WHEN_NO_RIGHT      eTDAL_FLA_NOT_DONE

#define		 MSFLASH_ACCESS_8_BITS  1
#define		 MSFLASH_ACCESS_16_BITS 2


/****************************************************************************
 *  MACROS                                              *
 ****************************************************************************/
#define MEM_ALIGN_CONSTANT 16
/****************************************************************************
 *  TYPEDEFS                                           *
 ****************************************************************************/
typedef   struct{
	uint32_t   Address;
	uint32_t   Length;
}tTDAL_FLA_BlockInfo;

tTDAL_FLA_BlockInfo lastReadAddressBlock = {0};


#define CH_FLASH_MAP		(0)
#define FLASH_DMA_READ_MIN_SIZE 0x400
/****************************************************************************
 *  GLOBAL VARIABLES (GLOBAL/IMPORT)                            *
 ****************************************************************************/
LOCAL tTDAL_FLA_Partition  * TDAL_FLA_Partitions = NULL;

/****************************************************************************
 * LOCAL   MODULE   VARIABLES   (MODULE/IMPORT)                        *
 ****************************************************************************/
TDAL_mutex_id        TDAL_FLA_Mutex = NULL;
#if CH_FLASH_MAP
#define CH_FLASH_MAP_NUM (2)
uint32_t g_flash_map_src[CH_FLASH_MAP_NUM][2] = {{0x780000,0x78ffff},{0x7A0000,0x7Affff} };
uint32_t g_flash_map_dst[CH_FLASH_MAP_NUM] = {0x7E0000,0x7F0000  };

TDAL_FLA_AddressMap(uint32_t   * rp_Address)
{
    int i = 0;
    uint32_t fla_address = *rp_Address;
    for(i = 0; i < CH_FLASH_MAP_NUM;i++)
    {
        if(fla_address >= g_flash_map_src[i][0] &&  fla_address <= g_flash_map_src[i][1] )
        {
            *rp_Address = (g_flash_map_dst[i] + (fla_address - g_flash_map_src[i][0]));
        }
    }

}
#endif
/****************************************************************************
 *  LOCAL FILE VARIABLES (LOCAL)                               *
 ****************************************************************************/
mTBOX_SET_MODULE(eTDAL_FLA);

LOCAL   tTDAL_FLA_BlockInfo *TDAL_FLA_BlockInfoList;
LOCAL   int32_t             TDAL_FLA_AlreadyInitialized = 0;
LOCAL   bool                TDAL_FLA_isPartitionSet = 0;
LOCAL   bool                TDAL_FLA_Opened=FALSE;
LOCAL   uint8_t             TDAL_FLA_NbPartition = 0;
LOCAL   uint8_t             *TDAL_FLA_LayerBuffer = NULL;
LOCAL   uint8_t             *TDAL_FLA_LayerNotAlligned = NULL;

LOCAL  uint32_t TDAL_FLA_BlockCount = 0;
LOCAL  uint32_t TDAL_FLA_BlockSize = 0;

LOCAL bool mstarFlashInitialized = false;

/****************************************************************************
 *  FUNCTIONS DECLARATIONS (LOCAL/GLOBAL)                        *
 ****************************************************************************/
LOCAL bool        p_TDAL_FLA_CheckIfReadEnable(   uint32_t   Address, uint32_t   Size   );
LOCAL bool        p_TDAL_FLA_CheckIfWriteEnable(   uint32_t   Address, uint32_t   Size   );
LOCAL bool        p_TDAL_FLA_CheckValidAddress(uint32_t address);
LOCAL bool        p_TDAL_FLA_IsCached(uint32_t address);
LOCAL void        p_TDAL_FLA_SetCachedBlock(uint32_t address);
LOCAL uint32_t    p_TDAL_FLA_ReadFlash(uint32_t address, uint32_t count);
LOCAL void        p_TDAL_FLA_ReadCache(uint8_t *buffer, uint32_t address, uint32_t count);
LOCAL void        p_TDAL_FLA_ResetCachedBlock();

/****************************************************************************
 *  FUNCTIONS DEFINITIONS (LOCAL/GLOBAL)                        *
 ****************************************************************************/

bool p_TDAL_FLA_GetFlashInfo()
{
    const SERFLASH_Info* pSerFlashInfo;

    pSerFlashInfo = MDrv_SERFLASH_GetInfo();
    if (pSerFlashInfo != NULL)
    {
        TDAL_FLA_BlockCount = (pSerFlashInfo->u32SecNum);
        TDAL_FLA_BlockSize = pSerFlashInfo->u32SecSize;
        mTBOX_TRACE((kTBOX_NIV_1, "Number of blocks detected = %d,block size=0x%x\n", TDAL_FLA_BlockCount,TDAL_FLA_BlockSize));
        return true;
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MDrv_SERFLASH_GetInfo() returned error\n"));
        return false;
    }
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
tTDAL_FLA_ErrorCode   TDAL_FLA_Init()
{
    uint32_t             address;
    uint16_t             i;
    MS_BOOL              bRet;
    uint8_t             *tmpLayerBuffer = NULL;
    mTBOX_FCT_ENTER(   "TDAL_FLA_Init"   );

    if   (TDAL_FLA_AlreadyInitialized == 1)
    {
        mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
    }

    if (!mstarFlashInitialized)
    {
        MDrv_SERFLASH_Init();
        mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_FLA_Init]   MDrv_SERFLASH_Init()"));
        if (p_TDAL_FLA_GetFlashInfo() == false)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not obtain flash information\n"));
            mTBOX_RETURN(eTDAL_FLA_ERROR);
        }
    }
    
    mTBOX_TRACE((kTBOX_NIV_1, "****************************************\n"));
    mTBOX_TRACE((kTBOX_NIV_1, "*   BDMA, virtual cache for over 0x400 *\n"));
    mTBOX_TRACE((kTBOX_NIV_1, "*   refactored and improved            *\n"));
    mTBOX_TRACE((kTBOX_NIV_1, "****************************************\n"));
    
    TDAL_FLA_BlockInfoList = TDAL_Malloc(TDAL_FLA_BlockCount * sizeof(tTDAL_FLA_BlockInfo));
    mTBOX_ASSERT(TDAL_FLA_BlockInfoList != NULL);

    TDAL_FLA_Partitions = TDAL_Malloc(TDAL_FLA_BlockCount * 9 * sizeof(tTDAL_FLA_Partition));
    mTBOX_ASSERT(TDAL_FLA_Partitions != NULL);

    TDAL_FLA_LayerNotAlligned = TDAL_Malloc(TDAL_FLA_BlockSize * sizeof(uint8_t) + MEM_ALIGN_CONSTANT);
    mTBOX_ASSERT(TDAL_FLA_LayerNotAlligned != NULL);
    if (TDAL_FLA_LayerNotAlligned != 0)
    {
        TDAL_FLA_LayerBuffer = (void *)MEMALIGN(MEM_ALIGN_CONSTANT, (uint32_t) TDAL_FLA_LayerNotAlligned);
    }

    address = /*BaseAddress*/   0;
    for (i=0 ; i < TDAL_FLA_BlockCount; i++   )
    {
        //TDAL_FLA_STBlocks[i].Type = STFLASH_MAIN_BLOCK;
        //TDAL_FLA_STBlocks[i].Length = kTDAL_FLA_BLOCK_SIZE;

        TDAL_FLA_BlockInfoList[i].Address = address;
        TDAL_FLA_BlockInfoList[i].Length = TDAL_FLA_BlockSize;
        address   +=   TDAL_FLA_BlockSize;
    }

    //   create   a   mutex   to   protect   access   to   the   flash   -   done   only   once
    //MS_S32 MsOS_CreateMutex ( MsOSAttribute eAttribute, char *pMutexName, MS_U32 u32Flag);
    if   (TDAL_FLA_Mutex == NULL)
    {
        TDAL_CreateMutex(&TDAL_FLA_Mutex);
        //MsOS_CreateMutex ( MsOSAttribute eAttribute, char *pMutexName, MS_U32 u32Flag);
    }

    if (!mstarFlashInitialized)
    {
        //set to RIU mode
        bRet = MDrv_SERFLASH_SetMode(1); //XIU is faster and default
        mTBOX_ASSERT(bRet == TRUE);
    }

    TDAL_FLA_AlreadyInitialized = 1;
    TDAL_FLA_isPartitionSet = 0;
    mstarFlashInitialized = true;

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
tTDAL_FLA_ErrorCode   TDAL_FLA_SetPartition   (   uint8_t   nbPartition   ,
		tTDAL_FLA_Partition * partition   )
{
    mTBOX_FCT_ENTER("TDAL_FLA_SetPartition");

    if   ((partition == NULL) || (nbPartition == 0))
    {
        mTBOX_RETURN(eTDAL_FLA_BAD_PARAMETER);
    }

    //TBD
    if   (TDAL_FLA_Partitions != NULL)
    {
        TDAL_Free(TDAL_FLA_Partitions);
    }
    TDAL_FLA_Partitions = (tTDAL_FLA_Partition*)   TDAL_Malloc(nbPartition * sizeof(tTDAL_FLA_Partition));

    if   (TDAL_FLA_Partitions == NULL)
    {
        TDAL_FLA_NbPartition = 0;
        TDAL_FLA_isPartitionSet = 0;
        mTBOX_RETURN(eTDAL_FLA_ERROR);
    }

    memcpy(TDAL_FLA_Partitions, partition, nbPartition * sizeof(tTDAL_FLA_Partition));
    /* For backup NVM-in-flash strategy */
    TDAL_FLA_Partitions[nbPartition].Rights = TDAL_FLA_READ_ENABLE | TDAL_FLA_WRITE_ENABLE;
    TDAL_FLA_Partitions[nbPartition].StartAddress = 0x510000;
    TDAL_FLA_Partitions[nbPartition].EndAddress = 0x520000 - 1;

    TDAL_FLA_NbPartition = nbPartition + 1;
    TDAL_FLA_isPartitionSet = 1;
    /* Write protection should be removed earlier for NVM backup mechanism*/
    if (FALSE==MDrv_SERFLASH_WriteProtect(FALSE))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "a: MDrv_SERFLASH_WriteProtect: FALSE\n"));
        mTBOX_RETURN eTDAL_FLA_ERROR;
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_1, "a: MDrv_SERFLASH_WriteProtect: TRUE\n"));
        TDAL_FLA_Opened=TRUE;
    }

    mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
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

tTDAL_FLA_ErrorCode TDAL_FLA_Term(void)
{
    mTBOX_FCT_ENTER("TDAL_FLA_Term");

    if   (TDAL_FLA_AlreadyInitialized == 0)
    {
        mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
    }

    //memset(&FLASH_TermParams,0,sizeof(STFLASH_TermParams_t));

    TDAL_Free(TDAL_FLA_Partitions);
    TDAL_FLA_Partitions = NULL;

    TDAL_Free(TDAL_FLA_LayerBuffer);
    TDAL_FLA_LayerBuffer = NULL;

    /*   Terminate   semaphore   for   api   lock   */
    TDAL_DeleteMutex(TDAL_FLA_Mutex);
    TDAL_FLA_Mutex = NULL;

    TDAL_FLA_AlreadyInitialized = 0;
    TDAL_FLA_isPartitionSet   = 0;
    TDAL_FLA_Close();

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
uint32_t TDAL_FLA_GetBlockNumber(uint32_t Address)
{
    mTBOX_FCT_ENTER("TDAL_FLA_GetBlockNumber");
    uint16_t   Block;

    //   find   the   block   this   address   is   in
    for (Block = 0; Block < TDAL_FLA_BlockCount; Block++)
    {
        if ((Address >= TDAL_FLA_BlockInfoList[Block].Address) &&
            (Address < (TDAL_FLA_BlockInfoList[Block].Address + TDAL_FLA_BlockInfoList[Block].Length)))
        {
            break;
        }
    }
	mTBOX_RETURN(Block);
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
uint32_t   TDAL_FLA_GetBlockSize(   uint32_t   BlockNo   )
{
    if (BlockNo >= TDAL_FLA_BlockCount)
    {
        return 0;
    }
    else
    {
        return TDAL_FLA_BlockInfoList[BlockNo].Length;
    }
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
uint32_t   TDAL_FLA_GetBlockAddress(   uint32_t   BlockNo   )
{
	mTBOX_FCT_ENTER("TDAL_FLA_GetBlockAddress");


	if (BlockNo >= TDAL_FLA_BlockCount)
	{
		mTBOX_RETURN(-1);
	}
	else
	{
		mTBOX_RETURN(TDAL_FLA_BlockInfoList[BlockNo].Address);
	}
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
uint32_t   TDAL_FLA_GetNumberOfBlocks(void)
{
	mTBOX_FCT_ENTER("TDAL_FLA_GetNumberOfBlocks");
	mTBOX_RETURN (   TDAL_FLA_BlockCount   );
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
tTDAL_FLA_ErrorCode   TDAL_FLA_Open(void)
{
	mTBOX_FCT_ENTER("TDAL_FLA_Open");
	if (FALSE==MDrv_SERFLASH_WriteProtect(FALSE))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "a: MDrv_SERFLASH_WriteProtect: FALSE\n"));
		mTBOX_RETURN eTDAL_FLA_ERROR;
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_1, "a: MDrv_SERFLASH_WriteProtect: TRUE\n"));
		TDAL_FLA_Opened=TRUE;
	}
	mTBOX_RETURN (eTDAL_FLA_NO_ERROR);
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
tTDAL_FLA_ErrorCode   TDAL_FLA_Close   (void)
{
	mTBOX_FCT_ENTER("TDAL_FLA_Close");
	if (FALSE==MDrv_SERFLASH_WriteProtect(TRUE))
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "a: MDrv_SERFLASH_WriteProtect: FALSE\n"));
		mTBOX_RETURN( eTDAL_FLA_ERROR);
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_1, "a: MDrv_SERFLASH_WriteProtect: TRUE\n"));
		TDAL_FLA_Opened = FALSE;
	}
	mTBOX_RETURN (eTDAL_FLA_NO_ERROR);
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
tTDAL_FLA_ErrorCode   TDAL_FLA_Erase_OTA   (   uint32_t   Address, uint32_t   NumberToErase   )
{
    uint32_t        blockNumber;
    uint32_t        startingBlockAddress;
    uint32_t        blockSize;

    mTBOX_FCT_ENTER("TDAL_FLA_Erase");

    /*   check   right   access   */

    TDAL_LockMutex(TDAL_FLA_Mutex);

    /*   check   Address   parameter   */
    blockNumber = TDAL_FLA_GetBlockNumber(Address);
    startingBlockAddress = TDAL_FLA_GetBlockAddress(blockNumber);
    Address = startingBlockAddress;

    /*   check   NumberToErase   parameter   */
    blockSize = TDAL_FLA_GetBlockSize(blockNumber);
    NumberToErase = blockSize;
    if   (blockSize != NumberToErase)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "NumberToErase   doesn't   match   the   block   size\n"));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(eTDAL_FLA_BAD_PARAMETER);
    }

    /*   test   if   access   to   flash   memory   is   allowed   */
    if(!TDAL_FLA_Opened)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Flash   not   opened\n"));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(eTDAL_FLA_NOT_DONE);
    }

    if (NumberToErase > 0 &&
            ((p_TDAL_FLA_CheckValidAddress(Address + NumberToErase - 1) != true) ||
                    (p_TDAL_FLA_CheckValidAddress(Address) != true)))
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Address 0x%x not valid\n", Address + NumberToErase - 1));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(eTDAL_FLA_ERROR);
    }

    mTBOX_TRACE((kTBOX_NIV_1, "MDrv_SERFLASH_AddressErase adr 0x%x, num %d\n", Address, NumberToErase));

    if (FALSE == MDrv_SERFLASH_AddressErase(Address, NumberToErase, TRUE))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unable   to   erase   a   flash   block\n"));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(eTDAL_FLA_ERROR);
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_1, "erase   flash   block   @0x%x   s=%d\n", Address, NumberToErase));
    }

    TDAL_UnlockMutex(TDAL_FLA_Mutex);
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
tTDAL_FLA_ErrorCode   TDAL_FLA_Erase   (   uint32_t   Address, uint32_t   NumberToErase   )
{
    uint32_t        blockNumber;
    uint32_t        startingBlockAddress;
    uint32_t        blockSize;

    mTBOX_FCT_ENTER("TDAL_FLA_Erase");

    /*   check   right   access   */
    if   (TDAL_FLA_isPartitionSet == 1)
    {
        if   (p_TDAL_FLA_CheckIfWriteEnable(Address, NumberToErase) == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "No   Right   Erase   @   0x%x\r\n", Address));
            mTBOX_RETURN(   ERROR_WHEN_NO_RIGHT   );
        }
    }
#if CH_FLASH_MAP
    TDAL_FLA_AddressMap(&Address);
#endif

    TDAL_LockMutex(TDAL_FLA_Mutex);

    /*   check   Address   parameter   */
    blockNumber = TDAL_FLA_GetBlockNumber(Address);
    startingBlockAddress = TDAL_FLA_GetBlockAddress(blockNumber);
    if   (Address != startingBlockAddress)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Address   is   not   a   starting   block   address\n"));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(eTDAL_FLA_BAD_PARAMETER);
    }

    /*   check   NumberToErase   parameter   */
    blockSize = TDAL_FLA_GetBlockSize(blockNumber);
    if   (blockSize != NumberToErase)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "NumberToErase   doesn't   match   the   block   size\n"));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(eTDAL_FLA_BAD_PARAMETER);
    }

    /*   test   if   access   to   flash   memory   is   allowed   */
    if(!TDAL_FLA_Opened)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Flash   not   opened\n"));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(eTDAL_FLA_NOT_DONE);
    }

    if (NumberToErase > 0 &&
            ((p_TDAL_FLA_CheckValidAddress(Address + NumberToErase - 1) != true) ||
                    (p_TDAL_FLA_CheckValidAddress(Address) != true)))
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Address 0x%x not valid\n", Address + NumberToErase - 1));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(eTDAL_FLA_ERROR);
    }

    mTBOX_TRACE((kTBOX_NIV_1, "MDrv_SERFLASH_AddressErase adr 0x%x, num %d\n", Address, NumberToErase));

    if (FALSE == MDrv_SERFLASH_AddressErase(Address, NumberToErase, TRUE))
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unable   to   erase   a   flash   block\n"));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(eTDAL_FLA_ERROR);
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_1, "erase   flash   block   @0x%x   s=%d\n", Address, NumberToErase));
    }

    TDAL_UnlockMutex(TDAL_FLA_Mutex);
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
uint32_t   TDAL_FLA_Read_OTA(uint32_t Address,
                             uint8_t *Buffer,
                             uint32_t NumberToRead)
{
    bool   ErrCode;
    uint32_t    nbActualRead;   /*   returned    */
    size_t      realCount;      /*   real   number   of   bytes   read        */
    uint32_t    realAddr;       /*   real   start   address   of   read        */
    uint32_t    begin;          /*   number   bytes   between   addr   and   realAddr   */
    uint32_t    end;            /*   number   bytes   between   (addr+count)      */

    mTBOX_FCT_ENTER("TDAL_FLA_Read");

    if(   NumberToRead == 0 || Buffer == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Bad   Parameter   ERROR\n"));
        mTBOX_RETURN   (0);
    }

    TDAL_LockMutex(TDAL_FLA_Mutex);

    begin=(int)Address % (int)MSFLASH_ACCESS_8_BITS;
    end=(int)(Address + NumberToRead) % (int)MSFLASH_ACCESS_8_BITS;
    mTBOX_TRACE((kTBOX_NIV_1,"begin=%d   end=%d\n",begin,end));

    realCount = NumberToRead;

    if   (begin > 0)
    {
        realAddr = (uint32_t)(   Address   -   begin   );
        realCount = realCount + (uint32_t)   begin;
    }
    else
    {
        realAddr = Address;
    }
    if   (end > 0)
    {
        realCount = realCount + (MSFLASH_ACCESS_8_BITS   -   end);
    }

    if (NumberToRead > 0 &&
       ((p_TDAL_FLA_CheckValidAddress(realAddr + realCount - 1) != true) ||
        (p_TDAL_FLA_CheckValidAddress(realAddr) != true))
    )
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Address %x not valid\n", realAddr + realCount - 1));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(0);
    }

    mTBOX_TRACE((kTBOX_NIV_1,"Read   %u   octets   at   address   [0x%x]\n",NumberToRead,Address));

    if (p_TDAL_FLA_IsCached(realAddr) == FALSE)
    {
        p_TDAL_FLA_SetCachedBlock(realAddr);
        nbActualRead = p_TDAL_FLA_ReadFlash(realAddr, realCount);
    }
    else
    {
        nbActualRead = realCount;
    }
    
    if (nbActualRead != realCount)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "nbActualRead != realCount   \n"));
        TDAL_UnlockMutex(   TDAL_FLA_Mutex   );
        mTBOX_RETURN(0);
    }

    p_TDAL_FLA_ReadCache(Buffer, realAddr - begin, NumberToRead);
    
    nbActualRead=nbActualRead-begin;
    if   (end > 0)
    {
        nbActualRead=nbActualRead-(MSFLASH_ACCESS_8_BITS-end);
    }
    if   (nbActualRead != NumberToRead)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "nbActualRead   %u != count   %u\n",nbActualRead,NumberToRead));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(0);
    }

    TDAL_UnlockMutex(TDAL_FLA_Mutex);
    mTBOX_RETURN(nbActualRead);
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
uint32_t   TDAL_FLA_Read(uint32_t Address,
                         uint8_t * Buffer,
                         uint32_t NumberToRead)
{
    bool        ErrCode;
    uint32_t    nbActualRead;   /*   returned    */
    size_t      realCount;      /*   real   number   of   bytes   read        */
    uint32_t    realAddr;       /*   real   start   address   of   read        */
    uint32_t    begin;          /*   number   bytes   between   addr   and   realAddr   */
    uint32_t    end;            /*   number   bytes   between   (addr+count)      */
    uint32_t    allignDiff = 0;
    /*        and   (realAddr+realCount)   */
    mTBOX_FCT_ENTER("TDAL_FLA_Read");

    if (TDAL_FLA_isPartitionSet == 1)
    {
        if (p_TDAL_FLA_CheckIfReadEnable(Address, NumberToRead) == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Read   No   Right   @   0x%x\r\n", Address));
            mTBOX_RETURN(0);
        }
    }
#if CH_FLASH_MAP
    TDAL_FLA_AddressMap(&Address);
#endif
    if(   NumberToRead == 0 || Buffer == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Bad   Parameter   ERROR\n"));
        mTBOX_RETURN   (0);
    }

    TDAL_LockMutex(TDAL_FLA_Mutex);

    begin=(int)Address % (int)MSFLASH_ACCESS_8_BITS;
    end=(int)(Address + NumberToRead) % (int)MSFLASH_ACCESS_8_BITS;
    mTBOX_TRACE((kTBOX_NIV_1,"begin=%d   end=%d\n",begin,end));

    realCount = NumberToRead;

    if   (begin > 0)
    {
        realAddr = (uint32_t)(   Address   -   begin   );
        realCount = realCount + (uint32_t)   begin;
    }
    else
    {
        realAddr = Address;
    }
    if   (end > 0)
    {
        realCount = realCount + (MSFLASH_ACCESS_8_BITS   -   end);
    }

    if (NumberToRead > 0 &&
       ((p_TDAL_FLA_CheckValidAddress(realAddr + realCount - 1) != true) ||
        (p_TDAL_FLA_CheckValidAddress(realAddr) != true))
    )
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Address %x not valid\n", realAddr + realCount - 1));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(0);
    }

    if (p_TDAL_FLA_IsCached(realAddr) == FALSE)
    {
        p_TDAL_FLA_SetCachedBlock(realAddr);
        nbActualRead = p_TDAL_FLA_ReadFlash(realAddr, realCount);
    }
    else
    {
        nbActualRead = realCount;
    }

    if (nbActualRead != realCount)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "nbActualRead != realCount   \n"));
        TDAL_UnlockMutex(   TDAL_FLA_Mutex   );
        mTBOX_RETURN(0);
    }

    p_TDAL_FLA_ReadCache(Buffer, realAddr - begin, NumberToRead);

    nbActualRead=nbActualRead-begin;
    if (end > 0)
    {
        nbActualRead=nbActualRead-(MSFLASH_ACCESS_8_BITS-end);
    }

    if (nbActualRead != NumberToRead)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "nbActualRead   %u != count   %u\n",nbActualRead,NumberToRead));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(0);
    }

    TDAL_UnlockMutex(TDAL_FLA_Mutex);
    mTBOX_RETURN(nbActualRead);
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
uint32_t   TDAL_FLA_Write_OTA(uint32_t Address,
                              uint8_t* Buffer,
                              uint32_t NumberToWrite)
{
    uint32_t      nbActualWrite;
    uint32_t      nbActualRead;
    uint32_t      realAddr;   /*   real   start   address   of   write      */
    uint32_t      realCount;   /*   real   number   of   bytes   write        */
    uint32_t      begin;       /*   number   bytes   between   addr   and   realAddr   */
    uint32_t      end;       /*   number   bytes   between   (addr+count)      */
    /*        and   (realAddr+realCount)   */
    uint32_t      BlockBaseAddress;
    bool   ms_err = TRUE;
    /*uint32_t      Blockbyteswritten;*/

    mTBOX_FCT_ENTER("TDAL_FLA_Write()");

    if(NumberToWrite == 0 || Buffer == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Bad   Parameter   ERROR\n"));
        mTBOX_RETURN(   0   );
    }

    TDAL_LockMutex(TDAL_FLA_Mutex);

    BlockBaseAddress = (Address   /   TDAL_FLA_BlockSize) * TDAL_FLA_BlockSize;

    begin   =(int)Address % (int)MSFLASH_ACCESS_8_BITS;
    end     =(int)(Address + NumberToWrite) % (int)MSFLASH_ACCESS_8_BITS;
    mTBOX_TRACE((kTBOX_NIV_4,"begin=%d   end=%d\n",begin,end));

    realCount = NumberToWrite;
    if   (begin > 0)
    {
        /*   align   realAddr   and   update   realCount   */
        realAddr = (uint32_t)(Address   -   begin);
        realCount = realCount + (uint32_t)begin;
    }
    else
    {
        /*   addr   is   already   aligned   */
        realAddr = Address;
    }
    if   (end > 0)
    {
        /*   end   of   buffer   is   not   aligned   :   update   realCount   */
        realCount = realCount + (uint32_t)(MSFLASH_ACCESS_8_BITS   -   end);
    }

    if (realCount > 0 &&
       ((p_TDAL_FLA_CheckValidAddress(realAddr + realCount - 1) != true) ||
        (p_TDAL_FLA_CheckValidAddress(realAddr) != true))
    )
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Address 0x%x not valid\n", realAddr + realCount - 1));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(0);
    }
    /*   read   to   get   bytes   before   and   after   buffer   in   TDAL_FLA_LayerBuffer   */
    ms_err = MDrv_SERFLASH_Read(BlockBaseAddress,TDAL_FLA_BlockSize,TDAL_FLA_LayerBuffer);
    nbActualRead = realCount;
    if(   ms_err == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MSFLASH_Read   failed\n"   ));
        TDAL_UnlockMutex(   TDAL_FLA_Mutex   );
        mTBOX_RETURN(   0   );
    }

    if   (   nbActualRead != realCount   )
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "nbActualRead != realCount   \n"));
        TDAL_UnlockMutex(   TDAL_FLA_Mutex   );
        mTBOX_RETURN(0);
    }

    mTBOX_TRACE((kTBOX_NIV_1,"Write   %u   octets   at   address   [0x%x]\n", NumberToWrite,Address));

    /*   copy   bytes   from   buffer   to   TDAL_FLA_LayerBuffer   */
    //memcpy(&(TDAL_FLA_LayerBuffer[begin]),Buffer,NumberToWrite);
    memcpy(&(TDAL_FLA_LayerBuffer[realAddr - BlockBaseAddress]),Buffer,NumberToWrite);

    TDAL_FLA_Erase_OTA(realAddr,realCount);
    mTBOX_TRACE((kTBOX_NIV_1, "Flash Writing: realAddr = 0x%x, realCount = %d\n", realAddr,realCount));
    ms_err = MDrv_SERFLASH_Write(BlockBaseAddress,TDAL_FLA_BlockSize,TDAL_FLA_LayerBuffer);
    nbActualWrite=realCount;
    //STOS_TaskUnlock();
    if(   ms_err == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MSFLASH_Write   failed \n"));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(   0   );
    }
    else
    {
        mTBOX_TRACE((kTBOX_NIV_1, "write   flash[Addr@0x%x   s=%d]   OK\n", realAddr, realCount));
    }

    if   (nbActualWrite != realCount)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "nbActualWrite != realCount   \n"));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(   0   );
    }
    mTBOX_TRACE((kTBOX_NIV_1,"Number   bytes   written   :   %u   \n", nbActualWrite));

    nbActualWrite=nbActualWrite-begin;
    if   (end > 0)
    {
        nbActualWrite=nbActualWrite-(MSFLASH_ACCESS_8_BITS-end);
    }

    if   (nbActualWrite != NumberToWrite)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "nbActualWrite != count   \n"));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(   0   );
    }

#if   0
    p_TDAL_FLA_VerifyWrite(realAddr, realCount, BlockBaseAddress, Address, NumberToWrite, beginOffset);
#endif

    p_TDAL_FLA_ResetCachedBlock();

    TDAL_UnlockMutex(TDAL_FLA_Mutex);
    mTBOX_RETURN(nbActualWrite);
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
uint32_t   TDAL_FLA_Write(uint32_t Address,
                          uint8_t* Buffer,
                          uint32_t NumberToWrite)
{
    uint32_t      nbActualWrite;
    uint32_t      nbActualRead;
    uint32_t      realAddr;   /*   real   start   address   of   write      */
    uint32_t      realCount;   /*   real   number   of   bytes   write        */
    uint32_t      begin;       /*   number   bytes   between   addr   and   realAddr   */
    uint32_t      end;       /*   number   bytes   between   (addr+count)      */
    /*        and   (realAddr+realCount)   */
    uint32_t      BlockBaseAddress;
    bool   ms_err = TRUE;
    /*uint32_t      Blockbyteswritten;*/

    mTBOX_FCT_ENTER("TDAL_FLA_Write()");

    /*   check   right   access   */
    if   (TDAL_FLA_isPartitionSet == 1)
    {
        if   (p_TDAL_FLA_CheckIfWriteEnable(Address, NumberToWrite) == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "No   Right   Write   @   0x%x\r\n", Address));
            mTBOX_RETURN(   0   );
        }
    }
#if CH_FLASH_MAP
    TDAL_FLA_AddressMap(&Address);
#endif
    if(NumberToWrite == 0 || Buffer == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Bad   Parameter   ERROR\n"));
        mTBOX_RETURN(   0   );
    }

    TDAL_LockMutex(TDAL_FLA_Mutex);

    begin   =(int)Address % (int)MSFLASH_ACCESS_8_BITS;
    end     =(int)(Address + NumberToWrite) % (int)MSFLASH_ACCESS_8_BITS;
    mTBOX_TRACE((kTBOX_NIV_4,"begin=%d   end=%d\n",begin,end));

    realCount = NumberToWrite;
    if   (begin > 0)
    {
        /*   align   realAddr   and   update   realCount   */
        realAddr = (uint32_t)(Address   -   begin);
        realCount = realCount + (uint32_t)begin;
    }
    else
    {
        /*   addr   is   already   aligned   */
        realAddr = Address;
    }
    if   (end > 0)
    {
        /*   end   of   buffer   is   not   aligned   :   update   realCount   */
        realCount = realCount + (uint32_t)(MSFLASH_ACCESS_8_BITS   -   end);
    }

    if (realCount > 0 &&
       ((p_TDAL_FLA_CheckValidAddress(realAddr + realCount - 1) != true) ||
        (p_TDAL_FLA_CheckValidAddress(realAddr) != true))
    )
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "Address 0x%x not valid\n", realAddr + realCount - 1));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(0);
    }
    memcpy(&(TDAL_FLA_LayerBuffer[begin]),Buffer,NumberToWrite);
    /*   read   to   get   bytes   before   and   after   buffer   in   TDAL_FLA_LayerBuffer   */

    ms_err = MDrv_SERFLASH_Write(realAddr, realCount, TDAL_FLA_LayerBuffer);
    nbActualWrite=realCount;
    if(   ms_err == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MSFLASH_Write   failed \n"));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(   0   );
    }

    if   (nbActualWrite != realCount)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "nbActualWrite != realCount   \n"));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(   0   );
    }
    mTBOX_TRACE((kTBOX_NIV_1,"Number   bytes   written   :   %u   \n", nbActualWrite));

    nbActualWrite=nbActualWrite-begin;
    if   (end > 0)
    {
        nbActualWrite=nbActualWrite-(MSFLASH_ACCESS_8_BITS-end);
    }

    if   (nbActualWrite != NumberToWrite)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "nbActualWrite != count   \n"));
        TDAL_UnlockMutex(TDAL_FLA_Mutex);
        mTBOX_RETURN(   0   );
    }

    p_TDAL_FLA_ResetCachedBlock();

    TDAL_UnlockMutex(TDAL_FLA_Mutex);
    mTBOX_RETURN(nbActualWrite);
}

/*===========================================================================
 *
 * p_TDAL_FLA_CheckIfWriteEnable
 *
 * Parameters:
 *
 * Description: only start and end address are tested
 *
 * Returns:
 *
 *===========================================================================*/
bool   p_TDAL_FLA_CheckIfWriteEnable(uint32_t   Address, uint32_t   Size)
{
   uint8_t   index;

   if   (TDAL_FLA_Partitions == NULL)
      return   TRUE;

   for   (index = 0; index < TDAL_FLA_NbPartition; index++)
   {
      /*   test   start   address   */
      if   (   Address >= TDAL_FLA_Partitions[index].StartAddress   &&
      (TDAL_FLA_Partitions[index].Rights   &   TDAL_FLA_WRITE_ENABLE))
      {
      /*   test   if   end   address   is   still   in   the   partition   */
      if   ((Address+Size-1) <= TDAL_FLA_Partitions[index].EndAddress)
      {
        return   TRUE;
      }
      }
   }

   return   FALSE;
}

/*===========================================================================
 *
 * p_TDAL_FLA_CheckIfReadEnable
 *
 * Parameters:
 *
 * Description:
 *
 *
 * Returns:
 *
 *===========================================================================*/
bool   p_TDAL_FLA_CheckIfReadEnable(uint32_t   Address, uint32_t   Size)
{
   uint8_t   index;

   if   (TDAL_FLA_Partitions == NULL)
      return   TRUE;

   for   (index = 0; index < TDAL_FLA_NbPartition; index++)
   {
      /*   test   start   address   */
      if   (   Address >= TDAL_FLA_Partitions[index].StartAddress   &&
      (TDAL_FLA_Partitions[index].Rights   &   TDAL_FLA_READ_ENABLE))
      {
      /*   test   if   end   address   is   still   in   the   partition   */
      if   ((Address+Size-1) <= TDAL_FLA_Partitions[index].EndAddress)
      {
        return   TRUE;
      }
      }
   }

   return   FALSE;
}

bool p_TDAL_FLA_CheckValidAddress(uint32_t address)
{
    bool 			result = false;
    MS_BOOL 		bRet;
    MS_U32			blockIndex = 0xFFFFFFFF;
    uint32_t        blockCount;

    mTBOX_FCT_ENTER("p_TDAL_FLA_CheckValidAddress");

    bRet = MDrv_SERFLASH_AddressToBlock((MS_U32) address, &blockIndex);

    if (bRet == TRUE)
    {
        blockCount = TDAL_FLA_GetNumberOfBlocks();
        if (blockIndex < blockCount)
        {
            result = true;
        }
    }

    mTBOX_TRACE((kTBOX_NIV_1, "[p_TDAL_FLA_CheckValidAddress] Address 0x%x is %s and belongs to block %d\n", address, result == true ? "valid" : "invalid", blockIndex ));

    mTBOX_RETURN(result);
}

bool p_TDAL_FLA_IsCached(uint32_t address)
{
    if (address>=lastReadAddressBlock.Address + lastReadAddressBlock.Length ||
        address<lastReadAddressBlock.Address)
        return false;
    else
        return true;
}

void p_TDAL_FLA_SetCachedBlock(uint32_t address)
{
    lastReadAddressBlock.Address = address;
    lastReadAddressBlock.Length = TDAL_FLA_BlockSize;
}

void p_TDAL_FLA_ResetCachedBlock()
{
    lastReadAddressBlock.Address = 0;
    lastReadAddressBlock.Length = 0;
}

uint32_t p_TDAL_FLA_ReadFlash(uint32_t address, uint32_t count)
{
    if (count > FLASH_DMA_READ_MIN_SIZE)
    {
        BDMA_Result bdma_ret = E_BDMA_OK;
        lastReadAddressBlock.Address = address & 0xFFFF0000;
        lastReadAddressBlock.Length  = 0x10000;
        
        //mTBOX_TRACE((kTBOX_NIV_CRITICAL, "read flash[ddress=0x%x real_count=%d cachedAddr=%d]   OK\n", address, count, lastReadAddressBlock.Address));
        bdma_ret = MDrv_BDMA_CopyHnd(lastReadAddressBlock.Address,
                                     MsOS_VA2PA(TDAL_FLA_LayerBuffer), 
                                     lastReadAddressBlock.Length, 
                                     E_BDMA_FLASH2SDRAM, 0x8);
        if(bdma_ret != E_BDMA_OK)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Failed to read through BDMA"
                                            " address [0x%x] size %d\n", 
                                            lastReadAddressBlock.Address, 
                                            lastReadAddressBlock.Length ));
        }
        MsOS_Dcache_Flush(TDAL_FLA_LayerBuffer, lastReadAddressBlock.Length);
        MsOS_FlushMemory();
    }
    else
    {
        MS_BOOL ms_ret = TRUE;
        //mTBOX_TRACE((kTBOX_NIV_CRITICAL, "read flash[ddress=0x%x real_count=%d cachedAddr=%d]   OK\n", address, count, lastReadAddressBlock.Address));
        ms_ret = MDrv_SERFLASH_Read(address, count, TDAL_FLA_LayerBuffer);
        if( ms_ret == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unable   to   read   flash\n"));
        }
        lastReadAddressBlock.Address = address;
        lastReadAddressBlock.Length  = 0x0;
    }
    
    return count;
}

void p_TDAL_FLA_ReadCache(uint8_t *buffer, uint32_t address, uint32_t count)
{
    memcpy(buffer, &(TDAL_FLA_LayerBuffer[address - lastReadAddressBlock.Address]), count);
}

#if   0
bool p_TDAL_FLA_VerifyWrite(uint32_t realAddr, 
                            uint32_t realCount,
                            uint32_t BlockBaseAddress,
                            uint32_t Address,
                            uint32_t NumberToWrite,
                            uint32_t beginOffset
                            )
{
    mTBOX_FCT_ENTER("p_TDAL_FLA_VerifyWrite");
/*-----------------------------------------------------------*/
    /*   Verification   that   the   write   operation   has   been   successful   */
    /*-----------------------------------------------------------*/
    /*   read   to   get   bytes   before   and   after   buffer   in   TDAL_FLA_LayerBuffer   */
    if(   MDrv_SERFLASH_Read(realAddr,realCount,TDAL_FLA_LayerBuffer) == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unable   to   read   flash\n"));
        mTBOX_RETURN(0);
    }   

    #if   0
    {
        uint32_t   count;

        for   (count=0;count<NumberToWrite;count++)
        {
            if(*(TDAL_FLA_LayerBuffer+beginOffset+count) != *(Buffer+count))
            {

                /*      mTBOX_RETURN(0);*/
            }
        }

    }
    #else
        if(memcmp(TDAL_FLA_LayerBuffer+beginOffset,Buffer,NumberToWrite)!=0)
        {
            printf("Flash   Page   Error\r\n");
            mTBOX_TRACE((kTBOX_NIV_WARNING, "TDAL_FLA_LayerBuffer[begin+i] != buffer[i]\n"));
        #if   0
            /*   Save   all   the   block   */
            if(   MDrv_SERFLASH_Read(realAddr,realCount,TDAL_FLA_LayerBuffer) == FALSE)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unable   to   read   flash\n"));
                mTBOX_RETURN(0);
            }
    
            MDrv_SERFLASH_AddressErase(BlockBaseAddress, TDAL_FLA_BlockSize, TRUE);
            /*   Copy   the   right   buffer   at   the   right   place   */
            memcpy(TDAL_FLA_BlockBuffer + (Address   -   BlockBaseAddress), Buffer, NumberToWrite);
    
            MDrv_SERFLASH_Write(BlockBaseAddress,TDAL_FLA_BlockSize,TDAL_FLA_BlockBuffer)
    
            MDrv_SERFLASH_Read(realAddr,realCount,TDAL_FLA_LayerBuffer);
    
            if(memcmp(TDAL_FLA_LayerBuffer+beginOffset,Buffer,NumberToWrite)!=0)
            {
                printf("Err\r\n");
            }
        #endif
        }
    #endif
    /*-----------------------------------------------------------*/
    mTBOX_RETURN(1);
}
#endif
