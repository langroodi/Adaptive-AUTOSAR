# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/async-bsd-socket-lib-src"
  "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/async-bsd-socket-lib-build"
  "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/async-bsd-socket-lib-subbuild/async-bsd-socket-lib-populate-prefix"
  "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/async-bsd-socket-lib-subbuild/async-bsd-socket-lib-populate-prefix/tmp"
  "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/async-bsd-socket-lib-subbuild/async-bsd-socket-lib-populate-prefix/src/async-bsd-socket-lib-populate-stamp"
  "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/async-bsd-socket-lib-subbuild/async-bsd-socket-lib-populate-prefix/src"
  "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/async-bsd-socket-lib-subbuild/async-bsd-socket-lib-populate-prefix/src/async-bsd-socket-lib-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/async-bsd-socket-lib-subbuild/async-bsd-socket-lib-populate-prefix/src/async-bsd-socket-lib-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/async-bsd-socket-lib-subbuild/async-bsd-socket-lib-populate-prefix/src/async-bsd-socket-lib-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
