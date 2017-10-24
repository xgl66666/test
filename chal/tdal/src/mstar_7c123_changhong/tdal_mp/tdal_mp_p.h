/*
 * tdal_mp_p.h
 *
 *  Created on: 5 Feb 2013
 *      Author: mijovic
 */

#ifndef TDAL_MP_P_H_
#define TDAL_MP_P_H_

#include "crules.h"
#include "tdal_mp.h"
#include "MsTypes2.h"
#include "MSrv_ZmplayerSubtitleDecoder.h"

#include "MsTypes.h"
#include "msAPI_MM.h"

#define kTDAL_MPm_OPEN_SESSION_MAX	10
#define kTDAL_MPm_MAGIC_NUMBER		0xDEADBEEF

typedef struct
{
	uint32_t            	uiMagicNumber;
	bool                	bOpened;
	tTDAL_MP_EvtNotify  	pEvtNotify;
	void               		*pCtx;
	tTDAL_MP_InputDataRead  pInputDataRead;
	tTDAL_MP_InputDataSeek  pInputDataSeek;
	tTDAL_MP_InputDataLength pInputDataLength;
	//STFFMPEG_Handle_t   	STFFMPEGHandle;
	bool                	bFileOpened;
	bool                    bIsPlaying;
	tTDAL_MP_Capabilities   stCaps;
	//tTDAL_MPm_STVIDCodec   *pstStvidCodec;
	//tTDAL_MPm_STAUDCodec   *pstStaudCodec;
	//uint32_t            	uiOutputMask; /* current output mask destination of the audio/video stream */
	tTDAL_MP_Wnd            stWnd;
    uint16_t            	usPictureWidth;  /* width returned by decoder  */
    uint16_t            	usPictureHeight; /* height returned by decoder */
	//uint32_t            	uiPictureDisplayEventCpt;
	int32_t					iSpeed;
	//int32_t             	iSTCDelay;
	//uint32_t            	uiClocksPerSecond;
	//uint32_t            	uiPreviousPTSInTicks;
	//STOS_Clock_t        	uiPreviousTimeInTicks;

	//uint8_t            		*pPrivDataBuf;  /* Pointer to private data buffer that may
	                                    // be needed by some AV codecs */
	//uint32_t            	uiPrivDataSize;/* Size of the private data buffer */

	/* Stream specific parameters:
	 * The video/audio codecs are given by the client
	 * and not probed in the media data file
	 */
	bool                bIsStreamed;
	tTDAL_MP_ContentType  eStreamVidCodecType; /* only relevant if bIsStreamed is TRUE */
	tTDAL_MP_ContentType  eStreamAudCodecType; /* only relevant if bIsStreamed is TRUE */

    //#if 1
    /* TEMP BUGFIX - THIS MODIFICATION SHOULD BE DONE LATER (BY GS) INSIDE  THE MODULE "stffmpeg.c" */
    //bool                bGoingToBeStopped;   /* means that AVP_Stop() have been requested : so we can abort a read() operation */
    //#endif

    //#if 1  /* fix interlock-bug */
    /* fix an interlock-bug when calling AVP_LengthGet when AVP_Start is called by another thread */
    /* the thread calling AVP_Start locks AVPm_ClientLock() and is blocked by MEDIA_RENDERER_AV's ClientLock in Read callback */
    /* the thread calling AVP_LengthGet locked MEDIA_RENDERER_AV's ClientLock and is blocked by AVPm_ClientLock */
    /* note: the AVP_Start can be very long due to the PlayProbe call */
    //bool                bGoingToBeStarted;
    //#endif /* fix interlock-bug */

	//void               *pSpecificDesc;
} tTDAL_MPm_Desc;

IMPORT EN_MPLAYER_MEDIA_TYPE TDAL_MPm_eMediaType;
IMPORT MPLAYER_STREAM_IOCB TDAL_MPm_s_sStreamHandle;
IMPORT MS_U8 TDAL_MPm_u8IsRepeat;
IMPORT tTDAL_MPm_Desc *TDAL_MPi_DescList[kTDAL_MPm_OPEN_SESSION_MAX];

//LOCAL MS_U32 _u32FileOption = MPLAYER_MODE_INVALID;

IMPORT MS_BOOL          is_TDAL_MP_Initialized(void);
IMPORT int TDAL_MPi_GetRunningMediaDescriptor(void);
IMPORT bool TDAL_MPm_Stop(void);

IMPORT void TDAL_MPi_LockMutex(void);
IMPORT void TDAL_MPi_UnlockMutex(void);
IMPORT bool TDAL_MPi_LockMutexIfDifferentThread(void);
#if defined(SDEC_GLUE_JPEG_TDAL_MP_PARTITION)
IMPORT bool TDAL_MPi_ReleasePool(void);
#endif

/*******************************************************************************
 * TDAL_MP SubRender                                                           *
 *******************************************************************************/
MS_BOOL MM_SubRender_Create(void *arg);
MS_BOOL MM_SubRender_GetVideoSizeFromVdec(SBT_Rect *pVideoSize, void *arg);
void MM_SubRender_Open(void *arg);
void MM_SubRender_Close(void *arg);
MS_BOOL MM_SubRender_CreateWindow(U8 u8WinID, SBT_Rect *pRect, MS_BOOL bDisplayAble, void *arg);
MS_BOOL MM_SubRender_DestroyWindow(U8 u8WinID, void *arg);
void MM_SubRender_SetPalette(U8 u8WinID, RGBPaletteColor *pPaletteTbl, U16 u16PaletteSize, U8 u8Index, void *arg);
void MM_SubRender_Bitblt(U8 u8SrcWinID, U8 u8DstWinID, SBT_Rect *pSrcRect, SBT_Rect *pDstRect, void *arg);
void MM_SubRender_ClearWindowByIndex(U8 u8WinID, SBT_Rect *pRect, U8 u8ColorIndex, void *arg);
void MM_SubRender_ClearWindowByColor(U8 u8WinID, SBT_Rect *pRect, RGBPaletteColor *pColor, void *arg);
MS_BOOL MM_SubRender_GetWindowInfo(U8 u8WinID, GWinInfo *pInfo, void *arg);
MS_BOOL MM_SubRender_Lock(U8 u8WinID, EN_GRAPHIC_LOCK_TYPE enLockType, U8 **pu8Buffer, void *arg);
void MM_SubRender_UnLock(U8 u8WinID, void *arg);
MS_BOOL MM_SubRender_RenderFont(U8 *pu8TextBuff, U16 u16TextLength, U8 u8TransPaletteIdx, U8 *pu8DisplayBuffer, U16 u16DisplayWidth, U16 u16DisplayHeight, U16 u16DisplayPitch);
void MM_SubRender_Show(void *arg);
void MM_SubRender_Unshow(void *arg);

#endif /* TDAL_MP_P_H_ */
