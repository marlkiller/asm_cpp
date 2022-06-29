@echo off

rmdir /s /q build

set /p flag="Clean CMakeCache.txt ?  y/n :"
if "%flag%" == "y" (
    del /s /q CMakeCache.txt
)

set /p flag="Build x64 ? If type 'N' will build with x86 !  y/n :"
if "%flag%" == "y" (
    cmake -A x64 -B build . 
    cmake --build ./build --config Release
) else (
    cmake -A Win32 -B build . 
    cmake --build ./build --config Release
)
cd build
bin_asm_cpp.exe
cd ../