#!/bin/sh
set -e -u


cd ${HOME}
echo "HOME is ${PWD}"
mkdir -p build/release
cd build/release

cmake -DCMAKE_BUILD_TYPE=Release /source
cmake --build .

mkdir -p /source/build_artifacts
cp ./libarg_parse.* /source/build_artifacts
