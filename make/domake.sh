#!/usr/bin/env bash

# run by build_jconsole and build_libj

mkdir -p $jbld/jout/$TARGET/j/blis
cd $jbld/jout/$TARGET/j

if ((useavx==1));then
	avx=""
else
	avx=-nonavx
fi 

echo "building  $jbld/j/bin/$TARGET $avx"
echo "output in $jbld/j/bin/build_$TARGET$avx.txt"
$make -f $jmake/makefile >$jbld/j/bin/build_$TARGET$avx.txt 2>&1
echo `egrep -w 'warning|error|note' $jbld/j/bin/build_$TARGET$avx.txt`

if ((useavx==0)); then
	if [ $TARGET = "libj.dylib" ] ; then
		cp $TARGET $jbld/j/bin/libj-nonavx.dylib
	else
		cp $TARGET $jbld/j/bin/libj-nonavx.so
	fi
else
	cp $TARGET $jbld/j/bin
fi
