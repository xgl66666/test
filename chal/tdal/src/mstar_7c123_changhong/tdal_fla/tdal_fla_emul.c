/*******************************************************************************
 *              COPYRIGHT 2007 IWEDIA TECHNOLOGIES                 *
 *******************************************************************************
 *
 * MODULE NAME: TDAL_FLA
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_fla/tdal_fla_emul.c $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description: TDAL FLASH driver implementation
 *
 ******************************************************************************/

/********************************************************/
/*        Includes                 */
/********************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>      /*   for   memcpy/memset   functions   */
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/file.h>
#include <sys/stat.h>


#include "crules.h"
#include "tbox.h"

#if(TDAL_FLASH_DEBUG==0)
#undef   __DEBUG_TRACE__
#endif

#include "tdal_common.h"
#include "tdal_fla.h"


/********************************************************/
/*        Defines                   */
/********************************************************/

#define   kTDAL_COMMON_ASSERT   1

#if   0   /*   for   stworkbench   */
#define   kTDAL_FLA_EMUL_FLASH_DATA_PATH1      "yp/tdal"
#define   kTDAL_FLA_EMUL_FLASH_DATA_PATH2      "yp/tdal/fla"
#define   kTDAL_FLA_EMUL_FLASH_DATA_FILE_NAME   "yp/tdal/fla/emul-flash"
#endif

#if   1
#define   kTDAL_FLA_EMUL_FLASH_DATA_PATH1      "tdal"
#define   kTDAL_FLA_EMUL_FLASH_DATA_PATH2      "tdal/fla"
#define   kTDAL_FLA_EMUL_FLASH_DATA_FILE_NAME   "tdal/fla/emul-flash"
#endif

#define   kTDAL_FLA_BLOCK_SIZE_1M      0x100000
#define   kTDAL_FLA_BLOCK_SIZE_512K   0x80000
#define   kTDAL_FLA_BLOCK_SIZE_256K   0x40000
#define   kTDAL_FLA_BLOCK_SIZE_128K   0x20000
#define   kTDAL_FLA_BLOCK_SIZE_64K   0x10000
#define   kTDAL_FLA_BLOCK_SIZE_32K   0x08000
#define   kTDAL_FLA_BLOCK_SIZE_16K   0x04000
#define   kTDAL_FLA_BLOCK_SIZE_8K      0x02000
#define   kTDAL_FLA_BLOCK_SIZE_4K      0x01000

#ifndef   kTDAL_FLA_NB_BLOCKS
#define   kNUM_BLOCKS            14
#else
#define   kNUM_BLOCKS            kTDAL_FLA_NB_BLOCKS
#endif

#define   kTDAL_FLA_BLOCK_SIZE      kTDAL_FLA_BLOCK_SIZE_128K

#define   kTDAL_ERASE_SIZE      256

/*******************************************************/
/*           Macros                        */
/*******************************************************/

#if   (kTDAL_FLA_ASSERT == 1)
#define   mTDAL_FLA_ASSERT(x)   if(!(x))   {TBOX_Print(kTBOX_LF,"Assertion   failed   in   FILE   %s, LINE   %d\n", __FILE__, __LINE__); TDAL_Abort();}
#else
#define   mTDAL_FLA_ASSERT      mTBOX_ASSERT
#endif

/********************************************************/
/*           Typedefs                      */
/********************************************************/

typedef   struct   {
   uint32_t   Address;
   uint32_t   Length;
}   tTDAL_FLA_BlockInfo;

/********************************************************/
/*           Global Variables (GLOBAL/IMPORT)       */
/********************************************************/


/********************************************************/
/*           Local Module Variables (MODULE)      */
/********************************************************/


/********************************************************/
/*           Local File Variables (LOCAL)        */
/********************************************************/
LOCAL   pthread_mutex_t   TDAL_FLA_MutexHandle=PTHREAD_MUTEX_INITIALIZER;

#ifdef   TDAL_BLOC_INFO_DYNAMIC
LOCAL   tTDAL_FLA_BlockInfo   *      TDAL_FLA_BlockInfoList;
#else
LOCAL   tTDAL_FLA_BlockInfo      TDAL_FLA_BlockInfoList[kNUM_BLOCKS];
#endif

