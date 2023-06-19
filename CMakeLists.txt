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

# Fetching GoogleTest 1.12.1 for unit testing
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
)

# Fetching libcurl 7.88.0 for REST API
FetchContent_Declare(
  libcurl
  URL https://github.com/curl/curl/archive/3027611ca6d4cc5510d2d0fccc3e5a074e09a2fb.zip
)

# Fetching JsonCpp 1.9.5 for JSON parsing
FetchContent_Declare(
  jsoncpp_lib
  URL https://github.com/open-source-parsers/jsoncpp/archive/5defb4ed1a4293b8e2bf641e16b156fb9de498cc.zip
)

# Fetching Async-BSD-Socket-Lib for network abstraction
FetchContent_Declare(
  Async-BSD-Socket-Lib
  URL https://github.com/langroodi/Async-BSD-Socket-Lib/archive/8c84da8f4b5e7552b41a54c92acb0b4565d07024.zip
)

# Fetching  OBD-II-Emulator for OBD2 CAN bus communication
FetchContent_Declare(
  OBD-II-Emulator
  URL https://github.com/langroodi/OBD-II-Emulator/archive/2c33042a6397d7030f3bafb57b6b88b913d95cb2.zip
)

# Fetching  Doip-Lib for DoIP communication
FetchContent_Declare(
  Doip-Lib
  URL https://github.com/langroodi/Doip-Lib/archive/5acfd483eb3dc90429a98dbe4b70de8b1db68af6.zip
)

FetchContent_MakeAvailable(
  googletest
  libcurl
  jsoncpp_lib
  Async-BSD-Socket-Lib
  OBD-II-Emulator
  Doip-Lib
)

########################################################################
#
# Options:

option(build_tests "Build all the tests." ON)

########################################################################
#
# Source Directories:

set(source_dir
  "${CMAKE_SOURCE_DIR}/src")

set(source_arxml_dir
  "${CMAKE_SOURCE_DIR}/src/arxml")

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

set(source_ara_com_e2e_dir
  "${CMAKE_SOURCE_DIR}/src/ara/com/e2e")

set(source_ara_com_helper_dir
  "${CMAKE_SOURCE_DIR}/src/ara/com/helper")

set(source_ara_com_option_dir
  "${CMAKE_SOURCE_DIR}/src/ara/com/option")  

set(source_ara_com_entry_dir
  "${CMAKE_SOURCE_DIR}/src/ara/com/entry")

set(source_ara_com_someip_dir
  "${CMAKE_SOURCE_DIR}/src/ara/com/someip")

set(source_ara_com_someip_rpc_dir
  "${CMAKE_SOURCE_DIR}/src/ara/com/someip/rpc")

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

set(source_ara_exec_helper_dir
  "${CMAKE_SOURCE_DIR}/src/ara/exec/helper")

set(source_ara_diag_dir
  "${CMAKE_SOURCE_DIR}/src/ara/diag")

set(source_ara_diag_routing_dir
  "${CMAKE_SOURCE_DIR}/src/ara/diag/routing")

set(source_ara_diag_debouncing_dir
  "${CMAKE_SOURCE_DIR}/src/ara/diag/debouncing")

set(source_ara_phm_dir
  "${CMAKE_SOURCE_DIR}/src/ara/phm")

set(source_ara_phm_supervisors_dir
  "${CMAKE_SOURCE_DIR}/src/ara/phm/supervisors")

set(source_application_dir
  "${CMAKE_SOURCE_DIR}/src/application")

set(source_application_doip_dir
  "${CMAKE_SOURCE_DIR}/src/application/doip")

set(source_application_helper_dir
  "${CMAKE_SOURCE_DIR}/src/application/helper")

set(source_application_platform_dir
  "${CMAKE_SOURCE_DIR}/src/application/platform")

# Test Directories:

set(test_arxml_dir
  "${CMAKE_SOURCE_DIR}/test/arxml")

set(test_ara_core_dir
  "${CMAKE_SOURCE_DIR}/test/ara/core")

set(test_ara_log_dir
  "${CMAKE_SOURCE_DIR}/test/ara/log")

set(test_ara_sm_dir
  "${CMAKE_SOURCE_DIR}/test/ara/sm")

set(test_ara_com_e2e_dir
  "${CMAKE_SOURCE_DIR}/test/ara/com/e2e")

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

