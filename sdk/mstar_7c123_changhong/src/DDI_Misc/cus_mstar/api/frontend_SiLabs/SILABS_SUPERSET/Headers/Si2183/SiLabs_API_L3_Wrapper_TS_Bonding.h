#ifndef  _SiLabs_API_L3_Wrapper_TS_Bonding_H_
#define  _SiLabs_API_L3_Wrapper_TS_Bonding_H_
/* Change log:

  As from V2.9.0:
  <new_feature>[TS_Bonding/Duals] Adding TS Bonding capability via SiLabs_API_L3_TS_Bonding.c/.h
        This is only valid for dual demodulators with the TS bonding feature.

 *************************************************************************************************************/

#include "SiLabs_API_L3_Wrapper.h"

#ifdef __cplusplus
extern "C" {
#endif

/* possible values for TS Bonding role */
typedef enum  _CUSTOM_TS_Bonding_Enum       {
  SILABS_TS_CHANNEL_BONDING_OFF = 0,
  SILABS_TS_MASTER              = 1,
  SILABS_TS_SLAVE               = 2,
  SILABS_TS_SLAVE_BRIDGE        = 3
} CUSTOM_TS_Bonding_Enum;

/* Structure used to store all bonding-related information */
typedef struct _SILABS_TS_Bonding      {
  unsigned int        nbFEs;
  SILABS_FE_Context  *fe[4];
  SILABS_FE_Context  *master;
  SILABS_TS_Config   *bridge;
  SILABS_TS_Config   *slave;
  unsigned int        freq[4];
  unsigned int        symbol_rate_kb[4];
  unsigned int        first_dual_output;
  unsigned int        second_dual_input;
} SILABS_TS_Bonding;

extern SILABS_TS_Bonding  TS_Bonding_Context;

signed   int   SiLabs_TS_Bonding_Config          (SILABS_TS_Bonding  *TS_Bonding
                                                , unsigned int        nbFEs
                                                , SILABS_FE_Context  *feX
                                                , SILABS_FE_Context  *feY
                                                , SILABS_FE_Context  *feZ
                                                , SILABS_FE_Context  *feT
                                                , unsigned int        first_dual_output
                                                , unsigned int        second_dual_input
                                                 );

signed   int   SiLabs_TS_Bonding_Activate        (SILABS_TS_Bonding *TS_Bonding
                                                , unsigned int freqX, unsigned int symbol_rate_kbX
                                                , unsigned int freqY, unsigned int symbol_rate_kbY
                                                , unsigned int freqZ, unsigned int symbol_rate_kbZ
                                                 );

signed   int   SiLabs_TS_Bonding_Stop            (SILABS_TS_Bonding *TS_Bonding);

signed   int   SiLabs_TS_Bonding                 (signed int master_freq_kbps
                                                , SILABS_FE_Context  *fe_A, CUSTOM_TS_Bonding_Enum A_role
                                                , SILABS_FE_Context  *fe_B, CUSTOM_TS_Bonding_Enum B_role
                                                , SILABS_FE_Context  *fe_C, CUSTOM_TS_Bonding_Enum C_role
                                                , SILABS_FE_Context  *fe_D, CUSTOM_TS_Bonding_Enum D_role );

#if defined( __cplusplus )
}
#endif

#endif /* _SiLabs_API_L3_Wrapper_TS_Bonding_H_ */
