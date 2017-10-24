/******************************************************************************
 *                     COPYRIGHT 2004 IWEDIA TECHNOLOGIES                     *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_SMC
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_smc/tdal_smc.c $
 *            $Rev: 414 $
 *            $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION : TDAL Smart Card driver implementation
 *
 *****************************************************************************/

/********************************************************/
/*            Includes                        */
/********************************************************/
#include "crules.h"
#include "tdal_common.h"
#include "MsTypes.h"
#include "drvSC.h"
#include "tdal_smc.h"
#include "tdal_smc_priv.h"
#include "tbox.h"
#include "tkel.h"



#define SC_USE_IO_51

#ifdef SC_USE_IO_51
//#include "MsMemory.h"
#include "drvPM.h"
#include "drvMBX.h"
#include "MsOS.h"
#endif

/********************************************************/
/*            Defines                           */
/********************************************************/
#define kTDAL_SMC_NB_SLOTS 1
/********************************************************/
/*            Macros                           */
/********************************************************/
mTBOX_SET_MODULE(eTDAL_SMC);


#if defined (__LOCAL_SMC_DEBUG__)
#define mSMC_DEBUG(x) printf x
#else
#define mSMC_DEBUG(x)
#endif

#define SC_T1_SEND_TIMEOUT             70// 1.5 //50
#define SC_T1_RECV_TIMEOUT             70// 1.5 //80

#define PM51_LITE_AVAILABLE                                    0x0003030000
#define PM51_LITE_ADR                                          0x0003030000  //Alignment 0x01000
#define PM51_LITE_GAP_CHK                                      0x0000000000
#define PM51_LITE_LEN                                          0x0000008000
#define PM51_MEM_ADR                                           0x0003038000  //Alignment 0x01000

/********************************************************/
/*            Typedefs                        */
/********************************************************/

/********************************************************/
/*            Global   Variables   (GLOBAL/IMPORT)      */
/********************************************************/

/********************************************************/
/*            Local   Module   Variables   (MODULE)         */
/********************************************************/

/********************************************************/
/*            Local   File   Variables   (LOCAL)         */
/********************************************************/
LOCAL tTDAL_SMC_status           TDAL_SMC_status[kTDAL_SMC_NB_SLOTS];
LOCAL tTDAL_SMC_Clients          TDAL_SMC_Clients[kTDAL_SMC_MAX_CLIENTS];
LOCAL TKEL_queue_id              TDAL_SMC_QueueId;
LOCAL TKEL_task_id               TDAL_SMC_TaskId;
LOCAL MS_U8                      u8SCID;
LOCAL TDAL_mutex_id TdalSMCMutexID = NULL;

static MS_U8    u8Protocol = 0x01;
static MS_U8    u8Fi = 0x01;
static MS_U8    u8Di = 0x01;
MS_U8  _u8AtrBuf[SC_ATR_LEN_MAX];
MS_U16 _u16AtrLen = 0;
SC_Param      sInitParams;
#ifdef SC_USE_IO_51
MS_U8   u8SCFirmware[] = {
    #include "fwSC.dat"
};
#endif
/********************************************************/
/*            Functions   Definitions   (LOCAL/GLOBAL)   */
/********************************************************/
/* Main Smart Card Task */
LOCAL void Notify_Slot_0(MS_U8 u8SCID, SC_Event eEvent);
LOCAL void SMC_Task(void *arg);
LOCAL tTDAL_SMC_ErrorCode SMCi_OpenSlot  (tTDAL_SMC_Slot slot);
/* Callback function to notify about Smart Card driver events (Read/Write/Reset/...) */
LOCAL void SMC_Callback(tTDAL_SMC_Slot  slot, tTDAL_SMC_Event Event, uint8_t* Buffer, uint32_t BufferLength, uint8_t  SW1, uint8_t  SW2);
//MARIJA DEBUG
LOCAL void SMCi_GetInfo ();
LOCAL void SMC_PrintData(MS_U8 *pu8Data, MS_U16 u16DataLen);

LOCAL tTDAL_SMC_ErrorCode SMCi_CloseSlot (tTDAL_SMC_Slot slot);
#ifdef SC_USE_IO_51
MS_BOOL SMC_Load_Fw(MS_U32 u32FwSCAddr);
#endif
/*===================================================================
 *
 * TDAL_SMC_Init
 *
 * Parameters:
 *    none.
 *
 * Description:
 *    This function creates the Smart Card Events Receiver task
 *    and initializes global structure.
 *
 * Returns: tTDAL_SMC_ErrorCode
 *    eTDAL_SMC_NO_ERROR if no problem,
 *    eTDAL_SMC_ERROR if an error has occurred.
 *
 *==================================================================*/
tTDAL_SMC_ErrorCode   TDAL_SMC_Init()
{
    SC_Result    errCode;
    tTDAL_SMC_ErrorCode rc;
    unsigned char    i;

    mTBOX_FCT_ENTER("TDAL_SMC_Init");

    TDAL_CreateMutex(&TdalSMCMutexID);

#ifdef SC_USE_IO_51    
    SMC_Load_Fw(PM51_LITE_ADR);
#endif 

    u8SCID = eTDAL_SMC_SLOT1;
    /*  Init the smart card driver */
    if ((errCode = MDrv_SC_Init(u8SCID)) != E_SC_OK)
    {
        mTBOX_TRACE(( kTBOX_NIV_1, "[%s] MDrv_SC_Init failed with error code=%d !!!!\n", __FUNCTION__, errCode));
        return (eTDAL_SMC_ERROR);
    }
    /* Initialize Global Structures */
   for(i=0; i<kTDAL_SMC_NB_SLOTS; i++)
    {
        TDAL_SMC_status[i].is_open	= FALSE;
        TDAL_SMC_status[i].statusCard = eTDAL_SMC_EXTRACTED;
        TDAL_SMC_status[i].reset_Ok	= FALSE;
        TDAL_SMC_status[i].ATR_len	= 0;
        TDAL_SMC_status[i].lenAnswer	= 0;
        TDAL_SMC_status[i].lenAPDU	= 0;
        TDAL_SMC_status[i].numWrite	= 0;
        TDAL_SMC_status[i].numRead	= 0;
    }
    for(i=0; i<kTDAL_SMC_MAX_CLIENTS; i++)
    {
        TDAL_SMC_Clients[i].ca_type = eTDAL_SMC_NO_CA;
        TDAL_SMC_Clients[i].callback = NULL;
    }

    /* Create Main Message Queue */
    TKEL_CreateQueue(kTDAL_SMC_MAX_MSG, sizeof(tTDAL_SMC_msg), &TDAL_SMC_QueueId);
    if((&TDAL_SMC_QueueId) == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"ERROR : message_create_queue() returned NULL value\r\n"));
        mTBOX_RETURN(eTDAL_SMC_ERROR);
    }

    /* Create Main Task */
    if (TKEL_CreateTask(kTDAL_SMC_PRIORITY,
                           "SMC_Task",
                           kTDAL_SMC_STACKSIZE,
                           SMC_Task,
                           NULL,
                           &TDAL_SMC_TaskId) != TKEL_NO_ERR)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"ERROR : task_create() failure\r\n"));
        mTBOX_RETURN(eTDAL_SMC_ERROR);
    }

    rc = SMCi_OpenSlot(eTDAL_SMC_SLOT1);
    if (rc != eTDAL_SMC_NO_ERROR)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                     "ERROR : Open slot 1 failure\n"));
                     mTBOX_RETURN(eTDAL_SMC_ERROR);
    }

