# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build

# Include any dependencies generated for this target.
include CMakeFiles/adaptive_autosar.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/adaptive_autosar.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/adaptive_autosar.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/adaptive_autosar.dir/flags.make

CMakeFiles/adaptive_autosar.dir/src/main.cpp.o: CMakeFiles/adaptive_autosar.dir/flags.make
CMakeFiles/adaptive_autosar.dir/src/main.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/main.cpp
CMakeFiles/adaptive_autosar.dir/src/main.cpp.o: CMakeFiles/adaptive_autosar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/adaptive_autosar.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/adaptive_autosar.dir/src/main.cpp.o -MF CMakeFiles/adaptive_autosar.dir/src/main.cpp.o.d -o CMakeFiles/adaptive_autosar.dir/src/main.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/main.cpp

CMakeFiles/adaptive_autosar.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adaptive_autosar.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/main.cpp > CMakeFiles/adaptive_autosar.dir/src/main.cpp.i

CMakeFiles/adaptive_autosar.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adaptive_autosar.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/main.cpp -o CMakeFiles/adaptive_autosar.dir/src/main.cpp.s

CMakeFiles/adaptive_autosar.dir/src/application/extended_vehicle.cpp.o: CMakeFiles/adaptive_autosar.dir/flags.make
CMakeFiles/adaptive_autosar.dir/src/application/extended_vehicle.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/extended_vehicle.cpp
CMakeFiles/adaptive_autosar.dir/src/application/extended_vehicle.cpp.o: CMakeFiles/adaptive_autosar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/adaptive_autosar.dir/src/application/extended_vehicle.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/adaptive_autosar.dir/src/application/extended_vehicle.cpp.o -MF CMakeFiles/adaptive_autosar.dir/src/application/extended_vehicle.cpp.o.d -o CMakeFiles/adaptive_autosar.dir/src/application/extended_vehicle.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/extended_vehicle.cpp

CMakeFiles/adaptive_autosar.dir/src/application/extended_vehicle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adaptive_autosar.dir/src/application/extended_vehicle.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/extended_vehicle.cpp > CMakeFiles/adaptive_autosar.dir/src/application/extended_vehicle.cpp.i

CMakeFiles/adaptive_autosar.dir/src/application/extended_vehicle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adaptive_autosar.dir/src/application/extended_vehicle.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/extended_vehicle.cpp -o CMakeFiles/adaptive_autosar.dir/src/application/extended_vehicle.cpp.s

CMakeFiles/adaptive_autosar.dir/src/application/helper/argument_configuration.cpp.o: CMakeFiles/adaptive_autosar.dir/flags.make
CMakeFiles/adaptive_autosar.dir/src/application/helper/argument_configuration.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/argument_configuration.cpp
CMakeFiles/adaptive_autosar.dir/src/application/helper/argument_configuration.cpp.o: CMakeFiles/adaptive_autosar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/adaptive_autosar.dir/src/application/helper/argument_configuration.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/adaptive_autosar.dir/src/application/helper/argument_configuration.cpp.o -MF CMakeFiles/adaptive_autosar.dir/src/application/helper/argument_configuration.cpp.o.d -o CMakeFiles/adaptive_autosar.dir/src/application/helper/argument_configuration.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/argument_configuration.cpp

CMakeFiles/adaptive_autosar.dir/src/application/helper/argument_configuration.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adaptive_autosar.dir/src/application/helper/argument_configuration.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/argument_configuration.cpp > CMakeFiles/adaptive_autosar.dir/src/application/helper/argument_configuration.cpp.i

CMakeFiles/adaptive_autosar.dir/src/application/helper/argument_configuration.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adaptive_autosar.dir/src/application/helper/argument_configuration.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/argument_configuration.cpp -o CMakeFiles/adaptive_autosar.dir/src/application/helper/argument_configuration.cpp.s

CMakeFiles/adaptive_autosar.dir/src/application/helper/curl_wrapper.cpp.o: CMakeFiles/adaptive_autosar.dir/flags.make
CMakeFiles/adaptive_autosar.dir/src/application/helper/curl_wrapper.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/curl_wrapper.cpp
CMakeFiles/adaptive_autosar.dir/src/application/helper/curl_wrapper.cpp.o: CMakeFiles/adaptive_autosar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/adaptive_autosar.dir/src/application/helper/curl_wrapper.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/adaptive_autosar.dir/src/application/helper/curl_wrapper.cpp.o -MF CMakeFiles/adaptive_autosar.dir/src/application/helper/curl_wrapper.cpp.o.d -o CMakeFiles/adaptive_autosar.dir/src/application/helper/curl_wrapper.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/curl_wrapper.cpp

