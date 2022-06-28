//
// Created by 马治武 on 2022/6/28.
//

#if !defined(__GNUC__) && defined(TEST_EXPORTS)
#define DLLEXPORT __declspec(dllexport)
#elif !defined(__GNUC__) && !defined(TEST_IMPORTS_IGNORE)
#define DLLEXPORT __declspec(dllimport)
#else
#define DLLEXPORT
#endif

extern "C" {
    DLLEXPORT int lib_dev_fun(int u1);
}