LOCAL   bool      TDAL_FLA_alreadyInitialized = false;
LOCAL   bool      TDAL_FLA_Opened = false;
LOCAL   uint32_t   TDAL_FLA_FlashSize = 0; /*   Size   of   Flash   in   bytes   */
LOCAL   uint8_t   TDAL_FLA_FlashNbBlock = kNUM_BLOCKS;


LOCAL   tTDAL_FLA_Partition*   TDAL_FLA_Partition = NULL;
LOCAL   uint8_t        TDAL_FLA_NbPartition = 0;
LOCAL   bool            TDAL_FLA_isPartitionSet = false;
LOCAL   uint8_t        TDAL_FLA_INIT_BYTE = 0xFF;


LOCAL   int8_t   TDAL_FLA_Emul_Flash_fd = -1;

mTBOX_SET_MODULE(eTDAL_FLA);

/********************************************************/
/*           Local Functions Declarations (LOCAL)   */
/********************************************************/

LOCAL   bool   TDAL_FLA_CheckRights(uint32_t   address, uint32_t   size, uint8_t   rights);

LOCAL   uint32_t   tdalFlashRead(uint32_t   Address, uint8_t*   Buffer, uint32_t   NumberToRead);
LOCAL   uint32_t   tdalFlashWrite(uint32_t   Address, uint8_t*   Buffer,   uint32_t   NumberToWrite);

LOCAL   tTDAL_FLA_ErrorCode   tdalFlashBlockErase(uint8_t   blockNumber);

/********************************************************/
/*           Functions Definitions (LOCAL/GLOBAL)   */
/********************************************************/


/*===========================================================================
 *
 * TDAL_FLA_Init
 *
 * Parameters:
 *
 * Description:
 *      initialize an instance of the Flash device driver
 *
 * Returns:
 *      eTDAL_FLA_NO_ERROR       no error
 *      eTDAL_FLA_BAD_PARAMETER  wrong argument value
 *      eTDAL_FLA_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_FLA_ErrorCode   TDAL_FLA_Init(void)
{
   uint32_t        address;
   uint8_t            i;
   struct   stat        status;
   int            returnValue;

   mTBOX_FCT_ENTER("TDAL_FLA_Init");

   /*   Test   if   TDAL   FLASH   has   already   been   initialized   */
   if(TDAL_FLA_alreadyInitialized)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_FLA_Init()   -   Flash   already   initialized\n"));
      mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
   }

   returnValue = stat(kTDAL_FLA_EMUL_FLASH_DATA_PATH2, &status);

   if   (returnValue   !=0)
   {
      /*   the   directory   does   not   exist:   create   it   */
      returnValue = mkdir(kTDAL_FLA_EMUL_FLASH_DATA_PATH1, 666);
      mTDAL_FLA_ASSERT(returnValue == 0);
      returnValue = mkdir(kTDAL_FLA_EMUL_FLASH_DATA_PATH2, 666);
      mTDAL_FLA_ASSERT(returnValue == 0);
   }

#ifdef   TDAL_BLOC_INFO_DYNAMIC
   TDAL_FLA_BlockInfoList = TDAL_Malloc(   kNUM_BLOCKS   *
                        sizeof(tTDAL_FLA_BlockInfo)   );
#endif

   address = /*BaseAddress*/   0;
   for   (i = 0   ; i < TDAL_FLA_FlashNbBlock   ; i++)
   {
      TDAL_FLA_BlockInfoList[i].Address = address;
      TDAL_FLA_BlockInfoList[i].Length = kTDAL_FLA_BLOCK_SIZE;
      address   +=   TDAL_FLA_BlockInfoList[i].Length;
   }

   /*   Compute   the   total   flash   size   */
   TDAL_FLA_FlashSize = address;

   TDAL_FLA_isPartitionSet = false;
   TDAL_FLA_alreadyInitialized = true;

   mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
}


