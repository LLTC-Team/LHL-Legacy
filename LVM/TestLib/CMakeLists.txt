project(LVMTestLib)

add_library(TestLib SHARED testlib.cpp)

target_link_libraries(TestLib LVM)

find_program(PYTHON python3)
if (PYTHON)
    find_program(PYTHON python)
endif ()

add_custom_command(
        TARGET TestLib POST_BUILD
        COMMAND ${PYTHON} ${LVM_SOURCE_DIR}/copy_libraries.py ${CMAKE_CURRENT_BINARY_DIR} ${LVM_BINARY_DIR}
)