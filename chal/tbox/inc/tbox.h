/******************************************************************************
 *             COPYRIGHT 2004 IWEDIA TECHNOLOGIES             *
 ******************************************************************************
 *
 * MODULE NAME: TBOX
 *
 * FILE NAME: $URL: http://10.50.1.219/svn/sources/hal/tbox/branches/comediaV21/inc/tbox.h $
 *       $Rev: 61536 $
 *       $Date: 2011-03-29 17:39:37 +0200 (mar., 29 mars 2011) $
 *
 * DESCRIPTION : TBOX API
 *
 *****************************************************************************/

#ifndef TBOX_H
#define TBOX_H


#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*          Includes                                   */
/*******************************************************/
#ifndef _CRULES_H_
   #include "crules.h"
#endif
#if defined (__DEBUG_TIME__)
#include <sys/timeb.h>
#endif

/*******************************************************/
/*          Defines                                    */
/*******************************************************/
   /** TBOX defines functions to access file on the development host **/
#define TBOX_DEVHOST_FILE_ACCESS   1

#define kTBOX_TRC_BUFF_SIZE  1024 /* size max of the string to be display */

#define kTBOX_SIT           __LINE__,__FILE__,TBOX_ModuleID,TBOX_ModuleName
#define kTBOX_LF            __LINE__,__FILE__

/* Trace levels */
#define kTBOX_TRC_NONE      0x00 /* No trace */
#define kTBOX_TRC_CRITICAL  0x01 /* Critical error which need to reboot */
#define kTBOX_TRC_WARNING   0x02 /* Minor error */
#define kTBOX_TRC_FCT      0x04 /* Functions enter and exit */
#define kTBOX_TRC_1      0x08 /* Trace level 1 */
#define kTBOX_TRC_2      0x10 /* Trace level 2 */
#define kTBOX_TRC_3      0x20 /* Trace level 3 */
#define kTBOX_TRC_4      0x40 /* Trace level 4 */
#define kTBOX_TRC_5      0x80 /* Trace level 5 */
#define kTBOX_TRC_ALL      0xFF /* All levels */

/* Defines for TBOX_TRACE calls */
#define kTBOX_NIV_CRITICAL kTBOX_SIT,kTBOX_TRC_CRITICAL /* Critical error which need to reboot */
#define kTBOX_NIV_WARNING  kTBOX_SIT,kTBOX_TRC_WARNING  /* Minor error */
#define kTBOX_NIV_FCT      kTBOX_SIT,kTBOX_TRC_FCT      /* Functions enter and exit */
#define kTBOX_NIV_1      kTBOX_SIT,kTBOX_TRC_1      /* Trace level 1 */
#define kTBOX_NIV_2      kTBOX_SIT,kTBOX_TRC_2      /* Trace level 2 */
#define kTBOX_NIV_3      kTBOX_SIT,kTBOX_TRC_3      /* Trace level 3 */
#define kTBOX_NIV_4      kTBOX_SIT,kTBOX_TRC_4      /* Trace level 4 */
#define kTBOX_NIV_5      kTBOX_SIT,kTBOX_TRC_5      /* Trace level 5 */

/* Trace Id */
typedef unsigned long tTBOX_TraceId;

/* Trace mask */
typedef unsigned char tTBOX_TraceMask;

/*******************************************************/
/*          Macros                     */
/*******************************************************/
#if defined (COMEDIA_TRACES_FOR_INTEGRATION_ENABLE)

   #define mTBOX_Str(x) # x

   #define   mTBOX_COMEDIA_FOR_INTEG_INIT(x)      TBOX_InitModuleTrace x
   #define mTBOX_TRACE_SYNCHRONE(x)      TBOX_TraceSynchrone x
   #define   mTBOX_COMEDIA_FOR_INTEG_TRACE(x)             TBOX_Trace x

   #define mTBOX_SET_MODULE(x) \
   static char* TBOX_ModuleName = mTBOX_Str(x); \
   static tTBOX_TraceId TBOX_ModuleID = x

   #define mTBOX_TRACE_ENABLED(x)      TBOX_SetTraceEnabled x
