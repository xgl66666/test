/******************************************************************************
**
**  Copyright (c) Newport Media Inc.  All rights reserved.
**
**  Module Name:  nmitypes.h
**      These are the typed used by the ASIC driver.
**
*******************************************************************************/

#ifndef _NMI_TYPES_
#define _NMI_TYPES_

#include <linux/types.h>

#define AVOID_INCLUDE_C_FROM_C
#define NMI_TUNER_CALL_DIRECT


#ifndef NULL
#define NULL ((void*)0)
#endif
#if 0
typedef unsigned char           uint8_t;
typedef unsigned long           uint32_t;
typedef unsigned short          uint16_t;
typedef unsigned char           bool_t;
typedef signed short            int16_t;
#endif

typedef unsigned char           bool_t;

typedef unsigned long           u17_15_t;

void print_32hex(uint32_t value);

void nmi_memset(void *pu8Des, uint8_t value, uint32_t len);
void nmi_memcpy(void *pu8Des, void *pu8Src, uint32_t len);
int nmi_abs(int x);
#endif

