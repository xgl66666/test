/*
 * tdal_pvr_mem.c
 *
 *  Created on: Mar 7, 2013
 *      Author: rtrk
 */
#include "crules.h"
#include "MsTypes.h"
#include "MsCommon.h"
#include "MsMemory.h"
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <cyg/fileio/fileio.h>
#include <errno.h>

#include "drvDSCMB.h"
#include "drvBDMA.h"
#include "drvAUDIO_if.h"
#include "apiPVR.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "apiAUDIO.h"
#include "apiHDMITx.h"
#include "MsFS.h"

#include "tdal_common.h"
#include "tbox.h"
#include "tdal_pvr.h"
#include "tdal_pvr_p.h"

#define TDAL_PVR_USE_TDAL_FS

#if defined(TDAL_PVR_USE_TDAL_FS)
#include "tdal_fs.h"
#endif

/********************************************************
   *   Defines                        *
********************************************************/
//#define PVR_ALLOC_FROM_VD

/********************************************************
   *   Macros                        *
********************************************************/
mTBOX_SET_MODULE(eTDAL_PVR_EM);
/********************************************************
   *   Local   File   Variables   (LOCAL)            *
********************************************************/
#define ENABLE_STATICMEM_PVR
#if(defined(ENABLE_STATICMEM_PVR) && (MEMORY_MAP == MMAP_128MB))
#define PVR_DOWNLOAD_LEN                                0x0000360000*4         /* 3.375 M*/
#define PVR_UPLOAD_LEN                                  0x0000120000         /* 1.125 M*/
#else
/* VDPLAYER_MEM   */
#define PVR_DOWNLOAD_LEN                                0x0000360000         /* 3.375 M*/
#define PVR_UPLOAD_LEN                                  0x0000120000         /* 1.125 M*/
#define PVR_AUDIO_BUFFER_LEN                            (0x00010000)       /* 64 KB*/
#define PVR_MN_VIDEO_BUFFER_LEN                         (0x000300000)
#define PVR_MN_AUDIO_BUFFER_LEN                         (0x00010000)
#define PVR_TSR_VIDEO_BUFFER_LEN                        (0x000360000)
#define PVR_TSR_AUDIO_BUFFER_LEN                        (0x00020000)
#endif

LOCAL void *  pTDAL_PVR_Rec = NULL;
LOCAL void *  pTDAL_PVR_Play = NULL;
LOCAL void *  pTDAL_PVR_AudioBuf = NULL;
LOCAL void *  pTDAL_PVR_MnVid = NULL;
LOCAL void *  pTDAL_PVR_MnAud = NULL;
LOCAL void *  pTDAL_PVR_TsrVid = NULL;
LOCAL void *  pTDAL_PVR_TsrAud = NULL;

LOCAL void *  pTDAL_PVR_AesBuf = NULL;
extern APIPVR_PLAYBACK_TYPE eTDAL_PVR_PlayType;
extern APIPVR_RECORD_TYPE   eTDAL_PVR_RecordType;
LOCAL uint8_t u8CryptKey[4] = {0};
/********************************************************
   *   Module   variables   (MODULE)               *
********************************************************/
extern void*  AllocNoncachedMem(int size);
extern void   FreeNoncachedMem(void* pBuf);

void* TDAL_PVRm_MEMALLOC_FUNC(MS_U32 u32Size)
{
    return AllocNoncachedMem (u32Size);
}

MS_BOOL TDAL_PVRm_MEMFREE_FUNC(void *pBuf)
{
    FreeNoncachedMem(pBuf);
    return TRUE;
}

MS_U32 TDAL_PVRm_MEMPA2VA_FUNC(MS_U32 u32PhyAdr)
{
    MS_U32 u32VirAdr=MsOS_PA2KSEG1(u32PhyAdr);
    return u32VirAdr;
}

void TDAL_PVRm_MEMFLUSH_FUNC(MS_U32 u32Adr,MS_U32 u32Len)
{
    MsOS_Sync();
    MsOS_FlushMemory();
}

