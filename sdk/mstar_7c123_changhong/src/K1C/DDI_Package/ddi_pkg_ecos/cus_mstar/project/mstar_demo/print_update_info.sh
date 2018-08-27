#!/bin/sh
# This will print information of how to update image on 290C_D01A_S board 

if [ -f  ../../../bsp/ecos/tftp_root/parameters.txt ]; then
echo -e "\e[1;32;40m" ===== How to update image to 290C_D01A_S board? ====="\e[0m "
echo -e "\e[1;33;40m"     1. Program k1c_ecos_zip.bin to 0x100000 position on SPI!"\e[0m "
echo -e "\e[1;33;40m"     You can done that by setting Base shift to 0x100000 on ISP tool"\e[0m "
echo -e "\e[1;33;40m"     Note: You can find the k1c_ecos_zip.bin under ${PWD}/bin directory"\e[0m "
echo -e "\e[1;33;40m"     2. After programming k1c_ecos_zip.bin to SPI,enter the following command in uboot consol!"\e[0m "
paramfile="../../../bsp/ecos/tftp_root/parameters.txt"
exec < $paramfile
while read var
do
if [[ $var == *"setenv"* ]]; then
echo -e "\e[1;33;40m $var\e[0m "
fi
done
paramfile="../../../bsp/ecos/tftp_root/set_config"
exec < $paramfile
while read var
do
if [[ $var == *"bootcmd"* ]]; then
echo -e "\e[1;33;40m $var\e[0m "
fi
done
echo -e "\e[1;33;40m" saveenv"\e[0m "
echo -e "\e[1;33;40m" reset"\e[0m "
echo -e "\e[1;32;40m" =====                   Done                    ====="\e[0m "
fi
