#!/usr/bin/env bash

# run by build_jconsole and build_libj

mkdir -p $jbld/jout/$TARGET/j/blis
cd $jbld/jout/$TARGET/j

echo "building  $jbld/j/bin/$TARGET"
echo "output in $jbld/j/bin/build_$TARGET.txt"
make -f $jmake/makefile >$jbld/j/bin/build_$TARGET.txt 2>&1

egrep -w 'warning|error|note' -B 2 $jbld/j/bin/build_$TARGET.txt

cp $TARGET $jbld/j/bin
