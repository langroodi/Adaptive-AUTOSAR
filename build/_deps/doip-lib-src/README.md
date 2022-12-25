# DoIP-Lib
![example workflow](https://github.com/langroodi/DoIP-Lib/actions/workflows/cmake.yml/badge.svg)

This repository corresponds to the Diagnostics over Internet Protocol (DoIP) C++ library for serializing and deserializing DoIP payloads based on [ISO 13400-2 standard](https://www.iso.org/obp/ui/#iso:std:iso:13400:-2:ed-2:v1:en).

## Dependecies
The current library dependencies are as follows:
- Cpp Standard: 14
- Cmake mimimum version: 3.14
- Compiler:
    - GCC C/C++ Compiler (x86-64 Linux GNU): 11.2.0; or
    - Clang C/C++ Compiler (x86-64 PC Linux GNU): 14.0.0
- Google Test: v1.12.1

## Documentation
The library documentation is available from [GitHub Pages](https://langroodi.github.io/DoIP-Lib/) which is powered by [Doxygen](https://www.doxygen.nl/index.html).

## Build and Test
To be able to build the library with the unit tests (enabled by default), following bash command can be used:
```bash
# To configure the build (Use '-Dbuild_tests=OFF' switch at the end to disable building the unit tests):
cmake -S . -B build

# To build the library:
cmake --build build

# To run the tests:
cd build && ctest
```

### Samples
The library sample build is disabled by default. To be able to build the library with the sample, the first bash command introduced above should be modified as follows:
```bash
cmake -S . -B build -Dbuild_samples=ON
```