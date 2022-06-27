

#ifdef DLL_EXPORTS
#define lib_def _declspec(dllexport)
#else
#define lib_def
#endif

#ifdef __cplusplus
extern "C" {
#endif

lib_def int lib_dev(int a, int b);

#ifdef __cplusplus
}
#endif