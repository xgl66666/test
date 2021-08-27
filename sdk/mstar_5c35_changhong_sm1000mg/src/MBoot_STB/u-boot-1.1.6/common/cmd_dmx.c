/*
 * (C) Copyright 2002
 * Detlev Zundel, DENX Software Engineering, dzu@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * DMX handling routines
 */

#include <common.h>
#include <command.h>
#include <sys/drv/MsTypes.h>
#include <sys/drv/drvMMIO.h>
#include <sys/common/MsOS.h>
#include <sys/drv/drvIIC.h>
#include <sys/api/apiDMX.h>
#include <configs/uboot_module_config.h>

#if (CONFIG_COMMANDS & CFG_CMD_DMX)

#define MAX_PG_IN_PAT           8
#define SECTION_BUFFER_NUM    64  // 64 * 0x1000 = 0x40000
#define SECTION_BUFFER_SIZE   0x1000

static U32 _s32DataReadyEventId;

#define TSP_FW_SIZE 0x4000
static U8 u8TSFwBuf[TSP_FW_SIZE + 128];

#define TSP_VQ_SIZE 0x1900
static U8 u8TSVQBuf[TSP_FW_SIZE + 16];

MS_U8 u8TSPFirmware[] = {
    #include "fwTSP.dat"
};

typedef enum
{
    E_SI_STREAM_MPEG1_VID           = 0x01,
    E_SI_STREAM_MPEG2_VID           = 0x02,
    E_SI_STREAM_MPEG1_AUD           = 0x03,
    E_SI_STREAM_MPEG2_AUD           = 0x04,
    E_SI_STREAM_PRIVATE_DATA        = 0x06,
    E_SI_STREAM_DSMCC_DATA_TYPE_A   = 0x0A,
    E_SI_STREAM_DSMCC_DATA_TYPE_B   = 0x0B,
    E_SI_STREAM_DSMCC_DATA_TYPE_C   = 0x0C,
    E_SI_STREAM_DSMCC_DATA_TYPE_D   = 0x0D,
    E_SI_STREAM_DSMCC_DATA_TYPE_E   = 0x0E,
    E_SI_STREAM_AAC_AUD             = 0x0F,
    E_SI_STREAM_MPEG4_AUD           = 0x11,
    E_SI_STREAM_AVCH264_VID         = 0x1B,
    E_SI_STREAM_OP_MEPG2_VID        = 0x80,
    E_SI_STREAM_AC3_AUD             = 0x81,
} E_SI_StreamType;


typedef enum
{
    SiNOT_VALID                    = 0x00,
    SiMPEG                         = 0x01,
    SiAC3                          = 0x02,
    SiMPEG_AD                      = 0x03,
    SiMPEG4                        = 0x04,
    SiAAC                          = 0x05,
    SiAC3P                         = 0x06,
    SiAC3_AD                       = 0x07,
} SI_AudioFormat;


typedef struct
{
    MS_U32 u32PidVideo;
    MS_U32 u32PidAudio;
    MS_U32 u32PidPcr;
    MS_U32 u32StreamTypeVideo;
    MS_U32 u32StreamTypeAudio;
    MS_U32 u32ProgNum;
    MS_U32 u32PidPmt;
    MS_BOOL b264;
    SI_AudioFormat eAudioFmt;
} stPG;


typedef struct
{
//    MS_U32 u32Bandth;
//    MS_U32 u32Freq;
    stPG stPgList[MAX_PG_IN_PAT];
    MS_U32 u32PgNum;
} stFreqPG;





static MS_U8                       Data[1024];

static MS_U8 SEC_BUF[SECTION_BUFFER_SIZE];
static MS_U8* Section_Buf_Addr = (MS_U8*)SEC_BUF;




// channel database
static stFreqPG stChDB;


/*
 * Subroutine:  do_tuner
 *
 * Description: Handler for 'tuner' command..
 *
 * Inputs:	argv[1] contains the subcommand
 *
 * Return:      None
 *
 */
