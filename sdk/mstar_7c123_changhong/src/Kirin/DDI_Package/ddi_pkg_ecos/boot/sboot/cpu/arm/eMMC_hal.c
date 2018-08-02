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
#include "loader_emmc.h"
#include "chip.h"

#define RREG16(addr) (*(volatile U16 *)(addr))
#define WREG16(addr, value) (*(volatile U16 *)(addr) = (value))


/*
void LDR_PUTC(int c)
{
    while((RREG16(REG_UART_LSR) & UART_LSR_THRE) != UART_LSR_THRE)
        ;
    WREG16(REG_UART_TX, c);
    while((RREG16(REG_UART_LSR) & (UART_LSR_TEMT | UART_LSR_THRE)) != (UART_LSR_TEMT | UART_LSR_THRE))
        ;
}

void PUTS(const char *s)
{
    while(*s) {
        if(*s == '\n')
            LDR_PUTC('\r');
        LDR_PUTC(*s++);
    }
}

void PUTX(char val)
{
    if (val > 0x9)
        LDR_PUTC('A' - 10 + val);
    else
        LDR_PUTC('0' + val);
}

void PUTW(U16 val)
{
    PUTX((val >> 12) & 0xF);
    PUTX((val >> 8) & 0xF);
    PUTX((val >> 4) & 0xF);
    PUTX(val & 0xF);
}

void PUTDW(U32 val)
{
    PUTW((val >> 16) & 0xFFFF);
    PUTW(val & 0xFFFF);
}
*/

//========================================================
// Send CMD HAL APIs
//========================================================

U32 eMMC_FCIE_WaitD0High_Ex(U32 u32_us)
{
   volatile U32 u32_cnt;
   volatile U16 u16_read0=0, u16_read1=0;

   for(u32_cnt=0; u32_cnt < u32_us; u32_cnt++)
   {
   	   REG_FCIE_R(FCIE_SD_STATUS, u16_read0);
	   eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
	   REG_FCIE_R(FCIE_SD_STATUS, u16_read1);

	   if((u16_read0&BIT_SD_CARD_D0_ST) && (u16_read1&BIT_SD_CARD_D0_ST))
	       break;

	   if(u32_cnt > 500 && u32_us-u32_cnt > 1000)
	   {
	   	   eMMC_hw_timer_sleep(1);
		   u32_cnt += 1000-2;
	   }
   }

   return u32_cnt;
}


U32 eMMC_FCIE_WaitD0High(U32 u32_us)
{
    volatile U32 u32_cnt;

	REG_FCIE_SETBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
	u32_cnt = eMMC_FCIE_WaitD0High_Ex(u32_us);

	if(u32_us == u32_cnt)
	{
        EMMC_PUTS("eMMC Err: wait D0 H timeout\n");
		return eMMC_ST_ERR_TIMEOUT_WAITD0HIGH;
	}

    return eMMC_ST_SUCCESS;
}

void eMMC_FCIE_ErrHandler_Stop(void)
{

    EMMC_PUTS("ErrHandler_Stop emmc die should do dump\n\t");
	//eMMC_DumpDriverStatus();
	//eMMC_DumpPadClk();
	//eMMC_FCIE_DumpRegisters();
	//eMMC_FCIE_DumpDebugBus();
	eMMC_die("\n");


}

#define eMMC_FCIE_CLK_DIS()    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN)
#define FCIE_WAIT_RST_DONE_D1  0x5555
#define FCIE_WAIT_RST_DONE_D2  0xAAAA
#define FCIE_WAIT_RST_DONE_CNT 3

void eMMC_FCIE_CheckResetDone(void)
{
	volatile U16 au16_tmp[FCIE_WAIT_RST_DONE_CNT];
	volatile U32 u32_i, u32_j, u32_err;

	for(u32_i=0; u32_i<TIME_WAIT_FCIE_RESET; u32_i++)
	{
	    for(u32_j=0; u32_j<FCIE_WAIT_RST_DONE_CNT; u32_j++)
		    REG_FCIE_W(FCIE_CIFC_ADDR(u32_j), FCIE_WAIT_RST_DONE_D1+u32_j);

		for(u32_j=0; u32_j<FCIE_WAIT_RST_DONE_CNT; u32_j++)
		    REG_FCIE_R(FCIE_CIFC_ADDR(u32_j), au16_tmp[u32_j]);

		for(u32_j=0; u32_j<FCIE_WAIT_RST_DONE_CNT; u32_j++)
			if(FCIE_WAIT_RST_DONE_D1+u32_j != au16_tmp[u32_j])
		        break;

		if(FCIE_WAIT_RST_DONE_CNT == u32_j)
			break;
		eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
	}

	if(TIME_WAIT_FCIE_RESET == u32_i)
	{
		u32_err = eMMC_ST_ERR_FCIE_NO_RIU;
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: check CIFC fail: %Xh \n", u32_err);
        
		eMMC_FCIE_ErrHandler_Stop();
	}
}


U32 eMMC_FCIE_Reset(void)
{
	U16 au16_tmp[FCIE_WAIT_RST_DONE_CNT];
	U32 u32_i, u32_j, u32_err = eMMC_ST_SUCCESS;
	U16 u16_clk = g_eMMCDrv.u16_ClkRegVal;

	//eMMC_clock_setting(gau8_FCIEClkSel[0]); // speed up FCIE reset done
    eMMC_clock_setting(BIT_FCIE_CLK_48M);
	REG_FCIE_W(FCIE_PATH_CTRL, BIT_SD_EN);
	eMMC_FCIE_CLK_DIS(); // do not output clock

	// miu request reset - set
	REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU_REQUEST_RST);

	// FCIE reset - prepare
	for(u32_i=0; u32_i<TIME_WAIT_FCIE_RESET; u32_i++)
	{
	    for(u32_j=0; u32_j<FCIE_WAIT_RST_DONE_CNT; u32_j++)
		    REG_FCIE_W(FCIE_CIFC_ADDR(u32_j), FCIE_WAIT_RST_DONE_D1+u32_j);

		for(u32_j=0; u32_j<FCIE_WAIT_RST_DONE_CNT; u32_j++)
		    REG_FCIE_R(FCIE_CIFC_ADDR(u32_j), au16_tmp[u32_j]);

		for(u32_j=0; u32_j<FCIE_WAIT_RST_DONE_CNT; u32_j++)
			if(FCIE_WAIT_RST_DONE_D1+u32_j != au16_tmp[u32_j])
		        break;

		if(FCIE_WAIT_RST_DONE_CNT == u32_j)
			break;
		eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
	}
	if(TIME_WAIT_FCIE_RESET == u32_i)
	{
		u32_err = eMMC_ST_ERR_FCIE_NO_RIU;
        EMMC_PUTS("preset CIFC fail \n");
	    eMMC_FCIE_ErrHandler_Stop();
	}

	// FCIE reset - set
	REG_FCIE_CLRBIT(FCIE_TEST_MODE, BIT_FCIE_SOFT_RST_n); /* active low */
	// FCIE reset - wait
	for(u32_i=0; u32_i<TIME_WAIT_FCIE_RESET; u32_i++)
	{
		REG_FCIE_R(FCIE_CIFC_ADDR(0), au16_tmp[0]);
		REG_FCIE_R(FCIE_CIFC_ADDR(1), au16_tmp[1]);
		REG_FCIE_R(FCIE_CIFC_ADDR(2), au16_tmp[2]);
		if(0==au16_tmp[0] && 0==au16_tmp[1] && 0==au16_tmp[2])
			break;
		eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
	}
	if(TIME_WAIT_FCIE_RESET == u32_i){
		u32_err = eMMC_ST_ERR_FCIE_NO_CLK;
        EMMC_PUTS("eMMC Err: reset CIFC fail \n");
		eMMC_FCIE_ErrHandler_Stop();
	}

	// FCIE reset - clear
	REG_FCIE_SETBIT(FCIE_TEST_MODE, BIT_FCIE_SOFT_RST_n);
	// FCIE reset - check
	eMMC_FCIE_CheckResetDone();
	// miu request reset - clear
	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_REQUEST_RST);

	eMMC_clock_setting(u16_clk);
	return u32_err;
}

