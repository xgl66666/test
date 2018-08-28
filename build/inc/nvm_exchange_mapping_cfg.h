/******************************************************************************
 *                    COPYRIGHT 2004 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: NVM EXCHANGE
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/idecode/product/comedia/branches/snt_cos/main/build/inc/nvm_exchange_mapping_cfg.h $
 *            $Rev: 9231 $
 *            $Date: 2012-02-16 18:29:05 +0100 (jeu., 16 févr. 2012) $
 *
 * PUBLIC
 *
 * DESCRIPTION: nvm exchange configuration
 *
 *****************************************************************************/

#ifndef NVM_EXCHANGE_MAPPING_CFG_H
#define NVM_EXCHANGE_MAPPING_CFG_H


/*---------------------------------------------------*/
/*      General constant, type and enum definition   */
/*---------------------------------------------------*/




typedef enum
{
    eOAD_FEC_NONE,
    eOAD_FEC_1_2,
    eOAD_FEC_2_3,   /* only used for VSB (US market) */
    eOAD_FEC_3_4,
    eOAD_FEC_4_5,   /* only used for DVB-T2 */
    eOAD_FEC_5_6,
    eOAD_FEC_6_7,   /* not used normally in terrestrial */
    eOAD_FEC_7_8,
    eOAD_FEC_8_9,   /* not used normally in satellite and terrestrial */
    eOAD_FEC_3_5,   /* only used for DVB-T2 */
    eOAD_FEC_AUTO,
    eOAD_FEC_MAX
}tOAD_Convolution;

/*------------------------------------------*/
/*  satellite Front-End types Declarations  */
/*------------------------------------------*/
typedef enum
{
	eOAD_POLAR_HORIZONTAL,
	eOAD_POLAR_VERTICAL,
	eOAD_POLAR_CIRCULAR_LEFT,      /* not implemented */
	eOAD_POLAR_CIRCULAR_RIGHT,     /* not implemented */
	eOAD_POLAR_ALL,                /* not implemented */
	eOAD_POLAR_NONE                /* LNB OFF */
} tOAD_LNB_Polarization;


typedef struct
{
    tOAD_Convolution	    Convolution;
	tOAD_LNB_Polarization	Polarization;
}tOAD_SatTunData;

/*------------------------------------------*/
/*    cable Front-End types Declarations    */
/*------------------------------------------*/
typedef uint32_t tOAD_CabTunData;

/*------------------------------------------*/
/* terrestrial Front-End types Declarations */
/*------------------------------------------*/
#define kOAD_BW_6MHZ 6000000
#define kOAD_BW_7MHZ 7000000
#define kOAD_BW_8MHZ 8000000
#define kOAD_BW_5MHZ 5000000
#define kOAD_BW_10MHZ 10000000
#define kOAD_BW_1_7MHZ 1700000


typedef enum
{
    eOAD_MODE_2K,
    eOAD_MODE_8K,
    eOAD_MODE_4K,    /* only used for DVB-T2 */
    eOAD_MODE_1K,    /* only used for DVB-T2 */
    eOAD_MODE_16K,   /* only used for DVB-T2 */
    eOAD_MODE_32K,   /* only used for DVB-T2 */
    eOAD_MODE_AUTO,
    eOAD_MODE_MAX
}tOAD_TerMode;
typedef enum
{
    eOAD_GUARD_1_32,
    eOAD_GUARD_1_16,
    eOAD_GUARD_1_8,
    eOAD_GUARD_1_4,
    eOAD_GUARD_1_128,    /* only used for DVB-T2 */
    eOAD_GUARD_19_128,   /* only used for DVB-T2 */
    eOAD_GUARD_19_256,   /* only used for DVB-T2 */
    eOAD_GUARD_AUTO,
    eOAD_GUARD_MAX
}tOAD_TerGuard;

typedef enum
{
    eOAD_HIER_NONE,
    eOAD_HIER_1,
    eOAD_HIER_2,
    eOAD_HIER_4,
    eOAD_HIER_AUTO,
    eOAD_HIER_MAX
}tOAD_TerHierarchy;

