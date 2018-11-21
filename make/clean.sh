#!/usr/bin/env bash
source "$(cd "$(dirname "$BASH_SOURCE")"&&pwd)/jvars.sh"

# rm all *.o for clean builds - makefile dependencies are not set 
find $jbld/jout -name "*.o" -type f -delete
find $jgit/jsrc -name "*.o" -type f -delete
find $jgit/dllsrc -name "*.o" -type f -delete



