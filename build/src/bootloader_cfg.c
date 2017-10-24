/********************************************************************************
 *                       COPYRIGHT 2006 IWEDIA                                  *
 ********************************************************************************
 *
 * MODULE NAME:
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/src/bootloader_cfg.c $
 *            $Rev: 332 $
 *            $Date: 2011-09-07 13:51:23 +0200 (Wed, 07 Sep 2011) $
 *
 *
 *******************************************************************************/

/********************************************************/
/*                Includes                              */
/********************************************************/
#include "crules.h"
#include "bootloader.h"
#include "bootloader_cfg.h"
#include "tdal_uart.h"
#ifdef PRODUCT_USE_FP
#include "tdal_fp.h"
#endif

extern int sprintf ( char * str, const char * format, ... );

/********************************************************/
/*              Defines                                 */
/********************************************************/
/*    the following values allow to fill the NVM_exchange structure: (BTL_CFG_NVM_EX_WriteDefaultParam) */
/* it is used by bootloader at first power-on                                                                                               */
/* it is used in case of NVM_exchange structure corruption too                                                                  */
/* corruption can be detected either by the bootloader or by the trigger                                                        */
/* this values will be updated as soon as an explicit download is required                                                   */
/* All this values shall be CUSTOMIZED                                                                                                  */

 /* INITIAL_LSB_UPDATE_ID :
    0x0000 means flash delivered with bootloader.bin and without comedia.bin
    0x0001 means flash delivered with an initial comedia.bin */
    #define INITIAL_LSB_UPDATE_ID   0x0001
    #define DEFAULT_UPDATE_ID       ((SYS_INFO_GetVersionIdMsb()<<16)+ (INITIAL_LSB_UPDATE_ID&0xFFFF))

/* DEFAULT_DL_STATE  : what to do at first bootloader launch */
/* - NVM_DLSTART : immediate download try, required if no software is in flash (INITIAL_LSB_UPDATE_ID==0)       */
/* - NVM_NODL : that is the standard, nothing to do, (needs INITIAL_LSB_UPDATE_ID >0)                                   */
/* Other enum values are not allowed as default                                                                                                 */
    #define DEFAULT_DL_STATE       ((INITIAL_LSB_UPDATE_ID==0)?NVM_DLSTART:NVM_NODL)

/*     DEFAULT_PID is the data PID where is usually broadcast the upgrade carousel */
    #define DEFAULT_PID             	0x001224

/* broadcast parameter: according the medium you used, please customized the appropriate set of values  */
/* Please look at nvm_exchange_mapping_cfg.h to retrieve the enum symbols                                           */
#if defined (MEDIUM_SAT)
    #define DEFAULT_FREQUENCY       10820000 /* in kHz */
    #define DEFAULT_SYMBOLE_RATE    27500000 /* in bauds */
    #define DEFAULT_POLARIZATION    eOAD_POLAR_VERTICAL
    #define DEFAULT_MODULATION      eOAD_MOD_QPSK
    #define DEFAULT_FEC_RATE        eOAD_FEC_AUTO
    #define DEFAULT_LOW_FREQ        9750
    #define DEFAULT_HIGH_FREQ       10600
    #define DEFAULT_ODU_TRACK       eOAD_ODU_TRACK_NONE
    #define DEFAULT_ODU_TONE_STATE  eOAD_ODU_TONE_NONE
    #define DEFAULT_ODU_POWER       eOAD_ODU_POWER_NONE
    #define DEFAULT_SPEC_INV        eOAD_SPECTRUM_AUTO

#elif defined (MEDIUM_CAB)
    #define DEFAULT_FREQUENCY       100000000 /* in Hz */
    #define DEFAULT_SYMBOLE_RATE    6875000   /* in bauds */
    #define DEFAULT_MODULATION      eOAD_MOD_QAM64
    #define DEFAULT_SPEC_INV        eOAD_SPECTRUM_AUTO

