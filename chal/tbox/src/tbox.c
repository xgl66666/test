/******************************************************************************
 *            COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TBOX
 *
 * FILE NAME: $URL: http://10.50.1.219/svn/sources/hal/tbox/branches/comediaV21/src/tbox.c $
 *      $Rev: 61403 $
 *      $Date: 2011-01-11 15:02:56 +0100 (mar., 11 janv. 2011) $
 *
 * DESCRIPTION : TBOX Implementation
 *
 *****************************************************************************/


/********************************************************/
/*        Includes                 */
/********************************************************/
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "tbox.h"
#include "tbox_hard_p.h"
#include "crules.h"

#if defined TKEL_os21
extern int _SH_posix_PollKey(long int *);
#endif

/********************************************************/
/*        Defines                  */
/********************************************************/
#define TBOX_TRC_NBMOD_BY_ENTRY  10

#define DEBUG_ROW_LEN      kTBOX_TRC_BUFF_SIZE
#define DEBUG_ROWS_MAX      100

#define TBOX_MAX_DRIVER_LENGTH   15

/********************************************************/
/*        Macros                   */
/********************************************************/


/********************************************************/
/*        Typedefs                 */
/********************************************************/

/* trace managment static structure */
typedef struct
{
   bool trace_location; /* trace function call location flag */
   bool trace_enable;   /* enable trace flag */
   bool trace_time;    /* trace function call time flag */
}tTBOX_trc_mngt;

typedef struct
{
   char   text [DEBUG_ROW_LEN+1];
   unsigned long timestamp;
}debugRow_t;


/********************************************************/
/*        Global Variables (GLOBAL/IMPORT)      */
/********************************************************/


/********************************************************/
/*        Local Module Variables (MODULE)      */
/********************************************************/


/* Control trace level array */
tTBOX_TraceIdAndMask TBOX_hash_ctrl_trc_level[256][TBOX_TRC_NBMOD_BY_ENTRY];

LOCAL tTBOX_trc_mngt TBOX_trc_mngt = {FALSE,FALSE,FALSE};


/********************************************************/
/*        Local File Variables (LOCAL)      */
/********************************************************/
LOCAL bool RTinitDone=FALSE;
LOCAL bool MaskIdinitDone=FALSE;

/* buffer used with vsprintf function */
LOCAL char RT_vbuffer[kTBOX_TRC_BUFF_SIZE];

LOCAL debugRow_t debugRowBuffer[DEBUG_ROWS_MAX];
LOCAL uint16_t RT_debugRead  = 0;
LOCAL uint16_t RT_debugWrite = 0;
LOCAL uint16_t RT_debugOverrunErrors = 0;

LOCAL bool TBOXi_bTraceSynchrone = FALSE;

LOCAL bool TBOXi_accessBlocked = FALSE;
/********************************************************/
/*        Local Functions Declarations (LOCAL)   */
/********************************************************/
GLOBAL  void* RT_Task(void *arg);
LOCAL   void RT_Print(char *text);


/********************************************************/
/*        Functions Definitions (LOCAL/GLOBAL)   */
/********************************************************/

void TBOX_BlockAccess(){
    TBOXi_accessBlocked = TRUE;
}

void TBOX_UnblockAccess(){
    TBOXi_accessBlocked = FALSE;
}

/*=============================================================================
*
* TBOX_SetTraceLocation : unable or disable the trace location.
*
* Parameters:
*         - trace_location_value :   TBOX_trc_mngt.trace_location value
*                       TRUE to enable the trace location
*                       FALSE to disable the trace location
*
* Description:
*         Set the TBOX_trc_mngt.trace_location global flag to unable or disable the
*         trace function call location.
*
* Returns:
*         nothing
*
*===========================================================================*/
void TBOX_SetTraceLocation(bool trace_location_value)
{
   TBOX_trc_mngt.trace_location = trace_location_value;
   return;
}

/*=============================================================================
*
* TBOX_SetTraceTime : unable or disable the trace time.
*
* Parameters:
*         - trace_time_value :   TBOX_trc_mngt.trace_time value
*                    TRUE to enable the trace time
*                    FALSE to disable the trace time
*
* Description:
*         Set the TBOX_trc_mngt.trace_time global flag to unable or
*         disable the trace function call time.
*
* Returns:
*         nothing
*
*===========================================================================*/
void TBOX_SetTraceTime(bool trace_time_value)
{
   TBOX_trc_mngt.trace_time = trace_time_value;
   return;
}