void eMMC_FCIE_ClearEvents(void)
{
	volatile U16 u16_reg;
	REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU_REQUEST_RST);
	while(1){
		REG_FCIE_W(FCIE_MIE_EVENT, BIT_ALL_CARD_INT_EVENTS);
		REG_FCIE_R(FCIE_MIE_EVENT, u16_reg);
		if(0==(u16_reg&BIT_ALL_CARD_INT_EVENTS))
			break;
		REG_FCIE_W(FCIE_MIE_EVENT, 0);
		REG_FCIE_W(FCIE_MIE_EVENT, 0);
	}
	REG_FCIE_W1C(FCIE_SD_STATUS, BIT_SD_FCIE_ERR_FLAGS); // W1C
	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_REQUEST_RST);
}



U32 eMMC_FCIE_Init(void)
{
	U32 u32_err;
    EMMC_PUTS("eMMC_FCIE_Init start\n");
	eMMC_PlatformResetPre();
    
	// ------------------------------------------
	u32_err = eMMC_FCIE_Reset();
	if(eMMC_ST_SUCCESS != u32_err){
        EMMC_PUTS("eMMC Err: reset fail\n");
		eMMC_FCIE_ErrHandler_Stop();
		return u32_err;
	}

	REG_FCIE_W(FCIE_MIE_INT_EN, 0);
	REG_FCIE_W(FCIE_MMA_PRI_REG, BIT_MIU_CLK_FREE_RUN);
	REG_FCIE_W(FCIE_PATH_CTRL, BIT_SD_EN);
	// all cmd are 5 bytes (excluding CRC)
	REG_FCIE_W(FCIE_CMD_SIZE, eMMC_CMD_BYTE_CNT);
	REG_FCIE_W(FCIE_SD_CTRL, 0);
	REG_FCIE_W(FCIE_SD_MODE, g_eMMCDrv.u16_Reg10_Mode);
	#if FICE_BYTE_MODE_ENABLE
	// default sector size: 0x200
	REG_FCIE_W(FCIE_SDIO_CTRL, BIT_SDIO_BLK_MODE | eMMC_SECTOR_512BYTE);
	#else
    REG_FCIE_W(FCIE_SDIO_CTRL, 0);
	#endif

	REG_FCIE_W(FCIE_PWR_SAVE_MODE, 0);
	eMMC_FCIE_ClearEvents();
    eMMC_PlatformResetPost();
    EMMC_PUTS("eMMC_FCIE_Init end\n");
	return eMMC_ST_SUCCESS;	// ok
}




U32 eMMC_FCIE_PollingEvents(U32 u32_RegAddr, U16 u16_Events, U32 u32_MicroSec)
{
	volatile U32 u32_i;
	volatile U16 u16_val;

	for(u32_i=0; u32_i<u32_MicroSec; u32_i++)
	{
		eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
		REG_FCIE_R(u32_RegAddr, u16_val);
		if(u16_Events == (u16_val & u16_Events))
			break;
	}

	if(u32_i == u32_MicroSec)
	{
        EMMC_PUTDW((u32_RegAddr-FCIE_MIE_EVENT)>>REG_OFFSET_SHIFT_BITS);
        EMMC_PUTC('\n');
        EMMC_PUTDW(u16_val);
        EMMC_PUTC('\n');
        EMMC_PUTDW(u16_Events);
        EMMC_PUTC('\n');
		return eMMC_ST_ERR_TIMEOUT_WAIT_REG0;
	}

	return eMMC_ST_SUCCESS;
}


U32 eMMC_FCIE_WaitEvents(U32 u32_RegAddr, U16 u16_Events, U32 u32_MicroSec)
{
	#if defined(ENABLE_eMMC_INTERRUPT_MODE)&&ENABLE_eMMC_INTERRUPT_MODE
	return eMMC_WaitCompleteIntr(u32_RegAddr, u16_Events, u32_MicroSec);
	#else
	if(u16_Events & BIT_MIU_LAST_DONE)
		u16_Events |= BIT_CARD_DMA_END;
	return eMMC_FCIE_PollingEvents(u32_RegAddr, u16_Events, u32_MicroSec);
	#endif
}