#if (kTDAL_SMC_NB_SLOTS == 2)
    rc = SMCi_OpenSlot(eTDAL_SMC_SLOT2);
    if (rc != eTDAL_SMC_NO_ERROR)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                    "ERROR : Open slot 2 failure\n"));
                    mTBOX_RETURN(eTDAL_SMC_ERROR);
    }
#endif
//printf("\n****************************************MARIJA       SMC INIT DONE\n");
    return (eTDAL_SMC_NO_ERROR);

}


/*===================================================================
 *
 * TDAL_SMC_Open
 *
 * Parameters:
 *    ca_type : the value that identifies the CA.
 *    callback : this structure contains the pointer to the callback.
 *
 * Description:
 *    This function registers all callbacks.
 *
 * Returns: tTDAL_SMC_ErrorCode
 *    eTDAL_SMC_NO_ERROR in case of success.
 *
 *==================================================================*/
tTDAL_SMC_ErrorCode   TDAL_SMC_Open(tTDAL_SMC_CA_Type   ca_type, tTDAL_SMC_CBF   callback)
{
    unsigned char i;
    mTBOX_FCT_ENTER("TDAL_SMC_Open");

    if (callback == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"ERROR : CA Client passed a NULL pointer to TDAL_SMC_Open() !\r\n"));
        mTBOX_RETURN(eTDAL_SMC_NULL_POINTER);
    }

    for(i=0; i<kTDAL_SMC_MAX_CLIENTS; i++)
    {
        if(TDAL_SMC_Clients[i].ca_type == ca_type)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"ERROR : CA Client already registered to TDAL_SMC_Open() !\r\n"));
            mTBOX_RETURN(eTDAL_SMC_BAD_PARAMETER);
        }
    }

    for(i=0; i<kTDAL_SMC_MAX_CLIENTS; i++)
    {
        if(TDAL_SMC_Clients[i].ca_type == eTDAL_SMC_NO_CA)
        {
            break;
        }
    }

    if(i == kTDAL_SMC_MAX_CLIENTS)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,"NB Max Clients reached in TDAL_SMC_Open() !\r\n"));
        mTBOX_RETURN(eTDAL_SMC_ERROR);
    }

    TDAL_SMC_Clients[i].ca_type = ca_type;
    TDAL_SMC_Clients[i].callback = callback;

    mTBOX_RETURN(eTDAL_SMC_NO_ERROR);

}


/*===================================================================
 *
 * TDAL_SMC_Close
 *
 * Parameters:
 *    ca_type : the value that identifies the CA.
 *
 * Description:
 *    This function unregisters the callback.
 *
 * Returns: UINT8
 *    eTDAL_SMC_NO_ERROR in case of success.
 *
 *==================================================================*/
tTDAL_SMC_ErrorCode   TDAL_SMC_Close(tTDAL_SMC_CA_Type   ca_type)
{
    unsigned char i;
    unsigned char nb_register=0;
    unsigned char nb_unregister=0;
    mTBOX_FCT_ENTER("TDAL_SMC_Close");

    for(i=0; i<kTDAL_SMC_MAX_CLIENTS; i++)
    {
        if(TDAL_SMC_Clients[i].ca_type == ca_type)
        {
            TDAL_SMC_Clients[i].ca_type = eTDAL_SMC_NO_CA;
            TDAL_SMC_Clients[i].callback = NULL;
            nb_unregister=nb_unregister+1;
        }
        if(TDAL_SMC_Clients[i].callback!=NULL)nb_register++;
    }
    if(nb_register==0)
    {
        MDrv_SC_Deactivate(u8SCID);
    }
    if(nb_unregister!=0)
    {
        mTBOX_RETURN(eTDAL_SMC_NO_ERROR);
    }
    /* Close the associated smart card slot */
    mTBOX_RETURN(eTDAL_SMC_ERROR);
}

/*===================================================================
 *
 * TDAL_SMC_GetState
 *
 * Parameters:
 *    slot : Specifies the smart card slot identifier.
 *    pState: if defined, card state returned.
 *            if NULL, the card state is notified by the callback with
 *            event eTDAL_SMC_CBF_INSERT or eTDAL_SMC_CBF_EXTRACT.
 *
 * Description:
 *    This function get the card status to determine synchronously or
 *    asynchronously with the client callbask the card state.
 *
 * Returns: tTDAL_SMC_ErrorCode
 *    eTDAL_SMC_NO_ERROR in case of success.
 *    eTDAL_SMC_ERROR    if error driver
 *
 *==================================================================*/
