/** \file testhelper.h
 *  \brief Global helper functions to write tests
 *  \author Olivier DOLE
 *  \date 14/03/06
 *  \version 1.0
 */

#ifndef _TEST_HELPER_H
#define _TEST_HELPER_H

#include "tkel.h"
#include "tdal_dmd.h"
#include "tdal_dmx.h"
#include "tdal_avs.h"
#include "tdal_ts.h"
#include "tdal_gfx.h"
#ifdef USE_TDAL_FS
#include "tdal_fs.h"
#endif
#include "tdal_av.h"

#if defined(ANALOG)
#include "tdal_ana_aud.h"
#include "tdal_ana_vid.h"
#include "tdal_vbi_slicer.h"
#include "tdal_ana_input.h"
#include "tdal_hdmi_in.h"
#endif

#define NUMBER_OF_DMD kTDAL_TEST_COMMON_MAX_CONTEXT

#if defined(MEDIUM_SAT)
#define eTDAL_DMD_TYPE eTDAL_DMD_TYPE_QPSK
#elif defined(MEDIUM_TER)
#define eTDAL_DMD_TYPE eTDAL_DMD_TYPE_OFDM
#elif defined(MEDIUM_CAB)
#define eTDAL_DMD_TYPE eTDAL_DMD_TYPE_QAM
#endif

/* export semaphore */
/*! \var TKEL_sema_id TDAL_DMD_TST_NotifySem
 *  \brief Semaphore used for notification in DMD callbacks
 */
/*! \var tTDAL_DMD_NotifyStatus TDAL_DMD_TST_NotifyCond
 *  \brief Variable used to know the status of the DMD
 */
extern TKEL_sema_id             TDAL_HDMI_TST_NotifySem; /* = NULL;*/

/* struct and enum */
typedef enum
{
    eTDAL_TEST_BOOT=30,
    eTDAL_TEST_COMMON,
    eTDAL_TEST_DMD,
    eTDAL_TEST_DMX,
    eTDAL_TEST_GFX,
    eTDAL_TEST_FLA,
    eTDAL_TEST_NVM,
    eTDAL_TEST_HDMI,
    eTDAL_TEST_KBD,
    eTDAL_TEST_DESC,
    eTDAL_TEST_ANA,
	eTDAL_TEST_MP
} tBOOT_ModuleTraceID;

/*! \struct TestManager_Channel
 *  \brief Contain useful info to tune and demux a channel
 */
struct TestManager_Channel
{
    /*! Name of the satelite */
    char *mSatellite;
    /*! Name of the channel */
    char *mChannelName;
    /*! structure containing all info about the signal to tune */
    /* tTDAL_DMD_TunData mTuneData;*/
    uint32_t                    TunFrequency;
#if !defined(ANALOG)
    uint32_t                    SymbolRateBw;
    tTDAL_DMD_Modulation        Modulation;
    tTDAL_DMD_Spectrum          SpecInv;
#endif
#if defined (MEDIUM_SAT)
    tTDAL_DMD_SatTunData    Sat;
#elif defined (MEDIUM_CAB)
    tTDAL_DMD_CabTunData    Cab;
#elif defined (MEDIUM_TER)
    tTDAL_DMD_TerTunData    Ter;
#endif
    /*! PID to demux PMT */
    uint16_t mPMTPID;
    /*! PID to demux video */
    uint16_t mVideoPID;
    /*! PID to demux audio */
    uint16_t mAudioPID;
    /*! PID to demux PCR */
    uint16_t mPCRPID;
	/* video stream type */
	tTDAL_AV_VideoType videoType;
	/* audio stream type */
	tTDAL_AV_AudioType audioType;
	/* video W size */
	uint16_t Width;
	/* video H size */
	uint16_t Height;
};

#if defined(ANALOG)
struct TestManager_Channel_Analog
{
    /*! Name of the satelite */
    char *mSatellite;
    /*! Name of the channel */
    char *mChannelName;
    /*! structure containing all info about the signal to tune */
    uint32_t                    TunFrequency;
    tTDAL_DMD_TerAnalogTunData  AnalogTer;
};

