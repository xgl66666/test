/******************************************************************************
 *                         COPYRIGHT 2004 IWEDIA TECHNOLOGIES                 *
 ******************************************************************************
 *
 * MODULE NAME: TDAL SMC
 *
 * FILE NAME: $URL: http://10.50.1.219/svn/sources/hal/tdal/trunk/TDAL_SMC/src/arch/ST5100/tdal_smc_priv.h $
 *            $Rev: 8396 $
 *            $Date: 2005-07-01 12:05:07 +0200 (Fri, 01 Jul 2005) $
 *
 * PUBLIC
 *
 * DESCRIPTION - Private Header of the TDAL Smart card
 *
 *****************************************************************************/

#ifndef _TDAL_SMC_P_H_
#define _TDAL_SMC_P_H_

/*******************************************************/
/*              Includes                               */
/*******************************************************/

/*******************************************************/
/*              Defines                                */
/*******************************************************/
#define kTDAL_SMC_MAX_CLIENTS 2
#define kTDAL_SMC_MAX_MSG 20
#define kTDAL_SMC_STACKSIZE 4096
#define kTDAL_SMC_PRIORITY 65
#define TDAL_SMC_MAX_ATR_LENGTH 32
#define TDAL_SMC_MAX_APDU_BUFFER_LENGTH 256

/*******************************************************/
/*              Macros                                 */
/*******************************************************/

/*******************************************************/
/*              Typedefs                               */
/*******************************************************/
typedef enum
{
    eTDAL_SMC_CARD_INSERTED,
    eTDAL_SMC_CARD_REMOVED,
    eTDAL_SMC_CARD_DEACTIVATED,
    eTDAL_SMC_CARD_RESET,
    eTDAL_SMC_TRANSFER_COMPLETED,
    eTDAL_SMC_RESET_NO_CARD,
    eTDAL_SMC_RESET_ERROR
}tTDAL_SMC_msg_type;

typedef enum
{
    eTDAL_SMC_WRITE,
    eTDAL_SMC_READ,
    eTDAL_SMC_PPS
}tTDAL_SMC_Request;

typedef struct
{
    bool              is_open;
    bool              reset_Ok;
    tTDAL_SMC_State   statusCard;
    MS_U8             SCID;
    SC_Status         status;
    uint8_t           ATR[TDAL_SMC_MAX_ATR_LENGTH];
    uint8_t           ATR_len;
    tTDAL_SMC_Request RW_Request;

    uint8_t           Answer[TDAL_SMC_MAX_APDU_BUFFER_LENGTH];
    uint32_t          lenAnswer;
    uint8_t           APDU[TDAL_SMC_MAX_APDU_BUFFER_LENGTH+7]; /* With command */
    uint32_t          lenAPDU;
    uint32_t          numWrite;
    uint32_t          numRead;
}tTDAL_SMC_status;

typedef struct
{
    tTDAL_SMC_CA_Type ca_type;
    tTDAL_SMC_CBF callback;
}tTDAL_SMC_Clients;

typedef struct
{
    tTDAL_SMC_msg_type type;
    unsigned char slotID;
}tTDAL_SMC_msg;


#endif /*_TDAL_SMC_P_H_*/

