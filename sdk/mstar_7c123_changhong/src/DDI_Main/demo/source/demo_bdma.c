//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
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
#if (DEMO_BDMA_TEST == 1)
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsMemory.h"
#include "string.h"
#include "drvBDMA.h"
#include "demo_utility.h"
#include "demo_bdma.h"
#include "drvDTC.h"

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Local Definition
//-------------------------------------------------------------------------------------------------
#define TEST_BUF_SIZE   128

static void *TestSrcBuf = 0;
static void *TestDstBuf = 0;

//-------------------------------------------------------------------------------------------------
// Local Function
//-------------------------------------------------------------------------------------------------
static void _AllocTestBuf(void)
{
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&s32MstarCachedPoolID);


    if (!TestSrcBuf)
    {
        TestSrcBuf = MsOS_AllocateMemory(TEST_BUF_SIZE, s32MstarCachedPoolID);
        memset(TestSrcBuf, 0x00, TEST_BUF_SIZE);
    }
    if (!TestDstBuf)
    {
        TestDstBuf = MsOS_AllocateMemory(TEST_BUF_SIZE, s32MstarCachedPoolID);
        memset(TestDstBuf, 0x00, TEST_BUF_SIZE);
    }
}
static void _FreeTestBuf(void)
{
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE,&s32MstarCachedPoolID);

    if (!TestSrcBuf)
    {
        MsOS_FreeMemory(TestSrcBuf, s32MstarCachedPoolID);
    }
    if (!TestDstBuf)
    {
        MsOS_FreeMemory(TestDstBuf, s32MstarCachedPoolID);
    }
}
static void _WriteTestBuf(void *Buf, MS_U8 pattern, MS_U8 inc, MS_U32 len)
{
    MS_U32 i = 0, tmp = pattern;
    for (i = 0; i < len; i ++)
    {
        *((MS_U8 *)Buf + i) = tmp;
        tmp += inc;
    }
}
static void _DumpTestBuf(void *Buf, MS_U32 len)
{
    MS_U32 i = 0;
    for (i = 0; i < len; i++)
    {
        printf("0x%02x%c", *((MS_U8 *)Buf + i), ((i % 16) == 15)? '\n': ' ');
    }
    printf("\n");
}
static MS_BOOL _CompareTestBuf(void *SrcBuf, void *DstBuf, MS_U32 len)
{
    MS_U8 *CmpSrcBuf = (MS_U8 *)SrcBuf;
    MS_U8 *DstSrcBuf = (MS_U8 *)DstBuf;
    MS_U32 i = 0;

    printf("Compare Source Data and Destination Data\n");

    for (i = 0; i < len; i++)
    {
        if (CmpSrcBuf[i] != DstSrcBuf[i])
        {
            printf("Compare Error - Src[%"DTC_MS_U32_d"]=[0x%02x] ; Dst[%"DTC_MS_U32_d"]=[0x%02x]\n", i, CmpSrcBuf[i], i, DstSrcBuf[i]);
            return FALSE;
        }
    }

    printf("Compare Pass\n");

    return TRUE;
}


//-------------------------------------------------------------------------------------------------
// Demo Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief BDMA Initialize
/// @param[in] None
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b BDMA_Init \n
///
/// Sample Command: \b BDMA_Init \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_BDMA_Init(void)
{
     // init BDMA
     // _NOTE_
     // The argument for MDrv_BDMA_Init is not used.
     if (E_BDMA_OK != MDrv_BDMA_Init(0x0))
     {
         printf("Error: BDMA init failed\n");
         return FALSE;
     }

     return TRUE;
}

