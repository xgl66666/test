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
#define FP_KEY_STANDBY          0xDC//0x02
#define FP_KEY_DOWN             0xD2//0x01
#define FP_KEY_UP               0xCA//0x00
#define FP_KEY_FACTORY_RESET    0xA5//when fp key down last 10s,report factory reset key to middleware
#define FP_KEY_STANDBY_TIMEOUT  50
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
//#define __LOCAL_FP_DEBUG__
#if defined (__LOCAL_FP_DEBUG__)
#define mFP_DEBUG  printf 

#else
#define mFP_DEBUG(...) 
#endif

/****************************************************************************
 *  FUNCTIONS DEFINITIONS (LOCAL/GLOBAL)                        *
 ****************************************************************************/

LOCAL   void   TDAL_FP_key_Send (tTDALm_WorkerThreadsDescriptor * p);

LOCAL void  TDAL_FPi_FPKeypadCallback (MS_U32 u8Key, MS_U8 u8RepeatFlag);

extern   void   TDAL_FPi_CallClientsCallbackFct(      tTDAL_FP_KeyCode   KeyCode,
                            tTDAL_FP_KeyStatus   KeyStatus);


typedef unsigned int U32;
typedef unsigned short U16;
typedef unsigned char U8;
typedef signed int S32;
typedef signed short S16;
typedef signed char S8;

/** @brief IO �˿ڶ��� */
typedef struct CHDRV_FP_IO_NUM_t
{
	U32	ui_Type;		///< ���� CHDRV_FP_IO_TYPE_e
	U32	ui_Bit;			///< λ��
	U32	ui_Group;		///< ���


	U8	uc_Polar;		///< ����
	U8	uc_IsKey;		///< 1=���� 0=ָʾ��
	U8  uc_IsUserGroup;	///< �û������IO��, ����ʵ����ϼ�(��)
}CHDRV_FP_IO_NUM_t;



//////////////////////////////////////////////////////////////////////////
/** @brief IO���� */
typedef enum CHDRV_FP_IO_TYPE_e
{
	CHDRV_FP_NML_GPIO = 0,		///< ��ͨIO
	CHDRV_FP_AON_GPIO,			///< AON ����IO
	CHDRV_FP_S_GPIO,			///< SGPIO
	CHDRV_FP_AONS_GPIO,			///< ANOS GPIO
}CHDRV_FP_IO_TYPE_e;

//////////////////////////////////////////////////////////////////////////
/** @brief   GPIO �˿�ӳ����Ϣ 
@details ĳЩǰ�����ר�ŵ�IO�����ӵ���������ָʾ��
*/
typedef struct CHDRV_FP_IOMap_t
{
	U32					ui_MapedCode;	///< ӳ���İ���ֵ����ָʾ�Ʊ��� 
	CHDRV_FP_IO_NUM_t	str_IONum;		///< �����ӵ�IO��
}CHDRV_FP_IOMap_t;
typedef enum
{
	CHDRV_GPIO_NORMAL=0,	///< ͨ��IO �ڣ���������
	CHDRV_GPIO_AONGPIO,		///< ��ͨƽ̨ר��IO ��
	CHDRV_GPIO_SGPIO,		///< ��ͨƽ̨ר��IO ��
	CHDRV_GPIO_AONSGPIO,	///< ��ͨƽ̨ר��IO ��
	CHDRV_GPIO_SPECIAL,		///< ����IO �ڣ�Ԥ��ʹ��
	CHDRV_GPIO_TYPE_MAX		///< �Ƿ��ж�
}CHDRV_GPIO_IOTYPE_e;


// ����һ��IO key_lamp:1=����,0=ָʾ��, polar:����, type:IO����, group:���, bit:λ��
#define DECLARE_IO(key_lamp, polar, type, group, bit) \
{.uc_IsKey=(key_lamp), .uc_Polar=(polar), .ui_Type=(type), .ui_Group=(group), .ui_Bit=bit}

