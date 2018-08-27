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

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/err.h>
#include <asm/uaccess.h>
#else
#include <stdio.h>
#endif
#include "MaxLinearDataTypes.h"
#include "drvTuner.h"
/******************************************************************************
    Macros
******************************************************************************/

#define MxL_DLL_DEBUG0 printf


#endif /* __MXL_DEBUG_H__*/
