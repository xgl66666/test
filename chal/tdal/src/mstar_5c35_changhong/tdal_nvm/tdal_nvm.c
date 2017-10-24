/********************************************************************************
 *                       COPYRIGHT 2004 IWEDIA TECHNOLOGIES                           *
 ********************************************************************************
 *
 * MODULE NAME: TDAL_NVM
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_nvm/tdal_nvm.c $
 *            $Rev: 414 $
 *            $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION : TDAL NVM implementation
 *
 *****************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include "crules.h"
#include "string.h"            /*   for   memcpy   */

#include "tdal_common.h"
#include "tdal_fla.h"
#include "tdal_nvm.h"
#include "flash_cfg.h"
#include "tbox.h"


/********************************************************
 *              Defines                                 *
 ********************************************************/

/********************************************************
 *              Local File Variables (LOCAL)            *
 ********************************************************/
mTBOX_SET_MODULE(eTDAL_NVM);
LOCAL   bool               TDAL_NVM_Initialized = FALSE;
LOCAL   bool               TDAL_NVM_Opened = FALSE;
LOCAL   TDAL_mutex_id         TDAL_NVM_Mutex;
LOCAL   tFLASH_CFG_Location   TDAL_NVM_Location;

/*===========================================================================
 *
 * TDAL_NVM_Init
 *
 * Parameters:
 *
 * Description:
 *      initialize TDAL_NVM.
 *
 * Returns:
 *      eTDAL_NVM_NO_ERROR       no error
 *      eTDAL_NVM_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_NVM_ErrorCode   TDAL_NVM_Init   (void)
{
   bool ret;

   mTBOX_FCT_ENTER("TDAL_NVM_Init");

   /*   Test   if   TDAL_NVM   has   already   been   initialised   */
   if   (TDAL_NVM_Initialized == TRUE)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Init:   NVM   is   already   initialized\n"));
      mTBOX_RETURN(eTDAL_NVM_NOT_DONE);
   }
   
   /*   create   a   mutex   to   protect   access   to   the   flash   memory   */
   TDAL_CreateMutex(   &TDAL_NVM_Mutex   );

   /*   Not   done   here*/
   /*TDAL_FLA_Init();*/

   /*   set   flag   "Initialized"   to   TRUE   */
   TDAL_NVM_Initialized = TRUE;

   ret = FLASH_CFG_GetNvmLocation(&TDAL_NVM_Location);
   mTBOX_ASSERTm(ret != FALSE, "FLASH_CFG_GetNVMLocation should return a valid value");

   mTBOX_TRACE((kTBOX_NIV_1, "TDAL_NVM_Init   succeed\n"));
   mTBOX_RETURN(eTDAL_NVM_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_NVM_Term
 *
 * Parameters:
 *
 * Description:
 *      terminate TDAL_NVM.
 *
 * Returns:
 *      eTDAL_NVM_NO_ERROR       no error
 *      eTDAL_NVM_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_NVM_ErrorCode   TDAL_NVM_Term   (void)
{
   mTBOX_FCT_ENTER("TDAL_NVM_Term");

   /*   Test   if   TDAL_NVM   is   still   initialised   */
   if   (TDAL_NVM_Initialized == FALSE)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Term:   NVM   is   not   initialized   or   already   terminated\n"));
      mTBOX_RETURN(eTDAL_NVM_NOT_DONE);
   }

   /*   delete   the   mutex   protecting   the   access   to   the   flash   memory   */
   TDAL_DeleteMutex(   TDAL_NVM_Mutex   );

   /*   Not   done   here*/
   /*TDAL_FLA_Term();*/

   /*   set   flag   Initialized   to   FALSE   */
   TDAL_NVM_Initialized = FALSE;
   TDAL_NVM_Opened = FALSE;

   mTBOX_TRACE((kTBOX_NIV_1, "TDAL_NVM_Term   succeed\n"));
   mTBOX_RETURN(eTDAL_NVM_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_NVM_Open
 *
 * Parameters:
 *
 * Description:
 *      Allow the access to the flash memory via TDAL_NVM functions
 *
 * Returns:
 *      eTDAL_NVM_NO_ERROR       no error
 *      eTDAL_NVM_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_NVM_ErrorCode   TDAL_NVM_Open(void)
{
   mTBOX_FCT_ENTER("TDAL_NVM_Open");

   /*   Test   if   TDAL_NVM   is   initialised   */
   if   (TDAL_NVM_Initialized == FALSE)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Open:   NVM   is   not   initialized\n"));
      mTBOX_RETURN(eTDAL_NVM_NOT_DONE);
   }

   /*   Test   if   TDAL_NVM   has   already   been   open   */
   if   (TDAL_NVM_Opened == TRUE)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Open:   NVM   is   already   opened\n"));
      mTBOX_RETURN(eTDAL_NVM_NOT_DONE);
   }

   /*   Not   done   here   */
   /*if   (TDAL_FLA_Open() != eTDAL_FLA_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Open:   Flash   open   error\n"));
      mTBOX_RETURN(eTDAL_NVM_NOT_DONE);
   }*/
   TDAL_NVM_Opened = TRUE;

   mTBOX_RETURN(eTDAL_NVM_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_NVM_Close
 *
 * Parameters:
 *
 * Description:
 *      Close access to the Flash bank via TDAL_NVM functions. Any subsequent
 *      calls to TDAL_NVM_Read or TDAL_NVM_Write operations will require a new
 *      TDAL_NVM_Open call.
 *
 * Returns:
 *      eTDAL_NVM_NO_ERROR       no error
 *      eTDAL_NVM_NOT_DONE       command not done
 *
 *===========================================================================*/
tTDAL_NVM_ErrorCode   TDAL_NVM_Close   (   void   )
{
   mTBOX_FCT_ENTER("TDAL_NVM_Close");

   /*   Test   if   TDAL_NVM   is   still   open   */
   if   (TDAL_NVM_Opened == FALSE)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Close:   NVM   is   not   already   opened\n"));
      mTBOX_RETURN(eTDAL_NVM_NOT_DONE);
   }

   /*   not   done   here   */
   /*if   (TDAL_FLA_Close() != eTDAL_FLA_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Close:   Flash   close   error\n"));
      mTBOX_RETURN(eTDAL_NVM_NOT_DONE);
   }*/
   TDAL_NVM_Opened = FALSE;

   mTBOX_RETURN(eTDAL_NVM_NO_ERROR);
}

/*===========================================================================
 *
 * TDAL_NVM_Read
 *
 * Parameters:
 *
 * Description:
 *      The contents of the Flash memory starting at 'Address' and of
 *      length 'NumberToRead' will be copied into 'Buffer'.
 *
 * Returns:
 *      Number of bytes that have been successfully read from the flash
 *
 *===========================================================================*/
uint32_t   TDAL_NVM_Read   (
   uint32_t   Address,    /*   I:   Address   offset   to   the   flash   memory   area   to   be   read   */
   uint32_t   NumberToRead, /*   I:   Number   of   bytes   to   read   */
   uint8_t*   Buffer         /*   I:   Buffer   where   to   get   data   */
)
{
   uint32_t   NumberRead;

   mTBOX_FCT_ENTER("TDAL_NVM_Read");
   
   /*   Test   if   good   parameters   */
   if   (   (Buffer == NULL)   ||
         (NumberToRead == 0)   )
   {
      mTBOX_RETURN(0);
   }

   /*   Test   if   TDAL_NVM   is   open   */
   if   (TDAL_NVM_Opened == FALSE)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Read:   NVM   is   not   opened\n"));
      mTBOX_RETURN(0);
   }
   
   /*   test   input   values   */
   if   (Address+NumberToRead > TDAL_NVM_Location.size   ||   Address > TDAL_NVM_Location.size)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Read:   out   of   range\n"));
      mTBOX_RETURN(0);
   }

   /*   acquire   the   mutex   used   to   protect   access   to   the   flash   memory   */
   TDAL_LockMutex(TDAL_NVM_Mutex);

   NumberRead = TDAL_FLA_Read(Address + TDAL_NVM_Location.addressOffset, Buffer, NumberToRead);

   /*   release   the   mutex   */
   TDAL_UnlockMutex(TDAL_NVM_Mutex);

   mTBOX_RETURN(NumberRead);
}


