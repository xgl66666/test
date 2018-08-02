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


//#include "eMMC.h"


#include "loader_emmc.h"



U32 eMMC_Init_Device(void)
{
	U32 u32_err;


    EMMC_PUTS("\neMMC_Init_Device start \n");
    //EMMC_PUTS("eMMC_Init_Device \n");

	#if defined(eMMC_RSP_FROM_RAM) && eMMC_RSP_FROM_RAM
	EMMC_PUTS("eMMC Info: Rsp from RAM \n");
	g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_RSPFROMRAM_SAVE;
	#endif

	g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_INIT_DONE;

    EMMC_PUTS("do eMMC_Identify \n");
	// init eMMC device
	u32_err = eMMC_Identify();
	if(u32_err)
		goto LABEL_INIT_END;

    EMMC_PUTS("do eMMC_clock_setting \n");
	eMMC_clock_setting(FCIE_SLOW_CLK);

	// determine device parameters, from CSD
    EMMC_PUTS("do eMMC_CSD_Config \n");
	u32_err = eMMC_CSD_Config();
	if(eMMC_ST_SUCCESS != u32_err)
		goto  LABEL_INIT_END;

	// setup eMMC device
	// CMD7
    EMMC_PUTS("do eMMC_CMD3_CMD7 \n");
	u32_err = eMMC_CMD3_CMD7(g_eMMCDrv.u16_RCA, 7);
	if(eMMC_ST_SUCCESS != u32_err)
		goto  LABEL_INIT_END;   

	// determine device parameters, from Ext_CSD
    EMMC_PUTS("do eMMC_ExtCSD_Config \n");
	u32_err = eMMC_ExtCSD_Config();
	if(eMMC_ST_SUCCESS != u32_err)
		goto  LABEL_INIT_END;
   
	//printf("set bus witdth 8 bits\n");
    EMMC_PUTS("do eMMC_SetBusWidth \n");
	u32_err = eMMC_SetBusWidth(8, 0);
	if(eMMC_ST_SUCCESS != u32_err)
        goto  LABEL_INIT_END;
  

	//printf("set to high speed\n");
    EMMC_PUTS("do eMMC_SetBusSpeed \n");
	u32_err = eMMC_SetBusSpeed(eMMC_SPEED_HIGH);
	if(eMMC_ST_SUCCESS != u32_err)
		goto  LABEL_INIT_END;

	eMMC_clock_setting(FCIE_DEFAULT_CLK);

	g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_INIT_DONE;
   

    /*
	//eMMC_dump_mem(g_eMMCDrv.au8_AllRsp, 0x100);
      */
LABEL_INIT_END:

	return u32_err;

}



// =======================================================
// u32_DataByteCnt: has to be 512B-boundary !
// =======================================================
U32 eMMC_ReadData(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr)
{
	U32 u32_err;
	volatile U16 u16_BlkCnt;
	// read data
	// first 512 bytes, special handle if not cache line aligned
	while(u32_DataByteCnt)
	{
		if(u32_DataByteCnt > eMMC_SECTOR_512BYTE)
		{
			if((u32_DataByteCnt>>eMMC_SECTOR_512BYTE_BITS) < BIT_SD_JOB_BLK_CNT_MASK)
				u16_BlkCnt = (u32_DataByteCnt>>eMMC_SECTOR_512BYTE_BITS);
			else
				u16_BlkCnt = BIT_SD_JOB_BLK_CNT_MASK;

			u32_err = eMMC_CMD18(u32_BlkAddr, pu8_DataBuf, u16_BlkCnt);
		}
		else
		{
			u16_BlkCnt = 1;
			u32_err = eMMC_CMD17(u32_BlkAddr, pu8_DataBuf);
		}

		if(eMMC_ST_SUCCESS != u32_err)
		{
		    EMMC_PUTS("do eMMC Read Data Fail \n");
			break;
		}
		u32_BlkAddr += u16_BlkCnt;
		pu8_DataBuf += u16_BlkCnt << eMMC_SECTOR_512BYTE_BITS;
		u32_DataByteCnt -= u16_BlkCnt << eMMC_SECTOR_512BYTE_BITS;
	}

	return u32_err;
}

