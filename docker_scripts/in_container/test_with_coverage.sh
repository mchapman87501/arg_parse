#!/bin/sh
set -e -u

cat <<EOF
------------------------------------------------------------------------
Run unit tests with coverage.

EOF

cd
mkdir -p build/debug
cd build/debug

cmake -DCMAKE_BUILD_TYPE=Debug /source

# Defer checking for test failures so we can 
# copy any test output to build_artifacts.
set +e
cmake --build . --target arg_parse_coverage_report
STATUS=$?

set -e
mkdir -p /source/build_artifacts
tar cf - Testing | (cd /source/build_artifacts && tar xf - )
tar cf - arg_parse_coverage_report | (cd /source/build_artifacts && tar xf - )

exit ${STATUS}
