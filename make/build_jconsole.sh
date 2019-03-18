#!/usr/bin/env bash
source "$(cd "$(dirname "$BASH_SOURCE")"&&pwd)/jvars.sh"

common=" -march=native -fPIC -O1 -Wextra -Wno-unused-parameter "

case $jplatform in

linux)
COMPILE="$common -DREADLINE"
LINK=" -ledit -ldl -o jconsole "
;;
raspberry)
COMPILE="$common -march=armv8-a+crc -DREADLINE -DRASPI"
LINK=" -ledit -ldl -o jconsole "
;;
darwin)
COMPILE="$common -DREADLINE"
LINK=" -ledit -ldl -lncurses -o jconsole "
;;
freebsd)
COMPILE="$common -DREADLINE -flto "
LINK=" $LDFLAGS $COMPILE -ledit -ldl -o jconsole "
;;
*)
echo no case for those parameters
exit 1
esac

OBJS="jconsole.o jeload.o "
TARGET=jconsole
export OBJS COMPILE LINK TARGET
$jmake/domake.sh

