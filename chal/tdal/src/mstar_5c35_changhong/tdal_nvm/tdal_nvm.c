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
#define TDAL_NVM_REDUNDANCY_LEVEL 2
#define TDAL_NVM_CRC32_SIZE sizeof(uint32_t)
#define TDAL_NVM_VERSION_AGE_SIZE sizeof(uint8_t)
#define TDAL_NVM_VERSION_MINIMAL 0x01
#define TDAL_NVM_VERSION_MAXIMAL 0xFF
#define TDAL_NVM_MAX_SIZE_INBYTES 0x10000
#define TDAL_ABS(a,b) (((a)>(b))?((a)-(b)):((b)-(a)))

typedef struct _tFlashNVMArea
{
    uint32_t            uiFlashOffset;
    uint32_t            uiByteSize;
    uint32_t            uiCRC32;
    uint8_t             ucVersion;
} tFlashNVMArea;

/********************************************************
 *              Local File Variables (LOCAL)            *
 ********************************************************/
mTBOX_SET_MODULE(eTDAL_NVM);
LOCAL   bool               TDAL_NVM_Initialized = FALSE;
LOCAL   bool               TDAL_NVM_Opened = FALSE;
LOCAL   TDAL_mutex_id         TDAL_NVM_Mutex;
LOCAL   tFlashNVMArea   stAreaArray[TDAL_NVM_REDUNDANCY_LEVEL] = {{0}, {0}};

/********************************************************
 *          Local functions                             *
 ********************************************************/
