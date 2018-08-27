////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007, 2016 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#if (DEMO_URDMA_TEST == 1)
#include <ctype.h>
#include "MsOS.h"
#include "string.h"
#include "MsCommon.h"
#include "drvMMIO.h"
#include "drvUART.h"
#include "drvURDMA.h"
#include "demo_utility.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
#define EVENT_STOP_URDMA 0x01

// FCR
#define FCR_FIFO_ENABLE                 (0x01 << 0)
#define FCR_FIFO_RX_CLR                 (0x01 << 1)
#define FCR_FIFO_TX_CLR                 (0x01 << 2)
#define FCR_FIFO_TX_TRIG_EMPTY          (0x00 << 4)
#define FCR_FIFO_TX_TRIG_TWO_CHAR       (0x01 << 4)
#define FCR_FIFO_TX_TRIG_QFULL          (0x02 << 4)
#define FCR_FIFO_TX_TRIG_HFULL          (0x03 << 4)
#define FCR_FIFO_RX_TRIG_ONE_CHAR       (0x00 << 6)
#define FCR_FIFO_RX_TRIG_QFULL          (0x01 << 6)
#define FCR_FIFO_RX_TRIG_HFULL          (0x02 << 6)
#define FCR_FIFO_RX_TRIG_TWO_LESS_FULL  (0x03 << 6)

// LCR
#define LCR_CHAR_FIVE_BITS      (0x00 << 0)
#define LCR_CHAR_SIX_BITS       (0x01 << 0)
#define LCR_CHAR_SEVEN_BITS     (0x02 << 0)
#define LCR_CHAR_EIGHT_BITS     (0x03 << 0)
#define LCR_CHAR_ONE_STOP_BIT   (0x00 << 2)
#define LCR_CHAR_TWO_STOP_BIT   (0x01 << 2)
#define LCR_PARITY_BIT_OFF      (0x00 << 3)
#define LCR_PARITY_BIT_ON       (0x01 << 3)

// MCR
#define MCR_DTR_HIGH_ACT (0x00 << 0)// aka CTS
#define MCR_DTR_LOW_ACT  (0x01 << 0)
#define MCR_RTS_HIGH_ACT (0x00 << 1)
#define MCR_RTS_LOW_ACT  (0x01 << 1)
#define MCR_LOOPBACK     (0x01 << 4)
#define MCR_AFCE         (0x01 << 5) // Auto Flow Control Enable

extern MS_S32 gs32CachedPoolID;
extern MS_S32 gs32NonCachedPoolID;

static void *rx_pTaskStack = NULL;
static void *tx_pTaskStack = NULL;
static MS_S32 rx_task_id = -1;
static MS_S32 tx_task_id = -1;
static MS_S32 event_id;
static MS_BOOL running = FALSE;
static MS_U32 u32NonPmBase = 0;
static MS_U32 u32_fuart_handle;

#define REG16_NPM(addr)         *((volatile MS_U16*)(u32NonPmBase+ (addr)))

#define FAST_UART_DMA_REG           0x6980  //(0x1A60) * 4

// TODO: we NEED TWO BYTE SPACE! otherwise registers offset will not be correct!!
#define TWO_BYTE_SPACE              1

#define Struct_Register __attribute__((packed))
#define TEST_DATA_LEN 25

