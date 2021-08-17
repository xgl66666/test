/******************************************************************************
 *            COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TBOX_HARD
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tbox/trunk/src/arch/ST7109/tbox_hard.c $
 *      $Rev: 411 $
 *      $Date: 2011-09-23 12:32:35 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION : TBOX Implementation for ST driver
 *         Trace on UART by default
 *
 *****************************************************************************/

/********************************************************/
/*        Includes                 */
/********************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#include <crules.h>

#if defined(__TRACE_DCU__)
   #include <string.h>
#elif defined(__TRACE_FPANEL__)
   /* Not implemented. */
#elif defined(__TRACE_FLASH__)
   /* Not implemented. */
#elif defined (__TRACE_UART__)
    #include "tdal_uart.h"
    
#endif
#include "drvUART.h"
#include "tbox_hard_p.h"


/********************************************************/
/*        Defines                  */
/********************************************************/

#if defined (__TRACE_DCU__)
   #define kTBOX_FGETS_BUFF_SIZE   kTBOX_TRC_BUFF_SIZE /* size max of the string to be display */
#elif defined (__TRACE_UART__)
   #define kTBOX_UART_BUFFER_SIZE  20
#endif


/********************************************************/
/*        Macros                   */
/********************************************************/


/********************************************************/
/*        Typedefs                 */
/********************************************************/


/********************************************************/
/*        Global Variables (GLOBAL/IMPORT)      */
/********************************************************/


/********************************************************/
/*        Local Module Variables (MODULE)      */
/********************************************************/


/********************************************************/
/*        Local File Variables (LOCAL)      */
/********************************************************/
LOCAL sem_t    semaphore_RT;
LOCAL sem_t    semaphore_PRINTF;
LOCAL pthread_t taskID;
LOCAL uint8_t stackBuffer[0x4000];

/********************************************************/
/*        Local Functions Declarations (LOCAL)   */
/********************************************************/
IMPORT  void*   RT_Task(void *arg);


/********************************************************/
/*        Functions Definitions (LOCAL/GLOBAL)   */
/********************************************************/

/******************************************************************************
 * Function Name   : TBOXi_HardPrint
 * Description     : Print the string to the output defined at compilation
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
GLOBAL void TBOXi_HardPrint(const char * string )
{
   
#if defined(__TRACE_DCU__)
     printf( "%s", string);

#else //#elif defined(__TRACE_UART__)
   char* tmp = string;
   MDrv_UART_PutString(tmp);
#endif
}

/******************************************************************************
 * Function Name   : TBOXi_HardPutChar
 * Description     : Put a character to the output defined at compilation
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
GLOBAL void TBOXi_HardPutChar(const char c )
{
#if defined(__TRACE_DCU__)
     putchar(c);

#else //#elif defined(__TRACE_UART__)
   MDrv_UART_PutChar(c);
#endif
}

unsigned char cyg_uart_getchar(void);

/******************************************************************************
 * Function Name   : TBOXi_HardGetChar
 * Description     : Get a character to the input defined at compilation
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
GLOBAL void TBOXi_HardGetChar( char * c )
{
#if defined(__TRACE_DCU__)
   char string[8];

   fgets(string,8,stdin);
   *c = string[0];

#else //#elif defined (__TRACE_UART__)
   *c = (char)cyg_uart_getchar();

#endif
}
/******************************************************************************
 * Function Name   : TBOXi_HardGetString
 * Description     : Get a string to the input defined at compilation
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
GLOBAL void TBOXi_HardGetString( char * string  )
{
#if defined(__TRACE_DCU__)
   char *ptr;
   fgets(string,kTBOX_FGETS_BUFF_SIZE,stdin);
   ptr = strchr(string,'\n');
   if(ptr!=NULL)
     *ptr='\0';

#elif defined (__TRACE_UART__)
   {
	   char *ptr;
	   fgets(string,kTBOX_UART_BUFFER_SIZE,stdin);
	   ptr = strchr(string,'\n');
	   if(ptr!=NULL)
	     *ptr='\0';	   
//      char *  tmp = string;
//      char *  ntmp;
//      while((*tmp = (char) cyg_uart_getchar()) != '\n')
//      {
//          if ( *tmp == 0x08 )
//          {
//              if ( tmp != string )
//                  ntmp = tmp-1;
//              else
//                  ntmp = tmp;
//
//              tmp = ntmp;
//          }
//          else
//          {
//              tmp++;
//          }
//      }   
   }   
#endif
}

/******************************************************************************
 * Function Name   : TBOX_GetCurrentTime
 * Description     : Get the current time
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
GLOBAL void TBOXi_GetCurrentTime( tTBOX_Time* TboxCurrentTime )
{
}

/******************************************************************************
 * Function Name   : TBOX_GetClocksPerSecond
 * Description     : Return number of tick per second.
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
GLOBAL unsigned long TBOXi_GetClocksPerSecond( void )
{
   return 0;
}


/******************************************************************************
 * Function Name   : TBOXi_HardInitialize
 * Description     : Initialize semaphores and launch the RT task.
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
GLOBAL  bool   TBOXi_HardInitialize( void )
{
	bool returnedValue = TRUE;	
	pthread_attr_t task_attr;
    struct sched_param schedparam;

    schedparam.sched_priority = 16;	
	/* initialize semaphores */
	if ((sem_init(&semaphore_RT,0,1) != 0) || (sem_init(&semaphore_PRINTF,0,1) != 0))
	{
		TBOXi_HardPrint("Error create semaphore for traces\n");
		returnedValue = FALSE;
	}
	
	/* launch RT task */
    pthread_attr_init( &task_attr );
    pthread_attr_setinheritsched( &task_attr, PTHREAD_EXPLICIT_SCHED );
    pthread_attr_setstackaddr( &task_attr, &stackBuffer[sizeof(stackBuffer)] );
    pthread_attr_setstacksize( &task_attr, sizeof(stackBuffer) );
    pthread_attr_setschedpolicy( &task_attr, 3); //SCHED_RR
    pthread_attr_setschedparam( &task_attr, &schedparam );

    if (pthread_create( &taskID, &task_attr, RT_Task, NULL ) != 0)
    {
    	TBOXi_HardPrint("Error create the RT task for traces\n");
    	returnedValue = FALSE;
    }
