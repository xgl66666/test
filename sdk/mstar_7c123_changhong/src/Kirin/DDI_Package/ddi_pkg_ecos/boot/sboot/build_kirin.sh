#=============DDI Linux=============

make clean
make distclean

cp configs/kirin/.config.kirin_221A_D01A_S_XCXX.DDI.linux .config
echo "build kirin_221A_D01A_S_XCXX.DDI.linux .."
make oldconfig TARGET_UBOOT_VERSION=2
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kirin/DDI_Package/ddi_pkg_linux/bsp/linux/mboot_BD_MST221A_D01A_S_XCXX.bin

make clean
make distclean

cp configs/kirin/.config.kirin_221A_D01A_XCXX.DDI.linux .config
echo "build kirin_221A_D01A_XCXX.DDI.linux .."
make oldconfig TARGET_UBOOT_VERSION=2
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kirin/DDI_Package/ddi_pkg_linux/bsp/linux/mboot_BD_MST221A_D01A_XCXX.bin

make clean
make distclean

cp configs/kirin/.config.kirin_221B_D01A_S_XCXX.DDI.linux .config
echo "build kirin_221B_D01A_S_XCXX.DDI.linux .."
make oldconfig TARGET_UBOOT_VERSION=2
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kirin/DDI_Package/ddi_pkg_linux/bsp/linux/mboot_BD_MST221B_D01A_S_XCXX.bin

make clean
make distclean

cp configs/kirin/.config.kirin_221B_D01A_XCXX.DDI.linux .config
echo "build kirin_221B_D01A_XCXX.DDI.linux .."
make oldconfig TARGET_UBOOT_VERSION=2
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kirin/DDI_Package/ddi_pkg_linux/bsp/linux/mboot_BD_MST221B_D01A_XCXX.bin

#=============DDI eCos=============

make clean
make distclean

cp configs/kirin/.config.kirin_221A_D01A_S_XCXX.DDI .config
echo "build kirin_221A_D01A_S_XCXX.DDI .."
make oldconfig TARGET_UBOOT_VERSION=2
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kirin/DDI_Package/ddi_pkg_ecos/bsp/ecos/Kirin_Mbootbin/mboot_BD_MST221A_D01A_S_XCXX.bin

make clean
make distclean

cp configs/kirin/.config.kirin_221A_D01A_XCXX.DDI .config
echo "build kirin_221A_D01A_XCXX.DDI .."
make oldconfig TARGET_UBOOT_VERSION=2
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kirin/DDI_Package/ddi_pkg_ecos/bsp/ecos/Kirin_Mbootbin/mboot_BD_MST221A_D01A_XCXX.bin

make clean
make distclean

cp configs/kirin/.config.kirin_221B_D01A_S_XCXX.DDI .config
echo "build kirin_221B_D01A_S_XCXX.DDI .."
make oldconfig TARGET_UBOOT_VERSION=2
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kirin/DDI_Package/ddi_pkg_ecos/bsp/ecos/Kirin_Mbootbin/mboot_BD_MST221B_D01A_S_XCXX.bin

make clean
make distclean

cp configs/kirin/.config.kirin_221B_D01A_XCXX.DDI .config
echo "build kirin_221B_D01A_XCXX.DDI .."
make oldconfig TARGET_UBOOT_VERSION=2
make clean;make
cp bin/flash_with_uboot.bin ../../../../THEALE/Uranus/Kirin/DDI_Package/ddi_pkg_ecos/bsp/ecos/Kirin_Mbootbin/mboot_BD_MST221B_D01A_XCXX.bin