/*===========================================================================
 *
 * TDAL_NVM_Write
 *
 * Parameters:
 *
 * Description:
 *      The content of the memory in Buffer and of length NumberToWrite will
 *      be copied to the flash starting at Address.
 *
 * Returns:
 *      Number of bytes that have been successfully written to the flash
 *
 *===========================================================================*/
uint32_t   TDAL_NVM_Write(
   uint32_t   Address,    /*   I:   Address   offset   to   the   flash   memory   area   to   be   written   */
   uint32_t   NumberToWrite, /*   I:   Number   of   bytes   to   write   */
   uint8_t*   Buffer         /*   I:   Buffer   where   to   get   data   */
)
{
   uint32_t            NumberWritten;
   uint16_t               BlockNumber;
   uint32_t            BaseAddress;
   uint32_t            BlockSize;
   uint8_t   *            p_BlockBuffer;
   uint32_t            NumberRead;
   tTDAL_FLA_ErrorCode      FlashError;
   uint32_t            Offset;

   mTBOX_FCT_ENTER("TDAL_NVM_Write");

   mTBOX_TRACE((kTBOX_NIV_1, "[TDAL_NVM_Write] Address = %x, NumberOfBytes = %x, Buffer = %p", Address, NumberToWrite, Buffer));


   /*   Test   if   TDAL_NVM   is   open   */
   if   (TDAL_NVM_Opened == FALSE)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   NVM   is   not   opened\n"));
      mTBOX_RETURN(0);
   }

   /*   test   input   values   */
   if   (Address+NumberToWrite > TDAL_NVM_Location.size   ||   Address > TDAL_NVM_Location.size)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   out   of   range\n"));
      mTBOX_RETURN(0);
   }
   
   if (Buffer == NULL)
   {
	   mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Buffer is null"));
	   mTBOX_RETURN(0);
   }

   Address   +=   TDAL_NVM_Location.addressOffset;
   
   /*   get   the   block   number   from   the   address   */
   BlockNumber = TDAL_FLA_GetBlockNumber(   Address   );
   
   /*   get   the   base   address   of   the   BlockNumber   */
   BaseAddress = TDAL_FLA_GetBlockAddress(   BlockNumber   );
   
   /*   get   the   block   size   */
   BlockSize = TDAL_FLA_GetBlockSize(   BlockNumber   );
   
   /*   allocate   a   p_BlockBuffer   with   block   size   for   reading   */
   p_BlockBuffer = TDAL_Malloc(   BlockSize   );
   if   (p_BlockBuffer == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   cannot   allocate   internal   buffer\n"));
      mTBOX_RETURN(0);
   }
   
   /*   acquire   the   mutex   used   to   protect   access   to   the   flash   memory   */
   TDAL_LockMutex(   TDAL_NVM_Mutex   );

   /*   read   current   block   and   saved   it   in   the   p_BlockBuffer   */
   NumberRead = TDAL_FLA_Read(BaseAddress, p_BlockBuffer, BlockSize);
   if   (NumberRead != BlockSize)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   TDAL_FLA_Read   failed\n"));
      TDAL_Free(   p_BlockBuffer   );
      TDAL_UnlockMutex(   TDAL_NVM_Mutex   );
      mTBOX_RETURN(0);
   }

   Offset = Address   -   BaseAddress;

   if(memcmp(   (uint8_t   *)   (p_BlockBuffer+Offset)   ,
            (uint8_t   *)   (Buffer)   ,
            NumberToWrite   )==0)
   {
      TDAL_Free(   p_BlockBuffer   );
      TDAL_UnlockMutex(   TDAL_NVM_Mutex   );
      mTBOX_RETURN(NumberToWrite);
   }
   
   /*   erase   the   block   before   write   */
   FlashError = TDAL_FLA_Erase(   BaseAddress   , BlockSize   );
   if   (FlashError != eTDAL_FLA_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   TDAL_FLA_Erase   failed\n"));
      TDAL_Free(   p_BlockBuffer   );
      TDAL_UnlockMutex(   TDAL_NVM_Mutex   );
      mTBOX_RETURN(0);
   }
   
   /*   copy   data   from   Buffer   to   p_BlockBuffer   */
   memcpy(   (uint8_t   *)   (p_BlockBuffer+Offset)   ,
         (uint8_t   *)   (Buffer)   ,
         NumberToWrite   );
   
   /*   copy   data   from   p_BlockBuffer   to   the   flash   */
   NumberWritten = TDAL_FLA_Write(BaseAddress, p_BlockBuffer, BlockSize);
   if   (NumberWritten != BlockSize)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   TDAL_FLA_Write   failed\n"));
      TDAL_Free(   p_BlockBuffer   );
      TDAL_UnlockMutex(   TDAL_NVM_Mutex   );
      mTBOX_RETURN(0);
   }

   /*   free   p_BlockBuffer   */
   TDAL_Free(   p_BlockBuffer   );

   /*   release   the   mutex   */
   TDAL_UnlockMutex(   TDAL_NVM_Mutex   );

   mTBOX_RETURN(NumberToWrite);
}
