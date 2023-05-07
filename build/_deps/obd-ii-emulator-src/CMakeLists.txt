########################################################################
#
# Requirements and dependencies:

cmake_minimum_required(VERSION 3.14)
project(OBD_II_Emulator)

set(CMAKE_CXX_STANDARD 14)
# Enable 'rdyamic' switch to export the executable's symbols for debugging
set(CMAKE_ENABLE_EXPORTS 1)
# Fetching GoogleTest 1.12.1 for unit testing
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
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

# Test Directories:

set(test_dir
  "${PROJECT_SOURCE_DIR}/test")

set(test_helpers_dir
  "${PROJECT_SOURCE_DIR}/test/helpers")

# Samples Directory

set(sample_dir
  "${PROJECT_SOURCE_DIR}/samples")

########################################################################

include_directories(${include_dir})

add_library(
  obd_ii_emulator
  ${source_dir}/can_frame.cpp
  ${source_dir}/can_driver.cpp
  ${source_dir}/obd_service.cpp
  ${source_dir}/communication_layer.cpp
  ${source_dir}/obd_emulator.cpp
  ${source_dir}/packet_buffer.cpp
  ${source_dir}/serial_communication.cpp
)

target_link_libraries(
  obd_ii_emulator
  pthread
)

target_include_directories(obd_ii_emulator
  INTERFACE ${include_dir}
)

if(build_tests)
  enable_testing()

  add_executable(
    obd_ii_emulator_test
    ${test_helpers_dir}/loopback_communication_layer.cpp
    ${test_helpers_dir}/dummy_obd_service.cpp
    ${test_dir}/can_frame_test.cpp
    ${test_dir}/can_driver_test.cpp
    ${test_dir}/obd_service_test.cpp
    ${test_dir}/communication_layer_test.cpp
    ${test_dir}/obd_emulator_test.cpp
  )

  target_link_libraries(
    obd_ii_emulator_test
    gtest_main
    obd_ii_emulator
  )

  include(GoogleTest)
  gtest_discover_tests(obd_ii_emulator_test)
endif()

if(build_samples)
  add_executable(
    obd_emulator_sample
    ${sample_dir}/current_data_obd_service.cpp
    ${sample_dir}/obd_emulator_sample.cpp
  )

  target_link_libraries(
    obd_emulator_sample
    obd_ii_emulator
  )
endif()