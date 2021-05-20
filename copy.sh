#!/bin/bash

if [ $# -ne 1 ] ; then
	echo -e "usage: source copy.sh"
	echo -e "        for example: source copy.sh dal"
else

BIN_DIR=$1
ROOT_DIR=./
BUILD_DIR=${ROOT_DIR}build
TOOLS_DIR=./boot_5c35_tools
OUT_DIR=out
SDK_DIR=sdk/mstar_5c35_changhong/src/Keltic/DDI_Package/ddi_pkg_ecos

#copy platform libs
copy_file()
{
	#create dirs
	rm -rvf ${OUT_DIR}/${BIN_DIR}
	mkdir -p ${OUT_DIR}/${BIN_DIR}
	
	#copy file

	cp -rvf ${SDK_DIR}/boot/mboot/out/flash.bin ${OUT_DIR}/${BIN_DIR}
	cp -rvf ${SDK_DIR}/cus_mstar/project/mstar_demo/bin/keltic_ecos.bin ${OUT_DIR}/${BIN_DIR}
	cp -rvf ${BUILD_DIR}/bin/comedia.out ${OUT_DIR}/${BIN_DIR}
	cp -rvf ${BUILD_DIR}/bin/resources/resources.lzma ${OUT_DIR}/${BIN_DIR}
	cp -rvf ${TOOLS_DIR}/crc ${OUT_DIR}/${BIN_DIR}
	cp -rvf ${TOOLS_DIR}/lzma ${OUT_DIR}/${BIN_DIR}
	#cp -rvf ${SDK_DIR}/boot/mboot/bin/mboot_BD_MST183B_D01A_S_MSD5XX5_Nagra_K5L.bin ${OUT_DIR}/${BIN_DIR}
	#cp -rvf ${BUILD_DIR}/bin/comedia.bin ${OUT_DIR}/${BIN_DIR}
	#cp -rvf ${BUILD_DIR}/bin/comedia_lzma.bin ${OUT_DIR}/${BIN_DIR}
}

copy_file

if [ $1 = 'chal' ] ; then
	cp -rvf ${BUILD_DIR}/bin/chalvalidator.out ${OUT_DIR}/${BIN_DIR}
	cp -rvf ${BUILD_DIR}/bin/chalvalidator.bin ${OUT_DIR}/${BIN_DIR}
fi

fi