set(test_ara_com_someip_rpc_dir
  "${CMAKE_SOURCE_DIR}/test/ara/com/someip/rpc")

set(test_ara_com_someip_sd_dir
  "${CMAKE_SOURCE_DIR}/test/ara/com/someip/sd")

set(test_ara_com_someip_sd_fsm_dir
  "${CMAKE_SOURCE_DIR}/test/ara/com/someip/sd/fsm")

set(test_ara_exec_dir
  "${CMAKE_SOURCE_DIR}/test/ara/exec")

set(test_ara_exec_helper_dir
  "${CMAKE_SOURCE_DIR}/test/ara/exec/helper")

set(test_ara_diag_dir
  "${CMAKE_SOURCE_DIR}/test/ara/diag")

set(test_ara_diag_routing_dir
  "${CMAKE_SOURCE_DIR}/test/ara/diag/routing")

set(test_ara_diag_debouncing_dir
  "${CMAKE_SOURCE_DIR}/test/ara/diag/debouncing")

set(test_ara_phm_dir
  "${CMAKE_SOURCE_DIR}/test/ara/phm")

set(test_ara_phm_supervisors_dir
  "${CMAKE_SOURCE_DIR}/test/ara/phm/supervisors")

########################################################################

add_library(
  arxml
  ${source_arxml_dir}/pugiconfig.hpp
  ${source_arxml_dir}/pugixml.hpp
  ${source_arxml_dir}/pugixml.cpp
  ${source_arxml_dir}/arxml_node.h
  ${source_arxml_dir}/arxml_node.cpp
  ${source_arxml_dir}/arxml_node_iterator.h
  ${source_arxml_dir}/arxml_node_range.h
  ${source_arxml_dir}/arxml_reader.h
  ${source_arxml_dir}/arxml_reader.cpp
)

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
  ${source_ara_com_e2e_dir}/profile11.h
  ${source_ara_com_e2e_dir}/profile11.cpp
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
  ${source_ara_com_helper_dir}/concurrent_queue.h
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
  ${source_ara_com_someip_rpc_dir}/someip_rpc_message.h
  ${source_ara_com_someip_rpc_dir}/someip_rpc_message.cpp
  ${source_ara_com_someip_rpc_dir}/rpc_server.h
  ${source_ara_com_someip_rpc_dir}/rpc_server.cpp
  ${source_ara_com_someip_rpc_dir}/socket_rpc_server.h
  ${source_ara_com_someip_rpc_dir}/socket_rpc_server.cpp
  ${source_ara_com_someip_rpc_dir}/rpc_client.h
  ${source_ara_com_someip_rpc_dir}/rpc_client.cpp
  ${source_ara_com_someip_rpc_dir}/socket_rpc_client.h
  ${source_ara_com_someip_rpc_dir}/socket_rpc_client.cpp
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
  ${source_ara_com_someip_sd_dir}/sd_network_layer.h
  ${source_ara_com_someip_sd_dir}/sd_network_layer.cpp
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
  ${source_ara_exec_dir}/deterministic_client.h
  ${source_ara_exec_dir}/deterministic_client.cpp
  ${source_ara_exec_dir}/execution_client.h
  ${source_ara_exec_dir}/execution_client.cpp
  ${source_ara_exec_dir}/execution_server.h
  ${source_ara_exec_dir}/execution_server.cpp
  ${source_ara_exec_dir}/function_group.h
  ${source_ara_exec_dir}/function_group.cpp
  ${source_ara_exec_dir}/function_group_state.h
  ${source_ara_exec_dir}/function_group_state.cpp
  ${source_ara_exec_dir}/exec_error_domain.h
  ${source_ara_exec_dir}/exec_error_domain.cpp
  ${source_ara_exec_dir}/exec_exception.h
  ${source_ara_exec_dir}/exec_exception.cpp
  ${source_ara_exec_dir}/execution_error_event.h
  ${source_ara_exec_dir}/state_client.h
  ${source_ara_exec_dir}/state_client.cpp
  ${source_ara_exec_dir}/state_server.h
  ${source_ara_exec_dir}/state_server.cpp
  ${source_ara_exec_helper_dir}/atomic_optional.h
  ${source_ara_exec_helper_dir}/modelled_process.h
  ${source_ara_exec_helper_dir}/modelled_process.cpp
)

