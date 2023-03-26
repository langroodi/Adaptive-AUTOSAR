# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.26.0)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(libcurl-populate NONE)



include(ExternalProject)
ExternalProject_Add(libcurl-populate
                     "UPDATE_DISCONNECTED" "False" "DOWNLOAD_EXTRACT_TIMESTAMP" "TRUE" "URL" "https://github.com/curl/curl/archive/3027611ca6d4cc5510d2d0fccc3e5a074e09a2fb.zip"
                    SOURCE_DIR          "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/libcurl-src"
                    BINARY_DIR          "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/libcurl-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