typedef enum
{
    eOAD_PRIORITY_STREAM_HIGH,
    eOAD_PRIORITY_STREAM_LOW,
    eOAD_PRIORITY_STREAM_MAX
}tOAD_TerPrioStream;

typedef enum
{
    eOAD_OFFSET_NONE,
    eOAD_OFFSET_1_6_MORE, 	    /* + 1/6 Mhz */
    eOAD_OFFSET_1_6_MINUS,	    /* - 1/6 Mhz */
    eOAD_OFFSET_2_6_MORE,	    /* + 2/6 Mhz */
    eOAD_OFFSET_2_6_MINUS, 	    /* - 2/6 Mhz */
    eOAD_OFFSET_3_6_MORE, 	    /* + 3/6 Mhz */
    eOAD_OFFSET_3_6_MINUS,  	/* - 3/6 Mhz */
    eOAD_OFFSET_AUTO,
    eOAD_OFFSET_MAX
}tOAD_TerFreqOff;

typedef struct
{
    tOAD_TerMode		    Mode;
    tOAD_TerGuard 		    Guard;
    tOAD_TerHierarchy	    Hierarchy;
    tOAD_TerPrioStream	    Priority;
    tOAD_Convolution	    ConvolutionHP;
    tOAD_Convolution	    ConvolutionLP;
    tOAD_TerFreqOff		    Offset;
    int32_t                 PlpID;      /* only used for DVB-T2 */
    int32_t                 PlpIdx;      /* only used for DVB-T2 */
    int32_t                 PlpNB;      /* only used for DVB-T2 */
    uint16_t                T2SystemId; /* only used for DVB-T2 */
}tOAD_TerTunData;
/*------------------------------------------*/
/*   common Front-End types Declarations    */
/*------------------------------------------*/
typedef enum
{
	eOAD_NO_ERROR = 0,
	eOAD_BAD_DEMODULATOR,
	eOAD_BAD_PARAMETER,
	eOAD_DEVICE_BUSY,
    eOAD_NOT_SCANNING,
    eOAD_NOT_LOCKED
} tOAD_Error;


typedef enum
{
	eOAD_LOCKED = 0,
	eOAD_LOCK_FAILED,
	eOAD_FOUND,
	eOAD_END_OF_RANGE,
	eOAD_SIGNAL_LOST
} tOAD_NotifyStatus;

typedef enum
{
    eOAD_MOD_BPSK = 0,
    eOAD_MOD_QPSK,
    eOAD_MOD_QAM4 = eOAD_MOD_QPSK,
    eOAD_MOD_8PSK,
    eOAD_MOD_QAM16,
    eOAD_MOD_QAM32,
    eOAD_MOD_QAM64,
    eOAD_MOD_QAM128,
    eOAD_MOD_QAM256,
    eOAD_MOD_AUTO,
    eOAD_MOD_MAX
} tOAD_Modulation;

typedef enum
{
	eOAD_SPECTRUM_AUTO = 0,
	eOAD_SPECTRUM_NORMAL,
	eOAD_SPECTRUM_INVERTED,
	eOAD_SPECTRUM_MAX
} tOAD_Spectrum;

typedef union
{
    tOAD_SatTunData	Sat;
    tOAD_CabTunData Cab;
    tOAD_TerTunData	Ter;
} tOAD_SpecificTunData;

typedef struct
{
    uint32_t	            TunFrequency;
    uint32_t                SymbolRateBw;
    tOAD_Modulation         Modulation;
    tOAD_Spectrum           SpecInv;
    tOAD_SpecificTunData	Specific;
} tOAD_TunData;

#ifdef MEDIUM_SAT
    /* Out Door Unit definition */
    typedef enum
    {
        eOAD_ODU_TRACK_NONE,
        eOAD_ODU_TRACK_1,
        eOAD_ODU_TRACK_2,
        eOAD_ODU_TRACK_3,
        eOAD_ODU_TRACK_4
    }tOAD_ODU_TRACK;
    
    typedef enum
    {
        eOAD_ODU_TONE_NONE,
        eOAD_ODU_TONE_OFF,
        eOAD_ODU_TONE_ON
    }tOAD_ODU_ToneState;
    
    typedef enum
    {
        eOAD_ODU_POWER_NONE,
        eOAD_ODU_POWER_13V,
        eOAD_ODU_POWER_18V
    }tOAD_ODU_Power;
    
    typedef struct
    {
        tOAD_ODU_TRACK          TrackIndex;
        tOAD_ODU_ToneState      ToneState;
        tOAD_ODU_Power          Power;
    }tOAD_ODU;

    typedef struct
    {
        uint32_t        LowFreq;
        uint32_t        HighFreq;
        tOAD_ODU        OduInfo;
    } tOAD_SatInfo;
