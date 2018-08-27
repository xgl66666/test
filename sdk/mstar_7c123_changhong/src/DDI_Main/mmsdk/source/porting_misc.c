#include <string.h>
#include "MsOS.h"
#include "porting_misc.h"
#include "porting_sysinfo.h"
#include "porting_thread.h"

#include "MsTypes.h"

#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
#include "apiXC_EX.h"
#else
#include "drvXC_IOPort.h"
#include "apiXC.h"
#ifndef MMSDK_DISPLAY_NEW_PQ //remove after PQ update
#include "msAPI_Scaler_Adaptive.h"
#endif
#endif
#include "apiDMX.h"
#include "iniparser.h"

#define MISC_FBL_MODE_THRESHOLD     0xFFFFFFFF  //(2200UL * 1125UL * 60UL)
#define MISC_MIRROR_VIDEO_FLAG      0
#define MISC_MM_FRC_ENABLE          0

#define PT_MISC_ERR(fmt, arg...)   PT_SYS_PrintLog(E_MMSDK_DBG_LEVEL_ERR, "\033[1;31m######[%s]###### "fmt" \033[0m\n",__FUNCTION__,##arg);
#define PT_MISC_DBG(fmt, arg...)   PT_SYS_PrintLog(E_MMSDK_DBG_LEVEL_DBG, "\033[1;31m######[%s]###### "fmt" \033[0m\n",__FUNCTION__,##arg);

#if defined(MSOS_TYPE_LINUX)
#define CPCODEC_CFG_FILE_PATH_SIZE 128
static const MMSDK_U8 cfgFileName[] = "/config/MM/mm_cfg.ini";
static MMSDK_U8  g_u8CfgFilePath[CPCODEC_CFG_FILE_PATH_SIZE] = "";
#else
#include "mm_cfg_ini.h"
extern MMSDK_U8 u8IniInfo[STRING_LENGTH];
#endif

MS_U8 u8TSPFW[] = {
    #include "fwTSP.dat"
};

static PT_dictionary* _gpDictionary = NULL;
static MMSDK_U32 _gu32IniRefCount = 0;

static void* _gpstMutex = NULL;

/// For vdplayer run on co-processor only
const MMSDK_U8* PT_MISC_GetVdplayerBinPath(void)
{
    /* Not used in MM_HK mode. */
    return (MMSDK_U8*)"";
}


/// TSP FW Bin path
const MMSDK_U8*  PT_MISC_GetTspFwBinPath(void)
{
    return (MMSDK_U8*)"\bin\fwTSP.dat";
}


MMSDK_BOOL PT_MISC_LoadVdplayerBin(MMSDK_U32 u32PhyAdd, const MMSDK_U32 u32BufferSize)
{
    /* Not used in MM_HK mode. */
    return TRUE;
}


MMSDK_BOOL PT_MISC_LoadTspFwBin(MMSDK_U32 u32PhyAdd, const MMSDK_U32 u32BufferSize)
{
    if (sizeof(u8TSPFW) <= (MMSDK_S32)u32BufferSize)
        memcpy((void*)(MsOS_PA2KSEG1(u32PhyAdd)), (void*)u8TSPFW, sizeof(u8TSPFW));
    else
        return FALSE;

    return TRUE;
}


MMSDK_U32 PT_MISC_GetFBLModeThreshold(void)
{
    return MISC_FBL_MODE_THRESHOLD;
}


MMSDK_BOOL PT_MISC_GetMirrorVideoFlag(void)
{
    return MISC_MIRROR_VIDEO_FLAG;
}


MMSDK_BOOL PT_MISC_IsMMFrcEnable(void)
{
    return MISC_MM_FRC_ENABLE;
}


MMSDK_BOOL PT_MISC_Demux_IsInitialized(void)
{
    return ((MApi_DMX_ChkAlive() == DMX_FILTER_STATUS_OK) ? 1 : 0);

}

MMSDK_BOOL PT_MISC_Demux_GetSettingInfo(MMSDK_U32* pu32VQStart, MMSDK_U32* pu32VQSize, MMSDK_U8* pu8CPFilterStart, MMSDK_U8* pu8CPFilterEnd)
{
    if((!pu32VQStart) || (!pu32VQSize) || (!pu8CPFilterStart) || (!pu8CPFilterEnd))
        return FALSE;

    PT_SYS_GetTspVQInfo(pu32VQStart, pu32VQSize);
    *pu8CPFilterStart = 0;//Not applied on K1.
    *pu8CPFilterEnd = 0;//Not applied on K1.

    return TRUE;
}

MMSDK_BOOL PT_MISC_PQ_AdaptContinuousFramePQ(const MMSDK_BOOL bEnable)
{
#if (MMSDK_DISPLAY_DUALXC_PORTING == 1)
    XC_DEVICE_ID stDevice;
    stDevice.u32Id = 0;
    stDevice.u32Version = 0;

    MApi_XC_EX_W2BYTEMSK(&stDevice, 0x0A48, (bEnable?(BIT(15)):0), BIT(15));
    //sub bank 0x0A , 16 bit addr 24
#else
#ifndef MMSDK_DISPLAY_NEW_PQ //remove after PQ update
    MApi_XC_FilmMode_AnyCandence_Enable(bEnable);
#endif
#endif
    return TRUE;
}

