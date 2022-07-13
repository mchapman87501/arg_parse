# Build a version of the library that supports test coverage
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    append_coverage_compiler_flags()
endif()

string(REPLACE "src/" ";../src/" COV_SOURCES ${SOURCES})
message(STATUS "Coverage library sources: ${COV_SOURCES}")
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

setup_target_for_coverage_lcov(NAME arg_parse_coverage_report
    EXECUTABLE ctest
    DEPENDENCIES
    test_arg_parse
    EXCLUDE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*" "/opt/homebrew/*" "/Applications/*"
    "/usr/*")