U32 eMMC_FCIE_SendCmd
(
	U16 u16_Mode, U16 u16_Ctrl, U32 u32_Arg, U8 u8_CmdIdx, U8 u8_RspByteCnt
)
{
	U32 u32_err, u32_Timeout = TIME_WAIT_DAT0_HIGH;
	U16 au16_tmp[3];

	if(38 == u8_CmdIdx)
		u32_Timeout = TIME_WAIT_ERASE_DAT0_HIGH;



	REG_FCIE_W(FCIE_RSP_SIZE, u8_RspByteCnt & BIT_SD_RSP_SIZE_MASK);
	REG_FCIE_W(FCIE_SD_MODE, u16_Mode);

	while(1)
	{
		REG_FCIE_W(FCIE_CIFC_ADDR(0),((u32_Arg>>24)<<8) | (0x40|u8_CmdIdx));
		REG_FCIE_W(FCIE_CIFC_ADDR(1), (u32_Arg&0xFF00) | ((u32_Arg>>16)&0xFF));
		REG_FCIE_W(FCIE_CIFC_ADDR(2),  u32_Arg&0xFF);

		REG_FCIE_R(FCIE_CIFC_ADDR(0),au16_tmp[0]);
		REG_FCIE_R(FCIE_CIFC_ADDR(1),au16_tmp[1]);
		REG_FCIE_R(FCIE_CIFC_ADDR(2),au16_tmp[2]);

		if(au16_tmp[0] == (((u32_Arg>>24)<<8) | (0x40|u8_CmdIdx))&&
			au16_tmp[1] == ((u32_Arg&0xFF00) | ((u32_Arg>>16)&0xFF))&&
			au16_tmp[2] == (u32_Arg&0xFF))
			break;
	}

	if(12 != u8_CmdIdx)
	{
		u32_err = eMMC_FCIE_WaitD0High(u32_Timeout);
		if(eMMC_ST_SUCCESS != u32_err)
			goto LABEL_SEND_CMD_ERROR;
	}

	if((u8_CmdIdx==12)||(u8_CmdIdx==24)||(u8_CmdIdx==25))
	{
	    //eMMC_CheckPowerCut();
        EMMC_PUTS("PWR_CUT\t\n");
	}

	REG_FCIE_W(FCIE_SD_CTRL, u16_Ctrl);

	#if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
    REG_FCIE_W(FCIE_MIE_INT_EN, BIT_SD_CMD_END);
	#endif
	if(g_eMMCDrv.u32_DrvFlag & DRV_FLAG_RSP_WAIT_D0H){
		u32_err = eMMC_FCIE_WaitD0High(u32_Timeout);
        if(eMMC_ST_SUCCESS != u32_err)
	        goto LABEL_SEND_CMD_ERROR;
	}

	// wait event
	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_SD_CMD_END, TIME_WAIT_CMDRSP_END);

	LABEL_SEND_CMD_ERROR:
	return u32_err;

}

// reest, etc.
U32 eMMC_CMD0(U32 u32_Arg)
{
	U32 u32_err;
	U16 u16_ctrl;

	u16_ctrl = BIT_SD_CMD_EN;
    EMMC_PUTS("CMD0 start\n");

	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_Arg, 0, 0);

	//eMMC_FCIE_CLK_DIS();
    EMMC_PUTS("CMD0 end\n");
	return u32_err;
}

void eMMC_FCIE_GetCIFC(U16 u16_WordPos, U16 u16_WordCnt, U16 *pu16_Buf)
{
	U16 u16_i;

	for(u16_i=0; u16_i<u16_WordCnt; u16_i++)
		REG_FCIE_R(FCIE_CIFC_ADDR(u16_i), pu16_Buf[u16_i]);
}

#if defined(eMMC_RSP_FROM_RAM) && eMMC_RSP_FROM_RAM
    #define eMMC_KEEP_RSP(pu8_OneRspBuf, u8_CmdIdx)         \
		if(g_eMMCDrv.u32_DrvFlag & DRV_FLAG_RSPFROMRAM_SAVE)\
			eMMC_KeepRsp(pu8_OneRspBuf, u8_CmdIdx)
#else
    #define eMMC_KEEP_RSP(pu8_OneRspBuf, u8_CmdIdx)  // NULL to save CPU a JMP/RET time
#endif

// send OCR
U32 eMMC_CMD1(void)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl, u16_reg;


	// (sector mode | byte mode) | (3.0|3.1|3.2|3.3|3.4 V)
	u32_arg = BIT30 | (BIT23|BIT22|BIT21|BIT20|BIT19|BIT18|BIT17|BIT16|BIT15);
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;

	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_arg, 1, eMMC_R3_BYTE_CNT);

	if(eMMC_ST_SUCCESS != u32_err)
	{
		return u32_err;
	}

	// check status
	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	// R3 has no CRC, so does not check BIT_SD_RSP_CRC_ERR
	if(u16_reg & BIT_SD_RSP_TIMEOUT)
	{
	    u32_err = eMMC_ST_ERR_CMD1;
		return u32_err;
	}
	else
	{	// CMD1 ok, do things here
		eMMC_FCIE_GetCIFC(0, 3, (U16*)g_eMMCDrv.au8_Rsp);
		//eMMC_dump_mem(g_eMMCDrv.au8_Rsp, eMMC_R3_BYTE_CNT);

		if(0 == (g_eMMCDrv.au8_Rsp[1] & 0x80))
			u32_err =  eMMC_ST_ERR_CMD1_DEV_NOT_RDY;
		else
		{
			g_eMMCDrv.u8_IfSectorMode = (g_eMMCDrv.au8_Rsp[1]&BIT6)>>6;
			eMMC_KEEP_RSP(g_eMMCDrv.au8_Rsp, 1);
		}
	}

	return u32_err;
}

// send CID
U32 eMMC_CMD2(void)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl, u16_reg;

	u32_arg = 0;
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_RSPR2_EN;

	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_arg, 2, eMMC_R2_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
		return u32_err;
    }
	else
	{	// check status
		REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
		if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
		{
			u32_err = eMMC_ST_ERR_CMD2;
			return u32_err;
		}
		else
		{	// CMD2 ok, do things here (get CID)
			eMMC_FCIE_GetCIFC(0, eMMC_R2_BYTE_CNT>>1, (U16*)g_eMMCDrv.au8_CID);
			//eMMC_dump_mem(g_eMMCDrv.au8_CID, eMMC_R2_BYTE_CNT);
			eMMC_KEEP_RSP(g_eMMCDrv.au8_CID, 2);
		}
	}
	//eMMC_FCIE_CLK_DIS();
	return u32_err;
}

U32 eMMC_CheckR1Error(void)
{
	U32 u32_err = eMMC_ST_SUCCESS;

	eMMC_FCIE_GetCIFC(0, 3, (U16*)g_eMMCDrv.au8_Rsp);

	if(g_eMMCDrv.au8_Rsp[1] & (eMMC_ERR_R1_31_24>>24))
	{
		u32_err = eMMC_ST_ERR_R1_31_24;
		goto LABEL_CHECK_R1_END;
	}

	if(g_eMMCDrv.au8_Rsp[2] & (eMMC_ERR_R1_23_16>>16))
	{
		u32_err = eMMC_ST_ERR_R1_23_16;
		goto LABEL_CHECK_R1_END;
	}

	if(g_eMMCDrv.au8_Rsp[3] & (eMMC_ERR_R1_15_8>>8))
	{
		u32_err = eMMC_ST_ERR_R1_15_8;
		goto LABEL_CHECK_R1_END;
	}

	if(g_eMMCDrv.au8_Rsp[4] & (eMMC_ERR_R1_7_0>>0))
	{
		u32_err = eMMC_ST_ERR_R1_7_0;
		goto LABEL_CHECK_R1_END;
	}

LABEL_CHECK_R1_END:
	return u32_err;
}


