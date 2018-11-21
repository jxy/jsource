#!/usr/bin/env bash
# install jbld folders - new install or a reinstall

source "$(cd "$(dirname "$BASH_SOURCE")"&&pwd)/jvars.sh"

rm -f -r $jbld
mkdir $jbld
mkdir $jbld/j
mkdir $jbld/jout
cp -r $jgit/jlibrary/* $jbld/j

echo "install complete"
echo ""
echo "$jgit/license.txt"

cat $jgit/license.txt
exit 0

