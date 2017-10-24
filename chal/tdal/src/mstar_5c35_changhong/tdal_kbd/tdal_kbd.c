/******************************************************************************
 *               COPYRIGHT 2004 IWEDIA TECHNOLOGIES               *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_KBD
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_kbd/tdal_kbd.c $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * DESCRIPTION : TDAL KBD implementation
 *
 *****************************************************************************/

/********************************************************/
/*        Includes                   */
/********************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "crules.h"
#include "tbox.h"
#include "tdal_common.h"
#include "tdal_common_priv.h"
#include "tdal_kbd.h"

#include "MsCommon.h"
#include "drvMMIO.h"
#include "mdrv_ir.h"
#include "MsOS.h"
#include "IR_MSTAR_DTV.h"
#define KBD_GTPL 
/********************************************************/
/*           Defines                        */
/********************************************************/
#define TDAL_KBD_TASK_STACK         (2*4096)

#define HELD_DOWN_INTERVAL (300)

#define KEY_RELEASED_TIMEOUT ((IR_TIMEOUT_CYC/1000)+20)


#ifdef KBD_NEWLAND
#define KEY_EXIT            0x16
#endif

#ifdef KBD_MSTAR
#define KEY_EXIT            0x1b
#endif

#ifdef KBD_GTPL
#define KEY_EXIT            0xC5
#endif

//#define _DEBUG_AUTO_ZAPPING_
#ifdef _DEBUG_AUTO_ZAPPING_
#define _DEBUG_AUTO_ZAPPING_START_KEY    0x84   /* green key */
#define _DEBUG_AUTO_ZAPPING_PROG_PLUS   0x85
#define _DEBUG_AUTO_ZAPPING_DELAY       1       /* seconds */
#endif /* _DEBUG_AUTO_ZAPPING_ */

LOCAL unsigned char     TDAL_KBD_EvtTaskStack[TDAL_KBD_TASK_STACK];

#ifdef _DEBUG_AUTO_ZAPPING_
MS_S32  *gTDAL_kbdpAutoZapTaskId = NULL;
#endif

/********************************************************/
/*           Macros                        */
/********************************************************/
mTBOX_SET_MODULE(eTDAL_KBD);

/********************************************************/
/*           Typedefs                      */
/********************************************************/

/********************************************************/
/*           Global Variables (GLOBAL/IMPORT)       */
/********************************************************/
/********************************************************/
/*           Local Module Variables (MODULE)      */
/********************************************************/
uint16_t      gTDAL_KeyAddress;

typedef struct _RCKey
{
	uint32_t keyCode;
	uint8_t repeatFleg;
	bool shouldExit;
} tdal_kbd_RCKey;

/********************************************************/
/*           Local   File Variables (LOCAL)      */
/********************************************************/
LOCAL   bool            gTDAL_kbdInitialized   = FALSE;
LOCAL   uint8_t        gTDAL_kbdNbClientRegistered = 0;
LOCAL   tTDAL_KBD_CallbackFct   gTDAL_kbdClientFctArray[kTDAL_KBD_NB_MAX_CLIENTS];
LOCAL TDAL_task_id _s32TaskId = NULL;
LOCAL void *_pTaskStack = NULL;
LOCAL volatile bool TDAL_KBDKeyboardProcess_Finished = FALSE;
LOCAL TDAL_queue_id tdalKbdQueueID = -1;
LOCAL tdal_kbd_RCKey rckey;

#ifdef _DEBUG_AUTO_ZAPPING_
LOCAL bool gAutoZap = FALSE;
#endif

//LOCAL unsigned   short   NbKeys=64;
//LOCAL tTDAL_KBD_KeyCode   KeyCode[64];

#ifdef KBD_MSTAR
#define KBD_MASK 0x7f80
#endif

#ifdef KBD_NEWLAND
#define KBD_MASK 0xfe02
#endif

#ifdef KBD_GTPL
#define KBD_MASK 0xFD01
#endif

/********************************************************/
/*           Local Functions Declarations (LOCAL)   */
/********************************************************/
LOCAL void   TDAL_KBD_CallClientsCallbackFct(   tTDAL_KBD_KeyCode   KeyCode,
                        tTDAL_KBD_KeyStatus   KeyStatus);
