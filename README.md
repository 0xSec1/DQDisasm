# DQDisasm
DQDisasm is a lightweight CLI tool designed to disassemble executable code stored as data blob in ELF binaries

## Features
- Extracts exact byte spans mapping from Virtual Addresses (VAddr).
- Disassembles using the Capstone engine.
- Supports length-based or address-range-based extraction.

## Prerequisites
- GCC(latest) or Clang(latest)
- CMake (3.16+)
- Capstone Disassembly Engine

## Build
> [!NOTE]  
> Only support ELF Binaries.  
  
```bash
mkdir build
cd build
cmake ..
make
```
