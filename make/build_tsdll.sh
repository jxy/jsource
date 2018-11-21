#!/usr/bin/env bash
source "$(cd "$(dirname "$BASH_SOURCE")"&&pwd)/jvars.sh"

common=" -march=native -fPIC -O1 -Werror -Wextra -Wno-unused-parameter"

case $jplatform in

raspberry)
TARGET=libtsdll.so
COMPILE="$common -march=armv8-a+crc "
LINK=" -shared -Wl,-soname,libtsdll.so -o libtsdll.so -lm "
;;
darwin)
TARGET=libtsdll.dylib
COMPILE="$common "
LINK=" $macmin -dynamiclib -o libtsdll.dylib -lm "
;;
*)
TARGET=libtsdll.so
COMPILE="$common "
LINK=" -shared -Wl,-soname,libtsdll.so -o libtsdll.so -lm "
esac

echo "COMPILE=$COMPILE"

OBJS="tsdll.o "
export OBJS COMPILE LINK TARGET
$jmake/domake.sh