LOCAL void TDAL_KBDKeyboardProcess(void *argv);

#ifdef   _DEBUG_AUTO_ZAPPING_
LOCAL   void   TDAL_KBD_AutoZapTask(void*   unused);
LOCAL   void   TDAL_KBD_StopAutoZap(void);
#endif

/********************************************************/
/*           Functions Definitions (LOCAL)       */
/********************************************************/
LOCAL void _Input_IRCallback(MS_U32 u32IRKey, MS_U8 u8RepeatFlag)
{
	tdal_kbd_RCKey rckey_local;
	MS_BOOL status;

	if ((u32IRKey & 0xFFFF) != KBD_MASK)
    {
	    mTBOX_TRACE((kTBOX_NIV_3, "Unsupported key 0x%x\n", u32IRKey));
	    return;
    }

	rckey_local.keyCode = (u32IRKey >> 16) & 0xFF;
	rckey_local.repeatFleg = u8RepeatFlag;
	rckey_local.shouldExit = false;

	//mTBOX_TRACE((kTBOX_NIV_CRITICAL, "%04d %s u8Key = 0x%x, u8RepeatFlag = 0x%x ------- from IRCallBack\n", __LINE__, __FUNCTION__, u32IRKey, u8RepeatFlag));
    #if 1 // can't use mutex in isr
    status = MsOS_SendToQueue (tdalKbdQueueID, (MS_U8 *)&rckey_local, sizeof(tdal_kbd_RCKey), 0);
    #else
	status = TDAL_Enqueue(tdalKbdQueueID, &rckey_local);
    #endif
	if (status == FALSE)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MsOS_SendToQueue   failed\n"));
	}
}

