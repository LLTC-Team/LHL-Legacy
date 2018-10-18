project(LML)
cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)

# force Unicode over Multi-byte
if (COMPILER_MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
endif ()

aux_source_directory(Source MAIN_SRC_LIST)
add_library(LML STATIC ${MAIN_SRC_LIST})
target_include_directories(LML PUBLIC Include)
target_link_libraries(LML Threads::Threads)

add_executable(LML_TEST Tests/Tests.cpp)
target_link_libraries(LML_TEST CATCH LML)

if (BUILD_DOC MATCHES "TRUE")
    doxygen_add_docs(doxygen)
endif ()