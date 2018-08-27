/***************************************************************************************
                  Silicon Laboratories Broadcast Si2141 Layer 2 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   L2 API header for commands and properties
   FILE: Si2141_L2_API.h
   Supported IC : Si2141
   Compiled for ROM 61 firmware 1_1_build_11
   Revision: 0.2
   Date: December 04 2015
  (C) Copyright 2015, Silicon Laboratories, Inc. All rights reserved.
****************************************************************************************/

#ifndef   Si2141_L2_API_H
#define   Si2141_L2_API_H

#include "Si2141_L1_API.h"
#include "Si2141_Properties_Strings.h"

#ifdef    TUNER_ONLY
/* define some externals, required for VCO_BLOCKING */
/* only used for TUNER_ONLY mode */
extern int fe;
extern L1_Si2141_Context  FrontEnd_Table[];
#endif // TUNER_ONLY

int  Si2141_Init                      (L1_Si2141_Context *api);
int  Si2141_Configure                 (L1_Si2141_Context *api);
int  Si2141_PowerUpWithPatch          (L1_Si2141_Context *api);
int Si2141_PowerUpUsingBroadcastI2C    (L1_Si2141_Context *tuners[], int tuner_count );
int Si2141_PowerUpOnly                 (L1_Si2141_Context *api);
int  Si2141_LoadFirmware              (L1_Si2141_Context *api, unsigned char *fw_table, int lines);
int  Si2141_StartFirmware             (L1_Si2141_Context *api);

int Si2141_LoadFirmware_16    (L1_Si2141_Context *api, firmware_struct fw_table[], int nbLines);
int Si2141_GetRF                 (L1_Si2141_Context *api);
int Si2141_Tune                  (L1_Si2141_Context *api, unsigned char mode, unsigned long freq);
int Si2141_DTVTune               (L1_Si2141_Context *api, unsigned long freq, unsigned char bw, unsigned char modulation, unsigned char invert_spectrum);
int Si2141_XoutOn                (L1_Si2141_Context *api);
int Si2141_XoutOff               (L1_Si2141_Context *api);
int Si2141_Standby               (L1_Si2141_Context *api);
int Si2141_Powerdown             (L1_Si2141_Context *api);
int Si2141_LoadVideofilter       (L1_Si2141_Context *api, vid_filt_struct vidFiltTable[], int lines);
int Si2141_AGC_Override    (L1_Si2141_Context *api, unsigned char mode );
int Si2141_Tuner_Block_VCO       (L1_Si2141_Context *api, int vco_code);
int Si2141_Tuner_Block_VCO2       (L1_Si2141_Context *api, int vco_code);
int Si2141_Tuner_Block_VCO3       (L1_Si2141_Context *api, int vco_code);
int Si2141_L2_VCO_Blocking_PostTune(L1_Si2141_Context *tuners[], int tuner_num, int tuner_count);
int Si2141_L2_VCO_Blocking_PreTune(L1_Si2141_Context *tuners[], int tuner_num, int tuner_count);
int Si2141_SetVcoProperties (L1_Si2141_Context *api);

#define Si2141_FORCE_NORMAL_AGC       0
#define Si2141_FORCE_MAX_AGC          1
#define Si2141_FORCE_TOP_AGC          2

#endif /* Si2141_L2_API_H */







