#include "LVM/LVMSDK.h"

extern "C" {
LVM_LIBRARY_DECLARE

DLL_EXPORT void greet()
{
	std::cout << "hello world" << std::endl;
}

LVM_LIBRARY_IMPLEMENT( "test", "creatorlxd", "0.0.1", "0.0.1", {
	LVM_LIBRARY_FUNCTION( greet )
} )
}