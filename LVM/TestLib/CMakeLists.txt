project(LVMTestLib)

add_library(TestLib SHARED testlib.cpp)

target_link_libraries(TestLib LVM)

find_program(PYTHON3 python3)
add_custom_command(
        TARGET TestLib POST_BUILD
        COMMAND ${PYTHON3} ${LVM_SOURCE_DIR}/copy_libraries.py ${CMAKE_CURRENT_BINARY_DIR} ${LVM_BINARY_DIR}
)