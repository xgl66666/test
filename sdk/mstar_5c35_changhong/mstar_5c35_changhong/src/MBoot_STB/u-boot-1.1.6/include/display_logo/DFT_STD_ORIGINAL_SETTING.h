
#define FINE_TUNE      "DFT_STD_ORIGINAL_SETTING"

#if ENABLE_DLC
#define DLC_PARAMETER_L_L_U                12
#define DLC_PARAMETER_L_L_D                5
#define DLC_PARAMETER_L_H_U                4
#define DLC_PARAMETER_L_H_D                3
#define DLC_PARAMETER_S_L_U                128
#define DLC_PARAMETER_S_L_D                128
#define DLC_PARAMETER_S_H_U                128
#define DLC_PARAMETER_S_H_D                192
#define DLC_PARAMETER_PURE_IMAGE_MODE      E_XC_DLC_PURE_IMAGE_DO_NOTHING
#define DLC_PARAMETER_LEVEL_LIMIT          1
#define DLC_PARAMETER_AVG_DELTA            12
#define DLC_PARAMETER_AVG_DELTA_STILL      0
#define DLC_PARAMETER_FAST_ALPHA_BLENDING  31
#define DLC_PARAMETER_Y_AVG_THRESHOLD_L    0
#define DLC_PARAMETER_Y_AVG_THRESHOLD_H    128
#define DLC_PARAMETER_BLE_POINT            48
#define DLC_PARAMETER_WLE_POINT            48
#define DLC_PARAMETER_ENABLE_BLE           0
#define DLC_PARAMETER_ENABLE_WLE           0

#define DLC_CGC_ENABLE                     0
#define DLC_CGC_CGAIN_OFFSET               0x08
#define DLC_CGC_CGAIN_LIMIT_H              0x38
#define DLC_CGC_CGAIN_LIMIT_L              0x08
#define DLC_CGC_YC_SLOPE                   0x0C
#define DLC_CGC_Y_TH                       0x01

extern MS_U8 mApi_DLC_GetLumaCurve(MS_U8 u8Index);
extern MS_U8 mApi_DLC_GetLumaCurve2_a(MS_U8 u8Index);
extern MS_U8 mApi_DLC_GetLumaCurve2_b(MS_U8 u8Index);
#endif // #if ENABLE_DLC