CMakeFiles/adaptive_autosar.dir/src/application/helper/curl_wrapper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adaptive_autosar.dir/src/application/helper/curl_wrapper.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/curl_wrapper.cpp > CMakeFiles/adaptive_autosar.dir/src/application/helper/curl_wrapper.cpp.i

CMakeFiles/adaptive_autosar.dir/src/application/helper/curl_wrapper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adaptive_autosar.dir/src/application/helper/curl_wrapper.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/curl_wrapper.cpp -o CMakeFiles/adaptive_autosar.dir/src/application/helper/curl_wrapper.cpp.s

CMakeFiles/adaptive_autosar.dir/src/application/helper/network_configuration.cpp.o: CMakeFiles/adaptive_autosar.dir/flags.make
CMakeFiles/adaptive_autosar.dir/src/application/helper/network_configuration.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/network_configuration.cpp
CMakeFiles/adaptive_autosar.dir/src/application/helper/network_configuration.cpp.o: CMakeFiles/adaptive_autosar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/adaptive_autosar.dir/src/application/helper/network_configuration.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/adaptive_autosar.dir/src/application/helper/network_configuration.cpp.o -MF CMakeFiles/adaptive_autosar.dir/src/application/helper/network_configuration.cpp.o.d -o CMakeFiles/adaptive_autosar.dir/src/application/helper/network_configuration.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/network_configuration.cpp

CMakeFiles/adaptive_autosar.dir/src/application/helper/network_configuration.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adaptive_autosar.dir/src/application/helper/network_configuration.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/network_configuration.cpp > CMakeFiles/adaptive_autosar.dir/src/application/helper/network_configuration.cpp.i

CMakeFiles/adaptive_autosar.dir/src/application/helper/network_configuration.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adaptive_autosar.dir/src/application/helper/network_configuration.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/network_configuration.cpp -o CMakeFiles/adaptive_autosar.dir/src/application/helper/network_configuration.cpp.s

CMakeFiles/adaptive_autosar.dir/src/application/helper/rpc_configuration.cpp.o: CMakeFiles/adaptive_autosar.dir/flags.make
CMakeFiles/adaptive_autosar.dir/src/application/helper/rpc_configuration.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/rpc_configuration.cpp
CMakeFiles/adaptive_autosar.dir/src/application/helper/rpc_configuration.cpp.o: CMakeFiles/adaptive_autosar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/adaptive_autosar.dir/src/application/helper/rpc_configuration.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/adaptive_autosar.dir/src/application/helper/rpc_configuration.cpp.o -MF CMakeFiles/adaptive_autosar.dir/src/application/helper/rpc_configuration.cpp.o.d -o CMakeFiles/adaptive_autosar.dir/src/application/helper/rpc_configuration.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/rpc_configuration.cpp

CMakeFiles/adaptive_autosar.dir/src/application/helper/rpc_configuration.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adaptive_autosar.dir/src/application/helper/rpc_configuration.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/rpc_configuration.cpp > CMakeFiles/adaptive_autosar.dir/src/application/helper/rpc_configuration.cpp.i

CMakeFiles/adaptive_autosar.dir/src/application/helper/rpc_configuration.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adaptive_autosar.dir/src/application/helper/rpc_configuration.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/rpc_configuration.cpp -o CMakeFiles/adaptive_autosar.dir/src/application/helper/rpc_configuration.cpp.s

CMakeFiles/adaptive_autosar.dir/src/application/helper/read_data_by_identifier.cpp.o: CMakeFiles/adaptive_autosar.dir/flags.make
CMakeFiles/adaptive_autosar.dir/src/application/helper/read_data_by_identifier.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/read_data_by_identifier.cpp
CMakeFiles/adaptive_autosar.dir/src/application/helper/read_data_by_identifier.cpp.o: CMakeFiles/adaptive_autosar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/adaptive_autosar.dir/src/application/helper/read_data_by_identifier.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/adaptive_autosar.dir/src/application/helper/read_data_by_identifier.cpp.o -MF CMakeFiles/adaptive_autosar.dir/src/application/helper/read_data_by_identifier.cpp.o.d -o CMakeFiles/adaptive_autosar.dir/src/application/helper/read_data_by_identifier.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/read_data_by_identifier.cpp

