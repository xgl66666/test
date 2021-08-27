////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <config.h>
#include <configs/uboot_board_config.h>
#include <sys/common/MsTypes.h>
#include <sys/common/MsIRQ.h>
#include <sys/common/MsOS.h>
#include <sys/api/apiGOP.h>
#include <sys/api/apiGFX.h>
#include <sys/drv/drvSEM.h>
#include <sys/drv/drvGPIO.h>
#include <sys/drv/drvIIC.h>
#include <sys/api/apiDMX.h>
#include <configs/uboot_module_config.h>
#include "msAPI_demux.h"
#include "msAPI_oad_process.h"
#include "msAPI_oad_parser.h"
#include "msAPI_Memory.h"
#include "msAPI_Tuner.h"
#include <linux/string.h>
#include <asm/string.h>
#include "uboot_mmap.h"
#include "frontend_config.h"
#if (ENABLE_HUMMINGBIRD_CUSTOMER)

#include "cus_oad/msAPI_oad_scan.h"
#include "cus_oad/apiRfChannel.h"
#endif 
#if (CONFIG_COMMANDS & CONFIG_OAD_IN_MBOOT)
#define ENABLE_OAD 1
#define ENABLE_BL_OAD_SCAN 0
#if (CONFIG_COMMANDS & CONFIG_DISPLAY_OSD)
#define ENABLE_BL_UI 1
#define GWIN_WIDTH          720
#define GWIN_HEIGHT         576
#define GRAPHIC_WIDTH       600
#define GRAPHIC_HEIGHT      400
#define GRAPHIC_X           60
#define GRAPHIC_Y           88
#else
#define ENABLE_BL_UI 0
#endif
#define DBG_PRINT(x)    //x
#define TUNER_LOCK_TIME_OUT     5000//ms
#define MONITOR_DSI_TIME_OUT    60000//ms
#define MONITOR_DII_TIME_OUT    60000//ms
#define MONITOR_DDB_TIME_OUT    60000*1//ms
#define FE_NOT_LOCK     0
#define FE_LOCK         1
#define FE_AGC_NOT_LOCK 2
#define BUF_SIZE 4096

#if ATSC_SDDS_ENABLE
#define CUSTOMER_OUI            0x000214    //SeedSystem's OUI.    0x0014B9    // MStar OUI
#define HW_MODEL                0x2000      //  SeedSystem's HW MODEL.   0x01        // HW Model
#define HW_VERSION              0x01        // HW Version
#define BL_SW_MODEL             0x01        // SW Model of Bootloader
#define BL_SW_VERSION           0x01        // SW Version of of Bootloader
#define AP_SW_MODEL             0x2000      //  SeedSystem's SW MODEL.   0x0001        // SW Model of Application
#define AP_SW_VERSION           0x0001        // SW Version of of Application

#define VERSION_CHECK 1
#define DATA_BC_ID_SSU          0x000A // DVB-SSU
#define DATA_BC_ID_UK_EC        0x0111 // UK Engineering Channel
#define MAX_DDB_MSGSIZE 4066
#define HW_MODEL_VER_CHECK      1       //1: Enable th HW model cersion check; 0: Disable, original setting.
#else
// OAD info
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
//#define CUSTOMER_OUI            0x0014B9    // MStar OUI
#define CUSTOMER_OUI            0x00169B    // MStar OUI

#else
#define CUSTOMER_OUI            0x0014B9    // MStar OUI
#endif
#define HW_MODEL                0x01        // HW Model
#define HW_VERSION              0x01        // HW Version
#define BL_SW_MODEL             0x01        // SW Model of Bootloader
#define BL_SW_VERSION           0x01        // SW Version of of Bootloader
#define AP_SW_MODEL             0x0001        // SW Model of Application
#define AP_SW_VERSION           0x0001        // SW Version of of Application

#define VERSION_CHECK 1
#define DATA_BC_ID_SSU          0x000A // DVB-SSU
#define DATA_BC_ID_UK_EC        0x0111 // UK Engineering Channel
#define MAX_DDB_MSGSIZE 4066
#define HW_MODEL_VER_CHECK      0
#endif

//buffer address
#define DOWNLOAD_BUFFER_ADR         UPGRADE_BUFFER_ADDR
#define DOWNLOAD_BUFFER_LEN_OAD     0x0003200000
#define DOWNLOAD_BUFFER_ADR_OAD     DOWNLOAD_BUFFER_ADR
//#define OADSEC_BUFFER_AVAILABLE                                0x0001B00000
#define OADSEC_BUFFER_ADR                                      0x0001B00000  //Alignment 0x00010
#define OADSEC_BUFFER_GAP_CHK                                  0x0000000000
#define OADSEC_BUFFER_LEN                                      0x0000014000
//#define OADSEC_BUFFER_MEMORY_TYPE                              (MIU0 | SW | WRITE_BACK)

#define DMX_BUF_SIZE_MONITOR    0x4000
#define DMX_BUF_SIZE_DOWNLOAD   0x4000*4

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

static U16 wOad_PID = 0;
static U32 u32Frequency = 0;
static U8 u8BandWidth = 0;
static U32 tsVersion=0;
static U32 U32DOWNLOADSIZE=0;
static U8  _u8OADMonitorFID=MSAPI_DMX_INVALID_FLT_ID;
static U8  _u8OADDownloadFID=MSAPI_DMX_INVALID_FLT_ID;
U8 au8MonitorSection[4096];
U8 au8DownloadSection[4096];
static U32 u32DownloadSize = 0x0;
static U8 versionNum = 0xff ;
static U32 u32ModuleAddr=0;
U32 u32OadbufMonitorAddr = NULL;
U32 u32OadbufMonitorSize = NULL;
U32 u32OadbufDownloadAddr = NULL;
U32 u32OadbufDownloadSize = NULL;

