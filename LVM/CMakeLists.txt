project(LVM)
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
add_executable(LVM ${MAIN_SRC_LIST})

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	target_link_libraries(LVM pthread)
endif()

add_library(LVM_lib STATIC ${MAIN_SRC_LIST})

add_executable(LVMTest Tests/Tests.cpp)
target_link_libraries(LVMTest CATCH LVM_lib)