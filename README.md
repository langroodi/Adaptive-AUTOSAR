# Adaptive-AUTOSAR
![example workflow](https://github.com/langroodi/Adaptive-AUTOSAR/actions/workflows/cmake.yml/badge.svg)

Adaptive AUTOSAR is a simulated _Adaptive Platform_ enviroment over Linux defined by [AUTOSAR](https://www.autosar.org/standards/adaptive-platform/). The goal of this project is to implement the interfaces defined by the standard for educational purposes. For more information, please refer to [the project Wiki](https://github.com/langroodi/Adaptive-AUTOSAR/wiki).

## Dependecies

It will be tried to use minimum number of dependencies as much as possible. The current dependencies are as follows:

- Cpp Standard: 14
- Cmake mimimum version: 3.14
- Compiler:
    - GCC C/C++ Compiler (x86-64 Linux GNU): 11.2.0; or
    - Clang C/C++ Compiler (x86-64 PC Linux GNU): 14.0.0
- Google Test: v1.12.1
- [Async BSD Socket Lib](https://github.com/langroodi/Async-BSD-Socket-Lib) (in-house C++ libary)
- [OBD-II Emulator](https://github.com/langroodi/OBD-II-Emulator) (in-house C++ emulator)


## Build

### Compiler debug configuration

- GCC:
```bash
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/x86_64-linux-gnu-gcc-11 -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/x86_64-linux-gnu-g++-11 -S . -B build
```
- Clang:
```bash
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang-14 -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++-14 -S . -B build
```

### Compiling
```bash
cmake --build build
```

### Unit tests running
```bash
cd build && ctest
```

## Documentation

Please refer to [the project GitHub pages](https://langroodi.github.io/Adaptive-AUTOSAR/) powered by Doxygen.

## Contribution

Please refer to [the contributing page](https://github.com/langroodi/Adaptive-AUTOSAR/blob/master/CONTRIBUTING.md).