tTDAL_SMC_ErrorCode TDAL_SMC_GetState (tTDAL_SMC_Slot slot,
                                       tTDAL_SMC_State *pState)
{
    SC_Result	   rc;
    SC_Status    last_status;
    tTDAL_SMC_State	   state;
    tTDAL_SMC_msg	  msg;

    mTBOX_FCT_ENTER("TDAL_SMC_GetState");
//   printf("\n\n\n********************TDAL_SMC_GetState    ENTER ******************\n");

    if (pState != NULL) *pState = eTDAL_SMC_STATE_UNKNOWN;

    rc = MDrv_SC_GetStatus(u8SCID,&last_status);

    switch(rc)
    {
    case E_SC_OK: 
        /* No error: the card is inserted and the last operation succeeded */
        mTBOX_TRACE((kTBOX_NIV_1,"TDAL_SMC_GetState(slot %d): OKAY => OPERATIONNAL\r\n",slot));
		if(last_status.bCardIn == TRUE)
            state = eTDAL_SMC_OPERATIONAL;
        else
            state = eTDAL_SMC_EXTRACTED;
        break;

    case E_SC_PARMERROR:
        /* Invalid Handle passed */
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,
        "TDAL_SMC_GetState ERROR : Parameter error\r\n"));
        mTBOX_RETURN(eTDAL_SMC_ERROR);
        break;
 
    case E_SC_CARDOUT:
        /* The card is not inserted */
        mTBOX_TRACE((kTBOX_NIV_1,"TDAL_SMC_GetState(slot %d): NOT INSERTED => EXTRACTED\r\n",slot));
        state = eTDAL_SMC_EXTRACTED;
        break;
    default:
        /* Unwaiting error code => indicated INSERTED !!! */
        mTBOX_TRACE((kTBOX_NIV_1,"TDAL_SMC_GetState(slot %d): code=%d => UNKNOWN\r\n",slot,rc));
        state = eTDAL_SMC_STATE_UNKNOWN;
    }
     //printf("MARIJA GET STATE     returns %d\n",rc);
    if (pState != NULL)
    {
        *pState = state;
    }
    else
    {
        /* Asynchronous state notification */
        TDAL_LockMutex(TdalSMCMutexID);
//        printf("\n\n\n********************TDAL_SMC_GetState    %d           TDAL_SMC_status[slot].statusCard  %d******************\n",last_status.bCardIn,TDAL_SMC_status[slot].statusCard );
        msg.slotID = slot;
        if((last_status.bCardIn == TRUE)&& ((TDAL_SMC_status[slot].statusCard == eTDAL_SMC_INSERTED) || (TDAL_SMC_status[slot].statusCard == eTDAL_SMC_OPERATIONAL)))
        {
            msg.type = eTDAL_SMC_CARD_INSERTED;
//           printf("********************TDAL_SMC_GetState    SEND INSERTED EVENT******************\n\n\n");
            TKEL_Enqueue(TDAL_SMC_QueueId, (void *)&msg);
        }
            
        if((last_status.bCardIn == FALSE) && (TDAL_SMC_status[slot].statusCard == eTDAL_SMC_EXTRACTED))
        {
            msg.type = eTDAL_SMC_CARD_REMOVED;
            TKEL_Enqueue(TDAL_SMC_QueueId, (void *)&msg);
        }
        TDAL_UnlockMutex(TdalSMCMutexID);
    }
    mTBOX_RETURN(eTDAL_SMC_NO_ERROR);
}
/*===================================================================
 *
 * TDAL_SMC_Reset
 *
 * Parameters:
 *    slot : Specifies the smart card slot identifier.
 *
 * Description:
 *    This function performs a Hardware Reset.
 *
 * Returns: tTDAL_SMC_ErrorCode
 *    eTDAL_SMC_NO_ERROR in case of success.
 *
 *==================================================================*/
tTDAL_SMC_ErrorCode   TDAL_SMC_Reset(tTDAL_SMC_Slot   slot, bool isCold)
{
    bool       	   ok  = TRUE;
    SC_Result	   rc  = E_SC_OK;
    tTDAL_SMC_msg	   msg;
    tTDAL_SMC_msg_type  msgType;
    MS_U8               history[SC_HIST_LEN_MAX];
    MS_U16              history_len = 200;
    MS_U8 guardTime;
    SC_Info * pInfo;
    mTBOX_FCT_ENTER("TDAL_SMC_Reset");

    /* 
        * Init message to send if not OK at RESET_ERROR 
         * when no card detected set RESET_NO_CARD
    */
    /* COLD Reset: Deactivate card */
    if (isCold)
    {
        /* Clean reset context of the slot */
        TDAL_SMC_status[slot].reset_Ok   = FALSE;
        TDAL_SMC_status[slot].ATR_len    = 0;
        TDAL_SMC_status[slot].lenAnswer  = 0;
        TDAL_SMC_status[slot].lenAPDU    = 0;
        TDAL_SMC_status[slot].numWrite   = 0;
        TDAL_SMC_status[slot].numRead    = 0;

        /* Deactivate */
        rc = MDrv_SC_Deactivate(u8SCID);
        MsOS_DelayTask(50);
        rc = MDrv_SC_Activate(u8SCID);
        MsOS_DelayTask(50);

        if (rc != E_SC_OK)
        {
            ok = FALSE;
            mSMC_DEBUG(("[SMC DBG] TDAL_SMC_Reset(slot %d,COLD) ERROR: "
                        "Deactivate failure (%s)\n",
                        slot,SMCi_StrError(rc)));
        }
    }


    /* The reset */
    if (ok)
   {
        /* Init params */
        sInitParams.eCardClk = E_SC_CLK_4P5M;
        sInitParams.u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_NO;
        sInitParams.u16ClkDiv = 372;
        sInitParams.eVccCtrl	 = E_SC_VCC_CTRL_8024_ON;
        sInitParams.pfOCPControl = NULL;
        sInitParams.u8Convention = 1;
        rc = MDrv_SC_Reset(u8SCID, &sInitParams);
        MsOS_DelayTask(50);

        if(rc == E_SC_OK)
        {
            rc = MDrv_SC_ClearState(u8SCID);

            rc = MDrv_SC_GetATR(u8SCID, 30000, _u8AtrBuf, &_u16AtrLen, history, &history_len);

            if(rc==E_SC_OK)
            {
                if (_u8AtrBuf[0] == 0x3b)
                {
                    sInitParams.u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_EVEN;
                }
                else if (_u8AtrBuf[0] == 0x3f)
                {
                    sInitParams.u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_ODD;
                }
                else
                {
                     ok = FALSE;
                }

                // Parsing the protocol from ATR
                int i;

                if (_u8AtrBuf[1] & 0x80)
                {
                    MS_U8 u8T0 = _u8AtrBuf[1] >> 4;
                    i = 1;
                    while (u8T0)
                    {
                       if (u8T0 & 1)
                           i++;
                      u8T0 >>= 1;
                    }
                   u8Protocol = _u8AtrBuf[i] & 0xF;
                }
                else
                {
                    u8Protocol = 0;
                }
    
                if (MDrv_SC_Config(u8SCID, &sInitParams) != E_SC_OK)
                {
                    ok = FALSE;
    }
  /*  pInfo = MDrv_SC_GetInfo(u8SCID);
        printf("Get Info after MDrv_SC_Config    clkdiv=%d;\n",pInfo->u16ClkDiv); */
    i=0;
    if (_u8AtrBuf[1] & 0x10 ) /* TA1 present */
    {
        i=1;
        u8Fi = (_u8AtrBuf[2] >> 4) & 0x0f;
        u8Di =_u8AtrBuf[2] & 0x0f;
   /*     rc = MDrv_SC_SetPPS(u8SCID,u8Protocol,u8Di,u8Fi); */
    }
    if(_u8AtrBuf[1] & 0x40) /* TC1 present */
    {  
        if(_u8AtrBuf[1] & 0x20) /* TB1 present */
        {
            i++;
        }
        guardTime = _u8AtrBuf[2+i];
        if(guardTime != 0xFF)
        {
          /*  MDrv_SC_SetGuardTime(u8SCID, guardTime); */
        }
                }


                rc = MDrv_SC_PPS(u8SCID);
                if (rc!=E_SC_OK)
                {
                    ok = FALSE;
                }
            }/* GetATR ok */
            else
            {
                ok = FALSE;
            }
             msgType = eTDAL_SMC_CARD_RESET;
        }/*  Reset OK*/
         else if (rc == E_SC_CARDOUT)
         {
             msgType = eTDAL_SMC_RESET_NO_CARD;
         }
        }/* Cold Reset OK*/

        if (!ok)
        {
             msgType = eTDAL_SMC_RESET_ERROR;
            mSMC_DEBUG(("[SMC DBG] TDAL_SMC_Reset(slot %d,%s) ERROR: Reset failure (%s)\n",
                         slot,(isCold)?"COLD":"WARM",SMCi_StrError(rc)));
        }

        /* Asynchronous error or no card notification */
            msg.type = msgType;
            msg.slotID = slot;
            TKEL_Enqueue(TDAL_SMC_QueueId, (void *)&msg);
    mTBOX_RETURN(eTDAL_SMC_NO_ERROR);

}


