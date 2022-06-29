# 指定 cmake 最小版本
cmake_minimum_required(VERSION 3.22)
# project 会引入两个变量：HELLO_BINARY_DIR, HELLO_SOURCE_DIR , 同时也会定义两个等价的变量：PROJECT_BINARY_DIR, PROJECT_SOURCE_DIR
project(asm_cpp)

# #设置 c++ 版本
set(CMAKE_CXX_STANDARD 14)
SET(PROJECT_ROOT_PATH "${PROJECT_SOURCE_DIR}")
set(OUTPUT_DIRECTORY "${PROJECT_ROOT_PATH}/build/")
set(EXECUTABLE_OUTPUT_PATH "${OUTPUT_DIRECTORY}")
#set(CMAKE_BINARY_DIR "${OUTPUT_DIRECTORY}")
#TODO without relese/debug
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
#message(">>>>>>>>>>>>>> CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}")
message(">>>>>>>>>>>>>> CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")
message(">>>>>>>>>>>>>> PROJECT_ROOT_PATH ${PROJECT_ROOT_PATH}")
message(">>>>>>>>>>>>>> OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY}")
message(">>>>>>>>>>>>>> CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")
message(">>>>>>>>>>>>>> CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}")
message(">>>>>>>>>>>>>> CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}")
message(">>>>>>>>>>>>>> CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}")
message(">>>>>>>>>>>>>> CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}")
message(">>>>>>>>>>>>>> CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE}")


# 头文件目录
INCLUDE_DIRECTORIES(include)
# 源文件目录
AUX_SOURCE_DIRECTORY(asm_cpp DIR_SRCS)

# 设置环境变量，编译用到的源文件全部都要放到这里，
# 否则编译能够通过，但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"
SET(TEST_MATH ${DIR_SRCS})


if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CURRENT_OS "Linux")
    set(LINUX 1)
elseif (CMAKE_SYSTEM_NAME STREQUAL "AIX")
    set(CURRENT_OS "UNIX")
    set(UNIX 1)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CURRENT_OS "macOS")
    set(MACOS 1)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CURRENT_OS "windows")
    set(WINDOWS 1)
else ()
    message(STATUS "Configuring on ${CMAKE_SYSTEM_NAME}")
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


message(">>>>>>>>>>>>>> Current OS is ${CURRENT_OS}/${CURRENT_ARCH}/${CURRENT_BIT}")
message(">>>>>>>>>>>>>> TEST_MATH : ${TEST_MATH}")

if (CMAKE_CL_64)
    enable_language(ASM_MASM)
    message(">> enable_language(ASM_MASM)")
    list(APPEND TEST_MATH
            "asm_cpp/win64.asm"
            )
endif ()


message(">>>>>>>>>>>>>> LINK_DIR : ${LINK_DIR}")
message(">>>>>>>>>>>>>> LIBS : ${LIBS}")

add_subdirectory(lib_dev)

set(target_name ${PROJECT_NAME})
set(bin_target_name bin_${target_name})

#TODO 配置 bin 输出路径
#set(EXECUTABLE_OUTPUT_PATH "${OUTPUT_PATH}/bin/")
## 生成可执行文件
ADD_EXECUTABLE(${bin_target_name} ${TEST_MATH})
## 把目标文件与库文件进行链接
#target_link_libraries(${target_name} ${LIBS})
# 在Linux上，使用动态链接的应用程序需要和库libdl.so一起链接，也就是使用选项-ldl
target_link_libraries(${bin_target_name} ${CMAKE_DL_LIBS})
target_link_libraries(${bin_target_name}
        lib_dev
        )

if(WINDOWS)
    
    set(copySource ${PROJECT_ROOT_PATH}/build/shared_lib_sub_project.dll)
    set(copyDest ${PROJECT_ROOT_PATH}/build/lib/windows/Release/shared_lib_sub_project.dll)
    ADD_CUSTOM_COMMAND( TARGET ${bin_target_name}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${copySource} ${copyDest}
            )
endif()