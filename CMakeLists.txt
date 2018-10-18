project(LHL)
cmake_minimum_required(VERSION 3.12)

if (BUILD_DOC MATCHES "TRUE")
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_OUTPUT_DIRECTORY doxygen)
    find_package(Doxygen
            REQUIRED dot
            OPTIONAL_COMPONENTS mscgen dia)
    message("doxygen version: ${DOXYGEN_VERSION}")
endif ()

add_library(CATCH INTERFACE)
target_include_directories(CATCH INTERFACE Catch)

if (CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
endif ()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(LML)
add_subdirectory(LVM)