## 前记

    原本是想了解在 mac 下开发 C++ 内联汇编 与 Windows 下的区别,
    后来发现我的 Mac (x86) 架构 与 公司的 Mac m1(arm架构) 在汇编这块是有很大的差别的, 索性 arm把 也简单了解了下.
    后来, 发现 用 cmake 可以做到跨平台编译, 索性又把 windows的写法也加进来了...


## ENV

```shell
# cmake (VERSION >=3.22)
# export PATH=$PATH:/home/cloudshell-user/cmake-3.24.0-rc1-linux-x86_64/bin
yum install -y cmake
# gcc/g++
yum install -y gcc-c++
```

## Settings
### Windows
    cmake -G "Visual Studio 16 2019"
    项目调试属性 > 配置属性 > 常规 > 输出目录 > {project_name}\build 
    并且 exe 与 dll 需要打包到同一路径下,否则启动会报错
    Windows 下依赖 Visual Studio, 用 VS 启动项目时,要自己配置下 cmake, 因为vs集成的cmake版本有点低
    Visual Studio (Configuring the cmake environment)   

### MacOS
    
    sudo xcode-select -s /Applications/Xcode.app/Contents/Developer
    cmake -G Xcode
    Mac 用 Xcode 开发的话, Release 模式要设置下Optimization Level ,否则手写 asm, 编译器开启自动优化会有意想不到的异常发生. (大神忽略)
    Debug 模式下 是默认关闭 自动优化的, 所以Debug模式可以忽略
    Xcode (Build Settings > Optimization Level > Release > None[-O0])
    Xcode 需要手动配置头文件路径
    Xcode (Build Settings > Search Paths > Header Search Paths : include)
    建议Mac 用 Clion 开发...

## Code

### C++ 定义变量/函数
- 首先用 C++ 代码,定义了一个方法 fun_dev
- 然后 定义了四个变量 , a, b, result_ra , result_rc  

```cpp
void fun_dev() {
    printf("this is fun_dev\r\n");
}
```

```cpp
    uintptr_t a = 10;
    uintptr_t b = 20;
    uintptr_t result_ra = 0;
    uintptr_t result_rc = 0;
```


### inlie_asm 实现
- 实现1: 计算 a+b, 并且 将结果给 result_ra , 同时讲 0x20 赋值给 result_rc
- 实现2: 汇编代码 执行 上面定义的 函数 fun_dev  


#### Arm架构

```
#if defined  __aarch64__

//    32 位为 寄存器为 r1-rn, 64为  x1-xn
//    armv8 , 要给寄存器赋值一个64位的数 居然是这种写法吗?
//    __asm__ __volatile__ (
//      "mov x1, #0x9012\n" // long = 0x123456789012 (16) / 20015998341138 (10);
//      "movk x1, #0x5678, lsl #16\n"
//      "movk x1, #0x1234, lsl #32\n"
//      "mov %[res],x1\n"
//      :[res]"=r"(tmp)
//      :
//      :"cc","memory"
// );

    std::cout << "this is arm cpu" << std::endl;
               
    __asm__ __volatile__ (
          "add %[result_ra], %[value_a], %[value_b]\n\t"
          "mov %[result_rc], #0x20\n\t"
          :[result_ra]"=r"(result_ra),[result_rc]"=r"(result_rc)
          :[value_a]"r"(a), [value_b]"r"(b)
           :"cc","memory"
                );
   
    __asm__ __volatile__ (
          "blr %[fun_dev_addr]\n\t"
          :
          :[fun_dev_addr]"r"(fun_dev)
          :"cc","memory"
                 );
#endif
```

#### __x86_64__架构

```
#if defined  __x86_64__

//    这个起码寄存器我都认识...
//    __asm__ __volatile__ (
//         "mov $0x1234,%%rax\n\t"
//    );

    std::cout << "this is x86 cpu" << std::endl;
               
    __asm__ __volatile__(
          "add %[value_a],%[value_b]\n\t"
          "movq %[value_b], %[result_ra]\n\t"
          "movq $0x20, %[result_rc]\n\t"
          :[result_ra]"=a"(result_ra),[result_rc]"=c"(result_rc) // result_ra 赋值给 rax,result_rc 赋值给 rcx
          :[value_a] "a"(a),[value_b] "b"(b) // value_a 赋值给 rax , value_b 赋值给 rbx
          :"cc","memory"
                 );
   
    __asm__ __volatile__ (
          "callq *%[fun_dev_addr]\n\t" // 在内联汇编语句中使用寄存器 eax 时，寄存器名前应该加两个 ‘%’，即 %%eax
          :
          :[fun_dev_addr]"a"(fun_dev) // mov fun_dev_addr,rax; call rax; // :"%eax"
           :"cc","memory"
                );
#endif
```


