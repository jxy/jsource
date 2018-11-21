#!/usr/bin/env bash
source "$(cd "$(dirname "$BASH_SOURCE")"&&pwd)/jvars.sh"

USE_LINENOISE="${USE_LINENOISE:=1}"
common=" -march=native -fPIC -O1 -Wextra -Wno-unused-parameter "

case $jplatform in

raspberry)
if [ "$USE_LINENOISE" -ne "1" ] ; then
COMPILE="$common -march=armv8-a+crc -DREADLINE -DRASPI"
LINK=" -ledit -ldl -o jconsole "
else
COMPILE="$common -march=armv8-a+crc -DREADLINE -DUSE_LINENOISE -DRASPI"
LINK=" -ldl -o jconsole "
OBJSLN="linenoise.o"
fi
;;
*)
if [ "$USE_LINENOISE" -ne "1" ] ; then
COMPILE="$common -DREADLINE"
LINK=" -ledit -ldl -o jconsole "
else
COMPILE="$common -DREADLINE -DUSE_LINENOISE"
LINK=" -ldl -o jconsole "
OBJSLN="linenoise.o"
fi

esac

OBJS="jconsole.o jeload.o ${OBJSLN}"
TARGET=jconsole
export OBJS COMPILE LINK TARGET
$jmake/domake.sh

