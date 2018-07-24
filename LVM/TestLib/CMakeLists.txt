project(LVMTestLib)

add_library(TestLib SHARED testlib.cpp)

target_link_libraries(TestLib LVM)