#elif defined (MEDIUM_TER)
    #define DEFAULT_FREQUENCY       498000000 /* in Hz  */
    #define DEFAULT_BANDWIDTH       8000000   /* in Hz */
    #define DEFAULT_MODULATION      eOAD_MOD_QAM64
    #define DEFAULT_MODE            eOAD_MODE_AUTO
    #define DEFAULT_GUARD           eOAD_GUARD_AUTO
    #define DEFAULT_HIERCHARCHY     eOAD_HIER_AUTO
    #define DEFAULT_PRIORITY        eOAD_PRIORITY_STREAM_HIGH
    #define DEFAULT_CONV_HP         eOAD_FEC_AUTO
    #define DEFAULT_CONV_LP         eOAD_FEC_AUTO
    #define DEFAULT_OFFSET          eOAD_OFFSET_AUTO
    #define DEFAULT_SPEC_INV        eOAD_SPECTRUM_NORMAL

#endif

/*
**  Defining DEFAULT_CAROUSEL_CHECK_FLAGS
*/
#if defined(TRIG_DTG) | defined(TRIG_DVBSSU)
   #define DEFAULT_CAROUSEL_CHECK_FLAGS (eBTL_CFG_CHECK_OUI + eBTL_CFG_CHECK_SIGNATURE)
   /* The hardware and product compatibility descriptors will be set  by the trigger */
#elif  defined(TRIG_OFF)
    /* without trigger, only serial download is available */
    #define DEFAULT_CAROUSEL_CHECK_FLAGS (eBTL_CFG_CHECK_SIGNATURE)
 #elif  defined(TRIG_IDT)
   #warning Bootloader is not designed for IREDETO requirements
   #define DEFAULT_CAROUSEL_CHECK_FLAGS (eBTL_CFG_CHECK_NOTHING)
#endif

/*
**  Defining DEFAULT_UART_INSTANCE
*/

#ifdef BTL_SERIAL
#define DEFAULT_UART_INSTANCE eTDAL_UART_3
#else
#define DEFAULT_UART_INSTANCE 0
#endif

/*
**  defining DEFAULT_BOOTLOADER_METHOD
*/
/* USB FILE METHOD */
#ifdef BTL_USB_FILE
    #define kBTL_CFG_USB_XMOUNT_POINT        ((const uint8_t*)"/usb%d")
    #define kBTL_CFG_USB_DOWNLOAD_FULL_FILE_NAME ("/usb0/"kBTL_CFG_USB_DOWNLOAD_FILE_NAME)

    #ifdef kBTL_CFG_USB_DOWNLOAD_ENABLE_AUTOMATIC_MODE
        #define DEFAULT_FILE_METHOD           eBTL_CFG_USB_FILE_BOOTLOADER
    #else
        #define DEFAULT_FILE_METHOD           eBTL_CFG_NO_BOOTLOADER
    #endif
#else
    #define DEFAULT_FILE_METHOD           eBTL_CFG_NO_BOOTLOADER
#endif

/* SERIAL METHOD */
#ifdef BTL_SERIAL
    #define DEFAULT_SERIAL_METHOD           eBTL_CFG_UART_SERIAL_BOOTLOADER
#else
    #define DEFAULT_SERIAL_METHOD           eBTL_CFG_NO_BOOTLOADER
#endif

/* BROADCAST METHOD */
#define DEFAULT_BROADCAST_METHOD           eBTL_CFG_NO_BOOTLOADER

/* INTEGRITY CHECK METHOD */
#ifdef kSW_INTEGRITY_CHECK
    #define DEFAULT_INTEGRITY_METHOD    eBTL_CFG_CHECK_FLASH_INTEGRITY
#else
    #define DEFAULT_INTEGRITY_METHOD    eBTL_CFG_NO_BOOTLOADER
#endif
/* CAROUSEL METHOD */
#if defined(TRIG_DTG)
    #define DEFAULT_CAROUSEL_METHOD     eBTL_CFG_DTT_OBJECT_CAROUSEL
