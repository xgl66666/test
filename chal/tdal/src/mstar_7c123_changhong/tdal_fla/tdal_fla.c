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
#include "drvSERFLASH.h"

#ifdef USE_LOCK_SPLS
#include "tdal_perso.h"
#endif

/****************************************************************************
 *  DEFINES                                            *
 ****************************************************************************/
#define      ERROR_WHEN_NO_RIGHT      eTDAL_FLA_NOT_DONE

#define		 MSFLASH_ACCESS_8_BITS  1
#define		 MSFLASH_ACCESS_16_BITS 2

#if !defined (PAIRING_KEY_ENCRYPTED)
#define          PAIRING_KEY_ENCRYPTED   0
#endif
/****************************************************************************
 *  MACROS                                              *
 ****************************************************************************/

/****************************************************************************
 *  TYPEDEFS                                           *
 ****************************************************************************/
typedef   struct{
	uint32_t   Address;
	uint32_t   Length;
}tTDAL_FLA_BlockInfo;




/****************************************************************************
 *  GLOBAL VARIABLES (GLOBAL/IMPORT)                            *
 ****************************************************************************/
LOCAL tTDAL_FLA_Partition  * TDAL_FLA_Partitions = NULL;

/****************************************************************************
 * LOCAL   MODULE   VARIABLES   (MODULE/IMPORT)                        *
 ****************************************************************************/
TDAL_mutex_id        TDAL_FLA_Mutex = NULL;
/****************************************************************************
 *  LOCAL FILE VARIABLES (LOCAL)                               *
 ****************************************************************************/
mTBOX_SET_MODULE(eTDAL_FLA);
LOCAL   tTDAL_FLA_BlockInfo *   TDAL_FLA_BlockInfoList;
LOCAL   int32_t             TDAL_FLA_AlreadyInitialized = 0;
LOCAL   bool            TDAL_FLA_isPartitionSet = 0;
LOCAL   bool            TDAL_FLA_Opened=FALSE;
LOCAL   uint8_t               TDAL_FLA_NbPartition = 0;
//LOCAL   uint8_t             * TDAL_FLA_LayerBuffer = NULL;

LOCAL  uint32_t TDAL_FLA_BlockCount = 0;
LOCAL  uint32_t TDAL_FLA_BlockSize = 0;

LOCAL bool mstarFlashInitialized = false;

/****************************************************************************
 *  FUNCTIONS DECLARATIONS (LOCAL/GLOBAL)                        *
 ****************************************************************************/
bool   p_TDAL_FLA_CheckIfReadEnable(   uint32_t   Address, uint32_t   Size   );
bool   p_TDAL_FLA_CheckIfWriteEnable(   uint32_t   Address, uint32_t   Size   );
bool p_TDAL_FLA_CheckValidAddress(uint32_t address);

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

	TDAL_FLA_BlockInfoList = TDAL_Malloc(TDAL_FLA_BlockCount * sizeof(tTDAL_FLA_BlockInfo));
	mTBOX_ASSERT(TDAL_FLA_BlockInfoList != NULL);

	TDAL_FLA_Partitions = TDAL_Malloc(TDAL_FLA_BlockCount * 9 * sizeof(tTDAL_FLA_Partition));
	mTBOX_ASSERT(TDAL_FLA_Partitions != NULL);

	//TDAL_FLA_LayerBuffer = TDAL_Malloc(TDAL_FLA_BlockSize * sizeof(uint8_t));
	//mTBOX_ASSERT(TDAL_FLA_LayerBuffer != NULL);

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
		bRet = MDrv_SERFLASH_SetMode(0); //XIU is faster and default
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

	TDAL_FLA_NbPartition = nbPartition;
	TDAL_FLA_isPartitionSet = 1;

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

