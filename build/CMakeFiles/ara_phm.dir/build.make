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
include CMakeFiles/ara_phm.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ara_phm.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ara_phm.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ara_phm.dir/flags.make

CMakeFiles/ara_phm.dir/src/ara/phm/checkpoint_communicator.cpp.o: CMakeFiles/ara_phm.dir/flags.make
CMakeFiles/ara_phm.dir/src/ara/phm/checkpoint_communicator.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/ara/phm/checkpoint_communicator.cpp
CMakeFiles/ara_phm.dir/src/ara/phm/checkpoint_communicator.cpp.o: CMakeFiles/ara_phm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ara_phm.dir/src/ara/phm/checkpoint_communicator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ara_phm.dir/src/ara/phm/checkpoint_communicator.cpp.o -MF CMakeFiles/ara_phm.dir/src/ara/phm/checkpoint_communicator.cpp.o.d -o CMakeFiles/ara_phm.dir/src/ara/phm/checkpoint_communicator.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/ara/phm/checkpoint_communicator.cpp

CMakeFiles/ara_phm.dir/src/ara/phm/checkpoint_communicator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ara_phm.dir/src/ara/phm/checkpoint_communicator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/ara/phm/checkpoint_communicator.cpp > CMakeFiles/ara_phm.dir/src/ara/phm/checkpoint_communicator.cpp.i

CMakeFiles/ara_phm.dir/src/ara/phm/checkpoint_communicator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ara_phm.dir/src/ara/phm/checkpoint_communicator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/ara/phm/checkpoint_communicator.cpp -o CMakeFiles/ara_phm.dir/src/ara/phm/checkpoint_communicator.cpp.s

CMakeFiles/ara_phm.dir/src/ara/phm/supervised_entity.cpp.o: CMakeFiles/ara_phm.dir/flags.make
CMakeFiles/ara_phm.dir/src/ara/phm/supervised_entity.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/ara/phm/supervised_entity.cpp
CMakeFiles/ara_phm.dir/src/ara/phm/supervised_entity.cpp.o: CMakeFiles/ara_phm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ara_phm.dir/src/ara/phm/supervised_entity.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ara_phm.dir/src/ara/phm/supervised_entity.cpp.o -MF CMakeFiles/ara_phm.dir/src/ara/phm/supervised_entity.cpp.o.d -o CMakeFiles/ara_phm.dir/src/ara/phm/supervised_entity.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/ara/phm/supervised_entity.cpp

CMakeFiles/ara_phm.dir/src/ara/phm/supervised_entity.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ara_phm.dir/src/ara/phm/supervised_entity.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/ara/phm/supervised_entity.cpp > CMakeFiles/ara_phm.dir/src/ara/phm/supervised_entity.cpp.i

CMakeFiles/ara_phm.dir/src/ara/phm/supervised_entity.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ara_phm.dir/src/ara/phm/supervised_entity.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/ara/phm/supervised_entity.cpp -o CMakeFiles/ara_phm.dir/src/ara/phm/supervised_entity.cpp.s

CMakeFiles/ara_phm.dir/src/ara/phm/recovery_action.cpp.o: CMakeFiles/ara_phm.dir/flags.make
CMakeFiles/ara_phm.dir/src/ara/phm/recovery_action.cpp.o: /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/ara/phm/recovery_action.cpp
CMakeFiles/ara_phm.dir/src/ara/phm/recovery_action.cpp.o: CMakeFiles/ara_phm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ara_phm.dir/src/ara/phm/recovery_action.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ara_phm.dir/src/ara/phm/recovery_action.cpp.o -MF CMakeFiles/ara_phm.dir/src/ara/phm/recovery_action.cpp.o.d -o CMakeFiles/ara_phm.dir/src/ara/phm/recovery_action.cpp.o -c /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/ara/phm/recovery_action.cpp

CMakeFiles/ara_phm.dir/src/ara/phm/recovery_action.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ara_phm.dir/src/ara/phm/recovery_action.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/ara/phm/recovery_action.cpp > CMakeFiles/ara_phm.dir/src/ara/phm/recovery_action.cpp.i

CMakeFiles/ara_phm.dir/src/ara/phm/recovery_action.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ara_phm.dir/src/ara/phm/recovery_action.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/src/ara/phm/recovery_action.cpp -o CMakeFiles/ara_phm.dir/src/ara/phm/recovery_action.cpp.s

# Object files for target ara_phm
ara_phm_OBJECTS = \
"CMakeFiles/ara_phm.dir/src/ara/phm/checkpoint_communicator.cpp.o" \
"CMakeFiles/ara_phm.dir/src/ara/phm/supervised_entity.cpp.o" \
"CMakeFiles/ara_phm.dir/src/ara/phm/recovery_action.cpp.o"

# External object files for target ara_phm
ara_phm_EXTERNAL_OBJECTS =

lib/libara_phm.so: CMakeFiles/ara_phm.dir/src/ara/phm/checkpoint_communicator.cpp.o
lib/libara_phm.so: CMakeFiles/ara_phm.dir/src/ara/phm/supervised_entity.cpp.o
lib/libara_phm.so: CMakeFiles/ara_phm.dir/src/ara/phm/recovery_action.cpp.o
lib/libara_phm.so: CMakeFiles/ara_phm.dir/build.make
lib/libara_phm.so: lib/libara_exec.so
lib/libara_phm.so: lib/libara_core.so
lib/libara_phm.so: lib/libasync_bsd_socket_lib.so
lib/libara_phm.so: CMakeFiles/ara_phm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library lib/libara_phm.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ara_phm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ara_phm.dir/build: lib/libara_phm.so
.PHONY : CMakeFiles/ara_phm.dir/build

CMakeFiles/ara_phm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ara_phm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ara_phm.dir/clean

CMakeFiles/ara_phm.dir/depend:
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/CMakeFiles/ara_phm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ara_phm.dir/depend