#define DECLARE_NORMAL_KEY_IO(group, bit)	DECLARE_IO(1, 0, CHDRV_GPIO_NORMAL, (group), (bit))
#define DECLARE_NORMAL_LAMP_IO(group, bit)	DECLARE_IO(0, 0, CHDRV_GPIO_NORMAL, (group), (bit))
#define DECLARE_NORMAL_LAMP_IO_RESERVE(group, bit)	DECLARE_IO(0, 1, CHDRV_GPIO_NORMAL, (group), (bit))
#define DECLARE_IO_NOT_CONNECT	DECLARE_IO(0,0,CHDRV_FP_NML_GPIO,~(0L),~(0L))


#define DECLARE_NORMAL_IO		DECLARE_NORMAL_LAMP_IO

typedef enum
{
	CHDRV_FP_KEY_POWER = FP_KEY_STANDBY, 			///< ��ɫָʾ��
	CHDRV_FP_KEY_UP = FP_KEY_UP, 			///< ��ɫָʾ��
	CHDRV_FP_KEY_DOWN = FP_KEY_DOWN, 			///< ��ɫָʾ��

};

///@brief  IOmap
//////////////////////////////////////////////////////////////////////////
// IOӳ���(���ӵ�IO�ڵİ�����ָʾ��)
static const CHDRV_FP_IOMap_t g_FPIOMap[] =
{
	///TODO: �ڴ����GPIOӳ�����....

	{CHDRV_FP_LED_RED, DECLARE_NORMAL_LAMP_IO_RESERVE(0,99)}, 
	{CHDRV_FP_LED_GREEN, DECLARE_NORMAL_LAMP_IO(0,98)},
#if 1
	{CHDRV_FP_KEY_POWER, DECLARE_NORMAL_KEY_IO(0,102)}, 
	{CHDRV_FP_KEY_UP, DECLARE_NORMAL_KEY_IO(0,63)},
	{CHDRV_FP_KEY_DOWN, DECLARE_NORMAL_KEY_IO(0,62)},
#endif
	/* ���һ��,��ʾ����Ľ���,����ɾ��!!!! */
	{~0L, DECLARE_IO_NOT_CONNECT}
};


//////////////////////////////////////////////////////////////////////////
/** @brief  Ӳ��������Ϣ */
typedef union CHDRV_FP_HWInfo_u
{
	U32		ui_StandbyLamp;		///< ����ָʾ��, ~0L ��ʾ��������ʱ��ָʾ��
	U32		ui_StandbyKey;		///< ������ֵ, ~0L ��ʾ��������ʱ�İ���
	
	/** @brief   SPI ģʽ��ǰ���,�� PT6964,FD650 */
	struct  
	{
		CHDRV_FP_IO_NUM_t		ui_Stb;		///< STB �����ӵ�IO ��
		CHDRV_FP_IO_NUM_t		ui_Clk;		///< CLK �����ӵ�IO ��
		CHDRV_FP_IO_NUM_t		ui_Data;	///< DATA �����ӵ�IO ��
	}spi;

	/** @brief  I2C ģʽ��ǰ���, �� VFD, TOUCH_SCREEN ��*/ 
	struct
	{
		CHDRV_FP_IO_NUM_t		ui_Gpio1;   ///< ��GPIO1  ����������оƬ�ܽ� 
		CHDRV_FP_IO_NUM_t		ui_Gpio2;   ///< ��GPIO2  ����������оƬ�ܽ�,�����ж� 
		U32						ui_I2CAddr;	///< I2C��ַ
		MS_BOOL					b_InnerAsc;	///< ��Ƭ���ڽ�asc�ַ�
		MS_BOOL					b_HaveStandbyFunc;	///< ��Ƭ�������Ƿ�������(����)����
		MS_BOOL					b_UseSoftI2c;///< �Ƿ�ʹ�����ģ���I2C 
		
		U32						ui_Channel;	///< ��ʵI2C: I2Cͨ��, 
		
		/** @brief   ����Ϊģ��I2C����,�������ʵI2C,����Ҫ��������*/
		CHDRV_FP_IO_NUM_t		ui_Clk;		///< ģ��I2C:ʱ������ 
		CHDRV_FP_IO_NUM_t		ui_Data;	///< ģ��I2C:�������� 
	}i2c;
}CHDRV_FP_HWInfo_u;

