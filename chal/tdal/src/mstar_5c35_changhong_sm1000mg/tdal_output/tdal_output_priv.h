/******************************************************************************
 *                  COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_OUTPUT_PRIV
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_output/tdal_output_priv.h $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description        : Output library private header.
 *
 *****************************************************************************/

#ifndef   _TDAL_OUTPUT_PRIV_H_
#define   _TDAL_OUTPUT_PRIV_H_

#ifdef   __cplusplus
extern   "C"   {
#endif

/*--------------------------------------*/
/*   Defines               */
/*--------------------------------------*/
#define   kTDAL_OUTPUT_AUDIO_VOLUME_MAX   100

#define   kTDAL_OUTPUTi_NB_VIDEO_DENC      (1)
#define   kTDAL_OUTPUTi_NB_AUDIO_ANA      (1)
#define   kTDAL_OUTPUTi_NB_AUDIO_SPDIF   (1)
#define   kTDAL_OUTPUTi_NB_VIDEO_HD_DAC   (1)
#define   kTDAL_OUTPUTi_NB_HDMI      (1)
#define   kTDAL_OUTPUTi_NB_RFMOD      (1)
#define   kTDAL_OUTPUTi_NB_OUTPUT      (kTDAL_OUTPUTi_NB_VIDEO_DENC   +   kTDAL_OUTPUTi_NB_AUDIO_ANA   +   kTDAL_OUTPUTi_NB_AUDIO_SPDIF   +   kTDAL_OUTPUTi_NB_VIDEO_HD_DAC   +   kTDAL_OUTPUTi_NB_HDMI + kTDAL_OUTPUTi_NB_RFMOD)

#define   mMAY_RETURN_DRIVER_ERROR(   err   ,   fct   )      \
   if   (err   !=   ST_NO_ERROR)                 \
   {                            \
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,        \
            "%s   failed   :   %s\n",   fct,      \
            TBOX_GetDriverErrorText(err)));   \
      mTBOX_RETURN(   eTDAL_OUTPUT_DRIVER_ERROR   );      \
   }                     \
   else                   \
   {                     \
      mTBOX_TRACE((kTBOX_NIV_1,            \
            "%s   OK   :   %s\n",   fct,        \
            TBOX_GetDriverErrorText(err)));   \
   }
#define   mMAY_RETURN_TDAL_OUTPUT_ERROR(   err   ,   fct   )      \
   if   (err   !=   eTDAL_OUTPUT_NO_ERROR)            \
   {                            \
      mTBOX_TRACE((kTBOX_NIV_CRITICAL,        \
            "%s   failed   :   %d\n",   fct,      \
            err));                 \
      mTBOX_RETURN(   err   );               \
   }

/*--------------------------------------*/
/*      Public   types   definition      */
/*--------------------------------------*/

typedef   struct
{
   tTDAL_OUTPUT_VideoDencCapability      stCapability;
   uint32_t            Standard;
   tTDAL_OUTPUT_VideoDencFormat      FormatMask;
   bool            IsColorBarActive;
   bool            IsMacrovisionActive;
}tTDAL_OUTPUTi_VideoDencDescriptor;

typedef   struct
{
   tTDAL_OUTPUT_AudioAnaCapability      stCapability;
   tTDAL_OUTPUT_AudioAnaSpeakerParams      stSpeakerParams;
   tTDAL_OUTPUT_AudioAnaSpeakerAttenuation   stSpeakerAttenuation;
}tTDAL_OUTPUTi_AudioAnaDescriptor;

typedef   struct
{
   tTDAL_OUTPUT_AudioDigitalMode   DigitalMode;
   uint32_t        AudioDelay;
   uint8_t            AudioAdjust;
}tTDAL_OUTPUTi_AudioSPDIFDescriptor;

typedef   struct
{
   uint32_t   uiAudioFrequency;
}tTDAL_OUTPUTi_HDMIDescriptor;

typedef struct
{
  tTDAL_OUTPUT_RFModCapability stCapability;
  tTDAL_OUTPUT_RFModBand Band;
  uint8_t Channel;
  tTDAL_OUTPUT_VideoSDStandard Standard;
}tTDAL_OUTPUTi_RFModDescriptor;

typedef   struct
{
   tTDAL_OUTPUT_OutputId      Id;
   bool            IsEnable;
   tTDAL_OUTPUTi_VideoDencDescriptor   *pstVideoDencDesc;
   tTDAL_OUTPUTi_AudioAnaDescriptor      *pstAudioAnaDesc;
   tTDAL_OUTPUTi_AudioSPDIFDescriptor   *pstAudioSPDIFDesc;
   tTDAL_OUTPUTi_HDMIDescriptor      *pstHDMIDesc;
   tTDAL_OUTPUTi_RFModDescriptor			*pstRFModDesc;
}tTDAL_OUTPUTi_Descriptor;


/*******************************************************
   *   Variables   Declarations   (IMPORT)            *
   *******************************************************/
IMPORT   tTDAL_OUTPUTi_Descriptor      TDAL_OUTPUTi_DescriptorTable[kTDAL_OUTPUTi_NB_OUTPUT];
IMPORT   tTDAL_OUTPUTi_VideoDencDescriptor   TDAL_OUTPUTi_VideoDENCDescriptor[kTDAL_OUTPUTi_NB_VIDEO_DENC];
IMPORT   tTDAL_OUTPUTi_AudioAnaDescriptor      TDAL_OUTPUTi_AudioAnaDescriptor[kTDAL_OUTPUTi_NB_AUDIO_ANA];
IMPORT   tTDAL_OUTPUTi_AudioSPDIFDescriptor   TDAL_OUTPUTi_AudioSPDIFDescriptor[kTDAL_OUTPUTi_NB_AUDIO_SPDIF];
IMPORT   tTDAL_OUTPUTi_HDMIDescriptor      TDAL_OUTPUTi_HDMIDescriptor[kTDAL_OUTPUTi_NB_HDMI];
IMPORT   tTDAL_OUTPUTi_RFModDescriptor TDAL_OUTPUTi_RFModDescriptor[kTDAL_OUTPUTi_NB_RFMOD];
IMPORT   tTDAL_OUTPUT_VideoHDStandard      TDAL_OUTPUT_CurrentVideoHDStandard;

IMPORT   bool      TDAL_OUTPUT_AlreadyInitialized;

IMPORT   tTDAL_OUTPUT_Error TDAL_OUTPUTi_AudioAnaEnable(bool enable);

IMPORT   bool   TDAL_OUTPUTm_SetNewVideoStandard(tTDAL_OUTPUT_VideoHDStandard   newVideoStandard);
IMPORT   void TDAL_OUTPUTm_VEInit();
IMPORT   void TDAL_OUTPUTi_RFMOD_Init(void);
IMPORT   void TDAL_OUTPUTi_RFMOD_Term(void);
IMPORT   tTDAL_OUTPUT_Error TDAL_OUTPUT_RFModEnable(void);
IMPORT   tTDAL_OUTPUT_Error TDAL_OUTPUT_RFModDisable(void);


#ifdef   __cplusplus
}
#endif

#endif   /*_TDAL_AV_PRIV_H_*/