CMakeFiles/adaptive_autosar.dir/src/application/helper/read_data_by_identifier.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adaptive_autosar.dir/src/application/helper/read_data_by_identifier.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/read_data_by_identifier.cpp > CMakeFiles/adaptive_autosar.dir/src/application/helper/read_data_by_identifier.cpp.i

CMakeFiles/adaptive_autosar.dir/src/application/helper/read_data_by_identifier.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adaptive_autosar.dir/src/application/helper/read_data_by_identifier.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/helper/read_data_by_identifier.cpp -o CMakeFiles/adaptive_autosar.dir/src/application/helper/read_data_by_identifier.cpp.s

CMakeFiles/adaptive_autosar.dir/src/application/doip/vehicle_id_request_handler.cpp.o: CMakeFiles/adaptive_autosar.dir/flags.make
CMakeFiles/adaptive_autosar.dir/src/application/doip/vehicle_id_request_handler.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/vehicle_id_request_handler.cpp
CMakeFiles/adaptive_autosar.dir/src/application/doip/vehicle_id_request_handler.cpp.o: CMakeFiles/adaptive_autosar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/adaptive_autosar.dir/src/application/doip/vehicle_id_request_handler.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/adaptive_autosar.dir/src/application/doip/vehicle_id_request_handler.cpp.o -MF CMakeFiles/adaptive_autosar.dir/src/application/doip/vehicle_id_request_handler.cpp.o.d -o CMakeFiles/adaptive_autosar.dir/src/application/doip/vehicle_id_request_handler.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/vehicle_id_request_handler.cpp

CMakeFiles/adaptive_autosar.dir/src/application/doip/vehicle_id_request_handler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adaptive_autosar.dir/src/application/doip/vehicle_id_request_handler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/vehicle_id_request_handler.cpp > CMakeFiles/adaptive_autosar.dir/src/application/doip/vehicle_id_request_handler.cpp.i

CMakeFiles/adaptive_autosar.dir/src/application/doip/vehicle_id_request_handler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adaptive_autosar.dir/src/application/doip/vehicle_id_request_handler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/vehicle_id_request_handler.cpp -o CMakeFiles/adaptive_autosar.dir/src/application/doip/vehicle_id_request_handler.cpp.s

CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_message_handler.cpp.o: CMakeFiles/adaptive_autosar.dir/flags.make
CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_message_handler.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/doip_message_handler.cpp
CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_message_handler.cpp.o: CMakeFiles/adaptive_autosar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_message_handler.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_message_handler.cpp.o -MF CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_message_handler.cpp.o.d -o CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_message_handler.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/doip_message_handler.cpp

CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_message_handler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_message_handler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/doip_message_handler.cpp > CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_message_handler.cpp.i

CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_message_handler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_message_handler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/doip_message_handler.cpp -o CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_message_handler.cpp.s

CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_server.cpp.o: CMakeFiles/adaptive_autosar.dir/flags.make
CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_server.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/doip_server.cpp
CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_server.cpp.o: CMakeFiles/adaptive_autosar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_server.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_server.cpp.o -MF CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_server.cpp.o.d -o CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_server.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/doip_server.cpp

CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/doip_server.cpp > CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_server.cpp.i

CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/doip_server.cpp -o CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_server.cpp.s

CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_client.cpp.o: CMakeFiles/adaptive_autosar.dir/flags.make
CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_client.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/doip_client.cpp
CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_client.cpp.o: CMakeFiles/adaptive_autosar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_client.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_client.cpp.o -MF CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_client.cpp.o.d -o CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_client.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/doip_client.cpp

CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/doip_client.cpp > CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_client.cpp.i

CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/doip_client.cpp -o CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_client.cpp.s

CMakeFiles/adaptive_autosar.dir/src/application/doip/obd_to_doip_converter.cpp.o: CMakeFiles/adaptive_autosar.dir/flags.make
CMakeFiles/adaptive_autosar.dir/src/application/doip/obd_to_doip_converter.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/obd_to_doip_converter.cpp
CMakeFiles/adaptive_autosar.dir/src/application/doip/obd_to_doip_converter.cpp.o: CMakeFiles/adaptive_autosar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/adaptive_autosar.dir/src/application/doip/obd_to_doip_converter.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/adaptive_autosar.dir/src/application/doip/obd_to_doip_converter.cpp.o -MF CMakeFiles/adaptive_autosar.dir/src/application/doip/obd_to_doip_converter.cpp.o.d -o CMakeFiles/adaptive_autosar.dir/src/application/doip/obd_to_doip_converter.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/obd_to_doip_converter.cpp