#endif

typedef struct
{
    tOAD_TunData        TunData;
#ifdef MEDIUM_SAT
    tOAD_SatInfo        SatInfo;
#endif
	tOAD_NotifyStatus   CarrierStatus;
    uint8_t 			SignalQuality;
    uint32_t 			SignalBER;
    uint8_t 			SignalLevel;
} tOAD_Info;

/* Error Delivered by the DownLoader */
typedef enum
{

    NVM_ERR_NO_ERROR,                 /* 0x00 */

    NVM_ERR_TUNER_SEARCH,             /* 0x01 */
    NVM_ERR_DMX_NO_SECTION_RECEPTION, /* 0x02 */

    NVM_ERR_RAM_ACCESS,               /* 0x03 */
    NVM_ERR_FLH_ACCESS,               /* 0x04 */
    NVM_ERR_EEP_ACCESS,               /* 0x05 */

    NVM_ERR_SLD_SEND_FRAME,           /* 0x06 */
    NVM_ERR_SLD_READ_FRAME,           /* 0x07 */
    NVM_ERR_SLD_NO_ACK,               /* 0x08 */

    NVM_ERR_DL_STATE,                 /* 0x09 */
    
    NVM_ERR_DATA_NOT_FOUND,           /* 0x0A */
    NVM_ERR_DATA_INCOMPLETE,          /* 0x0B */
    NVM_ERR_DATA_EXCEED_SIZE,         /* 0x0C */
    NVM_ERR_DATA_BAD_CRC,             /* 0x0D */

    NVM_ERR_BAD_UPDATEID,             /* 0x0E */

    NVM_ERR_BAD_OUI,                  /* 0x0F */

    NVM_ERR_BAD_START_ADDRESS,        /* 0x10 */
    NVM_ERR_BAD_SIGNATURE,            /* 0x11 */

    NVM_ERR_DRIVER,                   /* 0x12 */
    
    NVM_ERR_FAILED,                   /* 0x13 */
    
    NVM_ERR_NVM,
    NVM_ERR_DOWNLOADER_REQUIRED  = 0x20,
    NVM_ERR_DOWNLOADER_INTERRUPTED
      
} tNVM_error_code;

/********************************************************/
/*              Macros                                  */
/********************************************************/
#define mOAD_CONVERT_MODULATION(x)  ((x==eDMD_MOD_BPSK) ? eOAD_MOD_BPSK : \
                                    ((x==eDMD_MOD_QPSK) ? eOAD_MOD_QPSK : \
                                    ((x==eDMD_MOD_8PSK) ? eOAD_MOD_8PSK : \
                                    ((x==eDMD_MOD_QAM16) ? eOAD_MOD_QAM16 : \
                                    ((x==eDMD_MOD_QAM32) ? eOAD_MOD_QAM32 : \
                                    ((x==eDMD_MOD_QAM64) ? eOAD_MOD_QAM64 : \
                                    ((x==eDMD_MOD_QAM128) ? eOAD_MOD_QAM128 : \
                                    ((x==eDMD_MOD_QAM256) ? eOAD_MOD_QAM256 : eOAD_MOD_AUTO))))))))

#define mOAD_CONVERT_SPECTRUM(x)    ((x==eDMD_SPECTRUM_NORMAL) ? eOAD_SPECTRUM_NORMAL : \
                                    ((x==eDMD_SPECTRUM_INVERTED) ? eOAD_SPECTRUM_INVERTED : eOAD_SPECTRUM_AUTO))