// CMD3: assign RCA. CMD7: select device
U32 eMMC_CMD3_CMD7(U16 u16_RCA, U8 u8_CmdIdx)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl, u16_reg;

	if(7 == u8_CmdIdx)
		g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_RSP_WAIT_D0H;

	u32_arg = u16_RCA<<16;
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;

	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_arg, u8_CmdIdx, eMMC_R1_BYTE_CNT);
	if(eMMC_ST_SUCCESS == u32_err)
	{	
	    // check status
		REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

		// de-select has no rsp
		if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
		{
			u32_err = eMMC_ST_ERR_CMD3_CMD7;
		}
		else
		{	// CMD3 ok, do things here
			eMMC_FCIE_GetCIFC(0, 3, (U16*)g_eMMCDrv.au8_Rsp);
			u32_err = eMMC_CheckR1Error();
		}		
	}

	//eMMC_FCIE_CLK_DIS();
	g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_RSP_WAIT_D0H;
	return u32_err;
}



#define eMMC_CMD1_RETRY_CNT    0x8000

U32 eMMC_Identify(void)
{
	U32 u32_err = eMMC_ST_SUCCESS;
	U16 u16_i;

	g_eMMCDrv.u16_RCA=1;

    EMMC_PUTS("eMMC_Identify start\n\t");

	if(eMMC_ST_SUCCESS != eMMC_FCIE_WaitD0High(TIME_WAIT_DAT0_HIGH))
	{
		EMMC_PUTS("eMMC Err: WaitD0High TO\n");
	    eMMC_FCIE_ErrHandler_Stop();
	}

	eMMC_RST_L();  eMMC_hw_timer_sleep(1);
	eMMC_RST_H();  eMMC_hw_timer_sleep(1);


	// CMD0
    EMMC_PUTS("do CMD0 \n");
	u32_err = eMMC_CMD0(0); // reset to idle state
	if(eMMC_ST_SUCCESS != u32_err)
	    return u32_err;

	// CMD1
    EMMC_PUTS("do CMD1 \n");
	for(u16_i=0; u16_i<eMMC_CMD1_RETRY_CNT; u16_i++)
	{
		u32_err = eMMC_CMD1();
		if(eMMC_ST_SUCCESS == u32_err)
			break;

		eMMC_hw_timer_sleep(2);

		if(eMMC_ST_ERR_CMD1_DEV_NOT_RDY != u32_err)
		    return u32_err;
	}

	if(eMMC_ST_SUCCESS != u32_err)
	    return u32_err;

	// CMD2
    EMMC_PUTS("do CMD2 \n");
	u32_err = eMMC_CMD2();
	if(eMMC_ST_SUCCESS != u32_err)
	    return u32_err;

	// CMD3
    EMMC_PUTS("do CMD3 \n");
	u32_err = eMMC_CMD3_CMD7(g_eMMCDrv.u16_RCA, 3);
	if(eMMC_ST_SUCCESS != u32_err)
		goto  LABEL_INIT_END;

    EMMC_PUTS("eMMC_Identify end\n");
	LABEL_INIT_END:
	return u32_err;

}


// send CSD (in R2)
U32 eMMC_CMD9(U16 u16_RCA)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl, u16_reg;

	u32_arg = u16_RCA<<16;
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_RSPR2_EN;

	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_arg, 9, eMMC_R2_BYTE_CNT);
	if(u32_err == eMMC_ST_SUCCESS)
	{	
        // check status
		REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
		if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
		{
			u32_err = eMMC_ST_ERR_CMD9;
		}
		else
		{	// CMD2 ok, do things here
			eMMC_FCIE_GetCIFC(0, eMMC_R2_BYTE_CNT>>1, (U16*)g_eMMCDrv.au8_CSD);
			//eMMC_dump_mem(g_eMMCDrv.au8_CSD, eMMC_R2_BYTE_CNT);
			eMMC_KEEP_RSP(g_eMMCDrv.au8_CSD, 9);
		}
	}

	//eMMC_FCIE_CLK_DIS();
	return u32_err;
}


//------------------------------------------------
U32 eMMC_CSD_Config(void)
{
	U32 u32_err;

	u32_err = eMMC_CMD9(g_eMMCDrv.u16_RCA);
	if(eMMC_ST_SUCCESS != u32_err)
		return u32_err;

	// ------------------------------
	g_eMMCDrv.u8_SPEC_VERS = (g_eMMCDrv.au8_CSD[1]&0x3C)>>2;
	g_eMMCDrv.u8_R_BL_LEN = g_eMMCDrv.au8_CSD[6] & 0xF;
	g_eMMCDrv.u8_W_BL_LEN = ((g_eMMCDrv.au8_CSD[13]&0x3)<<2)+
		((g_eMMCDrv.au8_CSD[14]&0xC0)>>6);

	// ------------------------------
	g_eMMCDrv.u16_C_SIZE = (g_eMMCDrv.au8_CSD[7] & 3)<<10;
	g_eMMCDrv.u16_C_SIZE += g_eMMCDrv.au8_CSD[8] << 2;
	g_eMMCDrv.u16_C_SIZE +=(g_eMMCDrv.au8_CSD[9] & 0xC0) >> 6;
	if(0xFFF == g_eMMCDrv.u16_C_SIZE)
	{
		g_eMMCDrv.u32_SEC_COUNT = 0;
	}
	else
	{
		g_eMMCDrv.u8_C_SIZE_MULT = ((g_eMMCDrv.au8_CSD[10]&3)<<1)+
			((g_eMMCDrv.au8_CSD[11]&0x80)>>7);

		g_eMMCDrv.u32_SEC_COUNT =
			(g_eMMCDrv.u16_C_SIZE+1)*
			(1<<(g_eMMCDrv.u8_C_SIZE_MULT+2))*
			((1<<g_eMMCDrv.u8_R_BL_LEN)>>9) - 8; // -8: //Toshiba CMD18 access the last block report out of range error
	}
	// ------------------------------
	g_eMMCDrv.u8_ERASE_GRP_SIZE = (g_eMMCDrv.au8_CSD[10]&0x7C)>>2;
	g_eMMCDrv.u8_ERASE_GRP_MULT = ((g_eMMCDrv.au8_CSD[10]&0x03)<<3)+
		((g_eMMCDrv.au8_CSD[11]&0xE0)>>5);
	g_eMMCDrv.u32_EraseUnitSize = (g_eMMCDrv.u8_ERASE_GRP_SIZE+1)*
		(g_eMMCDrv.u8_ERASE_GRP_MULT+1);
    // ------------------------------
	// others
	g_eMMCDrv.u8_TAAC = g_eMMCDrv.au8_CSD[2];
	g_eMMCDrv.u8_NSAC = g_eMMCDrv.au8_CSD[3];
	g_eMMCDrv.u8_Tran_Speed = g_eMMCDrv.au8_CSD[4];
	g_eMMCDrv.u8_R2W_FACTOR = (g_eMMCDrv.au8_CSD[13]&0x1C)>>2;

	return eMMC_ST_SUCCESS;
}





