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


///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   drvEMAC.c
/// @author MStar Semiconductor Inc.
/// @brief  EMAC Tx Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------
// Include Files
//--------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsIRQ.h"
#include "drvGMAC.h"
#include "mhal_gmac.h"
#include "mii.h"
//#include <Types.h>

#include <network.h>
#include <dhcp.h>
//--------------------------------------------------------------------------------------------------
// Local Defines
//--------------------------------------------------------------------------------------------------
#define REMOVE_UPPER_LAYER_FUNCTIONS 1

#undef KSEG02KSEG1
#define KSEG02KSEG1(addr)  ((void *)((MS_U32)(addr)|0x20000000))
#undef KSEG12KSEG0
#define KSEG12KSEG0(addr)  ((void *)((MS_U32)(addr)&~0x20000000))
#define EMAC_DBG(fmt, args...)              {diag_printf("%04d %s " fmt, __LINE__, __FUNCTION__, ##args);}

//--------------------------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------------------------
#define EMAC_CTL_START    0x0000001C  //enable MPE/TE/RE
#define EMAC_CTL_STOP     0x00000010  //enable MPE
#define EMAC_CTRL_CURRENT ((EMAC_txrx_start&&EMAC_connection)?EMAC_CTL_START:EMAC_CTL_STOP)
//--------------------------------------------------------------------------------------------------
// External Debug Function
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------------------
extern int PHY_Source;
MS_BOOL (*PHY_ConnectStatus)(void) = NULL;

//--------------------------------------------------------------------------------------------------
// Local function declaration
//--------------------------------------------------------------------------------------------------
void SendPacket(MS_U32 xframeAddr,MS_U32 xframeLeng);
static void drvEMAC_send(void **xTxBuf);
void drvEMAC_recv(void *sg_list0,int sg_len);
static MS_U32 CheckFrameRX(MS_U32 xMethodChkFrameRX);
void drvEMAC_RxEvent(MS_U32 *FrameLeng);
static void drvEMAC_EnableInterrupt(void);
static MS_U8* drvEMAC_GetMacAddr(void);
MS_BOOL MDrv_EMAC_ConnectStatus(void);
static void EMAC_start(void);
static void EMAC_stop(void);

void PHY_Tx(MS_U32 buf, MS_U32 len) { SendPacket(buf, len); }
void PHY_TxBufGet(void **xTxBuf) { drvEMAC_send(xTxBuf); }
void PHY_Rx(void *sg_list,int sg_len) { drvEMAC_recv(sg_list, sg_len); }
MS_U32 PHY_RxCheck(MS_U32 xMethodChkFrameRX) { return CheckFrameRX(xMethodChkFrameRX); }
void PHY_RxLength(MS_U32 *FrameLeng) { drvEMAC_RxEvent(FrameLeng); }
void PHY_IntEnable(void) { drvEMAC_EnableInterrupt(); }
MS_U8* PHY_MacGet(void) { return drvEMAC_GetMacAddr(); }
void PHY_Start(void) { EMAC_start(); }
void PHY_Stop(void) { EMAC_stop(); }


//--------------------------------------------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------------------------------------------
#define EMAC_STACK_SIZE                     4096
static Task_Info _VerEMAC_Task[1] =
{
    { -1, E_TASK_PRI_HIGH, NULL, EMAC_STACK_SIZE, "EMAC Task 0" },        // Driver
};
static MS_S32 _s32EMAC_EventGroupID;
#define EVNT_EMAC_MAIN_THREAD               0x01
#define EVNT_EMAC_CHECK_TRANSMIT            0x02


static MS_S32 _s32EMAC_Register_Mutex = -1;
static MS_U8 EMAC_txrx_start=0;
static MS_U8 EMAC_connection=0;

#define _EMAC_Register_Lock()   MsOS_ObtainMutex(_s32EMAC_Register_Mutex, MSOS_WAIT_FOREVER);
#define _EMAC_Register_Unlock() MsOS_ReleaseMutex(_s32EMAC_Register_Mutex);

// EMAC callback function used in interrupt context:


void MDrv_EMAC_RegisterCallBack(InterruptCb pCallbackFn);

#define EMAC_DELAY_10()                     MsOS_DelayTask(10);
#define EMAC_DELAY_2000()                   MsOS_DelayTask(2000);
//#define EMAC_DELAY_10()                     { volatile int i; for (i = 0; i < 540000; i++); };
//#define EMAC_DELAY_2000()                   { volatile int i; for (i = 0; i < 108000000; i++); };
MS_U8 phyaddr = 0;

struct recv_desc_bufs *dlist;

//--------------------------------------------------------------------------------------------------
// Local Function Prototypes
//--------------------------------------------------------------------------------------------------


#if SW_DESC_SUPPORT
//#define EMAC_ABSO_MEM_SIZE                  ((32 * 1024)+SW_DESC_RX_BUFFER_SIZE)// More than: (32 + 16(0x3FFF)) KB
#define _RBQP_BASE                          RX_BUFFER_SIZE
#define _TX_BUFFER_BASE                     (_RBQP_BASE     +0x00002000)
#define _RX_FRAME_ADDR                      (_TX_BUFFER_BASE+0x00000800)
#else//SW_DESC_SUPPORT
//#define EMAC_ABSO_MEM_SIZE                  (48 * 1024)     // More than: (32 + 16(0x3FFF)) KB
#define _RBQP_BASE                          0x00004000
#define _TX_BUFFER_BASE                     0x00006000
#define _RX_FRAME_ADDR                      0x00006800
#endif//SW_DESC_SUPPORT

// ATTENTION: PLEASE_CHECK_IT_BEFORE_TEST:
// 1. It's better to set the lowest 13 bits of &RBQP_BASE as 0 to allow max 1024 descriptors.
//    Original: 000123'00' ==> 8 bits - 2 bits = 6 bits ==> 2^6 = Max 64 descriptors

// NOTE: --------------------------------------------------------------------
// 1. Check RX frames:
//    (1) 0x00000020 : ETH_RSR(Receive Status Register  ) : DNA (0x00000001)
//    (2) 0x00000024 : ETH_ISR(Interrupt Status Register) : RCOM (0x00000002)
//    (3) Polling RX Descriptor List
// 2. IMPORTANT ASSUMPTION:
//    (1) My MAC Addr = 00-30-1B-BA-02-DB
//        PC MAC Addr = 00-16-D3-38-B0-77

// Initialization ===========================================================
// Definition ---------------------------------------------------------------
#define EMAC_ABSO_MEM_BASE                  0xA0000000
// URANUS here:
#if (!USE_TASK) // MEM_BASE_ADJUSTMENT ......................................
#define RAM_ADDR_BASE                         0xA0000000
//#define REG_ADDR_BASE                        _REG_ADDR_BASE  // The register address base. Depends on system define.
#define RX_BUFFER_BASE                        0x00000000         // ==0xA0000000 ~~ 0xA0004000 (Max: 16 KB)
#define RBQP_BASE                            _RBQP_BASE         // ==0xA0004000 ~~ 0xA0005FFF for MAX 1024 descriptors
#define TX_BUFFER_BASE                       _TX_BUFFER_BASE         // ==0xA0006000 ~~ ????????
#define RX_FRAME_ADDR                        _RX_FRAME_ADDR         // Software COPY&STORE one RX frame. Size is not defined.
//#define RX_BUFFER_SEL                        0x0003            // 0x0=2KB,0x1=4KB,0x2=8KB,0x3=16KB
#else // The memory allocation for TASK.
//#define REG_ADDR_BASE                        _REG_ADDR_BASE  // The register address base. Depends on system define.
//#define RX_BUFFER_SEL                        0x0003            // 0x0=2KB,0x1=4KB,0x2=8KB,0x3=16KB
MS_U32     alloRAM_ADDR_BASE                 = 0x00000000;
MS_U32     RAM_ADDR_BASE                     = 0x00000000;     // After init, RAM_ADDR_BASE = EMAC_ABSO_MEM_BASE
MS_U32     RX_BUFFER_BASE                    = 0x00000000;     // IMPORTANT: lowest 14 bits as zero.
MS_U32     RBQP_BASE                         = _RBQP_BASE;     // IMPORTANT: lowest 13 bits as zero.
MS_U32     TX_BUFFER_BASE                    = _TX_BUFFER_BASE;
MS_U32     RX_FRAME_ADDR                     = _RX_FRAME_ADDR;
#endif //^MEM_BASE_ADJUSTMENT ...............................................

#if SW_DESC_SUPPORT
//#define RX_BUFFER_SIZE                        SW_DESC_RX_BUFFER_SIZE
#else
//#define RX_BUFFER_SIZE                        (0x00000800<<RX_BUFFER_SEL)
#endif

//#define RBQP_LENG                               0x0100                  // ==?descriptors
//#define RBQP_SIZE                               (0x0008* RBQP_LENG )      // ==8bytes*?==?descriptors
//MS_U32  RBQP_LENG = RBQP_LENG;
//MS_U32  RBQP_SIZE;
#if 0 // obsolete ningyuan nov 22
#if (EMAC_CHIP_ID == EMAC_ON_URANUS)
#define E_IRQ_EMAC                          (E_IRQL_START+3)
#endif

#if (EMAC_CHIP_ID == EMAC_ON_TITANIA)
#define E_IRQ_EMAC                          (E_IRQL_START+9)
#endif

#if (EMAC_CHIP_ID == EMAC_ON_OBERON)
//***** need to update IRQ ******
#define E_IRQ_EMAC                          (E_IRQL_START+3)
#endif
#endif
// Global Variables ---------------------------------------------------------
BasicConfigEMAC ThisBCE;
UtilityVarsEMAC ThisUVE;
// MAC Address:
MS_U8    MY_MAC[6] = { 0x00, 0x30, 0x1B, 0xBA, 0x02, 0xDB };     // The MAC address for test with notebook
//MS_U8        MY_MAC[6] = { 0x08, 0x88, 0x12, 0x34, 0x56, 0x78 };     // The MAC address specified in configtool

static MS_U8 u8EMAC_StackBuffer[EMAC_STACK_SIZE];
static MS_U8 u8EMAC_Abso_Buffer[EMAC_ABSO_MEM_SIZE];
struct eth_drv_sgX {
    MS_U32                 buf;
    MS_U32                 len;
};

static MS_U8 u8PhyAddr = 0xFF;
#define EMAC_IER_FOR_INT_JULIAN_D   (0x0000E435) 
// RAM:Read/Write :

//------------------------------------------------------------------------------
/// @brief Write a word to the specific memory location.
/// @param  [in] xoffset offset of base address
/// @param  [in] xval value for the memory location
/// @return None
//------------------------------------------------------------------------------

void EMAC_WritRam32(MS_U32 xoffset,MS_U32 xval)
{
    *((volatile MS_U32*)((char*)RAM_ADDR_BASE + xoffset)) = xval;
}

//------------------------------------------------------------------------------
/// @brief Write 2 bytes  to the specific memory location.
/// @param  [in] xoffset offset of base address
/// @param  [in] xval value for the memory location
/// @return None
//------------------------------------------------------------------------------
void EMAC_WritRam16(MS_U32 xoffset,MS_U16 xval)
{
    *((volatile MS_U16*)((char*)RAM_ADDR_BASE + xoffset)) = xval;
}

//------------------------------------------------------------------------------
/// @brief Write 1 byte  to the specific memory location.
/// @param  [in] xoffset offset of base address
/// @param  [in] xval value for the memory location
/// @return None
//------------------------------------------------------------------------------
void EMAC_WritRam08(MS_U32 xoffset,MS_U8 xval)
{
    *((volatile MS_U8*)((char*)RAM_ADDR_BASE + xoffset)) = xval;
}

//------------------------------------------------------------------------------
/// @brief Read 4 bytes from the specific memory location.
/// @param  [in] xoffset offset of base address
/// @return [out] MS_U32 4 bytes value
//------------------------------------------------------------------------------
MS_U32 EMAC_ReadRam32(MS_U32 xoffset)
{
return( *((volatile MS_U32*)((char*)RAM_ADDR_BASE + xoffset)) );
}

//------------------------------------------------------------------------------
/// @brief Read 2 bytes from the specific memory location.
/// @param  [in] xoffset offset of base address
/// @return [out] MS_U16 2 bytes value
//------------------------------------------------------------------------------
MS_U16 EMAC_ReadRam16(MS_U32 xoffset)
{
return( *((volatile MS_U16*)((char*)RAM_ADDR_BASE + xoffset)) );
}