/*=============================================================================
*
* TBOX_SetTraceEnabled  : unable or disable the trace location.
*
* Parameters:
*         - trace_enable_value   :   TBOX_trc_mngt.trace_enable value
*                       TRUE to enable the trace
*                       FALSE to disable the trace
*
* Description:
*         Set the TBOX_trc_mngt.trace_enable global flag to unable or disable the
*         trace
*
* Returns:
*         nothing
*
*===========================================================================*/
void TBOX_SetTraceEnabled(bool trace_enable_value)
{
   TBOX_trc_mngt.trace_enable = trace_enable_value;
   return;
}

/*=============================================================================
*
* TBOX_InitModuleTrace : Trace level intialisation for a driver
*
* Parameters:
*         - mod_id   :   driver identifier
*         - mask_init :   trace level mask value
*
*
* Description:
*         Initialise the trace level mask with the mask_�init value
*         for the driver mod_id.
*
* Returns:
*         nothing
*
*===========================================================================*/
void TBOX_InitModuleTrace(tTBOX_TraceId mod_id, tTBOX_TraceMask mask_init)
{
   int i;
   tTBOX_TraceId hash_value;

   if (MaskIdinitDone == FALSE)
   {
     for (i=0; i<256; i++)
      {
      TBOX_hash_ctrl_trc_level[i][0].id = (tTBOX_TraceId)(-1);
      }
      MaskIdinitDone = TRUE;
   }

   /* calculate which array entry must be used */
   hash_value = (mod_id & 0x000000FF)      + ((mod_id & 0x0000FF00) >> 8) +
         ((mod_id & 0x00FF0000) >> 16) + ((mod_id & 0xFF000000) >> 24);
   hash_value = hash_value & 0xFF;

   for(i=0; i<TBOX_TRC_NBMOD_BY_ENTRY; i++)
     {
      if (TBOX_hash_ctrl_trc_level[(int)hash_value][i].id == mod_id)
     {
   /* update the trace level with the new one given */
   TBOX_hash_ctrl_trc_level[(int)hash_value][i].mask = mask_init;
     break;
     }
      if (TBOX_hash_ctrl_trc_level[(int)hash_value][i].id == (tTBOX_TraceId)(-1))
      {
      /* find the first free entry */
      TBOX_hash_ctrl_trc_level[(int)hash_value][i].id   = mod_id;
      TBOX_hash_ctrl_trc_level[(int)hash_value][i].mask = mask_init;

      TBOX_hash_ctrl_trc_level[(int)hash_value][++i].id = (tTBOX_TraceId)(-1);
      break;
      }
   }

   if (i == TBOX_TRC_NBMOD_BY_ENTRY)
   {
     TBOXi_HardPrint("### Too much TBOX module Ids for one entry !!!\n");
   }

   return;
}

