# 指定 cmake 最小版本
cmake_minimum_required(VERSION 3.22)
# project 会引入两个变量：HELLO_BINARY_DIR, HELLO_SOURCE_DIR , 同时也会定义两个等价的变量：PROJECT_BINARY_DIR, PROJECT_SOURCE_DIR
project(asm_cpp)

# #设置 c++ 版本
set(CMAKE_CXX_STANDARD 14)

# 头文件目录
INCLUDE_DIRECTORIES(include)
# 源文件目录
AUX_SOURCE_DIRECTORY(asm_cpp DIR_SRCS)

# 设置环境变量，编译用到的源文件全部都要放到这里，
# 否则编译能够通过，但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"
SET(TEST_MATH ${DIR_SRCS})


if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(Linux 1)
    set(CURRENT_OS "Linux")
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(Windows 1)
    set(CURRENT_OS "Windows")
elseif (CMAKE_SYSTEM_NAME MATCHES "Apple")
    set(Apple 1)
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
        list(APPEND TEST_MATH
            "asm_cpp/win64.asm"
            )
    endif ()
endif ()

message(STATUS ">> Current OS is ${CURRENT_OS}/${CURRENT_ARCH}/${CURRENT_BIT}")
message("${TEST_MATH}")

if(WINDOWS)
    link_directories("${PROJECT_SOURCE_DIR}/lib/win")
    ADD_DEFINITIONS(-DDLL_EXPORTS)
elseif(LINUX)
    link_directories("${PROJECT_SOURCE_DIR}/lib/linux")
elseif(MACOS)
    set(LINK_DIR lib/macos)
    file(GLOB LIBS "${LINK_DIR}/libopencv_*.so")

endif()

# 生成可执行文件
#ADD_EXECUTABLE(bin_asm_cpp ${TEST_MATH})
# 把目标文件与库文件进行链接
#target_link_libraries(${PROJECT_NAME} ${LIBS})

# 生成静态库
ADD_LIBRARY(_asm_cpp STATIC ${TEST_MATH}) 
# 生成动态库或共享库
#ADD_LIBRARY(_asm_cpp SHARED ${TEST_MATH}) 


## add_library 默认生成是静态库，通过以上命令生成文件名字，
#
#在 Linux 下分别是：
#demo
#libcommon.a
#libcommon.so

#在 Mac 下分别是：
#demo
#libcommon.a
#libcommon.dylib

#在 Windows 下分别是：
#demo.exe
#common.lib
#common.dll
