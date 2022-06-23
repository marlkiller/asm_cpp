//
//  main.cpp
//  mac_c
//
//  Created by artemis on 2022/5/9.
//

#include <iostream>
#include "httplib.h"

using namespace std;

#if defined _WIN32
    #if defined(_M_X64) 
    EXTERN_C ULONG64 asm_win64_add(ULONG64 u1, ULONG64 u2,DWORD_PTR u3);
    EXTERN_C ULONG64 asm_win64_call(DWORD_PTR u1);   
    #endif
#endif
    
void fun_dev() {
    printf("this is fun_dev\r\n");
}

int arm_asm_add(int a, int b) {
    //    asm(
    //      asm-qualifiers
    
    //     这里取值有三种 volatile , inline , goto:
    //     volatile的意思是易变的、不稳定的，用来告诉编译器不要随便优化这段代码，否则可能出问题。
    //     比如汇编指令“mov r0, r0”，它把r0的值复制到r0，并没有实际做什么事情，你的本意可能是用这条指令来延时。编译器看到这指令后，可能就把它去掉了。
    //     加上volatile的话，编译器就不会擅自优化。
    
    //    code
    // : output operand list     /*输出操作数列表*/
    //    :"constraint"(variable)"constraint"用于定义variable的存放位置：
    //    r表示使用任何可用的寄存器
    //    m表示使用变量的内存地址+可读可写=只写&表示该输出操作数不能使用输入部分使用过的寄存器，只能用"+&"或"=&"的方式使用
    
    // : input operand list      /*输入操作数列表*/
    //    :"constraint"(variable/immediate)"constraint"用于定义variable的存放位置：
    //    r(register operand，寄存器操作数，使用寄存器来保存这些操作数)
    //    m(memory operand，表示要传入有效的地址，只要CPU能支持该地址，就可以传入)
    //    i(immediate integer operand，表示可以传入一个立即数)
    
    // : clobber list               /*被改变资源列表*/
    //      Clobbers: 一般是"cc", "memory"开头，然后接着填内联汇编中用到的通用寄存器和向量寄存器
    //      "cc"表示内联汇编代码修改了标志寄存器；
    //      "memory"表示汇编代码对输入和输出操作数执行内存读取或写入操作,表示汇编代码中,除了“InputOperands”和“OutputOperands”中指定的之外,还会读、写更多的内存
    //    );
    
    //    int arm_asm_add(int m, int n)
    //         m  -- r0
    //         n  -- r1
    //         返回值给 r0
    //    asm volatile
    //    (
    //        "add %[result_1],%[value_1],%[value_2]\n\t"
    //        "sub %[result_2],%[value_1],%[value_2]\n\t"
    //        :[result_1]"=r"(x),[result_2]"=r"(y)
    //        :[value_1]"r"(x),[value_2]"r"(y)
    //        :"memory"
    //    );
    //
    return 0;
}


void http_test() {
    std::cout << "Hello, World!\n";
    httplib::Client cli("http://www.baidu.com");
    auto res = cli.Get("/");
    printf("status {%d}\r\nbody {%s}\r\n", res->status, res->body.c_str());
}

