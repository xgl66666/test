mkdir -p ../sboot/out

#
# Unmark below to sync code for s-boot build standalone (i.e. without u-boot)
#

# KERNEL=../../RedLion/2.6.26
# U_BOOT=../u-boot-1.1.6
# cp tools/mkimage ${KERNEL}/arch/mips/boot
# cp tools/minilzo/lzo ${KERNEL}/arch/mips/boot
# cp -r ${U_BOOT}/include/configs ../sboot/inc
# cp -r ${KERNEL}/arch/mips/mips-boards/titania/board ../sboot/inc/titania
# cp -r ${KERNEL}/arch/mips/mips-boards/titania2/board ../sboot/inc/titania2

cp u-boot.bin ../sboot/out
cp u-boot.map ../sboot/out
cd ../sboot
./load_uboot.sh
cd ../u-boot-1.1.6/