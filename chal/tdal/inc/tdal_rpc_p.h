/*****************************************************************************
 *         COPYRIGHT 2006 IWEDIA TECHNOLOGIES         *
 *****************************************************************************
 *
 * MODULE NAME: TDAL RPC
 *
 * FILE NAME: $URL: http://10.50.1.219/svncomedia/sources/chal/core/trunk/tdal/inc/tdal_rpc_p.h $
 *      $Rev: 66 $
 *      $Date: 2008-10-14 10:29:19 +0200 (mar., 14 oct. 2008) $
 *
 * PRIVATE
 *
 * DESCRIPTION - Private definition for TDAL RPC.
 *
 *****************************************************************************/

#ifndef _TDAL_RPC_P_H_
#define _TDAL_RPC_P_H_

/*******************************************************/
/*       Includes              */
/*******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*       Defines              */
/*******************************************************/

#define kTDAL_RPCP_SZMAX_DUMP   800 /* Bytes dump maxi (260 x 3 char) + 20   */


/*******************************************************/
/*       Macros               */
/*******************************************************/

/*******************************************************/
/*       Typedefs              */
/*******************************************************/

/*
 * Physical communication on serial port handle
 * generic type, OS dependence.
 */
typedef uint32_t tTDAL_RPCP_HandleComm;

/*
 * Wait synchronious response state
 */
typedef enum
{
   eTDAL_RPCP_WRS_NONE,
   eTDAL_RPCP_WRS_WAIT,
   eTDAL_RPCP_WRS_READY
} tTDAL_RPCP_WaitRespState;

/*
 * Task state
 */
typedef enum
{
   eTDAL_TASK_STATE_FREE,
   eTDAL_TASK_STATE_IN_START,
   eTDAL_TASK_STATE_RUNNING,
   eTDAL_TASK_STATE_IN_STOP,
   eTDAL_TASK_STATE_STOPPED
} tTDAL_RPCP_TaskState;

/*
 * Command manager context 
 */
typedef struct
{
   bool          bIsStarted;   /* TRUE when context started   */
   tTDAL_RPC_ClientId    idCli;    /* Id client          */
   tTDAL_RPC_SerialPort   eSerialPort;  /* Serial port         */
   uint8_t         ucAddrLocal;  /* Local host address      */
   uint8_t         ucAddrTarget; /* Target address        */
   uint32_t         uiCmdFamily;  /* Commands family subscribed   */
   tTDAL_RPC_CmdRecvFct   cbCmdRecvFtc; /* Callback to transmit command   */
   /* Synchrone command management :                  */
   tTDAL_RPCP_WaitRespState  eWaitResp;   /* Wait resp synchrone state   */
   uint8_t         ucIdResp;   /* Id response waiting      */
   tTDAL_RPC_Command    *pResp;    /* Ptr where set response waiting */
   /* Client task management :                     */
   TKEL_task_id       idTask;    /* TKEL task id         */
   TKEL_mutex_id      idMutex;   /* TKEL mutex id         */
   TKEL_queue_id      idQueue;   /* TKEL queue id         */
   tTDAL_RPCP_TaskState   eTaskState;   /* State of the task       */
} tTDAL_RPCP_CmdmCtx;

/*
 * Communications context 
 */
typedef struct
{
   bool          bIsStarted;   /* TRUE when context started   */
   int           iNbUsers;   /* Nb users (comm started if > 0) */
   tTDAL_RPC_SerialPort   eSerialPort;  /* The port           */
   tTDAL_RPC_SP_Config    theConfig;   /* Port configuration      */
   tTDAL_RPCP_HandleComm   hdlComm;   /* Handle of comm opened     */
   TKEL_task_id       idTask;    /* TKEL task id         */
   TKEL_mutex_id      idMutex;   /* TKEL mutex id         */
   tTDAL_RPCP_TaskState   eTaskState;   /* State of the task       */
   int32_t         iSzBuffR;   /* Size of read buffer      */
   uint8_t         tucBuffR[kTDAL_RPC_CMD_SZMAX*2]; /* Read buffer */
} tTDAL_RPCP_CommCtx;



/*******************************************************/
/*       Variables Declarations (IMPORT)    */
/*******************************************************/

#ifdef _TDAL_RPC_C_
#ifdef ST_OSLINUX
#define _CLASS kTDAL_MODULE
#else
#define _CLASS MODULE
#endif
#else
#define _CLASS IMPORT
#endif

