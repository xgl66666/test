#include <sys/common/MsTypes.h>
#include <display_logo/DemoFineTune.h>
#include <sys/api/apiPNL.h>

#if( DEMO_FINE_TUNE_SEL == DFT_STD_ORIGINAL_SETTING )
    #include "DFT_STD_ORIGINAL_SETTING.c"
#elif( DEMO_FINE_TUNE_SEL == DFT_STD_FULLHD_CMO216_H1L01 )
    #include "DFT_STD_FULLHD_CMO216_H1L01.c"
#elif( DEMO_FINE_TUNE_SEL == DFT_STD_WXGA_AU20_T200XW02 )
    #include "DFT_STD_WXGA_AU20_T200XW02.c"
#endif


#if ENABLE_DLC
MS_U8 mApi_DLC_GetLumaCurve(MS_U8 u8Index)
{
    return tLumaCurve[u8Index];
}

MS_U8 mApi_DLC_GetLumaCurve2_a(MS_U8 u8Index)
{
    return tLumaCurve2_a[u8Index];
}

MS_U8 mApi_DLC_GetLumaCurve2_b(MS_U8 u8Index)
{
    return tLumaCurve2_b[u8Index];
}
#endif // #if ENABLE_DLC



/*****************************************************************************/