//------------------------------------------------------------------------------
/// @brief BDMA Usage Demo
/// @param[in] None
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b BDMA_Demo \n
///
/// Sample Command: \b BDMA_Demo \n
///
//------------------------------------------------------------------------------
MS_BOOL Demo_BDMA_Demo(void)
{
    MS_BOOL bResult = 0;

    // alloc test buffer for BDMA demo code
    _AllocTestBuf();
    // write test data into Src buffer
    _WriteTestBuf(TestSrcBuf, 0x00, 0x01, TEST_BUF_SIZE);
    // print Dst buffer content to user
    printf("===== source buffer (before) =====\n");
    _DumpTestBuf(TestSrcBuf, TEST_BUF_SIZE);
    printf("===== destination buffer (before) =====\n");
    _DumpTestBuf(TestDstBuf, TEST_BUF_SIZE);
    // copy content from test buffer by BDMA
    // _NOTE_
    // MDrv_BDMA_MemCopy takes physical address only.
    MsOS_Dcache_Flush((MS_U32)TestSrcBuf, TEST_BUF_SIZE);
    MsOS_Dcache_Flush((MS_U32)TestDstBuf, TEST_BUF_SIZE);
    if (E_BDMA_OK != MDrv_BDMA_MemCopy(MsOS_VA2PA((MS_U32)TestSrcBuf),
                                       MsOS_VA2PA((MS_U32)TestDstBuf),
                                       TEST_BUF_SIZE))
    {
        printf("Error: BDMA copy failed\n");
    }
    // print Dst buffer content to user
    printf("===== destination buffer (after) =====\n");
    _DumpTestBuf(TestDstBuf, TEST_BUF_SIZE);

    bResult = _CompareTestBuf(TestSrcBuf, TestDstBuf, TEST_BUF_SIZE);
    // free test buffer
    _FreeTestBuf();
    return bResult;

}
//------------------------------------------------------------------------------
/// @brief Demo_BDMA_HELP Usage Demo
/// @param[in] None
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b BDMA_Help \n
///
/// Sample Command: \b BDMA_Help \n
///
//------------------------------------------------------------------------------

