/******************************************************************************
 *            COPYRIGHT 2001 IWEDIA TECHNOLOGIES        *
 ******************************************************************************
 *
 * MODULE NAME: TDAL FLA
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_fla.h $
 *      $Rev: 66 $
 *      $Date: 2008-10-14 10:29:19 +0200 (mar., 14 oct. 2008) $
 *
 * PUBLIC
 *
 * DESCRIPTION - Header of the TDAL Flash
 *
 *****************************************************************************/

#ifndef _TDAL_FLA_H_
#define _TDAL_FLA_H_

/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include <crules.h>

/*******************************************************/
/*       Defines              */
/*******************************************************/

#define TDAL_FLA_READ_ENABLE    0x01
#define TDAL_FLA_WRITE_ENABLE    0x02


/*******************************************************/
/*       Macros               */
/*******************************************************/

/*******************************************************/
/*       Typedefs              */
/*******************************************************/
typedef enum 
{
   eTDAL_FLA_NO_ERROR,
   eTDAL_FLA_NOT_DONE,
   eTDAL_FLA_BAD_PARAMETER,
   eTDAL_FLA_ERROR
}tTDAL_FLA_ErrorCode;


typedef struct
{
   uint32_t   StartAddress;
   uint32_t   EndAddress;
   uint8_t Rights;   
}tTDAL_FLA_Partition;

/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/
tTDAL_FLA_ErrorCode TDAL_FLA_Init ( void );
tTDAL_FLA_ErrorCode TDAL_FLA_SetPartition (uint8_t nbPartition, tTDAL_FLA_Partition* partition );
tTDAL_FLA_ErrorCode TDAL_FLA_Term ( void );
const char *    TDAL_FLA_APIRevisionGet(void );
const char *    TDAL_FLA_PlatformRevisionGet(void );
tTDAL_FLA_ErrorCode TDAL_FLA_Open ( void );
tTDAL_FLA_ErrorCode TDAL_FLA_Close ( void );
tTDAL_FLA_ErrorCode TDAL_FLA_Erase ( uint32_t Address, uint32_t NumberToErase);
uint32_t TDAL_FLA_Read      ( uint32_t Address, uint8_t* Buffer, uint32_t NumberToRead);
uint32_t TDAL_FLA_Write      ( uint32_t Address, uint8_t* Buffer, uint32_t NumberToWrite);
uint32_t TDAL_FLA_GetBlockAddress  ( uint32_t BlockNo );
uint32_t TDAL_FLA_GetBlockSize   ( uint32_t BlockNo );
uint32_t  TDAL_FLA_GetBlockNumber   ( uint32_t Address );
uint32_t  TDAL_FLA_GetNumberOfBlocks(void);



#ifdef __cplusplus
}
#endif


#endif /*_TDAL_FLA_H_*/
