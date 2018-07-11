project(LVM)
cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)

# force Unicode over Multi-byte
if(COMPILER_MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

aux_source_directory(Source MAIN_SRC_LIST)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
#Useless for develop environment
add_executable(LVM ${MAIN_SRC_LIST})
target_include_directories(LVM PUBLIC Include)

target_link_libraries(LVM Threads::Threads ${CMAKE_DL_LIBS})

add_subdirectory(TestLib)

add_library(LVM_lib STATIC ${MAIN_SRC_LIST})
target_include_directories(LVM_lib PUBLIC Include)

target_link_libraries(LVM_lib Threads::Threads ${CMAKE_DL_LIBS})

add_executable(LVMTest Tests/Tests.cpp)
target_link_libraries(LVMTest CATCH LVM_lib)