extern MS_U32 msAPI_Timer_GetTime0(void);
extern MS_U32 msAPI_Timer_DiffTimeFromNow(MS_U32 u32TaskTimer);
extern BOOLEAN msAPI_Tuner_CheckLock(BOOLEAN *CheckLockResult,BOOLEAN fScan);


#if (ENABLE_BL_UI)
void MApp_BL_DisplaySystem(U8 u8Percent);
void MApp_BL_DisplaySystem_clear(void);
void MApp_BL_DisplaySystem_setStatus(S8 *status);

void MApp_BL_DisplaySystem(U8 u8Percent)
{
    char* buffer=NULL;
    int ret=0;

    buffer=(char *)malloc(BUF_SIZE);
    if(buffer != NULL)
    {
        memset(buffer, 0 , 256);
        sprintf(buffer, "draw_rect %d %d %d %d 000000ff", GRAPHIC_X, GRAPHIC_Y+300, GRAPHIC_WIDTH, GRAPHIC_HEIGHT-300);
        ret=run_command(buffer, 0);

        memset(buffer, 0 , 256);
        sprintf(buffer, "draw_progress %d %d ffffffff %u", GRAPHIC_X+60, GRAPHIC_Y+300, u8Percent);
        ret=run_command(buffer, 0);

        free(buffer);
    }
    else
    {
        printf("%s: malloc() failed, at %d\n", __func__, __LINE__);
    }
}

