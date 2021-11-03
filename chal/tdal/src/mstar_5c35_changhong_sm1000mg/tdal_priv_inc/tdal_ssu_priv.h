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


/**@brief ������Ͷ���*/
typedef enum
{
	CH_DEMOD_DVB_S = 0, 				///< DVB-S
	CH_DEMOD_DVB_C,						///< DVB-C
	CH_DEMOD_DVB_T,						///< DVB-T
	CH_DEMOD_DMB_TH,					///< ������沨
	CH_DEMOD_DVB_S2 , 					///< DVB-S2
	CH_DEMOD_DVB_C2,					///< DVB-C2
	CH_DEMOD_DVB_T2,					///< DVB-T2
	CH_DEMOD_MAX						///< �����ж�
}CH_DEMOD_TYPE_e;

typedef enum
{
	CHMID_TUNER_QAM_AUTO,   						///< �Զ���δ����,��ʹ��
	CHMID_TUNER_QAM_QAM4,    						///<  QAM 4
	CHMID_TUNER_QAM_QAM8,     					///<  QAM 8
	CHMID_TUNER_QAM_QAM16,    					///<  QAM 16
	CHMID_TUNER_QAM_QAM32,    					///<  QAM 32
	CHMID_TUNER_QAM_QAM64,    					///<  QAM 64
	CHMID_TUNER_QAM_QAM128,    					///<  QAM 128
	CHMID_TUNER_QAM_QAM256,    					///<  QAM 256
	CHMID_TUNER_QAM_QAM512,    					///<  QAM 512
	CHMID_TUNER_QAM_QAM1024,    					///<  QAM 1024
	CHMID_TUNER_QAM_MAX		   					///< �Ƿ��ж�
}CHMID_TUNER_QAM_MODE_e;

/**@brief PLPͨ�����ݽṹ*/
typedef struct
{
	U32 ui_PLPId;                                   ///< ID
	U32 enm_Type;                ///< ����
	U32 enm_PayloadType;      ///< ��������ʽ
	U32 ui_Reserved;                                ///< ����ֵ
} CHCOMMON_TUNER_PLP_t;

typedef struct
{
	S8	uc_name[12];			 		///< magic = "Update Info"
	S32 i_length;    					///< ��Ϣ��С*/
	CH_DEMOD_TYPE_e enm_AntType;		///< ��������� 0 C   1 T   2 S   3 S2
	union
	{
		struct
		{
			U32 ui_FreqKHZ;				///< Ƶ��
			U32 ui_SymbKbps;			///< ������
			U8	uc_QAMMode;				///< ���Ʒ�ʽ��1:QAM4,2:QAM8��.7:QAM256
		}stru_CableFreqInfo;
		struct
		{
			U32 ui_FreqKHZ;				///< Ƶ��
			U32 ui_BandWithKHz;			///< ����
			CHCOMMON_TUNER_PLP_t	str_PLP;
		}stru_TerrestrialFreqInfo;
		struct
		{
			U32	ui_LNBHighFreKhz;		///< �����Ƶ��
			U32	ui_LNBLowFreKhz;		///< �����Ƶ��
			U8	uc_LNBPower;			///< ���߹��磬0��OFF��1��13V, 2:18V
			U8	uc_Polarization;		///< ��������0����ֱ��1��ˮƽ
			U8	uc_22KType;		  		///< 22k���أ�0 Auto 1������2����
			U8	uc_DiseqcVer;			///< DISEQC�汾��0:1.0, 1:1.1, 2:1.2, 3:1.3
			U8	uc_DieseqcPort;		  	///< DISEQ1.0�˿ڡ�0:A; 1:B; 2:C; 3:D
			U8	uc_DieseqcPos;		  	///< DISEQ1.2 λ��
			S32	i_SatelliteLongitude;	///< ���Ǿ��ȣ�-1800~1800������ʾ����
			S32	i_LocalLongitude;		///< ���ؾ���
			S32	i_LocalLatitude;		///< ����γ��:-900~900 ������ʾ��γ
			U32	ui_FreqKHz;			  	///< Ƶ��
			U32	ui_SymbKbps;		  	///< ������
			U8	uc_QPSKMode;			///< QPSK���ģʽ��0:BPSK; 1:QPSK; 2:8PSK
			U8	uc_FEC;					///< FECϵ��
		}stru_SatelliteFreqInfo;
	}un_FreqInfo;						///< Ƶ����Ϣ
	U16 us_DataPid;						///< ����PID����loaderȷ��
	U16	us_DataGid; 					///< ����table ID����loaderȷ��
	U8	uc_UpdaeFlag;					///< �������λ
	U32 ui_crc;                      	///< CRC��Ϣ��������uc_name��i_length
}CH_UpdateInfo_t;

