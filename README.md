# g10
[![CMake](https://github.com/Jacob-C-Smith/g10/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/g10/actions/workflows/cmake.yml)

**Dependencies:**\
[![CMake](https://github.com/Jacob-C-Smith/array/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/Jacob-C-Smith/array/actions/workflows/cmake.yml) [![crypto](https://github.com/Jacob-C-Smith/crypto/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/crypto/actions/workflows/cmake.yml) [![CMake](https://github.com/Jacob-C-Smith/dict/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/dict/actions/workflows/cmake.yml) [![json](https://github.com/Jacob-C-Smith/json/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/json/actions/workflows/cmake.yml) [![log](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml) [![queue](https://github.com/Jacob-C-Smith/queue/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/queue/actions/workflows/cmake.yml) [![stack](https://github.com/Jacob-C-Smith/stack/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/stack/actions/workflows/cmake.yml) [![sync](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml) [![tuple](https://github.com/Jacob-C-Smith/tuple/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/tuple/actions/workflows/cmake.yml)

 A cross platform, multithreaded game engine written in C. 
 
 > 1 [Download](#download)
 >
 > 2 [Build](#build)
 >
 > 3 [Example](#example)
 >
 > 4 [Tester](#tester)
 >
 > 5 [Definitions](#definitions)
 >
 >> 5.1 [Type definitions](#type-definitions)
 >>
 >> 5.2 [Function definitions](#function-definitions)
 >>
 >> 5.3 [Macro definitions](#macro-definitinos)

 ## Download
 To download g10, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/g10 --recurse-submodules
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd g10
 $ cmake .
 $ make
 ```
  This will build the example program, and dynamic / shared libraries

  To build g10 for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 **NOTE: If you are running the tester program on Windows, you may have to copy example files to the ```Debug``` or ```Release``` directories.**
 The example program parses the input file, and writes the parsed tokens to stdout. To run the example program, execute this command.
 ```
 ./g10_example 
 ```
 [Source](main.c)
 
 ## Tester
 **NOTE: If you are running the tester program on Windows, you may have to copy the ```serial test cases``` and ```parse test cases``` directories to the ```Debug``` or ```Release``` directories.**

 To run the tester program, execute this command after building
 ```
 $ ./g10_test
 ```
 [Source](g10_test.c)
 
 [TODO: Tester output](.)
 ## Definitions
 
 ### Type definitions
 See [gtypedef](./include/g10/gtypedef.h)

 ### Function definitions
 See [g10 manual](https://github.com/Jacob-C-Smith/g10/wiki)

 ### Macro definitions
 Macros change the features and functionality of G10.
 
 | name                      | description                                                      | 
 |---------------------------|------------------------------------------------------------------| 
 | G10_VERSION_MAJOR         | Different major versions introduce incompatible API changes      | 
 | G10_VERSION_MINOR         | Different minor versions add backward compatible functionality   | 
 | G10_VERSION_PATCH         | Different patch versions introduce backward compatible bug fixes | 
 | BUILD_G10_WITH_ANSI_COLOR | true for color coded logs, false for no color coding             | 
 