LOCAL bool TDALi_NVM_InitValidArea();
LOCAL bool TDALi_NVM_GetValidReadProps(tFLASH_CFG_Location *pstArea, uint32_t Address, uint32_t uiReadBytes);
LOCAL bool TDALi_NVM_GetValidWriteProps(tFLASH_CFG_Location *pstArea, uint32_t Address, uint32_t uiWriteBytes);
LOCAL void TDALi_NVM_ValidateWriteBuffer(uint8_t *pucBuffer, uint32_t uiSize);
LOCAL void TDALi_NVM_UpdateValidWriteProp(uint8_t *pucBuffer);
LOCAL uint8_t TDALi_NVM_GetAreaIdx(const bool bOld);

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
   tFLASH_CFG_Location TDAL_FLASH_Location;

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

   /* Get area from flash configuration for kind of NVRAM reserved area */
   ret = FLASH_CFG_GetNvmLocation(&TDAL_FLASH_Location);
   stAreaArray[0].uiFlashOffset = TDAL_FLASH_Location.addressOffset;
   if (TDAL_NVM_MAX_SIZE_INBYTES < TDAL_FLASH_Location.size)
   {
       stAreaArray[0].uiByteSize    = TDAL_NVM_MAX_SIZE_INBYTES;
   }
   else
   {
       stAreaArray[0].uiByteSize    = TDAL_FLASH_Location.size;
   }
   /* Get spare NVRAM area from flash configuration in this case it will be one block before
    * application read only resources start address */
   FLASH_CFG_GetFlashAreaLocation(eFLASH_CFG_FLASH_AREA_RESOURCES_PARTITION_MIRROR_1, &TDAL_FLASH_Location);
   stAreaArray[1].uiFlashOffset = TDAL_FLASH_Location.addressOffset - TDAL_NVM_MAX_SIZE_INBYTES;
   if (TDAL_NVM_MAX_SIZE_INBYTES < TDAL_FLASH_Location.size)
   {
       stAreaArray[1].uiByteSize    = TDAL_NVM_MAX_SIZE_INBYTES;
   }
   else
   {
       stAreaArray[1].uiByteSize    = TDAL_FLASH_Location.size;
   }
   mTBOX_ASSERTm(ret != FALSE, "FLASH_CFG_GetNVMLocation should return a valid value");

   TDALi_NVM_InitValidArea();

   mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Init   succeed\n"));
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
   tFLASH_CFG_Location  stValidFlash;

   mTBOX_FCT_ENTER("TDAL_NVM_Read");
   
   /*   Test   if   good   parameters   */
   if (Buffer == NULL || NumberToRead == 0)
   {
      mTBOX_RETURN(0);
   }

   /*   Test   if   TDAL_NVM   is   open   */
   if   (TDAL_NVM_Opened == FALSE)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Read:   NVM   is   not   opened\n"));
      mTBOX_RETURN(0);
   }
   
   /* Prepare valid area properties for read */
   if (TDALi_NVM_GetValidReadProps(&stValidFlash, Address, NumberToRead) == false)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Read:   out   of   range\n"));
      mTBOX_RETURN(0);
   }

   /*   acquire   the   mutex   used   to   protect   access   to   the   flash   memory   */
   TDAL_LockMutex(TDAL_NVM_Mutex);

   NumberRead = TDAL_FLA_Read(Address + stValidFlash.addressOffset, Buffer, NumberToRead);

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
    uint32_t            NumberWritten = 0;
    uint8_t*            p_BlockBuffer = NULL;
    uint32_t            NumberRead = 0;
    uint32_t            Offset = 0;
    tTDAL_FLA_ErrorCode FlashError = eTDAL_FLA_NO_ERROR;
    tFLASH_CFG_Location stValidFlash = {0};

   mTBOX_FCT_ENTER("TDAL_NVM_Write");

    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_NVM_Write] Address = %x, NumberOfBytes = %x, Buffer = %p\n", Address, NumberToWrite, Buffer));

   /*   Test   if   TDAL_NVM   is   open   */
   if   (TDAL_NVM_Opened == FALSE)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   NVM   is   not   opened\n"));
      mTBOX_RETURN(0);
   }

   if (Buffer == NULL)
   {
	   mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Buffer is null"));
	   mTBOX_RETURN(0);
   }

    /* Prepare valid area properties for read */
    if (TDALi_NVM_GetValidReadProps(&stValidFlash, Address, NumberToWrite) == false)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write: out of range\n"));
        mTBOX_RETURN(0);
    }
   
   /*   allocate   a   p_BlockBuffer   with   block   size   for   reading   */
    p_BlockBuffer = TDAL_Malloc(stValidFlash.size);
   if   (p_BlockBuffer == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   cannot   allocate   internal   buffer\n"));
      mTBOX_RETURN(0);
   }
   
   /*   acquire   the   mutex   used   to   protect   access   to   the   flash   memory   */
    TDAL_LockMutex(TDAL_NVM_Mutex);

   /*   read   current   block   and   saved   it   in   the   p_BlockBuffer   */
    NumberRead = TDAL_FLA_Read(stValidFlash.addressOffset,
                               p_BlockBuffer,
                               stValidFlash.size);
    if (NumberRead != stValidFlash.size)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   TDAL_FLA_Read   failed\n"));
        TDAL_Free(p_BlockBuffer);
        TDAL_UnlockMutex(TDAL_NVM_Mutex);
      mTBOX_RETURN(0);
   }
    /* Check whether write operation is needed, we want to spare flash from writing */
    if (memcmp((uint8_t *) (p_BlockBuffer+Address), Buffer, NumberToWrite) == 0)
    {
        TDAL_Free(p_BlockBuffer);
        TDAL_UnlockMutex(TDAL_NVM_Mutex);
      mTBOX_RETURN(NumberToWrite);
   }
    /* Prepare valid area properties for write */
    if (TDALi_NVM_GetValidWriteProps(&stValidFlash, Address, NumberToWrite) == false)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write: out of range\n"));
        mTBOX_RETURN(0);
    }

    /* Prepare flash block before write in it */
    FlashError = TDAL_FLA_Erase(stValidFlash.addressOffset, stValidFlash.size);
   if   (FlashError != eTDAL_FLA_NO_ERROR)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   TDAL_FLA_Erase   failed\n"));
        TDAL_Free(p_BlockBuffer);
        TDAL_UnlockMutex(TDAL_NVM_Mutex);
      mTBOX_RETURN(0);
   }
   
   /*   copy   data   from   Buffer   to   p_BlockBuffer   */
    memcpy((uint8_t*)(p_BlockBuffer+Address), Buffer, NumberToWrite);

    /* Whole block buffer to validate with CRC and version */
    TDALi_NVM_ValidateWriteBuffer(p_BlockBuffer, stValidFlash.size);
   
   /*   copy   data   from   p_BlockBuffer   to   the   flash   */
    NumberWritten = TDAL_FLA_Write(stValidFlash.addressOffset, p_BlockBuffer, stValidFlash.size);
    if   (NumberWritten != stValidFlash.size)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   TDAL_FLA_Write   failed\n"));
        TDAL_Free(p_BlockBuffer);
        TDAL_UnlockMutex(TDAL_NVM_Mutex);
      mTBOX_RETURN(0);
   }

    /* Update write properties */
    TDALi_NVM_UpdateValidWriteProp(p_BlockBuffer);

   /*   free   p_BlockBuffer   */
    TDAL_Free(p_BlockBuffer);

   /*   release   the   mutex   */
    TDAL_UnlockMutex(TDAL_NVM_Mutex);

   mTBOX_RETURN(NumberToWrite);
}