tTDAL_FLA_ErrorCode   TDAL_FLA_Term   (void)
{
	mTBOX_FCT_ENTER("TDAL_FLA_Term");

	if   (TDAL_FLA_AlreadyInitialized == 0)
	{
		mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
	}

	//memset(&FLASH_TermParams,0,sizeof(STFLASH_TermParams_t));

	TDAL_Free(TDAL_FLA_Partitions);
	TDAL_FLA_Partitions = NULL;

	//TDAL_Free(TDAL_FLA_LayerBuffer);
	//TDAL_FLA_LayerBuffer = NULL;

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
uint32_t      TDAL_FLA_GetBlockNumber(   uint32_t   Address   )
{
	//MS_U32   Block;

	mTBOX_FCT_ENTER("TDAL_FLA_GetBlockNumber");
	uint16_t   Block;

	//   find   the   block   this   address   is   in
	for   (   Block = 0; Block < TDAL_FLA_BlockCount; Block++   )
	{
		if   ((   Address >= TDAL_FLA_BlockInfoList[Block].Address   )   &&   (   Address < (   TDAL_FLA_BlockInfoList[Block].Address   +TDAL_FLA_BlockInfoList[Block].Length   )   ))
		{
			break;
		}
	}



	/*if(FALSE == MDrv_SERFLASH_AddressToBlock((MS_U32)Address, &Block))
	{
		//printf("1: appDemo_SerFlash_RWTest u32StartAddr[0x%lx] is error \n",*u32StartAddr);
		mTBOX_RETURN(eTDAL_FLA_BAD_PARAMETER);
	}
	else
	{
		//printf("2: appDemo_SerFlash_RWTest u32StartBlock[0x%lx](%d) \n",u32StartBlock,u32StartBlock);
	}*/

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
uint32_t   TDAL_FLA_GetBlockSize(   uint32_t   BlockNo   )
{
	//mTBOX_FCT_ENTER("TDAL_FLA_GetBlockSize");

	if (BlockNo >= TDAL_FLA_BlockCount)
	{
		//mTBOX_TRACE((kTBOX_NIV_1, "BlockNo=%d\n",BlockNo));
		//mTBOX_RETURN(0);
		return 0;
	}
	else
	{
		//mTBOX_TRACE((kTBOX_NIV_1, "TDAL_FLA_GetBlockSize[%d]=0x%x\n",BlockNo,TDAL_FLA_BlockInfoList[BlockNo].Length));
		//mTBOX_RETURN(TDAL_FLA_BlockInfoList[BlockNo].Length);
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
    //#if 0//skip here for supporting erase bytes instead of blocks
    #if 1 // for pass TestFlashEraseBadBlock
	blockSize = TDAL_FLA_GetBlockSize(blockNumber);
	if   (blockSize != NumberToErase)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "NumberToErase   doesn't   match   the   block   size\n"));
		TDAL_UnlockMutex(TDAL_FLA_Mutex);
		mTBOX_RETURN(eTDAL_FLA_BAD_PARAMETER);
	}
	#endif
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
uint32_t   TDAL_FLA_Read   (   uint32_t   Address   ,
		uint8_t * Buffer   ,
		uint32_t   NumberToRead   )
{
	bool   ErrCode;
	uint32_t   nbActualRead; /*   returned    */
	size_t   realCount;   /*   real   number   of   bytes   read        */
	uint32_t   realAddr;   /*   real   start   address   of   read        */
	uint32_t   begin;       /*   number   bytes   between   addr   and   realAddr   */
	uint32_t   end;       /*   number   bytes   between   (addr+count)      */
	/*        and   (realAddr+realCount)   */

	mTBOX_FCT_ENTER("TDAL_FLA_Read");

	if   (TDAL_FLA_isPartitionSet == 1)
	{
		if   (p_TDAL_FLA_CheckIfReadEnable(Address, NumberToRead) == FALSE)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Read   No   Right   @   0x%x\r\n", Address));
			mTBOX_RETURN(   0   );
		}
	}

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


	/***************************************************************************************/
	/*   While watching live, if Flash has to be accessed (Timer DB update or whatever...)  */
	/*   this task_lock()/task_unlock() is causing Video Artefacts!!!            */
	/***************************************************************************************/
	//STOS_TaskLock();

	ErrCode = MDrv_SERFLASH_Read(realAddr,realCount,Buffer);
	nbActualRead = realCount;

	if(   ErrCode == FALSE)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unable   to   read   flash\n"));
		TDAL_UnlockMutex(   TDAL_FLA_Mutex   );
		mTBOX_RETURN(0);
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_1, "read   flash[Addr@0x%x   s=%d]   OK\n", realAddr, realCount));
	}

	if   (nbActualRead != realCount)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "nbActualRead != realCount   \n"));
		TDAL_UnlockMutex(   TDAL_FLA_Mutex   );
		mTBOX_RETURN(0);
	}

	//memcpy(Buffer,&(TDAL_FLA_LayerBuffer[begin]),NumberToRead);

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

