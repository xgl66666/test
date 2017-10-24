#include "platform_dibcom.h"
#include "dibcom_i2c_app.h"
#include <sys/common/MsCommon.h>
#include <sys/drv/drvIIC.h>

extern MS_U8 g_debugi2cdata;


#define DIBCOM_I2C_MASTER_BASE_REGISTER     1024
#define DIB_RETURN_SUCCESS                   0
#define DIB_RETURN_ERROR                    -1
#define DIB_RETURN_NOT_SUPPORTED            -2

#define MDrv_IIC_Read(a, b, c, d, e) MDrv_IIC_ReadBytes(a, c, b, e, d)
#define MDrv_IIC_Write(a, b, c, d, e) MDrv_IIC_WriteBytes(a, c, b, e, d)

MS_U8 dib8000_read(MS_U16 subaddr, MS_U8 *i2c_rx_buf, int len)
{
    MS_U8 wb[2], i=0;

    wb[0] = (MS_U8 )((subaddr >> 8) & 0x0f);
    wb[1] = (MS_U8 )((subaddr   ) & 0xff);


    if(MDrv_IIC_Read(0x12, wb,2, i2c_rx_buf, len) == FALSE)
    {
        DBG_DIB8096("dibcom read error\n");
        return 1;
    }

    if(g_debugi2cdata)
    {
        Out(printf("-R- "));
        Out(printf("0x12] "));
        for(i=0;i<2;i++)
        {
            Out(printf("[w 0x%02x]", wb[i]));
        }
        for(i=0;i<2;i++)
        {
            Out(printf(" [r 0x%02x]", i2c_rx_buf[i]));
        }

        Out(printf(" [%d: ", (wb[0]<<8)|wb[1]));
        Out(printf(" %d]", (i2c_rx_buf[0]<<8)|i2c_rx_buf[1]));
        Out(printf("\n"));
    }

    return 0;
}

MS_U8 dib8000_write(MS_U16 subaddr, MS_U8 *buf, int len)
{
    MS_U8 wb[16], i;

    wb[0] = (MS_U8 )((subaddr >> 8) & 0x0f);
    wb[1] = (MS_U8 )((subaddr   ) & 0xff);

    for(i = 0; i < len; i++)
    {
        wb[2+i] = buf[i];
    }

    if(g_debugi2cdata)
    {
        Out(printf("-W- "));
        Out(printf("0x12 "));
        Out(printf("[%ld]", len+2));
        for(i=0;i<4;i++)
        {
            Out(printf(" 0x%02x ", wb[i]));
        }
        Out(printf(" [%d: %d]", (wb[0]<<8)|wb[1], (wb[2]<<8)|wb[3]));
        Out(printf("\n"));
    }
    if(MDrv_IIC_Write(0x12, NULL,0, wb, len+2) == FALSE)
    {
        DBG_DIB8096("dibcom write error\n");
        return 1;
    }

    return 0;
}

MS_U8 dib8000_write_word( MS_U16 subaddr, MS_U16 value )
{
    MS_U8 wb[4], i;

    wb[0] = (MS_U8 )((subaddr >> 8) & 0x0f);
    wb[1] = (MS_U8 )((subaddr   ) & 0xff);

    wb[2] = (MS_U8 )((value>>8) & 0xff);
    wb[3] = (MS_U8 )((value) & 0xff);


    //memcpy(&wb[2], &value, 2);

    if(g_debugi2cdata)
    {
        Out(printf("-W- "));
        Out(printf("0x12] "));
        Out(printf("[4]"));
        for(i=0;i<4;i++)
        {
            Out(printf(" 0x%02x ", wb[i]));
        }
        Out(printf(" [%d: %d]", (wb[0]<<8)|wb[1], (wb[2]<<8)|wb[3]));
        Out(printf("\n"));
    }

    if(MDrv_IIC_Write(0x12, NULL,0, wb, 4) == FALSE)
    {
        DBG_DIB8096("dib demod write error\n");
        return 1;
    }

    return 0;
}

