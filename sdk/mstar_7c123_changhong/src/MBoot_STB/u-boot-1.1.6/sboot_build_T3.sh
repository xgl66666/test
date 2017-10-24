mkdir -p ../sboot/out

cp u-boot.bin ../sboot/out
cp u-boot.map ../sboot/out
cd ../sboot
./load_T3_uboot.sh
cd ../u-boot-1.1.6/