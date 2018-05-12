project(LHL)
cmake_minimum_required(VERSION 3.1)

add_library(CATCH INTERFACE)
target_include_directories(CATCH INTERFACE Catch)

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
endif()

add_subdirectory(LML)
add_subdirectory(LVM)