#define WAIT_FIFOCLK_READY_CNT  0x10000

U32 eMMC_FCIE_FifoClkRdy(U8 u8_Dir)
{
	 U32 u32_i;
	 U16 u16_Reg;

	for(u32_i=0; u32_i<TIME_WAIT_FIFOCLK_RDY; u32_i++)
	{
		REG_FCIE_R(FCIE_MMA_PRI_REG, u16_Reg);
		if(u16_Reg & BIT_FIFO_CLKRDY)
			break;
		eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
	}

	if(WAIT_FIFOCLK_READY_CNT == u32_i)
	{
	    // add debug register
		return eMMC_ST_ERR_TIMEOUT_FIFOCLKRDY;
	}

	return eMMC_ST_SUCCESS;
}


// CMD8: send EXT_CSD
U32 eMMC_CMD8_MIU(U8 *pu8_DataBuf)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;

	// -------------------------------
	// send cmd
	u32_arg =  0;
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DAT_EN;

	u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;
	if(g_eMMCDrv.u32_DrvFlag & (DRV_FLAG_DDR_MODE|DRV_FLAG_SPEED_HS200) )
	{
		REG_FCIE_W(FCIE_TOGGLE_CNT, (g_eMMCDrv.u32_DrvFlag&DRV_FLAG_SPEED_HS200) ? TOGGLE_CNT_512_CLK_R : TOGGLE_CNT_256_CLK_R);
		REG_FCIE_SETBIT(FCIE_MACRO_REDNT, BIT_TOGGLE_CNT_RST);
		REG_FCIE_CLRBIT(FCIE_MACRO_REDNT, BIT_MACRO_DIR);
		eMMC_hw_timer_delay(TIME_WAIT_FCIE_RST_TOGGLE_CNT); // Brian needs 2T
		REG_FCIE_CLRBIT(FCIE_MACRO_REDNT, BIT_TOGGLE_CNT_RST);
	}
	eMMC_FCIE_ClearEvents();
	REG_FCIE_W(FCIE_JOB_BL_CNT, 1);
	u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE);
    #if FICE_BYTE_MODE_ENABLE
	REG_FCIE_W(FCIE_SDIO_ADDR0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_SDIO_ADDR1, u32_dma_addr >> 16);
    #else
	REG_FCIE_W(FCIE_MIU_DMA_15_0, (u32_dma_addr>>MIU_BUS_WIDTH_BITS)&0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_26_16,(u32_dma_addr>>MIU_BUS_WIDTH_BITS)>>16);
    #endif
	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_DMA_DIR_W);
	u32_err = eMMC_FCIE_FifoClkRdy(0);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		goto LABEL_END;
	}
	REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_MMA_EN);

	u32_err = eMMC_FCIE_SendCmd(u16_mode, u16_ctrl, u32_arg, 8, eMMC_R1_BYTE_CNT);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		goto LABEL_END;
	}

	// -------------------------------
	// check FCIE
	#if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
	REG_FCIE_W(FCIE_MIE_INT_EN, BIT_MIU_LAST_DONE);
	#endif
	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT,
        BIT_MIU_LAST_DONE|BIT_CARD_DMA_END, TIME_WAIT_1_BLK_END);

	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(eMMC_ST_SUCCESS != u32_err || (u16_reg & BIT_SD_FCIE_ERR_FLAGS))
	{
		goto LABEL_END;
	}

	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		u32_err = eMMC_ST_ERR_CMD8_MIU;
		goto LABEL_END;
	}

    LABEL_END:
	//eMMC_FCIE_CLK_DIS();
	return u32_err;
}


U32 eMMC_CMD8(U8 *pu8_DataBuf)
{
	return eMMC_CMD8_MIU(pu8_DataBuf);
}

// SWITCH cmd
U32 eMMC_CMD6(U32 u32_Arg)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl, u16_reg;
	U8  u8_retry_r1=0, u8_retry_fcie=0, u8_retry_cmd=0;

	u32_arg = u32_Arg;
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;
	g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_RSP_WAIT_D0H;


	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_arg, 6, eMMC_R1b_BYTE_CNT);
    if(u32_err == eMMC_ST_SUCCESS)
    {
	    // check status
		REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

		if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
		{
			u32_err = eMMC_ST_ERR_CMD6;
			return u32_err;
		}
		else
		{	// CMD3 ok, do things here
			u32_err = eMMC_CheckR1Error();
			if(eMMC_ST_SUCCESS != u32_err)
                return u32_err;
		}
	}

	//eMMC_FCIE_CLK_DIS();
	g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_RSP_WAIT_D0H;
	return u32_err;
}

// CMD13: send Status
U32 eMMC_CMD13(U16 u16_RCA)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl, u16_reg;

	u32_arg = (u16_RCA<<16); 
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;

	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_arg, 13, eMMC_R1_BYTE_CNT);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		return u32_err;
	}
	else
	{	// check status
		REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

		if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
		{
			u32_err = eMMC_ST_ERR_CMD13;
			return u32_err;
		}
		else
		{	// CMD13 ok, do things here
			u32_err = eMMC_CheckR1Error();
			if(eMMC_ST_SUCCESS != u32_err)
				return u32_err;
		}
	}

	//eMMC_FCIE_CLK_DIS();
	return u32_err;
}



U32 eMMC_ModifyExtCSD(U8 u8_AccessMode, U8 u8_ByteIdx, U8 u8_Value)
{
	U32 u32_arg, u32_err;

	u32_arg = ((u8_AccessMode&3)<<24) | (u8_ByteIdx<<16) |
		      (u8_Value<<8);

	u32_err = eMMC_CMD6(u32_arg);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		EMMC_PUTS("eMMC Err: CMD6\n");
		return u32_err;
	}

	u32_err = eMMC_CMD13(g_eMMCDrv.u16_RCA);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		EMMC_PUTS("eMMC Err: CMD13\n");
		return u32_err;
	}

    return u32_err;
}