#elif  defined(TRIG_DVBSSU)
    #define DEFAULT_CAROUSEL_METHOD     eBTL_CFG_SSU_DATA_CAROUSEL
#else
    #define DEFAULT_CAROUSEL_METHOD     eBTL_CFG_NO_BOOTLOADER
    #ifdef BTL_USB_FILE
        #error BTL_USB_FILE but do not know DEFAULT_CAROUSEL_METHOD
    #endif
#endif

#ifdef BTL_USB_FILE
#define DEFAULT_BAD_INTEGRITY_METHOD    eBTL_CFG_BAD_INTEGRITY_USB_FILE_BOOTLOADER        
#else
#define DEFAULT_BAD_INTEGRITY_METHOD    eBTL_CFG_BAD_INTEGRITY_BROADCAST_BOOTLOADER  /* NOT IMPLEMENTED ! */
#endif

#define DEFAULT_RESCUE_METHOD             eBTL_CFG_RESCUE_TRY_TO_LAUNCH_APPLICATION


#ifdef BOOTLOADER_FORCED_MODE_ENABLED
#define DEFAULT_FORCED_MODE_METHOD    eBTL_CFG_FORCED_DOWNLOAD

   #ifdef BTL_USB_FILE
   #define DEFAULT_FORCED_METHOD    eBTL_CFG_FORCED_DOWNLOAD_USB_FILE_BOOTLOADER        
   #else
   #define DEFAULT_FORCED_METHOD    eBTL_CFG_FORCED_DOWNLOAD_BROADCAST_BOOTLOADER  /* NOT IMPLEMENTED ! */
   #endif
#else
#define DEFAULT_FORCED_MODE_METHOD    eBTL_CFG_NO_BOOTLOADER
#define DEFAULT_FORCED_METHOD    eBTL_CFG_NO_BOOTLOADER
#endif


/* MERGED DEFAULT_BOOTLOADER_METHOD METHOD */
#define DEFAULT_BOOTLOADER_METHOD (\
    DEFAULT_FILE_METHOD+\
    DEFAULT_SERIAL_METHOD+\
    DEFAULT_BROADCAST_METHOD+\
    DEFAULT_INTEGRITY_METHOD+\
    DEFAULT_CAROUSEL_METHOD+\
	DEFAULT_BAD_INTEGRITY_METHOD+\
    DEFAULT_RESCUE_METHOD+\
	DEFAULT_FORCED_MODE_METHOD+\
	DEFAULT_FORCED_METHOD)


/********************************************************/
/*              Local Functions Declarations (LOCAL)    */
/*******************************************************/

/********************************************************/
/*              Functions Definitions (LOCAL/GLOBAL)    */
/********************************************************/

/******************************************************************************
 * Function Name : BTL_CFG_GetNvmexDefaultParam
 *
 * Description   : Fill-in NVM exchange Dialog area with default values
 *
 * Side effects  :
 *
 * Comment       : Please DO NOT customise this function
 *
 *****************************************************************************/