add_library(
  ara_diag
  ${source_ara_diag_dir}/meta_info.h
  ${source_ara_diag_dir}/meta_info.cpp
  ${source_ara_diag_dir}/cancellation_handler.h
  ${source_ara_diag_dir}/cancellation_handler.cpp
  ${source_ara_diag_dir}/event.h
  ${source_ara_diag_dir}/event.cpp
  ${source_ara_diag_dir}/dtc_information.h
  ${source_ara_diag_dir}/dtc_information.cpp
  ${source_ara_diag_dir}/condition.h
  ${source_ara_diag_dir}/condition.cpp
  ${source_ara_diag_dir}/operation_cycle.h
  ${source_ara_diag_dir}/operation_cycle.cpp
  ${source_ara_diag_dir}/conversation.h
  ${source_ara_diag_dir}/conversation.cpp
  ${source_ara_diag_dir}/diag_error_domain.h
  ${source_ara_diag_dir}/diag_error_domain.cpp
  ${source_ara_diag_dir}/generic_uds_service.h
  ${source_ara_diag_dir}/security_access.h
  ${source_ara_diag_dir}/security_access.cpp
  ${source_ara_diag_dir}/download.h
  ${source_ara_diag_dir}/download.cpp
  ${source_ara_diag_dir}/upload.h
  ${source_ara_diag_dir}/upload.cpp
  ${source_ara_diag_dir}/ecu_reset_request.h
  ${source_ara_diag_dir}/ecu_reset_request.cpp
  ${source_ara_diag_dir}/generic_routine.h
  ${source_ara_diag_dir}/generic_routine.cpp
  ${source_ara_diag_dir}/monitor.h
  ${source_ara_diag_dir}/monitor.cpp
  ${source_ara_diag_routing_dir}/routable_uds_service.h
  ${source_ara_diag_routing_dir}/routable_uds_service.cpp
  ${source_ara_diag_routing_dir}/uds_service_router.h
  ${source_ara_diag_routing_dir}/uds_service_router.cpp
  ${source_ara_diag_routing_dir}/delay_timer.h
  ${source_ara_diag_routing_dir}/delay_timer.cpp
  ${source_ara_diag_routing_dir}/transfer_data.h
  ${source_ara_diag_routing_dir}/transfer_data.cpp
  ${source_ara_diag_routing_dir}/request_transfer_exit.h
  ${source_ara_diag_routing_dir}/request_transfer_exit.cpp
  ${source_ara_diag_routing_dir}/nrc_exception.h
  ${source_ara_diag_routing_dir}/nrc_exception.cpp
  ${source_ara_diag_routing_dir}/request_transfer.h
  ${source_ara_diag_routing_dir}/request_transfer.cpp
  ${source_ara_diag_debouncing_dir}/debouncer.h
  ${source_ara_diag_debouncing_dir}/debouncer.cpp
  ${source_ara_diag_debouncing_dir}/counter_based_debouncer.h
  ${source_ara_diag_debouncing_dir}/counter_based_debouncer.cpp
  ${source_ara_diag_debouncing_dir}/timer_based_debouncer.h
  ${source_ara_diag_debouncing_dir}/timer_based_debouncer.cpp
)

add_library(
  ara_phm
  ${source_ara_phm_dir}/checkpoint_communicator.h
  ${source_ara_phm_dir}/checkpoint_communicator.cpp
  ${source_ara_phm_dir}/supervised_entity.h
  ${source_ara_phm_dir}/supervised_entity.cpp
  ${source_ara_phm_dir}/recovery_action.h
  ${source_ara_phm_dir}/recovery_action.cpp
  ${source_ara_phm_supervisors_dir}/elementary_supervision.h
  ${source_ara_phm_supervisors_dir}/elementary_supervision.cpp
  ${source_ara_phm_supervisors_dir}/alive_supervision.h
  ${source_ara_phm_supervisors_dir}/alive_supervision.cpp
  ${source_ara_phm_supervisors_dir}/deadline_supervision.h
  ${source_ara_phm_supervisors_dir}/deadline_supervision.cpp
  ${source_ara_phm_supervisors_dir}/global_supervision.h
  ${source_ara_phm_supervisors_dir}/global_supervision.cpp
)

target_link_libraries(
  ara_com
  async_bsd_socket_lib
)

