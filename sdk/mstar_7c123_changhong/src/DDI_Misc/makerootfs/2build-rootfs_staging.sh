#!/bin/sh
#
# Run this script to cross-compile libraries and software packages
# Configure first by editing makerootfs.conf !
#
# Copyright (C) 2005 Matthias Ihmig <m.ihmig@mytum.de>
#
# Ideas for some patches and a few make scripts from
#   http://buildroot.uclibc.org/  and
#   http://www.gumstix.org/
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
echo "include var_list"
VAR_LIST=../../../bsp/linux/partition/auto_gen_output/var_list
[ ! -e ${VAR_LIST} ] && echo "ERROR: Config file ${VAR_LIST} not found!" && exit 1
source ${VAR_LIST}

echo "include makerootfs.conf"
MAKEROOTFS_CONFIG=../../../../../../DDI_Misc/makerootfs/makerootfs.conf
#[ -z "$MAKEROOTFS_CONFIG" ] && MAKEROOTFS_CONFIG=$PWD/makerootfs.conf
[ ! -e ${MAKEROOTFS_CONFIG} ] && echo "ERROR: Config file ($MAKEROOTFS_CONFIG) not found!" && exit 1
. ${MAKEROOTFS_CONFIG}

! which $TARGET_CC && echo "ERROR: Can't find $TARGET_CC in PATH!" && exit 1

#if [ ! -d "$KERNEL_PATH" ] || [ ! -e "$KERNEL_PATH/include/linux/version.h" ]
#then
#	echo "WARNING: Kernel sources not found or not built in $KERNEL_PATH!"
#	echo "Some packages need them to compile and may fail otherwise."
#	echo -n "Do you still want to continue? [y/N]"
#	read ASK
#	[ ! $ASK == "y" ] && exit 1
#fi
[ -e $DOWNLOADS ] && rm -r $DOWNLOADS
[ -e $ROOTFS_STAGING ] && rm -r $ROOTFS_STAGING
[ -e $BUILDDIR ] && rm -r $BUILDDIR

mkdir -p $DOWNLOADS
mkdir -p $ROOTFS_STAGING
mkdir -p $BUILDDIR

echo "=== Cross-compiling selected software (configure in makerootfs.conf)"

for pkg in $MYMAKE_LIST ; do
#	[ ! -e $BUILDDIR/.$pkg.successful ] && $PWD/makescripts/make_$pkg.sh && touch $BUILDDIR/.$pkg.successful
#echo "1"
  [ ! -e $BUILDDIR/.$pkg.successful ] && ${MAKEROOTFS_PATH}/makescripts/make_$pkg.sh && touch $BUILDDIR/.$pkg.successful
#echo "2"
	[ ! -e $BUILDDIR/.$pkg.successful ] && echo "ERROR: couldn't make $pkg" && exit 1
	echo "===build rootfs staging done!!\n\n"
done
