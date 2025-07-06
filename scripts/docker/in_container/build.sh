#!/bin/sh
set -e -u


BA_DIR=/source/build_artifacts

cd ${HOME}

cmake --preset release --fresh
cmake --build --preset release
cmake --install build/release --prefix=${BA_DIR}/local