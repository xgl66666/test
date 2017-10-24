////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
///
/// @file   apiDTVSatTable.h
/// @brief  DVBC Physical Channel Database API
/// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////

#if !defined(__API_DTVSATTABLE_H__)
#define __API_DTVSATTABLE_H__

//#include "MsCommon.h"
//#include "TVAVGlobal.h"
//#include "Board.h"

#define MAX_NUM_OF_SAT 254 //max physical channel
#define MAX_NUM_OF_DTV_SAT 253
#define START_ID_OF_ATV_SAT    MAX_NUM_OF_DTV_SAT
#define END_ID_OF_ATV_SAT    (MAX_NUM_OF_SAT -1)
#define MAX_SATNAME_LEN 32
#define MAX_SATTABLEMAP  ((MAX_NUM_OF_SAT+7)/8)
// Const SAT Info BuildIn
#define BAND_C    0
#define BAND_KU  1
#if 0//ATV_ENABLE
#define BAND_ATV_C  2
#define BAND_ATV_KU  3
#endif

#if(MS_BOARD_TYPE_SEL == BD_MST098E_CUS36_01B)
typedef enum _SATELLITE
{
      C_2950_STARTONE_C1,
      C_2900_STARTONE_C2,
      C_2850_BRASILSAT_B3,
      C_3195_NSS_806,
  #if 0//ATV_ENABLE
      ATV_SAT_START,
      ATV_2900_STARTONE_C2=ATV_SAT_START,
      ATV_SAT_END,
  #endif
}TYPE_SATELLITE;
#else
typedef enum _SATELLITE
{
       KU_70_EUTELSAT_W3A,
       KU_100_EUTELSAT_W1,
       KU_130_HOTBIRD_6_7A_8,
       KU_160_EUTELSAT_W2,
       KU_192_ASTRA_1H_1KR_1L_1M,
       KU_235_ASTRA_1E_1G_3A,
       KU_260_BADR_2_3_4_EURO,
       KU_282_ASTRA_2A_2B_2D,
       KU_305_ARABSAT_2B,
       KU_360_EUTELSAT_SESAT,
       KU_390_HELLAS_SAT_2,
       KU_400_EXPRESS_AM1,
       KU_420_TURKSAT_2A_3A,
       KU_620_INTELSAT_920,
       C_686_INTELSAT_7_10,
       KU_703_NSS_703,
       KU_765_TELSTAR_10,
       C_765_TELSTAR_10,
       KU_785_THAICOM_2_5,
      C_785_THAICOM_2_5,
      KU_830_INSAT_3B_4A,
      KU_900_YAMAL_210,
      KU_950_NSS_6,
      C_1005_ASIASAT_2,
      KU_1005_ASIASAT_2,
      C_1055_ASIASAT_3S,
      KU_1055_ASIASAT_3S,
      C_1155_CHINASAT_6B,
      C_1250_SINOSAT_3,
      C_1340_APSTAR_6,
      KU_1340_APSTAR_6,
      KU_1380_TELSTAR_18,
      C_1660_INTELSAT_8,
      KU_1660_INTELSAT_8,
      C_1690_INTELSAT_2,
      KU_1690_INTELSAT_2,
      C_2330_HORIZONS_1,
      KU_2330_HORIZONS_1,
      KU_2370_GALAXY_10R,
      C_2370_GALAXY_10R,
      C_2390_GALAXY_23,
      KU_2630_GALAXY_25,
      C_2650_GALAXY_3C,
      KU_2650_GALAXY_3C,
      C_2690_GALAXY_11,
      KU_2690_GALAXY_11,
      KU_3020_INTELSAT_9,
      C_3020_INTELSAT_9,
      KU_3070_INTELSAT_707,
      KU_3300_HISPASAT_1C_1D,
      KU_3325_INTELSAT_907,
      KU_3380_NSS_7,
      KU_3450_TELSTAR_12,
      KU_3475_ATLANTIC_BIRD_1,
      KU_3490_EXPRESS_A3,
      KU_3520_ATLANTIC_BIRD_2,
      KU_3530_NILESAT_101_10,
      KU_3560_AMOS_2_3,
#if 0//ATV_ENABLE
      ATV_SAT_START,
      ANOLOGSAT1=ATV_SAT_START,
      ATV_SAT_END,
#endif
}TYPE_SATELLITE;
#endif