/*===========================================================================
 *
 * TDAL_FLA_SetPartition
 *
 * Parameters:
 *      Pointer on allocated and filled tTDAL_FLA_Partition array
 * Description:
 *
 *
 * Returns:
 *      eTDAL_FLA_NO_ERROR       no error
 *      eTDAL_FLA_BAD_PARAMETER  wrong argument value
 *      eTDAL_FLA_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_FLA_ErrorCode   TDAL_FLA_SetPartition   (   uint8_t   nbPartition,
                        tTDAL_FLA_Partition*   partition   )
{
   mTBOX_FCT_ENTER("TDAL_FLA_SetPartition");

   if   ((partition == NULL) || (nbPartition == 0))
   {
      mTBOX_RETURN(eTDAL_FLA_BAD_PARAMETER);
   }

   if   (TDAL_FLA_Partition   !=   NULL)
   {
      TDAL_Free(TDAL_FLA_Partition);
      TDAL_FLA_Partition = NULL;
   }

   TDAL_FLA_Partition = (tTDAL_FLA_Partition*)
      TDAL_Malloc(nbPartition   *   sizeof(tTDAL_FLA_Partition));
   if   (TDAL_FLA_Partition == NULL)
   {
      TDAL_FLA_NbPartition = 0;
      TDAL_FLA_isPartitionSet = false;
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }

   memcpy(TDAL_FLA_Partition,
      partition,
      nbPartition   *   sizeof(tTDAL_FLA_Partition));

   TDAL_FLA_NbPartition = nbPartition;
   TDAL_FLA_isPartitionSet = true;

   mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_FLA_Term
 *
 * Parameters:
 *
 * Description:
 *      terminate an instance of the driver opened by TDAL_FLA_Init
 *
 * Returns:
 *      eTDAL_FLA_NO_ERROR       no error
 *      eTDAL_FLA_BAD_PARAMETER  wrong argument value
 *      eTDAL_FLA_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_FLA_ErrorCode   TDAL_FLA_Term   (   void   )
{
   mTBOX_FCT_ENTER("TDAL_FLA_Term");

   /*   Check   if   the   flash   has   already   been   initialized   */
   if   (!TDAL_FLA_alreadyInitialized)
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_FLA_Term   ()   -   Flash   not   initialized\n"));
      mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
   }

   /*   Destroy   the   mutex   */
/*   if(pthread_mutex_destroy(&TDAL_FLA_MutexHandle)!=0)
   {
      perror("pthread_mutex_destroy   :\n");
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Term   ()   -   MutexDestroy   error"));
      mTBOX_RETURN(eTDAL_FLA_NOT_DONE);
   }*/

   /*   Free   the   buffers   allocated   in   TDAL_FLA_Init   */
#ifdef   TDAL_BLOC_INFO_DYNAMIC
   TDAL_Free(TDAL_FLA_BlockInfoList);
   TDAL_FLA_BlockInfoList = NULL;
#endif

   TDAL_FLA_alreadyInitialized = false;

   mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_FLA_Open
 *
 * Parameters:
 *
 * Description:
 *      Allow the access to the flash memory
 *
 * Returns:
 *      eTDAL_FLA_NO_ERROR       no error
 *      eTDAL_FLA_BAD_PARAMETER  wrong argument value
 *      eTDAL_FLA_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_FLA_ErrorCode   TDAL_FLA_Open   (   void   )
{
   int      returnValue;
   uint32_t   i;
   struct   stat   stats;

   mTBOX_FCT_ENTER("TDAL_FLA_Open");

   if(TDAL_FLA_alreadyInitialized==false)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Open()   -   Error   :   TDAL_FLA   not   initialized   \n"));
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }

   memset(&stats, 0, sizeof(stats));

   TDAL_FLA_Emul_Flash_fd = open(kTDAL_FLA_EMUL_FLASH_DATA_FILE_NAME, O_RDWR);

   if   (TDAL_FLA_Emul_Flash_fd >= 0)
   {
      returnValue = fstat(TDAL_FLA_Emul_Flash_fd, &stats);
   }

   if   ((TDAL_FLA_Emul_Flash_fd < 0) || ((uint32_t)stats.st_size   !=   TDAL_FLA_FlashSize))
   {
      mTBOX_TRACE((kTBOX_NIV_WARNING,"TDAL_FLA_Open()   -   Erasing   emul-flash...\n"));

      if   (TDAL_FLA_Emul_Flash_fd >= 0)
      {
      close(TDAL_FLA_Emul_Flash_fd);
      }

      /*   Open   the   fake   flash   device   in   order   to   erase   it   */
      TDAL_FLA_Emul_Flash_fd = open(kTDAL_FLA_EMUL_FLASH_DATA_FILE_NAME,
                     O_CREAT|O_RDWR|O_TRUNC,
                     S_IRWXU|S_IRWXG|S_IRWXO);
      if   (TDAL_FLA_Emul_Flash_fd   <0)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Open()   -   Error   :   Can't   open   file   \"%s\\n", kTDAL_FLA_EMUL_FLASH_DATA_FILE_NAME));
      mTBOX_RETURN(eTDAL_FLA_ERROR);
      }

