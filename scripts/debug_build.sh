#!/bin/sh
set -e -u

mkdir -p build/debug
cd build/debug

cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../..
cmake --build . --target arg_parse_coverage_report