/*===================================================================
   *
   *   TDAL_SMC_ATR
   *
   *   Parameters:
   *   slot   :   Specifies   the   smart   card   slot   identifier.
   *   ATRBuffer   :   pointer   to   a   buffer   to   fill   with   the   ATR.
   *   ATRBufferLength   :   pointer   to   the   length   available   for   this   buffer.
   *
   *   Description:
   *   This   function   fills   the   buffer   passed   in   parameter   with
   *   the   Answer   To   Reset.
   *
   *   Returns:   UINT8
   *   eTDAL_SMC_NO_ERROR   in   case   of   success.
   *
   *==================================================================*/
tTDAL_SMC_ErrorCode   TDAL_SMC_ATR(tTDAL_SMC_Slot   slot, uint8_t   *ATRBuffer, uint32_t   *ATRBufferLength)
{
    return (eTDAL_SMC_NO_ERROR);
}


/*===================================================================
 *
 * TDAL_SMC_State
 *
 * Parameters:
 *    slot : Specifies the smart card slot identifier.
 *    ca_type : the value that identifies the CA.
 *
 * Description: This function is called to known the state of
 *              the Smart Card Reader.
 *
 *
 * Returns: tTDAL_SMC_State
 *    eTDAL_SMC_ERROR if slot parameter is bad,
 *    eTDAL_SMC_EXTRACTED if no Smart Card is inserted or can't determine the state,
 *    eTDAL_SMC_INSERTED if one Card has been detected but ATR has failed,
 *    eTDAL_SMC_OPERATIONNAL if one Card has been detected and ATR is OK.
 *
 *==================================================================*/
tTDAL_SMC_State   TDAL_SMC_State(tTDAL_SMC_Slot   slot, tTDAL_SMC_CA_Type   ca_type)
{
    mTBOX_FCT_ENTER("TDAL_SMC_State");
    mTBOX_RETURN(TDAL_SMC_status[slot].statusCard);
}


/*===================================================================
 *
 * TDAL_SMC_GetClockFrequency
 *
 * Parameters:
 *    slot : Specifies the smart card slot identifier.
 *    freq : pointer to a buffer to fill with the Clock Frequency.
 *
 * Description:
 *    This function fills the buffer passed in parameter with the Clock Frequency.
 *
 * Returns: tTDAL_SMC_ErrorCode
 *    eTDAL_SMC_NO_ERROR in case of success.
 *
 *==================================================================*/
tTDAL_SMC_ErrorCode   TDAL_SMC_GetClockFrequency(tTDAL_SMC_Slot   slot, uint32_t*   pFreq)
{
    const SC_Info *pParams;
	
	*pFreq = 4500000;
    mTBOX_FCT_ENTER("TDAL_SMC_GetClockFrequency");
    pParams = MDrv_SC_GetInfo(u8SCID);
	if(pParams->eCardClk == E_SC_CLK_4P5M)
	{
		*pFreq = 4500000;
	}

    mTBOX_RETURN(eTDAL_SMC_NO_ERROR);
}

/*===================================================================
 *
 * TDAL_SMC_RawReadWrite
 *
 * Parameters:
 *    slot :   Specifies the smart card slot identifier.
 *    data:    Pointer to a 'P3' bytes buffer containing the data
 *              to send (if direction = TRUE).
 *    expectedLength: expected length of answer
 *    pNumberExchanged: Pointer to store number of bytes received.
 *    is_write: Set TRUE to write data in the smart card
 *              or FALSE to read data from it.
 *
 * Description:
 *    This function is called to read data from the card or to write
 *    data in the card, it returns when the request is recorded but
 *    not done.
 *    A callback function is called later by the driver to give the result of the command.
 *
 * Returns: tTDAL_SMC_ErrorCode
 *    eTDAL_SMC_NO_ERROR in case of success,
 *    eTDAL_SMC_ERROR otherwise.
 *
 *==================================================================*/

tTDAL_SMC_ErrorCode TDAL_SMC_RawReadWrite(tTDAL_SMC_Slot slot, uint8_t *data, uint8_t expectedLength,uint32_t *pNumberExchanged, bool is_write)
{
    SC_Result rc=E_SC_FAIL;
    tTDAL_SMC_msg    msg;
	uint8_t loops, i;

    mTBOX_FCT_ENTER("TDAL_SMC_RawReadWrite");

#if 1
    MS_U16 len = expectedLength;
    if (is_write)
    {

        TDAL_SMC_status[slot].RW_Request = eTDAL_SMC_WRITE;
        /* Write data to the smart card  */
         MsOS_DelayTask(300);	
        rc  = MDrv_SC_Send(u8SCID, data, len, SC_T1_SEND_TIMEOUT);
        *pNumberExchanged = len;
        mTBOX_TRACE((kTBOX_NIV_1,"rc:%d\n",rc));
    }
    else
    {
        TDAL_SMC_status[slot].RW_Request = eTDAL_SMC_READ;
        mTBOX_TRACE((kTBOX_NIV_1,">>> expectedLength %d\n", expectedLength));
        MsOS_DelayTask(300);	
        rc  = MDrv_SC_Recv(u8SCID, data, &len, 15000);
        memcpy(TDAL_SMC_status[slot].Answer,data,len);
        TDAL_SMC_status[slot].numRead = len;       
        mTBOX_TRACE((kTBOX_NIV_1,"rc:%d\n",rc));
    }
    if((rc == E_SC_OK) || (rc == E_SC_TIMEOUT))
    {
            msg.type = eTDAL_SMC_TRANSFER_COMPLETED;
            msg.slotID = slot;
            TKEL_Enqueue(TDAL_SMC_QueueId, (void *)&msg);
    }

#endif	
    mTBOX_RETURN( eTDAL_SMC_NO_ERROR);
}
/*===================================================================
 *
 * TDAL_SMC_ReadWrite
 *
 * Parameters:
 *    slot : Specifies the smart card slot identifier.
 *    command : Pointer to a 5 bytes buffer containing the
 *              incoming instruction to send to the Smart Card :
 *              CLA INS P1 P2 P3.
 *    data : Pointer to a 'P3' bytes buffer containing the data
 *           to send (if direction = TRUE).
 *    direction : select TRUE to write data in the smart card
 *                and FALSE to read data from it.
 *
 * Description:
 *    This function is called to read data from the card or to write
 *    data in the card, it returns when the request is recorded but
 *    not done.
 *    A callback function is called later by the driver to give the result of the command.
 *
 * Returns: tTDAL_SMC_ErrorCode
 *    eTDAL_SMC_NO_ERROR in case of success,
 *    eTDAL_SMC_ERROR otherwise.
 *
 *==================================================================*/