#define mOAD_CONVERT_MODE(x)        ((x==eDMD_MODE_2K) ? eOAD_MODE_2K : \
                                    ((x==eDMD_MODE_8K) ? eOAD_MODE_8K : \
                                    ((x==eDMD_MODE_4K) ? eOAD_MODE_4K : \
                                    ((x==eDMD_MODE_1K) ? eOAD_MODE_1K : \
                                    ((x==eDMD_MODE_16K) ? eOAD_MODE_16K : \
                                    ((x==eDMD_MODE_32K) ? eOAD_MODE_32K : eOAD_MODE_AUTO))))))

#define mOAD_CONVERT_PRIORITY(x)    ((x==eDMD_PRIORITY_STREAM_HIGH) ? eOAD_PRIORITY_STREAM_HIGH : eOAD_PRIORITY_STREAM_LOW)

#define mOAD_CONVERT_HIERARCHY(x)   ((x==eDMD_HIER_NONE) ? eOAD_HIER_NONE : \
                                    ((x==eDMD_HIER_1) ? eOAD_HIER_1 : \
                                    ((x==eDMD_HIER_2) ? eOAD_HIER_2 : \
                                    ((x==eDMD_HIER_4) ? eOAD_HIER_4 : eOAD_HIER_AUTO))))

#define mOAD_CONVERT_OFFSET(x)      ((x==eDMD_OFFSET_NONE) ? eOAD_OFFSET_NONE : \
                                    ((x==eDMD_OFFSET_1_6_MORE) ? eOAD_OFFSET_1_6_MORE : \
                                    ((x==eDMD_OFFSET_1_6_MINUS) ? eOAD_OFFSET_1_6_MINUS : \
                                    ((x==eDMD_OFFSET_2_6_MORE) ? eOAD_OFFSET_2_6_MORE : \
                                    ((x==eDMD_OFFSET_2_6_MINUS) ? eOAD_OFFSET_2_6_MINUS : \
                                    ((x==eDMD_OFFSET_3_6_MORE) ? eOAD_OFFSET_3_6_MORE : \
                                    ((x==eDMD_OFFSET_3_6_MINUS) ? eOAD_OFFSET_3_6_MINUS : eOAD_OFFSET_AUTO)))))))


/*
** TO DO clean up __REMOVE_NVMEX__
*/
#define __REMOVE_NVMEX__

#ifndef __REMOVE_NVMEX__
typedef struct _OPAQUE_DIALOG_AREA_STRUCTURE_ { void* notused; } tNVM_Dialog_Area;

/* For Custom Data purpose in the futur */
typedef struct
{
    uint32_t uDummy;
}tNVM_Custom;

/* For serial UART connection */
/* this structure is functionaly equivalent to tTDAL_UART_Connect_parameters */
/* but size of enum field are reduced and size of the sructure is 2 word of 32 bits */
typedef    struct
{
    uint16_t                   uUartInstance;
    uint16_t                   uUartRate;
    uint8_t                     uUartDataBit;
    uint8_t                     uUartStopBit;
    uint8_t                     uUartParity;
    uint8_t                     uUartFlowControl;
} tNVM_UartParameter;
    

/*---------------------------*/
/*   Dialog Area structure   */
/*---------------------------*/

struct _OPAQUE_DIALOG_AREA_STRUCTURE_;