#if( defined(NAGRA_CHIP_5C35) && (NAGRA_CHIP_5C35 == 1))
MS_BOOL TDAL_PVRm_KLADDER_FUNC(void)
{
    MS_U8 u8KLInputKey[32]={0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
                            0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f};
    DSCMB_KL_Status KL_Status = 0;
    MS_U8 ACPU_Out[16] = {0x00};

	DSCMB_KLCfg_All KLConfigAll = {
		.eAlgo = E_DSCMB_KL_TDES,
		.eSrc = E_DSCMB_KL_SRC_SECRET_4,
		.eDst = E_DSCMB_KL_DST_PRIVATE_1,
		.eOutsize = E_DSCMB_KL_128_BITS,
		.eKeyType = 0,
		.u32Level = 2,
		.u32EngID = 0,
		.u32DscID = 0,
		.u8KeyACPU = 0,
		.pu8KeyKLIn = u8KLInputKey,
		.bDecrypt = TRUE,
		.bInverse = FALSE,
		.eKLSel = E_DSCMB_KL_SEL_CW,
		.u32CAVid = 0x02, //NARGA_CAVID
    };
    // Key Ladder
	if( MDrv_DSCMB2_KLadder_AtomicExec(&KLConfigAll , ACPU_Out, &KL_Status ) == FALSE )
	{
		printf("Key Ladder: Fail!!! 0x%x\n",(unsigned int)KL_Status);
		return FALSE;
	}

    KLConfigAll.eSrc     = E_DSCMB_KL_SRC_SECRET_6;
	KLConfigAll.eDst     = E_DSCMB_KL_DST_DMA_SK0;
	KLConfigAll.u32Level = 0;
	KLConfigAll.eKLSel	 = E_DSCMB_KL_SEL_PVR;
	if( MDrv_DSCMB2_KLadder_AtomicExec(&KLConfigAll , ACPU_Out, &KL_Status ) == FALSE )
	{
		printf("Key Ladder: Fail!!! 0x%x\n",(unsigned int)KL_Status);
		return FALSE;
	}

    return TRUE;
}
#elif( defined(NAGRA_CHIP_5029) && (NAGRA_CHIP_5029 == 1))
MS_BOOL TDAL_PVRm_KLADDER_FUNC(void)
{
    MS_U8 u8Key[48]={0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
                     0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
                     0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f};

    MS_BOOL bRet=FALSE;
    DSCMB_KL_Status u32Status;
    DSCMB_KLCfg_All KLCfg;

    KLCfg.bDecrypt   = TRUE;
    KLCfg.bInverse   = FALSE;
    // Key ladder running level, support level 0 ~ 3 // [NOTE] Level 0 is only supported by  CryptoDMA
    KLCfg.u32Level   = 3;
    KLCfg.eAlgo      = E_DSCMB_KL_AES;           // Select Key ladder key generation algorithm //
    KLCfg.eSrc       = E_DSCMB_KL_SRC_SECRET_2;   // Select root key source //
    KLCfg.eDst       = E_DSCMB_KL_DST_DMA_AES;    // Select output Key destination //
    KLCfg.eOutsize   = E_DSCMB_KL_128_BITS;   // Select ouput Key size //
    KLCfg.u8KeyACPU  = NULL;                 // ACPU key as KL root key //
    KLCfg.pu8KeyKLIn = u8Key;

    bRet = MDrv_DSCMB_KLadder_AtomicExec(&KLCfg , NULL, &u32Status );

    return bRet;

}
#else  //non CA chip (mstar demo board)
MS_BOOL TDAL_PVRm_KLADDER_FUNC(void)
{
    MS_BOOL ret = FALSE;
    MS_U8 u8KLInputKey[48]={0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
                            0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
                            0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f};
    MS_U8 key[16] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};
    DSCMB_KL_Status KL_Status = 0;
    MS_U8 ACPU_Out[16] = {0x00};

    DSCMB_KLCfg_All KLConfigAll = {
        .eAlgo = E_DSCMB_KL_AES,
        .eSrc = E_DSCMB_KL_SRC_ACPU,
        .eDst = E_DSCMB_KL_DST_DMA_AES,
        .eOutsize = E_DSCMB_KL_128_BITS,
        .eKeyType = E_DSCMB_KEY_CLEAR,      // Don't care when dst is Crypto DMA
        .u32Level = 3,
        .u32EngID = 0,                      // Don't care when dst is Crypto DMA
        .u32DscID = 0,                      // Don't care when dst is Crypto DMA
        .u8KeyACPU = key,                   // Don't care when src is NOT ACPU
        .pu8KeyKLIn = u8KLInputKey,
        .bDecrypt = TRUE,
        .bInverse = FALSE,
    };

    // Key Ladder
    ret = MDrv_DSCMB_KLadder_AtomicExec(&KLConfigAll , ACPU_Out, &KL_Status );
    if(ret == FALSE)
    {
        printf("Key Ladder: Fail!!!\n");
        return FALSE;
    }

    return TRUE;
}
#endif

void TDAL_PVRm_FreezeScreen(MS_BOOL bFrezon)
{
    MApi_VDEC_SetFreezeDisp(bFrezon);
}

MS_BOOL TDAL_PVRm_BatchFreeMem()
{
    MS_U8 result = 0;
    if (TDAL_PVRm_FreeMem(PVR_ID_DOWNLOAD) == FALSE) result++;
    if (TDAL_PVRm_FreeMem(PVR_ID_UPLOAD) == FALSE) result++;
    if (TDAL_PVRm_FreeMem(PVR_ID_BUFFER) == FALSE) result++;
#if 0

    if(eTDAL_PVR_PlayType == EN_APIPVR_PLAYBACK_TYPE_ESPLAYER)
    {
        if (TDAL_PVRm_FreeMem(PVR_ID_ES_VIDEO_BUFFER) == FALSE) result++;
        if (TDAL_PVRm_FreeMem(PVR_ID_ES_AUDIO_BUFFER) == FALSE) result++;
        if (TDAL_PVRm_FreeMem(PVR_ID_MN_VIDEO_BUFFER) == FALSE) result++;
        if (TDAL_PVRm_FreeMem(PVR_ID_MN_AUDIO_BUFFER) == FALSE) result++;
        if (TDAL_PVRm_FreeMem(PVR_ID_TSR_VIDEO_BUFFER) == FALSE) result++;
        if (TDAL_PVRm_FreeMem(PVR_ID_TSR_AUDIO_BUFFER) == FALSE) result++;
    }
 #endif   
    return ((result == 0) ? TRUE: FALSE);
}

