/*************************************************************************************
                  Silicon Laboratories Broadcast Si2141 Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API properties functions definitions
   FILE: Si2141_Properties_Functions.h
   Supported IC : Si2141
   Compiled for ROM 61 firmware 1_1_build_11
   Revision: 0.2
   Date: December 04 2015
  (C) Copyright 2015, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#ifndef   _Si2141_PROPERTIES_FUNCTIONS_H_
#define   _Si2141_PROPERTIES_FUNCTIONS_H_

void          Si2141_storeUserProperties           (Si2141_PropObj   *prop);
unsigned char Si2141_PackProperty                  (Si2141_PropObj   *prop, unsigned int prop_code, int *data);
unsigned char Si2141_UnpackProperty                (Si2141_PropObj   *prop, unsigned int prop_code, int  data);
void          Si2141_storePropertiesDefaults       (Si2141_PropObj   *prop);

int  Si2141_downloadCOMMONProperties(L1_Si2141_Context *api);
int  Si2141_downloadDTVProperties   (L1_Si2141_Context *api);
int  Si2141_downloadTUNERProperties (L1_Si2141_Context *api);
int  Si2141_downloadAllProperties   (L1_Si2141_Context *api);

#endif /* _Si2141_PROPERTIES_FUNCTIONS_H_ */