MS_BOOL Demo_BDMA_Help(void)
{
    printf("BDMA Command Usage Guide: \n");
    printf("------------------------------------------------------------------------------\n");
    printf("BDMA_Init\n");
    printf("Description:Init BDMA without parameters \n");
    printf("------------------------------------------------------------------------------\n");
    printf("BDMA_Allocate \n");
    printf("                          <MIU Selection> <Size>\n");
    printf("Description:Allocate Buffer from Selected MIU Non-Cache Pool \n");
    printf("------------------------------------------------------------------------------\n");
    printf("BDMA_Free \n");
    printf("                          <MIU POOL ID >\n");
    printf("                          <Allocated POOL Virtual Address> \n");
    printf("Description:Free Buffer from Selected MIU Non-Cache Pool \n");
    printf("------------------------------------------------------------------------------\n");
    printf("BDMA_FillMem \n");
    printf("                          <Allocated Phyical Address> \n");
    printf("                          <Size> \n");
    printf("                          <Pattern 32bits, ex: 0x12345678> \n");
    printf("                          <Value of BDMA_DstDev of drvBDMA.h ex: 0x0 for E_BDMA_DSTDEV_MIU0> \n");
    printf("------------------------------------------------------------------------------\n");
    printf("BDMA_HKFillMem \n");
    printf("                          <Allocated POOL Virtual Address> \n");
    printf("                          <Size> \n");
    printf("                          <Pattern 8bits, ex: 0xFF> \n");
    printf("                          <Increment value, ex: 0x1> \n");
    printf("Description:Fill buffer with HK CPU \n");
    printf("------------------------------------------------------------------------------\n");
    printf("BDMA_DumpMem \n");
    printf("                          <Allocated POOL Virtual Address> \n");
    printf("                          <Size> \n");
    printf("Description:Dump buffer with HK CPU \n");
    printf("------------------------------------------------------------------------------\n");
    printf("BDMA_Copy        \n");
    printf("                          <Allocated POOL Phyical Address Source> \n");
    printf("                          <Allocated POOL Phyical Address Destination> \n");
    printf("                          <Size> \n");
    printf("                          <Copy Type:reference enum of drvBDMA.h BDMA_CpyType > \n");
    printf("                          <OPT Type:reference definition of drvBDMA.h /// Operation cfg> \n");
    printf("Description:Trigger BDMA HW to Copy Data from Src to Dst \n");
    printf("------------------------------------------------------------------------------\n");
    printf("BDMA_Cmp         \n");
    printf("                          <Allocated POOL Virtual Address A> \n");
    printf("                          <Allocated POOL Virtual Address B> \n");
    printf("                          <Size> \n");
    printf("Description:Compare buffers with HK CPU \n");
    printf("------------------------------------------------------------------------------\n");
    printf("BDMA_Demo:\n");
    printf("Description:Process Demo Test Buffers Copy with BDMA Directly\n");
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Demo_BDMA_CmpMem Usage Demo
/// @param[in] compared source address A (Virtual Address)
/// @param[in] compared source address B (Virtual Address)
/// @param[in] Compare Length (Byte)
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b BDMA_Cmp \n
///
/// Sample Command: \b BDMA_Cmp \n
///
//------------------------------------------------------------------------------

MS_BOOL Demo_BDMA_CmpMem(MS_U32 *srcPoolAddressA,MS_U32 *srcPoolAddressB,MS_U32 *plength)
{
    void* _pcmpaddressA =NULL;
    void* _pcmpaddressB =NULL;
    MS_U32  _length = 0;

    if ((srcPoolAddressA == NULL)||(srcPoolAddressB == NULL)||(plength == NULL))
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }


    _pcmpaddressA = (void*)*srcPoolAddressA;
    _pcmpaddressB = (void*)*srcPoolAddressB;
    _length = *plength;

    if ((_pcmpaddressA == NULL) ||(_pcmpaddressB == NULL) || (_length == 0))
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    printf("[%s][%d] %p %p %"DTC_MS_U32_x" \n",__FUNCTION__,__LINE__,_pcmpaddressA,_pcmpaddressB,_length);


    _CompareTestBuf(_pcmpaddressA,_pcmpaddressB,_length);


    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Demo_BDMA_DumpMem Usage Demo
/// @param[in] Dump source address (Virtual Address)
/// @param[in] Dump Length (Byte)
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b BDMA_Dump \n
///
/// Sample Command: \b BDMA_Dump \n
///
//------------------------------------------------------------------------------

MS_BOOL Demo_BDMA_DumpMem(MS_U32 *MIU_PoolAddress,MS_U32 *plength)
{
    void* _pdumpaddress =NULL;
    MS_U32  _length = 0;

    if ((MIU_PoolAddress == NULL)||(plength == NULL))
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }


    _pdumpaddress = (void*)*MIU_PoolAddress;
    _length = *plength;

    if ((_pdumpaddress == NULL) || (_length == 0))
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    printf("[%s][%d] %p %"DTC_MS_U32_x" \n",__FUNCTION__,__LINE__,_pdumpaddress,_length);


    _DumpTestBuf(_pdumpaddress,_length);


    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Demo_BDMA_FillMem Usage Demo
/// @param[in] Target address (Phycial Address)
/// @param[in] Fill Length (Byte)
/// @param[in] Pattern (32bits hex)
/// @param[in] Dstdevice (MIU0 or MIU1 or some SRAM depend on drvBDMA.h
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b BDMA_Fill \n
///
/// Sample Command: \b BDMA_Fill \n
///
//------------------------------------------------------------------------------

MS_BOOL Demo_BDMA_FillMem(MS_U32 *PHYAddress,MS_U32 *plength,MS_U32 *ppattern,MS_U32 *pDstDev)
{
    MS_PHYADDR _PHYAddress = 0;
    MS_U32  _pattern = 0;
    MS_U32  _length = 0;
    BDMA_DstDev _eDev  = 0;
    BDMA_Result eRet =E_BDMA_FAIL;

    if ((PHYAddress == NULL)||(plength == NULL)||(ppattern == NULL)|| (pDstDev == NULL))
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    _pattern = *ppattern;
    _PHYAddress = (MS_PHYADDR)*PHYAddress;
    _length = *plength;
    _eDev = *pDstDev;

    if ((_length == 0) || (_eDev> E_BDMA_DSTDEV_NOT_SUPPORT))
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    eRet = MDrv_BDMA_PatternFill(_PHYAddress,_length,_pattern,_eDev);

    if (eRet != E_BDMA_OK)
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Demo_BDMA_HKFillMem Usage Demo
/// @param[in] Target address (Virtual Address)
/// @param[in] Fill Length (Byte)
/// @param[in] Pattern (8bits hex)
/// @param[in] Increment gap
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b BDMA_HKFill \n
///
/// Sample Command: \b BDMA_HKFill \n
///
//------------------------------------------------------------------------------

MS_BOOL Demo_BDMA_HKFillMem(MS_U32 *MIU_PoolAddress,MS_U32 *plength,MS_U32 *ppattern,MS_U32 *pincrement)
{
    void* _pfilladdress =NULL;
    MS_U8  _pattern = 0, _increment= 0;
    MS_U32  _length = 0;

    if ((MIU_PoolAddress == NULL)||(plength == NULL)||(ppattern == NULL)||(pincrement == NULL))
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }


    _pattern = *ppattern & 0xFF;
    _increment = *pincrement & 0xFF;
    _pfilladdress = (void*)*MIU_PoolAddress;
    _length = *plength;

    if ((_pfilladdress == NULL) || (_length == 0))
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    printf("[%s][%d] %p %"DTC_MS_U32_x" %x %x \n",__FUNCTION__,__LINE__,_pfilladdress,_length,_pattern,_increment);


    _WriteTestBuf(_pfilladdress,_pattern,_increment,_length);


    return TRUE;
}

//------------------------------------------------------------------------------
/// @brief Demo_BDMA_FreeMemTest Usage Demo
/// @param[in]  POOL Index
/// @param[in]  Target free address (Virtual Address)
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b BDMA_Free \n
///
/// Sample Command: \b BDMA_Free \n
///
//------------------------------------------------------------------------------

MS_BOOL Demo_BDMA_FreeMemTest(MS_U32 *MIU_PoolID, MS_U32 *MIU_PoolAddress)
{
    MS_S32 s32MstarPoolID = INVALID_POOL_ID  ;
    MS_BOOL bRet= FALSE;
    void*  freeaddress = NULL;
    if ((MIU_PoolID == NULL)||(MIU_PoolAddress == NULL))
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }


    printf("[%s][%d] %"DTC_MS_U32_x" %"DTC_MS_U32_x"  \n",__FUNCTION__,__LINE__,*MIU_PoolID,*MIU_PoolAddress);
    s32MstarPoolID = *MIU_PoolID;
    freeaddress = (void*)*MIU_PoolAddress;

    if (freeaddress != NULL)
    {
         bRet = MsOS_FreeMemory(freeaddress, s32MstarPoolID);
    }

     printf("[%s][%d] %d\n",__FUNCTION__,__LINE__,bRet);
     return TRUE;
}
//------------------------------------------------------------------------------
/// @brief Demo_BDMA_AllocateMemTest Usage Demo
/// @param[in]  MIU Index
/// @param[in]  Allocated Size
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b BDMA_Allocate\n
///
/// Sample Command: \b BDMA_Allocate \n
///
//------------------------------------------------------------------------------

