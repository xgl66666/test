/*****************************************************************************
 *                  COPYRIGHT 2004 IWEDIA TECHNOLOGIES            *
 *****************************************************************************
 *
 * MODULE NAME: TDAL_FP
 *
 * FILE NAME: $URL: http://svnsrv/svn/iwedia-ext/sources/chal/core/tdal/branches/idecode_hd/os21/tdal_fp/tdal_fp_key.c $
 *        $Rev: 673 $
 *        $Date: 2011-10-28 15:10:43 +0200 (Fri, 28 Oct 2011) $
 *
 * Description : Front Panel driver TDAL library implementation.
 *
 ****************************************************************************/

/****************************************************************************
 *  INCLUDES                                           *
 ****************************************************************************/
/*   Generic   Headers   */
#include "MsCommon.h"
#include "drvFrontPnl.h"
#include "drvSAR.h"


#include "crules.h"
#include "tbox.h"

#include "tdal_common.h"
#include "tdal_fp.h"
#include "tdal_fp_p.h"
#include "tdal_common_priv.h"

/****************************************************************************
 *  DEFINES                                            *
 ****************************************************************************/
/****************************************************************************
 *  DEFINES                                            *
 ****************************************************************************/
/****************************************************************************
 *  MACROS                                              *
 ****************************************************************************/

/****************************************************************************
 *  TYPEDEFS                                           *
 ****************************************************************************/

/****************************************************************************
 *  GLOBAL VARIABLES (GLOBAL/IMPORT)                            *
 ****************************************************************************/

/****************************************************************************
 *  LOCAL MODULE VARIABLES (MODULE/IMPORT)                             *
 ****************************************************************************/

/****************************************************************************
 *  TRACE CONFIGURATION                                    *
 ****************************************************************************/


/****************************************************************************
 *  LOCAL FILE VARIABLES (LOCAL)                               *
 ****************************************************************************/
mTBOX_SET_MODULE(eTDAL_FP);

/****************************************************************************
 *  FUNCTIONS DEFINITIONS (LOCAL/GLOBAL)                        *
 ****************************************************************************/

LOCAL   void   TDAL_FP_key_Send (tTDALm_WorkerThreadsDescriptor * p);

LOCAL void  TDAL_FPi_FPKeypadCallback (MS_U32 u8Key, MS_U8 u8RepeatFlag);

extern   void   TDAL_FPi_CallClientsCallbackFct(      tTDAL_FP_KeyCode   KeyCode,
                            tTDAL_FP_KeyStatus   KeyStatus);

#define FP_POWER_KEY	98

/**========================================================================**
 * Function    : TDAL_FP_key_Init
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_FP_ErrorCode   TDAL_FP_key_Init(void)
{
	SAR_KpdRegCfg   stSarKpdCfg;
	MS_U8 data;
	MS_U32 ret = 0;
	MS_U8 u8Key = 0, u8Repeat = 0;
   mTBOX_FCT_ENTER(   "TDAL_FP_key_Init"   );
	memset(&stSarKpdCfg,0,sizeof(stSarKpdCfg));
	
	ret = MDrv_SAR_Adc_Config(SAR_CHANNEL_1,TRUE);
	if (ret != E_SAR_ADC_OK)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MDrv_SAR_Adc_Config=0x%x\n",ret));
	}
	
	data = MDrv_SAR_Adc_GetValue(SAR_CHANNEL_1);
	//if ((data <= 0xFF) && (data > 0xDD))
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

    MDrv_SAR_SetCallback(TDAL_FPi_FPKeypadCallback);

    MDrv_SAR_SetLevel(E_SAR_33V);

   mTBOX_RETURN(eTDAL_FP_NO_ERROR);
}

/**========================================================================**
 * Function    : TDAL_FP_key_Terminate
 *
 * Description  :
 *
 * Side effects :
 *
 * Comment      :
 *
 **========================================================================**/
tTDAL_FP_ErrorCode   TDAL_FP_key_Terminate(void)
{
   mTBOX_FCT_ENTER(   "TDAL_FP_key_Terminate"   );

	//MDrv_FrontPnl_Callback(NULL);
	MDrv_SAR_SetCallback(NULL);

   mTBOX_RETURN(eTDAL_FP_NO_ERROR);
}

LOCAL void  TDAL_FPi_FPKeypadCallback (MS_U32 u8Key, MS_U8 u8RepeatFlag)
{
    tTDALm_WorkerThreadsDescriptor desc;
    bool status;

    desc.param1 = (uint32_t) ((u8Key >> 24) & 0xFF);
    desc.param2 = (uint32_t) u8RepeatFlag;

    mTBOX_TRACE((kTBOX_NIV_CRITICAL, "%04d %s u8Key = 0x%02x, u8RepeatFlag = 0x%02x ------- from FPCallBack\n", __LINE__, __FUNCTION__, u8Key, u8RepeatFlag));

    status = TDALm_WorkerThreadsSendRequest(TDAL_FP_key_Send, &desc);
    if (status == false)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "[TDAL_FPi_FPKeypadCallback] MsOS_SendToQueue   failed\n"));
    }

}


/******************************************************************************
 * Function Name      : TDAL_FP_key_Send
 * Description       : 
 * Side effects       :
 * Comment        :
 * Inputs          :
 * Outputs        :
 *****************************************************************************/
LOCAL   void   TDAL_FP_key_Send (tTDALm_WorkerThreadsDescriptor * p)
{
	tTDAL_FP_KeyCode key;
	static MS_U32 lastPressed = 0;
	static tTDAL_FP_KeyCode lastKey = 0;
	MS_U32 currentTime;
	bool allowFpKey;

	mTBOX_FCT_ENTER("TDAL_FP_key_Send");

	key = (tTDAL_FP_KeyCode) p->param1;

	allowFpKey = true;

	currentTime = MsOS_GetSystemTime();

	if (key == FP_POWER_KEY)
	{
		if ((currentTime - lastPressed) < 500)
		{
			if (lastKey == key)
			{
				allowFpKey = false;
			}
		}
	}

	if (allowFpKey)
	{
		TDAL_FPi_CallClientsCallbackFct(key, eTDAL_FP_KEY_PRESSED);
		TDAL_FPi_CallClientsCallbackFct(key, eTDAL_FP_KEY_RELEASED);
	}

	lastPressed = currentTime;
	lastKey = key;

	mTBOX_RETURN;
}

/******************************************************************************
 * Function Name      : TDAL_FPi_ReadKeys
 * Description       :  This function read keys from the FP driver.
 *                      It is very tightly connected to the driver and needs to be called every 200 ms.
 * Side effects       :
 * Comment        :
 * Inputs          :
 * Outputs        :
 *****************************************************************************/
void TDAL_FPi_ReadKeys(void)
{
    static MS_U8 u8Channel = 0;
    MS_U32 u32Key = 0;
    MS_U8 u8Flag = 0;

    //if( _Keypad_GetKey(u8Channel, &u32Key, &u8Flag))
    {
        if(TDAL_FPi_FPKeypadCallback)
        {
            TDAL_FPi_FPKeypadCallback(u32Key, u8Flag);
        }
    }
}