LOCAL void TDAL_KBDKeyboardProcess(void *argv)
{
    int32_t   KeyRead = -1;
    int32_t   PrevKey = -1;
    int32_t   PrevRepFleg = FALSE;
    //uint32_t  lastHeldDown = 0;
    bool   nothingPressed = TRUE;
    mTBOX_FCT_ENTER("TDAL_KBDKeyboardProcess");

    tdal_kbd_RCKey receivedKey;
    receivedKey.keyCode=-1;
    receivedKey.repeatFleg=FALSE;
    MS_U32 actualSize;
    MS_BOOL bRet;

    while (1)
    {
        if   (nothingPressed == TRUE)
        {
            //mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Tdal_kbd_Queue - waiting forever (wait for RC key)\n"));
            #if 1 // can't use mutex in isr
            bRet = MsOS_RecvFromQueue(tdalKbdQueueID, (MS_U8 *)&receivedKey, sizeof(tdal_kbd_RCKey), &actualSize, MSOS_WAIT_FOREVER);
            #else
            bRet = TDAL_Dequeue(tdalKbdQueueID, &receivedKey);
            #endif
            if (FALSE == bRet)
            {
                mTBOX_TRACE((kTBOX_NIV_WARNING, "Tdal_kbd_Queue    failed\n"));
            }
            else
            {
                mTBOX_TRACE((kTBOX_NIV_1, "KBDProcess   receives from queue  u8Key = 0x%02x, u8RepeatFlag = 0x%02x\n", receivedKey.keyCode, receivedKey.repeatFleg));
            }
            nothingPressed = FALSE;
        }
        else
        {
            #if 1 // can't use mutex in isr
            bRet = MsOS_RecvFromQueue(tdalKbdQueueID, (MS_U8 *)&receivedKey, sizeof(tdal_kbd_RCKey), &actualSize, KEY_RELEASED_TIMEOUT);
            #else
            bRet = TDAL_Dequeue(tdalKbdQueueID, &receivedKey);
            #endif
            if (FALSE == bRet)
            {
                mTBOX_TRACE((kTBOX_NIV_1, "Tdal_kbd_Queue empty - timeout;   failed\n"));
                nothingPressed = TRUE;
            }
            else
            {
                mTBOX_TRACE((kTBOX_NIV_1, "KBDProcess   receives from queue  u8Key = 0x%02x, u8RepeatFlag = 0x%02x\n", receivedKey.keyCode, receivedKey.repeatFleg));
            }
        }

        if ((bRet == TRUE) && (receivedKey.shouldExit == true))
        {
            mTBOX_TRACE((kTBOX_NIV_1, "KBDProcess  should exit = TRUE\n", receivedKey.keyCode, receivedKey.repeatFleg));
            break;
        }

        if   (nothingPressed == TRUE)
        {
            if   (PrevKey   !=   -1)
            {
                mTBOX_TRACE((kTBOX_NIV_1, "eTDAL_KBD_KEY_RELEASED\n"));
            //  lastHeldDown = 0;
                TDAL_KBD_CallClientsCallbackFct((PrevKey), eTDAL_KBD_KEY_RELEASED);
            }

            KeyRead = -1;
            PrevKey = -1;
        }
        else
        {
            rckey.keyCode = receivedKey.keyCode;
            rckey.repeatFleg = receivedKey.repeatFleg;

            KeyRead = rckey.keyCode;

                //if   (KeyRead == PrevKey)
                if   (rckey.repeatFleg==TRUE)
                {
                   // if ((MsOS_GetSystemTime() - lastHeldDown) > HELD_DOWN_INTERVAL)
                    {
                        /*   the   same   key   has   been   pressed   */
                        mTBOX_TRACE((kTBOX_NIV_1, "eTDAL_KBD_KEY_HELD_DOWN\n"));
                        TDAL_KBD_CallClientsCallbackFct(KeyRead, eTDAL_KBD_KEY_HELD_DOWN);
                       // lastHeldDown = MsOS_GetSystemTime();
                    }
                }
                else
                {

#ifdef _DEBUG_AUTO_ZAPPING_
                  if(KeyRead==_DEBUG_AUTO_ZAPPING_START_KEY || KeyRead==(_DEBUG_AUTO_ZAPPING_START_KEY))
                  {
                    gAutoZap = !gAutoZap;
                    if(!gAutoZap)
                    {
                        TDAL_KBD_StopAutoZap();
                    }
                    else
                    {
                        gTDAL_kbdpAutoZapTaskId = MsOS_CreateTask(TDAL_KBD_AutoZapTask,
                                        NULL,
                                        E_TASK_PRI_HIGH,
                                        TRUE,
                                        TDAL_KBD_EvtTaskStack,
                                        TDAL_KBD_TASK_STACK,
                                        "KeyboardAutoZappingTask");
                       mTBOX_TRACE((kTBOX_NIV_CRITICAL, "starting zap stress test\n" ));
                    }
                }
                else
#endif /* _DEBUG_AUTO_ZAPPING_ */
                {
                    /*   a   new   key   has   been   pressed   */
                    if   (PrevKey   !=   -1)
                    {
                        mTBOX_TRACE((kTBOX_NIV_1, "eTDAL_KBD_KEY_RELEASED\n"));
                        TDAL_KBD_CallClientsCallbackFct(PrevKey, eTDAL_KBD_KEY_RELEASED);
                    }

                    mTBOX_TRACE((kTBOX_NIV_1, "eTDAL_KBD_KEY_PRESSED\n"));
                    TDAL_KBD_CallClientsCallbackFct(KeyRead, eTDAL_KBD_KEY_PRESSED);
                    //lastHeldDown = MsOS_GetSystemTime();
                    }
                }

                PrevKey = KeyRead;
                PrevRepFleg = rckey.repeatFleg;
        }
    }

    TDAL_KBDKeyboardProcess_Finished = true;

    mTBOX_RETURN;
}


/********************************************************/
/*           Functions Definitions (GLOBAL)       */
/********************************************************/

/******************************************************************************
 * Function Name   : TDAL_KBD_Init
 * Description      : Initialize of the Keyboard module
 * Side effects    :
 * Comment       :
 * Inputs        :
 * Outputs       :
 *****************************************************************************/
tTDAL_KBD_ErrorCode   TDAL_KBD_Init()
{
	uint8_t   i;

	mTBOX_FCT_ENTER("TDAL_KBD_Init");

	/*   Test   if   TDAL_KBD   has   already   been   initialised   */
	if   (gTDAL_kbdInitialized == TRUE)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "initialization   error   :   already   initialized\n"));
		mTBOX_RETURN(eTDAL_KBD_NO_ERROR);
	}

	_pTaskStack = TDAL_Malloc(TDAL_KBD_TASK_STACK);
	mTBOX_ASSERT(_pTaskStack != NULL);
