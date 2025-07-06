# Build a version of the library that supports test coverage
if(${CMAKE_BUILD_TYPE} STREQUAL "Profile")
    # https://gcovr.com/en/stable/cookbook.html#oos-cmake
    # This should work with both gcc and clang.
    set(CMAKE_C_FLAGS_PROFILE "-O0 --coverage")
    set(CMAKE_CXX_FLAGS_PROFILE "-O0 --coverage")
endif()

string(REPLACE "src/" ";../src/" COV_SOURCES ${SOURCES})

add_library(arg_parse_cov STATIC ${COV_SOURCES})
target_compile_features(arg_parse_cov PUBLIC cxx_std_20)
target_include_directories(arg_parse_cov PUBLIC ../include)

add_executable(test_arg_parse src/test_arg_parse.cpp src/arg_parse_result.cpp)
target_compile_features(test_arg_parse PUBLIC cxx_std_20)
target_include_directories(test_arg_parse PUBLIC include ../include)

# Prevent stripping unused code from the coverage build of the library.
target_link_libraries(test_arg_parse
    PUBLIC arg_parse_cov PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(test_arg_parse)

# *nix only:
add_custom_command(TARGET test_arg_parse
    PRE_BUILD
    COMMAND find ${CMAKE_BINARY_DIR} -name '*.gcda' -exec rm "{}" "';'"
)

# LCOV 2.0 appears to be the reason why I needed to change this.
# See discussion at
# https://bytemeta.vip/repo/linux-test-project/lcov/issues/220
set(REPORT_NAME arg_parse_coverage_report)

set(CLEAN_PROFILE_CMD
    lcov --directory ${CMAKE_BINARY_DIR} -b ${PROJECT_SOURCE_DIR}/src --zerocounters)

# Grrr: as of this writing, homebrew mac installs lcov 2.0.
# linux distros use lcov 1.x.
# 2.0 requires "--ignore-errors inconsistent,inconsistent" when running w.
# clang profiling output.  1.x does not understand those options.
execute_process(
    COMMAND lcov "--version"
    OUTPUT_VARIABLE LCOV_VERSION_OUT)

# Update 20250704 attempt to address lcov 2.3.1-1 warning
#   "WARNING: (unsupported) Function begin/end line exclusions not supported..."
# and subsequent genhtml errors:

# Presume LCOV version 1.x
set(LCOV_IGNORE_ARG "")
if(LCOV_VERSION_OUT MATCHES "LCOV version 2\.3")
    set(LCOV_IGNORE_ARG "--ignore-errors=unsupported,inconsistent")
elseif(LCOV_VERSION_OUT MATCHES "LCOV version 2\.")
    set(LCOV_IGNORE_ARG "--ignore-errors=inconsistent,inconsistent")
endif()

set(GENHTML_IGNORE_ARG "")
if(LCOV_VERSION_OUT MATCHES "LCOV version 2\.3")
    set(GENHTML_IGNORE_ARG "--ignore-errors=category")
endif()

set(CAPTURE_PROFILE_CMD
    lcov --capture --directory ${CMAKE_BINARY_DIR}
    -b ${PROJECT_SOURCE_DIR}/src
    --no-external
    ${LCOV_IGNORE_ARG}
    --output-file coverage.info)
set(GEN_REPORT_CMD
    genhtml
    ${GENHTML_IGNORE_ARG}
    --demangle-cpp
    --rc derive_function_end_line=0
    --output-directory ${CMAKE_BINARY_DIR}/${REPORT_NAME}
    coverage.info)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/${REPORT_NAME}/index.html
    COMMAND ${CLEAN_PROFILE_CMD}
    COMMAND ctest
    COMMAND ${CAPTURE_PROFILE_CMD}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${REPORT_NAME}
    COMMAND ${GEN_REPORT_CMD}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS test_arg_parse
)

add_custom_target(
    ${REPORT_NAME}
    DEPENDS ${CMAKE_BINARY_DIR}/${REPORT_NAME}/index.html
)