/*=============================================================================
*
* TBOX_Trace : debug trace function
*
* Parameters:
*         - line     :   line in the source file where the trace
*                 function is called
*         - file     :   (I) source file where the trace function is called
*         - mod_id   :   driver identifier
*         - trc_level :   trace level
*         - fmt      :   (I) printf format type
*         - ...      :   printf argument types
*
*
* Description:
*         Display the trace defined by fmt and args, if the trc_level
*         value is set in the trace level mask for the mod_id driver.
*         If the global flag 'TBOX_trc_mngt.trace_location' has a non zero value,
*         the line and file where is called the trace function are also
*         displayed.
*
* Returns:
*         nothing
*
*===========================================================================*/
void TBOX_Trace(int line,const char *file,tTBOX_TraceId mod_id, const char *modName, tTBOX_TraceMask trc_level,const char *fmt, ...)
{
   va_list   args;
   char   Buff[kTBOX_TRC_BUFF_SIZE]; /* working buffer */
   tTBOX_Time   TboxCurrentTime;
   uint32_t   remain_length;
   int   i;
   tTBOX_TraceId hash_value;

   if(RTinitDone == FALSE)
   {
     RTinitDone = TBOXi_HardInitialize();
   }

   if(TBOXi_accessBlocked)
   {
       return;
   }

   /*------------------------------------------*/
   /* Check if the traces have been enabled   */
   /*------------------------------------------*/
   if(TBOX_trc_mngt.trace_enable == FALSE)
   {
     return;
   }

   /*----------------------*/
   /* Check the driver ID  */
   /*----------------------*/
   Buff[0] = '\0';

   /* calculate which array entry must be used */
   hash_value = (mod_id & 0x000000FF)      + ((mod_id & 0x0000FF00) >> 8) +
         ((mod_id & 0x00FF0000) >> 16) + ((mod_id & 0xFF000000) >> 24);
   hash_value = hash_value & 0xFF;

   i = 0;
   while ( (i<TBOX_TRC_NBMOD_BY_ENTRY) &&
      (TBOX_hash_ctrl_trc_level[(int)hash_value][i].id != mod_id) &&
      (TBOX_hash_ctrl_trc_level[(int)hash_value][i].id != (tTBOX_TraceId)(-1)) )
   {
     i++;
   }

   if (TBOX_hash_ctrl_trc_level[(int)hash_value][i].id == mod_id)
   {
     /*--------------------------------------------------*/
     /* Check if the driver is allowed to do this trace  */
     /*--------------------------------------------------*/
     if(TBOX_hash_ctrl_trc_level[(int)hash_value][i].mask & trc_level)
     {
      /*-------------------*/
      /* timestamp display */
      /*-------------------*/
      if(TBOX_trc_mngt.trace_time == TRUE)
      {
         TBOXi_GetCurrentTime(&TboxCurrentTime);
         sprintf(&Buff[strlen(Buff)],"(%ldh%ldm%02lds%03ldms) ",
         TboxCurrentTime.hour,
         TboxCurrentTime.min,
         TboxCurrentTime.sec,
         TboxCurrentTime.msec);
      }

      /* ------------- */
      /* Level display */
      /* ------------- */
      switch(trc_level)
      {
         case kTBOX_TRC_CRITICAL:
          sprintf(&Buff[strlen(Buff)],"_C_ ");
          break;
         case kTBOX_TRC_WARNING:
          sprintf(&Buff[strlen(Buff)],"_W_ ");
          break;
         case kTBOX_TRC_FCT:
          sprintf(&Buff[strlen(Buff)],"_F_ ");
          break;
         case kTBOX_TRC_1:
          sprintf(&Buff[strlen(Buff)],"_1_ ");
          break;
         case kTBOX_TRC_2:
          sprintf(&Buff[strlen(Buff)],"_2_ ");
          break;
         case kTBOX_TRC_3:
          sprintf(&Buff[strlen(Buff)],"_3_ ");
          break;
         case kTBOX_TRC_4:
          sprintf(&Buff[strlen(Buff)],"_4_ ");
          break;
         case kTBOX_TRC_5:
          sprintf(&Buff[strlen(Buff)],"_5_ ");
          break;
      }

   /*--------------------------*/
   /* line and file display   */
   /*--------------------------*/
   if(TBOX_trc_mngt.trace_location == TRUE)
   {
     sprintf(&Buff[strlen(Buff)],"[%s : %d] - ",file,line);
     }
   /*----------------------*/
   /* Name driver display  */
   /*----------------------*/

   sprintf(&Buff[strlen(Buff)],"%*.*s : ", TBOX_MAX_DRIVER_LENGTH, TBOX_MAX_DRIVER_LENGTH, modName);

   va_start(args,fmt);
   vsprintf( RT_vbuffer , fmt , args );
   va_end(args);

   remain_length = DEBUG_ROW_LEN - strlen(Buff);

   if (strlen(RT_vbuffer) < remain_length)
   {
   sprintf(&Buff[strlen(Buff)],"%s",RT_vbuffer);
   }
   else
     {
     RT_vbuffer[ remain_length-30 ] = '\0'; /* reserve some room for "truncated" message */
     snprintf( &Buff[strlen(Buff)] , remain_length ,
     "%s [line truncated by TBOX]\n", RT_vbuffer);
     }
   }

   if(strlen(Buff))
   {
     TBOXi_HardLock( TBOXi_HardSema_PRINTF );

   /* Print to output */
   RT_Print(Buff);

   TBOXi_HardUnlock( TBOXi_HardSema_PRINTF );
   }
   }

   return;
}