tTDAL_SMC_ErrorCode   TDAL_SMC_ReadWrite(tTDAL_SMC_Slot   slot, uint8_t   *command, uint8_t   *data, bool   direction)
{
return (eTDAL_SMC_NO_ERROR);
}


/*=============================================================================
 *
 * TDAL_SMC_PPS_Negociation
 *
 * Parameters:
 *    slot : Specifies the smart card slot identifier.
 *    PPS_Request :
 *
 * Description:
 *    This function is called to ask for a PPS negociation.
 *
 * Returns: tTDAL_SMC_ErrorCode
 *    eTDAL_SMC_NO_ERROR in case of success,
 *    eTDAL_SMC_ERROR otherwise.
 *
 *===========================================================================*/
tTDAL_SMC_ErrorCode   TDAL_SMC_PPS_Negociation(tTDAL_SMC_Slot   slot, uint8_t   PPS_Request[5])
{
return (eTDAL_SMC_NO_ERROR);
}


/*=============================================================================
 *
 * TDAL_SMC_CA_Autho_Register
 *
 * Parameters:
 *    Notif_fct : Pointer to a tTDAL_SMC_AccessType_Notif function.
 *
 * Description:
 *    This function is called to register a callback function that will be called
 *    when a card will be inserted or extracted.
 *
 * Returns: tTDAL_SMC_ErrorCode
 *    eTDAL_SMC_NO_ERROR in case of success,
 *    eTDAL_SMC_ERROR otherwise.
 *
 *===========================================================================*/
tTDAL_SMC_ErrorCode   TDAL_SMC_CA_Autho_Register(tTDAL_SMC_AccessType_Notif   Notif_fct)
{
return (eTDAL_SMC_NO_ERROR);
}


/*=============================================================================
 *
 * SMC_Task
 *
 * Parameters:
 *    arg : Null pointer.
 *
 * Description:
 *    This is SMC main Task.
 *
 * Returns: nothing.
 *
 *===========================================================================*/
LOCAL void SMC_Task(void *arg)
{
    tTDAL_SMC_msg    msg;
//    SC_Status        status;
//    SC_Param         params;
//    unsigned char    sw1,sw2;
    SC_Result     error = 0;

    mTBOX_FCT_ENTER("SMC_Task");

    while(1)
    {

      //  printf("MARIJA SMC TASK IS RUNNING \n");
        TKEL_Dequeue(TDAL_SMC_QueueId, (tTDAL_SMC_msg *)&msg);
	//	printf("MARIJA        receive message type %d\n",msg.type);

        if (&msg != NULL)
        {
            switch (msg.type)
            {
         case eTDAL_SMC_CARD_RESET :
            /* Card reset */
               {
               TDAL_SMC_status[msg.slotID].statusCard = eTDAL_SMC_OPERATIONAL;
               TDAL_SMC_status[msg.slotID].reset_Ok = TRUE;

               memcpy(TDAL_SMC_status[msg.slotID].ATR, _u8AtrBuf, (size_t)_u16AtrLen);
               TDAL_SMC_status[msg.slotID].ATR_len = _u16AtrLen;

               /* Reset succeeded */
               SMC_Callback(msg.slotID,eTDAL_SMC_CBF_RESET,
                            TDAL_SMC_status[msg.slotID].ATR,
                            TDAL_SMC_status[msg.slotID].ATR_len,
                            0,0); /* SW1, SW2 */
              
            }
            break;
			
            case eTDAL_SMC_CARD_INSERTED :
                TDAL_LockMutex(TdalSMCMutexID);
                TDAL_SMC_status[msg.slotID].statusCard = eTDAL_SMC_INSERTED;
                SMC_Callback(msg.slotID,eTDAL_SMC_CBF_INSERT,NULL,0,0,0);
				//printf("@@@@@@@@@@@@@@@@@@@@@@@@@@ SMC TASK  send CALLBACK eTDAL_SMC_CBF_INSERT @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
                TDAL_UnlockMutex(TdalSMCMutexID);
                break;

            case eTDAL_SMC_CARD_REMOVED :
                TDAL_SMC_status[msg.slotID].statusCard = eTDAL_SMC_EXTRACTED;
                TDAL_SMC_status[msg.slotID].reset_Ok   = FALSE;
                SMC_Callback(msg.slotID, eTDAL_SMC_CBF_EXTRACT,NULL,0,0,0);
                break;


                case eTDAL_SMC_TRANSFER_COMPLETED :
	    		{
			     error = msg.slotID	&  0x7;
				 msg.slotID = (msg.slotID & 0x8) >> 3;		   
				 

               switch(TDAL_SMC_status[msg.slotID].RW_Request)
               {
               case eTDAL_SMC_READ :
                  SMC_Callback(msg.slotID,eTDAL_SMC_CBF_READ,
                               TDAL_SMC_status[msg.slotID].Answer,
                               TDAL_SMC_status[msg.slotID].numRead,
                               0,error);
				  error = 0;
                  break;

               case eTDAL_SMC_WRITE :
                  SMC_Callback(msg.slotID,eTDAL_SMC_CBF_WRITE,
                               NULL,0,0,0);
                  break;

               case eTDAL_SMC_PPS :
                  SMC_Callback(msg.slotID,eTDAL_SMC_CBF_PPS,
                               TDAL_SMC_status[msg.slotID].Answer,
                               TDAL_SMC_status[msg.slotID].numRead,
                               0,0);
                  break;
    
               default :
                  /* Houlala !!! Programmation ERROR ? Ze coder is a clown !!!! */
                  mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                               "[SMC_Task] Card automate on slot %d in ze choux "
                               "(R/W request %d unsupported)\n",
                               msg.slotID,TDAL_SMC_status[msg.slotID].RW_Request));
                  mSMC_DEBUG(("[SMC DBG] Card automate on slot %d in ze choux "
                               "(R/W request %d unsupported)\n",
                               msg.slotID,TDAL_SMC_status[msg.slotID].RW_Request));
                  SMC_Callback(msg.slotID,eTDAL_SMC_CBF_TRANSFERT_ERROR,NULL,0,0,0);
               }
            }
            break;

            case eTDAL_SMC_RESET_NO_CARD:
                /* Asynchronous notification RESET return no card detected */
                SMC_Callback(msg.slotID, eTDAL_SMC_CBF_EXTRACT,NULL,0,0,0);
                break;

            case eTDAL_SMC_RESET_ERROR:
                /* Asynchronous notification RESET return ERROR */
                TDAL_SMC_status[msg.slotID].statusCard = eTDAL_SMC_INSERTED;
                TDAL_SMC_status[msg.slotID].reset_Ok = FALSE;
                SMC_Callback(msg.slotID, eTDAL_SMC_CBF_RESET,NULL,0,0,0);
                break;
       /*     case eTDAL_SMC_TRANSFER_ERROR:
              SMC_Callback(msg.slotID, eTDAL_SMC_CBF_TRANSFERT_ERROR,NULL,0,0,0);
                break;*/

            default :
                mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                            "ERROR : Request can't be identified 0x%x on Slot 0x%x\r\n", 
                             msg.type, msg.slotID));
            }
        }
    }

    mTBOX_RETURN;
}