#else
   #define   mTBOX_COMEDIA_FOR_INTEG_INIT(x)           
   #define   mTBOX_COMEDIA_FOR_INTEG_TRACE(x)
#endif

#if defined (__DEBUG_TRACE__)

   #define mTBOX_INIT_MOD_TRACE(x)      TBOX_InitModuleTrace x

   #ifndef mTBOX_TRACE_ENABLED
      #define mTBOX_TRACE_ENABLED(x)      TBOX_SetTraceEnabled x
   #endif

   #define mTBOX_TRACE_LOC_ENABLED(x)      TBOX_SetTraceLocation x
   #define mTBOX_TRACE_TIME_ENABLED(x)    TBOX_SetTraceTime x
   
   #if defined(DIAGNOSTICS_ENABLE) && defined(MODULE_DIAGNOTICS_ENABLE) && !defined(BOOTLOADER) && !defined(DOWNLOADER)
      void DIAG_Trace(int line,char *file,uint32_t mod_id, char *modName, tTBOX_TraceMask trc_level, char *fmt, ...);
      #define mTBOX_TRACE(x) TBOX_Trace x ;\
       { \
           uint32_t TBOX_ModuleID=0; TBOX_ModuleID = TBOX_ModuleID;\
           char * TBOX_FunctionName=""; TBOX_FunctionName = TBOX_FunctionName;\
           char * TBOX_ModuleName=""; TBOX_ModuleName = TBOX_ModuleName;\
           DIAG_Trace x;  \
       }

   #else
      #define mTBOX_TRACE(x)           TBOX_Trace x
   #endif
   
   #define mTBOX_PRINT(x)           TBOX_Print x
   #define mTBOX_GETCHAR(x)           TBOX_GetChar x
   #define mTBOX_GETSTRING(x)          TBOX_GetString x
   #define mTBOX_GETCHAR_NON_BLOCKING(x)   TBOX_GetCharNonBlocking x
   #define mTBOX_GETSTRING_NON_BLOCKING(x) TBOX_GetStringNonBlocking x

   #if defined (COMEDIA_TRACES_FOR_INTEGRATION_ENABLE)
      #define mTBOX_ASSERT(x)   if(x) {}
      #define mTBOX_ASSERTm(x,m) if(x) {}         
   #else
      #define mTBOX_ASSERT(x)   if(!(x)) { mTBOX_PRINT((kTBOX_LF,"Assertion failed ("#x") in FILE %s, LINE %d\n", __FILE__, __LINE__)); }
      #define mTBOX_ASSERTm(x,m) if(!(x)) { mTBOX_PRINT((kTBOX_LF,"Assertion failed (%s) in FILE %s, LINE %d\n", (m), __FILE__, __LINE__)); }
   #endif
   
   #define mTBOX_TRACE_SYNCHRONE(x)      TBOX_TraceSynchrone x

   #ifndef mTBOX_Str
      #define mTBOX_Str(x) # x
   #endif
/*
 * Module name and identifier declaration
 * To be included in all the source files of a module
 */
   #ifndef mTBOX_SET_MODULE
      #define mTBOX_SET_MODULE(x) \
      static const char* TBOX_ModuleName = #x; \
      static tTBOX_TraceId TBOX_ModuleID = x
   #endif

#ifdef __DEBUG_TIME__
/* Function name declaration and function begining
 * To be added after the local variable declarations in all the functions
 * In mode time Add a TRACE if time sup 100 milli seconds
 */
