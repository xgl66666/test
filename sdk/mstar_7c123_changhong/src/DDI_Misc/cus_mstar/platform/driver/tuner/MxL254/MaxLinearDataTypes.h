/*****************************************************************************************
 *
 * FILE NAME          : MaxLinearDataTypes.h
 * 
 * AUTHOR             : Mariusz Murawski                        
 * DATE CREATED       : Nov/30, 2011
 *
 * DESCRIPTION        : This file contains MaxLinear-defined data types.
 *                      Instead of using ANSI C data type directly in source code
 *                      All module should include this header file.
 *                      And conditional compilation switch is also declared
 *                      here.
 *
 *****************************************************************************************
 *                Copyright (c) 2006, MaxLinear, Inc.
 ****************************************************************************************/
  
#ifndef __MAXLINEAR_DATA_TYPES_H__
#define __MAXLINEAR_DATA_TYPES_H__

/*****************************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
*****************************************************************************************/
//#ifndef _MSC_VER 
//#ifdef __KERNEL__
//#include <linux/types.h>
//#else
//#include <stdint.h>
//#endif
//#endif

/*****************************************************************************************
    Macros
*****************************************************************************************/

/*****************************************************************************************
    User-Defined Types (Typedefs)
*****************************************************************************************/
typedef unsigned char        UINT8;
typedef unsigned short       UINT16;
typedef unsigned int         UINT32;
typedef unsigned long long   UINT64;
typedef char                 SINT8;
typedef short                SINT16;
typedef int                  SINT32;
typedef float                REAL32;
typedef double               REAL64;
typedef unsigned long        ULONG_32;
typedef signed long long     SINT64;


#define MXL_MIN(x, y)                   (((x) < (y)) ? (x) : (y))
#define MXL_MAX(x, y)                   (((x) >= (y)) ? (x) : (y))
#define MXL_ABS(x)                      (((x) >= 0) ? (x) : -(x))
#define MXL_DIV_ROUND(x, y)             (((y)!=0)?(((x) + ((y)/2)) / (y)):0)
// MXL_DIV_ROUND_S round div operation - use only for signed types
#define MXL_DIV_ROUND_S(x, y)           (((y)!=0)?(((x) + ((((x)>=0)?1:-1)*(y)/2)) / (y)):0)

#if (((!defined __MXL_WIN__) || (defined _WIN_CLI_)) && (!defined _KEEP_STDCALL_))
#undef  __stdcall
#define __stdcall
#endif // (((!defined __MXL_WIN__) || (defined _WIN_CLI_)) && (!defined _KEEP_STDCALL_))

typedef UINT32 (__stdcall *MXL_CALLBACK_FN_T)(UINT8 devId, UINT32 callbackType, void * callbackPayload);

typedef enum 
{
  MXL_SUCCESS = 0,
  MXL_FAILURE = 1,
  MXL_INVALID_PARAMETER,
  MXL_NOT_INITIALIZED,
  MXL_ALREADY_INITIALIZED,
  MXL_NOT_SUPPORTED,
  MXL_NOT_READY
} MXL_STATUS_E;

typedef enum 
{
  MXL_DISABLE = 0,
  MXL_ENABLE  = 1,  
        
  MXL_FALSE = 0,
  MXL_TRUE  = 1,  

  MXL_INVALID = 0,
  MXL_VALID   = 1,

  MXL_NO      = 0,
  MXL_YES     = 1,  
  
  MXL_OFF     = 0,
  MXL_ON      = 1  
} MXL_BOOL_E;


/*****************************************************************************************
    Global Variable Declarations
*****************************************************************************************/

/*****************************************************************************************
    Prototypes
*****************************************************************************************/

#endif /* __MAXLINEAR_DATA_TYPES_H__ */