typedef struct _OPAQUE_DIALOG_AREA_STRUCTURE_
{
    uint32_t               uUpdateID;

#if defined (MEDIUM_SAT)
    uint32_t               uFrequency;         /* frequency in KHz   */
    uint32_t               uSymbol_rate;       /* frequency in bauds */
    tOAD_LNB_Polarization  ePolarization;
    tOAD_Modulation        eModulation;
    tOAD_Convolution       eFec_rate;
    uint32_t               uLow_freq;          /* frequency in MHz */
    uint32_t               uHigh_freq;         /* frequency in MHz */
    tOAD_ODU               OduInfo;

#elif defined (MEDIUM_CAB)
    uint32_t               uFrequency;         /* frequency in Hz    */
    uint32_t               uSymbol_rate;       /* frequency in bauds */
    tOAD_Modulation        eModulation;

#elif defined (MEDIUM_TER)
    uint32_t               uFrequency;         /* frequency in Hz */
    uint32_t               uBandwidth;         /* bandwidth in Hz */
    tOAD_Modulation        eModulation;
    tOAD_TerMode           eMode;
    tOAD_TerGuard          eGuard;
    tOAD_TerHierarchy      eHierarchy;
    tOAD_TerPrioStream     ePriority;
    tOAD_Convolution       eConvolutionHP;
    tOAD_Convolution       eConvolutionLP;
    tOAD_TerFreqOff        eOffset;

#endif

    uint32_t                    uDlState;
    uint32_t                    eDownloaderMethod;
    uint32_t                    uPid;
    uint32_t                    eErrorCode;
    uint32_t                    uLastUpdate;
    uint32_t                    uCurrentVersion;
    uint32_t                    uFactoryOADFlag;
    uint32_t                    uUpdateType;    
    uint32_t                    uSubgroupAssociationValue;
    uint32_t                    uSubgroupAssociationOui;
    uint32_t                    uReserved1;
	uint32_t					uDownloaderVersion;
	uint32_t					uExchangeAreaVersion;
	uint32_t					uStatus_U;
    uint32_t                           eCarouselCheckFlags;
#ifndef __USE_CONFIG_FILE__    
    uint32_t    				uOUI;
    uint32_t    				uHW_Version; 
    uint32_t    				uHW_Model;
    uint32_t    				uSW_Version;
    uint32_t    				uSW_Model;	
#endif    
    tNVM_UartParameter      sUartParameter;
    tNVM_Custom                sCustom;
    unsigned char               uCRC16[4]; /* only 2 first byte are used, but as all field of this structure, uCRC16 have a 4 bytes size */
} tNVM_Dialog_Area;


typedef enum 
{
   eDLD_CFG_NO_DOWNLOADER                       = 0x0000, /* the only goal of downloader is to launch the application */
   eDLD_CFG_USB_FILE_DOWNLOADER                 = 0x0001, /* To search carousel on USB file */
   eDLD_CFG_UART_SERIAL_DOWNLOADER              = 0x0002, /* To search Iwedia carousel on UART slot */
   eDLD_CFG_BROADCAST_DOWNLOADER                = 0x0004, /* To search dsmcc carousel over the air   */
   eDLD_CFG_CHECK_FLASH_INTEGRITY               = 0x0008, /* To check application integrity each time */
    /* To search a SSU DSMCC data carousel over the air */
   eDLD_CFG_SSU_DATA_CAROUSEL                   = 0x0010,         
   /* To search a DTT DSMCC object carousel over the air (exclusive to eDLD_CFG_SSU_DATA_CAROUSEL) */
   eDLD_CFG_DTT_OBJECT_CAROUSEL                 = 0x0020,      
    /* What to do when not any way to get a valid application image */
   eDLD_CFG_RESCUE_RETRY_DOWNLOADER             = 0x0040, /* To retry downloader until a valid software image had been sucessfully burned, not yet implemented */
   eDLD_CFG_RESCUE_TRY_TO_LAUNCH_APPLICATION    = 0x0080,
	/* What to do when bad integrity */
	eDLD_CFG_BAD_INTEGRITY_USB_FILE_DOWNLOADER   = 0x0100,
	eDLD_CFG_BAD_INTEGRITY_BROADCAST_DOWNLOADER  = 0x0200,
	/* forced download : most of the time when a fp key is hold down during boot*/
	eDLD_CFG_FORCED_DOWNLOAD                     = 0x0400,
	/* What to do when forced download is activated */
	eDLD_CFG_FORCED_DOWNLOAD_USB_FILE_DOWNLOADER = 0x0800,
	eDLD_CFG_FORCED_DOWNLOAD_BROADCAST_DOWNLOADER= 0x1000
} tDLD_CFG_DownloaderMethod;

