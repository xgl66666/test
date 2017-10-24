/******************************************************************************
 * COPYRIGHT 2004 IWEDIA TECHNOLOGIES *
 ******************************************************************************
 *
 * MODULE NAME: TRIGGER
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/idecode/product/comedia/branches/snt_cos/external/packages/trigger/inc/trigger.h $
 * $Rev: 10129 $
 * $Date: 2012-05-02 14:57:25 +0200 (mer., 02 mai 2012) $
 *
 * PUBLIC
 *
 * DESCRIPTION: trigger public header
 *
 *****************************************************************************/

#ifndef _TRIGGER_H_
#define _TRIGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/* Includes */
/*******************************************************/

#include "nvm_exchange.h"
#include "nvm_exchange_mapping_cfg.h"
#include "ssu_enhanced_types.h"

/*******************************************************/
/* Defines */
/*******************************************************/
#define	TRIG_CLIENTID_NOT_USED		0
#define	TRIG_CLIENTID_USED			1

/*******************************************************/
/* Traces */
/*******************************************************/
typedef enum
{
    TRIGGER = 0x04280001
}tTRIG_TracesID;

/*******************************************************/
/* Macros */
/*******************************************************/

/*******************************************************/
/* Typedefs */
/*******************************************************/
typedef enum
{
    eTRIG_NO_ERROR = 0,
    eTRIG_ERROR,
    eTRIG_NOT_FOUND,
    eTRIG_UP_TO_DATE
} tTRIG_Error;

typedef enum
{
    eTRIG_DVBSSU_SIMPLE = 0,
    eTRIG_DVBSSU_ENHANCED,
    eTRIG_DTG,
    eTRIG_USB,
    eTRIG_SERIAL,
    eTRIG_NONE
} tTRIG_eType;

typedef enum
{
    eTRIG_ERROR_NONE,
    eTRIG_ERROR_TUNING,
    eTRIG_ERROR_BAT_LOADING,
    eTRIG_ERROR_BAT_NO_LINKAGE,
    eTRIG_ERROR_BAT_OUI_NO_MATCH,
    eTRIG_ERROR_TUNING_ASYNC,
    eTRIG_ERROR_PAT_LOADING,
    eTRIG_ERROR_PAT_PMT_PARSING,
    eTRIG_ERROR_PMT_OUI_NO_MATCH,
    eTRIG_ERROR_DSI_LOADING,
    eTRIG_ERROR_DSI_BAD_MODEL_VERSION,
    eTRIG_ERROR_STB_NOT_TARGETED
} tTRIG_TriggerError;

typedef struct
{
    uint32_t dummy;
} tTRIG_Param_DVBSSU_SIMPLE;

typedef struct
{
    tTRIG_ENHANCED_Info *pInfo;
    tTRIG_ENHANCED_CallBacks *pCallBacks;
} tTRIG_Param_DVBSSU_ENHANCED;

typedef struct
{
    uint32_t dummy;
} tTRIG_Param_DTG;

typedef struct
{
    char *FullFileName;
} tTRIG_Param_USB;

typedef struct
{
    tTRIG_eType eType;
    union
    {
        tTRIG_Param_DVBSSU_SIMPLE param_DVBSSU_SIMPLE;
        tTRIG_Param_DVBSSU_ENHANCED param_DVBSSU_ENHANCED;
        tTRIG_Param_DTG param_DTG;
        tTRIG_Param_USB param_USB;
    } uParam;
} tTRIG_Type;

/* client id type definition */ 
typedef int32_t tTRIG_ClientId;

/* Callback definition */
typedef tOAD_Error (* fTRIG_TuneCallback) (tOAD_TunData *pTunData);
typedef tOAD_Error (* fTRIG_GetTunerInfoCallback) (tOAD_Info *pTunerInfo);

typedef struct
{
    fTRIG_TuneCallback		 fTuneCallBack;
    fTRIG_GetTunerInfoCallback	fGetTunerInfoCallBack;
} tTRIG_DmdCallBacks;


/*******************************************************/
/* Variables Declarations (IMPORT) */
/*******************************************************/

/*******************************************************/
/* Functions Declarations */
/*******************************************************/
const char* TRIG_GetRevision (void);

tTRIG_Error TRIG_Init(void);
tTRIG_Error TRIG_Term(void);

tTRIG_Error TRIG_ClientRegister (tTRIG_ClientId *pClientId,
                                            tTRIG_DmdCallBacks *pDmdCallbacks,
                                            uint8_t ucDemuxId);
tTRIG_Error TRIG_ClientUnRegister (tTRIG_ClientId ClientId);

tTRIG_Error TRIG_CheckForDownload(tTRIG_Type Type);
tTRIG_Error TRIG_GetError(tTRIG_TriggerError *pError);
tTRIG_Error TRIG_SetError(tTRIG_TriggerError error);

tTRIG_Error TRIG_ConfirmDownload(bool confirm);
tTRIG_Error TRIG_DownloadEnd(void);

tNVM_dlState TRIG_GetDownloadStatus(void);
bool TRIG_CheckDownloadMethod(uint32_t uMethod);
tNVM_error_code TRIG_GetDownloadError(void);
uint32_t TRIG_GetUpdateID(void);
uint32_t TRIG_GetLastDetectedUpdateID(void);
uint32_t TRIG_GetCurrentUpdateID(void);
bool     TRIG_ResetDownloadStatus(void);
uint32_t TRIG_GetFactoryOADflag(void);
uint32_t TRIG_RestoreVersion(uint32_t uUpdateId);

#ifdef TRIG_USB
tTRIG_Error TRIG_USB_Init(void);
tTRIG_Error TRIG_USB_Term(void);
tTRIG_Error TRIG_USB_CheckForDownload(char *FullFileName);
#endif
#ifdef TRIG_DVBSSU
tTRIG_Error TRIG_DVBSSU_SIMPLE_CheckForDownload(void);
#endif

tTRIG_Error TRIG_SERIAL_CheckForDownload(void);

#ifdef __cplusplus
}
#endif

#endif /*_TRIGGER_H_*/
