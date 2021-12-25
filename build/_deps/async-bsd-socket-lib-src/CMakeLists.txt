########################################################################
#
# Requirements and dependencies:

cmake_minimum_required(VERSION 3.14)
project(Async_BSD_Socket_Lib)

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
option(build_samples "Build all the samples." OFF)

########################################################################
#
# Include Directory:

set(include_dir
  "${PROJECT_SOURCE_DIR}/include")

# Source Directory:

set(source_dir
  "${PROJECT_SOURCE_DIR}/src")

# Test Directory:

set(test_dir
  "${PROJECT_SOURCE_DIR}/test")

# Samples Directory

set(sample_dir
  "${PROJECT_SOURCE_DIR}/samples")

########################################################################

include_directories(${include_dir})

add_library(
  async_bsd_socket_lib
  ${source_dir}/network_socket.cpp
  ${source_dir}/poller.cpp
  ${source_dir}/tcp_client.cpp
  ${source_dir}/tcp_listener.cpp
  ${source_dir}/udp_client.cpp
)

target_include_directories(async_bsd_socket_lib
  INTERFACE ${include_dir}
)

if(build_tests)
  enable_testing()

  add_executable(
    async_bsd_socket_lib_test
    ${test_dir}/network_socket_test.cpp
    ${test_dir}/tcp_communication_test.cpp
    ${test_dir}/udp_unicast_test.cpp
    ${test_dir}/udp_multicast_test.cpp
    ${test_dir}/poller_test.cpp
  )

  target_link_libraries(
    async_bsd_socket_lib_test
    gtest_main
    async_bsd_socket_lib
  )

  include(GoogleTest)
  gtest_discover_tests(async_bsd_socket_lib_test)
endif()

if(build_samples)
  add_executable(
    async_bsd_socket_lib_samples
    ${sample_dir}/poller_sample.cpp
  )

  target_link_libraries(
    async_bsd_socket_lib_samples
    pthread
    async_bsd_socket_lib
  )
endif()