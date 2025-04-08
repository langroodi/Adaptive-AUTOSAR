# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/async-bsd-socket-lib-src")
  file(MAKE_DIRECTORY "/home/runner/work/Adaptive-AUTOSAR/Adaptive-AUTOSAR/build/_deps/async-bsd-socket-lib-src")
endif()
file(MAKE_DIRECTORY
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