MS_BOOL Demo_BDMA_AllocateMemTest(MS_U32 *MIU_Select,MS_U32 *MIU_SIZE)
{
    MS_U8 u8MIU = 0;
    MS_U32 u32Size = 0;
    MS_BOOL bRet = FALSE;
    MS_S32 s32MstarPoolID = INVALID_POOL_ID;
    MS_PHYADDR _PHYAddress = 0;
    void* u32AllocateAddress = NULL;
    if ((MIU_Select == NULL)||(MIU_SIZE == NULL))
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if ((*MIU_SIZE == 0))
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    printf("[%s][%d] %"DTC_MS_U32_x" %"DTC_MS_U32_x"  \n",__FUNCTION__,__LINE__,*MIU_Select,*MIU_SIZE);
    u32Size = *MIU_SIZE;
    u8MIU =  *MIU_Select & 0xFF;

#ifdef MIU1_NON_CACHED_POOL_BUF_ADR
    /*Support MIU1*/
#else
     if (u8MIU >= 1)
     {
         printf("[%s][%d] \n",__FUNCTION__,__LINE__);
         return FALSE;
     }
#endif

    printf("[%s][%d] %d \n",__FUNCTION__,__LINE__,u8MIU );

    if(u8MIU == 0)
    {
        bRet=Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_NONCACHE,&s32MstarPoolID);
        u32AllocateAddress = MsOS_AllocateMemory(u32Size, s32MstarPoolID);
    }

