#!/usr/bin/env bash
set -euo pipefail

# source shell script (read with . jvars.sh) so stuff is easy to find

# edit following if your install is not standard 
jbld=$HOME/pkg/jbld        # test libraries and binaries will be put here
useavx=1

# platform and shared library suffix
jmake=$(cd "$(dirname "$BASH_SOURCE")"&&pwd)
jgit="$jmake"/.. # git jsource folder
jplatform=`uname|tr '[:upper:]' '[:lower:]'`
jsuffix=so
if [ $jplatform = "darwin" ] ; then jsuffix=dylib ; fi

CC=clang70 # compiler
LDFLAGS=-fuse-ld=lld70
make=gmake

# should not be necessary to edit after here
tsu=$jgit/test/tsu.ijs
if ((useavx==1));then
	j="$jbld/j/bin/jconsole $tsu"
else
	j="$jbld/j/bin/jconsole -lib libj-nonavx.$jsuffix $tsu"
fi

export jgit jbld jplatform j jmake useavx CC make