void MApp_BL_DisplaySystem_clear(void)
{
    char* buffer=NULL;
    int ret=0;

    buffer=(char *)malloc(BUF_SIZE);
    if(buffer != NULL)
    {
        memset(buffer, 0 , 256);
        sprintf(buffer, "draw_rect %d %d %d %d 000000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
        ret=run_command(buffer, 0);

        memset(buffer, 0 , 256);
        sprintf(buffer, "draw_string %d %d ffffffff 1 '%s'", GRAPHIC_X, GRAPHIC_Y+100, "PLEASE_DO_NOT_POWER_OFF");
        ret=run_command(buffer, 0);

        free(buffer);
    }
    else
    {
        printf("%s: malloc() failed, at %d\n", __func__, __LINE__);
    }
}

void MApp_BL_DisplaySystem_setStatus(S8 *status)
{
    char* buffer=NULL;
    int ret=0;

    buffer=(char *)malloc(BUF_SIZE);
    if(buffer != NULL)
    {
        memset(buffer, 0 , 256);
        sprintf(buffer, "draw_string %d %d ffffffff 1 '%s'", GRAPHIC_X, GRAPHIC_Y+200, status);
        ret=run_command(buffer, 0);
        free(buffer);
    }
    else
    {
        printf("%s: malloc() failed, at %d\n", __func__, __LINE__);
    }
}
#else //(ENABLE_BL_UI)
void MApp_BL_DisplaySystem(U8 u8Percent){UNUSED(u8Percent);}
void MApp_BL_DisplaySystem_clear(void){}
void MApp_BL_DisplaySystem_setStatus(S8 *status){UNUSED(status);}
#endif //(ENABLE_BL_UI)

int oad_check(void);
int oad_check()
{
    U8 *pu8Buf = NULL;
    OAD_STATUS_REPORT_TYPE ddbStatus ;
    U16 blockID=0,msgLen=0,msgType=0 ;
    U32 msgAddr = 0 ;
    U32 u32DsmccTimer=0;
    char env_buf [12];

    DBG_PRINT( printf("%s: at %d\n", __func__, __LINE__) );
#if ((CONFIG_COMMANDS&(CFG_CMD_TUNER | CFG_CMD_DEMOD  | CFG_CMD_DEMOCODE)) == (CFG_CMD_TUNER | CFG_CMD_DEMOD | CFG_CMD_DEMOCODE))
    MS_BOOL Check_FE_Result;

    u32DsmccTimer = msAPI_Timer_GetTime0();
    while(1)
    {
        if( msAPI_Tuner_CheckLock( &Check_FE_Result,FALSE) == FALSE )
        {
            printf("FE Fail!\n");
        }
        else
        {
            if(Check_FE_Result==FE_LOCK)
            {
                printf("FE Locked.\n");
                break;
            }
            else
            {
                printf("FE Not Locked. Check_FE_Result = %d\n", Check_FE_Result);
            }
        }
        if (msAPI_Timer_DiffTimeFromNow(u32DsmccTimer) > TUNER_LOCK_TIME_OUT)
        {
            OAD_DEBUG( printf("Error> Exit DSI\n") );
            return 1; //return to do USB download
        }
        MsOS_DelayTask(500);
    }
#endif
    if(msAPI_OAD_CreateSectionFilter( MSAPI_DMX_FILTER_TYPE_OAD_MONITOR,OAD_CONTROL_SECTION_TYPE,OAD_DSMCC_MSGID_DSI,
            wOad_PID, &_u8OADMonitorFID ) == TRUE)
    {
        DBG_PRINT( printf("Create filter: OK, pid: 0x%x. OADMonitorFID[0x%x]. \n", wOad_PID, _u8OADMonitorFID) );
    }

    DBG_PRINT( printf("Line: %d\n", __LINE__) );

    u32DsmccTimer = msAPI_Timer_GetTime0();
    while(1)
    {
        pu8Buf = msAPI_OAD_PollingSectionFilter(_u8OADMonitorFID, au8MonitorSection);

        if(pu8Buf)
        {
            DBG_PRINT( printf("DSI Poll filter: OK.\n") );

            if (OAD_SUCCEED==msAPI_OAD_ProcessDSI(pu8Buf))
            {
                u32DsmccTimer = msAPI_Timer_GetTime0();
                versionNum = msAPI_OAD_GetDsiVersionNum() ;

                if (versionNum!=msAPI_OAD_GetDsiVersionNum())
                {
                    printf("versionNum error !!!\n");
                }
                break;
            }
            else
            {
                printf("process DSI error !!!\n");
            }
        }
        else
        {
            printf("No DSI Data!\n");
        }

        if (msAPI_Timer_DiffTimeFromNow(u32DsmccTimer) > MONITOR_DSI_TIME_OUT)
        {
            OAD_DEBUG( printf("Error> Exit DSI\n") );
            return 1;
        }
        MsOS_DelayTask(100);
    }

    if(msAPI_OAD_CreateSectionFilter( MSAPI_DMX_FILTER_TYPE_OAD_DOWNLOAD,OAD_CONTROL_SECTION_TYPE,OAD_DSMCC_MSGID_DII,
            wOad_PID, &_u8OADDownloadFID) == TRUE)
    {
        DBG_PRINT( printf("\n") );
        DBG_PRINT( printf("DII Create filter: OK.\n") );
        DBG_PRINT( printf("DII _u8OADDownloadFID = %d\n", _u8OADDownloadFID) );
        u32DsmccTimer = msAPI_Timer_GetTime0();
    }
    else
    {
        printf("Error> msAPI_OAD_CreateSectionFilter(OAD_CONTROL_SECTION_TYPE) : Invalid FID=%x\n",_u8OADDownloadFID);
        return 1;
    }

    while(1)
    {
        pu8Buf = msAPI_OAD_PollingSectionFilter(_u8OADDownloadFID, au8DownloadSection);
        if(pu8Buf)
        {
            if(OAD_SUCCEED==msAPI_OAD_ProcessDII(pu8Buf))
            {
                msAPI_OAD_DeleteSectionFilter(&_u8OADDownloadFID) ;
                DBG_PRINT( printf("process DII ok\n") );
                break;
            }

            if (msAPI_Timer_DiffTimeFromNow(u32DsmccTimer) > MONITOR_DII_TIME_OUT)
            {
                printf("Error> Timeout to receive DII\n");
                return 1;
            }
        }
    }

    if(msAPI_OAD_CreateSectionFilter( MSAPI_DMX_FILTER_TYPE_OAD_DOWNLOAD,OAD_DATA_SECTION_TYPE ,OAD_DSMCC_MSGID_DDB,
                wOad_PID,&_u8OADDownloadFID) == TRUE)
    {
        DBG_PRINT( printf("\n") );
        DBG_PRINT( printf("DDB Create filter: OK.\n") );
        u32DsmccTimer = msAPI_Timer_GetTime0();
    }
    else
    {
        printf("Error> msAPI_OAD_CreateSectionFilter(OAD_DATA_SECTION_TYPE) : Invalid FID\n");
    }

    MApp_BL_DisplaySystem_clear(); //clear screen
    MApp_BL_DisplaySystem_setStatus((S8*)"DOWNLOAD");
    MApp_BL_DisplaySystem(0);

    while(1)
    {
//        msAPI_Timer_ResetWDT();
        pu8Buf = msAPI_OAD_PollingSectionFilter(_u8OADDownloadFID, au8DownloadSection);

        if(pu8Buf)
        {
            ddbStatus = msAPI_OAD_ProcessDDB(pu8Buf,&blockID,&msgLen,&msgAddr,&msgType) ;

            if (ddbStatus != OAD_FAIL)
            {
                u32DsmccTimer = msAPI_Timer_GetTime0();
                if(msgLen>0)
                {
                    if (msgType==OAD_MODE_OBJCAR)
                    {
                        if (blockID==0)
                        {
                           memcpy((U8*)MS_PA2KSEG1(MS_VA2PA(DOWNLOAD_BUFFER_ADR_OAD)),(U8*)msgAddr,(U32)msgLen) ;
                        }
                        else
                        {
                           memcpy((U8*)MS_PA2KSEG1(MS_VA2PA(DOWNLOAD_BUFFER_ADR_OAD+(blockID*MAX_DDB_MSGSIZE)-BIOP_HEADER_LEN)),(U8*)msgAddr,(U32)msgLen) ;
                        }
                    }
                    else if (msgType==OAD_MODE_DATACAR)
                    {
                        memcpy((U8*)MS_PA2KSEG1(MS_VA2PA(DOWNLOAD_BUFFER_ADR_OAD+(blockID*MAX_DDB_MSGSIZE))),(U8*)msgAddr,(U32)msgLen) ;
                    }
                    else
                    {
                        printf("oad_check(), Undefined SSU msgType[%d] in DDB ?!\n", msgType);
                    }
                    //u32DownloadSize += msgLen ;
                }
            }
            if (ddbStatus == OAD_COMPLETE)
            {
                //U32 CRC_result=0;
                //U32 CRC_value=0;
                DBG_PRINT( printf("DDB complete.\n") );
                //Check CRC32
                //CRC_value=*((U32*)(DOWNLOAD_BUFFER_ADR_OAD+u32DownloadSize-4));
                //CRC_result=crc32( 0x00, (U8*)DOWNLOAD_BUFFER_ADR_OAD, u32DownloadSize-4 );
                //DBG_PRINT( printf("CRC_result 0x%x, CRC_value 0x%x\n", CRC_result, CRC_value) );
                break;
            }
        }
        {
            static U8 u8PrevPersentage=0;
            U8 u8Percentage = msAPI_OAD_GetProgress();
            if (u8Percentage > 100)
                u8Percentage = 100;
            if(u8PrevPersentage != u8Percentage)
            {
                u8PrevPersentage = u8Percentage;
                MApp_BL_DisplaySystem(u8Percentage);
                DBG_PRINT( printf("u8Percentage: %u, at %d\n", u8Percentage, __LINE__) );
            }
        }
        if (msAPI_Timer_DiffTimeFromNow(u32DsmccTimer) > MONITOR_DDB_TIME_OUT)
        {
            msAPI_OAD_ClearData();
            OAD_DEBUG( printf("Error> Exit DDB\n") );
            printf("DDB timeout!\n");
            //Show UI promt msg to do SDDS again or abort SDDS directly.
            return 1;
        }
    }

    MApp_BL_DisplaySystem(100);
    DBG_PRINT( printf("\nOAD Finished\n") );

    msAPI_OAD_GetModuleInfo( &u32ModuleAddr, &u32DownloadSize );
    DBG_PRINT( printf("u32ModuleAddr 0x%x, u32DownloadSize 0x%x\n", u32ModuleAddr, u32DownloadSize) );
    DBG_PRINT( printf("DOWNLOAD_BUFFER_ADR_OAD 0x%X, DOWNLOAD_BUFFER_LEN_OAD 0x%X\n", DOWNLOAD_BUFFER_ADR_OAD, DOWNLOAD_BUFFER_LEN_OAD) );
    U32DOWNLOADSIZE = u32DownloadSize;

    MApp_BL_DisplaySystem_clear();
#if ATSC_SDDS_ENABLE
    MApp_BL_DisplaySystem_setStatus((S8*)"Software Update(SDDS)");
#else
    MApp_BL_DisplaySystem_setStatus((S8*)"Software Update(OAD)");
#endif
    MApp_BL_DisplaySystem(0);


    sprintf(env_buf, "%lX", u32DownloadSize);
	setenv("loadsize", env_buf);
    return 0;
}

int chakra2_main_loop(void)
{
    DBG_PRINT( printf("%s: at %d\n", __func__, __LINE__) );

#if (ENABLE_BL_OAD_SCAN)
#error "NOT Ready!"
    EN_OAD_SCAN_STATE scanState, preState=STATE_OAD_SCAN_END ;
    U8 u8Percentage=0;
    U16 wOad_PID_Orig;
    MS_TP_SETTING stOrigTP;
    static BOOLEAN bUseDefaultCountry = FALSE; // Use country setting in database for the first scan/loop

    MApp_BL_DisplaySystem_clear(); //clear screen
    MApp_BL_DisplaySystem(0);
    {
        DBG_PRINT( printf("goto usb_check() !! at %d\n", __LINE__) );
        usb_check();

        MApp_OAD_GetInfo_BL( &wOad_PID_Orig, &(stOrigTP.u32Frequency), (U8 *)&(stOrigTP.enBandWidth) );
        DBG_PRINT( printf("OAD upgrade start, PID : 0x%x !!\n", wOad_PID_Orig) );
        DBG_PRINT( printf("OAD upgrade start, freq: %lu !!\n", stOrigTP.u32Frequency) );
        DBG_PRINT( printf("OAD upgrade start, band: %u !!\n", (U8)stOrigTP.enBandWidth) );
        DBG_PRINT( printf("goto oad_check() !! at %d\n", __LINE__) );
        oad_check();
        //TODO: if OADInfo valid
        MApp_BL_DisplaySystem_setStatus((S8*)"SCAN");
    }

    // Use DEFAULT_COUNTRY for OAD scan
    if((bUseDefaultCountry == TRUE) && (DEFAULT_COUNTRY != msAPI_CM_GetCountry()))
    {
        msAPI_CM_SetCountry(DEFAULT_COUNTRY); //TODO: consider general case
    }
    MApp_OAD_Init();
    MApp_OAD_Monitor();
    while(1)
    {
        scanState = MApp_OAD_Scan(&u8Percentage) ;
        MApp_BL_DisplaySystem(u8Percentage);
        if(scanState==STATE_OAD_SCAN_NEXT_CHANNEL)
        {
            //Check usb if switch to next channel. (shorter time gap to check usb)
            //DBG_PRINT( printf("goto usb_check() !! at %d\n", __LINE__) );
            //usb_check();
        }
        else if (scanState==STATE_OAD_SCAN_END)
        {
            MS_TP_SETTING stTempTP;
            MApp_OAD_GetInfo_BL( &wOad_PID, &(stTempTP.u32Frequency), (U8 *)&(stTempTP.enBandWidth) );
            DBG_PRINT( printf("OAD upgrade start, PID : 0x%x !!\n", wOad_PID) );
            DBG_PRINT( printf("OAD upgrade start, freq: %lu !!\n", stTempTP.u32Frequency) );
            DBG_PRINT( printf("OAD upgrade start, band: %u !!\n", (U8)stTempTP.enBandWidth) );
            if((wOad_PID == wOad_PID_Orig) &&
                (stTempTP.u32Frequency == stOrigTP.u32Frequency) &&
                ((U8)stTempTP.enBandWidth == (U8)stOrigTP.enBandWidth)) //OAD Scan finds no OAD information available
                break;
            DBG_PRINT( printf("goto oad_check() !! at %d\n", __LINE__) );
            oad_check();
            break ;
        }
        if(preState != scanState)
        {
            preState = scanState;
            printf("u8Percentage: %u, scanState: %x, at %d\n", u8Percentage, scanState, __LINE__);
        }
        MsOS_DelayTask(25);
        MApp_OAD_Monitor();
        MsOS_DelayTask(25);
    }
    bUseDefaultCountry = TRUE; // Always use DEFAULT_COUNTRY after the first scan/loop
#else //(ENABLE_BL_OAD_SCAN)
//    usb_check();
    if(!oad_check())
    {
        return 0;
    }
    return 1;
#endif //(ENABLE_BL_OAD_SCAN)
}

BOOLEAN oad_VersonCheck(U16 type,U8 *pPrivateData)
{
    U16  compatibilityLength,descriptorCount,model,version=0x0,i;
    U8  descriptorType, descriptorLength, specifierType,subDescriptorCount;
    U32 specifierData;
    BOOLEAN swPass=FALSE;
#if HW_MODEL_VER_CHECK
    BOOLEAN hwPass=FALSE;
#endif

    if (!VERSION_CHECK) return TRUE ;

    if (type==DATA_BC_ID_UK_EC) // MIS
    {
        U16 model_hw, version_hw;
        model_hw = GET_2BYTE(&pPrivateData[0]);
        version_hw = GET_2BYTE(&pPrivateData[2]);
        model = GET_2BYTE(&pPrivateData[4]);
        version = GET_2BYTE(&pPrivateData[6]);
#if HW_MODEL_VER_CHECK
        if ((model_hw == HW_MODEL)&&(version_hw == HW_VERSION))
#endif //HW_MODEL_VER_CHECK
        {
            if (model == AP_SW_MODEL)
            {
                tsVersion = (U32)version;
                return TRUE ;
            }
        }
        return FALSE ;
    }
    else if (type==DATA_BC_ID_SSU) // GroupCompatibility()
    {
        compatibilityLength = GET_2BYTE(pPrivateData);
        pPrivateData += 2;

        if ( compatibilityLength < 2 )
        {
            OAD_ASSERT( printf("Error> msAPI_OAD_ProcessDSI : compatibilityLength = 0x%x\n", compatibilityLength) );
            return FALSE ;
        }

        descriptorCount = GET_2BYTE(pPrivateData); pPrivateData += 2;
        OAD_DEBUG( printf("descriptorCount = %u\n", descriptorCount) );

        for ( i = 0; i < descriptorCount; i ++ )
        {
            descriptorType  = *pPrivateData++;
            descriptorLength= *pPrivateData++;

            switch ( descriptorType )
            {
                case 0x02:
                {
                    specifierType = *pPrivateData++;
                    specifierData = GET_3BYTE( pPrivateData ); pPrivateData += 3;
                    model = GET_2BYTE(pPrivateData); pPrivateData += 2;
                    version = GET_2BYTE(pPrivateData); pPrivateData += 2;
                    subDescriptorCount = *pPrivateData++;
                    tsVersion = (U32)version;
                    OAD_DEBUG( printf("%s: at %d, [%d]th groupCompatibilityDesc, groupCompatibilityDescType[%d]->SW. \n", __func__, __LINE__, i, descriptorType ) );
                    OAD_DEBUG( printf("%s: at %d, groupCompatibility type[%d]->SW. specifierType[%d], specifierData[0x%x], model[0x%x], version[0x%x], subDescriptorCount[%d]. \n", __func__, __LINE__, descriptorType, specifierData, specifierType, model, version, subDescriptorCount ) );
                    OAD_DEBUG( printf("TS=%x\n",tsVersion));
                    if ((specifierType==0x01)&&(model == AP_SW_MODEL)&&(specifierData==CUSTOMER_OUI)) //Do NOT check version
                    {
                        OAD_DEBUG( printf("%s: at %d, groupCompatibilityDescType[%d]->SW check. PASS. \n", __func__, __LINE__, descriptorType ) );
                        swPass = TRUE;
                    }
                    else
                    {
                        return FALSE ;
                    }
                    //pPrivateData += (descriptorLength-9) ;
                    //OAD_DEBUG( printf("[OTA] specifierType = 0x%02bx, specifierData = 0x%08lx\n", specifierType, specifierData) );
                    break;
                }
                case 0x01:
                    OAD_DEBUG( printf("%s: at %d, [%d]th groupCompatibilityDesc, groupCompatibilityDescType[%d]->HW. \n", __func__, __LINE__, i, descriptorType ) );
#if HW_MODEL_VER_CHECK
                    specifierType = *pPrivateData++;
                    specifierData = GET_3BYTE( pPrivateData ); pPrivateData += 3;
                    model = GET_2BYTE(pPrivateData); pPrivateData += 2;
                    version = GET_2BYTE(pPrivateData); pPrivateData += 2;
                    subDescriptorCount = *pPrivateData++;
                    OAD_DEBUG( printf("%s: at %d, groupCompatibility type[%d]->HW. specifierType[%d], specifierData[0x%x], model[0x%x], version[0x%x], subDescriptorCount[%d]. \n", __func__, __LINE__, descriptorType, specifierType, specifierData, model, version, subDescriptorCount ) );
                    if(1)
                    {
                        tsVersion = (U32)version;
                    }
                    OAD_DEBUG( printf("TS=%x\n",tsVersion));
                    if ((specifierType==0x01)&&(model == HW_MODEL)&&(version == HW_VERSION)&&(specifierData==CUSTOMER_OUI))
                    {
                        OAD_DEBUG( printf("%s: at %d, groupCompatibilityDescType[%d]->HW check. PASS. \n", __func__, __LINE__, descriptorType ) );
                        hwPass = TRUE;
                    }
                    else
                    {
                        return FALSE ;
                    }
                    //pPrivateData += (descriptorLength-9) ;
                    //OAD_DEBUG( printf("[OTA] specifierType = 0x%02bx, specifierData = 0x%08lx\n", specifierType, specifierData) );
                    break;
#endif
                default:
                {
                    pPrivateData += descriptorLength;
                    OAD_DEBUG( printf("[OTA] descriptorType = 0x%02bx\n", descriptorType) );
                    break;
                }
            }
       }// End of for
    }
    else
    {
        OAD_DEBUG( printf("%s: at %d, undefined type[%d]. \n", __func__, __LINE__, type) );
    }
#if HW_MODEL_VER_CHECK
    if((swPass == TRUE) && (hwPass == TRUE))
#else //HW_MODEL_VER_CHECK
    if(swPass == TRUE)
#endif //HW_MODEL_VER_CHECK
    {
        return TRUE ;
    }
    return FALSE ;
}


#if (ENABLE_HUMMINGBIRD_CUSTOMER)
// added  for dmx init just once
static int init_times = 0;
int oad_prepare(void)
{
    DBG_PRINT( printf("%s: at %d\n", __func__, __LINE__) );
    u32OadbufMonitorAddr = (U32)(OADSEC_BUFFER_ADR); //(_VA2PA((U32)demuxBuf.monitor)) ;
    u32OadbufDownloadAddr = (U32)(OADSEC_BUFFER_ADR + 0x4000);//(_VA2PA((U32)demuxBuf.download)) ;
    u32OadbufMonitorSize = DMX_BUF_SIZE_MONITOR;
    u32OadbufDownloadSize = DMX_BUF_SIZE_DOWNLOAD;

    _u8OADMonitorFID=MSAPI_DMX_INVALID_FLT_ID;
    _u8OADDownloadFID=MSAPI_DMX_INVALID_FLT_ID;
    U32DOWNLOADSIZE = 0;
    u32DownloadSize = 0;
    versionNum = 0;
    u32ModuleAddr = 0;
    tsVersion=0;
    memset(au8MonitorSection, 0, sizeof(au8MonitorSection));
    memset(au8DownloadSection, 0, sizeof(au8DownloadSection));

    msAPI_OAD_ClearData();
    msAPI_OAD_SetOui((U8)(CUSTOMER_OUI>>16), (U8)(CUSTOMER_OUI>>8), (U8)(CUSTOMER_OUI));
    msAPI_OAD_SetVersionCheck(oad_VersonCheck) ;
    
    {
        //front end init
        char* buffer=NULL;
        int ret=0;

        buffer=(char *)malloc(BUF_SIZE);
        if(buffer != NULL)
        {
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD )||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT51_DEMOD )||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC51_DEMOD ))//!= MSTAR_MSB123x_DEMOD)||(FRONTEND_DEMOD_TYPE != MSTAR_MSB122x_DEMOD)
            //printf("Internal \n");
            memset(buffer, 0 , 256);
            sprintf(buffer, "tuner_init");
            ret=run_command(buffer, 0);
            if(ret==-1)
            {
                free(buffer);
                return -1;
            }
            memset(buffer, 0 , 256);
            sprintf(buffer, "tuner_tune %lu %x", u32Frequency, u8BandWidth);
            ret=run_command(buffer, 0);
            if(ret==-1)
            {
				free(buffer);
                return -1;
            }
			//changed  for dmx just init once
			if(init_times < 1)
			{
                memset(buffer, 0 , 256);
                sprintf(buffer, "dmx_init 0");
                ret=run_command(buffer, 0);
                if(ret==-1)
                {
                    free(buffer);
                    return -1;
                }
			   
			    init_times+=1;
			}
