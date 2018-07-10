#include "LVM/LVMSDK.h"

extern "C" {
LVM_LIBRARY_DECLARE

void greet() {
    std::cout << "hello world" << std::endl;
}

LVM_LIBRARY_IMPLEMENT("test", "test", "test", "test", {
    LVM_LIBRARY_FUNCTION(greet)
}) ;
}