CMakeFiles/adaptive_autosar.dir/src/application/doip/obd_to_doip_converter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adaptive_autosar.dir/src/application/doip/obd_to_doip_converter.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/obd_to_doip_converter.cpp > CMakeFiles/adaptive_autosar.dir/src/application/doip/obd_to_doip_converter.cpp.i

CMakeFiles/adaptive_autosar.dir/src/application/doip/obd_to_doip_converter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adaptive_autosar.dir/src/application/doip/obd_to_doip_converter.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/doip/obd_to_doip_converter.cpp -o CMakeFiles/adaptive_autosar.dir/src/application/doip/obd_to_doip_converter.cpp.s

CMakeFiles/adaptive_autosar.dir/src/application/platform/state_management.cpp.o: CMakeFiles/adaptive_autosar.dir/flags.make
CMakeFiles/adaptive_autosar.dir/src/application/platform/state_management.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/platform/state_management.cpp
CMakeFiles/adaptive_autosar.dir/src/application/platform/state_management.cpp.o: CMakeFiles/adaptive_autosar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/adaptive_autosar.dir/src/application/platform/state_management.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/adaptive_autosar.dir/src/application/platform/state_management.cpp.o -MF CMakeFiles/adaptive_autosar.dir/src/application/platform/state_management.cpp.o.d -o CMakeFiles/adaptive_autosar.dir/src/application/platform/state_management.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/platform/state_management.cpp

CMakeFiles/adaptive_autosar.dir/src/application/platform/state_management.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adaptive_autosar.dir/src/application/platform/state_management.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/platform/state_management.cpp > CMakeFiles/adaptive_autosar.dir/src/application/platform/state_management.cpp.i

CMakeFiles/adaptive_autosar.dir/src/application/platform/state_management.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adaptive_autosar.dir/src/application/platform/state_management.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/platform/state_management.cpp -o CMakeFiles/adaptive_autosar.dir/src/application/platform/state_management.cpp.s

CMakeFiles/adaptive_autosar.dir/src/application/platform/execution_management.cpp.o: CMakeFiles/adaptive_autosar.dir/flags.make
CMakeFiles/adaptive_autosar.dir/src/application/platform/execution_management.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/platform/execution_management.cpp
CMakeFiles/adaptive_autosar.dir/src/application/platform/execution_management.cpp.o: CMakeFiles/adaptive_autosar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/adaptive_autosar.dir/src/application/platform/execution_management.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/adaptive_autosar.dir/src/application/platform/execution_management.cpp.o -MF CMakeFiles/adaptive_autosar.dir/src/application/platform/execution_management.cpp.o.d -o CMakeFiles/adaptive_autosar.dir/src/application/platform/execution_management.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/platform/execution_management.cpp

CMakeFiles/adaptive_autosar.dir/src/application/platform/execution_management.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adaptive_autosar.dir/src/application/platform/execution_management.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/platform/execution_management.cpp > CMakeFiles/adaptive_autosar.dir/src/application/platform/execution_management.cpp.i

CMakeFiles/adaptive_autosar.dir/src/application/platform/execution_management.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adaptive_autosar.dir/src/application/platform/execution_management.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/platform/execution_management.cpp -o CMakeFiles/adaptive_autosar.dir/src/application/platform/execution_management.cpp.s

CMakeFiles/adaptive_autosar.dir/src/application/platform/diagnostic_manager.cpp.o: CMakeFiles/adaptive_autosar.dir/flags.make
CMakeFiles/adaptive_autosar.dir/src/application/platform/diagnostic_manager.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/platform/diagnostic_manager.cpp
CMakeFiles/adaptive_autosar.dir/src/application/platform/diagnostic_manager.cpp.o: CMakeFiles/adaptive_autosar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/adaptive_autosar.dir/src/application/platform/diagnostic_manager.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/adaptive_autosar.dir/src/application/platform/diagnostic_manager.cpp.o -MF CMakeFiles/adaptive_autosar.dir/src/application/platform/diagnostic_manager.cpp.o.d -o CMakeFiles/adaptive_autosar.dir/src/application/platform/diagnostic_manager.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/platform/diagnostic_manager.cpp