#else

//printf("External \n");
#if ((CONFIG_COMMANDS&(CFG_CMD_TUNER | CFG_CMD_DEMOD  | CFG_CMD_DEMOCODE)) == (CFG_CMD_TUNER | CFG_CMD_DEMOD | CFG_CMD_DEMOCODE))

            memset(buffer, 0 , 256);
            sprintf(buffer, "tuner_init");
            ret=run_command(buffer, 0);
            if(ret==-1)
            {
                free(buffer);
                return -1;
            }

#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD || FRONTEND_DEMOD_TYPE == MSTAR_MSB1236C_DEMOD )

               
            memset(buffer, 0 , 256);
            sprintf(buffer, "tuner_demodtype 11"); //T2
            ret=run_command(buffer, 0);
            if(ret==-1)
            {
                free(buffer);
                return -1;
            }
            memset(buffer, 0 , 256);
			
			extern void MApi_FE_SetT2Param(MS_U8 u8ScanType, MS_U8 u8PlpId);
			MApi_FE_SetT2Param(1,0xff);
            sprintf(buffer, "tuner_tune %lu %x", u32Frequency, u8BandWidth);
            ret=run_command(buffer, 0);
            if(ret==-1)
            {
                free(buffer);
                return -1;
            }
          
#else

            memset(buffer, 0 , 256);
            sprintf(buffer, "tuner_demodtype 1"); //T1 // internal
            ret=run_command(buffer, 0);
            if(ret==-1)
            {
                free(buffer);
                return -1;
            }
            memset(buffer, 0 , 256);
            sprintf(buffer, "tuner_tune %lu %x", u32Frequency, u8BandWidth);
            ret=run_command(buffer, 0);
