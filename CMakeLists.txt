cmake_policy(VERSION 3.22)
cmake_minimum_required(VERSION 3.22)

project(arg_parse VERSION 1.0.0)

include(GNUInstallDirs)

include(cmake_modules/CodeCoverage.cmake)

set(SOURCES
    src/argument_parser.cpp
    src/flag.cpp
    src/option.cpp
    src/parse_result.cpp
    src/help_fmt.cpp)

add_library(arg_parse SHARED ${SOURCES})
target_compile_features(arg_parse PUBLIC cxx_std_20)

# https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html?highlight=cmake_install_bindir
# Also:
# https://stackoverflow.com/q/54702582/2826337
target_include_directories(arg_parse
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>/arg_parse")

# https://stackoverflow.com/a/49858236/2826337
install(
    TARGETS arg_parse
    EXPORT arg_parseTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/arg_parse)

# file glob is considered bad.  This, on the other hand, is fragile.
set(HEADERS
    include/aliases.hpp
    include/arg_parse.hpp
    include/argument_parser.hpp
    include/argument.hpp
    include/flag.hpp
    include/help_fmt.hpp
    include/i_argument.hpp
    include/i_option.hpp
    include/nargs.hpp
    include/option.hpp
    include/parse_result.hpp
    include/value_converter.hpp)

install(
    FILES ${HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/arg_parse")

install(
    EXPORT arg_parseTargets
    FILE arg_parseTargets.cmake
    NAMESPACE arg_parse::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/arg_parse")

find_package(Catch2 3 REQUIRED)
enable_testing()
include(CTest)
include(Catch)

add_subdirectory(tests)