/*===========================================================================
 *
 * TDALi_NVM_InitValidArea
 *
 * Parameters:
 *
 * Description:
 *      Initialization of all NVM areas based on available flash blocks
 *      where should reside the data which are versioned(latest/oldest) and
 *      validated by CRC32
 *
 * Returns:
 *
 *===========================================================================*/
bool TDALi_NVM_InitValidArea()
{
    uint8_t     ucCntArea;
    uint8_t     *pucBuffer = NULL;
    uint8_t     *pucValidBuffer = NULL;
    uint32_t    uiCRCOffset = 0;
    uint32_t    uiCRC32 = 0;
    uint32_t    uiVersionOffset = 0;
    uint8_t     ucMaxVersion = 0;
    struct{uint8_t bValid; uint8_t ucVersion;} stCheckArray[TDAL_NVM_REDUNDANCY_LEVEL];
    bool        bValid = TRUE;

    mTBOX_FCT_ENTER("TDALi_NVM_GetInitValidArea");

    for(ucCntArea = 0; ucCntArea < TDAL_NVM_REDUNDANCY_LEVEL; ucCntArea++)
    {
        if (pucBuffer == NULL)
        {
            pucBuffer = TDAL_Malloc(stAreaArray[ucCntArea].uiByteSize);
        }
        /* Read one NVM entity */
        memset(pucBuffer, 0, stAreaArray[ucCntArea].uiByteSize);
        TDAL_FLA_Read(stAreaArray[ucCntArea].uiFlashOffset,
                      pucBuffer,
                      stAreaArray[ucCntArea].uiByteSize);
        uiCRCOffset     = stAreaArray[ucCntArea].uiByteSize - TDAL_NVM_CRC32_SIZE;
        uiVersionOffset = uiCRCOffset - TDAL_NVM_VERSION_AGE_SIZE;
        /* Validate NVM entity */
        memcpy(&uiCRC32, &pucBuffer[uiCRCOffset], sizeof(uiCRC32));
        bValid      = TDALm_CRC32_Check(pucBuffer,
                                        stAreaArray[ucCntArea].uiByteSize - TDAL_NVM_CRC32_SIZE,
                                        uiCRC32);

        stAreaArray[ucCntArea].ucVersion    = pucBuffer[uiVersionOffset];
        stCheckArray[ucCntArea].bValid      = bValid;
        stCheckArray[ucCntArea].ucVersion   = pucBuffer[uiVersionOffset];
        if (bValid)
        {
            stAreaArray[ucCntArea].uiCRC32      = uiCRC32;

            if (stAreaArray[ucCntArea].ucVersion > ucMaxVersion)
            {
                ucMaxVersion = stAreaArray[ucCntArea].ucVersion;
            }
            if (pucValidBuffer != NULL)
            {
                TKEL_Free(pucValidBuffer);
            }

            pucValidBuffer = pucBuffer;
            pucBuffer = NULL;
        }
    }

    /* If invalid NVM area reset it to default */
    for (ucCntArea = 0; ucCntArea < TDAL_NVM_REDUNDANCY_LEVEL; ucCntArea++)
    {
        if (!stCheckArray[ucCntArea].bValid)
        {
            if (ucMaxVersion == TDAL_NVM_VERSION_MINIMAL)
            {
                pucBuffer = pucValidBuffer;
                pucBuffer[uiVersionOffset] = TDAL_NVM_VERSION_MAXIMAL;
                pucValidBuffer = NULL;
            }
            else if (ucMaxVersion == 0)
            {
            memset(pucBuffer, 0, stAreaArray[ucCntArea].uiByteSize);
                pucBuffer[uiVersionOffset] = ucMaxVersion + 1;
            }
            else
            {
                pucBuffer = pucValidBuffer;
                pucBuffer[uiVersionOffset] = ucMaxVersion - 1;
                pucValidBuffer = NULL;
            }
            uiCRC32 = TDALm_CRC32_Calculate(pucBuffer, stAreaArray[ucCntArea].uiByteSize - TDAL_NVM_CRC32_SIZE);
            memcpy(&pucBuffer[uiCRCOffset], &uiCRC32, sizeof(uiCRC32));

            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "%s ucVersion=%03d\n", __FUNCTION__, pucBuffer[uiVersionOffset]));
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "%s uiCRC32=0x%02X\n", __FUNCTION__, pucBuffer[uiCRCOffset]));
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "%s uiCRC32=0x%02X\n", __FUNCTION__, pucBuffer[uiCRCOffset + 1]));
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "%s uiCRC32=0x%02X\n", __FUNCTION__, pucBuffer[uiCRCOffset + 2]));
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "%s uiCRC32=0x%02X\n", __FUNCTION__, pucBuffer[uiCRCOffset + 3]));

            TDAL_FLA_Erase(stAreaArray[ucCntArea].uiFlashOffset, stAreaArray[ucCntArea].uiByteSize);
            TDAL_FLA_Write(stAreaArray[ucCntArea].uiFlashOffset, pucBuffer, stAreaArray[ucCntArea].uiByteSize);
            stAreaArray[ucCntArea].ucVersion = pucBuffer[uiVersionOffset];
        }
    }

    if (pucBuffer)
    {
        TDAL_Free(pucBuffer);
    }
    if (pucValidBuffer)
    {
        TKEL_Free(pucValidBuffer);
    }
    mTBOX_RETURN(true);
}
/*===========================================================================
 *
 * TDALi_NVM_GetValidReadProps
 *
 * Parameters:
 *      pointer of read area properties
 *      address of data in NVM(relative to beginning of NVM)
 *      size in bytes to be written
 * Description:
 *      Getting valid read parameters for write operation to NVM by finding
 *      the area(NVM) with latest data
 *
 *
 * Returns:
 *
 *===========================================================================*/
