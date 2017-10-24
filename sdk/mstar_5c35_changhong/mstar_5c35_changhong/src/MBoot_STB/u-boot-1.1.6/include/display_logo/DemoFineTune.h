#ifndef _DEMO_FINE_TUNE_H_
#define _DEMO_FINE_TUNE_H_

#include <sys/api/apiPNL.h>


#define DLC_PURE_IMAGE_DLC_CURVE        0
#define DLC_PURE_IMAGE_LINEAR_CURVE     1
#define DLC_PURE_IMAGE_DO_NOTHING       2

// ===== Demo Fine Tune item =======================================
#define DFT_NONE                        0
#define DFT_STD_ORIGINAL_SETTING        1
#define DFT_STD_FULLHD_CMO216_H1L01     2
#define DFT_STD_WXGA_AU20_T200XW02      3

// =============================================================

#if( ENABLE_DEMO_FINE_TUNE )
  #ifndef DEMO_FINE_TUNE_SEL
    #define DEMO_FINE_TUNE_SEL  DFT_STD_FULLHD_CMO216_H1L01
  #endif
#else
  #ifndef DEMO_FINE_TUNE_SEL
    #define DEMO_FINE_TUNE_SEL  DFT_STD_ORIGINAL_SETTING
  #endif
#endif // #if( ENABLE_DEMO_FINE_TUNE )

// =============================================================
#if( DEMO_FINE_TUNE_SEL == DFT_STD_ORIGINAL_SETTING )
    #include "display_logo/DFT_STD_ORIGINAL_SETTING.h"
#elif( DEMO_FINE_TUNE_SEL == DFT_STD_FULLHD_CMO216_H1L01 )
    #include "display_logo/DFT_STD_FULLHD_CMO216_H1L01.h"
#elif( DEMO_FINE_TUNE_SEL == DFT_STD_WXGA_AU20_T200XW02 )
    #include "display_logo/DFT_STD_WXGA_AU20_T200XW02.h"
#endif

extern BYTE *tAllGammaTab[3];
extern APIPNL_GAMMA_MAPPEING_MODE GAMMA_MAPPING_MODE;



/*****************************************************************************/
#endif // _DEMO_FINE_TUNE_H_