#if 1
	tdalKbdQueueID = MsOS_CreateQueue(NULL,
			NULL,
			E_MSG_FIXED_SIZE,
			sizeof(tdal_kbd_RCKey),
			E_MSOS_FIFO,
			"Tdal Kbd Queue");
#else
	TDAL_CreateQueue(TDAL_QUEUE_MSG_MAX_NO, sizeof(tdal_kbd_RCKey), &tdalKbdQueueID);
#endif
	if (tdalKbdQueueID < 0)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MsOS_CreateQueue FAIL\n"));
	}

	_s32TaskId = TDAL_CreateTask(
			eTDAL_PRIORITY_NORMAL,
			"Tdal Kbd Task",
			_pTaskStack,
			TDAL_KBD_TASK_STACK,
			TDAL_KBDKeyboardProcess,
			NULL);
	if (_s32TaskId == NULL)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Creating task failed\n"));
	}
	mTBOX_TRACE((kTBOX_NIV_CRITICAL, "==============zg test keyboard init start\n"));

	MDrv_IR_HK_Init();
	MDrv_IR_HK_SetCallback(_Input_IRCallback);

	/*   reset   counter   of   registered   clients   */
	gTDAL_kbdNbClientRegistered = 0;

	/*   init   of   array   of   CallbackFct   clients   */
	for (i = 0; i < kTDAL_KBD_NB_MAX_CLIENTS; i++)
	{
		gTDAL_kbdClientFctArray[i] = NULL;
	}

	gTDAL_kbdInitialized = TRUE;

	mTBOX_RETURN(eTDAL_KBD_NO_ERROR);
}

/******************************************************************************
 * Function Name      : TDAL_KBD_Term
 * Description       : De-Initialize of the Keyboard module
 * Side effects       :
 * Comment        :
 * Inputs          :
 * Outputs        :
 *****************************************************************************/
tTDAL_KBD_ErrorCode   TDAL_KBD_Term()
{
	bool   bError = TRUE;
	MS_BOOL bRet = FALSE;
	tdal_kbd_RCKey rckey_local;

	mTBOX_FCT_ENTER("TDAL_KBD_Term");

	if   (gTDAL_kbdInitialized == FALSE)
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "termination   error   :   already   terminated\n"));
		mTBOX_RETURN(eTDAL_KBD_NO_ERROR);
	}

	/*   wait   for   task   exit   */

	MDrv_IR_HK_Enable(FALSE);

	rckey_local.shouldExit = true;
	TDAL_KBDKeyboardProcess_Finished = false;

    #if 1 // can't use mutex in isr
	bRet = MsOS_SendToQueue(tdalKbdQueueID, (MS_U8 *) &rckey_local, sizeof(rckey_local), MSOS_WAIT_FOREVER);
    #else
	bRet = TDAL_Enqueue(tdalKbdQueueID, &rckey_local);
    #endif
	mTBOX_ASSERT(bRet == TRUE);

	while (TDAL_KBDKeyboardProcess_Finished == false)
	{
		mTBOX_TRACE((kTBOX_NIV_1, "Waiting keyboard thread to terminate\n"));
		MsOS_DelayTask(100);
	}

	MsOS_DelayTask(100);

    #if 1 // can't use mutex in isr
	bRet = MsOS_DeleteQueue (tdalKbdQueueID);
    #else
	bRet = TDAL_DeleteQueue(tdalKbdQueueID);
    #endif
	if   (bRet == FALSE)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_KBD_Term   error   :   MsOS_DeleteQueue   Failed\n"));
		bError = FALSE;
	}
	tdalKbdQueueID = -1;

	TDAL_DeleteTask(_s32TaskId);
	_s32TaskId = NULL;

	gTDAL_kbdInitialized = FALSE;

	if   (bError == FALSE)
	{
		mTBOX_RETURN(eTDAL_KBD_ERROR);
	}

	mTBOX_RETURN(eTDAL_KBD_NO_ERROR);
}

/******************************************************************************
 * Function Name      : TDAL_KBD_Register
 * Description       : This function registers a new client
 * Side effects       :
 * Comment        :
 * Inputs          :
 * Outputs        :
 *****************************************************************************/