//diseqc
typedef enum
{
    EN_DISEQC_LVL_OFF,
    EN_DISEQC_LVL_10,
    EN_DISEQC_LVL_11,
    EN_DISEQC_LVL_12,
    EN_DISEQC_LVL_NUM,
    EN_DISEQC_LVL_13,
    EN_DISEQC_LVL_20,
    EN_DISEQC_LVL_MULTI
}MS_EN_DISEQC_LEVEL;

typedef enum
{
    EN_SWT_PORT_OFF,
    EN_SWT_PORT_1,
    EN_SWT_PORT_2,
    EN_SWT_PORT_3,
    EN_SWT_PORT_4,
    EN_SWT_PORT_5,
    EN_SWT_PORT_6,
    EN_SWT_PORT_7,
    EN_SWT_PORT_8,
    EN_SWT_PORT_9,
    EN_SWT_PORT_10,
    EN_SWT_PORT_11,
    EN_SWT_PORT_12,
    EN_SWT_PORT_13,
    EN_SWT_PORT_14,
    EN_SWT_PORT_15,
    EN_SWT_PORT_16,
    EN_SWT_PORT_NUM,
    EN_SWT_DISEQC10_PORT_NUM = EN_SWT_PORT_5,
    EN_SWT_DISEQC11_PORT_NUM = EN_SWT_PORT_NUM,
}MS_EN_SWT_PORT;

typedef enum
{
    EN_TONEBURST_NONE,
    EN_TONEBURST_0,
    EN_TONEBURST_1,
    EN_TONEBURST_TYPE_NUM
}MS_EN_TONEBURST_TYPE;

typedef enum
{
    EN_22K_OFF,
    EN_22K_ON,
    EN_22K_AUTO,
    EN_22K_ONOFF_NUM
}MS_EN_22K_ONOFF;

typedef enum
{
    EN_LNBPWR_OFF,
    EN_LNBPWR_13OR18V,
    EN_LNBPWR_13V,
    EN_LNBPWR_18V,
    EN_LNBPWR_ONOFF_NUM
}MS_EN_LNBPWR_ONOFF;

typedef enum
{
    EN_0V12V_ONOFF_OFF,
    EN_0V12V_ONOFF_ON,
    EN_0V12V_ONOFF_NUM
}MS_EN_0V12V_ONOFF;

typedef enum
{
    EN_LNBTYPE_C,
    EN_LNBTYPE_KU,
#if 0//ATV_ENABLE
    EN_LNBTYPE_ATV_C,
    EN_LNBTYPE_ATV_KU,
#endif
    EN_LNBTYPE_2LOF,
    EN_LNBTYPE_NUM
}MS_EN_LNBTYPE;

typedef enum
{
    EN_BWTYPE_HALF,
    EN_BWTYPE_FULL,
    EN_BWTYPE_NUM
}MS_EN_BWTYPE;

typedef struct
{
    MS_U8* Location;
    MS_S16 MyLongitude;
    MS_S16 MyLatitude;
}DISH_LOCATION;

typedef struct
{
       TYPE_SATELLITE eSatellite;
       MS_U16 u16Band:2;//0:C  1:Ku 2:ATV c 3:ATV ku
	MS_U16 u16Angle:14;
	MS_U8 *pSatName;

}Antenna_SatInfo;
typedef struct
{
      TYPE_SATELLITE eSatellite;
	MS_U32 u32Frequency;
	MS_U16 u16SymbolRate;  //Bw Type
	MS_U8   u8Polarity;
}Antenna_TPInfo;