target_link_libraries(
  ara_exec
  ara_core
  async_bsd_socket_lib
)

target_link_libraries(
  ara_log
  ara_core
  ara_com
)

target_link_libraries(
  ara_diag
  ara_core
)

target_link_libraries(
  ara_phm
  ara_exec
)

add_executable(
  adaptive_autosar
  ${source_dir}/main.cpp
  ${source_application_dir}/extended_vehicle.h
  ${source_application_dir}/extended_vehicle.cpp
  ${source_application_helper_dir}/argument_configuration.h
  ${source_application_helper_dir}/argument_configuration.cpp
  ${source_application_helper_dir}/curl_wrapper.h
  ${source_application_helper_dir}/curl_wrapper.cpp
  ${source_application_helper_dir}/network_configuration.h
  ${source_application_helper_dir}/network_configuration.cpp
  ${source_application_helper_dir}/rpc_configuration.h
  ${source_application_helper_dir}/rpc_configuration.cpp
  ${source_application_helper_dir}/read_data_by_identifier.h
  ${source_application_helper_dir}/read_data_by_identifier.cpp
  ${source_application_helper_dir}/memory_cache.h
  ${source_application_helper_dir}/log_recovery_action.h
  ${source_application_helper_dir}/log_recovery_action.cpp
  ${source_application_helper_dir}/fifo_checkpoint_communicator.h
  ${source_application_helper_dir}/fifo_checkpoint_communicator.cpp
  ${source_application_doip_dir}/vehicle_id_request_handler.h
  ${source_application_doip_dir}/vehicle_id_request_handler.cpp
  ${source_application_doip_dir}/diag_message_handler.h
  ${source_application_doip_dir}/diag_message_handler.cpp
  ${source_application_doip_dir}/doip_server.h
  ${source_application_doip_dir}/doip_server.cpp
  ${source_application_doip_dir}/doip_client.h
  ${source_application_doip_dir}/doip_client.cpp
  ${source_application_doip_dir}/obd_to_doip_converter.h
  ${source_application_doip_dir}/obd_to_doip_converter.cpp
  ${source_application_platform_dir}/state_management.h
  ${source_application_platform_dir}/state_management.cpp
  ${source_application_platform_dir}/execution_management.h
  ${source_application_platform_dir}/execution_management.cpp
  ${source_application_platform_dir}/diagnostic_manager.h
  ${source_application_platform_dir}/diagnostic_manager.cpp
  ${source_application_platform_dir}/platform_health_management.h
  ${source_application_platform_dir}/platform_health_management.cpp
)

target_link_libraries(
  adaptive_autosar
  libcurl
  jsoncpp_lib
  obd_ii_emulator
  doip_lib
  arxml
  ara_exec
  ara_log
  ara_diag
  ara_phm
)

