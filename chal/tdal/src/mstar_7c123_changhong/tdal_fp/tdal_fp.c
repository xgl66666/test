/*****************************************************************************
 *                  COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_FP
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_fp/tdal_fp.c $
 *        $Rev: 414 $
 *        $Date: 2011-09-23 12:34:21 +0200 (Fri, 23 Sep 2011) $
 *
 * Description : Front Panel driver TDAL library implementation.
 *
 ****************************************************************************/

/****************************************************************************
 *  INCLUDES                                                                *
 ****************************************************************************/
/*   Generic   Headers   */

//#define CUSTOMIZE_LED 1

#include "MsCommon.h"

#include "Board.h"
#include "drvFrontPnl.h"
#include "drvSAR.h"
#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
#include "drvGPIO.h"
#endif
#include "crules.h"
#include "tbox.h"
#include "tdal_common.h"
#include "tdal_fp.h"
#include "tdal_fp_p.h"
#include "tdal_common_priv.h"



/****************************************************************************
 *  DEFINES                                                                 *
 ****************************************************************************/
#define kTDAL_FP_NB_MAX_CLIENTS   2
#define kTDAL_FP_KEYS_NUMBERS     3

#define TDAL_FP_BLINK_INTERVAL 100
#define TDAL_FP_KBD_QUERY      200 // same as KEYPAD_QUERY_TIME_MS in drvGA1204A.c

#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
#define LED_POWER_RED       BALL_E16
#define LED_POWER_GREEN       BALL_D12
#endif
/****************************************************************************
 *  MACROS                                                                  *
 ****************************************************************************/

/****************************************************************************
 *  TYPEDEFS                                                                *
 ****************************************************************************/

typedef enum
{
    TDAL_FPi_LedAction_RedOn,
    TDAL_FPi_LedAction_RedOff,
    TDAL_FPi_LedAction_YellowOn,
    TDAL_FPi_LedAction_YellowOff,
    TDAL_FPi_LedAction_YellowBlink,
    TDAL_FPi_LedAction_Terminate,
} TDAL_FPi_LedAction;


/****************************************************************************
 *  GLOBAL VARIABLES (GLOBAL/IMPORT)                                        *
 ****************************************************************************/
#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
tTDAL_FP_LED_STATUS LED_STATUS = FP_LED_GREEN_ON;
#endif
/****************************************************************************
 *  LOCAL MODULE VARIABLES (MODULE/IMPORT)                                  *
 ****************************************************************************/

/****************************************************************************
 *  TRACE CONFIGURATION                                                     *
 ****************************************************************************/

/****************************************************************************
 *  LOCAL FILE VARIABLES (LOCAL)                                            *
 ****************************************************************************/
mTBOX_SET_MODULE(eTDAL_FP);

LOCAL   bool                     TDAL_FP_Initialized = false;
#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
#else
LOCAL   bool                     TDAL_FP_Terminated = false;
#endif
LOCAL uint8_t                 gTDAL_fpNbClientRegistered = 0;
LOCAL tTDAL_FP_CallbackFct    gTDAL_fpClientFctArray[kTDAL_FP_NB_MAX_CLIENTS];

LOCAL TDAL_task_id tdalFpTask = NULL;
LOCAL TDAL_queue_id tdalFpQueueID = -1;
LOCAL MS_U8  tdalFpStack[16*1024];


/****************************************************************************
 *  FUNCTIONS DEFINITIONS (LOCAL/GLOBAL)                                    *
 ****************************************************************************/

#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
void TP_FPi_LEDGreenOn(void)
{
	mdrv_gpio_set_low(LED_POWER_GREEN);
}
void TP_FPi_LEDGreenOff(void)
{
	mdrv_gpio_set_high(LED_POWER_GREEN);
}
void TP_FPi_LEDRedOn(void)
{
	mdrv_gpio_set_low(LED_POWER_RED);
}
void TP_FPi_LEDRedOff(void)
{
	mdrv_gpio_set_high(LED_POWER_RED);
}

 void TDAL_FP_SetLedSatus(tTDAL_FP_LED_STATUS eLED_STATUS)
{
    LED_STATUS = eLED_STATUS;
}

#else
/* ----------------------------------------------------------------------------
   Name:        TP_FPi_LedCommanOn

   Description:

   Parameters:

   Return Value:
   ------------------------------------------------------------------------- */
