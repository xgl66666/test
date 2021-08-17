/******************************************************************************
 *            COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TBOX
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tbox/trunk/src/arch/ST7109/tbox_hard_p.h $
 *      $Rev: 411 $
 *      $Date: 2011-09-23 12:32:35 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION : Private header, TBOX specific to hardware
 *
 *****************************************************************************/

#ifndef TBOX_HARD_P_H
#define TBOX_HARD_P_H

#define TBOXi_DEVHOST_FILE_ACCESS   1
/*******************************************************/
/*        Includes                 */
/*******************************************************/
#include "tbox.h"
/*******************************************************/
/*        Defines                 */
/*******************************************************/

/*******************************************************/
/*        Macros                  */
/*******************************************************/

/*******************************************************/
/*        Typedefs                 */
/*******************************************************/
typedef enum TBOXi_HardSema_e {
   TBOXi_HardSema_RT,
   TBOXi_HardSema_PRINTF
}tTBOXi_HardSema;

/*******************************************************/
/*        Variables Declarations (IMPORT)      */
/*******************************************************/


/*******************************************************/
/*        Functions Declarations          */
/*******************************************************/

IMPORT  void      TBOXi_HardPrint( const char * string );
IMPORT  void      TBOXi_HardGetChar( char * c );
IMPORT  void      TBOXi_HardGetString( char * string );
IMPORT  unsigned long   TBOXi_GetClocksPerSecond( void );
IMPORT  bool      TBOXi_HardInitialize( void );
IMPORT  void      TBOXi_GetCurrentTime( tTBOX_Time * TboxCurrentTime );
IMPORT  void      TBOXi_HardPause( unsigned long milliseconds );
IMPORT  void      TBOXi_HardLock( tTBOXi_HardSema  sema );
IMPORT  void      TBOXi_HardUnlock( tTBOXi_HardSema  sema );

#if TBOXi_DEVHOST_FILE_ACCESS
IMPORT  tTBOX_File     TBOXi_FileOpen( char * filename , tTBOX_OpenMode openmode );
IMPORT  unsigned long   TBOXi_FileRead( tTBOX_File , void * , unsigned long );
IMPORT  unsigned long   TBOXi_FileWrite( tTBOX_File , void * , unsigned long );
IMPORT  void      TBOXi_FileClose( tTBOX_File );
IMPORT unsigned long   TBOXi_FileGetSize( tTBOX_File );
IMPORT  unsigned long   TBOXi_FileSeek( tTBOX_File file, long offset, int whence );
IMPORT  unsigned long   TBOXi_FileTell( tTBOX_File   file );

#endif /* TBOXi_DEVHOST_FILE_ACCESS */


#endif /* TBOX_HARD_P_H */
