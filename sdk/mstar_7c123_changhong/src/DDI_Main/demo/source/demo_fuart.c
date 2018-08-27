#if (DEMO_FUART_TEST == 1)
#include <string.h>
#include "MsOS.h"
#include "MsCommon.h"
#include "MsMemory.h"
#include "drvMMIO.h"
#include "drvUART.h"
#include "drvDTC.h"
#include "demo_utility.h"

#define REG16_NPM(addr)     *((volatile MS_U16*)(u32NonPmBase + (addr)))
//#define UART_CLK_FREQ       54000000
#define UART_CLK_FREQ       123000000   //Depends on Chip
#define UART_BAUDRATE       115200
#define UART_DIVISOR        (UART_CLK_FREQ/(16*UART_BAUDRATE))

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
#define MCR_DTR_HIGH_ACT (0x00 << 0) // aka CTS
#define MCR_DTR_LOW_ACT  (0x01 << 0)
#define MCR_RTS_HIGH_ACT (0x00 << 1)
#define MCR_RTS_LOW_ACT  (0x01 << 1)
#define MCR_LOOPBACK     (0x01 << 4)
#define MCR_AFCE         (0x01 << 5) // Auto Flow Control Enable

// IER bits
#define UART_IIR_NO_INT             0x01          // No interrupts pending
#define UART_IIR_THRI               0x02          // Transmitter holding register empty
#define UART_IIR_RDI                0x04          // Receiver data interrupt
#define UART_IIR_RLSI               0x06          // Receiver line status interrupt
#define UART_IIR_TOI                0x0c          // Receive time out interrupt

#define UART_IIR_ID                 0x06          // Mask for the interrupt ID

// LSR bits
#define UART_LSR_DR                 0x01          // Receiver data ready

#define RING_BUFF_SIZE  129

static MS_U32 u32NonPmBase = 0;
static MS_U32 u32_fuart_handle;
static MS_U32 int_count = 0;
static MS_BOOL fuart_inited = FALSE;
static MS_U8 rx_ring_buff[RING_BUFF_SIZE] = {0};
static MS_U8 rx_buff_index = 0;
static MS_U32 tx_buff_index = 0;

void fuart_isr(void);

MS_U8 test_data[] = {
#include "uart_test.dat"
};

MS_BOOL fuart_init(void)
{
    MS_VIRT u32MMIOBaseAdr;
    MS_PHY u32BankSize;

    if (TRUE == fuart_inited)
        return TRUE;

    if (!MDrv_MMIO_GetBASE(&u32MMIOBaseAdr, &u32BankSize, MS_MODULE_HW)) {
        printf("Get IOMap failure\n");
        MS_ASSERT(0);
        return FALSE;
    } else {
        printf("Get IOMap u32NonPmBase = 0x%"DTC_MS_VIRT_x"\n", u32MMIOBaseAdr);
    }

    u32NonPmBase = u32MMIOBaseAdr;

    // select Fast UART as fourthUart source (bit12~15)
    // (must reference schematics and adjust UART port accordingly).
#if 1
    REG16_NPM(0x3D4C) = REG16_NPM(0x3D4C) | (0x7000);
#else
    // connect/set UART Port3 source to FUART0
    mdrv_uart_connect(E_UART_PORT3, E_UART_PIU_FUART0);
#endif

    // set baud rate
    // set reg_lcr_dl_access to enable DL access
    REG16_NPM(0x20D18) = REG16_NPM(0x20D18) | 0x80;

    // set divisor latch
    REG16_NPM(0x20D00) = (UART_DIVISOR) & 0x00ff;
    REG16_NPM(0x20D08) = ((UART_DIVISOR) >> 8) & 0x00ff;

    // clear reg_lcr_dl_access to disable DL access
    REG16_NPM(0x20D18) = REG16_NPM(0x20D18) & 0xFF7F;

    // IER
    REG16_NPM(0x20D08) = 0x8F; // RDA, THERI, RL, MS, S

    // FCR
    REG16_NPM(0x20D10) = 0x0000;
    REG16_NPM(0x20D10) = FCR_FIFO_ENABLE |
                         FCR_FIFO_RX_CLR |
                         FCR_FIFO_TX_CLR |
                         FCR_FIFO_TX_TRIG_TWO_CHAR |
                         FCR_FIFO_RX_TRIG_HFULL;

    // LCR
    REG16_NPM(0x20D18) = 0x0000;
    REG16_NPM(0x20D18) = LCR_CHAR_EIGHT_BITS |
                         LCR_CHAR_ONE_STOP_BIT |
                         LCR_PARITY_BIT_ON;

    // MCR
    REG16_NPM(0x20D20) = 0x0000;
    REG16_NPM(0x20D20) = MCR_RTS_LOW_ACT | MCR_AFCE; //MCR_LOOPBACK;

    fuart_inited = TRUE;
    printf("fuart inited\n");
    return TRUE;
}

void fuart_callback(int c)
{
    int_count++;
    printf("callback\n");
}