#endif
#endif
/*
    0    DMX_FLOW_INPUT_DEMOD,               ///< DMX input from internal demod
    1    DMX_FLOW_INPUT_MEM,                 ///< DMX input from memory
    2    DMX_FLOW_INPUT_EXT_INPUT0,          ///< DMX input from external input 0
    3    DMX_FLOW_INPUT_EXT_INPUT1,          ///< DMX input from external input 1
*/
#if ((MS_BOARD_TYPE_SEL == BD_MST098E_D01B) || (MS_BOARD_TYPE_SEL == BD_MST138B_D01A \
	  ||MS_BOARD_TYPE_SEL == BD_MST124C_D01A_S))
	  
	  //if test isdbt ,add ||MS_BOARD_TYPE_SEL == BD_MST124C_D01A_S
	  //changed  for dmx just init once
			if(init_times < 1)
			{
                memset(buffer, 0 , 256);
                sprintf(buffer, "dmx_init 3");
                ret=run_command(buffer, 0);

                if(ret==-1)
                {
                    free(buffer);
                    return -1;
                }
			    init_times+=1;
			}

#elif (MS_BOARD_TYPE_SEL == BD_SS2000_DT0A)
           if(init_times < 1)
			{
                DBG_PRINT( printf("%s: at %d, DMX_init 3 for SS200. \n", __func__, __LINE__) );
                memset(buffer, 0 , 256);
                sprintf(buffer, "dmx_init 3");
                ret=run_command(buffer, 0);
                if(ret==-1)
                {
                    free(buffer);
                    return -1;
                }
			    init_times+=1;
			}