#if 1
/* Warning: Little Endian */
typedef struct dma_interface
{
    union
    {
        volatile MS_U16 reg00; /* 00h */
        struct
        {
            volatile MS_U16 sw_rst          :   1;  /* BIT0 */
            volatile MS_U16 urdma_mode      :   1;  /* BIT1 */
            volatile MS_U16 tx_urdma_en     :   1;  /* BIT2 */
            volatile MS_U16 rx_urdma_en     :   1;  /* BIT3 */
            volatile MS_U16 tx_endian       :   1;  /* BIT4 */
            volatile MS_U16 rx_endian       :   1;  /* BIT5 */
            volatile MS_U16 tx_sw_rst       :   1;  /* BIT6 */
            volatile MS_U16 rx_sw_rst       :   1;  /* BIT7 */
            volatile MS_U16 reserve00       :   3;  /* BIT8 ~ BIT10 */
            volatile MS_U16 rx_op_mode      :   1;  /* BIT11 */
            volatile MS_U16 tx_busy         :   1;  /* BIT12 */
            volatile MS_U16 rx_busy         :   1;  /* BIT13 */
            volatile MS_U16 reserve01       :   2;  /* BIT14 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space00;
#endif
    union
    {
        volatile MS_U16 reg01; /* 02h */
        struct
        {
            volatile MS_U16 intr_threshold  :   12; /* BIT0 ~ BIT11 */
            volatile MS_U16 reserve02       :   4;  /* BIT12 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space01;
#endif
    union
    {
        volatile MS_U16 reg02; /* 04h */
        struct
        {
            volatile MS_U16 tx_buf_base_h   :   11;  /* BIT0 ~ BIT7 */
            volatile MS_U16 reserve03       :   5;  /* BIT8 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space02;
#endif
    union
    {
        volatile MS_U16 reg03; /* 06h */
        struct
        {
            volatile MS_U16 tx_buf_base_l   :   16;  /* BIT0 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space03;
#endif
    union
    {
        volatile MS_U16 reg04; /* 08h */
        struct
        {
            volatile MS_U16 tx_buf_size     :   13;  /* BIT0 ~ BIT12 */
            volatile MS_U16 reserve04       :   3;   /* BIT13 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space04;
#endif
    union
    {
        volatile MS_U16 reg05; /* 0Ah */
        struct
        {
            volatile MS_U16 tx_buf_rptr     :   16;  /* BIT0 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space05;
#endif
    union
    {
        volatile MS_U16 reg06; /* 0Ch */
        struct
        {
            volatile MS_U16 tx_buf_wptr     :   16;  /* BIT0 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space06;
#endif
    union
    {
        volatile MS_U16 reg07; /* 0Eh */
        struct
        {
            volatile MS_U16 tx_timeout      :   4;   /* BIT0 ~ BIT3 */
            volatile MS_U16 reserve05       :   12;  /* BIT4 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space07;
#endif
    union
    {
        volatile MS_U16 reg08; /* 10h */
        struct
        {
            volatile MS_U16 rx_buf_base_h   :   11;  /* BIT0 ~ BIT7 */
            volatile MS_U16 reserve06       :   5;  /* BIT8 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space08;
#endif
    union
    {
        volatile MS_U16 reg09; /* 12h */
        struct
        {
            volatile MS_U16 rx_buf_base_l   :   16; /* BIT0 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space09;
#endif
    union
    {
        volatile MS_U16 reg0a; /* 14h */
        struct
        {
            volatile MS_U16 rx_buf_size     :   13; /* BIT0 ~ BIT12 */
            volatile MS_U16 reserve07       :   3;  /* BIT13 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space0a;
#endif
    union
    {
        volatile MS_U16 reg0b; /* 16h */
        struct
        {
            volatile MS_U16 rx_buf_wptr     :   16; /* BIT0 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space0b;
#endif
    union
    {
        volatile MS_U16 reg0c; /* 18h */
        struct
        {
            volatile MS_U16 rx_timeout      :   4;  /* BIT0 ~ BIT3 */
            volatile MS_U16 reserve08       :   12; /* BIT4 ~ BIT15 */
        } Struct_Register;
    } Struct_Register;
#ifdef TWO_BYTE_SPACE
    MS_U16 space0c;
#endif
    union
    {
        volatile MS_U16 reg0d;  /* 1Ah */
        struct
        {
            volatile MS_U16 rx_intr_clr     :   1; /* BIT0 */
            volatile MS_U16 rx_intr1_en     :   1; /* BIT1 */
            volatile MS_U16 rx_intr2_en     :   1; /* BIT2 */
            volatile MS_U16 reserve09       :   1; /* BIT3 */
            volatile MS_U16 rx_intr1        :   1; /* BIT4 */
            volatile MS_U16 rx_intr2        :   1; /* BIT5 */
            volatile MS_U16 reserve0a       :   1; /* BIT6 */
            volatile MS_U16 rx_mcu_intr     :   1; /* BIT7 */
            volatile MS_U16 tx_intr_clr     :   1; /* BIT8 */
            volatile MS_U16 tx_intr_en      :   1; /* BIT9 */
            volatile MS_U16 reserve0b       :   5; /* BIT10 ~ BIT14 */
            volatile MS_U16 tx_mcu_intr     :   1; /* BIT15 */
        } Struct_Register;
    } Struct_Register;
} Struct_Register dma_interface_t;
#endif

#define URDMA_LOU16(u32Val)   ((MS_U16)(u32Val)&0xffff)
#define URDMA_HIU16(u32Val)   ((MS_U16)((u32Val) >> 16)&0x07ff)

static MS_U32 u32urdma_base = 0;//HS_UART_DMA_REG_BASE
static volatile dma_interface_t *p_uart_dma = NULL;
static MS_U8 *rxBuff,*txBuff;
static void (*fast_uart_dma_rx_cb)(void) = NULL;
static void (*fast_uart_dma_tx_cb)(void) = NULL;
static MS_U8 brxflag = 0;

// this function is straight from drvURDMA.c
DRVURDMA_RESULT RxInit(MS_PHYADDR *rx_buf, MS_U16 rx_size)//hs_uart_dma_prepare_rx(void *rx_buf, U16 rx_size)
{
    DRVURDMA_RESULT ret = DRVURDMA_PASS;

    printf("prepare rx: 0x%08x 0x%x\n", (int)rx_buf, (int)rx_size);

    /* disable interrupt */
    MsOS_DisableInterrupt(E_INT_IRQ_URDMA2MCU);

    if (p_uart_dma == NULL)
    {
        ret = DRVURDMA_FATAL_ERROR;
        goto dma_prepare_rx_end;
    }

    p_uart_dma->rx_urdma_en = 0; /* disable dma */
    while (p_uart_dma->rx_busy)
    {
        int i=0;
        ret = DRVURDMA_RX_BUSY;
        printf("rx_busy!\n");
        if (1000 == (i++))
        {
            goto dma_prepare_rx_end;
        }
    }

    p_uart_dma->rx_buf_base_h = URDMA_HIU16((MS_PHYADDR)rx_buf);
    p_uart_dma->rx_buf_base_l = URDMA_LOU16((MS_PHYADDR)rx_buf);
    // p_uart_dma->rx_buf_size = (U16)(rx_size>>3);
    p_uart_dma->rx_buf_size = 0x1fff;
    p_uart_dma->intr_threshold = (MS_U16)rx_size;
    p_uart_dma->rx_timeout = 10; /* receive timeout. */
    p_uart_dma->rx_intr2_en = 1; /* Enable rx threshold interrupt. */

#ifdef MS_DEBUG
    MDrv_UrDMA_DumpREG(); //@@@ pending function
#endif

    p_uart_dma->rx_urdma_en = 1; /* Enable rx dma and start receiving. */

dma_prepare_rx_end:
    /* enable interrupt */
    MsOS_EnableInterrupt(E_INT_IRQ_URDMA2MCU);

    return ret;
}

// this function is straight from drvURDMA.c
DRVURDMA_RESULT TxInit(MS_PHYADDR *tx_buf, MS_U16 tx_size)
{
    DRVURDMA_RESULT ret = DRVURDMA_PASS;
    printf("prepare tx: 0x%08x 0x%x\n", (int)tx_buf, (int)tx_size);

    /* disable interrupt */
    MsOS_DisableInterrupt(E_INT_IRQ_URDMA2MCU);

    if (p_uart_dma == NULL)
    {
        ret = DRVURDMA_FATAL_ERROR;
        goto dma_prepare_tx_end;
    }

    p_uart_dma->tx_urdma_en = 0; /* disable dma */
    while (p_uart_dma->tx_busy)
    {
        int i = 0;
        ret = DRVURDMA_TX_BUSY;
        printf("tx_busy!\n");
        if (1000 == (i++))
        {
            //reg dump, here
            goto dma_prepare_tx_end;
        }
    }

    p_uart_dma->tx_buf_base_h = URDMA_HIU16((MS_PHYADDR)tx_buf);
    p_uart_dma->tx_buf_base_l = URDMA_LOU16((MS_PHYADDR)tx_buf);
    // p_uart_dma->tx_buf_size = (U16)(tx_size>>3);
    p_uart_dma->tx_buf_size = 0x1fff;
    p_uart_dma->tx_buf_wptr = (MS_U16)((tx_size-1) + (((MS_U32)tx_buf)%8)); /* alignment issue */
    p_uart_dma->tx_timeout = 1; /* send-out asap. */
    p_uart_dma->tx_intr_en = 1; /* Enable tx interrupt. */

#ifdef MS_DEBUG
    MDrv_UrDMA_DumpREG();
#endif

    p_uart_dma->tx_urdma_en = 1;/* Enable tx dma and start transmission. */

dma_prepare_tx_end:
    /* enable interrupt */
    MsOS_EnableInterrupt(E_INT_IRQ_URDMA2MCU);

    return ret;
}

void rxCallBack(void)
{
    // TODO: we need this print. In the case of rx interrupt raise AFTER ISR, we might
    //       not be able to process rx interrupt.
    MDrv_UART_PutString("rxCallBack\r\n");
    brxflag = 1;
}

void txCallBack(void)
{
    // TODO: we need this print. In the case of rx interrupt raise AFTER ISR, we might
    //       not be able to process rx interrupt.
    MDrv_UART_PutString("txCallBack\r\n");
}

#if 1
// From drvURDMA.c ISR
void MDrv_UrDMA_ISREx(void)//void hs_uart_dma_isr(void)
{
    //MDrv_UART_PutString("INT\r\n");
    if (p_uart_dma != NULL)
    {
        MsOS_DisableInterrupt(E_INT_IRQ_URDMA2MCU);
        // Be sure to handle all the interrupts before exiting
        while ((p_uart_dma->rx_mcu_intr) || (p_uart_dma->tx_mcu_intr)) {
            /* TX interrupt */
            if (p_uart_dma->tx_mcu_intr)
            {
                //MDrv_UART_PutString("tx int\r\n");
                while (p_uart_dma->tx_busy)
                    ;
                p_uart_dma->tx_intr_clr = 1; /* clear int status */
                p_uart_dma->tx_intr_en = 0; /* disable tx int */

                if (fast_uart_dma_tx_cb != NULL)
                {
                    fast_uart_dma_tx_cb();
                }
            }
            /* RX interrupt */
            if (p_uart_dma->rx_mcu_intr)
            {
                //MDrv_UART_PutString("rx int\r\n");
                while (p_uart_dma->rx_busy)
                    ;
                if (p_uart_dma->rx_intr2) /* Caused by intr threshold */
                {
                    //MDrv_UART_PutString("rx intr2\r\n");
                    p_uart_dma->rx_intr_clr = 1; /* clear int status */
                    p_uart_dma->rx_intr2_en = 0; /* disable rx threshold int */
                }
                if (p_uart_dma->rx_intr1) /* Caused by receive timeout */
                {
                    //MDrv_UART_PutString("rx intr1\r\n");
                    p_uart_dma->rx_intr_clr = 1; /* clear int status */
                    p_uart_dma->rx_intr1_en = 0; /* disable rx timeout int */
                }
                if (fast_uart_dma_rx_cb != NULL)
                {
                    fast_uart_dma_rx_cb();
                }
            }
        }
        MsOS_EnableInterrupt(E_INT_IRQ_URDMA2MCU);
    }
    return;
}
#endif

//#define UART_CLK_FREQ   123000000
#define UART_CLK_FREQ   54000000
#define UART_BAUDRATE    115200
#define UART_DIVISOR   (UART_CLK_FREQ/(16*UART_BAUDRATE))

static void fuart_setup(void)
{
    REG16_NPM(0x20D18) = REG16_NPM(0x20D18) | 0x80;
    REG16_NPM(0x20D00) = (UART_DIVISOR) & 0x00ff;
    REG16_NPM(0x20D08) = ((UART_DIVISOR) >> 8) & 0x00ff;
    REG16_NPM(0x20D18) = REG16_NPM(0x20D18) & 0xFF7F;
    REG16_NPM(0x20D08) = 0x8F; // RDA, THERI, RL, MS, S

    REG16_NPM(0x20D10) = 0x0000;
    REG16_NPM(0x20D10) = FCR_FIFO_ENABLE |
                         FCR_FIFO_RX_CLR |
                         FCR_FIFO_TX_CLR |
                         FCR_FIFO_TX_TRIG_TWO_CHAR |
                         FCR_FIFO_RX_TRIG_HFULL;//0x0007;

    REG16_NPM(0x20D18) = 0x0000;
    //WREG 0xbf201318,     0x0003
    REG16_NPM(0x20D18) = LCR_CHAR_EIGHT_BITS |
                         LCR_CHAR_ONE_STOP_BIT |
                         LCR_PARITY_BIT_ON; //0x0003;

    REG16_NPM(0x20D20) = 0x0000;
    REG16_NPM(0x20D20) = MCR_RTS_LOW_ACT | MCR_AFCE; //MCR_LOOPBACK;

    u32_fuart_handle = mdrv_uart_open(E_UART_PIU_FUART0);
    mdrv_uart_rx_enable(u32_fuart_handle);
    //mdrv_uart_set_rx_callback(u32_fuart_handle, fuart_callback); // has no effect
    //MsOS_AttachInterrupt(E_INT_IRQ_UART2MCU, (InterruptCb)MDrv_UrDMA_ISREx);
    //MsOS_EnableInterrupt(E_INT_IRQ_UART2MCU);
    mdrv_uart_connect(E_UART_PORT3, E_UART_PIU_FUART0);
}

static MS_BOOL GetNonPmBase(void)
{
    MS_U32 u32MMIOBaseAdr, u32BankSize;

    if( !MDrv_MMIO_GetBASE(&u32MMIOBaseAdr, &u32BankSize, MS_MODULE_HW))
    {
        printf("Get IOMap failure\n");
        MS_ASSERT(0);
        return FALSE;
    }
    else
    {
        printf("Get IOMap u32NonPmBase = 0x%lx \n",(MS_U32)u32MMIOBaseAdr);
    }

    u32NonPmBase = u32MMIOBaseAdr;

    printf("REG16_NPM(0x165C) = 0x%x \n",REG16_NPM(0x165C));
    return TRUE;
}

MS_BOOL urdma_setup(void)
{
    MsOS_AttachInterrupt(E_INT_IRQ_URDMA2MCU, (InterruptCb)MDrv_UrDMA_ISREx);
    MsOS_DisableInterrupt(E_INT_IRQ_URDMA2MCU);

    MDrv_UrDMA_SetDbgLevel(3);

    MDrv_UrDMA_Init(rxCallBack, txCallBack, 1);

    fast_uart_dma_rx_cb = rxCallBack;
    fast_uart_dma_tx_cb = txCallBack;

    u32urdma_base = u32NonPmBase + 0x6980;//FAST_UART_DMA_REG;
    p_uart_dma = (dma_interface_t *)u32urdma_base;
    brxflag = 0;

    return TRUE;
}


static void urdma_rx_task(MS_U32 argc, void *argv)
{
    MS_U32 i;
    MS_U32 count = 0;
    MS_U32 events;
    MS_BOOL timeout = FALSE;

    // allocate more than we needed, just in case.
    rxBuff = MsOS_AllocateMemory(1*1024, gs32NonCachedPoolID);
    memset(rxBuff, 0, sizeof(rxBuff));
    printf("Rx buffer addr = 0x%x \n",MsOS_VA2PA((MS_U32)rxBuff));

    RxInit((MS_PHYADDR *)MsOS_VA2PA((MS_U32)rxBuff), TEST_DATA_LEN);

    while (!brxflag) {
        printf("waiting for rx...\n");
        MsOS_WaitEvent(event_id,
                EVENT_STOP_URDMA,
                &events,
                E_OR_CLEAR,
                1000);

        if (events == EVENT_STOP_URDMA) {
            break;
        }
        if (++count >= 4) {
            timeout = TRUE;
            break;
        }
    }

    if (brxflag) {
        printf("rx!!!\n");
        for (i = 0; i < TEST_DATA_LEN; i++) {
            printf("rxBuff[%d] = %c\n", i, rxBuff[i]);
        }
        printf("Rx done.\n");
    }
    else if (timeout == TRUE) {
        printf("Rx timeout.\n");
    }
    else {
        printf("Rx terminated as requested.\n");
    }

    running = FALSE;
    if (rxBuff != NULL) {
        MsOS_FreeMemory(rxBuff, gs32NonCachedPoolID);
        rxBuff = NULL;
    }

    printf("Rx task exit gracefully.\n");
}

static void urdma_tx_task(MS_U32 argc, void *argv)
{
    // Allocate more buffer than we needed, just in case
    txBuff = MsOS_AllocateMemory(1*1024, gs32NonCachedPoolID);
    memset(txBuff, 0, sizeof(txBuff));
    printf("Tx buffer addr = 0x%x \n",MsOS_VA2PA((MS_U32)txBuff));

    // sample data, TODO: should generate random data
    txBuff[0] = 't';
    txBuff[1] = 'h';
    txBuff[2] = 'i';
    txBuff[3] = 's';
    txBuff[4] = ' ';
    txBuff[5] = 'i';
    txBuff[6] = 's';
    txBuff[7] = ' ';
    txBuff[8] = 'U';
    txBuff[9] = 'R';
    txBuff[10] = 'D';
    txBuff[11] = 'M';
    txBuff[12] = 'A';
    txBuff[13] = ' ';
    txBuff[14] = 't';
    txBuff[15] = 'e';
    txBuff[16] = 's';
    txBuff[17] = 't';
    txBuff[18] = '!';
    txBuff[19] = '@';
    txBuff[20] = '#';
    txBuff[21] = '%';
    txBuff[22] = '^';
    txBuff[23] = '&';
    txBuff[24] = '*';
    txBuff[25] = '(';
    MsOS_FlushMemory();

    // start tx
    TxInit((MS_PHYADDR *)MsOS_VA2PA((MS_U32)txBuff), TEST_DATA_LEN);

    if (MDrv_UrDMA_Polling_Tx_Done() == 0) {
        printf("Tx done.\n");
        MDrv_UrDMA_SelMode(0);
    }
    if (txBuff != NULL) {
        printf("free tx buf in task\n");
        MsOS_FreeMemory(txBuff, gs32NonCachedPoolID);
        txBuff = NULL;
    }
    printf("Tx task exit gracefully.\n");
}

MS_BOOL demo_urdma_test_stop(void)
{
    if (running == FALSE) {
        printf("no instance running\n");
        return FALSE;
    }
    running = FALSE;
    MsOS_SetEvent(event_id, EVENT_STOP_URDMA);

    // wait till Rx task exit before we proceed
    MsOS_DelayTask(1500);

    MsOS_DeleteEventGroup(event_id);
    event_id = -1;

    if (rxBuff != NULL) {
        MsOS_FreeMemory(rxBuff, gs32NonCachedPoolID);
    }
    if (txBuff != NULL) {
        MsOS_FreeMemory(txBuff, gs32NonCachedPoolID);
    }

    return TRUE;
}

MS_BOOL demo_urdma_test(void)
{
    MS_S32 s32MstarCachedPoolID = INVALID_POOL_ID;

    if (running == TRUE) {
        printf("already running\n");
        return FALSE;
    }

    if (GetNonPmBase() == FALSE) {
        return FALSE;
    }

    fuart_setup();
    urdma_setup();

    event_id = MsOS_CreateEventGroup("urdma_event");
    Demo_Util_GetSystemPoolID(E_DDI_POOL_SYS_CACHE, &s32MstarCachedPoolID);

    rx_pTaskStack = MsOS_AllocateMemory(4096, s32MstarCachedPoolID);
    if (rx_pTaskStack == NULL) {
        printf("failed to allocate rx task stack\n");
        return FALSE;
    }
    rx_task_id = MsOS_CreateTask(urdma_rx_task,
            (MS_U32)NULL,
            E_TASK_PRI_HIGH,
            TRUE,
            rx_pTaskStack,
            4096, // stack size
            "urdma rx task");
    if (rx_task_id < 0) {
        printf("failed to create rx task\n");
        return FALSE;
    }

    tx_pTaskStack = MsOS_AllocateMemory(4096, s32MstarCachedPoolID);
    if (tx_pTaskStack == NULL) {
        printf("failed to allocate tx task stack\n");
        return FALSE;
    }

    tx_task_id = MsOS_CreateTask(urdma_tx_task,
            (MS_U32)NULL,
            E_TASK_PRI_HIGH,
            TRUE,
            tx_pTaskStack,
            4096,
            "urdma tx task");
    if (tx_task_id < 0) {
        printf("failed to create tx task\n");
        return FALSE;
    }

    running = TRUE;

    return TRUE;
}
#endif
