#if !defined(__GNUC__) && defined(TEST_EXPORTS)
#define DLLEXPORT __declspec(dllexport)
#elif !defined(__GNUC__) && !defined(TEST_IMPORTS_IGNORE)
#define DLLEXPORT __declspec(dllimport)
#else
#define DLLEXPORT 
#endif

extern "C" {
	DLLEXPORT int lib_dev(int u1);
}


