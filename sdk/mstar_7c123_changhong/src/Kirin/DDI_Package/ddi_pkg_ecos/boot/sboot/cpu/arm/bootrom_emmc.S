    .include "bootrom_emmc.inc"
    .include "macro.inc"


.macro eMMC_PADS_SWITCH
	@ REG_SET_BITS_UINT16(REG_CHIPTOP_0x29, BIT13);
	LDR		r12, =REG_CHIPTOP_0x29
	LDR		r11, [r12]
	ORR		r11, r11, #0x2000
	STR		r11, [r12]

	@ REG_WRITE_UINT16(REG_FCIE_CLK, 0x0135);
	LDR 	r11,=0x0134
	LDR		r12, =REG_FCIE_CLK
	STR		r11, [r12]

    LDR     r12, =FCIE_REG_0X2D
    LDR		r11, [r12]
    LDR     r12, =0xFFFFBF7F
    AND		r11, r11, r12
    LDR     r12, =FCIE_REG_0X2D
    STR		r11, [r12]
 
	@ REG_CLR_BITS_UINT16(FCIE_BOOT_CONFIG, (BIT_SD_DDR_EN | BIT_SD_FROM_TMUX));	//bit9 bit12
	LDR		r12, =FCIE_BOOT_CONFIG
	LDR		r11, [r12]
	AND		r11, r11, #0xFFFFEDFF
	STR		r11, [r12]

	@ REG_SET_BITS_UINT16(FCIE_BOOT_CONFIG, BIT_MACRO_EN | BIT_SD_BYPASS_MODE_EN | BIT_SD_SDR_IN_BYPASS);	//bit8 bit10 bit11
	LDR		r12, =FCIE_BOOT_CONFIG
	LDR		r11, [r12]
	ORR		r11, r11, #0xD00
	STR		r11, [r12]

	@	// set chiptop
	@	REG_READ_UINT16(REG_CHIPTOP_0x06, u16temp);
	@	u16temp &= ~(BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT12|BIT13);
	@	u16temp |= BIT12;
	@	REG_WRITE_UINT16(REG_CHIPTOP_0x06, u16temp);
	LDR		r12, =REG_CHIPTOP_0x06
	LDR		r11, [r12]
	@ AND		r11, r11, #0xFFFFCFC0 @ this could result in problem
	BIC		r11, r11, #0x3000
	BIC		r11, r11, #0x3F
	ORR		r11, r11, #0x1000
	STR		r11, [r12]

	@	REG_READ_UINT16(REG_CHIPTOP_0x35, u16temp);
	@	u16temp |= BIT6|BIT7;
	@	REG_WRITE_UINT16(REG_CHIPTOP_0x35, u16temp);
	LDR		r12, =REG_CHIPTOP_0x35
	LDR		r11, [r12]
	ORR		r11, r11, #0xC0
	STR		r11, [r12]

	@	// clr Pad Ctrl all_pad_in
	@	REG_CLR_BITS_UINT16(REG_CHIPTOP_0x50, BIT_all_pad_in);	//bit15
	@	return eMMC_ST_SUCCESS;
	LDR		r12, =REG_CHIPTOP_0x50
	LDR		r11, [r12]
	AND		r11, r11, #0xFFFF7FFF
	STR		r11, [r12]

.endm


@--------------------------------------------------------------------------------
@  check_emmc_exist_asm
@--------------------------------------------------------------------------------
    .globl check_emmc_exist_asm
check_emmc_exist_asm:
	@PUSH		{LR}
	@B		BOOT_CopyUboot_EMMC_Stage1
	@ REG_SET_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT4);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	ORR		r11, r11, #0x10
	STR		r11, [r12]

	@ REG_CLR_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT0);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	@AND		r11, r11, #0xFFFFFFFE
	BIC		r11, r11, #0x01
	STR		r11, [r12]

	
	@ delay 10T

	MOVS		r11, #10
