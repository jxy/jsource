#!/usr/bin/env bash
# build all binaries
source "$(cd "$(dirname "$BASH_SOURCE")"&&pwd)/jvars.sh"

if [ ! -d $jbld ]; then
 echo "jbld ($jbld) does not exist - running install.sh"
 $jmake/install.sh
fi

$jmake/clean.sh

$jmake/build_jconsole.sh
$jmake/build_libj.sh
$jmake/build_tsdll.sh
