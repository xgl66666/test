#!/bin/bash

if [ $# -ne 1 ] ; then
	echo -e "usage: source setenv.sh"
	echo -e "        for example: source setenv.sh dal"
else
##ROOT_DIR=$1
CONFIG=$1

if [ $1 != 'chal' ] ; then
if [ $1 != 'dal' ] ; then
if [ $1 != 'nak' ] ; then
	echo -e "SETENV ONLY SUPPORT:"
	echo -e "	chal		mstar_5C35_changhong_maestro_base_merlin"
	echo -e "	dal		mstar_5C35_changhong_maestro_base_merlin_dalts"
	echo -e "	nak		mstar_5C35_changhong_maestro_base_merlin_nak_rel"
fi
fi
fi

if [ $1 = 'chal' ] ; then
	echo -e "SETENV=mstar_5C35_changhong_maestro_base_merlin_dalts"
	export SETENV=mstar_5C35_changhong_maestro_base_merlin
fi

if [ $1 = 'dal' ] ; then
	echo -e "SETENV=mstar_5C35_changhong_maestro_base_merlin_dalts"
	export SETENV=mstar_5C35_changhong_maestro_base_merlin_dalts
fi

if [ $1 = 'nak' ] ; then
	echo -e "SETENV=mstar_5C35_changhong_maestro_base_merlin_nak_rel"
	export SETENV=mstar_5C35_changhong_maestro_base_merlin_nak_rel
fi

fi