LOCAL void TP_FPi_LedCommanOn(void)
{
#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
	TDAL_FP_LedGreenOn();
#endif
}

/* ----------------------------------------------------------------------------
   Name:        TP_FPi_LedCommanOff

   Description:

   Parameters:

   Return Value:
   ------------------------------------------------------------------------- */
LOCAL void TP_FPi_LedCommanOff(void)
{
#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
	TDAL_FP_LedREDOff();
	TDAL_FP_LedGreenOff();
#endif
}

/* ----------------------------------------------------------------------------
   Name:        TP_FPi_LedTunerLock

   Description:

   Parameters:

   Return Value:
   ------------------------------------------------------------------------- */
LOCAL void TP_FPi_LedTunerLock(void)
{
#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
	TDAL_FP_LedGreenOn();
#else
	mdrv_gpio_set_high(IO_NUM_LED_GREEN_LOCK);
#endif
}

/* ----------------------------------------------------------------------------
   Name:        TP_FPi_LedTunerUnLock

   Description:

   Parameters:

   Return Value:
   ------------------------------------------------------------------------- */
LOCAL void TP_FPi_LedTunerUnLock(void)
{
#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
	TDAL_FP_LedGreenOff();
#else
	mdrv_gpio_set_low(IO_NUM_LED_GREEN_LOCK);
#endif
}

/* ----------------------------------------------------------------------------
   Name:        TP_FPi_LedMailOn

   Description:

   Parameters:

   Return Value:
   ------------------------------------------------------------------------- */
LOCAL void TP_FPi_LedMailOn(void)
{
#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
#else
	mdrv_gpio_set_high(IO_NUM_LED_YELLOW_STANDBY);
#endif
}

/* ----------------------------------------------------------------------------
   Name:        TP_FPi_LedMailOff

   Description:

   Parameters:

   Return Value:
   ------------------------------------------------------------------------- */
LOCAL void TP_FPi_LedMailOff(void)
{
#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
#else
	mdrv_gpio_set_low(IO_NUM_LED_YELLOW_STANDBY);
#endif
}

/* ----------------------------------------------------------------------------
   Name:        TP_FPi_LedStandbyOn

   Description:

   Parameters:

   Return Value:
   ------------------------------------------------------------------------- */
LOCAL void TP_FPi_LedStandbyOn(void)
{
#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
	TDAL_FP_LedREDOn();
	TDAL_FP_LedGreenOff();
#else
	mdrv_gpio_set_high(IO_NUM_LED_RED_POWER);
#endif
}

/* ----------------------------------------------------------------------------
   Name:        TP_FPi_LedStandbyOff

   Description:

   Parameters:

   Return Value:
   ------------------------------------------------------------------------- */
LOCAL void TP_FPi_LedStandbyOff(void)
{
#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
	TDAL_FP_LedREDOff();
	TDAL_FP_LedGreenOff();
#else
	mdrv_gpio_set_low(IO_NUM_LED_RED_POWER);
#endif
}
#endif
static MS_U32 TDAL_FPi_SmallerTime(MS_U32 time1, MS_U32 time2)
{
    MS_U32 diff1 = time1 - time2;
    MS_U32 diff2 = time2 - time1;

    if (diff1 < diff2)
    {
        return time2;
    }
    else
    {
        return time1;
    }
}

static MS_U32 TDAL_FPi_CalculateTimeUntilEvent(MS_U32 nextEventTime)
{
    MS_U32 now = MsOS_GetSystemTime();
    MS_U32 smaller = TDAL_FPi_SmallerTime(now, nextEventTime);

    if (smaller == now)
    {
        return nextEventTime - now;
    }
    else
    {
        return 0;
    }

}