//------------------------------------------------------------------------------
/// @brief Read 1 byte from the specific memory location.
/// @param  [in] xoffset offset of base address
/// @return [out] MS_U8 1 byte value
//------------------------------------------------------------------------------
MS_U8 EMAC_ReadRam08(MS_U32 xoffset)
{
return( *((volatile MS_U8*)((char*)RAM_ADDR_BASE + xoffset)) );
}
// REG:Read/Write :

//------------------------------------------------------------------------------
/// @brief write 4 bytes to a register location.
/// @param  [in] xoffset offset of base address
/// @param  [in] xval value for the memory location
/// @return None
//------------------------------------------------------------------------------
void EMAC_WritReg32_raw(MS_U32 xoffset,MS_U32 xval)
{
#if 0 //(EMAC_CHIP_ID == EMAC_ON_URANUS)
    *((MS_U32*)((char*)REG_ADDR_BASE + xoffset)) = xval;
#else
#if 0
    if((xoffset>=0x100)&&(xoffset<0x200))
    {
      *((volatile MS_U32*)((char*)(REG_ADDR_BASE) + xoffset+0x100)) = xval;
    }
    else
#endif
    {
       *((volatile MS_U32*)((char*)(REG_ADDR_BASE) + xoffset*2)) = xval&0x0000FFFF;
       *((volatile MS_U32*)((char*)(REG_ADDR_BASE) + xoffset*2 + 4)) = xval>>0x10;
    }
#endif
}

//---------------------------------
void EMAC_WritReg32(MS_U32 xoffset,MS_U32 xval)
{
#if 0 //(EMAC_CHIP_ID == EMAC_ON_URANUS)
    *((MS_U32*)((char*)REG_ADDR_BASE + xoffset)) = xval;
#else
    _EMAC_Register_Lock();
    EMAC_WritReg32_raw(xoffset,xval);
    _EMAC_Register_Unlock();
#endif
}
//------------------------------------------------------------------------------
/// @brief Read 4 bytes from a register location.
/// @param  [in] xoffset offset of base address
/// @return [out] MS_U32 4 bytes value from that register
//------------------------------------------------------------------------------
MS_U32 EMAC_ReadReg32_raw(MS_U32 xoffset)
{
#if 0 //(EMAC_CHIP_ID == EMAC_ON_URANUS)
    return( *((MS_U32*)((char*)REG_ADDR_BASE + xoffset)) );
#else
#if 0
    if((xoffset>=0x100)&&(xoffset<0x200))
    {
        MS_U32 xoffsetValueL = *((volatile MS_U32*)((char*)(REG_ADDR_BASE) + xoffset+0x100));
        return(xoffsetValueL);
    }
    else
#endif
    {
        MS_U32 xoffsetValueL = ( *((volatile MS_U32*)((char*)(REG_ADDR_BASE) + xoffset*2)))&0x0000FFFF;
        MS_U32 xoffsetValueH = ( *((volatile MS_U32*)((char*)(REG_ADDR_BASE) + xoffset *2+ 4)))<<0x10;
        return( xoffsetValueH|xoffsetValueL);
    }
#endif
}
//---------------------------------
MS_U32 EMAC_ReadReg32(MS_U32 xoffset)
{
#if 0 // (EMAC_CHIP_ID == EMAC_ON_URANUS)
    return( *((MS_U32*)((char*)REG_ADDR_BASE + xoffset)) );
#else
    MS_U32 xval;
    _EMAC_Register_Lock();
    xval=EMAC_ReadReg32_raw(xoffset);
    _EMAC_Register_Unlock();
    return xval;
#endif
}

#if 0
void EMAC_Sync(void)
{
#define EMAC_SYNC_SWITCH                    1
    if(EMAC_SYNC_SWITCH) {
        asm volatile (
               "sync;"
        );
    } // if
}
#else
void MAsm_CPU_Sync(void);
#define EMAC_Sync MAsm_CPU_Sync
#endif

MS_U32 g_u32PhyStatus=0;
#include "ver_gmac.inc"
//==============================================================================


// Interrupt Handler Here ===================================================
// ISR Here -----------------------------------------------------------------

void MDrv_EMAC_DumpMem(MS_U32 addr, MS_U32 len)
{
    MS_U8 *ptr = (MS_U8 *)addr;
    MS_U32 i;

    printf("\n ===== Dump %lx =====\n", (long unsigned int)ptr);
    for (i=0; i<len; i++)
    {
        if ((MS_U32)i%0x10UL ==0)
            printf("%lx: ", (long unsigned int)ptr);
        if (*ptr < 0x10UL)
            printf("0%x ", *ptr);
        else
            printf("%x ", *ptr);
        if ((MS_U32)i%0x10UL == 0x0fUL)
            printf("\n");
	ptr++;
    }
    printf("\n");
}

//------------------------------------------------------------------------------
/// @brief ISR for HRESP
/// @return [out] MS_U32 always 0
//------------------------------------------------------------------------------
MS_U32 Isr_INT_HRESP(void)
{
    ThisUVE.cntHRESP++;
return(0);
}

//------------------------------------------------------------------------------
/// @brief ISR for Receive buffer overrun
/// @return [out] MS_U32 always 0
//------------------------------------------------------------------------------
MS_U32 Isr_INT_ROVR(void)
{
    ThisUVE.cntROVR++;
return(0);
}

//------------------------------------------------------------------------------
/// @brief ISR for LINK
/// @return [out] MS_U32 always 0
//------------------------------------------------------------------------------
MS_U32 Isr_INT_LINK(void)
{
    ThisUVE.cntLINK++;
return(0);
}

//------------------------------------------------------------------------------
/// @brief ISR for Tx idle
/// @return [out] MS_U32 always 0
//------------------------------------------------------------------------------
MS_U32 Isr_INT_TIDLE(void)
{
    ThisUVE.cntTIDLE++;
return(0);
}

//------------------------------------------------------------------------------
/// @brief ISR for Tx complete
/// @return [out] MS_U32 always 0
//------------------------------------------------------------------------------
MS_U32 Isr_INT_TCOM(void)
{
    ThisUVE.flagMacTxPermit = 1;
    ThisUVE.cntTCOM++;
    /* matt.lin -- We don't need to wake up this since nobody is waiting. */
    //MsOS_SetEvent(_s32EMAC_EventGroupID, EVNT_EMAC_CHECK_TRANSMIT);

return(0);
}

//------------------------------------------------------------------------------
/// @brief ISR for TBRE
/// @return [out] MS_U32 always 0
//------------------------------------------------------------------------------
MS_U32 Isr_INT_TBRE(void)
{
    ThisUVE.cntTBRE++;
return(0);
}

//------------------------------------------------------------------------------
/// @brief ISR for Rx try
/// @return [out] MS_U32 always 0
//------------------------------------------------------------------------------
MS_U32 Isr_INT_RTRY(void)
{
    ThisUVE.cntRTRY++;
return(0);
}

//------------------------------------------------------------------------------
/// @brief ISR for Tx buffer under run
/// @return [out] MS_U32 always 0
//------------------------------------------------------------------------------
MS_U32 Isr_INT_TUND(void)
{
    ThisUVE.cntTUND++;
return(0);
}

//------------------------------------------------------------------------------
/// @brief ISR for Tx buffer overrun
/// @return [out] MS_U32 always 0
//------------------------------------------------------------------------------
MS_U32 Isr_INT_TOVR(void)
{
    ThisUVE.cntTOVR++;
    EMAC_WritReg32_raw(REG_ETH_TSR,0x00000001);        // Clear OVR bit by writing '1'
return(0);
}

//------------------------------------------------------------------------------
/// @brief ISR for Rx buffer not available
/// @return [out] MS_U32 always 0
//------------------------------------------------------------------------------
MS_U32 Isr_INT_RBNA(void)
{
    ThisUVE.flagISR_INT_RBNA = 0x01;
    ThisUVE.cntRBNA++;
return(0);
}

//------------------------------------------------------------------------------
/// @brief ISR for Rx complete
/// @return [out] MS_U32 always 0
//------------------------------------------------------------------------------
MS_U32 Isr_INT_RCOM(void)
{
    ThisUVE.flagISR_INT_RCOM = 0x01;
    ThisUVE.cntRCOM++;
    // Set event to wake up MAIN_THREAD here.
    // (20071029_CHARLES)   MsOS_SetEvent(_s32EMAC_EventGroupID, EVNT_EMAC_MAIN_THREAD);

return(0);
}

//------------------------------------------------------------------------------
/// @brief ISR for DONE
/// @return [out] MS_U32 always 0
//------------------------------------------------------------------------------
MS_U32 Isr_INT_DONE(void)
{
    ThisUVE.flagISR_INT_DONE = 1;
    ThisUVE.cntDONE++;
return(0);
}

//------------------------------------------------------------------------------
/// @brief EMAC ISR
/// @return None
//------------------------------------------------------------------------------
static void _DrvEMAC_ISR(InterruptNum eIntNum)
{

    //EMAC_DBG("%s->%d\n",__FUNCTION__,__LINE__);
    MS_U32 txrxWord   = 0x00000000;
//    MS_U32 retETH_ISR = EMAC_ReadReg32_raw(REG_ETH_ISR);
    MS_U32 retETH_ISR = MHal_EMAC_Read_ISR() & ~MHal_EMAC_Read_IMR();// IMPORTANT: ISR read to clear
    //EMAC_DBG("MHal_EMAC_Read_ISR(): 0x%x\n", MHal_EMAC_Read_ISR());
    //EMAC_DBG("MHal_EMAC_Read_IMR(): 0x%x\n", MHal_EMAC_Read_IMR());
//    EMAC_DBG("emac_isr = %08x\n", retETH_ISR);

#if 1
//Jesse add for OS lost emac_dsr, cause ftp fail
    //check_rx
    MS_U32 rx_len=0;
//    MS_U32 retETH_TCR = EMAC_ReadReg32_raw(REG_ETH_TCR);        // IMPORTANT: ISR read to clear
    MS_U32 retETH_TCR = MHal_EMAC_Read_TCR();// IMPORTANT: ISR read to clear
    if((retETH_TCR & 0x7ff) == 0) txrxWord |= 0x00000001;
    //check tx
    drvEMAC_RxEvent(&rx_len);
    if(rx_len) txrxWord |= 0x00000002;

#endif


#if DELAY_INTR
    //Read rx num
    MS_U32 delay_intr_status = EMAC_ReadReg32_raw(DELAY_INTR_STATUS_REG);
    if(delay_intr_status&0x8000)
    {
        MS_U8 i;
        MS_U8 num = delay_intr_status&0x00ff;
        for(i=0;i<num;i++)
        {
            Isr_INT_RCOM();
#if VER_EMAC_ENABLE
#else
            emac_dsr(0x00000002);
#endif
        }
    }
#endif //DELAY_INTR

#if MIU_WRITE_PROTECT
    if(EMAC_ReadReg32_raw(EMAC_JULIAN_0104)&0x10)
    {
#if VER_EMAC_MIU_WRITE_PROTECT
        ver_emac_miu_write_protect();
#endif
        EMAC_DBG("EMAC: FATAL ERROR MIU Write protect violation\n");
        MS_ASSERT(0);
    }
#endif//MIU_WRITE_PROTECT

    if(retETH_ISR & EMAC_INT_HRESP) {
        Isr_INT_HRESP();
        EMAC_DBG("%s->%d\n",__FUNCTION__,__LINE__);
    } // if
    if(retETH_ISR & EMAC_INT_ROVR) {
        Isr_INT_ROVR();
        //write 1 clear
        MHal_EMAC_Write_RSR(EMAC_RSROVR);
        EMAC_DBG("%s->%d\n",__FUNCTION__,__LINE__);
    } // if
    if(retETH_ISR & EMAC_INT_LINK) {
        Isr_INT_LINK();
        EMAC_DBG("%s->%d\n",__FUNCTION__,__LINE__);
    } // if
    if(retETH_ISR & EMAC_INT_TIDLE) {
        Isr_INT_TIDLE();
        EMAC_DBG("%s->%d\n",__FUNCTION__,__LINE__);
    } // if
    if(retETH_ISR & EMAC_INT_TCOM) {
        Isr_INT_TCOM();
        txrxWord |= 0x00000001;
        EMAC_DBG("%s->%d\n",__FUNCTION__,__LINE__);
    } // if
    if(retETH_ISR & EMAC_INT_TBRE) {
        Isr_INT_TBRE();
        EMAC_DBG("%s->%d\n",__FUNCTION__,__LINE__);
    } // if
    if(retETH_ISR & EMAC_INT_RTRY) {
        Isr_INT_RTRY();
        EMAC_DBG("%s->%d\n",__FUNCTION__,__LINE__);
    } // if
    if(retETH_ISR & EMAC_INT_TUND) {
        Isr_INT_TUND();
        EMAC_DBG("%s->%d\n",__FUNCTION__,__LINE__);
    } // if
    if(retETH_ISR & EMAC_INT_TOVR) {
        Isr_INT_TOVR();
        EMAC_DBG("%s->%d\n",__FUNCTION__,__LINE__);
    } // if
    if(retETH_ISR & EMAC_INT_RBNA) {
        Isr_INT_RBNA();
        EMAC_DBG("%s->%d\n",__FUNCTION__,__LINE__);
        MHal_EMAC_Write_RSR(EMAC_BNA);
    } // if
    if(retETH_ISR & EMAC_INT_RCOM) {

        // (20070412) IMPORTANT: It has problem if only read one frame by one RCOM interrupt.
        //EMAC_DBG("%s->%d\n",__FUNCTION__,__LINE__);
        Isr_INT_RCOM();
        txrxWord |= 0x00000002;
    } // if
    if(retETH_ISR & EMAC_INT_DONE) {
        Isr_INT_DONE();
        //EMAC_DBG("%s->%d\n",__FUNCTION__,__LINE__);
    } // if

    /* matt.lin(20071009) -- Do not handle interrupt directly. Take advantage of Network alarm support. */
    //if(txrxWord) emac_deliver(txrxWord);
#if VER_EMAC_ENABLE
    ver_retETH_ISR |= retETH_ISR;
    MsOS_EnableInterrupt((InterruptNum)E_INT_IRQ_SECEMAC);
#else
    if(txrxWord)
    {
        emac_dsr(txrxWord);
        //EMAC_DBG("%08x\n", txrxWord);
    }
    /* matt.lin(20071009) -- Interrupt should be turned on from pcnet driver once polling is finished. */
    else
    {

    }
    #endif
    if(retETH_ISR) MsOS_EnableInterrupt((InterruptNum)E_INT_IRQ_SECEMAC);
}