tNVM_Error BTL_CFG_GetNvmexDefaultParam(tNVM_Dialog_Area    *pDialogArea)
{

    if (pDialogArea==NULL)
    {
        /* Bad call to this function */
        return eNVM_ERROR;
    }

    /*---------------------------------------------------------*/
    /* tuning parameters default values         */
    /*---------------------------------------------------------*/
    pDialogArea->uUpdateID              = DEFAULT_UPDATE_ID;

#if defined (MEDIUM_SAT)
    pDialogArea->uFrequency             = DEFAULT_FREQUENCY;
    pDialogArea->uSymbol_rate           = DEFAULT_SYMBOLE_RATE;
    pDialogArea->ePolarization          = DEFAULT_POLARIZATION;
    pDialogArea->eModulation            = DEFAULT_MODULATION;
    pDialogArea->eFec_rate              = DEFAULT_FEC_RATE;
    pDialogArea->uLow_freq              = DEFAULT_LOW_FREQ;
    pDialogArea->uHigh_freq             = DEFAULT_HIGH_FREQ;
    pDialogArea->OduInfo.TrackIndex     = DEFAULT_ODU_TRACK;
    pDialogArea->OduInfo.ToneState      = DEFAULT_ODU_TONE_STATE;
    pDialogArea->OduInfo.Power          = DEFAULT_ODU_POWER;
#elif defined (MEDIUM_CAB)
    pDialogArea->uFrequency             = DEFAULT_FREQUENCY;
    pDialogArea->uSymbol_rate           = DEFAULT_SYMBOLE_RATE;
    pDialogArea->eModulation            = DEFAULT_MODULATION;
#elif defined (MEDIUM_TER)
    pDialogArea->uFrequency             = DEFAULT_FREQUENCY;
    pDialogArea->uBandwidth             = DEFAULT_BANDWIDTH;
    pDialogArea->eModulation            = DEFAULT_MODULATION;
    pDialogArea->eMode                  = DEFAULT_MODE;
    pDialogArea->eGuard                 = DEFAULT_GUARD;
    pDialogArea->eHierarchy             = DEFAULT_HIERCHARCHY;
    pDialogArea->ePriority              = DEFAULT_PRIORITY;
    pDialogArea->eConvolutionHP         = DEFAULT_CONV_HP;
    pDialogArea->eConvolutionLP         = DEFAULT_CONV_LP;
    pDialogArea->eOffset                = DEFAULT_OFFSET;
#endif

    pDialogArea->uDlState               = DEFAULT_DL_STATE;
    pDialogArea->eBootloaderMethod = DEFAULT_BOOTLOADER_METHOD;
    pDialogArea->uPid                   = DEFAULT_PID;
    pDialogArea->eErrorCode             = NVM_ERR_NO_ERROR;
    pDialogArea->uLastUpdate            = DEFAULT_UPDATE_ID;
    pDialogArea->uCurrentVersion        = DEFAULT_UPDATE_ID;
    pDialogArea->uFactoryOADFlag        = 0;
    pDialogArea->uUpdateType            = 0;
    pDialogArea->uSubgroupAssociationValue = 0;
    pDialogArea->uSubgroupAssociationOui   = SYS_INFO_GetOui();
    pDialogArea->uReserved1             = 0;

	pDialogArea->uBootloaderVersion     	= SYS_INFO_GetBootloaderVersion();
	pDialogArea->uExchangeAreaVersion   	= SYS_INFO_NvmExVersion();
	pDialogArea->uStatus_U			   = 1; /* By default, the binary is loaded by other way that bootloader */
	pDialogArea->eCarouselCheckFlags    = DEFAULT_CAROUSEL_CHECK_FLAGS;
	pDialogArea->uOUI				   	= SYS_INFO_GetOui();
	pDialogArea->uHW_Version			= SYS_INFO_GetHardwareVersionId();
	pDialogArea->uHW_Model			= SYS_INFO_GetHardwareModelId();
	pDialogArea->uSW_Version			= SYS_INFO_GetProductVersionId();
	pDialogArea->uSW_Model			= SYS_INFO_GetProductModelId();
	
    pDialogArea->sUartParameter.uUartInstance = (uint16_t) DEFAULT_UART_INSTANCE;
    pDialogArea->sUartParameter.uUartRate = (uint16_t) eTDAL_UART_BAUD_RATE_115200;
    pDialogArea->sUartParameter.uUartDataBit = (uint8_t) eTDAL_UART_DATA_BITS_8;
    pDialogArea->sUartParameter.uUartStopBit = (uint8_t) eTDAL_UART_STOP_BITS_1;
    pDialogArea->sUartParameter.uUartParity = (uint8_t) eTDAL_UART_PARITY_NONE;
    pDialogArea->sUartParameter.uUartFlowControl = (uint8_t) eTDAL_UART_FLOW_CTRL_NONE;
	
    pDialogArea->sCustom.uDummy         = 0;

   return eNVM_NO_ERROR;

} /* end void BTL_CFG_GetNvmexDefaultParam(void) */
/******************************************************************************
 * Function Name : BTL_CFG_GetUsbDownloadFileName
 *
 * Description   : return the file name used for usb download
 *
 * Side effects  :
 *
 * Comment       : complete path including /mount_point/
 *
 *****************************************************************************/