static void TDAL_FPi_Task0(void * p)
{
#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)	
    TDAL_FPi_LedAction       ledAction;
    bool                     TDAL_FPi_RedOn = false;
    bool                     blinkRequested = false;
    MS_U32                   blinkTime = 0;
    bool                     terminate  = false;
    MS_BOOL                  bRet;
    MS_U32                   actualSize;
	
    mTBOX_FCT_ENTER("TDAL_FPi_Task0");

    while (!terminate)
    {
        MS_U32 timeout;

        if (blinkRequested)
        {
            MS_U32 nextFpBlink = blinkTime + TDAL_FP_BLINK_INTERVAL;
            timeout = TDAL_FPi_CalculateTimeUntilEvent(nextFpBlink);
        }
        else
        {
            timeout = MSOS_WAIT_FOREVER;
        }

#if MSTAR_QUEUE
        bRet = MsOS_RecvFromQueue(tdalFpQueueID, (MS_U8 *) &ledAction, sizeof(TDAL_FPi_LedAction), &actualSize, timeout);
#else
        bRet = TDAL_Dequeue(tdalFpQueueID, &ledAction);
#endif

        if (blinkRequested && ((MsOS_GetSystemTime() - blinkTime) >= TDAL_FP_BLINK_INTERVAL))
        {
            blinkRequested = false;
            blinkTime = 0;
            if (TDAL_FPi_RedOn)
            {
                TP_FPi_LEDRedOn();
                TP_FPi_LEDGreenOff();
            }
            else
            {
                TP_FPi_LEDRedOff();
                TP_FPi_LEDGreenOn();
            }
        }

        if (FALSE == bRet)
        {
            mTBOX_TRACE((kTBOX_NIV_WARNING, "tdalFpQueueID    failed\n"));
            continue;
        }

        switch (ledAction)
        {
        case TDAL_FPi_LedAction_RedOn:
            TP_FPi_LEDRedOn();
            TP_FPi_LEDGreenOff();
            TDAL_FPi_RedOn = true;
            break;
        case TDAL_FPi_LedAction_RedOff:
            TP_FPi_LEDRedOff();
            TP_FPi_LEDGreenOn();
            TDAL_FPi_RedOn = false;
            break;
        case TDAL_FPi_LedAction_YellowOn:
            break;
        case TDAL_FPi_LedAction_YellowOff:
            break;
        case TDAL_FPi_LedAction_YellowBlink:
            TP_FPi_LEDRedOff();
            TP_FPi_LEDGreenOff();
            blinkRequested = true;
            blinkTime = MsOS_GetSystemTime();

            break;
        case TDAL_FPi_LedAction_Terminate:
            terminate = true;
            break;
        default:
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_FPi_Task: Unknown action 0x%x\n", ledAction));
            break;
        }
    }
#else	
	SAR_KpdRegCfg   stSarKpdCfg;
	MS_U8 data;
	MS_U32 ret = 0;
	MS_U8 u8Key = 0, u8Repeat = 0;
	
    mTBOX_FCT_ENTER("TDAL_FPi_Task0");
	memset(&stSarKpdCfg,0,sizeof(stSarKpdCfg));
	
	ret = MDrv_SAR_Adc_Config(SAR_CHANNEL_1,TRUE);
	if (ret != E_SAR_ADC_OK)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MDrv_SAR_Adc_Config=0x%x\n",ret));
	}
	
	data = MDrv_SAR_Adc_GetValue(SAR_CHANNEL_1);
	//if (data )
	{
        stSarKpdCfg.u8SARChID = SAR_CHANNEL_1;
        stSarKpdCfg.tSARChBnd.u8LoBnd = 0x00;
        stSarKpdCfg.tSARChBnd.u8UpBnd = 0xFF;
        stSarKpdCfg.u8KeyLevelNum = 3;
 
        stSarKpdCfg.u8KeyThreshold[0] = 0x18;
        stSarKpdCfg.u8KeyThreshold[1] = 0x67;
        stSarKpdCfg.u8KeyThreshold[2] = 0xAD;
        stSarKpdCfg.u8KeyCode[0] = 0x00;
        stSarKpdCfg.u8KeyCode[1] = 0x01;
        stSarKpdCfg.u8KeyCode[2] = 0x02;
    }

	MDrv_SAR_Kpd_SetChInfo(&stSarKpdCfg);

    MDrv_SAR_SetCallback(NULL);

    MDrv_SAR_SetLevel(E_SAR_33V);
#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
    while (1)
#else
    while (!TDAL_FP_Terminated)
#endif
    {
		MDrv_SAR_Kpd_GetKeyCode(&u8Key, &u8Repeat);
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "data=0x%x,u8Key=0x%x,u8Repeat=%d\n",MDrv_SAR_Adc_GetValue(SAR_CHANNEL_1),u8Key,u8Repeat));
		TDAL_DelayTask(1000);
		TP_FPi_LedTunerLock();
		TDAL_DelayTask(1000);
		TP_FPi_LedTunerUnLock();
		TDAL_DelayTask(1000);
		TP_FPi_LedMailOn();
		TDAL_DelayTask(1000);
		TP_FPi_LedMailOff();
		TDAL_DelayTask(1000);
		TP_FPi_LedStandbyOn();
		TDAL_DelayTask(1000);
		TP_FPi_LedStandbyOff();
		TDAL_DelayTask(1000);
		
    }
	#endif
}