//	pthread_attr_init(&task_attr);
//	/*if (pthread_attr_setstackaddr(&task_attr, stackBuffer)!=0)
//	{
//	    printf("pthread_attr_setstackaddr\n");
//	}*/
//	
//	pthread_attr_setstacksize( &task_attr, PTHREAD_STACK_MIN);
//	pthread_attr_setinheritsched(&task_attr, PTHREAD_EXPLICIT_SCHED);
//	if (pthread_create(&taskID, &task_attr, RT_Task, NULL) != 0)
//	{
//		TBOXi_HardPrint("Error create the RT task for traces\n");
//		returnedValue = FALSE;
//	}
		
    return returnedValue;
}

GLOBAL  void   TBOXi_HardPause( unsigned long milliseconds )
{
    struct timespec timeout0;
    struct timespec timeout1;
    struct timespec* tmp;
    struct timespec* t0 = &timeout0;
    struct timespec* t1 = &timeout1;	
    t0->tv_sec  = milliseconds / 1000;          /* Seconds */
    t0->tv_nsec = (milliseconds % 1000) * 1000 * 1000; /* nanoseconds */
	
    while (-1 == nanosleep (t0, t1) && (errno == EINTR))
    {
        tmp = t0;
        t0 = t1;
        t1 = tmp;
    }
    
}

GLOBAL  void   TBOXi_HardLock( tTBOXi_HardSema  sema )
{
	switch(sema)
	{
	case TBOXi_HardSema_RT:
		sem_wait(&semaphore_RT);
		break;
	case TBOXi_HardSema_PRINTF:
		sem_wait(&semaphore_PRINTF);
		break;
	}
}


GLOBAL  void   TBOXi_HardUnlock( tTBOXi_HardSema  sema )
{
	switch(sema)
	{
	case TBOXi_HardSema_RT:
		sem_post(&semaphore_RT);
		break;
	case TBOXi_HardSema_PRINTF:
		sem_post(&semaphore_PRINTF);
		break;
	}
}


#if TBOXi_DEVHOST_FILE_ACCESS
/******************************************************************************
 * Function Name   : TBOX_FileOpen
 * Description     : Open a file on development host
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
GLOBAL  tTBOX_File  TBOXi_FileOpen( char * filename ,
                  tTBOX_OpenMode open_mode )
{
   char open[4];
   void *file;
   if( open_mode == eTBOX_READ )
   {
     sprintf(open,"rb");
   }
   else if( open_mode == eTBOX_WRITE )
   {
     sprintf(open,"wb");
   }
   else if( open_mode == eTBOX_READWRITE )
   {
     sprintf(open,"rb+");
   }
   else
   {
      return (tTBOX_File)0;
   }
   file = fopen( filename , open );
   if (file == NULL)
   {
      return (tTBOX_File)0;
   }
   return (tTBOX_File)file;
}
/******************************************************************************
 * Function Name   : TBOX_FileRead
 * Description     : Read data from an opened file
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
GLOBAL  unsigned long   TBOXi_FileRead( tTBOX_File   file ,
                    void *      buffer ,
                    unsigned long buffer_size )
{
   unsigned long result;

   if (file == 0)
   {
     return 0;
   }

   result = fread (buffer, 1, (size_t)buffer_size, (void *)file);
   return result;
}

/******************************************************************************
 * Function Name   : TBOXi_FileGetSize
 * Description     : Return the size of an opened file
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
GLOBAL unsigned long TBOXi_FileGetSize(tTBOX_File   file )
{
   unsigned long result;

   if (file == 0)
   {
      return 0;
   }

   fseek((void *)file, 0, SEEK_END); /* End of file */
   result = ftell( (void *)file );
   rewind((FILE *)file); /* Beginning of file */

   return result;
}

/******************************************************************************
 * Function Name   : TBOXi_FileWrite
 * Description     : Write data into an opened file
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
GLOBAL  unsigned long   TBOXi_FileWrite( tTBOX_File   file ,
                     void *      buffer ,
                     unsigned long buffer_size )
{
   unsigned long result;

   if (file == 0)
   {
      return 0;
   }

   result = fwrite (buffer, (size_t)buffer_size, 1, (void *)file);
   if(result == 1) result=buffer_size; else result = 0;
   return result;
}
/******************************************************************************
 * Function Name   : TBOX_FileClose
 * Description     : Close an opened file
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
GLOBAL  void   TBOXi_FileClose( tTBOX_File   file )
{
   if (file == 0)
   {
      return;
   }
   fclose((void *)file );
   return;
}

/******************************************************************************
 * Function Name   : TBOXi_FileSeek
 * Description     : Write data into an opened file
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
GLOBAL  unsigned long   TBOXi_FileSeek(tTBOX_File file, long offset, int whence)
{
   unsigned long result;

   if (file == 0)
   {
      return 0;
   }
   result = fseek ((void *)file, offset, whence);
   return result;
}

/******************************************************************************
 * Function Name   : TBOXi_FileTell
 * Description     : Tell the position of an opened file
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
GLOBAL  unsigned long  TBOXi_FileTell( tTBOX_File   file )
{
   if (file == 0)
   {
      return 0;
   }
   return(ftell((void *)file ));
}
#endif

