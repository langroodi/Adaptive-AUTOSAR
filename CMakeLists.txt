########################################################################
#
# Requirements and dependencies:

cmake_minimum_required(VERSION 3.14)
project(Adaptive_Platform)

# This AUTOSAR Adaptive Platform requires C++14
set(CMAKE_CXX_STANDARD 14)
# Enable 'rdyamic' switch to export the executable's symbols for debugging
set(CMAKE_ENABLE_EXPORTS 1)

include(FetchContent)

# Fetching GoogleTest 1.11.0 for unit testing
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/8d51ffdfab10b3fba636ae69bc03da4b54f8c235.zip
)

# Fetching Async-BSD-Socket-Lib for network abstraction
FetchContent_Declare(
  Async-BSD-Socket-Lib
  URL https://github.com/langroodi/Async-BSD-Socket-Lib/archive/510bd5e288ac827217191310d18fa93ca3ac0d50.zip
)

FetchContent_MakeAvailable(googletest Async-BSD-Socket-Lib)

########################################################################
#
# Options:

option(build_tests "Build all the tests." ON)

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

set(source_ara_com_cg_dir
  "${CMAKE_SOURCE_DIR}/src/ara/com/cg")

set(source_ara_com_helper_dir
  "${CMAKE_SOURCE_DIR}/src/ara/com/helper")

set(source_ara_com_option_dir
  "${CMAKE_SOURCE_DIR}/src/ara/com/option")  

set(source_ara_com_entry_dir
  "${CMAKE_SOURCE_DIR}/src/ara/com/entry")

set(source_ara_com_someip_dir
  "${CMAKE_SOURCE_DIR}/src/ara/com/someip")

set(source_ara_com_someip_sd_dir
  "${CMAKE_SOURCE_DIR}/src/ara/com/someip/sd")

set(source_ara_com_someip_sd_fsm_dir
  "${CMAKE_SOURCE_DIR}/src/ara/com/someip/sd/fsm")

set(source_ara_com_someip_pubsub_dir
  "${CMAKE_SOURCE_DIR}/src/ara/com/someip/pubsub")

set(source_ara_com_someip_pubsub_fsm_dir
  "${CMAKE_SOURCE_DIR}/src/ara/com/someip/pubsub/fsm")

set(source_ara_exec_dir
  "${CMAKE_SOURCE_DIR}/src/ara/exec")

# Test Directories:

set(test_ara_log_dir
  "${CMAKE_SOURCE_DIR}/test/ara/log")

set(test_ara_sm_dir
  "${CMAKE_SOURCE_DIR}/test/ara/sm")

set(test_ara_com_entry_dir
  "${CMAKE_SOURCE_DIR}/test/ara/com/entry")

set(test_ara_com_helper_dir
  "${CMAKE_SOURCE_DIR}/test/ara/com/helper")

set(test_ara_com_option_dir
  "${CMAKE_SOURCE_DIR}/test/ara/com/option")

set(test_ara_com_someip_pubsub_dir
  "${CMAKE_SOURCE_DIR}/test/ara/com/someip/pubsub")

set(test_ara_com_someip_pubsub_fsm_dir
  "${CMAKE_SOURCE_DIR}/test/ara/com/someip/pubsub/fsm")

set(test_ara_com_someip_sd_dir
  "${CMAKE_SOURCE_DIR}/test/ara/com/someip/sd")

set(test_ara_com_someip_sd_fsm_dir
  "${CMAKE_SOURCE_DIR}/test/ara/com/someip/sd/fsm")

set(test_ara_exec_dir
  "${CMAKE_SOURCE_DIR}/test/ara/exec")

set(test_ara_core_dir
  "${CMAKE_SOURCE_DIR}/test/ara/core")

########################################################################