void * memset(void * s,char c,int count)
{
	char *xs = (char *) s;

	while (count--)
		*xs++ = c;

	return s;
}

static eMMC_ALIGN0 eMMC_TEST_ALIGN_PACK_t g_eMMCTestAlignPack_t eMMC_ALIGN1;
U32 eMMC_CheckAlignPack(U8 u8_AlignByteCnt)
{
	// check alignment
	if((U32)&(g_eMMCTestAlignPack_t.u8_0) & (u8_AlignByteCnt-1))
	{
		eMMC_die("\n");
		return eMMC_ST_ERR_NOT_ALIGN;
	}
	// check packed - 0
	if((U32)&(g_eMMCTestAlignPack_t.u16_0)-(U32)&(g_eMMCTestAlignPack_t.u8_0) != 1 ||
	   (U32)&(g_eMMCTestAlignPack_t.u32_0)-(U32)&(g_eMMCTestAlignPack_t.u8_0) != 3 ||
	   (U32)&(g_eMMCTestAlignPack_t.u32_1)-(U32)&(g_eMMCTestAlignPack_t.u8_0) != 7)
	{
		eMMC_die("\n");
		return eMMC_ST_ERR_NOT_PACKED;
	}
	// check packed - 1
	if((U32)&(g_eMMCTestAlignPack_t.u16_0)-(U32)&(g_eMMCTestAlignPack_t.u8_0) != 1 ||
	   (U32)&(g_eMMCTestAlignPack_t.u32_0)-(U32)&(g_eMMCTestAlignPack_t.u16_0)!= 2 ||
	   (U32)&(g_eMMCTestAlignPack_t.u32_1)-(U32)&(g_eMMCTestAlignPack_t.u32_0)!= 4)
	{
		eMMC_die("\n");
		return eMMC_ST_ERR_NOT_PACKED;
	}

	return eMMC_ST_SUCCESS;
}


U32 eMMC_Init_Ex(void)
{
    U32 u32_err;

	// ---------------------------------
	u32_err = eMMC_CheckAlignPack(eMMC_CACHE_LINE);
	if(u32_err)
        goto  LABEL_INIT_END;

	memset((void*)&g_eMMCDrv, '\0', sizeof(eMMC_DRIVER));

	// ---------------------------------
	// init platform & FCIE
	eMMC_PlatformInit();
	g_eMMCDrv.u8_BUS_WIDTH = BIT_SD_DATA_WIDTH_1;
	g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;
	u32_err = eMMC_FCIE_Init();
	if(u32_err)
        goto  LABEL_INIT_END;

	// ---------------------------------
	u32_err = eMMC_Init_Device();

	if(u32_err)
		goto LABEL_INIT_END;

	LABEL_INIT_END:
	// ---------------------------------
	// setup ID
	// use first 10 bytes of CID
/*
	memcpy(g_eMMCDrv.au8_ID, &g_eMMCDrv.au8_CID[1], eMMC_ID_FROM_CID_BYTE_CNT);
	g_eMMCDrv.u8_IDByteCnt = eMMC_ID_DEFAULT_BYTE_CNT;
	// add a 11-th byte for number of GB
	g_eMMCDrv.au8_ID[eMMC_ID_FROM_CID_BYTE_CNT] =
		(g_eMMCDrv.u32_SEC_COUNT >> (1+10+10)) + 1;
	eMMC_debug(eMMC_DEBUG_LEVEL, 0, "%uMB [%Xh]\n", g_eMMCDrv.u32_SEC_COUNT>>11, g_eMMCDrv.u32_SEC_COUNT);
	//eMMC_dump_mem(g_eMMCDrv.au8_ID, 0x10);
    #if 0
    u32_err = eMMC_ReadPartitionInfo_Ex();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Err: eMMC Init, no pni, :%Xh\n", u32_err);
		return eMMC_ST_ERR_NO_CIS;
	}
	//eMMC_DumpDriverStatus();
    #endif
*/
	return u32_err;
}


