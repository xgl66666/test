#!/bin/bash


source ./cfg_board.inc

#Temp use XCXX
if [ "$CFG_BOARD" == "BD_KIRIN_FPGA" ]; then
cp .config.kirin.fpga .config
elif [ "$CFG_BOARD" == "BD_MST221A_D01A_S" ]; then
cp .config.kirin_221A_D01A_S_XCXX.DDI .config
elif [ "$CFG_BOARD" == "BD_MST221A_D01A" ]; then
cp .config.kirin_221A_D01A_XCXX.DDI .config
elif [ "$CFG_BOARD" == "BD_MST221B_D01A_S" ]; then
cp .config.kirin_221B_D01A_S_XCXX.DDI .config
elif [ "$CFG_BOARD" == "BD_MST221B_D01A" ]; then
cp .config.kirin_221B_D01A_XCXX.DDI .config
else
echo "no match board definition ~~ "
exit 1
fi

echo "Board config ",$CFG_BOARD
echo "Chip config ",$CFG_CHIP_SELECTION

unset CPU

make oldconfig TARGET_UBOOT_VERSION=0 > /dev/null
cp ../../cus_mstar/project/mstar_demo/bin/kirin_ecos.elf kirin_ecos.elf
make clean;make
cp bin/flash_with_ecos.bin flash_kirin_ecos.bin
