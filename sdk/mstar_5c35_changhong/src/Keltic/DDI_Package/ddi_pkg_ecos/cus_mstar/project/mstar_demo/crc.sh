#!/bin/bash
FLASH_BIN=./bin/keltic_ecos.bin
UNAME=`uname`
printf 'UNAME= %s \n' $UNAME
if [ "$UNAME" == "Linux" ]; then
	./../../../boot/mboot/crc -a $FLASH_BIN $FLASH_BIN
else
	printf '%04X' 0x0  >> $FLASH_BIN
	ADDR_CRC=`stat -c '%s' $FLASH_BIN`
	ADDR_CRC=$(($ADDR_CRC - 4))
	printf 'ADDR_CRC= %08X \n' $ADDR_CRC
	./../../../boot/mboot/CRC32.exe -SetCRC32 $ADDR_CRC $FLASH_BIN
fi
	