bool TDALi_NVM_GetValidReadProps(
        tFLASH_CFG_Location *pstArea,
        uint32_t Address,
        uint32_t uiReadBytes)
{
    uint8_t     ucCntArea;
    uint8_t     ucVersion = 0;
    uint32_t    uiCRC32Offset = 0;
    uint32_t    uiVersionOffset = 0;
    uint32_t    uiFlashAddress = 0;
    uint32_t    uiBlockAddress = 0;
    uint16_t    usBlockIdx = 0;
    bool        bRead = true;
    uint8_t     ucIdx = 0;

    mTBOX_FCT_ENTER("TDALi_NVM_GetValidReadProps");

    if (pstArea == 0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Bad parameter pstArea\n"));
        mTBOX_RETURN(false);
    }
    memset(pstArea, 0, sizeof(tFLASH_CFG_Location));

    /* Get valid area index */
    ucIdx = TDALi_NVM_GetAreaIdx(bRead);

    /* When valid check age and choose older */
    pstArea->addressOffset  = stAreaArray[ucIdx].uiFlashOffset;

    /* Calculate final validity indicators */
    uiVersionOffset = stAreaArray[ucIdx].uiByteSize - TDAL_NVM_CRC32_SIZE - TDAL_NVM_VERSION_AGE_SIZE;

    /* Check whether address and size of bytes to read exceed valid area */
    if (uiReadBytes + Address > uiVersionOffset)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Reading parameters exceed valid area\n"));
        mTBOX_RETURN(false);
    }

    /* Compute real FLASH address and size of block */
    uiFlashAddress = Address + pstArea->addressOffset;

    /* get the block number from the address */
    usBlockIdx = TDAL_FLA_GetBlockNumber(uiFlashAddress);

    /* get the base address of the BlockNumber */
    uiBlockAddress = TDAL_FLA_GetBlockAddress(usBlockIdx);
    if (uiBlockAddress != pstArea->addressOffset)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Something really went wrong\n"));
    }
    else
    {
        /* get the block size */
        pstArea->size = TDAL_FLA_GetBlockSize(usBlockIdx);
    }

    mTBOX_RETURN(true);
}
/*===========================================================================
 *
 * TDALi_NVM_GetValidWriteProps
 *
 * Parameters:
 *      pointer of write area properties
 *      address of data in NVM(relative to beginning of NVM)
 *      size in bytes to be written
 * Description:
 *      Getting valid write parameters for write operation to NVM by finding
 *      the area(NVM) with oldest data
 *
 *
 * Returns:
 *
 *===========================================================================*/
