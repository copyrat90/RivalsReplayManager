
# Rivals Replay Manager

Replay manager for the indie platform-fighting game, [Rivals of Aether](https://rivalsofaether.com/).

It's currently being developed.\
**(i.e. Nothing is implemented as of now.)**

## Build

You need to install these along with your favorite [C++ compiler that supports **C++20 or later**](https://en.cppreference.com/w/cpp/compiler_support).
1. [**CMake**](https://cmake.org/), the meta build system for C/C++.
2. [**vcpkg**](https://vcpkg.io/), the C/C++ library manager by Microsoft.

When executing `cmake`, you need to locate the `vcpkg.cmake` comes with the vcpkg.
```powershell
# Assuming that vcpkg is installed on C:\vcpkg
cmake ../my/project -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
```
Since this project uses vcpkg's [Manifest Mode](https://vcpkg.io/en/docs/users/manifests.html), **all the dependencies are automatically gets installed** when you specify the [vcpkg's CMake toolchain file](https://vcpkg.io/en/docs/users/integration.html#cmake-toolchain-file-recommended-for-open-source-cmake-projects) on executing CMake.

## Dependencies

This project relies on these libraries:
+ [**Nana**](http://nanapro.org/en-us/) [(GitHub link)](https://github.com/cnjinhao/nana) : A Modern C++ GUI Library
    - License: [BSL-1.0](https://github.com/cnjinhao/nana/blob/master/LICENSE)
+ [**UTF8-CPP**](https://github.com/nemtrif/utfcpp) : UTF-8 with C++ in a Portable Way
    - License: [BSL-1.0](https://github.com/nemtrif/utfcpp/blob/master/LICENSE)
+ [**fmt**](https://github.com/fmtlib/fmt) : A modern formatting library
    - License: [MIT](https://github.com/fmtlib/fmt/blob/master/LICENSE.rst)

## License

*Rivals Replay Manager* is distributed under the [**GNU GPL-3.0 License**](LICENSE).
