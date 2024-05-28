@echo off
rmdir /s /q build

rem MinGW 似乎不支持 32 位
rem cmake -B build -G "MinGW Makefiles"
rem cmake --build build

set /p flag="Build x64 ? If type 'n' will build with x86 !  y/n :"
if "%flag%" == "y" (
    rem cmake -A x64 -B build . 
    cmake -G "Visual Studio 17 2022" -A x64 -B build .
    cmake --build ./build --config Release
) else (
    cmake -G "Visual Studio 17 2022" -A Win32 -B build .
    cmake --build ./build --config Release
)