#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
#else
static void TDAL_FPi_Task(void * p)
{
    TDAL_FPi_LedAction       ledAction;
    bool                     TDAL_FPi_YellowOn = false;
    bool                     blinkRequested = false;
    MS_U32                   blinkTime = 0;
    MS_U32                   kbdQueryTime = 0;
    bool                     terminate  = false;
    MS_BOOL                  bRet;
    MS_U32                   actualSize;


    mTBOX_FCT_ENTER("TDAL_FPi_Task");

    bRet = MDrv_SAR_Enable(TRUE);
	if (bRet != TRUE)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MDrv_SAR_Enable failed\n"));
	}
	else
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MDrv_SAR_Enable success\n"));
	}

    while (!terminate)
    {
        MS_U32 timeout;

        /* Let's calculate how much do we need to wait */
        {
            MS_U32 nextKbdRead = kbdQueryTime + TDAL_FP_KBD_QUERY;
            MS_U32 nextFpBlink = blinkRequested ? (blinkTime + TDAL_FP_BLINK_INTERVAL) : nextKbdRead;
            MS_U32 next = TDAL_FPi_SmallerTime(nextKbdRead, nextFpBlink);

            timeout = TDAL_FPi_CalculateTimeUntilEvent(next);
        }

		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MDrv_SAR_Adc_GetValue=0x%x\n",MDrv_SAR_Adc_GetValue(SAR_CHANNEL_1)));
#if MSTAR_QUEUE
        bRet = MsOS_RecvFromQueue(tdalFpQueueID, (MS_U8 *)&ledAction, sizeof(TDAL_FPi_LedAction), &actualSize, timeout);
#else
        bRet = TDAL_Dequeue(tdalFpQueueID, &ledAction);
#endif
        if (blinkRequested && ((MsOS_GetSystemTime() - blinkTime) >= TDAL_FP_BLINK_INTERVAL))
        {
            blinkRequested = false;
            blinkTime = 0;
            if (TDAL_FPi_YellowOn)
            {
                MDrv_SAR_Enable(FALSE);
            }
            else
            {
                MDrv_SAR_Enable(TRUE);//FRONTPNL_LED_RED,FRONTPNL_LED_GREEN
            }
        }

        if ((MsOS_GetSystemTime() - kbdQueryTime) >= TDAL_FP_KBD_QUERY)
        {
            TDAL_FPi_ReadKeys();
            kbdQueryTime = MsOS_GetSystemTime();
        }
#if 0
        if (FALSE == bRet)
        {
            mTBOX_TRACE((kTBOX_NIV_WARNING, "tdalFpQueueID    failed\n"));
            continue;
        }


        switch(ledAction)
        {
        case TDAL_FPi_LedAction_RedOn:
            MDrv_SAR_Enable(FALSE);//MDrv_FrontPnl_EnableLED
            break;
        case TDAL_FPi_LedAction_RedOff:
            MDrv_SAR_Enable(TRUE);
            break;
        case TDAL_FPi_LedAction_YellowOn:
            MDrv_SAR_Enable(FALSE);
            TDAL_FPi_YellowOn = true;
            break;
        case TDAL_FPi_LedAction_YellowOff:
            MDrv_SAR_Enable(TRUE);
            TDAL_FPi_YellowOn = false;
            break;
        case TDAL_FPi_LedAction_YellowBlink:
            if (TDAL_FPi_YellowOn)
            {
                MDrv_SAR_Enable(TRUE);
            }
            else
            {
                MDrv_SAR_Enable(FALSE);
            }
            blinkRequested = true;
            blinkTime = MsOS_GetSystemTime();

            break;
        case TDAL_FPi_LedAction_Terminate:
            terminate = true;
            break;
        default:
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_FPi_Task: Unknown action 0x%x\n", ledAction));
            break;
        }
#endif
    }
}
#endif

