# YAQ Project

A C++ project template using Conan package manager and Google Test framework.

## Prerequisites

- CMake (>= 3.15)
- Conan (>= 2.0)
- C++ compiler with C++17 support

## Building the Project

1. Create a build directory and navigate to it:
```bash
mkdir build && cd build
```

2. Install dependencies using Conan:
```bash
conan install .. --output-folder=. --build=missing
```

3. Configure and build the project:
```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Running Tests

Or run the test executable directly:
```bash
./YAQ_tests
```
