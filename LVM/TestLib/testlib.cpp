#include "LVM/NativeLibraryManager.h"

extern "C" {

TEST_DLL_EXPORT void greet()
{
	std::cout << "hello world" << std::endl;
}

}