/**@brief �洢�豸����*/
typedef enum
{
    CH_COM_SAVEDEVICE_FLASH,		///< ��flash �洢��Ϣ
    CH_COM_SAVEDEVICE_E2P,			///< ��E2prom �洢��Ϣ
    CH_COM_SAVEDEVICE_MAX
}CH_COM_SAVEDEVICE_e;

/**@brief �洢��Ϣ����*/
typedef enum
{
    CH_COM_INFOTYPE_LOADER,			///< loader��Ϣ���������̡���Ӳ���汾�ŵȵ�
    CH_COM_INFOTYPE_UPDATE,			///< ������Ϣ����������Ƶ����Ϣ�ȵ�
    CH_COM_INFOTYPE_SYSTEM,			///< ϵͳ��Ϣ��������š�MAC�ȵ�
    CH_COM_INFOTYPE_FACTBOOT,		///< ����������Ϻ����õ�������־��Ϣ
    CH_COM_INFOTYPE_EXTARL,			///< ��չ��Ϣ
    CH_COM_INFOTYPE_MAX
}CH_COM_INFOTYPE_e;

typedef struct
{
    CH_COM_SAVEDEVICE_e enm_Device;		///< �����������
    S32 i_DeviceId;						///< �洢�豸��
    U8* puc_MtdName;					///< Linuxƽ̨��Flash�������ƣ����ó�NULL ��ʾ��Ч
    U32 ui_OffsetAddress;				///< ƫ�Ƶ�ַ
    U32 ui_Length;						///< ��ȡ���򳤶�
}CH_COM_Address_t;

/**@brief ��Ϣ�洢�ṹ�嶨��*/
typedef struct
{
    CH_COM_INFOTYPE_e enm_InfoType;		///< �洢��Ϣ���ͣ�ָʾ��ǰ��Ҫ���õ���Ϣ
    bool b_Used;						///< �Ƿ����ã�ָʾ��������Ϣ�Ƿ���Ч
    CH_COM_Address_t stru_MainInfo;		///< ����Ϣ�����ַ
    bool b_BackUpUsed;				///< �Ƿ�ʹ�ñ������ã�ָʾ��������Ϣ�Ƿ���Ч
    CH_COM_Address_t stru_BackUpInfo;	///< ������Ϣ��ַ
}CH_COM_Params_t;

/**@brief ϵͳ��Ϣ*/
typedef struct
{
	S8 	uc_name[12];				///< magic = "System Info"
	S32 i_length;    				///< ��Ϣ��С*/
	U8 	uc_STBSN[20]; 				///< ���������кţ��ַ�����ʽ��
	U8 	uc_MacAddress[8]; 			///< MAC��ַ,ǰ6λ��Ч
	U8	uc_HDCPKey[256];			///< HDCPѡ������
	U32	ui_CRC;						///< У����
}CH_SystemInfo_t;


/**@brief Loader��Ϣ*/
typedef struct
{
	S8 	uc_name[12];			///< magic = "Loader Info"
	S32 i_length;				///< ��Ϣ��С
	U32	ui_OperatorNumber;		///< ��Ӫ��ID
	U32 ui_LoaderType;			///< loader���ͣ�������loader�����
	U32 ui_Manufacturer;		///< ���̴���
	U32 ui_Hardware;			///< Ӳ���汾��
	U32 ui_Software;			///< ����汾��
	U32 ui_DateCode;			///< �������
	U32 ui_TimeCode;			///< ���ʱ��
	S32	i_CaSpecailDataOffset;	///< CA˽�����ݴ�ŵ�ַ
	U32 ui_crc;					///< CRC��Ϣ
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