/*=============================================================================
 *
 * SMCi_OpenSlot
 *
 * Parameters:
 *    slot : Slot ID (1 or 2).
 *
 * Description:
 *    Open a smartcard slot.
 *
 * Returns: eTDAL_SMC_NO_ERROR Open successfull.
 *          eTDAL_SMC_ERROR    Open failed
 *
 *===========================================================================*/
LOCAL tTDAL_SMC_ErrorCode SMCi_OpenSlot (tTDAL_SMC_Slot slot)
{

    SC_Result       rc;

    if (TDAL_SMC_status[slot].is_open)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL,
                    "SMCi_OpenSlot(%d) ERROR: Slot already open\n",slot));
        return(eTDAL_SMC_ERROR);
    }

    TDAL_SMC_status[slot].statusCard = eTDAL_SMC_EXTRACTED;
    TDAL_SMC_status[slot].reset_Ok   = FALSE;
    TDAL_SMC_status[slot].ATR_len    = 0;
    TDAL_SMC_status[slot].lenAnswer  = 0;
    TDAL_SMC_status[slot].lenAPDU    = 0;
    TDAL_SMC_status[slot].numWrite   = 0;
    TDAL_SMC_status[slot].numRead    = 0;

    sInitParams.eCardClk = E_SC_CLK_4P5M;
    sInitParams.u8UartMode = SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_NO;
    sInitParams.u16ClkDiv = 372;
    sInitParams.eVccCtrl	= E_SC_VCC_CTRL_8024_ON;
    sInitParams.pfOCPControl = NULL;
    sInitParams.u8Convention = 1;

    mTBOX_TRACE((kTBOX_NIV_1, "SMCi_OpenSlot(%d): Calling STSMART_Open()...\n", u8SCID));
    if((rc = MDrv_SC_Open(u8SCID, u8Protocol, &sInitParams, Notify_Slot_0)) != E_SC_OK)
    {
        mTBOX_TRACE((kTBOX_NIV_1, "[%s] MDrv_SC_Open failed with error code = %d!!!!\n", __FUNCTION__, rc));
        return(eTDAL_SMC_ERROR);
    }
//     printf("MARIJA      slot %d is opened\n",slot);
    TDAL_SMC_status[slot].is_open = TRUE;

    mTBOX_TRACE((kTBOX_NIV_1, "SMCi_OpenSlot(%d): STSMART_Open() DONE \n", u8SCID));
    mSMC_DEBUG(("[SMC DBG] SMCi_OpenSlot(%d): STSMART Open DONE on handle \n", u8SCID));
    return(eTDAL_SMC_NO_ERROR);
}

/*=============================================================================
 *
 * SMC_Callback
 *
 * Parameters:
 *
 * Description:
 *
 * Returns: nothing.
 *
 *===========================================================================*/
LOCAL void SMC_Callback(tTDAL_SMC_Slot  slot, tTDAL_SMC_Event Event, uint8_t* Buffer, uint32_t BufferLength, uint8_t SW1, uint8_t  SW2)
{
    unsigned char i;
    for(i=0; i<kTDAL_SMC_MAX_CLIENTS; i++)
    {
        if(TDAL_SMC_Clients[i].callback != NULL)
        {
            TDAL_SMC_Clients[i].callback(slot, Event, Buffer, BufferLength, SW1, SW2);
        }
    }
}

/*=============================================================================
 *
 * SMCi_CloseSlot
 *
 * Parameters:
 *    slot : Slot ID (1 or 2).
 *
 * Description:
 *    Close a smartcard slot.
 *
 * Returns: eTDAL_SMC_NO_ERROR Close successfull.
 *          eTDAL_SMC_ERROR    Close failed
 *
 *===========================================================================*/
LOCAL tTDAL_SMC_ErrorCode SMCi_CloseSlot (tTDAL_SMC_Slot slot)
{
    SC_Result       rc;

    if (!TDAL_SMC_status[slot].is_open)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "SMCi_OpenSlot(%d) ERROR: Slot not open\n",u8SCID));
        return(eTDAL_SMC_ERROR);
    }

    mTBOX_TRACE((kTBOX_NIV_1, "SMCi_CloseSlot(%d): Calling STSMART_Close...\n", u8SCID));

    if ((rc = MDrv_SC_Close(u8SCID)) != E_SC_OK)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "SMCi_CloseSlot(%d) ERROR: STSMART_Close() failed \n",u8SCID));
        mSMC_DEBUG(("[SMC DBG] SMCi_CloseSlot(%d) ERROR: STSMART Close failure \n", u8SCID));
        return(eTDAL_SMC_ERROR);
    }

    mTBOX_TRACE((kTBOX_NIV_1, "SMCi_CloseSlot(%d): STSMART_Close() DONE\n", u8SCID));
    mSMC_DEBUG(("[SMC DBG] SMCi_CloseSlot(%d): STSMART Close handle DONE\n", u8SCID));

    TDAL_SMC_status[slot].is_open      = FALSE;
    TDAL_SMC_status[slot].statusCard   = eTDAL_SMC_EXTRACTED;
    TDAL_SMC_status[slot].reset_Ok     = FALSE;
    TDAL_SMC_status[slot].ATR_len      = 0;
    TDAL_SMC_status[slot].lenAnswer = 0;
    TDAL_SMC_status[slot].lenAPDU   = 0;

    return(eTDAL_SMC_NO_ERROR);
}

/*=============================================================================
 *
 * Notify_Slot_0
 *
 * Parameters:
 *
 * Description:
 *
 * Returns: nothing.
 *
 *===========================================================================*/