add_library(
  ara_core
  ${source_ara_core_dir}/optional.h
  ${source_ara_core_dir}/result.h
  ${source_ara_core_dir}/error_domain.h
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
  ara_com
  ${source_ara_com_cg_dir}/communication_group_client.h
  ${source_ara_com_cg_dir}/communication_group_server.h
  ${source_ara_com_helper_dir}/payload_helper.h
  ${source_ara_com_helper_dir}/payload_helper.cpp
  ${source_ara_com_helper_dir}/ipv4_address.h
  ${source_ara_com_helper_dir}/ipv4_address.cpp
  ${source_ara_com_helper_dir}/abstract_state_machine.h
  ${source_ara_com_helper_dir}/machine_state.h
  ${source_ara_com_helper_dir}/finite_state_machine.h
  ${source_ara_com_helper_dir}/ttl_timer.h
  ${source_ara_com_helper_dir}/ttl_timer.cpp
  ${source_ara_com_helper_dir}/network_layer.h
  ${source_ara_com_entry_dir}/entry.h
  ${source_ara_com_entry_dir}/entry.cpp
  ${source_ara_com_entry_dir}/eventgroup_entry.h
  ${source_ara_com_entry_dir}/eventgroup_entry.cpp
  ${source_ara_com_entry_dir}/service_entry.h
  ${source_ara_com_entry_dir}/service_entry.cpp
  ${source_ara_com_entry_dir}/entry_deserializer.h
  ${source_ara_com_entry_dir}/entry_deserializer.cpp
  ${source_ara_com_option_dir}/option.h
  ${source_ara_com_option_dir}/option.cpp
  ${source_ara_com_option_dir}/ipv4_endpoint_option.h
  ${source_ara_com_option_dir}/ipv4_endpoint_option.cpp
  ${source_ara_com_option_dir}/loadbalancing_option.h
  ${source_ara_com_option_dir}/loadbalancing_option.cpp
  ${source_ara_com_option_dir}/option_deserializer.h
  ${source_ara_com_option_dir}/option_deserializer.cpp
  ${source_ara_com_someip_dir}/someip_message.h
  ${source_ara_com_someip_dir}/someip_message.cpp
  ${source_ara_com_someip_pubsub_dir}/someip_pubsub_server.h
  ${source_ara_com_someip_pubsub_dir}/someip_pubsub_server.cpp
  ${source_ara_com_someip_pubsub_dir}/someip_pubsub_client.h
  ${source_ara_com_someip_pubsub_dir}/someip_pubsub_client.cpp
  ${source_ara_com_someip_pubsub_fsm_dir}/service_down_state.h
  ${source_ara_com_someip_pubsub_fsm_dir}/service_down_state.cpp
  ${source_ara_com_someip_pubsub_fsm_dir}/notsubscribed_state.h
  ${source_ara_com_someip_pubsub_fsm_dir}/notsubscribed_state.cpp
  ${source_ara_com_someip_pubsub_fsm_dir}/subscribed_state.h
  ${source_ara_com_someip_pubsub_fsm_dir}/subscribed_state.cpp
  ${source_ara_com_someip_sd_dir}/someip_sd_agent.h
  ${source_ara_com_someip_sd_dir}/someip_sd_message.h
  ${source_ara_com_someip_sd_dir}/someip_sd_message.cpp
  ${source_ara_com_someip_sd_dir}/someip_sd_server.h
  ${source_ara_com_someip_sd_dir}/someip_sd_server.cpp
  ${source_ara_com_someip_sd_dir}/someip_sd_client.h
  ${source_ara_com_someip_sd_dir}/someip_sd_client.cpp
  ${source_ara_com_someip_sd_fsm_dir}/timer_set_state.h
  ${source_ara_com_someip_sd_fsm_dir}/client_service_state.h
  ${source_ara_com_someip_sd_fsm_dir}/notready_state.h
  ${source_ara_com_someip_sd_fsm_dir}/notready_state.cpp
  ${source_ara_com_someip_sd_fsm_dir}/initial_wait_state.h
  ${source_ara_com_someip_sd_fsm_dir}/client_initial_wait_state.h
  ${source_ara_com_someip_sd_fsm_dir}/client_initial_wait_state.cpp
  ${source_ara_com_someip_sd_fsm_dir}/repetition_state.h
  ${source_ara_com_someip_sd_fsm_dir}/client_repetition_state.h
  ${source_ara_com_someip_sd_fsm_dir}/client_repetition_state.cpp
  ${source_ara_com_someip_sd_fsm_dir}/main_state.h
  ${source_ara_com_someip_sd_fsm_dir}/main_state.cpp
  ${source_ara_com_someip_sd_fsm_dir}/stopped_state.h
  ${source_ara_com_someip_sd_fsm_dir}/stopped_state.cpp
  ${source_ara_com_someip_sd_fsm_dir}/service_notseen_state.h
  ${source_ara_com_someip_sd_fsm_dir}/service_notseen_state.cpp
  ${source_ara_com_someip_sd_fsm_dir}/service_ready_state.h
  ${source_ara_com_someip_sd_fsm_dir}/service_ready_state.cpp
  ${source_ara_com_someip_sd_fsm_dir}/service_seen_state.h
  ${source_ara_com_someip_sd_fsm_dir}/service_seen_state.cpp
)