#define mTBOX_FCT_ENTER(x) \
      const char* TBOX_FunctionName = mTBOX_Str(x); \
      struct timeb TBOX_TopBeg, TBOX_TopEnd; \
      double TBOX_duration; \
      ftime(&TBOX_TopBeg); \
      mTBOX_TRACE((kTBOX_NIV_FCT, "{ %s :\n", TBOX_FunctionName))

/* Trace exit function macro */
#define mTBOX_RETURN \
   ftime(&TBOX_TopEnd); \
   TBOX_duration = (((double)TBOX_TopEnd.time - (double)TBOX_TopBeg.time)*1000.0 + (double)TBOX_TopEnd.millitm - (double)TBOX_TopBeg.millitm)/1000.0;\
   if (TBOX_duration > 0.109) { \
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,"}%s (%.03f sec)\n",TBOX_FunctionName,TBOX_duration));\
   }\
   else { \
      mTBOX_TRACE((kTBOX_NIV_FCT,"}%s (%.03f sec)\n",TBOX_FunctionName,TBOX_duration));\
   }\
   return

/* Idem only for trace a particular function */
#define mTBOX_FCT_ENTER_T(x) \
   const char* TBOX_FunctionName = mTBOX_Str(x); \
   struct timeb TBOX_TopBeg, TBOX_TopEnd; \
   double TBOX_duration; \
   ftime(&TBOX_TopBeg); \
   mTBOX_TRACE((kTBOX_NIV_CRITICAL, "{ %s :\n", TBOX_FunctionName))

/* Trace exit function macro */
#define mTBOX_RETURN_T \
   ftime(&TBOX_TopEnd); \
   TBOX_duration = (((double)TBOX_TopEnd.time - (double)TBOX_TopBeg.time)*1000.0 + (double)TBOX_TopEnd.millitm - (double)TBOX_TopBeg.millitm)/1000.0;\
   mTBOX_TRACE((kTBOX_NIV_CRITICAL,"}%s (%.03f sec)\n",TBOX_FunctionName,TBOX_duration));\
   return

#else

/* Function name declaration and function begining
 * To be added after the local variable declarations in all the functions
 */
#define mTBOX_FCT_ENTER(x) \
   const char* TBOX_FunctionName = mTBOX_Str(x); \
   mTBOX_TRACE((kTBOX_NIV_FCT, "{ %s :\n", TBOX_FunctionName))

/* Trace exit function macro */
#define mTBOX_RETURN \
   mTBOX_TRACE((kTBOX_NIV_FCT, "}%s\n", TBOX_FunctionName)); \
   return

#define mTBOX_FCT_ENTER_T(x) \
   const char* TBOX_FunctionName = mTBOX_Str(x); \
   mTBOX_TRACE((kTBOX_NIV_FCT, "{ %s :\n", TBOX_FunctionName))

/* Trace exit function macro */
#define mTBOX_RETURN_T \
   mTBOX_TRACE((kTBOX_NIV_FCT, "}%s\n", TBOX_FunctionName)); \
   return

#endif /* __DEBUG_TIME__ */

/* #define return mUT_RETURN */

#else
#define mTBOX_INIT_MOD_TRACE(x)
#ifndef mTBOX_TRACE_ENABLED
#define mTBOX_TRACE_ENABLED(x)
#endif
#define mTBOX_TRACE_LOC_ENABLED(x)
#define mTBOX_TRACE_TIME_ENABLED(x)

#if defined(DIAGNOSTICS_ENABLE) && defined(MODULE_DIAGNOTICS_ENABLE)&& !defined(BOOTLOADER) && !defined(DOWNLOADER)
void DIAG_Trace(int line,char *file,uint32_t mod_id, char *modName, tTBOX_TraceMask trc_level, char *fmt, ...);
/*#define __DEBUG_TRACE__ DON'T DO THAT */
#define mTBOX_TRACE(x) ; \
   { \
      uint32_t TBOX_ModuleID=0; TBOX_ModuleID = TBOX_ModuleID;\
      char * TBOX_FunctionName=""; TBOX_FunctionName = TBOX_FunctionName;\
      char * TBOX_ModuleName=""; TBOX_ModuleName = TBOX_ModuleName;\
      DIAG_Trace x;  \
   }