// --------------------------------------------------------------------------
// Register a callback function to process the interrupt of EMAC
// --------------------------------------------------------------------------
void MDrv_EMAC_RegisterCallBack(InterruptCb pCallbackFn)
{
    MsOS_DisableInterrupt((InterruptNum)E_INT_IRQ_SECEMAC);

    if(pCallbackFn != NULL) 
    {
        // Attach EMAC ISR to the EMAC INT
        MsOS_AttachInterrupt((InterruptNum)E_INT_IRQ_SECEMAC, pCallbackFn);
        MsOS_EnableInterrupt((InterruptNum)E_INT_IRQ_SECEMAC);
    } // if
}

//^Interrupt Handler Here ===================================================

void WritMAC_Address(MS_U32 addr,MS_U8 m0,MS_U8 m1,MS_U8 m2,MS_U8 m3,MS_U8 m4,MS_U8 m5)
{
    MS_U32 w0 = (MS_U32)m3 << 24 | m2 << 16 | m1 << 8 | m0;
    MS_U32 w1 = (MS_U32)m5 <<  8 | m4;
    EMAC_WritReg32(addr    ,w0);
    EMAC_WritReg32(addr + 4,w1);
}

void ReadMAC_Address(MS_U32 addr)
{
    MS_U32 w0 = EMAC_ReadReg32(addr    );
    MS_U32 w1 = EMAC_ReadReg32(addr + 4);

    MY_MAC[0]=(w0 & 0xFF);
    MY_MAC[1]=(w0 >> 8) & 0xFF;
    MY_MAC[2]=(w0 >> 16)& 0xFF;
    MY_MAC[3]=(w0 >> 24)& 0xFF;
    MY_MAC[4]=(w1 & 0xFF);
    MY_MAC[5]=(w1 >> 8) & 0xFF;

    if(!(w0|w1))
    printf("\033[1;33m""\ndrvEMC:Can't get MAC from EMAC register!!!\ndrvEMC:Plz Setting MAC Address on MBOOT FIRST!!!\n(setenv ethaddr XX:XX:XX:XX:XX:XX;saveenv)\n""\033[m");
    else
    printf("\033[1;33m""\ndrvEMC:MAC>>>>%02x:%02x:%02x:%02x:%02x:%02x\n""\033[m",MY_MAC[0],MY_MAC[1],MY_MAC[2],MY_MAC[3],MY_MAC[4],MY_MAC[5]);

}

//------------------------------------------------------------------------------
/// @brief Configure MAC address to CAM memory
/// @param  [in] addr memory address
/// @param  [in] m0 the sixth byte of MAC address (LSB)
/// @param  [in] m1 the fifth byte of MAC address
/// @param  [in] m2 the fourth byte of MAC address
/// @param  [in] m3 the third byte of MAC address
/// @param  [in] m4 the second byte of MAC address
/// @param  [in] m5 the first byte of MAC address (MSB)
/// @return None
//------------------------------------------------------------------------------

void WritCAM_Address(MS_U32 addr,MS_U8 m0,MS_U8 m1,MS_U8 m2,MS_U8 m3,MS_U8 m4,MS_U8 m5)
{
    MS_U32 w0 = (MS_U32)m1 << 24 | m0 << 16;
    MS_U32 w1 = (MS_U32)m5 << 24 | m4 << 16 | m3 << 8 | m2;
    EMAC_WritReg32(addr    ,w0);
    EMAC_WritReg32(addr + 4,w1);
}

//------------------------------------------------------------------------------
/// @brief Read MAC address from CAM memory
/// @param  [in] addr memory address
/// @param  [in] w0 the first word of MAC address
/// @param  [in] w1 the rest bytes of MAC address
/// @return None
//------------------------------------------------------------------------------
void ReadCAM_Address(MS_U32 addr,MS_U32 *w0,MS_U32 *w1)
{
    *w0 = EMAC_ReadReg32(addr    );
    *w1 = EMAC_ReadReg32(addr + 4);
}

//------------------------------------------------------------------------------
/// @brief Specify the default setting for EMAC
/// @return None
//------------------------------------------------------------------------------
void DefaultCfgEMAC(void)
{
    memset(&ThisBCE, 0x00, sizeof(BasicConfigEMAC));
    memset(&ThisUVE, 0x00, sizeof(UtilityVarsEMAC));
    ThisBCE.speed     = EMAC_SPEED_100;        // 100Mbps
    ThisBCE.wes       = 0;                    // 0:Disable, 1:Enable (WR_ENABLE_STATISTICS_REGS)
    ThisBCE.duplex    = 2;                    // 1:Half-duplex, 2:Full-duplex
    ThisBCE.cam       = 0;                    // 0:No CAM, 1:Yes
    ThisBCE.rcv_bcast = 0;                  // 0:No, 1:Yes
    ThisBCE.rlf       = 0;                    // 0:No, 1:Yes receive long frame(1522)
}

//------------------------------------------------------------------------------
/// @brief Specify the default setting for EMAC
/// @return 0:No, 1:Yes
//------------------------------------------------------------------------------
int CheckEMAC_Initialized(void)
{
return(ThisUVE.initedEMAC);
}

MS_U8 EMAC_PhyAddrScan(void)
{
    MS_U8 addr;
    MS_U32 value;

    EMAC_WritReg32(REG_ETH_CTL,EMAC_CTRL_CURRENT);

    for(addr=0;addr<32;addr++)
    {
        MHal_EMAC_read_phy(addr, MII_BMSR, &value);
        EMAC_DBG("phy [%d]=%x\n",addr, value);

        if((0xffff != value) && (0x0000 != value))
        {
            return addr;
        }
    }
    return 0xff;
}
//------------------------------------------------------------------------------
/// @brief Check LAN cable connection
/// @return 0:Disconnected, 1:Connected
//------------------------------------------------------------------------------
MS_U32 EMAC_CableConnection(void)
{
    MS_U32 u32Ret      = 0;
    MS_U32 u32EthMan  = 0x00000000;

    EMAC_WritReg32(REG_ETH_CTL, EMAC_CTRL_CURRENT);

    MHal_EMAC_read_phy(u8PhyAddr, PHY_REG_STATUS, &u32EthMan);

    if(g_u32PhyStatus != u32EthMan)
    {
        EMAC_DBG("Link change %x\n",u32EthMan);//jc_db
        g_u32PhyStatus = u32EthMan;
    }

    if(u32EthMan & 0x00000004)
    {
        u32Ret = 1;
        EMAC_connection=1;
    }
    else
    {
        u32Ret = 0;
        EMAC_connection=0;
    } // else

    return(u32Ret);
}

void EMAC_ChangeSpeed(MS_U32 speed)
{
    MS_U32 phy_an_adv;
    EMAC_WritReg32(REG_ETH_CTL,EMAC_CTRL_CURRENT);

    /* Jesse.Huang -- Change PHY seepd*/
    //step 1: read advertisement
    MHal_EMAC_read_phy(u8PhyAddr, PHY_REG_AN_ADV, &phy_an_adv);

    phy_an_adv = phy_an_adv & 0xFE1F;
    switch(speed)
    {
        case EMAC_SPEED_100|EMAC_DUPLEX_FULL:
                phy_an_adv|=0x1E0;
                break;
        case EMAC_SPEED_100|EMAC_DUPLEX_HALF:
                phy_an_adv|=0xE0;
                break;
        case EMAC_SPEED_10|EMAC_DUPLEX_FULL:
                phy_an_adv|=0x60;
                break;
        case EMAC_SPEED_10|EMAC_DUPLEX_HALF:
                phy_an_adv|=0x20;
                break;
        default:
                break;
    }

    //step 3: write advertisement back
    MHal_EMAC_write_phy(u8PhyAddr, PHY_REG_AN_ADV, phy_an_adv);

    //step 4: re-start AN
    MHal_EMAC_write_phy(u8PhyAddr, PHY_REG_CONFIG, 0x1200);
}

void EMAC_CheckDuplex(void)
{
    MS_U32 i;
//    MS_U32 word_PHY0     = 0x00000000;
    MS_U32 word_ETH_CFG = 0x00000000;
    MS_U32 status_monitor;
    EMAC_WritReg32(REG_ETH_CTL,EMAC_CTRL_CURRENT);
    MS_U16 SpeedMask[4]={0x100,0x80,0x40,0x20};
    ThisBCE.duplex = 1;   // Set default as Half-duplex if negotiation fails.
    /* Jesse.Huang -- Change PHY address for IP100A */
    MHal_EMAC_read_phy(u8PhyAddr, MII_LPA, &status_monitor);

    EMAC_DBG("EMAC Link Status(%08x): ",status_monitor);

    for(i=0;i<4;i++)
    {
        if(status_monitor & SpeedMask[i])break;
    }

    switch(i)
    {
        case 0:
            EMAC_DBG("100M Full\n");
            ThisBCE.speed=100;
            ThisBCE.duplex = 2;
        break;
        case 1:
            EMAC_DBG("100M Half\n");
            ThisBCE.speed=100;
            break;
        case 2:
            EMAC_DBG("10M Full\n");
            ThisBCE.speed=10;
            ThisBCE.duplex = 2;
            break;
        case 3:
            EMAC_DBG("10M Half\n");
            ThisBCE.speed=10;
            break;
        default:

            MHal_EMAC_read_phy(u8PhyAddr, MII_BMCR, &status_monitor);

            if(status_monitor & BMCR_FULLDPLX)
            {
                ThisBCE.duplex = 2;
            }
            else
            {
                ThisBCE.duplex = 1;
            }

            if(status_monitor & BMCR_SPEED100)
            {
                ThisBCE.speed = 100;
            }
            else
            {
                ThisBCE.speed = 10;
            }

            EMAC_DBG("%s %s\n", (ThisBCE.speed == 100 ? "100" : "10"), (ThisBCE.duplex == 2 ? "FD" : "HD"));
        break;
    }

    // Set REG_ETH_CFG:
    //word_ETH_CFG = EMAC_ReadReg32(REG_ETH_CFG);
    word_ETH_CFG = MHal_EMAC_Read_CFG() & ~( EMAC_SPD | EMAC_FD);
    if(ThisBCE.duplex == 2) word_ETH_CFG |= EMAC_FD;
    if(ThisBCE.speed == 100) word_ETH_CFG |= EMAC_SPD;
    MHal_EMAC_Write_CFG(word_ETH_CFG);
}