#if   0
      /*   Initialize   the   Flash   */
      for(i = 0; i < TDAL_FLA_FlashSize; i++)
      {
      if(write(TDAL_FLA_Emul_Flash_fd, &TDAL_FLA_INIT_BYTE, 1)   !=   1)
      {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Open()   -   Error   :   Unable   to   write   in   the   \"emul-flash\"   file\n"));
        mTBOX_RETURN(eTDAL_FLA_ERROR);
      }
      }
#else
      for   (i = 0   ; i < TDAL_FLA_FlashNbBlock   ; i++)
      {
      if   (tdalFlashBlockErase(i)   !=   eTDAL_FLA_NO_ERROR)
      {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Open()   -   Error   in   tdalFlashBlockErase\n"));
        mTBOX_RETURN(eTDAL_FLA_ERROR);
      }
      }
#endif
   }

   TDAL_FLA_Opened = true;

   mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_FLA_Close
 *
 * Parameters:
 *
 * Description:
 *      Close access to the Flash bank. Any subsequent calls to TDAL_FLA_Erase,
 *      TDAL_FLA_Read and TDAL_FLA_Write operations will require a new TDAL_FLA_Open
 *      call.
 *
 * Returns:
 *      eTDAL_FLA_NO_ERROR       no error
 *      eTDAL_FLA_BAD_PARAMETER  wrong argument value
 *      eTDAL_FLA_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_FLA_ErrorCode   TDAL_FLA_Close   (   void   )
{
   mTBOX_FCT_ENTER("TDAL_FLA_Close");

   if(!TDAL_FLA_Opened)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Close()   -   Error   :   TDAL_FLA   not   opened   \n"));
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }

   if(TDAL_FLA_isPartitionSet)
   {
      TDAL_Free(TDAL_FLA_Partition);
      TDAL_FLA_Partition = NULL;
   }


   /*   Close   the   fake   flash   device   :   file   "emul-flash"   */
   if   (close(TDAL_FLA_Emul_Flash_fd) < 0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Close()   -   Error   :   Can't   close   file\n"));
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }

   TDAL_FLA_Emul_Flash_fd = -1;

   TDAL_FLA_Opened = false;

   mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_FLA_Erase
 *
 * Parameters:
 *      Address      address offset to the flash area to be erased
 *      NumberToErase   number of bytes to be erased
 *
 * Description:
 *      Erase NumberToErase bytes beginning at memory Address offset relative
 *      to the base address. The Address parameter must be on a block boundary
 *      and the NumberToErase must match that block size.
 *
 * Returns:
 *      eTDAL_FLA_NO_ERROR       no error
 *      eTDAL_FLA_BAD_PARAMETER  wrong argument value
 *      eTDAL_FLA_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_FLA_ErrorCode   TDAL_FLA_Erase   (
   uint32_t   Address,   /*   I:   address   offset   to   the   flash   area   to   be   erased   */
   uint32_t   NumberToErase   /*   I:   number   of   bytes   to   be   erased   */
)
{
   uint16_t   blockNumber;
   uint32_t   startingBlockAddress;
   uint32_t   blockSize;

   mTBOX_FCT_ENTER("TDAL_FLA_Erase");
   /*
   mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Erase()   -   Address:%lx   -   NumberToErase:%lx\n", Address, NumberToErase));
   */

   /*   check   input   parameters   */
   if   (TDAL_FLA_isPartitionSet)
      if   (!TDAL_FLA_CheckRights(Address, NumberToErase, TDAL_FLA_WRITE_ENABLE))
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Erase()   -   Error   :   No   Right   Erase   0x%x\r\n", Address));
      mTBOX_RETURN(eTDAL_FLA_NOT_DONE);
      }


   /*   check   Address   parameter   */
   blockNumber = TDAL_FLA_GetBlockNumber(Address);
   startingBlockAddress = TDAL_FLA_GetBlockAddress(blockNumber);
   if   (Address   !=   startingBlockAddress)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Erase()   -   Error   :   Address   is   not   a   starting   block   address\n"));
      mTBOX_RETURN(eTDAL_FLA_BAD_PARAMETER);
   }

   /*   check   NumberToErase   parameter   */
   blockSize = TDAL_FLA_GetBlockSize(blockNumber);
   if   (blockSize   !=   NumberToErase)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Erase()   -   Error   :   NumberToErase   doesn't   match   the   block   size\n"));
      mTBOX_RETURN(eTDAL_FLA_BAD_PARAMETER);
   }

   /*   test   if   access   to   flash   memory   is   allowed   */
   if(!TDAL_FLA_Opened)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Erase()   -   Error   :   Flash   not   opened\n"));
      mTBOX_RETURN(eTDAL_FLA_NOT_DONE);
   }

   /*   protection   against   multiple   calls   */
   if(pthread_mutex_lock(&TDAL_FLA_MutexHandle)!=0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Erase()   -   Error   :   Error   with   pthread_mutex_lock\n"));
      mTBOX_RETURN(eTDAL_FLA_NOT_DONE);
   }

   /*   erase   */
   if(eTDAL_FLA_NO_ERROR!=tdalFlashBlockErase(blockNumber))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Erase()   -   Error   :   Erase   didn't   complete   successfully\n"));
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }

   /*   release   the   mutex   */
   if(pthread_mutex_unlock(&TDAL_FLA_MutexHandle)!=0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Erase()   :   Error   with   pthread_mutex_unlock\n"));
      mTBOX_RETURN(eTDAL_FLA_NOT_DONE);
   }


   mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_FLA_Read
 *
 * Parameters:
 *      Address      Address offset to the flash area to be read
 *      Buffer       Buffer where to copy the data read
 *      NumberToRead   Number of bytes to read

 *
 * Description:
 *      The content of the flash memory starting at Address and of length
 *      NumberToRead will be copied to Buffer. The Address + NumberToRead value
 *      is checked to ensure that read attempt is not made beyond the end of
 *      flash memory.
 *
 * Returns:
 *      Number of bytes that have been successfully read into the buffer
 *
 *===========================================================================*/
