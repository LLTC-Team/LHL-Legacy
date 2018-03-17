project(LHL)
cmake_minimum_required(VERSION 3.1)

add_library(CATCH INTERFACE)
target_include_directories(CATCH INTERFACE Catch)

add_subdirectory(LML)
add_subdirectory(LVM)