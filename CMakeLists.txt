########################################################################
#
# Requirements and dependencies:

cmake_minimum_required(VERSION 3.14)
project(Adaptive_Platform)

# This AUTOSAR Adaptive Platform requires C++14
set(CMAKE_CXX_STANDARD 14)
# Enable 'rdyamic' switch to export the executable's symbols for debugging
set(CMAKE_ENABLE_EXPORTS 1)
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
option(gtest_disable_pthreads "Disable uses of pthreads in gtest." OFF)

########################################################################
#
# Source Directories:

set(source_ara_core_dir
  "${CMAKE_SOURCE_DIR}/src/ara/core")

set(source_ara_log_dir
  "${CMAKE_SOURCE_DIR}/src/ara/log")

set(source_ara_log_sink_dir
  "${CMAKE_SOURCE_DIR}/src/ara/log/sink")

set(source_ara_sm_dir
  "${CMAKE_SOURCE_DIR}/src/ara/sm")

# Test Directories:

set(test_ara_log_dir
  "${CMAKE_SOURCE_DIR}/test/ara/log")

set(test_ara_sm_dir
  "${CMAKE_SOURCE_DIR}/test/ara/sm")

########################################################################

add_library(
  ara_core
  ${source_ara_core_dir}/error_code.h
  ${source_ara_core_dir}/error_code.cpp
  ${source_ara_core_dir}/instance_specifier.h
  ${source_ara_core_dir}/instance_specifier.cpp
)

add_library(
  ara_log
  ${source_ara_log_dir}/argument.h
  ${source_ara_log_dir}/log_stream.h
  ${source_ara_log_dir}/log_stream.cpp
  ${source_ara_log_dir}/logger.h
  ${source_ara_log_dir}/logger.cpp
  ${source_ara_log_dir}/logging_framework.h
  ${source_ara_log_dir}/logging_framework.cpp
  ${source_ara_log_sink_dir}/console_log_sink.h
  ${source_ara_log_sink_dir}/console_log_sink.cpp
  ${source_ara_log_sink_dir}/file_log_sink.h
  ${source_ara_log_sink_dir}/file_log_sink.cpp
  ${source_ara_log_sink_dir}/log_sink.h
  ${source_ara_log_sink_dir}/log_sink.cpp
)

add_library(
  ara_sm
  ${source_ara_sm_dir}/states.h
  ${source_ara_sm_dir}/trigger.h
  ${source_ara_sm_dir}/notifier.h
  ${source_ara_sm_dir}/trigger_in.h
  ${source_ara_sm_dir}/trigger_out.h
)

set_target_properties(
  ara_sm
  PROPERTIES LINKER_LANGUAGE CXX
)

target_link_libraries(
  ara_log
  ara_core
)

if(build_tests)
  enable_testing()

  add_executable(
    ara_unit_test
    ${test_ara_log_dir}/argument_test.cpp
    ${test_ara_log_dir}/log_stream_test.cpp
    ${test_ara_log_dir}/logger_test.cpp
    ${test_ara_log_dir}/logging_framework_test.cpp
    ${test_ara_sm_dir}/trigger_in_test.cpp
    ${test_ara_sm_dir}/trigger_out_test.cpp
  )

  target_link_libraries(
    ara_unit_test
    gtest_main
    ara_log
    ara_sm
  )

  include(GoogleTest)
  gtest_discover_tests(ara_unit_test)
 endif()