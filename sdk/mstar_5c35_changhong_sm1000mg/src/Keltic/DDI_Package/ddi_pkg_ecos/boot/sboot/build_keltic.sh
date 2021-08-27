#!/bin/bash


source ./cfg_board.inc

cp .config.keltic_"$CFG_BOARD"_$CFG_CHIP_SELECTION.DDI .config

echo "Board config ",$CFG_BOARD
echo "Chip config ",$CFG_CHIP_SELECTION


make oldconfig
cp ../../cus_mstar/project/mstar_demo/bin/keltic_ecos.elf keltic_ecos.elf
make clean;make
cp bin/flash_with_ecos.bin flash_keltic_ecos.bin