const uint8_t *BTL_CFG_GetUsbDownloadFileName(void)
{
#ifdef kBTL_CFG_USB_DOWNLOAD_FULL_FILE_NAME
    return (const uint8_t *)kBTL_CFG_USB_DOWNLOAD_FULL_FILE_NAME;
#else
    return NULL;
#endif
}

/******************************************************************************
 * Function Name : BTL_CFG_GetUsbMountPoint
 *
 * Description   : return the mount point used for usb volume
 *
 *  parameter : uIndx the index of the volume (usualy 0)
 *
 * Side effects  :
 *
 * Comment       :
 *
 *****************************************************************************/
const uint8_t *BTL_CFG_GetUsbMountPoint(uint8_t uIndx)
{
#ifdef kBTL_CFG_USB_XMOUNT_POINT
    static uint8_t uszMountPoint[sizeof(kBTL_CFG_USB_XMOUNT_POINT) + 5];
    sprintf((char*)&uszMountPoint[0],(const char*)kBTL_CFG_USB_XMOUNT_POINT, uIndx);
    return &uszMountPoint[0];
#else
    return NULL;
#endif
}

/******************************************************************************
 * Function Name : BTL_CFG_getUsbWaitMs
 *
 * Description   : return the laps needed to detect a usb device is inserted
 *
 * Side effects  :
 *
 * Comment       : hardware dependent
 *
 *****************************************************************************/
extern const uint32_t BTL_CFG_getUsbWaitMs(void)
{
    #ifdef kBTL_CFG_USB_DOWNLOAD_ENABLE_AUTOMATIC_MODE
        return 3000;
    #else
        return 30000;
    #endif
}

/******************************************************************************
 * Function Name : BTL_CFG_getSerialWaitMs
 *
 * Description   : return the laps PC will respond through serial link
 *
 * Side effects  :
 *
 * Comment       : hardware dependent
 *
 *****************************************************************************/
extern const uint32_t BTL_CFG_getSerialWaitMs(void)
{
    return 300; /* 300 mili seconds */
}

/******************************************************************************
 * Function Name : BTL_CFG_GetPartitionKind
 *
 * Description   : return TRUE when product model and version should match descriptor
 *
 * Side effects  :
 *
 * Comment       : Please DO NOT customise this function
 *
 *****************************************************************************/
tBTL_PartitionKind BTL_CFG_GetPartitionKind(tFLASH_CFG_FLASH_Area eArea)
{
	tBTL_PartitionKind ePartitionKind=eBTL_UNKNOWN_PARTITION;
	switch(eArea)
	{
		case eFLASH_CFG_FLASH_AREA_BOOTSTRAP:
			ePartitionKind=eBTL_CONSTANT_PARTITION;
			break;
		case eFLASH_CFG_FLASH_AREA_BOOTLOADER:
			ePartitionKind=eBTL_CONSTANT_PARTITION;
			break;
		case eFLASH_CFG_FLASH_AREA_COMEDIA:
			ePartitionKind=eBTL_UPGRADABLE_PARTITION;
			break;
		case eFLASH_CFG_FLASH_AREA_PARTITION_A:
			ePartitionKind=eBTL_UPGRADABLE_PARTITION;
			break;
		case eFLASH_CFG_FLASH_AREA_PARTITION_B:
			ePartitionKind=eBTL_DYNAMIC_PARTITION;
			break;
		case eFLASH_CFG_FLASH_AREA_PARTITION_B_CTRLBLOCK:
			ePartitionKind=eBTL_DYNAMIC_PARTITION;
			break;
		default:
			ePartitionKind=eBTL_UNKNOWN_PARTITION;
			break;				
						
	} /* 	switch(eArea) */
	return ePartitionKind;
}