tTDAL_KBD_ClientId   TDAL_KBD_Register(tTDAL_KBD_CallbackFct   callback)
{
	uint8_t   i;
	mTBOX_FCT_ENTER("TDAL_KBD_Register");

	if   ((gTDAL_kbdInitialized == FALSE)   ||
			(gTDAL_kbdNbClientRegistered == kTDAL_KBD_NB_MAX_CLIENTS)   ||
			(callback == NULL))
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "TDAL_KBD_Register   error\n"));
		mTBOX_RETURN(kTDAL_KBD_NO_ID);
	}

	for   (i = 0   ; i < kTDAL_KBD_NB_MAX_CLIENTS   ; i++)
	{
		if   (gTDAL_kbdClientFctArray[i] == NULL)
		{
			gTDAL_kbdClientFctArray[i] = callback;
			gTDAL_kbdNbClientRegistered++;
			mTBOX_RETURN(i);
		}
	}

	mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_KBD_Register   error   NB_MAX_CLIENTS   reached\n"));
	mTBOX_RETURN(kTDAL_KBD_NO_ID);
}

/******************************************************************************
 * Function Name      : TDAL_KBD_Term
 * Description       : This function unregisters a client
 * Side effects       :
 * Comment        :
 * Inputs          :
 * Outputs        :
 *****************************************************************************/
tTDAL_KBD_ErrorCode   TDAL_KBD_UnRegister(tTDAL_KBD_ClientId   clientIdter)
{
	mTBOX_FCT_ENTER("TDAL_KBD_UnRegister");

	if   (   (   clientIdter   >=   kTDAL_KBD_NB_MAX_CLIENTS)   ||
			(   gTDAL_kbdClientFctArray[clientIdter] == NULL)   )
	{
		mTBOX_TRACE((kTBOX_NIV_WARNING, "TDAL_KBD_UnRegister   error\n"));
		mTBOX_RETURN(eTDAL_KBD_ERROR);
	}

	gTDAL_kbdClientFctArray[clientIdter] = NULL;
	gTDAL_kbdNbClientRegistered--;

	mTBOX_RETURN(eTDAL_KBD_NO_ERROR);
}

/******************************************************************************
 * Function Name      : TDAL_KBD_CallClientsCallbackFct
 * Description       :
 * Side effects       :
 * Comment        :
 * Inputs          :
 * Outputs        :
 *****************************************************************************/
void   TDAL_KBD_CallClientsCallbackFct(   tTDAL_KBD_KeyCode   KeyCode,
                        tTDAL_KBD_KeyStatus   KeyStatus)
{
	uint8_t   i;

	mTBOX_FCT_ENTER("TDAL_KBD_CallClientsCallbackFct");

	mTBOX_TRACE((kTBOX_NIV_1, "TDAL_KBD CALLBACK: KeyCode=0x%x; KeyStatus=%d\n", KeyCode, KeyStatus));

/*#if   !defined(KBD_ST_SKARDIN) && !defined(KBD_PBI_SDQW_5105) && !defined(KBD_ST_RC60_5105)
	//   remove   Start   bit   and   Toggle   bit
	KeyCode = KeyCode & (~kTDAL_KBD_RC5_TOGGLEBIT);
	//   in   TDAL, we   kept   start   bits   or   we   add   it   if   not   present
	//   KeyCode = KeyCode & (~kTDAL_KBD_RC5E_STARTBIT);
	KeyCode = KeyCode   |   kTDAL_KBD_RC5E_STARTBIT;
#endif*/

/*#ifdef   __DEBUG_MEMORY__
	if   (   (KeyCode == 0x43) && (KeyStatus == eTDAL_KBD_KEY_PRESSED)   )
	{
		debug_task_ps();
	}

	if   (   (KeyCode == 0xB0) && (KeyStatus == eTDAL_KBD_KEY_PRESSED)   )
	{
		DisplayAllPartitionStatus();
	}
#endif*/

	for (i = 0;i < kTDAL_KBD_NB_MAX_CLIENTS; i++)
	{
		if   (gTDAL_kbdClientFctArray[i]   !=   NULL)
		{
			mTBOX_TRACE((kTBOX_NIV_1, "Call   function   @0x%x\n",gTDAL_kbdClientFctArray[i]));
			gTDAL_kbdClientFctArray[i](KeyCode, KeyStatus);
		}
	}

	mTBOX_RETURN;
}

