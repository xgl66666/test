#ifndef __DIBCOM_I2C_H_
#define __DIBCOM_I2C_H_

MS_U8 dib8000_read(MS_U16 subaddr, MS_U8 *buf, int len);
MS_U8 dib8000_write(MS_U16 subaddr, MS_U8 *buf, int len);
MS_U8 dib8000_write_word( MS_U16 subaddr, MS_U16 value );
MS_U16 dib8000_read_word(MS_U16 subaddr );
MS_U32 dib8000_read32( MS_U16 subaddr );
MS_U8 dib0090_write_reg( MS_U8 subaddr, MS_U16 value );
MS_U16 dib0090_read_reg(MS_U8 subaddr );
MS_U8 dib0090_write_regs( MS_U8 subaddr, MS_U16 *i2c_tx_buf, int num );


#endif