#if defined(KAON_MSD5C59_BOOTLOADER) && (PAIRING_KEY_ENCRYPTED == 1)
	/* If PK is encrypted, decrypt it */
	if (Address == 0x160 && (Buffer[0] != 0x00 || Buffer[1] != 0x00 || Buffer[2] != 0x01 || Buffer[3] != 0x6C))
	{
		unsigned char PK_decrypted[NumberToRead];
		if (TDAL_SEC_Decryptdata(Buffer, PK_decrypted, NumberToRead) == 0)
		{
			memcpy(Buffer, PK_decrypted, NumberToRead);
		}
	}
#endif
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
uint32_t   TDAL_FLA_Write   (   uint32_t   Address   ,
		uint8_t*   Buffer   ,
		uint32_t   NumberToWrite   )
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

	if(NumberToWrite == 0 || Buffer == NULL)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "Bad   Parameter   ERROR\n"));
		mTBOX_RETURN(   0   );
	}

	TDAL_LockMutex(TDAL_FLA_Mutex);

	BlockBaseAddress = (Address   /   TDAL_FLA_BlockSize) * TDAL_FLA_BlockSize;

	begin      =(int)Address % (int)MSFLASH_ACCESS_8_BITS;
	end      =(int)(Address + NumberToWrite) % (int)MSFLASH_ACCESS_8_BITS;
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

	/***************************************************************************************/
	/*   While watching live, if Flash has to be accessed (Timer DB update or whatever...)  */
	/*   this task_lock()/task_unlock() is causing Video Artefacts!!!            */
	/***************************************************************************************/
	//STOS_TaskLock();
	/*ms_err = STFLASH_Read(   TDAL_FLA_STFlash_Hndl   ,
			(uint32_t)(realAddr   ),
			TDAL_FLA_LayerBuffer,
			realCount,
			&nbActualRead);*/
	mTBOX_TRACE((kTBOX_NIV_1, "Flash Read: realAddr = 0x%x, realCount = %d\n", realAddr, realCount));

	//ms_err = MDrv_SERFLASH_Read(realAddr,realCount,TDAL_FLA_LayerBuffer);
		nbActualRead = realCount;
	//STOS_TaskUnlock();

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

	/*   write   TDAL_FLA_LayerBuffer   */

	/***************************************************************************************/
	/*   While watching live, if Flash has to be accessed (Timer DB update or whatever...)  */
	/*   this task_lock()/task_unlock() is causing Video Artefacts!!!            */
	/***************************************************************************************/
	//STOS_TaskLock();

	/*ms_err = STFLASH_Write(   TDAL_FLA_STFlash_Hndl   ,
			(uint32_t)(realAddr   ),
			TDAL_FLA_LayerBuffer,
			realCount,
			&nbActualWrite);*/
	mTBOX_TRACE((kTBOX_NIV_1, "Flash Writing: realAddr = 0x%x, realCount = %d\n", realAddr,realCount));
	ms_err = MDrv_SERFLASH_Write(realAddr,realCount,Buffer);
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
	/*-----------------------------------------------------------*/
	/*   Verification   that   the   write   operation   has   been   successful   */
	/*-----------------------------------------------------------*/
	/*   read   to   get   bytes   before   and   after   buffer   in   TDAL_FLA_LayerBuffer   */
	if(   MDrv_SERFLASH_Read(realAddr,realCount,TDAL_FLA_LayerBuffer) == FALSE)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unable   to   read   flash\n"));
		TDAL_UnlockMutex(TDAL_FLA_Mutex);
		mTBOX_RETURN(0);
	}

#if   0
	{
		uint32_t   count;

		for   (count=0;count<NumberToWrite;count++)
		{
			if(*(TDAL_FLA_LayerBuffer+begin+count) != *(Buffer+count))
			{
				TDAL_UnlockMutex(TDAL_FLA_Mutex);
				/*      mTBOX_RETURN(0);*/
			}
		}

	}