//------------------------------------------------
U32 eMMC_ExtCSD_Config(void)
{
	U32 u32_err;

	u32_err = eMMC_CMD8(gau8_eMMC_SectorBuf);
	if(eMMC_ST_SUCCESS != u32_err)
		return u32_err;

	//eMMC_dump_mem(gau8_eMMC_SectorBuf, 0x200);

	//--------------------------------
	if(0 == g_eMMCDrv.u32_SEC_COUNT)
		 g_eMMCDrv.u32_SEC_COUNT = ((gau8_eMMC_SectorBuf[215]<<24)|
		                           (gau8_eMMC_SectorBuf[214]<<16)|
		                           (gau8_eMMC_SectorBuf[213]<< 8)|
		                           (gau8_eMMC_SectorBuf[212])) - 8; //-8: Toshiba CMD18 access the last block report out of range error

	//-------------------------------
	if(0 == g_eMMCDrv.u32_BOOT_SEC_COUNT)
		 g_eMMCDrv.u32_BOOT_SEC_COUNT = gau8_eMMC_SectorBuf[226] * 128 * 2;

	//--------------------------------
	if(!g_eMMCDrv.u8_BUS_WIDTH) {
		g_eMMCDrv.u8_BUS_WIDTH = gau8_eMMC_SectorBuf[183];
		switch(g_eMMCDrv.u8_BUS_WIDTH)
		{
			case 0:  g_eMMCDrv.u8_BUS_WIDTH = BIT_SD_DATA_WIDTH_1;  break;
			case 1:  g_eMMCDrv.u8_BUS_WIDTH = BIT_SD_DATA_WIDTH_4;  break;
			case 2:  g_eMMCDrv.u8_BUS_WIDTH = BIT_SD_DATA_WIDTH_8;  break;
			default: EMMC_PUTS("eMMC Err: eMMC BUS_WIDTH not support \n");
			    while(1);
		}
	}
	//--------------------------------
	if(gau8_eMMC_SectorBuf[231]&BIT4) // TRIM
		g_eMMCDrv.u32_eMMCFlag |= eMMC_FLAG_TRIM;
	else
		g_eMMCDrv.u32_eMMCFlag &= ~eMMC_FLAG_TRIM;

	//--------------------------------
	if(gau8_eMMC_SectorBuf[503]&BIT0) // HPI
	{
		if(gau8_eMMC_SectorBuf[503]&BIT1)
			g_eMMCDrv.u32_eMMCFlag |= eMMC_FLAG_HPI_CMD12;
		else
			g_eMMCDrv.u32_eMMCFlag |= eMMC_FLAG_HPI_CMD13;
	}else
		g_eMMCDrv.u32_eMMCFlag &= ~(eMMC_FLAG_HPI_CMD12|eMMC_FLAG_HPI_CMD13);

	//--------------------------------
	if(gau8_eMMC_SectorBuf[166]&BIT2) // Reliable Write
	    g_eMMCDrv.u16_ReliableWBlkCnt = BIT_SD_JOB_BLK_CNT_MASK;
	else
	{
		#if 0
		g_eMMCDrv.u16_ReliableWBlkCnt = gau8_eMMC_SectorBuf[222];
		#else
		if((gau8_eMMC_SectorBuf[503]&BIT0) && 1==gau8_eMMC_SectorBuf[222])
		    g_eMMCDrv.u16_ReliableWBlkCnt = 1;
		else if(0==(gau8_eMMC_SectorBuf[503]&BIT0))
			g_eMMCDrv.u16_ReliableWBlkCnt = gau8_eMMC_SectorBuf[222];
		else
		{
			g_eMMCDrv.u16_ReliableWBlkCnt = 0; // can not support Reliable Write
		}
		#endif
	}

	//--------------------------------
	g_eMMCDrv.u8_ErasedMemContent = gau8_eMMC_SectorBuf[181];

	//--------------------------------
	g_eMMCDrv.u8_ECSD185_HsTiming = gau8_eMMC_SectorBuf[185];
	g_eMMCDrv.u8_ECSD192_Ver = gau8_eMMC_SectorBuf[192];
	g_eMMCDrv.u8_ECSD196_DevType = gau8_eMMC_SectorBuf[196];
	g_eMMCDrv.u8_ECSD197_DriverStrength = gau8_eMMC_SectorBuf[197];
	g_eMMCDrv.u8_ECSD248_CMD6TO = gau8_eMMC_SectorBuf[248];
	g_eMMCDrv.u8_ECSD247_PwrOffLongTO = gau8_eMMC_SectorBuf[247];
	g_eMMCDrv.u8_ECSD34_PwrOffCtrl = gau8_eMMC_SectorBuf[34];

	/*printf("[196] DEVICE TYPE = %02Xh\n", g_eMMCDrv.u8_ECSD196_DevType);
	printf("[197] DRIVER STRENGTH = %02Xh\n", g_eMMCDrv.u8_ECSD197_DriverStrength);
	printf("[185] HS_TIMING = %02Xh\n", g_eMMCDrv.u8_ECSD185_HsTiming);*/

	//--------------------------------
	// set HW RST
	if(0 == gau8_eMMC_SectorBuf[162])
	{
		u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 162, BIT0); // RST_FUNC
		if(eMMC_ST_SUCCESS != u32_err)
		{
			EMMC_PUTS("eMMC Err: set Ext_CSD[162]\n");
			return u32_err;
		}
	}

	return eMMC_ST_SUCCESS;
}

// Use CMD6 to set ExtCSD[183] BUS_WIDTH
U32 eMMC_SetBusWidth(U8 u8_BusWidth, U8 u8_IfDDR)
{
	U8  u8_value;
	U32 u32_err;

	// -------------------------------
	switch(u8_BusWidth)
	{
		case 1:  u8_value=0; break;
		case 4:  u8_value=1; break;
		case 8:  u8_value=2; break;
		default: return eMMC_ST_ERR_PARAMETER;
	}

	if(u8_IfDDR)
		u8_value |= BIT2;
	if(BIT2 == u8_value)
		return eMMC_ST_ERR_PARAMETER;

	// -------------------------------
	u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 183, u8_value); // BUS_WIDTH
	if(eMMC_ST_SUCCESS != u32_err)
		return u32_err;

	// -------------------------------
	g_eMMCDrv.u16_Reg10_Mode &= ~BIT_SD_DATA_WIDTH_MASK;
	switch(u8_BusWidth)
	{
		case 1:
			g_eMMCDrv.u8_BUS_WIDTH = BIT_SD_DATA_WIDTH_1;
			g_eMMCDrv.u16_Reg10_Mode |= BIT_SD_DATA_WIDTH_1;
			break;
		case 4:
			g_eMMCDrv.u8_BUS_WIDTH = BIT_SD_DATA_WIDTH_4;
			g_eMMCDrv.u16_Reg10_Mode |= BIT_SD_DATA_WIDTH_4;
			break;
		case 8:
			g_eMMCDrv.u8_BUS_WIDTH = BIT_SD_DATA_WIDTH_8;
			g_eMMCDrv.u16_Reg10_Mode |= BIT_SD_DATA_WIDTH_8;
			break;
	}

	return u32_err;
}

