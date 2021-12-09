cmake_minimum_required(VERSION 3.15)
project(Lab8)

set(CMAKE_CXX_STANDARD 17)

MATH(EXPR stack_size "1024 * 1024 * 1024") # 256 Mb
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}") # Increase stack size

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1")

add_executable(Lab8 cache.cpp)