uint32_t   TDAL_FLA_Read(
   uint32_t   Address,   /*   I:   Address   offset   to   the   flash   area   to   be   read   */
   uint8_t*   Buffer,   /*   I:   Buffer   where   to   copy   the   data   read   */
   uint32_t   NumberToRead   /*   I:   Number   of   bytes   to   read   */
)
{
   uint32_t   NbByteRead=0;

   mTBOX_FCT_ENTER("TDAL_FLA_Read");

   if   (TDAL_FLA_isPartitionSet)
   {
      if   (!TDAL_FLA_CheckRights(Address, NumberToRead, TDAL_FLA_READ_ENABLE))
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"Read   No   Right   0x%08lxh\n", Address));
      mTBOX_RETURN(0);
      }
   }

   /*   check   input   parameters   */
   if   ((Address > TDAL_FLA_FlashSize)   ||
      ((Address+NumberToRead) > TDAL_FLA_FlashSize))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Read()   -   Error   :   Bad   input   parameter\n"));
      mTBOX_RETURN(0);
   }

   /*   test   if   access   to   flash   memory   is   allowed   */
   if(!TDAL_FLA_Opened)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Read()   -   Error   :   Flash   not   opened\n"));
      mTBOX_RETURN(0);
   }

   /*   read   the   buffer   */
   /*   protection   against   multiple   calls   */
   if(pthread_mutex_lock(&TDAL_FLA_MutexHandle)!=0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Read()   -   Error   :   with   pthread_mutex_lock\n"));
      mTBOX_RETURN(0);
   }

   NbByteRead=tdalFlashRead(Address, Buffer, NumberToRead);

   /*   release   the   mutex   */
   if(pthread_mutex_unlock(&TDAL_FLA_MutexHandle)!=0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Erase()   -   Error   :   with   pthread_mutex_unlock\n"));
      mTBOX_RETURN(0);
   }

   mTBOX_RETURN(NbByteRead);
}



/*===========================================================================
 *
 * TDAL_FLA_Write
 *
 * Parameters:
 *
 * Description:
 *      The content of the memory in Buffer and of length NumberToWrite will
 *      be copied to the flash starting at Address. The result of a write
 *      operation is undefined if the block to which data are written is not
 *      previously erased.
 *
 * Returns:
 *      Number of bytes that have been successfully written to the flash
 *
 *===========================================================================*/