//////////////////////////////////////////////////////////////////////////
// Ӳ��������Ϣ
static const CHDRV_FP_HWInfo_u g_FPHWInfo =
{

	.ui_StandbyLamp = ~0L,
	.ui_StandbyKey = ~0L,

#if 1	// ���PT6964��SPIģʽ��ǰ���, ��Ҫ(ֻ��)��spi������������
	.spi=
	{
		.ui_Stb		= DECLARE_NORMAL_IO(0,0),	// STB �����ӵ�IO ��
		//.ui_Clk		= DECLARE_NORMAL_IO(0,18),	// CLK �����ӵ�IO ��
		.ui_Clk		= DECLARE_IO(1,1,CHDRV_GPIO_NORMAL,0,18),	// CLK �����ӵ�IO ��
		.ui_Data	= DECLARE_NORMAL_IO(0,19),	// DATA �����ӵ�IO ��
	}
#endif


#if 0		// �����VFD ��I2Cģʽ��ǰ���, ��Ҫ(ֻ��)��i2c������������
	// ��������������
	.i2c =
	{
		.ui_Gpio1	= DECLARE_IO_NOT_CONNECT,	// GPIO1 δ����
		.ui_Gpio2	= DECLARE_NORMAL_IO(5,5),	// GPIO2 ���ӵ�5.5  �����ж�
		.ui_I2CAddr	= 0xd0,					// I2C��ַ
		.b_InnerAsc	= 1,					// �������ڽ�asc�ַ�
		.b_HaveStandbyFunc = 0,				// ��Ƭ�������Ƿ�������(����)����
		.b_UseSoftI2c= 1,					// �Ƿ�ʹ�����ģ���I2C
		.ui_Channel	= 0,					// ͨ����, ���ģ��ʱ��ʹ�ô�����
		.ui_Clk		= DECLARE_NORMAL_IO(5,3),		// ģ��I2C:ʱ������
		.ui_Data	= DECLARE_NORMAL_IO(5,2),	// ģ��I2C:��������
	}
#endif


};

//////////////////////////////////////////////////////////////////////////
/** @brief   ǰ������ò���*/
typedef struct CHDRV_FP_Params_t
{
	U32		ui_Size;		///< ���ṹ���С, =sizeof(CHDRV_FP_FrontPanelParams_t) 
	U32		ui_Ver;			///< =CHDRV_CONFIG_FP_VER 

	U32		ui_Type;		///< ǰ�������
	U32		ui_PrintKeyLevel;	///< ��ӡ(����ֵ)����: 0=����ӡ,1=ֻ��ӡû��ӳ������ҵ���,2=��ӡ���е� 
	U32		ui_LedGridOffset;	///< �����λ��ƫ��, һ������´�ֵΪ0 
	U32		ui_LenNum;			///< ���������, 0 ��ʾû����������� 

	U32		ui_1stRepeatTime;	///< �Ӱ������µ���һ���ظ�����ʱ��
	U32		ui_RepeateTime;		///< �ظ����ļ��ʱ��

	CHDRV_FP_HWInfo_u	const *	pstru_HWInfo;	///< Ӳ��������Ϣ
	CHDRV_FP_IOMap_t	const * pstru_IOMap;		///< GPIO ӳ����Ϣ


	const char * pc_AdditionParas;	///< ���Ӳ���, ���������ʹ��, ͨ�������ΪNULL
} CHDRV_FP_Params_t;


