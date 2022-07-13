#!/bin/sh
set -e -u

cat <<EOF
------------------------------------------------------------------------
Verify the installed package can be used via CMake's find_package().

EOF

cd

mkdir -p build/release
cd build/release

INSTALL_DIR=${HOME}/local
cmake -DCMAKE_BUILD_TYPE=Release /source
cmake --build .
cmake --install . --prefix ${INSTALL_DIR}

cd ..

mkdir pkg_consumer
cd pkg_consumer
cmake -DCMAKE_PREFIX_PATH=${INSTALL_DIR} /source/tests/package_configuration_tests
cmake --build .
./use_arg_parse --verbose

