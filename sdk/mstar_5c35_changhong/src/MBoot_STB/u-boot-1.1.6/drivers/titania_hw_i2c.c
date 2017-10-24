
#include <common.h>

//#if defined(CONFIG_URANUS4)
#if 0
#include "titania_hw_i2c.h"

extern void MDrv_IIC_Init(void);
//void MHal_IIC_Start(void);
//void MHal_IIC_Stop(void);
//void MHal_IIC_NoAck(void);
//B16 MHal_IIC_SendData(U8 u8DataIIC);
//B16 MHal_IIC_SendByte(U8 u8DataIIC);
//B16 MHal_IIC_GetByte(U8* pu8DataIIC);
extern S32 MDrv_HW_IIC_Write(U8 u8SlaveIdIIC, U8 *pu8AddrIIC, U8 u8AddrSizeIIC, U8 *pu8BufIIC, U32 u32BufSizeIIC);
extern S32 MDrv_HW_IIC_Read(U8 u8SlaveIdIIC, U8 *pu8AddrIIC, U8 u8AddrSizeIIC, U8 *pu8BufIIC, U32 u32BufSizeIIC);

void i2c_init(void)
{
    MDrv_IIC_Init();
}

#if 0
static int i2c_read_byte (u8 devaddr, u8 regoffset, u8 * value)
{

}

static int i2c_write_byte (u8 devaddr, u8 regoffset, u8 value)
{

}
#endif

int i2c_probe (uchar chip)
{
   return 0;
}

//int i2c_read (uchar chip, uint * addr, int alen, uchar * buffer, int len)
void i2c_read (uchar chip, uchar * addr, uchar alen, uchar * buffer, uint len)
{
    MDrv_HW_IIC_Read(chip, addr, alen, buffer, len);
}

//int i2c_write (uchar chip, uint * addr, int alen, uchar * buffer, int len)
void i2c_write (uchar chip, uchar * addr, uchar alen, uchar * buffer, uint len)
{
    MDrv_HW_IIC_Write(chip, addr, alen, buffer, len);
}

void MDrv_IIC_Init(void)
{
    //set all pads (except SPI) as input
    MHal_CHIP_REG(REG_IIC_MODE) &= ~BIT15;
    //iic master mode is enabled, bit 9~8 set to 1
    MHal_CHIP_REG(REG_IIC_MODE) &= ~BIT9;
    MHal_CHIP_REG(REG_IIC_MODE) |= BIT8;

    // clock = (Xtal / 4T) / div
    MHal_IIC_REG(REG_IIC_CLK_SEL) = 3; // 28.8M / 4 / 16 = 450KHz
    MHal_IIC_REG(REG_IIC_CTRL) = 0x80; // enable IIC
}

void MHal_IIC_Start(void)
{
    //reset iic circuit
    MHal_IIC_REG(REG_IIC_STATUS) = 0x08;
    MHal_IIC_REG(REG_IIC_STATUS) = 0x00;

    //reset iic circuit
    MHal_IIC_REG(REG_IIC_CTRL) = 0xC0;
}


void MHal_IIC_Stop(void)
{
    MHal_IIC_REG(REG_IIC_CTRL) = 0xA0;
    MHal_IIC_DELAY();
}

void MHal_IIC_NoAck(void)
{
    MHal_IIC_REG(REG_IIC_CTRL) = 0x90;
}

