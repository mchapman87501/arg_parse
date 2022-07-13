# Overview

This sample project eases testing of the arg_parse project's CMake configuration info.
To use it:

* Install the arg_parse project somewhere
* Try to build using the CMakeLists.txt file in this directory.

# Install arg_parse
```shell

# Assume you are in the directory containing this README.
TEST_SRC_DIR=${PWD}

cd ../..
SRC_ROOT=${PWD}
TEST_ROOT=${HOME}/tmp/arg_parse_package_test

mkdir -p ${TEST_ROOT}
cd ${TEST_ROOT}

# Build arg_parse package
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ${SRC_ROOT}
cmake --build .
# Install it to ../install
cmake --install . --prefix ${TEST_ROOT}/install
cd ..

# Test the config by building the sample project.
mkdir build_sample_proj
cd build_sample_proj
cmake -DCMAKE_PREFIX_PATH=${TEST_ROOT}/install ${TEST_SRC_DIR}
cmake --build .

./use_arg_parse --verbose
```