typedef enum
{
	eANALOG_INPUT_TYPE_VIDEO_CVBS,
	eANALOG_INPUT_TYPE_VIDEO_YC,
    eANALOG_INPUT_TYPE_VIDEO_RGB_S,   /* RGB + composite synch signal */
    eANALOG_INPUT_TYPE_VIDEO_RGB_SOG, /* RGB Sync On Green */
	eANALOG_INPUT_TYPE_VIDEO_YPBPR,
	eANALOG_INPUT_TYPE_VIDEO_VGA,
	eANALOG_INPUT_TYPE_AUDIO
} tTestManager_AnalogInputType;

struct TestManager_Input_Analog
{
	char                        *mInputName;
	tTestManager_AnalogInputType eType;
	uint32_t                     Index;
	/* uint32_t                     MuxIndex; not used anymore */
	tTDAL_ANA_VID_Std            eStd;
};
#endif

typedef enum
{
	eCONNECTION_MANAGER_TYPE_ANALOG_TUNER,
	eCONNECTION_MANAGER_TYPE_ANALOG_SCART,
	eCONNECTION_MANAGER_TYPE_ANALOG_YPRPB,
	eCONNECTION_MANAGER_TYPE_ANALOG_PC,
	eCONNECTION_MANAGER_TYPE_DIGITAL
} tTestManager_ConnectionManagerType;

/* DMD functions */
void TestHelper_Initialize_TDAL_DMD( void );
void TestHelper_Terminate_TDAL_DMD( void );
void pTestHelper_DMDStatusFct( tTDAL_DMD_FE eFeID, tTDAL_DMD_NotifyStatus Status );
void pTestHelper_DMDProgressFct( tTDAL_DMD_FE eFeID, tTDAL_DMD_NotifyProgress Progress );
void pTestHelper_DMDGetInfo( int iFeIdx );
void TestHelper_DMDTune(int iFeIdx );
void TestHelper_DMDTuneSpecific(int iFeIdx, tTDAL_DMD_TunData psTunData, uint32_t Frequency);

/* FLASH functions */
void TestHelper_Initialize_TDAL_FLA( void );
void TestHelper_Terminate_TDAL_FLA( void );

/* DMX functions */
void TestHelper_Configure_TDAL_DMX( int iDmxIdx );
void TestHelper_Configure_TDAL_DMX2(int      iDmxIdx,
									uint16_t pid_video,
									uint16_t pid_audio,
									uint16_t pid_pcr);
void TestHelper_Unconfigure_TDAL_DMX( void );
void TestHelper_ControlChannel_TDAL_DMX( int iEnbleChannel );

void TestHelper_Initialize_TDAL_AV( void );
void TestHelper_Terminate_TDAL_AV( void );

/* AVS functions */
void TestHelper_Configure_TDAL_AVS( void );

struct TestManager_Channel *    TestHelper_GetChannel( void );
struct TestManager_Channel *    TestHelper_GetChannelIdx( int index );

/* HDMI functions */
void TestHelper_OpenComponentsForTDAL_HDMI();
void TestHelper_CloseComponentsForTDAL_HDMI();

