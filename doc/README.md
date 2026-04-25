# tc-base

This document collects the build, install, and package-consumer notes for `tc-base`.

## Build

Configure and build with one of the presets in `CMakePresets.json`:

```sh
cmake --preset gcc
cmake --build bld_gcc
```

Available presets:

| Preset   | Platform | Compiler | Type    |
|----------|----------|----------|---------|
| `gcc`    | Linux    | GCC      | Debug   |
| `gccrel` | Linux    | GCC      | Release |
| `cl`     | Linux    | Clang    | Debug   |
| `clrel`  | Linux    | Clang    | Release |
| `vc`     | Windows  | MSVC     | Debug   |
| `mw`     | Windows  | MinGW    | Debug   |
| `mwrel`  | Windows  | MinGW    | Release |

Each preset places build artifacts in `bld_<preset>/` and install output in `inst_<preset>/`.

To configure only the library targets and skip tests:

```sh
cmake -GNinja -Bbld_gcc -S. -DBUILD_TESTING=OFF
# or using a preset:
cmake --preset gcc -DBUILD_TESTING=OFF
```

## Test

Run all tests after building:

```sh
ctest --test-dir bld_gcc --output-on-failure
```

To run a specific test suite:

```sh
ctest --test-dir bld_gcc -R Julian --output-on-failure
```

## Install

Install to the preset-defined prefix:

```sh
cmake --preset gcc
cmake --build bld_gcc
cmake --install bld_gcc
```

The `gcc` preset sets `CMAKE_INSTALL_PREFIX` to `./inst_gcc` by default.

To install somewhere else:

```sh
cmake --preset gcc -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build bld_gcc
cmake --install bld_gcc
```

Installed files include:

- `<prefix>/lib/libbase.a`
- `<prefix>/include/*.hh`
- `<prefix>/lib/cmake/tc-base/*.cmake`

## Consumer Usage

In another CMake project:

```cmake
cmake_minimum_required(VERSION 3.15)
project(my_app LANGUAGES CXX)

find_package(tc-base REQUIRED)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE tc-base::base)
```

Example source:

```cpp
#include <Hash.hh>
#include <iostream>

int main() {
    std::string hash = TC::crc32Hex("hello world");
    std::cout << "CRC32: " << hash << '\n';
    return 0;
}
```

When configuring a consuming project, point `CMAKE_PREFIX_PATH` at the tc-base install prefix:

```sh
cmake -DCMAKE_PREFIX_PATH=~/tc-base/inst_gcc /path/to/my/project
```