#else
            if(init_times < 1)
			{
                memset(buffer, 0 , 256);
                sprintf(buffer, "dmx_init 2");
                ret=run_command(buffer, 0);
                if(ret==-1)
                {
                    free(buffer);
                    return -1;
                }
			    init_times+=1;
			}
#endif
#endif
            free(buffer);
        }
        else
        {
            return 1;
        }
    }
	  #if (ENABLE_HUMMINGBIRD_CUSTOMER)
 // for dvbs 131x
	        #define DMX_INPUT_INV 0
			
			#if(DMX_INPUT_INV == 1)
            MApi_DMX_Parl_Invert(DMX_FLOW_PLAYBACK, TRUE);
            #endif
	 #endif
    return 0;
}


#else // else ENABLE_HUMMINGBIRD_CUSTOMER
int oad_prepare(void)
{
    DBG_PRINT( printf("%s: at %d\n", __func__, __LINE__) );
    u32OadbufMonitorAddr = (U32)(OADSEC_BUFFER_ADR); //(_VA2PA((U32)demuxBuf.monitor)) ;
    u32OadbufDownloadAddr = (U32)(OADSEC_BUFFER_ADR + 0x4000);//(_VA2PA((U32)demuxBuf.download)) ;
    u32OadbufMonitorSize = DMX_BUF_SIZE_MONITOR;
    u32OadbufDownloadSize = DMX_BUF_SIZE_DOWNLOAD;

    _u8OADMonitorFID = MSAPI_DMX_INVALID_FLT_ID;
    _u8OADDownloadFID = MSAPI_DMX_INVALID_FLT_ID;
    U32DOWNLOADSIZE = 0;
    u32DownloadSize = 0;
    versionNum = 0;
    u32ModuleAddr = 0;
    tsVersion=0;
    memset(au8MonitorSection, 0, sizeof(au8MonitorSection));
    memset(au8DownloadSection, 0, sizeof(au8DownloadSection));

    msAPI_OAD_ClearData();
    msAPI_OAD_SetOui((U8)(CUSTOMER_OUI>>16), (U8)(CUSTOMER_OUI>>8), (U8)(CUSTOMER_OUI));
    msAPI_OAD_SetVersionCheck(oad_VersonCheck) ;

    {
        //front end init
        char* buffer = NULL;
        int ret = 0;

        buffer = (char *)malloc(BUF_SIZE);
        if(buffer != NULL)
        {
            memset(buffer, 0 , 256);
            sprintf(buffer, "tuner_init");
            ret = run_command(buffer, 0);
            if(ret == -1)
            {
                free(buffer);
                return -1;
            }

#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD )||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT51_DEMOD )||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC51_DEMOD ))//!= MSTAR_MSB123x_DEMOD)||(FRONTEND_DEMOD_TYPE != MSTAR_MSB122x_DEMOD)
            //printf("Internal \n");
            memset(buffer, 0 , 256);
            sprintf(buffer, "tuner_tune %lu %x", u32Frequency, u8BandWidth);
            ret = run_command(buffer, 0);
            if(ret == -1)
            {
                free(buffer);
                return -1;
            }

            memset(buffer, 0 , 256);
            sprintf(buffer, "dmx_init 0");
            ret = run_command(buffer, 0);
            if(ret == -1)
            {
                free(buffer);
                return -1;
            }
			

#else
            //printf("External \n");
            memset(buffer, 0 , 256);
            sprintf(buffer, "tuner_demodtype 1"); //T1 // internal
            ret = run_command(buffer, 0);
            if(ret == -1)
            {
                free(buffer);
                return -1;
            }
            memset(buffer, 0 , 256);
            sprintf(buffer, "tuner_tune %lu %x", u32Frequency, u8BandWidth);
            ret = run_command(buffer, 0);
#if (FRONTEND_DEMOD_TYPE == MSTAR_MSB123x_DEMOD)
            if(ret == -1)
            {
                //try T2
                memset(buffer, 0 , 256);
                sprintf(buffer, "tuner_demodtype 2"); //T2
                ret = run_command(buffer, 0);
                if(ret == -1)
                {
                    free(buffer);
                    return -1;
                }
                memset(buffer, 0 , 256);
                sprintf(buffer, "tuner_tune %lu %x", u32Frequency, u8BandWidth);
                ret = run_command(buffer, 0);
                if(ret == -1)
                {
                    free(buffer);
                    return -1;
                }
            }
#endif
/*
    0    DMX_FLOW_INPUT_DEMOD,               ///< DMX input from internal demod
    1    DMX_FLOW_INPUT_MEM,                 ///< DMX input from memory
    2    DMX_FLOW_INPUT_EXT_INPUT0,          ///< DMX input from external input 0
    3    DMX_FLOW_INPUT_EXT_INPUT1,          ///< DMX input from external input 1
*/
#if ((MS_BOARD_TYPE_SEL == BD_MST098E_D01B) || (MS_BOARD_TYPE_SEL == BD_MST138B_D01A))
            memset(buffer, 0 , 256);
            sprintf(buffer, "dmx_init 3");
            ret = run_command(buffer, 0);
            if(ret == -1)
            {
                free(buffer);
                return -1;
            }
#elif (MS_BOARD_TYPE_SEL == BD_SS2000_DT0A)
            DBG_PRINT( printf("%s: at %d, DMX_init 3 for SS200. \n", __func__, __LINE__) );
            memset(buffer, 0 , 256);
            sprintf(buffer, "dmx_init 3");
            ret = run_command(buffer, 0);
            if(ret == -1)
            {
                free(buffer);
                return -1;
            }
#else
            memset(buffer, 0 , 256);
            sprintf(buffer, "dmx_init 2");
            ret = run_command(buffer, 0);
            if(ret == -1)
            {
                free(buffer);
                return -1;
            }
#endif
#endif
            free(buffer);
        }
        else
        {
            return 1;
        }
    }
    return 0;
}
#endif // end ENABLE_HUMMINGBBIRD_CUSTOMER
void MApp_BL_Init(void)
{
#if (ENABLE_BL_UI)
    {
        char* buffer=NULL;
        int ret=0;

        buffer=(char *)malloc(BUF_SIZE);
        if(buffer != NULL)
        {
            memset(buffer, 0 , 256);
            sprintf(buffer, "osd_create %d %d", GWIN_WIDTH, GWIN_HEIGHT);
            ret=run_command(buffer, 0);
            free(buffer);
            if(ret==-1)
                return;
        }
        else
        {
            printf("%s: malloc() failed, at %d\n", __func__, __LINE__);
        }
    }
#endif //(ENABLE_BL_UI)

    /* initialize 8K bytes memory pool */
    msAPI_Memory_Init();
}
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
#define DEFAULT_OAD_PID 0
extern int MApp_BL_FindOADChan(MS_U16 *pOADPid);
MS_BOOL oad_get_pid(void)
{
    MS_U16 u16OADPid;
    int ret = 0;
    if (wOad_PID != 0xFF && wOad_PID !=0)
    {
        return TRUE;
    }
    ret = MApp_BL_FindOADChan(&u16OADPid);//if  true return :0 if false return:-1
    if (ret)//MApp_BL_FindOADChan return wrong value
    {
        printf("\nhererere\n");
        u16OADPid = DEFAULT_OAD_PID;//if we couldn't get oad_pid,use the default one 1260
        return FALSE;
	}
    
    wOad_PID = u16OADPid;
    return TRUE;

}

