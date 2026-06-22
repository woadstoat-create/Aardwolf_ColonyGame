# ColonySim

A C++20 and SFML prototype for a large-scale, multi-layer colony simulation
inspired by Dwarf Fortress, RimWorld and Songs of Syx.

## Current milestone

Milestone 1 establishes:

- CMake and Ninja builds
- SFML application target
- Platform-independent core library
- Catch2 and CTest
- GitHub Actions continuous integration
- Compiler warnings and analysis configuration

## Requirements

- CMake 3.28 or later
- Ninja
- MSVC with the Desktop development with C++ workload
- Git

## Configure

```powershell
cmake --preset dev