//------------------------------------------------------------------------------
/// @brief Negotiation between EMAC and peer
/// @return 0:Fail, 1:Success
//------------------------------------------------------------------------------
#define PHY_LOOP_TIMES 1000
MS_U32 NegotiationPHY(void)
{
    // Set PHY --------------------------------------------------------------
    MS_U32 i;
    MS_U32 word_PHY = 0x00000000;
    MS_U32 retValue = 0;
    // IMPORTANT: Get real duplex by negotiation with peer.
    EMAC_WritReg32(REG_ETH_CTL,EMAC_CTRL_CURRENT);

    u8PhyAddr = EMAC_PhyAddrScan();
    /* matt.lin -- Change PHY address for LAN8700 */
    MHal_EMAC_write_phy( u8PhyAddr, PHY_REG_CONFIG, 0x2100);

    /* matt.lin -- Change PHY address for LAN8700 */
    MHal_EMAC_write_phy( u8PhyAddr, PHY_REG_AN_ADV, 0x01E1);

    /* matt.lin -- Change PHY address for LAN8700 */
    MHal_EMAC_write_phy( u8PhyAddr, PHY_REG_CONFIG, 0x1200);

    // IMPORTANT: (20070906) There must be some delay (about 2 ~ 3 seconds) between writing 0x57821200 and 0x67FE0000.
    //                       Otherwise, the later EMAC_WritReg32(REG_ETH_MAN,word_PHY) has no effect.
    EMAC_DELAY_2000();
    ThisBCE.duplex = 1;   // Set default as Half-duplex if negotiation fails.

    if(EMAC_CableConnection() == 1) {
    /* matt.lin -- Change PHY address for LAN8700 */
    for(i=0;i<100;i++)    // (20071026_CHARLES) No check connection here, if no connect, also init here.
    {
            MHal_EMAC_read_phy( u8PhyAddr, PHY_REG_SPECIAL, &word_PHY);
            
        if(word_PHY & 0x00001000) {
            MS_U32 test1 = (word_PHY & 0x0000001C) >> 2;
                if((test1 == 0x001) || (test1 == 0x002)) {
                ThisBCE.duplex = 1;
                    //diag_printf("\nNegotiationPHY : half duplex");
            } else {
                ThisBCE.duplex = 2;
                    //diag_printf("\nNegotiationPHY : full duplex");
            } // else
                retValue = 1;
              //diag_printf("\nNegotiationPHY : point 5");
            break;
        } // if
        EMAC_DELAY_10();
        } // for
    } // if

    // NOTE: REG_ETH_CFG must be set according to new ThisBCE.duplex.
    // Set PHY --------------------------------------------------------------
return(retValue);
}
void EMAC_special_reg(MS_U32 addr, MS_U32 value, MS_U32 mask)
{
    MS_U32 old_value = *((volatile MS_U32*)((char*)addr));
    MS_U32 new_value = old_value & (~mask);
    value = value & (mask);
    *((volatile MS_U32*)((char*)addr)) = new_value | value;
    printf("set addr[%08x or %08x]=from %08x to %08x\n",addr,(addr-0xbf800000)/4,old_value,*((volatile MS_U32*)((char*)addr)));
}

void EMAC_MIU_write_protect(void)
{
#if MIU_WRITE_PROTECT
    MS_U32 upper, lower;
    upper=(((MS_U32)u8EMAC_Abso_Buffer+EMAC_ABSO_MEM_SIZE)&0x00ffffff)/8;
    lower=((MS_U32)u8EMAC_Abso_Buffer&0x00ffffff)/8;

#if VER_EMAC_MIU_WRITE_PROTECT
    upper=(((MS_U32)VER_EMAC_MIU_WRITE_PROTECT_UP_BASE+VER_EMAC_MIU_WRITE_PROTECT_UP_BASE_LEN)&0x00ffffff)/8;
    memset((void*)(VER_EMAC_MIU_WRITE_PROTECT_UP_BASE|0xa0000000),0x5a,VER_EMAC_MIU_WRITE_PROTECT_DUMP_SIZE);
#endif

    //Write protect Upper bound
    EMAC_WritReg32(REG_MIU_WRUPB_l,upper&0xffff);  //0x00080000
    EMAC_WritReg32(REG_MIU_WRUPB_h,upper>>16);  //0x00080000
    //Write protect Lower bound
    EMAC_WritReg32(REG_MIU_WRLPB_l,lower&0xffff);  //0x00040000/8
    EMAC_WritReg32(REG_MIU_WRLPB_h,lower>>16);  //0x00040000/8
//    EMAC_WritReg32(EMAC_JULIAN_0100,EMAC_ReadReg32(EMAC_JULIAN_0100)|0xC0);//enable write protect
    MHal_EMAC_Write_JULIAN_0100(MHal_EMAC_Read_JULIAN_0100() | 0xC0);
#endif
}

//------------------------------------------------------------------------------
/// @brief Initialization function for EMAC
/// @return None
//------------------------------------------------------------------------------
void InitializeEMAC(void)
{
#if SW_DESC_SUPPORT
    MS_U32 u32EMAC_JULIAN_0108=0;
    MS_U32 u32EMAC_JULIAN_0104 = JULIAN_104_VAL;
    MS_U16 i;
    MS_U32 rx_buf_addr;
#endif
    MS_U32 uJulian104Value = 0;
    MS_U32 word_ETH_CTL = 0x00000000;
    MS_U32 word_ETH_CFG = 0x00000000;
    MS_U32 word_ETH_IMR = 0x00000000;
//    RBQP_SIZE = _RBQP_SIZE;
    EMAC_txrx_start=0;
    MHal_EMAC_Power_On_Clk();
    MHal_EMAC_Write_JULIAN_0100(JULIAN_100_VAL); // RMII RMII2.1

    // (20071026_CHARLES) Disable TX, RX and MDIO:   (If RX still enabled, the RX buffer will be overwrited)
    word_ETH_CTL = 0x00000000;
//    EMAC_WritReg32(REG_ETH_CTL,EMAC_CTRL_CURRENT);
//    MHal_EMAC_Write_CTL(EMAC_CTRL_CURRENT);
    // Enable Receive and Transmit //
    word_ETH_CTL = MHal_EMAC_Read_CTL();
    word_ETH_CTL |= (EMAC_RE | EMAC_TE);
    word_ETH_CTL = 0;
    MHal_EMAC_Write_CTL(word_ETH_CTL);

    // Init RX --------------------------------------------------------------
    memset((MS_U8*)RAM_ADDR_BASE + RX_BUFFER_BASE, 0x00, RX_BUFFER_SIZE);

#if SW_DESC_SUPPORT
//    RBQP_LENG = RX_BUFFER_SIZE/ONE_RX_BUF_SIZE;
//    u32EMAC_JULIAN_0108 |= SW_DESC_ENABLE;
    u32EMAC_JULIAN_0104 |= SW_DESC_ENABLE;
#else
    EMAC_WritReg32(REG_ETH_BUFF_CONF,RX_BUFFER_BASE|RX_BUFFER_SEL);    // Write RX data buffer base address and size=??kbyte
    EMAC_WritReg32(REG_ETH_BUFFRDPTR,0x00000000);                    // RxBuffRdPtr = 0000
    EMAC_WritReg32(REG_ETH_BUFFWRPTR,0x00000000);                    // RxBuffWrPtr = 0000   Write current RX data write pointer address
#endif
    // Initialize "Receive Buffer Queue Pointer"
    EMAC_WritReg32(REG_ETH_RBQP,RBQP_BASE);                            // Write RBQP_BASE
    EMAC_WritReg32(REG_ETH_BUFF_CONF,RX_BUFFER_BASE);    // Write RX data buffer base address and size=??kbyte

    // Initialize Receive Buffer Descriptors
    memset((MS_U8*)RAM_ADDR_BASE+RBQP_BASE,0x00,0x2000);        // Clear for max(8*1024)bytes (max:1024 descriptors)
#if SW_DESC_SUPPORT
    rx_buf_addr = RX_BUFFER_BASE;
    for(i=0;i<RBQP_LENG;i++)
    {
        rx_buf_addr=ALIGN_16(rx_buf_addr);
        dlist->descriptors[i].addr=rx_buf_addr|((i+1==RBQP_LENG)?2:0);
        rx_buf_addr+=ONE_RX_BUF_SIZE;
    }


    EMAC_WritReg32(EMAC_JULIAN_0108,u32EMAC_JULIAN_0108);        // Julian interrupt overall switch(INFORM_CPU). bit0=0 ==> enable
    MHal_EMAC_Write_JULIAN_0104(u32EMAC_JULIAN_0104);
#else
    EMAC_WritRam32(RBQP_BASE+RBQP_SIZE-0x08,0x00000002);             // (n-1) : Wrap = 1
#endif

#if RX_CHKSUM_SUPPORT
#ifndef u32EMAC_JULIAN_0108
    MS_U32 u32EMAC_JULIAN_0108=0;
#endif
    u32EMAC_JULIAN_0108 |= RX_CHKSUM_ENABLE;
    EMAC_WritReg32(EMAC_JULIAN_0108,u32EMAC_JULIAN_0108);        // Julian interrupt overall switch(INFORM_CPU). bit0=0 ==> enable
#endif

    // Enable Interrupts ----------------------------------------------------
#if DELAY_INTR
#ifndef u32EMAC_JULIAN_0108
    MS_U32 u32EMAC_JULIAN_0108=0;
#endif
    //Set rx_frame_cyc, rx_frame_num
    EMAC_WritReg32(DELAY_INTR_CTRL_REG,0xff10);//cyc=0x7f, rx num=0x10
    //Enable Delay Intr
    //u32EMAC_JULIAN_0108 = EMAC_ReadReg32(EMAC_JULIAN_0108) | DELAY_INTR_ENABLE;
    u32EMAC_JULIAN_0108 = MHal_EMAC_Read_JULIAN_0108() | DELAY_INTR_ENABLE; // TODO Check JULIAN 108

    EMAC_WritReg32(EMAC_JULIAN_0108,u32EMAC_JULIAN_0108);

    //Not enable rx complete bit
//    EMAC_WritReg32(REG_ETH_IER,0x0000FFFD);
    MHal_EMAC_Write_IER(EMAC_IER_FOR_INT_JULIAN_D | EMAC_INT_RCOM);

#else //DELAY_INTR
//    EMAC_WritReg32(REG_ETH_IER,0x0000FFFF);           // Enable all interrupts
    MHal_EMAC_Write_IER(EMAC_IER_FOR_INT_JULIAN_D | EMAC_INT_RCOM);
    //MHal_EMAC_Write_IDR(EMAC_INT_TCOM);
    //MHal_EMAC_Write_IDR(EMAC_INT_TIDLE);

#endif //DELAY_INTR

//    EMAC_WritReg32(EMAC_JULIAN_0104,0x00000000);        // Julian interrupt overall switch(INFORM_CPU). bit0=0 ==> enable
#ifdef INT_JULIAN_D
    //Reg_rx_frame_cyc[15:8] -0xFF range 1~255
    //Reg_rx_frame_num[7:0]  -0x05 receive frames per INT.
    //0x80 Enable interrupt delay mode.
    //register 0x104 receive counter need to modify smaller for ping
    //Modify bigger(need small than 8) for throughput
    uJulian104Value = JULIAN_104_VAL;
    MHal_EMAC_Write_JULIAN_0104(uJulian104Value);
#else
    // Enable Interrupts ----------------------------------------------------
    uJulian104Value = 0x00000000 | SOFTWARE_DESCRIPTOR_ENABLE;
    MHal_EMAC_Write_JULIAN_0104(uJulian104Value);
#endif
//    word_ETH_IMR = EMAC_ReadReg32(REG_ETH_IMR);
    word_ETH_IMR = MHal_EMAC_Read_IMR();

    EMAC_MIU_write_protect();

    // Set MAC address (clean emac's register)------------------------------------------------------
#if 1//We need MAC address from u-boot's emac setting, so we don't clean "REG_ETH_SA1L"
    ReadMAC_Address(REG_ETH_SA1L);
#else
    WritMAC_Address(REG_ETH_SA1L,ThisBCE.sa1[0],ThisBCE.sa1[1],ThisBCE.sa1[2],ThisBCE.sa1[3],ThisBCE.sa1[4],ThisBCE.sa1[5]);
#endif
    WritMAC_Address(REG_ETH_SA2L,ThisBCE.sa2[0],ThisBCE.sa2[1],ThisBCE.sa2[2],ThisBCE.sa2[3],ThisBCE.sa2[4],ThisBCE.sa2[5]);
    WritMAC_Address(REG_ETH_SA3L,ThisBCE.sa3[0],ThisBCE.sa3[1],ThisBCE.sa3[2],ThisBCE.sa3[3],ThisBCE.sa3[4],ThisBCE.sa3[5]);
    WritMAC_Address(REG_ETH_SA4L,ThisBCE.sa4[0],ThisBCE.sa4[1],ThisBCE.sa4[2],ThisBCE.sa4[3],ThisBCE.sa4[4],ThisBCE.sa4[5]);

    // IMPORTANT: Run NegotiationPHY() before writing REG_ETH_CFG.
    MHal_EMAC_enable_mdi();
    if(NegotiationPHY())
        ThisUVE.flagMacTxPermit = 0x01;

    // ETH_CFG Register -----------------------------------------------------
#if (EMAC_CHIP_ID == EMAC_ON_URANUS)
    word_ETH_CFG = 0x00000800;        // Init: CLK = 0x2
#else
    word_ETH_CFG = 0x00000801;        //JC_DB:100M
   //word_ETH_CFG = 0x00000800;        //JC_DB:10M
#endif

#if VER_EMAC_INTERNAL_LOOPBACK
    word_ETH_CFG = 0x00000802;
#endif
    // (20070808) IMPORTANT: REG_ETH_CFG:bit1(FD), 1:TX will halt running RX frame, 0:TX will not halt running RX frame.
    //                       If always set FD=0, no CRC error will occur. But throughput maybe need re-evaluate.
    // IMPORTANT: (20070809) NO_MANUAL_SET_DUPLEX : The real duplex is returned by "negotiation"
    if(ThisBCE.duplex    == 2) word_ETH_CFG |= 0x00000002;
    if(ThisBCE.cam       == 1) word_ETH_CFG |= 0x00000200;
    if(ThisBCE.rcv_bcast == 0) word_ETH_CFG |= 0x00000020;
    if(ThisBCE.rlf       == 1) word_ETH_CFG |= 0x00000100;
    EMAC_WritReg32(REG_ETH_CFG,word_ETH_CFG);

    // ETH_CTL Register -----------------------------------------------------
    word_ETH_CTL = EMAC_CTRL_CURRENT;

#if VER_EMAC_INTERNAL_LOOPBACK
    word_ETH_CTL |= 0x2;
#endif
    if(ThisBCE.wes == 1) word_ETH_CTL |= 0x00000080;
    // MsOS_DelayTask(10000);
    EMAC_WritReg32(REG_ETH_CTL,word_ETH_CTL);
    

    ThisUVE.flagPowerOn = 1;
#if REMOVE_UPPER_LAYER_FUNCTIONS
#else//REMOVE_UPPER_LAYER_FUNCTIONS
    /* matt.lin -- we init loopback interface & eth0 interface should be up already */
    // init_loopback_interface(0);
    // eth0_up = 1;
    /* end of matt.lin */
#endif//REMOVE_UPPER_LAYER_FUNCTIONS
    ThisUVE.initedEMAC  = 1;
    // wyc.dbg-150624
    printf("ThisUVE.initedEMAC  = 1\n");

#if VER_EMAC_CAM
    ver_emac_cam_test();
#endif
}
//^Initialization ===========================================================

