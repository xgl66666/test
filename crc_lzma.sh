#!/bin/bash

if [ $# -ne 1 ] ; then
	echo -e "usage: source crc_lzma.sh"
	echo -e "        for example: source crc_lzma.sh dal"
else

OUT_DIR=out
BIN_DIR=$1
file_size=3452928

cd ${OUT_DIR}/${BIN_DIR}

export PATH=/opt/mstar/gnutools/mipsisa32-elf-3.4.4/bin:$PATH

lzma()
{
	mipsisa32-elf-strip $1.out
	mipsisa32-elf-objcopy -O binary $1.out $1.bin
	./crc -a $1.bin $1.bin
	./lzma e $1.bin $1_lzma.bin
}

add_lib()
{
	#cat PK_CSO_TEST_PK_2005_10000.D33 >> $1_lzma.bin
	
	dd if=$1_lzma.bin ibs=3452928 count=1 of=$1_lzma_padding.bin conv=sync
	cat $1_lzma_padding.bin > $1_lzma_ota.bin
	cat resources.lzma >> $1_lzma_ota.bin
	
	#dd if=$1_lzma.bin ibs=4067328 count=1 of=$1_lzma_padding.bin conv=sync
	#cat $1_lzma_ota.bin > $1_lzma_ota_pk.bin
	#cat PK_CSO_TEST_PK_2005_10000.D33 >> $1_lzma_ota_pk.bin
}

rm_file()
{
	rm -rvf lzma
	rm -rvf crc
	rm -rvf $1.out
	rm -rvf $1_lzma_padding.bin
	rm -rvf resources.lzma
}

cp -rf ../../boot_5c35_tools/PK_CSO_TEST_PK_2005_10000.D33 ./

if [ $1 = 'chal' ] ; then
	lzma chalvalidator
	add_lib chalvalidator
	rm_file chalvalidator
else
	lzma comedia
	add_lib comedia
	rm_file comedia
fi

cd -

fi
