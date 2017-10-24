/******************************************************************************
 *            COPYRIGHT 2004 IWEDIA TECHNOLOGIES        *
 ******************************************************************************
 *
 * MODULE NAME: TDAL SMC
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_smc.h $
 *      $Rev: 5745 $
 *      $Date: 2011-05-09 12:25:36 +0200 (lun., 09 mai 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION - Header of the TDAL Smart card
 *
 *****************************************************************************/

#ifndef _TDAL_SMC_H_
#define _TDAL_SMC_H_

/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************/
/*       Defines              */
/*******************************************************/

/*******************************************************/
/*       Macros               */
/*******************************************************/

/*******************************************************/
/*       Typedefs              */
/*******************************************************/
typedef enum {
   eTDAL_SMC_NO_ERROR,
   eTDAL_SMC_NULL_POINTER,
   eTDAL_SMC_UNKNOWN_CA,
   eTDAL_SMC_NOT_INSERTED,
   eTDAL_SMC_BAD_PARAMETER,
   eTDAL_SMC_ERROR
}tTDAL_SMC_ErrorCode;


typedef enum
{
   eTDAL_SMC_SLOT1,
   eTDAL_SMC_SLOT2
}tTDAL_SMC_Slot;


typedef enum
{
   eTDAL_SMC_CBF_RESET,
   eTDAL_SMC_CBF_WRITE,
   eTDAL_SMC_CBF_READ,
   eTDAL_SMC_CBF_PPS,
   eTDAL_SMC_CBF_TRANSFERT_ERROR, /* Read/Write/PPS error without S.W. to explain the problem */
   eTDAL_SMC_CBF_INSERT,
   eTDAL_SMC_CBF_EXTRACT,
   eTDAL_SMC_CBF_INIT
}tTDAL_SMC_Event;

typedef enum
{
   eTDAL_SMC_CA_VIACCESS   = 0x05,
   eTDAL_SMC_CA_IRDETO   = 0x06,
   eTDAL_SMC_CA_CRYPTOWORKS = 0x0D,
   eTDAL_SMC_CA_CONAX    = 0x0B,
   eTDAL_SMC_CA_NAGRA    = 0x18,
   eTDAL_SMC_NO_CA     = 0xFF
}tTDAL_SMC_CA_Type;

typedef enum
{
   eTDAL_SMC_STATE_ERROR,
   eTDAL_SMC_STATE_UNKNOWN,
   eTDAL_SMC_OPERATIONAL,
   eTDAL_SMC_EXTRACTED,
   eTDAL_SMC_INSERTED
}tTDAL_SMC_State;


typedef void (*tTDAL_SMC_CBF)(
   tTDAL_SMC_Slot  slot,
   tTDAL_SMC_Event Event,
   uint8_t* Buffer,
   uint32_t BufferLength,
   uint8_t  SW1,
   uint8_t  SW2);

/* NOTE: the following types MUST be the same as in file GAL_CAM_status_card.h */
typedef enum
{
   eTDAL_SMC_CAM_UNDEF    = 0xffff,
   eTDAL_SMC_CAM_CLEAR    = 0xfffe,
   eTDAL_SMC_CAM_CPLUS    = 0x0100,
   eTDAL_SMC_CAM_CCETT    = 0x0200,
   eTDAL_SMC_CAM_DT     = 0x0300,
   eTDAL_SMC_CAM_EURODEC   = 0x0400,
   eTDAL_SMC_CAM_VIACCESS   = 0x0500,
   eTDAL_SMC_CAM_IRDETO   = 0x0600,
   eTDAL_SMC_CAM_JERROLD   = 0x0700,
   eTDAL_SMC_CAM_MATRA    = 0x0800,
   eTDAL_SMC_CAM_DATACOM   = 0x0900,
   eTDAL_SMC_CAM_NOKIA    = 0x0a00,
   eTDAL_SMC_CAM_CONAX    = 0x0b00,
   eTDAL_SMC_CAM_NTL     = 0x0c00,
   eTDAL_SMC_CAM_PHILIPS   = 0x0d00,
   eTDAL_SMC_CAM_ATLANTA   = 0x0e00,
   eTDAL_SMC_CAM_SONY    = 0x0f00,
   eTDAL_SMC_CAM_TANBERG   = 0x1000,
   eTDAL_SMC_CAM_THOMSON   = 0x1100,
   eTDAL_SMC_CAM_TV_COM   = 0x1200,
   eTDAL_SMC_CAM_HPT     = 0x1300,
   eTDAL_SMC_CAM_HRT     = 0x1400,
   eTDAL_SMC_CAM_IBM     = 0x1500,
   eTDAL_SMC_CAM_NERA    = 0x1600,
   eTDAL_SMC_CAM_BETA    = 0x1700,
   eTDAL_SMC_CAM_KUDELSKI   = 0x1800,
   eTDAL_SMC_CAM_TITAN_IS   = 0x1900,
   eTDAL_SMC_CAM_TSA     = 0x2000,
   eTDAL_SMC_CAM_STENTOR   = 0x2100
}tTDAL_SMC_CAM_Access_Type;