// 20070503_TEST ............................................................
#define RX_CHECK_METHOD3_SYNC                0    // 0:No, 1:Yes    <== sync in read command is abnormal.
#define RX_CHECK_METHOD3_SUB                2    // 1:SUB1, 2:SUB2
MS_U32 rxCheckMethod2PollTimes = 0;            // The times of really reading REG.
MS_U32 rxCheckMethod2PollOKs   = 0;            // The get frame counter.
MS_U32 rxCheckMethod3PollTimes = 0;            // The times of really reading RAM.
MS_U32 rxCheckMethod3PollOKs   = 0;            // The get frame counter.
MS_U32 rxThroughputTestStart   = 0;            // The global flag. Turn on after 1st frame received.
MS_U32 flagRxInterruptRBNA     = 0;
MS_U32 cntRxInterruptRBNA      = 0;
// NOTE: Test 4 cases to observe rxCheckMethod3PollTimes :
// Case 1: SYNC=0, SUB=1 : noSYNC & circular POLL <== suppose most method3polltimes
// Case 2: SYNC=0, SUB=2 : noSYNC & once     POLL <== suppose most cntPollMethod3
// Case 3: SYNC=1, SUB=1 : SYNC   & circular POLL <== sync overhead
// Case 4: SYNC=1, SUB=2 : SYNC   & once     POLL <== sync overhead
//^20070503_TEST ............................................................
MS_U32 CheckFrameRX(MS_U32 xMethodChkFrameRX)
// Purpose: Polling to know whether frames come in.
// Return: 0: Non, 1: Yes
{
    MS_U32 checkMethod = xMethodChkFrameRX;            // NOTE: Set check method here.
    MS_U32 retRX = 0;
    EMAC_Sync();
    MsOS_FlushMemory();
    if((ISR_IMPLEMENTED == 1) && (checkMethod != 3))    // If ISR, only polling descriptor allown.
    {
        return(0);
    }
#if (ISR_IMPLEMENTED != 1)
    if(checkMethod == 1) {        // Check method: 1 : Check Receive Status Register
//        MS_U32 retETH_RSR     = EMAC_ReadReg32(REG_ETH_RSR);
        MS_U32 retETH_RSR     = MHal_EMAC_Read_RSR();
        MS_U32 retETH_RSR_DNA = retETH_RSR & 0x00000001;
        if(retETH_RSR_DNA == 0x00000001) {
            EMAC_WritReg32(REG_ETH_RSR,0x00000001);        // Clear DNA bit by writing '1'
            retRX = 0x00000001;
        } // if
    } // if
    if(checkMethod == 2) {        // Check method: 2 : Polling Interrupt Status Register
        // (20070418) NOTE: If enable interrupt, don't use method 2 here.
        // (20070907) Omit.
    } // if
#endif
#if (ISR_IMPLEMENTED != 1)
    if(checkMethod == 3) 
#endif
        {    // Check method: 3 : Polling the RX Descriptor List // <= ALWAYS THIS METHOD ningyuan.huang 110208
        MS_U32 idxRBQP = ThisUVE.readIdxRBQP;
        MS_U32 flagChkMethod3 = 1;
        while(flagChkMethod3 == 1)
        {
            if(RX_CHECK_METHOD3_SYNC == 1) {
                EMAC_Sync();
            } // if
            MS_U32 entry_W0 = dlist->descriptors[idxRBQP].addr;
//            EMAC_DBG("%08x\n", entry_W0);
            if(rxThroughputTestStart) rxCheckMethod3PollTimes++;        // Times to really read RAM.
            if((entry_W0 & 0x00000001) == 0x00000001) {        // If any Ownership bit is 1, frame received.
                retRX = 0x00000001;
                ThisUVE.readIdxRBQP = idxRBQP;
                flagChkMethod3 = 0;        // End the while loop
                if(rxThroughputTestStart) rxCheckMethod3PollOKs++;
            } else {
                // 20070806_HANDLE_EXCEPTION_CASE : Descriptor empty, but currRxBuffRdPtr and currRxBuffWrPtr different by 0x00004000
                if(ThisUVE.flagISR_INT_RBNA == 0x01) {
//                    MS_U32 currRxBuffRdPtr = EMAC_ReadReg32(REG_ETH_BUFFRDPTR);
//                    MS_U32 currRxBuffWrPtr = EMAC_ReadReg32(REG_ETH_BUFFWRPTR);
                    MS_U32 currRxBuffRdPtr = MHal_EMAC_Read_RDPTR();
                    MS_U32 currRxBuffWrPtr = MHal_EMAC_Read_WRPTR();

                    if((currRxBuffRdPtr & 0x00003FFF) == (currRxBuffWrPtr & 0x00003FFF)) {
                        ThisUVE.cntCase20070806++;
                        EMAC_WritReg32(REG_ETH_BUFFRDPTR,currRxBuffWrPtr);
                        ThisUVE.flagISR_INT_RBNA = 0x00;
                    } // if
                } // if
                //^20070806_HANDLE_EXCEPTION_CASE
                if(RX_CHECK_METHOD3_SUB == 1) {
                    // (20070426) if(1) --> if(0) OK after adding the "sync;" above.
                    // IMPORTANT: (20070413) @@@@@@ It wastes of almost one day debugging the "CheckFrameRX" fails issue.
                    //            The "(20070412) New thinking" below doesn't really work now. Need to figure out why ?
                    idxRBQP = idxRBQP + 1;
                    if(idxRBQP >= RBQP_LENG)    // Scan all the list finished
                        idxRBQP = 0;
                    if(idxRBQP == ThisUVE.readIdxRBQP)
                        flagChkMethod3 = 0;        // End the while loop
                } else {
                    // "(20070412) New thinking": return directly if current "Ownership=0" found.
                    //            Because RX:Write on the current descriptor afterward. --> Sequentially.
                    flagChkMethod3 = 0;            // End the while loop
                } // else
            } // else
        } // while
    } // if
//    EMAC_DBG("%d %d\n", xMethodChkFrameRX, retRX);
    return(retRX);
}

//------------------------------------------------------------------------------
/// @brief Read one frame from Rx buffer
/// @param  [in] xframeAddr the frame address
/// @param  [in] xframeLeng the frame length
/// @return [out] MS_U32 The address storing the RX_FRAME
//------------------------------------------------------------------------------

MS_U32 ReadONE_RX_Frame(MS_U32 xframeAddr,MS_U32 xframeLeng)
// Return: The address storing the RX_FRAME
{
    EMAC_DBG("\n");
    MS_U32 thisRX_FRAME_ADDR = RX_FRAME_ADDR;        // Default condition
    if((xframeAddr + xframeLeng - RX_BUFFER_BASE) > RX_BUFFER_SIZE) {
        // Wrap condition:
        MS_U32 frameLeng2 = (xframeAddr + xframeLeng - RX_BUFFER_BASE) - RX_BUFFER_SIZE;
        MS_U32 frameLeng1 = xframeLeng - frameLeng2;
        memcpy((MS_U8*)RAM_ADDR_BASE + RX_FRAME_ADDR,(MS_U8*)RAM_ADDR_BASE + xframeAddr,frameLeng1);
        memcpy((MS_U8*)RAM_ADDR_BASE + RX_FRAME_ADDR + frameLeng1,(MS_U8*)RAM_ADDR_BASE + RX_BUFFER_BASE,frameLeng2);
    } else {
        // Normal condition:
        if(1==1) {
            // Traditional method:
            memcpy((MS_U8*)RAM_ADDR_BASE + RX_FRAME_ADDR,(MS_U8*)RAM_ADDR_BASE + xframeAddr,xframeLeng);
        } else {
             // (20070411) New method to lower down the DRAM R/W activity
             // (20070412) Found this method is not good because the data in "&xframeAddr" may be overwrite by new RX.
             thisRX_FRAME_ADDR = xframeAddr;
        } // else
    } // else
    EMAC_Sync();
    ThisUVE.cntReadONE_RX++;
return(thisRX_FRAME_ADDR);
}

//------------------------------------------------------------------------------
/// @brief Update Rx Buufer
/// @param  [in] xframeAddr the frame address
/// @param  [in] xframeLeng the frame length
/// @return None
//------------------------------------------------------------------------------

void UpdateRxBuffRdPtr(MS_U32 xframeAddr,MS_U32 xframeLeng)
{
    // Update RxBuffRdPtr:
    MS_U32 currRxBuffRdPtr = EMAC_ReadReg32(REG_ETH_BUFFRDPTR);             // ETH_BUFFRDPTR
    MS_U32 bitWRAP_RD      = currRxBuffRdPtr & 0x00004000;
    MS_U32 advFrameLeng    = xframeLeng;
    if( (xframeLeng % 4) != 0 )
        advFrameLeng = advFrameLeng + (4 - (xframeLeng % 4));
    currRxBuffRdPtr = (xframeAddr & 0x00003FFC) + advFrameLeng;
    // Handle wrap condition:
    if(currRxBuffRdPtr >= RX_BUFFER_SIZE) {
        currRxBuffRdPtr = currRxBuffRdPtr - RX_BUFFER_SIZE;
        currRxBuffRdPtr = currRxBuffRdPtr | (bitWRAP_RD ^ 0x00004000);
    } else {
        currRxBuffRdPtr |= bitWRAP_RD;
    } // else
    EMAC_WritReg32(REG_ETH_BUFFRDPTR,currRxBuffRdPtr);
}

