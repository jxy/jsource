#!/usr/bin/env bash

# run by build_jconsole and build_libj and build_tsdll

echo "domake: $TARGET CC=$CC"

mkdir -p $jbld/jout/$TARGET/j/blis
cd $jbld/jout/$TARGET/j

OUT=$jbld/jout/$1/bld-$TARGET

mkdir -p $OUT/blis
mkdir -p $OUT/txt
cd       $OUT

echo "        $OUT/txt/build.txt"
echo ""

gmake -f $jmake/makefile >$jbld/jout/$1/bld-$TARGET/txt/build.txt 2>&1

egrep -w 'warning|error|note' -B 2 $OUT/txt/build.txt

cp $TARGET $jbld/j/bin
