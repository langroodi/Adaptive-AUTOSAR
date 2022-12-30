# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/obd-ii-emulator-src"
  "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/obd-ii-emulator-build"
  "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/obd-ii-emulator-subbuild/obd-ii-emulator-populate-prefix"
  "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/obd-ii-emulator-subbuild/obd-ii-emulator-populate-prefix/tmp"
  "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/obd-ii-emulator-subbuild/obd-ii-emulator-populate-prefix/src/obd-ii-emulator-populate-stamp"
  "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/obd-ii-emulator-subbuild/obd-ii-emulator-populate-prefix/src"
  "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/obd-ii-emulator-subbuild/obd-ii-emulator-populate-prefix/src/obd-ii-emulator-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/obd-ii-emulator-subbuild/obd-ii-emulator-populate-prefix/src/obd-ii-emulator-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/obd-ii-emulator-subbuild/obd-ii-emulator-populate-prefix/src/obd-ii-emulator-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