// http://ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html
// https://stackoverflow.com/questions/5784824/bus-error-inline-x86-assembly-with-gcc-asm-block-on-mac-os-x-converting-from
// https://www.cs.uaf.edu/courses/cs301/2014-fall/notes/inline-assembly/
// re read xx ; disas
void asm_test() {
    
    printf("fun_dev addr ->%p \r\n", &fun_dev);
    printf("printf addr ->%p \r\n", &printf);
    
    uintptr_t a = 10;
    uintptr_t b = 20;
    uintptr_t result_ra = 0;
    uintptr_t result_rc = 0;
    
#if defined  __aarch64__
    // ARMv8
    // https://cloud.tencent.com/developer/article/1635842
    // 32 位为 mov r1-rn, 64为 mov x1-xn
    // blr: (branch)跳转到某地址(无返回)
    // bl: 跳转到某地址(有返回)
    // ret: 子程序(函数调用)返回指令，返回地址已默认保存在寄存器 lr (x30) 中
    // 31个通用寄存器(R0~R30)，每个寄存器可以存取一个64位大小的数，
    // 当使用 X0-X30时，是一个64位的数，
    // 当使用 W0-W30访问时，是一个32位的数，为寄存器的低32位。
    // 使用`%w[name]` 操作W寄存器(正如这里的情况).
    // 针对X寄存器可以使用 `%x[name]`，这是默认的情况
    std::cout << "this is arm cpu" << std::endl;
    
    int64_t result;
    // asm("movz %w[res], #0x100" : [res] "=r"(result));
    // asm("movz %w[res], %[value]" : [res] "=r"(result) : [value] "i"(10));
    
    long tmp;
    
//    __asm__ __volatile__ (
//                          "mov w0, #0x11111111\n"
//                          "mov x1, #0x9012\n" // long = 0x123456789012 (16) / 20015998341138 (10);
//                          "movk x1, #0x5678, lsl #16\n"
//                          "movk x1, #0x1234, lsl #32\n"
//                          "mov %[res],x1\n"
//                          :[res]"=r"(tmp)
//                          :
//                          :"cc","memory"
//                          );
//
//    __asm__ __volatile__ (
//                          "ADD x0, %[input_i], %[input_j]"
//                          :
//                          : [input_i] "r"(a), [input_j] "r"(b)
//                          :"cc","memory"
//                          );
//
    __asm__ __volatile__ (
                          "add %[result_ra], %[value_a], %[value_b]\n"
                          "mov %[result_rc], #0x20\n"
                          :[result_ra]"=r"(result_ra),[result_rc]"=r"(result_rc)
                          :[value_a]"r"(a), [value_b]"r"(b)
                          :"cc","memory"
                          );
    
    __asm__ __volatile__ ("blr %[fun_dev_addr]\n\t"
                          :
                          :[fun_dev_addr]"r"(fun_dev)
                          :"cc","memory"
                         );
    
#elif defined __x86_64__
    // https://blog.csdn.net/u014555106/article/details/124577187
    // gcc AT&T语法中操作数的方向与intel相反。在 Intel 语法中，第一个操作数是目标，第二个操作数是源
    // 在 Intel 语法中，基址寄存器包含在 '[' 和 ']' 中，而在 AT&T 中它们更改为 '(' 和 ')'
    // https://github.com/akhzarna/AssemblyLanguage
    // uint64_t 位操作 rax,rbx.. 指令为 movq,callq
    // uint32_t 位操作 eax,ebx.. 指令为 movl,calll
    
    std::cout << "this is x86 cpu" << std::endl;
    
    
    __asm__ __volatile__ (
                          "mov $0x1111111,%%rax\n"
                          "mov $0x1111111,%%rax\n"
                          "mov $0x1111111,%%rax\n"
                          "mov $0x1111111,%%rax\n"
                          :::"cc","memory"
                          );
    
    __asm__ __volatile__(
                         "add %[value_a],%[value_b]\n"
                         "movq %[value_b], %[result_ra]\n"
                         "movq $0x20, %[result_rc]\n\n"
                         
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
    
    //    uint32_t rax,rbx,rcx, rdx; // uint64_t
    //      asm __volatile__ (
    //        "rdtsc" "\n\t"
    //        : "=a" (rax), "=b" (rbx),"=c" (rcx),"=d" (rdx)
    //        :
    //      );
#elif defined _WIN32
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
    printf("asm result (%lu + %lu ) => %lu ?  , (0x20) => %lu ?  \r\n",
           a,b,
           result_ra,
           result_rc
           );
    
    
}


int main(int argc, const char *argv[]) {
    
    asm_test();
    return 0;
}