#ifdef MIU1_NON_CACHED_POOL_BUF_ADR
    if(u8MIU == 1)
    {
        bRet=Demo_Util_GetSystemPoolID(E_DDI_POOL_MIU1_SYS_NONCACHE,&s32MstarPoolID);
        u32AllocateAddress = MsOS_AllocateMemory(u32Size, s32MstarPoolID);
    }
#endif

    printf("[%s][%d] %d %"DTC_MS_S32_d" \n",__FUNCTION__,__LINE__,bRet,s32MstarPoolID );

    if (u32AllocateAddress == NULL)
    {
        printf("[%s][%d] \n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    _PHYAddress = (MS_PHYADDR)MsOS_MPool_VA2PA((MS_U32)u32AllocateAddress);

    printf("[%s][%d] s32MstarPoolID %"DTC_MS_S32_x" Allocate Address %p PA %"DTC_MS_U32_x"\n",__FUNCTION__,__LINE__,s32MstarPoolID,u32AllocateAddress,_PHYAddress);

    return TRUE;
}
//------------------------------------------------------------------------------
/// @brief BDMA_Copy Usage Demo
/// @param[in] Source Address (Phycial Address)
/// @param[in] Destination Address (Phycial Address)
/// @param[in] Copy Size
/// @param[in] Copy Type (MIU0->MIU1 or SerialNor to MIU0... from drvBDMA.h Enum)
/// @param[in] Copy Option of drvBDMA.h
/// @return TRUE - Process success
/// @return FALSE - Process fail
/// @sa
/// @note
/// Command: \b BDMA_Copy \n
///
/// Sample Command: \b BDMA_Copy \n
///
//------------------------------------------------------------------------------

MS_BOOL Demo_BDMA_Copy(MS_U32 *u32SrcAddr,MS_U32 *u32DstAddr,MS_U32 *u32Size, MS_U32 *CopyType,MS_U32 *CopyOPT)
{
    BDMA_CpyType _eCpyType = E_BDMA_CPYTYPE_MAX;
    MS_U8 _u8OPT = 0;
    BDMA_Result _eBDMARet = E_BDMA_FAIL;
    if ((u32SrcAddr == NULL)||(u32DstAddr == NULL)||(u32Size == NULL)||(CopyType == NULL)||(CopyOPT == NULL))
    {
        printf("[%s][%d] %d\n",__FUNCTION__,__LINE__,_eBDMARet);
        return FALSE;
    }

    _eCpyType = *CopyType;
    _u8OPT = (*CopyOPT) & 0xFF;

    _eBDMARet = MDrv_BDMA_CopyHnd(*u32SrcAddr, *u32DstAddr ,*u32Size, _eCpyType ,  _u8OPT);

    if (_eBDMARet != E_BDMA_OK)
    {
        printf("[%s][%d] %d\n",__FUNCTION__,__LINE__,_eBDMARet);
        return FALSE;
    }



    return TRUE;
}

#endif