#define BITS_MSK_TIMING			0x0F

U32 eMMC_SetBusSpeed(U8 u8_BusSpeed)
{
	U32 u32_err;

	g_eMMCDrv.u8_ECSD185_HsTiming &= ~BITS_MSK_TIMING;
	g_eMMCDrv.u8_ECSD185_HsTiming |= u8_BusSpeed;

	//printf("%s(%02Xh)\n", __FUNCTION__, g_eMMCDrv.u8_ECSD185_HsTiming);

	u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 185, g_eMMCDrv.u8_ECSD185_HsTiming); // HS_TIMING, HS200
	if(eMMC_ST_SUCCESS != u32_err)
		return u32_err;

	g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_SPEED_MASK;
	switch(u8_BusSpeed)
	{
		case eMMC_SPEED_HIGH:
			g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_SPEED_HIGH;
			g_eMMCDrv.u16_Reg10_Mode |= BIT_SD_DATA_SYNC;
			break;
		case eMMC_SPEED_HS200:
			g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_SPEED_HS200;
			g_eMMCDrv.u16_Reg10_Mode &= ~BIT_SD_DATA_SYNC;
			break;
		default:
			g_eMMCDrv.u16_Reg10_Mode &= ~BIT_SD_DATA_SYNC;
	}

	return u32_err;
}





U32 eMMC_CMD12(U16 u16_RCA)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl, u16_reg;
	U8  u8_retry_fcie=0, u8_retry_r1=0, u8_retry_cmd=0;

	#if 1
	u32_arg = (u16_RCA<<16)|
		((g_eMMCDrv.u32_eMMCFlag & eMMC_FLAG_HPI_CMD12)?1:0);
	#else
    u32_arg = (u16_RCA<<16);
	#endif

	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;
	g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_RSP_WAIT_D0H;


	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_arg, 12, eMMC_R1b_BYTE_CNT);
    if(u32_err == eMMC_ST_SUCCESS)
    {
	    // check status
		REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

		if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
		{
			u32_err = eMMC_ST_ERR_CMD12;
			return u32_err;

		}
		else
		{	u32_err = eMMC_CheckR1Error();
			if(eMMC_ST_SUCCESS != u32_err)
		        return u32_err;
		}
	}

	//eMMC_FCIE_CLK_DIS();
	g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_RSP_WAIT_D0H;
	return u32_err;
}

U32 eMMC_CMD12_NoCheck(U16 u16_RCA)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl;

	#if 1
	u32_arg = (u16_RCA<<16)|
		((g_eMMCDrv.u32_eMMCFlag & eMMC_FLAG_HPI_CMD12)?1:0);
	#else
    u32_arg = (u16_RCA<<16);
	#endif

	#if defined(eMMC_UPDATE_FIRMWARE) && (eMMC_UPDATE_FIRMWARE)
    u32_arg = 0;
	#endif

	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;
	g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_RSP_WAIT_D0H;

	//LABEL_SEND_CMD:
	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_arg, 12, eMMC_R1b_BYTE_CNT);

	g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_RSP_WAIT_D0H;
	return u32_err;
}


U32 eMMC_CMD18_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;

	// -------------------------------
	// send cmd
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DAT_EN;

	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv.u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);
	u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;

	eMMC_FCIE_ClearEvents();
	REG_FCIE_W(FCIE_JOB_BL_CNT, u16_BlkCnt);
	u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE*u16_BlkCnt);
	#if FICE_BYTE_MODE_ENABLE
	REG_FCIE_W(FCIE_SDIO_ADDR0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_SDIO_ADDR1, u32_dma_addr >> 16);
	#else
	REG_FCIE_W(FCIE_MIU_DMA_15_0, (u32_dma_addr >> MIU_BUS_WIDTH_BITS)& 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_26_16,(u32_dma_addr >> MIU_BUS_WIDTH_BITS)>>16);
	#endif
	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_DMA_DIR_W);
	u32_err = eMMC_FCIE_FifoClkRdy(0);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		goto LABEL_END;
	}
	REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_MMA_EN);

	u32_err = eMMC_FCIE_SendCmd(u16_mode, u16_ctrl, u32_arg, 18, eMMC_R1_BYTE_CNT);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		goto LABEL_END;
	}

	// -------------------------------
	// check FCIE
	#if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
	REG_FCIE_W(FCIE_MIE_INT_EN, BIT_MIU_LAST_DONE);
	#endif
	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT,
	    BIT_MIU_LAST_DONE|BIT_CARD_DMA_END, TIME_WAIT_n_BLK_END*(1+(u16_BlkCnt>>9)));

	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(eMMC_ST_SUCCESS != u32_err || (u16_reg & BIT_SD_FCIE_ERR_FLAGS))
	{
		u32_err = eMMC_ST_ERR_CMD18;
		goto LABEL_END;
	}

	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
		goto LABEL_END;

	LABEL_END:
	if(eMMC_ST_SUCCESS != eMMC_CMD12(g_eMMCDrv.u16_RCA))
		eMMC_CMD12_NoCheck(g_eMMCDrv.u16_RCA);

	//eMMC_FCIE_CLK_DIS();
	return u32_err;
}

void eMMC_FCIE_GetCIFD(U16 u16_WordPos, U16 u16_WordCnt, U16 *pu16_Buf)
{
	U16 u16_i;

	for(u16_i=0; u16_i<u16_WordCnt; u16_i++)
		REG_FCIE_R(FCIE_CIFD_ADDR(u16_i), pu16_Buf[u16_i]);
}


U32 eMMC_CMD18(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt)
{
    return eMMC_CMD18_MIU(u32_eMMCBlkAddr, pu8_DataBuf, u16_BlkCnt);

}


