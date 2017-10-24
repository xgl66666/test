/******************************************************************************
 *      COPYRIGHT 2001 IWEDIA TECHNOLOGIES                                    *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_SSU
 *
 * PUBLIC
 *
 * DESCRIPTION
 *
 *****************************************************************************/

#ifndef _TDAL_SSU_H_
#define _TDAL_SSU_H_

/*******************************************************/
/*       Includes                                      */
/*******************************************************/

/*******************************************************/
/*       Defines                                       */
/*******************************************************/

/*******************************************************/
/*       Macros                                        */
/*******************************************************/


/*******************************************************/
/*       Typedefs                                      */
/*******************************************************/

/** Enumeration for errors */
typedef enum
{
    eTDAL_SSU_NO_ERROR,
    eTDAL_SSU_ERROR
} tTDAL_SSU_Error;

/** Holds enumerations for modulation */
typedef enum
{
    eTDAL_SSU_MODULATION_UNDEFINED = 0,
    eTDAL_SSU_MODULATION_QPSK,
    eTDAL_SSU_MODULATION_QAM_16,
    eTDAL_SSU_MODULATION_QAM_32,
    eTDAL_SSU_MODULATION_QAM_64,
    eTDAL_SSU_MODULATION_QAM_128,
    eTDAL_SSU_MODULATION_QAM_256,
} tTDAL_SSU_Modulation;

/**
 * Platform parameters that are used to present
 * information about hardware
 */
typedef struct stTDAL_SSU_PlatformInfo
{
    uint32_t   uiOUI;                      /* Organization Unique Identifier */

    char       cHardwareModel[10];         /* Hardware Model       */
    uint16_t   uiHardwareModel;            /* Hardware Model Id    */

    char       cHardwareVersion[10];       /* Hardware Version     */
    uint16_t   uiHardwareVersion;          /* Hardware Version Id  */

    char       cProductModel[10];          /* Product Model        */
    uint16_t   uiProductModel;             /* Product Model Id     */

    char       cProductVersion[10];        /* Product Version      */
    uint16_t   uiProductVersion;           /* Product Version Id   */

    char       cDeviceBrandName[10];       /* Device Name     */
    char       cHardwareDriverVersion[10]; /* Driver version  */
    char       cManufacturerID[10];        /* Manufacturer ID */
}tTDAL_SSU_PlatformInfo;

/**
 * Control parameters used in a process of
 * System software update.
 *
 * @note: Not all the parameters are required for
 * a particular implementation of SSU procedure.
 */
typedef struct stTDAL_SSU_ControlParameters
{
    uint8_t               bInStandBy;     /* DTV application was in standby */
    uint32_t              uiFreqency;     /* Frequency in kHz of Transport Stream carrying SSU */
    uint16_t              uiSymbolRate;   /* Symbol rate in kBd of stream */
    tTDAL_SSU_Modulation  uiModulation;   /* Modulation of stream */
    uint16_t              uiPid;          /* PID of SSU carousel */
    uint32_t              uiUpdateID;     /* In a form 0xAABBCCDD of version AA.BB.CC.DD */
}tTDAL_SSU_ControlParameters;

/**
 * Holds status parameters after SSU
 * to share information about update.
 *
 * @note: Not all parameters may be required
 * for a particular implementation.
 */
typedef struct stTDAL_SSU_StatusInfo
{
    char                  cUtcTime[5];    /* UTC time encoded as specified in TDT */
    uint32_t              uiLastUpdateID; /* Version Id, of last updated software.
                                             In a form 0xAABBCCDD of version AA.BB.CC.DD */
    bool                  bInStandBy;     /* Loader was in standby after SSU */
    bool                  bFirstBoot;     /* First boot after SSU */
    bool                  bForced;        /* True if SSU was not triggered using HAL */
    char                  cStatus[10];    /* Custom status message: Success, Failed, Suspended */
    char                  cErrorCode[5];  /* Error code in format: EXXX, or custom format */
    tTDAL_SSU_Error       eErrorStatus;   /* Status of finished SSU */
}tTDAL_SSU_StatusInfo;


/*******************************************************/
/*       Variables Declarations (IMPORT)               */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations                        */
/*******************************************************/

/**
 * This function initializes SSU module. If there are
 * anything that should be done before calls to API
 * functions this is the place to do it.
 *
 * @note Function MUST be called before any attempt to
 * use API.
 *
 */
GLOBAL void TDAL_SSU_Init(void);

/**
 * This function terminates SSU module. If there are
 * anything that should be done to safely stop and
 * de-initialize module.
 *
 * @note Function MUST be called after any attempt to
 * use API.
 *
 */
GLOBAL void TDAL_SSU_Term(void);


/**
 * Function is called to retrieve information about
 * platform and its versions.
 *
 * @param[out] pstSsuInfo is a points to allocated
 * tTDAL_SSU_PlatformParameters structure to be
 * populated by information.
 *
 * @return eTDAL_SSU_STATUS_NO_ERROR if call was successful.
 * @return eTDAL_SSU_STATUS_NO_ERROR if module is not
 * initialized or pointer is NULL.
 *
 * @note Argument pstSsuInfo must be allocated before
 * calling the function.
 *
 */
GLOBAL tTDAL_SSU_Error TDAL_SSU_GetPlatformInfo(tTDAL_SSU_PlatformInfo *pstSsuInfo);


/**
 * Function is called to retrieve information about
 * status of last SSU attempt.
 *
 * @param[out] pstSsuInfo is a points to allocated
 * tTDAL_SSU_StatusParameters structure to be
 * populated by this function.
 *
 * @return eTDAL_SSU_STATUS_NO_ERROR if call was successful.
 * @return eTDAL_SSU_STATUS_NO_ERROR if module is not
 * initialized or pointer is NULL.
 *
 * @note Argument pstSsuInfo must be allocated before
 * calling the function.
 *
 */
GLOBAL tTDAL_SSU_Error TDAL_SSU_GetStatusInfo(tTDAL_SSU_StatusInfo *pstSsuStatus);


/**
 * Function is called to set information for SSU.
 *
 * @param[out] pstSsuParams is a points to allocated
 * tTDAL_SSU_ControlParameters structure to be
 * populated by this function.
 *
 * @return eTDAL_SSU_STATUS_NO_ERROR if call was successful.
 * @return eTDAL_SSU_STATUS_NO_ERROR if module is not
 * initialized or pointer is NULL.
 *
 * @note Argument pstSsuParams must be allocated before
 * calling the function.
 *
 */
GLOBAL tTDAL_SSU_Error TDAL_SSU_SetParameters(tTDAL_SSU_ControlParameters *pstSsuParams);


/**
 * Function is called to start SSU procedure. It
 * should only start downloader.
 * In case parameters are set, and box is powered off,
 * on next boot SSU must start, even without call to
 * this function.
 *
 * @return eTDAL_SSU_STATUS_NO_ERROR if call was successful.
 * @return eTDAL_SSU_STATUS_NO_ERROR if module is not
 * initialized or pointer is NULL.
 *
 * @note It is the same as calling  to the function
 * TDAL_Reset or TDAL_Jump.
 *
 */
GLOBAL tTDAL_SSU_Error TDAL_SSU_Initiate();


#endif /*_TDAL_SSU_H_*/