if(build_tests)
  enable_testing()

  add_executable(
    arxml_unit_test
    ${test_arxml_dir}/arxml_node_test.cpp
    ${test_arxml_dir}/arxml_reader_test.cpp
  )

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
    ${test_ara_com_e2e_dir}/profile11_test.cpp
    ${test_ara_com_entry_dir}/eventgroup_entry_test.cpp
    ${test_ara_com_entry_dir}/service_entry_test.cpp
    ${test_ara_com_helper_dir}/ipv4_address_test.cpp
    ${test_ara_com_helper_dir}/mockup_network_layer.h
    ${test_ara_com_helper_dir}/ttl_timer_test.cpp
    ${test_ara_com_helper_dir}/concurrent_queue_test.cpp
    ${test_ara_com_option_dir}/ipv4_endpoint_option_test.cpp
    ${test_ara_com_option_dir}/loadbalancing_option_test.cpp
    ${test_ara_com_someip_pubsub_dir}/someip_pubsub_test.cpp
    ${test_ara_com_someip_pubsub_fsm_dir}/pubsub_state_test.cpp
    ${test_ara_com_someip_rpc_dir}/someip_rpc_message_test.cpp
    ${test_ara_com_someip_rpc_dir}/rpc_server_test.cpp
    ${test_ara_com_someip_rpc_dir}/rpc_client_test.cpp
    ${test_ara_com_someip_sd_dir}/someip_sd_message_test.cpp
    ${test_ara_com_someip_sd_dir}/network_abstraction_test.cpp
    ${test_ara_com_someip_sd_dir}/someip_sd_test.cpp
    ${test_ara_com_someip_sd_fsm_dir}/machine_state_test.cpp
    ${test_ara_exec_dir}/worker_thread_test.cpp
    ${test_ara_exec_dir}/worker_runnable_test.cpp
    ${test_ara_exec_dir}/deterministic_client_test.cpp
    ${test_ara_exec_dir}/execution_client_test.cpp
    ${test_ara_exec_dir}/execution_server_test.cpp
    ${test_ara_exec_dir}/function_group_test.cpp
    ${test_ara_exec_dir}/function_group_state_test.cpp
    ${test_ara_exec_dir}/exec_exception_test.cpp
    ${test_ara_exec_dir}/state_client_test.cpp
    ${test_ara_exec_dir}/state_server_test.cpp
    ${test_ara_exec_helper_dir}/atomic_optional_test.cpp
    ${test_ara_exec_helper_dir}/mock_rpc_client.h
    ${test_ara_exec_helper_dir}/mock_rpc_server.h
    ${test_ara_exec_helper_dir}/modelled_process_test.cpp
    ${test_ara_core_dir}/optional_test.cpp
    ${test_ara_core_dir}/result_test.cpp
    ${test_ara_core_dir}/result_void_test.cpp
    ${test_ara_core_dir}/error_domain_test.cpp
    ${test_ara_core_dir}/error_code_test.cpp
    ${test_ara_core_dir}/instance_specifier_test.cpp
    ${test_ara_diag_dir}/obd_communication_test.cpp
    ${test_ara_diag_dir}/meta_info_test.cpp
    ${test_ara_diag_dir}/cancellation_handler_test.cpp
    ${test_ara_diag_dir}/event_test.cpp
    ${test_ara_diag_dir}/dtc_information_test.cpp
    ${test_ara_diag_dir}/condition_test.cpp
    ${test_ara_diag_dir}/operation_cycle_test.cpp
    ${test_ara_diag_dir}/conversation_test.cpp
    ${test_ara_diag_dir}/diag_error_domain_test.cpp
    ${test_ara_diag_dir}/generic_uds_service_test.cpp
    ${test_ara_diag_dir}/security_access_test.cpp
    ${test_ara_diag_dir}/ecu_reset_request_test.cpp
    ${test_ara_diag_dir}/generic_routine_test.cpp
    ${test_ara_diag_dir}/monitor_test.cpp
    ${test_ara_diag_dir}/doip_lib_test.cpp
    ${test_ara_diag_routing_dir}/testable_uds_service.h
    ${test_ara_diag_routing_dir}/uds_service_router_test.cpp
    ${test_ara_diag_routing_dir}/delay_timer_test.cpp
    ${test_ara_diag_routing_dir}/transfer_data_test.cpp
    ${test_ara_diag_routing_dir}/request_transfer_exit_test.cpp
    ${test_ara_diag_routing_dir}/nrc_exception_test.cpp
    ${test_ara_diag_routing_dir}/request_transfer_test.cpp
    ${test_ara_diag_debouncing_dir}/counter_based_debouncer_test.cpp
    ${test_ara_diag_debouncing_dir}/timer_based_debouncer_test.cpp
    ${test_ara_phm_dir}/recovery_action_test.cpp
    ${test_ara_phm_dir}/mocked_checkpoint_communicator.h
    ${test_ara_phm_dir}/supervised_entity_test.cpp
    ${test_ara_phm_supervisors_dir}/dummy_supervision.h
    ${test_ara_phm_supervisors_dir}/elementary_supervision_test.cpp
    ${test_ara_phm_supervisors_dir}/alive_supervision_test.cpp
    ${test_ara_phm_supervisors_dir}/deadline_supervision_test.cpp
    ${test_ara_phm_supervisors_dir}/global_supervision_test.cpp
  )

  target_link_libraries(
    arxml_unit_test
    gtest_main
    arxml
  )

  target_link_libraries(
    ara_unit_test
    gtest_main
    obd_ii_emulator
    doip_lib
    ara_log
    ara_sm
    ara_phm
    ara_diag
  )

  include(GoogleTest)
  gtest_discover_tests(arxml_unit_test)
  gtest_discover_tests(ara_unit_test)
 endif()