//------------------------------------------------------------------------------
/// @brief Look for available space from Rx buffer
/// @return [out] MS_S32 negative value means no available space. Otherwise return available address
//------------------------------------------------------------------------------
MS_S32 GetRxBuffFreeSpace(void)
// Return: signed free space (if negative, means RX BUF not available)
{
    MS_S32 retValue = 0x00000000;
    MS_U32 currRxBuffRdPtr = EMAC_ReadReg32(REG_ETH_BUFFRDPTR);             // ETH_BUFFRDPTR
    MS_U32 bitWRAP_RD      = currRxBuffRdPtr & 0x00004000;
    MS_U32 currRxBuffWrPtr = EMAC_ReadReg32(REG_ETH_BUFFWRPTR);             // ETH_BUFFWRPTR
    MS_U32 bitWRAP_WR      = currRxBuffWrPtr & 0x00004000;
    MS_U32 currRxBuffRdBuf = currRxBuffRdPtr & 0x00003FFC;
    MS_U32 currRxBuffWrBuf = currRxBuffWrPtr & 0x00003FFC;
    if(bitWRAP_RD == bitWRAP_WR) {
        retValue = (int)RX_BUFFER_SIZE - currRxBuffWrBuf + currRxBuffRdBuf;
    } else {
        retValue = (int)currRxBuffRdBuf - currRxBuffWrBuf;
    } // else
return(retValue);
}

//------------------------------------------------------------------------------
/// @brief Waiting for transmission is complete
/// @param [in] xFrameLeng the frame length
/// @return None
//------------------------------------------------------------------------------
void CheckToTransmit(MS_U32 xFrameLeng)
{
    MS_U32 chkTXWaitTimes = 0x1000; // (20071026_CHARLES) Times is tunable.
    // IMPORTANT: If waits till "chkTXWaitTimes" without "txPermit", give up this frame, next frame will be permitted.
    while(chkTXWaitTimes > 0)
    {
        MS_U32 retETH_TSR = EMAC_ReadReg32(REG_ETH_TSR);
        if( (retETH_TSR & 0x00000008) == 0x00000008 ) {
            ThisUVE.flagMacTxPermit = 0x00; // Maybe TCOM interrupt occurrs sooner, the "ThisUVE.macTxPermit" -> 1 -> 0 !!!
            EMAC_WritReg32(REG_ETH_TCR,xFrameLeng);         // Write transmit length register <== START TX
            chkTXWaitTimes = 0;
        }
    else {
            // (20070804) Wait and clear EVNT_EMAC_CHECK_TRANSMIT here. (NOTE: try E_OR_CLEAR later)
            MS_U32 u32RetrievedEventFlags;
            if( MsOS_WaitEvent(_s32EMAC_EventGroupID, EVNT_EMAC_CHECK_TRANSMIT, &u32RetrievedEventFlags, E_OR, 200) ) {
                ThisUVE.cntChkToTransmit++;
                MsOS_ClearEvent(_s32EMAC_EventGroupID, EVNT_EMAC_CHECK_TRANSMIT);
            } // if
            chkTXWaitTimes = chkTXWaitTimes - 1;
        } // else
    } // while
}

//------------------------------------------------------------------------------
/// @brief EMAC Driver Main Thread
/// @return None
//------------------------------------------------------------------------------
void EMAC_DRIVER(void)
{
    // Main Loop:
  //ThisUVE.cntRxFrames            = 0;            // Counter of RX frames
  //diag_printf("\nEMAC_DRIVER():");
    while(1)
    {
#if VER_EMAC_ENABLE
    ver_emac_in_EMAC_DRIVER();
#endif
        // Check Main Thread Tasks:
        if(ThisUVE.mainThreadTasks & 0x00000001) {
//            EMAC_PowerOff();
            MHal_EMAC_Power_Off_Clk();
            ThisUVE.mainThreadTasks &= 0xFFFFFFFE;
            ThisUVE.flagPowerOn = 0;
        } // if
        if(ThisUVE.mainThreadTasks & 0x00000002) {
//            EMAC_PowerOn();
            MHal_EMAC_Power_On_Clk();
            ThisUVE.mainThreadTasks &= 0xFFFFFFFD;
            ThisUVE.flagPowerOn = 1;
        } // if

        if(ThisUVE.flagPowerOn) {

//            MDev_EMAC_update_linkspeed();

#if 1
            // (20071026_CHARLES) Check cable connection:

            if(EMAC_CableConnection() == 1) {
              //diag_printf("\nEMAC_DRIVER : connect    (re=%d,ed=%d)",reConnecting,ThisBCE.connected);
                if(ThisBCE.connected == 0) {
                    ThisUVE.flagMacTxPermit = 0x01;
                    // Check duplex here:
                    EMAC_CheckDuplex();
#if VER_EMAC_INTERNAL_LOOPBACK
                EMAC_WritReg32(REG_ETH_CTL,EMAC_CTRL_CURRENT | 0x02);//loopback
#else
                EMAC_WritReg32(REG_ETH_CTL,EMAC_CTRL_CURRENT);
#endif
                    //diag_printf("\nEMAC_DRIVER : reconnected");
                    } // if
                ThisBCE.connected = 1;
                } else {
              //diag_printf("\nEMAC_DRIVER : disconnect (re=%d,ed=%d)",reConnecting,ThisBCE.connected);
                if(ThisBCE.connected == 1) {
                    ThisUVE.flagMacTxPermit = 0x00;
                    // (20071026_CHARLES) Disable TX, RX:
                    EMAC_WritReg32(REG_ETH_CTL,EMAC_CTRL_CURRENT);
                    //diag_printf("\nEMAC_DRIVER : disconnected");
                } // if
                ThisBCE.connected = 0;
                } // else
            //^(20071026_CHARLES) Check cable connection:
#endif
        } // if

            // MAIN_THREAD : Wait and clear event here. (NOTE: try E_OR_CLEAR later)
        {
            MS_U32 u32RetrievedEventFlags;
            if( MsOS_WaitEvent(_s32EMAC_EventGroupID, EVNT_EMAC_MAIN_THREAD, &u32RetrievedEventFlags, E_OR, MAIN_THREAD_SLEEP_MS) ) {
                MsOS_ClearEvent(_s32EMAC_EventGroupID, EVNT_EMAC_MAIN_THREAD);
                EMAC_DBG("\n");
            } // if
        }
        MsOS_DelayTask(10);
    } // while
}

//------------------------------------------------------------------------------
/// @brief MAC hash function for CAM memory
/// @param [in] m0 the sixth byte of MAC address (LSB)
/// @param [in] m1 the fifth byte of MAC address
/// @param [in] m2 the fourth byte of MAC address
/// @param [in] m3 the third byte of MAC address
/// @param [in] m4 the second byte of MAC address
/// @param [in] m5 the first byte of MAC address (MSB)
/// @return [out] MS_U8 the index inside the CAM memory
//------------------------------------------------------------------------------

MS_U8 CalcMACHash(MS_U8 m0,MS_U8 m1,MS_U8 m2,MS_U8 m3,MS_U8 m4,MS_U8 m5)
{
    MS_U8 hashIdx0 = (m0&0x01)^((m0&0x40)>>6)^((m1&0x10)>>4)^((m2&0x04)>>2)^(m3&0x01)^((m3&0x40)>>6)^((m4&0x10)>>4)^((m5&0x04)>>2);
    MS_U8 hashIdx1 = (m0&0x02)^((m0&0x80)>>6)^((m1&0x20)>>4)^((m2&0x08)>>2)^(m3&0x02)^((m3&0x80)>>6)^((m4&0x20)>>4)^((m5&0x08)>>2);
    MS_U8 hashIdx2 = (m0&0x04)^((m1&0x01)<<2)^((m1&0x40)>>4)^((m2&0x10)>>2)^(m3&0x04)^((m4&0x01)<<2)^((m4&0x40)>>4)^((m5&0x10)>>2);
    MS_U8 hashIdx3 = (m0&0x08)^((m1&0x02)<<2)^((m1&0x80)>>4)^((m2&0x20)>>2)^(m3&0x08)^((m4&0x02)<<2)^((m4&0x80)>>4)^((m5&0x20)>>2);
    MS_U8 hashIdx4 = (m0&0x10)^((m1&0x04)<<2)^((m2&0x01)<<4)^((m2&0x40)>>2)^(m3&0x10)^((m4&0x04)<<2)^((m5&0x01)<<4)^((m5&0x40)>>2);
    MS_U8 hashIdx5 = (m0&0x20)^((m1&0x08)<<2)^((m2&0x02)<<4)^((m2&0x80)>>2)^(m3&0x20)^((m4&0x08)<<2)^((m5&0x02)<<4)^((m5&0x80)>>2);
return(hashIdx0 | hashIdx1 | hashIdx2 | hashIdx3 | hashIdx4 | hashIdx5);
}

//------------------------------------------------------------------------------
/// @brief Send packet function
/// @param [in] xframeAddr the frame address
/// @param [in] xframeLeng the frame length
/// @return None
//------------------------------------------------------------------------------
void SendPacket(MS_U32 xframeAddr,MS_U32 xframeLeng)
// IMPORTANT: The "xframeAddr" of SendPacket() must be absolute address. Not relative to RAM_ADDR_BASE.
{
    MS_U32 realTxAddr = 0x00000000;
//    EMAC_DBG("\n");
//    MDrv_EMAC_DumpMem(xframeAddr, xframeLeng);

#if VER_EMAC_LOOPBACK
#if VER_EMAC_FORCE_TX
        printf("ERROR: VER_EMAC_FORCE_TX can't be use in VER_EMAC_LOOPBACK");
#endif//VER_EMAC_FORCE_TX
    return;//disable os transmit
#endif//VER_EMAC_LOOPBACK

    if(xframeLeng > 1522)
return;
    // IMPORTANT: If xframeAddr < RAM_ADDR_BASE, we should copy it first.
    if(xframeAddr < RAM_ADDR_BASE) {
        realTxAddr = TX_BUFFER_BASE;
        memcpy((MS_U8*)RAM_ADDR_BASE + TX_BUFFER_BASE,(MS_U8*)xframeAddr,xframeLeng);
    }
    else{
        realTxAddr = xframeAddr - RAM_ADDR_BASE;
    }
#if VER_EMAC_ENABLE
    if(ver_emac_in_SendPacket(xframeAddr)==0)return;
#endif//VER_EMAC_ENABLE
    MsOS_FlushMemory();
    EMAC_Sync();
    while(EMAC_ReadReg32(REG_ETH_TCR)&0x7ff)
    {
        //EMAC_DELAY_10();
    }
    EMAC_WritReg32(REG_ETH_TAR,realTxAddr);     // Write transmit address register
    EMAC_WritReg32(REG_ETH_TCR,xframeLeng);
    
    //Set TX enable event
    emac_dsr(0x00000001);
    //CheckToTransmit(xframeLeng);
}

