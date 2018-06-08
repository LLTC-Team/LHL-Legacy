project(LML)
cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)

# force Unicode over Multi-byte
if(COMPILER_MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

aux_source_directory(Source MAIN_SRC_LIST)
include_directories(Include)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
#Useless for develop environment
add_executable(LML ${MAIN_SRC_LIST})

target_link_libraries(LML Threads::Threads)

add_library(LML_lib STATIC ${MAIN_SRC_LIST})

target_link_libraries(LML_lib Threads::Threads)

add_executable(LMLTest Tests/Tests.cpp)
target_link_libraries(LMLTest CATCH LML_lib)