int do_dmx_init(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    MS_PHYADDR      phyFW;
    MS_U8*          pu8FW;
    DMX_FLOW_INPUT inSrc;
    if (argc<1)
    {
        printf ("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }
    inSrc = (DMX_FLOW_INPUT)simple_strtoul(argv[1], NULL, 10);
    MDrv_MMIO_Init(); // must init MMI before init DMX

    pu8FW = (MS_U8*)(((int)u8TSFwBuf + 0x7F) & (~0x7F));
    pu8FW = (MS_U8*)MsOS_VA2PA((MS_U32)pu8FW);
    pu8FW = (MS_U8*)MsOS_PA2KSEG1((MS_U32)pu8FW);
    memcpy(pu8FW, u8TSPFirmware, sizeof(u8TSPFirmware));
    MsOS_FlushMemory();

    phyFW = (MS_PHYADDR)MsOS_VA2PA((MS_U32)pu8FW);

    if(  (((U32)u8TSFwBuf >= 0x88000000) && ((U32)u8TSVQBuf >= 0x88000000)) || 
         (((U32)u8TSFwBuf >= 0xA8000000) && ((U32)u8TSVQBuf >= 0xA8000000)) )
    {
        if (DMX_FILTER_STATUS_ERROR == MApi_DMX_CMD_Run(0xF0010002, 1, 0, 0) )
        {
            printf("MApi_DMX_CMD_Run for 128~256M setting fail\n");
            return -1;
        }
    }
    else if ( (((U32)u8TSFwBuf < 0x88000000) && ((U32)u8TSVQBuf < 0x88000000)) ||
              (((U32)u8TSFwBuf < 0xA8000000) && ((U32)u8TSVQBuf < 0xA8000000)) )
    {
        /* defautl 0~128M, so below doesn't need to be set
        if (DMX_FILTER_STATUS_ERROR == MApi_DMX_CMD_Run(0xF0010002, 1, 0, 0) )
        {
            printf("MApi_DMX_CMD_Run for 128~256M setting fail\n");
            return -1;
        }
        */
    }
    else
    {
        printf("u8TSFwBuf and u8TSVQBuf mismatch in same memory 0~128M or 128M~256M\n");
        return -1;
    }
     
    if (DMX_FILTER_STATUS_ERROR == MApi_DMX_SetFW(phyFW, sizeof(u8TSPFirmware)))
    {
        printf("DeMux>>ERROR! SetFW fail\n");
        return -1;
    }
    MApi_DMX_SetHK(TRUE);
    if (DMX_FILTER_STATUS_ERROR == MApi_DMX_Init())
    {
        printf("DeMux>>ERROR! TSP init fail\n");
        return -1;
    }

#if defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KAPPA) ||  defined(CONFIG_KELTIC)|| defined(CONFIG_KRITI) || defined(CONFIG_KERES)
    // set VQ for TSP, set FW/VQ for TSP2
    MS_U8*          pu8VQ;
    MS_PHYADDR      phyVQ;
    DMX_TSPParam    param;

    memset(&param,0,sizeof(param));
    pu8VQ = (MS_U8*)(((int)u8TSVQBuf + 0x7) & (~0x7));
    phyVQ = (MS_PHYADDR)MsOS_VA2PA((MS_U32)pu8VQ);

    // printf("[%s][%d] (fw, vq) = (0x%08x 0x%08x)\n", __FUNCTION__, __LINE__,
    // phyFW, phyVQ);
    param.phyFWAddr = (MS_PHYADDR)phyFW;
    param.u32FWSize = TSP_FW_SIZE;
    param.phyVQAddr = (MS_PHYADDR)phyVQ;
    param.u32VQSize = TSP_VQ_SIZE;//(200*16 + 200*8 + 200*8);

    if (DMX_FILTER_STATUS_ERROR == MApi_DMX_TSPInit(&param))
    {
        printf("DeMux>>ERROR! TSP set fw/vq fail\n");
        return -1;
    }
#endif

    if (DMX_FILTER_STATUS_OK!= MApi_DMX_FlowSet(DMX_FLOW_PLAYBACK, inSrc ,FALSE,TRUE,TRUE))
    {
        printf("DeMux>>ERROR! switch to demod fail\n");
        return -1;
    }

    return 0;
}

#if 0
// Data callback function for event setting.
// Callback function inform user thread the section status.
// If in Polling mode, user thread need to polling filter status by themselves.
static void _appDemo_DataCb(MS_U8 u8DmxId, DMX_EVENT enFilterStatus)
{
    if(enFilterStatus == DMX_EVENT_DATA_READY)
    {
        _s32DataReadyEventId = 1;
    }
    else if(enFilterStatus == DMX_EVENT_BUF_OVERFLOW)
    {
        MApi_DMX_Stop(u8DmxId);
        MApi_DMX_Start(u8DmxId);
    }
}
#endif

static MS_BOOL _Dmx_setup(MS_U32* DmxId,MS_U32 Pid, MS_BOOL bOneShot,MS_U8* ipattern, MS_U8* imask, MS_U8* inmask,MS_BOOL filein)
{
    MS_U8                                  DmxIdSect;
    MS_U16                                 u16Pid;
    MS_U8                                  pattern[16];
    MS_U8                                  mask[16];
    MS_U8                                  nmask[16];
    DMX_FILTER_TYPE                        FilterType;
    MS_U8*                                 pu8BufAddr;
    DMX_Flt_info                           FltInfo;


    u16Pid = Pid ;

    memcpy(pattern,   ipattern, sizeof(pattern));
    memcpy(mask   ,   imask   , sizeof(mask));
    memcpy(nmask  ,   inmask  , sizeof(nmask));

    if (filein)
        FilterType = DMX_FILTER_TYPE_SECTION | DMX_FILTER_SOURCE_TYPE_FILE;
    else
        FilterType = DMX_FILTER_TYPE_SECTION | DMX_FILTER_SOURCE_TYPE_LIVE;

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(FilterType, &DmxIdSect))
    {
        printf("[%s] Allocate filter fail \n",__FUNCTION__);
        goto appDemo_Dmx_setup_Fail ;
    }

    pu8BufAddr = (MS_U8*)Section_Buf_Addr;

    printf("FltInfo.Info.SectInfo.SectBufAddr = 0x%x, 0x%x \n",(MS_U32)pu8BufAddr,MsOS_VA2PA((MS_U32)pu8BufAddr));

    FltInfo.Info.SectInfo.SectBufAddr = MsOS_VA2PA((MS_U32)pu8BufAddr);
    FltInfo.Info.SectInfo.SectBufSize =     SECTION_BUFFER_SIZE;
    FltInfo.Info.SectInfo.Event       =     DMX_EVENT_DATA_READY | DMX_EVENT_CB_POLL | DMX_EVENT_BUF_OVERFLOW;
    FltInfo.Info.SectInfo.pNotify     =    NULL;

    if (bOneShot)
        FltInfo.Info.SectInfo.SectMode =  DMX_SECT_MODE_ONESHOT|DMX_SECT_MODE_CRCCHK ;
    else
       FltInfo.Info.SectInfo.SectMode  =  DMX_SECT_MODE_CRCCHK ;


    if (DMX_FILTER_STATUS_OK != MApi_DMX_Info( DmxIdSect, &FltInfo, &FilterType, TRUE))
    {
        printf("[%s] MApi_DMX_Info fail \n",__FUNCTION__);
        goto appDemo_Dmx_setup_Fail ;
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid( DmxIdSect , &u16Pid , TRUE))
    {
        printf("[%s] MApi_DMX_Pid fail \n",__FUNCTION__);
        goto appDemo_Dmx_setup_Fail ;
    }

    MApi_DMX_SectReset(DmxIdSect);

    MApi_DMX_SectPatternSet(DmxIdSect, pattern, mask, nmask, 16);

    *DmxId = DmxIdSect ;

    return TRUE;

appDemo_Dmx_setup_Fail:

    MApi_DMX_Close(DmxIdSect);
    return FALSE;

}

//-------------------------------------------------------
/// @brief Abstraction for start a filter.
/// @param DmxIdSect: Demux Filter ID
/// @return none
/// @sa
/// @note
//-------------------------------------------------------

static void _Dmx_start(MS_U32 DmxIdSect)
{
    // Start Filter and record section into Section Buffer.
    if (DMX_FILTER_STATUS_OK!= MApi_DMX_Start(DmxIdSect))
    {
        printf("enable section filter fail\n");
    }

}

//-------------------------------------------------------
/// @brief Abstraction for close a filter.
/// @param DmxIdSect: Demux Filter ID
/// @return none
/// @sa
/// @note
//-------------------------------------------------------
static void _Dmx_close(MS_U32 DmxIdSect)
{

    // Stop Filter
    MApi_DMX_Stop(DmxIdSect);

    // Free Filter.
    MApi_DMX_Close(DmxIdSect);

    //MsOS_FreeMemory(pu8BufAddr , 0);


}

// Parse PAT Table and save program service information
MS_BOOL _ScanFilterParsePAT(MS_U8 u8PatId)
{
//    MS_U8       u8PatBuf[1024];
    MS_U32      u32ActualSize = 0;
    MS_U32      u32RmnSize;
    MS_U32      u32LoopLen;
    MS_U8*      pu8Loop;
    MS_U32      i;
    MS_U32      u32ProgNum;

    if (DMX_FILTER_STATUS_OK!= MApi_DMX_CopyData(u8PatId, Data, 1024, &u32ActualSize, &u32RmnSize, NULL))
    {
        printf("[%s][%d] error u32ActualSize = 0x%x, u32RmnSize = 0x%x\n", __FUNCTION__, __LINE__,u32ActualSize,u32RmnSize);
        return FALSE;
    }

    if (u32ActualSize == 0 ) {printf("no data\n") ; return false ;}

    u32LoopLen = ((Data[1] & 0x0f ) << 8) | Data[2];
    if (9> u32LoopLen)
    {
        printf("[%s][%d] error\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    u32LoopLen -= 5; // PAT header
    u32LoopLen -= 4; // CRC
    if (0x3 & u32LoopLen)
    {
        printf("[%s][%d] error\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    pu8Loop = Data + 8;

    stChDB.u32PgNum = 0;
    for (i = 0; i< u32LoopLen >> 2; i++)
    {
        if (MAX_PG_IN_PAT<= stChDB.u32PgNum)
        {
            break;
        }
        u32ProgNum = ((MS_U32)(((MS_U8*)(pu8Loop))[0]<<8)|(((MS_U8*)(pu8Loop))[1]));
        if (0 == u32ProgNum)
        {
            goto pat_next_loop;
        }
        stChDB.stPgList[stChDB.u32PgNum].u32ProgNum = u32ProgNum;
        stChDB.stPgList[stChDB.u32PgNum].u32PidPmt = ((MS_U32)(((MS_U8*)(pu8Loop))[2]<<8)|(((MS_U8*)(pu8Loop))[3])) & 0x1FFF;


        stChDB.u32PgNum++;
pat_next_loop:
        pu8Loop+= 4;
    }
    // printf("[%s][%d] %d programs\n", __FUNCTION__, __LINE__, stChDB.u32PgNum);
    return TRUE;
}


static void _DumpCh(void)
{
    stPG* pstPG = NULL;
    MS_U32 i;
    char AudioFmtStr[][21]={"SiNOT_VALID","SiMPEG","SiAC3","SiMPEG_AD","SiMPEG4","SiAAC","SiAC3P","SiAC3_AD"};

    printf("[%s][%d] start =======================================\n", __FUNCTION__, __LINE__);
    for (i= 0; i< stChDB.u32PgNum; i++)
    {
        printf("[%s][%d] %d\n", __FUNCTION__, __LINE__, (int)i);
        pstPG = &(stChDB.stPgList[i]);
        printf("[%s][%d]     video pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidVideo);
        printf("[%s][%d]     audio pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidAudio);
        printf("[%s][%d]     pcr   pid = 0x%04x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidPcr);
        printf("[%s][%d]     pmt   pid = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32PidPmt);
        printf("[%s][%d]     v    type = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32StreamTypeVideo);
        printf("[%s][%d]     a    type = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32StreamTypeAudio);
        printf("[%s][%d]     prog  num = 0x%08x\n", __FUNCTION__, __LINE__, (int)pstPG->u32ProgNum);
        printf("[%s][%d]     264       = %s\n", __FUNCTION__, __LINE__, (pstPG->b264)? "TRUE": "FALSE");
        printf("[%s][%d]     AudioFmt  = %s\n", __FUNCTION__, __LINE__, AudioFmtStr[pstPG->eAudioFmt]);
    }
    printf("[%s][%d] end   =======================================\n", __FUNCTION__, __LINE__);
}

static stPG* _ScanPgFind(MS_U32 u32ProgNum)
{
    MS_U32 i;
    for (i= 0; i< stChDB.u32PgNum; i++)
    {
        if (u32ProgNum == stChDB.stPgList[i].u32ProgNum)
        {
            return &(stChDB.stPgList[i]);
        }
    }
    return NULL;

}

//---------
MS_BOOL _ScanFilterParsePMT(MS_U8 u8PmtId)
{
    MS_U8       u8PmtBuf[1024];
    MS_U32      u32ActualSize;
    MS_U32      u32RmnSize;
    MS_U32      s32LoopLen;
    MS_U8*      pu8Loop;
    MS_U32      u32ProgInfoLen;
    stPG*       pstPG= NULL;
    MS_U32      u32Pid;
    MS_U32      u32Pcr;
    MS_U32      u32ProgNum;
    MS_U32      u32TempLoopLen;

    if (DMX_FILTER_STATUS_OK!= MApi_DMX_CopyData(u8PmtId, u8PmtBuf, 1024, &u32ActualSize, &u32RmnSize, NULL))
    {
        printf("[%s][%d] error\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if (u8PmtBuf[0] != 0x2 )
    {
        return FALSE ;
    }

    s32LoopLen = ((u8PmtBuf[1] & 0x0f ) << 8) | u8PmtBuf[2];
    if (12 > s32LoopLen)
    {
        return FALSE;
    }
    u32ProgInfoLen = ((u8PmtBuf[10] & 0x0f ) << 8) | u8PmtBuf[11];
    if ((13 + u32ProgInfoLen)> s32LoopLen)
    {
        return FALSE;
    }
    s32LoopLen -= 9; // PMT header
    s32LoopLen -= u32ProgInfoLen; // program_info_length
    s32LoopLen -= 4; // CRC

    u32Pcr = ((MS_U32)(((MS_U8*)(u8PmtBuf))[8]<<8)|(((MS_U8*)(u8PmtBuf))[9])) & 0x1FFF;
    u32ProgNum = (u8PmtBuf[3] << 8) | u8PmtBuf[4];

    pu8Loop = u8PmtBuf + 12 + u32ProgInfoLen;


    if (NULL == (pstPG= _ScanPgFind(u32ProgNum)))
    {
        return FALSE;
    }

    pstPG->u32PidPcr = u32Pcr;
    while (5 <= s32LoopLen)
    {
        u32TempLoopLen = ((pu8Loop[3] & 0x0f ) << 8) | pu8Loop[4];
        u32Pid = ((MS_U32)(((MS_U8*)(pu8Loop))[1]<<8)|(((MS_U8*)(pu8Loop))[2])) & 0x1FFF;
        switch (pu8Loop[0])
        {
        case E_SI_STREAM_AVCH264_VID:
            pstPG->b264 = TRUE;
        case E_SI_STREAM_MPEG1_VID:
        case E_SI_STREAM_MPEG2_VID:
            if (!pstPG->u32PidVideo)
            {
                pstPG->u32PidVideo = u32Pid;
                pstPG->u32StreamTypeVideo = pu8Loop[0];
            }
            break;
            // [5-2] Audio
        case E_SI_STREAM_MPEG1_AUD:
            // MPEG-1 Audio
        case E_SI_STREAM_MPEG2_AUD:
            // MPEG-2 Audio
        case E_SI_STREAM_AC3_AUD:
            // 13818-7 AAC
        case E_SI_STREAM_AAC_AUD:
            // 14496-3 MPEG4
        case E_SI_STREAM_MPEG4_AUD:
            if (!pstPG->u32PidAudio)
            {
                pstPG->u32PidAudio = u32Pid;
                pstPG->u32StreamTypeAudio = pu8Loop[0];
                if ( pu8Loop[0] == E_SI_STREAM_AC3_AUD )
                {
                    pstPG->eAudioFmt = SiAC3;
                }
                else if(pu8Loop[0] == E_SI_STREAM_MPEG4_AUD)
                {
                    pstPG->eAudioFmt = SiMPEG4;
                }
                else if(pu8Loop[0] == E_SI_STREAM_AAC_AUD)
                {
                    pstPG->eAudioFmt = SiAAC;
                }
                else
                {
                    pstPG->eAudioFmt = SiMPEG;
                }
            }
            break;
        default:
            break;
        }
        pu8Loop += (5+ u32TempLoopLen);
        s32LoopLen -= (5+ u32TempLoopLen);
    }
    return TRUE;
}


/*
 * Subroutine:  do_tuner_init
 *
 * Description: Handler for 'tuner' command to initialization..
 *
 * Inputs:	argv[1] - NULL
 *
 * Return:      0:success; -1:failure
 *
 */
int do_dmx_scan(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{


    MS_U32 DmxId;
    MS_U32 u32Event = 0 ;
    MS_U32 i ;
    DMX_FILTER_STATUS procRet ;
    MS_U8  pattern[16],mask[16],nmask[16];
    DMX_EVENT eEvent = DMX_EVENT_DATA_READY;

    printf ("do_dmx_scan \n");

    _s32DataReadyEventId = 0;
    memset(pattern , 0x0 , sizeof(pattern));
    memset(mask    , 0x0 , sizeof(mask));
    memset(nmask   , 0x0 , sizeof(nmask));

     if (!_Dmx_setup(&DmxId,0x0,true,pattern,mask,nmask,false))
    {
        goto DMX_SCAN_FAIL;
    }

    printf("dmx_set pat up ... \n");
    // Start section filter
    _Dmx_start(DmxId);
    printf("dmx pat start ...  \n");
    // wait until data ready event or timeout
    //MsOS_WaitEvent(_s32EventId, _appDemo_Data_Ready, &u32Event, E_OR_CLEAR, 2000);
    //WAIT_DATA_EVENT(DmxId, u32Event);

    while(1)
    {
            procRet = MApi_DMX_Proc(DmxId,&eEvent) ;
            if (procRet==DMX_FILTER_STATUS_OK)
            {
                if (eEvent == DMX_EVENT_DATA_READY)
                {
                    u32Event = 1;
                    break;

                }
                else
                {
                    printf("Polling OVERFLOW = %u\n", (U8)DmxId);
                    MApi_DMX_Stop(DmxId);
                    MApi_DMX_Reset();
                    MApi_DMX_Start(DmxId);
                }
            }
    }

    if (!u32Event)
    {
        printf("Wait PAT time out ! \n");
        _Dmx_close(DmxId);
        goto DMX_SCAN_FAIL;
    }


    printf("dmx_copy data ...\n");
    _ScanFilterParsePAT(DmxId);
    _Dmx_close(DmxId);

    for (i= 0; i< stChDB.u32PgNum; i++)
    {
        _Dmx_setup(&DmxId ,stChDB.stPgList[i].u32PidPmt,true,pattern,mask,nmask,false);
        _Dmx_start(DmxId);
        //MsOS_WaitEvent(_s32EventId, _appDemo_Data_Ready, &u32Event, E_OR_CLEAR, 5000);
        //WAIT_DATA_EVENT(DmxId, u32Event);
        while(1)
        {
            procRet = MApi_DMX_Proc(DmxId,&eEvent) ;
            if (procRet==DMX_FILTER_STATUS_OK)
            {
                if (eEvent == DMX_EVENT_DATA_READY)
                {
                    u32Event = 1;
                    break;

                }
                else
                {
                    printf("Polling OVERFLOW = %u\n", (U8)DmxId);
                    MApi_DMX_Stop(DmxId);
                    MApi_DMX_Reset();
                    MApi_DMX_Start(DmxId);
                }
            }
        }

        if (u32Event)
        {
            _ScanFilterParsePMT(DmxId);
        }
        _Dmx_close(DmxId);
    }


    _DumpCh();

    return TRUE ;

DMX_SCAN_FAIL:
    return FALSE;
    return 0;
}

U_BOOT_CMD(
	dmx_init,	2,	1,	do_dmx_init,
	"dmx_init     - initialize the demux setting\n",
	"<arg[1]>: dmx flow input source => 0:demodulator; 1:memory; 2:ext input0; 3:ext input1\n"
);


U_BOOT_CMD(
	dmx_scan,	1,	1,	do_dmx_scan,
	"dmx_scan     - Scan program information\n",
	"<arg>: none\n"
);


#endif /* (CONFIG_COMMANDS) */