/*=============================================================================
*
* TBOX_Print      : debug trace function
*
* Parameters:
*         - line     :   line in the source file where the trace
*                 function is called
*         - file     :   (I) source file where the trace function is called
*         - fmt      :   (I) printf format type
*         - ...      :   printf argument types
*
*
* Description:
*         Display the trace defined by fmt and args.
*         If the global flag 'TBOX_trc_mngt.trace_location' has a non zero value,
*         the line and file where is called the trace function are also
*         displayed.
*
* Returns:
*         nothing
*
*===========================================================================*/
void TBOX_Print(int line,const char *file,const char *fmt, ...)
{
   va_list args;
   char Buff[kTBOX_TRC_BUFF_SIZE]; /* working buffer */
   tTBOX_Time TboxCurrentTime;
   uint32_t   remain_length;

   if(RTinitDone == FALSE)
   {
     RTinitDone = TBOXi_HardInitialize();
   }

   if(TBOXi_accessBlocked)
   {
      return;
   }

   /*------------------------------------------*/
   /* Check if the traces have been enabled   */
   /*------------------------------------------*/
   if(TBOX_trc_mngt.trace_enable == FALSE)
   {
     return;
   }


   Buff[0] = '\0';
   /*-------------------*/
   /* timestamp display */
   /*-------------------*/
   if(TBOX_trc_mngt.trace_time == TRUE)
   {
     TBOXi_GetCurrentTime(&TboxCurrentTime);
     sprintf(&Buff[strlen(Buff)],"(%ldh%ldm%02lds%03ldms) ",
     TboxCurrentTime.hour,
     TboxCurrentTime.min,
     TboxCurrentTime.sec,
     TboxCurrentTime.msec);
   }

   /*--------------------------*/
   /* line and file display   */
   /*--------------------------*/
   if(TBOX_trc_mngt.trace_location == TRUE)
   {
     int k;
     int pos = 0;

     for (k = 0; k < (int)strlen (file); k++)
     {
      if (file[k] == '/') pos = k;
      if (file[k] == '\\') pos = k;
     }

     sprintf(&Buff[strlen(Buff)],"[%s : %d] - ",(file+pos+1),line);
   }

   va_start(args,fmt);
   vsprintf( RT_vbuffer , fmt , args );
   va_end(args);

   remain_length = DEBUG_ROW_LEN - strlen(Buff);

   if (strlen(RT_vbuffer) < remain_length)
   {
     sprintf(&Buff[strlen(Buff)],"%s",RT_vbuffer);
   }
   else
   {
     RT_vbuffer[ remain_length-30 ] = '\0'; /* reserve some room for "truncated" message */
     snprintf( &Buff[strlen(Buff)] , remain_length ,
     "%s [line truncated by TBOX]\n", RT_vbuffer);
   }

   if(strlen(Buff))
   {
     TBOXi_HardLock( TBOXi_HardSema_PRINTF );

   /* Print to output */
   RT_Print(Buff);

   TBOXi_HardUnlock( TBOXi_HardSema_PRINTF );
   }

   return;
}

/******************************************************************************
 * Function Name   : TBOX_Puts
 * Description     : Print a string on the output display
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       : a ASCII code
 *****************************************************************************/
void TBOX_Puts (char *pString)
{
   RT_Print(pString);
}

/******************************************************************************
 * Function Name   : TBOX_PutChar
 * Description     : Put a character
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
void TBOX_PutChar(char c)
{
   while (RT_debugWrite != RT_debugRead)
   {
     TBOXi_HardPause( 100 );
   }

   TBOXi_HardPutChar(c);
   return;
}

/******************************************************************************
 * Function Name   : TBOX_GetChar
 * Description     : Get a character
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       : a ASCII code
 *****************************************************************************/
void TBOX_GetChar(char * c )
{
   while (RT_debugWrite != RT_debugRead)
   {
     TBOXi_HardPause( 100 );
   }

   TBOXi_HardGetChar(c);
   return;
}

/******************************************************************************
 * Function Name   : TBOX_GetString
 * Description     : Get a character
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
void TBOX_GetString(char * string )
{
   while (RT_debugWrite != RT_debugRead)
   {
     TBOXi_HardPause( 100 );
   }

   TBOXi_HardGetString(string);
   return;
}

/******************************************************************************
 * Function Name   : TBOX_GetCharNonBlocking
 * Description     : Get a character without blocking call
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
void TBOX_GetCharNonBlocking(char *c)
{
#ifdef TKEL_os20
   long int ctemp;
   long int flag;
   
   #if defined(__TRACE_UART__)
   /* not blocking for uart */
   TBOX_GetChar(c);
   #else
   flag = debugpollkey(&ctemp);
   if (flag == 1)
   {
     *c = (char)ctemp;
   }
   else
   {
     *c = (char)0;
   }
   #endif
   return;
#elif defined TKEL_os21

   long int ctemp;
   #if defined(__TRACE_UART__)
   /* not blocking for uart */
   TBOX_GetChar(c);
   #else
     _SH_posix_PollKey (&ctemp);
     *c = (char) ctemp;
   #endif
   return;

