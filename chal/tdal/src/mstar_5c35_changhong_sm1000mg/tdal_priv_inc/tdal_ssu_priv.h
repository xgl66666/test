/*
 * tdal_ssu_priv.h
 *
 *  Created on: June 01, 2018
 *      Author: rtrk
 */

#ifndef TDAL_SSU_PRIV_H_
#define TDAL_SSU_PRIV_H_

#include "tdal_ssu.h"

#define CHAL_VERSION               	"00.01"
#define CHAL_VERSION_ID            	0x0001

#define LOADER_VERSION             	"1.0.0"  //No use
#define LOADER_VERSION_ID          	0x10000  //No use

#define LAUNCHER_VERSION           	"1.0.4"  //No use
#define LAUNCHER_VERSION_ID        	0x10004  //No use

#define HARDWARE_VERSION           	"2223"  //61001
#define HARDWARE_VERSION_ID        	0x2223

#define HARDWARE_MODEL             	"8108"
#define HARDWARE_MODEL_ID          	0x8108

#define HARDWARE_DEVICE_NAME       	"CH8500MUM"   //CA90
#define HARDWARE_DEVICE_NAME_ID    	0xCA80   //1125

#define HARDWARE_MANUFACTURER      	"CHANGHONG"
#define HARDWARE_MANUFACTURER_ID   	0x1449


#define UPDATE_INFO_MAGIC			"Update Info"
#define SYSTEM_INFO_MAGIC			"System Info"
#define LOADER_INFO_MAGIC			"Loader Info"

typedef unsigned int U32;
typedef unsigned short U16;
typedef unsigned char U8;
typedef signed int S32;
typedef signed short S16;
typedef signed char S8;


/**@brief 解调类型定义*/
typedef enum
{
	CH_DEMOD_DVB_S = 0, 				///< DVB-S
	CH_DEMOD_DVB_C,						///< DVB-C
	CH_DEMOD_DVB_T,						///< DVB-T
	CH_DEMOD_DMB_TH,					///< 国标地面波
	CH_DEMOD_DVB_S2 , 					///< DVB-S2
	CH_DEMOD_DVB_C2,					///< DVB-C2
	CH_DEMOD_DVB_T2,					///< DVB-T2
	CH_DEMOD_MAX						///< 错误判断
}CH_DEMOD_TYPE_e;

typedef enum
{
	CHMID_TUNER_QAM_AUTO,   						///< 自动，未定义,不使用
	CHMID_TUNER_QAM_QAM4,    						///<  QAM 4
	CHMID_TUNER_QAM_QAM8,     					///<  QAM 8
	CHMID_TUNER_QAM_QAM16,    					///<  QAM 16
	CHMID_TUNER_QAM_QAM32,    					///<  QAM 32
	CHMID_TUNER_QAM_QAM64,    					///<  QAM 64
	CHMID_TUNER_QAM_QAM128,    					///<  QAM 128
	CHMID_TUNER_QAM_QAM256,    					///<  QAM 256
	CHMID_TUNER_QAM_QAM512,    					///<  QAM 512
	CHMID_TUNER_QAM_QAM1024,    					///<  QAM 1024
	CHMID_TUNER_QAM_MAX		   					///< 非法判断
}CHMID_TUNER_QAM_MODE_e;

/**@brief PLP通道数据结构*/
typedef struct
{
	U32 ui_PLPId;                                   ///< ID
	U32 enm_Type;                ///< 类型
	U32 enm_PayloadType;      ///< 输入流格式
	U32 ui_Reserved;                                ///< 保留值
} CHCOMMON_TUNER_PLP_t;

typedef struct
{
	S8	uc_name[12];			 		///< magic = "Update Info"
	S32 i_length;    					///< 信息大小*/
	CH_DEMOD_TYPE_e enm_AntType;		///< 解调器类型 0 C   1 T   2 S   3 S2
	union
	{
		struct
		{
			U32 ui_FreqKHZ;				///< 频率
			U32 ui_SymbKbps;			///< 符号率
			U8	uc_QAMMode;				///< 调制方式，1:QAM4,2:QAM8….7:QAM256
		}stru_CableFreqInfo;
		struct
		{
			U32 ui_FreqKHZ;				///< 频率
			U32 ui_BandWithKHz;			///< 带宽
			CHCOMMON_TUNER_PLP_t	str_PLP;
		}stru_TerrestrialFreqInfo;
		struct
		{
			U32	ui_LNBHighFreKhz;		///< 本振高频率
			U32	ui_LNBLowFreKhz;		///< 本振低频率
			U8	uc_LNBPower;			///< 天线供电，0：OFF，1：13V, 2:18V
			U8	uc_Polarization;		///< 极化方向：0：垂直；1：水平
			U8	uc_22KType;		  		///< 22k开关，0 Auto 1：开，2：关
			U8	uc_DiseqcVer;			///< DISEQC版本。0:1.0, 1:1.1, 2:1.2, 3:1.3
			U8	uc_DieseqcPort;		  	///< DISEQ1.0端口。0:A; 1:B; 2:C; 3:D
			U8	uc_DieseqcPos;		  	///< DISEQ1.2 位置
			S32	i_SatelliteLongitude;	///< 卫星经度：-1800~1800负数表示西经
			S32	i_LocalLongitude;		///< 本地经度
			S32	i_LocalLatitude;		///< 本地纬度:-900~900 负数表示南纬
			U32	ui_FreqKHz;			  	///< 频率
			U32	ui_SymbKbps;		  	///< 符号率
			U8	uc_QPSKMode;			///< QPSK解调模式：0:BPSK; 1:QPSK; 2:8PSK
			U8	uc_FEC;					///< FEC系数
		}stru_SatelliteFreqInfo;
	}un_FreqInfo;						///< 频点信息
	U16 us_DataPid;						///< 数据PID，由loader确定
	U16	us_DataGid; 					///< 数据table ID，由loader确定
	U8	uc_UpdaeFlag;					///< 升级标记位
	U32 ui_crc;                      	///< CRC信息，不包含uc_name及i_length
}CH_UpdateInfo_t;