bool TDALi_NVM_GetValidWriteProps(
        tFLASH_CFG_Location *pstArea,
        uint32_t Address,
        uint32_t uiWriteBytes)
{
    uint8_t     ucCntArea;
    uint8_t     ucVersion = 0xFF;
    uint32_t    uiCRC32Offset = 0;
    uint32_t    uiVersionOffset = 0;
    uint32_t    uiFlashAddress = 0;
    uint32_t    uiBlockAddress = 0;
    uint16_t    usBlockIdx = 0;
    bool        bRead = false;
    uint8_t     ucIdx = 0;

    mTBOX_FCT_ENTER("TDALi_NVM_GetValidWriteProps");

    if (pstArea == 0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Bad parameter pstArea\n"));
        mTBOX_RETURN(true);
    }
    memset(pstArea, 0, sizeof(tFLASH_CFG_Location));

    /* Get valid area index */
    ucIdx = TDALi_NVM_GetAreaIdx(bRead);

    /* When valid check age and choose older */
    pstArea->addressOffset  = stAreaArray[ucIdx].uiFlashOffset;

    /* Calculate final validity indicators */
    uiVersionOffset = stAreaArray[ucIdx].uiByteSize - TDAL_NVM_CRC32_SIZE - TDAL_NVM_VERSION_AGE_SIZE;

    /* Check whether address and size of bytes to read exceed valid area */
    if (uiWriteBytes + Address > uiVersionOffset)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Reading parameters exceed valid area\n"));
        mTBOX_RETURN(false);
    }

    /* Compute real FLASH address and size of block */
    uiFlashAddress = Address + pstArea->addressOffset;

    /* get the block number from the address */
    usBlockIdx = TDAL_FLA_GetBlockNumber(uiFlashAddress);

    /* get the base address of the BlockNumber */
    uiBlockAddress = TDAL_FLA_GetBlockAddress(usBlockIdx);
    if (uiBlockAddress != pstArea->addressOffset)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Something really went wrong\n"));
    }
    else
    {
        /* get the block size */
        pstArea->size = TDAL_FLA_GetBlockSize(usBlockIdx);
    }

    mTBOX_RETURN(true);
}
/*===========================================================================
 *
 * TDALi_NVM_ValidateWriteBuffer
 *
 * Parameters:
 *      pointer of write buffer before storing in NVM
 *      size of data
 * Description:
 *      Validates buffer data after setting the proper version by CRC32
 *      Stamps CRC32 as last 4-bytes
 * Returns:
 *
 *===========================================================================*/