delay10_1_0:
	SUBS		r11, #1
	BNE		delay10_1_0

	@ REG_CLR_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT4);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	AND		r11, r11, #0xFFFFFFEF
	STR		r11, [r12]

	
	@ call eMMC_pads_switch();
	eMMC_PADS_SWITCH

	@ REG_WRITE_UINT16(FCIE_BOOT_CONFIG, 0x0D18);	 //nand_boot enable and set bootsram_access_sel to 1	//reg2F
	LDR		r11, =0x0D18
	LDR		r12, =FCIE_BOOT_CONFIG
	STR		r11, [r12]

	@ REG_WRITE_UINT16(FCIE_PATH_CTL, 0x02);			 //set SD enable		//reg0A
	MOVS		r11, #0x02
	LDR		r12, =FCIE_PATH_CTL
	STR		r11, [r12]

	@ REG_WRITE_UINT16(FCIE_SD_MODE, 0x54);			 //set SD port and dataline and fifo dest	//reg10
	MOVS		r11, #0x54
	LDR		r12, =FCIE_SD_MODE
	STR		r11, [r12]

	@	REG_WRITE_UINT16(FCIE_MIE_INT_EN, 0xFF);		// set interrupt enable //reg01
	@MOVS		r11, #0xFF
	@LDR		r12, =FCIE_MIE_INT_EN
	@STR		r11, [r12]

	@	REG_WRITE_UINT16(FCIE_JOB_BL_CNT, length);	// set job_block_cnt	//reg0B
	LDR		r12, =REG_JOB_BL_CNT
	STR		r10, [r12]

	@	REG_WRITE_UINT16(FCIE_SD_CTRL, 0x08);				// set SD datrx_en		//reg11
	@MOVS		r11, #0x08
	@LDR		r12, =FCIE_SD_CTRL
	@STR		r11, [r12]

	@	REG_WRITE_UINT16(FCIE_SDIO_CTL, 0x200);			//reg1B
	MOVS		r11, #0x200
	LDR		r12, =FCIE_SDIO_CTL
	STR		r11, [r12]

	@	REG_WRITE_UINT16(FCIE_MMA_PRI_REG, 0);			// set MMA read //reg02
	MOVS		r11, #0
	LDR		r12, =FCIE_MMA_PRI_REG
	STR		r11, [r12]

	@ REG_WRITE_UINT16(FCIE_MIU_DMA_15_0, (dma_addr & 0xFFFF));	// set MMA Address LOW //reg4
	LDR		r1, =0xFFFF
	AND		r11, r8, r1
	LDR		r12, =FCIE_MIU_DMA_15_0
	STR		r11, [r12]

	@ REG_WRITE_UINT16(FCIE_MIU_DMA_26_16, (u32_dma_addr>>16));	// set MMA Address HIGH //reg3
	MOV		r11, r8, LSR #16
	LDR		r12, =FCIE_MIU_DMA_26_16
	STR		r11, [r12]

	@  //eMMC_FCIE_FifoClkRdy
	@   while(1)
        @  {
        @      REG_READ_UINT16(FCIE_MMA_PRI_REG, u16_Reg);
        @      if(u16_Reg & BIT_NC_FIFO_CLKRDY)		//BIT5
        @          break;
        @  }
WAIT_FIFO_CLK_READY_1_0:
	LDR		r12, =FCIE_MMA_PRI_REG
	LDR		r11, [r12]
	AND		r11, r11, #0x20
	MOVS		r0, #0
	CMP		r11, r0
	BEQ		WAIT_FIFO_CLK_READY_1_0

	@ REG_SET_UINT16(FCIE_PATH_CTL, BIT0);	//REG_WRITE_UINT16(FCIE_PATH_CTL, 0x03); //set MMA enable //reg0A
	LDR		r12, =FCIE_PATH_CTL
	LDR		r11, [r12]
	ORR		r11, r11, #1
	STR		r11, [r12]
	
	@ // set boot stage1 by SW
	@ REG_SET_UINT16(FCIE_BOOT_CONFIG, BIT2);  	//REG_WRITE_UINT16(FCIE_BOOT_CONFIG, 0x091C);	 //reg2F
	LDR		r12, =FCIE_BOOT_CONFIG
	LDR		r11, [r12]
	ORR		r11, r11, #4
	STR		r11, [r12]
    
	@ //wait Last_done
	@  while(1)
        @  {
        @      REG_READ_UINT16(FCIE_MIE_EVENT, u16_Reg);
        @      if(u16_Reg & BIT14)
        @          break;
        @  }
 
    MOV     r8, #0
