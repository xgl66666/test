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

/** @brief IO 端口定义 */
typedef struct CHDRV_FP_IO_NUM_t
{
	U32	ui_Type;		///< 类型 CHDRV_FP_IO_TYPE_e
	U32	ui_Bit;			///< 位号
	U32	ui_Group;		///< 组号


	U8	uc_Polar;		///< 极性
	U8	uc_IsKey;		///< 1=按键 0=指示灯
	U8  uc_IsUserGroup;	///< 用户定义的IO组, 用于实现组合键(灯)
}CHDRV_FP_IO_NUM_t;



//////////////////////////////////////////////////////////////////////////
/** @brief IO类型 */
typedef enum CHDRV_FP_IO_TYPE_e
{
	CHDRV_FP_NML_GPIO = 0,		///< 普通IO
	CHDRV_FP_AON_GPIO,			///< AON 类型IO
	CHDRV_FP_S_GPIO,			///< SGPIO
	CHDRV_FP_AONS_GPIO,			///< ANOS GPIO
}CHDRV_FP_IO_TYPE_e;

//////////////////////////////////////////////////////////////////////////
/** @brief   GPIO 端口映射信息 
@details 某些前面板有专门的IO口连接到按键或者指示灯
*/
typedef struct CHDRV_FP_IOMap_t
{
	U32					ui_MapedCode;	///< 映射后的按键值或者指示灯编码 
	CHDRV_FP_IO_NUM_t	str_IONum;		///< 所连接的IO口
}CHDRV_FP_IOMap_t;
typedef enum
{
	CHDRV_GPIO_NORMAL=0,	///< 通用IO 口，常用配置
	CHDRV_GPIO_AONGPIO,		///< 博通平台专用IO 口
	CHDRV_GPIO_SGPIO,		///< 博通平台专用IO 口
	CHDRV_GPIO_AONSGPIO,	///< 博通平台专用IO 口
	CHDRV_GPIO_SPECIAL,		///< 特殊IO 口，预留使用
	CHDRV_GPIO_TYPE_MAX		///< 非法判断
}CHDRV_GPIO_IOTYPE_e;


// 声明一个IO key_lamp:1=按键,0=指示灯, polar:极性, type:IO类型, group:组号, bit:位号
#define DECLARE_IO(key_lamp, polar, type, group, bit) \
{.uc_IsKey=(key_lamp), .uc_Polar=(polar), .ui_Type=(type), .ui_Group=(group), .ui_Bit=bit}

#define DECLARE_NORMAL_KEY_IO(group, bit)	DECLARE_IO(1, 0, CHDRV_GPIO_NORMAL, (group), (bit))
#define DECLARE_NORMAL_LAMP_IO(group, bit)	DECLARE_IO(0, 0, CHDRV_GPIO_NORMAL, (group), (bit))
#define DECLARE_NORMAL_LAMP_IO_RESERVE(group, bit)	DECLARE_IO(0, 1, CHDRV_GPIO_NORMAL, (group), (bit))
#define DECLARE_IO_NOT_CONNECT	DECLARE_IO(0,0,CHDRV_FP_NML_GPIO,~(0L),~(0L))


#define DECLARE_NORMAL_IO		DECLARE_NORMAL_LAMP_IO

typedef enum
{
	CHDRV_FP_KEY_POWER = FP_KEY_STANDBY, 			///< 绿色指示灯
	CHDRV_FP_KEY_UP = FP_KEY_UP, 			///< 绿色指示灯
	CHDRV_FP_KEY_DOWN = FP_KEY_DOWN, 			///< 绿色指示灯

};