//////////////////////////////////////////////////////////////////////////
// ǰ���ȫ�����ò���
const CHDRV_FP_Params_t gstr_CHDRV_FP_Params =
{
	// ����2��ֵΪ�̶�ֵ,�����޸� !!!
	.ui_Size			= sizeof(CHDRV_FP_Params_t),
	.ui_Ver				= 0x202,

	.ui_Type			= 0,	// ǰ�������
	//.ui_Type			= CHDRV_FP_TYPE_FD650,// ǰ�������
	.ui_PrintKeyLevel	= 1,	// ��ӡ(����ֵ)����: 0=����ӡ,1=ֻ��ӡû��ӳ������ҵ���,2=��ӡ���е�


	// ����2��������������"none"�������,���ý�������

#if 0	// �������İ��������ӵ�GRID0, �ĸ�����ֱ����ӵ�GRID[1:4]
	.ui_LedGridOffset	= 1,	// �����λ��ƫ��, һ������´�ֵΪ 0
	.ui_LenNum			= 4,			// ���������, 0 ��ʾû�����������
#endif

#if 1	// ͨ�������PT6964����
	.ui_LedGridOffset	= 0,	// �����λ��ƫ��, һ������´�ֵΪ 0
	.ui_LenNum			= 4,	// ���������, 0 ��ʾû�����������
#endif

	// �ظ�������ʱ���趨(��λ:����)
	.ui_1stRepeatTime = 350,
	.ui_RepeateTime   = 150,

	// ���¼���ָ��,ָ������(����)���ݽṹ
	.pstru_HWInfo		= &g_FPHWInfo,		// Ӳ��������Ϣ
	.pstru_IOMap		= g_FPIOMap,		// IO��ӳ���

	// ���ò���
	.pc_AdditionParas = NULL
};
typedef struct  
{


	MS_BOOL					new_key_act;
	U32						new_key_act_code;
	U32						new_key_code;
}KEY_STATE_t;



#define DIRECT_OUT	1
#define DIRECT_IN	0
MS_BOOL	chdrv_fp_set_io_dir(CHDRV_FP_IO_NUM_t ionum, MS_BOOL dir)
{
	if(dir == DIRECT_OUT)
		;//	mdrv_gpio_set_output(ionum.ui_Bit);
	else
		mdrv_gpio_set_input(ionum.ui_Bit);

	return TRUE;
}


// дIO��
// 
MS_BOOL chdrv_fp_set_io_data( CHDRV_FP_IO_NUM_t ionum, U32 data )
{
	if(ionum.uc_Polar)
		data = data ? 0 : 1;

	if(data)
		mdrv_gpio_set_high(ionum.ui_Bit);
	else
		mdrv_gpio_set_low(ionum.ui_Bit);

	return TRUE;
}

// ��IO��
// �����߼���ƽ����
U32 chdrv_fp_get_io_data(CHDRV_FP_IO_NUM_t ionum)
{
	int data;
	//MsOS_DelayTask(50);
	data = mdrv_gpio_get_level(ionum.ui_Bit);

	if(ionum.uc_Polar)
		data = data ? 0 : 1;
	return data;
}
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
	CHDRV_FP_IOMap_t const * io_map = gstr_CHDRV_FP_Params.pstru_IOMap;

	memset(&stSarKpdCfg,0,sizeof(stSarKpdCfg));
	
//	ret = MDrv_SAR_Adc_Config(SAR_CHANNEL_1,TRUE);
	if (ret != E_SAR_ADC_OK)
	{
		mTBOX_TRACE((kTBOX_NIV_CRITICAL, "MDrv_SAR_Adc_Config=0x%x\n",ret));
	}
	
