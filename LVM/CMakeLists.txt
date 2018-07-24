project(LVM)
cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)

# force Unicode over Multi-byte
if(COMPILER_MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

aux_source_directory(Source MAIN_SRC_LIST)
add_library(LVM ${MAIN_SRC_LIST})
target_include_directories(LVM PUBLIC Include)
target_link_libraries(LVM Threads::Threads ${CMAKE_DL_LIBS})

add_subdirectory(TestLib)

add_executable(LVM_TEST Tests/Tests.cpp)
target_link_libraries(LVM_TEST CATCH LVM)

add_executable(LVM_EXE main.cpp)
target_link_libraries(LVM_EXE LVM)