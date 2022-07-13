cmake_policy(VERSION 3.22)
cmake_minimum_required(VERSION 3.22)
project(use_arg_parse)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(arg_parse)

add_executable(use_arg_parse use_arg_parse.cpp)
target_link_libraries(use_arg_parse PRIVATE arg_parse::arg_parse)