void TDALi_NVM_ValidateWriteBuffer(
        uint8_t *pucBuffer,
        uint32_t uiSize)
{
    uint8_t     ucCntArea;
    uint32_t    uiCRC32 = 0;
    uint8_t     ucVersion = 0xFF;
    uint32_t    uiCRC32Offset = 0;
    uint32_t    uiVersionOffset = 0;
    uint8_t     ucYoungestIdx = 0;
    uint8_t     ucOldestIdx = 0;
    bool        bRead = true;

    mTBOX_FCT_ENTER("TDALi_NVM_ValidateWriteBuffer");

    /* Get valid area index */
    ucYoungestIdx   = TDALi_NVM_GetAreaIdx(bRead);
    ucOldestIdx     = TDALi_NVM_GetAreaIdx(!bRead);

    /* Calculate final validity indicators' address offsets */
    uiCRC32Offset   = stAreaArray[ucOldestIdx].uiByteSize - TDAL_NVM_CRC32_SIZE;
    uiVersionOffset = uiCRC32Offset - TDAL_NVM_VERSION_AGE_SIZE;

    if (stAreaArray[ucYoungestIdx].ucVersion >= stAreaArray[ucOldestIdx].ucVersion &&
        stAreaArray[ucYoungestIdx].ucVersion + 1 >= TDAL_NVM_VERSION_MAXIMAL)
    {
        ucVersion = TDAL_NVM_VERSION_MINIMAL;
    }
    else
    {
        ucVersion = stAreaArray[ucYoungestIdx].ucVersion + 1;
    }
    pucBuffer[uiVersionOffset] = ucVersion;
    /* Calculate CRC on data buffer */
    uiCRC32 = TDALm_CRC32_Calculate(pucBuffer, uiSize - TDAL_NVM_CRC32_SIZE);
    if (uiCRC32 == 0xFFFFFFFF || uiCRC32 == 0x0)
    {
        if (++ucVersion == TDAL_NVM_VERSION_MAXIMAL)
        {
            ucVersion = TDAL_NVM_VERSION_MINIMAL;
        }
        pucBuffer[uiVersionOffset] = ucVersion;
        uiCRC32 = TDALm_CRC32_Calculate(pucBuffer, uiSize - TDAL_NVM_CRC32_SIZE);
    }
    /* Set valid data CRC */
    memcpy(&pucBuffer[uiCRC32Offset], &uiCRC32, sizeof(uiCRC32));

    mTBOX_RETURN;
}

/*===========================================================================
 *
 * TDALi_NVM_UpdateValidWriteProp
 *
 * Parameters:
 *      pointer of written buffer over NVM
 * Description:
 *      Updates operative properties of area over which the latest NVM write
 *      operation was successfully finished
 *
 * Returns:
 *
 *===========================================================================*/
