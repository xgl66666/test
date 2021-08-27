
FLASH_BIN=./out/flash.bin
FLASH_TEMP_BIN=./out/tempA.bin
LOGO=./Sunset.jpg

perl ./ChangeLogo.pl $FLASH_BIN $LOGO $FLASH_TEMP_BIN

rm -f $FLASH_BIN
cp $FLASH_TEMP_BIN $FLASH_BIN


crc -a $FLASH_BIN $FLASH_BIN

rm -f $FLASH_TEMP_BIN
