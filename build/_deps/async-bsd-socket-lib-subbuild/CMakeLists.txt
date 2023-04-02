# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.26.1)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(async-bsd-socket-lib-populate NONE)



include(ExternalProject)
ExternalProject_Add(async-bsd-socket-lib-populate
                     "UPDATE_DISCONNECTED" "False" "DOWNLOAD_EXTRACT_TIMESTAMP" "TRUE" "URL" "https://github.com/langroodi/Async-BSD-Socket-Lib/archive/8c84da8f4b5e7552b41a54c92acb0b4565d07024.zip"
                    SOURCE_DIR          "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/async-bsd-socket-lib-src"
                    BINARY_DIR          "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/async-bsd-socket-lib-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