MMSDK_BOOL PT_resetCfgFilePath(void)
{
    if(_gpstMutex == NULL)
    {
        if(PT_Thread_MutexInit(&_gpstMutex) == FALSE)
        {
            PT_MISC_ERR("mutex init fail \n");
            return FALSE;
        }
    }

#if defined(MSOS_TYPE_LINUX)
    PT_MISC_DBG("[%s]\n",__FUNCTION__);
    return PT_setCfgFilePath(cfgFileName);
#else
    return TRUE;
#endif
}

MMSDK_BOOL PT_setCfgFilePath(const MMSDK_U8 * pu8FilePath)
{
    PT_Thread_MutexLock(_gpstMutex);

#if defined(MSOS_TYPE_LINUX)
    if((pu8FilePath == NULL) || (pu8FilePath[0] == 0))
    {
        PT_MISC_ERR("setCfgFilePath path is null\n");
        PT_Thread_MutexUnlock(_gpstMutex);
        return FALSE;
    }
    if (strlen((const char*)pu8FilePath) >= CPCODEC_CFG_FILE_PATH_SIZE)
    {
        PT_MISC_ERR("Path length should less than 128 char (your=%d)\n", strlen((const char*)pu8FilePath));
        PT_Thread_MutexUnlock(_gpstMutex);
        return FALSE;
    }
    // coverity[buffer_size_warning]
    strncpy((char*)g_u8CfgFilePath, (const char*)pu8FilePath, CPCODEC_CFG_FILE_PATH_SIZE);
    PT_MISC_DBG("setCfgFilePath=%s\n", (char*)g_u8CfgFilePath);
#else
    if(!pu8FilePath)
    {
        PT_Thread_MutexUnlock(_gpstMutex);
        return FALSE;
    }
#endif

    PT_Thread_MutexUnlock(_gpstMutex);
    return TRUE;
}

const MMSDK_U8 * PT_getCfgFilePath(void)
{
    PT_Thread_MutexLock(_gpstMutex);

#if defined(MSOS_TYPE_LINUX)
    //PT_MISC_DBG("getCfgFilePath=%s\n", (char*)g_u8CfgFilePath);
    PT_Thread_MutexUnlock(_gpstMutex);

    return g_u8CfgFilePath;
#else
    static MMSDK_U8 u8Res[30] = "ecos does not need it";
    PT_Thread_MutexUnlock(_gpstMutex);
    return u8Res;
#endif
}

MMSDK_BOOL PT_encodeJpeg(const MMSDK_U8* pu8DstFullName, const MMSDK_U8 *pu8RgbData, const MMSDK_U16 u16Width, const MMSDK_U16 u16Height)
{
    if((!pu8DstFullName) || (!pu8RgbData))
        return FALSE;

    return TRUE;
}

PT_dictionary * PT_iniparser_load(const MMSDK_U8 * ininame)
{
    PT_Thread_MutexLock(_gpstMutex);
    if(!ininame)
    {
        PT_MISC_ERR("Error!! ininame is NULL!! /n");
        PT_Thread_MutexUnlock(_gpstMutex);
        return NULL;
    }

    if(NULL == _gpDictionary)
    {
#if defined(MSOS_TYPE_LINUX)
        _gpDictionary = (PT_dictionary*) iniparser_load((const char*) ininame);
#else
        MM_INI_setIniInfo();
        if(strlen((const char * )u8IniInfo) > STRING_LENGTH)
        {
            PT_MISC_ERR("ini string length is %d > STRING_LENGTH %d", strlen((const char * )u8IniInfo), STRING_LENGTH);
            PT_Thread_MutexUnlock(_gpstMutex);
            return NULL;
        }
        _gpDictionary = (PT_dictionary*) iniparser_load_ecos((const char*) u8IniInfo);
#endif
    }

    if(NULL != _gpDictionary)
    {
        _gu32IniRefCount++;
    }

    PT_Thread_MutexUnlock(_gpstMutex);
    return _gpDictionary;
}

MMSDK_U8 * PT_iniparser_getstring(PT_dictionary * d, const MMSDK_U8 * key, MMSDK_U8 * def)
{
    return (MMSDK_U8*) iniparser_getstring((dictionary*) d,(const char*) key,(char*) def);
}

void PT_iniparser_freedict(PT_dictionary * d)
{
    PT_Thread_MutexLock(_gpstMutex);
    if(!d)
    {
        PT_MISC_ERR("Error!! d is NULL!! /n");
        PT_Thread_MutexUnlock(_gpstMutex);
        return;
    }

    if(d == _gpDictionary)
    {
        _gu32IniRefCount--;
        if(_gu32IniRefCount <= 0)
        {
            _gpDictionary = NULL;
            iniparser_freedict((dictionary*) d);
        }
    }
    PT_Thread_MutexUnlock(_gpstMutex);

    if (_gpstMutex != NULL && _gu32IniRefCount <= 0)
    {
        if(PT_Thread_MutexDestroy(&_gpstMutex) != TRUE)
        {
            PT_MISC_ERR("Release mutex fail\n");
            return;
        }
    }
}

MMSDK_BOOL PT_GenericFunction(const MMSDK_U8* const pu8MsgType, void* pMsgInfo, void* pAckInfo)
{
    return TRUE;
}

MMSDK_BOOL PT_GetPortingVersion(MMSDK_U32* pu32Version)
{
    *pu32Version = MMSDK_PORTING_VERSION;
    return TRUE;
}