uint32_t   TDAL_FLA_Write   (
   uint32_t   Address,   /*   I:   Address   offset   to   the   flash   memory   area   to   be   written   */
   uint8_t*   Buffer,   /*   I:   Buffer   where   to   get   data   */
   uint32_t   NumberToWrite   /*   I:   Number   of   bytes   to   write   */
)
{
   uint32_t   NbByteWritten = 0;

   mTBOX_FCT_ENTER("TDAL_FLA_Write");

   /*   check   input   parameters   */
   if   ((Address > TDAL_FLA_FlashSize)   ||
      ((Address+NumberToWrite) > TDAL_FLA_FlashSize))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Write()   -   Error   :   Bad   input   parameter\n"));
      mTBOX_RETURN(0);
   }

   /*   test   if   access   to   flash   memory   is   allowed   */
   if(!TDAL_FLA_Opened)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Write()   -   Error   :   Flash   not   opened\n"));
      mTBOX_RETURN(0);
   }

   /*   protection   against   multiple   calls   */
   if(pthread_mutex_lock(&TDAL_FLA_MutexHandle)!=0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Write()   -   Error   with   pthread_mutex_lock\n"));
      mTBOX_RETURN(0);
   }

   /*   Check   if   flash   area   is   protected   */
   if   (TDAL_FLA_isPartitionSet)
   {
      if   (!TDAL_FLA_CheckRights(Address, NumberToWrite, TDAL_FLA_WRITE_ENABLE))
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"No   Right   Write   0x%08lXh\n", Address));

      /*   release   the   mutex   */
      if(pthread_mutex_unlock(&TDAL_FLA_MutexHandle)!=0)
      {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Erase()   -   Error   with   pthread_mutex_unlock\n"));
      }

      mTBOX_RETURN(0);
      }
   }

   /*   write   data   into   the   flash   area   */
   NbByteWritten = tdalFlashWrite(Address, Buffer, NumberToWrite);

   /*   release   the   mutex   */
   if(pthread_mutex_unlock(&TDAL_FLA_MutexHandle)!=0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"TDAL_FLA_Erase()   -   Error   with   pthread_mutex_unlock\n"));
      mTBOX_RETURN(0);
   }

   mTBOX_RETURN(NbByteWritten);
}


/*===========================================================================
 *
 * TDAL_FLA_GetBlockAddress
 *
 * Parameters:
 *      BlockNo    Number of the block
 *
 * Description:
 *      Return the starting address of the block
 *
 * Returns:
 *      address of the block selected
 *
 *===========================================================================*/
uint32_t   TDAL_FLA_GetBlockAddress(
   uint8_t   BlockNo      /*   I:   Number   of   the   block   */
)
{
   mTBOX_FCT_ENTER("TDAL_FLA_GetBlockAddress");

   mTBOX_RETURN(   TDAL_FLA_BlockInfoList[BlockNo].Address   );
}

/*===========================================================================
 *
 * TDAL_FLA_GetBlockSize
 *
 * Parameters:
 *      BlockNo    Number of the block
 *
 * Description:
 *      Return the size of the block
 *
 * Returns:
 *      size of the block
 *
 *===========================================================================*/
uint32_t   TDAL_FLA_GetBlockSize(
   uint8_t   BlockNo      /*   I:   Number   of   the   block   */
)
{
   mTBOX_FCT_ENTER("TDAL_FLA_GetBlockSize");

   mTBOX_RETURN(   TDAL_FLA_BlockInfoList[BlockNo].Length   );
}

/*===========================================================================
 *
 * TDAL_FLA_GetBlockNumber
 *
 * Parameters:
 *      Address    address to find
 *
 * Description:
 *      Return the number of the block that contains the address given in parameter
 *
 * Returns:
 *      number of the block, 0xff if address is out of range
 *
 *===========================================================================*/