//	data = MDrv_SAR_Adc_GetValue(SAR_CHANNEL_1);
	//if ((data <= 0xFF) && (data > 0xDD))
	{
//        stSarKpdCfg.u8SARChID = SAR_CHANNEL_1;
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
       
	//MDrv_SAR_Kpd_SetChInfo(&stSarKpdCfg);
// YYS add for fp key 
	mdrv_gpio_init();
	// �趨IO�ڵķ���
	while(io_map->ui_MapedCode	!= ~0L)
	{
		U32 dir = io_map->str_IONum.uc_IsKey ? DIRECT_IN :DIRECT_OUT;
		if(TRUE != chdrv_fp_set_io_dir(io_map->str_IONum, dir))
		{
			mFP_DEBUG("set io<%d.%d> dir fail.", 
				io_map->str_IONum.ui_Group, io_map->str_IONum.ui_Bit);
			return eTDAL_FP_ERROR;
		}
		++io_map;
	}

	
//yys add end

//    MDrv_SAR_SetCallback(TDAL_FPi_FPKeypadCallback);

//    MDrv_SAR_SetLevel(E_SAR_33V);

}



tTDAL_FP_ErrorCode CHDRV_FP_CtrlLamp( CHDRV_FP_LAMP_TYPE_e led_type, 
									CHDRV_FP_LAMP_STATE_e led_state )
{
	CHDRV_FP_IOMap_t const * map = gstr_CHDRV_FP_Params.pstru_IOMap;



	// ����GPIOָʾ��
	while(map->ui_MapedCode != ~0L)
	{
		if(map->str_IONum.uc_IsKey==0 && map->ui_MapedCode == led_type) 
		{
#if 0
			U32 sdata = map->str_IONum.uc_Polar ? 1 : 0;
			if(led_state == CHDRV_FP_LAMP_OFF)
				sdata = sdata == 0 ? 1 : 0;
#else
                        U32 sdata = led_state == CHDRV_FP_LAMP_OFF ? 1 : 0;
#endif
			chdrv_fp_set_io_data(map->str_IONum, sdata);

			break;
		}
		++map;
	}

	return eTDAL_FP_NO_ERROR;
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
   mFP_DEBUG(   "TDAL_FP_key_Terminate"   );

	//MDrv_FrontPnl_Callback(NULL);
//	MDrv_SAR_SetCallback(NULL);
	return  eTDAL_FP_NO_ERROR;

}

LOCAL void  TDAL_FPi_FPKeypadCallback (MS_U32 u8Key, MS_U8 u8RepeatFlag)
{
    tTDALm_WorkerThreadsDescriptor desc;
    bool status;

   desc.param1 = (uint32_t) ((u8Key) & 0xFF);
   // desc.param1 = (uint32_t) ((u8Key >> 24) & 0xFF);
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
	tTDAL_FP_KeyStatus fp_Status = eTDAL_FP_KEY_PRESSED;

	//mFP_DEBUG("TDAL_FP_key_Send");

	key = (tTDAL_FP_KeyCode) p->param1;
	fp_Status = (tTDAL_FP_KeyCode) p->param2;

	allowFpKey = true;

	currentTime = MsOS_GetSystemTime();

	{
		if ((currentTime - lastPressed) < 500)
		{
			if (lastKey == key && 0xDC == key)
			{
				allowFpKey = false;
			}
		}
	}

	if (allowFpKey)
	{
			mFP_DEBUG("%s %d keycode [%d][%d]\n",__FUNCTION__,__LINE__,key,fp_Status);
		TDAL_FPi_CallClientsCallbackFct(key, fp_Status);
		//TDAL_FPi_CallClientsCallbackFct(key, eTDAL_FP_KEY_RELEASED);
	}

	lastPressed = currentTime;
	lastKey = key;

}