LOCAL void Notify_Slot_0(MS_U8 u8SCID, SC_Event eEvent)
{
    tTDAL_SMC_msg  msg;
    MS_U32 u32Mask, u32Evt;
    mTBOX_FCT_ENTER("Notify_Slot_0");

    msg.slotID = u8SCID;

    switch(eEvent)
    {
        case E_SC_EVENT_IN :
            mTBOX_TRACE((kTBOX_NIV_1,"SMC%d: Card In\n", u8SCID));
            msg.type = eTDAL_SMC_CARD_INSERTED;
//            printf("\n\n\n^^^^^^^^^^^^^^^^^^^^^^^^ Notify_Slot_0    %d^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n\n",msg.type);
            break;

        case E_SC_EVENT_OUT :
            mTBOX_TRACE((kTBOX_NIV_1,"SMC%d: Card Out\n", u8SCID));
            msg.type = eTDAL_SMC_CARD_REMOVED;
            break;

        case E_SC_EVENT_DATA :
            mTBOX_TRACE((kTBOX_NIV_1,"SMC%d: Incoming Data\n", u8SCID));
            msg.type = eTDAL_SMC_TRANSFER_COMPLETED;
            break;

        default :
            mTBOX_TRACE((kTBOX_NIV_CRITICAL,"SMC%d: Unknown Event\n", u8SCID));
            mTBOX_RETURN;
            break;
    }

    TKEL_Enqueue(TDAL_SMC_QueueId, (void *)&msg);

    mTBOX_RETURN;
}


#if defined (__LOCAL_SMC_DEBUG__)

LOCAL char *SMCi_StrError (SC_Result errCode)
{
    static char __str_error__[50];

    sprintf(__str_error__,"error %d=%08X:",errCode,errCode);
    switch (errCode)
    {
    case E_SC_FAIL:                        strcat(__str_error__,"Function fail");                 break;
    case E_SC_OK:                          strcat(__str_error__,"No Error");        break;
    case E_SC_PPSFAIL:                     strcat(__str_error__,"Do PPS fail");            break;
    case E_SC_INVCONV:                     strcat(__str_error__,"Inverse convention");       break;
    case E_SC_CARDIN:                      strcat(__str_error__,"Card in");  break;
    case E_SC_CARDOUT:                     strcat(__str_error__,"Card out");      break;
    case E_SC_NODATA:                      strcat(__str_error__,"No data");          break;
    case E_SC_TIMEOUT:                     strcat(__str_error__,"Timeout");       break;
    case E_SC_OVERFLOW:                    strcat(__str_error__,"Rx data fifo overflow");break;
    case E_SC_CRCERROR:                    strcat(__str_error__,"ATR checksum error");    break;
    case E_SC_DATAERROR:                   strcat(__str_error__,"Data error");   break;
    case E_SC_PARMERROR:                   strcat(__str_error__,"Parameter error");              break;
    default:                               strcat(__str_error__,"Not referenced");
    }

    return __str_error__;
}
#endif


LOCAL void SMCi_GetInfo ()
{
	/*	MARIJA DEBUG */
	    const SC_Info *pInfo;
		pInfo = MDrv_SC_GetInfo(u8SCID);
	
		printf("				   MARIJA\n SmsartCard Protocol:\n"
				"u8Protocol  %d               "
				"bSpecMode  %d               "
				"pu8Atr[SC_ATR_LEN_MAX]  %s               "
				"u16AtrLen  %d               \n"
				"pu8Hist[SC_HIST_LEN_MAX]  %s               "
				"u16HistLen  %d               "
				"u8Fi  %d                "
				"u8Di  %d               \n\n"
				"Device Setting:\n"
				"bInited  %d               "
				"bOpened  %d               "
				"bCardIn  %d               "
				"blast_cardin  %d               \n"
				"eCardClk  %d               "
				"u8UartMode  %d               "
				"eVccCtrl  %d               "
				"u16ClkDiv %d               \n"
				"u16Bonding %d               "
				"pfEn5V %d               "
				"u8Convention %d               \n\n"
				"bLastCardIn  %d               "
				"s32DevFd %d\n",pInfo->u8Protocol, pInfo->bSpecMode, pInfo->pu8Atr, pInfo->u16AtrLen, pInfo->pu8Hist, pInfo->u16HistLen, pInfo->u8Fi, pInfo->u8Di, pInfo->bInited, pInfo->bOpened, pInfo->bCardIn, pInfo->blast_cardin,
							   pInfo->eCardClk, pInfo->u8UartMode, pInfo->eVccCtrl, pInfo->u16ClkDiv, pInfo->u16Bonding, pInfo->pfEn5V, pInfo->u8Convention, pInfo->bLastCardIn, pInfo->s32DevFd);
	
	/*	if (MDrv_SC_Activate(u8SCID) != E_SC_OK)
			{
				printf("	   MARIJA	   ACTIVATE    FAILUREEE  \n");
			}
		else
						printf("	   MARIJA	   ACTIVATE    SUCCEEDED  \n");*/
	/* marija debug */


}
LOCAL void SMC_PrintData(MS_U8 *pu8Data, MS_U16 u16DataLen)
{
    int i;

    printf("Rx :\n         ");
    for (i = 0; i < u16DataLen; i++)
    {
        printf("%02x ", pu8Data[i]);
        if (((i+1) % 16) == 0)
        {
            printf("\n         ");
        }
    }
    printf("\n");
}

tTDAL_SMC_ErrorCode TDAL_T1RawExchange(uint32_t u32SendLen, const uint8_t *pu8SendBlock, uint32_t u32ReplyMaxLen,uint32_t *pu32ReplyLen, uint8_t *pu8ReplyBlock)
{

    SC_Result       rc;
	    tTDAL_SMC_msg    msg;

#if 1
     *pu32ReplyLen = u32ReplyMaxLen;
     rc =  MDrv_SC_RawExchange(u8SCID, pu8SendBlock, &u32SendLen, pu8ReplyBlock, pu32ReplyLen);
      // printf("DONE  MDrv_SC_RawExchange   *pu32ReplyLen   %d               returns %d\n",*pu32ReplyLen,rc);

     TDAL_SMC_status[0].RW_Request = eTDAL_SMC_READ;
	 memcpy(TDAL_SMC_status[0].Answer,pu8ReplyBlock,*pu32ReplyLen);
         TDAL_SMC_status[0].numRead = *pu32ReplyLen; 
	 if((rc == E_SC_OK) || (rc == E_SC_TIMEOUT) || (rc == E_SC_NODATA))
    {
            msg.type = eTDAL_SMC_TRANSFER_COMPLETED;
            msg.slotID = 0;
			if((rc == E_SC_TIMEOUT) || (rc == E_SC_NODATA))
			{
			    /* since only one slot has been supported, instead of slot id status error will be provided */
                msg.slotID = rc;			
			}
            TKEL_Enqueue(TDAL_SMC_QueueId, (void *)&msg);
    }
#endif
	return eTDAL_SMC_NO_ERROR;

#if 0
      MsOS_DelayTask(300);
    //send data
    rc = MDrv_SC_Send(u8SCID, pu8SendBlock, u32SendLen, SC_T1_SEND_TIMEOUT);
    if (rc != E_SC_OK)
    {
       printf("send data error returns %d\n",rc);
        return eTDAL_SMC_ERROR;
    }
    //block waiting time
    MsOS_DelayTask(300);

    //receive data
    *pu32ReplyLen = u32ReplyMaxLen;
    rc = MDrv_SC_Recv(u8SCID, pu8ReplyBlock, pu32ReplyLen, SC_T1_RECV_TIMEOUT);
    
    if (rc != E_SC_OK)
    {
               printf("receive data error, return %d\n",rc);
        return eTDAL_SMC_ERROR;
    }
     TDAL_SMC_status[0].RW_Request = eTDAL_SMC_READ;
	 memcpy(TDAL_SMC_status[0].Answer,pu8ReplyBlock,*pu32ReplyLen);
         TDAL_SMC_status[0].numRead = *pu32ReplyLen; 
	 msg.type = eTDAL_SMC_TRANSFER_COMPLETED;    
    
	        msg.slotID = 0;
            TKEL_Enqueue(TDAL_SMC_QueueId, (void *)&msg);
	return eTDAL_SMC_NO_ERROR;
#endif
}