typedef enum
{
   eTDAL_SMC_CAM_OPERATIONAL     =1,
   eTDAL_SMC_CAM_EXTRACTED      =0,
   eTDAL_SMC_CAM_INSERTED      =-1,
   eTDAL_SMC_CAM_STATE_ERROR     =-2,
   eTDAL_SMC_CAM_STATE_UNKNOWN    =-3
}tTDAL_SMC_CAM_State;



typedef void (*tTDAL_SMC_AccessType_Notif)(
   tTDAL_SMC_CAM_Access_Type   sc_type,
   tTDAL_SMC_CAM_State     ca_state);

/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/
tTDAL_SMC_ErrorCode TDAL_SMC_Init(
   void
);

tTDAL_SMC_ErrorCode TDAL_SMC_Terminate(
   void
);

tTDAL_SMC_ErrorCode TDAL_SMC_Open(
   tTDAL_SMC_CA_Type ca_type,
   tTDAL_SMC_CBF callback
);

tTDAL_SMC_ErrorCode TDAL_SMC_Close(
   tTDAL_SMC_CA_Type ca_type
);

tTDAL_SMC_ErrorCode TDAL_SMC_GetState(
   tTDAL_SMC_Slot slot,
   tTDAL_SMC_State *pState
);

tTDAL_SMC_ErrorCode TDAL_SMC_Reset(
   tTDAL_SMC_Slot slot,
   bool isCold
);

tTDAL_SMC_ErrorCode TDAL_SMC_ATR(
   tTDAL_SMC_Slot slot,
   uint8_t*   ATRBuffer,
   uint32_t*   ATRBufferLength
);

tTDAL_SMC_State TDAL_SMC_State(
   tTDAL_SMC_Slot slot,
   tTDAL_SMC_CA_Type ca_type   /* NOT USED */
);

tTDAL_SMC_ErrorCode TDAL_SMC_GetClockFrequency(
   tTDAL_SMC_Slot slot,
   uint32_t* pFreq
);
tTDAL_SMC_ErrorCode TDAL_SMC_GetTimeout(
   tTDAL_SMC_Slot slot, 
   uint32_t* pTimeout
);
tTDAL_SMC_ErrorCode TDAL_SMC_ReadWrite(
   tTDAL_SMC_Slot slot,
   uint8_t*   command,
   uint8_t*   data,
   bool is_write
);

tTDAL_SMC_ErrorCode TDAL_SMC_RawReadWrite(
   tTDAL_SMC_Slot slot,
   uint8_t *data,
   uint8_t expectedLength,
   uint32_t *pNumberExchanged,
   bool is_write);

tTDAL_SMC_ErrorCode TDAL_SMC_PPS_Negociation(
   tTDAL_SMC_Slot slot,
   uint8_t PPS_Request[5]
);

tTDAL_SMC_ErrorCode TDAL_SMC_CA_Autho_Register(
   tTDAL_SMC_AccessType_Notif Notif_fct
);


#ifdef __cplusplus
}
#endif


#endif /*_TDAL_SMC_H_*/

