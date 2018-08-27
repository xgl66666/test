#!/bin/sh
# This will patch boot/sboot which setup eCos as 2nd loader environmnet

if [ -f cfg_board.inc ]; then
    source ./cfg_board.inc

    if [ -d "../../../boot/sboot" ]; then
        #patch the sboot
        sed -i 's/source \"UConfig.in\"//g' ../../../boot/sboot/Config.in

        find ../../../boot/sboot/configs/${CHIP} -type f -exec sed -i 's/CONFIG_UBOOT_AS_2ED_LOADER=y/# CONFIG_UBOOT_AS_2ED_LOADER is not set/g' {} \;
        find ../../../boot/sboot/configs/${CHIP} -type f -exec sed -i 's/# CONFIG_ECOS_AS_2ED_LOADER is not set/CONFIG_ECOS_AS_2ED_LOADER=y/g' {} \;
        find ../../../boot/sboot/configs/${CHIP} -type f -exec sed -i 's/CONFIG_ECOS_ELF_PATH=\"\"/CONFIG_ECOS_ELF_PATH=\"'${CHIP}'_ecos.elf\"/g' {} \;

        if [ -f bin/${CHIP}_ecos.elf ]; then
            #copy ecos to sboot
            cp bin/${CHIP}_ecos.elf ../../../boot/sboot/
        else
            echo "[Error] Please make the DDI first."
        fi

    else
        echo "[Error] Please use repo or git to donwload sboot."
    fi
else
    echo "[Error] Please make the DDI first."
fi
