#!/usr/bin/env bash
# build all binaries
source "$(cd "$(dirname "$BASH_SOURCE")"&&pwd)/jvars.sh"

$jmake/install.sh

$jmake/build_jconsole.sh
$jmake/build_libj.sh
$jmake/build_tsdll.sh