void scan_chaninfo_init(CHANINFO_T * chan_info)
{
    #if ENABLE_DVB_C_SYS
	U8 *pTemp;
	#endif
	
    #if ENABLE_ISDB_T_SYS

	chan_info->m_bandwith = 1;
	chan_info->m_chanNum = 7;
	chan_info->m_countryType = COUNTRY_TYPE_BRAZIL;
	chan_info->m_frequency = 177100L;
	chan_info->m_u16OADPid = 0;
	wOad_PID = 0;
    u32Frequency = 177100L;
    u8BandWidth = 3;
	#elif ENABLE_DVB_C_SYS
	wOad_PID = 0;
	
    pTemp = (U8 *)getenv("oad_upgrade_freq") ;
    if(NULL == pTemp)
    {
	    return ;
    }
    u32Frequency = (simple_strtol((const char*)pTemp, NULL, 10))*1000;
	u8BandWidth = 3;
	#elif ENABLE_DVB_S_SYS
	//dvbs do nothing
	#else
    chan_info->m_bandwith = 3;
	chan_info->m_chanNum = 21;
	chan_info->m_countryType = COUNTRY_TYPE_EUROPE;
	chan_info->m_frequency = 474000L;
	chan_info->m_u16OADPid = 0;
	wOad_PID = 0;
    u32Frequency = 474000L;
    u8BandWidth = 3;
	#endif
	
}
void scan_chaninfo_update(CHANINFO_T * chan_info)
{
    u32Frequency = chan_info->m_frequency;
	u8BandWidth =  chan_info->m_bandwith;
	
}
#endif // end ENABLE_HUMMINGBIRD_CUSTOMER
int chakra2_main(void)
{
#if (ENABLE_HUMMINGBIRD_CUSTOMER)
    CHANINFO_T  RfChanInfo;
	int ret = 0;
#endif // end ENABLE_HUMMINGBIRD_CUSTOMER

    DBG_PRINT( printf("%s: at %d\n", __func__, __LINE__) );
    MApp_BL_Init();

    DBG_PRINT( printf("prepare USB/OAD upgrade !!\n") );
    //MDrv_Sys_ClearWatchDog();
    //usb_prepare();
	#if (ENABLE_HUMMINGBIRD_CUSTOMER)
    scan_chaninfo_init(&RfChanInfo);
	#if (ENABLE_DVB_C_SYS)
	if(oad_prepare())//if true return 0 else return -1
        return -1;
    if(!(ret = oad_get_pid()))//dvbc get oad_pid failed return
		return -1;
	#elif ENABLE_DVB_S_SYS
	if((ret = oad_prepare()))//if true return 0 else return -1
        return -1;
    if(!(ret = oad_get_pid()))//dvbs get oad_pid failed return
        return -1;

    #else//else ENABLE_DVB_C_SYS
	do{
		scan_chaninfo_update(&RfChanInfo);
        if(oad_prepare())//if true return 0 else return -1
        {
            continue;
        }
	    if(!oad_get_pid())// if true return ture else return false
	    {
	    	  
		    continue;//get oad_pid false
	    }
		else
		{ 
			  ret = 1;
			  break;//get oad_pid true
		}
	  }while(!OAD_SCAN_GetNextChanInfo(&RfChanInfo));//if ture,return 0 else return -1


    if(!ret)//if scan for oad_pid failed then return 
    return -1;
	#endif // end ENABLE_DVB_C_SYS
	
	#else // else ENABLE_HUMMINGBIRD_CUSTOMER
        if(oad_prepare())
        return -1;
	#endif //end ENABLE_HUMMINGBIRD_CUSTOMER
	
    MApp_BL_DisplaySystem_clear(); //clear screen
    MApp_BL_DisplaySystem(0);
    if(!chakra2_main_loop())
    {
        return 0;
    }
    return -1;
}

