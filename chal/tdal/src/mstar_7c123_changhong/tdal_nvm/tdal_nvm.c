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

LOCAL   bool TDAL_NVM_has_backup_block = FALSE;
LOCAL   uint32_t TDAL_NVM_BK_Location = 0;

#define NVM_BLOCK_SIZE (0x10000)
#define NVM_DATA_SIZE (NVM_BLOCK_SIZE - sizeof(uint32_t))
#define SUPPORT_BACKUP_BLOCK 0 

typedef struct
{
    uint8_t data[NVM_DATA_SIZE];
    uint32_t u32crc;       //CRC32 over data bytes
 
}SPI_NVMInfo;


static uint32_t _crc32 (uint32_t crc, const uint8_t *buf, uint32_t len)
{                 
#define DO1(buf) crc = crc_table[((int)crc ^ (*buf++)) & 0xff] ^ (crc >> 8);
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);
                  
    const static uint32_t  crc_table[] =
    {             
        0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
        0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
        0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
        0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
        0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
        0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
        0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
        0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
        0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
        0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
        0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
        0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
        0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
        0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
        0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
        0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
        0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
        0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
        0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
        0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
        0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
        0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
        0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
        0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
        0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
        0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
        0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
        0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
        0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
        0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
        0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
        0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
        0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
        0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
        0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
        0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
        0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
        0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
        0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,    
        0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
        0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
        0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
        0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
        0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
        0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
        0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
        0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
        0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
        0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
        0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
        0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
        0x2d02ef8dL
    };
    
    crc = crc ^ 0xffffffffL;
    while(len >= 8)
    {    
        DO8(buf);
        len -= 8;
    }    
    
    if(len)
    {    
        do       
        {        
            DO1(buf);
        }        
        while(--len);
    }
    return crc ^ 0xffffffffL;
}

bool _TDAL_NVM_CheckCRC( uint8_t* p_BlockBuffer, bool overWriteCRC)
{
    SPI_NVMInfo *pNVMInfo = (SPI_NVMInfo *)p_BlockBuffer;
    if(TDAL_NVM_has_backup_block == false)
    {
        //mTBOX_TRACE((kTBOX_NIV_CRITICAL, "_TDAL_NVM_CheckBKBlock:   No   Backup   NVM   exist\n"));
        return false;
    }

    uint32_t crc_value = _crc32 (0, p_BlockBuffer, NVM_BLOCK_SIZE-sizeof(uint32_t));
    

    if(pNVMInfo->u32crc != crc_value)
    {
        if(overWriteCRC)
        {
            pNVMInfo->u32crc = crc_value;
        }
   
        return false;
    }
    else
    {
        return true;
    }
}

bool _TDAL_NVM_CheckBKBlock(void)
{
    bool _ret = false;
    uint8_t * p_BlockBuffer;
    SPI_NVMInfo *pNVMInfo = (SPI_NVMInfo *)p_BlockBuffer;
    uint32_t   NumberRead;

    if(TDAL_NVM_has_backup_block == false)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "_TDAL_NVM_CheckBKBlock:   No   Backup   NVM   exist\n"));
        return false;
    }

    p_BlockBuffer = TDAL_Malloc(NVM_BLOCK_SIZE);

    if(p_BlockBuffer == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "_TDAL_NVM_CheckBKBlock:   cannot   allocate   internal   buffer\n"));
        return false;
    }

    NumberRead = TDAL_FLA_Read(TDAL_NVM_BK_Location, p_BlockBuffer, NVM_BLOCK_SIZE); 
    if(NumberRead != NVM_BLOCK_SIZE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "_TDAL_NVM_CheckBKBlock:   TDAL_FLA_Read   failed\n"));
        TDAL_Free(p_BlockBuffer);
        return false; 
    }

    _ret = _TDAL_NVM_CheckCRC(p_BlockBuffer, false);

    if(_ret == false)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "_TDAL_NVM_CheckBKBlock:   NVM   Backup   Block   CRC   CHECK   FAIL\n"));
    }

    /*   free   p_BlockBuffer   */
    TDAL_Free(p_BlockBuffer);
    return _ret;
}

