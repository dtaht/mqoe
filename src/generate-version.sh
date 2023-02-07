#!/bin/sh

set -e

if [ -d .git ] && [ "$(git tag)" ] ; then
    version="$(git describe --dirty)"
elif [ -d .git ] ; then
    version="$(git rev-list --oneline HEAD | head -1)"
elif [ -f version ] ; then
    version="$(cat version)"
else
    version="unknown"
fi

echo "#pragma once\n#define MQOE_VERSION \"$version\"" > $1