WAIT_LAST_DONE_1_0:
	LDR		r12, =FCIE_MIE_EVENT
	LDR		r11, [r12]
	AND		r11, r11, #0x4000
	MOVS	r0, #0
	ADD     r8,r8, #1
	CMP     r8, #0x2500
	BEQ		WAIT_FAIL
	CMP		r11, r0
	BEQ		WAIT_LAST_DONE_1_0
   
    @ Wait OK!	
    @MOV     r8, #0x1
	@B       RESTORE_REG
 	
	@REG_SET_UINT16(FCIE_MIE_EVENT, BIT14);  	//REG_WRITE_UINT16(FCIE_MIE_EVENT, BIT14);	 //reg0
	@LDR		r12, =FCIE_BOOT_CONFIG
	LDR		r12, =FCIE_MIE_EVENT
	LDR		r11, [r12]
	ORR		r11, r11, #0x4000
	STR		r11, [r12]

	@REG_SET_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT4);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	ORR		r11, r11, #0x10
	STR		r11, [r12]

	@REG_SET_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT0);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	ORR		r11, r11, #0x01
	STR		r11, [r12]

	@ delay 10T
	MOVS		r11, #10
delay10_2_0:
	SUBS		r11, #1
	BNE		delay10_2_0

	@ REG_CLR_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT4);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	AND		r11, r11, #0xFFFFFFEF
	STR		r11, [r12]
	
	@ return
	@POP		{PC}
	MOV		PC, LR
	
WAIT_FAIL:    
    @Clean wait job
    LDR		r12, =FCIE0_BASE
	LDR		r11, [r12]
	AND		r11, r11, #100
	STR		r11, [r12]
    MOV     r8, #0x0

    @RESTORE_REG:
    
	@REG_SET_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT4);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	ORR		r11, r11, #0x10
	STR		r11, [r12]

	@REG_SET_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT0);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	ORR		r11, r11, #0x01
	STR		r11, [r12]

	@ REG_CLR_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT4);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	AND		r11, r11, #0xFFFFFFEF
	STR		r11, [r12]
	
	@ return
	MOV		PC, LR
	
@----------------------------------------------------------------
@ BOOT_CopyUboot_EMMC_Stage1 1st load emmc bootpart function
@----------------------------------------------------------------
    .globl BOOT_CopyUboot_EMMC_Stage1
BOOT_CopyUboot_EMMC_Stage1:
	@PUSH		{LR}
	@B		BOOT_CopyUboot_EMMC_Stage1
	@ REG_SET_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT4);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	ORR		r11, r11, #0x10
	STR		r11, [r12]

	@ REG_CLR_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT0);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	@AND		r11, r11, #0xFFFFFFFE
	BIC		r11, r11, #0x01
	STR		r11, [r12]

	
	@ delay 10T

	MOVS		r11, #10
delay10_1:
	SUBS		r11, #1
	BNE		delay10_1

	@ REG_CLR_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT4);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	AND		r11, r11, #0xFFFFFFEF
	STR		r11, [r12]

	
	@ call eMMC_pads_switch();
	eMMC_PADS_SWITCH

	@ REG_WRITE_UINT16(FCIE_BOOT_CONFIG, 0x0D18);	 //nand_boot enable and set bootsram_access_sel to 1	//reg2F
	LDR		r11, =0x0D18
	LDR		r12, =FCIE_BOOT_CONFIG
	STR		r11, [r12]

	@ REG_WRITE_UINT16(FCIE_PATH_CTL, 0x02);			 //set SD enable		//reg0A
	MOVS		r11, #0x02
	LDR		r12, =FCIE_PATH_CTL
	STR		r11, [r12]

	@ REG_WRITE_UINT16(FCIE_SD_MODE, 0x54);			 //set SD port and dataline and fifo dest	//reg10
	MOVS		r11, #0x54
	LDR		r12, =FCIE_SD_MODE
	STR		r11, [r12]

	@	REG_WRITE_UINT16(FCIE_MIE_INT_EN, 0xFF);		// set interrupt enable //reg01
	@MOVS		r11, #0xFF
	@LDR		r12, =FCIE_MIE_INT_EN
	@STR		r11, [r12]

	@	REG_WRITE_UINT16(FCIE_JOB_BL_CNT, length);	// set job_block_cnt	//reg0B
	LDR		r12, =REG_JOB_BL_CNT
	STR		r10, [r12]

	@	REG_WRITE_UINT16(FCIE_SD_CTRL, 0x08);				// set SD datrx_en		//reg11
	@MOVS		r11, #0x08
	@LDR		r12, =FCIE_SD_CTRL
	@STR		r11, [r12]

	@	REG_WRITE_UINT16(FCIE_SDIO_CTL, 0x200);			//reg1B
	MOVS		r11, #0x200
	LDR		r12, =FCIE_SDIO_CTL
	STR		r11, [r12]

	@	REG_WRITE_UINT16(FCIE_MMA_PRI_REG, 0);			// set MMA read //reg02
	MOVS		r11, #0
	LDR		r12, =FCIE_MMA_PRI_REG
	STR		r11, [r12]

	@ REG_WRITE_UINT16(FCIE_MIU_DMA_15_0, (dma_addr & 0xFFFF));	// set MMA Address LOW //reg4
	LDR		r1, =0xFFFF
	AND		r11, r8, r1
	LDR		r12, =FCIE_MIU_DMA_15_0
	STR		r11, [r12]

	@ REG_WRITE_UINT16(FCIE_MIU_DMA_26_16, (u32_dma_addr>>16));	// set MMA Address HIGH //reg3
	MOV		r11, r8, LSR #16
	LDR		r12, =FCIE_MIU_DMA_26_16
	STR		r11, [r12]

	@  //eMMC_FCIE_FifoClkRdy
	@   while(1)
        @  {
        @      REG_READ_UINT16(FCIE_MMA_PRI_REG, u16_Reg);
        @      if(u16_Reg & BIT_NC_FIFO_CLKRDY)		//BIT5
        @          break;
        @  }
