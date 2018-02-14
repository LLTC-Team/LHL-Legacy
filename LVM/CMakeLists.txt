project(LVM)
cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
aux_source_directory(Source MAIN_SRC_LIST)
include_directories(Include)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
#Useless for develop environment
add_executable(LVM ${MAIN_SRC_LIST})