MS_U16 dib8000_read_word(MS_U16 subaddr )
{
    MS_U8 i, i2c_rx_buf[2], wb[2];

    wb[0] = (MS_U8)((subaddr >> 8) & 0x0f);
    wb[1] = (MS_U8)( subaddr   & 0xff);


    if(MDrv_IIC_Read(0x12, wb,2, i2c_rx_buf, 2) == FALSE)
    {
        DBG_DIB8096("dibcom read error\n");

        return 1;
    }

    if(g_debugi2cdata)
    {
        Out(printf("-R- "));
        Out(printf("0x12] "));
        for(i=0;i<2;i++)
        {
            Out(printf("[w 0x%02x]", wb[i]));
        }
        for(i=0;i<2;i++)
        {
            Out(printf(" [r 0x%02x]", i2c_rx_buf[i]));
        }

        Out(printf(" [%d: ", (wb[0]<<8)|wb[1]));
        Out(printf(" %d]", (i2c_rx_buf[0]<<8)|i2c_rx_buf[1]));
        Out(printf("\n"));
    }

    return ((i2c_rx_buf[0] << 8) | i2c_rx_buf[1]);
}

MS_U32 dib8000_read32(MS_U16 subaddr)
{
    MS_U8 i, i2c_rx_buf[4], wb[2];

    wb[0] = (MS_U8)((subaddr >> 8) & 0x0f);
    wb[1] = (MS_U8)( subaddr   & 0xff);


    if(MDrv_IIC_Read(0x12, wb,2, i2c_rx_buf, 4) == FALSE)
    {
        DBG_DIB8096("dibcom read error\n");

        return 1;
    }

    if(g_debugi2cdata)
    {
        Out(printf("-R- "));
        Out(printf("0x12] "));
        for(i=0;i<2;i++)
        {
            Out(printf("[w 0x%02x]", wb[i]));
        }
        for(i=0;i<4;i++)
        {
            Out(printf(" [r 0x%02x]", i2c_rx_buf[i]));
        }

        Out(printf(" [%d: ", (wb[0]<<8)|wb[1]));
        Out(printf(" %d]", (i2c_rx_buf[0]<<8)|i2c_rx_buf[1]));
        Out(printf("\n"));
    }

    return ((i2c_rx_buf[0] << 24) |(i2c_rx_buf[1] << 16) |(i2c_rx_buf[2] << 8) | i2c_rx_buf[3] );
}

static int dibx0090_is_i2c_done(void)
{
    int i = 100; // max_i2c_polls;
    MS_U16 status;

    while (((status = dib8000_read_word(DIBCOM_I2C_MASTER_BASE_REGISTER + 2)) & 0x0100) == 0 && --i > 0);

    /* i2c timed out */
    if (i == 0)
        return DIB_RETURN_ERROR;

    /* no acknowledge */
    if ((status & 0x0080) == 0)
        return DIB_RETURN_ERROR;

    return DIB_RETURN_SUCCESS;
}

MS_U8 dib0090_write_reg( MS_U8 subaddr, MS_U16 value )
{
    MS_U8 wb[3], i;
	MS_U16 data;
	MS_U16 da;
	MS_U16 txlen, len, outlen;

    wb[0] = subaddr;
    wb[1] = (MS_U8 )((value>>8 ) & 0xff);
    wb[2] = (MS_U8 )((value ) & 0xff);
	txlen = 3;
	outlen = 3;

	while (txlen) {
		dib8000_read_word(DIBCOM_I2C_MASTER_BASE_REGISTER + 2);	 // reset fifo ptr  	//data = *wb++ << 8;


		len = txlen > 8 ? 8 : txlen;
		for (i = 0; i < len; i += 2) {
			data= (MS_U16)wb[i];
			data<<=8;

  		if (i+1 < len)
				data |= wb[i+1];
			dib8000_write_word(DIBCOM_I2C_MASTER_BASE_REGISTER, data);
		}
		da = ((0xC0 >> 1) << 9) | // addr
			 (1 		  << 8) | // master
			 (1 		  << 7) | // rq
			 (0 		  << 6) | // stop
			 (0 		  << 5) | // start
			 ((len & 0x7) << 2) | // nb 8 bytes == 0 here
			 (0 		  << 1) | // rw
			 (0 		  << 0);  // irqen

		if (txlen == outlen)
			da |= 1 << 5; /* start */

		if (txlen-len == 0)
			da |= 1 << 6; /* stop */

		dib8000_write_word(DIBCOM_I2C_MASTER_BASE_REGISTER+1, da);

		if (dibx0090_is_i2c_done() != DIB_RETURN_SUCCESS)
			return DIB_RETURN_ERROR;
		txlen -= len;
	}
//	printf("\r\n [dib0090_write_reg] the subaddr = %02x,value=%04x",subaddr,value);

	return DIB_RETURN_SUCCESS;
}