WAIT_FIFO_CLK_READY_1:
	LDR		r12, =FCIE_MMA_PRI_REG
	LDR		r11, [r12]
	AND		r11, r11, #0x20
	MOVS		r0, #0
	CMP		r11, r0
	BEQ		WAIT_FIFO_CLK_READY_1

	@ REG_SET_UINT16(FCIE_PATH_CTL, BIT0);	//REG_WRITE_UINT16(FCIE_PATH_CTL, 0x03); //set MMA enable //reg0A
	LDR		r12, =FCIE_PATH_CTL
	LDR		r11, [r12]
	ORR		r11, r11, #1
	STR		r11, [r12]
	
	@ // set boot stage1 by SW
	@ REG_SET_UINT16(FCIE_BOOT_CONFIG, BIT2);  	//REG_WRITE_UINT16(FCIE_BOOT_CONFIG, 0x091C);	 //reg2F
	LDR		r12, =FCIE_BOOT_CONFIG
	LDR		r11, [r12]
	ORR		r11, r11, #4
	STR		r11, [r12]
    
	@ //wait Last_done
	@  while(1)
        @  {
        @      REG_READ_UINT16(FCIE_MIE_EVENT, u16_Reg);
        @      if(u16_Reg & BIT14)
        @          break;
        @  }
WAIT_LAST_DONE_1:
	LDR		r12, =FCIE_MIE_EVENT
	LDR		r11, [r12]
	AND		r11, r11, #0x4000
	MOVS		r0, #0
	CMP		r11, r0
	BEQ		WAIT_LAST_DONE_1

	@REG_SET_UINT16(FCIE_MIE_EVENT, BIT14);  	//REG_WRITE_UINT16(FCIE_MIE_EVENT, BIT14);	 //reg0
	@LDR		r12, =FCIE_BOOT_CONFIG
	LDR		r12, =FCIE_MIE_EVENT
	LDR		r11, [r12]
	ORR		r11, r11, #0x4000
	STR		r11, [r12]

	@REG_SET_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT4);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	ORR		r11, r11, #0x10
	STR		r11, [r12]

	@REG_SET_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT0);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	ORR		r11, r11, #0x01
	STR		r11, [r12]

	@ delay 10T
	MOVS		r11, #10
delay10_2:
	SUBS		r11, #1
	BNE		delay10_2

	@ REG_CLR_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT4);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	AND		r11, r11, #0xFFFFFFEF
	STR		r11, [r12]
	
	@ return
	@POP		{PC}
	MOV		PC, LR

EMMC_MAX_BLK_CNT = 4095


@------------------------------------------------------------------------------
@ BOOT_CopyUboot_EMMC_Stage2: After 1 stage to load emmc bootpart function
@ r7 : dst
@ r8 : src
@ r9 : size
@------------------------------------------------------------------------------
    .globl BOOT_CopyUboot_EMMC_Stage2
BOOT_CopyUboot_EMMC_Stage2:
	@B		BOOT_CopyUboot_EMMC_Stage2
	@PUSH		{LR}

    CMP r7 , #1
    BEQ DRAM_MODE_BYPASS_0
	
	@ REG_SET_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT4);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	ORR		r11, r11, #0x10
	STR		r11, [r12]

	@ REG_CLR_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT0);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	@AND		r11, r11, #0xFFFFFFFE
	BIC		r11, r11, #0x01
	STR		r11, [r12]

	@ delay 10T
	MOVS		r11, #10