/**========================================================================**
 * Function    : TDAL_FP_Init
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_FP_ErrorCode TDAL_FP_Init()
{
    tTDAL_FP_ErrorCode err;
    MS_BOOL bRet = TRUE;

    mTBOX_FCT_ENTER("TDAL_FP_Init");

    //bRet = MDrv_SAR_Kpd_Init();//MDrv_FrontPnl_Init();

    if (bRet == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Could not intialize front panel\n"));
        mTBOX_RETURN(eTDAL_FP_ERROR);
    }
	mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MDrv_SAR_Kpd_Init success\n"));
#if MSTAR_QUEUE
    tdalFpQueueID = MsOS_CreateQueue(NULL,
            NULL,
            E_MSG_FIXED_SIZE,
            sizeof(TDAL_FPi_LedAction),
            E_MSOS_FIFO,
            "Tdal Fp Queue");
#else
    TDAL_CreateQueue(TDAL_QUEUE_MSG_MAX_NO, sizeof(TDAL_FPi_LedAction), &tdalFpQueueID);
#endif
    if (tdalFpQueueID < 0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MsOS_CreateQueue FAIL\n"));
    }
	mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_CreateQueue success\n"));

    tdalFpTask = TDAL_CreateTask(
            eTDAL_PRIORITY_NORMAL,
            "Tdal Fp Task",
            tdalFpStack,
            sizeof(tdalFpStack),
            TDAL_FPi_Task0,
            NULL);
    if (tdalFpTask == NULL)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Creating task failed\n"));
    }
	mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_CreateTask TDAL_FPi_Task success\n"));
	#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
	TDAL_FP_SetLedSatus(FP_LED_GREEN_ON);
	err = eTDAL_FP_NO_ERROR;
	#else
    err = TDAL_FP_key_Init();
	#endif
	TDAL_FP_Initialized = TRUE;

	//mdrv_gpio_set_high(IO_NUM_LED_GREEN_LOCK);
	//mdrv_gpio_set_high(IO_NUM_LED_RED_POWER);
	//mdrv_gpio_set_high(IO_NUM_LED_YELLOW_STANDBY);

    mTBOX_RETURN(err);
}

/**========================================================================**
 * Function    : TDAL_FP_Terminate
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_FP_ErrorCode TDAL_FP_Terminate(void)
{
    tTDAL_FP_ErrorCode err;
    MS_BOOL status;
    TDAL_FPi_LedAction ledAction;

    mTBOX_FCT_ENTER("TDAL_FP_Terminate");

    err = TDAL_FP_key_Terminate();

#if defined (CUSTOMIZE_LED) && (CUSTOMIZE_LED == 1)
    do
    {
        ledAction = TDAL_FPi_LedAction_Terminate;

#if MSTAR_QUEUE
        status = MsOS_SendToQueue (tdalFpQueueID, (MS_U8 *)&ledAction, sizeof(TDAL_FPi_LedAction), 0);
#else
        status = TDAL_Enqueue(tdalFpQueueID, &ledAction);
#endif
        if (status == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_FP_Terminate: MsOS_SendToQueue   failed\n"));
        }
    } while (status == FALSE);
#endif
    TDAL_DeleteTask(tdalFpTask);
    tdalFpTask = NULL;

#if MSTAR_QUEUE
    status = MsOS_DeleteQueue (tdalFpQueueID);
#else
    status = TDAL_DeleteQueue(tdalFpQueueID);
#endif
    if   (status == FALSE)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_FP_Terminate   error   :   MsOS_DeleteQueue   Failed\n"));
        err = eTDAL_FP_ERROR;
    }
    tdalFpQueueID = -1;

    mTBOX_RETURN(err);
}

/**========================================================================**
 * Function    : TDAL_FP_Register
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_FP_ClientId   TDAL_FP_Register(tTDAL_FP_CallbackFct   callback)
{
   uint8_t   i;

   mTBOX_FCT_ENTER("TDAL_FP_Register");

   if ((TDAL_FP_Initialized == FALSE)   ||
       (gTDAL_fpNbClientRegistered == kTDAL_FP_NB_MAX_CLIENTS)   ||
       (callback == NULL))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_FP_Register   error\n"));
      mTBOX_RETURN(kTDAL_FP_NO_ID);
   }

   for (i=0 ; i<kTDAL_FP_NB_MAX_CLIENTS ; i++)
   {
      if (gTDAL_fpClientFctArray[i] == NULL)
      {
         gTDAL_fpClientFctArray[i] = callback;
         gTDAL_fpNbClientRegistered++;
         mTBOX_RETURN(i);
      }
   }

   mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_FP_Register   error   NB_MAX_CLIENTS   reached\n"));
   mTBOX_RETURN(kTDAL_FP_NO_ID);
}

/**========================================================================**
 * Function     : TDAL_FP_UnRegister
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_FP_ErrorCode   TDAL_FP_UnRegister(tTDAL_FP_ClientId   clientId)
{
   mTBOX_FCT_ENTER("TDAL_FP_UnRegister");

   if ((clientId >= kTDAL_FP_NB_MAX_CLIENTS) ||
       (gTDAL_fpClientFctArray[clientId] == NULL) || (gTDAL_fpNbClientRegistered == 0))
   {
      mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_FP_UnRegister   error\n"));
      mTBOX_RETURN(eTDAL_FP_ERROR);
   }

   gTDAL_fpClientFctArray[clientId] = NULL;
   gTDAL_fpNbClientRegistered--;

   mTBOX_RETURN(eTDAL_FP_NO_ERROR);
}

/**========================================================================**
 * Function    : TDAL_FP_SetInfo
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_FP_ErrorCode   TDAL_FP_SetInfo(tTDAL_FP_Info   eInfo, const   void   *param)
{
    bool setInfo;
    MS_BOOL status;
    TDAL_FPi_LedAction ledAction;

    mTBOX_FCT_ENTER(TDAL_FP_SetInfo);

    setInfo = true;

    switch(eInfo)
    {
    case eTDAL_FP_INFO_ON:
        ledAction = TDAL_FPi_LedAction_RedOff;
        break;
    case eTDAL_FP_INFO_OFF:
        ledAction = TDAL_FPi_LedAction_RedOn;
        break;
    case eTDAL_FP_INFO_RCVINFRARED:
        ledAction = TDAL_FPi_LedAction_YellowBlink;
        break;
    case eTDAL_FP_INFO_FE_LOCK:
        if (param != NULL)
        {
            if (strcmp("OK", ((char *) param)) == 0)
            {
                ledAction = TDAL_FPi_LedAction_YellowOn;
            }
            else
            {
                ledAction = TDAL_FPi_LedAction_YellowOff;
            }
        }
        else
        {
            ledAction = TDAL_FPi_LedAction_YellowOff;
        }
        break;
    default:
        mTBOX_TRACE((kTBOX_NIV_1, "TDAL_FP_SetInfo: Unsupported command\n"));
        setInfo = false;
        break;
    }

    if (setInfo)
    {
#if MSTAR_QUEUE
        status = MsOS_SendToQueue (tdalFpQueueID, (MS_U8 *)&ledAction, sizeof(TDAL_FPi_LedAction), 0);
#else
        status = TDAL_Enqueue(tdalFpQueueID, &ledAction);
#endif
        if (status == FALSE)
        {
            mTBOX_TRACE((kTBOX_NIV_CRITICAL, "TDAL_FP_SetInfo: MsOS_SendToQueue   failed\n"));
            return(eTDAL_FP_ERROR);
        }
    }

    return (eTDAL_FP_NO_ERROR);
}

tTDAL_FP_ErrorCode TDAL_FP_EnterActiveLowPower(uint32_t timeout)
{
    return (eTDAL_FP_NO_ERROR);
}

tTDAL_FP_ErrorCode TDAL_FP_ExitActiveLowPower(void)
{
    return (eTDAL_FP_NO_ERROR);
}

void TDAL_FPi_CallClientsCallbackFct(
   tTDAL_FP_KeyCode    KeyCode, 
   tTDAL_FP_KeyStatus  KeyStatus)
{
   uint8_t   i;

   mTBOX_FCT_ENTER("TDAL_FP_CallClientsCallbackFct");

   for (i=0 ; i<kTDAL_FP_NB_MAX_CLIENTS ; i++)
   {
      if (gTDAL_fpClientFctArray[i] != NULL)
      {
         mTBOX_TRACE((kTBOX_NIV_1, "Call   function   @0x%x\n",
                      gTDAL_fpClientFctArray[i]   ));
         gTDAL_fpClientFctArray[i](KeyCode, KeyStatus);
      }
   }

   mTBOX_RETURN;
}