#### Win x64/x32

> Win64 需要单独添加**一个 asm文件**,并导入函数   


```cpp
#if defined _WIN32
    #if defined(_M_X64)
    EXTERN_C ULONG64 asm_win64_add(ULONG64 u1, ULONG64 u2,DWORD_PTR u3);
    EXTERN_C ULONG64 asm_win64_call(DWORD_PTR u1);   
    #endif
#endif
```


> win64.asm  

```asm
.data
.CODE  ;

vt_Add PROC  ;
    add rcx,rdx                ;
    mov rax,rcx                ;
    ret                                ;
vt_Add ENDP  ;

asm_win64_add PROC  ;
    push rbp
    sub rsp,30h ; params is rcx,rdx,r8,r9,rsp+20,rsp+28..
    mov         qword ptr [rsp+18h],r8
    mov         qword ptr [rsp+10h],rdx
    mov         qword ptr [rsp+8],rcx
    add rcx,rdx
    mov rax,rcx
    mov qword ptr[r8],20h
    add rsp,30h
    pop rbp
    ret

asm_win64_add ENDP  ;

asm_win64_call PROC  ;
   
    push rbp
    sub rsp,30h ;
    mov         qword ptr [rsp+8],rcx
    call qword ptr [rsp+8h]
    add rsp,30h
    pop rbp
    ret

asm_win64_call ENDP  ;
END     ;

```

```cpp
#if defined _WIN32

    #if defined(_M_X64)        
    std::cout << "this is _WIN64 " << std::endl;
   
    result_ra = asm_win64_add(a,b, (DWORD_PTR)&result_rc);
    asm_win64_call((DWORD_PTR)&fun_dev);


    #else
        std::cout << "this is _WIN32 " << std::endl;
        __asm {
            mov eax, a;
            mov ebx, b;
            add eax, ebx;
            mov result_ra, eax;
            mov result_rc, 0x20;
        }

        __asm {
            call fun_dev;
        }
    #endif
#endif

```


### 执行输出

```cpp
printf("asm result (%lu + %lu ) => %lu ?  , (0x20) => %lu ?  \r\n",
           a,b,
           result_ra,
           result_rc
           );
```
```
# fun_dev addr ->0x1009cc120  // 函数地址
# this is x86 cpu // 这一行 会随着操作系统而变动,后面的执行结果都一样
# this is fun_dev // asm 执行 call 打印记录
# asm result (10 + 20 ) => 30 ?  , (0x20) => 32 ?  
```


## Build&Run

### Linux/Mac/Unix

```shell
rm -rf build/*
cmake . -B build
make -C build
./build/bin_asm_cpp
```
### Windows

```bat
rmdir /s /q build
### win64
cmake -A x64 -B build .
cmake --build ./build --config Release
### win32
cmake -A Win32 -B build .
cmake --build ./build --config Release

### 然后自己去 build/Release 目录下找 exe 执行程序吧
```

### MakeFile

> win64 依赖一个 .asm 的文件, 所以这里只需要判断 win64 下 开启 **ASM_MASM** 既可 , 别的地方都一样

```cpp
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
        message("enable_language(ASM_MASM)")
    endif()
endif()

message(STATUS "Current OS is ${CURRENT_OS}/${CURRENT_ARCH}/${CURRENT_BIT}")
include_directories(asm_cpp)
add_executable(bin_asm_cpp
        asm_cpp/win64.asm
        asm_cpp/httplib.h
        asm_cpp/main.cpp)

```


## Supported Platform

| Platform            | 32 | 64 |
|---------------------|----|----|
| MacOS(__x86_64__)   | ❌ | ✅ |
| MacOS(Arm)          | ❌ | ✅ |
| Linux(__x86_64__)   | ❌ | ✅ |
| Linux(Arm)          | ❌ | ✅ |
| Windowx(__x86_64__) | ✅ | ✅ |


## 完整源码
Gayhub : https://github.com/marlkiller/asm_cpp.git  

## 后记

汇编菜鸟一枚, 请提宝贵意见, 勿喷~


See Also:
- https://gcc.gnu.org/
- http://ftp.gnu.org/gnu/gcc/gcc-11.2.0/
- https://cmake.org/download/
- https://github.com/Kitware/CMake/releases/tag/v3.24.0-rc1
