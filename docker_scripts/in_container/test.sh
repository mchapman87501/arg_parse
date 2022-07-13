#!/bin/sh
set -e -u

HERE=$(dirname "$0")
echo "Here is ${HERE}."

cd ${HERE}
sh ./test_with_coverage.sh
sh ./test_installed.sh