int do_oad_get_size (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    U32* size;
    if (argc !=2) {
        printf("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }
    size=(U32*)simple_strtoul(argv[1], NULL, 16);
    *size = U32DOWNLOADSIZE;
    return 0;
}

int do_oad (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    if (argc !=4) {
        printf("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }
    u32Frequency = (U32)simple_strtoul(argv[1], NULL, 10);
    u8BandWidth = (U8)simple_strtoul(argv[2], NULL, 16);
    wOad_PID = (U16)simple_strtoul(argv[3], NULL, 16);
   printf("do_oad(), Freq[%d], BandWidth[%d], Oad_PID[%d].\n", u32Frequency, u8BandWidth, wOad_PID);
    return chakra2_main();
}

U_BOOT_CMD(
    oad_get_size,    2,   1,  do_oad_get_size,
    "oad_get_size - Get the file size from OAD download \n",
    "Get download size\n"
    "    - Address: 32 bits (Hex) address to save download size\n"
);
U_BOOT_CMD(
    oad,    4,   1,  do_oad,
    "oad - Software Upgrade(OAD) \n",
    "Over the Air Download\n"
    "    - Frequency: 32 bits (Decimal). e.g. 474000\n"
    "    - Bandwidth: 1:6MHz, 2:7MHz, 3: 8MHz\n"
    "    - Pid: 16 bits (Hex) pid, e.g. 0x150\n"
);
#endif