delay10_21:
	SUBS		r11, #1
	BNE		delay10_21

	@ REG_CLR_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT4);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	AND		r11, r11, #0xFFFFFFEF
	STR		r11, [r12]

	B GENERAL

DRAM_MODE_BYPASS_0 :
    @ REG_CLR_BITS_UINT16(FCIE_BOOT_CONFIG, BIT4);
	LDR		r12, =FCIE_BOOT_CONFIG
	LDR		r11, [r12]
	AND		r11, r11, #0xFFFFFFE7
	STR		r11, [r12]  

GENERAL :
	@ REG_WRITE_UINT16(FCIE_JOB_BL_CNT, length);	// set job_block_cnt	//reg0B
	LDR		r12, =REG_JOB_BL_CNT
	STR		r10, [r12]

	@ REG_WRITE_UINT16(FCIE_SD_CTRL, 0x08);				// set SD datrx_en		//reg11
	MOVS		r11, #0x08
	LDR		r12, =FCIE_SD_CTRL
	STR		r11, [r12]

	@ REG_WRITE_UINT16(FCIE_SDIO_CTL, 0x200);			//reg1B
	MOVS		r11, #0x200
	LDR		r12, =FCIE_SDIO_CTL
	STR		r11, [r12]

	@ REG_WRITE_UINT16(FCIE_MMA_PRI_REG, 0);			// set MMA read //reg02
	MOVS		r11, #0
	LDR		r12, =FCIE_MMA_PRI_REG
	STR		r11, [r12]

	@ REG_WRITE_UINT16(FCIE_MIU_DMA_15_0, (dma_addr & 0xFFFF));	// set MMA Address LOW //reg4
	LDR		r1, =0xFFFF
	AND		r11, r8, r1
	LDR		r12, =FCIE_MIU_DMA_15_0
	STR		r11, [r12]

	@ REG_WRITE_UINT16(FCIE_MIU_DMA_26_16, (u32_dma_addr>>16));	// set MMA Address HIGH //reg3
	MOV		r11, r8, LSR #16
	LDR		r12, =FCIE_MIU_DMA_26_16
	STR		r11, [r12]

	@  //eMMC_FCIE_FifoClkRdy
	@  while(1)
	@  {
	@      REG_READ_UINT16(FCIE_MMA_PRI_REG, u16_Reg);
	@      if(u16_Reg & BIT_NC_FIFO_CLKRDY)		//BIT5
	@          break;
	@  }
WAIT_FIFO_CLK_READY_2:
	LDR		r12, =FCIE_MMA_PRI_REG
	LDR		r11, [r12]
	AND		r11, r11, #0x20
	MOVS		r0, #0
	CMP		r11, r0
	BEQ		WAIT_FIFO_CLK_READY_2

	@ REG_SET_UINT16(FCIE_PATH_CTL, BIT0);	//REG_WRITE_UINT16(FCIE_PATH_CTL, 0x03);		 //set MMA enable//reg0A
	LDR		r12, =FCIE_PATH_CTL
	LDR		r11, [r12]
	ORR		r11, r11, #1
	STR		r11, [r12]
  
	@// set boot stage2
	@ REG_SET_UINT16(FCIE_BOOT_CONFIG, BIT0);  	//REG_WRITE_UINT16(FCIE_BOOT_CONFIG, h0919);	 //reg2F
	LDR		r12, =FCIE_BOOT_CONFIG
	LDR		r11, [r12]
	ORR		r11, r11, #1
	STR		r11, [r12]

    @ //wait Last_done
	@  while(1)
	@  {
	@      REG_READ_UINT16(FCIE_MIE_EVENT, u16_Reg);
	@      if(u16_Reg & BIT14)
	@          break;
	@  }
WAIT_LAST_DONE_2:
	LDR		r12, =FCIE_MIE_EVENT
	LDR		r11, [r12]
	AND		r11, r11, #0x4000
	MOVS		r0, #0
	CMP		r11, r0
	BEQ		WAIT_LAST_DONE_2

	@ REG_SET_UINT16(FCIE_MIE_EVENT, BIT14);  	//REG_WRITE_UINT16(FCIE_MIE_EVENT, BIT14);	 //reg0
	@LDR		r12, =FCIE_BOOT_CONFIG
	LDR		r12, =FCIE_MIE_EVENT
	LDR		r11, [r12]
	ORR		r11, r11, #0x4000
	STR		r11, [r12]

    CMP r7 , #1
    BEQ DRAM_MODE_BYPASS

  	@REG_SET_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT4);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	ORR		r11, r11, #0x10
	STR		r11, [r12]

	@REG_SET_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT0);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	ORR		r11, r11, #0x01
	STR		r11, [r12]

	@ delay 10T
	MOVS		r11, #10