#else
	if(memcmp(TDAL_FLA_LayerBuffer+begin,Buffer,NumberToWrite)!=0)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "TDAL_FLA_LayerBuffer[begin+i] != buffer[i]\n"));
#if   0
		/*   Save   all   the   block   */
		if(   MDrv_SERFLASH_Read(realAddr,realCount,TDAL_FLA_LayerBuffer) == FALSE)
		{
			mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Unable   to   read   flash\n"));
			TDAL_UnlockMutex(TDAL_FLA_Mutex);
			mTBOX_RETURN(0);
		}

		MDrv_SERFLASH_AddressErase(BlockBaseAddress, TDAL_FLA_BlockSize, TRUE);
		/*   Copy   the   right   buffer   at   the   right   place   */
		memcpy(TDAL_FLA_BlockBuffer + (Address   -   BlockBaseAddress), Buffer, NumberToWrite);

		MDrv_SERFLASH_Write(BlockBaseAddress,TDAL_FLA_BlockSize,TDAL_FLA_BlockBuffer)

		MDrv_SERFLASH_Read(realAddr,realCount,TDAL_FLA_LayerBuffer);
#endif
	}
#endif
	/*-----------------------------------------------------------*/
#endif

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


#if   (defined(futarque_hybrid) || defined(idtv7109))   &&   defined(JUMP_RESET_FLASH)
/**========================================================================**
 * Function   Name   :   TDAL_FLA_Write
 *
 * Description   :
 *
 * Side   effects   :
 *
 * Comment      :
 *
 **========================================================================**/