///@brief  IOmap
//////////////////////////////////////////////////////////////////////////
// IO映射表(连接到IO口的按键和指示灯)
static const CHDRV_FP_IOMap_t g_FPIOMap[] =
{
	///TODO: 在此添加GPIO映射表项....

	{CHDRV_FP_LED_RED, DECLARE_NORMAL_LAMP_IO_RESERVE(0,99)}, 
	{CHDRV_FP_LED_GREEN, DECLARE_NORMAL_LAMP_IO(0,98)},
#if 1
	{CHDRV_FP_KEY_POWER, DECLARE_NORMAL_KEY_IO(0,102)}, 
	{CHDRV_FP_KEY_UP, DECLARE_NORMAL_KEY_IO(0,63)},
	{CHDRV_FP_KEY_DOWN, DECLARE_NORMAL_KEY_IO(0,62)},
#endif
	/* 最后一行,表示数组的结束,请勿删除!!!! */
	{~0L, DECLARE_IO_NOT_CONNECT}
};


//////////////////////////////////////////////////////////////////////////
/** @brief  硬件连接信息 */
typedef union CHDRV_FP_HWInfo_u
{
	U32		ui_StandbyLamp;		///< 待机指示灯, ~0L 表示不理会待机时的指示灯
	U32		ui_StandbyKey;		///< 待机键值, ~0L 表示不理会待机时的按键
	
	/** @brief   SPI 模式的前面板,如 PT6964,FD650 */
	struct  
	{
		CHDRV_FP_IO_NUM_t		ui_Stb;		///< STB 所连接的IO 口
		CHDRV_FP_IO_NUM_t		ui_Clk;		///< CLK 所连接的IO 口
		CHDRV_FP_IO_NUM_t		ui_Data;	///< DATA 所连接的IO 口
	}spi;

	/** @brief  I2C 模式的前面板, 如 VFD, TOUCH_SCREEN 等*/ 
	struct
	{
		CHDRV_FP_IO_NUM_t		ui_Gpio1;   ///< 与GPIO1  脚相连的主芯片管脚 
		CHDRV_FP_IO_NUM_t		ui_Gpio2;   ///< 与GPIO2  脚相连的主芯片管脚,按键中断 
		U32						ui_I2CAddr;	///< I2C地址
		MS_BOOL					b_InnerAsc;	///< 单片机内建asc字符
		MS_BOOL					b_HaveStandbyFunc;	///< 单片机自身是否带真待机(唤醒)功能
		MS_BOOL					b_UseSoftI2c;///< 是否使用软件模拟的I2C 
		
		U32						ui_Channel;	///< 真实I2C: I2C通道, 
		
		/** @brief   以下为模拟I2C配置,如果是真实I2C,不需要进行配置*/
		CHDRV_FP_IO_NUM_t		ui_Clk;		///< 模拟I2C:时钟引脚 
		CHDRV_FP_IO_NUM_t		ui_Data;	///< 模拟I2C:数据引脚 
	}i2c;
}CHDRV_FP_HWInfo_u;

//////////////////////////////////////////////////////////////////////////
// 硬件连接信息
static const CHDRV_FP_HWInfo_u g_FPHWInfo =
{

	.ui_StandbyLamp = ~0L,
	.ui_StandbyKey = ~0L,

#if 1	// 如果PT6964等SPI模式的前面板, 需要(只需)对spi参数进行设置
	.spi=
	{
		.ui_Stb		= DECLARE_NORMAL_IO(0,0),	// STB 所连接的IO 口
		//.ui_Clk		= DECLARE_NORMAL_IO(0,18),	// CLK 所连接的IO 口
		.ui_Clk		= DECLARE_IO(1,1,CHDRV_GPIO_NORMAL,0,18),	// CLK 所连接的IO 口
		.ui_Data	= DECLARE_NORMAL_IO(0,19),	// DATA 所连接的IO 口
	}
#endif


#if 0		// 如果是VFD 等I2C模式的前面板, 需要(只需)对i2c参数进行设置
	// 触摸屏配置例子
	.i2c =
	{
		.ui_Gpio1	= DECLARE_IO_NOT_CONNECT,	// GPIO1 未连接
		.ui_Gpio2	= DECLARE_NORMAL_IO(5,5),	// GPIO2 连接到5.5  按键中断
		.ui_I2CAddr	= 0xd0,					// I2C地址
		.b_InnerAsc	= 1,					// 触摸屏内建asc字符
		.b_HaveStandbyFunc = 0,				// 单片机自身是否带真待机(唤醒)功能
		.b_UseSoftI2c= 1,					// 是否使用软件模拟的I2C
		.ui_Channel	= 0,					// 通道号, 软件模拟时不使用此配置
		.ui_Clk		= DECLARE_NORMAL_IO(5,3),		// 模拟I2C:时钟引脚
		.ui_Data	= DECLARE_NORMAL_IO(5,2),	// 模拟I2C:数据引脚
	}
#endif


};

