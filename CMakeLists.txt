cmake_policy(VERSION 3.22)
cmake_minimum_required(VERSION 3.22)

set(version_major 1)
set(version_minor 0)
set(version_patch 0)
set(version ${version_major}.${version_minor}.${version_patch})
project(arg_parse VERSION ${version})

set(SOURCES
    src/argument_parser.cpp
    src/flag.cpp
    src/option.cpp
    src/parse_result.cpp
    src/help_fmt.cpp)

add_library(arg_parse SHARED ${SOURCES})
target_compile_features(arg_parse PUBLIC cxx_std_20)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html?highlight=cmake_install_bindir
# Also:
# https://stackoverflow.com/q/54702582/2826337
target_include_directories(arg_parse
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include/arg_parse>")

# https://stackoverflow.com/a/49858236/2826337
install(
    TARGETS arg_parse
    EXPORT arg_parse_targets
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
    EXPORT arg_parse_targets
    FILE arg_parse-targets.cmake
    NAMESPACE arg_parse::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/arg_parse")

# CMake config info
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/arg_parse-config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/arg_parse-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/arg_parse
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/arg_parse-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/arg_parse-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/arg_parse
)

# https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html?highlight=cmake_install_includedir
# Define package properties
set_property(TARGET arg_parse PROPERTY VERSION ${version})
set_property(TARGET arg_parse PROPERTY SOVERSION ${version_major})
set_property(TARGET arg_parse PROPERTY INTERFACE_arg_parse_MAJOR_VERSION ${version_major})
set_property(TARGET arg_parse APPEND PROPERTY COMPATIBLE_INTERFACE_STRING arg_parse_MAJOR_VERSION)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/arg_parse-config-version.cmake"
    VERSION "${version}"
    COMPATIBILITY AnyNewerVersion
)

# For direct consumption of the package, from source.
export(EXPORT arg_parse_targets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/arg_parse-targets.cmake"
    NAMESPACE arg_parse::
)

include(cmake_modules/CodeCoverage.cmake)
find_package(Catch2 3 REQUIRED)
enable_testing()

include(CTest)
include(Catch)

add_subdirectory(tests)