//------------------------------------------------------------------------------
/// @brief Retrieve the current configurations from EMAC registers
/// @param [in] xoid the configuration item
/// @param [in] xbuf
/// @param [in] xlen
/// @param [in] xread
/// @return None
//------------------------------------------------------------------------------
DRVEMAC_RESULT QueryInformation(EMAC_OID xoid,MS_U8 *xbuf,MS_U32 xlen,MS_U32 *xread)
{
    MS_U32  readReg = 0, ulInfo = 0;
    EMAC_WritReg32(REG_ETH_CTL,EMAC_CTRL_CURRENT);



    if(xlen < 4)
return(DRVEMAC_FAIL);
    switch(xoid)
    {
        case EOID_LINK_SPEED:
            if(ThisBCE.speed == 10) ulInfo = EMAC_LINK_SPEED_10;
            else                    ulInfo = EMAC_LINK_SPEED_100;
            break;
        case EOID_MEDIA_CONNECT_STATUS:
            // (20070515) Julian: reg 1: bit2   6306xxxx  ==>  bit 2 link status   1:yes, 0:no
            
        /* matt.lin -- PHY control */
            MHal_EMAC_read_phy(u8PhyAddr, PHY_REG_STATUS, &readReg);

            if((readReg & 0x00000004) == 0x00000004) ThisBCE.connected = 1;
            else                                     ThisBCE.connected = 0;
            //^(20070515)
            if(ThisBCE.connected == 0) ulInfo = EMAC_CONN_STATUS_DISCONNECTED;
            else                       ulInfo = EMAC_CONN_STATUS_CONNECTED;
            break;
        case EOID_MAC_ADDRESS:
            if(xlen >= 6)
                memcpy(xbuf,ThisBCE.sa1,6);
            break;
        case EOID_DUPLEX:
            if(ThisBCE.duplex == 1) ulInfo = EMAC_DUPLEX_HALF;
            else                    ulInfo = EMAC_DUPLEX_FULL;
            break;
        case EOID_XMIT_OK:
            ThisUVE.cntREG_ETH_FRA += EMAC_ReadReg32(REG_ETH_FRA);
            ulInfo = ThisUVE.cntREG_ETH_FRA;
            break;
        case EOID_RCV_OK:
            ThisUVE.cntREG_ETH_OK += EMAC_ReadReg32(REG_ETH_OK);
            ulInfo = ThisUVE.cntREG_ETH_OK;
            break;
        case EOID_XMIT_ERROR:
            ulInfo = ThisUVE.cntRTRY + ThisUVE.cntTUND + ThisUVE.cntTOVR;
            break;
        case EOID_RCV_ERROR:
            ThisUVE.cntREG_ETH_SEQE += EMAC_ReadReg32(REG_ETH_SEQE);
            ThisUVE.cntREG_ETH_ALE  += EMAC_ReadReg32(REG_ETH_ALE);
            ThisUVE.cntREG_ETH_RE   += EMAC_ReadReg32(REG_ETH_RE);
            ThisUVE.cntREG_ETH_ROVR += EMAC_ReadReg32(REG_ETH_ROVR);
            ThisUVE.cntREG_ETH_SE   += EMAC_ReadReg32(REG_ETH_SE);
            ThisUVE.cntREG_ETH_ELR  += EMAC_ReadReg32(REG_ETH_ELR);
            ThisUVE.cntREG_ETH_RJB  += EMAC_ReadReg32(REG_ETH_RJB);
            ThisUVE.cntREG_ETH_USF  += EMAC_ReadReg32(REG_ETH_USF);
            ulInfo = ThisUVE.cntREG_ETH_SEQE + ThisUVE.cntREG_ETH_ALE + ThisUVE.cntREG_ETH_RE +
                ThisUVE.cntREG_ETH_ROVR + ThisUVE.cntREG_ETH_SE + ThisUVE.cntREG_ETH_ELR + ThisUVE.cntREG_ETH_RJB +
                ThisUVE.cntREG_ETH_USF;
            break;
        case EOID_RCV_NO_BUFFER:
            ThisUVE.cntREG_ETH_RE += EMAC_ReadReg32(REG_ETH_RE);
            ulInfo = ThisUVE.cntREG_ETH_RE;
            break;
        default:
            break;
    } // switch
    if(xoid == EOID_MAC_ADDRESS) {
        if(xlen >= 6)
            *xread        = 6;          // MAC_ADDRESS_SIZE
        else
return(DRVEMAC_FAIL);
    } else {
        *((MS_U32*)xbuf) = ulInfo;
        *xread        = sizeof(MS_U32);
    } // else
return(DRVEMAC_OK);
}
DRVEMAC_RESULT SetInformation(EMAC_OID xoid,MS_U8 *xbuf,MS_U32 xlen,MS_U32 *xwrit)
{
    MS_U32  ulInfo = 0;
    if(xlen < 4)
return(DRVEMAC_FAIL);
    ulInfo = *((MS_U32*)xbuf);
    *xwrit = sizeof(MS_U32);
    switch(xoid)
    {
        case EOID_LINK_SPEED:
            if(ulInfo == EMAC_LINK_SPEED_10) ThisBCE.speed = 10;
            else                             ThisBCE.speed = 100;
            break;
        case EOID_DUPLEX:
            if(ulInfo == EMAC_DUPLEX_HALF) ThisBCE.duplex = 1;
            else                           ThisBCE.duplex = 2;
            break;
         default:
            break;
    } // switch
    InitializeEMAC();
return(DRVEMAC_OK);
}
#if 0
unsigned char TCP_PACKET[]={
0x00,0x1c,0x25,0x7c,0xed,0xc5,0x30,0x31,0x30,0x32,0x30,0x33,0x08,0x00,0x45,0x00,
0x05,0xdc,0x37,0x4c,0x40,0x00,0x40,0x06,0xf0,0x35,0xac,0x10,0x5a,0xfd,0xac,0x10,
0x5a,0x7c,0x04,0x07,0x00,0x14,0x05,0xa7,0xf9,0x66,0x8d,0xea,0xb0,0x7c,0x50,0x10,
0x44,0x70,0x8f,0x83,0x00,0x00,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x61,0x62,0x63,0x64,
0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,
0x75,0x76,0x77,0x78,0x79,0x7a,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,
0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
0x0d,0x0a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37};
#endif
static void _DrvEMAC_Task0(MS_U32 argc, VOID *argv)
// Driver Thread:
{
    MS_ASSERT(-1 == _s32EMAC_Register_Mutex);
    if (0> (_s32EMAC_Register_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "MutexEMAC_Register", MSOS_PROCESS_PRIVATE)))
    {
        MS_ASSERT(0);
    }
    DefaultCfgEMAC();
    ThisBCE.rcv_bcast = 1;
    ThisBCE.speed     = EMAC_SPEED_100;
    ThisBCE.sa1[0]    = MY_MAC[0]; ThisBCE.sa1[1] = MY_MAC[1]; ThisBCE.sa1[2] = MY_MAC[2]; ThisBCE.sa1[3] = MY_MAC[3]; ThisBCE.sa1[4] = MY_MAC[4]; ThisBCE.sa1[5] = MY_MAC[5];
    ThisUVE.initedEMAC = 0; // 0:Not initialized, 1:Initialized.
    // wyc.dbg-150624
    printf("_DrvEMAC_Task0\n");

    InitializeEMAC();
#if 0
        MS_U8* pu8Data;
        MS_U32 i;
        if(EMAC_CableConnection())
        {
            pu8Data=(MS_U8*)RAM_ADDR_BASE + TX_BUFFER_BASE;
            for(i=0;i<sizeof(TCP_PACKET);i++)
            {
                pu8Data[i]=TCP_PACKET[i];
            }
            while(1)
            {
                SendPacket((MS_U32)pu8Data,sizeof(TCP_PACKET));
            }
        }
#endif
#if VER_EMAC_ENABLE
    ver_emac_in__DrvEMAC_Task0();
#endif
    printf("EMAC: init done\n");
#if 1  // (20071026_CHARLES) Enable the EMAC_DRIVER thread
    EMAC_DRIVER();
#endif //^(20071026_CHARLES) Enable the EMAC_DRIVER thread
}

//------------------------------------------------------------------------------
/// @brief EMAC Driver init function
/// @return [out] DRVEMAC_RESULT the initialization result
//------------------------------------------------------------------------------
DRVEMAC_RESULT MDrv_EMAC_Init(void)
{
    EMAC_DBG("%s %s\n", __TIME__, __DATE__);
//    PHY_Tx = SendPacket;
//    PHY_TxBufGet = drvEMAC_send;
//    PHY_Rx = drvEMAC_recv;
//    PHY_RxCheck = CheckFrameRX;
//    PHY_RxLength = drvEMAC_RxEvent;
//    PHY_IntEnable = drvEMAC_EnableInterrupt;
//    PHY_MacGet = drvEMAC_GetMacAddr;
    PHY_ConnectStatus = MDrv_EMAC_ConnectStatus;
//    PHY_Start = EMAC_start;
//    PHY_Stop = EMAC_stop;
    PHY_Source = 1;

#if REMOVE_UPPER_LAYER_FUNCTIONS
#else//REMOVE_UPPER_LAYER_FUNCTIONS
     /* matt.lin -- we start DHCP thread here */
    dhcp_start_dhcp_mgt_thread();
    /* end of matt.lin */
#endif//REMOVE_UPPER_LAYER_FUNCTIONS

    // Create Task ----------------------------------------------------------
#if (USE_TASK)
    // Allocate memory before threads start.
    // RAM_ADDR_BASE:
#if 0
    alloRAM_ADDR_BASE = (MS_U32)MsOS_AllocateMemory(EMAC_ABSO_MEM_SIZE,gs32NonCachedPoolID);
    if(!alloRAM_ADDR_BASE) {
        GEN_EXCEP;
    } // if
#else
    alloRAM_ADDR_BASE = (MS_U32)KSEG02KSEG1(u8EMAC_Abso_Buffer);
#endif
    memset((MS_U8*)alloRAM_ADDR_BASE, 0x00, EMAC_ABSO_MEM_SIZE);
    // IMPORTANT:
    // (1) Assume the allocated "RAM_ADDR_BASE" is greater than 0xA0000000
    // (2) We use the memory address at 0x?????000
    RAM_ADDR_BASE    = (alloRAM_ADDR_BASE & 0xFFFFC000) + 0x4000;   // IMPORTANT: Let lowest 14 bits as zero.
    RX_BUFFER_BASE    += RAM_ADDR_BASE - EMAC_ABSO_MEM_BASE;
    RBQP_BASE        += RAM_ADDR_BASE - EMAC_ABSO_MEM_BASE;
    TX_BUFFER_BASE    += RAM_ADDR_BASE - EMAC_ABSO_MEM_BASE;
    RX_FRAME_ADDR   += RAM_ADDR_BASE - EMAC_ABSO_MEM_BASE;
    RAM_ADDR_BASE    = EMAC_ABSO_MEM_BASE;  // IMPORTANT_TRICK_20070512
    dlist = (struct recv_desc_bufs *)(RBQP_BASE + RAM_ADDR_BASE);
    printf("emac addr:%x,%x,%x,%x\n",alloRAM_ADDR_BASE,RAM_ADDR_BASE,RX_BUFFER_BASE,RBQP_BASE);
#endif
    // Create task 0
#if 0
    _VerEMAC_Task[0].pStack = MsOS_AllocateMemory(_VerEMAC_Task[0].u32StackSize, gs32CachedPoolID);
    if(!_VerEMAC_Task[0].pStack) {
        GEN_EXCEP;
    } // if
#else
    _VerEMAC_Task[0].pStack = u8EMAC_StackBuffer;
#endif
    _VerEMAC_Task[0].iId = MsOS_CreateTask(
        _DrvEMAC_Task0                    ,
        (MS_U32)NULL                            ,
        _VerEMAC_Task[0].ePriority        ,
        TRUE                            ,
        _VerEMAC_Task[0].pStack            ,
        _VerEMAC_Task[0].u32StackSize    ,
        _VerEMAC_Task[0].szName
    );
    if(_VerEMAC_Task[0].iId < 0) {
        GEN_EXCEP;
    } // if

    // Create Event Group EMAC
    _s32EMAC_EventGroupID = MsOS_CreateEventGroup("Event Group EMAC");
    if(_s32EMAC_EventGroupID < 0) {
        GEN_EXCEP;
    } // if

    //^Create Task ----------------------------------------------------------
    if(ISR_IMPLEMENTED == 1) {
        MDrv_EMAC_RegisterCallBack(_DrvEMAC_ISR);
    } // if


return(DRVEMAC_OK);
}

// API for system networking driver =========================================
void drvEMAC_RxEvent(MS_U32 *FrameLeng)
// Return: frame length in the RX descriptor
//         0     : no frame
//         non-0 : frame length
{
//    EMAC_DBG("\n");
    MS_U32 idxRBQP  = ThisUVE.readIdxRBQP;
    MS_U32 entry_W0 = dlist->descriptors[idxRBQP].addr;
    MS_U32 entry_W1 = ((dlist->descriptors[idxRBQP].high_tag & 0x7) << 11) | (dlist->descriptors[idxRBQP].low_tag & 0x7ff);
    if((entry_W0 & 0x00000001) == 0x00000001) {        // If any Ownership bit is 1, frame received.
        // Received frame:
      //MS_U32 frameAddr = entry_W0 & 0xFFFFFFFC;
        *FrameLeng = ( entry_W1 & 0x000007FF ) - 4;  // minus the 4-byte CRC
    } else
        *FrameLeng = 0;
}

