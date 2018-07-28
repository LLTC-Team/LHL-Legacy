project(LVMTestLib)

add_library(TestLib SHARED testlib.cpp)

target_link_libraries(TestLib LVM)

file(COPY "${CMAKE_CURRENT_BINARY_DIR}/"
        DESTINATION ${LVM_BINARY_DIR}
        FILES_MATCHING REGEX "(.so$)|(.dll$)|(.dylib$)"
        )