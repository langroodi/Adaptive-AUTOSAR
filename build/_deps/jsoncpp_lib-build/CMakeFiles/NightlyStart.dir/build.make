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

# Utility rule file for NightlyStart.

# Include any custom commands dependencies for this target.
include _deps/jsoncpp_lib-build/CMakeFiles/NightlyStart.dir/compiler_depend.make

# Include the progress variables for this target.
include _deps/jsoncpp_lib-build/CMakeFiles/NightlyStart.dir/progress.make

_deps/jsoncpp_lib-build/CMakeFiles/NightlyStart:
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-build && /usr/local/bin/ctest -D NightlyStart

NightlyStart: _deps/jsoncpp_lib-build/CMakeFiles/NightlyStart
NightlyStart: _deps/jsoncpp_lib-build/CMakeFiles/NightlyStart.dir/build.make
.PHONY : NightlyStart

# Rule to build all files generated by this target.
_deps/jsoncpp_lib-build/CMakeFiles/NightlyStart.dir/build: NightlyStart
.PHONY : _deps/jsoncpp_lib-build/CMakeFiles/NightlyStart.dir/build

_deps/jsoncpp_lib-build/CMakeFiles/NightlyStart.dir/clean:
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-build && $(CMAKE_COMMAND) -P CMakeFiles/NightlyStart.dir/cmake_clean.cmake
.PHONY : _deps/jsoncpp_lib-build/CMakeFiles/NightlyStart.dir/clean

_deps/jsoncpp_lib-build/CMakeFiles/NightlyStart.dir/depend:
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-src /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-build /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-build/CMakeFiles/NightlyStart.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : _deps/jsoncpp_lib-build/CMakeFiles/NightlyStart.dir/depend