//////////////////////////////////////////////////////////////////////////
/** @brief   前面板配置参数*/
typedef struct CHDRV_FP_Params_t
{
	U32		ui_Size;		///< 本结构体大小, =sizeof(CHDRV_FP_FrontPanelParams_t) 
	U32		ui_Ver;			///< =CHDRV_CONFIG_FP_VER 

	U32		ui_Type;		///< 前面板类型
	U32		ui_PrintKeyLevel;	///< 打印(按键值)设置: 0=不打印,1=只打印没从映射表中找到的,2=打印所有的 
	U32		ui_LedGridOffset;	///< 数码管位号偏移, 一般情况下此值为0 
	U32		ui_LenNum;			///< 数码管数量, 0 表示没有连接数码管 

	U32		ui_1stRepeatTime;	///< 从按键按下到第一次重复键的时间
	U32		ui_RepeateTime;		///< 重复键的间隔时间

	CHDRV_FP_HWInfo_u	const *	pstru_HWInfo;	///< 硬件连接信息
	CHDRV_FP_IOMap_t	const * pstru_IOMap;		///< GPIO 映射信息


	const char * pc_AdditionParas;	///< 附加参数, 特殊情况下使用, 通常情况置为NULL
} CHDRV_FP_Params_t;


//////////////////////////////////////////////////////////////////////////
// 前面板全局配置参数
const CHDRV_FP_Params_t gstr_CHDRV_FP_Params =
{
	// 以下2个值为固定值,请勿修改 !!!
	.ui_Size			= sizeof(CHDRV_FP_Params_t),
	.ui_Ver				= 0x202,

	.ui_Type			= 0,	// 前面板类型
	//.ui_Type			= CHDRV_FP_TYPE_FD650,// 前面板类型
	.ui_PrintKeyLevel	= 1,	// 打印(按键值)设置: 0=不打印,1=只打印没从映射表中找到的,2=打印所有的


	// 以下2个设置在类型是"none"的情况下,不用进行配置

#if 0	// 触摸屏的按键灯连接到GRID0, 四个数码分别连接到GRID[1:4]
	.ui_LedGridOffset	= 1,	// 数码管位号偏移, 一般情况下此值为 0
	.ui_LenNum			= 4,			// 数码管数量, 0 表示没有连接数码管
#endif

#if 1	// 通常情况的PT6964配置
	.ui_LedGridOffset	= 0,	// 数码管位号偏移, 一般情况下此值为 0
	.ui_LenNum			= 4,	// 数码管数量, 0 表示没有连接数码管
#endif

	// 重复键发送时间设定(单位:毫秒)
	.ui_1stRepeatTime = 350,
	.ui_RepeateTime   = 150,

	// 以下几个指针,指向具体的(配置)数据结构
	.pstru_HWInfo		= &g_FPHWInfo,		// 硬件连接信息
	.pstru_IOMap		= g_FPIOMap,		// IO口映射表

	// 备用参数
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


// 写IO口
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

// 读IO口
// 返回逻辑电平数据
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
	// 设定IO口的方向
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



	// 遍历GPIO指示灯
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