/*===========================================================================
 *
 * TDAL_KBD_SetKeyMapping
 *
 * Parameters:
 *            Key mapping
 *
 * Description:
 *      Set the key mapping depends to Product
 *
 * Returns:
 *      eTDAL_KBD_ERROR if error occurs else eTDAL_KBD_ERROR
 *
 *===========================================================================*/

tTDAL_KBD_ErrorCode   TDAL_KBD_SetKeyMapping(
      unsigned   short   NbKeys,
      tTDAL_KBD_KeyCode   *KeyCode
)
{
	mTBOX_FCT_ENTER("TDAL_KBD_SetKeyMapping");
	mTBOX_RETURN(eTDAL_KBD_NO_ERROR);
}

#ifdef   _DEBUG_AUTO_ZAPPING_

/*IMPORT void TKEL_DebugPrintMemoryUsage(void);*/

/*===========================================================================
 *
 * TDAL_KBD_AutoZapTask
 *
 * Parameters: none
 *
 *
 * Description:
 *      Start the automatic zapping (P+ keycode is sent every 5sec)
 * Returns:
 *===========================================================================*/
LOCAL   void   TDAL_KBD_AutoZapTask(void*   unused)
{
    uint32_t   delay = _DEBUG_AUTO_ZAPPING_DELAY * 1000; /*   3sec   */
    uint32_t   counter = 0;
/*
    bufsize curalloc;
    bufsize totfree;
    bufsize maxfree;
    long nget;
    long nrel;
*/
    mTBOX_FCT_ENTER("TDAL_KBD_AutoZapTask");

    if(unused)   {}   /*   keep   compiler   happy   */
    
    while(   gAutoZap   )
    {
        printf("zapping   %d\n", ++counter);
/*
        if(counter %10 == 0)
        {
            TKEL_bstats(&curalloc, &totfree, &maxfree, &nget, &nrel);
            printf("\n\n\n ###################################################\n TDAL_KBD_AutoZapTask curalloc = %d, totfree = %d, maxfree = %d, nget = %d, nrel = %d\n###################################################\n",curalloc,totfree,maxfree,nget,nrel);
            TKEL_DebugPrintMemoryUsage();
        }
*/
        TDAL_KBD_CallClientsCallbackFct(_DEBUG_AUTO_ZAPPING_PROG_PLUS, eTDAL_KBD_KEY_PRESSED); /*   P+   */
        TDAL_KBD_CallClientsCallbackFct(_DEBUG_AUTO_ZAPPING_PROG_PLUS, eTDAL_KBD_KEY_RELEASED); /*   P+   */
        MsOS_DelayTask(delay); /*   3sec   */
    }

}

/*===========================================================================
 *
 * TDAL_KBD_StopAutoZap
 *
 * Parameters: none
 *
 *
 * Description:
 *      Stop the automatic zapping
 *
 * Returns:
 *===========================================================================*/
LOCAL   void   TDAL_KBD_StopAutoZap(void)
{
    mTBOX_FCT_ENTER("TDAL_KBD_StopAutoZap");
    
    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "stopping   zap   stress   test\n"   ));
    if   (gTDAL_kbdpAutoZapTaskId   !=   NULL)
    {
       MsOS_DeleteTask(gTDAL_kbdpAutoZapTaskId);
       gTDAL_kbdpAutoZapTaskId = NULL;
    }
    
    mTBOX_RETURN;

}
#endif   /*   _DEBUG_AUTO_ZAPPING_   */


/*===========================================================================
 *
 * TDAL_KBD_ExitKeyPress
 *
 * Parameters: none
 *
 *
 * Description:
 *      Sends Exit key
 * Returns:
 *===========================================================================*/
void   TDAL_KBD_ExitKeyPress(void)
{
    mTBOX_FCT_ENTER("TDAL_KBD_ExitKeyPress");

    printf("\n\n\n ################### TDAL_KBD_ExitKeyPress \n\n\n\n");

    TDAL_KBD_CallClientsCallbackFct(KEY_EXIT, eTDAL_KBD_KEY_PRESSED); 
    TDAL_KBD_CallClientsCallbackFct(KEY_EXIT, eTDAL_KBD_KEY_RELEASED);
}


