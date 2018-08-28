/******************************************************************************
 *                    COPYRIGHT 2004 IWEDIA TECHNOLOGIES                      *
 ******************************************************************************
 *
 * MODULE NAME: NVM EXCHANGE
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/product/comedia/branches/idecode_hd/build/inc/nvm_exchange.h $
 *            $Rev: 244 $
 *            $Date: 2011-08-18 18:33:39 +0200 (Thu, 18 Aug 2011) $
 *
 * PUBLIC
 *
 * DESCRIPTION: nvm exchange API header
 *
 *****************************************************************************/
 
#ifndef _NVM_EX
#define _NVM_EX

/*******************************************************/
/*              Includes                               */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************/
/*              Defines                                */
/*******************************************************/

/*******************************************************/
/*              Traces                                 */
/*******************************************************/
typedef enum
{
    NVM_EX = 0x04280200
}tNVM_EX_TracesID;

/*******************************************************/
/*              Macros                                 */
/*******************************************************/

/*******************************************************/
/*              Typedefs                               */
/*******************************************************/
typedef enum
{
    eNVM_NO_ERROR = 0,
    eNVM_ERROR,
    eNVM_BAD_CRC,
    eNVM_BAD_VERSION,
    eNVM_EMPTY
} tNVM_Error;

typedef enum
{
    /* be careful such enum values are tested by xml without including present header file */
    NVM_NODL=0,           /* no download */
    NVM_DLSTART,          /* a download must be done */
    NVM_DLCOMPLETED,      /* the download has succeedded */
    NVM_DL_ASK=6          /* a new software is available, the user will asked if the download must be done */
} tNVM_dlState;


/*******************************************************/
/*              Variables Declarations (IMPORT)        */
/*******************************************************/

/*******************************************************/
/*              Functions Declarations                 */
/*******************************************************/
struct _OPAQUE_DIALOG_AREA_STRUCTURE_;

tNVM_Error NVM_EX_Init(void);
tNVM_Error NVM_EX_WriteDefaultParam(void);
tNVM_Error NVM_EX_Write(struct _OPAQUE_DIALOG_AREA_STRUCTURE_ *psDialogArea);
tNVM_Error NVM_EX_Read(struct _OPAQUE_DIALOG_AREA_STRUCTURE_ *psDialogArea);
tNVM_Error NVM_EX_Term(void);

tNVM_Error NVM_EX_CRC_crc16Calculate(struct _OPAQUE_DIALOG_AREA_STRUCTURE_ *psDialogArea);
tNVM_Error NVM_EX_CRC_crc16Check(struct _OPAQUE_DIALOG_AREA_STRUCTURE_ *psDialogArea);



#ifdef __cplusplus
}
#endif


#endif

