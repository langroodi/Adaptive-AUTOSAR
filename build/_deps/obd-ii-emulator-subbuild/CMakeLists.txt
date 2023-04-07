# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.26.2)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(obd-ii-emulator-populate NONE)



include(ExternalProject)
ExternalProject_Add(obd-ii-emulator-populate
                     "UPDATE_DISCONNECTED" "False" "DOWNLOAD_EXTRACT_TIMESTAMP" "TRUE" "URL" "https://github.com/langroodi/OBD-II-Emulator/archive/b77dadf5e5f73f785bc4a78238a1bbc6becb76dd.zip"
                    SOURCE_DIR          "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/obd-ii-emulator-src"
                    BINARY_DIR          "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/obd-ii-emulator-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