#else
#define mTBOX_TRACE(x)
#endif

#define mTBOX_PRINT(x)
#define mTBOX_GETCHAR(x)
#define mTBOX_GETSTRING(x)
#ifndef mTBOX_SET_MODULE
#define mTBOX_SET_MODULE(x) typedef void tTBOX_Dummy
#endif
#define mTBOX_FCT_ENTER(x)
#define mTBOX_FCT_ENTER_T(x)
#define mTBOX_RETURN return
#define mTBOX_RETURN_T return
#define mTBOX_ASSERT(x)   if(x) {/*Avoid compilation warning*/}
#define mTBOX_ASSERTm(x,m) if(x) {/*Avoid compilation warning*/}
#ifndef mTBOX_TRACE_SYNCHRONE
#define mTBOX_TRACE_SYNCHRONE(x)
#endif

#endif /* __DEBUG_TRACE__ */
/*******************************************************/
/*          Typedefs                                   */
/*******************************************************/

typedef struct{
   unsigned long sec;
   unsigned long min;
   unsigned long hour;
   unsigned long msec;
}tTBOX_Time;

typedef struct{
   tTBOX_TraceId   id;
   tTBOX_TraceMask  mask;
}tTBOX_TraceIdAndMask;

#if TBOX_DEVHOST_FILE_ACCESS
typedef uint32_t   tTBOX_File;
/*typedef void*   tTBOX_File; */

typedef enum {
   eTBOX_READ,
   eTBOX_WRITE,
   eTBOX_READWRITE
}tTBOX_OpenMode;
#endif /* TBOX_DEVHOST_FILE_ACCESS */
/*******************************************************/
/*          Variables Declarations (IMPORT)            */
/*******************************************************/


/*******************************************************/
/*          Functions Declarations                     */
/*******************************************************/

typedef unsigned char tTBOX_ModuleTraceID;

void TBOX_SetTraceLocation(bool enable_trace);
void TBOX_SetTraceTime(bool enable_trace);
void TBOX_SetTraceEnabled(bool trace_enable_value);
void TBOX_InitModuleTrace(tTBOX_TraceId mod_id, tTBOX_TraceMask mask_init);
void TBOX_Trace(
                int line,
                const char *file,
                tTBOX_TraceId mod_id,
                const  char *modName,
                tTBOX_TraceMask trc_level,
                const  char *fmt,
                ...);
void TBOX_Print(int line,const char *file,const char *fmt, ...);
void TBOX_Puts (char *pString);
void TBOX_GetChar(char* c);
void TBOX_GetString(char* string);
void TBOX_TraceSynchrone(bool bTraceSync);
void TBOX_GetCurrentTime(tTBOX_Time* TboxCurrentTime);

void TBOX_GetCharNonBlocking(char *c);
void TBOX_GetStringNonBlocking( char * string );

int tbox_printf(const char *fmt, ...);

#if TBOX_DEVHOST_FILE_ACCESS
tTBOX_File     TBOX_FileOpen(char * filename, tTBOX_OpenMode mode);
unsigned long  TBOX_FileRead(
                   tTBOX_File    file,
                   void*         buffer,
                   unsigned long buffer_size);
unsigned long  TBOX_FileWrite(
                   tTBOX_File    file,
                   void*         buffer,
                   unsigned long buffer_size);
unsigned long  TBOX_FileSeek(tTBOX_File file, long offset, int whence);
unsigned long  TBOX_FileTell(tTBOX_File file);
void           TBOX_FileClose(tTBOX_File file);
unsigned long  TBOX_FileGetSize(tTBOX_File file);

#endif /* TBOX_DEVHOST_FILE_ACCESS */


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif /* TBOX_H */