extern Antenna_SatInfo const AntennaSatList_BuiltIn[];
extern Antenna_TPInfo const AntennaTpList_BuiltIn[];

extern DISH_LOCATION SetSATLocation[];

#define TOTAL_BUILDIN_SATELLITE_NUMBER         (sizeof(AntennaSatList_BuiltIn)/sizeof(Antenna_SatInfo))			// (47)
#define TOTAL_BUILDIN_TRANSPONDER_NUMBER      (sizeof(AntennaTpList_BuiltIn)/sizeof(Antenna_TPInfo)) // 1269

typedef struct
{
    MS_U8 aSatName[MAX_SATNAME_LEN+1]; // Sat Name
    MS_U16 u16LoLOF; // low LOF value,unit MHz
    MS_U16 u16HiLOF; // high LOF value,unit MHz
    MS_EN_LNBTYPE eLNBType;// Single LOF or Double LOF
    MS_EN_DISEQC_LEVEL   eDiseqcLevel;
    MS_EN_TONEBURST_TYPE eToneburstType;//
    MS_EN_SWT_PORT  eSwt10Port; //
    MS_EN_SWT_PORT  eSwt11Port; //
    MS_EN_22K_ONOFF e22KOnOff;
    MS_EN_LNBPWR_ONOFF eLNBPwrOnOff;
    MS_EN_0V12V_ONOFF e0V12VOnOff; //@FIXME: is it used?
    MS_U8 u8Position; // Motor position  bit8 1:USALS 0:DISEQC1.2
    MS_U16 u16Angle; // angle
    MS_U8 bPorInvert;
}MS_SAT_PARAM;

#define DISEQC12_USALS_POSITION_NUM   0x40
#define DISEQC12_USALS_POSITION_MASK  0x3F
#define DISEQC_USALS_BIT    _BIT7
#define IsUSALS(x)   (x.u8Position&DISEQC_USALS_BIT)
//****************************************************************************
// Public attributes.
//****************************************************************************
#define INVALID_LOF        0xFFFF
#define INVALID_SAT_POSITION    0xFF
#define INVALID_LNBTYPE        0xFF
#define INVALID_0V12VONOFF        0xFF
#define INVALID_LNBPWRONOFF        0xFF
#define INVALID_22KONOFF        0xFF
#define INVALID_SWTPORT        0xFF
#define INVALID_DISEQCLEVEL        0xFF
#define INVALID_SATINDEX         0xFF
#define INVALID_SATANGLE         0xFFFF

typedef MS_U8    eSATDATA_MEMBER;
//----------------------------------
#define pftSATNAME        0x01
#define pftLOLOF    0x02
#define pftHILOF       0x03
#define pftLNBTYPE           0x04
#define pftDISEQCLEVEL        0x05
#define pftSWT10PORT      0x06
#define pftSWT11PORT      0x07
#define pft22KONOFF  0x08
#define pftSATALL              0x09
#define pftLNBPWRONOFF	0x0a
#define pft0V12VONOFF    	0x0b
#define pftPOSITION    	0x0c
#define pftANGLE               0x0d
#define pftPORTYPE               0x0e


typedef struct
{
    MS_U8                  acSATTableMap[MAX_SATTABLEMAP];
    MS_SAT_PARAM   astSATTable[MAX_NUM_OF_SAT];
} SAT_DATA_STRUCTURE;

#define DBMemAlignWithUnit(n, unit)           ((((n)+(unit)-1)/(unit))*(unit))

#define START_ADDRESS_OF_SAT_DATA_STRUCTURE  DBMemAlignWithUnit(RM_CH_DB_SETTING_START_ADR, 4)

