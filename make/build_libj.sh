#!/usr/bin/env bash
source "$(cd "$(dirname "$BASH_SOURCE")"&&pwd)/jvars.sh"

# gcc 5 vs 4 - killing off linux asm routines (overflow detection)
# new fast code uses builtins not available in gcc 4
# use -DC_NOMULTINTRINSIC to continue to use more standard c in version 4
# too early to move main linux release package to gcc 5

compiler=${CC:0:3}

OPENMP=""
LDOPENMP=""
USE_OPENMP="${USE_OPENMP:=0}"
if [ $USE_OPENMP -eq 1 ] ; then
OPENMP=" -fopenmp "
LDOPENMP=" -fopenmp "
if [ "x$compiler" = x'gcc' ] ; then
LDOPENMP32=" -l:libgomp.so.1 "    # gcc
else
LDOPENMP32=" -l:libomp.so.5 "     # clang
fi
fi

if [ "x$compiler" = x'gcc' ] ; then
# gcc
common="-march=native $OPENMP -fPIC -O1 -fwrapv -fno-strict-aliasing -Wextra -Wno-maybe-uninitialized -Wno-unused-parameter -Wno-sign-compare -Wno-clobbered -Wno-empty-body -Wno-unused-value -Wno-pointer-sign -Wno-parentheses -Wno-shift-negative-value"
# alternatively, add comment /* fall through */
OVER_GCC_VER7=$($CC -dumpversion | awk -F '.' '{print($1>=7)}')
if [ $OVER_GCC_VER7 -eq 1 ] ; then
common="$common -Wno-implicit-fallthrough"
fi
OVER_GCC_VER8=$($CC -dumpversion | awk -F '.' '{print($1>=8)}')
if [ $OVER_GCC_VER8 -eq 1 ] ; then
common="$common -Wno-cast-function-type"
fi
else
# clang 3.5 .. 5.0
common="-march=native $OPENMP -Werror -fPIC -O1 -fwrapv -fno-strict-aliasing -Wextra -Wno-consumed -Wno-uninitialized -Wno-unused-parameter -Wno-sign-compare -Wno-empty-body -Wno-unused-value -Wno-pointer-sign -Wno-parentheses -Wno-unsequenced -Wno-string-plus-int"
fi
darwin="-march=native $OPENMP -fPIC -O1 -fwrapv -fno-strict-aliasing -Wno-string-plus-int -Wno-empty-body -Wno-unsequenced -Wno-unused-value -Wno-pointer-sign -Wno-parentheses -Wno-return-type -Wno-constant-logical-operand -Wno-comment -Wno-unsequenced"

case $jplatform in

linux)
TARGET=libj.so
COMPILE="$common "
LINK=" -shared -Wl,-soname,libj.so -lm -ldl $LDOPENMP -o libj.so "
;;

raspberry) # linux arm64
TARGET=libj.so
COMPILE="$common -march=armv8-a+crc -DRASPI -DC_CRC32C=1 "
LINK=" -shared -Wl,-soname,libj.so -lm -ldl $LDOPENMP -o libj.so "
;;

darwin)
TARGET=libj.dylib
COMPILE="$darwin "
LINK=" -dynamiclib -lm -ldl $LDOPENMP -o libj.dylib"
;;

freebsd)
TARGET=libj.so
COMPILE="$common -flto "
LINK=" $COMPILE -shared -Wl,-soname,libj.so -lm -ldl $LDOPENMP -o libj.so "
;;

*)
echo no case for those parameters
exit 1
esac

if ((useavx==1));then
	COMPILE="$COMPILE -mavx -DC_AVX=1 "
	OBJS_FMA=" blis/gemm_int-fma.o "
fi

OBJS="\
 a.o \
 ab.o \
 af.o \
 ai.o \
 am.o \
 am1.o \
 amn.o \
 ao.o \
 ap.o \
 ar.o \
 as.o \
 au.o \
 blis/gemm_c-ref.o \
 blis/gemm_int-aarch64.o \
 blis/gemm_int-avx.o \
 blis/gemm_int-sse2.o \
 blis/gemm_vec-ref.o \
 c.o \
 ca.o \
 cc.o \
 cd.o \
 cf.o \
 cg.o \
 ch.o \
 cip.o \
 cl.o \
 cp.o \
 cpdtsp.o \
 cpuinfo.o \
 cr.o \
 crs.o \
 ct.o \
 cu.o \
 cv.o \
 cx.o \
 d.o \
 dc.o \
 dss.o \
 dstop.o \
 dsusp.o \
 dtoa.o \
 f.o \
 f2.o \
 fbu.o \
 gemm.o \
 i.o \
 io.o \
 j.o \
 jdlllic.o \
 k.o \
 m.o \
 mbx.o \
 p.o \
 pv.o \
 px.o \
 r.o \
 rl.o \
 rt.o \
 s.o \
 sc.o \
 sl.o \
 sn.o \
 t.o \
 u.o \
 v.o \
 v0.o \
 v1.o \
 v2.o \
 va1.o \
 va1ss.o \
 va2.o \
 va2s.o \
 va2ss.o \
 vamultsp.o \
 vb.o \
 vbang.o \
 vbit.o \
 vcant.o \
 vchar.o \
 vcat.o \
 vcatsp.o \
 vcomp.o \
 vcompsc.o \
 vd.o \
 vdx.o \
 ve.o \
 vf.o \
 vfft.o \
 vfrom.o \
 vfromsp.o \
 vg.o \
 vgauss.o \
 vgcomp.o \
 vgranking.o \
 vgsort.o \
 vgsp.o \
 vi.o \
 viavx.o \
 viix.o \
 visp.o \
 vm.o \
 vo.o \
 vp.o \
 vq.o \
 vrand.o \
 vrep.o \
 vs.o \
 vsb.o \
 vt.o \
 vu.o \
 vx.o \
 vz.o \
 w.o \
 wc.o \
 wn.o \
 ws.o \
 x.o \
 x15.o \
 xa.o \
 xb.o \
 xc.o \
 xcrc.o \
 xd.o \
 xf.o \
 xfmt.o \
 xh.o \
 xi.o \
 xl.o \
 xo.o \
 xs.o \
 xsha.o \
 xt.o \
 xu.o "

export OBJS OBJS_FMA COMPILE LINK TARGET
$jmake/domake.sh