uint16_t   TDAL_FLA_GetBlockNumber(
   uint32_t   Address   /*   I:   Address   to   find   */
)
{
   uint16_t   Block = (uint8_t)-1;

   mTBOX_FCT_ENTER("TDAL_FLA_GetBlockNumber");

   /*   find   the   block   this   address   is   in   */
   for   (   Block = 0; Block < kNUM_BLOCKS; Block++   )
   {
      if   ((   Address >= TDAL_FLA_BlockInfoList[Block].Address   )   &&
      (   Address < (   TDAL_FLA_BlockInfoList[Block].Address
               +TDAL_FLA_BlockInfoList[Block].Length   )   ))
      {
      break;
      }
   }

   mTBOX_RETURN(   Block   );
}


/*===========================================================================
 *
 * TDAL_FLA_GetNumberOfBlocks
 *
 * Parameters:
 *      none
 *
 * Description:
 *      Return the number of all blocks
 *
 * Returns:
 *      number of blocks
 *
 *===========================================================================*/
uint16_t   TDAL_FLA_GetNumberOfBlocks(void)
{
   mTBOX_FCT_ENTER("TDAL_FLA_GetNumberOfBlocks");

   mTBOX_RETURN(TDAL_FLA_FlashNbBlock);
}


/*===========================================================================
 *
 * tdalFlashRead
 *
 * Parameters:
 *      Address      Address offset to the flash area to be read
 *      Buffer       Buffer where to copy the data read
 *      NumberToRead   Number of bytes to read
 *
 * Description:
 *      Read in the fake flash file :
 *      The content of the flash memory starting at Address and of length
 *      NumberToRead will be copied into Buffer.
 *
 * Returns:
 *      Number of byte successfully read
 *
 *===========================================================================*/
LOCAL   uint32_t   tdalFlashRead(
   uint32_t   Address,   /*   I:   Address   offset   to   the   flash   area   to   be   read   */
   uint8_t*   Buffer,   /*   I:   Buffer   where   to   copy   the   data   read   */
   uint32_t   NumberToRead   /*   I:   Number   of   bytes   to   read   */
   )
{
   uint32_t   NbByteRead;

   mTBOX_FCT_ENTER("tdalFlashRead");

   if(lseek(TDAL_FLA_Emul_Flash_fd, Address, SEEK_SET)<0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"tdalFlashRead()   :   Error   with   fonction   \"lseek\""));
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }

   if((NbByteRead=read(TDAL_FLA_Emul_Flash_fd, Buffer, NumberToRead))<0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"tdalFlashRead()   :   Unable   to   read   in   the   \"emul-flash\"   file\n"));
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }

   mTBOX_RETURN   (NbByteRead);
}



/*===========================================================================
 *
 * tdalFlashWrite
 *
 * Parameters:
 *      Address      Address offset to the flash area to be read
 *      Buffer       Buffer where to copy the data read
 *      NumberToWrite   Number of bytes to write
 *
 * Description:
 *      Write in the fake flash file :
 *      The content of the Buffer will be copied into the flash memory
 *      starting at Address and of length NumberToWrite
 *
 * Returns:
 *      Number of byte successfully written
 *
 *===========================================================================*/

LOCAL   uint32_t   tdalFlashWrite(
   uint32_t   Address,   /*   I:   Address   offset   to   the   flash   area   to   be   read   */
   uint8_t*   Buffer,   /*   I:   Buffer   where   to   copy   the   data   read   */
   uint32_t   NumberToWrite   /*   I:   Number   of   bytes   to   read   */
   )
{
   uint32_t   NbByteWritten;
   int      returnValue;

   mTBOX_FCT_ENTER("tdalFlashWrite");

   if(lseek(TDAL_FLA_Emul_Flash_fd, Address, SEEK_SET)<0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"tdalFlashWrite()   :   Error   with   fonction   \"lseek\""));
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }

   if((NbByteWritten=write(TDAL_FLA_Emul_Flash_fd, Buffer, NumberToWrite))<0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"tdalFlashWrite()   :   Unable   to   write   in   the   \"emul-flash\"   file\n"));
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }

   returnValue = fsync(TDAL_FLA_Emul_Flash_fd);
   mTDAL_FLA_ASSERT(returnValue == 0);

   mTBOX_RETURN   (NbByteWritten);
}


/*===========================================================================
 *
 * TDAL_FLA_CheckRights
 *
 * Parameters:
 *
 * Description:
 *
 *
 * Returns:
 *
 *===========================================================================*/
