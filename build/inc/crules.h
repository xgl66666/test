/******************************************************************************
 *                    COPYRIGHT 2006 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: None
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/crules.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: This file includes the definitions of the C objects
 *              needed to apply C coding rules.
 *
 *****************************************************************************/

#ifndef _CRULES_H_
#define _CRULES_H_

/*******************************************************/
/*              Includes                               */
/*******************************************************/
#if defined(WIN32) && !defined(PRODUCT_PC_SIM)
#include <windows.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*              Defines                                */
/*******************************************************/

/* KEYWORDS */
#ifndef LOCAL
#define LOCAL  static
#endif
#ifndef GLOBAL
#define GLOBAL
#endif
#ifndef IMPORT
#define IMPORT extern
#endif
#ifndef MODULE
#define MODULE
#endif

#ifndef _SIZE_
	#ifndef __INLINE
	#define __INLINE	__inline
	#endif
#else
	#ifndef __INLINE
	#define __INLINE
	#endif
#endif



#ifndef REGISTER
#define REGISTER  register
#endif
#ifndef VOLATILE
#define VOLATILE  volatile
#endif
#ifndef INTERRUPT
#define INTERRUPT
#endif

/* BOOLEAN TYPE AND VALUES */

#ifndef __cplusplus
#ifndef __bool_true_false_are_defined

#ifdef BOOL_TYPE
#define _Bool BOOL_TYPE
#else
#define _Bool char
#endif

#define bool  _Bool

#ifndef true
#define true  1
#endif

#ifndef false
#define false 0
#endif
#endif //__cplusplus
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#define __bool_true_false_are_defined 1
#endif

/* EXACT-WIDTH INTEGER TYPES */

#if defined(PRODUCT_PC_SIM)
#include <stdint.h>
#else
	
#ifndef int8_t
#define int8_t signed char
#endif

#ifndef int16_t
#define int16_t signed short
#endif

#ifndef int32_t
#define int32_t signed int
#endif

#ifndef int64_t
#define int64_t signed long long
#endif

#ifndef uint8_t
#define uint8_t unsigned char
#endif

#ifndef uint16_t
#define uint16_t unsigned short
#endif

#ifndef uint32_t
#define uint32_t unsigned int
#endif

#ifndef uint64_t
#define uint64_t unsigned long long
#endif

/* LIMITS OF EXACT-WIDTH INTEGER TYPES */

#ifndef INT8_MIN
#define INT8_MIN -127
#endif

#ifndef INT16_MIN
#define INT16_MIN -32767
#endif

#ifndef INT32_MIN
#define INT32_MIN -2147483647
#endif

#ifndef INT64_MIN
#define INT64_MIN -9223372036854775807
#endif

#ifndef INT8_MAX
#define INT8_MAX +127
#endif

#ifndef INT16_MAX
#define INT16_MAX +32767
#endif

#ifndef INT32_MAX
#define INT32_MAX +2147483647
#endif

#ifndef INT64_MAX
#define INT64_MAX +9223372036854775807
#endif

#ifndef UINT8_MAX
#define UINT8_MAX 255
#endif

#ifndef UINT16_MAX
#define UINT16_MAX 65535
#endif

#ifndef UINT32_MAX
#define UINT32_MAX 4294967295U
#endif

#ifndef UINT64_MAX
#define UINT64_MAX 18446744073709551615
#endif

/* INTEGER TYPES CAPABLE OF HOLDING OBJECT POINTERS */

#ifndef intptr_t
#define intptr_t int
#endif

#ifndef uintptr_t
#define uintptr_t unsigned int
#endif

#endif //

/* NULL POINTER CONSTANT */

#ifndef NULL
#define NULL 0
#endif


/*******************************************************/
/*              Macros                                 */
/*******************************************************/


/*******************************************************/
/*              Typedefs                               */
/*******************************************************/


/*******************************************************/
/*              Variables Declarations (IMPORT)        */
/*******************************************************/


/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/


#ifdef __cplusplus
}
#endif


#endif /*_CRULES_H_*/