void TDALi_NVM_UpdateValidWriteProp(uint8_t *pucBuffer)
{
    uint8_t     ucVersion = 0xFF;
    uint32_t    uiCRC32Offset = 0;
    uint32_t    uiVersionOffset = 0;
    uint8_t     ucIdx = 0;
    bool        bRead = false;

    mTBOX_FCT_ENTER("TDALi_NVM_UpdateValidWriteProp");

    ucIdx = TDALi_NVM_GetAreaIdx(bRead);

    /* Check validity of current area */
    if (stAreaArray[ucIdx].uiCRC32 > 0)
    {
        mTBOX_TRACE((kTBOX_NIV_WARNING, "In RAM CRC32 indicator for NVM area is unset\n"));
    }
    /* Calculate final validity indicators' address offsets */
    uiCRC32Offset   = stAreaArray[ucIdx].uiByteSize - TDAL_NVM_CRC32_SIZE;
    uiVersionOffset = uiCRC32Offset - TDAL_NVM_VERSION_AGE_SIZE;
    stAreaArray[ucIdx].ucVersion = pucBuffer[uiVersionOffset];
    memcpy(&stAreaArray[ucIdx].uiCRC32, &pucBuffer[uiCRC32Offset], TDAL_NVM_CRC32_SIZE);

    mTBOX_TRACE((kTBOX_NIV_1, "Actual NVM stAreaArray[%d] uiCRC32=%d ucVersion=%d\n",
                ucIdx,
                stAreaArray[ucIdx].uiCRC32,
                stAreaArray[ucIdx].ucVersion));

    mTBOX_RETURN;
}
/*===========================================================================
 *
 * TDALi_NVM_GetAreaIdx
 *
 * Parameters:
 *
 * Description:
 *      Gets valid area index depending on bRead parameter if it is for
 *      reading(bRead=true) the index should point to area(NVM) parameters with
 *      latest data, otherwise in case of writing the index should point to
 *      area(NVM) parameters with oldest data.
 *
 * Returns:
 *      uint8_t index of area
 *
 *===========================================================================*/

uint8_t TDALi_NVM_GetAreaIdx(const bool bRead)
{
    uint8_t ucIdx = 0;
    uint8_t ucCntArea;
    uint8_t ucVersionMin = 0xFF;
    uint8_t ucVersionMax = 0x00;
    uint8_t ucVersionDiff = 0x00;
    uint8_t ucYoungestIdx = 0;
    uint8_t ucOldestIdx = 0;

    mTBOX_FCT_ENTER("TDALi_NVM_GetAreaIdx");

    for(ucCntArea = 0; ucCntArea < TDAL_NVM_REDUNDANCY_LEVEL; ucCntArea++)
    {
        if (stAreaArray[ucCntArea].ucVersion > ucVersionMax)
        {
            ucVersionMax = stAreaArray[ucCntArea].ucVersion;
            ucYoungestIdx = ucCntArea;
        }

        if (stAreaArray[ucCntArea].ucVersion < ucVersionMin)
        {
            ucVersionMin = stAreaArray[ucCntArea].ucVersion;
            ucOldestIdx = ucCntArea;
        }
    }
    
    ucVersionDiff = TDAL_ABS(ucVersionMax,ucVersionMin);
    if (ucVersionDiff > TDAL_NVM_REDUNDANCY_LEVEL - 1)
    {
        if (bRead)
        {
            mTBOX_TRACE((kTBOX_NIV_1, "Version cycle reached choose minimal valid version for read IDx=%d\n", ucIdx));
            ucIdx = ucOldestIdx;
        }
        else
        {
            mTBOX_TRACE((kTBOX_NIV_1, "Version cycle reached choose maximal valid version for write IDx=%d\n", ucIdx));
            ucIdx = ucYoungestIdx;
        }
    }
    else
    {
        if (bRead)
        {
            mTBOX_TRACE((kTBOX_NIV_1, "Common choose maximum valid version for read IDx=%d\n", ucIdx));
            ucIdx = ucYoungestIdx;
        }
        else
        {
            mTBOX_TRACE((kTBOX_NIV_1, "Common choose minimum valid version for write IDx=%d\n", ucIdx));
            ucIdx = ucOldestIdx;
        }
    }

    mTBOX_RETURN(ucIdx);
}