MS_BOOL TDAL_PVRm_FreeMem(int pvrId)
{
    switch(pvrId)
    {
        case PVR_ID_DOWNLOAD:
            #if(defined(ENABLE_STATICMEM_PVR) && (MEMORY_MAP == MMAP_128MB))

            #else
            mTBOX_TRACE((kTBOX_NIV_1, "PVR_ID_DOWNLOAD\n"));
            if(pTDAL_PVR_Rec != NULL)
            {
#ifdef PVR_ALLOC_FROM_VD
#else
                mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] --> try to free _pRec\n",__FUNCTION__,__LINE__));
                if(!TDAL_PVRm_MEMFREE_FUNC(pTDAL_PVR_Rec))
                {
                    printf("_pRec Free Fail\n");
                    return FALSE;
                }
#endif
                pTDAL_PVR_Rec = NULL;
            }
            #endif
            break;
        case PVR_ID_UPLOAD:
            #if(defined(ENABLE_STATICMEM_PVR) && (MEMORY_MAP == MMAP_128MB))

            #else
            mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] _pPlay=%p\n",__FUNCTION__,__LINE__,pTDAL_PVR_Play));
            if(pTDAL_PVR_Play != NULL)
            {
#ifdef PVR_ALLOC_FROM_VD
#else
                mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] --> try to free _pPlay\n",__FUNCTION__,__LINE__));
                if(!TDAL_PVRm_MEMFREE_FUNC(pTDAL_PVR_Play))
                {
                    printf("_pPlay Free Fail\n");
                    return FALSE;
                }
#endif
                pTDAL_PVR_Play = NULL;
            }
            #endif
            break;

        case PVR_ID_BUFFER:
            mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] _pAesBuf=%p\n",__FUNCTION__,__LINE__,pTDAL_PVR_AesBuf));
            if(pTDAL_PVR_AesBuf != NULL)
            {
#ifdef PVR_ALLOC_FROM_VD
#else
                mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] --> try to free _pAesBuf\n",__FUNCTION__,__LINE__));
                if(!TDAL_PVRm_MEMFREE_FUNC(pTDAL_PVR_AesBuf))
                {
                    printf("_pAesBuf Free Fail\n");
                    return FALSE;
                }
#endif
                pTDAL_PVR_AesBuf = NULL;
            }
            break;

        case PVR_ID_ES_VIDEO_BUFFER:
            return TRUE;
            break;
        case PVR_ID_ES_AUDIO_BUFFER:
            #if(defined(ENABLE_STATICMEM_PVR) && (MEMORY_MAP == MMAP_128MB))

            #else
            mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] _pAudioBuf=%p\n",__FUNCTION__,__LINE__,pTDAL_PVR_AudioBuf));
            if(pTDAL_PVR_AudioBuf != NULL)
            {
                if(!TDAL_PVRm_MEMFREE_FUNC(pTDAL_PVR_AudioBuf))
                {
                    printf("_pAudioBuf Free Fail\n");
                    return FALSE;
                }
                pTDAL_PVR_AudioBuf = NULL;
            }
            #endif
            break;
        case PVR_ID_MN_VIDEO_BUFFER:
            #if(defined(ENABLE_STATICMEM_PVR) && (MEMORY_MAP == MMAP_128MB))

            #else
            mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] _pMnVid=%p\n",__FUNCTION__,__LINE__,pTDAL_PVR_MnVid));
            if(pTDAL_PVR_MnVid != NULL)
            {
                if(!TDAL_PVRm_MEMFREE_FUNC(pTDAL_PVR_MnVid))
                {
                    printf("_pAudioBuf Free Fail\n");
                    return FALSE;
                }
                pTDAL_PVR_MnVid = NULL;
            }
            #endif
            break;
        case PVR_ID_MN_AUDIO_BUFFER:
            #if(defined(ENABLE_STATICMEM_PVR) && (MEMORY_MAP == MMAP_128MB))

            #else
            mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] _pMnAud=%p\n",__FUNCTION__,__LINE__,pTDAL_PVR_MnAud));
            if(pTDAL_PVR_MnAud != NULL)
            {
                if(!TDAL_PVRm_MEMFREE_FUNC(pTDAL_PVR_MnAud))
                {
                    printf("_pAudioBuf Free Fail\n");
                    return FALSE;
                }
                pTDAL_PVR_MnAud = NULL;
            }
            #endif
            break;
        case PVR_ID_TSR_VIDEO_BUFFER:
            #if(defined(ENABLE_STATICMEM_PVR) && (MEMORY_MAP == MMAP_128MB))

            #else
            mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] _pTsrVid=%p\n",__FUNCTION__,__LINE__,pTDAL_PVR_TsrVid));
            if(pTDAL_PVR_TsrVid != NULL)
            {
                if(!TDAL_PVRm_MEMFREE_FUNC(pTDAL_PVR_TsrVid))
                {
                    printf("_pAudioBuf Free Fail\n");
                    return FALSE;
                }
                pTDAL_PVR_TsrVid = NULL;
            }
            #endif
            break;
        case PVR_ID_TSR_AUDIO_BUFFER:
            #if(defined(ENABLE_STATICMEM_PVR) && (MEMORY_MAP == MMAP_128MB))

            #else
            mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] _pTsrAud=%p\n",__FUNCTION__,__LINE__,pTDAL_PVR_TsrAud));
            if(pTDAL_PVR_TsrAud != NULL)
            {
                if(!TDAL_PVRm_MEMFREE_FUNC(pTDAL_PVR_TsrAud))
                {
                    printf("_pAudioBuf Free Fail\n");
                    return FALSE;
                }
                pTDAL_PVR_TsrAud = NULL;
            }
            #endif
            break;

        default:
            printf("[%s][%d] Should not be here. pvrId=%d\n",__FUNCTION__,__LINE__,pvrId);
            return FALSE;
            break;
    }
    
    return MApi_PVR_ResetMmap(pvrId);
}

