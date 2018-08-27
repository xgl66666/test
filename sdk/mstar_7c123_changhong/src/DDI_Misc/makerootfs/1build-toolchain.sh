#!/bin/sh
#
# Run this script to generate a cross-compiler toolchain
# Configure by editing makerootfs.conf
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

[ -z "$MAKEROOTFS_CONFIG" ] && MAKEROOTFS_CONFIG=$PWD/makerootfs.conf
[ ! -e $MAKEROOTFS_CONFIG ] && echo "ERROR: Config file ($MAKEROOTFS_CONFIG) not found!" && exit 1
. $MAKEROOTFS_CONFIG

which $TARGET_CC && echo "*** WARNING: $TARGET_CC already in PATH! Skip building toolchain." && exit
[ -e `dirname $RESULT_TOP`/set-env-`basename $RESULT_TOP` ] && echo "*** Toolchain already build. skipping.." && exit

FILE=crosstool-0.31.tar.gz
SRCDIR=crosstool-0.31

TARBALLS_DIR=$DOWNLOADS/crosstool-downloads

export TARBALLS_DIR RESULT_TOP
export GCC_LANGUAGES

mkdir -p $TARBALLS_DIR
mkdir -p $RESULT_TOP

[ ! -e $BUILDDIR ] && mkdir $BUILDDIR
cd $BUILDDIR

[ ! -e "$TARBALLS_DIR/$FILE" ] && wget http://kegel.com/crosstool/$FILE -P $TARBALLS_DIR

tar xfvz $TARBALLS_DIR/$FILE
cd $SRCDIR

# Build the toolchain. Can take an hour and a couple gigabytes.
eval `cat $TOOLCHAIN_VERSION` LINUX_DIR=linux-$KVERSBASE TARGET=$CROSS_HOST sh all.sh --notest

echo -n "PATH=$RESULT_TOP/bin:" > `dirname $RESULT_TOP`/set-env-`basename $RESULT_TOP`
echo '$PATH' >> `dirname $RESULT_TOP`/set-env-`basename $RESULT_TOP`

cd $RESULT_TOP
mv $CROSS_HOST $CROSS_HOST-old
mv $CROSS_HOST-old/*/* .
rmdir $CROSS_HOST-old/* $CROSS_HOST-old

echo "*** Toolchain built successfully."
echo "*** Put $RESULT_TOP/bin in your PATH to use it, or source set-env-`basename $RESULT_TOP`"
