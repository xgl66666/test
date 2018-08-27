#!/bin/bash
source ${AUTO_GEN_OUT}/var_list

function hex()
{
	printf '0x%08x' $1
}


echo -e "lzma the app bin file \a \n"

UNAME=`uname`
PWD=`pwd`
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
echo >> $PARAFILE
echo "% <- this is end of file symbol" >> $PARAFILE
