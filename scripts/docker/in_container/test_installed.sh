#!/bin/sh
set -e -u

cat <<EOF
------------------------------------------------------------------------
Verify the installed package can be used via CMake's find_package().

EOF

cd /source
cmake --version
cmake --preset release --fresh
cmake --build --preset release
INSTALL_DIR=${HOME}/local
cmake --install build/release --prefix=${INSTALL_DIR}

mkdir pkg_consumer
cd pkg_consumer
cmake -DCMAKE_PREFIX_PATH=${INSTALL_DIR} /source/tests/package_configuration_tests
cmake --build .

./use_arg_parse --help
