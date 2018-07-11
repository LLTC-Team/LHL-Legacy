#include "LVM/LVMSDK.h"

#ifdef _WIN32
#define TEST_DLL_EXPORT __declspec(dllexport)
#else
#define TEST_DLL_EXPORT
#endif

extern "C" {
TEST_DLL_EXPORT LVM_LIBRARY_DECLARE

TEST_DLL_EXPORT void greet() {
    std::cout << "hello world" << std::endl;
}

LVM_LIBRARY_IMPLEMENT("test", "test", "test", "test", {
    LVM_LIBRARY_FUNCTION(greet)
}) ;
}