/**
 * Sample:
 * eDLD_CFG_UART_SERIAL_THEN_SSU_DATA_CAROUSEL                  = eDLD_CFG_UART_SERIAL_DOWNLOADER+
 *                                                                eDLD_CFG_SSU_DATA_CAROUSEL
 * eDLD_CFG_UART_SERIAL_THEN_INTEGRITY_THEN_SSU_DATA_CAROUSEL   = eDLD_CFG_UART_SERIAL_DOWNLOADER +
 *                                                                eDLD_CFG_CHECK_FLASH_INTEGRITY  +
 *                                                                eDLD_CFG_SSU_DATA_CAROUSEL
 * eDLD_CFG_INTEGRITY_THEN_SSU_DATA_CAROUSEL                    = eDLD_CFG_CHECK_FLASH_INTEGRITY + 
 *                                                                eDLD_CFG_SSU_DATA_CAROUSEL
 *
 * eDLD_CFG_UART_SERIAL_THEN_DTT_OBJECT_CAROUSEL                = eDLD_CFG_UART_SERIAL_DOWNLOADER + 
 *                                                                eDLD_CFG_DTT_OBJECT_CAROUSEL,
 * eDLD_CFG_UART_SERIAL_THEN_INTEGRITY_THEN_DTT_OBJECT_CAROUSEL = eDLD_CFG_UART_SERIAL_DOWNLOADER + 
 *                                                                eDLD_CFG_CHECK_FLASH_INTEGRITY + 
 *                                                                eDLD_CFG_DTT_OBJECT_CAROUSEL
 * eDLD_CFG_INTEGRITY_THEN_DTT_OBJECT_CAROUSEL                  = eDLD_CFG_CHECK_FLASH_INTEGRITY + 
 *                                                                eDLD_CFG_DTT_OBJECT_CAROUSEL
 *
 */

    
/* tDLD_CFG_CarouselCheckFlag is a flag type : any binary_or combination are available */
typedef enum { 
    eDLD_CFG_CHECK_NOTHING=0,     
    eDLD_CFG_CHECK_OUI=1, 
    eDLD_CFG_CHECK_HARDWARE=2, 
    eDLD_CFG_CHECK_PRODUCT=4, 
    eDLD_CFG_CHECK_SIGNATURE=8 ,
    eDLD_CFG_CHECK_USAGE_ID=16,
    eDLD_CFG_CHECK_ALL=(eDLD_CFG_CHECK_OUI|eDLD_CFG_CHECK_HARDWARE|eDLD_CFG_CHECK_PRODUCT|eDLD_CFG_CHECK_SIGNATURE|eDLD_CFG_CHECK_USAGE_ID)   
} tDLD_CFG_CarouselCheckFlag;

/*------------------------------*/
/*      Default definition      */
/*------------------------------*/

/*******************************************************/
/*           Debug Functions Declarations              */
/*******************************************************/

#if defined (__DEBUG_TRACE__)
#define __NVM_EX_DEBUG__  1

void  NVM_EX_DEBUG_Dump                 (tNVM_Dialog_Area *pNDA, char *pLabel);
char *NVM_EX_DEBUG_LabelDownloadState   (uint32_t eDownloadState);
char *NVM_EX_DEBUG_LabelDownloadErrCode (uint32_t eDownloadErrCode);
char *NVM_EX_DEBUG_LabelPolarization    (tOAD_LNB_Polarization ePolarization);
char *NVM_EX_DEBUG_LabelModulation      (tOAD_Modulation eModul);
char *NVM_EX_DEBUG_LabelConvolution     (tOAD_Convolution eConvol);
#if defined (MEDIUM_TER)
char *NVM_EX_DEBUG_LabelTerMode         (tOAD_TerMode eTerMode);
char *NVM_EX_DEBUG_LabelTerGuard        (tOAD_TerGuard eTerGuard);
char *NVM_EX_DEBUG_LabelTerHierarchy    (tOAD_TerHierarchy eTerHierarchy);
char *NVM_EX_DEBUG_LabelTerPrioStream   (tOAD_TerPrioStream eTerPrioStream);
char *NVM_EX_DEBUG_LabelTerFreqOff      (tOAD_TerFreqOff eTerFreqOff);
#endif
#if defined (MEDIUM_SAT)
char *NVM_EX_DEBUG_LabelOduTrack        (tOAD_ODU_TRACK eOduTrack);
char *NVM_EX_DEBUG_LabelOduTone         (tOAD_ODU_ToneState eOduTone);
char *NVM_EX_DEBUG_LabelOduPower        (tOAD_ODU_Power eOduPower);
#endif
#endif
#endif /* __REMOVE_NVMEX__ */

#endif

