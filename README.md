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
- [pugixml 1.13](https://pugixml.org) (3rd party C++ libary)
- [libcurl 7.88.0](https://github.com/curl/curl) (3rd party C libary)
- [NLohmann JSON 3.11.2](https://github.com/nlohmann/json) (3rd party modern C++ library)
- [Async BSD Socket Lib](https://github.com/langroodi/Async-BSD-Socket-Lib) (in-house C++ libary)
- [OBD-II Emulator](https://github.com/langroodi/OBD-II-Emulator) (in-house C++ emulator)
- [DoIP Lib](https://github.com/langroodi/DoIP-Lib) (in-house C++ libary)

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

## Run
To run the Adaptive AUTOSAR simulation, launch the following executable by passing the execution manifest file path as the argument:
```bash
./build/adaptive_autosar ./configuration/execution_manifest.arxml ./configuration/extended_vehicle_manifest.arxml ./configuration/diagnostic_manager_manifest.arxml
```
Then the executable will ask for:

1. First, the VCC API key;
2. And then the test access OAuth 2.0 token;

in order to connect to the [Volvo Extended Vehicle](https://developer.volvocars.com/apis/extended-vehicle/overview/) RESTful API. To create the API key and the access token, you can follow [this tutorial](https://developer.volvocars.com/apis/docs/getting-started/).

> ⚠️ Due to security reasons, the terminal echo is temporarily disabled while typing the key and the token.

## Documentation

Please refer to [the project GitHub pages](https://langroodi.github.io/Adaptive-AUTOSAR/) powered by Doxygen.

## Contribution

Please refer to [the contributing page](https://github.com/langroodi/Adaptive-AUTOSAR/blob/master/CONTRIBUTING.md).