MS_BOOL TDAL_PVRm_BatchAllocateMem()
{
    if(!TDAL_PVRm_AllocateMem(PVR_ID_DOWNLOAD))
    {
        return FALSE;
    }

    if(!TDAL_PVRm_AllocateMem(PVR_ID_UPLOAD))
    {
        return FALSE;
    }
    if(!TDAL_PVRm_AllocateMem(PVR_ID_BUFFER))
    {
        return FALSE;
    }
#if 0

    if(eTDAL_PVR_PlayType==EN_APIPVR_PLAYBACK_TYPE_ESPLAYER)
    {
        if(!TDAL_PVRm_AllocateMem(PVR_ID_ES_VIDEO_BUFFER))
        {
            return FALSE;
        }
        if(!TDAL_PVRm_AllocateMem(PVR_ID_ES_AUDIO_BUFFER))
        {
            return FALSE;
        }
        if(!TDAL_PVRm_AllocateMem(PVR_ID_MN_VIDEO_BUFFER))
        {
            return FALSE;
        }

        if(!TDAL_PVRm_AllocateMem(PVR_ID_MN_AUDIO_BUFFER))
        {
            return FALSE;
        }
        if(!TDAL_PVRm_AllocateMem(PVR_ID_TSR_VIDEO_BUFFER))
        {
            return FALSE;
        }

        if(!TDAL_PVRm_AllocateMem(PVR_ID_TSR_AUDIO_BUFFER))
        {
            return FALSE;
        }
    }
#endif
    return TRUE;    
}

