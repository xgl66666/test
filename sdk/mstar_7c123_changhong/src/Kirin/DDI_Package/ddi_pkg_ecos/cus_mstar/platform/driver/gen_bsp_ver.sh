#!/bin/sh
if [ -z $1 ]; then
    CURR_PATH=./
else
    CURR_PATH=$1
fi

PROJ_PATH=`git remote -v | head -n1 | awk '{print $2}' | sed 's/ssh:\/\/.*@.*:[0-9]*\///'`
VER=`git rev-parse HEAD`
echo "$VER @ $PROJ_PATH"

echo "char BSP_VERSION[]=\"$VER @ $PROJ_PATH\";" >  $CURR_PATH/BSP_version_info.c