static MS_U8 do_io_key(MS_U32* key, MS_U8* Flag)
{

    static U32 last_key_code;
    static CHDRV_FP_IOMap_t * plast_io = NULL;
    CHDRV_FP_IOMap_t const * pmap = gstr_CHDRV_FP_Params.pstru_IOMap;
    static KEY_STATE_t g_key_state = {0};
    static  MS_U8 keyTimes=0;
	mFP_DEBUG("%s %d do_io_key\n",__FUNCTION__,__LINE__);
    g_key_state.new_key_act = 0;

    if(plast_io)
    {
        g_key_state.new_key_act_code =
                chdrv_fp_get_io_data(plast_io->str_IONum) == plast_io->str_IONum.uc_Polar ? eTDAL_FP_KEY_HELD_DOWN : eTDAL_FP_KEY_RELEASED;
        keyTimes++;
        mFP_DEBUG("%s %d do_io_key\n",__FUNCTION__,__LINE__);

        if(g_key_state.new_key_act_code == eTDAL_FP_KEY_RELEASED)
        {
            if (last_key_code == CHDRV_FP_KEY_POWER)
            {
                if (keyTimes < FP_KEY_STANDBY_TIMEOUT)
                {
                    *key = FP_KEY_STANDBY;
                    *Flag = eTDAL_FP_KEY_PRESSED;
                    keyTimes = 0;
                    plast_io = 0;
                }
                return 1;
            }
            keyTimes = 0;
            plast_io = 0;
            
            mFP_DEBUG("%s %d do_io_key\n",__FUNCTION__,__LINE__);
        }
        else
        {
            if(last_key_code == CHDRV_FP_KEY_POWER)
            {
                if(keyTimes == FP_KEY_STANDBY_TIMEOUT)
                {
                    *key = FP_KEY_FACTORY_RESET;
                    *Flag = eTDAL_FP_KEY_PRESSED;

                    mFP_DEBUG("%s %d do_io_key\n",__FUNCTION__,__LINE__);
                }
                return 1;
            }
        }
        mFP_DEBUG("%s %d do_io_key\n",__FUNCTION__,__LINE__);

        g_key_state.new_key_act = 1;
        g_key_state.new_key_code = last_key_code;
        *key = last_key_code;
        *Flag = g_key_state.new_key_act_code;
        
        return 1;
    }

    while(pmap->ui_MapedCode != ~(0L))
    {
        if(pmap->str_IONum.uc_IsKey &&
          (chdrv_fp_get_io_data(pmap->str_IONum) == pmap->str_IONum.uc_Polar)
        )
        {
            g_key_state.new_key_act_code= eTDAL_FP_KEY_PRESSED;
            g_key_state.new_key_code = pmap->ui_MapedCode;
            last_key_code = pmap->ui_MapedCode;
            if(last_key_code != CHDRV_FP_KEY_POWER)
                g_key_state.new_key_act = 1;    
            mFP_DEBUG("%s %d do_io_key\n",__FUNCTION__,__LINE__);
            plast_io = pmap;
            break;
        }
        ++pmap;
    }
    if(g_key_state.new_key_act  == 1)
    {
        *key = last_key_code;
        *Flag = g_key_state.new_key_act_code;

        mFP_DEBUG("%s %d do_io_key\n",__FUNCTION__,__LINE__);
        return 1;
    }
    else
    {
        return 0;	
    }
}

MS_U8  _Keypad_GetKey(MS_U32* key, MS_U8* Flag)
{

    static  MS_U8 keyTimes=0;

    if(mdrv_gpio_get_level(CHDRV_FP_KEY_POWER) == 0)  //high
    {
        keyTimes++;
        //mFP_DEBUG("%s %d keyTimes[%d]  key[%d]\n",__FUNCTION__,__LINE__,keyTimes,mdrv_gpio_get_level(POWER_NUM));		
    }
    else
    {
        //mFP_DEBUG("%s %d keyTimes[%d]\n",__FUNCTION__,__LINE__,keyTimes);
        if(keyTimes>50)
        {
            *key = 0xA5;
            keyTimes = 0;
            return 1;
        }
        if(keyTimes>0)
        {
            *key = 0xDC;
            keyTimes = 0;
            return 1;
        }
        else
        {
            *key = 0;
            keyTimes = 0;
            return 0;
        }

    }
    return 0;		
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

   // if( do_io_key(&u32Key, &u8Flag))//77
    {
        if(TDAL_FPi_FPKeypadCallback)
        {
            TDAL_FPi_FPKeypadCallback(u32Key, u8Flag);
        }
    }
}