MS_BOOL TDAL_PVRm_AllocateMem(int pvrId)
{
#if 1
    MS_U32 u32PhyStartAdr=0;
    MS_U32 u32Size=0;
    switch(pvrId)
    {
        case PVR_ID_DOWNLOAD:
#if(defined(ENABLE_STATICMEM_PVR) && (MEMORY_MAP == MMAP_128MB))
            u32PhyStartAdr = PVR_DOWNLOAD_STATIC_ADR;
            memset((void *)MsOS_MPool_PA2KSEG0(u32PhyStartAdr), 0, PVR_DOWNLOAD_LEN);
#else
#ifdef PVR_ALLOC_FROM_VD
            u32PhyStartAdr=VDPLAYER_DATA_ADR;
//            mTBOX_ASSERTm(PVR_DOWNLOAD_LEN < VDPLAYER_DATA_LEN, "PVR_DOWNLOAD_LEN greater than VDPLAYER_DATA_LEN. This can cause random crashes!!!!");
#else
            if(pTDAL_PVR_Rec == NULL)
            {
                pTDAL_PVR_Rec = TDAL_PVRm_MEMALLOC_FUNC(PVR_DOWNLOAD_LEN);
                if(pTDAL_PVR_Rec == NULL)
                {
                    printf("[%s][%d] _pRec HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)pTDAL_PVR_Rec);
#endif
            #endif

            u32Size = PVR_DOWNLOAD_LEN;

            mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] PVR_ID_DOWNLOAD %8lX (0x%8lX+0x%8lX=0x%8lX) \n",__FUNCTION__,__LINE__,pTDAL_PVR_Rec,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size)));

            break;
        case PVR_ID_UPLOAD:
#if(defined(ENABLE_STATICMEM_PVR) && (MEMORY_MAP == MMAP_128MB))
            u32PhyStartAdr = PVR_UPLOAD_STATIC_ADR;
            memset((void *)MsOS_MPool_PA2KSEG0(u32PhyStartAdr), 0, PVR_UPLOAD_LEN);
#else
#ifdef PVR_ALLOC_FROM_VD
            u32PhyStartAdr=(VDPLAYER_DATA_ADR+PVR_DOWNLOAD_LEN);//VDPLAYER_BS_ADR;
//            mTBOX_ASSERTm(PVR_UPLOAD_LEN < VDPLAYER_BS_LEN, "PVR_UPLOAD_LEN greater than VDPLAYER_BS_LEN. This can cause random crashes!!!!");
#else
            if(pTDAL_PVR_Play == NULL)
            {
                pTDAL_PVR_Play= TDAL_PVRm_MEMALLOC_FUNC(PVR_UPLOAD_LEN);
                if(pTDAL_PVR_Play == NULL)
                {
                    printf("[%s][%d] _pPlay HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)pTDAL_PVR_Play);
#endif
#endif
            u32Size = PVR_UPLOAD_LEN;
            mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] PVR_ID_UPLOAD %8lX (0x%8lX+0x%8lX=0x%8lX) \n",__FUNCTION__,__LINE__,pTDAL_PVR_Play,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size)));
            break;

        case PVR_ID_BUFFER:
#ifdef PVR_ALLOC_FROM_VD
//            mTBOX_ASSERTm(PVR_AESBUFFER_LEN < (VDPLAYER_BS_LEN-PVR_UPLOAD_LEN), "PVR_AESBUFFER_LEN greater than VDPLAYER_BS_LEN. This can cause random crashes!!!!");
            u32PhyStartAdr=(VDPLAYER_DATA_ADR+PVR_DOWNLOAD_LEN)+PVR_UPLOAD_LEN;//VDPLAYER_BS_ADR+PVR_UPLOAD_LEN;
#else
            if(pTDAL_PVR_AesBuf == NULL)
            {
                pTDAL_PVR_AesBuf = TDAL_PVRm_MEMALLOC_FUNC(PVR_AESBUFFER_LEN);
                if(pTDAL_PVR_AesBuf == NULL)
                {
                    printf("[%s][%d] _pAesBuf HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)pTDAL_PVR_AesBuf);
#endif
            u32Size = PVR_AESBUFFER_LEN;
            mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] PVR_ID_BUFFER %8lX (0x%8lX+0x%8lX=0x%8lX)\n",__FUNCTION__,__LINE__,pTDAL_PVR_AesBuf,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size)));
            break;
#if 0
        case PVR_ID_ES_VIDEO_BUFFER:
#if ENABLE_MIU_1
            u32PhyStartAdr = VDEC_BIT_STREAM_ADR + MIU_INTERVAL;
#else
            u32PhyStartAdr = VDEC_BIT_STREAM_ADR;
#endif
            u32Size = VDEC_BIT_STREAM_LEN;
            mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] PVR_ID_ES_VIDEO_BUFFER (0x%8lX+0x%8lX=0x%8lX)\n",__FUNCTION__,__LINE__,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size)));

            break;
        case PVR_ID_ES_AUDIO_BUFFER: // MIU1
            #if(defined(ENABLE_STATICMEM_PVR) && (MEMORY_MAP == MMAP_128MB))
            u32PhyStartAdr = PVR_AUDIO_BUFFER_ADR;
            #else
            if(pTDAL_PVR_AudioBuf == NULL)
            {
                pTDAL_PVR_AudioBuf= TDAL_PVRm_MEMALLOC_FUNC(PVR_AUDIO_BUFFER_LEN);
                if(pTDAL_PVR_AudioBuf == NULL)
                {
                    printf("[%s][%d] _pAudioBuf HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)pTDAL_PVR_AudioBuf);
            #endif
            u32Size = PVR_AUDIO_BUFFER_LEN;
            mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] PVR_ID_ES_AUDIO_BUFFER %8lX (0x%8lX+0x%8lX=0x%8lX)\n",__FUNCTION__,__LINE__,pTDAL_PVR_AudioBuf,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size)));
            break;

        case PVR_ID_MN_VIDEO_BUFFER:
            #if(defined(ENABLE_STATICMEM_PVR) && (MEMORY_MAP == MMAP_128MB))
            u32PhyStartAdr = PVR_MN_VIDEO_BUFFER_ADR;
            #else
            if(pTDAL_PVR_MnVid== NULL)
            {
                pTDAL_PVR_MnVid= TDAL_PVRm_MEMALLOC_FUNC(PVR_MN_VIDEO_BUFFER_LEN);
                if(pTDAL_PVR_MnVid == NULL)
                {
                    printf("[%s][%d] _pMnVid HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)pTDAL_PVR_MnVid);
            #endif
            u32Size = PVR_MN_VIDEO_BUFFER_LEN;
            mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] PVR_ID_MN_VIDEO_BUFFER %8lX (0x%8lX+0x%8lX=0x%8lX)\n", __FUNCTION__, __LINE__, pTDAL_PVR_MnVid,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size)));
            break;
        case PVR_ID_MN_AUDIO_BUFFER:
            #if(defined(ENABLE_STATICMEM_PVR) && (MEMORY_MAP == MMAP_128MB))
            u32PhyStartAdr = PVR_MN_AUDIO_BUFFER_ADR;
            #else
            if(pTDAL_PVR_MnAud== NULL)
            {
                pTDAL_PVR_MnAud= TDAL_PVRm_MEMALLOC_FUNC(PVR_MN_AUDIO_BUFFER_LEN);
                if(pTDAL_PVR_MnAud == NULL)
                {
                    printf("[%s][%d] _pMnAud HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)pTDAL_PVR_MnAud);
            #endif
            u32Size = PVR_MN_AUDIO_BUFFER_LEN;
            mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] PVR_ID_MN_AUDIO_BUFFER %8lX (0x%8lX+0x%8lX=0x%8lX)\n",__FUNCTION__,__LINE__,pTDAL_PVR_MnAud,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size)));
            break;

        case PVR_ID_TSR_VIDEO_BUFFER:
            #if(defined(ENABLE_STATICMEM_PVR) && (MEMORY_MAP == MMAP_128MB))
            u32PhyStartAdr = PVR_TSR_VIDEO_BUFFER_ADR;
            #else
            if(pTDAL_PVR_TsrVid== NULL)
            {
                pTDAL_PVR_TsrVid= TDAL_PVRm_MEMALLOC_FUNC(PVR_TSR_VIDEO_BUFFER_LEN);
                if(pTDAL_PVR_TsrVid == NULL)
                {
                    printf("[%s][%d] _pTsrVid HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)pTDAL_PVR_TsrVid);
            #endif
            u32Size = PVR_TSR_VIDEO_BUFFER_LEN;
            mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] PVR_ID_TSR_VIDEO_BUFFER %8lX (0x%8lX+0x%8lX=0x%8lX)\n",__FUNCTION__,__LINE__,pTDAL_PVR_TsrVid,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size)));
            break;

        case PVR_ID_TSR_AUDIO_BUFFER:
            #if(defined(ENABLE_STATICMEM_PVR) && (MEMORY_MAP == MMAP_128MB))
            u32PhyStartAdr = PVR_TSR_AUDIO_BUFFER_ADR;
            #else
            if(pTDAL_PVR_TsrAud== NULL)
            {
                pTDAL_PVR_TsrAud= TDAL_PVRm_MEMALLOC_FUNC(PVR_TSR_AUDIO_BUFFER_LEN);
                if(pTDAL_PVR_TsrAud == NULL)
                {
                    printf("[%s][%d] _pTsrAud HB_AllocateMemory failed\n",__FUNCTION__,__LINE__);
                    return FALSE;
                }
            }
            u32PhyStartAdr=MsOS_MPool_VA2PA((MS_U32)pTDAL_PVR_TsrAud);
            #endif
            u32Size = PVR_TSR_AUDIO_BUFFER_LEN;
            mTBOX_TRACE((kTBOX_NIV_1, "[%s][%d] PVR_ID_TSR_AUDIO_BUFFER %8lX (0x%8lX+0x%8lX=0x%8lX)\n",__FUNCTION__,__LINE__,pTDAL_PVR_TsrAud,u32PhyStartAdr,u32Size,(u32PhyStartAdr+u32Size)));
            break;
#endif
        default:
            printf("[%s][%d] Should not be here. pvrId=%d\n",__FUNCTION__,__LINE__,pvrId);
            return FALSE;
            break;
    }

    if(MApi_PVR_Mmap(pvrId,u32PhyStartAdr,u32Size)==FALSE)
    {
        printf("[%s][%d] PVR Mmap failed\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
#endif
    return TRUE;
}

MS_BOOL  TDAL_PVRm_CheckUSB(char *mountPath)
{
    //TODO:
    int ret=-1;
    DIR *dp=NULL;
    dp = MsFS_OpenDir(mountPath);
    if(dp == NULL)
    {
        ret = MsFS_MkDir(mountPath, 0755);
        if(ret)
        {
            if(errno != EEXIST)
            {
                printf("[PVR ERROR] : fail to mkdir %s.\n", mountPath);
                return FALSE;
            }
            else
            {
                printf("[PVR] : %s exists\n", mountPath);
            }
        }
    }
    else
    {
        MsFS_CloseDir(dp);
    }
    return TRUE;
}

MS_BOOL  TDAL_PVRm_GetDiskSpace(char *mountPath,MS_U64* u64FreeSpaceInKB, MS_U64* u64TotalSpaceInKB)
{
#if defined(TDAL_PVR_USE_TDAL_FS)
    tTDAL_FS_FsInfo fsInfo;
    tTDAL_FS_Error  error;
#else
    MApi_FsInfo fsInfo;
#endif

    if(mountPath==NULL)
    {
        printf("[%s][%d] mountPath==NULL\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

#if defined(TDAL_PVR_USE_TDAL_FS)
    error = TDAL_FS_GetFsInfo(mountPath, &fsInfo);
    if(error == eTDAL_FS_NO_ERROR)
    {
        *u64FreeSpaceInKB = fsInfo.freeBlocks >> 1;
        *u64TotalSpaceInKB = fsInfo.totalSize  >> 1;

        return TRUE;
    }
#else
    TDAL_PVRm_CheckUSB(mountPath);
    mTBOX_TRACE((kTBOX_NIV_1,"[%s][%d] mountPath=%s\n",__FUNCTION__,__LINE__, mountPath));

    if(MApi_FS_Info((char *)mountPath, FALSE, &fsInfo)==TRUE)
    {
        *u64TotalSpaceInKB = (((MS_U64)fsInfo.u32ClusTotal* (MS_U64)fsInfo.u32ClusSize) / 1024);
        *u64FreeSpaceInKB = (((MS_U64)fsInfo.u32ClusFree* (MS_U64)fsInfo.u32ClusSize) / 1024);
        mTBOX_TRACE((kTBOX_NIV_1, "*u64TotalSpaceInKB=%lld *u64FreeSpaceInKB=%lld\n",*u64TotalSpaceInKB,*u64FreeSpaceInKB));
        return TRUE;
    }
#endif

    return FALSE;
}

MS_BOOL  TDAL_PVRm_IsFileExist(const char *m_mount_path,char *filename)
{
    struct stat buf;
    char path[FILE_PATH_SIZE]={0};
    if (m_mount_path == NULL)
        sprintf(path,"%s", filename);
    else
        sprintf(path,"%s/%s",m_mount_path,filename);
    if( MsFS_Stat(path,&buf)<0)
    {
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "errno = %d\n",errno));
        return FALSE;
    }

    return TRUE;
}

MS_U32 TDAL_PVRm_IsH264(APIPVR_CODEC_TYPE enPvrVCodec)
{
    if((EN_APIPVR_CODEC_TYPE_H264_IFRAME==enPvrVCodec)||(EN_APIPVR_CODEC_TYPE_H264==enPvrVCodec))
    {
        return 1;
    }
    return 0;
}

MS_BOOL TDAL_PVRm_InitAudioInfo(MS_BOOL bReload,MS_BOOL bFileMode, MS_U32 u32ACodec,MS_BOOL bSPDIFMODE,MS_BOOL bHDMIMODE)
{
    if(bReload )
    {
        if(bFileMode)
        {
            if(eTDAL_PVR_PlayType== EN_APIPVR_PLAYBACK_TYPE_ESPLAYER)
            {
                MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);
                //MsOS_DelayTask(20);
                //MDrv_AUDIO_SetPlayFileFlag(DSP_DEC, 1);
                printf("[Audio] System Set\n");
            }
        }

        printf("[Audio] ACodec=%lX\n",u32ACodec);
        switch (u32ACodec)
        {
            case SiMPEG:
                MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_MPEG);
                break;
            case SiAC3:
                MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_AC3);
                break;
            case SiMPEG4:
            case SiAAC:
                MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_AAC);
                break;
            case SiAC3P:
                MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_AC3P);
                break;
            default:
                MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_MPEG);
                break;
        }

    }

    if(!bSPDIFMODE)
    {
        MApi_AUDIO_SPDIF_SetMode(SPDIF_OUT_NONPCM);
        printf("SPIDF mode is AUTO mode\n");
    }
    else
    {

        MApi_AUDIO_SPDIF_SetMode(SPDIF_OUT_PCM);
        printf("SPIDF mode is PCM mode\n");

    }

    if(!bHDMIMODE)
    {
        MApi_AUDIO_HDMI_TX_SetMode(HDMI_OUT_NONPCM);
        printf("HDMI mode is AUTO mode\n");
    }
    else
    {

        MApi_AUDIO_HDMI_TX_SetMode(HDMI_OUT_PCM);
        printf("HDMI mode is PCM mode\n");
    }


    MApi_HDMITx_SetAudioOnOff((MS_BOOL)TRUE);
    MApi_HDMITx_SetAudioFrequency(HDMITX_AUDIO_48K);
    MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);


    if(bFileMode)
    {
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILE);
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_FREE_RUN);
        printf("[Audio] Play File\n");
    }
    else
    {
        MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAY);
        printf("[Audio] Play TS File\n");
    }

    MApi_HDMITx_SetAudioOnOff((MS_BOOL)TRUE);
    MApi_HDMITx_SetAudioFrequency(HDMITX_AUDIO_48K);
    MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_48K, HDMITX_AUDIO_CH_2, HDMITX_AUDIO_PCM);


    return TRUE;
}

MS_BOOL TDAL_PVRm_FileModePlay(MS_U32 u32VCodec, MS_U32 u32ACodec)
{
#if ENABLE_MIU_1
    MS_U32 u32MiuInterval = MIU_INTERVAL;
#else
    MS_U32 u32MiuInterval = 0;
#endif
    VDEC_Status vdecStatus;
    VDEC_InitParam initParam;
    VDEC_CodecType codecType;
    VDEC_CodecType codecTypeCur;

    //Reset/Clear VDEC
    {
        MApi_VDEC_Rst();
        MApi_VDEC_Exit();
    }

    switch (u32VCodec)
    {
        default:
        case E_DDI_VDEC_CODEC_TYPE_MPEG2:
        case E_DDI_VDEC_CODEC_TYPE_MPEG2_IFRAME:
        {
            codecType = E_VDEC_CODEC_TYPE_MPEG2;
            break;
        }
        case E_DDI_VDEC_CODEC_TYPE_H264:
        case E_DDI_VDEC_CODEC_TYPE_H264_IFRAME:
        {
            codecType = E_VDEC_CODEC_TYPE_H264;
            break;
        }


    }

    memset(&vdecStatus, 0, sizeof(VDEC_Status));
    codecTypeCur = MApi_VDEC_GetActiveCodecType();
    MApi_VDEC_GetStatus(&vdecStatus);
    if ((TRUE == vdecStatus.bInit) && (codecTypeCur == codecType))
    {
        return TRUE;
    }

    memset(&initParam, 0, sizeof(VDEC_InitParam));
    initParam.bDisableDropErrFrame = TRUE;
    initParam.eCodecType = codecType;

    /* TODO START: H264.IFrame */
    //Temp Solution for I-Frame decoder on H264
    initParam.VideoInfo.eSrcMode = E_VDEC_SRC_MODE_FILE;
    initParam.SysConfig.u32CodeBufAddr = VDEC_AEON_ADR + u32MiuInterval;
    initParam.SysConfig.u32CodeBufSize = VDEC_AEON_LEN;
    initParam.SysConfig.u32FrameBufAddr = VDEC_FRAME_BUF_ADR + u32MiuInterval;
    initParam.SysConfig.u32FrameBufSize = VDEC_FRAME_BUF_LEN;
    initParam.SysConfig.u32BitstreamBufAddr = VDEC_BIT_STREAM_ADR + u32MiuInterval;
    initParam.SysConfig.u32BitstreamBufSize = VDEC_BIT_STREAM_LEN;
    initParam.SysConfig.u32DrvProcBufAddr = VDEC_BIT_STREAM_ADR + u32MiuInterval;
    initParam.VideoInfo.eTimeStampType = E_VDEC_EX_TIME_STAMP_PTS;
    initParam.SysConfig.eDbgMsgLevel = E_VDEC_EX_DBG_LEVEL_NONE;

    printf("FILE MODE\n");
    printf("VDEC FW add = 0x%lx\n", initParam.SysConfig.u32CodeBufAddr);
    printf("VDEC FB add = 0x%lx\n", initParam.SysConfig.u32FrameBufAddr);
    printf("VDEC BS add = 0x%lx\n", initParam.SysConfig.u32BitstreamBufAddr);

    //Init/Re-Init VDEC
    if (E_VDEC_OK != MApi_VDEC_Init(&initParam))
    {
        printf("*******************************************************\n");
        printf("MApi_VDEC_Init  return fail!!!\n");
        printf("*******************************************************\n");
        return FALSE;
    }

    //Move MVOP Setting to XC Demo code
    //MDrv_MVOP_Init();
    //MDrv_MVOP_Enable(TRUE);

    //change sync offset to 120ms, this value will influence lip-sync
    //If you want to change this value, please inform video and audio team at the same time
    VDEC_Result enRst = MApi_VDEC_AVSyncOn(TRUE, PVR_SYNC_DELAY, 66);
    if(enRst != E_VDEC_OK)
    {
        printf("[%s][%d] MApi_VDEC_AVSyncOn fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    //Set Field Delay count befor VDEC play
    MApi_VDEC_SetControl(VDEC_USER_CMD_FD_MASK_DELAY_COUNT, 16);
    MApi_VDEC_SetControl(VDEC_USER_CMD_FAST_DISPLAY, ENABLE);

    if(MApi_VDEC_SetAVSyncFreerunThreshold(450000)!= E_VDEC_OK)//VDES_AVSYNCTIME
    {
        printf("[%s][%d] MApi_VDEC_SetAVSyncFreerunThreshold fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    MApi_VDEC_EnableESBuffMalloc(TRUE);
    if (MApi_VDEC_Play() != E_VDEC_OK)
    {
        printf("[%s][%d] MApi_VDEC_Play fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }


    TDAL_PVRm_InitAudioInfo(TRUE,TRUE, u32ACodec,TRUE,TRUE);

    return TRUE;
}

void TDAL_PVRm_PhyClose(void)
{
    MApi_VDEC_Rst();
    MApi_AUDIO_FileEndNotification();
    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);
    MsOS_DelayTask(30);
}

APIPVR_ENCRYPTION_TYPE TDAL_PVRm_ConvEncrType(uint8_t eEncrType)
{
    tTDAL_PVR_Encr encrType = (tTDAL_PVR_Encr)eEncrType;
    APIPVR_ENCRYPTION_TYPE mstar_EncrType = EN_APIPVR_ENCRYPTION_NONE;
    switch (encrType)
    {
    case eTDAL_PVR_ENCR_NONE:
        mstar_EncrType = EN_APIPVR_ENCRYPTION_NONE;
        break;
    case eTDAL_PVR_ENCR_DEFAULT:
        mstar_EncrType = EN_APIPVR_ENCRYPTION_DEFAULT;
        break;        
    case eTDAL_PVR_ENCR_USER:
        mstar_EncrType = EN_APIPVR_ENCRYPTION_USER;
        break;
    case eTDAL_PVR_ENCR_CIPLUS:
        mstar_EncrType = EN_APIPVR_ENCRYPTION_CIPLUS;
        break;
    case eTDAL_PVR_ENCR_SMARTCARD:
        mstar_EncrType = EN_APIPVR_ENCRYPTION_SMARTCARD;
        break;         
    }
    return mstar_EncrType; 
}

void    TDAL_PVRm_SetCryptKey(uint8_t *u8Crypt)
{
    if (u8Crypt)
        memcpy(u8CryptKey, u8Crypt, sizeof(u8CryptKey));
    else
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "Key for crypting PVR recording/playback is corrupted"));
}

uint8_t* TDAL_PVRm_GetCryptKey()
{
    return u8CryptKey;
}


tTDAL_PVR_Error TDAL_PVR_RemoveFile(char *fileName, int filesCount)
{
    tTDAL_PVR_Error error = eTDAL_PVR_NO_ERROR;
    mTBOX_FCT_ENTER("TDAL_PVR_RemoveFile");
    uint8_t fullFileName[128] = {0};
    uint32_t i = 0;

    if (fileName == NULL)
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_RETURN(error);
    }

    //remove multiple backaslashes from start of path
    while(fileName[i+1] == '/') i++;
    strcpy(fullFileName, fileName+i);

/*    if (!TDAL_PVRm_IsFileExist(MApi_PVR_GetMouthPath(), &fullFileName))
    {
        error = eTDAL_PVR_ERROR_BAD_ARG;
        mTBOX_TRACE((kTBOX_NIV_CRITICAL, "File does not exist %s!\n", fullFileName));
        mTBOX_RETURN(error);
    }*/

#if !defined(TDAL_PVR_USE_TDAL_FS)
    for(i=0; i<filesCount; i++)
    {
        sprintf(strchr(fullFileName, '.') - 3, "%03d.trp", i);
        MApi_PVR_RemoveFile(&fullFileName);
    }

    sprintf(strchr(fullFileName, '.') - 3, "000.trp.meta");
    MApi_PVR_RemoveFile(&fullFileName);
#else
    sprintf(strchr(fullFileName, '.') - 3, "%03d.trp", i);

    while((i<filesCount) ||
    	  (filesCount == 0 && TDAL_FS_FileExist(fullFileName) == eTDAL_FS_NO_ERROR))
    {
        TDAL_FS_Remove(&fullFileName);
		strcat(fullFileName, ".meta");
		TDAL_FS_Remove(&fullFileName);

		fullFileName[strlen(fullFileName) - strlen(".meta")] = '\0';
        strcat(fullFileName, ".idx");
        TDAL_FS_Remove(&fullFileName);
        fullFileName[strlen(fullFileName) - strlen(".meta")] = '\0';

		i++;

		sprintf(strchr(fullFileName, '.') - 3, "%03d.trp", i);
    }
#endif

    mTBOX_RETURN(error);
}



pTDAL_PVR_NotifyRecordedContent(uint8_t *pMountPoint, uint32_t sizeKB)
{
#if defined(TDAL_PVR_USE_TDAL_FS)
    mTDAL_FS_PVR_NotifyRecordedContent(pMountPoint, sizeKB);
#endif

}
