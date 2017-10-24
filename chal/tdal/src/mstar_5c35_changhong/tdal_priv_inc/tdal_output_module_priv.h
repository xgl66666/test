/******************************************************************************
 *                  COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_PRIV
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_priv_inc/tdal_output_module_priv.h $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description           : library primitives private tdal header.
 *
 *****************************************************************************/

#ifndef   _TDAL_OUTPUT_MODULE_PRIV_H_
#define   _TDAL_OUTPUT_MODULE_PRIV_H_


#ifdef   __cplusplus
extern   "C"   {
#endif
/*--------------------------------------*/
/*   Defines               */
/*--------------------------------------*/
#define   kTDAL_OUTPUT_AUDIO_VOLUME_MAX   100

#define   kTDAL_OUTPUTi_NB_VIDEO_DENC   (1)
#define   kTDAL_OUTPUTi_NB_AUDIO_ANA    (1)
#define   kTDAL_OUTPUTi_NB_AUDIO_SPDIF  (0)
#define   kTDAL_OUTPUTi_NB_VIDEO_HD_DAC (0)
#define   kTDAL_OUTPUTi_NB_HDMI         (0)
#define   kTDAL_OUTPUTi_NB_RFMOD        (0)
#define   kTDAL_OUTPUTi_NB_OUTPUT       (kTDAL_OUTPUTi_NB_VIDEO_DENC + \
                                         kTDAL_OUTPUTi_NB_AUDIO_ANA + \
                                         kTDAL_OUTPUTi_NB_AUDIO_SPDIF + \
                                         kTDAL_OUTPUTi_NB_VIDEO_HD_DAC + \
                                         kTDAL_OUTPUTi_NB_HDMI + \
                                         kTDAL_OUTPUTi_NB_RFMOD)
/*--------------------------------------*/
/*      Public   types   definition      */
/*--------------------------------------*/
typedef   struct
{
   uint32_t   LayerWidth;
   uint32_t   LayerHeight;
   bool       IsAspectRatio16To9;
   uint32_t   HDStandard;
}tTDAL_OUTPUT_VideoHDStandardParams;
/*--------------------------------------*/
/*      Public   types   definition      */
/*--------------------------------------*/

typedef   struct
{
   tTDAL_OUTPUT_VideoDencCapability     stCapability;
   uint32_t                             Standard;
   tTDAL_OUTPUT_VideoDencFormat         FormatMask;
   bool                                 IsColorBarActive;
   bool                                 IsMacrovisionActive;
}tTDAL_OUTPUTi_VideoDencDescriptor;

typedef   struct
{
   tTDAL_OUTPUT_AudioAnaCapability          stCapability;
   tTDAL_OUTPUT_AudioAnaSpeakerParams       stSpeakerParams;
   tTDAL_OUTPUT_AudioAnaSpeakerAttenuation  stSpeakerAttenuation;
}tTDAL_OUTPUTi_AudioAnaDescriptor;

typedef   struct
{
   tTDAL_OUTPUT_AudioDigitalMode            DigitalMode;
   uint32_t                                 AudioDelay;
   uint8_t                                  AudioAdjust;
}tTDAL_OUTPUTi_AudioSPDIFDescriptor;

typedef   struct
{
   uint32_t   uiAudioFrequency;
}tTDAL_OUTPUTi_HDMIDescriptor;

typedef struct
{
  tTDAL_OUTPUT_RFModCapability  stCapability;
  tTDAL_OUTPUT_RFModBand        Band;
  uint8_t                       Channel;
  tTDAL_OUTPUT_VideoSDStandard  Standard;
}tTDAL_OUTPUTi_RFModDescriptor;

typedef   struct
{
   tTDAL_OUTPUT_OutputId                Id;
   bool            IsEnable;
   tTDAL_OUTPUTi_VideoDencDescriptor    *pstVideoDencDesc;
   tTDAL_OUTPUTi_AudioAnaDescriptor     *pstAudioAnaDesc;
   tTDAL_OUTPUTi_AudioSPDIFDescriptor   *pstAudioSPDIFDesc;
   tTDAL_OUTPUTi_HDMIDescriptor         *pstHDMIDesc;
   tTDAL_OUTPUTi_RFModDescriptor        *pstRFModDesc;
}tTDAL_OUTPUTi_Descriptor;
/*------------------------------------------*/
/*      Public   Functions   Declarations      */
/*------------------------------------------*/

IMPORT bool TDAL_OUTPUTi_isOutputDISABLING_GFX_ID_1;
IMPORT bool TDAL_GlobalResolutionChangeOnGoing;

/* MAIN */
bool        TDAL_OUTPUTm_isOutputResolutionChangedForGFX_ID_0(void);
void        TDAL_OUTPUTm_resetOutputResolutionChangedForGFX_ID_0(void);

bool        TDAL_OUTPUTm_isOutputResolutionChangedForGFX_ID_2(void);
void        TDAL_OUTPUTm_resetOutputResolutionChangedForGFX_ID_2(void);


/* AUX */
bool        TDAL_OUTPUTm_isOutputResolutionChangedForGFX_ID_1(void);
void        TDAL_OUTPUTm_resetOutputResolutionChangedForGFX_ID_1(void);


#ifdef   __cplusplus
}
#endif

#endif   /*_TDAL_OUTPUT_MODULE_PRIV_H_*/

