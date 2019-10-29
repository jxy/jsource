#!/bin/sh

realpath()
{
 oldpath=`pwd`
 if ! cd $1 > /dev/null 2>&1; then
  cd ${1##*/} > /dev/null 2>&1
  echo $( pwd -P )/${1%/*}
 else
  pwd -P
 fi
 cd $oldpath > /dev/null 2>&1
}

cd "$(realpath "$0")"
echo "entering `pwd`"

if [ "`uname -m`" = "armv6l" ] || [ "`uname -m`" = "aarch64" ] || [ "$RASPI" = 1 ]; then
jplatform="${jplatform:=raspberry}"
elif [ "`uname`" = "Darwin" ]; then
jplatform="${jplatform:=darwin}"
else
jplatform="${jplatform:=linux}"
fi
if [ "`uname -m`" = "x86_64" ]; then
j64x="${j64x:=j64avx}"
elif [ "`uname -m`" = "aarch64" ]; then
j64x="${j64x:=j64}"
else
j64x="${j64x:=j32}"
fi
USE_LINENOISE="${USE_LINENOISE:=1}"

# gcc 5 vs 4 - killing off linux asm routines (overflow detection)
# new fast code uses builtins not available in gcc 4
# use -DC_NOMULTINTRINSIC to continue to use more standard c in version 4
# too early to move main linux release package to gcc 5

macmin="-mmacosx-version-min=10.6"

if [ "x$CC" = x'' ] ; then
if [ -f "/usr/bin/cc" ]; then
CC=cc
else
if [ -f "/usr/bin/clang" ]; then
CC=clang
else
CC=gcc
fi
fi
export CC
fi
# compiler=`$CC --version | head -n 1`
compiler=$(readlink -f $(command -v $CC) 2> /dev/null || echo $CC)
echo "CC=$CC"
echo "compiler=$compiler"

if [ -z "${compiler##*gcc*}" ] || [ -z "${CC##*gcc*}" ]; then
# gcc
common=" -fPIC -O1 -fwrapv -fno-strict-aliasing -Wextra -Wno-maybe-uninitialized -Wno-unused-parameter -Wno-sign-compare -Wno-clobbered -Wno-empty-body -Wno-unused-value -Wno-pointer-sign -Wno-parentheses"
OVER_GCC_VER6=$(echo `$CC -dumpversion | cut -f1 -d.` \>= 6 | bc)
if [ $OVER_GCC_VER6 -eq 1 ] ; then
common="$common -Wno-shift-negative-value"
else
common="$common -Wno-type-limits"
fi
# alternatively, add comment /* fall through */
OVER_GCC_VER7=$(echo `$CC -dumpversion | cut -f1 -d.` \>= 7 | bc)
if [ $OVER_GCC_VER7 -eq 1 ] ; then
common="$common -Wno-implicit-fallthrough"
fi
OVER_GCC_VER8=$(echo `$CC -dumpversion | cut -f1 -d.` \>= 8 | bc)
if [ $OVER_GCC_VER8 -eq 1 ] ; then
common="$common -Wno-cast-function-type"
fi
else
# clang 3.5 .. 5.0
common=" -Werror -fPIC -O1 -fwrapv -fno-strict-aliasing -Wextra -Wno-consumed -Wno-uninitialized -Wno-unused-parameter -Wno-sign-compare -Wno-empty-body -Wno-unused-value -Wno-pointer-sign -Wno-parentheses -Wno-unsequenced -Wno-string-plus-int"
fi
darwin=" -fPIC -O1 -fwrapv -fno-strict-aliasing -Wno-string-plus-int -Wno-empty-body -Wno-unsequenced -Wno-unused-value -Wno-pointer-sign -Wno-parentheses -Wno-return-type -Wno-constant-logical-operand -Wno-comment -Wno-unsequenced"

TARGET=jconsole

case $jplatform\_$j64x in

linux_j32)
if [ "$USE_LINENOISE" -ne "1" ] ; then
CFLAGS="$common -m32 -DREADLINE"
LDFLAGS=" -m32 -ldl "
else
CFLAGS="$common -m32 -DREADLINE -DUSE_LINENOISE"
LDFLAGS=" -m32 -ldl "
OBJSLN="linenoise.o"
fi
;;
linux_j64)
if [ "$USE_LINENOISE" -ne "1" ] ; then
CFLAGS="$common -DREADLINE"
LDFLAGS=" -ldl "
else
CFLAGS="$common -DREADLINE -DUSE_LINENOISE"
LDFLAGS=" -ldl "
OBJSLN="linenoise.o"
fi
;;
linux_j64avx)
if [ "$USE_LINENOISE" -ne "1" ] ; then
CFLAGS="$common -DREADLINE"
LDFLAGS=" -ldl "
else
CFLAGS="$common -DREADLINE -DUSE_LINENOISE"
LDFLAGS=" -ldl "
OBJSLN="linenoise.o"
fi
;;
linux_j64avx2)
if [ "$USE_LINENOISE" -ne "1" ] ; then
CFLAGS="$common -DREADLINE"
LDFLAGS=" -ldl "
else
CFLAGS="$common -DREADLINE -DUSE_LINENOISE"
LDFLAGS=" -ldl "
OBJSLN="linenoise.o"
fi
;;
raspberry_j32)
if [ "$USE_LINENOISE" -ne "1" ] ; then
CFLAGS="$common -marm -march=armv6 -mfloat-abi=hard -mfpu=vfp -DREADLINE -DRASPI"
LDFLAGS=" -ldl "
else
CFLAGS="$common -marm -march=armv6 -mfloat-abi=hard -mfpu=vfp -DREADLINE -DUSE_LINENOISE -DRASPI"
LDFLAGS=" -ldl "
OBJSLN="linenoise.o"
fi
;;
raspberry_j64)
if [ "$USE_LINENOISE" -ne "1" ] ; then
CFLAGS="$common -march=armv8-a+crc -DREADLINE -DRASPI"
LDFLAGS=" -ldl "
else
CFLAGS="$common -march=armv8-a+crc -DREADLINE -DUSE_LINENOISE -DRASPI"
LDFLAGS=" -ldl "
OBJSLN="linenoise.o"
fi
;;
darwin_j32)
if [ "$USE_LINENOISE" -ne "1" ] ; then
CFLAGS="$darwin -m32 -DREADLINE $macmin"
LDFLAGS=" -ldl -m32 $macmin "
else
CFLAGS="$darwin -m32 -DREADLINE -DUSE_LINENOISE $macmin"
LDFLAGS=" -ldl -m32 $macmin "
OBJSLN="linenoise.o"
fi
;;
#-mmacosx-version-min=10.5
darwin_j64)
if [ "$USE_LINENOISE" -ne "1" ] ; then
CFLAGS="$darwin -DREADLINE $macmin"
LDFLAGS=" -ldl $macmin "
else
CFLAGS="$darwin -DREADLINE -DUSE_LINENOISE $macmin"
LDFLAGS=" -ldl $macmin "
OBJSLN="linenoise.o"
fi
;;
darwin_j64avx)
if [ "$USE_LINENOISE" -ne "1" ] ; then
CFLAGS="$darwin -DREADLINE $macmin"
LDFLAGS=" -ldl $macmin "
else
CFLAGS="$darwin -DREADLINE -DUSE_LINENOISE $macmin"
LDFLAGS=" -ldl $macmin "
OBJSLN="linenoise.o"
fi
;;
darwin_j64avx2)
if [ "$USE_LINENOISE" -ne "1" ] ; then
CFLAGS="$darwin -DREADLINE $macmin"
LDFLAGS=" -ldl $macmin "
else
CFLAGS="$darwin -DREADLINE -DUSE_LINENOISE $macmin"
LDFLAGS=" -ldl $macmin "
OBJSLN="linenoise.o"
fi
;;
windows_j32)
TARGET=jconsole.exe
CFLAGS="$common -m32 "
LDFLAGS=" -m32 -Wl,--stack=0x1000000,--subsystem,console -static-libgcc "
;;
windows_j64)
TARGET=jconsole.exe
CFLAGS="$common "
LDFLAGS=" -Wl,--stack=0x1000000,--subsystem,console -static-libgcc "
;;
windows_j64avx)
TARGET=jconsole.exe
CFLAGS="$common "
LDFLAGS=" -Wl,--stack=0x1000000,--subsystem,console -static-libgcc "
;;
windows_j64avx2)
TARGET=jconsole.exe
CFLAGS="$common "
LDFLAGS=" -Wl,--stack=0x1000000,--subsystem,console -static-libgcc "
;;
*)
echo no case for those parameters
exit
esac

echo "CFLAGS=$CFLAGS"

if [ ! -f ../jsrc/jversion.h ] ; then
  cp ../jsrc/jversion-x.h ../jsrc/jversion.h
fi

mkdir -p ../bin/$jplatform/$j64x
mkdir -p obj/$jplatform/$j64x/
cp makefile-jconsole obj/$jplatform/$j64x/.
export CFLAGS LDFLAGS TARGET OBJSLN jplatform j64x
cd obj/$jplatform/$j64x/
make -f makefile-jconsole
cd -