static void fuart_setup(void)
{
    u32_fuart_handle = mdrv_uart_open(E_UART_PIU_FUART0);
    //mdrv_uart_set_baudrate(u32_fuart_handle, 115200); // we have already set baudrate in fuart_init()
    mdrv_uart_rx_enable(u32_fuart_handle);
    mdrv_uart_set_rx_callback(u32_fuart_handle, fuart_callback); // has no effect
    //MsOS_AttachInterrupt(E_INT_IRQ_UART1, (InterruptCb)fuart_isr);
    MsOS_AttachInterrupt(E_INT_IRQ_UART2MCU, (InterruptCb)fuart_isr);
    //MsOS_EnableInterrupt(E_INT_IRQ_UART1);
    MsOS_EnableInterrupt(E_INT_IRQ_UART2MCU);
}

void fuart_isr(void)
{
    int             c;
    int             iir;

    int_count++;

    iir = REG16_NPM(0x20D10); // IIR
    if (!(iir & UART_IIR_NO_INT))
    {
        switch (iir & UART_IIR_ID) {
        case UART_IIR_RLSI:
            // fall through
        case UART_IIR_RDI:
            // Rx Data Interrupt
            //while (UART_REG8(UART_LSR) & UART_LSR_DR) {
            while (REG16_NPM(0x20D28) & UART_LSR_DR) {
                //c = UART_REG8(UART_RX);
                c = REG16_NPM(0x20D00);
                rx_ring_buff[rx_buff_index++] = c;
                if (rx_buff_index >= RING_BUFF_SIZE)
                    rx_buff_index = 0;
#if 0
                if (!(u->rx_flags & RX_FLAG_DISABLED))
                {
                    if (u->rx_buf)
                    {
                        u->rx_buf[u->rx_buf_in++] = c;
                        if (u->rx_buf_in >= u->rx_buf_len)
                        {
                            u->rx_buf_in = 0;
                        }
                        if (u->rx_buf_in == u->rx_buf_out)
                        {
                            u->rx_flags |= RX_FLAG_OVERFLOW;
                        }
                    }
                }
#endif
            }
            // fall through
#if 0 // ignore Tx for now
        case UART_IIR_THRI:
            tx_int++;
            if (tx_buff_index < sizeof(test_data)) {
                // finish off incompleted data transfer
                while (REG16_NPM(0x20D38 & 0x01))
                    // wait till device is not busy
                    ;
                while (REG16_NPM(0x20D38) & 0x02) {
                    // if TX FIFO is not full, keep writing
                    REG16_NPM(0x20D00) = test_data[tx_buff_index++];
                    tx_int_cnt++;
                }
            }
#if 0
            if (u->tx_buf_in == u->tx_buf_out) {
                u->tx_flags |= TX_FLAG_DIRECT;
            }
            else {
                while ((UART_REG8(UART_LSR) & UART_LSR_THRE)
                    && (u->tx_buf_in != u->tx_buf_out))
                {
                    c = u->tx_buf[u->tx_buf_out++];
                    if (!(u->tx_flags & TX_FLAG_DISABLED)) {
                        UART_REG8(UART_TX) = c;
                        if (u->tx_buf_out >= u->tx_buf_len) {
                            u->tx_buf_len = 0;
                        }
                    }
                }
            }
#endif
#endif
        }
    }
    //MsOS_EnableInterrupt(E_INT_IRQ_UART1);
    MsOS_EnableInterrupt(E_INT_IRQ_UART2MCU);
}

static void initiate_tx(MS_U8 * tx_data, MS_U32 len)
{
    printf("Make sure CTS is pull LOW. (either by short CTS and RTS pins or connect CTS to ground)\n");
    while (tx_buff_index < len) {
        while (REG16_NPM(0x20D38) & 0x01);
        while ((REG16_NPM(0x20D38) & 0x02) && (tx_buff_index < len)) {
            // only write when FIFO is not full AND device is not busy
            REG16_NPM(0x20D00) = tx_data[tx_buff_index++];
        }
    }
}

MS_BOOL demo_fast_uart_test(MS_U32 *arg1)
{
    MS_U32 bad_byte = 0;
    int i;

    if (fuart_init() == FALSE)
        return FALSE;

    fuart_setup();

    tx_buff_index = 0;
    memset(rx_ring_buff, 0, sizeof(rx_ring_buff));

    // initiate transfer till buffer is full then ISR takes over the rest of the transmission.
    //initiate_tx(test_data, sizeof(test_data), &tx_buff_index);
    initiate_tx(test_data, sizeof(test_data));

    // do we need to wait for few second here?
    printf("total interrupt count = %d\n", (int)int_count);
    printf("tx_buff_index = %d\n", tx_buff_index);

    for (i = 0; i < sizeof(rx_ring_buff); i++) {
        printf("%0.2x %c= %0.2x, ", (unsigned char)rx_ring_buff[i],
                                    (unsigned char)rx_ring_buff[i] == (unsigned char)test_data[i]?'=':'!',
                                    (unsigned char)test_data[i]);
        if (rx_ring_buff[i] != test_data[i]) {
            bad_byte++;
        }

        if ((i & 0xf) == 0xf)
            printf("\n");
    }
    printf("\nData compare ");
    if (bad_byte) {
        printf("failed, bad byte = %d\n", bad_byte);
        return FALSE;
    } else {
        printf("OK!\n");
        return TRUE;
    }
}
#endif // (DEMO_FUART_TEST == 1)
