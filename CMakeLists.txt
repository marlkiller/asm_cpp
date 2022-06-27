# 指定 cmake 最小版本
cmake_minimum_required(VERSION 3.22)
# project 会引入两个变量：HELLO_BINARY_DIR, HELLO_SOURCE_DIR , 同时也会定义两个等价的变量：PROJECT_BINARY_DIR, PROJECT_SOURCE_DIR
project(asm_cpp)

# #设置 c++ 版本
set(CMAKE_CXX_STANDARD 14)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CURRENT_OS "Linux")
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CURRENT_OS "Windows")
elseif (CMAKE_SYSTEM_NAME MATCHES "Apple")
    set(CURRENT_OS "Apple")
endif ()


if (CMAKE_CL_64)
    set(CURRENT_BIT "x64")
else (CMAKE_CL_64)
    set(CURRENT_BIT "x86")
endif (CMAKE_CL_64)


if (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(CURRENT_ARCH "aarch64")

elseif (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
    set(CURRENT_ARCH "x86_64")
elseif (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "AMD64")
    set(CURRENT_ARCH "AMD64")
endif ()

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    if (CMAKE_CL_64)
        enable_language(ASM_MASM)
        message(">> enable_language(ASM_MASM)")
    endif ()
endif ()

message(STATUS ">> Current OS is ${CURRENT_OS}/${CURRENT_ARCH}/${CURRENT_BIT}")

# 头文件目录
INCLUDE_DIRECTORIES(include)
# 源文件目录
AUX_SOURCE_DIRECTORY(asm_cpp DIR_SRCS)

# 设置环境变量，编译用到的源文件全部都要放到这里，
# 否则编译能够通过，但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"
SET(TEST_MATH ${DIR_SRCS})
# 添加要编译的可执行文件
ADD_EXECUTABLE(bin_asm_cpp ${TEST_MATH})

#add_executable(demo demo.cpp) # 生成可执行文件
#add_library(common STATIC util.cpp) # 生成静态库
#add_library(common SHARED util.cpp) # 生成动态库或共享库

## add_library 默认生成是静态库，通过以上命令生成文件名字，
#
#在 Linux 下分别是：
#demo
#libcommon.a
#libcommon.so
#
#在 Windows 下分别是：
#demo.exe
#common.lib
#common.dll

#add_executable(bin_asm_cpp
#        asm_cpp/win64.asm
#        asm_cpp/httplib.h
#        asm_cpp/main.cpp)
