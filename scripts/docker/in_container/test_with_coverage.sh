#!/bin/sh
set -e -u

cat <<EOF
------------------------------------------------------------------------
Run unit tests with coverage.

EOF

cd /source
cmake --version
cmake --preset profile --fresh

# Defer checking for test failures so we can 
# copy any test output to build_artifacts.
set +e
cmake --build --preset profile --target arg_parse_coverage_report
STATUS=$?

set -e
mkdir -p /source/build_artifacts
cd build/profile
tar cf - Testing | (cd /source/build_artifacts && tar xf - )
tar cf - arg_parse_coverage_report | (cd /source/build_artifacts && tar xf - )

exit ${STATUS}
