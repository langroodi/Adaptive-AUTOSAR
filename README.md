# Adaptive-AUTOSAR
![example workflow](https://github.com/langroodi/Adaptive-AUTOSAR/actions/workflows/cmake.yml/badge.svg)

Adaptive AUTOSAR is a simulated _Adaptive Platform_ enviroment over Linux defined by [AUTOSAR](https://www.autosar.org/standards/adaptive-platform/). The goal of this project is to implement the interfaces defined by the standard for educational purposes.

## Dependecies

It will be tried to use minimum number of dependencies as much as possible. The current dependencies are as follows:

- Cpp Standard: 14
- Cmake mimimum version: 3.14
- Google Test: v1.11.0

## Project Milestones

- [x] CI workflow
- [ ] Doxygen documentation
- [ ] Hardware integration (i.e., CAN-bus and LIN-bus)
- [ ] OBD-II and DoIP
- [ ] Replace STL with ARA Core types (e.g.,`std::future` to `ara::core::Future`)
- [ ] MISRA certified
- [ ] Dedicated memory allocator
- [ ] Integration with Volvo [Extended Vehicle](https://developer.volvocars.com/volvo-api/extended-vehicle/) and [Connected Vehicle](https://developer.volvocars.com/volvo-api/connected-vehicle/) APIs
- [ ] Inter-ECU communication via SOME/IP
