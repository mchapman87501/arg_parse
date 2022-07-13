# Overview

This is a small library to help with command-line option/argument parsing.

Why not just use an existing argument parsing package?  Writing one for myself provided an excuse to use C++20 language features.

![Github Docker CI](https://github.com/mchapman87501/arg_parse/actions/workflows/docker-image.yml/badge.svg)

## Examples

See `tests/package_configuration_tests/use_arg_parse.cpp`, and `tests/src/test_arg_parse.cpp`.


```c++
#include "arg_parse.hpp"
#include <iostream>

int main(int argc, char **argv) {
  auto parser = ArgParse::ArgumentParser::create("Example program");
  auto verbose = ArgParse::flag(parser, "-v", "--verbose", "Be verbose.");

  parser->parse_args(argc, argv);
  if (parser->should_exit()) {
    return parser->exit_code();
  }

  if (verbose->is_set()) {
    std::cout << "Here is some verbose output." << std::endl;
  }
  return 0;
}
```

## Building

### On Host

#### Dependencies

In order to build and test on macOS or linux you'll need the following (at least):
* [CMake](https://www.cmake.org)
* A C++ compiler ;)
* [Catch2](https://github.com/catchorg/Catch2)
* [lcov](https://github.com/linux-test-project/lcov.git)

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