//------------------------------------------------------------------------------
/// @brief Read one frame from Rx buffer for pcnet driver
/// @param  [in] sg_list0 the mbuf list
/// @param  [in] sg_len the mbuf records
/// @param  [in] xframeAddr the frame address
/// @param  [in] xframeLeng the frame length
/// @return none
//------------------------------------------------------------------------------
void ReadONE_RX_Frame_recv(void *sg_list0,int sg_len,MS_U32 xframeAddr,MS_U32 xframeLeng)
{
//    EMAC_DBG("\n");
    struct eth_drv_sgX *sg_list = (struct eth_drv_sgX *)sg_list0;
        // Normal condition:
      //memcpy((MS_U8*)RAM_ADDR_BASE + RX_FRAME_ADDR,(MS_U8*)RAM_ADDR_BASE + xframeAddr,xframeLeng);
        int i, mlen = 0;
        MS_U8 *data;
        MS_U8 *buf  = (MS_U8*)RAM_ADDR_BASE + xframeAddr;
        int plen = xframeLeng - 4;   // Ignore CRC 4 bytes.
        for(i = 0;  i < sg_len;  i++)
        {
            data = (MS_U8*)sg_list[i].buf;
            mlen = sg_list[i].len;
            if(data)
            {
                memcpy(data, buf, mlen);
                buf = buf + mlen;
                plen = plen - mlen;
            }
        } // for
      //diag_printf("\nReadONE_RX_Frame_recv : NORMAL : leng=%d",xframeLeng - 4);

#if VER_EMAC_ENABLE
    ver_emac_in__ReadONE_RX_Frame_recv(sg_list0, xframeLeng);
#endif

    ThisUVE.cntReadONE_RX++;
}


void drvEMAC_recv(void *sg_list0,int sg_len)
{
//    EMAC_DBG("\n");
    // NOTE: (20071030_CHARLES) : Copy source code from ReadSINGLE_RX_Frame() and modify.
    // Handle "buffer not available" case
//tpoint2 = cyg_current_time();
    if(ThisUVE.flagISR_INT_RBNA == 1) {
        ThisUVE.flagISR_INT_RBNA = 0;
        MHal_EMAC_Write_RSR(EMAC_BNA);        // Clear REC bit by writing '1'
    } // if
    MS_U32 idxRBQP     = ThisUVE.readIdxRBQP;   // IMPORTANT: Wait to use the method in PAGE_13 of EMAC manual.
    MS_U32 entry_W0 = dlist->descriptors[idxRBQP].addr;
    if((entry_W0 & 0x00000001) == 0x00000001) {        // If any Ownership bit is 1, frame received.
        // Received frame:
        MS_U32 entry_W1  = ((dlist->descriptors[idxRBQP].high_tag & 0x7) << 11) | (dlist->descriptors[idxRBQP].low_tag & 0x7ff);
        MS_U32 frameAddr = entry_W0 & 0xFFFFFFFC;
        MS_U32 frameLeng = entry_W1 ;    // NOTE: frameLeng contains 4 bytes of CRC

#if VER_EMAC_LOOPBACK
        _ver_emac_current_rx_desc[0]=entry_W0;
        _ver_emac_current_rx_desc[1]=entry_W1;
#endif
        // Now "frameAddr"++"frameLeng" is current frame.
        // IMPORTANT: The "frameAddr" is absolute by 0xA0000000 (not relative by RX_BUFFER_BASE)
        //MDrv_EMAC_DumpMem(RAM_ADDR_BASE + frameAddr, frameLeng);
        ReadONE_RX_Frame_recv(sg_list0 , sg_len, frameAddr, frameLeng);  // Note: ThisUVE.rxOneFrameAddr not used here.
        
#if SW_DESC_SUPPORT
        // Clear Ownership of this descriptor(Keep address):
//        EMAC_DBG("%08x %08x\n", entry_W0 & ~(EMAC_DESC_DONE | EMAC_DESC_WRAP), 0xFFFFFFFE & entry_W0);
        dlist->descriptors[idxRBQP].addr &= 0xFFFFFFFE;
#else
        UpdateRxBuffRdPtr(frameAddr,frameLeng);
        
        // Clear this descriptor:
        dlist->descriptors[idxRBQP].addr &= 0x00000002;
#endif
        ThisUVE.readIdxRBQP = (idxRBQP + 1) % RBQP_LENG;

        //Check if the next packet available
        if( (dlist->descriptors[ThisUVE.readIdxRBQP].addr & 0x00000001) == 0x00000001 )
        {
            //Next packet is available, disable RX interrupt and assert RX event
            MHal_EMAC_Write_IDR(EMAC_INT_RCOM);
            emac_dsr(0x02);
        }
        else
        {
            //Next packet is not available, enable RX interrupt
            MHal_EMAC_Write_IER(EMAC_INT_RCOM);
        }
    } // if
    EMAC_Sync();
//tpoint3 = cyg_current_time();
}

void drvEMAC_send(void **xTxBuf)
{
//    EMAC_DBG("\n");
    while(EMAC_ReadReg32(REG_ETH_TCR)&0x7ff)
    {
        //EMAC_DELAY_10();
    }
    *xTxBuf = (void*)((char*)RAM_ADDR_BASE + TX_BUFFER_BASE);
}

/* matt.lin(20071009) -- Add a function to control our own interrupt from pcnet driver */
void drvEMAC_EnableInterrupt(void){
//    EMAC_DBG("\n");
    MsOS_EnableInterrupt((InterruptNum)E_INT_IRQ_SECEMAC);
}

MS_U8* drvEMAC_GetMacAddr(void){
    EMAC_DBG("\n");
    return &MY_MAC[0];
}

void MDrv_EMAC_ChangeMacAddr(MS_U8* mac){
    int idx;
    // EMac driver also need to re-configuration as well.
    // Need to disable Tx/Rx first ?

    // Re-init EMac driver for new MAC address
    for ( idx = 0; idx < 6; idx++ )
    {
        ThisBCE.sa1[idx] = mac[idx];
        MY_MAC[idx] = mac[idx];
    }

        // Set MAC address ------------------------------------------------------
    WritMAC_Address(REG_ETH_SA1L,ThisBCE.sa1[0],ThisBCE.sa1[1],ThisBCE.sa1[2],ThisBCE.sa1[3],ThisBCE.sa1[4],ThisBCE.sa1[5]);
    WritMAC_Address(REG_ETH_SA2L,ThisBCE.sa2[0],ThisBCE.sa2[1],ThisBCE.sa2[2],ThisBCE.sa2[3],ThisBCE.sa2[4],ThisBCE.sa2[5]);
    WritMAC_Address(REG_ETH_SA3L,ThisBCE.sa3[0],ThisBCE.sa3[1],ThisBCE.sa3[2],ThisBCE.sa3[3],ThisBCE.sa3[4],ThisBCE.sa3[5]);
    WritMAC_Address(REG_ETH_SA4L,ThisBCE.sa4[0],ThisBCE.sa4[1],ThisBCE.sa4[2],ThisBCE.sa4[3],ThisBCE.sa4[4],ThisBCE.sa4[5]);

}

MS_BOOL MDrv_EMAC_ConnectStatus(void)
{
    EMAC_DBG("\n");
    return ThisBCE.connected;
    //return EMAC_CableConnection();
}

//------------------------------------------------------------------------------
/// @brief EMAC Power Off Mode
/// @return None
//------------------------------------------------------------------------------
void EMAC_PowerOff(void)
{
    MS_U32 emac_clk;

    // (20071026_CHARLES) Disable TX, RX. Enable MDIO.
    EMAC_WritReg32(REG_ETH_CTL,0x0010);//only enable MDIO
    // (1) Let PHY chip enters its own power-saving mode:
    //     Register0[11]=¡¦1b1;  ( For our test case: write 0xbf809c34 with 0x57820800)  // (20071023) NOTE: 0xbf809c34 is management port.
    
    /* matt.lin -- PHY control */
    MHal_EMAC_write_phy(u8PhyAddr, PHY_REG_CONFIG, 0x0800);

    EMAC_WritReg32(REG_ETH_CTL,0x00000000);
  //diag_printf("\nEMAC_PowerOff : 3");
    // (2) Turn off all clock sources for EMAC.
    //     0xbf803c54[11: 8]=4¡¦h1;    // clk_emac_ahb
    emac_clk = *((MS_U32*)((char*)0xbf803c54));
    emac_clk = emac_clk | 0x00000F00;

    *((MS_U32*)((char*)0xbf803c54)) = emac_clk;
    //     0xbf803c14[15:13]=3¡¦h1;    // clk_emac_tx
    //     0xbf803c14[12:10]=3¡¦h1;    // clk_emac_rx
    emac_clk = *((MS_U32*)((char*)0xbf803c14));
    emac_clk = emac_clk | 0x0000FC00;
    *((MS_U32*)((char*)0xbf803c14)) = emac_clk;
    //     (20071023) IMPORTANT: 0xbf803c54, 0xbf803c14 are Uranus registers. Must not write directly.
    //                We should read first, OR some bits with '1', then write them.
    //diag_printf("\nEMAC_PowerOff : OK");
}
void EMAC_PowerOff_Set(void)
{
    ThisUVE.mainThreadTasks |= 0x00000001;  // b0=Poweroff
    MsOS_SetEvent(_s32EMAC_EventGroupID, EVNT_EMAC_MAIN_THREAD);
}

//------------------------------------------------------------------------------
/// @brief EMAC Power On Mode
/// @return None
//------------------------------------------------------------------------------
void EMAC_PowerOn(void)
{
    MS_U32 emac_clk;
    // (1) Turn on all clock sources for EMAC.
    //     0xbf803c54[11: 8]=4¡¦h0;    // clk_emac_ahb
    emac_clk = *((MS_U32*)((char*)0xbf803c54));
    emac_clk = emac_clk & 0xFFFFF0FF;
    *((MS_U32*)((char*)0xbf803c54)) = emac_clk;
    //     0xbf803c14[15:13]=3¡¦h0;    // clk_emac_tx
    //     0xbf803c14[12:10]=3¡¦h0;    // clk_emac_rx
    emac_clk = *((MS_U32*)((char*)0xbf803c14));
    emac_clk = emac_clk & 0xFFFF03FF;

#if EMAC_USE_RMII
    emac_clk |= 0x9000;
#endif

    *((MS_U32*)((char*)0xbf803c14)) = emac_clk;
    //     (20071023) IMPORTANT: 0xbf803c54, 0xbf803c14 are Uranus registers. Must not write directly.
    //                We should read first, OR some bits with '1', then write them.
    // (2) Let PHY chip enters its own power-saving mode:
    //     Register0[11]=¡¦1b0;  ( For our test case: write 0xbf809c34 with 0x57820800)  // (20071023) NOTE: 0xbf809c34 is management port.
    EMAC_WritReg32(REG_ETH_CTL,0x00000010);    // Enable MDIO. Disable TX/RX.
    
  //diag_printf("\nEMAC_PowerOn : 0");
    /* matt.lin -- PHY control */
    MHal_EMAC_write_phy(u8PhyAddr, PHY_REG_CONFIG, 0x1000);
   
  //diag_printf("\nEMAC_PowerOn : 2");
    // Check connection and duplex:
    if(EMAC_CableConnection() == 1) {
        EMAC_CheckDuplex();
        ThisUVE.flagMacTxPermit = 0x01;
        ThisBCE.connected = 1;
    } else {
        ThisUVE.flagMacTxPermit = 0x00;
        ThisBCE.connected = 0;
    } // else

    EMAC_WritReg32(REG_ETH_CTL,EMAC_CTRL_CURRENT);
    //diag_printf("\nEMAC_PowerOn : OK");
}

void EMAC_PowerOn_Set(void)
{
    ThisUVE.mainThreadTasks |= 0x00000002;  // b1=Poweron
    MsOS_SetEvent(_s32EMAC_EventGroupID, EVNT_EMAC_MAIN_THREAD);
}

static void EMAC_start(void)
{
    EMAC_txrx_start=1;
    EMAC_WritReg32(REG_ETH_CTL,EMAC_CTRL_CURRENT);
}

static void EMAC_stop(void)
{
    EMAC_DBG("\n");
    EMAC_txrx_start=0;
    EMAC_WritReg32(REG_ETH_CTL,EMAC_CTRL_CURRENT);
}

