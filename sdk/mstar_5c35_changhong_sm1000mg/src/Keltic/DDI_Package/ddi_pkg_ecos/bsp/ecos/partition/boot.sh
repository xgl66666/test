#!/bin/bash

APP_BIN=../../../cus_mstar/project/mstar_demo/bin/kappa_ecos.bin
ZIP_BIN=../tftp_root/kappa_ecos.zip
PARAFILE=../tftp_root/parameters.txt

App_Dram_Address=0x80000180
#App_Dram_Address=2147484032

App_Reset_Vector_Dram_Address=0x80000224
#App_Reset_Vector_Dram_Address=2147484196


LZMA_BUF_Address=0x80B00000
#LZMA_BUF_Address=2159017984

LZMA_Dec_BUF_Address=0x81000000
#LZMA_Dec_BUF_Address=2164260864


function hex()
{
	printf '0x%08x' $1
}


echo -e "lzma the app bin file \a \n"

UNAME=`uname`

printf 'UNAME= %s \n' $UNAME


if [ "$UNAME" == "Linux" ]; then
        ./crc -a $APP_BIN $APP_BIN
else
        printf '%04X' 0x0  >> $APP_BIN
        ADDR_CRC=`stat -c '%s' $APP_BIN`
        ADDR_CRC=$(($ADDR_CRC - 4))
        printf 'ADDR_CRC= %08X \n' $ADDR_CRC
        ./CRC32.exe -SetCRC32 $ADDR_CRC $APP_BIN
fi

if [ "$UNAME" == "Linux" ]; then
        ./lzma e $APP_BIN $ZIP_BIN
else
        ./lzma.exe e $APP_BIN $ZIP_BIN
fi


# calculate the dvbc_u4.zip size
SIZE_APP=$(hex `stat -c %s $ZIP_BIN`)


echo "Generate $PARAFILE"
> $PARAFILE
echo "# This file was auto generated" >> $PARAFILE
echo >> $PARAFILE
echo "setenv SIZE_APP $SIZE_APP" >> $PARAFILE
echo "setenv App_Dram_Address $App_Dram_Address" >> $PARAFILE
echo "setenv App_Reset_Vector_Dram_Address $App_Reset_Vector_Dram_Address" >> $PARAFILE
echo "setenv LZMA_BUF_Address $LZMA_BUF_Address" >> $PARAFILE
echo "setenv LZMA_Dec_BUF_Address $LZMA_Dec_BUF_Address" >> $PARAFILE
echo >> $PARAFILE
echo "% <- this is end of file symbol" >> $PARAFILE

