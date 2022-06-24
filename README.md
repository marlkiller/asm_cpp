# cpp_asm

c++ inline asm


## ENV

```shell
# cmake
yum install -y cmake
# gcc/g++
yum install -y gcc-c++
```

## Settings
### Windows
> Visual Studio (Configuring the cmake environment)  

### MacOS
> Xcode (Build Settings > Optimization Level > Release > None[-O0])  


## build

### linux/unix/macos
```shell
rm -rf build/*
cmake . -B build
make -C build
```

### win32/64
```shell
## win
### win64
cmake -A x64 -B build . 
cmake --build ./build --config Release
### win32
cmake -A Win32 -B build . 
cmake --build ./build --config Release

```
### run
```shell
./build/bin_asm_cpp
```

## Supported Platform

| Platform            | 32 | 64 |
|---------------------|----|----|
| MacOS(__x86_64__)   | ❌ | ✅ |
| MacOS(Arm)          | ❌ | ✅ |
| Linux(__x86_64__)   | ❌ | ✅ |
| Windowx(__x86_64__) | ✅ | ✅ |


### See Also:
- https://gcc.gnu.org/
- https://cmake.org/download/
- https://github.com/Kitware/CMake/releases/tag/v3.24.0-rc1