#elif defined TKEL_OSAL_YODA
   scanf("%s", c);
#elif defined TKEL_ecos
   /* not blocking for uart */
   if(!TBOXi_accessBlocked)
   {
       TBOX_GetChar(c);
   }
   else
   {
       *c = '\0';
   }
#else
   int ctemp;
   ctemp = getchar();
   *c = (char) ctemp;
#endif
   return;
}

/******************************************************************************
 * Function Name   : TBOX_GetStringNonBlocking
 * Description     : Get a string
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
void TBOX_GetStringNonBlocking( char * string )
{
   int8_t  entry = 0;
   int8_t  Fin;
   int16_t rg;

   if(string == NULL)
   {
     return;
   }

   Fin = 0;
   rg  = 0;

   while(! Fin)
   {
   /* TBOXi_HardPause( 50 ); */

   TBOX_GetCharNonBlocking ((char*)&entry);

   if( (entry >= '0' && entry <= '9') ||
     (entry >= 'a' && entry <= 'z') ||
     (entry == '_') ||
     (entry == '?')
   )
   {
     string[rg] = entry;
   }
   else if(entry == 10 || entry == 13)
   {
     string[rg] = '\0';
     Fin = 1;
     continue;
   }
   else
   {
     continue;
   }

   rg++;

   TBOXi_HardPrint((char*)&entry);

   }


   return;
}

/******************************************************************************
 * Function Name   : RT_Print
 * Description     :
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
  char c_PrintStr0[1024+1];
LOCAL void RT_Print(char *text)
{
   uint16_t i=0;
   uint16_t len=0;
 
   int	  i_len0 = 0;
   int	  i_len1  = 0;
   int	  i_len2 = 0;
   
   char* pc_pos = NULL, *pc_pos1 = NULL,*pc_pos2 = NULL;
   
   int	  i_templen;


   if(RTinitDone != TRUE)
     return;


   if(text == NULL)
   {
	   return;
   }
		
   i_len0 = strlen(text);
   
   if(i_len0 > 1024)
    {
	   printf("@@too long print \r\n");    
	   return;
    }
   /*Maybe a text contains /r/n prime in many places
   len = strlen(text);
	if ((text[len-1] == '\n') && ((len == 1) || (text[len-2] != '\r')))
	{
		text[len-1] = '\r';
		text[len] = '\n';
		text[len+1] = 0;
	}
*/   
   pc_pos = text;
   


   TBOXi_HardLock( TBOXi_HardSema_RT );

   if(TBOXi_bTraceSynchrone == FALSE)
   {
   /* ------------------------------------------- */
   /* Trace ASYNCHRONE _ displayed by the RT Task */
   /* ------------------------------------------- */

     i = RT_debugWrite++;
     if (RT_debugWrite >= DEBUG_ROWS_MAX)
      RT_debugWrite = 0;
      if (RT_debugWrite == RT_debugRead)
      {
      RT_debugWrite = i;     /* Miss this entry! */
      RT_debugOverrunErrors++;
      }
      else
      {
      /*debugRowBuffer[i].timestamp = time_now();*/
      len = strlen(text);
      if (len > DEBUG_ROW_LEN)
      len = DEBUG_ROW_LEN;
      if (len)
      memcpy(debugRowBuffer[i].text, text, len);
      debugRowBuffer[i].text [len] = (char) 0;
      }
   }
   else
   {
     /* -------------------------------------------- */
     /* Trace SYNCHRONE _ displayed by this function */
     /* -------------------------------------------- */

		do
		{
			pc_pos1 = strstr(pc_pos,"\r\n");
			
			if(pc_pos1 != NULL)
			{
				i_templen = pc_pos1 - pc_pos;
				i_len2 = 2;
			}
			else
			{
				pc_pos1 = strstr(pc_pos,"\n");
	 
				if(pc_pos1 != NULL)
				{
					i_templen = pc_pos1 - pc_pos;
					i_len2 = 1;
				}
				else
				{
					i_templen = strlen(pc_pos);
				}
			}
			
			if(i_templen > 0)
			{
				memset(c_PrintStr0,'\0',1024+1);
				memcpy(c_PrintStr0,pc_pos,i_templen);
			
				TBOXi_HardPrint(c_PrintStr0);
				
				if(pc_pos1 != NULL)
				{
                 #if (NAGRA_CAK_VERSION == NVCA_DALTESTVERSION)
					if(memcmp(pc_pos,"Enter Selection >>",18)!=0)
                 #endif
						TBOXi_HardPrint("\r\n");
					
					pc_pos = pc_pos1+i_len2;
					i_len0 -=i_len2;
					i_len2 = 0;
				}
				i_len0 -= i_templen;				
			}
			else 
			{
				//stringa\n			
				if(pc_pos1 == pc_pos)
				{
					TBOXi_HardPrint("\r\n");		
					pc_pos = pc_pos1+1;
					i_len0 -=1;
				}
			}	
		}while(i_len0 > 0);
   }

   TBOXi_HardUnlock( TBOXi_HardSema_RT );

   return;
}