/**@brief 存储设备类型*/
typedef enum
{
    CH_COM_SAVEDEVICE_FLASH,		///< 由flash 存储信息
    CH_COM_SAVEDEVICE_E2P,			///< 由E2prom 存储信息
    CH_COM_SAVEDEVICE_MAX
}CH_COM_SAVEDEVICE_e;

/**@brief 存储信息类型*/
typedef enum
{
    CH_COM_INFOTYPE_LOADER,			///< loader信息，包括厂商、软硬件版本号等等
    CH_COM_INFOTYPE_UPDATE,			///< 升级信息，包括升级频点信息等等
    CH_COM_INFOTYPE_SYSTEM,			///< 系统信息，包括序号、MAC等等
    CH_COM_INFOTYPE_FACTBOOT,		///< 工厂调试完毕后设置的启动标志信息
    CH_COM_INFOTYPE_EXTARL,			///< 扩展信息
    CH_COM_INFOTYPE_MAX
}CH_COM_INFOTYPE_e;

typedef struct
{
    CH_COM_SAVEDEVICE_e enm_Device;		///< 存放区域类型
    S32 i_DeviceId;						///< 存储设备号
    U8* puc_MtdName;					///< Linux平台下Flash分区名称，配置成NULL 表示无效
    U32 ui_OffsetAddress;				///< 偏移地址
    U32 ui_Length;						///< 存取区域长度
}CH_COM_Address_t;

/**@brief 信息存储结构体定义*/
typedef struct
{
    CH_COM_INFOTYPE_e enm_InfoType;		///< 存储信息类型，指示当前需要配置的信息
    bool b_Used;						///< 是否配置，指示该配置信息是否有效
    CH_COM_Address_t stru_MainInfo;		///< 主信息保存地址
    bool b_BackUpUsed;				///< 是否使用备份配置，指示该配置信息是否有效
    CH_COM_Address_t stru_BackUpInfo;	///< 备份信息地址
}CH_COM_Params_t;

/**@brief 系统信息*/
typedef struct
{
	S8 	uc_name[12];				///< magic = "System Info"
	S32 i_length;    				///< 信息大小*/
	U8 	uc_STBSN[20]; 				///< 机顶盒序列号（字符串格式）
	U8 	uc_MacAddress[8]; 			///< MAC地址,前6位有效
	U8	uc_HDCPKey[256];			///< HDCP选项数据
	U32	ui_CRC;						///< 校验字
}CH_SystemInfo_t;


/**@brief Loader信息*/
typedef struct
{
	S8 	uc_name[12];			///< magic = "Loader Info"
	S32 i_length;				///< 信息大小
	U32	ui_OperatorNumber;		///< 运营商ID
	U32 ui_LoaderType;			///< loader类型，具体由loader组分配
	U32 ui_Manufacturer;		///< 厂商代码
	U32 ui_Hardware;			///< 硬件版本号
	U32 ui_Software;			///< 软件版本号
	U32 ui_DateCode;			///< 软件日期
	U32 ui_TimeCode;			///< 软件时间
	S32	i_CaSpecailDataOffset;	///< CA私有数据存放地址
	U32 ui_crc;					///< CRC信息
}CH_LoaderInfo_t;

U32 CRC_MPEG32_(U8 * pvStartAddress, U32 ulSize_in_bytes);
U32 CHDRV_FLASH_DirectWrite(U32 Address, U8* Buffer, U32 NumberToWrite);
tTDAL_OTA_ErrorCode CH_COM_SetOTAInfo(CH_UpdateInfo_t *rpstru_OTAInfo);
tTDAL_OTA_ErrorCode CH_COM_SetSystemInfo(CH_SystemInfo_t *rpstru_SystemInfo);
tTDAL_OTA_ErrorCode CH_COM_GetSystemInfo(CH_SystemInfo_t *rpstru_SystemInfo);
tTDAL_OTA_ErrorCode CH_COM_SetLoaderInfo(CH_LoaderInfo_t *rpstru_LoaderInfo);
tTDAL_OTA_ErrorCode CH_COM_GetLoaderInfo(CH_LoaderInfo_t *rpstru_LoaderInfo);

extern const CH_COM_Params_t gstru_fact_SaveParams[CH_COM_INFOTYPE_MAX];
extern unsigned long ulTable_MPEG32[256];

#endif /* TDAL_SSU_PRIV_H_ */