bool _TDAL_NVM_Recovery(bool fromBackupBlock)
{
    uint8_t * p_BlockBuffer;
    uint32_t crc_value = 0;
    SPI_NVMInfo *pNVMInfo = (SPI_NVMInfo *)p_BlockBuffer;
    uint32_t read_addr = TDAL_NVM_Location.addressOffset;
    uint32_t temp_size = 0;
    uint32_t temp_count = 0;
    tTDAL_FLA_ErrorCode      FlashError;
    uint32_t            NumberWritten;
    uint32_t   NumberRead;
    bool _ret = true;

    if(TDAL_NVM_has_backup_block == false)
    {
        return false;
    }

   /*   allocate   a   p_BlockBuffer   with   block   size   for   reading   */
   p_BlockBuffer = TDAL_Malloc(NVM_BLOCK_SIZE);
   if(p_BlockBuffer == NULL)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   cannot   allocate   internal   buffer\n"));
      return false;
   }

    while(read_addr != TDAL_NVM_BK_Location)
    {
        NumberRead = TDAL_FLA_Read(read_addr, p_BlockBuffer, NVM_BLOCK_SIZE); 
        if   (NumberRead != NVM_BLOCK_SIZE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "_TDAL_NVM_Recovery:   TDAL_FLA_Read   failed\n"));
            _ret = false;
            break;
        }
        if( _TDAL_NVM_CheckCRC(p_BlockBuffer, true) == false)
        {
            if(fromBackupBlock)
            {
                //recover from backup
                NumberRead = TDAL_FLA_Read(TDAL_NVM_BK_Location, p_BlockBuffer, NVM_BLOCK_SIZE); 
                if(NumberRead != NVM_BLOCK_SIZE)
                {
                    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "_TDAL_NVM_Recovery:   TDAL_FLA_Read   failed\n"));
                    _ret = false;
                    break;
                }
            }

            /*   erase   the   block   before   write   */ //For Backup Block
            FlashError = TDAL_FLA_Erase( read_addr, NVM_BLOCK_SIZE);
            if(FlashError != eTDAL_FLA_NO_ERROR)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "_TDAL_NVM_Recovery:   TDAL_FLA_Erase   failed\n"));
                _ret = false;
                break;
            }

            NumberWritten = TDAL_FLA_Write(read_addr, p_BlockBuffer, NVM_BLOCK_SIZE);
            if(NumberWritten != NVM_BLOCK_SIZE)
            {
                mTBOX_TRACE((kTBOX_NIV_CRITICAL, "_TDAL_NVM_Recovery:   TDAL_FLA_Write   failed\n"));
                _ret = false;
                break;
            }
            //break; // recovery all block? 
        }
        read_addr += NVM_BLOCK_SIZE;
        temp_size += NVM_BLOCK_SIZE;
        if(temp_size >= TDAL_NVM_Location.size)
        {
            break;
        }
    }
   
    /*   free   p_BlockBuffer   */
    TDAL_Free(p_BlockBuffer);
    return _ret;
}
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

    if(TDAL_NVM_Location.size / NVM_BLOCK_SIZE >= 2 )
    {
#if SUPPORT_BACKUP_BLOCK 
        TDAL_NVM_has_backup_block = TRUE;
        TDAL_NVM_BK_Location = TDAL_NVM_Location.addressOffset + TDAL_NVM_Location.size - NVM_BLOCK_SIZE;
#endif
    }
    else
    {
        TDAL_NVM_has_backup_block = FALSE;
        TDAL_NVM_BK_Location = 0; 
    }

    if( _TDAL_NVM_CheckBKBlock())
    {
        _TDAL_NVM_Recovery(true);
    }
    else
    {
        _TDAL_NVM_Recovery(false);
    }
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
   uint32_t            NumberWritten = 0;
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
   /*   test   input   values   */
   if   (NumberToWrite == 0)
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   NumberToWrite    0\n"));
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
   
   Offset = (Address   -   BaseAddress)%NVM_BLOCK_SIZE;

   if(NumberToWrite > NVM_BLOCK_SIZE)
   {
        NumberWritten += TDAL_NVM_Write( Address+(NVM_BLOCK_SIZE-Offset)-TDAL_NVM_Location.addressOffset, NumberToWrite - (NVM_BLOCK_SIZE-Offset), Buffer+(NVM_BLOCK_SIZE-Offset));
   }  
   NumberToWrite = NumberToWrite%NVM_BLOCK_SIZE;
   if(NumberToWrite == 0)
   {
        NumberToWrite = NVM_BLOCK_SIZE;   
   }
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


   if(memcmp(   (uint8_t   *)   (p_BlockBuffer+Offset)   ,
            (uint8_t   *)   (Buffer)   ,
            NumberToWrite   )==0)
   {
      TDAL_Free(   p_BlockBuffer   );
      TDAL_UnlockMutex(   TDAL_NVM_Mutex   );
      mTBOX_RETURN(NumberToWrite);
   }
   
   /*   copy   data   from   Buffer   to   p_BlockBuffer   */
   memcpy(   (uint8_t   *)   (p_BlockBuffer+Offset)   ,
         (uint8_t   *)   (Buffer)   ,
         NumberToWrite   );
  
   // append CRC
   _TDAL_NVM_CheckCRC(p_BlockBuffer, true);

   if(TDAL_NVM_has_backup_block == true)
   {
       /*   erase   the   block   before   write   */ //For Backup Block
       FlashError = TDAL_FLA_Erase( TDAL_NVM_BK_Location , BlockSize   );
       if   (FlashError != eTDAL_FLA_NO_ERROR)
       {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   TDAL_FLA_Erase   failed\n"));
            TDAL_Free(   p_BlockBuffer   );
            TDAL_UnlockMutex(   TDAL_NVM_Mutex   );
            mTBOX_RETURN(0);
       }

       /*   copy   data   from   p_BlockBuffer   to   the   flash   */ //For Backup Block
       if(TDAL_FLA_Write(TDAL_NVM_BK_Location, p_BlockBuffer, BlockSize) != BlockSize)
       {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   TDAL_FLA_Write   failed\n"));
            TDAL_Free(   p_BlockBuffer   );
            TDAL_UnlockMutex(   TDAL_NVM_Mutex   );
            mTBOX_RETURN(0);
       }
   }

   if(TDAL_NVM_BK_Location != BaseAddress)
   {
     /*   erase   the   block   before   write   */
     FlashError = TDAL_FLA_Erase(   BaseAddress   , BlockSize   );
     if   (FlashError != eTDAL_FLA_NO_ERROR)
     {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   TDAL_FLA_Erase   failed\n"));
        TDAL_Free(   p_BlockBuffer   );
        TDAL_UnlockMutex(   TDAL_NVM_Mutex   );
        mTBOX_RETURN(0);
     }
   
     /*   copy   data   from   p_BlockBuffer   to   the   flash   */
     if(TDAL_FLA_Write(BaseAddress, p_BlockBuffer, BlockSize) != BlockSize) 
     {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_NVM_Write:   TDAL_FLA_Write   failed\n"));
        TDAL_Free(   p_BlockBuffer   );
        TDAL_UnlockMutex(   TDAL_NVM_Mutex   );
        mTBOX_RETURN(0);
     } 
   }
   NumberWritten += NumberToWrite;
   /*   free   p_BlockBuffer   */
   TDAL_Free(   p_BlockBuffer   );

   /*   release   the   mutex   */
   TDAL_UnlockMutex(   TDAL_NVM_Mutex   );

   mTBOX_RETURN(NumberWritten);
}
