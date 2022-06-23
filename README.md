# cpp_asm

c++ inline asm with xcode (intel & apple)


## env

> cmake :  
> https://cmake.org/download/  
> https://github.com/Kitware/CMake/releases/tag/v3.24.0-rc1

yum install cmake

## build

```shell

rm -rf build

# linux/unix
cmake . -B build
make -C build

# win
## win64
cmake -A x64 -B build . 
cmake --build ./build --config Release

## win32
cmake -A Win32 -B build . 
cmake --build ./build --config Release
```
