cmake_minimum_required(VERSION 3.22)
project(asm_cpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(asm_cpp)

add_executable(asm_cpp
        asm_cpp/httplib.h
        asm_cpp/main.cpp)