/******************************************************************************
 * Function Name   : RT_Task
 * Description     :
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
void* RT_Task(void *arg)
{
   while(TRUE)
   {
     TBOXi_HardLock( TBOXi_HardSema_RT );

     while (RT_debugWrite != RT_debugRead)
     {
      if (RT_debugOverrunErrors)
     {
      TBOXi_HardPrint("### Overrun erros !\n");
      RT_debugOverrunErrors=0;
     }
      TBOXi_HardPrint(debugRowBuffer[RT_debugRead].text);
      if (RT_debugRead < (DEBUG_ROWS_MAX-1) )
         RT_debugRead++;
      else
         RT_debugRead = 0;
     }

     TBOXi_HardUnlock( TBOXi_HardSema_RT );
     TBOXi_HardPause( 100 );
   }
}

/******************************************************************************
 * Function Name   : TBOX_TraceSynchrone
 * Description     : Configuration of trace system (synchrone, or aynchrone)
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
void TBOX_TraceSynchrone(bool bTraceSync)
{
   TBOXi_bTraceSynchrone = bTraceSync;
}

/******************************************************************************
 * Function Name   : TBOX_GetCurrentTime
 * Description     : Get local time
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
void TBOX_GetCurrentTime( tTBOX_Time* TboxCurrentTime )

{
   TBOXi_GetCurrentTime(TboxCurrentTime);
}

#if TBOX_DEVHOST_FILE_ACCESS
/******************************************************************************
 * Function Name   : TBOX_FileOpen
 * Description     : Open a file on development host
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
tTBOX_File  TBOX_FileOpen( char * filename , tTBOX_OpenMode open_mode )
{
   tTBOX_File result;
   result = TBOXi_FileOpen( filename , open_mode );
   return result;
}
/******************************************************************************
 * Function Name   : TBOX_FileRead
 * Description     : Read data from an opened file
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
unsigned long   TBOX_FileRead( tTBOX_File   file ,
   void *   buffer ,
   unsigned long   buffer_size )
{
   unsigned long   result;
   result = TBOXi_FileRead( file , buffer , buffer_size );
   return result;
}
/******************************************************************************
 * Function Name   : TBOX_FileWrite
 * Description     : Write data into an opened file
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
unsigned long   TBOX_FileWrite( tTBOX_File   file ,
   void *   buffer ,
   unsigned long buffer_size )
{
   unsigned long   result;
   result = TBOXi_FileWrite( file , buffer , buffer_size );
   return result;
}

/******************************************************************************
 * Function Name   : TBOX_FileSeek
 * Description     : Seek into an opened file
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
unsigned long   TBOX_FileSeek(tTBOX_File file,  long offset, int whence)
{
   unsigned long   result;
   result = TBOXi_FileSeek( file , offset , whence );
   return result;
}

/******************************************************************************
 * Function Name   : TBOX_FileGetSize
 * Description     : Get the size of the file
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
unsigned long   TBOX_FileGetSize( tTBOX_File   file )
{
   unsigned long   result;
   result = TBOXi_FileGetSize( file );
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
void   TBOX_FileClose( tTBOX_File   file )
{
   TBOXi_FileClose( file );
}
/******************************************************************************
 * Function Name   : TBOX_FileTell
 * Description     : tell position of an opened file
 * Side effects    :
 * Comment       :
 * Inputs      :
 * Outputs       :
 *****************************************************************************/
unsigned long   TBOX_FileTell( tTBOX_File   file )
{
   return(TBOXi_FileTell( file ));
}

#endif /* TBOX_DEVHOST_FILE_ACCESS */

/* tbox_printf is used as a replacement for printf when building with TRACES=UART
 * Preprocessor mechanism is used for this. Please see generic.mak
 */
int tbox_printf(const char *fmt, ...)
{
    va_list arg;

    va_start (arg, fmt);
    TBOX_Print(0, "", fmt, arg);
    va_end (arg);

    return 0;
}
