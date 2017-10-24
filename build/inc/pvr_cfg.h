/******************************************************************************
 *                           COPYRIGHT 2006 IWEDIA                            *
 ******************************************************************************
 *
 * MODULE NAME: CFG XPEL
 *
 * FILE NAME: $URL: http://10.50.1.219/svn/products/st5100_ref/tags/pesi_pvr/2008_04_28_v1.36/build/include/pvr_cfg.h $
 *            $Rev: 45358 $
 *            $Date: 2007-10-25 11:15:20 +0200 (jeu., 25 oct. 2007) $
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _PVR_CFG_H_
#define _PVR_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *  Typedefs                                                                 *
 *****************************************************************************/
typedef enum
{
    /* First compatible volume is choosen   */
    ePVR_CFG_VOLUME_CHOICE_POLICY_FIRST = 0,

    /* Biggest compatible volume is choosen */
    ePVR_CFG_VOLUME_CHOICE_POLICY_BIGGEST = 1,

    /* Volume with most free space is choosen */
    ePVR_CFG_VOLUME_CHOICE_POLICY_MOST_FREE = 2,

    /* User is asked to choose the volume   */
    ePVR_CFG_VOLUME_CHOICE_POLICY_USER_CHOICE = 3

} tPVR_CFG_VolumeChoicePolicy;

typedef enum
{
    /* First usable device is choosen   */
    ePVR_CFG_DEVICE_CHOICE_POLICY_FIRST = 0,

    /* Biggest usable device is choosen */
    ePVR_CFG_DEVICE_CHOICE_POLICY_BIGGEST = 1,

    /* User is asked to choose the device   */
    ePVR_CFG_DEVICE_CHOICE_POLICY_USER_CHOICE = 3

} tPVR_CFG_DeviceChoicePolicy;

typedef enum
{
    /* User activate the PVR in a config menu */
    ePVR_CFG_ACTIVATION_POLICY_MANUAL,

    /* User is prompted to activate (or not) when storage device is
     * plugged to the STB */
    ePVR_CFG_ACTIVATION_POLICY_SEMI_AUTOMATIC,

    /* User is never asked about PVR activation */
    ePVR_CFG_ACTIVATION_POLICY_AUTOMATIC

} tPVR_CFG_ActivationPolicy;


typedef enum
{
    ePVR_CFG_FILTYPE_UNKNOWN                                    = 0x0000,

    ePVR_CFG_FILETYPE_MPEG2TS                                   = 0x0001,

    ePVR_CFG_FILETYPE_PROPRIETARY_METADATA                      = 0x0002,
    ePVR_CFG_FILETYPE_PROPRIETARY_TEMPORARY_METADATA            = 0x0004,
    ePVR_CFG_FILETYPE_PROPRIETARY_MISC                          = 0x0008,
    ePVR_CFG_FILETYPE_PROPRIETARY_TIMEINDEXING                  = 0x0010,
    
    ePVR_CFG_FILETYPE_PLAYLIST_WMP                              = 0x0020,
    ePVR_CFG_FILETYPE_PLAYLIST_MPEG                             = 0x0040,
    ePVR_CFG_FILETYPE_PLAYLIST_WINAMP                           = 0x0080,
#ifdef GS_6301
    ePVR_CFG_FILETYPE_PROPRIETARY_IFRAMEINDEXING                = 0x0100,
#endif
    ePVR_CFG_FILETYPE_PROPRIETARY_THUMBNAIL                     = 0x0200
} tPVR_CFG_FileType_Id;


typedef enum
{
    ePVR_CFG_SPEED_64X_BACKWARD  = 0,
    ePVR_CFG_SPEED_32X_BACKWARD,
    ePVR_CFG_SPEED_16X_BACKWARD,
    ePVR_CFG_SPEED_8X_BACKWARD,
    ePVR_CFG_SPEED_4X_BACKWARD,
    ePVR_CFG_SPEED_2X_BACKWARD,
    ePVR_CFG_SPEED_1X_BACKWARD ,
    ePVR_CFG_SPEED_0X5_BACKWARD ,
    ePVR_CFG_SPEED_0X25_BACKWARD ,
    ePVR_CFG_SPEED_0X ,
    ePVR_CFG_SPEED_0X25_FORWARD ,
    ePVR_CFG_SPEED_0X5_FORWARD ,
    ePVR_CFG_SPEED_1X_FORWARD ,
    ePVR_CFG_SPEED_2X_FORWARD ,
    ePVR_CFG_SPEED_4X_FORWARD ,
    ePVR_CFG_SPEED_8X_FORWARD ,
    ePVR_CFG_SPEED_16X_FORWARD ,
    ePVR_CFG_SPEED_32X_FORWARD ,
    ePVR_CFG_SPEED_64X_FORWARD
} tPVR_CFG_Speed;

typedef enum
{
	ePVR_CFG_Forward = 0,
	ePVR_CFG_Backward
} tPVR_CFG_Direction;


/*****************************************************************************
 *  Functions                                                                *
 *****************************************************************************/

bool            PVR_CFG_GotoLiveAfterEndOfPlayback(void);

bool            PVR_CFG_GotoRecLiveAfterEndOfChasingPlayback(void);

uint32_t        PVR_CFG_GetAutomaticBookMarkDuration(void);

bool            PVR_CFG_DisplayDiskSpaceInTime(void);

tPVR_CFG_VolumeChoicePolicy     PVR_CFG_GetVolumeChoicePolicy_ForUse( void );

tPVR_CFG_VolumeChoicePolicy     PVR_CFG_GetVolumeChoicePolicy_ForActivation( void );

tPVR_CFG_DeviceChoicePolicy     PVR_CFG_GetDeviceChoicePolicy_ForActivation( void );

uint32_t        PVR_CFG_GetMinimumSizeForFullMode( void );

uint32_t        PVR_CFG_GetMinimumSizeForTimeshiftOnlyMode( void );

char *          PVR_CFG_GetDirectoryName( void );

tPVR_CFG_ActivationPolicy       PVR_CFG_GetActivationPolicyForFullMode( void );

tPVR_CFG_ActivationPolicy       PVR_CFG_GetActivationPolicyForTimeshiftOnlyMode( void );

uint32_t        PVR_CFG_GetMaximumSizeForHDDSpeedWarning( void );

bool        PVR_CFG_does_PlayKey_start_LastPlayedMedia(void);

bool        PVR_CFG_does_OKKey_same_PlayKey(void);



char *PVR_CFG_FileType_ExtensionGet(tPVR_CFG_FileType_Id Id);
tPVR_CFG_FileType_Id PVR_CFG_FileType_IdGet(char *Extension);

bool PVR_CFG_PLAYLIST_ListGet(uint8_t *puiFileTypeNb, tPVR_CFG_FileType_Id **pstFileTypeList);

tPVR_CFG_Speed PVR_CFG_ForwardBackward_GetNextSpeed(tPVR_CFG_Speed currentSpeed, tPVR_CFG_Direction direction);

/*===========================================================================*/
#ifdef __cplusplus
}
#endif

#endif /* _XPEL_CFG_H_ */
