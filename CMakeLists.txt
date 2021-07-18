########################################################################
#
# Requirements and dependencies:

cmake_minimum_required(VERSION 3.14)
project(Adaptive_Platform)

# This AUTOSAR Adaptive Platform requires C++14
set(CMAKE_CXX_STANDARD 14)
# dsd dsds 
# Fetching GoogleTest 1.11.0 for unit testing
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/8d51ffdfab10b3fba636ae69bc03da4b54f8c235.zip
)

FetchContent_MakeAvailable(googletest)

########################################################################
#
# Options:

option(build_tests "Build all the tests." ON)

########################################################################
#
# Directories:

set(source_ara_log_dir
  "${CMAKE_SOURCE_DIR}/src/ara/log")

set(test_ara_log_dir
  "${CMAKE_SOURCE_DIR}/test/ara/log")

########################################################################

add_library(
  ara_log
  ${source_ara_log_dir}/argument.h
  ${source_ara_log_dir}/argument.cpp
)

if(build_tests)
  enable_testing()

  add_executable(
    ara_log_test
    ${test_ara_log_dir}/argument_test.cpp
  )

  target_link_libraries(
    ara_log_test
    gtest_main
    ara_log
  )

  include(GoogleTest)
  gtest_discover_tests(ara_log_test)
 endif()