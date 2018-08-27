#!/bin/bash
#
# Run this script to copy the shared libraries, config files, binaries, ...
# into final root filesystem, based on rootfs-skel.tar.gz
#
# Copyright (C) 2005 Matthias Ihmig <m.ihmig@mytum.de>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Library General Public License for more details.

set -e

# The following masks, separated by |, define the files which will be EXCLUDED from the final rootfs
EXCLUDEFILES="^man|^usr/share/man|^usr/lib/ssl/man|^usr/share/aclocal|^usr/share/locale|^usr/man|^usr/include|\.a$|.la$|^usr/lib/pkgconfig|^usr/info"
#echo "include var_list"
#VAR_LIST=../../../bsp/linux/partition/auto_gen_output/var_list
#[ ! -e ${VAR_LIST} ] && echo "ERROR: Config file ${VAR_LIST} not found!" && exit 1
#source ${VAR_LIST}

echo "include makerootfs.conf"
MAKEROOTFS_CONFIG=../../../../../../DDI_Misc/makerootfs/makerootfs.conf
#[ -z "$MAKEROOTFS_CONFIG" ] && MAKEROOTFS_CONFIG=$PWD/makerootfs.conf
[ ! -e $MAKEROOTFS_CONFIG ] && echo "ERROR: Config file ($MAKEROOTFS_CONFIG) not found!" && exit 1
. $MAKEROOTFS_CONFIG

#! which $TARGET_STRIP && echo "ERROR: Can't find $TARGET_STRIP in PATH!" && exit 1
echo "=== Creating $MSTAR_CUST_STAGING"
echo "=== Creating $MYROOTFS"
ROOTFS_STAGING=$MSTAR_CUST_STAGING
#echo -e "\n*** Unpacking rootfs skeleton... Note that devices will be created later during mkfs.jffs2!"
#sleep 3s

#rm -rf rootfs $MYROOTFS
#tar xpsvzf $PATCHES/rootfs-skel.tar.gz
#mv rootfs $MYROOTFS && echo -n
#echo $MYROOTFS
echo -e "\n\n*** Copying files"
#echo -e "cd $ROOTFS_STAGING"
cd $ROOTFS_STAGING

for fil in $( find * | grep -v -E "$EXCLUDEFILES" )
do
	#echo -n "$fil "
	[ -d "$fil" ] && [ ! -L "$fil" ] && [ `mkdir -p $MYROOTFS/$fil` ] && echo "! "
	if [ ! -d "$fil" ] || [ -L "$fil" ]
	then
    echo
		[ ! -d `dirname $fil` ] && mkdir -p `dirname $fil` && echo "!! "
		#echo "rm -rf $MYROOTFS/$fil"
    [ -e $MYROOTFS/$fil ] && rm -rf $MYROOTFS/$fil && echo "rm -rf $MYROOTFS/$fil"
		echo "cp -af $fil $MYROOTFS/$fil"
    cp -af $fil $MYROOTFS/$fil
    echo
	fi
done
echo "==create rootfs fromstaging done!!\n\n"
#echo -e "\n\n*** Stripping binaries and libraries. You can ignore File Format error messages."
#cd $MYROOTFS
#for fil in $( find * | grep -E "bin/|\.so" )
#do
#	[ ! -x $fil ] && echo -n "...$fil not executable and will be skipped "
#	[ -x $fil ] && [ -f $fil ] && echo -n "stripping $fil... " \
#		&& $TARGET_STRIP  --discard-locals --discard-all --strip-all --strip-debug $fil && echo -n
#	echo
#done