add_library(
  ara_sm
  ${source_ara_sm_dir}/states.h
  ${source_ara_sm_dir}/trigger.h
  ${source_ara_sm_dir}/notifier.h
  ${source_ara_sm_dir}/trigger_in.h
  ${source_ara_sm_dir}/trigger_out.h
  ${source_ara_sm_dir}/trigger_inout.h
)

set_target_properties(
  ara_sm
  PROPERTIES LINKER_LANGUAGE CXX
)

add_library(
  ara_exec
  ${source_ara_exec_dir}/worker_thread.h
  ${source_ara_exec_dir}/worker_thread.cpp
  ${source_ara_exec_dir}/worker_runnable.h
)

set_target_properties(
  ara_exec
  PROPERTIES LINKER_LANGUAGE CXX
)

target_link_libraries(
  ara_log
  ara_core
  ara_com
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
    ${test_ara_sm_dir}/trigger_inout_test.cpp
    ${test_ara_sm_dir}/power_mode_test.cpp
    ${test_ara_com_entry_dir}/eventgroup_entry_test.cpp
    ${test_ara_com_entry_dir}/service_entry_test.cpp
    ${test_ara_com_helper_dir}/ipv4_address_test.cpp
    ${test_ara_com_helper_dir}/ttl_timer_test.cpp
    ${test_ara_com_helper_dir}/mockup_network_layer.h
    ${test_ara_com_option_dir}/ipv4_endpoint_option_test.cpp
    ${test_ara_com_option_dir}/loadbalancing_option_test.cpp
    ${test_ara_com_someip_pubsub_dir}/someip_pubsub_test.cpp
    ${test_ara_com_someip_pubsub_fsm_dir}/pubsub_state_test.cpp
    ${test_ara_com_someip_sd_dir}/someip_sd_message_test.cpp
    ${test_ara_com_someip_sd_dir}/network_abstraction_test.cpp
    ${test_ara_com_someip_sd_dir}/someip_sd_test.cpp
    ${test_ara_com_someip_sd_fsm_dir}/machine_state_test.cpp
    ${test_ara_exec_dir}/worker_thread_test.cpp
    ${test_ara_exec_dir}/worker_runnable_test.cpp
    ${test_ara_core_dir}/optional_test.cpp
    ${test_ara_core_dir}/result_test.cpp
    ${test_ara_core_dir}/result_void_test.cpp
    ${test_ara_core_dir}/error_domain_test.cpp
    ${test_ara_core_dir}/error_code_test.cpp
    ${test_ara_core_dir}/instance_specifier_test.cpp
  )

  target_link_libraries(
    ara_unit_test
    gtest_main
    async_bsd_socket_lib
    ara_log
    ara_sm
    ara_exec
  )

  include(GoogleTest)
  gtest_discover_tests(ara_unit_test)
 endif()