/******************************************************************************
 * Function Name : BTL_CFG_NotifyProgress
 *
 * Description   : This is a notification custom it as you want
 *
 * Side effects  : - none -
 *
 * Parameter	
 *			- IN - tBTL_CFG_ProgressStep eStep : The step reached
				eBTL_CFG_INIT_STEP=0,
			 	eBTL_CFG_UPTODATE,
				eBTL_CFG_SERIAL_DETECTED,
				eBTL_CFG_CORRUPTION_DETECTED,
				eBTL_CFG_SERIAL_CONNECTED,
				eBTL_CFG_BROADCAST_CONNECTED,
				eBTL_CFG_TUNER_LOCKED,
				eBTL_CFG_DATA_CAROUSEL_IDDENTIFIED,
				eBTL_CFG_OBJECT_CAROUSEL_IDDENTIFIED,
				eBTL_CFG_CAROUSEL_FRAME_RECEIVED,
				eBTL_CFG_CAROUSEL_LOOP,
				eBTL_CFG_SIGNATURE_GRANTED,
				eBTL_CFG_FLASH_BLOCK_ERASED,
				eBTL_CFG_FLASH_BLOCK_WRITTEN,
				eBTL_CFG_INTEGRITY_CRC_WRITTEN,
				eBTL_CFG_DOWNLOAD_COMPLETED,
				eBTL_CFG_DOWNLOAD_FAILURE=0xFF
 *			- IN - uint32_t uCount	: counter inside the step
 *			- IN - uint32_t uTotal	: max value for counter inside the step
 *
 * Comment / advise :
 *		To get a good graphic rendering progress
 *		you can use 3 bargraphs :
 *		- first one for steps eBTL_CFG_CAROUSEL_FRAME_RECEIVED
 *		- second one for steps eBTL_CFG_FLASH_BLOCK_ERASED
 *		- last one for steps eBTL_CFG_FLASH_BLOCK_WRITTEN
 *		other notification are mainly single event you can display or not as you want
 *		Of course you can merge the 3 main progress step in a single bargraph
 *
 *****************************************************************************/
/* @@@@@@@@@@@@
#define _BOOTLOADER_CFG_DEBUG_*/
/* @@@@@@@@@@@@ */
#ifdef _BOOTLOADER_CFG_DEBUG_
extern int sttbx_Print(const char *, ...);
#define PROGRESS_PRINTF(x) sttbx_Print x;
#else
#define PROGRESS_PRINTF(x)
#endif


#ifdef PRODUCT_USE_FP
#define PROGRESS_SetInfo(step, string)	TDAL_FP_SetInfo(step, string)
#define PROGRESS_SetInfoInit()			TDAL_FP_Init()
#else /* PRODUCT_USE_FP */
#define PROGRESS_SetInfo(step, string)
#define PROGRESS_SetInfoInit()			
#endif /* PRODUCT_USE_FP */

extern tNVM_Dialog_Area gsBTL_DialogArea;
extern tNVM_Error NVM_EX_Write(struct _OPAQUE_DIALOG_AREA_STRUCTURE_ *psDialogArea);
extern tNVM_Error NVM_EX_CRC_crc16Calculate(struct _OPAQUE_DIALOG_AREA_STRUCTURE_ *psDialogArea);
extern void FDMX_TST_main(void);

#ifdef BOOTLOADER_FORCED_MODE_ENABLED
#ifdef PRODUCT_USE_FP
void BTL_CFGi_FPCallbackFct( tTDAL_FP_KeyCode   KeyCode,
									tTDAL_FP_KeyStatus KeyStatus );
