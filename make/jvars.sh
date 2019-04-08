#!/usr/bin/env bash
set -euo pipefail

# source shell script (read with . jvars.sh) so stuff is easy to find

# edit following if your install is not standard 
jbld=$HOME/pkg/jbld        # test libraries and binaries will be put here

# platform and shared library suffix
jmake=$(cd "$(dirname "$BASH_SOURCE")"&&pwd)
jgit="$jmake"/.. # git jsource folder
jplatform=`uname|tr '[:upper:]' '[:lower:]'`
jsuffix=so
if [ $jplatform = "darwin" ] ; then jsuffix=dylib ; fi

CC=clang80 # compiler
LDFLAGS='-fuse-ld=lld80 -L/usr/local/lib'
make=gmake

# should not be necessary to edit after here
tsu=$jgit/test/tsu.ijs
j="$jbld/j/bin/jconsole $tsu"

export jgit jbld jplatform j jmake CC make
