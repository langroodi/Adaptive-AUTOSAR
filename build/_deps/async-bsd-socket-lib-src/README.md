# Asynchronous BSD Socket Library
![example workflow](https://github.com/langroodi/Async-BSD-Socket-Lib/actions/workflows/cmake.yml/badge.svg)

This is a non-blocking BSD (POSIX) network socket library over TCP/IP and Inter-process Communication (IPC) for Liunx. The library is a wrapper around the POSIX socket programming and IPC system calls which hires Linux kernel edge-triggered polling mechanism (epoll) to provide an async network/inter-process communication. The library supports following communications:
- TCP
- UDP Unicast
- UDP Multicast
- FIFO (Named Pipe)

## Dependecies
The current library dependencies are as follows:
- Cpp Standard: 14
- Cmake mimimum version: 3.14
- Compiler:
    - GCC C/C++ Compiler (x86-64 Linux GNU): 11.2.0; or
    - Clang C/C++ Compiler (x86-64 PC Linux GNU): 14.0.0
- Google Test: v1.12.1
- Linux kernel minimum version: 2.6

## Documentation
The library documentation is available from [GitHub Pages](https://langroodi.github.io/Async-BSD-Socket-Lib/) which is powered by [Doxygen](https://www.doxygen.nl/index.html) .

## Samples
The library samples build is disabled by default. To be able to build the library with the samples, following bash command can be used:
```bash
cmake -S . -B build -Dbuild_samples=ON
cmake --build build
# To run the tests:
cd build && ctest
``` 