#endif
bool bBTL_CFG_specialFPkey_HoldDown=false;
#endif

void BTL_CFG_NotifyProgress(tBTL_CFG_ProgressStep eStep, uint32_t uCount, uint32_t uTotal)
{
	switch (eStep)
	{
		case eBTL_CFG_INIT_STEP:
		{
    		PROGRESS_SetInfoInit();
#ifdef PRODUCT_USE_FP

    #ifdef BOOTLOADER_FORCED_MODE_ENABLED
			TDAL_FP_Register(BTL_CFGi_FPCallbackFct)
    #endif

    		PROGRESS_SetInfo(eTDAL_FP_INFO_ON, NULL);
#endif
		}
		break;
	 	case eBTL_CFG_UPTODATE:
		{
		}
		break;
		case eBTL_CFG_SERIAL_DETECTED:
		{
		}
		break;
		case eBTL_CFG_FILE_DETECTED:
		{
			PROGRESS_PRINTF(("BTL_CFG_NotifyProgress eBTL_CFG_FILE_DETECTED \n"));
		}
		break;
		case eBTL_CFG_CORRUPTION_DETECTED:
		{
		}
		break;
		case eBTL_CFG_SERIAL_CONNECTED:
		{
#ifdef PRODUCT_USE_FP
            PROGRESS_SetInfo(eTDAL_FP_INFO_DOWNLOAD, NULL);
#endif
		}
		break;
		case eBTL_CFG_BROADCAST_CONNECTED:
		{
#ifdef PRODUCT_USE_FP
            PROGRESS_SetInfo(eTDAL_FP_INFO_DOWNLOAD, NULL);
#endif
		}
		break;
		case eBTL_CFG_TUNER_LOCKED:
		{
			PROGRESS_PRINTF(("BTL_CFG_NotifyProgress eBTL_CFG_TUNER_LOCKED \n"));
#ifdef PRODUCT_USE_FP
            PROGRESS_SetInfo(eTDAL_FP_INFO_FE_LOCK, NULL);
#endif
		}
		break;
		case eBTL_CFG_SERIAL_CAROUSEL_IDDENTIFIED:
		{
		}
		break;
		case eBTL_CFG_DATA_CAROUSEL_IDDENTIFIED:
		{
		}
		break;
		case eBTL_CFG_OBJECT_CAROUSEL_IDDENTIFIED:
		{
		}
		break;
		case eBTL_CFG_CAROUSEL_FRAME_RECEIVED:
		{
#ifdef PRODUCT_USE_FP
			PROGRESS_SetInfo(eTDAL_FP_INFO_RCVDATA, NULL);
#endif
			/* do not print any thing */
			return;
		}
		break;
		case eBTL_CFG_CAROUSEL_LOOP:
		{
			PROGRESS_PRINTF(("BTL_CFG_NotifyProgress eBTL_CFG_CAROUSEL_LOOP loop done %d loop watch dog %d\n",uCount,uTotal));
		}
		break;
		case eBTL_CFG_CAROUSEL_COMPLETED:
		{
			PROGRESS_PRINTF(("BTL_CFG_NotifyProgress eBTL_CFG_CAROUSEL_COMPLETED \n"));
		}
		case eBTL_CFG_SIGNATURE_GRANTED:
		{
		}
		break;
		case eBTL_CFG_FLASH_BLOCK_ERASED:
		{
#ifdef PRODUCT_USE_FP
            PROGRESS_SetInfo(eTDAL_FP_INFO_FLASH_ERASE, NULL);
#endif
			PROGRESS_PRINTF(("BTL_CFG_NotifyProgress eBTL_CFG_FLASH_BLOCK_ERASED erased %d / %d\n",uCount,uTotal));
		}
		break;
		case eBTL_CFG_FLASH_BLOCK_WRITTEN:
		{
#ifdef PRODUCT_USE_FP
            PROGRESS_SetInfo(eTDAL_FP_INFO_FLASH_RW, NULL);
#endif
			PROGRESS_PRINTF(("BTL_CFG_NotifyProgress eBTL_CFG_FLASH_BLOCK_WRITTEN written %d / %d\n",uCount,uTotal));
		}
		break;
		case eBTL_CFG_INTEGRITY_CRC_WRITTEN:
		{
		}
		break;
		case eBTL_CFG_DOWNLOAD_COMPLETED:
		{
#ifdef PRODUCT_USE_FP
            PROGRESS_SetInfo(eTDAL_FP_INFO_DOWNLOAD, kTDAL_FP_OK_STRING);
#endif
			PROGRESS_PRINTF(("BTL_CFG_NotifyProgress eBTL_CFG_DOWNLOAD_COMPLETED\n"));
		}
		break;
		case eBTL_CFG_DOWNLOAD_FAILURE:
		{
#ifdef PRODUCT_USE_FP
            PROGRESS_SetInfo(eTDAL_FP_INFO_DOWNLOAD, kTDAL_FP_NOK_STRING);
#endif
			PROGRESS_PRINTF(("BTL_CFG_NotifyProgress eBTL_CFG_DOWNLOAD_FAILURE error 0x%08x \n",uCount));
		}
		case eBTL_CFG_USB_DEVICE_NEEDED:
		{
			if ( uCount == 1 )
			{
				PROGRESS_PRINTF(("BTL_CFG_NotifyProgress eBTL_CFG_USB_DEVICE_NEEDED : ON\n"));
#ifdef PRODUCT_USE_FP
	            PROGRESS_SetInfo(eTDAL_FP_INFO_ERROR, kTDAL_FP_OK_STRING);
#endif
			}
			else
			{
				PROGRESS_PRINTF(("BTL_CFG_NotifyProgress eBTL_CFG_USB_DEVICE_NEEDED : OFF\n"));
#ifdef PRODUCT_USE_FP
	            PROGRESS_SetInfo(eTDAL_FP_INFO_ERROR, kTDAL_FP_NOK_STRING);
#endif
			}
		}
		break;
		break;
	} /* end switch (eStep) */
}



