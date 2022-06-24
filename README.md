# cpp_asm

c++ inline asm 


## env

> cmake :  
> https://cmake.org/download/  
> https://github.com/Kitware/CMake/releases/tag/v3.24.0-rc1  
> gcc/g++:  
> https://gcc.gnu.org/

## Settings
### Windows
> Visual Studio
### MacOS
> Xcode (Build Settings > Optimization Level > Release > None[-O0])

yum install cmake

## build

### linux/unix/macos
```shell
rm -rf build
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
# run:  build/bin_asm_cpp
```

## Supported Platform

| Platform            | 32 | 64 |
|---------------------|----|----|
| MacOS(__x86_64__)   | ❌ | ✅ |
| MacOS(Arm)          | ❌ | ✅ |
| Linux(__x86_64__)   | ❌ | ✅ |
| Windowx(__x86_64__) | ✅ | ✅ |