#if 0
tTDAL_SMC_ErrorCode TDAL_SC_T1_Parse_Atr(MS_U8 u8SCID, MS_U8 *pu8AtrData, MS_U16 u16AtrLen)
{

 MS_U8				u8TS, u8T0, u8TAx, u8TBx, u8TCx, u8TDx, u8Len, u8TA2;
 MS_U8				u8Ch, u8Crc = 0;
 MS_U8				iu8Protocol;

 int 				ii, x;
 ii = 0;
 u8TS = pu8AtrData[ii++];
 u8T0 = pu8AtrData[ii++];

 u8Crc = u8Crc ^ u8T0;
 u8Len = u8T0 & 0x0F;
 u8TDx = u8T0 & 0xF0;

x = 1;
//_scInfo[u8SCID].bSpecMode = FALSE;
//_scInfo[u8SCID].u8Di = _scInfo[u8SCID].u8Fi = 0;
 iu8Protocol = 0;
 while (u8TDx & 0xF0)
 {
	if (ii >= u16AtrLen)
	{
		return eTDAL_SMC_ERROR;
	}

	if (u8TDx & 0x10) // TA
	{
		u8TAx = pu8AtrData[ii++];
		u8Crc = u8Crc ^ u8TAx;
		if (x == 1)
		{
			//_scInfo[u8SCID].u8Fi = u8TAx >> 4;
			//_scInfo[u8SCID].u8Di = u8TAx & 0xF;
		}
		if (x == 2)
		{
			//_scInfo[u8SCID].bSpecMode = TRUE; // specific mode
			u8TA2 = u8TAx;
		}
		if (x == 3)
		{
	// TA3 is for encoding T1 IFSC
	if ( (u8TAx != 0x00) && (u8TAx != 0xFF) )
		{
		T1State[u8SCID].u8IFSC = u8TAx;
	}
		}
	}
	if (u8TDx & 0x20) // TB
	{
		u8TBx = pu8AtrData[ii++];
		u8Crc = u8Crc ^ u8TBx;
	// TB3 is for encoding T1 BWI and CWI

	}
	if (u8TDx & 0x40) // TC
	{
		u8TCx = pu8AtrData[ii++];
		u8Crc = u8Crc ^ u8TCx;
		if (x == 3)
		{
	// TC3 is for encoding T1 CRC type
			T1State[u8SCID].u8RCType = u8TCx & 0x1;
		}
	}
	if ((u8TDx & 0x80))
	{
		u8TDx = pu8AtrData[ii++];
		if ((u8TDx & 0xF) != 0)
		{
			iu8Protocol = u8TDx & 0xF;
		}
		u8Crc = u8Crc ^ u8TDx;

		
		x++;
	}
	else
	{
		break;
	}
}

// Get ATR history
//_scInfo[u8SCID].u16HistLen = u8Len;
for (x = 0; x < u8Len; x++)
{
	if (ii >= u16AtrLen)
	{
		return eTDAL_SMC_ERROR;
	}
	u8Ch = pu8AtrData[ii++];
	u8Crc = u8Crc ^ u8Ch;

	//_scInfo[u8SCID].pu8Hist[x] = u8Ch;
}

// Check ATR checksum
if ((iu8Protocol != 0x00) && (iu8Protocol != 0xFF)) // If not T=0
{
	if (ii >= u16AtrLen)
	{
		return eTDAL_SMC_ERROR;
	}
	if (u8Crc != pu8AtrData[ii++])
	{
	// CRC error
		return eTDAL_SMC_ERROR;
	}
}

// Check ATR length
 if (ii != u16AtrLen)
{
	return eTDAL_SMC_ERROR; // len error
}

 T1State[u8SCID].bInit = TRUE;


 return eTDAL_SMC_NO_ERROR;
}
#endif

#ifdef SC_USE_IO_51
MS_BOOL SMC_Load_Fw(MS_U32 u32FwSCAddr)
{
    MBX_Result mbxResult;
    SC_BuffAddr ScBuffAddr;

    ScBuffAddr.u32DataBuffAddr = PM51_MEM_ADR;
    ScBuffAddr.u32FwBuffAddr   = u32FwSCAddr;
    
    MDrv_SC_SetBuffAddr(&ScBuffAddr);

    printf("load fw to u32FwSCAddr %lx\n", u32FwSCAddr);
    memcpy((void*)MS_PA2KSEG1((MS_U32)u32FwSCAddr/*u8SCFirmware*/) , u8SCFirmware, sizeof(u8SCFirmware));
    MDrv_PM_SetDRAMOffsetForMCU(u32FwSCAddr >> 16); //64K unit

    MDrv_MBX_SetDbgLevel(MBX_DBG_LEVEL_NONE);
    mbxResult = MDrv_MBX_Init(E_MBX_CPU_MIPS, E_MBX_ROLE_HK, 1500);
    if(E_MBX_SUCCESS != mbxResult)
    {
        printf("MBX init error\n");
        return FALSE;
    }
    MDrv_MBX_Enable(TRUE);    

    printf("E_MBX_CLASS_PM_WAIT - MBX register msg\n");    
    mbxResult = MDrv_MBX_RegisterMSG(E_MBX_CLASS_PM_WAIT, 6);
    if(E_MBX_SUCCESS != mbxResult)
    {
        printf("E_MBX_CLASS_PM_WAIT - MBX register msg error\n");
        return FALSE;
    }
    else
    {
        printf("E_MBX_CLASS_PM_WAIT - MBX register msg success\n");
        return TRUE;
    }
}
#endif