/* Contexts Client (command) and COMM (seroal port) */
_CLASS tTDAL_RPCP_CmdmCtx TDAL_RPCP_ctxCmdm[kTDAL_RPC_NB_MAX_CLIENTS];
_CLASS tTDAL_RPCP_CommCtx TDAL_RPCP_ctxComm[eTDAL_RPC_SP_NBMAXI];

/* Key forcing stack management */
_CLASS uint8_t TDAL_RPCP_ucNbKeysStack;
_CLASS uint8_t TDAL_RPCP_tucStask[kTDAL_RPC_NB_MAX_KEY_FORCED];

/*******************************************************/
/*       Functions Declarations        */
/*******************************************************/

/*
 * Commands Manager  functions
 */
tTDAL_RPC_ErrorCode TDAL_RPCP_CmdmInit   (int32_t iDelayRead);
tTDAL_RPC_ErrorCode TDAL_RPCP_CmdmDone   (void);
tTDAL_RPC_ErrorCode TDAL_RPCP_CmdmStart   (tTDAL_RPCP_CmdmCtx *pCtx);
tTDAL_RPC_ErrorCode TDAL_RPCP_CmdmStop   (tTDAL_RPCP_CmdmCtx *pCtx);
tTDAL_RPC_ErrorCode TDAL_RPCP_CmdmSend   (tTDAL_RPCP_CmdmCtx *pCtx,
                   tTDAL_RPC_Command *pCommand);
tTDAL_RPC_ErrorCode TDAL_RPCP_CmdmExec   (tTDAL_RPCP_CmdmCtx *pCtx,
                   tTDAL_RPC_Command *pCommand,
                   tTDAL_RPC_Command *pResponse,
                   int16_t iTimeoutInt,
                   int16_t iTimeoutNum);
tTDAL_RPC_ErrorCode TDAL_RPCP_CmdmRecv   (tTDAL_RPC_SerialPort ePort,
                   uint8_t *pBufCmd,
                   uint8_t ucSzCmd);
tTDAL_RPC_ErrorCode TDAL_RPCP_ExecKeyStack (void);

/*
 * Communications functions
 */
tTDAL_RPC_ErrorCode TDAL_RPCP_CommInit   (int32_t iDelayRead);
tTDAL_RPC_ErrorCode TDAL_RPCP_CommDone   (void);
tTDAL_RPC_ErrorCode TDAL_RPCP_CommSetConfig (tTDAL_RPC_SerialPort ePort,
                    tTDAL_RPC_SP_Config *pConfig);
tTDAL_RPC_ErrorCode TDAL_RPCP_CommStart   (tTDAL_RPC_SerialPort ePort);
tTDAL_RPC_ErrorCode TDAL_RPCP_CommStop   (tTDAL_RPC_SerialPort ePort,
                    bool bForce);
tTDAL_RPC_ErrorCode TDAL_RPCP_CommSend   (tTDAL_RPC_SerialPort ePort,
                    uint8_t *pData, int32_t iSzData);
void       TDAL_RPCP_CommTrcFKey   (tTDAL_RPC_SerialPort ePort, 
                    uint8_t ucKey);

/*
 * Serial Port (OS dependence) management
 */
tTDAL_RPC_ErrorCode TDAL_RPCP_SP_Open  (tTDAL_RPCP_CommCtx *pCtx);
tTDAL_RPC_ErrorCode TDAL_RPCP_SP_Close (tTDAL_RPCP_CommCtx *pCtx);
tTDAL_RPC_ErrorCode TDAL_RPCP_SP_Read  (tTDAL_RPCP_CommCtx *pCtx, 
                  uint8_t *pData, int32_t iSzMax, 
                  int32_t *pSzReaded);
tTDAL_RPC_ErrorCode TDAL_RPCP_SP_Write (tTDAL_RPCP_CommCtx *pCtx, 
                  uint8_t *pData, int32_t iSzData);

/* 
 * Divers
 */
char   *TDAL_RPCP_Dump   (char *pDump, uint8_t *pData, int iSzData);
uint32_t TDAL_RPCP_GetTimeT (void);


#ifdef __cplusplus
}
#endif

#endif /*_TDAL_RPC_P_H_*/

/********************************************************/
/* End tdal_rpc_p.h                */
/********************************************************/
