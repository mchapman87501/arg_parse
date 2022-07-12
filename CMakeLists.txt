cmake_policy(VERSION 3.22)
project(arg_parse VERSION 1.0.0)

cmake_minimum_required(VERSION 3.22)

include(cmake_modules/CodeCoverage.cmake)

set(SOURCES
    src/argument_parser.cpp
    src/flag.cpp
    src/option.cpp
    src/parse_result.cpp
    src/help_fmt.cpp)

add_library(arg_parse SHARED ${SOURCES})
target_compile_features(arg_parse PUBLIC cxx_std_20)
target_include_directories(arg_parse PUBLIC include)

find_package(Catch2 3 REQUIRED)
enable_testing()
include(CTest)
include(Catch)

add_subdirectory(tests)