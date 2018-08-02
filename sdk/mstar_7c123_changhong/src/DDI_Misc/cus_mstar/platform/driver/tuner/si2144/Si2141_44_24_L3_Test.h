/***************************************************************************************
                  Silicon Laboratories Broadcast Si2141_44_24 Layer 3 console application

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   L3 sample application header
   FILE: Si2141_44_24_L3_Test.h
   Supported IC : Si2141-A10, Si2141-B10, Si2144-A20, Si2124-A20
   Compiled for ROM 61 firmware 1_1_build_10
   Revision: 0.1
   Date: July 24 2015
  (C) Copyright 2015, Silicon Laboratories, Inc. All rights reserved.
****************************************************************************************/

#ifndef   Si2141_44_24_L3_TEST_H
#define   Si2141_44_24_L3_TEST_H

#define   Si2141_44_24_COMMAND_PROTOTYPES

#include "Si2141_44_24_L2_API.h"
#include "Si2141_44_24_Commands_Prototypes.h"

#define   Si2141_44_24_BASE_ADDRESS 0xc0
#define   SYSTEM_PAUSE system("pause")

void Si2141_44_24_configure_i2c_communication    (L1_Si2141_44_24_Context *api);

#ifdef    Si2141_44_24_COMMAND_LINE_APPLICATION

int  Si2141_44_24_DisplayStatusByte              (L1_Si2141_44_24_Context *api);
int  Si2141_44_24_GetRevision                    (L1_Si2141_44_24_Context *api);
int  Si2141_44_24_TunerStatus                    (L1_Si2141_44_24_Context *Si2141_44_24);
int  Si2141_44_24_GetUserFrequency               (unsigned long *fIn);
int  Si2141_44_24_help                           (void);
void Si2141_44_24_menu                           (unsigned char full);
int  Si2141_44_24_demoLoop                       (L1_Si2141_44_24_Context *api, char* choice);

#endif /* Si2141_44_24_COMMAND_LINE_APPLICATION */

#endif /* Si2141_44_24_L3_TEST_H */







