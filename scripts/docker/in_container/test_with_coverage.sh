#!/bin/sh
set -e -u

cat <<EOF
------------------------------------------------------------------------
Run unit tests with coverage.

EOF

cd
cmake -DCMAKE_BUILD_TYPE=Profile -Bbuild/profile -S/source

# Defer checking for test failures so we can 
# copy any test output to build_artifacts.
set +e
cmake --build build/profile --target arg_parse_coverage_report
STATUS=$?

set -e
mkdir -p /source/build_artifacts
cd build/profile
tar cf - Testing | (cd /source/build_artifacts && tar xf - )
tar cf - arg_parse_coverage_report | (cd /source/build_artifacts && tar xf - )

exit ${STATUS}
