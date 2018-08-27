/*******************************************************************************
 *
 * FILE NAME          : MxL_Debug.h
 * 
 * AUTHOR             : Brenndon Lee
 *
 * DATE CREATED       : 10/25/2010
 *
 * DESCRIPTION        : Debug header files 
 *
 *******************************************************************************
 *                Copyright (c) 2010, MaxLinear, Inc.
 ******************************************************************************/

#ifndef __MXL_DEBUG_H__
#define __MXL_DEBUG_H__

/******************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
******************************************************************************/

#include <stdio.h>
#include "MaxLinearDataTypes.h"

/******************************************************************************
    Macros
******************************************************************************/

#define NO_DEBUG_OUT

#ifdef NO_DEBUG_OUT
    #define MxL_DLL_DEBUG0(...) {}
#else /* HAS_DEBUG_OUT */
    #define MxL_DLL_DEBUG0 printf
#endif /* NO_DEBUG_OUT */

#endif /* __MXL_DEBUG_H__*/