uint32_t   TDAL_FLAm_JumpWrite   (   uint32_t   Address,
		uint8_t*   Buffer,
		uint32_t   NumberToWrite   )
{
	return 0;
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
tTDAL_FLA_ErrorCode   TDAL_FLAm_Erase   (   uint32_t   Address, uint32_t   NumberToErase   )
{
	return (eTDAL_FLA_NO_ERROR);
}


#endif

#ifdef USE_LOCK_SPLS
/**
 *  @brief
 *    Process a request to the perso via SDTVDPerso to known the list of
 *    blocks locked in the flash. This function manage a contigus blocks
 *    list and return the numbers of first and last block protected (=locked).
 *
 *  @param[out] pxu32FirstBlockP
 *      Block Number of first block protected.
 *
 *  @param[out] pxu32LastBlockP
 *      Block Number of last block protected.
 *
 *  @retval eTDAL_FLA_NO_ERROR
 *       Request successful
 *
 *  @retval eTDAL_FLA_ERROR
 *       Request failure
 *
 *  @retval eTDAL_FLA_BAD_PARAMETER
 *       Parameter invalid
 *
 *  @remarks
 *			If no protected block (SPLS lock data size = 0) return OK and
 *       the last block number < the first.
 *
 */
/*static tTDAL_FLA_ErrorCode TDAL_FLAi_GetProtectedAreaFromPerso
(
		uint32_t *pxu32FirstBlockP,
		uint32_t *pxu32LastBlockP
)
{
	return (eTDAL_FLA_NO_ERROR);
}*/
#endif


#if   0
/*   A   FUNCTION   TO   TEST   NATIVE   DRIVER   */
#define   ERASED_STATE      0xFF
void   p_TDAL_FLA_DriverErrorReport(   int *       ErrorCount,
		ST_ErrorCode_t   ErrorGiven,
		ST_ErrorCode_t   ExpectedErr   )
{
	ST_ErrorCode_t   Temp = ErrorGiven;

	switch(   Temp   )
	{
	case   ST_NO_ERROR:
		mTBOX_PRINT((kTBOX_LF, "ST_NO_ERROR   -   Successful   Return\n"   ));
		break;

	case   ST_ERROR_ALREADY_INITIALIZED:
		mTBOX_PRINT((kTBOX_LF, "ST_ERROR_ALREADY_INITIALIZED   -   Prior   Init   call   w/o   Term\n"   ));
		break;

	case   ST_ERROR_FEATURE_NOT_SUPPORTED:
		mTBOX_PRINT((kTBOX_LF, "ST_ERROR_FEATURE_NOT_SUPPORTED   -   Device   mismatch\n"   ));
		break;

	case   ST_ERROR_UNKNOWN_DEVICE:
		mTBOX_PRINT((kTBOX_LF, "ST_ERROR_UNKNOWN_DEVICE   -   Init   call   must   precede   access\n"   ));
		break;

	case   ST_ERROR_INVALID_HANDLE:
		mTBOX_PRINT((kTBOX_LF, "ST_ERROR_INVALID_HANDLE   -   Rogue   Handle   value   or   Closed\n"   ));
		break;

	case   ST_ERROR_OPEN_HANDLE:
		mTBOX_PRINT((kTBOX_LF, "ST_ERROR_OPEN_HANDLE   -   Term   called   with   Open   Handle\n"   ));
		break;

	case   ST_ERROR_BAD_PARAMETER:
		mTBOX_PRINT((kTBOX_LF, "ST_ERROR_BAD_PARAMETER   -   Parameter(s)   out   of   valid   range\n"   ));
		break;

	case   ST_ERROR_NO_FREE_HANDLES:
		mTBOX_PRINT((kTBOX_LF, "ST_ERROR_NO_FREE_HANDLES   -   DeviceName   already   Open\n"   ));
		break;

	case   ST_ERROR_TIMEOUT:
		mTBOX_PRINT((kTBOX_LF, "ST_ERROR_TIMEOUT   -   Timeout   limit   reached\n"   ));
		break;

	case   STFLASH_ERROR_WRITE:
		mTBOX_PRINT((kTBOX_LF, "STFLASH_ERROR_WRITE   -   Error   during   Flash   Write\n"   ));
		break;

	case   STFLASH_ERROR_ERASE:
		mTBOX_PRINT((kTBOX_LF, "STFLASH_ERROR_ERASE   -   Error   during   Flash   Erase\n"   ));
		break;

	case   STFLASH_ERROR_VPP_LOW:
		mTBOX_PRINT((kTBOX_LF, "STFLASH_ERROR_VPP_LOW   -   Flash   Vpp   Voltage   too   low\n"   ));
		break;

	default:
		mTBOX_PRINT((kTBOX_LF, "***   Unrecognised   return   code   %x   ***\n"   ));
	}

	if(   ErrorGiven != ExpectedErr   )
	{
		(*ErrorCount)++;
	}

	mTBOX_PRINT((kTBOX_LF, "----------------------------------------------------------\n"   ));
}
uint32_t   TDAL_FLA_TestDriver(   void   )
{
	/*   use   a   STFLASHT   function   */
	BOOL      Match;
	int        i;
	U32        NumberRead;
	U32        NumberWrit;

	int   ErrorCount = 0;
	ST_ErrorCode_t   ReturnError;
	ST_Revision_t   RevisionStr;

	static   U8      ReadArray[STFLASH_MAIN_SIZE];
	static   U8      WritArray[STFLASH_MAIN_SIZE];
	U32        BankNo, TestBlock, TestSize, TestOffset, ErrorCounter;

	STFLASH_InitParams_t   InitParams;
	STFLASH_OpenParams_t   OpenParams;
	STFLASH_Params_t      GetParams;
	STFLASH_TermParams_t   TermParams;

	mTBOX_PRINT((kTBOX_LF, "\n============================================================\n"   ));
	mTBOX_PRINT((kTBOX_LF, "Commencing   Full   Erase/Read/Write   Test   Function   ....\n"   ));
	mTBOX_PRINT((kTBOX_LF, "============================================================\n"   ));

	/*   revision   string   available   before   Flash   Initialized   */
	mTBOX_PRINT((kTBOX_LF, "Calling   STFLASH_GetRevision()   pre-Init   \n"   ));
	RevisionStr = STFLASH_GetRevision();
	mTBOX_PRINT((kTBOX_LF, "Software   Revision   reported   as   %s\n", RevisionStr   ));
	mTBOX_PRINT((kTBOX_LF, "------------------------------------------------------------\n"   ));

	/*   Init   Bank   0, Vpp   0   */

	InitParams.DeviceType   = kTDAL_FLA_DEVICE_TYPE;
	InitParams.BaseAddress   = (U32*)FLASH_BANK_0_BASE_ADDRESS;
	InitParams.VppAddress   = (U32*)0;
	InitParams.MinAccessWidth = kTDAL_FLA_MIN_ACCESS_WIDTH;
	InitParams.MaxAccessWidth = kTDAL_FLA_MAX_ACCESS_WIDTH;
	InitParams.NumberOfBlocks = kTDAL_FLA_NUM_BLOCKS;
	InitParams.Blocks       = TDAL_FLA_STBlocks;
	InitParams.DriverPartition = TheSystemPartition;

	mTBOX_PRINT((kTBOX_LF, "Calling   STFLASH_Init()   Bank0   ..........\n"   ));
	ReturnError = STFLASH_Init(   SYS_FlashName[0]   , &InitParams   );
	p_TDAL_FLA_DriverErrorReport(   &ErrorCount, ReturnError, ST_NO_ERROR   );

	/*   Open   Bank   0   */
	mTBOX_PRINT((kTBOX_LF, "Calling   STFLASH_Open()   Bank0   ..........\n"   ));
	ReturnError = STFLASH_Open(   SYS_FlashName[0],
			&OpenParams,
			&TDAL_FLA_STFlash_Hndl   );
	p_TDAL_FLA_DriverErrorReport(   &ErrorCount, ReturnError, ST_NO_ERROR   );

	for(   BankNo = 0; BankNo < 1; BankNo++   )
	{

		/*   GetParams   for   Bank   BankNo   */
		/*
      GetParams.InitParams.Blocks = GetBlkDat_s;
      mTBOX_PRINT((kTBOX_LF, "Calling   STFLASH_GetParams()   Bank   %u   ....\n", BankNo   ));
      ReturnError = STFLASH_GetParams(   TDAL_FLA_STFlash_Hndl, &GetParams   );
      p_TDAL_FLA_DriverErrorReport(   &ErrorCount, ReturnError, ST_NO_ERROR   );
      ParamsReport(   &GetParams_s   );
		 */
		for(TestBlock = 0; TestBlock < InitParams.NumberOfBlocks; TestBlock++   )
		{
			TestSize = TDAL_FLA_STBlocks[TestBlock].Length;
			TestOffset = 0;
			for(i=0; i<TestBlock; i++)
				TestOffset   +=   TDAL_FLA_STBlocks[i].Length;

			mTBOX_PRINT((kTBOX_LF, "------------------------------------------------------------\n"   ));
			mTBOX_PRINT((kTBOX_LF, "Bank   %u, TestBlock   %2u, TestOffset   0x%08x   TestSize   0x%08x\n",
					BankNo, TestBlock, TestOffset, TestSize   ));
			mTBOX_PRINT((kTBOX_LF, "------------------------------------------------------------\n"   ));


			/*   Erase   Bank   BankNo   Block   TestBlock   */

			mTBOX_PRINT((kTBOX_LF, "Calling   STFLASH_Erase()   Bank   %u   ........\n", BankNo   ));
			ReturnError = STFLASH_Erase(   TDAL_FLA_STFlash_Hndl,
					TestOffset,
					TestSize   );
			if(   ReturnError != ST_NO_ERROR   )
			{
				p_TDAL_FLA_DriverErrorReport(   &ErrorCount, ReturnError, ST_NO_ERROR   );
			}

			/*   the   following   reads   all   of   the   erased   block   */
			mTBOX_PRINT((kTBOX_LF, "Calling   STFLASH_Read()   Bank   %u, ........\n", BankNo   ));
			ReturnError = STFLASH_Read(   TDAL_FLA_STFlash_Hndl,
					TestOffset,
					ReadArray,
					TestSize,
					&NumberRead   );
			if(   ReturnError != ST_NO_ERROR   )
			{
				p_TDAL_FLA_DriverErrorReport(   &ErrorCount, ReturnError, ST_NO_ERROR   );
			}
			mTBOX_PRINT((kTBOX_LF, "NumberActuallyRead   reported   as   %d   bytes\n",
					NumberRead   ));

			Match = TRUE;
			if(   (   ReturnError == ST_NO_ERROR   )   &&
					(   NumberRead == TestSize   )   )
			{
				ErrorCounter = 0;
				for(   i = 0; i < NumberRead; i++   )
				{
					if(   ReadArray[i] != ERASED_STATE   )
					{
						Match = FALSE;
						mTBOX_PRINT((kTBOX_LF, "Mismatch   =>   Erase/ReadArray[%2d] = %02X   /   %02X\n",
								i, ERASED_STATE, (U32)ReadArray[i]   ));
						ErrorCounter++;
					}
				}

				if(   Match   )
				{
					mTBOX_PRINT((kTBOX_LF, "Erased   data   verified   with   Read   of   %d   bytes\n",
							NumberRead   ));
				}
				else
				{
					mTBOX_PRINT((kTBOX_LF, "=====>   VERIFY   ERROR:   %u   errors   out   of   %u   bytes\n",
							ErrorCounter, NumberRead   ));
				}
			}

			/*   Program   Bank   BankNo, Block   TestBlock   */

			for(   i = 0; i < TestSize; i++   )
			{
				WritArray[i] = (U8)(TestSize   -   i);
				ReadArray[i] = (U8)i;
			}
			mTBOX_PRINT((kTBOX_LF, "Calling   STFLASH_Write()   Bank   %u, .......\n", BankNo   ));
			ReturnError = STFLASH_Write(   TDAL_FLA_STFlash_Hndl,
					TestOffset,
					WritArray,
					TestSize,
					&NumberWrit   );
			if(   ReturnError != ST_NO_ERROR   )
			{
				p_TDAL_FLA_DriverErrorReport(   &ErrorCount, ReturnError, ST_NO_ERROR   );
			}
			mTBOX_PRINT((kTBOX_LF, "NumberActuallyWritten   reported   as   %d   bytes\n",
					NumberWrit   ));

			if(   NumberWrit > 0   )
			{
				mTBOX_PRINT((kTBOX_LF, "Calling   STFLASH_Read()   Bank   %u, ........\n", BankNo   ));
				ReturnError = STFLASH_Read(   TDAL_FLA_STFlash_Hndl,
						TestOffset,
						ReadArray,
						TestSize,
						&NumberRead   );
				if(   ReturnError != ST_NO_ERROR   )
				{
					p_TDAL_FLA_DriverErrorReport(   &ErrorCount, ReturnError, ST_NO_ERROR   );
				}
				mTBOX_PRINT((kTBOX_LF, "NumberActuallyRead   reported   as   %d   bytes\n",
						NumberRead   ));
				Match = TRUE;
				if(   (   ReturnError == ST_NO_ERROR   )   &&
						(   NumberRead == TestSize   )   )
				{
					ErrorCounter = 0;
					for(   i = 0; i < NumberRead; i++   )
					{
						if(   WritArray[i] != ReadArray[i]   )
						{
							Match = FALSE;
							mTBOX_PRINT((kTBOX_LF, "Mismatch   =>   Write/ReadArray[%2d] = %02X   /   %02X\n",
									i, (U32)WritArray[i], (U32)ReadArray[i]   ));
							ErrorCounter++;
						}
					}

					if(   Match   )
					{
						mTBOX_PRINT((kTBOX_LF, "Write   data   verified   with   Read   of   %d   bytes\n",
								NumberRead   ));
					}
					else
					{
						mTBOX_PRINT((kTBOX_LF, "=====>   VERIFY   ERROR:   %u   errors   out   of   %u   bytes\n",
								ErrorCounter, NumberRead   ));
					}
				}
			}
			STOS_TaskDelay(   1000   );
		}

		mTBOX_PRINT((kTBOX_LF, "------------------------------------------------------------\n"   ));

		if(   BankNo == 0   )
		{
			/*   Close   Bank   0   */

			mTBOX_PRINT((kTBOX_LF, "Calling   STFLASH_Close()   Bank0   ..........\n"   ));
			ReturnError = STFLASH_Close(   TDAL_FLA_STFlash_Hndl   );
			p_TDAL_FLA_DriverErrorReport(   &ErrorCount, ReturnError, ST_NO_ERROR   );

			break;
		}
	}

	/*   Term   Bank   0   */

	TermParams.ForceTerminate = TRUE;
	mTBOX_PRINT((kTBOX_LF, "Calling   STFLASH_Term()   Bank   0   .........\n"   ));
	ReturnError = STFLASH_Term(   SYS_FlashName[0], &TermParams   );
	p_TDAL_FLA_DriverErrorReport(   &ErrorCount, ReturnError, ST_NO_ERROR   );

	return   ErrorCount;
}
#endif   /*   0/1   for   TEST   FUNCTION   */

