#!/usr/bin/env bash
source "$(cd "$(dirname "$BASH_SOURCE")"&&pwd)/jvars.sh"

common=" -march=native -fPIC -O1 -Werror -Wextra -Wno-unused-parameter"

case $jplatform in

linux)
TARGET=libtsdll.so
COMPILE="$common "
LINK=" -shared -Wl,-soname,libtsdll.so -o libtsdll.so "
;;
raspberry)
TARGET=libtsdll.so
COMPILE="$common -march=armv8-a+crc "
LINK=" -shared -Wl,-soname,libtsdll.so -o libtsdll.so "
;;
darwin)
TARGET=libtsdll.dylib
COMPILE="$common "
LINK=" -dynamiclib -o libtsdll.dylib "
;;
freebsd)
TARGET=libtsdll.so
COMPILE="$common "
LINK=" -shared -Wl,-soname,libtsdll.so -o libtsdll.so "
;;
*)
echo no case for those parameters
exit 1
esac

OBJS="tsdll.o "
export OBJS COMPILE LINK TARGET
$jmake/domake.sh