LOCAL   bool   TDAL_FLA_CheckRights(uint32_t   address, uint32_t   size, uint8_t   rights)
{
   uint32_t   lastAddress = address   +   size   -   1;
   uint8_t   index;

   mTBOX_FCT_ENTER("TDAL_FLA_CheckRights");

   if   (TDAL_FLA_Partition == NULL)
   {
      mTBOX_RETURN   (true);
   }

   if   ((address < TDAL_FLA_Partition[0].StartAddress)   ||
      (lastAddress > TDAL_FLA_Partition[TDAL_FLA_NbPartition   -   1].EndAddress))
   {
      /*   do   not   write   outside   partitions   */
      mTBOX_RETURN   (false);
   }

   for   (index = 0; index < TDAL_FLA_NbPartition; index++)
   {
      if   ((address >= TDAL_FLA_Partition[index].StartAddress)   &&
      (address <= TDAL_FLA_Partition[index].EndAddress))
      {
      if   ((TDAL_FLA_Partition[index].Rights   &   rights)   !=   rights)
      {
        /*   this   partition   is   protected   */
        mTBOX_RETURN   (false);
      }
      else   if   (lastAddress > TDAL_FLA_Partition[index].EndAddress)
      {
        /*   the   range   exceeds   this   partition:   continue   */
        address = TDAL_FLA_Partition[index].EndAddress   +   1;
      }
      else
      {
        mTBOX_RETURN   (true);
      }
      }
   }

   mTBOX_RETURN   (false);
}


/*===========================================================================
 *
 * tdalFlashEraseAllBlock
 *
 * Parameters:
 *      None
 * Description:
 *      Erase all the block
 *
 * Returns:
 *
 *===========================================================================*/

LOCAL   tTDAL_FLA_ErrorCode   tdalFlashBlockErase(uint8_t   blockNumber)
{
   uint32_t   remainingSize;
   int32_t   nbBytesToWrite;
   uint32_t   nbBytesWritten;
   uint8_t   dataToWrite[kTDAL_ERASE_SIZE];
   int      returnValue;

   mTBOX_FCT_ENTER("tdalFlashBlockErase");

   /*   move   the   read/write   file   offset   to   the   bloc   start   address   */
   if   (lseek(TDAL_FLA_Emul_Flash_fd,
        TDAL_FLA_BlockInfoList[blockNumber].Address,
        SEEK_SET) < 0)
   {
      perror("lseek\n");
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"tdalFlashBlockErase()   :   Error   with   fonction   \"lseek\"\n"));
      mTBOX_RETURN(eTDAL_FLA_ERROR);
   }

   /*   initialize   the   data   buffer   to   the   init   value   */
   memset(dataToWrite, TDAL_FLA_INIT_BYTE, kTDAL_ERASE_SIZE);

   /*   the   whole   "flash"   block   must   be   erased   */
   remainingSize = TDAL_FLA_GetBlockSize(blockNumber);

   /*   the   size   of   data   to   be   written   */
   nbBytesToWrite   =
      (kTDAL_ERASE_SIZE < remainingSize)   ?   kTDAL_ERASE_SIZE   :   remainingSize;

   while(nbBytesToWrite > 0)
   {
      nbBytesWritten = write(TDAL_FLA_Emul_Flash_fd,
                 dataToWrite,
                 nbBytesToWrite);

      if   (nbBytesWritten < 1)
      {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"tdalFlashBlockErase()   :   Unable   to   write   in   the   \"emul-flash\"   file\n"));
      mTBOX_RETURN(eTDAL_FLA_ERROR);
      }

      /*   update   sizes   */
      remainingSize -= nbBytesWritten;
      if   (remainingSize < (uint32_t)nbBytesToWrite)
      {
      nbBytesToWrite = remainingSize;
      }
   }

   returnValue = fsync(TDAL_FLA_Emul_Flash_fd);
   mTDAL_FLA_ASSERT(returnValue == 0);

   mTBOX_RETURN(eTDAL_FLA_NO_ERROR);
}
#if   defined(futarque_hybrid) || defined(idtv7109) || defined(dtt7167)
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
uint32_t   TDAL_FLAm_JumpWrite   (   uint32_t   Address,
                 uint8_t*   Buffer,
                 uint32_t   NumberToWrite   )
{
   /*uint32_t      Blockbyteswritten;*/

   mTBOX_FCT_ENTER("TDAL_FLAm_JumpWrite");
   mTBOX_RETURN(0);
}
#endif