MS_U16 dib0090_read_reg(MS_U8 subaddr )
{
    MS_U8 i, wb[3];
	MS_U16 data;
	MS_U16 da;
	MS_U16 txlen, len, rxlen;
	MS_U16 inlen,outlen;

    wb[0] = subaddr;
	txlen = 1;
	outlen = 1;


    dib8000_read_word(DIBCOM_I2C_MASTER_BASE_REGISTER + 2);	 // reset fifo ptr

	while (txlen) {
		dib8000_read_word(DIBCOM_I2C_MASTER_BASE_REGISTER + 2);	 // reset fifo ptr

		len = txlen > 8 ? 8 : txlen;
		for (i = 0; i < len; i += 2) {

			data = wb[i];
			data <<= 8;

			if (i+1 < len)
				data |= wb[i+1];
			dib8000_write_word(DIBCOM_I2C_MASTER_BASE_REGISTER, data);
		}
		da = ((0xC0 >> 1) << 9) | // addr
			 (1 		  << 8) | // master
			 (1 		  << 7) | // rq
			 (0 		  << 6) | // stop
			 (0 		  << 5) | // start
			 ((len & 0x7) << 2) | // nb 8 bytes == 0 here
			 (0 		  << 1) | // rw
			 (0 		  << 0);  // irqen

		if (txlen == outlen)
			da |= 1 << 5; /* start */

#if 0
		if (txlen-len == 0 && stop)
			da |= 1 << 6; /* stop */
#endif

		dib8000_write_word(DIBCOM_I2C_MASTER_BASE_REGISTER+1, da);

		if (dibx0090_is_i2c_done() != DIB_RETURN_SUCCESS)
			return DIB_RETURN_ERROR;
		txlen -= len;
	}

	rxlen = 2;
	inlen = 2;
    while (rxlen) {
        len = rxlen > 8 ? 8 : rxlen;
        da = ((0xC0 >> 1) << 9) | // addr
             (1           << 8) | // master
             (1           << 7) | // rq
             (0           << 6) | // stop
             (0           << 5) | // start
             ((len & 0x7) << 2) | // nb
             (1           << 1) | // rw
             (0           << 0);  // irqen

        if (rxlen == inlen)
            da |= 1 << 5; /* start */

        if (rxlen-len == 0)
            da |= 1 << 6; /* stop */
        dib8000_write_word(DIBCOM_I2C_MASTER_BASE_REGISTER+1, da);

        if (dibx0090_is_i2c_done() != DIB_RETURN_SUCCESS)
            return DIB_RETURN_ERROR;

        rxlen -= len;

        while (len) {
            da = dib8000_read_word(DIBCOM_I2C_MASTER_BASE_REGISTER);
			len=len-2;
			/*
	            *i2c_rx_buf++ = (da >> 8) & 0xff;
	            len--;
	            if (len >= 1) {
	                *i2c_rx_buf++ =  da   & 0xff;
	                len--;*/
            }
        }
//	printf("\r\n [dib0090_read_reg] the subaddr = %02x,da=%04x",subaddr,da);

    return da;
    //return ((i2c_rx_buf[0] << 8) | i2c_rx_buf[1]);
}

MS_U8 dib0090_write_regs( MS_U8 subaddr, MS_U16 *i2c_rx_buf, int num )
{
    int i;
    for(i= 0; i< num; i++)
    {
        if(0 != dib0090_write_reg(subaddr + i, i2c_rx_buf[i]))
        {
            return 1;
        }
    }
    return 0;
}

