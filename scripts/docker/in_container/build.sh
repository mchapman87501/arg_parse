#!/bin/sh
set -e -u


BA_DIR=/source/build_artifacts

cd ${HOME}

mkdir -p build/release
cd build/release

cmake -DCMAKE_BUILD_TYPE=Release /source
cmake --build .
cmake --install . --prefix ${BA_DIR}/local
