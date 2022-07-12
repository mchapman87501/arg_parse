# Overview

This is a small library to help with command-line option/argument parsing.

Why not just use an existing argument parsing package?  Writing one for myself provided an excuse to use C++20 language features.

## Building

### On Host

#### Dependencies

In order to build and test on macOS or linux you'll need the following (at least):
* [CMake](https://www.cmake.org)
* A C++ compiler ;)
* [Catch2](https://github.com/catchorg/Catch2)
* [lcov](https://github.com/linux-test-project/lcov.git)
* [OpenCV](https://docs.opencv.org/4.5.5/) 
* [LibRaw](https://www.libraw.org/docs/API-CXX.html)

#### Compilation

To compile:
```shell
mkdir -p build/release
cd build/release
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../..
cmake --build .
```

### Using Docker

```shell
sh ./docker_scripts/on_host/create_image.sh
sh ./docker_scripts/on_host/run_build.sh
# Check './build_artifacts' for built executables/libraries.
```

## Running Tests

### On Host

```shell
mkdir -p build/debug
cd build/debug
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../..
cmake --build . --target coverage_report
# If all tests pass:
open ./coverage_report/index.html
```

### Using Docker

```shell
sh ./docker_scripts/on_host/create_image.sh
sh ./docker_scripts/on_host/run_tests.sh
# If all tests pass:
open ./build_artifacts/coverage_report/index.html
```


## Formatting with clang-format

If you have both [clang-format](https://clang.llvm.org/docs/ClangFormat.html) and [fd](https://github.com/sharkdp/fd.git) (an alternative to `find`) on your PATH:

```shell
clang-format -i $(fd '.*\.(cpp|hpp)')
```

## Static Analysis with clang-tidy

If you have [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) on your PATH:

```shell
mkdir -p build/debug
cd build/debug
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../..
clang-tidy ../../src/*.cpp ../../tests/src/*.cpp
```
