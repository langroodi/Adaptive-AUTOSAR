# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_SOURCE_DIR = /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild

# Utility rule file for jsoncpp_lib-populate.

# Include any custom commands dependencies for this target.
include CMakeFiles/jsoncpp_lib-populate.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/jsoncpp_lib-populate.dir/progress.make

CMakeFiles/jsoncpp_lib-populate: CMakeFiles/jsoncpp_lib-populate-complete

CMakeFiles/jsoncpp_lib-populate-complete: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-install
CMakeFiles/jsoncpp_lib-populate-complete: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-mkdir
CMakeFiles/jsoncpp_lib-populate-complete: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-download
CMakeFiles/jsoncpp_lib-populate-complete: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-update
CMakeFiles/jsoncpp_lib-populate-complete: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-patch
CMakeFiles/jsoncpp_lib-populate-complete: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-configure
CMakeFiles/jsoncpp_lib-populate-complete: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-build
CMakeFiles/jsoncpp_lib-populate-complete: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-install
CMakeFiles/jsoncpp_lib-populate-complete: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-test
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'jsoncpp_lib-populate'"
	/usr/local/bin/cmake -E make_directory /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/CMakeFiles
	/usr/local/bin/cmake -E touch /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/CMakeFiles/jsoncpp_lib-populate-complete
	/usr/local/bin/cmake -E touch /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-done

jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-build: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No build step for 'jsoncpp_lib-populate'"
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-build && /usr/local/bin/cmake -E echo_append
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-build && /usr/local/bin/cmake -E touch /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-build

jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-configure: jsoncpp_lib-populate-prefix/tmp/jsoncpp_lib-populate-cfgcmd.txt
jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-configure: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "No configure step for 'jsoncpp_lib-populate'"
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-build && /usr/local/bin/cmake -E echo_append
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-build && /usr/local/bin/cmake -E touch /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-configure

jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-download: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/download-jsoncpp_lib-populate.cmake
jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-download: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-urlinfo.txt
jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-download: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (download, verify and extract) for 'jsoncpp_lib-populate'"
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps && /usr/local/bin/cmake -DCMAKE_MESSAGE_LOG_LEVEL=VERBOSE -P /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/download-jsoncpp_lib-populate.cmake
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps && /usr/local/bin/cmake -DCMAKE_MESSAGE_LOG_LEVEL=VERBOSE -P /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/verify-jsoncpp_lib-populate.cmake
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps && /usr/local/bin/cmake -DCMAKE_MESSAGE_LOG_LEVEL=VERBOSE -P /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/extract-jsoncpp_lib-populate.cmake
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps && /usr/local/bin/cmake -E touch /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-download

jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-install: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-build
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No install step for 'jsoncpp_lib-populate'"
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-build && /usr/local/bin/cmake -E echo_append
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-build && /usr/local/bin/cmake -E touch /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-install

jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'jsoncpp_lib-populate'"
	/usr/local/bin/cmake -Dcfgdir= -P /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/jsoncpp_lib-populate-prefix/tmp/jsoncpp_lib-populate-mkdirs.cmake
	/usr/local/bin/cmake -E touch /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-mkdir

jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-patch: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-patch-info.txt
jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-patch: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-update
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'jsoncpp_lib-populate'"
	/usr/local/bin/cmake -E echo_append
	/usr/local/bin/cmake -E touch /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-patch

jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-test: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-install
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No test step for 'jsoncpp_lib-populate'"
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-build && /usr/local/bin/cmake -E echo_append
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-build && /usr/local/bin/cmake -E touch /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-test

jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-update: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-update-info.txt
jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-update: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-download
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "No update step for 'jsoncpp_lib-populate'"
	/usr/local/bin/cmake -E echo_append
	/usr/local/bin/cmake -E touch /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-update

CMakeFiles/jsoncpp_lib-populate.dir/codegen:
.PHONY : CMakeFiles/jsoncpp_lib-populate.dir/codegen

jsoncpp_lib-populate: CMakeFiles/jsoncpp_lib-populate
jsoncpp_lib-populate: CMakeFiles/jsoncpp_lib-populate-complete
jsoncpp_lib-populate: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-build
jsoncpp_lib-populate: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-configure
jsoncpp_lib-populate: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-download
jsoncpp_lib-populate: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-install
jsoncpp_lib-populate: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-mkdir
jsoncpp_lib-populate: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-patch
jsoncpp_lib-populate: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-test
jsoncpp_lib-populate: jsoncpp_lib-populate-prefix/src/jsoncpp_lib-populate-stamp/jsoncpp_lib-populate-update
jsoncpp_lib-populate: CMakeFiles/jsoncpp_lib-populate.dir/build.make
.PHONY : jsoncpp_lib-populate

# Rule to build all files generated by this target.
CMakeFiles/jsoncpp_lib-populate.dir/build: jsoncpp_lib-populate
.PHONY : CMakeFiles/jsoncpp_lib-populate.dir/build

CMakeFiles/jsoncpp_lib-populate.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/jsoncpp_lib-populate.dir/cmake_clean.cmake
.PHONY : CMakeFiles/jsoncpp_lib-populate.dir/clean

CMakeFiles/jsoncpp_lib-populate.dir/depend:
	cd /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild /home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/jsoncpp_lib-subbuild/CMakeFiles/jsoncpp_lib-populate.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/jsoncpp_lib-populate.dir/depend