#if defined(ANALOG)
/* ANALOG functions*/
void TestHelper_Initialize_ANA_AUD ( tTDAL_ANA_AUD_Decoder* IndexDecoder, tTDAL_ANA_AUD_DecoderCapability* pstDecoderCapability  );
void TestHelper_Initialize_ANA_VID ( tTDAL_ANA_VID_Decoder* IndexDecoder, tTDAL_ANA_VID_DecoderCapability* pstDecoderCapability,tTDAL_ANA_VID_Coding eVideoCoding   );
void TestHelper_Terminate_ANA_AUD(tTDAL_ANA_AUD_Decoder IndexDecoder);
void TestHelper_Terminate_ANA_VID(tTDAL_ANA_VID_Decoder IndexDecoder);
void TestHelper_Initialize_VBI_SLICER ( tTDAL_VBI_SLICER_Capability* pstDecoderCapability  );
void TestHelper_RegisterClient_VBI_SLICER(tTDAL_VBI_SLICER_ClientId *pstClientId, tTDAL_VBI_SLICER_Capability* pstDecoderCapability);
void TestHelper_UnRegisterClient_VBI_SLICER(tTDAL_VBI_SLICER_ClientId *pstClientId, tTDAL_VBI_SLICER_Capability* pstDecoderCapability);
void TestHelper_Initialize_ANA_INPUT ( tTDAL_ANA_INPUT_Object *p_VidObject,
                                       tTDAL_ANA_INPUT_Object *p_VidMuxObj,
									   tTDAL_ANA_INPUT_Object *p_AudMuxObj,
									   tTDAL_ANA_VID_Coding  *peVidCoding );

void TestHelper_AskForInput_ANA_INPUT( tTDAL_ANA_INPUT_Object * p_obj, bool bVideo, bool bAudio, tTDAL_ANA_VID_Coding *p_coding, uint32_t *p_MuxIndex);
void TestHelper_Initialize_SCART_INPUT (void);

void TestHelper_Initialize_ANA_VID_SUB ( tTDAL_ANA_VID_Decoder* pIndexDecoder, tTDAL_ANA_VID_DecoderCapability* pstDecoderCapability, tTDAL_ANA_VID_Coding eVideoCoding  );
void TestHelper_Initialize_ANA_INPUT_SUB ( tTDAL_ANA_INPUT_Object *p_VidObject,
                                       tTDAL_ANA_INPUT_Object *p_VidMuxObj,
									   tTDAL_ANA_VID_Coding   *pstVidCoding  );
void TestHelper_Initialize_HDMI_IN_INPUT (tTDAL_HDMI_IN_SinkId *peId);

void TestHelper_CheckPCSettings(void);
void TestHelper_CheckPanelSettings(void);
void TestHelper_CheckSpeakersSettings(void);

#endif

/* Connection Manager functions */
void TestHelper_Configure_TDAL_ConnectionManager(tTestManager_ConnectionManagerType type);

/* TSROUTE functions */
#define TEST_HELPER_TSROUTE_TUNER(i)  mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_SOURCE , eTDAL_TS_TYPE_TUNER , i )
#define TEST_HELPER_TSROUTE_DEMUX(i)  mTDAL_TS_GetObjectId( eTDAL_TS_CLASS_DESTINATION , eTDAL_TS_TYPE_DEMUX , i )
void TestHelper_Configure_TDAL_TSROUTE( tTDAL_TS_Object source, tTDAL_TS_Object demux );
void TestHelper_Unconfigure_TDAL_TSROUTE( tTDAL_TS_Object source, tTDAL_TS_Object demux );

#ifdef USE_TDAL_FS
void TestHelper_AskForVolume_TDAL_FS( tTDAL_FS_VolumeHandle * p_handle );
void TestHelper_Configure_TDAL_FS( void );
void TestHelper_Unconfigure_TDAL_FS( void );
#endif

typedef struct TestHelper_SectionHeader_s {
    uint8_t  TableId;
    uint8_t  Indicators;
    uint16_t SectionLength;
    uint16_t StreamId;
    uint8_t  VersionNumber;
    bool     CurrentNext;
    uint8_t  SectionNumber;
    uint8_t  LastSectionNumber;
} tTestHelper_SectionHeader;

void TestHelper_DVBHeaderExtract( uint8_t* Data_p, tTestHelper_SectionHeader *Header_p );
void TestHelper_DVBBufferDecode( uint8_t *Buffer_p, bool Display );

uint32_t TestHelper_DMXGetStreamHandle(tTDAL_AV_Decoder decoder);

#endif /* #ifndef _TEST_HELPER_H */


