project(sub_project)


AUX_SOURCE_DIRECTORY(src SUB_DIR_SRC)

# 设置环境变量，编译用到的源文件全部都要放到这里，
# 否则编译能够通过，但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"
SET(SUB_TEST_MATH ${SUB_DIR_SRC})
message(">>>>>>>>>>>>>> SUB_TEST_MATH : ${SUB_TEST_MATH}")


# CACHE INTERNAL 共享变量
#set(CMAKE_SHARED_LIBRARY_PREFIX ${SHARED_LIB_PREFIX})
#set(CMAKE_STATIC_LIBRARY_PREFIX ${STATIC_LIB_PREFIX})

# 自动去搜索该目录下的相对于的 dylib/so/dll 文件
#if (WINDOWS)
#    SET(LIBRARY_OUTPUT_PATH ../lib/windows)
#elseif (LINUX)
#    SET(LIBRARY_OUTPUT_PATH ../lib/linux)
#    # link_directories("${PROJECT_ROOT_PATH}/lib/linux")
#elseif (MACOS)
#    SET(LIBRARY_OUTPUT_PATH ../lib/macos)
#endif ()

# Windows 需要配置 dll 与 exe 在同一路径输出
SET(LIBRARY_OUTPUT_PATH ${PROJECT_ROOT_PATH}/build)

add_library(${PROJECT_NAME} SHARED ${SUB_TEST_MATH})
add_library(lib_dev ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}/header
        )


# 生成静态库
#ADD_LIBRARY(${target_name} STATIC ${TEST_MATH}) 
# 生成动态库或共享库
#ADD_LIBRARY(${target_name} SHARED ${TEST_MATH}) 


## add_library 默认生成是静态库，通过以上命令生成文件名字，
#
#在 Linux 下分别是：
#bin_asm_cpp
#shared_lib_sub_project.a
#shared_lib_sub_project.so

#在 Mac 下分别是：
#bin_asm_cpp
#shared_lib_sub_project.a
#shared_lib_sub_project.dylib

#在 Windows 下分别是：
#bin_asm_cpp.exe
#shared_lib_sub_project.lib
#shared_lib_sub_project.dll

# import SHARED
#link_directories(${PROJECT_SOURCE_DIR}/lib) #添加动态连接库的路径
#target_link_libraries(project_name -lmxnet ) #添加libmxnet.so
# import STATIC
#add_library(libdev STATIC IMPORTED)
#set_property(TARGET libdev PROPERTY IMPORTED_LOCATION /path/to/libmxnet.a)
#target_link_libraries(project_name libdev ) #添加libmxnet.a
