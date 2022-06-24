cmake_minimum_required(VERSION 3.22)
project(asm_cpp)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CURRENT_OS "Linux")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CURRENT_OS "Windows")
elseif(CMAKE_SYSTEM_NAME MATCHES "Apple")
    set(CURRENT_OS "Apple")
endif()



if(CMAKE_CL_64)
    set(CURRENT_BIT "x64")
else(CMAKE_CL_64)
    set(CURRENT_BIT "x86")
endif(CMAKE_CL_64)


if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(CURRENT_ARCH "aarch64")

elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
    set(CURRENT_ARCH "x86_64")
elseif (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "AMD64")
    set(CURRENT_ARCH "AMD64")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    if(CMAKE_CL_64)
        enable_language(ASM_MASM)
        message(">> enable_language(ASM_MASM)")
    endif()
endif()

message(STATUS ">> Current OS is ${CURRENT_OS}/${CURRENT_ARCH}/${CURRENT_BIT}")

include_directories(asm_cpp)

add_executable(bin_asm_cpp
        asm_cpp/win64.asm
        asm_cpp/httplib.h
        asm_cpp/main.cpp)
