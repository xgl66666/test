#!/bin/bash

if [ $# -ne 1 ] ; then
	echo -e "usage: source build_target.sh"
	echo -e "        for example: source build_target.sh dal"
	echo -e "        for example: source build_target.sh nak"
	echo -e "        for example: source build_target.sh chal"
	exit 1
else

TARGET=$1

make_build()
{
	make clean_all
	
	make sdk
	
	echo -e "\nmake chal..."
	make chal TRACES=UART
	
	if [ $1 = 'chal' ] ; then
		echo -e "\nmake chal..."
		make chalvalidator TRACES=UART
	fi
	
	if [ $1 = 'dal' ] ; then
		echo -e "\nmake comedia..."
		make comedia TRACES=UART DEBUG=1
	fi
	
	if [ $1 = 'nak' ] ; then
		echo -e "\nmake comedia..."
		make comedia
	fi
}

if [ $1 != 'chal' ] ; then
if [ $1 != 'dal' ] ; then
if [ $1 != 'nak' ] ; then
	echo -e "BUILD:"
	echo -e " chal 		mstar_5C35_changhong_maestro_base_merlin"
	echo -e " dal 		mstar_5C35_changhong_maestro_base_merlin_dalts"
	echo -e " nak 		mstar_5C35_changhong_maestro_base_merlin_nak_rel"
	exit 1
fi
fi
fi

echo -e "\nbuild" $1 "..."
source setenv.sh $1
make_build $1
	
echo -e "\ncopy file..."
source copy.sh $1
	
echo -e "\nlzma file..."
source crc_lzma.sh $1

fi

