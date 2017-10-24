/******************************************************************************
 *            COPYRIGHT 2008 IWEDIA TECHNOLOGIES            *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_VBI_SLICER
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_vbi_slicer.h $
 *
 *      $Rev: 1579 $
 *      $Date: 2009-09-04 15:11:15 +0200 (ven., 04 sept. 2009) $
 *
 * PUBLIC
 *
 * DESCRIPTION : API Prototype for TDAL_VBI_SLICER abstraction layer
 *
 *****************************************************************************/

#ifndef _TDAL_VBI_SLICER_H_
#define _TDAL_VBI_SLICER_H_


/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*       Defines              */
/*******************************************************/

#define kTDAL_VBI_SLICER_MAX_CLIENT       4
#define kTDAL_VBI_SLICER_ILLEGAL_CLIENT_ID    -1

/*******************************************************/
/*       Macros               */
/*******************************************************/

/*******************************************************/
/*       Typedefs              */
/*******************************************************/

typedef enum
{
   eTDAL_VBI_SLICER_NO_ERROR,      /* No Error returned */
   eTDAL_VBI_SLICER_NOT_INIT_ERROR,   /* TDAL_VBI_SLICER not initialised */
   eTDAL_VBI_SLICER_ALREADY_INIT_ERROR,  /* TDAL_VBI_SLICER already initialised */
   eTDAL_VBI_SLICER_BAD_ARG_ERROR,    /* Bad parameter given to the function */
   eTDAL_VBI_SLICER_DRIVER_ERROR,    /* Low level driver returns an error */
   eTDAL_VBI_SLICER_NO_MEMORY_ERROR,   /* Not enough memory to perform action*/
   eTDAL_VBI_SLICER_NOT_AVAILABLE_ERROR, /* Feature not available*/
}tTDAL_VBI_SLICER_Error;

typedef enum
{
   eTDAL_VBI_SLICER_STANDARD_PAL   = 0x00000001,
   eTDAL_VBI_SLICER_STANDARD_SECAM   = 0x00000002,
   eTDAL_VBI_SLICER_STANDARD_NTSC   = 0x00000004
}tTDAL_VBI_SLICER_Standard;

typedef enum
{
   eTDAL_VBI_SLICER_DATA_VPS     = 0x00000001,
   eTDAL_VBI_SLICER_DATA_WST     = 0x00000002,
   eTDAL_VBI_SLICER_DATA_WSS     = 0x00000004
} tTDAL_VBI_SLICER_DataType;

typedef struct TDAL_VBI_SLICER_Capability_s {
   uint32_t       StandardSupportedBitmask;
   uint32_t       DataTypeSupportedBitmask;
} tTDAL_VBI_SLICER_Capability;


typedef int32_t tTDAL_VBI_SLICER_ClientId;


typedef void (*tTDAL_VBI_SLICER_CallbackWST)(tTDAL_VBI_SLICER_ClientId  stClientId,
                    uint32_t         ulInputHandle,
                    uint16_t         usSizeOfData,
                    const void        *pData);

typedef void (*tTDAL_VBI_SLICER_CallbackWSS)(tTDAL_VBI_SLICER_ClientId  stClientId,
                    uint32_t         ulInputHandle,
                    uint16_t         usWssData);

typedef void (*tTDAL_VBI_SLICER_CallbackVPS)(tTDAL_VBI_SLICER_ClientId  stClientId,
                    uint32_t         ulInputHandle,
                    uint32_t         ulVpsDataMsb,
                    uint32_t         ulVpsDataLsb);

/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/

tTDAL_VBI_SLICER_Error  TDAL_VBI_SLICER_Init (void);
tTDAL_VBI_SLICER_Error  TDAL_VBI_SLICER_Term (void);

tTDAL_VBI_SLICER_Error  TDAL_VBI_SLICER_CapabilityGet(tTDAL_VBI_SLICER_Capability       *pstCapability);

const char *      TDAL_VBI_SLICER_APIRevisionGet(void);
const char *      TDAL_VBI_SLICER_PlatformRevisionGet(void);

tTDAL_VBI_SLICER_Error  TDAL_VBI_SLICER_ClientRegister(tTDAL_VBI_SLICER_ClientId       *pstClientId);
tTDAL_VBI_SLICER_Error  TDAL_VBI_SLICER_ClientUnregister(tTDAL_VBI_SLICER_ClientId       stClientId);


tTDAL_VBI_SLICER_Error  TDAL_VBI_SLICER_ClientRegisterWSTCallback(tTDAL_VBI_SLICER_ClientId   stClientId,
                             tTDAL_VBI_SLICER_CallbackWST  cbWSTCallback);
tTDAL_VBI_SLICER_Error  TDAL_VBI_SLICER_ClientUnregisterWSTCallback(tTDAL_VBI_SLICER_ClientId   stClientId);


tTDAL_VBI_SLICER_Error  TDAL_VBI_SLICER_ClientRegisterVPSCallback(tTDAL_VBI_SLICER_ClientId   stClientId,
                             tTDAL_VBI_SLICER_CallbackVPS  cbVPSCallback);
tTDAL_VBI_SLICER_Error  TDAL_VBI_SLICER_ClientUnregisterVPSCallback(tTDAL_VBI_SLICER_ClientId   stClientId);


tTDAL_VBI_SLICER_Error  TDAL_VBI_SLICER_ClientRegisterWSSCallback(tTDAL_VBI_SLICER_ClientId   stClientId,
                             tTDAL_VBI_SLICER_CallbackWSS  cbWSSCallback);
tTDAL_VBI_SLICER_Error  TDAL_VBI_SLICER_ClientUnregisterWSSCallback(tTDAL_VBI_SLICER_ClientId   stClientId);


tTDAL_VBI_SLICER_Error  TDAL_VBI_SLICER_ConfigureClientInput(tTDAL_VBI_SLICER_ClientId     stClientId,
                           uint32_t            ulInputHandle);

tTDAL_VBI_SLICER_Error  TDAL_VBI_SLICER_EnableInputSlicing(tTDAL_VBI_SLICER_ClientId   stClientId, uint32_t             ulInputHandle);
tTDAL_VBI_SLICER_Error  TDAL_VBI_SLICER_DisableInputSlicing(tTDAL_VBI_SLICER_ClientId   stClientId, uint32_t            ulInputHandle);

void         TDAL_VBI_SLICER_FreeWstPacket(void                 *pData);

#ifdef __cplusplus
}
#endif

#endif  /*_TDAL_VBI_SLICER_H_*/