#define BASEADDRESS_SAT_DATA                              START_ADDRESS_OF_SAT_DATA_STRUCTURE ///TODO:: move to the position in front of channel database
#define BASEADDRESS_SAT_TABLEMAP                     ( BASEADDRESS_SAT_DATA + (offsetof(SAT_DATA_STRUCTURE, acSATTableMap)) )
#define BASEADDRESS_SAT_TABLE                        ( BASEADDRESS_SAT_DATA + (offsetof(SAT_DATA_STRUCTURE, astSATTable)) )
#define END_ADDRESS_OF_SAT_DATA_STRUCTURE    (BASEADDRESS_SAT_DATA + (sizeof(SAT_DATA_STRUCTURE)))


//eOP_RESULT MApi_DB_SAT_SetCurrentSATPosition(MS_U8 bPosition);
MS_U8 MApi_DB_SAT_GetCurrentSATPosition(void);
MS_U8 MApi_DB_SAT_GetDtvSATCount(void);
//eOP_RESULT MApi_DB_SAT_AddSAT(MS_U8 *pcSATIdx, MS_SAT_PARAM *pSATParam);
//eOP_RESULT MApi_DB_SAT_UpdateSAT(MS_U8 cSATIdx, MS_SAT_PARAM *pSATParam);
//eOP_RESULT MApi_DB_SAT_DeleteSAT(MS_U8 cSATIdx);
//eOP_RESULT MApi_DB_SAT_DeleteAllSAT(void);
//eOP_RESULT MApi_DB_SAT_GetSATParam(MS_U8 cSATIdx, MS_SAT_PARAM *pSATParam);
MS_U8 MApi_DB_SAT_GetActiveSatIndexBySatID(MS_U8 bSatID);
MS_U8 MApi_DB_SAT_GetSatIDByIndexOfActiveSats(MS_U8 bIndexofactivesats);
MS_U8 MApi_DB_SAT_GetSATMotorPositionBySatID(MS_U8 bSatID);
//eOP_RESULT MApi_DB_SAT_GetSATName(MS_U8 cSATIdx,MS_U8 *pcSATName);
MS_U16 MApi_DB_SAT_GetLoLOF(MS_U8 cSATIdx);
MS_U16 MApi_DB_SAT_GetHiLOF(MS_U8 cSATIdx);
MS_EN_LNBTYPE MApi_DB_SAT_GetLNBType(MS_U8 cSATIdx);
MS_EN_DISEQC_LEVEL MApi_DB_SAT_GetDisqecLevel(MS_U8 cSATIdx);
MS_EN_SWT_PORT MApi_DB_SAT_GetSWT10Port(MS_U8 cSATIdx);
MS_EN_SWT_PORT MApi_DB_SAT_GetSWT11Port(MS_U8 bSATIdx);
MS_EN_22K_ONOFF MApi_DB_SAT_Get22KOnOff(MS_U8 cSATIdx);
MS_EN_LNBPWR_ONOFF MApi_DB_SAT_GetLNBPWROnOff(MS_U8 cSATIdx);
MS_EN_0V12V_ONOFF MApi_DB_SAT_Get0V12VOnOff(MS_U8 cSATIdx);
MS_U16 MApi_DB_SAT_GetSATAngleBySatID(MS_U8 bSatID);
MS_U8 MApi_DB_GetSatIdByAngle(MS_U16 u16Angle,MS_U8 u8LNBType);
//MS_U8 MApi_DB_SAT_GetPosition(MS_U8 cSATIdx);
//eOP_RESULT MApi_DB_SAT_InitSATTable(void);
//eOP_RESULT MApi_DB_SAT_ResetAllSATTable(void);

//for satellite selected
MS_BOOL MApi_SAT_IsSatEntirySelect(MS_U8 ucSatID);
//eOP_RESULT MApi_SAT_SelectSATEntity(MS_U8 ucSatID, MS_BOOL bSelected);
//eOP_RESULT MApi_SAT_UnSelectAllSAT(void);
MS_U8  MApi_SAT_SelectedSatNum(void);

MS_U16 MApi_GetLocationNum(void);

#endif // __API_DTVFREQTABLE_H__

