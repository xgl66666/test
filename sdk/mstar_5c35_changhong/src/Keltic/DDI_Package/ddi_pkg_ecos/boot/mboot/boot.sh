#!/bin/bash

source ./boot.inc
source ./cfg_board.inc

mkdir -p out



BOOT_INC=./out/boot1.inc
MBOOT_TEMP_BIN=./out/tempM.bin
APP_TEMP_BIN=./out/tempA.bin
FLASH_BIN=./out/flash.bin
PARAFILE=../../bsp/ecos/usb_update/Target/script/parameters.txt

##MBOOT_BIN=./bin/mboot_"$CFG_BOARD"_$CFG_CHIP_SELECTION.bin
MBOOT_BIN=./bin/mboot_BD_MST183B_D01A_S_MSD5XX5_Nagra_K5L.bin


echo "MBOOT_BIN PATH=" $MBOOT_BIN

# calculate the mboot.bin size
SIZE_MBOOT=`stat -c '%s' $MBOOT_BIN`
printf "SIZE_MBOOT = " > $BOOT_INC
printf $SIZE_MBOOT  >> $BOOT_INC
printf "\n" >> $BOOT_INC


echo -e "Pading Mboot to size $Mboot_Align_size \a \n"

#PadingLen=$(($Mboot_Align_size - $SIZE_MBOOT))
echo "SIZE_MBOOT" $SIZE_MBOOT

PadingLen=$(($SIZE_MBOOT + 65535))
PadingLen=$(($PadingLen & 0xFFFF0000))

printf 'PadingLen= %08X \n' $PadingLen

App_in_SPI_Address=$(($PadingLen + 28))

PadingLen=$(($PadingLen - $SIZE_MBOOT))

printf "PadingLen = " >> $BOOT_INC
printf $PadingLen  >> $BOOT_INC
printf "\n" >> $BOOT_INC

perl ./PadingData.pl $MBOOT_BIN $MBOOT_TEMP_BIN $PadingLen

echo -e "Pading Header file to APP \a \n"

# calculate the dvbc_xx.zip size
SIZE_APP=`stat -c '%s' $ZIP_BIN`

printf "SIZE_APP = " >> $BOOT_INC
printf $SIZE_APP  >> $BOOT_INC
printf "\n" >> $BOOT_INC

printf "App_in_SPI_Address = " >> $BOOT_INC
printf $App_in_SPI_Address  >> $BOOT_INC
printf "\n" >> $BOOT_INC

#echo "Generate $PARAFILE"
> $PARAFILE
printf "# This file was auto generated\n" >> $PARAFILE
printf "setenv SIZE_APP 0x%x\n" $SIZE_APP >> $PARAFILE
printf "setenv App_Dram_Address 0x%x\n" $App_Dram_Address >> $PARAFILE
printf "setenv App_Reset_Vector_Dram_Address 0x%x\n" $App_Reset_Vector_Dram_Address >> $PARAFILE
printf "setenv LZMA_BUF_Address 0x%x\n" $LZMA_BUF_Address >> $PARAFILE
printf "setenv LZMA_Dec_BUF_Address 0x%x\n" $LZMA_Dec_BUF_Address >> $PARAFILE
printf "%% <- this is end of file symbol" >> $PARAFILE


#./addheader.pl     <in_file>    <out_file> <App Magic flag> <file len> <App Dram Address> <App Reset Vector Dram Address> <App in SPI Address> <LZMA BUF Address> <LZMA Dec BUF Address>

perl ./addheader.pl $ZIP_BIN $APP_TEMP_BIN $App_Magic_flag $SIZE_APP $App_Dram_Address $App_Reset_Vector_Dram_Address $App_in_SPI_Address $LZMA_BUF_Address $LZMA_Dec_BUF_Address

echo -e "generate flash.bin! \a \n"

cat $MBOOT_TEMP_BIN > $FLASH_BIN
cat $APP_TEMP_BIN >> $FLASH_BIN

UNAME=`uname`

printf 'UNAME= %s \n' $UNAME

if [ "$UNAME" == "Linux" ]; then
        ./crc -a $FLASH_BIN $FLASH_BIN
else
        printf '%04X' 0x0  >> $FLASH_BIN
        ADDR_CRC=`stat -c '%s' $FLASH_BIN`
        ADDR_CRC=$(($ADDR_CRC - 4))
        printf 'ADDR_CRC= %08X \n' $ADDR_CRC
        ./CRC32.exe -SetCRC32 $ADDR_CRC $FLASH_BIN
fi


rm -f $MBOOT_TEMP_BIN
rm -f $APP_TEMP_BIN