CMakeFiles/adaptive_autosar.dir/src/application/platform/diagnostic_manager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adaptive_autosar.dir/src/application/platform/diagnostic_manager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/platform/diagnostic_manager.cpp > CMakeFiles/adaptive_autosar.dir/src/application/platform/diagnostic_manager.cpp.i

CMakeFiles/adaptive_autosar.dir/src/application/platform/diagnostic_manager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adaptive_autosar.dir/src/application/platform/diagnostic_manager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/application/platform/diagnostic_manager.cpp -o CMakeFiles/adaptive_autosar.dir/src/application/platform/diagnostic_manager.cpp.s

# Object files for target adaptive_autosar
adaptive_autosar_OBJECTS = \
"CMakeFiles/adaptive_autosar.dir/src/main.cpp.o" \
"CMakeFiles/adaptive_autosar.dir/src/application/extended_vehicle.cpp.o" \
"CMakeFiles/adaptive_autosar.dir/src/application/helper/argument_configuration.cpp.o" \
"CMakeFiles/adaptive_autosar.dir/src/application/helper/curl_wrapper.cpp.o" \
"CMakeFiles/adaptive_autosar.dir/src/application/helper/network_configuration.cpp.o" \
"CMakeFiles/adaptive_autosar.dir/src/application/helper/rpc_configuration.cpp.o" \
"CMakeFiles/adaptive_autosar.dir/src/application/helper/read_data_by_identifier.cpp.o" \
"CMakeFiles/adaptive_autosar.dir/src/application/doip/vehicle_id_request_handler.cpp.o" \
"CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_message_handler.cpp.o" \
"CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_server.cpp.o" \
"CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_client.cpp.o" \
"CMakeFiles/adaptive_autosar.dir/src/application/doip/obd_to_doip_converter.cpp.o" \
"CMakeFiles/adaptive_autosar.dir/src/application/platform/state_management.cpp.o" \
"CMakeFiles/adaptive_autosar.dir/src/application/platform/execution_management.cpp.o" \
"CMakeFiles/adaptive_autosar.dir/src/application/platform/diagnostic_manager.cpp.o"

# External object files for target adaptive_autosar
adaptive_autosar_EXTERNAL_OBJECTS =

bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/src/main.cpp.o
bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/src/application/extended_vehicle.cpp.o
bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/src/application/helper/argument_configuration.cpp.o
bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/src/application/helper/curl_wrapper.cpp.o
bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/src/application/helper/network_configuration.cpp.o
bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/src/application/helper/rpc_configuration.cpp.o
bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/src/application/helper/read_data_by_identifier.cpp.o
bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/src/application/doip/vehicle_id_request_handler.cpp.o
bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_message_handler.cpp.o
bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_server.cpp.o
bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/src/application/doip/doip_client.cpp.o
bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/src/application/doip/obd_to_doip_converter.cpp.o
bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/src/application/platform/state_management.cpp.o
bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/src/application/platform/execution_management.cpp.o
bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/src/application/platform/diagnostic_manager.cpp.o
bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/build.make
bin/adaptive_autosar: _deps/libcurl-build/lib/libcurl-d.so.4.8.0
bin/adaptive_autosar: lib/libjsoncpp.so.1.9.5
bin/adaptive_autosar: lib/libobd_ii_emulator.so
bin/adaptive_autosar: lib/libdoip_lib.so
bin/adaptive_autosar: lib/libarxml.so
bin/adaptive_autosar: lib/libara_exec.so
bin/adaptive_autosar: lib/libara_log.so
bin/adaptive_autosar: lib/libara_diag.so
bin/adaptive_autosar: lib/libara_com.so
bin/adaptive_autosar: lib/libasync_bsd_socket_lib.so
bin/adaptive_autosar: lib/libara_core.so
bin/adaptive_autosar: CMakeFiles/adaptive_autosar.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Linking CXX executable bin/adaptive_autosar"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/adaptive_autosar.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/adaptive_autosar.dir/build: bin/adaptive_autosar
.PHONY : CMakeFiles/adaptive_autosar.dir/build

CMakeFiles/adaptive_autosar.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/adaptive_autosar.dir/cmake_clean.cmake
.PHONY : CMakeFiles/adaptive_autosar.dir/clean

CMakeFiles/adaptive_autosar.dir/depend:
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles/adaptive_autosar.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/adaptive_autosar.dir/depend

