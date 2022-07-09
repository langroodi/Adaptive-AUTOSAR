# OBD-II-Emulator
![example workflow](https://github.com/langroodi/OBD-II-Emulator/actions/workflows/cmake.yml/badge.svg)

This is an OBD-II emulator via CAN over Linux that handles diagnostics queries of OBD2 scanners. The emulator listens to the CAN packets coming from the USB-CAN Analyzer device over a *serial communication* and by using the device *CAN driver*, converts the packets to *CAN frames*. Then the *OBD emulator* extracts the parameter ID (*PID*) from the frames and routes them to the corresponding *OBD service* as shown below:

```
                                 +-------------------------+                                
                                 | Serial Communication :  |                                
                                 | Communication Layer     |                                
                                 +------------|------------+                                
                                              |                                             
                                         Byte Array                                         
                                              |                                             
                                 +-------------------------+                                
                                 |       CAN Driver        |                                
                                 |                         |                                
                                 +------------|------------+                                
                                              |                                             
                                          CAN Frame                                         
                                              |                                             
                                 +------------|------------+                                
                                 |      OBD Emulator       |                                
                                 |                         |                                
                                 /------------|------------\                                
                               /-             |             -\                              
                            PID              PID             PID                            
                           /-                 |                -\                           
+--------------------------      +------------|------------+     --------------------------+
|     OBD Service(0)      |      |     OBD Service(i)      |     |     OBD Service(n)      |
|                         |      |                         |     |                         |
+-------------------------+      +-------------------------+     +-------------------------+
```

## Dependecies
The current library has two categories of dependency as follows:

### Hardware dependencies
- [USB-CAN Analyzer](https://github.com/SeeedDocument/USB-CAN-Analyzer)
- [CH341SER Linux Driver v1.3](https://github.com/SeeedDocument/USB-CAN-Analyzer/tree/master/res/Driver)

### Software dependencies
- Cpp Standard: 14
- Cmake mimimum version: 3.14
- Compiler:
    - GCC C/C++ Compiler (x86-64 Linux GNU): 9.3.0; or
    - Clang C/C++ Compiler (x86-64 PC Linux GNU): 10.0.0
- Google Test: v1.11.0
- Linux kernel minimum version: 2.6.25

## Documentation
The library documentation is available from [GitHub Pages](https://langroodi.github.io/OBD-II-Emulator/) which is powered by [Doxygen](https://www.doxygen.nl/index.html) .

## Samples
The library samples build is disabled by default. To be able to build the library with the samples, following bash command can be used:
```bash
cmake -S . -B build -Dbuild_samples=ON
cmake --build build
# To run the tests:
cd build && ctest
```