/******************************************************************************
 * Function Name : BTL_CFG_IsForcedDownloadActivated
 *
 * Description   : return TRUE when the forced download has to be launched :
 *                 Most of the time, when a fp key is hold down. see BTL_CFGi_FPCallbackFct.
 *
 * Side effects  :
 *
 * Comment       : 
 *
 *****************************************************************************/
bool BTL_CFG_IsForcedDownloadActivated(void)
{
    bool retCode=false;

#ifdef BOOTLOADER_FORCED_MODE_ENABLED
	retCode = bBTL_CFG_specialFPkey_HoldDown;
#endif
	
	return retCode;
}

#ifdef BOOTLOADER_FORCED_MODE_ENABLED
#ifdef PRODUCT_USE_FP
/******************************************************************************
 * Function Name : BTL_CFGi_FPCallbackFct
 *
 * Description   : 
 *
 * Side effects  :
 *
 * Comment       : 
 *
 *****************************************************************************/
void BTL_CFGi_FPCallbackFct( tTDAL_FP_KeyCode   KeyCode,
									tTDAL_FP_KeyStatus KeyStatus )
{
    tSYS_KeyEvtCode sysCode = 0;

    sysCode = SYS_FP_ConvertPhysicalCodeToSYSCode(KeyCode);
    if (sysCode == eSYS_DOWN_ARROW_KEY)
    {
		switch(KeyStatus)
		{
			case eTDAL_FP_KEY_PRESSED:
			case eTDAL_FP_KEY_RELEASED:
			{
				bBTL_CFG_specialFPkey_HoldDown = true;
				break;
			}
			case eTDAL_FP_KEY_HELD_DOWN:
			default:
			{
				bBTL_CFG_specialFPkey_HoldDown = false;
				break;
			}
		}
	}
	return;
}
#endif
#endif

