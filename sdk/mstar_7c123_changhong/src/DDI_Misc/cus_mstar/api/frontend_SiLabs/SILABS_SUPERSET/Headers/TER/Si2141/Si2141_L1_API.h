/*************************************************************************************
                  Silicon Laboratories Broadcast Si2141 Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API functions prototypes used by commands and properties
   FILE: Si2141_L1_API.h
   Supported IC : Si2141
   Compiled for ROM 61 firmware 1_1_build_11
   Revision: 0.2
   Date: December 04 2015
  (C) Copyright 2015, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#ifndef    _Si2141_L1_API_H_
#define    _Si2141_L1_API_H_

#include "Silabs_L0_API.h"


#include "Si2141_Commands.h"
#include "Si2141_Properties.h"
#include "Si2141_typedefs.h"
#include "Si2141_Commands_Prototypes.h"
#include "Si2141_Properties_Functions.h"

#define NO_Si2141_ERROR                     0x00
#define ERROR_Si2141_PARAMETER_OUT_OF_RANGE 0x01
#define ERROR_Si2141_ALLOCATING_CONTEXT     0x02
#define ERROR_Si2141_SENDING_COMMAND        0x03
#define ERROR_Si2141_CTS_TIMEOUT            0x04
#define ERROR_Si2141_ERR                    0x05
#define ERROR_Si2141_POLLING_CTS            0x06
#define ERROR_Si2141_POLLING_RESPONSE       0x07
#define ERROR_Si2141_LOADING_FIRMWARE       0x08
#define ERROR_Si2141_LOADING_BOOTBLOCK      0x09
#define ERROR_Si2141_STARTING_FIRMWARE      0x0a
#define ERROR_Si2141_SW_RESET               0x0b
#define ERROR_Si2141_INCOMPATIBLE_PART      0x0c
#define ERROR_Si2141_TUNINT_TIMEOUT         0x0d
#define ERROR_Si2141_xTVINT_TIMEOUT         0x0e
#define ERROR_Si2141_UNKNOWN_COMMAND        0xf0
#define ERROR_Si2141_UNKNOWN_PROPERTY       0xf1


unsigned char Si2141_L1_API_Init    (L1_Si2141_Context *api, int add);
unsigned char Si2141_L1_API_Patch   (L1_Si2141_Context *api, int iNbBytes, unsigned char *pucDataBuffer);
unsigned char Si2141_L1_CheckStatus (L1_Si2141_Context *api);
char*         Si2141_L1_API_ERROR_TEXT(int error_code);
char*         Si2141_L1_API_TAG_TEXT  (void);

#define Si2141_TAG Si2141_L1_API_TAG_TEXT


#ifdef    Si2141_GET_PROPERTY_STRING
void Si2141_L1_FillPropertyStringText(L1_Si2141_Context *api, unsigned int prop_code, const char *separator, char *msg);
#endif /* Si2141_GET_PROPERTY_STRING */

#endif /* _Si2141_L1_API_H_ */








