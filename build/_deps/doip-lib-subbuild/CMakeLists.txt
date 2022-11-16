# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.24.2)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(doip-lib-populate NONE)



include(ExternalProject)
ExternalProject_Add(doip-lib-populate
                     "UPDATE_DISCONNECTED" "False" "DOWNLOAD_EXTRACT_TIMESTAMP" "TRUE" "URL" "https://github.com/langroodi/Doip-Lib/archive/269510a6c43b3754466ed93486fd7cc2c9deee2e.zip"
                    SOURCE_DIR          "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/doip-lib-src"
                    BINARY_DIR          "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/doip-lib-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