delay10_22:
	SUBS		r11,  #1
	BNE		delay10_22
	

	@ REG_CLR_BITS_UINT16(GET_REG_ADDR(BANK_1018_BASE, 0x60), BIT4);
	LDR		r12, =REG_BANK_1018_0x60
	LDR		r11, [r12]
	AND		r11, r11, #0xFFFFFFEF
	STR		r11, [r12]

DRAM_MODE_BYPASS:     
	//return
	@POP		{PC}
	MOV		PC, LR


@------------------------------------------------------------------------------
@ BootEnd: 
@------------------------------------------------------------------------------
    .globl BootEnd
BootEnd:
	@B		BootEnd
	@PUSH		{LR}
	@ REG_SET_UINT16(FCIE_BOOT_CONFIG, BIT1);  	//REG_WRITE_UINT16(FCIE_BOOT_CONFIG, 0x091A);	 //reg2F
	LDR		r12, =FCIE_BOOT_CONFIG
	LDR		r11, [r12]
	BIC		r11, r11, #0x2
	STR		r11, [r12]

	@ delay 10T
	MOVS		r11, #10
BootEnd_delay10:
	SUBS		r11,  #1
	BNE		BootEnd_delay10

	LDR		r12, =FCIE_BOOT_CONFIG
	LDR		r11, [r12]
	ORR		r11, r11, #0x2
	STR		r11, [r12]

	@while(1)
	@  {
	@      REG_READ_UINT16(FCIE_MIE_EVENT, u16_Reg);
	@      if(u16_Reg & BIT15)
	@          break;
	@  }
WAIT_BOOT_END:
	LDR		r12, =FCIE_MIE_EVENT
	LDR		r11, [r12]
	AND		r11, r11, #0x8000
	MOVS		r0, #0
	CMP		r11, r0
	BEQ		WAIT_BOOT_END

	@ REG_WRITE_UINT16(FCIE_MIE_EVENT,0x8805));
	LDR		r11, =8805
	LDR		r12, =FCIE_MIE_EVENT
	STR		r11, [r12]
	@POP		{PC}

	@ REG_CLR_BITS_UINT16(FCIE_BOOT_CONFIG, BIT4);
	LDR		r12, =FCIE_BOOT_CONFIG
	LDR		r11, [r12]
	AND		r11, r11, #0xFFFFFFE7
	STR		r11, [r12] 
	
	MOV		PC, LR


@------------------------------------------------------------------------------
@ RESET_eMMC: 
@------------------------------------------------------------------------------
    .globl RESET_eMMC
RESET_eMMC:
	@B		RESET_eMMC
	@PUSH		{LR}
	@ REG_SET_BITS_UINT16(REG_CHIPTOP_0x06, BIT5);
	LDR		r12, =REG_CHIPTOP_0x06
	LDR		r11, [r12]
	ORR		r11, r11, #0x20
	STR		r11, [r12]

	@ REG_CLR_BITS_UINT16(REG_CHIPTOP_0x06, BIT4);
	LDR		r12, =REG_CHIPTOP_0x06
	LDR		r11, [r12]
	AND		r11, r11, #0xFFFFFFEF
	STR		r11, [r12]

	@delay_1ms
	@LDR		r11, =1000000 @takes too long?
	LDR		r11, =10000
delay1000000:
	SUBS		r11, #1
	BNE		delay1000000

	@ REG_SET_BITS_UINT16(REG_CHIPTOP_0x06, BIT4);
	LDR		r12, =REG_CHIPTOP_0x06
	LDR		r11, [r12]
	ORR		r11, r11, #0x10
	STR		r11, [r12]

	@ REG_CLR_BITS_UINT16(REG_CHIPTOP_0x06, BIT5);
	LDR		r12, =REG_CHIPTOP_0x06
	LDR		r11, [r12]
	AND		r11, r11, #0xFFFFFFDF
	STR		r11, [r12]

	@POP		{PC}
	MOV		PC, LR