U32 eMMC_CMD25_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;

	// -------------------------------
	// send cmd
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;

	u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;
	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv.u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);

	eMMC_FCIE_ClearEvents();
	REG_FCIE_W(FCIE_JOB_BL_CNT, u16_BlkCnt);
	u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE*u16_BlkCnt);
	#if FICE_BYTE_MODE_ENABLE
	REG_FCIE_W(FCIE_SDIO_ADDR0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_SDIO_ADDR1, u32_dma_addr >> 16);
	#else
	REG_FCIE_W(FCIE_MIU_DMA_15_0, (u32_dma_addr >> MIU_BUS_WIDTH_BITS)& 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_26_16,(u32_dma_addr >> MIU_BUS_WIDTH_BITS)>>16);
	#endif
	REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_DMA_DIR_W);
	u32_err = eMMC_FCIE_FifoClkRdy(BIT_DMA_DIR_W);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		goto LABEL_END;
	}
	REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_MMA_EN);

	u32_err = eMMC_FCIE_SendCmd(
		u16_mode, u16_ctrl, u32_arg, 25, eMMC_R1_BYTE_CNT);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		goto LABEL_END;
	}

	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		u32_err = eMMC_ST_ERR_CMD25_CHK_R1;
		goto LABEL_END;
	}

	// -------------------------------
	// send data
	u32_err = eMMC_FCIE_WaitD0High(TIME_WAIT_DAT0_HIGH);
	if(eMMC_ST_SUCCESS != u32_err)
	{
	    u32_err = eMMC_ST_ERR_CMD25_WAIT_D0H;
		goto LABEL_END;
	}

	REG_FCIE_W(FCIE_SD_CTRL, BIT_SD_DAT_EN|BIT_SD_DAT_DIR_W);
	#if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
    REG_FCIE_W(FCIE_MIE_INT_EN, BIT_CARD_DMA_END);
	#endif

	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT,
	    BIT_CARD_DMA_END, TIME_WAIT_n_BLK_END*(1+(u16_BlkCnt>>7)));


	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(eMMC_ST_SUCCESS != u32_err || (u16_reg & (BIT_SD_W_FAIL|BIT_SD_W_CRC_ERR)))
		goto LABEL_END;


	LABEL_END:
	if(eMMC_ST_SUCCESS != eMMC_CMD12(g_eMMCDrv.u16_RCA))
		eMMC_CMD12_NoCheck(g_eMMCDrv.u16_RCA);

	//eMMC_FCIE_CLK_DIS();
	return u32_err;
}


U32 eMMC_CMD24_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;

	// -------------------------------
	// send cmd
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;

	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv.u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);
	u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;

	eMMC_FCIE_ClearEvents();
	REG_FCIE_W(FCIE_JOB_BL_CNT, 1);
	u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE);
    #if FICE_BYTE_MODE_ENABLE
	REG_FCIE_W(FCIE_SDIO_ADDR0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_SDIO_ADDR1, u32_dma_addr >> 16);
	#else
    REG_FCIE_W(FCIE_MIU_DMA_15_0, (u32_dma_addr>>MIU_BUS_WIDTH_BITS)&0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_26_16,(u32_dma_addr>>MIU_BUS_WIDTH_BITS)>>16);
	#endif
    REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_DMA_DIR_W);
	u32_err = eMMC_FCIE_FifoClkRdy(BIT_DMA_DIR_W);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		goto LABEL_END;
	}
	REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_MMA_EN);

	u32_err = eMMC_FCIE_SendCmd(
		u16_mode, u16_ctrl, u32_arg, 24, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
		goto LABEL_END;
    }

	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		u32_err = eMMC_ST_ERR_CMD24_MIU_CHK_R1;
		goto LABEL_END;
	}

	// -------------------------------
	// send data
	u32_err = eMMC_FCIE_WaitD0High(TIME_WAIT_DAT0_HIGH);
	if(eMMC_ST_SUCCESS != u32_err)
	{
	    u32_err = eMMC_ST_ERR_CMD24_MIU_WAIT_D0H;
		goto LABEL_END;
	}

	REG_FCIE_W(FCIE_SD_CTRL, BIT_SD_DAT_EN|BIT_SD_DAT_DIR_W);
	#if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
    REG_FCIE_W(FCIE_MIE_INT_EN, BIT_CARD_DMA_END);
	#endif
	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT,
	    BIT_CARD_DMA_END, TIME_WAIT_1_BLK_END);

	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(eMMC_ST_SUCCESS != u32_err || (u16_reg & (BIT_SD_W_FAIL|BIT_SD_W_CRC_ERR)))
	{
		u32_err = eMMC_ST_ERR_CMD24_MIU;
		goto LABEL_END;
	}

	LABEL_END:
	//eMMC_FCIE_CLK_DIS();
	return u32_err;
}


U32 eMMC_CMD24(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf)
{
    return eMMC_CMD24_MIU(u32_eMMCBlkAddr, pu8_DataBuf);

}

U32 eMMC_CMD17_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;

	// -------------------------------
	// send cmd
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DAT_EN;

	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv.u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);
	u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;

	eMMC_FCIE_ClearEvents();
	REG_FCIE_W(FCIE_JOB_BL_CNT, 1);
	u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE);
	#if FICE_BYTE_MODE_ENABLE
	REG_FCIE_W(FCIE_SDIO_ADDR0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_SDIO_ADDR1, u32_dma_addr >> 16);
	#else
	REG_FCIE_W(FCIE_MIU_DMA_15_0, (u32_dma_addr>>MIU_BUS_WIDTH_BITS)&0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_26_16,(u32_dma_addr>>MIU_BUS_WIDTH_BITS)>>16);
	#endif
	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_DMA_DIR_W);
	u32_err = eMMC_FCIE_FifoClkRdy(0);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		goto LABEL_END;
	}
	REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_MMA_EN);

	u32_err = eMMC_FCIE_SendCmd(
		u16_mode, u16_ctrl, u32_arg, 17, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
		goto LABEL_END;
    }

	// -------------------------------
	// check FCIE
	#if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
    REG_FCIE_W(FCIE_MIE_INT_EN, BIT_MIU_LAST_DONE);
	#endif
	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT,
	    BIT_MIU_LAST_DONE|BIT_CARD_DMA_END, TIME_WAIT_1_BLK_END);

	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(eMMC_ST_SUCCESS != u32_err || (u16_reg & BIT_SD_FCIE_ERR_FLAGS))
	{
		u32_err = eMMC_ST_ERR_CMD17_MIU;
		goto LABEL_END;
	}

	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
	    u32_err = eMMC_ST_ERR_CMD17_MIU;
		goto LABEL_END;
	}

	LABEL_END:
	//eMMC_FCIE_CLK_DIS();
	return u32_err;
}


U32 eMMC_CMD17(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf)
{
    return eMMC_CMD17_MIU(u32_eMMCBlkAddr, pu8_DataBuf);
}




