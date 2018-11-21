#!/usr/bin/env bash
# copy git jlibrary to jbld j

source "$(cd "$(dirname "$BASH_SOURCE")"&&pwd)/jvars.sh"

cp -r $jgit/jlibrary/* $jbld/j
exit 0