B16 MHal_IIC_SendData(U8 u8DataIIC)
{
    MHal_IIC_REG(REG_IIC_STATUS) = 0x02; /* clear interrupt */
    MHal_IIC_REG(REG_IIC_WDATA) = u8DataIIC;

    {   //tune the dalay
        U8 u8Delay;
        for(u8Delay=0; u8Delay<100; u8Delay++);
        udelay(1000);
    }

    while (1)
    {
        if (MHal_IIC_REG(REG_IIC_STATUS) & 0x01)
        {
            MHal_IIC_REG(REG_IIC_STATUS) = 0x02; /* clear interrupt */
            if (MHal_IIC_REG(REG_IIC_CTRL) & 0x08) /* no ACK */
            {
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

B16 MHal_IIC_SendByte(U8 u8DataIIC)
{
    U8 u8I;

    for(u8I=0;u8I<10;u8I++)
    {
        if(MHal_IIC_SendData(u8DataIIC) == TRUE)
            return TRUE;
    }
    return FALSE;
}

B16 MHal_IIC_GetByte(U8* pu8DataIIC) /* auto generate ACK */
{

    /* clear interrupt & start byte reading */
    MHal_IIC_REG(REG_IIC_STATUS) = 0x04;

    {   //tune the dalay
        U8 u8Delay;
        for(u8Delay=0; u8Delay<100; u8Delay++);
        udelay(1000);
    }

    while(1)
    {
        if (MHal_IIC_REG(REG_IIC_STATUS))
        {
            MHal_IIC_REG(REG_IIC_STATUS) = 0x02; /* clear interrupt */
            *pu8DataIIC = MHal_IIC_REG(REG_IIC_RDATA);
            return TRUE;
        }
    }

    return FALSE;
}

S32 MDrv_HW_IIC_Write(U8 u8SlaveIdIIC, U8 *pu8AddrIIC, U8 u8AddrSizeIIC, U8 *pu8BufIIC, U32 u32BufSizeIIC)
{
    U32     u32I;
    U8      u8I;
    B16     bReturnIIC = TRUE;
    S32     s32RetCountIIC = -1;

    MHal_IIC_Start();

    if (FALSE == MHal_IIC_SendByte(u8SlaveIdIIC&0xFE))
    {
        bReturnIIC = FALSE;
        s32RetCountIIC = -2;
        goto HW_IIC_Write_End;
    }

    for (u8I = 0; u8I < u8AddrSizeIIC; u8I++)
    {
        if (FALSE == MHal_IIC_SendByte(pu8AddrIIC[u8I]))
        {
            bReturnIIC = FALSE;
            s32RetCountIIC = -3;
            goto HW_IIC_Write_End;
        }
    }

    for (u32I = 0; u32I < u32BufSizeIIC; u32I++)
    {
        if (FALSE == MHal_IIC_SendByte(pu8BufIIC[u32I]))
        {
            bReturnIIC = FALSE;
            s32RetCountIIC = -4;
            goto HW_IIC_Write_End;
        }
    }

    s32RetCountIIC = u32BufSizeIIC;

HW_IIC_Write_End:
    MHal_IIC_Stop();
    return s32RetCountIIC;
}

S32 MDrv_HW_IIC_Read(U8 u8SlaveIdIIC, U8 *pu8AddrIIC, U8 u8AddrSizeIIC, U8 *pu8BufIIC, U32 u32BufSizeIIC)
{
    U32     u32I;
    U8      u8I;
    B16     bReturnIIC = TRUE;
    S32     s32RetCountIIC = -1;

    MHal_IIC_Start();

    if (FALSE == MHal_IIC_SendByte(u8SlaveIdIIC&0xFE))
    {
        bReturnIIC = FALSE;
        s32RetCountIIC = -2;
        goto HW_IIC_Read_End;
    }

    for (u8I = 0; u8I < u8AddrSizeIIC; u8I++)
    {
        if (FALSE == MHal_IIC_SendByte(pu8AddrIIC[u8I]))
        {
            bReturnIIC = FALSE;
            s32RetCountIIC = -3;
            goto HW_IIC_Read_End;
        }
    }

    MHal_IIC_Start();

    if (FALSE == MHal_IIC_SendByte(u8SlaveIdIIC|0x1))
    {
        bReturnIIC = FALSE;
        s32RetCountIIC = -4;
        goto HW_IIC_Read_End;
    }

    for (u32I = 0; u32I < u32BufSizeIIC; u32I++)
    {
        if (u32I == (u32BufSizeIIC-1))
        {
            MHal_IIC_NoAck();
        }

        if (FALSE == MHal_IIC_GetByte(&pu8BufIIC[u32I]))
        {
            bReturnIIC = FALSE;
            s32RetCountIIC = -5;
            goto HW_IIC_Read_End;
        }
    }

    s32RetCountIIC = u32BufSizeIIC;

HW_IIC_Read_End:
    MHal_IIC_Stop();
    return s32RetCountIIC;
}



#endif
